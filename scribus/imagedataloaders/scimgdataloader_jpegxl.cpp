/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.

Reference implementation with BSD style licenced code used from
https://github.com/libjxl/libjxl/blob/main/examples/decode_oneshot.cc


*/

#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QImageIOHandler>

#include <math.h>
#include <fstream>
#include <inttypes.h>
#include <iterator>
#include <string>
#include <vector>


#include <jxl/resizable_parallel_runner.h>
#include <jxl/resizable_parallel_runner_cxx.h>

#include "scconfig.h"
#include "scimgdataloader_jpegxl.h"
#include "scribuscore.h"
#include "exif.h"

ScImgDataLoader_JPEGXL::ScImgDataLoader_JPEGXL()
{
	initSupportedFormatList();
}

void ScImgDataLoader_JPEGXL::initSupportedFormatList()
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "jxl" );
}

void ScImgDataLoader_JPEGXL::loadEmbeddedProfile(const QString& fn, int /*page*/)
{

}

bool ScImgDataLoader_JPEGXL::preloadAlphaChannel(const QString& fn, int /*page*/, int res, bool& hasAlpha)
{
	// No support for alpha in jpeg pictures
	initialize();
	hasAlpha = false;
	return true;
}

bool ScImgDataLoader_JPEGXL::loadPicture(const QString& fn, int /*page*/, int res, bool thumbnail)
{
	if (!QFile::exists(fn))
		return false;
	initialize();
	std::vector<uint8_t> jxl;
	if (!loadFile(fn.toLocal8Bit(), &jxl))
	{
		qDebug() << "Couldn't load " << fn;
		return false;
	}

	std::vector<float> pixels;
	size_t xsize = 0, ysize = 0;
	QByteArray icc_data;
	if (!decodeJpegXlOneShot(jxl.data(), jxl.size(), &pixels, &xsize, &ysize, &icc_data))
	{
		qDebug() << "Error while decoding the jxl file " << fn;
		return false;
	}
	if (m_basicinfo.have_animation)
	{
		qDebug() << "JXL Animations not supported";
		return false;
	}
	qDebug()<<"pixels size"<<pixels.size();
	qDebug()<<"jxl size"<<jxl.size();
//	bool loadalpha = (m_basicinfo.alpha_bits > 0) ? true : false;

	qDebug()<<m_basicinfo.xsize;
	qDebug()<<m_basicinfo.ysize;
	qDebug()<<m_basicinfo.alpha_bits;
	qDebug()<<m_basicinfo.orientation;
	m_embeddedProfile =  icc_data;
	JxlPixelFormat m_input_pixel_format;
	m_input_pixel_format.endianness = JXL_NATIVE_ENDIAN;
	m_input_pixel_format.align = 0;
	m_input_pixel_format.num_channels = 4;


	if (m_basicinfo.bits_per_sample > 8)
	{ // high bit depth
		m_input_pixel_format.data_type = JXL_TYPE_UINT16;
//		m_buffer_size = 8 * (size_t)m_basicinfo.xsize * (size_t)m_basicinfo.ysize;
		m_input_image_format = QImage::Format_RGBA64;

//		if (loadalpha)
//			m_target_image_format = QImage::Format_RGBA64;
//		else
//			m_target_image_format = QImage::Format_RGBX64;
	}
	else
	{ // 8bit depth
		m_input_pixel_format.data_type = JXL_TYPE_UINT8;
//		m_buffer_size = 4 * (size_t)m_basicinfo.xsize * (size_t)m_basicinfo.ysize;
		m_input_image_format = QImage::Format_RGBA8888;

//		if (loadalpha)
//			m_target_image_format = QImage::Format_ARGB32;
//		else
//			m_target_image_format = QImage::Format_RGB32;
	}

//	m_image = tmpImage;
	m_image = QImage( xsize, ysize, m_input_image_format );

	int i = 0;
	for (int y = 0; y < m_image.height(); ++y)
	{
		QRgb *line = reinterpret_cast<QRgb*>(m_image.scanLine(y));
		for (int x = 0; x < m_image.width(); ++x)
		{
			QRgb &rgb = line[x];
			rgb = qRgba(pixels.at(i)*255,
						pixels.at(i+1)*255,
						pixels.at(i+2)*255,
						pixels.at(i+3)*255);
			i+=4;
		}
	}

	return true;
}


bool ScImgDataLoader_JPEGXL::loadFile(const char *filename, std::vector<uint8_t> *out)
{
	FILE* file = fopen(filename, "rb");
	if (!file)
	{
		return false;
	}

	if (fseek(file, 0, SEEK_END) != 0)
	{
		fclose(file);
		return false;
	}

	long size = ftell(file);
	// Avoid invalid file or directory.
	if (size >= LONG_MAX || size < 0)
	{
		fclose(file);
		return false;
	}

	if (fseek(file, 0, SEEK_SET) != 0)
	{
		fclose(file);
		return false;
	}

	out->resize(size);
	size_t readsize = fread(out->data(), 1, size, file);
	if (fclose(file) != 0)
	{
		return false;
	}

	return readsize == static_cast<size_t>(size);
}

