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

#ifndef COLORPICKER_COLOR_H
#define COLORPICKER_COLOR_H

#include <QWidget>
#include "ui_colorpicker_color.h"
#include "scribusdoc.h"
#include "colorpicker_enum.h"

class ColorPickerColor : public QWidget, Ui::ColorPickerColor
{
	Q_OBJECT

public:
	explicit ColorPickerColor(QWidget *parent = nullptr);
	~ColorPickerColor() {};

	const CPColorData& colorData() const { return m_color; }
    void setColorList(const ColorList& list, bool insertNone = true);

	Context context() const;
	void setContext(Context config);

	int type() const { return Gradient_None; };

	QString toolTipText() const;

public slots:
	void setDoc(ScribusDoc *doc);
	void setColorData(const CPColorData& color);
	void languageChange();

private slots:
	void updateColorFromSwatches();
	void updateColorShade();
	void updateColorAlpha();
	void updateColor();
	void updateSize();

private:
	ScribusDoc *m_doc {nullptr};
	CPColorData m_color;
	Context m_context {Context::Simple};
	bool isMask {false};

	void connectSlots();
	void disconnectSlots();

signals:
	void colorChanged();
	void sizeChanged();
};

#endif // COLORPICKER_COLOR_H
