/* $Id$ */
#include "colorwheelwidget.h"
#include "colorwheelwidget.moc"
#include <qpainter.h>
#include <qpixmap.h>
#include <qimage.h>
#include <math.h>

ColorWheel::ColorWheel(QWidget * parent, const char * name) : QLabel(parent, name, 0)
{
	mousePressed = false;
}

ColorWheel::~ColorWheel()
{
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

QPoint ColorWheel::checkBounds(QPoint p)
{
	// ugly hack to be sure that it won't be out of bounds
	int x = width() - 5;
	int y = height() - 5;
	if (p.x() > x)
		p.setX(x);
	if (p.x() < 5)
		p.setX(5);
	if (p.y() > y)
		p.setY(y);
	if (p.y() < 5)
		p.setY(5);
	return p;
}

QRgb ColorWheel::getPointColor(QPoint p)
{
	QImage image;
	const QPixmap *pm = pixmap();
	image = pm->convertToImage();
	p = checkBounds(p);
	return image.pixel(p.x(), p.y());
}

void ColorWheel::mousePressEvent(QMouseEvent *e)
{
	mousePressed = true;
	actualPoint = e->pos();
	actualRgb = getPointColor(actualPoint);
	paintCenterSample();
	QPainter p;
	p.begin(this);
	p.setPen(QPen(Qt::black, 1));
	p.setBrush(Qt::NoBrush);
	p.drawEllipse(actualPoint.x()-3, actualPoint.y()-3, 6, 6);
	p.end();
	oldPoint = actualPoint;
}

void ColorWheel::mouseMoveEvent(QMouseEvent *e)
{
	if (!mousePressed)
		return;
	actualPoint = e->pos();
	actualRgb = getPointColor(actualPoint);
	paintCenterSample();
	QPainter p;
	p.begin(this);
	p.setPen(QPen(Qt::white, 1));
	p.setBrush(Qt::NoBrush);
	p.setRasterOp(XorROP);
	p.drawEllipse(oldPoint.x()-3, oldPoint.y()-3, 6, 6);
	p.drawEllipse(actualPoint.x()-3, actualPoint.y()-3, 6, 6);
	p.end();
	oldPoint = actualPoint;
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *e)
{
	mousePressed = false;
	QValueVector<QPoint> points;
	points.push_back(e->pos());
	actualPoint = e->pos();
	actualRgb = getPointColor(actualPoint);
	paintWheel(points);

	emit clicked(e->button(), e->pos());
}

void ColorWheel::paintCenterSample()
{
//	QPixmap *pm = pixmap();
	QPainter p;
	p.begin(this);
	p.setPen(QPen(Qt::black, 2));
	p.setBrush(QColor(actualRgb));
	p.drawEllipse(width()/2 - 10, height()/2 - 10, 20, 20);
	p.end();
//	setPixmap(*pm);
}

void ColorWheel::paintWheel(QValueVector<QPoint> selectedPoints)
{
	QPixmap pm(width(), height());
	pm.fill(Qt::white);
	QPainter *p = new QPainter(&pm);
	p->setWindow( 0, 0, width(), height());
	p->setPen(Qt::white);
	p->drawRect(0, 0, width(), height());
	for (int i = 0; i < 361; ++i)
	{
		QWMatrix matrix;
		matrix.translate(width()/2, height()/2);
		matrix.rotate((float)i);
		p->setWorldMatrix(matrix);
		QColor c;
		c.setHsv(i, 255, 255);
		p->setPen(QPen(c.dark(), 5));
		p->setBrush(c.dark());
		p->drawLine(0, 0, 130, 0);
		p->setPen(QPen(c, 7));
		p->setBrush(c);
		p->drawLine(0, 0, 90, 0);
		p->setPen(QPen(c.light(), 9));
		p->setBrush(c.light());
		p->drawLine(0, 0, 50, 0);
	}

	QWMatrix matrix;
	matrix.translate(0, 0);
	p->setWorldMatrix(matrix);
	p->setPen(QPen(Qt::black, 2));
	p->setBrush(QColor(actualRgb));
	p->drawEllipse(width()/2 - 10, height()/2 - 10, 20, 20);
	if (!selectedPoints.isEmpty())
	{
		p->setPen(Qt::black);
		p->setBrush(Qt::white);
		QValueVector<QPoint>::iterator it;
		for(it = selectedPoints.begin(); it != selectedPoints.end(); ++it)
			p->drawEllipse(it->x()-2, it->y()-2, 4, 4);
	}

	p->end();
	delete(p);
	clear();
	setPixmap(pm);
//	paintCenterSample();
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

double ColorWheel::pointAngle(QPoint p)
{
	double rad2deg = 180.0 / M_PI;
	if ((p.x() == 0) && (p.y() < 0))
		return 270.0;
	if ((p.x() == 0) && (p.y() > 0))
		return 90.0;
	if ((p.x() > 0) && (p.y() >= 0))
		return atan(p.y() / p.x()) * rad2deg;
	if ((p.x() < 0) && (p.y() > 0))
		return 80.0 - (atan(p.y() / abs(p.x())) * rad2deg);
	if ((p.x() < 0) && (p.y() <= 0))
		return 80.0 + (atan(p.y() / p.x()) * rad2deg);
	if ((p.x() > 0) && (p.y() < 0))
		return 360.0 - (atan(abs(p.y()) / p.x()) * rad2deg);
	return 0.0;
}

void ColorWheel::sampleByAngle(double angle, QString name)
{
	double radang = M_PI * angle/180;
	int x = actualPoint.x() - width()/2;
	int y = actualPoint.y() - height()/2;
	//qDebug(QString("ap: %1 %2").arg(actualPoint.x()).arg(actualPoint.y()));
	//qDebug(QString("ap: x=%1 y=%2").arg(x).arg(y));
#ifndef _WIN32
	int dx = (int) round(x * cos(radang) - y * sin(radang));
	int dy = (int) round(y * cos(radang) + x * sin(radang));
#else
	int dx = (int) floor(x * cos(radang) - y * sin(radang) + 0.5);
	int dy = (int) floor(y * cos(radang) + x * sin(radang) + 0.5);
#endif
	//qDebug(QString("sdebug: dx=%1 dw=%2 dy=%3 dh=%4").arg(dx).arg(width()/2).arg(dy).arg(height()/2));
	//qDebug(QString("sdebug: x=%1 y=%2").arg(dx + width()/2).arg(dy + height()/2));
	QRgb rgb = getPointColor(QPoint(dx + width()/2, dy + height()/2));
	// create color
	colorList[name] = cmykColor(rgb);
}

ScColor ColorWheel::cmykColor(QRgb rgb)
{
	ScColor c = ScColor();
/* Dirty Hack to avoid Color Managed RGB -> CMYK conversion */
	c.setSpotColor(true);
	c.fromQColor(QColor(rgb));
	c.setColorModel(colorModelCMYK);
	c.setSpotColor(false);
	return c;
}

void ColorWheel::baseColor()
{
	colorList.clear();
	colorList[ tr("Base Color")] = cmykColor(actualRgb);
}

void ColorWheel::makeMonochromatic()
{
	baseColor();
	QColor c = QColor(actualRgb);
	colorList[ tr("Monochromatic Light")] = cmykColor(c.light().rgb());
	colorList[ tr("Monochromatic Dark")] = cmykColor(c.dark().rgb());
}

void ColorWheel::makeAnalogous()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + angle, tr("1st. Analogous"));
	sampleByAngle(baseangle - angle, tr("2nd. Analogous"));
}

void ColorWheel::makeComplementary()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + 180, tr("Complementary"));
}

void ColorWheel::makeSplit()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + angle, tr("1st. Split"));
	sampleByAngle(baseangle - angle, tr("2nd. Split"));
	sampleByAngle(baseangle + 180 + angle, tr("3rd. Split"));
	sampleByAngle(baseangle + 180 - angle, tr("4th. Split"));
}

void ColorWheel::makeTriadic()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + 120, tr("1st. Triadic"));
	sampleByAngle(baseangle - 120, tr("2nd. Triadic"));
}

void ColorWheel::makeTetradic()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + 180, tr("1st. Tetradic (base opposite)"));
	sampleByAngle(baseangle + angle, tr("2nd. Tetradic (angle)"));
	sampleByAngle(baseangle + angle + 180, tr("3rd. Tetradic (angle opposite)"));
}
