#include "reformdoc.h"
#include "reformdoc.moc"
#include <qtooltip.h>
#include <qspinbox.h>
#include <qcolordialog.h>
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

extern QPixmap loadIcon(QString nam);
extern bool CMSavail;
extern ProfilesL InputProfiles;

ReformDoc::ReformDoc( QWidget* parent, ScribusDoc* doc ) : PrefsDialogBase( parent )
{
	einheit = doc->docUnitIndex;
	currDoc = doc;
	ap = (ScribusApp*)parent;
	unitRatio = doc->unitRatio;
	QString ein = unitGetSuffixFromIndex(doc->docUnitIndex);
	decimals = unitGetDecimalsFromIndex(doc->docUnitIndex);
	pageWidth = doc->pageWidth * unitRatio;
	pageHeight = doc->pageHeight * unitRatio;
	setCaption( tr( "Document Setup" ) );
	tabPage = new QWidget( prefsWidgets, "tab" );
	reformDocLayout = new QVBoxLayout( tabPage );
	reformDocLayout->setSpacing( 5 );
	reformDocLayout->setMargin( 0 );
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
	widthMSpinBox = new MSpinBox( 1, 100000, dsGroupBox7, 2 );
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7, "widthLabel" );
	widthMSpinBox->setEnabled( false );
	widthMSpinBox->setSuffix(ein);
	widthMSpinBox->setValue(pageWidth);
	widthQLabel->setBuddy(widthMSpinBox);
	dsLayout4->addWidget( widthQLabel, 1, 0 );
	dsLayout4->addWidget( widthMSpinBox, 1, 1 );
	heightMSpinBox = new MSpinBox( 1, 100000, dsGroupBox7, 2 );
	heightMSpinBox->setEnabled( false );
	heightMSpinBox->setSuffix(ein);
	heightMSpinBox->setValue(pageHeight);
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
	unitCombo->setCurrentItem(doc->docUnitIndex);
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
	topR->setValue(doc->pageMargins.Top * unitRatio);
	layout4->addWidget( topR, 0, 1 );
	TextLabel5 = new QLabel( tr( "&Top:" ), groupBox7, "TextLabel5" );
	TextLabel5->setBuddy(topR);
	layout4->addWidget( TextLabel5, 0, 0 );
	leftR = new MSpinBox( groupBox7, 4 );
	leftR->setSuffix( ein );
	leftR->setDecimals( decimals );
	leftR->setMaxValue(pageWidth);
	leftR->setValue(doc->pageMargins.Left * unitRatio);
	layout4->addWidget( leftR, 0, 3 );
	Links = new QLabel( tr( "&Left:" ), groupBox7, "Links" );
	Links->setBuddy(leftR);
	layout4->addWidget( Links, 0, 2 );
	bottomR = new MSpinBox( groupBox7, 4 );
	bottomR->setSuffix( ein );
	bottomR->setDecimals( decimals );
	bottomR->setMaxValue(pageHeight);
	bottomR->setValue(doc->pageMargins.Bottom * unitRatio);
	layout4->addWidget( bottomR, 1, 1 );
	TextLabel7 = new QLabel( tr( "&Bottom:" ), groupBox7, "TextLabel7" );
	TextLabel7->setBuddy(bottomR);
	layout4->addWidget( TextLabel7, 1, 0 );
	rightR = new MSpinBox( groupBox7, 4 );
	rightR->setSuffix( ein );
	rightR->setDecimals( decimals );
	rightR->setMaxValue(pageWidth);
	rightR->setValue(doc->pageMargins.Right * unitRatio);
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

	docInfos = new DocInfos(prefsWidgets, doc->documentInfo);
	addItem( tr("Document Information"), loadIcon("documentinfo32.png"), docInfos);

	tabGuides = new TabGuides(prefsWidgets, &doc->guidesSettings, &doc->typographicSetttings, einheit);
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
	QPixmap pm5 = QPixmap(54, 14);
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
	addItem( tr("Display"), loadIcon("screen.png"), tabView);

	tabTypo = new TabTypograpy(  prefsWidgets, &doc->typographicSetttings);
	addItem( tr("Typography"), loadIcon("font.png"), tabTypo);

	tabTools = new TabTools(  prefsWidgets, &doc->toolSettings, einheit, doc);
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets, &ap->LangTransl);
	tabHyphenator->verbose->setChecked(!doc->docHyphenator->Automatic);
	tabHyphenator->input->setChecked(doc->docHyphenator->AutoCheck);
	tabHyphenator->language->setCurrentText(ap->LangTransl[doc->docHyphenator->Language]);
	tabHyphenator->wordLen->setValue(doc->docHyphenator->MinWordLen);
	tabHyphenator->maxCount->setValue(doc->docHyphenator->HyCount);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);

	tabFonts = new FontPrefs(  prefsWidgets, ap->Prefs.AvailFonts, true, &ap->Prefs, ap->PrefsPfad, doc);
	addItem( tr("Fonts"), loadIcon("font.png"), tabFonts);

	tabDocChecker = new TabCheckDoc(  prefsWidgets, doc->checkerProfiles, doc->curCheckProfile);
	addItem( tr("Document Checker"), loadIcon("checkdoc.png"), tabDocChecker);

	tabPDF = new TabPDFOptions( prefsWidgets, &doc->PDF_Options, ap->Prefs.AvailFonts,
								&ap->PDFXProfiles, doc->UsedFonts, doc->PDF_Options.PresentVals,
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
	RandR = rightR->value() / unitRatio;
	RandL = leftR->value() / unitRatio;
	RandB = bottomR->value() / unitRatio;
	RandT = topR->value() / unitRatio;
	pageWidth = widthMSpinBox->value() / unitRatio;
	pageHeight = heightMSpinBox->value() / unitRatio;
	//tooltips
	QToolTip::add( checkLink, "<qt>" + tr("Turns the of linked frames on or off") + "</qt>");
	QToolTip::add( checkControl, "<qt>" + tr("Display non-printing characters such as paragraph markers in text frames") + "</qt>");
	QToolTip::add( checkFrame, "<qt>" + tr("Turns the display of frames on or off") + "</qt>");
	QToolTip::add( checkPictures, "<qt>" + tr("Turns the display of pictures on or off") + "</qt>");
	QToolTip::add( backColor, "<qt>" + tr( "Color for paper" ) + "</qt>" );
	QToolTip::add( checkUnprintable, "<qt>" + tr( "Mask the area outside the margins in the margin color" ) + "</qt>" );
	QToolTip::add( facingPages, "<qt>" + tr( "Enable single or spread based layout" ) + "</qt>" );
	QToolTip::add( firstPage, "<qt>" + tr( "Make the first page the left page of the document" ) + "</qt>" );
	QToolTip::add( topR, "<qt>" + tr( "Distance between the top margin guide and the edge of the page" ) + "</qt>" );
	QToolTip::add( bottomR, "<qt>" + tr( "Distance between the bottom margin guide and the edge of the page" ) + "</qt>" );
	QToolTip::add( leftR, "<qt>" + tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" )  + "</qt>");
	QToolTip::add( rightR, "<qt>" + tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) + "</qt>" );

	// signals and slots connections
	connect( facingPages, SIGNAL( clicked() ), this, SLOT( setDS() ) );
	connect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	connect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	connect(backColor, SIGNAL(clicked()), this, SLOT(changePaperColor()));
	connect(unitCombo, SIGNAL(activated(int)), this, SLOT(unitChange()));
	connect(backToDefaults, SIGNAL(clicked()), this, SLOT(restoreDefaults()));
	connect(orientationQComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(sizeQComboBox, SIGNAL(activated(const QString &)), this, SLOT(setSize(const QString &)));

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
		unitCombo->setCurrentItem(currDoc->docUnitIndex);
		unitChange();
		autoSaveTime->setValue(currDoc->AutoSaveTime / 1000 / 60);
		groupAutoSave->setChecked( currDoc->AutoSave );
		pageNumber->setValue(currDoc->FirstPnum);
		firstPage->setChecked( currDoc->FirstPageLeft );
		facingPages->setChecked( currDoc->PageFP );
		rightR->setValue(currDoc->pageMargins.Right * unitRatio);
		bottomR->setValue(currDoc->pageMargins.Bottom * unitRatio);
		leftR->setValue(currDoc->pageMargins.Left * unitRatio);
		topR->setValue(currDoc->pageMargins.Top * unitRatio);
		RandR = rightR->value() / unitRatio;
		RandL = leftR->value() / unitRatio;
		RandB = bottomR->value() / unitRatio;
		RandT = topR->value() / unitRatio;
	}
	else if (current == tabView)
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(currDoc->papColor);
		colorPaper = currDoc->papColor;
		backColor->setPixmap(pm);
		checkUnprintable->setChecked( currDoc->marginColored );
		checkPictures->setChecked(currDoc->guidesSettings.showPic);
		checkLink->setChecked(currDoc->guidesSettings.linkShown);
		checkFrame->setChecked(currDoc->guidesSettings.framesShown);
		topScratch->setValue(currDoc->ScratchTop * unitRatio);
		leftScratch->setValue(currDoc->ScratchLeft * unitRatio);
		bottomScratch->setValue(currDoc->ScratchBottom * unitRatio);
		rightScratch->setValue(currDoc->ScratchRight * unitRatio);
	}
	else if (current == tabHyphenator)
	{
		tabHyphenator->verbose->setChecked(!currDoc->docHyphenator->Automatic);
		tabHyphenator->input->setChecked(currDoc->docHyphenator->AutoCheck);
		tabHyphenator->language->setCurrentText(ap->LangTransl[currDoc->docHyphenator->Language]);
		tabHyphenator->wordLen->setValue(currDoc->docHyphenator->MinWordLen);
		tabHyphenator->maxCount->setValue(currDoc->docHyphenator->HyCount);
	}
	else if (current == tabGuides)
		tabGuides->restoreDefaults(&currDoc->guidesSettings, &currDoc->typographicSetttings, einheit);
	else if (current == tabTypo)
		tabTypo->restoreDefaults(&currDoc->typographicSetttings);
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
	disconnect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	disconnect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	disconnect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	disconnect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
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
	topR->setSuffix(einh);
	bottomR->setSuffix(einh);
	leftR->setSuffix(einh);
	rightR->setSuffix(einh);
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
	tabPDF->BleedBottom->setSuffix(einh);
	tabPDF->BleedTop->setSuffix(einh);
	tabPDF->BleedRight->setSuffix(einh);
	tabPDF->BleedLeft->setSuffix(einh);

	double invUnitConversion = 1.0 / oldUnitRatio * unitRatio;

	widthMSpinBox->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	widthMSpinBox->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, currDoc->pageWidth * unitRatio);
	heightMSpinBox->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	heightMSpinBox->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, currDoc->pageHeight * unitRatio);
	topR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	topR->setValues(0, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	bottomR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	bottomR->setValues(0, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	leftR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	leftR->setValues(0, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	rightR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	rightR->setValues(0, oldMax * invUnitConversion, decimals, val * invUnitConversion);
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
	tabPDF->BleedBottom->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabPDF->BleedBottom->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabPDF->BleedTop->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabPDF->BleedTop->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabPDF->BleedRight->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabPDF->BleedRight->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabPDF->BleedLeft->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabPDF->BleedLeft->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabPDF->unitRatio = unitRatio;
	pageWidth = widthMSpinBox->value() / unitRatio;
	pageHeight = heightMSpinBox->value() / unitRatio;
	rightR->setMaxValue(pageWidth - leftR->value());
	leftR->setMaxValue(pageWidth - rightR->value());
	topR->setMaxValue(pageHeight - bottomR->value());
	bottomR->setMaxValue(pageHeight - topR->value());
	connect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	connect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
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

void ReformDoc::setTop(int )
{
	RandT = topR->value() / unitRatio;
	bottomR->setMaxValue(pageHeight - topR->value());
}

void ReformDoc::setBottom(int )
{
	RandB = bottomR->value() / unitRatio;
	topR->setMaxValue(pageHeight - bottomR->value());
}

void ReformDoc::setLeft(int )
{
	RandL = leftR->value() / unitRatio;
	rightR->setMaxValue(pageWidth - leftR->value());
}

void ReformDoc::setRight(int )
{
	RandR = rightR->value() / unitRatio;
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

void ReformDoc::setPageWidth(int)
{
	pageWidth = widthMSpinBox->value() / unitRatio;
	rightR->setMaxValue(widthMSpinBox->value() - leftR->value());
	leftR->setMaxValue(widthMSpinBox->value() - rightR->value());
	topR->setMaxValue(heightMSpinBox->value() - bottomR->value());
	bottomR->setMaxValue(heightMSpinBox->value() - topR->value());
}

void ReformDoc::setPageHeight(int)
{
	pageHeight = heightMSpinBox->value() / unitRatio;
	rightR->setMaxValue(widthMSpinBox->value() - leftR->value());
	leftR->setMaxValue(widthMSpinBox->value() - rightR->value());
	topR->setMaxValue(heightMSpinBox->value() - bottomR->value());
	bottomR->setMaxValue(heightMSpinBox->value() - topR->value());
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
	rightR->setMaxValue(widthMSpinBox->value() - leftR->value());
	leftR->setMaxValue(widthMSpinBox->value() - rightR->value());
	topR->setMaxValue(heightMSpinBox->value() - bottomR->value());
	bottomR->setMaxValue(heightMSpinBox->value() - topR->value());
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
