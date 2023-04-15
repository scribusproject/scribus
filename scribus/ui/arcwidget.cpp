/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "arcwidget.h"
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

ArcWidget::ArcWidget(QWidget* parent) : QWidget( parent )
{
	setupUi(this);
	startAngle->setNewUnit(6);
	startAngle->setValues(-360, 360, 1, 0);
	endAngle->setNewUnit(6);
	endAngle->setValues(-360, 360, 1, 0);
}

void ArcWidget::connectSignals(bool conn)
{
	if (conn)
	{
		connect(startAngle, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
		connect(endAngle, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
	}
	else
	{
		disconnect(startAngle, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
		disconnect(endAngle, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
	}
}


void ArcWidget::restoreDefaults(const ItemToolPrefs *prefsData)
{
	connectSignals(false);
	startAngle->setValue(prefsData->arcStartAngle);
	endAngle->setValue(prefsData->arcStartAngle + prefsData->arcSweepAngle);
	updatePreview();
	connectSignals(true);
}

void ArcWidget::saveGuiToPrefs(ItemToolPrefs *prefsData)
{
	prefsData->arcStartAngle = startAngle->value();
	double nSweep = endAngle->value() - startAngle->value();
	if (nSweep < 0)
		nSweep += 360;
	prefsData->arcSweepAngle = nSweep;
}

void ArcWidget::updatePreview()
{
	int arcWidth = Preview->width() - 11;
	int arcHeight = Preview->height() - 11;
	int pixWidth = Preview->width() - 5;
	int pixHeight = Preview->height() - 5;

	const QPalette& palette = this->palette();
	const QColor& textColor = palette.color(QPalette::Text);
	const QColor& windowColor = palette.color(QPalette::Base);

	QPixmap pm(pixWidth * devicePixelRatioF(), pixHeight * devicePixelRatioF());
	pm.setDevicePixelRatio(devicePixelRatioF());
	pm.fill(windowColor);

	QPainter p;
	p.begin(&pm);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setBrush(Qt::NoBrush);
	p.setPen(textColor);
	QPainterPath path;
	path.moveTo(pixWidth / 2.0, pixHeight / 2.0);
	double nSweep = endAngle->value() - startAngle->value();
	if (nSweep < 0)
		nSweep += 360;
	path.arcTo(3.0, 3.0, arcWidth, arcHeight, startAngle->value(), nSweep);
	path.closeSubpath();
	p.strokePath(path, p.pen());
	p.end();

	Preview->setPixmap(pm);
}
