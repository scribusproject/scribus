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
#include "arrowchooser.h"
#include "tabtypography.h"
#include "tabguides.h"
#include "hysettings.h"
#include "cmsprefs.h"
#include "keymanager.h"

using namespace std;

extern QPixmap fontSamples(QString da, int s, QString ts, QColor back);
extern QPixmap loadIcon(QString nam);
extern bool CMSavail;
extern ProfilesL InputProfiles;

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
	UnitCombo->setCurrentItem(prefsData->Einheit);
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
	Layout21->addWidget( GroupAS );
	tabLayout_7->addLayout( Layout21 );
	addItem( tr("Document"), loadIcon("page.png"), tab_7);

	tabGuides = new TabGuides(prefsWidgets, &prefsData->guidesSettings, Umrech, "");
	addItem( tr("Guides"), loadIcon("guides.png"), tabGuides);
	tabTypo = new TabTypograpy(  prefsWidgets, &prefsData->typographicSetttings, Umrech, "");
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
		if (fontComboText->text(fc) == prefsData->DefFont)
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
		if (sizeComboText->text(a).left(2).toInt() == prefsData->DefSize / 10)
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
	if (prefsData->DpenText == "None")
		colorComboText->setCurrentItem(colorComboText->count()-1);
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pmT2.fill(prefsData->DColors[itc.key()].getRGBColor());
		colorComboText->insertItem(pmT2, itc.key());
		if (itc.key() == prefsData->DpenText)
			colorComboText->setCurrentItem(colorComboText->count()-1);
	}
	subTabTextLayout->addMultiCellWidget( colorComboText, 2, 2, 1, 3, Qt::AlignLeft );
	textLabel3b = new QLabel(colorComboText, tr( "Fill Color:" ), subTabText, "textLabel3b" );
	subTabTextLayout->addWidget( textLabel3b, 2, 0 );
	colorComboStrokeText = new QComboBox( true, subTabText, "colorComboStrokeText" );
	colorComboStrokeText->setEditable(false);
	colorComboStrokeText->insertItem( tr("None"));
	if (prefsData->DstrokeText == "None")
		colorComboStrokeText->setCurrentItem(colorComboStrokeText->count()-1);
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pmT2.fill(prefsData->DColors[itc.key()].getRGBColor());
		colorComboStrokeText->insertItem(pmT2, itc.key());
		if (itc.key() == prefsData->DstrokeText)
			colorComboStrokeText->setCurrentItem(colorComboStrokeText->count()-1);
	}
	subTabTextLayout->addMultiCellWidget( colorComboStrokeText, 3, 3, 1, 3, Qt::AlignLeft );
	textLabel3b2 = new QLabel(colorComboStrokeText, tr( "Stroke Color:" ), subTabText, "textLabel3b2" );
	subTabTextLayout->addWidget( textLabel3b2, 3, 0 );
	columnsText = new QSpinBox( subTabText, "columnsText" );
	columnsText->setMinValue( 1 );
	columnsText->setMaxValue(100);
	columnsText->setValue(prefsData->DCols);
	subTabTextLayout->addWidget( columnsText, 4, 1, Qt::AlignLeft );
	textLabel4b = new QLabel(columnsText, tr("Colu&mns:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel4b, 4, 0 );
	gapText = new MSpinBox( 0, 200, subTabText, decimals );
	gapText->setSuffix( tr( " pt" ) );
	gapText->setValue(prefsData->DGap * Umrech);
	subTabTextLayout->addWidget( gapText, 4, 3, Qt::AlignLeft );
	textLabel5b = new QLabel(gapText, tr("&Gap:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel5b, 4, 2 );
	previewText = new QLabel( tr( "Woven silk pyjamas exchanged for blue quartz" ), subTabText, "previewText" );
	previewText->setMaximumWidth(width());
	previewText->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	previewText->setFrameShape(QFrame::Box);
	previewText->setPaletteBackgroundColor(paletteBackgroundColor());
	subTabTextLayout->addMultiCellWidget( previewText, 5, 5, 0, 3 );
	subStackTools->addWidget( subTabText, 0 );

	subTabShape = new QWidget( subStackTools, "subTabShape" );
	subTabShapeLayout = new QGridLayout( subTabShape, 1, 1, 11, 6, "subTabShapeLayout");
	subTabShapeLayout->setAlignment( Qt::AlignTop );
	colorComboLineShape = new QComboBox( true, subTabShape, "colorComboLineShape" );
	colorComboLineShape->setEditable(false);
	colorComboLineShape->insertItem( tr("None"));
	if (prefsData->Dpen == "None")
		colorComboLineShape->setCurrentItem(colorComboLineShape->count()-1);
	QPixmap pm2S = QPixmap(15, 15);
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pm2S.fill(prefsData->DColors[itc.key()].getRGBColor());
		colorComboLineShape->insertItem(pm2S, itc.key());
		if (itc.key() == prefsData->Dpen)
			colorComboLineShape->setCurrentItem(colorComboLineShape->count()-1);
	}
	subTabShapeLayout->addWidget( colorComboLineShape, 0, 1, Qt::AlignLeft );
	textLabel7b = new QLabel( colorComboLineShape, tr( "&Line Color:" ), subTabShape, "textLabel7b" );
	subTabShapeLayout->addWidget( textLabel7b, 0, 0 );
	shadingLineShape = new QSpinBox( subTabShape, "shadingLineShape" );
	shadingLineShape->setMaxValue( 100 );
	shadingLineShape->setSuffix( tr( " %" ) );
	shadingLineShape->setMinValue( 0 );
	shadingLineShape->setValue(prefsData->Dshade2);
	subTabShapeLayout->addWidget( shadingLineShape, 1, 1, Qt::AlignLeft );
	textLabel8b = new QLabel( shadingLineShape, tr( "&Shading:" ), subTabShape, "textLabel8b" );
	subTabShapeLayout->addWidget( textLabel8b, 1, 0 );
	comboFillShape = new QComboBox( true, subTabShape, "comboFillShape" );
	comboFillShape->setEditable(false);
	comboFillShape->insertItem( tr("None"));
	if (prefsData->Dbrush == "None")
		comboFillShape->setCurrentItem(comboFillShape->count()-1);
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pm2S.fill(prefsData->DColors[itc.key()].getRGBColor());
		comboFillShape->insertItem(pm2S, itc.key());
		if (itc.key() == prefsData->Dbrush)
			comboFillShape->setCurrentItem(comboFillShape->count()-1);
	}
	subTabShapeLayout->addWidget( comboFillShape, 2, 1, Qt::AlignLeft );
	textLabel9b = new QLabel( comboFillShape, tr( "&Fill Color:" ), subTabShape, "textLabel9b" );
	subTabShapeLayout->addWidget( textLabel9b, 2, 0 );
	shadingFillShape = new QSpinBox( subTabShape, "shadingFillShape" );
	shadingFillShape->setMaxValue( 100 );
	shadingFillShape->setSuffix( tr( " %" ) );
	shadingFillShape->setMinValue( 0 );
	shadingFillShape->setValue(prefsData->Dshade);
	subTabShapeLayout->addWidget( shadingFillShape, 3, 1, Qt::AlignLeft );
	textLabel10b = new QLabel( shadingFillShape, tr( "S&hading:" ), subTabShape, "textLabel10b" );
	subTabShapeLayout->addWidget( textLabel10b, 3, 0 );
	comboStyleShape = new LineCombo(subTabShape);
	comboStyleShape->setEditable(false);
	switch (prefsData->DLineArt)
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
	lineWidthShape->setValue(prefsData->Dwidth);
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
	if (prefsData->DpenLine == "None")
		colorComboLine->setCurrentItem(colorComboLine->count()-1);
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pm2L.fill(prefsData->DColors[itc.key()].getRGBColor());
		colorComboLine->insertItem(pm2L, itc.key());
		if (itc.key() == prefsData->DpenLine)
			colorComboLine->setCurrentItem(colorComboLine->count()-1);
	}
	subTabLineLayout->addMultiCellWidget( colorComboLine, 0, 0, 1, 2, Qt::AlignLeft );
	textLabel13b = new QLabel(colorComboLine, tr("&Line Color:"), subTabLine, "textLabel13b" );
	subTabLineLayout->addWidget( textLabel13b, 0, 0 );
	shadingLine = new QSpinBox( subTabLine, "shadingLine" );
	shadingLine->setMaxValue( 100 );
	shadingLine->setValue(prefsData->DshadeLine);
	shadingLine->setSuffix( tr( " %" ) );
	subTabLineLayout->addMultiCellWidget( shadingLine, 1, 1, 1, 2, Qt::AlignLeft );
	textLabel14b = new QLabel(shadingLine, tr("&Shading:"), subTabLine, "textLabel14b" );
	subTabLineLayout->addWidget( textLabel14b, 1, 0 );
	comboStyleLine = new LineCombo(subTabLine);
	comboStyleLine->setEditable(false);
	switch (prefsData->DLstyleLine)
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
	startArrow->rebuildList(&prefsData->arrowStyles);
	endArrow->rebuildList(&prefsData->arrowStyles);
	startArrow->setCurrentItem(prefsData->DstartArrow);
	endArrow->setCurrentItem(prefsData->DendArrow);
	arrowText = new QLabel( tr( "Arrows:" ), subTabLine, "arrowText" );
	subTabLineLayout->addMultiCellWidget( arrowText, 3, 4, 0, 0 );
	startArrowText = new QLabel( startArrow, tr( "Start:" ), subTabLine, "startArrowText" );
	subTabLineLayout->addWidget( startArrowText, 3, 1 );
	endArrowText = new QLabel( endArrow, tr( "End:" ), subTabLine, "endArrowText" );
	subTabLineLayout->addWidget( endArrowText, 3, 2 );
	lineWidthLine = new MSpinBox( 1, 36, subTabLine, 1 );
	lineWidthLine->setSuffix( tr( " pt" ) );
	lineWidthLine->setValue(prefsData->DwidthLine);
	subTabLineLayout->addMultiCellWidget( lineWidthLine, 5, 5, 1, 2, Qt::AlignLeft );
	textLabel16b = new QLabel(lineWidthLine, tr("Line &Width:"), subTabLine, "textLabel16b" );
	subTabLineLayout->addWidget( textLabel16b, 5, 0 );
	subStackTools->addWidget( subTabLine, 2 );

	subTabImage = new QWidget( subStackTools, "subTabImage" );
	subTabImageLayout = new QGridLayout( subTabImage, 1, 1, 11, 6, "subTabImageLayout");
	subTabImageLayout->setAlignment( Qt::AlignTop );
	buttonGroup3 = new QButtonGroup( subTabImage, "buttonGroup3" );
	buttonGroup3->setCheckable( true );
	buttonGroup3->setChecked( prefsData->ScaleType );
	buttonGroup3->setColumnLayout(0, Qt::Vertical );
	buttonGroup3->layout()->setSpacing( 6 );
	buttonGroup3->layout()->setMargin( 11 );
	buttonGroup3->setTitle( tr( "&Free Scaling" ) );
	buttonGroup3Layout = new QGridLayout( buttonGroup3->layout() );
	buttonGroup3Layout->setAlignment( Qt::AlignTop );
	scalingHorizontal = new QSpinBox( buttonGroup3, "scalingHorizontal" );
	scalingHorizontal->setMaxValue( 1000 );
	scalingHorizontal->setMinValue( 1 );
	scalingHorizontal->setValue(qRound(prefsData->ScaleX * 100));
	scalingHorizontal->setSuffix( tr( " %" ) );
	buttonGroup3Layout->addWidget( scalingHorizontal, 0, 1, Qt::AlignRight );
	textLabel17b = new QLabel(scalingHorizontal, tr("&Horizontal Scaling:"), buttonGroup3, "textLabel17b" );
	buttonGroup3Layout->addWidget( textLabel17b, 0, 0 );
	scalingVertical = new QSpinBox( buttonGroup3, "scalingVertical" );
	scalingVertical->setMaxValue( 1000 );
	scalingVertical->setMinValue( 1 );
	scalingVertical->setSuffix( tr( " %" ) );
	scalingVertical->setValue(qRound(prefsData->ScaleY * 100));
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
	buttonGroup5->setChecked( !prefsData->ScaleType );
	buttonGroup5->setColumnLayout(0, Qt::Vertical );
	buttonGroup5->layout()->setSpacing( 6 );
	buttonGroup5->layout()->setMargin( 11 );
	buttonGroup5->setTitle( tr( "&Scale Picture to Frame Size" ) );
	buttonGroup5Layout = new QHBoxLayout( buttonGroup5->layout() );
	buttonGroup5Layout->setAlignment( Qt::AlignTop );
	checkRatioImage = new QCheckBox( buttonGroup5, "checkRatioImage" );
	checkRatioImage->setText( tr( "Keep Aspect &Ratio" ) );
	checkRatioImage->setChecked(prefsData->AspectRatio);
	buttonGroup5Layout->addWidget( checkRatioImage );
	subTabImageLayout->addMultiCellWidget( buttonGroup5, 1, 1, 0, 1 );
	comboFillImage = new QComboBox( true, subTabImage, "comboFillImage" );
	comboFillImage->setEditable(false);
	QPixmap pm2I = QPixmap(15, 15);
	comboFillImage->insertItem( tr("None"));
	if (prefsData->DbrushPict == "None")
		comboFillImage->setCurrentItem(comboFillImage->count()-1);
	for (itc = prefsData->DColors.begin(); itc != prefsData->DColors.end(); ++itc)
	{
		pm2I.fill(prefsData->DColors[itc.key()].getRGBColor());
		comboFillImage->insertItem(pm2I, itc.key());
		if (itc.key() == prefsData->DbrushPict)
			comboFillImage->setCurrentItem(comboFillImage->count()-1);
	}
	subTabImageLayout->addWidget( comboFillImage, 2, 1, Qt::AlignLeft );
	textLabel19b = new QLabel(comboFillImage, tr( "F&ill Color:" ), subTabImage, "textLabel19b" );
	subTabImageLayout->addWidget( textLabel19b, 2, 0 );
	shadingFillImage = new QSpinBox( subTabImage, "shadingFillImage" );
	shadingFillImage->setMaxValue( 100 );
	shadingFillImage->setValue( prefsData->ShadePict );
	shadingFillImage->setSuffix( tr( " %" ) );
	subTabImageLayout->addWidget( shadingFillImage, 3, 1, Qt::AlignLeft );
	textLabel20b = new QLabel(shadingFillImage, tr( "S&hading:" ), subTabImage, "textLabel20b" );
	subTabImageLayout->addWidget( textLabel20b, 3, 0 );
	subStackTools->addWidget( subTabImage, 3 );

	subTabPolygon = new QWidget( subStackTools, "subTabPolygon" );
	subTabPolygonLayout = new QHBoxLayout( subTabPolygon, 11, 6, "subTabPolygonLayout");
	subTabPolygonLayout->setAlignment( Qt::AlignTop );
	polyWidget = new PolygonWidget(subTabPolygon, prefsData->PolyC, prefsData->PolyFd, prefsData->PolyF, prefsData->PolyS, prefsData->PolyR);
	subTabPolygonLayout->addWidget( polyWidget );
	subStackTools->addWidget( subTabPolygon, 4 );

	subTabZoom = new QWidget( subStackTools, "subTabZoom" );
	subTabZoomLayout = new QGridLayout( subTabZoom, 1, 1, 11, 6, "subTabZoomLayout");
	subTabZoomLayout->setAlignment( Qt::AlignTop );
	minimumZoom = new QSpinBox( subTabZoom, "minimumZoom" );
	minimumZoom->setMaxValue( 3200 );
	minimumZoom->setMinValue( 10 );
	minimumZoom->setValue(prefsData->MagMin);
	minimumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( minimumZoom, 0, 1, Qt::AlignLeft );
	textLabel21b = new QLabel( minimumZoom, tr( "Mi&nimum:" ), subTabZoom, "textLabel21b" );
	subTabZoomLayout->addWidget( textLabel21b, 0, 0);
	maximumZoom = new QSpinBox( subTabZoom, "maximumZoom" );
	maximumZoom->setMaxValue( 3200 );
	maximumZoom->setMinValue( 10 );
	maximumZoom->setValue(prefsData->MagMax);
	maximumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( maximumZoom, 1, 1, Qt::AlignLeft );
	textLabel22b = new QLabel( maximumZoom, tr( "Ma&ximum:" ), subTabZoom, "textLabel22b" );
	subTabZoomLayout->addWidget( textLabel22b, 1, 0 );
	zoomStep = new QSpinBox( subTabZoom, "zoomStep" );
	zoomStep->setMaxValue( 200 );
	zoomStep->setMinValue( 1 );
	zoomStep->setLineStep( 25 );
	zoomStep->setValue( prefsData->MagStep );
	zoomStep->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( zoomStep, 2, 1, Qt::AlignLeft );
	textLabel23b = new QLabel( zoomStep, tr( "&Stepping:" ), subTabZoom, "textLabel23b" );
	subTabZoomLayout->addWidget( textLabel23b, 2, 0 );
	subStackTools->addWidget( subTabZoom, 5 );
	tabToolsLayout->addWidget( subStackTools );
	addItem( tr("Tools"), loadIcon("tools.png"), tabTools);

	tabHyphenator = new HySettings(prefsWidgets, &ap->LangTransl);
	tabHyphenator->verbose->setChecked(!prefsData->Automatic);
	tabHyphenator->input->setChecked(prefsData->AutoCheck);
	tabHyphenator->language->setCurrentText(ap->LangTransl[prefsData->Language]);
	tabHyphenator->wordLen->setValue(prefsData->MinWordLen);
	tabHyphenator->maxCount->setValue(prefsData->HyCount);
	addItem( tr("Hyphenator"), loadIcon("hyphenate.png"), tabHyphenator);

	if (CMSavail)
	{
		tabColorManagement = new CMSPrefs(prefsWidgets, &prefsData->DCMSset, &InputProfiles, &ap->PrinterProfiles, &ap->MonitorProfiles);
		addItem( tr("Color Management"), loadIcon("blend.png"), tabColorManagement);
	}

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
	setSize(prefsData->PageFormat);
	setOrien(prefsData->Ausrichtung);
	pageWidth->setValue(prefsData->PageBreite * Umrech);
	pageHeight->setValue(prefsData->PageHoehe * Umrech);
	unitChange();
	prefsWidgets->raiseWidget(0);
	toolText->setOn(true);
	setSample();
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
 \fn void Preferences::ToggleKette()
 \author Franz Schmid
 \date
 \brief Preferences (Tools, Image Frame), Sets Y Scale value from X Scale value when chain is toggled
 \param None
 \retval None
 */
void Preferences::toggleChain()
{
	if (chainButton->isOn())
		scalingVertical->setValue(scalingHorizontal->value());
}

/*!
 \fn void Preferences::HChange()
 \author Franz Schmid
 \date
 \brief Preferences (Tools, Image Frame), Sets Y Scale value from X Scale value when X Scale value is changed
 \param None
 \retval None
 */
void Preferences::hChange()
{
	if (chainButton->isOn())
		scalingVertical->setValue(scalingHorizontal->value());
}

/*!
 \fn void Preferences::vChange()
 \author Franz Schmid
 \date
 \brief Preferences (Tools, Image Frame). Sets X Scale value from Y Scale value when Y Scale value is changed
 \param None
 \retval None
 */
void Preferences::vChange()
{
	if (chainButton->isOn())
		scalingHorizontal->setValue(scalingVertical->value());
}

void Preferences::changeImageScalingFree(int)
{
	if (buttonGroup3->isChecked())
		buttonGroup5->setChecked(false);
	else
		buttonGroup5->setChecked(true);
}

void Preferences::changeImageScalingRatio(int)
{
	if (buttonGroup5->isChecked())
		buttonGroup3->setChecked(false);
	else
		buttonGroup3->setChecked(true);
}

/*!
 \fn void Preferences::SetSample()
 \author Franz Schmid
 \date
 \brief Preferences (Tools, Text frame), Sets the sample text in selected font in text frame preferences
 \param None
 \retval None
 */
void Preferences::setSample()
{
	QString ts = tr( "Woven silk pyjamas exchanged for blue quartz" );
	QString da = (*fon)[fontComboText->currentText()]->Datei;
	int s = sizeComboText->currentText().left(2).toInt();
	QPixmap pm = fontSamples(da, s, ts, paletteBackgroundColor());
	previewText->setPixmap(pm);
}

/*!
 \fn void Preferences::SetTool()
 \author Franz Schmid
 \date
 \brief Preferences (Tools), Raises widget for selected Tool properties
 \param None
 \retval None
 */
void Preferences::setTool()
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
	pageWidth->setSuffix(einh);
	pageHeight->setSuffix(einh);
	TopR->setSuffix(einh);
	BottomR->setSuffix(einh);
	LeftR->setSuffix(einh);
	RightR->setSuffix(einh);
	tabGuides->minorSpace->setSuffix(einh);
	tabGuides->majorSpace->setSuffix(einh);
	tabGuides->snapDistance->setSuffix(einh);
	tabTypo->baseGrid->setSuffix(einh);
	tabTypo->baseOffset->setSuffix(einh);
	gapText->setSuffix(einh);
	topScratch->setSuffix(einh);
	bottomScratch->setSuffix(einh);
	leftScratch->setSuffix(einh);
	rightScratch->setSuffix(einh);
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

