/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QPoint>
#include "scribusapi.h"
/**
  *@author Franz Schmid
  */

class SCRIBUS_API FPoint
{
public: 
	FPoint() : xp(0), yp(0) {};
	FPoint(double x, double y) : xp(x), yp(y) {};
	FPoint(const QPoint & p) : xp(p.x()), yp(p.y()) {};
	FPoint(const FPoint & p) : xp(p.xp), yp(p.yp) {};
	//Creates a transformed point, replaces ScribusView::transformPoint()
	FPoint(const double x, const double y, const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert=false);
//  ~FPoint() {};
	FPoint &  operator=(const FPoint & rhs);
	double x() const;
	double y() const;
	void setX(double x);
	void setY(double y);
	void setXY(double x, double y);
	bool operator==(const FPoint &rhs) const;
	bool operator!=(const FPoint &rhs) const;
	FPoint &operator+=( const FPoint &p );
	FPoint &operator-=( const FPoint &p );
	friend inline const FPoint operator+( const FPoint &, const FPoint & );
	friend inline const FPoint operator-( const FPoint &, const FPoint & );
	friend inline const FPoint operator*( const FPoint &, const double & );
	friend inline const FPoint operator*( const double &, const FPoint & );
	friend inline double  operator*( const FPoint &a, const FPoint &b );
	//Transform an existing point
	void transform(const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert);
	//Transform an existing point, return a new one
	FPoint transformPoint(const QMatrix& m, const bool invert) const;
	FPoint transformPoint(const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert) const;
	friend class FPointArray;

private:
	double xp;
	double yp;
};


inline const FPoint operator+( const FPoint &p1, const FPoint &p2 ) { 
	return FPoint(p1.xp+p2.xp, p1.yp+p2.yp); 
}

inline const FPoint operator-( const FPoint &p1, const FPoint &p2 ) { 
	return FPoint(p1.xp-p2.xp, p1.yp-p2.yp); 
}

inline const FPoint operator*( const FPoint &p, const double &c ) { 
	return FPoint(p.xp*c, p.yp*c); 
}

inline const FPoint operator*( const double &c, const FPoint &p ) { 
	return FPoint(p.xp*c, p.yp*c); 
}

inline double operator*( const FPoint &a, const FPoint &b ) {
	return a.xp * b.xp + a.yp * b.yp; 
}

inline FPoint &  FPoint::operator=(const FPoint & rhs)  { 
	xp = rhs.xp; 
	yp = rhs.yp; 
	return *this; 
}

inline double FPoint::x() const { 
	return xp; 
}

inline double FPoint::y() const { 
	return yp; 
}

inline void FPoint::setX(double x) { 
	xp = x; 
}

inline void FPoint::setY(double y) { 
	yp = y; 
}

inline void FPoint::setXY(double x, double y) { 
	xp = x;
	yp = y; 
}
 
inline FPoint & FPoint::operator+=( const FPoint &p ) { 
	xp += p.xp; 
	yp += p.yp; 
	return *this; 
}

inline FPoint & FPoint::operator-=( const FPoint &p ) { 
	xp -= p.xp; 
	yp -= p.yp; 
	return *this; 
}

#endif
