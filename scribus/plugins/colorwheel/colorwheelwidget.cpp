/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* $Id$ */
#include "colorwheelwidget.h"
#include "colorwheelwidget.moc"
#include <qpainter.h>
#include <qpixmap.h>
#include <qimage.h>
#include <math.h>

#include "sccolor.h"


ColorWheel::ColorWheel(QWidget * parent, const char * name) : QLabel(parent, name, 0)
{
	baseAngle = 0;
	angleShift = 270;
	widthH = heightH = 150;
}

void ColorWheel::mousePressEvent(QMouseEvent *e)
{
	mouseReleaseEvent(e);
}

void ColorWheel::mouseMoveEvent(QMouseEvent *e)
{
	mouseReleaseEvent(e);
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *e)
{
	baseAngle = valueFromPoint(e->pos());
	actualColor = colorMap[baseAngle];
	emit clicked(e->button(), e->pos());
}

void ColorWheel::paintCenterSample()
{
	QPainter p;
	p.begin(this);
	p.setPen(QPen(Qt::black, 2));
	p.setBrush(actualColor);
	p.drawEllipse(widthH - 20, heightH - 20, 40, 40);
	p.end();
}

void ColorWheel::paintWheel()
{
	int h, s, v;
	actualColor.hsv(&h, &s, &v);

	colorMap.clear();
	QPixmap pm(width(), height());
	pm.fill(Qt::white);
	QPainter *p = new QPainter(&pm);
	p->setWindow( 0, 0, width(), height());
	p->setPen(Qt::white);
	p->drawRect(0, 0, width(), height());
	// Half sizes
	heightH = height() / 2;
	widthH = width() / 2;
	// fit the colorMap 1st value with matrix beginning
	int mapIndex = angleShift;
	for (int i = 0; i < 360; ++i)
	{
		QWMatrix matrix;
		matrix.translate(widthH, heightH);
		matrix.rotate((float)i);
		p->setWorldMatrix(matrix);
		QColor c;
		c.setHsv(i, 255, 255);
		colorMap[mapIndex] = c;
		++mapIndex;
		if (mapIndex > 359)
			mapIndex = 0;
		p->setPen(QPen(c, 7));
		p->setBrush(c);
		p->drawLine(0, 0, 130, 0);
	}
	p->setPen(QPen(Qt::black, 2));
	p->setBrush(actualColor);
	p->drawEllipse(-20, -20, 40, 40);
	p->end();
	delete(p);
	clear();
	setPixmap(pm);
}

QString ColorWheel::getTypeDescription(MethodType aType)
{
	switch (aType)
	{
		case Monochromatic: return tr("Monochromatic");
		case Analogous: return tr("Analogous");
		case Complementary: return tr("Complementary");
		case Split: return tr("Split Complementary");
		case Triadic: return tr("Triadic");
		case Tetradic: return tr("Tetradic (Double Complementary)");
	}
	return "n/a";
}

ScColor ColorWheel::sampleByAngle(int angle)
{
	while (angle > 359)
		angle -= 359;
	while (angle < 0)
		angle += 359;
	drawBorderPoint(angle);
	return cmykColor(colorMap[angle]);
}

ScColor ColorWheel::cmykColor(QColor col)
{
	ScColor c = ScColor();
	/* Dirty Hack to avoid Color Managed RGB -> CMYK conversion */
	c.setSpotColor(true);
	int h, sm, vm, s, v;
	actualColor.getHsv(&h, &s, &v);
	col.getHsv(&h, &sm, &vm);
	col.setHsv(h, s, v);
	c.fromQColor(col);
	c.setColorModel(colorModelCMYK);
	c.setSpotColor(false);
	return c;
}

void ColorWheel::baseColor()
{
	paintCenterSample();
	clearBorder();
	drawBorderPoint(baseAngle, true);
	paintCenterSample();
	colorList.clear();
	colorList[ tr("Base Color")] = cmykColor(actualColor);
}

