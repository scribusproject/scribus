/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "masterpagepalette.h"

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
#include "undomanager.h"
#include "util_icon.h"

MasterPagesPalette::MasterPagesPalette( QWidget* parent, ScribusDoc *pCurrentDoc, ScribusView *pCurrentView, QString masterPageName) : ScrPaletteBase( parent, "MasterPages", false, 0 )
{
	setAttribute(Qt::WA_DeleteOnClose);
	setModal(false);
	undoManager = UndoManager::instance();
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	m_doc = pCurrentDoc;
	m_view = pCurrentView;
	masterPagesLayout = new QVBoxLayout(this);
	masterPagesLayout->setMargin(5);
	masterPagesLayout->setSpacing(5);
	buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing( 5 );
	buttonLayout->setMargin( 0 );
	importButton = new QToolButton(this);
	importButton->setIcon(QIcon(loadIcon("16/document-open.png")));
	newButton = new QToolButton(this);
	newButton->setIcon(QIcon(loadIcon("16/document-new.png")));
	duplicateButton = new QToolButton(this);
	duplicateButton->setIcon(QIcon(loadIcon("16/edit-copy.png")));
	deleteButton = new QToolButton(this);
	deleteButton->setIcon(QIcon(loadIcon("16/edit-delete.png")));
	buttonLayout->addWidget( newButton );
	buttonLayout->addWidget( duplicateButton );
	buttonLayout->addWidget( importButton );
	buttonLayout->addWidget( deleteButton );
	QSpacerItem* spacer = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum );
	buttonLayout->addItem( spacer );
	masterPagesLayout->addLayout( buttonLayout );
	masterPageListBox = new QListWidget( this );
	masterPageListBox->clear();
	masterPageListBox->setMinimumSize( QSize( 100, 240 ) );
	masterPageListBox->setSelectionMode(QAbstractItemView::ExtendedSelection);
	masterPagesLayout->addWidget( masterPageListBox );

	languageChange();

	if (masterPageName.isEmpty())
		m_masterPage = m_doc->MasterNames.begin().key();
	else
		m_masterPage = masterPageName;
	updateMasterPageList(m_masterPage);
	m_view->showMasterPage(m_doc->MasterNames[m_masterPage]);

	setMinimumSize(sizeHint());

	// signals and slots connections
	connect(duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateMasterPage()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteMasterPage()));
	connect(newButton, SIGNAL(clicked()), this, SLOT(newMasterPage()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(importPage()));
	connect(masterPageListBox, SIGNAL(itemClicked(QListWidgetItem*)),
			 this, SLOT(selectMasterPage(QListWidgetItem*)));
	connect(masterPageListBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
			 this, SLOT(renameMasterPage( QListWidgetItem*)));
}

void MasterPagesPalette::languageChange()
{
	setWindowTitle( tr( "Edit Master Pages" ) );
	duplicateButton->setToolTip( tr( "Duplicate the selected master page" ) );
	deleteButton->setToolTip( tr( "Delete the selected master page" ) );
	newButton->setToolTip( tr( "Add a new master page" ) );
	importButton->setToolTip( tr( "Import master pages from another document" ) );
}

void MasterPagesPalette::reject()
{
	emit finished();
	QDialog::reject();
}

void MasterPagesPalette::closeEvent(QCloseEvent *closeEvent)
{
	emit finished();
	hide();
	closeEvent->accept();
}

void MasterPagesPalette::deleteMasterPage()
{
	bool forceDelete = false;
	UndoTransaction* activeTransaction = NULL;
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::Group, Um::IGroup, Um::DelMasterPage, "", Um::IDelete));
	// allow to delete multiple pages in one step
	foreach (QListWidgetItem * delItem, masterPageListBox->selectedItems())
	{
		m_masterPage = delItem->text();

		if ((m_masterPage == CommonStrings::masterPageNormal)
			|| (m_masterPage == CommonStrings::trMasterPageNormal)
			|| (m_masterPage == CommonStrings::trMasterPageNormalLeft)
			|| (m_masterPage == CommonStrings::trMasterPageNormalMiddle)
			|| (m_masterPage == CommonStrings::trMasterPageNormalRight))
			continue;

		QString extraWarn = "";

		if (!forceDelete)
		{
			for (int i=0; i < m_doc->DocPages.count(); ++i )
			{
				if (m_doc->DocPages[i]->MPageNam == m_masterPage)
					extraWarn = tr("This master page is used at least once in the document.");
			}
			int exit = QMessageBox::warning(this,
										CommonStrings::trWarning,
										tr("Do you really want to delete master page \"%1\"?").arg(m_masterPage)+"\n"+extraWarn,
										QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::Cancel);

			if (exit == QMessageBox::YesToAll)
				forceDelete = true;
			if (exit == QMessageBox::No)
				continue;
			if (exit == QMessageBox::Cancel)
				break;
		}

		if (m_doc->appMode == modeEditClip)
			m_view->requestMode(submodeEndNodeEdit);
		m_doc->scMW()->deletePage2(m_doc->MasterNames[m_masterPage]);
		//<<CB TODO Move back into ScribusDoc::deleteMasterPage();
		//This must happen after the pages have been reformed (view/doc)
		m_doc->rebuildMasterNames();
		// Fix up any pages that refer to the deleted master page
		m_doc->replaceMasterPage(m_masterPage);
	}

	// set the 1st MP for the other slots
	QMap<QString,int>::Iterator it = m_doc->MasterNames.begin();
	m_masterPage = it.key();
	updateMasterPageList(m_masterPage);
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
}

