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

#include <inttypes.h>
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

	std::vector<uint8_t> pixels;
	QByteArray icc_data;
	JXLImageInfo imageInfo;
	if (!decodeJpegXlOneShot(jxl.data(), jxl.size(), &pixels, &imageInfo, &icc_data))
	{
		qDebug() << "Error while decoding the jxl file " << fn;
		return false;
	}

	m_embeddedProfile =  icc_data;

	if (imageInfo.bits_per_sample > 8)
	{
		// 16-bit path
		QImage::Format imageFormat;
		if (imageInfo.channels == 3)
			imageFormat = QImage::Format_RGBX64;  // No RGB48 in Qt, use RGBX64
		else
			imageFormat = QImage::Format_RGBA64;

		m_image = QImage(imageInfo.width, imageInfo.height, imageFormat);

		const uint16_t* src = reinterpret_cast<const uint16_t*>(pixels.data());
		int i = 0;
		for (int y = 0; y < m_image.height(); ++y)
		{
			quint16* line = reinterpret_cast<quint16*>(m_image.scanLine(y));
			if (imageInfo.channels == 3)
			{
				for (int x = 0; x < m_image.width(); ++x)
				{
					line[x * 4 + 0] = src[i + 0];  // R
					line[x * 4 + 1] = src[i + 1];  // G
					line[x * 4 + 2] = src[i + 2];  // B
					line[x * 4 + 3] = 0xFFFF;      // A (fully opaque)
					i += 3;
				}
			}
			else
			{
				for (int x = 0; x < m_image.width(); ++x)
				{
					line[x * 4 + 0] = src[i + 0];  // R
					line[x * 4 + 1] = src[i + 1];  // G
					line[x * 4 + 2] = src[i + 2];  // B
					line[x * 4 + 3] = src[i + 3];  // A
					i += 4;
				}
			}
		}
	}
	else
	{
		// 8-bit path
		QImage::Format imageFormat;
		if (imageInfo.channels == 3)
			imageFormat = QImage::Format_RGB888;
		else
			imageFormat = QImage::Format_RGBA8888;

		m_image = QImage(imageInfo.width, imageInfo.height, imageFormat);

		int i = 0;
		for (int y = 0; y < m_image.height(); ++y)
		{
			if (imageInfo.channels == 3)
			{
				uchar *line = m_image.scanLine(y);
				for (int x = 0; x < m_image.width(); ++x)
				{
					line[x * 3 + 0] = pixels.at(i + 0); // R
					line[x * 3 + 1] = pixels.at(i + 1); // G
					line[x * 3 + 2] = pixels.at(i + 2); // B
					i += 3;
				}
			}
			else
			{
				uchar *line = m_image.scanLine(y);
				for (int x = 0; x < m_image.width(); ++x)
				{
					line[x * 4 + 0] = pixels.at(i + 0); // R
					line[x * 4 + 1] = pixels.at(i + 1); // G
					line[x * 4 + 2] = pixels.at(i + 2); // B
					line[x * 4 + 3] = pixels.at(i + 3); // A
					i += 4;
				}
			}
		}
	}

	// Convert to ARGB32 for Scribus
	m_image = m_image.convertToFormat(QImage::Format_ARGB32);

	m_imageInfoRecord.xres = 72;
	m_imageInfoRecord.yres = 72;
	m_imageInfoRecord.BBoxX = 0;
	m_imageInfoRecord.BBoxH = m_image.height();
	m_imageInfoRecord.colorspace = ColorSpaceRGB;
	m_imageInfoRecord.type = ImageTypeOther;
	m_imageInfoRecord.valid = true;

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
		return false;

	return readsize == static_cast<size_t>(size);
}

