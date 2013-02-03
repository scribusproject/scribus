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
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
/* Adapted for Scribus 22.08.2003 by Franz Schmid */
// kopainter/libart wrapper

#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

#include <valarray>
#include <windows.h>

#include "scconfig.h"

#include <gdiplus.h>
using namespace Gdiplus;

#include "scribusdoc.h"
#include "scpainterex_gdiplus.h"
#include "sccolorengine.h"
#include "util.h"

#include <QImage>
#include <QList>
#include <QPaintDevice>
#include <QPixmap>

#include <iostream>
using namespace std;
 
#include <math.h>

ScPainterEx_GDIPlus::ScPainterEx_GDIPlus( HDC hDC, QRect& rect,  ScribusDoc* doc, bool gray ) : ScPainterExBase()
{
	m_doc    = doc;
	m_width  = rect.width();
	m_height = rect.height();
	m_strokeColor = ScColorShade( QColor(0,0,0), 100 );
	m_fillColor = ScColorShade( QColor(0,0,0), 100 );
	m_fillTrans = 1.0;
	m_strokeTrans = 1.0;
	m_fillRule  = true;
	m_fillMode  = 1;
	m_lineWidth = 1.0;
	m_offset    = 0;
	m_lineEnd   = Qt::SquareCap;
	m_lineJoin  = Qt::RoundJoin;
	m_fillGradient   = VGradientEx(VGradientEx::linear);
	m_strokeGradient = VGradientEx(VGradientEx::linear);
	// Grayscale conversion parameter
	m_convertToGray = gray;
	// Initialization of Windows GDI data
	m_dc = hDC;
	m_pathIsClosed = true;
	m_drawingClosedPath = false;
	m_positionX = m_positionY = 0;
	m_graphics  = new Gdiplus::Graphics( m_dc );
	m_graphics->SetCompositingMode( CompositingModeSourceOver );
	m_graphics->SetCompositingQuality( CompositingQualityHighQuality );
	m_graphics->SetSmoothingMode( SmoothingModeHighQuality );
	m_graphics->SetPageUnit( UnitPoint );
	m_graphicsPath = NULL;
	clear();
}

ScPainterEx_GDIPlus::~ScPainterEx_GDIPlus()
{
	if ( m_graphicsPath )
		delete m_graphicsPath;
	if ( m_graphics )
		delete m_graphics;
}

QColor ScPainterEx_GDIPlus::transformColor( ScColorShade& colorShade, double trans )
{
	QColor c, color;
	c = ScColorEngine::getShadeColor(colorShade.color, m_doc, colorShade.shade);
	color = qRgba( qRed(c.rgb()), qGreen(c.rgb()), qBlue(c.rgb()), qAlpha(trans * 255));
	if ( m_convertToGray )
	{
		int grayLevel;
		QColor grayColor;
		grayLevel = 0.3 * color.red() + 0.59 * color.green() + 0.11 * color.blue();
		grayColor = qRgba( grayLevel, grayLevel, grayLevel, qAlpha(color.rgb()));
		color = grayColor;
	}
	return color;
}

void ScPainterEx_GDIPlus::transformImage( QImage* image, uchar* data, int scan)
{
	int rgb, grey;
	int imageWidth = image->width();
	int imageHeight = image->height();
	QRgb* imageBits = (QRgb *) image->bits();
	QRgb*  pscani;
	uchar *pscand;

	pscand = data;
	for ( int j = 0; j < imageHeight; j++ )
	{
		pscani = (QRgb *) image->scanLine(j);
		for (int i = 0; i < imageWidth; i++ )
		{
			rgb = pscani[i];
			grey = 0.3 * qRed(rgb) + 0.59 * qGreen(rgb) + 0.11 * qBlue(rgb);
			pscand[i] = grey;
		}
		pscand += scan;
	}
}

void ScPainterEx_GDIPlus::begin()
{
}

void ScPainterEx_GDIPlus::end()
{
	m_graphics->ResetTransform();
}

void ScPainterEx_GDIPlus::clear()
{
	SolidBrush brush( Color(255,255,255) );
	m_graphics->FillRectangle(&brush, 0, 0, m_width, m_height);
}

