/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "muster.h"
#include "muster.moc"
#include "newtemp.h"
#include "mergedoc.h"
#include <qinputdialog.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qcursor.h>
#include <qstring.h>
#include <qtooltip.h>
#include "page.h"
#include "pagestructs.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "util.h"
#include "commonstrings.h"
#include "scribus.h"

#include "scmessagebox.h"

MasterPagesPalette::MasterPagesPalette( QWidget* parent, ScribusDoc *pCurrentDoc, ScribusView *pCurrentView, QString masterPageName)
		: QDialog( parent, "Muster", false, WDestructiveClose)
{
	setCaption( tr( "Edit Master Pages" ) );
	setIcon(loadIcon("AppIcon.png"));
	currentDoc = pCurrentDoc;
	currentView = pCurrentView;
	masterPagesLayout = new QVBoxLayout( this, 5, 5 );
	buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing( 5 );
	buttonLayout->setMargin( 0 );
	importButton = new QToolButton(this, "importButton" );
	importButton->setPixmap(loadIcon("fileopen.png"));
	newButton = new QToolButton(this, "newButton" );
	newButton->setPixmap(loadIcon("filenew.png"));
	duplicateButton = new QToolButton(this, "DublicateB" );
	duplicateButton->setPixmap(loadIcon("editcopy22.png"));
	deleteButton = new QToolButton(this, "deleteButton" );
	deleteButton->setPixmap(loadIcon("edittrash.png"));
	buttonLayout->addWidget( newButton );
	buttonLayout->addWidget( duplicateButton );
	buttonLayout->addWidget( importButton );
	buttonLayout->addWidget( deleteButton );
	QSpacerItem* spacer = new QSpacerItem( 16, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
	buttonLayout->addItem( spacer );
	masterPagesLayout->addLayout( buttonLayout );
	masterPageListBox = new QListBox( this, "masterPageListBox" );
	masterPageListBox->setMinimumSize( QSize( 100, 240 ) );
	masterPagesLayout->addWidget( masterPageListBox );


	if (masterPageName.isEmpty())
	{
		sMuster = "Normal";
		updateMasterPageList(sMuster);
		currentView->showMasterPage(0);
	}
	else
	{
		sMuster = masterPageName;
		updateMasterPageList(sMuster);
		currentView->showMasterPage(currentDoc->MasterNames[sMuster]);
	}

	setMinimumSize(sizeHint());

	QToolTip::add( duplicateButton, tr( "Duplicate the selected master page" ) );
	QToolTip::add( deleteButton, tr( "Delete the selected master page" ) );
	QToolTip::add( newButton, tr( "Add a new master page" ) );
	QToolTip::add( importButton, tr( "Import master pages from another document" ) );
	// signals and slots connections
	connect(duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateMasterPage()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteMasterPage()));
	connect(newButton, SIGNAL(clicked()), this, SLOT(newMasterPage()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(appendPage()));
	connect(masterPageListBox, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selectMasterPage(QListBoxItem*)));
	connect(masterPageListBox, SIGNAL(doubleClicked(QListBoxItem*)), this, SLOT(renameMasterPage( QListBoxItem*)));
}

void MasterPagesPalette::reject()
{
	emit finished();
	QDialog::reject();
}

void MasterPagesPalette::closeEvent(QCloseEvent *closeEvent)
{
	emit finished();
	closeEvent->accept();
}

void MasterPagesPalette::deleteMasterPage()
{
	if (sMuster == "Normal")
		return;
	int exit=ScMessageBox::warning(this,
	                              CommonStrings::trWarning,
	                              tr("Do you really want to delete this master page?"),
	                              tr("&Yes"),
	                              tr("&No"),
	                              0, QMessageBox::No, QMessageBox::Yes);
	if (exit == 0)
	{
		currentDoc->scMW()->DeletePage2(currentDoc->MasterNames[sMuster]);
		//<<CB TODO Move back into ScribusDoc::deleteMasterPage();
		//This must happen after the pages have been reformed (view/doc)
		currentDoc->MasterNames.clear();
		for (uint a = 0; a < currentDoc->Pages->count(); ++a)
			currentDoc->MasterNames[currentDoc->Pages->at(a)->pageName()] = currentDoc->Pages->at(a)->pageNr();
		// and fix up any pages that refer to the deleted master page
		for (Page* docPage = currentDoc->DocPages.first(); docPage; docPage = currentDoc->DocPages.next() )
		{
			if (docPage->MPageNam == sMuster)
				docPage->MPageNam = "Normal";
		}
		//>>
		
		sMuster = "Normal";
		updateMasterPageList(sMuster);
		//currentDoc->MasterPages = currentDoc->Pages;
	}
}

