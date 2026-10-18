/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "newmarginwidget.h"
#include "iconmanager.h"
#include "manager/pagepreset_manager.h"
#include "prefsmanager.h"
#include "scribusapp.h"
#include "scrspinbox.h"
#include "ui/marginpresetlayout.h"
#include "ui/useprintermarginsdialog.h"
#include "units.h"

NewMarginWidget::NewMarginWidget(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
}

void NewMarginWidget::setup(const MarginStruct& margs, int layoutType, int unitIndex, int flags)
{
	m_unitIndex = unitIndex;
	m_unitRatio = unitGetRatioFromIndex(unitIndex);
	m_flags = flags;
	m_marginData = margs;
	m_savedMarginData = margs;

	leftMarginSpinBox->init(unitIndex);
	rightMarginSpinBox->init(unitIndex);
	topMarginSpinBox->init(unitIndex);
	bottomMarginSpinBox->init(unitIndex);

	if (!(m_flags & ShowPreset))
	{
		presetLayoutComboBox->hide();
		presetLayoutLabel->hide();
	}

	if (!(m_flags & ShowPrinterMargins))
	{
		printerMarginsPushButton->hide();
		defaultMarginsPushButton->hide();
	}

	setFacingPages(!(layoutType == singlePage));

	disconnect(topMarginSpinBox, &ScrSpinBox::valueChanged, this, &NewMarginWidget::onSpinBoxChanged);
	disconnect(bottomMarginSpinBox, &ScrSpinBox::valueChanged, this, &NewMarginWidget::onSpinBoxChanged);
	disconnect(leftMarginSpinBox, &ScrSpinBox::valueChanged, this, &NewMarginWidget::onSpinBoxChanged);
	disconnect(rightMarginSpinBox, &ScrSpinBox::valueChanged, this, &NewMarginWidget::onSpinBoxChanged);

	updateUIFromState();

	connect(ScQApp, &ScribusQApp::iconSetChanged, this, &NewMarginWidget::iconSetChange);
	connect(topMarginSpinBox, &ScrSpinBox::valueChanged, this, &NewMarginWidget::onSpinBoxChanged);
	connect(bottomMarginSpinBox, &ScrSpinBox::valueChanged, this, &NewMarginWidget::onSpinBoxChanged);
	connect(leftMarginSpinBox, &ScrSpinBox::valueChanged, this, &NewMarginWidget::onSpinBoxChanged);
	connect(rightMarginSpinBox, &ScrSpinBox::valueChanged, this, &NewMarginWidget::onSpinBoxChanged);
	connect(presetLayoutComboBox, &QComboBox::activated, this, &NewMarginWidget::setPreset);
	connect(marginLinkButton, &QToolButton::clicked, this, &NewMarginWidget::onLinkMarginsClicked);
	connect(printerMarginsPushButton, &QPushButton::clicked, this, &NewMarginWidget::onPrinterMarginsClicked);
	connect(defaultMarginsPushButton, &QPushButton::clicked, this, &NewMarginWidget::onDefaultMarginsClicked);

	languageChange();
	iconSetChange();
	toggleLabelVisibility(true);
}

void NewMarginWidget::blockSpinBoxSignals(bool block)
{
	leftMarginSpinBox->blockSignals(block);
	rightMarginSpinBox->blockSignals(block);
	topMarginSpinBox->blockSignals(block);
	bottomMarginSpinBox->blockSignals(block);
	presetLayoutComboBox->blockSignals(block);
	marginLinkButton->blockSignals(block);
}

MarginStruct NewMarginWidget::calculateMarginsForPreset(int preset, double w, double h, double refLeft) const
{
	if (preset != PresetLayout::none && (m_flags & ShowPreset))
		return presetLayoutComboBox->getMargins(preset, w, h, refLeft);

	return m_marginData;
}

MarginStruct NewMarginWidget::sanitizeMargins(const MarginStruct& margins, double w, double h) const
{
	MarginStruct safe = margins;
	bool validW = w > 0.001;
	bool validH = h > 0.001;

	if (!validW && !validH)
		return safe;

	if (marginLinkButton->isChecked() && !m_facingPages)
	{
		double limit = (validW && validH) ? qMin(w, h) : (validW ? w : h);
		double maxVal = limit / 2.0;
		double val = qMin(safe.left(), maxVal);
		safe.set(val, val, val, val);
		return safe;
	}

	auto adjustDimension = [](double val1, double val2, double maxDim, double& out1, double& out2)
	{
		if (val1 + val2 > maxDim)
		{
			if (val1 > maxDim)
			{
				out1 = maxDim;
				out2 = 0.0;
			}
			else
			{
				out1 = val1;
				out2 = maxDim - val1;
			}
		}
		else
		{
			out1 = val1;
			out2 = val2;
		}
	};

	double l = safe.left(), r = safe.right(), t = safe.top(), b = safe.bottom();
	if (validW)
		adjustDimension(l, r, w, l, r);
	if (validH)
		adjustDimension(t, b, h, t, b);
	safe.set(t, l, b, r);

	return safe;
}

