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

#include <qstring.h>
#include <qobject.h>
#include "units.h"

/*!
 * @brief Returns the ratio to points for the selected unit of measure. Ratios are for: PT, MM, IN, P
 */
const double unitGetRatioFromIndex(const int index)
{
	//PT, MM, IN, P
	if (index>UNITCOUNT) 
		return 0;
	double ratio[] = { 1.0, 25.4/72.0, 1.0/72.0, 1.0/12.0 };
	return ratio[index];
}

/*!
 * @brief Strip the text from a value and return the double value for the unit
 */
const double unitValueFromString(const QString& value)
{
	QString lowerValue = value.lower();
	QString dbl = "0.0";
	if (lowerValue.find("pt") != -1)
	{
		dbl = lowerValue.remove("pt");
	}
	else if (lowerValue.find("mm") != -1)
	{
		dbl = lowerValue.remove("mm");
	}
	else if (lowerValue.find("in") != -1)
	{
		dbl = lowerValue.remove("in");
	}
	else if (lowerValue.find("p") != -1)
	{
		dbl = lowerValue.remove("p");
	}
	else
		dbl = "0.0";

	dbl = dbl.stripWhiteSpace();
	return dbl.toDouble();
}

/*!
 * @brief Returns the suffix used in GUI widgets
 */
const QString unitGetSuffixFromIndex(const int index)
{
	//Could also return " "+unitGetStrFromIndex(indeX);
	if (index>UNITCOUNT) 
		return "";
	QString suffix[] = { QObject::tr(" pt"), QObject::tr(" mm"), QObject::tr(" in"), QObject::tr(" p")};
	return suffix[index];
}

/*!
 * @brief Returns a general suffix for each of the units
 */
const QString unitGetStrFromIndex(const int index)
{
	if (index>UNITCOUNT) 
		return "";
	QString suffix[] = { QObject::tr("pt"), QObject::tr("mm"), QObject::tr("in"), QObject::tr("p")};
	return suffix[index];
}

/*!
 * @brief Returns the decimals for the units
 */
const int unitGetDecimalsFromIndex(const int index)
{
	if (index>UNITCOUNT) 
		return 0;
	int decimalPoints[] = {100, 1000, 10000, 100};
	return decimalPoints[index];
}

/*!
 * @brief Returns the precision for the units
 */
const int unitGetPrecisionFromIndex(const int index)
{
	if (index>UNITCOUNT) 
		return 0;
	int precision[] = {2, 3, 4, 2};
	return precision[index];
}

/*!
 * @brief Returns a QStringList of the units for use in QComboBoxes etc
 */
const QStringList unitGetTextUnitList()
{
	QStringList suffixList;
	suffixList.append( QObject::tr( "Points (pt)" ) );
	suffixList.append( QObject::tr( "Millimetres (mm)" ) );
	suffixList.append( QObject::tr( "Inches (in)" ) );
	suffixList.append( QObject::tr( "Picas (p)" ) );
	return QStringList(suffixList);
}

/*!
 * @brief Returns the pts value from the mm value supplied
 */
const double mm2pts(double mm)
{
	return mm / unitGetRatioFromIndex(MM);
}

/*!
 * @brief Returns the pts value from the in value supplied
 */
const double in2pts(double in)
{
	return in / unitGetRatioFromIndex(IN);
}

/*!
 * @brief Returns the pts value from the pica value supplied
 */
const double p2pts(double p)
{
	return p / unitGetRatioFromIndex(P);
}

/*!
 * @brief Returns the mm value from the pt value supplied
 */
const double pts2mm(double pts)
{
	return pts * unitGetRatioFromIndex(MM);
}

/*!
 * @brief Returns the in value from the pt value supplied
 */
const double pts2in(double pts)
{
	return pts * unitGetRatioFromIndex(IN);
}

/*!
 * @brief Returns the pica value from the pt value supplied
 */
const double pts2p(double pts)
{
	return pts * unitGetRatioFromIndex(P);
}

/*!
 * @brief Returns the value from the pt value supplied based on unit index
 */
double pts2value(double Val, int unit)
{
	double ret = 0.0;
	switch (unit)
	{
		case 0:
			ret = Val; //dont multiply by 1
			break;
		default:
			ret = Val * unitGetRatioFromIndex(unit);
			break;
	}
	return ret;
}

/*!
 * @brief Returns the pt value from the value supplied based on unit index
 */
double value2pts(double unitValue, int unit)
{
	double ret = 0.0;
	switch (unit)
	{
		case 0:
			ret = unitValue; // dont divide by 1
			break;
		default:
			ret = unitValue / unitGetRatioFromIndex(unit);
	}
	return ret;
}
