/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tabpdfoptions.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QListWidget>
#include <QListWidgetItem>

#include <QStandardItem>
#include <QAbstractItemView>

#include "ui/createrange.h"
#include "pdfoptions.h"
#include "prefsmanager.h"
#include "scribuscore.h"
#include "scconfig.h"
#include "units.h"
#include "usertaskstructs.h"
#include "scrspinbox.h"
#include "util_icon.h"
#include "scribuscore.h"
#include "util.h"

TabPDFOptions::TabPDFOptions(   QWidget* parent, PDFOptions & Optionen,
                                const SCFonts &AllFonts,
                                const ProfilesL & PDFXProfiles,
                                const QMap<QString, int> & DocFonts,
                                const QList<PDFPresentationData> & Eff,
                                int unitIndex, double PageH, double PageB,
                                ScribusDoc * mdoc, bool exporting )
	: QTabWidget( parent ),
	// Initialize all those darn pointer members so we catch unitialized
	// accesses. I (CR) use the following command to generate these based on
	// the header excerpt:
	//    !cut -d \* -f 2 | sed -r 's/ ?(.*);/\t\1\(0\),/g'
	// Public GUI member pointers:
	AddSec(0),
	Article(0),
	BleedBottom(0),
	BleedLeft(0),
	BleedRight(0),
	BleedTop(0),
	CheckBM(0),
	CheckBox1(0),
	CMethod(0),
	ClipMarg(0),
	ComboBind(0),
	Compression(0),
	continuousPages(0),
	CopySec(0),
	CQuality(0),
	doublePageLeft(0),
	doublePageRight(0),
	DSColor(0),
	EmbedProfs(0),
	EmbedProfs2(0),
	Encry(0),
	ImageP(0),
	IntendI(0),
	IntendS(0),
	MirrorH(0),
	MirrorV(0),
	ModifySec(0),
	NoEmbedded(0),
	OutCombo(0),
	Pages(0),
	PassOwner(0),
	PassUser(0),
	PDFVersionCombo(0),
	PrintProfC(0),
	PrintSec(0),
	Resolution(0),
	RotateDeg(0),
	singlePage(0),
	SolidPr(0),
	useBookmarks(0),
	useFullScreen(0),
	useLayers(0),
	useLayers2(0),
	UseLPI(0),
	useSpot(0),
	overprintMode(0),
	useThumbnails(0),
	ValC(0),
	// Protected members other than GUI member pointers
	// End GUI member pointers
	// Protected non-gui members
	PgSel(0),
	EffVal(Eff),
	// Protected GUI member pointers
	actionCombo(0),
	AllPages(0),
	CheckBox10(0),
	EDirection(0),
	EDirection_2(0),
	EDirection_2_2(0),
	EffectTime(0),
	EffectType(0),
	fitWindow(0),
	hideMenuBar(0),
	hideToolBar(0),
	InfoString(0),
	LPIangle(0),
	LPIfreq(0),
	LPIfunc(0),
	PageNr(0),
	PageTime(0),
	// End protected gui member pointers
	// Private member gui pointers
	actionLabel(0),
	AvailFlist(0),
	BleedGroup(0),
	BleedGroupLayout(0),
	CBox(0),
	CBoxLayout(0),
	ColorGroup(0),
	ColorGroupLayout(0),
	ColorText1(0),
	Effects(0),
	EffectsLayout(0),
	EmbedFonts(0),
	EmbedList(0),
	EonAllPg(0),
	FromEmbed(0),
	FromOutline(0),
	GroupBox1(0),
	GroupBox1Layout(0),
	GroupBox9(0),
	GroupBox9Layout(0),
	groupDisplay(0),
	groupDisplayLayout(0),
	GroupFont(0),
	GroupFontLayout(0),
	groupJava(0),
	groupJavaLayout(0),
	groupNavigation(0),
	groupNavigationLayout(0),
	GroupPass(0),
	GroupPassLayout(0),
	GroupSecSet(0),
	GroupSecSetLayout(0),
	Layout11(0),
	pageNumberSelectorLayout(0),
	Layout11a(0),
	Layout13(0),
	Layout4_2(0),
	Layout5_2(0),
	Layout5_2a(0),
	Layout6(0),
	LayoutSpecial(0),
	LPIcolor(0),
	LPIgroup(0),
	LPIgroupLayout(0),
	OnlySome(0),
	pageLayout(0),
	pageLayoutLayout(0),
	PagePrev(0),
	PDFX1(0),
	PDFX2(0),
	PrintSec2(0),
	ProfsGroup(0),
	ProfsGroupLayout(0),
	ProfsTxt1(0),
	ProfsTxt2(0),
	ProfsTxt3(0),
	ProfsTxt4(0),
	RangeGroup(0),
	RangeGroupLayout(0),
	OutlineFonts(0),
	OutlineList(0),
	tabColor(0),
	tabColorLayout(0),
	tabFonts(0),
	tabGeneral(0),
	tabLayout(0),
	tabLayout_3(0),
	tabLayout_5(0),
	tabPDFX(0),
	tabPDFXLayout(0),
	tabPresentation(0),
	tabSecurity(0),
	tabSecurityLayout(0),
	tabSpecial(0),
	tabSpecialLayout(0),
	TextCom1(0),
	TextCom2(0),
	TextFont1(0),
	TextFont1_2(0),
	TextFont1_2a(0),
	TextLabel1(0),
	TextLabel1e(0),
	TextLabel1x(0),
	TextLabel2(0),
	TextLabel2e(0),
	TextLabel3(0),
	TextLabel3e(0),
	TextLabel4e(0),
	TextLabel5e(0),
	TextLabel6e(0),
	textLPI1(0),
	textLPI2(0),
	textLPI3(0),
	TextSec1(0),
	TextSec2(0),
	pageNrButton(0),
	ToEmbed(0),
	ToOutline(0),
	useViewDefault(0),
	X3Group(0),
	X3GroupLayout(0),
	// end protected member gui pointers
	// Private members
	unit(unitGetSuffixFromIndex(unitIndex)),
	precision(unitGetPrecisionFromIndex(unitIndex)),
	unitRatio(unitGetRatioFromIndex(unitIndex)),
	pdfExport(exporting),
	doc(mdoc),
	AllFonts(AllFonts),
	Opts(Optionen),
	pageH(PageH),
	pageB(PageB),
	cms(false)
{

	tabGeneral = new QWidget( this );
	tabLayout = new QVBoxLayout( tabGeneral );
	tabLayout->setSpacing( 5 );
	tabLayout->setMargin( 10 );
	Layout13 = new QHBoxLayout;
	Layout13->setSpacing( 5 );
	Layout13->setMargin( 0 );
	RangeGroup = new QGroupBox( tabGeneral );
	RangeGroup->setTitle( tr( "Export Range" ) );
	RangeGroupLayout = new QVBoxLayout( RangeGroup );
	RangeGroupLayout->setSpacing( 5 );
	RangeGroupLayout->setMargin( 10 );
	RangeGroupLayout->setAlignment( Qt::AlignTop );
	AllPages = new QRadioButton( tr( "&All Pages" ), RangeGroup );
	RangeGroupLayout->addWidget( AllPages );
	Layout11 = new QGridLayout;
	Layout11->setSpacing( 5 );
	Layout11->setMargin( 0 );
	OnlySome = new QRadioButton( tr( "C&hoose Pages" ), RangeGroup );
	Layout11->addWidget( OnlySome, 0, 0 );
 	pageNumberSelectorLayout = new QHBoxLayout;
	pageNumberSelectorLayout->setSpacing( 5 );
	pageNumberSelectorLayout->setMargin( 0 );
	PageNr = new QLineEdit( RangeGroup );
 	pageNumberSelectorLayout->addWidget( PageNr );
 	pageNrButton = new QPushButton( RangeGroup );
 	pageNrButton->setIcon(QIcon(loadIcon("ellipsis.png")));
 	pageNumberSelectorLayout->addWidget( pageNrButton );
 	Layout11->addLayout( pageNumberSelectorLayout, 1, 0 );
	RangeGroupLayout->addLayout( Layout11 );
	RotationLayout = new QHBoxLayout;
	RotationLayout->setSpacing( 5 );
	RotationLayout->setMargin( 0 );
	TextLabel3 = new QLabel( tr( "&Rotation:" ), RangeGroup );
	RotationLayout->addWidget( TextLabel3 );
	RotateDeg = new QComboBox(RangeGroup);
	RotateDeg->addItem(QString::fromUtf8("0 °"));
	RotateDeg->addItem(QString::fromUtf8("90 °"));
	RotateDeg->addItem(QString::fromUtf8("180 °"));
	RotateDeg->addItem(QString::fromUtf8("270 °"));
	RotateDeg->setEditable(false);
	TextLabel3->setBuddy(RotateDeg);
	RotationLayout->addWidget( RotateDeg );
	RangeGroupLayout->addLayout(RotationLayout);
	Layout11a = new QGridLayout;
	Layout11a->setSpacing( 5 );
	Layout11a->setMargin( 0 );
	MirrorH = new QToolButton(RangeGroup);
	MirrorH->setIcon(QIcon(loadIcon("16/flip-object-horizontal.png")));
	MirrorH->setCheckable( true );
	Layout11a->addWidget( MirrorH, 0, 0 );
	MirrorV = new QToolButton( RangeGroup );
	MirrorV->setIcon(QIcon(loadIcon("16/flip-object-vertical.png")));
	MirrorV->setCheckable( true );
	Layout11a->addWidget( MirrorV, 0, 1 );
	RangeGroupLayout->addLayout( Layout11a );
	ClipMarg = new QCheckBox( tr( "Clip to Page Margins" ), RangeGroup );
	RangeGroupLayout->addWidget( ClipMarg );
	Layout13->addWidget( RangeGroup );

	GroupBox1 = new QGroupBox( tr( "File Options" ), tabGeneral );
	GroupBox1Layout = new QGridLayout( GroupBox1 );
	GroupBox1Layout->setAlignment( Qt::AlignTop );
	GroupBox1Layout->setSpacing( 5 );
	GroupBox1Layout->setMargin( 10 );
	TextLabel1 = new QLabel( tr( "Compatibilit&y:" ), GroupBox1 );
	TextLabel1->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
	GroupBox1Layout->addWidget( TextLabel1, 0, 0 );
	PDFVersionCombo = new QComboBox( GroupBox1 );
	PDFVersionCombo->setEditable(false);
	TextLabel1->setBuddy(PDFVersionCombo);
	PDFVersionCombo->addItem("PDF 1.3 (Acrobat 4)");
	PDFVersionCombo->addItem("PDF 1.4 (Acrobat 5)");
	PDFVersionCombo->addItem("PDF 1.5 (Acrobat 6)");
	cms = doc ? (ScCore->haveCMS() && doc->HasCMS) : false;
	if (cms && (!PDFXProfiles.isEmpty()))
		PDFVersionCombo->addItem("PDF/X-3");
	GroupBox1Layout->addWidget( PDFVersionCombo, 0, 1, 1, 2 );
	TextLabel1x = new QLabel( tr( "&Binding:" ), GroupBox1 );
	TextLabel1x->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	GroupBox1Layout->addWidget( TextLabel1x, 1, 0 );
	ComboBind = new QComboBox( GroupBox1 );
	ComboBind->addItem( tr("Left Margin"));
	ComboBind->addItem( tr("Right Margin"));
	ComboBind->setEditable(false);
	TextLabel1x->setBuddy(ComboBind);
	GroupBox1Layout->addWidget( ComboBind, 1, 1, 1, 2 );
	CheckBox1 = new QCheckBox( tr( "Generate &Thumbnails" ), GroupBox1 );
	GroupBox1Layout->addWidget( CheckBox1, 2, 0, 1, 3 );
	Article = new QCheckBox( tr( "Save &Linked Text Frames as PDF Articles" ), GroupBox1 );
	GroupBox1Layout->addWidget( Article, 3, 0, 1, 3 );
	CheckBM = new QCheckBox( tr( "&Include Bookmarks" ), GroupBox1 );
	GroupBox1Layout->addWidget( CheckBM, 4, 0, 1, 3 );
	useLayers = new QCheckBox( tr( "Include La&yers" ), GroupBox1 );
	GroupBox1Layout->addWidget( useLayers, 5, 0, 1, 3 );
	Resolution = new QSpinBox( GroupBox1 );
	Resolution->setMaximum( 4000 );
	Resolution->setMinimum( 35 );
	Resolution->setSuffix( tr( " dpi" ) );
	TextLabel2 = new QLabel( tr( "&Resolution for EPS Graphics:" ), GroupBox1 );
	TextLabel2->setBuddy(Resolution);
	TextLabel2->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
	GroupBox1Layout->addWidget( TextLabel2, 6, 0 );
	GroupBox1Layout->addWidget( Resolution, 6, 1, Qt::AlignLeft );
	Layout13->addWidget( GroupBox1 );
	tabLayout->addLayout( Layout13 );
	Compression = new QCheckBox( tr( "Com&press Text and Vector Graphics" ), tabGeneral );
	tabLayout->addWidget( Compression );
	CBox = new QGroupBox( tr( "Image Compression Method" ), tabGeneral );
	CBoxLayout = new QGridLayout( CBox );
	CBoxLayout->setSpacing( 5 );
	CBoxLayout->setMargin( 10 );
	CBoxLayout->setAlignment( Qt::AlignTop );
	CMethod = new QComboBox(CBox);
	CMethod->clear();
	CMethod->addItem( tr( "Automatic" ) );
	CMethod->addItem( tr( "Lossy - JPEG" ) );
	CMethod->addItem( tr( "Lossless - Zip" ) );
	CMethod->addItem( tr( "None" ) );
	CMethod->setEditable(false);
	TextCom1 = new QLabel( tr( "Compression Metho&d:" ), CBox );
	TextCom1->setBuddy(CMethod);
	CBoxLayout->addWidget( TextCom1, 0, 0 );
	CBoxLayout->addWidget( CMethod, 0, 1 );
	CQuality = new QComboBox(CBox);
	CQuality->clear();
	CQuality->addItem( tr( "Maximum" ) );
	CQuality->addItem( tr( "High" ) );
	CQuality->addItem( tr( "Medium" ) );
	CQuality->addItem( tr( "Low" ) );
	CQuality->addItem( tr( "Minimum" ) );
	CQuality->setEditable(false);
	TextCom2 = new QLabel( tr( "Compression &Quality:" ), CBox );
	TextCom2->setBuddy(CQuality);
	CBoxLayout->addWidget( TextCom2, 1, 0 );
	CBoxLayout->addWidget( CQuality, 1, 1 );
	DSColor = new QCheckBox( tr( "Maximum Image Resolution:" ), CBox );
	CBoxLayout->addWidget( DSColor, 2, 0 );
	ValC = new QSpinBox( CBox );
	ValC->setSuffix( tr( " dpi" ) );
	ValC->setMaximum( 4000 );
	ValC->setMinimum( 35 );
	CBoxLayout->addWidget( ValC, 2, 1, Qt::AlignLeft );
	tabLayout->addWidget( CBox );
	addTab(tabGeneral, tr( "&General" ));
	if (doc != 0 && exporting)
	{
		tabFonts = new QWidget( this );
		tabLayout_3 = new QVBoxLayout( tabFonts );
		tabLayout_3->setSpacing( 5 );
		tabLayout_3->setMargin( 10 );
		GroupFont = new QGroupBox( tr( "Embedding" ), tabFonts);
		GroupFontLayout = new QHBoxLayout( GroupFont );
		GroupFontLayout->setAlignment( Qt::AlignTop );
		GroupFontLayout->setSpacing( 5 );
		GroupFontLayout->setMargin( 10 );
		Layout4_2 = new QVBoxLayout;
		Layout4_2->setSpacing( 5 );
		Layout4_2->setMargin( 0 );
		TextFont1 = new QLabel( tr( "Available Fonts:" ), GroupFont);
		Layout4_2->addWidget( TextFont1 );
		AvailFlist = new QListWidget( GroupFont );
		AvailFlist->setMaximumHeight(300);
		Layout4_2->addWidget( AvailFlist );
		GroupFontLayout->addLayout( Layout4_2 );
		Layout5_2 = new QVBoxLayout;
		Layout5_2->setSpacing( 5 );
		Layout5_2->setMargin( 0 );
		QSpacerItem* spacerS3a = new QSpacerItem( 2, 30, QSizePolicy::Minimum, QSizePolicy::Minimum );
		Layout5_2->addItem( spacerS3a );
		ToEmbed = new QPushButton(GroupFont);
		ToEmbed->setIcon(QIcon(loadIcon("22/go-next.png")));
		Layout5_2->addWidget( ToEmbed );
		FromEmbed = new QPushButton(GroupFont);
		FromEmbed->setIcon(QIcon(loadIcon("22/go-previous.png")));
		Layout5_2->addWidget( FromEmbed );
		QSpacerItem* spacerS3 = new QSpacerItem( 2, 2, QSizePolicy::Minimum, QSizePolicy::Expanding );
		Layout5_2->addItem( spacerS3 );
		GroupFontLayout->addLayout( Layout5_2 );
		Layout6 = new QVBoxLayout;
		Layout6->setSpacing( 5 );
		Layout6->setMargin( 0 );
		TextFont1_2 = new QLabel( tr( "Fonts to embed:" ), GroupFont);
		Layout6->addWidget( TextFont1_2 );
		EmbedList = new QListWidget( GroupFont);
		EmbedList->setMaximumHeight(105);
		Layout6->addWidget( EmbedList );

		Layout5_2a = new QHBoxLayout;
		Layout5_2a->setSpacing( 5 );
		Layout5_2a->setMargin( 0 );
		EmbedFonts = new QPushButton( tr( "&Embed all" ), GroupFont);
		Layout5_2a->addWidget( EmbedFonts );
		QSpacerItem* spacerS1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout5_2a->addItem( spacerS1 );
		ToOutline = new QPushButton(GroupFont);
		ToOutline->setIcon(QIcon(loadIcon("22/go-down.png")));
		Layout5_2a->addWidget( ToOutline );
		FromOutline = new QPushButton(GroupFont);
		FromOutline->setIcon(QIcon(loadIcon("22/go-up.png")));
		Layout5_2a->addWidget( FromOutline );
		QSpacerItem* spacerS2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout5_2a->addItem( spacerS2 );
		Layout6->addLayout( Layout5_2a );
		TextFont1_2a = new QLabel( tr( "Fonts to outline:" ), GroupFont);
		Layout6->addWidget( TextFont1_2a );
		OutlineList = new QListWidget( GroupFont );
		OutlineList->setMaximumHeight(105);
		Layout6->addWidget( OutlineList );
		OutlineFonts = new QPushButton( tr( "&Outline all" ), GroupFont);
		Layout6->addWidget( OutlineFonts );
		GroupFontLayout->addLayout( Layout6 );
		tabLayout_3->addWidget( GroupFont );
		addTab( tabFonts, tr( "&Fonts" ) );
		tabPresentation = new QWidget( this );
		tabLayout_5 = new QGridLayout( tabPresentation );
		tabLayout_5->setSpacing( 5 );
		tabLayout_5->setMargin( 10 );
		CheckBox10 = new QCheckBox( tr( "Enable &Presentation Effects" ), tabPresentation);
		tabLayout_5->addWidget( CheckBox10, 0, 0, 1, 2 );
		Pages = new QListWidget( tabPresentation );
//		Pages->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, Pages->sizePolicy().hasHeightForWidth() ) );
		tabLayout_5->addWidget( Pages, 1, 0 );
		PagePrev = new QCheckBox( tr( "Show Page Pre&views" ), tabPresentation);
		tabLayout_5->addWidget( PagePrev, 2, 0 );
		Effects = new QGroupBox( tr( "Effects" ), tabPresentation);
		EffectsLayout = new QGridLayout( Effects );
		EffectsLayout->setAlignment( Qt::AlignTop );
		EffectsLayout->setSpacing( 5 );
		EffectsLayout->setMargin( 10 );
		TextLabel1e = new QLabel( tr( "&Display Duration:" ), Effects);
		EffectsLayout->addWidget( TextLabel1e, 0, 0 );
		TextLabel2e = new QLabel( tr( "Effec&t Duration:" ), Effects);
		EffectsLayout->addWidget( TextLabel2e, 1, 0 );
		TextLabel3e = new QLabel( tr( "Effect T&ype:" ), Effects);
		EffectsLayout->addWidget( TextLabel3e, 2, 0 );
		TextLabel4e = new QLabel( tr( "&Moving Lines:" ), Effects);
		EffectsLayout->addWidget( TextLabel4e, 3, 0 );
		TextLabel5e = new QLabel( tr( "F&rom the:" ), Effects);
		EffectsLayout->addWidget( TextLabel5e, 4, 0 );
		TextLabel6e = new QLabel( tr( "D&irection:" ), Effects);
		EffectsLayout->addWidget( TextLabel6e, 5, 0 );
		PageTime = new QSpinBox( Effects);
		PageTime->setSuffix( tr( " sec" ) );
		PageTime->setMaximum( 3600 );
		PageTime->setMinimum( 0 );
		TextLabel1e->setBuddy(PageTime);
		EffectsLayout->addWidget( PageTime, 0, 1 );
		EffectTime = new QSpinBox( Effects);
		EffectTime->setSuffix( tr( " sec" ) );
		EffectTime->setMaximum( 3600 );
		EffectTime->setMinimum( 1 );
		TextLabel2e->setBuddy(EffectTime);
		EffectsLayout->addWidget( EffectTime, 1, 1 );
		EffectType = new QComboBox(Effects);
		QString tmpc[] = { tr("No Effect"), tr("Blinds"), tr("Box"), tr("Dissolve"), tr("Glitter"), tr("Split"), tr("Wipe")};
		size_t ar = sizeof(tmpc) / sizeof(*tmpc);
		for (uint a = 0; a < ar; ++a)
			EffectType->addItem(tmpc[a]);
		EffectType->setEditable(false);
		TextLabel3e->setBuddy(EffectType);
		EffectsLayout->addWidget( EffectType, 2, 1 );
		EDirection = new QComboBox(Effects);
		EDirection->addItem( tr( "Horizontal" ) );
		EDirection->addItem( tr( "Vertical" ) );
		EDirection->setEditable(false);
		TextLabel4e->setBuddy(EDirection);
		EffectsLayout->addWidget( EDirection, 3, 1 );
		EDirection_2 = new QComboBox(Effects);
		EDirection_2->addItem( tr( "Inside" ) );
		EDirection_2->addItem( tr( "Outside" ) );
		EDirection_2->setEditable(false);
		TextLabel5e->setBuddy(EDirection_2);
		EffectsLayout->addWidget( EDirection_2, 4, 1 );
		EDirection_2_2 = new QComboBox(Effects);
		QString tmp_ed[] = { tr("Left to Right"), tr("Top to Bottom"), tr("Bottom to Top"), tr("Right to Left"),
		                     tr("Top-left to Bottom-Right")};
		size_t ar_ed = sizeof(tmp_ed) / sizeof(*tmp_ed);
		for (uint a = 0; a < ar_ed; ++a)
			EDirection_2_2->addItem(tmp_ed[a]);
		EDirection_2_2->setEditable(false);
		TextLabel6e->setBuddy(EDirection_2_2);
		EffectsLayout->addWidget( EDirection_2_2, 5, 1 );
		EonAllPg = new QPushButton( tr( "&Apply Effect to all Pages" ), Effects);
		EffectsLayout->addWidget( EonAllPg, 6, 0, 1, 2 );
		tabLayout_5->addWidget( Effects, 1, 1, 2, 1 );
		addTab( tabPresentation, tr( "E&xtras" ) );
		tabSpecial = new QWidget( this );
		tabSpecialLayout = new QVBoxLayout( tabSpecial );
		tabSpecialLayout->setSpacing( 5 );
		tabSpecialLayout->setMargin( 10 );
		groupDisplay = new QGroupBox( tabSpecial );
		groupDisplay->setTitle( tr( "Display Settings" ) );
		groupDisplayLayout = new QVBoxLayout( groupDisplay );
		groupDisplayLayout->setSpacing( 5 );
		groupDisplayLayout->setMargin( 10 );
		groupDisplayLayout->setAlignment( Qt::AlignTop );
		LayoutSpecial = new QHBoxLayout;
		LayoutSpecial->setSpacing( 5 );
		LayoutSpecial->setMargin( 0 );
		pageLayout = new QGroupBox( groupDisplay );
		pageLayout->setTitle( tr( "Document Layout" ) );
		pageLayoutLayout = new QVBoxLayout( pageLayout );
		pageLayoutLayout->setSpacing( 5 );
		pageLayoutLayout->setMargin( 10 );
		pageLayoutLayout->setAlignment( Qt::AlignTop );
		singlePage = new QRadioButton( pageLayout );
		singlePage->setText( tr( "Single Page" ) );
		pageLayoutLayout->addWidget( singlePage );
		continuousPages = new QRadioButton( pageLayout );
		continuousPages->setText( tr( "Continuous" ) );
		pageLayoutLayout->addWidget( continuousPages );
		doublePageLeft = new QRadioButton( pageLayout );
		doublePageLeft->setText( tr( "Double Page Left" ) );
		pageLayoutLayout->addWidget( doublePageLeft );
		doublePageRight = new QRadioButton( pageLayout );
		doublePageRight->setText( tr( "Double Page Right" ) );
		pageLayoutLayout->addWidget( doublePageRight );
		LayoutSpecial->addWidget( pageLayout );
		groupNavigation = new QGroupBox( groupDisplay );
		groupNavigation->setTitle( tr( "Visual Appearance" ) );
		groupNavigationLayout = new QVBoxLayout( groupNavigation );
		groupNavigationLayout->setSpacing( 5 );
		groupNavigationLayout->setMargin( 10 );
		groupNavigationLayout->setAlignment( Qt::AlignTop );
		useViewDefault = new QRadioButton( groupNavigation );
		useViewDefault->setText( tr( "Use Viewers Defaults" ) );
		groupNavigationLayout->addWidget( useViewDefault );
		useFullScreen = new QRadioButton( groupNavigation );
		useFullScreen->setText( tr( "Use Full Screen Mode" ) );
		groupNavigationLayout->addWidget( useFullScreen );
		useBookmarks = new QRadioButton( groupNavigation );
		useBookmarks->setText( tr( "Display Bookmarks Tab" ) );
		groupNavigationLayout->addWidget( useBookmarks );
		useThumbnails = new QRadioButton( groupNavigation );
		useThumbnails->setText( tr( "Display Thumbnails" ) );
		groupNavigationLayout->addWidget( useThumbnails );
		useLayers2 = new QRadioButton( groupNavigation );
		useLayers2->setText( tr( "Display Layers Tab" ) );
		groupNavigationLayout->addWidget( useLayers2 );
		hideToolBar = new QCheckBox( tr( "Hide Viewers Toolbar" ), groupNavigation );
		groupNavigationLayout->addWidget( hideToolBar );
		hideMenuBar = new QCheckBox( tr( "Hide Viewers Menubar" ), groupNavigation );
		groupNavigationLayout->addWidget( hideMenuBar );
		fitWindow = new QCheckBox( tr( "Zoom Pages to fit Viewer Window" ), groupNavigation );
		groupNavigationLayout->addWidget( fitWindow );
		LayoutSpecial->addWidget( groupNavigation );
		groupDisplayLayout->addLayout( LayoutSpecial );
		tabSpecialLayout->addWidget( groupDisplay );
		groupJava = new QGroupBox( tabSpecial );
		groupJava->setTitle( tr( "Special Actions" ) );
		groupJavaLayout = new QHBoxLayout( groupJava );
		groupJavaLayout->setSpacing( 5 );
		groupJavaLayout->setMargin( 10 );
		groupJavaLayout->setAlignment( Qt::AlignTop );
		actionLabel = new QLabel( groupJava );
		actionLabel->setText( tr( "Javascript to be executed\nwhen PDF document is opened:" ) );
		groupJavaLayout->addWidget( actionLabel );
		actionCombo = new QComboBox(groupJava);
		actionCombo->setEditable(false);
		actionCombo->addItem( tr("No Script"));
		groupJavaLayout->addWidget( actionCombo );
		tabSpecialLayout->addWidget( groupJava );
		addTab( tabSpecial, tr("Viewer") );
	}

	tabSecurity = new QWidget( this );
	tabSecurityLayout = new QVBoxLayout( tabSecurity );
	tabSecurityLayout->setSpacing( 5 );
	tabSecurityLayout->setMargin( 10 );
	Encry = new QCheckBox( tr( "&Use Encryption" ), tabSecurity);
	tabSecurityLayout->addWidget( Encry );
	GroupPass = new QGroupBox( tr( "Passwords" ), tabSecurity );
	GroupPass->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	GroupPassLayout = new QGridLayout( GroupPass );
	GroupPassLayout->setSpacing( 5 );
	GroupPassLayout->setMargin( 10 );
	GroupPassLayout->setAlignment( Qt::AlignTop );
	TextSec2 = new QLabel( tr( "&User:" ), GroupPass);
	GroupPassLayout->addWidget( TextSec2, 1, 0 );
	TextSec1 = new QLabel( tr( "&Owner:" ), GroupPass);
	GroupPassLayout->addWidget( TextSec1, 0, 0 );
	PassOwner = new QLineEdit( GroupPass );
	PassOwner->setEchoMode( QLineEdit::Password );
	TextSec1->setBuddy(PassOwner);
	GroupPassLayout->addWidget( PassOwner, 0, 1 );
	PassUser = new QLineEdit( GroupPass );
	PassUser->setEchoMode( QLineEdit::Password );
	TextSec2->setBuddy(PassUser);
	GroupPassLayout->addWidget( PassUser, 1, 1 );
	tabSecurityLayout->addWidget( GroupPass );

	GroupSecSet = new QGroupBox( tr( "Settings" ), tabSecurity);
	GroupSecSet->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	GroupSecSetLayout = new QVBoxLayout( GroupSecSet );
	GroupSecSetLayout->setSpacing( 5 );
	GroupSecSetLayout->setMargin( 10 );
	GroupSecSetLayout->setAlignment( Qt::AlignTop );
	PrintSec = new QCheckBox( tr( "Allow &Printing the Document" ), GroupSecSet);
	GroupSecSetLayout->addWidget( PrintSec );
	ModifySec = new QCheckBox( tr( "Allow &Changing the Document" ), GroupSecSet);
	GroupSecSetLayout->addWidget( ModifySec );
	CopySec = new QCheckBox( tr( "Allow Cop&ying Text and Graphics" ), GroupSecSet);
	GroupSecSetLayout->addWidget( CopySec );
	AddSec = new QCheckBox( tr( "Allow Adding &Annotations and Fields" ), GroupSecSet);
	GroupSecSetLayout->addWidget( AddSec );
	tabSecurityLayout->addWidget( GroupSecSet );
	addTab( tabSecurity, tr( "S&ecurity" ) );

	tabColor = new QWidget( this );
	tabColorLayout = new QVBoxLayout( tabColor );
	tabColorLayout->setSpacing( 5 );
	tabColorLayout->setMargin( 10 );
	ColorGroup = new QGroupBox( tr( "General" ), tabColor);
	ColorGroupLayout = new QHBoxLayout( ColorGroup );
	ColorGroupLayout->setSpacing( 5 );
	ColorGroupLayout->setMargin( 10 );
	ColorGroupLayout->setAlignment( Qt::AlignTop );
	ColorText1 = new QLabel( tr( "Output &Intended For:" ), ColorGroup );
	ColorGroupLayout->addWidget( ColorText1 );
	OutCombo = new QComboBox( ColorGroup );
	OutCombo->addItem( tr( "Screen / Web" ) );
	OutCombo->addItem( tr( "Printer" ) );
	OutCombo->addItem( tr( "Grayscale" ) );
	OutCombo->setEditable(false);
	ColorText1->setBuddy(OutCombo);
	ColorGroupLayout->addWidget( OutCombo );
	tabColorLayout->addWidget( ColorGroup );

	useSpot = new QCheckBox( tr( "Convert Spot Colors to Process Colors" ), tabColor);
	tabColorLayout->addWidget( useSpot );

	overprintMode = new QCheckBox( tr( "Force Overprint Mode" ), tabColor);
	tabColorLayout->addWidget( overprintMode );

	UseLPI = new QCheckBox( tr( "&Use Custom Rendering Settings" ), tabColor);
	tabColorLayout->addWidget( UseLPI );
	LPIgroup = new QGroupBox( tr( "Rendering Settings" ), tabColor);
	LPIgroupLayout = new QGridLayout( LPIgroup );
	LPIgroupLayout->setSpacing( 5 );
	LPIgroupLayout->setMargin( 10 );
	LPIgroupLayout->setAlignment( Qt::AlignTop );
	LPIcolor = new QComboBox(LPIgroup);
	LPIcolor->setEditable(false);
	LPIgroupLayout->addWidget( LPIcolor, 0, 0 );
	textLPI1 = new QLabel( tr( "Fre&quency:" ), LPIgroup);
	LPIgroupLayout->addWidget( textLPI1, 0, 1 );
	LPIfreq = new QSpinBox( LPIgroup );
	LPIfreq->setMinimum(10);
	LPIfreq->setMaximum(1000);
	textLPI1->setBuddy(LPIfreq);
	LPIgroupLayout->addWidget( LPIfreq, 0, 2 );
	textLPI2 = new QLabel( tr( "&Angle:" ), LPIgroup);
	LPIgroupLayout->addWidget( textLPI2, 1, 1 );
	LPIangle = new QSpinBox( LPIgroup );
	LPIangle->setSuffix( QString::fromUtf8(" °"));
	LPIangle->setMinimum(-180);
	LPIangle->setMaximum(180);
	textLPI2->setBuddy(LPIangle);
	LPIgroupLayout->addWidget( LPIangle, 1, 2 );
	textLPI3 = new QLabel( tr( "S&pot Function:" ), LPIgroup );
	LPIgroupLayout->addWidget( textLPI3, 2, 1 );
	LPIfunc = new QComboBox(LPIgroup);
	LPIfunc->setEditable(false);
	LPIfunc->addItem( tr( "Simple Dot" ) );
	LPIfunc->addItem( tr( "Line" ) );
	LPIfunc->addItem( tr( "Round" ) );
	LPIfunc->addItem( tr( "Ellipse" ) );
	textLPI3->setBuddy(LPIfunc);
	LPIgroupLayout->addWidget( LPIfunc, 2, 2 );
	tabColorLayout->addWidget( LPIgroup );
	SelLPIcolor = LPIcolor->currentText();
	
	if (doc==0)
	{
		UseLPI->hide();
		LPIgroup->hide();
	}
	
	GroupBox9 = new QGroupBox( tr( "Solid Colors:" ), tabColor);
	GroupBox9Layout = new QGridLayout( GroupBox9 );
	GroupBox9Layout->setSpacing( 5 );
	GroupBox9Layout->setMargin( 10 );
	GroupBox9Layout->setAlignment( Qt::AlignTop );
	EmbedProfs = new QCheckBox( GroupBox9 );
	EmbedProfs->setText( tr( "Use Color Profile" ) );
	GroupBox9Layout->addWidget( EmbedProfs, 0, 0, 1, 2 );
	ProfsTxt1 = new QLabel(GroupBox9);
	ProfsTxt1->setText( tr( "Profile:" ) );
	GroupBox9Layout->addWidget( ProfsTxt1, 1, 0 );
	ProfsTxt2 = new QLabel( GroupBox9 );
	ProfsTxt2->setText( tr( "Rendering-Intent:" ) );
	GroupBox9Layout->addWidget( ProfsTxt2, 1, 1 );
	SolidPr = new QComboBox(GroupBox9);
	SolidPr->setEditable(false);
	GroupBox9Layout->addWidget( SolidPr, 2, 0 );
	IntendS = new QComboBox(GroupBox9);
	QString tmp_ip[] = { tr("Perceptual"), tr("Relative Colorimetric"), tr("Saturation"), tr("Absolute Colorimetric")};
	size_t ar_ip = sizeof(tmp_ip) / sizeof(*tmp_ip);
	for (uint a = 0; a < ar_ip; ++a)
		IntendS->addItem(tmp_ip[a]);
	IntendS->setEditable(false);
	GroupBox9Layout->addWidget( IntendS, 2, 1 );
	tabColorLayout->addWidget( GroupBox9 );
	ProfsGroup = new QGroupBox( tr( "Images:" ), tabColor );
	ProfsGroupLayout = new QGridLayout( ProfsGroup );
	ProfsGroupLayout->setSpacing( 5 );
	ProfsGroupLayout->setMargin( 10 );
	ProfsGroupLayout->setAlignment( Qt::AlignTop );
	EmbedProfs2 = new QCheckBox( tr( "Use Color Profile" ), ProfsGroup );
	ProfsGroupLayout->addWidget( EmbedProfs2, 0, 0, 1, 2 );
	NoEmbedded = new QCheckBox( ProfsGroup );
	NoEmbedded->setText( tr( "Do not use embedded color profiles" ) );
	ProfsGroupLayout->addWidget( NoEmbedded, 1, 0, 1, 2 );
	ProfsTxt3 = new QLabel( ProfsGroup );
	ProfsTxt3->setText( tr( "Profile:" ) );
	ProfsGroupLayout->addWidget( ProfsTxt3, 2, 0 );
	ProfsTxt4 = new QLabel( ProfsGroup );
	ProfsTxt4->setText( tr( "Rendering-Intent:" ) );
	ProfsGroupLayout->addWidget( ProfsTxt4, 2, 1 );
	ImageP = new QComboBox(ProfsGroup );
	ImageP->setEditable(false);
	ProfsGroupLayout->addWidget( ImageP, 3, 0 );
	IntendI = new QComboBox(ProfsGroup);
	for (uint a = 0; a < ar_ip; ++a)
		IntendI->addItem(tmp_ip[a]);
	IntendI->setEditable(false);
	ProfsGroupLayout->addWidget( IntendI, 3, 1 );
	tabColorLayout->addWidget( ProfsGroup );

	QSpacerItem* spacerCG = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	tabColorLayout->addItem( spacerCG );
	addTab( tabColor, tr( "C&olor" ) );

	tabPDFX = new QWidget( this );
	tabPDFXLayout = new QVBoxLayout( tabPDFX );
	tabPDFXLayout->setSpacing( 5 );
	tabPDFXLayout->setMargin( 10 );

	MarkGroup = new QGroupBox( tabPDFX );
	MarkGroup->setTitle( tr( "Printer Marks" ) );
	MarkGroupLayout = new QGridLayout( MarkGroup );
	MarkGroupLayout->setSpacing( 5 );
	MarkGroupLayout->setMargin( 10 );
	MarkGroupLayout->setAlignment( Qt::AlignTop );
	cropMarks = new QCheckBox( tr( "Crop Marks" ), MarkGroup );
	MarkGroupLayout->addWidget( cropMarks, 0, 0 );
	bleedMarks = new QCheckBox( tr( "Bleed Marks" ), MarkGroup );
	MarkGroupLayout->addWidget( bleedMarks, 1, 0 );
	registrationMarks = new QCheckBox( tr( "Registration Marks" ), MarkGroup );
	MarkGroupLayout->addWidget( registrationMarks, 2, 0 );
	colorMarks = new QCheckBox( tr( "Color Bars" ), MarkGroup);
	MarkGroupLayout->addWidget( colorMarks, 0, 1, 1, 2 );
	docInfoMarks = new QCheckBox( tr( "Page Information" ), MarkGroup );
	MarkGroupLayout->addWidget( docInfoMarks, 1, 1, 1, 2 );
	MarkTxt1 = new QLabel( MarkGroup );
	MarkTxt1->setText( tr( "Offset:" ) );
	MarkGroupLayout->addWidget( MarkTxt1, 2, 1 );
	markOffset = new ScrSpinBox( MarkGroup, unitIndex );
	MarkGroupLayout->addWidget( markOffset, 2, 2 );
	markOffset->setSuffix( unit );
	markOffset->setMinimum(0);
	markOffset->setMaximum(3000 * unitRatio);
	tabPDFXLayout->addWidget( MarkGroup );

	BleedGroup = new QGroupBox( tabPDFX );
	BleedGroup->setTitle( tr( "Bleed Settings" ) );
	BleedGroupLayout = new QGridLayout( BleedGroup );
	BleedGroupLayout->setSpacing( 5 );
	BleedGroupLayout->setMargin( 10 );
	BleedGroupLayout->setAlignment( Qt::AlignTop );
	BleedTxt1 = new QLabel( BleedGroup );
	BleedTxt1->setText( tr( "Top:" ) );
	BleedGroupLayout->addWidget( BleedTxt1, 0, 0 );
	BleedTop = new ScrSpinBox( BleedGroup, unitIndex );
	BleedGroupLayout->addWidget( BleedTop, 0, 1 );
	BleedTxt2 = new QLabel( BleedGroup );
	BleedTxt2->setText( tr( "Bottom:" ) );
	BleedGroupLayout->addWidget( BleedTxt2, 1, 0 );
	BleedBottom = new ScrSpinBox( BleedGroup, unitIndex );
	BleedGroupLayout->addWidget( BleedBottom, 1, 1 );
	BleedTxt3 = new QLabel( BleedGroup );
	BleedTxt3->setText( tr( "Left:" ) );
	BleedGroupLayout->addWidget( BleedTxt3, 0, 2 );
	BleedLeft = new ScrSpinBox( BleedGroup, unitIndex );
	BleedGroupLayout->addWidget( BleedLeft, 0, 3 );
	BleedTxt4 = new QLabel( BleedGroup );
	BleedTxt4->setText( tr( "Right:" ) );
	BleedGroupLayout->addWidget( BleedTxt4, 1, 2 );
	BleedRight = new ScrSpinBox( BleedGroup, unitIndex );
	BleedGroupLayout->addWidget( BleedRight, 1, 3 );
	docBleeds = new QCheckBox( tr( "Use Document Bleeds" ), BleedGroup );
	BleedGroupLayout->addWidget( docBleeds, 2, 0, 1, 4 );
	tabPDFXLayout->addWidget( BleedGroup );

	X3Group = new QGroupBox( tabPDFX );
	X3Group->setTitle( tr( "PDF/X-3 Output Intent" ) );
	X3GroupLayout = new QGridLayout( X3Group );
	X3GroupLayout->setSpacing( 5 );
	X3GroupLayout->setMargin( 10 );
	X3GroupLayout->setAlignment( Qt::AlignTop );
	PrintProfC = new QComboBox( X3Group );
	PrintProfC->setEditable(false);
	X3GroupLayout->addWidget( PrintProfC, 0, 1 );
	InfoString = new QLineEdit( X3Group );
	X3GroupLayout->addWidget( InfoString, 1, 1 );
	PDFX2 = new QLabel( tr( "&Info String:" ), X3Group );
	PDFX2->setBuddy(InfoString);
	X3GroupLayout->addWidget( PDFX2, 1, 0 );
	PDFX1 = new QLabel( tr( "Output &Profile:" ), X3Group );
	PDFX1->setBuddy(PrintProfC);
	X3GroupLayout->addWidget( PDFX1, 0, 0 );
	tabPDFXLayout->addWidget( X3Group );

	QSpacerItem* spacerPX2 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	tabPDFXLayout->addItem( spacerPX2 );

	addTab( tabPDFX, tr( "Pre-Press" ) );
	BleedTop->setSuffix( unit );
	BleedTop->setMinimum(0);
	BleedTop->setMaximum(3000*unitRatio);
	BleedBottom->setSuffix( unit );
	BleedBottom->setMinimum(0);
	BleedBottom->setMaximum(3000*unitRatio);
	BleedRight->setSuffix( unit );
	BleedRight->setMinimum(0);
	BleedRight->setMaximum(3000*unitRatio);
	BleedLeft->setSuffix( unit );
	BleedLeft->setMinimum(0);
	BleedLeft->setMaximum(3000*unitRatio);

	restoreDefaults(Optionen, AllFonts, PDFXProfiles, DocFonts, Eff, unitIndex, PageH, PageB, doc, pdfExport);

	if (doc != 0 && exporting)
	{
		connect(EmbedFonts, SIGNAL(clicked()), this, SLOT(EmbedAll()));
		connect(AvailFlist, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelAFont(QListWidgetItem*)));
		connect(EmbedList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelEFont(QListWidgetItem*)));
		connect(ToEmbed, SIGNAL(clicked()), this, SLOT(PutToEmbed()));
		connect(FromEmbed, SIGNAL(clicked()), this, SLOT(RemoveEmbed()));
		connect(OutlineFonts, SIGNAL(clicked()), this, SLOT(OutlineAll()));
		connect(OutlineList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelSFont(QListWidgetItem*)));
		connect(ToOutline, SIGNAL(clicked()), this, SLOT(PutToOutline()));
		connect(FromOutline, SIGNAL(clicked()), this, SLOT(RemoveOutline()));
		connect(PagePrev, SIGNAL(clicked()), this, SLOT(PagePr()));
		connect(Pages, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetPgEff()));
		connect(EffectType, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));
		connect(EDirection_2_2, SIGNAL(activated(int)), this, SLOT(ValidDI(int)));
		connect(CheckBox10, SIGNAL(clicked()), this, SLOT(DoEffects()));
		connect(EonAllPg, SIGNAL(clicked()), this, SLOT(EffectOnAll()));
		connect(InfoString, SIGNAL(textChanged(const QString &)), this, SLOT(checkInfo()));
		connect(InfoString, SIGNAL(editingFinished()), this, SLOT(checkInfo()));
		connect(docBleeds, SIGNAL(clicked()), this, SLOT(doDocBleeds()));
		EmbedFonts->setToolTip( "<qt>" + tr( "Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document." ) + "</qt>");
		CheckBox10->setToolTip( "<qt>" + tr( "Enables presentation effects when using Adobe&#174; Reader&#174; and other PDF viewers which support this in full screen mode." ) + "</qt>");
		PagePrev->setToolTip( "<qt>" + tr( "Show page previews of each page listed above." ) + "</qt>");
		PageTime->setToolTip( "<qt>" + tr( "Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition." ) + "</qt>" );
		EffectTime->setToolTip( "<qt>" + tr( "Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down." ) + "</qt>" );
		EffectType->setToolTip( "<qt>" + tr( "Type of the display effect." ) + "</qt>" );
		EDirection->setToolTip( "<qt>" + tr( "Direction of the effect of moving lines for the split and blind effects." ) + "</qt>" );
		EDirection_2->setToolTip( "<qt>" + tr( "Starting position for the box and split effects." ) + "</qt>" );
		EDirection_2_2->setToolTip( "<qt>" + tr( "Direction of the glitter or wipe effects." ) + "</qt>" );
		EonAllPg->setToolTip( "<qt>" + tr( "Apply the selected effect to all pages." ) + "</qt>" );
		OutlineFonts->setToolTip( "<qt>" + tr("Convert all glyphs in the document to outlines.") + "</qt>");
		//Viewer tab
		singlePage->setToolTip( "<qt>" + tr( "Show the document in single page mode" ) + "</qt>" );
		continuousPages->setToolTip( "<qt>" + tr( "Show the document in single page mode with the pages displayed continuously end to end like a scroll" ) + "</qt>" );
		doublePageLeft->setToolTip( "<qt>" + tr( "Show the document with facing pages, starting with the first page displayed on the left" ) + "</qt>" );
		doublePageRight->setToolTip( "<qt>" + tr( "Show the document with facing pages, starting with the first page displayed on the right" ) + "</qt>" );
		useViewDefault->setToolTip( "<qt>" + tr( "Use the viewer's defaults or the user's preferences if set differently from the viewer defaults" ) + "</qt>" );
		useFullScreen->setToolTip( "<qt>" + tr( "Enables viewing the document in full screen" ) + "</qt>" );
		useBookmarks->setToolTip( "<qt>" + tr( "Display the bookmarks upon opening" ) + "</qt>" );
		useThumbnails->setToolTip( "<qt>" + tr( "Display the page thumbnails upon opening" ) + "</qt>" );
		useLayers2->setToolTip( "<qt>" + tr( "Forces the displaying of layers. Useful only for PDF 1.5+." ) + "</qt>" );
		hideToolBar->setToolTip( "<qt>" + tr( "Hides the Tool Bar which has selection and other editing capabilities" ) + "</qt>" );
		hideMenuBar->setToolTip( "<qt>" + tr( "Hides the Menu Bar for the viewer, the PDF will display in a plain window. " ) + "</qt>" );
		fitWindow->setToolTip( "<qt>" + tr( "Fit the document page or pages to the available space in the viewer window." ) + "</qt>" );
	}
	else
		docBleeds->hide();
	connect(AllPages, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect(pageNrButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
	connect(DSColor, SIGNAL(clicked()), this, SLOT(DoDownsample()));
	connect(MirrorH, SIGNAL(clicked()), this, SLOT(PDFMirror()));
	connect(MirrorV, SIGNAL(clicked()), this, SLOT(PDFMirror()));
	connect(RotateDeg, SIGNAL(activated(int)), this, SLOT(Rotation(int)));
	connect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
	connect(EmbedProfs, SIGNAL(clicked()), this, SLOT(EnablePG()));
	connect(EmbedProfs2, SIGNAL(clicked()), this, SLOT(EnablePGI()));
	connect(NoEmbedded, SIGNAL(clicked()), this, SLOT(EnablePGI2()));
	connect(PDFVersionCombo, SIGNAL(activated(int)), this, SLOT(EnablePDFX(int)));
	connect(Encry, SIGNAL(clicked()), this, SLOT(ToggleEncr()));
	connect(UseLPI, SIGNAL(clicked()), this, SLOT(EnableLPI2()));
	connect(LPIcolor, SIGNAL(activated(int)), this, SLOT(SelLPIcol(int)));
	//tooltips
	AllPages->setToolTip( "<qt>" + tr( "Export all pages to PDF" ) + "</qt>" );
	OnlySome->setToolTip( "<qt>" + tr( "Export a range of pages to PDF" ) );
	PageNr->setToolTip( "<qt>" + tr( "Insert a comma separated list of tokens where "
		                                    "a token can be * for all the pages, 1-5 for "
		                                    "a range of pages or a single page number.") + "</qt>" );

	PDFVersionCombo->setToolTip( "<qt>" + tr( "Determines the PDF compatibility.<br/>The default is <b>PDF 1.3</b> which gives the widest compatibility.<br/>Choose <b>PDF 1.4</b> if your file uses features such as transparency or you require 128 bit encryption.<br/><b>PDF 1.5</b> is necessary when you wish to preserve objects in separate layers within the PDF.<br/><b>PDF/X-3</b> is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer." ) + "</qt>");
	ComboBind->setToolTip( "<qt>" + tr( "Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left." ) + "</qt>" );
	CheckBox1->setToolTip( "<qt>" + tr( "Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation." ) + "</qt>" );
	Article->setToolTip( "<qt>" + tr( "Generate PDF Articles, which is useful for navigating linked articles in a PDF." ) + "</qt>" );
	useLayers->setToolTip( "<qt>" + tr( "Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen." ) + "</qt>" );
	CheckBM->setToolTip( "<qt>" + tr( "Embed the bookmarks you created in your document. These are useful for navigating long PDF documents." ) + "</qt>" );
	Resolution->setToolTip( "<qt>" + tr( "Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos." ) + "</qt>" );
	Compression->setToolTip( "<qt>" + tr( "Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size." ) + "</qt>" );
	CMethod->setToolTip( "<qt>" + tr( "Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options." ) + "</qt>");
	CQuality->setToolTip( "<qt>" + tr( "Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg." ) + "</qt>");
	DSColor->setToolTip( "<qt>" + tr( "Limits the resolution of your bitmap images to the selected DPI. Images with a lower resolution will be left untouched. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export." ) + "</qt>" );
	ValC->setToolTip( "<qt>" + tr( "DPI (Dots Per Inch) for image export.") + "</qt>" );
	Encry->setToolTip( "<qt>" + tr( "Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations." ) + "</qt>" );
	PassOwner->setToolTip( "<qt>" + tr( "Choose a master password which enables or disables all the security features in your exported PDF" ) + "</qt>" );
	PassUser->setToolTip( "<qt>" + tr( "Choose a password for users to be able to read your PDF." ) + "</qt>" );
	PrintSec->setToolTip( "<qt>" + tr( "Allow printing of the PDF. If un-checked, printing is prevented. " ) + "</qt>" );
	ModifySec->setToolTip( "<qt>" + tr( "Allow modifying of the PDF. If un-checked, modifying the PDF is prevented." ) + "</qt>" );
	CopySec->setToolTip( "<qt>" + tr( "Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied." ) + "</qt>" );
	AddSec->setToolTip( "<qt>" + tr( "Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented." ) + "</qt>" );
	OutCombo->setToolTip( "<qt>" + tr( "Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF." ) + "</qt>" );
	UseLPI->setToolTip( "<qt>" + tr( "This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems." ) + "</qt>" );
	EmbedProfs->setToolTip( "<qt>" + tr( "Embed a color profile for solid colors" ) + "</qt>" );
	SolidPr->setToolTip( "<qt>" + tr( "Color profile for solid colors" ) + "</qt>" );
	IntendS->setToolTip( "<qt>" + tr( "Rendering intent for solid colors" ) + "</qt>" );
	EmbedProfs2->setToolTip( "<qt>" + tr( "Embed a color profile for images" ) + "</qt>" );
	NoEmbedded->setToolTip( "<qt>" + tr( "Do not use color profiles that are embedded in source images" ) + "</qt>" );
	ImageP->setToolTip( "<qt>" + tr( "Color profile for images" ) + "</qt>" );
	IntendI->setToolTip( "<qt>" + tr( "Rendering intent for images" ) + "</qt>" );
	MirrorH->setToolTip( "<qt>" + tr( "Mirror Page(s) horizontally" ) + "</qt>" );
	MirrorV->setToolTip( "<qt>" + tr( "Mirror Page(s) vertically" ) + "</qt>" );
	overprintMode->setToolTip( "<qt>"+ tr("Enables global Overprint Mode for this document, overrides object settings") + "<qt>");
	useSpot->setToolTip("<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");
	ClipMarg->setToolTip( "<qt>" + tr( "Do not show objects outside the margins in the exported file" ) + "</qt>" );
	//PrePress tab 
	cropMarks->setToolTip( "<qt>" + tr( "Creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing" ) + "</qt>" );
	bleedMarks->setToolTip( "<qt>" + tr( "This creates bleed marks which are indicated by  _ . _ and show the bleed limit" ) + "</qt>" );
	registrationMarks->setToolTip( "<qt>" + tr( "Add registration marks to each separation" ) + "</qt>" );
	colorMarks->setToolTip( "<qt>" + tr( "Add color calibration bars" ) + "</qt>" );
	docInfoMarks->setToolTip( "<qt>" + tr( "Add document information which includes the document title and page numbers" ) + "</qt>" );
	markOffset->setToolTip( "<qt>" + tr( "Indicate the distance offset for the registration marks" ) + "</qt>" );
	BleedTop->setToolTip( "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
	BleedBottom->setToolTip( "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
	BleedLeft->setToolTip( "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
	BleedRight->setToolTip( "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
	docBleeds->setToolTip( "<qt>" + tr( "Use the existing bleed settings from the document preferences" ) + "</qt>" );
	PrintProfC->setToolTip( "<qt>" + tr( "Output profile for printing. If possible, get some guidance from your printer on profile selection." ) + "</qt>" );
	InfoString->setToolTip( "<qt>" + tr( "Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document." ) + "</qt>" );
}

void TabPDFOptions::restoreDefaults(PDFOptions & Optionen,
									const SCFonts &AllFonts,
									const ProfilesL & PDFXProfiles,
									const QMap<QString, int> & DocFonts,
									const QList<PDFPresentationData> & Eff,
									int unitIndex, double PageH, double PageB,
									ScribusDoc * mdoc, bool exporting)
{
	AllPages->setChecked( true );
	PageNr->setEnabled(false);
	pageNrButton->setEnabled(false);
	RotateDeg->setCurrentIndex(Opts.RotateDeg / 90);
	MirrorH->setChecked(Opts.MirrorH);
	MirrorV->setChecked(Opts.MirrorV);
	ClipMarg->setChecked(Opts.doClip);
	bool cmsUse = mdoc ? (ScCore->haveCMS() && mdoc->HasCMS) : false;
	if (cmsUse)
	{
		if (Opts.Version == PDFOptions::PDFVersion_X3)
			PDFVersionCombo->setCurrentIndex(3);
	}
	else
		PDFVersionCombo->setCurrentIndex(0);
	if (Opts.Version == PDFOptions::PDFVersion_13)
		PDFVersionCombo->setCurrentIndex(0);
	if (Opts.Version == PDFOptions::PDFVersion_14)
		PDFVersionCombo->setCurrentIndex(1);
	if (Opts.Version == PDFOptions::PDFVersion_15)
		PDFVersionCombo->setCurrentIndex(2);
	ComboBind->setCurrentIndex(Opts.Binding);
	CheckBox1->setChecked(Opts.Thumbnails);
	Article->setChecked(Opts.Articles);
	CheckBM->setChecked(Opts.Bookmarks);
	useLayers->setChecked(Opts.useLayers);
	if (Opts.Version == 15)
		useLayers->setEnabled(true);
	else
		useLayers->setEnabled(false);
	Resolution->setValue(Opts.Resolution);
	Compression->setChecked( Opts.Compress );
	CMethod->setCurrentIndex(Opts.CompressMethod);
	CQuality->setCurrentIndex(Opts.Quality);
	DSColor->setChecked(Opts.RecalcPic);
	ValC->setValue(Opts.PicRes);
	ValC->setEnabled(DSColor->isChecked() ? true : false);
	if (mdoc != 0 && exporting)
	{
//	Build a list of all Fonts used in Annotations;
		PageItem *pgit;
		for (int c = 0; c < doc->FrameItems.count(); ++c)
		{
			pgit = doc->FrameItems.at(c);
			if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()))
				AnnotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
		}
		for (int c = 0; c < doc->MasterItems.count(); ++c)
		{
			pgit = doc->MasterItems.at(c);
			if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()))
				AnnotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
		}
		for (int c = 0; c < doc->DocItems.count(); ++c)
		{
			pgit = doc->DocItems.at(c);
			if (((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText)) && (pgit->isAnnotation()))
				AnnotationFonts.insert(pgit->itemText.defaultStyle().charStyle().font().replacementName(), "");
		}
		QMap<QString,int>::const_iterator it;
		AvailFlist->clear();
		for (it = DocFonts.constBegin(); it != DocFonts.constEnd(); ++it)
		{
			if (AllFonts[it.key()].isReplacement())
				new QListWidgetItem( QIcon(loadIcon("font_subst16.png")), it.key(), AvailFlist );
			else if (AllFonts[it.key()].type() == ScFace::TYPE1)
				new QListWidgetItem( QIcon(loadIcon("font_type1_16.png")), it.key(), AvailFlist );
			else if (AllFonts[it.key()].type() == ScFace::TTF)
				new QListWidgetItem( QIcon(loadIcon("font_truetype16.png")), it.key(), AvailFlist );
			else if (AllFonts[it.key()].type() == ScFace::OTF)
				new QListWidgetItem( QIcon(loadIcon("font_otf16.png")), it.key(), AvailFlist );
		}
		ToEmbed->setEnabled(false);
		FromEmbed->setEnabled(false);
		ToOutline->setEnabled(false);
		FromOutline->setEnabled(false);
		if ((Opts.EmbedList.count() == 0) && (Opts.SubsetList.count() == 0) && (Opts.firstUse))
			EmbedAll();
		else
		{
			EmbedList->clear();
			FontsToEmbed.clear();
			for (int fe = 0; fe < Opts.EmbedList.count(); ++fe)
			{
				EmbedList->addItem(Opts.EmbedList[fe]);
				FontsToEmbed.append(Opts.EmbedList[fe]);
			}
			if (Opts.SubsetList.count() != 0)
			{
				OutlineList->clear();
				FontsToOutline.clear();
				for (int fe = 0; fe < Opts.SubsetList.count(); ++fe)
				{
					OutlineList->addItem(Opts.SubsetList[fe]);
					FontsToOutline.append(Opts.SubsetList[fe]);
				}
			}
			QMap<QString, QString>::Iterator itAnn;
			for (itAnn = AnnotationFonts.begin(); itAnn != AnnotationFonts.end(); ++itAnn)
			{
				if (FontsToEmbed.contains(itAnn.key()) == 0)
				{
					EmbedList->addItem(itAnn.key());
					EmbedList->item(EmbedList->count()-1)->setFlags(Qt::ItemIsEnabled);
					FontsToEmbed.append(itAnn.key());
				}
				if (FontsToOutline.contains(itAnn.key()) != 0)
				{
					FontsToOutline.removeAll(itAnn.key());
					QList<QListWidgetItem *> itR = OutlineList->findItems(itAnn.key(), Qt::MatchExactly);
					delete OutlineList->takeItem(OutlineList->row(itR.at(0)));
				}
			}
		}
		CheckBox10->setChecked(Opts.PresentMode);
		QString tmp;
		struct PDFPresentationData ef;
		Pages->clear();
		EffVal.clear();
		if (EffVal.count() != 0)
		{
			for (int pg2 = 0; pg2 < doc->Pages->count(); ++pg2)
			{
				Pages->addItem( tr("Page")+" "+tmp.setNum(pg2+1));
				if (EffVal.count()-1 < pg2)
				{
					ef.pageEffectDuration = 1;
					ef.pageViewDuration = 1;
					ef.effectType = 0;
					ef.Dm = 0;
					ef.M = 0;
					ef.Di = 0;
					EffVal.append(ef);
				}
			}
		}
		else
		{
			for (int pg = 0; pg < doc->Pages->count(); ++pg)
			{
				Pages->addItem( tr("Page")+" "+tmp.setNum(pg+1));
				ef.pageEffectDuration = 1;
				ef.pageViewDuration = 1;
				ef.effectType = 0;
				ef.Dm = 0;
				ef.M = 0;
				ef.Di = 0;
				EffVal.append(ef);
			}
		}
		PagePrev->setChecked(false);
		PageTime->setValue(EffVal[0].pageViewDuration);
		EffectTime->setValue(EffVal[0].pageEffectDuration);
		bool df = true;
		if ((Opts.displayBookmarks) || (Opts.displayFullscreen) || (Opts.displayLayers) || (Opts.displayThumbs))
			df = false;
		if (df)
			useViewDefault->setChecked(df);
		useFullScreen->setChecked(Opts.displayFullscreen);
		useBookmarks->setChecked(Opts.displayBookmarks);
		useThumbnails->setChecked(Opts.displayThumbs);
		useLayers2->setChecked(Opts.displayLayers);
		hideToolBar->setChecked(Opts.hideToolBar);
		hideMenuBar->setChecked(Opts.hideMenuBar);
		fitWindow->setChecked(Opts.fitWindow);
		QMap<QString,QString>::Iterator itja;
		actionCombo->clear();
		actionCombo->addItem( tr("No Script"));
		for (itja = doc->JavaScripts.begin(); itja != doc->JavaScripts.end(); ++itja)
			actionCombo->addItem(itja.key());
		if (doc->JavaScripts.contains(Opts.openAction))
			setCurrentComboItem(actionCombo, Opts.openAction);
		if (Opts.PageLayout == PDFOptions::SinglePage)
			singlePage->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::OneColumn)
			continuousPages->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::TwoColumnLeft)
			doublePageLeft->setChecked(true);
		else if (Opts.PageLayout == PDFOptions::TwoColumnRight)
			doublePageRight->setChecked(true);
		if (Opts.Version == 15)
			useLayers2->setEnabled(true);
		else
			useLayers2->setEnabled(false);
	}

	Encry->setChecked( Opts.Encrypt );
	PassOwner->setText(Opts.PassOwner);
	PassUser->setText(Opts.PassUser);
	PrintSec->setChecked( Opts.Permissions & 4 );
	ModifySec->setChecked( Opts.Permissions & 8 );
	CopySec->setChecked( Opts.Permissions & 16 );
	AddSec->setChecked( Opts.Permissions & 32 );
	if (!Encry->isChecked())
	{
		GroupSecSet->setEnabled(false);
		GroupPass->setEnabled(false);
	}

	if (Opts.UseRGB)
		OutCombo->setCurrentIndex(0);
	else
	{
		if (Opts.isGrayscale)
			OutCombo->setCurrentIndex(2);
		else
			OutCombo->setCurrentIndex(1);
	}
	useSpot->setChecked(!Opts.UseSpotColors);
	overprintMode->setChecked(Opts.doOverprint);
	UseLPI->setChecked(Opts.UseLPI);
	QMap<QString,LPIData>::Iterator itlp;
	LPIcolor->clear();
	for (itlp = Opts.LPISettings.begin(); itlp != Opts.LPISettings.end(); ++itlp)
		LPIcolor->addItem( itlp.key() );
	LPIcolor->setCurrentIndex(0);

	LPIfreq->setValue(Opts.LPISettings[LPIcolor->currentText()].Frequency);
	LPIangle->setValue(Opts.LPISettings[LPIcolor->currentText()].Angle);
	LPIfunc->setCurrentIndex(Opts.LPISettings[LPIcolor->currentText()].SpotFunc);
	EmbedProfs->setChecked(Opts.UseProfiles);
	EmbedProfs2->setChecked(Opts.UseProfiles2);
	NoEmbedded->setChecked(Opts.EmbeddedI);
	if ((Opts.UseRGB) || (Opts.isGrayscale))
	{
		ProfsGroup->setEnabled(false);
		GroupBox9->setEnabled(false);
		EnablePr(0);
	}
	else
		EnablePr(1);
	EnablePG();
	EnablePGI();
	QString tp = Opts.SolidProf;
	if (!ScCore->InputProfiles.contains(tp))
	{
		if (mdoc != 0 && exporting)
			tp = mdoc->CMSSettings.DefaultSolidColorRGBProfile;
		else
			tp = PrefsManager::instance()->appPrefs.DCMSset.DefaultSolidColorRGBProfile;
	}
	ProfilesL::Iterator itp;
	ProfilesL::Iterator itpend=ScCore->InputProfiles.end();
	SolidPr->clear();
	for (itp = ScCore->InputProfiles.begin(); itp != itpend; ++itp)
	{
		SolidPr->addItem(itp.key());
		if (itp.key() == tp)
		{
			if (cmsUse)
				SolidPr->setCurrentIndex(SolidPr->count()-1);
		}
	}
	if (cmsUse)
		IntendS->setCurrentIndex(Opts.Intent);
	QString tp1 = Opts.ImageProf;
	if (!ScCore->InputProfiles.contains(tp1))
	{
		if (mdoc != 0 && exporting)
			tp1 = mdoc->CMSSettings.DefaultSolidColorRGBProfile;
		else
			tp1 = PrefsManager::instance()->appPrefs.DCMSset.DefaultSolidColorRGBProfile;
	}
	ProfilesL::Iterator itp2;
	ProfilesL::Iterator itp2end=ScCore->InputProfiles.end();
	ImageP->clear();
	for (itp2 = ScCore->InputProfiles.begin(); itp2 != itp2end; ++itp2)
	{
		ImageP->addItem(itp2.key());
		if (itp2.key() == tp1)
		{
			if (cmsUse)
				ImageP->setCurrentIndex(ImageP->count()-1);
		}
	}
	if (cmsUse)
		IntendI->setCurrentIndex(Opts.Intent2);
	if (!cmsUse)
	{
		GroupBox9->hide();
		ProfsGroup->hide();
	}

	ProfilesL::const_iterator itp3;
	QString tp3 = Opts.PrintProf;
	if (!PDFXProfiles.contains(tp3))
	{
		if (mdoc != 0 && exporting)
			tp3 = mdoc->CMSSettings.DefaultPrinterProfile;
		else
			tp3 = PrefsManager::instance()->appPrefs.DCMSset.DefaultPrinterProfile;
	}
	PrintProfC->clear();
	for (itp3 = PDFXProfiles.constBegin(); itp3 != PDFXProfiles.constEnd(); ++itp3)
	{
		PrintProfC->addItem(itp3.key());
		if (itp3.key() == tp3)
			PrintProfC->setCurrentIndex(PrintProfC->count()-1);
	}
	InfoString->setText(Opts.Info);
	BleedTop->setValue(Opts.bleeds.Top*unitRatio);
	BleedBottom->setValue(Opts.bleeds.Bottom*unitRatio);
	BleedRight->setValue(Opts.bleeds.Right*unitRatio);
	BleedLeft->setValue(Opts.bleeds.Left*unitRatio);
	if (mdoc != 0 && exporting)
	{
		docBleeds->setChecked(Opts.useDocBleeds);
		doDocBleeds();
	}
	markOffset->setValue(Opts.markOffset*unitRatio);
	cropMarks->setChecked(Opts.cropMarks);
	bleedMarks->setChecked(Opts.bleedMarks);
	registrationMarks->setChecked(Opts.registrationMarks);
	colorMarks->setChecked(Opts.colorMarks);
	docInfoMarks->setChecked(Opts.docInfoMarks);
	if (!cmsUse)
		X3Group->setEnabled(false);
	if (cmsUse && (Opts.Version == 12) && (!PDFXProfiles.isEmpty()))
		EnablePDFX(3);
	else
		X3Group->setEnabled(false);
	if (mdoc != 0  && exporting)
	{
		PgSel = 0;
		Pages->setCurrentRow(0);
		SetEffOpts(0);
		Pages->setEnabled(false);
		Effects->setEnabled(false);
		PagePrev->setEnabled(false);
		DoEffects();
		if (CheckBox10->isChecked())
		{
			PageTime->setValue(EffVal[0].pageViewDuration);
			EffectTime->setValue(EffVal[0].pageEffectDuration);
			EffectType->setCurrentIndex(EffVal[0].effectType);
			EDirection->setCurrentIndex(EffVal[0].Dm);
			EDirection_2->setCurrentIndex(EffVal[0].M);
			EDirection_2_2->setCurrentIndex(EffVal[0].Di);
			SetEffOpts(EffectType->currentIndex());
		}
		if (mdoc->currentPageLayout != 0)
		{
			BleedTxt3->setText( tr( "Inside:" ) );
			BleedTxt4->setText( tr( "Outside:" ) );
		}
		
	}
}

