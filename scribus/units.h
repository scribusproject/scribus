/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
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

#ifndef UNITS_H
#define UNITS_H

#include <qstring.h>
/*
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
*/
const double unitGetRatioFromIndex(const int index);
const QString unitGetStrFromIndex(const int index);
const QString unitGetSuffixFromIndex(const int index);
const int unitGetDecimalsFromIndex(const int index);
const int unitGetPrecisionFromIndex(const int index);
const double mm2pts(int mm);
const double pts2mm(double pts);


#endif // UNITS_H
