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

#include <qpoint.h>
/**
  *@author Franz Schmid
  */

class FPoint
{
public: 
	FPoint() {};
	FPoint(double x, double y);
	FPoint(QPoint p);
	~FPoint() {};
	double x();
	double y();
	void setX(double x);
	void setY(double y);
	bool operator==(const FPoint &rhs);
	bool operator!=(const FPoint &rhs);
	FPoint &operator+=( const FPoint &p );
	FPoint &operator-=( const FPoint &p );

private:
	double xp;
	double yp;
};

#endif
