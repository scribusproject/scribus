/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tabpdfoptions.h"
#include "tabpdfoptions.moc"

#include <qpainter.h>
#include "units.h"
#include "pdfoptions.h"
#include "prefsmanager.h"
#include "scconfig.h"

extern QPixmap loadIcon(QString nam);
#ifdef HAVE_CMS
extern bool CMSuse;
#endif
extern bool CMSavail;
#include "scribus.h"




TabPDFOptions::TabPDFOptions(   QWidget* parent, PDFOptions *Optionen, SCFonts &AllFonts,
                                ProfilesL *PDFXProfiles, QMap<QString,int> DocFonts,
                                QValueList<PDFPresentationData> Eff, int unitIndex,
                                double PageH, double PageB, ScribusView *vie )
	: QTabWidget( parent, "pdf" ),
	// Initialize all those darn pointer members so we catch unitialized
	// accesses. I (CR) use the following command to generate these based on
	// the header excerpt:
	//    !cut -d \* -f 2 | sed -r 's/ ?(.*);/\t\1\(0\),/g'
	// Public GUI member pointers:
	actionCombo(0),
	AddSec(0),
	AllPages(0),
	Article(0),
	BleedBottom(0),
	BleedLeft(0),
	BleedRight(0),
	BleedTop(0),
	CheckBM(0),
	CheckBox1(0),
	CheckBox10(0),
	CMethod(0),
	ComboBind(0),
	Compression(0),
	continuousPages(0),
	CopySec(0),
	CQuality(0),
	doublePageLeft(0),
	doublePageRight(0),
	DSColor(0),
	EDirection(0),
	EDirection_2(0),
	EDirection_2_2(0),
	EffectTime(0),
	EffectType(0),
	EmbedProfs(0),
	EmbedProfs2(0),
	Encry(0),
	fitWindow(0),
	hideMenuBar(0),
	hideToolBar(0),
	ImageP(0),
	InfoString(0),
	IntendI(0),
	IntendS(0),
	LPIangle(0),
	LPIfreq(0),
	LPIfunc(0),
	MirrorH(0),
	MirrorV(0),
	ModifySec(0),
	NoEmbedded(0),
	OutCombo(0),
	PageNr(0),
	Pages(0),
	PageTime(0),
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
	useThumbnails(0),
	ValC(0),
	// End GUI member pointers
	view(vie),
	AllFontsP(AllFonts),
	EffVal(Eff),
	PgSel(0),
	Opts(Optionen),
	pageH(PageH),
	pageB(PageB),
	cms(false),
	unitRatio(unitGetRatioFromIndex(unitIndex)),
	// Begin protected member gui pointers
	actionLabel(0),
	AvailFlist(0),
	BleedGroup(0),
	BleedGroupLayout(0),
	BleedIcon(0),
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
	FromSubset(0),
	GroupBox1(0),
	GroupBox1Layout(0),
	GroupBox2Layout(0),
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
	Layout11a(0),
	Layout13(0),
	Layout2(0),
	Layout3(0),
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
	SubsetFonts(0),
	SubsetList(0),
	tabColor(0),
	tabColorLayout(0),
	tabFonts(0),
	tabGeneral(0),
	tabLayout(0),
	tabLayout_3(0),
	tabLayout_5(0),
	tabOptionsGridLayout(0),
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
	ToEmbed(0),
	ToSubset(0),
	useViewDefault(0),
	X3Group(0),
	X3GroupLayout(0),
	// end protected member gui pointers
	unit(unitGetSuffixFromIndex(unitIndex)),
	precision(unitGetPrecisionFromIndex(unitIndex))
{

	tabGeneral = new QWidget( this, "tabGeneral" );
	tabLayout = new QVBoxLayout( tabGeneral );
	tabLayout->setSpacing( 5 );
	tabLayout->setMargin( 0 );
	Layout13 = new QHBoxLayout( 0, 0, 5, "Layout13");
	RangeGroup = new QButtonGroup( tabGeneral, "RangeGroup" );
	RangeGroup->setTitle( tr( "Export Range" ) );
	RangeGroup->setColumnLayout(0, Qt::Vertical );
	RangeGroup->layout()->setSpacing( 5 );
	RangeGroup->layout()->setMargin( 10 );
	RangeGroupLayout = new QVBoxLayout( RangeGroup->layout() );
	RangeGroupLayout->setAlignment( Qt::AlignTop );
	AllPages = new QRadioButton( tr( "&All Pages" ), RangeGroup, "AllPages" );
	AllPages->setChecked( true );
	RangeGroupLayout->addWidget( AllPages );
	Layout11 = new QGridLayout( 0, 1, 1, 0, 5, "Layout11");
	OnlySome = new QRadioButton( tr( "C&hoose Pages" ), RangeGroup, "OnlySome" );
	Layout11->addWidget( OnlySome, 0, 0 );
	PageNr = new QLineEdit( RangeGroup, "PageNr" );
	PageNr->setEnabled(false);
	Layout11->addWidget( PageNr, 1, 0 );
	RangeGroupLayout->addLayout( Layout11 );
	TextLabel3 = new QLabel( tr( "&Rotation:" ), RangeGroup, "TextLabel3" );
	RangeGroupLayout->addWidget( TextLabel3 );
	RotateDeg = new QComboBox( true, RangeGroup, "RotateDeg" );
	RotateDeg->insertItem(QString::fromUtf8("0 °"));
	RotateDeg->insertItem(QString::fromUtf8("90 °"));
	RotateDeg->insertItem(QString::fromUtf8("180 °"));
	RotateDeg->insertItem(QString::fromUtf8("270 °"));
	RotateDeg->setEditable(false);
	TextLabel3->setBuddy(RotateDeg);
	RotateDeg->setCurrentItem(Optionen->RotateDeg / 90);
	RangeGroupLayout->addWidget( RotateDeg );
	Layout11a = new QGridLayout( 0, 1, 1, 0, 5, "Layout11a");
	MirrorH = new QToolButton( RangeGroup, "MirrorH" );
	MirrorH->setPixmap(loadIcon("FlipH.xpm"));
	MirrorH->setToggleButton( true );
	MirrorH->setOn(Optionen->MirrorH);
	Layout11a->addWidget( MirrorH, 0, 0 );
	MirrorV = new QToolButton( RangeGroup, "MirrorH" );
	MirrorV->setPixmap(loadIcon("FlipV.xpm"));
	MirrorV->setToggleButton( true );
	MirrorV->setOn(Optionen->MirrorV);
	Layout11a->addWidget( MirrorV, 0, 1 );
	RangeGroupLayout->addLayout( Layout11a );
	Layout13->addWidget( RangeGroup );

	GroupBox1 = new QGroupBox( tr( "File Options" ), tabGeneral, "GroupBox1" );
	GroupBox1->setColumnLayout(0, Qt::Vertical );
	GroupBox1->layout()->setSpacing( 0 );
	GroupBox1->layout()->setMargin( 0 );
	GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
	GroupBox1Layout->setAlignment( Qt::AlignTop );
	GroupBox1Layout->setSpacing( 5 );
	GroupBox1Layout->setMargin( 10 );
	TextLabel1 = new QLabel( tr( "Compatibilit&y:" ), GroupBox1, "TextLabel1" );
	TextLabel1->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	GroupBox1Layout->addWidget( TextLabel1, 0, 0 );
	PDFVersionCombo = new QComboBox( true, GroupBox1, "PDFVersionCombo" );
	PDFVersionCombo->setEditable(false);
	TextLabel1->setBuddy(PDFVersionCombo);
	PDFVersionCombo->insertItem("PDF 1.3");
	PDFVersionCombo->insertItem("PDF 1.4");
	PDFVersionCombo->insertItem("PDF 1.5");
#ifdef HAVE_CMS
	if ((CMSuse) && (CMSavail) && (!PDFXProfiles->isEmpty()))
		PDFVersionCombo->insertItem("PDF/X-3");
	if ((CMSuse) && (CMSavail))
	{
		if (Optionen->Version == PDFOptions::PDFVersion_X3)
			PDFVersionCombo->setCurrentItem(3);
	}
	else
		PDFVersionCombo->setCurrentItem(0);
	cms = CMSuse && CMSavail;
#else
	cms = false;
#endif
	if (Optionen->Version == PDFOptions::PDFVersion_13)
		PDFVersionCombo->setCurrentItem(0);
	if (Optionen->Version == PDFOptions::PDFVersion_14)
		PDFVersionCombo->setCurrentItem(1);
	if (Optionen->Version == PDFOptions::PDFVersion_15)
		PDFVersionCombo->setCurrentItem(2);
	GroupBox1Layout->addMultiCellWidget( PDFVersionCombo, 0, 0, 1, 2 );
	TextLabel1x = new QLabel( tr( "&Binding:" ), GroupBox1, "TextLabel1" );
	TextLabel1x->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	GroupBox1Layout->addWidget( TextLabel1x, 1, 0 );
	ComboBind = new QComboBox( true, GroupBox1, "ComboBind" );
	ComboBind->insertItem( tr("Left Margin"));
	ComboBind->insertItem( tr("Right Margin"));
	ComboBind->setEditable(false);
	ComboBind->setCurrentItem(Optionen->Binding);
	TextLabel1x->setBuddy(ComboBind);
	GroupBox1Layout->addMultiCellWidget( ComboBind, 1, 1, 1, 2 );
	CheckBox1 = new QCheckBox( tr( "Generate &Thumbnails" ), GroupBox1, "CheckBox1" );
	CheckBox1->setChecked(Optionen->Thumbnails);
	GroupBox1Layout->addMultiCellWidget( CheckBox1, 2, 2, 0, 2 );
	Article = new QCheckBox( tr( "Save &Linked Text Frames as PDF Articles" ), GroupBox1, "CheckBox1" );
	Article->setChecked(Optionen->Articles);
	GroupBox1Layout->addMultiCellWidget( Article, 3, 3, 0, 2 );
	CheckBM = new QCheckBox( tr( "&Include Bookmarks" ), GroupBox1, "E" );
	CheckBM->setChecked(Optionen->Bookmarks);
	GroupBox1Layout->addMultiCellWidget( CheckBM, 4, 4, 0, 2 );
	useLayers = new QCheckBox( tr( "Include La&yers" ), GroupBox1, "LI" );
	useLayers->setChecked(Optionen->useLayers);
	GroupBox1Layout->addMultiCellWidget( useLayers, 5, 5, 0, 2 );
	if (Optionen->Version == 15)
		useLayers->setEnabled(true);
	else
		useLayers->setEnabled(false);
	Resolution = new QSpinBox( GroupBox1, "Resolution" );
	Resolution->setMaxValue( 4000 );
	Resolution->setMinValue( 35 );
	Resolution->setValue(Optionen->Resolution);
	Resolution->setSuffix( tr( " dpi" ) );
	TextLabel2 = new QLabel( Resolution, tr( "&Resolution for EPS Graphics:" ), GroupBox1, "TextLabel2" );
	TextLabel2->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	GroupBox1Layout->addWidget( TextLabel2, 6, 0 );
	GroupBox1Layout->addWidget( Resolution, 6, 1, AlignLeft );
	Layout13->addWidget( GroupBox1 );
	tabLayout->addLayout( Layout13 );
	Compression = new QCheckBox( tr( "Com&press Text and Vector Graphics" ), tabGeneral, "Compression" );
	Compression->setChecked( Optionen->Compress );
	tabLayout->addWidget( Compression );
	CBox = new QGroupBox( tr( "Image Settings" ), tabGeneral, "CBox" );
	CBox->setColumnLayout(0, Qt::Vertical );
	CBox->layout()->setSpacing( 5 );
	CBox->layout()->setMargin( 11 );
	CBoxLayout = new QGridLayout( CBox->layout() );
	CBoxLayout->setAlignment( Qt::AlignTop );
	CMethod = new QComboBox( true, CBox, "CMethod" );
	CMethod->clear();
	CMethod->insertItem( tr( "Automatic" ) );
	CMethod->insertItem( tr( "JPEG" ) );
	CMethod->insertItem( tr( "Zip" ) );
	CMethod->insertItem( tr( "None" ) );
	CMethod->setEditable(false);
	CMethod->setCurrentItem(Optionen->CompressMethod);
	TextCom1 = new QLabel( CMethod, tr( "Compression Metho&d:" ), CBox, "TextCom1" );
	CBoxLayout->addWidget( TextCom1, 0, 0 );
	CBoxLayout->addWidget( CMethod, 0, 1 );
	CQuality = new QComboBox( true, CBox, "CQuality" );
	CQuality->clear();
	CQuality->insertItem( tr( "Maximum" ) );
	CQuality->insertItem( tr( "High" ) );
	CQuality->insertItem( tr( "Medium" ) );
	CQuality->insertItem( tr( "Low" ) );
	CQuality->insertItem( tr( "Minimum" ) );
	CQuality->setEditable(false);
	CQuality->setCurrentItem(Optionen->Quality);
	TextCom2 = new QLabel( CQuality, tr( "Compression &Quality:" ), CBox, "TextCom2" );
	CBoxLayout->addWidget( TextCom2, 1, 0 );
	CBoxLayout->addWidget( CQuality, 1, 1 );
	DSColor = new QCheckBox( tr( "Resa&mple Images to:" ), CBox, "DSColor" );
	DSColor->setChecked(Optionen->RecalcPic);
	CBoxLayout->addWidget( DSColor, 2, 0 );
	ValC = new QSpinBox( CBox, "ValC" );
	ValC->setSuffix( tr( " dpi" ) );
	ValC->setMaxValue( 4000 );
	ValC->setMinValue( 35 );
	ValC->setValue(Optionen->PicRes);
	ValC->setEnabled(DSColor->isChecked() ? true : false);
	CBoxLayout->addWidget( ValC, 2, 1, AlignLeft );
	tabLayout->addWidget( CBox );
	insertTab( tabGeneral, tr( "&General" ) );
	if (vie != 0)
	{
		tabFonts = new QWidget( this, "tabFonts" );
		tabLayout_3 = new QVBoxLayout( tabFonts );
		tabLayout_3->setSpacing( 5 );
		tabLayout_3->setMargin( 11 );
		EmbedFonts = new QCheckBox( tr( "&Embed all Fonts" ), tabFonts, "EmbedFonts" );
		tabLayout_3->addWidget( EmbedFonts );
		SubsetFonts = new QCheckBox( tr( "&Subset all Fonts" ), tabFonts, "SubsetFonts" );
		tabLayout_3->addWidget( SubsetFonts );
		GroupFont = new QGroupBox( tr( "Embedding" ), tabFonts, "GroupFont" );
		GroupFont->setColumnLayout(0, Qt::Vertical );
		GroupFont->layout()->setSpacing( 0 );
		GroupFont->layout()->setMargin( 0 );
		GroupFontLayout = new QHBoxLayout( GroupFont->layout() );
		GroupFontLayout->setAlignment( Qt::AlignTop );
		GroupFontLayout->setSpacing( 5 );
		GroupFontLayout->setMargin( 10 );
		Layout4_2 = new QVBoxLayout;
		Layout4_2->setSpacing( 5 );
		Layout4_2->setMargin( 0 );
		TextFont1 = new QLabel( tr( "Available Fonts:" ), GroupFont, "TextFont1" );
		Layout4_2->addWidget( TextFont1 );
		AvailFlist = new QListBox( GroupFont, "AvailFlist" );
		QMap<QString,int>::Iterator it;
		for (it = DocFonts.begin(); it != DocFonts.end(); ++it)
		{
			if (AllFonts[it.key()]->typeCode == Foi::TYPE1)
				AvailFlist->insertItem(loadIcon("font_type1_16.png"), it.key());
			else if (AllFonts[it.key()]->typeCode == Foi::TTF)
				AvailFlist->insertItem(loadIcon("font_truetype16.png"), it.key());
			else if (AllFonts[it.key()]->typeCode == Foi::OTF)
				AvailFlist->insertItem(loadIcon("font_otf16.png"), it.key());
		}
		AvailFlist->setMinimumSize(QSize(150, 140));
		Layout4_2->addWidget( AvailFlist );
		GroupFontLayout->addLayout( Layout4_2 );
		Layout5_2 = new QVBoxLayout;
		Layout5_2->setSpacing( 5 );
		Layout5_2->setMargin( 0 );
		QSpacerItem* spacerS3a = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Minimum );
		Layout5_2->addItem( spacerS3a );
		ToEmbed = new QPushButton( tr( "&>>" ), GroupFont, "ToEmbed" );
		ToEmbed->setEnabled(false);
		Layout5_2->addWidget( ToEmbed );
		FromEmbed = new QPushButton( tr( "&<<" ), GroupFont, "FromEmbed" );
		FromEmbed->setEnabled(false);
		Layout5_2->addWidget( FromEmbed );
		QSpacerItem* spacerS3 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
		Layout5_2->addItem( spacerS3 );
		GroupFontLayout->addLayout( Layout5_2 );
		Layout6 = new QVBoxLayout;
		Layout6->setSpacing( 5 );
		Layout6->setMargin( 0 );
		TextFont1_2 = new QLabel( tr( "Fonts to embed:" ), GroupFont, "TextFont1_2" );
		Layout6->addWidget( TextFont1_2 );
		EmbedList = new QListBox( GroupFont, "EmbedList" );
		EmbedList->setMinimumSize(QSize(150, 40));
		Layout6->addWidget( EmbedList );

		Layout5_2a = new QHBoxLayout;
		Layout5_2a->setSpacing( 5 );
		Layout5_2a->setMargin( 0 );
		QSpacerItem* spacerS1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout5_2a->addItem( spacerS1 );
		ToSubset = new QPushButton( "", GroupFont, "ToSubset" );
		ToSubset->setPixmap(loadIcon("down.png"));
		ToSubset->setEnabled(false);
		Layout5_2a->addWidget( ToSubset );
		FromSubset = new QPushButton( "", GroupFont, "FromSubset" );
		FromSubset->setPixmap(loadIcon("up.png"));
		FromSubset->setEnabled(false);
		Layout5_2a->addWidget( FromSubset );
		QSpacerItem* spacerS2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout5_2a->addItem( spacerS2 );
		Layout6->addLayout( Layout5_2a );
		TextFont1_2a = new QLabel( tr( "Fonts to subset:" ), GroupFont, "TextFont1_2a" );
		Layout6->addWidget( TextFont1_2a );
		SubsetList = new QListBox( GroupFont, "SubsetList" );
		SubsetList->setMinimumSize(QSize(150, 40));
		Layout6->addWidget( SubsetList );
		if ((Optionen->EmbedList.count() == 0) && (Optionen->SubsetList.count() == 0))
		{
			EmbedFonts->setChecked(true);
			EmbedAll();
		}
		else
		{
			for (uint fe = 0; fe < Optionen->EmbedList.count(); ++fe)
			{
				EmbedList->insertItem(Optionen->EmbedList[fe]);
				FontsToEmbed.append(Optionen->EmbedList[fe]);
			}
		}
		if (Optionen->SubsetList.count() != 0)
		{
			for (uint fe = 0; fe < Optionen->SubsetList.count(); ++fe)
			{
				SubsetList->insertItem(Optionen->SubsetList[fe]);
				FontsToSubset.append(Optionen->SubsetList[fe]);
			}
		}
		GroupFontLayout->addLayout( Layout6 );
		tabLayout_3->addWidget( GroupFont );
		insertTab( tabFonts, tr( "&Fonts" ) );
		tabPresentation = new QWidget( this, "tabPresentation" );
		tabLayout_5 = new QGridLayout( tabPresentation );
		tabLayout_5->setSpacing( 5 );
		tabLayout_5->setMargin( 11 );
		CheckBox10 = new QCheckBox( tr( "Enable &Presentation Effects" ), tabPresentation, "CheckBox10" );
		CheckBox10->setChecked(Optionen->PresentMode);
		tabLayout_5->addMultiCellWidget( CheckBox10, 0, 0, 0, 1 );
		Pages = new QListBox( tabPresentation, "Pages" );
		Pages->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, Pages->sizePolicy().hasHeightForWidth() ) );
		QString tmp;
		struct PDFPresentationData ef;
		if (EffVal.count() != 0)
		{
			for (uint pg2 = 0; pg2 < view->Doc->Pages->count(); ++pg2)
			{
				Pages->insertItem( tr("Page")+" "+tmp.setNum(pg2+1));
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
			for (uint pg = 0; pg < view->Doc->Pages->count(); ++pg)
			{
				Pages->insertItem( tr("Page")+" "+tmp.setNum(pg+1));
				ef.pageEffectDuration = 1;
				ef.pageViewDuration = 1;
				ef.effectType = 0;
				ef.Dm = 0;
				ef.M = 0;
				ef.Di = 0;
				EffVal.append(ef);
			}
		}
		tabLayout_5->addWidget( Pages, 1, 0 );
		PagePrev = new QCheckBox( tr( "Show Page Pre&views" ), tabPresentation, "CheckBox10" );
		PagePrev->setChecked(false);
		tabLayout_5->addWidget( PagePrev, 2, 0 );
		Effects = new QGroupBox( tr( "Effects" ), tabPresentation, "Effects" );
		Effects->setColumnLayout(0, Qt::Vertical );
		Effects->layout()->setSpacing( 0 );
		Effects->layout()->setMargin( 0 );
		EffectsLayout = new QGridLayout( Effects->layout() );
		EffectsLayout->setAlignment( Qt::AlignTop );
		EffectsLayout->setSpacing( 5 );
		EffectsLayout->setMargin( 11 );
		TextLabel1e = new QLabel( tr( "&Display Duration:" ), Effects, "TextLabel1_2" );
		EffectsLayout->addWidget( TextLabel1e, 0, 0 );
		TextLabel2e = new QLabel( tr( "Effec&t Duration:" ), Effects, "TextLabel2_2" );
		EffectsLayout->addWidget( TextLabel2e, 1, 0 );
		TextLabel3e = new QLabel( tr( "Effect T&ype:" ), Effects, "TextLabel3_2" );
		EffectsLayout->addWidget( TextLabel3e, 2, 0 );
		TextLabel4e = new QLabel( tr( "&Moving Lines:" ), Effects, "TextLabel4_2" );
		EffectsLayout->addWidget( TextLabel4e, 3, 0 );
		TextLabel5e = new QLabel( tr( "F&rom the:" ), Effects, "TextLabel6" );
		EffectsLayout->addWidget( TextLabel5e, 4, 0 );
		TextLabel6e = new QLabel( tr( "D&irection:" ), Effects, "TextLabel5" );
		EffectsLayout->addWidget( TextLabel6e, 5, 0 );
		PageTime = new QSpinBox( Effects, "PageTime" );
		PageTime->setSuffix( tr( " sec" ) );
		PageTime->setMaxValue( 3600 );
		PageTime->setMinValue( 1 );
		PageTime->setValue(EffVal[0].pageViewDuration);
		TextLabel1e->setBuddy(PageTime);
		EffectsLayout->addWidget( PageTime, 0, 1 );
		EffectTime = new QSpinBox( Effects, "EffectTime" );
		EffectTime->setSuffix( tr( " sec" ) );
		EffectTime->setMaxValue( 3600 );
		EffectTime->setMinValue( 1 );
		EffectTime->setValue(EffVal[0].pageEffectDuration);
		TextLabel2e->setBuddy(EffectTime);
		EffectsLayout->addWidget( EffectTime, 1, 1 );
		EffectType = new QComboBox( true, Effects, "EffectType" );
		QString tmpc[] = { tr("No Effect"), tr("Blinds"), tr("Box"), tr("Dissolve"), tr("Glitter"), tr("Split"), tr("Wipe")};
		size_t ar = sizeof(tmpc) / sizeof(*tmpc);
		for (uint a = 0; a < ar; ++a)
			EffectType->insertItem(tmpc[a]);
		EffectType->setEditable(false);
		TextLabel3e->setBuddy(EffectType);
		EffectsLayout->addWidget( EffectType, 2, 1 );
		EDirection = new QComboBox( true, Effects, "EDirection" );
		EDirection->insertItem( tr( "Horizontal" ) );
		EDirection->insertItem( tr( "Vertical" ) );
		EDirection->setEditable(false);
		TextLabel4e->setBuddy(EDirection);
		EffectsLayout->addWidget( EDirection, 3, 1 );
		EDirection_2 = new QComboBox( true, Effects, "EDirection_2" );
		EDirection_2->insertItem( tr( "Inside" ) );
		EDirection_2->insertItem( tr( "Outside" ) );
		EDirection_2->setEditable(false);
		TextLabel5e->setBuddy(EDirection_2);
		EffectsLayout->addWidget( EDirection_2, 4, 1 );
		EDirection_2_2 = new QComboBox( true, Effects, "EDirection_2_2" );
		QString tmp_ed[] = { tr("Left to Right"), tr("Top to Bottom"), tr("Bottom to Top"), tr("Right to Left"),
		                     tr("Top-left to Bottom-Right")};
		size_t ar_ed = sizeof(tmp_ed) / sizeof(*tmp_ed);
		for (uint a = 0; a < ar_ed; ++a)
			EDirection_2_2->insertItem(tmp_ed[a]);
		EDirection_2_2->setEditable(false);
		TextLabel6e->setBuddy(EDirection_2_2);
		EffectsLayout->addWidget( EDirection_2_2, 5, 1 );
		EonAllPg = new QPushButton( tr( "&Apply Effect on all Pages" ), Effects, "Eon" );
		EffectsLayout->addMultiCellWidget( EonAllPg, 6, 6, 0, 1 );
		tabLayout_5->addMultiCellWidget( Effects, 1, 2, 1, 1 );
		insertTab( tabPresentation, tr( "E&xtras" ) );
	}

	if (view->Doc->currentPageLayout == doublePage)
	{
		if (view->Doc->pageSets[view->Doc->currentPageLayout].FirstPage == 0)
			Optionen->PageLayout = PDFOptions::TwoColumnLeft;
		else
			Optionen->PageLayout = PDFOptions::TwoColumnRight;
	}
	else
		Optionen->PageLayout = PDFOptions::SinglePage;
	if (Optionen->PresentMode)
		Optionen->displayFullscreen = true;
	else
	{
		if ((Optionen->Version == 15) && (Optionen->useLayers))
			Optionen->displayLayers = true;
	}
	tabSpecial = new QWidget( this, "tabSpecial" );
	tabSpecialLayout = new QVBoxLayout( tabSpecial, 11, 6, "tabSpecialLayout");
	groupDisplay = new QGroupBox( tabSpecial, "groupDisplay" );
	groupDisplay->setTitle( tr( "Display Settings" ) );
	groupDisplay->setColumnLayout(0, Qt::Vertical );
	groupDisplay->layout()->setSpacing( 5 );
	groupDisplay->layout()->setMargin( 10 );
	groupDisplayLayout = new QVBoxLayout( groupDisplay->layout() );
	groupDisplayLayout->setAlignment( Qt::AlignTop );
	LayoutSpecial = new QHBoxLayout( 0, 0, 5, "LayoutSpecial");
	pageLayout = new QButtonGroup( groupDisplay, "pageLayout" );
	pageLayout->setTitle( tr( "Page Layout" ) );
	pageLayout->setColumnLayout(0, Qt::Vertical );
	pageLayout->layout()->setSpacing( 5 );
	pageLayout->layout()->setMargin( 10 );
	pageLayoutLayout = new QVBoxLayout( pageLayout->layout() );
	pageLayoutLayout->setAlignment( Qt::AlignTop );
	singlePage = new QRadioButton( pageLayout, "singlePage" );
	singlePage->setText( tr( "Single Page" ) );
	pageLayoutLayout->addWidget( singlePage );
	continuousPages = new QRadioButton( pageLayout, "continuousPages" );
	continuousPages->setText( tr( "Continuous" ) );
	pageLayoutLayout->addWidget( continuousPages );
	doublePageLeft = new QRadioButton( pageLayout, "doublePageLeft" );
	doublePageLeft->setText( tr( "Double Page Left" ) );
	pageLayoutLayout->addWidget( doublePageLeft );
	doublePageRight = new QRadioButton( pageLayout, "doublePageRight" );
	doublePageRight->setText( tr( "Double Page Right" ) );
	pageLayoutLayout->addWidget( doublePageRight );
	LayoutSpecial->addWidget( pageLayout );
	groupNavigation = new QButtonGroup( groupDisplay, "groupNavigation" );
	groupNavigation->setTitle( tr( "Visual Appearance" ) );
	groupNavigation->setColumnLayout(0, Qt::Vertical );
	groupNavigation->layout()->setSpacing( 5 );
	groupNavigation->layout()->setMargin( 10 );
	groupNavigationLayout = new QVBoxLayout( groupNavigation->layout() );
	groupNavigationLayout->setAlignment( Qt::AlignTop );
	useViewDefault = new QRadioButton( groupNavigation, "useViewDefault" );
	bool df = true;
	if ((Optionen->displayBookmarks) || (Optionen->displayFullscreen) || (Optionen->displayLayers) || (Optionen->displayThumbs))
		df = false;
	useViewDefault->setChecked(df);
	useViewDefault->setText( tr( "Use Viewers Defaults" ) );
	groupNavigationLayout->addWidget( useViewDefault );
	useFullScreen = new QRadioButton( groupNavigation, "useFullScreen" );
	useFullScreen->setChecked(Optionen->displayFullscreen);
	useFullScreen->setText( tr( "Use Full Screen Mode" ) );
	groupNavigationLayout->addWidget( useFullScreen );
	useBookmarks = new QRadioButton( groupNavigation, "useBookmarks" );
	useBookmarks->setText( tr( "Display Bookmarks Tab" ) );
	useBookmarks->setChecked(Optionen->displayBookmarks);
	groupNavigationLayout->addWidget( useBookmarks );
	useThumbnails = new QRadioButton( groupNavigation, "useThumbnails" );
	useThumbnails->setText( tr( "Display Thumbnails" ) );
	useThumbnails->setChecked(Optionen->displayThumbs);
	groupNavigationLayout->addWidget( useThumbnails );
	useLayers2 = new QRadioButton( groupNavigation, "useLayers" );
	useLayers2->setText( tr( "Display Layers Tab" ) );
	useLayers2->setChecked(Optionen->displayLayers);
	groupNavigationLayout->addWidget( useLayers2 );
	hideToolBar = new QCheckBox( tr( "Hide Viewers Toolbar" ), groupNavigation, "hideToolBar" );
	hideToolBar->setChecked(Optionen->hideToolBar);
	groupNavigationLayout->addWidget( hideToolBar );
	hideMenuBar = new QCheckBox( tr( "Hide Viewers Menubar" ), groupNavigation, "hideMenuBar" );
	hideMenuBar->setChecked(Optionen->hideMenuBar);
	groupNavigationLayout->addWidget( hideMenuBar );
	fitWindow = new QCheckBox( tr( "Zoom Pages to fit Viewer Window" ), groupNavigation, "fitWindow" );
	fitWindow->setChecked(Optionen->fitWindow);
	groupNavigationLayout->addWidget( fitWindow );
	LayoutSpecial->addWidget( groupNavigation );
	groupDisplayLayout->addLayout( LayoutSpecial );
	tabSpecialLayout->addWidget( groupDisplay );
	groupJava = new QGroupBox( tabSpecial, "groupJava" );
	groupJava->setTitle( tr( "Special Actions" ) );
	groupJava->setColumnLayout(0, Qt::Vertical );
	groupJava->layout()->setSpacing( 5 );
	groupJava->layout()->setMargin( 10 );
	groupJavaLayout = new QHBoxLayout( groupJava->layout() );
	groupJavaLayout->setAlignment( Qt::AlignTop );
	actionLabel = new QLabel( groupJava, "actionLabel" );
	actionLabel->setText( tr( "Javascript to be executed\nwhen Document is opened:" ) );
	groupJavaLayout->addWidget( actionLabel );
	actionCombo = new QComboBox( true, groupJava, "actionCombo" );
	actionCombo->setEditable(false);
	actionCombo->insertItem( tr("No Script"));
	QMap<QString,QString>::Iterator itja;
	for (itja = view->Doc->JavaScripts.begin(); itja != view->Doc->JavaScripts.end(); ++itja)
		actionCombo->insertItem(itja.key());
	if (view->Doc->JavaScripts.contains(Optionen->openAction))
		actionCombo->setCurrentText(Optionen->openAction);
	groupJavaLayout->addWidget( actionCombo );
	tabSpecialLayout->addWidget( groupJava );
	if (Optionen->PageLayout == PDFOptions::SinglePage)
		singlePage->setChecked(true);
	else if (Optionen->PageLayout == PDFOptions::OneColumn)
		continuousPages->setChecked(true);
	else if (Optionen->PageLayout == PDFOptions::TwoColumnLeft)
		doublePageLeft->setChecked(true);
	else if (Optionen->PageLayout == PDFOptions::TwoColumnRight)
		doublePageRight->setChecked(true);
	if (Optionen->Version == 15)
		useLayers2->setEnabled(true);
	else
		useLayers2->setEnabled(false);
	insertTab( tabSpecial, tr("Viewer") );

	tabSecurity = new QWidget( this, "tabSecurity" );
	tabSecurityLayout = new QVBoxLayout( tabSecurity, 11, 5, "tabSecurityLayout");
	Encry = new QCheckBox( tr( "&Use Encryption" ), tabSecurity, "Enc" );
	Encry->setChecked( Optionen->Encrypt );
	tabSecurityLayout->addWidget( Encry );
	GroupPass = new QGroupBox( tr( "Passwords" ), tabSecurity, "GroupPass" );
	GroupPass->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0,
	                                       GroupPass->sizePolicy().hasHeightForWidth() ) );
	GroupPass->setColumnLayout(0, Qt::Vertical );
	GroupPass->layout()->setSpacing( 5 );
	GroupPass->layout()->setMargin( 10 );
	GroupPassLayout = new QGridLayout( GroupPass->layout() );
	GroupPassLayout->setAlignment( Qt::AlignTop );
	TextSec2 = new QLabel( tr( "&User:" ), GroupPass, "TextSec2" );
	GroupPassLayout->addWidget( TextSec2, 1, 0 );
	TextSec1 = new QLabel( tr( "&Owner:" ), GroupPass, "TextSec1" );
	GroupPassLayout->addWidget( TextSec1, 0, 0 );
	PassOwner = new QLineEdit( GroupPass, "PassOwner" );
	PassOwner->setEchoMode( QLineEdit::Password );
	PassOwner->setText(Optionen->PassOwner);
	TextSec1->setBuddy(PassOwner);
	GroupPassLayout->addWidget( PassOwner, 0, 1 );
	PassUser = new QLineEdit( GroupPass, "PassUser" );
	PassUser->setEchoMode( QLineEdit::Password );
	PassUser->setText(Optionen->PassUser);
	TextSec2->setBuddy(PassUser);
	GroupPassLayout->addWidget( PassUser, 1, 1 );
	tabSecurityLayout->addWidget( GroupPass );

	GroupSecSet = new QGroupBox( tr( "Settings" ), tabSecurity, "GroupSecSet" );
	GroupSecSet->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0,
	                            GroupSecSet->sizePolicy().hasHeightForWidth() ) );
	GroupSecSet->setColumnLayout(0, Qt::Vertical );
	GroupSecSet->layout()->setSpacing( 5 );
	GroupSecSet->layout()->setMargin( 10 );
	GroupSecSetLayout = new QVBoxLayout( GroupSecSet->layout() );
	GroupSecSetLayout->setAlignment( Qt::AlignTop );
	PrintSec = new QCheckBox( tr( "Allow &Printing the Document" ), GroupSecSet, "PrintSec" );
	PrintSec->setChecked( Optionen->Permissions & 4 );
	GroupSecSetLayout->addWidget( PrintSec );
	ModifySec = new QCheckBox( tr( "Allow &Changing the Document" ), GroupSecSet, "ModifySec" );
	ModifySec->setChecked( Optionen->Permissions & 8 );
	GroupSecSetLayout->addWidget( ModifySec );
	CopySec = new QCheckBox( tr( "Allow Cop&ying Text and Graphics" ), GroupSecSet, "CopySec" );
	CopySec->setChecked( Optionen->Permissions & 16 );
	GroupSecSetLayout->addWidget( CopySec );
	AddSec = new QCheckBox( tr( "Allow Adding &Annotations and Fields" ), GroupSecSet, "AddSec" );
	AddSec->setChecked( Optionen->Permissions & 32 );
	GroupSecSetLayout->addWidget( AddSec );
	tabSecurityLayout->addWidget( GroupSecSet );
	if (!Encry->isChecked())
	{
		GroupSecSet->setEnabled(false);
		GroupPass->setEnabled(false);
	}
	insertTab( tabSecurity, tr( "S&ecurity" ) );

	tabColor = new QWidget( this, "tabColor" );
	tabColorLayout = new QVBoxLayout( tabColor, 11, 5, "tabColorLayout");
	ColorGroup = new QGroupBox( tr( "General" ), tabColor, "ColorGroup" );
	ColorGroup->setColumnLayout(0, Qt::Vertical );
	ColorGroup->layout()->setSpacing( 5 );
	ColorGroup->layout()->setMargin( 10 );
	ColorGroupLayout = new QHBoxLayout( ColorGroup->layout() );
	ColorGroupLayout->setAlignment( Qt::AlignTop );
	ColorText1 = new QLabel( tr( "Output &Intended For:" ), ColorGroup, "ColorText1" );
	ColorGroupLayout->addWidget( ColorText1 );
	OutCombo = new QComboBox( true, ColorGroup, "OutCombo" );
	OutCombo->insertItem( tr( "Screen / Web" ) );
	OutCombo->insertItem( tr( "Printer" ) );
	OutCombo->insertItem( tr( "Grayscale" ) );
	OutCombo->setEditable(false);
	if (Optionen->UseRGB)
		OutCombo->setCurrentItem(0);
	else
	{
		if (Optionen->isGrayscale)
			OutCombo->setCurrentItem(2);
		else
			OutCombo->setCurrentItem(1);
	}
	ColorText1->setBuddy(OutCombo);
	ColorGroupLayout->addWidget( OutCombo );
	tabColorLayout->addWidget( ColorGroup );

	useSpot = new QCheckBox( tr( "Convert Spot Colors to Process Colors" ), tabColor, "useSpot" );
	useSpot->setChecked(!Optionen->UseSpotColors);
	tabColorLayout->addWidget( useSpot );

	UseLPI = new QCheckBox( tr( "&Use Custom Rendering Settings" ), tabColor, "UseLPI" );
	UseLPI->setChecked(Optionen->UseLPI);
	tabColorLayout->addWidget( UseLPI );
	LPIgroup = new QGroupBox( tr( "Rendering Settings" ), tabColor, "LPIgroup" );
	LPIgroup->setColumnLayout(0, Qt::Vertical );
	LPIgroup->layout()->setSpacing( 5 );
	LPIgroup->layout()->setMargin( 10 );
	LPIgroupLayout = new QGridLayout( LPIgroup->layout() );
	LPIgroupLayout->setAlignment( Qt::AlignTop );
	LPIcolor = new QComboBox( true, LPIgroup, "LPIcolor" );
	LPIcolor->setEditable(false);
	QMap<QString,LPIData>::Iterator itlp;
	for (itlp = Optionen->LPISettings.begin(); itlp != Optionen->LPISettings.end(); ++itlp)
	{
		LPIcolor->insertItem( itlp.key() );
	}
	LPIcolor->setCurrentItem(0);
	LPIgroupLayout->addWidget( LPIcolor, 0, 0 );
	textLPI1 = new QLabel( tr( "Fre&quency:" ), LPIgroup, "textLPI1" );
	LPIgroupLayout->addWidget( textLPI1, 0, 1 );
	LPIfreq = new QSpinBox( LPIgroup, "LPIfreq" );
	LPIfreq->setMinValue(10);
	LPIfreq->setMaxValue(1000);
	LPIfreq->setValue(Optionen->LPISettings[LPIcolor->currentText()].Frequency);
	textLPI1->setBuddy(LPIfreq);
	LPIgroupLayout->addWidget( LPIfreq, 0, 2 );
	textLPI2 = new QLabel( tr( "&Angle:" ), LPIgroup, "textLPI2" );
	LPIgroupLayout->addWidget( textLPI2, 1, 1 );
	LPIangle = new QSpinBox( LPIgroup, "LPIangle" );
	LPIangle->setSuffix( QString::fromUtf8(" °"));
	LPIangle->setMinValue(-180);
	LPIangle->setMaxValue(180);
	LPIangle->setValue(Optionen->LPISettings[LPIcolor->currentText()].Angle);
	textLPI2->setBuddy(LPIangle);
	LPIgroupLayout->addWidget( LPIangle, 1, 2 );
	textLPI3 = new QLabel( tr( "S&pot Function:" ), LPIgroup, "textLPI3" );
	LPIgroupLayout->addWidget( textLPI3, 2, 1 );
	LPIfunc = new QComboBox( true, LPIgroup, "LPIfunc" );
	LPIfunc->setEditable(false);
	LPIfunc->insertItem( tr( "Simple Dot" ) );
	LPIfunc->insertItem( tr( "Line" ) );
	LPIfunc->insertItem( tr( "Round" ) );
	LPIfunc->insertItem( tr( "Ellipse" ) );
	LPIfunc->setCurrentItem(Optionen->LPISettings[LPIcolor->currentText()].SpotFunc);
	textLPI3->setBuddy(LPIfunc);
	LPIgroupLayout->addWidget( LPIfunc, 2, 2 );
	tabColorLayout->addWidget( LPIgroup );
	SelLPIcolor = LPIcolor->currentText();

	GroupBox9 = new QGroupBox( tr( "Solid Colors:" ), tabColor, "GroupBox9" );
	GroupBox9->setColumnLayout(0, Qt::Vertical );
	GroupBox9->layout()->setSpacing( 5 );
	GroupBox9->layout()->setMargin( 10 );
	GroupBox9Layout = new QGridLayout( GroupBox9->layout() );
	GroupBox9Layout->setAlignment( Qt::AlignTop );
	EmbedProfs = new QCheckBox( GroupBox9, "EmbedProfs" );
	EmbedProfs->setText( tr( "Use ICC Profile" ) );
	EmbedProfs->setChecked(Optionen->UseProfiles);
	GroupBox9Layout->addMultiCellWidget( EmbedProfs, 0, 0, 0, 1 );
	ProfsTxt1 = new QLabel(GroupBox9, "ProfsTxt1");
	ProfsTxt1->setText( tr( "Profile:" ) );
	GroupBox9Layout->addWidget( ProfsTxt1, 1, 0 );
	ProfsTxt2 = new QLabel( GroupBox9, "ProfsTxt2_2" );
	ProfsTxt2->setText( tr( "Rendering-Intent:" ) );
	GroupBox9Layout->addWidget( ProfsTxt2, 1, 1 );
	SolidPr = new QComboBox(true, GroupBox9, "SolidPr" );
	SolidPr->setEditable(false);
	GroupBox9Layout->addWidget( SolidPr, 2, 0 );
	IntendS = new QComboBox( true, GroupBox9, "IntendS" );
	QString tmp_ip[] = { tr("Perceptual"), tr("Relative Colorimetric"), tr("Saturation"), tr("Absolute Colorimetric")};
	size_t ar_ip = sizeof(tmp_ip) / sizeof(*tmp_ip);
	for (uint a = 0; a < ar_ip; ++a)
		IntendS->insertItem(tmp_ip[a]);
	IntendS->setEditable(false);
	GroupBox9Layout->addWidget( IntendS, 2, 1 );
	tabColorLayout->addWidget( GroupBox9 );
	ProfsGroup = new QGroupBox( tr( "Images:" ), tabColor, "ProfsGroup" );
	ProfsGroup->setColumnLayout(0, Qt::Vertical );
	ProfsGroup->layout()->setSpacing( 5 );
	ProfsGroup->layout()->setMargin( 10 );
	ProfsGroupLayout = new QGridLayout( ProfsGroup->layout() );
	ProfsGroupLayout->setAlignment( Qt::AlignTop );
	EmbedProfs2 = new QCheckBox( tr( "Use ICC Profile" ), ProfsGroup, "EmbedProfs" );
	EmbedProfs2->setChecked(Optionen->UseProfiles2);
	ProfsGroupLayout->addMultiCellWidget( EmbedProfs2, 0, 0, 0, 1 );
	NoEmbedded = new QCheckBox( ProfsGroup, "NoEmbedded" );
	NoEmbedded->setText( tr( "Don't use embedded ICC profiles" ) );
	NoEmbedded->setChecked(Optionen->EmbeddedI);
	ProfsGroupLayout->addMultiCellWidget( NoEmbedded, 1, 1, 0, 1 );
	ProfsTxt3 = new QLabel( ProfsGroup, "ProfsTxt3" );
	ProfsTxt3->setText( tr( "Profile:" ) );
	ProfsGroupLayout->addWidget( ProfsTxt3, 2, 0 );
	ProfsTxt4 = new QLabel( ProfsGroup, "ProfsTxt2_2" );
	ProfsTxt4->setText( tr( "Rendering-Intent:" ) );
	ProfsGroupLayout->addWidget( ProfsTxt4, 2, 1 );
	ImageP = new QComboBox( true, ProfsGroup, "ImageP" );
	ImageP->setEditable(false);
	ProfsGroupLayout->addWidget( ImageP, 3, 0 );
	IntendI = new QComboBox( true, ProfsGroup, "IntendS" );
	for (uint a = 0; a < ar_ip; ++a)
		IntendI->insertItem(tmp_ip[a]);
	IntendI->setEditable(false);
	ProfsGroupLayout->addWidget( IntendI, 3, 1 );
	tabColorLayout->addWidget( ProfsGroup );
	if ((Optionen->UseRGB) || (Optionen->isGrayscale))
	{
		ProfsGroup->setEnabled(false);
		GroupBox9->setEnabled(false);
		EnablePr(0);
	}
	else
		EnablePr(1);
	EnablePG();
	EnablePGI();
