#include "reformdoc.h"
#include "reformdoc.moc"
#include <qtooltip.h>
#include <qcolordialog.h>
#include "fontcombo.h"
#include "polygonwidget.h"
extern QPixmap loadIcon(QString nam);
extern QPixmap fontSamples(QString da, int s, QString ts, QColor back);
extern double UmReFaktor;

ReformDoc::ReformDoc( QWidget* parent, ScribusDoc* doc, preV *prefsData ) : PrefsDialogBase( parent )
{
	einheit = doc->Einheit;
	fon = &prefsData->AvailFonts;
	QString units[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	QString ein = units[doc->Einheit];
	int dp[] = {100, 1000, 10000, 100};
	int decimals = dp[doc->Einheit];
	int i=-1;
	pageWidth = doc->PageB * UmReFaktor;
	pageHeight = doc->PageH * UmReFaktor;
	setCaption( tr( "Document Setup" ) );
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
	widthMSpinBox->setValue(pageWidth);
	widthQLabel->setBuddy(widthMSpinBox);
	dsLayout4->addWidget( widthQLabel, 1, 0 );
	dsLayout4->addWidget( widthMSpinBox, 1, 1 );
	heightMSpinBox = new MSpinBox( 1, 10000, dsGroupBox7, 2 );
	heightQLabel = new QLabel( tr( "&Height:" ), dsGroupBox7, "heightLabel" );
	heightMSpinBox->setEnabled( false );
	heightMSpinBox->setSuffix(ein);
	heightMSpinBox->setValue(pageHeight);
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
	topR = new MSpinBox( GroupBox7, 4 );
	topR->setSuffix( ein );
	topR->setDecimals( decimals );
	topR->setMaxValue(pageHeight);
	topR->setValue(doc->PageM.Top * UmReFaktor);
	Layout4->addWidget( topR, 0, 1 );
	TextLabel5 = new QLabel( tr( "&Top:" ), GroupBox7, "TextLabel5" );
	TextLabel5->setBuddy(topR);
	Layout4->addWidget( TextLabel5, 0, 0 );
	leftR = new MSpinBox( GroupBox7, 4 );
	leftR->setSuffix( ein );
	leftR->setDecimals( decimals );
	leftR->setMaxValue(pageWidth);
	leftR->setValue(doc->PageM.Left * UmReFaktor);
	Layout4->addWidget( leftR, 0, 3 );
	Links = new QLabel( tr( "&Left:" ), GroupBox7, "Links" );
	Links->setBuddy(leftR);
	Layout4->addWidget( Links, 0, 2 );
	bottomR = new MSpinBox( GroupBox7, 4 );
	bottomR->setSuffix( ein );
	bottomR->setDecimals( decimals );
	bottomR->setMaxValue(pageHeight);
	bottomR->setValue(doc->PageM.Bottom * UmReFaktor);
	Layout4->addWidget( bottomR, 1, 1 );
	TextLabel7 = new QLabel( tr( "&Bottom:" ), GroupBox7, "TextLabel7" );
	TextLabel7->setBuddy(bottomR);
	Layout4->addWidget( TextLabel7, 1, 0 );
	rightR = new MSpinBox( GroupBox7, 4 );
	rightR->setSuffix( ein );
	rightR->setDecimals( decimals );
	rightR->setMaxValue(pageWidth);
	rightR->setValue(doc->PageM.Right * UmReFaktor);
	Layout4->addWidget( rightR, 1, 3 );
	Rechts = new QLabel( tr( "&Right:" ), GroupBox7, "Rechts" );
	Rechts->setBuddy(rightR);
	Layout4->addWidget( Rechts, 1, 2 );
	facingPages = new QCheckBox( tr( "&Facing Pages" ),GroupBox7, "facingPages" );
	facingPages->setChecked( doc->PageFP );
	Layout4->addMultiCellWidget( facingPages, 2, 2, 0, 1 );
	firstPage = new QCheckBox( tr( "Left &Page First" ), GroupBox7, "n" );
	firstPage->setChecked( doc->FirstPageLeft );
	Layout4->addMultiCellWidget( firstPage, 2, 2, 2, 3 );
	if (!doc->PageFP)
		firstPage->setEnabled(false);
	setDS();
	TextLabel1_3 = new QLabel( tr( "F&irst Page Number:" ), GroupBox7, "TextLabel1_3" );
	Layout4->addMultiCellWidget( TextLabel1_3, 3, 3, 0, 1 );
	pageNumber = new QSpinBox( GroupBox7, "pageNumber" );
	pageNumber->setMaxValue( 10000 );
	pageNumber->setMinValue( 1 );
	pageNumber->setValue(doc->FirstPnum);
	Layout4->addWidget( pageNumber, 3, 2, Qt::AlignRight );
	TextLabel1_3->setBuddy(pageNumber);
	GroupBox7Layout->addLayout( Layout4 );
	ReformDocLayout->addWidget( GroupBox7 );
	addItem( tr("Page"), loadIcon("page.png"), tabPage);

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
	addItem( tr("Guides"), loadIcon("guides.png"), tabGuides);

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
	addItem( tr("Display"), loadIcon("screen.png"), tabView);


	tabTypo = new QWidget( prefsWidgets, "tabTypo" );
	tabTypoLayout = new QGridLayout( tabTypo, 1, 1, 11, 6, "tabTypoLayout");
	groupBox1a = new QGroupBox( tabTypo, "groupBox1a" );
	groupBox1a->setColumnLayout(0, Qt::Vertical );
	groupBox1a->layout()->setSpacing( 6 );
	groupBox1a->layout()->setMargin( 11 );
	groupBox1a->setTitle( tr( "Subscript" ) );
	groupBox1aLayout = new QGridLayout( groupBox1a->layout() );
	groupBox1aLayout->setAlignment( Qt::AlignTop );
	subDisplacement = new QSpinBox( groupBox1a, "subDisplacement" );
	subDisplacement->setMaxValue( 100 );
	subDisplacement->setValue( doc->VTief );
	subDisplacement->setSuffix( tr( " %" ) );
	groupBox1aLayout->addWidget( subDisplacement, 0, 1 );
	textLabel1a = new QLabel(subDisplacement, tr( "&Displacement:" ), groupBox1a, "textLabel1a" );
	groupBox1aLayout->addWidget( textLabel1a, 0, 0 );
	subScaling = new QSpinBox( groupBox1a, "subScaling" );
	subScaling->setMaxValue( 100 );
	subScaling->setMinValue( 1 );
	subScaling->setValue( doc->VTiefSc );
	subScaling->setSuffix( tr( " %" ) );
	groupBox1aLayout->addWidget( subScaling, 0, 3 );
	textLabel2a = new QLabel(subScaling, tr( "&Scaling:" ), groupBox1a, "textLabel2a" );
	groupBox1aLayout->addWidget( textLabel2a, 0, 2 );
	tabTypoLayout->addWidget( groupBox1a, 0, 0 );
	groupBox2a = new QGroupBox( tabTypo, "groupBox2a" );
	groupBox2a->setColumnLayout(0, Qt::Vertical );
	groupBox2a->layout()->setSpacing( 6 );
	groupBox2a->layout()->setMargin( 11 );
	groupBox2a->setTitle( tr( "Superscript" ) );
	groupBox2aLayout = new QGridLayout( groupBox2a->layout() );
	groupBox2aLayout->setAlignment( Qt::AlignTop );
	superDisplacement = new QSpinBox( groupBox2a, "superDisplacement" );
	superDisplacement->setMaxValue( 100 );
	superDisplacement->setValue( doc->VHoch );
	superDisplacement->setSuffix( tr( " %" ) );
	groupBox2aLayout->addWidget( superDisplacement, 0, 1 );
	textLabel3a = new QLabel(superDisplacement, tr( "D&isplacement:" ), groupBox2a, "textLabel3a" );
	groupBox2aLayout->addWidget( textLabel3a, 0, 0 );
	superScaling = new QSpinBox( groupBox2a, "superScaling" );
	superScaling->setMaxValue( 100 );
	superScaling->setMinValue( 1 );
	superScaling->setValue( doc->VHochSc );
	superScaling->setSuffix( tr( " %" ) );
	groupBox2aLayout->addWidget( superScaling, 0, 3 );
	textLabel4a = new QLabel(superScaling, tr( "S&caling:" ), groupBox2a, "textLabel4a" );
	groupBox2aLayout->addWidget( textLabel4a, 0, 2 );
	tabTypoLayout->addWidget( groupBox2a, 1, 0 );
	groupBox3a = new QGroupBox( tabTypo, "groupBox3a" );
	groupBox3a->setColumnLayout(0, Qt::Vertical );
	groupBox3a->layout()->setSpacing( 6 );
	groupBox3a->layout()->setMargin( 11 );
	groupBox3a->setTitle( tr( "Small Caps" ) );
	groupBox3aLayout = new QGridLayout( groupBox3a->layout() );
	groupBox3aLayout->setAlignment( Qt::AlignTop );
	capsScaling = new QSpinBox( groupBox3a, "capsScaling" );
	capsScaling->setMaxValue( 100 );
	capsScaling->setMinValue( 1 );
	capsScaling->setValue( doc->VKapit );
	capsScaling->setSuffix( tr( " %" ) );
	groupBox3aLayout->addWidget( capsScaling, 0, 1 );
	textLabel5a = new QLabel(capsScaling, tr( "Sc&aling:" ), groupBox3a, "textLabel5a" );
	groupBox3aLayout->addWidget( textLabel5a, 0, 0 );
	tabTypoLayout->addWidget( groupBox3a, 2, 0 );
	groupBox4a = new QGroupBox( tabTypo, "groupBox4a" );
	groupBox4a->setColumnLayout(0, Qt::Vertical );
	groupBox4a->layout()->setSpacing( 6 );
	groupBox4a->layout()->setMargin( 11 );
	groupBox4a->setTitle( tr( "Other" ) );
	groupBox4aLayout = new QGridLayout( groupBox4a->layout() );
	groupBox4aLayout->setAlignment( Qt::AlignTop );
	baseGrid = new MSpinBox( 1, doc->PageH, groupBox4a, 1 );
	baseGrid->setSuffix( ein );
	baseGrid->setValue(doc->BaseGrid * UmReFaktor);
	groupBox4aLayout->addWidget( baseGrid, 0, 1 );
	textLabel6a = new QLabel(baseGrid, tr( "Baseline &Grid:" ),groupBox4a, "textLabel6a" );
	groupBox4aLayout->addWidget( textLabel6a, 0, 0 );
	baseOffset = new MSpinBox( 0, doc->PageH, groupBox4a, 1 );
	baseOffset->setSuffix( ein );
	baseOffset->setValue(doc->BaseOffs * UmReFaktor);
	groupBox4aLayout->addWidget( baseOffset, 1, 1 );
	textLabel7a = new QLabel(baseOffset, tr( "Baseline &Offset:" ),groupBox4a, "textLabel7a" );
	groupBox4aLayout->addWidget( textLabel7a, 1, 0 );
	autoLine = new QSpinBox( groupBox4a, "autoLine" );
	autoLine->setMaxValue( 100 );
	autoLine->setMinValue( 1 );
	autoLine->setValue( doc->AutoLine );
	autoLine->setSuffix( tr( " %" ) );
	groupBox4aLayout->addWidget( autoLine, 2, 1 );
	textLabel8a = new QLabel( autoLine, tr( "Automatic &Line Spacing:" ), groupBox4a, "textLabel8a" );
	groupBox4aLayout->addWidget( textLabel8a, 2, 0 );
	tabTypoLayout->addWidget( groupBox4a, 3, 0 );
	addItem( tr("Typography"), loadIcon("font.png"), tabTypo);

	tabTools = new QWidget( prefsWidgets, "tabTools" );
	tabToolsLayout = new QHBoxLayout( tabTools, 11, 6, "tabToolsLayout");
	buttonGroupTools = new QButtonGroup( tabTools, "buttonGroupTools" );
	buttonGroupTools->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, buttonGroupTools->sizePolicy().hasHeightForWidth() ) );
	buttonGroupTools->setExclusive( true );
	buttonGroupTools->setRadioButtonExclusive( true );
	buttonGroupTools->setColumnLayout(0, Qt::Vertical );
	buttonGroupTools->layout()->setSpacing( 5 );
	buttonGroupTools->layout()->setMargin( 5 );
	buttonGroupTools->setTitle( QString::null );
	buttonGroupToolsLayout = new QVBoxLayout( buttonGroupTools->layout() );
	buttonGroupToolsLayout->setAlignment( Qt::AlignTop );
	toolText = new QToolButton( buttonGroupTools, "toolText" );
	toolText->setToggleButton( true );
	toolText->setText( QString::null );
	toolText->setIconSet( QIconSet( loadIcon("Text.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolText );
	toolImage = new QToolButton( buttonGroupTools, "toolImage" );
	toolImage->setToggleButton( true );
	toolImage->setText( QString::null );
	toolImage->setIconSet( QIconSet( loadIcon("Bild.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolImage );
	toolShape = new QToolButton( buttonGroupTools, "toolShape" );
	toolShape->setToggleButton( true );
	toolShape->setText( QString::null );
	toolShape->setIconSet( QIconSet( loadIcon("Rechtecke.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolShape);
	toolPoly = new QToolButton( buttonGroupTools, "toolPoly" );
	toolPoly->setToggleButton( true );
	toolPoly->setText( QString::null );
	toolPoly->setIconSet( QIconSet( loadIcon("spline.png") ) );
	buttonGroupToolsLayout->addWidget( toolPoly );
	toolLine = new QToolButton( buttonGroupTools, "toolLine" );
	toolLine->setToggleButton( true );
	toolLine->setText( QString::null );
	toolLine->setIconSet( QIconSet( loadIcon("Stift.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolLine );
	toolZoom = new QToolButton( buttonGroupTools, "toolZoom" );
	toolZoom->setToggleButton( true );
	toolZoom->setText( QString::null );
	toolZoom->setIconSet( QIconSet( loadIcon("Lupe.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolZoom );
	tabToolsLayout->addWidget( buttonGroupTools );
	subStackTools = new QWidgetStack( tabTools, "subStackTools" );
	subStackTools->setFrameShape( QWidgetStack::GroupBoxPanel );
	subStackTools->setFrameShadow( QWidgetStack::Sunken );
	subTabText = new QWidget( subStackTools, "subTabText" );
	subTabTextLayout = new QGridLayout( subTabText, 1, 1, 11, 6, "subTabTextLayout");
	textLabel3b = new QLabel( subTabText, "textLabel3b" );
	textLabel3b->setText( tr( "Color:" ) );
	subTabTextLayout->addWidget( textLabel3b, 2, 0 );
	textLabel1b = new QLabel( subTabText, "textLabel1b" );
	textLabel1b->setText( tr( "Font:" ) );
	subTabTextLayout->addWidget( textLabel1b, 0, 0 );
	textLabel2b = new QLabel( subTabText, "textLabel2b" );
	textLabel2b->setText( tr( "Size:" ) );
	subTabTextLayout->addWidget( textLabel2b, 1, 0 );
	columnsText = new QSpinBox( subTabText, "columnsText" );
	columnsText->setMinValue( 1 );
	columnsText->setMaxValue(100);
	columnsText->setValue(doc->DCols);
	subTabTextLayout->addWidget( columnsText, 3, 1 );
	textLabel4b = new QLabel(columnsText, tr("Colu&mns:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel4b, 3, 0 );
	previewText = new QLabel( tr( "Woven silk pyjamas exchanged for blue quartz" ), subTabText, "previewText" );
	previewText->setMinimumSize( QSize( 280, 70 ) );
	previewText->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	subTabTextLayout->addMultiCellWidget( previewText, 4, 4, 0, 3 );
	fontComboText = new FontCombo(subTabText, prefsData);
	for (int fc=0; fc<fontComboText->count(); ++fc)
	{
		if (fontComboText->text(fc) == doc->Dfont)
		{
			fontComboText->setCurrentItem(fc);
			break;
		}
	}
	subTabTextLayout->addMultiCellWidget( fontComboText, 0, 0, 1, 3 );
	sizeComboText = new QComboBox( true, subTabText, "SizeCombo" );
	sizeComboText->setEditable(false);
	QString ar_sizes[] = {" 7", " 9", "10", "11", "12", "14", "18", "24", "36", "48", "60", "72"};
	size_t f_size = sizeof(ar_sizes) / sizeof(*ar_sizes);
	for (uint s = 0; s < f_size; ++s)
		sizeComboText->insertItem(ar_sizes[s] + tr(" pt"));
	for (int a = 0; a < sizeComboText->count(); ++a)
	{
		if (sizeComboText->text(a).left(2).toInt() == doc->Dsize / 10)
			sizeComboText->setCurrentItem(a);
	}
	subTabTextLayout->addMultiCellWidget( sizeComboText, 1, 1, 1, 3 );
	colorComboText = new QComboBox( true, subTabText, "colorComboText" );
	colorComboText->setEditable(false);
	QPixmap pmT2;
	pmT2 = QPixmap(15, 15);
	CListe::Iterator itc;
	if (doc->DpenText == "None")
		colorComboText->setCurrentItem(colorComboText->count()-1);
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		pmT2.fill(doc->PageColors[itc.key()].getRGBColor());
		colorComboText->insertItem(pmT2, itc.key());
		if (itc.key() == doc->DpenText)
			colorComboText->setCurrentItem(colorComboText->count()-1);
	}
	subTabTextLayout->addMultiCellWidget( colorComboText, 2, 2, 1, 3 );
	gapText = new MSpinBox( 0, 200, subTabText, decimals );
	gapText->setSuffix( tr( " pt" ) );
	gapText->setValue(doc->DGap);
	subTabTextLayout->addWidget( gapText, 3, 3 );
	textLabel5b = new QLabel(gapText, tr("&Gap:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel5b, 3, 2 );
	subStackTools->addWidget( subTabText, 0 );

	subTabShape = new QWidget( subStackTools, "subTabShape" );
	subTabShapeLayout = new QGridLayout( subTabShape, 1, 1, 11, 6, "subTabShapeLayout");
	colorComboLineShape = new QComboBox( true, subTabShape, "colorComboLineShape" );
	colorComboLineShape->setEditable(false);
	colorComboLineShape->insertItem( tr("None"));
	if (doc->Dpen == "None")
		colorComboLineShape->setCurrentItem(colorComboLineShape->count()-1);
	QPixmap pm2S = QPixmap(15, 15);
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		pm2S.fill(doc->PageColors[itc.key()].getRGBColor());
		colorComboLineShape->insertItem(pm2S, itc.key());
		if (itc.key() == doc->Dpen)
			colorComboLineShape->setCurrentItem(colorComboLineShape->count()-1);
	}
	subTabShapeLayout->addWidget( colorComboLineShape, 0, 1 );
	textLabel7b = new QLabel( colorComboLineShape, tr( "&Line Color:" ), subTabShape, "textLabel7b" );
	subTabShapeLayout->addWidget( textLabel7b, 0, 0 );
	shadingLineShape = new QSpinBox( subTabShape, "shadingLineShape" );
	shadingLineShape->setMaxValue( 100 );
	shadingLineShape->setSuffix( tr( " %" ) );
	shadingLineShape->setMinValue( 0 );
	shadingLineShape->setValue(doc->Dshade2);
	subTabShapeLayout->addWidget( shadingLineShape, 1, 1, Qt::AlignLeft );
	textLabel8b = new QLabel( shadingLineShape, tr( "&Shading:" ), subTabShape, "textLabel8b" );
	subTabShapeLayout->addWidget( textLabel8b, 1, 0 );
	comboFillShape = new QComboBox( true, subTabShape, "comboFillShape" );
	comboFillShape->setEditable(false);
	comboFillShape->insertItem( tr("None"));
	if (doc->Dbrush == "None")
		comboFillShape->setCurrentItem(comboFillShape->count()-1);
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		pm2S.fill(doc->PageColors[itc.key()].getRGBColor());
		comboFillShape->insertItem(pm2S, itc.key());
		if (itc.key() == doc->Dbrush)
			comboFillShape->setCurrentItem(comboFillShape->count()-1);
	}
	subTabShapeLayout->addWidget( comboFillShape, 2, 1 );
	textLabel9b = new QLabel( comboFillShape, tr( "&Fill Color:" ), subTabShape, "textLabel9b" );
	subTabShapeLayout->addWidget( textLabel9b, 2, 0 );
	shadingFillShape = new QSpinBox( subTabShape, "shadingFillShape" );
	shadingFillShape->setMaxValue( 100 );
	shadingFillShape->setSuffix( tr( " %" ) );
	shadingFillShape->setMinValue( 0 );
	shadingFillShape->setValue(doc->Dshade);
	subTabShapeLayout->addWidget( shadingFillShape, 3, 1, Qt::AlignLeft );
	textLabel10b = new QLabel( shadingFillShape, tr( "S&hading:" ), subTabShape, "textLabel10b" );
	subTabShapeLayout->addWidget( textLabel10b, 3, 0 );
	comboStyleShape = new LineCombo(subTabShape);
	comboStyleShape->setEditable(false);
	switch (doc->DLineArt)
	{
	case SolidLine:
		comboStyleShape->setCurrentItem(0);
		break;
	case DashLine:
		comboStyleShape->setCurrentItem(1);
		break;
	case DotLine:
		comboStyleShape->setCurrentItem(2);
		break;
	case DashDotLine:
		comboStyleShape->setCurrentItem(3);
		break;
	case DashDotDotLine:
		comboStyleShape->setCurrentItem(4);
		break;
	default:
		comboStyleShape->setCurrentItem(0);
		break;
	}
	subTabShapeLayout->addWidget( comboStyleShape, 4, 1 );
	textLabel11b = new QLabel( comboStyleShape, tr( "Line Style:" ), subTabShape, "textLabel11b" );
	subTabShapeLayout->addWidget( textLabel11b, 4, 0 );
	lineWidthShape = new MSpinBox( 0, 36, subTabShape, 1 );
	lineWidthShape->setSuffix( tr( " pt" ) );
	lineWidthShape->setValue(doc->Dwidth);
	subTabShapeLayout->addWidget( lineWidthShape, 5, 1, Qt::AlignLeft );
	textLabel12b = new QLabel( lineWidthShape, tr( "Line &Width:" ), subTabShape, "TextLabel2_3_4" );
	subTabShapeLayout->addWidget( textLabel12b, 5, 0 );
	subStackTools->addWidget( subTabShape, 1 );

	subTabLine = new QWidget( subStackTools, "subTabLine" );
	subTabLineLayout = new QGridLayout( subTabLine, 1, 1, 11, 6, "subTabLineLayout");
	colorComboLine = new QComboBox( true, subTabLine, "colorComboLine" );
	colorComboLine->setEditable(false);
	QPixmap pm2L = QPixmap(15, 15);
	comboFillShape->insertItem( tr("None"));
	if (doc->DpenLine == "None")
		colorComboLine->setCurrentItem(colorComboLine->count()-1);
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		pm2L.fill(doc->PageColors[itc.key()].getRGBColor());
		colorComboLine->insertItem(pm2L, itc.key());
		if (itc.key() == doc->DpenLine)
			colorComboLine->setCurrentItem(colorComboLine->count()-1);
	}
	subTabLineLayout->addWidget( colorComboLine, 0, 1 );
	textLabel13b = new QLabel(colorComboLine, tr("&Line Color:"), subTabLine, "textLabel13b" );
	subTabLineLayout->addWidget( textLabel13b, 0, 0 );
	shadingLine = new QSpinBox( subTabLine, "shadingLine" );
	shadingLine->setMaxValue( 100 );
	shadingLine->setValue(doc->DshadeLine);
	shadingLine->setSuffix( tr( " %" ) );
	subTabLineLayout->addWidget( shadingLine, 1, 1, Qt::AlignLeft );
	textLabel14b = new QLabel(shadingLine, tr("&Shading:"), subTabLine, "textLabel14b" );
	subTabLineLayout->addWidget( textLabel14b, 1, 0 );
	comboStyleLine = new LineCombo(subTabLine);
	comboStyleLine->setEditable(false);
	switch (doc->DLstyleLine)
	{
	case SolidLine:
		comboStyleLine->setCurrentItem(0);
		break;
	case DashLine:
		comboStyleLine->setCurrentItem(1);
		break;
	case DotLine:
		comboStyleLine->setCurrentItem(2);
		break;
	case DashDotLine:
		comboStyleLine->setCurrentItem(3);
		break;
	case DashDotDotLine:
		comboStyleLine->setCurrentItem(4);
		break;
	default:
		comboStyleLine->setCurrentItem(0);
		break;
	}
	subTabLineLayout->addWidget( comboStyleLine, 2, 1 );
	textLabel15b = new QLabel(subTabLine, tr("Line S&tyle:"), subTabLine, "textLabel15b" );
	subTabLineLayout->addWidget( textLabel15b, 2, 0 );
	lineWidthLine = new MSpinBox( 1, 36, subTabLine, 1 );
	lineWidthLine->setSuffix( tr( " pt" ) );
	lineWidthLine->setValue(doc->DwidthLine);
	subTabLineLayout->addWidget( lineWidthLine, 3, 1, Qt::AlignLeft );
	textLabel16b = new QLabel(lineWidthLine, tr("Line &Width:"), subTabLine, "textLabel16b" );
	subTabLineLayout->addWidget( textLabel16b, 3, 0 );
	subStackTools->addWidget( subTabLine, 2 );

	subTabImage = new QWidget( subStackTools, "subTabImage" );
	subTabImageLayout = new QGridLayout( subTabImage, 1, 1, 11, 6, "subTabImageLayout");
	buttonGroup3 = new QButtonGroup( subTabImage, "buttonGroup3" );
	buttonGroup3->setCheckable( true );
	buttonGroup3->setChecked( doc->ScaleType );
	buttonGroup3->setColumnLayout(0, Qt::Vertical );
	buttonGroup3->layout()->setSpacing( 6 );
	buttonGroup3->layout()->setMargin( 11 );
	buttonGroup3->setTitle( tr( "&Free Scaling" ) );
	buttonGroup3Layout = new QGridLayout( buttonGroup3->layout() );
	buttonGroup3Layout->setAlignment( Qt::AlignTop );
	scalingHorizontal = new QSpinBox( buttonGroup3, "scalingHorizontal" );
	scalingHorizontal->setMaxValue( 1000 );
	scalingHorizontal->setMinValue( 1 );
	scalingHorizontal->setValue(qRound(doc->ScaleX * 100));
	scalingHorizontal->setSuffix( tr( " %" ) );
	buttonGroup3Layout->addWidget( scalingHorizontal, 0, 1, Qt::AlignRight );
	textLabel17b = new QLabel(scalingHorizontal, tr("&Horizontal Scaling:"), buttonGroup3, "textLabel17b" );
	buttonGroup3Layout->addWidget( textLabel17b, 0, 0 );
	scalingVertical = new QSpinBox( buttonGroup3, "scalingVertical" );
	scalingVertical->setMaxValue( 1000 );
	scalingVertical->setMinValue( 1 );
	scalingVertical->setSuffix( tr( " %" ) );
	scalingVertical->setValue(qRound(doc->ScaleY * 100));
	buttonGroup3Layout->addWidget( scalingVertical, 1, 1, Qt::AlignRight );
	textLabel18b = new QLabel(scalingVertical, tr( "&Vertical Scaling:" ), buttonGroup3, "textLabel18b" );
	buttonGroup3Layout->addWidget( textLabel18b, 1, 0 );
	chainButton = new LinkButton( buttonGroup3 );
	chainButton->setToggleButton( true );
	chainButton->setAutoRaise(true);
	buttonGroup3Layout->addMultiCellWidget( chainButton, 0, 1, 2, 2, Qt::AlignLeft );
	subTabImageLayout->addMultiCellWidget( buttonGroup3, 0, 0, 0, 1 );
	buttonGroup5 = new QButtonGroup( subTabImage, "buttonGroup5" );
	buttonGroup5->setCheckable( true );
	buttonGroup5->setChecked( !doc->ScaleType );
	buttonGroup5->setColumnLayout(0, Qt::Vertical );
	buttonGroup5->layout()->setSpacing( 6 );
	buttonGroup5->layout()->setMargin( 11 );
	buttonGroup5->setTitle( tr( "&Scale Picture to Frame Size" ) );
	buttonGroup5Layout = new QHBoxLayout( buttonGroup5->layout() );
	buttonGroup5Layout->setAlignment( Qt::AlignTop );
	checkRatioImage = new QCheckBox( buttonGroup5, "checkRatioImage" );
	checkRatioImage->setText( tr( "Keep Aspect &Ratio" ) );
	checkRatioImage->setChecked(doc->AspectRatio);
	buttonGroup5Layout->addWidget( checkRatioImage );
	subTabImageLayout->addMultiCellWidget( buttonGroup5, 1, 1, 0, 1 );
	comboFillImage = new QComboBox( true, subTabImage, "comboFillImage" );
	comboFillImage->setEditable(false);
	QPixmap pm2I = QPixmap(15, 15);
	comboFillImage->insertItem( tr("None"));
	if (doc->DbrushPict == "None")
		comboFillImage->setCurrentItem(comboFillImage->count()-1);
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		pm2I.fill(doc->PageColors[itc.key()].getRGBColor());
		comboFillImage->insertItem(pm2I, itc.key());
		if (itc.key() == doc->DbrushPict)
			comboFillImage->setCurrentItem(comboFillImage->count()-1);
	}
	subTabImageLayout->addWidget( comboFillImage, 2, 1 );
	textLabel19b = new QLabel(comboFillImage, tr( "F&ill Color:" ), subTabImage, "textLabel19b" );
	subTabImageLayout->addWidget( textLabel19b, 2, 0 );
	shadingFillImage = new QSpinBox( subTabImage, "shadingFillImage" );
	shadingFillImage->setMaxValue( 100 );
	shadingFillImage->setValue( doc->ShadePict );
	shadingFillImage->setSuffix( tr( " %" ) );
	subTabImageLayout->addWidget( shadingFillImage, 3, 1, Qt::AlignLeft );
	textLabel20b = new QLabel(shadingFillImage, tr( "S&hading:" ), subTabImage, "textLabel20b" );
	subTabImageLayout->addWidget( textLabel20b, 3, 0 );
	subStackTools->addWidget( subTabImage, 3 );

	subTabPolygon = new QWidget( subStackTools, "subTabPolygon" );
	subTabPolygonLayout = new QHBoxLayout( subTabPolygon, 11, 6, "subTabPolygonLayout");
	polyWidget = new PolygonWidget(subTabPolygon, doc->PolyC, doc->PolyFd, doc->PolyF, doc->PolyS, doc->PolyR);
	subTabPolygonLayout->addWidget( polyWidget );
	subStackTools->addWidget( subTabPolygon, 4 );

	subTabZoom = new QWidget( subStackTools, "subTabZoom" );
	subTabZoomLayout = new QGridLayout( subTabZoom, 1, 1, 11, 6, "subTabZoomLayout");
	minimumZoom = new QSpinBox( subTabZoom, "minimumZoom" );
	minimumZoom->setMaxValue( 3200 );
	minimumZoom->setMinValue( 10 );
	minimumZoom->setValue(doc->MagMin);
	minimumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( minimumZoom, 0, 1 );
	textLabel21b = new QLabel( subTabZoom, "textLabel21b" );
	textLabel21b = new QLabel( minimumZoom, tr( "Mi&nimum:" ), subTabZoom, "textLabel21b" );
	subTabZoomLayout->addWidget( textLabel21b, 0, 0, Qt::AlignLeft );
	maximumZoom = new QSpinBox( subTabZoom, "maximumZoom" );
	maximumZoom->setMaxValue( 3200 );
	maximumZoom->setMinValue( 10 );
	maximumZoom->setValue(doc->MagMax);
	maximumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( maximumZoom, 1, 1 );
	textLabel22b = new QLabel( maximumZoom, tr( "Ma&ximum:" ), subTabZoom, "textLabel22b" );
	subTabZoomLayout->addWidget( textLabel22b, 1, 0, Qt::AlignLeft );
	zoomStep = new QSpinBox( subTabZoom, "zoomStep" );
	zoomStep->setMaxValue( 200 );
	zoomStep->setMinValue( 1 );
	zoomStep->setLineStep( 25 );
	zoomStep->setValue( doc->MagStep );
	zoomStep->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( zoomStep, 2, 1 );
	textLabel23b = new QLabel( subTabZoom, "textLabel23b" );
	textLabel23b = new QLabel( zoomStep, tr( "&Stepping:" ), subTabZoom, "textLabel23b" );
	subTabZoomLayout->addWidget( textLabel23b, 2, 0, Qt::AlignLeft );
	subStackTools->addWidget( subTabZoom, 5 );
	tabToolsLayout->addWidget( subStackTools );
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	rightR->setMaxValue(pageWidth - leftR->value());
	leftR->setMaxValue(pageWidth - rightR->value());
	topR->setMaxValue(pageHeight - bottomR->value());
	bottomR->setMaxValue(pageHeight - topR->value());
	toolText->setOn(true);
	setSample();
	//tooltips
	QToolTip::add( facingPages, tr( "Enable single or spread based layout" ) );
	QToolTip::add( firstPage, tr( "Make the first page the left page of the document" ) );
	QToolTip::add( topR, tr( "Distance between the top margin guide and the edge of the page" ) );
	QToolTip::add( bottomR, tr( "Distance between the bottom margin guide and the edge of the page" ) );
	QToolTip::add( leftR, tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( rightR, tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );

	// signals and slots connections
	connect( facingPages, SIGNAL( clicked() ), this, SLOT( setDS() ) );
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	connect(majorGridColor, SIGNAL(clicked()), this, SLOT(changeMajorColor()));
	connect(minorGridColor, SIGNAL(clicked()), this, SLOT(changeMinorColor()));
	connect(baselineColor, SIGNAL(clicked()), this, SLOT(changeBaselineColor()));
	connect(guideColor, SIGNAL(clicked()), this, SLOT(changeGuideColor()));
	connect(marginColor, SIGNAL(clicked()), this, SLOT(changeMarginColor()));
	connect(backColor, SIGNAL(clicked()), this, SLOT(changePaperColor()));
	connect(toolShape, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolPoly, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolImage, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolText, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolLine, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolZoom, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(fontComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
	connect(sizeComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
	connect(buttonGroup3, SIGNAL(clicked(int)), this, SLOT(changeImageScalingFree(int)));
	connect(buttonGroup5, SIGNAL(clicked(int)), this, SLOT(changeImageScalingRatio(int)));
	connect(chainButton, SIGNAL(clicked()), this, SLOT(toggleChain()));
	connect(scalingHorizontal, SIGNAL(valueChanged(int)), this, SLOT(hChange()));
	connect(scalingVertical, SIGNAL(valueChanged(int)), this, SLOT(vChange()));

	prefsWidgets->raiseWidget(0);
	resize( minimumSizeHint() );
	clearWState( WState_Polished );
}

/*!
 \fn void ReformDoc::ToggleKette()
 \author Franz Schmid 
 \date  
 \brief ReformDoc (Tools, Image Frame), Sets Y Scale value from X Scale value when chain is toggled
 \param None
 \retval None 
 */
void ReformDoc::toggleChain()
{
	if (chainButton->isOn())
		scalingVertical->setValue(scalingHorizontal->value());
}

/*!
 \fn void ReformDoc::HChange()
 \author Franz Schmid 
 \date  
 \brief ReformDoc (Tools, Image Frame), Sets Y Scale value from X Scale value when X Scale value is changed
 \param None
 \retval None 
 */
void ReformDoc::hChange()
{
	if (chainButton->isOn())
		scalingVertical->setValue(scalingHorizontal->value());
}

/*!
 \fn void ReformDoc::vChange()
 \author Franz Schmid 
 \date  
 \brief ReformDoc (Tools, Image Frame). Sets X Scale value from Y Scale value when Y Scale value is changed
 \param None
 \retval None 
 */
void ReformDoc::vChange()
{
	if (chainButton->isOn())
		scalingHorizontal->setValue(scalingVertical->value());
}

void ReformDoc::changeImageScalingFree(int)
{
	if (buttonGroup3->isChecked())
		buttonGroup5->setChecked(false);
	else
		buttonGroup5->setChecked(true);
}

void ReformDoc::changeImageScalingRatio(int)
{
	if (buttonGroup5->isChecked())
		buttonGroup3->setChecked(false);
	else
		buttonGroup3->setChecked(true);
}

/*!
 \fn void ReformDoc::SetSample()
 \author Franz Schmid 
 \date  
 \brief ReformDoc (Tools, Text frame), Sets the sample text in selected font in text frame preferences
 \param None
 \retval None 
 */
void ReformDoc::setSample()
{
	QString ts = tr( "Woven silk pyjamas exchanged for blue quartz" );
	QString da = (*fon)[fontComboText->currentText()]->Datei;
	int s = sizeComboText->currentText().left(2).toInt();
	QPixmap pm = fontSamples(da, s, ts, paletteBackgroundColor());
	previewText->setPixmap(pm);
}

/*!
 \fn void ReformDoc::SetTool()
 \author Franz Schmid 
 \date  
 \brief ReformDoc (Tools), Raises widget for selected Tool properties
 \param None
 \retval None 
 */
void ReformDoc::setTool()
{
	if (toolText == sender())
		subStackTools->raiseWidget(0);
	if (toolShape == sender())
		subStackTools->raiseWidget(1);
	if (toolLine == sender())
		subStackTools->raiseWidget(2);
	if (toolImage == sender())
		subStackTools->raiseWidget(3);
	if (toolPoly == sender())
		subStackTools->raiseWidget(4);
	if (toolZoom == sender())
		subStackTools->raiseWidget(5);
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
	bottomR->setMaxValue(pageHeight - topR->value());
}

void ReformDoc::setBottom(int )
{
	topR->setMaxValue(pageHeight - bottomR->value());
}

void ReformDoc::setLeft(int )
{
	rightR->setMaxValue(pageWidth - leftR->value());
}

void ReformDoc::setRight(int )
{
	leftR->setMaxValue(pageWidth - rightR->value());
}

void ReformDoc::setDS()
{
	if (facingPages->isChecked())
	{
		Links->setText( tr( "&Inside:" ) );
		Rechts->setText( tr( "&Outside:" ) );
		firstPage->setEnabled(true);
	}
	else
	{
		Links->setText( tr( "&Left:" ) );
		Rechts->setText( tr( "&Right:" ) );
		firstPage->setEnabled(false);
	}
}
