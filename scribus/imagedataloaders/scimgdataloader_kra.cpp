/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QBuffer>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QList>
#include <QScopedPointer>

#include <png.h>

#include "colormgmt/sccolormgmtengine.h"
#include "scimgdataloader_kra.h"
#include "scribuscore.h"
#include "util_formats.h"

ScImgDataLoader_KRA::ScImgDataLoader_KRA()
{
	initSupportedFormatList();
}

void ScImgDataLoader_KRA::initSupportedFormatList()
{
	m_supportedFormats.clear();
	m_supportedFormats.append("kra");
}

static void ScImgDataLoader_KRA_read_fn(png_structp pngPtr, png_bytep data, png_size_t length)
{
	QBuffer *buffer = (QBuffer*) png_get_io_ptr(pngPtr);

	while (length)
	{
		int nr = buffer->read((char*) data, length);
		if (nr <= 0)
		{
			png_error(pngPtr, "Read Error");
			return;
		}
		length -= nr;
	}
}

QByteArray ScImgDataLoader_KRA::getICCProfileFromPNGData(QByteArray& pngData)
{
	QByteArray profileData;
	if (pngData.size() <= 0)
		return QByteArray();

	QBuffer pngBuffer(&pngData);
	if (!pngBuffer.open(QFile::ReadOnly))
		return QByteArray();

	png_byte pngSignature[8];
	pngBuffer.peek((char*) pngSignature, 8);
	if (png_sig_cmp(pngSignature, 0, 8) != 0)
	{
		pngBuffer.close();
		return QByteArray();
	}

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!pngPtr)
	{
		pngBuffer.close();
		return QByteArray();
	}

	png_infop pngInfo = png_create_info_struct(pngPtr);
	if (!pngInfo)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp) NULL, (png_infopp) NULL);
		pngBuffer.close();
		return QByteArray();
	}

	// Catch errors (huh!!! we have to use setjump)
	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &pngInfo, (png_infopp) NULL);
		pngBuffer.close();
		return QByteArray();
	}

	png_set_read_fn(pngPtr, &pngBuffer, ScImgDataLoader_KRA_read_fn);
	png_read_info(pngPtr, pngInfo);

	// Read image profile
    png_charp profileName;
	png_bytep profileBuffer;
	int compression_type;
	png_uint_32 profileLen;

	if (png_get_iCCP(pngPtr, pngInfo, &profileName, &compression_type, &profileBuffer, &profileLen))
	{
		QByteArray profArray((const char*) profileBuffer, profileLen);
		if ((profArray.size() >= 40) && (profArray[36] == 'a') && (profArray[37] == 'c') && (profArray[38] == 's') && (profArray[39] == 'p'))
			profileData = profArray;
	}
	else if (png_get_valid(pngPtr, pngInfo, PNG_INFO_sRGB))
	{
		QByteArray profArray;
		ScColorProfile prof = ScCore->defaultEngine.createProfile_sRGB();
		if (prof.save(profArray))
			profileData = profArray;
	}

    pngBuffer.close();
    png_destroy_read_struct(&pngPtr, &pngInfo, (png_infopp) NULL);

	return profileData;
}

void ScImgDataLoader_KRA::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;

	QScopedPointer<ScZipHandler> uz(new ScZipHandler());
	if (!uz->open(fn))
		return;

	if (!uz->contains("mergedimage.png"))
		return;

	QByteArray im;
	if (!uz->read("mergedimage.png", im))
		return;

	QByteArray profileData = getICCProfileFromPNGData(im);
	if (profileData.isEmpty())
		return;

	ScColorProfile prof = ScCore->defaultEngine.openProfileFromMem(profileData);
	if (prof && prof.isSuitableForOutput())
	{
		if (prof.colorSpace() == ColorSpace_Rgb)
			m_profileComponents = 3;
		if (prof.colorSpace() == ColorSpace_Cmyk)
			m_profileComponents = 4;
		if (prof.colorSpace() == ColorSpace_Gray)
			m_profileComponents = 1;
		m_embeddedProfile = profileData;
	}
}

