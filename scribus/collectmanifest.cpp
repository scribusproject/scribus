/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "collectmanifest.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSaveFile>

namespace {
bool saveBytes(const QString& path, const QByteArray& bytes)
{
	QSaveFile file(path);
	return file.open(QIODevice::WriteOnly) && file.write(bytes) == bytes.size() && file.commit();
}
}

bool writeCollectManifest(const QString& directoryPath, const QString& documentPath, QString* error)
{
	const QDir directory(directoryPath);
	const QFileInfo document(documentPath);
	if (!directory.exists() || !document.isFile()
		|| QDir::cleanPath(document.absolutePath()) != QDir::cleanPath(directory.absolutePath()))
	{
		if (error)
			*error = QObject::tr("The collected document is not inside the output directory.");
		return false;
	}
	QJsonArray files;
	QDirIterator iterator(directory.absolutePath(), QDir::Files | QDir::NoSymLinks,
		QDirIterator::Subdirectories);
	while (iterator.hasNext())
	{
		const QString path = iterator.next();
		const QString relative = directory.relativeFilePath(path).replace(QLatin1Char('\\'), QLatin1Char('/'));
		if (relative == QLatin1String("collect-manifest.json") || relative == QLatin1String("PRINT-INSTRUCTIONS.txt"))
			continue;
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly))
		{
			if (error)
				*error = QObject::tr("Could not read collected file: %1").arg(relative);
			return false;
		}
		QCryptographicHash hasher(QCryptographicHash::Sha256);
		if (!hasher.addData(&file))
		{
			if (error)
				*error = QObject::tr("Could not hash collected file: %1").arg(relative);
			return false;
		}
		const QByteArray digest = hasher.result().toHex();
		const QFileInfo info(path);
		QString kind = QStringLiteral("other");
		if (relative == directory.relativeFilePath(document.absoluteFilePath()))
			kind = QStringLiteral("document");
		else if (relative.startsWith(QLatin1String("images/")))
			kind = QStringLiteral("image");
		else if (relative.startsWith(QLatin1String("fonts/")))
			kind = QStringLiteral("font");
		else if (relative.startsWith(QLatin1String("profiles/")))
			kind = QStringLiteral("color-profile");
		QJsonObject entry;
		entry.insert(QStringLiteral("path"), relative);
		entry.insert(QStringLiteral("kind"), kind);
		entry.insert(QStringLiteral("bytes"), static_cast<qint64>(info.size()));
		entry.insert(QStringLiteral("sha256"), QString::fromLatin1(digest));
		if (kind == QLatin1String("font") || kind == QLatin1String("image")
			|| kind == QLatin1String("color-profile"))
			entry.insert(QStringLiteral("licenseReview"), QStringLiteral("required"));
		files.append(entry);
	}
	QJsonObject manifest;
	manifest.insert(QStringLiteral("formatVersion"), 1);
	manifest.insert(QStringLiteral("document"), directory.relativeFilePath(document.absoluteFilePath()));
	manifest.insert(QStringLiteral("files"), files);
	const QByteArray instructions = QObject::tr(
		"COLLECT FOR OUTPUT\n\n"
		"Open the collected Scribus document in this folder and check all links, fonts, "
		"color profiles, page size, bleeds, and preflight warnings before printing.\n"
		"The collect-manifest.json file lists file sizes and SHA-256 checksums.\n"
		"Review the redistribution licenses of every collected font, image, and ICC profile. "
		"Inclusion in this folder does not imply permission to redistribute.\n").toUtf8();
	if (!saveBytes(directory.filePath(QStringLiteral("PRINT-INSTRUCTIONS.txt")), instructions)
		|| !saveBytes(directory.filePath(QStringLiteral("collect-manifest.json")),
			QJsonDocument(manifest).toJson(QJsonDocument::Indented)))
	{
		if (error)
			*error = QObject::tr("Could not write the Collect for Output manifest or instructions.");
		return false;
	}
	return true;
}
