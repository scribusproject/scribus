/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scimagecacheproxy.h"
#include "scimagestructs.h"

#include <QByteArray>
#include <QDataStream>

#define SC_DEBUG_FILE defined(DEBUG_SCIMAGECACHE)
#include "scdebug.h"

namespace {
	const QDataStream::Version dsVersion = QDataStream::Qt_4_0;
}

const qint32 ExifValues::dsVersion = 1;

QDataStream & operator<< (QDataStream& stream, const ExifValues & exif)
{
	stream << static_cast<qint32>(exif.width) << static_cast<qint32>(exif.height) << exif.ExposureTime
		   << exif.ApertureFNumber << static_cast<qint32>(exif.ISOequivalent) << exif.cameraName
		   << exif.cameraVendor << exif.comment << exif.userComment << exif.artist << exif.copyright
		   << exif.dateTime << exif.thumbnail;
	return stream;
}

QDataStream & operator>> (QDataStream& stream, ExifValues & exif)
{
	qint32 w, h, iso;
	stream >> w >> h >> exif.ExposureTime >> exif.ApertureFNumber >> iso >> exif.cameraName
		   >> exif.cameraVendor >> exif.comment >> exif.userComment >> exif.artist >> exif.copyright
		   >> exif.dateTime >> exif.thumbnail;
	exif.width = w;
	exif.height = h;
	exif.ISOequivalent = iso;
	return stream;
}

ExifValues::ExifValues(void)
{
	init();
}

void ExifValues::init(void)
{
	width = 0;
	height = 0;
	ExposureTime = 0;
	ApertureFNumber = 0;
	ISOequivalent = 0;
	cameraName.resize(0);
	cameraVendor.resize(0);
	comment.resize(0);
	userComment.resize(0);
	artist.resize(0);
	copyright.resize(0);
	dateTime.resize(0);
	thumbnail = QImage();
}

const qint32 ImageInfoRecord::iirVersion = 1;

ImageInfoRecord::ImageInfoRecord(void)
{
	init();
}

void ImageInfoRecord::init(void)
{
	type = ImageTypeOther;	/* 0 = jpg, 1 = tiff, 2 = psd, 3 = eps/ps, 4 = pdf, 5 = jpg2000, 6 = other */
	xres = 72;
	yres = 72;
	BBoxX = 0;
	BBoxH = 0;
	colorspace = ColorSpaceRGB; /* 0 = RGB  1 = CMYK  2 = Grayscale 3 = Duotone */
	valid = false;
	isRequest = false;
	progressive = false;
	isEmbedded = false;
	exifDataValid = false;
	lowResType = 1; /* 0 = full Resolution, 1 = 72 dpi, 2 = 36 dpi */
	lowResScale = 1.0;
	PDSpathData.clear();
	RequestProps.clear();
	numberOfPages = 1;
	actualPageNumber = 0; // default
	clipPath.resize(0);
	usedPath.resize(0);
	profileName.resize(0);
	layerInfo.clear();
	duotoneColors.clear();
	exifInfo.init();
}

bool ImageInfoRecord::canSerialize() const
{
	return PDSpathData.empty() && RequestProps.empty() && layerInfo.empty() && duotoneColors.empty();
}

bool ImageInfoRecord::serialize(ScImageCacheProxy & cache) const
{
	if (!canSerialize())
	{
		scDebug() << "cannot serialize" << PDSpathData.empty() << RequestProps.empty() << layerInfo.empty() << duotoneColors.empty();
		return false;
	}

	cache.addInfo("iirVersion", QString::number(iirVersion));
	cache.addInfo("type", QString::number(static_cast<int>(type)));
	cache.addInfo("xres", QString::number(xres));
	cache.addInfo("yres", QString::number(yres));
	cache.addInfo("BBoxX", QString::number(BBoxX));
	cache.addInfo("BBoxH", QString::number(BBoxH));
	cache.addInfo("colorspace", QString::number(static_cast<int>(colorspace)));
	cache.addInfo("valid", QString::number(static_cast<int>(valid)));
	cache.addInfo("isRequest", QString::number(static_cast<int>(isRequest)));
	cache.addInfo("progressive", QString::number(static_cast<int>(progressive)));
	cache.addInfo("isEmbedded", QString::number(static_cast<int>(isEmbedded)));
	cache.addInfo("lowResType", QString::number(lowResType));
	cache.addInfo("lowResScale", QString::number(lowResScale, 'g', 15));
	cache.addInfo("clipPath", clipPath);
	cache.addInfo("profileName", profileName);

	if (exifDataValid)
	{
		QByteArray exif;
		QDataStream es(&exif, QIODevice::WriteOnly);
		es.setVersion(dsVersion);
		es << ExifValues::dsVersion;
		es << exifInfo;
		cache.addInfo("exifInfo", exif.toBase64());
	}

	return true;
}

bool ImageInfoRecord::deserialize(const ScImageCacheProxy & cache)
{
	PDSpathData.clear();
	RequestProps.clear();
	layerInfo.clear();
	duotoneColors.clear();
	usedPath.resize(0);
	int v1 = cache.getInfo("iirVersion").toInt();
	if (v1 != iirVersion)
	{
		scDebug() << "image info version mismatch" << v1 << "!=" << iirVersion;
		return false;
	}
	type = static_cast<ImageTypeEnum>(cache.getInfo("type").toInt());
	xres = cache.getInfo("xres").toInt();
	yres = cache.getInfo("yres").toInt();
	BBoxX = cache.getInfo("BBoxX").toInt();
	BBoxH = cache.getInfo("BBoxH").toInt();
	colorspace = static_cast<ColorSpaceEnum>(cache.getInfo("colorspace").toInt());
	valid = cache.getInfo("valid").toInt() != 0;
	isRequest = cache.getInfo("isRequest").toInt() != 0;
	progressive = cache.getInfo("progressive").toInt() != 0;
	isEmbedded = cache.getInfo("isEmbedded").toInt() != 0;
	lowResType = cache.getInfo("lowResType").toInt();
	lowResScale = cache.getInfo("lowResScale").toDouble();
	clipPath = cache.getInfo("clipPath");
	profileName = cache.getInfo("profileName");
	QString exifData = cache.getInfo("exifInfo");
	exifDataValid = !exifData.isNull();
	if (exifDataValid)
	{
		QByteArray exif = QByteArray::fromBase64(exifData.toAscii());
		QDataStream es(exif);
		es.setVersion(dsVersion);
		qint32 v2;
		es >> v2;
		if (v2 != ExifValues::dsVersion)
		{
			scDebug() << "exif version mismatch" << v2 << "!=" << ExifValues::dsVersion;
			return false;
		}
		es >> exifInfo;
	}

	return true;
}

