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

	LoadM = new QPushButton( this, "LoadF" );
	LoadM->setText( tr( "Append" ) );
	Layout2->addWidget( LoadM );

	NewB = new QPushButton( this, "NewB" );
	NewB->setText( tr( "New" ) );
	Layout2->addWidget( NewB );

	DuplicateB = new QPushButton( this, "DublicateB" );
	DuplicateB->setText( tr( "Duplicate" ) );
	Layout2->addWidget( DuplicateB );

	DeleteB = new QPushButton( this, "DeleteB" );
	DeleteB->setText( tr( "Delete" ) );
	Layout2->addWidget( DeleteB );

	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout2->addItem( spacer );

	ExitB = new QPushButton( this, "ExitB" );
	ExitB->setText( tr( "Close" ) );
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
		View->ShowTemplate(View->MasterNames[sMuster]);
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
	                              tr("Do you really want do delete this Template?"),
	                              tr("No"),
	                              tr("Yes"),
	                              0, QMessageBox::No, QMessageBox::Yes);
	if (exit == 1)
	{
		Doc->PageC = View->Pages.count();
		View->delPage(View->MasterNames[sMuster]);
		View->reformPages();
		View->MasterNames.clear();
		for (uint a=0; a<View->Pages.count(); ++a)
			View->MasterNames[View->Pages.at(a)->PageNam] = View->Pages.at(a)->PageNr;
		for (uint b=0; b<View->DocPages.count(); ++b)
		{
			if (View->DocPages.at(b)->MPageNam == sMuster)
				View->DocPages.at(b)->MPageNam = "Normal";
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
	NewTm *dia = new NewTm(this, tr("Name:"), tr("New Template"), Doc);
	dia->Answer->setText( tr("Copy of %1").arg(sMuster));
	dia->Answer->selectAll();
	if (dia->exec())
	{
		nam = dia->Answer->text();
		while (View->MasterNames.contains(nam) || (nam == "Normal"))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			nam = dia->Answer->text();
		}
		nr = View->Pages.count();
		View->MasterNames.insert(nam, nr);
		for (uint a=0; a<View->Pages.count(); ++a)
			View->Pages.at(a)->parentWidget()->hide();
		Doc->PageC = 0;
		atf = Doc->PageAT;
		Doc->PageAT = false;
		emit CreateNew(nr);
		if (Doc->PageFP)
			View->Pages.at(nr)->LeftPg = dia->Links->currentItem() == 0 ? true : false;
		int inde = View->MasterNames[sMuster];
		QMap<int,int> TableID;
		QPtrList<PageItem> TableItems;
		TableID.clear();
		TableItems.clear();
		for (uint a=0; a<View->Pages.at(inde)->Items.count(); ++a)
		{
			CopyPageItem(&Buffer, View->Pages.at(inde)->Items.at(a));
			Doc->ActPage->PasteItem(&Buffer, true, true);
			PageItem* Neu = Doc->ActPage->Items.at(Doc->ActPage->Items.count()-1);
			if (Neu->isTableItem)
			{
				TableItems.append(Neu);
				TableID.insert(a, Neu->ItemNr);
			}
		}
		if (TableItems.count() != 0)
		{
			for (uint ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->TopLink = Doc->ActPage->Items.at(TableID[ta->TopLinkID]);
				else
					ta->TopLink = 0;
				if (ta->LeftLinkID != -1)
					ta->LeftLink = Doc->ActPage->Items.at(TableID[ta->LeftLinkID]);
				else
					ta->LeftLink = 0;
				if (ta->RightLinkID != -1)
					ta->RightLink = Doc->ActPage->Items.at(TableID[ta->RightLinkID]);
				else
					ta->RightLink = 0;
				if (ta->BottomLinkID != -1)
					ta->BottomLink = Doc->ActPage->Items.at(TableID[ta->BottomLinkID]);
				else
					ta->BottomLink = 0;
			}
		}
		Doc->ActPage->Deselect(true);
		View->DrawNew();
		View->Pages.at(nr)->PageNam = nam;
		View->Pages.at(nr)->MPageNam = "";
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
		while (View->MasterNames.contains(nam) || (nam == "Normal"))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			nam = dia->Answer->text();
		}
		nr = View->Pages.count();
		View->MasterNames.insert(nam, nr);
		for (uint a=0; a<View->Pages.count(); ++a)
			View->Pages.at(a)->parentWidget()->hide();
		Doc->PageC = 0;
		atf = Doc->PageAT;
		Doc->PageAT = false;
		emit CreateNew(nr);
		if (Doc->PageFP)
			View->Pages.at(nr)->LeftPg = dia->Links->currentItem() == 0 ? true : false;
		View->Pages.at(nr)->PageNam = nam;
		View->Pages.at(nr)->MPageNam = "";
		updateMList(nam);
		Doc->PageAT = atf;
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
		nr = View->Pages.count();
		for (uint a=0; a<View->Pages.count(); ++a)
			View->Pages.at(a)->parentWidget()->hide();
		Doc->PageC = 0;
		atf = Doc->PageAT;
		Doc->PageAT = false;
		emit CreateNew(nr);
		qApp->processEvents();
		emit LoadPage(dia->Filename->text(), dia->PageNa->currentItem(), true);
		qApp->processEvents();
		nam = View->Pages.at(nr)->PageNam;
		nam2 = nam;
		int copyC = 1;
		while (View->MasterNames.contains(nam2))
		{
			nam2 = tr("Copy #%1 of ").arg(copyC)+nam;
			copyC++;
		}
		View->MasterNames.insert(nam2, nr);
		View->Pages.at(nr)->PageNam = nam2;
		View->Pages.at(nr)->MPageNam = "";
		View->DrawNew();
		updateMList(nam2);
		Doc->PageAT = atf;
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	delete dia;
}

void MusterSeiten::selTemplate(QListBoxItem *c)
{
	sMuster = c->text();
	DeleteB->setEnabled(View->MasterNames.count() == 1 ? false : true);
	if (sMuster == tr("Normal"))
	{
		sMuster = "Normal";
		DeleteB->setEnabled(false);
	}
	else
		DeleteB->setEnabled(true);
	View->ShowTemplate(View->MasterNames[sMuster]);
}

void MusterSeiten::updateMList(QString nam)
{
	ListBox1->clear();
	QMap<QString,int>::Iterator it;
	for (it = View->MasterNames.begin(); it != View->MasterNames.end(); ++it)
		ListBox1->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());
	DeleteB->setEnabled(View->MasterNames.count() == 1 ? false : true);
	if (nam == "Normal")
	{
		nam = tr("Normal");
		DeleteB->setEnabled(false);
	}
	ListBox1->setSelected(ListBox1->index(ListBox1->findItem(nam)), true);
}

