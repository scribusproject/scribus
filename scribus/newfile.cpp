#include "newfile.h"
#include "newfile.moc"
extern QPixmap loadIcon(QString nam);

NewDoc::NewDoc( QWidget* parent, preV *Vor )
    : QDialog( parent, "newDoc", true, 0 )
{
		switch (Vor->Einheit)
			{
			case 0:
    		Umrech = 1.0;
    		ein = " pt";
				break;
			case 1:
				Umrech = 0.3527777;
    		ein = " mm";
				break;
			case 2:
				Umrech = 1.0 / 72.0;
    		ein = " in";
				break;
			case 3:
				Umrech = 1.0 / 12.0;
    		ein = " p";
				break;
			}
		einheit = Vor->Einheit;
		Orient = 0;
    setCaption( tr( "New Document" ) );
  	setIcon(loadIcon("AppIcon.png"));
    NewDocLayout = new QHBoxLayout( this, 10, 5, "NewDocLayout");
    Layout9 = new QVBoxLayout(0, 0, 5, "Layout9");

    ButtonGroup1_2 = new QButtonGroup(this, "ButtonGroup1_2" );
    ButtonGroup1_2->setTitle( tr( "Page Size" ));
    ButtonGroup1_2->setColumnLayout(0, Qt::Vertical);
    ButtonGroup1_2->layout()->setSpacing(6);
    ButtonGroup1_2->layout()->setMargin(10);
    ButtonGroup1_2Layout = new QVBoxLayout(ButtonGroup1_2->layout());
    ButtonGroup1_2Layout->setAlignment(Qt::AlignTop);
    Layout6 = new QGridLayout(0, 1, 1, 0, 6, "Layout6");
    TextLabel1 = new QLabel( ButtonGroup1_2, "TextLabel1" );
    TextLabel1->setText( tr( "Size:" ));
    Layout6->addWidget( TextLabel1, 0, 0 );
    ComboBox1 = new QComboBox( true, ButtonGroup1_2, "ComboBox1" );
    ComboBox1->insertItem("A0");
    ComboBox1->insertItem("A1");
    ComboBox1->insertItem("A2");
    ComboBox1->insertItem("A3");
    ComboBox1->insertItem("A4");
    ComboBox1->insertItem("A5");
    ComboBox1->insertItem("A6");
    ComboBox1->insertItem("A7");
    ComboBox1->insertItem("A8");
    ComboBox1->insertItem("A9");
    ComboBox1->insertItem("B0");
    ComboBox1->insertItem("B1");
    ComboBox1->insertItem("B2");
    ComboBox1->insertItem("B3");
    ComboBox1->insertItem("B4");
    ComboBox1->insertItem("B5");
    ComboBox1->insertItem("B6");
    ComboBox1->insertItem("B7");
    ComboBox1->insertItem("B8");
    ComboBox1->insertItem("B9");
    ComboBox1->insertItem("B10");
    ComboBox1->insertItem("C5E");
    ComboBox1->insertItem("Comm10E");
    ComboBox1->insertItem("DLE");
    ComboBox1->insertItem("Executive");
    ComboBox1->insertItem("Folio");
    ComboBox1->insertItem("Ledger");
    ComboBox1->insertItem("Legal");
    ComboBox1->insertItem("Letter");
    ComboBox1->insertItem("Tabloid");
    ComboBox1->insertItem( tr( "Custom" ) );
		ComboBox1->setEditable(false);
    Layout6->addWidget(ComboBox1, 0, 1 );
    TextLabel2 = new QLabel( ButtonGroup1_2, "TextLabel2" );
    TextLabel2->setText( tr( "Orientation:" ));
    Layout6->addWidget( TextLabel2, 1, 0 );
		ComboBox2 = new QComboBox( true, ButtonGroup1_2, "ComboBox2" );
    ComboBox2->insertItem( tr( "Portrait" ) );
    ComboBox2->insertItem( tr( "Landscape" ) );
		ComboBox2->setEditable(false);
		ComboBox2->setCurrentItem(Vor->Ausrichtung);
    Layout6->addWidget( ComboBox2, 1, 1 );
    ButtonGroup1_2Layout->addLayout( Layout6 );

    Layout5 = new QHBoxLayout( 0, 0, 6, "Layout5");
		TextLabel1_2 = new QLabel( ButtonGroup1_2, "TextLabel1_2" );
    TextLabel1_2->setText( tr( "Width:" ) );
    Layout5->addWidget( TextLabel1_2 );
		Breite = new MSpinBox( ButtonGroup1_2, 2 );
    Breite->setEnabled( false );
    Breite->setMinimumSize( QSize( 70, 20 ) );
    Breite->setSuffix(ein);
    Breite->setMaxValue( 1000000 );
    Breite->setMinValue( 1 );
    Layout5->addWidget( Breite );
    TextLabel2_2 = new QLabel( ButtonGroup1_2, "TextLabel2_2" );
    TextLabel2_2->setText( tr( "Height:" ) );
    Layout5->addWidget( TextLabel2_2 );
		Hoehe = new MSpinBox( ButtonGroup1_2, 2 );
    Hoehe->setEnabled( false );
    Hoehe->setMinimumSize( QSize( 70, 20 ) );
    Hoehe->setSuffix(ein);
    Hoehe->setMaxValue( 1000000 );
    Hoehe->setMinValue( 1 );
    Layout5->addWidget( Hoehe );
    ButtonGroup1_2Layout->addLayout( Layout5 );
    Layout8 = new QHBoxLayout( 0, 0, 6, "Layout8");
    Doppelseiten = new QCheckBox( ButtonGroup1_2, "Doppelseiten" );
    Doppelseiten->setText( tr( "Facing Pages" ) );
		Doppelseiten->setChecked(Vor->DoppelSeiten);
    Layout8->addWidget( Doppelseiten );
    ErsteSeite = new QCheckBox( ButtonGroup1_2, "CheckBox3" );
    ErsteSeite->setText( tr( "Left Page first" ) );
		ErsteSeite->setChecked(Vor->ErsteLinks);
    Layout8->addWidget( ErsteSeite );
    ButtonGroup1_2Layout->addLayout( Layout8 );
    Layout9->addWidget( ButtonGroup1_2 );
		ComboBox1->setCurrentItem(Vor->PageFormat);
    Breite->setValue(qRound(Vor->PageBreite * Umrech) * 100);
    Hoehe->setValue(qRound(Vor->PageHoehe * Umrech) * 100);
    Breite->setLineStep(100);
    Hoehe->setLineStep(100);

    GroupBox7 = new QGroupBox( this, "GroupBox7" );
    GroupBox7->setTitle( tr( "Margin Guides" ) );
    GroupBox7->setColumnLayout(0, Qt::Vertical );
    GroupBox7->layout()->setSpacing( 0 );
    GroupBox7->layout()->setMargin( 0 );
    GroupBox7Layout = new QHBoxLayout( GroupBox7->layout() );
    GroupBox7Layout->setAlignment( Qt::AlignTop );
    GroupBox7Layout->setSpacing( 5 );
    GroupBox7Layout->setMargin( 10 );
    Layout3 = new QGridLayout;
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 5 );
    TextLabel6 = new QLabel( GroupBox7, "TextLabel6" );
    TextLabel6->setText( tr( "Left:" ) );
    Layout3->addWidget( TextLabel6, 0, 2 );
    TextLabel8 = new QLabel( GroupBox7, "TextLabel8" );
    TextLabel8->setText( tr( "Right:" ) );
    Layout3->addWidget( TextLabel8, 1, 2 );
    TextLabel5 = new QLabel( GroupBox7, "TextLabel5" );
    TextLabel5->setText( tr( "Top:" ) );
    Layout3->addWidget( TextLabel5, 0, 0 );
    TextLabel7 = new QLabel( GroupBox7, "TextLabel7" );
    TextLabel7->setText( tr( "Bottom:" ) );
    Layout3->addWidget( TextLabel7, 1, 0 );
		TopR = new MSpinBox( GroupBox7, 2 );
    TopR->setMinimumSize( QSize( 70, 20 ) );
    TopR->setMaxValue( 100000 );
    TopR->setMinValue( 0 );
    TopR->setSuffix( ein );
    TopR->setValue( qRound(Vor->RandOben * Umrech * 100));
		Top = Vor->RandOben;
    Layout3->addWidget( TopR, 0, 1 );
		BottomR = new MSpinBox( GroupBox7, 2 );
    BottomR->setMinimumSize( QSize( 70, 20 ) );
    BottomR->setSuffix( ein );
    BottomR->setMaxValue( 100000 );
    BottomR->setMinValue( 0 );
    BottomR->setValue( qRound(Vor->RandUnten * Umrech * 100) );
		Bottom = Vor->RandUnten;
    Layout3->addWidget( BottomR, 1, 1 );
		LeftR = new MSpinBox( GroupBox7, 2 );
    LeftR->setMinimumSize( QSize( 70, 20 ) );
    LeftR->setSuffix( ein );
    LeftR->setMaxValue( 100000 );
    LeftR->setMinValue( 0 );
    LeftR->setValue( qRound(Vor->RandLinks * Umrech * 100) );
		Left = Vor->RandLinks;
    Layout3->addWidget( LeftR, 0, 3 );
		RightR = new MSpinBox( GroupBox7, 2 );
    RightR->setMinimumSize( QSize( 70, 20 ) );
    RightR->setSuffix( ein );
    RightR->setMaxValue( 100000 );
    RightR->setMinValue( 0 );
    RightR->setValue( qRound(Vor->RandRechts * Umrech * 100) );
		Right = Vor->RandRechts;
    Layout3->addWidget( RightR, 1, 3 );
    GroupBox7Layout->addLayout( Layout3 );
    Layout9->addWidget( GroupBox7 );
    NewDocLayout->addLayout( Layout9 );
		setDS();
    TopR->setDecimals(100);
    LeftR->setDecimals(100);
    RightR->setDecimals(100);
   	BottomR->setDecimals(100);
		setSize(Vor->PageFormat);
		setOrien(Vor->Ausrichtung);

    Layout10 = new QVBoxLayout( 0, 0, 6, "Layout10");

    GroupBox3 = new QGroupBox( this, "GroupBox3" );
    GroupBox3->setTitle( tr( "Options" ) );
    GroupBox3->setColumnLayout(0, Qt::Vertical );
    GroupBox3->layout()->setSpacing( 5 );
    GroupBox3->layout()->setMargin( 10 );
    GroupBox3Layout = new QGridLayout( GroupBox3->layout() );
    GroupBox3Layout->setAlignment( Qt::AlignTop );
    TextLabel1_3 = new QLabel( GroupBox3, "TextLabel1_3" );
    TextLabel1_3->setText( tr( "First Page Number:" ) );
    GroupBox3Layout->addMultiCellWidget( TextLabel1_3, 0, 0, 0, 1 );
    PgNr = new QSpinBox( GroupBox3, "PgNr" );
    PgNr->setMaxValue( 1000 );
    PgNr->setMinValue( 1 );
    GroupBox3Layout->addWidget( PgNr, 0, 2, Qt::AlignRight );
    TextLabel2_3 = new QLabel( GroupBox3, "TextLabel2_3" );
    TextLabel2_3->setText( tr( "Default Unit:" ) );
    GroupBox3Layout->addWidget( TextLabel2_3, 1, 0 );
    ComboBox3 = new QComboBox( true, GroupBox3, "ComboBox3" );
    ComboBox3->insertItem( tr( "Points (pts)" ) );
    ComboBox3->insertItem( tr( "Millimeters (mm)" ) );
    ComboBox3->insertItem( tr( "Inches (in)" ) );
    ComboBox3->insertItem( tr( "Picas (p)" ) );
		ComboBox3->setCurrentItem(einheit);
		ComboBox3->setEditable(false);
    GroupBox3Layout->addMultiCellWidget( ComboBox3, 1, 1, 1, 2 );
    Layout10->addWidget( GroupBox3 );

    AutoFrame = new QCheckBox( this, "AutoFrame" );
    AutoFrame->setText( tr( "Autom. Text Frames" ) );
    Layout10->addWidget( AutoFrame );

    GroupBox4 = new QGroupBox( this, "GroupBox4" );
    GroupBox4->setTitle( tr( "Column Guides" ) );
    GroupBox4->setColumnLayout(0, Qt::Vertical );
    GroupBox4->layout()->setSpacing( 0 );
    GroupBox4->layout()->setMargin( 0 );
    GroupBox4Layout = new QHBoxLayout( GroupBox4->layout() );
    GroupBox4Layout->setAlignment( Qt::AlignTop );
    GroupBox4Layout->setSpacing( 5 );
    GroupBox4Layout->setMargin( 10 );
    Layout2 = new QGridLayout;
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 5 );
    TextLabel4 = new QLabel( GroupBox4, "TextLabel4" );
    TextLabel4->setText( tr( "Gutter Width:" ) );
    Layout2->addWidget( TextLabel4, 1, 0 );
    TextLabel3 = new QLabel( GroupBox4, "TextLabel3" );
    TextLabel3->setText( tr( "Columns:" ) );
    Layout2->addWidget( TextLabel3, 0, 0 );
		Distance = new MSpinBox( GroupBox4, 2 );
    Distance->setSuffix( ein );
    Distance->setMaxValue( 100000 );
    Distance->setValue( qRound(11 * Umrech * 100) );
   	Distance->setDecimals(100);
		Dist = 11;
    Layout2->addWidget( Distance, 1, 1, Qt::AlignLeft );
    SpinBox10 = new QSpinBox( GroupBox4, "SpinBox10" );
    SpinBox10->setButtonSymbols( QSpinBox::UpDownArrows );
    SpinBox10->setMinValue( 1 );
    SpinBox10->setValue( 1 );
    Layout2->addWidget( SpinBox10, 0, 1, Qt::AlignLeft );
    GroupBox4Layout->addLayout( Layout2 );
    Layout10->addWidget( GroupBox4 );
		GroupBox4->setEnabled(false);

    Layout1 = new QHBoxLayout;
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );
    OKButton = new QPushButton( this, "OKButton" );
    OKButton->setText( tr( "OK" ) );
    OKButton->setDefault( true );
    Layout1->addWidget( OKButton );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );
    CancelB = new QPushButton( this, "CancelB" );
    CancelB->setText( tr( "Cancel" ) );
    CancelB->setAutoDefault( false );
    Layout1->addWidget( CancelB );
    Layout10->addLayout( Layout1 );
    NewDocLayout->addLayout( Layout10 );

    setMinimumSize(sizeHint());

    // signals and slots connections
    connect( OKButton, SIGNAL( clicked() ), this, SLOT( ExitOK() ) );
    connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( Doppelseiten, SIGNAL( clicked() ), this, SLOT( setDS() ) );
    connect( AutoFrame, SIGNAL( clicked() ), this, SLOT( setAT() ) );
		connect(ComboBox1, SIGNAL(activated(int)), this, SLOT(setPGsize()));
		connect(ComboBox2, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
		connect(ComboBox3, SIGNAL(activated(int)), this, SLOT(setUnit(int)));
		connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
		connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
		connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
		connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
		connect(Distance, SIGNAL(valueChanged(int)), this, SLOT(setDist(int)));
}