void TabPDFOptions::doDocBleeds()
{
	if (docBleeds->isChecked())
	{
		Opts.bleeds.Top = BleedTop->value() / unitRatio;
		Opts.bleeds.Bottom = BleedBottom->value() / unitRatio;
		Opts.bleeds.Right = BleedRight->value() / unitRatio;
		Opts.bleeds.Left = BleedLeft->value() / unitRatio;
		BleedTop->setValue(doc->bleeds.Top*unitRatio);
		BleedBottom->setValue(doc->bleeds.Bottom*unitRatio);
		BleedRight->setValue(doc->bleeds.Right*unitRatio);
		BleedLeft->setValue(doc->bleeds.Left*unitRatio);
		BleedTop->setEnabled(false);
		BleedBottom->setEnabled(false);
		BleedRight->setEnabled(false);
		BleedLeft->setEnabled(false);
	}
	else
	{
		BleedTop->setValue(Opts.bleeds.Top*unitRatio);
		BleedBottom->setValue(Opts.bleeds.Bottom*unitRatio);
		BleedRight->setValue(Opts.bleeds.Right*unitRatio);
		BleedLeft->setValue(Opts.bleeds.Left*unitRatio);
		BleedTop->setEnabled(true);
		BleedBottom->setEnabled(true);
		BleedRight->setEnabled(true);
		BleedLeft->setEnabled(true);
	}
}

