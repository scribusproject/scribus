/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/* Code inspired by cairo and adapted for Scribus by Jean Ghali */

#include "scstreamfilter_flate.h"

#include <stdlib.h>
#include <zlib.h>

#include <QDataStream>

#define BUFFER_SIZE 16384
struct  ScFlateEncodeFilterData
{
    z_stream      zlib_stream;
    unsigned char input_buffer [BUFFER_SIZE];
    unsigned char output_buffer[BUFFER_SIZE];
};

ScFlateEncodeFilter::ScFlateEncodeFilter(QDataStream* stream)
				   : ScStreamFilter(stream)
{
	m_filterData   = NULL;
	m_openedFilter = false;
}

ScFlateEncodeFilter::ScFlateEncodeFilter(ScStreamFilter* filter)
				   : ScStreamFilter(filter)
{
	m_filterData   = NULL;
	m_openedFilter = false;
}

ScFlateEncodeFilter::~ScFlateEncodeFilter()
{
	if (m_filterData && m_openedFilter)
		closeFilter();
	freeData();
}

void ScFlateEncodeFilter::freeData(void)
{
	if (m_filterData)
		free (m_filterData);
	m_filterData = NULL;
}

bool ScFlateEncodeFilter::openFilter (void)
{
	freeData();

	m_filterData = (ScFlateEncodeFilterData*) malloc(sizeof(ScFlateEncodeFilterData));
	if (m_filterData == NULL)
		return false;

	m_filterData->zlib_stream.zalloc = Z_NULL;
	m_filterData->zlib_stream.zfree  = Z_NULL;
	m_filterData->zlib_stream.opaque = Z_NULL;
	
	if (deflateInit (&m_filterData->zlib_stream, Z_DEFAULT_COMPRESSION) != Z_OK)
	{
		freeData();
		return false;
	}

	m_filterData->zlib_stream.next_in   = m_filterData->input_buffer;
    m_filterData->zlib_stream.avail_in  = 0;
    m_filterData->zlib_stream.next_out  = m_filterData->output_buffer;
    m_filterData->zlib_stream.avail_out = BUFFER_SIZE;

	m_openedFilter = ScStreamFilter::openFilter();
	return m_openedFilter;
}
bool ScFlateEncodeFilter::closeFilter(void)
{
	bool closeSucceed = writeDeflate(true);
    deflateEnd (&m_filterData->zlib_stream);
	m_openedFilter = false;
	closeSucceed  &= ScStreamFilter::closeFilter();
	return closeSucceed;
}

bool ScFlateEncodeFilter::writeData(const char* data, int dataLen)
{
	bool deflateSuccess = true;
    unsigned int count;
    const unsigned char *p = (const unsigned char *) data;

	if (!m_filterData)
		return 0;

    while (dataLen) {
        count = dataLen;
        if (count > BUFFER_SIZE - m_filterData->zlib_stream.avail_in)
            count = BUFFER_SIZE - m_filterData->zlib_stream.avail_in;
        memcpy (m_filterData->input_buffer + m_filterData->zlib_stream.avail_in, p, count);
        p += count;
        m_filterData->zlib_stream.avail_in += count;
        dataLen -= count;

        if (m_filterData->zlib_stream.avail_in == BUFFER_SIZE)
            deflateSuccess &= writeDeflate(false);
    }

	return deflateSuccess;
}

bool ScFlateEncodeFilter::writeDeflate(bool flush)
{
	int  ret;
	bool deflateSuccess = true;
    bool finished;
	
	do {
		ret = deflate (&m_filterData->zlib_stream, flush ? Z_FINISH : Z_NO_FLUSH);
        if (flush || m_filterData->zlib_stream.avail_out == 0)
        {
			deflateSuccess &= writeDataInternal((const char*) m_filterData->output_buffer, BUFFER_SIZE - m_filterData->zlib_stream.avail_out);
            m_filterData->zlib_stream.next_out  = m_filterData->output_buffer;
            m_filterData->zlib_stream.avail_out = BUFFER_SIZE;
        }

        finished = TRUE;
        if (m_filterData->zlib_stream.avail_in != 0)
            finished = FALSE;
        if (flush && ret != Z_STREAM_END)
            finished = FALSE;

    } while (!finished);

    m_filterData->zlib_stream.next_in = m_filterData->input_buffer;
	return deflateSuccess;
}
