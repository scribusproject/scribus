#include "druck.h"
#include "druck.moc"

#include "scconfig.h"

#include "commonstrings.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "customfdialog.h"
#include "cupsoptions.h"
#include <qtextstream.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qmap.h>
#if defined( HAVE_CUPS )
#include <cups/cups.h>
#elif defined(_WIN32)
#include <windows.h>
#include <winspool.h>
#endif
#ifdef HAVE_CMS
extern bool CMSuse;
#endif
#include "util.h"
extern bool previewDinUse;

Druck::Druck( QWidget* parent, QString PDatei, QString PDev, QString PCom, QByteArray& PSettings, bool gcr, QStringList spots)
		: QDialog( parent, "Dr", true, 0)
{
	prefs = PrefsManager::instance()->prefsFile->getContext("print_options");
	DevMode = PSettings;
	ToSeparation = false;
	PrinterOpts = "";
	setCaption( tr( "Setup Printer" ) );
	setIcon(loadIcon("AppIcon.png"));
	DruckLayout = new QVBoxLayout( this );
	DruckLayout->setSpacing( 5 );
	DruckLayout->setMargin( 10 );

	Drucker = new QGroupBox( this, "Drucker" );
	Drucker->setTitle( tr( "Print Destination" ) );
	Drucker->setColumnLayout(0, Qt::Vertical );
	Drucker->layout()->setSpacing( 0 );
	Drucker->layout()->setMargin( 0 );
	DruckerLayout = new QGridLayout( Drucker->layout() );
	DruckerLayout->setAlignment( Qt::AlignTop );
	DruckerLayout->setSpacing( 5 );
	DruckerLayout->setMargin( 10 );

	Layout1x = new QHBoxLayout;
	Layout1x->setSpacing( 5 );
	Layout1x->setMargin( 0 );
	PrintDest = new QComboBox( true, Drucker, "PrintDest" );
	PrintDest->setMinimumSize( QSize( 250, 22 ) );
	PrintDest->setMaximumSize( QSize( 260, 30 ) );
	PrintDest->setEditable(false);
	QString Pcap;
	QString printerName;
	QStringList printerNames = getPrinterNames();
	int numPrinters = printerNames.count();
	for( int i = 0; i < numPrinters; i++)
	{
		printerName = printerNames[i];
		PrintDest->insertItem(printerName);
		if( printerName == PDev )
		{
			PrintDest->setCurrentItem(PrintDest->count()-1);
			prefs->set("CurrentPrn", PrintDest->currentText());
			ToFile = false;
		}
	}

	PrintDest->insertItem( tr("File"));
	if (PDev.isEmpty())
	{
		Geraet = PrintDest->text(0);
		ToFile = false;
	}
	else
		Geraet = PDev;

	Layout1x->addWidget( PrintDest );

#if defined(HAVE_CUPS) || defined(_WIN32)
	OptButton = new QPushButton( tr( "&Options..." ), Drucker, "Optionen" );
	Layout1x->addWidget( OptButton );
#endif
	QSpacerItem* spacerDR = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1x->addItem( spacerDR );
	DruckerLayout->addLayout( Layout1x, 0, 0);
	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 5 );
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
	DruckerLayout->addWidget( OtherCom, 2, 0, Qt::AlignLeft);
	LayoutCC = new QHBoxLayout;
	LayoutCC->setSpacing( 5 );
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

	rangeGroup = new QButtonGroup( tr( "Range" ), this, "rangeGroup" );
	rangeGroup->setColumnLayout(0, Qt::Vertical );
	rangeGroup->layout()->setSpacing( 5 );
	rangeGroup->layout()->setMargin( 10 );
	rangeGroupLayout = new QGridLayout( rangeGroup->layout() );
	rangeGroupLayout->setAlignment( Qt::AlignTop );
	RadioButton1 = new QRadioButton( tr( "Print &All" ), rangeGroup, "RadioButton1" );
	rangeGroupLayout->addMultiCellWidget( RadioButton1, 0, 0, 0, 1 );
	CurrentPage = new QRadioButton( tr( "Print Current Pa&ge" ), rangeGroup, "CurrentPage" );
	rangeGroupLayout->addMultiCellWidget( CurrentPage, 1, 1, 0, 1 );
	RadioButton2 = new QRadioButton( tr( "Print &Range" ), rangeGroup, "RadioButton2" );
	rangeGroupLayout->addWidget( RadioButton2, 2, 0 );
	PageNr = new QLineEdit( rangeGroup, "PageNr" );
	rangeGroupLayout->addWidget( PageNr, 2, 1 );
	PageNr->setEnabled(false);
	Copies = new QSpinBox( rangeGroup, "Copies" );
	Copies->setEnabled( true );
	Copies->setMinimumSize( QSize( 70, 22 ) );
	Copies->setMinValue( 1 );
	Copies->setMaxValue(1000);
	Copies->setValue(1);
	rangeGroupLayout->addWidget( Copies, 0, 3 );
	TextLabel3 = new QLabel( Copies, tr( "N&umber of Copies:" ), rangeGroup, "TextLabel3" );
	rangeGroupLayout->addWidget( TextLabel3, 0, 2 );
	DruckLayout->addWidget( rangeGroup );

	printOptions = new QTabWidget( this, "printOptions" );
	tab = new QWidget( printOptions, "tab" );
	tabLayout = new QGridLayout( tab, 1, 1, 10, 5, "tabLayout");
	PrintSep = new QComboBox( true, tab, "PrintSep" );
	PrintSep->setEditable(false);
	PrintSep->insertItem( tr( "Print Normal" ) );
	PrintSep->insertItem( tr( "Print Separations" ) );
	tabLayout->addWidget( PrintSep, 0, 0 );
	colorType = new QComboBox( true, tab, "colorType" );
	colorType->setEditable(false);
	colorType->insertItem( tr( "Print in Color if Available" ) );
	colorType->insertItem( tr( "Print in Grayscale" ) );
	colorType->setCurrentItem(0);
	tabLayout->addWidget( colorType, 0, 1 );
	SepArt = new QComboBox( true, tab, "SepArt" );
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
	tabLayout->addWidget( SepArt, 1, 0 );
	psLevel = new QComboBox( true, tab, "psLevel" );
	psLevel->insertItem( tr( "PostScript Level 1" ) );
	psLevel->insertItem( tr( "PostScript Level 2" ) );
	psLevel->insertItem( tr( "PostScript Level 3" ) );
	psLevel->setEditable( false );
	tabLayout->addWidget( psLevel, 1, 1 );
	printOptions->insertTab( tab, tr( "Options" ) );
	tab_2 = new QWidget( printOptions, "tab_2" );
	tabLayout_2 = new QHBoxLayout( tab_2, 10, 5, "tabLayout_2");
	pageOpts = new QGroupBox( tab_2, "pageOpts" );
	pageOpts->setTitle( tr( "Page" ) );
	pageOpts->setColumnLayout(0, Qt::Vertical );
	pageOpts->layout()->setSpacing( 5 );
	pageOpts->layout()->setMargin( 10 );
	pageOptsLayout = new QVBoxLayout( pageOpts->layout() );
	pageOptsLayout->setAlignment( Qt::AlignTop );
	MirrorHor = new QCheckBox( pageOpts, "MirrorH" );
	MirrorHor->setText( tr( "Mirror Page(s) Horizontal" ) );
	pageOptsLayout->addWidget( MirrorHor );
	MirrorVert = new QCheckBox( pageOpts, "MirrorV" );
	MirrorVert->setText( tr( "Mirror Page(s) Vertical" ) );
	pageOptsLayout->addWidget( MirrorVert );
	devPar = new QCheckBox( pageOpts, "devPar" );
	devPar->setText( tr( "Set Media Size" ) );
	pageOptsLayout->addWidget( devPar );
	tabLayout_2->addWidget( pageOpts );
	colorOpts = new QButtonGroup( tab_2, "colorOpts" );
	colorOpts->setTitle( tr( "Color" ) );
	colorOpts->setColumnLayout(0, Qt::Vertical );
	colorOpts->layout()->setSpacing( 5 );
	colorOpts->layout()->setMargin( 10 );
	colorOptsLayout = new QVBoxLayout( colorOpts->layout() );
	colorOptsLayout->setAlignment( Qt::AlignTop );
	GcR = new QCheckBox( colorOpts, "GcR" );
	GcR->setText( tr( "Apply Under Color Removal" ) );
	colorOptsLayout->addWidget( GcR );
	spotColors = new QCheckBox( colorOpts, "spotColors" );
	spotColors->setText( tr( "Convert Spot Colors to Process Colors" ) );
	colorOptsLayout->addWidget( spotColors );