bool ScImgDataLoader_JPEGXL::decodeJpegXlOneShot(const uint8_t *jxl, size_t size, std::vector<float> *pixels, size_t *xsize, size_t *ysize, QByteArray* icc_profile)
{
	// Multi-threaded parallel runner.
	auto runner = JxlResizableParallelRunnerMake(nullptr);

	auto dec = JxlDecoderMake(nullptr);
	if (JXL_DEC_SUCCESS !=
		JxlDecoderSubscribeEvents(dec.get(), JXL_DEC_BASIC_INFO |
												 JXL_DEC_COLOR_ENCODING |
												 JXL_DEC_FULL_IMAGE)) {
	  fprintf(stderr, "JxlDecoderSubscribeEvents failed\n");
	  return false;
	}

	if (JXL_DEC_SUCCESS != JxlDecoderSetParallelRunner(dec.get(),
													   JxlResizableParallelRunner,
													   runner.get())) {
		fprintf(stderr, "JxlDecoderSetParallelRunner failed\n");
		return false;
	}

	JxlBasicInfo info;
	JxlPixelFormat format = {4, JXL_TYPE_FLOAT, JXL_NATIVE_ENDIAN, 0};

	JxlDecoderSetInput(dec.get(), jxl, size);
	JxlDecoderCloseInput(dec.get());

	for (;;)
	{
		JxlDecoderStatus status = JxlDecoderProcessInput(dec.get());

		if (status == JXL_DEC_ERROR)
		{
			fprintf(stderr, "Decoder error\n");
			return false;
		}
		else if (status == JXL_DEC_NEED_MORE_INPUT)
		{
			fprintf(stderr, "Error, already provided all input\n");
			return false;
		}
		else if (status == JXL_DEC_BASIC_INFO)
		{
			if (JXL_DEC_SUCCESS != JxlDecoderGetBasicInfo(dec.get(), &info))
			{
				fprintf(stderr, "JxlDecoderGetBasicInfo failed\n");
				return false;
			}
			*xsize = info.xsize;
			*ysize = info.ysize;
			JxlResizableParallelRunnerSetThreads(
						runner.get(),
						JxlResizableParallelRunnerSuggestThreads(info.xsize, info.ysize));
		}
		else if (status == JXL_DEC_COLOR_ENCODING)
		{
			// Get the ICC color profile of the pixel data
			size_t icc_size;
			if (JXL_DEC_SUCCESS !=
					JxlDecoderGetICCProfileSize(
						dec.get(), &format, JXL_COLOR_PROFILE_TARGET_DATA, &icc_size))
			{
				fprintf(stderr, "JxlDecoderGetICCProfileSize failed\n");
				return false;
			}
			icc_profile->resize(icc_size);
			icc_profile->fill(0);
			if (JXL_DEC_SUCCESS != JxlDecoderGetColorAsICCProfile(
						dec.get(), &format,
						JXL_COLOR_PROFILE_TARGET_DATA,
						reinterpret_cast<uint8_t *>(icc_profile->data()), icc_profile->size()))
			{
				fprintf(stderr, "JxlDecoderGetColorAsICCProfile failed\n");
				return false;
			}
		}
		else if (status == JXL_DEC_NEED_IMAGE_OUT_BUFFER)
		{
			size_t buffer_size;
			if (JXL_DEC_SUCCESS !=
					JxlDecoderImageOutBufferSize(dec.get(), &format, &buffer_size))
			{
				fprintf(stderr, "JxlDecoderImageOutBufferSize failed\n");
				return false;
			}

			if (buffer_size != *xsize * *ysize * 16)
			{
				fprintf(stderr, "Invalid out buffer size %" PRIu64 " %" PRIu64 "\n",
						static_cast<uint64_t>(buffer_size),
						static_cast<uint64_t>(*xsize * *ysize * 16));
				return false;
			}

			if (!QImageIOHandler::allocateImage(QSize(*xsize, *ysize), QImage::Format_RGBA8888, &tmpImage))
					tmpImage = QImage(); // paranoia


//			if (tmpImage.isNull())
//			{
//				qWarning("Memory cannot be allocated");
//				return false;
//			}
			// correct to resize this?
			int multiplier = (m_basicinfo.bits_per_sample > 8) ? 8 : 4;
			pixels->resize(*xsize * *ysize * multiplier);
			void* pixels_buffer = (void*)pixels->data();
			size_t pixels_buffer_size = pixels->size() * sizeof(float);

//			if (JXL_DEC_SUCCESS != JxlDecoderSetImageOutBuffer(dec.get(), &format,
//															   tmpImage.bits(),
//															   pixels_buffer_size))
			if (JXL_DEC_SUCCESS != JxlDecoderSetImageOutBuffer(dec.get(), &format,
															   pixels_buffer,
															   pixels_buffer_size))
			{
				fprintf(stderr, "JxlDecoderSetImageOutBuffer failed\n");
				return false;
			}
		}
		else if (status == JXL_DEC_FULL_IMAGE)
		{
			// Nothing to do. Do not yet return. If the image is an animation, more
			// full frames may be decoded. This example only keeps the last one.
		}
		else if (status == JXL_DEC_SUCCESS)
		{
			m_basicinfo = info;

			// All decoding successfully finished.
			// It's not required to call JxlDecoderReleaseInput(dec.get()) here since
			// the decoder will be destroyed.
			return true;
		}
		else
		{
			fprintf(stderr, "Unknown decoder status\n");
			return false;
		}
	}
}
