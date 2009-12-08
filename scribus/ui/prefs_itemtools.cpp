/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_itemtools.h"
#include "ui/arrowchooser.h"
#include "prefsstructs.h"
#include "scrspinbox.h"
#include "scribusdoc.h"
#include "units.h"
//#include "colorcombo.h"
//#include "colorlistbox.h"
#include "commonstrings.h"
#include "prefsmanager.h"

Prefs_ItemTools::Prefs_ItemTools(QWidget* parent)
	: Prefs_Pane(parent),
	m_doc(NULL)
{
	setupUi(this);
	lineEndArrowComboBox->setStartDirection(false);
	shapeLineWidthSpinBox->setNewUnit(0);
	lineWidthSpinBox->setNewUnit(0);
	shapeLineWidthSpinBox->setMaximum(36);
	lineWidthSpinBox->setMaximum(36);
	shapeLineWidthSpinBox->setDecimals(1);
	lineWidthSpinBox->setDecimals(1);
	shapeLineWidthSpinBox->setSpecialValueText( tr("Hairline"));
	lineWidthSpinBox->setSpecialValueText( tr("Hairline"));


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
	enableSignals(false);
	int docUnitIndex = prefsData->docSetupPrefs.docUnitIndex;
	unitChange(docUnitIndex);

	PrefsManager* prefsManager=PrefsManager::instance();
	ColorList::Iterator itc, endOfColorList;
	//ColorList* colorList = (docu != 0) ? (&docu->PageColors) : prefsManager->colorSetPtr();
	ColorList* colorList = prefsManager->colorSetPtr();


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

*/
/*
			polygonCornerCountSpinBox
			polygonRotationSpinBox
			applyFactorCheckBox
			polygonFactorSpinBox
			polygonFactorSlider
			polygonCurvatureSpinBox
			polygonCurvatureSlider
			polygonPreviewWidet

			*/


	//Shape Tool

//			shapeFrameFillColorComboBox
//			shapeFrameLineColorComboBox
//			shapeFrameLineStyleComboBox
//			shapeLineWidthSpinBox
//			shapeFrameFillShadingSpinBox
//			shapeFrameLineShadingSpinBox




	shapeFrameLineColorComboBox->clear();
	shapeFrameLineColorComboBox->addItem(CommonStrings::tr_NoneColor);
	if (prefsData->itemToolPrefs.shapeLineColor == CommonStrings::None)
		shapeFrameLineColorComboBox->setCurrentIndex(shapeFrameLineColorComboBox->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		shapeFrameLineColorComboBox->insertFancyItem( itc.value(), m_doc, itc.key() );
		if (itc.key() == prefsData->itemToolPrefs.shapeLineColor)
			shapeFrameLineColorComboBox->setCurrentIndex(shapeFrameLineColorComboBox->count()-1);
	}
	shapeFrameLineShadingSpinBox->setValue(prefsData->itemToolPrefs.shapeLineColorShade);

	shapeFrameFillColorComboBox->clear();
	shapeFrameFillColorComboBox->addItem( tr("None"));
	if (prefsData->itemToolPrefs.shapeFillColor == CommonStrings::None)
		shapeFrameFillColorComboBox->setCurrentIndex(shapeFrameFillColorComboBox->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		shapeFrameFillColorComboBox->insertFancyItem( itc.value(), m_doc, itc.key() );
		if (itc.key() == prefsData->itemToolPrefs.shapeFillColor)
			shapeFrameFillColorComboBox->setCurrentIndex(shapeFrameFillColorComboBox->count()-1);
	}

	shapeFrameFillShadingSpinBox->setValue(prefsData->itemToolPrefs.shapeFillColorShade);
	shapeFrameLineStyleComboBox->setCurrentIndex(static_cast<int>(prefsData->itemToolPrefs.shapeLineStyle) - 1);
	shapeLineWidthSpinBox->setValue(prefsData->itemToolPrefs.shapeLineWidth);

	//Polygon Tool
//	polygonCornerCountSpinBox
//	polygonRotationSpinBox
//	applyFactorCheckBox
//	polygonFactorSpinBox
//	polygonFactorSlider
//	polygonCurvatureSpinBox
//	polygonCurvatureSlider
//
//
//	prefsData->polyCorners,
//									   prefsData->polyFactorGuiVal,
//									   prefsData->polyFactor,
//									   prefsData->polyUseFactor,
//									   prefsData->polyRotation,
//									   prefsData->polyCurvature


	//Line Tool
	lineFillColorComboBox->clear();
	lineFillColorComboBox->addItem(CommonStrings::tr_NoneColor);
	if (prefsData->itemToolPrefs.lineColor == CommonStrings::None)
		lineFillColorComboBox->setCurrentIndex(lineFillColorComboBox->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		lineFillColorComboBox->insertFancyItem( itc.value(), m_doc, itc.key() );
		if (itc.key() == prefsData->itemToolPrefs.lineColor)
			lineFillColorComboBox->setCurrentIndex(lineFillColorComboBox->count()-1);
	}
	lineFillColorShadingSpinBox->setValue(prefsData->itemToolPrefs.lineColorShade);
	lineStyleComboBox->setCurrentIndex(static_cast<int>(prefsData->itemToolPrefs.lineStyle) - 1);
	if (m_doc != 0)
	{
		lineStartArrowComboBox->rebuildList(&m_doc->arrowStyles);
		lineEndArrowComboBox->rebuildList(&m_doc->arrowStyles);
	}
	else
	{
		lineStartArrowComboBox->rebuildList(&prefsData->arrowStyles);
		lineEndArrowComboBox->rebuildList(&prefsData->arrowStyles);
	}
	lineStartArrowComboBox->setCurrentIndex(prefsData->itemToolPrefs.lineStartArrow);
	lineEndArrowComboBox->setCurrentIndex(prefsData->itemToolPrefs.lineEndArrow);
	lineWidthSpinBox->setValue(prefsData->itemToolPrefs.lineWidth);
	//


	enableSignals(true);
}

