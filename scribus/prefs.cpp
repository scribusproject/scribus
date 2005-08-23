#include "prefs.h"
#include "prefs.moc"
#include "keymanager.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include <qfont.h>
#include <qcolordialog.h>
#include <qcombobox.h>
#include <qcolor.h>
#include <qstringlist.h>
#include <qstylefactory.h>
#include <qpointarray.h>
#include <qpainter.h>
#include <qrect.h>
#include <qstring.h>
#include <qwmatrix.h>
#include <cmath>
#include "units.h"
#include "langmgr.h"
#include "tabtypography.h"
#include "tabguides.h"
#include "hysettings.h"
#include "cmsprefs.h"
#include "keymanager.h"
#include "tabtools.h"
#include "undomanager.h"
#include "tabcheckdoc.h"
#include "tabpdfoptions.h"
#include "fontprefs.h"
#include "units.h"
#include "pluginmanager.h"
#include "pagesize.h"
#include "docitemattrprefs.h"
#include "tocindexprefs.h"
#include "marginWidget.h"
#include "prefsmanager.h"
#include "polygonwidget.h"
#include "fontcombo.h"
#include "linecombo.h"
#include "arrowchooser.h"
#include "pagelayout.h"

using namespace std;

extern QPixmap loadIcon(QString nam);
extern bool CMSavail;
extern bool CMSuse;
extern ScribusApp *ScApp;

/*!
 \fn Preferences::Preferences( QWidget* parent)
 \author Franz Schmid
 \date
 \brief Constructor for Preferences dialog box
 \param parent QWidget pointer to parent window
 \param prefsData->ApplicationPrefs * struct
 \retval Preferences dialog
 */
