/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scconfig.h"
#include "scstreamfilter_jpeg.h"

#include <cstdlib>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

extern "C"
{
#define XMD_H           // shut JPEGlib up
#if defined(Q_OS_UNIXWARE)
#  define HAVE_BOOLEAN  // libjpeg under Unixware seems to need this
#endif
#include <jpeglib.h>
#include <jerror.h>
#undef HAVE_STDLIB_H
#ifdef const
#  undef const          // remove crazy C hackery in jconfig.h
#endif
}

#define BUFFER_SIZE 65536

struct ScJpegErrorMgr
{
	struct  jpeg_error_mgr pub;            /* "public" fields */
	jmp_buf setjmp_buffer;  /* for return to caller */

	static void jpegErrorExit (j_common_ptr cinfo);
};

void ScJpegErrorMgr::jpegErrorExit (j_common_ptr cinfo)
{
	ScJpegErrorMgr* myerr = (ScJpegErrorMgr*) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp (myerr->setjmp_buffer, 1);
}

struct ScJpegDestinationMgr : public jpeg_destination_mgr 
{
    ScJpegEncodeFilter *filter;
    JOCTET buffer[BUFFER_SIZE];

public:
    ScJpegDestinationMgr(ScJpegEncodeFilter *);

	static boolean jpegEmptyBuffer(j_compress_ptr cinfo);
	static void    jpegDestinationInit(j_compress_ptr);
	static void    jpegDestinationTerm(j_compress_ptr cinfo);
};

void ScJpegDestinationMgr::jpegDestinationInit(j_compress_ptr)
{
}

boolean ScJpegDestinationMgr::jpegEmptyBuffer(j_compress_ptr cinfo)
{
    ScJpegDestinationMgr* dest = (ScJpegDestinationMgr*) cinfo->dest;

    bool written = dest->filter->writeDataInternal((const char*) dest->buffer, BUFFER_SIZE);
    if (!written)
        (*cinfo->err->error_exit)((j_common_ptr) cinfo);

    dest->next_output_byte = dest->buffer;
    dest->free_in_buffer   = BUFFER_SIZE;

    return boolean(true);
}

void ScJpegDestinationMgr::jpegDestinationTerm(j_compress_ptr cinfo)
{
    ScJpegDestinationMgr* dest = (ScJpegDestinationMgr*) cinfo->dest;
    int n = BUFFER_SIZE - dest->free_in_buffer;

    bool written = dest->filter->writeDataInternal((const char*) dest->buffer, BUFFER_SIZE);
    if (!written)
        (*cinfo->err->error_exit)((j_common_ptr)cinfo);
}

ScJpegDestinationMgr::ScJpegDestinationMgr(ScJpegEncodeFilter *filter)
{
	jpeg_destination_mgr::empty_output_buffer = jpegEmptyBuffer;
    jpeg_destination_mgr::init_destination    = jpegDestinationInit;
    jpeg_destination_mgr::term_destination    = jpegDestinationTerm;
    this->filter     = filter;
    next_output_byte = buffer;
    free_in_buffer   = BUFFER_SIZE;
}

struct ScJpegEncodeFilterData
{
	struct   jpeg_compress_struct  cinfo;
	struct   ScJpegDestinationMgr* cdest;
	JSAMPROW row_pointer[1];

	ScJpegEncodeFilterData();
	~ScJpegEncodeFilterData();
};

ScJpegEncodeFilterData::ScJpegEncodeFilterData() : cdest(NULL)
{
	row_pointer[0] = NULL;
}

ScJpegEncodeFilterData::~ScJpegEncodeFilterData()
{
	if (cdest)
		delete cdest;
	if (row_pointer[0])
		delete row_pointer[0];
}

ScJpegEncodeFilter::ScJpegEncodeFilter(QDataStream* stream, unsigned int imgWidth, unsigned int imgHeight, 
		            ScJpegEncodeFilter::Color color) : ScStreamFilter(stream), m_width(imgWidth), m_height(imgHeight),
					m_color(color), m_quality(75)
{
	m_filterData = NULL;
	m_openedFilter = false;
}

ScJpegEncodeFilter::ScJpegEncodeFilter(ScStreamFilter* filter, unsigned int imgWidth, unsigned int imgHeight, 
					ScJpegEncodeFilter::Color color) : ScStreamFilter(filter), m_width(imgWidth), m_height(imgHeight),
					m_color(color), m_quality(75)
{
	m_filterData = NULL;
	m_openedFilter = false;
}

