/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "colorwheelwidget.h"

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <QPainter>

#include "sccolor.h"
#include "sccolorengine.h"


ColorWheel::ColorWheel(QWidget * parent, const char * name) : QLabel(parent)
{
	setObjectName(name);
	pointList.clear();
	currentDoc = NULL;
	currentColorSpace = colorModelRGB;
	baseAngle = 0;
	angleShift = 270;
	widthH = heightH = 150;
	// create color map
	colorMap.clear();
	// fit the colorMap 1st value with matrix beginning
	int mapIndex = angleShift;
	for (int i = 0; i < 360; ++i)
	{
		QColor c;
		c.setHsv(i, 255, 255);
		ScColor col;
		col.fromQColor(c);
		colorMap[mapIndex++] = col;
		if (mapIndex > 359)
			mapIndex = 0;
	}
	actualColor = colorMap[0];
	trBaseColor = tr("Base Color");
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
	actualColor = ScColorEngine::convertToModel(actualColor, currentDoc, currentColorSpace);
	emit clicked(e->button(), e->pos());
	update();
}

void ColorWheel::paintEvent(QPaintEvent *)
{
	paintWheel();
	paintCenterSample();
	makeColors();
	// clear marks
	for (int i = 0; i < 360; ++i)
		drawBorderPoint(i, false, true);
	QList<PaintPoint>::const_iterator it;
	for (it = pointList.constBegin(); it != pointList.constEnd(); ++it)
		drawBorderPoint((*it).angle, (*it).base);
}

void ColorWheel::makeColors()
{
	if (currentType == Monochromatic)
		makeMonochromatic();
	if (currentType == Analogous)
		makeAnalogous();
	if (currentType == Complementary)
		makeComplementary();
	if (currentType == Split)
		makeSplit();
	if (currentType == Triadic)
		makeTriadic();
	if (currentType == Tetradic)
		makeTetradic();
}

void ColorWheel::paintCenterSample()
{
	QPainter p;
	p.begin(this);
	p.setPen(QPen(Qt::black, 2));
	p.setBrush(ScColorEngine::getDisplayColor(actualColor, currentDoc ));
	p.drawEllipse(widthH - 20, heightH - 20, 40, 40);
	p.end();
}

