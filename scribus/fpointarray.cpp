/***************************************************************************
                          fpointarray.cpp  -  description
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

#include "fpointarray.h"
#include <cstdarg>
#include <math.h>
#include "util.h"
//Added by qt3to4:
#include <Q3MemArray>

using namespace std;


FPointArray FPointArray::copy() const
{ 
	FPointArray tmp; 
	tmp.duplicate(*this);
	tmp.count = count;
	tmp.capacity = capacity;
	return tmp; 
}


FPointArray & FPointArray::operator=( const FPointArray &a )
{ 
	assign( a );
	count = a.count; 
	capacity = a.capacity;
	return *this; 
}


/* optimized for speed:
 *   never shrink
 *   when growing, try to double size
 *   if capacity permits, just increase count
 */
bool FPointArray::resize(uint newCount)
{
	if (newCount <= capacity) {
		count = newCount;
		return true;
	}
	else if (newCount <= 2*capacity && Q3MemArray<QPointF>::resize(2*capacity)) {
		capacity *= 2;
		count = newCount;
		return true;
	}
	else if (Q3MemArray<QPointF>::resize(newCount)) {
		capacity = newCount;
		count = newCount;
		return true;
	}
	else {
		sDebug(QString("Failed resize(): count=%1 capacity=%2 newCount=%3").arg(count).arg(capacity).arg(newCount));
		return false;
	}
}


bool FPointArray::setPoints( int nPoints, double firstx, double firsty, ... )
{
	va_list ap;
	if ( nPoints < 0 || !FPointArray::resize(nPoints) )
		return false;
	setPoint( 0, firstx, firsty );
	int i = 1;
	double x, y;
	nPoints--;
	va_start( ap, firsty );
	while ( nPoints-- )
	{
		x = static_cast<double>(va_arg( ap, double ));
		y = static_cast<double>(va_arg( ap, double ));
		setPoint( i++, x, y );
    }
	va_end( ap );
	return true;
}

bool FPointArray::putPoints( int index, int nPoints, double firstx, double firsty,  ... )
{
	va_list ap;
	if ( index + nPoints > static_cast<int>(count) )
	{
		if ( !FPointArray::resize(index + nPoints) )
			return false;
	}
	if ( nPoints <= 0 )
		return true;
	setPoint( index, firstx, firsty );		// set first point
	int i = index + 1;
	double x, y;
	nPoints--;
	va_start( ap, firsty );
	while ( nPoints-- )
	{
		x = static_cast<double>(va_arg(ap, double));
		y = static_cast<double>(va_arg(ap, double));
		setPoint( i++, x, y );
	}
	va_end( ap );
	return true;
}

bool FPointArray::putPoints( int index, int nPoints, const FPointArray & from, int fromIndex )
{
	if ( index + nPoints > static_cast<int>(count) )
	{	// extend array
		if ( !FPointArray::resize(index + nPoints) )
			return false;
	}
	if ( nPoints <= 0 )
		return true;
	Iterator p = begin();
	p += index;
	ConstIterator q = from.begin();
	q += fromIndex;
	while( --nPoints >= 0 )
	{
		*p++ = *q++;
    }
	return true;
}

void FPointArray::point(uint i, double *x, double *y)
{
//	QPointF p = QMemArray<QPointF>::at(i);
	ConstIterator p = begin();
	p += i;
	if (x)
		*x = p->x();
	if (y)
		*y = p->y();
}


QPoint FPointArray::pointQ(uint i)
{
//	QPointF p = QMemArray<QPointF>::at(i);
	ConstIterator p = begin();
	p += i;
	QPoint r(qRound(p->x()),qRound(p->y()));
	return r;
}

void FPointArray::translate( double dx, double dy )
{
	QPointF pt( dx, dy );
	Iterator pend = begin();
	pend += count;
	for (Iterator p = begin(); p != pend; p++)
	{
		if (p->x() < 900000)
			*p += pt;
	}
}

