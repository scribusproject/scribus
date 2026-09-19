/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtTest>

#include <QFile>
#include <QTemporaryDir>

#include "embeddedimageextractor.h"

class EmbeddedImageExtractorTests : public QObject
{
	Q_OBJECT

private slots:
	void sanitizesCrossPlatformFileNames();
	void preservesExtensionAndUsesFallbackName();
	void resolvesConflictsWithoutOverwriting();
	void copiesBytesExactlyAndHonoursReplacePolicy();
};

void EmbeddedImageExtractorTests::sanitizesCrossPlatformFileNames()
{
	QCOMPARE(sanitizedImageBaseName(QStringLiteral("  Cover: Front?.png.  ")), QStringLiteral("Cover_ Front_.png"));
	QCOMPARE(sanitizedImageBaseName(QStringLiteral("CON")), QStringLiteral("_CON"));
	QCOMPARE(sanitizedImageBaseName(QStringLiteral("lpt9")), QStringLiteral("_lpt9"));
	QCOMPARE(sanitizedImageBaseName(QStringLiteral("Chapter   opener")), QStringLiteral("Chapter opener"));
}

void EmbeddedImageExtractorTests::preservesExtensionAndUsesFallbackName()
{
	QCOMPARE(embeddedImageExtension(QStringLiteral("/tmp/scribus_temp_1234.TIFF")), QStringLiteral("TIFF"));
	QCOMPARE(embeddedImageExtension(QStringLiteral("/tmp/no-extension")), QStringLiteral("img"));
	QCOMPARE(suggestedEmbeddedImageFileName(QString(), QStringLiteral("/tmp/source.PNG"), 3),
		QStringLiteral("Embedded Image 3.PNG"));
}

void EmbeddedImageExtractorTests::resolvesConflictsWithoutOverwriting()
{
	QTemporaryDir directory;
	QVERIFY(directory.isValid());
	QFile existing(directory.filePath(QStringLiteral("Cover.png")));
	QVERIFY(existing.open(QIODevice::WriteOnly));
	existing.write("existing");
	existing.close();

	QSet<QString> reserved;
	auto kept = resolveImageExtractionPath(directory.path(), QStringLiteral("Cover.png"),
		ImageExtractionConflict::KeepBoth, &reserved);
	QVERIFY(kept.renamed);
	QVERIFY(!kept.skipped);
	QCOMPARE(QFileInfo(kept.path).fileName(), QStringLiteral("Cover-2.png"));

	auto second = resolveImageExtractionPath(directory.path(), QStringLiteral("Cover.png"),
		ImageExtractionConflict::KeepBoth, &reserved);
	QCOMPARE(QFileInfo(second.path).fileName(), QStringLiteral("Cover-3.png"));

	auto skipped = resolveImageExtractionPath(directory.path(), QStringLiteral("Cover.png"),
		ImageExtractionConflict::Skip);
	QVERIFY(skipped.skipped);
	QVERIFY(skipped.path.isEmpty());

	auto replaced = resolveImageExtractionPath(directory.path(), QStringLiteral("Cover.png"),
		ImageExtractionConflict::Replace, &reserved);
	QCOMPARE(QFileInfo(replaced.path).fileName(), QStringLiteral("Cover.png"));
	auto uniqueReplacement = resolveImageExtractionPath(directory.path(), QStringLiteral("Cover.png"),
		ImageExtractionConflict::Replace, &reserved);
	QCOMPARE(QFileInfo(uniqueReplacement.path).fileName(), QStringLiteral("Cover-4.png"));
}

void EmbeddedImageExtractorTests::copiesBytesExactlyAndHonoursReplacePolicy()
{
	QTemporaryDir directory;
	QVERIFY(directory.isValid());
	const QString sourcePath = directory.filePath(QStringLiteral("source.psd"));
	const QString destinationPath = directory.filePath(QStringLiteral("output.psd"));
	const QByteArray originalBytes = QByteArray::fromHex("00010203ff5043443800000000a5");
	QFile source(sourcePath);
	QVERIFY(source.open(QIODevice::WriteOnly));
	QCOMPARE(source.write(originalBytes), originalBytes.size());
	source.close();

	QString error;
	QVERIFY2(copyEmbeddedImageBytes(sourcePath, destinationPath, false, &error), qPrintable(error));
	QFile extracted(destinationPath);
	QVERIFY(extracted.open(QIODevice::ReadOnly));
	QCOMPARE(extracted.readAll(), originalBytes);
	extracted.close();

	QVERIFY(!copyEmbeddedImageBytes(sourcePath, destinationPath, false, &error));
	QVERIFY(error.contains(QStringLiteral("already exists")));
	QVERIFY(copyEmbeddedImageBytes(sourcePath, destinationPath, true, &error));
	QVERIFY(!copyEmbeddedImageBytes(sourcePath, sourcePath, true, &error));
	QVERIFY(error.contains(QStringLiteral("same file")));
}

QTEST_GUILESS_MAIN(EmbeddedImageExtractorTests)
#include "embeddedimageextractortests.moc"
