#include "reformdoc.h"
#include "reformdoc.moc"
#include <qtooltip.h>
#include <qcolordialog.h>
#include "fontcombo.h"
#include "polygonwidget.h"
#include "arrowchooser.h"
#include "tabtypography.h"
#include "hysettings.h"
#include "cmsprefs.h"

extern QPixmap loadIcon(QString nam);
extern QPixmap fontSamples(QString da, int s, QString ts, QColor back);
extern double UmReFaktor;
extern bool CMSavail;
extern ProfilesL InputProfiles;

ReformDoc::ReformDoc( QWidget* parent, ScribusDoc* doc, preV *prefsData ) : PrefsDialogBase( parent )
{
	einheit = doc->Einheit;
	docc = doc;
	ap = (ScribusApp*)parent;
	fon = &prefsData->AvailFonts;
	Umrech = UmReFaktor;
	QString units[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	QString ein = units[doc->Einheit];
	int dp[] = {100, 1000, 10000, 100};
	decimals = dp[doc->Einheit];
	int i=-1;
	pageWidth = doc->PageB * UmReFaktor;
	pageHeight = doc->PageH * UmReFaktor;
	setCaption( tr( "Document Setup" ) );
	tabPage = new QWidget( prefsWidgets, "tab" );
	reformDocLayout = new QVBoxLayout( tabPage );
	reformDocLayout->setSpacing( 6 );
	reformDocLayout->setMargin( 10 );
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
	heightMSpinBox->setEnabled( false );
	heightMSpinBox->setSuffix(ein);
	heightMSpinBox->setValue(pageHeight);
	heightQLabel = new QLabel(heightMSpinBox,  tr( "&Height:" ), dsGroupBox7, "heightLabel" );
	dsLayout4->addWidget( heightQLabel, 1, 2 );
	dsLayout4->addWidget( heightMSpinBox, 1, 3 );
	unitCombo = new QComboBox( true, dsGroupBox7, "unitCombo" );
	unitCombo->insertItem( tr( "Points (pt)" ) );
	unitCombo->insertItem( tr( "Millimetres (mm)" ) );
	unitCombo->insertItem( tr( "Inches (in)" ) );
	unitCombo->insertItem( tr( "Picas (p)" ) );
	unitCombo->setEditable(false);
	unitCombo->setCurrentItem(doc->Einheit);
	unitQLabel = new QLabel(unitCombo, tr( "&Unit:" ), dsGroupBox7, "unitQLabel" );
	dsLayout4->addWidget( unitQLabel, 2, 0 );
	dsLayout4->addWidget( unitCombo, 2, 1 );
	dsGroupBox7Layout->addLayout( dsLayout4 );
	reformDocLayout->addWidget( dsGroupBox7 );
	groupBox7 = new QGroupBox( tabPage, "GroupBox7" );
	groupBox7->setTitle( tr( "Margin Guides" ) );
	groupBox7->setColumnLayout(0, Qt::Vertical );
	groupBox7->layout()->setSpacing( 0 );
	groupBox7->layout()->setMargin( 0 );
	groupBox7Layout = new QHBoxLayout( groupBox7->layout() );
	groupBox7Layout->setAlignment( Qt::AlignTop );
	groupBox7Layout->setSpacing( 0 );
	groupBox7Layout->setMargin( 10 );
	layout4 = new QGridLayout;
	layout4->setSpacing( 6 );
	layout4->setMargin( 0 );
	topR = new MSpinBox( groupBox7, 4 );
	topR->setSuffix( ein );
	topR->setDecimals( decimals );
	topR->setMaxValue(pageHeight);
	topR->setValue(doc->PageM.Top * UmReFaktor);
	layout4->addWidget( topR, 0, 1 );
	TextLabel5 = new QLabel( tr( "&Top:" ), groupBox7, "TextLabel5" );
	TextLabel5->setBuddy(topR);
	layout4->addWidget( TextLabel5, 0, 0 );
	leftR = new MSpinBox( groupBox7, 4 );
	leftR->setSuffix( ein );
	leftR->setDecimals( decimals );
	leftR->setMaxValue(pageWidth);
	leftR->setValue(doc->PageM.Left * UmReFaktor);
	layout4->addWidget( leftR, 0, 3 );
	Links = new QLabel( tr( "&Left:" ), groupBox7, "Links" );
	Links->setBuddy(leftR);
	layout4->addWidget( Links, 0, 2 );
	bottomR = new MSpinBox( groupBox7, 4 );
	bottomR->setSuffix( ein );
	bottomR->setDecimals( decimals );
	bottomR->setMaxValue(pageHeight);
	bottomR->setValue(doc->PageM.Bottom * UmReFaktor);
	layout4->addWidget( bottomR, 1, 1 );
	TextLabel7 = new QLabel( tr( "&Bottom:" ), groupBox7, "TextLabel7" );
	TextLabel7->setBuddy(bottomR);
	layout4->addWidget( TextLabel7, 1, 0 );
	rightR = new MSpinBox( groupBox7, 4 );
	rightR->setSuffix( ein );
	rightR->setDecimals( decimals );
	rightR->setMaxValue(pageWidth);
	rightR->setValue(doc->PageM.Right * UmReFaktor);
	layout4->addWidget( rightR, 1, 3 );
	Rechts = new QLabel( tr( "&Right:" ), groupBox7, "Rechts" );
	Rechts->setBuddy(rightR);
	layout4->addWidget( Rechts, 1, 2 );
	groupBox7Layout->addLayout( layout4 );
	reformDocLayout->addWidget( groupBox7 );
	groupBox7a = new QGroupBox( tabPage, "groupBox7" );
	groupBox7a->setTitle( tr( "Layout" ) );
	groupBox7a->setColumnLayout(0, Qt::Vertical );
	groupBox7a->layout()->setSpacing( 0 );
	groupBox7a->layout()->setMargin( 0 );
	groupBox7aLayout = new QHBoxLayout( groupBox7a->layout() );
	groupBox7aLayout->setAlignment( Qt::AlignTop );
	groupBox7aLayout->setSpacing( 0 );
	groupBox7aLayout->setMargin( 10 );
	layout4a = new QGridLayout;
	layout4a->setSpacing( 6 );
	layout4a->setMargin( 0 );
	facingPages = new QCheckBox( tr( "&Facing Pages" ),groupBox7a, "facingPages" );
	facingPages->setChecked( doc->PageFP );
	layout4a->addMultiCellWidget( facingPages, 2, 2, 0, 1 );
	firstPage = new QCheckBox( tr( "Left &Page First" ), groupBox7a, "n" );
	firstPage->setChecked( doc->FirstPageLeft );
	layout4a->addMultiCellWidget( firstPage, 2, 2, 2, 3 );
	if (!doc->PageFP)
		firstPage->setEnabled(false);
	setDS();
	TextLabel1_3 = new QLabel( tr( "F&irst Page Number:" ), groupBox7a, "TextLabel1_3" );
	layout4a->addMultiCellWidget( TextLabel1_3, 3, 3, 0, 1 );
	pageNumber = new QSpinBox( groupBox7a, "pageNumber" );
	pageNumber->setMaxValue( 10000 );
	pageNumber->setMinValue( 1 );
	pageNumber->setValue(doc->FirstPnum);
	layout4a->addWidget( pageNumber, 3, 2, Qt::AlignRight );
	TextLabel1_3->setBuddy(pageNumber);
	groupBox7aLayout->addLayout( layout4a );
	reformDocLayout->addWidget( groupBox7a );
	groupAutoSave = new QGroupBox( tabPage, "groupAutoSave" );
	groupAutoSave->setTitle( tr( "Autosave" ) );
	groupAutoSave->setCheckable( true );
	groupAutoSave->setChecked( doc->AutoSave );
	groupAutoSave->setColumnLayout(0, Qt::Vertical );
	groupAutoSave->layout()->setSpacing( 6 );
	groupAutoSave->layout()->setMargin( 11 );
	groupAutoSaveLayout = new QHBoxLayout( groupAutoSave->layout() );
	groupAutoSaveLayout->setAlignment( Qt::AlignTop );
	autoSaveTime = new QSpinBox( groupAutoSave, "autoSaveTime" );
	autoSaveTime->setMinValue( 1 );
	autoSaveTime->setMaxValue( 60 );
	autoSaveTime->setSuffix( " " + tr("min") );
	autoSaveTime->setValue(doc->AutoSaveTime / 1000 / 60);
	groupAutoSaveLayout->addWidget( autoSaveTime );
	textLabel1m = new QLabel(autoSaveTime, tr( "&Interval:" ), groupAutoSave, "textLabel1m" );
	groupAutoSaveLayout->addWidget( textLabel1m );
	reformDocLayout->addWidget( groupAutoSave );
	addItem( tr("Document"), loadIcon("page.png"), tabPage);

	tabGuides = new QWidget( prefsWidgets, "tabView" );
	tabGuidesLayout = new QVBoxLayout( tabGuides, 10, 5, "tabViewLayout");
	checkGrid = new QGroupBox( tabGuides, "checkGrid" );
	checkGrid->setTitle( tr( "Show Grid" ) );
	checkGrid->setCheckable( true );
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
	majorSpace = new MSpinBox( 10 * UmReFaktor, 1000 * UmReFaktor, groupBox1, decimals );
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
	minorSpace = new MSpinBox( UmReFaktor, 1000 * UmReFaktor, groupBox2, decimals );
	minorSpace->setValue( doc->minorGrid  * UmReFaktor);
	minorSpace->setSuffix( ein );
	groupBox2Layout->addWidget( minorSpace, 0, 1 );
	checkGridLayout->addWidget( groupBox2, 0, 1 );
	tabGuidesLayout->addWidget( checkGrid );
	layout11 = new QGridLayout( 0, 1, 1, 0, 5, "layout11");
	textLabel8 = new QLabel( tabGuides, "textLabel8" );
	textLabel8->setText( tr( "Guide Snap Distance:" ) );
	layout11->addWidget( textLabel8, 0, 0 );
	snapDistance = new MSpinBox( UmReFaktor, 1000 * UmReFaktor, tabGuides, decimals );
	snapDistance->setValue( doc->GuideRad * UmReFaktor );
	snapDistance->setSuffix( ein );
	layout11->addWidget( snapDistance, 0, 1, Qt::AlignLeft );
	textLabel82 = new QLabel( tabGuides, "textLabel8" );
	textLabel82->setText( tr( "Grab Radius:" ) );
	layout11->addWidget( textLabel82, 1, 0 );
	grabDistance = new QSpinBox( tabGuides, "grabDistance" );
	grabDistance->setMaxValue( 1000 );
	grabDistance->setMinValue( 1 );
	grabDistance->setLineStep( 1 );
	grabDistance->setValue( doc->GrabRad );
	grabDistance->setSuffix( tr( " px" ) );
	layout11->addWidget( grabDistance, 1, 1, Qt::AlignLeft );

	checkMargin = new QCheckBox( tabGuides, "checkMargin" );
	checkMargin->setText( tr( "Show Margins" ) );
	checkMargin->setChecked(doc->MarginsShown);
	layout11->addWidget( checkMargin, 2, 0 );
	checkGuides = new QCheckBox( tabGuides, "checkGuides" );
	checkGuides->setText( tr( "Show Guides" ) );
	checkGuides->setChecked(doc->GuidesShown);
	layout11->addWidget( checkGuides, 3, 0 );
	checkBaseline = new QCheckBox( tabGuides, "checkBaseline" );
	checkBaseline->setText( tr( "Show Baseline Grid" ) );
	checkBaseline->setChecked(doc->BaseShown);
	layout11->addWidget( checkBaseline, 4, 0 );
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
	layout11->addLayout( layout2, 4, 1 );
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
	layout11->addLayout( layout3, 3, 1 );
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
	layout11->addLayout( layout9, 2, 1 );
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

	groupScratch = new QGroupBox( tabView, "GroupBox7" );
	groupScratch->setTitle( tr( "Scratch Space" ) );
	groupScratch->setColumnLayout(0, Qt::Vertical );
	groupScratch->layout()->setSpacing( 0 );
	groupScratch->layout()->setMargin( 0 );
	groupScratchLayout = new QHBoxLayout( groupScratch->layout() );
	groupScratchLayout->setAlignment( Qt::AlignTop );
	groupScratchLayout->setSpacing( 0 );
	groupScratchLayout->setMargin( 10 );
	layout4s = new QGridLayout;
	layout4s->setSpacing( 6 );
	layout4s->setMargin( 0 );
	topScratch = new MSpinBox( groupScratch, 4 );
	topScratch->setSuffix( ein );
	topScratch->setDecimals( decimals );
	topScratch->setMaxValue(1000);
	topScratch->setValue(doc->ScratchTop * UmReFaktor);
	layout4s->addWidget( topScratch, 0, 1 );
	TextLabel5s = new QLabel(topScratch, tr( "&Top:" ), groupScratch, "TextLabel5" );
	layout4s->addWidget( TextLabel5s, 0, 0 );
	leftScratch = new MSpinBox( groupScratch, 4 );
	leftScratch->setSuffix( ein );
	leftScratch->setDecimals( decimals );
	leftScratch->setMaxValue(1000);
	leftScratch->setValue(doc->ScratchLeft * UmReFaktor);
	layout4s->addWidget( leftScratch, 0, 3 );
	Linkss = new QLabel(leftScratch, tr( "&Left:" ), groupScratch, "Links" );
	layout4s->addWidget( Linkss, 0, 2 );
	bottomScratch = new MSpinBox( groupScratch, 4 );
	bottomScratch->setSuffix( ein );
	bottomScratch->setDecimals( decimals );
	bottomScratch->setMaxValue(1000);
	bottomScratch->setValue(doc->ScratchBottom * UmReFaktor);
	layout4s->addWidget( bottomScratch, 1, 1 );
	TextLabel7s = new QLabel(bottomScratch, tr( "&Bottom:" ), groupScratch, "TextLabel7" );
	layout4s->addWidget( TextLabel7s, 1, 0 );
	rightScratch = new MSpinBox( groupScratch, 4 );
	rightScratch->setSuffix( ein );
	rightScratch->setDecimals( decimals );
	rightScratch->setMaxValue(1000);
	rightScratch->setValue(doc->ScratchRight * UmReFaktor);
	layout4s->addWidget( rightScratch, 1, 3 );
	Rechtss = new QLabel(rightScratch, tr( "&Right:" ), groupScratch, "Rechts" );
	layout4s->addWidget( Rechtss, 1, 2 );
	groupScratchLayout->addLayout( layout4s );
	tabViewLayout->addWidget( groupScratch );

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

	tabTypo = new TabTypograpy(  prefsWidgets, 
																doc->VTief, 
																doc->VTiefSc, 
																doc->VHoch, 
																doc->VHochSc, 
																doc->VKapit, 
																doc->BaseGrid * UmReFaktor, 
																doc->BaseOffs * UmReFaktor, 
																doc->AutoLine, 
																ein);
	addItem( tr("Typography"), loadIcon("font.png"), tabTypo);

	tabTools = new QWidget( prefsWidgets, "tabTools" );
	tabToolsLayout = new QHBoxLayout( tabTools, 11, 6, "tabToolsLayout");
	buttonGroupTools = new QButtonGroup( tabTools, "buttonGroupTools" );
	buttonGroupTools->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, buttonGroupTools->sizePolicy().hasHeightForWidth() ) );
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
	subStackTools->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, subStackTools->sizePolicy().hasHeightForWidth() ) );
	subStackTools->setFrameShape( QWidgetStack::GroupBoxPanel );
	subStackTools->setFrameShadow( QWidgetStack::Sunken );

	subTabText = new QWidget( subStackTools, "subTabText" );
	subTabTextLayout = new QGridLayout( subTabText, 1, 1, 11, 6, "subTabTextLayout");
	fontComboText = new FontCombo(subTabText, prefsData);
	for (int fc=0; fc<fontComboText->count(); ++fc)
	{
		if (fontComboText->text(fc) == doc->Dfont)
		{
			fontComboText->setCurrentItem(fc);
			break;
		}
	}
	subTabTextLayout->addMultiCellWidget( fontComboText, 0, 0, 1, 3, Qt::AlignLeft );
	textLabel1b = new QLabel( fontComboText, tr( "Font:" ), subTabText, "textLabel1b" );
	subTabTextLayout->addWidget( textLabel1b, 0, 0 );
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
	subTabTextLayout->addMultiCellWidget( sizeComboText, 1, 1, 1, 3, Qt::AlignLeft );
	textLabel2b = new QLabel(sizeComboText, tr( "Size:" ), subTabText, "textLabel2b" );
	subTabTextLayout->addWidget( textLabel2b, 1, 0 );
	colorComboText = new QComboBox( true, subTabText, "colorComboText" );
	colorComboText->setEditable(false);
	QPixmap pmT2;
	pmT2 = QPixmap(15, 15);
	CListe::Iterator itc;
	colorComboText->insertItem( tr("None"));
	if (doc->DpenText == "None")
		colorComboText->setCurrentItem(colorComboText->count()-1);
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		pmT2.fill(doc->PageColors[itc.key()].getRGBColor());
		colorComboText->insertItem(pmT2, itc.key());
		if (itc.key() == doc->DpenText)
			colorComboText->setCurrentItem(colorComboText->count()-1);
	}
	subTabTextLayout->addMultiCellWidget( colorComboText, 2, 2, 1, 3, Qt::AlignLeft );
	textLabel3b = new QLabel(colorComboText, tr( "Fill Color:" ), subTabText, "textLabel3b" );
	subTabTextLayout->addWidget( textLabel3b, 2, 0 );
	colorComboStrokeText = new QComboBox( true, subTabText, "colorComboStrokeText" );
	colorComboStrokeText->setEditable(false);
	colorComboStrokeText->insertItem( tr("None"));
	if (doc->DstrokeText == "None")
		colorComboStrokeText->setCurrentItem(colorComboStrokeText->count()-1);
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		pmT2.fill(doc->PageColors[itc.key()].getRGBColor());
		colorComboStrokeText->insertItem(pmT2, itc.key());
		if (itc.key() == doc->DstrokeText)
			colorComboStrokeText->setCurrentItem(colorComboStrokeText->count()-1);
	}
	subTabTextLayout->addMultiCellWidget( colorComboStrokeText, 3, 3, 1, 3, Qt::AlignLeft );
	textLabel3b2 = new QLabel(colorComboStrokeText, tr( "Stroke Color:" ), subTabText, "textLabel3b2" );
	subTabTextLayout->addWidget( textLabel3b2, 3, 0 );
	columnsText = new QSpinBox( subTabText, "columnsText" );
	columnsText->setMinValue( 1 );
	columnsText->setMaxValue(100);
	columnsText->setValue(doc->DCols);
	subTabTextLayout->addWidget( columnsText, 4, 1, Qt::AlignLeft );
	textLabel4b = new QLabel(columnsText, tr("Colu&mns:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel4b, 4, 0 );
	gapText = new MSpinBox( 0, 200, subTabText, decimals );
	gapText->setSuffix( tr( " pt" ) );
	gapText->setValue(doc->DGap * UmReFaktor);
	subTabTextLayout->addWidget( gapText, 4, 3, Qt::AlignLeft );
	textLabel5b = new QLabel(gapText, tr("&Gap:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel5b, 4, 2 );
	previewText = new QLabel( tr( "Woven silk pyjamas exchanged for blue quartz" ), subTabText, "previewText" );
	previewText->setMinimumSize( QSize( 280, 70 ) );
	previewText->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	subTabTextLayout->addMultiCellWidget( previewText, 5, 5, 0, 3 );
	subStackTools->addWidget( subTabText, 0 );

	subTabShape = new QWidget( subStackTools, "subTabShape" );
	subTabShapeLayout = new QGridLayout( subTabShape, 1, 1, 11, 6, "subTabShapeLayout");
	subTabShapeLayout->setAlignment( Qt::AlignTop );
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
	subTabShapeLayout->addWidget( colorComboLineShape, 0, 1, Qt::AlignLeft );
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
	subTabShapeLayout->addWidget( comboFillShape, 2, 1, Qt::AlignLeft );
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
	subTabShapeLayout->addWidget( comboStyleShape, 4, 1, Qt::AlignLeft );
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
	subTabLineLayout->setAlignment( Qt::AlignTop );
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
	subTabLineLayout->addMultiCellWidget( colorComboLine, 0, 0, 1, 2, Qt::AlignLeft );
	textLabel13b = new QLabel(colorComboLine, tr("&Line Color:"), subTabLine, "textLabel13b" );
	subTabLineLayout->addWidget( textLabel13b, 0, 0 );
	shadingLine = new QSpinBox( subTabLine, "shadingLine" );
	shadingLine->setMaxValue( 100 );
	shadingLine->setValue(doc->DshadeLine);
	shadingLine->setSuffix( tr( " %" ) );
	subTabLineLayout->addMultiCellWidget( shadingLine, 1, 1, 1, 2, Qt::AlignLeft );
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
	subTabLineLayout->addMultiCellWidget( comboStyleLine, 2, 2, 1, 2, Qt::AlignLeft );
	textLabel15b = new QLabel(subTabLine, tr("Line S&tyle:"), subTabLine, "textLabel15b" );
	subTabLineLayout->addWidget( textLabel15b, 2, 0 );
	startArrow = new ArrowChooser(subTabLine, true);
	subTabLineLayout->addWidget( startArrow, 4, 1 );
	endArrow = new ArrowChooser(subTabLine, false);
	subTabLineLayout->addWidget( endArrow, 4, 2 );
	startArrow->rebuildList(&doc->arrowStyles);
	endArrow->rebuildList(&doc->arrowStyles);
	startArrow->setCurrentItem(doc->DstartArrow);
	endArrow->setCurrentItem(doc->DendArrow);
	arrowText = new QLabel( tr( "Arrows:" ), subTabLine, "arrowText" );
	subTabLineLayout->addMultiCellWidget( arrowText, 3, 4, 0, 0 );
	startArrowText = new QLabel( startArrow, tr( "Start:" ), subTabLine, "startArrowText" );
	subTabLineLayout->addWidget( startArrowText, 3, 1 );
	endArrowText = new QLabel( endArrow, tr( "End:" ), subTabLine, "endArrowText" );
	subTabLineLayout->addWidget( endArrowText, 3, 2 );
	lineWidthLine = new MSpinBox( 1, 36, subTabLine, 1 );
	lineWidthLine->setSuffix( tr( " pt" ) );
	lineWidthLine->setValue(doc->DwidthLine);
	subTabLineLayout->addMultiCellWidget( lineWidthLine, 5, 5, 1, 2, Qt::AlignLeft );
	textLabel16b = new QLabel(lineWidthLine, tr("Line &Width:"), subTabLine, "textLabel16b" );
	subTabLineLayout->addWidget( textLabel16b, 5, 0 );
	subStackTools->addWidget( subTabLine, 2 );

	subTabImage = new QWidget( subStackTools, "subTabImage" );
	subTabImageLayout = new QGridLayout( subTabImage, 1, 1, 11, 6, "subTabImageLayout");
	subTabImageLayout->setAlignment( Qt::AlignTop );
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
	subTabImageLayout->addWidget( comboFillImage, 2, 1, Qt::AlignLeft );
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
	subTabPolygonLayout->setAlignment( Qt::AlignTop );
	polyWidget = new PolygonWidget(subTabPolygon, doc->PolyC, doc->PolyFd, doc->PolyF, doc->PolyS, doc->PolyR);
	subTabPolygonLayout->addWidget( polyWidget );
	subStackTools->addWidget( subTabPolygon, 4 );

	subTabZoom = new QWidget( subStackTools, "subTabZoom" );
	subTabZoomLayout = new QGridLayout( subTabZoom, 1, 1, 11, 6, "subTabZoomLayout");
	subTabZoomLayout->setAlignment( Qt::AlignTop );
	minimumZoom = new QSpinBox( subTabZoom, "minimumZoom" );
	minimumZoom->setMaxValue( 3200 );
	minimumZoom->setMinValue( 10 );
	minimumZoom->setValue(doc->MagMin);
	minimumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( minimumZoom, 0, 1, Qt::AlignLeft );
	textLabel21b = new QLabel( minimumZoom, tr( "Mi&nimum:" ), subTabZoom, "textLabel21b" );
	subTabZoomLayout->addWidget( textLabel21b, 0, 0);
	maximumZoom = new QSpinBox( subTabZoom, "maximumZoom" );
	maximumZoom->setMaxValue( 3200 );
	maximumZoom->setMinValue( 10 );
	maximumZoom->setValue(doc->MagMax);
	maximumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( maximumZoom, 1, 1, Qt::AlignLeft );
	textLabel22b = new QLabel( maximumZoom, tr( "Ma&ximum:" ), subTabZoom, "textLabel22b" );
	subTabZoomLayout->addWidget( textLabel22b, 1, 0 );
	zoomStep = new QSpinBox( subTabZoom, "zoomStep" );
	zoomStep->setMaxValue( 200 );
	zoomStep->setMinValue( 1 );
	zoomStep->setLineStep( 25 );
	zoomStep->setValue( doc->MagStep );
	zoomStep->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( zoomStep, 2, 1, Qt::AlignLeft );
	textLabel23b = new QLabel( zoomStep, tr( "&Stepping:" ), subTabZoom, "textLabel23b" );
	subTabZoomLayout->addWidget( textLabel23b, 2, 0 );
	subStackTools->addWidget( subTabZoom, 5 );
	tabToolsLayout->addWidget( subStackTools );
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets, &ap->LangTransl);
	tabHyphenator->verbose->setChecked(!doc->Trenner->Automatic);
	tabHyphenator->input->setChecked(doc->Trenner->AutoCheck);
	tabHyphenator->language->setCurrentText(ap->LangTransl[doc->Trenner->Language]);
	tabHyphenator->wordLen->setValue(doc->Trenner->MinWordLen);
	tabHyphenator->maxCount->setValue(doc->Trenner->HyCount);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);
	
	int cmsTab = 0;
	if (CMSavail)
	{
		tabColorManagement = new CMSPrefs(prefsWidgets, &doc->CMSSettings, &InputProfiles, &ap->PrinterProfiles, &ap->MonitorProfiles);
		cmsTab = addItem( tr("Color Management"), loadIcon("blend.png"), tabColorManagement);
	}

	rightR->setMaxValue(pageWidth - leftR->value());
	leftR->setMaxValue(pageWidth - rightR->value());
	topR->setMaxValue(pageHeight - bottomR->value());
	bottomR->setMaxValue(pageHeight - topR->value());
	toolText->setOn(true);
	setSample();
	//tooltips
	QToolTip::add( toolText, tr( "Text Frame Properties" ) );
	QToolTip::add( toolImage, tr( "Picture Frame Properties" ) );
	QToolTip::add( toolShape, tr( "Shape Drawing Properties" ) );
	QToolTip::add( toolZoom, tr( "Magnification Level Defaults" ) );
	QToolTip::add( toolLine, tr( "Line Drawing Properties" ) );
	QToolTip::add( toolPoly, tr( "Polygon Drawing Properties" ) );
	QToolTip::add( fontComboText, tr( "Font for new text frames" ) );
	QToolTip::add( sizeComboText, tr( "Size of font for new text frames" ) );
	QToolTip::add( colorComboText, tr( "Color of font" ) );
	QToolTip::add( columnsText, tr( "Number of columns in a text frame" ) );
	QToolTip::add( gapText, tr( "Gap between text frame columns" ) );
	QToolTip::add( previewText, tr( "Sample of your font" ) );
	QToolTip::add( buttonGroup3, tr( "Picture frames allow pictures to scale to any size" ) );
	QToolTip::add( scalingHorizontal, tr( "Horizontal scaling of images" ) );
	QToolTip::add( scalingVertical, tr( "Vertical scaling of images" ) );
	QToolTip::add( chainButton, tr( "Keep horizontal and vertical scaling the same" ) );
	QToolTip::add( buttonGroup5, tr( "Pictures in picture frames are scaled to the size of the frame" ) );
	QToolTip::add( checkRatioImage, tr( "Automatically scaled pictures keep their original proportions" ) );
	QToolTip::add( comboFillImage, tr( "Fill color of picture frames" ) );
	QToolTip::add( shadingFillImage, tr( "Saturation of color of fill" ) );
	QToolTip::add( colorComboLineShape, tr( "Line color of shapes" ) );
	QToolTip::add( shadingLineShape, tr( "Saturation of color of lines" ) );
	QToolTip::add( comboFillShape, tr( "Fill color of shapes" ) );
	QToolTip::add( shadingFillShape, tr( "Saturation of color of fill" ) );
	QToolTip::add( comboStyleShape, tr( "Line style of shapes" ) );
	QToolTip::add( lineWidthShape, tr( "Line width of shapes" ) );
	QToolTip::add( minimumZoom, tr( "Minimum magnification allowed" ) );
	QToolTip::add( maximumZoom, tr( "Maximum magnification allowed" ) );
	QToolTip::add( zoomStep, tr( "Change in magnification for each zoom operation" ) );
	QToolTip::add( colorComboLine, tr( "Color of lines" ) );
	QToolTip::add( shadingLine, tr( "Saturation of color" ) );
	QToolTip::add( comboStyleLine, tr( "Style of lines" ) );
	QToolTip::add( lineWidthLine, tr( "Width of lines" ) );
	QToolTip::add( backColor, tr( "Color for paper" ) );
	QToolTip::add( checkUnprintable, tr( "Mask the area outside the margins in the margin color" ) );
	QToolTip::add( minorSpace, tr( "Distance between the minor grid lines" ) );
	QToolTip::add( majorSpace, tr( "Distance between the major grid lines" ) );
	QToolTip::add( snapDistance, tr( "Distance within which an object will snap to your placed guides" ) );
	QToolTip::add( grabDistance, tr( "Radius of the area where Scribus will allow you to grab an objects handles" ) );
	QToolTip::add( minorGridColor, tr( "Color of the minor grid lines" ) );
	QToolTip::add( majorGridColor, tr( "Color of the major grid lines" ) );
	QToolTip::add( guideColor, tr( "Color of the guide lines you insert" ) );
	QToolTip::add( marginColor, tr( "Color for the margin lines" ) );
	QToolTip::add( baselineColor, tr( "Color for the basegrid lines" ) );
	QToolTip::add( checkBaseline, tr("Turns the basegrid on or off"));
	QToolTip::add( checkGrid, tr("Turns the gridlines on or off"));
	QToolTip::add( checkGuides, tr("Turns the guides on or off"));
	QToolTip::add( checkMargin, tr("Turns the margins on or off"));
	QToolTip::add( checkLink, tr("Turns the of linked frames on or off"));
	QToolTip::add( checkFrame, tr("Turns the display of frames on or off"));
	QToolTip::add( checkPictures, tr("Turns the display of pictures on or off"));
	QToolTip::add( facingPages, tr( "Enable single or spread based layout" ) );
	QToolTip::add( firstPage, tr( "Make the first page the left page of the document" ) );
	QToolTip::add( topR, tr( "Distance between the top margin guide and the edge of the page" ) );
	QToolTip::add( bottomR, tr( "Distance between the bottom margin guide and the edge of the page" ) );
	QToolTip::add( leftR, tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( rightR, tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );

	// signals and slots connections
	connect( facingPages, SIGNAL( clicked() ), this, SLOT( setDS() ) );
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
	connect(unitCombo, SIGNAL(activated(int)), this, SLOT(unitChange()));

	if (CMSavail)
		prefsWidgets->raiseWidget(cmsTab);
	arrangeIcons();
	resize( minimumSizeHint() );
	clearWState( WState_Polished );
	prefsWidgets->raiseWidget(0);
}

void ReformDoc::unitChange()
{
	disconnect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	disconnect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	disconnect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	disconnect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	int decimalsOld;
	double AltUmrech = Umrech;
	double oldMin, oldMax, val;
	QString einh;
	einheit = unitCombo->currentItem();
	switch (unitCombo->currentItem())
	{
	case 0:
		Umrech = 1.0;
		decimals = 100;
		einh = tr( " pt" );
		break;
	case 1:
		Umrech = 0.3527777;
		decimals = 1000;
		einh = tr( " mm" );
		break;
	case 2:
		Umrech = 1.0 / 72.0;
		decimals = 10000;
		einh = tr( " in" );
		break;
	case 3:
		Umrech = 1.0 / 12.0;
		decimals = 100;
		einh = tr( " p" );
		break;
	}

	widthMSpinBox->setSuffix(einh);
	heightMSpinBox->setSuffix(einh);
	topR->setSuffix(einh);
	bottomR->setSuffix(einh);
	leftR->setSuffix(einh);
	rightR->setSuffix(einh);
	minorSpace->setSuffix(einh);
	majorSpace->setSuffix(einh);
	snapDistance->setSuffix(einh);
	tabTypo->baseGrid->setSuffix(einh);
	tabTypo->baseOffset->setSuffix(einh);
	gapText->setSuffix(einh);
	topScratch->setSuffix(einh);
	bottomScratch->setSuffix(einh);
	leftScratch->setSuffix(einh);
	rightScratch->setSuffix(einh);

	double invUnitConversion = 1.0 / AltUmrech * Umrech;

	widthMSpinBox->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	widthMSpinBox->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, docc->PageB * Umrech);
	heightMSpinBox->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	heightMSpinBox->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, docc->PageH * Umrech);
	topR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	topR->setValues(0, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	bottomR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	bottomR->setValues(0, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	leftR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	leftR->setValues(0, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	rightR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	rightR->setValues(0, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	minorSpace->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	minorSpace->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	majorSpace->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	majorSpace->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	snapDistance->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	snapDistance->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabTypo->baseGrid->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabTypo->baseGrid->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabTypo->baseOffset->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabTypo->baseOffset->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	gapText->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	gapText->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	topScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	topScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	bottomScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	bottomScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	leftScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	leftScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	rightScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	rightScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);

	pageWidth = docc->PageB * Umrech;
	pageHeight = docc->PageH * Umrech;
	rightR->setMaxValue(pageWidth - leftR->value());
	leftR->setMaxValue(pageWidth - rightR->value());
	topR->setMaxValue(pageHeight - bottomR->value());
	bottomR->setMaxValue(pageHeight - topR->value());
	connect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
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
