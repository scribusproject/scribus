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
	darkness = 100;
}

QPixmap ColorWheel::sample(QColor c)
{
	QPixmap pmap(30, 10);
	QPainter p(&pmap);
	p.setPen(Qt::black);
	p.setBrush(c);
	p.drawRect(0, 0, 30, 10);
	p.end();
	return pmap;
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
	p.drawEllipse(width()/2 - 20, height()/2 - 20, 40, 40);
	p.end();
}

void ColorWheel::paintWheel()
{
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

void ColorWheel::sampleByAngle(int angle, QString name)
{
	while (angle > 359)
		angle -= 359;
	while (angle < 0)
		angle += 359;
	colorList[name] = cmykColor(colorMap[angle]);
	drawBorderPoint(angle);
}

ScColor ColorWheel::cmykColor(QColor col)
{
	ScColor c = ScColor();
	/* Dirty Hack to avoid Color Managed RGB -> CMYK conversion */
	c.setSpotColor(true);
	c.fromQColor(col.dark(darkness));
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
	colorList[ tr("Monochromatic Light")] = cmykColor(actualColor.light());
	colorList[ tr("Monochromatic Dark")] = cmykColor(actualColor.dark());
}

void ColorWheel::makeAnalogous()
{
	baseColor();
	sampleByAngle(baseAngle + angle, tr("1st. Analogous"));
	sampleByAngle(baseAngle - angle, tr("2nd. Analogous"));
}

void ColorWheel::makeComplementary()
{
	baseColor();
	sampleByAngle(baseAngle + 180, tr("Complementary"));
}

void ColorWheel::makeSplit()
{
	baseColor();
	sampleByAngle(baseAngle + angle, tr("1st. Split"));
	sampleByAngle(baseAngle - angle, tr("2nd. Split"));
	sampleByAngle(baseAngle + 180 + angle, tr("3rd. Split"));
	sampleByAngle(baseAngle + 180 - angle, tr("4th. Split"));
}

void ColorWheel::makeTriadic()
{
	baseColor();
	sampleByAngle(baseAngle + 120, tr("1st. Triadic"));
	sampleByAngle(baseAngle - 120, tr("2nd. Triadic"));
}

void ColorWheel::makeTetradic()
{
	baseColor();
	sampleByAngle(baseAngle + 180, tr("1st. Tetradic (base opposite)"));
	sampleByAngle(baseAngle + angle, tr("2nd. Tetradic (angle)"));
	sampleByAngle(baseAngle + angle + 180, tr("3rd. Tetradic (angle opposite)"));
}

void ColorWheel::clearBorder()
{
	QPainter p;
	p.begin(this);
	p.setPen(QPen(Qt::white, 2));
	for (int i = 0; i < 360; ++i)
	{
		QWMatrix matrix;
		matrix.translate(width()/2, height()/2);
		matrix.rotate((float)i);
		p.setWorldMatrix(matrix);
		p.drawLine(130, 0, 145, 30);
	}
	p.end();
}

void ColorWheel::drawBorderPoint(int angle, bool base)
{
	double r = 137.0;
	angle -= angleShift;
	double radang = M_PI * (double)angle/180.0;
	int x = (int)(r * cos(radang)) + widthH;
	int y = (int)(r * sin(radang)) + heightH;
	// draw border mark
	QPainter p;
	p.begin(this);
	p.setPen(QPen(Qt::black, 1));
	if (base)
		p.setBrush(Qt::red);
	else
		p.setBrush(Qt::SolidPattern);
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
	int v;

	v =  (int)(0.5 + minv + r * (M_PI * 3/2 -a) / (2 * M_PI));

	if ( dist > 0 )
		v -= dist;

	return v;
}

void ColorWheel::recomputeColor(QColor col)
{
	int h, s, v;
	ColorMap::iterator it;

	col.hsv(&h, &s, &v);
	for (it = colorMap.begin(); it != colorMap.end(); ++it)
	{
		int hm, sm, vm;
		it.data().hsv(&hm, &sm, &vm);
		if (h == hm)
		{
			QColor c;
			c.setHsv(h, 255, 255);
			actualColor = c;
			baseAngle = it.key();
			darkness = vm;
			return;
		}
	}
	qDebug("DEBUG: ColorWheel::recomputeColor(QColor col): color not in colorMap");
}
