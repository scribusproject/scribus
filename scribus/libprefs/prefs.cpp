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
#include "langmgr.h"
#include "fontcombo.h"
#include "polygonwidget.h"

using namespace std;

extern QPixmap fontSamples(QString da, int s, QString ts, QColor back);
extern QPixmap loadIcon(QString nam);

extern "C" void* Run(QWidget *d, preV *prefsData);

/*!
 \fn void* Run(QWidget *d, preV *prefsData)
 \author Franz Schmid
 \date  
 \brief Creates Preferences dialog and returns pointer to it
 \param d QWidget * to Preferences dialog widget
 \param prefsData preV * struct
 \retval dia * widget pointer
 */
void* Run(QWidget *d, preV *prefsData)
{
	Preferences *dia = new Preferences(d, prefsData);
	return dia;
}

/*!
 \fn Preferences::Preferences( QWidget* parent, preV *prefsData)
 \author Franz Schmid 
 \date  
 \brief Constructor for Preferences dialog box
 \param parent QWidget pointer to parent window
 \param prefsData preV * struct
 \retval Preferences dialog
 */
Preferences::Preferences( QWidget* parent, preV *prefsData) : PrefsDialogBase( parent )
{
	int decimals;
	fon = &prefsData->AvailFonts;
	ap = (ScribusApp*)parent;
	Umrech = 1.0;
	Einheit = prefsData->Einheit;
	int f[] = {2, 3, 4};
	if (Einheit == 3)
		decimals = f[0];
	else
		decimals = f[Einheit];
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
	ButtonGroup1Layout->addWidget( guiLangCombo, 0, 1, Qt::AlignRight );
	
	GUICombo = new QComboBox( true, ButtonGroup1, "GUICombo" );
	QStringList STtest;
	STtest = QStyleFactory::keys();
	for (uint stt = 0; stt < STtest.count(); ++stt)
		GUICombo->insertItem(STtest[stt]);
	GUICombo->setEditable(false);
	GUICombo->setCurrentText(prefsData->GUI);
	TextGstil = new QLabel(GUICombo, tr("&Theme:"), ButtonGroup1, "dd");
	ButtonGroup1Layout->addWidget( TextGstil, 1, 0 );
	ButtonGroup1Layout->addWidget( GUICombo, 1, 1, Qt::AlignRight );
	
	GFsize = new QSpinBox(ButtonGroup1, "gfs" );
	GFsize->setSuffix( tr( " pt" ) );
	GFsize->setMaxValue( 22 );
	GFsize->setMinValue( 8 );
	GFsize->setValue( prefsData->AppFontSize );
	TextGstil2 = new QLabel(GFsize, tr("&Font Size:"), ButtonGroup1, "dd");
	ButtonGroup1Layout->addWidget( TextGstil2, 2, 0 );
	ButtonGroup1Layout->addWidget( GFsize, 2, 1, Qt::AlignRight );
	tabLayout->addWidget( ButtonGroup1, 0, 0 );

	Mouse = new QGroupBox( tr( "Other Settings" ),tab, "Mouse" );
	Mouse->setColumnLayout(0, Qt::Vertical );
	Mouse->layout()->setSpacing( 0 );
	Mouse->layout()->setMargin( 0 );
	MouseLayout = new QHBoxLayout( Mouse->layout() );
	MouseLayout->setAlignment( Qt::AlignTop );
	MouseLayout->setSpacing( 6 );
	MouseLayout->setMargin( 10 );
	Layout5 = new QGridLayout;
	Layout5->setSpacing( 6 );
	Layout5->setMargin( 0 );
	
	SpinBox3 = new QSpinBox( Mouse, "SpinBox3" );
	SpinBox3->setMaxValue( 1000 );
	SpinBox3->setMinValue( 0 );
	SpinBox3->setLineStep( 10 );
	SpinBox3->setValue( prefsData->Wheelval );
	TextLabel1_2 = new QLabel( SpinBox3, tr( "&Wheel Jump:" ), Mouse, "TextLabel1_2" );
	TextLabel1_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabel1_2->sizePolicy().hasHeightForWidth() ) );
	Layout5->addWidget( TextLabel1_2, 0, 0 );
	Layout5->addWidget( SpinBox3, 0, 1 );
	Recen = new QSpinBox( Mouse, "Recen" );
	Recen->setMaxValue( 30 );
	Recen->setMinValue( 1 );
	Recen->setValue( prefsData->RecentDCount );
	TextLabel4c = new QLabel( Recen, tr( "&Recent Documents:" ), Mouse, "TextLabel4c" );
	TextLabel4c->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                            TextLabel4c->sizePolicy().hasHeightForWidth() ) );
	Layout5->addWidget( TextLabel4c, 1, 0);
	Layout5->addWidget( Recen, 1, 1 );
	MouseLayout->addLayout( Layout5 );
	tabLayout->addWidget( Mouse, 0, 1 );

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
	tabLayout->addMultiCellWidget( GroupBox200, 1, 1, 0, 1 );
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
	const QString ar_size[] = {"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3",
	                   "B4", "B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", "Executive",
	                   "Folio", "Ledger", "Legal", "Letter", "Tabloid"};
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

	GroupSizeLayout->addLayout( Layout6 );

	Layout5_2 = new QHBoxLayout( 0, 0, 6, "Layout5_2");

	Breite = new MSpinBox( 1, 10000, GroupSize, decimals );
	Breite->setEnabled( false );
	Breite->setMinimumSize( QSize( 70, 20 ) );
	Breite->setValue(prefsData->PageBreite * Umrech);
	GZText3 = new QLabel( Breite, tr( "&Width:" ), GroupSize, "GZText3" );
	Layout5_2->addWidget( GZText3 );	
	Layout5_2->addWidget( Breite );

	Hoehe = new MSpinBox( 1, 10000, GroupSize, decimals );
	Hoehe->setEnabled( false );
	Hoehe->setMinimumSize( QSize( 70, 20 ) );
	Hoehe->setValue(prefsData->PageHoehe * Umrech);
	GZText4 = new QLabel( Hoehe, tr( "&Height:" ), GroupSize, "GZText4" );
	Layout5_2->addWidget( GZText4 );	
	Layout5_2->addWidget( Hoehe );
	
	GroupSizeLayout->addLayout( Layout5_2 );
	
	Layout8 = new QHBoxLayout( 0, 0, 6, "Layout8");

	Doppelseiten = new QCheckBox( tr( "&Facing Pages" ), GroupSize, "Doppelseiten" );
	Doppelseiten->setChecked(prefsData->DoppelSeiten);
	Layout8->addWidget( Doppelseiten );

	Linkszuerst = new QCheckBox( tr( "Left &Page First" ), GroupSize, "Linkszuerst" );
	Linkszuerst->setChecked(prefsData->ErsteLinks);
	if (!Doppelseiten->isChecked())
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
	tabLayout_7->addLayout( Layout21 );

	Layout21b = new QVBoxLayout( 0, 0, 6, "Layout21b");
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
	Layout21b->addWidget( GroupAS );
	GroupBox20 = new QGroupBox( tr( "Units" ), tab_7, "GroupBox20" );
	GroupBox20->setColumnLayout(0, Qt::Vertical );
	GroupBox20->layout()->setSpacing( 0 );
	GroupBox20->layout()->setMargin( 0 );
	GroupBox20Layout = new QHBoxLayout( GroupBox20->layout() );
	GroupBox20Layout->setAlignment( Qt::AlignTop );
	GroupBox20Layout->setSpacing( 0 );
	GroupBox20Layout->setMargin( 25 );
	UnitCombo = new QComboBox( true, GroupBox20, "UnitCombo" );
	UnitCombo->insertItem( tr( "Points (pt)" ) );
	UnitCombo->insertItem( tr( "Millimetres (mm)" ) );
	UnitCombo->insertItem( tr( "Inches (in)" ) );
	UnitCombo->insertItem( tr( "Picas (p)" ) );
	UnitCombo->setEditable(false);
	UnitCombo->setCurrentItem(prefsData->Einheit);
	GroupBox20Layout->addWidget( UnitCombo );
	Layout21b->addWidget( GroupBox20 );
	QSpacerItem* spacerDocb = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout21b->addItem( spacerDocb );
	tabLayout_7->addLayout( Layout21b );
	addItem( tr("Document"), loadIcon("page.png"), tab_7);

	tabGuides = new QWidget( prefsWidgets, "tabView" );
	tabGuidesLayout = new QVBoxLayout( tabGuides, 10, 5, "tabViewLayout");
	tabGuidesLayout->setAlignment( Qt::AlignTop );
	checkGrid = new QGroupBox( tabGuides, "checkGrid" );
	checkGrid->setTitle( tr( "Show Grid" ) );
	checkGrid->setCheckable( true );
	checkGrid->setChecked(prefsData->GridShown);
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
	pm1.fill(prefsData->DmajColor);
	colorMajorGrid = prefsData->DmajColor;
	majorGridColor->setPixmap(pm1);
	majorGridColor->setText( QString::null );
	groupBox1Layout->addWidget( majorGridColor, 1, 1 );
	textLabel4 = new QLabel( groupBox1, "textLabel4" );
	textLabel4->setText( tr( "Color:" ) );
	groupBox1Layout->addWidget( textLabel4, 1, 0 );
	textLabel6 = new QLabel( groupBox1, "textLabel6" );
	textLabel6->setText( tr( "Spacing:" ) );
	groupBox1Layout->addWidget( textLabel6, 0, 0 );
	majorSpace = new MSpinBox( 10 * Umrech, 1000 * Umrech, groupBox1, decimals );
	majorSpace->setValue( prefsData->DmajGrid * Umrech );
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
	pm.fill(prefsData->DminColor);
	colorMinorGrid = prefsData->DminColor;
	minorGridColor->setPixmap(pm);
	minorGridColor->setText( QString::null );
	groupBox2Layout->addWidget( minorGridColor, 1, 1 );
	textLabel7 = new QLabel( groupBox2, "textLabel7" );
	textLabel7->setText( tr( "Spacing:" ) );
	groupBox2Layout->addWidget( textLabel7, 0, 0 );
	minorSpace = new MSpinBox(Umrech, 1000 * Umrech, groupBox2, decimals);
	minorSpace->setValue(prefsData->DminGrid * Umrech);
	groupBox2Layout->addWidget( minorSpace, 0, 1 );
	checkGridLayout->addWidget( groupBox2, 0, 1 );
	tabGuidesLayout->addWidget( checkGrid );
	layout11 = new QGridLayout( 0, 1, 1, 0, 5, "layout11");
	textLabel8 = new QLabel( tabGuides, "textLabel8" );
	textLabel8->setText( tr( "Guide Snap Distance:" ) );
	layout11->addWidget( textLabel8, 0, 0 );
	snapDistance = new MSpinBox( Umrech, 1000 * Umrech, tabGuides, decimals );
	snapDistance->setValue( prefsData->GuideRad * Umrech );
	layout11->addWidget( snapDistance, 0, 1, Qt::AlignLeft );
	textLabel82 = new QLabel( tabGuides, "textLabel8" );
	textLabel82->setText( tr( "&Grab Radius:" ) );
	layout11->addWidget( textLabel82, 1, 0 );	
	grabDistance = new QSpinBox( tabGuides, "grabDistance" );
	grabDistance->setMaxValue( 1000 );
	grabDistance->setMinValue( 1 );
	grabDistance->setLineStep( 1 );
	grabDistance->setValue(prefsData->GrabRad );
	grabDistance->setSuffix( tr( " px" ) );
	layout11->addWidget( grabDistance, 1, 1, Qt::AlignLeft );
	checkMargin = new QCheckBox( tabGuides, "checkMargin" );
	checkMargin->setText( tr( "Show Margins" ) );
	checkMargin->setChecked(prefsData->MarginsShown);
	layout11->addWidget( checkMargin, 2, 0 );
	checkGuides = new QCheckBox( tabGuides, "checkGuides" );
	checkGuides->setText( tr( "Show Guides" ) );
	checkGuides->setChecked(prefsData->GuidesShown);
	layout11->addWidget( checkGuides, 3, 0 );
	checkBaseline = new QCheckBox( tabGuides, "checkBaseline" );
	checkBaseline->setText( tr( "Show Baseline Grid" ) );
	checkBaseline->setChecked(prefsData->BaseShown);
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
	pm4.fill(prefsData->baseColor);
	colorBaselineGrid = prefsData->baseColor;
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
	pm3.fill(prefsData->guideColor);
	colorGuides = prefsData->guideColor;
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
	pm6.fill(prefsData->DmargColor);
	colorMargin = prefsData->DmargColor;
	marginColor->setPixmap(pm6);
	marginColor->setFlat( false );
	marginColor->setText( QString::null );
	layout9->addWidget( marginColor );
	layout11->addLayout( layout9, 2, 1 );
	tabGuidesLayout->addLayout( layout11 );
	layout12 = new QGridLayout( 0, 1, 1, 0, 5, "layout12");
	checkLink = new QCheckBox( tabGuides, "checkLink" );
	checkLink->setText( tr( "Show Text Chains" ) );
	checkLink->setChecked(prefsData->linkShown);
	layout12->addWidget( checkLink, 1, 0 );
	checkFrame = new QCheckBox( tabGuides, "checkFrame" );
	checkFrame->setText( tr( "Show Frames" ) );
	checkFrame->setChecked(prefsData->FramesShown);
	layout12->addWidget( checkFrame, 0, 0 );
	checkPictures = new QCheckBox( tabGuides, "checkPictures" );
	checkPictures->setText( tr( "Show Pictures" ) );
	layout12->addWidget( checkPictures, 0, 1 );
	checkPictures->setChecked(prefsData->ShowPic);
	tabGuidesLayout->addLayout( layout12 );
	addItem( tr("Guides"), loadIcon("guides.png"), tabGuides);

