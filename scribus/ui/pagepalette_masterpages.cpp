/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pagepalette_masterpages.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QToolButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolTip>
#include <QCursor>
#include <QCloseEvent>
#include <QMessageBox>
#include <QInputDialog>

#include "canvasmode.h"
#include "commonstrings.h"
#include "mergedoc.h"
#include "newtemp.h"
#include "pagestructs.h"
#include "prefsmanager.h"
#include "scpage.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribusXml.h"
#include "selection.h"
#include "util_icon.h"

PagePalette_MasterPages::PagePalette_MasterPages( QWidget* parent, ScribusView *pCurrentView, QString masterPageName) : QWidget(parent)
{
	currentDoc  = pCurrentView->Doc;
	currentView = pCurrentView;

	setupUi(this);

	importButton->setIcon(QIcon(loadIcon("16/document-open.png")));
	newButton->setIcon(QIcon(loadIcon("16/document-new.png")));
	duplicateButton->setIcon(QIcon(loadIcon("16/edit-copy.png")));
	deleteButton->setIcon(QIcon(loadIcon("16/edit-delete.png")));

	masterPageListBox->setSelectionMode(QAbstractItemView::ExtendedSelection);

	languageChange();

	if (masterPageName.isEmpty())
		currentPage = currentDoc->MasterNames.begin().key();
	else
		currentPage = masterPageName;
	updateMasterPageList(currentPage);
	currentView->showMasterPage(currentDoc->MasterNames[currentPage]);

	setMinimumSize(sizeHint());

	// signals and slots connections
	connectSignals();
}