QPointF FPointArray::WidthHeight()
{
	if ( count == 0 )
		return QPointF( 0.0, 0.0 );		// null rectangle
	Iterator pd = begin();
	Iterator pend = begin();
	pend += count;
	double minx, maxx, miny, maxy;
	minx = maxx = pd->x();
	miny = maxy = pd->y();
	for ( ++pd; pd != pend; ++pd )
	{	// find min+max x and y
		if (pd->x() > 900000)
		{
			continue;
		}
		if ( pd->x() < minx )
			minx = pd->x();
		else
			if ( pd->x() > maxx )
		    	maxx = pd->x();
		if ( pd->y() < miny )
			miny = pd->y();
		else
			if ( pd->y() > maxy )
	    		maxy = pd->y();
    }
	return QPointF(maxx - minx,maxy - miny);
}

void FPointArray::map( QMatrix m )
{
	const double m11 = m.m11();
	const double m12 = m.m12();
	const double m21 = m.m21();
	const double m22 = m.m22();
	const double dx  = m.dx();
	const double dy  = m.dy();
	double mx, my;
	Iterator pend = begin();
	pend += count;
	for (Iterator p = begin(); p != pend; p++)
	{
		if (p->x() > 900000)
		{
			mx = p->x();
			my = p->y();
		}
		else
		{
			mx = m11 * p->x() + m21 * p->y() + dx;
			my = m22 * p->y() + m12 * p->x() + dy;
		}
		p->setX( mx );
		p->setY( my );
	}
}

void FPointArray::setMarker()
{
	addQuadPoint(999999.0, 999999.0,
				999999.0, 999999.0,
				999999.0, 999999.0,
				999999.0, 999999.0);
}

void FPointArray::addPoint(double x, double y)
{
	FPointArray::resize(count+1);
	setPoint(count-1, x, y);
}

void FPointArray::addPoint(QPointF p)
{
	FPointArray::resize(count+1);
	setPoint(count-1, p);
}


bool FPointArray::hasLastQuadPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
	int i = count-4;
	if (i < 0)
		return false;
	ConstIterator p = begin();
	p += i;
	if (p->x() != x1 || p->y() != y1)
		return false;
	++p; 
	if (p->x() != x2 || p->y() != y2)
		return false;
	++p; 
	if (p->x() != x3 || p->y() != y3)
		return false;
	++p; 
	if (p->x() != x4 || p->y() != y4)
		return false;
	
	return true;
}

void FPointArray::addQuadPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
	const int i = count;
	FPointArray::resize(count+4);
	Iterator p = begin();
	p += i;
	*p = QPointF(x1, y1);
	++p;
	*p = QPointF(x2, y2);
	++p;
	*p = QPointF(x3, y3);
	++p;
	*p = QPointF(x4, y4);
}

void FPointArray::addQuadPoint(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
	const int i = count;
	FPointArray::resize(count+4);
	Iterator p = begin();
	p += i;
	*p++ = p1;
	*p++ = p2;
	*p++ = p3;
	*p = p4;
}

double FPointArray::lenPathSeg(int seg)
{
	QPointF p1 = point(seg);
	QPointF k1 = point(seg+1);
	QPointF p2 = point(seg+2);
	QPointF k2 = point(seg+3);
	QPointF newP, oldP;
	double newLen = 1;
	double oldLen = 0;
	double ts = 0.5;
	double t = 0.5;
	int iter = 2;
	while (true)
	{
		oldP = p1;
		newLen = 0;
		for (int dx = 0; dx < iter; ++dx)
		{
			double tm = 1.0 - t;
			newP = ((tm * tm * tm) * p1) + (3 * t * (tm * tm) * k1) + (3 * t * t * tm * k2 + t * t * t * p2);
			newLen += sqrt(pow(newP.x()-oldP.x(),2.0)+pow(newP.y()-oldP.y(),2.0));
			oldP = newP;
			t += ts;
		}
		if (fabs(newLen - oldLen) < 0.01)
			break;
		oldLen = newLen;
		ts /= 2.0;
		iter *= 2;
		t = ts;
	}
	return newLen;
}

