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