void TabPDFOptions::checkInfo()
{
	if ((PDFVersionCombo->currentIndex() == 3) && (InfoString->text().isEmpty()))
		emit noInfo();
	else
		emit hasInfo();
}

void TabPDFOptions::ToggleEncr()
{
	bool setter = Encry->isChecked() ? true : false;
	GroupSecSet->setEnabled(setter);
	GroupPass->setEnabled(setter);
}

void TabPDFOptions::enableCMS(bool enable)
{
	disconnect(PDFVersionCombo, SIGNAL(activated(int)), this, SLOT(EnablePDFX(int)));
	int a = PDFVersionCombo->currentIndex();
	PDFVersionCombo->clear();
	PDFVersionCombo->addItem("PDF 1.3 (Acrobat 4)");
	PDFVersionCombo->addItem("PDF 1.4 (Acrobat 5)");
	PDFVersionCombo->addItem("PDF 1.5 (Acrobat 6)");
	cms=enable;
	if (enable)
		PDFVersionCombo->addItem("PDF/X-3");
	else
		a = qMin(a, 3);
	PDFVersionCombo->setCurrentIndex(a);
	EnablePr(1);
	connect(PDFVersionCombo, SIGNAL(activated(int)), this, SLOT(EnablePDFX(int)));
}

void TabPDFOptions::EnablePDFX(int a)
{
	useLayers->setEnabled(a == 2);
	if (useLayers2)
		useLayers2->setEnabled(a == 2);
	if (a != 3)
	{
		X3Group->setEnabled(false);
		setTabEnabled(indexOf(tabSecurity), true);
		OutCombo->setEnabled(true);
		EmbedProfs2->setEnabled(true);
		emit hasInfo();
		if (doc != 0 && pdfExport)
		{
			CheckBox10->setEnabled(true);
			EmbedFonts->setEnabled(true);
			if (EmbedList->count() != 0)
				FromEmbed->setEnabled(true);
			ToEmbed->setEnabled(true);
		}
		return;
	}
	disconnect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
	OutCombo->setCurrentIndex(1);
	OutCombo->setEnabled(false);
	EnablePr(1);
	EmbedProfs2->setChecked(true);
	EmbedProfs2->setEnabled(false);
	if (doc != 0 && pdfExport)
	{
//		EmbedFonts->setChecked(true);
		EmbedAll();
		CheckBox10->setChecked(false);
		CheckBox10->setEnabled(false);
//		EmbedFonts->setEnabled(false);
		FromEmbed->setEnabled(false);
		ToEmbed->setEnabled(false);
		if (InfoString->text().isEmpty())
			emit noInfo();
		else
			emit hasInfo();
	}
	EnablePGI();
	X3Group->setEnabled(true);
	setTabEnabled(indexOf(tabSecurity), false);
	connect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
}

