/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This file is part of the KDE project.
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
// kopainter/libart wrapper

#include "scpainter.h"

#include <qpaintdevice.h>
#include <qpixmap.h>
#include <qpointarray.h>
#include <qimage.h>

#ifdef HAVE_CAIRO
#include <cairo.h>
#include <cairo-xlib.h>
#else
#include <libart_lgpl/art_vpath.h>
#include <libart_lgpl/art_bpath.h>
#include <libart_lgpl/art_vpath_bpath.h>
#include <libart_lgpl/art_svp_vpath.h>
#include <libart_lgpl/art_svp_vpath_stroke.h>
#include <libart_lgpl/art_svp.h>
#include <libart_lgpl/art_svp_ops.h>
#include <libart_lgpl/art_affine.h>
#include <libart_lgpl/art_svp_intersect.h>
#include <libart_lgpl/art_rect_svp.h>
#include <libart_lgpl/art_pathcode.h>
#include <libart_lgpl/art_vpath_dash.h>
#include <libart_lgpl/art_render_svp.h>
#include <libart_lgpl/art_vpath_svp.h>
#include "libart/art_render_misc.h"
#include "libart/art_rgb_svp.h"
#include "libart/art_render_pattern.h"
#include "libart/art_kmisc.h"
#define INITIAL_ALLOC	300
#define ALLOC_INCREMENT	100
#endif

#if defined(Q_WS_X11) && defined(SC_USE_PIXBUF)
#include <X11/Xlib.h>
#include <pixbuf/gdk-pixbuf-xlibrgb.h>
#endif

#if defined(_WIN32) && defined(SC_USE_GDI)
#include <windows.h>
#endif

#include <math.h>

ScPainter::ScPainter( QPaintDevice *target, unsigned int w, unsigned int h, unsigned int x, unsigned int y )
{
	m_target = target;
	m_width = w;
	m_height= h;
	m_x = x;
	m_y = y;
	m_buffer = 0L;
#ifndef HAVE_CAIRO
	m_path = 0L;
#endif
	m_index = 0;
	m_stroke = QColor(0,0,0);
	m_fill = QColor(0,0,0);
	fill_trans = 1.0;
	stroke_trans = 1.0;
	m_fillRule = true;
	fillMode = 1;
	LineWidth = 1.0;
	m_offset = 0;
	m_array.clear();
	mf_underline = false;
	mf_strikeout = false;
	mf_shadow = false;
	mf_outlined = false;
	PLineEnd = Qt::SquareCap;
	PLineJoin = Qt::RoundJoin;
	fill_gradient = VGradient(VGradient::linear);
	stroke_gradient = VGradient(VGradient::linear);
	m_zoomFactor = 1;
	imageMode = false;
	m_matrix = QWMatrix();
#if defined(Q_WS_X11) && defined(SC_USE_PIXBUF)
#ifdef HAVE_CAIRO
	pixm = QPixmap(w, h);
	Display *dpy = pixm.x11AppDisplay();
	Drawable drw = pixm.handle();
	cairo_surface_t *img;
	img = cairo_xlib_surface_create(dpy, drw, (Visual*)pixm.x11Visual(), w, h);
	m_cr = cairo_create(img);
	clear();
	cairo_save( m_cr );
	cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
/*	Setting to 0.5 here gives a good tradeoff between speed and precision
	the former setting of 0.2 is just too precise, and setting it to 0.9 or greater will give bad rendering */
	cairo_set_tolerance( m_cr, 0.5 );
#else
	resize( m_width, m_height );
	clear();
	xlib_rgb_init_with_depth( target->x11Display(), XScreenOfDisplay( target->x11Display(), target->x11Screen() ), target->x11Depth() );
	gc = XCreateGC( target->x11Display(), target->handle(), 0, 0 );
#endif
#elif defined(_WIN32) && defined(SC_USE_GDI)
	resize( m_width, m_height );
	clear();
	dc = CreateCompatibleDC( target->handle() );
#else
	resize( m_width, m_height );
	clear();
#endif
}

ScPainter::ScPainter( QImage *target, unsigned int w, unsigned int h, unsigned int x, unsigned int y )
{
	m_target = 0L;
	m_width = w;
	m_height= h;
	m_x = x;
	m_y = y;
	m_buffer = 0L;
	m_index = 0;
	m_stroke = QColor(0,0,0);
	m_fill = QColor(0,0,0);
	fill_trans = 1.0;
	stroke_trans = 1.0;
	m_fillRule = true;
	fillMode = 1;
	LineWidth = 1.0;
	m_offset = 0;
	m_array.clear();
	mf_underline = false;
	mf_strikeout = false;
	mf_shadow = false;
	mf_outlined = false;
	PLineEnd = Qt::SquareCap;
	PLineJoin = Qt::RoundJoin;
	fill_gradient = VGradient(VGradient::linear);
	stroke_gradient = VGradient(VGradient::linear);
	m_zoomFactor = 1;
	imageMode = true;
	m_image = target;
#ifdef HAVE_CAIRO
	cairo_surface_t *img = cairo_image_surface_create_for_data(target->bits(), CAIRO_FORMAT_ARGB32, w, h, w*4);
	m_cr = cairo_create(img);
	clear();
	cairo_save( m_cr );
	cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
/*	Setting to 0.5 here gives a good tradeoff between speed and precision
	the former setting of 0.2 is just too precise, and setting it to 0.9 or greater will give bad rendering */
	cairo_set_tolerance( m_cr, 0.5 );
#else
	resize( m_width, m_height );
	clear();
	m_path = 0L;
#endif
}

