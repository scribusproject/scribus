/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

class QEvent;

#include "scribusapi.h"
#include "scrpalettebase.h"

class QGridLayout;
class QLabel;

class SCRIBUS_API Measurements : public ScrPaletteBase
{
	Q_OBJECT

public:
	Measurements( QWidget* parent );
	~Measurements() {};
	
	virtual void changeEvent(QEvent *e);

public slots:
	void setValues(double x1, double y1, double x2, double y2, double angle, double length, int unitIndex);
	void languageChange();

protected:
	QGridLayout* measurementsLayout;
	QLabel* x1Label;
	QLabel* y1Label;
	QLabel* x2Label;
	QLabel* y2Label;
	QLabel* dxLabel;
	QLabel* dyLabel;
	QLabel* angleLabel;
	QLabel* lengthLabel;
	QLabel* x1Data;
	QLabel* y1Data;
	QLabel* x2Data;
	QLabel* y2Data;
	QLabel* dXData;
	QLabel* dYData;
	QLabel* lengthData;
	QLabel* angleData;
};

#endif // MEASUREMENTS_H
