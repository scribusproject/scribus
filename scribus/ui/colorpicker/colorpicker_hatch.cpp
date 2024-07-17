#include "colorpicker_hatch.h"
#include "units.h"
#include "util_gui.h"
#include "sccolorengine.h"
#include "sccolor.h"
#include "ui/colorlistbox.h"

ColorPickerHatch::ColorPickerHatch(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	listForegroundSwatches = new ColorListBox(ColorListBox::fancyPixmap);
	listBackgroundSwatches = new ColorListBox(ColorListBox::fancyPixmap);

	buttonLineColor->setContextWidget(listForegroundSwatches);
	buttonBackgroundColor->setContextWidget(listBackgroundSwatches);

	hatchAngle->setDecimals(0);
	hatchAngle->setNewUnit(6);
	hatchAngle->setValues(-360, 360, 1, 0);
	hatchDist->setDecimals(0);
	hatchDist->setNewUnit(0);
	hatchDist->setValues(1, 1000, 0, 1);

	connectSlots();
}

void ColorPickerHatch::connectSlots()
{
	connect(hatchAngle, SIGNAL(valueChanged(double)), this, SLOT(updateHatch()));
	connect(hatchDist, SIGNAL(valueChanged(double)), this, SLOT(updateHatch()));
	connect(hatchType, SIGNAL(activated(int)), this, SLOT(updateHatch()));
	connect(listForegroundSwatches, &ColorListBox::itemSelectionChanged, this, &ColorPickerHatch::updateHatch);
	connect(listBackgroundSwatches, &ColorListBox::itemSelectionChanged, this, &ColorPickerHatch::updateHatch);
}

void ColorPickerHatch::disconnectSlots()
{
	disconnect(hatchAngle, SIGNAL(valueChanged(double)), this, SLOT(updateHatch()));
	disconnect(hatchDist, SIGNAL(valueChanged(double)), this, SLOT(updateHatch()));
	disconnect(hatchType, SIGNAL(activated(int)), this, SLOT(updateHatch()));
	disconnect(listForegroundSwatches, &ColorListBox::itemSelectionChanged, this, &ColorPickerHatch::updateHatch);
	disconnect(listBackgroundSwatches, &ColorListBox::itemSelectionChanged, this, &ColorPickerHatch::updateHatch);
}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

Context ColorPickerHatch::context() const
{
    return m_context;
}

void ColorPickerHatch::setContext(Context config)
{
	m_context = config;
}

void ColorPickerHatch::setColorList(const ColorList &list)
{
	QSignalBlocker sig1(listForegroundSwatches);
	QSignalBlocker sig2(listBackgroundSwatches);

	colList = list;
	listForegroundSwatches->setColors(colList, false);
	listBackgroundSwatches->setColors(colList, true);
}

const CPHatchData& ColorPickerHatch::hatchData() const
{
	return m_hatch;
}

QString ColorPickerHatch::toolTipText() const
{
	return QString( tr("Hatch") );
}

void ColorPickerHatch::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
	unitChange();
}

void ColorPickerHatch::setHatchData(const CPHatchData& hatch)
{
	m_hatch = hatch;

	disconnectSlots();

	hatchAngle->setValue(m_hatch.Angle);
	hatchDist->setValue(m_hatch.Distance * unitGetRatioFromIndex(currentUnit));
	hatchType->setCurrentIndex(m_hatch.Type);

	if ((m_hatch.ColorForeground != CommonStrings::None) && (!m_hatch.ColorForeground.isEmpty()))
		listForegroundSwatches->setCurrentColor(m_hatch.ColorForeground);

	if (m_hatch.HasBackgroundColor)
	{
		if (!m_hatch.ColorBackground.isEmpty())
			listBackgroundSwatches->setCurrentColor(m_hatch.ColorBackground);
	}
	else
		listBackgroundSwatches->setCurrentColor(CommonStrings::None);

	buttonLineColor->setBackground( colorBrush(buttonLineColor->backgroundDotSize(), m_hatch.ColorForeground) );
	buttonLineColor->setToolTip(m_hatch.ColorForeground);
	buttonBackgroundColor->setBackground( colorBrush(buttonBackgroundColor->backgroundDotSize(), m_hatch.ColorBackground) );
	buttonBackgroundColor->setToolTip(m_hatch.ColorBackground);

	connectSlots();
}

/* ********************************************************************************* *
 *
 * Public Slots
 *
 * ********************************************************************************* */

void ColorPickerHatch::languageChange()
{

	int oldHatchTypeIndex = hatchType->currentIndex();
	QSignalBlocker sigType(hatchType);

	// Retranslate UI
	retranslateUi(this);

	hatchType->setCurrentIndex(oldHatchTypeIndex);
}

void ColorPickerHatch::unitChange()
{
	if (!m_doc)
		return;

	currentUnit = m_doc->unitIndex();
	hatchDist->setNewUnit(currentUnit);
}

/* ********************************************************************************* *
 *
 * Private Slots
 *
 * ********************************************************************************* */

void ColorPickerHatch::updateSize()
{
	int w = this->width();
	adjustSize();
	resize(w, sizeHint().height());
	emit sizeChanged();
}

void ColorPickerHatch::updateHatch()
{
	QString color1 = listForegroundSwatches->currentColor();
	if (color1 == CommonStrings::tr_NoneColor)
		color1 = CommonStrings::None;
	QString color2 = listBackgroundSwatches->currentColor();
	if (color2 == CommonStrings::tr_NoneColor)
		color2 = CommonStrings::None;

	m_hatch.ColorForeground = color1;
	m_hatch.ColorBackground = color2;
	m_hatch.HasBackgroundColor = (color2 != CommonStrings::None);
	m_hatch.Angle = hatchAngle->value();
	m_hatch.Distance = hatchDist->value() / unitGetRatioFromIndex(currentUnit);
	m_hatch.Type = hatchType->currentIndex();

	buttonLineColor->setBackground( colorBrush(buttonLineColor->backgroundDotSize(), m_hatch.ColorForeground) );
	buttonLineColor->setToolTip(m_hatch.ColorForeground);
	buttonBackgroundColor->setBackground( colorBrush(buttonBackgroundColor->backgroundDotSize(), m_hatch.ColorBackground) );
	buttonBackgroundColor->setToolTip(m_hatch.ColorBackground);

	emit hatchChanged();
}

QColor ColorPickerHatch::colorFromName(QString colorName, double shade) const
{
	if (!m_doc)
		return QColor();

	ScColor sColor(0, 0, 0);
	shade = qBound(0.0, shade, 100.0);

	if (m_doc->PageColors.contains(colorName))
		sColor = m_doc->PageColors.value(colorName);

	return ScColorEngine::getDisplayColor(sColor, m_doc, shade);
}

QBrush ColorPickerHatch::colorBrush(QSize size, QString colorName, double shade, double opacity) const
{
	if (!m_doc || colorName == CommonStrings::tr_NoneColor || colorName == CommonStrings::None)
		return renderEmptyPattern(size);

	return renderColor(size, colorFromName(colorName, 100.0), colorFromName(colorName, shade), opacity);
}