ScPainter::~ScPainter()
{
	// If we are in target mode, we created a buffer, else if we used the other ctor
	// we didnt.
#ifndef HAVE_CAIRO
	if (( m_target ) || ( imageMode))
		art_free( m_buffer );
	if( m_path )
		art_free( m_path );
#endif
#if defined(Q_WS_X11) && defined(SC_USE_PIXBUF)
#ifdef HAVE_CAIRO
	cairo_surface_destroy(cairo_get_target(m_cr));
	cairo_destroy( m_cr );
#else
	if (imageMode)
		return;
	if( gc )
		XFreeGC( m_target->x11Display(), gc );
#endif
#elif defined(_WIN32) && defined(SC_USE_GDI)
	if (imageMode)
		return;
	if( dc )
		DeleteDC( dc );
#endif
}

void ScPainter::begin()
{
}

void ScPainter::end()
{
	if (imageMode)
	{
#ifdef HAVE_CAIRO
		cairo_surface_flush(cairo_get_target(m_cr));
		cairo_restore( m_cr );
#else
		QRgb * bits = (QRgb *) m_image->bits();
		int words = m_image->numBytes() / 4;
		art_u8 * p = m_buffer;;
		for (int i=0; i < words; ++i)
		{
			art_u8 r = *p++;
			art_u8 g = *p++;
			art_u8 b = *p++;
			art_u8 a = *p++;
			*bits++ = qRgba(r,g,b,a);
		}
#endif
	}
	else
	{
#if defined(Q_WS_X11) && defined(SC_USE_PIXBUF)
#ifdef HAVE_CAIRO
		cairo_surface_flush(cairo_get_target(m_cr));
		bitBlt( m_target, m_x, m_y, &pixm, 0, 0, m_width, m_height );
		cairo_restore( m_cr );
#else
		// Use the original gdk-pixbuf based bitblit on X11
		xlib_draw_rgb_32_image( m_target->handle(), gc, m_x, m_y, m_width, m_height, XLIB_RGB_DITHER_NONE, m_buffer, m_width * 4 );
#endif
#elif defined(_WIN32) && defined(SC_USE_GDI)
		// Use Win32 implementation
		BITMAPINFO bmpInfo;
		BITMAPINFOHEADER *bmpHeader;
		long* bmpData;

		bmpHeader = &(bmpInfo.bmiHeader);
		bmpHeader->biSize = sizeof(BITMAPINFOHEADER);
		bmpHeader->biWidth = m_width;
		bmpHeader->biHeight = -m_height;
		bmpHeader->biPlanes = 1;
		bmpHeader->biBitCount = 32;
		bmpHeader->biCompression = BI_RGB;
		bmpHeader->biSizeImage = 0; // Valid only if biCompression = BI_RGB
		bmpHeader->biXPelsPerMeter = 0;
		bmpHeader->biYPelsPerMeter = 0;
		bmpHeader->biClrUsed = 0;
		bmpHeader->biClrImportant = 0;

		// Create a device independent Bitmap
		HBITMAP hBmp = CreateDIBSection(dc, &bmpInfo, DIB_RGB_COLORS, (void** ) (&bmpData), NULL, NULL);
		art_u8 * p = m_buffer;
		art_u8 r, g, b, a;
		int words = m_width * m_height; // Valid only if biBitCount = 32

		for (int i = 0; i < words; ++i) {
			r = *p++;
			g = *p++;
			b = *p++;
			a = *p++;
			*bmpData++ = ( (a << 24) + (r << 16) + (g << 8) + b);
		}

		HGDIOBJ obj = SelectObject( dc, hBmp);
		BitBlt( m_target->handle() , m_x, m_y, m_width, m_height, dc, 0, 0, SRCCOPY);
		SelectObject( dc, obj);
		DeleteObject( hBmp );
#else
		// Portable copying onto the canvas with no X11 dependency by Andreas Vox
		QImage qimg(m_width, m_height, 32, QImage::BigEndian);
		QRgb * bits = (QRgb *) qimg.bits();
		int words = qimg.numBytes() / 4;
		art_u8 * p = m_buffer;;
		for (int i=0; i < words; ++i) {
			art_u8 r = *p++;
			art_u8 g = *p++;
			art_u8 b = *p++;
			art_u8 a = *p++;
			*bits++ = qRgba(r,g,b,a);
		}
		bitBlt(m_target, m_x, m_y, &qimg);
#endif
	}
}

void ScPainter::clear()
{
#ifdef HAVE_CAIRO
	if (imageMode)
		m_image->fill( qRgba(255, 255, 255, 255) );
	else
		pixm.fill( Qt::white );
#else
	if( m_buffer )
		memset( m_buffer, 255, m_width * m_height * 4 );
#endif
}

void ScPainter::clear( const QColor &c )
{
#ifdef HAVE_CAIRO
	QRgb cs = c.rgb();
	if (imageMode)
		m_image->fill( qRgba(qRed(cs), qGreen(cs), qBlue(cs), qAlpha(cs)) );
	else
		pixm.fill( c );
#else
	if( m_buffer )
	{
		unsigned int *src = (unsigned int*)m_buffer;
		unsigned int co = qRgba(c.blue(), c.green(),  c.red(), 255);
		for(unsigned int cc = 0; cc < (m_width * m_height); ++cc)
		{
			*src++ = co;
		}
	}
#endif
}

const QWMatrix ScPainter::worldMatrix()
{
#ifdef HAVE_CAIRO
	cairo_matrix_t matrix;
	cairo_get_matrix(m_cr, &matrix);
	QWMatrix mat;
	mat.setMatrix ( matrix.xx, matrix.yx, matrix.xy, matrix.yy, matrix.x0, matrix.y0 );
	return mat;
#else
	return m_matrix;
#endif
}

void ScPainter::setWorldMatrix( const QWMatrix &mat )
{
#ifdef HAVE_CAIRO
	cairo_matrix_t matrix;
	cairo_matrix_init(&matrix, mat.m11(), mat.m12(), mat.m21(), mat.m22(), mat.dx(), mat.dy());
	cairo_set_matrix(m_cr, &matrix);
#else
	m_matrix = mat;
#endif
}

