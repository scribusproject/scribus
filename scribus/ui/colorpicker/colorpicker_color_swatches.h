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

#ifndef COLORPICKER_COLOR_SWATCHES_H
#define COLORPICKER_COLOR_SWATCHES_H

#include "ui_colorpicker_color_swatches.h"
#include <QWidget>
#include "sccolor.h"

class ColorPickerColorSwatches : public QWidget, Ui::ColorPickerColorSwatches
{
	Q_OBJECT

public:

	explicit ColorPickerColorSwatches(QWidget *parent = nullptr);
	~ColorPickerColorSwatches() {};

	QString currentColor() const;
	void setColors(const ColorList& list, bool insertNone = true);

public slots:
	void setCurrentColor(QString colorName);

private slots:
	void setColorFromWidget(QString colorName);

private:

signals:
	void colorChanged();

};

#endif // COLORPICKER_COLOR_SWATCHES_H

