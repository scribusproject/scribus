/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          util.cpp  -  description
                             -------------------
    begin                : Fri Sep 14 2001
    copyright            : (C) 2001 by Franz Schmid
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

#include "colorutil.h"

#include <qbitmap.h>
#include <qdatetime.h> 
#include <qpainter.h>
#include <qcheckbox.h>
// #include <algorithm>
// #include <cstdlib>
// #include <cmath>

#include "scconfig.h"

//#ifdef HAVE_UNISTD_H
//#include <unistd.h>
//#endif

#if defined(_WIN32)
#if defined(_MSC_VER)
#define __STDC__ 1 // hack to get md5_buffer correctly identified
#endif
#include <valarray>
#include <windows.h>
#endif

#include "md5.h"

// #include <setjmp.h>
// #include "commonstrings.h"
// #include "pagestructs.h"
// #include "prefsfile.h"
// #include "prefscontext.h"
// #include "prefstable.h"
// #include "prefsmanager.h"
// #include "qprocess.h"
// #include "scmessagebox.h"
#include "scribusdoc.h"
#include "scpixmapcache.h"
#include "commonstrings.h"
// #include "scpaths.h"
// #include "text/nlsconfig.h"
#include "util.h"
#include "sccolorengine.h"

extern "C"
{
#define XMD_H           // shut JPEGlib up
#if defined(Q_OS_UNIXWARE)
#  define HAVE_BOOLEAN  // libjpeg under Unixware seems to need this
#endif
#include <jpeglib.h>
#include <jerror.h>
#undef HAVE_STDLIB_H
#ifdef const
#  undef const          // remove crazy C hackery in jconfig.h
#endif
}

#include CMS_INC
using namespace std;

void handleOldColorShade(ScribusDoc* doc, QString& colName, int& shade)
{
	int r, g, b;
	bool found = false;
	if( colName.isEmpty() || colName == CommonStrings::None )
		return;
	ScColor& scCol1 = doc->PageColors[colName];
	if( (shade == 100) || (scCol1.getColorModel() != colorModelRGB) )
		return;
	scCol1.getRGB(&r, &g, &b);
	QColor col1 = getOldColorShade(r, g, b, shade), col2;
	ColorList::Iterator it, itEnd = doc->PageColors.end();
	for( it = doc->PageColors.begin(); it != itEnd; it++)
	{
		if ( it.data().getColorModel() == colorModelRGB )
		{
			it.data().getRGB(&r, &g, &b);
			col2.setRgb(r, g, b);
			if( col1 == col2 )
			{
				found = true;
				break;
			}
		}
	}
	if(!found)
	{
		ScColor tmp;
		tmp.fromQColor(col1);
		colName = QString("%1 %2%").arg(colName).arg(shade);
		doc->PageColors.insert(colName, tmp);
	}
	else
		colName = it.key();
	shade = 100;
}

QColor getOldColorShade(const QColor& color, int shade)
{
	int r, g, b;
	color.getRgb(&r, &g, &b);
	return getOldColorShade(r, g, b, shade);
}

QColor SCRIBUS_API getOldColorShade(uchar red, uchar green, uchar blue, int shade)
{
	int h, s, v, snew;
	QColor color(red, green, blue);
	color.hsv(&h, &s, &v);
	if (red == green && green == blue)
	{
		snew = 255 - ((255 - v) * shade / 100);
		color.setHsv(h, s, snew);
	}
	else
	{
		snew = s * shade / 100;
		color.setHsv(h, snew, v);
	}
	return color;
}

QImage ProofImage(QImage *Image, ScribusDoc* doc)
{
	QImage out = Image->copy();
	bool cmsUse = doc ? doc->HasCMS : false;
	bool softProofing = doc ? doc->SoftProofing : false;
	if (cmsUse && softProofing)
	{
		int outheight=out.height();
		for (int i=0; i < outheight; ++i)
		{
			LPBYTE ptr = out.scanLine(i);
			cmsDoTransform(doc->stdProofImg, ptr, ptr, out.width());
		}
	}
	else
	{
		if (cmsUse)
		{
			int outheight=out.height();
			for (int i=0; i < outheight; ++i)
			{
				LPBYTE ptr = out.scanLine(i);
				cmsDoTransform(doc->stdTransImg, ptr, ptr, out.width());
			}
		}
	}
	return out;
}