void PagePalette_MasterPages::setView(ScribusView* view, QString masterPageName)
{
	ScribusView* oldView = currentView;
	QString oldPage = currentPage;

	disconnectSignals();
	currentView = view;

	if (!view)
	{
		this->setEnabled(false);
		return;
	}

	currentDoc = currentView->Doc;

	if (masterPageName.isEmpty() && currentDoc->masterPageMode())
		currentPage = currentDoc->currentPage()->pageName();
	else if (masterPageName.isEmpty())
		currentPage = currentDoc->MasterNames.begin().key();
	else
		currentPage = masterPageName;

	QString currentPageName;
	if (currentDoc->masterPageMode())
		currentPageName = currentDoc->currentPage()->pageName();
	if (!currentDoc->masterPageMode() || currentPage != currentPageName)
		currentView->showMasterPage(currentDoc->MasterNames[currentPage]);

	this->setEnabled(true);
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
	connect(masterPageListBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
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
	disconnect(masterPageListBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
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

void PagePalette_MasterPages::deleteMasterPage()
{
	bool forceDelete = false;

	// allow to delete multiple pages in one step
	foreach (QListWidgetItem * delItem, masterPageListBox->selectedItems())
	{
		currentPage = delItem->text();

		if ((currentPage == CommonStrings::masterPageNormal)
			|| (currentPage == CommonStrings::trMasterPageNormal)
			|| (currentPage == CommonStrings::trMasterPageNormalLeft)
			|| (currentPage == CommonStrings::trMasterPageNormalMiddle)
			|| (currentPage == CommonStrings::trMasterPageNormalRight))
			continue;

		QString extraWarn = "";

		if (!forceDelete)
		{
			for (int i=0; i < currentDoc->DocPages.count(); ++i )
			{
				if (currentDoc->DocPages[i]->MPageNam == currentPage)
					extraWarn = tr("This master page is used at least once in the document.");
			}
			int exit = QMessageBox::warning(this,
										CommonStrings::trWarning,
										tr("Do you really want to delete master page \"%1\"?").arg(currentPage)+"\n"+extraWarn,
										QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::Cancel);

			if (exit == QMessageBox::YesToAll)
				forceDelete = true;
			if (exit == QMessageBox::No)
				continue;
			if (exit == QMessageBox::Cancel)
				break;
		}

		if (currentDoc->appMode == modeEditClip)
			currentView->requestMode(submodeEndNodeEdit);
		currentDoc->scMW()->deletePage2(currentDoc->MasterNames[currentPage]);
		//<<CB TODO Move back into ScribusDoc::deleteMasterPage();
		//This must happen after the pages have been reformed (view/doc)
		currentDoc->rebuildMasterNames();
		// Fix up any pages that refer to the deleted master page
		currentDoc->replaceMasterPage(currentPage);
	}

	// set the 1st MP for the other slots
	QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin();
	currentPage = it.key();
	updateMasterPageList(currentPage);
}

void PagePalette_MasterPages::duplicateMasterPage()
{
	int copyC = 1;
	QString potentialMasterPageName(currentPage);
	while (currentDoc->MasterNames.contains(potentialMasterPageName))
		potentialMasterPageName = tr("Copy #%1 of %2").arg(copyC++).arg(currentPage);

	NewTm *dia = new NewTm(this, tr("&Name:"), tr("New Master Page"), currentDoc, potentialMasterPageName);
	if (dia->exec())
	{
		if (currentDoc->appMode == modeEditClip)
			currentView->requestMode(submodeEndNodeEdit);
		QString MasterPageName = dia->Answer->text();
		bool MasterPageNameWrong = currentDoc->MasterNames.contains(MasterPageName);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::masterPageNormal);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormal);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalLeft);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalMiddle);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalRight);
		MasterPageNameWrong |=  MasterPageName.isEmpty();
		while (MasterPageNameWrong)
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			MasterPageName = dia->Answer->text();
			bool MasterPageNameWrong = currentDoc->MasterNames.contains(MasterPageName);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::masterPageNormal);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormal);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalLeft);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalMiddle);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalRight);
			MasterPageNameWrong |=  MasterPageName.isEmpty();
		}
		PrefsManager* prefsManager = PrefsManager::instance();
		int inde = currentDoc->MasterNames[currentPage];
		int nr = currentDoc->Pages->count();
		ScPage* from = currentDoc->Pages->at(inde);
		ScPage* destination = currentDoc->addMasterPage(nr, MasterPageName);
		if (currentDoc->pagePositioning() != singlePage)
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
		destination->initialMargins.Top = from->initialMargins.Top;
		destination->initialMargins.Bottom = from->initialMargins.Bottom;
		if (currentDoc->pageSets()[currentDoc->pagePositioning()].Columns == 1)
		{
			destination->initialMargins.Left = from->initialMargins.Left;
			destination->initialMargins.Right = from->initialMargins.Right;
		}
		else
		{
			if (destination->LeftPg != from->LeftPg)
			{
				if (destination->LeftPg > 1)
				{
					destination->initialMargins.Right = from->initialMargins.Left;
					destination->initialMargins.Left = from->initialMargins.Left;
				}
				else
				{
					destination->initialMargins.Left = from->initialMargins.Left;
					destination->initialMargins.Right = from->initialMargins.Right;
				}
			}
			else
			{
				destination->initialMargins.Left = from->initialMargins.Left;
				destination->initialMargins.Right = from->initialMargins.Right;
			}
		}
		//#8321 : incorrect selection of master page on new mp creation/duplictation
		//currentDoc->setCurrentPage(destination);
		selectMasterPage(MasterPageName);
		uint oldItems = currentDoc->Items->count();
		uint end2 = currentDoc->MasterItems.count();
		int GrMax = currentDoc->GroupCounter;
		currentDoc->m_Selection->clear();
		if (oldItems>0)
		{
			ScLayers::iterator it;
			currentDoc->m_Selection->delaySignalsOn();
			for (it = currentDoc->Layers.begin(); it != currentDoc->Layers.end(); ++it)
			{
				for (uint ite = 0; ite < oldItems; ++ite)
				{
					PageItem *itemToCopy = currentDoc->Items->at(ite);
					if (itemToCopy->OwnPage == inde && (it->ID == itemToCopy->LayerID))
						currentDoc->m_Selection->addItem(itemToCopy, true);
				}
				if (currentDoc->m_Selection->count() != 0)
				{
					ScriXmlDoc *ss = new ScriXmlDoc();
					QString buffer = ss->WriteElem(currentDoc, currentDoc->m_Selection);
					ss->ReadElemToLayer(buffer, prefsManager->appPrefs.fontPrefs.AvailFonts, currentDoc, destination->xOffset(), destination->yOffset(), false, true, prefsManager->appPrefs.fontPrefs.GFontSub, it->ID);
					currentDoc->m_Selection->clear();
					delete ss;
				}
			}
			currentDoc->m_Selection->clear();
			currentDoc->m_Selection->delaySignalsOff();
		}
		uint end3 = currentDoc->MasterItems.count();
		for (uint a = end2; a < end3; ++a)
		{
			PageItem *newItem = currentDoc->MasterItems.at(a);
			newItem->OnMasterPage = MasterPageName;
			newItem->OwnPage = currentDoc->MasterNames[MasterPageName];
		}
		from->guides.copy(&destination->guides);
		currentDoc->GroupCounter = GrMax + 1;
		currentView->Deselect(true);
		updateMasterPageList(MasterPageName);
		currentDoc->setLoading(false);
		currentView->reformPages();
		currentView->DrawNew();
	}
	delete dia;
}

