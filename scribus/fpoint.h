/***************************************************************************
                          fpoint.h  -  description
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

#ifndef FPOINT_H
#define FPOINT_H

#include "qpoint.h"
/**
  *@author Franz Schmid
  */

class FPoint
{
public: 
	FPoint() {};
	FPoint(float x, float y);
	FPoint(QPoint p);
	~FPoint() {};
	float x();
	float y();
	void setX(float x);
	void setY(float y);
	FPoint &operator+=( const FPoint &p );
	FPoint &operator-=( const FPoint &p );
private:
	float xp;
	float yp;
};

#endif
