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
		ratio = 0.3527777;
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

double gtMeasure::parse(QString value)
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
		dbl = lowerValue.remove("pt");
	}
	else if (lowerValue.find("in") != -1)
	{
		init(IN);
		dbl = lowerValue.remove("pt");
	}
	else if (lowerValue.find("p") != -1)
	{
		init(P);
		dbl = lowerValue.remove("pt");
	}
	else
		init(PT);

	dbl = dbl.stripWhiteSpace();
	if (dbl.find(".") == -1)
		dbl += ".0";
	return dbl.toDouble();
}

double gtMeasure::d2d(double value, Unit from)
{
	init(from);
	return convert(value);
}


double gtMeasure::i2d(int value, Unit from)
{
	init(from);
	return convert(value);
}

double gtMeasure::qs2d(QString value)
{
	return convert(parse(value));
}
