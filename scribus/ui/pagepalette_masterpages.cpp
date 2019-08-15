/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pagepalette_masterpages.h"

#include <QApplication>
#include <QCloseEvent>
#include <QCursor>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QScopedPointer>
#include <QSpacerItem>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

#include "appmodes.h"
#include "canvasmode.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "mergedoc.h"
#include "newtemp.h"
#include "pagestructs.h"
#include "prefsmanager.h"
#include "scpage.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "util_os.h"


PagePalette_MasterPages::PagePalette_MasterPages( QWidget* parent, ScribusView *pCurrentView, const QString& masterPageName) : QWidget(parent)
{
	m_doc  = pCurrentView->Doc;
	m_view = pCurrentView;

	setupUi(this);

	importButton->setIcon(IconManager::instance().loadIcon("16/document-open.png"));
	newButton->setIcon(IconManager::instance().loadIcon("16/document-new.png"));
	duplicateButton->setIcon(IconManager::instance().loadIcon("16/edit-copy.png"));
	deleteButton->setIcon(IconManager::instance().loadIcon("16/edit-delete.png"));

	masterPageListBox->setSelectionMode(QAbstractItemView::ExtendedSelection);
	styleChange();
	languageChange();

	if (masterPageName.isEmpty())
		m_currentPage = m_doc->MasterNames.begin().key();
	else
		m_currentPage = masterPageName;
	updateMasterPageList(m_currentPage);
	m_view->showMasterPage(m_doc->MasterNames[m_currentPage]);

	setMinimumSize(sizeHint());

	// signals and slots connections
	connectSignals();
}

void PagePalette_MasterPages::setView(ScribusView* view, const QString& masterPageName)
{
//	ScribusView* oldView = m_view;
//	QString oldPage = m_currentPage;

	disconnectSignals();
	m_view = view;
	m_doc = m_view ? m_view->Doc : nullptr;

	if (!view)
	{
		this->setEnabled(false);
		return;
	}

	if (masterPageName.isEmpty() && m_doc->masterPageMode())
		m_currentPage = m_doc->currentPage()->pageName();
	else if (masterPageName.isEmpty())
		m_currentPage = m_doc->MasterNames.begin().key();
	else
		m_currentPage = masterPageName;

	QString currentPageName;
	if (m_doc->masterPageMode())
		currentPageName = m_doc->currentPage()->pageName();
	if (m_doc->masterPageMode() && m_currentPage != currentPageName)
		m_view->showMasterPage(m_doc->MasterNames[m_currentPage]);

	this->setEnabled(m_doc->masterPageMode());
	connectSignals();
}

void PagePalette_MasterPages::connectSignals()
{
	connect(duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateMasterPage()));
	connect(deleteButton   , SIGNAL(clicked()), this, SLOT(deleteMasterPage()));
	connect(newButton      , SIGNAL(clicked()), this, SLOT(newMasterPage()));
	connect(importButton   , SIGNAL(clicked()), this, SLOT(importPage()));
	connect(masterPageListBox, SIGNAL(itemClicked(QListWidgetItem*)),
			 this, SLOT(selectMasterPage(QListWidgetItem*)));
	connect(masterPageListBox, SIGNAL(itemChanged(QListWidgetItem*)),
			 this, SLOT(renameMasterPage( QListWidgetItem*)));
	connect(finishButton   , SIGNAL(released()), this, SIGNAL(finished()));
}

void PagePalette_MasterPages::disconnectSignals()
{
	disconnect(duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateMasterPage()));
	disconnect(deleteButton   , SIGNAL(clicked()), this, SLOT(deleteMasterPage()));
	disconnect(newButton      , SIGNAL(clicked()), this, SLOT(newMasterPage()));
	disconnect(importButton   , SIGNAL(clicked()), this, SLOT(importPage()));
	disconnect(masterPageListBox, SIGNAL(itemClicked(QListWidgetItem*)),
			   this, SLOT(selectMasterPage(QListWidgetItem*)));
	disconnect(masterPageListBox, SIGNAL(itemChanged(QListWidgetItem*)),
			   this, SLOT(renameMasterPage( QListWidgetItem*)));
	disconnect(finishButton   , SIGNAL(released()), this, SIGNAL(finished()));
}

