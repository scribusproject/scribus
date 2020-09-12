/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMAGE_H
#define SCIMAGE_H

#include "scconfig.h"
#include "scribusapi.h"
#include "scimagestructs.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <QImage>
#include <QString>
#include <QDataStream>
#include <QMap>
#include <QList>
#include <QVector>
#include <QFile>
#include <QDir>
#include <QStringList>

#include "fpointarray.h"
#include "sccolor.h"

class ScribusDoc;
class ScStreamFilter;
class CMSettings;
class ScImageCacheProxy;
class ScColorProfile;

class SCRIBUS_API ScImage : private QImage
{
public:
	ScImage();
	ScImage(const QImage & image);
	ScImage(const ScImage & image);
	ScImage( int width, int height );
	~ScImage();

	enum RequestType
	{
		CMYKData = 0,
		RGBData = 1,
		RawData = 2,
		OutputProfile = 3,
		Thumbnail = 4,
	};

	void initialize();

	const QImage& qImage();
	QImage* qImagePtr();
	QImage scaled(int w, int h, Qt::AspectRatioMode mode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation) const;
	
	int height() const { return QImage::height(); }
	int width() const { return QImage::width(); }
	bool hasAlpha() const { return QImage::hasAlphaChannel(); }
	bool hasSmoothAlpha() const;
	
	// Routines for PDF/PS output of images
	QByteArray ImageToArray() const;

	void convertToGray();

	bool writeRGBDataToFilter(ScStreamFilter* filter) const;
	bool writeGrayDataToFilter(ScStreamFilter* filter, bool precal) const;
	bool writeMonochromeDataToFilter(ScStreamFilter* filter, bool fromCmyk) const;
	bool writeCMYKDataToFilter(ScStreamFilter* filter) const;

	bool writePSImageToFilter(ScStreamFilter* filter, int pl) const;
	bool writePSImageToFilter(ScStreamFilter* filter, const QByteArray& mask, int pl) const;

	bool getAlpha(const QString& fn, int page, QByteArray& alpha, bool PDF, bool pdf14, int gsRes = 72, int scaleXSize = 0, int scaleYSize = 0);
	bool convert2JPG(const QString& fn, int Quality, bool isCMYK, bool isGray);

	// Image effects
	void applyEffect(const ScImageEffectList& effectsList, ColorList& colors, bool cmyk);

	// Generate a low res image for user preview
	bool createLowRes(double scale);

	// Scale this image in-place
	void scaleImage(int width, int height);

	// Retrieve an embedded ICC profile from the file path `fn', storing it in `profile'.
	// TODO: Bad API. Should probably be static member returning an ICCProfile (custom class) or something like that.
	void getEmbeddedProfile(const QString & fn, QByteArray *profile, int *components, int page = 0);

	// Load an image into this ScImage instance
	// TODO: document params, split into smaller functions
	bool loadPicture(const QString & fn, int page, const CMSettings& cmSettings, RequestType requestType, int gsRes, bool *realCMYK = 0, bool showMsg = false);
	bool loadPicture(ScImageCacheProxy & cache, bool & fromCache, int page, const CMSettings& cmSettings, RequestType requestType, int gsRes, bool *realCMYK = 0, bool showMsg = false);
	bool saveCache(ScImageCacheProxy & cache);

	ImageInfoRecord imgInfo;

private:

	// Scale image in-place : case of 32bpp image (RGBA, RGB32, CMYK)
	void scaleImage32bpp(int width, int height);

	// Scale image in-place : generic case
	void scaleImageGeneric(int width, int height);

	// Image effects
	void solarize(double factor, bool cmyk);
	void blur(int radius = 0);
	void sharpen(double radius= 0.0, double sigma = 1.0);
	void contrast(int contrastValue, bool cmyk);
	void brightness(int brightnessValue, bool cmyk);
	void invert(bool cmyk);
	void colorize(ScribusDoc* doc, ScColor color, int shade, bool cmyk);
	void duotone(ScribusDoc* doc, ScColor color1, int shade1, FPointArray curve1, bool lin1, ScColor color2, int shade2, FPointArray curve2, bool lin2, bool cmyk);
	void tritone(ScribusDoc* doc, ScColor color1, int shade1, FPointArray curve1, bool lin1, ScColor color2, int shade2, FPointArray curve2, bool lin2, ScColor color3, int shade3, const FPointArray& curve3, bool lin3, bool cmyk);
	void quadtone(ScribusDoc* doc, ScColor color1, int shade1, FPointArray curve1, bool lin1, ScColor color2, int shade2, FPointArray curve2, bool lin2, ScColor color3, int shade3, FPointArray curve3, bool lin3, ScColor color4, int shade4, FPointArray curve4, bool lin4, bool cmyk);
	void toGrayscale(bool cmyk);
	void doGraduate(FPointArray curve, bool cmyk, bool linear);
	void swapRGBA();
	bool convolveImage(QImage *dest, const unsigned int order, const double *kernel);
	int  getOptimalKernelWidth(double radius, double sigma);
	void applyCurve(const QVector<int>& curveTable, bool cmyk);

	void addProfileToCacheModifiers(ScImageCacheProxy & cache, const QString & prefix, const ScColorProfile & profile) const;
};

#endif