#ifdef HAVE_CMS
	if (CMSuse)
	{
		UseICC = new QCheckBox( colorOpts, "UseICC" );
		UseICC->setText( tr( "Apply ICC Profiles" ) );
		colorOptsLayout->addWidget( UseICC );
	}
#endif
	tabLayout_2->addWidget( colorOpts );
	printOptions->insertTab( tab_2, tr( "Advanced Options" ) );
	DruckLayout->addWidget( printOptions );

	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 5 );
	Layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );
	
	previewButton = new QPushButton( tr( "Preview..." ), this, "previewButton" );
	previewButton->setDefault( false );
	previewButton->setEnabled(!previewDinUse);
	Layout2->addWidget( previewButton );

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
		prefs->set("CurrentPrn", PrintDest->currentText());
		DateiT->setEnabled(true);
		LineEdit1->setEnabled(true);
		ToolButton1->setEnabled(true);
		ToFile = true;
	}

	setMaximumSize(sizeHint());
	PrintDest->setFocus();
	QToolTip::add( PageNr, tr( "Insert a comma separated list of tokens where\n"
		                           "a token can be * for all the pages, 1-5 for\n"
		                           "a range of pages or a single page number.") );
	QToolTip::add( OtherCom,"<qt>" + tr( "Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options") + "</qt>" );
	QToolTip::add( psLevel,"<qt>" +  tr( "Sets the PostScript Level.\n Setting to Level 1 or 2 can create huge files" ) + "</qt>" );
	QToolTip::add( GcR, "<qt>" + tr( "A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks." ) + "</qt>");
	QToolTip::add(spotColors,"<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");
	#ifdef HAVE_CMS
	if (CMSuse)
		QToolTip::add(UseICC,"<qt>" + tr( "Allows you to embed ICC profiles in the print stream when color management is enabled" ) + "</qt>");
	#endif
	QToolTip::add(devPar, "<qt>" + tr( "This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer." ) + "</qt>");
	// signals and slots connections
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( okButtonClicked() ) );
	connect( OKButton_2, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( PrintDest, SIGNAL(activated(const QString&)), this, SLOT(SelPrinter(const QString&)));
	connect( RadioButton1, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect( CurrentPage, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect( PrintSep, SIGNAL(activated(int)), this, SLOT(SelMode(int)));
	connect( ToolButton1, SIGNAL(clicked()), this, SLOT(SelFile()));
	connect( OtherCom, SIGNAL(clicked()), this, SLOT(SelComm()));
	connect( previewButton, SIGNAL(clicked()), this, SIGNAL(doPreview()));
#if defined(HAVE_CUPS) || defined(_WIN32)
	connect( OptButton, SIGNAL( clicked() ), this, SLOT( SetOptions() ) );
#endif

	setStoredValues(gcr);

	if (!ToFile)
		initDeviceSettings( PrintDest->currentText() );
	if ( isPostscriptPrinter(PrintDest->currentText()) || ToFile )
		psLevel->setEnabled( true );
	else
		psLevel->setEnabled( false );
}

QStringList Druck::getPrinterNames(void)
{
	QString printerName;
	QStringList printerNames;
#if defined (HAVE_CUPS)
	cups_dest_t *dests;
	int num_dests = cupsGetDests(&dests);
	for (int pr = 0; pr < num_dests; ++pr)
	{
		printerName = QString(dests[pr].name);
		printerNames.append(printerName);
	}
	cupsFreeDests(num_dests, dests);
#elif defined(_WIN32)
	DWORD size;
	DWORD numPrinters;
	PRINTER_INFO_2* printerInfos = NULL;
    EnumPrinters ( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS , NULL, 2, NULL, 0, &size, &numPrinters );
	printerInfos = (PRINTER_INFO_2*) malloc(size);
	if ( EnumPrinters ( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, 2, (LPBYTE) printerInfos, size, &size, &numPrinters ) )
	{
		for ( uint i = 0; i < numPrinters; i++)
		{
			printerName = printerInfos[i].pPrinterName;
			printerNames.append(printerName);
		}
		printerNames.sort();	
	}
	if ( printerInfos) free(printerInfos);
#else
	QString tmp;
	QString Pcap;
	QStringList wt;
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
				printerName = wt[0];
				printerNames.append(printerName);
			}
		}
	}
