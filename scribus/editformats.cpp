#include "editformats.h"
#include "editformats.moc"
#include "edit1format.h"
#include <qmessagebox.h>
extern QPixmap loadIcon(QString nam);

StilFormate::StilFormate( QWidget* parent, ScribusDoc *doc, preV *avail)
    : QDialog( parent, "Formate", true, 0)
{
    resize( 327, 260 );
    setCaption( tr( "Edit Styles" ) );
  	setIcon(loadIcon("AppIcon.png"));
		fon = avail;
		Docu = doc;
    StilFormateLayout = new QHBoxLayout( this ); 
    StilFormateLayout->setSpacing( 5 );
    StilFormateLayout->setMargin( 10 );

    ListBox1 = new QListBox( this, "ListBox1" );
    ListBox1->setMinimumSize( QSize( 200, 240 ) );
    StilFormateLayout->addWidget( ListBox1 );

    Layout15 = new QVBoxLayout; 
    Layout15->setSpacing( 6 );
    Layout15->setMargin( 0 );

    NewB = new QPushButton( this, "NewB" );
    NewB->setText( tr( "New" ) );
    Layout15->addWidget( NewB );

    EditB = new QPushButton( this, "EditB" );
    EditB->setText( tr( "Edit" ) );
    EditB->setDefault( true );
    EditB->setEnabled(false);
    Layout15->addWidget( EditB );

    DublicateB = new QPushButton( this, "DublicateB" );
    DublicateB->setText( tr( "Duplicate" ) );
    DublicateB->setEnabled(false);
    Layout15->addWidget( DublicateB );

    DeleteB = new QPushButton( this, "DeleteB" );
    DeleteB->setText( tr( "Delete" ) );
    DeleteB->setEnabled(false);
    Layout15->addWidget( DeleteB );

    SaveB = new QPushButton( this, "SaveB" );
    SaveB->setText( tr( "Save" ) );
    Layout15->addWidget( SaveB );

    CancelB = new QPushButton( this, "CancelB" );
    CancelB->setText( tr( "Cancel" ) );
    Layout15->addWidget( CancelB );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout15->addItem( spacer );
    StilFormateLayout->addLayout( Layout15 );

    // signals and slots connections
    connect(CancelB, SIGNAL(clicked()), this, SLOT(reject()));
    connect(SaveB, SIGNAL(clicked()), this, SLOT(accept()));
    connect(EditB, SIGNAL(clicked()), this, SLOT(editFormat()));
    connect(NewB, SIGNAL(clicked()), this, SLOT(neuesFormat()));
    connect(DublicateB, SIGNAL(clicked()), this, SLOT(dupFormat()));
    connect(DeleteB, SIGNAL(clicked()), this, SLOT(deleteFormat()));
    connect(ListBox1, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selFormat(QListBoxItem*)));
    TempVorl = doc->Vorlagen;
    UpdateFList();
}

void StilFormate::selFormat(QListBoxItem *c)
{
    sFnumber = c->listBox()->currentItem() + 5;
    EditB->setEnabled(true);
    DublicateB->setEnabled(true);
    DeleteB->setEnabled(true);
}

void StilFormate::dupFormat()
{
   	struct StVorL sty;
   	sty.Vname = tr("Copy of ") + TempVorl[sFnumber].Vname;
   	sty.LineSpa = TempVorl[sFnumber].LineSpa;
   	sty.Ausri = TempVorl[sFnumber].Ausri;
   	sty.Indent = TempVorl[sFnumber].Indent;
   	sty.First = TempVorl[sFnumber].First;
   	sty.Avor = TempVorl[sFnumber].Avor;
   	sty.Anach = TempVorl[sFnumber].Anach;
   	sty.Font = TempVorl[sFnumber].Font;
   	sty.FontSize = TempVorl[sFnumber].FontSize;
   	TempVorl.append(sty);
   	sFnumber = TempVorl.count()-1;
		EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true, fon, static_cast<double>(Docu->AutoLine));
		if (!dia2->exec())
			TempVorl.remove(TempVorl.fromLast());
   	delete dia2;
		UpdateFList();
}

void StilFormate::neuesFormat()
{
   	struct StVorL sty;
   	sty.Vname = tr("New Style");
   	sty.LineSpa = (Docu->Dsize * static_cast<double>(Docu->AutoLine) / 100) + Docu->Dsize;
   	sty.Ausri = 0;
   	sty.Indent = 0;
   	sty.First = 0;
   	sty.Avor = 0;
   	sty.Anach = 0;
		sty.Font = Docu->Dfont;
		sty.FontSize = Docu->Dsize;
   	TempVorl.append(sty);
   	sFnumber = TempVorl.count()-1;
		EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true, fon, static_cast<double>(Docu->AutoLine));
		if (!dia2->exec())
			TempVorl.remove(TempVorl.fromLast());
   	delete dia2;
		UpdateFList();
}

void StilFormate::editFormat()
{
	EditStyle* dia = new EditStyle(this, &TempVorl[sFnumber], TempVorl, false, fon, static_cast<double>(Docu->AutoLine));
	dia->exec();
	delete dia;
	UpdateFList();
}

void StilFormate::deleteFormat()
{
  int exit=QMessageBox::warning(this,
  															tr("Warning"),
                                tr("Do you really want do delete this Style?"),
                                tr("No"),
                                tr("Yes"),
                                0, 0, 1);
  if (exit == 1)
  	{
		ListBox1->removeItem(sFnumber);
		TempVorl.remove(TempVorl.at(sFnumber));
		UpdateFList();
		}
}

void StilFormate::UpdateFList()
{
	ListBox1->clear();
	if (TempVorl.count() < 6)
		return;
	for (uint x = 5; x < TempVorl.count(); ++x)
		{
		ListBox1->insertItem(TempVorl[x].Vname);
		}
	if (ListBox1->currentItem() == -1)
   	{
   	DublicateB->setEnabled(false);
   	EditB->setEnabled(false);
    DeleteB->setEnabled(false);
   	}
	ListBox1->setSelected(ListBox1->currentItem(), false);
}