void ScPainter::setZoomFactor( double zoomFactor )
{
	m_zoomFactor = zoomFactor;
}

void ScPainter::translate( double x, double y )
{
#ifdef HAVE_CAIRO
	cairo_translate (m_cr, x, y);
#else
	m_matrix.translate(x, y);
#endif
}

void ScPainter::rotate( double r )
{
#ifdef HAVE_CAIRO
	cairo_rotate (m_cr, r * 3.1415927 / 180.0);
#else
	m_matrix.rotate(r);
#endif
}

void ScPainter::scale( double x, double y )
{
#ifdef HAVE_CAIRO
	cairo_scale (m_cr, x, y);
#else
	m_matrix.scale(x, y);
#endif
}

void ScPainter::moveTo( const double &x, const double &y )
{
#ifdef HAVE_CAIRO
	cairo_move_to( m_cr, x * m_zoomFactor, y * m_zoomFactor);
#else
	ensureSpace( m_index + 1 );
	m_path[ m_index ].code = ART_MOVETO;
	m_path[ m_index ].x3 = x * m_zoomFactor;
	m_path[ m_index ].y3 = y * m_zoomFactor;
	m_index++;
#endif
}

void 
ScPainter::lineTo( const double &x, const double &y )
{
#ifdef HAVE_CAIRO
	cairo_line_to( m_cr, x * m_zoomFactor, y * m_zoomFactor);
#else
	ensureSpace( m_index + 1 );
	m_path[ m_index ].code = ART_LINETO;
	m_path[ m_index ].x3	= x * m_zoomFactor;
	m_path[ m_index ].y3	= y * m_zoomFactor;
	m_index++;
#endif
}

void ScPainter::curveTo( FPoint p1, FPoint p2, FPoint p3 )
{
#ifdef HAVE_CAIRO
	cairo_curve_to(m_cr, 
								p1.x() * m_zoomFactor, 
								p1.y() * m_zoomFactor, 
								p2.x() * m_zoomFactor, 
								p2.y() * m_zoomFactor, 
								p3.x() * m_zoomFactor, 
								p3.y() * m_zoomFactor);
#else
	ensureSpace( m_index + 1 );
	m_path[ m_index ].code = ART_CURVETO;
	m_path[ m_index ].x1	= p1.x() * m_zoomFactor;
	m_path[ m_index ].y1	= p1.y() * m_zoomFactor;
	m_path[ m_index ].x2	= p2.x() * m_zoomFactor;
	m_path[ m_index ].y2	= p2.y() * m_zoomFactor;
	m_path[ m_index ].x3	= p3.x() * m_zoomFactor;
	m_path[ m_index ].y3	= p3.y() * m_zoomFactor;
	m_index++;
#endif
}

void ScPainter::newPath()
{
#ifdef HAVE_CAIRO
	cairo_new_path( m_cr );
#else
	m_index = 0;
#endif
}

void ScPainter::setFillRule( bool fillRule )
{
	m_fillRule = fillRule;
}

void ScPainter::setFillMode( int fill )
{
	fillMode = fill;
}

void ScPainter::setGradient(VGradient::VGradientType mode, FPoint orig, FPoint vec, FPoint foc)
{
	fill_gradient.setType(mode);
	fill_gradient.setOrigin(orig);
	fill_gradient.setVector(vec);
	fill_gradient.setFocalPoint(foc);
}

void ScPainter::fillTextPath()
{
#ifdef HAVE_CAIRO
	drawVPath( 0 );
#else
	ArtVpath *path = art_bez_path_to_vec( m_path , 0.25 );
	drawVPath( path, 0, true );
#endif
}

void ScPainter::strokeTextPath()
{
	if( LineWidth == 0 )
		return;
#ifdef HAVE_CAIRO
	drawVPath( 1 );
#else
	ArtVpath *path = art_bez_path_to_vec( m_path , 0.25 );
	drawVPath( path, 1, true );
#endif
}

void ScPainter::fillPath()
{
#ifdef HAVE_CAIRO
	if( fillMode != 0)
		drawVPath( 0 );
#else
	if( m_index == 0 ) 
		return;
	if( fillMode != 0)
	{
		ArtVpath *path = art_bez_path_to_vec( m_path , 0.25 );
		drawVPath( path, 0 );
	}
#endif
}

void ScPainter::strokePath()
{
	if( LineWidth == 0 )
		return;
#ifdef HAVE_CAIRO
	drawVPath( 1 );
#else
	if( m_index == 0 )
		return;
	if( m_path[ m_index ].code != ART_END)
		m_path[ m_index ].code = ART_END;
	ArtVpath *path = art_bez_path_to_vec( m_path , 0.25 );
	drawVPath( path, 1 );
#endif
}

QColor ScPainter::pen()
{
	return m_stroke;
}

QColor ScPainter::brush()
{
	return m_fill;
}

void ScPainter::setPen( const QColor &c )
{
	m_stroke = c;
}

void ScPainter::setPen( const QColor &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo )
{
	m_stroke = c;
	LineWidth = w;
	PLineEnd = ca;
	PLineJoin = jo;
	double Dt = QMAX(2*w, 1);
	double Da = QMAX(6*w, 1);
	QValueList<double> tmp;
	m_array.clear();
	m_offset = 0;
	switch (st)
		{
		case Qt::SolidLine:
			break;
		case Qt::DashLine:
			m_array.append(Da);
			m_array.append(Dt);
			break;
		case Qt::DotLine:
			m_array.append(Dt);
			break;
		case Qt::DashDotLine:
			m_array.append(Da);
			m_array.append(Dt);
			m_array.append(Dt);
			m_array.append(Dt);
			break;
		case Qt::DashDotDotLine:
			m_array.append(Da);
			m_array.append(Dt);
			m_array.append(Dt);
			m_array.append(Dt);
			m_array.append(Dt);
			m_array.append(Dt);
			break;
		default:
			break;
		}
}