void TabPDFOptions::EnablePGI()
{
	if (EmbedProfs2->isChecked())
	{
		NoEmbedded->setEnabled(true);
		bool setter = NoEmbedded->isChecked() ? true : false;
		ProfsTxt3->setEnabled(setter);
		ProfsTxt4->setEnabled(setter);
		ImageP->setEnabled(setter);
		IntendI->setEnabled(setter);
	}
	else
	{
		ProfsTxt3->setEnabled(false);
		ProfsTxt4->setEnabled(false);
		ImageP->setEnabled(false);
		IntendI->setEnabled(false);
		NoEmbedded->setEnabled(false);
	}
}

void TabPDFOptions::EnablePGI2()
{
	bool setter = NoEmbedded->isChecked() ? true : false;
	ProfsTxt3->setEnabled(setter);
	ProfsTxt4->setEnabled(setter);
	ImageP->setEnabled(setter);
	IntendI->setEnabled(setter);
}

void TabPDFOptions::EnablePG()
{
	bool setter = EmbedProfs->isChecked() ? true : false;
	ProfsTxt1->setEnabled(setter);
	ProfsTxt2->setEnabled(setter);
	SolidPr->setEnabled(setter);
	IntendS->setEnabled(setter);
}

void TabPDFOptions::EnablePr(int a)
{
	EnableLPI(a);
	bool setter = a == 1 ? true : false;
	GroupBox9->setEnabled(setter);
	ProfsGroup->setEnabled(setter);
}

