/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QObject>

#include "scconfig.h"
#include "colormgmt/sccolormgmtengine.h"
#include "documentlogmanager.h"
#include "scimgdataloader_heif.h"
#include "scribuscore.h"

// libheif must be initialised before its first use, which also loads its codec
// plugins. That is done once for the whole run, not for every image.
class ScImgDataLoader_HEIF::HeifLibrary
{
	public:
		static void ensureInitialised()
		{
			static HeifLibrary library;
		}

	private:
		HeifLibrary()
		{
			// A plugin that fails to load is reported here, but the codecs built
			// into libheif still work, so the result is not treated as fatal
			heif_init(nullptr);
		}

		~HeifLibrary()
		{
			heif_deinit();
		}
};

// Owns the libheif objects for one opened file
class ScImgDataLoader_HEIF::HeifFile
{
	public:
		HeifFile() = default;
		HeifFile(const HeifFile&) = delete;
		HeifFile& operator=(const HeifFile&) = delete;

		~HeifFile()
		{
			if (m_handle)
				heif_image_handle_release(m_handle);
			if (m_context)
				heif_context_free(m_context);
		}

		/// Reads a file and opens its primary image. On failure, error holds the reason.
		bool open(const QString& filename, QString& error)
		{
			QFile file(filename);
			if (!file.open(QIODevice::ReadOnly))
			{
				error = file.errorString();
				return false;
			}

			// Read the file with Qt and pass libheif the data, so non-ASCII paths
			// work on every platform, whichever libheif version is installed
			m_data = file.readAll();
			HeifLibrary::ensureInitialised();
			m_context = heif_context_alloc();
			heif_error err = heif_context_read_from_memory_without_copy(m_context, m_data.constData(), m_data.size(), nullptr);
			if (err.code == heif_error_Ok)
				err = heif_context_get_primary_image_handle(m_context, &m_handle);
			if (err.code != heif_error_Ok)
			{
				error = QString::fromUtf8(err.message);
				return false;
			}
			return true;
		}

		heif_image_handle* primaryImage() const { return m_handle; }

	private:
		QByteArray m_data;
		heif_context* m_context { nullptr };
		heif_image_handle* m_handle { nullptr };
};

ScImgDataLoader_HEIF::ScImgDataLoader_HEIF()
{
	initSupportedFormatList();
}

void ScImgDataLoader_HEIF::initSupportedFormatList()
{
	m_supportedFormats.clear();
	m_supportedFormats.append("heic");
	m_supportedFormats.append("heif");
	m_supportedFormats.append("hif");
}

void ScImgDataLoader_HEIF::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;

	HeifFile file;
	QString error;
	if (file.open(fn, error))
		readProfile(file.primaryImage());
}

bool ScImgDataLoader_HEIF::preloadAlphaChannel(const QString& fn, int /*page*/, int /*res*/, bool& hasAlpha)
{
	initialize();
	hasAlpha = false;

	HeifFile file;
	QString error;
	if (!file.open(fn, error))
	{
		m_message = DocumentLogManager::msgDecoderError(fn, error);
		m_msgType = errorMsg;
		return false;
	}

	// Only decode when there is an alpha channel to return
	if (!heif_image_handle_has_alpha_channel(file.primaryImage()))
		return true;
	if (!decodeImage(file.primaryImage(), error))
	{
		m_message = DocumentLogManager::msgDecoderError(fn, error);
		m_msgType = errorMsg;
		return false;
	}
	hasAlpha = true;
	return true;
}

bool ScImgDataLoader_HEIF::loadPicture(const QString& fn, int /*page*/, int /*res*/, bool /*thumbnail*/)
{
	initialize();
	if (!QFile::exists(fn))
	{
		m_message = DocumentLogManager::msgFileNotFound(fn);
		m_msgType = errorMsg;
		return false;
	}

	HeifFile file;
	QString error;
	if (!file.open(fn, error) || !decodeImage(file.primaryImage(), error))
	{
		m_message = DocumentLogManager::msgDecoderError(fn, error);
		m_msgType = errorMsg;
		return false;
	}

	// Greyscale images are decoded to RGB but reported as grey, as the Qt loader does
	m_imageInfoRecord.colorspace = isMonochrome(file.primaryImage()) ? ColorSpaceGray : ColorSpaceRGB;

	// HEIF stores no resolution
	m_image.setDotsPerMeterX(qRound(72.0 / 0.0254));
	m_image.setDotsPerMeterY(qRound(72.0 / 0.0254));
	m_imageInfoRecord.xres = 72;
	m_imageInfoRecord.yres = 72;
	m_imageInfoRecord.BBoxX = 0;
	m_imageInfoRecord.BBoxH = m_image.height();
	m_imageInfoRecord.type = ImageTypeOther;
	m_imageInfoRecord.exifDataValid = false;
	m_imageInfoRecord.valid = true;
	m_pixelFormat = Format_BGRA_8;

	// Keep the profile only if it matches the image's color space
	readProfile(file.primaryImage());
	if (!m_embeddedProfile.isEmpty())
	{
		ScColorProfile profile = ScCore->defaultEngine.openProfileFromMem(m_embeddedProfile);
		const eColorSpaceType profileColorSpace = profile.colorSpace();
		const bool profileIsValid = (m_imageInfoRecord.colorspace == ColorSpaceGray) ? (profileColorSpace == ColorSpace_Gray) : (profileColorSpace == ColorSpace_Rgb);
		if (profileIsValid)
		{
			m_imageInfoRecord.profileName = profile.productDescription();
			if (m_imageInfoRecord.profileName.isEmpty())
				m_imageInfoRecord.profileName = "Profile #" + profile.dataHash();
			m_imageInfoRecord.embeddedProfileName = m_imageInfoRecord.profileName;
		}
		else
		{
			m_embeddedProfile.clear();
			m_profileComponents = 0;
			m_message = QObject::tr("The color profile embedded in %1 does not match the image's color space and was ignored").arg(fn);
			m_msgType = warningMsg;
		}
		m_imageInfoRecord.isEmbedded = profileIsValid;
	}

	return true;
}