void ScPainter::setLineWidth( double w )
{
	LineWidth = w;
}

void ScPainter::setPenOpacity( double op )
{
	stroke_trans = op;
}


void ScPainter::setDash(const QValueList<double>& array, double ofs)
{
	m_array = array;
	m_offset = ofs;
}

void ScPainter::setBrush( const QColor &c )
{
	m_fill = c;
}

void ScPainter::setBrushOpacity( double op )
{
	fill_trans = op;
}

void ScPainter::setOpacity( double op )
{
	fill_trans = op;
	stroke_trans = op;
}

void ScPainter::setFont( const QFont &f)
{
	m_font = f;
}

QFont ScPainter::font()
{
	return m_font;
}

void ScPainter::save()
{
#ifdef HAVE_CAIRO
	cairo_save( m_cr );
#else
	MStack.push(m_matrix);
#endif
}

void ScPainter::restore()
{
#ifdef HAVE_CAIRO
	cairo_restore( m_cr );
#else
	m_matrix = MStack.pop();
#endif
}

void ScPainter::setRasterOp( int   )
{
}

#ifdef HAVE_CAIRO
void ScPainter::drawVPath( int mode )
{
	cairo_save( m_cr );
	if (mode == 0)
	{
		if( m_fillRule )
			cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
		else
			cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_WINDING);
		if (fillMode == 2)
		{
			cairo_pattern_t *pat;
			double x1 = fill_gradient.origin().x() * m_zoomFactor;
			double y1 = fill_gradient.origin().y() * m_zoomFactor;
			double x2 = fill_gradient.vector().x() * m_zoomFactor;
			double y2 = fill_gradient.vector().y() * m_zoomFactor;
			if (fill_gradient.type() == VGradient::linear)
				pat = cairo_pattern_create_linear (x1, y1,  x2, y2);
			else
				pat = cairo_pattern_create_radial (x1, y1, 0.1, x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)));
			QPtrVector<VColorStop> colorStops = fill_gradient.colorStops();
			for( uint offset = 0 ; offset < colorStops.count() ; offset++ )
			{
				QColor qStopColor = colorStops[ offset ]->color;
				int h, s, v, sneu;
				int shad = colorStops[offset]->shade;
				qStopColor.rgb(&h, &s, &v);
				if ((h == s) && (s == v))
				{
					qStopColor.hsv(&h, &s, &v);
					sneu = 255 - ((255 - v) * shad / 100);
					qStopColor.setHsv(h, s, sneu);
				}
				else
				{
					qStopColor.hsv(&h, &s, &v);
					sneu = s * shad / 100;
					qStopColor.setHsv(h, sneu, v);
				}
				double r = qRed( qStopColor.rgb() ) / 255.0;
				double g = qGreen( qStopColor.rgb() ) / 255.0;
				double b = qBlue( qStopColor.rgb() ) / 255.0;
				double a = colorStops[offset]->opacity;
				cairo_pattern_add_color_stop_rgba (pat, colorStops[ offset ]->rampPoint, r, g, b, a);
			}
			cairo_set_source (m_cr, pat);
			cairo_clip_preserve (m_cr);
			cairo_paint_with_alpha (m_cr, fill_trans);
			cairo_pattern_destroy (pat);
		}
		else
		{
			double r = qRed( m_fill.rgb() ) / 255.0;
			double g = qGreen( m_fill.rgb() ) / 255.0;
			double b = qBlue( m_fill.rgb() ) / 255.0;
			cairo_set_source_rgba( m_cr, r, g, b, fill_trans );
			cairo_fill_preserve( m_cr );
		}
	}
	else
	{
		double *dashes;
		cairo_set_line_width( m_cr, LineWidth * m_zoomFactor );
		if( m_array.count() > 0 )
		{
			dashes = new double[ m_array.count() ];
			for( uint i = 0; i < m_array.count();++ i )
			{
				dashes[i] = static_cast<double>(m_array[i]) * m_zoomFactor;
			}
			cairo_set_dash( m_cr, dashes, m_array.count(), static_cast<double>(m_offset) * m_zoomFactor );
		}
		else
			cairo_set_dash( m_cr, NULL, 0, 0 );
		double r = qRed( m_stroke.rgb() ) / 255.0;
		double g = qGreen( m_stroke.rgb() ) / 255.0;
		double b = qBlue( m_stroke.rgb() ) / 255.0;
		cairo_set_source_rgba( m_cr, r, g, b, stroke_trans );
		if( PLineEnd == Qt::RoundCap )
			cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_ROUND);
		else if( PLineEnd == Qt::SquareCap )
			cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_SQUARE);
		else if( PLineEnd == Qt::FlatCap )
			cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_BUTT);
		if( PLineJoin == Qt::RoundJoin )
			cairo_set_line_join( m_cr, CAIRO_LINE_JOIN_ROUND );
		else if( PLineJoin == Qt::BevelJoin )
			cairo_set_line_join( m_cr, CAIRO_LINE_JOIN_BEVEL );
		else if( PLineJoin == Qt::MiterJoin )
			cairo_set_line_join( m_cr, CAIRO_LINE_JOIN_MITER );
		cairo_stroke_preserve( m_cr );
		if( m_array.count() > 0 )
			delete [] dashes;
	}
	cairo_restore( m_cr );
