/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "reformdoc.h"
#include "reformdoc.moc"

#include <qcolordialog.h>
#include <qcursor.h>
#include <qspinbox.h>
#include <qtooltip.h>

#include "docsections.h"
#include "tabtypography.h"
#include "docinfo.h"
#include "tabguides.h"
#include "tabtools.h"
#include "tabcheckdoc.h"
#include "hysettings.h"
#include "cmsprefs.h"
#include "units.h"
#include "mspinbox.h"
#include "scribus.h"
#include "tabpdfoptions.h"
#include "fontprefs.h"
#include "units.h"
#include "pagesize.h"
#include "docitemattrprefs.h"
#include "tocindexprefs.h"
#include "marginWidget.h"
#include "page.h"
#include "prefsmanager.h"
#include "pagelayout.h"
#include "fontcombo.h"
#include "linecombo.h"
#include "arrowchooser.h"
#include "polygonwidget.h"
#include "hyphenator.h"
#include "sccombobox.h"

extern QPixmap loadIcon(QString nam);
extern bool CMSavail;

#ifdef HAVE_CMS
extern cmsHPROFILE CMSoutputProf;
extern cmsHPROFILE CMSprinterProf;
extern cmsHTRANSFORM stdTransG;
extern cmsHTRANSFORM stdProofG;
extern cmsHTRANSFORM stdTransImgG;
extern cmsHTRANSFORM stdProofImgG;
extern cmsHTRANSFORM stdTransCMYKG;
extern cmsHTRANSFORM stdProofCMYKG;
extern cmsHTRANSFORM stdTransRGBG;
extern cmsHTRANSFORM stdProofCMYKGCG;
extern cmsHTRANSFORM stdProofGCG;
extern bool BlackPoint;
extern bool SoftProofing;
extern bool Gamut;
extern bool CMSuse;
extern int IntentMonitor;
extern int IntentPrinter;
#endif