void TabPDFOptions::EnableLPI(int a)
{
	if (a == 1)
	{
		QString tp = Opts.SolidProf;
		if (!ScCore->InputProfiles.contains(tp))
		{
			if (doc != 0)
				tp = doc->CMSSettings.DefaultSolidColorRGBProfile;
			else
				tp = PrefsManager::instance()->appPrefs.DCMSset.DefaultSolidColorRGBProfile;
		}
		SolidPr->clear();
		ProfilesL::Iterator itp;
		ProfilesL::Iterator itpend=ScCore->InputProfiles.end();
		for (itp = ScCore->InputProfiles.begin(); itp != itpend; ++itp)
		{
			SolidPr->addItem(itp.key());
			if (itp.key() == tp)
			{
				if (cms)
					SolidPr->setCurrentIndex(SolidPr->count()-1);
			}
		}
		if (cms)
			IntendS->setCurrentIndex(Opts.Intent);
		QString tp1 = Opts.ImageProf;
		if (!ScCore->InputProfiles.contains(tp1))
		{
			if (doc != 0)
				tp1 = doc->CMSSettings.DefaultSolidColorRGBProfile;
			else
				tp1 = PrefsManager::instance()->appPrefs.DCMSset.DefaultSolidColorRGBProfile;
		}
		ImageP->clear();
		ProfilesL::Iterator itp2;
		ProfilesL::Iterator itp2end=ScCore->InputProfiles.end();
		for (itp2 = ScCore->InputProfiles.begin(); itp2 != itp2end; ++itp2)
		{
			ImageP->addItem(itp2.key());
			if (itp2.key() == tp1)
			{
				if (cms)
					ImageP->setCurrentIndex(ImageP->count()-1);
			}
		}
		if (cms)
			IntendI->setCurrentIndex(Opts.Intent2);
		if (cms)
		{
			GroupBox9->show();
			ProfsGroup->show();
		}
		else
		{
			GroupBox9->hide();
			ProfsGroup->hide();
		}
		useSpot->show();
		overprintMode->show();
		if (doc!=0)
		{
			UseLPI->show();
			if (UseLPI->isChecked())
				LPIgroup->show();
			else
				LPIgroup->hide();
		}	
	}
	else
	{
		useSpot->hide();
		overprintMode->hide();
		UseLPI->hide();
		LPIgroup->hide();
	}
}