#endif
	return printerNames;
}

void Druck::SetOptions()
{
#ifdef HAVE_CUPS
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
#elif defined(_WIN32)
	bool done;
	QString  printerS;
	QCString printerC;
	HANDLE handle = NULL;
	// Retrieve the selected printer
	printerS = PrintDest->currentText(); 
	printerC = printerS.local8Bit();
	// Get a printer handle
	done = OpenPrinter( printerC.data(), &handle, NULL );
	if(!done)
		return;
	// Merge stored settings, prompt user and return user settings
	DocumentProperties( winId(), handle, printerC.data(), (DEVMODE*) DevMode.data(), (DEVMODE*) DevMode.data(), 
						DM_IN_BUFFER | DM_IN_PROMPT | DM_OUT_BUFFER);
	// Free the printer handle
	ClosePrinter( handle );
#endif
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
#if defined(HAVE_CUPS) || defined(_WIN32)
		OptButton->setEnabled(false);
#endif

	}
	else
	{
		SelPrinter(PrintDest->currentText());
#if defined(HAVE_CUPS) || defined(_WIN32)
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
	prefs->set("CurrentPrn", prn);
#if defined(HAVE_CUPS) || defined(_WIN32)
	OptButton->setEnabled(!setter);
#endif
	if ( !ToFile )
		if( !getDefaultSettings(PrintDest->currentText()) )
			qWarning(tr("Failed to retrieve printer settings"));
	if ( ToFile || isPostscriptPrinter(PrintDest->currentText()) )
	{
		psLevel->setEnabled( true );
		PrintSep->setEnabled( true );
#ifdef HAVE_CMS
		if (CMSuse)
			UseICC->setEnabled( true );
#endif
	}
	else
	{
		psLevel->setEnabled( false );
		PrintSep->setCurrentText( tr("Print Normal") );
		PrintSep->setEnabled( false );
		SepArt->setCurrentText( tr("All") );
		SepArt->setEnabled( false );
		ToSeparation = false;
#ifdef HAVE_CMS
		if (CMSuse)
		{
			UseICC->setEnabled( false );
			UseICC->setChecked( false );
		}
#endif
	}
	Geraet = prn;
}