#else
void ScPainter::drawVPath( struct ArtVpath *vec, int mode, bool preCal )
{
	ArtSVP *strokeSvp = 0L;
	ArtSVP *fillSvp = 0L;
	if (!preCal)
	{
		double affine[6];
		affine[0] = m_matrix.m11();
		affine[1] = m_matrix.m12();
		affine[2] = m_matrix.m21();
		affine[3] = m_matrix.m22();
		affine[4] = m_matrix.dx();
		affine[5] = m_matrix.dy();
		ArtVpath *temp1 = art_vpath_affine_transform( vec, affine );
		art_free( vec );
		vec = temp1;
	}
	int af = 0;
	int as = 0;
	art_u32 fillColor = 0;
	QColor color;
	if (mode == 0)
		{
		color = m_fill;
		af = qRound( 255 * fill_trans );
#ifdef WORDS_BIGENDIAN
		fillColor = ( color.red() << 24 ) | ( color.green() << 16 ) | ( color.blue() << 8 );
#else
 		fillColor = ( 0 << 24 ) | ( color.blue() << 16 ) | ( color.green() << 8 ) | color.red();
#endif
		ArtSvpWriter *swr;
		ArtSVP *temp;
		temp = art_svp_from_vpath( vec );
		if( m_fillRule )
			swr = art_svp_writer_rewind_new( ART_WIND_RULE_ODDEVEN );
		else
			swr = art_svp_writer_rewind_new( ART_WIND_RULE_NONZERO );
		art_svp_intersector( temp, swr );
		fillSvp = art_svp_writer_rewind_reap( swr );
		art_svp_free( temp );
		}
	art_u32 strokeColor = 0;
	if (mode == 1)
		{
		ArtPathStrokeCapType capStyle = ART_PATH_STROKE_CAP_BUTT;
		ArtPathStrokeJoinType joinStyle = ART_PATH_STROKE_JOIN_MITER;
		color = m_stroke;
		as = qRound( 255 * stroke_trans );
#ifdef WORDS_BIGENDIAN
    strokeColor = ( color.red() << 24 ) | ( color.green() << 16 ) | ( color.blue() << 8 );
#else
		strokeColor = ( 0 << 24 ) | ( color.blue() << 16 ) | ( color.green() << 8 ) | color.red();
#endif
		double ratio = m_zoomFactor;
		if( m_array.count() > 0 )
			{
			ArtVpathDash dash;
			dash.offset = static_cast<double>(m_offset) * ratio;
			dash.n_dash = m_array.count();
			double *dashes = new double[ dash.n_dash ];
			for( int i = 0; i < dash.n_dash; i++ )
				{
				dashes[i] = static_cast<double>(m_array[i]) * ratio;
				}
			dash.dash = dashes;
			ArtVpath *vec2 = art_vpath_dash( vec, &dash );
			art_free( vec );
			vec = vec2;
			delete [] dashes;
			}
		if( PLineEnd == Qt::RoundCap )
			capStyle = ART_PATH_STROKE_CAP_ROUND;
		else if( PLineEnd == Qt::SquareCap )
			capStyle = ART_PATH_STROKE_CAP_SQUARE;
		if( PLineJoin == Qt::RoundJoin )
			joinStyle = ART_PATH_STROKE_JOIN_ROUND;
		else if( PLineJoin == Qt::BevelJoin )
			joinStyle = ART_PATH_STROKE_JOIN_BEVEL;
		strokeSvp = art_svp_vpath_stroke( vec, joinStyle, capStyle, ratio * LineWidth, 10, 0.25 );
		}
	int x0, y0, x1, y1;
	if(( fillSvp ) && (mode == 0))
		{
		if (fillMode == 2)
			applyGradient( fillSvp, true );
		else
			{
			clampToViewport( *fillSvp, x0, y0, x1, y1 );
			if( x0 != x1 && y0 != y1 )
				art_rgb_svp_alpha_( fillSvp, x0, y0, x1, y1, fillColor, af, m_buffer + x0 * 4 + y0 * m_width * 4, m_width * 4, 0 );
			}
		art_svp_free( fillSvp );
		}
	if(( strokeSvp ) && (mode == 1))
	{
			clampToViewport( *strokeSvp, x0, y0, x1, y1 );
			if( x0 != x1 && y0 != y1 )
				art_rgb_svp_alpha_( strokeSvp, x0, y0, x1, y1, strokeColor, as, m_buffer + x0 * 4 + y0 * m_width * 4, m_width * 4, 0 );
		art_svp_free( strokeSvp );
	}
	art_free( vec );
#endif
}

void ScPainter::setClipPath()
{
#ifdef HAVE_CAIRO
	cairo_clip (m_cr);
#else
	ArtVpath *vec = art_bez_path_to_vec( m_path , 0.25 );
	double affine[6];
	affine[0] = m_matrix.m11();
	affine[1] = m_matrix.m12();
	affine[2] = m_matrix.m21();
	affine[3] = m_matrix.m22();
	affine[4] = m_matrix.dx();
	affine[5] = m_matrix.dy();
	ArtVpath *temp1 = art_vpath_affine_transform( vec, affine );
	art_free( vec );
	ArtSvpWriter *swr;
	ArtSVP *temp = art_svp_from_vpath( temp1 );
	if( m_fillRule )
		swr = art_svp_writer_rewind_new( ART_WIND_RULE_ODDEVEN );
	else
		swr = art_svp_writer_rewind_new( ART_WIND_RULE_NONZERO );
	art_svp_intersector( temp, swr );
	m_clipPath = art_svp_writer_rewind_reap( swr );
	art_svp_free( temp );
	art_free( temp1 );
#endif
}

void ScPainter::drawImage( QImage *image )
{
#ifdef HAVE_CAIRO
	cairo_set_fill_rule(m_cr, cairo_get_fill_rule(m_cr));
	cairo_surface_t *image2;
	image2  = cairo_image_surface_create_for_data ((uchar*)image->bits(), CAIRO_FORMAT_ARGB32, image->width(), image->height(), image->width()*4);
	cairo_scale(m_cr, m_zoomFactor, m_zoomFactor);
	cairo_set_source_surface (m_cr, image2, 0, 0);
	cairo_paint_with_alpha(m_cr, fill_trans);
	cairo_surface_destroy (image2);
#else
	double affineresult[6];
	affineresult[0] = m_matrix.m11() * m_zoomFactor;
	affineresult[1] = m_matrix.m12() * m_zoomFactor;
	affineresult[2] = m_matrix.m21() * m_zoomFactor;
	affineresult[3] = m_matrix.m22() * m_zoomFactor;
	affineresult[4] = m_matrix.dx();
	affineresult[5] = m_matrix.dy();
	ksvg_art_rgb_affine_clip( m_clipPath, m_buffer, 0, 0, m_width, m_height, m_width * 4, 4,
					 image->bits(), image->width(), image->height(), image->width() * 4,
					 affineresult, qRound( 255 * fill_trans ), 0L );
	art_svp_free( m_clipPath );
#endif
}

