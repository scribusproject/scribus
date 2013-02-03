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

#ifndef __VGRADIENTEX_H__
#define __VGRADIENTEX_H__

#include <QList>
#include <QMatrix>

#include "fpoint.h"
#include "sccolor.h"
#include "scribusapi.h"
class ScribusDoc;
class VGradient;

class SCRIBUS_API VColorStopEx
{
public:
	VColorStopEx( double r, double m, ScColor c, double o, QString n, int s )
	{
		rampPoint = r;
		midPoint = m; 
		color = c;
		opacity = o; 
		name = n;
		shade = s;
	};
	
	VColorStopEx( const VColorStopEx& colorStop )
	{
		rampPoint = colorStop.rampPoint;
		midPoint = colorStop.midPoint;
		color = colorStop.color;
		opacity = colorStop.opacity;
		name = colorStop.name;
		shade = colorStop.shade;
	};

	ScColor color;

	// relative position of color point (0.0-1.0):
	double rampPoint;

	// relative position of midpoint (0.0-1.0)
	// between two ramp points. ignored for last VColorStop.
	double midPoint;
	double opacity;
	int shade;
	QString name;
	friend inline bool operator== ( VColorStopEx& s1, VColorStopEx& s2 )
	{ return s1.rampPoint == s2.rampPoint; };
}
; // VColorStopEx

// comparison function for use with stable_sort
bool compareStopsEx( const VColorStopEx* item1, const VColorStopEx* item2 );

class SCRIBUS_API VGradientEx
{
	// friend class VGradientWidget;

public:
	enum Type
	{
	    linear = 0,
	    radial = 1,
	    conic  = 2
	};

	enum RepeatMethod
	{
	    none    = 0,
	    reflect = 1,
	    repeat  = 2
	};

	VGradientEx( VGradientEx::Type type = linear );
	VGradientEx( const VGradientEx& gradient );
	VGradientEx( const VGradient& gradient, ScribusDoc& doc );
	~VGradientEx();

	VGradientEx& operator=(const VGradientEx& gradient);

	VGradientEx::Type type() const { return m_type; }
	void setType( VGradientEx::Type type ) { m_type = type; }

	VGradientEx::RepeatMethod repeatMethod() const { return m_repeatMethod; }
	void setRepeatMethod( VGradientEx::RepeatMethod repeatMethod ) { m_repeatMethod = repeatMethod; }

	const QList<VColorStopEx*> colorStops() const;
	void addStop( const VColorStopEx& colorStop );
	void addStop( const ScColor &color, double rampPoint, double midPoint, double opa, QString name = "", int shade = 100 );
	void removeStop( VColorStopEx& colorStop );
	void removeStop( uint n );
	void clearStops();
	uint Stops() { return m_colorStops.count(); }

	FPoint origin() const { return m_origin; }
	void setOrigin( const FPoint &origin ) { m_origin = origin; }

	FPoint focalPoint() const { return m_focalPoint; }
	void setFocalPoint( const FPoint &focalPoint ) { m_focalPoint = focalPoint; }

	FPoint vector() const { return m_vector; }
	void setVector( const FPoint &vector ) { m_vector = vector; }

	void transform( const QMatrix& m );

protected:
	QList<VColorStopEx*>        m_colorStops;

	int  compareItems( const VColorStopEx* item1, const VColorStopEx* item2 ) const;
	void inSort( VColorStopEx* d );

private:
	VGradientEx::Type         m_type			: 2;
	VGradientEx::RepeatMethod m_repeatMethod	: 2;

	// coordinates:
	FPoint m_origin;
	FPoint m_focalPoint;
	FPoint m_vector;
}
; // VGradientEx

#endif /* __VGRADIENT_H__ */
