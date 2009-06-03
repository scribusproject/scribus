/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmsprefs.h"
#include "scribusdoc.h"
#include "scribusstructs.h"

CMSPrefs::CMSPrefs( QWidget* parent, CMSData *Vor, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles)
	: QWidget( parent )
{
	prefs = Vor;
	changed = false;
	setupUi(this);
	restoreDefaults(prefs, InputProfiles, InputProfilesCMYK, PrinterProfiles, MonitorProfiles);
	connect( checkBox1, SIGNAL( clicked() ), this, SLOT( slotCMSon() ) );
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

	simulate->setChecked(prefs->SoftProofOn);
	convertAll->setChecked(prefs->SoftProofFullOn);
	gamutC->setChecked(prefs->GamutCheck);
	blackP->setChecked(prefs->BlackPoint);

	simulate->setEnabled( checkBox1->isChecked() );
	blackP->setEnabled( checkBox1->isChecked() );
	sysProfiles->setEnabled( checkBox1->isChecked() );
	renderInt->setEnabled( checkBox1->isChecked() );
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
		(prefs->SoftProofOn != simulate->isChecked()) ||
		(prefs->SoftProofFullOn != convertAll->isChecked()) ||
		(prefs->GamutCheck != gamutC->isChecked()) ||
		(prefs->BlackPoint != blackP->isChecked()) ||
		(prefs->CMSinUse != checkBox1->isChecked()))
			changed = true;
	prefs->DefaultImageRGBProfile = inputPRGBIm->currentText();
	prefs->DefaultImageCMYKProfile = inputPCMYKIm->currentText();
	prefs->DefaultSolidColorRGBProfile = inputPRGB->currentText();
	prefs->DefaultSolidColorCMYKProfile = inputPCMYK->currentText();
	prefs->DefaultMonitorProfile = monitorP->currentText();
	prefs->DefaultPrinterProfile = printerP->currentText();
	prefs->DefaultIntentColors = colorsI->currentIndex();
	prefs->DefaultIntentImages = imagesI->currentIndex();
	prefs->SoftProofOn = simulate->isChecked();
	prefs->SoftProofFullOn = convertAll->isChecked();
	prefs->GamutCheck = gamutC->isChecked();
	prefs->CMSinUse = checkBox1->isChecked();
	prefs->BlackPoint = blackP->isChecked();
}

void CMSPrefs::updateDocSettings(ScribusDoc* doc)
{
	if ((doc->CMSSettings.DefaultImageRGBProfile != inputPRGBIm->currentText()) ||
		(doc->CMSSettings.DefaultImageCMYKProfile != inputPCMYKIm->currentText()) ||
		(doc->CMSSettings.DefaultSolidColorRGBProfile != inputPRGB->currentText()) ||
		(doc->CMSSettings.DefaultSolidColorCMYKProfile != inputPCMYK->currentText()) ||
		(doc->CMSSettings.DefaultMonitorProfile != monitorP->currentText()) ||
		(doc->CMSSettings.DefaultPrinterProfile != printerP->currentText()) ||
		(doc->CMSSettings.DefaultIntentColors != colorsI->currentIndex()) ||
		(doc->CMSSettings.DefaultIntentImages != imagesI->currentIndex()) ||
		(doc->CMSSettings.SoftProofOn != simulate->isChecked()) ||
		(doc->CMSSettings.SoftProofFullOn != convertAll->isChecked()) ||
		(doc->CMSSettings.GamutCheck != gamutC->isChecked()) ||
		(doc->CMSSettings.BlackPoint != blackP->isChecked()) ||
		(doc->CMSSettings.CMSinUse != checkBox1->isChecked()))
			changed = true;
	doc->CMSSettings.DefaultImageRGBProfile = inputPRGBIm->currentText();
	doc->CMSSettings.DefaultImageCMYKProfile = inputPCMYKIm->currentText();
	doc->CMSSettings.DefaultSolidColorRGBProfile = inputPRGB->currentText();
	doc->CMSSettings.DefaultSolidColorCMYKProfile = inputPCMYK->currentText();
	doc->CMSSettings.DefaultMonitorProfile = monitorP->currentText();
	doc->CMSSettings.DefaultPrinterProfile = printerP->currentText();
	doc->CMSSettings.DefaultIntentColors = colorsI->currentIndex();
	doc->CMSSettings.DefaultIntentImages = imagesI->currentIndex();
	doc->CMSSettings.SoftProofOn = simulate->isChecked();
	doc->CMSSettings.SoftProofFullOn = convertAll->isChecked();
	doc->CMSSettings.GamutCheck = gamutC->isChecked();
	doc->CMSSettings.CMSinUse = checkBox1->isChecked();
	doc->CMSSettings.BlackPoint = blackP->isChecked();
}

void CMSPrefs::slotCMSon()
{
	emit cmsOn(checkBox1->isChecked());
}

