#include "align.h"
#include "align.moc"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

Align::Align( QWidget* parent, int anz, int ein)
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
    ButtonGroup1 = new QButtonGroup( this, "ButtonGroup1" );
    ButtonGroup1->setTitle( tr( "Horizontal" ) );
    ButtonGroup1->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1->layout()->setSpacing( 0 );
    ButtonGroup1->layout()->setMargin( 0 );
    ButtonGroup1Layout = new QGridLayout( ButtonGroup1->layout() );
    ButtonGroup1Layout->setAlignment( Qt::AlignTop );
    ButtonGroup1Layout->setSpacing( 5 );
    ButtonGroup1Layout->setMargin( 10 );
    TextLabel1 = new QLabel( ButtonGroup1, "TextLabel1" );
    TextLabel1->setEnabled( true );
    TextLabel1->setText( tr( "between:" ) );
    ButtonGroup1Layout->addWidget( TextLabel1, 0, 0 );
    VartH = new QComboBox( true, ButtonGroup1, "VartH" );
    VartH->setEnabled( true );
    VartH->setMinimumSize( QSize( 100, 22 ) );
    VartH->insertItem( tr("Left Sides"));
    VartH->insertItem( tr("Middles"));
    VartH->insertItem( tr("Right Sides"));
    VartH->setEditable(false);
		ButtonGroup1Layout->addMultiCellWidget( VartH, 0, 0, 1, 2 );
    NichtsH = new QRadioButton( ButtonGroup1, "NichtsH" );
    NichtsH->setText( tr( "Don't change" ) );
    NichtsH->setChecked(true);
    ButtonGroup1Layout->addMultiCellWidget( NichtsH, 1, 1, 0, 2 );
    CheckH = new QRadioButton( ButtonGroup1, "CheckH" );
    CheckH->setText( tr( "Align" ) );
    ButtonGroup1Layout->addMultiCellWidget( CheckH, 2, 2, 0, 2 );
    TextLabelD = new QLabel( ButtonGroup1, "TextLabelD" );
    TextLabelD->setText( tr( "Displacement" ) );
    TextLabelD->setEnabled(false);
    ButtonGroup1Layout->addWidget( TextLabelD, 3, 1);
    AHor = new MSpinBox( ButtonGroup1, 2 );
    AHor->setEnabled( true );
    AHor->setMinimumSize( QSize( 80, 20 ) );
    AHor->setMaxValue( 1000 );
    AHor->setMinValue( -1000 );
    AHor->setValue( 0 );
    AHor->setEnabled(false);
    ButtonGroup1Layout->addWidget( AHor, 3, 2, Qt::AlignRight);
    VerteilenH = new QRadioButton( ButtonGroup1, "VerteilenH" );
    VerteilenH->setEnabled( true );
    VerteilenH->setText( tr( "Distribute evenly" ) );
    if (Anzahl < 3)
    	VerteilenH->setEnabled(false);
    ButtonGroup1Layout->addMultiCellWidget( VerteilenH, 4, 4, 0, 2 );
    Layout4->addWidget( ButtonGroup1 );

    ButtonGroup1_2 = new QButtonGroup( this, "ButtonGroup1" );
    ButtonGroup1_2->setTitle( tr( "Vertical" ) );
    ButtonGroup1_2->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1_2->layout()->setSpacing( 0 );
    ButtonGroup1_2->layout()->setMargin( 0 );
    ButtonGroup1_2Layout = new QGridLayout( ButtonGroup1_2->layout() );
    ButtonGroup1_2Layout->setAlignment( Qt::AlignTop );
    ButtonGroup1_2Layout->setSpacing( 5 );
    ButtonGroup1_2Layout->setMargin( 10 );
    TextLabel1_2 = new QLabel( ButtonGroup1_2, "TextLabel1" );
    TextLabel1_2->setEnabled( true );
    TextLabel1_2->setText( tr( "between:" ) );
    ButtonGroup1_2Layout->addWidget( TextLabel1_2, 0, 0 );
    VartV = new QComboBox( true, ButtonGroup1_2, "VartH" );
    VartV->setEnabled( true );
    VartV->setMinimumSize( QSize( 100, 22 ) );
    VartV->insertItem( tr("Top Sides"));
    VartV->insertItem( tr("Middles"));
    VartV->insertItem( tr("Bottom Sides"));
    VartV->setEditable(false);
		ButtonGroup1_2Layout->addMultiCellWidget( VartV, 0, 0, 1, 2 );
    NichtsV = new QRadioButton( ButtonGroup1_2, "NichtsH" );
    NichtsV->setText( tr( "Don't change" ) );
    NichtsV->setChecked(true);
    ButtonGroup1_2Layout->addMultiCellWidget( NichtsV, 1, 1, 0, 2 );
    CheckV = new QRadioButton( ButtonGroup1_2, "CheckH" );
    CheckV->setText( tr( "Align" ) );
    ButtonGroup1_2Layout->addMultiCellWidget( CheckV, 2, 2, 0, 2 );
    TextLabelD2 = new QLabel( ButtonGroup1_2, "TextLabelD" );
    TextLabelD2->setText( tr( "Displacement" ) );
    TextLabelD2->setEnabled(false);
    ButtonGroup1_2Layout->addWidget( TextLabelD2, 3, 1 );
    AVert = new MSpinBox( ButtonGroup1_2, 2 );
    AVert->setEnabled( true );
    AVert->setMinimumSize( QSize( 80, 20 ) );
		switch (ein)
			{
			case 0:
    		AHor->setSuffix( tr( " pts" ) );
    		AVert->setSuffix( tr( " pts" ) );
				break;
			case 1:
    		AHor->setSuffix( tr( " mm" ) );
    		AVert->setSuffix( tr( " mm" ) );
				break;
			case 2:
    		AHor->setSuffix( tr( " in" ) );
    		AVert->setSuffix( tr( " in" ) );
				break;
			case 3:
    		AHor->setSuffix( tr( " p" ) );
    		AVert->setSuffix( tr( " p" ) );
				break;
			}
    AVert->setMaxValue( 1000 );
    AVert->setMinValue( -1000 );
    AVert->setValue( 0 );
    AVert->setEnabled(false);
    ButtonGroup1_2Layout->addWidget( AVert, 3, 2, Qt::AlignRight);
    VerteilenV = new QRadioButton( ButtonGroup1_2, "VerteilenH" );
    VerteilenV->setText( tr( "Distribute evenly" ) );
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
    OKbutton = new QPushButton( this, "OKbutton" );
    OKbutton->setText( tr( "OK" ) );
    OKbutton->setDefault( true );
    Layout3->addWidget( OKbutton );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer_2 );
    ButtonApply = new QPushButton( this, "ButtonApply" );
    ButtonApply->setText( tr( "Apply" ) );
    Layout3->addWidget( ButtonApply );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer_3 );
    CancelB = new QPushButton( this, "CancelB" );
    CancelB->setText( tr( "Cancel" ) );
    Layout3->addWidget( CancelB );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer_4 );
    AlignLayout->addLayout( Layout3 );
    setMinimumSize(sizeHint());

    // signals and slots connections
    connect( OKbutton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
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

void Align::DistHoriz()
{
	bool setter = false;
  if (CheckH->isChecked())
  	setter = true;
	AHor->setEnabled( setter );
  TextLabelD->setEnabled( setter );

}

void Align::DistVert()
{
	bool setter = false;
  if (CheckV->isChecked())
  	setter = true;
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
	emit ApplyDist(xa, ya, Vth, Vtv, xdp, ydp, xart, yart);
}