void PagePalette_MasterPages::languageChange()
{
	setWindowTitle( tr( "Edit Master Pages" ) );
	duplicateButton->setToolTip( tr( "Duplicate the selected master page" ) );
	deleteButton->setToolTip( tr( "Delete the selected master page" ) );
	newButton->setToolTip( tr( "Add a new master page" ) );
	importButton->setToolTip( tr( "Import master pages from another document" ) );
}

void PagePalette_MasterPages::styleChange()
{
	PrefsManager& prefsManager = PrefsManager::instance();
	if (prefsManager.appPrefs.uiPrefs.style=="Macintosh" || (os_is_osx() && prefsManager.appPrefs.uiPrefs.style==""))
		finishButton->setMinimumHeight(32);
	else
		finishButton->setMinimumHeight(0);
}

void PagePalette_MasterPages::deleteMasterPage()
{
	bool forceDelete = false;

	// allow to delete multiple pages in one step
	foreach (QListWidgetItem * delItem, masterPageListBox->selectedItems())
	{
		m_currentPage = delItem->text();

		if ((m_currentPage == CommonStrings::masterPageNormal)
			|| (m_currentPage == CommonStrings::trMasterPageNormal)
			|| (m_currentPage == CommonStrings::trMasterPageNormalLeft)
			|| (m_currentPage == CommonStrings::trMasterPageNormalMiddle)
			|| (m_currentPage == CommonStrings::trMasterPageNormalRight))
			continue;

		QString extraWarn = "";

		if (!forceDelete)
		{
			for (int i=0; i < m_doc->DocPages.count(); ++i )
			{
				if (m_doc->DocPages[i]->masterPageName() == m_currentPage)
					extraWarn = tr("This master page is used at least once in the document.");
			}
			int exit = ScMessageBox::warning(this,
										CommonStrings::trWarning,
										tr("Do you really want to delete master page \"%1\"?").arg(m_currentPage)+"\n"+extraWarn,
										QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::Cancel,
										QMessageBox::NoButton,	// GUI default
										QMessageBox::YesToAll);	// batch default

			if (exit == QMessageBox::YesToAll)
				forceDelete = true;
			if (exit == QMessageBox::No)
				continue;
			if (exit == QMessageBox::Cancel)
				break;
		}

		if (m_doc->appMode == modeEditClip)
			m_view->requestMode(submodeEndNodeEdit);
		m_doc->scMW()->deletePage2(m_doc->MasterNames[m_currentPage]);
		//<<CB TODO Move back into ScribusDoc::deleteMasterPage();
		//This must happen after the pages have been reformed (view/doc)
		m_doc->rebuildMasterNames();
		// Fix up any pages that refer to the deleted master page
		m_doc->replaceMasterPage(m_currentPage);
	}

	// set the 1st MP for the other slots
	QMap<QString,int>::Iterator it = m_doc->MasterNames.begin();
	m_currentPage = it.key();
	updateMasterPageList(m_currentPage);
	selectMasterPage(m_currentPage);
}

