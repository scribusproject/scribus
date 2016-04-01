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

	QRgb *s = (QRgb*)(output->scanLine(0));
	int i;
	for (i = 0; i < width*height; i++)
	{
		unsigned char b = ScaleQuantumToChar(pixels[i].blue);
		unsigned char g = ScaleQuantumToChar(pixels[i].green);
		unsigned char r = ScaleQuantumToChar(pixels[i].red);
		unsigned char a;
		if (hasAlpha)
			a = 255 - ScaleQuantumToChar(pixels[i].opacity);
		else
			a = 255;
		*s = qRgba(r, g, b, a);
		s++;
	}

	return true;
}


bool ScImgDataLoader_GMagick::loadPicture(const QString& fn, int /*page*/, int res, bool thumbnail)
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
	initGraphicsMagick();

	ExceptionInfo exception;
	GetExceptionInfo(&exception);
	ImageInfo *image_info = CloneImageInfo(0);
	strcpy(image_info->filename, fn.toUtf8().data());
	image_info->units = PixelsPerInchResolution;
	Image *image = ReadImage(image_info, &exception);
	if (exception.severity != UndefinedException)
		CatchException(&exception);
	if (!image)
	{
		qCritical() << "Failed to read image" << fn;
		return false;
	}
	int width = image->columns;
	int height = image->rows;
	double xres = image->x_resolution;
	double yres = image->y_resolution;

	if (thumbnail)
		image = FlattenImages(image, &exception);
	else
	{
		struct PSDLayer layer;
		layerCount = 0;
		Image *flatten_image = CloneImage(image, 0, 0, true, &exception);
		Image *next;
		if ((flatten_image != (Image *) NULL) && (image->next != (Image *) NULL))
		{
			bool visible = true;
			double opacity = 1.0;
			QString compositeOp = blendModeToString(flatten_image->compose);
			QString layerName = QString("layer%1").arg(layerCount+1);
			if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerCount)))
				opacity = m_imageInfoRecord.RequestProps[layerCount].opacity / 255.0;
			if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerCount)))
				visible = m_imageInfoRecord.RequestProps[layerCount].visible;
			if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerCount)))
				compositeOp = m_imageInfoRecord.RequestProps[layerCount].blend;
			layer.layerName = layerName;
			layer.channelType.clear();
			layer.channelLen.clear();
			layer.opacity = qRound(opacity * 255);
			layer.blend = compositeOp;
			layer.maskYpos = 0;
			layer.maskXpos = 0;
			layer.maskHeight = 0;
			layer.maskWidth = 0;
			layer.flags = visible ? 0 : 2;
			QImage imt;
			if (image->colorspace == CMYKColorspace)
			{
				RawImage r2_image;
				readCMYK(flatten_image, &r2_image, width, height);
				imt = r2_image.convertToQImage(true);
			}
			else
			{
				m_useRawImage = false;
				imt = QImage(width, height, QImage::Format_ARGB32);
				readRGB(flatten_image, &imt, width, height);
			}
			double sx = imt.width() / 40.0;
			double sy = imt.height() / 40.0;
			layer.thumb = sy < sx ?  imt.scaled(qRound(imt.width() / sx), qRound(imt.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation) :
				  imt.scaled(qRound(imt.width() / sy), qRound(imt.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			m_imageInfoRecord.layerInfo.append(layer);
			layerCount++;
			if (!visible)
			{
				(void)CompositeImage(flatten_image, ClearCompositeOp, flatten_image, flatten_image->page.x, flatten_image->page.y);
			}
			for (next = image->next; next != (Image*)NULL; next = next->next)
			{
				visible = true;
				opacity = 1.0;
				compositeOp = blendModeToString(next->compose);
				layerName = QString("layer%1").arg(layerCount+1);
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerCount)))
					opacity = m_imageInfoRecord.RequestProps[layerCount].opacity / 255.0;
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerCount)))
					visible = m_imageInfoRecord.RequestProps[layerCount].visible;
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerCount)))
					compositeOp = m_imageInfoRecord.RequestProps[layerCount].blend;
				layer.layerName = layerName;
				layer.channelType.clear();
				layer.channelLen.clear();
				layer.opacity = qRound(opacity * 255);
				layer.blend = compositeOp;
				layer.maskYpos = 0;
				layer.maskXpos = 0;
				layer.maskHeight = 0;
				layer.maskWidth = 0;
				layer.flags = visible ? 0 : 2;
				if (image->colorspace == CMYKColorspace)
				{
					RawImage r2_image;
					readCMYK(next, &r2_image, width, height);
					imt = r2_image.convertToQImage(true);
				}
				else
				{
					m_useRawImage = false;
					imt = QImage(width, height, QImage::Format_ARGB32);
					readRGB(next, &imt, width, height);
				}
				double sx = imt.width() / 40.0;
				double sy = imt.height() / 40.0;
				layer.thumb = sy < sx ?  imt.scaled(qRound(imt.width() / sx), qRound(imt.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation) :
					  imt.scaled(qRound(imt.width() / sy), qRound(imt.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
				m_imageInfoRecord.layerInfo.append(layer);
				layerCount++;
				if (visible)
					(void)CompositeImage(flatten_image, (CompositeOperator)blendModeToInt(compositeOp), next, next->page.x, next->page.y);
			}
			image = CloneImage(flatten_image, 0, 0, true, &exception);
		}
	}

	if (image->colorspace == CMYKColorspace)
	{
		m_useRawImage = true;
		if (!readCMYK(image, &r_image, width, height))
			return false;
		m_imageInfoRecord.colorspace = ColorSpaceCMYK;
		m_pixelFormat = (r_image.channels() == 5) ? Format_CMYKA_8 : Format_CMYK_8;
	}
	else
	{
		m_useRawImage = false;
		m_image = QImage(width, height, QImage::Format_ARGB32);
		if (!readRGB(image, &m_image, width, height))
			return false;
		m_imageInfoRecord.colorspace = ColorSpaceRGB;
		m_pixelFormat = Format_BGRA_8;
	}
	m_imageInfoRecord.exifDataValid = false;
	if (thumbnail)
	{
		m_imageInfoRecord.exifDataValid = true;
		m_imageInfoRecord.exifInfo.thumbnail = m_image;
		m_imageInfoRecord.exifInfo.height = height;
		m_imageInfoRecord.exifInfo.width = width;
	}
	m_imageInfoRecord.type = ImageTypeOther;
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