void MasterPagesPalette::duplicateMasterPage()
{
	int copyC = 1;
	QString potentialMasterPageName(sMuster);
	while (currentDoc->MasterNames.contains(potentialMasterPageName))
		potentialMasterPageName = tr("Copy #%1 of ").arg(copyC++)+sMuster;

	NewTm *dia = new NewTm(this, tr("&Name:"), tr("New Master Page"), currentDoc, potentialMasterPageName);
	if (dia->exec())
	{
		QString MasterPageName = dia->Answer->text();
		while (currentDoc->MasterNames.contains(MasterPageName) || (MasterPageName == "Normal"))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			MasterPageName = dia->Answer->text();
		}
		int nr = currentDoc->Pages->count();
		currentDoc->setCurrentPage(currentDoc->addMasterPage(nr, MasterPageName));
		currentDoc->setLoading(true);
		if (currentDoc->currentPageLayout != singlePage)
		{
			int lp = dia->Links->currentItem();
			if (lp == 0)
				lp = 1;
			else if (lp == static_cast<int>(dia->Links->count()-1))
				lp = 0;
			else
				lp++;
			currentDoc->Pages->at(nr)->LeftPg = lp;
		}
		int inde = currentDoc->MasterNames[sMuster];
		QMap<int,int> TableID;
		QPtrList<PageItem> TableItems;
		TableID.clear();
		TableItems.clear();
		/* PV guides refactoring
		if (currentDoc->Pages->at(inde)->YGuides.count() != 0)
		{
			currentDoc->currentPage->YGuides.clear();
			for (uint y = 0; y < currentDoc->Pages->at(inde)->YGuides.count(); ++y)
			{
				currentDoc->currentPage->YGuides.append(currentDoc->Pages->at(inde)->YGuides[y]);
			}
			qHeapSort(currentDoc->currentPage->YGuides);
		}
		if (currentDoc->Pages->at(inde)->XGuides.count() != 0)
		{
			currentDoc->currentPage->XGuides.clear();
			for (uint x = 0; x < currentDoc->Pages->at(inde)->XGuides.count(); ++x)
			{
				currentDoc->currentPage->XGuides.append(currentDoc->Pages->at(inde)->XGuides[x]);
			}
			qHeapSort(currentDoc->currentPage->XGuides);
		}
		*/
		currentDoc->Pages->at(inde)->guides.copy(&currentDoc->currentPage()->guides);
		uint end = currentDoc->Items->count();
		int GrMax = currentDoc->GroupCounter;
		struct CopyPasteBuffer Buffer;
		for (uint a = 0; a < end; ++a)
		{
			PageItem *itemToCopy = currentDoc->Items->at(a);
			if (currentDoc->Items->at(a)->OwnPage == inde)
			{
				itemToCopy->copyToCopyPasteBuffer(&Buffer);
				if (itemToCopy->Groups.count() != 0)
				{
					Buffer.Groups.clear();
					QValueStack<int>::Iterator nx;
					QValueStack<int> tmpGroup;
					for (nx = itemToCopy->Groups.begin(); nx != itemToCopy->Groups.end(); ++nx)
					{
						tmpGroup.push((*nx)+currentDoc->GroupCounter);
						GrMax = QMAX(GrMax, (*nx)+currentDoc->GroupCounter);
					}
					for (nx = tmpGroup.begin(); nx != tmpGroup.end(); ++nx)
					{
						Buffer.Groups.push((*nx));
					}
				}
				currentView->PasteItem(&Buffer, true, true);
				PageItem* Neu = currentDoc->Items->at(currentDoc->Items->count()-1);
				Neu->OnMasterPage = MasterPageName;
				if (Neu->isTableItem)
				{
					TableItems.append(Neu);
					TableID.insert(a, Neu->ItemNr);
				}
			}
		}
		if (TableItems.count() != 0)
		{
			for (uint ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->TopLink = currentDoc->Items->at(TableID[ta->TopLinkID]);
				else
					ta->TopLink = 0;
				if (ta->LeftLinkID != -1)
					ta->LeftLink = currentDoc->Items->at(TableID[ta->LeftLinkID]);
				else
					ta->LeftLink = 0;
				if (ta->RightLinkID != -1)
					ta->RightLink = currentDoc->Items->at(TableID[ta->RightLinkID]);
				else
					ta->RightLink = 0;
				if (ta->BottomLinkID != -1)
					ta->BottomLink = currentDoc->Items->at(TableID[ta->BottomLinkID]);
				else
					ta->BottomLink = 0;
			}
		}
		currentView->Deselect(true);
		currentView->DrawNew();
		updateMasterPageList(MasterPageName);
		currentDoc->setLoading(false);
		currentView->DrawNew();
		currentDoc->GroupCounter = GrMax + 1;
	}
	delete dia;
}

