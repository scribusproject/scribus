/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "pdfopts.h"
#include "pdfopts.moc"
#include "customfdialog.h"
#include "prefsfile.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

extern QPixmap loadIcon(QString nam);
extern bool overwrite(QWidget *parent, QString filename);
extern double UmReFaktor;
extern ProfilesL InputProfiles;
#ifdef HAVE_CMS
extern bool CMSuse;
#endif
extern bool CMSavail;
extern PrefsFile* prefsFile;

PDF_Opts::PDF_Opts( QWidget* parent,  QString Fname, QMap<QString,QFont> DocFonts, ScribusView *vie, PDFOpt *Optionen, QValueList<PreSet> Eff, ProfilesL *PDFXProfiles, SCFonts &AllFonts)
		: QDialog( parent, "pdf", true, 0 )
{
	setCaption( tr( "Save as PDF" ) );
	setIcon(loadIcon("AppIcon.png"));
	FontsToEmbed.clear();
	view = vie;
	EffVal = Eff;
	Opts = Optionen;
	Einheit = view->Doc->Einheit;
	PDFOptsLayout = new QVBoxLayout( this );
	PDFOptsLayout->setSpacing( 5 );
	PDFOptsLayout->setMargin( 11 );
	Name = new QLabel( tr( "O&utput to File:" ), this, "Name" );
	Name->setFrameShape( QLabel::NoFrame );
	PDFOptsLayout->addWidget( Name );
	Layout5 = new QHBoxLayout;
	Layout5->setSpacing( 5 );
	Layout5->setMargin( 0 );
	Datei = new QLineEdit( this, "Datei" );
	Datei->setMinimumSize( QSize( 268, 22 ) );
	if (Optionen->Datei != "")
		Datei->setText(Optionen->Datei);
	else
	{
		PrefsContext* dirs = prefsFile->getContext("dirs");
		QFileInfo fi = QFileInfo(Fname);
		QString pdfdir = dirs->get("pdf", fi.dirPath());
		if (pdfdir.right(1) != "/")
			pdfdir += "/";
		Datei->setText(pdfdir+fi.baseName()+".pdf");
	}
	Name->setBuddy(Datei);
	Layout5->addWidget( Datei );
	FileC = new QToolButton( this, "FileC" );
	FileC->setText( tr( "Cha&nge..." ) );
	FileC->setMinimumSize( QSize( 88, 24 ) );
	Layout5->addWidget( FileC );
	PDFOptsLayout->addLayout( Layout5 );
	Options = new QTabWidget( this, "TabWidget2" );
	tabGeneral = new QWidget( Options, "tabGeneral" );
	tabLayout = new QVBoxLayout( tabGeneral );
	tabLayout->setSpacing( 5 );
	tabLayout->setMargin( 11 );

	Layout13 = new QHBoxLayout( 0, 0, 5, "Layout13");

	RangeGroup = new QButtonGroup( tabGeneral, "RangeGroup" );
	RangeGroup->setTitle( tr( "Export Range" ) );
	RangeGroup->setColumnLayout(0, Qt::Vertical );
	RangeGroup->layout()->setSpacing( 5 );
	RangeGroup->layout()->setMargin( 11 );
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
	GroupBox1Layout->setMargin( 11 );
	TextLabel1 = new QLabel( tr( "Compatibilit&y:" ), GroupBox1, "TextLabel1" );
	TextLabel1->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	GroupBox1Layout->addWidget( TextLabel1, 0, 0 );
	ComboBox1 = new QComboBox( true, GroupBox1, "ComboBox1" );
	ComboBox1->insertItem("Acrobat 4.0");
	ComboBox1->insertItem("Acrobat 5.0");
#ifdef HAVE_CMS
	if ((CMSuse) && (CMSavail) && (!PDFXProfiles->isEmpty()))
		ComboBox1->insertItem("PDF/X-3");
#endif
	ComboBox1->setEditable(false);
	TextLabel1->setBuddy(ComboBox1);
#ifdef HAVE_CMS
	if ((CMSuse) && (CMSavail))
	{
		if (Optionen->Version == 12)
			ComboBox1->setCurrentItem(2);
	}
	else
		ComboBox1->setCurrentItem(0);
#endif
	if (Optionen->Version == 13)
		ComboBox1->setCurrentItem(0);
	if (Optionen->Version == 14)
		ComboBox1->setCurrentItem(1);
	GroupBox1Layout->addMultiCellWidget( ComboBox1, 0, 0, 1, 2, AlignLeft );
	TextLabel1x = new QLabel( tr( "&Binding:" ), GroupBox1, "TextLabel1" );
	TextLabel1x->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	GroupBox1Layout->addWidget( TextLabel1x, 1, 0 );
	ComboBind = new QComboBox( true, GroupBox1, "ComboBind" );
	ComboBind->insertItem( tr("Left Margin"));
	ComboBind->insertItem( tr("Right Margin"));
	ComboBind->setEditable(false);
	ComboBind->setCurrentItem(Optionen->Binding);
	TextLabel1x->setBuddy(ComboBind);
	GroupBox1Layout->addMultiCellWidget( ComboBind, 1, 1, 1, 2, AlignLeft );
	CheckBox1 = new QCheckBox( tr( "Generate &Thumbnails" ), GroupBox1, "CheckBox1" );
	CheckBox1->setChecked(Optionen->Thumbnails);
	GroupBox1Layout->addMultiCellWidget( CheckBox1, 2, 2, 0, 2 );
	Article = new QCheckBox( tr( "Save &Linked Text Frames as PDF Articles" ), GroupBox1, "CheckBox1" );
	Article->setChecked(Optionen->Articles);
	GroupBox1Layout->addMultiCellWidget( Article, 3, 3, 0, 2 );
	CheckBM = new QCheckBox( tr( "&Include Bookmarks" ), GroupBox1, "E" );
	CheckBM->setChecked(Optionen->Bookmarks);
	GroupBox1Layout->addMultiCellWidget( CheckBM, 4, 4, 0, 2 );
	Resolution = new QSpinBox( GroupBox1, "Resolution" );
	Resolution->setMaxValue( 4000 );
	Resolution->setMinValue( 35 );
	Resolution->setValue(Optionen->Resolution);
	Resolution->setSuffix( tr( " dpi" ) );
	TextLabel2 = new QLabel( Resolution, tr( "&Resolution:" ), GroupBox1, "TextLabel2" );
	TextLabel2->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	GroupBox1Layout->addWidget( TextLabel2, 5, 0 );
	GroupBox1Layout->addWidget( Resolution, 5, 1, AlignLeft );
	Layout13->addWidget( GroupBox1 );
	tabLayout->addLayout( Layout13 );
	Compression = new QCheckBox( tr( "Compress Text and &Vector Graphics" ), tabGeneral, "Compression" );
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
	TextCom1 = new QLabel( CMethod, tr( "&Method:" ), CBox, "TextCom1" );
	CBoxLayout->addWidget( TextCom1, 0, 0 );	CBoxLayout->addWidget( CMethod, 0, 1 );
	TextCom2 = new QLabel( tr( "&Quality:" ), CBox, "TextCom2" );
	CBoxLayout->addWidget( TextCom2, 1, 0 );
	CQuality = new QComboBox( true, CBox, "CQuality" );
	CQuality->clear();
	CQuality->insertItem( tr( "Maximum" ) );
	CQuality->insertItem( tr( "High" ) );
	CQuality->insertItem( tr( "Medium" ) );
	CQuality->insertItem( tr( "Low" ) );
	CQuality->insertItem( tr( "Minimum" ) );
	CQuality->setEditable(false);
	CQuality->setCurrentItem(Optionen->Quality);
	TextCom2->setBuddy(CQuality);
	CBoxLayout->addWidget( CQuality, 1, 1 );
	DSColor = new QCheckBox( tr( "&Downsample Images to:" ), CBox, "DSColor" );
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
	Options->insertTab( tabGeneral, tr( "&General" ) );

	tabFonts = new QWidget( Options, "tabFonts" );
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
	GroupFontLayout->setMargin( 11 );
	Layout4_2 = new QVBoxLayout;
	Layout4_2->setSpacing( 6 );
	Layout4_2->setMargin( 0 );
	TextFont1 = new QLabel( tr( "Available Fonts:" ), GroupFont, "TextFont1" );
	Layout4_2->addWidget( TextFont1 );
	AvailFlist = new QListBox( GroupFont, "AvailFlist" );
	QMap<QString,QFont>::Iterator it;
	for (it = DocFonts.begin(); it != DocFonts.end(); ++it)
	{
		if (AllFonts[it.key()]->HasMetrics)
			AvailFlist->insertItem(loadIcon("ok.png"), it.key());
		else
		{
			AvailFlist->insertItem(it.key());
			AvailFlist->item(AvailFlist->count()-1)->setSelectable(false);
		}
	}
	AvailFlist->setMinimumSize(QSize(150, 140));
	Layout4_2->addWidget( AvailFlist );
	GroupFontLayout->addLayout( Layout4_2 );
	Layout5_2 = new QVBoxLayout;
	Layout5_2->setAlignment( Qt::AlignTop );
	Layout5_2->setSpacing( 5 );
	Layout5_2->setMargin( 0 );
	QSpacerItem* spacerS3 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Minimum );
	Layout5_2->addItem( spacerS3 );
	ToEmbed = new QPushButton( tr( "&>>" ), GroupFont, "ToEmbed" );
	ToEmbed->setEnabled(false);
	Layout5_2->addWidget( ToEmbed );
	FromEmbed = new QPushButton( tr( "&<<" ), GroupFont, "FromEmbed" );
	FromEmbed->setEnabled(false);
	Layout5_2->addWidget( FromEmbed );
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
	Options->insertTab( tabFonts, tr( "&Fonts" ) );

	tabPresentation = new QWidget( Options, "tabPresentation" );
	tabLayout_5 = new QGridLayout( tabPresentation );
	tabLayout_5->setSpacing( 5 );
	tabLayout_5->setMargin( 11 );
	
	CheckBox10 = new QCheckBox( tr( "En&able Presentation Effects" ), tabPresentation, "CheckBox10" );
	CheckBox10->setChecked(Optionen->PresentMode);
	tabLayout_5->addMultiCellWidget( CheckBox10, 0, 0, 0, 1 );
	Pages = new QListBox( tabPresentation, "Pages" );
	Pages->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1,
	                                   Pages->sizePolicy().hasHeightForWidth() ) );
	QString tmp;
	struct PreSet ef;
	if (EffVal.count() != 0)
	{
		for (uint pg2 = 0; pg2 < vie->Pages.count(); ++pg2)
		{
			Pages->insertItem( tr("Page")+" "+tmp.setNum(pg2+1));
			if (EffVal.count()-1 < pg2)
			{
				ef.EffektLen = 1;
				ef.AnzeigeLen = 1;
				ef.Effekt = 0;
				ef.Dm = 0;
				ef.M = 0;
				ef.Di = 0;
				EffVal.append(ef);
			}
		}
	}
	else
	{
		for (uint pg = 0; pg < vie->Pages.count(); ++pg)
		{
			Pages->insertItem( tr("Page")+" "+tmp.setNum(pg+1));
			ef.EffektLen = 1;
			ef.AnzeigeLen = 1;
			ef.Effekt = 0;
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
	PageTime->setValue(EffVal[0].AnzeigeLen);
	TextLabel1e->setBuddy(PageTime);
	EffectsLayout->addWidget( PageTime, 0, 1 );
	EffectTime = new QSpinBox( Effects, "EffectTime" );
	EffectTime->setSuffix( tr( " sec" ) );
	EffectTime->setMaxValue( 3600 );
	EffectTime->setMinValue( 1 );
	EffectTime->setValue(EffVal[0].EffektLen);
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
	Options->insertTab( tabPresentation, tr( "&Presentation" ) );

	tabSecurity = new QWidget( Options, "tabSecurity" );
	tabSecurityLayout = new QVBoxLayout( tabSecurity, 11, 5, "tabSecurityLayout");
	Encry = new QCheckBox( tr( "&Use Encryption" ), tabSecurity, "Enc" );
	Encry->setChecked( Optionen->Encrypt );
	tabSecurityLayout->addWidget( Encry );
	GroupPass = new QGroupBox( tr( "Passwords" ), tabSecurity, "GroupPass" );
	GroupPass->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0,
	                                       GroupPass->sizePolicy().hasHeightForWidth() ) );
	GroupPass->setColumnLayout(0, Qt::Vertical );
	GroupPass->layout()->setSpacing( 5 );
	GroupPass->layout()->setMargin( 11 );
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
	GroupSecSet->layout()->setMargin( 11 );
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
	Options->insertTab( tabSecurity, tr( "S&ecurity" ) );

	tabColor = new QWidget( Options, "tabColor" );
	tabColorLayout = new QVBoxLayout( tabColor, 11, 5, "tabColorLayout");
	ColorGroup = new QGroupBox( tr( "General" ), tabColor, "ColorGroup" );
	ColorGroup->setColumnLayout(0, Qt::Vertical );
	ColorGroup->layout()->setSpacing( 5 );
	ColorGroup->layout()->setMargin( 11 );
	ColorGroupLayout = new QHBoxLayout( ColorGroup->layout() );
	ColorGroupLayout->setAlignment( Qt::AlignTop );
	ColorText1 = new QLabel( tr( "Output &Intended For:" ), ColorGroup, "ColorText1" );
	ColorGroupLayout->addWidget( ColorText1 );
	OutCombo = new QComboBox( true, ColorGroup, "OutCombo" );
	OutCombo->insertItem( tr( "Screen / Web" ) );
	OutCombo->insertItem( tr( "Printer" ) );
	OutCombo->setEditable(false);
	OutCombo->setCurrentItem(Optionen->UseRGB ? 0 : 1);
	ColorText1->setBuddy(OutCombo);
	ColorGroupLayout->addWidget( OutCombo );
	tabColorLayout->addWidget( ColorGroup );

	UseLPI = new QCheckBox( tr( "&Use Custom Rendering Settings" ), tabColor, "UseLPI" );
	UseLPI->setChecked(Optionen->UseLPI);
	tabColorLayout->addWidget( UseLPI );
	LPIgroup = new QGroupBox( tr( "Rendering Settings" ), tabColor, "LPIgroup" );
	LPIgroup->setColumnLayout(0, Qt::Vertical );
	LPIgroup->layout()->setSpacing( 5 );
	LPIgroup->layout()->setMargin( 11 );
	LPIgroupLayout = new QGridLayout( LPIgroup->layout() );
	LPIgroupLayout->setAlignment( Qt::AlignTop );
	LPIcolor = new QComboBox( true, LPIgroup, "LPIcolor" );
	LPIcolor->setEditable(false);
	QMap<QString,LPIset>::Iterator itlp;
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
	GroupBox9->layout()->setMargin( 11 );
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
	ProfsGroup->layout()->setMargin( 11 );
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
	if (Optionen->UseRGB)
	{
		ProfsGroup->setEnabled(false);
		GroupBox9->setEnabled(false);
	}
	EnablePr(Optionen->UseRGB ? 0 : 1);
	EnablePG();
	EnablePGI();
