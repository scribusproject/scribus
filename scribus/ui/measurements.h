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

protected:
	void changeEvent(QEvent *e) override;

public slots:
	void setValues(double x1, double y1, double x2, double y2, double angle, double length);
	void unitChanged();
	/** @brief Sets GUI strings on language change */
	void languageChange();
	void localChange();
	
private:
	double mX1 {0.0};
	double mY1 {0.0};
	double mX2 {0.0};
	double mY2 {0.0};
	double mDX {0.0};
	double mDY {0.0};
	double mLength {0.0};
};

#endif // MEASUREMENTS_H
