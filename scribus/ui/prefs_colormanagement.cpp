/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_colormanagement.h"
#include "prefsstructs.h"
#include "scribusstructs.h"
#include "scribuscore.h"
#include "scribusdoc.h"

Prefs_ColorManagement::Prefs_ColorManagement(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent),
	m_Doc(doc)
{
	m_canChangeMonitorProfile = !ScCore->primaryMainWindow()->HaveDoc; 
	setupUi(this);
	languageChange();
	connect(activateCMCheckBox, SIGNAL(clicked(bool)), this, SLOT(cmActivated(bool)));
	connect(simulatePrinterOnScreenCheckBox, SIGNAL(clicked(bool)), this, SLOT(simulatePrinter(bool)));
	if (!m_canChangeMonitorProfile)
	{
		if (!m_Doc)
		{
			monitorProfileComboBox->setVisible(false);
			monitorLabel->setText( tr("Monitor profiles can only be changed when no documents are open.") );
		}
		else
		{
			monitorProfileLabel->setVisible(false);
			monitorProfileLine->setVisible(false);
			monitorProfileComboBox->setVisible(false);
			monitorLabel->setVisible(false);
		}
	}
}

Prefs_ColorManagement::~Prefs_ColorManagement()
{
}

void Prefs_ColorManagement::languageChange()
{
}

void Prefs_ColorManagement::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	if (!ScCore->haveCMS())
	{
		activateCMCheckBox->setEnabled(false);
		warningLabel->setText("<qt><font color=\"red\">"+tr("<qt>Color Management cannot be activated due to missing color profiles.</br>You need to install at least one RGB and one CMYK profile.")+"</font></qt>");
	}
	else
	{
		warningLabel->resize(0,0);
		warningLabel->hide();
	}
	activateCMCheckBox->setChecked(prefsData->colorPrefs.DCMSset.CMSinUse);

	QString tmp_mp[] = { tr("Perceptual"), tr("Relative Colorimetric"),
		tr("Saturation"), tr("Absolute Colorimetric")};
		size_t array = sizeof(tmp_mp) / sizeof(*tmp_mp);
	imageRenderingIntentComboBox->clear();
	for (uint prop = 0; prop < array; ++prop)
		imageRenderingIntentComboBox->addItem(tmp_mp[prop]);
	imageRenderingIntentComboBox->setCurrentIndex(prefsData->colorPrefs.DCMSset.DefaultIntentImages);
	solidColorsRenderingIntentComboBox->clear();
	for (uint prop = 0; prop < array; ++prop)
		solidColorsRenderingIntentComboBox->addItem(tmp_mp[prop]);
	solidColorsRenderingIntentComboBox->setCurrentIndex(prefsData->colorPrefs.DCMSset.DefaultIntentColors);

	simulatePrinterOnScreenCheckBox->setChecked(prefsData->colorPrefs.DCMSset.SoftProofOn);
	convertAllColorsToPrinterSpaceCheckBox->setChecked(prefsData->colorPrefs.DCMSset.SoftProofFullOn);
	markColorsOutOfGamutCheckBox->setChecked(prefsData->colorPrefs.DCMSset.GamutCheck);
	useBlackpointCompensationCheckBox->setChecked(prefsData->colorPrefs.DCMSset.BlackPoint);

	cmActivated(prefsData->colorPrefs.DCMSset.CMSinUse);
	simulatePrinter(prefsData->colorPrefs.DCMSset.SoftProofOn);
}