ReformDoc::ReformDoc( QWidget* parent, ScribusDoc* doc ) : PrefsDialogBase( parent )
{
	einheit = doc->unitIndex();
	currDoc = doc;
	ScMW = (ScribusMainWindow*)parent;
	unitRatio = doc->unitRatio();
	QString ein = unitGetSuffixFromIndex(einheit);
	decimals = unitGetDecimalsFromIndex(einheit);
	pageWidth = doc->pageWidth;
	pageHeight = doc->pageHeight;
	setCaption( tr( "Document Setup" ) );
	tabPage = new QWidget( prefsWidgets, "tab" );
	reformDocLayout = new QVBoxLayout( tabPage );
	reformDocLayout->setSpacing( 5 );
	reformDocLayout->setMargin( 0 );
	reformDocLayout->setAlignment( Qt::AlignTop );
	dsLayout4p = new QHBoxLayout;
	dsLayout4p->setSpacing( 5 );
	dsLayout4p->setMargin( 0 );
	docLayout = new PageLayouts(tabPage, doc->pageSets);
	docLayout->selectItem(doc->currentPageLayout);
	docLayout->firstPage->setCurrentItem(doc->pageSets[doc->currentPageLayout].FirstPage);
	dsLayout4p->addWidget( docLayout );

	dsLayout4pv = new QVBoxLayout;
	dsLayout4pv->setSpacing( 5 );
	dsLayout4pv->setMargin( 0 );
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
	dsLayout4->setSpacing( 5 );
	dsLayout4->setMargin( 0 );
	sizeQComboBox = new QComboBox( true, dsGroupBox7, "sizeQComboBox" );
	sizeQComboBox->setEditable(false);
	sizeQLabel = new QLabel( sizeQComboBox, tr( "&Size:" ), dsGroupBox7, "sizeQLabel" );

	PageSize *ps=new PageSize(doc->PageSize);
	QStringList pageSizes=ps->getPageSizeList();
	sizeQComboBox->insertStringList(ps->getTrPageSizeList());
	sizeQComboBox->insertItem( tr( "Custom" ) );
	prefsPageSizeName=ps->getPageName();

	int sizeIndex=pageSizes.findIndex(ps->getPageText());
	//set Custom if we dont have one already as old docs wont have this attribute
	if (sizeIndex!=-1)
		sizeQComboBox->setCurrentItem(sizeIndex);
	else
		sizeQComboBox->setCurrentItem(sizeQComboBox->count()-1);

	dsLayout4->addWidget( sizeQLabel, 0, 0 );
	dsLayout4->addWidget( sizeQComboBox, 0, 1 );
	orientationQComboBox = new QComboBox( true, dsGroupBox7, "orientationQComboBox" );
	orientationQLabel = new QLabel( orientationQComboBox, tr( "Orie&ntation:" ), dsGroupBox7, "orientationQLabel" );
	orientationQComboBox->insertItem( tr( "Portrait" ) );
	orientationQComboBox->insertItem( tr( "Landscape" ) );
	orientationQComboBox->setCurrentItem(doc->PageOri);
	orientationQComboBox->setEditable(false);
	dsLayout4->addWidget( orientationQLabel, 0, 2 );
	dsLayout4->addWidget( orientationQComboBox, 0, 3 );
	widthMSpinBox = new MSpinBox( 1, 10000, dsGroupBox7, 2 );
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7, "widthLabel" );
	widthMSpinBox->setEnabled( false );
	widthMSpinBox->setSuffix(ein);
	widthMSpinBox->setValue(pageWidth * unitRatio);
	widthQLabel->setBuddy(widthMSpinBox);
	dsLayout4->addWidget( widthQLabel, 1, 0 );
	dsLayout4->addWidget( widthMSpinBox, 1, 1 );
	heightMSpinBox = new MSpinBox( 1, 10000, dsGroupBox7, 2 );
	heightMSpinBox->setEnabled( false );
	heightMSpinBox->setSuffix(ein);
	heightMSpinBox->setValue(pageHeight * unitRatio);
	heightQLabel = new QLabel(heightMSpinBox,  tr( "&Height:" ), dsGroupBox7, "heightLabel" );
	dsLayout4->addWidget( heightQLabel, 1, 2 );
	dsLayout4->addWidget( heightMSpinBox, 1, 3 );
	if (sizeQComboBox->currentText() == tr("Custom"))
	{
		heightMSpinBox->setEnabled( true );
		widthMSpinBox->setEnabled( true );
	}
	unitCombo = new QComboBox( true, dsGroupBox7, "unitCombo" );
	unitCombo->insertStringList(unitGetTextUnitList());
	unitCombo->setEditable(false);
	unitCombo->setCurrentItem(doc->unitIndex());
	unitQLabel = new QLabel(unitCombo, tr( "&Unit:" ), dsGroupBox7, "unitQLabel" );
	dsLayout4->addWidget( unitQLabel, 2, 0 );
	dsLayout4->addWidget( unitCombo, 2, 1 );
	
	sizeAllPages = new QCheckBox( dsGroupBox7, "moveObjects" );
	sizeAllPages->setText( tr( "Apply size settings to all pages" ) );
	sizeAllPages->setChecked( false );
	dsLayout4->addMultiCellWidget( sizeAllPages, 3, 3, 0, 3 );
	
	dsGroupBox7Layout->addLayout( dsLayout4 );
	dsLayout4pv->addWidget( dsGroupBox7 );

	GroupRand = new MarginWidget(tabPage,  tr( "Margin Guides" ), &doc->pageMargins, einheit, true );
	GroupRand->setPageWidthHeight(pageWidth, pageHeight);
	GroupRand->setPageSize(sizeQComboBox->currentText());
	dsLayout4pv->addWidget( GroupRand );
		
	dsLayout4p->addLayout( dsLayout4pv );
	reformDocLayout->addLayout( dsLayout4p );
	
	/*
	groupBox7a = new QGroupBox( tabPage, "groupBox7" );
	groupBox7a->setTitle( tr( "Options" ) );
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
	
	sizeAllPages = new QCheckBox( groupBox7a, "moveObjects" );
	sizeAllPages->setText( tr( "Apply size settings to all Pages" ) );
	sizeAllPages->setChecked( false );
	layout4a->addMultiCellWidget( sizeAllPages, 1, 1, 0, 1 );
	
	marginsForAllPages = new QCheckBox( groupBox7a, "moveObjects" );
	marginsForAllPages->setText( tr( "Apply margin settings to all Pages" ) );
	marginsForAllPages->setChecked( false );
	layout4a->addMultiCellWidget( marginsForAllPages, 1, 1, 2, 3 );
	
	TextLabel1_3 = new QLabel( tr( "F&irst Page Number:" ), groupBox7a, "TextLabel1_3" );
	layout4a->addMultiCellWidget( TextLabel1_3, 0, 0, 0, 1 );
	pageNumber = new QSpinBox( groupBox7a, "pageNumber" );
	pageNumber->setMaxValue( 10000 );
	pageNumber->setMinValue( 1 );
	pageNumber->setValue(doc->FirstPnum);
	layout4a->addWidget( pageNumber, 0, 2, Qt::AlignRight );
	TextLabel1_3->setBuddy(pageNumber);
	groupBox7aLayout->addLayout( layout4a );
	reformDocLayout->addWidget( groupBox7a );
	*/
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
	textLabel1m = new QLabel(autoSaveTime, tr( "&Interval:" ), groupAutoSave, "textLabel1m" );
	groupAutoSaveLayout->addWidget( textLabel1m );
	groupAutoSaveLayout->addWidget( autoSaveTime );
	reformDocLayout->addWidget( groupAutoSave );
	addItem( tr("Document"), loadIcon("scribusdoc.png"), tabPage);

	docInfos = new DocInfos(prefsWidgets, doc->documentInfo);
	addItem( tr("Document Information"), loadIcon("documentinfo32.png"), docInfos);

	tabGuides = new TabGuides(prefsWidgets, &doc->guidesSettings, &doc->typographicSettings, einheit);
	addItem( tr("Guides"), loadIcon("guides.png"), tabGuides);

	tabView = new QWidget( prefsWidgets, "tabView" );
	tabViewLayout = new QVBoxLayout( tabView, 0, 5, "tabViewLayout");
	pageBackground = new QButtonGroup( tabView, "pageBackground" );
	pageBackground->setTitle( tr( "Page Display" ) );
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
	QPixmap pm5(54, 14);
	pm5.fill(doc->papColor);
	colorPaper = doc->papColor;
	backColor->setPixmap(pm5);
	backColor->setText( QString::null );
	layout10->addWidget( backColor );
	QSpacerItem* spacer3 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout10->addItem( spacer3 );
	pageBackgroundLayout->addLayout( layout10 );
	checkUnprintable = new QCheckBox( pageBackground, "checkUnprintable" );
	checkUnprintable->setText( tr( "Display &Unprintable Area in Margin Color" ) );
	checkUnprintable->setAccel( QKeySequence( tr( "Alt+U" ) ) );
	checkUnprintable->setChecked( doc->marginColored );
	pageBackgroundLayout->addWidget( checkUnprintable );
	checkPictures = new QCheckBox( pageBackground, "checkPictures" );
	checkPictures->setText( tr( "Show Pictures" ) );
	checkPictures->setChecked(doc->guidesSettings.showPic);
	pageBackgroundLayout->addWidget( checkPictures );
	checkLink = new QCheckBox( pageBackground, "checkLink" );
	checkLink->setText( tr( "Show Text Chains" ) );
	checkLink->setChecked(doc->guidesSettings.linkShown);
	pageBackgroundLayout->addWidget( checkLink );
	checkControl = new QCheckBox( pageBackground, "checkControl" );
	checkControl->setText( tr( "Show Text Control Characters" ) );
	checkControl->setChecked(doc->guidesSettings.showControls);
	pageBackgroundLayout->addWidget( checkControl );
	checkFrame = new QCheckBox( pageBackground, "checkFrame" );
	checkFrame->setText( tr( "Show Frames" ) );
	checkFrame->setChecked(doc->guidesSettings.framesShown);
	pageBackgroundLayout->addWidget( checkFrame );
	checkRuler = new QCheckBox( pageBackground, "checkRuler" );
	checkRuler->setText( tr( "Rulers relative to Page" ) );
	checkRuler->setChecked(doc->guidesSettings.rulerMode);
	pageBackgroundLayout->addWidget( checkRuler );
	tabViewLayout->addWidget( pageBackground );

	groupScratch = new QGroupBox( tabView, "GroupBox7" );
	groupScratch->setTitle( tr( "Minimum Scratch Space" ) );
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
	topScratch->setValue(doc->ScratchTop * unitRatio);
	layout4s->addWidget( topScratch, 0, 1 );
	TextLabel5s = new QLabel(topScratch, tr( "&Top:" ), groupScratch, "TextLabel5" );
	layout4s->addWidget( TextLabel5s, 0, 0 );
	leftScratch = new MSpinBox( groupScratch, 4 );
	leftScratch->setSuffix( ein );
	leftScratch->setDecimals( decimals );
	leftScratch->setMaxValue(1000);
	leftScratch->setValue(doc->ScratchLeft * unitRatio);
	layout4s->addWidget( leftScratch, 0, 3 );
	Linkss = new QLabel(leftScratch, tr( "&Left:" ), groupScratch, "Links" );
	layout4s->addWidget( Linkss, 0, 2 );
	bottomScratch = new MSpinBox( groupScratch, 4 );
	bottomScratch->setSuffix( ein );
	bottomScratch->setDecimals( decimals );
	bottomScratch->setMaxValue(1000);
	bottomScratch->setValue(doc->ScratchBottom * unitRatio);
	layout4s->addWidget( bottomScratch, 1, 1 );
	TextLabel7s = new QLabel(bottomScratch, tr( "&Bottom:" ), groupScratch, "TextLabel7" );
	layout4s->addWidget( TextLabel7s, 1, 0 );
	rightScratch = new MSpinBox( groupScratch, 4 );
	rightScratch->setSuffix( ein );
	rightScratch->setDecimals( decimals );
	rightScratch->setMaxValue(1000);
	rightScratch->setValue(doc->ScratchRight * unitRatio);
	layout4s->addWidget( rightScratch, 1, 3 );
	Rechtss = new QLabel(rightScratch, tr( "&Right:" ), groupScratch, "Rechts" );
	layout4s->addWidget( Rechtss, 1, 2 );
	groupScratchLayout->addLayout( layout4s );
	tabViewLayout->addWidget( groupScratch );

	groupGap = new QGroupBox( tabView, "GroupBox7" );
	groupGap->setTitle( tr( "Gaps between Pages" ) );
	groupGap->setColumnLayout(0, Qt::Vertical );
	groupGap->layout()->setSpacing( 0 );
	groupGap->layout()->setMargin( 0 );
	groupGapLayout = new QHBoxLayout( groupGap->layout() );
	groupGapLayout->setAlignment( Qt::AlignTop );
	groupGapLayout->setSpacing( 0 );
	groupGapLayout->setMargin( 10 );
	layout4sg = new QGridLayout;
	layout4sg->setSpacing( 6 );
	layout4sg->setMargin( 0 );
	gapHorizontal = new MSpinBox( groupGap, 4 );
	gapHorizontal->setSuffix( ein );
	gapHorizontal->setDecimals( decimals );
	gapHorizontal->setMaxValue(1000);
	gapHorizontal->setValue(doc->pageSets[doc->currentPageLayout].GapHorizontal * unitRatio);
	layout4sg->addWidget( gapHorizontal, 0, 1 );
	TextLabel5sg = new QLabel(gapHorizontal, tr( "Horizontal:" ), groupGap, "TextLabel5" );
	layout4sg->addWidget( TextLabel5sg, 0, 0 );
	gapVertical = new MSpinBox( groupGap, 4 );
	gapVertical->setSuffix( ein );
	gapVertical->setDecimals( decimals );
	gapVertical->setMaxValue(1000);
	gapVertical->setValue(doc->pageSets[doc->currentPageLayout].GapBelow * unitRatio);
	layout4sg->addWidget( gapVertical, 0, 3 );
	TextLabel7sg = new QLabel(gapVertical, tr( "Vertical:" ), groupGap, "Links" );
	layout4sg->addWidget( TextLabel7sg, 0, 2 );
	groupGapLayout->addLayout( layout4sg );
	tabViewLayout->addWidget( groupGap );
	addItem( tr("Display"), loadIcon("screen.png"), tabView);

	tabTypo = new TabTypograpy(  prefsWidgets, &doc->typographicSettings);
	addItem( tr("Typography"), loadIcon("typography.png"), tabTypo);

	tabTools = new TabTools(  prefsWidgets, &doc->toolSettings, einheit, doc);
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets, &ScMW->LangTransl);
	tabHyphenator->verbose->setChecked(!doc->docHyphenator->Automatic);
	tabHyphenator->input->setChecked(doc->docHyphenator->AutoCheck);
	tabHyphenator->language->setCurrentText(ScMW->LangTransl[doc->docHyphenator->Language]);
	tabHyphenator->wordLen->setValue(doc->docHyphenator->MinWordLen);
	tabHyphenator->maxCount->setValue(doc->docHyphenator->HyCount);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);

	tabFonts = new FontPrefs(  prefsWidgets, PrefsManager::instance()->appPrefs.AvailFonts, true, ScMW->PrefsPfad, doc);
	addItem( tr("Fonts"), loadIcon("font.png"), tabFonts);

	tabDocChecker = new TabCheckDoc(  prefsWidgets, doc->checkerProfiles, doc->curCheckProfile);
	addItem( tr("Preflight Verifier"), loadIcon("checkdoc.png"), tabDocChecker);

	tabPDF = new TabPDFOptions( prefsWidgets, doc->PDF_Options, PrefsManager::instance()->appPrefs.AvailFonts,
								ScMW->PDFXProfiles, doc->UsedFonts, doc->PDF_Options.PresentVals,
								einheit, doc->pageHeight, doc->pageWidth, 0 );
	addItem( tr("PDF Export"), loadIcon("acroread.png"), tabPDF);

	tabDocItemAttributes = new DocumentItemAttributes( prefsWidgets);
	docAttributesList=tabDocItemAttributes->getDocAttributesNames();
	tabDocItemAttributes->setup(&doc->docItemAttributes);
	addItem( tr("Document Item Attributes"), loadIcon("docattributes.png"), tabDocItemAttributes);

	tabTOCIndexPrefs = new TOCIndexPrefs( prefsWidgets );
	tabTOCIndexPrefs->setupItemAttrs( docAttributesList );
	tabTOCIndexPrefs->setup(&(currDoc->docToCSetups), currDoc);
	connect( prefsWidgets, SIGNAL(aboutToShow(QWidget *)), this, SLOT(setTOCIndexData(QWidget *)));
	addItem( tr("Table of Contents and Indexes"), loadIcon("tabtocindex.png"), tabTOCIndexPrefs);

	tabDocSections = new DocSections(prefsWidgets);
	tabDocSections->setup(currDoc->sections, currDoc->DocPages.count()-1);
	addItem( tr("Sections"), loadIcon("tabtocindex.png"), tabDocSections);
	
	int cmsTab = 0;
	if (CMSavail)
	{
		tabColorManagement = new CMSPrefs(prefsWidgets, &doc->CMSSettings, &ScMW->InputProfiles, &ScMW->InputProfilesCMYK, &ScMW->PrinterProfiles, &ScMW->MonitorProfiles);
		cmsTab = addItem( tr("Color Management"), loadIcon("blend.png"), tabColorManagement);
	}

	setDS(doc->currentPageLayout);
	pageWidth = widthMSpinBox->value() / unitRatio;
	pageHeight = heightMSpinBox->value() / unitRatio;
	//tooltips
	QToolTip::add( checkLink, "<qt>" + tr("Enable or disable the display of linked text frames.") + "</qt>");
	QToolTip::add( checkControl, "<qt>" + tr("Display non-printing characters such as paragraph markers in text frames") + "</qt>");
	QToolTip::add( checkFrame, "<qt>" + tr("Turns the display of frames on or off") + "</qt>");
	QToolTip::add( checkPictures, "<qt>" + tr("Turns the display of pictures on or off") + "</qt>");
	QToolTip::add( backColor, "<qt>" + tr( "Color for paper" ) + "</qt>" );
	QToolTip::add( checkUnprintable, "<qt>" + tr( "Mask the area outside the margins in the margin color" ) + "</qt>" );