#ifdef HAVE_CMS

	QString tp = Optionen->SolidProf;
	if (!ScMW->InputProfiles.contains(tp))
	{
		if (vie != 0)
			tp = vie->Doc->CMSSettings.DefaultSolidColorProfile;
		else
			tp = PrefsManager::instance()->appPrefs.DCMSset.DefaultSolidColorProfile;
	}
	ProfilesL::Iterator itp;
	ProfilesL::Iterator itpend=ScMW->InputProfiles.end();
	for (itp = ScMW->InputProfiles.begin(); itp != itpend; ++itp)
	{
		SolidPr->insertItem(itp.key());
		if (itp.key() == tp)
		{
			if ((CMSuse) && (CMSavail))
				SolidPr->setCurrentItem(SolidPr->count()-1);
		}
	}
	if ((CMSuse) && (CMSavail))
		IntendS->setCurrentItem(Optionen->Intent);
	QString tp1 = Optionen->ImageProf;
	if (!ScMW->InputProfiles.contains(tp1))
	{
		if (vie != 0)
			tp1 = vie->Doc->CMSSettings.DefaultSolidColorProfile;
		else
			tp1 = PrefsManager::instance()->appPrefs.DCMSset.DefaultSolidColorProfile;
	}
	ProfilesL::Iterator itp2;
	ProfilesL::Iterator itp2end=ScMW->InputProfiles.end();
	for (itp2 = ScMW->InputProfiles.begin(); itp2 != itp2end; ++itp2)
	{
		ImageP->insertItem(itp2.key());
		if (itp2.key() == tp1)
		{
			if ((CMSuse) && (CMSavail))
				ImageP->setCurrentItem(ImageP->count()-1);
		}
	}
	if ((CMSuse) && (CMSavail))
		IntendI->setCurrentItem(Optionen->Intent2);
	if ((!CMSuse) || (!CMSavail))
	{
		GroupBox9->hide();
		ProfsGroup->hide();
	}
