/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "imagecmykbatch.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QSet>

#include "pageitem.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "undotransaction.h"

namespace {

void appendFrames(const QList<PageItem*>& roots, QList<PageItem*>* frames,
	QSet<PageItem*>* seen, QSet<PageItem*>* masters = nullptr)
{
	for (PageItem* root : roots)
	{
		const QList<PageItem*> items = root->isGroup() ? root->getAllChildren() : QList<PageItem*> { root };
		for (PageItem* item : items)
		{
			if (!seen->contains(item) && item->isImageFrame() && !item->isLatexFrame()
				&& !item->Pfile.isEmpty())
			{
				seen->insert(item);
				frames->append(item);
				if (masters)
					masters->insert(item);
			}
		}
	}
}

QString safeBaseName(const QString& name)
{
	QString base = name;
	for (QChar& character : base)
		if (!character.isLetterOrNumber() && character != QLatin1Char('-') && character != QLatin1Char('_'))
			character = QLatin1Char('-');
	base = base.left(80).trimmed();
	return base.isEmpty() ? QStringLiteral("image") : base;
}

QString unusedPath(const QDir& directory, const QString& base, const QString& suffix,
	QSet<QString>* reserved)
{
	for (int number = 0; ; ++number)
	{
		const QString fileName = number == 0 ? base + suffix
			: base + QStringLiteral("-%1").arg(number) + suffix;
		const QString path = directory.filePath(fileName);
		if (!reserved->contains(path) && !QFileInfo::exists(path) && !QFileInfo(path).isSymLink())
		{
			reserved->insert(path);
			return path;
		}
	}
}

bool writeReport(const QDir& directory, const ImageCMYKBatchOptions& options,
	ImageCMYKBatchResult* result)
{
	QSet<QString> reserved;
	const QString path = unusedPath(directory, QStringLiteral("scribus-cmyk-report"),
		QStringLiteral(".json"), &reserved);
	QJsonObject root;
	root.insert(QStringLiteral("formatVersion"), 1);
	root.insert(QStringLiteral("sourceProfile"), options.color.sourceProfileName);
	root.insert(QStringLiteral("destinationProfile"), options.color.destinationProfileName);
	root.insert(QStringLiteral("renderingIntent"), options.color.renderingIntent
		? static_cast<int>(*options.color.renderingIntent) : -1);
	root.insert(QStringLiteral("blackPointCompensation"), options.color.blackPointCompensation
		? QJsonValue(*options.color.blackPointCompensation) : QJsonValue());
	root.insert(QStringLiteral("originalsPreserved"), true);
	root.insert(QStringLiteral("exported"), result->exported);
	root.insert(QStringLiteral("relinked"), result->relinked);
	root.insert(QStringLiteral("failed"), result->failed);
	QJsonArray entries;
	for (const ImageCMYKBatchEntry& entry : result->entries)
	{
		QJsonObject record;
		record.insert(QStringLiteral("frame"), entry.frame);
		record.insert(QStringLiteral("source"), entry.source);
		record.insert(QStringLiteral("destination"), entry.destination);
		record.insert(QStringLiteral("backup"), entry.backup);
		record.insert(QStringLiteral("status"), entry.status);
		record.insert(QStringLiteral("error"), entry.error);
		entries.append(record);
	}
	root.insert(QStringLiteral("images"), entries);
	QSaveFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(QJsonDocument(root).toJson()) < 0 || !file.commit())
		return false;
	result->reportPath = path;
	return true;
}

}

