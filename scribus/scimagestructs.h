/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMAGESTRUCTS_H
#define SCIMAGESTRUCTS_H

#include <QImage>
#include <QList>
#include <QMap>
#include <QString>

#include "fpointarray.h"
#include "sccolor.h"

struct ImageLoadRequest
{
	bool visible;
	bool useMask;
	ushort opacity;
	QString blend;
};

struct ImageEffect
{
	int effectCode;
	QString effectParameters;
};
typedef QList<ImageEffect> ScImageEffectList;

struct PSDHeader
{
	uint signature;
	ushort version;
	uchar reserved[6];
	ushort channel_count;
	uint height;
	uint width;
	ushort depth;
	ushort color_mode;
};

struct PSDLayer
{
	QList<uint> channelLen;
	QList<int> channelType;
	int xpos;
	int ypos;
	int width;
	int height;
	ushort opacity;
	uchar clipping;
	uchar flags;
	int maskXpos;
	int maskYpos;
	int maskWidth;
	int maskHeight;
	QString layerName;
	QString blend;
	QImage thumb;
	QImage thumb_mask;
};

struct PSDDuotone_Color
{
	QString Name;
	ScColor Color;
	FPointArray Curve;
};

class ExifValues
{
public:
	ExifValues(void);
	void init(void);

	int width;
	int height;
	float ExposureTime;
	float ApertureFNumber;
	int   ISOequivalent;
	QString cameraName;
	QString cameraVendor;
	QString comment;
	QString userComment;
	QString artist;
	QString copyright;
	QString dateTime;
	QImage thumbnail;
};

class ImageInfoRecord
{
public:
	ImageInfoRecord(void);
	void init(void);

	int type;			/* 0 = jpg, 1 = tiff, 2 = psd, 3 = eps/ps, 4 = pdf, 5 = jpg2000, 6 = other */
	int xres;
	int yres;
	int BBoxX;
	int BBoxH;
	int colorspace; /* 0 = RGB  1 = CMYK  2 = Grayscale 3 = Duotone */
	bool valid;
	bool isRequest;
	bool progressive;
	bool isEmbedded;
	bool exifDataValid;
	int lowResType; /* 0 = full Resolution, 1 = 72 dpi, 2 = 36 dpi */
	double lowResScale;
	QMap<QString, FPointArray> PDSpathData;
	QMap<int, ImageLoadRequest> RequestProps;
	QString clipPath;
	QString usedPath;
	QString profileName;
	QList<PSDLayer> layerInfo;
	QList<PSDDuotone_Color> duotoneColors;
	ExifValues exifInfo;
};

#endif