QString ScImgDataLoader_GMagick::blendModeToString(int compositeOp)
{
	QString ret = "norm";
	if (compositeOp == 24)
		ret = "dark";
	else if (compositeOp == 25)
		ret = "lite";
	else if (compositeOp == 11)
		ret = "mul ";
	else if (compositeOp == 30)
		ret = "scrn";
	else if (compositeOp == 31)
		ret = "over";
	else if (compositeOp == 37)
		ret = "hLit";
	else if (compositeOp == 41)
		ret = "sLit";
	else if (compositeOp == 10)
		ret = "diff";
	else if (compositeOp == 39)
		ret = "div ";
	else if (compositeOp == 40)
		ret = "idiv";
	else if (compositeOp == 28)
		ret = "colr";
	else if (compositeOp == 29)
		ret = "lum ";
	else if (compositeOp == 26)
		ret = "hue ";
	else if (compositeOp == 27)
		ret = "sat ";
	else if (compositeOp == 6)
		ret = "plus";
	else if (compositeOp == 2)
		ret = "dsti";
	else if (compositeOp == 3)
		ret = "dsto";
	return ret;
}

int ScImgDataLoader_GMagick::blendModeToInt(QString compositeOp)
{
	int ret = 1;
	if (compositeOp == "norm")
		ret = 1;
	else if (compositeOp == "dark")
		ret = 24;
	else if (compositeOp == "lite")
		ret = 25;
	else if (compositeOp == "mul ")
		ret = 11;
	else if (compositeOp == "scrn")
		ret = 30;
	else if (compositeOp == "over")
		ret = 31;
	else if (compositeOp == "hLit")
		ret = 37;
	else if (compositeOp == "sLit")
		ret = 41;
	else if (compositeOp == "diff")
		ret = 10;
	else if (compositeOp == "div ")
		ret = 39;
	else if (compositeOp == "idiv")
		ret = 40;
	else if (compositeOp == "colr")
		ret = 28;
	else if (compositeOp == "lum ")
		ret = 29;
	else if (compositeOp == "hue ")
		ret = 26;
	else if (compositeOp == "sat ")
		ret = 27;
	else if (compositeOp == "plus")
		ret = 6;
	else if (compositeOp == "dsti")
		ret = 2;
	else if (compositeOp == "dsto")
		ret = 3;
	return ret;
}
