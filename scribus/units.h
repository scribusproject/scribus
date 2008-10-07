/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
    begin                : Jan 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef UNITS_H
#define UNITS_H

#include "scribusapi.h"
#include <QString>
#include <QStringList>

#define UNITMIN 0
#define UNITMAX 7

enum scUnit {
	SC_POINTS      = 0,
	SC_PT          = 0,
	SC_MILLIMETERS = 1,
	SC_MM          = 1,
	SC_INCHES      = 2,
	SC_IN          = 2,
	SC_PICAS       = 3,
	SC_P           = 3,
	SC_CENTIMETERS = 4,
	SC_CM          = 4,
	SC_CICERO      = 5,
	SC_C           = 5,
	SC_DEGREES     = 6,
	SC_DEG         = 6,
	SC_PERCENT     = 7,
	SC_PCT         = 7
};

double SCRIBUS_API unitGetRatioFromIndex(const int index);
int SCRIBUS_API unitGetBaseFromIndex(const int index);
const QString SCRIBUS_API unitGetStrFromIndex(const int index);
const QString SCRIBUS_API unitGetUntranslatedStrFromIndex(const int index);
const QString SCRIBUS_API unitGetSuffixFromIndex(const int index);
int SCRIBUS_API unitGetDecimalsFromIndex(const int index);
int SCRIBUS_API unitGetPrecisionFromIndex(const int index);
double SCRIBUS_API unitValueFromString(const QString& value);
scUnit SCRIBUS_API unitIndexFromString(const QString& value);
const QStringList SCRIBUS_API unitGetTextUnitList();
int SCRIBUS_API unitGetMaxIndex();
double SCRIBUS_API mm2pts(double mm);
double SCRIBUS_API in2pts(double in);
double SCRIBUS_API p2pts(double p);
double SCRIBUS_API cm2pts(double cm);
double SCRIBUS_API c2pts(double c);
double SCRIBUS_API pts2mm(double pts);
double SCRIBUS_API pts2in(double pts);
double SCRIBUS_API pts2p(double pts);
double SCRIBUS_API pts2cm(double pts);
double SCRIBUS_API pts2c(double pts);
double SCRIBUS_API pts2value(double Val, int unit);
double SCRIBUS_API value2pts(double unitValue, int unit);
double SCRIBUS_API value2value(double unitValue, int primaryUnit, int secondaryUnit);
QString SCRIBUS_API value2String(double unitValue, int unitIndex, bool round2Precision, bool appendSuffix);
//Ruler specific functions
double SCRIBUS_API unitRulerGetIter1FromIndex(const int index);
double SCRIBUS_API unitRulerGetIter2FromIndex(const int index);
bool SCRIBUS_API unitValidForDocUnit(const int index);

#endif // UNITS_H