#else
	GroupBox9->hide();
	ProfsGroup->hide();
#endif

	QSpacerItem* spacerCG = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	tabColorLayout->addItem( spacerCG );
	insertTab( tabColor, tr( "C&olor" ) );

	tabPDFX = new QWidget( this, "tabPDFX" );
	tabPDFXLayout = new QVBoxLayout( tabPDFX, 11, 5, "tabPDFXLayout");
	X3Group = new QGroupBox( tabPDFX, "X3Group" );
	X3Group->setTitle( tr( "PDF/X-3 Output Intent" ) );
	X3Group->setColumnLayout(0, Qt::Vertical );
	X3Group->layout()->setSpacing( 5 );
	X3Group->layout()->setMargin( 10 );
	X3GroupLayout = new QGridLayout( X3Group->layout() );
	X3GroupLayout->setAlignment( Qt::AlignTop );
	PrintProfC = new QComboBox( true, X3Group, "PrintProfC" );
	PrintProfC->setEditable(false);
#ifdef HAVE_CMS

	ProfilesL::Iterator itp3;
	QString tp3 = Optionen->PrintProf;
	if (!PDFXProfiles->contains(tp3))
	{
		if (vie != 0)
			tp3 = vie->Doc->CMSSettings.DefaultPrinterProfile;
		else
			tp3 = PrefsManager::instance()->appPrefs.DCMSset.DefaultPrinterProfile;
	}
	for (itp3 = PDFXProfiles->begin(); itp3 != PDFXProfiles->end(); ++itp3)
	{
		PrintProfC->insertItem(itp3.key());
		if (itp3.key() == tp3)
			PrintProfC->setCurrentItem(PrintProfC->count()-1);
	}