void ScPainter::setupPolygon(FPointArray *points, bool closed)
{
	bool nPath = true;
	FPoint np, np1, np2, np3;
#ifdef HAVE_CAIRO
	if (points->size() > 3)
	{
		newPath();
		for (uint poi=0; poi<points->size()-3; poi += 4)
		{
			if (points->point(poi).x() > 900000)
			{
				nPath = true;
//    			cairo_close_path( m_cr );
				continue;
			}
			if (nPath)
			{
				np = points->point(poi);
    			cairo_move_to( m_cr, np.x() * m_zoomFactor, np.y() * m_zoomFactor);
				nPath = false;
			}
			np = points->point(poi);
			np1 = points->point(poi+1);
			np2 = points->point(poi+3);
			np3 = points->point(poi+2);
			if ((np == np1) && (np2 == np3))
				cairo_line_to( m_cr, np3.x() * m_zoomFactor, np3.y() * m_zoomFactor);
			else
				cairo_curve_to(m_cr, 
											np1.x() * m_zoomFactor, 
											np1.y() * m_zoomFactor, 
											np2.x() * m_zoomFactor, 
											np2.y() * m_zoomFactor, 
											np3.x() * m_zoomFactor, 
											np3.y() * m_zoomFactor);
		}
		if (closed)
    		cairo_close_path( m_cr );
	}
#else
	if (points->size() > 3)
		{
		newPath();
		for (uint poi=0; poi<points->size()-3; poi += 4)
			{
			if (points->point(poi).x() > 900000)
				{
				nPath = true;
				ensureSpace( m_index + 1 );
				m_path[ m_index ].code = ART_END;
				continue;
				}
			if (nPath)
				{
				np = points->point(poi);
				ensureSpace( m_index + 1 );
				if (closed)
					m_path[ m_index ].code = ART_MOVETO;
				else
					m_path[ m_index ].code = ART_MOVETO_OPEN;
				m_path[ m_index ].x3 = np.x() * m_zoomFactor;
				m_path[ m_index ].y3 = np.y() * m_zoomFactor;
				m_index++;
				nPath = false;
				}
			np = points->point(poi);
			np1 = points->point(poi+1);
			np2 = points->point(poi+3);
			np3 = points->point(poi+2);
			ensureSpace( m_index + 1 );
			if ((np == np1) && (np2 == np3))
				{
				m_path[ m_index ].code = ART_LINETO;
				m_path[ m_index ].x3	= np3.x() * m_zoomFactor;
				m_path[ m_index ].y3	= np3.y() * m_zoomFactor;
				}
			else
				{
				m_path[ m_index ].code = ART_CURVETO;
				m_path[ m_index ].x1	= np1.x() * m_zoomFactor;
				m_path[ m_index ].y1	= np1.y() * m_zoomFactor;
				m_path[ m_index ].x2	= np2.x() * m_zoomFactor;
				m_path[ m_index ].y2	= np2.y() * m_zoomFactor;
				m_path[ m_index ].x3	= np3.x() * m_zoomFactor;
				m_path[ m_index ].y3	= np3.y() * m_zoomFactor;
				}
			m_index++;
			}
		ensureSpace( m_index + 1 );
		m_path[ m_index ].code = ART_END;
		m_index++;
		}
#endif
}

void ScPainter::setupTextPolygon(FPointArray *points)
{
	bool nPath = true;
	FPoint np, np1, np2, np3;
#ifdef HAVE_CAIRO
	if (points->size() > 3)
	{
		newPath();
		for (uint poi=0; poi<points->size()-3; poi += 4)
		{
			if (points->point(poi).x() > 900000)
			{
				nPath = true;
				continue;
			}
			if (nPath)
			{
				np = points->point(poi);
    			cairo_move_to( m_cr, np.x(), np.y());
				nPath = false;
			}
			np = points->point(poi);
			np1 = points->point(poi+1);
			np2 = points->point(poi+3);
			np3 = points->point(poi+2);
			if ((np == np1) && (np2 == np3))
				cairo_line_to( m_cr, np3.x(), np3.y());
			else
				cairo_curve_to(m_cr, np1.x(), np1.y(), np2.x(), np2.y(), np3.x(), np3.y());
		}
    	cairo_close_path( m_cr );
	}
#else
	newPath();
	for (uint poi=0; poi<points->size()-3; poi += 4)
	{
		if (points->point(poi).x() > 900000)
		{
			nPath = true;
			ensureSpace( m_index + 1 );
			m_path[ m_index ].code = ART_END;
			continue;
		}
		if (nPath)
		{
			np = points->point(poi);
			ensureSpace( m_index + 1 );
			m_path[ m_index ].code = ART_MOVETO;
			m_path[ m_index ].x3 = np.x();
			m_path[ m_index ].y3 = np.y();
			m_index++;
			nPath = false;
		}
		np = points->point(poi);
		np1 = points->point(poi+1);
		np2 = points->point(poi+3);
		np3 = points->point(poi+2);
		ensureSpace( m_index + 1 );
		if ((np == np1) && (np2 == np3))
		{
			m_path[ m_index ].code = ART_LINETO;
			m_path[ m_index ].x3	= np3.x();
			m_path[ m_index ].y3	= np3.y();
		}
		else
		{
			m_path[ m_index ].code = ART_CURVETO;
			m_path[ m_index ].x1	= np1.x();
			m_path[ m_index ].y1	= np1.y();
			m_path[ m_index ].x2	= np2.x();
			m_path[ m_index ].y2	= np2.y();
			m_path[ m_index ].x3	= np3.x();
			m_path[ m_index ].y3	= np3.y();
		}
		m_index++;
	}
	ensureSpace( m_index + 1 );
	m_path[ m_index ].code = ART_END;
	m_index++;
#endif
}

