#include "lineformats.h"
#include "lineformats.moc"
#include "multiline.h"
#include <qmessagebox.h>
extern QPixmap loadIcon(QString nam);

LineFormate::LineFormate( QWidget* parent, ScribusDoc *doc)
    : QDialog( parent, "Formate", true, 0)
{
    resize( 327, 260 );
    setCaption( tr( "Edit Line Styles" ) );
  	setIcon(loadIcon("AppIcon.xpm"));
		Docu = doc;
		TempStyles = doc->MLineStyles;
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
		if (ListBox1->count() > 0)
    	sFnumber = TempStyles.begin().key();
		else
			sFnumber = "";
    UpdateFList();
}

void LineFormate::selFormat(QListBoxItem *c)
{
	sFnumber = c->text();
	EditB->setEnabled(true);
	DublicateB->setEnabled(true);
	DeleteB->setEnabled(true);
}

void LineFormate::dupFormat()
{
	multiLine ml;
	ml = TempStyles[sFnumber];
	MultiLine* dia = new MultiLine(this, Docu, ml, tr("Copy of ")+sFnumber, true, &TempStyles);
	if (dia->exec())
		{
		TempStyles.insert(dia->SName->text(), dia->TempVorl);
		sFnumber = dia->SName->text();
		}
	UpdateFList();
}

void LineFormate::neuesFormat()
{
	struct singleLine sl;
	sl.Color = "Black";
	sl.Shade = 100;
	sl.Dash = SolidLine;
	sl.LineEnd = FlatCap;
	sl.LineJoin = MiterJoin;
	sl.Width = 1.0;
	multiLine ml;
	ml.push_back(sl);
	MultiLine* dia = new MultiLine(this, Docu, ml, tr("New Style"), true, &TempStyles);
	if (dia->exec())
		{
		TempStyles.insert(dia->SName->text(), dia->TempVorl);
		sFnumber = dia->SName->text();
		}
	UpdateFList();
}

void LineFormate::editFormat()
{
	MultiLine* dia = new MultiLine(this, Docu, TempStyles[sFnumber], sFnumber, false, &TempStyles);
	if (dia->exec())
		TempStyles[sFnumber] = dia->TempVorl;
	delete dia;
	UpdateFList();
}

void LineFormate::deleteFormat()
{
  int exit=QMessageBox::warning(this,
  															tr("Warning"),
                                tr("Do you really want do delete this Style?"),
                                tr("No"),
                                tr("Yes"),
                                0, 0, 1);
	if (exit == 1)
  	{
		TempStyles.remove(sFnumber);
		UpdateFList();
		}
}

void LineFormate::UpdateFList()
{
	disconnect(ListBox1, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selFormat(QListBoxItem*)));
	ListBox1->clear();
	QMap<QString,multiLine>::Iterator it;
	for (it = TempStyles.begin(); it != TempStyles.end(); ++it)
		{
		ListBox1->insertItem(it.key());
		}
	if (ListBox1->count() > 0)
		ListBox1->setSelected(ListBox1->findItem(sFnumber), true);
	bool setter = ListBox1->count() == 0 ? true : false;
	if (setter == false)
		sFnumber = "";
  DublicateB->setEnabled(setter);
  EditB->setEnabled(setter);
  DeleteB->setEnabled(setter);
	connect(ListBox1, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selFormat(QListBoxItem*)));
}
