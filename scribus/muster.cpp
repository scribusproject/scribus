#include "muster.h"
#include "muster.moc"
#include "newtemp.h"
#include "mergedoc.h"
#include <qmessagebox.h>
#include <qcursor.h>

extern QPixmap loadIcon(QString nam);
extern void CopyPageItem(struct CLBuf *Buffer, PageItem *b);

MusterSeiten::MusterSeiten( QWidget* parent, ScribusDoc *doc, ScribusView *view, QString temp)
		: QDialog( parent, "Muster", false, WDestructiveClose)
{
	setCaption( tr( "Edit Templates" ) );
	setIcon(loadIcon("AppIcon.png"));
	Doc = doc;
	View = view;
	MusterSeitenLayout = new QHBoxLayout( this );
	MusterSeitenLayout->setSpacing( 6 );
	MusterSeitenLayout->setMargin( 10 );

	ListBox1 = new QListBox( this, "ListBox1" );
	ListBox1->setMinimumSize( QSize( 150, 240 ) );
	MusterSeitenLayout->addWidget( ListBox1 );

	Layout2 = new QVBoxLayout;
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 0 );

	LoadM = new QPushButton( tr( "&Append" ), this, "LoadF" );
	Layout2->addWidget( LoadM );

	NewB = new QPushButton( tr( "&New" ), this, "NewB" );
	Layout2->addWidget( NewB );

	DuplicateB = new QPushButton( tr( "D&uplicate" ), this, "DublicateB" );
	Layout2->addWidget( DuplicateB );

	DeleteB = new QPushButton( tr( "&Delete" ), this, "DeleteB" );
	Layout2->addWidget( DeleteB );

	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout2->addItem( spacer );

	ExitB = new QPushButton( tr( "&Close" ), this, "ExitB" );
	Layout2->addWidget( ExitB );
	MusterSeitenLayout->addLayout( Layout2 );
	if (temp == "")
	{
		sMuster = "Normal";
		updateMList(sMuster);
		View->ShowTemplate(0);
	}
	else
	{
		sMuster = temp;
		updateMList(sMuster);
		View->ShowTemplate(Doc->MasterNames[sMuster]);
	}
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect(ExitB, SIGNAL(clicked()), this, SLOT(ExitEditor()));
	connect(DuplicateB, SIGNAL(clicked()), this, SLOT(DuplTemp()));
	connect(DeleteB, SIGNAL(clicked()), this, SLOT(DelTemp()));
	connect(NewB, SIGNAL(clicked()), this, SLOT(NewTemp()));
	connect(LoadM, SIGNAL(clicked()), this, SLOT(loadMpage()));
	connect(ListBox1, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selTemplate(QListBoxItem*)));
}

void MusterSeiten::closeEvent(QCloseEvent *ce)
{
	emit Fertig();
	ce->accept();
}

void MusterSeiten::ExitEditor()
{
	close();
}

void MusterSeiten::DelTemp()
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
		Doc->PageC = Doc->Pages.count();
		View->delPage(Doc->MasterNames[sMuster]);
		View->reformPages();
		Doc->MasterNames.clear();
		for (uint a = 0; a < Doc->Pages.count(); ++a)
			Doc->MasterNames[Doc->Pages.at(a)->PageNam] = Doc->Pages.at(a)->PageNr;
		for (uint b = 0; b < Doc->DocPages.count(); ++b)
		{
			if (Doc->DocPages.at(b)->MPageNam == sMuster)
				Doc->DocPages.at(b)->MPageNam = "Normal";
		}
		Doc->PageC = 1;
		sMuster = "Normal";
		updateMList(sMuster);
	}
}