QColor SetColor(ScribusDoc *currentDoc, QString color, int shad)
{
	const ScColor& col = currentDoc->PageColors[color];
	return ScColorEngine::getShadeColorProof(col, currentDoc, shad);
}


/**
 * QPixmaps are really slow with Qt/Mac 3.3.4. Really, *really*, slow.
 * So we better cache them.
 */
QPixmap * getSmallPixmap(QColor rgb)
{
	static ScPixmapCache<QRgb> pxCache;

	QRgb index=rgb.rgb();
	if (pxCache.contains(index))
		return pxCache[index];

	QPixmap *pm = new QPixmap(15, 15);
	pm->fill(rgb);
	QPainter p;
	p.begin(pm);
	p.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	p.setPen(b);
	p.drawRect(0, 0, 15, 15);
	p.end();
	pxCache.insert(index, pm);
	return pm;
}

QPixmap * getWidePixmap(QColor rgb)
{
	static ScPixmapCache<QRgb> pxCache;

	QRgb index=rgb.rgb();
	if (pxCache.contains(index))
		return pxCache[index];

	QPixmap *pm = new QPixmap(30, 15);
	pm->fill(rgb);
	pxCache.insert(index, pm);
	return pm;
}

static Q_UINT64 code64(const ScColor & col) {
	int C, M, Y, K, R, G, B;
	Q_UINT64 result=0;
	col.getRGB( &R, &G, &B );
	col.getCMYK( &C, &M, &Y, &K );
	result |= col.getColorModel() == colorModelRGB ? 1 : 0;
	result |= col.isSpotColor() ? 64 : 0;
	result |= col.isRegistrationColor() ? 32 : 0;
	result <<= 8;
	result |= C;
	result <<= 8;
	result |= M;
	result <<= 8;
	result |= Y;
	result <<= 8;
	result |= K;
	result <<= 8;
	result |= R;
	result <<= 8;
	result |= G;
	result <<= 8;
	result |= B;
	return result;
}

QPixmap * getFancyPixmap(const ScColor& col, ScribusDoc* doc) {
	static ScPixmapCache<Q_UINT64> pxCache;

	static QPixmap alertIcon;
	static QPixmap cmykIcon;
	static QPixmap rgbIcon;
	static QPixmap spotIcon;
	static QPixmap regIcon;
	static bool iconsInitialized = false;

	if ( !iconsInitialized ) {
		alertIcon = loadIcon("alert.png");
		cmykIcon = loadIcon("cmyk.png");
		rgbIcon = loadIcon("rgb.png");
		spotIcon = loadIcon("spot.png");
		regIcon = loadIcon("register.png");
		iconsInitialized = true;
	}

	Q_UINT64 res=code64(col);
	if (pxCache.contains(res))
		return pxCache[res];

	QPixmap *pa=new QPixmap(60, 15);
	QPixmap *pm=getSmallPixmap(col.getRawRGBColor());
	pa->fill(Qt::white);
	paintAlert(*pm, *pa, 0, 0);
	if (ScColorEngine::isOutOfGamut(col, doc))
		paintAlert(alertIcon, *pa, 15, 0);
	if ((col.getColorModel() == colorModelCMYK) || (col.isSpotColor()))
		paintAlert(cmykIcon, *pa, 30, 0);
	else
		paintAlert(rgbIcon, *pa, 30, 0);
	if (col.isSpotColor())
		paintAlert(spotIcon, *pa, 46, 2);
	if (col.isRegistrationColor())
		paintAlert(regIcon, *pa, 45, 0);
	pxCache.insert(res, pa);
	return pa;
}


void paintAlert(QPixmap &toPaint, QPixmap &target, int x, int y, bool useMask)
{
	// there is no alpha mask in the beginning
	if (useMask)
	{
		if (target.mask()==0)
			target.setMask(QBitmap(target.width(), target.height(), useMask));
	}
	QPainter p;
	p.begin(&target);
	p.drawPixmap(x, y, toPaint);
	if (useMask)
	{
		QPainter alpha; // transparency handling
		alpha.begin(target.mask());
		alpha.setBrush(Qt::color1);
		alpha.setPen(Qt::color1);
		alpha.drawRect(x, y, 15, 15);
		if (toPaint.mask() != 0)
			alpha.drawPixmap(x, y, *toPaint.mask());
		alpha.end();
	}
	p.end();
}