void MasterPagesPalette::duplicateMasterPage()
{
	int copyC = 1;
	UndoTransaction* activeTransaction = NULL;
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::Group, Um::IGroup, Um::DuplicateMasterPage, "", Um::IGroup));
	QString potentialMasterPageName(m_masterPage);
	while (m_doc->MasterNames.contains(potentialMasterPageName))
		potentialMasterPageName = tr("Copy #%1 of %2").arg(copyC++).arg(m_masterPage);

	NewTm *dia = new NewTm(this, tr("&Name:"), tr("New Master Page"), m_doc, potentialMasterPageName);
	if (dia->exec())
	{
		if (m_doc->appMode == modeEditClip)
			m_view->requestMode(submodeEndNodeEdit);
		QString MasterPageName = dia->Answer->text();
		bool MasterPageNameWrong = m_doc->MasterNames.contains(MasterPageName);
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
			bool MasterPageNameWrong = m_doc->MasterNames.contains(MasterPageName);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::masterPageNormal);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormal);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalLeft);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalMiddle);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalRight);
			MasterPageNameWrong |=  MasterPageName.isEmpty();
		}
		PrefsManager* prefsManager = PrefsManager::instance();
		int inde = m_doc->MasterNames[m_masterPage];
		int nr = m_doc->Pages->count();
		ScPage* from = m_doc->Pages->at(inde);
		ScPage* destination = m_doc->addMasterPage(nr, MasterPageName);
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
		destination->initialMargins.Top = from->initialMargins.Top;
		destination->initialMargins.Bottom = from->initialMargins.Bottom;
		if (m_doc->pageSets()[m_doc->pagePositioning()].Columns == 1)
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
		//m_doc->setCurrentPage(destination);
		selectMasterPage(MasterPageName);
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
					if (itemToCopy->OwnPage == inde && (it->ID == itemToCopy->LayerID))
						m_doc->m_Selection->addItem(itemToCopy, true);
				}
				if (m_doc->m_Selection->count() != 0)
				{
					ScriXmlDoc ss;
					QString buffer = ss.WriteElem(m_doc, m_doc->m_Selection);
					ss.ReadElemToLayer(buffer, prefsManager->appPrefs.fontPrefs.AvailFonts, m_doc, destination->xOffset(), destination->yOffset(), false, true, prefsManager->appPrefs.fontPrefs.GFontSub, it->ID);
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
			newItem->OnMasterPage = MasterPageName;
			newItem->OwnPage = m_doc->MasterNames[MasterPageName];
		}
		from->guides.copy(&destination->guides);
		m_doc->GroupCounter = GrMax + 1;
		m_view->Deselect(true);
		updateMasterPageList(MasterPageName);
		m_doc->setLoading(false);
		m_view->reformPages();
		m_view->DrawNew();
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	delete dia;
}

void MasterPagesPalette::newMasterPage()
{
	QString MasterPageName;
	UndoTransaction* activeTransaction = NULL;
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::Group, Um::IGroup, Um::NewMasterPage, "", Um::IGroup));
	int nr = m_doc->Pages->count();
	NewTm *dia = new NewTm(this, tr("Name:"), tr("New MasterPage"), m_doc, tr("New Master Page %1").arg(nr));
	if (dia->exec())
	{
		if (m_doc->appMode == modeEditClip)
			m_view->requestMode(submodeEndNodeEdit);
		MasterPageName = dia->Answer->text();
		bool MasterPageNameWrong = m_doc->MasterNames.contains(MasterPageName);
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
			MasterPageNameWrong = m_doc->MasterNames.contains(MasterPageName);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::masterPageNormal);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormal);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalLeft);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalMiddle);
			MasterPageNameWrong |= (MasterPageName == CommonStrings::trMasterPageNormalRight);
			MasterPageNameWrong |=  MasterPageName.isEmpty();
		}
		m_doc->setCurrentPage(m_doc->addMasterPage(nr, MasterPageName));
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
		updateMasterPageList(MasterPageName);
		//#8321 : incorrect selection of master page on new mp creation/duplictation
		//m_view->showMasterPage(m_doc->MasterNames[MasterPageName]);
		selectMasterPage(MasterPageName);
		m_view->reformPages();
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	delete dia;
}

