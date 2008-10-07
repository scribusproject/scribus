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

#include <QString>
#include <QObject>
#include "units.h"

/*!
 * @brief Returns the ratio to points for the selected unit of measure. Ratios are for: PT, MM, IN, P, CM, C. DEG and PCT return 1.0 as they will never convert
 */
double unitGetRatioFromIndex(const int index)
{
	//PT, MM, IN, P, CM, C (Cicero)
	//NOTE: Calling functions that divide by this value will crash on divide by 0. They shouldnt be getting
	// a zero value if they are accessing here with a correct index.
	if (index<UNITMIN || index>UNITMAX)
		return 0;
	//                  PT,        MM,       IN,   P,             CM,               C,   °,   %
	double ratio[] = { 1.0, 25.4/72.0, 1.0/72.0, 1.0,      2.54/72.0, 25.4/72.0/4.512, 1.0, 1.0 };
// 	double ratio[] = { 1.0, 25.4/72.0, 1.0/72.0, 1.0/12.0, 2.54/72.0, 25.4/72.0/4.512, 1.0, 1.0 };
	return ratio[index];
}

int SCRIBUS_API unitGetBaseFromIndex(const int index)
{
	if (index==SC_P)
		return 12;
	return 10;
}

/*!
 * @brief Strip the text from a value and return the double value for the unit
 */
double unitValueFromString(const QString& value)
{
	QString lowerValue = value.toLower();
	QString dbl = "0.0";
	if (lowerValue.indexOf("pt") != -1)
	{
		dbl = lowerValue.remove("pt");
	}
	else if (lowerValue.indexOf("mm") != -1)
	{
		dbl = lowerValue.remove("mm");
	}
	else if (lowerValue.indexOf("in") != -1)
	{
		dbl = lowerValue.remove("in");
	}
	else if (lowerValue.indexOf("p") != -1)
	{
		dbl = lowerValue.remove("p");
	}
	else if (lowerValue.indexOf("cm") != -1)
	{
		dbl = lowerValue.remove("cm");
	}
	else if (lowerValue.indexOf("°") != -1)
	{
		dbl = lowerValue.remove("°");
	}
	else if (lowerValue.indexOf("%") != -1)
	{
		dbl = lowerValue.remove("%");
	}
	else
		dbl = "0.0";

	dbl = dbl.trimmed();
	return dbl.toDouble();
}

/*!
 * @brief Strip the text from a value and return the Unit index for the value
 */
scUnit unitIndexFromString(const QString& value)
{
	QString lowerValue = value.toLower();
	scUnit retVal;
	if (lowerValue.indexOf("pt") != -1)
	{
		retVal=SC_PT;
	}
	else if (lowerValue.indexOf("mm") != -1)
	{
		retVal=SC_MM;
	}
	else if (lowerValue.indexOf("in") != -1)
	{
		retVal=SC_IN;
	}
	else if (lowerValue.indexOf("p") != -1)
	{
		retVal=SC_P;
	}
	else if (lowerValue.indexOf("cm") != -1)
	{
		retVal=SC_CM;
	}
	else if (lowerValue.indexOf("c") != -1)
	{
		retVal=SC_C;
	}	
	else if (lowerValue.indexOf("°") != -1)
	{
		retVal=SC_DEGREES;
	}
	else if (lowerValue.indexOf("%") != -1)
	{
		retVal=SC_PERCENT;
	}	
	else
		retVal=SC_PT;
	return retVal;
}

/*!
 * @brief Returns the suffix used in GUI widgets
 */
const QString unitGetSuffixFromIndex(const int index)
{
	if (index==SC_P)
	{
		return "";
	}
	return QString(" %1").arg(unitGetStrFromIndex(index));
}

/*!
 * @brief Returns a general suffix for each of the units
 */
const QString unitGetStrFromIndex(const int index)
{
	if (index<UNITMIN || index>UNITMAX) 
		return "";
	QString suffix[] = {
						QObject::tr("pt"), 
						QObject::tr("mm"), 
						QObject::tr("in"), 
						QObject::tr("p"), 
						QObject::tr("cm"), 
						QObject::tr("c"),
						QObject::tr("\xB0", "degrees, unicode 0xB0"), //degree
						QObject::tr("%")
						};
	return suffix[index];
}

/*!
 * @brief Returns a general untranslated suffix for each of the units
 */
const QString unitGetUntranslatedStrFromIndex(const int index)
{
	if (index<UNITMIN || index>UNITMAX) 
		return "";
	QString suffix[] = { "pt", "mm", "in", "p", "cm", "c", "\xB0", "%" };
	return suffix[index];
}
/*!
 * @brief Returns the decimals for the units
 */
int unitGetDecimalsFromIndex(const int index)
{
	if (index<UNITMIN || index>UNITMAX) 
		return 0;
	//                      PT,   MM,    IN,   P,    CM,     C,   °,   %
	int decimalPoints[] = {100, 1000, 10000, 100, 10000, 10000, 100, 100};
	return decimalPoints[index];
}

/*!
 * @brief Returns the precision for the units
 */
int unitGetPrecisionFromIndex(const int index)
{
	if (index<UNITMIN || index>UNITMAX) 
		return 0;
	//                PT,MM,IN, P,CM, C, °, %
	int precision[] = {2, 3, 4, 2, 4, 4, 2, 2};
	return precision[index];
}