void PagePalette_MasterPages::duplicateMasterPage()
{
	int copyC = 1;
	QString potentialMasterPageName(m_currentPage);
	while (m_doc->MasterNames.contains(potentialMasterPageName))
		potentialMasterPageName = tr("Copy #%1 of %2").arg(copyC++).arg(m_currentPage);

	QScopedPointer<NewTm> dia(new NewTm(this, tr("&Name:"), tr("New Master Page"), m_doc, potentialMasterPageName));
	if (!dia->exec())
		return;

	if (m_doc->appMode == modeEditClip)
		m_view->requestMode(submodeEndNodeEdit);
	QString masterPageName = dia->Answer->text();
	bool masterPageNameWrong = m_doc->MasterNames.contains(masterPageName);
	masterPageNameWrong |= (masterPageName == CommonStrings::masterPageNormal);
	masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormal);
	masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalLeft);
	masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalMiddle);
	masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalRight);
	masterPageNameWrong |=  masterPageName.isEmpty();
	while (masterPageNameWrong)
	{
		if (!dia->exec())
			return;
		masterPageName = dia->Answer->text();
		masterPageNameWrong = m_doc->MasterNames.contains(masterPageName);
		masterPageNameWrong |= (masterPageName == CommonStrings::masterPageNormal);
		masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormal);
		masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalLeft);
		masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalMiddle);
		masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalRight);
		masterPageNameWrong |=  masterPageName.isEmpty();
	}
	PrefsManager& prefsManager = PrefsManager::instance();
	int inde = m_doc->MasterNames[m_currentPage];
	int nr = m_doc->Pages->count();
	ScPage* from = m_doc->Pages->at(inde);
	ScPage* destination = m_doc->addMasterPage(nr, masterPageName);
	if (m_doc->pagePositioning() != singlePage)
	{
		int lp = dia->Links->currentIndex();
		if (lp == 0)
			lp = 1;
		else if (lp == static_cast<int>(dia->Links->count()-1))
			lp = 0;
		else
			lp++;
		destination->LeftPg = lp;
	}
	destination->initialMargins.setTop(from->initialMargins.top());
	destination->initialMargins.setBottom(from->initialMargins.bottom());
	if (m_doc->pageSets()[m_doc->pagePositioning()].Columns == 1)
	{
		destination->initialMargins.setLeft(from->initialMargins.left());
		destination->initialMargins.setRight(from->initialMargins.right());
	}
	else
	{
		if (destination->LeftPg != from->LeftPg)
		{
			if (destination->LeftPg > 1)
			{
				destination->initialMargins.setRight(from->initialMargins.left());
				destination->initialMargins.setLeft(from->initialMargins.left());
			}
			else
			{
				destination->initialMargins.setLeft(from->initialMargins.left());
				destination->initialMargins.setRight(from->initialMargins.right());
			}
		}
		else
		{
			destination->initialMargins.setLeft(from->initialMargins.left());
			destination->initialMargins.setRight(from->initialMargins.right());
		}
	}
	//#8321 : incorrect selection of master page on new mp creation/duplication
	//m_doc->setCurrentPage(destination);
	updateMasterPageList(masterPageName);
	selectMasterPage(masterPageName);
	uint oldItems = m_doc->Items->count();
	uint end2 = m_doc->MasterItems.count();
	int GrMax = m_doc->GroupCounter;
	m_doc->m_Selection->clear();
	if (oldItems>0)
	{
		ScLayers::iterator it;
		m_doc->m_Selection->delaySignalsOn();
		for (it = m_doc->Layers.begin(); it != m_doc->Layers.end(); ++it)
		{
			for (uint ite = 0; ite < oldItems; ++ite)
			{
				PageItem *itemToCopy = m_doc->Items->at(ite);
				if (itemToCopy->OwnPage == inde && (it->ID == itemToCopy->m_layerID))
					m_doc->m_Selection->addItem(itemToCopy, true);
			}
			if (m_doc->m_Selection->count() != 0)
			{
				ScriXmlDoc ss;
				QString buffer = ss.writeElem(m_doc, m_doc->m_Selection);
				ss.readElemToLayer(buffer, prefsManager.appPrefs.fontPrefs.AvailFonts, m_doc, destination->xOffset(), destination->yOffset(), false, true, prefsManager.appPrefs.fontPrefs.GFontSub, it->ID);
				m_doc->m_Selection->clear();
			}
		}
		m_doc->m_Selection->clear();
		m_doc->m_Selection->delaySignalsOff();
	}
	uint end3 = m_doc->MasterItems.count();
	for (uint a = end2; a < end3; ++a)
	{
		PageItem *newItem = m_doc->MasterItems.at(a);
		int masterPageIndex = m_doc->MasterNames[masterPageName];
		newItem->setMasterPage(masterPageIndex, masterPageName);
	}
	from->guides.copy(&destination->guides);
	m_doc->GroupCounter = GrMax + 1;
	m_view->Deselect(true);
	m_doc->setLoading(false);
	m_view->reformPages();
	m_view->DrawNew();
}

