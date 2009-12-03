/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_itemtools.h"
#include "prefsstructs.h"
#include "scrspinbox.h"
#include "units.h"

Prefs_ItemTools::Prefs_ItemTools(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_ItemTools::~Prefs_ItemTools()
{
}


void Prefs_ItemTools::languageChange()
{
}

void Prefs_ItemTools::unitChange(int newIndex)
{

}

void Prefs_ItemTools::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	int docUnitIndex = prefsData->docSetupPrefs.docUnitIndex;
	unitChange(docUnitIndex);
	/*
	textFrameFillColorComboBox
			textFrameLineColorComboBox
			textFrameFillShadingSpinBox
			textFrameLineShadingSpinBox
			textColumnsSpinBox
			textColumnGapSpinBox
			textTabFillCharComboBox
			textTabWidthSpinBox
			textFontComboBox
			textSizeSpinBox
			textColorComboBox
			textStrokeColorComboBox
			textColorShadingSpinBox
			textStrokeShadingSpinBox
			textPreviewWidget

			imageFrameFillColorComboBox
			imageFrameLineColorComboBox
			imageFrameFillShadingSpinBox
			imageFrameLineShadingSpinBox
			imageUseEmbeddedClippingPathCheckBox
			imageFreeScalingRadioButton
			imageFrameScalingRadioButton
			imageHorizontalScalingSpinBox
			imageVerticalScalingSpinBox
			scalingLockToolButton
			imageKeepAspectRatioCheckBox
			onscreenResolutionFullRadioButton
			onscreenResolutionNormalRadioButton
			onscreenResolutionLowRadioButton

			shapeFrameFillColorComboBox
			shapeFrameLineColorComboBox
			shapeFrameLineStyleComboBox
			shapeLineWidthSpinBox
			shapeFrameFillShadingSpinBox
			shapeFrameLineShadingSpinBox

			polygonCornerCountSpinBox
			polygonRotationSpinBox
			applyFactorCheckBox
			polygonFactorSpinBox
			polygonFactorSlider
			polygonCurvatureSpinBox
			polygonCurvatureSlider
			polygonPreviewWidet

			lineFillColorComboBox
			lineStyleComboBox
			lineWidthSpinBox
			lineStartArrowComboBox
			lineEndArrowComboBox
			lineFillColorShadingSpinBox




			*/
}

void Prefs_ItemTools::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
}
