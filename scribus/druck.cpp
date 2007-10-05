/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "druck.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QTabWidget>
#include <QWidget>
#include <QSpinBox>
#include <QToolButton>
#include <QDir>
#include <QMap>
#include <QStringList>
#include <QToolTip>
#include <QSpacerItem>
#include <QByteArray>

#include "scconfig.h"
#include "scpaths.h"
#include "scribusdoc.h"
#include "ui/createrange.h"
#include "commonstrings.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "customfdialog.h"
#include "cupsoptions.h"
#if defined( HAVE_CUPS )
	#include <cups/cups.h>
#elif defined(_WIN32)
	#include <windows.h>
	#include <winspool.h>
#endif
#include "util_printer.h"
#include "util_icon.h"
#include "util.h"
#include "units.h"
#include "usertaskstructs.h"
#include "scrspinbox.h"

extern bool previewDinUse;

Druck::Druck( QWidget* parent, ScribusDoc* doc, QString PDatei, QString PDev, QString PCom, QByteArray& PSettings, bool gcr, QStringList spots)
		: QDialog( parent )
{
	setModal(true);
	cdia = 0;
	m_doc = doc;
	unit = doc->unitIndex();
	unitRatio = unitGetRatioFromIndex(doc->unitIndex());
	prefs = PrefsManager::instance()->prefsFile->getContext("print_options");
	DevMode = PSettings;
	ToSeparation = false;
	PrinterOpts = "";
	setWindowTitle( tr( "Setup Printer" ) );
	setWindowIcon(QIcon(loadIcon("AppIcon.png")));
	DruckLayout = new QVBoxLayout( this );
	DruckLayout->setSpacing( 5 );
	DruckLayout->setMargin( 10 );

	Drucker = new QGroupBox( this );
	Drucker->setTitle( tr( "Print Destination" ) );
	DruckerLayout = new QGridLayout( Drucker );
	DruckerLayout->setAlignment( Qt::AlignTop );
	DruckerLayout->setSpacing( 5 );
	DruckerLayout->setMargin( 10 );

	Layout1x = new QHBoxLayout;
	Layout1x->setSpacing( 5 );
	Layout1x->setMargin( 0 );
	PrintDest = new QComboBox(Drucker);
	PrintDest->setMinimumSize( QSize( 250, 22 ) );
	PrintDest->setMaximumSize( QSize( 260, 30 ) );
	PrintDest->setEditable(false);
	QString Pcap;
	QString printerName;
	QStringList printerNames = PrinterUtil::getPrinterNames();
	int numPrinters = printerNames.count();
	for( int i = 0; i < numPrinters; i++)
	{
		printerName = printerNames[i];
		PrintDest->addItem(printerName);
		if( printerName == PDev )
		{
			PrintDest->setCurrentIndex(PrintDest->count()-1);
			prefs->set("CurrentPrn", PrintDest->currentText());
			ToFile = false;
		}
	}

	PrintDest->addItem( tr("File"));
	if (PDev.isEmpty())
	{
		Geraet = PrintDest->itemText(0);
		ToFile = false;
	}
	else
		Geraet = PDev;

	Layout1x->addWidget( PrintDest );

#if defined(HAVE_CUPS) || defined(_WIN32)
	OptButton = new QPushButton( tr( "&Options..." ), Drucker );
	Layout1x->addWidget( OptButton );
#endif
	QSpacerItem* spacerDR = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1x->addItem( spacerDR );
	DruckerLayout->addLayout( Layout1x, 0, 0);
	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 5 );
	Layout1->setMargin( 0 );
	LineEdit1 = new QLineEdit( QDir::convertSeparators(PDatei), Drucker );
	LineEdit1->setMinimumSize( QSize( 240, 22 ) );
	LineEdit1->setEnabled(false);
	DateiT = new QLabel( tr( "&File:" ), Drucker );
	DateiT->setBuddy(LineEdit1);
	DateiT->setEnabled( false );
	Layout1->addWidget( DateiT );
	Layout1->addWidget( LineEdit1 );
	ToolButton1 = new QToolButton( Drucker );
	ToolButton1->setText( tr( "C&hange..." ) );
	ToolButton1->setMinimumSize( QSize( 80, 22 ) );
	ToolButton1->setEnabled(false);
	ToolButton1->setFocusPolicy( Qt::TabFocus );
	Layout1->addWidget( ToolButton1 );
	DruckerLayout->addLayout( Layout1, 1, 0 );

	OtherCom = new QCheckBox( tr("A&lternative Printer Command"), Drucker);
	OtherCom->setChecked(false);
	DruckerLayout->addWidget( OtherCom, 2, 0, Qt::AlignLeft);
	LayoutCC = new QHBoxLayout;
	LayoutCC->setSpacing( 5 );
	LayoutCC->setMargin( 0 );
	Command = new QLineEdit( PCom, Drucker );
	Command->setMinimumSize( QSize( 240, 22 ) );
	Command->setEnabled(false);
	OthText = new QLabel( tr( "Co&mmand:" ), Drucker );
	OthText->setBuddy(Command);
	OthText->setEnabled( false );
	LayoutCC->addWidget( OthText );
	LayoutCC->addWidget( Command );
	DruckerLayout->addLayout( LayoutCC, 3, 0 );
	DruckLayout->addWidget( Drucker );

	rangeGroup = new QGroupBox( tr( "Range" ), this );
	rangeGroupLayout = new QGridLayout( rangeGroup );
	rangeGroupLayout->setSpacing( 5 );
	rangeGroupLayout->setMargin( 10 );
	rangeGroupLayout->setAlignment( Qt::AlignTop );
	RadioButton1 = new QRadioButton( tr( "Print &All" ), rangeGroup );
	rangeGroupLayout->addWidget( RadioButton1, 0, 0, 1, 2 );
	CurrentPage = new QRadioButton( tr( "Print Current Pa&ge" ), rangeGroup );
	rangeGroupLayout->addWidget( CurrentPage, 1, 0, 1, 2 );
	RadioButton2 = new QRadioButton( tr( "Print &Range" ), rangeGroup );
	rangeGroupLayout->addWidget( RadioButton2, 2, 0 );
	pageNumberSelectorLayout = new QHBoxLayout;
	pageNumberSelectorLayout->setSpacing( 5 );
	pageNumberSelectorLayout->setMargin( 0 );
	pageNr = new QLineEdit( rangeGroup );
 	pageNumberSelectorLayout->addWidget( pageNr );
 	pageNrButton = new QPushButton(rangeGroup);
 	pageNrButton->setIcon(QIcon(loadIcon("ellipsis.png")));
 	pageNumberSelectorLayout->addWidget( pageNrButton );
	rangeGroupLayout->addLayout( pageNumberSelectorLayout, 2, 1 );
	pageNr->setEnabled(false);
	pageNrButton->setEnabled(false);
	Copies = new QSpinBox( rangeGroup );
	Copies->setEnabled( true );
	Copies->setMinimumSize( QSize( 70, 22 ) );
	Copies->setMinimum( 1 );
	Copies->setMaximum(1000);
	Copies->setValue(1);
	rangeGroupLayout->addWidget( Copies, 0, 3 );
	TextLabel3 = new QLabel( tr( "N&umber of Copies:" ), rangeGroup );
	TextLabel3->setBuddy(Copies);
	rangeGroupLayout->addWidget( TextLabel3, 0, 2 );
	DruckLayout->addWidget( rangeGroup );

	printOptions = new QTabWidget( this );
	tab = new QWidget( printOptions );
	tabLayout = new QGridLayout( tab );
	tabLayout->setSpacing( 5 );
	tabLayout->setMargin( 10 );
	PrintSep = new QComboBox( tab );
	PrintSep->setEditable(false);
	PrintSep->addItem( tr( "Print Normal" ) );
	PrintSep->addItem( tr( "Print Separations" ) );
	tabLayout->addWidget( PrintSep, 0, 0 );
	colorType = new QComboBox(tab);
	colorType->setEditable(false);
	colorType->addItem( tr( "Print in Color if Available" ) );
	colorType->addItem( tr( "Print in Grayscale" ) );
	colorType->setCurrentIndex(0);
	tabLayout->addWidget( colorType, 0, 1 );
	SepArt = new QComboBox(tab);
	QString sep[] =
	    {
	        tr("All"), tr("Cyan"), tr("Magenta"), tr("Yellow"),
	        tr("Black")
	    };
	size_t sepArray = sizeof(sep) / sizeof(*sep);
	for (uint prop = 0; prop < sepArray; ++prop)
		SepArt->addItem(sep[prop]);
	SepArt->addItems(spots);
	SepArt->setEnabled( false );
	SepArt->setEditable( false );
	tabLayout->addWidget( SepArt, 1, 0 );
	psLevel = new QComboBox(tab);
	psLevel->addItem( tr( "PostScript Level 1" ) );
	psLevel->addItem( tr( "PostScript Level 2" ) );
	psLevel->addItem( tr( "PostScript Level 3" ) );
	psLevel->setEditable( false );
	tabLayout->addWidget( psLevel, 1, 1 );
	printOptions->addTab( tab, tr( "Options" ) );
	tab_2 = new QWidget( printOptions );
	tabLayout_2 = new QHBoxLayout(tab_2);
	tabLayout_2->setSpacing( 5 );
	tabLayout_2->setMargin( 10 );
	pageOpts = new QGroupBox( tab_2 );
	pageOpts->setTitle( tr( "Page" ) );
	pageOptsLayout = new QVBoxLayout( pageOpts );
	pageOptsLayout->setSpacing( 5 );
	pageOptsLayout->setMargin( 10 );
	pageOptsLayout->setAlignment( Qt::AlignTop );
	MirrorHor = new QCheckBox( pageOpts );
	MirrorHor->setText( tr( "Mirror Page(s) Horizontal" ) );
	pageOptsLayout->addWidget( MirrorHor );
	MirrorVert = new QCheckBox( pageOpts );
	MirrorVert->setText( tr( "Mirror Page(s) Vertical" ) );
	pageOptsLayout->addWidget( MirrorVert );
	devPar = new QCheckBox( pageOpts );
	devPar->setText( tr( "Set Media Size" ) );
	pageOptsLayout->addWidget( devPar );
	ClipMarg = new QCheckBox( tr( "Clip to Page Margins" ), pageOpts );
	pageOptsLayout->addWidget( ClipMarg );
	tabLayout_2->addWidget( pageOpts );
	colorOpts = new QGroupBox( tab_2 );
	colorOpts->setTitle( tr( "Color" ) );
	colorOptsLayout = new QVBoxLayout( colorOpts );
	colorOptsLayout->setSpacing( 5 );
	colorOptsLayout->setMargin( 10 );
	colorOptsLayout->setAlignment( Qt::AlignTop );
	GcR = new QCheckBox( colorOpts );
	GcR->setText( tr( "Apply Under Color Removal" ) );
	colorOptsLayout->addWidget( GcR );
	spotColors = new QCheckBox( colorOpts );
	spotColors->setText( tr( "Convert Spot Colors to Process Colors" ) );
	colorOptsLayout->addWidget( spotColors );
	overprintMode = new QCheckBox( colorOpts );
	overprintMode->setText( tr( "Force Overprint Mode" ) );
	colorOptsLayout->addWidget( overprintMode );
	if (m_doc->HasCMS)
	{
		UseICC = new QCheckBox( colorOpts );
		UseICC->setText( tr( "Apply Color Profiles" ) );
		colorOptsLayout->addWidget( UseICC );
	}
	tabLayout_2->addWidget( colorOpts );
	printOptions->addTab( tab_2, tr( "Advanced Options" ) );

	tab_3 = new QWidget( printOptions );
	tabLayout_3 = new QGridLayout( tab_3 );
	tabLayout_3->setSpacing( 5 );
	tabLayout_3->setMargin( 10 );
	MarkGroup = new QGroupBox( tab_3 );
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
	colorMarks = new QCheckBox( tr( "Color Bars" ), MarkGroup );
	MarkGroupLayout->addWidget( colorMarks, 0, 1, 1, 2 );
	MarkTxt1 = new QLabel( MarkGroup );
	MarkTxt1->setText( tr( "Offset:" ) );
	MarkGroupLayout->addWidget( MarkTxt1, 1, 1 );
	markOffset = new ScrSpinBox( 0, 3000*unitRatio, MarkGroup, unit );
	MarkGroupLayout->addWidget( markOffset, 1, 2 );
	tabLayout_3->addWidget( MarkGroup, 0, 0 );
	printOptions->addTab( tab_3, tr( "Marks" ) );

	tab_4 = new QWidget( printOptions );
	tabLayout_4 = new QGridLayout( tab_4 );
	tabLayout_4->setSpacing( 5 );
	tabLayout_4->setMargin( 10 );
	BleedGroup = new QGroupBox( tab_4 );
	BleedGroup->setTitle( tr( "Bleed Settings" ) );
	BleedGroupLayout = new QGridLayout( BleedGroup );
	BleedGroupLayout->setSpacing( 5 );
	BleedGroupLayout->setMargin( 10 );
	BleedGroupLayout->setAlignment( Qt::AlignTop );
	BleedTxt1 = new QLabel( BleedGroup );
	BleedTxt1->setText( tr( "Top:" ) );
	BleedGroupLayout->addWidget( BleedTxt1, 0, 0 );
	BleedTop = new ScrSpinBox( 0, 3000*unitRatio, BleedGroup, unit );
	BleedGroupLayout->addWidget( BleedTop, 0, 1 );
	BleedTxt2 = new QLabel( BleedGroup );
	BleedTxt2->setText( tr( "Bottom:" ) );
	BleedGroupLayout->addWidget( BleedTxt2, 1, 0 );
	BleedBottom = new ScrSpinBox( 0, 3000*unitRatio, BleedGroup, unit );
	BleedGroupLayout->addWidget( BleedBottom, 1, 1 );
	BleedTxt3 = new QLabel( BleedGroup );
	BleedTxt3->setText( tr( "Left:" ) );
	BleedGroupLayout->addWidget( BleedTxt3, 0, 2 );
	BleedRight = new ScrSpinBox( 0, 3000*unitRatio, BleedGroup, unit );
	BleedGroupLayout->addWidget( BleedRight, 0, 3 );
	BleedTxt4 = new QLabel( BleedGroup );
	BleedTxt4->setText( tr( "Right:" ) );
	BleedGroupLayout->addWidget( BleedTxt4, 1, 2 );
	BleedLeft = new ScrSpinBox( 0, 3000*unitRatio, BleedGroup, unit );
	BleedGroupLayout->addWidget( BleedLeft, 1, 3 );
	docBleeds = new QCheckBox( tr( "Use Document Bleeds" ), BleedGroup );
	docBleeds->setChecked(false);
	BleedGroupLayout->addWidget( docBleeds, 2, 0, 1, 4 );
	tabLayout_4->addWidget( BleedGroup, 0, 0 );
	printOptions->addTab( tab_4, tr( "Bleeds" ) );
	if (m_doc->currentPageLayout != 0)
	{
		BleedTxt3->setText( tr( "Inside:" ) );
		BleedTxt4->setText( tr( "Outside:" ) );
	}

	DruckLayout->addWidget( printOptions );

	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 5 );
	Layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );
	
	previewButton = new QPushButton( tr( "Preview..." ), this );
	previewButton->setDefault( false );
	previewButton->setEnabled(!previewDinUse);
	Layout2->addWidget( previewButton );

	OKButton = new QPushButton( tr( "&Print" ), this );
	OKButton->setDefault( true );
	Layout2->addWidget( OKButton );
	OKButton_2 = new QPushButton( CommonStrings::tr_Cancel, this );
	OKButton_2->setDefault( false );
	Layout2->addWidget( OKButton_2 );

	DruckLayout->addLayout( Layout2 );
	if ((PDev== tr("File")) || (PrintDest->count() == 1))
	{
		PrintDest->setCurrentIndex(PrintDest->count()-1);
		prefs->set("CurrentPrn", PrintDest->currentText());
		DateiT->setEnabled(true);
		LineEdit1->setEnabled(true);
		ToolButton1->setEnabled(true);
		ToFile = true;
	}

	setMaximumSize(sizeHint());
	PrintDest->setFocus();
	BleedTop->setToolTip( "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
	BleedBottom->setToolTip( "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
	BleedLeft->setToolTip( "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
	BleedRight->setToolTip( "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
	ClipMarg->setToolTip( "<qt>" + tr( "Do not show objects outside the margins on the printed page" ) + "</qt>" );
	pageNr->setToolTip( tr( "Insert a comma separated list of tokens where\n"
		                           "a token can be * for all the pages, 1-5 for\n"
		                           "a range of pages or a single page number.") );
	OtherCom->setToolTip("<qt>" + tr( "Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options") + "</qt>" );
	psLevel->setToolTip("<qt>" +  tr( "Sets the PostScript Level.\n Setting to Level 1 or 2 can create huge files" ) + "</qt>" );
	GcR->setToolTip( "<qt>" + tr( "A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks." ) + "</qt>");
	spotColors->setToolTip("<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");
	overprintMode->setToolTip( "<qt>"+ tr("Enables global Overprint Mode for this document, overrides object settings") + "<qt>");
	if (m_doc->HasCMS)
		UseICC->setToolTip("<qt>" + tr( "Allows you to embed color profiles in the print stream when color management is enabled" ) + "</qt>");
	devPar->setToolTip( "<qt>" + tr( "This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer." ) + "</qt>");
	cropMarks->setToolTip( "<qt>" + tr( "This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing" ) + "</qt>" );
	bleedMarks->setToolTip( "<qt>" + tr( "This creates bleed marks which are indicated by  _ . _ and show the bleed limit" ) + "</qt>" );
	registrationMarks->setToolTip( "<qt>" + tr( "Add registration marks which are added to each separation" ) + "</qt>" );
	colorMarks->setToolTip( "<qt>" + tr( "Add color calibration bars" ) + "</qt>" );
	markOffset->setToolTip( "<qt>" + tr( "Indicate the distance offset for the registration marks" ) + "</qt>" );
	BleedTop->setToolTip( "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
	BleedBottom->setToolTip( "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
	BleedLeft->setToolTip( "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
	BleedRight->setToolTip( "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
	docBleeds->setToolTip( "<qt>" + tr( "Use the existing bleed settings from the document preferences" ) + "</qt>" );

	// signals and slots connections
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( okButtonClicked() ) );
	connect( OKButton_2, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( PrintDest, SIGNAL(activated(const QString&)), this, SLOT(SelPrinter(const QString&)));
	connect( RadioButton1, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect( CurrentPage, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect( pageNrButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
	connect( PrintSep, SIGNAL(activated(int)), this, SLOT(SelMode(int)));
	connect( ToolButton1, SIGNAL(clicked()), this, SLOT(SelFile()));
	connect( OtherCom, SIGNAL(clicked()), this, SLOT(SelComm()));
	connect( previewButton, SIGNAL(clicked()), this, SLOT(previewButtonClicked()));
	connect(docBleeds, SIGNAL(clicked()), this, SLOT(doDocBleeds()));
#if defined(HAVE_CUPS) || defined(_WIN32)
	connect( OptButton, SIGNAL( clicked() ), this, SLOT( SetOptions() ) );
#endif

	setStoredValues(gcr);
#if defined(_WIN32)
	if (!ToFile)
		PrinterUtil::initDeviceSettings( PrintDest->currentText(), DevMode );
#endif
	if ( PrinterUtil::isPostscriptPrinter(PrintDest->currentText()) || ToFile )
		psLevel->setEnabled( true );
	else
		psLevel->setEnabled( false );
}

Druck::~Druck()
{
#ifdef HAVE_CUPS
	delete cdia;
#endif
	cdia = 0;
}

void Druck::SetOptions()
{
#ifdef HAVE_CUPS
	PrinterOpts = "";
	if (!cdia)
		cdia = new CupsOptions(this, Geraet);
	if (!cdia->exec())
	{
		delete cdia; // if options was canceled delete dia 
		cdia = 0;    // so that getoptions() in the okButtonClicked() will get 
		             // the default values from the last succesful run
	}

#elif defined(_WIN32)
	bool done;
	Qt::HANDLE handle = NULL;
	// Retrieve the selected printer
	QString printerS = PrintDest->currentText(); 
	// Get a printer handle
	done = OpenPrinterW( (LPWSTR) printerS.utf16(), &handle, NULL );
	if(!done)
		return;
	// Merge stored settings, prompt user and return user settings
	DocumentPropertiesW( winId(), handle, (LPWSTR) printerS.utf16(), (DEVMODEW*) DevMode.data(), (DEVMODEW*) DevMode.data(), 
						DM_IN_BUFFER | DM_IN_PROMPT | DM_OUT_BUFFER);
	// Free the printer handle
	ClosePrinter( handle );
#endif
}

void Druck::getOptions()
{
#ifdef HAVE_CUPS
	PrinterOpts = "";
	if (!cdia)
		cdia = new CupsOptions(this, Geraet);
	struct CupsOptions::OpData daten;
	QMap<QString,CupsOptions::OpData>::Iterator it;
	for (it = cdia->KeyToText.begin(); it != cdia->KeyToText.end(); ++it)
	{
		if (cdia->KeyToDefault[it.key()] != cdia->FlagsOpt.at(it.value().Cnum)->currentText())
		{
			if (it.value().KeyW == "mirror")
				PrinterOpts += " -o mirror";
			else
			{
				if (it.value().KeyW == "page-set")
				{
					PrinterOpts += " -o "+it.value().KeyW+"=";
					if (cdia->FlagsOpt.at(it.value().Cnum)->currentIndex() == 1)
						PrinterOpts += "even";
					else
						PrinterOpts += "odd";
				}
				else
				{
					if (it.value().KeyW == "number-up")
					{
						PrinterOpts += " -o "+it.value().KeyW+"=";
						switch (cdia->FlagsOpt.at(it.value().Cnum)->currentIndex())
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
						if (it.value().KeyW == "orientation")
							PrinterOpts += " -o landscape";
						else
							PrinterOpts += " -o " +
									it.value().KeyW+"="+cdia->FlagsOpt.at(it.value().Cnum)->currentText();
					}
				}
			}
		}
	}
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
#if defined(_WIN32)
	if ( !ToFile )
		if( !PrinterUtil::getDefaultSettings(PrintDest->currentText(), DevMode) )
			qWarning( tr("Failed to retrieve printer settings").toAscii().data() );
#endif
	if ( ToFile || PrinterUtil::isPostscriptPrinter(PrintDest->currentText()) )
	{
		psLevel->setEnabled( true );
		PrintSep->setEnabled( true );
		if (m_doc->HasCMS)
			UseICC->setEnabled( true );
	}
	else
	{
		psLevel->setEnabled( false );
		setCurrentComboItem(PrintSep, tr("Print Normal"));
		PrintSep->setEnabled( false );
		setCurrentComboItem(SepArt, tr("All"));
		SepArt->setEnabled( false );
		ToSeparation = false;
		if (m_doc->HasCMS)
		{
			UseICC->setEnabled( false );
			UseICC->setChecked( false );
		}
	}
	Geraet = prn;
}

void Druck::SelRange(bool e)
{
	pageNr->setEnabled(!e);
	pageNrButton->setEnabled(!e);
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
	CustomFDialog dia(this, wdir, tr("Save as"), tr("PostScript Files (*.ps);;All Files (*)"), fdNone);
	if (!LineEdit1->text().isEmpty())
		dia.setSelection(LineEdit1->text());
	if (dia.exec() == QDialog::Accepted)
	{
		QString selectedFile = dia.selectedFile();
		dirs->set("druck", selectedFile.left(selectedFile.lastIndexOf("/")));
		LineEdit1->setText( QDir::convertSeparators(selectedFile) );
	}
}

void Druck::setMinMax(int min, int max, int cur)
{
	QString tmp, tmp2;
	CurrentPage->setText( tr( "Print Current Pa&ge" )+" ("+tmp.setNum(cur)+")");
	pageNr->setText(tmp.setNum(min)+"-"+tmp2.setNum(max));
}

void Druck::storeValues()
{
	getOptions(); // options were not set get last options with this hack

	m_doc->Print_Options.printer = PrintDest->currentText();
	m_doc->Print_Options.filename = ScPaths::separatorsToSlashes(LineEdit1->text());
	m_doc->Print_Options.toFile = ToFile;
	m_doc->Print_Options.copies = numCopies();
	m_doc->Print_Options.outputSeparations = outputSeparations();
	m_doc->Print_Options.separationName = separationName();
	m_doc->Print_Options.allSeparations = allSeparations();
	if (m_doc->Print_Options.outputSeparations)
		m_doc->Print_Options.useSpotColors = true;
	else
		m_doc->Print_Options.useSpotColors = doSpot();
	m_doc->Print_Options.useColor = color();
	m_doc->Print_Options.mirrorH = mirrorHorizontal();
	m_doc->Print_Options.mirrorV = mirrorVertical();
	m_doc->Print_Options.useICC = ICCinUse();
	m_doc->Print_Options.doClip = doClip();
	m_doc->Print_Options.doGCR = doGCR();
	m_doc->Print_Options.PSLevel = PSLevel();
	m_doc->Print_Options.setDevParam = doDev();
	m_doc->Print_Options.doOverprint = doOverprint();
	m_doc->Print_Options.bleeds.Top = BleedTop->value() / m_doc->unitRatio();
	m_doc->Print_Options.bleeds.Left = BleedLeft->value() / m_doc->unitRatio();
	m_doc->Print_Options.bleeds.Right = BleedRight->value() / m_doc->unitRatio();
	m_doc->Print_Options.bleeds.Bottom = BleedBottom->value() / m_doc->unitRatio();
	m_doc->Print_Options.markOffset = markOffset->value() / m_doc->unitRatio();
	m_doc->Print_Options.cropMarks = cropMarks->isChecked();
	m_doc->Print_Options.bleedMarks = bleedMarks->isChecked();
	m_doc->Print_Options.registrationMarks = registrationMarks->isChecked();
	m_doc->Print_Options.colorMarks = colorMarks->isChecked();
	if (OtherCom->isChecked())
	{
		m_doc->Print_Options.printerCommand = Command->text();
		m_doc->Print_Options.useAltPrintCommand = true;
	}
	else
		m_doc->Print_Options.useAltPrintCommand = false;
#ifdef HAVE_CUPS
		m_doc->Print_Options.printerOptions = PrinterOpts;
#else
		m_doc->Print_Options.printerOptions = QString("");
#endif
/*
	prefs->set("PrintDest", PrintDest->currentItem());
	prefs->set("CurrentPrn", PrintDest->currentText());
	prefs->set("OtherCom", OtherCom->isChecked());
	prefs->set("PrintAll", RadioButton1->isChecked());
	prefs->set("CurrentPage", CurrentPage->isChecked());
	prefs->set("PrintRange", RadioButton2->isChecked());
	prefs->set("PageNr", pageNr->text());
	prefs->set("Copies", Copies->value());
	prefs->set("Separations", PrintSep->currentItem());
	prefs->set("PrintColor", colorType->currentItem());
	prefs->set("SepArt", SepArt->currentItem());
	prefs->set("MirrorH", MirrorHor->isChecked());
	prefs->set("MirrorV", MirrorVert->isChecked());
	prefs->set("DoGCR", GcR->isChecked());
	prefs->set("Clip", ClipMarg->isChecked());
	prefs->set("PSLevel", psLevel->currentItem() + 1);
	prefs->set("doDev", devPar->isChecked());
	prefs->set("doSpot", !spotColors->isChecked());
	prefs->set("doOverprint", overprintMode->isChecked());
	if (m_doc->HasCMS)
		prefs->set("ICCinUse", UseICC->isChecked());
	prefs->set("BleedTop", BleedTop->value() / unitRatio);
	prefs->set("BleedBottom", BleedBottom->value() / unitRatio);
	prefs->set("BleedRight", BleedRight->value() / unitRatio);
	prefs->set("BleedLeft", BleedLeft->value() / unitRatio);
	prefs->set("markOffset", markOffset->value() / unitRatio);
	prefs->set("cropMarks", cropMarks->isChecked());
	prefs->set("bleedMarks", bleedMarks->isChecked());
	prefs->set("registrationMarks", registrationMarks->isChecked());
	prefs->set("colorMarks", colorMarks->isChecked()); */
}

void Druck::okButtonClicked()
{
	storeValues();
	accept();
}

void Druck::previewButtonClicked()
{
	storeValues();
	emit doPreview();
}

void Druck::setStoredValues(bool gcr)
{
	if (m_doc->Print_Options.firstUse)
	{
		int selectedDest = prefs->getInt("PrintDest", 0);
		if ((selectedDest > -1) && (selectedDest < PrintDest->count()))
		{
			PrintDest->setCurrentIndex(selectedDest);
			prefs->set("CurrentPrn", PrintDest->currentText());
			if (PrintDest->currentText() == tr("File"))
				SelPrinter( tr("File"));
			Geraet = PrintDest->currentText();
		}
		OtherCom->setChecked(prefs->getBool("OtherCom", false));
		if (OtherCom->isChecked())
		{
			SelComm();
			Command->setText(prefs->get("Command", ""));
		}
		RadioButton1->setChecked(prefs->getBool("PrintAll", true));
		CurrentPage->setChecked(prefs->getBool("CurrentPage", false));
		bool printRangeChecked=prefs->getBool("PrintRange", false);
		RadioButton2->setChecked(printRangeChecked);
		pageNr->setEnabled(printRangeChecked);
		pageNr->setText(prefs->get("PageNr", "1-1"));
		Copies->setValue(prefs->getInt("Copies", 1));
		PrintSep->setCurrentIndex(prefs->getInt("Separations", 0));
		colorType->setCurrentIndex(prefs->getInt("PrintColor", 0));
		int selectedSep = prefs->getInt("SepArt", 0);
		if ((selectedSep > -1) && (selectedSep < 5))
			SepArt->setCurrentIndex(selectedSep);
		if (PrintSep->currentIndex() == 1)
		{
			SepArt->setEnabled(true);
			ToSeparation = true;
		}
		psLevel->setCurrentIndex(prefs->getInt("PSLevel", 3)-1);
		MirrorHor->setChecked(prefs->getBool("MirrorH", false));
		MirrorVert->setChecked(prefs->getBool("MirrorV", false));
		devPar->setChecked(prefs->getBool("doDev", false));
		GcR->setChecked(prefs->getBool("DoGCR", gcr));
		ClipMarg->setChecked(prefs->getBool("Clip", false));
		spotColors->setChecked(!prefs->getBool("doSpot", true));
		overprintMode->setChecked(prefs->getBool("doOverprint", false));
		if (m_doc->HasCMS)
		{
			bool iccInUse = prefs->getBool("ICCinUse", false);
			bool psPrinter = PrinterUtil::isPostscriptPrinter(PrintDest->currentText()) || ToFile;
			UseICC->setChecked( psPrinter ? iccInUse : false );
			UseICC->setEnabled( psPrinter );
		}
		BleedTop->setValue(prefs->getDouble("BleedTop",0.0)*unitRatio);
		BleedBottom->setValue(prefs->getDouble("BleedBottom",0.0)*unitRatio);
		BleedRight->setValue(prefs->getDouble("BleedRight",0.0)*unitRatio);
		BleedLeft->setValue(prefs->getDouble("BleedLeft",0.0)*unitRatio);
		markOffset->setValue(prefs->getDouble("markOffset",0.0)*unitRatio);
		cropMarks->setChecked(prefs->getBool("cropMarks", false));
		bleedMarks->setChecked(prefs->getBool("bleedMarks", false));
		registrationMarks->setChecked(prefs->getBool("registrationMarks", false));
		colorMarks->setChecked(prefs->getBool("colorMarks", false));
	}
	else
	{
		int selectedDest = prefs->getInt("PrintDest", 0);
		if ((selectedDest > -1) && (selectedDest < PrintDest->count()))
		{
			PrintDest->setCurrentIndex(selectedDest);
			prefs->set("CurrentPrn", PrintDest->currentText());
			if (PrintDest->currentText() == tr("File"))
				SelPrinter( tr("File"));
			Geraet = PrintDest->currentText();
		}
		OtherCom->setChecked(m_doc->Print_Options.useAltPrintCommand);
		if (OtherCom->isChecked())
		{
			SelComm();
			Command->setText(m_doc->Print_Options.printerCommand);
		}
		RadioButton1->setChecked(prefs->getBool("PrintAll", true));
		CurrentPage->setChecked(prefs->getBool("CurrentPage", false));
		bool printRangeChecked=prefs->getBool("PrintRange", false);
		RadioButton2->setChecked(printRangeChecked);
		pageNr->setEnabled(printRangeChecked);
		pageNr->setText(prefs->get("PageNr", "1-1"));
		Copies->setValue(1);
		PrintSep->setCurrentIndex(m_doc->Print_Options.outputSeparations);
		colorType->setCurrentIndex(m_doc->Print_Options.useColor? 0 : 1);
		ColorList usedSpots;
		m_doc->getUsedColors(usedSpots, true);
		QStringList spots = usedSpots.keys();
		spots.prepend( tr("Black"));
		spots.prepend( tr("Yellow"));
		spots.prepend( tr("Magenta"));
		spots.prepend( tr("Cyan"));
		spots.prepend( tr("All"));
		int selectedSep = spots.indexOf(m_doc->Print_Options.separationName);
		if ((selectedSep > -1) && (selectedSep < SepArt->count()))
			SepArt->setCurrentIndex(selectedSep);
		if (PrintSep->currentIndex() == 1)
		{
			SepArt->setEnabled(true);
			ToSeparation = true;
		}
		psLevel->setCurrentIndex(m_doc->Print_Options.PSLevel-1);
		MirrorHor->setChecked(m_doc->Print_Options.mirrorH);
		MirrorVert->setChecked(m_doc->Print_Options.mirrorV);
		devPar->setChecked(m_doc->Print_Options.setDevParam);
		GcR->setChecked(m_doc->Print_Options.doGCR);
		ClipMarg->setChecked(m_doc->Print_Options.doClip);
		spotColors->setChecked(!m_doc->Print_Options.useSpotColors);
		overprintMode->setChecked(m_doc->Print_Options.doOverprint);
		if (m_doc->HasCMS)
		{
			bool iccInUse = m_doc->Print_Options.useICC;
			bool psPrinter = PrinterUtil::isPostscriptPrinter(PrintDest->currentText()) || ToFile;
			UseICC->setChecked( psPrinter ? iccInUse : false );
			UseICC->setEnabled( psPrinter );
		}
		BleedTop->setValue(m_doc->Print_Options.bleeds.Top*unitRatio);
		BleedBottom->setValue(m_doc->Print_Options.bleeds.Bottom*unitRatio);
		BleedRight->setValue(m_doc->Print_Options.bleeds.Right*unitRatio);
		BleedLeft->setValue(m_doc->Print_Options.bleeds.Left*unitRatio);
		markOffset->setValue(m_doc->Print_Options.markOffset*unitRatio);
		cropMarks->setChecked(m_doc->Print_Options.cropMarks);
		bleedMarks->setChecked(m_doc->Print_Options.bleedMarks);
		registrationMarks->setChecked(m_doc->Print_Options.registrationMarks);
		colorMarks->setChecked(m_doc->Print_Options.colorMarks);
	}
}

QString Druck::printerName()
{
	return Geraet;
}

QString Druck::outputFileName()
{
	return ScPaths::separatorsToSlashes(LineEdit1->text());
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
		ret.append(SepArt->itemText(a));
	}
	return ret;
}

bool Druck::color()
{
	if (colorType->currentIndex() == 0)
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

bool Druck::doClip()
{
	return ClipMarg->isChecked();
}

int Druck::PSLevel()
{
	return psLevel->currentIndex() + 1;
}

bool Druck::doDev()
{
	return devPar->isChecked();
}

bool Druck::doSpot()
{
	return !spotColors->isChecked();
}

bool Druck::doOverprint()
{
	return overprintMode->isChecked();
}

bool Druck::ICCinUse()
{
	if (m_doc->HasCMS)
		return UseICC->isChecked();
	else
		return false;
}

void Druck::doDocBleeds()
{
	if (docBleeds->isChecked())
	{
		prefs->set("BleedTop", BleedTop->value() / unitRatio);
		prefs->set("BleedBottom", BleedBottom->value() / unitRatio);
		prefs->set("BleedRight", BleedRight->value() / unitRatio);
		prefs->set("BleedLeft", BleedLeft->value() / unitRatio);
		BleedTop->setValue(m_doc->bleeds.Top*unitRatio);
		BleedBottom->setValue(m_doc->bleeds.Bottom*unitRatio);
		BleedRight->setValue(m_doc->bleeds.Right*unitRatio);
		BleedLeft->setValue(m_doc->bleeds.Left*unitRatio);
		BleedTop->setEnabled(false);
		BleedBottom->setEnabled(false);
		BleedRight->setEnabled(false);
		BleedLeft->setEnabled(false);
	}
	else
	{
		BleedTop->setValue(prefs->getDouble("BleedTop",0.0)*unitRatio);
		BleedBottom->setValue(prefs->getDouble("BleedBottom",0.0)*unitRatio);
		BleedRight->setValue(prefs->getDouble("BleedRight",0.0)*unitRatio);
		BleedLeft->setValue(prefs->getDouble("BleedLeft",0.0)*unitRatio);
		BleedTop->setEnabled(true);
		BleedBottom->setEnabled(true);
		BleedRight->setEnabled(true);
		BleedLeft->setEnabled(true);
	}
}

void Druck::createPageNumberRange( )
{
	if (m_doc!=0)
	{
		CreateRange cr(pageNr->text(), m_doc->DocPages.count(), this);
		if (cr.exec())
		{
			CreateRangeData crData;
			cr.getCreateRangeData(crData);
			pageNr->setText(crData.pageRange);
			return;
		}
	}
	pageNr->setText(QString::null);
}
