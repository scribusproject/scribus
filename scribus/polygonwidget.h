/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef POLYGONWIDGET_H
#define POLYGONWIDGET_H

#include <QWidget>
#include "scribusapi.h"
#include "ui_polygonwidgetbase.h"
struct toolPrefs;
class SCRIBUS_API PolygonWidget : public QWidget, Ui::PolygonWidgetBase
{
	Q_OBJECT

public:
	PolygonWidget(QWidget* parent, int polyC, int polyFd, double polyF, bool polyS, double polyR, double polyCurvature);
	~PolygonWidget() {};
	double GetZeroFaktor();
	double GetMaxFaktor();
	void getValues(int* polyC, int* polyFd, double* polyF, bool* polyS, double* polyR, double* polyCurvature);
	void restoreDefaults(struct toolPrefs *prefsData);
	double PFactor;

public slots:
	void UpdatePreView();
	double GetFaktor();
	void ValFromSpin(int a);
	void ValFromSpin2(int a);
	void ValFromSpin3(int a);
};

#endif