void PagePalette_MasterPages::newMasterPage()
{
	QString MasterPageName;
	int nr = currentDoc->Pages->count();
	NewTm *dia = new NewTm(this, tr("Name:"), tr("New MasterPage"), currentDoc, tr("New Master Page %1").arg(nr));
	if (dia->exec())
	{
		if (currentDoc->appMode == modeEditClip)
			currentView->requestMode(submodeEndNodeEdit);
		MasterPageName = dia->Answer->text();
		bool MasterPageNameWrong = currentDoc->MasterNames.contains(MasterPageName);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::masterPageNormal);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormal);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalLeft);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalMiddle);
		MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalRight);
		MasterPageNameWrong |=  MasterPageName.isEmpty();
		while (MasterPageNameWrong)
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			MasterPageName = dia->Answer->text();
			MasterPageNameWrong = currentDoc->MasterNames.contains(MasterPageName);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::masterPageNormal);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormal);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalLeft);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalMiddle);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalRight);
			MasterPageNameWrong |=  MasterPageName.isEmpty();
		}
		currentDoc->setCurrentPage(currentDoc->addMasterPage(nr, MasterPageName));
		if (currentDoc->pagePositioning() != singlePage)
		{
			int lp = dia->Links->currentIndex();
			if (lp == 0)
				lp = 1;
			else if (lp == static_cast<int>(dia->Links->count()-1))
				lp = 0;
			else
				lp++;
			currentDoc->Pages->at(nr)->LeftPg = lp;
		}
		updateMasterPageList(MasterPageName);
		//#8321 : incorrect selection of master page on new mp creation/duplictation
		//currentView->showMasterPage(currentDoc->MasterNames[MasterPageName]);
		selectMasterPage(MasterPageName);
		currentView->reformPages();
	}
	delete dia;
}

void PagePalette_MasterPages::importPage()
{
	//bool atf;
	MergeDoc *dia = new MergeDoc(this, true);
	if (dia->exec())
	{
		if (currentDoc->appMode == modeEditClip)
			currentView->requestMode(submodeEndNodeEdit);
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
		int nr = currentDoc->Pages->count();
		//currentDoc->pageCount = 0;
		//atf = currentDoc->usesAutomaticTextFrames();
		//currentDoc->setUsesAutomaticTextFrames(false);
		//emit createNew(nr);
		QString MasterPageName(dia->getMasterPageNameText());
		QString MasterPageName2(MasterPageName);
		int copyC = 1;
		while (currentDoc->MasterNames.contains(MasterPageName2))
		{
			MasterPageName2 = tr("Copy #%1 of ").arg(copyC)+MasterPageName;
			copyC++;
		}
		currentDoc->setCurrentPage(currentDoc->addMasterPage(nr, MasterPageName2));
		qApp->processEvents();
		//CB TODO: If we are loading to a new name, we rely on doc->onpage in 
		//FileLoader::PasteItem as this call doesnt pass in the new destination page
		currentDoc->scMW()->loadPage(dia->getFromDoc(), dia->getMasterPageNameItem(), true, MasterPageName2);
		qApp->processEvents();
		/*
		MasterPageName = currentDoc->Pages->at(nr)->PageNam;
		MasterPageName2 = MasterPageName;
		int copyC = 1;
		while (currentDoc->MasterNames.contains(MasterPageName2))
		{
			MasterPageName2 = tr("Copy #%1 of ").arg(copyC)+MasterPageName;
			copyC++;
		}
		currentDoc->MasterNames.insert(MasterPageName2, nr);
		currentDoc->Pages->at(nr)->setPageName(MasterPageName2);
		currentDoc->Pages->at(nr)->MPageNam = "";
		*/
		updateMasterPageList(MasterPageName2);
		//currentDoc->setUsesAutomaticTextFrames(atf);
		currentView->showMasterPage(currentDoc->MasterNames[MasterPageName2]);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		//currentDoc->MasterPages = currentDoc->Pages;
	}
	delete dia;
}