void NewMarginWidget::setPageWidth(double newWidth)
{
	MarginStruct targetMargins = m_marginData;

	if ((m_flags & ShowPreset) && m_savedPresetItem != PresetLayout::none)
	{
		double currentLeft = leftMarginSpinBox->value() / m_unitRatio;
		targetMargins = calculateMarginsForPreset(m_savedPresetItem, newWidth, m_pageHeight, currentLeft);
	}

	setInternalState(newWidth, m_pageHeight, targetMargins, m_savedPresetItem);
}

void NewMarginWidget::setPageHeight(double newHeight)
{
	MarginStruct targetMargins = m_marginData;

	if ((m_flags & ShowPreset) && m_savedPresetItem != PresetLayout::none)
	{
		double currentLeft = leftMarginSpinBox->value() / m_unitRatio;
		targetMargins = calculateMarginsForPreset(m_savedPresetItem, m_pageWidth, newHeight, currentLeft);
	}

	setInternalState(m_pageWidth, newHeight, targetMargins, m_savedPresetItem);
}

void NewMarginWidget::setNewValues(const MarginStruct& margs)
{
	setInternalState(m_pageWidth, m_pageHeight, margs, m_savedPresetItem);
}

void NewMarginWidget::setNewUnit(int unitIndex)
{
	m_unitIndex = unitIndex;
	m_unitRatio = unitGetRatioFromIndex(unitIndex);

	blockSpinBoxSignals(true);
	leftMarginSpinBox->setNewUnit(unitIndex);
	rightMarginSpinBox->setNewUnit(unitIndex);
	topMarginSpinBox->setNewUnit(unitIndex);
	bottomMarginSpinBox->setNewUnit(unitIndex);
	blockSpinBoxSignals(false);

	setInternalState(m_pageWidth, m_pageHeight, m_marginData, m_savedPresetItem);
}

void NewMarginWidget::setMarginPreset(int p)
{
	if (!(m_flags & ShowPreset))
		return;

	MarginStruct targetMargins = m_marginData;

	if (p == PresetLayout::none || m_facingPages == false)
	{
		if (m_savedPresetItem != PresetLayout::none)
			targetMargins = m_savedMarginData;
	}
	else
	{
		if (m_savedPresetItem == PresetLayout::none)
			m_savedMarginData = m_marginData;

		double currentLeft = leftMarginSpinBox->value() / m_unitRatio;
		targetMargins = calculateMarginsForPreset(p, m_pageWidth, m_pageHeight, currentLeft);
	}

	setInternalState(m_pageWidth, m_pageHeight, targetMargins, p);
}

void NewMarginWidget::setFacingPages(bool facing, int pageType)
{
	m_facingPages = facing;
	m_pageType = pageType;

	iconSetChange();
	languageChange();

	if (m_flags & ShowPreset)
	{
		presetLayoutComboBox->setEnabled(m_facingPages);

		if (m_facingPages)
			setPreset();
		else
			setInternalState(m_pageWidth, m_pageHeight, m_marginData, m_savedPresetItem);
	}
}

void NewMarginWidget::setInternalState(double width, double height, const MarginStruct& margins, int presetIndex)
{
	blockSpinBoxSignals(true);

	m_pageWidth = width;
	m_pageHeight = height;
	m_marginData = sanitizeMargins(margins, width, height);
	m_savedPresetItem = presetIndex;

	bool noPreset = presetIndex == PresetLayout::none || m_facingPages == false;

	if (noPreset)
		m_savedMarginData = margins;

	if (m_flags & ShowPreset)
	{
		presetLayoutComboBox->setCurrentIndex(presetIndex);
		marginLinkButton->setEnabled(noPreset);
		if (presetIndex != PresetLayout::none && m_facingPages)
			marginLinkButton->setChecked(false);
	}

	updateUIFromState();
	updateSpinBoxLimits();

	blockSpinBoxSignals(false);

	emit valuesChanged(m_marginData);
}

