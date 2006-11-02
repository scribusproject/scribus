/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "multipleduplicate.h"
#include "multipleduplicate.moc"

#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qtabwidget.h>

#include "mspinbox.h"
#include "units.h"

MultipleDuplicate::MultipleDuplicate( int unitIndex, QWidget* parent, const char* name, WFlags fl )
	: MultipleDuplicateBase(parent, name, fl),
	m_unitIndex(unitIndex)
{

	//set tab order
	QWidget::setTabOrder(createGapRadioButton, horizShiftMSpinBox);
	QWidget::setTabOrder(horizShiftMSpinBox, vertShiftMSpinBox);
	QWidget::setTabOrder(gridColsSpinBox, horizRCGapMSpinBox);
	QWidget::setTabOrder(horizRCGapMSpinBox, vertRCGapMSpinBox);
	
	//set up mspinboxes
	int decimals = unitGetDecimalsFromIndex(unitIndex);
	QString unitSuffix(unitGetSuffixFromIndex(unitIndex));
	horizShiftMSpinBox->setValues(-1000.0, 1000.0, decimals, 0.0);
	vertShiftMSpinBox->setValues(-1000.0, 1000.0, decimals, 0.0);
	horizRCGapMSpinBox->setValues(-1000.0, 1000.0, decimals, 0.0);
	vertRCGapMSpinBox->setValues(-1000.0, 1000.0, decimals, 0.0);
	horizShiftMSpinBox->setSuffix(unitSuffix);
	vertShiftMSpinBox->setSuffix(unitSuffix);
	horizRCGapMSpinBox->setSuffix(unitSuffix);
	vertRCGapMSpinBox->setSuffix(unitSuffix);
	
	copiesCreateButtonGroup->setButton(0);
	// signals and slots connections
	connect(copiesCreateButtonGroup, SIGNAL(clicked(int)), this, SLOT(setCopiesShiftGap(int)));
}

MultipleDuplicate::~MultipleDuplicate()
{
}

void MultipleDuplicate::setCopiesShiftGap(int sel)
{
	if (sel==0)
	{
		horizShiftLabel->setText( tr("&Horizontal Shift:"));
		vertShiftLabel->setText( tr("&Vertical Shift:"));
	}
	else
	{
		horizShiftLabel->setText( tr("&Horizontal Gap:"));
		vertShiftLabel->setText( tr("&Vertical Gap:"));
	}
}

void MultipleDuplicate::getMultiplyData(ItemMultipleDuplicateData& mdData)
{
	mdData.type=tabWidget->currentPageIndex();
	mdData.copyCount=numberOfCopiesSpinBox->value();
	mdData.copyShiftOrGap=copiesCreateButtonGroup->selectedId();
	mdData.copyShiftGapH=horizShiftMSpinBox->value();
	mdData.copyShiftGapV=vertShiftMSpinBox->value();
	mdData.gridRows=gridRowsSpinBox->value();
	mdData.gridCols=gridColsSpinBox->value();
	mdData.gridGapH=horizRCGapMSpinBox->value();
	mdData.gridGapV=vertRCGapMSpinBox->value();
}