void Druck::SelRange(bool e)
{
	PageNr->setEnabled(!e);
}

void Druck::SelMode(int e)
{
	if (e == 0)
	{
		SepArt->setEnabled( false );
		ToSeparation = false;
	}
	else
	{
		SepArt->setEnabled( true );
		ToSeparation = true;
	}
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
	prefs->set("CurrentPrn", PrintDest->currentText());
	prefs->set("OtherCom", OtherCom->isChecked());
	prefs->set("PrintAll", RadioButton1->isChecked());
	prefs->set("CurrentPage", CurrentPage->isChecked());
	prefs->set("PrintRange", RadioButton2->isChecked());
	prefs->set("PageNr", PageNr->text());
	prefs->set("Copies", Copies->value());
	prefs->set("Separations", PrintSep->currentItem());
	prefs->set("PrintColor", colorType->currentItem());
	prefs->set("SepArt", SepArt->currentItem());
	prefs->set("MirrorH", MirrorHor->isChecked());
	prefs->set("MirrorV", MirrorVert->isChecked());
	prefs->set("DoGCR", GcR->isChecked());
	prefs->set("PSLevel", psLevel->currentItem() + 1);
	prefs->set("doDev", devPar->isChecked());
	prefs->set("doSpot", !spotColors->isChecked());
#ifdef HAVE_CMS
	if (CMSuse)
		prefs->set("ICCinUse", UseICC->isChecked());
#endif
	accept();
}