/*	RadioButton6 = new QRadioButton( tr( "In the &Background" ), ButtonGroup5, "RadioButton6" );
	RadioButton6->setChecked(prefsData->Before );
	Layout7->addWidget( RadioButton6 );
	RadioButton7 = new QRadioButton( tr( "In the Fore&ground" ), ButtonGroup5, "RadioButton6_2" );
	RadioButton7->setChecked(!prefsData->Before );
	Layout7->addWidget( RadioButton7 ); */

	tabTypo = new QWidget( prefsWidgets, "tabTypo" );
	tabTypoLayout = new QGridLayout( tabTypo, 1, 1, 11, 6, "tabTypoLayout");
	tabTypoLayout->setAlignment( Qt::AlignTop );
	groupBox1a = new QGroupBox( tabTypo, "groupBox1a" );
	groupBox1a->setColumnLayout(0, Qt::Vertical );
	groupBox1a->layout()->setSpacing( 6 );
	groupBox1a->layout()->setMargin( 11 );
	groupBox1a->setTitle( tr( "Subscript" ) );
	groupBox1aLayout = new QGridLayout( groupBox1a->layout() );
	groupBox1aLayout->setAlignment( Qt::AlignTop );
	subDisplacement = new QSpinBox( groupBox1a, "subDisplacement" );
	subDisplacement->setMaxValue( 100 );
	subDisplacement->setValue( prefsData->DVTief );
	subDisplacement->setSuffix( tr( " %" ) );
	groupBox1aLayout->addWidget( subDisplacement, 0, 1, Qt::AlignLeft );
	textLabel1a = new QLabel(subDisplacement, tr( "&Displacement:" ), groupBox1a, "textLabel1a" );
	groupBox1aLayout->addWidget( textLabel1a, 0, 0 );
	subScaling = new QSpinBox( groupBox1a, "subScaling" );
	subScaling->setMaxValue( 100 );
	subScaling->setMinValue( 1 );
	subScaling->setValue( prefsData->DVTiefSc );
	subScaling->setSuffix( tr( " %" ) );
	groupBox1aLayout->addWidget( subScaling, 0, 3, Qt::AlignLeft );
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
	superDisplacement->setValue( prefsData->DVHoch );
	superDisplacement->setSuffix( tr( " %" ) );
	groupBox2aLayout->addWidget( superDisplacement, 0, 1, Qt::AlignLeft );
	textLabel3a = new QLabel(superDisplacement, tr( "D&isplacement:" ), groupBox2a, "textLabel3a" );
	groupBox2aLayout->addWidget( textLabel3a, 0, 0 );
	superScaling = new QSpinBox( groupBox2a, "superScaling" );
	superScaling->setMaxValue( 100 );
	superScaling->setMinValue( 1 );
	superScaling->setValue( prefsData->DVHochSc );
	superScaling->setSuffix( tr( " %" ) );
	groupBox2aLayout->addWidget( superScaling, 0, 3, Qt::AlignLeft );
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
	capsScaling->setValue( prefsData->DVKapit );
	capsScaling->setSuffix( tr( " %" ) );
	groupBox3aLayout->addWidget( capsScaling, 0, 1, Qt::AlignLeft );
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
	baseGrid = new MSpinBox( 1, prefsData->PageHoehe * Umrech, groupBox4a, 1 );
	baseGrid->setValue(prefsData->BaseGrid * Umrech);
	groupBox4aLayout->addWidget( baseGrid, 0, 1, Qt::AlignLeft );
	textLabel6a = new QLabel(baseGrid, tr( "Baseline &Grid:" ),groupBox4a, "textLabel6a" );
	groupBox4aLayout->addWidget( textLabel6a, 0, 0 );
	baseOffset = new MSpinBox( 0, prefsData->PageHoehe * Umrech, groupBox4a, 1 );
	baseOffset->setValue(prefsData->BaseOffs * Umrech);
	groupBox4aLayout->addWidget( baseOffset, 1, 1, Qt::AlignLeft );
	textLabel7a = new QLabel(baseOffset, tr( "Baseline &Offset:" ),groupBox4a, "textLabel7a" );
	groupBox4aLayout->addWidget( textLabel7a, 1, 0 );
	autoLine = new QSpinBox( groupBox4a, "autoLine" );
	autoLine->setMaxValue( 100 );
	autoLine->setMinValue( 1 );
	autoLine->setValue( prefsData->AutoLine );
	autoLine->setSuffix( tr( " %" ) );
	groupBox4aLayout->addWidget( autoLine, 2, 1, Qt::AlignLeft );
	textLabel8a = new QLabel( autoLine, tr( "Automatic &Line Spacing:" ), groupBox4a, "textLabel8a" );
	groupBox4aLayout->addWidget( textLabel8a, 2, 0 );
	tabTypoLayout->addWidget( groupBox4a, 3, 0 );
	addItem( tr("Typography"), loadIcon("font.png"), tabTypo);

	tab_3 = new QWidget( prefsWidgets, "tab_3" );
	tabLayout_4 = new QHBoxLayout( tab_3 );
	tabLayout_4->setSpacing( 0 );
	tabLayout_4->setMargin( 10 );
	ButtonGroup2 = new QButtonGroup( "", tab_3, "ButtonGroup2" );
	ButtonGroup2->setExclusive( true );
	ButtonGroup2->setColumnLayout(0, Qt::Vertical );
	ButtonGroup2->layout()->setSpacing( 0 );
	ButtonGroup2->layout()->setMargin( 0 );
	ButtonGroup2Layout = new QHBoxLayout( ButtonGroup2->layout() );
	ButtonGroup2Layout->setAlignment( Qt::AlignTop );
	ButtonGroup2Layout->setSpacing( 6 );
	ButtonGroup2Layout->setMargin( 5 );
	Layout16 = new QGridLayout;
	Layout16->setSpacing( 6 );
	Layout16->setMargin( 0 );
	TextButton = new QToolButton( ButtonGroup2, "TextButton" );
	TextButton->setText("");
	TextButton->setPixmap(loadIcon("Text.xpm"));
	TextButton->setToggleButton( true );
	TextButton->setOn( true );
	ButtonGroup2->insert( TextButton, -1 );
	Layout16->addWidget( TextButton, 0, 0 );
	BildButton = new QToolButton( ButtonGroup2, "BildButton" );
	BildButton->setText("");
	BildButton->setPixmap(loadIcon("Bild.xpm"));
	BildButton->setToggleButton( true );
	ButtonGroup2->insert( BildButton, -1 );
	Layout16->addWidget( BildButton, 0, 1 );
	RectButton = new QToolButton( ButtonGroup2, "RectButton" );
	RectButton->setText("");
	RectButton->setPixmap(loadIcon("Rechtecke.xpm") );
	RectButton->setToggleButton( true );
	ButtonGroup2->insert( RectButton, -1 );
	Layout16->addWidget( RectButton, 1, 0 );
	OvalButton = new QToolButton( ButtonGroup2, "OvalButton" );
	OvalButton->setText("");
	OvalButton->setPixmap(loadIcon("Kreise.xpm"));
	OvalButton->setToggleButton( true );
	ButtonGroup2->insert( OvalButton, -1 );
	Layout16->addWidget( OvalButton, 1, 1 );
	ZoomButton = new QToolButton( ButtonGroup2, "ZoomButton" );
	ZoomButton->setText("");
	ZoomButton->setPixmap(loadIcon("Lupe.xpm"));
	ZoomButton->setToggleButton( true );
	ButtonGroup2->insert( ZoomButton, -1 );
	Layout16->addWidget( ZoomButton, 2, 0 );
	LineButton = new QToolButton( ButtonGroup2, "LineButton" );
	LineButton->setText("");
	LineButton->setPixmap(loadIcon("Stift.xpm"));
	LineButton->setToggleButton( true );
	ButtonGroup2->insert( LineButton, -1 );
	Layout16->addWidget( LineButton, 2, 1 );
	PolyButton = new QToolButton( ButtonGroup2, "PolyButton" );
	PolyButton->setText("");
	PolyButton->setPixmap(loadIcon("spline.png"));
	PolyButton->setToggleButton( true );
	ButtonGroup2->insert( PolyButton, -1 );
	Layout16->addWidget( PolyButton, 3, 0 );
	ButtonGroup2Layout->addLayout( Layout16 );
	tabLayout_4->addWidget( ButtonGroup2 );

	Fram = new QWidgetStack(tab_3);
	tabLayout_4->addWidget( Fram );

	ToolFrame = new QFrame( this, "ToolFrame" );
	ToolFrame->setFrameShape( QFrame::Box );
	ToolFrame->setFrameShadow( QFrame::Sunken );
	ToolFrameLayout = new QHBoxLayout( ToolFrame );
	ToolFrameLayout->setSpacing( 0 );
	ToolFrameLayout->setMargin( 10 );
	Layout15a = new QGridLayout;
	Layout15a->setSpacing( 6 );
	Layout15a->setMargin( 0 );

	FontComb = new FontCombo(ToolFrame, prefsData);
	for (int fc=0; fc<FontComb->count(); ++fc)
	{
		if (FontComb->text(fc) == prefsData->DefFont)
		{
			FontComb->setCurrentItem(fc);
			break;
		}
	}
	TextLabel1_3 = new QLabel( FontComb, tr( "Default &Font:" ), ToolFrame, "TextLabel1_3" );
	TextLabel1_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabel1_3->sizePolicy().hasHeightForWidth() ) );
	Layout15a->addWidget( TextLabel1_3, 0, 0 );	
	Layout15a->addWidget( FontComb, 0, 1 );
	SizeCombo = new QComboBox( true, ToolFrame, "SizeCombo" );
	SizeCombo->setEditable(false);
	QString ar_sizes[] = {" 7", " 9", "10", "11", "12", "14", "18", "24", "36", "48", "60", "72"};
	size_t f_size = sizeof(ar_sizes) / sizeof(*ar_sizes);
	for (uint s = 0; s < f_size; ++s)
		SizeCombo->insertItem(ar_sizes[s] + tr(" pt"));
	int a;
	for (a = 0; a < SizeCombo->count(); ++a)
	{
		if (SizeCombo->text(a).left(2).toInt() == prefsData->DefSize / 10)
			SizeCombo->setCurrentItem(a);
	}
	TextLabel2_2 = new QLabel( SizeCombo, tr( "Default &Size:" ), ToolFrame, "TextLabel2_2" );
	TextLabel2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabel2_2->sizePolicy().hasHeightForWidth() ) );
	Layout15a->addWidget( TextLabel2_2, 1, 0 );
	Layout15a->addWidget( SizeCombo, 1, 1 );

	ForegroundT = new QComboBox( true, ToolFrame, "Foreground" );
	ForegroundT->setEditable(false);
	QPixmap pm2;
	pm2 = QPixmap(15, 15);
	CListe::Iterator itc;
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pm2.fill(prefsData->DColors[itc.key()].getRGBColor());
		ForegroundT->insertItem(pm2, itc.key());
		if (itc.key() == prefsData->DpenText)
			ForegroundT->setCurrentItem(ForegroundT->count()-1);
	}
	TextLabelT1 = new QLabel( ForegroundT, tr( "&Text Color:" ), ToolFrame, "TextLabel1_3_2" );
	TextLabelT1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                            TextLabelT1->sizePolicy().hasHeightForWidth() ) );
	Layout15a->addWidget( TextLabelT1, 2, 0 );	
	Layout15a->addWidget( ForegroundT, 2, 1 );
	
	TextColVal = new QSpinBox( ToolFrame, "TextColVal" );
	TextColVal->setMaxValue( 100 );
	TextColVal->setMinValue( 1 );
	TextColVal->setValue(prefsData->DCols );
	TextCol = new QLabel(TextColVal, tr("Colu&mns:"), ToolFrame, "TextCol");
	Layout15a->addWidget( TextCol, 3, 0 );
	Layout15a->addWidget( TextColVal, 3, 1 );
	
	TextGapVal = new MSpinBox( 0, 200, ToolFrame, decimals );
	TextGapVal->setSuffix( tr( " pt" ) );
	TextGapVal->setValue(prefsData->DGap);
	TextGap = new QLabel(TextGapVal, tr("&Gap:"), ToolFrame, "TextCol");
	Layout15a->addWidget( TextGap, 4, 0 );
	Layout15a->addWidget( TextGapVal, 4, 1 );
	
	TextLabel1_4 = new QLabel( tr( "Woven silk pyjamas exchanged for blue quartz" ), ToolFrame, "TextLabel1_4" );
	TextLabel1_4->setMinimumSize(QSize(260, 70));
	TextLabel1_4->setMaximumSize(QSize(260, 70));
	TextLabel1_4->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	SetSample();
	Layout15a->addMultiCellWidget( TextLabel1_4, 5, 5, 0, 1 );
	QSpacerItem* sp09 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrameLayout->addItem( sp09 );
	ToolFrameLayout->addLayout( Layout15a );
	QSpacerItem* sp10 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrameLayout->addItem( sp10 );
	Fram->addWidget(ToolFrame, 1);

	ToolFrame2 = new QFrame( this, "ToolFrame" );
	ToolFrame2->setFrameShape( QFrame::Box );
	ToolFrame2->setFrameShadow( QFrame::Sunken );
	ToolFrame2Layout = new QHBoxLayout( ToolFrame2 );
	ToolFrame2Layout->setSpacing( 0 );
	ToolFrame2Layout->setMargin( 10 );
	Layout15b = new QGridLayout;
	Layout15b->setSpacing( 6 );
	Layout15b->setMargin( 0 );
	
	Foreground = new QComboBox( true, ToolFrame2, "Foreground" );
	Foreground->setEditable(false);
	Foreground->insertItem( tr("None"));
	if (prefsData->Dpen == "None")
		Foreground->setCurrentItem(Foreground->count()-1);
	pm2 = QPixmap(15, 15);
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pm2.fill(prefsData->DColors[itc.key()].getRGBColor());
		Foreground->insertItem(pm2, itc.key());
		if (itc.key() == prefsData->Dpen)
			Foreground->setCurrentItem(Foreground->count()-1);
	}
	TextLabelT21 = new QLabel( Foreground, tr( "&Line Color:" ), ToolFrame2, "TextLabel1_3_2" );
	TextLabelT21->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT21->sizePolicy().hasHeightForWidth() ) );
	Layout15b->addWidget(TextLabelT21, 0, 0);
	Layout15b->addWidget(Foreground, 0, 1);
	
	Shade2 = new QSpinBox( ToolFrame2, "Shade2" );
	Shade2->setSuffix( tr( " %" ) );
	Shade2->setMaxValue( 100 );
	Shade2->setMinValue( 0 );
	Shade2->setValue(prefsData->Dshade2 );
	TextLabelT22 = new QLabel( Shade2, tr( "&Shading:" ), ToolFrame2, "TextLabel2_2" );
	TextLabelT22->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT22->sizePolicy().hasHeightForWidth() ) );
	Layout15b->addWidget(TextLabelT22, 1, 0);
	Layout15b->addWidget(Shade2, 1, 1);
	
	Background = new QComboBox( true, ToolFrame2, "Background" );
	Background->setEditable(false);
	Background->insertItem( tr("None"));
	if (prefsData->Dbrush == "None")
		Background->setCurrentItem(Background->count()-1);
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pm2.fill(prefsData->DColors[itc.key()].getRGBColor());
		Background->insertItem(pm2, itc.key());
		if (itc.key() == prefsData->Dbrush)
			Background->setCurrentItem(Background->count()-1);
	}
	TextLabelT23 = new QLabel( Background, tr( "&Fill Color:" ), ToolFrame2, "TextLabel1_3" );
	TextLabelT23->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT23->sizePolicy().hasHeightForWidth() ) );
	Layout15b->addWidget(TextLabelT23, 2, 0);
	Layout15b->addWidget(Background, 2, 1);
	
	Shade = new QSpinBox( ToolFrame2, "Shade" );
	Shade->setSuffix( tr( " %" ) );
	Shade->setMaxValue( 100 );
	Shade->setMinValue( 0 );
	Shade->setValue(prefsData->Dshade );
	TextLabelT24 = new QLabel( Shade, tr( "S&hading:" ), ToolFrame2, "TextLabel2_2_2" );
	TextLabelT24->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT24->sizePolicy().hasHeightForWidth() ) );
	Layout15b->addWidget(TextLabelT24, 3, 0);
	Layout15b->addWidget(Shade, 3, 1);
	
	Linestyle = new LineCombo(ToolFrame2);
	Linestyle->setEditable(false);
	switch (prefsData->DLineArt)
	{
	case SolidLine:
		Linestyle->setCurrentItem(0);
		break;
	case DashLine:
		Linestyle->setCurrentItem(1);
		break;
	case DotLine:
		Linestyle->setCurrentItem(2);
		break;
	case DashDotLine:
		Linestyle->setCurrentItem(3);
		break;
	case DashDotDotLine:
		Linestyle->setCurrentItem(4);
		break;
	default:
		Linestyle->setCurrentItem(0);
		break;
	}
	TextLabelT25 = new QLabel( Linestyle, tr( "&Type of Line:" ), ToolFrame2, "TextLabel1_3_3" );
	TextLabelT25->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT25->sizePolicy().hasHeightForWidth() ) );
	Layout15b->addWidget(TextLabelT25, 4, 0);
	Layout15b->addWidget(Linestyle, 4, 1);
	
	LineW = new MSpinBox( 0, 36, ToolFrame2, 1 );
	LineW->setSuffix( tr( " pt" ) );
	LineW->setValue(prefsData->Dwidth);
	TextLabelT26 = new QLabel( LineW, tr( "Line &Width:" ), ToolFrame2, "TextLabel2_3_4" );
	TextLabelT26->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT26->sizePolicy().hasHeightForWidth() ) );
	Layout15b->addWidget(TextLabelT26, 5, 0);
	
	Layout15b->addWidget(LineW, 5, 1);
	
	QSpacerItem* sp07 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrame2Layout->addItem( sp07 );
	ToolFrame2Layout->addLayout( Layout15b );
	QSpacerItem* sp08 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrame2Layout->addItem( sp08 );
	Fram->addWidget(ToolFrame2, 2);

	ToolFrame3 = new QFrame( this, "ToolFrame" );
	ToolFrame3->setFrameShape( QFrame::Box );
	ToolFrame3->setFrameShadow( QFrame::Sunken );
	ToolFrame3Layout = new QHBoxLayout( ToolFrame3 );
	ToolFrame3Layout->setSpacing( 0 );
	ToolFrame3Layout->setMargin( 10 );
	Layout15c = new QGridLayout;
	Layout15c->setSpacing( 6 );
	Layout15c->setMargin( 0 );

	Foreground2 = new QComboBox( true, ToolFrame3, "Foreground" );
	Foreground2->setEditable(false);
	pm3 = QPixmap(15, 15);
	CListe::Iterator itc2;
	for (itc2 = prefsData->DColors.begin(); itc2 != prefsData->DColors.end(); ++itc2)
	{
		pm3.fill(prefsData->DColors[itc2.key()].getRGBColor());
		Foreground2->insertItem(pm3, itc2.key());
		if (itc2.key() == prefsData->DpenLine)
			Foreground2->setCurrentItem(Foreground2->count()-1);
	}
	TextLabelT31 = new QLabel( Foreground2, tr( "&Line Color:" ), ToolFrame3, "TextLabel1_3_2" );
	TextLabelT31->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT31->sizePolicy().hasHeightForWidth() ) );
	Layout15c->addWidget(TextLabelT31, 0, 0);	
	Layout15c->addWidget(Foreground2, 0, 1);
	Shade22 = new QSpinBox( ToolFrame3, "Shade2" );
	Shade22->setSuffix( tr( " %" ) );
	Shade22->setMaxValue( 100 );
	Shade22->setMinValue( 0 );
	Shade22->setValue(prefsData->DshadeLine );
	TextLabelT32 = new QLabel( Shade22, tr( "&Shading:" ), ToolFrame3, "TextLabel2_2" );
	TextLabelT32->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT32->sizePolicy().hasHeightForWidth() ) );
	Layout15c->addWidget(TextLabelT32, 1, 0);	
	Layout15c->addWidget(Shade22, 1, 1);
	Linestyle2 = new LineCombo(ToolFrame3);
	Linestyle2->setEditable(false);
	int dla = prefsData->DLstyleLine;
	switch (dla)
	{
	case SolidLine:
		Linestyle2->setCurrentItem(0);
		break;
	case DashLine:
		Linestyle2->setCurrentItem(1);
		break;
	case DotLine:
		Linestyle2->setCurrentItem(2);
		break;
	case DashDotLine:
		Linestyle2->setCurrentItem(3);
		break;
	case DashDotDotLine:
		Linestyle2->setCurrentItem(4);
		break;
	default:
		Linestyle2->setCurrentItem(0);
		break;
	}
	TextLabelT35 = new QLabel( Linestyle2, tr( "&Type of Line:" ), ToolFrame3, "TextLabel1_3_3" );
	TextLabelT35->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT35->sizePolicy().hasHeightForWidth() ) );
	Layout15c->addWidget(TextLabelT35, 2, 0);
	Layout15c->addWidget(Linestyle2, 2, 1);
	
	LineW2 = new MSpinBox( 1, 36, ToolFrame3, 1 );
	LineW2->setSuffix( tr( " pt" ) );
	LineW2->setValue(prefsData->DwidthLine);
	TextLabelT36 = new QLabel( LineW2, tr( "Line &Width:" ), ToolFrame3, "TextLabel2_3_4" );
	TextLabelT36->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT36->sizePolicy().hasHeightForWidth() ) );
	Layout15c->addWidget(TextLabelT36, 3, 0);	
	Layout15c->addWidget(LineW2, 3, 1);
	QSpacerItem* sp05 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrame3Layout->addItem( sp05 );
	ToolFrame3Layout->addLayout( Layout15c );
	QSpacerItem* sp06 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrame3Layout->addItem( sp06 );
	Fram->addWidget(ToolFrame3, 3);

	ToolFrame4 = new QFrame( this, "ToolFrame" );
	ToolFrame4->setFrameShape( QFrame::Box );
	ToolFrame4->setFrameShadow( QFrame::Sunken );
	ToolFrame4Layout = new QHBoxLayout( ToolFrame4 );
	ToolFrame4Layout->setAlignment( Qt::AlignTop );
	ToolFrame4Layout->setSpacing( 0 );
	ToolFrame4Layout->setMargin( 10 );
	Layout15d = new QGridLayout;
	Layout15d->setSpacing( 6 );
	Layout15d->setMargin( 0 );
	
	MinMag = new QSpinBox( ToolFrame4, "Shade2" );
	MinMag->setSuffix( tr( " %" ) );
	MinMag->setMaxValue( 800 );
	MinMag->setMinValue( 10 );
	MinMag->setLineStep( 10 );
	MinMag->setValue(prefsData->MagMin );
	TextLabelT42 = new QLabel( MinMag, tr( "Mi&nimum:" ), ToolFrame4, "TextLabel2_2" );
	TextLabelT42->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT42->sizePolicy().hasHeightForWidth() ) );
	Layout15d->addWidget(TextLabelT42, 0, 0);
	Layout15d->addWidget(MinMag, 0, 1);
	
	MaxMag = new QSpinBox( ToolFrame4, "Shade" );
	MaxMag->setSuffix( tr( " %" ) );
	MaxMag->setMaxValue( 800 );
	MaxMag->setMinValue( 10 );
	MaxMag->setLineStep( 10 );
	MaxMag->setValue(prefsData->MagMax );
	TextLabelT43 = new QLabel( MaxMag, tr( "Ma&ximum:" ), ToolFrame4, "TextLabel2_2_3" );
	TextLabelT43->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT43->sizePolicy().hasHeightForWidth() ) );
	Layout15d->addWidget(TextLabelT43, 1, 0);
	Layout15d->addWidget(MaxMag, 1, 1);
	
	StepMag = new QSpinBox( ToolFrame4, "LineW" );
	StepMag->setSuffix( tr( " %" ) );
	StepMag->setMaxValue( 200 );
	StepMag->setMinValue( 1 );
	StepMag->setLineStep( 25 );
	StepMag->setValue(prefsData->MagStep );
	TextLabelT44 = new QLabel( StepMag, tr( "&Stepping:" ), ToolFrame4, "TextLabel2_3_4" );
	TextLabelT44->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT44->sizePolicy().hasHeightForWidth() ) );
	Layout15d->addWidget(TextLabelT44, 2, 0);	
	Layout15d->addWidget(StepMag, 2, 1);
	
	QSpacerItem* sp03 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrame4Layout->addItem( sp03 );
	ToolFrame4Layout->addLayout( Layout15d );
	QSpacerItem* sp04 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrame4Layout->addItem( sp04 );
	Fram->addWidget(ToolFrame4, 4);

	ToolFrame5 = new QButtonGroup( this, "ToolFrame5" );
	ToolFrame5->setFrameShape( QFrame::Box );
	ToolFrame5->setFrameShadow( QFrame::Sunken );
	ToolFrame5->setExclusive( false );
	ToolFrame5->setRadioButtonExclusive(true);
	ToolFrame5Layout = new QHBoxLayout( ToolFrame5 );
	ToolFrame5Layout->setSpacing( 0 );
	ToolFrame5Layout->setMargin( 20 );
	Layout15 = new QGridLayout;
	Layout15->setSpacing( 6 );
	Layout15->setMargin( 0 );
	FreeScale = new QRadioButton( tr( "&Free Scaling" ), ToolFrame5, "FreeScale" );
	Layout15->addMultiCellWidget( FreeScale, 0, 0, 0, 3 );

	XScale = new QSpinBox( ToolFrame5, "XScale" );
	XScale->setSuffix( tr( " %" ) );
	XScale->setMaxValue( 1000 );
	XScale->setMinValue( 1 );
	XScale->setValue(qRound(prefsData->ScaleX * 100) );
	TextLabelT51 = new QLabel( XScale, tr( "&Horizontal Scaling:" ), ToolFrame5, "TextLabelT51" );
	TextLabelT51->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabelT51->sizePolicy().hasHeightForWidth() ) );
	Layout15->addWidget( TextLabelT51, 1, 1 );	
	Layout15->addWidget( XScale, 1, 2 );
	YScale = new QSpinBox( ToolFrame5, "YScale" );
	YScale->setSuffix( tr( " %" ) );
	YScale->setMaxValue( 1000 );
	YScale->setMinValue( 1 );
	YScale->setValue(qRound(prefsData->ScaleY * 100) );
	TextLabelT52 = new QLabel( YScale, tr( "&Vertical Scaling:" ), ToolFrame5, "TextLabelT52" );
	TextLabelT52->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabelT52->sizePolicy().hasHeightForWidth() ) );
	Layout15->addWidget( TextLabelT52, 2, 1 );	
	Layout15->addWidget( YScale, 2, 2 );
	Kette = new LinkButton( ToolFrame5 );
	Kette->setToggleButton( true );
	Kette->setAutoRaise(true);
	Layout15->addMultiCellWidget( Kette, 1, 2, 3, 3 );
	FrameScale = new QRadioButton(tr( "&Scale Picture to Frame Size" ), ToolFrame5, "FrameScale" );
	Layout15->addMultiCellWidget( FrameScale, 3, 3, 0, 3 );
	Aspect = new QCheckBox( tr( "Keep Aspect &Ratio" ), ToolFrame5, "Aspect" );
	Layout15->addMultiCellWidget( Aspect, 4, 4, 1, 3 );
	ShadeP = new QSpinBox( ToolFrame5, "ShadeP" );
	ShadeP->setSuffix( tr( " %" ) );
	ShadeP->setMaxValue( 100 );
	ShadeP->setMinValue( 0 );
	ShadeP->setValue(prefsData->ShadePict );
	TextLabelT54 = new QLabel( ShadeP, tr( "S&hading:" ), ToolFrame5, "TextLabelT54" );
	TextLabelT54->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT54->sizePolicy().hasHeightForWidth() ) );
	Layout15->addWidget( TextLabelT54, 6, 0 );	
	Layout15->addWidget( ShadeP, 6, 1 );
	BackgroundP = new QComboBox( true, ToolFrame5, "BackgroundP" );
	BackgroundP->setEditable(false);
	BackgroundP->insertItem( tr("None"));
	if (prefsData->DbrushPict == "None")
		BackgroundP->setCurrentItem(BackgroundP->count()-1);
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pm2.fill(prefsData->DColors[itc.key()].getRGBColor());
		BackgroundP->insertItem(pm2, itc.key());
		if (itc.key() == prefsData->DbrushPict)
			BackgroundP->setCurrentItem(BackgroundP->count()-1);
	}
	TextLabelT53 = new QLabel( BackgroundP, tr( "F&ill Color:" ), ToolFrame5, "TextLabelT53" );
	TextLabelT53->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabelT53->sizePolicy().hasHeightForWidth() ) );
	Layout15->addWidget( TextLabelT53, 5, 0 );
	Layout15->addMultiCellWidget( BackgroundP, 5, 5, 1, 3 );
	QSpacerItem* sp01 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrame5Layout->addItem( sp01 );
	ToolFrame5Layout->addLayout( Layout15 );
	QSpacerItem* sp02 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ToolFrame5Layout->addItem( sp02 );
	Aspect->setChecked(prefsData->AspectRatio);
	FreeScale->setChecked(prefsData->ScaleType ? true : false);
	FrameScale->setChecked(prefsData->ScaleType ? false : true);
	ChangeScaling();
	Fram->addWidget(ToolFrame5, 5);

	ToolFrame6 = new QFrame( this, "ToolFrame6" );
	ToolFrame6->setFrameShape( QFrame::Box );
	ToolFrame6->setFrameShadow( QFrame::Sunken );
	PolygonPropsLayout = new QHBoxLayout( ToolFrame6, 10, 5, "PolygonPropsLayout");
	PolygonPropsLayout->setAlignment( Qt::AlignTop );
	polyWidget = new PolygonWidget(ToolFrame6, prefsData->PolyC, prefsData->PolyFd, prefsData->PolyF, prefsData->PolyS, prefsData->PolyR);
	PolygonPropsLayout->addWidget( polyWidget );
	QSpacerItem* T6_spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	PolygonPropsLayout->addItem( T6_spacer2 );
	Fram->addWidget(ToolFrame6, 6);
	addItem( tr("Tools"), loadIcon("tools.png"), tab_3);

	tab_5 = new QWidget( prefsWidgets, "tab_5" );
	tabLayout_5 = new QGridLayout( tab_5 );
	tabLayout_5->setSpacing( 6 );
	tabLayout_5->setMargin( 11 );

	Saving = new QGroupBox( tr( "Other Options" ), tab_5, "Saving" );
	Saving->setColumnLayout(0, Qt::Vertical );
	Saving->layout()->setSpacing( 0 );
	Saving->layout()->setMargin( 0 );
	SavingLayout = new QHBoxLayout( Saving->layout() );
	SavingLayout->setAlignment( Qt::AlignTop );
	SavingLayout->setSpacing( 6 );
	SavingLayout->setMargin( 25 );

	SaveAtQuit = new QCheckBox( tr( "Sa&ve Contents on Changes" ), Saving, "SaveAtQuit" );
	SaveAtQuit->setChecked(prefsData->SaveAtQ);
	SavingLayout->addWidget( SaveAtQuit );

	tabLayout_5->addWidget( Saving, 1, 0 );

	Preview = new QGroupBox( tr( "Preview" ), tab_5, "Preview" );
	Preview->setColumnLayout(0, Qt::Vertical );
	Preview->layout()->setSpacing( 0 );
	Preview->layout()->setMargin( 0 );
	PreviewLayout = new QHBoxLayout( Preview->layout() );
	PreviewLayout->setAlignment( Qt::AlignTop );
	PreviewLayout->setSpacing( 10 );
	PreviewLayout->setMargin( 24 );

	PreviewSize = new QComboBox( true, Preview, "PreviewSize" );
	PreviewSize->insertItem( tr( "Small" ) );
	PreviewSize->insertItem( tr( "Medium" ) );
	PreviewSize->insertItem( tr( "Large" ) );
	PreviewSize->setEditable(false);
	int sci = prefsData->PSize == 40 ? 0 : prefsData->PSize == 60 ? 1 : 2;
	PreviewSize->setCurrentItem(sci);
	TextLabelP = new QLabel( PreviewSize, tr( "&Size:" ), Preview, "TextLabel1_3" );
	PreviewLayout->addWidget( TextLabelP );
	PreviewLayout->addWidget( PreviewSize );

	tabLayout_5->addWidget( Preview, 0, 0 );

	Frame6 = new QFrame( tab_5, "Frame6" );
	Frame6->setFrameShape( QFrame::NoFrame );
	Frame6->setFrameShadow( QFrame::Plain );
	Frame6->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

	tabLayout_5->addMultiCellWidget( Frame6, 0, 1, 1, 1 );
	addItem(  tr("Scrapbook"), loadIcon("scrap.png"), tab_5);

	tabView = new QWidget( prefsWidgets, "tabView" );
	tabViewLayout = new QVBoxLayout( tabView, 10, 5, "tabViewLayout");
	tabViewLayout->setAlignment( Qt::AlignTop );
	pageBackground = new QButtonGroup( tabView, "pageBackground" );
	pageBackground->setTitle( tr( "Page Background" ) );
	pageBackground->setColumnLayout(0, Qt::Vertical );
	pageBackground->layout()->setSpacing( 5 );
	pageBackground->layout()->setMargin( 10 );
	pageBackgroundLayout = new QHBoxLayout( pageBackground->layout() );
	pageBackgroundLayout->setAlignment( Qt::AlignTop );
	textLabel9 = new QLabel( pageBackground, "textLabel9" );
	textLabel9->setText( tr( "Color:" ) );
	pageBackgroundLayout->addWidget( textLabel9 );
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
	pageBackgroundLayout->addWidget( backColor );
	checkUnprintable = new QCheckBox( pageBackground, "checkUnprintable" );
	checkUnprintable->setText( tr( "Display &Unprintable Area in Margin Color" ) );
	checkUnprintable->setAccel( QKeySequence( tr( "Alt+U" ) ) );
	checkUnprintable->setChecked( prefsData->RandFarbig );
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

	buttonGroup1 = new QButtonGroup( tabView, "buttonGroup1" );
	buttonGroup1->setExclusive( true );
	buttonGroup1->setColumnLayout(0, Qt::Vertical );
	buttonGroup1->layout()->setSpacing( 5 );
	buttonGroup1->layout()->setMargin( 10 );
	buttonGroup1->setTitle( tr( "Placing of Guides" ) );
	buttonGroup1Layout = new QHBoxLayout( buttonGroup1->layout() );
	buttonGroup1Layout->setAlignment( Qt::AlignTop );
	inBackground = new QRadioButton( buttonGroup1, "inBackground" );
	inBackground->setText( tr( "In the Background" ) );
	buttonGroup1Layout->addWidget( inBackground );
	inForeground = new QRadioButton( buttonGroup1, "inForeground" );
	inForeground->setText( tr( "In the Foreground" ) );
	buttonGroup1Layout->addWidget( inForeground );
	inBackground->setChecked( prefsData->Before );
	inForeground->setChecked( !prefsData->Before );
	tabViewLayout->addWidget( buttonGroup1 );
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
	DrawRuler();
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

	Fram->raiseWidget(1);
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
	QToolTip::add( Breite, tr( "Width of document pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( Hoehe, tr( "Height of document pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( Doppelseiten, tr( "Enable single or spread based layout" ) );
	QToolTip::add( Linkszuerst, tr( "Make the first page the left page of a document" ) );
	QToolTip::add( TopR, tr( "Distance between the top margin guide and the edge of the page" ) );
	QToolTip::add( BottomR, tr( "Distance between the bottom margin guide and the edge of the page" ) );
	QToolTip::add( LeftR, tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( RightR, tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( ASon, tr( "When enabled, Scribus saves a backup copy of your file with the .bak extension\neach time the time period elapses" ) );
	QToolTip::add( ASTime, tr( "Time period between saving automatically" ) );

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
	QToolTip::add( inBackground, tr( "Place the grid behind your page objects" ) );
	QToolTip::add( inForeground, tr( "Place the grid in front of your page objects" ) );

	QToolTip::add( superDisplacement, tr( "Displacement above the baseline of the font on a line" ) );
	QToolTip::add( superScaling, tr( "Relative size of the superscript compared to the normal font" ) );
	QToolTip::add( subDisplacement, tr( "Displacement below the baseline of the normal font on a line" ) );
	QToolTip::add( subScaling, tr( "Relative size of the subscript compared to the normal font" ) );
	QToolTip::add( capsScaling, tr( "Relative size of the small caps font compared to the normal font" ) );
	QToolTip::add( autoLine, tr( "Percentage increase over the font size for the line spacing" ) );

	QToolTip::add( TextButton, tr( "Text Frame Properties" ) );
	QToolTip::add( BildButton, tr( "Picture Frame Properties" ) );
	QToolTip::add( RectButton, tr( "Shape Drawing Properties" ) );
	QToolTip::add( OvalButton, tr( "Shape Drawing Properties" ) );
	QToolTip::add( ZoomButton, tr( "Magnification Level Defaults" ) );
	QToolTip::add( LineButton, tr( "Line Drawing Properties" ) );
	QToolTip::add( PolyButton, tr( "Polygon Drawing Properties" ) );

	QToolTip::add( FontComb, tr( "Font for new text frames" ) );
	QToolTip::add( SizeCombo, tr( "Size of font for new text frames" ) );
	QToolTip::add( ForegroundT, tr( "Color of font" ) );
	QToolTip::add( TextColVal, tr( "Number of columns in a text frame" ) );
	QToolTip::add( TextGapVal, tr( "Gap between text frame columns" ) );
	QToolTip::add( TextLabel1_4, tr( "Sample of your font" ) );


	QToolTip::add( FreeScale, tr( "Picture frames allow pictures to scale to any size" ) );
	QToolTip::add( XScale, tr( "Horizontal scaling of images" ) );
	QToolTip::add( YScale, tr( "Vertical scaling of images" ) );
	QToolTip::add( Kette, tr( "Keep horizontal and vertical scaling the same" ) );
	QToolTip::add( FrameScale, tr( "Pictures in picture frames are scaled to the size of the frame" ) );
	QToolTip::add( Aspect, tr( "Automatically scaled pictures keep their original proportions" ) );
	QToolTip::add( BackgroundP, tr( "Fill color of picture frames" ) );
	QToolTip::add( ShadeP, tr( "Saturation of color of fill" ) );

	QToolTip::add( Foreground, tr( "Line color of shapes" ) );
	QToolTip::add( Shade2, tr( "Saturation of color of lines" ) );
	QToolTip::add( Background, tr( "Fill color of shapes" ) );
	QToolTip::add( Shade, tr( "Saturation of color of fill" ) );
	QToolTip::add( Linestyle, tr( "Line style of shapes" ) );
	QToolTip::add( LineW, tr( "Line width of shapes" ) );

	QToolTip::add( MinMag, tr( "Minimum magnification allowed" ) );
	QToolTip::add( MaxMag, tr( "Maximum magnification allowed" ) );
	QToolTip::add( StepMag, tr( "Change in magnification for each zoom operation" ) );

	QToolTip::add( Foreground2, tr( "Color of lines" ) );
	QToolTip::add( Shade22, tr( "Saturation of color" ) );
	QToolTip::add( Linestyle2, tr( "Style of lines" ) );
	QToolTip::add( LineW2, tr( "Width of lines" ) );

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
	connect(majorGridColor, SIGNAL(clicked()), this, SLOT(changeMajorColor()));
	connect(minorGridColor, SIGNAL(clicked()), this, SLOT(changeMinorColor()));
	connect(baselineColor, SIGNAL(clicked()), this, SLOT(changeBaselineColor()));
	connect(guideColor, SIGNAL(clicked()), this, SLOT(changeGuideColor()));
	connect(marginColor, SIGNAL(clicked()), this, SLOT(changeMarginColor()));
	connect(backColor, SIGNAL(clicked()), this, SLOT(changePaperColor()));
	connect(FontComb, SIGNAL(activated(int)), this, SLOT(SetSample()));
	connect(SizeCombo, SIGNAL(activated(int)), this, SLOT(SetSample()));
	connect(TextButton, SIGNAL(clicked()), this, SLOT(SetTool()));
	connect(RectButton, SIGNAL(clicked()), this, SLOT(SetTool()));
	connect(OvalButton, SIGNAL(clicked()), this, SLOT(SetTool()));
	connect(LineButton, SIGNAL(clicked()), this, SLOT(SetTool()));
	connect(ZoomButton, SIGNAL(clicked()), this, SLOT(SetTool()));
	connect(BildButton, SIGNAL(clicked()), this, SLOT(SetTool()));
	connect(PolyButton, SIGNAL(clicked()), this, SLOT(SetTool()));
	connect(Kette, SIGNAL(clicked()), this, SLOT(ToggleKette()));
	connect(XScale, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
	connect(YScale, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
	connect(UnitCombo, SIGNAL(activated(int)), this, SLOT(UnitChange()));
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	connect(GZComboO, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(GZComboF, SIGNAL(activated(int)), this, SLOT(setSize(int)));
	connect(Doppelseiten, SIGNAL(clicked()), this, SLOT(setDS()));
	connect(FreeScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
	connect(FrameScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
	connect(FileC, SIGNAL(clicked()), this, SLOT(ChangeDocs()));
	connect(FileC2, SIGNAL(clicked()), this, SLOT(ChangeProfs()));
	connect(FileC3, SIGNAL(clicked()), this, SLOT(ChangeScripts()));
	connect(FileC4, SIGNAL(clicked()), this, SLOT(ChangeTemplates()));
	connect(CaliSlider, SIGNAL(valueChanged(int)), this, SLOT(SetDisScale()));
	setSize(prefsData->PageFormat);
	setOrien(prefsData->Ausrichtung);
	Breite->setValue(prefsData->PageBreite * Umrech);
	Hoehe->setValue(prefsData->PageHoehe * Umrech);
	UnitChange();
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
void Preferences::ChangeDocs()
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
void Preferences::ChangeProfs()
{
	QString s = QFileDialog::getExistingDirectory(Docs->text(), this, "d", tr("Choose a Directory"), true);
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
void Preferences::ChangeScripts()
{
	QString s = QFileDialog::getExistingDirectory(Docs->text(), this, "d", tr("Choose a Directory"), true);
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
void Preferences::ChangeTemplates()
{
	QString s = QFileDialog::getExistingDirectory(Docs->text(), this, "d", tr("Choose a Directory"), true);
	if (s != "")
		TemplateDir->setText(s);
}

/*!
 \fn void Preferences::ChangeScaling()
 \author Franz Schmid 
 \date  
 \brief Preferences (Tools / Image Frame), switches default between free or set scaling and their settings
 \param None
 \retval None 
 */
void Preferences::ChangeScaling()
{
	bool setter = FreeScale->isChecked() ? true : false;
	TextLabelT51->setEnabled(setter);
	TextLabelT52->setEnabled(setter);
	XScale->setEnabled(setter);
	YScale->setEnabled(setter);
	Kette->setEnabled(setter);
	Aspect->setEnabled(!setter);
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
	bool m = Doppelseiten->isChecked() ? true : false;
	GRText3->setText(m == true ? tr( "&Inside:" ) : tr( "&Left:" ));
	GRText4->setText(m == true ? tr( "O&utside:" ) : tr( "&Right:" ));
	Linkszuerst->setEnabled(m);
	if (m == false)
		Linkszuerst->setChecked(false);
}

/*!
 \fn void Preferences::setBreite(int v)
 \author Franz Schmid 
 \date  
 \brief Preferences (Document / Page Size), sets Page width values
 \param v Width value
 \retval None 
 */
void Preferences::setBreite(int)
{
	Pagebr = Breite->value() / Umrech;
	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
}

/*!
 \fn void Preferences::setHoehe(int v)
 \author Franz Schmid 
 \date  
 \brief Preferences (Document / Page Size), sets Page height values
 \param v Height value
 \retval None 
 */
void Preferences::setHoehe(int)
{
	Pageho = Hoehe->value() / Umrech;
	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
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
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
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
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
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
	RightR->setMaxValue(Breite->value() - LeftR->value());
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
	LeftR->setMaxValue(Breite->value() - RightR->value());
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
	Pagebr = Breite->value() / Umrech;
	Pageho = Hoehe->value() / Umrech;
	Breite->setEnabled(false);
	Hoehe->setEnabled(false);
	int br[] = {2380, 1684, 1190, 842, 595, 421, 297, 210, 148, 105, 2836, 2004, 1418, 1002, 709, 501, 355,
	            250, 178, 125, 89, 462, 298, 312, 542, 595, 1224, 612, 612, 792};
	int ho[] = {3368, 2380, 1684, 1190, 842, 595, 421, 297, 210, 148, 4008, 2836, 2004, 1418, 1002, 709, 501,
	            355, 250, 178, 125, 649, 683, 624, 720, 935, 792, 1008, 792, 1224};
	if (gr == 30)
	{
		Breite->setEnabled(true);
		Hoehe->setEnabled(true);
	}
	else
	{
		Pagebr = br[gr];
		Pageho = ho[gr];
	}
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	Breite->setValue(Pagebr * Umrech);
	Hoehe->setValue(Pageho * Umrech);
	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
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
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	if (ori == 0)
	{
		if (GZComboF->currentItem() == 30)
		{
			br = Breite->value();
			Breite->setValue(Hoehe->value());
			Hoehe->setValue(br);
		}
	}
	else
	{
		br = Breite->value();
		Breite->setValue(Hoehe->value());
		Hoehe->setValue(br);
	}
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
}

/*!
 \fn void Preferences::SetSample()
 \author Franz Schmid 
 \date  
 \brief Preferences (Tools, Text frame), Sets the sample text in selected font in text frame preferences
 \param None
 \retval None 
 */
void Preferences::SetSample()
{
	QString ts = tr( "Woven silk pyjamas exchanged for blue quartz" );
	QString da = (*fon)[FontComb->currentText()]->Datei;
	int s = SizeCombo->currentText().left(2).toInt();
	QPixmap pm = fontSamples(da, s, ts, paletteBackgroundColor());
	TextLabel1_4->setPixmap(pm);
}

/*!
 \fn void Preferences::changeMaColor()
 \author Franz Schmid 
 \date  
 \brief Preferences ([dox?], [dox?]), Sets Maximum colour [dox?]
 \param None
 \retval None 
 */

void Preferences::changeMajorColor()
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

/*!
 \fn void Preferences::changeMicolor()
 \author Franz Schmid 
 \date  
 \brief Preferences ([dox?], [dox?]), Sets Minimum colour [dox?]
 \param None
 \retval None 
 */

void Preferences::changeMinorColor()
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
 \fn void Preferences::changeRandColor()
 \author Franz Schmid 
 \date  
 \brief Preferences ([dox?], [dox?]), Sets Rand colour [dox?]
 \param None
 \retval None 
 */
void Preferences::changeMarginColor()
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

/*!
 \fn void Preferences::changeGuideColor()
 \author Franz Schmid 
 \date  
 \brief Preferences ([dox?], [dox?]), Sets Guide colour [dox?]
 \param None
 \retval None 
 */
void Preferences::changeGuideColor()
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

void Preferences::changeBaselineColor()
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

/*!
 \fn void Preferences::SetTool()
 \author Franz Schmid 
 \date  
 \brief Preferences (Tools), Raises widget for selected Tool properties
 \param None
 \retval None 
 */
void Preferences::SetTool()
{
	if (TextButton == sender())
		Fram->raiseWidget(1);
	if (RectButton == sender())
		Fram->raiseWidget(2);
	if (OvalButton == sender())
		Fram->raiseWidget(2);
	if (LineButton == sender())
		Fram->raiseWidget(3);
	if (ZoomButton == sender())
		Fram->raiseWidget(4);
	if (BildButton == sender())
		Fram->raiseWidget(5);
	if (PolyButton == sender())
		Fram->raiseWidget(6);
}

/*!
 \fn void Preferences::ToggleKette()
 \author Franz Schmid 
 \date  
 \brief Preferences (Tools, Image Frame), Sets Y Scale value from X Scale value when chain is toggled
 \param None
 \retval None 
 */
void Preferences::ToggleKette()
{
	if (Kette->isOn())
		YScale->setValue(XScale->value());
}

/*!
 \fn void Preferences::HChange()
 \author Franz Schmid 
 \date  
 \brief Preferences (Tools, Image Frame), Sets Y Scale value from X Scale value when X Scale value is changed
 \param None
 \retval None 
 */
void Preferences::HChange()
{
	if (Kette->isOn())
		YScale->setValue(XScale->value());
}

/*!
 \fn void Preferences::VChange()
 \author Franz Schmid 
 \date  
 \brief Preferences (Tools, Image Frame). Sets X Scale value from Y Scale value when Y Scale value is changed
 \param None
 \retval None 
 */
void Preferences::VChange()
{
	if (Kette->isOn())
		XScale->setValue(YScale->value());
}

/*!
 \fn void Preferences::UnitChange()
 \author Franz Schmid 
 \date  
 \brief Preferences (General, Units). Sets scaling factors and units descriptions when default units are changed. Updates preference values
 \param None
 \retval None 
 */
void Preferences::UnitChange()
{
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	disconnect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	disconnect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	disconnect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	disconnect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	int decimals;
	double AltUmrech = Umrech;
	double oldMin, oldMax, oldTG, oldTGM, oldTGV, oldB, oldBM, oldH, oldHM, val;
	TextGapVal->getValues(&oldTG, &oldTGM, &decimals, &oldTGV);
	oldTG /= AltUmrech;
	oldTGM /= AltUmrech;
	oldTGV /= AltUmrech;
	Breite->getValues(&oldB, &oldBM, &decimals, &val);
	oldB /= AltUmrech;
	oldBM /= AltUmrech;
	Hoehe->getValues(&oldH, &oldHM, &decimals, &val);
	oldH /= AltUmrech;
	oldHM /= AltUmrech;
	QString einh;
	Einheit = UnitCombo->currentItem();
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
	Breite->setSuffix(einh);
	Hoehe->setSuffix(einh);
	TopR->setSuffix(einh);
	BottomR->setSuffix(einh);
	LeftR->setSuffix(einh);
	RightR->setSuffix(einh);
	TextGapVal->setSuffix(einh);
	Breite->setValues(oldB * Umrech, oldBM * Umrech, decimals, Pagebr * Umrech);
	Hoehe->setValues(oldH * Umrech, oldHM * Umrech, decimals, Pageho * Umrech);
	TopR->setValues(0, Hoehe->value() - RandB * Umrech, decimals, RandT * Umrech);
	BottomR->setValues(0, Hoehe->value() - RandT * Umrech, decimals, RandB * Umrech);
	LeftR->setValues(0, Breite->value() - RandR * Umrech, decimals, RandL * Umrech);
	RightR->setValues(0, Breite->value() - RandL * Umrech, decimals, RandR * Umrech);
	TextGapVal->setValues(oldTG * Umrech, oldTGM * Umrech, decimals, oldTGV * Umrech);
	int decimalsOld;
	topScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	topScratch->setValues(oldMin / AltUmrech * Umrech, oldMax / AltUmrech * Umrech, decimals, val / AltUmrech * Umrech);
	topScratch->setSuffix(einh);
	bottomScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	bottomScratch->setValues(oldMin / AltUmrech * Umrech, oldMax / AltUmrech * Umrech, decimals, val / AltUmrech * Umrech);
	bottomScratch->setSuffix(einh);
	leftScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	leftScratch->setValues(oldMin / AltUmrech * Umrech, oldMax / AltUmrech * Umrech, decimals, val / AltUmrech * Umrech);
	leftScratch->setSuffix(einh);
	rightScratch->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	rightScratch->setValues(oldMin / AltUmrech * Umrech, oldMax / AltUmrech * Umrech, decimals, val / AltUmrech * Umrech);
	rightScratch->setSuffix(einh);
	baseGrid->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	baseGrid->setValues(oldMin / AltUmrech * Umrech, oldMax / AltUmrech * Umrech, decimals, val / AltUmrech * Umrech);
	baseGrid->setSuffix(einh);
	baseOffset->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	baseOffset->setValues(oldMin / AltUmrech * Umrech, oldMax / AltUmrech * Umrech, decimals, val / AltUmrech * Umrech);
	baseOffset->setSuffix(einh);
	minorSpace->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	minorSpace->setValues(oldMin / AltUmrech * Umrech, oldMax / AltUmrech * Umrech, decimals, val / AltUmrech * Umrech);
	minorSpace->setSuffix(einh);
	majorSpace->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	majorSpace->setValues(oldMin / AltUmrech * Umrech, oldMax / AltUmrech * Umrech, decimals, val / AltUmrech * Umrech);
	majorSpace->setSuffix(einh);
	snapDistance->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	snapDistance->setValues(oldMin / AltUmrech * Umrech, oldMax / AltUmrech * Umrech, decimals, val / AltUmrech * Umrech);
	snapDistance->setSuffix(einh);
	DrawRuler();
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
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
void Preferences::SetDisScale()
{
	DisScale = QMAX((100.0 + CaliSlider->value()) / 100.0, 0.01);
	DrawRuler();
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
void Preferences::DrawRuler()
{
	double xl, iter, iter2, maxi;
	switch (Einheit)
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
		switch (Einheit)
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