#ifdef HAVE_CMS

	QString tp = Optionen->SolidProf;
	if (!InputProfiles.contains(tp))
		tp = vie->Doc->CMSSettings.DefaultInputProfile2;
	ProfilesL::Iterator itp;
	for (itp = InputProfiles.begin(); itp != InputProfiles.end(); ++itp)
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
	ProfilesL::Iterator itp2;
	QString tp1 = Optionen->ImageProf;
	if (!InputProfiles.contains(tp1))
		tp1 = vie->Doc->CMSSettings.DefaultInputProfile2;
	for (itp2 = InputProfiles.begin(); itp2 != InputProfiles.end(); ++itp2)
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
	Options->insertTab( tabColor, tr( "C&olor" ) );

	tabPDFX = new QWidget( Options, "tabPDFX" );
	tabPDFXLayout = new QVBoxLayout( tabPDFX, 11, 5, "tabPDFXLayout");
	X3Group = new QGroupBox( tabPDFX, "X3Group" );
	X3Group->setTitle( tr( "PDF/X-3 Output Intent" ) );
	X3Group->setColumnLayout(0, Qt::Vertical );
	X3Group->layout()->setSpacing( 6 );
	X3Group->layout()->setMargin( 11 );
	X3GroupLayout = new QGridLayout( X3Group->layout() );
	X3GroupLayout->setAlignment( Qt::AlignTop );
	PrintProfC = new QComboBox( true, X3Group, "PrintProfC" );
	PrintProfC->setEditable(false);
