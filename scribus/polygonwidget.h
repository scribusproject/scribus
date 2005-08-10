#ifndef POLYGONWIDGET_H
#define POLYGONWIDGET_H

#include <qwidget.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qpixmap.h>

#include "scribusapi.h"

class SCRIBUS_API PolygonWidget : public QWidget
{
	Q_OBJECT

public:
	PolygonWidget(QWidget* parent, int polyC, int polyFd, double polyF, bool polyS, double polyR);
	~PolygonWidget() {};
	double GetZeroFaktor();
	double GetMaxFaktor();
	void getValues(int* polyC, int* polyFd, double* polyF, bool* polyS, double* polyR);

	QLabel* Text1;
	QSpinBox* Ecken;
	QCheckBox* Konvex;
	QLabel* Text2;
	QSpinBox* Faktor;
	QSlider* Slider1;
	QLabel* Preview;
	QLabel* Text2_2;
	QSpinBox* Faktor2;
	QSlider* Slider2;
	QPixmap* Pre;
	double PFactor;

public slots:
	void UpdatePreView();
	double GetFaktor();
	void ValFromSpin(int a);
	void ValFromSpin2(int a);

protected:
	QVBoxLayout* PolygonPropsLayout;
	QHBoxLayout* Layout11;
	QVBoxLayout* Layout10;
	QHBoxLayout* Layout2;
	QHBoxLayout* Layout9;
	QVBoxLayout* Layout8;
	QHBoxLayout* Layout7;
	QHBoxLayout* Layout1_2;
	QHBoxLayout* Layout9_2;
	QVBoxLayout* Layout8_2;
	QHBoxLayout* Layout7_2;
};

#endif
