/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_printer.h"
#include "ui/newmarginwidget.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefsstructs.h"
#include "util_printer.h"
#include "units.h"

Prefs_Printer::Prefs_Printer(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
	connect(useAltPrinterCmdCheckBox, SIGNAL(clicked()), this, SLOT(selOtherComm()));
}

Prefs_Printer::~Prefs_Printer()
{
}

void Prefs_Printer::languageChange()
{
	clipToPrinterMarginsCheckBox->setToolTip( "<qt>" + tr( "Do not show objects outside the margins on the printed page" ) + "</qt>" );
	altPrinterCmdLineEdit->setToolTip("<qt>" + tr( "Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options") + "</qt>" );
	postscriptLevelComboBox->setToolTip("<qt>" +  tr( "Sets the PostScript Level.\n Setting to Level 1 or 2 can create huge files" ) + "</qt>" );
	applyUnderColorRemovalCheckBox->setToolTip( "<qt>" + tr( "A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks." ) + "</qt>");
	convertSpotsToProcessCheckBox->setToolTip("<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");
	applyICCProfilesCheckBox->setToolTip("<qt>" + tr( "Allows you to embed color profiles in the print stream when color management is enabled" ) + "</qt>");
	setMediaSizeCheckBox->setToolTip( "<qt>" + tr( "This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer." ) + "</qt>");
}

void Prefs_Printer::unitChange(int newIndex)
{
	bleedsWidget->setNewUnitIndex(newIndex);
	markOffsetSpinBox->setNewUnit(newIndex);
}

void Prefs_Printer::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	int docUnitIndex = prefsData->docSetupPrefs.docUnitIndex;
	unitChange(docUnitIndex);
	double unitRatio = unitGetRatioFromIndex(docUnitIndex);
	QString unitSuffix = unitGetSuffixFromIndex(docUnitIndex);

	QString Pcap;
	QString printerName;
	QStringList printerNames = PrinterUtil::getPrinterNames();
	int numPrinters = printerNames.count();
	for( int i = 0; i < numPrinters; i++)
	{
		printerName = printerNames[i];
		destinationComboBox->addItem(printerName);
	}

	destinationComboBox->addItem( tr("File"));

	PrefsContext* prefs = PrefsManager::instance()->prefsFile->getContext("print_options");

	QString selectedDest = prefs->get("CurrentPrn", "");
	int prnIndex = destinationComboBox->findText(selectedDest);
	if ((prnIndex > -1) && (prnIndex < destinationComboBox->count()))
		destinationComboBox->setCurrentIndex(prnIndex);
	useAltPrinterCmdCheckBox->setChecked(prefs->getBool("OtherCom", false));
	selOtherComm();
	altPrinterCmdLineEdit->setText(prefs->get("Command", ""));
	bool iccInUse = prefs->getBool("ICCinUse", false);
	bool psPrinter = PrinterUtil::isPostscriptPrinter(destinationComboBox->currentText());
	applyICCProfilesCheckBox->setChecked( psPrinter ? iccInUse : false );
	applyICCProfilesCheckBox->setEnabled( psPrinter );
	outputComboBox->setCurrentIndex(prefs->getInt("Separations", 0));
	postscriptPrintToColorComboBox->setCurrentIndex(prefs->getInt("PrintColor", 0));
	postscriptLevelComboBox->setCurrentIndex(prefs->getInt("PSLevel", 3)-1);
	pageMirrorHorizontallyCheckBox->setChecked(prefs->getBool("MirrorH", false));
	pageMirrorVerticallyCheckBox->setChecked(prefs->getBool("MirrorV", false));
	setMediaSizeCheckBox->setChecked(prefs->getBool("doDev", false));
	applyUnderColorRemovalCheckBox->setChecked(prefs->getBool("DoGCR", false));
	clipToPrinterMarginsCheckBox->setChecked(prefs->getBool("Clip", false));
	convertSpotsToProcessCheckBox->setChecked(!prefs->getBool("doSpot", true));
	MarginStruct bleeds;
	bleeds.set(prefs->getDouble("BleedTop",0.0)*unitRatio,
			   prefs->getDouble("BleedBottom",0.0)*unitRatio,
			   prefs->getDouble("BleedRight",0.0)*unitRatio,
			   prefs->getDouble("BleedLeft",0.0)*unitRatio);
	bleedsWidget->setup(bleeds, 0, 0, false, false);
	markOffsetSpinBox->setValue(prefs->getDouble("markOffset",0.0)*unitRatio);
	bleedMarksCheckBox->setChecked(prefs->getBool("cropMarks", false));
	bleedMarksCheckBox->setChecked(prefs->getBool("bleedMarks", false));
	registrationMarksCheckBox->setChecked(prefs->getBool("registrationMarks", false));
	colorBarsCheckBox->setChecked(prefs->getBool("colorMarks", false));
	includePDFAnnotationsCheckBox->setChecked(prefs->getBool("includePDFMarks", true));
}

void Prefs_Printer::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	PrefsContext* prefs = PrefsManager::instance()->prefsFile->getContext("print_options");
	prefs->set("CurrentPrn", destinationComboBox->currentText());
	prefs->set("OtherCom", useAltPrinterCmdCheckBox->isChecked());
	prefs->set("Command", altPrinterCmdLineEdit->text());
	prefs->set("PrintAll", true);
	prefs->set("CurrentPage", false);
	prefs->set("PrintRange", false);
	prefs->set("PageNr", "");
	prefs->set("Copies", 1);
	prefs->set("Separations", static_cast<int>(outputComboBox->currentIndex()==1));
	prefs->set("PrintColor", static_cast<int>(!postscriptPrintToColorComboBox->currentIndex()==0));
	prefs->set("SepArt", tr("All"));
	prefs->set("MirrorH", pageMirrorHorizontallyCheckBox->isChecked());
	prefs->set("MirrorV", pageMirrorVerticallyCheckBox->isChecked());
	prefs->set("DoGCR", applyUnderColorRemovalCheckBox->isChecked());
	prefs->set("Clip", clipToPrinterMarginsCheckBox->isChecked());
	prefs->set("PSLevel", postscriptLevelComboBox->currentIndex() + 1);
	prefs->set("doDev", setMediaSizeCheckBox->isChecked());
	prefs->set("doSpot", !convertSpotsToProcessCheckBox->isChecked());
	prefs->set("ICCinUse", applyICCProfilesCheckBox->isChecked());
	double unitRatio = unitGetRatioFromIndex(prefsData->docSetupPrefs.docUnitIndex);
	MarginStruct bleeds(bleedsWidget->margins());
		prefs->set("BleedTop", bleeds.Left / unitRatio);
	prefs->set("BleedBottom", bleeds.Bottom / unitRatio);
	prefs->set("BleedRight", bleeds.Right / unitRatio);
	prefs->set("BleedLeft", bleeds.Left / unitRatio);
	prefs->set("markOffset", markOffsetSpinBox->value() / unitRatio);
	prefs->set("cropMarks", cropMarksCheckBox->isChecked());
	prefs->set("bleedMarks", bleedMarksCheckBox->isChecked());
	prefs->set("registrationMarks", registrationMarksCheckBox->isChecked());
	prefs->set("colorMarks", colorBarsCheckBox->isChecked());
	prefs->set("includePDFMarks", includePDFAnnotationsCheckBox->isChecked());
}

void Prefs_Printer::selOtherComm()
{
	bool setter=useAltPrinterCmdCheckBox->isChecked();
	destinationComboBox->setEnabled(!setter);
	altPrinterCmdLineEdit->setEnabled(setter);
}

