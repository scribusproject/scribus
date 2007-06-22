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
#include <QMutableListIterator>
#include <algorithm>

// colorStop comparison function for stable_sort function
bool compareStops( const VColorStop* item1, const VColorStop* item2 ) 
{
	double r1 = item1->rampPoint;
	double r2 = item2->rampPoint;
	return ( r1 < r2 ? true : false );
}

int VGradient::VColorStopList::compareItems(VColorStop* item1, VColorStop* item2 ) const
{
	double r1 = item1->rampPoint;
	double r2 = item2->rampPoint;

	return ( r1 == r2 ? 0 : r1 < r2 ? -1 : 1 );
} // VGradient::VColorStopList::compareItems


void VGradient::VColorStopList::inSort( VColorStop* d )
{
	int index = 0;
	register VColorStop *n = value(index);
	while (n && compareItems(n,d) <= 0)
	{
		n = value(index);
		index++;
	}
	insert( index, d );
}

VGradient::VGradient( VGradientType type ) : m_type( type )
{
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
	m_origin		= gradient.m_origin;
	m_focalPoint	= gradient.m_focalPoint;
	m_vector		= gradient.m_vector;
	m_type			= gradient.m_type;
	m_repeatMethod	= gradient.m_repeatMethod;
	while (!m_colorStops.isEmpty())
		delete m_colorStops.takeFirst();
	QVector<VColorStop*> cs = gradient.colorStops();
	std::stable_sort(cs.data(), cs.data() + cs.count(), compareStops);
	for( int i = 0; i < cs.count(); ++i)
		m_colorStops.append( new VColorStop( *cs[i] ) );
} // VGradient::VGradient

VGradient::~VGradient()
{
	clearStops();
}

VGradient& VGradient::operator=( const VGradient& gradient )
{
	if ( this == &gradient )
		return *this;

	m_origin		= gradient.m_origin;
	m_focalPoint	= gradient.m_focalPoint;
	m_vector		= gradient.m_vector;
	m_type			= gradient.m_type;
	m_repeatMethod	= gradient.m_repeatMethod;

	while (!m_colorStops.isEmpty())
		delete m_colorStops.takeFirst();
	QVector<VColorStop*> cs = gradient.colorStops();
	std::stable_sort(cs.data(), cs.data() + cs.count(), compareStops);
	for( int i = 0; i < cs.count(); ++i )
		m_colorStops.append( new VColorStop( *cs[i] ) );
	return *this;
} // VGradient::operator=

const QVector<VColorStop*> VGradient::colorStops() const
{
	QVector<VColorStop*> v;
	v.resize(m_colorStops.size());
	qCopy(m_colorStops.begin(), m_colorStops.end(), v.begin());
	return v;
} // VGradient::colorStops()

void
VGradient::clearStops()
{
	while (!m_colorStops.isEmpty())
		delete m_colorStops.takeFirst();
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
	rampPoint = qMax( 0.0, rampPoint );
	rampPoint = qMin( 1.0, rampPoint );
	// Clamping between 0.0 and 1.0
	midPoint = qMax( 0.0, midPoint );
	midPoint = qMin( 1.0, midPoint );

	m_colorStops.inSort( new VColorStop( rampPoint, midPoint, color, opa, name, shade ) );
}

void VGradient::removeStop( VColorStop& colorstop )
{
	int n = m_colorStops.indexOf(&colorstop);
	delete m_colorStops.takeAt(n);
}

void VGradient::removeStop( uint n )
{
	delete m_colorStops.takeAt(n);
}

void VGradient::filterStops(void)
{
	VColorStop* colorStop = NULL;
	bool zeroFound = false;
	QMutableListIterator<VColorStop*> i(m_colorStops);
	i.toBack();
	while (i.hasPrevious())
	{
		colorStop = i.previous();
		if (colorStop->rampPoint == 0.0 && zeroFound)
		{
			delete i.value();
			i.remove();
		}
		else if (colorStop->rampPoint == 0.0)
			zeroFound = true;
	}
	i.toFront();
	while (i.hasNext())
	{
		colorStop = i.next();
		if (colorStop->rampPoint == 0.0 && zeroFound)
		{
			delete i.value();
			i.remove();
		}
		else if (colorStop->rampPoint == 0.0)
			zeroFound = true;
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