/*!
 * @brief Returns a QStringList of the units for use in QComboBoxes etc
 */
const QStringList unitGetTextUnitList()
{
	QStringList suffixList;
	suffixList.append( QObject::tr( "Points (pt)" ) );
	suffixList.append( QObject::tr( "Millimeters (mm)" ) );
	suffixList.append( QObject::tr( "Inches (in)" ) );
	suffixList.append( QObject::tr( "Picas (p)" ) );
	suffixList.append( QObject::tr( "Centimeters (cm)" ) );
	suffixList.append( QObject::tr( "Cicero (c)" ) );
	//Here for completeness, dont use!
	//suffixList.append( QObject::tr( "°" ) );
	//suffixList.append( QObject::tr( "%" ) );
	return QStringList(suffixList);
}

/*!
 * @brief Returns the maximum index of the units we have now
 */
int unitGetMaxIndex()
{
	return UNITMAX;
}

/*!
 * @brief Returns the pts value from the mm value supplied
 */
double mm2pts(double mm)
{
	return mm / unitGetRatioFromIndex(SC_MM);
}

/*!
 * @brief Returns the pts value from the in value supplied
 */
double in2pts(double in)
{
	return in / unitGetRatioFromIndex(SC_IN);
}

/*!
 * @brief Returns the pts value from the pica value supplied
 */
double p2pts(double p)
{
	return p / unitGetRatioFromIndex(SC_P);
}

/*!
 * @brief Returns the pts value from the cm value supplied
 */
double cm2pts(double cm)
{
	return cm / unitGetRatioFromIndex(SC_CM);
}

/*!
 * @brief Returns the pts value from the cm value supplied
 */
double c2pts(double c)
{
	return c / unitGetRatioFromIndex(SC_C);
}

/*!
 * @brief Returns the mm value from the pt value supplied
 */
double pts2mm(double pts)
{
	return pts * unitGetRatioFromIndex(SC_MM);
}

/*!
 * @brief Returns the in value from the pt value supplied
 */
double pts2in(double pts)
{
	return pts * unitGetRatioFromIndex(SC_IN);
}

/*!
 * @brief Returns the pica value from the pt value supplied
 */
double pts2p(double pts)
{
	return pts * unitGetRatioFromIndex(SC_P);
}

/*!
 * @brief Returns the cm value from the pt value supplied
 */
double pts2cm(double pts)
{
	return pts * unitGetRatioFromIndex(SC_CM);
}

/*!
 * @brief Returns the c value from the pt value supplied
 */
double pts2c(double pts)
{
	return pts * unitGetRatioFromIndex(SC_C);
}

/*!
 * @brief Returns the value from the pt value supplied based on unit index
 */
double pts2value(double unitValue, int unit)
{
	double ret = 0.0;
	switch (unit)
	{
		case 0:
		case 3:
		case 6:
		case 7:
			ret = unitValue; //dont multiply by 1
			break;
		default:
			ret = unitValue * unitGetRatioFromIndex(unit);
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
		case 3:
		case 6:
		case 7:
			ret = unitValue; // dont divide by 1
			break;
		default:
			ret = unitValue / unitGetRatioFromIndex(unit);
			break;
	}
	return ret;
}

/*!
 * @brief Returns the secondary unit value from the value supplied based on primary unit
 */
double value2value(double unitValue, int primaryUnit, int secondaryUnit)
{
	if (primaryUnit==secondaryUnit)
		return unitValue;
		
	double pts = 0.0, ret = 0.0;
	//Can make this not convert to points at a later stage, for now, the function exists and works.
	pts= primaryUnit==0 ? unitValue : unitValue / unitGetRatioFromIndex(primaryUnit);
	ret= secondaryUnit==0 ? pts : pts * unitGetRatioFromIndex(secondaryUnit);
	return ret;
}

QString value2String(double unitValue, int unitIndex, bool round2Precision, bool appendSuffix)
{
	QString s;
	if (round2Precision)
		s=QString::number(pts2value(unitValue, unitIndex), 'f', unitGetPrecisionFromIndex(unitIndex));
	else
		s=QString::number(pts2value(unitValue, unitIndex));
	if (appendSuffix)
		s += unitGetStrFromIndex(unitIndex);
	return s;
}

/*!
 * @brief Sets up iteration value 1 for vruler, hruler and tabruler
 */
double unitRulerGetIter1FromIndex(const int index)
{
	if (!unitValidForDocUnit(index)) 
		return 0;
	//                 PT,         MM,   IN,    P,        CM,               C     °,    %
	double iter[] = {10.0, 720.0/25.4, 18.0, 12.0, 72.0/25.4, 72.0/25.4*4.512, 10.0, 10.0};
	return iter[index];
}

/*!
 * @brief Sets up iteration value 2 for vruler, hruler and tabruler
 */
double unitRulerGetIter2FromIndex(const int index)
{
	if (!unitValidForDocUnit(index))
		return 0;
	//                  PT,          MM,   IN,     P,         CM,                C,     °,     %
	double iter[] = {100.0, 7200.0/25.4, 72.0, 120.0, 720.0/25.4, 720.0/25.4*4.512, 100.0, 100.0};
	return iter[index];
}

bool unitValidForDocUnit(const int index)
{
	if (index<UNITMIN || index>UNITMAX)
		return false;
	if (index==6 || index==7)
		return false;
	return true;
}