void PagePalette_MasterPages::selectMasterPage(QListWidgetItem *item)
{
	currentPage = item->text();
	deleteButton->setEnabled(currentDoc->MasterNames.count() == 1 ? false : true);
	if (currentPage == CommonStrings::trMasterPageNormal || currentPage == CommonStrings::masterPageNormal)
	{
// 		currentPage = CommonStrings::masterPageNormal;
		deleteButton->setEnabled(false);
	}
	else
		deleteButton->setEnabled(true);
	if (currentDoc->appMode == modeEditClip)
		currentView->requestMode(submodeEndNodeEdit);
	else if (currentDoc->appMode == modeEdit || currentDoc->appMode == modeEditGradientVectors)
		currentView->requestMode(modeNormal);
	currentView->showMasterPage(currentDoc->MasterNames[currentPage]);
}

void PagePalette_MasterPages::selectMasterPage(QString name)
{
	if (!currentDoc || !currentView)
		return;

	currentPage = name;
	deleteButton->setEnabled(currentDoc->MasterNames.count() == 1 ? false : true);
	if (currentPage == CommonStrings::trMasterPageNormal)
	{
		currentPage = CommonStrings::masterPageNormal;
		deleteButton->setEnabled(false);
	}
	else
		deleteButton->setEnabled(true);
	if (currentDoc->appMode == modeEditClip)
		currentView->requestMode(submodeEndNodeEdit);
	currentView->showMasterPage(currentDoc->MasterNames[currentPage]);
}

void PagePalette_MasterPages::updateMasterPageList(void)
{
	QString masterPageName = (currentDoc->MasterNames.contains(currentPage)) ? currentPage : CommonStrings::masterPageNormal;
	updateMasterPageList(masterPageName);
}

void PagePalette_MasterPages::updateMasterPageList(QString masterPageName)
{
	if (!currentDoc || !currentView)
		return;

	masterPageListBox->clear();
	for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
		masterPageListBox->addItem(it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key());
	deleteButton->setEnabled(currentDoc->MasterNames.count() == 1 ? false : true);
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
	QString oldName(item->text());
	if ((oldName == CommonStrings::masterPageNormal) || (oldName == CommonStrings::trMasterPageNormal) || (oldName == CommonStrings::trMasterPageNormalLeft) || (oldName == CommonStrings::trMasterPageNormalMiddle) || (oldName == CommonStrings::trMasterPageNormalRight))
	{
		QMessageBox::information( this, tr("Unable to Rename Master Page"), tr("The Normal page is not allowed to be renamed."), QMessageBox::Ok );
		return;
	}
	bool ok;
	QString newName = QInputDialog::getText( this, tr("Rename Master Page"), tr("New Name:"), QLineEdit::Normal, oldName, &ok);
	if (ok && !newName.isEmpty())
	{
		if (currentDoc->renameMasterPage( oldName, newName))
			updateMasterPageList(newName);
	}
}


void PagePalette_MasterPages::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}

	QWidget::changeEvent(e);
}
