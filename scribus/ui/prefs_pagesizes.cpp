/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QListWidget>
#include <QStringList>

#include "iconmanager.h"
#include "pagesize.h"
#include "prefsstructs.h"
#include "scribusdoc.h"
#include "ui/prefs_pagesizes.h"


Prefs_PageSizes::Prefs_PageSizes(QWidget* parent, ScribusDoc* /*doc*/)
	: Prefs_Pane(parent)
{
	setupUi(this);

	m_caption = tr("Page Sizes");
	m_icon = "page-sizes";

	treeAvailableSizes->setColumnCount(2);
	treeActiveSizes->setColumnCount(2);

	languageChange();

	toActiveButton->setIcon(IconManager::instance().loadIcon("22/go-next.png"));
	fromActiveButton->setIcon(IconManager::instance().loadIcon("22/go-previous.png"));
	connect(toActiveButton, SIGNAL(clicked()), this, SLOT(moveToActive()));
	connect(fromActiveButton, SIGNAL(clicked()), this, SLOT(moveFromActive()));
}

Prefs_PageSizes::~Prefs_PageSizes() = default;

void Prefs_PageSizes::languageChange()
{
	QString textSize = tr("Page Format");
	QString textDimension = tr("Dimension");

	if(QTreeWidgetItem* header = treeAvailableSizes->headerItem()) {
		header->setText(0, textSize);

		if (header->columnCount() > 1)
			header->setText(1, textDimension);
	}
	else
		treeAvailableSizes->setHeaderLabel( textSize );


	if(QTreeWidgetItem* header = treeActiveSizes->headerItem()) {
		header->setText(0, textSize);
		if (header->columnCount() > 1)
			header->setText(1, textDimension);
	}
	else
		treeActiveSizes->setHeaderLabel( textSize );

}

void Prefs_PageSizes::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	PageSize ps(prefsData->docSetupPrefs.pageSize);

	treeAvailableSizes->clear();
	treeActiveSizes->clear();

	auto cats = ps.categories();

	// Available Page Sizes
	for (auto it = cats.begin(); it != cats.end(); ++it)
	{
		QTreeWidgetItem* tlItem = new QTreeWidgetItem();
		tlItem->setText(0, it.value());
		tlItem->setData(0, Qt::UserRole, it.key());

		treeAvailableSizes->addTopLevelItem(tlItem);

		PageSizeInfoMap sizes = ps.sizesByCategory(it.key());

		for (auto s = sizes.begin(); s != sizes.end(); ++s)
		{
			if (!ps.activePageSizes().contains(s.key()))
			{
				QTreeWidgetItem* sItem = new QTreeWidgetItem();
				sItem->setText(0, s.value().trSizeName);
				sItem->setText(1, s.value().sizeLabel);
				sItem->setData(0, Qt::UserRole, s.key());

				tlItem->addChild(sItem);
			}
		}
	}

	// Active page Sizes
	for (auto it = cats.begin(); it != cats.end(); ++it)
	{
		PageSizeInfo::Category cat = it.key();

		QTreeWidgetItem* tlItem = new QTreeWidgetItem();
		tlItem->setText(0, it.value());
		tlItem->setData(0, Qt::UserRole, cat);

		treeActiveSizes->addTopLevelItem(tlItem);

		PageSizeInfoMap sizes = ps.activePageSizes();

		for (auto s = sizes.begin(); s != sizes.end(); ++s)
		{
			if (s.value().category == cat)
			{
				QTreeWidgetItem* sItem = new QTreeWidgetItem();
				sItem->setText(0, s.value().trSizeName);
				sItem->setText(1, s.value().sizeLabel);
				sItem->setData(0, Qt::UserRole, s.key());

				tlItem->addChild(sItem);
			}

		}
	}

}

void Prefs_PageSizes::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	QStringList newActivePageSizes;

	for (int i = 0; i < treeActiveSizes->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem* item = treeActiveSizes->takeTopLevelItem(i);

		for (int j = 0; j < item->childCount(); ++j)
			newActivePageSizes << item->child(j)->data(0, Qt::UserRole).toString();
	}

	prefsData->activePageSizes = newActivePageSizes;

}

void Prefs_PageSizes::moveToActive()
{
	moveItems(treeAvailableSizes, treeActiveSizes);
}

void Prefs_PageSizes::moveFromActive()
{
	int childrenCount = 0;

	for (int i = 0; i < treeActiveSizes->topLevelItemCount(); ++i)
		childrenCount += treeActiveSizes->topLevelItem(i)->childCount();

	if (childrenCount > 1)
		moveItems(treeActiveSizes, treeAvailableSizes);
}

void Prefs_PageSizes::moveItems(QTreeWidget *fromList, QTreeWidget *toList)
{
	QList<QTreeWidgetItem *> si(fromList->selectedItems());

	if (si.count() != 1)
		return;

	for (int i = 0; i < si.count(); ++i)
	{
		QTreeWidgetItem* currentItem = si.at(i);
		QTreeWidgetItem* currentParent = currentItem->parent();

		if (currentParent)
		{
			int index = fromList->indexOfTopLevelItem(currentParent);

			QTreeWidgetItem* exItem = currentParent->takeChild(currentParent->indexOfChild(currentItem));
			if (exItem)
			{
				QTreeWidgetItem* newParentItem = toList->topLevelItem(index);
				newParentItem->addChild(exItem);
				newParentItem->sortChildren(0, Qt::AscendingOrder);
			}
		}
		else
		{
			int index = fromList->indexOfTopLevelItem(currentItem);

			QList<QTreeWidgetItem*> exItems = currentItem->takeChildren();
			if (exItems.count() > 0)
			{
				QTreeWidgetItem* newParentItem = toList->topLevelItem(index);
				newParentItem->addChildren(exItems);
				newParentItem->sortChildren(0, Qt::AscendingOrder);
			}
		}
	}
}