void NewMarginWidget::updateUIFromState()
{
	double maxVal = qMax(m_pageWidth, m_pageHeight) * m_unitRatio;
	if (maxVal <= 0) maxVal = 99999;

	leftMarginSpinBox->setMaximum(maxVal);
	rightMarginSpinBox->setMaximum(maxVal);
	topMarginSpinBox->setMaximum(maxVal);
	bottomMarginSpinBox->setMaximum(maxVal);

	leftMarginSpinBox->setValue(m_marginData.left() * m_unitRatio);
	topMarginSpinBox->setValue(m_marginData.top() * m_unitRatio);
	rightMarginSpinBox->setValue(m_marginData.right() * m_unitRatio);
	bottomMarginSpinBox->setValue(m_marginData.bottom() * m_unitRatio);

	bool hasPreset = (m_savedPresetItem != PresetLayout::none) && (m_flags & ShowPreset) && (m_facingPages == true);
	bool isNineParts = (m_savedPresetItem == PresetLayout::nineparts) && (m_facingPages == true);

	leftMarginSpinBox->setEnabled(!(hasPreset && isNineParts));
	topMarginSpinBox->setEnabled(!hasPreset);
	bottomMarginSpinBox->setEnabled(!hasPreset);
	rightMarginSpinBox->setEnabled(!hasPreset);

	marginLinkButton->setEnabled(!hasPreset);
	if (hasPreset) marginLinkButton->setChecked(false);
}

void NewMarginWidget::updateSpinBoxLimits()
{
	double h = m_pageHeight * m_unitRatio;
	double w = m_pageWidth * m_unitRatio;

	bool validW = w > 0.001;
	bool validH = h > 0.001;

	double fallbackMax = 99999.0;

	if (marginLinkButton->isChecked())
	{
		double limit = fallbackMax;
		if (validW && validH)
			limit = qMin(w, h);
		else if (validW)
			limit = w;
		else if (validH)
			limit = h;

		double maxAllowed = (validW || validH) ? (limit / 2.0) : fallbackMax;

		topMarginSpinBox->setMaximum(maxAllowed);
		bottomMarginSpinBox->setMaximum(maxAllowed);
		leftMarginSpinBox->setMaximum(maxAllowed);
		rightMarginSpinBox->setMaximum(maxAllowed);
	}
	else
	{
		if (validH)
		{
			topMarginSpinBox->setMaximum(qMax(0.0, h - bottomMarginSpinBox->value()));
			bottomMarginSpinBox->setMaximum(qMax(0.0, h - topMarginSpinBox->value()));
		}
		else
		{
			topMarginSpinBox->setMaximum(fallbackMax);
			bottomMarginSpinBox->setMaximum(fallbackMax);
		}

		if (validW)
		{
			leftMarginSpinBox->setMaximum(qMax(0.0, w - rightMarginSpinBox->value()));
			rightMarginSpinBox->setMaximum(qMax(0.0, w - leftMarginSpinBox->value()));
		}
		else
		{
			leftMarginSpinBox->setMaximum(fallbackMax);
			rightMarginSpinBox->setMaximum(fallbackMax);
		}
	}
}

void NewMarginWidget::setPreset()
{
	setMarginPreset(presetLayoutComboBox->currentIndex());
}

void NewMarginWidget::onSpinBoxChanged()
{
	double val = 0.0;

	QObject* s = sender();
	if (s == topMarginSpinBox) val = topMarginSpinBox->value();
	else if (s == bottomMarginSpinBox) val = bottomMarginSpinBox->value();
	else if (s == rightMarginSpinBox) val = rightMarginSpinBox->value();
	else val = leftMarginSpinBox->value();

	double valInPoints = val / m_unitRatio;

	if ((m_flags & ShowPreset) && m_savedPresetItem != PresetLayout::none && m_facingPages)
	{
		MarginStruct newMargins = calculateMarginsForPreset(m_savedPresetItem, m_pageWidth, m_pageHeight, valInPoints);
		setInternalState(m_pageWidth, m_pageHeight, newMargins, m_savedPresetItem);
	}
	else
	{
		MarginStruct newM;

		if (marginLinkButton->isChecked())
		{
			double maxAllowed = qMin(m_pageWidth, m_pageHeight) / 2.0;
			valInPoints = qMin(maxAllowed, valInPoints);

			newM.set(valInPoints, valInPoints, valInPoints, valInPoints);
		}
		else
		{
			double t = topMarginSpinBox->value() / m_unitRatio;
			double b = bottomMarginSpinBox->value() / m_unitRatio;
			double l = leftMarginSpinBox->value() / m_unitRatio;
			double r = rightMarginSpinBox->value() / m_unitRatio;

			if (s == topMarginSpinBox) t = valInPoints;
			else if (s == bottomMarginSpinBox) b = valInPoints;
			else if (s == leftMarginSpinBox) l = valInPoints;
			else if (s == rightMarginSpinBox) r = valInPoints;

			newM.set(t, l, b, r);
		}

		m_savedMarginData = newM;
		setInternalState(m_pageWidth, m_pageHeight, newM, m_savedPresetItem);
	}
}