unsigned char INT_MULT ( unsigned char a, unsigned char b )
{
	int c = a * b + 0x80;
	return (unsigned char)(( ( c >> 8 ) + c ) >> 8);
}

void RGBTOHSV ( uchar& red, uchar& green, uchar& blue )
{
	int r, g, b;
	double h, s, v;
	int min, max;
	h = 0.;
	r = red;
	g = green;
	b = blue;
	if ( r > g )
	{
		max = QMAX( r, b );
		min = QMIN( g, b );
	}
	else
	{
		max = QMAX( g, b );
		min = QMIN( r, b );
	}
	v = max;
	if ( max != 0 )
		s = ( ( max - min ) * 255 ) / (double)max;
	else
		s = 0;
	if ( s == 0 )
		h = 0;
	else
	{
		int delta = max - min;
		if ( r == max )
			h = ( g - b ) / (double)delta;
		else if ( g == max )
			h = 2 + ( b - r ) / (double)delta;
		else if ( b == max )
			h = 4 + ( r - g ) / (double)delta;
		h *= 42.5;
		if ( h < 0 )
			h += 255;
		if ( h > 255 )
			h -= 255;
	}
	red   = (uchar)h;
	green = (uchar)s;
	blue  = (uchar)v;
}

void HSVTORGB ( uchar& hue, uchar& saturation, uchar& value )
{
	if ( saturation == 0 )
	{
		hue        = value;
		saturation = value;
		value      = value;
	}
	else
	{
		double h = hue * 6. / 255.;
		double s = saturation / 255.;
		double v = value / 255.;

		double f = h - (int)h;
		double p = v * ( 1. - s );
		double q = v * ( 1. - ( s * f ) );
		double t = v * ( 1. - ( s * ( 1. - f ) ) );
		// Worth a note here that gcc 2.96 will generate different results
		// depending on optimization mode on i386.
		switch ((int)h)
		{
		case 0:
			hue        = (uchar)( v * 255 );
			saturation = (uchar)( t * 255 );
			value      = (uchar)( p * 255 );
			break;
		case 1:
			hue        = (uchar)( q * 255 );
			saturation = (uchar)( v * 255 );
			value      = (uchar)( p * 255 );
			break;
		case 2:
			hue        = (uchar)( p * 255 );
			saturation = (uchar)( v * 255 );
			value      = (uchar)( t * 255 );
			break;
		case 3:
			hue        = (uchar)( p * 255 );
			saturation = (uchar)( q * 255 );
			value      = (uchar)( v * 255 );
			break;
		case 4:
			hue        = (uchar)( t * 255 );
			saturation = (uchar)( p * 255 );
			value      = (uchar)( v * 255 );
			break;
		case 5:
			hue        = (uchar)( v * 255 );
			saturation = (uchar)( p * 255 );
			value      = (uchar)( q * 255 );
		}
	}
}

void RGBTOHLS ( uchar& red, uchar& green, uchar& blue )
{
	int r = red;
	int g = green;
	int b = blue;
	int min, max;
	if ( r > g )
	{
		max = QMAX( r, b );
		min = QMIN( g, b );
	}
	else
	{
		max = QMAX( g, b );
		min = QMIN( r, b );
	}
	double h;
	double l = ( max + min ) / 2.;
	double s;
	if ( max == min )
	{
		s = 0.;
		h = 0.;
	}
	else
	{
		int delta = max - min;
		if ( l < 128 )
			s = 255 * (double)delta / (double)( max + min );
		else
			s = 255 * (double)delta / (double)( 511 - max - min );
		if ( r == max )
			h = ( g - b ) / (double)delta;
		else if ( g == max )
			h = 2 + ( b - r ) / (double)delta;
		else
			h = 4 + ( r - g ) / (double)delta;
		h *= 42.5;
		if ( h < 0 )
			h += 255;
		else if ( h > 255 )
			h -= 255;
	}
	red   = (uchar)h;
	green = (uchar)l;
	blue  = (uchar)s;
}

int HLSVALUE ( double n1, double n2, double hue )
{
	double value;
	if ( hue > 255 )
		hue -= 255;
	else if ( hue < 0 )
		hue += 255;
	if ( hue < 42.5 )
		value = n1 + ( n2 - n1 ) * ( hue / 42.5 );
	else if ( hue < 127.5 )
		value = n2;
	else if ( hue < 170 )
		value = n1 + ( n2 - n1 ) * ( ( 170 - hue ) / 42.5 );
	else
		value = n1;
	return (int)( value * 255 );
}

