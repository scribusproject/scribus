#include "reformdoc.h"
#include "reformdoc.moc"
#include <qtooltip.h>
#include <qcolordialog.h>
#include "scribusdoc.h"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

ReformDoc::ReformDoc( QWidget* parent, ScribusDoc* doc ) : PrefsDialogBase( parent )
{
	einheit = doc->Einheit;
	QString units[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	QString ein = units[doc->Einheit];
	int dp[] = {100, 1000, 10000, 100};
	int decimals = dp[doc->Einheit];
	int i=-1;
	Breite = doc->PageB * UmReFaktor;
	Hoehe = doc->PageH * UmReFaktor;
	setCaption( tr( "Document Setup" ) );
	prefsSelection->insertItem( tr("Page"));
	tabPage = new QWidget( prefsWidgets, "tab" );
	ReformDocLayout = new QVBoxLayout( tabPage );
	ReformDocLayout->setSpacing( 6 );
	ReformDocLayout->setMargin( 10 );
	dsGroupBox7 = new QGroupBox( tabPage, "GroupBox7" );
	dsGroupBox7->setTitle( tr( "Page Size" ) );
	dsGroupBox7->setColumnLayout(0, Qt::Vertical );
	dsGroupBox7->layout()->setSpacing( 0 );
	dsGroupBox7->layout()->setMargin( 0 );
	dsGroupBox7Layout = new QHBoxLayout( dsGroupBox7->layout() );
	dsGroupBox7Layout->setAlignment( Qt::AlignTop );
	dsGroupBox7Layout->setSpacing( 0 );
	dsGroupBox7Layout->setMargin( 10 );
	dsLayout4 = new QGridLayout;
	dsLayout4->setSpacing( 6 );
	dsLayout4->setMargin( 0 );
	sizeQComboBox = new QComboBox( true, dsGroupBox7, "sizeQComboBox" );
	sizeQLabel = new QLabel( sizeQComboBox, tr( "&Size:" ), dsGroupBox7, "sizeQLabel" );
	QString sizelist[] = {"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3", "B4",
	                      "B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", "Executive", "Folio",
	                      "Ledger", tr("Legal"), tr("Letter"), tr("Tabloid"), tr("Custom")};
	size_t const num_mappings = (sizeof sizelist)/(sizeof *sizelist);
	for (uint m = 0; m < num_mappings; ++m)
	{
		sizeQComboBox->insertItem(sizelist[m]);
		if (sizelist[m]==doc->PageSize)
			i=m;
	}
	//set Custom if we dont have one already as old docs wont have this attribute
	if (i==-1)
		i=num_mappings-1;
	sizeQComboBox->setEnabled(false);
	sizeQComboBox->setCurrentItem(i);
	dsLayout4->addWidget( sizeQLabel, 0, 0 );
	dsLayout4->addWidget( sizeQComboBox, 0, 1 );
	orientationQComboBox = new QComboBox( true, dsGroupBox7, "orientationQComboBox" );
	orientationQLabel = new QLabel( orientationQComboBox, tr( "Orie&ntation:" ), dsGroupBox7, "orientationQLabel" );
	orientationQComboBox->insertItem( tr( "Portrait" ) );
	orientationQComboBox->insertItem( tr( "Landscape" ) );
	orientationQComboBox->setEnabled(false);
	orientationQComboBox->setCurrentItem(doc->PageOri);
	dsLayout4->addWidget( orientationQLabel, 0, 2 );
	dsLayout4->addWidget( orientationQComboBox, 0, 3 );
	widthMSpinBox = new MSpinBox( 1, 10000, dsGroupBox7, 2 );
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7, "widthLabel" );
	widthMSpinBox->setEnabled( false );
	widthMSpinBox->setSuffix(ein);
	widthMSpinBox->setValue(Breite);
	widthQLabel->setBuddy(widthMSpinBox);
	dsLayout4->addWidget( widthQLabel, 1, 0 );
	dsLayout4->addWidget( widthMSpinBox, 1, 1 );
	heightMSpinBox = new MSpinBox( 1, 10000, dsGroupBox7, 2 );
	heightQLabel = new QLabel( tr( "&Height:" ), dsGroupBox7, "heightLabel" );
	heightMSpinBox->setEnabled( false );
	heightMSpinBox->setSuffix(ein);
	heightMSpinBox->setValue(Hoehe);
	heightQLabel->setBuddy(heightMSpinBox);
	dsLayout4->addWidget( heightQLabel, 1, 2 );
	dsLayout4->addWidget( heightMSpinBox, 1, 3 );
	dsGroupBox7Layout->addLayout( dsLayout4 );
	ReformDocLayout->addWidget( dsGroupBox7 );
	GroupBox7 = new QGroupBox( tabPage, "GroupBox7" );
	GroupBox7->setTitle( tr( "Margin Guides" ) );
	GroupBox7->setColumnLayout(0, Qt::Vertical );
	GroupBox7->layout()->setSpacing( 0 );
	GroupBox7->layout()->setMargin( 0 );
	GroupBox7Layout = new QHBoxLayout( GroupBox7->layout() );
	GroupBox7Layout->setAlignment( Qt::AlignTop );
	GroupBox7Layout->setSpacing( 0 );
	GroupBox7Layout->setMargin( 10 );
	Layout4 = new QGridLayout;
	Layout4->setSpacing( 6 );
	Layout4->setMargin( 0 );
	TopR = new MSpinBox( GroupBox7, 4 );
	TopR->setSuffix( ein );
	TopR->setDecimals( decimals );
	TopR->setMaxValue(Hoehe);
	TopR->setValue(doc->PageM.Top * UmReFaktor);
	Layout4->addWidget( TopR, 0, 1 );
	TextLabel5 = new QLabel( tr( "&Top:" ), GroupBox7, "TextLabel5" );
	TextLabel5->setBuddy(TopR);
	Layout4->addWidget( TextLabel5, 0, 0 );
	LeftR = new MSpinBox( GroupBox7, 4 );
	LeftR->setSuffix( ein );
	LeftR->setDecimals( decimals );
	LeftR->setMaxValue(Breite);
	LeftR->setValue(doc->PageM.Left * UmReFaktor);
	Layout4->addWidget( LeftR, 0, 3 );
	Links = new QLabel( tr( "&Left:" ), GroupBox7, "Links" );
	Links->setBuddy(LeftR);
	Layout4->addWidget( Links, 0, 2 );
	BottomR = new MSpinBox( GroupBox7, 4 );
	BottomR->setSuffix( ein );
	BottomR->setDecimals( decimals );
	BottomR->setMaxValue(Hoehe);
	BottomR->setValue(doc->PageM.Bottom * UmReFaktor);
	Layout4->addWidget( BottomR, 1, 1 );
	TextLabel7 = new QLabel( tr( "&Bottom:" ), GroupBox7, "TextLabel7" );
	TextLabel7->setBuddy(BottomR);
	Layout4->addWidget( TextLabel7, 1, 0 );
	RightR = new MSpinBox( GroupBox7, 4 );
	RightR->setSuffix( ein );
	RightR->setDecimals( decimals );
	RightR->setMaxValue(Breite);
	RightR->setValue(doc->PageM.Right * UmReFaktor);
	Layout4->addWidget( RightR, 1, 3 );
	Rechts = new QLabel( tr( "&Right:" ), GroupBox7, "Rechts" );
	Rechts->setBuddy(RightR);
	Layout4->addWidget( Rechts, 1, 2 );
	Doppelseiten = new QCheckBox( tr( "&Facing Pages" ),GroupBox7, "Doppelseiten" );
	Doppelseiten->setChecked( doc->PageFP );
	Layout4->addMultiCellWidget( Doppelseiten, 2, 2, 0, 1 );
	ErsteSeite = new QCheckBox( tr( "Left &Page First" ), GroupBox7, "n" );
	ErsteSeite->setChecked( doc->FirstPageLeft );
	Layout4->addMultiCellWidget( ErsteSeite, 2, 2, 2, 3 );
	if (!doc->PageFP)
		ErsteSeite->setEnabled(false);
	setDS();
	TextLabel1_3 = new QLabel( tr( "F&irst Page Number:" ), GroupBox7, "TextLabel1_3" );
	Layout4->addMultiCellWidget( TextLabel1_3, 3, 3, 0, 1 );
	PgNr = new QSpinBox( GroupBox7, "PgNr" );
	PgNr->setMaxValue( 1000 );
	PgNr->setMinValue( 1 );
	PgNr->setValue(doc->FirstPnum);
	Layout4->addWidget( PgNr, 3, 2, Qt::AlignRight );
	TextLabel1_3->setBuddy(PgNr);
	GroupBox7Layout->addLayout( Layout4 );
	ReformDocLayout->addWidget( GroupBox7 );
	prefsWidgets->addWidget( tabPage, 0 );

	prefsSelection->insertItem( tr("Guides"));
	tabGuides = new QWidget( prefsWidgets, "tabView" );
	tabGuidesLayout = new QVBoxLayout( tabGuides, 10, 5, "tabViewLayout");
	checkGrid = new QGroupBox( tabGuides, "checkGrid" );
	checkGrid->setTitle( tr( "ShowGrid" ) );
	checkGrid->setCheckable( TRUE );
	checkGrid->setChecked(doc->GridShown);
	checkGrid->setColumnLayout(0, Qt::Vertical );
	checkGrid->layout()->setSpacing( 5 );
	checkGrid->layout()->setMargin( 10 );
	checkGridLayout = new QGridLayout( checkGrid->layout() );
	checkGridLayout->setAlignment( Qt::AlignTop );
	groupBox1 = new QGroupBox( checkGrid, "groupBox1" );
	groupBox1->setColumnLayout(0, Qt::Vertical );
	groupBox1->layout()->setSpacing( 5 );
	groupBox1->layout()->setMargin( 10 );
	groupBox1->setTitle( tr( "Major Grid" ) );
	groupBox1Layout = new QGridLayout( groupBox1->layout() );
	groupBox1Layout->setAlignment( Qt::AlignTop );
	majorGridColor = new QPushButton( groupBox1, "majorGridColor" );
	majorGridColor->setMinimumSize( QSize( 60, 20 ) );
	majorGridColor->setMaximumSize( QSize( 60, 20 ) );
	majorGridColor->setFlat( false );
	majorGridColor->setAutoDefault( false );
	QPixmap pm1 = QPixmap(54, 14);
	pm1.fill(doc->majorColor);
	colorMajorGrid = doc->majorColor;
	majorGridColor->setPixmap(pm1);
	majorGridColor->setText( QString::null );
	groupBox1Layout->addWidget( majorGridColor, 1, 1 );
	textLabel4 = new QLabel( groupBox1, "textLabel4" );
	textLabel4->setText( tr( "Color:" ) );
	groupBox1Layout->addWidget( textLabel4, 1, 0 );
	textLabel6 = new QLabel( groupBox1, "textLabel6" );
	textLabel6->setText( tr( "Spacing:" ) );
	groupBox1Layout->addWidget( textLabel6, 0, 0 );
	majorSpace = new MSpinBox( 1, 10000, groupBox1, 2 );
	majorSpace->setValue( doc->majorGrid * UmReFaktor );
	majorSpace->setSuffix( ein );
	groupBox1Layout->addWidget( majorSpace, 0, 1 );
	checkGridLayout->addWidget( groupBox1, 0, 0 );
	groupBox2 = new QGroupBox( checkGrid, "groupBox2" );
	groupBox2->setColumnLayout(0, Qt::Vertical );
	groupBox2->layout()->setSpacing( 5 );
	groupBox2->layout()->setMargin( 10 );
	groupBox2->setTitle( tr( "Minor Grid" ) );
	groupBox2Layout = new QGridLayout( groupBox2->layout() );
	groupBox2Layout->setAlignment( Qt::AlignTop );
	textLabel5 = new QLabel( groupBox2, "textLabel5" );
	textLabel5->setText( tr( "Color:" ) );
	groupBox2Layout->addWidget( textLabel5, 1, 0 );
	minorGridColor = new QPushButton( groupBox2, "minorGridColor" );
	minorGridColor->setMinimumSize( QSize( 60, 20 ) );
	minorGridColor->setMaximumSize( QSize( 60, 20 ) );
	minorGridColor->setFlat( false );
	minorGridColor->setAutoDefault( false );
	QPixmap pm = QPixmap(54, 14);
	pm.fill(doc->minorColor);
	colorMinorGrid = doc->minorColor;
	minorGridColor->setPixmap(pm);
	minorGridColor->setText( QString::null );
	groupBox2Layout->addWidget( minorGridColor, 1, 1 );
	textLabel7 = new QLabel( groupBox2, "textLabel7" );
	textLabel7->setText( tr( "Spacing:" ) );
	groupBox2Layout->addWidget( textLabel7, 0, 0 );
	minorSpace = new MSpinBox( 1, 10000, groupBox2, 2 );
	minorSpace->setValue( doc->minorGrid  * UmReFaktor);
	minorSpace->setSuffix( ein );
	groupBox2Layout->addWidget( minorSpace, 0, 1 );
	checkGridLayout->addWidget( groupBox2, 0, 1 );
	tabGuidesLayout->addWidget( checkGrid );
	layout11 = new QGridLayout( 0, 1, 1, 0, 5, "layout11");
	textLabel8 = new QLabel( tabGuides, "textLabel8" );
	textLabel8->setText( tr( "Guide Snap Distance:" ) );
	layout11->addWidget( textLabel8, 0, 0 );
	snapDistance = new MSpinBox( 1, 10000, tabGuides, 2 );
	snapDistance->setValue( doc->GuideRad * UmReFaktor );
	snapDistance->setSuffix( ein );
	layout11->addWidget( snapDistance, 0, 1, Qt::AlignLeft );
	checkMargin = new QCheckBox( tabGuides, "checkMargin" );
	checkMargin->setText( tr( "Show Margins" ) );
	checkMargin->setChecked(doc->MarginsShown);
	layout11->addWidget( checkMargin, 1, 0 );
	checkGuides = new QCheckBox( tabGuides, "checkGuides" );
	checkGuides->setText( tr( "Show Guides" ) );
	checkGuides->setChecked(doc->GuidesShown);
	layout11->addWidget( checkGuides, 2, 0 );
	checkBaseline = new QCheckBox( tabGuides, "checkBaseline" );
	checkBaseline->setText( tr( "Show Baseline Grid" ) );
	checkBaseline->setChecked(doc->BaseShown);
	layout11->addWidget( checkBaseline, 3, 0 );
	layout2 = new QHBoxLayout( 0, 0, 5, "layout2");
	textLabel2 = new QLabel( tabGuides, "textLabel2" );
	textLabel2->setText( tr( "Color:" ) );
	layout2->addWidget( textLabel2 );
	baselineColor = new QPushButton( tabGuides, "baselineColor" );
	baselineColor->setMinimumSize( QSize( 60, 20 ) );
	baselineColor->setMaximumSize( QSize( 60, 20 ) );
	baselineColor->setFlat( false );
	baselineColor->setAutoDefault( false );
	QPixmap pm4 = QPixmap(54, 14);
	pm4.fill(doc->baseColor);
	colorBaselineGrid = doc->baseColor;
	baselineColor->setPixmap(pm4);
	baselineColor->setText( QString::null );
	layout2->addWidget( baselineColor );
	layout11->addLayout( layout2, 3, 1 );
	layout3 = new QHBoxLayout( 0, 0, 5, "layout3");
	textLabel3 = new QLabel( tabGuides, "textLabel3" );
	textLabel3->setText( tr( "Color:" ) );
	layout3->addWidget( textLabel3 );
	guideColor = new QPushButton( tabGuides, "guideColor" );
	guideColor->setMinimumSize( QSize( 60, 20 ) );
	guideColor->setMaximumSize( QSize( 60, 20 ) );
	guideColor->setFlat( false );
	guideColor->setAutoDefault( false );
	QPixmap pm3 = QPixmap(54, 14);
	pm3.fill(doc->guideColor);
	colorGuides = doc->guideColor;
	guideColor->setPixmap(pm3);
	guideColor->setText( QString::null );
	layout3->addWidget( guideColor );
	layout11->addLayout( layout3, 2, 1 );
	layout9 = new QHBoxLayout( 0, 0, 5, "layout9");
	textLabel1 = new QLabel( tabGuides, "textLabel1" );
	textLabel1->setText( tr( "Color:" ) );
	layout9->addWidget( textLabel1 );
	marginColor = new QPushButton( tabGuides, "marginColor" );
	marginColor->setMinimumSize( QSize( 60, 20 ) );
	marginColor->setMaximumSize( QSize( 60, 20 ) );
	marginColor->setAutoDefault( false );
	QPixmap pm6 = QPixmap(54, 14);
	pm6.fill(doc->margColor);
	colorMargin = doc->margColor;
	marginColor->setPixmap(pm6);
	marginColor->setFlat( false );
	marginColor->setText( QString::null );
	layout9->addWidget( marginColor );
	layout11->addLayout( layout9, 1, 1 );
	tabGuidesLayout->addLayout( layout11 );
	layout12 = new QGridLayout( 0, 1, 1, 0, 5, "layout12");
	checkLink = new QCheckBox( tabGuides, "checkLink" );
	checkLink->setText( tr( "Show Text Chains" ) );
	checkLink->setChecked(doc->linkShown);
	layout12->addWidget( checkLink, 1, 0 );
	checkFrame = new QCheckBox( tabGuides, "checkFrame" );
	checkFrame->setText( tr( "Show Frames" ) );
	checkFrame->setChecked(doc->FramesShown);
	layout12->addWidget( checkFrame, 0, 0 );
	checkPictures = new QCheckBox( tabGuides, "checkPictures" );
	checkPictures->setText( tr( "Show Pictures" ) );
	layout12->addWidget( checkPictures, 0, 1 );
	checkPictures->setChecked(doc->ShowPic);
	tabGuidesLayout->addLayout( layout12 );
	prefsWidgets->addWidget( tabGuides, 1 );

	prefsSelection->insertItem( tr("View"));
	tabView = new QWidget( prefsWidgets, "tabView" );
	tabViewLayout = new QVBoxLayout( tabView, 10, 5, "tabViewLayout");
	pageBackground = new QButtonGroup( tabView, "pageBackground" );
	pageBackground->setTitle( tr( "Page Background" ) );
	pageBackground->setColumnLayout(0, Qt::Vertical );
	pageBackground->layout()->setSpacing( 5 );
	pageBackground->layout()->setMargin( 10 );
	pageBackgroundLayout = new QVBoxLayout( pageBackground->layout() );
	pageBackgroundLayout->setAlignment( Qt::AlignTop );
	layout10 = new QHBoxLayout( 0, 0, 5, "layout10");
	textLabel9 = new QLabel( pageBackground, "textLabel9" );
	textLabel9->setText( tr( "Color:" ) );
	layout10->addWidget( textLabel9 );
	backColor = new QPushButton( pageBackground, "backColor" );
	backColor->setMinimumSize( QSize( 60, 20 ) );
	backColor->setMaximumSize( QSize( 60, 20 ) );
	backColor->setFlat( false );
	backColor->setAutoDefault( false );
	QPixmap pm5 = QPixmap(54, 14);
	pm5.fill(doc->papColor);
	colorPaper = doc->papColor;
	backColor->setPixmap(pm5);
	backColor->setText( QString::null );
	layout10->addWidget( backColor );
	spacer3 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout10->addItem( spacer3 );
	pageBackgroundLayout->addLayout( layout10 );
	checkUnprintable = new QCheckBox( pageBackground, "checkUnprintable" );
	checkUnprintable->setText( tr( "Display &Unprintable Area in Margin Color" ) );
	checkUnprintable->setAccel( QKeySequence( tr( "Alt+U" ) ) );
	checkUnprintable->setChecked( doc->RandFarbig );
	pageBackgroundLayout->addWidget( checkUnprintable );
	tabViewLayout->addWidget( pageBackground );
	buttonGroup1 = new QButtonGroup( tabView, "buttonGroup1" );
	buttonGroup1->setExclusive( true );
	buttonGroup1->setColumnLayout(0, Qt::Vertical );
	buttonGroup1->layout()->setSpacing( 5 );
	buttonGroup1->layout()->setMargin( 10 );
	buttonGroup1->setTitle( tr( "Placing of Guides" ) );
	buttonGroup1Layout = new QVBoxLayout( buttonGroup1->layout() );
	buttonGroup1Layout->setAlignment( Qt::AlignTop );
	inBackground = new QRadioButton( buttonGroup1, "inBackground" );
	inBackground->setText( tr( "In the Background" ) );
	buttonGroup1Layout->addWidget( inBackground );
	inForeground = new QRadioButton( buttonGroup1, "inForeground" );
	inForeground->setText( tr( "In the Foreground" ) );
	buttonGroup1Layout->addWidget( inForeground );
	inBackground->setChecked( doc->Before );
	inForeground->setChecked( !doc->Before );
	tabViewLayout->addWidget( buttonGroup1 );
	prefsWidgets->addWidget( tabView, 2 );
	
	
	RightR->setMaxValue(Breite - LeftR->value());
	LeftR->setMaxValue(Breite - RightR->value());
	TopR->setMaxValue(Hoehe - BottomR->value());
	BottomR->setMaxValue(Hoehe - TopR->value());
	//tooltips
	QToolTip::add( Doppelseiten, tr( "Enable single or spread based layout" ) );
	QToolTip::add( ErsteSeite, tr( "Make the first page the left page of the document" ) );
	QToolTip::add( TopR, tr( "Distance between the top margin guide and the edge of the page" ) );
	QToolTip::add( BottomR, tr( "Distance between the bottom margin guide and the edge of the page" ) );
	QToolTip::add( LeftR, tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( RightR, tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );

	// signals and slots connections
	connect( Doppelseiten, SIGNAL( clicked() ), this, SLOT( setDS() ) );
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	connect(majorGridColor, SIGNAL(clicked()), this, SLOT(changeMajorColor()));
	connect(minorGridColor, SIGNAL(clicked()), this, SLOT(changeMinorColor()));
	connect(baselineColor, SIGNAL(clicked()), this, SLOT(changeBaselineColor()));
	connect(guideColor, SIGNAL(clicked()), this, SLOT(changeGuideColor()));
	connect(marginColor, SIGNAL(clicked()), this, SLOT(changeMarginColor()));
	connect(backColor, SIGNAL(clicked()), this, SLOT(changePaperColor()));
	prefsWidgets->raiseWidget(0);
	resize( minimumSizeHint() );
	clearWState( WState_Polished );
}

void ReformDoc::changeMajorColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorMajorGrid, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorMajorGrid = neu;
		majorGridColor->setPixmap(pm);
	}
}