bool ScImgDataLoader_JPEGXL::decodeJpegXlOneShot(const uint8_t *jxl, size_t size, std::vector<uint8_t> *pixels, JXLImageInfo* imageInfo, QByteArray* icc_profile)
{
	// Multi-threaded parallel runner.
	auto runner = JxlResizableParallelRunnerMake(nullptr);
	auto dec = JxlDecoderMake(nullptr);

	if (JXL_DEC_SUCCESS != JxlDecoderSubscribeEvents(dec.get(),
		JXL_DEC_BASIC_INFO | JXL_DEC_COLOR_ENCODING | JXL_DEC_FULL_IMAGE))
	{
		qWarning("JxlDecoderSubscribeEvents failed");
		return false;
	}

	if (JXL_DEC_SUCCESS != JxlDecoderSetParallelRunner(dec.get(),
		JxlResizableParallelRunner, runner.get()))
	{
		qWarning("JxlDecoderSetParallelRunner failed");
		return false;
	}

	JxlBasicInfo info;
	JxlPixelFormat format = {4, JXL_TYPE_UINT8, JXL_NATIVE_ENDIAN, 0};

	JxlDecoderSetInput(dec.get(), jxl, size);
	JxlDecoderCloseInput(dec.get());

	for (;;)
	{
		JxlDecoderStatus status = JxlDecoderProcessInput(dec.get());

		if (status == JXL_DEC_ERROR)
		{
			qWarning("JXL decoder error");
			return false;
		}
		else if (status == JXL_DEC_NEED_MORE_INPUT)
		{
			qWarning("JXL error: already provided all input");
			return false;
		}
		else if (status == JXL_DEC_BASIC_INFO)
		{
			if (JXL_DEC_SUCCESS != JxlDecoderGetBasicInfo(dec.get(), &info))
			{
				qWarning("JxlDecoderGetBasicInfo failed");
				return false;
			}

			imageInfo->width = info.xsize;
			imageInfo->height = info.ysize;
			imageInfo->channels = (info.alpha_bits > 0) ? 4 : 3;

			if (info.bits_per_sample > 8)
			{
				format.data_type = JXL_TYPE_UINT16;
				imageInfo->bits_per_sample = 16;
			}
			else
			{
				format.data_type = JXL_TYPE_UINT8;
				imageInfo->bits_per_sample = 8;
			}

			format.num_channels = imageInfo->channels;

			JxlResizableParallelRunnerSetThreads(runner.get(),
				JxlResizableParallelRunnerSuggestThreads(info.xsize, info.ysize));
		}
		else if (status == JXL_DEC_COLOR_ENCODING)
		{
			size_t icc_size;
			if (JXL_DEC_SUCCESS != JxlDecoderGetICCProfileSize(dec.get(),
				JXL_COLOR_PROFILE_TARGET_DATA, &icc_size))
			{
				qWarning("JxlDecoderGetICCProfileSize failed");
				return false;
			}
			icc_profile->resize(icc_size);
			icc_profile->fill(0);
			if (JXL_DEC_SUCCESS != JxlDecoderGetColorAsICCProfile(dec.get(),
				JXL_COLOR_PROFILE_TARGET_DATA,
				reinterpret_cast<uint8_t*>(icc_profile->data()), icc_profile->size()))
			{
				qWarning("JxlDecoderGetColorAsICCProfile failed");
				return false;
			}
		}
		else if (status == JXL_DEC_NEED_IMAGE_OUT_BUFFER)
		{
			size_t buffer_size;
			if (JXL_DEC_SUCCESS != JxlDecoderImageOutBufferSize(dec.get(), &format, &buffer_size))
			{
				qWarning("JxlDecoderImageOutBufferSize failed");
				return false;
			}

			int bytesPerSample = (imageInfo->bits_per_sample > 8) ? 2 : 1;
			size_t expected = (size_t) imageInfo->width * imageInfo->height * imageInfo->channels * bytesPerSample;
			if (buffer_size != expected)
			{
				qWarning("Invalid out buffer size %" PRIu64 " expected %" PRIu64,
					static_cast<uint64_t>(buffer_size), static_cast<uint64_t>(expected));
				return false;
			}

			pixels->resize(buffer_size);
			if (JXL_DEC_SUCCESS != JxlDecoderSetImageOutBuffer(dec.get(), &format,
				pixels->data(), pixels->size()))
			{
				qWarning("JxlDecoderSetImageOutBuffer failed");
				return false;
			}
		}
		else if (status == JXL_DEC_FULL_IMAGE)
		{
			// For animations, take the first frame and stop
			if (info.have_animation)
			{
				m_basicinfo = info;
				return true;
			}
		}
		else if (status == JXL_DEC_SUCCESS)
		{
			m_basicinfo = info;
			return true;
		}
		else
		{
			qWarning("Unknown JXL decoder status");
			return false;
		}
	}
}