#ifdef HAVE_CMS

	ProfilesL::Iterator itp3;
	QString tp3 = Optionen->PrintProf;
	if (!PDFXProfiles->contains(tp3))
		tp3 = vie->Doc->CMSSettings.DefaultPrinterProfile;
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
	BleedTop = new MSpinBox( BleedGroup, 2 );
	BleedGroupLayout->addWidget( BleedTop, 0, 2 );
	BleedBottom = new MSpinBox( BleedGroup, 2 );
	BleedGroupLayout->addWidget( BleedBottom, 2, 2 );
	BleedRight = new MSpinBox( BleedGroup, 2 );
	BleedGroupLayout->addWidget( BleedRight, 1, 3 );
	BleedLeft = new MSpinBox( BleedGroup, 2 );
	BleedGroupLayout->addWidget( BleedLeft, 1, 1 );
	QSpacerItem* spacerPX = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	BleedGroupLayout->addItem( spacerPX, 1, 0 );
	QSpacerItem* spacerPX2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	BleedGroupLayout->addItem( spacerPX2, 1, 4 );
	tabPDFXLayout->addWidget( BleedGroup );
	Options->insertTab( tabPDFX, tr( "PDF/X-&3" ) );
	QString size[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	QString EinTxt = size[Einheit];

	BleedTop->setSuffix( EinTxt );
	BleedTop->setMinValue(0);
	BleedTop->setMaxValue(view->Doc->PageH*UmReFaktor);
	BleedTop->setValue(Optionen->BleedTop*UmReFaktor);
	BleedBottom->setSuffix( EinTxt );
	BleedBottom->setMinValue(0);
	BleedBottom->setMaxValue(view->Doc->PageH*UmReFaktor);
	BleedBottom->setValue(Optionen->BleedBottom*UmReFaktor);
	BleedRight->setSuffix( EinTxt );
	BleedRight->setMinValue(0);
	BleedRight->setMaxValue(view->Doc->PageB*UmReFaktor);
	BleedRight->setValue(Optionen->BleedRight*UmReFaktor);
	BleedLeft->setSuffix( EinTxt );
	BleedLeft->setMinValue(0);
	BleedLeft->setMaxValue(view->Doc->PageB*UmReFaktor);
	BleedLeft->setValue(Optionen->BleedLeft*UmReFaktor);
/*
#ifdef HAVE_CMS
	if ((!CMSuse) || (!CMSavail))
		Options->setTabEnabled(tabPDFX, false);
	if ((CMSuse) && (CMSavail) && (Optionen->Version == 12) && (!PDFXProfiles->isEmpty()))
		EnablePDFX(2);
	else
		Options->setTabEnabled(tabPDFX, false);
#else
	Options->setTabEnabled(tabPDFX, false);
#endif
*/
#ifdef HAVE_CMS
	if ((!CMSavail) || (!CMSuse) || (PDFXProfiles->isEmpty()))
	{
		Options->setTabToolTip(tabPDFX,
			tr("<qt>Color management must be enabled to use PDF/X-3. "
			   "You can enable color management from the Settings menu.</qt>"));
		Options->setTabEnabled(tabPDFX, false);
	}
	else if (Optionen->Version == 12)
	{
		// PDF/X only possible with PDF 1.2, which has been selected so we enable
		// access to the PDF/X UI.
		// enablePDFX is usually triggered by a change in the PDF version combo box.
		// 2 is the index of PDF/X-3 in the combo box.
		EnablePDFX(2);
	}
	else
	{
		// PDF/X-3 is off since we're using another PDF version, so disable access
		// to the PDF/X UI.
		Options->setTabEnabled(tabPDFX, false);
		Options->setTabToolTip(tabPDFX,
			tr("<qt>PDF/X-3 is supported and enabled, but can not be used for "
				"the selected PDF version. If you want to use PDF/X-3, you need "
				"to set PDF/X-3 as your PDF version (compatibility level).</qt>"));
	}
#else
	Options->setTabEnabled(tabPDFX, false);
	Options->setTabToolTip(tabPDFX,
		tr("<qt>PDF/X-3 is not supported by this Scribus build (CMS support not present).</qt>"));
#endif
	BleedChanged();
	PgSel = 0;
	Pages->setCurrentItem(0);
	SetEffOpts(0);
	Pages->setEnabled(false);
	Effects->setEnabled(false);
	PagePrev->setEnabled(false);
	PDFOptsLayout->addWidget( Options );
	DoEffects();
	if (CheckBox10->isChecked())
	{
		PageTime->setValue(EffVal[0].AnzeigeLen);
		EffectTime->setValue(EffVal[0].EffektLen);
		EffectType->setCurrentItem(EffVal[0].Effekt);
		EDirection->setCurrentItem(EffVal[0].Dm);
		EDirection_2->setCurrentItem(EffVal[0].M);
		EDirection_2_2->setCurrentItem(EffVal[0].Di);
		SetEffOpts(EffectType->currentItem());
	}
	Layout7 = new QHBoxLayout;
	Layout7->setSpacing( 5 );
	Layout7->setMargin( 0 );
	QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout7->addItem( spacer_2 );
	OK = new QPushButton( tr( "&Save" ), this, "OK" );
	OK->setAutoDefault( true );
	OK->setDefault( true );
	Layout7->addWidget( OK );
	Cancel = new QPushButton( tr( "&Cancel" ), this, "Cancel" );
	Layout7->addWidget( Cancel );
	PDFOptsLayout->addLayout( Layout7 );
	resize(sizeHint());
//	setMaximumSize( sizeHint() );
	//tab order
	QWidget::setTabOrder ( AllPages, OnlySome );
	QWidget::setTabOrder ( OnlySome, PageNr );
	QWidget::setTabOrder ( PageNr, ComboBox1 );
	QWidget::setTabOrder ( ComboBox1, ComboBind );
	QWidget::setTabOrder ( ComboBind, CheckBox1 );
	QWidget::setTabOrder ( CheckBox1, Article );
	QWidget::setTabOrder ( Article, CheckBM );
	QWidget::setTabOrder ( CheckBM, Resolution );
	QWidget::setTabOrder ( Resolution, Compression );
	QWidget::setTabOrder ( Compression, CMethod );
	QWidget::setTabOrder ( CMethod, CQuality );
	QWidget::setTabOrder ( CQuality, DSColor );
	QWidget::setTabOrder ( DSColor, ValC );

	QWidget::setTabOrder ( PrintProfC, InfoString);
	QWidget::setTabOrder ( InfoString, BleedTop);
	QWidget::setTabOrder ( BleedTop, BleedLeft);
	QWidget::setTabOrder ( BleedLeft, BleedRight);
	QWidget::setTabOrder ( BleedRight, BleedBottom);

	//tooltips
	QToolTip::add( AllPages, tr( "Export all pages to PDF" ) );
	QToolTip::add( OnlySome, tr( "Export a range of pages to PDF" ) );
	QToolTip::add( PageNr, tr( "Insert a comma separated list of tokens where\n"
			              "a token can be * for all the pages, 1-5 for\n"
			              "a range of pages or a single page number.") );

	QToolTip::add( ComboBox1, "<qt>" + tr( "Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility. Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for " "commercial printing and is selectable when you have activated color management. Note: PDF/X-3 will convert all CMYK images to ICC based RGB colors. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.") + "</qt>" );
	QToolTip::add( ComboBind, tr( "Determines the binding of pages in the PDF. Unless you know\nyou need to change it leave the default choice - Left." ) );
	QToolTip::add( CheckBox1, tr( "Generates thumbnails of each page in the PDF.\nSome viewers can use the thumbnails for navigation." ) );
	QToolTip::add( Article, tr( "Generate PDF Articles, which is useful for navigating linked articles in a PDF." ) );
	QToolTip::add( CheckBM, tr( "Embed the bookmarks you created in your document.\nThese are useful for navigating long PDF documents." ) );
	QToolTip::add( Resolution, tr( "Export resolution of text and vector graphics.\nThis does not affect the resolution of bitmap images like photos." ) );
	QToolTip::add( Compression, tr( " Lossless compression of text and graphics.\nUnless you have a reason, leave this checked. This reduces PDF size." ) );
	QToolTip::add( CMethod, "<qt>" + tr( "Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options." ) + "</qt>");
	QToolTip::add( CQuality, tr( "Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)" ) );
	QToolTip::add( DSColor, tr( "Re-sample your bitmap images to the selected DPI.\nLeaving this unchecked will render them at their native resolution.\nThis can increase memory usage and slow down export." ) );
	QToolTip::add( ValC, tr( "DPI (Dots Per Inch) for image export.") );
	QToolTip::add( EmbedFonts, tr( "Embed fonts into the PDF. Embedding the fonts\nwill preserve the layout and appearance of your document." ) );
	QToolTip::add( CheckBox10, tr( "Enables presentation effects when using Acrobat Reader in full screen mode." ) );
	QToolTip::add( PagePrev, tr( "Show page previews of each page listed above." ) );
	QToolTip::add( PageTime, tr( "Length of time the page is shown before the presentation starts on the selected page." ) );
	QToolTip::add( EffectTime, tr( "Length of time the effect runs.\nA shorter time will speed up the effect, a longer one will slow it down." ) );
	QToolTip::add( EffectType, tr( "Type of the display effect." ) );
	QToolTip::add( EDirection, tr( "Direction of the effect of moving lines for the split and blind effects." ) );
	QToolTip::add( EDirection_2, tr( "Starting position for the box and split effects." ) );
	QToolTip::add( EDirection_2_2, tr( "Direction of the glitter or wipe effects." ) );
	QToolTip::add( EonAllPg, tr( "Apply the selected effect to all pages." ) );
	QToolTip::add( Encry, tr( "Enable the security features in your exported PDF.\nIf you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.\nIf you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.\nDisclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations." ) );
	QToolTip::add( PassOwner, tr( "Choose a master password which enables or disables all the\nsecurity features in your exported PDF" ) );
	QToolTip::add( PassUser, tr( "Choose a password for users to be able to read your PDF." ) );
	QToolTip::add( PrintSec, tr( "Allow printing of the PDF. If un-checked, printing is prevented. " ) );
	QToolTip::add( ModifySec, tr( "Allow modifying of the PDF. If un-checked, modifying the PDF is prevented." ) );
	QToolTip::add( CopySec, tr( "Allow copying of text or graphics from the PDF. \nIf un-checked, text and graphics cannot be copied." ) );
	QToolTip::add( AddSec, tr( "Allow adding annotations and fields to the PDF. \nIf un-checked, editing annotations and fields is prevented." ) );
	QToolTip::add( OutCombo, tr( "Color model for the output of your PDF.\nChoose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.\nChoose Printer when printing to a true 4 color CMYK printer." ) );
	QToolTip::add( UseLPI, tr( "This is an advanced setting which is not enabled by default. This should only be enabled\nwhen specifically requested by your printer and they have given you the exact details needed.\nOtherwise, your exported PDF may not print properly and is truly not portable across systems." ) );
	QToolTip::add( EmbedProfs, tr( "Embed a color profile for solid colors" ) );
	QToolTip::add( SolidPr, tr( "Color profile for solid colors" ) );
	QToolTip::add( IntendS, tr( "Rendering intent for solid colors" ) );
	QToolTip::add( EmbedProfs2, tr( "Embed a color profile for images" ) );
	QToolTip::add( NoEmbedded, tr( "Do not use color profiles that are embedded in source images" ) );
	QToolTip::add( ImageP, tr( "Color profile for images" ) );
	QToolTip::add( IntendI, tr( "Rendering intent for images" ) );
	QToolTip::add( PrintProfC, tr( "Output profile for printing. If possible, get some guidance from your printer on profile selection." ) );
	QToolTip::add( InfoString, tr( "Mandatory string for PDF/X-3 or the PDF will fail\nPDF/X-3 conformance. We recommend you use the title of the document." ) );
	QToolTip::add( BleedTop, tr( "Distance for bleed from the top of the physical page" ) );
	QToolTip::add( BleedBottom, tr( "Distance for bleed from the bottom of the physical page" ) );
	QToolTip::add( BleedLeft, tr( "Distance for bleed from the left of the physical page" ) );
	QToolTip::add( BleedRight, tr( "Distance for bleed from the right of the physical page" ) );
	QToolTip::add( MirrorH, tr( "Mirror Page(s) horizontally" ) );
	QToolTip::add( MirrorV, tr( "Mirror Page(s) vertically" ) );

	// signals and slots connections
	connect( FileC, SIGNAL( clicked() ), this, SLOT( ChangeFile() ) );
	connect( OK, SIGNAL( clicked() ), this, SLOT( DoExport() ) );
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( Datei, SIGNAL( lostFocus() ), this, SLOT( filenameChanged() ) );
	connect( Datei, SIGNAL( returnPressed() ), this, SLOT( filenameChanged() ) );
	connect(EmbedFonts, SIGNAL(clicked()), this, SLOT(EmbedAll()));
	connect(AvailFlist, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SelAFont(QListBoxItem*)));
	connect(EmbedList, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SelEFont(QListBoxItem*)));
	connect(ToEmbed, SIGNAL(clicked()), this, SLOT(PutToEmbed()));
	connect(FromEmbed, SIGNAL(clicked()), this, SLOT(RemoveEmbed()));
	connect(DSColor, SIGNAL(clicked()), this, SLOT(DoDownsample()));
	connect(PagePrev, SIGNAL(clicked()), this, SLOT(PagePr()));
	connect(Pages, SIGNAL(highlighted(int)), this, SLOT(SetPgEff(int)));
	connect(EffectType, SIGNAL(activated(int)), this, SLOT(SetEffOpts(int)));
	connect(EDirection_2_2, SIGNAL(activated(int)), this, SLOT(ValidDI(int)));
	connect(CheckBox10, SIGNAL(clicked()), this, SLOT(DoEffects()));
	connect(MirrorH, SIGNAL(clicked()), this, SLOT(PDFMirror()));
	connect(MirrorV, SIGNAL(clicked()), this, SLOT(PDFMirror()));
	connect(RotateDeg, SIGNAL(activated(int)), this, SLOT(Rotation(int)));
	connect(EonAllPg, SIGNAL(clicked()), this, SLOT(EffectOnAll()));
	connect(AllPages, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
	connect(EmbedProfs, SIGNAL(clicked()), this, SLOT(EnablePG()));
	connect(EmbedProfs2, SIGNAL(clicked()), this, SLOT(EnablePGI()));
	connect(NoEmbedded, SIGNAL(clicked()), this, SLOT(EnablePGI2()));
	connect(ComboBox1, SIGNAL(activated(int)), this, SLOT(EnablePDFX(int)));
	connect(BleedTop, SIGNAL(valueChanged(int)), this, SLOT(BleedChanged()));
	connect(BleedBottom, SIGNAL(valueChanged(int)), this, SLOT(BleedChanged()));
	connect(BleedLeft, SIGNAL(valueChanged(int)), this, SLOT(BleedChanged()));
	connect(BleedRight, SIGNAL(valueChanged(int)), this, SLOT(BleedChanged()));
	connect(Encry, SIGNAL(clicked()), this, SLOT(ToggleEncr()));
	connect(UseLPI, SIGNAL(clicked()), this, SLOT(EnableLPI2()));
	connect(LPIcolor, SIGNAL(activated(int)), this, SLOT(SelLPIcol(int)));
	connect(SubsetFonts, SIGNAL(clicked()), this, SLOT(SubsetAll()));
	connect(SubsetList, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SelSFont(QListBoxItem*)));
	connect(ToSubset, SIGNAL(clicked()), this, SLOT(PutToSubset()));
	connect(FromSubset, SIGNAL(clicked()), this, SLOT(RemoveSubset()));
	connect(InfoString, SIGNAL(lostFocus()), this, SLOT(EnablePDFX3save()));
	connect(InfoString, SIGNAL(textChanged ( const QString & )), this, SLOT(EnablePDFX3save()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
PDF_Opts::~PDF_Opts()
{
	// no need to delete child widgets, Qt does it all for us
}

void PDF_Opts::DoExport()
{
	QString fn = Datei->text();
	if (overwrite(this, fn))
	{
		EffVal[PgSel].AnzeigeLen = PageTime->value();
		EffVal[PgSel].EffektLen = EffectTime->value();
		EffVal[PgSel].Effekt = EffectType->currentItem();
		EffVal[PgSel].Dm = EDirection->currentItem();
		EffVal[PgSel].M = EDirection_2->currentItem();
		EffVal[PgSel].Di = EDirection_2_2->currentItem();
		Opts->LPISettings[SelLPIcolor].Frequency = LPIfreq->value();
		Opts->LPISettings[SelLPIcolor].Angle = LPIangle->value();
		Opts->LPISettings[SelLPIcolor].SpotFunc = LPIfunc->currentItem();
		accept();
	}
	else
		return;
}

void PDF_Opts::ToggleEncr()
{
	bool setter = Encry->isChecked() ? true : false;
	GroupSecSet->setEnabled(setter);
	GroupPass->setEnabled(setter);
}

void PDF_Opts::BleedChanged()
{
	BleedTop->setMaxValue(view->Doc->PageH*UmReFaktor-BleedBottom->value());
	BleedBottom->setMaxValue(view->Doc->PageH*UmReFaktor-BleedTop->value());
	BleedRight->setMaxValue(view->Doc->PageB*UmReFaktor-BleedLeft->value());
	BleedLeft->setMaxValue(view->Doc->PageB*UmReFaktor-BleedRight->value());
	QPixmap pm = QPixmap(70,80);
	pm.fill(white);
	QPainter p;
	p.begin(&pm);
	p.setBrush(NoBrush);
	p.setPen(black);
	p.drawRect(0, 0, pm.width(), pm.height());
	p.setPen(QPen(black, 1, DotLine, FlatCap, MiterJoin));
	int x = qRound((BleedLeft->value() / UmReFaktor) * (70.0 / view->Doc->PageB));
	int y = qRound((BleedTop->value() / UmReFaktor) * (80.0 / view->Doc->PageH));
	int w =qRound((view->Doc->PageB-((BleedLeft->value()+BleedRight->value())/UmReFaktor))*
	              (70.0/view->Doc->PageB));
	int h = qRound((view->Doc->PageH-((BleedTop->value()+BleedBottom->value())/UmReFaktor))*
	               (80.0/view->Doc->PageH));
	p.drawRect(x, y, w, h);
	BleedIcon->setPixmap(pm);
	p.end();
}

void PDF_Opts::EnablePDFX(int comboIndex)
{
	if (comboIndex != 2)
	{
		// Disable access to PDF/X settings and re-enable security settings
		Options->setTabEnabled(tabPDFX, false);
		Options->setTabToolTip(tabPDFX,
			tr("<qt>PDF/X-3 is supported and enabled, but can not be used for "
				"the selected PDF version. If you want to use PDF/X-3, you need "
				"to set PDF/X-3 as your PDF version (compatibility level).</qt>"));
		Options->setTabEnabled(tabSecurity, true);
		Options->setTabToolTip(tabSecurity, tr("PDF security settings"));
		EmbedProfs2->setEnabled(true);
		CheckBox10->setEnabled(true);
		EmbedFonts->setEnabled(true);
		OutCombo->setEnabled(true);
		OK->setEnabled(true);
		return;
	}
	// PDF/X-3 selected
	// Let user tweak PDF/X settings and disable access to security settings
	Options->setTabEnabled(tabPDFX, true);
	Options->setTabToolTip(tabPDFX, tr("PDF/X-3 settings"));
	Options->setTabEnabled(tabSecurity, false);
	Options->setTabToolTip(tabSecurity,
		tr("<qt>PDF security can not be used with PDF/X-3. If you want to turn "
			"on security, change your PDF version (compatibility level) to "
			"something other than PDF/X-3.</qt>"));
	EmbedFonts->setChecked(true);
	EmbedAll();
	CheckBox10->setChecked(false);
	disconnect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
	OutCombo->setCurrentItem(1);
	OutCombo->setEnabled(false);
	EnablePr(1);
	EmbedProfs2->setChecked(true);
	EmbedProfs2->setEnabled(false);
	CheckBox10->setEnabled(false);
	EmbedFonts->setEnabled(false);
	EnablePGI();
	if (InfoString->text() == "")
		OK->setEnabled(false);
	connect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
}

void PDF_Opts::EnablePDFX3save()
{
	if (!InfoString->text().isEmpty())
		OK->setEnabled(true);
	else
		OK->setEnabled(false);
}

void PDF_Opts::EnablePGI()
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

void PDF_Opts::EnablePGI2()
{
	bool setter = NoEmbedded->isChecked() ? true : false;
	ProfsTxt3->setEnabled(setter);
	ProfsTxt4->setEnabled(setter);
	ImageP->setEnabled(setter);
	IntendI->setEnabled(setter);
}

void PDF_Opts::EnablePG()
{
	bool setter = EmbedProfs->isChecked() ? true : false;
	ProfsTxt1->setEnabled(setter);
	ProfsTxt2->setEnabled(setter);
	SolidPr->setEnabled(setter);
	IntendS->setEnabled(setter);
}

void PDF_Opts::EnablePr(int a)
{
	EnableLPI(a);
	bool setter = a == 1 ? true : false;
	GroupBox9->setEnabled(setter);
	ProfsGroup->setEnabled(setter);
}

void PDF_Opts::EnableLPI(int a)
{
	if (a == 1)
	{
		UseLPI->show();
		if (UseLPI->isChecked())
			LPIgroup->show();
		else
			LPIgroup->hide();
	}
	else
	{
		UseLPI->hide();
		LPIgroup->hide();
	}
	tabColor->layout()->activate();
	tabColor->updateGeometry();
	adjustSize();
}

void PDF_Opts::EnableLPI2()
{
	if (UseLPI->isChecked())
		LPIgroup->show();
	else
		LPIgroup->hide();
	tabColor->layout()->activate();
	tabColor->updateGeometry();
	adjustSize();
}

void PDF_Opts::SelLPIcol(int c)
{
	Opts->LPISettings[SelLPIcolor].Frequency = LPIfreq->value();
	Opts->LPISettings[SelLPIcolor].Angle = LPIangle->value();
	Opts->LPISettings[SelLPIcolor].SpotFunc = LPIfunc->currentItem();
	LPIfreq->setValue(Opts->LPISettings[LPIcolor->text(c)].Frequency);
	LPIangle->setValue(Opts->LPISettings[LPIcolor->text(c)].Angle);
	LPIfunc->setCurrentItem(Opts->LPISettings[LPIcolor->text(c)].SpotFunc);
	SelLPIcolor = LPIcolor->text(c);
}

void PDF_Opts::SelRange(bool e)
{
	bool setter = e ? false : true;
	PageNr->setEnabled( setter );
	if (setter == false)
		CheckBM->setChecked(false);
}

void PDF_Opts::EffectOnAll()
{
	for (uint pg = 0; pg < view->Pages.count(); ++pg)
	{
		EffVal[pg].AnzeigeLen = PageTime->value();
		EffVal[pg].EffektLen = EffectTime->value();
		EffVal[pg].Effekt = EffectType->currentItem();
		EffVal[pg].Dm = EDirection->currentItem();
		EffVal[pg].M = EDirection_2->currentItem();
		EffVal[pg].Di = EDirection_2_2->currentItem();
	}
}

void PDF_Opts::PDFMirror()
{
	Opts->MirrorH = MirrorH->isOn();
	Opts->MirrorV = MirrorV->isOn();
}

void PDF_Opts::Rotation( int value )
{
	Opts->RotateDeg = value * 90; 
}

void PDF_Opts::DoEffects()
{
	bool setter = CheckBox10->isChecked() ? true : false;
	Pages->setEnabled(setter);
	Effects->setEnabled(setter);
	PagePrev->setEnabled(setter);
}

void PDF_Opts::ValidDI(int nr)
{
	if (!EDirection_2_2->listBox()->item(nr)->isSelectable())
		EDirection_2_2->setCurrentItem(0);
}

void PDF_Opts::SetPgEff(int nr)
{
	if (nr < 0)
		return;
	EffVal[PgSel].AnzeigeLen = PageTime->value();
	EffVal[PgSel].EffektLen = EffectTime->value();
	EffVal[PgSel].Effekt = EffectType->currentItem();
	EffVal[PgSel].Dm = EDirection->currentItem();
	EffVal[PgSel].M = EDirection_2->currentItem();
	EffVal[PgSel].Di = EDirection_2_2->currentItem();
	SetEffOpts(EffVal[nr].Effekt);
	PageTime->setValue(EffVal[nr].AnzeigeLen);
	EffectTime->setValue(EffVal[nr].EffektLen);
	EffectType->setCurrentItem(EffVal[nr].Effekt);
	EDirection->setCurrentItem(EffVal[nr].Dm);
	EDirection_2->setCurrentItem(EffVal[nr].M);
	EDirection_2_2->setCurrentItem(EffVal[nr].Di);
	PgSel = nr;
}

void PDF_Opts::SetEffOpts(int nr)
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

void PDF_Opts::PagePr()
{
	disconnect(Pages, SIGNAL(highlighted(int)), this, SLOT(SetPgEff(int)));
	QString tmp;
	int ci = Pages->currentItem();
	if (PagePrev->isChecked())
	{
		for (uint pg = 0; pg < view->Pages.count(); ++pg)
			Pages->changeItem(view->PageToPixmap(pg, 70), tr("Page")+" "+tmp.setNum(pg+1), pg);
	}
	else
	{
		for (uint pg = 0; pg < view->Pages.count(); ++pg)
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

void PDF_Opts::DoDownsample()
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

void PDF_Opts::RemoveEmbed()
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

void PDF_Opts::PutToEmbed()
{
	if (EmbedList->count() != 0)
	{
		if (EmbedList->findItem(AvailFlist->currentText()) == NULL)
		{
			FontsToEmbed.append(AvailFlist->currentText());
			EmbedList->insertItem(AvailFlist->currentText());
		}
	}
	else
	{
		FontsToEmbed.append(AvailFlist->currentText());
		EmbedList->insertItem(AvailFlist->currentText());
	}
}

void PDF_Opts::RemoveSubset()
{
	FontsToSubset.remove(SubsetList->currentText());
	FontsToEmbed.append(SubsetList->currentText());
	EmbedList->insertItem(SubsetList->currentText());
	SubsetList->removeItem(SubsetList->currentItem());
	SubsetList->clearSelection();
	if (SubsetList->count() == 0)
		FromSubset->setEnabled(false);
}

void PDF_Opts::PutToSubset()
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

void PDF_Opts::SelAFont(QListBoxItem *c)
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

void PDF_Opts::SelEFont(QListBoxItem *c)
{
	if ((c != NULL) && (!EmbedFonts->isChecked()))
	{
		FromEmbed->setEnabled(true);
		ToEmbed->setEnabled(false);
		ToSubset->setEnabled(true);
		FromSubset->setEnabled(false);
		AvailFlist->clearSelection();
		SubsetList->clearSelection();
	}
}

void PDF_Opts::SelSFont(QListBoxItem *c)
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

void PDF_Opts::EmbedAll()
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
				FontsToEmbed.append(AvailFlist->item(a)->text());
				EmbedList->insertItem(AvailFlist->item(a)->text());
			}
		}
	}
}

void PDF_Opts::SubsetAll()
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

void PDF_Opts::ChangeFile()
{
	QString fn;
	PrefsContext* dirs = prefsFile->getContext("dirs");
	QString wdir = dirs->get("pdf", ".");
	CustomFDialog dia(this, wdir, tr("Save as"), tr("PDF Files (*.pdf);;All Files (*)"), false, false);
	if (Datei->text() != "")
		dia.setSelection(Datei->text());
	if (dia.exec() == QDialog::Accepted)
	{
		fn = dia.selectedFile();
		dirs->set("pdf", fn.left(fn.findRev("/")));
	}
	else
		return;
	Datei->setText(fn);
}

void PDF_Opts::filenameChanged()
{
	QString currentText=Datei->text();
	if (currentText.right(1)==".")
	{
		Datei->setText(currentText+"pdf");
		return;
	}
	if (currentText.right(4).lower()!=".pdf")
		Datei->setText(currentText+".pdf");
}
