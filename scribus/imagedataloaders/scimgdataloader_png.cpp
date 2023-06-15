/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QList>

#include <png.h>

#include "colormgmt/sccolormgmtengine.h"
#include "scimgdataloader_png.h"
#include "scribuscore.h"
#include "util_formats.h"

ScImgDataLoader_PNG::ScImgDataLoader_PNG()
{
	initSupportedFormatList();
}

void ScImgDataLoader_PNG::initSupportedFormatList()
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "png" );
}

static void ScImgDataLoader_PNG_read_fn(png_structp pngPtr, png_bytep data, png_size_t length)
{
	QFile *file = (QFile*) png_get_io_ptr(pngPtr);

	while (length)
	{
		qint64 nr = file->read((char*) data, length);
		if (nr <= 0)
		{
			png_error(pngPtr, "Read Error");
			return;
		}
		length -= nr;
	}
}

void ScImgDataLoader_PNG::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;

	QFile pngFile(fn);
	if (!pngFile.open(QFile::ReadOnly))
		return;

	png_byte pngSignature[8];
	pngFile.peek((char*) pngSignature, 8);
	if (png_sig_cmp(pngSignature, 0, 8) != 0)
	{
		pngFile.close();
		return;
	}

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!pngPtr)
	{
		pngFile.close();
		return;
	}

	png_infop pngInfo = png_create_info_struct(pngPtr);
	if (!pngInfo)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp) nullptr, (png_infopp) nullptr);
		pngFile.close();
		return;
	}

	// Catch errors (huh!!! we have to use setjump)
	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &pngInfo, (png_infopp) nullptr);
		pngFile.close();
		return;
	}

	png_set_read_fn(pngPtr, &pngFile, ScImgDataLoader_PNG_read_fn);
	png_read_info(pngPtr, pngInfo);

	// Read image profile
    png_charp profileName;
	png_bytep profileData;
	int compression_type;
	png_uint_32 profileLen;

	if (png_get_iCCP(pngPtr, pngInfo, &profileName, &compression_type, &profileData, &profileLen))
	{
		QByteArray profArray((const char*) profileData, profileLen);
		ScColorProfile prof = ScCore->defaultEngine.openProfileFromMem(profArray);
		if (prof && prof.isSuitableForOutput())
		{
			if (prof.colorSpace() == ColorSpace_Rgb)
				m_profileComponents = 3;
			if (prof.colorSpace() == ColorSpace_Cmyk)
				m_profileComponents = 4;
			if (prof.colorSpace() == ColorSpace_Gray)
				m_profileComponents = 1;
			m_embeddedProfile = profArray;
		}
	}
	else if (png_get_valid(pngPtr, pngInfo, PNG_INFO_sRGB))
	{
		QByteArray profArray;
		ScColorProfile prof = ScCore->defaultEngine.createProfile_sRGB();
		if (prof.save(profArray))
		{
			m_embeddedProfile = profArray;
			m_profileComponents = 3;
		}
	}

    pngFile.close();
    png_destroy_read_struct(&pngPtr, &pngInfo, (png_infopp) nullptr);
}

bool ScImgDataLoader_PNG::loadPicture(const QString& fn, int page, int res, bool thumbnail)
{
	bool result1 = ScImgDataLoader_QT::loadPicture(fn, page, res, thumbnail);
	if (!result1)
		return false;

	loadEmbeddedProfile(fn, page);
	if (m_embeddedProfile.size() > 0)
	{
		ScColorMgmtEngine engine(ScCore->defaultEngine);
		ScColorProfile imgProfile = engine.openProfileFromMem(m_embeddedProfile);
		eColorSpaceType profColorSpace = imgProfile.colorSpace();
		// Trick to detect images with grayscale + alpha pixel format
		// that Qt returns as ARGB 
		if (profColorSpace == ColorSpace_Gray && m_image.allGray())
			m_imageInfoRecord.colorspace = ColorSpaceGray;
		// Test profile colorspace consistency with image colorspace
		bool profileIsValid = false;
		if (m_imageInfoRecord.colorspace == ColorSpaceRGB)
			profileIsValid = (profColorSpace == ColorSpace_Rgb);
		if (m_imageInfoRecord.colorspace == ColorSpaceGray)
			profileIsValid = (profColorSpace == ColorSpace_Gray);
		if (profileIsValid)
		{
			m_imageInfoRecord.profileName = imgProfile.productDescription();
			if (m_imageInfoRecord.profileName.isEmpty())
				m_imageInfoRecord.profileName = "Profile #" + imgProfile.dataHash();
			m_imageInfoRecord.embeddedProfileName = m_imageInfoRecord.profileName;
		}
		else
		{
			m_embeddedProfile.clear();
			m_imageInfoRecord.profileName.clear();
			m_imageInfoRecord.embeddedProfileName.clear();
			m_profileComponents = 0;
		}
		m_imageInfoRecord.isEmbedded = profileIsValid;
	}

	return true;
}