bool ScImgDataLoader_HEIF::decodeImage(heif_image_handle* handle, QString& error)
{
	const bool hasAlpha = heif_image_handle_has_alpha_channel(handle);

	// Scribus works with 8-bit images, so reduce 10- and 12-bit images while decoding
	heif_decoding_options* options = heif_decoding_options_alloc();
	options->convert_hdr_to_8bit = 1;
	heif_image* image = nullptr;
	const heif_error err = heif_decode_image(handle, &image, heif_colorspace_RGB, hasAlpha ? heif_chroma_interleaved_RGBA : heif_chroma_interleaved_RGB, options);
	heif_decoding_options_free(options);
	if (err.code != heif_error_Ok)
	{
		error = QString::fromUtf8(err.message);
		return false;
	}

	int stride = 0;
	const uint8_t* data = heif_image_get_plane_readonly(image, heif_channel_interleaved, &stride);
	if (data == nullptr)
	{
		heif_image_release(image);
		error = QObject::tr("The image contains no pixel data");
		return false;
	}

	QImage::Format format = QImage::Format_RGB888;
	if (hasAlpha)
		format = heif_image_is_premultiplied_alpha(image) ? QImage::Format_RGBA8888_Premultiplied : QImage::Format_RGBA8888;

	// QImage wraps libheif's buffer.
	const QImage decoded(data, heif_image_get_primary_width(image), heif_image_get_primary_height(image), stride, format);
	m_image = decoded.convertToFormat(QImage::Format_ARGB32);
	heif_image_release(image);

	if (m_image.isNull())
	{
		error = QObject::tr("Not enough memory to load the image");
		return false;
	}
	return true;
}

void ScImgDataLoader_HEIF::readProfile(heif_image_handle* handle)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;

	// Load ICC profile if there is one
	const size_t iccSize = heif_image_handle_get_raw_color_profile_size(handle);
	if (iccSize > 0)
	{
		QByteArray icc(static_cast<int>(iccSize), '\0');
		if (heif_image_handle_get_raw_color_profile(handle, icc.data()).code != heif_error_Ok)
			return;
		ScColorProfile profile = ScCore->defaultEngine.openProfileFromMem(icc);
		if (!profile || !profile.isSuitableForOutput())
			return;
		if (profile.colorSpace() == ColorSpace_Rgb)
			m_profileComponents = 3;
		else if (profile.colorSpace() == ColorSpace_Gray)
			m_profileComponents = 1;
		else
			return;
		m_embeddedProfile = icc;
		return;
	}

	// Otherwise get color primaries and transfer function (nclx). Only sRGB color
	// images have a profile, the rest use the default image profile.
	if (isMonochrome(handle))
		return;
	heif_color_profile_nclx* nclx = nullptr;
	if (heif_image_handle_get_nclx_color_profile(handle, &nclx).code != heif_error_Ok)
		return;
	const bool isSRGB = (nclx->color_primaries == heif_color_primaries_ITU_R_BT_709_5)
			&& (nclx->transfer_characteristics == heif_transfer_characteristic_IEC_61966_2_1);
	heif_nclx_color_profile_free(nclx);
	if (!isSRGB)
		return;

	ScColorProfile profile = ScCore->defaultEngine.createProfile_sRGB();
	QByteArray icc;
	if (profile.save(icc))
	{
		m_embeddedProfile = icc;
		m_profileComponents = 3;
	}
}

bool ScImgDataLoader_HEIF::isMonochrome(heif_image_handle* handle) const
{
	heif_colorspace colorspace = heif_colorspace_undefined;
	heif_chroma chroma = heif_chroma_undefined;
	heif_image_handle_get_preferred_decoding_colorspace(handle, &colorspace, &chroma);
	return (colorspace == heif_colorspace_monochrome);
}
