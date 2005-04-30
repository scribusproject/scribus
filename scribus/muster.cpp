#include "muster.h"
#include "muster.moc"
#include "newtemp.h"
#include "mergedoc.h"
#include <qlayout.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qcursor.h>
#include <qstring.h>
#include <qtooltip.h>
#include "scribusdoc.h"
#include "scribusview.h"

extern QPixmap loadIcon(QString nam);
extern void CopyPageItem(struct CopyPasteBuffer *Buffer, PageItem *currItem);

MusterPages::MusterPages( QWidget* parent, ScribusDoc *pCurrentDoc, ScribusView *pCurrentView, QString temp)
		: QDialog( parent, "Muster", false, WDestructiveClose)
{
	setCaption( tr( "Edit Templates" ) );
	setIcon(loadIcon("AppIcon.png"));
	currentDoc = pCurrentDoc;
	currentView = pCurrentView;
	musterPagesLayout = new QVBoxLayout( this, 5, 5 );
	buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing( 5 );
	buttonLayout->setMargin( 0 );
	appendButton = new QPushButton(this, "appendButton" );
	appendButton->setPixmap(loadIcon("fileopen.png"));
	newButton = new QPushButton(this, "newButton" );
	newButton->setPixmap(loadIcon("filenew.png"));
	duplicateButton = new QPushButton(this, "DublicateB" );
	duplicateButton->setPixmap(loadIcon("editcopy22.png"));
	deleteButton = new QPushButton(this, "deleteButton" );
	deleteButton->setPixmap(loadIcon("edittrash.png"));
	buttonLayout->addWidget( newButton );
	buttonLayout->addWidget( duplicateButton );
	buttonLayout->addWidget( appendButton );
	buttonLayout->addWidget( deleteButton );
	musterPagesLayout->addLayout( buttonLayout );
	templateData = new QListBox( this, "templateData" );
	templateData->setMinimumSize( QSize( 100, 240 ) );
	musterPagesLayout->addWidget( templateData );


	if (temp == "")
	{
		sMuster = "Normal";
		updateTemplateList(sMuster);
		currentView->ShowTemplate(0);
	}
	else
	{
		sMuster = temp;
		updateTemplateList(sMuster);
		currentView->ShowTemplate(currentDoc->MasterNames[sMuster]);
	}

	setMaximumSize(sizeHint());

	QToolTip::add( duplicateButton, tr( "Duplicates the selected master page" ) );
	QToolTip::add( deleteButton, tr( "Deletes the selected master page" ) );
	QToolTip::add( newButton, tr( "Adds a new master page" ) );
	QToolTip::add( appendButton, tr( "Loads master page(s) from another document" ) );
	// signals and slots connections
	connect(duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateTemplate()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteTemplate()));
	connect(newButton, SIGNAL(clicked()), this, SLOT(newTemplate()));
	connect(appendButton, SIGNAL(clicked()), this, SLOT(appendPage()));
	connect(templateData, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selectTemplate(QListBoxItem*)));
}

void MusterPages::reject()
{
	emit finished();
	QDialog::reject();
}

void MusterPages::closeEvent(QCloseEvent *closeEvent)
{
	emit finished();
	closeEvent->accept();
}

void MusterPages::deleteTemplate()
{
	if (sMuster == "Normal")
		return;
	int exit=QMessageBox::warning(this,
	                              tr("Warning"),
	                              tr("Do you really want to delete this Template?"),
	                              tr("&No"),
	                              tr("&Yes"),
	                              0, QMessageBox::No, QMessageBox::Yes);
	if (exit == 1)
	{
		currentDoc->PageC = currentDoc->Pages.count();
		currentView->delPage(currentDoc->MasterNames[sMuster]);
		currentView->reformPages();
		currentDoc->MasterNames.clear();
		for (uint a = 0; a < currentDoc->Pages.count(); ++a)
			currentDoc->MasterNames[currentDoc->Pages.at(a)->PageNam] = currentDoc->Pages.at(a)->PageNr;
		for (uint b = 0; b < currentDoc->DocPages.count(); ++b)
		{
			if (currentDoc->DocPages.at(b)->MPageNam == sMuster)
				currentDoc->DocPages.at(b)->MPageNam = "Normal";
		}
		currentDoc->PageC = 1;
		sMuster = "Normal";
		updateTemplateList(sMuster);
		currentDoc->MasterPages = currentDoc->Pages;
		emit docAltered(currentDoc);
	}
}

