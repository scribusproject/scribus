#include "newfile.h"
#include "newfile.moc"
#include <qtooltip.h>
extern QPixmap loadIcon(QString nam);

NewDoc::NewDoc( QWidget* parent, preV *Vor )
		: QDialog( parent, "newDoc", true, 0 )
{
	QString units[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	int decimals;
	ein = units[Vor->Einheit];
	switch (Vor->Einheit)
	{
	case 0:
		Umrech = 1.0;
		decimals = 2;
		break;
	case 1:
		Umrech = 0.3527777;
		decimals = 3;
		break;
	case 2:
		Umrech = 1.0 / 72.0;
		decimals = 4;
		break;
	case 3:
		Umrech = 1.0 / 12.0;
		decimals = 2;
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
	TextLabel1 = new QLabel( tr( "&Size:" ), ButtonGroup1_2, "TextLabel1" );
	Layout6->addWidget( TextLabel1, 0, 0 );
	ComboBox1 = new QComboBox( true, ButtonGroup1_2, "ComboBox1" );
	QString sizelist[] = {"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3", "B4",
	                      "B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", "Executive", "Folio",
	                      "Ledger", tr("Legal"), tr("Letter"), tr("Tabloid"), tr("Custom")};
	size_t const num_mappings = (sizeof sizelist)/(sizeof *sizelist);
	for (uint m = 0; m < num_mappings; ++m)
		ComboBox1->insertItem(sizelist[m]);
	ComboBox1->setEditable(false);
	TextLabel1->setBuddy(ComboBox1);
	Layout6->addWidget(ComboBox1, 0, 1 );
	TextLabel2 = new QLabel( tr( "Orie&ntation:" ), ButtonGroup1_2, "TextLabel2" );
	Layout6->addWidget( TextLabel2, 1, 0 );
	ComboBox2 = new QComboBox( true, ButtonGroup1_2, "ComboBox2" );
	ComboBox2->insertItem( tr( "Portrait" ) );
	ComboBox2->insertItem( tr( "Landscape" ) );
	ComboBox2->setEditable(false);
	ComboBox2->setCurrentItem(Vor->Ausrichtung);
	TextLabel2->setBuddy(ComboBox2);
	Layout6->addWidget( ComboBox2, 1, 1 );
	ButtonGroup1_2Layout->addLayout( Layout6 );

	Layout5 = new QHBoxLayout( 0, 0, 6, "Layout5");
	TextLabel1_2 = new QLabel( tr( "&Width:" ), ButtonGroup1_2, "TextLabel1_2" );
	Layout5->addWidget( TextLabel1_2 );
	Breite = new MSpinBox( 1, 10000, ButtonGroup1_2, decimals );
	Breite->setEnabled( false );
	Breite->setMinimumSize( QSize( 70, 20 ) );
	Breite->setSuffix(ein);
	TextLabel1_2->setBuddy(Breite);
	Layout5->addWidget( Breite );
	TextLabel2_2 = new QLabel( tr( "&Height:" ), ButtonGroup1_2, "TextLabel2_2" );
	Layout5->addWidget( TextLabel2_2 );
	Hoehe = new MSpinBox( 1, 10000, ButtonGroup1_2, decimals );
	Hoehe->setEnabled( false );
	Hoehe->setMinimumSize( QSize( 70, 20 ) );
	Hoehe->setSuffix(ein);
	TextLabel2_2->setBuddy(Hoehe);
	Layout5->addWidget( Hoehe );
	ButtonGroup1_2Layout->addLayout( Layout5 );
	Layout8 = new QHBoxLayout( 0, 0, 6, "Layout8");
	Doppelseiten = new QCheckBox( tr( "&Facing Pages" ), ButtonGroup1_2, "Doppelseiten" );
	Doppelseiten->setChecked(Vor->DoppelSeiten);
	Layout8->addWidget( Doppelseiten );
	ErsteSeite = new QCheckBox( tr( "Left &Page First" ), ButtonGroup1_2, "CheckBox3" );
	ErsteSeite->setChecked(Vor->ErsteLinks);
	Layout8->addWidget( ErsteSeite );
	ButtonGroup1_2Layout->addLayout( Layout8 );
	Layout9->addWidget( ButtonGroup1_2 );

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
	TextLabel6 = new QLabel( tr( "&Left:" ), GroupBox7, "TextLabel6" );
	Layout3->addWidget( TextLabel6, 0, 2 );
	TextLabel8 = new QLabel( tr( "&Right:" ), GroupBox7, "TextLabel8" );
	Layout3->addWidget( TextLabel8, 1, 2 );
	TextLabel5 = new QLabel( tr( "&Top:" ), GroupBox7, "TextLabel5" );
	Layout3->addWidget( TextLabel5, 0, 0 );
	TextLabel7 = new QLabel( tr( "&Bottom:" ), GroupBox7, "TextLabel7" );
	Layout3->addWidget( TextLabel7, 1, 0 );
	TopR = new MSpinBox( 0, 1000, GroupBox7, decimals );
	TopR->setMinimumSize( QSize( 70, 20 ) );
	TopR->setSuffix( ein );
	TopR->setValue(Vor->RandOben * Umrech);
	Top = Vor->RandOben;
	TextLabel5->setBuddy(TopR);
	Layout3->addWidget( TopR, 0, 1 );
	BottomR = new MSpinBox( 0, 1000, GroupBox7, decimals );
	BottomR->setMinimumSize( QSize( 70, 20 ) );
	BottomR->setSuffix( ein );
	BottomR->setValue(Vor->RandUnten * Umrech);
	Bottom = Vor->RandUnten;
	TextLabel7->setBuddy(BottomR);
	Layout3->addWidget( BottomR, 1, 1 );
	LeftR = new MSpinBox( 0, 1000, GroupBox7, decimals );
	LeftR->setMinimumSize( QSize( 70, 20 ) );
	LeftR->setSuffix( ein );
	LeftR->setValue(Vor->RandLinks * Umrech);
	Left = Vor->RandLinks;
	TextLabel6->setBuddy(LeftR);
	Layout3->addWidget( LeftR, 0, 3 );
	RightR = new MSpinBox( 0, 1000, GroupBox7, decimals );
	RightR->setMinimumSize( QSize( 70, 20 ) );
	RightR->setSuffix( ein );
	RightR->setValue(Vor->RandRechts * Umrech);
	Right = Vor->RandRechts;
	TextLabel8->setBuddy(RightR);
	Layout3->addWidget( RightR, 1, 3 );
	GroupBox7Layout->addLayout( Layout3 );
	Layout9->addWidget( GroupBox7 );
	NewDocLayout->addLayout( Layout9 );
	Breite->setValue(Vor->PageBreite * Umrech);
	Hoehe->setValue(Vor->PageHoehe * Umrech);
	ComboBox1->setCurrentItem(Vor->PageFormat);
	setDS();
	setSize(Vor->PageFormat);
	setOrien(Vor->Ausrichtung);
	Breite->setValue(Vor->PageBreite * Umrech);
	Hoehe->setValue(Vor->PageHoehe * Umrech);

	Layout10 = new QVBoxLayout( 0, 0, 6, "Layout10");

	GroupBox3 = new QGroupBox( this, "GroupBox3" );
	GroupBox3->setTitle( tr( "Options" ) );
	GroupBox3->setColumnLayout(0, Qt::Vertical );
	GroupBox3->layout()->setSpacing( 5 );
	GroupBox3->layout()->setMargin( 10 );
	GroupBox3Layout = new QGridLayout( GroupBox3->layout() );
	GroupBox3Layout->setAlignment( Qt::AlignTop );
	TextLabel1_3 = new QLabel( tr( "F&irst Page Number:" ), GroupBox3, "TextLabel1_3" );
	GroupBox3Layout->addMultiCellWidget( TextLabel1_3, 0, 0, 0, 1 );
	PgNr = new QSpinBox( GroupBox3, "PgNr" );
	PgNr->setMaxValue( 1000 );
	PgNr->setMinValue( 1 );
	TextLabel1_3->setBuddy(PgNr);
	GroupBox3Layout->addWidget( PgNr, 0, 2, Qt::AlignRight );
	TextLabel2_3 = new QLabel( tr( "&Default Unit:" ), GroupBox3, "TextLabel2_3" );
	GroupBox3Layout->addWidget( TextLabel2_3, 1, 0 );
	ComboBox3 = new QComboBox( true, GroupBox3, "ComboBox3" );
	ComboBox3->insertItem( tr( "Points (pts)" ) );
	ComboBox3->insertItem( tr( "Millimetres (mm)" ) );
	ComboBox3->insertItem( tr( "Inches (in)" ) );
	ComboBox3->insertItem( tr( "Picas (p)" ) );
	ComboBox3->setCurrentItem(einheit);
	ComboBox3->setEditable(false);
	TextLabel2_3->setBuddy(ComboBox3);
	GroupBox3Layout->addMultiCellWidget( ComboBox3, 1, 1, 1, 2 );
	Layout10->addWidget( GroupBox3 );

	AutoFrame = new QCheckBox( tr( "&Automatic Text Frames" ), this, "AutoFrame" );
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
	TextLabel4 = new QLabel( tr( "&Gap:" ), GroupBox4, "TextLabel4" );
	Layout2->addWidget( TextLabel4, 1, 0 );
	TextLabel3 = new QLabel( tr( "Colu&mns:" ), GroupBox4, "TextLabel3" );
	Layout2->addWidget( TextLabel3, 0, 0 );
	Distance = new MSpinBox( 0, 1000, GroupBox4, decimals );
	Distance->setSuffix( ein );
	Distance->setValue(11 * Umrech);
	Dist = 11;
	TextLabel4->setBuddy(Distance);
	Layout2->addWidget( Distance, 1, 1, Qt::AlignLeft );
	SpinBox10 = new QSpinBox( GroupBox4, "SpinBox10" );
	SpinBox10->setButtonSymbols( QSpinBox::UpDownArrows );
	SpinBox10->setMinValue( 1 );
	SpinBox10->setValue( 1 );
	TextLabel3->setBuddy(SpinBox10);
	Layout2->addWidget( SpinBox10, 0, 1, Qt::AlignLeft );
	GroupBox4Layout->addLayout( Layout2 );
	Layout10->addWidget( GroupBox4 );
	GroupBox4->setEnabled(false);

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 6 );
	Layout1->setMargin( 0 );
	OKButton = new QPushButton( tr( "&OK" ), this, "OKButton" );
	OKButton->setDefault( true );
	Layout1->addWidget( OKButton );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );
	CancelB = new QPushButton( tr( "&Cancel" ), this, "CancelB" );
	CancelB->setAutoDefault( false );
	Layout1->addWidget( CancelB );
	Layout10->addLayout( Layout1 );
	NewDocLayout->addLayout( Layout10 );

	setMinimumSize(sizeHint());
	//tab order
	QWidget::setTabOrder ( AutoFrame, SpinBox10 );
	QWidget::setTabOrder ( SpinBox10, Distance );
	//tooltips
	QToolTip::add( ComboBox1, tr( "Document page size, either a standard size or a custom size" ) );
	QToolTip::add( ComboBox2, tr( "Orientation of the document's pages" ) );
	QToolTip::add( Breite, tr( "Width of the document's pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( Hoehe, tr( "Height of the document's pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( Doppelseiten, tr( "Enable single or spread based layout" ) );
	QToolTip::add( ErsteSeite, tr( "Make the first page the left page of the document" ) );
	QToolTip::add( TopR, tr( "Distance between the top margin guide and the edge of the page" ) );
	QToolTip::add( BottomR, tr( "Distance between the bottom margin guide and the edge of the page" ) );
	QToolTip::add( LeftR, tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( RightR, tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( PgNr, tr( "First page number of the document" ) );
	QToolTip::add( ComboBox3, tr( "Default unit of measurement for document editing" ) );
	QToolTip::add( AutoFrame, tr( "Create text frames automatically when new pages are added" ) );
	QToolTip::add( SpinBox10, tr( "Number of columns to create in automatically created text frames" ) );
	QToolTip::add( Distance, tr( "Distance between automatically created columns" ) );

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
	QString units[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	ein = units[u];
	int decimals;
	double AltUmrech = Umrech;
	double val, oldB, oldBM, oldH, oldHM;
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	disconnect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	disconnect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	disconnect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	disconnect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	Breite->getValues(&oldB, &oldBM, &decimals, &val);
	oldB /= AltUmrech;
	oldBM /= AltUmrech;
	Hoehe->getValues(&oldH, &oldHM, &decimals, &val);
	oldH /= AltUmrech;
	oldHM /= AltUmrech;
	switch (u)
	{
	case 0:
		Umrech = 1.0;
		decimals = 100;
		break;
	case 1:
		Umrech = 0.3527777;
		decimals = 1000;
		break;
	case 2:
		Umrech = 1.0 / 72.0;
		decimals = 10000;
		break;
	case 3:
		Umrech = 1.0 / 12.0;
		decimals = 100;
		break;
	}
	einheit = u;
	Breite->setValues(oldB * Umrech, oldBM * Umrech, decimals, Pagebr * Umrech);
	Hoehe->setValues(oldH * Umrech, oldHM * Umrech, decimals, Pageho * Umrech);
	RightR->setValues(0, Breite->value() - Left * Umrech, decimals, Right * Umrech);
	LeftR->setValues(0, Breite->value() - Right * Umrech, decimals, Left * Umrech);
	TopR->setValues(0, Hoehe->value() - Bottom * Umrech, decimals, Top * Umrech);
	BottomR->setValues(0, Hoehe->value() - Top * Umrech, decimals, Bottom * Umrech);
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
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	if (ori != Orient)
	{
		br = Breite->value();
		Breite->setValue(Hoehe->value());
		Hoehe->setValue(br);
	}
	if (ori == 0)
		Orient = 0;
	else
		Orient = 1;
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
	{
		setSize(ComboBox1->currentItem());
		setOrien(ComboBox2->currentItem());
	}
}

void NewDoc::setSize(int gr)
{
	Pagebr = Breite->value() / Umrech;
	Pageho = Hoehe->value() / Umrech;
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
	else
	{
		Pagebr = page_x[gr];
		Pageho = page_y[gr];
	}
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
	TextLabel6->setText(test == false ? tr("&Inside:") : tr("&Left:"));
	TextLabel8->setText(test == false ? tr("O&utside:") : tr("&Right:"));
	ErsteSeite->setEnabled(test == false ? true : false);
}
