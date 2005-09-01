/****************************************************************************
** Form implementation generated from reading ui file 'Druck.ui'
**
** Created: Mon Sep 17 21:42:19 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "druck.h"
#include "druck.moc"

#include "scconfig.h"

#include "commonstrings.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "customfdialog.h"
#include "cupsoptions.h"
#include <qtextstream.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qmap.h>
#ifdef HAVE_CUPS
#include <cups/cups.h>
#endif
#ifdef HAVE_CMS
extern bool CMSuse;
#endif
#include <util.h>

AdvOptions::AdvOptions(QWidget* parent, bool Hm, bool Vm, bool Ic, int ps, bool DoGcr, bool doDev, bool doSpot) : QDialog( parent, "prin", true, 0 )
{
	setCaption( tr( "Advanced Options" ) );
	setIcon(loadIcon("AppIcon.png"));
	AdvOptionsLayout = new QVBoxLayout( this );
	AdvOptionsLayout->setSpacing( 5 );
	AdvOptionsLayout->setMargin( 10 );
	MirrorH = new QCheckBox( tr("Mirror Page(s) &Horizontal"), this, "MirrorH");
	MirrorH->setChecked(Hm);
	AdvOptionsLayout->addWidget( MirrorH );
	MirrorV = new QCheckBox( tr("Mirror Page(s) &Vertical"), this, "MirrorV");
	MirrorV->setChecked(Vm);
	AdvOptionsLayout->addWidget( MirrorV );
	GcR = new QCheckBox( tr("Apply Under Color &Removal"), this, "GCR");
	GcR->setChecked(DoGcr);
	AdvOptionsLayout->addWidget( GcR );
	spotColors = new QCheckBox( tr("Convert Spot Colors to Process Colors"), this, "spotColors");
	spotColors->setChecked(!doSpot);
	AdvOptionsLayout->addWidget( spotColors );
	devPar = new QCheckBox( tr("Set Media Size"), this, "devPar");
	devPar->setChecked(doDev);
	AdvOptionsLayout->addWidget( devPar );
#ifdef HAVE_CMS
	if (CMSuse)
	{
		UseICC = new QCheckBox( tr("Apply &ICC Profiles"), this, "ICC");
		UseICC->setChecked(Ic);
		AdvOptionsLayout->addWidget( UseICC );
	}
#endif
	ButtonGroupP = new QButtonGroup( "", this, "ButtonGroup5" );
	ButtonGroupP->setFrameShape( QButtonGroup::NoFrame );
	ButtonGroupP->setColumnLayout(0, Qt::Vertical );
	ButtonGroupP->layout()->setSpacing( 0 );
	ButtonGroupP->layout()->setMargin( 0 );
	ButtonGroupPLayout = new QVBoxLayout( ButtonGroupP->layout() );
	ButtonGroupPLayout->setSpacing( 6 );
	ButtonGroupPLayout->setMargin( 0 );
	PS1 = new QRadioButton( tr( "PostScript Level &1" ), ButtonGroupP, "RadioButton1" );
	PS2 = new QRadioButton( tr( "PostScript Level &2" ), ButtonGroupP, "RadioButton1" );
	PS3 = new QRadioButton( tr( "PostScript Level &3" ), ButtonGroupP, "RadioButton1" );
	PS3->setChecked( true );
	if (ps == 3)
		PS3->setChecked( true );
	if (ps == 2)
		PS2->setChecked( true );
	if (ps == 1)
		PS1->setChecked( true );
	ButtonGroupPLayout->addWidget( PS1 );
	ButtonGroupPLayout->addWidget( PS2 );
	ButtonGroupPLayout->addWidget( PS3 );
	AdvOptionsLayout->addWidget( ButtonGroupP );

	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );
	PushButton1 = new QPushButton( CommonStrings::tr_OK, this, "PushButton1" );
	Layout2->addWidget( PushButton1 );
	PushButton2 = new QPushButton( CommonStrings::tr_Cancel, this, "PushButton1_2" );
	PushButton2->setDefault( true );
	PushButton2->setFocus();
	Layout2->addWidget( PushButton2 );
	AdvOptionsLayout->addLayout( Layout2 );
	setMinimumSize( sizeHint() );
	QToolTip::add( PS3, tr( "Creates PostScript Level 3" ) );
	QToolTip::add( PS2, tr( "Creates PostScript Level 2 only, beware,\nthis can create huge files" ) );
	QToolTip::add( PS1, tr( "Creates PostScript Level 1 only, beware,\nthis can create huge files" ) );
	QToolTip::add( GcR, tr( "A way of switching off some of the gray shades which are composed\n"
	                                   "of cyan, yellow and magenta and using black instead.\n"
									   "UCR most affects parts of images which are neutral and/or dark tones\n"
									   "which are close to the gray. Use of this may improve printing some images\n"
									   "and some experimentation and testing is need on a case by case basis.\n"
									   "UCR reduces the possibility of over saturation with CMY inks." ) );

	connect( PushButton2, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( PushButton1, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

Druck::Druck( QWidget* parent, QString PDatei, QString PDev, QString PCom, bool gcr, QStringList spots)
		: QDialog( parent, "Dr", true, 0)
{
	prefs = PrefsManager::instance()->prefsFile->getContext("print_options");
	PrinterOpts = "";
	setCaption( tr( "Setup Printer" ) );
	setIcon(loadIcon("AppIcon.png"));
	DruckLayout = new QVBoxLayout( this );
	DruckLayout->setSpacing( 6 );
	DruckLayout->setMargin( 10 );

	Drucker = new QGroupBox( this, "Drucker" );
	Drucker->setTitle( tr( "Print Destination" ) );
	Drucker->setColumnLayout(0, Qt::Vertical );
	Drucker->layout()->setSpacing( 0 );
	Drucker->layout()->setMargin( 0 );
	DruckerLayout = new QGridLayout( Drucker->layout() );
	DruckerLayout->setAlignment( Qt::AlignTop );
	DruckerLayout->setSpacing( 6 );
	DruckerLayout->setMargin( 10 );

	Layout1x = new QHBoxLayout;
	Layout1x->setSpacing( 6 );
	Layout1x->setMargin( 0 );
	PrintDest = new QComboBox( true, Drucker, "PrintDest" );
	PrintDest->setMinimumSize( QSize( 250, 22 ) );
	PrintDest->setMaximumSize( QSize( 260, 30 ) );
	PrintDest->setEditable(false);
	QString Pcap;
	QString tmp;
	QStringList wt;
#ifdef HAVE_CUPS
	cups_dest_t *dests;
	int num_dests = cupsGetDests(&dests);
	for (int pr = 0; pr < num_dests; ++pr)
	{
		tmp = QString(dests[pr].name);
		PrintDest->insertItem(tmp);
		if (tmp == PDev)
		{
			PrintDest->setCurrentItem(PrintDest->count()-1);
			ToFile = false;
		}
	}
	cupsFreeDests(num_dests, dests);
#else
	if (loadText("/etc/printcap", &Pcap))
	{
		QTextStream ts(&Pcap, IO_ReadOnly);
		while(!ts.atEnd())
		{
			tmp = ts.readLine();
			if (tmp.isEmpty())
				continue;
			if ((tmp[0] != '#') && (tmp[0] != ' ') && (tmp[0] != '\n') && (tmp[0] != '\t'))
			{
				tmp = tmp.stripWhiteSpace();
				tmp = tmp.left(tmp.length() - (tmp.right(2) == ":\\" ? 2 : 1));
				wt = QStringList::split("|", tmp);
				PrintDest->insertItem(wt[0]);
				if (wt[0] == PDev)
				{
					PrintDest->setCurrentItem(PrintDest->count()-1);
					ToFile = false;
				}
			}
		}
	}
#endif
	PrintDest->insertItem( tr("File"));
	if (PDev.isEmpty())
	{
		Geraet = PrintDest->text(0);
		ToFile = false;
	}
	else
		Geraet = PDev;

	Layout1x->addWidget( PrintDest );

#ifdef HAVE_CUPS
	OptButton = new QPushButton( tr( "&Options..." ), Drucker, "Optionen" );
	Layout1x->addWidget( OptButton );
#endif
	QSpacerItem* spacerDR = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1x->addItem( spacerDR );
	DruckerLayout->addLayout( Layout1x, 0, 0);
	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 6 );
	Layout1->setMargin( 0 );
	LineEdit1 = new QLineEdit( PDatei, Drucker, "LineEdit1" );
	LineEdit1->setMinimumSize( QSize( 240, 22 ) );
	LineEdit1->setEnabled(false);
	DateiT = new QLabel( LineEdit1, tr( "&File:" ), Drucker, "DateiT" );
	DateiT->setEnabled( false );
	Layout1->addWidget( DateiT );
	Layout1->addWidget( LineEdit1 );
	ToolButton1 = new QToolButton( Drucker, "ToolButton1" );
	ToolButton1->setText( tr( "C&hange..." ) );
	ToolButton1->setMinimumSize( QSize( 80, 22 ) );
	ToolButton1->setEnabled(false);
	ToolButton1->setFocusPolicy( QToolButton::TabFocus );
	Layout1->addWidget( ToolButton1 );
	DruckerLayout->addLayout( Layout1, 1, 0 );

	OtherCom = new QCheckBox( tr("A&lternative Printer Command"), Drucker, "Dc");
	OtherCom->setChecked(false);
	QToolTip::add( OtherCom, tr( "Use an alternative print manager, such as kprinter or gtklp,\nto utilize additional printing options") );
	DruckerLayout->addWidget( OtherCom, 2, 0, Qt::AlignLeft);
	LayoutCC = new QHBoxLayout;
	LayoutCC->setSpacing( 6 );
	LayoutCC->setMargin( 0 );
	Command = new QLineEdit( PCom, Drucker, "LineEdit12" );
	Command->setMinimumSize( QSize( 240, 22 ) );
	Command->setEnabled(false);
	OthText = new QLabel( Command, tr( "Co&mmand:" ), Drucker, "DateiTc" );
	OthText->setEnabled( false );
	LayoutCC->addWidget( OthText );
	LayoutCC->addWidget( Command );
	DruckerLayout->addLayout( LayoutCC, 3, 0 );
	DruckLayout->addWidget( Drucker );


	Umfang = new QButtonGroup( tr( "Range" ), this, "Umfang" );
	Umfang->setColumnLayout(0, Qt::Vertical );
	Umfang->layout()->setSpacing( 0 );
	Umfang->layout()->setMargin( 0 );
	UmfangLayout = new QHBoxLayout( Umfang->layout() );
	UmfangLayout->setAlignment( Qt::AlignTop );
	UmfangLayout->setSpacing( 6 );
	UmfangLayout->setMargin( 5 );

	ButtonGroup5 = new QButtonGroup( "", Umfang, "ButtonGroup5" );
	ButtonGroup5->setFrameShape( QButtonGroup::NoFrame );
	ButtonGroup5->setColumnLayout(0, Qt::Vertical );
	ButtonGroup5->layout()->setSpacing( 0 );
	ButtonGroup5->layout()->setMargin( 0 );
	ButtonGroup5Layout = new QGridLayout( ButtonGroup5->layout() );
	ButtonGroup5Layout->setAlignment( Qt::AlignTop );
	ButtonGroup5Layout->setSpacing( 6 );
	ButtonGroup5Layout->setMargin( 5 );
	RadioButton1 = new QRadioButton( tr( "Print &All" ), ButtonGroup5, "RadioButton1" );
	RadioButton1->setChecked( true );
	ButtonGroup5Layout->addMultiCellWidget( RadioButton1, 0, 0, 0, 1 );
	CurrentPage = new QRadioButton( tr( "Print Current Pa&ge" ), ButtonGroup5, "RadioButton2" );
	ButtonGroup5Layout->addMultiCellWidget( CurrentPage, 1, 1, 0, 1 );
	RadioButton2 = new QRadioButton( tr( "Print &Range" ), ButtonGroup5, "RadioButton2" );
	ButtonGroup5Layout->addMultiCellWidget( RadioButton2, 2, 2, 0, 1 );
	PageNr = new QLineEdit( ButtonGroup5, "PageNr" );
	PageNr->setEnabled(false);
	QToolTip::add( PageNr, tr( "Insert a comma separated list of tokens where\n"
		                           "a token can be * for all the pages, 1-5 for\n"
		                           "a range of pages or a single page number.") );
	ButtonGroup5Layout->addWidget( PageNr, 3, 1 );
	UmfangLayout->addWidget( ButtonGroup5 );

	ButtonGroup4 = new QButtonGroup( Umfang, "ButtonGroup4" );
	ButtonGroup4->setFrameShape( QButtonGroup::NoFrame );
	ButtonGroup4->setTitle( "" );
	ButtonGroup4->setColumnLayout(0, Qt::Vertical );
	ButtonGroup4->layout()->setSpacing( 0 );
	ButtonGroup4->layout()->setMargin( 0 );
	ButtonGroup4Layout = new QGridLayout( ButtonGroup4->layout() );
	ButtonGroup4Layout->setAlignment( Qt::AlignTop );
	ButtonGroup4Layout->setSpacing( 6 );
	ButtonGroup4Layout->setMargin( 5 );


	Copies = new QSpinBox( ButtonGroup4, "Copies" );
	Copies->setEnabled( true );
	Copies->setMinimumSize( QSize( 70, 22 ) );
	Copies->setMinValue( 1 );
	Copies->setMaxValue(1000);
	Copies->setValue(1);
	TextLabel3 = new QLabel( Copies, tr( "N&umber of Copies:" ), ButtonGroup4, "TextLabel3" );
	ButtonGroup4Layout->addWidget( TextLabel3, 0, 0 );
	ButtonGroup4Layout->addWidget( Copies, 0, 1 );
	UmfangLayout->addWidget( ButtonGroup4 );
	DruckLayout->addWidget( Umfang );

	Optionen = new QButtonGroup( this, "Optionen" );
	Optionen->setTitle( tr( "Options" ) );
	Optionen->setColumnLayout(0, Qt::Vertical );
	Optionen->layout()->setSpacing( 0 );
	Optionen->layout()->setMargin( 0 );
	OptionenLayout = new QGridLayout( Optionen->layout() );
	OptionenLayout->setAlignment( Qt::AlignTop );
	OptionenLayout->setSpacing( 5 );
	OptionenLayout->setMargin( 5 );

	ButtonGroup3 = new QButtonGroup( Optionen, "ButtonGroup3" );
	ButtonGroup3->setGeometry( QRect( 11, 19, 138, 95 ) );
	ButtonGroup3->setFrameShape( QButtonGroup::NoFrame );
	ButtonGroup3->setTitle( "" );
	ButtonGroup3->setColumnLayout(0, Qt::Vertical );
	ButtonGroup3->layout()->setSpacing( 0 );
	ButtonGroup3->layout()->setMargin( 0 );
	ButtonGroup3Layout = new QVBoxLayout( ButtonGroup3->layout() );
	ButtonGroup3Layout->setAlignment( Qt::AlignTop );
	ButtonGroup3Layout->setSpacing( 5 );
	ButtonGroup3Layout->setMargin( 5 );

	NormalP = new QRadioButton( tr( "Print &Normal" ), ButtonGroup3, "NormalP" );
	NormalP->setChecked( true );
	ButtonGroup3Layout->addWidget( NormalP );

	PrintSep = new QRadioButton( tr( "Print &Separations" ), ButtonGroup3, "PrintSep" );
	PrintSep->setFocusPolicy( QRadioButton::TabFocus );
	ButtonGroup3Layout->addWidget( PrintSep );
	ToSeparation = false;
	doSpot = true;
	SepArt = new QComboBox( true, ButtonGroup3, "SepArt" );
	/* PFJ - 29.02.04 - Altered to QString, size_t, for */
	QString sep[] =
	    {
	        tr("All"), tr("Cyan"), tr("Magenta"), tr("Yellow"),
	        tr("Black")
	    };
	size_t sepArray = sizeof(sep) / sizeof(*sep);
	for (uint prop = 0; prop < sepArray; ++prop)
		SepArt->insertItem(sep[prop]);
	SepArt->insertStringList(spots);
	SepArt->setEnabled( false );
	SepArt->setEditable( false );
	ButtonGroup3Layout->addWidget( SepArt );
	OptionenLayout->addWidget( ButtonGroup3, 0, 0, Qt::AlignLeft );

	ButtonGroup3_2 = new QButtonGroup( Optionen, "ButtonGroup3_2" );
	ButtonGroup3_2->setFrameShape( QButtonGroup::NoFrame );
	ButtonGroup3_2->setTitle( "" );
	ButtonGroup3_2->setColumnLayout(0, Qt::Vertical );
	ButtonGroup3_2->layout()->setSpacing( 0 );
	ButtonGroup3_2->layout()->setMargin( 0 );
	ButtonGroup3_2Layout = new QVBoxLayout( ButtonGroup3_2->layout() );
	ButtonGroup3_2Layout->setAlignment( Qt::AlignTop );
	ButtonGroup3_2Layout->setSpacing( 5 );
	ButtonGroup3_2Layout->setMargin( 5 );

	PrintGray = new QRadioButton( tr( "Pr&int In Color If Available" ), ButtonGroup3_2, "PrintGray" );
	PrintGray->setChecked( true );
	PrintGray->setFocusPolicy( QRadioButton::TabFocus );
	ButtonGroup3_2Layout->addWidget( PrintGray );

	PrintGray2 = new QRadioButton( tr( "Print In Gra&yscale" ), ButtonGroup3_2, "PrintGray2" );
	ButtonGroup3_2Layout->addWidget( PrintGray2 );
	MirrorH = prefs->getBool("MirrorH", false);
	MirrorV = prefs->getBool("MirrorV", false);
	ICCinUse = prefs->getBool("ICCinUse", false);
	DoGCR = prefs->getBool("DoGCR", gcr);
	doDev = prefs->getBool("doDev", false);
	PSLevel = prefs->getInt("PSLevel", 3);
	AdvOptButton = new QPushButton( tr("Ad&vanced Options..."), ButtonGroup3_2, "Adv");
	ButtonGroup3_2Layout->addWidget( AdvOptButton );

	OptionenLayout->addWidget( ButtonGroup3_2, 0, 1 );
	DruckLayout->addWidget( Optionen );

	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 24 );
	Layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );

	OKButton = new QPushButton( tr( "&Print" ), this, "OKButton" );
	OKButton->setDefault( true );
	Layout2->addWidget( OKButton );
	OKButton_2 = new QPushButton( CommonStrings::tr_Cancel, this, "OKButton_2" );
	OKButton_2->setDefault( false );
	Layout2->addWidget( OKButton_2 );

	DruckLayout->addLayout( Layout2 );
	if ((PDev== tr("File")) || (PrintDest->count() == 1))
	{
		PrintDest->setCurrentItem(PrintDest->count()-1);
		DateiT->setEnabled(true);
		LineEdit1->setEnabled(true);
		ToolButton1->setEnabled(true);
		ToFile = true;
	}
	setMaximumSize(sizeHint());
	//tab order
