/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtTest>

#include <QFile>
#include <QTemporaryDir>
#include <tiffio.h>

#include "imagecmykexport.h"

class ImageCMYKExportTests : public QObject
{
	Q_OBJECT

private slots:
	void writesSeparatedTiffWithProfileAndResolution();
	void neverOverwritesAnExistingFile();
	void rejectsInvalidInputs();
};

void ImageCMYKExportTests::writesSeparatedTiffWithProfileAndResolution()
{
	QTemporaryDir directory;
	QVERIFY(directory.isValid());
	const QString path = directory.filePath(QStringLiteral("proof.tif"));
	QImage pixels(2, 1, QImage::Format_ARGB32);
	pixels.setPixel(0, 0, qRgba(10, 20, 30, 40));
	pixels.setPixel(1, 0, qRgba(50, 60, 70, 80));
	const QByteArray profile("test-profile-bytes");
	QString error;
	QVERIFY2(writeCMYKTiffCopy(path, pixels, profile, 300, 240, &error), qPrintable(error));

	TIFF* tif = TIFFOpen(QFile::encodeName(path).constData(), "r");
	QVERIFY(tif != nullptr);
	uint16_t photo = 0, samples = 0, inkset = 0, resolutionUnit = 0;
	uint32_t width = 0, height = 0, profileSize = 0;
	void* profileData = nullptr;
	float xResolution = 0, yResolution = 0;
	QVERIFY(TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width));
	QVERIFY(TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height));
	QVERIFY(TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photo));
	QVERIFY(TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples));
	QVERIFY(TIFFGetField(tif, TIFFTAG_INKSET, &inkset));
	QVERIFY(TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &resolutionUnit));
	QVERIFY(TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xResolution));
	QVERIFY(TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yResolution));
	QVERIFY(TIFFGetField(tif, TIFFTAG_ICCPROFILE, &profileSize, &profileData));
	QCOMPARE(width, 2U);
	QCOMPARE(height, 1U);
	QCOMPARE(photo, static_cast<uint16_t>(PHOTOMETRIC_SEPARATED));
	QCOMPARE(samples, static_cast<uint16_t>(4));
	QCOMPARE(inkset, static_cast<uint16_t>(INKSET_CMYK));
	QCOMPARE(resolutionUnit, static_cast<uint16_t>(RESUNIT_INCH));
	QCOMPARE(xResolution, 300.0f);
	QCOMPARE(yResolution, 240.0f);
	QCOMPARE(QByteArray(static_cast<const char*>(profileData), profileSize), profile);
	uchar row[8] = {};
	QVERIFY(TIFFReadScanline(tif, row, 0) >= 0);
	QCOMPARE(QByteArray(reinterpret_cast<const char*>(row), 8),
		QByteArray::fromHex("0a141e28323c4650"));
	TIFFClose(tif);
}

void ImageCMYKExportTests::neverOverwritesAnExistingFile()
{
	QTemporaryDir directory;
	QVERIFY(directory.isValid());
	const QString path = directory.filePath(QStringLiteral("original.tif"));
	QFile file(path);
	QVERIFY(file.open(QIODevice::WriteOnly));
	QCOMPARE(file.write("original"), 8LL);
	file.close();
	QImage pixels(1, 1, QImage::Format_ARGB32);
	pixels.fill(qRgba(1, 2, 3, 4));
	QVERIFY(!writeCMYKTiffCopy(path, pixels, QByteArray("profile"), 72, 72));
	QVERIFY(file.open(QIODevice::ReadOnly));
	QCOMPARE(file.readAll(), QByteArray("original"));
}

void ImageCMYKExportTests::rejectsInvalidInputs()
{
	QTemporaryDir directory;
	QVERIFY(directory.isValid());
	const QString path = directory.filePath(QStringLiteral("invalid.tif"));
	QImage rgb(1, 1, QImage::Format_RGB32);
	QVERIFY(!writeCMYKTiffCopy(path, rgb, QByteArray("profile"), 72, 72));
	QVERIFY(!QFile::exists(path));
	QImage cmyk(1, 1, QImage::Format_ARGB32);
	QVERIFY(!writeCMYKTiffCopy(path, cmyk, QByteArray(), 72, 72));
	QVERIFY(!writeCMYKTiffCopy(path, cmyk, QByteArray("profile"), 0, 72));
	QVERIFY(!QFile::exists(path));
}

QTEST_GUILESS_MAIN(ImageCMYKExportTests)
#include "imagecmykexporttests.moc"