//	QToolTip::add( facingPages, "<qt>" + tr( "Enable single or spread based layout" ) + "</qt>" );
//	QToolTip::add( firstPage, "<qt>" + tr( "Make the first page the left page of the document" ) + "</qt>" );
	QToolTip::add( sizeAllPages, "<qt>" + tr( "Apply the page size changes to all existing pages in the document" ) + "</qt>" );

	// signals and slots connections
	connect(docLayout, SIGNAL( selectedLayout(int) ), this, SLOT( setDS(int) ) );
	connect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	connect(backColor, SIGNAL(clicked()), this, SLOT(changePaperColor()));
	connect(unitCombo, SIGNAL(activated(int)), this, SLOT(unitChange()));
	connect(backToDefaults, SIGNAL(clicked()), this, SLOT(restoreDefaults()));
	connect(orientationQComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(sizeQComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));

	if (CMSavail)
	{
		prefsWidgets->raiseWidget(cmsTab);
		connect(tabColorManagement, SIGNAL(cmsOn(bool )), this, SLOT(switchCMS(bool )));
	}
	arrangeIcons();
	resize( minimumSizeHint() );
	clearWState( WState_Polished );
	prefsSelection->setSelected(prefsSelection->firstItem(), true);
	itemSelected(prefsSelection->firstItem());
}

