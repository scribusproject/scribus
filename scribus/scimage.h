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

#include <setjmp.h>
#include <cstdlib>
#include <cmath>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
//#include <valarray>
#include <qmemarray.h>
#include <qimage.h>
#include <qstring.h>
#include <qdatastream.h>
#include <qmap.h>
#include <qvaluelist.h>
#include <qfile.h>
#include <qdir.h>
#include <qstringlist.h>

#include "cmsettings.h"
#include "fpointarray.h"
#include "sccolor.h"
extern "C"
{
#define XMD_H           // shut JPEGlib up
#if defined(Q_OS_UNIXWARE)
#  define HAVE_BOOLEAN  // libjpeg under Unixware seems to need this
#endif
#include <jpeglib.h>
#include <jerror.h>
#undef HAVE_STDLIB_H
#ifdef const
#  undef const          // remove crazy C hackery in jconfig.h
#endif
}

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
		RGBProof = 2,
		RawData = 3,
		Thumbnail = 4,
	};

	enum ImageEffectCode
	{
		EF_INVERT = 0,
		EF_GRAYSCALE = 1,
		EF_COLORIZE = 2,
		EF_BRIGHTNESS = 3,
		EF_CONTRAST = 4,
		EF_SHARPEN = 5,
		EF_BLUR = 6,
		EF_SOLARIZE = 7
	};

	struct imageEffect
	{
		int effectCode;
		QString effectParameters;
	};
	void initialize();

	const QImage& qImage();
	QImage smoothScale(int h, int w, QImage::ScaleMode mode = ScaleFree) const;
	
	int height() const { return QImage::height(); }
	int width() const { return QImage::width(); }
	bool hasAlpha() const { return QImage::hasAlphaBuffer(); }
	
	// Routines for PDF/PS output of images
	QByteArray ImageToArray();
	QByteArray ImageToGray();
	QByteArray ImageToCMYK_PS(int pl, bool pre);
	QByteArray ImageToCMYK_PDF(bool pre);
	QByteArray getAlpha(QString fn, bool PDF, bool pdf14, int gsRes = 72);
	void Convert2JPG(QString fn, int Quality, bool isCMYK, bool isGray);

	// Image effects
	void applyEffect(QValueList<imageEffect> effectsList, QMap<QString,ScColor> colors, bool cmyk);

	// Generate a low res image for user preview
	void createLowRes(double scale);

	// Scale this image in-place
	void scaleImage(int width, int height);

	// Retrieve an embedded ICC profile from the file path `fn', storing it in `profile'.
	// TODO: Bad API. Should probably be static member returning an ICCProfile (custom class) or something like that.
	void getEmbeddedProfile(const QString & fn, QByteArray *profile, int *components);

	// Load an image into this ScImage instance
	// TODO: document params, split into smaller functions
	bool LoadPicture(const QString & fn, const CMSettings& cmSettings, bool useEmbedded, bool useProf, RequestType requestType, int gsRes, bool *realCMYK = 0, bool showMsg = false);

	ImageInfoRecord imgInfo;

private:
	// Image effects
	void solarize(double factor, bool cmyk);
//	void blur(double radius= 0.0, double sigma = 1.0);
	void blur(int radius = 0);
	void sharpen(double radius= 0.0, double sigma = 1.0);
	void contrast(int contrastValue, bool cmyk);
	void brightness(int brightnessValue, bool cmyk);
	void invert(bool cmyk);
	void colorize(ScColor color, int shade, bool cmyk);
	void toGrayscale(bool cmyk);
	void swapRGBA();

	// Misc implementation
	void liberateMemory(void **memory);
//	void blurScanLine(double *kernel, int width, unsigned int *src, unsigned int *dest, int columns);
//	int getBlurKernel(int width, double sigma, double **kernel);
	bool convolveImage(QImage *dest, const unsigned int order, const double *kernel);
	int getOptimalKernelWidth(double radius, double sigma);
	void applyCurve(bool cmyk);
	char* iccbuf;
	uint icclen;
	//std::valarray<int> curveTable;
	QMemArray<int> curveTable;
	QValueList<unsigned int> colorTable;
	int random_table[4096];
	
};
#endif
