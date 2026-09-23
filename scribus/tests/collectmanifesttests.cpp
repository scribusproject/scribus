/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtTest>

#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>

#include "collectmanifest.h"

class CollectManifestTests : public QObject
{
	Q_OBJECT
private slots:
	void writesChecksumsAndLicenseReview();
	void rejectsDocumentOutsideCollection();
};

void CollectManifestTests::writesChecksumsAndLicenseReview()
{
	QTemporaryDir temporary;
	QVERIFY(temporary.isValid());
	QDir directory(temporary.path());
	QVERIFY(directory.mkpath(QStringLiteral("images")));
	QVERIFY(directory.mkpath(QStringLiteral("fonts")));
	const QString document = directory.filePath(QStringLiteral("book.sla"));
	const QString image = directory.filePath(QStringLiteral("images/picture.png"));
	const QString font = directory.filePath(QStringLiteral("fonts/type.otf"));
	for (const QString& path : {document, image, font})
	{
		QFile file(path);
		QVERIFY(file.open(QIODevice::WriteOnly));
		QCOMPARE(file.write("test"), 4LL);
	}
	QString error;
	QVERIFY2(writeCollectManifest(directory.path(), document, &error), qPrintable(error));
	QFile manifest(directory.filePath(QStringLiteral("collect-manifest.json")));
	QVERIFY(manifest.open(QIODevice::ReadOnly));
	const QJsonObject data = QJsonDocument::fromJson(manifest.readAll()).object();
	QCOMPARE(data.value(QStringLiteral("formatVersion")).toInt(), 1);
	const QJsonArray files = data.value(QStringLiteral("files")).toArray();
	QCOMPARE(files.size(), 3);
	const QString expectedHash = QString::fromLatin1(QCryptographicHash::hash("test", QCryptographicHash::Sha256).toHex());
	for (const QJsonValue& value : files)
	{
		const QJsonObject entry = value.toObject();
		QCOMPARE(entry.value(QStringLiteral("sha256")).toString(), expectedHash);
		if (entry.value(QStringLiteral("kind")).toString() != QLatin1String("document"))
			QCOMPARE(entry.value(QStringLiteral("licenseReview")).toString(), QStringLiteral("required"));
	}
	QVERIFY(QFileInfo::exists(directory.filePath(QStringLiteral("PRINT-INSTRUCTIONS.txt"))));
}

void CollectManifestTests::rejectsDocumentOutsideCollection()
{
	QTemporaryDir directory;
	QTemporaryDir other;
	QVERIFY(directory.isValid() && other.isValid());
	const QString document = QDir(other.path()).filePath(QStringLiteral("other.sla"));
	QFile file(document);
	QVERIFY(file.open(QIODevice::WriteOnly));
	QCOMPARE(file.write("test"), 4LL);
	file.close();
	QVERIFY(!writeCollectManifest(directory.path(), document));
}

QTEST_GUILESS_MAIN(CollectManifestTests)
#include "collectmanifesttests.moc"
