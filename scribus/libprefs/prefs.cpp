#include "prefs.h"
#include "prefs.moc"
#include "keymanager.h"
#include <qfont.h>
#include <qcolordialog.h>
#include <qcolor.h>
#include <qstringlist.h>
#include <qstylefactory.h>
#include <qpointarray.h>
#include <qpainter.h>
#include <qrect.h>
#include <qwmatrix.h>
#include <cmath>

extern QPixmap FontSample(QString da, int s, QString ts, QColor back);
extern QPixmap loadIcon(QString nam);
extern QPointArray RegularPolygon(double w, double h, uint c, bool star, double factor, double rota);

extern "C" void* Run(QWidget *d, preV *Vor);

void* Run(QWidget *d, preV *Vor)
{
	Preferences *dia = new Preferences(d, Vor);
	return dia;
}

Preferences::Preferences( QWidget* parent, preV *Vor)
    : QDialog( parent, "pref", true, 0 )
{
    fon = &Vor->AvailFonts;
		ap = (ScribusApp*)parent;
    Umrech = 1.0;
    Einheit = ap->HaveDoc ? ap->doc->Einheit : Vor->Einheit;
		DisScale = Vor->DisScale;
    KKC = Vor->KeyActions;
    setCaption( tr( "Preferences" ) );
  	setIcon(loadIcon("AppIcon.png"));
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    PrefsLayout = new QVBoxLayout( this );
    PrefsLayout->setSpacing( 6 );
    PrefsLayout->setMargin( 11 );

    PrefsLayout2 = new QHBoxLayout(0, 0, 6);

		TabListe = new QListBox(this, "StyledL");
    TabListe->insertItem( tr( "General" ) );
    TabListe->insertItem( tr( "Document" ) );
    TabListe->insertItem( tr( "Guides" ) );
    TabListe->insertItem( tr( "Typography" ) );
    TabListe->insertItem( tr( "Tools" ) );
    TabListe->insertItem( tr( "Scrapbook" ) );
    TabListe->insertItem( tr( "Display" ) );
		PrefsLayout2->addWidget(TabListe);

    TabWidget3 = new QWidgetStack( this, "TabWidget3" );
    TabWidget3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    TabWidget3->setFrameShape( QWidgetStack::StyledPanel );
    TabWidget3->setFrameShadow( QWidgetStack::Sunken );
    tab = new QWidget( TabWidget3, "tab" );
    tabLayout = new QGridLayout( tab );
    tabLayout->setSpacing( 6 );
    tabLayout->setMargin( 10 );
    ButtonGroup1 = new QButtonGroup( tab, "ButtonGroup1" );
    ButtonGroup1->setTitle( tr( "GUI" ) );
    ButtonGroup1->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1->layout()->setSpacing( 0 );
    ButtonGroup1->layout()->setMargin( 0 );
    ButtonGroup1Layout = new QGridLayout( ButtonGroup1->layout() );
    ButtonGroup1Layout->setAlignment( Qt::AlignTop );
    ButtonGroup1Layout->setSpacing( 6 );
    ButtonGroup1Layout->setMargin( 10 );
    TextGstil = new QLabel(ButtonGroup1, "dd");
    TextGstil->setText( tr("Theme"));
    ButtonGroup1Layout->addWidget( TextGstil, 0, 0 );
    GUICombo = new QComboBox( true, ButtonGroup1, "GUICombo" );
		QStringList STtest;
		STtest = QStyleFactory::keys();
		for (uint stt = 0; stt < STtest.count(); ++stt)
			{
    	GUICombo->insertItem(STtest[stt]);
			}
    GUICombo->setEditable(false);
    GUICombo->setCurrentText(Vor->GUI);
    ButtonGroup1Layout->addWidget( GUICombo, 0, 1, Qt::AlignRight );
    TextGstil2 = new QLabel(ButtonGroup1, "dd");
    TextGstil2->setText( tr("Font Size:"));
    ButtonGroup1Layout->addWidget( TextGstil2, 1, 0 );
    GFsize = new QSpinBox(ButtonGroup1, "gfs" );
    GFsize->setSuffix( tr( " pts" ) );
    GFsize->setMaxValue( 22 );
    GFsize->setMinValue( 8 );
    GFsize->setValue( Vor->AppFontSize );
    ButtonGroup1Layout->addWidget( GFsize, 1, 1, Qt::AlignRight );
    tabLayout->addWidget( ButtonGroup1, 0, 0 );
    GroupBox20 = new QGroupBox( tab, "GroupBox20" );
    GroupBox20->setTitle( tr( "Units" ) );
    GroupBox20->setColumnLayout(0, Qt::Vertical );
    GroupBox20->layout()->setSpacing( 0 );
    GroupBox20->layout()->setMargin( 0 );
    GroupBox20Layout = new QHBoxLayout( GroupBox20->layout() );
    GroupBox20Layout->setAlignment( Qt::AlignTop );
    GroupBox20Layout->setSpacing( 0 );
    GroupBox20Layout->setMargin( 25 );
    UnitCombo = new QComboBox( true, GroupBox20, "UnitCombo" );
    UnitCombo->insertItem( tr( "Points (pt)" ) );
    UnitCombo->insertItem( tr( "Millimeters (mm)" ) );
    UnitCombo->insertItem( tr( "Inches (in)" ) );
    UnitCombo->insertItem( tr( "Picas (p)" ) );
    UnitCombo->setEditable(false);
    UnitCombo->setCurrentItem(ap->HaveDoc ? ap->doc->Einheit : Vor->Einheit);
    GroupBox20Layout->addWidget( UnitCombo );
    tabLayout->addWidget( GroupBox20, 1, 0 );
    Mouse = new QGroupBox( tab, "Mouse" );
    Mouse->setTitle( tr( "Mouse-Settings" ) );
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
    TextLabel1_2 = new QLabel( Mouse, "TextLabel1_2" );
    TextLabel1_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel1_2->sizePolicy().hasHeightForWidth() ) );
		TextLabel1_2->setText( tr( "Wheel-Jump" ) );
    Layout5->addWidget( TextLabel1_2, 0, 0 );
    SpinBox3 = new QSpinBox( Mouse, "SpinBox3" );
    SpinBox3->setMaxValue( 1000 );
    SpinBox3->setMinValue( 0 );
    SpinBox3->setLineStep( 10 );
    SpinBox3->setValue( Vor->Wheelval );
    Layout5->addWidget( SpinBox3, 0, 1 );
    TextLabel1_2_2 = new QLabel( Mouse, "TextLabel1_2_2" );
    TextLabel1_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel1_2_2->sizePolicy().hasHeightForWidth() ) );
    TextLabel1_2_2->setText( tr( "Grab-Radius" ) );
    Layout5->addWidget( TextLabel1_2_2, 1, 0 );
    SpinBox3_2 = new QSpinBox( Mouse, "SpinBox3_2" );
    SpinBox3_2->setMaxValue( 1000 );
    SpinBox3_2->setMinValue( 1 );
    SpinBox3_2->setLineStep( 1 );
    SpinBox3_2->setValue(ap->HaveDoc ? ap->doc->GrabRad : Vor->GrabRad );
    Layout5->addWidget( SpinBox3_2, 1, 1 );
    MouseLayout->addLayout( Layout5 );
    tabLayout->addWidget( Mouse, 0, 1 );

    GroupBox20b = new QGroupBox( tab, "GroupBox20b" );
    GroupBox20b->setTitle( tr( "Menus" ) );
    GroupBox20b->setColumnLayout(0, Qt::Vertical );
    GroupBox20b->layout()->setSpacing( 0 );
    GroupBox20b->layout()->setMargin( 0 );
    GroupBox20bLayout = new QGridLayout( GroupBox20b->layout() );
    GroupBox20bLayout->setAlignment( Qt::AlignTop );
    GroupBox20bLayout->setSpacing( 5 );
    GroupBox20bLayout->setMargin( 10 );
    TextLabel4c = new QLabel( GroupBox20b, "TextLabel4c" );
    TextLabel4c->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel4c->sizePolicy().hasHeightForWidth() ) );
    TextLabel4c->setText( tr( "Recent Documents:" ) );
    GroupBox20bLayout->addWidget( TextLabel4c, 0, 0);
    Recen = new QSpinBox( GroupBox20b, "Recen" );
    Recen->setMaxValue( 30 );
    Recen->setMinValue( 1 );
    Recen->setValue( Vor->RecentDCount );
    GroupBox20bLayout->addWidget( Recen, 0, 1 );
    Shorty = new QPushButton(GroupBox20b, "SH");
    Shorty->setText( tr("Keyboard Shortcuts..."));
    GroupBox20bLayout->addMultiCellWidget( Shorty, 1, 1, 0, 1 );
    tabLayout->addWidget( GroupBox20b, 1, 1 );

    GroupBox200 = new QGroupBox( tab, "GroupBox200" );
    GroupBox200->setTitle( tr( "Paths" ) );
    GroupBox200->setColumnLayout(0, Qt::Horizontal );
    GroupBox200->layout()->setSpacing( 0 );
    GroupBox200->layout()->setMargin( 0 );
    GroupBox200Layout = new QGridLayout( GroupBox200->layout() );
    GroupBox200Layout->setAlignment( Qt::AlignTop );
    GroupBox200Layout->setSpacing( 0 );
    GroupBox200Layout->setMargin( 5 );
		PfadText = new QLabel( GroupBox200, "Pfadtext" );
		PfadText->setText( tr("Documents:"));
    GroupBox200Layout->addWidget( PfadText, 0, 0 );
    Docs = new QLineEdit( GroupBox200, "Datei" );
    Docs->setMinimumSize( QSize( 268, 22 ) );
		Docs->setText(Vor->DocDir);
    GroupBox200Layout->addWidget( Docs, 0, 1 );
    FileC = new QToolButton( GroupBox200, "FileC" );
    FileC->setMinimumSize( QSize( 88, 24 ) );
    FileC->setText( tr( "Change..." ) );
    GroupBox200Layout->addWidget( FileC, 0, 2 );
		PfadText2 = new QLabel( GroupBox200, "Pfadtext1" );
		PfadText2->setText( tr("ICC-Profiles:"));
    GroupBox200Layout->addWidget( PfadText2, 1, 0 );
    ProPfad = new QLineEdit( GroupBox200, "Datei1" );
    ProPfad->setMinimumSize( QSize( 268, 22 ) );
		ProPfad->setText(Vor->ProfileDir);
    GroupBox200Layout->addWidget( ProPfad, 1, 1 );
    FileC2 = new QToolButton( GroupBox200, "FileC1" );
    FileC2->setMinimumSize( QSize( 88, 24 ) );
    FileC2->setText( tr( "Change..." ) );
    GroupBox200Layout->addWidget( FileC2, 1, 2 );

		PfadText3 = new QLabel( GroupBox200, "Pfadtext1" );
		PfadText3->setText( tr("Scripts:"));
    GroupBox200Layout->addWidget( PfadText3, 2, 0 );
    ScriptPfad = new QLineEdit( GroupBox200, "Datei1" );
    ScriptPfad->setMinimumSize( QSize( 268, 22 ) );
		ScriptPfad->setText(Vor->ScriptDir);
    GroupBox200Layout->addWidget( ScriptPfad, 2, 1 );
    FileC3 = new QToolButton( GroupBox200, "FileC3" );
    FileC3->setMinimumSize( QSize( 88, 24 ) );
    FileC3->setText( tr( "Change..." ) );
    GroupBox200Layout->addWidget( FileC3, 2, 2 );

    tabLayout->addMultiCellWidget( GroupBox200, 2, 2, 0, 1 );

    TabWidget3->addWidget( tab, 0 );

    tab_7 = new QWidget( TabWidget3, "tab_7" );
    tabLayout_7 = new QHBoxLayout( tab_7, 11, 6, "tabLayout_7");
    Layout21 = new QVBoxLayout( 0, 0, 6, "Layout21");
    GroupSize = new QButtonGroup( tab_7, "GroupSize" );
    GroupSize->setTitle( tr( "Page Size" ) );
    GroupSize->setColumnLayout(0, Qt::Vertical );
    GroupSize->layout()->setSpacing( 6 );
    GroupSize->layout()->setMargin( 10 );
    GroupSizeLayout = new QVBoxLayout( GroupSize->layout() );
    GroupSizeLayout->setAlignment( Qt::AlignTop );
    Layout6 = new QGridLayout( 0, 1, 1, 0, 6, "Layout6");
    GZText2 = new QLabel( GroupSize, "GZText2" );
    GZText2->setText( tr( "Orientation:" ) );
    Layout6->addWidget( GZText2, 1, 0 );
    GZComboF = new QComboBox( true, GroupSize, "GZComboF" );
    GZComboF->insertItem("A0");
    GZComboF->insertItem("A1");
    GZComboF->insertItem("A2");
    GZComboF->insertItem("A3");
    GZComboF->insertItem("A4");
    GZComboF->insertItem("A5");
    GZComboF->insertItem("A6");
    GZComboF->insertItem("A7");
    GZComboF->insertItem("A8");
    GZComboF->insertItem("A9");
    GZComboF->insertItem("B0");
    GZComboF->insertItem("B1");
    GZComboF->insertItem("B2");
    GZComboF->insertItem("B3");
    GZComboF->insertItem("B4");
    GZComboF->insertItem("B5");
    GZComboF->insertItem("B6");
    GZComboF->insertItem("B7");
    GZComboF->insertItem("B8");
    GZComboF->insertItem("B9");
    GZComboF->insertItem("B10");
    GZComboF->insertItem("C5E");
    GZComboF->insertItem("Comm10E");
    GZComboF->insertItem("DLE");
    GZComboF->insertItem("Executive");
    GZComboF->insertItem("Folio");
    GZComboF->insertItem("Ledger");
    GZComboF->insertItem("Legal");
    GZComboF->insertItem("Letter");
    GZComboF->insertItem("Tabloid");
    GZComboF->insertItem( tr( "Custom" ) );
		GZComboF->setEditable(false);
		GZComboF->setCurrentItem(Vor->PageFormat);
    Layout6->addWidget( GZComboF, 0, 1 );

    GZComboO = new QComboBox( true, GroupSize, "GZComboO" );
    GZComboO->insertItem( tr( "Portrait" ) );
    GZComboO->insertItem( tr( "Landscape" ) );
		GZComboO->setEditable(false);
		GZComboO->setCurrentItem(Vor->Ausrichtung);

    Layout6->addWidget( GZComboO, 1, 1 );

    GZText1 = new QLabel( GroupSize, "GZText1" );
    GZText1->setText( tr( "Size:" ) );

    Layout6->addWidget( GZText1, 0, 0 );
    GroupSizeLayout->addLayout( Layout6 );

    Layout5_2 = new QHBoxLayout( 0, 0, 6, "Layout5_2");

    GZText3 = new QLabel( GroupSize, "GZText3" );
    GZText3->setText( tr( "Width:" ) );
    Layout5_2->addWidget( GZText3 );

		Breite = new MSpinBox( GroupSize, 2 );
    Breite->setEnabled( false );
    Breite->setMinimumSize( QSize( 70, 20 ) );
    Breite->setMaxValue( 1000000 );
    Breite->setMinValue( 1 );
    Layout5_2->addWidget( Breite );

    GZText4 = new QLabel( GroupSize, "GZText4" );
    GZText4->setText( tr( "Height:" ) );
    Layout5_2->addWidget( GZText4 );

		Hoehe = new MSpinBox( GroupSize, 2 );
    Hoehe->setEnabled( false );
    Hoehe->setMinimumSize( QSize( 70, 20 ) );
    Hoehe->setMaxValue( 1000000 );
    Hoehe->setMinValue( 1 );
    Layout5_2->addWidget( Hoehe );
    GroupSizeLayout->addLayout( Layout5_2 );
    Breite->setValue(qRound(Vor->PageBreite * Umrech) * 100);
    Hoehe->setValue(qRound(Vor->PageHoehe * Umrech) * 100);
    Breite->setLineStep(100);
    Hoehe->setLineStep(100);

    Layout8 = new QHBoxLayout( 0, 0, 6, "Layout8");

    Doppelseiten = new QCheckBox( GroupSize, "Doppelseiten" );
    Doppelseiten->setText( tr( "Facing Pages" ) );
		Doppelseiten->setChecked(Vor->DoppelSeiten);
    Layout8->addWidget( Doppelseiten );

    Linkszuerst = new QCheckBox( GroupSize, "Linkszuerst" );
    Linkszuerst->setText( tr( "Left Page First" ) );
		Linkszuerst->setChecked(Vor->ErsteLinks);
		if (!Doppelseiten->isChecked())
			Linkszuerst->setEnabled(false);
    Layout8->addWidget( Linkszuerst );
    GroupSizeLayout->addLayout( Layout8 );
    Layout21->addWidget( GroupSize );

    GroupRand = new QGroupBox( tab_7, "GroupRand" );
    GroupRand->setTitle( tr( "Margin Guides" ) );
    GroupRand->setColumnLayout(0, Qt::Vertical );
    GroupRand->layout()->setSpacing( 5 );
    GroupRand->layout()->setMargin( 10 );
    GroupRandLayout = new QGridLayout( GroupRand->layout() );
    GroupRandLayout->setAlignment( Qt::AlignTop );

		TopR = new MSpinBox( GroupRand, 2 );
    TopR->setMinimumSize( QSize( 70, 20 ) );
    TopR->setMaxValue( 100000 );
    TopR->setMinValue( 0 );
    TopR->setValue( qRound(Vor->RandOben * Umrech * 100));
		RandT = Vor->RandOben;
    GroupRandLayout->addWidget( TopR, 0, 1 );
    BottomR = new MSpinBox( GroupRand, 2 );
    BottomR->setMinimumSize( QSize( 70, 20 ) );
    BottomR->setMaxValue( 100000 );
    BottomR->setMinValue( 0 );
    BottomR->setValue( qRound(Vor->RandUnten * Umrech * 100));
		RandB = Vor->RandUnten;
    GroupRandLayout->addWidget( BottomR, 1, 1 );
    RightR = new MSpinBox( GroupRand, 2 );
    RightR->setMinimumSize( QSize( 70, 20 ) );
    RightR->setMaxValue( 100000 );
    RightR->setMinValue( 0 );
    RightR->setValue( qRound(Vor->RandRechts * Umrech * 100));
		RandR = Vor->RandRechts;
    GroupRandLayout->addWidget( RightR, 1, 3 );
    LeftR = new MSpinBox( GroupRand, 2 );
    LeftR->setMinimumSize( QSize( 70, 20 ) );
    LeftR->setMaxValue( 100000 );
    LeftR->setMinValue( 0 );
    LeftR->setValue( qRound(Vor->RandLinks * Umrech * 100));
		RandL = Vor->RandLinks;
    GroupRandLayout->addWidget( LeftR, 0, 3 );
    Breite->setLineStep(100);
    Hoehe->setLineStep(100);
    TopR->setLineStep(100);
    LeftR->setLineStep(100);
    RightR->setLineStep(100);
   	BottomR->setLineStep(100);

    GRText2 = new QLabel( GroupRand, "GRText2" );
    GRText2->setText( tr( "Bottom:" ) );
    GroupRandLayout->addWidget( GRText2, 1, 0 );
    GRText1 = new QLabel( GroupRand, "GRText1" );
    GRText1->setText( tr( "Top:" ) );
    GroupRandLayout->addWidget( GRText1, 0, 0 );
    GRText4 = new QLabel( GroupRand, "GRText4" );
    GRText4->setText( tr( "Right:" ) );
    GroupRandLayout->addWidget( GRText4, 1, 2 );
    GRText3 = new QLabel( GroupRand, "GRText3" );
    GRText3->setText( tr( "Left:" ) );
    GroupRandLayout->addWidget( GRText3, 0, 2 );

    Layout21->addWidget( GroupRand );
    tabLayout_7->addLayout( Layout21 );

    Layout21b = new QVBoxLayout( 0, 0, 6, "Layout21b");
    GroupAS = new QGroupBox( tab_7, "GroupAS" );
    GroupAS->setTitle( tr( "Autosave" ) );
    GroupAS->setColumnLayout(0, Qt::Vertical );
    GroupAS->layout()->setSpacing( 5 );
    GroupAS->layout()->setMargin( 10 );
    GroupASLayout = new QGridLayout( GroupAS->layout() );
    GroupASLayout->setAlignment( Qt::AlignTop );
    ASon = new QCheckBox( GroupAS, "Enable Autosave" );
    ASon->setText( tr( "Enabled" ) );
    ASon->setChecked(ap->HaveDoc ? ap->doc->AutoSave : Vor->AutoSave);
    GroupASLayout->addMultiCellWidget( ASon, 0, 0, 0, 1 );
    ASText = new QLabel( GroupAS, "ASText" );
    ASText->setText( tr( "Interval:" ) );
    GroupASLayout->addWidget( ASText, 1, 0 );
    ASTime = new QSpinBox( GroupAS, "Time" );
    ASTime->setMaxValue( 60 );
    ASTime->setMinValue( 1 );
    ASTime->setSuffix( " " + tr("min") );
		ASTime->setValue(Vor->AutoSaveTime / 1000 / 60);
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
    GroupBox1 = new QGroupBox( tab_2, "GroupBox1" );
    GroupBox1->setTitle( tr( "Grid Layout" ) );
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
    TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );
    TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel1->sizePolicy().hasHeightForWidth() ) );
    TextLabel1->setText( tr( "Minor Grid Spacing:" ) );
    Layout10->addWidget( TextLabel1, 0, 0 );
    TextLabel2 = new QLabel( GroupBox1, "TextLabel2" );
    TextLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2->setText( tr( "Major Grid Spacing:" ) );
    Layout10->addWidget( TextLabel2, 1, 0 );
    SpinBox1 = new MSpinBox( GroupBox1, 2 );
    SpinBox1->setMaxValue( 100000 );
    SpinBox1->setMinValue( 1 );
    Layout10->addWidget( SpinBox1, 0, 1 );
    SpinBox2 = new MSpinBox( GroupBox1, 2 );
    SpinBox2->setMaxValue( 100000 );
    SpinBox2->setMinValue( 10 );
    SpinBox1->setLineStep(100);
    SpinBox2->setLineStep(100);
    Layout10->addWidget( SpinBox2, 1, 1 );
    TextLabel2g = new QLabel( GroupBox1, "TextLabel2" );
    TextLabel2g->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2g->setText( tr( "Guide Snap Distance:" ) );
    Layout10->addWidget( TextLabel2g, 2, 0 );
    SpinBox2g = new MSpinBox( GroupBox1, 2 );
    SpinBox2g->setMaxValue( 100000 );
    SpinBox2g->setMinValue( 1 );
    SpinBox2g->setLineStep(100);
    Layout10->addWidget( SpinBox2g, 2, 1 );
		if (ap->HaveDoc)
			{
    	SpinBox2->setValue( qRound(ap->doc->majorGrid * Umrech * 100));
    	SpinBox1->setValue( qRound(ap->doc->minorGrid * Umrech * 100));
    	SpinBox2g->setValue( qRound(ap->doc->GuideRad * Umrech * 100));
			}
		else
			{
    	SpinBox2->setValue( qRound(Vor->DmajGrid * Umrech * 100));
    	SpinBox1->setValue( qRound(Vor->DminGrid * Umrech * 100));
    	SpinBox2g->setValue( qRound(Vor->GuideRad * Umrech * 100));
			}
    GroupBox1Layout->addLayout( Layout10 );
    tabLayout_2->addWidget( GroupBox1, 0, 0 );
    GroupBox2 = new QGroupBox( tab_2, "GroupBox2" );
    GroupBox2->setTitle( tr( "Grid Colors" ) );
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
    TextLabel3 = new QLabel( GroupBox2, "TextLabel3" );
    TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel3->sizePolicy().hasHeightForWidth() ) );
    TextLabel3->setText( tr( "Minor Grid Color:" ) );
    Layout11->addWidget( TextLabel3, 0, 0 );
    TextLabel4 = new QLabel( GroupBox2, "TextLabel4" );
    TextLabel4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel4->sizePolicy().hasHeightForWidth() ) );
    TextLabel4->setText( tr( "Major Grid Color:" ) );
    Layout11->addWidget( TextLabel4, 1, 0 );
    minColor = new QPushButton( GroupBox2, "minColor" );
    minColor->setText( tr( "" ) );
    minColor->setAutoDefault( false );
    QPixmap pm = QPixmap(40, 20);
		if (ap->HaveDoc)
			{
    	pm.fill(ap->doc->minorColor);
			Cmin = ap->doc->minorColor;
			}
		else
			{
    	pm.fill(Vor->DminColor);
			Cmin = Vor->DminColor;
			}
    minColor->setPixmap(pm);
    Layout11->addWidget( minColor, 0, 1 );
    MaxColor = new QPushButton( GroupBox2, "MaxColor" );
    MaxColor->setText( tr( "" ) );
    MaxColor->setAutoDefault( false );
    QPixmap pm1 = QPixmap(40, 20);
		if (ap->HaveDoc)
			{
    	pm1.fill(ap->doc->majorColor);
			Cmax = ap->doc->majorColor;
			}
		else
			{
    	pm1.fill(Vor->DmajColor);
			Cmax = Vor->DmajColor;
			}
    MaxColor->setPixmap(pm1);
    Layout11->addWidget( MaxColor, 1, 1 );
    TextLabel4g = new QLabel( GroupBox2, "TextLabel4g" );
    TextLabel4g->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel4->sizePolicy().hasHeightForWidth() ) );
    TextLabel4g->setText( tr( "User Guides Color:" ) );
    Layout11->addWidget( TextLabel4g, 2, 0 );
    GuideColor = new QPushButton( GroupBox2, "GuideColor" );
    GuideColor->setText( tr( "" ) );
    GuideColor->setAutoDefault( false );
    QPixmap pm12 = QPixmap(40, 20);
		if (ap->HaveDoc)
			{
    	pm12.fill(ap->doc->guideColor);
			Cgui = ap->doc->guideColor;
			}
		else
			{
    	pm12.fill(Vor->guideColor);
			Cgui = Vor->guideColor;
			}
    GuideColor->setPixmap(pm12);
    Layout11->addWidget( GuideColor, 2, 1 );
    GroupBox2Layout->addLayout( Layout11 );
    tabLayout_2->addWidget( GroupBox2, 1, 0 );
    ButtonGroup5 = new QButtonGroup( tab_2, "ButtonGroup5" );
    ButtonGroup5->setTitle( tr( "Placing" ) );
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
    RadioButton6 = new QRadioButton( ButtonGroup5, "RadioButton6" );
    RadioButton6->setText( tr( "In the Background" ) );
    RadioButton6->setChecked(ap->HaveDoc ? ap->doc->Before : Vor->Before );
    Layout7->addWidget( RadioButton6 );
    RadioButton7 = new QRadioButton( ButtonGroup5, "RadioButton6_2" );
    RadioButton7->setText( tr( "In the Foreground" ) );
    RadioButton7->setChecked(ap->HaveDoc ? !ap->doc->Before : !Vor->Before );
    Layout7->addWidget( RadioButton7 );
    ButtonGroup5Layout->addLayout( Layout7 );
    tabLayout_2->addWidget( ButtonGroup5, 0, 1 );
    TabWidget3->addWidget( tab_2, 2 );

    tab_4 = new QWidget( TabWidget3, "tab_4" );
    tabLayout_3 = new QGridLayout( tab_4 );
    tabLayout_3->setSpacing( 6 );
    tabLayout_3->setMargin( 11 );
    GroupBox4_2 = new QGroupBox( tab_4, "GroupBox4_2" );
    GroupBox4_2->setTitle( tr( "Subscript" ) );
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
    TextLabel2_3_2 = new QLabel( GroupBox4_2, "TextLabel2_3_2" );
    TextLabel2_3_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel2_3_2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2_3_2->setText( tr( "Scaling:" ) );
    Layout12->addWidget( TextLabel2_3_2, 1, 0 );
    TextLabel1_5_2 = new QLabel( GroupBox4_2, "TextLabel1_5_2" );
    TextLabel1_5_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel1_5_2->sizePolicy().hasHeightForWidth() ) );
    TextLabel1_5_2->setText( tr( "Disp.:" ) );
    Layout12->addWidget( TextLabel1_5_2, 0, 0 );
    VTiefW = new QSpinBox( GroupBox4_2, "VTiefW" );
    VTiefW->setSuffix( tr( " %" ) );
    VTiefW->setMaxValue( 100 );
    VTiefW->setMinValue( 1 );
    VTiefW->setValue(ap->HaveDoc ? ap->doc->VTief : Vor->DVTief );
    Layout12->addWidget( VTiefW, 0, 1 );
    VTiefWSc = new QSpinBox( GroupBox4_2, "VTiefWSc" );
    VTiefWSc->setSuffix( tr( " %" ) );
    VTiefWSc->setMaxValue( 100 );
    VTiefWSc->setMinValue( 1 );
    VTiefWSc->setValue(ap->HaveDoc ? ap->doc->VTiefSc : Vor->DVTiefSc );
    Layout12->addWidget( VTiefWSc, 1, 1 );
    GroupBox4_2Layout->addLayout( Layout12 );
    tabLayout_3->addWidget( GroupBox4_2, 1, 0 );
    GroupBox4 = new QGroupBox( tab_4, "GroupBox4" );
    GroupBox4->setTitle( tr( "Superscript" ) );
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
    TextLabel2_3 = new QLabel( GroupBox4, "TextLabel2_3" );
    TextLabel2_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel2_3->sizePolicy().hasHeightForWidth() ) );
    TextLabel2_3->setText( tr( "Scaling:" ) );
    Layout13->addWidget( TextLabel2_3, 1, 0 );
    TextLabel1_5 = new QLabel( GroupBox4, "TextLabel1_5" );
    TextLabel1_5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel1_5->sizePolicy().hasHeightForWidth() ) );
    TextLabel1_5->setText( tr( "Disp.:" ) );
    Layout13->addWidget( TextLabel1_5, 0, 0 );
    VHochW = new QSpinBox( GroupBox4, "VHochW" );
    VHochW->setSuffix( tr( " %" ) );
    VHochW->setMaxValue( 100 );
    VHochW->setMinValue( 1 );
    VHochW->setValue(ap->HaveDoc ? ap->doc->VHoch : Vor->DVHoch );
    Layout13->addWidget( VHochW, 0, 1 );
    VHochWSc = new QSpinBox( GroupBox4, "VHochWSc" );
    VHochWSc->setSuffix( tr( " %" ) );
    VHochWSc->setMaxValue( 100 );
    VHochWSc->setMinValue( 1 );
    VHochWSc->setValue(ap->HaveDoc ? ap->doc->VHochSc : Vor->DVHochSc );
    Layout13->addWidget( VHochWSc, 1, 1 );
    GroupBox4Layout->addLayout( Layout13 );
    tabLayout_3->addWidget( GroupBox4, 0, 0 );
    GroupBox4_3 = new QGroupBox( tab_4, "GroupBox4_3" );
    GroupBox4_3->setTitle( tr( "Small Caps" ) );
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
    TextLabel2_3_3 = new QLabel( GroupBox4_3, "TextLabel2_3_3" );
    TextLabel2_3_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel2_3_3->sizePolicy().hasHeightForWidth() ) );
    TextLabel2_3_3->setText( tr( "Scaling:" ) );
    Layout14->addWidget( TextLabel2_3_3 );
    SmallCaps = new QSpinBox( GroupBox4_3, "SmallCaps" );
    SmallCaps->setSuffix( tr( " %" ) );
    SmallCaps->setMaxValue( 100 );
    SmallCaps->setMinValue( 1 );
    SmallCaps->setValue(ap->HaveDoc ? ap->doc->VKapit : Vor->DVKapit );
    Layout14->addWidget( SmallCaps );
    GroupBox4_3Layout->addLayout( Layout14 );
    tabLayout_3->addWidget( GroupBox4_3, 0, 1 );

    GroupBox4_3a = new QGroupBox( tab_4, "GroupBox4_3a" );
    GroupBox4_3a->setTitle( tr( "Other" ) );
    GroupBox4_3a->setColumnLayout(0, Qt::Vertical );
    GroupBox4_3a->layout()->setSpacing( 0 );
    GroupBox4_3a->layout()->setMargin( 0 );
    GroupBox4_3aLayout = new QHBoxLayout( GroupBox4_3a->layout() );
    GroupBox4_3aLayout->setAlignment( Qt::AlignTop );
    GroupBox4_3aLayout->setSpacing( 6 );
    GroupBox4_3aLayout->setMargin( 11 );
    Layout14a = new QHBoxLayout;
    Layout14a->setSpacing( 6 );
    Layout14a->setMargin( 0 );
    TextLabel2_3_3a = new QLabel( GroupBox4_3a, "TextLabel2_3_3" );
    TextLabel2_3_3a->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel2_3_3a->sizePolicy().hasHeightForWidth() ) );
    TextLabel2_3_3a->setText( tr( "Automatic Line Spacing:" ) );
    Layout14a->addWidget( TextLabel2_3_3a );
    AutoLineV = new QSpinBox( GroupBox4_3a, "LineS" );
    AutoLineV->setSuffix( tr( " %" ) );
    AutoLineV->setMaxValue( 100 );
    AutoLineV->setMinValue( 1 );
    AutoLineV->setValue(ap->HaveDoc ? ap->doc->AutoLine : Vor->AutoLine );
    Layout14a->addWidget( AutoLineV );
    GroupBox4_3aLayout->addLayout( Layout14a );
    tabLayout_3->addWidget( GroupBox4_3a, 1, 1 );

    TabWidget3->addWidget( tab_4, 3 );

    tab_3 = new QWidget( TabWidget3, "tab_3" );
    tabLayout_4 = new QHBoxLayout( tab_3 );
    tabLayout_4->setSpacing( 0 );
    tabLayout_4->setMargin( 10 );
    ButtonGroup2 = new QButtonGroup( tab_3, "ButtonGroup2" );
    ButtonGroup2->setTitle( tr( "" ) );
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
    TextButton->setText( tr( "" ) );
    TextButton->setPixmap(loadIcon("Text.xpm"));
    TextButton->setToggleButton( true );
    TextButton->setOn( true );
    ButtonGroup2->insert( TextButton, -1 );
    Layout16->addWidget( TextButton, 0, 0 );
    BildButton = new QToolButton( ButtonGroup2, "BildButton" );
    BildButton->setText( tr( "" ) );
    BildButton->setPixmap(loadIcon("Bild.xpm"));
    BildButton->setToggleButton( true );
    ButtonGroup2->insert( BildButton, -1 );
    Layout16->addWidget( BildButton, 0, 1 );
    RectButton = new QToolButton( ButtonGroup2, "RectButton" );
    RectButton->setText( tr( "" ) );
    RectButton->setPixmap(loadIcon("Rechtecke.xpm") );
    RectButton->setToggleButton( true );
    ButtonGroup2->insert( RectButton, -1 );
    Layout16->addWidget( RectButton, 1, 0 );
    OvalButton = new QToolButton( ButtonGroup2, "OvalButton" );
    OvalButton->setText( tr( "" ) );
    OvalButton->setPixmap(loadIcon("Kreise.xpm"));
    OvalButton->setToggleButton( true );
    ButtonGroup2->insert( OvalButton, -1 );
    Layout16->addWidget( OvalButton, 1, 1 );
    ZoomButton = new QToolButton( ButtonGroup2, "ZoomButton" );
    ZoomButton->setText( tr( "" ) );
    ZoomButton->setPixmap(loadIcon("Lupe.xpm"));
    ZoomButton->setToggleButton( true );
    ButtonGroup2->insert( ZoomButton, -1 );
    Layout16->addWidget( ZoomButton, 2, 0 );
    LineButton = new QToolButton( ButtonGroup2, "LineButton" );
    LineButton->setText( tr( "" ) );
    LineButton->setPixmap(loadIcon("Stift.xpm"));
    LineButton->setToggleButton( true );
    ButtonGroup2->insert( LineButton, -1 );
    Layout16->addWidget( LineButton, 2, 1 );
		PolyButton = new QToolButton( ButtonGroup2, "PolyButton" );
    PolyButton->setText( tr( "" ) );
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
    TextLabel2_2 = new QLabel( ToolFrame, "TextLabel2_2" );
    TextLabel2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel2_2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2_2->setText( tr( "Default Size:" ) );
    Layout15a->addWidget( TextLabel2_2, 1, 0 );
    TextLabel1_3 = new QLabel( ToolFrame, "TextLabel1_3" );
    TextLabel1_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel1_3->sizePolicy().hasHeightForWidth() ) );
    TextLabel1_3->setText( tr( "Default Font:" ) );
    Layout15a->addWidget( TextLabel1_3, 0, 0 );
    FontComb = new FontCombo(ToolFrame, Vor);
		if (ap->HaveDoc)
			{
    	for (int fc=0; fc<FontComb->count(); ++fc)
    		{
    		if (FontComb->text(fc) == ap->doc->Dfont)
    			{
    			FontComb->setCurrentItem(fc);
    			break;
    			}
    		}
			}
		else
			{
    	for (int fc=0; fc<FontComb->count(); ++fc)
    		{
    		if (FontComb->text(fc) == Vor->DefFont)
    			{
    			FontComb->setCurrentItem(fc);
    			break;
    			}
    		}
			}
    Layout15a->addWidget( FontComb, 0, 1 );
    SizeCombo = new QComboBox( true, ToolFrame, "SizeCombo" );
    SizeCombo->setEditable(false);
		SizeCombo->insertItem(" 7 pt");
		SizeCombo->insertItem(" 9 pt");
		SizeCombo->insertItem("10 pt");
		SizeCombo->insertItem("12 pt");
		SizeCombo->insertItem("14 pt");
		SizeCombo->insertItem("18 pt");
		SizeCombo->insertItem("24 pt");
		SizeCombo->insertItem("36 pt");
		SizeCombo->insertItem("48 pt");
		SizeCombo->insertItem("60 pt");
		SizeCombo->insertItem("72 pt");
		int a;
		if (ap->HaveDoc)
			{
			for (a = 0; a < SizeCombo->count(); ++a)
				{
				if (SizeCombo->text(a).left(2).toInt() == ap->doc->Dsize)
					SizeCombo->setCurrentItem(a);
				}
			}
		else
			{
			for (a = 0; a < SizeCombo->count(); ++a)
				{
				if (SizeCombo->text(a).left(2).toInt() == Vor->DefSize)
					SizeCombo->setCurrentItem(a);
				}
			}
    Layout15a->addWidget( SizeCombo, 1, 1 );
    TextLabelT1 = new QLabel( ToolFrame, "TextLabel1_3_2" );
    TextLabelT1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT1->sizePolicy().hasHeightForWidth() ) );
    TextLabelT1->setText( tr( "Text Color:" ) );
    Layout15a->addWidget( TextLabelT1, 2, 0 );
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
    		{
    		ForegroundT->setCurrentItem(ForegroundT->count()-1);
    		}
			}
    Layout15a->addWidget( ForegroundT, 2, 1 );
    TextLabel1_4 = new QLabel( ToolFrame, "TextLabel1_4" );
    TextLabel1_4->setMinimumSize(QSize(260, 70));
    TextLabel1_4->setMaximumSize(QSize(260, 70));
    TextLabel1_4->setText( tr( "Woven silk pyjamas exchanged for blue quartz" ) );
    TextLabel1_4->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
    SetSample();
    Layout15a->addMultiCellWidget( TextLabel1_4, 3, 3, 0, 1 );
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
    TextLabelT21 = new QLabel( ToolFrame2, "TextLabel1_3_2" );
    TextLabelT21->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT21->sizePolicy().hasHeightForWidth() ) );
    TextLabelT21->setText( tr( "Line Color:" ) );
    Layout15b->addWidget(TextLabelT21, 0, 0);
    TextLabelT22 = new QLabel( ToolFrame2, "TextLabel2_2" );
    TextLabelT22->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT22->sizePolicy().hasHeightForWidth() ) );
    TextLabelT22->setText( tr( "Shading:" ) );
    Layout15b->addWidget(TextLabelT22, 1, 0);
    TextLabelT23 = new QLabel( ToolFrame2, "TextLabel1_3" );
    TextLabelT23->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT23->sizePolicy().hasHeightForWidth() ) );
    TextLabelT23->setText( tr( "Fill Color:" ) );
    Layout15b->addWidget(TextLabelT23, 2, 0);
    TextLabelT24 = new QLabel( ToolFrame2, "TextLabel2_2_2" );
    TextLabelT24->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT24->sizePolicy().hasHeightForWidth() ) );
    TextLabelT24->setText( tr( "Shading:" ) );
    Layout15b->addWidget(TextLabelT24, 3, 0);
    TextLabelT25 = new QLabel( ToolFrame2, "TextLabel1_3_3" );
    TextLabelT25->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT25->sizePolicy().hasHeightForWidth() ) );
    TextLabelT25->setText( tr( "Linestyle:" ) );
    Layout15b->addWidget(TextLabelT25, 4, 0);
    TextLabelT26 = new QLabel( ToolFrame2, "TextLabel2_3_4" );
    TextLabelT26->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT26->sizePolicy().hasHeightForWidth() ) );
    TextLabelT26->setText( tr( "Line Width:" ) );
    Layout15b->addWidget(TextLabelT26, 5, 0);
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
    		{
    		Foreground->setCurrentItem(Foreground->count()-1);
    		}
			}
    Layout15b->addWidget(Foreground, 0, 1);
    Shade2 = new QSpinBox( ToolFrame2, "Shade2" );
    Shade2->setSuffix( tr( " %" ) );
    Shade2->setMaxValue( 100 );
    Shade2->setMinValue( 0 );
    Shade2->setValue(ap->HaveDoc ? ap->doc->Dshade2 : Vor->Dshade2 );
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
    		{
    		Background->setCurrentItem(Background->count()-1);
    		}
			}
    Layout15b->addWidget(Background, 2, 1);
    Shade = new QSpinBox( ToolFrame2, "Shade" );
    Shade->setSuffix( tr( " %" ) );
    Shade->setMaxValue( 100 );
    Shade->setMinValue( 0 );
    Shade->setValue(ap->HaveDoc ? ap->doc->Dshade : Vor->Dshade );
    Layout15b->addWidget(Shade, 3, 1);
    Linestyle = new LineCombo(ToolFrame2);
    Linestyle->setEditable(false);
		int dla;
		dla = ap->HaveDoc ? ap->doc->DLineArt : Vor->DLineArt;
		switch (dla)
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
    Layout15b->addWidget(Linestyle, 4, 1);
    LineW = new MSpinBox( ToolFrame2, 1 );
    LineW->setSuffix( tr( " pts" ) );
    LineW->setMaxValue( 200 );
    LineW->setMinValue( 0 );
		LineW->setLineStep(10);
		LineW->setValue(ap->HaveDoc ? static_cast<int>(ap->doc->Dwidth*10) : static_cast<int>(Vor->Dwidth*10));
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
    TextLabelT31 = new QLabel( ToolFrame3, "TextLabel1_3_2" );
    TextLabelT31->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT31->sizePolicy().hasHeightForWidth() ) );
    TextLabelT31->setText( tr( "Line Color:" ) );
    Layout15c->addWidget(TextLabelT31, 0, 0);
    TextLabelT32 = new QLabel( ToolFrame3, "TextLabel2_2" );
    TextLabelT32->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT32->sizePolicy().hasHeightForWidth() ) );
    TextLabelT32->setText( tr( "Shading:" ) );
    Layout15c->addWidget(TextLabelT32, 1, 0);
    TextLabelT35 = new QLabel( ToolFrame3, "TextLabel1_3_3" );
    TextLabelT35->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT35->sizePolicy().hasHeightForWidth() ) );
    TextLabelT35->setText( tr( "Line Style:" ) );
    Layout15c->addWidget(TextLabelT35, 2, 0);
    TextLabelT36 = new QLabel( ToolFrame3, "TextLabel2_3_4" );
    TextLabelT36->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT36->sizePolicy().hasHeightForWidth() ) );
    TextLabelT36->setText( tr( "Line Width:" ) );
    Layout15c->addWidget(TextLabelT36, 3, 0);
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
    		{
    		Foreground2->setCurrentItem(Foreground2->count()-1);
    		}
			}
    Layout15c->addWidget(Foreground2, 0, 1);
    Shade22 = new QSpinBox( ToolFrame3, "Shade2" );
    Shade22->setSuffix( tr( " %" ) );
    Shade22->setMaxValue( 100 );
    Shade22->setMinValue( 0 );
    Shade22->setValue(ap->HaveDoc ? ap->doc->DshadeLine : Vor->DshadeLine );
    Layout15c->addWidget(Shade22, 1, 1);
    Linestyle2 = new LineCombo(ToolFrame3);
    Linestyle2->setEditable(false);
    dla = ap->HaveDoc ? ap->doc->DLstyleLine : Vor->DLstyleLine;
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
    Layout15c->addWidget(Linestyle2, 2, 1);
    LineW2 = new MSpinBox( ToolFrame3, 1 );
    LineW2->setSuffix( tr( " pts" ) );
    LineW2->setMaxValue( 200 );
    LineW2->setMinValue( 1 );
		LineW2->setLineStep(10);
		LineW2->setValue(ap->HaveDoc ? static_cast<int>(ap->doc->DwidthLine*10) : static_cast<int>(Vor->DwidthLine*10) );
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
    TextLabelT42 = new QLabel( ToolFrame4, "TextLabel2_2" );
    TextLabelT42->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT42->sizePolicy().hasHeightForWidth() ) );
    TextLabelT42->setText( tr( "Minimum:" ) );
    Layout15d->addWidget(TextLabelT42, 0, 0);
    TextLabelT43 = new QLabel( ToolFrame4, "TextLabel2_2_3" );
    TextLabelT43->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT43->sizePolicy().hasHeightForWidth() ) );
    TextLabelT43->setText( tr( "Maximum:" ) );
    Layout15d->addWidget(TextLabelT43, 1, 0);
    TextLabelT44 = new QLabel( ToolFrame4, "TextLabel2_3_4" );
    TextLabelT44->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT44->sizePolicy().hasHeightForWidth() ) );
    TextLabelT44->setText( tr( "Stepping:" ) );
    Layout15d->addWidget(TextLabelT44, 2, 0);
    MinMag = new QSpinBox( ToolFrame4, "Shade2" );
    MinMag->setSuffix( tr( " %" ) );
    MinMag->setMaxValue( 800 );
    MinMag->setMinValue( 10 );
    MinMag->setLineStep( 10 );
    MinMag->setValue(ap->HaveDoc ? ap->doc->MagMin : Vor->MagMin );
    Layout15d->addWidget(MinMag, 0, 1);
    MaxMag = new QSpinBox( ToolFrame4, "Shade" );
    MaxMag->setSuffix( tr( " %" ) );
    MaxMag->setMaxValue( 800 );
    MaxMag->setMinValue( 10 );
    MaxMag->setLineStep( 10 );
    MaxMag->setValue(ap->HaveDoc ? ap->doc->MagMax : Vor->MagMax );
    Layout15d->addWidget(MaxMag, 1, 1);
    StepMag = new QSpinBox( ToolFrame4, "LineW" );
    StepMag->setSuffix( tr( " %" ) );
    StepMag->setMaxValue( 200 );
    StepMag->setMinValue( 1 );
    StepMag->setLineStep( 25 );
    StepMag->setValue(ap->HaveDoc ? ap->doc->MagStep : Vor->MagStep );
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
    FreeScale = new QRadioButton( ToolFrame5, "FreeScale" );
    FreeScale->setText( tr( "Free Scaling" ) );
    Layout15->addMultiCellWidget( FreeScale, 0, 0, 0, 3 );
    TextLabelT51 = new QLabel( ToolFrame5, "TextLabelT51" );
    TextLabelT51->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT51->sizePolicy().hasHeightForWidth() ) );
    TextLabelT51->setText( tr( "Horizontal Scaling" ) );
    Layout15->addWidget( TextLabelT51, 1, 1 );
    TextLabelT52 = new QLabel( ToolFrame5, "TextLabelT52" );
    TextLabelT52->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT52->sizePolicy().hasHeightForWidth() ) );
    TextLabelT52->setText( tr( "Vertical Scaling" ) );
    Layout15->addWidget( TextLabelT52, 2, 1 );
    TextLabelT53 = new QLabel( ToolFrame5, "TextLabelT53" );
    TextLabelT53->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT53->sizePolicy().hasHeightForWidth() ) );
    TextLabelT53->setText( tr( "Fill Color:" ) );
    Layout15->addWidget( TextLabelT53, 5, 0 );
    TextLabelT54 = new QLabel( ToolFrame5, "TextLabelT54" );
    TextLabelT54->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabelT54->sizePolicy().hasHeightForWidth() ) );
    TextLabelT54->setText( tr( "Shading:" ) );
    Layout15->addWidget( TextLabelT54, 6, 0 );
    XScale = new QSpinBox( ToolFrame5, "XScale" );
    XScale->setSuffix( tr( " %" ) );
    XScale->setMaxValue( 1000 );
    XScale->setMinValue( 1 );
    XScale->setValue(ap->HaveDoc ? qRound(ap->doc->ScaleX * 100) : qRound(Vor->ScaleX * 100) );
    Layout15->addWidget( XScale, 1, 2 );
    YScale = new QSpinBox( ToolFrame5, "YScale" );
    YScale->setSuffix( tr( " %" ) );
    YScale->setMaxValue( 1000 );
    YScale->setMinValue( 1 );
    YScale->setValue(ap->HaveDoc ? qRound(ap->doc->ScaleY * 100) : qRound(Vor->ScaleY * 100) );
    Layout15->addWidget( YScale, 2, 2 );
    Kette = new LinkButton( ToolFrame5 );
    Kette->setToggleButton( true );
    Kette->setAutoRaise(true);
    Layout15->addMultiCellWidget( Kette, 1, 2, 3, 3 );
    FrameScale = new QRadioButton(ToolFrame5, "FrameScale" );
    FrameScale->setText( tr( "Scale Picture to Frame Size" ) );
    Layout15->addMultiCellWidget( FrameScale, 3, 3, 0, 3 );
    Aspect = new QCheckBox( ToolFrame5, "Aspect" );
    Aspect->setText( tr( "Keep Aspect Ratio" ) );
    Layout15->addMultiCellWidget( Aspect, 4, 4, 1, 3 );
    ShadeP = new QSpinBox( ToolFrame5, "ShadeP" );
    ShadeP->setSuffix( tr( " %" ) );
    ShadeP->setMaxValue( 100 );
    ShadeP->setMinValue( 0 );
    ShadeP->setValue(ap->HaveDoc ? ap->doc->ShadePict : Vor->ShadePict );
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
    		{
    		BackgroundP->setCurrentItem(BackgroundP->count()-1);
    		}
			}
    Layout15->addMultiCellWidget( BackgroundP, 5, 5, 1, 3 );
    QSpacerItem* sp01 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ToolFrame5Layout->addItem( sp01 );
    ToolFrame5Layout->addLayout( Layout15 );
    QSpacerItem* sp02 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ToolFrame5Layout->addItem( sp02 );
    Aspect->setChecked(ap->HaveDoc ? ap->doc->AspectRatio : Vor->AspectRatio);
		bool sct;
		sct = ap->HaveDoc ? ap->doc->ScaleType : Vor->ScaleType;
		if (sct)
			{
			FreeScale->setChecked(true);
			FrameScale->setChecked(false);
			}
		else
			{
			FreeScale->setChecked(false);
			FrameScale->setChecked(true);
			}
		ChangeScaling();
    Fram->addWidget(ToolFrame5, 5);

    ToolFrame6 = new QFrame( this, "ToolFrame6" );
    ToolFrame6->setFrameShape( QFrame::Box );
    ToolFrame6->setFrameShadow( QFrame::Sunken );
		Pre = new QPixmap(101, 101);
		Pre->fill(white);
    PolygonPropsLayout = new QHBoxLayout( ToolFrame6, 10, 5, "PolygonPropsLayout");
    T6_Layout11 = new QHBoxLayout( 0, 0, 5, "Layout11");
    T6_Layout10 = new QVBoxLayout( 0, 0, 5, "Layout10");
    T6_Layout2 = new QHBoxLayout( 0, 0, 5, "Layout2");
    T6_Text1 = new QLabel( ToolFrame6, "Text1" );
    T6_Text1->setText( tr( "Corners:" ) );
    T6_Layout2->addWidget( T6_Text1 );
    T6_Ecken = new QSpinBox( ToolFrame6, "Ecken" );
    T6_Ecken->setMaxValue( 999 );
    T6_Ecken->setMinValue( 3 );
		T6_Ecken->setValue(Vor->PolyC);
    T6_Layout2->addWidget( T6_Ecken );
    T6_Layout10->addLayout( T6_Layout2 );
    T6_Layout9_2 = new QHBoxLayout( 0, 0, 5, "Layout9_2");
    T6_Layout8_2 = new QVBoxLayout( 0, 0, 5, "Layout8_2");
    T6_Layout7_2 = new QHBoxLayout( 0, 0, 5, "Layout7_2");
    T6_Text2_2 = new QLabel( ToolFrame6, "Text2_2" );
    T6_Text2_2->setText( tr( "Rotation:" ) );
    T6_Layout7_2->addWidget( T6_Text2_2 );
    T6_Faktor2 = new QSpinBox( ToolFrame6, "Faktor_2" );
    T6_Faktor2->setSuffix(" °");
    T6_Faktor2->setMaxValue( 180 );
    T6_Faktor2->setMinValue( -180 );
		T6_Faktor2->setValue(static_cast<int>(Vor->PolyR));
    T6_Layout7_2->addWidget( T6_Faktor2 );
    T6_Layout8_2->addLayout( T6_Layout7_2 );
    T6_Slider2 = new QSlider( ToolFrame6, "Slider1_2" );
    T6_Slider2->setMinValue( -180 );
    T6_Slider2->setMaxValue( 180 );
		T6_Slider2->setValue(static_cast<int>(Vor->PolyR));
    T6_Slider2->setOrientation( QSlider::Horizontal );
    T6_Slider2->setTickmarks( QSlider::Right );
    T6_Layout8_2->addWidget( T6_Slider2 );
    T6_Layout9_2->addLayout( T6_Layout8_2 );
    T6_Layout10->addLayout( T6_Layout9_2 );
    T6_Konvex = new QCheckBox( ToolFrame6, "Konvex" );
    T6_Konvex->setText( tr( "Convex Polygon" ) );
		T6_Konvex->setChecked(Vor->PolyS);
    T6_Layout10->addWidget( T6_Konvex );
    T6_Layout9 = new QHBoxLayout( 0, 0, 5, "Layout9");
    QSpacerItem* T6_spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    T6_Layout9->addItem( T6_spacer );
    T6_Layout8 = new QVBoxLayout( 0, 0, 5, "Layout8");
    T6_Layout7 = new QHBoxLayout( 0, 0, 5, "Layout7");
    T6_Text2 = new QLabel( ToolFrame6, "Text2" );
    T6_Text2->setText( tr( "Factor:" ) );
    T6_Layout7->addWidget( T6_Text2 );
    T6_Faktor = new QSpinBox( ToolFrame6, "Faktor" );
    T6_Faktor->setSuffix( tr( " %" ) );
    T6_Faktor->setMaxValue( 100 );
    T6_Faktor->setMinValue( -100 );
		T6_Faktor->setValue(Vor->PolyFd);
    T6_Layout7->addWidget( T6_Faktor );
    T6_Layout8->addLayout( T6_Layout7 );
    T6_Slider1 = new QSlider( ToolFrame6, "Slider1" );
    T6_Slider1->setMinValue( -100 );
    T6_Slider1->setMaxValue( 100 );
    T6_Slider1->setOrientation( QSlider::Horizontal );
    T6_Slider1->setTickmarks( QSlider::Right );
		T6_Slider1->setValue(Vor->PolyFd);
    T6_Layout8->addWidget( T6_Slider1 );
    T6_Layout9->addLayout( T6_Layout8 );
    T6_Layout10->addLayout( T6_Layout9 );
    T6_Layout11->addLayout( T6_Layout10 );
    T6_Preview = new QLabel( ToolFrame6, "Preview" );
    T6_Preview->setMinimumSize( QSize( 106, 106 ) );
    T6_Preview->setMaximumSize( QSize( 106, 106 ) );
    T6_Preview->setFrameShape( QLabel::Panel );
    T6_Preview->setFrameShadow( QLabel::Sunken );
		T6_Preview->setLineWidth(2);
		T6_Preview->setAlignment(AlignCenter);
    T6_Preview->setPixmap(*Pre);
    T6_Layout11->addWidget( T6_Preview );
    QSpacerItem* T6_spacer3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    PolygonPropsLayout->addItem( T6_spacer3 );
    PolygonPropsLayout->addLayout( T6_Layout11 );
    QSpacerItem* T6_spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    PolygonPropsLayout->addItem( T6_spacer2 );
    UpdatePreView();
    Fram->addWidget(ToolFrame6, 6);

    TabWidget3->addWidget( tab_3, 4 );

    tab_5 = new QWidget( TabWidget3, "tab_5" );
    tabLayout_5 = new QGridLayout( tab_5 );
    tabLayout_5->setSpacing( 6 );
    tabLayout_5->setMargin( 11 );

    Saving = new QGroupBox( tab_5, "Saving" );
    Saving->setTitle( tr( "Other Options" ) );
    Saving->setColumnLayout(0, Qt::Vertical );
    Saving->layout()->setSpacing( 0 );
    Saving->layout()->setMargin( 0 );
    SavingLayout = new QHBoxLayout( Saving->layout() );
    SavingLayout->setAlignment( Qt::AlignTop );
    SavingLayout->setSpacing( 6 );
    SavingLayout->setMargin( 25 );

    SaveAtQuit = new QCheckBox( Saving, "SaveAtQuit" );
    SaveAtQuit->setText( tr( "Save Contents on Exit" ) );
    SaveAtQuit->setChecked(Vor->SaveAtQ);
    SavingLayout->addWidget( SaveAtQuit );

    tabLayout_5->addWidget( Saving, 1, 0 );

    Preview = new QGroupBox( tab_5, "Preview" );
    Preview->setTitle( tr( "Preview" ) );
    Preview->setColumnLayout(0, Qt::Vertical );
    Preview->layout()->setSpacing( 0 );
    Preview->layout()->setMargin( 0 );
    PreviewLayout = new QHBoxLayout( Preview->layout() );
    PreviewLayout->setAlignment( Qt::AlignTop );
    PreviewLayout->setSpacing( 10 );
    PreviewLayout->setMargin( 24 );

    TextLabelP = new QLabel( Preview, "TextLabel1_3" );
    TextLabelP->setText( tr( "Size:" ) );
    PreviewLayout->addWidget( TextLabelP );

    PreviewSize = new QComboBox( true, Preview, "PreviewSize" );
    PreviewSize->insertItem( tr( "Small" ) );
    PreviewSize->insertItem( tr( "Medium" ) );
    PreviewSize->insertItem( tr( "Big" ) );
    PreviewSize->setEditable(false);
    switch (Vor->PSize)
    	{
    	case 40:
    		PreviewSize->setCurrentItem(0);
    		break;
    	case 60:
    		PreviewSize->setCurrentItem(1);
    		break;
    	case 80:
    		PreviewSize->setCurrentItem(2);
    		break;
    	}
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
    GroupBox10 = new QGroupBox( tab_6, "GroupBox10" );
    GroupBox10->setTitle( tr( "" ) );
    GroupBox10->setColumnLayout(0, Qt::Vertical );
    GroupBox10->layout()->setSpacing( 0 );
    GroupBox10->layout()->setMargin( 0 );
    GroupBox10Layout = new QVBoxLayout( GroupBox10->layout() );
    GroupBox10Layout->setAlignment( Qt::AlignTop );
    GroupBox10Layout->setSpacing( 6 );
    GroupBox10Layout->setMargin( 11 );
    FramesVisible = new QCheckBox( GroupBox10, "FramesVisible" );
    FramesVisible->setText( tr( "Display Frames" ) );
    FramesVisible->setChecked(ap->HaveDoc ? ap->doc->ShFrames : Vor->ShFrames);
    GroupBox10Layout->addWidget( FramesVisible );
    SidebySide = new QCheckBox( GroupBox10, "SidebySide" );
    SidebySide->setText( tr( "Display Pages Side by Side" ) );
    SidebySide->setChecked(ap->HaveDoc ? ap->doc->PagesSbS : Vor->PagesSbS);
    GroupBox10Layout->addWidget( SidebySide );

    Layout_6b = new QHBoxLayout();
    Layout_6b->setSpacing( 0 );
    Layout_6b->setMargin( 0 );
    GroupBox20a = new QGroupBox( GroupBox10, "GroupBox20" );
    GroupBox20a->setTitle( tr( "Page-Colors" ) );
    GroupBox20a->setColumnLayout(0, Qt::Vertical );
    GroupBox20a->layout()->setSpacing( 0 );
    GroupBox20a->layout()->setMargin( 0 );
    GroupBox20aLayout = new QHBoxLayout( GroupBox20a->layout() );
    GroupBox20aLayout->setAlignment( Qt::AlignTop );
    GroupBox20aLayout->setSpacing( 10 );
    GroupBox20aLayout->setMargin( 5 );
    TextLabel4a = new QLabel( GroupBox20a, "TextLabel4" );
    TextLabel4a->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel4a->sizePolicy().hasHeightForWidth() ) );
    TextLabel4a->setText( tr( "Background:" ) );
    GroupBox20aLayout->addWidget( TextLabel4a);
    PapColor = new QPushButton( GroupBox20a, "PapColor" );
    PapColor->setText( tr( "" ) );
    PapColor->setAutoDefault( false );
    QPixmap pm1a = QPixmap(40, 20);
		if (ap->HaveDoc)
			{
    	pm1a.fill(ap->doc->papColor);
			Cpaper = ap->doc->papColor;
			}
		else
			{
    	pm1a.fill(Vor->DpapColor);
			Cpaper = Vor->DpapColor;
			}
    PapColor->setPixmap(pm1a);
    GroupBox20aLayout->addWidget( PapColor );
    TextLabel4b = new QLabel( GroupBox20a, "TextLabel4" );
    TextLabel4b->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel4b->sizePolicy().hasHeightForWidth() ) );
    TextLabel4b->setText( tr( "Margins:" ) );
    GroupBox20aLayout->addWidget( TextLabel4b);
    RandColor = new QPushButton( GroupBox20a, "RColor" );
    RandColor->setText( tr( "" ) );
    RandColor->setAutoDefault( false );
		if (ap->HaveDoc)
			{
    	pm1a.fill( ap->doc->margColor );
			Crand = ap->doc->margColor;
			}
		else
			{
    	pm1a.fill( Vor->DmargColor );
			Crand = Vor->DmargColor;
			}
    RandColor->setPixmap(pm1a);
    GroupBox20aLayout->addWidget( RandColor );
    Layout_6b->addWidget(GroupBox20a);
    QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout_6b->addItem( spacer2 );
    GroupBox10Layout->addLayout(Layout_6b);
    RandFarb = new QCheckBox( GroupBox10, "Ran" );
    RandFarb->setText( tr( "Display Unprintable Area in Margin Color" ) );
    RandFarb->setChecked(ap->HaveDoc ? ap->doc->RandFarbig : Vor->RandFarbig);
    GroupBox10Layout->addWidget( RandFarb );
    UsePDFTrans = new QCheckBox( GroupBox10, "Ran2" );
    UsePDFTrans->setText( tr( "Use PDF-1.4 Transparency Features" ) );
    UsePDFTrans->setChecked(Vor->PDFTransparency);
    GroupBox10Layout->addWidget( UsePDFTrans );

    CaliGroup = new QGroupBox( GroupBox10, "CaliGroup" );
    CaliGroup->setTitle( tr( "Adjust Display Size" ) );
    CaliGroup->setColumnLayout(0, Qt::Vertical );
    CaliGroup->layout()->setSpacing( 6 );
    CaliGroup->layout()->setMargin( 11 );
    CaliGroupLayout = new QVBoxLayout( CaliGroup->layout() );
    CaliGroupLayout->setAlignment( Qt::AlignTop );
    CaliText = new QLabel( CaliGroup, "CaliText" );
    CaliText->setText( tr( "To adjust the display drag the ruler below with the Slider." ) );
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
    layout15ca->addWidget( CaliSlider );
    CaliAnz = new QLabel( CaliGroup, "CaliAnz" );
		CaliAnz->setText(QString::number(DisScale*100, 'f', 2)+" %");
    layout15ca->addWidget( CaliAnz );
    CaliGroupLayout->addLayout( layout15ca );

    GroupBox10Layout->addWidget( CaliGroup );

    tabLayout_6->addWidget( GroupBox10 );
    TabWidget3->addWidget( tab_6, 6 );
    Fram->raiseWidget(1);

		PrefsLayout2->addWidget( TabWidget3 );
		PrefsLayout->addLayout(PrefsLayout2);

    Layout3 = new QHBoxLayout;
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer );
    buttonOK = new QPushButton( this, "buttonOK" );
    buttonOK->setText( tr( "OK" ) );
    buttonOK->setDefault( true );
    Layout3->addWidget( buttonOK );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer_2 );
    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( tr( "Cancel" ) );
    buttonCancel->setDefault( false );
    Layout3->addWidget( buttonCancel );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer_3 );
    PrefsLayout->addLayout( Layout3 );

    // signals and slots connections
    connect( minColor, SIGNAL( clicked() ), this, SLOT( changeMicolor() ) );
    connect( MaxColor, SIGNAL( clicked() ), this, SLOT( changeMaColor() ) );
    connect( PapColor, SIGNAL( clicked() ), this, SLOT( changePapColor() ) );
    connect( RandColor, SIGNAL( clicked() ), this, SLOT( changeRandColor() ) );
    connect( GuideColor, SIGNAL( clicked() ), this, SLOT( changeGuideColor() ) );
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
    connect(Shorty, SIGNAL(clicked()), this, SLOT(DefKB()));
    connect(T6_Faktor, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin(int)));
    connect(T6_Slider1, SIGNAL(valueChanged(int)), T6_Faktor, SLOT(setValue(int)));
    connect(T6_Slider1, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
    connect(T6_Faktor2, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin2(int)));
    connect(T6_Slider2, SIGNAL(valueChanged(int)), T6_Faktor2, SLOT(setValue(int)));
    connect(T6_Slider2, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
		connect(T6_Ecken, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
		connect(T6_Konvex, SIGNAL(clicked()), this, SLOT(UpdatePreView()));
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
    connect(CaliSlider, SIGNAL(valueChanged(int)), this, SLOT(SetDisScale()));
		connect(TabListe, SIGNAL(highlighted(int)), TabWidget3, SLOT(raiseWidget(int)));
		setSize(Vor->PageFormat);
		setOrien(Vor->Ausrichtung);
    UnitChange();
}

/*
 *  Destroys the object and frees any allocated resources
 */
Preferences::~Preferences()
{
    // no need to delete child widgets, Qt does it all for us
}

void Preferences::ChangeDocs()
{
	QString s = QFileDialog::getExistingDirectory(Docs->text(), this, "d", tr("Choose a Directory"), true);
	if (s != "")
		Docs->setText(s);
}

void Preferences::ChangeProfs()
{
	QString s = QFileDialog::getExistingDirectory(Docs->text(), this, "d", tr("Choose a Directory"), true);
	if (s != "")
		ProPfad->setText(s);
}

void Preferences::ChangeScripts()
{
	QString s = QFileDialog::getExistingDirectory(Docs->text(), this, "d", tr("Choose a Directory"), true);
	if (s != "")
		ScriptPfad->setText(s);
}

void Preferences::ChangeScaling()
{
	bool setter = false;
	if (FreeScale->isChecked())
		setter = true;
	TextLabelT51->setEnabled(setter);
	TextLabelT52->setEnabled(setter);
	XScale->setEnabled(setter);
	YScale->setEnabled(setter);
	Kette->setEnabled(setter);
	Aspect->setEnabled(!setter);
}

void Preferences::setDS()
{
    if (Doppelseiten->isChecked())
    	{
    	GRText3->setText( tr( "Inside:" ) );
    	GRText4->setText( tr( "Outside:" ) );
    	Linkszuerst->setEnabled(true);
    	}
    else
    	{
    	GRText3->setText( tr( "Left:" ) );
    	GRText4->setText( tr( "Right:" ) );
    	Linkszuerst->setEnabled(false);
			Linkszuerst->setChecked(false);
    	}
}

void Preferences::setBreite(int v)
{
	Pagebr = v / Umrech / 100.0;
	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
}

void Preferences::setHoehe(int v)
{
	Pageho = v / Umrech / 100.0;
	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
}

void Preferences::setTop(int v)
{
	RandT = v / Umrech / 100.0;
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
}

void Preferences::setBottom(int v)
{
	RandB = v / Umrech / 100.0;
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
}

void Preferences::setLeft(int v)
{
	RandR = v / Umrech / 100.0;
	RightR->setMaxValue(Breite->value() - LeftR->value());
}

void Preferences::setRight(int v)
{
	RandL = v / Umrech / 100.0;
	LeftR->setMaxValue(Breite->value() - RightR->value());
}

void Preferences::setSize(int gr)
{
	Pagebr = Breite->value() / 100.0;
	Pageho = Hoehe->value() / 100.0;
	Breite->setEnabled(false);
	Hoehe->setEnabled(false);
	switch (gr)
		{
		case 0: // A0
			Pagebr = 2380;
			Pageho = 3368;
			break;
		case 1: // A1
			Pagebr = 1684;
			Pageho = 2380;
			break;
		case 2: // A2
			Pagebr = 1190;
			Pageho = 1684;
			break;
		case 3: // A3
			Pagebr = 842;
			Pageho = 1190;
			break;
		case 4: // A4
			Pagebr = 595;
			Pageho = 842;
			break;
		case 5: // A5
			Pagebr = 421;
			Pageho = 595;
			break;
		case 6: // A6
			Pagebr = 297;
			Pageho = 421;
			break;
		case 7: // A7
			Pagebr = 210;
			Pageho = 297;
			break;
		case 8: // A8
			Pagebr = 148;
			Pageho = 210;
			break;
		case 9: // A9
			Pagebr = 105;
			Pageho = 148;
			break;
		case 10: // B0
			Pagebr = 2836;
			Pageho = 4008;
			break;
		case 11: // B1
			Pagebr = 2004;
			Pageho = 2836;
			break;
		case 12: // B2
			Pagebr = 1418;
			Pageho = 2004;
			break;
		case 13: // B3
			Pagebr = 1002;
			Pageho = 1418;
			break;
		case 14: // B4
			Pagebr = 709;
			Pageho = 1002;
			break;
		case 15: // B5
    	Pagebr = 501;
    	Pageho = 709;
			break;
		case 16: // B6
			Pagebr = 355;
			Pageho = 501;
			break;
		case 17: // B7
			Pagebr = 250;
			Pageho = 355;
			break;
		case 18: // B8
			Pagebr = 178;
			Pageho = 250;
			break;
		case 19: // B9
			Pagebr = 125;
			Pageho = 178;
			break;
		case 20: // B10
			Pagebr = 89;
			Pageho = 125;
			break;
		case 21: // C5E
			Pagebr = 462;
			Pageho = 649;
			break;
		case 22: // Comm10E
			Pagebr = 298;
			Pageho = 683;
			break;
		case 23: // DLE
			Pagebr = 312;
			Pageho = 624;
			break;
		case 24: // Executive
			Pagebr = 542;
			Pageho = 720;
			break;
		case 25: // Folio
			Pagebr = 595;
			Pageho = 935;
			break;
		case 26: // Ledger
			Pagebr = 1224;
			Pageho = 792;
			break;
		case 27: // Legal
    	Pagebr = 612;
    	Pageho = 1008;
			break;
		case 28: // Letter
    	Pagebr = 612;
    	Pageho = 792;
			break;
		case 29: // Tabloid
    	Pagebr = 792;
    	Pageho = 1224;
			break;
		case 30: // Custom
    	Breite->setEnabled(true);
    	Hoehe->setEnabled(true);
			break;
		}
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	Breite->setValue(qRound(Pagebr * Umrech * 100));
	Hoehe->setValue(qRound(Pageho * Umrech * 100));
	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
}

void Preferences::setOrien(int ori)
{
	int br;
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

void Preferences::DefKB()
{
	KeyManager *dia = new KeyManager(this, KKC);
	if (dia->exec())
		KKC = dia->KK;
	delete dia;
}

void Preferences::SetSample()
{
	QString ts = tr( "Woven silk pyjamas exchanged for blue quartz" );
	QString da = (*fon)[FontComb->currentText()]->Datei;
	int s = SizeCombo->currentText().left(2).toInt();
	QPixmap pm = FontSample(da, s, ts, paletteBackgroundColor());
	TextLabel1_4->setPixmap(pm);
}

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

void Preferences::ToggleKette()
{
	if (Kette->isOn())
		YScale->setValue(XScale->value());
}

void Preferences::HChange()
{
	if (Kette->isOn())
		YScale->setValue(XScale->value());
}

void Preferences::VChange()
{
	if (Kette->isOn())
		XScale->setValue(YScale->value());
}

void Preferences::UnitChange()
{
	double AltUmrech = Umrech;
	Einheit = UnitCombo->currentItem();
	switch (UnitCombo->currentItem())
		{
		case 0:
			Umrech = 1.0;
    	SpinBox1->setSuffix( tr( " pt" ) );
    	SpinBox2->setSuffix( tr( " pt" ) );
    	SpinBox2g->setSuffix( tr( " pt" ) );
    	Breite->setSuffix( tr( " pt" ) );
    	Hoehe->setSuffix( tr( " pt" ) );
    	TopR->setSuffix( tr( " pt" ) );
    	BottomR->setSuffix( tr( " pt" ) );
    	LeftR->setSuffix( tr( " pt" ) );
    	RightR->setSuffix( tr( " pt" ) );
			break;
		case 1:
			Umrech = 0.3527777;
    	SpinBox1->setSuffix( tr( " mm" ) );
    	SpinBox2->setSuffix( tr( " mm" ) );
    	SpinBox2g->setSuffix( tr( " mm" ) );
    	Breite->setSuffix( tr( " mm" ) );
    	Hoehe->setSuffix( tr( " mm" ) );
    	TopR->setSuffix( tr( " mm" ) );
    	BottomR->setSuffix( tr( " mm" ) );
    	LeftR->setSuffix( tr( " mm" ) );
    	RightR->setSuffix( tr( " mm" ) );
			break;
		case 2:
			Umrech = 1.0 / 72.0;
    	SpinBox1->setSuffix( tr( " in" ) );
    	SpinBox2->setSuffix( tr( " in" ) );
    	SpinBox2g->setSuffix( tr( " in" ) );
    	Breite->setSuffix( tr( " in" ) );
    	Hoehe->setSuffix( tr( " in" ) );
    	TopR->setSuffix( tr( " in" ) );
    	BottomR->setSuffix( tr( " in" ) );
    	LeftR->setSuffix( tr( " in" ) );
    	RightR->setSuffix( tr( " in" ) );
			break;
		case 3:
			Umrech = 1.0 / 12.0;
    	SpinBox1->setSuffix( tr( " p" ) );
    	SpinBox2->setSuffix( tr( " p" ) );
    	SpinBox2g->setSuffix( tr( " p" ) );
    	Breite->setSuffix( tr( " p" ) );
    	Hoehe->setSuffix( tr( " p" ) );
    	TopR->setSuffix( tr( " p" ) );
    	BottomR->setSuffix( tr( " p" ) );
    	LeftR->setSuffix( tr( " p" ) );
    	RightR->setSuffix( tr( " p" ) );
			break;
		}
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	disconnect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	disconnect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	disconnect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	disconnect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	SpinBox1->setValue(qRound(SpinBox1->value() / AltUmrech * Umrech));
	SpinBox2->setValue(qRound(SpinBox2->value() / AltUmrech * Umrech));
	SpinBox2g->setValue(qRound(SpinBox2g->value() / AltUmrech * Umrech));
	Breite->setValue(qRound(Pagebr * Umrech * 100));
	Hoehe->setValue(qRound(Pageho * Umrech * 100));
/*	RightR->setMaxValue(Breite->value() - LeftR->value());
	LeftR->setMaxValue(Breite->value() - RightR->value());
	TopR->setMaxValue(Hoehe->value() - BottomR->value());
	BottomR->setMaxValue(Hoehe->value() - TopR->value());    */


	RightR->setMaxValue(Breite->value() - qRound(RandL * Umrech * 100));
	LeftR->setMaxValue(Breite->value() - qRound(RandR * Umrech * 100));
	TopR->setMaxValue(Hoehe->value() - qRound(RandB * Umrech * 100));
	BottomR->setMaxValue(Hoehe->value() - qRound(RandT * Umrech * 100));

	TopR->setValue(qRound(RandT * Umrech * 100));
	BottomR->setValue(qRound(RandB * Umrech * 100));
	LeftR->setValue(qRound(RandL * Umrech * 100));
	RightR->setValue(qRound(RandR * Umrech * 100));
/*	if (GZComboF->currentItem() == 30)
		{
		Breite->setValue(qRound(Breite->value() / AltUmrech * Umrech));
		Hoehe->setValue(qRound(Hoehe->value() / AltUmrech * Umrech));
		}
	else
		setSize(GZComboF->currentItem());  */
	DrawRuler();
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
}

void Preferences::ValFromSpin2(int a)
{
	disconnect(T6_Slider2, SIGNAL(valueChanged(int)), T6_Faktor2, SLOT(setValue(int)));
	T6_Slider2->setValue(a);
  connect(T6_Slider2, SIGNAL(valueChanged(int)), T6_Faktor2, SLOT(setValue(int)));
}

void Preferences::ValFromSpin(int a)
{
	disconnect(T6_Slider1, SIGNAL(valueChanged(int)), T6_Faktor, SLOT(setValue(int)));
	T6_Slider1->setValue(a);
  connect(T6_Slider1, SIGNAL(valueChanged(int)), T6_Faktor, SLOT(setValue(int)));
}

void Preferences::UpdatePreView()
{
	Pre->fill(white);
	QPainter p;
	p.begin(Pre);
	p.setBrush(NoBrush);
	p.setPen(black);
	QPointArray pp = RegularPolygon(100, 100, T6_Ecken->value(), T6_Konvex->isChecked(), GetFaktor(), T6_Slider2->value());
	QRect br = pp.boundingRect();
	if (br.x() < 0)
		pp.translate(-br.x(), 0);
	if (br.y() < 0)
		pp.translate(0, -br.y());
	br = pp.boundingRect();
	if ((br.height() > 100) || (br.width() > 100))
		{
		QWMatrix ma;
		double sca = 100.0 / static_cast<double>(QMAX(br.width(), br.height()));
		ma.scale(sca, sca);
		pp = ma * pp;
		}
	p.drawPolygon(pp);
	p.end();
	T6_Preview->setPixmap(*Pre);
}

double Preferences::GetZeroFaktor()
{
	return sqrt(pow(1,2)-pow(((sin((360.0/(T6_Ecken->value()*2))/180*M_PI)* 2.0)/2.0),2));
}

double Preferences::GetMaxFaktor()
{
	double win = (360.0/(T6_Ecken->value()*2)) / 180.0 * M_PI;
	double ret;
	if ((360.0/(T6_Ecken->value()*2)) > 45)
		ret = 1/sin(win);
	else
		ret = 1/cos(win);
	return ret;
}

double Preferences::GetFaktor()
{
	int val = T6_Slider1->value();
	if (val < 0)
		{
		PFactor = GetZeroFaktor() * (100.0 + val) / 100.0;
		}
	else
		{
		double ma = GetMaxFaktor();
		double mi = GetZeroFaktor();
		PFactor = ((ma - mi) * val / 100.0) + mi;
		}
	return PFactor;
}

void Preferences::SetDisScale()
{
	DisScale = QMAX((100.0 + CaliSlider->value()) / 100.0, 0.01);
	DrawRuler();
	CaliAnz->setText(QString::number(DisScale*100, 'f', 2)+" %");
}

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
		{
		p.drawLine(static_cast<int>(xl), 13, static_cast<int>(xl), 19);
		}
	for (xl = 0; xl < maxi+10; xl += iter2)
		{
		p.drawLine(static_cast<int>(xl), 6, static_cast<int>(xl), 19);
		p.save();
		p.scale(1.0 / DisScale, 1.0);
		switch (Einheit)
			{
			case 2:
				p.drawText(static_cast<int>((xl+qRound(2/DisScale)) * DisScale), 12, QString::number(xl / iter2));
				break;
			case 3:
				p.drawText(static_cast<int>((xl+qRound(2/DisScale)) * DisScale), 12, QString::number(xl / iter));
				break;
			default:
				p.drawText(static_cast<int>((xl+qRound(2/DisScale)) * DisScale), 12, QString::number(xl / iter * 10));
				break;
			}
		p.restore();
		}
	p.end();
	CaliRuler->setPixmap(pm);
}

