/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@welho.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <qstring.h>
#include <qobject.h>

#include "units.h"

const double unitGetRatioFromIndex(const int index)
{
	double ratio;
	switch (index)
	{
	case 0://PT
		ratio = 1.0;
		break;
	case 1://MM
		ratio = 25.4/72;
		break;
	case 2://IN
		ratio = 1.0 / 72.0;
		break;
	case 3://P
		ratio = 1.0 / 12.0;
		break;
	}
	return ratio;
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