void ScPainterEx_GDIPlus::clear( ScColorShade &c )
{
	QColor color = transformColor( c, 1.0 );
	QRgb cs = color.rgb();
	SolidBrush brush( Color(qAlpha(cs), qRed(cs), qGreen(cs),qBlue(cs)) );
	m_graphics->FillRectangle(&brush, 0, 0, m_width, m_height);
}

const QMatrix ScPainterEx_GDIPlus::worldMatrix()
{
	return m_matrix;
}

void ScPainterEx_GDIPlus::setWorldMatrix( const QMatrix &mat )
{
	m_matrix = mat;
}

void ScPainterEx_GDIPlus::transformPoint( const FPoint& in, FPoint& out )
{
	double x, y;
	x = in.x() * m_matrix.m11() + in.y() * m_matrix.m21() + m_matrix.dx();
	y = in.x() * m_matrix.m12() + in.y() * m_matrix.m22() + m_matrix.dy();
	out.setX( x );
	out.setY( y );
}

void ScPainterEx_GDIPlus::transformPoints( const FPoint* ArrayIn, FPoint* ArrayOut, uint length )
{
	for( uint i = 0; i < length; i++ )
	{
		transformPoint( ArrayIn[i], ArrayOut[i] );
	}
}

void ScPainterEx_GDIPlus::translate( double x, double y )
{
	m_matrix.translate(x, y);
}

void ScPainterEx_GDIPlus::rotate( double r )
{
	m_matrix.rotate(r);
}

void ScPainterEx_GDIPlus::scale( double x, double y )
{
	m_matrix.scale(x, y);
}

void ScPainterEx_GDIPlus::moveTo( const double &x, const double &y )
{
	FPoint pp( x, y );
	transformPoint( pp, pp );
	m_positionX = pp.x();
	m_positionY = pp.y();
}

void ScPainterEx_GDIPlus::lineTo( const double &x, const double &y )
{
	FPoint pp( x, y );
	transformPoint( pp, pp );
	m_graphicsPath->AddLine( (REAL) m_positionX, (REAL) m_positionY, pp.x(), pp.y() );
	m_positionX = pp.x();
	m_positionY = pp.y();
}

void ScPainterEx_GDIPlus::curveTo( FPoint p1, FPoint p2, FPoint p3 )
{
	FPoint fpoints[3];
	fpoints[0].setXY( p1.x(), p1.y() );
	fpoints[1].setXY( p2.x(), p2.y() );
	fpoints[2].setXY( p3.x(), p3.y() );
	transformPoints( fpoints, fpoints, 3 );
	double p1x = fpoints[0].x();
	double p1y = fpoints[0].y();
	double p2x = fpoints[1].x();
	double p2y = fpoints[1].y();
	double p3x = fpoints[2].x();
	double p3y = fpoints[2].y();
	m_graphicsPath->AddBezier( (REAL) m_positionX, (REAL) m_positionY, p1x, p1y, p2x, p2y, p3x, p3y );
	m_positionX = p3x;
	m_positionY = p3y;
}

void ScPainterEx_GDIPlus::newPath()
{
	if( m_graphicsPath )
		delete m_graphicsPath;
	m_graphicsPath = new Gdiplus::GraphicsPath(); 
	m_pathIsClosed = false;
	m_drawingClosedPath = false;
}

void ScPainterEx_GDIPlus::setFillRule( bool fillRule )
{
	m_fillRule = fillRule;
}

void ScPainterEx_GDIPlus::setFillMode( int fill )
{
	m_fillMode = fill;
}

void ScPainterEx_GDIPlus::setGradient(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc)
{
	m_fillGradient.setType(mode);
	m_fillGradient.setOrigin(orig);
	m_fillGradient.setVector(vec);
	m_fillGradient.setFocalPoint(foc);
}

void ScPainterEx_GDIPlus::setPattern( ScPattern* pattern, QMatrix& patternTransform )
{
	m_pattern = pattern;
	m_patternTransform = patternTransform;
}

void ScPainterEx_GDIPlus::fillTextPath()
{
	drawVPath( 0 );
}

