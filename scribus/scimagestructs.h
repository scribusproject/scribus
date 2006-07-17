#ifndef SCIMAGESTRUCTS_H
#define SCIMAGESTRUCTS_H

#include <qstring.h>
#include <qvaluelist.h>
#include <qimage.h>
#include <qmap.h>
#include "fpointarray.h"

struct ImageLoadRequest
{
	bool visible;
	ushort opacity;
	QString blend;
};

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
	QValueList<uint> channelLen;
	QValueList<int> channelType;
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
};

class ExifValues
{
public:
	ExifValues(void);
	void init(void);

	int width;
	int height;
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
	int colorspace; /* 0 = RGB  1 = CMYK  2 = Grayscale */
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
	QValueList<PSDLayer> layerInfo;
	ExifValues exifInfo;
};

#endif