double FPointArray::lenPathDist(int seg, double t1, double t2)
{
	QPointF p1 = point(seg);
	QPointF k1 = point(seg+1);
	QPointF p2 = point(seg+2);
	QPointF k2 = point(seg+3);
	QPointF newP, oldP;
	double newLen = 0;
	double ts, t, tm;
	tm = 1.0 - t1;
	oldP = ((tm * tm * tm) * p1) + (3 * t1 * (tm * tm) * k1) + (3 * t1 * t1 * tm * k2 + t1 * t1 * t1 * p2);
	ts = (t2 - t1) / 100;
	t = t1 + ts;
	for (int dx = 0; dx < 99; ++dx)
	{
		tm = 1.0 - t;
		newP = ((tm * tm * tm) * p1) + (3 * t * (tm * tm) * k1) + (3 * t * t * tm * k2 + t * t * t * p2);
		newLen += sqrt(pow(newP.x()-oldP.x(),2.0)+pow(newP.y()-oldP.y(),2.0));
		oldP = newP;
		t += ts;
	}
	return newLen;
}

void FPointArray::pointTangentNormalAt( int seg, double t, QPointF* p, QPointF* tn, QPointF* n )
{
	// Calculate derivative if necessary.
	QPointF d;
	if( tn || n )
		pointDerivativesAt( seg, t, p, &d, 0L );
	else
		pointDerivativesAt( seg, t, p, 0L, 0L );
	// Normalize derivative.
	if( tn || n )
	{
		const double norm = sqrt( d.x() * d.x() + d.y() * d.y() );
		d = norm ? d * ( 1.0 / norm ) : QPointF( 0.0, 0.0 );
	}
	// Assign tangent vector.
	if( tn )
		*tn = d;
	// Calculate normal vector.
	if( n )
	{
		// Calculate vector product of "binormal" x tangent
		// (0,0,1) x (dx,dy,0), which is simply (dy,-dx,0).
		n->setX( d.y() );
		n->setY( -d.x() );
	}
	QPointF p1 = point(seg);
	QPointF k1 = point(seg+1);
	QPointF p2 = point(seg+2);
	QPointF k2 = point(seg+3);
	double tm = 1.0 - t;
	*p = ((tm * tm * tm) * p1) + (3 * t * (tm * tm) * k1) + (3 * (t * t) * tm * k2 + (t * t * t) * p2);
}

void FPointArray::pointDerivativesAt( int seg, double t, QPointF* p, QPointF* d1, QPointF* d2 )
{
	// Copy points.
	QPointF* q = new QPointF[ 4 ];
	q[ 0 ] = point(seg);
	q[ 1 ] = point(seg+1);
	q[ 3 ] = point(seg+2);
	q[ 2 ] = point(seg+3);
	// The De Casteljau algorithm.
	for( unsigned short j = 1; j <= 3; j++ )
	{
		for( unsigned short i = 0; i <= 3 - j; i++ )
		{
			q[ i ] = ( 1.0 - t ) * q[ i ] + t * q[ i + 1 ];
		}
		// Save second derivative now that we have it.
		if( j == 1 )
		{
			if( d2 )
				*d2 = 6 * ( q[ 2 ] - 2 * q[ 1 ] + q[ 0 ] );
		}
		// Save first derivative now that we have it.
		else if( j == 2 )
		{
			if( d1 )
				*d1 = 3 * ( q[ 1 ] - q[ 0 ] );
		}
	}
	// Save point.
	if( p )
		*p = q[ 0 ];
	delete[]( q );
	return;
}

QPointF TransformPoint(const double xp, const double yp, const double dx, const double dy,
		       const double rot, const double sx, const double sy, const bool invert)
{

	QMatrix ma;
	ma.translate(dx, dy);
	ma.scale(sx, sy);
	ma.rotate(rot);
	if (invert)
		ma = ma.invert();
	double x = ma.m11() * xp + ma.m21() * yp + ma.dx();
	double y = ma.m22() * yp + ma.m12() * xp + ma.dy();
	return QPointF(x, y);
}
