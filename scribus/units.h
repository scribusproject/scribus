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

#include <qstring.h>
#include <qstringlist.h>

#define UNITCOUNT 4

enum Unit {
	POINTS      = 0,
	PT          = 0,
	MILLIMETERS = 1,
	MM          = 1,
	INCHES      = 2,
	IN          = 2,
	PICAS       = 3,
	P           = 3,
	CENTIMETERS = 4,
	CM          = 4

};

const double unitGetRatioFromIndex(const int index);
const QString unitGetStrFromIndex(const int index);
const QString unitGetSuffixFromIndex(const int index);
const int unitGetDecimalsFromIndex(const int index);
const int unitGetPrecisionFromIndex(const int index);
const QStringList unitGetTextUnitList();
const int unitGetMaxIndex();
const double mm2pts(double mm);
const double in2pts(double in);
const double p2pts(double p);
const double cm2pts(double cm);
const double pts2mm(double pts);
const double pts2in(double pts);
const double pts2p(double pts);
const double pts2cm(double pts);
double pts2value(double Val, int unit);
double value2pts(double unitValue, int unit);


#endif // UNITS_H

