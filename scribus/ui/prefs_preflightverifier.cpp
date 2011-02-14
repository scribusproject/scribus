/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_preflightverifier.h"
#include "prefsstructs.h"
#include "util.h"

Prefs_PreflightVerifier::Prefs_PreflightVerifier(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	connect(currentProfileComboBox, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(currentProfileComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));

	connect(ignoreAllErrorsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(autoCheckBeforePrintExportCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkMissingGlyphsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkItemsNotOnAPageCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkTextOverflowCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkTransparenciesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkMissingImagesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkPartFilledImageFramesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkImageResolutionCheckBox, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	connect(checkPDFAnnotFieldsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkPlacedPDFCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkGIFsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(minimumResolutionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	connect(maximumResolutionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	connect(checkNonPrintableLayersCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkVisiblePrintableLayersCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(removeProfilePushButton, SIGNAL(clicked()), this, SLOT(delProf()));
	connect(addProfilePushButton, SIGNAL(clicked()), this, SLOT(addProf()));
}

Prefs_PreflightVerifier::~Prefs_PreflightVerifier()
{
}

void Prefs_PreflightVerifier::languageChange()
{
}

void Prefs_PreflightVerifier::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	showPagesWithoutErrorsCheckBox->setChecked(prefsData->verifierPrefs.showPagesWithoutErrors);
	showNonPrintingLayerErrorsCheckBox->setChecked(prefsData->verifierPrefs.showNonPrintingLayerErrors);
	checkerProfile = prefsData->verifierPrefs.checkerPrefsList;
	currentProfileComboBox->clear();
	CheckerPrefsList::Iterator it;
	currentProfileComboBox->clear();
	for (it = checkerProfile.begin(); it != checkerProfile.end(); ++it)
		currentProfileComboBox->addItem(it.key());
	QString prefProfile(prefsData->verifierPrefs.curCheckProfile);
	setCurrentComboItem(currentProfileComboBox, prefProfile);
	ignoreAllErrorsCheckBox->setChecked(checkerProfile[prefProfile].ignoreErrors);
	autoCheckBeforePrintExportCheckBox->setChecked(checkerProfile[prefProfile].autoCheck);
	checkMissingGlyphsCheckBox->setChecked(checkerProfile[prefProfile].checkGlyphs);
	checkItemsNotOnAPageCheckBox->setChecked(checkerProfile[prefProfile].checkOrphans);
	checkTextOverflowCheckBox->setChecked(checkerProfile[prefProfile].checkOverflow);
	checkTransparenciesCheckBox->setChecked(checkerProfile[prefProfile].checkTransparency);
	checkMissingImagesCheckBox->setChecked(checkerProfile[prefProfile].checkPictures);
	checkPartFilledImageFramesCheckBox->setChecked(checkerProfile[prefProfile].checkPartFilledImageFrames);
	checkImageResolutionCheckBox->setChecked(checkerProfile[prefProfile].checkResolution);
	checkPDFAnnotFieldsCheckBox->setChecked(checkerProfile[prefProfile].checkAnnotations);
	checkPlacedPDFCheckBox->setChecked(checkerProfile[prefProfile].checkRasterPDF);
	checkGIFsCheckBox->setChecked(checkerProfile[prefProfile].checkForGIF);
	checkNonPrintableLayersCheckBox->setChecked(checkerProfile[prefProfile].ignoreOffLayers);
	checkVisiblePrintableLayersCheckBox->setChecked(checkerProfile[prefProfile].checkOffConflictLayers);
	minimumResolutionSpinBox->setValue( qRound(checkerProfile[prefProfile].minResolution) );
	maximumResolutionSpinBox->setValue( qRound(checkerProfile[prefProfile].maxResolution) );
	currentProfile = prefProfile;
	if (checkerProfile.count() == 1)
		removeProfilePushButton->setEnabled(false);
	addProfilePushButton->setEnabled(false);
	tempNewProfileName="";
}

void Prefs_PreflightVerifier::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->verifierPrefs.showPagesWithoutErrors=showPagesWithoutErrorsCheckBox->isChecked();
	prefsData->verifierPrefs.showNonPrintingLayerErrors=showNonPrintingLayerErrorsCheckBox->isChecked();
	prefsData->verifierPrefs.checkerPrefsList = checkerProfile;
	prefsData->verifierPrefs.curCheckProfile = currentProfileComboBox->currentText();

}

void Prefs_PreflightVerifier::putProfile()
{
	if (checkerProfile.contains(currentProfile))
	{
		checkerProfile[currentProfile].ignoreErrors = ignoreAllErrorsCheckBox->isChecked();
		checkerProfile[currentProfile].autoCheck = autoCheckBeforePrintExportCheckBox->isChecked();
		checkerProfile[currentProfile].checkGlyphs = checkMissingGlyphsCheckBox->isChecked();
		checkerProfile[currentProfile].checkOrphans = checkItemsNotOnAPageCheckBox->isChecked();
		checkerProfile[currentProfile].checkOverflow = checkTextOverflowCheckBox->isChecked();
		checkerProfile[currentProfile].checkPictures = checkMissingImagesCheckBox->isChecked();
		checkerProfile[currentProfile].checkPartFilledImageFrames = checkPartFilledImageFramesCheckBox->isChecked();
		checkerProfile[currentProfile].checkResolution = checkImageResolutionCheckBox->isChecked();
		checkerProfile[currentProfile].checkTransparency = checkTransparenciesCheckBox->isChecked();
		checkerProfile[currentProfile].minResolution = minimumResolutionSpinBox->value();
		checkerProfile[currentProfile].maxResolution = maximumResolutionSpinBox->value();
		checkerProfile[currentProfile].checkAnnotations = checkPDFAnnotFieldsCheckBox->isChecked();
		checkerProfile[currentProfile].checkRasterPDF = checkPlacedPDFCheckBox->isChecked();
		checkerProfile[currentProfile].checkForGIF = checkGIFsCheckBox->isChecked();
		checkerProfile[currentProfile].ignoreOffLayers = checkNonPrintableLayersCheckBox->isChecked();
		checkerProfile[currentProfile].checkOffConflictLayers = checkVisiblePrintableLayersCheckBox->isChecked();
	}
}

void Prefs_PreflightVerifier::setProfile(const QString& name)
{
	if (checkerProfile.contains(name))
	{
		putProfile();
		updateProfile(name);
		tempNewProfileName="";
		addProfilePushButton->setEnabled(false);
	}
	else
	{
		tempNewProfileName=name;
		addProfilePushButton->setEnabled(true);
	}
}

void Prefs_PreflightVerifier::updateProfile(const QString& name)
{
	disconnect(ignoreAllErrorsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(autoCheckBeforePrintExportCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkMissingGlyphsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkItemsNotOnAPageCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkMissingImagesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkPartFilledImageFramesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkImageResolutionCheckBox, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	disconnect(checkTransparenciesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(minimumResolutionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	disconnect(maximumResolutionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	disconnect(checkPlacedPDFCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkGIFsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkPDFAnnotFieldsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkNonPrintableLayersCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkVisiblePrintableLayersCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	ignoreAllErrorsCheckBox->setChecked(checkerProfile[name].ignoreErrors);
	autoCheckBeforePrintExportCheckBox->setChecked(checkerProfile[name].autoCheck);
	checkMissingGlyphsCheckBox->setChecked(checkerProfile[name].checkGlyphs);
	checkItemsNotOnAPageCheckBox->setChecked(checkerProfile[name].checkOrphans);
	checkTextOverflowCheckBox->setChecked(checkerProfile[name].checkOverflow);
	checkTransparenciesCheckBox->setChecked(checkerProfile[name].checkTransparency);
	checkMissingImagesCheckBox->setChecked(checkerProfile[name].checkPictures);
	checkPartFilledImageFramesCheckBox->setChecked(checkerProfile[name].checkPartFilledImageFrames);
	checkImageResolutionCheckBox->setChecked(checkerProfile[name].checkResolution);
	minimumResolutionSpinBox->setValue( qRound(checkerProfile[name].minResolution) );
	maximumResolutionSpinBox->setValue( qRound(checkerProfile[name].maxResolution) );
	checkPDFAnnotFieldsCheckBox->setChecked(checkerProfile[name].checkAnnotations);
	checkPlacedPDFCheckBox->setChecked(checkerProfile[name].checkRasterPDF);
	checkGIFsCheckBox->setChecked(checkerProfile[name].checkForGIF);
	checkNonPrintableLayersCheckBox->setChecked(checkerProfile[name].ignoreOffLayers);
	checkVisiblePrintableLayersCheckBox->setChecked(checkerProfile[name].checkOffConflictLayers);
	currentProfile = name;
	connect(ignoreAllErrorsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(autoCheckBeforePrintExportCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkMissingGlyphsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkItemsNotOnAPageCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkMissingImagesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkPartFilledImageFramesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkImageResolutionCheckBox, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	connect(checkTransparenciesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(minimumResolutionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	connect(maximumResolutionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	connect(checkPlacedPDFCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkGIFsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkPDFAnnotFieldsCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkNonPrintableLayersCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkVisiblePrintableLayersCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
}

void Prefs_PreflightVerifier::addProf()
{
	struct CheckerPrefs checkerSettings;
	checkerSettings.ignoreErrors = ignoreAllErrorsCheckBox->isChecked();
	checkerSettings.autoCheck = autoCheckBeforePrintExportCheckBox->isChecked();
	checkerSettings.checkGlyphs = checkMissingGlyphsCheckBox->isChecked();
	checkerSettings.checkOrphans = checkItemsNotOnAPageCheckBox->isChecked();
	checkerSettings.checkOverflow = checkTextOverflowCheckBox->isChecked();
	checkerSettings.checkPictures = checkMissingImagesCheckBox->isChecked();
	checkerSettings.checkPartFilledImageFrames = checkPartFilledImageFramesCheckBox->isChecked();
	checkerSettings.checkResolution = checkImageResolutionCheckBox->isChecked();
	checkerSettings.checkTransparency =  checkTransparenciesCheckBox->isChecked();
	checkerSettings.minResolution = minimumResolutionSpinBox->value();
	checkerSettings.maxResolution = maximumResolutionSpinBox->value();
	checkerSettings.checkAnnotations = checkPDFAnnotFieldsCheckBox->isChecked();
	checkerSettings.checkRasterPDF = checkPlacedPDFCheckBox->isChecked();
	checkerSettings.checkForGIF = checkGIFsCheckBox->isChecked();
	checkerSettings.ignoreOffLayers = checkNonPrintableLayersCheckBox->isChecked();
	checkerSettings.checkOffConflictLayers = checkVisiblePrintableLayersCheckBox->isChecked();
	checkerProfile.insert(tempNewProfileName, checkerSettings);
	currentProfile = tempNewProfileName;
	if (checkerProfile.count() > 1)
		removeProfilePushButton->setEnabled(true);
	addProfilePushButton->setEnabled(false);
	currentProfileComboBox->clear();

	disconnect(currentProfileComboBox, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	disconnect(currentProfileComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	CheckerPrefsList::Iterator it;
	int j,i=0;
	for (it = checkerProfile.begin(), j=0; it != checkerProfile.end(); ++it, ++j)
	{
		currentProfileComboBox->addItem(it.key());
		if (it.key()==currentProfile)
			i=j;
	}
	currentProfileComboBox->setCurrentIndex(i);
	connect(currentProfileComboBox, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(currentProfileComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	tempNewProfileName="";
}

void Prefs_PreflightVerifier::delProf()
{
	disconnect(currentProfileComboBox, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	disconnect(currentProfileComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	checkerProfile.remove(currentProfile);
	updateProfile(checkerProfile.begin().key());
	currentProfileComboBox->clear();
	CheckerPrefsList::Iterator it;
	CheckerPrefsList::Iterator itend=checkerProfile.end();
	for (it = checkerProfile.begin(); it != itend; ++it)
		currentProfileComboBox->addItem(it.key());
	setCurrentComboItem(currentProfileComboBox, currentProfile);
	connect(currentProfileComboBox, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(currentProfileComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	if (checkerProfile.count() == 1)
		removeProfilePushButton->setEnabled(false);
}
