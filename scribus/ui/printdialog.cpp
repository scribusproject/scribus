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

#include "commonstrings.h"
#include "customfdialog.h"
#include "iconmanager.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "cupsoptions.h"	
#if defined(_WIN32)
	#include <windows.h>
	#include <winspool.h>
#elif defined(HAVE_CUPS) // Haiku doesn't have it
	#include <cups/cups.h>
#endif
#include "sccolor.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scrspinbox.h"
#include "ui/createrange.h"
#include "units.h"
#include "usertaskstructs.h"
#include "util.h"
#include "util_printer.h"

extern bool previewDinUse;

PrintDialog::PrintDialog( QWidget* parent, ScribusDoc* doc, const PrintOptions& printOptions)
		: QDialog( parent )
{
	setupUi(this);
	setModal(true);

	m_doc = doc;
	m_unit = doc->unitIndex();
	m_unitRatio = unitGetRatioFromIndex(doc->unitIndex());
	prefs = PrefsManager::instance().prefsFile->getContext("print_options");
	m_devMode = printOptions.devMode;

	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	pageNrButton->setIcon(IconManager::instance().loadIcon("ellipsis.png"));
	printEngines->addItem( CommonStrings::trPostScript1 );
	printEngines->addItem( CommonStrings::trPostScript2 );
	printEngines->addItem( CommonStrings::trPostScript3 );
	markLength->setNewUnit(m_unit);
	markLength->setMinimum(1 * m_unitRatio);
	markLength->setMaximum(3000 * m_unitRatio);
	markOffset->setNewUnit(m_unit);
	markOffset->setMinimum(0);
	markOffset->setMaximum(3000 * m_unitRatio);
	bleedBottom->setNewUnit(m_unit);
	bleedBottom->setMinimum(0);
	bleedBottom->setMaximum(3000 * m_unitRatio);
	bleedLeft->setNewUnit(m_unit);
	bleedLeft->setMinimum(0);
	bleedLeft->setMaximum(3000 * m_unitRatio);
	bleedRight->setNewUnit(m_unit);
	bleedRight->setMinimum(0);
	bleedRight->setMaximum(3000 * m_unitRatio);
	bleedTop->setNewUnit(m_unit);
	bleedTop->setMinimum(0);
	bleedTop->setMaximum(3000 * m_unitRatio);

	if (ScCore->haveGS() || ScCore->isWinGUI())
		previewButton->setEnabled(!previewDinUse);
	else
	{
		previewButton->setVisible(false);
		previewButton->setEnabled(false);
	}

	// Fill printer list
	QString printerName;
	QStringList printerNames = PrinterUtil::getPrinterNames();
	int numPrinters = printerNames.count();
	for (int i = 0; i < numPrinters; i++)
	{
		printerName = printerNames[i];
		PrintDest->addItem(printerName);
	}
	PrintDest->addItem( tr("File"));

	int prnIndex = PrintDest->findText(printOptions.printer);
	if (prnIndex < 0)
		prnIndex = PrintDest->findText(PrinterUtil::getDefaultPrinterName());
	if (prnIndex >= 0)
	{
		PrintDest->setCurrentIndex(prnIndex);
		prefs->set("CurrentPrn", PrintDest->currentText());
	}

	// Fill Separation list
	ColorList usedSpots;
	doc->getUsedColors(usedSpots, true);
	m_spotColors = usedSpots.keys();

	separationsCombo->addItem( tr("All"), "All" );
	separationsCombo->addItem( tr("Cyan"), "Cyan" );
	separationsCombo->addItem( tr("Magenta"), "Magenta" );
	separationsCombo->addItem( tr("Yellow"), "Yellow" );
	for (int i = 0; i < m_spotColors.count(); ++i)
	{
		const QString& spotName = m_spotColors.at(i);
		separationsCombo->addItem(spotName, spotName);
	}

	if (m_doc->pagePositioning() != 0)
	{
		bleedLeftText->setText( tr( "Inside:" ) );
		bleedRightText->setText( tr( "Outside:" ) );
	}

	QString prnDevice = printOptions.printer;
	if (prnDevice.isEmpty())
		prnDevice = PrintDest->currentText();
	if ((prnDevice == tr("File")) || (PrintDest->count() == 1))
	{
		PrintDest->setCurrentIndex(PrintDest->count()-1);
		prefs->set("CurrentPrn", PrintDest->currentText());
		DateiT->setEnabled(true);
		fileNameEdit->setEnabled(true);
		if (!printOptions.filename.isEmpty())
			fileNameEdit->setText(QDir::toNativeSeparators(printOptions.filename));
		selectFileButton->setEnabled(true);
	}

	setMaximumSize(sizeHint());
	PrintDest->setFocus();

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( okButtonClicked() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( PrintDest, SIGNAL(activated(const QString&)), this, SLOT(selectPrinter(const QString&)));
	connect( printEngines, SIGNAL(activated(const QString&)), this, SLOT(selectEngine(const QString&)));
	connect( printAllRadio, SIGNAL(toggled(bool)), this, SLOT(selectRange(bool)));
	connect( printCurrentRadio, SIGNAL(toggled(bool)), this, SLOT(selectRange(bool)));
	connect( pageNrButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
	connect( printSepCombo, SIGNAL(activated(int)), this, SLOT(selectSepMode(int)));
	connect( selectFileButton, SIGNAL(clicked()), this, SLOT(selectFile()));
	connect( altComCheckBox, SIGNAL(clicked()), this, SLOT(selectCommand()));
	connect( previewButton, SIGNAL(clicked()), this, SLOT(previewButtonClicked()));
	connect( docBleeds, SIGNAL(clicked()), this, SLOT(doDocBleeds()));
	connect( optionsButton, SIGNAL( clicked() ), this, SLOT( selectOptions() ) );

	setStoredValues(printOptions.filename);
#if defined(_WIN32)
	if (!outputToFile())
	{
		m_devMode = printOptions.devMode;
		PrinterUtil::initDeviceSettings(PrintDest->currentText(), m_devMode);
	}
#endif

	m_printEngineMap = PrinterUtil::getPrintEngineSupport(PrintDest->currentText(), outputToFile());
	refreshPrintEngineBox();

	bool ps1Supported = m_printEngineMap.contains(CommonStrings::trPostScript1);
	bool ps2Supported = m_printEngineMap.contains(CommonStrings::trPostScript2);
	bool ps3Supported = m_printEngineMap.contains(CommonStrings::trPostScript3);
	bool psSupported  = (ps1Supported || ps2Supported || ps3Supported);
	printEngines->setEnabled(psSupported || outputToFile());
}

PrintDialog::~PrintDialog()
{
#ifdef HAVE_CUPS
	delete m_cupsOptions;
#endif
	m_cupsOptions = nullptr;
}

void PrintDialog::selectOptions()
{
#ifdef HAVE_CUPS
	if (!m_cupsOptions)
		m_cupsOptions = new CupsOptions(this, PrintDest->currentText());
	if (!m_cupsOptions->exec())
	{
		delete m_cupsOptions; // if options was canceled delete dia 
		m_cupsOptions = nullptr;    // so that getoptions() in the okButtonClicked() will get
		             // the default values from the last successful run
	}

#elif defined(_WIN32)
	bool done;
	Qt::HANDLE handle = nullptr;
	DEVMODEW* devMode = (DEVMODEW*) m_devMode.data();
	// Retrieve the selected printer
	QString printerS = PrintDest->currentText(); 
	// Get a printer handle
	done = OpenPrinterW((LPWSTR) printerS.utf16(), &handle, nullptr);
	if (!done)
		return;
	// Merge stored settings, prompt user and return user settings
	DocumentPropertiesW((HWND) winId(), handle, (LPWSTR) printerS.utf16(), (DEVMODEW*) m_devMode.data(), (DEVMODEW*) m_devMode.data(), 
						DM_IN_BUFFER | DM_IN_PROMPT | DM_OUT_BUFFER);
	// Free the printer handle
	ClosePrinter(handle);

	// With some drivers, one can set the number of copies in print option dialog
	// Set it back to Copies widget in this case
	devMode->dmCopies = qMax(1, qMin((int) devMode->dmCopies, Copies->maximum()));
	if (devMode->dmCopies != numCopies())
	{
		bool sigBlocked = Copies->blockSignals(true);
		Copies->setValue(devMode->dmCopies);
		Copies->blockSignals(false);
	}
#endif
}

QString PrintDialog::getOptions()
{
#ifdef HAVE_CUPS
	QString printerOptions;
	if (!m_cupsOptions)
		m_cupsOptions = new CupsOptions(this, PrintDest->currentText());

	auto printOptions = m_cupsOptions->options();
	for (auto it = printOptions.begin(); it != printOptions.end(); ++it)
	{
		const QString& optionKey = it.key();
		const CupsOptions::OptionData& printOption = it.value(); 
		if (m_cupsOptions->useDefaultValue(optionKey))
			continue;

		if (printOption.keyword == "mirror")
			printerOptions += " -o mirror";
		else if (printOption.keyword == "page-set")
		{
			int pageSetIndex = m_cupsOptions->optionIndex(optionKey);
			if (pageSetIndex > 0)
			{
				printerOptions += " -o " + printOption.keyword + "=";
				printerOptions += (pageSetIndex == 1) ? "even" : "odd";
			}
		}
		else if (printOption.keyword == "number-up")
		{
			printerOptions += " -o " + printOption.keyword + "=";
			switch (m_cupsOptions->optionIndex(optionKey))
			{
				case 0:
					printerOptions += "1";
					break;
				case 1:
					printerOptions += "2";
					break;
				case 2:
					printerOptions += "4";
					break;
				case 3:
					printerOptions += "6";
					break;
				case 4:
					printerOptions += "9";
					break;
				case 5:
					printerOptions += "16";
					break;
			}
		}
		else if (printOption.keyword == "orientation")
			printerOptions += " -o landscape";
		else
		{
			printerOptions += " -o " + printOption.keyword + "=" + m_cupsOptions->optionText(optionKey);
		}
	}
	return printerOptions;
#else
	return QString();
#endif
}

void PrintDialog::selectCommand()
{
	bool test = altComCheckBox->isChecked();
	OthText->setEnabled(test);
	altCommand->setEnabled(test);
	PrintDest->setEnabled(!test);
	if (altComCheckBox->isChecked())
	{
		DateiT->setEnabled(false);
		fileNameEdit->setEnabled(false);
		selectFileButton->setEnabled(false);
		optionsButton->setEnabled(false);
	}
	else
	{
		selectPrinter(PrintDest->currentText());
		if (PrintDest->currentText() != tr("File"))
			optionsButton->setEnabled(true);
	}
}

void PrintDialog::selectEngine(const QString& eng)
{
	prefs->set("CurrentPrnEngine", m_printEngineMap[printEngines->currentText()]);
	bool psSupported = outputToFile();
	psSupported |= (eng == CommonStrings::trPostScript1);
	psSupported |= (eng == CommonStrings::trPostScript2);
	psSupported |= (eng == CommonStrings::trPostScript3);
	if (psSupported)
	{
		printSepCombo->setEnabled(true);
		usePDFMarks->setEnabled(true);
	}
	else
	{
		setCurrentComboItem(printSepCombo, tr("Print Normal"));
		printSepCombo->setEnabled(false);
		setCurrentComboItem(separationsCombo, tr("All"));
		separationsCombo->setEnabled(false);
		usePDFMarks->setEnabled(false);
	}
}

void PrintDialog::selectPrinter(const QString& prn)
{
	bool toFile = prn == tr("File");
	DateiT->setEnabled(toFile);
	fileNameEdit->setEnabled(toFile);
	selectFileButton->setEnabled(toFile);
	optionsButton->setEnabled(!toFile);
#if defined(_WIN32)
	if (!toFile)
	{
		if (!PrinterUtil::getDefaultSettings(PrintDest->currentText(), m_devMode))
			qWarning( tr("Failed to retrieve printer settings").toLatin1().data() );
	}
#endif
	if (toFile && fileNameEdit->text().isEmpty())
	{
		QFileInfo fi(m_doc->documentFileName());
		if (fi.isRelative()) // if (m_doc->DocName.startsWith( tr("Document")))
			fileNameEdit->setText( QDir::toNativeSeparators(QDir::currentPath() + "/" + m_doc->documentFileName() + ".ps") );
		else
		{
			QString completeBaseName = fi.completeBaseName();
			if (completeBaseName.endsWith(".sla", Qt::CaseInsensitive))
				if (completeBaseName.length() > 4) completeBaseName.chop(4);
			if (completeBaseName.endsWith(".gz", Qt::CaseInsensitive))
				if (completeBaseName.length() > 3) completeBaseName.chop(3);
			fileNameEdit->setText( QDir::toNativeSeparators(fi.path() + "/" + completeBaseName + ".ps") );
		}
	}

	// Get page description language supported by the selected printer
	m_printEngineMap = PrinterUtil::getPrintEngineSupport(prn, toFile);
	refreshPrintEngineBox();

	prefs->set("CurrentPrn", prn);
	prefs->set("CurrentPrnEngine", m_printEngineMap[printEngines->currentText()]);
	
	bool ps1Supported = m_printEngineMap.contains(CommonStrings::trPostScript1);
	bool ps2Supported = m_printEngineMap.contains(CommonStrings::trPostScript2);
	bool ps3Supported = m_printEngineMap.contains(CommonStrings::trPostScript3);
	bool psSupported  = (ps1Supported || ps2Supported || ps3Supported);
	if (psSupported || toFile)
	{
		printEngines->setEnabled(true);
		printSepCombo->setEnabled(true);
		usePDFMarks->setEnabled(true);
	}
	else
	{
		printEngines->setEnabled(false);
		setCurrentComboItem(printSepCombo, tr("Print Normal"));
		printSepCombo->setEnabled(false);
		setCurrentComboItem(separationsCombo, tr("All"));
		separationsCombo->setEnabled(false);
		usePDFMarks->setEnabled(false);
	}
}

void PrintDialog::selectRange(bool e)
{
	pageNr->setEnabled(!e);
	pageNrButton->setEnabled(!e);
}

void PrintDialog::selectSepMode(int e)
{
	separationsCombo->setEnabled(e != 0);
}

void PrintDialog::selectFile()
{
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	QString wdir = dirs->get("printdir", ".");
	CustomFDialog dia(this, wdir, tr("Save As"), tr("PostScript Files (*.ps);;All Files (*)"), fdNone | fdHidePreviewCheckBox);
	if (!fileNameEdit->text().isEmpty())
		dia.setSelection(fileNameEdit->text());
	if (dia.exec() == QDialog::Accepted)
	{
		QString selectedFile = dia.selectedFile();
		dirs->set("printdir", selectedFile.left(selectedFile.lastIndexOf("/")));
		fileNameEdit->setText( QDir::toNativeSeparators(selectedFile) );
	}
}

void PrintDialog::setMinMax(int min, int max, int cur)
{
	QString tmp, tmp2;
	printCurrentRadio->setText( tr( "Print Current Pa&ge" )+" ("+tmp.setNum(cur)+")");
	pageNr->setText(tmp.setNum(min)+"-"+tmp2.setNum(max));
}

void PrintDialog::storeValues()
{
	getOptions(); // options were not set get last options with this hack

	m_doc->Print_Options.printer = PrintDest->currentText();
	m_doc->Print_Options.filename = QDir::fromNativeSeparators(fileNameEdit->text());
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
	m_doc->Print_Options.doClip   = doClip();
	m_doc->Print_Options.doGCR    = doGCR();
	m_doc->Print_Options.prnEngine= printEngine();
	m_doc->Print_Options.setDevParam = doDev();
	m_doc->Print_Options.useDocBleeds  = docBleeds->isChecked();
	m_doc->Print_Options.bleeds.setTop(bleedTop->value() / m_doc->unitRatio());
	m_doc->Print_Options.bleeds.setLeft(bleedLeft->value() / m_doc->unitRatio());
	m_doc->Print_Options.bleeds.setRight(bleedRight->value() / m_doc->unitRatio());
	m_doc->Print_Options.bleeds.setBottom(bleedBottom->value() / m_doc->unitRatio());
	m_doc->Print_Options.markLength = markLength->value() / m_doc->unitRatio();
	m_doc->Print_Options.markOffset = markOffset->value() / m_doc->unitRatio();
	m_doc->Print_Options.cropMarks  = cropMarks->isChecked();
	m_doc->Print_Options.bleedMarks = bleedMarks->isChecked();
	m_doc->Print_Options.registrationMarks = registrationMarks->isChecked();
	m_doc->Print_Options.colorMarks = colorMarks->isChecked();
	m_doc->Print_Options.includePDFMarks = usePDFMarks->isChecked();
	if (altComCheckBox->isChecked())
	{
		m_doc->Print_Options.printerCommand = altCommand->text();
		m_doc->Print_Options.useAltPrintCommand = true;
	}
	else
		m_doc->Print_Options.useAltPrintCommand = false;
	m_doc->Print_Options.printerOptions = getOptions();
	m_doc->Print_Options.devMode = m_devMode;
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

void PrintDialog::setStoredValues(const QString& fileName)
{
	if (m_doc->Print_Options.firstUse)
		PrinterUtil::getDefaultPrintOptions(m_doc->Print_Options, m_doc->bleedsVal());
	
	int selectedDest = PrintDest->findText(m_doc->Print_Options.printer);
	if ((selectedDest > -1) && (selectedDest < PrintDest->count()))
	{
		PrintDest->setCurrentIndex(selectedDest);
		prefs->set("CurrentPrn", PrintDest->currentText());
		if (PrintDest->currentText() == tr("File"))
			fileNameEdit->setText(fileName);
		selectPrinter(PrintDest->currentText());
	}
	altComCheckBox->setChecked(m_doc->Print_Options.useAltPrintCommand);
	if (altComCheckBox->isChecked())
	{
		selectCommand();
		altCommand->setText(m_doc->Print_Options.printerCommand);
	}
	printAllRadio->setChecked(prefs->getBool("PrintAll", true));
	printCurrentRadio->setChecked(prefs->getBool("CurrentPage", false));
	bool printRangeChecked = prefs->getBool("PrintRange", false);
	printRangeRadio->setChecked(printRangeChecked);
	pageNr->setEnabled(printRangeChecked);
	pageNr->setText(prefs->get("PageNr", "1-1"));
	Copies->setValue(1);
	printSepCombo->setCurrentIndex(m_doc->Print_Options.outputSeparations);
	colorType->setCurrentIndex(m_doc->Print_Options.useColor ? 0 : 1);
	int selectedSep = separationsCombo->findData(m_doc->Print_Options.separationName);
	separationsCombo->setCurrentIndex((selectedSep >= 0) ? selectedSep : 0);
	if (printSepCombo->currentIndex() == 1)
		separationsCombo->setEnabled(true);
	setPrintEngine(m_doc->Print_Options.prnEngine);
	mirrorHor->setChecked(m_doc->Print_Options.mirrorH);
	mirrorVert->setChecked(m_doc->Print_Options.mirrorV);
	setMediaSize->setChecked(m_doc->Print_Options.setDevParam);
	applyGCR->setChecked(m_doc->Print_Options.doGCR);
	clipMargins->setChecked(m_doc->Print_Options.doClip);
	convertSpots->setChecked(!m_doc->Print_Options.useSpotColors);
	docBleeds->setChecked(m_doc->Print_Options.useDocBleeds);
	if (docBleeds->isChecked())
	{
		bleedTop->setValue(m_doc->bleeds()->top() * m_unitRatio);
		bleedBottom->setValue(m_doc->bleeds()->bottom() * m_unitRatio);
		bleedRight->setValue(m_doc->bleeds()->right() * m_unitRatio);
		bleedLeft->setValue(m_doc->bleeds()->left() * m_unitRatio);
	}
	else
	{
		bleedTop->setValue(m_doc->Print_Options.bleeds.top() * m_unitRatio);
		bleedBottom->setValue(m_doc->Print_Options.bleeds.bottom() * m_unitRatio);
		bleedRight->setValue(m_doc->Print_Options.bleeds.right() * m_unitRatio);
		bleedLeft->setValue(m_doc->Print_Options.bleeds.left() * m_unitRatio);
	}
	bleedTop->setEnabled(!docBleeds->isChecked());
	bleedBottom->setEnabled(!docBleeds->isChecked());
	bleedRight->setEnabled(!docBleeds->isChecked());
	bleedLeft->setEnabled(!docBleeds->isChecked());
	markLength->setValue(m_doc->Print_Options.markLength * m_unitRatio);
	markOffset->setValue(m_doc->Print_Options.markOffset * m_unitRatio);
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
	return QDir::fromNativeSeparators(fileNameEdit->text());
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
	return separationsCombo->isEnabled();
}

QString PrintDialog::separationName()
{
	if (separationsCombo->currentIndex() == 0)
		return QString("All");
	if (separationsCombo->currentIndex() == 1)
		return QString("Cyan");
	if (separationsCombo->currentIndex() == 2)
		return QString("Magenta");
	if (separationsCombo->currentIndex() == 3)
		return QString("Yellow");
	if (separationsCombo->currentIndex() == 4)
		return QString("Black");
	return separationsCombo->currentText();
}

QStringList PrintDialog::allSeparations()
{
	QStringList ret;
	for (int a = 1; a < separationsCombo->count(); ++a)
	{
		ret.append(separationsCombo->itemText(a));
	}
	return ret;
}

bool PrintDialog::color()
{
	return colorType->currentIndex() == 0;
}

bool PrintDialog::mirrorHorizontal()
{
	return mirrorHor->isChecked();
}

bool PrintDialog::mirrorVertical()
{
	return mirrorVert->isChecked();
}

bool PrintDialog::doGCR()
{
	return applyGCR->isChecked();
}

bool PrintDialog::doClip()
{
	return clipMargins->isChecked();
}

PrintEngine PrintDialog::printEngine()
{
	return m_printEngineMap[printEngines->currentText()];
}

bool PrintDialog::doDev()
{
	return setMediaSize->isChecked();
}

bool PrintDialog::doSpot()
{
	return !convertSpots->isChecked();
}

bool PrintDialog::doPrintAll()
{
	return printAllRadio->isChecked();
}

bool PrintDialog::doPrintCurrentPage()
{
	return printCurrentRadio->isChecked();
}

QString PrintDialog::getPageString()
{
	return pageNr->text();
}

void PrintDialog::doDocBleeds()
{
	if (docBleeds->isChecked())
	{
		prefs->set("BleedTop", bleedTop->value() / m_unitRatio);
		prefs->set("BleedBottom", bleedBottom->value() / m_unitRatio);
		prefs->set("BleedRight", bleedRight->value() / m_unitRatio);
		prefs->set("BleedLeft", bleedLeft->value() / m_unitRatio);
		bleedTop->setValue(m_doc->bleeds()->top() * m_unitRatio);
		bleedBottom->setValue(m_doc->bleeds()->bottom() * m_unitRatio);
		bleedRight->setValue(m_doc->bleeds()->right() * m_unitRatio);
		bleedLeft->setValue(m_doc->bleeds()->left() * m_unitRatio);
	}
	else
	{
		bleedTop->setValue(prefs->getDouble("BleedTop",0.0) * m_unitRatio);
		bleedBottom->setValue(prefs->getDouble("BleedBottom",0.0) * m_unitRatio);
		bleedRight->setValue(prefs->getDouble("BleedRight",0.0) * m_unitRatio);
		bleedLeft->setValue(prefs->getDouble("BleedLeft",0.0) * m_unitRatio);
	}
	bool isChecked = docBleeds->isChecked();
	prefs->set("UseDocBleeds", isChecked);
	bleedTop->setEnabled(!isChecked);
	bleedBottom->setEnabled(!isChecked);
	bleedRight->setEnabled(!isChecked);
	bleedLeft->setEnabled(!isChecked);
}

void PrintDialog::createPageNumberRange( )
{
	if (m_doc!=nullptr)
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
	pageNr->setText(QString());
}

void PrintDialog::refreshPrintEngineBox()
{
	int index = 0, oldPDLIndex = -1;
	QString oldPDL  = printEngines->currentText();
	PrintEngineMap::Iterator it, itEnd = m_printEngineMap.end();
	printEngines->clear();
	for (it = m_printEngineMap.begin(); it != itEnd; ++it)
	{
		printEngines->addItem(it.key());
		if (it.key() == oldPDL)
			oldPDLIndex = index;
		index++;
	}
	// Try to not default on PostScript 1 when switching
	// from a GDI printer to a Postscript printer
	if (oldPDLIndex < 0)
	{
		oldPDLIndex = printEngines->findText(CommonStrings::trPostScript3);
		if (oldPDLIndex < 0)
			oldPDLIndex = 0;
	}
	printEngines->setCurrentIndex(oldPDLIndex);
}

void PrintDialog::setPrintEngine(PrintEngine engine)
{
	QString pdlString(m_printEngineMap.key(engine, ""));
	int itemIndex = printEngines->findText(pdlString);
	if (itemIndex >= 0)
		printEngines->setCurrentIndex(itemIndex);
	else if (printEngines->count() > 0)
	{
		pdlString = m_printEngineMap.key(PostScript3, "");
		itemIndex = printEngines->findText(pdlString);
		if (itemIndex >= 0)
			printEngines->setCurrentIndex(itemIndex);
		else
			printEngines->setCurrentIndex(printEngines->count() - 1);
	}
}