void ScPainterEx_GDIPlus::strokeTextPath()
{
	if( m_lineWidth == 0 )
		return;
	drawVPath( 1 );
}

void ScPainterEx_GDIPlus::fillPath()
{
	m_pathIsClosed = true;
	if( m_fillMode != 0)
		drawVPath( 0 );
}

void ScPainterEx_GDIPlus::strokePath()
{
	if( m_lineWidth == 0 )
		return;
	m_pathIsClosed = true;
	drawVPath( 1 );
}

ScColorShade ScPainterEx_GDIPlus::pen()
{
	return m_strokeColor;
}

ScColorShade ScPainterEx_GDIPlus::brush()
{
	return m_fillColor;
}

void ScPainterEx_GDIPlus::setPen( const ScColorShade &c )
{
	m_strokeColor = c;
}

void ScPainterEx_GDIPlus::setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo )
{
	m_strokeColor = c;
	m_lineWidth = w;
	m_lineEnd = ca;
	m_lineJoin = jo;
//	double Dt = qMax(1*w, 1.0);
//	double Sp = qMax(2*w, 1.0);
//	double Da = qMax(4*w, 1.0);
//	QList<double> tmp;
	m_array.clear();
	m_offset = 0;
	getDashArray(st, w, m_array);
/*	switch (st)
	{
		case Qt::SolidLine:
			break;
		case Qt::DashLine:
			m_array.append(Da);
			m_array.append(Sp);
			break;
		case Qt::DotLine:
			m_array.append(Dt);
			m_array.append(Sp);
#ifdef SC_USE_GDIPLUS
			m_array.append(Dt);
#endif
			break;
		case Qt::DashDotLine:
			m_array.append(Da);
			m_array.append(Sp);
			m_array.append(Dt);
			m_array.append(Sp);
			break;
		case Qt::DashDotDotLine:
			m_array.append(Da);
			m_array.append(Sp);
			m_array.append(Dt);
			m_array.append(Sp);
			m_array.append(Dt);
			m_array.append(Sp);
			break;
		default:
			break;
	} */
}

void ScPainterEx_GDIPlus::setLineWidth( double w )
{
	m_lineWidth = w;
}

void ScPainterEx_GDIPlus::setPenOpacity( double op )
{
	m_strokeTrans = op;
}


void ScPainterEx_GDIPlus::setDash(const QList<double>& array, double ofs)
{
	m_array = array;
	m_offset = ofs;
}

void ScPainterEx_GDIPlus::setBrush( const ScColorShade &c )
{
	m_fillColor = c;
}

void ScPainterEx_GDIPlus::setBrushOpacity( double op )
{
	m_fillTrans = op;
}

void ScPainterEx_GDIPlus::setOpacity( double op )
{
	m_fillTrans = op;
	m_strokeTrans = op;
}

void ScPainterEx_GDIPlus::setFont( const QFont &f)
{
	m_font = f;
}

QFont ScPainterEx_GDIPlus::font()
{
	return m_font;
}

void ScPainterEx_GDIPlus::save()
{
	m_stack.push( m_matrix );
	m_gStates.push( m_graphics->Save() );
}

void ScPainterEx_GDIPlus::restore()
{
	m_matrix = m_stack.pop();
	m_graphics->Restore( m_gStates.pop() );
}

void ScPainterEx_GDIPlus::setRasterOp( int   )
{
}