void MusterSeiten::DuplTemp()
{
	QString nam;
	int nr;
	bool atf;
	struct CLBuf Buffer;
	NewTm *dia = new NewTm(this, tr("&Name:"), tr("New Template"), Doc);
	dia->Answer->setText( tr("Copy of %1").arg(sMuster));
	dia->Answer->selectAll();
	if (dia->exec())
	{
		nam = dia->Answer->text();
		while (Doc->MasterNames.contains(nam) || (nam == "Normal"))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			nam = dia->Answer->text();
		}
		nr = Doc->Pages.count();
		Doc->MasterNames.insert(nam, nr);
		Doc->PageC = 0;
		atf = Doc->PageAT;
		Doc->PageAT = false;
		emit CreateNew(nr);
		if (Doc->PageFP)
			Doc->Pages.at(nr)->LeftPg = dia->Links->currentItem() == 0 ? true : false;
		int inde = Doc->MasterNames[sMuster];
		QMap<int,int> TableID;
		QPtrList<PageItem> TableItems;
		TableID.clear();
		TableItems.clear();
		if (Doc->Pages.at(inde)->YGuides.count() != 0)
		{
			Doc->ActPage->YGuides.clear();
			for (uint y = 0; y < Doc->Pages.at(inde)->YGuides.count(); ++y)
			{
				Doc->ActPage->YGuides.append(Doc->Pages.at(inde)->YGuides[y]);
			}
			qHeapSort(Doc->ActPage->YGuides);
		}
		if (Doc->Pages.at(inde)->XGuides.count() != 0)
		{
			for (uint x = 0; x < Doc->Pages.at(inde)->XGuides.count(); ++x)
			{
				Doc->ActPage->XGuides.append(Doc->Pages.at(inde)->XGuides[x]);
			}
			qHeapSort(Doc->ActPage->XGuides);
		}
		uint end = Doc->Items.count();
		for (uint a = 0; a < end; ++a)
		{
			if (Doc->Items.at(a)->OwnPage == inde)
			{
				CopyPageItem(&Buffer, Doc->Items.at(a));
				View->PasteItem(&Buffer, true, true);
				PageItem* Neu = Doc->Items.at(Doc->Items.count()-1);
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
					ta->TopLink = Doc->Items.at(TableID[ta->TopLinkID]);
				else
					ta->TopLink = 0;
				if (ta->LeftLinkID != -1)
					ta->LeftLink = Doc->Items.at(TableID[ta->LeftLinkID]);
				else
					ta->LeftLink = 0;
				if (ta->RightLinkID != -1)
					ta->RightLink = Doc->Items.at(TableID[ta->RightLinkID]);
				else
					ta->RightLink = 0;
				if (ta->BottomLinkID != -1)
					ta->BottomLink = Doc->Items.at(TableID[ta->BottomLinkID]);
				else
					ta->BottomLink = 0;
			}
		}
		View->Deselect(true);
		View->DrawNew();
		Doc->Pages.at(nr)->PageNam = nam;
		Doc->Pages.at(nr)->MPageNam = "";
		updateMList(nam);
		Doc->PageAT = atf;
	}
	delete dia;
}

void MusterSeiten::NewTemp()
{
	QString nam;
	int nr;
	bool atf;
	NewTm *dia = new NewTm(this, tr("Name:"), tr("New Template"), Doc);
	dia->Answer->setText( tr("New Template"));
	dia->Answer->selectAll();
	if (dia->exec())
	{
		nam = dia->Answer->text();
		while (Doc->MasterNames.contains(nam) || (nam == "Normal"))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			nam = dia->Answer->text();
		}
		nr = Doc->Pages.count();
		Doc->MasterNames.insert(nam, nr);
		Doc->PageC = 0;
		atf = Doc->PageAT;
		Doc->PageAT = false;
		emit CreateNew(nr);
		if (Doc->PageFP)
			Doc->Pages.at(nr)->LeftPg = dia->Links->currentItem() == 0 ? true : false;
		Doc->Pages.at(nr)->PageNam = nam;
		Doc->Pages.at(nr)->MPageNam = "";
		updateMList(nam);
		Doc->PageAT = atf;
		View->ShowTemplate(Doc->MasterNames[nam]);
	}
	delete dia;
}

void MusterSeiten::loadMpage()
{
	QString nam, nam2;
	int nr;
	bool atf;
	MergeDoc *dia = new MergeDoc(this, true);
	if (dia->exec())
	{
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		nr = Doc->Pages.count();
		Doc->PageC = 0;
		atf = Doc->PageAT;
		Doc->PageAT = false;
		emit CreateNew(nr);
		qApp->processEvents();
		emit LoadPage(dia->getFromDoc(), dia->getMasterPageNameItem(), true);
		qApp->processEvents();
		nam = Doc->Pages.at(nr)->PageNam;
		nam2 = nam;
		int copyC = 1;
		while (Doc->MasterNames.contains(nam2))
		{
			nam2 = tr("Copy #%1 of ").arg(copyC)+nam;
			copyC++;
		}
		Doc->MasterNames.insert(nam2, nr);
		Doc->Pages.at(nr)->PageNam = nam2;
		Doc->Pages.at(nr)->MPageNam = "";
		updateMList(nam2);
		Doc->PageAT = atf;
		View->ShowTemplate(Doc->MasterNames[nam2]);
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	delete dia;
}

void MusterSeiten::selTemplate(QListBoxItem *c)
{
	sMuster = c->text();
	DeleteB->setEnabled(Doc->MasterNames.count() == 1 ? false : true);
	if (sMuster == tr("Normal"))
	{
		sMuster = "Normal";
		DeleteB->setEnabled(false);
	}
	else
		DeleteB->setEnabled(true);
	View->ShowTemplate(Doc->MasterNames[sMuster]);
}

void MusterSeiten::updateMList(QString nam)
{
	ListBox1->clear();
	QMap<QString,int>::Iterator it;
	for (it = Doc->MasterNames.begin(); it != Doc->MasterNames.end(); ++it)
		ListBox1->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());
	DeleteB->setEnabled(Doc->MasterNames.count() == 1 ? false : true);
	if (nam == "Normal")
	{
		nam = tr("Normal");
		DeleteB->setEnabled(false);
	}
	ListBox1->setSelected(ListBox1->index(ListBox1->findItem(nam)), true);
}