void MusterPages::duplicateTemplate()
{
	QString templateName;
	int nr;
	bool atf;
	struct CopyPasteBuffer Buffer;
	NewTm *dia = new NewTm(this, tr("&Name:"), tr("New Template"), currentDoc);
	dia->Answer->setText( tr("Copy of %1").arg(sMuster));
	dia->Answer->selectAll();
	if (dia->exec())
	{
		templateName = dia->Answer->text();
		while (currentDoc->MasterNames.contains(templateName) || (templateName == "Normal"))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			templateName = dia->Answer->text();
		}
		nr = currentDoc->Pages.count();
		currentDoc->MasterNames.insert(templateName, nr);
		currentDoc->PageC = 0;
		atf = currentDoc->PageAT;
		currentDoc->PageAT = false;
		emit createNew(nr);
		currentDoc->loading = true;
		if (currentDoc->PageFP)
			currentDoc->Pages.at(nr)->LeftPg = dia->Links->currentItem() == 0 ? true : false;
		int inde = currentDoc->MasterNames[sMuster];
		QMap<int,int> TableID;
		QPtrList<PageItem> TableItems;
		TableID.clear();
		TableItems.clear();
		if (currentDoc->Pages.at(inde)->YGuides.count() != 0)
		{
			currentDoc->currentPage->YGuides.clear();
			for (uint y = 0; y < currentDoc->Pages.at(inde)->YGuides.count(); ++y)
			{
				currentDoc->currentPage->YGuides.append(currentDoc->Pages.at(inde)->YGuides[y]);
			}
			qHeapSort(currentDoc->currentPage->YGuides);
		}
		if (currentDoc->Pages.at(inde)->XGuides.count() != 0)
		{
			for (uint x = 0; x < currentDoc->Pages.at(inde)->XGuides.count(); ++x)
			{
				currentDoc->currentPage->XGuides.append(currentDoc->Pages.at(inde)->XGuides[x]);
			}
			qHeapSort(currentDoc->currentPage->XGuides);
		}
		uint end = currentDoc->Items.count();
		for (uint a = 0; a < end; ++a)
		{
			if (currentDoc->Items.at(a)->OwnPage == inde)
			{
				CopyPageItem(&Buffer, currentDoc->Items.at(a));
				currentView->PasteItem(&Buffer, true, true);
				PageItem* Neu = currentDoc->Items.at(currentDoc->Items.count()-1);
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
					ta->TopLink = currentDoc->Items.at(TableID[ta->TopLinkID]);
				else
					ta->TopLink = 0;
				if (ta->LeftLinkID != -1)
					ta->LeftLink = currentDoc->Items.at(TableID[ta->LeftLinkID]);
				else
					ta->LeftLink = 0;
				if (ta->RightLinkID != -1)
					ta->RightLink = currentDoc->Items.at(TableID[ta->RightLinkID]);
				else
					ta->RightLink = 0;
				if (ta->BottomLinkID != -1)
					ta->BottomLink = currentDoc->Items.at(TableID[ta->BottomLinkID]);
				else
					ta->BottomLink = 0;
			}
		}
		currentView->Deselect(true);
		currentView->DrawNew();
		currentDoc->Pages.at(nr)->setPageName(templateName);
		currentDoc->Pages.at(nr)->MPageNam = "";
		updateTemplateList(templateName);
		currentDoc->PageAT = atf;
		currentDoc->MasterPages = currentDoc->Pages;
		currentDoc->loading = false;
		currentView->DrawNew();
		emit docAltered(currentDoc);
	}
	delete dia;
}

void MusterPages::newTemplate()
{
	QString templateName;
	int nr;
	bool atf;
	NewTm *dia = new NewTm(this, tr("Name:"), tr("New Template"), currentDoc);
	dia->Answer->setText( tr("New Template"));
	dia->Answer->selectAll();
	if (dia->exec())
	{
		templateName = dia->Answer->text();
		while (currentDoc->MasterNames.contains(templateName) || (templateName == "Normal"))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			templateName = dia->Answer->text();
		}
		nr = currentDoc->Pages.count();
		currentDoc->MasterNames.insert(templateName, nr);
		currentDoc->PageC = 0;
		atf = currentDoc->PageAT;
		currentDoc->PageAT = false;
		emit createNew(nr);
		if (currentDoc->PageFP)
			currentDoc->Pages.at(nr)->LeftPg = dia->Links->currentItem() == 0 ? true : false;
		currentDoc->Pages.at(nr)->setPageName(templateName);
		currentDoc->Pages.at(nr)->MPageNam = "";
		updateTemplateList(templateName);
		currentDoc->PageAT = atf;
		currentView->ShowTemplate(currentDoc->MasterNames[templateName]);
		currentDoc->MasterPages = currentDoc->Pages;
		emit docAltered(currentDoc);
	}
	delete dia;
}

void MusterPages::appendPage()
{
	QString templateName, templateName2;
	int nr;
	bool atf;
	MergeDoc *dia = new MergeDoc(this, true);
	if (dia->exec())
	{
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		nr = currentDoc->Pages.count();
		currentDoc->PageC = 0;
		atf = currentDoc->PageAT;
		currentDoc->PageAT = false;
		emit createNew(nr);
		qApp->processEvents();
		emit loadPage(dia->getFromDoc(), dia->getMasterPageNameItem(), true);
		qApp->processEvents();
		templateName = currentDoc->Pages.at(nr)->PageNam;
		templateName2 = templateName;
		int copyC = 1;
		while (currentDoc->MasterNames.contains(templateName2))
		{
			templateName2 = tr("Copy #%1 of ").arg(copyC)+templateName;
			copyC++;
		}
		currentDoc->MasterNames.insert(templateName2, nr);
		currentDoc->Pages.at(nr)->setPageName(templateName2);
		currentDoc->Pages.at(nr)->MPageNam = "";
		updateTemplateList(templateName2);
		currentDoc->PageAT = atf;
		currentView->ShowTemplate(currentDoc->MasterNames[templateName2]);
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		currentDoc->MasterPages = currentDoc->Pages;
		emit docAltered(currentDoc);
	}
	delete dia;
}

void MusterPages::selectTemplate(QListBoxItem *item)
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
	currentView->ShowTemplate(currentDoc->MasterNames[sMuster]);
}

void MusterPages::selectTemplate(QString name)
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
	currentView->ShowTemplate(currentDoc->MasterNames[sMuster]);
}

void MusterPages::updateTemplateList(QString templateName)
{
	templateData->clear();
	for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
		templateData->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());
	deleteButton->setEnabled(currentDoc->MasterNames.count() == 1 ? false : true);
	if (templateName == "Normal")
	{
		templateName = tr("Normal");
		deleteButton->setEnabled(false);
	}
	templateData->setSelected(templateData->index(templateData->findItem(templateName)), true);
}

