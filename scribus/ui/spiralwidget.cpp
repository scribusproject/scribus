/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "spiralwidget.h"
#include "scconfig.h"
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "util_math.h"
#include "prefsstructs.h"

SpiralWidget::SpiralWidget(QWidget* parent) : QWidget( parent )
{
	setupUi(this);
	startAngle->setNewUnit(6);
	arcFactor->setDecimals(0);
	startAngle->setValues(0, 36000, 1, 0);
	endAngle->setNewUnit(6);
	endAngle->setValues(0, 36000, 1, 0);
}

void SpiralWidget::connectSignals(bool conn)
{
	if (conn)
	{
		connect(startAngle, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
		connect(endAngle, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
		connect(arcFactor, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
	}
	else
	{
		disconnect(startAngle, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
		disconnect(endAngle, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
		disconnect(arcFactor, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
	}
}


void SpiralWidget::restoreDefaults(const ItemToolPrefs *prefsData)
{
	connectSignals(false);
	startAngle->setValue(prefsData->spiralStartAngle);
	endAngle->setValue(prefsData->spiralEndAngle);
	arcFactor->setValue(qRound(prefsData->spiralFactor * 100) - 100);
	updatePreview();
	connectSignals(true);
}

void SpiralWidget::saveGuiToPrefs(ItemToolPrefs *prefsData)
{
	prefsData->spiralStartAngle = startAngle->value();
	prefsData->spiralEndAngle = endAngle->value();
	prefsData->spiralFactor = (static_cast<int>(arcFactor->value()) + 100) / 100.0;
}

void SpiralWidget::updatePreview()
{
	startAngle->setMaximum(endAngle->value() - 1);
	endAngle->setMinimum(startAngle->value() + 1);
	
	int spiralWidth = Preview->width() - 11;
	int spiralHeight = Preview->height() - 11;
	int pixWidth = (Preview->width() - 5) * devicePixelRatioF();
	int pixHeight = (Preview->height() - 5) * devicePixelRatioF();

	const QPalette& palette = this->palette();
	const QColor& textColor = palette.color(QPalette::Text);
	const QColor& windowColor = palette.color(QPalette::Base);

	QPixmap pm(pixWidth, pixHeight);
	pm.setDevicePixelRatio(devicePixelRatioF());
	pm.fill(windowColor);

	QPainter p;
	p.begin(&pm);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setBrush(Qt::NoBrush);
	p.setPen(textColor);
	QPainterPath path = spiralPath(spiralWidth, spiralHeight, startAngle->value(), endAngle->value(), (static_cast<int>(arcFactor->value()) + 100) / 100.0);
	path.translate(3, 3);
	p.strokePath(path, p.pen());
	p.end();

	Preview->setPixmap(pm);
}
