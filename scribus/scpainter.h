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

#ifndef __SCPAINTER_H__
#define __SCPAINTER_H__

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

// If defined, use gdk-pixbuf for ScPainter::end() on X11 (ignored on other
// platforms). Otherwise use portable ScPainter::end() and omit X11-specific
// code and pixbuf support.
#ifndef SC_USE_PIXBUF
#define SC_USE_PIXBUF
#endif

// If defined, use Win32 GDI functions for ScPainter::end() on Win32 (ignored 
// on other platforms). Otherwise use portable ScPainter::end()
#ifndef SC_USE_GDI
#define SC_USE_GDI
#endif


class QPainter;
#ifdef HAVE_CAIRO
typedef struct _cairo cairo_t;
#else
struct _ArtVpath;
struct _ArtBpath;
struct _ArtSVP;
struct _ArtGradientStop;
#endif

class SCRIBUS_API ScPainter
{
public:
	ScPainter( QPaintDevice *target, unsigned int w = 0, unsigned int h = 0, unsigned int x = 0, unsigned int y = 0 );
	ScPainter( QImage *target, unsigned int w = 0, unsigned int h = 0, unsigned int x = 0, unsigned int y = 0 );
	virtual ~ScPainter();
	enum FillMode { None, Solid, Gradient };
	virtual void begin();
	virtual void end();
	void clear();
	virtual void clear( const QColor & );

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
	virtual void setGradient( VGradient::VGradientType mode, FPoint orig, FPoint vec, FPoint foc = FPoint(0,0));
	virtual void setClipPath();

	virtual void drawImage( QImage *image );
	virtual void setupPolygon(FPointArray *points, bool closed = true);
	virtual void setupTextPolygon(FPointArray *points);
	virtual void drawPolygon();
	virtual void drawPolyLine();
	virtual void drawLine(FPoint start, FPoint end);
	virtual void drawRect(double, double, double, double);

	// pen + brush
	virtual QColor pen();
	virtual QColor brush();
	virtual void setPen( const QColor & );
	virtual void setPen( const QColor &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo );
	virtual void setPenOpacity( double op );
	virtual void setLineWidth( double w);
	virtual void setDash(const QValueList<double>& array, double ofs);
	virtual void setBrush( const QColor & );
	virtual void setBrushOpacity( double op );
	virtual void setOpacity( double op );
	virtual void setFont( const QFont &f );
	virtual QFont font();


	// stack management
	virtual void save();
	virtual void restore();

	//
	virtual void setRasterOp( int op );

	virtual QPaintDevice *device() { return m_target; }
	unsigned char *buffer() { return m_buffer; }
	VGradient fill_gradient;
	VGradient stroke_gradient;

private:
#ifdef HAVE_CAIRO
	void drawVPath( int mode );
#else
	void drawVPath( struct ArtVpath *vec, int mode, bool preCal = false );
	void applyGradient( ArtSVP *svp, bool fill );
	virtual void resize( unsigned int w, unsigned int h );
	_ArtGradientStop *buildStopArray( VGradient &gradient, int & );
	void clampToViewport( const ArtSVP &svp, int &x0, int &y0, int &x1, int &y1 );
	void clampToViewport( int &x0, int &y0, int &x1, int &y1 );
	void ensureSpace( unsigned int );
	struct _ArtBpath *m_path;
	struct _ArtBpath *m_path2;
	struct _ArtSVP *m_clipPath;
#endif
	unsigned int m_index;
	unsigned int m_alloccount;
	unsigned char *m_buffer;
	QPaintDevice *m_target;
	QImage *m_image;
	QPixmap pixm;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_x;
	unsigned int m_y;
	QWMatrix m_matrix;
	QFont m_font;
	bool mf_underline;
	bool mf_strikeout;
	bool mf_shadow;
	bool mf_outlined;
/* Filling */
	QColor m_fill;
	double fill_trans;
	bool m_fillRule;
	int fillMode;				// 0 = none, 1 = solid, 2 = gradient
	int gradientMode;		// 1 = linear, 2 = radial
/* Stroking */
	QColor m_stroke;
	double stroke_trans;
	double LineWidth;

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
	bool imageMode;
#if defined(Q_WS_X11) && defined(SC_USE_PIXBUF)
#ifdef HAVE_CAIRO
	cairo_t *m_cr;
#else
	GC gc;
#endif
#elif defined(_WIN32) && defined(SC_USE_GDI)
	HDC dc;
#endif
};

#endif
