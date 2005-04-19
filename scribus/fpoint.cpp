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


bool FPoint::operator==(const FPoint &rhs) const 
{
	return QABS(xp-rhs.xp) < 1E-10 && QABS(yp-rhs.yp) < 1E-10;
}

bool FPoint::operator!=(const FPoint &rhs) const
{
	return QABS(xp-rhs.xp) > 1E-10 || QABS(yp-rhs.yp) > 1E-10;
}
