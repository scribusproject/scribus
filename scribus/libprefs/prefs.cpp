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

extern "C" void* Run(QWidget *d, preV *Vor);

/*!
 \fn void* Run(QWidget *d, preV *Vor)
 \author Franz Schmid
 \date  
 \brief Creates Preferences dialog and returns pointer to it
 \param d QWidget * to Preferences dialog widget
 \param Vor preV * struct
 \retval dia * widget pointer
 */
void* Run(QWidget *d, preV *Vor)
{
	Preferences *dia = new Preferences(d, Vor);
	return dia;
}

/*!
 \fn Preferences::Preferences( QWidget* parent, preV *Vor)
 \author Franz Schmid 
 \date  
 \brief Constructor for Preferences dialog box
 \param parent QWidget pointer to parent window
 \param Vor preV * struct
 \retval Preferences dialog
 */
Preferences::Preferences( QWidget* parent, preV *Vor)
		: QDialog( parent, "pref", true, 0 )
{
	int decimals;
	fon = &Vor->AvailFonts;
	ap = (ScribusApp*)parent;
	Umrech = 1.0;
	Einheit = Vor->Einheit;
	int f[] = {2, 3, 4};
	if (Einheit == 3)
		decimals = f[0];
	else
		decimals = f[Einheit];
	DisScale = Vor->DisScale;
	setCaption( tr( "Preferences" ) );
	setIcon(loadIcon("AppIcon.png"));
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
	PrefsLayout = new QVBoxLayout( this );
	PrefsLayout->setSpacing( 6 );
	PrefsLayout->setMargin( 11 );

	PrefsLayout2 = new QHBoxLayout(0, 0, 6);

	TabListe = new QListBox(this, "StyledL");
	QString ar_tab[] = { tr("General"), tr("Document"), tr("Guides"), tr("Typography"), tr("Tools"), tr("Scrapbook"), tr("Display"), tr("External Tools"), tr("Misc.")};
	size_t a_tab = sizeof(ar_tab) / sizeof(*ar_tab);
	for (uint s = 0; s < a_tab; ++s)
		TabListe->insertItem(ar_tab[s]);
	TabListe->setFocusPolicy(QWidget::NoFocus);
	PrefsLayout2->addWidget(TabListe);

	TabWidget3 = new QWidgetStack( this, "TabWidget3" );
	TabWidget3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
	TabWidget3->setFrameShape( QWidgetStack::StyledPanel );
	TabWidget3->setFrameShadow( QWidgetStack::Sunken );
	tab = new QWidget( TabWidget3, "tab" );
	tabLayout = new QGridLayout( tab );
	tabLayout->setSpacing( 6 );
	tabLayout->setMargin( 10 );

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
	guiLangCombo->setCurrentText(langMgr.getLangFromAbbrev(Vor->guiLanguage));
	selectedGUILang=Vor->guiLanguage;
	guiLangLabel = new QLabel(guiLangCombo, tr("&Language:"), ButtonGroup1, "guiLangLabel");
	ButtonGroup1Layout->addWidget( guiLangLabel, 0, 0 );
	ButtonGroup1Layout->addWidget( guiLangCombo, 0, 1, Qt::AlignRight );
	
	GUICombo = new QComboBox( true, ButtonGroup1, "GUICombo" );
	QStringList STtest;
	STtest = QStyleFactory::keys();
	for (uint stt = 0; stt < STtest.count(); ++stt)
		GUICombo->insertItem(STtest[stt]);
	GUICombo->setEditable(false);
	GUICombo->setCurrentText(Vor->GUI);
	TextGstil = new QLabel(GUICombo, tr("&Theme:"), ButtonGroup1, "dd");
	ButtonGroup1Layout->addWidget( TextGstil, 1, 0 );
	ButtonGroup1Layout->addWidget( GUICombo, 1, 1, Qt::AlignRight );
	
	GFsize = new QSpinBox(ButtonGroup1, "gfs" );
	GFsize->setSuffix( tr( " pt" ) );
	GFsize->setMaxValue( 22 );
	GFsize->setMinValue( 8 );
	GFsize->setValue( Vor->AppFontSize );
	TextGstil2 = new QLabel(GFsize, tr("&Font Size:"), ButtonGroup1, "dd");
	ButtonGroup1Layout->addWidget( TextGstil2, 2, 0 );
	ButtonGroup1Layout->addWidget( GFsize, 2, 1, Qt::AlignRight );
	tabLayout->addWidget( ButtonGroup1, 0, 0 );


	GroupBox20 = new QGroupBox( tr( "Units" ), tab, "GroupBox20" );
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
	UnitCombo->setCurrentItem(Vor->Einheit);
	GroupBox20Layout->addWidget( UnitCombo );
	tabLayout->addWidget( GroupBox20, 1, 0 );
	Mouse = new QGroupBox( tr( "Mouse Settings" ),tab, "Mouse" );
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
	SpinBox3->setValue( Vor->Wheelval );
	TextLabel1_2 = new QLabel( SpinBox3, tr( "&Wheel Jump:" ), Mouse, "TextLabel1_2" );
	TextLabel1_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabel1_2->sizePolicy().hasHeightForWidth() ) );
	Layout5->addWidget( TextLabel1_2, 0, 0 );
	Layout5->addWidget( SpinBox3, 0, 1 );
	
	SpinBox3_2 = new QSpinBox( Mouse, "SpinBox3_2" );
	SpinBox3_2->setMaxValue( 1000 );
	SpinBox3_2->setMinValue( 1 );
	SpinBox3_2->setLineStep( 1 );
	SpinBox3_2->setValue(ap->HaveDoc ? ap->doc->GrabRad : Vor->GrabRad );
	SpinBox3_2->setSuffix( tr( " px" ) );
	TextLabel1_2_2 = new QLabel( SpinBox3_2, tr( "&Grab Radius:" ), Mouse, "TextLabel1_2_2" );
	TextLabel1_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                               TextLabel1_2_2->sizePolicy().hasHeightForWidth() ) );
	Layout5->addWidget( TextLabel1_2_2, 1, 0 );
	Layout5->addWidget( SpinBox3_2, 1, 1 );
	MouseLayout->addLayout( Layout5 );
	tabLayout->addWidget( Mouse, 0, 1 );

	GroupBox20b = new QGroupBox( tr( "Menus" ), tab, "GroupBox20b" );
	GroupBox20b->setColumnLayout(0, Qt::Vertical );
	GroupBox20b->layout()->setSpacing( 0 );
	GroupBox20b->layout()->setMargin( 0 );
	GroupBox20bLayout = new QGridLayout( GroupBox20b->layout() );
	GroupBox20bLayout->setAlignment( Qt::AlignTop );
	GroupBox20bLayout->setSpacing( 5 );
	GroupBox20bLayout->setMargin( 10 );

	Recen = new QSpinBox( GroupBox20b, "Recen" );
	Recen->setMaxValue( 30 );
	Recen->setMinValue( 1 );
	Recen->setValue( Vor->RecentDCount );
	TextLabel4c = new QLabel( Recen, tr( "&Recent Documents:" ), GroupBox20b, "TextLabel4c" );
	TextLabel4c->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                            TextLabel4c->sizePolicy().hasHeightForWidth() ) );
	GroupBox20bLayout->addWidget( TextLabel4c, 0, 0);
	GroupBox20bLayout->addWidget( Recen, 0, 1 );
	tabLayout->addWidget( GroupBox20b, 1, 1 );

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
	Docs->setText(Vor->DocDir);
	PfadText = new QLabel( Docs, tr("&Documents:"), GroupBox200, "Pfadtext" );
	FileC = new QToolButton( GroupBox200, "FileC" );
	FileC->setMinimumSize( QSize( 88, 24 ) );
	FileC->setText( tr( "&Change..." ) );
	GroupBox200Layout->addWidget( PfadText, 0, 0 );
	GroupBox200Layout->addWidget( Docs, 0, 1 );
	GroupBox200Layout->addWidget( FileC, 0, 2 );
	
	
	ProPfad = new QLineEdit( GroupBox200, "Datei1" );
	ProPfad->setMinimumSize( QSize( 268, 22 ) );
	ProPfad->setText(Vor->ProfileDir);
	PfadText2 = new QLabel( ProPfad, tr("&ICC Profiles:"), GroupBox200, "Pfadtext1" );
	FileC2 = new QToolButton( GroupBox200, "FileC1" );
	FileC2->setMinimumSize( QSize( 88, 24 ) );
	FileC2->setText( tr( "C&hange..." ) );
	GroupBox200Layout->addWidget( PfadText2, 1, 0 );
	GroupBox200Layout->addWidget( ProPfad, 1, 1 );
	GroupBox200Layout->addWidget( FileC2, 1, 2 );

	ScriptPfad = new QLineEdit( GroupBox200, "Datei1" );
	ScriptPfad->setMinimumSize( QSize( 268, 22 ) );
	ScriptPfad->setText(Vor->ScriptDir);
	PfadText3 = new QLabel( ScriptPfad, tr("&Scripts:"), GroupBox200, "Pfadtext1" );
	FileC3 = new QToolButton( GroupBox200, "FileC3" );
	FileC3->setMinimumSize( QSize( 88, 24 ) );
	FileC3->setText( tr( "Ch&ange..." ) );
	GroupBox200Layout->addWidget( PfadText3, 2, 0 );
	GroupBox200Layout->addWidget( ScriptPfad, 2, 1 );
	GroupBox200Layout->addWidget( FileC3, 2, 2 );

	TemplateDir = new QLineEdit( GroupBox200, "Datei1" );
	TemplateDir->setMinimumSize( QSize( 268, 22 ) );
	TemplateDir->setText(Vor->TemplateDir);
	PfadText4 = new QLabel( TemplateDir, tr("T&emplates:"), GroupBox200, "Pfadtext1" );
	FileC4 = new QToolButton( GroupBox200, "FileC4" );
	FileC4->setMinimumSize( QSize( 88, 24 ) );
	FileC4->setText( tr( "Cha&nge..." ) );
	GroupBox200Layout->addWidget( PfadText4, 3, 0 );
	GroupBox200Layout->addWidget( TemplateDir, 3, 1 );
	GroupBox200Layout->addWidget( FileC4, 3, 2 );

	tabLayout->addMultiCellWidget( GroupBox200, 2, 2, 0, 1 );

	TabWidget3->addWidget( tab, 0 );

	tab_7 = new QWidget( TabWidget3, "tab_7" );
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
	GZComboF->setCurrentItem(Vor->PageFormat);
	GZText1 = new QLabel( GZComboF, tr( "&Size:" ), GroupSize, "GZText1" );
	Layout6->addWidget( GZText1, 0, 0 );
	Layout6->addWidget( GZComboF, 0, 1 );

	GZComboO = new QComboBox( true, GroupSize, "GZComboO" );
	GZComboO->insertItem( tr( "Portrait" ) );
	GZComboO->insertItem( tr( "Landscape" ) );
	GZComboO->setEditable(false);
	GZComboO->setCurrentItem(Vor->Ausrichtung);
	GZText2 = new QLabel( GZComboO, tr( "Orie&ntation:" ), GroupSize, "GZText2" );
	Layout6->addWidget( GZText2, 1, 0 );
	Layout6->addWidget( GZComboO, 1, 1 );

	GroupSizeLayout->addLayout( Layout6 );

	Layout5_2 = new QHBoxLayout( 0, 0, 6, "Layout5_2");


	Breite = new MSpinBox( 1, 10000, GroupSize, decimals );
	Breite->setEnabled( false );
	Breite->setMinimumSize( QSize( 70, 20 ) );
	Breite->setValue(Vor->PageBreite * Umrech);
	GZText3 = new QLabel( Breite, tr( "&Width:" ), GroupSize, "GZText3" );
	Layout5_2->addWidget( GZText3 );	
	Layout5_2->addWidget( Breite );

	Hoehe = new MSpinBox( 1, 10000, GroupSize, decimals );
	Hoehe->setEnabled( false );
	Hoehe->setMinimumSize( QSize( 70, 20 ) );
	Hoehe->setValue(Vor->PageHoehe * Umrech);
	GZText4 = new QLabel( Hoehe, tr( "&Height:" ), GroupSize, "GZText4" );
	Layout5_2->addWidget( GZText4 );	
	Layout5_2->addWidget( Hoehe );
	
	GroupSizeLayout->addLayout( Layout5_2 );
	
	Layout8 = new QHBoxLayout( 0, 0, 6, "Layout8");

	Doppelseiten = new QCheckBox( tr( "&Facing Pages" ), GroupSize, "Doppelseiten" );
	Doppelseiten->setChecked(Vor->DoppelSeiten);
	Layout8->addWidget( Doppelseiten );

	Linkszuerst = new QCheckBox( tr( "Left &Page First" ), GroupSize, "Linkszuerst" );
	Linkszuerst->setChecked(Vor->ErsteLinks);
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
	TopR->setValue(Vor->RandOben * Umrech);
	RandT = Vor->RandOben;
	GroupRandLayout->addWidget( TopR, 0, 1 );
	BottomR = new MSpinBox( 0, 1000, GroupRand, decimals );
	BottomR->setMinimumSize( QSize( 70, 20 ) );
	BottomR->setValue(Vor->RandUnten * Umrech);
	RandB = Vor->RandUnten;
	GroupRandLayout->addWidget( BottomR, 1, 1 );
	RightR = new MSpinBox( 0, 1000, GroupRand, decimals );
	RightR->setMinimumSize( QSize( 70, 20 ) );
	RightR->setValue(Vor->RandRechts * Umrech);
	RandR = Vor->RandRechts;
	GroupRandLayout->addWidget( RightR, 1, 3 );
	LeftR = new MSpinBox( 0, 1000, GroupRand, decimals );
	LeftR->setMinimumSize( QSize( 70, 20 ) );
	LeftR->setValue(Vor->RandLinks * Umrech);
	RandL = Vor->RandLinks;
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
	ASon->setChecked(Vor->AutoSave);
	GroupASLayout->addMultiCellWidget( ASon, 0, 0, 0, 1 );
	ASTime = new QSpinBox( GroupAS, "Time" );
	ASTime->setMaxValue( 60 );
	ASTime->setMinValue( 1 );
	ASTime->setSuffix( " " + tr("min") );
	ASTime->setValue(Vor->AutoSaveTime / 1000 / 60);
	ASText = new QLabel( ASTime, tr( "&Interval:" ), GroupAS, "ASText" );
	GroupASLayout->addWidget( ASText, 1, 0 );
	GroupASLayout->addWidget( ASTime, 1, 1 );
	Layout21b->addWidget( GroupAS );
	QSpacerItem* spacerDocb = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout21b->addItem( spacerDocb );
	tabLayout_7->addLayout( Layout21b );
	TabWidget3->addWidget( tab_7, 1 );

	
	tab_2 = new QWidget( TabWidget3, "tab_2" );
	tabLayout_2 = new QGridLayout( tab_2 );
	tabLayout_2->setSpacing( 6 );
	tabLayout_2->setMargin( 11 );
	
	GroupBox1 = new QGroupBox( tr( "Grid Layout" ), tab_2, "GroupBox1" );
	GroupBox1->setColumnLayout(0, Qt::Vertical );
	GroupBox1->layout()->setSpacing( 0 );
	GroupBox1->layout()->setMargin( 0 );
	GroupBox1Layout = new QHBoxLayout( GroupBox1->layout() );
	GroupBox1Layout->setAlignment( Qt::AlignTop );
	GroupBox1Layout->setSpacing( 0 );
	GroupBox1Layout->setMargin( 10 );
	Layout10 = new QGridLayout;
	Layout10->setSpacing( 6 );
	Layout10->setMargin( 0 );
	
	SpinBox1 = new MSpinBox( Umrech, 1000 * Umrech, GroupBox1, decimals );
	TextLabel1 = new QLabel( SpinBox1, tr( "M&inor Grid Spacing:" ), GroupBox1, "TextLabel1" );
	TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabel1->sizePolicy().hasHeightForWidth() ) );
	Layout10->addWidget( TextLabel1, 0, 0 );	
	Layout10->addWidget( SpinBox1, 0, 1 );
	
	SpinBox2 = new MSpinBox( 10 * Umrech, 1000 * Umrech, GroupBox1, decimals );
	TextLabel2 = new QLabel( SpinBox2, tr( "Ma&jor Grid Spacing:" ), GroupBox1, "TextLabel2" );
	TextLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabel2->sizePolicy().hasHeightForWidth() ) );
	Layout10->addWidget( TextLabel2, 1, 0 );
	Layout10->addWidget( SpinBox2, 1, 1 );
	
	SpinBox2g = new MSpinBox( Umrech, 1000 * Umrech, GroupBox1, decimals );
	TextLabel2g = new QLabel( SpinBox2g, tr( "Guide &Snap Distance:" ), GroupBox1, "TextLabel2" );
	TextLabel2g->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                            TextLabel2->sizePolicy().hasHeightForWidth() ) );
	Layout10->addWidget( TextLabel2g, 2, 0 );
	Layout10->addWidget( SpinBox2g, 2, 1 );
	
	SpinBox2->setValue(Vor->DmajGrid * Umrech);
	SpinBox1->setValue(Vor->DminGrid * Umrech);
	SpinBox2g->setValue(Vor->GuideRad * Umrech);
	GroupBox1Layout->addLayout( Layout10 );
	tabLayout_2->addWidget( GroupBox1, 0, 0 );
	
	GroupBox2 = new QGroupBox( tr( "Grid Colors" ), tab_2, "GroupBox2" );
	GroupBox2->setColumnLayout(0, Qt::Vertical );
	GroupBox2->layout()->setSpacing( 0 );
	GroupBox2->layout()->setMargin( 0 );
	GroupBox2Layout = new QHBoxLayout( GroupBox2->layout() );
	GroupBox2Layout->setAlignment( Qt::AlignTop );
	GroupBox2Layout->setSpacing( 6 );
	GroupBox2Layout->setMargin( 11 );
	Layout11 = new QGridLayout;
	Layout11->setSpacing( 6 );
	Layout11->setMargin( 0 );
	
	minColor = new QPushButton( "", GroupBox2, "minColor" );
	minColor->setAutoDefault( false );
	QPixmap pm = QPixmap(40, 20);
	pm.fill(Vor->DminColor);
	Cmin = Vor->DminColor;
	minColor->setPixmap(pm);
	TextLabel3 = new QLabel( minColor, tr( "Min&or Grid Color:" ), GroupBox2, "TextLabel3" );
	TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabel3->sizePolicy().hasHeightForWidth() ) );
	Layout11->addWidget( TextLabel3, 0, 0 );	
	Layout11->addWidget( minColor, 0, 1 );
	MaxColor = new QPushButton( "", GroupBox2, "MaxColor" );
	MaxColor->setAutoDefault( false );
	QPixmap pm1 = QPixmap(40, 20);
	pm1.fill(Vor->DmajColor);
	Cmax = Vor->DmajColor;
	MaxColor->setPixmap(pm1);
	TextLabel4 = new QLabel( MaxColor, tr( "Majo&r Grid Color:" ), GroupBox2, "TextLabel4" );
	TextLabel4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabel4->sizePolicy().hasHeightForWidth() ) );
	Layout11->addWidget( TextLabel4, 1, 0 );
	
	Layout11->addWidget( MaxColor, 1, 1 );
	GuideColor = new QPushButton( "", GroupBox2, "GuideColor" );
	GuideColor->setAutoDefault( false );
	QPixmap pm12 = QPixmap(40, 20);
	pm12.fill(Vor->guideColor);
	Cgui = Vor->guideColor;
	GuideColor->setPixmap(pm12);
	TextLabel4g = new QLabel( GuideColor, tr( "&User Guides Color:" ), GroupBox2, "TextLabel4g" );
	TextLabel4g->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                            TextLabel4g->sizePolicy().hasHeightForWidth() ) );
	Layout11->addWidget( TextLabel4g, 2, 0 );
	Layout11->addWidget( GuideColor, 2, 1 );
	
	BaseColor = new QPushButton( "", GroupBox2, "BaseColor" );
	BaseColor->setAutoDefault( false );
	QPixmap pm13 = QPixmap(40, 20);
	pm13.fill(Vor->baseColor);
	Cbase = Vor->baseColor;
	BaseColor->setPixmap(pm13);
	TextLabel4ba = new QLabel( BaseColor, tr( "Base&line Grid Color:" ), GroupBox2, "TextLabel4ba" );
	TextLabel4ba->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                            TextLabel4ba->sizePolicy().hasHeightForWidth() ) );
	Layout11->addWidget( TextLabel4ba, 3, 0 );	
	Layout11->addWidget( BaseColor, 3, 1 );
	GroupBox2Layout->addLayout( Layout11 );
	tabLayout_2->addWidget( GroupBox2, 1, 0 );
	
	ButtonGroup5 = new QButtonGroup( tr( "Placing" ), tab_2, "ButtonGroup5" );
	ButtonGroup5->setExclusive( true );
	ButtonGroup5->setColumnLayout(0, Qt::Vertical );
	ButtonGroup5->layout()->setSpacing( 0 );
	ButtonGroup5->layout()->setMargin( 0 );
	ButtonGroup5Layout = new QHBoxLayout( ButtonGroup5->layout() );
	ButtonGroup5Layout->setAlignment( Qt::AlignTop );
	ButtonGroup5Layout->setSpacing( 6 );
	ButtonGroup5Layout->setMargin( 11 );
	Layout7 = new QVBoxLayout;
	Layout7->setSpacing( 6 );
	Layout7->setMargin( 0 );
	RadioButton6 = new QRadioButton( tr( "In the &Background" ), ButtonGroup5, "RadioButton6" );
	RadioButton6->setChecked(Vor->Before );
	Layout7->addWidget( RadioButton6 );
	RadioButton7 = new QRadioButton( tr( "In the Fore&ground" ), ButtonGroup5, "RadioButton6_2" );
	RadioButton7->setChecked(!Vor->Before );
	Layout7->addWidget( RadioButton7 );
	ButtonGroup5Layout->addLayout( Layout7 );
	tabLayout_2->addWidget( ButtonGroup5, 0, 1 );
	//TabWidget3->addWidget( tab_2, 2 );

	ButtonGroup6 = new QButtonGroup( tr( "Baseline Grid" ), tab_2, "ButtonGroup6" );
	ButtonGroup6->setExclusive( true );
	ButtonGroup6->setColumnLayout(0, Qt::Vertical );
	ButtonGroup6->layout()->setSpacing( 0 );
	ButtonGroup6->layout()->setMargin( 0 );
	ButtonGroup6Layout = new QHBoxLayout( ButtonGroup6->layout() );
	ButtonGroup6Layout->setAlignment( Qt::AlignTop );
	ButtonGroup6Layout->setSpacing( 6 );
	ButtonGroup6Layout->setMargin( 11 );
	Layout18 = new QVBoxLayout;
	Layout18->setSpacing( 6 );
	Layout18->setMargin( 0 );
	RadioButton8 = new QRadioButton( tr( "O&n" ), ButtonGroup6, "RadioButton8" );
	Layout18->addWidget( RadioButton8 );
	RadioButton9 = new QRadioButton( tr( "O&ff" ), ButtonGroup6, "RadioButton8_2" );
	Layout18->addWidget( RadioButton9 );
	RadioButton8->setChecked(Vor->BaseShown );
	RadioButton9->setChecked(!Vor->BaseShown );
	ButtonGroup6Layout->addLayout( Layout18 );
	tabLayout_2->addWidget( ButtonGroup6, 1, 1 );
	TabWidget3->addWidget( tab_2, 2 );
	
	tab_4 = new QWidget( TabWidget3, "tab_4" );
	tabLayout_3 = new QGridLayout( tab_4 );
	tabLayout_3->setSpacing( 6 );
	tabLayout_3->setMargin( 11 );
	GroupBox4_2 = new QGroupBox( tr( "Subscript" ), tab_4, "GroupBox4_2" );
	GroupBox4_2->setColumnLayout(0, Qt::Vertical );
	GroupBox4_2->layout()->setSpacing( 0 );
	GroupBox4_2->layout()->setMargin( 0 );
	GroupBox4_2Layout = new QHBoxLayout( GroupBox4_2->layout() );
	GroupBox4_2Layout->setAlignment( Qt::AlignTop );
	GroupBox4_2Layout->setSpacing( 6 );
	GroupBox4_2Layout->setMargin( 11 );

	Layout12 = new QGridLayout;
	Layout12->setSpacing( 6 );
	Layout12->setMargin( 0 );
	
	VTiefW = new QSpinBox( GroupBox4_2, "VTiefW" );
	VTiefW->setSuffix( tr( " %" ) );
	VTiefW->setMaxValue( 100 );
	VTiefW->setMinValue( 1 );
	VTiefW->setValue(Vor->DVTief );
	TextLabel1_5_2 = new QLabel( VTiefW, tr( "&Displacement:" ), GroupBox4_2, "TextLabel1_5_2" );
	TextLabel1_5_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                               TextLabel1_5_2->sizePolicy().hasHeightForWidth() ) );
	Layout12->addWidget( TextLabel1_5_2, 0, 0 );
	Layout12->addWidget( VTiefW, 0, 1 );
	
	VTiefWSc = new QSpinBox( GroupBox4_2, "VTiefWSc" );
	VTiefWSc->setSuffix( tr( " %" ) );
	VTiefWSc->setMaxValue( 100 );
	VTiefWSc->setMinValue( 1 );
	VTiefWSc->setValue(Vor->DVTiefSc );
	TextLabel2_3_2 = new QLabel( VTiefWSc, tr( "&Scaling:" ), GroupBox4_2, "TextLabel2_3_2" );
	TextLabel2_3_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                               TextLabel2_3_2->sizePolicy().hasHeightForWidth() ) );
	Layout12->addWidget( TextLabel2_3_2, 1, 0 );	
	Layout12->addWidget( VTiefWSc, 1, 1 );
	GroupBox4_2Layout->addLayout( Layout12 );
	
	tabLayout_3->addWidget( GroupBox4_2, 0, 0 );
	GroupBox4 = new QGroupBox( tr( "Superscript" ), tab_4, "GroupBox4" );
	GroupBox4->setColumnLayout(0, Qt::Vertical );
	GroupBox4->layout()->setSpacing( 0 );
	GroupBox4->layout()->setMargin( 0 );
	GroupBox4Layout = new QHBoxLayout( GroupBox4->layout() );
	GroupBox4Layout->setAlignment( Qt::AlignTop );
	GroupBox4Layout->setSpacing( 6 );
	GroupBox4Layout->setMargin( 11 );
	Layout13 = new QGridLayout;
	Layout13->setSpacing( 6 );
	Layout13->setMargin( 0 );

	VHochW = new QSpinBox( GroupBox4, "VHochW" );
	VHochW->setSuffix( tr( " %" ) );
	VHochW->setMaxValue( 100 );
	VHochW->setMinValue( 1 );
	VHochW->setValue(Vor->DVHoch );
	TextLabel1_5 = new QLabel( VHochW, tr( "D&isplacement:" ), GroupBox4, "TextLabel1_5" );
	TextLabel1_5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabel1_5->sizePolicy().hasHeightForWidth() ) );
	Layout13->addWidget( TextLabel1_5, 0, 0 );	
	Layout13->addWidget( VHochW, 0, 1 );
	VHochWSc = new QSpinBox( GroupBox4, "VHochWSc" );
	VHochWSc->setSuffix( tr( " %" ) );
	VHochWSc->setMaxValue( 100 );
	VHochWSc->setMinValue( 1 );
	VHochWSc->setValue(Vor->DVHochSc );
	TextLabel2_3 = new QLabel( VHochWSc, tr( "S&caling:" ), GroupBox4, "TextLabel2_3" );
	TextLabel2_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabel2_3->sizePolicy().hasHeightForWidth() ) );
	Layout13->addWidget( TextLabel2_3, 1, 0 );	
	Layout13->addWidget( VHochWSc, 1, 1 );
	GroupBox4Layout->addLayout( Layout13 );
	
	tabLayout_3->addWidget( GroupBox4, 1, 0 );
	GroupBox4_3 = new QGroupBox( tr( "Small Caps" ), tab_4, "GroupBox4_3" );
	GroupBox4_3->setColumnLayout(0, Qt::Vertical );
	GroupBox4_3->layout()->setSpacing( 0 );
	GroupBox4_3->layout()->setMargin( 0 );
	GroupBox4_3Layout = new QHBoxLayout( GroupBox4_3->layout() );
	GroupBox4_3Layout->setAlignment( Qt::AlignTop );
	GroupBox4_3Layout->setSpacing( 6 );
	GroupBox4_3Layout->setMargin( 11 );
	Layout14 = new QHBoxLayout;
	Layout14->setSpacing( 6 );
	Layout14->setMargin( 0 );
	SmallCaps = new QSpinBox( GroupBox4_3, "SmallCaps" );
	SmallCaps->setSuffix( tr( " %" ) );
	SmallCaps->setMaxValue( 100 );
	SmallCaps->setMinValue( 1 );
	SmallCaps->setValue(Vor->DVKapit );
	TextLabel2_3_3 = new QLabel( SmallCaps, tr( "Sc&aling:" ), GroupBox4_3, "TextLabel2_3_3" );
	TextLabel2_3_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                               TextLabel2_3_3->sizePolicy().hasHeightForWidth() ) );
	Layout14->addWidget( TextLabel2_3_3 );
	
	Layout14->addWidget( SmallCaps );
	GroupBox4_3Layout->addLayout( Layout14 );
	
	tabLayout_3->addWidget( GroupBox4_3, 0, 1 );

	GroupBox4_3a = new QGroupBox( tr( "Other" ), tab_4, "GroupBox4_3a" );
	GroupBox4_3a->setColumnLayout(0, Qt::Vertical );
	GroupBox4_3a->layout()->setSpacing( 0 );
	GroupBox4_3a->layout()->setMargin( 0 );
	GroupBox4_3aLayout = new QGridLayout( GroupBox4_3a->layout() );
	GroupBox4_3aLayout->setAlignment( Qt::AlignTop );
	GroupBox4_3aLayout->setSpacing( 6 );
	GroupBox4_3aLayout->setMargin( 11 );
	
	BaseGrid = new MSpinBox( 1, 100, GroupBox4_3a, 1 );
	BaseGrid->setSuffix( tr( " pt" ) );
	BaseGrid->setValue(Vor->BaseGrid );
	TextLabel2_3_3b = new QLabel( BaseGrid, tr( "Baseline &Grid:" ), GroupBox4_3a, "TextLabel2_3_3" );
	GroupBox4_3aLayout->addWidget( TextLabel2_3_3b, 0, 0 );
	
	GroupBox4_3aLayout->addWidget( BaseGrid, 0, 1 );
	
	BaseOffs = new MSpinBox( 0, 1000, GroupBox4_3a, 1 );
	BaseOffs->setSuffix( tr( " pt" ) );
	BaseOffs->setValue(Vor->BaseOffs );
	TextLabel2_3_3c = new QLabel( BaseOffs, tr( "Baseline &Offset:" ), GroupBox4_3a, "TextLabel2_3_3" );
	GroupBox4_3aLayout->addWidget( TextLabel2_3_3c, 1, 0 );
	GroupBox4_3aLayout->addWidget( BaseOffs, 1, 1 );
	
	AutoLineV = new QSpinBox( GroupBox4_3a, "LineS" );
	AutoLineV->setSuffix( tr( " %" ) );
	AutoLineV->setMaxValue( 100 );
	AutoLineV->setMinValue( 1 );
	AutoLineV->setValue(Vor->AutoLine );
	TextLabel2_3_3a = new QLabel( AutoLineV, tr( "Automatic &Line Spacing:" ), GroupBox4_3a, "TextLabel2_3_3" );
	GroupBox4_3aLayout->addWidget( TextLabel2_3_3a, 2, 0 );
	GroupBox4_3aLayout->addWidget( AutoLineV, 2, 1 );
	tabLayout_3->addWidget( GroupBox4_3a, 1, 1 );

	TabWidget3->addWidget( tab_4, 3 );

	tab_3 = new QWidget( TabWidget3, "tab_3" );
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

	FontComb = new FontCombo(ToolFrame, Vor);
	for (int fc=0; fc<FontComb->count(); ++fc)
	{
		if (FontComb->text(fc) == Vor->DefFont)
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
		if (SizeCombo->text(a).left(2).toInt() == Vor->DefSize / 10)
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
	for (itc = Vor->DColors.begin(); itc != Vor->DColors.end(); ++itc)
	{
		pm2.fill(Vor->DColors[itc.key()].getRGBColor());
		ForegroundT->insertItem(pm2, itc.key());
		if (itc.key() == Vor->DpenText)
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
	TextColVal->setValue(Vor->DCols );
	TextCol = new QLabel(TextColVal, tr("Colu&mns:"), ToolFrame, "TextCol");
	Layout15a->addWidget( TextCol, 3, 0 );
	Layout15a->addWidget( TextColVal, 3, 1 );
	
	TextGapVal = new MSpinBox( 0, 200, ToolFrame, decimals );
	TextGapVal->setSuffix( tr( " pt" ) );
	TextGapVal->setValue(Vor->DGap);
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
	if (Vor->Dpen == "None")
		Foreground->setCurrentItem(Foreground->count()-1);
	pm2 = QPixmap(15, 15);
	for (itc = Vor->DColors.begin(); itc != Vor->DColors.end(); ++itc)
	{
		pm2.fill(Vor->DColors[itc.key()].getRGBColor());
		Foreground->insertItem(pm2, itc.key());
		if (itc.key() == Vor->Dpen)
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
	Shade2->setValue(Vor->Dshade2 );
	TextLabelT22 = new QLabel( Shade2, tr( "&Shading:" ), ToolFrame2, "TextLabel2_2" );
	TextLabelT22->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT22->sizePolicy().hasHeightForWidth() ) );
	Layout15b->addWidget(TextLabelT22, 1, 0);
	Layout15b->addWidget(Shade2, 1, 1);
	
	Background = new QComboBox( true, ToolFrame2, "Background" );
	Background->setEditable(false);
	Background->insertItem( tr("None"));
	if (Vor->Dbrush == "None")
		Background->setCurrentItem(Background->count()-1);
	for (itc = Vor->DColors.begin(); itc != Vor->DColors.end(); ++itc)
	{
		pm2.fill(Vor->DColors[itc.key()].getRGBColor());
		Background->insertItem(pm2, itc.key());
		if (itc.key() == Vor->Dbrush)
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
	Shade->setValue(Vor->Dshade );
	TextLabelT24 = new QLabel( Shade, tr( "S&hading:" ), ToolFrame2, "TextLabel2_2_2" );
	TextLabelT24->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT24->sizePolicy().hasHeightForWidth() ) );
	Layout15b->addWidget(TextLabelT24, 3, 0);
	Layout15b->addWidget(Shade, 3, 1);
	
	Linestyle = new LineCombo(ToolFrame2);
	Linestyle->setEditable(false);
	switch (Vor->DLineArt)
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
	LineW->setValue(Vor->Dwidth);
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
	QPixmap pm3;
	pm3 = QPixmap(15, 15);
	CListe::Iterator itc2;
	for (itc2 = Vor->DColors.begin(); itc2 != Vor->DColors.end(); ++itc2)
	{
		pm3.fill(Vor->DColors[itc2.key()].getRGBColor());
		Foreground2->insertItem(pm3, itc2.key());
		if (itc2.key() == Vor->DpenLine)
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
	Shade22->setValue(Vor->DshadeLine );
	TextLabelT32 = new QLabel( Shade22, tr( "&Shading:" ), ToolFrame3, "TextLabel2_2" );
	TextLabelT32->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT32->sizePolicy().hasHeightForWidth() ) );
	Layout15c->addWidget(TextLabelT32, 1, 0);	
	Layout15c->addWidget(Shade22, 1, 1);
	Linestyle2 = new LineCombo(ToolFrame3);
	Linestyle2->setEditable(false);
	int dla = Vor->DLstyleLine;
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
	LineW2->setValue(Vor->DwidthLine);
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
	MinMag->setValue(Vor->MagMin );
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
	MaxMag->setValue(Vor->MagMax );
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
	StepMag->setValue(Vor->MagStep );
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
	XScale->setValue(qRound(Vor->ScaleX * 100) );
	TextLabelT51 = new QLabel( XScale, tr( "&Horizontal Scaling:" ), ToolFrame5, "TextLabelT51" );
	TextLabelT51->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                             TextLabelT51->sizePolicy().hasHeightForWidth() ) );
	Layout15->addWidget( TextLabelT51, 1, 1 );	
	Layout15->addWidget( XScale, 1, 2 );
	YScale = new QSpinBox( ToolFrame5, "YScale" );
	YScale->setSuffix( tr( " %" ) );
	YScale->setMaxValue( 1000 );
	YScale->setMinValue( 1 );
	YScale->setValue(qRound(Vor->ScaleY * 100) );
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
	ShadeP->setValue(Vor->ShadePict );
	TextLabelT54 = new QLabel( ShadeP, tr( "S&hading:" ), ToolFrame5, "TextLabelT54" );
	TextLabelT54->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabelT54->sizePolicy().hasHeightForWidth() ) );
	Layout15->addWidget( TextLabelT54, 6, 0 );	
	Layout15->addWidget( ShadeP, 6, 1 );
	BackgroundP = new QComboBox( true, ToolFrame5, "BackgroundP" );
	BackgroundP->setEditable(false);
	BackgroundP->insertItem( tr("None"));
	if (Vor->DbrushPict == "None")
		BackgroundP->setCurrentItem(BackgroundP->count()-1);
	for (itc = Vor->DColors.begin(); itc != Vor->DColors.end(); ++itc)
	{
		pm2.fill(Vor->DColors[itc.key()].getRGBColor());
		BackgroundP->insertItem(pm2, itc.key());
		if (itc.key() == Vor->DbrushPict)
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
	Aspect->setChecked(Vor->AspectRatio);
	FreeScale->setChecked(Vor->ScaleType ? true : false);
	FrameScale->setChecked(Vor->ScaleType ? false : true);
	ChangeScaling();
	Fram->addWidget(ToolFrame5, 5);

	ToolFrame6 = new QFrame( this, "ToolFrame6" );
	ToolFrame6->setFrameShape( QFrame::Box );
	ToolFrame6->setFrameShadow( QFrame::Sunken );
	PolygonPropsLayout = new QHBoxLayout( ToolFrame6, 10, 5, "PolygonPropsLayout");
	polyWidget = new PolygonWidget(ToolFrame6, Vor->PolyC, Vor->PolyFd, Vor->PolyF, Vor->PolyS, Vor->PolyR);
	PolygonPropsLayout->addWidget( polyWidget );
	QSpacerItem* T6_spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	PolygonPropsLayout->addItem( T6_spacer2 );
	Fram->addWidget(ToolFrame6, 6);

	TabWidget3->addWidget( tab_3, 4 );

	tab_5 = new QWidget( TabWidget3, "tab_5" );
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
	SaveAtQuit->setChecked(Vor->SaveAtQ);
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
	int sci = Vor->PSize == 40 ? 0 : Vor->PSize == 60 ? 1 : 2;
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
	TabWidget3->addWidget( tab_5, 5 );

	tab_6 = new QWidget( TabWidget3, "tab_6" );
	tabLayout_6 = new QHBoxLayout( tab_6 );
	tabLayout_6->setSpacing( 6 );
	tabLayout_6->setMargin( 11 );
	GroupBox10 = new QGroupBox( "", tab_6, "GroupBox10" );
	GroupBox10->setColumnLayout(0, Qt::Vertical );
	GroupBox10->layout()->setSpacing( 0 );
	GroupBox10->layout()->setMargin( 0 );
	GroupBox10Layout = new QVBoxLayout( GroupBox10->layout() );
	GroupBox10Layout->setAlignment( Qt::AlignTop );
	GroupBox10Layout->setSpacing( 6 );
	GroupBox10Layout->setMargin( 11 );
	SidebySide = new QCheckBox( tr( "Display Pages &Side by Side" ), GroupBox10, "SidebySide" );
	SidebySide->setChecked(Vor->PagesSbS);
	GroupBox10Layout->addWidget( SidebySide );

	Layout_6b = new QHBoxLayout();
	Layout_6b->setSpacing( 0 );
	Layout_6b->setMargin( 0 );
	GroupBox20a = new QGroupBox( tr( "Page Colors" ), GroupBox10, "GroupBox20" );
	GroupBox20a->setColumnLayout(0, Qt::Vertical );
	GroupBox20a->layout()->setSpacing( 0 );
	GroupBox20a->layout()->setMargin( 0 );
	GroupBox20aLayout = new QHBoxLayout( GroupBox20a->layout() );
	GroupBox20aLayout->setAlignment( Qt::AlignTop );
	GroupBox20aLayout->setSpacing( 10 );
	GroupBox20aLayout->setMargin( 5 );
	PapColor = new QPushButton( "", GroupBox20a, "PapColor" );
	PapColor->setAutoDefault( false );
	QPixmap pm1a = QPixmap(40, 20);
	pm1a.fill(Vor->DpapColor);
	Cpaper = Vor->DpapColor;
	PapColor->setPixmap(pm1a);
	TextLabel4a = new QLabel( PapColor, tr( "&Background:" ), GroupBox20a, "TextLabel4" );
	TextLabel4a->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                            TextLabel4a->sizePolicy().hasHeightForWidth() ) );
	GroupBox20aLayout->addWidget( TextLabel4a);
	GroupBox20aLayout->addWidget( PapColor );
	
	RandColor = new QPushButton( "", GroupBox20a, "RColor" );
	RandColor->setAutoDefault( false );
	pm1a.fill(Vor->DmargColor);
	Crand = Vor->DmargColor;
	RandColor->setPixmap(pm1a);
	TextLabel4b = new QLabel( RandColor, tr( "&Margins:" ), GroupBox20a, "TextLabel4" );
	TextLabel4b->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                            TextLabel4b->sizePolicy().hasHeightForWidth() ) );
	GroupBox20aLayout->addWidget( TextLabel4b);
	GroupBox20aLayout->addWidget( RandColor );
	Layout_6b->addWidget(GroupBox20a);
	
	QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout_6b->addItem( spacer2 );
	GroupBox10Layout->addLayout(Layout_6b);
	RandFarb = new QCheckBox( tr( "Display &Unprintable Area in Margin Color" ), GroupBox10, "Ran" );
	RandFarb->setChecked(Vor->RandFarbig);
	GroupBox10Layout->addWidget( RandFarb );
	UsePDFTrans = new QCheckBox( tr( "Use PDF 1.4 &Transparency Features" ), GroupBox10, "Ran2" );
	UsePDFTrans->setChecked(Vor->PDFTransparency);
	GroupBox10Layout->addWidget( UsePDFTrans );

	CaliGroup = new QGroupBox( tr( "&Adjust Display Size" ), GroupBox10, "CaliGroup" );
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
	CaliRuler->setScaledContents( FALSE );
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

	GroupBox10Layout->addWidget( CaliGroup );

	tabLayout_6->addWidget( GroupBox10 );
	TabWidget3->addWidget( tab_6, 6 );
	
	ExtTool = new QWidget( TabWidget3, "ExtTool" );
	ExtToolLayout = new QVBoxLayout( ExtTool, 11, 6, "ExtToolLayout"); 
	groupGS = new QGroupBox( tr( "Postscript Interpreter" ), ExtTool, "groupGS" );
	groupGS->setColumnLayout(0, Qt::Vertical );
	groupGS->layout()->setSpacing( 6 );
	groupGS->layout()->setMargin( 10 );
	groupGSLayout = new QVBoxLayout( groupGS->layout() );
	groupGSLayout->setAlignment( Qt::AlignTop );
	GSlayout = new QHBoxLayout( 0, 0, 6, "GSlayout"); 
	GSName = new QLineEdit( groupGS, "GSName" );
	GSName->setText(Vor->gs_exe);
	GSText = new QLabel( GSName, tr( "&Name of Executable:" ), groupGS, "GSText" );
	GSlayout->addWidget( GSText );
	GSlayout->addWidget( GSName );
	groupGSLayout->addLayout( GSlayout );
	GSantiText = new QCheckBox( tr( "Antialias &Text" ), groupGS, "GSantiText" );
	GSantiText->setChecked(Vor->gs_antiText);
	groupGSLayout->addWidget( GSantiText );
	GSantiGraph = new QCheckBox( tr( "Antialias &Graphics" ), groupGS, "GSantiGraph" );
	GSantiGraph->setChecked(Vor->gs_antiGraph);
	groupGSLayout->addWidget( GSantiGraph );
	ExtToolLayout->addWidget( groupGS );
	groupGimp = new QGroupBox( tr( "Image Processing Tool" ), ExtTool, "groupGimp" );
	groupGimp->setColumnLayout(0, Qt::Vertical );
	groupGimp->layout()->setSpacing( 6 );
	groupGimp->layout()->setMargin( 10 );
	groupGimpLayout = new QHBoxLayout( groupGimp->layout() );
	groupGimpLayout->setAlignment( Qt::AlignTop );
	GimpName = new QLineEdit( groupGimp, "GimpName" );
	GimpName->setText(Vor->gimp_exe);
	GimpText = new QLabel( GimpName, tr( "Name of &Executable:" ), groupGimp, "GimpText" );
	groupGimpLayout->addWidget( GimpText );
	groupGimpLayout->addWidget( GimpName );
	ExtToolLayout->addWidget( groupGimp );
	QSpacerItem* spacer_gs = new QSpacerItem( 0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	ExtToolLayout->addItem( spacer_gs );
	TabWidget3->addWidget( ExtTool, 7 );

	Misc = new QWidget( TabWidget3, "Misc" );
	MiscLayout = new QVBoxLayout( Misc, 11, 6, "MiscLayout"); 
	groupPrint = new QGroupBox( tr( "Printing" ), Misc, "groupPrint" );
	groupPrint->setColumnLayout(0, Qt::Vertical );
	groupPrint->layout()->setSpacing( 10 );
	groupPrint->layout()->setMargin( 10 );
	groupPrintLayout = new QVBoxLayout( groupPrint->layout() );
	groupPrintLayout->setAlignment( Qt::AlignTop );
	ClipMarg = new QCheckBox( tr( "Clip to Page &Margins" ), groupPrint, "ClipMarg" );
	ClipMarg->setChecked(Vor->ClipMargin);
	groupPrintLayout->addWidget( ClipMarg );
	DoGCR = new QCheckBox( tr( "Apply &Under Color Removal" ), groupPrint, "ClipMarg" );
	DoGCR->setChecked(Vor->GCRMode);
	groupPrintLayout->addWidget( DoGCR );
	MiscLayout->addWidget( groupPrint );
	QSpacerItem* spacer_3m = new QSpacerItem( 0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	MiscLayout->addItem( spacer_3m );
	TabWidget3->addWidget( Misc, 8 );

	TabListe->setCurrentItem(0);
	TabListe->setSelected(0, true);
	Fram->raiseWidget(1);

	PrefsLayout2->addWidget( TabWidget3 );
	PrefsLayout->addLayout(PrefsLayout2);

	Layout3 = new QHBoxLayout;
	Layout3->setSpacing( 6 );
	Layout3->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout3->addItem( spacer );
	buttonOK = new QPushButton( tr( "&OK" ), this, "buttonOK" );
	buttonOK->setDefault( true );
	buttonOK->setFocus();
	Layout3->addWidget( buttonOK );
	buttonCancel = new QPushButton( tr( "&Cancel" ), this, "buttonCancel" );
	buttonCancel->setDefault( false );
	Layout3->addWidget( buttonCancel );
	PrefsLayout->addLayout( Layout3 );
	setDS();
	//tab order
	QWidget::setTabOrder( GFsize, SpinBox3 );
	QWidget::setTabOrder( SpinBox3, SpinBox3_2 );
	QWidget::setTabOrder( SpinBox3_2, UnitCombo );
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
	QToolTip::add( SpinBox3_2, tr( "Radius of the area where Scribus will allow you to grab an objects handles" ) );
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

	QToolTip::add( SpinBox1, tr( "Distance between the minor grid lines" ) );
	QToolTip::add( SpinBox2, tr( "Distance between the major grid lines" ) );
	QToolTip::add( SpinBox2g, tr( "Distance within which an object will snap to your placed guides" ) );
	QToolTip::add( minColor, tr( "Color of the minor grid lines" ) );
	QToolTip::add( MaxColor, tr( "Color of the major grid lines" ) );
	QToolTip::add( GuideColor, tr( "Color of the guide lines you insert" ) );
	QToolTip::add( RadioButton6, tr( "Place the grid behind your page objects" ) );
	QToolTip::add( RadioButton7, tr( "Place the grid in front of your page objects" ) );
	QToolTip::add( RadioButton8, tr("Turns on the basegrid"));
	QToolTip::add( RadioButton9, tr("Turns off the basegrid")); 

	QToolTip::add( VHochW, tr( "Displacement above the baseline of the font on a line" ) );
	QToolTip::add( VHochWSc, tr( "Relative size of the superscript compared to the normal font" ) );
	QToolTip::add( VTiefW, tr( "Displacement below the baseline of the normal font on a line" ) );
	QToolTip::add( VTiefWSc, tr( "Relative size of the subscript compared to the normal font" ) );
	QToolTip::add( SmallCaps, tr( "Relative size of the small caps font compared to the normal font" ) );
	QToolTip::add( AutoLineV, tr( "Percentage increase over the font size for the line spacing" ) );

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

	QToolTip::add( SidebySide, tr( "When using facing pages, show the two pages side by side" ) );
	QToolTip::add( PapColor, tr( "Color for paper" ) );
	QToolTip::add( RandColor, tr( "Color for the margin lines" ) );
	QToolTip::add( RandFarb, tr( "Mask the area outside the margins in the margin color" ) );
	QToolTip::add( UsePDFTrans, tr( "Enable transparency features within PDF 1.4 export" ) );
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

	connect( minColor, SIGNAL( clicked() ), this, SLOT( changeMicolor() ) );
	connect( MaxColor, SIGNAL( clicked() ), this, SLOT( changeMaColor() ) );
	connect( PapColor, SIGNAL( clicked() ), this, SLOT( changePapColor() ) );
	connect( RandColor, SIGNAL( clicked() ), this, SLOT( changeRandColor() ) );
	connect( GuideColor, SIGNAL( clicked() ), this, SLOT( changeGuideColor() ) );
	connect( BaseColor, SIGNAL( clicked() ), this, SLOT( changeBaseColor() ) );
	connect( buttonOK, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
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
	connect(TabListe, SIGNAL(highlighted(int)), TabWidget3, SLOT(raiseWidget(int)));
	setSize(Vor->PageFormat);
	setOrien(Vor->Ausrichtung);
	Breite->setValue(Vor->PageBreite * Umrech);
	Hoehe->setValue(Vor->PageHoehe * Umrech);
	UnitChange();
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
void Preferences::changeMaColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(Cmax, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(40, 20);
		pm.fill(neu);
		Cmax = neu;
		MaxColor->setPixmap(pm);
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
void Preferences::changeMicolor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(Cmin, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(40, 20);
		pm.fill(neu);
		Cmin = neu;
		minColor->setPixmap(pm);
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
void Preferences::changePapColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(Cpaper, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(40, 20);
		pm.fill(neu);
		Cpaper = neu;
		PapColor->setPixmap(pm);
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
void Preferences::changeRandColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(Crand, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(40, 20);
		pm.fill(neu);
		Crand = neu;
		RandColor->setPixmap(pm);
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
	neu = QColorDialog::getColor(Cgui, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(40, 20);
		pm.fill(neu);
		Cgui = neu;
		GuideColor->setPixmap(pm);
	}
}
void Preferences::changeBaseColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(Cbase, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(40, 20);
		pm.fill(neu);
		Cbase = neu;
		BaseColor->setPixmap(pm);
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
	double oldS1, oldSM1, oldSV1, oldS2, oldSM2, oldSV2, oldS2g, oldSM2g, oldSV2g, oldTG, oldTGM, oldTGV, oldB, oldBM, oldH, oldHM, val;
	SpinBox1->getValues(&oldS1, &oldSM1, &decimals, &oldSV1);
	oldS1 /= AltUmrech;
	oldSM1 /= AltUmrech;
	oldSV1 /= AltUmrech;
	SpinBox2->getValues(&oldS2, &oldSM2, &decimals, &oldSV2);
	oldS2 /= AltUmrech;
	oldSM2 /= AltUmrech;
	oldSV2 /= AltUmrech;
	SpinBox2g->getValues(&oldS2g, &oldSM2g, &decimals, &oldSV2g);
	oldS2g /= AltUmrech;
	oldSM2g /= AltUmrech;
	oldSV2g /= AltUmrech;
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
	SpinBox1->setSuffix(einh);
	SpinBox2->setSuffix(einh);
	SpinBox2g->setSuffix(einh);
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
	SpinBox1->setValues(oldS1 * Umrech, oldSM1 * Umrech, decimals, oldSV1 * Umrech);
	SpinBox2->setValues(oldS2 * Umrech, oldSM2 * Umrech, decimals, oldSV2 * Umrech);
	SpinBox2g->setValues(oldS2g * Umrech, oldSM2g * Umrech, decimals, oldSV2g * Umrech);
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