ImageCMYKBatchResult runImageCMYKBatch(ScribusDoc* doc, const QString& outputDirectory,
	const ImageCMYKBatchOptions& options)
{
	ImageCMYKBatchResult result;
	if (!doc || outputDirectory.isEmpty())
	{
		result.error = QObject::tr("A document and output directory are required.");
		return result;
	}
	const QDir directory(outputDirectory);
	if (!directory.exists() || !QFileInfo(outputDirectory).isWritable())
	{
		result.error = QObject::tr("The CMYK output directory does not exist or is not writable.");
		return result;
	}
	QList<PageItem*> frames;
	QSet<PageItem*> seen;
	QSet<PageItem*> masters;
	appendFrames(doc->MasterItems, &frames, &seen, &masters);
	appendFrames(doc->DocItems, &frames, &seen);
	appendFrames(doc->FrameItems.values(), &frames, &seen);
	QSet<QString> reserved;
	QHash<QString, QString> backupBySource;
	UndoTransaction transaction;
	if (options.relink && !options.dryRun && UndoManager::undoEnabled())
		transaction = UndoManager::instance()->beginTransaction(Um::SelectionGroup, Um::IGroup,
			QObject::tr("Relink CMYK image batch"), QString(), Um::IGetImage);
	const bool originalMasterMode = doc->masterPageMode();
	for (PageItem* item : frames)
	{
		ImageCMYKBatchEntry entry;
		entry.frame = item->itemName();
		entry.source = item->Pfile;
		if ((options.relink && item->isImageInline())
			|| !canExportImageAsCMYKCopy(item, options.color, &entry.error))
		{
			entry.status = QStringLiteral("skipped");
			if (entry.error.isEmpty())
				entry.error = QObject::tr("Embedded image frames cannot be relinked.");
			result.entries.append(entry);
			continue;
		}
		entry.destination = unusedPath(directory, safeBaseName(item->itemName()) +
			QStringLiteral("-CMYK"), QStringLiteral(".tif"), &reserved);
		entry.status = options.dryRun ? QStringLiteral("ready") : QStringLiteral("pending");
		++result.ready;
		if (options.dryRun)
		{
			result.entries.append(entry);
			continue;
		}
		if (options.copyOriginals)
		{
			const QString source = QFileInfo(item->Pfile).absoluteFilePath();
			entry.backup = backupBySource.value(source);
			if (entry.backup.isEmpty())
			{
				QDir backupDir(directory.filePath(QStringLiteral("originals")));
				if (!backupDir.exists() && !directory.mkpath(QStringLiteral("originals")))
					entry.error = QObject::tr("Could not create the originals backup folder.");
				else
				{
					entry.backup = unusedPath(backupDir,
						safeBaseName(QFileInfo(source).completeBaseName()),
						QStringLiteral(".") + QFileInfo(source).suffix(), &reserved);
					if (!QFile::copy(source, entry.backup))
						entry.error = QObject::tr("Could not back up the original image.");
					else
						backupBySource.insert(source, entry.backup);
				}
			}
			if (!entry.error.isEmpty())
			{
				entry.status = QStringLiteral("failed");
				++result.failed;
				result.entries.append(entry);
				continue;
			}
		}
		QString conversionError;
		if (!exportImageAsCMYKCopy(item, entry.destination, options.color, &conversionError))
		{
			entry.status = QStringLiteral("failed");
			entry.error = conversionError;
			++result.failed;
		}
		else
		{
			entry.status = QStringLiteral("exported");
			++result.exported;
			if (options.relink)
			{
				const bool itemMasterMode = masters.contains(item);
				if (doc->masterPageMode() != itemMasterMode)
					doc->setMasterPageMode(itemMasterMode);
				if (item->relinkImage(entry.destination, false, true))
				{
					entry.status = QStringLiteral("relinked");
					++result.relinked;
				}
				else
				{
					entry.error = QObject::tr("TIFF exported, but relinking failed; the original frame link was kept.");
					++result.failed;
				}
			}
		}
		result.entries.append(entry);
	}
	if (doc->masterPageMode() != originalMasterMode)
		doc->setMasterPageMode(originalMasterMode);
	if (transaction)
	{
		if (result.relinked > 0)
			transaction.commit();
		else
			transaction.cancel();
	}
	if (!options.dryRun && !writeReport(directory, options, &result))
		result.error = QObject::tr("The images were processed, but the batch report could not be saved.");
	return result;
}
