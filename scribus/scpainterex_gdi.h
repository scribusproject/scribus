/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This file is part of the KDE project
   Copyright (C) 2001, 2002, 2003 The Karbon Developers

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
/* Adapted for Scribus 22.08.2003 by Franz Schmid */
/* Adapted for Scribus 15.01.2006 by Jean Ghali */

#ifndef __SCPAINTEREX_GDI_H__
#define __SCPAINTEREX_GDI_H__

// libart wrapper

#include <QColor>
#include <QFont>
#include <QGlobalStatic>
#include <QList>
#include <QMatrix>
#include <QPixmap>
#include <QStack>

#include "scribusapi.h"
#include "scconfig.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "vgradient.h"

#include "scpainterexbase.h"

#include <valarray>
#include <windows.h>

class QPainter;
class ScribusDoc;

// Typedef to use the GDI gradientFill function
typedef bool (STDAPICALLTYPE *gradientFillFunc) (HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);  

class SCRIBUS_API ScPainterEx_GDI : public ScPainterExBase
{
public:

	ScPainterEx_GDI(HDC hDC, QRect& rect, ScribusDoc* doc, bool gray );
	virtual ~ScPainterEx_GDI();

	virtual Capabilities capabilities() { return transparencies; }

	virtual int supportedColorModes() { return (int) rgbMode; }
	virtual ColorMode preferredColorMode() { return rgbMode; }
	virtual ImageMode imageMode() { return rgbImages; }

	virtual void begin();
	virtual void end();
	virtual void clear();
	virtual void clear( ScColorShade & );

	// matrix manipulation
	virtual void setWorldMatrix( const QMatrix & );
	virtual const QMatrix worldMatrix();
	virtual void translate( double, double );
	virtual void rotate( double );
	virtual void scale( double, double );

	// drawing
	virtual void moveTo( const double &, const double & );
	virtual void lineTo( const double &, const double & );
	virtual void curveTo( FPoint p1, FPoint p2, FPoint p3 );
	virtual void newPath();
	virtual void fillTextPath();
	virtual void strokeTextPath();
	virtual void fillPath();
	virtual void strokePath();
	virtual void setFillRule( bool fillRule );
	virtual bool fillRule() { return m_fillRule; }
	virtual void setFillMode( int fill );
	virtual int  fillMode() { return m_fillMode; }
	virtual void setGradient( VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc = FPoint(0,0));
	virtual void setPattern ( ScPattern* pattern, QMatrix& patternTransform );
	virtual void setClipPath();

	virtual void drawImage( ScImage *image, ScPainterExBase::ImageMode mode );
	virtual void setupPolygon(FPointArray *points, bool closed = true);
	virtual void drawPolygon();
	virtual void drawPolyLine();
	virtual void drawLine(FPoint start, FPoint end);
	virtual void drawRect(double, double, double, double);

	// pen + brush
	virtual ScColorShade pen();
	virtual ScColorShade brush();
	virtual void setPen( const ScColorShade & );
	virtual void setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo );
	virtual void setPenOpacity( double op );
	virtual void setLineWidth( double w);
	virtual void setDash(const QList<double>& array, double ofs);
	virtual void setBrush( const ScColorShade & );
	virtual void setBrushOpacity( double op );
	virtual void setOpacity( double op );
	virtual void setFont( const QFont &f );
	virtual QFont font();

	// stack management
	virtual void save();
	virtual void restore();

	virtual void setRasterOp( int op );

private:

	void drawVPath( int mode );
	void drawGradient( VGradientEx& gradient );
	void drawLinearGradient( VGradientEx& gradient, const QRect& rect );
	void drawLinearGradient_GradientFill( VGradientEx& gradient, const QRect& rect );
	void drawCircularGradient( VGradientEx& gradient, const QRect& rect );
	void drawCircleStrip( int x, int y, int w, int h, int r, int g, int b, int a );
	void drawRectangleStrip( int x, int y, int w, int h, int r, int g, int b, int a );
	void getClipPathDimensions( QRect& r );
	void transformPoint( const FPoint& in, FPoint& out );
	void transformPoints( const FPoint* in, FPoint* out, uint length );
	void loadMsImg32( void );

	ScribusDoc* m_doc;

	unsigned int m_width;
	unsigned int m_height;
	QMatrix m_matrix;
	QFont m_font;
/* Filling */
	ScColorShade m_fillColor;
	double m_fillTrans;
	bool   m_fillRule;
	int    m_fillMode;			// 0 = none, 1 = solid, 2 = gradient
	int    m_gradientMode;		// 1 = linear, 2 = radial
/* Stroking */
	ScColorShade m_strokeColor;
	double m_strokeTrans;
	double m_lineWidth;
/* Grayscale conversion option */
	bool   m_convertToGray;

/* Line End Style */
	Qt::PenCapStyle m_lineEnd;
/* Line Join Style */
	Qt::PenJoinStyle m_lineJoin;
/* The Dash Array */
	QList<double> m_array;
	double m_offset;
/* Transformation Stack */
	QStack<QMatrix> m_stack;

/* Some data to describe state of drawing */	
	bool m_pathIsClosed;
	bool m_drawingClosedPath;
/* Drawing surface dimensions */
	double m_deviceDimX;
	double m_deviceDimY;
/* Device resolutions */
	double m_deviceResX;
	double m_deviceResY;

/* Device context */
	HDC m_dc;
/* Handle to a bitmap */
	HBITMAP m_hBmp;
/* Handle to the msimg32.dll (contains the GradientFill function) */
	HMODULE m_hMsImg32;
/* Address of the GradientFill function */
	gradientFillFunc m_gradientFill;

/* Color conversion function */
	QColor transformColor( ScColorShade& colorShade, double trans );
	void   transformImage( QImage* image, uchar* data, int scan);
};

#endif
