/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QMatrix>

//Create transformed point
FPoint::FPoint(const double x, const double y, const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert)
	: xp(x), yp(y)
{
	transform(dx,dy,rot,sx,sy,invert);
}

bool FPoint::operator==(const FPoint &rhs) const 
{
	return qAbs(xp-rhs.xp) < 1E-10 && qAbs(yp-rhs.yp) < 1E-10;
}

bool FPoint::operator!=(const FPoint &rhs) const
{
	return qAbs(xp-rhs.xp) > 1E-10 || qAbs(yp-rhs.yp) > 1E-10;
}

void FPoint::transform(const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert)
{
	QMatrix ma;
	ma.translate(dx, dy);
	ma.scale(sx, sy);
	ma.rotate(rot);
	if (invert)
		ma = ma.inverted();
	//save new value as old one is used on next line
	double newxp = ma.m11() * xp + ma.m21() * yp + ma.dx();
	yp = ma.m22() * yp + ma.m12() * xp + ma.dy();
	xp = newxp;
}

FPoint FPoint::transformPoint(const QMatrix& m, const bool invert) const
{
	QMatrix ma;
	if (invert)
		ma = m.inverted();
	else
		ma = m;
	QPointF p(xp, yp);
	p = ma.map(p);
	return FPoint(p.x(), p.y());
}


FPoint FPoint::transformPoint(const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert) const
{
	QMatrix ma;
	ma.translate(dx, dy);
	ma.scale(sx, sy);
	ma.rotate(rot);
	if (invert)
		ma = ma.inverted();
	double x = ma.m11() * xp + ma.m21() * yp + ma.dx();
	double y = ma.m22() * yp + ma.m12() * xp + ma.dy();
	return FPoint(x, y);
}
