#include "align.h"
#include "align.moc"
#include "page.h"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

Align::Align( QWidget* parent, int anz, int ein, ScribusDoc* docc)
	: QDialog( parent, "al", true, 0 )
{
	Anzahl = anz;
	Anzahl > 2 ? setCaption( tr("Distribute/Align")) : setCaption( tr("Align"));
	setIcon(loadIcon("AppIcon.png"));
	AlignLayout = new QVBoxLayout( this );
	AlignLayout->setSpacing( 0 );
	AlignLayout->setMargin( 0 );
	Layout4 = new QHBoxLayout;
	Layout4->setSpacing( 10 );
	Layout4->setMargin( 10 );
	ButtonGroup1 = new QButtonGroup( tr( "Horizontal" ), this, "ButtonGroup1" );
	ButtonGroup1->setColumnLayout(0, Qt::Vertical );
	ButtonGroup1->layout()->setSpacing( 0 );
	ButtonGroup1->layout()->setMargin( 0 );
	ButtonGroup1Layout = new QGridLayout( ButtonGroup1->layout() );
	ButtonGroup1Layout->setAlignment( Qt::AlignTop );
	ButtonGroup1Layout->setSpacing( 5 );
	ButtonGroup1Layout->setMargin( 10 );
	VartH = new QComboBox( true, ButtonGroup1, "VartH" );
	VartH->setEnabled( true );
	VartH->setMinimumSize( QSize( 100, 22 ) );
	/* PFJ - 28/2/04 - altered the single line inserts for the faster way I've
					used before. sides is used later on in this method */
	QString sides[] = { tr("Left Sides"), tr("Middles"), tr("Right Sides")};
	size_t tempSides = sizeof(sides) / sizeof(*sides);
	for (uint propogate = 0; propogate < tempSides; ++propogate)
		VartH->insertItem(sides[propogate]);
	VartH->setEditable(false);
	TextLabel1 = new QLabel( VartH, tr( "&Between:" ), ButtonGroup1, "TextLabel1" );
	TextLabel1->setEnabled( true );
	ButtonGroup1Layout->addWidget( TextLabel1, 0, 0 );
	ButtonGroup1Layout->addMultiCellWidget( VartH, 0, 0, 1, 2 );
	NichtsH = new QRadioButton( tr("&Do Not Change"), ButtonGroup1, "NichtsH" );
	NichtsH->setChecked(true);
	ButtonGroup1Layout->addMultiCellWidget( NichtsH, 1, 1, 0, 2 );
	CheckH = new QRadioButton( tr( "A&lign" ), ButtonGroup1, "CheckH" );
	ButtonGroup1Layout->addMultiCellWidget( CheckH, 2, 2, 0, 2 );
	TextLabelD = new QLabel( tr( "Di&splacement" ), ButtonGroup1, "TextLabelD" );
	TextLabelD->setEnabled(false);
	ButtonGroup1Layout->addWidget( TextLabelD, 3, 1);
	AHor = new MSpinBox( -1000, 1000, ButtonGroup1, 2 );
	AHor->setMinimumSize( QSize( 80, 20 ) );
	AHor->setValue( 0 );
	AHor->setEnabled(false);
	TextLabelD->setBuddy(AHor);
	ButtonGroup1Layout->addWidget( AHor, 3, 2, Qt::AlignRight);
	VerteilenH = new QRadioButton( tr( "Distribute &Evenly" ), ButtonGroup1, "VerteilenH" );
	if (Anzahl < 3)
		VerteilenH->setEnabled(false);
	ButtonGroup1Layout->addMultiCellWidget( VerteilenH, 4, 4, 0, 2 );
	Layout4->addWidget( ButtonGroup1 );

	ButtonGroup1_2 = new QButtonGroup( tr( "Vertical" ), this, "ButtonGroup1" );
	ButtonGroup1_2->setColumnLayout(0, Qt::Vertical );
	ButtonGroup1_2->layout()->setSpacing( 0 );
	ButtonGroup1_2->layout()->setMargin( 0 );
	ButtonGroup1_2Layout = new QGridLayout( ButtonGroup1_2->layout() );
	ButtonGroup1_2Layout->setAlignment( Qt::AlignTop );
	ButtonGroup1_2Layout->setSpacing( 5 );
	ButtonGroup1_2Layout->setMargin( 10 );
	VartV = new QComboBox( true, ButtonGroup1_2, "VartH" );
	VartV->setEnabled( true );
	VartV->setMinimumSize( QSize( 100, 22 ) );
	VartV->setEditable(false);
	VartV->insertItem(tr("Top Sides"));
	VartV->insertItem(tr("Middles"));
	VartV->insertItem(tr("Bottom Sides"));
	TextLabel1_2 = new QLabel( VartV, tr( "Bet&ween:" ), ButtonGroup1_2, "TextLabel1" );
	TextLabel1_2->setEnabled( true );
	ButtonGroup1_2Layout->addWidget( TextLabel1_2, 0, 0 );
	ButtonGroup1_2Layout->addMultiCellWidget( VartV, 0, 0, 1, 2 );
	NichtsV = new QRadioButton( tr( "Do &Not Change" ), ButtonGroup1_2, "NichtsH" );
	NichtsV->setChecked(true);
	ButtonGroup1_2Layout->addMultiCellWidget( NichtsV, 1, 1, 0, 2 );
	CheckV = new QRadioButton( tr( "Al&ign" ), ButtonGroup1_2, "CheckH" );
	ButtonGroup1_2Layout->addMultiCellWidget( CheckV, 2, 2, 0, 2 );
	TextLabelD2 = new QLabel( tr( "Dis&placement" ), ButtonGroup1_2, "TextLabelD" );
	TextLabelD2->setEnabled(false);
	ButtonGroup1_2Layout->addWidget( TextLabelD2, 3, 1 );
	AVert = new MSpinBox( -1000, 1000, ButtonGroup1_2, 2 );
	AVert->setMinimumSize( QSize( 80, 20 ) );
	AVert->setValue( 0 );
	AVert->setEnabled(false);
	TextLabelD2->setBuddy(AVert);

	QString tmp = (ein == 0) ? tr(" pt") :
				(ein == 1) ? tr(" mm") :
				(ein == 2) ? tr(" in") : tr(" p");
	AHor->setSuffix(tmp);
	AVert->setSuffix(tmp);

	ButtonGroup1_2Layout->addWidget( AVert, 3, 2, Qt::AlignRight);
	VerteilenV = new QRadioButton( tr( "Distribute E&venly" ), ButtonGroup1_2, "VerteilenH" );
	if (Anzahl < 3)
		VerteilenV->setEnabled(false);
	ButtonGroup1_2Layout->addMultiCellWidget( VerteilenV, 4, 4, 0, 2 );
	Layout4->addWidget( ButtonGroup1_2 );
	AlignLayout->addLayout( Layout4 );

	Layout3 = new QHBoxLayout;
	Layout3->setSpacing( 10 );
	Layout3->setMargin( 10 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout3->addItem( spacer );
	OKbutton = new QPushButton( tr( "&OK" ), this, "OKbutton" );
	OKbutton->setDefault( true );
	Layout3->addWidget( OKbutton );
	ButtonApply = new QPushButton( tr( "&Apply" ), this, "ButtonApply" );
	Layout3->addWidget( ButtonApply );
	CancelB = new QPushButton( tr( "&Cancel" ), this, "CancelB" );
	Layout3->addWidget( CancelB );
	AlignLayout->addLayout( Layout3 );
	setMinimumSize(sizeHint());
	struct ItemPos po;
	doc = docc;
	for (uint x=0; x<doc->ActPage->SelItem.count(); ++x)
	{
		po.Nr = doc->ActPage->SelItem.at(x)->ItemNr;
		po.x = doc->ActPage->SelItem.at(x)->Xpos;
		po.y = doc->ActPage->SelItem.at(x)->Ypos;
		Backup.append(po);
	}

	// signals and slots connections
	connect( OKbutton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( CancelB, SIGNAL( clicked() ), this, SLOT( cancel() ) );
	connect(ButtonApply, SIGNAL(clicked()), this, SLOT( slotApplyDiag()));
	connect( ButtonGroup1, SIGNAL( clicked(int) ), this, SLOT( DistHoriz() ) );
	connect( ButtonGroup1_2, SIGNAL( clicked(int) ), this, SLOT( DistVert() ) );
}

/*
*  Destroys the object and frees any allocated resources
*/
Align::~Align()
{
	// no need to delete child widgets, Qt does it all for us
}

void Align::cancel()
{
	for (uint x=0; x < Backup.count(); ++x)
	{
		doc->ActPage->Items.at(Backup[x].Nr)->Xpos = Backup[x].x;
		doc->ActPage->Items.at(Backup[x].Nr)->Ypos = Backup[x].y;
		doc->ActPage->update();
	}
	reject();
}

void Align::DistHoriz()
{
	bool setter = CheckH->isChecked() ? true : false;
	AHor->setEnabled( setter );
	TextLabelD->setEnabled( setter );
}

void Align::DistVert()
{
	bool setter = CheckV->isChecked() ? true : false;
	AVert->setEnabled( setter );
	TextLabelD2->setEnabled( setter );
}

void Align::slotApplyDiag()
{
	double xdp = AHor->value() / UmReFaktor;
	bool xa = (CheckH->isChecked() || VerteilenH->isChecked());
	double ydp = AVert->value() / UmReFaktor;
	bool ya = (CheckV->isChecked() || VerteilenV->isChecked());
	int xart = VartH->currentItem();
	int yart = VartV->currentItem();
	bool Vth = VerteilenH->isChecked();
	bool Vtv = VerteilenV->isChecked();
	if (NichtsV->isChecked() && NichtsH->isChecked())
	{
		for (uint x=0; x < Backup.count(); ++x)
		{
			doc->ActPage->Items.at(Backup[x].Nr)->Xpos = Backup[x].x;
			doc->ActPage->Items.at(Backup[x].Nr)->Ypos = Backup[x].y;
			doc->ActPage->update();
		}
	}
	else
		emit ApplyDist(xa, ya, Vth, Vtv, xdp, ydp, xart, yart);
}
