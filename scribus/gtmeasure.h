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

#ifndef GTMEASURE_H
#define GTMEASURE_H

#include <qstring.h>

enum Unit {
	POINTS      = 0,
	PT          = 0,
	MILLIMETERS = 1,
	MM          = 1,
	INCHES      = 2,
	IN          = 2,
	PICAS       = 3,
	P           = 3
};

class gtMeasure
{
private:
	gtMeasure();
	static double ratio;
	static void   init(Unit u);
	static double convert(double value);
	static double convert(int value);
	static double parse(QString value);
public:
	static double d2d(double value, Unit from);
	static double i2d(int value, Unit from);
	static double qs2d(QString value);
};

#endif // GTMEASURE_H
