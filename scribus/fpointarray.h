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

#include <q3memarray.h>
#include <qmatrix.h>
#include <qpoint.h>
#include "scribusapi.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API FPointArray : private Q3MemArray<QPointF>
{
public: 
	FPointArray() : count(0), capacity(0) {};
	FPointArray(int size) : Q3MemArray<QPointF>(size), count(size), capacity(size) {};
	FPointArray(const FPointArray &a) : Q3MemArray<QPointF>(a), count(a.count), capacity(a.capacity) {};
	uint size() const { return count; };
	bool resize(uint newCount);
	void setPoint(uint i, double x, double y) { Iterator p = begin(); p+=i; p->setX(x); p->setY(y); };
	void setPoint(uint i, QPointF p) {	setPoint(i, p.x(), p.y()); };
	bool setPoints( int nPoints, double firstx, double firsty, ... );
	bool putPoints( int index, int nPoints, double firstx, double firsty,  ... );
	bool putPoints( int index, int nPoints, const FPointArray & from, int fromIndex = 0 );
	void point(uint i, double *x, double *y);
	const QPointF & point(uint i) { ConstIterator p = begin(); p+=i; return *p; };
	QPoint pointQ(uint i);
	void translate( double dx, double dy );
	QPointF WidthHeight();
	void map(QMatrix m);
	FPointArray &operator=( const FPointArray &a );
	FPointArray copy() const;
	void setMarker();
	void addPoint(double x, double y);
	void addPoint(QPointF p);
	bool hasLastQuadPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	void addQuadPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	void addQuadPoint(QPointF p1, QPointF p2, QPointF p3, QPointF p4);
	double lenPathSeg(int seg);
	double lenPathDist(int seg, double t1, double t2);
	void pointTangentNormalAt( int seg, double t, QPointF* p, QPointF* tn, QPointF* n );
	void pointDerivativesAt( int seg, double t, QPointF* p, QPointF* d1, QPointF* d2 );
	~FPointArray() {};
private:
	uint count;
	uint capacity;
};


QPointF TransformPoint(const double x, const double y,
		       const double dx, const double dy, const double rot,
		       const double sx, const double sy, const bool invert=false);

#endif
