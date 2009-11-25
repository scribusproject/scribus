/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QColorDialog>
#include "ui/prefs_guides.h"
#include "prefsstructs.h"
#include "scrspinbox.h"
#include "units.h"

Prefs_Guides::Prefs_Guides(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	connect(guideColorPushButton, SIGNAL(clicked()), this, SLOT(changeGuideColor()));
	connect(marginColorPushButton, SIGNAL(clicked()), this, SLOT(changeMarginColor()));
	connect(majorGridColorPushButton, SIGNAL(clicked()), this, SLOT(changeMajorColor()));
	connect(minorGridColorPushButton, SIGNAL(clicked()), this, SLOT(changeMinorColor()));
	connect(baselineGridColorPushButton, SIGNAL(clicked()), this, SLOT(changeBaselineColor()));
}

Prefs_Guides::~Prefs_Guides()
{
}

void Prefs_Guides::languageChange()
{
	int i=guidePlacementComboBox->currentIndex();
	guidePlacementComboBox->clear();
	guidePlacementComboBox->addItem( tr ("In the Background") );
	guidePlacementComboBox->addItem( tr ("In the Foreground") );
	guidePlacementComboBox->setCurrentIndex(i<0?0:i);
}

void Prefs_Guides::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	int docUnitIndex = prefsData->docSetupPrefs.docUnitIndex;
	QString unitSuffix(unitGetSuffixFromIndex(docUnitIndex));
	double unitRatio = unitGetRatioFromIndex(docUnitIndex);
	int decimals = unitGetPrecisionFromIndex(docUnitIndex);
	guidePlacementComboBox->setCurrentIndex(prefsData->guidesPrefs.guidePlacement ?0:1);
	guideSnapDistanceSpinBox->setValue(prefsData->guidesPrefs.guideRad);
	guideGrabRadiusSpinBox->setValue(prefsData->guidesPrefs.grabRadius);
	visibilityGuidesCheckBox->setChecked(prefsData->guidesPrefs.guidesShown);
	visibilityMarginsCheckBox->setChecked(prefsData->guidesPrefs.marginsShown);
	visibilityGridCheckBox->setChecked(prefsData->guidesPrefs.gridShown);
	visibilityBaselineGridCheckBox->setChecked(prefsData->guidesPrefs.baselineGridShown);
	majorGridSpacingSpinBox->setMinimum(10 * unitRatio);
	minorGridSpacingSpinBox->setMinimum(10 * unitRatio);
	majorGridSpacingSpinBox->setMaximum(1000 * unitRatio);
	minorGridSpacingSpinBox->setMaximum(1000 * unitRatio);
	majorGridSpacingSpinBox->setDecimals( decimals );
	minorGridSpacingSpinBox->setDecimals( decimals );
	majorGridSpacingSpinBox->setSuffix( unitSuffix );
	minorGridSpacingSpinBox->setSuffix( unitSuffix );
	majorGridSpacingSpinBox->setValue(prefsData->guidesPrefs.majorGridSpacing  * unitRatio);
	minorGridSpacingSpinBox->setValue(prefsData->guidesPrefs.minorGridSpacing  * unitRatio);
	baselineGridSpacingSpinBox->setValue(prefsData->guidesPrefs.valueBaselineGrid);
	baselineGridOffsetSpinBox->setValue(prefsData->guidesPrefs.offsetBaselineGrid);

	QPixmap pm(100, 30);
	pm.fill(prefsData->guidesPrefs.guideColor);
	colorGuides = prefsData->guidesPrefs.guideColor;
	guideColorPushButton->setText( QString::null );
	guideColorPushButton->setIcon(pm);


	pm.fill(prefsData->guidesPrefs.marginColor);
	colorMargin = prefsData->guidesPrefs.marginColor;
	marginColorPushButton->setText( QString::null );
	marginColorPushButton->setIcon(pm);

	pm.fill(prefsData->guidesPrefs.majorGridColor);
	colorMajorGrid = prefsData->guidesPrefs.majorGridColor;
	majorGridColorPushButton->setText( QString::null );
	majorGridColorPushButton->setIcon(pm);

	pm.fill(prefsData->guidesPrefs.minorGridColor);
	colorMinorGrid = prefsData->guidesPrefs.minorGridColor;
	minorGridColorPushButton->setText( QString::null );
	minorGridColorPushButton->setIcon(pm);

	pm.fill(prefsData->guidesPrefs.baselineGridColor);
	colorBaselineGrid = prefsData->guidesPrefs.baselineGridColor;
	baselineGridColorPushButton->setText( QString::null );
	baselineGridColorPushButton->setIcon(pm);
}

void Prefs_Guides::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->guidesPrefs.guidePlacement=(guidePlacementComboBox->currentIndex()==0);
	prefsData->guidesPrefs.guideRad=guideSnapDistanceSpinBox->value();
	prefsData->guidesPrefs.grabRadius=guideGrabRadiusSpinBox->value();
	prefsData->guidesPrefs.guidesShown=visibilityGuidesCheckBox->isChecked();
	prefsData->guidesPrefs.marginsShown=visibilityMarginsCheckBox->isChecked();
	prefsData->guidesPrefs.gridShown=visibilityGridCheckBox->isChecked();
	prefsData->guidesPrefs.baselineGridShown=visibilityBaselineGridCheckBox->isChecked();
	double unitRatio = unitGetRatioFromIndex(prefsData->docSetupPrefs.docUnitIndex);
	prefsData->guidesPrefs.majorGridSpacing=majorGridSpacingSpinBox->value() / unitRatio;
	prefsData->guidesPrefs.minorGridSpacing=minorGridSpacingSpinBox->value() / unitRatio;
	prefsData->guidesPrefs.valueBaselineGrid=baselineGridSpacingSpinBox->value();
	prefsData->guidesPrefs.offsetBaselineGrid=baselineGridOffsetSpinBox->value();
	prefsData->guidesPrefs.guideColor=colorGuides;
	prefsData->guidesPrefs.marginColor=colorMargin;
	prefsData->guidesPrefs.majorGridColor=colorMajorGrid;
	prefsData->guidesPrefs.minorGridColor=colorMinorGrid;
	prefsData->guidesPrefs.baselineGridColor=colorBaselineGrid;
}

void Prefs_Guides::changeMajorColor()
{
	QColor newColor(QColorDialog::getColor(colorMajorGrid, this));
	if (newColor.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(newColor);
		colorMajorGrid = newColor;
		majorGridColorPushButton->setIcon(pm);
	}
}

void Prefs_Guides::changeMinorColor()
{
	QColor newColor(QColorDialog::getColor(colorMinorGrid, this));
	if (newColor.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(newColor);
		colorMinorGrid = newColor;
		minorGridColorPushButton->setIcon(pm);
	}
}

void Prefs_Guides::changeBaselineColor()
{
	QColor newColor(QColorDialog::getColor(colorBaselineGrid, this));
	if (newColor.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(newColor);
		colorBaselineGrid = newColor;
		baselineGridColorPushButton->setIcon(pm);
	}
}

void Prefs_Guides::changeGuideColor()
{
	QColor newColor(QColorDialog::getColor(colorGuides, this));
	if (newColor.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(newColor);
		colorGuides = newColor;
		guideColorPushButton->setIcon(pm);
	}
}

void Prefs_Guides::changeMarginColor()
{
	QColor newColor(QColorDialog::getColor(colorMargin, this));
	if (newColor.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(newColor);
		colorMargin = newColor;
		marginColorPushButton->setIcon(pm);
	}
}

