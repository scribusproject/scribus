/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <setjmp.h>
#include "scconfig.h"
#include "scimgdataloader_jpeg.h"
#include "exif.h"

#include CMS_INC

typedef struct my_error_mgr
{
	struct jpeg_error_mgr pub;            /* "public" fields */
	jmp_buf setjmp_buffer;  /* for return to caller */
} *my_error_ptr;

static void my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp (myerr->setjmp_buffer, 1);
}

#define ICC_MARKER  (JPEG_APP0 + 2)	/* JPEG marker code for ICC */
#define PHOTOSHOP_MARKER  (JPEG_APP0 + 13)	/* JPEG marker code for PHOTOSHOP */
#define ICC_OVERHEAD_LEN  14		/* size of non-profile data in APP2 */
#define MAX_BYTES_IN_MARKER  65533	/* maximum data len of a JPEG marker */
#define MAX_DATA_BYTES_IN_MARKER  (MAX_BYTES_IN_MARKER - ICC_OVERHEAD_LEN)


ScImgDataLoader_JPEG::ScImgDataLoader_JPEG(void)
{
	initSupportedFormatList();
}

void ScImgDataLoader_JPEG::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "jpg" );
	m_supportedFormats.append( "jpeg" );
}

void ScImgDataLoader_JPEG::loadEmbeddedProfile(const QString& fn)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
	if (!QFile::exists(fn))
		return;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr         jerr;
	FILE     *infile;
	cinfo.err = jpeg_std_error (&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	infile = NULL;
	if (setjmp (jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress (&cinfo);
		if (infile)
			fclose (infile);
		return;
	}
	jpeg_create_decompress (&cinfo);
	if ((infile = fopen (fn.toLocal8Bit(), "rb")) == NULL)
		return;
	jpeg_stdio_src(&cinfo, infile);
	jpeg_save_markers(&cinfo, ICC_MARKER, 0xFFFF);
	jpeg_read_header(&cinfo, true);
	//jpeg_start_decompress(&cinfo);
	unsigned int EmbedLen = 0;
	unsigned char* EmbedBuffer;
	if (read_jpeg_marker(ICC_MARKER,&cinfo, &EmbedBuffer, &EmbedLen))
	{
		cmsHPROFILE prof = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
		if (prof)
		{
			if (static_cast<int>(cmsGetColorSpace(prof)) == icSigRgbData)
				m_profileComponents = 3;
			if (static_cast<int>(cmsGetColorSpace(prof)) == icSigCmykData)
				m_profileComponents = 4;
			m_embeddedProfile = QByteArray((const char*) EmbedBuffer, EmbedLen);
		}
		cmsCloseProfile(prof);
		free(EmbedBuffer);
	}
	//(void) jpeg_finish_decompress(&cinfo);
	fclose (infile);
	jpeg_destroy_decompress (&cinfo);
}

bool ScImgDataLoader_JPEG::preloadAlphaChannel(const QString& fn, int res, bool& hasAlpha)
{
	// No support for aplha in jpeg pictures
	initialize();
	hasAlpha = false;
	return true;
}

bool ScImgDataLoader_JPEG::loadPicture(const QString& fn, int res, bool thumbnail)
{
	bool isCMYK = false;
	bool fromPS = false;
	float xres = 72.0, yres = 72.0;
	if (!QFile::exists(fn))
		return false;
	ExifData ExifInf;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr         jerr;
	FILE     *infile;
	cinfo.err = jpeg_std_error (&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	infile = NULL;

	initialize();
	m_imageInfoRecord.type = ImageTypeJPG;
	m_imageInfoRecord.exifInfo.thumbnail = QImage();
	if (setjmp (jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress (&cinfo);
		if (infile)
			fclose (infile);
		return false;
	}
	jpeg_create_decompress (&cinfo);
	if ((infile = fopen (fn.toLocal8Bit(), "rb")) == NULL)
		return false;
	jpeg_stdio_src(&cinfo, infile);
	jpeg_save_markers(&cinfo, ICC_MARKER, 0xFFFF);
	jpeg_save_markers(&cinfo, PHOTOSHOP_MARKER, 0xFFFF);
	jpeg_read_header(&cinfo, true);
	jpeg_start_decompress(&cinfo);
	bool exi = ExifInf.scan(fn);
	if ((exi) && (ExifInf.exifDataValid))
	{
		if (cinfo.output_components == 4)
			m_imageInfoRecord.colorspace = ColorSpaceCMYK;
		else if (cinfo.output_components == 3)
			m_imageInfoRecord.colorspace = ColorSpaceRGB;
		else if (cinfo.output_components == 1)
			m_imageInfoRecord.colorspace = ColorSpaceGray;
		if ((!ExifInf.Thumbnail.isNull()) && thumbnail)
		{
			m_image = ExifInf.getThumbnail();
			m_imageInfoRecord.exifInfo.thumbnail = ExifInf.getThumbnail();
			if (cinfo.output_components == 4)
			{
				QRgb *s;
				unsigned char cc, cm, cy, ck;
				for( int yit=0; yit < m_image.height(); ++yit )
				{
					s = (QRgb*)(m_image.scanLine( yit ));
					for(int xit=0; xit < m_image.width(); ++xit )
					{
						cc = 255 - qRed(*s);
						cm = 255 - qGreen(*s);
						cy = 255 - qBlue(*s);
						ck = qMin(qMin(cc, cm), cy);
						*s = qRgba(cc-ck,cm-ck,cy-ck,ck);
						s++;
					}
				}
			}
		}
		else
			m_imageInfoRecord.exifInfo.thumbnail = QImage();
		m_imageInfoRecord.exifInfo.cameraName = ExifInf.getCameraModel();
		m_imageInfoRecord.exifInfo.cameraVendor = ExifInf.getCameraMake();
		m_imageInfoRecord.exifInfo.comment = ExifInf.getComment();
		m_imageInfoRecord.exifInfo.width = ExifInf.getWidth();
		m_imageInfoRecord.exifInfo.height = ExifInf.getHeight();
		m_imageInfoRecord.exifInfo.userComment = ExifInf.getUserComment();
		m_imageInfoRecord.exifInfo.dateTime = ExifInf.getDateTime();
		m_imageInfoRecord.exifInfo.ApertureFNumber = ExifInf.getApertureFNumber();
		m_imageInfoRecord.exifInfo.ExposureTime = ExifInf.getExposureTime();
		m_imageInfoRecord.exifInfo.ISOequivalent = ExifInf.getISOequivalent();
		m_imageInfoRecord.exifDataValid = true;
		if (cinfo.density_unit == 0)
		{
			xres = 72;
			yres = 72;
		}
		else if ( cinfo.density_unit == 1 )
		{
			xres = cinfo.X_density;
			yres = cinfo.Y_density;
		}
		else if ( cinfo.density_unit == 2 )
		{
			xres = cinfo.X_density * 2.54;
			yres = cinfo.Y_density * 2.54;
		}
		if( xres <= 1.0 || yres <= 1.0 || xres > 3000.0 || yres > 3000.0 )
		{
			xres = yres = 72.0;
			QFileInfo qfi(fn);
			m_message = QObject::tr("%1 may be corrupted : missing or wrong resolution tags").arg(qfi.fileName());
			m_msgType = warningMsg;
		}
		m_imageInfoRecord.xres = qRound(xres);
		m_imageInfoRecord.yres = qRound(yres);
		m_imageInfoRecord.progressive = jpeg_has_multiple_scans(&cinfo);
		if ((!ExifInf.Thumbnail.isNull()) && thumbnail)
		{
			jpeg_destroy_decompress(&cinfo);
			fclose(infile);
			return true;
		}
	}
	else
		m_imageInfoRecord.exifDataValid = false;
	m_imageInfoRecord.exifInfo.thumbnail = QImage();
	unsigned int EmbedLen = 0;
	unsigned char* EmbedBuffer;
	if (read_jpeg_marker(ICC_MARKER,&cinfo, &EmbedBuffer, &EmbedLen))
	{
		const char *Descriptor;
		cmsHPROFILE prof = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
		Descriptor = cmsTakeProductDesc(prof);
		m_embeddedProfile = QByteArray((const char*) EmbedBuffer, EmbedLen);
		m_imageInfoRecord.profileName = QString(Descriptor);
		m_imageInfoRecord.isEmbedded = true;
		free(EmbedBuffer);
	}
	else
	{
		m_imageInfoRecord.isEmbedded = false;
		m_imageInfoRecord.profileName = "";
	}
	unsigned int PhotoshopLen = 0;
	unsigned char * PhotoshopBuffer;
	if (cinfo.density_unit == 0)
	{
		xres = 72;
		yres = 72;
		m_image.setDotsPerMeterX(2834);
		m_image.setDotsPerMeterY(2834);
	}
	else if ( cinfo.density_unit == 1 )
	{
		xres = cinfo.X_density;
		yres = cinfo.Y_density;
		m_image.setDotsPerMeterX( int(100. * cinfo.X_density / 2.54) );
		m_image.setDotsPerMeterY( int(100. * cinfo.Y_density / 2.54) );
	}
	else if ( cinfo.density_unit == 2 )
	{
		xres = cinfo.X_density * 2.54;
		yres = cinfo.Y_density * 2.54;
		m_image.setDotsPerMeterX( int(100. * cinfo.X_density) );
		m_image.setDotsPerMeterY( int(100. * cinfo.Y_density) );
	}
	if( xres <= 1.0 || yres <= 1.0 || xres > 3000.0 || yres > 3000.0 )
	{
		xres = yres = 72.0;
		m_image.setDotsPerMeterX(2834);
		m_image.setDotsPerMeterY(2834);
		QFileInfo qfi(fn);
		m_message = QObject::tr("%1 may be corrupted : missing or wrong resolution tags").arg(qfi.fileName());
		m_msgType = warningMsg;
	}
	m_imageInfoRecord.xres = qRound(xres);
	m_imageInfoRecord.yres = qRound(yres);
	if (cinfo.output_components == 4)
	{
		isCMYK = true;
		m_imageInfoRecord.colorspace = ColorSpaceCMYK;
	}
	else if (cinfo.output_components == 3)
		m_imageInfoRecord.colorspace = ColorSpaceRGB;
	else if (cinfo.output_components == 1)
		m_imageInfoRecord.colorspace = ColorSpaceGray;
	m_imageInfoRecord.progressive = jpeg_has_multiple_scans(&cinfo);

	if (read_jpeg_marker(PHOTOSHOP_MARKER,&cinfo, &PhotoshopBuffer, &PhotoshopLen) )
	{
		if (PhotoshopLen != 0)
		{
			bool savEx = m_imageInfoRecord.exifDataValid;
			QByteArray arrayPhot(PhotoshopLen, ' ');
			arrayPhot = QByteArray::fromRawData((const char*)PhotoshopBuffer,PhotoshopLen);
			QDataStream strPhot(&arrayPhot,QIODevice::ReadOnly);
			strPhot.setByteOrder( QDataStream::BigEndian );
			PSDHeader fakeHeader;
			fakeHeader.width = cinfo.output_width;
			fakeHeader.height = cinfo.output_height;
			if (cinfo.output_components == 4)
				m_imageInfoRecord.colorspace = ColorSpaceCMYK;
			else if (cinfo.output_components == 3)
				m_imageInfoRecord.colorspace = ColorSpaceRGB;
			else if (cinfo.output_components == 1)
				m_imageInfoRecord.colorspace = ColorSpaceGray;
			m_imageInfoRecord.progressive = jpeg_has_multiple_scans(&cinfo);
			parseRessourceData(strPhot, fakeHeader, PhotoshopLen);
			// Photoshop resolution is more accurate than jpeg header resolution
			xres = m_imageInfoRecord.xres;
			yres = m_imageInfoRecord.yres;
			m_image.setDotsPerMeterX( int(100. * m_imageInfoRecord.xres / 2.54) );
			m_image.setDotsPerMeterY( int(100. * m_imageInfoRecord.yres / 2.54) );
			if( xres <= 1.0 || yres <= 1.0 || xres > 3000.0 || yres > 3000.0 )
			{
				xres = yres = 72.0;
				m_imageInfoRecord.xres = qRound(xres);
				m_imageInfoRecord.yres = qRound(yres);
				m_image.setDotsPerMeterX(2834);
				m_image.setDotsPerMeterY(2834);
				QFileInfo qfi(fn);
				m_message = QObject::tr("%1 may be corrupted : missing or wrong resolution tags").arg(qfi.fileName());
				m_msgType = warningMsg;
			}
			if (m_imageInfoRecord.exifDataValid && thumbnail)
			{
				m_image = QImage(m_imageInfoRecord.exifInfo.width, m_imageInfoRecord.exifInfo.height, QImage::Format_ARGB32 );
				m_imageInfoRecord.exifInfo.width = cinfo.output_width;
				m_imageInfoRecord.exifInfo.height = cinfo.output_height;
				if (cinfo.output_components == 4)
				{
					QRgb *d;
					QRgb *s;
					unsigned char cc, cm, cy, ck;
					for( int yit=0; yit < m_image.height(); ++yit )
					{
						d = (QRgb*)(m_image.scanLine( yit ));
						s = (QRgb*)(m_imageInfoRecord.exifInfo.thumbnail.scanLine( yit ));
						for(int xit=0; xit < m_image.width(); ++xit )
						{
							cc = 255 - qRed(*s);
							cm = 255 - qGreen(*s);
							cy = 255 - qBlue(*s);
							ck = qMin(qMin(cc, cm), cy);
							*d = qRgba(cc-ck,cm-ck,cy-ck,ck);
							s++;
							d++;
						}
					}
				}
				else
					m_image = m_imageInfoRecord.exifInfo.thumbnail.copy();
			}
			m_imageInfoRecord.valid = (m_imageInfoRecord.PDSpathData.size())>0?true:false; // The only interest is vectormask
			arrayPhot.clear();
			free( PhotoshopBuffer );
			if (m_imageInfoRecord.exifDataValid && thumbnail)
			{
				jpeg_destroy_decompress(&cinfo);
				fclose(infile);
				return true;
			}
			m_imageInfoRecord.exifInfo.thumbnail = QImage();
			m_imageInfoRecord.exifDataValid = savEx;
			fromPS = true;
		}
	}
	if ( cinfo.output_components == 3 || cinfo.output_components == 4)
		m_image = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_ARGB32 );
	else if ( cinfo.output_components == 1 )
	{
		m_image = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_Indexed8 );
		m_image.setNumColors(256);
		for (int i=0; i<256; i++)
			m_image.setColor(i, qRgb(i,i,i));
	}
	if (!m_image.isNull())
	{
		uchar* data = m_image.bits();
		int bpl = m_image.bytesPerLine();
		while (cinfo.output_scanline < cinfo.output_height)
		{
			uchar *d = data + cinfo.output_scanline * bpl;
			(void) jpeg_read_scanlines(&cinfo, &d, 1);
		}
		if ( cinfo.output_components == 3 )
		{
			uchar *in;
			QRgb *out;
			for (uint j=0; j<cinfo.output_height; j++)
			{
				in = m_image.scanLine(j) + cinfo.output_width * 3;
				out = (QRgb*) m_image.scanLine(j);
				for (uint i=cinfo.output_width; i--; )
				{
					in -= 3;
					out[i] = qRgb(in[0], in[1], in[2]);
				}
			}
		}
		if ( cinfo.output_components == 4 )
		{
			int method = 0;
			if (cinfo.jpeg_color_space == JCS_YCCK)
				method = 1;
			else if (fromPS)
			{
				if ((cinfo.jpeg_color_space == JCS_CMYK) && (cinfo.saw_Adobe_marker) && (cinfo.Adobe_transform == 0))
					method = 2;
			}
			else if ((cinfo.jpeg_color_space == JCS_CMYK) && (cinfo.saw_Adobe_marker))
				method = 1;
			QRgb *ptr;
			unsigned char c, m, y ,k;
			unsigned char *p;
			for (int i = 0; i < m_image.height(); i++)
			{
				ptr = (QRgb*)  m_image.scanLine(i);
				if (method == 1)
				{
					for (int j = 0; j <  m_image.width(); j++)
					{
						p = (unsigned char *) ptr;
						c = p[0];
						m = p[1];
						y =  p[2];
						k =  p[3];
						*ptr = qRgba(255 - c, 255 - m, 255 - y, 255 - k);
						ptr++;
					}
				}
				else if (method == 2)
				{
					for (int j = 0; j <  m_image.width(); j++)
					{
						p = (unsigned char *) ptr;
						c = p[0];
						m = p[1];
						y =  p[2];
						k =  p[3];
						*ptr = qRgba(255 - c, 255 - m, 255 - y, k);
						ptr++;
					}
				}
				else
				{
					for (int j = 0; j <  m_image.width(); j++)
					{
						p = (unsigned char *) ptr;
						c = p[0];
						m = p[1];
						y =  p[2];
						k =  p[3];
						*ptr = qRgba(y, m, c, k);
						ptr++;
					}
				}
			}
			isCMYK = true;
		}
		else
			isCMYK = false;
		if ( cinfo.output_components == 1 )
		{
			QImage tmpImg = m_image.convertToFormat(QImage::Format_ARGB32);
			m_image = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_ARGB32 );
			QRgb *s;
			QRgb *d;
			for( int yi=0; yi < tmpImg.height(); ++yi )
			{
				s = (QRgb*)(tmpImg.scanLine( yi ));
				d = (QRgb*)(m_image.scanLine( yi ));
				for(int xi=0; xi < tmpImg.width(); ++xi )
				{
					(*d) = (*s);
					s++;
					d++;
				}
			}
		}
	}
	(void) jpeg_finish_decompress(&cinfo);
	fclose (infile);
	jpeg_destroy_decompress (&cinfo);
	m_imageInfoRecord.layerInfo.clear();
	m_imageInfoRecord.BBoxX = 0;
	m_imageInfoRecord.BBoxH = m_image.height();
	return (!m_image.isNull());
}