void TabPDFOptions::EnableLPI2()
{
	if (doc!=0)
	{
		if (UseLPI->isChecked())
			LPIgroup->show();
		else
			LPIgroup->hide();
	}
}

void TabPDFOptions::SelLPIcol(int c)
{
	// XXX Optionen or Opts changed here
	Opts.LPISettings[SelLPIcolor].Frequency = LPIfreq->value();
	Opts.LPISettings[SelLPIcolor].Angle = LPIangle->value();
	Opts.LPISettings[SelLPIcolor].SpotFunc = LPIfunc->currentIndex();
	LPIfreq->setValue(Opts.LPISettings[LPIcolor->itemText(c)].Frequency);
	LPIangle->setValue(Opts.LPISettings[LPIcolor->itemText(c)].Angle);
	LPIfunc->setCurrentIndex(Opts.LPISettings[LPIcolor->itemText(c)].SpotFunc);
	SelLPIcolor = LPIcolor->itemText(c);
}

void TabPDFOptions::SelRange(bool e)
{
	bool setter = e ? false : true;
	PageNr->setEnabled( setter );
	pageNrButton->setEnabled( setter );
	if (setter == false)
		CheckBM->setChecked(false);
}

void TabPDFOptions::EffectOnAll()
{
	for (int pg = 0; pg < doc->Pages->count(); ++pg)
	{
		EffVal[pg].pageViewDuration = PageTime->value();
		EffVal[pg].pageEffectDuration = EffectTime->value();
		EffVal[pg].effectType = EffectType->currentIndex();
		EffVal[pg].Dm = EDirection->currentIndex();
		EffVal[pg].M = EDirection_2->currentIndex();
		EffVal[pg].Di = EDirection_2_2->currentIndex();
	}
}

