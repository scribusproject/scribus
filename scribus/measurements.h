/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include "ui_measurementsbase.h"
#include "scribusapi.h"
#include "scrpalettebase.h"

class SCRIBUS_API Measurements : public ScrPaletteBase, Ui::MeasurementsBase
{
	Q_OBJECT

public:
	Measurements( QWidget* parent );
	~Measurements() {};

	virtual void changeEvent(QEvent *e);


public slots:
	void setValues(double x1, double y1, double x2, double y2, double angle, double length);
	void unitChanged();
	/** @brief Sets GUI strings on language change */
	void languageChange();
	
private:
	double mX1;
	double mY1;
	double mX2;
	double mY2;
	double mDX;
	double mDY;
	double mLength;
};

#endif // MEASUREMENTS_H
