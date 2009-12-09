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
	textSizeSpinBox->setNewUnit(0);
	textSizeSpinBox->setMinimum(0.5);
	textSizeSpinBox->setMaximum(2048);
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
	textColumnGapSpinBox->setNewUnit(newIndex);
	textTabWidthSpinBox->setNewUnit(newIndex);
}

void Prefs_ItemTools::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	enableSignals(false);
	int docUnitIndex = prefsData->docSetupPrefs.docUnitIndex;
	double unitRatio = unitGetRatioFromIndex(docUnitIndex);
	unitChange(docUnitIndex);

	PrefsManager* prefsManager=PrefsManager::instance();
	ColorList::Iterator itc, endOfColorList;
	//TODO ColorList* colorList = (docu != 0) ? (&docu->PageColors) : prefsManager->colorSetPtr();
	ColorList* colorList = prefsManager->colorSetPtr();


	//Text Tool

//TODO	textPreviewWidget

	for (int fc=0; fc<textFontComboBox->count(); ++fc)
	{
		if (textFontComboBox->itemText(fc) == prefsData->itemToolPrefs.textFont)
		{
			textFontComboBox->setCurrentIndex(fc);
			break;
		}
	}

	textSizeSpinBox->setValue(prefsData->itemToolPrefs.textSize / 10);
	textColorComboBox->initColorList(colorList, m_doc, prefsData->itemToolPrefs.textColor);
	textColorShadingSpinBox->setValue(prefsData->itemToolPrefs.textShade);
	textStrokeColorComboBox->initColorList(colorList, m_doc, prefsData->itemToolPrefs.textStrokeColor);
	textStrokeShadingSpinBox->setValue(prefsData->itemToolPrefs.textStrokeShade);
	textFrameFillColorComboBox->initColorList(colorList, m_doc, prefsData->itemToolPrefs.textFillColor);
	textFrameFillShadingSpinBox->setValue(prefsData->itemToolPrefs.textFillColorShade);
	textFrameLineColorComboBox->initColorList(colorList, m_doc, prefsData->itemToolPrefs.textLineColor);
	textFrameLineShadingSpinBox->setValue(prefsData->itemToolPrefs.textLineColorShade);

	textTabFillCharComboBox->clear();
	textTabFillCharComboBox->addItem( tr("None", "tab fill" ));
	textTabFillCharComboBox->addItem( tr("Dot"));
	textTabFillCharComboBox->addItem( tr("Hyphen"));
	textTabFillCharComboBox->addItem( tr("Underscore"));
	textTabFillCharComboBox->addItem( tr("Custom"));
	if (prefsData->itemToolPrefs.textTabFillChar.isEmpty())
	{
		textTabFillCharComboBox->setCurrentIndex(0);
	}
	else if (prefsData->itemToolPrefs.textTabFillChar == ".")
	{
		textTabFillCharComboBox->setCurrentIndex(1);
	}
	else if (prefsData->itemToolPrefs.textTabFillChar == "-")
	{
		textTabFillCharComboBox->setCurrentIndex(2);
	}
	else if (prefsData->itemToolPrefs.textTabFillChar == "_")
	{
		textTabFillCharComboBox->setCurrentIndex(3);
	}
	else
	{
		textTabFillCharComboBox->setCurrentIndex(4);
		textTabFillCharComboBox->setEditable(true);
		textTabFillCharComboBox->setItemText(textTabFillCharComboBox->currentIndex(), CommonStrings::trCustomTabFill + prefsData->itemToolPrefs.textTabFillChar);
	}

	textTabWidthSpinBox->setValue(prefsData->itemToolPrefs.textTabWidth * unitRatio);
	textColumnsSpinBox->setValue(prefsData->itemToolPrefs.textColumns);
	textColumnGapSpinBox->setValue(prefsData->itemToolPrefs.textColumnGap * unitRatio);


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

	//Image Tool

//TODO	imageFrameLineColorComboBox
//TODO	imageFrameLineShadingSpinBox

	imageFreeScalingRadioButton->setChecked( prefsData->itemToolPrefs.imageScaleType );
	imageFrameScalingRadioButton->setChecked( !prefsData->itemToolPrefs.imageScaleType );
	imageHorizontalScalingSpinBox->setValue(qRound(prefsData->itemToolPrefs.imageScaleX * 100));
	imageVerticalScalingSpinBox->setValue(qRound(prefsData->itemToolPrefs.imageScaleY * 100));

	imageKeepAspectRatioCheckBox->setChecked(prefsData->itemToolPrefs.imageAspectRatio);
	imageFrameFillColorComboBox->initColorList(colorList, m_doc, prefsData->itemToolPrefs.imageFillColor);

	imageFrameFillShadingSpinBox->setValue(prefsData->itemToolPrefs.imageFillColorShade );
	imageUseEmbeddedClippingPathCheckBox->setChecked(prefsData->itemToolPrefs.imageUseEmbeddedPath);
	switch (prefsData->itemToolPrefs.imageLowResType)
	{
		case 0:
			onscreenResolutionFullRadioButton->setChecked(true);
			break;
		case 1:
			onscreenResolutionNormalRadioButton->setChecked(true);
			break;
		case 2:
			onscreenResolutionLowRadioButton->setChecked(true);
			break;
	}

	//Shape Tool
	shapeFrameLineColorComboBox->initColorList(colorList, m_doc, prefsData->itemToolPrefs.shapeLineColor);
	shapeFrameLineShadingSpinBox->setValue(prefsData->itemToolPrefs.shapeLineColorShade);
	shapeFrameFillColorComboBox->initColorList(colorList, m_doc, prefsData->itemToolPrefs.shapeFillColor);
	shapeFrameFillShadingSpinBox->setValue(prefsData->itemToolPrefs.shapeFillColorShade);
	shapeFrameLineStyleComboBox->setCurrentIndex(static_cast<int>(prefsData->itemToolPrefs.shapeLineStyle) - 1);
	shapeLineWidthSpinBox->setValue(prefsData->itemToolPrefs.shapeLineWidth);

	//Polygon Tool