void MasterPagesPalette::importPage()
{
	UndoTransaction* activeTransaction = NULL;
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::Group, Um::IGroup, Um::ImportMasterPage, "", Um::IGroup));
	//bool atf;
	MergeDoc *dia = new MergeDoc(this, true);
	if (dia->exec())
	{
		if (m_doc->appMode == modeEditClip)
			m_view->requestMode(submodeEndNodeEdit);
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
		int nr = m_doc->Pages->count();
		//m_doc->pageCount = 0;
		//atf = m_doc->usesAutomaticTextFrames();
		//m_doc->setUsesAutomaticTextFrames(false);
		//emit createNew(nr);
		QString MasterPageName(dia->getMasterPageNameText());
		QString MasterPageName2(MasterPageName);
		int copyC = 1;
		while (m_doc->MasterNames.contains(MasterPageName2))
		{
			MasterPageName2 = tr("Copy #%1 of ").arg(copyC)+MasterPageName;
			copyC++;
		}
		m_doc->setCurrentPage(m_doc->addMasterPage(nr, MasterPageName2));
		qApp->processEvents();
		//CB TODO: If we are loading to a new name, we rely on doc->onpage in 
		//FileLoader::PasteItem as this call doesnt pass in the new destination page
		m_doc->scMW()->loadPage(dia->getFromDoc(), dia->getMasterPageNameItem(), true, MasterPageName2);
		qApp->processEvents();
		/*
		MasterPageName = m_doc->Pages->at(nr)->PageNam;
		MasterPageName2 = MasterPageName;
		int copyC = 1;
		while (m_doc->MasterNames.contains(MasterPageName2))
		{
			MasterPageName2 = tr("Copy #%1 of ").arg(copyC)+MasterPageName;
			copyC++;
		}
		m_doc->MasterNames.insert(MasterPageName2, nr);
		m_doc->Pages->at(nr)->setPageName(MasterPageName2);
		m_doc->Pages->at(nr)->MPageNam = "";
		*/
		updateMasterPageList(MasterPageName2);
		//m_doc->setUsesAutomaticTextFrames(atf);
		m_view->showMasterPage(m_doc->MasterNames[MasterPageName2]);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		//m_doc->MasterPages = m_doc->Pages;
	}
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	delete dia;
}

void MasterPagesPalette::selectMasterPage(QListWidgetItem *item)
{
	m_masterPage = item->text();
	deleteButton->setEnabled(m_doc->MasterNames.count() == 1 ? false : true);
	if (m_masterPage == CommonStrings::trMasterPageNormal || m_masterPage == CommonStrings::masterPageNormal)
	{
// 		m_masterPage = CommonStrings::masterPageNormal;
		deleteButton->setEnabled(false);
	}
	else
		deleteButton->setEnabled(true);
	if (m_doc->appMode == modeEditClip)
		m_view->requestMode(submodeEndNodeEdit);
	else if (m_doc->appMode == modeEdit || m_doc->appMode == modeEditGradientVectors)
		m_view->requestMode(modeNormal);
	m_view->showMasterPage(m_doc->MasterNames[m_masterPage]);
}

void MasterPagesPalette::selectMasterPage(QString name)
{
	m_masterPage = name;
	deleteButton->setEnabled(m_doc->MasterNames.count() == 1 ? false : true);
	if (m_masterPage == CommonStrings::trMasterPageNormal)
	{
		m_masterPage = CommonStrings::masterPageNormal;
		deleteButton->setEnabled(false);
	}
	else
		deleteButton->setEnabled(true);
	if (m_doc->appMode == modeEditClip)
		m_view->requestMode(submodeEndNodeEdit);
	m_view->showMasterPage(m_doc->MasterNames[m_masterPage]);
}

void MasterPagesPalette::updateMasterPageList(void)
{
	QString masterPageName = (m_doc->MasterNames.contains(m_masterPage)) ? m_masterPage : CommonStrings::masterPageNormal;
	updateMasterPageList(masterPageName);
}

void MasterPagesPalette::updateMasterPageList(QString MasterPageName)
{
	masterPageListBox->clear();
	for (QMap<QString,int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
		masterPageListBox->addItem(it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key());
	deleteButton->setEnabled(m_doc->MasterNames.count() == 1 ? false : true);
	if (MasterPageName == CommonStrings::masterPageNormal)
	{
		MasterPageName = CommonStrings::trMasterPageNormal;
		deleteButton->setEnabled(false);
	}
	QList<QListWidgetItem *> itL = masterPageListBox->findItems(MasterPageName, Qt::MatchExactly);
	if (itL.count() != 0)
		itL.at(0)->setSelected(true);
}

void MasterPagesPalette::renameMasterPage(QListWidgetItem * item)
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
		if (m_doc->renameMasterPage( oldName, newName))
			updateMasterPageList(newName);
	}
}


void MasterPagesPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}


