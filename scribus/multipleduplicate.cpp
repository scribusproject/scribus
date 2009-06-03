/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "multipleduplicate.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTabWidget>

#include "scrspinbox.h"
#include "units.h"
#include "usertaskstructs.h"

MultipleDuplicate::MultipleDuplicate( int unitIndex, QWidget* parent, Qt::WFlags fl )
	: QDialog(parent, fl),
	m_unitIndex(unitIndex)
{
	setupUi(this);
	//set tab order
	QWidget::setTabOrder(createGapRadioButton, horizShiftSpinBox);
	QWidget::setTabOrder(horizShiftSpinBox, vertShiftSpinBox);
	QWidget::setTabOrder(gridColsSpinBox, horizRCGapSpinBox);
	QWidget::setTabOrder(horizRCGapSpinBox, vertRCGapSpinBox);
	QWidget::setTabOrder(vertRCGapSpinBox, rotationSpinBox);
	
	//set up mspinboxes
	horizShiftSpinBox->setNewUnit(unitIndex);
	vertShiftSpinBox->setNewUnit(unitIndex);
	horizRCGapSpinBox->setNewUnit(unitIndex);
	vertRCGapSpinBox->setNewUnit(unitIndex);
	horizShiftSpinBox->setMinimum(-1000);
	vertShiftSpinBox->setMinimum(-1000);
	horizRCGapSpinBox->setMinimum(-1000);
	vertRCGapSpinBox->setMinimum(-1000);
	horizShiftSpinBox->setMaximum(1000);
	vertShiftSpinBox->setMaximum(1000);
	horizRCGapSpinBox->setMaximum(1000);
	vertRCGapSpinBox->setMaximum(1000);

	rotationSpinBox->setValues(-180.0, 180.0, 6, 0.0);
	rotationSpinBox->setDecimals(1);
	rotationSpinBox->setNewUnit(6);
	
	createGapRadioButton->setChecked(true);
	setCopiesGap();
	// signals and slots connections
	connect(createGapRadioButton, SIGNAL(clicked()), this, SLOT(setCopiesGap()));
	connect(shiftCreatedItemsRadioButton, SIGNAL(clicked()), this, SLOT(setCopiesShift()));
}

MultipleDuplicate::~MultipleDuplicate()
{
}

void MultipleDuplicate::setCopiesShift()
{
	horizShiftLabel->setText( tr("&Horizontal Shift:"));
	vertShiftLabel->setText( tr("&Vertical Shift:"));
}

void MultipleDuplicate::setCopiesGap()
{
	horizShiftLabel->setText( tr("&Horizontal Gap:"));
	vertShiftLabel->setText( tr("&Vertical Gap:"));
}

void MultipleDuplicate::getMultiplyData(ItemMultipleDuplicateData& mdData)
{
	mdData.type = tabWidget->currentIndex();
	mdData.copyCount = numberOfCopiesSpinBox->value();
	mdData.copyShiftOrGap = createGapRadioButton->isChecked() ? 1 : 0;
	mdData.copyShiftGapH = horizShiftSpinBox->value();
	mdData.copyShiftGapV = vertShiftSpinBox->value();
	mdData.copyRotation = rotationSpinBox->value();
	mdData.gridRows = gridRowsSpinBox->value();
	mdData.gridCols = gridColsSpinBox->value();
	mdData.gridGapH = horizRCGapSpinBox->value();
	mdData.gridGapV = vertRCGapSpinBox->value();
}
