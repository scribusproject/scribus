/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "imagelinkreplacement.h"

#include <QDir>
#include <QFileInfo>
#include <QList>
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

void appendImageFrames(const QList<PageItem*>& roots, QList<PageItem*>* frames,
	QSet<PageItem*>* seen, QSet<PageItem*>* masters = nullptr)
{
	for (PageItem* root : roots)
	{
		const QList<PageItem*> items = root->isGroup() ? root->getAllChildren() : QList<PageItem*> { root };
		for (PageItem* item : items)
		{
			if (!seen->contains(item) && item->isImageFrame() && !item->isLatexFrame()
				&& !item->isImageInline() && !item->Pfile.isEmpty())
			{
				seen->insert(item);
				frames->append(item);
				if (masters)
					masters->insert(item);
			}
		}
	}
}

QString absoluteCleanPath(const QString& path)
{
	return QDir::cleanPath(QFileInfo(path).absoluteFilePath());
}

}

ImageLinkReplacementResult replaceImageLinks(ScribusDoc* doc, const QString& sourcePath,
	const QString& replacementPath, bool dryRun)
{
	return replaceImageLinks(doc, sourcePath, replacementPath,
		ImageLinkReplacementScope::EntireDocument, dryRun);
}

ImageLinkReplacementResult replaceImageLinks(ScribusDoc* doc, const QString& sourcePath,
	const QString& replacementPath, ImageLinkReplacementScope scope, bool dryRun)
{
	ImageLinkReplacementResult result;
	if (!doc || sourcePath.isEmpty())
		return result;

	QList<PageItem*> frames;
	QSet<PageItem*> seen;
	QSet<PageItem*> masters;
	appendImageFrames(doc->MasterItems, &frames, &seen, &masters);
	appendImageFrames(doc->DocItems, &frames, &seen);
	const QString source = absoluteCleanPath(sourcePath);
	QList<PageItem*> matches;
	for (PageItem* item : frames)
	{
		const bool master = masters.contains(item);
		const bool inScope = scope == ImageLinkReplacementScope::EntireDocument
			|| (scope == ImageLinkReplacementScope::MasterPages && master)
			|| (scope == ImageLinkReplacementScope::CurrentPage && !master
				&& item->OwnPage == doc->currentPageNumber());
		if (inScope && absoluteCleanPath(item->Pfile) == source)
			matches.append(item);
	}
	result.matched = matches.size();
	for (PageItem* item : matches)
		result.entries.append({item->itemName(), masters.contains(item)
			? (item->OnMasterPage.isEmpty() ? QStringLiteral("Master") : item->OnMasterPage)
			: QString::number(item->OwnPage + 1),
			QStringLiteral("matched")});
	if (dryRun || matches.isEmpty() || replacementPath.isEmpty())
		return result;

	const QString replacement = absoluteCleanPath(replacementPath);
	if (replacement == source)
		return result;

	UndoTransaction transaction;
	if (UndoManager::undoEnabled())
		transaction = UndoManager::instance()->beginTransaction(Um::SelectionGroup, Um::IGroup,
			QObject::tr("Replace image links"), QString(), Um::IGetImage);
	const bool originalMasterMode = doc->masterPageMode();
	for (int index = 0; index < matches.size(); ++index)
	{
		PageItem* item = matches.at(index);
		const bool itemMasterMode = masters.contains(item);
		if (doc->masterPageMode() != itemMasterMode)
			doc->setMasterPageMode(itemMasterMode);
		if (item->relinkImage(replacement, false))
		{
			++result.replaced;
			result.entries[index].status = QStringLiteral("replaced");
		}
		else
		{
			++result.failed;
			result.entries[index].status = QStringLiteral("failed");
		}
	}
	if (doc->masterPageMode() != originalMasterMode)
		doc->setMasterPageMode(originalMasterMode);
	if (transaction)
	{
		if (result.replaced > 0)
			transaction.commit();
		else
			transaction.cancel();
	}
	return result;
}

bool writeImageLinkReplacementReport(const QString& path, const QString& sourcePath,
	const QString& replacementPath, const ImageLinkReplacementResult& result)
{
	QJsonObject root;
	root.insert(QStringLiteral("formatVersion"), 1);
	root.insert(QStringLiteral("source"), sourcePath);
	root.insert(QStringLiteral("replacement"), replacementPath);
	root.insert(QStringLiteral("matched"), result.matched);
	root.insert(QStringLiteral("replaced"), result.replaced);
	root.insert(QStringLiteral("failed"), result.failed);
	QJsonArray entries;
	for (const ImageLinkReplacementEntry& entry : result.entries)
	{
		QJsonObject record;
		record.insert(QStringLiteral("frame"), entry.frame);
		record.insert(QStringLiteral("page"), entry.page);
		record.insert(QStringLiteral("status"), entry.status);
		entries.append(record);
	}
	root.insert(QStringLiteral("frames"), entries);
	QSaveFile file(path);
	const QByteArray bytes = QJsonDocument(root).toJson(QJsonDocument::Indented);
	return file.open(QIODevice::WriteOnly) && file.write(bytes) == bytes.size() && file.commit();
}
