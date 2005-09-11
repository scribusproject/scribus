/***************************************************************************
                          fpoint.cpp  -  description
                             -------------------
    begin                : Mit Jul 24 2002
    copyright            : (C) 2002 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "fpoint.h"

#include <qwmatrix.h>

//Create transformed point
FPoint::FPoint(const double x, const double y, const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert)
{
	xp=x;
	yp=y;
	transform(dx,dy,rot,sx,sy,invert);
}

bool FPoint::operator==(const FPoint &rhs) const 
{
	return QABS(xp-rhs.xp) < 1E-10 && QABS(yp-rhs.yp) < 1E-10;
}

bool FPoint::operator!=(const FPoint &rhs) const
{
	return QABS(xp-rhs.xp) > 1E-10 || QABS(yp-rhs.yp) > 1E-10;
}

void FPoint::transform(const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert)
{
	QWMatrix ma;
	ma.translate(dx, dy);
	ma.scale(sx, sy);
	ma.rotate(rot);
	if (invert)
		ma = ma.invert();
	//save new value as old one is used on next line
	double newxp = ma.m11() * xp + ma.m21() * yp + ma.dx();
	yp = ma.m22() * yp + ma.m12() * xp + ma.dy();
	xp = newxp;
}

FPoint FPoint::transformPoint(const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert)
{
	QWMatrix ma;
	ma.translate(dx, dy);
	ma.scale(sx, sy);
	ma.rotate(rot);
	if (invert)
		ma = ma.invert();
	double x = ma.m11() * xp + ma.m21() * yp + ma.dx();
	double y = ma.m22() * yp + ma.m12() * xp + ma.dy();
	return FPoint(x, y);
}
