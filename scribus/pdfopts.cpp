#include "pdfopts.h"
#include "pdfopts.moc"
#include "customfdialog.h"

#if (_MSC_VER >= 1200)
 #include "win-config.h"
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

PDF_Opts::PDF_Opts( QWidget* parent,  QString Fname, QMap<QString,QFont> DocFonts, ScribusView *vie, PDFOpt *Optionen, QValueList<PreSet> Eff, ProfilesL *PDFXProfiles, SCFonts &AllFonts)
    : QDialog( parent, "pdf", true, 0 )
{
    setCaption( tr( "Create PDF-File" ) );
  	setIcon(loadIcon("AppIcon.png"));
  	FontsToEmbed.clear();
  	view = vie;
  	EffVal = Eff;
	Einheit = view->Doc->Einheit;
  	PDFOptsLayout = new QVBoxLayout( this );
    PDFOptsLayout->setSpacing( 6 );
    PDFOptsLayout->setMargin( 11 );
    Name = new QLabel( this, "Name" );
    Name->setFrameShape( QLabel::NoFrame );
    Name->setText( tr( "Output to File:" ) );
    PDFOptsLayout->addWidget( Name );
    Layout5 = new QHBoxLayout;
    Layout5->setSpacing( 6 );
    Layout5->setMargin( 0 );
    Datei = new QLineEdit( this, "Datei" );
    Datei->setMinimumSize( QSize( 268, 22 ) );
	if (Optionen->Datei != "")
		Datei->setText(Optionen->Datei);
	else
	{
    	QFileInfo fi = QFileInfo(Fname);
    	Datei->setText(fi.dirPath()+"/"+fi.baseName()+".pdf");
	}
    Layout5->addWidget( Datei );
    FileC = new QToolButton( this, "FileC" );
    FileC->setMinimumSize( QSize( 88, 24 ) );
    FileC->setText( tr( "Change..." ) );
    Layout5->addWidget( FileC );
    PDFOptsLayout->addLayout( Layout5 );
    Options = new QTabWidget( this, "TabWidget2" );
    tab = new QWidget( Options, "tab" );
    tabLayout = new QVBoxLayout( tab );
    tabLayout->setSpacing( 6 );
    tabLayout->setMargin( 11 );

    Layout13 = new QHBoxLayout( 0, 0, 6, "Layout13");

    RangeGroup = new QButtonGroup( tab, "RangeGroup" );
    RangeGroup->setTitle( tr( "Export Range" ) );
    RangeGroup->setColumnLayout(0, Qt::Vertical );
    RangeGroup->layout()->setSpacing( 6 );
    RangeGroup->layout()->setMargin( 11 );
    RangeGroupLayout = new QVBoxLayout( RangeGroup->layout() );
    RangeGroupLayout->setAlignment( Qt::AlignTop );
    AllPages = new QRadioButton( RangeGroup, "AllPages" );
    AllPages->setText( tr( "All Pages" ) );
    AllPages->setChecked( true );
    RangeGroupLayout->addWidget( AllPages );
    Layout11 = new QGridLayout( 0, 1, 1, 0, 6, "Layout11");
    OnlySome = new QRadioButton( RangeGroup, "OnlySome" );
    OnlySome->setText( tr( "From:" ) );
    Layout11->addWidget( OnlySome, 0, 0 );
    LastPage = new QSpinBox( RangeGroup, "LastPage" );
    LastPage->setMaxValue(view->Pages.count());
    LastPage->setMinValue( 1 );
    Layout11->addWidget( LastPage, 1, 1 );
    RText = new QLabel( RangeGroup, "RText" );
    RText->setText( tr( "to:" ) );
    Layout11->addWidget( RText, 1, 0, AlignRight );
    FirstPage = new QSpinBox( RangeGroup, "FirstPage" );
    FirstPage->setMaxValue(view->Pages.count());
    FirstPage->setMinValue( 1 );
	FirstPage->setValue(1);
	LastPage->setValue(view->Pages.count());
    Layout11->addWidget( FirstPage, 0, 1 );
    RangeGroupLayout->addLayout( Layout11 );
	FirstPage->setEnabled(false);
	LastPage->setEnabled(false);
	RText->setEnabled(false);
    Layout13->addWidget( RangeGroup );

    GroupBox1 = new QGroupBox( tab, "GroupBox1" );
    GroupBox1->setTitle( tr( "File Options" ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 0 );
    GroupBox1->layout()->setMargin( 0 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );
    GroupBox1Layout->setSpacing( 6 );
    GroupBox1Layout->setMargin( 11 );
    TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );
    TextLabel1->setText( tr( "Compatibility:" ) );
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
    TextLabel1x = new QLabel( GroupBox1, "TextLabel1" );
    TextLabel1x->setText( tr( "Binding:" ) );
    TextLabel1x->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
    GroupBox1Layout->addWidget( TextLabel1x, 1, 0 );
    ComboBind = new QComboBox( true, GroupBox1, "ComboBind" );
    ComboBind->insertItem( tr("Left Margin"));
    ComboBind->insertItem( tr("Right Margin"));
    ComboBind->setEditable(false);
	ComboBind->setCurrentItem(Optionen->Binding);
    GroupBox1Layout->addMultiCellWidget( ComboBind, 1, 1, 1, 2, AlignLeft );
    CheckBox1 = new QCheckBox( GroupBox1, "CheckBox1" );
    CheckBox1->setText( tr( "Generate Thumbnails" ) );
	CheckBox1->setChecked(Optionen->Thumbnails);
    GroupBox1Layout->addMultiCellWidget( CheckBox1, 2, 2, 0, 2 );
    Article = new QCheckBox( GroupBox1, "CheckBox1" );
    Article->setText( tr( "Save linked Text Frames as PDF-Articles" ) );
	Article->setChecked(Optionen->Articles);
    GroupBox1Layout->addMultiCellWidget( Article, 3, 3, 0, 2 );
    CheckBM = new QCheckBox( GroupBox1, "E" );
    CheckBM->setText( tr( "Include Bookmarks" ) );
	CheckBM->setChecked(Optionen->Bookmarks);
    GroupBox1Layout->addMultiCellWidget( CheckBM, 4, 4, 0, 2 );
    TextLabel2 = new QLabel( GroupBox1, "TextLabel2" );
    TextLabel2->setText( tr( "Resolution:" ) );
    TextLabel2->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
    GroupBox1Layout->addWidget( TextLabel2, 5, 0 );
    Resolution = new QSpinBox( GroupBox1, "Resolution" );
    Resolution->setMaxValue( 4000 );
    Resolution->setMinValue( 35 );
    Resolution->setValue(Optionen->Resolution);
    Resolution->setSuffix( tr( " dpi" ) );
    GroupBox1Layout->addWidget( Resolution, 5, 1, AlignLeft );
    Layout13->addWidget( GroupBox1 );
    tabLayout->addLayout( Layout13 );
    GroupBox2 = new QGroupBox( tab, "GroupBox2" );
    GroupBox2->setTitle( tr( "Compression" ) );
    GroupBox2->setColumnLayout(0, Qt::Vertical );
    GroupBox2->layout()->setSpacing( 0 );
    GroupBox2->layout()->setMargin( 0 );
    GroupBox2Layout = new QGridLayout( GroupBox2->layout() );
    GroupBox2Layout->setAlignment( Qt::AlignTop );
    GroupBox2Layout->setSpacing( 6 );
    GroupBox2Layout->setMargin( 11 );
    DSColor = new QCheckBox( GroupBox2, "DSColor" );
    DSColor->setText( tr( "Downsample Images to:" ) );
	DSColor->setChecked(Optionen->RecalcPic);
    GroupBox2Layout->addWidget( DSColor, 1, 0 );
    ValC = new QSpinBox( GroupBox2, "ValC" );
    ValC->setSuffix( tr( " dpi" ) );
    ValC->setMaxValue( 4000 );
    ValC->setMinValue( 35 );
    ValC->setValue(Optionen->PicRes);
	ValC->setEnabled(DSColor->isChecked() ? true : false);
    GroupBox2Layout->addWidget( ValC, 1, 1, AlignLeft );
    Compression = new QCheckBox( GroupBox2, "Compression" );
    Compression->setText( tr( "Compress Text and Vector Graphics" ) );
    Compression->setChecked(Optionen->Compress);
    GroupBox2Layout->addMultiCellWidget( Compression, 0, 0, 0, 1 );
    tabLayout->addWidget( GroupBox2 );
    Options->insertTab( tab, tr( "General" ) );
    tab_3 = new QWidget( Options, "tab_3" );
    tabLayout_3 = new QVBoxLayout( tab_3 );
    tabLayout_3->setSpacing( 6 );
    tabLayout_3->setMargin( 11 );
    EmbedFonts = new QCheckBox( tab_3, "EmbedFonts" );
    EmbedFonts->setText( tr( "Embed all Fonts" ) );
    tabLayout_3->addWidget( EmbedFonts );
    GroupFont = new QGroupBox( tab_3, "GroupFont" );
    GroupFont->setTitle( tr( "Embedding" ) );
    GroupFont->setColumnLayout(0, Qt::Vertical );
    GroupFont->layout()->setSpacing( 0 );
    GroupFont->layout()->setMargin( 0 );
    GroupFontLayout = new QHBoxLayout( GroupFont->layout() );
    GroupFontLayout->setAlignment( Qt::AlignTop );
    GroupFontLayout->setSpacing( 6 );
    GroupFontLayout->setMargin( 11 );
    Layout4_2 = new QVBoxLayout;
    Layout4_2->setSpacing( 6 );
    Layout4_2->setMargin( 0 );
    TextFont1 = new QLabel( GroupFont, "TextFont1" );
    TextFont1->setText( tr( "Available Fonts:" ) );
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
    AvailFlist->setMaximumSize(QSize(150, 140));
    Layout4_2->addWidget( AvailFlist );
   	GroupFontLayout->addLayout( Layout4_2 );
    Layout5_2 = new QVBoxLayout;
    Layout5_2->setSpacing( 6 );
    Layout5_2->setMargin( 0 );
    ToEmbed = new QPushButton( GroupFont, "ToEmbed" );
    ToEmbed->setText( tr( ">>" ) );
    ToEmbed->setEnabled(false);
    Layout5_2->addWidget( ToEmbed );
    FromEmbed = new QPushButton( GroupFont, "FromEmbed" );
    FromEmbed->setText( tr( "<<" ) );
    FromEmbed->setEnabled(false);
    Layout5_2->addWidget( FromEmbed );
    GroupFontLayout->addLayout( Layout5_2 );
    Layout6 = new QVBoxLayout;
    Layout6->setSpacing( 6 );
    Layout6->setMargin( 0 );
    TextFont1_2 = new QLabel( GroupFont, "TextFont1_2" );
    TextFont1_2->setText( tr( "Fonts to embed:" ) );
    Layout6->addWidget( TextFont1_2 );
    EmbedList = new QListBox( GroupFont, "EmbedList" );
    EmbedList->setMinimumSize(QSize(150, 140));
    EmbedList->setMaximumSize(QSize(150, 140));
	if (Optionen->EmbedList.count() != 0)
	{
		for (uint efo = 0; efo < Optionen->EmbedList.count(); ++efo)
		{
			EmbedList->insertItem(Optionen->EmbedList[efo]);
			FontsToEmbed.append(Optionen->EmbedList[efo]);
		}
	}
    Layout6->addWidget( EmbedList );
    GroupFontLayout->addLayout( Layout6 );
    tabLayout_3->addWidget( GroupFont );
    Options->insertTab( tab_3, tr( "Fonts" ) );
   	tab_5 = new QWidget( Options, "tab_5" );
    tabLayout_5 = new QGridLayout( tab_5 );
    tabLayout_5->setSpacing( 6 );
    tabLayout_5->setMargin( 11 );
    CheckBox10 = new QCheckBox( tab_5, "CheckBox10" );
    CheckBox10->setText( tr( "Enable Presentation Effects" ) );
    CheckBox10->setChecked(Optionen->PresentMode);
	tabLayout_5->addMultiCellWidget( CheckBox10, 0, 0, 0, 1 );
   	Pages = new QListBox( tab_5, "Pages" );
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
    PagePrev = new QCheckBox( tab_5, "CheckBox10" );
    PagePrev->setText( tr( "Show Page Previews" ) );
    PagePrev->setChecked(false);
	tabLayout_5->addWidget( PagePrev, 2, 0 );
    Effects = new QGroupBox( tab_5, "Effects" );
    Effects->setTitle( tr( "Effects" ) );
    Effects->setColumnLayout(0, Qt::Vertical );
    Effects->layout()->setSpacing( 0 );
    Effects->layout()->setMargin( 0 );
    EffectsLayout = new QGridLayout( Effects->layout() );
    EffectsLayout->setAlignment( Qt::AlignTop );
    EffectsLayout->setSpacing( 6 );
    EffectsLayout->setMargin( 11 );
    TextLabel1e = new QLabel( Effects, "TextLabel1_2" );
    TextLabel1e->setText( tr( "Display Duration:" ) );
    EffectsLayout->addWidget( TextLabel1e, 0, 0 );
    TextLabel2e = new QLabel( Effects, "TextLabel2_2" );
    TextLabel2e->setText( tr( "Effect Duration:" ) );
	EffectsLayout->addWidget( TextLabel2e, 1, 0 );
    TextLabel3e = new QLabel( Effects, "TextLabel3_2" );
    TextLabel3e->setText( tr( "Effect Type:" ) );
	EffectsLayout->addWidget( TextLabel3e, 2, 0 );
    TextLabel4e = new QLabel( Effects, "TextLabel4_2" );
    TextLabel4e->setText( tr( "Moving Lines:" ) );
	EffectsLayout->addWidget( TextLabel4e, 3, 0 );
    TextLabel5e = new QLabel( Effects, "TextLabel6" );
    TextLabel5e->setText( tr( "from the:" ) );
    EffectsLayout->addWidget( TextLabel5e, 4, 0 );
	TextLabel6e = new QLabel( Effects, "TextLabel5" );
    TextLabel6e->setText( tr( "Direction:" ) );
	EffectsLayout->addWidget( TextLabel6e, 5, 0 );
    PageTime = new QSpinBox( Effects, "PageTime" );
    PageTime->setSuffix( tr( " sec" ) );
    PageTime->setMaxValue( 3600 );
    PageTime->setMinValue( 1 );
    PageTime->setValue(EffVal[0].AnzeigeLen);
    EffectsLayout->addWidget( PageTime, 0, 1 );
    EffectTime = new QSpinBox( Effects, "EffectTime" );
    EffectTime->setSuffix( tr( " sec" ) );
    EffectTime->setMaxValue( 3600 );
    EffectTime->setMinValue( 1 );
    EffectTime->setValue(EffVal[0].EffektLen);
    EffectsLayout->addWidget( EffectTime, 1, 1 );
    EffectType = new QComboBox( true, Effects, "EffectType" );
	QString tmpc[] = { tr("No Effect"), tr("Blinds"), tr("Box"), tr("Dissolve"), tr("Glitter"), tr("Split"), tr("Wipe")};
	size_t ar = sizeof(tmpc) / sizeof(*tmpc);
	for (uint a = 0; a < ar; ++a)
		EffectType->insertItem(tmpc[a]);
    EffectType->setEditable(false);
    EffectsLayout->addWidget( EffectType, 2, 1 );
    EDirection = new QComboBox( true, Effects, "EDirection" );
    EDirection->insertItem( tr( "Horizontal" ) );
    EDirection->insertItem( tr( "Vertical" ) );
    EDirection->setEditable(false);
    EffectsLayout->addWidget( EDirection, 3, 1 );
    EDirection_2 = new QComboBox( true, Effects, "EDirection_2" );
    EDirection_2->insertItem( tr( "Inside" ) );
    EDirection_2->insertItem( tr( "Outside" ) );
    EDirection_2->setEditable(false);
    EffectsLayout->addWidget( EDirection_2, 4, 1 );
    EDirection_2_2 = new QComboBox( true, Effects, "EDirection_2_2" );
	QString tmp_ed[] = { tr("Left to Right"), tr("Top to Bottom"), tr("Bottom to Top"), tr("Right to Left"), 
						tr("Top-left to Bottom-Right")};
	size_t ar_ed = sizeof(tmp_ed) / sizeof(*tmp_ed);
	for (uint a = 0; a < ar_ed; ++a)
		EDirection_2_2->insertItem(tmp_ed[a]);
    EDirection_2_2->setEditable(false);
	EffectsLayout->addWidget( EDirection_2_2, 5, 1 );
	EonAllPg = new QPushButton( Effects, "Eon" );
    EonAllPg->setText( tr( "Apply Effect on all Pages" ) );
	EffectsLayout->addMultiCellWidget( EonAllPg, 6, 6, 0, 1 );
    tabLayout_5->addMultiCellWidget( Effects, 1, 2, 1, 1 );
    Options->insertTab( tab_5, tr( "Extras" ) );

    tabsec = new QWidget( Options, "tabsec" );
    tabsecLayout = new QVBoxLayout( tabsec, 11, 6, "tabsecLayout");
    Encry = new QCheckBox( tabsec, "Enc" );
    Encry->setText( tr( "Use Encryption" ) );
    Encry->setChecked( Optionen->Encrypt );
    tabsecLayout->addWidget( Encry );
    GroupPass = new QGroupBox( tabsec, "GroupPass" );
    GroupPass->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0,
											GroupPass->sizePolicy().hasHeightForWidth() ) );
    GroupPass->setTitle( tr( "Passwords" ) );
    GroupPass->setColumnLayout(0, Qt::Vertical );
    GroupPass->layout()->setSpacing( 6 );
    GroupPass->layout()->setMargin( 11 );
    GroupPassLayout = new QGridLayout( GroupPass->layout() );
    GroupPassLayout->setAlignment( Qt::AlignTop );
    TextSec2 = new QLabel( GroupPass, "TextSec2" );
    TextSec2->setText( tr( "User:" ) );
    GroupPassLayout->addWidget( TextSec2, 1, 0 );
    TextSec1 = new QLabel( GroupPass, "TextSec1" );
    TextSec1->setText( tr( "Owner:" ) );
    GroupPassLayout->addWidget( TextSec1, 0, 0 );
    PassOwner = new QLineEdit( GroupPass, "PassOwner" );
    PassOwner->setEchoMode( QLineEdit::Password );
	PassOwner->setText(Optionen->PassOwner);
    GroupPassLayout->addWidget( PassOwner, 0, 1 );
    PassUser = new QLineEdit( GroupPass, "PassUser" );
    PassUser->setEchoMode( QLineEdit::Password );
	PassUser->setText(Optionen->PassUser);
    GroupPassLayout->addWidget( PassUser, 1, 1 );
    tabsecLayout->addWidget( GroupPass );

    GroupSecSet = new QGroupBox( tabsec, "GroupSecSet" );
    GroupSecSet->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0,
												GroupSecSet->sizePolicy().hasHeightForWidth() ) );
    GroupSecSet->setTitle( tr( "Settings" ) );
    GroupSecSet->setColumnLayout(0, Qt::Vertical );
    GroupSecSet->layout()->setSpacing( 6 );
    GroupSecSet->layout()->setMargin( 11 );
    GroupSecSetLayout = new QVBoxLayout( GroupSecSet->layout() );
    GroupSecSetLayout->setAlignment( Qt::AlignTop );
    PrintSec = new QCheckBox( GroupSecSet, "PrintSec" );
    PrintSec->setText( tr( "Allow Printing the Document" ) );
    PrintSec->setChecked( Optionen->Permissions & 4 );
    GroupSecSetLayout->addWidget( PrintSec );
    ModifySec = new QCheckBox( GroupSecSet, "ModifySec" );
    ModifySec->setText( tr( "Allow Changing the Document" ) );
    ModifySec->setChecked( Optionen->Permissions & 8 );
    GroupSecSetLayout->addWidget( ModifySec );
    CopySec = new QCheckBox( GroupSecSet, "CopySec" );
    CopySec->setText( tr( "Copying Text and Graphics is allowed" ) );
    CopySec->setChecked( Optionen->Permissions & 16 );
    GroupSecSetLayout->addWidget( CopySec );
    AddSec = new QCheckBox( GroupSecSet, "AddSec" );
    AddSec->setText( tr( "Adding Annotations and Fields is allowed" ) );
    AddSec->setChecked( Optionen->Permissions & 32 );
    GroupSecSetLayout->addWidget( AddSec );
    tabsecLayout->addWidget( GroupSecSet );
	if (!Encry->isChecked())
	{
		GroupSecSet->setEnabled(false);
		GroupPass->setEnabled(false);
	}
    Options->insertTab( tabsec, tr( "Security" ) );

    tabcolor = new QWidget( Options, "tabcolor" );
    tabcolorLayout = new QVBoxLayout( tabcolor, 11, 6, "tabcolorLayout");
    ColorGroup = new QGroupBox( tabcolor, "ColorGroup" );
    ColorGroup->setTitle( tr( "General" ) );
    ColorGroup->setColumnLayout(0, Qt::Vertical );
    ColorGroup->layout()->setSpacing( 6 );
    ColorGroup->layout()->setMargin( 11 );
    ColorGroupLayout = new QHBoxLayout( ColorGroup->layout() );
    ColorGroupLayout->setAlignment( Qt::AlignTop );
    ColorText1 = new QLabel( ColorGroup, "ColorText1" );
    ColorText1->setText( tr( "Output intended for:" ) );
    ColorGroupLayout->addWidget( ColorText1 );
    OutCombo = new QComboBox( true, ColorGroup, "OutCombo" );
    OutCombo->insertItem( tr( "Screen / Web" ) );
    OutCombo->insertItem( tr( "Printer" ) );
	OutCombo->setEditable(false);
	OutCombo->setCurrentItem(Optionen->UseRGB ? 0 : 1);
    ColorGroupLayout->addWidget( OutCombo );
    tabcolorLayout->addWidget( ColorGroup );

    GroupBox9 = new QGroupBox( tabcolor, "GroupBox9" );
    GroupBox9->setTitle( tr( "Solid Colors:" ) );
    GroupBox9->setColumnLayout(0, Qt::Vertical );
    GroupBox9->layout()->setSpacing( 6 );
    GroupBox9->layout()->setMargin( 11 );
    GroupBox9Layout = new QGridLayout( GroupBox9->layout() );
    GroupBox9Layout->setAlignment( Qt::AlignTop );
    EmbedProfs = new QCheckBox( GroupBox9, "EmbedProfs" );
    EmbedProfs->setText( tr( "Use ICC-Profile" ) );
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
    tabcolorLayout->addWidget( GroupBox9 );
    ProfsGroup = new QGroupBox( tabcolor, "ProfsGroup" );
    ProfsGroup->setTitle( tr( "Images:" ) );
    ProfsGroup->setColumnLayout(0, Qt::Vertical );
    ProfsGroup->layout()->setSpacing( 6 );
    ProfsGroup->layout()->setMargin( 11 );
    ProfsGroupLayout = new QGridLayout( ProfsGroup->layout() );
    ProfsGroupLayout->setAlignment( Qt::AlignTop );
    EmbedProfs2 = new QCheckBox( ProfsGroup, "EmbedProfs" );
    EmbedProfs2->setText( tr( "Use ICC-Profile" ) );
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
    tabcolorLayout->addWidget( ProfsGroup );
	if (Optionen->UseRGB)
	{
		ProfsGroup->setEnabled(false);
		GroupBox9->setEnabled(false);
	}
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
    tabcolorLayout->addItem( spacerCG );
    Options->insertTab( tabcolor, tr( "Color" ) );

    tabpdfx = new QWidget( Options, "tabpdfx" );
    tabpdfxLayout = new QVBoxLayout( tabpdfx, 11, 6, "tabpdfxLayout");
    X3Group = new QGroupBox( tabpdfx, "X3Group" );
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
    PDFX2 = new QLabel( X3Group, "PDFX2" );
    PDFX2->setText( tr( "Info String:" ) );
    X3GroupLayout->addWidget( PDFX2, 1, 0 );
    PDFX1 = new QLabel( X3Group, "PDFX1" );
    PDFX1->setText( tr( "Output Profile:" ) );
    X3GroupLayout->addWidget( PDFX1, 0, 0 );
    tabpdfxLayout->addWidget( X3Group );
    BleedGroup = new QGroupBox( tabpdfx, "BleedGroup" );
    BleedGroup->setTitle( tr( "Trim Box" ) );
    BleedGroup->setColumnLayout(0, Qt::Vertical );
    BleedGroup->layout()->setSpacing( 6 );
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
    tabpdfxLayout->addWidget( BleedGroup );
    Options->insertTab( tabpdfx, tr( "PDF/X-3" ) );
	char *size[] = {" pt", " mm", " in", " p"};
	QString EinTxt = tr(size[Einheit]);

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
#ifdef HAVE_CMS
	if ((!CMSuse) || (!CMSavail))
		Options->setTabEnabled(tabpdfx, false);
	if ((CMSuse) && (CMSavail) && (Optionen->Version == 12) && (!PDFXProfiles->isEmpty()))
		EnablePDFX(2);
	else
		Options->setTabEnabled(tabpdfx, false);
