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
#include "config.h"
#include <qpaintdevice.h>
#include <qpixmap.h>
#include <qpointarray.h>
#include <qimage.h>

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
#include "art_render_misc.h"
#include <libart_lgpl/art_render_svp.h>
#include <libart_lgpl/art_vpath_svp.h>

#include "art_rgb_svp.h"
#include "art_render_pattern.h"

#include <X11/Xlib.h>

#include <gdk-pixbuf-xlibrgb.h>
#include <math.h>
#include "art_kmisc.h"

#define INITIAL_ALLOC	300
#define ALLOC_INCREMENT	100

ScPainter::ScPainter( QPaintDevice *target, unsigned int w, unsigned int h )
{
	m_target = target;
	m_width = w;
	m_height= h;
	m_buffer = 0L;
	m_path = 0L;
	m_index = 0;
	resize( m_width, m_height );
	clear();
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
	fill_gradient = VGradient(VGradient::linear);
	stroke_gradient = VGradient(VGradient::linear);
	xlib_rgb_init_with_depth( target->x11Display(), XScreenOfDisplay( target->x11Display(),
							  target->x11Screen() ), target->x11Depth() );

	gc = XCreateGC( target->x11Display(), target->handle(), 0, 0 );
	m_zoomFactor = 1;
}

ScPainter::~ScPainter()
{
	// If we are in target mode, we created a buffer, else if we used the other ctor
	// we didnt.
	if( m_target )
		art_free( m_buffer );
	if( m_path )
		art_free( m_path );
	if( gc )
		XFreeGC( m_target->x11Display(), gc );
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

void ScPainter::begin()
{
}

void ScPainter::end()
{
	xlib_draw_rgb_32_image( m_target->handle(), gc, 0, 0, m_width, m_height,
						 XLIB_RGB_DITHER_NONE, m_buffer, m_width * 4 );
}

void
ScPainter::clear()
{
	if( m_buffer )
		memset( m_buffer, qRgba( 255, 255, 255, 255 ), m_width * m_height * 4 );
}

void
ScPainter::clear( const QColor &c )
{
	if( m_buffer )
		memset( m_buffer, c.rgb(), m_width * m_height * 4 );
}

void ScPainter::setWorldMatrix( const QWMatrix &mat )
{
	m_matrix = mat;
}

void ScPainter::setZoomFactor( double zoomFactor )
{
	m_zoomFactor = zoomFactor;
}

void ScPainter::translate( double x, double y )
{
	m_matrix.translate(x, y);
}

void ScPainter::rotate( double r )
{
	m_matrix.rotate(r);
}

void ScPainter::scale( double x, double y )
{
	m_matrix.scale(x, y);
}

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

void ScPainter::moveTo( const double &x, const double &y )
{
	ensureSpace( m_index + 1 );
	m_path[ m_index ].code = ART_MOVETO;
	m_path[ m_index ].x3 = x * m_zoomFactor;
	m_path[ m_index ].y3 = y * m_zoomFactor;
	m_index++;
}

void 
ScPainter::lineTo( const double &x, const double &y )
{
	ensureSpace( m_index + 1 );
	m_path[ m_index ].code = ART_LINETO;
	m_path[ m_index ].x3	= x * m_zoomFactor;
	m_path[ m_index ].y3	= y * m_zoomFactor;
	m_index++;
}

void ScPainter::curveTo( FPoint p1, FPoint p2, FPoint p3 )
{
	ensureSpace( m_index + 1 );
	m_path[ m_index ].code = ART_CURVETO;
	m_path[ m_index ].x1	= p1.x() * m_zoomFactor;
	m_path[ m_index ].y1	= p1.y() * m_zoomFactor;
	m_path[ m_index ].x2	= p2.x() * m_zoomFactor;
	m_path[ m_index ].y2	= p2.y() * m_zoomFactor;
	m_path[ m_index ].x3	= p3.x() * m_zoomFactor;
	m_path[ m_index ].y3	= p3.y() * m_zoomFactor;
	m_index++;
}

void ScPainter::newPath()
{
	m_index = 0;
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
//	fill_gradient.setRepeatMethod( VGradient::reflect );
}

void ScPainter::fillPath()
{
	if( m_index == 0 ) return;
	int find = -1;
	for( int i = m_index - 1; i >= 0; i-- )
		{
		if( m_path[i].code == ART_MOVETO_OPEN || m_path[i].code == ART_MOVETO )
			{
			find = i;
			break;
			}
		}
	if( find != -1 && ( m_path[ find ].x3 != m_path[ m_index - 1 ].x3 ||
						m_path[ find ].y3 != m_path[ m_index - 1 ].y3 ) )
		{
		ensureSpace( m_index + 1 );
		m_path[ m_index ].code = ART_LINETO;
		m_path[ m_index ].x3	= m_path[ find ].x3;
		m_path[ m_index ].y3	= m_path[ find ].y3;
		m_index++;
		m_path[ m_index ].code = ART_END;
		}
	else
		m_path[ m_index++ ].code = ART_END;
	if( fillMode != 0)
		{
		ArtVpath *path = art_bez_path_to_vec( m_path , 0.25 );
		drawVPath( path, 0 );
		}
	m_index--;
}

void ScPainter::strokePath()
{
	if( m_index == 0 )
		return;
	if( LineWidth == 0 )
		return;
	if( m_path[ m_index ].code != ART_END)
		m_path[ m_index ].code = ART_END;
	ArtVpath *path = art_bez_path_to_vec( m_path , 0.25 );
	drawVPath( path, 1 );
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
	MStack.push(m_matrix);
}

void ScPainter::restore()
{
	m_matrix = MStack.pop();
}

void ScPainter::setRasterOp( Qt::RasterOp  )
{
}

void ScPainter::clampToViewport( int &x0, int &y0, int &x1, int &y1 )
{
	// clamp to viewport
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
	// get SVP bbox
	ArtDRect bbox;
	art_drect_svp( &bbox, &svp );
	// Remove comments if we really decide for SVP bbox usage
	//m_bbox = KoRect( bbox.x0, bbox.y0, bbox.x1 - bbox.x0, bbox.y1 - bbox.y0 );

	// clamp to viewport
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

void ScPainter::drawVPath( ArtVpath *vec, int mode )
{
	ArtSVP *strokeSvp = 0L;
	ArtSVP *fillSvp = 0L;
	// set up world matrix
	double affine[6];
	affine[0] = m_matrix.m11();
	affine[1] = m_matrix.m12();   // was 0 in Karbon
	affine[2] = m_matrix.m21();   // was 0 in Karbon, don't know why
	affine[3] = m_matrix.m22();
	affine[4] = m_matrix.dx();
	affine[5] = m_matrix.dy();
	ArtVpath *temp1 = art_vpath_affine_transform( vec, affine );
	art_free( vec );
	vec = temp1;
	int af = 0;
	int as = 0;
	art_u32 fillColor = 0;
	// filling
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
		double ratio = m_zoomFactor;//sqrt(pow(affine[0], 2) + pow(affine[3], 2)) / sqrt(2);
		if( m_array.count() > 0 )
			{
			// there are dashes to be rendered
			ArtVpathDash dash;
			dash.offset = static_cast<double>(m_offset) * ratio;
			dash.n_dash = m_array.count();
			double *dashes = new double[ dash.n_dash ];
			for( int i = 0; i < dash.n_dash; i++ )
				{
				dashes[i] = static_cast<double>(m_array[i]) * ratio;
				}
			dash.dash = dashes;
			// get the dashed VPath and use that for the stroke render operation
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

	// render the svp to the buffer

	if( fillSvp )
		{
		if (fillMode == 2)
			applyGradient( fillSvp, true );
		else
			{
/*		if( m_fill && m_fill->type() == VFill::grad )
			applyGradient( fillSvp, true );
		else if( m_fill && m_fill->type() == VFill::patt )
			applyPattern( fillSvp, true );
		else
		{ */
			clampToViewport( *fillSvp, x0, y0, x1, y1 );
			if( x0 != x1 && y0 != y1 )
				art_rgb_svp_alpha_( fillSvp, x0, y0, x1, y1, fillColor, af, m_buffer + x0 * 4 + y0 * m_width * 4, m_width * 4, 0 );
			}
		art_svp_free( fillSvp );
		}
	if( strokeSvp )
	{
/*		if( m_stroke && m_stroke->type() == VStroke::grad )
			applyGradient( strokeSvp, false );
		else if( m_stroke && m_stroke->type() == VStroke::patt )
			applyPattern( strokeSvp, false );
		else
		{ */
			clampToViewport( *strokeSvp, x0, y0, x1, y1 );
			if( x0 != x1 && y0 != y1 )
				art_rgb_svp_alpha_( strokeSvp, x0, y0, x1, y1, strokeColor, as, m_buffer + x0 * 4 + y0 * m_width * 4, m_width * 4, 0 );
//		}
		art_svp_free( strokeSvp );
	}
	art_free( vec );
}
/*
void
VKoPainter::applyPattern( ArtSVP *svp, bool fill )
{
	int x0, y0, x1, y1;
	clampToViewport( *svp, x0, y0, x1, y1 );

	ArtRender *render = 0L;

	VPattern pat = fill ? m_fill->pattern() : m_stroke->pattern();
	if( !pat.isValid() )
		pat.load( "pics/cr32-app-karbon.png" );

	ArtPattern *pattern = art_new( ArtPattern, 1 );

	double dx = ( pat.vector().x() - pat.origin().x() ) * m_zoomFactor;
	double dy = ( pat.vector().y() - pat.origin().y() ) * m_zoomFactor;

	pattern->twidth = pat.tileWidth();
	pattern->theight = pat.tileHeight();
	pattern->buffer = pat.pixels();
	pattern->opacity = fill ? short( m_fill->color().opacity() * 255.0 ) : short( m_stroke->color().opacity() * 255.0 );
	pattern->angle = atan2( dy, dx );

	if( x0 != x1 && y0 != y1 )
	{
		render = art_render_new( x0, y0, x1, y1, m_buffer + 4 * int(x0) + m_width * 4 * int(y0), m_width * 4, 3, 8, ART_ALPHA_PREMUL, 0 );
		art_render_svp( render, svp );
		art_render_pattern( render, pattern, ART_FILTER_HYPER );
	}

	if( render )
		art_render_invoke( render );
	art_free( pattern );
}
*/
void ScPainter::applyGradient( ArtSVP *svp, bool fill )
{
	int x0, y0, x1, y1;
	clampToViewport( *svp, x0, y0, x1, y1 );

	ArtRender *render = 0L;

	VGradient gradient = fill ? fill_gradient : stroke_gradient;
	double opa = fill ? fill_trans : stroke_trans;

	if (gradient.type() == VGradient::linear)
		{
		ArtGradientLinear *linear = art_new( ArtGradientLinear, 1 );

		// TODO : make variable
		if( gradient.repeatMethod() == VGradient::none )
			linear->spread = ART_GRADIENT_PAD;
		else if( gradient.repeatMethod() == VGradient::repeat )
			linear->spread = ART_GRADIENT_REPEAT;
		else if( gradient.repeatMethod() == VGradient::reflect )
			linear->spread = ART_GRADIENT_REFLECT;

		double _x1 = static_cast<double>(gradient.origin().x());
		double _x2 = static_cast<double>(gradient.vector().x());
		double _y2 = static_cast<double>(gradient.origin().y());
		double _y1 = static_cast<double>(gradient.vector().y());

		double dx = ( _x2 - _x1 ) * m_zoomFactor;
		_y1 = /* m_matrix.m22() * */ _y1 + m_matrix.dy() / m_zoomFactor;
		_y2 = /* m_matrix.m22() * */ _y2 + m_matrix.dy() / m_zoomFactor;

		if (dx == 0.0)
			dx = 1;
		double dy = ( _y1 - _y2 ) * m_zoomFactor;
		if (dy == 0.0)
			dy = 1;
		double scale = 1.0 / ( dx * dx + dy * dy );

		linear->a = dx * scale;
		linear->b = dy * scale;
		linear->c = -( ( _x1 * m_zoomFactor + m_matrix.dx() ) * linear->a +
					   ( _y2 * m_zoomFactor ) * linear->b );

		// get stop array
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

		// TODO : make variable
		if( gradient.repeatMethod() == VGradient::none )
			radial->spread = ART_GRADIENT_PAD;
		else if( gradient.repeatMethod() == VGradient::repeat )
			radial->spread = ART_GRADIENT_REPEAT;
		else if( gradient.repeatMethod() == VGradient::reflect )
			radial->spread = ART_GRADIENT_REFLECT;

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
		double r = sqrt( pow( gradient.vector().x() - gradient.origin().x(), 2 ) +
						 pow( gradient.vector().y() - gradient.origin().y(), 2 ) );
		r *= m_zoomFactor;

		radial->fx = (fx - cx) / r;
		radial->fy = (fy - cy) / r;

		double aff1[6], aff2[6];
		art_affine_scale( aff1, r, r);
		art_affine_translate( aff2, cx, cy );
		art_affine_multiply( aff1, aff1, aff2 );
		art_affine_multiply( aff1, aff1, radial->affine );
		art_affine_invert( radial->affine, aff1 );

		// get stop array
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
	else if( gradient.type() == VGradient::conic )
	{
		ArtGradientConical *conical = art_new( ArtGradientConical, 1 );

		// TODO : make variable
		if( gradient.repeatMethod() == VGradient::none )
			conical->spread = ART_GRADIENT_PAD;
		else if( gradient.repeatMethod() == VGradient::repeat )
			conical->spread = ART_GRADIENT_REPEAT;
		else if( gradient.repeatMethod() == VGradient::reflect )
			conical->spread = ART_GRADIENT_REFLECT;

		double cx = gradient.origin().x() * m_zoomFactor;
		cx = m_matrix.m11() * cx + m_matrix.dx();
		double cy = gradient.origin().y() * m_zoomFactor;
		cy = m_matrix.m22() * cy + m_matrix.dy();
		double r = sqrt( pow( gradient.vector().x() - gradient.origin().x(), 2 ) +
						 pow( gradient.vector().y() - gradient.origin().y(), 2 ) );
		r *= m_zoomFactor;

		conical->cx = cx;
		conical->cy = cy;
		conical->r  = r;

		// get stop array
		int offsets = -1;
		conical->stops = buildStopArray( gradient, offsets );
		conical->n_stops = offsets;

		if( x0 != x1 && y0 != y1 )
		{
			render = art_render_new( x0, y0, x1, y1, m_buffer + 4 * x0 + m_width * 4 * y0, m_width * 4, 3, 8, ART_ALPHA_PREMUL, 0 );
			int opacity = static_cast<int>( opa * 255.0 );
			art_render_svp( render, svp );
			art_render_mask_solid (render, (opacity << 8) + opacity + (opacity >> 7));
			art_karbon_render_gradient_conical( render, conical, ART_FILTER_NEAREST );
			art_render_invoke( render );
		}
		art_free( conical->stops );
		art_free( conical );
	}
}


ArtGradientStop * ScPainter::buildStopArray( VGradient &gradient, int &offsets )
{
	// TODO : make this generic
	QPtrVector<VColorStop> colorStops = gradient.colorStops();
	offsets = colorStops.count();

	ArtGradientStop *stopArray = art_new( ArtGradientStop, offsets * 2 - 1 );

	for( int offset = 0 ; offset < offsets ; offset++ )
	{
		double ramp = colorStops[ offset ]->rampPoint;
		//double mid  = colorStops[ offset ]->midPoint;
		stopArray[ offset * 2 ].offset = ramp;

		QColor qStopColor = colorStops[ offset ]->color;
		int r = qRed( qStopColor.rgb() );
		int g = qGreen( qStopColor.rgb() );
		int b = qBlue( qStopColor.rgb() );
		art_u32 rgba = (r << 24) | (g << 16) | (b << 8) | qAlpha(qStopColor.rgb());
		int a = static_cast<int>( colorStops[offset]->opacity * 255.0 );
//		int a = int( fill_trans * 255.0 );
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
//			int a = int( fill_trans * 255.0 );
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

void ScPainter::drawImage( const QImage &image )
{
	double affineresult[6];
	affineresult[0] = m_matrix.m11() * m_zoomFactor;
	affineresult[1] = m_matrix.m12() * m_zoomFactor;
	affineresult[2] = m_matrix.m21() * m_zoomFactor;
	affineresult[3] = m_matrix.m22() * m_zoomFactor;
	affineresult[4] = m_matrix.dx(); // * m_zoomFactor;
	affineresult[5] = m_matrix.dy(); // * m_zoomFactor;
	ksvg_art_rgb_affine( m_buffer, 0, 0, m_width, m_height, m_width * 4,
					 image.bits(), image.width(), image.height(), image.width() * 4,
					 affineresult, ART_FILTER_NEAREST, 0L, qRound( 255 * fill_trans ) );
}

void ScPainter::setupPolygon(FPointArray *points)
{
	bool nPath = true;
	FPoint np, np1, np2, np3;
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
				ensureSpace( m_index + 1 );
				m_path[ m_index ].code = ART_MOVETO;
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
		}
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
	fillPath();
	strokePath();
}

void ScPainter::drawUnderline(FPoint x0, FPoint x2, bool ps, QString *psst)
{
	unsigned int m_index2 = 0;
	unsigned int m_alloccount2;
	m_path2 = 0L;
	m_path2 = art_new( ArtBpath, INITIAL_ALLOC );
	m_alloccount2 = INITIAL_ALLOC;
	m_path2[ m_index2 ].code = ART_MOVETO;
	m_path2[ m_index2 ].x3 = x0.x() * m_zoomFactor;
	m_path2[ m_index2 ].y3 = x0.y() * m_zoomFactor;
	m_index2++;
	m_alloccount2 += ALLOC_INCREMENT;
	m_path2 = art_renew( m_path2, ArtBpath, m_alloccount2 );
	m_path2[ m_index2 ].code = ART_LINETO;
	m_path2[ m_index2 ].x3	= x2.x() * m_zoomFactor;
	m_path2[ m_index2 ].y3	= x2.y() * m_zoomFactor;
	m_index2++;
	if( m_path[ m_index ].code != ART_END)
		m_path[ m_index ].code = ART_END;
	ArtVpath *path = art_bez_path_to_vec( m_path , 0.25 );
	m_path2[ m_index2 ].code = ART_END;
	ArtVpath *path2 = art_bez_path_to_vec( m_path2 , 0.25 );

	ArtSVP *strokeSvp = 0L;
	ArtSVP *stroke2Svp = 0L;
	ArtSVP *stroke3Svp = 0L;
	ArtSVP *stroke4Svp = 0L;
	ArtSVP *stroke5Svp = 0L;
	ArtSVP *stroke6Svp = 0L;
	art_u32 strokeColor = 0;
	// set up world matrix
	double affine[6];
	affine[0] = m_matrix.m11();
	affine[1] = m_matrix.m12();   // was 0 in Karbon
	affine[2] = m_matrix.m21();   // was 0 in Karbon, don't know why
	affine[3] = m_matrix.m22();
	affine[4] = m_matrix.dx();
	affine[5] = m_matrix.dy();
	ArtVpath *temp1 = art_vpath_affine_transform( path, affine );
	art_free( path );
	ArtVpath *temp2 = art_vpath_affine_transform( path2, affine );
	art_free( path2 );
	int as = 0;
	ArtPathStrokeCapType capStyle = ART_PATH_STROKE_CAP_BUTT;
	ArtPathStrokeJoinType joinStyle = ART_PATH_STROKE_JOIN_MITER;
	QColor color = m_stroke;
	as = qRound( 255 * stroke_trans );
	strokeColor = ( 0 << 24 ) | ( color.blue() << 16 ) | ( color.green() << 8 ) | color.red();
	double ratio = m_zoomFactor;
	ArtSvpWriter *swr;
	ArtSVP *temp;
	temp = art_svp_from_vpath( temp1 );
//	if( m_fillRule )
		swr = art_svp_writer_rewind_new( ART_WIND_RULE_ODDEVEN );
//	else
//		swr = art_svp_writer_rewind_new( ART_WIND_RULE_NONZERO );
	art_svp_intersector( temp, swr );
	strokeSvp = art_svp_writer_rewind_reap( swr );
	art_svp_free( temp );
	stroke5Svp = art_svp_vpath_stroke( temp1, joinStyle, capStyle, ratio * LineWidth, 10, 0.25 );
	stroke6Svp = art_svp_union(strokeSvp, stroke5Svp);
	stroke2Svp = art_svp_vpath_stroke( temp2, joinStyle, capStyle, ratio * LineWidth, 10, 0.25 );
	stroke3Svp = art_svp_intersect(stroke6Svp, stroke2Svp);
	stroke4Svp = art_svp_diff(stroke3Svp, stroke2Svp);
	int x_0, y0, x1, y1;
	clampToViewport( *stroke4Svp, x_0, y0, x1, y1 );
	if( x_0 != x1 && y0 != y1 )
		art_rgb_svp_alpha_( stroke4Svp, x_0, y0, x1, y1, strokeColor, as, m_buffer + x_0 * 4 + y0 * m_width * 4, m_width * 4, 0 );
	art_svp_free( strokeSvp );
	art_svp_free( stroke2Svp );
	art_svp_free( stroke3Svp );
	art_svp_free( stroke5Svp );
	art_svp_free( stroke6Svp );
	if (ps)
		{
  	int i;
		QString tmp;
		ArtVpath *vpath = art_vpath_from_svp(stroke4Svp);
  	for (i = 0; vpath[i].code != ART_END; i++)
			{
			tmp.setNum(vpath[i].x - m_matrix.dx());
			*psst += tmp+" ";
			tmp.setNum(vpath[i].y - x0.y());
			*psst += tmp+" ";
			if (vpath[i].code == ART_MOVETO)
				*psst += "m";
			if (vpath[i].code == ART_LINETO)
				*psst += "l";
			*psst += "\n";
			}
		art_free( vpath );
		}
	art_svp_free( stroke4Svp );
	art_free( temp1 );
	art_free( temp2 );
}