void TabPDFOptions::PDFMirror()
{
	// XXX Optionen or Opts changed here
	Opts.MirrorH = MirrorH->isChecked();
	Opts.MirrorV = MirrorV->isChecked();
}

void TabPDFOptions::Rotation( int value )
{
	Opts.RotateDeg = value * 90;
}

void TabPDFOptions::DoEffects()
{
	bool setter = CheckBox10->isChecked() ? true : false;
	Pages->setEnabled(setter);
	Effects->setEnabled(setter);
	PagePrev->setEnabled(setter);
}

void TabPDFOptions::ValidDI(int nr)
{
	// Qt4 if (!EDirection_2_2->listBox()->item(nr)->isSelectable())
	QStandardItem* si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(nr));
	if ( !(si && si->isSelectable()) )
		EDirection_2_2->setCurrentIndex(0);
}

void TabPDFOptions::SetPgEff()
{
	int nr = Pages->currentRow();
	if (nr < 0)
		return;
	EffVal[PgSel].pageViewDuration = PageTime->value();
	EffVal[PgSel].pageEffectDuration = EffectTime->value();
	EffVal[PgSel].effectType = EffectType->currentIndex();
	EffVal[PgSel].Dm = EDirection->currentIndex();
	EffVal[PgSel].M = EDirection_2->currentIndex();
	EffVal[PgSel].Di = EDirection_2_2->currentIndex();
	SetEffOpts(EffVal[nr].effectType);
	PageTime->setValue(EffVal[nr].pageViewDuration);
	EffectTime->setValue(EffVal[nr].pageEffectDuration);
	EffectType->setCurrentIndex(EffVal[nr].effectType);
	EDirection->setCurrentIndex(EffVal[nr].Dm);
	EDirection_2->setCurrentIndex(EffVal[nr].M);
	EDirection_2_2->setCurrentIndex(EffVal[nr].Di);
	PgSel = nr;
}

void TabPDFOptions::SetEffOpts(int nr)
{
	// Qt4
// 	EDirection_2_2->listBox()->item(2)->setSelectable(false);
// 	EDirection_2_2->listBox()->item(3)->setSelectable(false);
// 	EDirection_2_2->listBox()->item(4)->setSelectable(false);
	QStandardItem* si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(2));
	if (si) si->setSelectable(false);
	si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(3));
	if (si) si->setSelectable(false);
	si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(4));
	if (si) si->setSelectable(false);
	switch (nr)
	{
	case 0:
	case 3:
		EDirection->setEnabled(false);
		EDirection_2->setEnabled(false);
		EDirection_2_2->setEnabled(false);
		break;
	case 1:
		EDirection->setEnabled(true);
		EDirection_2->setEnabled(false);
		EDirection_2_2->setEnabled(false);
		break;
	case 2:
		EDirection->setEnabled(false);
		EDirection_2->setEnabled(true);
		EDirection_2_2->setEnabled(false);
		break;
	case 4:
	case 6:
		EDirection->setEnabled(false);
		EDirection_2->setEnabled(false);
		EDirection_2_2->setEnabled(true);
		if (nr == 6)
		{
			// Qt4
/*			EDirection_2_2->listBox()->item(2)->setSelectable(true);
			EDirection_2_2->listBox()->item(3)->setSelectable(true);*/
			si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(2));
			if (si) si->setSelectable(true);
			si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(3));
			if (si) si->setSelectable(true);
		}
		else {
			// Qt4
			// 			EDirection_2_2->listBox()->item(4)->setSelectable(true);
			si = dynamic_cast<QStandardItem*>(EDirection_2_2->view()->children().at(4));
			if (si) si->setSelectable(true);
		}
		break;
	case 5:
		EDirection->setEnabled(true);
		EDirection_2->setEnabled(true);
		EDirection_2_2->setEnabled(false);
		break;
	}
}

