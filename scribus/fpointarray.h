/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          fpointarray.h  -  description
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

#ifndef FPOINTARRAY_H
#define FPOINTARRAY_H

#include <qmemarray.h>
#include <qwmatrix.h>
#include <qpoint.h>
#include "scribusapi.h"
#include "fpoint.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API FPointArray : private QMemArray<FPoint>
{
public: 
	FPointArray() : count(0), capacity(0) {};
	FPointArray(int size) : QMemArray<FPoint>(size), count(size), capacity(size) {};
	FPointArray(const FPointArray &a) : QMemArray<FPoint>(a), count(a.count), capacity(a.capacity) {};
	uint size() const { return count; };
	bool resize(uint newCount);
	void setPoint(uint i, double x, double y) { Iterator p = begin(); p+=i; p->xp = x; p->yp = y; };
	void setPoint(uint i, FPoint p) {	setPoint(i, p.xp, p.yp); };
	bool setPoints( int nPoints, double firstx, double firsty, ... );
	bool putPoints( int index, int nPoints, double firstx, double firsty,  ... );
	bool putPoints( int index, int nPoints, const FPointArray & from, int fromIndex = 0 );
	void point(uint i, double *x, double *y);
	const FPoint & point(uint i) { ConstIterator p = begin(); p+=i; return *p; };
	QPoint pointQ(uint i);
	void translate( double dx, double dy );
	FPoint WidthHeight();
	void map(QWMatrix m);
	FPointArray &operator=( const FPointArray &a );
	FPointArray copy() const;
	void setMarker();
	void addPoint(double x, double y);
	void addPoint(FPoint p);
	bool hasLastQuadPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	void addQuadPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	void addQuadPoint(FPoint p1, FPoint p2, FPoint p3, FPoint p4);
	double lenPathSeg(int seg);
	double lenPathDist(int seg, double t1, double t2);
	void pointTangentNormalAt( int seg, double t, FPoint* p, FPoint* tn, FPoint* n );
	void pointDerivativesAt( int seg, double t, FPoint* p, FPoint* d1, FPoint* d2 );
	~FPointArray() {};
private:
	uint count;
	uint capacity;
};

#endif