bool ScImgDataLoader_JPEG::marker_is_icc (jpeg_saved_marker_ptr marker)
{
	return
	    marker->marker == ICC_MARKER &&
	    marker->data_length >= ICC_OVERHEAD_LEN &&
	    /* verify the identifying string */
	    GETJOCTET(marker->data[0]) == 0x49 &&
	    GETJOCTET(marker->data[1]) == 0x43 &&
	    GETJOCTET(marker->data[2]) == 0x43 &&
	    GETJOCTET(marker->data[3]) == 0x5F &&
	    GETJOCTET(marker->data[4]) == 0x50 &&
	    GETJOCTET(marker->data[5]) == 0x52 &&
	    GETJOCTET(marker->data[6]) == 0x4F &&
	    GETJOCTET(marker->data[7]) == 0x46 &&
	    GETJOCTET(marker->data[8]) == 0x49 &&
	    GETJOCTET(marker->data[9]) == 0x4C &&
	    GETJOCTET(marker->data[10]) == 0x45 &&
	    GETJOCTET(marker->data[11]) == 0x0;
}

bool ScImgDataLoader_JPEG::marker_is_photoshop (jpeg_saved_marker_ptr marker)
{
	return
	    marker->marker == PHOTOSHOP_MARKER &&
	    marker->data_length >= ICC_OVERHEAD_LEN &&
	    /* verify the identifying string */
	    GETJOCTET(marker->data[0]) == 0x50 &&
	    GETJOCTET(marker->data[1]) == 0x68 &&
	    GETJOCTET(marker->data[2]) == 0x6F &&
	    GETJOCTET(marker->data[3]) == 0x74 &&
	    GETJOCTET(marker->data[4]) == 0x6F &&
	    GETJOCTET(marker->data[5]) == 0x73 &&
	    GETJOCTET(marker->data[6]) == 0x68 &&
	    GETJOCTET(marker->data[7]) == 0x6F &&
	    GETJOCTET(marker->data[8]) == 0x70 &&
	    GETJOCTET(marker->data[9]) == 0x20 &&
	    GETJOCTET(marker->data[10]) == 0x33 &&
	    GETJOCTET(marker->data[11]) == 0x2E &&
	    GETJOCTET(marker->data[12]) == 0x30 &&
	    GETJOCTET(marker->data[13]) == 0x0;
}