void ReformDoc::restoreDefaults()
{
	QWidget* current = prefsWidgets->visibleWidget();
	if (current == tabPage)
	{
		unitCombo->setCurrentItem(currDoc->unitIndex());
		unitChange();
		autoSaveTime->setValue(currDoc->AutoSaveTime / 1000 / 60);
		groupAutoSave->setChecked( currDoc->AutoSave );
		//pageNumber->setValue(currDoc->FirstPnum);
		docLayout->selectItem(currDoc->currentPageLayout);
		setDS(currDoc->currentPageLayout);
		docLayout->firstPage->setCurrentItem(currDoc->pageSets[currDoc->currentPageLayout].FirstPage);
		GroupRand->rightR->setValue(currDoc->pageMargins.Right * unitRatio);
		GroupRand->bottomR->setValue(currDoc->pageMargins.Bottom * unitRatio);
		GroupRand->leftR->setValue(currDoc->pageMargins.Left * unitRatio);
		GroupRand->topR->setValue(currDoc->pageMargins.Top * unitRatio);
		GroupRand->RandR = GroupRand->rightR->value() / unitRatio;
		GroupRand->RandL = GroupRand->leftR->value() / unitRatio;
		GroupRand->RandB = GroupRand->bottomR->value() / unitRatio;
		GroupRand->RandT = GroupRand->topR->value() / unitRatio;
	}
	else if (current == tabView)
	{
		QPixmap pm(54, 14);
		pm.fill(currDoc->papColor);
		colorPaper = currDoc->papColor;
		backColor->setPixmap(pm);
		checkUnprintable->setChecked( currDoc->marginColored );
		checkPictures->setChecked(currDoc->guidesSettings.showPic);
		checkLink->setChecked(currDoc->guidesSettings.linkShown);
		checkFrame->setChecked(currDoc->guidesSettings.framesShown);
		checkRuler->setChecked(currDoc->guidesSettings.rulerMode);
		topScratch->setValue(currDoc->ScratchTop * unitRatio);
		leftScratch->setValue(currDoc->ScratchLeft * unitRatio);
		bottomScratch->setValue(currDoc->ScratchBottom * unitRatio);
		rightScratch->setValue(currDoc->ScratchRight * unitRatio);
		gapHorizontal->setValue(currDoc->pageSets[currDoc->currentPageLayout].GapHorizontal * unitRatio);
		gapVertical->setValue(currDoc->pageSets[currDoc->currentPageLayout].GapBelow * unitRatio);
	}
	else if (current == tabHyphenator)
	{
		tabHyphenator->verbose->setChecked(!currDoc->docHyphenator->Automatic);
		tabHyphenator->input->setChecked(currDoc->docHyphenator->AutoCheck);
		tabHyphenator->language->setCurrentText(ScMW->LangTransl[currDoc->docHyphenator->Language]);
		tabHyphenator->wordLen->setValue(currDoc->docHyphenator->MinWordLen);
		tabHyphenator->maxCount->setValue(currDoc->docHyphenator->HyCount);
	}
	else if (current == tabGuides)
		tabGuides->restoreDefaults(&currDoc->guidesSettings, &currDoc->typographicSettings, einheit);
	else if (current == tabTypo)
		tabTypo->restoreDefaults(&currDoc->typographicSettings);
	else if (current == tabTools)
		tabTools->restoreDefaults();
	else if (current == tabFonts)
		tabFonts->restoreDefaults();
	else if (current == tabDocChecker)
		tabDocChecker->restoreDefaults();
	else if (current == tabPDF)
		tabPDF->restoreDefaults();
	else if (current == tabColorManagement)
		tabColorManagement->restoreDefaults();
	else if (current == docInfos)
		docInfos->restoreDefaults();
}

void ReformDoc::unitChange()
{
	disconnect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	int decimalsOld;
	double oldUnitRatio = unitRatio;
	double oldMin, oldMax, val;
	QString einh;
	einheit = unitCombo->currentItem();
	unitRatio=unitGetRatioFromIndex(einheit);
	decimals=unitGetDecimalsFromIndex(einheit);
	einh=unitGetSuffixFromIndex(einheit);

	widthMSpinBox->setSuffix(einh);
	heightMSpinBox->setSuffix(einh);
	tabGuides->minorSpace->setSuffix(einh);
	tabGuides->majorSpace->setSuffix(einh);
	tabGuides->snapDistance->setSuffix(einh);
	tabGuides->baseGrid->setSuffix(einh);
	tabGuides->baseOffset->setSuffix(einh);
	tabTools->gapText->setSuffix(einh);
	topScratch->setSuffix(einh);
	bottomScratch->setSuffix(einh);
	leftScratch->setSuffix(einh);
	rightScratch->setSuffix(einh);
	gapVertical->setSuffix( einh );
	gapHorizontal->setSuffix( einh );

	double invUnitConversion = 1.0 / oldUnitRatio * unitRatio;
	widthMSpinBox->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	widthMSpinBox->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, currDoc->pageWidth * unitRatio);
	heightMSpinBox->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	heightMSpinBox->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, currDoc->pageHeight * unitRatio);
	tabGuides->minorSpace->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabGuides->minorSpace->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabGuides->majorSpace->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabGuides->majorSpace->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabGuides->snapDistance->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabGuides->snapDistance->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabGuides->baseGrid->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabGuides->baseGrid->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabGuides->baseOffset->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabGuides->baseOffset->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabTools->gapText->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabTools->gapText->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabTools->gapTab->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabTools->gapTab->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	topScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	topScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	bottomScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	bottomScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	leftScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	leftScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	rightScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	rightScratch->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	gapVertical->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	gapVertical->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	gapHorizontal->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	gapHorizontal->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabPDF->unitChange(einheit);
	pageWidth = widthMSpinBox->value() / unitRatio;
	pageHeight = heightMSpinBox->value() / unitRatio;
	GroupRand->unitChange(unitRatio, decimals, einh);
	GroupRand->setPageHeight(pageHeight);
	GroupRand->setPageWidth(pageWidth);
	connect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
}

void ReformDoc::changePaperColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorPaper, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorPaper = neu;
		backColor->setPixmap(pm);
	}
}

void ReformDoc::setDS(int layout)
{
	GroupRand->setFacingPages(!(layout == singlePage));
	choosenLayout = layout;
	docLayout->firstPage->setCurrentItem(currDoc->pageSets[choosenLayout].FirstPage);
	gapHorizontal->setValue(currDoc->pageSets[choosenLayout].GapHorizontal * unitRatio);
	gapVertical->setValue(currDoc->pageSets[choosenLayout].GapBelow * unitRatio);
}

void ReformDoc::setPageWidth(int)
{
	pageWidth = widthMSpinBox->value() / unitRatio;
	GroupRand->setPageWidth(pageWidth);
}

