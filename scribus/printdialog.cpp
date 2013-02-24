/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "printdialog.h"

#include <QDir>
#include <QMap>
#include <QStringList>
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
#if defined(_WIN32)
	#include <windows.h>
	#include <winspool.h>
#else
    #if defined (HAVE_CUPS)
	#include <cups/cups.h>
    #endif
#endif
#include "util_printer.h"
#include "util_icon.h"
#include "util.h"
#include "units.h"
#include "usertaskstructs.h"
#include "scrspinbox.h"

extern bool previewDinUse;

PrintDialog::PrintDialog( QWidget* parent, ScribusDoc* doc, const PrintOptions& printOptions, bool gcr, QStringList spots)
		: QDialog( parent )
{
	setupUi(this);
	setModal(true);
	cdia = 0;
	m_doc = doc;
	unit = doc->unitIndex();
	unitRatio = unitGetRatioFromIndex(doc->unitIndex());
	prefs = PrefsManager::instance()->prefsFile->getContext("print_options");
	DevMode = printOptions.devMode;
	PrinterOpts = "";
	setWindowIcon(QIcon(loadIcon("AppIcon.png")));
 	pageNrButton->setIcon(QIcon(loadIcon("ellipsis.png")));
	printEngines->addItem( CommonStrings::trPostScript1 );
	printEngines->addItem( CommonStrings::trPostScript2 );
	printEngines->addItem( CommonStrings::trPostScript3 );
	markOffset->setNewUnit(unit);
	markOffset->setMinimum(0);
	markOffset->setMaximum(3000*unitRatio);
	BleedBottom->setNewUnit(unit);
	BleedBottom->setMinimum(0);
	BleedBottom->setMaximum(3000*unitRatio);
	BleedLeft->setNewUnit(unit);
	BleedLeft->setMinimum(0);
	BleedLeft->setMaximum(3000*unitRatio);
	BleedRight->setNewUnit(unit);
	BleedRight->setMinimum(0);
	BleedRight->setMaximum(3000*unitRatio);
	BleedTop->setNewUnit(unit);
	BleedTop->setMinimum(0);
	BleedTop->setMaximum(3000*unitRatio);
	previewButton->setEnabled(!previewDinUse);
	// Fill printer list
	QString Pcap;
	QString printerName;
	QStringList printerNames = PrinterUtil::getPrinterNames();
	int numPrinters = printerNames.count();
	for( int i = 0; i < numPrinters; i++)
	{
		printerName = printerNames[i];
		PrintDest->addItem(printerName);
		if( printerName == printOptions.printer )
		{
			PrintDest->setCurrentIndex(PrintDest->count()-1);
			prefs->set("CurrentPrn", PrintDest->currentText());
		}
	}

	PrintDest->addItem( tr("File"));

	// Fill Separation list
	QString sep[] =
	    {
	        tr("All"), tr("Cyan"), tr("Magenta"), tr("Yellow"),
	        tr("Black")
	    };
	size_t sepArray = sizeof(sep) / sizeof(*sep);
	for (uint prop = 0; prop < sepArray; ++prop)
		SepArt->addItem(sep[prop]);
	SepArt->addItems(spots);

	if (m_doc->currentPageLayout != 0)
	{
		BleedTxt3->setText( tr( "Inside:" ) );
		BleedTxt4->setText( tr( "Outside:" ) );
	}

	QString prnDevice = printOptions.printer;
	if (prnDevice.isEmpty())
		prnDevice = PrintDest->currentText();
	if ((prnDevice == tr("File")) || (PrintDest->count() == 1))
	{
		PrintDest->setCurrentIndex(PrintDest->count()-1);
		prefs->set("CurrentPrn", PrintDest->currentText());
		DateiT->setEnabled(true);
		LineEdit1->setEnabled(true);
		if (!printOptions.filename.isEmpty())
			LineEdit1->setText(QDir::toNativeSeparators(printOptions.filename));
		ToolButton1->setEnabled(true);
	}

	setMaximumSize(sizeHint());
	PrintDest->setFocus();
	// signals and slots connections
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( okButtonClicked() ) );
	connect( OKButton_2, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( PrintDest, SIGNAL(activated(const QString&)), this, SLOT(SelPrinter(const QString&)));
	connect( printEngines, SIGNAL(activated(const QString&)), this, SLOT(SelEngine(const QString&)));
	connect( RadioButton1, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect( CurrentPage, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
	connect( pageNrButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
	connect( PrintSep, SIGNAL(activated(int)), this, SLOT(SelMode(int)));
	connect( ToolButton1, SIGNAL(clicked()), this, SLOT(SelFile()));
	connect( OtherCom, SIGNAL(clicked()), this, SLOT(SelComm()));
	connect( previewButton, SIGNAL(clicked()), this, SLOT(previewButtonClicked()));
	connect( docBleeds, SIGNAL(clicked()), this, SLOT(doDocBleeds()));
	connect( OptButton, SIGNAL( clicked() ), this, SLOT( SetOptions() ) );

	setStoredValues(printOptions.filename, gcr);
#if defined(_WIN32)
	if (!outputToFile())
	{
		DevMode = printOptions.devMode;
		PrinterUtil::initDeviceSettings( PrintDest->currentText(), DevMode );
	}
#endif

	printEngineMap = PrinterUtil::getPrintEngineSupport(PrintDest->currentText(), outputToFile());
	refreshPrintEngineBox();

	bool ps1Supported = printEngineMap.contains(CommonStrings::trPostScript1);
	bool ps2Supported = printEngineMap.contains(CommonStrings::trPostScript2);
	bool ps3Supported = printEngineMap.contains(CommonStrings::trPostScript3);
	bool psSupported  = (ps1Supported || ps2Supported || ps3Supported);
	printEngines->setEnabled(psSupported || outputToFile());
	UseICC->setEnabled(m_doc->HasCMS && psSupported);
}

PrintDialog::~PrintDialog()
{
#ifdef HAVE_CUPS
	delete cdia;
#endif
	cdia = 0;
}

void PrintDialog::SetOptions()
{
#ifdef HAVE_CUPS
	PrinterOpts = "";
	if (!cdia)
		cdia = new CupsOptions(this, PrintDest->currentText());
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

void PrintDialog::getOptions()
{
#ifdef HAVE_CUPS
	PrinterOpts = "";
	if (!cdia)
		cdia = new CupsOptions(this, PrintDest->currentText());
	struct CupsOptions::OpData daten;
	QMap<QString,CupsOptions::OpData>::Iterator it;
	for (it = cdia->KeyToText.begin(); it != cdia->KeyToText.end(); ++it)
	{
		if (cdia->KeyToDefault[it.key()] == cdia->FlagsOpt.at(it.value().Cnum)->currentText())
			continue;
		
		if (it.value().KeyW == "mirror")
			PrinterOpts += " -o mirror";
		else if (it.value().KeyW == "page-set")
		{
			PrinterOpts += " -o " + it.value().KeyW + "=";
			if (cdia->FlagsOpt.at(it.value().Cnum)->currentIndex() == 1)
				PrinterOpts += "even";
			else
				PrinterOpts += "odd";
		}
		else if (it.value().KeyW == "number-up")
		{
			PrinterOpts += " -o " + it.value().KeyW + "=";
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
		else if (it.value().KeyW == "orientation")
			PrinterOpts += " -o landscape";
		else
		{
			PrinterOpts += " -o " +
				it.value().KeyW+"="+cdia->FlagsOpt.at(it.value().Cnum)->currentText();
		}
	}
#endif
}

void PrintDialog::SelComm()
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
		OptButton->setEnabled(false);
	}
	else
	{
		SelPrinter(PrintDest->currentText());
		if (PrintDest->currentText() != tr("File"))
			OptButton->setEnabled(true);
	}
}

void PrintDialog::SelEngine(const QString& eng)
{
	prefs->set("CurrentPrnEngine", printEngineMap[printEngines->currentText()]);
	bool psSupported = outputToFile();
	psSupported |= (eng == CommonStrings::trPostScript1);
	psSupported |= (eng == CommonStrings::trPostScript2);
	psSupported |= (eng == CommonStrings::trPostScript3);
	if (psSupported)
	{
		PrintSep->setEnabled( true );
		UseICC->setEnabled( m_doc->HasCMS );
		usePDFMarks->setEnabled(true);
	}
	else
	{
		setCurrentComboItem(PrintSep, tr("Print Normal"));
		PrintSep->setEnabled( false );
		setCurrentComboItem(SepArt, tr("All"));
		SepArt->setEnabled( false );
		UseICC->setEnabled( false );
		UseICC->setChecked( false );
		usePDFMarks->setEnabled(false);
	}
}

void PrintDialog::SelPrinter(const QString& prn)
{
	bool toFile = prn == tr("File") ? true : false;
	DateiT->setEnabled(toFile);
	LineEdit1->setEnabled(toFile);
	ToolButton1->setEnabled(toFile);
	OptButton->setEnabled(!toFile);
#if defined(_WIN32)
	if (!toFile)
	{
		if( !PrinterUtil::getDefaultSettings(PrintDest->currentText(), DevMode) )
			qWarning( tr("Failed to retrieve printer settings").toAscii().data() );
	}
#endif
	if (toFile && LineEdit1->text().isEmpty())
	{
		QFileInfo fi(m_doc->DocName);
		if (fi.isRelative()) // if (m_doc->DocName.startsWith( tr("Document")))
			LineEdit1->setText( QDir::toNativeSeparators(QDir::currentPath() + "/" + m_doc->DocName + ".ps") );
		else
		{
			QString completeBaseName = fi.completeBaseName();
			if (completeBaseName.endsWith(".sla", Qt::CaseInsensitive))
				if (completeBaseName.length() > 4) completeBaseName.chop(4);
			if (completeBaseName.endsWith(".gz", Qt::CaseInsensitive))
				if (completeBaseName.length() > 3) completeBaseName.chop(3);
			LineEdit1->setText( QDir::toNativeSeparators(fi.path() + "/" + completeBaseName + ".ps") );
		}
	}

	// Get page description language supported by the selected printer
	printEngineMap = PrinterUtil::getPrintEngineSupport(prn, toFile);
	refreshPrintEngineBox();

	prefs->set("CurrentPrn", prn);
	prefs->set("CurrentPrnEngine", printEngineMap[printEngines->currentText()]);
	
	bool ps1Supported = printEngineMap.contains(CommonStrings::trPostScript1);
	bool ps2Supported = printEngineMap.contains(CommonStrings::trPostScript2);
	bool ps3Supported = printEngineMap.contains(CommonStrings::trPostScript3);
	bool psSupported  = (ps1Supported || ps2Supported || ps3Supported);
	if (psSupported || toFile)
	{
		printEngines->setEnabled( true );
		PrintSep->setEnabled( true );
		UseICC->setEnabled( m_doc->HasCMS );
		usePDFMarks->setEnabled(true);
	}
	else
	{
		printEngines->setEnabled( false );
		setCurrentComboItem(PrintSep, tr("Print Normal"));
		PrintSep->setEnabled( false );
		setCurrentComboItem(SepArt, tr("All"));
		SepArt->setEnabled( false );
		UseICC->setEnabled( false );
		UseICC->setChecked( false );
		usePDFMarks->setEnabled(false);
	}
}

void PrintDialog::SelRange(bool e)
{
	pageNr->setEnabled(!e);
	pageNrButton->setEnabled(!e);
}

void PrintDialog::SelMode(int e)
{
	SepArt->setEnabled( (e != 0) );
}

void PrintDialog::SelFile()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("printdir", ".");
	CustomFDialog dia(this, wdir, tr("Save As"), tr("PostScript Files (*.ps);;All Files (*)"), fdNone | fdHidePreviewCheckBox);
	if (!LineEdit1->text().isEmpty())
		dia.setSelection(LineEdit1->text());
	if (dia.exec() == QDialog::Accepted)
	{
		QString selectedFile = dia.selectedFile();
		dirs->set("printdir", selectedFile.left(selectedFile.lastIndexOf("/")));
		LineEdit1->setText( QDir::toNativeSeparators(selectedFile) );
	}
}

void PrintDialog::setMinMax(int min, int max, int cur)
{
	QString tmp, tmp2;
	CurrentPage->setText( tr( "Print Current Pa&ge" )+" ("+tmp.setNum(cur)+")");
	pageNr->setText(tmp.setNum(min)+"-"+tmp2.setNum(max));
}

void PrintDialog::storeValues()
{
	getOptions(); // options were not set get last options with this hack

	m_doc->Print_Options.printer = PrintDest->currentText();
	m_doc->Print_Options.filename = QDir::fromNativeSeparators(LineEdit1->text());
	m_doc->Print_Options.toFile = outputToFile();
	m_doc->Print_Options.copies = numCopies();
	m_doc->Print_Options.outputSeparations = outputSeparations();
	m_doc->Print_Options.separationName = separationName();
	m_doc->Print_Options.allSeparations = allSeparations();
	if (m_doc->Print_Options.outputSeparations)
		m_doc->Print_Options.useSpotColors = true;
	else
		m_doc->Print_Options.useSpotColors = doSpot();
	m_doc->Print_Options.useColor = color();
	m_doc->Print_Options.mirrorH  = mirrorHorizontal();
	m_doc->Print_Options.mirrorV  = mirrorVertical();
	m_doc->Print_Options.useICC   = ICCinUse();
	m_doc->Print_Options.doClip   = doClip();
	m_doc->Print_Options.doGCR    = doGCR();
	m_doc->Print_Options.prnEngine= printEngine();
	m_doc->Print_Options.setDevParam = doDev();
	m_doc->Print_Options.useDocBleeds  = docBleeds->isChecked();
	m_doc->Print_Options.bleeds.Top    = BleedTop->value() / m_doc->unitRatio();
	m_doc->Print_Options.bleeds.Left   = BleedLeft->value() / m_doc->unitRatio();
	m_doc->Print_Options.bleeds.Right  = BleedRight->value() / m_doc->unitRatio();
	m_doc->Print_Options.bleeds.Bottom = BleedBottom->value() / m_doc->unitRatio();
	m_doc->Print_Options.markOffset = markOffset->value() / m_doc->unitRatio();
	m_doc->Print_Options.cropMarks  = cropMarks->isChecked();
	m_doc->Print_Options.bleedMarks = bleedMarks->isChecked();
	m_doc->Print_Options.registrationMarks = registrationMarks->isChecked();
	m_doc->Print_Options.colorMarks = colorMarks->isChecked();
	m_doc->Print_Options.includePDFMarks = usePDFMarks->isChecked();
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
	m_doc->Print_Options.printerOptions = QString();
#endif
	m_doc->Print_Options.devMode = DevMode;
}

void PrintDialog::okButtonClicked()
{
	storeValues();
	accept();
}

void PrintDialog::previewButtonClicked()
{
	storeValues();
	emit doPreview();
}

void PrintDialog::getDefaultPrintOptions(PrintOptions& options, bool gcr)
{
	QStringList spots;
	options.firstUse = true;
	options.printer  = prefs->get("CurrentPrn", "");
	options.useAltPrintCommand = prefs->getBool("OtherCom", false);
	options.printerCommand = prefs->get("Command", "");
	options.outputSeparations = prefs->getInt("Separations", 0);
	options.useColor = (prefs->getInt("PrintColor", 0) == 0);
	spots << tr("All") << tr("Cyan") << tr("Magenta") << tr("Yellow") << tr("Black");
	int selectedSep  = prefs->getInt("SepArt", 0);
	if ((selectedSep < 0) || (selectedSep > 4))
		selectedSep = 0;
	options.separationName = spots.at(selectedSep);
	options.prnEngine = (PrintEngine) prefs->getInt("PSLevel", PostScript3);
	options.mirrorH = prefs->getBool("MirrorH", false);
	options.mirrorV = prefs->getBool("MirrorV", false);
	options.setDevParam = prefs->getBool("doDev", false);
	options.doGCR   = prefs->getBool("DoGCR", gcr);
	options.doClip  = prefs->getBool("Clip", false);
	options.useSpotColors = prefs->getBool("doSpot", true);
	options.useICC  = m_doc->HasCMS ? prefs->getBool("ICCinUse", false) : false;
	options.useDocBleeds  = true;
	options.bleeds.Top    = m_doc->bleeds.Top;
	options.bleeds.Bottom = m_doc->bleeds.Bottom;
	options.bleeds.Right  = m_doc->bleeds.Right;
	options.bleeds.Left   = m_doc->bleeds.Left;
	options.markOffset = prefs->getDouble("markOffset",0.0);
	options.cropMarks  = prefs->getBool("cropMarks", false);
	options.bleedMarks = prefs->getBool("bleedMarks", false);
	options.registrationMarks = prefs->getBool("registrationMarks", false);
	options.colorMarks = prefs->getBool("colorMarks", false);
	options.includePDFMarks = prefs->getBool("includePDFMarks", true);
}

void PrintDialog::setStoredValues(const QString& fileName, bool gcr)
{
	if (m_doc->Print_Options.firstUse)
		getDefaultPrintOptions(m_doc->Print_Options, gcr);
	
	int selectedDest = PrintDest->findText(m_doc->Print_Options.printer);
	if ((selectedDest > -1) && (selectedDest < PrintDest->count()))
	{
		PrintDest->setCurrentIndex(selectedDest);
		prefs->set("CurrentPrn", PrintDest->currentText());
		if (PrintDest->currentText() == tr("File"))
			LineEdit1->setText(fileName);
		SelPrinter(PrintDest->currentText());
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
	colorType->setCurrentIndex(m_doc->Print_Options.useColor ? 0 : 1);
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
		SepArt->setEnabled(true);
	setPrintEngine(m_doc->Print_Options.prnEngine);
	MirrorHor->setChecked(m_doc->Print_Options.mirrorH);
	MirrorVert->setChecked(m_doc->Print_Options.mirrorV);
	devPar->setChecked(m_doc->Print_Options.setDevParam);
	GcR->setChecked(m_doc->Print_Options.doGCR);
	ClipMarg->setChecked(m_doc->Print_Options.doClip);
	spotColors->setChecked(!m_doc->Print_Options.useSpotColors);
	bool iccInUse  = m_doc->HasCMS ? m_doc->Print_Options.useICC : false;
	bool psPrinter = PrinterUtil::isPostscriptPrinter(PrintDest->currentText()) || outputToFile();
	UseICC->setChecked( psPrinter ? iccInUse : false );
	UseICC->setEnabled( psPrinter );
	docBleeds->setChecked(m_doc->Print_Options.useDocBleeds);
	if (docBleeds->isChecked())
	{
		BleedTop->setValue(m_doc->bleeds.Top*unitRatio);
		BleedBottom->setValue(m_doc->bleeds.Bottom*unitRatio);
		BleedRight->setValue(m_doc->bleeds.Right*unitRatio);
		BleedLeft->setValue(m_doc->bleeds.Left*unitRatio);
	}
	else
	{
		BleedTop->setValue(m_doc->Print_Options.bleeds.Top*unitRatio);
		BleedBottom->setValue(m_doc->Print_Options.bleeds.Bottom*unitRatio);
		BleedRight->setValue(m_doc->Print_Options.bleeds.Right*unitRatio);
		BleedLeft->setValue(m_doc->Print_Options.bleeds.Left*unitRatio);
	}
	BleedTop->setEnabled(!docBleeds->isChecked());
	BleedBottom->setEnabled(!docBleeds->isChecked());
	BleedRight->setEnabled(!docBleeds->isChecked());
	BleedLeft->setEnabled(!docBleeds->isChecked());
	markOffset->setValue(m_doc->Print_Options.markOffset*unitRatio);
	cropMarks->setChecked(m_doc->Print_Options.cropMarks);
	bleedMarks->setChecked(m_doc->Print_Options.bleedMarks);
	registrationMarks->setChecked(m_doc->Print_Options.registrationMarks);
	colorMarks->setChecked(m_doc->Print_Options.colorMarks);
	usePDFMarks->setChecked(m_doc->Print_Options.includePDFMarks);
}

QString PrintDialog::printerName()
{
	return PrintDest->currentText();
}

QString PrintDialog::outputFileName()
{
	return QDir::fromNativeSeparators(LineEdit1->text());
}

bool PrintDialog::outputToFile()
{
	return (PrintDest->currentText() == tr("File"));
}

int PrintDialog::numCopies()
{
	return Copies->value();
}

bool PrintDialog::outputSeparations()
{
	return SepArt->isEnabled();
}

QString PrintDialog::separationName()
{
	return SepArt->currentText();
}

QStringList PrintDialog::allSeparations()
{
	QStringList ret;
	for (int a = 1; a < SepArt->count(); ++a)
	{
		ret.append(SepArt->itemText(a));
	}
	return ret;
}

bool PrintDialog::color()
{
	if (colorType->currentIndex() == 0)
		return true;
	else
		return false;
}

bool PrintDialog::mirrorHorizontal()
{
	return MirrorHor->isChecked();
}

bool PrintDialog::mirrorVertical()
{
	return MirrorVert->isChecked();
}

bool PrintDialog::doGCR()
{
	return GcR->isChecked();
}

bool PrintDialog::doClip()
{
	return ClipMarg->isChecked();
}

PrintEngine PrintDialog::printEngine()
{
	return printEngineMap[printEngines->currentText()];
}

bool PrintDialog::doDev()
{
	return devPar->isChecked();
}

bool PrintDialog::doSpot()
{
	return !spotColors->isChecked();
}

bool PrintDialog::ICCinUse()
{
	return (m_doc->HasCMS && UseICC->isChecked());
}

bool PrintDialog::doPrintAll()
{
	return RadioButton1->isChecked();
}

bool PrintDialog::doPrintCurrentPage()
{
	return CurrentPage->isChecked();
}

QString PrintDialog::getPageString()
{
	return pageNr->text();
}

void PrintDialog::doDocBleeds()
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
	}
	else
	{
		BleedTop->setValue(prefs->getDouble("BleedTop",0.0)*unitRatio);
		BleedBottom->setValue(prefs->getDouble("BleedBottom",0.0)*unitRatio);
		BleedRight->setValue(prefs->getDouble("BleedRight",0.0)*unitRatio);
		BleedLeft->setValue(prefs->getDouble("BleedLeft",0.0)*unitRatio);
	}
	bool isChecked = docBleeds->isChecked();
	prefs->set("UseDocBleeds", isChecked);
	BleedTop->setEnabled(!isChecked);
	BleedBottom->setEnabled(!isChecked);
	BleedRight->setEnabled(!isChecked);
	BleedLeft->setEnabled(!isChecked);
}

void PrintDialog::createPageNumberRange( )
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

void PrintDialog::refreshPrintEngineBox()
{
	int index = 0, oldPDLIndex = 0;
	QString oldPDL  = printEngines->currentText();
	PrintEngineMap::Iterator it, itEnd = printEngineMap.end();
	printEngines->clear();
	for (it = printEngineMap.begin(); it != itEnd; ++it)
	{
		printEngines->addItem(it.key());
		if (it.key() == oldPDL)
			oldPDLIndex = index;
		index++;
	}
	printEngines->setCurrentIndex( oldPDLIndex );
}

void PrintDialog::setPrintEngine(PrintEngine pdl)
{
	QString pdlString = printEngineMap.key(pdl, "");
	int itemIndex = printEngines->findText(pdlString);
	if (itemIndex >= 0)
		printEngines->setCurrentIndex(itemIndex);
	else if(printEngines->count() > 0)
	{
		pdlString = printEngineMap.key(PostScript3, "");
		itemIndex = printEngines->findText(pdlString);
		if (itemIndex >= 0)
			printEngines->setCurrentIndex(itemIndex);
		else
			printEngines->setCurrentIndex(printEngines->count() - 1);
	}
}


