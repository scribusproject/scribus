/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This file is part of the KDE project
   Copyright (C) 2002, The Karbon Developers
 
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <QtAlgorithms>

#include "vgradientex.h"
#include "vgradient.h"
#include "scribusdoc.h"

// comparison function for use with stable_sort
bool compareStopsEx( const VColorStopEx* item1, const VColorStopEx* item2 )
{
	double r1 = item1->rampPoint;
	double r2 = item2->rampPoint;
	return ( r1 < r2 ? true : false );
}

int VGradientEx::compareItems( const VColorStopEx* item1, const VColorStopEx* item2 ) const
{
	double r1 = item1->rampPoint;
	double r2 = item2->rampPoint;

	return ( r1 == r2 ? 0 : r1 < r2 ? -1 : 1 );
} // VGradientEx::VColorStopList::compareItems

void VGradientEx::inSort( VColorStopEx* d )
{
	int index = 0;
	register VColorStopEx *n = m_colorStops.value(index);
	while (n && compareItems(n,d) <= 0)
	{
		n = m_colorStops.value(index);
		++index;
	}
	m_colorStops.insert( qMin(index, m_colorStops.size()), d );
}

VGradientEx::VGradientEx( VGradientEx::Type type ) : m_type( type )
{
	// set up dummy gradient
	addStop( ScColor(255,0,0) , 0.0, 0.5, 1.0 );
	addStop( ScColor(255,255,0) , 1.0, 0.5, 1.0 );

	setOrigin( FPoint( 0, 0 ) );
	setVector( FPoint( 0, 50 ) );
	setRepeatMethod( VGradientEx::reflect );
}

VGradientEx::VGradientEx( const VGradientEx& gradient )
{
	m_origin		= gradient.m_origin;
	m_focalPoint	= gradient.m_focalPoint;
	m_vector		= gradient.m_vector;
	m_type			= gradient.m_type;
	m_repeatMethod	= gradient.m_repeatMethod;

	clearStops();
	QList<VColorStopEx*> cs = gradient.colorStops();
	qStableSort( cs.begin(), cs.end(), compareStopsEx);
	for( int i = 0; i < cs.count(); ++i)
		m_colorStops.append( new VColorStopEx( *cs[i] ) );
}

VGradientEx::VGradientEx( const VGradient& gradient, ScribusDoc& doc )
{
	m_origin		= gradient.origin();
	m_focalPoint	= gradient.focalPoint();
	m_vector		= gradient.vector();
	m_type			= (VGradientEx::Type) gradient.type();
	m_repeatMethod	= (VGradientEx::RepeatMethod) gradient.repeatMethod();

	while (!m_colorStops.isEmpty())
		delete m_colorStops.takeFirst();
	QList<VColorStop*> stops = gradient.colorStops();
	qStableSort( stops.begin(), stops.end(), compareStops);
	for( int i = 0; i < stops.count(); ++i)
	{
		VColorStop stop( *stops[i] );
		ScColor color = doc.PageColors[stop.name];
		double ramp = stop.rampPoint;
		double mid = stop.midPoint;
		double opacity = stop.opacity;
		int shade = stop.shade;
		QString name = stop.name;
		m_colorStops.append( new VColorStopEx(ramp, mid, color, opacity, name, shade) );
	}
}

VGradientEx& VGradientEx::operator=( const VGradientEx& gradient )
{
	if ( this == &gradient )
		return *this;

	m_origin		= gradient.m_origin;
	m_focalPoint	= gradient.m_focalPoint;
	m_vector		= gradient.m_vector;
	m_type			= gradient.m_type;
	m_repeatMethod	= gradient.m_repeatMethod;

	clearStops();
	QList<VColorStopEx*> cs = gradient.colorStops();
	qStableSort( cs.begin(), cs.end(), compareStopsEx);
	for( int i = 0; i < cs.count(); ++i )
		m_colorStops.append( new VColorStopEx( *cs[i] ) );
	return *this;
} // VGradientEx::operator=

VGradientEx::~VGradientEx()
{
	clearStops();
}

const QList<VColorStopEx*> VGradientEx::colorStops() const
{
	return m_colorStops;
} // VGradientEx::colorStops()

void
VGradientEx::clearStops()
{
	while (!m_colorStops.isEmpty())
		delete m_colorStops.takeFirst();
}

void
VGradientEx::addStop( const VColorStopEx& colorStop )
{
	inSort( new VColorStopEx( colorStop ) );
} // VGradientEx::addStop

void
VGradientEx::addStop( const ScColor &color, double rampPoint, double midPoint, double opa, QString name, int shade )
{
	// Clamping between 0.0 and 1.0
	rampPoint = qMax( 0.0, rampPoint );
	rampPoint = qMin( 1.0, rampPoint );
	// Clamping between 0.0 and 1.0
	midPoint = qMax( 0.0, midPoint );
	midPoint = qMin( 1.0, midPoint );

	inSort( new VColorStopEx( rampPoint, midPoint, color, opa, name, shade ) );
}

void VGradientEx::removeStop( VColorStopEx& colorstop )
{
	int n = m_colorStops.indexOf(&colorstop);
	delete m_colorStops.takeAt(n);
}

void VGradientEx::removeStop( uint n )
{
	delete m_colorStops.takeAt(n);
}

void VGradientEx::transform( const QMatrix &m )
{
	double mx, my;
	mx = m.m11() * m_origin.x() + m.m21() * m_origin.y() + m.dx();
	my = m.m22() * m_origin.y() + m.m12() * m_origin.x() + m.dy();
	m_origin = FPoint(mx, my);
	mx = m.m11() * m_focalPoint.x() + m.m21() * m_focalPoint.y() + m.dx();
	my = m.m22() * m_focalPoint.y() + m.m12() * m_focalPoint.x() + m.dy();
	m_focalPoint = FPoint(mx, my);
	mx = m.m11() * m_vector.x() + m.m21() * m_vector.y() + m.dx();
	my = m.m22() * m_vector.y() + m.m12() * m_vector.x() + m.dy();
	m_vector = FPoint(mx, my);
}