void ReformDoc::setPageHeight(int)
{
	pageHeight = heightMSpinBox->value() / unitRatio;
	GroupRand->setPageHeight(pageHeight);
}

void ReformDoc::setPageSize()
{
	setOrien(orientationQComboBox->currentItem());
}

void ReformDoc::setSize(const QString & gr)
{
	pageWidth = widthMSpinBox->value() / unitRatio;
	pageHeight = heightMSpinBox->value() / unitRatio;
	widthMSpinBox->setEnabled(false);
	heightMSpinBox->setEnabled(false);
	PageSize *ps2=new PageSize(gr);
	prefsPageSizeName=ps2->getPageName();
	if (gr == tr("Custom"))
	{
		widthMSpinBox->setEnabled(true);
		heightMSpinBox->setEnabled(true);
	}
	else
	{
		pageWidth = ps2->getPageWidth();
		pageHeight = ps2->getPageHeight();
	}
	disconnect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	widthMSpinBox->setValue(pageWidth * unitRatio);
	heightMSpinBox->setValue(pageHeight * unitRatio);
	GroupRand->setPageHeight(pageHeight);
	GroupRand->setPageWidth(pageWidth);
	GroupRand->setPageSize(gr);
	connect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	delete ps2;
}

void ReformDoc::setOrien(int ori)
{
	double br;
	setSize(sizeQComboBox->currentText());
	disconnect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	if (ori == 0)
	{
		if (sizeQComboBox->currentText() == tr("Custom"))
		{
			br = widthMSpinBox->value();
			widthMSpinBox->setValue(heightMSpinBox->value());
			heightMSpinBox->setValue(br);
		}
	}
	else
	{
		br = widthMSpinBox->value();
		widthMSpinBox->setValue(heightMSpinBox->value());
		heightMSpinBox->setValue(br);
	}
	pageWidth = widthMSpinBox->value() / unitRatio;
	pageHeight = heightMSpinBox->value() / unitRatio;
	connect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
}

void ReformDoc::switchCMS(bool enable)
{
	tabPDF->enableCMS(enable);
}

void ReformDoc::setTOCIndexData(QWidget *widgetToShow)
{
	//Update the attributes list in TOC setup
	if (widgetToShow==tabTOCIndexPrefs)
		tabTOCIndexPrefs->setupItemAttrs( tabDocItemAttributes->getDocAttributesNames() );
}

const int ReformDoc::getSelectedUnit()
{
	return unitCombo->currentItem();
}

const bool ReformDoc::colorManagementSettingsChanged()
{
	return tabColorManagement->changed;
}

const bool ReformDoc::imageResolutionChanged()
{
	return viewToRecalcPictureRes;
}