void NewDoc::code_repeat(int m)
{
	switch (m)
	{
		case 0 :
		case 3 :
			RightR->setMaxValue(Breite->value() - LeftR->value());
			if (m == 3)
				break;
		case 4 :
			LeftR->setMaxValue(Breite->value() - RightR->value());
			if (m == 4)
				break;
		case 2 :
			TopR->setMaxValue(Hoehe->value() - BottomR->value());
			if (m == 2)
				break;
		case 1 :
			BottomR->setMaxValue(Hoehe->value() - TopR->value());
			break;
	}
}
		
void NewDoc::setBreite(int v)
{
	Pagebr = v / Umrech / 100.0;
	code_repeat(0);
}

void NewDoc::setHoehe(int v)
{
	Pageho = v / Umrech / 100.0;
	code_repeat(0);
}

void NewDoc::setTop(int v)
{
	Top = v / Umrech / 100.0;
	code_repeat(1);
}

void NewDoc::setBottom(int v)
{
	Bottom = v / Umrech / 100.0;
	code_repeat(2);
}

void NewDoc::setLeft(int v)
{
	Left = v / Umrech / 100.0;
	code_repeat(3);
}

void NewDoc::setRight(int v)
{
	Right = v / Umrech / 100.0;
	code_repeat(4);
}

