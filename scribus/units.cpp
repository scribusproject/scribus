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

const double unitGetRatioFromIndex(const int index)
{
	//PT, MM, IN, P
	double ratio[] = { 1.0, 25.4/72.0, 1.0/72.0, 1.0/12.0 };
	return ratio[index];
}

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

const QString unitGetSuffixFromIndex(const int index)
{
	QString suffix[] = { QObject::tr(" pt"), QObject::tr(" mm"), QObject::tr(" in"), QObject::tr(" p")};
	return suffix[index];
}

const QString unitGetStrFromIndex(const int index)
{
	QString suffix[] = { QObject::tr("pt"), QObject::tr("mm"), QObject::tr("in"), QObject::tr("p")};
	return suffix[index];
}

const int unitGetDecimalsFromIndex(const int index)
{
	int decimalPoints[] = {100, 1000, 10000, 100};
	return decimalPoints[index];
}

const int unitGetPrecisionFromIndex(const int index)
{
	int precision[] = {2, 3, 4, 2};
	return precision[index];
}

const double mm2pts(int mm)
{
	return mm * 72.0 / 25.4;
}

const double pts2mm(double pts)
{
	return pts / 72.0 * 25.4;
}

