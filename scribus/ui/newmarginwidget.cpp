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
	: QWidget(parent),
	savedPresetItem(PresetLayout::none),
	facingPages(false),
	usingPreset(true)
{
	setupUi(this);

}

NewMarginWidget::~NewMarginWidget()
{
}

void NewMarginWidget::setup(const MarginStruct& margs, int layoutType, int unitIndex, bool showPreset, bool showPrinterMargins)
{
	marginData=savedMarginData=margs;
	m_unitIndex=unitIndex;
	m_unitRatio=unitGetRatioFromIndex(unitIndex);
	leftMarginSpinBox->setMaximum(1000);
	rightMarginSpinBox->setMaximum(1000);
	topMarginSpinBox->setMaximum(1000);
	bottomMarginSpinBox->setMaximum(1000);
	leftMarginSpinBox->init(unitIndex);
	rightMarginSpinBox->init(unitIndex);
	topMarginSpinBox->init(unitIndex);
	bottomMarginSpinBox->init(unitIndex);
	updateMarginSpinValues();
	usingPreset=showPreset;
	if (!showPreset)
	{
		presetLayoutComboBox->blockSignals(true);
		presetLayoutComboBox->resize(0,0);
		presetLayoutLabel->resize(0,0);
		presetLayoutComboBox->hide();
		presetLayoutLabel->hide();
		formLayout->removeWidget(presetLayoutComboBox);
		formLayout->removeWidget(presetLayoutLabel);
	}
	if (!showPrinterMargins)
	{
		printerMarginsPushButton->blockSignals(true);
		printerMarginsPushButton->resize(0,0);
		printerMarginsPushButton->hide();
		formLayout->removeWidget(printerMarginsPushButton);
	}
	setFacingPages(!(layoutType == singlePage));

	formLayout->invalidate();

	connect(topMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	connect(bottomMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	connect(leftMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	connect(rightMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
	connect(presetLayoutComboBox, SIGNAL(activated(int)), this, SLOT(setPreset()));
	connect(marginLinkCheckBox, SIGNAL(clicked()), this, SLOT(slotLinkMargins()));
	connect(printerMarginsPushButton, SIGNAL(clicked()), this, SLOT(setMarginsToPrinterMargins()));
}

void NewMarginWidget::languageChange()
{
	leftMarginSpinBox->setToolTip( "<qt>" + tr( "Distance between the top margin guide and the edge of the page" ) + "</qt>");
	rightMarginSpinBox->setToolTip( "<qt>" + tr( "Distance between the bottom margin guide and the edge of the page" ) + "</qt>");
	topMarginSpinBox->setToolTip( "<qt>" + tr( "Distance between the left margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding") + "</qt>");
	bottomMarginSpinBox->setToolTip( "<qt>" + tr( "Distance between the right margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding") + "</qt>");
}

void NewMarginWidget::setNewValues(const MarginStruct& margs)
{
	marginData=savedMarginData=margs;
	updateMarginSpinValues();
}

void NewMarginWidget::setPageWidth(double newWidth)
{
	leftMarginSpinBox->setMaximum(qMax(0.0, newWidth * m_unitRatio - leftMarginSpinBox->value()));
	rightMarginSpinBox->setMaximum(qMax(0.0,newWidth * m_unitRatio - rightMarginSpinBox->value()));
	pageWidth = newWidth;
	setPreset();
}

void NewMarginWidget::setPageHeight(double newHeight)
{
	topMarginSpinBox->setMaximum(qMax(0.0, newHeight * m_unitRatio - bottomMarginSpinBox->value()));
	bottomMarginSpinBox->setMaximum(qMax(0.0,newHeight * m_unitRatio - topMarginSpinBox->value()));
	pageHeight = newHeight;
	setPreset();
}

void NewMarginWidget::setTop()
{
	double newVal=topMarginSpinBox->value() / m_unitRatio;
	bottomMarginSpinBox->setMaximum(qMax(0.0, pageHeight * m_unitRatio - topMarginSpinBox->value()));
	if (marginLinkCheckBox->isChecked() && savedPresetItem==PresetLayout::none)
	{
		marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		marginData.Top = newVal;
	setPreset();
}

void NewMarginWidget::setBottom()
{
	double newVal = bottomMarginSpinBox->value() / m_unitRatio;
	topMarginSpinBox->setMaximum(qMax(0.0, pageHeight * m_unitRatio - bottomMarginSpinBox->value()));
	if (marginLinkCheckBox->isChecked() && savedPresetItem==PresetLayout::none)
	{
		marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		marginData.Bottom = newVal;
	setPreset();
}

void NewMarginWidget::setLeft()
{
	double newVal = leftMarginSpinBox->value() / m_unitRatio;
	rightMarginSpinBox->setMaximum(qMax(0.0, pageWidth * m_unitRatio - leftMarginSpinBox->value()));
	if (marginLinkCheckBox->isChecked() && savedPresetItem==PresetLayout::none)
	{
		marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		marginData.Left = newVal;
	setPreset();
}

void NewMarginWidget::setRight()
{
	double newVal = rightMarginSpinBox->value() / m_unitRatio;
	leftMarginSpinBox->setMaximum(qMax(0.0, pageWidth * m_unitRatio - rightMarginSpinBox->value()));
	if (marginLinkCheckBox->isChecked() && savedPresetItem==PresetLayout::none)
	{
		marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		marginData.Right = newVal;
	setPreset();
}

void NewMarginWidget::setNewUnitIndex(int newUnitIndex)
{
	leftMarginSpinBox->blockSignals(true);
	rightMarginSpinBox->blockSignals(true);
	topMarginSpinBox->blockSignals(true);
	bottomMarginSpinBox->blockSignals(true);
	m_unitIndex=newUnitIndex;
	m_unitRatio=unitGetRatioFromIndex(newUnitIndex);
	topMarginSpinBox->setNewUnit(newUnitIndex);
	bottomMarginSpinBox->setNewUnit(newUnitIndex);
	leftMarginSpinBox->setNewUnit(newUnitIndex);
	rightMarginSpinBox->setNewUnit(newUnitIndex);
	leftMarginSpinBox->blockSignals(false);
	rightMarginSpinBox->blockSignals(false);
	topMarginSpinBox->blockSignals(false);
	bottomMarginSpinBox->blockSignals(false);
}

void NewMarginWidget::setPreset()
{
	if (!usingPreset)
		return;
	leftMarginSpinBox->blockSignals(true);
	rightMarginSpinBox->blockSignals(true);
	topMarginSpinBox->blockSignals(true);
	bottomMarginSpinBox->blockSignals(true);
	if (savedPresetItem==PresetLayout::none)
		savedMarginData=marginData;
	int item = presetLayoutComboBox->currentIndex();

	MarginStruct marg = presetLayoutComboBox->getMargins(item, pageWidth, pageHeight, leftMarginSpinBox->value() / m_unitRatio);
	facingPages ? presetLayoutComboBox->setEnabled(true) : presetLayoutComboBox->setEnabled(false);
	bool restoringValues=false;
	if (item==PresetLayout::none && savedPresetItem!=PresetLayout::none)
	{
		marg=savedMarginData;
		restoringValues=true;
	}
	if (restoringValues || (presetLayoutComboBox->needUpdate() && facingPages))
	{
		marginData.set(qMax(0.0, marg.Top), qMax(0.0, marg.Left), qMax(0.0, marg.Bottom), qMax(0.0, marg.Right));
		updateMarginSpinValues();

		bottomMarginSpinBox->setMaximum(qMax(0.0, pageHeight * m_unitRatio - topMarginSpinBox->value()));
		topMarginSpinBox->setMaximum(qMax(0.0, pageHeight * m_unitRatio - bottomMarginSpinBox->value()));
		rightMarginSpinBox->setMaximum(qMax(0.0, pageWidth * m_unitRatio - leftMarginSpinBox->value()));
		leftMarginSpinBox->setMaximum(qMax(0.0, pageWidth * m_unitRatio - rightMarginSpinBox->value()));
		rightMarginSpinBox->setEnabled(restoringValues);
		topMarginSpinBox->setEnabled(restoringValues);
		bottomMarginSpinBox->setEnabled(restoringValues);
	}
	else
	{
		rightMarginSpinBox->setEnabled(true);
		topMarginSpinBox->setEnabled(true);
		bottomMarginSpinBox->setEnabled(true);
	}
	if (pageType == 1)
		rightMarginSpinBox->setEnabled(false);
	leftMarginSpinBox->setEnabled(item != PresetLayout::nineparts);
	if (item!=PresetLayout::none)
		marginLinkCheckBox->setChecked(false);
	marginLinkCheckBox->setEnabled(item==PresetLayout::none || !presetLayoutComboBox->isEnabled());
	leftMarginSpinBox->blockSignals(false);
	rightMarginSpinBox->blockSignals(false);
	topMarginSpinBox->blockSignals(false);
	bottomMarginSpinBox->blockSignals(false);
	savedPresetItem=item;
}

void NewMarginWidget::setPageSize(const QString& pageSize)
{
	m_pageSize=pageSize;
}


void NewMarginWidget::updateMarginSpinValues()
{
	leftMarginSpinBox->blockSignals(true);
	rightMarginSpinBox->blockSignals(true);
	topMarginSpinBox->blockSignals(true);
	bottomMarginSpinBox->blockSignals(true);
	topMarginSpinBox->setValue(marginData.Top * m_unitRatio);
	rightMarginSpinBox->setValue(marginData.Right * m_unitRatio);
	bottomMarginSpinBox->setValue(marginData.Bottom * m_unitRatio);
	leftMarginSpinBox->setValue(marginData.Left * m_unitRatio);
	leftMarginSpinBox->blockSignals(false);
	rightMarginSpinBox->blockSignals(false);
	topMarginSpinBox->blockSignals(false);
	bottomMarginSpinBox->blockSignals(false);
}

void NewMarginWidget::slotLinkMargins()
{
	leftMarginSpinBox->blockSignals(true);
	rightMarginSpinBox->blockSignals(true);
	topMarginSpinBox->blockSignals(true);
	bottomMarginSpinBox->blockSignals(true);
	if (marginLinkCheckBox->isChecked())
	{
		bottomMarginSpinBox->setValue(leftMarginSpinBox->value());
		topMarginSpinBox->setValue(leftMarginSpinBox->value());
		rightMarginSpinBox->setValue(leftMarginSpinBox->value());
		double newVal=leftMarginSpinBox->value() / m_unitRatio;
		marginData.set(newVal, newVal, newVal, newVal);
	}
	leftMarginSpinBox->blockSignals(false);
	rightMarginSpinBox->blockSignals(false);
	topMarginSpinBox->blockSignals(false);
	bottomMarginSpinBox->blockSignals(false);
}

void NewMarginWidget::setMarginPreset(int p)
{
	if (!usingPreset)
		return;
	presetLayoutComboBox->blockSignals(true);
	savedPresetItem = p;
	presetLayoutComboBox->setCurrentIndex(p);
	if (savedPresetItem==PresetLayout::none)
		savedMarginData=marginData;
	int item = presetLayoutComboBox->currentIndex();
	facingPages ? presetLayoutComboBox->setEnabled(true) : presetLayoutComboBox->setEnabled(false);
	bool restoringValues=false;
	if (item==PresetLayout::none && savedPresetItem!=PresetLayout::none)
	{
		restoringValues=true;
	}
	if (restoringValues || (presetLayoutComboBox->needUpdate() && facingPages))
	{
		rightMarginSpinBox->setEnabled(restoringValues);
		topMarginSpinBox->setEnabled(restoringValues);
		bottomMarginSpinBox->setEnabled(restoringValues);
	}
	else
	{
		rightMarginSpinBox->setEnabled(true);
		topMarginSpinBox->setEnabled(true);
		bottomMarginSpinBox->setEnabled(true);
	}
	if (pageType == 1)
		rightMarginSpinBox->setEnabled(false);
	leftMarginSpinBox->setEnabled(item != PresetLayout::nineparts);
	if (item!=PresetLayout::none)
		marginLinkCheckBox->setChecked(false);
	marginLinkCheckBox->setEnabled(item==PresetLayout::none);
	presetLayoutComboBox->blockSignals(false);
}

void NewMarginWidget::setFacingPages(bool facing, int pagetype)
{
	facingPages = facing;
	pageType = pagetype;
	leftMarginLabel->setText(facing == true ? tr( "&Inside:" ) : tr( "&Left:" ));
	rightMarginLabel->setText(facing == true ? tr( "O&utside:" ) : tr( "&Right:" ));
	setPreset();
}

void NewMarginWidget::setMarginsToPrinterMargins()
{
	UsePrinterMarginsDialog upm(parentWidget(), m_pageSize, unitGetRatioFromIndex(m_unitIndex), unitGetSuffixFromIndex(m_unitIndex));
	if (upm.exec())
	{
		double t,b,l,r;
		upm.getNewPrinterMargins(t,b,l,r);
		presetLayoutComboBox->setCurrentIndex(PresetLayout::none);
		marginData.set(t,l,b,r);

		updateMarginSpinValues();

		bottomMarginSpinBox->setMaximum((qMax(0.0, pageHeight - t) * m_unitRatio));
		topMarginSpinBox->setMaximum((qMax(0.0, pageHeight - b) * m_unitRatio));
		rightMarginSpinBox->setMaximum((qMax(0.0, pageWidth - l) * m_unitRatio));
		leftMarginSpinBox->setMaximum((qMax(0.0, pageWidth - r) * m_unitRatio));

		rightMarginSpinBox->setEnabled(true);
		topMarginSpinBox->setEnabled(true);
		bottomMarginSpinBox->setEnabled(true);
	}
}

