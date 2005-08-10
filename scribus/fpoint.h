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
#include "scribusapi.h"
/**
  *@author Franz Schmid
  */

class SCRIBUS_API FPoint
{
public: 
	FPoint() {};
	FPoint(double x, double y) : xp(x), yp(y) {};
	FPoint(const QPoint & p) : xp(p.x()), yp(p.y()) {};
	FPoint(const FPoint & p) : xp(p.xp), yp(p.yp) {};
//  ~FPoint() {};
	FPoint &  operator=(const FPoint & rhs)  { xp = rhs.xp; yp = rhs.yp; return *this; };
	double x() const { return xp; };
	double y() const { return yp; };
	void setX(double x) { xp = x; };
	void setY(double y) { yp = y; };
	void setXY(double x, double y) { xp = x; yp = y; };
	bool operator==(const FPoint &rhs) const;
	bool operator!=(const FPoint &rhs) const;
	FPoint &operator+=( const FPoint &p ) { xp += p.xp; yp += p.yp; return *this; };
	FPoint &operator-=( const FPoint &p ) { xp -= p.xp; yp -= p.yp; return *this; };
	friend inline const FPoint operator+( const FPoint &, const FPoint & );
	friend inline const FPoint operator-( const FPoint &, const FPoint & );
	friend inline const FPoint operator*( const FPoint &, const double & );
	friend inline const FPoint operator*( const double &, const FPoint & );
	friend inline const double  operator*( const FPoint &a, const FPoint &b );
	friend class FPointArray;

private:
	double xp;
	double yp;
};

inline const FPoint operator+( const FPoint &p1, const FPoint &p2 )
{ return FPoint(p1.xp+p2.xp, p1.yp+p2.yp); }

inline const FPoint operator-( const FPoint &p1, const FPoint &p2 )
{ return FPoint(p1.xp-p2.xp, p1.yp-p2.yp); }

inline const FPoint operator*( const FPoint &p, const double &c )
{ return FPoint(p.xp*c, p.yp*c); }

inline const FPoint operator*( const double &c, const FPoint &p )
{ return FPoint(p.xp*c, p.yp*c); }

inline const double operator*( const FPoint &a, const FPoint &b )
{ return a.xp * b.xp + a.yp * b.yp; }

#endif
