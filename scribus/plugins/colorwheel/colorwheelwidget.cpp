/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "colorwheelwidget.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <QPainter>

#include "sccolor.h"
#include "sccolorengine.h"


ColorWheel::ColorWheel(QWidget * parent, const char * name) : QLabel(parent)
{
	setObjectName(name);
	// fit the colorMap 1st value with matrix beginning
	int mapIndex = m_angleShift;
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
	m_baseAngle = valueFromPoint(e->pos());
	actualColor = colorMap[m_baseAngle];
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
	for (auto it = pointList.constBegin(); it != pointList.constEnd(); ++it)
		drawBorderPoint(it->angle, it->base);
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
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(QPen(Qt::black, 2));
	p.setBrush(ScColorEngine::getDisplayColor(actualColor, currentDoc ));
	p.drawEllipse(m_widthH - 20, m_heightH - 20, 40, 40);
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
	p.setWindow(0, 0, width, height);
	p.fillRect(0, 0, width, height, palette().color(QPalette::Base));
	p.setPen(Qt::black);
	p.drawRect(0, 0, width, height);
	// Half sizes
	m_heightH = height / 2;
	m_widthH = width / 2;
	for (int i = 0; i < 360; ++i)
	{
		QTransform matrix;
		matrix.translate(m_widthH, m_heightH);
		matrix.rotate((float) i);
		p.setWorldTransform(matrix);
		QColor c;
		c.setHsv(i, 255, 255);
		p.setPen(QPen(c, 7));
		p.setBrush(c);
		p.drawLine(0, 0, 130, 0);
	}
}

QString ColorWheel::getTypeDescription(MethodType aType) const
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
	while (angle >= 360)
		angle -= 360;
	while (angle < 0)
		angle += 360;
	return colorSpaceColor(colorMap[angle]);
}

ScColor ColorWheel::sampleByAngle(int angle)
{
	while (angle >= 360)
		angle -= 360;
	while (angle < 0)
		angle += 360;
	PaintPoint p;
	p.angle = angle;
	p.base = false;
	pointList.append(p);
	return colorSpaceColor(colorMap[angle]);
}

ScColor ColorWheel::colorSpaceColor(const ScColor& col) const
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
	pointList.clear();
	PaintPoint p;
	p.angle = m_baseAngle;
	p.base = true;
	pointList.append(p);
	colorList.clear();
	colorList[trBaseColor] = colorSpaceColor(actualColor);
}

void ColorWheel::makeMonochromatic()
{
	baseColor();
	QColor col(ScColorEngine::getRGBColor(actualColor, currentDoc));
	ScColor l;
	l.fromQColor(col.lighter());
	l = ScColorEngine::convertToModel(l, currentDoc, currentColorSpace);
	colorList[tr("Monochromatic Light")] = l;
	l.fromQColor(col.darker());
	l = ScColorEngine::convertToModel(l, currentDoc, currentColorSpace);
	colorList[tr("Monochromatic Dark")] = l;
	currentType = Monochromatic;
}

void ColorWheel::makeAnalogous()
{
	baseColor();
	colorList[tr("1st. Analogous")] = sampleByAngle(m_baseAngle + m_angle);
	colorList[tr("2nd. Analogous")] = sampleByAngle(m_baseAngle - m_angle);
	currentType = Analogous;
}

void ColorWheel::makeComplementary()
{
	baseColor();
	colorList[tr("Complementary")] = sampleByAngle(m_baseAngle + 180);
	currentType = Complementary;
}

void ColorWheel::makeSplit()
{
	baseColor();
	colorList[tr("1st. Split")] = sampleByAngle(m_baseAngle + m_angle);
	colorList[tr("2nd. Split")] = sampleByAngle(m_baseAngle - m_angle);
	colorList[tr("3rd. Split")] = sampleByAngle(m_baseAngle + 180 + m_angle);
	colorList[tr("4th. Split")] = sampleByAngle(m_baseAngle + 180 - m_angle);
	currentType = Split;
}

void ColorWheel::makeTriadic()
{
	baseColor();
	colorList[tr("1st. Triadic")] = sampleByAngle(m_baseAngle + 120);
	colorList[tr("2nd. Triadic")] = sampleByAngle(m_baseAngle - 120);
	currentType = Triadic;
}

void ColorWheel::makeTetradic()
{
	baseColor();
	colorList[tr("1st. Tetradic (base opposite)")] = sampleByAngle(m_baseAngle + 180);
	colorList[tr("2nd. Tetradic (angle)")] = sampleByAngle(m_baseAngle + m_angle);
	colorList[tr("3rd. Tetradic (angle opposite)")] = sampleByAngle(m_baseAngle + m_angle + 180);
	currentType = Tetradic;
}

void ColorWheel::drawBorderPoint(int angle, bool base, bool clear)
{
	double r = 137.0;
	angle -= m_angleShift;
	double radang = M_PI * (double) angle / 180.0;
	int x = (int)(r * cos(radang)) + m_widthH;
	int y = (int)(r * sin(radang)) + m_heightH;
	// draw border mark
	QPainter p;
	p.begin(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	if (clear)
	{
		QColor baseColor = palette().color(QPalette::Base);
		p.setPen(QPen(baseColor, 1));
		p.setBrush(baseColor);
	}
	else
	{
		QColor textColor = palette().color(QPalette::WindowText);
		p.setPen(QPen(textColor, 1));
		if (base)
			p.setBrush(Qt::red);
		else
			p.setBrush(Qt::SolidPattern);
	}
	p.drawEllipse(x - 4, y - 4, 8, 8);
	p.end();
}

int ColorWheel::valueFromPoint(const QPoint & p) const
{
	double yy = (double) m_heightH - (double) p.y();
	double xx = (double) p.x() - (double) m_widthH;
	double a = (xx || yy) ? atan2(yy, xx) : 0.0;

	if (a < M_PI / -2)
		a = a + M_PI * 2;

	int minv = 0, maxv = 359;
	int r = maxv - minv;
	int val;

	val = (int)(0.5 + minv + r * (M_PI * 3/2 -a) / (2 * M_PI));

	return val;
}

bool ColorWheel::recomputeColor(const ScColor& col)
{
	int origh, origs, origv;
	QColor c(ScColorEngine::getRGBColor(col, currentDoc));
	QColor act(ScColorEngine::getRGBColor(actualColor, currentDoc));

	c.getHsv(&origh, &origs, &origv);
	int angle = origh + m_angleShift;
	if (angle > 359)
		angle -= 360;

	if (!colorMap.contains(angle))
		return false;

	int tmph, tmps, tmpv;
	QColor qCol(ScColorEngine::getRGBColor(colorMap.value(angle), currentDoc));
	qCol.getHsv(&tmph, &tmps, &tmpv);
	act.setHsv(tmph , origs, origv);
	actualColor.fromQColor(act);
	actualColor = ScColorEngine::convertToModel(actualColor, currentDoc, currentColorSpace);
	m_baseAngle = angle;

	return true;
}
