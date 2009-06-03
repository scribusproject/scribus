/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "polygonwidget.h"
#include "scconfig.h"
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>

#if _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "util_math.h"
#include "prefsstructs.h"


using namespace std;

PolygonWidget::PolygonWidget(QWidget* parent, int polyC, int polyFd, double polyF, bool polyS, double polyR, double polyCurvature) : QWidget( parent )
{
	setupUi(this);
	PFactor = polyF;
	Ecken->setValue(polyC);
	Faktor2->setValue(static_cast<int>(polyR));
	Slider2->setValue(static_cast<int>(polyR));
	Konvex->setChecked(polyS);
	Faktor->setValue(polyFd);
	Slider1->setValue(polyFd);
	if (polyFd == 0)
		Konvex->setChecked(false);
	CurvatureSpin->setValue(qRound(polyCurvature * 100));
	CurvatureSlider->setValue(qRound(polyCurvature * 100));

	UpdatePreView();
	// signals and slots connections
	connect(Faktor, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin(int)));
	connect(Faktor2, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin2(int)));
	connect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
	connect(Slider1, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	connect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
	connect(Slider2, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	connect(Ecken, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	connect(Konvex, SIGNAL(clicked()), this, SLOT(UpdatePreView()));
	connect(CurvatureSpin, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin3(int)));
	connect(CurvatureSlider, SIGNAL(valueChanged(int)), CurvatureSpin, SLOT(setValue(int)));
	connect(CurvatureSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
}

void PolygonWidget::restoreDefaults(struct toolPrefs *prefsData)
{
	disconnect(Faktor, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin(int)));
	disconnect(Faktor2, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin2(int)));
	disconnect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
	disconnect(Slider1, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	disconnect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
	disconnect(Slider2, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	disconnect(Ecken, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	disconnect(Konvex, SIGNAL(clicked()), this, SLOT(UpdatePreView()));
	disconnect(CurvatureSpin, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin3(int)));
	disconnect(CurvatureSlider, SIGNAL(valueChanged(int)), CurvatureSpin, SLOT(setValue(int)));
	disconnect(CurvatureSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	PFactor = prefsData->polyS;
	Ecken->setValue(prefsData->polyC);	
	Faktor2->setValue(static_cast<int>(prefsData->polyR));
	Slider2->setValue(static_cast<int>(prefsData->polyR));
	Konvex->setChecked(prefsData->polyS);
	Faktor->setValue(prefsData->polyFd);
	Slider1->setValue(prefsData->polyFd);
	if (prefsData->polyFd == 0)
		Konvex->setChecked(false);
	CurvatureSpin->setValue(qRound(prefsData->polyCurvature * 100));
	CurvatureSlider->setValue(qRound(prefsData->polyCurvature * 100));
	UpdatePreView();
	connect(Faktor, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin(int)));
	connect(Faktor2, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin2(int)));
	connect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
	connect(Slider1, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	connect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
	connect(Slider2, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	connect(Ecken, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
	connect(Konvex, SIGNAL(clicked()), this, SLOT(UpdatePreView()));
	connect(CurvatureSpin, SIGNAL(valueChanged(int)), this, SLOT(ValFromSpin3(int)));
	connect(CurvatureSlider, SIGNAL(valueChanged(int)), CurvatureSpin, SLOT(setValue(int)));
	connect(CurvatureSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdatePreView()));
}

void PolygonWidget::getValues(int* polyC, int* polyFd, double* polyF, bool* polyS, double* polyR, double* polyCurvature)
{
	*polyC = Ecken->value();
	*polyF = PFactor;
	*polyS = Konvex->isChecked();
	*polyFd = Slider1->value();
	*polyR = Faktor2->value();
	*polyCurvature = CurvatureSpin->value() / 100.0;
}

void PolygonWidget::ValFromSpin(int a)
{
	disconnect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
	Slider1->setValue(a);
	connect(Slider1, SIGNAL(valueChanged(int)), Faktor, SLOT(setValue(int)));
}

void PolygonWidget::ValFromSpin2(int a)
{
	disconnect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
	Slider2->setValue(a);
	connect(Slider2, SIGNAL(valueChanged(int)), Faktor2, SLOT(setValue(int)));
}

void PolygonWidget::ValFromSpin3(int a)
{
	disconnect(CurvatureSlider, SIGNAL(valueChanged(int)), CurvatureSpin, SLOT(setValue(int)));
	CurvatureSlider->setValue(a);
	connect(CurvatureSlider, SIGNAL(valueChanged(int)), CurvatureSpin, SLOT(setValue(int)));
}

void PolygonWidget::UpdatePreView()
{
	double roundness = CurvatureSpin->value() / 100.0;
	QPixmap pm = QPixmap(Preview->width() - 5, Preview->height() - 5);
	pm.fill(Qt::white);
	QPainter p;
	p.begin(&pm);
	p.setBrush(Qt::NoBrush);
	p.setPen(Qt::black);
	QPainterPath pp = RegularPolygon(Preview->width() - 6, Preview->height() - 6, Ecken->value(), Konvex->isChecked(), GetFaktor(), Slider2->value(), roundness);
	QRectF br = pp.boundingRect();
	if (br.x() < 0)
	{
		QMatrix m;
		m.translate(-br.x(), 0);
		pp = pp * m;
	}
	if (br.y() < 0)
	{
		QMatrix m;
		m.translate(0, -br.y());
		pp = pp * m;
	}
	br = pp.boundingRect();
	if ((br.height() > Preview->height() - 6) || (br.width() > Preview->width() - 6))
	{
		QMatrix ma;
		double sca = static_cast<double>(qMax(Preview->height() - 6, Preview->width() - 6)) / static_cast<double>(qMax(br.width(), br.height()));
		ma.scale(sca, sca);
		pp = pp * ma;
	}
	p.strokePath(pp, p.pen());
	p.end();
	Preview->setPixmap(pm);
}

double PolygonWidget::GetZeroFaktor()
{
	return sqrt(pow(1.0,2.0)-pow(((sin((360.0/(Ecken->value()*2))/180* M_PI)* 2.0)/2.0),2.0));
}

double PolygonWidget::GetMaxFaktor()
{
	double win = (360.0/(Ecken->value()*2)) / 180.0 * M_PI;
	double ret;
	if ((360.0/(Ecken->value()*2)) > 45)
		ret = 1/sin(win);
	else
		ret = 1/cos(win);
	return ret;
}

double PolygonWidget::GetFaktor()
{
	int val = Slider1->value();
	if (val < 0)
		PFactor = GetZeroFaktor() * (100.0 + val) / 100.0;
	else
	{
		double ma = GetMaxFaktor();
		double mi = GetZeroFaktor();
		PFactor = ((ma - mi) * val / 100.0) + mi;
	}
	return PFactor;
}
