/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QDebug>
#include <setjmp.h>
#include "scconfig.h"
#include "scimgdataloader_gmagick.h"
#include <Magick++.h>
using namespace Magick;

static bool read_cmyk(Image *input, RawImage *output, int width, int height)
{
	/* Mapping:
		red:     cyan
		green:   magenta
		blue:    yellow
		opacity: black
		index:   alpha
	*/
	//Copied from GraphicsMagick header and modified
	#define GetCyanSample(p) (p.red)
	#define GetMagentaSample(p) (p.green)
	#define GetYellowSample(p) (p.blue)
	#define GetCMYKBlackSample(p) (p.opacity)
	#define GetAlphaSample(p) (p)
	
	bool hasAlpha = input->matte();
	if (!output->create(width, height, hasAlpha ? 5 : 4)) return false;
	const PixelPacket *pixels = input->getConstPixels(0, 0, width, height);
	const IndexPacket *alpha  = input->getConstIndexes();
	unsigned char *buffer = output->bits();

	int i;
	for (i = 0; i < width*height; i++) {
		*buffer++ = ScaleQuantumToChar(GetCyanSample(pixels[i]));
		*buffer++ = ScaleQuantumToChar(GetMagentaSample(pixels[i]));
		*buffer++ = ScaleQuantumToChar(GetYellowSample(pixels[i]));
		*buffer++ = ScaleQuantumToChar(GetCMYKBlackSample(pixels[i]));
		if (hasAlpha) {
			*buffer++ = 255 - ScaleQuantumToChar(GetAlphaSample(alpha[i]));
		}
	}
	return true;
}


static bool read_rgb(Image *input, QImage *output, int width, int height)
{
	//Copied from GraphicsMagick header and modified
	#define GetRedSample(p) (p.red)
	#define GetGreenSample(p) (p.green)
	#define GetBlueSample(p) (p.blue)
	#define GetRGBAlphaSample(p) (p.opacity)
	
	bool hasAlpha = input->matte();
	const PixelPacket *pixels = input->getConstPixels(0, 0, width, height);
	unsigned char *buffer = output->bits();

	int i;
	for (i = 0; i < width*height; i++) {
		*buffer++ = ScaleQuantumToChar(GetBlueSample(pixels[i]));
		*buffer++ = ScaleQuantumToChar(GetGreenSample(pixels[i]));
		*buffer++ = ScaleQuantumToChar(GetRedSample(pixels[i]));
		if (hasAlpha) {
			*buffer++ = 255 - ScaleQuantumToChar(GetRGBAlphaSample(pixels[i]));
		} else {
			*buffer++ = 255;
		}
	}
	return true;
}

ScImgDataLoader_GMagick::ScImgDataLoader_GMagick(void)
{
	initSupportedFormatList();
	m_useRawImage = true;
}

void ScImgDataLoader_GMagick::initSupportedFormatList(void)
{
	//This is unused! See util_formats.{h,cpp}
	m_supportedFormats.clear();
	m_supportedFormats.append("bmp"); //TODO
}

void ScImgDataLoader_GMagick::loadEmbeddedProfile(const QString& fn, int /*page*/)
{	
	if (!QFile::exists(fn)) {
		m_embeddedProfile.resize(0);
		m_profileComponents = 0;
		return;
	}
	
	try {
		Image image(std::string(fn.toUtf8()));
		Blob icc = image.iccColorProfile();
		
		if (image.colorSpace() == CMYKColorspace) {
			m_profileComponents = 4;
		} else if (image.colorSpace() == RGBColorspace) {
			m_profileComponents = 3;
		} else {
			m_profileComponents = 0;
			m_embeddedProfile.resize(0);
		}
		
		m_embeddedProfile.resize(icc.length());
		
		char *dest = m_embeddedProfile.data();
		const char *src = (const char *)icc.data();
		
		unsigned int i;
		for (i=0; i<icc.length(); i++) {
			dest[i] = src[i];
		}

	} catch (std::exception &error_) {
		m_profileComponents = 0;
		m_embeddedProfile.resize(0);
		
		qDebug() << "GraphicsMagic Exception in ScImgDataLoader_GMagick::loadEmbeddedProfile(" 
			<< fn << "):" << error_.what();
		
		return;
	}
}

bool ScImgDataLoader_GMagick::preloadAlphaChannel(const QString& fn, int /*page*/, int res, bool& hasAlpha)
{
	qDebug() << "GMAGICK: preloadAlpha" << fn;
	
	initialize();
	hasAlpha = false;
	
	if (!QFile::exists(fn))
		return false;

	try {
		Image image(std::string(fn.toUtf8()));
		
		hasAlpha = image.matte();
		
		if (!hasAlpha) {
			return true; //No need to load any image data
		} else {
			return loadPicture(fn, 0, 0, false);
		}
		
	} catch (std::exception &error_) {
		qDebug() << "GraphicsMagic Exception in ScImgDataLoader_GMagick::preloadAlphaChannel(" 
			<< fn << "):" << error_.what();
		return false;
	}
	return true;
}

bool ScImgDataLoader_GMagick::loadPicture(const QString& fn, int /*page*/, int res, bool thumbnail)
{
	qDebug() << "Loading image" << fn << "Tumbnail:" << thumbnail;

	if (!QFile::exists(fn))
		return false;
		
	initialize();
	
	try {
		Image image(std::string(fn.toUtf8()));
		
		qDebug() << "Detected format:" << QString(image.format().c_str()) 
			<< ", has matte(alpha):" << image.matte();

		int width = image.baseColumns();
		int height = image.baseRows();
		
		image.resolutionUnits(PixelsPerInchResolution);
		double xres = image.xResolution();
		double yres = image.yResolution();
		int resInf = m_imageInfoRecord.lowResType;
		
		/*NOTE: 
			- The BGRO bit ordering was found by try'n'error
			- Always use "O" for the alpha channel! "A" is inverted!
			- image.write() is broken for cmyk images!
		*/
		if (image.colorSpace() == CMYKColorspace) {
			qDebug() << "CMYK image";
			m_useRawImage = true;
			if (!read_cmyk(&image, &r_image, width, height)) return false;
			m_imageInfoRecord.colorspace = ColorSpaceCMYK;
		} else {
			m_useRawImage = false;
			m_image = QImage(width, height, QImage::Format_ARGB32);
			if (!read_rgb(&image, &m_image, width, height)) return false;
			m_imageInfoRecord.colorspace = ColorSpaceRGB;
		}
		m_imageInfoRecord.type = ImageTypeOther;
		m_imageInfoRecord.exifDataValid = false;
		m_imageInfoRecord.xres = qMax(72, qRound(xres));
		m_imageInfoRecord.yres = qMax(72, qRound(yres));
		m_imageInfoRecord.lowResType = resInf;
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = m_image.height();
		m_imageInfoRecord.valid = true;
	} catch (std::exception &error_) {
		qDebug() << "GraphicsMagic Exception in ScImgDataLoader_GMagick::loadImage(" 
			<< fn << "):" << error_.what();
		return false;
	}
	return true;
}