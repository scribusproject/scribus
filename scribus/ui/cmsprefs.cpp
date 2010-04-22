/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmsprefs.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusstructs.h"

CMSPrefs::CMSPrefs( QWidget* parent, CMSData *Vor, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles)
	: QWidget( parent )
{
	m_canChangeMonitorProfile = !ScCore->primaryMainWindow()->HaveDoc;
	prefs = Vor;
	changed = false;
	setupUi(this);
	restoreDefaults(prefs, InputProfiles, InputProfilesCMYK, PrinterProfiles, MonitorProfiles);
	connect( checkBox1, SIGNAL( clicked(bool) ), this, SLOT( slotCMSon(bool) ) );

	monProfile->setEnabled(m_canChangeMonitorProfile);
	if (!m_canChangeMonitorProfile)
	{
		monitorP->setVisible(false);
		monitorProfileLabel->setText( tr("Monitor profiles can only be changed when no documents are currently opened.") );
	}
}

void CMSPrefs::restoreDefaults(CMSData *prefs, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles)
{
	checkBox1->setChecked(prefs->CMSinUse);
	
	ProfilesL::Iterator it;
	inputPRGBIm->clear();
	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		inputPRGBIm->addItem(it.key());
		if (it.key() == prefs->DefaultImageRGBProfile)
			inputPRGBIm->setCurrentIndex(inputPRGBIm->count()-1);
	}
	inputPCMYKIm->clear();
	for (it = InputProfilesCMYK->begin(); it != InputProfilesCMYK->end(); ++it)
	{
		inputPCMYKIm->addItem(it.key());
		if (it.key() == prefs->DefaultImageCMYKProfile)
			inputPCMYKIm->setCurrentIndex(inputPCMYKIm->count()-1);
	}
	inputPRGB->clear();
	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		inputPRGB->addItem(it.key());
		if (it.key() == prefs->DefaultSolidColorRGBProfile)
			inputPRGB->setCurrentIndex(inputPRGB->count()-1);
	}
	inputPCMYK->clear();
	for (it = InputProfilesCMYK->begin(); it != InputProfilesCMYK->end(); ++it)
	{
		inputPCMYK->addItem(it.key());
		if (it.key() == prefs->DefaultSolidColorCMYKProfile)
			inputPCMYK->setCurrentIndex(inputPCMYK->count()-1);
	}
	monitorP->clear();
	for (it = MonitorProfiles->begin(); it != MonitorProfiles->end(); ++it)
	{
		monitorP->addItem(it.key());
		if (it.key() == prefs->DefaultMonitorProfile)
			monitorP->setCurrentIndex(monitorP->count()-1);
	}
	printerP->clear();
	for (it = PrinterProfiles->begin(); it != PrinterProfiles->end(); ++it)
	{
		printerP->addItem(it.key());
		if (it.key() == prefs->DefaultPrinterProfile)
			printerP->setCurrentIndex(printerP->count()-1);
	}

	QString tmp_mp[] = { tr("Perceptual"), tr("Relative Colorimetric"),
		tr("Saturation"), tr("Absolute Colorimetric")};
		size_t array = sizeof(tmp_mp) / sizeof(*tmp_mp);
	imagesI->clear();
	for (uint prop = 0; prop < array; ++prop)
		imagesI->addItem(tmp_mp[prop]);
	imagesI->setCurrentIndex(prefs->DefaultIntentImages);
	colorsI->clear();
	for (uint prop = 0; prop < array; ++prop)
		colorsI->addItem(tmp_mp[prop]);
	colorsI->setCurrentIndex(prefs->DefaultIntentColors);

	simulatePrinter->setChecked(prefs->SoftProofOn);
	convertAll->setChecked(prefs->SoftProofFullOn);
	gamutCheck->setChecked(prefs->GamutCheck);
	blackPoint->setChecked(prefs->BlackPoint);

	docOptions->setEnabled( checkBox1->isChecked() );
	docProfiles->setEnabled( checkBox1->isChecked() );
	renderInt->setEnabled( checkBox1->isChecked() );
	monProfile->setEnabled( m_canChangeMonitorProfile );
}

