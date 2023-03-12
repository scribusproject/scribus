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

class ScImageCacheProxy;

struct ImageLoadRequest
{
	bool visible;
	bool useMask;
	ushort opacity;
	QString blend;
	bool operator==(const ImageLoadRequest &rhs) const
	{
		return visible == rhs.visible && useMask == rhs.useMask && opacity == rhs.opacity && blend == rhs.blend;
	}
};

struct ImageEffect
{
	enum EffectCode
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

	int effectCode;
	QString effectParameters;

	bool operator==(const ImageEffect& other) const
	{
		if (effectCode != other.effectCode)
			return false;
		if (effectParameters != other.effectParameters)
			return false;
		return true;
	}
};

class ScImageEffectList : public QList<ImageEffect>
{
public:
	bool useColorEffect() const
	{
		int effectCount = this->count();
		if (effectCount <= 0)
			return false;

		for (int i = 0; i < effectCount; ++i)
		{
			const auto& effect = at(i);
			if (effect.effectCode == ImageEffect::EF_COLORIZE)
				return true;
			if (effect.effectCode == ImageEffect::EF_DUOTONE)
				return true;
			if (effect.effectCode == ImageEffect::EF_TRITONE)
				return true;
			if (effect.effectCode == ImageEffect::EF_QUADTONE)
				return true;
		}
		return false;
	}
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
	ExifValues();
	void init();

	// Remember to increment this version number and update
	// the QDataStream operators if this class in changed.
	static constexpr qint32 dsVersion { 1 };

	int width { 0 };
	int height { 0 };
	int   orientation { 1 };
	float ExposureTime { 0.0f };
	float ApertureFNumber { 0.0f };
	int   ISOequivalent { 0 };
	QString cameraName;
	QString cameraVendor;
	QString comment;
	QString userComment;
	QString artist;
	QString copyright;
	QString dateTime;
	QImage thumbnail;
};

enum ImageTypeEnum
{
	ImageTypeJPG = 0,
	ImageTypeTIF = 1,
	ImageTypePSD = 2,
	ImageTypeEPS = 3,
	ImageTypePDF = 4,
	ImageTypeJPG2K = 5,
	ImageTypeOther = 6,
	ImageType7 = 7
};

enum ColorSpaceEnum
{
	ColorSpaceRGB  = 0,
	ColorSpaceCMYK = 1,
	ColorSpaceGray = 2,
	ColorSpaceDuotone = 3,
	ColorSpaceMonochrome = 4
};

class ImageInfoRecord
{
public:
	ImageInfoRecord();
	void init();

	// Remember to increment this version number and update
	// the serialization routines if this class in changed.
	static constexpr int iirVersion { 1 };

	bool canSerialize() const;
	bool serialize(ScImageCacheProxy & cache) const;
	bool deserialize(const ScImageCacheProxy & cache);

	ImageTypeEnum type { ImageTypeOther };			/* 0 = jpg, 1 = tiff, 2 = psd, 3 = eps/ps, 4 = pdf, 5 = jpg2000, 6 = other */
	int  xres { 72 };
	int  yres { 72 };
	int  BBoxX { 0 };
	int  BBoxH { 0 };
	ColorSpaceEnum colorspace { ColorSpaceRGB }; /* 0 = RGB  1 = CMYK  2 = Grayscale 3 = Duotone */
	bool valid { false };
	bool isRequest { false };
	bool progressive { false };
	bool isEmbedded { false };
	bool exifDataValid { false };
	int  lowResType { 1 }; /* 0 = full Resolution, 1 = 72 dpi, 2 = 36 dpi */
	double lowResScale { 1.0 };
	int numberOfPages { 1 };
	int actualPageNumber { 0 };
	QMap<QString, FPointArray> PDSpathData;
	QMap<int, ImageLoadRequest> RequestProps;
	QString clipPath;
	QString usedPath;
	QString profileName;
	QString embeddedProfileName;
	QList<PSDLayer> layerInfo;
	QList<PSDDuotone_Color> duotoneColors;
	ExifValues exifInfo;
};

#endif