void TabPDFOptions::PagePr()
{
	disconnect(Pages, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetPgEff()));
	QString tmp;
	QPixmap pm;
	int ci = Pages->currentRow();
	int pgMaxX = 0;
	int pgMaxY = 0;
	Pages->clear();
	if (PagePrev->isChecked())
	{
		for (int pg = 0; pg < doc->Pages->count(); ++pg)
		{
			pm=QPixmap::fromImage(doc->view()->PageToPixmap(pg, 70));
			pgMaxX = qMax(pgMaxX, pm.width());
			pgMaxY = qMax(pgMaxY, pm.height());
			new QListWidgetItem( pm, tr("Page")+" "+tmp.setNum(pg+1), Pages);
		}
		Pages->setIconSize(QSize(pgMaxX, pgMaxY));
	}
	else
	{
		for (int pg = 0; pg < doc->Pages->count(); ++pg)
		{
			new QListWidgetItem( tr("Page")+" "+tmp.setNum(pg+1), Pages);
		}
	}
	if (ci != -1)
	{
		PgSel = ci;
		Pages->setCurrentRow(ci);
	}
	else
	{
		PgSel = 0;
		Pages->clearSelection();
	}
	connect(Pages, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetPgEff()));
}

void TabPDFOptions::DoDownsample()
{
	if (DSColor->isChecked())
	{
		ValC->setEnabled(true);
		if (ValC->value() > Resolution->value())
			ValC->setValue(Resolution->value());
		ValC->setMaximum(Resolution->value());
		ValC->setMinimum(35);
	}
	else
		ValC->setEnabled(false);
}

void TabPDFOptions::RemoveEmbed()
{
	FontsToEmbed.removeAll(EmbedList->currentItem()->text());
	delete EmbedList->takeItem(EmbedList->currentRow());
	EmbedList->clearSelection();
	if (EmbedList->count() == 0)
	{
		FromEmbed->setEnabled(false);
		ToOutline->setEnabled(false);
	}
	else
	{
		if (!EmbedList->item(EmbedList->currentRow())->flags() & Qt::ItemIsSelectable)
		{
			FromEmbed->setEnabled(false);
			ToOutline->setEnabled(false);
		}
	}
}

void TabPDFOptions::PutToEmbed()
{
	if (EmbedList->count() != 0)
	{
		if (!AllFonts[AvailFlist->currentItem()->text()].subset())
		{
			if (EmbedList->findItems(AvailFlist->currentItem()->text(), Qt::MatchExactly).count() == 0)
			{
				FontsToEmbed.append(AvailFlist->currentItem()->text());
				EmbedList->addItem(AvailFlist->currentItem()->text());
				if (AnnotationFonts.contains(AvailFlist->currentItem()->text()))
					EmbedList->item(EmbedList->count()-1)->setFlags(Qt::ItemIsEnabled);
			}
		}
		else
		{
			if (OutlineList->count() != 0)
			{
				if (OutlineList->findItems(AvailFlist->currentItem()->text(), Qt::MatchExactly).count() == 0)
				{
					FontsToOutline.append(AvailFlist->currentItem()->text());
					OutlineList->addItem(AvailFlist->currentItem()->text());
				}
			}
			else
			{
				FontsToOutline.append(AvailFlist->currentItem()->text());
				OutlineList->addItem(AvailFlist->currentItem()->text());
			}
		}
	}
	else
	{
		if ((AllFonts[AvailFlist->currentItem()->text()].type() != ScFace::OTF) && (!AllFonts[AvailFlist->currentItem()->text()].subset()))
		{
			FontsToEmbed.append(AvailFlist->currentItem()->text());
			EmbedList->addItem(AvailFlist->currentItem()->text());
			if (AnnotationFonts.contains(AvailFlist->currentItem()->text()))
				EmbedList->item(EmbedList->count()-1)->setFlags(Qt::ItemIsEnabled);
		}
		else
		{
			if (OutlineList->count() != 0)
			{
				if (OutlineList->findItems(AvailFlist->currentItem()->text(), Qt::MatchExactly).count() == 0)
				{
					FontsToOutline.append(AvailFlist->currentItem()->text());
					OutlineList->addItem(AvailFlist->currentItem()->text());
				}
			}
			else
			{
				FontsToOutline.append(AvailFlist->currentItem()->text());
				OutlineList->addItem(AvailFlist->currentItem()->text());
			}
		}
	}
}

void TabPDFOptions::RemoveOutline()
{
	FontsToOutline.removeAll(OutlineList->currentItem()->text());
	if ((AllFonts[OutlineList->currentItem()->text()].type() != ScFace::OTF) && (!AllFonts[OutlineList->currentItem()->text()].subset()))
	{
		FontsToEmbed.append(OutlineList->currentItem()->text());
		EmbedList->addItem(OutlineList->currentItem()->text());
	}
	delete OutlineList->takeItem(OutlineList->currentRow());
	OutlineList->clearSelection();
	if (OutlineList->count() == 0)
		FromOutline->setEnabled(false);
}

void TabPDFOptions::PutToOutline()
{
	if (OutlineList->count() != 0)
	{
		if (OutlineList->findItems(EmbedList->currentItem()->text(), Qt::MatchExactly).count() == 0)
		{
			FontsToOutline.append(EmbedList->currentItem()->text());
			OutlineList->addItem(EmbedList->currentItem()->text());
		}
	}
	else
	{
		FontsToOutline.append(EmbedList->currentItem()->text());
		OutlineList->addItem(EmbedList->currentItem()->text());
	}
	FontsToEmbed.removeAll(EmbedList->currentItem()->text());
	delete EmbedList->takeItem(EmbedList->currentRow());
	EmbedList->clearSelection();
	if (EmbedList->count() == 0)
	{
		FromEmbed->setEnabled(false);
		ToOutline->setEnabled(false);
	}
	else
	{
		if (!(EmbedList->currentItem()->flags() & Qt::ItemIsSelectable))
		{
			FromEmbed->setEnabled(false);
			ToOutline->setEnabled(false);
		}
	}
}

void TabPDFOptions::SelAFont(QListWidgetItem *c)
{
	if (c != NULL)
	{
		FromEmbed->setEnabled(false);
		if (c->flags() & Qt::ItemIsSelectable)
			ToEmbed->setEnabled(true);
		ToOutline->setEnabled(false);
		FromOutline->setEnabled(false);
		EmbedList->clearSelection();
		OutlineList->clearSelection();
	}
}

void TabPDFOptions::SelEFont(QListWidgetItem *c)
{
	if (c != NULL)
	{
		if ((PDFVersionCombo->currentIndex() != 3) && (c->flags() & Qt::ItemIsSelectable))
			FromEmbed->setEnabled(true);
		else
			FromEmbed->setEnabled(false);
		ToEmbed->setEnabled(false);
		if (c->flags() & Qt::ItemIsSelectable)
			ToOutline->setEnabled(true);
		FromOutline->setEnabled(false);
		AvailFlist->clearSelection();
		OutlineList->clearSelection();
	}
}

void TabPDFOptions::SelSFont(QListWidgetItem *c)
{
	if (c != NULL)
	{
		if (PDFVersionCombo->currentIndex() == 3)
		{
			if ((AllFonts[c->text()].type() == ScFace::OTF) || (AllFonts[c->text()].subset()))
				FromOutline->setEnabled(false);
			else
				FromOutline->setEnabled(true);
		}
		else
			FromOutline->setEnabled(true);
		ToOutline->setEnabled(false);
		ToEmbed->setEnabled(false);
		FromEmbed->setEnabled(false);
		EmbedList->clearSelection();
		AvailFlist->clearSelection();
	}
}

void TabPDFOptions::EmbedAll()
{
	EmbedList->clear();
	FontsToEmbed.clear();
	OutlineList->clear();
	FontsToOutline.clear();
	FromEmbed->setEnabled(false);
	ToEmbed->setEnabled(false);
	ToOutline->setEnabled(false);
	FromOutline->setEnabled(false);
	for (int a=0; a < AvailFlist->count(); ++a)
	{
		if (AvailFlist->item(a)->flags() & Qt::ItemIsSelectable)
		{
			if (!AllFonts[AvailFlist->item(a)->text()].subset())
			{
				FontsToEmbed.append(AvailFlist->item(a)->text());
				EmbedList->addItem(AvailFlist->item(a)->text());
				if (AnnotationFonts.contains(AvailFlist->item(a)->text()))
					EmbedList->item(EmbedList->count()-1)->setFlags(Qt::ItemIsEnabled);
			}
			else
			{
				if (AnnotationFonts.contains(AvailFlist->item(a)->text()))
				{
					FontsToEmbed.append(AvailFlist->item(a)->text());
					EmbedList->addItem(AvailFlist->item(a)->text());
					EmbedList->item(EmbedList->count()-1)->setFlags(Qt::ItemIsEnabled);
				}
				else
				{
					FontsToOutline.append(AvailFlist->item(a)->text());
					OutlineList->addItem(AvailFlist->item(a)->text());
				}
			}
		}
	}
}

void TabPDFOptions::OutlineAll()
{
	EmbedList->clear();
	FontsToEmbed.clear();
	OutlineList->clear();
	FontsToOutline.clear();
	FromEmbed->setEnabled(false);
	ToEmbed->setEnabled(false);
	ToOutline->setEnabled(false);
	FromOutline->setEnabled(false);
	for (int a=0; a < AvailFlist->count(); ++a)
	{
		if (AvailFlist->item(a)->flags() & Qt::ItemIsSelectable)
		{
			if (AnnotationFonts.contains(AvailFlist->item(a)->text()))
			{
				FontsToEmbed.append(AvailFlist->item(a)->text());
				EmbedList->addItem(AvailFlist->item(a)->text());
				EmbedList->item(EmbedList->count()-1)->setFlags(Qt::ItemIsEnabled);
			}
			else
			{
				FontsToOutline.append(AvailFlist->item(a)->text());
				OutlineList->addItem(AvailFlist->item(a)->text());
			}
		}
	}
}

void TabPDFOptions::unitChange(QString unit, int docUnitIndex, double invUnitConversion)
{
	double oldMin = -1, oldMax = -1, val = -1;
	int decimalsOld = -1;
	int decimals = unitGetPrecisionFromIndex(docUnitIndex);

	BleedBottom->setSuffix(unit);
	BleedTop->setSuffix(unit);
	BleedRight->setSuffix(unit);
	BleedLeft->setSuffix(unit);
	markOffset->setSuffix(unit);

	BleedBottom->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	BleedBottom->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	BleedTop->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	BleedTop->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	BleedRight->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	BleedRight->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	BleedLeft->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	BleedLeft->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
	markOffset->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	markOffset->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimals, val * invUnitConversion);
}

void TabPDFOptions::createPageNumberRange( )
{
	if (doc!=0)
	{
		CreateRange cr(PageNr->text(), doc->DocPages.count(), this);
		if (cr.exec())
		{
			CreateRangeData crData;
			cr.getCreateRangeData(crData);
			PageNr->setText(crData.pageRange);
			return;
		}
	}
	PageNr->setText(QString::null);
}
