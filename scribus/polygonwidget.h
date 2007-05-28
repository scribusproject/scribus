/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>

#include "scribusapi.h"
#include "prefsstructs.h"

class SCRIBUS_API PolygonWidget : public QWidget
{
	Q_OBJECT

public:
	PolygonWidget(QWidget* parent, int polyC, int polyFd, double polyF, bool polyS, double polyR);
	~PolygonWidget() {};
	double GetZeroFaktor();
	double GetMaxFaktor();
	void getValues(int* polyC, int* polyFd, double* polyF, bool* polyS, double* polyR);
	void restoreDefaults(struct toolPrefs *prefsData);

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
	Q3VBoxLayout* PolygonPropsLayout;
	Q3HBoxLayout* Layout11;
	Q3VBoxLayout* Layout10;
	Q3HBoxLayout* Layout2;
	Q3HBoxLayout* Layout9;
	Q3VBoxLayout* Layout8;
	Q3HBoxLayout* Layout7;
	Q3HBoxLayout* Layout1_2;
	Q3HBoxLayout* Layout9_2;
	Q3VBoxLayout* Layout8_2;
	Q3HBoxLayout* Layout7_2;
};

#endif
