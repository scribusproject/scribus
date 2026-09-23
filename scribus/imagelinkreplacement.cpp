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
#include <QSet>

#include "pageitem.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "undotransaction.h"

namespace {

void appendImageFrames(const QList<PageItem*>& roots, QList<PageItem*>* frames, QSet<PageItem*>* seen)
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
	ImageLinkReplacementResult result;
	if (!doc || sourcePath.isEmpty())
		return result;

	QList<PageItem*> frames;
	QSet<PageItem*> seen;
	appendImageFrames(doc->MasterItems, &frames, &seen);
	appendImageFrames(doc->DocItems, &frames, &seen);
	const QString source = absoluteCleanPath(sourcePath);
	QList<PageItem*> matches;
	for (PageItem* item : frames)
	{
		if (absoluteCleanPath(item->Pfile) == source)
			matches.append(item);
	}
	result.matched = matches.size();
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
	for (PageItem* item : matches)
	{
		const bool itemMasterMode = !item->OnMasterPage.isEmpty();
		if (doc->masterPageMode() != itemMasterMode)
			doc->setMasterPageMode(itemMasterMode);
		if (item->relinkImage(replacement, false))
			++result.replaced;
		else
			++result.failed;
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