void Druck::setStoredValues(bool gcr)
{
	int selectedDest = prefs->getInt("PrintDest", 0);
	if ((selectedDest > -1) && (selectedDest < PrintDest->count()))
	{
		PrintDest->setCurrentItem(selectedDest);
		prefs->set("CurrentPrn", PrintDest->currentText());
		if (PrintDest->currentText() == tr("File"))
			SelPrinter( tr("File"));
		Geraet = PrintDest->currentText();
	}
	OtherCom->setChecked(prefs->getBool("OtherCom", false));
	if (OtherCom->isChecked())
		SelComm();
	RadioButton1->setChecked(prefs->getBool("PrintAll", true));
	CurrentPage->setChecked(prefs->getBool("CurrentPage", false));
	bool printRangeChecked=prefs->getBool("PrintRange", false);
	RadioButton2->setChecked(printRangeChecked);
	PageNr->setEnabled(printRangeChecked);
	PageNr->setText(prefs->get("PageNr", "1-1"));
	Copies->setValue(prefs->getInt("Copies", 1));
	PrintSep->setCurrentItem(prefs->getInt("Separations", 0));
	colorType->setCurrentItem(prefs->getInt("PrintColor", 0));
	int selectedSep = prefs->getInt("SepArt", 0);
	if ((selectedSep > -1) && (selectedSep < 5))
		SepArt->setCurrentItem(selectedSep);
	if (PrintSep->currentItem() == 1)
		SepArt->setEnabled(true);
	psLevel->setCurrentItem(prefs->getInt("PSLevel", 3)-1);
	MirrorHor->setChecked(prefs->getBool("MirrorH", false));
	MirrorHor->setChecked(prefs->getBool("MirrorV", false));
	devPar->setChecked(prefs->getBool("doDev", false));
	GcR->setChecked(prefs->getBool("DoGCR", gcr));
	spotColors->setChecked(!prefs->getBool("doSpot", true));
#ifdef HAVE_CMS
	if (CMSuse)
	{
		bool iccInUse = prefs->getBool("ICCinUse", false);
		bool psPrinter = isPostscriptPrinter(PrintDest->currentText());
		UseICC->setChecked( psPrinter ? iccInUse : false );
		UseICC->setEnabled( psPrinter );
	}
#endif
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
	if (colorType->currentItem() == 0)
		return true;
	else
		return false;
}

bool Druck::mirrorHorizontal()
{
	return MirrorHor->isChecked();
}

bool Druck::mirrorVertical()
{
	return MirrorVert->isChecked();
}

bool Druck::doGCR()
{
	return GcR->isChecked();
}

int Druck::PSLevel()
{
	return psLevel->currentItem() + 1;
}

bool Druck::doDev()
{
	return devPar->isChecked();
}

bool Druck::doSpot()
{
	return !spotColors->isChecked();
}

bool Druck::ICCinUse()
{
#ifdef HAVE_CMS
	if (CMSuse)
		return UseICC->isChecked();
	else
		return false;
#else
	return false;
#endif
}

bool Druck::getDefaultSettings( QString printerName )
{
#ifdef _WIN32
	bool done;
	uint size;
	QCString printer;
	LONG result = IDOK+1;
	HANDLE handle = NULL;
	printer = printerName.local8Bit();
	// Get the printer handle
	done = OpenPrinter( printer.data(), &handle, NULL );
	if(!done)
		return false;
	// Get size of DEVMODE structure (public + private data)
	size = DocumentProperties( winId(), handle, printer.data(), NULL, NULL, 0);
	// Allocate the memory needed by the DEVMODE structure
	DevMode.resize( size );
	// Retrieve printer default settings
	result = DocumentProperties( winId(), handle, printer.data(), (DEVMODE*) DevMode.data(), NULL, DM_OUT_BUFFER);
	// Free the printer handle
	ClosePrinter( handle );
	return ( result == IDOK );
#else
	return true;
#endif
}

bool Druck::initDeviceSettings( QString printerName )
{
#ifdef _WIN32
	bool done;
	uint size;
	QCString printer;
	LONG result = IDOK+1;
	HANDLE handle = NULL;
	printer = printerName.local8Bit();
	// Get the printer handle
	done = OpenPrinter( printer.data(), &handle, NULL );
	if(!done)
		return false;
	// Get size of DEVMODE structure (public + private data)
	size = DocumentProperties( winId(), handle, printer.data(), NULL, NULL, 0);
	// Compare size with DevMode structure size
	if( DevMode.size() == size )
	{
		// Merge printer settings
		result = DocumentProperties( winId(), handle, printer.data(), (DEVMODE*) DevMode.data(), (DEVMODE*) DevMode.data(), DM_IN_BUFFER | DM_OUT_BUFFER);
	}
	else
	{
		// Retrieve default settings
		DevMode.resize( size );
		result = DocumentProperties( winId(), handle, printer.data(), (DEVMODE*) DevMode.data(), NULL, DM_OUT_BUFFER);
	}
	done = ( result == IDOK);
	// Free the printer handle
	ClosePrinter( handle );
	return done;
#else
	return true;
#endif
}