void CMSPrefs::setValues()
{
	if ((prefs->DefaultImageRGBProfile != inputPRGBIm->currentText()) ||
		(prefs->DefaultImageCMYKProfile != inputPCMYKIm->currentText()) ||
		(prefs->DefaultSolidColorRGBProfile != inputPRGB->currentText()) ||
		(prefs->DefaultSolidColorCMYKProfile != inputPCMYK->currentText()) ||
		(prefs->DefaultMonitorProfile != monitorP->currentText()) ||
		(prefs->DefaultPrinterProfile != printerP->currentText()) ||
		(prefs->DefaultIntentColors != colorsI->currentIndex()) ||
		(prefs->DefaultIntentImages != imagesI->currentIndex()) ||
		(prefs->SoftProofOn != simulatePrinter->isChecked()) ||
		(prefs->SoftProofFullOn != convertAll->isChecked()) ||
		(prefs->GamutCheck != gamutCheck->isChecked()) ||
		(prefs->BlackPoint != blackPoint->isChecked()) ||
		(prefs->CMSinUse != checkBox1->isChecked()))
			changed = true;
	prefs->DefaultImageRGBProfile = inputPRGBIm->currentText();
	prefs->DefaultImageCMYKProfile = inputPCMYKIm->currentText();
	prefs->DefaultSolidColorRGBProfile = inputPRGB->currentText();
	prefs->DefaultSolidColorCMYKProfile = inputPCMYK->currentText();
	prefs->DefaultPrinterProfile = printerP->currentText();
	prefs->DefaultIntentColors = (eRenderIntent) colorsI->currentIndex();
	prefs->DefaultIntentImages = (eRenderIntent) imagesI->currentIndex();
	prefs->SoftProofOn = simulatePrinter->isChecked();
	prefs->SoftProofFullOn = convertAll->isChecked();
	prefs->GamutCheck = gamutCheck->isChecked();
	prefs->CMSinUse = checkBox1->isChecked();
	prefs->BlackPoint = blackPoint->isChecked();
	if (m_canChangeMonitorProfile)
	{
		prefs->DefaultMonitorProfile = monitorP->currentText();
	}
}

void CMSPrefs::updateDocSettings(ScribusDoc* doc)
{
	if ((doc->cmsSettings().DefaultImageRGBProfile != inputPRGBIm->currentText()) ||
		(doc->cmsSettings().DefaultImageCMYKProfile != inputPCMYKIm->currentText()) ||
		(doc->cmsSettings().DefaultSolidColorRGBProfile != inputPRGB->currentText()) ||
		(doc->cmsSettings().DefaultSolidColorCMYKProfile != inputPCMYK->currentText()) ||
		(doc->cmsSettings().DefaultPrinterProfile != printerP->currentText()) ||
		(doc->cmsSettings().DefaultIntentColors != colorsI->currentIndex()) ||
		(doc->cmsSettings().DefaultIntentImages != imagesI->currentIndex()) ||
		(doc->cmsSettings().SoftProofOn != simulatePrinter->isChecked()) ||
		(doc->cmsSettings().SoftProofFullOn != convertAll->isChecked()) ||
		(doc->cmsSettings().GamutCheck != gamutCheck->isChecked()) ||
		(doc->cmsSettings().BlackPoint != blackPoint->isChecked()) ||
		(doc->cmsSettings().CMSinUse != checkBox1->isChecked()))
			changed = true;
	doc->cmsSettings().DefaultImageRGBProfile = inputPRGBIm->currentText();
	doc->cmsSettings().DefaultImageCMYKProfile = inputPCMYKIm->currentText();
	doc->cmsSettings().DefaultSolidColorRGBProfile = inputPRGB->currentText();
	doc->cmsSettings().DefaultSolidColorCMYKProfile = inputPCMYK->currentText();
	doc->cmsSettings().DefaultPrinterProfile = printerP->currentText();
	doc->cmsSettings().DefaultIntentColors = (eRenderIntent) colorsI->currentIndex();
	doc->cmsSettings().DefaultIntentImages = (eRenderIntent) imagesI->currentIndex();
	doc->cmsSettings().SoftProofOn = simulatePrinter->isChecked();
	doc->cmsSettings().SoftProofFullOn = convertAll->isChecked();
	doc->cmsSettings().GamutCheck = gamutCheck->isChecked();
	doc->cmsSettings().CMSinUse = checkBox1->isChecked();
	doc->cmsSettings().BlackPoint = blackPoint->isChecked();
}

void CMSPrefs::slotCMSon(bool active)
{
	docProfiles->setEnabled(active);
	monProfile->setEnabled(m_canChangeMonitorProfile);
	renderInt->setEnabled(active);
	docOptions->setEnabled(active);
	emit cmsOn(checkBox1->isChecked());
}

