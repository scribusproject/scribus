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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
/* Adapted for Scribus 22.08.2003 by Franz Schmid */
/* Adapted for Scribus 15.01.2006 by Jean Ghali */

#ifndef __SCGDIPAINTER_H__
#define __SCGDIPAINTER_H__

// libart wrapper

#include <qglobal.h>
#include <qwmatrix.h>
#include <qvaluelist.h>
#include <qvaluestack.h>
#include <qcolor.h>
#include <qfont.h>
#include <qpixmap.h>
#include "scribusapi.h"
#include "scconfig.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "vgradient.h"

#include "scpainterexbase.h"

#include <valarray>
#include <windows.h>

class QPainter;

#ifdef SC_USE_GDIPLUS
namespace Gdiplus
{
	class Graphics;
	class GraphicsPath;
};
#endif

// Typedef to use the GDI gradientFill function
typedef bool (STDAPICALLTYPE *gradientFillFunc) (HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);  

class SCRIBUS_API ScPainterEx_GDI : public ScPainterExBase
{
public:

	ScPainterEx_GDI( HDC hDC, QRect& rect, bool gray );
	virtual ~ScPainterEx_GDI();

	virtual int supportedColorModes() { return (int) rgbMode; }
	virtual ColorMode preferredColorMode() { return rgbMode; }
	virtual ImageMode imageMode() { return rgbImages; }

	virtual void begin();
	virtual void end();
	virtual void clear();
	virtual void clear( ScColorShade & );

	// matrix manipulation
	virtual void setWorldMatrix( const QWMatrix & );
	virtual const QWMatrix worldMatrix();
	virtual void setZoomFactor( double );
	virtual double zoomFactor() { return m_zoomFactor; }
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
	virtual void setGradient( VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc = FPoint(0,0));
	virtual void setClipPath();

	virtual void drawImage( ScImage *image );
	virtual void setupPolygon(FPointArray *points, bool closed = true);
	virtual void setupTextPolygon(FPointArray *points);
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
	virtual void setDash(const QValueList<double>& array, double ofs);
	virtual void setBrush( const ScColorShade & );
	virtual void setBrushOpacity( double op );
	virtual void setOpacity( double op );
	virtual void setFont( const QFont &f );
	virtual QFont font();

	// stack management
	virtual void save();
	virtual void restore();

	virtual void setRasterOp( int op );

	virtual QPaintDevice *device() { return m_target; }
	unsigned char *buffer() { return m_buffer; }

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

	unsigned int m_index;
	unsigned int m_alloccount;
	unsigned char *m_buffer;
	QPaintDevice *m_target;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_x;
	unsigned int m_y;
	QWMatrix m_pageTrans;
	QWMatrix m_matrix;
	QFont m_font;
	bool mf_underline;
	bool mf_strikeout;
	bool mf_shadow;
	bool mf_outlined;
/* Filling */
	ScColorShade m_fill;
	double fill_trans;
	bool m_fillRule;
	int fillMode;				// 0 = none, 1 = solid, 2 = gradient
	int gradientMode;		// 1 = linear, 2 = radial
/* Stroking */
	ScColorShade m_stroke;
	double stroke_trans;
	double LineWidth;
/* Grayscale conversion option */
	bool toGray;

/* Line End Style */
	Qt::PenCapStyle PLineEnd;
/* Line Join Style */
	Qt::PenJoinStyle PLineJoin;
/* The Dash Array */
	QValueList<double> m_array;
	double m_offset;
/* Transformation Stack */
	QValueStack<QWMatrix> MStack;
/* Zoom Factor of the Painter */
	double m_zoomFactor;

/* Some data to describe state of drawing */	
	bool pathClosed;
	bool drawingClosedCurve;
/* Drawing surface dimensions */
	double deviceDimX;
	double deviceDimY;
/* Device resolutions */
	double deviceResX;
	double deviceResY;

/* Device context */
	HDC dc;
/* Handle to a bitmap */
	HBITMAP hBmp;
/* Handle to the msimg32.dll (contains the GradientFill function) */
	HMODULE hMsImg32;
/* Address of the GradientFill function */
	gradientFillFunc gradientFill;

/* Color conversion function */
	QColor transformColor( ScColorShade& colorShade, double trans );
	void   transformImage( QImage* image, uchar* data, int scan);

#ifdef SC_USE_GDIPLUS
/* GDI+ needed data */
	QValueStack<int> gStates;
	Gdiplus::Graphics *graphics;
	Gdiplus::GraphicsPath *graphicsPath;
	double positionX;
	double positionY;
#endif
};

#endif
