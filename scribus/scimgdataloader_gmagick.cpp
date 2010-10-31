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
#include "scimgdataloader_gmagick.h"
#include "scconfig.h"



bool ScImgDataLoader_GMagick::gm_initialized = false;

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


void ScImgDataLoader_GMagick::initGraphicsMagick()
{
	if (!gm_initialized) {
		InitializeMagick(0); //TODO: Get path
		gm_initialized = true;
	}
}


bool ScImgDataLoader_GMagick::readCMYK(Image *input, RawImage *output, int width, int height)
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

	bool hasAlpha = input->matte;

	if (!output->create(width, height, hasAlpha ? 5 : 4)) return false;

	ExceptionInfo exception;
	GetExceptionInfo(&exception);
	const PixelPacket *pixels = AcquireImagePixels(input, 0, 0, width, height, &exception);
	if (exception.severity != UndefinedException)
		CatchException(&exception);
	if (!pixels) {
		qCritical() << QObject::tr("Could not get pixel data!");
		return false;
	}

    const IndexPacket *alpha = 0;
    if (hasAlpha) {
        alpha = AccessImmutableIndexes(input);
        if (!alpha) {
            qCritical() << QObject::tr("Could not get alpha channel data!");
            return false;
        }
    }

	unsigned char *buffer = output->bits();
	if (!buffer) {
	   qCritical() << QObject::tr("Could not allocate output buffer!");
	   return false;
    }

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


bool ScImgDataLoader_GMagick::readRGB(Image *input, QImage *output, int width, int height)
{
	//Copied from GraphicsMagick header and modified
#define GetRedSample(p) (p.red)
#define GetGreenSample(p) (p.green)
#define GetBlueSample(p) (p.blue)
#define GetRGBAlphaSample(p) (p.opacity)

	bool hasAlpha = input->matte;

	ExceptionInfo exception;
	GetExceptionInfo(&exception);
	const PixelPacket *pixels = AcquireImagePixels(input, 0, 0, width, height, &exception);
	if (exception.severity != UndefinedException)
		CatchException(&exception);
	if (!pixels) {
		qCritical() << QObject::tr("Could not get pixel data!");
		return false;
	}

	unsigned char *buffer = output->bits();
	if (!buffer) {
		qCritical() << QObject::tr("Could not allocate output buffer!");
		return false;
	}

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


bool ScImgDataLoader_GMagick::loadPicture(const QString& fn, int /*page*/, int res, bool thumbnail)
{
	initGraphicsMagick();
	qDebug() << "Loading image" << fn << "Tumbnail:" << thumbnail;

	if (!QFile::exists(fn))
		return false;

	initialize();

	ExceptionInfo exception;
	GetExceptionInfo(&exception);
	ImageInfo *image_info = CloneImageInfo(0);
	strcpy(image_info->filename, fn.toUtf8().data());
	image_info->units = PixelsPerInchResolution;
	Image *image = ReadImage(image_info, &exception);
	if (exception.severity != UndefinedException)
		CatchException(&exception);
	if (!image) {
		qCritical() << "Failed to read image" << fn;
		return false;
	}

//	qDebug() << "has matte(alpha):" << image->matte;
	int width = image->columns;
	int height = image->rows;

	double xres = image->x_resolution;
	double yres = image->y_resolution;

	if (image->colorspace == CMYKColorspace) {
		qDebug() << "CMYK image";
		m_useRawImage = true;
		if (!readCMYK(image, &r_image, width, height)) return false;
		m_imageInfoRecord.colorspace = ColorSpaceCMYK;
		m_pixelFormat = (r_image.channels() == 5) ? Format_CMYKA_8 : Format_CMYK_8;
	} else {
		m_useRawImage = false;
		m_image = QImage(width, height, QImage::Format_ARGB32);
		if (!readRGB(image, &m_image, width, height)) return false;
		m_imageInfoRecord.colorspace = ColorSpaceRGB;
		m_pixelFormat = Format_BGRA_8;
	}
	m_imageInfoRecord.type = ImageTypeOther;
	m_imageInfoRecord.exifDataValid = false;
	m_imageInfoRecord.xres = qMax(72, qRound(xres));
	m_imageInfoRecord.yres = qMax(72, qRound(yres));
	m_imageInfoRecord.BBoxX = 0;
	m_imageInfoRecord.BBoxH = m_image.height();
	m_imageInfoRecord.valid = true;
	return true;
}


bool ScImgDataLoader_GMagick::preloadAlphaChannel(const QString& fn, int /*page*/, int res, bool& hasAlpha)
{
	initGraphicsMagick();
	qDebug() << "GMAGICK: preloadAlpha" << fn;
	initialize();
	hasAlpha = false;

	if (!QFile::exists(fn))
		return false;

	ExceptionInfo exception;
	GetExceptionInfo(&exception);
	ImageInfo *image_info = CloneImageInfo(0);
	strcpy(image_info->filename, fn.toUtf8().data());
	image_info->units = PixelsPerInchResolution;
	Image *image = PingImage(image_info, &exception);
	if (exception.severity != UndefinedException)
		CatchException(&exception);
	if (!image) {
		qCritical() << "Failed to read image" << fn;
		return false;
	}

	hasAlpha = image->matte;
	if (!hasAlpha) return true;
	return loadPicture(fn, 0, 0, false);
}


void ScImgDataLoader_GMagick::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	qDebug() << "GMAGICK: loadEmbeddedProfile";

	initGraphicsMagick();
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
	if (!QFile::exists(fn)) return;

	ExceptionInfo exception;
	GetExceptionInfo(&exception);
	ImageInfo *image_info = CloneImageInfo(0);
	strcpy(image_info->filename, fn.toUtf8().data());
	image_info->units = PixelsPerInchResolution;
	Image *image = ReadImage(image_info, &exception);
	if (exception.severity != UndefinedException)
		CatchException(&exception);
	if (!image) {
		qCritical() << "Failed to read image" << fn;
		return;
	}

	size_t length = 0;
	const unsigned char *src = GetImageProfile(image, "ICM", &length);
	char *dest = m_embeddedProfile.data();

	if (image->colorspace == CMYKColorspace) {
		m_profileComponents = 4;
	} else if (image->colorspace == RGBColorspace) {
		m_profileComponents = 3;
	}

	m_embeddedProfile.resize(length);
	memcpy(dest, src, length);
}

