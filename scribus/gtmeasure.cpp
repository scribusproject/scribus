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

#include "gtmeasure.h"

double gtMeasure::ratio = 1.0;

void gtMeasure::init(Unit u)
{
	switch (u)
	{
	case PT:
		ratio = 1.0;
		break;
	case MM:
		ratio = 25.4/72;
		break;
	case IN:
		ratio = 1.0 / 72.0;
		break;
	case P:
		ratio = 1.0 / 12.0;
		break;
	}
}

double gtMeasure::convert(double value)
{
	return value / ratio;
}

double gtMeasure::convert(int value)
{
	return value / ratio;
}

double gtMeasure::convert2(double value)
{
	return value * ratio;
}

double gtMeasure::convert2(int value)
{
	return value * ratio;
}

double gtMeasure::parse(const QString& value)
{
	QString lowerValue = value.lower();
	QString dbl = "0.0";
	if (lowerValue.find("pt") != -1)
	{
		init(PT);
		dbl = lowerValue.remove("pt");
	}
	else if (lowerValue.find("mm") != -1)
	{
		init(MM);
		dbl = lowerValue.remove("mm");
	}
	else if (lowerValue.find("in") != -1)
	{
		init(IN);
		dbl = lowerValue.remove("in");
	}
	else if (lowerValue.find("p") != -1)
	{
		init(P);
		dbl = lowerValue.remove("p");
	}
	else
		init(PT);

	dbl = dbl.stripWhiteSpace();
	return dbl.toDouble();
}

double gtMeasure::convert(double value, Unit from, Unit to)
{
	return d2d(value, from, to);
}

double gtMeasure::convert(int value, Unit from, Unit to)
{
	return i2d(value, from, to);
}

double gtMeasure::d2d(double value, Unit from, Unit to)
{
	init(from);
	double tmp = convert(value);
	init(to);
	return convert2(tmp);
}


double gtMeasure::i2d(int value, Unit from, Unit to)
{
	init(from);
	double tmp = convert(value);
	init(to);
	return convert2(tmp);
}

double gtMeasure::qs2d(const QString& value, Unit to)
{
	double tmp = convert(parse(value));
	init(to);
	return convert2(tmp);
}

QString gtMeasure::getSuffixFromIndex(int index)
{
	QString suffix[] = { QObject::tr(" pt"), QObject::tr(" mm"), QObject::tr(" in"), QObject::tr(" p")};
	return suffix[index];
}

int gtMeasure::getDecimalsFromIndex(int index)
{
	int decimalPoints[] = {100, 1000, 10000, 100};
	return decimalPoints[index];
}
