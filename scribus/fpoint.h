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

#include <QTransform>
#include <QPoint>
#include <QPointF>
#include "scribusapi.h"

/**
  * @author Franz Schmid
  * @brief A point with floating point precision
  *
  * One of the advantage of FPoint vs QPointF is that FPoint has consistent precision across platforms :
  * the coordinates are systematically stored as double. QPointF uses the qreal typedef which is sometime double,
  * sometime float.
  *
  * FPoint has also some coordinate transformation functionalities not provided by QPointF. 
  */

class SCRIBUS_API FPoint
{
public: 
	FPoint() : m_xp(0), m_yp(0) {};
	FPoint(double x, double y) : m_xp(x), m_yp(y) {};
	FPoint(const QPoint & p) : m_xp(p.x()), m_yp(p.y()) {};
	FPoint(const FPoint & p) : m_xp(p.m_xp), m_yp(p.m_yp) {};
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
	FPoint transformPoint(const QTransform& m, const bool invert) const;
	FPoint transformPoint(const double dx, const double dy, const double rot, const double sx, const double sy, const bool invert) const;
	/// Returns a copy of the point as a QPointF.
	QPointF toQPointF() const { return QPointF(m_xp, m_yp); }
	friend class FPointArray;
	bool isNull() const;

private:
	double m_xp;
	double m_yp;
};


inline const FPoint operator+( const FPoint &p1, const FPoint &p2 ) { 
	return FPoint(p1.m_xp+p2.m_xp, p1.m_yp+p2.m_yp);
}

inline const FPoint operator-( const FPoint &p1, const FPoint &p2 ) { 
	return FPoint(p1.m_xp-p2.m_xp, p1.m_yp-p2.m_yp);
}

inline const FPoint operator*( const FPoint &p, const double &c ) { 
	return FPoint(p.m_xp*c, p.m_yp*c);
}

inline const FPoint operator*( const double &c, const FPoint &p ) { 
	return FPoint(p.m_xp*c, p.m_yp*c);
}

inline double operator*( const FPoint &a, const FPoint &b ) {
	return a.m_xp * b.m_xp + a.m_yp * b.m_yp;
}

inline FPoint &  FPoint::operator=(const FPoint & rhs)  { 
	m_xp = rhs.m_xp;
	m_yp = rhs.m_yp;
	return *this; 
}

inline double FPoint::x() const { 
	return m_xp;
}

inline double FPoint::y() const { 
	return m_yp;
}

inline void FPoint::setX(double x) { 
	m_xp = x;
}

inline void FPoint::setY(double y) { 
	m_yp = y;
}

inline void FPoint::setXY(double x, double y) { 
	m_xp = x;
	m_yp = y;
}
 
inline FPoint & FPoint::operator+=( const FPoint &p ) { 
	m_xp += p.m_xp;
	m_yp += p.m_yp;
	return *this; 
}

inline FPoint & FPoint::operator-=( const FPoint &p ) { 
	m_xp -= p.m_xp;
	m_yp -= p.m_yp;
	return *this; 
}

#endif