#endif
	X3GroupLayout->addWidget( PrintProfC, 0, 1 );
	InfoString = new QLineEdit( X3Group, "InfoString" );
	InfoString->setText(Optionen->Info);
	X3GroupLayout->addWidget( InfoString, 1, 1 );
	PDFX2 = new QLabel( InfoString, tr( "&Info String:" ), X3Group, "PDFX2" );
	X3GroupLayout->addWidget( PDFX2, 1, 0 );
	PDFX1 = new QLabel( PrintProfC, tr( "Output &Profile:" ), X3Group, "PDFX1" );
	X3GroupLayout->addWidget( PDFX1, 0, 0 );
	tabPDFXLayout->addWidget( X3Group );
	BleedGroup = new QGroupBox( tabPDFX, "BleedGroup" );
	BleedGroup->setTitle( tr( "Trim Box" ) );
	BleedGroup->setColumnLayout(0, Qt::Vertical );
	BleedGroup->layout()->setSpacing( 5 );
	BleedGroup->layout()->setMargin( 11 );
	BleedGroupLayout = new QGridLayout( BleedGroup->layout() );
	BleedGroupLayout->setAlignment( Qt::AlignTop );
	BleedIcon = new QLabel( BleedGroup, "BleedIcon" );
	BleedIcon->setMinimumSize( QSize( 74, 84 ) );
	BleedIcon->setMaximumSize( QSize( 74, 84 ) );
	BleedIcon->setFrameShape( QLabel::Panel );
	BleedIcon->setFrameShadow( QLabel::Sunken );
	BleedIcon->setScaledContents( false );
	BleedIcon->setAlignment( static_cast<int>( QLabel::AlignCenter ) );
	BleedGroupLayout->addWidget( BleedIcon, 1, 2 );
	BleedTop = new MSpinBox( BleedGroup, precision );
	BleedGroupLayout->addWidget( BleedTop, 0, 2 );
	BleedBottom = new MSpinBox( BleedGroup, precision );
	BleedGroupLayout->addWidget( BleedBottom, 2, 2 );
	BleedRight = new MSpinBox( BleedGroup, precision );
	BleedGroupLayout->addWidget( BleedRight, 1, 3 );
	BleedLeft = new MSpinBox( BleedGroup, precision );
	BleedGroupLayout->addWidget( BleedLeft, 1, 1 );
	QSpacerItem* spacerPX = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	BleedGroupLayout->addItem( spacerPX, 1, 0 );
	QSpacerItem* spacerPX2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	BleedGroupLayout->addItem( spacerPX2, 1, 4 );
	tabPDFXLayout->addWidget( BleedGroup );
	insertTab( tabPDFX, tr( "PDF/X-&3" ) );
	BleedTop->setSuffix( unit );
	BleedTop->setMinValue(0);
	BleedTop->setMaxValue(PageH*unitRatio);
	BleedTop->setValue(Optionen->BleedTop*unitRatio);
	BleedBottom->setSuffix( unit );
	BleedBottom->setMinValue(0);
	BleedBottom->setMaxValue(PageH*unitRatio);
	BleedBottom->setValue(Optionen->BleedBottom*unitRatio);
	BleedRight->setSuffix( unit );
	BleedRight->setMinValue(0);
	BleedRight->setMaxValue(PageB*unitRatio);
	BleedRight->setValue(Optionen->BleedRight*unitRatio);
	BleedLeft->setSuffix( unit );
	BleedLeft->setMinValue(0);
	BleedLeft->setMaxValue(PageB*unitRatio);
	BleedLeft->setValue(Optionen->BleedLeft*unitRatio);