void ScPainterEx_GDIPlus::drawVPath( int mode )
{
	save();
	if (mode == 0)
	{
		if( m_fillRule )
			m_graphicsPath->SetFillMode( FillModeAlternate);
		else
			m_graphicsPath->SetFillMode( FillModeWinding);
		if (m_fillMode == ScPainterExBase::Gradient)
		{
			drawGradient( m_fillGradient );
		}
		else
		{
			QColor fillColor = transformColor(m_fillColor, 1.0);
			SolidBrush fill_brush( Color(m_fillTrans * 255, fillColor.red(), fillColor.green(), fillColor.blue()) );
			m_graphics->FillPath( &fill_brush, m_graphicsPath );
		}
	}
	else
	{
		double m11 = m_matrix.m11();
		double m12 = m_matrix.m12();
		double m21 = m_matrix.m21();
		double m22 = m_matrix.m22();
		double norm2 = m11 * m11 + m12 * m12 + m21 * m21 + m22 * m22;
		double penScale = sqrt(norm2 / 2.0);
		double penWidth =  m_lineWidth * penScale;
		QColor strokeColor = transformColor( m_strokeColor, 1.0 );
		SolidBrush stroke_brush( Color(m_strokeTrans * 255, strokeColor.red(), strokeColor.green(), strokeColor.blue()) );
		Pen stroke_pen( &stroke_brush, penWidth );
		REAL *dashes = NULL;
		
		if( m_array.count() > 0 )
		{
			dashes = new REAL[ m_array.count() ];
			for( int i = 0; i < m_array.count();++ i )
			{
				dashes[i] = (REAL) ( m_array[i] / (double) m_lineWidth );
				// The following lines are needed so that gdi+ rendering matches the libart one
				if( m_lineEnd == Qt::RoundCap || m_lineEnd == Qt::SquareCap )
				{
					if( (i % 2) == 0 ) 
						dashes[i] = dashes[i] + 1.0;
					else
						dashes[i] = dashes[i] - 1.0;
				}
			}
			// The following two lines are needed so that gdi+ rendering matches the libart one
			if( m_lineEnd == Qt::SquareCap && m_array.count() >= 2)
				stroke_pen.SetDashOffset( dashes[1]/2 );
			stroke_pen.SetDashPattern( dashes, m_array.count() );
		}
		if( m_lineEnd == Qt::RoundCap )
			stroke_pen.SetLineCap( LineCapRound, LineCapRound, DashCapRound );
		else if( m_lineEnd == Qt::SquareCap )
			stroke_pen.SetLineCap( LineCapSquare, LineCapSquare, DashCapFlat );
		else if( m_lineEnd == Qt::FlatCap && m_array.count() == 0 && m_drawingClosedPath )
			stroke_pen.SetLineCap( LineCapSquare, LineCapSquare, DashCapFlat );
		else if( m_lineEnd == Qt::FlatCap )
			stroke_pen.SetLineCap( LineCapFlat, LineCapFlat, DashCapFlat );
		if( m_lineJoin == Qt::RoundJoin )
			stroke_pen.SetLineJoin( LineJoinRound );
		else if( m_lineJoin == Qt::BevelJoin )
			stroke_pen.SetLineJoin( LineJoinBevel );
		else if( m_lineJoin == Qt::MiterJoin )
			stroke_pen.SetLineJoin( LineJoinMiter );

		m_graphics->DrawPath( &stroke_pen, m_graphicsPath );
		if( dashes )
			delete [] dashes;
	}
	restore();
}

void ScPainterEx_GDIPlus::setClipPath()
{
	m_graphics->SetClip( m_graphicsPath, CombineModeIntersect );
}

void ScPainterEx_GDIPlus::drawImage( ScImage *image, ScPainterExBase::ImageMode mode )
{
	FPoint ulp( 0, 0 );
	FPoint urp( image->width(), 0 );
	FPoint blp( 0, image->height() );
	transformPoint( ulp, ulp );
	transformPoint( blp, blp );
	transformPoint( urp, urp );
	Point destinationPoint[] = { Point(ulp.x(), ulp.y()), Point(urp.x(), urp.y()), Point(blp.x(), blp.y()) };
	ColorMatrix colorMatrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, m_fillTrans, 0.0f,
                               0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	if ( m_convertToGray )
	{
		colorMatrix.m[0][0] = colorMatrix.m[0][1] = colorMatrix.m[0][2] = (REAL) 0.3;
		colorMatrix.m[1][0] = colorMatrix.m[1][1] = colorMatrix.m[1][2] = (REAL) 0.59;
		colorMatrix.m[2][0] = colorMatrix.m[2][1] = colorMatrix.m[2][2] = (REAL) 0.11;
	}
	ImageAttributes imageAtt;
	imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	Bitmap bitmap( image->width(), image->height(), 4 * image->width(), PixelFormat32bppARGB, (uchar*) image->qImage().bits() );
	m_graphics->DrawImage( &bitmap, destinationPoint, 3, 0, 0, image->width(), image->height(), UnitPixel, &imageAtt );
}

