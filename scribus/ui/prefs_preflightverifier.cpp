/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_preflightverifier.h"
#include "prefsstructs.h"
#include "util.h"

Prefs_PreflightVerifier::Prefs_PreflightVerifier(QWidget* parent, ScribusDoc* /*doc*/)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	m_caption = tr("Preflight Verifier");
	m_icon = "16/preflight-verifier.png";

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
	connect(checkAppliedMasterPageLocationCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkEmptyTextFramesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
}

Prefs_PreflightVerifier::~Prefs_PreflightVerifier() = default;

void Prefs_PreflightVerifier::languageChange()
{
	// No need to do anything here, the UI language cannot change while prefs dialog is opened
}

void Prefs_PreflightVerifier::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	showPagesWithoutErrorsCheckBox->setChecked(prefsData->verifierPrefs.showPagesWithoutErrors);
	showNonPrintingLayerErrorsCheckBox->setChecked(prefsData->verifierPrefs.showNonPrintingLayerErrors);
	checkerProfiles = prefsData->verifierPrefs.checkerPrefsList;
	currentProfileComboBox->clear();
	CheckerPrefsList::Iterator it;
	currentProfileComboBox->clear();
	for (it = checkerProfiles.begin(); it != checkerProfiles.end(); ++it)
		currentProfileComboBox->addItem(it.key());
	QString prefProfile(prefsData->verifierPrefs.curCheckProfile);
	setCurrentComboItem(currentProfileComboBox, prefProfile);

	CheckerPrefs& checkerProfile = checkerProfiles[prefProfile];
	ignoreAllErrorsCheckBox->setChecked(checkerProfile.ignoreErrors);
	autoCheckBeforePrintExportCheckBox->setChecked(checkerProfile.autoCheck);
	checkMissingGlyphsCheckBox->setChecked(checkerProfile.checkGlyphs);
	checkItemsNotOnAPageCheckBox->setChecked(checkerProfile.checkOrphans);
	checkTextOverflowCheckBox->setChecked(checkerProfile.checkOverflow);
	checkTransparenciesCheckBox->setChecked(checkerProfile.checkTransparency);
	checkMissingImagesCheckBox->setChecked(checkerProfile.checkPictures);
	checkPartFilledImageFramesCheckBox->setChecked(checkerProfile.checkPartFilledImageFrames);
	checkImageResolutionCheckBox->setChecked(checkerProfile.checkResolution);
	checkPDFAnnotFieldsCheckBox->setChecked(checkerProfile.checkAnnotations);
	checkPlacedPDFCheckBox->setChecked(checkerProfile.checkRasterPDF);
	checkGIFsCheckBox->setChecked(checkerProfile.checkForGIF);
	checkNonPrintableLayersCheckBox->setChecked(checkerProfile.ignoreOffLayers);
	checkVisiblePrintableLayersCheckBox->setChecked(checkerProfile.checkOffConflictLayers);
	checkAppliedMasterPageLocationCheckBox->setChecked(checkerProfile.checkAppliedMasterDifferentSide);
	checkEmptyTextFramesCheckBox->setChecked(checkerProfile.checkEmptyTextFrames);
	minimumResolutionSpinBox->setValue( qRound(checkerProfile.minResolution) );
	maximumResolutionSpinBox->setValue( qRound(checkerProfile.maxResolution) );

	currentProfile = prefProfile;
	if (checkerProfiles.count() == 1)
		removeProfilePushButton->setEnabled(false);
	addProfilePushButton->setEnabled(false);
	tempNewProfileName="";
}

void Prefs_PreflightVerifier::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->verifierPrefs.showPagesWithoutErrors = showPagesWithoutErrorsCheckBox->isChecked();
	prefsData->verifierPrefs.showNonPrintingLayerErrors = showNonPrintingLayerErrorsCheckBox->isChecked();
	prefsData->verifierPrefs.checkerPrefsList = checkerProfiles;
	prefsData->verifierPrefs.curCheckProfile = currentProfileComboBox->currentText();
}

