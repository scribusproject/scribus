#include "newfile.h"
#include "newfile.moc"
extern QPixmap loadIcon(QString nam);

NewDoc::NewDoc( QWidget* parent, preV *Vor )
    : QDialog( parent, "newDoc", true, 0 )
{
	char *units[] = {" pt", " mm", " in", " p"};
	ein = units[Vor->Einheit];
	switch (Vor->Einheit)
	{
		case 0:
    		Umrech = 1.0;
			break;
		case 1:
			Umrech = 0.3527777;
			break;
		case 2:
			Umrech = 1.0 / 72.0;
    		break;
		case 3:
			Umrech = 1.0 / 12.0;
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
	QString sizelist[] = {"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3", "B4",
						"B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", "Executive", "Folio",
						"Ledger", "Letter", "Tabloid", tr("Custom")};
	size_t const num_mappings = (sizeof sizelist)/(sizeof *sizelist);
	for (uint m = 0; m < num_mappings; ++m)
		ComboBox1->insertItem(sizelist[m]);
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
    Breite->setMaxValue( 10000 );
    Breite->setMinValue( 1 );
    Layout5->addWidget( Breite );
    TextLabel2_2 = new QLabel( ButtonGroup1_2, "TextLabel2_2" );
    TextLabel2_2->setText( tr( "Height:" ) );
    Layout5->addWidget( TextLabel2_2 );
	Hoehe = new MSpinBox( ButtonGroup1_2, 2 );
    Hoehe->setEnabled( false );
    Hoehe->setMinimumSize( QSize( 70, 20 ) );
    Hoehe->setSuffix(ein);
    Hoehe->setMaxValue( 10000 );
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
    Breite->setValue(Vor->PageBreite * Umrech);
    Hoehe->setValue(Vor->PageHoehe * Umrech);

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
    TopR->setMaxValue( 1000 );
    TopR->setMinValue( 0 );
    TopR->setSuffix( ein );
    TopR->setValue(Vor->RandOben * Umrech);
	Top = Vor->RandOben;
    Layout3->addWidget( TopR, 0, 1 );
	BottomR = new MSpinBox( GroupBox7, 2 );
    BottomR->setMinimumSize( QSize( 70, 20 ) );
    BottomR->setSuffix( ein );
    BottomR->setMaxValue( 1000 );
    BottomR->setMinValue( 0 );
    BottomR->setValue(Vor->RandUnten * Umrech);
	Bottom = Vor->RandUnten;
    Layout3->addWidget( BottomR, 1, 1 );
	LeftR = new MSpinBox( GroupBox7, 2 );
    LeftR->setMinimumSize( QSize( 70, 20 ) );
    LeftR->setSuffix( ein );
    LeftR->setMaxValue( 1000 );
    LeftR->setMinValue( 0 );
    LeftR->setValue(Vor->RandLinks * Umrech);
	Left = Vor->RandLinks;
    Layout3->addWidget( LeftR, 0, 3 );
	RightR = new MSpinBox( GroupBox7, 2 );
    RightR->setMinimumSize( QSize( 70, 20 ) );
    RightR->setSuffix( ein );
    RightR->setMaxValue( 1000 );
    RightR->setMinValue( 0 );
    RightR->setValue(Vor->RandRechts * Umrech);
	Right = Vor->RandRechts;
    Layout3->addWidget( RightR, 1, 3 );
    GroupBox7Layout->addLayout( Layout3 );
    Layout9->addWidget( GroupBox7 );
    NewDocLayout->addLayout( Layout9 );
	setDS();
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
    Distance->setMaxValue( 1000 );
    Distance->setValue(11 * Umrech);
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
		
void NewDoc::setBreite(int)
{
	Pagebr = Breite->value() / Umrech;
	code_repeat(0);
}

void NewDoc::setHoehe(int)
{
	Pageho = Hoehe->value() / Umrech;
	code_repeat(0);
}

void NewDoc::setTop(int)
{
	Top = TopR->value() / Umrech;
	code_repeat(1);
}

void NewDoc::setBottom(int)
{
	Bottom = BottomR->value() / Umrech;
	code_repeat(2);
}

void NewDoc::setLeft(int)
{
	Left = LeftR->value() / Umrech;
	code_repeat(3);
}

void NewDoc::setRight(int)
{
	Right = RightR->value() / Umrech;
	code_repeat(4);
}

void NewDoc::setDist(int v)
{
	Dist = Distance->value() / Umrech;
}

void NewDoc::setUnit(int u)
{
	ein = units[u];
	switch (u)
	{
		case 0:
   			Umrech = 1.0;
			break;
		case 1:
			Umrech = 0.3527777;
   			break;
		case 2:
			Umrech = 1.0 / 72.0;
   			break;
		case 3:
			Umrech = 1.0 / 12.0;
   			break;
	}
	einheit = u;
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	disconnect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	disconnect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	disconnect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	disconnect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	Breite->setValue(Pagebr * Umrech);
	Hoehe->setValue(Pageho * Umrech);
	RightR->setMaxValue(Breite->value() - Left * Umrech);
	LeftR->setMaxValue(Breite->value() - Right * Umrech);
	TopR->setMaxValue(Hoehe->value() - Bottom * Umrech);
	BottomR->setMaxValue(Hoehe->value() - Top * Umrech);
	TopR->setValue(Top * Umrech);
	BottomR->setValue(Bottom * Umrech);
	LeftR->setValue(Left * Umrech);
	RightR->setValue(Right * Umrech);
	connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	Distance->setValue(Dist * Umrech);
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
		Pagebr = Breite->value() / Umrech;
		Pageho = Hoehe->value() / Umrech;
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
	double br;
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
	Pagebr = Breite->value();
	Pageho = Hoehe->value();
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	Breite->setEnabled(false);
	Hoehe->setEnabled(false);
	int page_x[] = {2380, 1684, 1190, 842, 595, 421, 297, 210, 148, 105, 2836, 2004, 1418, 1002, 709, 501,
					355, 250, 178, 125, 89, 462, 298, 312, 542, 595, 1224, 612, 612, 792};
	int page_y[] = {3368, 2380, 1684, 1190, 842, 595, 421, 297, 210, 148, 4008, 2836, 2004, 1418, 1002, 709,
					501, 355, 250, 178, 125, 649, 683, 624, 720, 935, 792, 1008, 792, 1225};
	if (gr == 30)
	{
		Breite->setEnabled(true);
		Hoehe->setEnabled(true);
	}
	Pagebr = page_x[gr];
	Pageho = page_y[gr];
	Breite->setValue(Pagebr * Umrech);
	Hoehe->setValue(Pageho * Umrech);
	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
}

void NewDoc::setAT()
{
	GroupBox4->setEnabled(AutoFrame->isChecked() ? true : false);
}

void NewDoc::setDS()
{
	bool test = Doppelseiten->isChecked() ? false : true;
	TextLabel6->setText(test == false ? tr("Inside:") : tr("Left:"));
	TextLabel8->setText(test == false ? tr("Outside:") : tr("Right:"));
	ErsteSeite->setEnabled(test == false ? true : false);
}
