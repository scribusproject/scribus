/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_display.h"

#include "prefsstructs.h"
#include "units.h"

Prefs_Display::Prefs_Display(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);
}

Prefs_Display::~Prefs_Display()
{
}

void Prefs_Display::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	docUnitIndex = prefsData->docSetupPrefs.docUnitIndex;
	double unitRatio = unitGetRatioFromIndex(docUnitIndex);
	int decimals = unitGetPrecisionFromIndex(docUnitIndex);
	QString unitSuffix = unitGetSuffixFromIndex(docUnitIndex);

	showImagesCheckBox->setChecked(prefsData->guidesPrefs.showPic);
	showControlCharsCheckBox->setChecked(prefsData->guidesPrefs.showControls);
	showRulersRelativeToPageCheckBox->setChecked(prefsData->guidesPrefs.rulerMode);
	showTextChainsCheckBox->setChecked(prefsData->guidesPrefs.linkShown);
	showFramesCheckBox->setChecked(prefsData->guidesPrefs.framesShown);
	showLayerIndicatorsCheckBox->setChecked(prefsData->guidesPrefs.layerMarkersShown);
	showUnprintableAreaInMarginColorCheckBox->setChecked(prefsData->displayPrefs.marginColored);
	showBleedAreaCheckBox->setChecked(prefsData->guidesPrefs.showBleed);
	showPageShadowCheckBox->setChecked(prefsData->displayPrefs.showPageShadow);


	scratchSpaceLeftSpinBox->setMaximum(1000);
	scratchSpaceRightSpinBox->setMaximum(1000);
	scratchSpaceTopSpinBox->setMaximum(1000);
	scratchSpaceBottomSpinBox->setMaximum(1000);
	pageGapHorizontalSpinBox->setMaximum(1000);
	pageGapVerticalSpinBox->setMaximum(1000);
	scratchSpaceLeftSpinBox->setNewUnit(docUnitIndex);
	scratchSpaceRightSpinBox->setNewUnit(docUnitIndex);
	scratchSpaceTopSpinBox->setNewUnit(docUnitIndex);
	scratchSpaceBottomSpinBox->setNewUnit(docUnitIndex);
	pageGapHorizontalSpinBox->setNewUnit(docUnitIndex);
	pageGapVerticalSpinBox->setNewUnit(docUnitIndex);


	scratchSpaceLeftSpinBox->setValue(prefsData->displayPrefs.scratch.Left * unitRatio);
	scratchSpaceRightSpinBox->setValue(prefsData->displayPrefs.scratch.Right * unitRatio);
	scratchSpaceTopSpinBox->setValue(prefsData->displayPrefs.scratch.Top * unitRatio);
	scratchSpaceBottomSpinBox->setValue(prefsData->displayPrefs.scratch.Bottom * unitRatio);
	pageGapHorizontalSpinBox->setValue(prefsData->displayPrefs.pageGapHorizontal);
	pageGapVerticalSpinBox->setValue(prefsData->displayPrefs.pageGapVertical);

	/*
	DisScale = prefsData->displayPrefs.displayScale;

	QPixmap pm(54, 14);
	pm.fill(prefsData->displayPrefs.paperColor);
	colorPaper = prefsData->displayPrefs.paperColor;
	backColor->setIcon(pm);
	backColor->setText( QString::null );
	pm.fill(prefsData->displayPrefs.frameColor);
	colorFrame = prefsData->displayPrefs.frameColor;
	buttonFrameSelected->setText( QString::null );
	buttonFrameSelected->setIcon(pm);
	pm.fill(prefsData->displayPrefs.frameNormColor);
	colorFrameNorm = prefsData->displayPrefs.frameNormColor;
	buttonFrameNormal->setText( QString::null );
	buttonFrameNormal->setIcon(pm);
	pm.fill(prefsData->displayPrefs.frameGroupColor);
	colorFrameGroup = prefsData->displayPrefs.frameGroupColor;
	buttonFrameGrouped->setText( QString::null );
	buttonFrameGrouped->setIcon(pm);
	pm.fill(prefsData->displayPrefs.frameLinkColor);
	colorFrameLinked = prefsData->displayPrefs.frameLinkColor;
	buttonFrameLinked->setText( QString::null );
	buttonFrameLinked->setIcon(pm);
	pm.fill(prefsData->displayPrefs.frameLockColor);
	colorFrameLocked = prefsData->displayPrefs.frameLockColor;
	buttonFrameLocked->setText( QString::null );
	buttonFrameLocked->setIcon(pm);
	pm.fill(prefsData->displayPrefs.frameAnnotationColor);
	colorFrameAnnotation = prefsData->displayPrefs.frameAnnotationColor;
	buttonFrameAnnotation->setText( QString::null );
	buttonFrameAnnotation->setIcon(pm);
	pm.fill(prefsData->displayPrefs.pageBorderColor);
	colorPageBorder = prefsData->displayPrefs.pageBorderColor;
	buttonSelectedPage->setText( QString::null );
	buttonSelectedPage->setIcon(pm);
	pm.fill(prefsData->displayPrefs.controlCharColor);
	colorControlChars = prefsData->displayPrefs.controlCharColor;
	buttonControlChars->setText( QString::null );
	buttonControlChars->setIcon(pm);


	CaliSlider->setValue(qRound(100 * DisScale) - 150);
	CaliAnz->setText(QString::number(qRound(DisScale*72.0))+ tr(" dpi"));
	rulerUnitCombo->clear();
	rulerUnitCombo->addItems(unitGetTextUnitList());
	rulerUnitCombo->setCurrentIndex(docUnitIndex);
	drawRuler();
*/
}

