/* $Id$ */
#include "colorwheelwidget.h"
#include "colorwheelwidget.moc"
#include <qpainter.h>
#include <qpixmap.h>
#include <qimage.h>
#include <math.h>


ColorWheel::ColorWheel(QWidget * parent, const char * name) : QLabel(parent, name, 0)
{
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

QRgb ColorWheel::getPointColor(QPoint p1)
{
	QImage image;
	const QPixmap *pm = pixmap();
	image = pm->convertToImage();
	return image.pixel(p1.x(), p1.y());
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *e)
{
	QValueVector<QPoint> points;
	points.push_back(e->pos());
	actualPoint = e->pos();
	actualRgb = getPointColor(actualPoint);
	paintWheel(points);

	emit clicked(e->button(), e->pos());
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
	p->setPen(QPen(Qt::black, 2));
	p->setBrush(QColor(actualRgb));
	p->drawEllipse(-10, -10, 20, 20);

	QWMatrix matrix;
	matrix.translate(0, 0);
	p->setWorldMatrix(matrix);
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
	//cout << "angle: " << radang << endl;
	int x = actualPoint.x() - width()/2;
	int y = actualPoint.y() - height()/2;
	//cout << " xy: " << actualPoint.x() << " " << actualPoint.y() << endl;
	//cout << "oxy: " << x << " " << y << endl;
	int dx = (int) round(x * cos(radang) - y * sin(radang));
	int dy = (int) round(y * cos(radang) + x * sin(radang));
	//cout << "dxy: " << dx << " " << dy << endl;
	QRgb rgb = getPointColor(QPoint(dx + width()/2, dy + height()/2));
	//cout << "nxy: " << dx + xsize/2 << " " << dy + ysize/2 << endl;
	// create color
	colorList[name] = cmykColor(rgb);
}

CMYKColor ColorWheel::cmykColor(QRgb rgb)
{
	CMYKColor c = CMYKColor();
	c.fromQColor(QColor(rgb));
	c.setColorModel(colorModelCMYK);
	return c;
}

void ColorWheel::baseColor()
{
	colorList.clear();
	colorList["Base Color"] = cmykColor(actualRgb);
}

void ColorWheel::makeMonochromatic()
{
	baseColor();
	QColor c = QColor(actualRgb);
	colorList["Monochromatic Light"] = cmykColor(c.light().rgb());
	colorList["Monochromatic Dark"] = cmykColor(c.dark().rgb());
}

void ColorWheel::makeAnalogous()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + angle, "1st. Analogous");
	sampleByAngle(baseangle - angle, "2nd. Analogous");
}

void ColorWheel::makeComplementary()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + 180, "Complementary");
}

void ColorWheel::makeSplit()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + angle, "1st. Split");
	sampleByAngle(baseangle - angle, "2nd. Split");
	sampleByAngle(baseangle + 180 + angle, "3rd. Split");
	sampleByAngle(baseangle + 180 - angle, "4th. Split");
}

void ColorWheel::makeTriadic()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + 120, "1st. Triadic");
	sampleByAngle(baseangle - 120, "2nd. Triadic");
}

void ColorWheel::makeTetradic()
{
	double baseangle = pointAngle(actualPoint);
	baseColor();
	sampleByAngle(baseangle + 90, "1st. Tetradic");
	sampleByAngle(baseangle + 180, "2nd. Tetradic");
	sampleByAngle(baseangle + 270, "3rd. Tetradic");
}