//TODO	polygonCornerCountSpinBox
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
	lineFillColorComboBox->initColorList(colorList, m_doc, prefsData->itemToolPrefs.lineColor);

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

	//Text Tool
	prefsData->itemToolPrefs.textFont = textFontComboBox->currentText();
	prefsData->itemToolPrefs.textSize = textSizeSpinBox->value() * 10;
	prefsData->itemToolPrefs.textFillColor = textFrameFillColorComboBox->currentText();
	if (prefsData->itemToolPrefs.textFillColor == CommonStrings::tr_NoneColor)
		prefsData->itemToolPrefs.textFillColor = CommonStrings::None;
	prefsData->itemToolPrefs.textLineColor = textFrameLineColorComboBox->currentText();
	if (prefsData->itemToolPrefs.textLineColor == CommonStrings::tr_NoneColor)
		prefsData->itemToolPrefs.textLineColor = CommonStrings::None;
	prefsData->itemToolPrefs.textFillColorShade = textFrameFillShadingSpinBox->value();
	prefsData->itemToolPrefs.textLineColorShade = textFrameLineShadingSpinBox->value();
	prefsData->itemToolPrefs.textShade = textColorShadingSpinBox->value();
	prefsData->itemToolPrefs.textStrokeShade = textStrokeShadingSpinBox->value();
	prefsData->itemToolPrefs.textColor = textColorComboBox->currentText();
	if (prefsData->itemToolPrefs.textColor == CommonStrings::tr_NoneColor)
		prefsData->itemToolPrefs.textColor = CommonStrings::None;
	prefsData->itemToolPrefs.textStrokeColor = textStrokeColorComboBox->currentText();
	if (prefsData->itemToolPrefs.textStrokeColor == CommonStrings::tr_NoneColor)
		prefsData->itemToolPrefs.textStrokeColor = CommonStrings::None;
	prefsData->itemToolPrefs.textColumns = textColumnsSpinBox->value();
	int docUnitIndex = prefsData->docSetupPrefs.docUnitIndex;
	double unitRatio = unitGetRatioFromIndex(docUnitIndex);
	prefsData->itemToolPrefs.textColumnGap = textColumnGapSpinBox->value() / unitRatio;
	prefsData->itemToolPrefs.textTabWidth = textTabWidthSpinBox->value() / unitRatio;

	switch (textTabFillCharComboBox->currentIndex())
	{
		case 0:
			prefsData->itemToolPrefs.textTabFillChar = "";
			break;
		case 1:
			prefsData->itemToolPrefs.textTabFillChar = ".";
			break;
		case 2:
			prefsData->itemToolPrefs.textTabFillChar = "-";
			break;
		case 3:
			prefsData->itemToolPrefs.textTabFillChar = "_";
			break;
		case 4:
			prefsData->itemToolPrefs.textTabFillChar = textTabFillCharComboBox->currentText().right(1);
			break;
	}




	//Image Tool
//
//	TODO imageFrameLineColorComboBox
//	TODO imageFrameLineShadingSpinBox
//	TODO scalingLockToolButton

	prefsData->itemToolPrefs.imageFillColor = imageFrameFillColorComboBox->currentText();
	if (prefsData->itemToolPrefs.imageFillColor == CommonStrings::tr_NoneColor)
		prefsData->itemToolPrefs.imageFillColor = CommonStrings::None;
	prefsData->itemToolPrefs.imageFillColorShade = imageFrameFillShadingSpinBox->value();
	prefsData->itemToolPrefs.imageScaleX = static_cast<double>(imageHorizontalScalingSpinBox->value()) / 100.0;
	prefsData->itemToolPrefs.imageScaleY = static_cast<double>(imageVerticalScalingSpinBox->value()) / 100.0;
	prefsData->itemToolPrefs.imageScaleType = imageFreeScalingRadioButton->isChecked();
	prefsData->itemToolPrefs.imageAspectRatio = imageKeepAspectRatioCheckBox->isChecked();
	prefsData->itemToolPrefs.imageUseEmbeddedPath = imageUseEmbeddedClippingPathCheckBox->isChecked();
	int haRes = 0;
	if (onscreenResolutionFullRadioButton->isChecked())
		haRes = 0;
	if (onscreenResolutionNormalRadioButton->isChecked())
		haRes = 1;
	if (onscreenResolutionLowRadioButton->isChecked())
		haRes = 2;
	prefsData->itemToolPrefs.imageLowResType = haRes;


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