void ScPainter::drawPolygon()
{
	fillPath();
}

void ScPainter::drawPolyLine()
{
	strokePath();
}

void ScPainter::drawLine(FPoint start, FPoint end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainter::drawRect(double x, double y, double w, double h)
{
	newPath();
	moveTo( x, y );
	lineTo( x+w, y );
	lineTo( x+w, y+h );
	lineTo( x, y+h );
	lineTo( x, y );
#ifdef HAVE_CAIRO
	cairo_close_path( m_cr );
#else
	ensureSpace( m_index + 1 );
	m_path[ m_index ].code = ART_END;
	m_index++;
#endif
	fillPath();
	strokePath();
}

#ifndef HAVE_CAIRO
void ScPainter::ensureSpace( unsigned int newindex )
{
	if( m_index == 0 )
		{
		if( !m_path )
			m_path = art_new( ArtBpath, INITIAL_ALLOC );
		m_alloccount = INITIAL_ALLOC;
		}
	else if( newindex > m_alloccount )
		{
		m_alloccount += ALLOC_INCREMENT;
		m_path = art_renew( m_path, ArtBpath, m_alloccount );
		}
}

void ScPainter::resize( unsigned int w, unsigned int h )
{
	if( !m_buffer || w != m_width || h != m_height )
	{
		// TODO : realloc?
		art_free( m_buffer );
		m_buffer = 0;
		m_width = w;
		m_height = h;
		if ( m_width != 0 && m_height != 0 )
			m_buffer = art_new( art_u8, m_width * m_height * 4 );
		clear();
	}
}

void ScPainter::clampToViewport( int &x0, int &y0, int &x1, int &y1 )
{
	x0 = QMAX( x0, 0 );
	x0 = QMIN( x0, static_cast<int>( m_width ) );
	y0 = QMAX( y0, 0 );
	y0 = QMIN( y0, static_cast<int>( m_height ) );
	x1 = QMAX( x1, 0 );
	x1 = QMIN( x1, static_cast<int>( m_width ) );
	y1 = QMAX( y1, 0 );
	y1 = QMIN( y1, static_cast<int>( m_height ) );
}

void ScPainter::clampToViewport( const ArtSVP &svp, int &x0, int &y0, int &x1, int &y1 )
{
	ArtDRect bbox;
	art_drect_svp( &bbox, &svp );
	x0 = static_cast<int>( bbox.x0 );
	x0 = QMAX( x0, 0 );
	x0 = QMIN( x0, static_cast<int>( m_width ) );
	y0 = static_cast<int>( bbox.y0 );
	y0 = QMAX( y0, 0 );
	y0 = QMIN( y0, static_cast<int>( m_height ) );
	x1 = static_cast<int>( bbox.x1 ) + 1;
	x1 = QMAX( x1, 0 );
	x1 = QMIN( x1, static_cast<int>( m_width ) );
	y1 = static_cast<int>( bbox.y1 ) + 1;
	y1 = QMAX( y1, 0 );
	y1 = QMIN( y1, static_cast<int>( m_height ) );
}

void ScPainter::applyGradient( ArtSVP *svp, bool fill )
{
	int x0, y0, x1, y1;
	clampToViewport( *svp, x0, y0, x1, y1 );
	ArtRender *render = 0L;
	VGradient gradient;
	if (fill)
		gradient = fill_gradient;
	else
		gradient = stroke_gradient;
	double opa = fill ? fill_trans : stroke_trans;
	if (gradient.type() == VGradient::linear)
	{
		ArtGradientLinear *linear = art_new( ArtGradientLinear, 1 );
		linear->spread = ART_GRADIENT_PAD;
		double _x1 = static_cast<double>(gradient.origin().x());
		double _x2 = static_cast<double>(gradient.vector().x());
		double _y2 = static_cast<double>(gradient.origin().y());
		double _y1 = static_cast<double>(gradient.vector().y());
		double dx = ( _x2 - _x1 ) * m_zoomFactor;
		_y1 =  _y1 + m_matrix.dy() / m_zoomFactor;
		_y2 =  _y2 + m_matrix.dy() / m_zoomFactor;
		if (dx == 0.0)
			dx = 1;
		double dy = ( _y1 - _y2 ) * m_zoomFactor;
		if (dy == 0.0)
			dy = 1;
		double scale = 1.0 / ( dx * dx + dy * dy );
		linear->a = dx * scale;
		linear->b = dy * scale;
		linear->c = -( ( _x1 * m_zoomFactor + m_matrix.dx() ) * linear->a + ( _y2 * m_zoomFactor ) * linear->b );
		int offsets = -1;
		linear->stops = buildStopArray( gradient, offsets );
		linear->n_stops = offsets;
		if( x0 != x1 && y0 != y1 )
		{
			render = art_render_new( x0, y0, x1, y1, m_buffer + 4 * static_cast<int>(x0) + m_width * 4 * static_cast<int>(y0), m_width * 4, 3, 8, ART_ALPHA_PREMUL, 0 );
			int opacity = static_cast<int>( opa * 255.0 );
			art_render_svp( render, svp );
			art_render_mask_solid (render, (opacity << 8) + opacity + (opacity >> 7));
			art_karbon_render_gradient_linear( render, linear, ART_FILTER_NEAREST );
			art_render_invoke( render );
		}
		art_free( linear->stops );
		art_free( linear );
	}
	else if( gradient.type() == VGradient::radial )
	{
		ArtGradientRadial *radial = art_new( ArtGradientRadial, 1 );
		radial->spread = ART_GRADIENT_PAD;
		radial->affine[0] = m_matrix.m11();
		radial->affine[1] = m_matrix.m12();
		radial->affine[2] = m_matrix.m21();
		radial->affine[3] = m_matrix.m22();
		radial->affine[4] = m_matrix.dx();
		radial->affine[5] = m_matrix.dy();
		double cx = gradient.origin().x() * m_zoomFactor;
		double cy = gradient.origin().y() * m_zoomFactor;
		double fx = gradient.focalPoint().x() * m_zoomFactor;
		double fy = gradient.focalPoint().y() * m_zoomFactor;
		double r = sqrt( pow( gradient.vector().x() - gradient.origin().x(), 2 ) + pow( gradient.vector().y() - gradient.origin().y(), 2 ) );
		r *= m_zoomFactor;
		radial->fx = (fx - cx) / r;
		radial->fy = (fy - cy) / r;
		double aff1[6], aff2[6];
		art_affine_scale( aff1, r, r);
		art_affine_translate( aff2, cx, cy );
		art_affine_multiply( aff1, aff1, aff2 );
		art_affine_multiply( aff1, aff1, radial->affine );
		art_affine_invert( radial->affine, aff1 );
		int offsets = -1;
		radial->stops = buildStopArray( gradient, offsets );
		radial->n_stops = offsets;
		if( x0 != x1 && y0 != y1 )
		{
			render = art_render_new( x0, y0, x1, y1, m_buffer + 4 * x0 + m_width * 4 * y0, m_width * 4, 3, 8, ART_ALPHA_PREMUL, 0 );
			int opacity = static_cast<int>( opa * 255.0 );
			art_render_svp( render, svp );
			art_render_mask_solid (render, (opacity << 8) + opacity + (opacity >> 7));
			art_karbon_render_gradient_radial( render, radial, ART_FILTER_NEAREST );
			art_render_invoke( render );
		}
		art_free( radial->stops );
		art_free( radial );
	}
}

ArtGradientStop * ScPainter::buildStopArray( VGradient &gradient, int &offsets )
{
	QPtrVector<VColorStop> colorStops = gradient.colorStops();
	offsets = colorStops.count();
	ArtGradientStop *stopArray = art_new( ArtGradientStop, offsets * 2 - 1 );
	for( int offset = 0 ; offset < offsets ; offset++ )
	{
		double ramp = colorStops[ offset ]->rampPoint;
		stopArray[ offset * 2 ].offset = ramp;
		QColor qStopColor = colorStops[ offset ]->color;
		int h, s, v, sneu;
		int shad = colorStops[offset]->shade;
		qStopColor.rgb(&h, &s, &v);
		if ((h == s) && (s == v))
		{
			qStopColor.hsv(&h, &s, &v);
			sneu = 255 - ((255 - v) * shad / 100);
			qStopColor.setHsv(h, s, sneu);
		}
		else
		{
			qStopColor.hsv(&h, &s, &v);
			sneu = s * shad / 100;
			qStopColor.setHsv(h, sneu, v);
		}
		int r = qRed( qStopColor.rgb() );
		int g = qGreen( qStopColor.rgb() );
		int b = qBlue( qStopColor.rgb() );
		art_u32 rgba = (r << 24) | (g << 16) | (b << 8) | qAlpha(qStopColor.rgb());
		int a = static_cast<int>( colorStops[offset]->opacity * 255.0 );
		r = (rgba >> 24) * a + 0x80;
		r = (r + (r >> 8)) >> 8;
		g = ((rgba >> 16) & 0xff) * a + 0x80;
		g = (g + (g >> 8)) >> 8;
		b = ((rgba >> 8) & 0xff) * a + 0x80;
		b = (b + (b >> 8)) >> 8;
		stopArray[ offset * 2 ].color[ 0 ] = ART_PIX_MAX_FROM_8(r);
		stopArray[ offset * 2 ].color[ 1 ] = ART_PIX_MAX_FROM_8(g);
		stopArray[ offset * 2 ].color[ 2 ] = ART_PIX_MAX_FROM_8(b);
		stopArray[ offset * 2 ].color[ 3 ] = ART_PIX_MAX_FROM_8(a);
		if( offset + 1 != offsets )
		{
			stopArray[ offset * 2 + 1 ].offset = ramp + ( colorStops[ offset + 1 ]->rampPoint - ramp ) * colorStops[ offset ]->midPoint;
			QColor qStopColor2 = colorStops[ offset + 1 ]->color;
			rgba = static_cast<int>(r + ((qRed(qStopColor2.rgb()) - r)) * 0.5) << 24 |
						 static_cast<int>(g + ((qGreen(qStopColor2.rgb()) - g)) * 0.5) << 16 |
						 static_cast<int>(b + ((qBlue(qStopColor2.rgb()) - b)) * 0.5) << 8 |
						qAlpha(qStopColor2.rgb());
			int a = static_cast<int>( colorStops[offset]->opacity * 255.0 );
			r = (rgba >> 24) * a + 0x80;
			r = (r + (r >> 8)) >> 8;
			g = ((rgba >> 16) & 0xff) * a + 0x80;
			g = (g + (g >> 8)) >> 8;
			b = ((rgba >> 8) & 0xff) * a + 0x80;
			b = (b + (b >> 8)) >> 8;
			stopArray[ offset * 2 + 1 ].color[ 0 ] = ART_PIX_MAX_FROM_8(r);
			stopArray[ offset * 2 + 1 ].color[ 1 ] = ART_PIX_MAX_FROM_8(g);
			stopArray[ offset * 2 + 1 ].color[ 2 ] = ART_PIX_MAX_FROM_8(b);
			stopArray[ offset * 2 + 1 ].color[ 3 ] = ART_PIX_MAX_FROM_8(a);
		}
	}
	offsets = offsets * 2 - 1;
	return stopArray;
}
#endif
