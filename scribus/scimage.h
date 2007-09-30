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
class CMSettings;

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
		EF_SOLARIZE = 7,
		EF_DUOTONE = 8,
		EF_TRITONE = 9,
		EF_QUADTONE = 10,
		EF_GRADUATE = 11
	};

	struct imageEffect
	{
		int effectCode;
		QString effectParameters;
	};
	void initialize();

	const QImage& qImage();
	QImage* qImagePtr();
	QImage scaled(int h, int w, Qt::AspectRatioMode mode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation) const;
	
	int height() const { return QImage::height(); }
	int width() const { return QImage::width(); }
	bool hasAlpha() const { return QImage::hasAlphaChannel(); }
	
	// Routines for PDF/PS output of images
	QByteArray ImageToArray();
	QByteArray ImageToGray();
	QByteArray ImageToCMYK_PS(int pl, bool pre);
	QByteArray ImageToCMYK_PDF(bool pre);
	QByteArray getAlpha(QString fn, bool PDF, bool pdf14, int gsRes = 72, int scaleXSize = 0, int scaleYSize = 0);
	void Convert2JPG(QString fn, int Quality, bool isCMYK, bool isGray);

	// Image effects
	void applyEffect(QList<imageEffect> effectsList, ColorList& colors, bool cmyk);

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
	void blur(int radius = 0);
	void sharpen(double radius= 0.0, double sigma = 1.0);
	void contrast(int contrastValue, bool cmyk);
	void brightness(int brightnessValue, bool cmyk);
	void invert(bool cmyk);
	void colorize(ScribusDoc* doc, ScColor color, int shade, bool cmyk);
	void duotone(ScribusDoc* doc, ScColor color1, int shade1, FPointArray curve1, bool lin1, ScColor color2, int shade2, FPointArray curve2, bool lin2, bool cmyk);
	void tritone(ScribusDoc* doc, ScColor color1, int shade1, FPointArray curve1, bool lin1, ScColor color2, int shade2, FPointArray curve2, bool lin2, ScColor color3, int shade3, FPointArray curve3, bool lin3, bool cmyk);
	void quadtone(ScribusDoc* doc, ScColor color1, int shade1, FPointArray curve1, bool lin1, ScColor color2, int shade2, FPointArray curve2, bool lin2, ScColor color3, int shade3, FPointArray curve3, bool lin3, ScColor color4, int shade4, FPointArray curve4, bool lin4, bool cmyk);
	void toGrayscale(bool cmyk);
	void doGraduate(FPointArray curve, bool cmyk, bool linear);
	void swapRGBA();
	bool convolveImage(QImage *dest, const unsigned int order, const double *kernel);
	int getOptimalKernelWidth(double radius, double sigma);
	void applyCurve(bool cmyk);
	char* iccbuf;
	uint icclen;
	QVector<int> curveTable;
	QList<unsigned int> colorTable;
	int random_table[4096];
	
};

typedef QList<ScImage::imageEffect> ScImageEffectList;

#endif
