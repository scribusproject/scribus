/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QtTest/QtTest>

#include "imagealphacontour.h"

class ImageAlphaContourTests : public QObject
{
	Q_OBJECT

private slots:
	void tracesTransparentSilhouette();
	void respectsThreshold();
	void rejectsOpaqueAndEmptyMasks();
	void preservesLargeImageCoordinates();
	void keepsTransparentHoles();
	void tracesLuminanceAndContrast();
};

void ImageAlphaContourTests::tracesTransparentSilhouette()
{
	QImage image(8, 8, QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	for (int y = 2; y < 6; ++y)
		for (int x = 1; x < 5; ++x)
			image.setPixelColor(x, y, QColor(255, 0, 0, 255));
	const QPainterPath path = imageAlphaSilhouette(image);
	QCOMPARE(path.boundingRect(), QRectF(1, 2, 4, 4));
	QVERIFY(path.contains(QPointF(2, 3)));
	QVERIFY(!path.contains(QPointF(6, 3)));
}

void ImageAlphaContourTests::respectsThreshold()
{
	QImage image(3, 3, QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	image.setPixelColor(1, 1, QColor(0, 0, 0, 100));
	QVERIFY(!imageAlphaSilhouette(image, 50).isEmpty());
	QVERIFY(imageAlphaSilhouette(image, 128).isEmpty());
}

void ImageAlphaContourTests::rejectsOpaqueAndEmptyMasks()
{
	QImage image(4, 4, QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	QVERIFY(imageAlphaSilhouette(image).isEmpty());
	image.fill(Qt::black);
	QVERIFY(imageAlphaSilhouette(image).isEmpty());
	QVERIFY(imageAlphaSilhouette(image, 0).isEmpty());
}

void ImageAlphaContourTests::preservesLargeImageCoordinates()
{
	QImage image(1024, 1024, QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	for (int y = 256; y < 768; ++y)
		for (int x = 256; x < 768; ++x)
			image.setPixelColor(x, y, QColor(255, 0, 0, 255));
	const QRectF bounds = imageAlphaSilhouette(image).boundingRect();
	QVERIFY(qAbs(bounds.left() - 256) <= 2);
	QVERIFY(qAbs(bounds.top() - 256) <= 2);
	QVERIFY(qAbs(bounds.right() - 768) <= 2);
	QVERIFY(qAbs(bounds.bottom() - 768) <= 2);
}

void ImageAlphaContourTests::keepsTransparentHoles()
{
	QImage image(10, 10, QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	for (int y = 1; y < 9; ++y)
		for (int x = 1; x < 9; ++x)
			if (x < 4 || x >= 6 || y < 4 || y >= 6)
				image.setPixelColor(x, y, QColor(0, 0, 0, 255));
	const QPainterPath path = imageAlphaSilhouette(image);
	QVERIFY(path.contains(QPointF(2, 2)));
	QVERIFY(!path.contains(QPointF(5, 5)));
}

void ImageAlphaContourTests::tracesLuminanceAndContrast()
{
	QImage image(12, 12, QImage::Format_RGB32);
	image.fill(Qt::white);
	for (int y = 3; y < 9; ++y)
		for (int x = 2; x < 10; ++x)
			image.setPixelColor(x, y, Qt::black);
	const QPainterPath luminance = imageLuminanceSilhouette(image, 128);
	const QPainterPath contrast = imageContrastSilhouette(image, 64);
	QCOMPARE(luminance.boundingRect(), QRectF(2, 3, 8, 6));
	QCOMPARE(contrast.boundingRect(), luminance.boundingRect());
	QVERIFY(!imageContrastSilhouette(image, 255).isEmpty());
}

QTEST_MAIN(ImageAlphaContourTests)
#include "imagealphacontourtests.moc"
