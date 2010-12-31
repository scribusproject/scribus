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

PolygonWidget::PolygonWidget(QWidget* parent)
		 : QWidget( parent )
{
	setupUi(this);
}


PolygonWidget::PolygonWidget(QWidget* parent, int polyCorners, int polyFd, double polyF, bool polyUseConvexFactor, double polyRotation, double polyCurvature) : QWidget( parent )
{
	setupUi(this);
	setValues(polyCorners, polyFd, polyF, polyUseConvexFactor, polyRotation, polyCurvature);
	updatePreview();
	// signals and slots connections
	connectSignals(true);
}


void PolygonWidget::connectSignals(bool conn)
{
	if (conn)
	{
		connect(factorSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setFactorSlider(int)));
		connect(rotationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setRotationSlider(int)));
		connect(factorSlider, SIGNAL(valueChanged(int)), factorSpinBox, SLOT(setValue(int)));
		connect(factorSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
		connect(rotationSlider, SIGNAL(valueChanged(int)), rotationSpinBox, SLOT(setValue(int)));
		connect(rotationSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
		connect(cornersSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
		connect(applyConvexGroupBox, SIGNAL(clicked()), this, SLOT(updatePreview()));
		connect(curvatureSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setCurvatureSlider(int)));
		connect(curvatureSlider, SIGNAL(valueChanged(int)), curvatureSpinBox, SLOT(setValue(int)));
		connect(curvatureSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	}
	else
	{
		disconnect(factorSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setFactorSlider(int)));
		disconnect(rotationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setRotationSlider(int)));
		disconnect(factorSlider, SIGNAL(valueChanged(int)), factorSpinBox, SLOT(setValue(int)));
		disconnect(factorSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
		disconnect(rotationSlider, SIGNAL(valueChanged(int)), rotationSpinBox, SLOT(setValue(int)));
		disconnect(rotationSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
		disconnect(cornersSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
		disconnect(applyConvexGroupBox, SIGNAL(clicked()), this, SLOT(updatePreview()));
		disconnect(curvatureSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setCurvatureSlider(int)));
		disconnect(curvatureSlider, SIGNAL(valueChanged(int)), curvatureSpinBox, SLOT(setValue(int)));
		disconnect(curvatureSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	}
}


void PolygonWidget::restoreDefaults(struct ItemToolPrefs *prefsData)
{
	connectSignals(false);
	PFactor = prefsData->polyFactor;
	cornersSpinBox->setValue(prefsData->polyCorners);
	rotationSpinBox->setValue(static_cast<int>(prefsData->polyRotation));
	rotationSlider->setValue(static_cast<int>(prefsData->polyRotation));
	applyConvexGroupBox->setChecked(prefsData->polyUseFactor);
	factorSpinBox->setValue(prefsData->polyFactorGuiVal);
	factorSlider->setValue(prefsData->polyFactorGuiVal);
	applyConvexGroupBox->setChecked(prefsData->polyUseFactor);
	curvatureSpinBox->setValue(qRound(prefsData->polyCurvature * 100));
	curvatureSlider->setValue(qRound(prefsData->polyCurvature * 100));
	updatePreview();
	connectSignals(true);
}

void PolygonWidget::saveGuiToPrefs(struct ItemToolPrefs *prefsData)
{
	prefsData->polyCorners = cornersSpinBox->value();
	prefsData->polyFactor = PFactor;
	prefsData->polyUseFactor = applyConvexGroupBox->isChecked();
	prefsData->polyFactorGuiVal = factorSlider->value();
	prefsData->polyRotation = rotationSpinBox->value();
	prefsData->polyCurvature = curvatureSpinBox->value() / 100.0;
}

void PolygonWidget::setValues(int polyCorners, int polyFd, double polyF, bool polyUseConvexFactor, double polyRotation, double polyCurvature)
{
	PFactor = polyF;
	cornersSpinBox->setValue(polyCorners);
	rotationSpinBox->setValue(static_cast<int>(polyRotation));
	rotationSlider->setValue(static_cast<int>(polyRotation));
	applyConvexGroupBox->setChecked(polyUseConvexFactor);
	factorSpinBox->setValue(polyFd);
	factorSlider->setValue(polyFd);
	applyConvexGroupBox->setChecked(polyUseConvexFactor);
	curvatureSpinBox->setValue(qRound(polyCurvature * 100));
	curvatureSlider->setValue(qRound(polyCurvature * 100));
}

void PolygonWidget::getValues(int* polyCorners, int* polyFd, double* polyF, bool* polyUseConvexFactor, double* polyRotation, double* polyCurvature)
{
	*polyCorners = cornersSpinBox->value();
	*polyF = PFactor;
	*polyUseConvexFactor = applyConvexGroupBox->isChecked();
	*polyFd = factorSlider->value();
	*polyRotation = rotationSpinBox->value();
	*polyCurvature = curvatureSpinBox->value() / 100.0;
}

void PolygonWidget::setFactorSlider(int a)
{
	disconnect(factorSlider, SIGNAL(valueChanged(int)), factorSpinBox, SLOT(setValue(int)));
	factorSlider->setValue(a);
	connect(factorSlider, SIGNAL(valueChanged(int)), factorSpinBox, SLOT(setValue(int)));
}

void PolygonWidget::setRotationSlider(int a)
{
	disconnect(rotationSlider, SIGNAL(valueChanged(int)), rotationSpinBox, SLOT(setValue(int)));
	rotationSlider->setValue(a);
	connect(rotationSlider, SIGNAL(valueChanged(int)), rotationSpinBox, SLOT(setValue(int)));
}

void PolygonWidget::setCurvatureSlider(int a)
{
	disconnect(curvatureSlider, SIGNAL(valueChanged(int)), curvatureSpinBox, SLOT(setValue(int)));
	curvatureSlider->setValue(a);
	connect(curvatureSlider, SIGNAL(valueChanged(int)), curvatureSpinBox, SLOT(setValue(int)));
}

void PolygonWidget::updatePreview()
{
	double roundness = curvatureSpinBox->value() / 100.0;
	QPixmap pm = QPixmap(Preview->width() - 5, Preview->height() - 5);
	pm.fill(Qt::white);
	QPainter p;
	p.begin(&pm);
	p.setBrush(Qt::NoBrush);
	p.setPen(Qt::black);
	QPainterPath pp = RegularPolygonPath(Preview->width() - 6, Preview->height() - 6, cornersSpinBox->value(), applyConvexGroupBox->isChecked(), GetFactor(), rotationSlider->value(), roundness);
	QRectF br = pp.boundingRect();
	if (br.x() < 0)
	{
		QTransform m;
		m.translate(-br.x(), 0);
		pp = pp * m;
	}
	if (br.y() < 0)
	{
		QTransform m;
		m.translate(0, -br.y());
		pp = pp * m;
	}
	br = pp.boundingRect();
	if ((br.height() > Preview->height() - 6) || (br.width() > Preview->width() - 6))
	{
		QTransform ma;
		double sca = static_cast<double>(qMax(Preview->height() - 6, Preview->width() - 6)) / static_cast<double>(qMax(br.width(), br.height()));
		ma.scale(sca, sca);
		pp = pp * ma;
	}
	p.strokePath(pp, p.pen());
	p.end();
	Preview->setPixmap(pm);
}

double PolygonWidget::GetZeroFactor()
{
	return sqrt(pow(1.0,2.0)-pow(((sin((360.0/(cornersSpinBox->value()*2))/180* M_PI)* 2.0)/2.0),2.0));
}

double PolygonWidget::GetMaxFactor()
{
	double win = (360.0/(cornersSpinBox->value()*2)) / 180.0 * M_PI;
	double ret;
	if ((360.0/(cornersSpinBox->value()*2)) > 45)
		ret = 1/sin(win);
	else
		ret = 1/cos(win);
	return ret;
}

double PolygonWidget::GetFactor()
{
	int val = factorSlider->value();
	if (val < 0)
		PFactor = GetZeroFactor() * (100.0 + val) / 100.0;
	else
	{
		double ma = GetMaxFactor();
		double mi = GetZeroFactor();
		PFactor = ((ma - mi) * val / 100.0) + mi;
	}
	return PFactor;
}