void PagePalette_MasterPages::newMasterPage()
{
	QString masterPageName;
	int nr = m_doc->Pages->count();

	QScopedPointer<NewTm> dia(new NewTm(this, tr("Name:"), tr("New MasterPage"), m_doc, tr("New Master Page %1").arg(nr)));
	if (!dia->exec())
		return;

	if (m_doc->appMode == modeEditClip)
		m_view->requestMode(submodeEndNodeEdit);
	masterPageName = dia->Answer->text();
	bool masterPageNameWrong = m_doc->MasterNames.contains(masterPageName);
	masterPageNameWrong |= (masterPageName == CommonStrings::masterPageNormal);
	masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormal);
	masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalLeft);
	masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalMiddle);
	masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalRight);
	masterPageNameWrong |=  masterPageName.isEmpty();
	while (masterPageNameWrong)
	{
		if (!dia->exec())
			return;
		masterPageName = dia->Answer->text();
		masterPageNameWrong = m_doc->MasterNames.contains(masterPageName);
		masterPageNameWrong |= (masterPageName == CommonStrings::masterPageNormal);
		masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormal);
		masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalLeft);
		masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalMiddle);
		masterPageNameWrong |= (masterPageName == CommonStrings::trMasterPageNormalRight);
		masterPageNameWrong |=  masterPageName.isEmpty();
	}
	m_doc->setCurrentPage(m_doc->addMasterPage(nr, masterPageName));
	if (m_doc->pagePositioning() != singlePage)
	{
		int lp = dia->Links->currentIndex();
		if (lp == 0)
			lp = 1;
		else if (lp == static_cast<int>(dia->Links->count()-1))
			lp = 0;
		else
			lp++;
		m_doc->Pages->at(nr)->LeftPg = lp;
	}
	updateMasterPageList(masterPageName);
	//#8321 : incorrect selection of master page on new mp creation/duplictation
	//m_view->showMasterPage(m_doc->MasterNames[masterPageName]);
	selectMasterPage(masterPageName);
	m_view->reformPages();
}

void PagePalette_MasterPages::importPage()
{
	QScopedPointer<MergeDoc> dia(new MergeDoc(this, true));
	if (!dia->exec())
		return;

	if (m_doc->appMode == modeEditClip)
		m_view->requestMode(submodeEndNodeEdit);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	int nr = m_doc->Pages->count();

	QString masterPageName(dia->getMasterPageNameText());
	QString masterPageName2(masterPageName);
	int copyC = 1;
	while (m_doc->MasterNames.contains(masterPageName2))
	{
		masterPageName2 = tr("Copy #%1 of ").arg(copyC)+masterPageName;
		copyC++;
	}
	m_doc->setCurrentPage(m_doc->addMasterPage(nr, masterPageName2));
	qApp->processEvents();
	//CB TODO: If we are loading to a new name, we rely on doc->onpage in 
	//FileLoader::PasteItem as this call doesn't pass in the new destination page
	m_doc->scMW()->loadPage(dia->getFromDoc(), dia->getMasterPageNameItem(), true, masterPageName2);
	qApp->processEvents();

	updateMasterPageList(masterPageName2);
	m_view->showMasterPage(m_doc->MasterNames[masterPageName2]);
	qApp->restoreOverrideCursor();
}

void PagePalette_MasterPages::selectMasterPage(QListWidgetItem *item)
{
	m_currentPage = item->data(Qt::UserRole).toString();

	bool deleteEnabled = true;
	deleteEnabled &= (m_doc->MasterNames.count() > 1);
	deleteEnabled &= (m_currentPage != CommonStrings::masterPageNormal);
	deleteEnabled &= (m_currentPage != CommonStrings::trMasterPageNormal);
	deleteButton->setEnabled(deleteEnabled);

	if (m_doc->appMode == modeEditClip)
		m_view->requestMode(submodeEndNodeEdit);
	else if (m_doc->appMode == modeEdit || m_doc->appMode == modeEditGradientVectors)
		m_view->requestMode(modeNormal);
	m_view->showMasterPage(m_doc->MasterNames[m_currentPage]);

	this->setEnabled(true);
}

void PagePalette_MasterPages::selectMasterPage(const QString& name)
{
	if (!m_doc || !m_view)
		return;

	m_currentPage.clear();
	for (int i = 0; i < masterPageListBox->count(); ++i)
	{
		QListWidgetItem* item = masterPageListBox->item(i);
		QString realPageName  = item->data(Qt::UserRole).toString();
		if (item->text() == name)
		{
			m_currentPage = realPageName;
			break;
		}
		if (realPageName == name)
		{
			m_currentPage = name;
			break;
		}
	}
	
	if (m_currentPage.isEmpty())
		m_currentPage = m_doc->MasterPages.at(0)->pageName();

	bool deleteEnabled = true;
	deleteEnabled &= (m_doc->MasterNames.count() > 1);
	deleteEnabled &= (m_currentPage != CommonStrings::masterPageNormal);
	deleteEnabled &= (m_currentPage != CommonStrings::trMasterPageNormal);
	deleteButton->setEnabled(deleteEnabled);

	if (m_doc->appMode == modeEditClip)
		m_view->requestMode(submodeEndNodeEdit);
	m_view->showMasterPage(m_doc->MasterNames[m_currentPage]);

	this->setEnabled(true);
}