/* Small modification of original read_icc_profile method from jpegicc of lcms project
 * to enable read of Photoshop marker
 */
bool ScImgDataLoader_JPEG::read_jpeg_marker (UINT8 requestmarker, j_decompress_ptr cinfo, JOCTET **icc_data_ptr, unsigned int *icc_data_len)
{
	jpeg_saved_marker_ptr marker;
	int num_markers = 0;
	int seq_no;
	JOCTET *icc_data;
	unsigned int total_length;
#define MAX_SEQ_NO  255		/* sufficient since marker numbers are bytes */
	char marker_present[MAX_SEQ_NO+1];	  /* 1 if marker found */
	unsigned int data_length[MAX_SEQ_NO+1]; /* size of profile data in marker */
	unsigned int data_offset[MAX_SEQ_NO+1]; /* offset for data in marker */

	*icc_data_ptr = NULL;		/* avoid confusion if false return */
	*icc_data_len = 0;

	/* This first pass over the saved markers discovers whether there are
	 * any ICC markers and verifies the consistency of the marker numbering.
	 */

	for (seq_no = 1; seq_no <= MAX_SEQ_NO; seq_no++)
		marker_present[seq_no] = 0;
	seq_no = 0;
	for (marker = cinfo->marker_list; marker != NULL; marker = marker->next)
	{
		if (requestmarker == ICC_MARKER && marker_is_icc(marker))
		{
			if (num_markers == 0)
				num_markers = GETJOCTET(marker->data[13]);
			else if (num_markers != GETJOCTET(marker->data[13]))
				return false;		/* inconsistent num_markers fields */
			seq_no = GETJOCTET(marker->data[12]);
			if (seq_no <= 0 || seq_no > num_markers)
				return false;		/* bogus sequence number */
			if (marker_present[seq_no])
				return false;		/* duplicate sequence numbers */
			marker_present[seq_no] = 1;
			data_length[seq_no] = marker->data_length - ICC_OVERHEAD_LEN;
		}
		else if(requestmarker == PHOTOSHOP_MARKER && marker_is_photoshop(marker))
		{
			num_markers = ++seq_no;
			marker_present[seq_no] = 1;
			data_length[seq_no] = marker->data_length - ICC_OVERHEAD_LEN;
		}
	}

	if (num_markers == 0)
		return false;

	/* Check for missing markers, count total space needed,
	 * compute offset of each marker's part of the data.
	 */

	total_length = 0;
	for (seq_no = 1; seq_no <= num_markers; seq_no++)
	{
		if (marker_present[seq_no] == 0)
			return false;		/* missing sequence number */
		data_offset[seq_no] = total_length;
		total_length += data_length[seq_no];
	}

	if (total_length <= 0)
		return false;		/* found only empty markers? */

	/* Allocate space for assembled data */
	icc_data = (JOCTET *) malloc(total_length * sizeof(JOCTET));
	if (icc_data == NULL)
		return false;		/* oops, out of memory */
	seq_no=0;
	/* and fill it in */
	for (marker = cinfo->marker_list; marker != NULL; marker = marker->next)
	{
		if ( (requestmarker == ICC_MARKER && marker_is_icc(marker)) ||
		        (requestmarker == PHOTOSHOP_MARKER && marker_is_photoshop(marker)) || (requestmarker == 0xE1))
		{
			JOCTET FAR *src_ptr;
			JOCTET *dst_ptr;
			unsigned int length;
			if(requestmarker == ICC_MARKER)
				seq_no = GETJOCTET(marker->data[12]);
			else if(requestmarker == PHOTOSHOP_MARKER)
				seq_no++;
			dst_ptr = icc_data + data_offset[seq_no];
			src_ptr = marker->data + ICC_OVERHEAD_LEN;
			length = data_length[seq_no];
			while (length--)
			{
				*dst_ptr++ = *src_ptr++;
			}
		}
	}

	*icc_data_ptr = icc_data;
	*icc_data_len = total_length;

	return true;
}
