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