void NewDoc::setDist(int v)
{
	Dist = v / Umrech / 100.0;
}

void NewDoc::setUnit(int u)
{
	switch (u)
		{
		case 0:
   		Umrech = 1.0;
   		ein = " pt";
			break;
		case 1:
			Umrech = 0.3527777;
   		ein = " mm";
			break;
		case 2:
			Umrech = 1.0 / 72.0;
   		ein = " in";
			break;
		case 3:
			Umrech = 1.0 / 12.0;
   		ein = " p";
			break;
		}
	einheit = u;
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	disconnect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	disconnect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	disconnect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	disconnect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	Breite->setValue(qRound(Pagebr * Umrech * 100));
	Hoehe->setValue(qRound(Pageho * Umrech * 100));
	RightR->setMaxValue(Breite->value() - qRound(Left * Umrech * 100));
	LeftR->setMaxValue(Breite->value() - qRound(Right * Umrech * 100));
	TopR->setMaxValue(Hoehe->value() - qRound(Bottom * Umrech * 100));
	BottomR->setMaxValue(Hoehe->value() - qRound(Top * Umrech * 100));
	TopR->setValue(qRound(Top * Umrech * 100));
	BottomR->setValue(qRound(Bottom * Umrech * 100));
	LeftR->setValue(qRound(Left * Umrech * 100));
	RightR->setValue(qRound(Right * Umrech * 100));
	connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	Distance->setValue(qRound(Dist * Umrech * 100));
//	setSize(ComboBox1->currentItem());
  TopR->setSuffix(ein);
  BottomR->setSuffix(ein);
  LeftR->setSuffix(ein);
  RightR->setSuffix(ein);
  Breite->setSuffix(ein);
  Hoehe->setSuffix(ein);
  Distance->setSuffix( ein );
}

