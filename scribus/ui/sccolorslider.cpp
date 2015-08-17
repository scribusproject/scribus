/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolorslider.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QStyle>
#include <QStyleOption>
#include <QWidget>

ScColorSlider::ScColorSlider(QWidget* parent) : QSlider(parent)
{

}

void ScColorSlider::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QStyleOptionSlider opt;
	initStyleOption(&opt);
	
	opt.subControls = QStyle::SC_SliderGroove;
	if (tickPosition() != NoTicks)
		opt.subControls |= QStyle::SC_SliderTickmarks;
	if (isSliderDown()) {
		opt.state |= QStyle::State_Sunken;
	}
	
	QPalette slPalette = this->palette();
	QBrush hooverBrush = slPalette.brush(QPalette::Window);
	QRect hooverRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
	
	painter.setPen(Qt::NoPen);
	painter.setBrush(hooverBrush);
	painter.drawRect(hooverRect);

	/*QSize slSize = this->size();
	int slWidth = slSize.width();
	int slHeight = slSize.height();

	QColor c1 = slPalette.color(QPalette::Mid);
	painter.setPen(c1);
	painter.drawLine(0, 0, slWidth - 1, 0);
    painter.drawLine(0, 0, 0, slHeight - 1);

	QColor c2 = slPalette.color(QPalette::Midlight);
	painter.setPen(c2);
    painter.drawLine(slWidth - 1,  0, slWidth - 1, slHeight - 1);
    painter.drawLine(0, slHeight - 1, slWidth - 1, slHeight - 1);*/

	opt.subControls = QStyle::SC_SliderHandle;
	opt.rect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
	style()->drawComplexControl(QStyle::CC_Slider, &opt, &painter, this);
}