void ScPainterEx_GDIPlus::setupPolygon(FPointArray *points, bool closed)
{
	bool nPath = true, first = true;
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
				if (!first && closed)
					m_graphicsPath->CloseFigure();
				m_graphicsPath->StartFigure();
				moveTo( np.x(), np.y() );
				nPath = first = false;
			}
			np = points->point(poi);
			np1 = points->point(poi+1);
			np2 = points->point(poi+3);
			np3 = points->point(poi+2);
			if ((np == np1) && (np2 == np3))
				lineTo( np3.x(), np3.y() );
			else
				curveTo(np1, np2, np3);
		}
		if (closed)
		{
			m_graphicsPath->CloseFigure();
			m_pathIsClosed = true;
			m_drawingClosedPath = true;
		}
	}
}

void ScPainterEx_GDIPlus::drawPolygon()
{
	fillPath();
}

void ScPainterEx_GDIPlus::drawPolyLine()
{
	strokePath();
}

void ScPainterEx_GDIPlus::drawLine(FPoint start, FPoint end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainterEx_GDIPlus::drawRect(double x, double y, double w, double h)
{
	newPath();
	moveTo( x, y );
	lineTo( x+w, y );
	lineTo( x+w, y+h );
	lineTo( x, y+h );
	lineTo( x, y );
	m_pathIsClosed = true;
	m_drawingClosedPath = true;
	fillPath();
	strokePath();
}

void ScPainterEx_GDIPlus::drawGradient( VGradientEx& gradient )
{
	QRect clipPathRect;
	save();
	setClipPath();
	getClipPathDimensions( clipPathRect );
	if ( gradient.type() == VGradientEx::linear )
		drawLinearGradient( gradient, clipPathRect );
	else if ( gradient.type() == VGradientEx::radial )
		drawCircularGradient( gradient, clipPathRect );
	restore();
}

void ScPainterEx_GDIPlus::drawLinearGradient( VGradientEx& gradient, const QRect& rect )
{
	Matrix matrix;
	int index = 0;
	int numElements;
	int r, g, b, a;
	double cosa, sina;
	double length, scale;
	double x1, y1, x2, y2, dx, dy;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx stop( *colorStops[0] );
	FPoint p1, p1a, p2, p2a;
	QColor color;

	if ( gradient.Stops() < 2 )
		return;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt( (double) (clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight) ) * 2;
	if ( maxDim <= 0 ) 
		return;

	x1 = m_matrix.dx() + gradient.origin().x() * m_matrix.m11();
	y1 = m_matrix.dy() + gradient.origin().y() * m_matrix.m22();
	x2 = m_matrix.dx() + gradient.vector().x() * m_matrix.m11();
	y2 = m_matrix.dy() + gradient.vector().y() * m_matrix.m22();
	p1.setXY( x1, y1 );
	p2.setXY( x2, y2 );

	dx = ( p2.x() - p1.x() );
	dy = ( p2.y() - p1.y() );
	length = sqrt(( dx * dx + dy * dy ));
	if ( length == 0.0 )
		scale = 1.0;
	else
		scale = 1.0 / length;
	cosa = dx * scale;
	sina = dy * scale;

	p1a.setXY( p1.x() - cosa * maxDim, p1.y() - sina * maxDim );
	p2a.setXY( p2.x() + cosa * maxDim, p2.y() + sina * maxDim );
	
	numElements = gradient.Stops() + 2;
	Color *colors = new Color[numElements];
	REAL  *positions = new REAL[numElements];
	
	stop = *colorStops[0];
	color = transformColor( ScColorShade(stop.color, stop.shade), 1.0 );
	colors[0] = Color(stop.opacity * 255, color.red(), color.green(), color.blue());
	positions[0] = 0.0;

	stop = *colorStops[gradient.Stops() - 1];
	color = transformColor( ScColorShade(stop.color, stop.shade), 1.0 );
	colors[numElements - 1] = Color(stop.opacity * 255, color.red(), color.green(), color.blue());
	positions[numElements - 1] = 1.0;

	for( uint index = 0 ; index < gradient.Stops(); index++)
	{
		stop = *colorStops[index];
		color = transformColor( ScColorShade(stop.color, stop.shade), 1.0 );
		r = color.red();
		g = color.green();
		b = color.blue();
		a = stop.opacity * 255;
		colors[index + 1] = Color(a, r, g, b);
		positions[index + 1] = ( stop.rampPoint * length + maxDim ) / ( length + 2 * maxDim );
	}
	LinearGradientBrush gradientBrush( PointF( p1a.x(), p1a.y() ) ,
									   PointF( p2a.x(), p2a.y() ),
									   colors[0], colors[numElements - 1] );
	gradientBrush.SetInterpolationColors(colors, positions, numElements);
	m_graphics->FillPath( &gradientBrush, m_graphicsPath );

	delete[] colors;
	delete[] positions;
}

void ScPainterEx_GDIPlus::drawCircularGradient( VGradientEx& gradient, const QRect& rect )
{
	int offset = 0;
	int numElements;
	int r, g, b, a;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx stop( *colorStops[0] );
	QColor color;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt( (double) (clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight) ) * 2;
	if ( maxDim <= 0 ) return;

	FPoint pc( gradient.origin().x(), gradient.origin().y() );
	FPoint pcz( gradient.origin().x(), gradient.origin().y() );
	FPoint pf( gradient.focalPoint().x(), gradient.focalPoint().y() );
	FPoint pfz( gradient.focalPoint().x(), gradient.focalPoint().y() );
	FPoint pv( gradient.vector().x(), gradient.vector().y() );
	transformPoint( pc, pc );
	transformPoint( pf, pf );
	transformPoint( pv, pv );
	transformPoint( pcz, pcz );
	transformPoint( pfz, pfz );
	double cx = pcz.x();
	double cy = pcz.y();
	double fx = pfz.x();
	double fy = pfz.y();
	double rad = sqrt( pow(pv.x() - pc.x(), 2) + pow(pv.y() - pc.y(), 2) );

	numElements = gradient.Stops() + 2;
	Color *colors = new Color[numElements];
	REAL  *positions = new REAL[numElements];

	stop = *colorStops[ 0 ];
	color = transformColor( ScColorShade(stop.color, stop.shade), stop.opacity);
	colors[numElements - 1] = Color(stop.opacity * 255, color.red(), color.green(), color.blue());
	positions[numElements - 1] = 1.0;		
	
	stop = *colorStops[ gradient.Stops() - 1 ];
	color = transformColor( ScColorShade(stop.color, stop.shade), stop.opacity);
	colors[0] = Color(stop.opacity * 255, color.red(), color.green(), color.blue());
	positions[0] = 0.0;

	for( uint index = 0 ; index < gradient.Stops() ; index++)
	{
		stop = *colorStops[index];
		color = transformColor( ScColorShade(stop.color, stop.shade), 1.0 );
		r = color.red();
		g = color.green();
		b = color.blue();
		a = stop.opacity * 255;
		colors[gradient.Stops() - index] = Color(a, r, g, b);
		positions[gradient.Stops() - index] = 1 - 2 * (stop.rampPoint * rad) / (double) maxDim;
	}
	
	GraphicsPath gradientPath;
	gradientPath.AddEllipse( cx - maxDim/2, cy - maxDim/2, maxDim, maxDim );
	PathGradientBrush gradientBrush( &gradientPath );
	gradientBrush.SetCenterPoint( PointF(cx, cy) );
	gradientBrush.SetCenterColor( colors[numElements - 1] );
	gradientBrush.SetInterpolationColors( colors, positions, numElements );
	m_graphics->FillPath( &gradientBrush, m_graphicsPath );

	delete[] colors;
	delete[] positions;
}

void ScPainterEx_GDIPlus::getClipPathDimensions( QRect& r )
{
	Rect rect(0, 0, 0, 0);
	Status result = m_graphics->GetClipBounds( &rect );
	if( result == Ok )
	{
		r.setLeft( rect.GetLeft() );
		r.setRight( rect.GetRight() );
		r.setBottom( rect.GetBottom() );
		r.setTop( rect.GetTop() );
	}
}

