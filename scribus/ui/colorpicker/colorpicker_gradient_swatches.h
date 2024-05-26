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

#ifndef COLORPICKER_GRADIENT_SWATCHES_H
#define COLORPICKER_GRADIENT_SWATCHES_H

#include "ui_colorpicker_gradient_swatches.h"
#include <QWidget>
#include "vgradient.h"

class ColorPickerGradientSwatches : public QWidget, Ui::ColorPickerGradientSwatches
{
	Q_OBJECT

public:

	explicit ColorPickerGradientSwatches(QWidget *parent = nullptr);
	~ColorPickerGradientSwatches() {};

	void setGradients(const GradientList& list, bool insertNone = true);

public slots:
	void setCurrentGradient(QString gradientName);

private slots:
	void setGradientFromWidget(QString gradientName);

private:
	QString m_gradientName;

signals:
	void gradientChanged(QString name, VGradient gradient);

};

#endif // COLORPICKER_GRADIENT_SWATCHES_H