void Prefs_ColorManagement::setProfiles(struct ApplicationPrefs *prefsData, ProfilesL *inputProfiles, ProfilesL *inputProfilesCMYK, ProfilesL *printerProfiles, ProfilesL *monitorProfiles)
{
	ProfilesL::Iterator it;
	rgbImageProfileComboBox->clear();
	for (it = inputProfiles->begin(); it != inputProfiles->end(); ++it)
	{
		rgbImageProfileComboBox->addItem(it.key());
		if (it.key() == prefsData->colorPrefs.DCMSset.DefaultImageRGBProfile)
			rgbImageProfileComboBox->setCurrentIndex(rgbImageProfileComboBox->count()-1);
	}
	cmykImageProfileComboBox->clear();
	for (it = inputProfilesCMYK->begin(); it != inputProfilesCMYK->end(); ++it)
	{
		cmykImageProfileComboBox->addItem(it.key());
		if (it.key() == prefsData->colorPrefs.DCMSset.DefaultImageCMYKProfile)
			cmykImageProfileComboBox->setCurrentIndex(cmykImageProfileComboBox->count()-1);
	}
	rgbSolidProfileComboBox->clear();
	for (it = inputProfiles->begin(); it != inputProfiles->end(); ++it)
	{
		rgbSolidProfileComboBox->addItem(it.key());
		if (it.key() == prefsData->colorPrefs.DCMSset.DefaultSolidColorRGBProfile)
			rgbSolidProfileComboBox->setCurrentIndex(rgbSolidProfileComboBox->count()-1);
	}
	cmykSolidProfileComboBox->clear();
	for (it = inputProfilesCMYK->begin(); it != inputProfilesCMYK->end(); ++it)
	{
		cmykSolidProfileComboBox->addItem(it.key());
		if (it.key() == prefsData->colorPrefs.DCMSset.DefaultSolidColorCMYKProfile)
			cmykSolidProfileComboBox->setCurrentIndex(cmykSolidProfileComboBox->count()-1);
	}
	monitorProfileComboBox->clear();
	if (m_canChangeMonitorProfile && !m_Doc)
	{
		for (it = monitorProfiles->begin(); it != monitorProfiles->end(); ++it)
		{
			monitorProfileComboBox->addItem(it.key());
			if (it.key() == prefsData->colorPrefs.DCMSset.DefaultMonitorProfile)
				monitorProfileComboBox->setCurrentIndex(monitorProfileComboBox->count()-1);
		}
	}
	printerProfileComboBox->clear();
	for (it = printerProfiles->begin(); it != printerProfiles->end(); ++it)
	{
		printerProfileComboBox->addItem(it.key());
		if (it.key() == prefsData->colorPrefs.DCMSset.DefaultPrinterProfile)
			printerProfileComboBox->setCurrentIndex(printerProfileComboBox->count()-1);
	}
}

void Prefs_ColorManagement::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->colorPrefs.DCMSset.CMSinUse=activateCMCheckBox->isChecked();
	prefsData->colorPrefs.DCMSset.DefaultIntentImages=(eRenderIntent) imageRenderingIntentComboBox->currentIndex();
	prefsData->colorPrefs.DCMSset.DefaultIntentColors=(eRenderIntent) solidColorsRenderingIntentComboBox->currentIndex();
	prefsData->colorPrefs.DCMSset.SoftProofOn=simulatePrinterOnScreenCheckBox->isChecked();
	prefsData->colorPrefs.DCMSset.SoftProofFullOn=convertAllColorsToPrinterSpaceCheckBox->isChecked();
	prefsData->colorPrefs.DCMSset.GamutCheck=markColorsOutOfGamutCheckBox->isChecked();
	prefsData->colorPrefs.DCMSset.BlackPoint=useBlackpointCompensationCheckBox->isChecked();

	prefsData->colorPrefs.DCMSset.DefaultImageRGBProfile = rgbImageProfileComboBox->currentText();
	prefsData->colorPrefs.DCMSset.DefaultImageCMYKProfile = cmykImageProfileComboBox->currentText();
	prefsData->colorPrefs.DCMSset.DefaultSolidColorRGBProfile = rgbSolidProfileComboBox->currentText();
	prefsData->colorPrefs.DCMSset.DefaultSolidColorCMYKProfile = cmykSolidProfileComboBox->currentText();
	prefsData->colorPrefs.DCMSset.DefaultPrinterProfile = printerProfileComboBox->currentText();

	if (m_canChangeMonitorProfile && !m_Doc)
		prefsData->colorPrefs.DCMSset.DefaultMonitorProfile = monitorProfileComboBox->currentText();
}

bool Prefs_ColorManagement::cmActive()
{
	return activateCMCheckBox->isChecked();
}

void Prefs_ColorManagement::cmActivated(bool active)
{
	imageRenderingIntentComboBox->setEnabled( active );
	solidColorsRenderingIntentComboBox->setEnabled( active );
	simulatePrinterOnScreenCheckBox->setEnabled( active );
	convertAllColorsToPrinterSpaceCheckBox->setEnabled( active );
	markColorsOutOfGamutCheckBox->setEnabled( active );
	useBlackpointCompensationCheckBox->setEnabled( active );
	rgbImageProfileComboBox->setEnabled( active );
	cmykImageProfileComboBox->setEnabled( active );
	rgbSolidProfileComboBox->setEnabled( active );
	cmykSolidProfileComboBox->setEnabled( active );
	if (!m_Doc)
		monitorProfileComboBox->setEnabled( m_canChangeMonitorProfile );
	printerProfileComboBox->setEnabled( active );
}

void Prefs_ColorManagement::simulatePrinter(bool active)
{
	convertAllColorsToPrinterSpaceCheckBox->setEnabled( active );
	markColorsOutOfGamutCheckBox->setEnabled( active );
}

