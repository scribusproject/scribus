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

void NewMarginWidget::setup(const MarginStruct& margs, int layoutType, int unitIndex, int flags)
{
	m_marginData = m_savedMarginData = margs;
	m_unitIndex = unitIndex;
	m_unitRatio = unitGetRatioFromIndex(unitIndex);
	m_flags = flags;
	leftMarginSpinBox->setMaximum(1000);
	rightMarginSpinBox->setMaximum(1000);
	topMarginSpinBox->setMaximum(1000);
	bottomMarginSpinBox->setMaximum(1000);
	leftMarginSpinBox->init(unitIndex);
	rightMarginSpinBox->init(unitIndex);
	topMarginSpinBox->init(unitIndex);
	bottomMarginSpinBox->init(unitIndex);
	updateMarginSpinValues();
	if ((m_flags & ShowPreset) == 0)
	{
		presetLayoutComboBox->blockSignals(true);
		presetLayoutComboBox->resize(0,0);
		presetLayoutLabel->resize(0,0);
		presetLayoutComboBox->hide();
		presetLayoutLabel->hide();
		gridLayout->removeWidget(presetLayoutComboBox);
		gridLayout->removeWidget(presetLayoutLabel);
	}
	if ((m_flags & ShowPrinterMargins) == 0)
	{
		printerMarginsPushButton->blockSignals(true);
		printerMarginsPushButton->resize(0,0);
		printerMarginsPushButton->hide();
		gridLayout->removeWidget(printerMarginsPushButton);
	}
	setFacingPages(!(layoutType == singlePage));

	gridLayout->invalidate();

	languageChange();

	connect(topMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	connect(bottomMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	connect(leftMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	connect(rightMarginSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
	connect(presetLayoutComboBox, SIGNAL(activated(int)), this, SLOT(setPreset()));
	connect(marginLinkButton, SIGNAL(clicked()), this, SLOT(slotLinkMargins()));
	connect(printerMarginsPushButton, SIGNAL(clicked()), this, SLOT(setMarginsToPrinterMargins()));
}

void NewMarginWidget::languageChange()
{
	if (m_flags & MarginWidgetFlags)
	{
		topMarginSpinBox->setToolTip( "<qt>" + tr( "Distance between the top margin guide and the edge of the page" ) + "</qt>");
		bottomMarginSpinBox->setToolTip( "<qt>" + tr( "Distance between the bottom margin guide and the edge of the page" ) + "</qt>");
		leftMarginSpinBox->setToolTip( "<qt>" + tr( "Distance between the left margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding.") + "</qt>");
		rightMarginSpinBox->setToolTip( "<qt>" + tr( "Distance between the right margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding.") + "</qt>");
		marginLinkButton->setToolTip( "<qt>" + tr( "Ensure all margins have the same value" ) + "</qt>");
	}
	else
	{
		topMarginSpinBox->setToolTip( "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
		bottomMarginSpinBox->setToolTip( "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
		leftMarginSpinBox->setToolTip( "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
		rightMarginSpinBox->setToolTip( "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
		marginLinkButton->setToolTip( "<qt>" + tr( "Ensure all bleeds have the same value" ) + "</qt>");
	}
	printerMarginsPushButton->setToolTip( "<qt>" + tr( "Import the margins for the selected page size from the available printers" ) + "</qt>");
}

void NewMarginWidget::setNewValues(const MarginStruct& margs)
{
	m_marginData = m_savedMarginData = margs;
	updateMarginSpinValues();
}

void NewMarginWidget::setPageWidth(double newWidth)
{
	leftMarginSpinBox->setMaximum(qMax(0.0, newWidth * m_unitRatio - rightMarginSpinBox->value()));
	rightMarginSpinBox->setMaximum(qMax(0.0, newWidth * m_unitRatio - leftMarginSpinBox->value()));
	m_pageWidth = newWidth;
	setPreset();
}

void NewMarginWidget::setPageHeight(double newHeight)
{
	topMarginSpinBox->setMaximum(qMax(0.0, newHeight * m_unitRatio - bottomMarginSpinBox->value()));
	bottomMarginSpinBox->setMaximum(qMax(0.0,newHeight * m_unitRatio - topMarginSpinBox->value()));
	m_pageHeight = newHeight;
	setPreset();
}

void NewMarginWidget::setTop()
{
	double newVal = topMarginSpinBox->value() / m_unitRatio;
	bottomMarginSpinBox->setMaximum(qMax(0.0, m_pageHeight * m_unitRatio - topMarginSpinBox->value()));
	if (marginLinkButton->isChecked() && m_savedPresetItem == PresetLayout::none)
	{
		m_marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		m_marginData.setTop(newVal);
	setPreset();
}

void NewMarginWidget::setBottom()
{
	double newVal = bottomMarginSpinBox->value() / m_unitRatio;
	topMarginSpinBox->setMaximum(qMax(0.0, m_pageHeight * m_unitRatio - bottomMarginSpinBox->value()));
	if (marginLinkButton->isChecked() && m_savedPresetItem == PresetLayout::none)
	{
		m_marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		m_marginData.setBottom(newVal);
	setPreset();
}

void NewMarginWidget::setLeft()
{
	double newVal = leftMarginSpinBox->value() / m_unitRatio;
	rightMarginSpinBox->setMaximum(qMax(0.0, m_pageWidth * m_unitRatio - leftMarginSpinBox->value()));
	if (marginLinkButton->isChecked() && m_savedPresetItem == PresetLayout::none)
	{
		m_marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		m_marginData.setLeft(newVal);
	setPreset();
}

void NewMarginWidget::setRight()
{
	double newVal = rightMarginSpinBox->value() / m_unitRatio;
	leftMarginSpinBox->setMaximum(qMax(0.0, m_pageWidth * m_unitRatio - rightMarginSpinBox->value()));
	if (marginLinkButton->isChecked() && m_savedPresetItem == PresetLayout::none)
	{
		m_marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		m_marginData.setRight(newVal);
	setPreset();
}

void NewMarginWidget::setNewUnit(int newUnitIndex)
{
	bool leftSigBlocked   = leftMarginSpinBox->blockSignals(true);
	bool rightSigBlocked  = rightMarginSpinBox->blockSignals(true);
	bool topSigBlocked    = topMarginSpinBox->blockSignals(true);
	bool bottomSigBlocked = bottomMarginSpinBox->blockSignals(true);

	m_unitIndex = newUnitIndex;
	m_unitRatio = unitGetRatioFromIndex(newUnitIndex);
	topMarginSpinBox->setNewUnit(newUnitIndex);
	bottomMarginSpinBox->setNewUnit(newUnitIndex);
	leftMarginSpinBox->setNewUnit(newUnitIndex);
	rightMarginSpinBox->setNewUnit(newUnitIndex);

	leftMarginSpinBox->blockSignals(leftSigBlocked);
	rightMarginSpinBox->blockSignals(rightSigBlocked);
	topMarginSpinBox->blockSignals(topSigBlocked);
	bottomMarginSpinBox->blockSignals(bottomSigBlocked);
}

void NewMarginWidget::setPreset()
{
	if ((m_flags & ShowPreset) == 0)
		return;
	leftMarginSpinBox->blockSignals(true);
	rightMarginSpinBox->blockSignals(true);
	topMarginSpinBox->blockSignals(true);
	bottomMarginSpinBox->blockSignals(true);
	if (m_savedPresetItem == PresetLayout::none)
		m_savedMarginData = m_marginData;
	int item = presetLayoutComboBox->currentIndex();

	MarginStruct marg = presetLayoutComboBox->getMargins(item, m_pageWidth, m_pageHeight, leftMarginSpinBox->value() / m_unitRatio);
	presetLayoutComboBox->setEnabled(m_facingPages);

	bool restoringValues = false;
	if (item == PresetLayout::none && m_savedPresetItem != PresetLayout::none)
	{
		marg = m_savedMarginData;
		restoringValues = true;
	}
	if (restoringValues || (presetLayoutComboBox->needUpdate() && m_facingPages))
	{
		m_marginData.set(qMax(0.0, marg.top()), qMax(0.0, marg.left()), qMax(0.0, marg.bottom()), qMax(0.0, marg.right()));
		updateMarginSpinValues();

		bottomMarginSpinBox->setMaximum(qMax(0.0, m_pageHeight * m_unitRatio - topMarginSpinBox->value()));
		topMarginSpinBox->setMaximum(qMax(0.0, m_pageHeight * m_unitRatio - bottomMarginSpinBox->value()));
		rightMarginSpinBox->setMaximum(qMax(0.0, m_pageWidth * m_unitRatio - leftMarginSpinBox->value()));
		leftMarginSpinBox->setMaximum(qMax(0.0, m_pageWidth * m_unitRatio - rightMarginSpinBox->value()));
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
	if (m_pageType == 1)
		rightMarginSpinBox->setEnabled(false);
	leftMarginSpinBox->setEnabled(item != PresetLayout::nineparts);
	if (item != PresetLayout::none)
		marginLinkButton->setChecked(false);
	marginLinkButton->setEnabled(item == PresetLayout::none || !presetLayoutComboBox->isEnabled());
	leftMarginSpinBox->blockSignals(false);
	rightMarginSpinBox->blockSignals(false);
	topMarginSpinBox->blockSignals(false);
	bottomMarginSpinBox->blockSignals(false);
	m_savedPresetItem = item;
}

void NewMarginWidget::setPageSize(const QString& pageSize)
{
	m_pageSize = pageSize;
}


void NewMarginWidget::updateMarginSpinValues()
{
	bool leftBlocked = leftMarginSpinBox->blockSignals(true);
	bool rightBlocked = rightMarginSpinBox->blockSignals(true);
	bool topBlocked = topMarginSpinBox->blockSignals(true);
	bool bottomBlocked = bottomMarginSpinBox->blockSignals(true);

	topMarginSpinBox->setValue(m_marginData.top() * m_unitRatio);
	rightMarginSpinBox->setValue(m_marginData.right() * m_unitRatio);
	bottomMarginSpinBox->setValue(m_marginData.bottom() * m_unitRatio);
	leftMarginSpinBox->setValue(m_marginData.left() * m_unitRatio);

	leftMarginSpinBox->blockSignals(leftBlocked);
	rightMarginSpinBox->blockSignals(rightBlocked);
	topMarginSpinBox->blockSignals(topBlocked);
	bottomMarginSpinBox->blockSignals(bottomBlocked);
}

void NewMarginWidget::slotLinkMargins()
{
	bool leftBlocked = leftMarginSpinBox->blockSignals(true);
	bool rightBlocked = rightMarginSpinBox->blockSignals(true);
	bool topBlocked = topMarginSpinBox->blockSignals(true);
	bool bottomBlocked = bottomMarginSpinBox->blockSignals(true);

	if (marginLinkButton->isChecked())
	{
		bottomMarginSpinBox->setValue(leftMarginSpinBox->value());
		topMarginSpinBox->setValue(leftMarginSpinBox->value());
		rightMarginSpinBox->setValue(leftMarginSpinBox->value());
		double newVal = leftMarginSpinBox->value() / m_unitRatio;
		m_marginData.set(newVal, newVal, newVal, newVal);
	}

	leftMarginSpinBox->blockSignals(leftBlocked);
	rightMarginSpinBox->blockSignals(rightBlocked);
	topMarginSpinBox->blockSignals(topBlocked);
	bottomMarginSpinBox->blockSignals(bottomBlocked);
}

void NewMarginWidget::setMarginPreset(int p)
{
	if ((m_flags & ShowPreset) == 0)
		return;
	presetLayoutComboBox->blockSignals(true);
	m_savedPresetItem = p;
	presetLayoutComboBox->setCurrentIndex(p);
	if (m_savedPresetItem == PresetLayout::none)
		m_savedMarginData = m_marginData;
	int item = presetLayoutComboBox->currentIndex();
	presetLayoutComboBox->setEnabled(m_facingPages);

	bool restoringValues = false;
	if ((item == PresetLayout::none) && (m_savedPresetItem != PresetLayout::none))
	{
		restoringValues = true;
	}
	if (restoringValues || (presetLayoutComboBox->needUpdate() && m_facingPages))
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
	if (m_pageType == 1)
		rightMarginSpinBox->setEnabled(false);
	leftMarginSpinBox->setEnabled(item != PresetLayout::nineparts);
	if (item != PresetLayout::none)
		marginLinkButton->setChecked(false);
	marginLinkButton->setEnabled(item == PresetLayout::none);
	presetLayoutComboBox->blockSignals(false);
}

void NewMarginWidget::setFacingPages(bool facing, int pageType)
{
	m_facingPages = facing;
	m_pageType = pageType;
	leftMarginLabel->setText(facing ? tr( "&Inside:" ) : tr( "&Left:" ));
	rightMarginLabel->setText(facing ? tr( "O&utside:" ) : tr( "&Right:" ));
	setPreset();
}

void NewMarginWidget::setMarginsToPrinterMargins()
{
	QSizeF pageDimensions(m_pageWidth, m_pageHeight);
	UsePrinterMarginsDialog upm(parentWidget(), pageDimensions, m_pageSize, unitGetRatioFromIndex(m_unitIndex), unitGetSuffixFromIndex(m_unitIndex));
	if (upm.exec() != QDialog::Accepted)
		return;

	double t, b, l, r;
	upm.getNewPrinterMargins(t, b, l, r);
	presetLayoutComboBox->setCurrentIndex(PresetLayout::none);
	m_marginData.set(t, l, b, r);

	updateMarginSpinValues();

	bottomMarginSpinBox->setMaximum((qMax(0.0, m_pageHeight - t) * m_unitRatio));
	topMarginSpinBox->setMaximum((qMax(0.0, m_pageHeight - b) * m_unitRatio));
	rightMarginSpinBox->setMaximum((qMax(0.0, m_pageWidth - l) * m_unitRatio));
	leftMarginSpinBox->setMaximum((qMax(0.0, m_pageWidth - r) * m_unitRatio));

	rightMarginSpinBox->setEnabled(true);
	topMarginSpinBox->setEnabled(true);
	bottomMarginSpinBox->setEnabled(true);
}

