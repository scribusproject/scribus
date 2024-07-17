/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "colorpicker_color.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

ColorPickerColor::ColorPickerColor(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	languageChange();

	connectSlots();
	connect(sectionSwatches,    &SectionContainer::sizeChanged,				this, &ColorPickerColor::updateSize);
}

void ColorPickerColor::connectSlots()
{
	connect(swatches,			&ColorPickerColorSwatches::colorChanged,	this, &ColorPickerColor::updateColorFromSwatches);	
	connect(numberShade,		&ScrSpinBox::valueChanged,					this, &ColorPickerColor::updateColorShade);
	connect(numberAlpha,		&ScrSpinBox::valueChanged,					this, &ColorPickerColor::updateColorAlpha);
}

void ColorPickerColor::disconnectSlots()
{
	disconnect(swatches,		&ColorPickerColorSwatches::colorChanged,	this, &ColorPickerColor::updateColorFromSwatches);
	disconnect(numberShade,		&ScrSpinBox::valueChanged,					this, &ColorPickerColor::updateColorShade);
	disconnect(numberAlpha,		&ScrSpinBox::valueChanged,					this, &ColorPickerColor::updateColorAlpha);
}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

void ColorPickerColor::setColorList(const ColorList &list, bool insertNone)
{
	QSignalBlocker sigSwatches(swatches);
	swatches->setColors(list, insertNone);
}

Context ColorPickerColor::context() const
{
	return m_context;
}

void ColorPickerColor::setContext(Context config)
{
	m_context = config;
	isMask = false;

	switch (m_context)
	{
	default:
	case Context::Simple:
		sectionSwatches->setVisible(true);
		numberShade->setVisible(false);
		labelShade->setVisible(false);
		numberAlpha->setVisible(false);
		labelAlpha->setVisible(false);
		break;
	case Context::Fill:
		sectionSwatches->setVisible(true);
		numberShade->setVisible(true);
		labelShade->setVisible(true);
		numberAlpha->setVisible(false);
		labelAlpha->setVisible(false);
		break;
	case Context::Line:
		sectionSwatches->setVisible(true);
		numberShade->setVisible(true);
		labelShade->setVisible(true);
		numberAlpha->setVisible(false);
		labelAlpha->setVisible(false);
		break;
	case Context::FillMask:
	case Context::LineMask:
		sectionSwatches->setVisible(false);
		numberShade->setVisible(false);
		labelShade->setVisible(false);
		numberAlpha->setVisible(true);
		labelAlpha->setVisible(true);
		isMask = true;
		break;
	case Context::DropShadow:
		sectionSwatches->setVisible(true);
		numberShade->setVisible(true);
		labelShade->setVisible(true);
		numberAlpha->setVisible(false);
		labelAlpha->setVisible(false);
		break;
	}

	updateSize();
}

QString ColorPickerColor::toolTipText() const
{
	int op = 100 - m_color.Opacity * 100.0;
	int sh = m_color.Shade;

	if (isMask)
		return QString( tr("Opacity: %1 %").arg(op) );

	QString name = m_color.Name.isEmpty() ? CommonStrings::tr_None : m_color.Name;
	QString shade = sh < 100 ? QString(", Shade: %1 %").arg(sh) : "";
	QString opacity = op < 100 ? QString(", Opacity: %1 %").arg(op) : "";
	return QString( tr("Color: %1%2%3").arg(name).arg(shade).arg(opacity) );
}

/* ********************************************************************************* *
 *
 * Slots
 *
 * ********************************************************************************* */

void ColorPickerColor::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
}

void ColorPickerColor::setColorData(const CPColorData& color)
{
	m_color = color;

	// update UI + controls
	QSignalBlocker sigSwatches(swatches);
	swatches->setCurrentColor(m_color.Name);

	QSignalBlocker sigShade(numberShade);
	numberShade->setValue(m_color.Shade);
	numberShade->setEnabled(m_color.Name != CommonStrings::tr_NoneColor);

	QSignalBlocker sigAlpha(numberAlpha);
	numberAlpha->setValue(qRound(100 - (m_color.Opacity * 100)));
	numberAlpha->setEnabled(m_color.Name != CommonStrings::tr_NoneColor);

}

void ColorPickerColor::languageChange()
{
	sectionSwatches->setText( tr("Color Swatches"));
}

void ColorPickerColor::updateColorFromSwatches()
{
	m_color.Name = swatches->currentColor();
	numberShade->setEnabled(m_color.Name != CommonStrings::tr_NoneColor);
	numberAlpha->setEnabled(m_color.Name != CommonStrings::tr_NoneColor);

	updateColor();
}

void ColorPickerColor::updateColorShade()
{
	m_color.Shade = numberShade->value();
	updateColor();
}

void ColorPickerColor::updateColorAlpha()
{
	m_color.Opacity = (100 - numberAlpha->value()) / 100.0;
	updateColor();
}

void ColorPickerColor::updateColor()
{
	//	setColorData(m_color);
	emit colorChanged();
}

void ColorPickerColor::updateSize()
{
	int w = this->width();
	adjustSize();
	resize(w, sizeHint().height());
	emit sizeChanged();
}