void ReformDoc::updateDocumentSettings()
{
	double tpr2, lr2, rr2, br2;
	tpr2 = GroupRand->RandT;
	br2 = GroupRand->RandB;
	lr2 = GroupRand->RandL;
	rr2 = GroupRand->RandR;
	int fp = choosenLayout;
	currDoc->pageSets[fp].FirstPage = docLayout->firstPage->currentItem();
	currDoc->pageSets[fp].GapHorizontal = gapHorizontal->value() / currDoc->unitRatio();
	currDoc->pageSets[fp].GapBelow = gapVertical->value() / currDoc->unitRatio();
	//currDoc->FirstPnum = pageNumber->value();
	currDoc->resetPage(tpr2, lr2, rr2, br2, fp);
	currDoc->PageOri = orientationQComboBox->currentItem();
	currDoc->PageSize = prefsPageSizeName;
	currDoc->pageWidth = pageWidth;
	currDoc->pageHeight = pageHeight;
	double TopD = topScratch->value() / currDoc->unitRatio() - currDoc->ScratchTop;
	double LeftD = leftScratch->value() / currDoc->unitRatio() - currDoc->ScratchLeft;
	currDoc->ScratchBottom = bottomScratch->value() / currDoc->unitRatio();
	currDoc->ScratchLeft = leftScratch->value() / currDoc->unitRatio();
	currDoc->ScratchRight = rightScratch->value() / currDoc->unitRatio();
	currDoc->ScratchTop = topScratch->value() / currDoc->unitRatio();
	for (uint p = 0; p < currDoc->Pages->count(); ++p)
	{
		Page *pp = currDoc->Pages->at(p);
		if (sizeAllPages->isChecked())
		{
			pp->setInitialWidth(currDoc->pageWidth);
			pp->setInitialHeight(currDoc->pageHeight);
		}
		if (GroupRand->marginsForAllPages->isChecked())
		{
			pp->initialMargins.Left = lr2;
			pp->initialMargins.Right = rr2;
			pp->initialMargins.Top = tpr2;
			pp->initialMargins.Bottom = br2;
		}
	}
	for (uint p = 0; p < currDoc->MasterPages.count(); ++p)
	{
		Page *pp = currDoc->MasterPages.at(p);
		if (sizeAllPages->isChecked())
		{
			pp->setInitialWidth(currDoc->pageWidth);
			pp->setInitialHeight(currDoc->pageHeight);
		}
		if (GroupRand->marginsForAllPages->isChecked())
		{
			pp->initialMargins.Left = lr2;
			pp->initialMargins.Right = rr2;
			pp->initialMargins.Top = tpr2;
			pp->initialMargins.Bottom = br2;
		}
		pp->setXOffset(currDoc->ScratchLeft);
		pp->setYOffset(currDoc->ScratchTop);
	}
	uint docItemsCount = currDoc->MasterItems.count();
	for (uint ite = 0; ite < docItemsCount; ++ite)
	{
		PageItem *item = currDoc->MasterItems.at(ite);
		item->moveBy(LeftD, TopD);
		item->setRedrawBounding();
	}
	currDoc->guidesSettings.before = tabGuides->inBackground->isChecked();
	currDoc->marginColored = checkUnprintable->isChecked();
	currDoc->papColor = colorPaper;
	currDoc->guidesSettings.marginsShown = tabGuides->marginBox->isChecked();
	currDoc->guidesSettings.framesShown = checkFrame->isChecked();
	currDoc->guidesSettings.gridShown = tabGuides->checkGrid->isChecked();
	currDoc->guidesSettings.guidesShown = tabGuides->guideBox->isChecked();
	currDoc->guidesSettings.baseShown = tabGuides->baselineBox->isChecked();
	currDoc->guidesSettings.showPic = checkPictures->isChecked();
	currDoc->guidesSettings.linkShown = checkLink->isChecked();
	currDoc->guidesSettings.showControls = checkControl->isChecked();
	currDoc->guidesSettings.rulerMode = checkRuler->isChecked();
	currDoc->guidesSettings.grabRad = tabGuides->grabDistance->value();
	currDoc->guidesSettings.guideRad = tabGuides->snapDistance->value() / currDoc->unitRatio();
	currDoc->guidesSettings.minorGrid = tabGuides->minorSpace->value() / currDoc->unitRatio();
	currDoc->guidesSettings.majorGrid = tabGuides->majorSpace->value() / currDoc->unitRatio();
	currDoc->guidesSettings.minorColor = tabGuides->colorMinorGrid;
	currDoc->guidesSettings.majorColor = tabGuides->colorMajorGrid;
	currDoc->guidesSettings.margColor = tabGuides->colorMargin;
	currDoc->guidesSettings.guideColor = tabGuides->colorGuides;
	currDoc->guidesSettings.baseColor = tabGuides->colorBaselineGrid;
	currDoc->checkerProfiles = tabDocChecker->checkerProfile;
	currDoc->curCheckProfile = tabDocChecker->curCheckProfile->currentText();
	currDoc->typographicSettings.valueSuperScript = tabTypo->superDisplacement->value();
	currDoc->typographicSettings.scalingSuperScript = tabTypo->superScaling->value();
	currDoc->typographicSettings.valueSubScript = tabTypo->subDisplacement->value();
	currDoc->typographicSettings.scalingSubScript = tabTypo->subScaling->value();
	currDoc->typographicSettings.valueSmallCaps = tabTypo->capsScaling->value();
	currDoc->typographicSettings.autoLineSpacing = tabTypo->autoLine->value();
	currDoc->typographicSettings.valueBaseGrid = tabGuides->baseGrid->value() / currDoc->unitRatio();
	currDoc->typographicSettings.offsetBaseGrid = tabGuides->baseOffset->value() / currDoc->unitRatio();
	currDoc->typographicSettings.valueUnderlinePos = qRound(tabTypo->underlinePos->value() * 10);
	currDoc->typographicSettings.valueUnderlineWidth = qRound(tabTypo->underlineWidth->value() * 10);
	currDoc->typographicSettings.valueStrikeThruPos = qRound(tabTypo->strikethruPos->value() * 10);
	currDoc->typographicSettings.valueStrikeThruWidth = qRound(tabTypo->strikethruWidth->value() * 10);
	currDoc->toolSettings.defFont = tabTools->fontComboText->currentText();
	currDoc->toolSettings.defSize = tabTools->sizeComboText->currentText().left(2).toInt() * 10;
	currDoc->toolSettings.dStrokeText = tabTools->colorComboStrokeText->currentText();
	switch (tabTools->tabFillCombo->currentItem())
	{
		case 0:
			currDoc->toolSettings.tabFillChar = "";
			break;
		case 1:
			currDoc->toolSettings.tabFillChar = ".";
			break;
		case 2:
			currDoc->toolSettings.tabFillChar = "-";
			break;
		case 3:
			currDoc->toolSettings.tabFillChar = "_";
			break;
		case 4:
			currDoc->toolSettings.tabFillChar = tabTools->tabFillCombo->currentText().right(1);
			break;
	}
	// TODO fix these tr("None") things
	if (currDoc->toolSettings.dStrokeText == ScMW->noneString)
		currDoc->toolSettings.dStrokeText = "None";
	currDoc->toolSettings.dPenText = tabTools->colorComboText->currentText();
	if (currDoc->toolSettings.dPenText == ScMW->noneString)
		currDoc->toolSettings.dPenText = "None";
	currDoc->toolSettings.dTextBackGround = tabTools->colorComboTextBackground->currentText();
	if (currDoc->toolSettings.dTextBackGround == ScMW->noneString)
		currDoc->toolSettings.dTextBackGround = "None";
	currDoc->toolSettings.dTextLineColor = tabTools->colorComboTextLine->currentText();
	if (currDoc->toolSettings.dTextLineColor == ScMW->noneString)
		currDoc->toolSettings.dTextLineColor = "None";
	currDoc->toolSettings.dTextBackGroundShade = tabTools->shadingTextBack->value();
	currDoc->toolSettings.dTextLineShade = tabTools->shadingTextLine->value();
	currDoc->toolSettings.dTextPenShade = tabTools->shadingText->value();
	currDoc->toolSettings.dTextStrokeShade = tabTools->shadingTextStroke->value();
	currDoc->toolSettings.dCols = tabTools->columnsText->value();
	currDoc->toolSettings.dGap = tabTools->gapText->value() / currDoc->unitRatio();
	currDoc->toolSettings.dTabWidth = tabTools->gapTab->value() / currDoc->unitRatio();
	currDoc->toolSettings.dPen = tabTools->colorComboLineShape->currentText();
	if (currDoc->toolSettings.dPen == ScMW->noneString)
		currDoc->toolSettings.dPen = "None";
	currDoc->toolSettings.dBrush = tabTools->comboFillShape->currentText();
	if (currDoc->toolSettings.dBrush == ScMW->noneString)
		currDoc->toolSettings.dBrush = "None";
	currDoc->toolSettings.dShade = tabTools->shadingFillShape->value();
	currDoc->toolSettings.dShade2 = tabTools->shadingLineShape->value();
	switch (tabTools->comboStyleShape->currentItem())
	{
	case 0:
		currDoc->toolSettings.dLineArt = SolidLine;
		break;
	case 1:
		currDoc->toolSettings.dLineArt = DashLine;
		break;
	case 2:
		currDoc->toolSettings.dLineArt = DotLine;
		break;
	case 3:
		currDoc->toolSettings.dLineArt = DashDotLine;
		break;
	case 4:
		currDoc->toolSettings.dLineArt = DashDotDotLine;
		break;
	}
	currDoc->toolSettings.dWidth = tabTools->lineWidthShape->value();
	currDoc->toolSettings.dStartArrow = tabTools->startArrow->currentItem();
	currDoc->toolSettings.dEndArrow = tabTools->endArrow->currentItem();
	currDoc->toolSettings.magMin = tabTools->minimumZoom->value();
	currDoc->toolSettings.magMax = tabTools->maximumZoom->value();
	currDoc->toolSettings.magStep = tabTools->zoomStep->value();
	currDoc->toolSettings.dPenLine = tabTools->colorComboLine->currentText();
	if (currDoc->toolSettings.dPenLine == ScMW->noneString)
		currDoc->toolSettings.dPenLine = "None";
	currDoc->toolSettings.dShadeLine = tabTools->shadingLine->value();
	switch (tabTools->comboStyleLine->currentItem())
	{
	case 0:
		currDoc->toolSettings.dLstyleLine = SolidLine;
		break;
	case 1:
		currDoc->toolSettings.dLstyleLine = DashLine;
		break;
	case 2:
		currDoc->toolSettings.dLstyleLine = DotLine;
		break;
	case 3:
		currDoc->toolSettings.dLstyleLine = DashDotLine;
		break;
	case 4:
		currDoc->toolSettings.dLstyleLine = DashDotDotLine;
		break;
	}
	currDoc->toolSettings.dWidthLine = tabTools->lineWidthLine->value();
	currDoc->toolSettings.dBrushPict = tabTools->comboFillImage->currentText();
	if (currDoc->toolSettings.dBrushPict == ScMW->noneString)
		currDoc->toolSettings.dBrushPict = "None";
	currDoc->toolSettings.shadePict = tabTools->shadingFillImage->value();
	currDoc->toolSettings.scaleX = static_cast<double>(tabTools->scalingHorizontal->value()) / 100.0;
	currDoc->toolSettings.scaleY = static_cast<double>(tabTools->scalingVertical->value()) / 100.0;
	currDoc->toolSettings.scaleType = tabTools->buttonGroup3->isChecked();
	currDoc->toolSettings.aspectRatio = tabTools->checkRatioImage->isChecked();
	currDoc->toolSettings.useEmbeddedPath = tabTools->embeddedPath->isChecked();
	int haRes = 0;
	if (tabTools->checkFullRes->isChecked())
		haRes = 0;
	if (tabTools->checkNormalRes->isChecked())
		haRes = 1;
	if (tabTools->checkHalfRes->isChecked())
		haRes = 2;
	if (currDoc->toolSettings.lowResType != haRes)
	{
		currDoc->toolSettings.lowResType = haRes;
		viewToRecalcPictureRes=true;
	}
	else
		viewToRecalcPictureRes=false;
	tabTools->polyWidget->getValues(&currDoc->toolSettings.polyC, &currDoc->toolSettings.polyFd, &currDoc->toolSettings.polyF, &currDoc->toolSettings.polyS, &currDoc->toolSettings.polyR);
	currDoc->AutoSave = groupAutoSave->isChecked();
	currDoc->AutoSaveTime = autoSaveTime->value() * 60 * 1000;
	if (currDoc->AutoSave)
	{
		currDoc->autoSaveTimer->stop();
		currDoc->autoSaveTimer->start(currDoc->AutoSaveTime);
	}
	currDoc->docHyphenator->slotNewDict(ScMW->GetLang(tabHyphenator->language->currentText()));
	currDoc->docHyphenator->slotNewSettings(tabHyphenator->wordLen->value(),
																!tabHyphenator->verbose->isChecked(),
																tabHyphenator->input->isChecked(),
																tabHyphenator->maxCount->value());
	if (CMSavail)
	{
		bool oldCM = currDoc->CMSSettings.CMSinUse;
		tabColorManagement->setValues();
		if (tabColorManagement->changed)
		{
			ScMW->mainWindowStatusLabel->setText( tr("Adjusting Colors"));
			ScMW->mainWindowProgressBar->reset();
			int cc = currDoc->PageColors.count() + currDoc->Items->count();
			ScMW->mainWindowProgressBar->setTotalSteps(cc);
#ifdef HAVE_CMS
			currDoc->HasCMS = currDoc->CMSSettings.CMSinUse;
			currDoc->SoftProofing = currDoc->CMSSettings.SoftProofOn;
			currDoc->Gamut = currDoc->CMSSettings.GamutCheck;
			currDoc->IntentPrinter = currDoc->CMSSettings.DefaultIntentPrinter;
			currDoc->IntentMonitor = currDoc->CMSSettings.DefaultIntentMonitor;
			CMSuse = currDoc->CMSSettings.CMSinUse;
			SoftProofing = currDoc->CMSSettings.SoftProofOn;
			Gamut = currDoc->CMSSettings.GamutCheck;
			BlackPoint = currDoc->CMSSettings.BlackPoint;
			IntentPrinter = currDoc->CMSSettings.DefaultIntentPrinter;
			IntentMonitor = currDoc->CMSSettings.DefaultIntentMonitor;
			qApp->setOverrideCursor(QCursor(waitCursor), true);
			bool newCM = currDoc->CMSSettings.CMSinUse;
			currDoc->CMSSettings.CMSinUse = oldCM;
			currDoc->CloseCMSProfiles();
			currDoc->CMSSettings.CMSinUse = newCM;
			currDoc->OpenCMSProfiles(ScMW->InputProfiles, ScMW->MonitorProfiles, ScMW->PrinterProfiles);
			stdProofG = currDoc->stdProof;
			stdTransG = currDoc->stdTrans;
			stdProofImgG = currDoc->stdProofImg;
			stdTransImgG = currDoc->stdTransImg;
			stdProofCMYKG = currDoc->stdProofCMYK;
			stdTransCMYKG = currDoc->stdTransCMYK;
			stdProofGCG = currDoc->stdProofGC;
			stdProofCMYKGCG = currDoc->stdProofCMYKGC;
			stdTransRGBG = currDoc->stdTransRGB;
			CMSoutputProf = currDoc->DocOutputProf;
			CMSprinterProf = currDoc->DocPrinterProf;
			if (static_cast<int>(cmsGetColorSpace(currDoc->DocInputProf)) == icSigRgbData)
				currDoc->CMSSettings.ComponentsInput2 = 3;
			if (static_cast<int>(cmsGetColorSpace(currDoc->DocInputProf)) == icSigCmykData)
				currDoc->CMSSettings.ComponentsInput2 = 4;
			if (static_cast<int>(cmsGetColorSpace(currDoc->DocInputProf)) == icSigCmyData)
				currDoc->CMSSettings.ComponentsInput2 = 3;
			if (static_cast<int>(cmsGetColorSpace(currDoc->DocPrinterProf)) == icSigRgbData)
				currDoc->CMSSettings.ComponentsPrinter = 3;
			if (static_cast<int>(cmsGetColorSpace(currDoc->DocPrinterProf)) == icSigCmykData)
				currDoc->CMSSettings.ComponentsPrinter = 4;
			if (static_cast<int>(cmsGetColorSpace(currDoc->DocPrinterProf)) == icSigCmyData)
				currDoc->CMSSettings.ComponentsPrinter = 3;
			currDoc->PDF_Options.SComp = currDoc->CMSSettings.ComponentsInput2;
			currDoc->PDF_Options.SolidProf = currDoc->CMSSettings.DefaultSolidColorProfile;
			currDoc->PDF_Options.ImageProf = currDoc->CMSSettings.DefaultImageRGBProfile;
			currDoc->PDF_Options.PrintProf = currDoc->CMSSettings.DefaultPrinterProfile;
			currDoc->PDF_Options.Intent = currDoc->CMSSettings.DefaultIntentMonitor;
			ScMW->recalcColors(ScMW->mainWindowProgressBar);
			currDoc->RecalcPictures(&ScMW->InputProfiles, &ScMW->InputProfilesCMYK, ScMW->mainWindowProgressBar);
#endif
			ScMW->mainWindowProgressBar->setProgress(cc);
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			ScMW->mainWindowStatusLabel->setText("");
			ScMW->mainWindowProgressBar->reset();
		}
	}
	PrefsManager* prefsManager=PrefsManager::instance();
	SCFontsIterator it(prefsManager->appPrefs.AvailFonts);
	for ( ; it.current() ; ++it)
	{
		it.current()->EmbedPS = tabFonts->fontFlags[it.currentKey()].FlagPS;
		it.current()->UseFont = tabFonts->fontFlags[it.currentKey()].FlagUse;
		it.current()->Subset = tabFonts->fontFlags[it.currentKey()].FlagSub;
	}
	uint a = 0;
	prefsManager->appPrefs.GFontSub.clear();
	QMap<QString,QString>::Iterator itfsu;
	QMap<QString,QString>::Iterator itfsuend=tabFonts->RList.end();
	for (itfsu = tabFonts->RList.begin(); itfsu != itfsuend; ++itfsu)
		prefsManager->appPrefs.GFontSub[itfsu.key()] = tabFonts->FlagsRepl.at(a++)->currentText();
	QStringList uf = currDoc->UsedFonts.keys();
	QMap<QString,int>::Iterator it3;
	for (it3 = currDoc->UsedFonts.begin(); it3 != currDoc->UsedFonts.end(); ++it3)
		FT_Done_Face(currDoc->FFonts[it3.key()]);
	currDoc->UsedFonts.clear();
	QStringList::Iterator it3a;
	QStringList::Iterator it3aend=uf.end();
	for (it3a = uf.begin(); it3a != it3aend; ++it3a)
		currDoc->AddFont(*it3a);

	currDoc->PDF_Options.Thumbnails = tabPDF->CheckBox1->isChecked();
	currDoc->PDF_Options.Compress = tabPDF->Compression->isChecked();
	currDoc->PDF_Options.CompressMethod = tabPDF->CMethod->currentItem();
	currDoc->PDF_Options.Quality = tabPDF->CQuality->currentItem();
	currDoc->PDF_Options.Resolution = tabPDF->Resolution->value();
	currDoc->PDF_Options.RecalcPic = tabPDF->DSColor->isChecked();
	currDoc->PDF_Options.PicRes = tabPDF->ValC->value();
	currDoc->PDF_Options.Bookmarks = tabPDF->CheckBM->isChecked();
	currDoc->PDF_Options.Binding = tabPDF->ComboBind->currentItem();
	currDoc->PDF_Options.MirrorH = tabPDF->MirrorH->isOn();
	currDoc->PDF_Options.MirrorV = tabPDF->MirrorV->isOn();
	currDoc->PDF_Options.RotateDeg = tabPDF->RotateDeg->currentItem() * 90;
	currDoc->PDF_Options.Articles = tabPDF->Article->isChecked();
	currDoc->PDF_Options.Encrypt = tabPDF->Encry->isChecked();
	currDoc->PDF_Options.UseLPI = tabPDF->UseLPI->isChecked();
	currDoc->PDF_Options.useLayers = tabPDF->useLayers->isChecked();
	currDoc->PDF_Options.UseSpotColors = !tabPDF->useSpot->isChecked();
	currDoc->PDF_Options.doMultiFile = false;
	currDoc->PDF_Options.BleedBottom = tabPDF->BleedBottom->value() / currDoc->unitRatio();
	currDoc->PDF_Options.BleedTop = tabPDF->BleedTop->value() / currDoc->unitRatio();
	currDoc->PDF_Options.BleedLeft = tabPDF->BleedLeft->value() / currDoc->unitRatio();
	currDoc->PDF_Options.BleedRight = tabPDF->BleedRight->value() / currDoc->unitRatio();
	/* DISABLED CR 2006-12-07 Work around #2964
	currDoc->PDF_Options.displayBookmarks = tabPDF->useBookmarks->isChecked();
	currDoc->PDF_Options.displayFullscreen = tabPDF->useFullScreen->isChecked();
	currDoc->PDF_Options.displayLayers = tabPDF->useLayers2->isChecked();
	currDoc->PDF_Options.displayThumbs = tabPDF->useThumbnails->isChecked();
	int pgl = PDFOptions::SinglePage;
	if (tabPDF->singlePage->isChecked())
		pgl = PDFOptions::SinglePage;
	else if (tabPDF->continuousPages->isChecked())
		pgl = PDFOptions::OneColumn;
	else if (tabPDF->doublePageLeft->isChecked())
		pgl = PDFOptions::TwoColumnLeft;
	else if (tabPDF->doublePageRight->isChecked())
		pgl = PDFOptions::TwoColumnRight;
	currDoc->PDF_Options.PageLayout = pgl;
	*/
	currDoc->PDF_Options.openAction = "";
	if (tabPDF->Encry->isChecked())
	{
		int Perm = -64;
		if (tabPDF->PDFVersionCombo->currentItem() == 1)
			Perm &= ~0x00240000;
		if (tabPDF->PrintSec->isChecked())
			Perm += 4;
		if (tabPDF->ModifySec->isChecked())
			Perm += 8;
		if (tabPDF->CopySec->isChecked())
			Perm += 16;
		if (tabPDF->AddSec->isChecked())
			Perm += 32;
		currDoc->PDF_Options.Permissions = Perm;
		currDoc->PDF_Options.PassOwner = tabPDF->PassOwner->text();
		currDoc->PDF_Options.PassUser = tabPDF->PassUser->text();
	}
	if (tabPDF->PDFVersionCombo->currentItem() == 0)
		currDoc->PDF_Options.Version = PDFOptions::PDFVersion_13;
	if (tabPDF->PDFVersionCombo->currentItem() == 1)
		currDoc->PDF_Options.Version = PDFOptions::PDFVersion_14;
	if (tabPDF->PDFVersionCombo->currentItem() == 2)
		currDoc->PDF_Options.Version = PDFOptions::PDFVersion_15;
	if (tabPDF->PDFVersionCombo->currentItem() == 3)
		currDoc->PDF_Options.Version = PDFOptions::PDFVersion_X3;
	if (tabPDF->OutCombo->currentItem() == 0)
	{
		currDoc->PDF_Options.isGrayscale = false;
		currDoc->PDF_Options.UseRGB = true;
		currDoc->PDF_Options.UseProfiles = false;
		currDoc->PDF_Options.UseProfiles2 = false;
	}
	else
	{
		if (tabPDF->OutCombo->currentItem() == 3)
		{
			currDoc->PDF_Options.isGrayscale = true;
			currDoc->PDF_Options.UseRGB = false;
			currDoc->PDF_Options.UseProfiles = false;
			currDoc->PDF_Options.UseProfiles2 = false;
		}
		else
		{
			currDoc->PDF_Options.isGrayscale = false;
			currDoc->PDF_Options.UseRGB = false;
#ifdef HAVE_CMS
			if (CMSuse)
			{
				currDoc->PDF_Options.UseProfiles = tabPDF->EmbedProfs->isChecked();
				currDoc->PDF_Options.UseProfiles2 = tabPDF->EmbedProfs2->isChecked();
				currDoc->PDF_Options.Intent = tabPDF->IntendS->currentItem();
				currDoc->PDF_Options.Intent2 = tabPDF->IntendI->currentItem();
				currDoc->PDF_Options.EmbeddedI = tabPDF->NoEmbedded->isChecked();
				currDoc->PDF_Options.SolidProf = tabPDF->SolidPr->currentText();
				currDoc->PDF_Options.ImageProf = tabPDF->ImageP->currentText();
				currDoc->PDF_Options.PrintProf = tabPDF->PrintProfC->currentText();
			}
#endif
		}
	}

	currDoc->documentInfo = docInfos->getDocInfo();
	currDoc->docItemAttributes = *(tabDocItemAttributes->getNewAttributes());
	currDoc->docToCSetups = *(tabTOCIndexPrefs->getNewToCs());
	currDoc->sections = tabDocSections->getNewSections();

	uint itemCount=currDoc->Items->count();
	for (uint b=0; b<itemCount; ++b)
	{
		if (currDoc->Items->at(b)->itemType() == PageItem::ImageFrame)
			currDoc->Items->at(b)->PicArt = currDoc->guidesSettings.showPic;
	}
}