void PagePalette_MasterPages::updateMasterPageList()
{
	QString masterPageName = (m_doc->MasterNames.contains(m_currentPage)) ? m_currentPage : CommonStrings::masterPageNormal;
	updateMasterPageList(masterPageName);
}

void PagePalette_MasterPages::updateMasterPageList(QString masterPageName)
{
	if (!m_doc || !m_view)
		return;

	masterPageListBox->clear();
	for (QMap<QString,int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
	{
		QString mpName = it.key();
//		qDebug() << mpName;
		if (it.key() == CommonStrings::masterPageNormal)
			mpName = CommonStrings::trMasterPageNormal;
		QListWidgetItem* mpItem = new QListWidgetItem(mpName);
		if ((mpName != CommonStrings::masterPageNormal)  && (mpName != CommonStrings::trMasterPageNormal) &&
			(mpName != CommonStrings::masterPageNormalLeft)  && (mpName != CommonStrings::trMasterPageNormalLeft) &&
			(mpName != CommonStrings::masterPageNormalMiddle) && (mpName != CommonStrings::trMasterPageNormalMiddle) &&
			(mpName != CommonStrings::masterPageNormalRight)  && (mpName != CommonStrings::trMasterPageNormalRight))
		{
			mpItem->setFlags(mpItem->flags() |= Qt::ItemIsEditable);
		}
		else
		{
			mpItem->setFlags(mpItem->flags() &= ~Qt::ItemIsEditable);
		}
		mpItem->setData(Qt::UserRole, it.key());
		masterPageListBox->addItem(mpItem);
	}
	deleteButton->setEnabled(m_doc->MasterNames.count() != 1);
	if (masterPageName == CommonStrings::masterPageNormal)
	{
		masterPageName = CommonStrings::trMasterPageNormal;
		deleteButton->setEnabled(false);
	}
	QList<QListWidgetItem *> itL = masterPageListBox->findItems(masterPageName, Qt::MatchExactly);
	if (itL.count() != 0)
		itL.at(0)->setSelected(true);
}

void PagePalette_MasterPages::renameMasterPage(QListWidgetItem * item)
{
	bool success = true;
	QString oldName(item->data(Qt::UserRole).toString());
	QString newName(item->text());
	if (oldName == newName)
		return;

	bool sigBlocked = masterPageListBox->blockSignals(true);
	if ((newName == CommonStrings::masterPageNormal) || (newName == CommonStrings::trMasterPageNormal) ||
		(newName == CommonStrings::masterPageNormalLeft)   || (newName == CommonStrings::trMasterPageNormalLeft) ||
		(newName == CommonStrings::masterPageNormalMiddle) || (newName == CommonStrings::trMasterPageNormalMiddle) ||
		(newName == CommonStrings::masterPageNormalRight)  || (newName == CommonStrings::trMasterPageNormalRight))
	{
		ScMessageBox::information( this, tr("Unable to Rename Master Page"), tr("The Normal page is not allowed to be renamed."), QMessageBox::Ok );
		success = false;
	}
	if (newName.isEmpty())
		success = false;

	if (success)
		success = m_doc->renameMasterPage(oldName, newName);
	if (success)
	{
		item->setData(Qt::UserRole, newName);
		masterPageListBox->blockSignals(sigBlocked);
		updateMasterPageList(newName);
		return;
	}

	QString masterPageName = oldName;
	if (masterPageName == CommonStrings::masterPageNormal)
		masterPageName = CommonStrings::trMasterPageNormal;
	if (masterPageName == CommonStrings::masterPageNormalLeft)
		masterPageName = CommonStrings::trMasterPageNormalLeft;
	if (masterPageName == CommonStrings::masterPageNormalMiddle)
		masterPageName = CommonStrings::trMasterPageNormalMiddle;
	if (masterPageName == CommonStrings::masterPageNormalRight)
		masterPageName = CommonStrings::trMasterPageNormalRight;
	item->setText(masterPageName);
	masterPageListBox->blockSignals(sigBlocked);
}


void PagePalette_MasterPages::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	if (e->type() == QEvent::StyleChange)
	{
		styleChange();
		return;
	}

	QWidget::changeEvent(e);
}