void HLSTORGB ( uchar& hue, uchar& lightness, uchar& saturation )
{
	double h = hue;
	double l = lightness;
	double s = saturation;
	if ( s == 0 )
	{
		hue        = (uchar)l;
		lightness  = (uchar)l;
		saturation = (uchar)l;
	}
	else
	{
		double m1, m2;
		if ( l < 128 )
			m2 = ( l * ( 255 + s ) ) / 65025.;
		else
			m2 = ( l + s - ( l * s ) / 255. ) / 255.;
		m1 = ( l / 127.5 ) - m2;
		hue        = HLSVALUE( m1, m2, h + 85 );
		lightness  = HLSVALUE( m1, m2, h );
		saturation = HLSVALUE( m1, m2, h - 85 );
	}
}

double getCurveYValue(FPointArray &curve, double x, bool linear)
{
    double t;
    FPoint p;
    FPoint p0,p1,p2,p3;
    double c0,c1,c2,c3;
    double val = 0.5;
    if(curve.size() == 0)
        return 0.5;
    // First find curve segment
    p = curve.point(0);
    if(x < p.x())
        return p.y();
    p = curve.point(curve.size()-1);
    if(x >= p.x())
        return p.y();
	uint cc = 0;
    // Find the four control points (two on each side of x)    
    p = curve.point(0);
    while(x >= p.x())
    {
		cc++;
        p = curve.point(cc);
    }
    if (cc > 1)
    {
    	p0 = curve.point(cc-2);
    	p1 = curve.point(cc-1);
    }
    else
        p1 = p0 = curve.point(0);
    p2 = p;
    if (cc < curve.size()-1)
    	p3 = curve.point(cc+1);
    else
    	p3 = p;
    // Calculate the value
	if (linear)
	{
		double mc;
		if (p1.x() - p2.x() != 0.0)
			mc = (p1.y() - p2.y()) / (p1.x() - p2.x());
		else
			mc = p2.y() / p2.x();
		val = (x - p1.x()) * mc + p1.y();
	}
	else
	{
		t = (x - p1.x()) / (p2.x() - p1.x());
		c2 = (p2.y() - p0.y()) * (p2.x()-p1.x()) / (p2.x()-p0.x());
		c3 = p1.y();
		c0 = -2*p2.y() + 2*c3 + c2 + (p3.y() - p1.y()) * (p2.x() - p1.x()) / (p3.x() - p1.x());
		c1 = p2.y() - c3 - c2 - c0;
		val = ((c0*t + c1)*t + c2)*t + c3;
	}
	if(val < 0.0)
		val = 0.0;
	if(val > 1.0)
		val = 1.0;
	return val;
}

double Lum(uchar red, uchar green, uchar blue)
{
	return 0.3 * (red / 255.0) + 0.59 * (green / 255.0) + 0.11 * (blue / 255.0);
}

double LumD(double red, double green, double blue)
{
	return 0.3 * red + 0.59 * green + 0.11 * blue;
}

void setLum(uchar& red, uchar& green, uchar& blue, double lum)
{
	double rP = (red / 255.0);
	double gP = (green / 255.0);
	double bP = (blue / 255.0);
	double d = lum - Lum(red, green, blue);
	rP += d;
	gP += d;
	bP += d;
	clipColor(rP, gP, bP);
	red = qRound(rP * 255);
	green = qRound(gP * 255);
	blue = qRound(bP * 255);
	return;
}

void clipColor(double& red, double& green, double& blue)
{
	double l = LumD(red, green, blue);
	double n = QMIN(red, QMIN(green, blue));
	double x = QMAX(red, QMAX(green, blue));
	if (n < 0.0)
	{
		red = l + (((red - l) * l) / (l - n));
		green = l + (((green - l) * l) / (l - n));
		blue = l + (((blue - l) * l) / (l - n));
	}
	if (x > 1.0)
	{
		red = l + (((red - l) * (1.0 - l)) / (x - l));
		green = l + (((green - l) * (1.0 - l)) / (x - l));
		blue = l + (((blue - l) * (1.0 - l)) / (x - l));
	}
}

