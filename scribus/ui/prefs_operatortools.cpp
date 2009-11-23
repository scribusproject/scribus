/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QSpinBox>
#include "ui/prefs_operatortools.h"
#include "prefsstructs.h"
#include "scrspinbox.h"
#include "units.h"

Prefs_OperatorTools::Prefs_OperatorTools(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	itemDuplicateXDispSpinBox->setMaximum(1000);
	itemDuplicateYDispSpinBox->setMaximum(1000);
	itemDuplicateXDispSpinBox->setMinimum(-1000);
	itemDuplicateYDispSpinBox->setMinimum(-1000);

	rotationConstraintSpinBox->setNewUnit(6);
	rotationConstraintSpinBox->setMaximum(359.99);
	rotationConstraintSpinBox->setMinimum(0);
	languageChange();
}

Prefs_OperatorTools::~Prefs_OperatorTools()
{
}

void Prefs_OperatorTools::languageChange()
{

}

void Prefs_OperatorTools::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	zoomMinimumSpinBox->setValue(prefsData->opToolPrefs.magMin);
	zoomMaximumSpinBox->setValue(prefsData->opToolPrefs.magMax);
	zoomSteppingSpinBox->setValue(prefsData->opToolPrefs.magStep);
	double unitRatio = unitGetRatioFromIndex(prefsData->docSetupPrefs.docUnitIndex);
	itemDuplicateXDispSpinBox->setNewUnit(prefsData->docSetupPrefs.docUnitIndex);
	itemDuplicateYDispSpinBox->setNewUnit(prefsData->docSetupPrefs.docUnitIndex);
	itemDuplicateXDispSpinBox->setValue(prefsData->opToolPrefs.dispX * unitRatio);
	itemDuplicateYDispSpinBox->setValue(prefsData->opToolPrefs.dispY * unitRatio);
	rotationConstraintSpinBox->setValue(prefsData->opToolPrefs.constrain);
}

void Prefs_OperatorTools::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->opToolPrefs.magMin=zoomMinimumSpinBox->value();
	prefsData->opToolPrefs.magMax=zoomMaximumSpinBox->value();
	prefsData->opToolPrefs.magStep=zoomSteppingSpinBox->value();
	double unitRatio = unitGetRatioFromIndex(prefsData->docSetupPrefs.docUnitIndex);
	prefsData->opToolPrefs.dispX=itemDuplicateXDispSpinBox->value() / unitRatio;
	prefsData->opToolPrefs.dispY=itemDuplicateYDispSpinBox->value() / unitRatio;
	prefsData->opToolPrefs.constrain=rotationConstraintSpinBox->value();
}