#ifdef HAVE_CUPS
	setTabOrder( PrintDest, OptButton );
	setTabOrder( OptButton, LineEdit1);
#else
	setTabOrder( PrintDest, LineEdit1 );
#endif
	setTabOrder( LineEdit1,ToolButton1);
	setTabOrder( ToolButton1,OtherCom);
	setTabOrder( OtherCom, Command );
	setTabOrder( Command, RadioButton1 );
	setTabOrder( RadioButton1, CurrentPage );
	setTabOrder( CurrentPage, RadioButton2 );
	setTabOrder( RadioButton2, Copies );
	setTabOrder( Copies, NormalP );
	setTabOrder( NormalP, PrintSep );
	setTabOrder( PrintSep, SepArt );
	setTabOrder( SepArt, PrintGray );
	setTabOrder( PrintGray, PrintGray2 );
	setTabOrder( PrintGray2, AdvOptButton );
	setTabOrder( AdvOptButton, OKButton );
	setTabOrder( OKButton, OKButton_2 );
//	setTabOrder( OKButton_2, PrintDest );
	PrintDest->setFocus();
	// signals and slots connections
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( okButtonClicked() ) );
	connect( OKButton_2, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( PrintDest, SIGNAL(activated(const QString&)), this, SLOT(SelPrinter(const QString&)));
	connect( RadioButton1, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect( CurrentPage, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect( NormalP, SIGNAL(toggled(bool)), this, SLOT(SelMode(bool)));
	connect( ToolButton1, SIGNAL(clicked()), this, SLOT(SelFile()));
	connect( OtherCom, SIGNAL(clicked()), this, SLOT(SelComm()));
	connect( AdvOptButton, SIGNAL( clicked() ), this, SLOT( SetAdvOptions() ) );
#ifdef HAVE_CUPS
	connect( OptButton, SIGNAL( clicked() ), this, SLOT( SetOptions() ) );
#endif
	setStoredValues();
}

void Druck::SetAdvOptions()
{
	AdvOptions* dia = new AdvOptions(this, MirrorH, MirrorV, ICCinUse, PSLevel, DoGCR, doDev, doSpot);
	if (dia->exec())
	{
		MirrorH = dia->MirrorH->isChecked();
		MirrorV = dia->MirrorV->isChecked();
		DoGCR = dia->GcR->isChecked();
		doDev = dia->devPar->isChecked();
		doSpot = !dia->spotColors->isChecked();
#ifdef HAVE_CMS
		if (CMSuse)
		{
			ICCinUse = dia->UseICC->isChecked();
			prefs->set("ICCinUse", ICCinUse);
		}
#endif
		if (dia->PS1->isChecked())
			PSLevel = 1;
		if (dia->PS2->isChecked())
			PSLevel = 2;
		if (dia->PS3->isChecked())
			PSLevel = 3;
		prefs->set("MirrorH", MirrorH);
		prefs->set("MirrorV", MirrorV);
		prefs->set("DoGCR", DoGCR);
		prefs->set("PSLevel", PSLevel);
	}
	delete dia;
}

void Druck::SetOptions()
{
	PrinterOpts = "";
	CupsOptions* dia = new CupsOptions(this, Geraet);
	if (dia->exec())
	{
		struct CupsOptions::OpData daten;
		QMap<QString,CupsOptions::OpData>::Iterator it;
		for (it = dia->KeyToText.begin(); it != dia->KeyToText.end(); ++it)
		{
			if (dia->KeyToDefault[it.key()] != dia->FlagsOpt.at(it.data().Cnum)->currentText())
			{
				if (it.data().KeyW == "mirror")
					PrinterOpts += " -o mirror";
				else
				{
					if (it.data().KeyW == "page-set")
					{
						PrinterOpts += " -o "+it.data().KeyW+"=";
						if (dia->FlagsOpt.at(it.data().Cnum)->currentItem() == 1)
							PrinterOpts += "even";
						else
							PrinterOpts += "odd";
					}
					else
					{
						if (it.data().KeyW == "number-up")
						{
							PrinterOpts += " -o "+it.data().KeyW+"=";
							switch (dia->FlagsOpt.at(it.data().Cnum)->currentItem())
							{
							case 1:
								PrinterOpts += "2";
								break;
							case 2:
								PrinterOpts += "4";
								break;
							case 3:
								PrinterOpts += "6";
								break;
							case 4:
								PrinterOpts += "9";
								break;
							case 5:
								PrinterOpts += "16";
								break;
							}
						}
						else
						{
							if (it.data().KeyW == "orientation")
								PrinterOpts += " -o landscape";
							else
								PrinterOpts += " -o " +
								               it.data().KeyW+"="+dia->FlagsOpt.at(it.data().Cnum)->currentText();
						}
					}
				}
			}
		}
	}
	delete dia;
}

void Druck::SelComm()
{
	/* PFJ - 29.02.04 - removed OthText, Command and PrintDest from switch */
	bool test = OtherCom->isChecked() ? true : false;
	OthText->setEnabled(test);
	Command->setEnabled(test);
	PrintDest->setEnabled(!test);
	if (OtherCom->isChecked())
	{
		DateiT->setEnabled(false);
		LineEdit1->setEnabled(false);
		ToolButton1->setEnabled(false);
		ToFile = false;
#ifdef HAVE_CUPS
		OptButton->setEnabled(false);
#endif

	}
	else
	{
		SelPrinter(PrintDest->currentText());
#ifdef HAVE_CUPS
		if (Geraet != tr("File"))
			OptButton->setEnabled(true);
#endif

	}
}

void Druck::SelPrinter(const QString& prn)
{
	bool setter = prn == tr("File") ? true : false;
	DateiT->setEnabled(setter);
	LineEdit1->setEnabled(setter);
	ToolButton1->setEnabled(setter);
	ToFile = setter;
#ifdef HAVE_CUPS
	OptButton->setEnabled(!setter);
#endif
	Geraet = prn;
}

void Druck::SelRange(bool e)
{
	PageNr->setEnabled(!e);
}

void Druck::SelMode(bool e)
{
	SepArt->setEnabled( !e );
	ToSeparation = !e;
}

void Druck::SelFile()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("druck", ".");
	CustomFDialog dia(this, wdir, tr("Save as"), tr("PostScript Files (*.ps);;All Files (*)"), false, false);
	if (!LineEdit1->text().isEmpty())
		dia.setSelection(LineEdit1->text());
	if (dia.exec() == QDialog::Accepted)
	{
		QString selectedFile = dia.selectedFile();
		dirs->set("druck", selectedFile.left(selectedFile.findRev("/")));
		LineEdit1->setText(selectedFile);
	}
}

void Druck::setMinMax(int min, int max, int cur)
{
	QString tmp, tmp2;
	CurrentPage->setText( tr( "Print Current Pa&ge" )+" ("+tmp.setNum(cur)+")");
	PageNr->setText(tmp.setNum(min)+"-"+tmp2.setNum(max));
}

void Druck::okButtonClicked()
{
	prefs->set("PrintDest", PrintDest->currentItem());
	prefs->set("OtherCom", OtherCom->isChecked());
	prefs->set("PrintAll", RadioButton1->isChecked());
	prefs->set("CurrentPage", CurrentPage->isChecked());
	prefs->set("PrintRange", RadioButton2->isChecked());
	prefs->set("PageNr", PageNr->text());
	prefs->set("Copies", Copies->value());
	prefs->set("NormalP", NormalP->isChecked());
	prefs->set("PrintSep", PrintSep->isChecked());
	prefs->set("PrintGray", PrintGray->isChecked());
	prefs->set("PrintGray2", PrintGray2->isChecked());
	prefs->set("SepArt", SepArt->currentItem());
	accept();	
}

void Druck::setStoredValues()
{
	int selectedDest = prefs->getInt("PrintDest", 0);
	if ((selectedDest > -1) && (selectedDest < PrintDest->count()))
	{
		PrintDest->setCurrentItem(selectedDest);
		if (PrintDest->currentText() == tr("File"))
			SelPrinter( tr("File"));
	}
	OtherCom->setChecked(prefs->getBool("OtherCom", false));
	if (OtherCom->isChecked())
		SelComm();
	RadioButton1->setChecked(prefs->getBool("PrintAll", true));
	CurrentPage->setChecked(prefs->getBool("CurrentPage", false));
	RadioButton2->setChecked(prefs->getBool("PrintRange", false));
	PageNr->setText(prefs->get("PageNr", "1-1"));
	Copies->setValue(prefs->getInt("Copies", 1));
	NormalP->setChecked(prefs->getBool("NormalP", true));
	PrintSep->setChecked(prefs->getBool("PrintSep", false));
	PrintGray->setChecked(prefs->getBool("PrintGray", true));
	PrintGray2->setChecked(prefs->getBool("PrintGray2", false));
	int selectedSep = prefs->getInt("SepArt", 0);
	if ((selectedSep > -1) && (selectedSep < 5))
		SepArt->setCurrentItem(selectedSep);
}

QString Druck::printerName()
{
	return Geraet;
}

QString Druck::outputFileName()
{
	return LineEdit1->text();
}

bool Druck::outputToFile()
{
	return ToFile;
}

int Druck::numCopies()
{
	return Copies->value();
}

bool Druck::outputSeparations()
{
	return ToSeparation;
}

QString Druck::separationName()
{
	return SepArt->currentText();
}

QStringList Druck::allSeparations()
{
	QStringList ret;
	for (int a = 1; a < SepArt->count(); ++a)
	{
		ret.append(SepArt->text(a));
	}
	return ret;
}
bool Druck::color()
{
	return PrintGray->isChecked();
}
