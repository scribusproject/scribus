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
	Opts = Optionen;
	Einheit = view->Doc->Einheit;
	PDFOptsLayout = new QVBoxLayout( this );
	PDFOptsLayout->setSpacing( 5 );
	PDFOptsLayout->setMargin( 11 );
	Name = new QLabel( this, "Name" );
	Name->setFrameShape( QLabel::NoFrame );
	Name->setText( tr( "Output to File:" ) );
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
	tabLayout->setSpacing( 5 );
	tabLayout->setMargin( 11 );

	Layout13 = new QHBoxLayout( 0, 0, 5, "Layout13");

	RangeGroup = new QButtonGroup( tab, "RangeGroup" );
	RangeGroup->setTitle( tr( "Export Range" ) );
	RangeGroup->setColumnLayout(0, Qt::Vertical );
	RangeGroup->layout()->setSpacing( 5 );
	RangeGroup->layout()->setMargin( 11 );
	RangeGroupLayout = new QVBoxLayout( RangeGroup->layout() );
	RangeGroupLayout->setAlignment( Qt::AlignTop );
	AllPages = new QRadioButton( RangeGroup, "AllPages" );
	AllPages->setText( tr( "All Pages" ) );
	AllPages->setChecked( true );
	RangeGroupLayout->addWidget( AllPages );
	Layout11 = new QGridLayout( 0, 1, 1, 0, 5, "Layout11");
	OnlySome = new QRadioButton( RangeGroup, "OnlySome" );
	OnlySome->setText( tr( "Choose Pages" ) );
	Layout11->addWidget( OnlySome, 0, 0 );
	PageNr = new QLineEdit( RangeGroup, "PageNr" );
	PageNr->setEnabled(false);
	QToolTip::add
		( PageNr, tr( "Insert a comma separated list of tokens where\n"
		              "a token can be * for all the pages, 1-5 for\n"
		              "a range of pages or a single page number.") );
	Layout11->addWidget( PageNr, 1, 0 );
	RangeGroupLayout->addLayout( Layout11 );
	Layout13->addWidget( RangeGroup );

	GroupBox1 = new QGroupBox( tab, "GroupBox1" );
	GroupBox1->setTitle( tr( "File Options" ) );
	GroupBox1->setColumnLayout(0, Qt::Vertical );
	GroupBox1->layout()->setSpacing( 0 );
	GroupBox1->layout()->setMargin( 0 );
	GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
	GroupBox1Layout->setAlignment( Qt::AlignTop );
	GroupBox1Layout->setSpacing( 5 );
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
	Compression = new QCheckBox( tab, "Compression" );
	Compression->setChecked( Optionen->Compress );
	Compression->setText( tr( "Compress Text and Vector Graphics" ) );
	tabLayout->addWidget( Compression );
	CBox = new QGroupBox( tab, "CBox" );
	CBox->setTitle( tr( "Image Settings" ) );
	CBox->setColumnLayout(0, Qt::Vertical );
	CBox->layout()->setSpacing( 5 );
	CBox->layout()->setMargin( 11 );
	CBoxLayout = new QGridLayout( CBox->layout() );
	CBoxLayout->setAlignment( Qt::AlignTop );
	TextCom1 = new QLabel( CBox, "TextCom1" );
	TextCom1->setText( tr( "Method:" ) );
	CBoxLayout->addWidget( TextCom1, 0, 0 );
	CMethod = new QComboBox( true, CBox, "CMethod" );
	CMethod->clear();
	CMethod->insertItem( tr( "Automatic" ) );
	CMethod->insertItem( tr( "JPEG" ) );
	CMethod->insertItem( tr( "Zip" ) );
	CMethod->insertItem( tr( "None" ) );
	CMethod->setEditable(false);
	CMethod->setCurrentItem(Optionen->CompressMethod);
	CBoxLayout->addWidget( CMethod, 0, 1 );
	TextCom2 = new QLabel( CBox, "TextCom2" );
	TextCom2->setText( tr( "Quality:" ) );
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
	CBoxLayout->addWidget( CQuality, 1, 1 );
	DSColor = new QCheckBox( CBox, "DSColor" );
	DSColor->setText( tr( "Downsample Images to:" ) );
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
	Options->insertTab( tab, tr( "&General" ) );

	tab_3 = new QWidget( Options, "tab_3" );
	tabLayout_3 = new QVBoxLayout( tab_3 );
	tabLayout_3->setSpacing( 5 );
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
	GroupFontLayout->setSpacing( 5 );
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
	Layout4_2->addWidget( AvailFlist );
	GroupFontLayout->addLayout( Layout4_2 );
	Layout5_2 = new QVBoxLayout;
	Layout5_2->setSpacing( 5 );
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
	Layout6->setSpacing( 5 );
	Layout6->setMargin( 0 );
	TextFont1_2 = new QLabel( GroupFont, "TextFont1_2" );
	TextFont1_2->setText( tr( "Fonts to embed:" ) );
	Layout6->addWidget( TextFont1_2 );
	EmbedList = new QListBox( GroupFont, "EmbedList" );
	EmbedList->setMinimumSize(QSize(150, 140));
	EmbedFonts->setChecked(true);
	EmbedAll();
	Layout6->addWidget( EmbedList );
	GroupFontLayout->addLayout( Layout6 );
	tabLayout_3->addWidget( GroupFont );
	Options->insertTab( tab_3, tr( "&Fonts" ) );
	tab_5 = new QWidget( Options, "tab_5" );
	tabLayout_5 = new QGridLayout( tab_5 );
	tabLayout_5->setSpacing( 5 );
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
	EffectsLayout->setSpacing( 5 );
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
	TextLabel5e->setText( tr( "From the:" ) );
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
	Options->insertTab( tab_5, tr( "E&xtras" ) );

	tabsec = new QWidget( Options, "tabsec" );
	tabsecLayout = new QVBoxLayout( tabsec, 11, 5, "tabsecLayout");
	Encry = new QCheckBox( tabsec, "Enc" );
	Encry->setText( tr( "Use Encryption" ) );
	Encry->setChecked( Optionen->Encrypt );
	tabsecLayout->addWidget( Encry );
	GroupPass = new QGroupBox( tabsec, "GroupPass" );
	GroupPass->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0,
	                                       GroupPass->sizePolicy().hasHeightForWidth() ) );
	GroupPass->setTitle( tr( "Passwords" ) );
	GroupPass->setColumnLayout(0, Qt::Vertical );
	GroupPass->layout()->setSpacing( 5 );
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
	GroupSecSet->layout()->setSpacing( 5 );
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
	CopySec->setText( tr( "Allow Copying Text and Graphics" ) );
	CopySec->setChecked( Optionen->Permissions & 16 );
	GroupSecSetLayout->addWidget( CopySec );
	AddSec = new QCheckBox( GroupSecSet, "AddSec" );
	AddSec->setText( tr( "Allow Adding Annotations and Fields" ) );
	AddSec->setChecked( Optionen->Permissions & 32 );
	GroupSecSetLayout->addWidget( AddSec );
	tabsecLayout->addWidget( GroupSecSet );
	if (!Encry->isChecked())
	{
		GroupSecSet->setEnabled(false);
		GroupPass->setEnabled(false);
	}
	Options->insertTab( tabsec, tr( "&Security" ) );

	tabcolor = new QWidget( Options, "tabcolor" );
	tabcolorLayout = new QVBoxLayout( tabcolor, 11, 5, "tabcolorLayout");
	ColorGroup = new QGroupBox( tabcolor, "ColorGroup" );
	ColorGroup->setTitle( tr( "General" ) );
	ColorGroup->setColumnLayout(0, Qt::Vertical );
	ColorGroup->layout()->setSpacing( 5 );
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

	UseLPI = new QCheckBox( tabcolor, "UseLPI" );
	UseLPI->setText( tr( "Use Custom Rendering Settings" ) );
	UseLPI->setChecked(Optionen->UseLPI);
	tabcolorLayout->addWidget( UseLPI );
	LPIgroup = new QGroupBox( tabcolor, "LPIgroup" );
	LPIgroup->setTitle( tr( "Rendering Settings" ) );
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
	textLPI1 = new QLabel( LPIgroup, "textLPI1" );
	textLPI1->setText( tr( "Frequency:" ) );
	LPIgroupLayout->addWidget( textLPI1, 0, 1 );
	LPIfreq = new QSpinBox( LPIgroup, "LPIfreq" );
	LPIfreq->setMinValue(10);
	LPIfreq->setMaxValue(1000);
	LPIfreq->setValue(Optionen->LPISettings[LPIcolor->currentText()].Frequency);
	LPIgroupLayout->addWidget( LPIfreq, 0, 2 );
	textLPI2 = new QLabel( LPIgroup, "textLPI2" );
	textLPI2->setText( tr( "Angle:" ) );
	LPIgroupLayout->addWidget( textLPI2, 1, 1 );
	LPIangle = new QSpinBox( LPIgroup, "LPIangle" );
	LPIangle->setSuffix( QString::fromUtf8(" °"));
	LPIangle->setMinValue(-180);
	LPIangle->setMaxValue(180);
	LPIangle->setValue(Optionen->LPISettings[LPIcolor->currentText()].Angle);
	LPIgroupLayout->addWidget( LPIangle, 1, 2 );
	textLPI3 = new QLabel( LPIgroup, "textLPI3" );
	textLPI3->setText( tr( "Spot Function:" ) );
	LPIgroupLayout->addWidget( textLPI3, 2, 1 );
	LPIfunc = new QComboBox( true, LPIgroup, "LPIfunc" );
	LPIfunc->setEditable(false);
	LPIfunc->insertItem( tr( "Simple Dot" ) );
	LPIfunc->insertItem( tr( "Line" ) );
	LPIfunc->insertItem( tr( "Round" ) );
	LPIfunc->insertItem( tr( "Ellipse" ) );
	LPIfunc->setCurrentItem(Optionen->LPISettings[LPIcolor->currentText()].SpotFunc);
	LPIgroupLayout->addWidget( LPIfunc, 2, 2 );
	tabcolorLayout->addWidget( LPIgroup );
	SelLPIcolor = LPIcolor->currentText();

	GroupBox9 = new QGroupBox( tabcolor, "GroupBox9" );
	GroupBox9->setTitle( tr( "Solid Colors:" ) );
	GroupBox9->setColumnLayout(0, Qt::Vertical );
	GroupBox9->layout()->setSpacing( 5 );
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
	ProfsGroup->layout()->setSpacing( 5 );
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
	tabcolorLayout->addItem( spacerCG );
	Options->insertTab( tabcolor, tr( "&Color" ) );

	tabpdfx = new QWidget( Options, "tabpdfx" );
	tabpdfxLayout = new QVBoxLayout( tabpdfx, 11, 5, "tabpdfxLayout");
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
	tabpdfxLayout->addWidget( BleedGroup );
	Options->insertTab( tabpdfx, tr( "&PDF/X-3" ) );
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
	Layout7->setSpacing( 5 );
	Layout7->setMargin( 0 );
	QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout7->addItem( spacer_2 );
	OK = new QPushButton( this, "OK" );
	OK->setText( tr( "Save" ) );
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
	QToolTip::add( ComboBox1, tr( "Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.\nChoose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.\nPDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management." ) );
	QToolTip::add( ComboBind, tr( "Determines the binding of pages in the PDF. Unless you know\nyou need to change it leave the default choice - Left." ) );
	QToolTip::add( CheckBox1, tr( "Generates thumbnails of each page in the PDF.\nSome viewers can use the thumbnails for navigation." ) );
	QToolTip::add( Article, tr( "Generate PDF Articles, which is useful for navigating linked articles in a PDF." ) );
	QToolTip::add( CheckBM, tr( "Embed the bookmarks you created in your document.\nThese are useful for navigating long PDF documents." ) );
	QToolTip::add( Resolution, tr( "Export resolution of text and vector graphics.\nThis does not affect the resolution of bitmap images like photos." ) );
	QToolTip::add( Compression, tr( "Compression of text and graphics.\nUnless you have a reason, leave this checked. This reduces PDF size." ) );
	QToolTip::add( CMethod, tr( "Version of compression for images.\nAutomatic allows Scribus to choose the best method.\nZIP is good for images with solid colors.\nJPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).\nLeave it set to automatic, unless you have a need for special compression options." ) );
	QToolTip::add( CQuality, tr( "Compression levels: Minimum (25\%), Low (50\%), Medium (75\%), High (85\%), Maximum (95\%)" ) );
	QToolTip::add( DSColor, tr( "Downsample your bitmap images to the selected DPI.\nLeaving this unchecked will render them at their native resolution." ) );
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
	QToolTip::add( AddSec, tr( "Allow adding annotations and fields to the PDF. \nIf un-checked, editing annotations and fileds is prevented." ) );
	QToolTip::add( OutCombo, tr( "Color model for the output of your PDF.\nChoose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.\nChoose Printer when printing to a true 4 color CMYK printer." ) );
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

	// signals and slots connections
	connect( FileC, SIGNAL( clicked() ), this, SLOT( ChangeFile() ) );
	connect( OK, SIGNAL( clicked() ), this, SLOT( DoExport() ) );
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
	disconnect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
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
	connect(OutCombo, SIGNAL(activated(int)), this, SLOT(EnablePr(int)));
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
	tabcolor->layout()->activate();
	tabcolor->updateGeometry();
	adjustSize();
}

void PDF_Opts::EnableLPI2()
{
	if (UseLPI->isChecked())
		LPIgroup->show();
	else
		LPIgroup->hide();
	tabcolor->layout()->activate();
	tabcolor->updateGeometry();
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
	Datei->setText(fn);
}