void Prefs_PreflightVerifier::putProfile()
{
	if (!checkerProfiles.contains(currentProfile))
		return;

	CheckerPrefs& checkerProfile = checkerProfiles[currentProfile];
	checkerProfile.ignoreErrors = ignoreAllErrorsCheckBox->isChecked();
	checkerProfile.autoCheck = autoCheckBeforePrintExportCheckBox->isChecked();
	checkerProfile.checkGlyphs = checkMissingGlyphsCheckBox->isChecked();
	checkerProfile.checkOrphans = checkItemsNotOnAPageCheckBox->isChecked();
	checkerProfile.checkOverflow = checkTextOverflowCheckBox->isChecked();
	checkerProfile.checkPictures = checkMissingImagesCheckBox->isChecked();
	checkerProfile.checkPartFilledImageFrames = checkPartFilledImageFramesCheckBox->isChecked();
	checkerProfile.checkResolution = checkImageResolutionCheckBox->isChecked();
	checkerProfile.checkTransparency = checkTransparenciesCheckBox->isChecked();
	checkerProfile.minResolution = minimumResolutionSpinBox->value();
	checkerProfile.maxResolution = maximumResolutionSpinBox->value();
	checkerProfile.checkAnnotations = checkPDFAnnotFieldsCheckBox->isChecked();
	checkerProfile.checkRasterPDF = checkPlacedPDFCheckBox->isChecked();
	checkerProfile.checkForGIF = checkGIFsCheckBox->isChecked();
	checkerProfile.ignoreOffLayers = checkNonPrintableLayersCheckBox->isChecked();
	checkerProfile.checkOffConflictLayers = checkVisiblePrintableLayersCheckBox->isChecked();
	checkerProfile.checkAppliedMasterDifferentSide = checkAppliedMasterPageLocationCheckBox->isChecked();
	checkerProfile.checkEmptyTextFrames = checkEmptyTextFramesCheckBox->isChecked();
}

void Prefs_PreflightVerifier::setProfile(const QString& name)
{
	if (checkerProfiles.contains(name))
	{
		putProfile();
		updateProfile(name);
		tempNewProfileName = "";
		addProfilePushButton->setEnabled(false);
	}
	else
	{
		tempNewProfileName = name;
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
	disconnect(checkAppliedMasterPageLocationCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkEmptyTextFramesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));

	CheckerPrefs& checkerProfile = checkerProfiles[name];
	ignoreAllErrorsCheckBox->setChecked(checkerProfile.ignoreErrors);
	autoCheckBeforePrintExportCheckBox->setChecked(checkerProfile.autoCheck);
	checkMissingGlyphsCheckBox->setChecked(checkerProfile.checkGlyphs);
	checkItemsNotOnAPageCheckBox->setChecked(checkerProfile.checkOrphans);
	checkTextOverflowCheckBox->setChecked(checkerProfile.checkOverflow);
	checkTransparenciesCheckBox->setChecked(checkerProfile.checkTransparency);
	checkMissingImagesCheckBox->setChecked(checkerProfile.checkPictures);
	checkPartFilledImageFramesCheckBox->setChecked(checkerProfile.checkPartFilledImageFrames);
	checkImageResolutionCheckBox->setChecked(checkerProfile.checkResolution);
	minimumResolutionSpinBox->setValue( qRound(checkerProfile.minResolution) );
	maximumResolutionSpinBox->setValue( qRound(checkerProfile.maxResolution) );
	checkPDFAnnotFieldsCheckBox->setChecked(checkerProfile.checkAnnotations);
	checkPlacedPDFCheckBox->setChecked(checkerProfile.checkRasterPDF);
	checkGIFsCheckBox->setChecked(checkerProfile.checkForGIF);
	checkNonPrintableLayersCheckBox->setChecked(checkerProfile.ignoreOffLayers);
	checkVisiblePrintableLayersCheckBox->setChecked(checkerProfile.checkOffConflictLayers);
	checkAppliedMasterPageLocationCheckBox->setChecked(checkerProfile.checkAppliedMasterDifferentSide);
	checkEmptyTextFramesCheckBox->setChecked(checkerProfile.checkEmptyTextFrames);
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
	connect(checkAppliedMasterPageLocationCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkEmptyTextFramesCheckBox, SIGNAL(clicked()), this, SLOT(putProfile()));
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
	checkerSettings.checkAppliedMasterDifferentSide = checkAppliedMasterPageLocationCheckBox->isChecked();
	checkerSettings.checkEmptyTextFrames = checkEmptyTextFramesCheckBox->isChecked();
	checkerProfiles.insert(tempNewProfileName, checkerSettings);
	currentProfile = tempNewProfileName;
	if (checkerProfiles.count() > 1)
		removeProfilePushButton->setEnabled(true);
	addProfilePushButton->setEnabled(false);
	currentProfileComboBox->clear();

	disconnect(currentProfileComboBox, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	disconnect(currentProfileComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	CheckerPrefsList::Iterator it;
	int j, i = 0;
	for (it = checkerProfiles.begin(), j = 0; it != checkerProfiles.end(); ++it, ++j)
	{
		currentProfileComboBox->addItem(it.key());
		if (it.key() == currentProfile)
			i = j;
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
	checkerProfiles.remove(currentProfile);
	updateProfile(checkerProfiles.begin().key());
	currentProfileComboBox->clear();
	CheckerPrefsList::Iterator it;
	CheckerPrefsList::Iterator itend = checkerProfiles.end();
	for (it = checkerProfiles.begin(); it != itend; ++it)
		currentProfileComboBox->addItem(it.key());
	setCurrentComboItem(currentProfileComboBox, currentProfile);
	connect(currentProfileComboBox, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(currentProfileComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	if (checkerProfiles.count() == 1)
		removeProfilePushButton->setEnabled(false);
}
