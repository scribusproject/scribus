/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QToolTip>
#include <QLineEdit>
#include <QRadioButton>

#include "tabprinter.h"
#include "ui/scrspinbox.h"
#include "prefsmanager.h"
#include "units.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "util_printer.h"


TabPrinter::TabPrinter(QWidget* parent, const char* name)
	: QWidget(parent)
{
	setupUi(this);
	setObjectName(name);
	bleedTop->setToolTip( "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
	bleedBottom->setToolTip( "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
	bleedLeft->setToolTip( "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
	bleedRight->setToolTip( "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
	setClip->setToolTip( "<qt>" + tr( "Do not show objects outside the margins on the printed page" ) + "</qt>" );
	useAltPrintCommand->setToolTip("<qt>" + tr( "Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options") + "</qt>" );
	psLevel->setToolTip("<qt>" +  tr( "Sets the PostScript Level.\n Setting to Level 1 or 2 can create huge files" ) + "</qt>" );
	doGCR->setToolTip( "<qt>" + tr( "A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks." ) + "</qt>");
	convertSpots->setToolTip("<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");
	useICC->setToolTip("<qt>" + tr( "Allows you to embed color profiles in the print stream when color management is enabled" ) + "</qt>");
	setMedia->setToolTip( "<qt>" + tr( "This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer." ) + "</qt>");
	connect(useAltPrintCommand, SIGNAL(clicked()), this, SLOT(selOtherComm()));
}

void TabPrinter::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	docUnitIndex = prefsData->docUnitIndex;
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
	QString unitSuffix = unitGetSuffixFromIndex(docUnitIndex);
	bleedTop->setNewUnit(docUnitIndex);
	bleedTop->setMinimum(0.0);
	bleedTop->setMaximum(3000*unitRatio);
	bleedBottom->setNewUnit(docUnitIndex);
	bleedBottom->setMinimum(0.0);
	bleedBottom->setMaximum(3000*unitRatio);
	bleedLeft->setNewUnit(docUnitIndex);
	bleedLeft->setMinimum(0.0);
	bleedLeft->setMaximum(3000*unitRatio);
	bleedRight->setNewUnit(docUnitIndex);
	bleedRight->setMinimum(0.0);
	bleedRight->setMaximum(3000*unitRatio);
	offsetValue->setNewUnit(docUnitIndex);
	offsetValue->setMinimum(0.0);
	offsetValue->setMaximum(3000*unitRatio);

	defaultPrinter->setMinimumSize( QSize( 250, 22 ) );
	defaultPrinter->setMaximumSize( QSize( 260, 30 ) );
	defaultPrinter->setEditable(false);
	QString Pcap;
	QString printerName;
	QStringList printerNames = PrinterUtil::getPrinterNames();
	int numPrinters = printerNames.count();
	for( int i = 0; i < numPrinters; i++)
	{
		printerName = printerNames[i];
		defaultPrinter->addItem(printerName);
	}

	defaultPrinter->addItem( tr("File"));

	prefs = PrefsManager::instance()->prefsFile->getContext("print_options");

	QString selectedDest = prefs->get("CurrentPrn", "");
	int prnIndex = defaultPrinter->findText(selectedDest);
	if ((prnIndex > -1) && (prnIndex < defaultPrinter->count()))
		defaultPrinter->setCurrentIndex(prnIndex);
	useAltPrintCommand->setChecked(prefs->getBool("OtherCom", false));
	if (useAltPrintCommand->isChecked())
	{
		defaultPrinter->setEnabled(false);
		printerCommand->setEnabled(true);
	}
	else
	{
		defaultPrinter->setEnabled(true);
		printerCommand->setEnabled(false);
	}
	printerCommand->setText(prefs->get("Command", ""));
	bool iccInUse = prefs->getBool("ICCinUse", false);
	bool psPrinter = PrinterUtil::isPostscriptPrinter(defaultPrinter->currentText());
	useICC->setChecked( psPrinter ? iccInUse : false );
	useICC->setEnabled( psPrinter );
	bool seps = static_cast<bool>(prefs->getInt("Separations", 0));
	buttonNormal->setChecked(!seps);
	buttonSeparations->setChecked(seps);
	bool color = static_cast<bool>(prefs->getInt("PrintColor", 0));
	printGray->setChecked(color);
	printColor->setChecked(!color);
	psLevel->setCurrentIndex(prefs->getInt("PSLevel", 3)-1);
	mirrorH->setChecked(prefs->getBool("MirrorH", false));
	mirrorV->setChecked(prefs->getBool("MirrorV", false));
	setMedia->setChecked(prefs->getBool("doDev", false));
	doGCR->setChecked(prefs->getBool("DoGCR", false));
	setClip->setChecked(prefs->getBool("Clip", false));
	convertSpots->setChecked(!prefs->getBool("doSpot", true));
	bleedTop->setValue(prefs->getDouble("BleedTop",0.0)*unitRatio);
	bleedBottom->setValue(prefs->getDouble("BleedBottom",0.0)*unitRatio);
	bleedRight->setValue(prefs->getDouble("BleedRight",0.0)*unitRatio);
	bleedLeft->setValue(prefs->getDouble("BleedLeft",0.0)*unitRatio);
	offsetValue->setValue(prefs->getDouble("markOffset",0.0)*unitRatio);
	cropMarks->setChecked(prefs->getBool("cropMarks", false));
	bleedMarks->setChecked(prefs->getBool("bleedMarks", false));
	registrationMarks->setChecked(prefs->getBool("registrationMarks", false));
	colorMarks->setChecked(prefs->getBool("colorMarks", false));
	usePDFMarks->setChecked(prefs->getBool("includePDFMarks", true));
}

void TabPrinter::storeValues()
{
	prefs->set("CurrentPrn", defaultPrinter->currentText());
	prefs->set("OtherCom", useAltPrintCommand->isChecked());
	prefs->set("Command", printerCommand->text());
	prefs->set("PrintAll", true);
	prefs->set("CurrentPage", false);
	prefs->set("PrintRange", false);
	prefs->set("PageNr", "");
	prefs->set("Copies", 1);
	prefs->set("Separations", static_cast<int>(!buttonNormal->isChecked()));
	prefs->set("PrintColor", static_cast<int>(!printColor->isChecked()));
	prefs->set("SepArt", tr("All"));
	prefs->set("MirrorH", mirrorH->isChecked());
	prefs->set("MirrorV", mirrorV->isChecked());
	prefs->set("DoGCR", doGCR->isChecked());
	prefs->set("Clip", setClip->isChecked());
	prefs->set("PSLevel", psLevel->currentIndex() + 1);
	prefs->set("doDev", setMedia->isChecked());
	prefs->set("doSpot", !convertSpots->isChecked());
	prefs->set("ICCinUse", useICC->isChecked());
	prefs->set("BleedTop", bleedTop->value() / unitRatio);
	prefs->set("BleedBottom", bleedBottom->value() / unitRatio);
	prefs->set("BleedRight", bleedRight->value() / unitRatio);
	prefs->set("BleedLeft", bleedLeft->value() / unitRatio);
	prefs->set("markOffset", offsetValue->value() / unitRatio);
	prefs->set("cropMarks", cropMarks->isChecked());
	prefs->set("bleedMarks", bleedMarks->isChecked());
	prefs->set("registrationMarks", registrationMarks->isChecked());
	prefs->set("colorMarks", colorMarks->isChecked());
	prefs->set("includePDFMarks", usePDFMarks->isChecked());
}

void TabPrinter::selOtherComm()
{
	if (useAltPrintCommand->isChecked())
	{
		defaultPrinter->setEnabled(false);
		printerCommand->setEnabled(true);
	}
	else
	{
		defaultPrinter->setEnabled(true);
		printerCommand->setEnabled(false);
	}
}

void TabPrinter::unitChange(QString unit, int docUnitIx, double invUnitConversion)
{
	bleedTop->setNewUnit(docUnitIx);
	bleedBottom->setNewUnit(docUnitIx);
	bleedLeft->setNewUnit(docUnitIx);
	bleedRight->setNewUnit(docUnitIx);
	offsetValue->setNewUnit(docUnitIx);
}
