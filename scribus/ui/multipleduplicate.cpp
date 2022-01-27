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

#include "iconmanager.h"
#include "scribusdoc.h"
#include "selection.h"
#include "scrspinbox.h"
#include "units.h"
#include "ui/createrange.h"
#include "usertaskstructs.h"

MultipleDuplicate::MultipleDuplicate(QWidget* parent, ScribusDoc *doc) : QDialog(parent), m_Doc(doc)
{
	setupUi(this);

	m_unitRatio = unitGetRatioFromIndex(m_Doc->unitIndex());

	//set up mspinboxes
	horizShiftSpinBox->setNewUnit(m_Doc->unitIndex());
	vertShiftSpinBox->setNewUnit(m_Doc->unitIndex());
	horizRCGapSpinBox->setNewUnit(m_Doc->unitIndex());
	vertRCGapSpinBox->setNewUnit(m_Doc->unitIndex());
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

	if (!m_Doc->m_Selection->itemsAreOnSamePage())
	{
		radioButtonPageOdd->setEnabled(false);
		radioButtonPageEven->setEnabled(false);
	}
	
	createGapRadioButton->setChecked(true);
	setCopiesGap();

	toolButtonPageRange->setIcon(IconManager::instance().loadIcon("ellipsis.png"));
	radioButtonPageAll->setChecked(true);

	// signals and slots connections
	connect(createGapRadioButton, SIGNAL(clicked()), this, SLOT(setCopiesGap()));
	connect(shiftCreatedItemsRadioButton, SIGNAL(clicked()), this, SLOT(setCopiesShift()));

	connect(lineEditPageRange, &QLineEdit::textChanged, this, &MultipleDuplicate::selectRangeOfPages);
	connect(toolButtonPageRange, &QToolButton::clicked, this, &MultipleDuplicate::createPageNumberRange);
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
	if (radioButtonPageAll->isChecked())
		mdData.pageSelection = 1;
	else if (radioButtonPageEven->isChecked())
		mdData.pageSelection = 2;
	else if (radioButtonPageOdd->isChecked())
		mdData.pageSelection = 3;
	else if (radioButtonPageRange->isChecked())
		mdData.pageSelection = 4;
	mdData.pageRange = lineEditPageRange->text();
	mdData.pageLinkText = checkBoxPageLinkText->isChecked();
}

void MultipleDuplicate::selectRangeOfPages()
{
	radioButtonPageRange->setChecked(true);
}

void MultipleDuplicate::createPageNumberRange()
{
	if (m_Doc != nullptr)
	{
		CreateRange cr(lineEditPageRange->text(), m_Doc->Pages->count(), this);
		if (cr.exec())
		{
			CreateRangeData crData;
			cr.getCreateRangeData(crData);
			lineEditPageRange->setText(crData.pageRange);
			return;
		}
	}
	lineEditPageRange->clear();
}