void Prefs_ItemTools::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{

	//Shape tool
	prefsData->itemToolPrefs.shapeLineColor = shapeFrameLineColorComboBox->currentText();
	if (prefsData->itemToolPrefs.shapeLineColor == CommonStrings::tr_NoneColor)
		prefsData->itemToolPrefs.shapeLineColor = CommonStrings::None;
	prefsData->itemToolPrefs.shapeFillColor = shapeFrameFillColorComboBox->currentText();
	if (prefsData->itemToolPrefs.shapeFillColor == CommonStrings::tr_NoneColor)
		prefsData->itemToolPrefs.shapeFillColor = CommonStrings::None;
	prefsData->itemToolPrefs.shapeFillColorShade = shapeFrameFillShadingSpinBox->value();
	prefsData->itemToolPrefs.shapeLineColorShade = shapeFrameLineShadingSpinBox->value();
	prefsData->itemToolPrefs.shapeLineStyle = static_cast<Qt::PenStyle>(shapeFrameLineStyleComboBox->currentIndex()) + 1;
	prefsData->itemToolPrefs.shapeLineWidth = shapeLineWidthSpinBox->value();



	//Polygon Tool
//	polygonCornerCountSpinBox
//	polygonRotationSpinBox
//	applyFactorCheckBox
//	polygonFactorSpinBox
//	polygonFactorSlider
//	polygonCurvatureSpinBox
//	polygonCurvatureSlider
//	polygonPreviewWidet

	//Line Tool
	prefsData->itemToolPrefs.lineColor = lineFillColorComboBox->currentText();
	if (prefsData->itemToolPrefs.lineColor == CommonStrings::tr_NoneColor)
		prefsData->itemToolPrefs.lineColor = CommonStrings::None;
	prefsData->itemToolPrefs.lineColorShade = lineFillColorShadingSpinBox->value();
	prefsData->itemToolPrefs.lineStyle = static_cast<Qt::PenStyle>(lineStyleComboBox->currentIndex()) + 1;
	prefsData->itemToolPrefs.lineWidth = lineWidthSpinBox->value();
	prefsData->itemToolPrefs.lineStartArrow = lineStartArrowComboBox->currentIndex();
	prefsData->itemToolPrefs.lineEndArrow = lineEndArrowComboBox->currentIndex();
}

void Prefs_ItemTools::enableSignals(bool on)
{
	if (on)
	{
//		connect(fontComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
//		connect(sizeComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
//		connect(colorComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
//		connect(colorComboTextBackground, SIGNAL(activated(int)), this, SLOT(setSample()));
//		connect(shadingTextBack, SIGNAL(valueChanged(int)), this, SLOT(setSample()));
//		connect(shadingText, SIGNAL(valueChanged(int)), this, SLOT(setSample()));
//		connect(buttonGroup3, SIGNAL(toggled(bool)), this, SLOT(changeImageScalingFree(bool)));
//		connect(buttonGroup5, SIGNAL(toggled(bool)), this, SLOT(changeImageScalingRatio(bool)));
//		connect(chainButton, SIGNAL(clicked()), this, SLOT(toggleChain()));
//		connect(scalingHorizontal, SIGNAL(valueChanged(int)), this, SLOT(hChange()));
//		connect(scalingVertical, SIGNAL(valueChanged(int)), this, SLOT(vChange()));
//		connect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar(int)));
	}
	else
	{
//		disconnect(fontComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
//		disconnect(sizeComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
//		disconnect(colorComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
//		disconnect(colorComboTextBackground, SIGNAL(activated(int)), this, SLOT(setSample()));
//		disconnect(shadingTextBack, SIGNAL(valueChanged(int)), this, SLOT(setSample()));
//		disconnect(shadingText, SIGNAL(valueChanged(int)), this, SLOT(setSample()));
//		disconnect(buttonGroup3, SIGNAL(toggled(bool)), this, SLOT(changeImageScalingFree(bool)));
//		disconnect(buttonGroup5, SIGNAL(toggled(bool)), this, SLOT(changeImageScalingRatio(bool)));
//		disconnect(chainButton, SIGNAL(clicked()), this, SLOT(toggleChain()));
//		disconnect(scalingHorizontal, SIGNAL(valueChanged(int)), this, SLOT(hChange()));
//		disconnect(scalingVertical, SIGNAL(valueChanged(int)), this, SLOT(vChange()));
//		disconnect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar(int)));
	}
}
