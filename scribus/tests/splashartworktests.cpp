/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QImage>
#include <QSet>
#include <QTest>

class SplashArtworkTests : public QObject
{
	Q_OBJECT

private slots:
	void lightAndDarkHaveFinishedArtwork();
};

void SplashArtworkTests::lightAndDarkHaveFinishedArtwork()
{
	const QString root = QStringLiteral(SPLASH_ARTWORK_DIR);
	const QImage light(root + QStringLiteral("/scribus_splash_light.png"));
	const QImage dark(root + QStringLiteral("/scribus_splash_dark.png"));
	QVERIFY(!light.isNull());
	QVERIFY(!dark.isNull());
	QCOMPARE(light.size(), QSize(720, 360));
	QCOMPARE(dark.size(), light.size());
	QVERIFY(light.pixelColor(400, 20).lightness() > dark.pixelColor(400, 20).lightness());

	// The page art must not regress to a blank frame and crossed placeholder.
	for (const QImage& artwork : {light, dark})
	{
		QSet<QRgb> colors;
		for (int y = 48; y < 150; y += 6)
			for (int x = 450; x < 620; x += 6)
				colors.insert(artwork.pixel(x, y));
		QVERIFY2(colors.size() >= 8, "splash page artwork has too little detail");
	}
}

QTEST_GUILESS_MAIN(SplashArtworkTests)
#include "splashartworktests.moc"