void ReformDoc::changeMinorColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorMinorGrid, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorMinorGrid = neu;
		minorGridColor->setPixmap(pm);
	}
}

void ReformDoc::changePaperColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorPaper, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorPaper = neu;
		backColor->setPixmap(pm);
	}
}

void ReformDoc::changeBaselineColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorBaselineGrid, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorBaselineGrid = neu;
		baselineColor->setPixmap(pm);
	}
}

void ReformDoc::changeGuideColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorGuides, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorGuides = neu;
		guideColor->setPixmap(pm);
	}
}

void ReformDoc::changeMarginColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorMargin, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorMargin = neu;
		marginColor->setPixmap(pm);
	}
}

void ReformDoc::setTop(int )
{
	BottomR->setMaxValue(Hoehe - TopR->value());
}

void ReformDoc::setBottom(int )
{
	TopR->setMaxValue(Hoehe - BottomR->value());
}

void ReformDoc::setLeft(int )
{
	RightR->setMaxValue(Breite - LeftR->value());
}

void ReformDoc::setRight(int )
{
	LeftR->setMaxValue(Breite - RightR->value());
}

void ReformDoc::setDS()
{
	if (Doppelseiten->isChecked())
	{
		Links->setText( tr( "&Inside:" ) );
		Rechts->setText( tr( "&Outside:" ) );
		ErsteSeite->setEnabled(true);
	}
	else
	{
		Links->setText( tr( "&Left:" ) );
		Rechts->setText( tr( "&Right:" ) );
		ErsteSeite->setEnabled(false);
	}
}
