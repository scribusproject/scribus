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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
#include "vgradient.h"
#include <algorithm>

// colorStop comparison function for stable_sort function
bool compareStops( const VColorStop* item1, const VColorStop* item2 ) 
{
	double r1 = item1->rampPoint;
	double r2 = item2->rampPoint;
	return ( r1 < r2 ? true : false );
}

int VGradient::VColorStopList::compareItems( Q3PtrCollection::Item item1, Q3PtrCollection::Item item2 ) const
{
	double r1 = ( (VColorStop*)item1 )->rampPoint;
	double r2 = ( (VColorStop*)item2 )->rampPoint;

	return ( r1 == r2 ? 0 : r1 < r2 ? -1 : 1 );
} // VGradient::VColorStopList::compareItems


void VGradient::VColorStopList::inSort( Q3PtrCollection::Item d )
{
	int index = 0;
	first();
	register VColorStop *n = first();
	while ( n && compareItems((Q3PtrCollection::Item) n,d) <= 0 ){ // find position in list
		n = next();
		index++;
	}
	insertAt( index, d );
}

VGradient::VGradient( VGradientType type )
		: m_type( type )
{
	m_colorStops.setAutoDelete( true );

	// set up dummy gradient
	QColor color;

	color = QColor(255,0,0);
	addStop( color, 0.0, 0.5, 1.0 );

	color = QColor(255,255,0);
	addStop( color, 1.0, 0.5, 1.0 );

	setOrigin( FPoint( 0, 0 ) );
	setVector( FPoint( 0, 50 ) );
	setRepeatMethod( VGradient::reflect );
}

VGradient::VGradient( const VGradient& gradient )
{
	m_colorStops.setAutoDelete( true );

	m_origin		= gradient.m_origin;
	m_focalPoint	= gradient.m_focalPoint;
	m_vector		= gradient.m_vector;
	m_type			= gradient.m_type;
	m_repeatMethod	= gradient.m_repeatMethod;

	m_colorStops.clear();
	Q3PtrVector<VColorStop> cs = gradient.colorStops();
	std::stable_sort(cs.data(), cs.data() + cs.count(), compareStops);
	for( uint i = 0; i < cs.count(); ++i)
		m_colorStops.append( new VColorStop( *cs[i] ) );
} // VGradient::VGradient

VGradient& VGradient::operator=( const VGradient& gradient )
{
	m_colorStops.setAutoDelete( true );

	if ( this == &gradient )
		return *this;

	m_origin		= gradient.m_origin;
	m_focalPoint	= gradient.m_focalPoint;
	m_vector		= gradient.m_vector;
	m_type			= gradient.m_type;
	m_repeatMethod	= gradient.m_repeatMethod;

	m_colorStops.clear();
	Q3PtrVector<VColorStop> cs = gradient.colorStops();
	std::stable_sort(cs.data(), cs.data() + cs.count(), compareStops);
	for( uint i = 0; i < cs.count(); ++i )
		m_colorStops.append( new VColorStop( *cs[i] ) );
	return *this;
} // VGradient::operator=

const Q3PtrVector<VColorStop> VGradient::colorStops() const
{
	Q3PtrVector<VColorStop> v;
	m_colorStops.toVector( &v );
	v.setAutoDelete( false );
	return v;
} // VGradient::colorStops()

void
VGradient::clearStops()
{
	m_colorStops.clear();
}

void
VGradient::addStop( const VColorStop& colorStop )
{
	m_colorStops.inSort( new VColorStop( colorStop ) );
} // VGradient::addStop

void
VGradient::addStop( const QColor &color, double rampPoint, double midPoint, double opa, QString name, int shade )
{
	// Clamping between 0.0 and 1.0
	rampPoint = QMAX( 0.0, rampPoint );
	rampPoint = QMIN( 1.0, rampPoint );
	// Clamping between 0.0 and 1.0
	midPoint = QMAX( 0.0, midPoint );
	midPoint = QMIN( 1.0, midPoint );

	m_colorStops.inSort( new VColorStop( rampPoint, midPoint, color, opa, name, shade ) );
}

void VGradient::removeStop( const VColorStop& colorstop )
{
	m_colorStops.remove( &colorstop );
}

void VGradient::removeStop( uint n )
{
	m_colorStops.remove( n );
}

void VGradient::filterStops(void)
{
	VColorStop* colorStop = NULL;
	bool zeroFound = false;
	colorStop = m_colorStops.last();
	while(colorStop != NULL)
	{
		if (colorStop->rampPoint == 0.0 && zeroFound)
			m_colorStops.remove();
		else if (colorStop->rampPoint == 0.0)
			zeroFound = true;
		colorStop = m_colorStops.prev();
	}
	bool oneFound = false;
	colorStop = m_colorStops.first();
	while(colorStop != NULL)
	{
		if (colorStop->rampPoint == 1.0 && oneFound)
		{
			bool isLast = (colorStop == m_colorStops.getLast());
			m_colorStops.remove();
			if (isLast)
				colorStop = m_colorStops.next();
			colorStop = m_colorStops.current();
		}
		else if (colorStop->rampPoint == 1.0)
		{
			oneFound = true;
			colorStop = m_colorStops.next();
		}
		else
			colorStop = m_colorStops.next();
	}
}

void VGradient::transform( const QMatrix &m )
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
