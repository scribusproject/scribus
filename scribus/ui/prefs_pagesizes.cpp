/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QListWidget>
#include <QStringList>
#include "ui/prefs_pagesizes.h"
#include "pagesize.h"
#include "prefsmanager.h"
#include "prefsstructs.h"
#include "scribusdoc.h"
#include "util_icon.h"

Prefs_PageSizes::Prefs_PageSizes(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
	toActiveButton->setIcon(QIcon(loadIcon("22/go-next.png")));
	fromActiveButton->setIcon(QIcon(loadIcon("22/go-previous.png")));
	connect(toActiveButton, SIGNAL(clicked()), this, SLOT(moveToActive()));
	connect(fromActiveButton, SIGNAL(clicked()), this, SLOT(moveFromActive()));
}

Prefs_PageSizes::~Prefs_PageSizes()
{
}

void Prefs_PageSizes::languageChange()
{
}

void Prefs_PageSizes::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	PageSize ps(prefsData->docSetupPrefs.pageSize);
	QStringList sl(ps.sizeTRList());
	activeSizesListWidget->addItems(ps.activeSizeTRList());
	for (int i = 0; i < sl.count(); ++i)
	{
		if (activeSizesListWidget->findItems(sl.at(i), Qt::MatchExactly).isEmpty())
			availableSizesListWidget->addItem(sl.at(i));
	}
}

void Prefs_PageSizes::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	QStringList newActivePageSizes;
	for (int i = 0; i < activeSizesListWidget->count(); ++i)
		newActivePageSizes << activeSizesListWidget->item(i)->text();
	PageSize ps(prefsData->docSetupPrefs.pageSize);
	prefsData->activePageSizes=ps.untransPageSizeList(newActivePageSizes);
}

void Prefs_PageSizes::moveToActive()
{
	QList<QListWidgetItem *> si(availableSizesListWidget->selectedItems());
	for (int i = 0; i < si.count(); ++i)
	{
		QListWidgetItem *it=availableSizesListWidget->takeItem(availableSizesListWidget->row(si.at(i)));
		activeSizesListWidget->addItem(it);
	}
}

void Prefs_PageSizes::moveFromActive()
{
	QList<QListWidgetItem *> si(activeSizesListWidget->selectedItems());
	for (int i = 0; i < si.count(); ++i)
	{
		if (activeSizesListWidget->count()>1)
		{
			QListWidgetItem *it=activeSizesListWidget->takeItem(activeSizesListWidget->row(si.at(i)));
			availableSizesListWidget->addItem(it);
		}
	}
}

