/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "annot.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGroupBox>
#include <QTextEdit>
#include <QLabel>
#include <QFrame>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QWidget>
#include <QRadioButton>
#include <QList>
#include <QStringList>
#include <QSpacerItem>
#include <QToolTip>
#include <QDateTime>
#include <QPixmap>

#include "annotation.h"
#include "buttonicon.h"
#include "cmsettings.h"
#include "colorcombo.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "customfdialog.h"
#include "editor.h"
#include "propertiespalette.h"
#include "navigator.h"
#include "pageitem.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "sccombobox.h"
#include "scfonts.h"
#include "scimage.h"
#include "scribusstructs.h"
#include "scribusview.h"
#include "selfield.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util.h"

Annot::Annot(QWidget* parent, PageItem *it, int Seite, int b, int h, ColorList Farben, ScribusView* vie)
		: QDialog( parent )
{
	ScribusDoc* doc = Farben.document();
	setModal(true);
	setWindowTitle( tr( "Field Properties" ) );
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	item = it;
	Breite = b;
	Hoehe = h;
	OriBreite = b;
	OriHoehe = h;
	view = vie;
	MaxSeite = Seite;
	QStringList tl;
	dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	if ((item->annotation().ActionType() == 2) || (item->annotation().ActionType() == 7) || (item->annotation().ActionType() == 9))
	{
		QString tm = item->annotation().Action();
		tl = tm.split(" ", QString::SkipEmptyParts);
	}
	else
	{
		tl.append("0");
		tl.append("0");
	}

	AnnotLayout = new QVBoxLayout( this );
	AnnotLayout->setSpacing( 5 );
	AnnotLayout->setMargin( 10 );

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 5 );
	Layout1->setMargin( 0 );

	TextLabel1 = new QLabel( this );
	TextLabel1->setText( tr( "Type:" ) );
	Layout1->addWidget( TextLabel1 );

	ComboBox1 = new QComboBox( this );
	QString tmp[] = { tr("Button"), tr("Text Field"), tr("Check Box"), tr("Combo Box"), tr("List Box")};
	size_t array = sizeof(tmp) / sizeof(*tmp);
	for (uint a = 0; a < array; ++a)
		ComboBox1->addItem(tmp[a]);
	ComboBox1->setEditable(false);
	Layout1->addWidget( ComboBox1 );
	AnnotLayout->addLayout( Layout1 );
	ComboBox1->setCurrentIndex(item->annotation().Type()-2);

	Fram = new QStackedWidget(this);
	AnnotLayout->addWidget( Fram );

	GroupBox10 = new QGroupBox( this );