void MasterPagesPalette::newMasterPage()
{
	QString MasterPageName;
	int nr = currentDoc->Pages->count();
	NewTm *dia = new NewTm(this, tr("Name:"), tr("New MasterPage"), currentDoc, tr("New Master Page %1").arg(nr));
	if (dia->exec())
	{
		MasterPageName = dia->Answer->text();
		while (currentDoc->MasterNames.contains(MasterPageName) || (MasterPageName == "Normal"))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			MasterPageName = dia->Answer->text();
		}
		currentDoc->setCurrentPage(currentDoc->addMasterPage(nr, MasterPageName));
		if (currentDoc->currentPageLayout != singlePage)
		{
			int lp = dia->Links->currentItem();
			if (lp == 0)
				lp = 1;
			else if (lp == static_cast<int>(dia->Links->count()-1))
				lp = 0;
			else
				lp++;
			currentDoc->Pages->at(nr)->LeftPg = lp;
		}
		updateMasterPageList(MasterPageName);
		currentView->showMasterPage(currentDoc->MasterNames[MasterPageName]);
		currentView->reformPages();
	}
	delete dia;
}

void MasterPagesPalette::appendPage()
{
	//bool atf;
	MergeDoc *dia = new MergeDoc(this, true);
	if (dia->exec())
	{
		qApp->setOverrideCursor(QCursor(waitCursor), true);
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
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		//currentDoc->MasterPages = currentDoc->Pages;
	}
	delete dia;
}

void MasterPagesPalette::selectMasterPage(QListBoxItem *item)
{
	sMuster = item->text();
	deleteButton->setEnabled(currentDoc->MasterNames.count() == 1 ? false : true);
	if (sMuster == tr("Normal"))
	{
		sMuster = "Normal";
		deleteButton->setEnabled(false);
	}
	else
		deleteButton->setEnabled(true);
	currentView->showMasterPage(currentDoc->MasterNames[sMuster]);
}

void MasterPagesPalette::selectMasterPage(QString name)
{
	sMuster = name;
	deleteButton->setEnabled(currentDoc->MasterNames.count() == 1 ? false : true);
	if (sMuster == tr("Normal"))
	{
		sMuster = "Normal";
		deleteButton->setEnabled(false);
	}
	else
		deleteButton->setEnabled(true);
	currentView->showMasterPage(currentDoc->MasterNames[sMuster]);
}

void MasterPagesPalette::updateMasterPageList(QString MasterPageName)
{
	masterPageListBox->clear();
	for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
		masterPageListBox->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());
	deleteButton->setEnabled(currentDoc->MasterNames.count() == 1 ? false : true);
	if (MasterPageName == "Normal")
	{
		MasterPageName = tr("Normal");
		deleteButton->setEnabled(false);
	}
	masterPageListBox->setSelected(masterPageListBox->index(masterPageListBox->findItem(MasterPageName)), true);
}

void MasterPagesPalette::renameMasterPage(QListBoxItem * item)
{
	QString oldName(item->text());
	if (oldName=="Normal" || oldName==tr("Normal"))
	{
		QMessageBox::information( this, tr("Unable to Rename Master Page"), tr("The Normal page is not allowed to be renamed."), QMessageBox::Ok );
		return;
	}
	bool ok;
	QString newName = QInputDialog::getText(
			tr("Rename Master Page"), tr("New Name:"), QLineEdit::Normal,
			oldName, &ok, this );
	if (ok && !newName.isEmpty())
		if (currentDoc->renameMasterPage( oldName, newName))
			updateMasterPageList(newName);
}