#ifdef HAVE_CMS
	if ((!CMSuse) || (!CMSavail))
		setTabEnabled(tabPDFX, false);
	if ((CMSuse) && (CMSavail) && (Optionen->Version == 12) && (!PDFXProfiles->isEmpty()))
		EnablePDFX(3);
	else
		setTabEnabled(tabPDFX, false);
#else
	setTabEnabled(tabPDFX, false);
#endif
	BleedChanged();
	if (vie != 0)
	{
		PgSel = 0;
		Pages->setCurrentItem(0);
		SetEffOpts(0);
		Pages->setEnabled(false);
		Effects->setEnabled(false);
		PagePrev->setEnabled(false);
		DoEffects();
		if (CheckBox10->isChecked())
		{
			PageTime->setValue(EffVal[0].pageViewDuration);
			EffectTime->setValue(EffVal[0].pageEffectDuration);
			EffectType->setCurrentItem(EffVal[0].effectType);
			EDirection->setCurrentItem(EffVal[0].Dm);
			EDirection_2->setCurrentItem(EffVal[0].M);
			EDirection_2_2->setCurrentItem(EffVal[0].Di);
			SetEffOpts(EffectType->currentItem());
		}
		connect(EmbedFonts, SIGNAL(clicked()), this, SLOT(EmbedAll()));
		connect(AvailFlist, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SelAFont(QListBoxItem*)));
		connect(EmbedList, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SelEFont(QListBoxItem*)));
		connect(ToEmbed, SIGNAL(clicked()), this, SLOT(PutToEmbed()));
		connect(FromEmbed, SIGNAL(clicked()), this, SLOT(RemoveEmbed()));
		connect(SubsetFonts, SIGNAL(clicked()), this, SLOT(SubsetAll()));
		connect(SubsetList, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SelSFont(QListBoxItem*)));
		connect(ToSubset, SIGNAL(clicked()), this, SLOT(PutToSubset()));
		connect(FromSubset, SIGNAL(clicked()), this, SLOT(RemoveSubset()));
		connect(PagePrev, SIGNAL(clicked()), this, SLOT(PagePr()));
		connect(Pages, SIGNAL(highlighted(int)), this, SLOT(SetPgEff(int)));
		connect(EffectType, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));
		connect(EDirection_2_2, SIGNAL(activated(int)), this, SLOT(ValidDI(int)));
		connect(CheckBox10, SIGNAL(clicked()), this, SLOT(DoEffects()));
		connect(EonAllPg, SIGNAL(clicked()), this, SLOT(EffectOnAll()));
		connect(InfoString, SIGNAL(textChanged(const QString &)), this, SLOT(checkInfo()));
		connect(InfoString, SIGNAL(returnPressed()), this, SLOT(checkInfo()));
		connect(InfoString, SIGNAL(lostFocus()), this, SLOT(checkInfo()));
		QToolTip::add( EmbedFonts, "<qt>" + tr( "Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document." ) + "</qt>");
		QToolTip::add( CheckBox10, "<qt>" + tr( "Enables presentation effects when using Adobe&#174; Reader&#174; in full screen mode." ) + "</qt>");
		QToolTip::add( PagePrev, "<qt>" + tr( "Show page previews of each page listed above." ) + "</qt>");
		QToolTip::add( PageTime, "<qt>" + tr( "Length of time the page is shown before the presentation starts on the selected page." ) + "</qt>" );
		QToolTip::add( EffectTime, "<qt>" + tr( "Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down." ) + "</qt>" );
		QToolTip::add( EffectType, "<qt>" + tr( "Type of the display effect." ) + "</qt>" );
		QToolTip::add( EDirection, "<qt>" + tr( "Direction of the effect of moving lines for the split and blind effects." ) + "</qt>" );
		QToolTip::add( EDirection_2, "<qt>" + tr( "Starting position for the box and split effects." ) + "</qt>" );
		QToolTip::add( EDirection_2_2, "<qt>" + tr( "Direction of the glitter or wipe effects." ) + "</qt>" );
		QToolTip::add( EonAllPg, "<qt>" + tr( "Apply the selected effect to all pages." ) + "</qt>" );
	}
	connect(AllPages, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect(DSColor, SIGNAL(clicked()), this, SLOT(DoDownsample()));
	connect(MirrorH, SIGNAL(clicked()), this, SLOT(PDFMirror()));
	connect(MirrorV, SIGNAL(clicked()), this, SLOT(PDFMirror()));
	connect(RotateDeg, SIGNAL(activated(int)), this, SLOT(Rotation(int)));
	connect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
	connect(EmbedProfs, SIGNAL(clicked()), this, SLOT(EnablePG()));
	connect(EmbedProfs2, SIGNAL(clicked()), this, SLOT(EnablePGI()));
	connect(NoEmbedded, SIGNAL(clicked()), this, SLOT(EnablePGI2()));
	connect(PDFVersionCombo, SIGNAL(activated(int)), this, SLOT(EnablePDFX(int)));
	connect(BleedTop, SIGNAL(valueChanged(int)), this, SLOT(BleedChanged()));
	connect(BleedBottom, SIGNAL(valueChanged(int)), this, SLOT(BleedChanged()));
	connect(BleedLeft, SIGNAL(valueChanged(int)), this, SLOT(BleedChanged()));
	connect(BleedRight, SIGNAL(valueChanged(int)), this, SLOT(BleedChanged()));
	connect(Encry, SIGNAL(clicked()), this, SLOT(ToggleEncr()));
	connect(UseLPI, SIGNAL(clicked()), this, SLOT(EnableLPI2()));
	connect(LPIcolor, SIGNAL(activated(int)), this, SLOT(SelLPIcol(int)));
	//tooltips
	QToolTip::add( AllPages, "<qt>" + tr( "Export all pages to PDF" ) + "</qt>" );
	QToolTip::add( OnlySome, "<qt>" + tr( "Export a range of pages to PDF" ) );
	QToolTip::add( PageNr, "<qt>" + tr( "Insert a comma separated list of tokens where "
		                                    "a token can be * for all the pages, 1-5 for "
		                                    "a range of pages or a single page number.") + "</qt>" );

	QToolTip::add( PDFVersionCombo, "<qt>" + tr( "Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer." ) + "</qt>");
	QToolTip::add( ComboBind, "<qt>" + tr( "Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left." ) + "</qt>" );
	QToolTip::add( CheckBox1, "<qt>" + tr( "Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation." ) + "</qt>" );
	QToolTip::add( Article, "<qt>" + tr( "Generate PDF Articles, which is useful for navigating linked articles in a PDF." ) + "</qt>" );
	QToolTip::add( useLayers, "<qt>" + tr( "Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen." ) + "</qt>" );
	QToolTip::add( CheckBM, "<qt>" + tr( "Embed the bookmarks you created in your document. These are useful for navigating long PDF documents." ) + "</qt>" );
	QToolTip::add( Resolution, "<qt>" + tr( "Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos." ) + "</qt>" );
	QToolTip::add( Compression, "<qt>" + tr( "Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size." ) + "</qt>" );
	QToolTip::add( CMethod, "<qt>" + tr( "Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options." ) + "</qt>");
	QToolTip::add( CQuality, "<qt>" + tr( "Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level." ) + "</qt>");
	QToolTip::add( DSColor, "<qt>" + tr( "Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export." ) + "</qt>" );
	QToolTip::add( ValC, "<qt>" + tr( "DPI (Dots Per Inch) for image export.") + "</qt>" );
	QToolTip::add( Encry, "<qt>" + tr( "Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations." ) + "</qt>" );
	QToolTip::add( PassOwner, "<qt>" + tr( "Choose a master password which enables or disables all the security features in your exported PDF" ) + "</qt>" );
	QToolTip::add( PassUser, "<qt>" + tr( "Choose a password for users to be able to read your PDF." ) + "</qt>" );
	QToolTip::add( PrintSec, "<qt>" + tr( "Allow printing of the PDF. If un-checked, printing is prevented. " ) + "</qt>" );
	QToolTip::add( ModifySec, "<qt>" + tr( "Allow modifying of the PDF. If un-checked, modifying the PDF is prevented." ) + "</qt>" );
	QToolTip::add( CopySec, "<qt>" + tr( "Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied." ) + "</qt>" );
	QToolTip::add( AddSec, "<qt>" + tr( "Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented." ) + "</qt>" );
	QToolTip::add( OutCombo, "<qt>" + tr( "Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer." ) + "</qt>" );
	QToolTip::add( UseLPI, "<qt>" + tr( "This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems." ) + "</qt>" );
	QToolTip::add( EmbedProfs, "<qt>" + tr( "Embed a color profile for solid colors" ) + "</qt>" );
	QToolTip::add( SolidPr, "<qt>" + tr( "Color profile for solid colors" ) + "</qt>" );
	QToolTip::add( IntendS, "<qt>" + tr( "Rendering intent for solid colors" ) + "</qt>" );
	QToolTip::add( EmbedProfs2, "<qt>" + tr( "Embed a color profile for images" ) + "</qt>" );
	QToolTip::add( NoEmbedded, "<qt>" + tr( "Do not use color profiles that are embedded in source images" ) + "</qt>" );
	QToolTip::add( ImageP, "<qt>" + tr( "Color profile for images" ) + "</qt>" );
	QToolTip::add( IntendI, "<qt>" + tr( "Rendering intent for images" ) + "</qt>" );
	QToolTip::add( PrintProfC, "<qt>" + tr( "Output profile for printing. If possible, get some guidance from your printer on profile selection." ) + "</qt>" );
	QToolTip::add( InfoString, "<qt>" + tr( "Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document." ) + "</qt>" );
	QToolTip::add( BleedTop, "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
	QToolTip::add( BleedBottom, "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
	QToolTip::add( BleedLeft, "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
	QToolTip::add( BleedRight, "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
	QToolTip::add( MirrorH, "<qt>" + tr( "Mirror Page(s) horizontally" ) + "</qt>" );
	QToolTip::add( MirrorV, "<qt>" + tr( "Mirror Page(s) vertically" ) + "</qt>" );
	QToolTip::add( useSpot,"<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");
}

void TabPDFOptions::restoreDefaults()
{}

void TabPDFOptions::checkInfo()
{
	if ((PDFVersionCombo->currentItem() == 3) && (InfoString->text().isEmpty()))
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

void TabPDFOptions::BleedChanged()
{
	BleedTop->setMaxValue(pageH*unitRatio-BleedBottom->value());
	BleedBottom->setMaxValue(pageH*unitRatio-BleedTop->value());
	BleedRight->setMaxValue(pageB*unitRatio-BleedLeft->value());
	BleedLeft->setMaxValue(pageB*unitRatio-BleedRight->value());
	QPixmap pm(70,80);
	pm.fill(white);
	QPainter p;
	p.begin(&pm);
	p.setBrush(NoBrush);
	p.setPen(black);
	p.drawRect(0, 0, pm.width(), pm.height());
	p.setPen(QPen(black, 1, DotLine, FlatCap, MiterJoin));
	int x = qRound((BleedLeft->value() / unitRatio) * (70.0 / pageB));
	int y = qRound((BleedTop->value() / unitRatio) * (80.0 / pageH));
	int w =qRound((pageB-((BleedLeft->value()+BleedRight->value())/unitRatio))*(70.0/pageB));
	int h = qRound((pageH-((BleedTop->value()+BleedBottom->value())/unitRatio))*(80.0/pageH));
	p.drawRect(x, y, w, h);
	BleedIcon->setPixmap(pm);
	p.end();
}

void TabPDFOptions::enableCMS(bool enable)
{
	disconnect(PDFVersionCombo, SIGNAL(activated(int)), this, SLOT(EnablePDFX(int)));
	int a = PDFVersionCombo->currentItem();
	PDFVersionCombo->clear();
	PDFVersionCombo->insertItem("PDF 1.3");
	PDFVersionCombo->insertItem("PDF 1.4");
	PDFVersionCombo->insertItem("PDF 1.5");
	cms=enable;
	if (enable)
		PDFVersionCombo->insertItem("PDF/X-3");
	else
		a = QMIN(a, 3);
	PDFVersionCombo->setCurrentItem(a);
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
		setTabEnabled(tabPDFX, false);
		setTabEnabled(tabSecurity, true);
		OutCombo->setEnabled(true);
		EmbedProfs2->setEnabled(true);
		emit hasInfo();
		if (view != 0)
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
	OutCombo->setCurrentItem(1);
	OutCombo->setEnabled(false);
	EnablePr(1);
	EmbedProfs2->setChecked(true);
	EmbedProfs2->setEnabled(false);
	if (view != 0)
	{
		EmbedFonts->setChecked(true);
		EmbedAll();
		CheckBox10->setChecked(false);
		CheckBox10->setEnabled(false);
		EmbedFonts->setEnabled(false);
		FromEmbed->setEnabled(false);
		ToEmbed->setEnabled(false);
		if (InfoString->text().isEmpty())
			emit noInfo();
		else
			emit hasInfo();
	}
	EnablePGI();
	setTabEnabled(tabPDFX, true);
	setTabEnabled(tabSecurity, false);
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
#ifdef HAVE_CMS
		QString tp = Opts->SolidProf;
		if (!ScMW->InputProfiles.contains(tp))
		{
			if (view != 0)
				tp = view->Doc->CMSSettings.DefaultSolidColorProfile;
			else
				tp = PrefsManager::instance()->appPrefs.DCMSset.DefaultSolidColorProfile;
		}
		SolidPr->clear();
		ProfilesL::Iterator itp;
		ProfilesL::Iterator itpend=ScMW->InputProfiles.end();
		for (itp = ScMW->InputProfiles.begin(); itp != itpend; ++itp)
		{
			SolidPr->insertItem(itp.key());
			if (itp.key() == tp)
			{
				if (cms)
					SolidPr->setCurrentItem(SolidPr->count()-1);
			}
		}
		if (cms)
			IntendS->setCurrentItem(Opts->Intent);
		QString tp1 = Opts->ImageProf;
		if (!ScMW->InputProfiles.contains(tp1))
		{
			if (view != 0)
				tp1 = view->Doc->CMSSettings.DefaultSolidColorProfile;
			else
				tp1 = PrefsManager::instance()->appPrefs.DCMSset.DefaultSolidColorProfile;
		}
		ImageP->clear();
		ProfilesL::Iterator itp2;
		ProfilesL::Iterator itp2end=ScMW->InputProfiles.end();
		for (itp2 = ScMW->InputProfiles.begin(); itp2 != itp2end; ++itp2)
		{
			ImageP->insertItem(itp2.key());
			if (itp2.key() == tp1)
			{
				if (cms)
					ImageP->setCurrentItem(ImageP->count()-1);
			}
		}
		if (cms)
			IntendI->setCurrentItem(Opts->Intent2);
#endif
		if (cms)
		{
			GroupBox9->show();
			ProfsGroup->show();
			UseLPI->hide();
			useSpot->hide();
		}
		else
		{
			GroupBox9->hide();
			ProfsGroup->hide();
			useSpot->show();
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
		UseLPI->hide();
		LPIgroup->hide();
	}
}

void TabPDFOptions::EnableLPI2()
{
	if (UseLPI->isChecked())
		LPIgroup->show();
	else
		LPIgroup->hide();
}

void TabPDFOptions::SelLPIcol(int c)
{
	Opts->LPISettings[SelLPIcolor].Frequency = LPIfreq->value();
	Opts->LPISettings[SelLPIcolor].Angle = LPIangle->value();
	Opts->LPISettings[SelLPIcolor].SpotFunc = LPIfunc->currentItem();
	LPIfreq->setValue(Opts->LPISettings[LPIcolor->text(c)].Frequency);
	LPIangle->setValue(Opts->LPISettings[LPIcolor->text(c)].Angle);
	LPIfunc->setCurrentItem(Opts->LPISettings[LPIcolor->text(c)].SpotFunc);
	SelLPIcolor = LPIcolor->text(c);
}

void TabPDFOptions::SelRange(bool e)
{
	bool setter = e ? false : true;
	PageNr->setEnabled( setter );
	if (setter == false)
		CheckBM->setChecked(false);
}

void TabPDFOptions::EffectOnAll()
{
	for (uint pg = 0; pg < view->Doc->Pages->count(); ++pg)
	{
		EffVal[pg].pageViewDuration = PageTime->value();
		EffVal[pg].pageEffectDuration = EffectTime->value();
		EffVal[pg].effectType = EffectType->currentItem();
		EffVal[pg].Dm = EDirection->currentItem();
		EffVal[pg].M = EDirection_2->currentItem();
		EffVal[pg].Di = EDirection_2_2->currentItem();
	}
}

void TabPDFOptions::PDFMirror()
{
	Opts->MirrorH = MirrorH->isOn();
	Opts->MirrorV = MirrorV->isOn();
}

void TabPDFOptions::Rotation( int value )
{
	Opts->RotateDeg = value * 90;
}

void TabPDFOptions::DoEffects()
{
	bool setter = CheckBox10->isChecked() ? true : false;
	Pages->setEnabled(setter);
	Effects->setEnabled(setter);
	PagePrev->setEnabled(setter);
	useFullScreen->setChecked(setter);
}

void TabPDFOptions::ValidDI(int nr)
{
	if (!EDirection_2_2->listBox()->item(nr)->isSelectable())
		EDirection_2_2->setCurrentItem(0);
}

void TabPDFOptions::SetPgEff(int nr)
{
	if (nr < 0)
		return;
	EffVal[PgSel].pageViewDuration = PageTime->value();
	EffVal[PgSel].pageEffectDuration = EffectTime->value();
	EffVal[PgSel].effectType = EffectType->currentItem();
	EffVal[PgSel].Dm = EDirection->currentItem();
	EffVal[PgSel].M = EDirection_2->currentItem();
	EffVal[PgSel].Di = EDirection_2_2->currentItem();
	SetEffOpts(EffVal[nr].effectType);
	PageTime->setValue(EffVal[nr].pageViewDuration);
	EffectTime->setValue(EffVal[nr].pageEffectDuration);
	EffectType->setCurrentItem(EffVal[nr].effectType);
	EDirection->setCurrentItem(EffVal[nr].Dm);
	EDirection_2->setCurrentItem(EffVal[nr].M);
	EDirection_2_2->setCurrentItem(EffVal[nr].Di);
	PgSel = nr;
}

void TabPDFOptions::SetEffOpts(int nr)
{
	EDirection_2_2->listBox()->item(2)->setSelectable(false);
	EDirection_2_2->listBox()->item(3)->setSelectable(false);
	EDirection_2_2->listBox()->item(4)->setSelectable(false);
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
			EDirection_2_2->listBox()->item(2)->setSelectable(true);
			EDirection_2_2->listBox()->item(3)->setSelectable(true);
		}
		else
			EDirection_2_2->listBox()->item(4)->setSelectable(true);
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
	disconnect(Pages, SIGNAL(highlighted(int)), this, SLOT(SetPgEff(int)));
	QString tmp;
	QPixmap pm;
	int ci = Pages->currentItem();
	if (PagePrev->isChecked())
	{
		for (uint pg = 0; pg < view->Doc->Pages->count(); ++pg)
		{
			pm.convertFromImage(view->PageToPixmap(pg, 70));
			Pages->changeItem(pm, tr("Page")+" "+tmp.setNum(pg+1), pg);
		}
	}
	else
	{
		for (uint pg = 0; pg < view->Doc->Pages->count(); ++pg)
			Pages->changeItem( tr("Page")+" "+tmp.setNum(pg+1), pg);
	}
	if (ci != -1)
	{
		PgSel = ci;
		Pages->setCurrentItem(ci);
	}
	else
	{
		PgSel = 0;
		Pages->clearSelection();
	}
	connect(Pages, SIGNAL(highlighted(int)), this, SLOT(SetPgEff(int)));
}

void TabPDFOptions::DoDownsample()
{
	if (DSColor->isChecked())
	{
		ValC->setEnabled(true);
		if (ValC->value() > Resolution->value())
			ValC->setValue(Resolution->value());
		ValC->setMaxValue(Resolution->value());
		ValC->setMinValue(35);
	}
	else
		ValC->setEnabled(false);
}

void TabPDFOptions::RemoveEmbed()
{
	FontsToEmbed.remove(EmbedList->currentText());
	EmbedList->removeItem(EmbedList->currentItem());
	EmbedList->clearSelection();
	if (EmbedList->count() == 0)
	{
		FromEmbed->setEnabled(false);
		ToSubset->setEnabled(false);
	}
}

void TabPDFOptions::PutToEmbed()
{
	if (EmbedList->count() != 0)
	{
		if (!AllFontsP[AvailFlist->currentText()]->Subset)
		{
			if (EmbedList->findItem(AvailFlist->currentText()) == NULL)
			{
				FontsToEmbed.append(AvailFlist->currentText());
				EmbedList->insertItem(AvailFlist->currentText());
			}
		}
		else
		{
			if (SubsetList->count() != 0)
			{
				if (SubsetList->findItem(AvailFlist->currentText()) == NULL)
				{
					FontsToSubset.append(AvailFlist->currentText());
					SubsetList->insertItem(AvailFlist->currentText());
				}
			}
			else
			{
				FontsToSubset.append(AvailFlist->currentText());
				SubsetList->insertItem(AvailFlist->currentText());
			}
		}
	}
	else
	{
		if (!AllFontsP[AvailFlist->currentText()]->Subset)
		{
			FontsToEmbed.append(AvailFlist->currentText());
			EmbedList->insertItem(AvailFlist->currentText());
		}
		else
		{
			if (SubsetList->count() != 0)
			{
				if (SubsetList->findItem(AvailFlist->currentText()) == NULL)
				{
					FontsToSubset.append(AvailFlist->currentText());
					SubsetList->insertItem(AvailFlist->currentText());
				}
			}
			else
			{
				FontsToSubset.append(AvailFlist->currentText());
				SubsetList->insertItem(AvailFlist->currentText());
			}
		}
	}
}

void TabPDFOptions::RemoveSubset()
{
	if (!AllFontsP[SubsetList->currentText()]->Subset)
	{
		FontsToSubset.remove(SubsetList->currentText());
		FontsToEmbed.append(SubsetList->currentText());
		EmbedList->insertItem(SubsetList->currentText());
		SubsetList->removeItem(SubsetList->currentItem());
		SubsetList->clearSelection();
		if (SubsetList->count() == 0)
			FromSubset->setEnabled(false);
	}
}

void TabPDFOptions::PutToSubset()
{
	if (SubsetList->count() != 0)
	{
		if (SubsetList->findItem(EmbedList->currentText()) == NULL)
		{
			FontsToSubset.append(EmbedList->currentText());
			SubsetList->insertItem(EmbedList->currentText());
		}
	}
	else
	{
		FontsToSubset.append(EmbedList->currentText());
		SubsetList->insertItem(EmbedList->currentText());
	}
	FontsToEmbed.remove(EmbedList->currentText());
	EmbedList->removeItem(EmbedList->currentItem());
	EmbedList->clearSelection();
	if (EmbedList->count() == 0)
	{
		FromEmbed->setEnabled(false);
		ToSubset->setEnabled(false);
	}
}

void TabPDFOptions::SelAFont(QListBoxItem *c)
{
	if ((c != NULL) && (!EmbedFonts->isChecked()))
	{
		FromEmbed->setEnabled(false);
		if (c->isSelectable())
			ToEmbed->setEnabled(true);
		ToSubset->setEnabled(false);
		FromSubset->setEnabled(false);
		EmbedList->clearSelection();
		SubsetList->clearSelection();
	}
}

void TabPDFOptions::SelEFont(QListBoxItem *c)
{
	if ((c != NULL) && (!EmbedFonts->isChecked()))
	{
		if (!isTabEnabled(tabPDFX))
			FromEmbed->setEnabled(true);
		ToEmbed->setEnabled(false);
		ToSubset->setEnabled(true);
		FromSubset->setEnabled(false);
		AvailFlist->clearSelection();
		SubsetList->clearSelection();
	}
}

void TabPDFOptions::SelSFont(QListBoxItem *c)
{
	if ((c != NULL) && (!EmbedFonts->isChecked()))
	{
		FromSubset->setEnabled(true);
		ToSubset->setEnabled(false);
		ToEmbed->setEnabled(false);
		FromEmbed->setEnabled(false);
		EmbedList->clearSelection();
		AvailFlist->clearSelection();
	}
}

void TabPDFOptions::EmbedAll()
{
	if (EmbedFonts->isChecked())
	{
		SubsetFonts->setChecked(false);
		EmbedList->clear();
		FontsToEmbed.clear();
		SubsetList->clear();
		FontsToSubset.clear();
		FromEmbed->setEnabled(false);
		ToEmbed->setEnabled(false);
		ToSubset->setEnabled(false);
		FromSubset->setEnabled(false);
		for (uint a=0; a < AvailFlist->count(); ++a)
		{
			if (AvailFlist->item(a)->isSelectable())
			{
				if (!AllFontsP[AvailFlist->item(a)->text()]->Subset)
				{
					FontsToEmbed.append(AvailFlist->item(a)->text());
					EmbedList->insertItem(AvailFlist->item(a)->text());
				}
				else
				{
					FontsToSubset.append(AvailFlist->item(a)->text());
					SubsetList->insertItem(AvailFlist->item(a)->text());
				}
			}
		}
	}
}

void TabPDFOptions::SubsetAll()
{
	if (SubsetFonts->isChecked())
	{
		EmbedFonts->setChecked(false);
		EmbedList->clear();
		FontsToEmbed.clear();
		SubsetList->clear();
		FontsToSubset.clear();
		FromEmbed->setEnabled(false);
		ToEmbed->setEnabled(false);
		ToSubset->setEnabled(false);
		FromSubset->setEnabled(false);
		for (uint a=0; a < AvailFlist->count(); ++a)
		{
			if (AvailFlist->item(a)->isSelectable())
			{
				FontsToSubset.append(AvailFlist->item(a)->text());
				SubsetList->insertItem(AvailFlist->item(a)->text());
			}
		}
	}
}