Preferences::Preferences( QWidget* parent) : PrefsDialogBase( parent )
{
	prefsManager=PrefsManager::instance();
	ApplicationPrefs* prefsData=&(prefsManager->appPrefs);
	int decimals;
	ap = (ScribusApp*)parent;
	docUnitIndex = prefsData->docUnitIndex;
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
	decimals = unitGetPrecisionFromIndex(docUnitIndex);

	DisScale = prefsData->DisScale;
	setCaption( tr( "Preferences" ) );

	tab = new QWidget( prefsWidgets, "tab" );
	tabLayout = new QGridLayout( tab );
	tabLayout->setSpacing( 5 );
	tabLayout->setMargin( 0 );
	tabLayout->setAlignment( Qt::AlignTop );

	// GUI Group
	ButtonGroup1 = new QButtonGroup( tr( "GUI" ), tab, "ButtonGroup1" );
	ButtonGroup1->setColumnLayout(0, Qt::Vertical );
	ButtonGroup1->layout()->setSpacing( 0 );
	ButtonGroup1->layout()->setMargin( 0 );
	ButtonGroup1Layout = new QGridLayout( ButtonGroup1->layout() );
	ButtonGroup1Layout->setAlignment( Qt::AlignTop );
	ButtonGroup1Layout->setSpacing( 6 );
	ButtonGroup1Layout->setMargin( 10 );

	langMgr.init();
	QStringList languageList;
	langMgr.fillInstalledStringList(&languageList, true);
	languageList.sort();
	guiLangCombo = new QComboBox( false, ButtonGroup1, "guiLangCombo");
	guiLangCombo->insertStringList( languageList );
	selectedGUILang=prefsData->guiLanguage;
	guiLangCombo->setCurrentText(langMgr.getLangFromAbbrev(selectedGUILang));
	guiLangLabel = new QLabel(guiLangCombo, tr("&Language:"), ButtonGroup1, "guiLangLabel");
	ButtonGroup1Layout->addWidget( guiLangLabel, 0, 0 );
	ButtonGroup1Layout->addWidget( guiLangCombo, 0, 1 );

	GUICombo = new QComboBox( true, ButtonGroup1, "GUICombo" );
	QStringList STtest;
	STtest = QStyleFactory::keys();
	for (uint stt = 0; stt < STtest.count(); ++stt)
		GUICombo->insertItem(STtest[stt]);
	GUICombo->setEditable(false);
	GUICombo->setCurrentText(prefsData->GUI);
	TextGstil = new QLabel(GUICombo, tr("&Theme:"), ButtonGroup1, "dd");
	ButtonGroup1Layout->addWidget( TextGstil, 1, 0 );
	ButtonGroup1Layout->addWidget( GUICombo, 1, 1 );

	GFsize = new QSpinBox(8, 22, 1, ButtonGroup1, "gfs" );
	GFsize->setSuffix( tr( " pt" ) );
	GFsize->setValue( prefsData->AppFontSize );
	TextGstil2 = new QLabel(GFsize, tr("&Font Size:"), ButtonGroup1, "dd");
	ButtonGroup1Layout->addWidget( TextGstil2, 2, 0 );
	ButtonGroup1Layout->addWidget( GFsize, 2, 1, Qt::AlignLeft );
	SpinBox3 = new QSpinBox( 0, 1000, 10, ButtonGroup1, "SpinBox3" );
	SpinBox3->setValue( prefsData->Wheelval );
	TextLabel1_2 = new QLabel( SpinBox3, tr( "&Wheel Jump:" ), ButtonGroup1, "TextLabel1_2" );
	ButtonGroup1Layout->addWidget( TextLabel1_2, 3, 0 );
	ButtonGroup1Layout->addWidget( SpinBox3, 3, 1, Qt::AlignLeft );
	Recen = new QSpinBox( 1, 30, 1, ButtonGroup1, "Recen" );
	Recen->setValue( prefsData->RecentDCount );
	TextLabel4c = new QLabel( Recen, tr( "&Recent Documents:" ), ButtonGroup1, "TextLabel4c" );
	ButtonGroup1Layout->addWidget( TextLabel4c, 4, 0);
	ButtonGroup1Layout->addWidget( Recen, 4, 1, Qt::AlignLeft );
	tabLayout->addWidget( ButtonGroup1, 0, 0 );

	GroupBox200 = new QGroupBox( tr( "Paths" ), tab, "GroupBox200" );
	GroupBox200->setColumnLayout(0, Qt::Horizontal );
	GroupBox200->layout()->setSpacing( 0 );
	GroupBox200->layout()->setMargin( 0 );
	GroupBox200Layout = new QGridLayout( GroupBox200->layout() );
	GroupBox200Layout->setAlignment( Qt::AlignTop );
	GroupBox200Layout->setSpacing( 0 );
	GroupBox200Layout->setMargin( 5 );

	Docs = new QLineEdit( GroupBox200, "Datei" );
	Docs->setMinimumSize( QSize( 268, 22 ) );
	Docs->setText(prefsData->DocDir);
	PfadText = new QLabel( Docs, tr("&Documents:"), GroupBox200, "Pfadtext" );
	FileC = new QToolButton( GroupBox200, "FileC" );
	FileC->setMinimumSize( QSize( 88, 24 ) );
	FileC->setText( tr( "&Change..." ) );
	GroupBox200Layout->addWidget( PfadText, 0, 0 );
	GroupBox200Layout->addWidget( Docs, 0, 1 );
	GroupBox200Layout->addWidget( FileC, 0, 2 );

	ProPfad = new QLineEdit( GroupBox200, "Datei1" );
	ProPfad->setMinimumSize( QSize( 268, 22 ) );
	ProPfad->setText(prefsData->ProfileDir);
	if (ScApp->HaveDoc)
		ProPfad->setEnabled(false);
	PfadText2 = new QLabel( ProPfad, tr("&ICC Profiles:"), GroupBox200, "Pfadtext1" );
	FileC2 = new QToolButton( GroupBox200, "FileC1" );
	FileC2->setMinimumSize( QSize( 88, 24 ) );
	FileC2->setText( tr( "C&hange..." ) );
	if (ScApp->HaveDoc)
		FileC2->setEnabled(false);
	GroupBox200Layout->addWidget( PfadText2, 1, 0 );
	GroupBox200Layout->addWidget( ProPfad, 1, 1 );
	GroupBox200Layout->addWidget( FileC2, 1, 2 );

	ScriptPfad = new QLineEdit( GroupBox200, "Datei1" );
	ScriptPfad->setMinimumSize( QSize( 268, 22 ) );
	ScriptPfad->setText(prefsData->ScriptDir);
	PfadText3 = new QLabel( ScriptPfad, tr("&Scripts:"), GroupBox200, "Pfadtext1" );
	FileC3 = new QToolButton( GroupBox200, "FileC3" );
	FileC3->setMinimumSize( QSize( 88, 24 ) );
	FileC3->setText( tr( "Ch&ange..." ) );
	GroupBox200Layout->addWidget( PfadText3, 2, 0 );
	GroupBox200Layout->addWidget( ScriptPfad, 2, 1 );
	GroupBox200Layout->addWidget( FileC3, 2, 2 );

	DocumentTemplateDir = new QLineEdit( GroupBox200, "Datei1" );
	DocumentTemplateDir->setMinimumSize( QSize( 268, 22 ) );
	DocumentTemplateDir->setText(prefsData->documentTemplatesDir);
	PfadText4 = new QLabel( DocumentTemplateDir, tr("Document T&emplates:"), GroupBox200, "Pfadtext1" );
	FileC4 = new QToolButton( GroupBox200, "FileC4" );
	FileC4->setMinimumSize( QSize( 88, 24 ) );
	FileC4->setText( tr( "Cha&nge..." ) );
	GroupBox200Layout->addWidget( PfadText4, 3, 0 );
	GroupBox200Layout->addWidget( DocumentTemplateDir, 3, 1 );
	GroupBox200Layout->addWidget( FileC4, 3, 2 );
	tabLayout->addWidget( GroupBox200, 1, 0 );
	addItem( tr("General"), loadIcon("AppIcon.png"), tab);

	tab_7 = new QWidget( prefsWidgets, "tab_7" );
	tabLayout_7 = new QHBoxLayout( tab_7, 0, 5, "tabLayout_7");
	Layout21 = new QVBoxLayout( 0, 0, 6, "Layout21");
	dsLayout4p = new QHBoxLayout;
	dsLayout4p->setSpacing( 5 );
	dsLayout4p->setMargin( 0 );
	dsLayout4p->setAlignment( Qt::AlignLeft );
	docLayout = new PageLayouts(tab_7, prefsData->pageSets);
	docLayout->selectItem(prefsData->FacingPages);
	docLayout->firstPage->setCurrentItem(prefsData->pageSets[prefsData->FacingPages].FirstPage);
	dsLayout4p->addWidget( docLayout );
	dsLayout4pv = new QVBoxLayout;
	dsLayout4pv->setSpacing( 5 );
	dsLayout4pv->setMargin( 0 );

	GroupSize = new QButtonGroup( tr( "Page Size" ), tab_7, "GroupSize" );
	GroupSize->setColumnLayout(0, Qt::Vertical );
	GroupSize->layout()->setSpacing( 6 );
	GroupSize->layout()->setMargin( 10 );
	GroupSizeLayout = new QVBoxLayout( GroupSize->layout() );
	GroupSizeLayout->setAlignment( Qt::AlignTop );
	Layout6 = new QGridLayout( 0, 1, 1, 0, 6, "Layout6");

	GZComboF = new QComboBox( true, GroupSize, "GZComboF" );
	/*
	const QString ar_size[] =
	    {"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3",
	     "B4", "B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", tr("Executive"),
	     tr("Folio"), tr("Ledger"), tr("Legal"), tr("Letter"), tr("Tabloid")
	    };
	size_t ar_s = sizeof(ar_size) / sizeof(*ar_size);
	for (uint s = 0; s < ar_s; ++s)
		GZComboF->insertItem(ar_size[s]);
	*/

	PageSize *ps=new PageSize(prefsData->pageSize);
	GZComboF->insertStringList(ps->getTrPageSizeList());
	GZComboF->insertItem( tr( "Custom" ) );
	GZComboF->setEditable(false);

	QStringList pageSizes=ps->getPageSizeList();
	int sizeIndex=pageSizes.findIndex(ps->getPageText());
	if (sizeIndex!=-1)
		GZComboF->setCurrentItem(sizeIndex);
	else
		GZComboF->setCurrentItem(GZComboF->count()-1);
	GZText1 = new QLabel( GZComboF, tr( "&Size:" ), GroupSize, "GZText1" );
	Layout6->addWidget( GZText1, 0, 0 );
	Layout6->addWidget( GZComboF, 0, 1 );

	GZComboO = new QComboBox( true, GroupSize, "GZComboO" );
	GZComboO->insertItem( tr( "Portrait" ) );
	GZComboO->insertItem( tr( "Landscape" ) );
	GZComboO->setEditable(false);
	GZComboO->setCurrentItem(prefsData->pageOrientation);
	GZText2 = new QLabel( GZComboO, tr( "Orie&ntation:" ), GroupSize, "GZText2" );
	Layout6->addWidget( GZText2, 1, 0 );
	Layout6->addWidget( GZComboO, 1, 1 );
	UnitCombo = new QComboBox( true, GroupSize, "UnitCombo" );
	UnitCombo->insertStringList(unitGetTextUnitList());
	UnitCombo->setEditable(false);
	UnitCombo->setCurrentItem(prefsData->docUnitIndex);
	unitComboText = new QLabel( UnitCombo, tr( "Units:" ), GroupSize, "unitComboText" );
	Layout6->addWidget( unitComboText, 2, 0 );
	Layout6->addWidget( UnitCombo, 2, 1 );

	GroupSizeLayout->addLayout( Layout6 );

	Layout5_2 = new QHBoxLayout( 0, 0, 6, "Layout5_2");

	pageWidth = new MSpinBox( 1, 100000, GroupSize, decimals );
	pageWidth->setEnabled( false );
	pageWidth->setMinimumSize( QSize( 70, 20 ) );
	pageWidth->setValue(prefsData->PageWidth * unitRatio);
	GZText3 = new QLabel( pageWidth, tr( "&Width:" ), GroupSize, "GZText3" );
	Layout5_2->addWidget( GZText3 );
	Layout5_2->addWidget( pageWidth );

	pageHeight = new MSpinBox( 1, 100000, GroupSize, decimals );
	pageHeight->setEnabled( false );
	pageHeight->setMinimumSize( QSize( 70, 20 ) );
	pageHeight->setValue(prefsData->PageHeight * unitRatio);
	GZText4 = new QLabel( pageHeight, tr( "&Height:" ), GroupSize, "GZText4" );
	Layout5_2->addWidget( GZText4 );
	Layout5_2->addWidget( pageHeight );

	GroupSizeLayout->addLayout( Layout5_2 );
	dsLayout4pv->addWidget( GroupSize );

	struct MarginStruct marg;
	marg.Top = prefsData->RandOben;
	marg.Bottom = prefsData->RandUnten;
	marg.Left = prefsData->RandLinks;
	marg.Right = prefsData->RandRechts;
	GroupRand = new MarginWidget(tab_7,  tr( "Margin Guides" ), &marg, decimals, unitRatio, unitGetSuffixFromIndex(docUnitIndex) );
	GroupRand->setPageHeight(prefsData->PageHeight);
	GroupRand->setPageWidth(prefsData->PageWidth);
	dsLayout4pv->addWidget( GroupRand );
	dsLayout4p->addLayout( dsLayout4pv );
	Layout21->addLayout( dsLayout4p );
	QBoxLayout *asurLayout = new QHBoxLayout( 0, 0, 6, "asurLayout");

	GroupAS = new QGroupBox( tr( "Autosave" ), tab_7, "GroupAS" );
	GroupAS->setColumnLayout(0, Qt::Vertical );
	GroupAS->layout()->setSpacing( 5 );
	GroupAS->layout()->setMargin( 10 );
	GroupASLayout = new QGridLayout( GroupAS->layout() );
	GroupASLayout->setAlignment( Qt::AlignTop );
	ASon = new QCheckBox( tr( "&Enabled" ), GroupAS, "Enable Autosave" );
	ASon->setChecked(prefsData->AutoSave);
	GroupASLayout->addMultiCellWidget( ASon, 0, 0, 0, 1 );
	ASTime = new QSpinBox( GroupAS, "Time" );
	ASTime->setMaxValue( 60 );
	ASTime->setMinValue( 1 );
	ASTime->setSuffix( " " + tr("min") );
	ASTime->setValue(prefsData->AutoSaveTime / 1000 / 60);
	ASText = new QLabel( ASTime, tr( "&Interval:" ), GroupAS, "ASText" );
	GroupASLayout->addWidget( ASText, 1, 0 );
	GroupASLayout->addWidget( ASTime, 1, 1 );
	asurLayout->addWidget(GroupAS);

	urGroup = new QGroupBox( tr("Undo/Redo"), tab_7, "urGroup");
	urGroup->setColumnLayout(0, Qt::Vertical);
	urGroup->layout()->setSpacing(5);
	urGroup->layout()->setMargin(10);
	QGridLayout *urGroupLayout = new QGridLayout(urGroup->layout());
	urGroupLayout->setAlignment(Qt::AlignTop);
	urSpinBox = new QSpinBox(urGroup, "urSpinBox");
	urSpinBox->setMinValue(0);
	urSpinBox->setMaxValue(1000);
	int urSBValue = UndoManager::instance()->getHistoryLength();
	if (urSBValue == -1)
		urSpinBox->setEnabled(false);
	else
		urSpinBox->setValue(urSBValue);
	urLabel = new QLabel(urSpinBox, tr("Action history length"), urGroup, "urLabel");
	urGroupLayout->addWidget(urLabel, 0, 0);
	urGroupLayout->addWidget(urSpinBox, 0, 1);
	asurLayout->addWidget(urGroup);
	Layout21->addLayout(asurLayout);

	tabLayout_7->addLayout( Layout21 );
	addItem( tr("Document"), loadIcon("page.png"), tab_7);

	tabGuides = new TabGuides(prefsWidgets, &prefsData->guidesSettings, &prefsData->typographicSettings, docUnitIndex);
	addItem( tr("Guides"), loadIcon("guides.png"), tabGuides);
	tabTypo = new TabTypograpy(  prefsWidgets, &prefsData->typographicSettings);
	addItem( tr("Typography"), loadIcon("font.png"), tabTypo);
	tabTools = new TabTools(  prefsWidgets, &prefsData->toolSettings, docUnitIndex, 0);
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets, &ap->LangTransl);
	tabHyphenator->verbose->setChecked(!prefsData->Automatic);
	tabHyphenator->input->setChecked(prefsData->AutoCheck);
	tabHyphenator->language->setCurrentText(ap->LangTransl[prefsData->Language]);
	tabHyphenator->wordLen->setValue(prefsData->MinWordLen);
	tabHyphenator->maxCount->setValue(prefsData->HyCount);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);

	tabFonts = new FontPrefs(  prefsWidgets, prefsData->AvailFonts, false, ap->PrefsPfad, 0);
	addItem( tr("Fonts"), loadIcon("font.png"), tabFonts);

	tabDocChecker = new TabCheckDoc(  prefsWidgets, prefsData->checkerProfiles, prefsData->curCheckProfile);
	addItem( tr("Preflight Verifier"), loadIcon("checkdoc.png"), tabDocChecker);

	if (CMSavail)
	{
		tabColorManagement = new CMSPrefs(prefsWidgets, &prefsData->DCMSset, &ap->InputProfiles, &ap->InputProfilesCMYK, &ap->PrinterProfiles, &ap->MonitorProfiles);
		addItem( tr("Color Management"), loadIcon("blend.png"), tabColorManagement);
	}
	QMap<QString,QFont> DocFonts;
	DocFonts.clear();
	tabPDF = new TabPDFOptions( prefsWidgets,
								&prefsData->PDF_Options,
								prefsData->AvailFonts,
								&ap->PDFXProfiles,
								DocFonts,
								prefsData->PDF_Options.PresentVals,
								docUnitIndex,
								prefsData->PageHeight,
								prefsData->PageWidth,
								0 );
	addItem( tr("PDF Export"), loadIcon("acroread.png"), tabPDF);

	tabDefaultItemAttributes = new DocumentItemAttributes( prefsWidgets);
	defaultAttributesList=tabDefaultItemAttributes->getDocAttributesNames();
	tabDefaultItemAttributes->setup(&prefsData->defaultItemAttributes);
	addItem( tr("Document Item Attributes"), loadIcon("docattributes.png"), tabDefaultItemAttributes);

	tabDefaultTOCIndexPrefs = new TOCIndexPrefs( prefsWidgets );
	tabDefaultTOCIndexPrefs->setupItemAttrs( defaultAttributesList );
	tabDefaultTOCIndexPrefs->setup(&prefsData->defaultToCSetups, NULL);
	connect( prefsWidgets, SIGNAL(aboutToShow(QWidget *)), this, SLOT(setTOCIndexData(QWidget *)));
	addItem( tr("Table of Contents and Indexes"), loadIcon("tabtocindex.png"), tabDefaultTOCIndexPrefs);


	tabKeys = new KeyManager(prefsWidgets, prefsData->KeyActions);
	addItem( tr("Keyboard Shortcuts"), loadIcon("key_bindings.png"), tabKeys);

	tab_5 = new QWidget( prefsWidgets, "tab_5" );
	tabLayout_5 = new QGridLayout( tab_5 );
	tabLayout_5->setSpacing( 5 );
	tabLayout_5->setMargin( 0 );
	saving = new QGroupBox( tr( "Other Options" ), tab_5, "Saving" );
	saving->setColumnLayout(0, Qt::Vertical );
	saving->layout()->setSpacing( 0 );
	saving->layout()->setMargin( 0 );
	savingLayout = new QHBoxLayout( saving->layout() );
	savingLayout->setAlignment( Qt::AlignTop );
	savingLayout->setSpacing( 6 );
	savingLayout->setMargin( 25 );
	SaveAtQuit = new QCheckBox( tr( "Sa&ve Contents on Changes" ), saving, "SaveAtQuit" );
	SaveAtQuit->setChecked(prefsData->SaveAtQ);
	savingLayout->addWidget( SaveAtQuit );
	tabLayout_5->addWidget( saving, 1, 0 );
	preview = new QGroupBox( tr( "Preview" ), tab_5, "Preview" );
	preview->setColumnLayout(0, Qt::Vertical );
	preview->layout()->setSpacing( 0 );
	preview->layout()->setMargin( 0 );
	previewLayout = new QHBoxLayout( preview->layout() );
	previewLayout->setAlignment( Qt::AlignTop );
	previewLayout->setSpacing( 10 );
	previewLayout->setMargin( 24 );
	PreviewSize = new QComboBox( true, preview, "PreviewSize" );
	PreviewSize->insertItem( tr( "Small" ) );
	PreviewSize->insertItem( tr( "Medium" ) );
	PreviewSize->insertItem( tr( "Large" ) );
	PreviewSize->setEditable(false);
	int sci = prefsData->PSize == 40 ? 0 : prefsData->PSize == 60 ? 1 : 2;
	PreviewSize->setCurrentItem(sci);
	textLabelP = new QLabel( PreviewSize, tr( "&Size:" ), preview, "TextLabel1_3" );
	previewLayout->addWidget( textLabelP );
	previewLayout->addWidget( PreviewSize );
	tabLayout_5->addWidget( preview, 0, 0 );
	addItem(  tr("Scrapbook"), loadIcon("scrap.png"), tab_5);

	tabView = new QWidget( prefsWidgets, "tabView" );
	tabViewLayout = new QVBoxLayout( tabView, 0, 5, "tabViewLayout");
	tabViewLayout->setAlignment( Qt::AlignTop );
	pageBackground = new QButtonGroup( tabView, "pageBackground" );
	pageBackground->setTitle( tr( "Page Display" ) );
	pageBackground->setColumnLayout(0, Qt::Vertical );
	pageBackground->layout()->setSpacing( 5 );
	pageBackground->layout()->setMargin( 10 );
	pageBackgroundLayout = new QGridLayout( pageBackground->layout() );
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
	pm5.fill(prefsData->DpapColor);
	colorPaper = prefsData->DpapColor;
	backColor->setPixmap(pm5);
	backColor->setText( QString::null );
	layout10->addWidget( backColor );
	QSpacerItem* spacer3 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout10->addItem( spacer3 );
	pageBackgroundLayout->addLayout( layout10, 0 ,0 );
	checkUnprintable = new QCheckBox( pageBackground, "checkUnprintable" );
	checkUnprintable->setText( tr( "Display &Unprintable Area in Margin Color" ) );
	checkUnprintable->setAccel( QKeySequence( tr( "Alt+U" ) ) );
	checkUnprintable->setChecked( prefsData->marginColored );
	pageBackgroundLayout->addWidget( checkUnprintable, 0, 1 );
	checkPictures = new QCheckBox( pageBackground, "checkPictures" );
	checkPictures->setText( tr( "Show Pictures" ) );
	checkPictures->setChecked(prefsData->guidesSettings.showPic);
	pageBackgroundLayout->addWidget( checkPictures, 1 , 0 );
	checkLink = new QCheckBox( pageBackground, "checkLink" );
	checkLink->setText( tr( "Show Text Chains" ) );
	checkLink->setChecked(prefsData->guidesSettings.linkShown);
	pageBackgroundLayout->addWidget( checkLink, 1, 1 );
	checkControl = new QCheckBox( pageBackground, "checkControl" );
	checkControl->setText( tr( "Show Text Control Characters" ) );
	checkControl->setChecked(prefsData->guidesSettings.showControls);
	pageBackgroundLayout->addWidget( checkControl, 2 , 0 );
	checkFrame = new QCheckBox( pageBackground, "checkFrame" );
	checkFrame->setText( tr( "Show Frames" ) );
	checkFrame->setChecked(prefsData->guidesSettings.framesShown);
	pageBackgroundLayout->addWidget( checkFrame, 2, 1 );
	checkRuler = new QCheckBox( pageBackground, "checkRuler" );
	checkRuler->setText( tr( "Rulers relative to Page" ) );
	checkRuler->setChecked(prefsData->guidesSettings.rulerMode);
	pageBackgroundLayout->addWidget( checkRuler, 3, 0 );
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
	Layout4s = new QGridLayout;
	Layout4s->setSpacing( 6 );
	Layout4s->setMargin( 0 );
	topScratch = new MSpinBox( groupScratch, 4 );
	topScratch->setDecimals( decimals );
	topScratch->setMaxValue(1000);
	topScratch->setValue(prefsData->ScratchTop * unitRatio);
	Layout4s->addWidget( topScratch, 0, 1 );
	TextLabel5s = new QLabel(topScratch, tr( "&Top:" ), groupScratch, "TextLabel5" );
	Layout4s->addWidget( TextLabel5s, 0, 0 );
	leftScratch = new MSpinBox( groupScratch, 4 );
	leftScratch->setDecimals( decimals );
	leftScratch->setMaxValue(1000);
	leftScratch->setValue(prefsData->ScratchLeft * unitRatio);
	Layout4s->addWidget( leftScratch, 0, 3 );
	Linkss = new QLabel(leftScratch, tr( "&Left:" ), groupScratch, "Links" );
	Layout4s->addWidget( Linkss, 0, 2 );
	bottomScratch = new MSpinBox( groupScratch, 4 );
	bottomScratch->setDecimals( decimals );
	bottomScratch->setMaxValue(1000);
	bottomScratch->setValue(prefsData->ScratchBottom * unitRatio);
	Layout4s->addWidget( bottomScratch, 1, 1 );
	TextLabel7s = new QLabel(bottomScratch, tr( "&Bottom:" ), groupScratch, "TextLabel7" );
	Layout4s->addWidget( TextLabel7s, 1, 0 );
	rightScratch = new MSpinBox( groupScratch, 4 );
	rightScratch->setDecimals( decimals );
	rightScratch->setMaxValue(1000);
	rightScratch->setValue(prefsData->ScratchRight * unitRatio);
	Layout4s->addWidget( rightScratch, 1, 3 );
	QString unitSuffix = unitGetSuffixFromIndex(docUnitIndex);
	topScratch->setSuffix(unitSuffix);
	bottomScratch->setSuffix(unitSuffix);
	leftScratch->setSuffix(unitSuffix);
	rightScratch->setSuffix(unitSuffix);
	Rechtss = new QLabel(rightScratch, tr( "&Right:" ), groupScratch, "Rechts" );
	Layout4s->addWidget( Rechtss, 1, 2 );
	groupScratchLayout->addLayout( Layout4s );
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
	gapHorizontal->setSuffix( unitSuffix );
	gapHorizontal->setDecimals( decimals );
	gapHorizontal->setMaxValue(1000);
	gapHorizontal->setValue(prefsData->pageSets[prefsData->FacingPages].GapHorizontal * unitRatio);
	layout4sg->addWidget( gapHorizontal, 0, 1 );
	TextLabel5sg = new QLabel(gapHorizontal, tr( "Horizontal:" ), groupGap, "TextLabel5" );
	layout4sg->addWidget( TextLabel5sg, 0, 0 );
	gapVertical = new MSpinBox( groupGap, 4 );
	gapVertical->setSuffix( unitSuffix );
	gapVertical->setDecimals( decimals );
	gapVertical->setMaxValue(1000);
	gapVertical->setValue(prefsData->pageSets[prefsData->FacingPages].GapVertical * unitRatio);
	layout4sg->addWidget( gapVertical, 0, 3 );
	TextLabel7sg = new QLabel(gapVertical, tr( "Vertical:" ), groupGap, "Links" );
	layout4sg->addWidget( TextLabel7sg, 0, 2 );
	groupGapLayout->addLayout( layout4sg );
	tabViewLayout->addWidget( groupGap );

	CaliGroup = new QGroupBox( tr( "&Adjust Display Size" ), tabView, "CaliGroup" );
	CaliGroup->setColumnLayout(0, Qt::Vertical );
	CaliGroup->layout()->setSpacing( 6 );
	CaliGroup->layout()->setMargin( 11 );
	CaliGroupLayout = new QVBoxLayout( CaliGroup->layout() );
	CaliGroupLayout->setAlignment( Qt::AlignTop );
	CaliText = new QLabel( tr( "To adjust the display drag the ruler below with the slider." ), CaliGroup, "CaliText" );
	CaliGroupLayout->addWidget( CaliText );
	CaliRuler = new QLabel( CaliGroup, "CaliRuler" );
	CaliRuler->setMinimumSize( QSize( 20, 20 ) );
	drawRuler();
	CaliRuler->setFrameShape( QLabel::Box );
	CaliRuler->setFrameShadow( QLabel::Sunken );
	CaliRuler->setScaledContents( false );
	CaliGroupLayout->addWidget( CaliRuler );
	layout15ca = new QHBoxLayout( 0, 0, 6, "layout15");
	CaliSlider = new QSlider( CaliGroup, "CaliSlider" );
	CaliSlider->setMinValue( -100 );
	CaliSlider->setMaxValue( 100 );
	CaliSlider->setValue(static_cast<int>(100 * DisScale)-100);
	CaliSlider->setOrientation( QSlider::Horizontal );
	CaliSlider->setTickmarks( QSlider::Right );
	CaliSlider->setTickInterval( 10 );
	CaliText->setBuddy(CaliGroup);
	layout15ca->addWidget( CaliSlider );
	CaliAnz = new QLabel( CaliGroup, "CaliAnz" );
	CaliAnz->setText(QString::number(DisScale*100, 'f', 2)+" %");
	layout15ca->addWidget( CaliAnz );
	CaliGroupLayout->addLayout( layout15ca );
	tabViewLayout->addWidget( CaliGroup );
	addItem( tr("Display"), loadIcon("screen.png"), tabView);

	ExtTool = new QWidget( prefsWidgets, "ExtTool" );
	ExtToolLayout = new QVBoxLayout( ExtTool, 0, 5, "ExtToolLayout");
	ExtToolLayout->setAlignment( Qt::AlignTop );
	ghostscriptGroup = new QGroupBox( tr( "Postscript Interpreter" ), ExtTool, "ghostscriptGroup" );
	ghostscriptGroup->setColumnLayout(0, Qt::Vertical );
	ghostscriptGroup->layout()->setSpacing( 6 );
	ghostscriptGroup->layout()->setMargin( 10 );
	groupGhostScriptLayout = new QVBoxLayout( ghostscriptGroup->layout() );
	groupGhostScriptLayout->setAlignment( Qt::AlignTop );
	ghostscriptLayout = new QHBoxLayout( 0, 0, 6, "ghostscriptLayout");
	ghostscriptLineEdit = new QLineEdit( ghostscriptGroup, "ghostscriptLineEdit" );
	ghostscriptLineEdit->setText(prefsManager->ghostscriptExecutable());
	ghostscriptLabel = new QLabel( ghostscriptLineEdit, tr( "&Name of Executable:" ), ghostscriptGroup, "ghostscriptLabel" );
	ghostscriptChangeButton = new QToolButton( ghostscriptGroup, "ghostscriptChangeButton" );
	ghostscriptChangeButton->setMinimumSize( QSize( 88, 24 ) );
	ghostscriptChangeButton->setText( tr( "&Change..." ) );
	ghostscriptLayout->addWidget( ghostscriptLabel );
	ghostscriptLayout->addWidget( ghostscriptLineEdit );
	ghostscriptLayout->addWidget( ghostscriptChangeButton );
	groupGhostScriptLayout->addLayout( ghostscriptLayout );
	GSantiText = new QCheckBox( tr( "Antialias &Text" ), ghostscriptGroup, "GSantiText" );
	GSantiText->setChecked(prefsData->gs_AntiAliasText);
	groupGhostScriptLayout->addWidget( GSantiText );
	GSantiGraph = new QCheckBox( tr( "Antialias &Graphics" ), ghostscriptGroup, "GSantiGraph" );
	GSantiGraph->setChecked(prefsData->gs_AntiAliasGraphics);
	groupGhostScriptLayout->addWidget( GSantiGraph );
	GSlayout2 = new QHBoxLayout( 0, 0, 6, "ghostscriptLayout");
	GSResolution = new QSpinBox( ghostscriptGroup, "Time" );
	GSResolution->setMaxValue( 2400 );
	GSResolution->setMinValue( 10 );
	GSResolution->setSuffix( " " + tr("dpi") );
	GSResolution->setValue(prefsData->gs_Resolution);
	GSResText = new QLabel( GSResolution, tr( "Resolution:" ), ghostscriptGroup, "GSResText" );
	GSlayout2->addWidget( GSResText );
	GSlayout2->addWidget( GSResolution );
	groupGhostScriptLayout->addLayout( GSlayout2 );
	ExtToolLayout->addWidget( ghostscriptGroup );
	groupImageEditor = new QGroupBox( tr( "Image Processing Tool" ), ExtTool, "groupImageEditor" );
	groupImageEditor->setColumnLayout(0, Qt::Vertical );
	groupImageEditor->layout()->setSpacing( 6 );
	groupImageEditor->layout()->setMargin( 10 );
	groupImageEditorLayout = new QHBoxLayout( groupImageEditor->layout() );
	groupImageEditorLayout->setAlignment( Qt::AlignTop );
	imageEditorLineEdit = new QLineEdit( groupImageEditor, "imageEditorLineEdit" );
	imageEditorLineEdit->setText(prefsManager->imageEditorExecutable());
	imageEditorLabel = new QLabel( imageEditorLineEdit, tr( "Name of &Executable:" ), groupImageEditor, "imageEditorLabel" );
	imageEditorChangeButton = new QToolButton( groupImageEditor, "imageEditorChangeButton" );
	imageEditorChangeButton->setMinimumSize( QSize( 88, 24 ) );
	imageEditorChangeButton->setText( tr( "&Change..." ) );
	groupImageEditorLayout->addWidget( imageEditorLabel );
	groupImageEditorLayout->addWidget( imageEditorLineEdit );
	groupImageEditorLayout->addWidget( imageEditorChangeButton );
	ExtToolLayout->addWidget( groupImageEditor );
	QSpacerItem* spacer_gs = new QSpacerItem( 0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	ExtToolLayout->addItem( spacer_gs );
	addItem(  tr("External Tools"), loadIcon("misc.png"), ExtTool);

	Misc = new QWidget( prefsWidgets, "Misc" );
	MiscLayout = new QVBoxLayout( Misc, 0, 5, "MiscLayout");
	MiscLayout->setAlignment( Qt::AlignTop );
	groupPrint = new QGroupBox( tr( "Printing" ), Misc, "groupPrint" );
	groupPrint->setColumnLayout(0, Qt::Vertical );
	groupPrint->layout()->setSpacing( 10 );
	groupPrint->layout()->setMargin( 10 );
	groupPrintLayout = new QVBoxLayout( groupPrint->layout() );
	groupPrintLayout->setAlignment( Qt::AlignTop );
	ClipMarg = new QCheckBox( tr( "Clip to Page &Margins" ), groupPrint, "ClipMarg" );
	ClipMarg->setChecked(prefsData->ClipMargin);
	groupPrintLayout->addWidget( ClipMarg );
	DoGCR = new QCheckBox( tr( "Apply &Under Color Removal" ), groupPrint, "ClipMarg" );
	DoGCR->setChecked(prefsData->GCRMode);
	groupPrintLayout->addWidget( DoGCR );
	MiscLayout->addWidget( groupPrint );
	AskForSubs = new QCheckBox( tr( "Always ask before fonts are replaced when loading a document" ), Misc, "askforSubs" );
	AskForSubs->setChecked(prefsData->askBeforeSubstituite);
	MiscLayout->addWidget( AskForSubs );
	stylePreview = new QCheckBox( tr( "Preview of current Paragraph Style visible when editing Styles" ), Misc, "stylePreview" );
	stylePreview->setChecked(prefsData->haveStylePreview);
	MiscLayout->addWidget( stylePreview );
	startUpDialog = new QCheckBox( tr( "Show Startup Dialog" ), Misc, "startUpDialog" );
	startUpDialog->setChecked(prefsData->showStartupDialog);
	MiscLayout->addWidget( startUpDialog );
	// lorem ipsum
	groupLI = new QGroupBox(tr("Lorem Ipsum"), Misc, "groupLI");
	groupLI->setColumnLayout(0, Qt::Vertical);
	groupLI->layout()->setSpacing(10);
	groupLI->layout()->setMargin(10);
	groupLILayout = new QVBoxLayout(groupLI->layout());
	groupLILayout->setAlignment(Qt::AlignTop);
	useStandardLI = new QCheckBox(tr("Always use standard Lorem Ipsum"), groupLI, "useStandardLI");
	useStandardLI->setChecked(prefsData->useStandardLI);
	groupLILayout->addWidget(useStandardLI);
	paraLabelLI = new QLabel(tr("Count of the Paragraphs:"), groupLI, "paraLabelLI");
	paragraphsLI = new QSpinBox(groupLI, "paragraphsLI");
	paragraphsLI->setMinValue(1);
	paragraphsLI->setValue(prefsData->paragraphsLI);
	QHBoxLayout *liLayout = new QHBoxLayout();
	liLayout->addWidget(paraLabelLI);
	liLayout->addWidget(paragraphsLI);
	groupLILayout->addLayout(liLayout);
	MiscLayout->addWidget(groupLI);
	// spacer filling
	QSpacerItem* spacer_3m = new QSpacerItem( 0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	MiscLayout->addItem( spacer_3m );
	addItem(  tr("Miscellaneous"), loadIcon("misc.png"), Misc);

	// plugin manager. pv.
	pluginManagerWidget = new QWidget(prefsWidgets, "pluginManagerWidget");
	pluginMainLayout = new QVBoxLayout( pluginManagerWidget, 0, 5, "pluginMainLayout");
	pluginMainLayout->setAlignment( Qt::AlignTop );
	plugGroupBox = new QGroupBox( tr("Plugin Manager"), pluginManagerWidget, "plugGroupBox");
	plugGroupBox->setColumnLayout(0, Qt::Vertical);
	plugGroupBox->layout()->setSpacing(6);
	plugGroupBox->layout()->setMargin(11);
	plugGroupBoxLayout = new QGridLayout( plugGroupBox->layout() );
	plugGroupBoxLayout->setAlignment(Qt::AlignTop);
	plugLayout1 = new QVBoxLayout( 0, 0, 6, "plugLayout1");
	pluginsList = new QListView(plugGroupBox, "pluginsList");
	pluginsList->setAllColumnsShowFocus(true);
	pluginsList->setShowSortIndicator(true);
	pluginsList->addColumn( tr("Plugin"));
	pluginsList->setColumnWidthMode(0, QListView::Maximum);
	pluginsList->addColumn( tr("How to run"));
	pluginsList->setColumnWidthMode(1, QListView::Maximum);
	pluginsList->addColumn( tr("Type"));
	pluginsList->setColumnWidthMode(2, QListView::Maximum);
	pluginsList->addColumn( tr("Load it?"));
	pluginsList->setColumnWidthMode(3, QListView::Maximum);
	pluginsList->addColumn( tr("Plugin ID"));
	pluginsList->setColumnWidthMode(4, QListView::Maximum);
	pluginsList->addColumn( tr("File"));
	pluginsList->setColumnWidthMode(5, QListView::Maximum);
	for (QMap<int,PluginManager::PluginData>::Iterator it = ap->pluginManager->pluginMap.begin(); it != ap->pluginManager->pluginMap.end(); ++it)
	{
		QListViewItem *plugItem = new QListViewItem(pluginsList);
		plugItem->setText(0, (*it).name.replace('&', "").replace("...", "")); // name
		plugItem->setText(1, QString("%1 %2").arg((*it).actMenu).arg((*it).actMenuAfterName)); // menu path
		plugItem->setText(2, ap->pluginManager->getPluginType((*it).type)); // type
		// load at start?
		plugItem->setPixmap(3, (*it).loadPlugin ? loadIcon("ok.png") : loadIcon("DateiClos16.png"));
		plugItem->setText(3, (*it).loadPlugin ? tr("Yes") : tr("No"));
		plugItem->setText(4, QString("%1").arg(it.key())); // id for developers
		plugItem->setText(5, (*it).pluginFile); // file for developers
	}
	plugLayout1->addWidget(pluginsList);
	pluginWarning = new QLabel(plugGroupBox);
	pluginWarning->setText("<qt>" + tr("You need to restart the application to apply the changes.") + "</qt>");
	plugLayout1->addWidget(pluginWarning);
	plugGroupBoxLayout->addLayout(plugLayout1, 0, 0);
	pluginMainLayout->addWidget(plugGroupBox);
	addItem( tr("Plugins"), loadIcon("plugins.png"), pluginManagerWidget);

	setDS(prefsData->FacingPages);
	//tab order
	QWidget::setTabOrder( GFsize, SpinBox3 );
	QWidget::setTabOrder( SpinBox3, UnitCombo );
	QWidget::setTabOrder( UnitCombo, Recen );
	QWidget::setTabOrder( Recen, Docs );
	QWidget::setTabOrder( Docs, FileC );
	QWidget::setTabOrder( FileC, ProPfad );
	QWidget::setTabOrder( ProPfad, FileC2  );
	QWidget::setTabOrder( FileC2, ScriptPfad );
	QWidget::setTabOrder( ScriptPfad, FileC3 );
	QWidget::setTabOrder( FileC3, DocumentTemplateDir );
	QWidget::setTabOrder( DocumentTemplateDir, FileC4 );
	QWidget::setTabOrder( PreviewSize, SaveAtQuit );

	QToolTip::add( checkLink, tr("Turns the display of linked frames on or off"));
	QToolTip::add( checkControl, "<qt>" + tr("Display non-printing characters such as paragraph markers in text frames") + "</qt>");
	QToolTip::add( checkFrame, tr("Turns the display of frames on or off"));
	QToolTip::add( checkPictures, tr("Turns the display of pictures on or off"));
	QToolTip::add( guiLangCombo, tr( "Select your default language for Scribus to run with.\nLeave this blank to choose based on environment variables.\nYou can still override this by passing a command line option when starting Scribus" ) );
	QToolTip::add( GUICombo, tr( "Choose the default window decoration and looks.\nScribus inherits any available KDE or Qt themes" ) );
	QToolTip::add( GFsize, tr( "Default font size for the menus and windows" ) );
	QToolTip::add( UnitCombo, tr( "Default unit of measurement for document editing" ) );
	QToolTip::add( SpinBox3, tr( "Number of lines Scribus will scroll for each move of the mouse wheel" ) );
	QToolTip::add( Recen, tr( "Number of recently edited documents to show in the File menu" ) );
	QToolTip::add( Docs, tr( "Default documents directory" ) );
	QToolTip::add( ProPfad, tr( "Default ICC profiles directory. This cannot\nbe changed with a document open." ) );
	QToolTip::add( ScriptPfad, tr( "Default Scripter scripts directory" ) );
	QToolTip::add( DocumentTemplateDir, "<qt>"+tr("Additional directory for document templates")+"</qt>" );

	QToolTip::add( GZComboF, tr( "Default page size, either a standard size or a custom size" ) );
	QToolTip::add( GZComboO, tr( "Default orientation of document pages" ) );
	QToolTip::add( pageWidth, tr( "Width of document pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( pageHeight, tr( "Height of document pages, editable if you have chosen a custom page size" ) );
//	QToolTip::add( facingPages, tr( "Enable single or spread based layout" ) );
//	QToolTip::add( Linkszuerst, tr( "Make the first page the left page of a document" ) );
	QToolTip::add( ASon, tr( "When enabled, Scribus saves a backup copy of your file with the .bak extension\neach time the time period elapses" ) );
	QToolTip::add( ASTime, tr( "Time period between saving automatically" ) );

	QToolTip::add( urSpinBox, tr("Set the length of the action history in steps.\nIf set to 0 infinite amount of actions will be stored."));

	QToolTip::add( PreviewSize, tr( "Choose the size of the preview in the scrapbook palette" ) );
	QToolTip::add( SaveAtQuit, tr( "Save the scrapbook contents everytime after a change" ) );

	QToolTip::add( backColor, tr( "Color for paper" ) );
	QToolTip::add( checkUnprintable, tr( "Mask the area outside the margins in the margin color" ) );
	QToolTip::add( CaliText, tr( "Set the default zoom level" ) );
	QToolTip::add( CaliGroup, "<qt>" + tr( "Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size" ) + "</qt>" );

	QToolTip::add( ghostscriptLineEdit, tr( "Filesystem location for the Ghostscript interpreter" ) );
	QToolTip::add( GSantiText, tr( "Antialias text for EPS and PDF onscreen rendering" ) );
	QToolTip::add( GSantiGraph, tr( "Antialias graphics for EPS and PDF onscreen rendering" ) );
	QToolTip::add( imageEditorLineEdit, tr( "File system location for graphics editor. If you use gimp\n"
						"and your distro includes it, we recommend 'gimp-remote',\n"
						"as it allows you to edit the image in an already running\n"
						"instance of gimp." ) );
	QToolTip::add( ClipMarg, tr( "Do not show objects outside the margins on the printed page or exported file" ) );
	QToolTip::add( DoGCR, tr( "A way of switching off some of the gray shades which are composed\n"
		                          "of cyan, yellow and magenta and using black instead.\n"
		                          "UCR most affects parts of images which are neutral and/or dark tones\n"
		                          "which are close to the gray. Use of this may improve printing some images\n"
		                          "and some experimentation and testing is need on a case by case basis.\n"
		                          "UCR reduces the possibility of over saturation with CMY inks." ) );

	QToolTip::add( leftScratch, "<qt>" + tr( "Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	QToolTip::add( rightScratch, "<qt>" + tr( "Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	QToolTip::add( topScratch, "<qt>" + tr( "Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );
	QToolTip::add( bottomScratch, "<qt>" + tr( "Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page" ) + "</qt>" );


	// signals and slots connections
	connect( guiLangCombo, SIGNAL( activated( const QString & ) ), this, SLOT( setSelectedGUILang( const QString & ) ) );
	connect(backColor, SIGNAL(clicked()), this, SLOT(changePaperColor()));
	connect(UnitCombo, SIGNAL(activated(int)), this, SLOT(unitChange()));
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	connect(docLayout, SIGNAL( selectedLayout(int) ), this, SLOT( setDS(int) ) );
	connect(GZComboO, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(GZComboF, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	connect(FileC, SIGNAL(clicked()), this, SLOT(changeDocs()));
	connect(FileC2, SIGNAL(clicked()), this, SLOT(changeProfs()));
	connect(FileC3, SIGNAL(clicked()), this, SLOT(changeScripts()));
	connect(FileC4, SIGNAL(clicked()), this, SLOT(changeDocumentTemplates()));
	connect(ghostscriptChangeButton, SIGNAL(clicked()), this, SLOT(changeGhostscript()));
	connect(imageEditorChangeButton, SIGNAL(clicked()), this, SLOT(changeImageEditor()));
	connect(CaliSlider, SIGNAL(valueChanged(int)), this, SLOT(setDisScale()));
	connect(buttonOk, SIGNAL(clicked()), this, SLOT(setActionHistoryLength()));
	connect(pluginsList, SIGNAL(clicked(QListViewItem *, const QPoint &, int)),
			this, SLOT(changePluginLoad(QListViewItem *, const QPoint &, int)));
	if (CMSavail)
		connect(tabColorManagement, SIGNAL(cmsOn(bool )), this, SLOT(switchCMS(bool )));

	setSize(prefsData->pageSize);
	setOrien(prefsData->pageOrientation);
	pageWidth->setValue(prefsData->PageWidth * unitRatio);
	pageHeight->setValue(prefsData->PageHeight * unitRatio);
	pageWidth->setSuffix(unitSuffix);
	pageHeight->setSuffix(unitSuffix);

	//unitChange();
	resize( minimumSizeHint() );
	arrangeIcons();
	backToDefaults->hide();
	prefsSelection->setSelected(prefsSelection->firstItem(), true);
	itemSelected(prefsSelection->firstItem());
	clearWState( WState_Polished );
}

/*!
 \fn Preferences::~Preferences
 \author Franz Schmid
 \date
 \brief Destructor for Preferences dialog box
 \param None
 \retval None
 */
Preferences::~Preferences()
{
	// no need to delete child widgets, Qt does it all for us
}


/*!
 \fn void Preferences::ChangeDocs()
 \author Franz Schmid
 \date
 \brief Runs QFileDialog to get Preferences (General) Path to Documents directory
 \param None
 \retval None
 */
void Preferences::changeDocs()
{
	QString s = QFileDialog::getExistingDirectory(Docs->text(), this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
		Docs->setText(s);
}

/*!
 \fn void Preferences::ChangeProfs()
 \author Franz Schmid
 \date
 \brief Runs QFileDialog to get Preferences (General) Path to Colour Profiles directory
 \param None
 \retval None
 */
void Preferences::changeProfs()
{
	QString s = QFileDialog::getExistingDirectory(ProPfad->text(), this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
		ProPfad->setText(s);
}

/*!
 \fn void Preferences::ChangeScripts()
 \author Franz Schmid
 \date
 \brief Runs QFileDialog to get Preferences (General) Path to Scripts directory
 \param None
 \retval None
 */
void Preferences::changeScripts()
{
	QString s = QFileDialog::getExistingDirectory(ScriptPfad->text(), this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
		ScriptPfad->setText(s);
}

/*!
 \fn void Preferences::ChangeDocumentTemplates()
 \author Riku Leino
 \date
 \brief Runs QFileDialog to get Preferences (General) Path to Document Templates directory
 \param None
 \retval None
 */
void Preferences::changeDocumentTemplates()
{
	QString s = QFileDialog::getExistingDirectory(DocumentTemplateDir->text(), this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
		DocumentTemplateDir->setText(s);
}

/*!
 \fn void Preferences::ChangeDocumentTemplates()
 \author Craig Bradney
 \brief Runs QFileDialog to get Preferences Path to Ghostscript
 \param None
 \retval None
 */
void Preferences::changeGhostscript()
{
	QFileInfo fi(ghostscriptLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeGhostscript", tr("Locate Ghostscript"));
	if (!s.isEmpty())
		ghostscriptLineEdit->setText(s);
}

/*!
 \fn void Preferences::ChangeDocumentTemplates()
 \author Craig Bradney
 \brief Runs QFileDialog to get Preferences Path to Image Editor
 \param None
 \retval None
 */
void Preferences::changeImageEditor()
{
	QFileInfo fi(imageEditorLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeImageEditor", tr("Locate your image editor"));
	if (!s.isEmpty())
		imageEditorLineEdit->setText(s);
}

/*!
 \fn void Preferences::setDS()
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), switches default between Facing Pages and swaps text labels for margin guides
 \param None
 \retval None
 */
void Preferences::setDS(int layout)
{
	GroupRand->setFacingPages(!(layout == singlePage));
	choosenLayout = layout;
	docLayout->firstPage->setCurrentItem(prefsManager->appPrefs.pageSets[choosenLayout].FirstPage);
	gapHorizontal->setValue(prefsManager->appPrefs.pageSets[choosenLayout].GapHorizontal * unitRatio);
	gapVertical->setValue(prefsManager->appPrefs.pageSets[choosenLayout].GapVertical * unitRatio);
}

/*!
 \fn void Preferences::setPageWidth(int v)
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), sets Page width values
 \param v Width value
 \retval None
 */
void Preferences::setPageWidth(int)
{
	Pagebr = pageWidth->value() / unitRatio;
	GroupRand->setPageWidth(Pagebr);
}

/*!
 \fn void Preferences::setPageHeight(int v)
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), sets Page height values
 \param v Height value
 \retval None
 */
void Preferences::setPageHeight(int)
{
	Pageho = pageHeight->value() / unitRatio;
	GroupRand->setPageHeight(Pageho);
}

void Preferences::setPageSize()
{
	setOrien(GZComboO->currentItem());
}

/*!
 \fn void Preferences::setSize(QString gr)
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), sets Page size values. Connects signals for setting page dimensions.
 \param gr Standard page size value (eg A4)
 \retval None
 */
void Preferences::setSize(const QString & gr)
{
	Pagebr = pageWidth->value() / unitRatio;
	Pageho = pageHeight->value() / unitRatio;
	pageWidth->setEnabled(false);
	pageHeight->setEnabled(false);
	PageSize *ps2=new PageSize(gr);

	prefsPageSizeName=ps2->getPageName();
	if (gr == tr("Custom"))
	{
		pageWidth->setEnabled(true);
		pageHeight->setEnabled(true);
	}
	else
	{
		Pagebr = ps2->getPageWidth();
		Pageho = ps2->getPageHeight();
	}
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	pageWidth->setValue(Pagebr * unitRatio);
	pageHeight->setValue(Pageho * unitRatio);
	GroupRand->setPageHeight(Pageho);
	GroupRand->setPageWidth(Pagebr);
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	delete ps2;
}

/*!
 \fn void Preferences::setOrien(int ori)
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), sets Page orientation value and page dimensions
 \param ori Orientation value
 \retval None
 */
void Preferences::setOrien(int ori)
{
	double br;
	setSize(GZComboF->currentText());
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	if (ori == 0)
	{
		//if (GZComboF->currentItem() == 30)
		if (GZComboF->currentText() == tr("Custom"))
		{
			br = pageWidth->value();
			pageWidth->setValue(pageHeight->value());
			pageHeight->setValue(br);
		}
	}
	else
	{
		br = pageWidth->value();
		pageWidth->setValue(pageHeight->value());
		pageHeight->setValue(br);
	}
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
}

/*!
 \fn void Preferences::changePaperColor()
 \author Franz Schmid
 \date
 \brief Preferences ([dox?], [dox?]), Sets Paper color [dox?]
 \param None
 \retval None
 */
void Preferences::changePaperColor()
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

/*!
 \fn void Preferences::UnitChange()
 \author Franz Schmid
 \date
 \brief Preferences (General, Units). Sets scaling factors and units descriptions when default units are changed. Updates preference values
 \param None
 \retval None
 */
void Preferences::unitChange()
{
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	int decimals;
	double oldUnitRatio = unitRatio;
	double oldMin, oldMax, oldB, oldBM, oldH, oldHM, val;
	pageWidth->getValues(&oldB, &oldBM, &decimals, &val);
	oldB /= oldUnitRatio;
	oldBM /= oldUnitRatio;
	pageHeight->getValues(&oldH, &oldHM, &decimals, &val);
	oldH /= oldUnitRatio;
	oldHM /= oldUnitRatio;
	QString einh;
	docUnitIndex = UnitCombo->currentItem();
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
	decimals = unitGetDecimalsFromIndex(docUnitIndex);
	einh = unitGetSuffixFromIndex(docUnitIndex);

	pageWidth->setSuffix(einh);
	pageHeight->setSuffix(einh);
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
	tabPDF->BleedBottom->setSuffix(einh);
	tabPDF->BleedTop->setSuffix(einh);
	tabPDF->BleedRight->setSuffix(einh);
	tabPDF->BleedLeft->setSuffix(einh);
	pageWidth->setValues(oldB * unitRatio, oldBM * unitRatio, decimals, Pagebr * unitRatio);
	pageHeight->setValues(oldH * unitRatio, oldHM * unitRatio, decimals, Pageho * unitRatio);
	GroupRand->unitChange(unitRatio, decimals, einh);
	GroupRand->setPageHeight(Pageho);
	GroupRand->setPageWidth(Pagebr);
	int decimalsOld;
	double invUnitConversion = 1.0 / oldUnitRatio * unitRatio;

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
	tabPDF->BleedBottom->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabPDF->BleedBottom->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabPDF->BleedTop->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabPDF->BleedTop->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabPDF->BleedRight->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabPDF->BleedRight->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabPDF->BleedLeft->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	tabPDF->BleedLeft->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	tabPDF->unitRatio = unitRatio;
	drawRuler();
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
}

/*!
 \fn void Preferences::SetDisScale()
 \author Franz Schmid
 \date
 \brief Preferences (Display, Display Size). Sets Scale for ruler scale
 \param None
 \retval None
 */
void Preferences::setDisScale()
{
	DisScale = QMAX((100.0 + CaliSlider->value()) / 100.0, 0.01);
	drawRuler();
	CaliAnz->setText(QString::number(DisScale*100, 'f', 2)+" %");
}

/*!
 \fn void Preferences::DrawRuler()
 \author Franz Schmid
 \date
 \brief Preferences (Display, Display Size). Draws ruler depending on scaling factor
 \param None
 \retval None
 */
void Preferences::drawRuler()
{
	double xl, iter, iter2, maxi;
	switch (docUnitIndex)
	{
	case 0:
		iter = 10.0;
		iter2 = iter * 10.0;
		maxi = 200.0;
		break;
	case 1:
		iter = (10.0 / 25.4) * 72.0;
		iter2 = iter * 10.0;
		maxi = iter2;
		break;
	case 2:
		iter = 18.0;
		iter2 = 72.0;
		maxi = 2 * iter2;
		break;
	case 3:
		iter = 12.0;
		iter2 = 120.0;
		maxi = 240.0;
		break;
	case 4:
		iter = 12.0;
		iter2 = 120.0;
		maxi = 240.0;
		break;		
	default:
		iter = 10.0;
		iter2 = iter * 10.0;
		maxi = 200.0;
		break;
	}

	QPixmap pm(static_cast<int>(maxi*DisScale+30), 21);
	pm.fill();
	QPainter p;
	p.begin(&pm);
	p.drawLine(0, 19, width(), 19);
	p.setBrush(black);
	p.setPen(black);
	p.scale(DisScale, 1.0);
	for (xl = 0; xl < maxi; xl += iter)
		p.drawLine(static_cast<int>(xl), 13, static_cast<int>(xl), 19);
	for (xl = 0; xl < maxi+10; xl += iter2)
	{
		p.drawLine(static_cast<int>(xl), 6, static_cast<int>(xl), 19);
		p.save();
		p.scale(1.0 / DisScale, 1.0);
		switch (docUnitIndex)
		{
		case 2:
			p.drawText(static_cast<int>((xl+qRound(2/DisScale)) * DisScale), 12,
			           QString::number(xl / iter2));
			break;
		case 3:
			p.drawText(static_cast<int>((xl+qRound(2/DisScale)) * DisScale), 12,
			           QString::number(xl / iter));
			break;
		default:
			p.drawText(static_cast<int>((xl+qRound(2/DisScale)) * DisScale), 12,
			           QString::number(xl / iter * 10));
			break;
		}
		p.restore();
	}
	p.end();
	CaliRuler->setPixmap(pm);
}
/*
QString Preferences::getSelectedGUILang( )
{
	return langMgr.getAbbrevFromLang(guiLangCombo->currentText());
}
*/
void Preferences::setSelectedGUILang( const QString &newLang )
{
	selectedGUILang=langMgr.getAbbrevFromLang(newLang);
}

void Preferences::setActionHistoryLength()
{
  UndoManager::instance()->setHistoryLength(urSpinBox->value());
}

void Preferences::switchCMS(bool enable)
{
	tabPDF->enableCMS(enable);
}

/*! Set selected item(=plugin) un/loadable
\author Petr Vanek
*/
void Preferences::changePluginLoad(QListViewItem *item, const QPoint &, int column)
{
	if (column != 3)
		return;
	if (item->text(3) == tr("Yes"))
	{
		item->setPixmap(3, loadIcon("DateiClos16.png"));
		item->setText(3, tr("No"));
		ap->pluginManager->pluginMap[item->text(4).toInt()].loadPlugin = false;
	}
	else
	{
		item->setPixmap(3, loadIcon("ok.png"));
		item->setText(3, tr("Yes"));
		ap->pluginManager->pluginMap[item->text(4).toInt()].loadPlugin = true;
	}
}

void Preferences::setTOCIndexData(QWidget *widgetToShow)
{
	//Update the attributes list in TOC setup
	if (widgetToShow==tabDefaultTOCIndexPrefs)
		tabDefaultTOCIndexPrefs->setupItemAttrs( tabDefaultItemAttributes->getDocAttributesNames() );
}

void Preferences::updatePreferences()
{
	prefsManager->appPrefs.AppFontSize = GFsize->value();
	prefsManager->appPrefs.Wheelval = SpinBox3->value();
	prefsManager->appPrefs.RecentDCount = Recen->value();
	prefsManager->appPrefs.DocDir = Docs->text();
	prefsManager->appPrefs.ProfileDir = ProPfad->text();
	prefsManager->appPrefs.ScriptDir = ScriptPfad->text();
	prefsManager->appPrefs.documentTemplatesDir = DocumentTemplateDir->text();
	switch (PreviewSize->currentItem())
	{
		case 0:
			prefsManager->appPrefs.PSize = 40;
			break;
		case 1:
			prefsManager->appPrefs.PSize = 60;
			break;
		case 2:
			prefsManager->appPrefs.PSize = 80;
			break;
	}
	prefsManager->appPrefs.SaveAtQ = SaveAtQuit->isChecked();
	prefsManager->appPrefs.guiLanguage=selectedGUILang;
	prefsManager->appPrefs.GUI = GUICombo->currentText();
	tabTools->polyWidget->getValues(&prefsManager->appPrefs.toolSettings.polyC, &prefsManager->appPrefs.toolSettings.polyFd, &prefsManager->appPrefs.toolSettings.polyF, &prefsManager->appPrefs.toolSettings.polyS, &prefsManager->appPrefs.toolSettings.polyR);
	prefsManager->appPrefs.pageSize = prefsPageSizeName;
	prefsManager->appPrefs.pageOrientation = GZComboO->currentItem();
	prefsManager->appPrefs.PageWidth = Pagebr;
	prefsManager->appPrefs.PageHeight = Pageho;
	prefsManager->appPrefs.RandOben = GroupRand->RandT;
	prefsManager->appPrefs.RandUnten = GroupRand->RandB;
	prefsManager->appPrefs.RandLinks = GroupRand->RandL;
	prefsManager->appPrefs.RandRechts = GroupRand->RandR;
	double prefsUnitRatio = unitGetRatioFromIndex(UnitCombo->currentItem());
	prefsManager->appPrefs.FacingPages  = choosenLayout;
	prefsManager->appPrefs.pageSets[choosenLayout].FirstPage = docLayout->firstPage->currentItem();
	prefsManager->appPrefs.pageSets[choosenLayout].GapHorizontal = gapHorizontal->value() / prefsUnitRatio;
	prefsManager->appPrefs.pageSets[choosenLayout].GapVertical = gapVertical->value() / prefsUnitRatio;
	prefsManager->setImageEditorExecutable(imageEditorLineEdit->text());
	prefsManager->appPrefs.gs_AntiAliasGraphics = GSantiGraph->isChecked();
	prefsManager->appPrefs.gs_AntiAliasText = GSantiText->isChecked();
	prefsManager->setGhostscriptExecutable(ghostscriptLineEdit->text());
	prefsManager->appPrefs.gs_Resolution = GSResolution->value();
	prefsManager->appPrefs.ClipMargin = ClipMarg->isChecked();
	prefsManager->appPrefs.GCRMode = DoGCR->isChecked();
	prefsManager->appPrefs.guidesSettings.before = tabGuides->inBackground->isChecked();
	prefsManager->appPrefs.marginColored = checkUnprintable->isChecked();
	prefsManager->appPrefs.askBeforeSubstituite = AskForSubs->isChecked();
	prefsManager->appPrefs.haveStylePreview = stylePreview->isChecked();
	prefsManager->appPrefs.showStartupDialog = startUpDialog->isChecked();
	// lorem ipsum
	prefsManager->appPrefs.useStandardLI = useStandardLI->isChecked();
	prefsManager->appPrefs.paragraphsLI = paragraphsLI->value();
	prefsManager->appPrefs.DisScale = DisScale;
	
	prefsManager->appPrefs.docUnitIndex = UnitCombo->currentItem();
	prefsManager->appPrefs.ScratchBottom = bottomScratch->value() / prefsUnitRatio;
	prefsManager->appPrefs.ScratchLeft = leftScratch->value() / prefsUnitRatio;
	prefsManager->appPrefs.ScratchRight = rightScratch->value() / prefsUnitRatio;
	prefsManager->appPrefs.ScratchTop = topScratch->value() / prefsUnitRatio;
	prefsManager->appPrefs.DpapColor = colorPaper;
	prefsManager->appPrefs.toolSettings.defFont = tabTools->fontComboText->currentText();
	prefsManager->appPrefs.toolSettings.defSize = tabTools->sizeComboText->currentText().left(2).toInt() * 10;
	prefsManager->appPrefs.guidesSettings.marginsShown = tabGuides->marginBox->isChecked();
	prefsManager->appPrefs.guidesSettings.framesShown = checkFrame->isChecked();
	prefsManager->appPrefs.guidesSettings.rulerMode = checkRuler->isChecked();
	prefsManager->appPrefs.guidesSettings.gridShown = tabGuides->checkGrid->isChecked();
	prefsManager->appPrefs.guidesSettings.guidesShown = tabGuides->guideBox->isChecked();
	prefsManager->appPrefs.guidesSettings.baseShown = tabGuides->baselineBox->isChecked();
	prefsManager->appPrefs.guidesSettings.showPic = checkPictures->isChecked();
	prefsManager->appPrefs.guidesSettings.linkShown = checkLink->isChecked();
	prefsManager->appPrefs.guidesSettings.showControls = checkControl->isChecked();
	prefsManager->appPrefs.guidesSettings.grabRad = tabGuides->grabDistance->value();
	prefsManager->appPrefs.guidesSettings.guideRad = tabGuides->snapDistance->value() / prefsUnitRatio;
	prefsManager->appPrefs.guidesSettings.minorGrid = tabGuides->minorSpace->value() / prefsUnitRatio;
	prefsManager->appPrefs.guidesSettings.majorGrid = tabGuides->majorSpace->value() / prefsUnitRatio;
	prefsManager->appPrefs.guidesSettings.minorColor = tabGuides->colorMinorGrid;
	prefsManager->appPrefs.guidesSettings.majorColor = tabGuides->colorMajorGrid;
	prefsManager->appPrefs.guidesSettings.margColor = tabGuides->colorMargin;
	prefsManager->appPrefs.guidesSettings.guideColor = tabGuides->colorGuides;
	prefsManager->appPrefs.guidesSettings.baseColor = tabGuides->colorBaselineGrid;
	prefsManager->appPrefs.checkerProfiles = tabDocChecker->checkerProfile;
	prefsManager->appPrefs.curCheckProfile = tabDocChecker->curCheckProfile->currentText();
	prefsManager->appPrefs.typographicSettings.valueSuperScript = tabTypo->superDisplacement->value();
	prefsManager->appPrefs.typographicSettings.scalingSuperScript = tabTypo->superScaling->value();
	prefsManager->appPrefs.typographicSettings.valueSubScript = tabTypo->subDisplacement->value();
	prefsManager->appPrefs.typographicSettings.scalingSubScript = tabTypo->subScaling->value();
	prefsManager->appPrefs.typographicSettings.valueSmallCaps = tabTypo->capsScaling->value();
	prefsManager->appPrefs.typographicSettings.autoLineSpacing = tabTypo->autoLine->value();
	prefsManager->appPrefs.typographicSettings.valueBaseGrid = tabGuides->baseGrid->value() / prefsUnitRatio;
	prefsManager->appPrefs.typographicSettings.offsetBaseGrid = tabGuides->baseOffset->value() / prefsUnitRatio;
	prefsManager->appPrefs.typographicSettings.valueUnderlinePos = qRound(tabTypo->underlinePos->value() * 10);
	prefsManager->appPrefs.typographicSettings.valueUnderlineWidth = qRound(tabTypo->underlineWidth->value() * 10);
	prefsManager->appPrefs.typographicSettings.valueStrikeThruPos = qRound(tabTypo->strikethruPos->value() * 10);
	prefsManager->appPrefs.typographicSettings.valueStrikeThruWidth = qRound(tabTypo->strikethruWidth->value() * 10);
	prefsManager->appPrefs.toolSettings.dPen = tabTools->colorComboLineShape->currentText();
	if (prefsManager->appPrefs.toolSettings.dPen == ScApp->noneString)
		prefsManager->appPrefs.toolSettings.dPen = "None";
	prefsManager->appPrefs.toolSettings.dPenText = tabTools->colorComboText->currentText();
	if (prefsManager->appPrefs.toolSettings.dPenText == ScApp->noneString)
		prefsManager->appPrefs.toolSettings.dPenText = "None";
	prefsManager->appPrefs.toolSettings.dStrokeText = tabTools->colorComboStrokeText->currentText();
	if (prefsManager->appPrefs.toolSettings.dStrokeText == ScApp->noneString)
		prefsManager->appPrefs.toolSettings.dStrokeText = "None";
	prefsManager->appPrefs.toolSettings.dCols = tabTools->columnsText->value();
	prefsManager->appPrefs.toolSettings.dGap = tabTools->gapText->value() / prefsUnitRatio;
	prefsManager->appPrefs.toolSettings.dTabWidth = tabTools->gapTab->value() / prefsUnitRatio;
	prefsManager->appPrefs.toolSettings.dBrush = tabTools->comboFillShape->currentText();
	if (prefsManager->appPrefs.toolSettings.dBrush == ScApp->noneString)
		prefsManager->appPrefs.toolSettings.dBrush = "None";
	prefsManager->appPrefs.toolSettings.dShade = tabTools->shadingFillShape->value();
	prefsManager->appPrefs.toolSettings.dShade2 = tabTools->shadingLineShape->value();
	switch (tabTools->tabFillCombo->currentItem())
	{
		case 0:
			prefsManager->appPrefs.toolSettings.tabFillChar = "";
			break;
		case 1:
			prefsManager->appPrefs.toolSettings.tabFillChar = ".";
			break;
		case 2:
			prefsManager->appPrefs.toolSettings.tabFillChar = "-";
			break;
		case 3:
			prefsManager->appPrefs.toolSettings.tabFillChar = "_";
			break;
		case 4:
			prefsManager->appPrefs.toolSettings.tabFillChar = tabTools->tabFillCombo->currentText().right(1);
			break;
	}
	switch (tabTools->comboStyleShape->currentItem())
	{
		case 0:
			prefsManager->appPrefs.toolSettings.dLineArt = SolidLine;
			break;
		case 1:
			prefsManager->appPrefs.toolSettings.dLineArt = DashLine;
			break;
		case 2:
			prefsManager->appPrefs.toolSettings.dLineArt = DotLine;
			break;
		case 3:
			prefsManager->appPrefs.toolSettings.dLineArt = DashDotLine;
			break;
		case 4:
			prefsManager->appPrefs.toolSettings.dLineArt = DashDotDotLine;
			break;
	}
	prefsManager->appPrefs.toolSettings.dWidth = tabTools->lineWidthShape->value();
	prefsManager->appPrefs.toolSettings.dPenLine = tabTools->colorComboLine->currentText();
	if (prefsManager->appPrefs.toolSettings.dPenLine == ScApp->noneString)
		prefsManager->appPrefs.toolSettings.dPenLine = "None";
	prefsManager->appPrefs.toolSettings.dShadeLine = tabTools->shadingLine->value();
	switch (tabTools->comboStyleLine->currentItem())
	{
	case 0:
		prefsManager->appPrefs.toolSettings.dLstyleLine = SolidLine;
		break;
	case 1:
		prefsManager->appPrefs.toolSettings.dLstyleLine = DashLine;
		break;
	case 2:
		prefsManager->appPrefs.toolSettings.dLstyleLine = DotLine;
		break;
	case 3:
		prefsManager->appPrefs.toolSettings.dLstyleLine = DashDotLine;
		break;
	case 4:
		prefsManager->appPrefs.toolSettings.dLstyleLine = DashDotDotLine;
		break;
	}
	prefsManager->appPrefs.toolSettings.dWidthLine = tabTools->lineWidthLine->value();
	prefsManager->appPrefs.toolSettings.dStartArrow = tabTools->startArrow->currentItem();
	prefsManager->appPrefs.toolSettings.dEndArrow = tabTools->endArrow->currentItem();
	prefsManager->appPrefs.toolSettings.magMin = tabTools->minimumZoom->value();
	prefsManager->appPrefs.toolSettings.magMax = tabTools->maximumZoom->value();
	prefsManager->appPrefs.toolSettings.magStep = tabTools->zoomStep->value();
	prefsManager->appPrefs.toolSettings.dBrushPict = tabTools->comboFillImage->currentText();
	if (prefsManager->appPrefs.toolSettings.dBrushPict == ScApp->noneString)
		prefsManager->appPrefs.toolSettings.dBrushPict = "None";
	prefsManager->appPrefs.toolSettings.shadePict = tabTools->shadingFillImage->value();
	prefsManager->appPrefs.toolSettings.scaleX = static_cast<double>(tabTools->scalingHorizontal->value()) / 100.0;
	prefsManager->appPrefs.toolSettings.scaleY = static_cast<double>(tabTools->scalingVertical->value()) / 100.0;
	prefsManager->appPrefs.toolSettings.scaleType = tabTools->buttonGroup3->isChecked();
	prefsManager->appPrefs.toolSettings.aspectRatio = tabTools->checkRatioImage->isChecked();
	prefsManager->appPrefs.toolSettings.useEmbeddedPath = tabTools->embeddedPath->isChecked();
	int haRes = 0;
	if (tabTools->checkFullRes->isChecked())
		haRes = 0;
	if (tabTools->checkNormalRes->isChecked())
		haRes = 1;
	if (tabTools->checkHalfRes->isChecked())
		haRes = 2;
	prefsManager->appPrefs.toolSettings.lowResType = haRes;
	prefsManager->appPrefs.AutoSave = ASon->isChecked();
	prefsManager->appPrefs.AutoSaveTime = ASTime->value() * 60 * 1000;
	prefsManager->appPrefs.MinWordLen = tabHyphenator->wordLen->value();
	prefsManager->appPrefs.Language = ScApp->GetLang(tabHyphenator->language->currentText());
	prefsManager->appPrefs.Automatic = !tabHyphenator->verbose->isChecked();
	prefsManager->appPrefs.AutoCheck = tabHyphenator->input->isChecked();
	prefsManager->appPrefs.HyCount = tabHyphenator->maxCount->value();
	if (CMSavail)
		tabColorManagement->setValues();
	uint a = 0;
	SCFontsIterator it(prefsManager->appPrefs.AvailFonts);
	for ( ; it.current() ; ++it)
	{
		it.current()->EmbedPS = tabFonts->fontFlags[it.currentKey()].FlagPS;
		it.current()->UseFont = tabFonts->fontFlags[it.currentKey()].FlagUse;
		it.current()->Subset = tabFonts->fontFlags[it.currentKey()].FlagSub;
	}
	a = 0;
	QMap<QString,QString>::Iterator itfsuend=tabFonts->RList.end();
	prefsManager->appPrefs.GFontSub.clear();
	for (QMap<QString,QString>::Iterator itfsu = tabFonts->RList.begin(); itfsu != itfsuend; ++itfsu)
		prefsManager->appPrefs.GFontSub[itfsu.key()] = tabFonts->FlagsRepl.at(a++)->currentText();
	prefsManager->appPrefs.PDF_Options.Thumbnails = tabPDF->CheckBox1->isChecked();
	prefsManager->appPrefs.PDF_Options.Compress = tabPDF->Compression->isChecked();
	prefsManager->appPrefs.PDF_Options.CompressMethod = tabPDF->CMethod->currentItem();
	prefsManager->appPrefs.PDF_Options.Quality = tabPDF->CQuality->currentItem();
	prefsManager->appPrefs.PDF_Options.Resolution = tabPDF->Resolution->value();
	prefsManager->appPrefs.PDF_Options.RecalcPic = tabPDF->DSColor->isChecked();
	prefsManager->appPrefs.PDF_Options.PicRes = tabPDF->ValC->value();
	prefsManager->appPrefs.PDF_Options.Bookmarks = tabPDF->CheckBM->isChecked();
	prefsManager->appPrefs.PDF_Options.Binding = tabPDF->ComboBind->currentItem();
	prefsManager->appPrefs.PDF_Options.MirrorH = tabPDF->MirrorH->isOn();
	prefsManager->appPrefs.PDF_Options.MirrorV = tabPDF->MirrorV->isOn();
	prefsManager->appPrefs.PDF_Options.RotateDeg = tabPDF->RotateDeg->currentItem() * 90;
	prefsManager->appPrefs.PDF_Options.Articles = tabPDF->Article->isChecked();
	prefsManager->appPrefs.PDF_Options.Encrypt = tabPDF->Encry->isChecked();
	prefsManager->appPrefs.PDF_Options.UseLPI = tabPDF->UseLPI->isChecked();
	prefsManager->appPrefs.PDF_Options.UseSpotColors = !tabPDF->useSpot->isChecked();
	prefsManager->appPrefs.PDF_Options.doMultiFile = false;
	prefsManager->appPrefs.PDF_Options.BleedBottom = tabPDF->BleedBottom->value() / prefsUnitRatio;
	prefsManager->appPrefs.PDF_Options.BleedTop = tabPDF->BleedTop->value() / prefsUnitRatio;
	prefsManager->appPrefs.PDF_Options.BleedLeft = tabPDF->BleedLeft->value() / prefsUnitRatio;
	prefsManager->appPrefs.PDF_Options.BleedRight = tabPDF->BleedRight->value() / prefsUnitRatio;
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
		prefsManager->appPrefs.PDF_Options.Permissions = Perm;
		prefsManager->appPrefs.PDF_Options.PassOwner = tabPDF->PassOwner->text();
		prefsManager->appPrefs.PDF_Options.PassUser = tabPDF->PassUser->text();
	}
	if (tabPDF->PDFVersionCombo->currentItem() == 0)
		prefsManager->appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_13;
	if (tabPDF->PDFVersionCombo->currentItem() == 1)
		prefsManager->appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_14;
	if (tabPDF->PDFVersionCombo->currentItem() == 2)
		prefsManager->appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_15;
	if (tabPDF->PDFVersionCombo->currentItem() == 3)
		prefsManager->appPrefs.PDF_Options.Version = PDFOptions::PDFVersion_X3;
	if (tabPDF->OutCombo->currentItem() == 0)
	{
		prefsManager->appPrefs.PDF_Options.isGrayscale = false;
		prefsManager->appPrefs.PDF_Options.UseRGB = true;
		prefsManager->appPrefs.PDF_Options.UseProfiles = false;
		prefsManager->appPrefs.PDF_Options.UseProfiles2 = false;
	}
	else
	{
		if (tabPDF->OutCombo->currentItem() == 2)
		{
			prefsManager->appPrefs.PDF_Options.isGrayscale = true;
			prefsManager->appPrefs.PDF_Options.UseRGB = false;
			prefsManager->appPrefs.PDF_Options.UseProfiles = false;
			prefsManager->appPrefs.PDF_Options.UseProfiles2 = false;
		}
		else
		{
			prefsManager->appPrefs.PDF_Options.isGrayscale = false;
			prefsManager->appPrefs.PDF_Options.UseRGB = false;
#ifdef HAVE_CMS
			if (CMSuse)
			{
				prefsManager->appPrefs.PDF_Options.UseProfiles = tabPDF->EmbedProfs->isChecked();
				prefsManager->appPrefs.PDF_Options.UseProfiles2 = tabPDF->EmbedProfs2->isChecked();
				prefsManager->appPrefs.PDF_Options.Intent = tabPDF->IntendS->currentItem();
				prefsManager->appPrefs.PDF_Options.Intent2 = tabPDF->IntendI->currentItem();
				prefsManager->appPrefs.PDF_Options.EmbeddedI = tabPDF->NoEmbedded->isChecked();
				prefsManager->appPrefs.PDF_Options.SolidProf = tabPDF->SolidPr->currentText();
				prefsManager->appPrefs.PDF_Options.ImageProf = tabPDF->ImageP->currentText();
				prefsManager->appPrefs.PDF_Options.PrintProf = tabPDF->PrintProfC->currentText();
			}
#endif
		}
	}
	prefsManager->appPrefs.defaultItemAttributes = *(tabDefaultItemAttributes->getNewAttributes());
	prefsManager->appPrefs.defaultToCSetups = *(tabDefaultTOCIndexPrefs->getNewToCs());
	prefsManager->appPrefs.KeyActions = tabKeys->getNewKeyMap();
}
