#include "prefs.h"
#include "prefs.moc"
#include "keymanager.h"
#include "scribusdoc.h"
#include <qfont.h>
#include <qcolordialog.h>
#include <qcombobox.h>
#include <qcolor.h>
#include <qstringlist.h>
#include <qstylefactory.h>
#include <qpointarray.h>
#include <qpainter.h>
#include <qrect.h>
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

using namespace std;

extern QPixmap loadIcon(QString nam);
extern bool CMSavail;
extern ProfilesL InputProfiles;

extern "C" void* Run(QWidget *d, ApplicationPrefs *prefsData);

/*!
 \fn void* Run(QWidget *d, ApplicationPrefs *prefsData)
 \author Franz Schmid
 \date
 \brief Creates Preferences dialog and returns pointer to it
 \param d QWidget * to Preferences dialog widget
 \param prefsData ApplicationPrefs * struct
 \retval dia * widget pointer
 */
void* Run(QWidget *d, ApplicationPrefs *prefsData)
{
	Preferences *dia = new Preferences(d, prefsData);
	return dia;
}

/*!
 \fn Preferences::Preferences( QWidget* parent, ApplicationPrefs *prefsData)
 \author Franz Schmid
 \date
 \brief Constructor for Preferences dialog box
 \param parent QWidget pointer to parent window
 \param prefsData ApplicationPrefs * struct
 \retval Preferences dialog
 */