bool ScImgDataLoader_KRA::loadPicture(const QString& fn, int /*page*/, int /*res*/, bool thumbnail)
{
	if (!QFile::exists(fn))
		return false;
	bool valid = m_imageInfoRecord.isRequest;
	QMap<int, ImageLoadRequest> req = m_imageInfoRecord.RequestProps;
	initialize();
	m_imageInfoRecord.RequestProps = req;
	m_imageInfoRecord.isRequest = valid;
	m_imageInfoRecord.type = ImageTypeOther;
	m_imageInfoRecord.exifDataValid = false;
	m_imageInfoRecord.layerInfo.clear();
	m_imageInfoRecord.PDSpathData.clear();

	QScopedPointer<ScZipHandler> uz(new ScZipHandler());
	if (!uz->open(fn))
		return false;

	// if thumbnail is set to true, this function is loading the thumbnail image into the preview area
	// if thumbnail is set to false, this function is loading the main image

	if (thumbnail)
	{
		if (!uz->contains("preview.png"))
			return false;

		QByteArray im;
		if (!uz->read("preview.png", im))
			return false;

		m_image = QImage::fromData(im);

		if (uz->contains("maindoc.xml"))
		{
			QByteArray f;
			if (uz->read("maindoc.xml", f))
			{
				QDomDocument designMapDom;
				if (designMapDom.setContent(f))
				{
					QDomElement docElem = designMapDom.documentElement().firstChildElement("IMAGE");
					m_imageInfoRecord.exifInfo.height = docElem.attribute("height", "0").toInt();
					m_imageInfoRecord.exifInfo.width = docElem.attribute("width", "0").toInt();

					// sanitize values for x-res and y-res. some older versions of Krita had resolutions like "76,0000023"
					QString xResClean = docElem.attribute("x-res", "72").replace(",", ".");
					QString yResClean = docElem.attribute("y-res", "72").replace(",", ".");

					// strip out all decimal stuff since we cannot have that.
					xResClean = xResClean.split(".").at(0);
					yResClean = yResClean.split(".").at(0);

					m_imageInfoRecord.xres = xResClean.toInt();
					m_imageInfoRecord.yres = yResClean.toInt();
				}
			}
		}
		m_imageInfoRecord.exifInfo.thumbnail = m_image;
		m_imageInfoRecord.exifDataValid = true;
		return true;
	}

	if (!uz->contains("maindoc.xml"))
		return false;

	QByteArray f;
	if (!uz->read("maindoc.xml", f))
		return false;

	QDomDocument designMapDom;
	if (!designMapDom.setContent(f))
		return false;

	QDomElement docElem = designMapDom.documentElement().firstChildElement("IMAGE");
	m_imageInfoRecord.exifInfo.height = docElem.attribute("height", "0").toInt();
	m_imageInfoRecord.exifInfo.width = docElem.attribute("width", "0").toInt();

	// sanitize values for x-res and y-res. some older versions of Krita had resolutions like "76,0000023"
	QString xResClean = docElem.attribute("x-res", "72").replace(",", ".");
	QString yResClean = docElem.attribute("y-res", "72").replace(",", ".");

	// strip out all decimal stuff since we cannot have that.
	xResClean = xResClean.split(".").at(0);
	yResClean = yResClean.split(".").at(0);

	m_imageInfoRecord.xres = xResClean.toInt();
	m_imageInfoRecord.yres = yResClean.toInt();

	m_image = QImage(m_imageInfoRecord.exifInfo.width, m_imageInfoRecord.exifInfo.height, QImage::Format_ARGB32_Premultiplied);
	if (m_image.isNull())
		return false;

	// load the mergedimage.png into the painter
	if (!uz->contains("mergedimage.png"))
		return false;

	QByteArray im;
	if (!uz->read("mergedimage.png", im))
		return false;
	
	m_image.fill( qRgba(0, 0, 0, 0) );
	m_image = QImage::fromData(im);

	m_image = m_image.convertToFormat(QImage::Format_ARGB32);
	m_imageInfoRecord.valid = true;
	m_image.setDotsPerMeterX((int) (m_imageInfoRecord.xres / 0.0254));
	m_image.setDotsPerMeterY((int) (m_imageInfoRecord.yres / 0.0254));
	m_imageInfoRecord.BBoxX = 0;
	m_imageInfoRecord.BBoxH = m_image.height();
	m_imageInfoRecord.colorspace = ColorSpaceRGB;
	m_pixelFormat = Format_BGRA_8;

	QByteArray profileData = getICCProfileFromPNGData(im);
	if (profileData.size() > 0)
	{
		ScColorProfile prof = ScCore->defaultEngine.openProfileFromMem(profileData);
		if (prof && prof.isSuitableForOutput())
		{
			if (prof.colorSpace() == ColorSpace_Rgb)
				m_profileComponents = 3;
			if (prof.colorSpace() == ColorSpace_Cmyk)
				m_profileComponents = 4;
			if (prof.colorSpace() == ColorSpace_Gray)
				m_profileComponents = 1;
			m_imageInfoRecord.profileName = prof.productDescription();
			if (m_imageInfoRecord.profileName.isEmpty())
				m_imageInfoRecord.profileName = "Profile #" + prof.dataHash();
			m_imageInfoRecord.embeddedProfileName = m_imageInfoRecord.profileName;
			m_embeddedProfile = profileData;
		}
	}

	uz->close();
	return true;
}

bool ScImgDataLoader_KRA::preloadAlphaChannel(const QString& fn, int /*page*/, int res, bool& hasAlpha)
{
	hasAlpha = false;
	if (loadPicture(fn, 0, 0, false))
	{
		hasAlpha = m_image.hasAlphaChannel();
		if (m_image.hasAlphaChannel())
			m_image = m_image.convertToFormat(QImage::Format_ARGB32);
		else
			m_image = QImage(); // Discard data immediately
		return true;
	}
	return false;
}