void ColorWheel::makeMonochromatic()
{
	baseColor();
	ScColor l = ScColor();
	l.fromQColor(actualColor.light());
	ScColor d = ScColor();
	d.fromQColor(actualColor.dark());
	colorList[tr("Monochromatic Light")] = l;
	colorList[tr("Monochromatic Dark")] = d;
}

void ColorWheel::makeAnalogous()
{
	baseColor();
	colorList[tr("1st. Analogous")] = sampleByAngle(baseAngle + angle);
	colorList[tr("2nd. Analogous")] = sampleByAngle(baseAngle - angle);
}

void ColorWheel::makeComplementary()
{
	baseColor();
	colorList[tr("Complementary")] = sampleByAngle(baseAngle + 180);
}

void ColorWheel::makeSplit()
{
	baseColor();
	colorList[tr("1st. Split")] = sampleByAngle(baseAngle + angle);
	colorList[tr("2nd. Split")] = sampleByAngle(baseAngle - angle);
	colorList[tr("3rd. Split")] = sampleByAngle(baseAngle + 180 + angle);
	colorList[tr("4th. Split")] = sampleByAngle(baseAngle + 180 - angle);
}

void ColorWheel::makeTriadic()
{
	baseColor();
	colorList[tr("1st. Triadic")] = sampleByAngle(baseAngle + 120);
	colorList[tr("2nd. Triadic")] = sampleByAngle(baseAngle - 120);
}

void ColorWheel::makeTetradic()
{
	baseColor();
	colorList[tr("1st. Tetradic (base opposite)")] = sampleByAngle(baseAngle + 180);
	colorList[tr("2nd. Tetradic (angle)")] = sampleByAngle(baseAngle + angle);
	colorList[tr("3rd. Tetradic (angle opposite)")] = sampleByAngle(baseAngle + angle + 180);
}

void ColorWheel::clearBorder()
{
	for (int i = 0; i < 360; ++i)
		drawBorderPoint(i, false, true);
}

void ColorWheel::drawBorderPoint(int angle, bool base, bool clear)
{
	double r = 137.0;
	angle -= angleShift;
	double radang = M_PI * (double)angle/180.0;
	int x = (int)(r * cos(radang)) + widthH;
	int y = (int)(r * sin(radang)) + heightH;
	// draw border mark
	QPainter p;
	p.begin(this);
	if (clear)
	{
		p.setPen(QPen(Qt::white, 1));
		p.setBrush(Qt::white);
	}
	else
	{
		p.setPen(QPen(Qt::black, 1));
		if (base)
			p.setBrush(Qt::red);
		else
			p.setBrush(Qt::SolidPattern);
	}
	p.drawEllipse(x-4, y-4, 8, 8);
	p.end();
}

int ColorWheel::valueFromPoint(const QPoint & p) const
{
	double yy = (double)heightH - (double)p.y();
	double xx = (double)p.x() - (double)widthH;
	double a = (xx || yy) ? atan2(yy, xx) : 0.0;

	if ( a < M_PI/-2 )
		a = a + M_PI * 2;

	int dist = 0;
	int minv = 0, maxv = 359;
	int r = maxv - minv;
	int val;

	val = (int)(0.5 + minv + r * (M_PI * 3/2 -a) / (2 * M_PI));

	if ( dist > 0 )
		val -= dist;

	return val;
}

bool ColorWheel::recomputeColor(QColor col)
{
	int origh, origs, origv;
	ColorMap::iterator it;

	col.hsv(&origh, &origs, &origv);
	for (it = colorMap.begin(); it != colorMap.end(); ++it)
	{
		int tmph, tmps, tmpv;
		it.data().hsv(&tmph, &tmps, &tmpv);
		if (origh == tmph)
		{
			actualColor.setHsv(tmph, origs, origv);
			baseAngle = it.key();
			return true;
		}
	}
	return false;
}