ScJpegEncodeFilter::~ScJpegEncodeFilter()
{
	if (m_filterData && m_openedFilter)
		closeFilter();
	freeData();
}

void ScJpegEncodeFilter::freeData(void)
{
	if (m_filterData)
		delete m_filterData;
	m_filterData = NULL;
}

bool ScJpegEncodeFilter::openFilter (void)
{
	freeData();

	if (m_width == 0 || m_height == 0)
		return false;

	m_filterData = new ScJpegEncodeFilterData();
	if (m_filterData == NULL)
		return false;

	struct ScJpegErrorMgr jerr;
	jerr.pub.error_exit     = ScJpegErrorMgr::jpegErrorExit;
	m_filterData->cinfo.err = jpeg_std_error (&jerr.pub);
	m_filterData->row_pointer[0] = 0;

	m_filterData->cdest = new ScJpegDestinationMgr(this);
	if (m_filterData->cdest == NULL)
		return false;

	bool success = false;

	if (!setjmp (jerr.setjmp_buffer))
	{
		jpeg_create_compress (&m_filterData->cinfo);
		m_filterData->cinfo.dest         = m_filterData->cdest;
		m_filterData->cinfo.image_width  = m_width;
		m_filterData->cinfo.image_height = m_height;
		if (m_color == ScJpegEncodeFilter::RGB)
		{
			m_filterData->cinfo.in_color_space   = JCS_RGB;
			m_filterData->cinfo.input_components = 3;
		}
		else if (m_color == ScJpegEncodeFilter::CMYK)
		{
			m_filterData->cinfo.in_color_space   = JCS_CMYK;
			m_filterData->cinfo.input_components = 4;
		}
		else // m_color == ScJpegEncodeFilter::GRAY
		{
			m_filterData->cinfo.in_color_space   = JCS_GRAYSCALE;
			m_filterData->cinfo.input_components = 1;
		}
		jpeg_set_defaults  (&m_filterData->cinfo);
		jpeg_set_quality   (&m_filterData->cinfo, m_quality, true);
		jpeg_start_compress(&m_filterData->cinfo, true);
		success = true;
	}

	if (success)
	{
		m_filterData->row_pointer[0] = new uchar[m_filterData->cinfo.image_width * m_filterData->cinfo.input_components];
		if (m_filterData->row_pointer[0] == NULL)
			success = false;
	}
	if (!success || !ScStreamFilter::openFilter())
	{
		jpeg_destroy_compress (&m_filterData->cinfo);
		return false;
	}
	m_openedFilter = true;
	return true;
}

bool ScJpegEncodeFilter::closeFilter(void)
{
	bool closeSucceed = true;

	if (!m_filterData)
		return false;

	struct ScJpegErrorMgr jerr;
	jerr.pub.error_exit     = ScJpegErrorMgr::jpegErrorExit;
	m_filterData->cinfo.err = jpeg_std_error (&jerr.pub);

	if (!setjmp (jerr.setjmp_buffer))
	{
		jpeg_finish_compress (&m_filterData->cinfo);
		jpeg_destroy_compress(&m_filterData->cinfo);
	}
	else
	{
		closeSucceed = false;
	}
	m_openedFilter = false;
	return closeSucceed;
}

bool ScJpegEncodeFilter::writeData(const char* data, int dataLen)
{
	bool succeed = false;

	if ((!m_filterData) || (dataLen < 0))
		return false;
	if (dataLen == 0)
		return true;

	// For now dataLen must be a multiple of scanline width
	int scanLineSize = m_filterData->cinfo.image_width * m_filterData->cinfo.input_components;
	if ((dataLen % scanLineSize) != 0)
		return false;

	struct ScJpegErrorMgr jerr;
	jerr.pub.error_exit     = ScJpegErrorMgr::jpegErrorExit;
	m_filterData->cinfo.err = jpeg_std_error (&jerr.pub);

	if (!setjmp (jerr.setjmp_buffer))
	{
		int nScanLines = dataLen / scanLineSize;
		for (int i = 0; i  < nScanLines; ++i)
		{
			memcpy(m_filterData->row_pointer[0], data, scanLineSize);
			jpeg_write_scanlines (&m_filterData->cinfo, m_filterData->row_pointer, 1);
			data += scanLineSize;
		}
		succeed = true;
	}
	else
	{
		// Subsequent calls to writeData() or closeFilter() will return false
		jpeg_destroy_compress(&m_filterData->cinfo);
		freeData(); 
	}

	return succeed;
}
