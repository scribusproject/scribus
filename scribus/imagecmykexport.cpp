/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "imagecmykexport.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QTemporaryFile>
#include <QVector>
#include <string>
#include <tiffio.h>

namespace
{
void setError(QString* error, const QString& message)
{
	if (error)
		*error = message;
}

TIFF* openTiffForWrite(const QString& path)
{
#ifdef Q_OS_WIN
	const std::wstring widePath = QDir::toNativeSeparators(path).toStdWString();
	return TIFFOpenW(widePath.c_str(), "w");
#else
	return TIFFOpen(QFile::encodeName(path).constData(), "w");
#endif
}
}

bool writeCMYKTiffCopy(const QString& destination, const QImage& cmyk,
	const QByteArray& iccProfile, int xResolution, int yResolution, QString* error)
{
	if (destination.isEmpty() || QFileInfo::exists(destination) || QFileInfo(destination).isSymLink() || cmyk.isNull()
		|| cmyk.format() != QImage::Format_ARGB32 || iccProfile.isEmpty()
		|| xResolution <= 0 || yResolution <= 0)
	{
		setError(error, QObject::tr("Invalid CMYK image, ICC profile, resolution, or destination; existing files are never replaced."));
		return false;
	}

	QTemporaryFile temporary(QFileInfo(destination).absolutePath() + QStringLiteral("/.scribus-cmyk-XXXXXX.tif"));
	if (!temporary.open())
	{
		setError(error, QObject::tr("Cannot create a temporary file beside the destination."));
		return false;
	}
	const QString temporaryPath = temporary.fileName();
	temporary.close();
	TIFF* tif = openTiffForWrite(temporaryPath);
	if (!tif)
	{
		setError(error, QObject::tr("Cannot open the temporary TIFF for writing."));
		return false;
	}

	bool ok = true;
	ok &= TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, static_cast<uint32_t>(cmyk.width())) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_IMAGELENGTH, static_cast<uint32_t>(cmyk.height())) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 4) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_SEPARATED) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_INKSET, INKSET_CMYK) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_XRESOLUTION, static_cast<float>(xResolution)) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_YRESOLUTION, static_cast<float>(yResolution)) == 1;
	ok &= TIFFSetField(tif, TIFFTAG_ICCPROFILE, static_cast<uint32_t>(iccProfile.size()), iccProfile.constData()) == 1;

	QVector<uchar> row(cmyk.width() * 4);
	for (int y = 0; ok && y < cmyk.height(); ++y)
	{
		const auto* pixels = reinterpret_cast<const QRgb*>(cmyk.constScanLine(y));
		for (int x = 0; x < cmyk.width(); ++x)
		{
			row[4 * x] = qRed(pixels[x]);
			row[4 * x + 1] = qGreen(pixels[x]);
			row[4 * x + 2] = qBlue(pixels[x]);
			row[4 * x + 3] = qAlpha(pixels[x]);
		}
		ok = TIFFWriteScanline(tif, row.data(), static_cast<uint32_t>(y)) >= 0;
	}
	TIFFClose(tif);
	if (!ok || QFileInfo::exists(destination) || QFileInfo(destination).isSymLink()
		|| !QFile::rename(temporaryPath, destination))
	{
		setError(error, QObject::tr("The CMYK TIFF could not be completed. The destination was not changed."));
		return false;
	}
	temporary.setAutoRemove(false);
	return true;
}