Preferences::Preferences( QWidget* parent, ApplicationPrefs *prefsData) : PrefsDialogBase( parent )
{
	int decimals;
	ap = (ScribusApp*)parent;
	Umrech = 1.0;
	docUnitIndex = prefsData->docUnitIndex;
	int f[] = {2, 3, 4};
	if (docUnitIndex == 3)
		decimals = f[0];
	else
		decimals = f[docUnitIndex];
	DisScale = prefsData->DisScale;
	setCaption( tr( "Preferences" ) );

	tab = new QWidget( prefsWidgets, "tab" );
	tabLayout = new QGridLayout( tab );
	tabLayout->setSpacing( 6 );
	tabLayout->setMargin( 10 );
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
	guiLangCombo->setCurrentText(langMgr.getLangFromAbbrev(prefsData->guiLanguage));
	selectedGUILang=prefsData->guiLanguage;
	guiLangLabel = new QLabel(guiLangCombo, tr("&Language:"), ButtonGroup1, "guiLangLabel");
	ButtonGroup1Layout->addWidget( guiLangLabel, 0, 0 );
	ButtonGroup1Layout->addWidget( guiLangCombo, 0, 1, Qt::AlignLeft );

	GUICombo = new QComboBox( true, ButtonGroup1, "GUICombo" );
	QStringList STtest;
	STtest = QStyleFactory::keys();
	for (uint stt = 0; stt < STtest.count(); ++stt)
		GUICombo->insertItem(STtest[stt]);
	GUICombo->setEditable(false);
	GUICombo->setCurrentText(prefsData->GUI);
	TextGstil = new QLabel(GUICombo, tr("&Theme:"), ButtonGroup1, "dd");
	ButtonGroup1Layout->addWidget( TextGstil, 1, 0 );
	ButtonGroup1Layout->addWidget( GUICombo, 1, 1, Qt::AlignLeft );

	GFsize = new QSpinBox(ButtonGroup1, "gfs" );
	GFsize->setSuffix( tr( " pt" ) );
	GFsize->setMaxValue( 22 );
	GFsize->setMinValue( 8 );
	GFsize->setValue( prefsData->AppFontSize );
	TextGstil2 = new QLabel(GFsize, tr("&Font Size:"), ButtonGroup1, "dd");
	ButtonGroup1Layout->addWidget( TextGstil2, 2, 0 );
	ButtonGroup1Layout->addWidget( GFsize, 2, 1, Qt::AlignLeft );
	SpinBox3 = new QSpinBox( ButtonGroup1, "SpinBox3" );
	SpinBox3->setMaxValue( 1000 );
	SpinBox3->setMinValue( 0 );
	SpinBox3->setLineStep( 10 );
	SpinBox3->setValue( prefsData->Wheelval );
	TextLabel1_2 = new QLabel( SpinBox3, tr( "&Wheel Jump:" ), ButtonGroup1, "TextLabel1_2" );
	ButtonGroup1Layout->addWidget( TextLabel1_2, 3, 0 );
	ButtonGroup1Layout->addWidget( SpinBox3, 3, 1, Qt::AlignLeft );
	Recen = new QSpinBox( ButtonGroup1, "Recen" );
	Recen->setMaxValue( 30 );
	Recen->setMinValue( 1 );
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
	PfadText2 = new QLabel( ProPfad, tr("&ICC Profiles:"), GroupBox200, "Pfadtext1" );
	FileC2 = new QToolButton( GroupBox200, "FileC1" );
	FileC2->setMinimumSize( QSize( 88, 24 ) );
	FileC2->setText( tr( "C&hange..." ) );
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

	TemplateDir = new QLineEdit( GroupBox200, "Datei1" );
	TemplateDir->setMinimumSize( QSize( 268, 22 ) );
	TemplateDir->setText(prefsData->TemplateDir);
	PfadText4 = new QLabel( TemplateDir, tr("T&emplates:"), GroupBox200, "Pfadtext1" );
	FileC4 = new QToolButton( GroupBox200, "FileC4" );
	FileC4->setMinimumSize( QSize( 88, 24 ) );
	FileC4->setText( tr( "Cha&nge..." ) );
	GroupBox200Layout->addWidget( PfadText4, 3, 0 );
	GroupBox200Layout->addWidget( TemplateDir, 3, 1 );
	GroupBox200Layout->addWidget( FileC4, 3, 2 );
	tabLayout->addWidget( GroupBox200, 1, 0 );
	addItem( tr("General"), loadIcon("AppIcon.png"), tab);

	tab_7 = new QWidget( prefsWidgets, "tab_7" );
	tabLayout_7 = new QHBoxLayout( tab_7, 11, 6, "tabLayout_7");
	Layout21 = new QVBoxLayout( 0, 0, 6, "Layout21");
	GroupSize = new QButtonGroup( tr( "Page Size" ), tab_7, "GroupSize" );
	GroupSize->setColumnLayout(0, Qt::Vertical );
	GroupSize->layout()->setSpacing( 6 );
	GroupSize->layout()->setMargin( 10 );
	GroupSizeLayout = new QVBoxLayout( GroupSize->layout() );
	GroupSizeLayout->setAlignment( Qt::AlignTop );
	Layout6 = new QGridLayout( 0, 1, 1, 0, 6, "Layout6");

	GZComboF = new QComboBox( true, GroupSize, "GZComboF" );
	const QString ar_size[] =
	    {"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3",
	     "B4", "B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", tr("Executive"),
	     tr("Folio"), tr("Ledger"), tr("Legal"), tr("Letter"), tr("Tabloid")
	    };
	size_t ar_s = sizeof(ar_size) / sizeof(*ar_size);
	for (uint s = 0; s < ar_s; ++s)
		GZComboF->insertItem(ar_size[s]);
	GZComboF->insertItem( tr( "Custom" ) );
	GZComboF->setEditable(false);
	GZComboF->setCurrentItem(prefsData->PageFormat);
	GZText1 = new QLabel( GZComboF, tr( "&Size:" ), GroupSize, "GZText1" );
	Layout6->addWidget( GZText1, 0, 0 );
	Layout6->addWidget( GZComboF, 0, 1 );

	GZComboO = new QComboBox( true, GroupSize, "GZComboO" );
	GZComboO->insertItem( tr( "Portrait" ) );
	GZComboO->insertItem( tr( "Landscape" ) );
	GZComboO->setEditable(false);
	GZComboO->setCurrentItem(prefsData->Ausrichtung);
	GZText2 = new QLabel( GZComboO, tr( "Orie&ntation:" ), GroupSize, "GZText2" );
	Layout6->addWidget( GZText2, 1, 0 );
	Layout6->addWidget( GZComboO, 1, 1 );
	UnitCombo = new QComboBox( true, GroupSize, "UnitCombo" );
	UnitCombo->insertItem( tr( "Points (pt)" ) );
	UnitCombo->insertItem( tr( "Millimetres (mm)" ) );
	UnitCombo->insertItem( tr( "Inches (in)" ) );
	UnitCombo->insertItem( tr( "Picas (p)" ) );
	UnitCombo->setEditable(false);
	UnitCombo->setCurrentItem(prefsData->docUnitIndex);
	unitComboText = new QLabel( UnitCombo, tr( "Units:" ), GroupSize, "unitComboText" );
	Layout6->addWidget( unitComboText, 2, 0 );
	Layout6->addWidget( UnitCombo, 2, 1 );

	GroupSizeLayout->addLayout( Layout6 );

	Layout5_2 = new QHBoxLayout( 0, 0, 6, "Layout5_2");

	pageWidth = new MSpinBox( 1, 10000, GroupSize, decimals );
	pageWidth->setEnabled( false );
	pageWidth->setMinimumSize( QSize( 70, 20 ) );
	pageWidth->setValue(prefsData->PageBreite * Umrech);
	GZText3 = new QLabel( pageWidth, tr( "&Width:" ), GroupSize, "GZText3" );
	Layout5_2->addWidget( GZText3 );
	Layout5_2->addWidget( pageWidth );

	pageHeight = new MSpinBox( 1, 10000, GroupSize, decimals );
	pageHeight->setEnabled( false );
	pageHeight->setMinimumSize( QSize( 70, 20 ) );
	pageHeight->setValue(prefsData->PageHoehe * Umrech);
	GZText4 = new QLabel( pageHeight, tr( "&Height:" ), GroupSize, "GZText4" );
	Layout5_2->addWidget( GZText4 );
	Layout5_2->addWidget( pageHeight );

	GroupSizeLayout->addLayout( Layout5_2 );

	Layout8 = new QHBoxLayout( 0, 0, 6, "Layout8");

	facingPages = new QCheckBox( tr( "&Facing Pages" ), GroupSize, "facingPages" );
	facingPages->setChecked(prefsData->DoppelSeiten);
	Layout8->addWidget( facingPages );

	Linkszuerst = new QCheckBox( tr( "Left &Page First" ), GroupSize, "Linkszuerst" );
	Linkszuerst->setChecked(prefsData->ErsteLinks);
	if (!facingPages->isChecked())
		Linkszuerst->setEnabled(false);
	Layout8->addWidget( Linkszuerst );
	GroupSizeLayout->addLayout( Layout8 );
	Layout21->addWidget( GroupSize );

	GroupRand = new QGroupBox( tr( "Margin Guides" ), tab_7, "GroupRand" );
	GroupRand->setColumnLayout(0, Qt::Vertical );
	GroupRand->layout()->setSpacing( 5 );
	GroupRand->layout()->setMargin( 10 );
	GroupRandLayout = new QGridLayout( GroupRand->layout() );
	GroupRandLayout->setAlignment( Qt::AlignTop );

	TopR = new MSpinBox( 0, 1000, GroupRand, decimals );
	TopR->setMinimumSize( QSize( 70, 20 ) );
	TopR->setValue(prefsData->RandOben * Umrech);
	RandT = prefsData->RandOben;
	GroupRandLayout->addWidget( TopR, 0, 1 );
	BottomR = new MSpinBox( 0, 1000, GroupRand, decimals );
	BottomR->setMinimumSize( QSize( 70, 20 ) );
	BottomR->setValue(prefsData->RandUnten * Umrech);
	RandB = prefsData->RandUnten;
	GroupRandLayout->addWidget( BottomR, 1, 1 );
	RightR = new MSpinBox( 0, 1000, GroupRand, decimals );
	RightR->setMinimumSize( QSize( 70, 20 ) );
	RightR->setValue(prefsData->RandRechts * Umrech);
	RandR = prefsData->RandRechts;
	GroupRandLayout->addWidget( RightR, 1, 3 );
	LeftR = new MSpinBox( 0, 1000, GroupRand, decimals );
	LeftR->setMinimumSize( QSize( 70, 20 ) );
	LeftR->setValue(prefsData->RandLinks * Umrech);
	RandL = prefsData->RandLinks;
	GroupRandLayout->addWidget( LeftR, 0, 3 );

	GRText2 = new QLabel( BottomR, tr( "&Bottom:" ), GroupRand, "GRText2" );
	GroupRandLayout->addWidget( GRText2, 1, 0 );
	GRText1 = new QLabel( TopR, tr( "&Top:" ), GroupRand, "GRText1" );
	GroupRandLayout->addWidget( GRText1, 0, 0 );
	GRText4 = new QLabel( RightR, tr( "&Right:" ), GroupRand, "GRText4" );
	GroupRandLayout->addWidget( GRText4, 1, 2 );
	GRText3 = new QLabel( LeftR, tr( "&Left:" ), GroupRand, "GRText3" );
	GroupRandLayout->addWidget( GRText3, 0, 2 );

	Layout21->addWidget( GroupRand );
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

	urGroup = new QGroupBox(tr("Undo/Redo"), tab_7, "urGroup");
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

	tabGuides = new TabGuides(prefsWidgets, &prefsData->guidesSettings, &prefsData->typographicSetttings, Umrech, "");
	addItem( tr("Guides"), loadIcon("guides.png"), tabGuides);
	tabTypo = new TabTypograpy(  prefsWidgets, &prefsData->typographicSetttings);
	addItem( tr("Typography"), loadIcon("font.png"), tabTypo);
	tabTools = new TabTools(  prefsWidgets, &prefsData->toolSettings, Umrech, "", 0);
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets, &ap->LangTransl);
	tabHyphenator->verbose->setChecked(!prefsData->Automatic);
	tabHyphenator->input->setChecked(prefsData->AutoCheck);
	tabHyphenator->language->setCurrentText(ap->LangTransl[prefsData->Language]);
	tabHyphenator->wordLen->setValue(prefsData->MinWordLen);
	tabHyphenator->maxCount->setValue(prefsData->HyCount);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);
	
	tabFonts = new FontPrefs(  prefsWidgets, ap->Prefs.AvailFonts, false, prefsData, ap->PrefsPfad, 0);
	addItem( tr("Fonts"), loadIcon("font.png"), tabFonts);

	tabDocChecker = new TabCheckDoc(  prefsWidgets, prefsData->checkerProfiles, prefsData->curCheckProfile);
	addItem( tr("Doc-Checker"), loadIcon("checkdoc.png"), tabDocChecker);

	if (CMSavail)
	{
		tabColorManagement = new CMSPrefs(prefsWidgets, &prefsData->DCMSset, &InputProfiles, &ap->PrinterProfiles, &ap->MonitorProfiles);
		addItem( tr("Color Management"), loadIcon("blend.png"), tabColorManagement);
	}
	QMap<QString,QFont> DocFonts;
	DocFonts.clear();
	tabPDF = new TabPDFOptions( prefsWidgets,
																&prefsData->PDF_Optionen,
																ap->Prefs.AvailFonts,
																&ap->PDFXProfiles,
																DocFonts,
																prefsData->PDF_Optionen.PresentVals,
																Umrech,
																unitGetSuffixFromIndex(prefsData->docUnitIndex),
																prefsData->PageHoehe,
																prefsData->PageBreite,
																0 );
	addItem( tr("PDF Export"), loadIcon("acroread.png"), tabPDF);

	tabKeys = new KeyManager(this, prefsData->KeyActions);
	addItem( tr("Keyboard Shortcuts"), loadIcon("key_bindings.png"), tabKeys);

	tab_5 = new QWidget( prefsWidgets, "tab_5" );
	tabLayout_5 = new QGridLayout( tab_5 );
	tabLayout_5->setSpacing( 6 );
	tabLayout_5->setMargin( 11 );
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
	tabViewLayout = new QVBoxLayout( tabView, 10, 5, "tabViewLayout");
	tabViewLayout->setAlignment( Qt::AlignTop );
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
	pm5.fill(prefsData->DpapColor);
	colorPaper = prefsData->DpapColor;
	backColor->setPixmap(pm5);
	backColor->setText( QString::null );
	layout10->addWidget( backColor );
	QSpacerItem* spacer3 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout10->addItem( spacer3 );
	pageBackgroundLayout->addLayout( layout10 );
	checkUnprintable = new QCheckBox( pageBackground, "checkUnprintable" );
	checkUnprintable->setText( tr( "Display &Unprintable Area in Margin Color" ) );
	checkUnprintable->setAccel( QKeySequence( tr( "Alt+U" ) ) );
	checkUnprintable->setChecked( prefsData->marginColored );
	pageBackgroundLayout->addWidget( checkUnprintable );
	checkPictures = new QCheckBox( pageBackground, "checkPictures" );
	checkPictures->setText( tr( "Show Pictures" ) );
	checkPictures->setChecked(prefsData->guidesSettings.showPic);
	pageBackgroundLayout->addWidget( checkPictures );
	checkLink = new QCheckBox( pageBackground, "checkLink" );
	checkLink->setText( tr( "Show Text Chains" ) );
	checkLink->setChecked(prefsData->guidesSettings.linkShown);
	pageBackgroundLayout->addWidget( checkLink );
	checkFrame = new QCheckBox( pageBackground, "checkFrame" );
	checkFrame->setText( tr( "Show Frames" ) );
	checkFrame->setChecked(prefsData->guidesSettings.framesShown);
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
	Layout4s = new QGridLayout;
	Layout4s->setSpacing( 6 );
	Layout4s->setMargin( 0 );
	topScratch = new MSpinBox( groupScratch, 4 );
	topScratch->setDecimals( decimals );
	topScratch->setMaxValue(1000);
	topScratch->setValue(prefsData->ScratchTop * Umrech);
	Layout4s->addWidget( topScratch, 0, 1 );
	TextLabel5s = new QLabel(topScratch, tr( "&Top:" ), groupScratch, "TextLabel5" );
	Layout4s->addWidget( TextLabel5s, 0, 0 );
	leftScratch = new MSpinBox( groupScratch, 4 );
	leftScratch->setDecimals( decimals );
	leftScratch->setMaxValue(1000);
	leftScratch->setValue(prefsData->ScratchLeft * Umrech);
	Layout4s->addWidget( leftScratch, 0, 3 );
	Linkss = new QLabel(leftScratch, tr( "&Left:" ), groupScratch, "Links" );
	Layout4s->addWidget( Linkss, 0, 2 );
	bottomScratch = new MSpinBox( groupScratch, 4 );
	bottomScratch->setDecimals( decimals );
	bottomScratch->setMaxValue(1000);
	bottomScratch->setValue(prefsData->ScratchBottom * Umrech);
	Layout4s->addWidget( bottomScratch, 1, 1 );
	TextLabel7s = new QLabel(bottomScratch, tr( "&Bottom:" ), groupScratch, "TextLabel7" );
	Layout4s->addWidget( TextLabel7s, 1, 0 );
	rightScratch = new MSpinBox( groupScratch, 4 );
	rightScratch->setDecimals( decimals );
	rightScratch->setMaxValue(1000);
	rightScratch->setValue(prefsData->ScratchRight * Umrech);
	Layout4s->addWidget( rightScratch, 1, 3 );
	Rechtss = new QLabel(rightScratch, tr( "&Right:" ), groupScratch, "Rechts" );
	Layout4s->addWidget( Rechtss, 1, 2 );
	groupScratchLayout->addLayout( Layout4s );
	tabViewLayout->addWidget( groupScratch );

	CaliGroup = new QGroupBox( tr( "&Adjust Display Size" ), tabView, "CaliGroup" );
	CaliGroup->setColumnLayout(0, Qt::Vertical );
	CaliGroup->layout()->setSpacing( 6 );
	CaliGroup->layout()->setMargin( 11 );
	CaliGroupLayout = new QVBoxLayout( CaliGroup->layout() );
	CaliGroupLayout->setAlignment( Qt::AlignTop );
	CaliText = new QLabel( tr( "To adjust the display drag the ruler below with the Slider." ), CaliGroup, "CaliText" );
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
	ExtToolLayout = new QVBoxLayout( ExtTool, 11, 6, "ExtToolLayout");
	ExtToolLayout->setAlignment( Qt::AlignTop );
	groupGS = new QGroupBox( tr( "Postscript Interpreter" ), ExtTool, "groupGS" );
	groupGS->setColumnLayout(0, Qt::Vertical );
	groupGS->layout()->setSpacing( 6 );
	groupGS->layout()->setMargin( 10 );
	groupGSLayout = new QVBoxLayout( groupGS->layout() );
	groupGSLayout->setAlignment( Qt::AlignTop );
	GSlayout = new QHBoxLayout( 0, 0, 6, "GSlayout");
	GSName = new QLineEdit( groupGS, "GSName" );
	GSName->setText(prefsData->gs_exe);
	GSText = new QLabel( GSName, tr( "&Name of Executable:" ), groupGS, "GSText" );
	GSlayout->addWidget( GSText );
	GSlayout->addWidget( GSName );
	groupGSLayout->addLayout( GSlayout );
	GSantiText = new QCheckBox( tr( "Antialias &Text" ), groupGS, "GSantiText" );
	GSantiText->setChecked(prefsData->gs_antiText);
	groupGSLayout->addWidget( GSantiText );
	GSantiGraph = new QCheckBox( tr( "Antialias &Graphics" ), groupGS, "GSantiGraph" );
	GSantiGraph->setChecked(prefsData->gs_antiGraph);
	groupGSLayout->addWidget( GSantiGraph );
	ExtToolLayout->addWidget( groupGS );
	groupGimp = new QGroupBox( tr( "Image Processing Tool" ), ExtTool, "groupGimp" );
	groupGimp->setColumnLayout(0, Qt::Vertical );
	groupGimp->layout()->setSpacing( 6 );
	groupGimp->layout()->setMargin( 10 );
	groupGimpLayout = new QHBoxLayout( groupGimp->layout() );
	groupGimpLayout->setAlignment( Qt::AlignTop );
	GimpName = new QLineEdit( groupGimp, "GimpName" );
	GimpName->setText(prefsData->gimp_exe);
	GimpText = new QLabel( GimpName, tr( "Name of &Executable:" ), groupGimp, "GimpText" );
	groupGimpLayout->addWidget( GimpText );
	groupGimpLayout->addWidget( GimpName );
	ExtToolLayout->addWidget( groupGimp );
	QSpacerItem* spacer_gs = new QSpacerItem( 0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	ExtToolLayout->addItem( spacer_gs );
	addItem(  tr("External Tools."), loadIcon("misc.png"), ExtTool);

	Misc = new QWidget( prefsWidgets, "Misc" );
	MiscLayout = new QVBoxLayout( Misc, 11, 6, "MiscLayout");
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
	QSpacerItem* spacer_3m = new QSpacerItem( 0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	MiscLayout->addItem( spacer_3m );
	addItem(  tr("Misc."), loadIcon("misc.png"), Misc);

	setDS();
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
	QWidget::setTabOrder( FileC3, TemplateDir );
	QWidget::setTabOrder( TemplateDir, FileC4 );


	QWidget::setTabOrder( TopR, BottomR );
	QWidget::setTabOrder( BottomR, LeftR );
	QWidget::setTabOrder( LeftR, RightR );

	QWidget::setTabOrder( PreviewSize, SaveAtQuit );

	QToolTip::add( checkLink, tr("Turns the of linked frames on or off"));
	QToolTip::add( checkFrame, tr("Turns the display of frames on or off"));
	QToolTip::add( checkPictures, tr("Turns the display of pictures on or off"));
	QToolTip::add( guiLangCombo, tr( "Select your default language for Scribus to run with.\nLeave this blank to choose based on environment variables.\nYou can still override this by passing a command line option when starting Scribus" ) );
	QToolTip::add( GUICombo, tr( "Choose the default window decoration and looks.\nScribus inherits any available KDE or Qt themes" ) );
	QToolTip::add( GFsize, tr( "Default font size for the menus and windows" ) );
	QToolTip::add( UnitCombo, tr( "Default unit of measurement for document editing" ) );
	QToolTip::add( SpinBox3, tr( "Number of lines Scribus will scroll for each move of the mouse wheel" ) );
	QToolTip::add( Recen, tr( "Number of recently edited documents to show in the File menu" ) );
	QToolTip::add( Docs, tr( "Default documents directory" ) );
	QToolTip::add( ProPfad, tr( "Default ICC profiles directory" ) );
	QToolTip::add( ScriptPfad, tr( "Default Scripter scripts directory" ) );
	QToolTip::add( TemplateDir, tr( "Additional Directory for Document Templates" ) );

	QToolTip::add( GZComboF, tr( "Default page size, either a standard size or a custom size" ) );
	QToolTip::add( GZComboO, tr( "Default orientation of document pages" ) );
	QToolTip::add( pageWidth, tr( "Width of document pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( pageHeight, tr( "Height of document pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( facingPages, tr( "Enable single or spread based layout" ) );
	QToolTip::add( Linkszuerst, tr( "Make the first page the left page of a document" ) );
	QToolTip::add( TopR, tr( "Distance between the top margin guide and the edge of the page" ) );
	QToolTip::add( BottomR, tr( "Distance between the bottom margin guide and the edge of the page" ) );
	QToolTip::add( LeftR, tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( RightR, tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( ASon, tr( "When enabled, Scribus saves a backup copy of your file with the .bak extension\neach time the time period elapses" ) );
	QToolTip::add( ASTime, tr( "Time period between saving automatically" ) );

	QToolTip::add( urSpinBox, tr("Set the length of the action history in steps.\nIf set to 0 infinite amount of actions will be stored."));

	QToolTip::add( PreviewSize, tr( "Choose the size of the preview in the scrapbook palette" ) );
	QToolTip::add( SaveAtQuit, tr( "Save the scrapbook contents everytime after a change" ) );

	QToolTip::add( backColor, tr( "Color for paper" ) );
	QToolTip::add( checkUnprintable, tr( "Mask the area outside the margins in the margin color" ) );
	QToolTip::add( CaliSlider, tr( "Set the default zoom level" ) );

	QToolTip::add( GSName, tr( "Filesystem location for the Ghostscript interpreter" ) );
	QToolTip::add( GSantiText, tr( "Antialias text for EPS and PDF onscreen rendering" ) );
	QToolTip::add( GSantiGraph, tr( "Antialias graphics for EPS and PDF onscreen rendering" ) );
	QToolTip::add( GimpName, tr( "Filesystem location for graphics editor" ) );

	QToolTip::add( ClipMarg, tr( "Do not show objects outside the margins on the printed page or exported file" ) );
	QToolTip::add( DoGCR, tr( "A way of switching off some of the gray shades which are composed\n"
		                          "of cyan, yellow and magenta and using black instead.\n"
		                          "UCR most affects parts of images which are neutral and/or dark tones\n"
		                          "which are close to the gray. Use of this may improve printing some images\n"
		                          "and some experimentation and testing is need on a case by case basis.\n"
		                          "UCR reduces the possibility of over saturation with CMY inks." ) );

	// signals and slots connections
	connect( guiLangCombo, SIGNAL( activated( const QString & ) ), this, SLOT( setSelectedGUILang( const QString & ) ) );
	connect(backColor, SIGNAL(clicked()), this, SLOT(changePaperColor()));
	connect(UnitCombo, SIGNAL(activated(int)), this, SLOT(unitChange()));
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	connect(GZComboO, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(GZComboF, SIGNAL(activated(int)), this, SLOT(setSize(int)));
	connect(facingPages, SIGNAL(clicked()), this, SLOT(setDS()));
	connect(FileC, SIGNAL(clicked()), this, SLOT(changeDocs()));
	connect(FileC2, SIGNAL(clicked()), this, SLOT(changeProfs()));
	connect(FileC3, SIGNAL(clicked()), this, SLOT(changeScripts()));
	connect(FileC4, SIGNAL(clicked()), this, SLOT(changeTemplates()));
	connect(CaliSlider, SIGNAL(valueChanged(int)), this, SLOT(setDisScale()));
	connect(buttonOk, SIGNAL(clicked()), this, SLOT(setActionHistoryLength()));
	if (CMSavail)
		connect(tabColorManagement, SIGNAL(cmsOn(bool )), this, SLOT(switchCMS(bool )));
	setSize(prefsData->PageFormat);
	setOrien(prefsData->Ausrichtung);
	pageWidth->setValue(prefsData->PageBreite * Umrech);
	pageHeight->setValue(prefsData->PageHoehe * Umrech);
	unitChange();
	prefsWidgets->raiseWidget(0);
	resize( minimumSizeHint() );
	arrangeIcons();
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
	if (s != "")
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
	if (s != "")
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
	if (s != "")
		ScriptPfad->setText(s);
}

/*!
 \fn void Preferences::ChangeTemplates()
 \author Riku Leino
 \date
 \brief Runs QFileDialog to get Preferences (General) Path to Templates directory
 \param None
 \retval None
 */
void Preferences::changeTemplates()
{
	QString s = QFileDialog::getExistingDirectory(TemplateDir->text(), this, "d", tr("Choose a Directory"), true);
	if (s != "")
		TemplateDir->setText(s);
}

/*!
 \fn void Preferences::setDS()
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), switches default between Facing Pages and swaps text labels for margin guides
 \param None
 \retval None
 */
void Preferences::setDS()
{
	bool m = facingPages->isChecked() ? true : false;
	GRText3->setText(m == true ? tr( "&Inside:" ) : tr( "&Left:" ));
	GRText4->setText(m == true ? tr( "O&utside:" ) : tr( "&Right:" ));
	Linkszuerst->setEnabled(m);
	if (m == false)
		Linkszuerst->setChecked(false);
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
	Pagebr = pageWidth->value() / Umrech;
	RightR->setMaxValue(pageWidth->value() - LeftR->value());
	LeftR->setMaxValue(pageWidth->value() - RightR->value());
	TopR->setMaxValue(pageHeight->value() - BottomR->value());
	BottomR->setMaxValue(pageHeight->value() - TopR->value());
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
	Pageho = pageHeight->value() / Umrech;
	RightR->setMaxValue(pageWidth->value() - LeftR->value());
	LeftR->setMaxValue(pageWidth->value() - RightR->value());
	TopR->setMaxValue(pageHeight->value() - BottomR->value());
	BottomR->setMaxValue(pageHeight->value() - TopR->value());
}

/*!
 \fn void Preferences::setTop(int v)
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), sets Page top values
 \param v Top value
 \retval None
 */
void Preferences::setTop(int)
{
	RandT = TopR->value() / Umrech;
	BottomR->setMaxValue(pageHeight->value() - TopR->value());
}

/*!
 \fn void Preferences::setBottom(int v)
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), sets Page bottom values
 \param v Bottom value
 \retval None
 */
void Preferences::setBottom(int)
{
	RandB = BottomR->value() / Umrech;
	TopR->setMaxValue(pageHeight->value() - BottomR->value());
}

/*!
 \fn void Preferences::setLeft(int v)
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), sets Page left values
 \param v Top value
 \retval None
 */
void Preferences::setLeft(int)
{
	RandL = LeftR->value() / Umrech;
	RightR->setMaxValue(pageWidth->value() - LeftR->value());
}

/*!
 \fn void Preferences::setRight(int v)
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), sets Page right values
 \param v Right value
 \retval None
 */
void Preferences::setRight(int)
{
	RandR = RightR->value() / Umrech;
	LeftR->setMaxValue(pageWidth->value() - RightR->value());
}

/*!
 \fn void Preferences::setSize(int gr)
 \author Franz Schmid
 \date
 \brief Preferences (Document / Page Size), sets Page size values. Connects signals for setting page dimensions.
 \param gr Standard page size value (eg A4)
 \retval None
 */
void Preferences::setSize(int gr)
{
	Pagebr = pageWidth->value() / Umrech;
	Pageho = pageHeight->value() / Umrech;
	pageWidth->setEnabled(false);
	pageHeight->setEnabled(false);
	int br[] = {2380, 1684, 1190, 842, 595, 421, 297, 210, 148, 105, 2836, 2004, 1418, 1002, 709, 501, 355,
	            250, 178, 125, 89, 462, 298, 312, 542, 595, 1224, 612, 612, 792};
	int ho[] = {3368, 2380, 1684, 1190, 842, 595, 421, 297, 210, 148, 4008, 2836, 2004, 1418, 1002, 709, 501,
	            355, 250, 178, 125, 649, 683, 624, 720, 935, 792, 1008, 792, 1224};
	if (gr == 30)
	{
		pageWidth->setEnabled(true);
		pageHeight->setEnabled(true);
	}
	else
	{
		Pagebr = br[gr];
		Pageho = ho[gr];
	}
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	pageWidth->setValue(Pagebr * Umrech);
	pageHeight->setValue(Pageho * Umrech);
	RightR->setMaxValue(pageWidth->value() - LeftR->value());
	LeftR->setMaxValue(pageWidth->value() - RightR->value());
	TopR->setMaxValue(pageHeight->value() - BottomR->value());
	BottomR->setMaxValue(pageHeight->value() - TopR->value());
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
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
	setSize(GZComboF->currentItem());
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	if (ori == 0)
	{
		if (GZComboF->currentItem() == 30)
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
 \fn void Preferences::changePapColor()
 \author Franz Schmid
 \date
 \brief Preferences ([dox?], [dox?]), Sets Paper colour [dox?]
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
	disconnect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	disconnect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	disconnect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	disconnect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	int decimals;
	double AltUmrech = Umrech;
	double oldMin, oldMax, oldB, oldBM, oldH, oldHM, val;
	pageWidth->getValues(&oldB, &oldBM, &decimals, &val);
	oldB /= AltUmrech;
	oldBM /= AltUmrech;
	pageHeight->getValues(&oldH, &oldHM, &decimals, &val);
	oldH /= AltUmrech;
	oldHM /= AltUmrech;
	QString einh;
	docUnitIndex = UnitCombo->currentItem();
	switch (UnitCombo->currentItem())
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
	pageWidth->setSuffix(einh);
	pageHeight->setSuffix(einh);
	TopR->setSuffix(einh);
	BottomR->setSuffix(einh);
	LeftR->setSuffix(einh);
	RightR->setSuffix(einh);
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
	pageWidth->setValues(oldB * Umrech, oldBM * Umrech, decimals, Pagebr * Umrech);
	pageHeight->setValues(oldH * Umrech, oldHM * Umrech, decimals, Pageho * Umrech);
	TopR->setValues(0, pageHeight->value() - RandB * Umrech, decimals, RandT * Umrech);
	BottomR->setValues(0, pageHeight->value() - RandT * Umrech, decimals, RandB * Umrech);
	LeftR->setValues(0, pageWidth->value() - RandR * Umrech, decimals, RandL * Umrech);
	RightR->setValues(0, pageWidth->value() - RandL * Umrech, decimals, RandR * Umrech);
	int decimalsOld;
	double invUnitConversion = 1.0 / AltUmrech * Umrech;
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
	tabPDF->unitConv = Umrech;
	drawRuler();
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
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

