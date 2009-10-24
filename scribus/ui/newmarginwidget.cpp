/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "newmarginwidget.h"
#include "scrspinbox.h"
#include "units.h"
#include "ui/marginpresetlayout.h"
#include "ui/useprintermarginsdialog.h"

NewMarginWidget::NewMarginWidget(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);

}

NewMarginWidget::~NewMarginWidget()
{
}

void NewMarginWidget::setup(const MarginStruct& margs, int unitIndex, bool showPreset)
{
	marginData=margs;
	savedMarginData=margs;
	savedPresetItem=PresetLayout::none;//we dont recheck if we are using a layout but always start at none
}

void NewMarginWidget::setPageWidth(double)
{

}

void NewMarginWidget::setPageHeight(double)
{

}

void NewMarginWidget::setPageSize(const QString &newSize)
{

}

void NewMarginWidget::setNewUnitIndex(int newIndex)
{

}