//	GroupBox10->setFixedSize(QSize(360, 350));
//	GroupBox10->setGeometry( QRect( 11, 41, 360, 350 ) );
	GroupBox10->setTitle( tr( "Properties" ) );
	GroupBox10Layout = new QVBoxLayout( GroupBox10 );
	GroupBox10Layout->setSpacing( 5 );
	GroupBox10Layout->setMargin( 10 );
	GroupBox10Layout->setAlignment( Qt::AlignTop );

	Layout60 = new QGridLayout;
	Layout60->setSpacing( 5 );
	Layout60->setMargin( 0 );
	TextLabel20 = new QLabel( GroupBox10 );
	TextLabel20->setText( tr( "Name:" ) );
	Layout60->addWidget( TextLabel20, 0, 0 );

	Tip = new QLineEdit( GroupBox10 );
	Tip->setText(item->annotation().ToolTip());
	Layout60->addWidget( Tip, 1, 1 );

	Name = new NameWidget(GroupBox10);
	Name->setText(item->itemName());
	Layout60->addWidget( Name, 0, 1 );
	OldName = item->itemName();

	TextLabel30 = new QLabel( GroupBox10 );
	TextLabel30->setText( tr( "Tooltip:" ) );
	Layout60->addWidget( TextLabel30, 1, 0);
	GroupBox10Layout->addLayout( Layout60 );

	TabWidget2 = new QTabWidget( GroupBox10 );

	tab = new QWidget( TabWidget2 );
	tabLayout = new QVBoxLayout(tab);
	tabLayout->setSpacing( 5 );
	tabLayout->setMargin( 10 );

	GroupBox40 = new QGroupBox( tab );
	GroupBox40->setTitle( tr( "Text" ) );
	GroupBox40Layout = new QGridLayout( GroupBox40 );
	GroupBox40Layout->setSpacing( 5 );
	GroupBox40Layout->setMargin( 10 );
	GroupBox40Layout->setAlignment( Qt::AlignTop );
	TextLabel60 = new QLabel( GroupBox40 );
	TextLabel60->setText( tr( "Font for use with PDF 1.3:" ) );
	GroupBox40Layout->addWidget( TextLabel60, 0, 0 );
	Schrift = new QComboBox(GroupBox40);
	/* PFJ - 28/02/04 - Altered from char* to QString. Renamed to fonts and the
	                    loop changed from uint to int and the name to propogate */
	/* FS - 08.03.04 - loop variable must be uint otherwise you get a compiler warning */
	QString fonts[]={"Courier", "Courier Bold", "Courier Italic",
	                 "Courier Bold Italic", "Helvetica",
	                 "Helvetica Bold", "Helvetica Italic", "Helvetica Bold Italic",
	                 "Times", "Times Bold",
	                 "Times Italic", "Times Bold Italic", "Zapf Dingbats",
	                 "Symbols"};
	size_t fontsArray = sizeof(fonts) / sizeof(*fonts);
	for (uint propogate = 0; propogate < fontsArray; ++propogate)
		Schrift->addItem(fonts[propogate]);
	Schrift->setEditable(false);
	Schrift->setCurrentIndex(item->annotation().Font());
	GroupBox40Layout->addWidget( Schrift, 0, 1, 1, 2);
	tabLayout->addWidget( GroupBox40 );

	Layout10 = new QHBoxLayout;
	Layout10->setSpacing( 5 );
	Layout10->setMargin( 0 );
	GroupBox20 = new QGroupBox( tab );
	GroupBox20->setTitle( tr( "Border" ) );
	GroupBox20Layout = new QGridLayout( GroupBox20 );
	GroupBox20Layout->setSpacing( 5 );
	GroupBox20Layout->setMargin( 10 );
	GroupBox20Layout->setAlignment( Qt::AlignTop );
	TextLabel40a = new QLabel( GroupBox20 );
	TextLabel40a->setText( tr( "Color:" ) );
	GroupBox20Layout->addWidget( TextLabel40a, 0, 0 );
	BorderC = new ColorCombo( false, GroupBox20);
	ColorList::Iterator cit;
	BorderC->addItem(CommonStrings::tr_NoneColor);
	if (item->annotation().borderColor() == CommonStrings::None)
		BorderC->setCurrentIndex(BorderC->count()-1);
	for (cit = Farben.begin(); cit != Farben.end(); ++cit)
	{
		BorderC->insertFancyItem(cit.value(), doc, cit.key());
		if (cit.key() == item->annotation().borderColor())
			BorderC->setCurrentIndex(BorderC->count()-1);
	}
	GroupBox20Layout->addWidget( BorderC, 0, 1 );
	TextLabel40 = new QLabel( GroupBox20 );
	TextLabel40->setText( tr( "Width:" ) );
	GroupBox20Layout->addWidget( TextLabel40, 1, 0 );
	BorderW = new QComboBox( GroupBox20 );
	// PFJ - 28/02/04 - Altered to the QString/size_t/for style
	QString borders[] = {CommonStrings::tr_NoneColor, tr("Thin"), tr("Normal"), tr("Wide")};
	size_t bordersArray = sizeof(borders) / sizeof(*borders);
	for (uint propogate = 0; propogate < bordersArray; ++propogate)
		BorderW->addItem(borders[propogate]);
	BorderW->setEditable(false);
	BorderW->setCurrentIndex(item->annotation().Bwid());
	GroupBox20Layout->addWidget( BorderW, 1, 1 );
	TextLabel50 = new QLabel( GroupBox20 );
	TextLabel50->setText( tr( "Style:" ) );
	GroupBox20Layout->addWidget( TextLabel50, 2, 0 );
	BorderS = new QComboBox( GroupBox20 );
	/* PFJ - 28/02/04 - Altered to the QString/size_t/for style */
	QString borders2[] = {tr("Solid"), tr("Dashed"), tr("Underline"),
	                      tr("Beveled"), tr("Inset")};
	size_t borders2Array = sizeof(borders2) / sizeof(*borders2);
	for (uint propogate = 0; propogate < borders2Array; ++propogate)
		BorderS->addItem(borders2[propogate]);
	BorderS->setEditable(false);
	BorderS->setCurrentIndex(item->annotation().Bsty());
	GroupBox20Layout->addWidget( BorderS, 2, 1 );
	Layout10->addWidget( GroupBox20 );

	GroupBox30 = new QGroupBox( tab );
	GroupBox30->setTitle( tr( "Other" ) );
	GroupBox30Layout = new QGridLayout( GroupBox30 );
	GroupBox30Layout->setSpacing( 5 );
	GroupBox30Layout->setMargin( 10 );
	GroupBox30Layout->setAlignment( Qt::AlignTop );
	ReadOnly = new QCheckBox( GroupBox30 );
	ReadOnly->setText( tr( "Read Only" ) );
	if (item->annotation().Type() == 2)
		ReadOnly->setEnabled(false);
	ReadOnly->setChecked(item->annotation().Flag() & 1);
	GroupBox30Layout->addWidget( ReadOnly, 0, 0, 1, 2 );
	Required = new QCheckBox( GroupBox30 );
	Required->setText( tr( "Required" ) );
	if (item->annotation().Type() == 2)
		Required->setEnabled(false);
	Required->setChecked(item->annotation().Flag() & 2);
	GroupBox30Layout->addWidget( Required, 1, 0, 1, 2 );
	NoExport = new QCheckBox( GroupBox30 );
	NoExport->setText( tr( "Do Not Export Value" ) );
	if (item->annotation().Type() == 2)
		NoExport->setEnabled(false);
	NoExport->setChecked(item->annotation().Flag() & 4);
	GroupBox30Layout->addWidget( NoExport, 2, 0, 1, 2 );
	TextLabel90 = new QLabel( GroupBox30 );
	TextLabel90->setText( tr( "Visibility:" ) );
	GroupBox30Layout->addWidget( TextLabel90, 3, 0 );
	Visib = new QComboBox( GroupBox30 );
	/* PFJ - 28/02/04 - Altered to the QString/size_t/for style */
	QString visible[] = {tr("Visible"), tr("Hidden"), tr("No Print"),
	                     tr("No View")};
	size_t visibleArray = sizeof(visible) / sizeof(*visible);
	for (uint propogate = 0; propogate < visibleArray; ++propogate)
		Visib->addItem(visible[propogate]);
	Visib->setEditable(false);
	Visib->setCurrentIndex(item->annotation().Vis());
	GroupBox30Layout->addWidget( Visib, 3, 1 );
	Layout10->addWidget( GroupBox30 );
	tabLayout->addLayout( Layout10 );
	TabWidget2->addTab( tab, tr( "Appearance" ) );

	tab_3 = new QWidget( TabWidget2 );
	tabLayout_3 = new QVBoxLayout( tab_3 );
	tabLayout_3->setSpacing( 5 );
	tabLayout_3->setMargin( 10 );
	FramOp = new QStackedWidget(tab_3);
	Frame4a = new QFrame( tab_3 );
	Frame4aLayout = new QGridLayout( Frame4a );
	Frame4aLayout->setSpacing( 0 );
	Frame4aLayout->setMargin( 0 );
	GroupBox40a = new QGroupBox( Frame4a );
	GroupBox40a->setTitle( tr( "Text" ) );
	GroupBox40aLayout = new QGridLayout( GroupBox40a );
	GroupBox40aLayout->setSpacing( 5 );
	GroupBox40aLayout->setMargin( 10 );
	GroupBox40aLayout->setAlignment( Qt::AlignTop );
	CheckBox30 = new QLabel( GroupBox40a );
	CheckBox30->setText( tr( "Text for Button Down" ) );
	GroupBox40aLayout->addWidget( CheckBox30, 0, 0, 1, 2 );
	CheckBox40 = new QLabel( GroupBox40a );
	CheckBox40->setText( tr( "Text for Roll Over" ) );
	GroupBox40aLayout->addWidget( CheckBox40, 1, 0, 1, 2 );
	DownT = new QLineEdit( GroupBox40a );
	DownT->setText(item->annotation().Down());
	GroupBox40aLayout->addWidget( DownT, 0, 2 );
	TextO = new QLineEdit( GroupBox40a );
	TextO->setText(item->annotation().RollOver());
	GroupBox40aLayout->addWidget( TextO, 1, 2 );
	Frame4aLayout->addWidget( GroupBox40a, 0, 0, 1, 2 );

	OptIcons = new QGroupBox( Frame4a );
	OptIcons->setTitle( tr( "Icons" ) );
	OptIconsLayout = new QVBoxLayout( OptIcons );
	OptIconsLayout->setSpacing( 5 );
	OptIconsLayout->setMargin( 10 );
	OptIconsLayout->setAlignment( Qt::AlignTop );
	UseIcons = new QCheckBox(OptIcons);
	UseIcons->setText( tr("Use Icons"));
	UseIcons->setChecked(item->annotation().UseIcons());
	OptIconsLayout->addWidget(UseIcons);
	Layout17 = new QHBoxLayout;
	Layout17->setSpacing( 5 );
	Layout17->setMargin( 0 );

	Layout14 = new QGridLayout;
	Layout14->setSpacing( 5 );
	Layout14->setMargin( 0 );
	IconN = new QPushButton( OptIcons );
	IconN->setText( tr( "Normal" ) );
	Layout14->addWidget( IconN, 0, 0, 1, 3);
	NiconPrev = new QLabel( OptIcons );
	NiconPrev->setMinimumSize( QSize( 25, 25 ) );
	NiconPrev->setMaximumSize( QSize( 25, 25 ) );
	NiconPrev->setFrameShape( QLabel::Panel );
	NiconPrev->setFrameShadow( QLabel::Raised );
	NiconPrev->setScaledContents( true );
	Layout14->addWidget( NiconPrev, 1, 1 );
	IconNR = new QPushButton( OptIcons );
	IconNR->setText( tr( "Remove" ) );
	Layout14->addWidget( IconNR, 2, 0, 1, 3);
	Layout17->addLayout( Layout14 );

	Layout15 = new QGridLayout;
	Layout15->setSpacing( 5 );
	Layout15->setMargin( 0 );
	IconP = new QPushButton( OptIcons );
	IconP->setText( tr( "Pressed" ) );
	Layout15->addWidget( IconP, 0, 0, 1, 3 );
	PiconPrev = new QLabel( OptIcons );
	PiconPrev->setMinimumSize( QSize( 25, 25 ) );
	PiconPrev->setMaximumSize( QSize( 25, 25 ) );
	PiconPrev->setFrameShape( QLabel::Panel );
	PiconPrev->setFrameShadow( QLabel::Raised );
	PiconPrev->setScaledContents( true );
	Layout15->addWidget( PiconPrev, 1, 1 );
	IconPR = new QPushButton( OptIcons );
	IconPR->setText( tr( "Remove" ) );
	Layout15->addWidget( IconPR, 2, 0, 1, 3);
	Layout17->addLayout( Layout15 );

	Layout16 = new QGridLayout;
	Layout16->setSpacing( 5 );
	Layout16->setMargin( 0 );
	IconR = new QPushButton( OptIcons );
	IconR->setText( tr( "Roll Over" ) );
	Layout16->addWidget( IconR, 0, 0, 1, 3 );
	RiconPrev = new QLabel( OptIcons );
	RiconPrev->setMinimumSize( QSize( 25, 25 ) );
	RiconPrev->setMaximumSize( QSize( 25, 25 ) );
	RiconPrev->setFrameShape( QLabel::Panel );
	RiconPrev->setFrameShadow( QLabel::Raised );
	RiconPrev->setScaledContents( true );
	Layout16->addWidget( RiconPrev, 1, 1 );
	IconRR = new QPushButton( OptIcons );
	IconRR->setText( tr( "Remove" ) );
	Layout16->addWidget( IconRR, 2, 0, 1, 3);
	Layout17->addLayout( Layout16 );
	OptIconsLayout->addLayout( Layout17 );

	Layout18 = new QHBoxLayout;
	Layout18->setSpacing( 5 );
	Layout18->setMargin( 0 );
	PlaceIcon = new QPushButton( OptIcons );
	PlaceIcon->setText( tr( "Icon Placement..." ) );
	Layout18->addWidget( PlaceIcon );
	QSpacerItem* spacer_2x = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout18->addItem( spacer_2x );
	OptIconsLayout->addLayout( Layout18 );
	IconNR->setEnabled(false);
	IconPR->setEnabled(false);
	IconRR->setEnabled(false);
	if (!UseIcons->isChecked())
	{
		IconN->setEnabled(false);
		IconP->setEnabled(false);
		IconR->setEnabled(false);
		NiconPrev->setEnabled(false);
		PiconPrev->setEnabled(false);
		RiconPrev->setEnabled(false);
		PlaceIcon->setEnabled(false);
	}
	else
	{
		QPixmap pmI1;
		ScImage im;
		CMSettings cms(view->Doc, "", 0);
		if (!item->Pfile.isEmpty())
		{
			im.LoadPicture(item->Pfile, 1, cms, false, false, ScImage::RGBData, 72);
			pmI1=QPixmap::fromImage(im.qImage());
			NiconPrev->setPixmap(pmI1);
			IconNR->setEnabled(true);
		}
		if (!item->Pfile2.isEmpty())
		{
			im.LoadPicture(item->Pfile2, 1, cms, false, false, ScImage::RGBData, 72);
			pmI1=QPixmap::fromImage(im.qImage());
			PiconPrev->setPixmap(pmI1);
			IconPR->setEnabled(true);
		}
		if (!item->Pfile3.isEmpty())
		{
			im.LoadPicture(item->Pfile3, 1, cms, false, false, ScImage::RGBData, 72);
			pmI1=QPixmap::fromImage(im.qImage());
			RiconPrev->setPixmap(pmI1);
			IconRR->setEnabled(true);
		}
	}
	Frame4aLayout->addWidget( OptIcons, 1, 0 );

	GroupBox30a = new QGroupBox( Frame4a );
	GroupBox30a->setTitle( tr( "Highlight" ) );
	GroupBox30aLayout = new QGridLayout( GroupBox30a );
	GroupBox30aLayout->setSpacing( 5 );
	GroupBox30aLayout->setMargin( 10 );
	GroupBox30aLayout->setAlignment( Qt::AlignTop );
	ComboBox7_2 = new QComboBox(GroupBox30a);
	// PFJ - 28/02/04 - Altered to QString/size_t/for style
	QString combo[] = {tr("None", "highlight"), tr("Invert"), tr("Outlined"), tr("Push")};
	size_t comboArray = sizeof(combo) / sizeof(*combo);
	for (uint propogate = 0; propogate < comboArray; ++propogate)
		ComboBox7_2->addItem(combo[propogate]);
	ComboBox7_2->setEditable(false);
	ComboBox7_2->setCurrentIndex(item->annotation().Feed());
	GroupBox30aLayout->addWidget( ComboBox7_2, 0, 0 );
	Frame4aLayout->addWidget( GroupBox30a, 1, 1 );
	FramOp->addWidget( Frame4a );

	OptTextFeld = new QGroupBox( tab_3 );
	OptTextFeld->setTitle( "" );
	OptTextFeldLayout = new QVBoxLayout( OptTextFeld );
	OptTextFeldLayout->setSpacing( 5 );
	OptTextFeldLayout->setMargin( 10 );
	OptTextFeldLayout->setAlignment( Qt::AlignTop );
	MultiL = new QCheckBox( OptTextFeld );
	MultiL->setText( tr( "Multi-Line" ) );
	MultiL->setChecked(item->annotation().Flag() & 4096);
	OptTextFeldLayout->addWidget( MultiL );
	Passwd = new QCheckBox( OptTextFeld );
	Passwd->setText( tr( "Password" ) );
	Passwd->setChecked(item->annotation().Flag() & 8192);
	OptTextFeldLayout->addWidget( Passwd );
	Layout8 = new QHBoxLayout;
	Layout8->setSpacing( 5 );
	Layout8->setMargin( 0 );
	Limit = new QCheckBox( OptTextFeld );
	Limit->setText( tr( "Limit of" ) );
	Layout8->addWidget( Limit );
	MaxChars = new QSpinBox( OptTextFeld );
	MaxChars->setMinimum(0);
	MaxChars->setMaximum(32768);
	bool setter = item->annotation().MaxChar() != -1 ? true : false;
	MaxChars->setValue(setter == true ? item->annotation().MaxChar() : 0);
	Limit->setChecked(setter);
	MaxChars->setEnabled(setter);
	Layout8->addWidget( MaxChars );
	TextLabel2_2 = new QLabel( OptTextFeld );
	TextLabel2_2->setText( tr( "Characters" ) );
	Layout8->addWidget( TextLabel2_2 );
	QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout8->addItem( spacer_2 );
	OptTextFeldLayout->addLayout( Layout8 );
	NoScroll = new QCheckBox( OptTextFeld );
	NoScroll->setText( tr( "Do Not Scroll" ) );
	NoScroll->setChecked(item->annotation().Flag() & 8388608);
	OptTextFeldLayout->addWidget( NoScroll );
	NoSpell = new QCheckBox( OptTextFeld );
	NoSpell->setText( tr( "Do Not Spell Check" ) );
	NoSpell->setChecked(item->annotation().Flag() & 4194304);
	OptTextFeldLayout->addWidget( NoSpell );
	FramOp->addWidget( OptTextFeld );

	OptCheck = new QGroupBox( tab_3 );
	OptCheck->setTitle( "" );
	OptCheckLayout = new QVBoxLayout( OptCheck );
	OptCheckLayout->setSpacing( 5 );
	OptCheckLayout->setMargin( 10 );
	OptCheckLayout->setAlignment( Qt::AlignTop );
	ChkLayout = new QHBoxLayout;
	ChkLayout->setSpacing( 5 );
	ChkLayout->setMargin( 0 );
	CText1 = new QLabel( OptCheck );
	CText1->setText( tr( "Check Style:" ) );
	ChkLayout->addWidget( CText1 );
	ChkStil = new QComboBox( OptCheck );
	QString tmp_chkstil2[]={ tr("Check"), tr("Cross"), tr("Diamond"), tr("Circle"), tr("Star"), tr("Square")};
	size_t array_chk2 = sizeof(tmp_chkstil2) / sizeof(*tmp_chkstil2);
	/* PFJ - 28/02/04 - Altered from uint to int and altered var name */
	for (uint propogate = 0; propogate < array_chk2; ++propogate)
		ChkStil->addItem(tmp_chkstil2[propogate]);

	ChkStil->setEditable(false);
	ChkStil->setCurrentIndex(item->annotation().ChkStil());
	ChkLayout->addWidget( ChkStil );
	QSpacerItem* spacerC = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	ChkLayout->addItem( spacerC );
	OptCheckLayout->addLayout( ChkLayout );

	isChkd = new QCheckBox( OptCheck );
	isChkd->setText( tr( "Default is Checked" ) );
	isChkd->setChecked(item->annotation().IsChk());
	OptCheckLayout->addWidget( isChkd );
	FramOp->addWidget( OptCheck );

	OptCombo = new QGroupBox( tab_3 );
	OptCombo->setTitle( "" );
	OptComboLayout = new QVBoxLayout( OptCombo );
	OptComboLayout->setSpacing( 5 );
	OptComboLayout->setMargin( 10 );
	OptComboLayout->setAlignment( Qt::AlignTop );
	CanEdit = new QCheckBox( OptCombo );
	CanEdit->setText( tr( "Editable" ) );
	CanEdit->setChecked(item->annotation().Flag() & 262144);
	OptComboLayout->addWidget( CanEdit );
	FramOp->addWidget( OptCombo );

	tabLayout_3->addWidget( FramOp );

	TabWidget2->addTab( tab_3, tr( "Options" ) );

	tab_2 = new QWidget( TabWidget2 );
	tabLayout_2 = new QVBoxLayout( tab_2 );
	tabLayout_2->setSpacing( 5 );
	tabLayout_2->setMargin( 10 );

	Layout20 = new QHBoxLayout;
	Layout20->setSpacing( 5 );
	Layout20->setMargin( 0 );

	TextLabel70 = new QLabel( tab_2 );
	TextLabel70->setText( tr( "Type:" ) );
	Layout20->addWidget( TextLabel70 );

	ActionCombo = new QComboBox(tab_2);
	QString tmp_actcom[] = { tr("None", "action"), tr("JavaScript"), tr("Go To"),
	                        tr("Submit Form"), tr("Reset Form"), tr("Import Data")};
	size_t array_act = sizeof(tmp_actcom) / sizeof(*tmp_actcom);
	/* PFJ - 28/02/04 - Altered from uint to int and var name */
	for (uint propogate = 0; propogate < array_act; ++propogate)
		ActionCombo->addItem(tmp_actcom[propogate]);
	ActionCombo->setEditable(false);
	int tmpac = item->annotation().ActionType();
	if (item->annotation().ActionType() < 0)
		tmpac = 1;
	ActionCombo->setCurrentIndex(tmpac == 7 ? 2 : tmpac);

	Layout20->addWidget( ActionCombo );
	tabLayout_2->addLayout( Layout20 );

	Fram2 = new QStackedWidget(tab_2);
	tabLayout_2->addWidget( Fram2 );

	Frame4 = new QFrame( tab_2 );
	Frame4->setFrameShape( QFrame::NoFrame );
	Frame4->setFrameShadow( QFrame::Raised );
	Fram2->addWidget( Frame4 );

	Frame3 = new QFrame( tab_2 );
	Frame3->setFrameShape( QFrame::Box );
	Frame3->setFrameShadow( QFrame::Sunken );
	Frame3Layout = new QVBoxLayout( Frame3 );
	Frame3Layout->setSpacing( 5 );
	Frame3Layout->setMargin( 10 );
	Layout7 = new QHBoxLayout;
	Layout7->setSpacing( 5 );
	Layout7->setMargin( 0 );
	AcText1 = new QLabel( Frame3 );
	AcText1->setText( tr( "Event:" ) );
	Layout7->addWidget( AcText1 );
	SelAction = new QComboBox(Frame3);
	QString tmp_selact[]={ tr("Mouse Up"), tr("Mouse Down"), tr("Mouse Enter"),
	                      tr("Mouse Exit"), tr("On Focus"), tr("On Blur")};
	size_t array_sel = sizeof(tmp_selact) / sizeof(*tmp_selact);
	/* PFJ - 28/02/04 - Altered from uint to int and var name */
	for (uint propogate = 0; propogate < array_sel; ++propogate)
		SelAction->addItem(tmp_selact[propogate]);
	SelAction->setEditable(false);
	Layout7->addWidget( SelAction );
	QSpacerItem* spacerac = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout7->addItem( spacerac );
	Frame3Layout->addLayout( Layout7 );
	AcText2 = new QLabel( Frame3 );
	AcText2->setText( tr( "Script:" ) );
	Frame3Layout->addWidget( AcText2 );
	Layout71 = new QHBoxLayout;
	Layout71->setSpacing( 5 );
	Layout71->setMargin( 0 );
	EditJava = new QTextEdit( Frame3 );
	QPalette palTxt = EditJava->palette();
	palTxt.setColor(QPalette::Base, palette().color(QPalette::Window));
	if ((item->annotation().ActionType() == 1) || (item->annotation().AAact()))
		EditJava->setPlainText(item->annotation().Action());
	ScrEdited = 0;
	SelAction->setCurrentIndex(0);
	EditJava->setReadOnly(true);
	EditJava->setAutoFillBackground(true);
	EditJava->setPalette(palTxt);
	EditJava->setMinimumSize( QSize( 0, 50 ) );
	EditJava->setFocusPolicy(Qt::NoFocus);
	Layout71->addWidget( EditJava );
	EditJ = new QPushButton( Frame3 );
	EditJ->setText( tr( "Edit..." ) );
	Layout71->addWidget( EditJ );
	Frame3Layout->addLayout( Layout71 );
	Fram2->addWidget( Frame3 );

	GroupBox11 = new QGroupBox( tab_2 );
	GroupBox11->setTitle( tr( "Destination" ) );
	GroupBox11Layout = new QGridLayout( GroupBox11 );
	GroupBox11Layout->setSpacing( 5 );
	GroupBox11Layout->setMargin( 10 );
	GroupBox11Layout->setAlignment( Qt::AlignTop );
	LExtern = new QCheckBox(GroupBox11);
	LExtern->setText( tr("To File:"));
	GroupBox11Layout->addWidget( LExtern, 0, 0 );
	Destfile = new QLineEdit(GroupBox11);
	Destfile->setText(item->annotation().Extern());
	Destfile->setReadOnly(true);
	GroupBox11Layout->addWidget( Destfile, 0, 1 );
	ChFile = new QPushButton(GroupBox11);
	ChFile->setText( tr("Change..."));
	GroupBox11Layout->addWidget( ChFile, 0, 2 );
	useAbsolute = new QCheckBox( tr("Export absolute Filename"), GroupBox11);
	GroupBox11Layout->addWidget( useAbsolute, 1, 0, 1, 3 );
	if (item->annotation().ActionType() == 7)
		useAbsolute->setChecked(false);
	else if (item->annotation().ActionType() == 9)
		useAbsolute->setChecked(true);
	TextLabel31 = new QLabel( GroupBox11 );
	TextLabel31->setText( tr( "Page:" ) );
	GroupBox11Layout->addWidget( TextLabel31, 2, 0 );
	SpinBox11 = new QSpinBox( GroupBox11 );
	SpinBox11->setMinimum(1);
	SpinBox11->setMaximum(((item->annotation().ActionType() == 7) || (item->annotation().ActionType() == 9)) ? 1000 : Seite);
	SpinBox11->setValue(qMin(item->annotation().Ziel()+1, Seite));
	GroupBox11Layout->addWidget( SpinBox11, 2, 1 );
	if ((item->annotation().ActionType() == 7) || (item->annotation().ActionType() == 9))
		Pg1 = new Navigator( GroupBox11, 100, item->annotation().Ziel()+1, view, item->annotation().Extern());
	else
		Pg1 = new Navigator( GroupBox11, 100, qMin(item->annotation().Ziel(), Seite-1), view);
	Pg1->setMinimumSize(QSize(Pg1->pmx.width(), Pg1->pmx.height()));
	GroupBox11Layout->addWidget(Pg1, 2, 2, 3, 1);
	TextLabel41 = new QLabel( GroupBox11 );
	TextLabel41->setText( tr( "X-Pos:" ) );
	GroupBox11Layout->addWidget( TextLabel41, 3, 0 );
	SpinBox21 = new QSpinBox( GroupBox11 );
	SpinBox21->setSuffix( tr( " pt" ) );
	SpinBox21->setMaximum(Breite);
	SpinBox21->setValue(tl[0].toInt());
	GroupBox11Layout->addWidget( SpinBox21, 3, 1 );
	TextLabel51 = new QLabel( GroupBox11 );
	TextLabel51->setText( tr( "Y-Pos:" ) );
	GroupBox11Layout->addWidget( TextLabel51, 4, 0 );
	SpinBox31 = new QSpinBox( GroupBox11 );
	SpinBox31->setMaximum(Hoehe);
	SpinBox31->setSuffix( tr( " pt" ) );
	SpinBox31->setValue(Hoehe-tl[1].toInt());
	GroupBox11Layout->addWidget( SpinBox31, 4, 1 );
	Fram2->addWidget(GroupBox11);

	Frame3b = new QFrame( tab_2 );
	Frame3b->setFrameShape( QFrame::Box );
	Frame3b->setFrameShadow( QFrame::Sunken );
	Frame3bLayout = new QVBoxLayout( Frame3b );
	Frame3bLayout->setSpacing( 5 );
	Frame3bLayout->setMargin( 10 );
	SubText1 = new QLabel( Frame3b );
	SubText1->setText( tr( "Submit to URL:" ) );
	Frame3bLayout->addWidget( SubText1 );
	SubURL = new QLineEdit( Frame3b );
	Frame3bLayout->addWidget( SubURL );
	if (item->annotation().ActionType() == 3)
		SubURL->setText(item->annotation().Action());
	SubAsHtml = new QCheckBox( Frame3b );
	SubAsHtml->setChecked(item->annotation().HTML());
	SubAsHtml->setText( tr( "Submit Data as HTML" ) );
	Frame3bLayout->addWidget( SubAsHtml );
	QSpacerItem* spacerSu = new QSpacerItem( 2, 2, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Frame3bLayout->addItem( spacerSu);
	Fram2->addWidget( Frame3b );

	Frame3c = new QFrame( tab_2 );
	Frame3c->setFrameShape( QFrame::Box );
	Frame3c->setFrameShadow( QFrame::Sunken );
	Frame3cLayout = new QVBoxLayout( Frame3c );
	Frame3cLayout->setSpacing( 5 );
	Frame3cLayout->setMargin( 10 );
	SubText1a = new QLabel( Frame3c );
	SubText1a->setText( tr( "Import Data from:" ) );
	Frame3cLayout->addWidget( SubText1a );
	SubURLa = new QLineEdit( Frame3c );
	Frame3cLayout->addWidget( SubURLa );
	if (item->annotation().ActionType() == 5)
		SubURLa->setText(item->annotation().Action());
	QSpacerItem* spacerSua = new QSpacerItem( 2, 2, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Frame3cLayout->addItem( spacerSua);
	Fram2->addWidget( Frame3c );
	TabWidget2->addTab( tab_2, tr( "Action" ) );
	if ((item->annotation().ActionType() == 7) || (item->annotation().ActionType() == 9))
	{
		LExtern->setChecked(true);
		if (!Destfile->text().isEmpty())
		{
			Breite = Pg1->Breite;
			Hoehe = Pg1->Hoehe;
		}
		else
		{
			Destfile->setEnabled(false);
			ChFile->setEnabled(false);
			LExtern->setChecked(false);
		}
	}
	else
	{
		Destfile->setEnabled(false);
		ChFile->setEnabled(false);
		LExtern->setChecked(false);
	}

	tab4 = new QWidget( TabWidget2 );
	Layout = new QVBoxLayout( tab4 );
	Layout->setSpacing( 5 );
	Layout->setMargin( 10 );
	FLayout = new QHBoxLayout;
	FLayout->setSpacing( 5 );
	FLayout->setMargin( 0 );
	TextForm1 = new QLabel( tab4 );
	TextForm1->setText( tr( "Field is formatted as:" ) );
	FLayout->addWidget( TextForm1 );
	TxFormat = new QComboBox(tab4);
	QString tmp_txf[]={ tr("Plain"), tr("Number"), tr("Percentage"), tr("Date"), tr("Time"), tr("Custom")};
	size_t array_txf = sizeof(tmp_txf) / sizeof(*tmp_txf);
	for (uint a = 0; a < array_txf; ++a)
		TxFormat->addItem(tmp_txf[a]);
	TxFormat->setEditable(false);
	TxFormat->setCurrentIndex(item->annotation().Format());
	FLayout->addWidget( TxFormat );
	QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	FLayout->addItem( spacer_3 );
	Layout->addLayout( FLayout );
	FoFram = new QStackedWidget(tab4);

	FoFrameNone = new QFrame( tab4 );
	FoFrameNone->setFrameShape( QFrame::NoFrame );
	FoFrameNone->setFrameShadow( QFrame::Plain );
	FoFram->addWidget(FoFrameNone);

	NumbGroup = new QGroupBox( tab4 );
	NumbGroup->setTitle( tr( "Number Format" ) );
	NumbGroupLayout = new QVBoxLayout( NumbGroup );
	NumbGroupLayout->setSpacing( 5 );
	NumbGroupLayout->setMargin( 10 );
	NumbGroupLayout->setAlignment( Qt::AlignTop );
	LayoutFN1 = new QHBoxLayout;
	LayoutFN1->setSpacing( 5 );
	LayoutFN1->setMargin( 0 );
	TextNu1 = new QLabel( NumbGroup );
	TextNu1->setText( tr( "Decimals:" ) );
	LayoutFN1->addWidget( TextNu1 );
	Decim = new QSpinBox( NumbGroup );
	Decim->setMinimum(0);
	Decim->setMaximum(12);
	Decim->setValue(0);
	LayoutFN1->addWidget( Decim );
	QSpacerItem* spacer_4 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	LayoutFN1->addItem( spacer_4 );
	NumbGroupLayout->addLayout( LayoutFN1 );
	LayoutFN2 = new QHBoxLayout;
	LayoutFN2->setSpacing( 5 );
	LayoutFN2->setMargin( 0 );
	UseCurr = new QCheckBox( NumbGroup );
	UseCurr->setText( tr( "Use Currency Symbol" ) );
	LayoutFN2->addWidget( UseCurr );
	CurSym = new QLineEdit( NumbGroup );
	LayoutFN2->addWidget( CurSym );
	CurSym->setEnabled(false);
	QSpacerItem* spacer_5 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	LayoutFN2->addItem( spacer_5 );
	NumbGroupLayout->addLayout( LayoutFN2 );
	PreCurr = new QCheckBox( NumbGroup );
	PreCurr->setText( tr( "Prepend Currency Symbol" ) );
	PreCurr->setEnabled(false);
	NumbGroupLayout->addWidget( PreCurr );
	NumGroup2 = new QGroupBox( NumbGroup );
	NumGroup2->setTitle( tr( "Formatting" ) );
	NumGroup2Layout = new QGridLayout( NumGroup2 );
	NumGroup2Layout->setSpacing( 5 );
	NumGroup2Layout->setMargin( 10 );
	NumGroup2Layout->setAlignment( Qt::AlignTop );
	Format0 = new QRadioButton( NumGroup2 );
	Format0->setText("9,999.99");
	Format0->setChecked( true );
	FormNum = 0;
	NumGroup2Layout->addWidget( Format0, 0, 0 );
	Format1 = new QRadioButton( NumGroup2 );
	Format1->setText("9999.99");
	NumGroup2Layout->addWidget( Format1, 1, 0 );
	Format2 = new QRadioButton( NumGroup2 );
	Format2->setText("9.999,99");
	NumGroup2Layout->addWidget( Format2, 0, 1 );
	Format3 = new QRadioButton( NumGroup2 );
	Format3->setText("9999,99");
	NumGroup2Layout->addWidget( Format3, 1, 1 );
	NumbGroupLayout->addWidget( NumGroup2 );
	FoFram->addWidget(NumbGroup);

	PercGroup = new QGroupBox( tab4 );
	PercGroup->setTitle( tr( "Percent Format" ) );
	PercGroupLayout = new QVBoxLayout( PercGroup );
	PercGroupLayout->setSpacing( 5 );
	PercGroupLayout->setMargin( 10 );
	PercGroupLayout->setAlignment( Qt::AlignTop );
	LayoutFN1a = new QHBoxLayout;
	LayoutFN1a->setSpacing( 5 );
	LayoutFN1a->setMargin( 0 );
	TextNu1a = new QLabel( PercGroup );
	TextNu1a->setText( tr( "Decimals:" ) );
	LayoutFN1a->addWidget( TextNu1a );
	Decim2 = new QSpinBox( PercGroup );
	Decim2->setMinimum(0);
	Decim2->setMaximum(12);
	Decim2->setValue(0);
	LayoutFN1a->addWidget( Decim2 );
	QSpacerItem* spacer_4a = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	LayoutFN1a->addItem( spacer_4a );
	PercGroupLayout->addLayout( LayoutFN1a );
	NumGroup2a = new QGroupBox( PercGroup );
	NumGroup2a->setTitle( tr( "Formatting" ) );
	NumGroup2aLayout = new QGridLayout( NumGroup2a );
	NumGroup2aLayout->setSpacing( 5 );
	NumGroup2aLayout->setMargin( 10 );
	NumGroup2aLayout->setAlignment( Qt::AlignTop );
	Format0a = new QRadioButton( NumGroup2a );
	Format0a->setText("9,999.99");
	Format0a->setChecked( true );
	FormNum = 0;
	NumGroup2aLayout->addWidget( Format0a, 0, 0 );
	Format1a = new QRadioButton( NumGroup2a );
	Format1a->setText("9999.99");
	NumGroup2aLayout->addWidget( Format1a, 1, 0 );
	Format2a = new QRadioButton( NumGroup2a );
	Format2a->setText("9.999,99");
	NumGroup2aLayout->addWidget( Format2a, 0, 1 );
	Format3a = new QRadioButton( NumGroup2a );
	Format3a->setText("9999,99");
	NumGroup2aLayout->addWidget( Format3a, 1, 1 );
	PercGroupLayout->addWidget( NumGroup2a );
	FoFram->addWidget(PercGroup);

	DateGroup = new QGroupBox( tab4 );
	DateGroup->setTitle( tr( "Date Format" ) );
	DateGroupLayout = new QVBoxLayout( DateGroup );
	DateGroupLayout->setSpacing( 5 );
	DateGroupLayout->setMargin( 10 );
	DateGroupLayout->setAlignment( Qt::AlignTop );
	LayoutFN1c = new QHBoxLayout;
	LayoutFN1c->setSpacing( 5 );
	LayoutFN1c->setMargin( 0 );
	Format0c = new QComboBox(DateGroup);
	QString tmp_form[] = {"m/d", "m/d/yy", "mm/dd/yy", "mm/yy", "d-mmm", "d-mmm-yy", "dd-mmm-yy", "yy-mm-dd",
	                    "mmm-yy", "mmmm-yy", "mmm d, yyyy", "mmmm d, yyyy", "m/d/yy h:MM tt", "m/d/yy HH:MM"};
	size_t array_form = sizeof(tmp_form) / sizeof(*tmp_form);
	/* PFJ - 28/02/04 - Altered from uint to int and var name */
	for (uint prop = 0; prop < array_form; ++prop)
		Format0c->addItem(tmp_form[prop]);
	Format0c->setEditable(false);
	LayoutFN1c->addWidget( Format0c );
	QSpacerItem* spacer_4c = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	LayoutFN1c->addItem( spacer_4c );
	DateGroupLayout->addLayout( LayoutFN1c );
	TextDa1 = new QLabel( DateGroup );
	TextDa1->setText( "" );
	DateGroupLayout->addWidget( TextDa1 );
	FoFram->addWidget(DateGroup);

	TimeGroup = new QGroupBox( tab4 );
	TimeGroup->setTitle( tr( "Time Format" ) );
	TimeGroupLayout = new QVBoxLayout( TimeGroup );
	TimeGroupLayout->setSpacing( 5 );
	TimeGroupLayout->setMargin( 10 );
	TimeGroupLayout->setAlignment( Qt::AlignTop );
	Format0b = new QRadioButton( TimeGroup );
	Format0b->setText("HH:MM");
	Format0b->setChecked( true );
	FormNum = 0;
	TimeGroupLayout->addWidget( Format0b);
	Format1b = new QRadioButton( TimeGroup );
	Format1b->setText("h:MM tt");
	TimeGroupLayout->addWidget( Format1b);
	Format2b = new QRadioButton( TimeGroup );
	Format2b->setText("HH:MM:ss");
	TimeGroupLayout->addWidget( Format2b);
	Format3b = new QRadioButton( TimeGroup );
	Format3b->setText("h:MM:ss tt");
	TimeGroupLayout->addWidget( Format3b);
	FoFram->addWidget(TimeGroup);

	GroupCust = new QGroupBox( tab4 );
	GroupCust->setTitle( tr( "Custom Scripts" ) );
	GroupCustLayout = new QVBoxLayout( GroupCust );
	GroupCustLayout->setSpacing( 5 );
	GroupCustLayout->setMargin( 10 );
	GroupCustLayout->setAlignment( Qt::AlignTop );
	FLayout3 = new QVBoxLayout;
	FLayout3->setSpacing( 5 );
	FLayout3->setMargin( 0 );
	FLayout2 = new QHBoxLayout;
	FLayout2->setSpacing( 5 );
	FLayout2->setMargin( 0 );
	TextForm2 = new QLabel( GroupCust );
	TextForm2->setText( tr( "Format:" ) );
	FLayout2->addWidget( TextForm2 );
	EditFormat = new QPushButton( GroupCust );
	EditFormat->setText( tr( "Edit..." ) );
	EditFormat->setAutoDefault( false );
	if (item->annotation().Format() != 5)
		EditFormat->setEnabled( false );
	FLayout2->addWidget( EditFormat );
	FLayout3->addLayout( FLayout2 );
	FormatScript = new QTextEdit( GroupCust );
	if (item->annotation().Format() == 5)
		FormatScript->setPlainText( item->annotation().F_act() );
	FormatScript->setReadOnly(true);
	FormatScript->setAutoFillBackground(true);
	FormatScript->setPalette(palTxt);
	FormatScript->setMaximumSize(QSize(32000,50));
	FormatScript->setFocusPolicy(Qt::NoFocus);
	FLayout3->addWidget( FormatScript );
	GroupCustLayout->addLayout( FLayout3 );
	FLayout5 = new QVBoxLayout;
	FLayout5->setSpacing( 5 );
	FLayout5->setMargin( 0 );
	FLayout4 = new QHBoxLayout;
	FLayout4->setSpacing( 5 );
	FLayout4->setMargin( 0 );
	TextForm3 = new QLabel( GroupCust );
	TextForm3->setText( tr( "Keystroke:" ) );
	FLayout4->addWidget( TextForm3 );
	EditKeystr = new QPushButton( GroupCust );
	EditKeystr->setText( tr( "Edit..." ) );
	EditKeystr->setAutoDefault( false );
	if (item->annotation().Format() != 5)
		EditKeystr->setEnabled( false );
	FLayout4->addWidget( EditKeystr );
	FLayout5->addLayout( FLayout4 );
	KeyScript = new QTextEdit( GroupCust );
	if (item->annotation().Format() == 5)
		KeyScript->setPlainText( item->annotation().K_act() );
	KeyScript->setReadOnly(true);
	KeyScript->setAutoFillBackground(true);
	KeyScript->setPalette(palTxt);
	KeyScript->setMaximumSize(QSize(32000,50));
	KeyScript->setFocusPolicy(Qt::NoFocus);
	FLayout5->addWidget( KeyScript );
	GroupCustLayout->addLayout( FLayout5 );
	FoFram->addWidget(GroupCust);

	DecodeNum();

	Layout->addWidget( FoFram );
	TabWidget2->addTab( tab4, tr( "Format" ) );
	TabWidget2->setTabEnabled(TabWidget2->indexOf(tab4), false);

	tab_4 = new QWidget( TabWidget2 );
	tabLayout_4 = new QVBoxLayout( tab_4 );
	tabLayout_4->setSpacing( 5 );
	tabLayout_4->setMargin( 10 );

	ValidateGroup = new QGroupBox( tab_4 );
	ValidateGroup->setTitle( "" );
	ValidateGroupLayout = new QVBoxLayout( ValidateGroup );
	ValidateGroupLayout->setSpacing( 5 );
	ValidateGroupLayout->setMargin( 10 );
	ValidateGroupLayout->setAlignment( Qt::AlignTop );

	NoValid = new QRadioButton( ValidateGroup );
	NoValid->setText( tr( "Value is not validated" ) );
	NoValid->setChecked( true );
	ValidateGroupLayout->addWidget( NoValid );

	VLayout1 = new QGridLayout;
	VLayout1->setSpacing( 5 );
	VLayout1->setMargin( 0 );

	SimpleValid = new QRadioButton( ValidateGroup );
	SimpleValid->setText( tr( "Value must be greater than or equal to:" ) );

	VLayout1->addWidget( SimpleValid, 0, 0 );
	TextVa = new QLabel( ValidateGroup );
	TextVa->setText( tr( "and less or equal to:" ) );
	VLayout1->addWidget( TextVa, 1, 0, Qt::AlignRight );

	MinValid = new QLineEdit( ValidateGroup );
	VLayout1->addWidget( MinValid, 0, 1 );

	MaxValid = new QLineEdit( ValidateGroup );
	VLayout1->addWidget( MaxValid, 1, 1 );
	ValidateGroupLayout->addLayout( VLayout1 );

	CustomValid = new QRadioButton( ValidateGroup );
	CustomValid->setText( tr( "Custom validate script:" ) );
	ValidateGroupLayout->addWidget( CustomValid );

	VLayout2 = new QHBoxLayout;
	VLayout2->setSpacing( 5 );
	VLayout2->setMargin( 0 );

	ValidScript = new QTextEdit( ValidateGroup );
	ValidScript->setMinimumSize( QSize( 0, 50 ) );
	ValidScript->setFocusPolicy(Qt::NoFocus);
	ValidScript->setReadOnly( true );
	ValidScript->setAutoFillBackground(true);
	ValidScript->setPalette(palTxt);
	VLayout2->addWidget( ValidScript );

	EditValScript = new QPushButton( ValidateGroup );
	EditValScript->setText( tr( "Edit..." ) );
	EditValScript->setAutoDefault( false );
	VLayout2->addWidget( EditValScript );
	ValidateGroupLayout->addLayout( VLayout2 );
	tabLayout_4->addWidget( ValidateGroup );
	TabWidget2->addTab( tab_4, tr( "Validate" ) );
	TabWidget2->setTabEnabled(TabWidget2->indexOf(tab_4), false);
	SetVali();

	tab_5 = new QWidget( TabWidget2 );
	tabLayout_5 = new QVBoxLayout( tab_5 );
	tabLayout_5->setSpacing( 5 );
	tabLayout_5->setMargin( 10 );

	CalcGroup = new QGroupBox( tab_5 );
	CalcGroup->setTitle( "" );
	CalcGroupLayout = new QVBoxLayout( CalcGroup );
	CalcGroupLayout->setSpacing( 5 );
	CalcGroupLayout->setMargin( 10 );
	CalcGroupLayout->setAlignment( Qt::AlignTop );

	NoCalc = new QRadioButton( CalcGroup );
	NoCalc->setText( tr( "Value is not calculated" ) );
	NoCalc->setChecked( true );
	CalcGroupLayout->addWidget( NoCalc );

	CLayout1 = new QHBoxLayout;
	CLayout1->setSpacing( 5 );
	CLayout1->setMargin( 0 );

	SimpleCalc = new QRadioButton( CalcGroup );
	SimpleCalc->setText( tr( "Value is the" ) );
	CLayout1->addWidget( SimpleCalc );

	CalcArt = new QComboBox( CalcGroup );
	/* PFJ - 28/02/04 - Altered to QString/size_t/for style */
	QString calc[] = {tr("sum"), tr("product"), tr("average"), tr("minimum"),
	                  tr("maximum")};
	size_t calcArray = sizeof(calc) / sizeof(*calc);
	for (uint prop = 0; prop < calcArray; ++prop)
		CalcArt->addItem(calc[prop]);
	CLayout1->addWidget( CalcArt );

	TextLabel1_2 = new QLabel( CalcGroup );
	TextLabel1_2->setText( tr( "of the following fields:" ) );
	CLayout1->addWidget( TextLabel1_2 );
	CalcGroupLayout->addLayout( CLayout1 );

	CLayout3 = new QHBoxLayout;
	CLayout3->setSpacing( 5 );
	CLayout3->setMargin( 0 );
	CalcFields = new QLineEdit( CalcGroup );
	CLayout3->addWidget( CalcFields );
	SeField = new QPushButton( CalcGroup );
	SeField->setText( tr( "Pick..." ) );
	CLayout3->addWidget( SeField );
	CalcGroupLayout->addLayout( CLayout3 );

	CustomCalc = new QRadioButton( CalcGroup );
	CustomCalc->setText( tr( "Custom calculation script:" ) );
	CalcGroupLayout->addWidget( CustomCalc );

	CLayout2 = new QHBoxLayout;
	CLayout2->setSpacing( 5 );
	CLayout2->setMargin( 0 );

	CalcScript = new QTextEdit( CalcGroup );
	CalcScript->setMinimumSize( QSize( 0, 50 ) );
	CalcScript->setFocusPolicy(Qt::NoFocus);
	CalcScript->setReadOnly( true );
	CLayout2->addWidget( CalcScript );
	CalcScript->setAutoFillBackground(true);
	CalcScript->setPalette(palTxt);

	EditCalc = new QPushButton( CalcGroup );
	EditCalc->setText( tr( "Edit..." ) );
	CLayout2->addWidget( EditCalc );
	CalcGroupLayout->addLayout( CLayout2 );
	tabLayout_5->addWidget( CalcGroup );
	TabWidget2->addTab( tab_5, tr( "Calculate" ) );
	TabWidget2->setTabEnabled(TabWidget2->indexOf(tab_5), false);
	SetCalc();
	GroupBox10Layout->addWidget( TabWidget2 );
	Fram->addWidget(GroupBox10);

	Frame9 = new QFrame( this );
	Frame9->setFrameShape( QFrame::NoFrame );
	Frame9->setFrameShadow( QFrame::Plain );
	Fram->addWidget(Frame9);

	SetZiel(item->annotation().Type()-2);
	Layout1_2 = new QHBoxLayout;
	Layout1_2->setSpacing( 5 );
	Layout1_2->setMargin( 0 );

	QSpacerItem* spacerr = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1_2->addItem( spacerr );
	PushButton1 = new QPushButton( this );
	PushButton1->setText( tr( "OK" ) );
	PushButton1->setDefault( true );
	Layout1_2->addWidget( PushButton1 );
	PushButton2 = new QPushButton( this );
	PushButton2->setText( tr( "Cancel" ) );
	Layout1_2->addWidget( PushButton2 );
	AnnotLayout->addLayout( Layout1_2 );
	connect(PushButton1, SIGNAL(clicked()), this, SLOT(SetVals()));
	connect(PushButton2, SIGNAL(clicked()), this, SLOT(reject()));
	connect(EditFormat, SIGNAL(clicked()), this, SLOT(editFormatSc()));
	connect(EditKeystr, SIGNAL(clicked()), this, SLOT(editKeySc()));
	connect(EditValScript, SIGNAL(clicked()), this, SLOT(editValidSc()));
	connect(EditCalc, SIGNAL(clicked()), this, SLOT(editCalcSc()));
	connect(EditJ, SIGNAL(clicked()), this, SLOT(editJavaSc()));
	connect(SeField, SIGNAL(clicked()), this, SLOT(SelectFelder()));
	connect(Format0c, SIGNAL(activated(const QString&)), this, SLOT(setDateSample(const QString&)));
	connect(TxFormat, SIGNAL(activated(int)), this, SLOT(SetFoScript(int)));
	connect(ComboBox1, SIGNAL(activated(int)), this, SLOT(SetZiel(int)));
	connect(ActionCombo, SIGNAL(activated(int)), this, SLOT(SetActTyp(int)));
	connect(SelAction, SIGNAL(activated(int)), this, SLOT(SetActScript(int)));
	connect(Pg1, SIGNAL(Coords(double, double)), this, SLOT(SetCo(double, double)));
	connect(SpinBox11, SIGNAL(valueChanged(int)), this, SLOT(SetPg(int)));
	connect(SpinBox21, SIGNAL(valueChanged(int)), this, SLOT(SetCross()));
	connect(SpinBox31, SIGNAL(valueChanged(int)), this, SLOT(SetCross()));
	connect(Limit, SIGNAL(clicked()), this, SLOT(SetLimit()));
	connect(UseCurr, SIGNAL(clicked()), this, SLOT(SetCurr()));
	connect(Format0, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format1, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format2, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format3, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format0a, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format1a, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format2a, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format3a, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format0b, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format1b, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format2b, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format3b, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(NoValid, SIGNAL(clicked()), this, SLOT(HandleVali()));
	connect(SimpleValid, SIGNAL(clicked()), this, SLOT(HandleVali()));
	connect(CustomValid, SIGNAL(clicked()), this, SLOT(HandleVali()));
	connect(NoCalc, SIGNAL(clicked()), this, SLOT(HandleCalc()));
	connect(SimpleCalc, SIGNAL(clicked()), this, SLOT(HandleCalc()));
	connect(CustomCalc, SIGNAL(clicked()), this, SLOT(HandleCalc()));
	connect(IconN, SIGNAL(clicked()), this, SLOT(GetNIcon()));
	connect(IconNR, SIGNAL(clicked()), this, SLOT(RemoveNIcon()));
	connect(IconP, SIGNAL(clicked()), this, SLOT(GetPIcon()));
	connect(IconPR, SIGNAL(clicked()), this, SLOT(RemovePIcon()));
	connect(IconR, SIGNAL(clicked()), this, SLOT(GetRIcon()));
	connect(IconRR, SIGNAL(clicked()), this, SLOT(RemoveRIcon()));
	connect(UseIcons, SIGNAL(clicked()), this, SLOT(IconsEin()));
	connect(PlaceIcon, SIGNAL(clicked()), this, SLOT(IPlace()));
	connect(ChFile, SIGNAL(clicked()), this, SLOT(GetFile()));
	connect(LExtern, SIGNAL(clicked()), this, SLOT(SetExternL()));
	connect(Name, SIGNAL(Leaved()), this, SLOT(NewName()));
	NoSpell->setToolTip( tr( "Flag is ignored for PDF 1.3" ) );
	NoScroll->setToolTip( tr( "Flag is ignored for PDF 1.3" ) );
	CalcFields->setToolTip( tr( "Enter a comma separated list of fields here" ) );
	IconNR->setToolTip( tr("You need at least the Icon for Normal to use Icons for Buttons"));
	SetPg(qMin(SpinBox11->value(), MaxSeite));
	SetCross();
	resize( minimumSizeHint() );
}

Annot::~Annot()
{}

void Annot::NewName()
{
	QString NameNew = Name->text();
	if (NameNew.isEmpty())
	{
		Name->setText(OldName);
		return;
	}
	bool found = false;
	for (int b = 0; b < view->Doc->Items->count(); ++b)
	{
		if ((NameNew == view->Doc->Items->at(b)->itemName()) && (view->Doc->Items->at(b) != item))
		{
			found = true;
			break;
		}
	}
	if (found)
	{
		Name->setText(OldName);
		Name->setFocus();
	}
}

void Annot::IPlace()
{
	ButtonIcon* dia = new ButtonIcon(this, item);
	if (dia->exec())
	{
		int w = item->pixm.width();
		int h = item->pixm.height();
		double sw = item->width() / w;
		double sh = item->height() / h;
		double sc = qMin(sw, sh);
		if (dia->IcScaleH == 3)
		{
			item->setImageXYScale(1.0, 1.0);
			item->setImageXYOffset((item->width()-w)*dia->IcPlaceX, (item->height()-h)*dia->IcPlaceY);
		}
		else
		{
			if (dia->ScaleH->currentIndex() == 0)
			{
				item->setImageXYScale(sc, sc);
				item->setImageXYOffset(((item->width()-w*sc)/sc)/2.0/ sc, ((item->height()-h*sc)/sc)/2.0/sc);
			}
			else
			{
				item->setImageXYScale(sw, sh);
				item->setImageXYOffset(0.0, 0.0);
			}
		}
		item->annotation().setIPlace(dia->Place->currentIndex());
		item->annotation().setScaleW(dia->ScaleW->currentIndex());
	}
	delete dia;
}

void Annot::RemoveNIcon()
{
	NiconPrev->clear();
	item->Pfile = "";
	item->PictureIsAvailable = false;
	IconNR->setEnabled(false);
}

void Annot::RemovePIcon()
{
	PiconPrev->clear();
	item->Pfile2 = "";
	IconPR->setEnabled(false);
}

void Annot::RemoveRIcon()
{
	RiconPrev->clear();
	item->Pfile3 = "";
	IconRR->setEnabled(false);
}

void Annot::IconsEin()
{
	bool setter = !UseIcons->isChecked() ? false : true;
	IconN->setEnabled(setter);
	IconP->setEnabled(setter);
	IconR->setEnabled(setter);

	NiconPrev->setEnabled(setter);
	PiconPrev->setEnabled(setter);
	RiconPrev->setEnabled(setter);
	PlaceIcon->setEnabled(setter);
	IconNR->setEnabled(!item->Pfile.isEmpty() ? true : false);
	IconPR->setEnabled(!item->Pfile2.isEmpty() ? true : false);
	IconRR->setEnabled(!item->Pfile3.isEmpty() ? true : false);
	item->annotation().setUseIcons(UseIcons->isChecked());
}

void Annot::GetNIcon()
{
	QString fileName;
	QString wdir = dirs->get("icon", ".");
	CustomFDialog dia(this, wdir, tr("Open"),
					  tr("Images (*.tif *.png *.jpg *.xpm);;%1;;All Files (*)").arg(FormatsManager::instance()->extensionsForFormat(FormatsManager::EPS)), fdShowPreview | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		dirs->set("icon", fileName.left(fileName.lastIndexOf("/")));
		QPixmap pmI1;
		CMSettings cms(view->Doc, "", 0);
		item->pixm.LoadPicture(fileName, 1, cms, false, false, ScImage::RGBData, 72);
		pmI1=QPixmap::fromImage(item->pixm.qImage());
		NiconPrev->setPixmap(pmI1);
		item->Pfile = fileName;
		item->PictureIsAvailable = true;
		int w = item->pixm.width();
		int h = item->pixm.height();
		double sw = item->width() / w;
		double sh = item->height() / h;
		double sc = qMin(sw,sh);
		item->setImageXYScale(sc, sc);
		item->setImageXYOffset(((item->width()-(w*sc))/2)/sc, ((item->height()-(h*sc))/2)/sc);
		IconNR->setEnabled(true);
	}
}

void Annot::GetPIcon()
{
	QString fileName;
	QString wdir = dirs->get("icon", ".");
	CustomFDialog dia(this, wdir, tr("Open"),
	                  tr("Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)"), fdShowPreview | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		dirs->set("icon", fileName.left(fileName.lastIndexOf("/")));
		QPixmap pmI1;
		ScImage im;
		CMSettings cms(view->Doc, "",0 );
		im.LoadPicture(fileName, 1, cms, false, false, ScImage::RGBData, 72);
		pmI1=QPixmap::fromImage(im.qImage());
		PiconPrev->setPixmap(pmI1);
		item->Pfile2 = fileName;
		IconPR->setEnabled(true);
	}
}

void Annot::GetRIcon()
{
	QString fileName;
	QString wdir = dirs->get("icon", ".");
	CustomFDialog dia(this, wdir, tr("Open"),
	                  tr("Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)"), fdShowPreview | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		dirs->set("icon", fileName.left(fileName.lastIndexOf("/")));
		QPixmap pmI1;
		ScImage im;
		CMSettings cms(view->Doc, "", 0);
		im.LoadPicture(fileName, 1, cms, false, false, ScImage::RGBData, 72);
		pmI1=QPixmap::fromImage(im.qImage());
		RiconPrev->setPixmap(pmI1);
		item->Pfile3 = fileName;
		IconRR->setEnabled(true);
	}
}

void Annot::SelectFelder()
{
	SelectFields* dia = new SelectFields(this, CalcFields->text(), item->itemName(), view->Doc, 3);
	if (dia->exec())
		CalcFields->setText(dia->S_Fields);
	delete dia;
}

void Annot::editKeySc()
{
	Editor* dia = new Editor(this, item->annotation().K_act(), view);
	if (dia->exec())
	{
		item->annotation().setK_act(dia->EditTex->toPlainText());
		KeyScript->setPlainText( item->annotation().K_act() );
	}
	delete dia;
}

void Annot::editFormatSc()
{
	Editor* dia = new Editor(this, item->annotation().F_act(), view);
	if (dia->exec())
	{
		item->annotation().setF_act(dia->EditTex->toPlainText());
		FormatScript->setPlainText( item->annotation().F_act() );
	}
	delete dia;
}

void Annot::editValidSc()
{
	Editor* dia = new Editor(this, item->annotation().V_act(), view);
	if (dia->exec())
	{
		item->annotation().setV_act(dia->EditTex->toPlainText());
		ValidScript->setPlainText( item->annotation().V_act() );
	}
	delete dia;
}

void Annot::editCalcSc()
{
	Editor* dia = new Editor(this, item->annotation().C_act(), view);
	if (dia->exec())
	{
		item->annotation().setC_act(dia->EditTex->toPlainText());
		CalcScript->setPlainText( item->annotation().C_act() );
	}
	delete dia;
}

void Annot::editJavaSc()
{
	Editor* dia = new Editor(this, EditJava->toPlainText(), view);
	if (dia->exec())
		EditJava->setPlainText(dia->EditTex->toPlainText());
	delete dia;
}

void Annot::setDateSample(const QString& ds)
{
	QDateTime dt = QDateTime::currentDateTime();
	QString tmp = "";
	if (ds == "m/d")
		tmp = "M/d";
	if (ds == "m/d/yy")
		tmp = "M/d/yy";
	if (ds == "mm/dd/yy")
		tmp = "MM/dd/yy";
	if (ds == "mm/yy")
		tmp = "MM/yy";
	if (ds == "d-mmm")
		tmp = "d-MMM";
	if (ds == "d-mmm-yy")
		tmp = "d-MMM-yy";
	if (ds == "dd-mmm-yy")
		tmp = "dd-MMM-yy";
	if (ds == "yy-mm-dd")
		tmp = "yy-MM-dd";
	if (ds == "mmm-yy")
		tmp = "MMM-yy";
	if (ds == "mmmm-yy")
		tmp = "MMMM-yy";
	if (ds == "mmm d, yyyy")
		tmp = "MMM d, yyyy";
	if (ds == "mmmm d, yyyy")
		tmp = "MMMM d, yyyy";
	if (ds == "m/d/yy h:MM tt")
		tmp = "M/d/yy h:mm ap";
	if (ds == "m/d/yy HH:MM")
		tmp = "M/d/yy hh:mm";
	TextDa1->setText( tr("Example:")+" "+dt.toString(tmp));
}

void Annot::DecodeVali()
{
	QString pfor = item->annotation().V_act();
	int ss = pfor.indexOf("(");
	QString pfo = pfor.mid(ss+1, pfor.length()-ss-2);
	QStringList pfol;
	pfol = pfo.split(",", QString::SkipEmptyParts);
	MinValid->setText(pfol[1].simplified());
	MaxValid->setText(pfol[3].simplified());
}

void Annot::DecodeCalc()
{
	QString tm = "";
	QString tm2;
	QString pfor = item->annotation().C_act();
	int ss = pfor.lastIndexOf("(");
	QString pfo = pfor.mid(ss+1, pfor.length()-ss-3);
	QStringList pfol;
	pfol = pfo.split(",", QString::SkipEmptyParts);
	if (pfol.count() > 1)
	{
		tm2 = pfol[0].simplified();
		tm += tm2.mid(1, tm2.length()-2);
		for (int cfx = 1; cfx < pfol.count(); ++cfx)
		{
			tm2 = pfol[cfx].simplified();
			tm += ", "+tm2.mid(1, tm2.length()-2);
		}
	}
	CalcFields->setText(tm);
	ss = pfor.indexOf("(");
	pfo = pfor.mid(ss+1, pfor.length()-ss-3);
	pfol = pfo.split(",", QString::SkipEmptyParts);
	/* PFJ - 28/02/04 - Let's get rid of lots of ifs.... */
	QString pf[] = {"\"SUM\"", "\"PRD\"", "\"AVG\"", "\"MIN\"", "\"MAX\""};
	size_t pfTest = sizeof(pf) / sizeof(*pf);
	for (uint test = 0; test < pfTest; ++test)
	{
		if (pfol[0] == pf[test])
			CalcArt->setCurrentIndex(test);
	}
}

void Annot::DecodeNum()
{
	QString pfor = item->annotation().F_act();
	int ss = pfor.indexOf("(");
	QString pfo = pfor.mid(ss+1, pfor.length()-ss-2);
	QStringList pfol;
	pfol = pfo.split(",", QString::SkipEmptyParts);
	if (item->annotation().Format() == 1)
		{
		Decim->setValue(pfol[0].toInt());
		switch (pfol[1].toInt())
			{
			case 0:
				Format0->setChecked(true);
				FormNum = 0;
				break;
			case 1:
				Format1->setChecked(true);
				FormNum = 1;
				break;
			case 2:
				Format2->setChecked(true);
				FormNum = 2;
				break;
			case 3:
				Format3->setChecked(true);
				FormNum = 3;
				break;
			}
		if (pfol[5] == " true")
			PreCurr->setChecked(true);
		else
			PreCurr->setChecked(false);
		if (pfol[4].length() > 2)
			{
			if (PreCurr->isChecked())
				CurSym->setText(pfol[4].mid(2,pfol[4].length()-4));
			else
				CurSym->setText(pfol[4].mid(3,pfol[4].length()-4));
			}
		else
			CurSym->setText("");
		if (!CurSym->text().isEmpty())
			{
			CurSym->setEnabled(true);
			UseCurr->setEnabled(true);
			UseCurr->setChecked(true);
			PreCurr->setEnabled(true);
			}
		}
	if (item->annotation().Format() == 2)
		{
		Decim2->setValue(pfol[0].toInt());
		switch (pfol[1].toInt())
			{
			case 0:
				Format0a->setChecked(true);
				FormNum = 0;
				break;
			case 1:
				Format1a->setChecked(true);
				FormNum = 1;
				break;
			case 2:
				Format2a->setChecked(true);
				FormNum = 2;
				break;
			case 3:
				Format3a->setChecked(true);
				FormNum = 3;
				break;
			}
		}
	if (item->annotation().Format() == 3)
	{
		setCurrentComboItem(Format0c, pfol[0].remove("\""));
		setDateSample(pfol[0]);
	}
	if (item->annotation().Format() == 4)
	{
		switch (pfol[0].toInt())
			{
			case 0:
				Format0b->setChecked(true);
				FormNum = 0;
				break;
			case 1:
				Format1b->setChecked(true);
				FormNum = 1;
				break;
			case 2:
				Format2b->setChecked(true);
				FormNum = 2;
				break;
			case 3:
				Format3b->setChecked(true);
				FormNum = 3;
				break;
			}
		}
}

void Annot::SetFormNum()
{
	switch (item->annotation().Format())
	{
	case 1:
		if (Format0->isChecked())
			FormNum = 0;
		if (Format1->isChecked())
			FormNum = 1;
		if (Format2->isChecked())
			FormNum = 2;
		if (Format3->isChecked())
			FormNum = 3;
		break;
	case 2:
		if (Format0a->isChecked())
			FormNum = 0;
		if (Format1a->isChecked())
			FormNum = 1;
		if (Format2a->isChecked())
			FormNum = 2;
		if (Format3a->isChecked())
			FormNum = 3;
		break;
	case 4:
		if (Format0b->isChecked())
			FormNum = 0;
		if (Format1b->isChecked())
			FormNum = 1;
		if (Format2b->isChecked())
			FormNum = 2;
		if (Format3b->isChecked())
			FormNum = 3;
		break;
	}
}

void Annot::HandleVali()
{
	bool setter = SimpleValid->isChecked() ? true : false;
	MaxValid->setEnabled(setter);
	MinValid->setEnabled(setter);
	EditValScript->setEnabled(false);
	if (CustomValid->isChecked())
		EditValScript->setEnabled(true);
}

void Annot::SetVali()
{
	MaxValid->setEnabled(false);
	MinValid->setEnabled(false);
	EditValScript->setEnabled(false);
	ValidScript->setEnabled(false);
	if (item->annotation().V_act().isEmpty())
		NoValid->setChecked(true);
	else
	{
		if (item->annotation().V_act().startsWith("AFRange_Validate"))
		{
			MaxValid->setEnabled(true);
			MinValid->setEnabled(true);
			SimpleValid->setChecked(true);
			DecodeVali();
		}
		else
		{
			EditValScript->setEnabled(true);
			CustomValid->setChecked(true);
			ValidScript->setPlainText(item->annotation().V_act());
		}
	}
}

void Annot::HandleCalc()
{
	bool setter = SimpleCalc->isChecked() ? true : false;
	CalcFields->setEnabled(setter);
	CalcArt->setEnabled(setter);
	EditCalc->setEnabled(false);
	SeField->setEnabled(setter);
	if (CustomCalc->isChecked())
		EditCalc->setEnabled(true);
}

void Annot::SetCalc()
{
	CalcFields->setEnabled(false);
	CalcArt->setEnabled(false);
	EditCalc->setEnabled(false);
	SeField->setEnabled(false);
	if (item->annotation().C_act().isEmpty())
		NoCalc->setChecked(true);
	else
	{
		if (item->annotation().C_act().startsWith("AFSimple_Calculate"))
		{
			CalcFields->setEnabled(true);
			CalcArt->setEnabled(true);
			SimpleCalc->setChecked(true);
			SeField->setEnabled(true);
			DecodeCalc();
		}
		else
		{
			EditCalc->setEnabled(true);
			CustomCalc->setChecked(true);
			CalcScript->setPlainText(item->annotation().C_act());
		}
	}
}

void Annot::SetCurr()
{
	bool setter = UseCurr->isChecked() ? true : false;
	CurSym->setEnabled(setter);
	PreCurr->setEnabled(setter);
}

void Annot::SetFoScript(int it)
{
	if (it >= 0 && it <=5)
		FoFram->setCurrentIndex(it);
	if (it == 1)
		SetCurr();
	if (it == 5)
	{
		EditFormat->setEnabled( true );
		EditKeystr->setEnabled( true );
		KeyScript->setPlainText("");
		FormatScript->setPlainText("");
//		KeyScript->setText( item->annotation().K_act() );
//		FormatScript->setText( item->annotation().F_act() );
	}
	item->annotation().setFormat(it);
}

void Annot::SetCo(double x, double y)
{
	SpinBox21->setValue(static_cast<int>(x*Breite));
	SpinBox31->setValue(static_cast<int>(y*Hoehe));
}

void Annot::SetPg(int v)
{
	disconnect(SpinBox11, SIGNAL(valueChanged(int)), this, SLOT(SetPg(int)));
	if ((item->annotation().ActionType() == 7) || (item->annotation().ActionType() == 9))
	{
		if (!Pg1->SetSeite(v, 100, Destfile->text()))
		{
			SpinBox11->setValue(1);
			Pg1->SetSeite(1, 100, Destfile->text());
		}
		Breite = Pg1->Breite;
		Hoehe = Pg1->Hoehe;
		//		SetCo(0,0);
	}
	else
	{
		Pg1->SetSeite(qMin(v-1, MaxSeite-1), 100);
		SpinBox11->setValue(qMin(v, MaxSeite));
		Breite = OriBreite;
		Hoehe = OriHoehe;
		//		SetCo(0,0);
	}
	SpinBox21->setMaximum(Breite);
	SpinBox31->setMaximum(Hoehe);
	connect(SpinBox11, SIGNAL(valueChanged(int)), this, SLOT(SetPg(int)));
}

void Annot::SetCross()
{
	int x,y;
	disconnect(Pg1, SIGNAL(Coords(double, double)), this, SLOT(SetCo(double, double)));
	x = static_cast<int>(static_cast<double>(SpinBox21->value())/static_cast<double>(Breite)*Pg1->pmx.width());
	y = static_cast<int>(static_cast<double>(SpinBox31->value())/static_cast<double>(Hoehe)*Pg1->pmx.height());
	Pg1->drawMark(x, y);
	connect(Pg1, SIGNAL(Coords(double, double)), this, SLOT(SetCo(double, double)));
}

void Annot::SetVals()
{
	QString tmp, tmp2;
	QString Nfo("");
	bool AAct = false;
	item->annotation().setType(ComboBox1->currentIndex()+2);
	if (Name->text() != OldName)
	{
		item->setItemName(Name->text());
		item->AutoName = false;
	}
	item->annotation().setToolTip(Tip->text());
	item->annotation().setRollOver(TextO->text());
	item->annotation().setDown(DownT->text());
	item->annotation().setBwid(BorderW->currentIndex());
	item->annotation().setBsty(BorderS->currentIndex());
	item->annotation().setFeed(ComboBox7_2->currentIndex());
	item->annotation().setVis(Visib->currentIndex());
	item->annotation().setFont(Schrift->currentIndex());
	item->annotation().setFlag(0);
	item->annotation().setIsChk(isChkd->isChecked());
	item->annotation().setChkStil(ChkStil->currentIndex());
	item->annotation().setBorderColor(BorderC->currentText());
	if (item->annotation().borderColor() == CommonStrings::tr_NoneColor)
		item->annotation().setBorderColor(CommonStrings::None);
	Limit->isChecked() ? item->annotation().setMaxChar(MaxChars->value()) : item->annotation().setMaxChar(-1);
	if (item->annotation().Type() == 2)
	{
		item->annotation().addToFlag(65536);
		if (item->Pfile.isEmpty())
			item->annotation().setUseIcons(false);
	}
	else
	{
		item->annotation().setUseIcons(false);
		if (ReadOnly->isChecked())
			item->annotation().addToFlag(1);
		if (Required->isChecked())
			item->annotation().addToFlag(2);
		if (NoExport->isChecked())
			item->annotation().addToFlag(4);
	}
	if (item->annotation().Type() == 5)
	{
		item->annotation().addToFlag(131072);
		if (CanEdit->isChecked())
			item->annotation().addToFlag(262144);
	}
	if (item->annotation().Type() == 3)
	{
		if (MultiL->isChecked())
			item->annotation().addToFlag(4096);
		if (Passwd->isChecked())
			item->annotation().addToFlag(8192);
		if (NoSpell->isChecked())
			item->annotation().addToFlag(4194304);
		if (NoScroll->isChecked())
			item->annotation().addToFlag(8388608);
	}
	if ((item->annotation().Type() == 3) || (item->annotation().Type() == 5))
	{
		if (NoValid->isChecked())
			item->annotation().setV_act("");
		if (SimpleValid->isChecked())
			item->annotation().setV_act("AFRange_Validate(true, "+MinValid->text()+", true, "+MaxValid->text()+")");
		if (CustomValid->isChecked())
			item->annotation().setV_act(ValidScript->toPlainText());
		if (NoCalc->isChecked())
			item->annotation().setC_act("");
		if (SimpleCalc->isChecked())
		{
			QString tmpCact = "AFSimple_Calculate(";
			switch (CalcArt->currentIndex())
			{
				case 0:
					tmpCact += "\"SUM\", ";
					break;
				case 1:
					tmpCact += "\"PRD\", ";
					break;
				case 2:
					tmpCact += "\"AVG\", ";
					break;
				case 3:
					tmpCact += "\"MIN\", ";
					break;
				case 4:
					tmpCact += "\"MAX\", ";
					break;
			}
			tmpCact += "new Array (";
			QStringList pfol;
			pfol = CalcFields->text().split(",", QString::SkipEmptyParts);
			if (pfol.count() > 1)
			{
				tmpCact += "\""+pfol[0].simplified()+"\"";
				for (int cfx = 1; cfx < pfol.count(); cfx++)
				{
					tmpCact += ", \""+pfol[cfx].simplified()+"\"";
				}
			}
			tmpCact += "))";
			item->annotation().setC_act(tmpCact);
		}
		if (CustomCalc->isChecked())
			item->annotation().setC_act(CalcScript->toPlainText());
		switch (TxFormat->currentIndex())
		{
			case 0:
				item->annotation().setF_act("");
				item->annotation().setK_act("");
				break;
			case 1:
				Nfo = tmp.setNum(Decim->value())+", "+tmp2.setNum(FormNum)+", 0, 0, \"";
				if (UseCurr->isChecked())
					{
					if (!PreCurr->isChecked())
						Nfo += " ";
					Nfo += CurSym->text().simplified();
					if (PreCurr->isChecked())
						Nfo += " ";
					}
				if (PreCurr->isChecked())
					Nfo += "\", true)";
				else
					Nfo += "\", false)";
				item->annotation().setF_act("AFNumber_Format("+Nfo);
				item->annotation().setK_act("AFNumber_Keystroke("+Nfo);
				break;
			case 2:
				Nfo = tmp.setNum(Decim2->value())+", "+tmp.setNum(FormNum)+")";
				item->annotation().setF_act("AFPercent_Format("+Nfo);
				item->annotation().setK_act("AFPercent_Keystroke("+Nfo);
				break;
			case 3:
				Nfo = Format0c->currentText()+"\")";
				item->annotation().setF_act("AFDate_FormatEx(\""+Nfo);
				item->annotation().setK_act("AFDate_KeystrokeEx(\""+Nfo);
				break;
			case 4:
				Nfo = tmp.setNum(FormNum)+")";
				item->annotation().setF_act("AFTime_Format("+Nfo);
				item->annotation().setK_act("AFTime_Keystroke("+Nfo);
				break;
			case 5:
				item->annotation().setF_act(FormatScript->toPlainText());
				item->annotation().setK_act(KeyScript->toPlainText());
				break;
		}
	}
	item->annotation().setAction("");
	switch (ActionCombo->currentIndex())
		{
		case 0:
			item->annotation().setActionType(0);
			break;
		case 1:
			item->annotation().setActionType(1);
			switch (ScrEdited)
				{
				case 0:
					item->annotation().setAction(EditJava->toPlainText());
					break;
				case 1:
					item->annotation().setD_act(EditJava->toPlainText());
					break;
				case 2:
					item->annotation().setE_act(EditJava->toPlainText());
					break;
				case 3:
					item->annotation().setX_act(EditJava->toPlainText());
					break;
				case 4:
					item->annotation().setFo_act(EditJava->toPlainText());
					break;
				case 5:
					item->annotation().setBl_act(EditJava->toPlainText());
					break;
				case 6:
					item->annotation().setK_act(EditJava->toPlainText());
					break;
				}
			break;
		case 2:
			if ((LExtern->isChecked()) && (!Destfile->text().isEmpty()))
			{
				item->annotation().setExtern(Destfile->text());
				if (useAbsolute->isChecked())
					item->annotation().setActionType(9);
				else
					item->annotation().setActionType(7);
			}
			else
			{
				item->annotation().setExtern("");
				item->annotation().setActionType(2);
			}
			item->annotation().setZiel(SpinBox11->value()-1);
			item->annotation().setAction(tmp.setNum(SpinBox21->value())+" "+tmp2.setNum(Hoehe-SpinBox31->value())+" 0");
			break;
		case 3:
			item->annotation().setActionType(3);
			item->annotation().setAction(SubURL->text().simplified());
			item->annotation().setHTML(SubAsHtml->isChecked());
			break;
		case 4:
			item->annotation().setActionType(4);
			break;
		case 5:
			item->annotation().setActionType(5);
			item->annotation().setAction(SubURLa->text().simplified());
			break;
		}
	if (!item->annotation().E_act().isEmpty())
		AAct = true;
	if (!item->annotation().X_act().isEmpty())
		AAct = true;
	if (!item->annotation().D_act().isEmpty())
		AAct = true;
	if (!item->annotation().Fo_act().isEmpty())
		AAct = true;
	if (!item->annotation().Bl_act().isEmpty())
		AAct = true;
	if (!item->annotation().K_act().isEmpty())
		AAct = true;
	if (!item->annotation().F_act().isEmpty())
		AAct = true;
	if (!item->annotation().V_act().isEmpty())
		AAct = true;
	if (!item->annotation().C_act().isEmpty())
		AAct = true;
	item->annotation().setAAact(AAct);
	accept();
}

void Annot::SetZiel(int it)
{
	disconnect(ActionCombo, SIGNAL(activated(int)), this, SLOT(SetActTyp(int)));
	disconnect(TxFormat, SIGNAL(activated(int)), this, SLOT(SetFoScript(int)));
	int tmpac = item->annotation().ActionType();
	if ((tmpac == 7) || (tmpac == 9))
		tmpac = 2;
	int sela = it + 2;
	TabWidget2->setTabEnabled(TabWidget2->indexOf(tab4), false);
	TabWidget2->setTabEnabled(TabWidget2->indexOf(tab_4), false);
	TabWidget2->setTabEnabled(TabWidget2->indexOf(tab_5), false);
	EditFormat->setEnabled( false );
	EditKeystr->setEnabled( false );
	SelAction->clear();
	QString tmp_selact[]={tr("Mouse Up"), tr("Mouse Down"), tr("Mouse Enter"),
	                      tr("Mouse Exit"), tr("On Focus"), tr("On Blur")};
	size_t array_sel = sizeof(tmp_selact) / sizeof(*tmp_selact);
	/* PFJ - 28/02/04 - Altered from uint to int and varname */
	for (uint prop = 0; prop < array_sel; ++prop)
		SelAction->addItem(tmp_selact[prop]);
	bool setter;
	switch (sela)
	{
	case 2:
		{
			Fram->setCurrentIndex(3);
			FramOp->setCurrentIndex(sela-2);
			ReadOnly->setEnabled(false);
			ReadOnly->setChecked(false);
			Required->setEnabled(false);
			Required->setChecked(false);
			NoExport->setEnabled(false);
			NoExport->setChecked(false);
			ActionCombo->clear();
			QString tmp_actcom[] = {tr("None", "action"), tr("JavaScript"), tr("Go To"),
			                        tr("Submit Form"), tr("Reset Form"), tr("Import Data")};
			size_t array_act = sizeof(tmp_actcom) / sizeof(*tmp_actcom);
			/* PFJ - 28/02/04 - Altered from uint to int and varname */
			for (uint prop = 0; prop < array_act; ++prop)
				ActionCombo->addItem(tmp_actcom[prop]);
			ActionCombo->setCurrentIndex(qMin(tmpac,5));
			setter = ((item->annotation().ActionType() != 7) && (item->annotation().ActionType() != 9)) ? true : false;
			Destfile->setEnabled(setter);
			ChFile->setEnabled(setter);
			SetActTyp(tmpac);
			break;
		}
	case 3:
	case 6:
	case 4:
	case 5:
	case 7:
		if ((sela == 3) || (sela == 5))
		{
			TabWidget2->setTabEnabled(TabWidget2->indexOf(tab4), true);
			TabWidget2->setTabEnabled(TabWidget2->indexOf(tab_4), true);
			TabWidget2->setTabEnabled(TabWidget2->indexOf(tab_5), true);
			TxFormat->setCurrentIndex(item->annotation().Format());
			SetFoScript(item->annotation().Format());
			SetVali();
			SetCalc();
		}
		if (sela == 6)
			SelAction->addItem( tr( "Selection Change" ) );
		ReadOnly->setEnabled(true);
		ReadOnly->setChecked(item->annotation().Flag() & 1);
		Required->setChecked(item->annotation().Flag() & 2);
		NoExport->setChecked(item->annotation().Flag() & 4);
		Fram->setCurrentIndex(3);
		sela > 5 ? FramOp->setCurrentIndex(3) : FramOp->setCurrentIndex(sela-2);
		ActionCombo->clear();
		ActionCombo->addItem( tr( "None" ) );
		ActionCombo->addItem( tr( "JavaScript" ) );
		ActionCombo->setCurrentIndex(qMin(tmpac, 1));
		SetActTyp(tmpac);
		break;
	default:
		Fram->setCurrentIndex(2);
		break;
	}
	MultiL->setChecked(item->annotation().Flag() & 4096);
	Passwd->setChecked(item->annotation().Flag() & 8192);
	CanEdit->setChecked(item->annotation().Flag() & 262144);
	NoSpell->setChecked(item->annotation().Flag() & 4194304);
	NoScroll->setChecked(item->annotation().Flag() & 8388608);
	ChkStil->setCurrentIndex(item->annotation().ChkStil());
	isChkd->setChecked(item->annotation().IsChk());
	setter = item->annotation().MaxChar() != -1 ? true : false;
	MaxChars->setValue(setter == true ? item->annotation().MaxChar() : 0);
	Limit->setChecked(setter);
	MaxChars->setEnabled(setter);
	connect(ActionCombo, SIGNAL(activated(int)), this, SLOT(SetActTyp(int)));
	connect(TxFormat, SIGNAL(activated(int)), this, SLOT(SetFoScript(int)));
}

void Annot::SetLimit()
{
	Limit->isChecked() ? MaxChars->setEnabled(true) :MaxChars->setEnabled(false);
}

void Annot::SetExternL()
{
	disconnect(LExtern, SIGNAL(clicked()), this, SLOT(SetExternL()));
	bool enable;
	if (!LExtern->isChecked())
	{
		item->annotation().setActionType(2);
		enable = false;
		//		Destfile->setEnabled(false);
		//		ChFile->setEnabled(false);
		SetPg(qMin(SpinBox11->value(), MaxSeite));
	}
	else
	{
		if (useAbsolute->isChecked())
			item->annotation().setActionType(9);
		else
			item->annotation().setActionType(7);
		enable = true;
		//		Destfile->setEnabled(true);
		//		ChFile->setEnabled(true);
		if (Destfile->text().isEmpty())
		{
			GetFile();
			if (Destfile->text().isEmpty())
			{
				item->annotation().setActionType(2);
				enable = false;
				//				Destfile->setEnabled(false);
				//				ChFile->setEnabled(false);
				LExtern->setChecked(false);
			}
		}
		SetPg(qMin(SpinBox11->value(), MaxSeite));
	}
	Destfile->setEnabled(enable);
	ChFile->setEnabled(enable);
	connect(LExtern, SIGNAL(clicked()), this, SLOT(SetExternL()));
}

void Annot::SetActTyp(int it)
{
	bool setter;
	switch (it)
	{
	case 5:
		Fram2->setCurrentIndex(4);
		SubURLa->setText(item->annotation().Action());
		break;
	case 3:
		Fram2->setCurrentIndex(3);
		SubURL->setText(item->annotation().Action());
		SubAsHtml->setChecked(item->annotation().HTML());
		break;
	case 2:
		Fram2->setCurrentIndex(2);
		setter = item->annotation().ActionType() != 7 ? true : false;
		Destfile->setEnabled(setter);
		ChFile->setEnabled(setter);
		SetPg(qMin(SpinBox11->value(), MaxSeite));
		break;
	case 1:
		Fram2->setCurrentIndex(1);
		SelAction->setCurrentIndex(ScrEdited);
		break;
	default:
		Fram2->setCurrentIndex(0);
		break;
	}
}

void Annot::SetActScript(int it)
{
	switch (ScrEdited)
	{
	case 0:
		item->annotation().setAction(EditJava->toPlainText());
		break;
	case 1:
		item->annotation().setD_act(EditJava->toPlainText());
		break;
	case 2:
		item->annotation().setE_act(EditJava->toPlainText());
		break;
	case 3:
		item->annotation().setX_act(EditJava->toPlainText());
		break;
	case 4:
		item->annotation().setFo_act(EditJava->toPlainText());
		break;
	case 5:
		item->annotation().setBl_act(EditJava->toPlainText());
		break;
	case 6:
		item->annotation().setK_act(EditJava->toPlainText());
		break;
	}
	switch (it)
	{
	case 0:
		EditJava->setPlainText(item->annotation().Action());
		break;
	case 1:
		EditJava->setPlainText(item->annotation().D_act());
		break;
	case 2:
		EditJava->setPlainText(item->annotation().E_act());
		break;
	case 3:
		EditJava->setPlainText(item->annotation().X_act());
		break;
	case 4:
		EditJava->setPlainText(item->annotation().Fo_act());
		break;
	case 5:
		EditJava->setPlainText(item->annotation().Bl_act());
		break;
	case 6:
		EditJava->setPlainText(item->annotation().K_act());
		break;
	}
	ScrEdited = it;
}

void Annot::GetFile()
{
	QString fn;
	QString wdir = dirs->get("annot_getfile", ".");
	CustomFDialog dia(this, wdir, tr("Open"), tr("PDF Files (*.pdf);;All Files (*)"));
	if (!Destfile->text().isEmpty())
		dia.setSelection(Destfile->text());
	if (dia.exec() == QDialog::Accepted)
	{
		fn = dia.selectedFile();
		if (!fn.isEmpty())
		{
			dirs->set("annot_getfile", fn.left(fn.lastIndexOf("/")));
			Destfile->setText(fn);
			SpinBox11->setValue(1);
			SpinBox11->setMaximum(1000);
			SetPg(1);
		}
	}
}
