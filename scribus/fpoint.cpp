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

FPoint::FPoint(float x, float y)
{
	xp = x;
	yp = y;
}

FPoint::FPoint(QPoint p)
{
	xp = p.x();
	yp = p.y();
}

float FPoint::x()
{
	return xp;
}

float FPoint::y()
{
	return yp;
}

void FPoint::setX(float x)
{
	xp = x;
}

void FPoint::setY(float y)
{
	yp = y;
}

bool FPoint::operator==(const FPoint &rhs)
{
	return QABS(xp-rhs.xp) < 1E-10 && QABS(yp-rhs.yp) < 1E-10;
}

bool FPoint::operator!=(const FPoint &rhs)
{
	return QABS(xp-rhs.xp) > 1E-10 || QABS(yp-rhs.yp) > 1E-10;
}

FPoint &FPoint::operator+=( const FPoint &p )
{
	xp += p.xp;
	yp += p.yp;
	return *this;
}

FPoint &FPoint::operator-=( const FPoint &p )
{
	xp -= p.xp;
	yp -= p.yp;
	return *this;
}