void NewMarginWidget::onLinkMarginsClicked()
{
	if (marginLinkButton->isChecked())
	{

		double maxAllowed = qMin(m_pageWidth, m_pageHeight) / 2.0;
		double val = qMin(maxAllowed, leftMarginSpinBox->value() / m_unitRatio);

		MarginStruct newM(val, val, val, val);
		setInternalState(m_pageWidth, m_pageHeight, newM, m_savedPresetItem);
	}
	else
		updateSpinBoxLimits();

}

void NewMarginWidget::languageChange()
{
	if (m_flags & MarginWidgetFlags)
	{
		topMarginSpinBox->setToolTip("<qt>" + tr("Distance between the top margin guide and the edge of the page") + "</qt>");
		bottomMarginSpinBox->setToolTip("<qt>" + tr("Distance between the bottom margin guide and the edge of the page") + "</qt>");
		leftMarginSpinBox->setToolTip("<qt>" + tr("Distance between the left margin guide and the edge of the page. If a double-sided layout is selected, this margin space can be used to achieve the correct margins for binding.") + "</qt>");
		rightMarginSpinBox->setToolTip("<qt>" + tr("Distance between the right margin guide and the edge of the page. If a double-sided layout is selected, this margin space can be used to achieve the correct margins for binding.") + "</qt>");
		marginLinkButton->setToolTip("<qt>" + tr("Ensure all margins have the same value") + "</qt>");
	}
	else if (m_flags & BleedWidgetFlags)
	{
		topMarginSpinBox->setToolTip("<qt>" + tr("Distance for bleed from the top of the physical page") + "</qt>");
		bottomMarginSpinBox->setToolTip("<qt>" + tr("Distance for bleed from the bottom of the physical page") + "</qt>");
		leftMarginSpinBox->setToolTip("<qt>" + tr("Distance for bleed from the left of the physical page") + "</qt>");
		rightMarginSpinBox->setToolTip("<qt>" + tr("Distance for bleed from the right of the physical page") + "</qt>");
		marginLinkButton->setToolTip("<qt>" + tr("Ensure all bleeds have the same value") + "</qt>");
	}
	else
	{
		topMarginSpinBox->setToolTip("<qt>" + tr("Distance from the top") + "</qt>");
		bottomMarginSpinBox->setToolTip("<qt>" + tr("Distance from the bottom") + "</qt>");
		leftMarginSpinBox->setToolTip("<qt>" + tr("Distance from the left") + "</qt>");
		rightMarginSpinBox->setToolTip("<qt>" + tr("Distance from the right") + "</qt>");
		marginLinkButton->setToolTip("<qt>" + tr("Ensure all distances have the same value") + "</qt>");
	}
	printerMarginsPushButton->setToolTip("<qt>" + tr("Import the margins for the selected page size from the available printers") + "</qt>");
	defaultMarginsPushButton->setToolTip("<qt>" + tr("Set the margins to the default for the active unit") + "</qt>");

	leftMarginLabel->setText(m_facingPages ? tr("Inside") : tr("Left"));
	rightMarginLabel->setText(m_facingPages ? tr("Outside") : tr("Right"));
}

void NewMarginWidget::iconSetChange()
{
	IconManager& im = IconManager::instance();

	leftMarginLabel->setPixmap(im.loadPixmap(m_facingPages ? "border-inside" : "border-left"));
	rightMarginLabel->setPixmap(im.loadPixmap(m_facingPages ? "border-outside" : "border-right"));
	topMarginLabel->setPixmap(im.loadPixmap("border-top"));
	bottomMarginLabel->setPixmap(im.loadPixmap("border-bottom"));
}

void NewMarginWidget::toggleLabelVisibility(bool v)
{
	formWidget->setLabelVisibility(v);
	leftMarginLabel->setLabelVisibility(v);
	rightMarginLabel->setLabelVisibility(v);
	topMarginLabel->setLabelVisibility(v);
	bottomMarginLabel->setLabelVisibility(v);
}

void NewMarginWidget::onPrinterMarginsClicked()
{
	QSizeF pageDimensions(m_pageWidth, m_pageHeight);
	PageSizeInfo psi = PagePresetManager::instance().pageInfoByDimensions(m_pageWidth, m_pageHeight);

	UsePrinterMarginsDialog upm(this, pageDimensions, psi.displayName, m_unitRatio, unitGetSuffixFromIndex(m_unitIndex));

	if (upm.exec() == QDialog::Accepted)
	{
		double t, b, l, r;
		upm.getNewPrinterMargins(t, b, l, r);
		MarginStruct newMargins(t, l, b, r);
		setInternalState(m_pageWidth, m_pageHeight, newMargins, PresetLayout::none);
	}
}

void NewMarginWidget::onDefaultMarginsClicked()
{
	setInternalState(m_pageWidth, m_pageHeight, PrefsManager::defaultPageMarginsForUnit(m_unitIndex), PresetLayout::none);
}