void NewDoc::ExitOK()
{
	if (ComboBox1->currentItem() == 30)
		{
		Pagebr = Breite->value() / 100.0 / Umrech;
		Pageho = Hoehe->value() / 100.0 / Umrech;
		}
	if (Orient == 1)
		{
		double br = Pagebr;
		Pagebr = Pageho;
		Pageho = br;
		}
	accept();
}

void NewDoc::setOrien(int ori)
{
	int br;
	setSize(ComboBox1->currentItem());
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	if (ori == 0)
		{
		if (ComboBox1->currentItem() == 30)
			{
			br = Breite->value();
			Breite->setValue(Hoehe->value());
			Hoehe->setValue(br);
			}
		Orient = 0;
		}
	else
		{
		Orient = 1;
		br = Breite->value();
		Breite->setValue(Hoehe->value());
		Hoehe->setValue(br);
		}
	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
}

void NewDoc::setPGsize()
{
	if (ComboBox1->currentItem() == 30)
		setSize(ComboBox1->currentItem());
	else
		setOrien(ComboBox2->currentItem());
}

void NewDoc::setSize(int gr)
{
	Pagebr = Breite->value() / 100.0;
	Pageho = Hoehe->value() / 100.0;
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	Breite->setEnabled(false);
	Hoehe->setEnabled(false);
	switch (gr)
		{
		case 0: // A0
			Pagebr = 2380;
			Pageho = 3368;
			break;
		case 1: // A1
			Pagebr = 1684;
			Pageho = 2380;
			break;
		case 2: // A2
			Pagebr = 1190;
			Pageho = 1684;
			break;
		case 3: // A3
			Pagebr = 842;
			Pageho = 1190;
			break;
		case 4: // A4
			Pagebr = 595;
			Pageho = 842;
			break;
		case 5: // A5
			Pagebr = 421;
			Pageho = 595;
			break;
		case 6: // A6
			Pagebr = 297;
			Pageho = 421;
			break;
		case 7: // A7
			Pagebr = 210;
			Pageho = 297;
			break;
		case 8: // A8
			Pagebr = 148;
			Pageho = 210;
			break;
		case 9: // A9
			Pagebr = 105;
			Pageho = 148;
			break;
		case 10: // B0
			Pagebr = 2836;
			Pageho = 4008;
			break;
		case 11: // B1
			Pagebr = 2004;
			Pageho = 2836;
			break;
		case 12: // B2
			Pagebr = 1418;
			Pageho = 2004;
			break;
		case 13: // B3
			Pagebr = 1002;
			Pageho = 1418;
			break;
		case 14: // B4
			Pagebr = 709;
			Pageho = 1002;
			break;
		case 15: // B5
    	Pagebr = 501;
    	Pageho = 709;
			break;
		case 16: // B6
			Pagebr = 355;
			Pageho = 501;
			break;
		case 17: // B7
			Pagebr = 250;
			Pageho = 355;
			break;
		case 18: // B8
			Pagebr = 178;
			Pageho = 250;
			break;
		case 19: // B9
			Pagebr = 125;
			Pageho = 178;
			break;
		case 20: // B10
			Pagebr = 89;
			Pageho = 125;
			break;
		case 21: // C5E
			Pagebr = 462;
			Pageho = 649;
			break;
		case 22: // Comm10E
			Pagebr = 298;
			Pageho = 683;
			break;
		case 23: // DLE
			Pagebr = 312;
			Pageho = 624;
			break;
		case 24: // Executive
			Pagebr = 542;
			Pageho = 720;
			break;
		case 25: // Folio
			Pagebr = 595;
			Pageho = 935;
			break;
		case 26: // Ledger
			Pagebr = 1224;
			Pageho = 792;
			break;
		case 27: // Legal
    	Pagebr = 612;
    	Pageho = 1008;
			break;
		case 28: // Letter
    	Pagebr = 612;
    	Pageho = 792;
			break;
		case 29: // Tabloid
    	Pagebr = 792;
    	Pageho = 1224;
			break;
		case 30: // Custom
    	Breite->setEnabled(true);
    	Hoehe->setEnabled(true);
			break;
		}
	Breite->setValue(qRound(Pagebr * Umrech * 100));
	Hoehe->setValue(qRound(Pageho * Umrech * 100));
	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
}

void NewDoc::setAT()
{
    if (AutoFrame->isChecked())
			GroupBox4->setEnabled(true); 	
    else
			GroupBox4->setEnabled(false);
}

void NewDoc::setDS()
{
    if (Doppelseiten->isChecked())
    	{
    	TextLabel6->setText( tr( "Inside:" ) );
    	TextLabel8->setText( tr( "Outside:" ) );
    	ErsteSeite->setEnabled(true);    	
    	}
    else
    	{
    	TextLabel6->setText( tr( "Left:" ) );
    	TextLabel8->setText( tr( "Right:" ) );
    	ErsteSeite->setEnabled(false);
    	}
}