void ColorWheel::paintWheel()
{
	int h, s, v;
	QColor col(ScColorEngine::getDisplayColor(actualColor, currentDoc ));
	col.getHsv(&h, &s, &v);
	int width = this->width();
	int height = this->height();
	QPainter p;
	p.begin(this);
	p.setWindow( 0, 0, width, height);
	p.fillRect(0, 0, width, height, Qt::white);
	p.setPen(Qt::black);
	p.drawRect(0, 0, width, height);
	// Half sizes
	heightH = height / 2;
	widthH = width / 2;
	for (int i = 0; i < 360; ++i)
	{
		QMatrix matrix;
		matrix.translate(widthH, heightH);
		matrix.rotate((float)i);
		p.setWorldMatrix(matrix);
		QColor c;
		c.setHsv(i, 255, 255);
		p.setPen(QPen(c, 7));
		p.setBrush(c);
		p.drawLine(0, 0, 130, 0);
	}
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

ScColor ColorWheel::colorByAngle(int angle)
{
	while (angle > 359)
		angle -= 359;
	while (angle < 0)
		angle += 359;
	return colorSpaceColor(colorMap[angle]);
}

ScColor ColorWheel::sampleByAngle(int angle)
{
	while (angle > 359)
		angle -= 359;
	while (angle < 0)
		angle += 359;
	//drawBorderPoint(angle);
	PaintPoint p;
	p.angle = angle;
	p.base = false;
	pointList.append(p);
	return colorSpaceColor(colorMap[angle]);
}

ScColor ColorWheel::colorSpaceColor(ScColor col)
{
	QColor newcol;
	ScColor ret;
	int h, s, v;

	ScColorEngine::getRGBColor(col, currentDoc).getHsv(&h, &s, &v);
	newcol.setHsv(h, s, v);
	ret.fromQColor(newcol);
	ret = ScColorEngine::convertToModel(ret, currentDoc, currentColorSpace);
	return ret;
}

void ColorWheel::baseColor()
{
	//clearBorder();
	pointList.clear();
	//drawBorderPoint(baseAngle, true);
	PaintPoint p;
	p.angle = baseAngle;
	p.base = true;
	pointList.append(p);
	//paintCenterSample();
	colorList.clear();
	colorList[trBaseColor] = colorSpaceColor(actualColor);
}

void ColorWheel::makeMonochromatic()
{
	baseColor();
	QColor col(ScColorEngine::getRGBColor(actualColor, currentDoc));
	ScColor l;
	l.fromQColor(col.light());
	l = ScColorEngine::convertToModel(l, currentDoc, currentColorSpace);
	colorList[tr("Monochromatic Light")] = l;
	l.fromQColor(col.dark());
	l = ScColorEngine::convertToModel(l, currentDoc, currentColorSpace);
	colorList[tr("Monochromatic Dark")] = l;
	currentType = Monochromatic;
}

void ColorWheel::makeAnalogous()
{
	baseColor();
	colorList[tr("1st. Analogous")] = sampleByAngle(baseAngle + angle);
	colorList[tr("2nd. Analogous")] = sampleByAngle(baseAngle - angle);
	currentType = Analogous;
}

void ColorWheel::makeComplementary()
{
	baseColor();
	colorList[tr("Complementary")] = sampleByAngle(baseAngle + 180);
	currentType = Complementary;
}

void ColorWheel::makeSplit()
{
	baseColor();
	colorList[tr("1st. Split")] = sampleByAngle(baseAngle + angle);
	colorList[tr("2nd. Split")] = sampleByAngle(baseAngle - angle);
	colorList[tr("3rd. Split")] = sampleByAngle(baseAngle + 180 + angle);
	colorList[tr("4th. Split")] = sampleByAngle(baseAngle + 180 - angle);
	currentType = Split;
}

void ColorWheel::makeTriadic()
{
	baseColor();
	colorList[tr("1st. Triadic")] = sampleByAngle(baseAngle + 120);
	colorList[tr("2nd. Triadic")] = sampleByAngle(baseAngle - 120);
	currentType = Triadic;
}

void ColorWheel::makeTetradic()
{
	baseColor();
	colorList[tr("1st. Tetradic (base opposite)")] = sampleByAngle(baseAngle + 180);
	colorList[tr("2nd. Tetradic (angle)")] = sampleByAngle(baseAngle + angle);
	colorList[tr("3rd. Tetradic (angle opposite)")] = sampleByAngle(baseAngle + angle + 180);
	currentType = Tetradic;
}

// void ColorWheel::clearBorder()
// {
// 	for (int i = 0; i < 360; ++i)
// 		drawBorderPoint(i, false, true);
// }

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

//	CB: DEADCODE detected by Coverity, unused dist
// 	int dist = 0;
	int minv = 0, maxv = 359;
	int r = maxv - minv;
	int val;

	val = (int)(0.5 + minv + r * (M_PI * 3/2 -a) / (2 * M_PI));

//	CB: DEADCODE detected by Coverity, dist=0 from above
//	if ( dist > 0 )
//		val -= dist;

	return val;
}

bool ColorWheel::recomputeColor(ScColor col)
{
	int origh, origs, origv;
	ColorMap::iterator it;
	QColor c(ScColorEngine::getRGBColor(col, currentDoc));
	QColor act(ScColorEngine::getRGBColor(actualColor, currentDoc));

	c.getHsv(&origh, &origs, &origv);
	for (it = colorMap.begin(); it != colorMap.end(); ++it)
	{
		int tmph, tmps, tmpv;
		QColor col(ScColorEngine::getRGBColor(it.value(), currentDoc));
		col.getHsv(&tmph, &tmps, &tmpv);
		if (origh == tmph)
		{
			act.setHsv(tmph, origs, origv);
			actualColor.fromQColor(act);
			actualColor = ScColorEngine::convertToModel(actualColor, currentDoc, currentColorSpace);
			baseAngle = it.key();
			return true;
		}
	}
	return false;
}