#else
	Options->setTabEnabled(tabpdfx, false);
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
    Layout7->setSpacing( 6 );
    Layout7->setMargin( 0 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout7->addItem( spacer_2 );
    OK = new QPushButton( this, "OK" );
    OK->setText( tr( "OK" ) );
    OK->setAutoDefault( true );
    OK->setDefault( true );
    Layout7->addWidget( OK );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout7->addItem( spacer_3 );
    Cancel = new QPushButton( this, "Cancel" );
    Cancel->setText( tr( "Cancel" ) );
    Layout7->addWidget( Cancel );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout7->addItem( spacer_4 );
    PDFOptsLayout->addLayout( Layout7 );
    setMaximumSize( sizeHint() );

    // signals and slots connections
    connect( FileC, SIGNAL( clicked() ), this, SLOT( ChangeFile() ) );
    connect( OK, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
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
    connect(EonAllPg, SIGNAL(clicked()), this, SLOT(EffectOnAll()));
    connect(AllPages, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
    connect(FirstPage, SIGNAL(valueChanged(int)), this, SLOT(ChFrom()));
    connect(LastPage, SIGNAL(valueChanged(int)), this, SLOT(ChTo()));
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
}

/*  
 *  Destroys the object and frees any allocated resources
 */
PDF_Opts::~PDF_Opts()
{
    // no need to delete child widgets, Qt does it all for us
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

void PDF_Opts::EnablePDFX(int a)
{
	if (a != 2)
	{
		Options->setTabEnabled(tabpdfx, false);
		Options->setTabEnabled(tabsec, true);
		EmbedProfs2->setEnabled(true);
		CheckBox10->setEnabled(true);
		EmbedFonts->setEnabled(true);
		OutCombo->setEnabled(true);
		return;
	}
	EmbedFonts->setChecked(true);
	EmbedAll();
	CheckBox10->setChecked(false);
	OutCombo->setCurrentItem(1);
	OutCombo->setEnabled(false);
	EnablePr(1);
	EmbedProfs2->setChecked(true);
	EmbedProfs2->setEnabled(false);
	CheckBox10->setEnabled(false);
	EmbedFonts->setEnabled(false);
	EnablePGI();
	Options->setTabEnabled(tabpdfx, true);
	Options->setTabEnabled(tabsec, false);
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
	bool setter = a == 1 ? true : false;
	GroupBox9->setEnabled(setter);
	ProfsGroup->setEnabled(setter);
}

void PDF_Opts::ChFrom()
{
	disconnect(LastPage, SIGNAL(valueChanged(int)), this, SLOT(ChTo()));
	if (FirstPage->value() > LastPage->value())
		LastPage->setValue(FirstPage->value());
	connect(LastPage, SIGNAL(valueChanged(int)), this, SLOT(ChTo()));
}

void PDF_Opts::ChTo()
{
	disconnect(FirstPage, SIGNAL(valueChanged(int)), this, SLOT(ChFrom()));
	if (LastPage->value() < FirstPage->value())
		FirstPage->setValue(LastPage->value());
	connect(FirstPage, SIGNAL(valueChanged(int)), this, SLOT(ChFrom()));
}

void PDF_Opts::SelRange(bool e)
{
	bool setter = e ? false : true;

	RText->setEnabled( setter );
	FirstPage->setEnabled( setter );
	LastPage->setEnabled( setter );
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
		FromEmbed->setEnabled(false);
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

void PDF_Opts::SelAFont(QListBoxItem *c)
{
	if ((c != NULL) && (!EmbedFonts->isChecked()))
	{
		FromEmbed->setEnabled(false);
		if (c->isSelectable())
  			ToEmbed->setEnabled(true);
  		EmbedList->clearSelection();
  	}
}

void PDF_Opts::SelEFont(QListBoxItem *c)
{
	if ((c != NULL) && (!EmbedFonts->isChecked()))
	{
		FromEmbed->setEnabled(true);
  		ToEmbed->setEnabled(false);
  		AvailFlist->clearSelection();
  	}
}

void PDF_Opts::EmbedAll()
{
	if (EmbedFonts->isChecked())
	{
		EmbedList->clear();
		FontsToEmbed.clear();
		FromEmbed->setEnabled(false);
  		ToEmbed->setEnabled(false);
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

void PDF_Opts::ChangeFile()
{
	QString fn;
	CustomFDialog dia(this, tr("Save as"), tr("PDF-Files (*.pdf);;All Files (*)"), false, false);
	if (Datei->text() != "")
		dia.setSelection(Datei->text());
	if (dia.exec() == QDialog::Accepted)
		fn = dia.selectedFile();
	else
		return;
	if (!fn.isEmpty())
	{
		if (overwrite(this, fn))
  			Datei->setText(fn);
	}
}
