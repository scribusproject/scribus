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
	else if (newCount <= 2*capacity && QMemArray<FPoint>::resize(2*capacity)) {
		capacity *= 2;
		count = newCount;
		return true;
	}
	else if (QMemArray<FPoint>::resize(newCount)) {
		capacity = newCount;
		count = newCount;
		return true;
	}
	else {
		qDebug(QString("failed resize(): count=%1 capacity=%2 newCount=%3").arg(count).arg(capacity).arg(newCount));
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
//	FPoint p = QMemArray<FPoint>::at(i);
	ConstIterator p = begin();
	p += i;
	if (x)
		*x = p->xp;
	if (y)
		*y = p->yp;
}


QPoint FPointArray::pointQ(uint i)
{
//	FPoint p = QMemArray<FPoint>::at(i);
	ConstIterator p = begin();
	p += i;
	QPoint r(qRound(p->xp),qRound(p->yp));
	return r;
}

void FPointArray::translate( double dx, double dy )
{
	FPoint pt( dx, dy );
	Iterator pend = begin();
	pend += count;
	for (Iterator p = begin(); p != pend; p++)
	{
		if (p->xp < 900000)
			*p += pt;
	}
}

FPoint FPointArray::WidthHeight()
{
	if ( count == 0 )
		return FPoint( 0.0, 0.0 );		// null rectangle
	Iterator pd = begin();
	Iterator pend = begin();
	pend += count;
	double minx, maxx, miny, maxy;
	minx = maxx = pd->xp;
	miny = maxy = pd->yp;
	for ( ++pd; pd != pend; ++pd )
	{	// find min+max x and y
		if (pd->xp > 900000)
		{
			continue;
		}
		if ( pd->xp < minx )
			minx = pd->xp;
		else
			if ( pd->xp > maxx )
		    	maxx = pd->xp;
		if ( pd->y() < miny )
			miny = pd->yp;
		else
			if ( pd->yp > maxy )
	    		maxy = pd->yp;
    }
	return FPoint(maxx - minx,maxy - miny);
}

void FPointArray::map( QWMatrix m )
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
		if (p->xp > 900000)
		{
			mx = p->xp;
			my = p->yp;
		}
		else
		{
			mx = m11 * p->xp + m21 * p->yp + dx;
			my = m22 * p->yp + m12 * p->xp + dy;
		}
		p->xp = mx;
		p->yp = my;
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

void FPointArray::addPoint(FPoint p)
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
	if (p->xp != x1 || p->yp != y1)
		return false;
	++p; 
	if (p->xp != x2 || p->yp != y2)
		return false;
	++p; 
	if (p->xp != x3 || p->yp != y3)
		return false;
	++p; 
	if (p->xp != x4 || p->yp != y4)
		return false;
	
	return true;
}

void FPointArray::addQuadPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
	const int i = count;
	FPointArray::resize(count+4);
	Iterator p = begin();
	p += i;
	p->setXY(x1, y1);
	++p;
	p->setXY(x2, y2);
	++p;
	p->setXY(x3, y3);
	++p;
	p->setXY(x4, y4);
}

void FPointArray::addQuadPoint(FPoint p1, FPoint p2, FPoint p3, FPoint p4)
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
	FPoint p1 = point(seg);
	FPoint k1 = point(seg+1);
	FPoint p2 = point(seg+2);
	FPoint k2 = point(seg+3);
	FPoint newP, oldP;
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
	FPoint p1 = point(seg);
	FPoint k1 = point(seg+1);
	FPoint p2 = point(seg+2);
	FPoint k2 = point(seg+3);
	FPoint newP, oldP;
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

void FPointArray::pointTangentNormalAt( int seg, double t, FPoint* p, FPoint* tn, FPoint* n )
{
	// Calculate derivative if necessary.
	FPoint d;
	if( tn || n )
		pointDerivativesAt( seg, t, p, &d, 0L );
	else
		pointDerivativesAt( seg, t, p, 0L, 0L );
	// Normalize derivative.
	if( tn || n )
	{
		const double norm = sqrt( d.x() * d.x() + d.y() * d.y() );
		d = norm ? d * ( 1.0 / norm ) : FPoint( 0.0, 0.0 );
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
	FPoint p1 = point(seg);
	FPoint k1 = point(seg+1);
	FPoint p2 = point(seg+2);
	FPoint k2 = point(seg+3);
	double tm = 1.0 - t;
	*p = ((tm * tm * tm) * p1) + (3 * t * (tm * tm) * k1) + (3 * (t * t) * tm * k2 + (t * t * t) * p2);
}

void FPointArray::pointDerivativesAt( int seg, double t, FPoint* p, FPoint* d1, FPoint* d2 )
{
	// Copy points.
	FPoint* q = new FPoint[ 4 ];
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
