#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include "scribusapi.h"
#include "scrpalettebase.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QVariant;

class SCRIBUS_API Measurements : public ScrPaletteBase
{
	Q_OBJECT

public:
	Measurements( QWidget* parent );
	~Measurements() {};
	//void closeEvent(QCloseEvent *ce);

signals:
	//void Schliessen(bool);

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
