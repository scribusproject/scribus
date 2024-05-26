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

#include "colorpicker_gradient_swatches.h"
#include "ui/widgets/gradientlistbox.h"

ColorPickerGradientSwatches::ColorPickerGradientSwatches(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	listSwatches->setPixmapType(GradientListBox::widePixmap);

	connect(listSwatches, &GradientListBox::currentTextChanged, this, &ColorPickerGradientSwatches::setGradientFromWidget);
}

void ColorPickerGradientSwatches::setGradients(const GradientList &list, bool insertNone)
{
	GradientListModel* clm = qobject_cast<GradientListModel*>(listSwatches->model());
	if (!clm)
		return;

	clm->setGradientList(list, insertNone);
}

void ColorPickerGradientSwatches::setCurrentGradient(QString gradientName)
{
	m_gradientName = gradientName == "" ? CommonStrings::trCustom : gradientName;

	// update color list
	QSignalBlocker sig(listSwatches);
	listSwatches->setCurrentGradient(m_gradientName);

}

void ColorPickerGradientSwatches::setGradientFromWidget(QString gradientName)
{
	if (gradientName == CommonStrings::trCustom)
		gradientName = "";

	setCurrentGradient(gradientName);
	emit gradientChanged(gradientName, listSwatches->currentGradient());
}
