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

#include "scribusdoc.h"
#include "scpainterex_gdi.h"
#include "sccolorengine.h"
#include "util.h"

#include <QImage>
#include <QList>
#include <QPaintDevice>
#include <QPixmap>

#include <iostream>
using namespace std;
 
#include <math.h>

typedef struct { 
    BITMAPINFOHEADER bmiHeader; 
    RGBQUAD bmiColors[256]; 
} BITMAPINFO256;

ScPainterEx_GDI::ScPainterEx_GDI( HDC hDC, QRect& rect,  ScribusDoc* doc, bool gray ) : ScPainterExBase()
{
	m_doc = doc;
	m_width = rect.width();
	m_height= rect.height();
	m_strokeColor = ScColorShade( QColor(0,0,0), 100 );
	m_fillColor = ScColorShade( QColor(0,0,0), 100 );
	m_fillTrans = 1.0;
	m_strokeTrans = 1.0;
	m_fillRule = true;
	m_fillMode = 1;
	m_lineWidth = 1.0;
	m_offset = 0;
	m_array.clear();
	m_lineEnd = Qt::SquareCap;
	m_lineJoin = Qt::RoundJoin;
	m_fillGradient = VGradientEx(VGradientEx::linear);
	m_strokeGradient = VGradientEx(VGradientEx::linear);
	m_matrix = QMatrix();
	// Grayscale conversion parameter
	m_convertToGray = gray;
	// Initialization of Windows GDI data
	m_dc = hDC;
	m_hBmp = NULL;
	m_hMsImg32 = NULL;
	m_gradientFill = NULL;
	m_pathIsClosed = true;
	m_drawingClosedPath = false;
	m_deviceDimX = GetDeviceCaps(m_dc, PHYSICALWIDTH);
	m_deviceDimY = GetDeviceCaps(m_dc, PHYSICALHEIGHT);
	m_deviceResX = GetDeviceCaps(m_dc, LOGPIXELSX);
	m_deviceResY = GetDeviceCaps(m_dc, LOGPIXELSY);
	SetGraphicsMode( m_dc, GM_ADVANCED );
	SetPolyFillMode( m_dc, ALTERNATE);
	loadMsImg32();
	clear();
}

ScPainterEx_GDI::~ScPainterEx_GDI()
{
	if ( m_hMsImg32 )
		FreeLibrary( m_hMsImg32 );
}

void ScPainterEx_GDI::loadMsImg32( void )
{
	m_hMsImg32 = LoadLibraryW( L"msimg32.dll" );
	if( m_hMsImg32 )
		m_gradientFill = (gradientFillFunc) GetProcAddress( m_hMsImg32, "GradientFill" );
}

QColor ScPainterEx_GDI::transformColor( ScColorShade& colorShade, double trans )
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

void ScPainterEx_GDI::transformImage( QImage* image, uchar* data, int scan)
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

void ScPainterEx_GDI::begin()
{
}

void ScPainterEx_GDI::end()
{
	ModifyWorldTransform(m_dc, NULL, MWT_IDENTITY);
}

void ScPainterEx_GDI::clear()
{
	RECT rect;
	HBRUSH brush = CreateSolidBrush( RGB(255, 255, 255) );
	rect.bottom = 0;
	rect.left = 0;
	rect.right = m_width;
	rect.top = m_height;
	FillRect(m_dc, &rect, brush);
	DeleteObject( brush );
}

void ScPainterEx_GDI::clear( ScColorShade &c )
{
	QColor color = transformColor( c, 1.0 );
	RECT rect;
	HBRUSH brush = CreateSolidBrush( RGB( qRed(color.rgb()), qGreen(color.rgb()), qBlue(color.rgb()) ) );
	rect.bottom = 0;
	rect.left = 0;
	rect.right = m_width;
	rect.top = m_height;
	FillRect(m_dc, &rect, brush);
	DeleteObject( brush );
}

const QMatrix ScPainterEx_GDI::worldMatrix()
{
	return m_matrix;
}

void ScPainterEx_GDI::setWorldMatrix( const QMatrix &mat )
{
	m_matrix = mat;
}

void ScPainterEx_GDI::transformPoint( const FPoint& in, FPoint& out )
{
	double x, y;
	x = in.x() * m_matrix.m11() + in.y() * m_matrix.m21() + m_matrix.dx();
	y = in.x() * m_matrix.m12() + in.y() * m_matrix.m22() + m_matrix.dy();
	out.setX( x );
	out.setY( y );
}

void ScPainterEx_GDI::transformPoints( const FPoint* ArrayIn, FPoint* ArrayOut, uint length )
{
	for( uint i = 0; i < length; i++ )
	{
		transformPoint( ArrayIn[i], ArrayOut[i] );
	}
}

void ScPainterEx_GDI::translate( double x, double y )
{
	m_matrix.translate(x, y);
}

void ScPainterEx_GDI::rotate( double r )
{
	m_matrix.rotate(r);
}

void ScPainterEx_GDI::scale( double x, double y )
{
	m_matrix.scale(x, y);
}

void ScPainterEx_GDI::moveTo( const double &x, const double &y )
{
	FPoint pp( x, y );
	transformPoint( pp, pp );
	MoveToEx(m_dc, (int) pp.x(), (int) pp.y(), NULL);
}

void ScPainterEx_GDI::lineTo( const double &x, const double &y )
{
	FPoint pp( x, y );
	transformPoint( pp, pp );
	LineTo(m_dc, (int) pp.x(), (int) pp.y());
}

void ScPainterEx_GDI::curveTo( FPoint p1, FPoint p2, FPoint p3 )
{
	FPoint fpoints[3];
	fpoints[0].setXY( p1.x(), p1.y() );
	fpoints[1].setXY( p2.x(), p2.y() );
	fpoints[2].setXY( p3.x(), p3.y() );
	transformPoints( fpoints, fpoints, 3 );
	POINT points[3];
	points[0].x = fpoints[0].x();
	points[0].y = fpoints[0].y();
	points[1].x = fpoints[1].x();
	points[1].y = fpoints[1].y();
	points[2].x = fpoints[2].x();
	points[2].y = fpoints[2].y();
	PolyBezierTo(m_dc, points, 3);
}

void ScPainterEx_GDI::newPath()
{
	bool done = BeginPath( m_dc );
	if(!done)
		cout << "BeginPath fuction has failed." << endl;
	m_pathIsClosed = false;
	m_drawingClosedPath = false;
}

void ScPainterEx_GDI::setFillRule( bool fillRule )
{
	m_fillRule = fillRule;
}

void ScPainterEx_GDI::setFillMode( int fill )
{
	m_fillMode = fill;
}

void ScPainterEx_GDI::setGradient(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc)
{
	m_fillGradient.setType(mode);
	m_fillGradient.setOrigin(orig);
	m_fillGradient.setVector(vec);
	m_fillGradient.setFocalPoint(foc);
}

void ScPainterEx_GDI::setPattern( ScPattern* pattern, QMatrix& patternTransform )
{
	m_pattern = pattern;
	m_patternTransform = patternTransform;
}

void ScPainterEx_GDI::fillTextPath()
{
	drawVPath( 0 );
}

void ScPainterEx_GDI::strokeTextPath()
{
	if( m_lineWidth == 0 )
		return;
	drawVPath( 1 );
}

void ScPainterEx_GDI::fillPath()
{
	if( !m_pathIsClosed )
	{
		EndPath( m_dc );
		m_pathIsClosed = true;
	}
	if( m_fillMode != 0)
		drawVPath( 0 );
}

void ScPainterEx_GDI::strokePath()
{
	if( m_lineWidth == 0 )
		return;
	if( !m_pathIsClosed )
		EndPath( m_dc );
	m_pathIsClosed = true;
	drawVPath( 1 );
}

ScColorShade ScPainterEx_GDI::pen()
{
	return m_strokeColor;
}

ScColorShade ScPainterEx_GDI::brush()
{
	return m_fillColor;
}

void ScPainterEx_GDI::setPen( const ScColorShade &c )
{
	m_strokeColor = c;
}

void ScPainterEx_GDI::setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo )
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

void ScPainterEx_GDI::setLineWidth( double w )
{
	m_lineWidth = w;
}

void ScPainterEx_GDI::setPenOpacity( double op )
{
	m_strokeTrans = op;
}


void ScPainterEx_GDI::setDash(const QList<double>& array, double ofs)
{
	m_array = array;
	m_offset = ofs;
}

void ScPainterEx_GDI::setBrush( const ScColorShade &c )
{
	m_fillColor = c;
}

void ScPainterEx_GDI::setBrushOpacity( double op )
{
	m_fillTrans = op;
}

void ScPainterEx_GDI::setOpacity( double op )
{
	m_fillTrans = op;
	m_strokeTrans = op;
}

void ScPainterEx_GDI::setFont( const QFont &f)
{
	m_font = f;
}

QFont ScPainterEx_GDI::font()
{
	return m_font;
}

void ScPainterEx_GDI::save()
{
	m_stack.push( m_matrix );
	SaveDC( m_dc );
}

void ScPainterEx_GDI::restore()
{
	m_matrix = m_stack.pop();
	RestoreDC( m_dc, -1 );
}

void ScPainterEx_GDI::setRasterOp( int   )
{
}

void ScPainterEx_GDI::drawVPath( int mode )
{
	int dcState;
	dcState = SaveDC( m_dc );
	if (mode == 0)
	{
		if( m_fillRule )
			SetPolyFillMode(m_dc, ALTERNATE);
		else
			SetPolyFillMode(m_dc, WINDING);
		if (m_fillMode == ScPainterExBase::Gradient)
		{
			drawGradient( m_fillGradient );
		}
		else
		{
			QColor fillColor = transformColor( m_fillColor, 1.0 );
			HBRUSH brush = CreateSolidBrush( RGB( qRed(fillColor.rgb()), qGreen(fillColor.rgb()), qBlue(fillColor.rgb()) ) );
			HGDIOBJ obj = SelectObject(m_dc, brush);
			FillPath(m_dc);
			SelectObject(m_dc, obj);
			DeleteObject(brush);
		}
	}
	else
	{
		DWORD penStyle = PS_GEOMETRIC;
		DWORD penWidth = 0;
		LOGBRUSH logBrush;
		DWORD *dashes = NULL;

		QColor strokeColor = transformColor( m_strokeColor, 1.0 );
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor = RGB( qRed(strokeColor.rgb()), qGreen(strokeColor.rgb()), qBlue(strokeColor.rgb()) );
		logBrush.lbHatch = 0;

		double m11 = m_matrix.m11();
		double m12 = m_matrix.m12();
		double m21 = m_matrix.m21();
		double m22 = m_matrix.m22();
		double norm2 = m11 * m11 + m12 * m12 + m21 * m21 + m22 * m22;
		double penScale = sqrt(norm2 /2.0);
		penWidth = (int) (m_lineWidth * penScale);
		
		if( m_array.count() > 0 )
		{
			dashes = new DWORD[ m_array.count() ];
			for(int i = 0; i < m_array.count();++ i )
			{
				dashes[i] = (DWORD) (m_array[i] * penScale);
			}
			penStyle = penStyle | PS_USERSTYLE; 
		}
		if( m_lineEnd == Qt::RoundCap )
			penStyle = penStyle | PS_ENDCAP_ROUND;
		else if( m_lineEnd == Qt::SquareCap )
			penStyle = penStyle | PS_ENDCAP_SQUARE;
		else if( m_lineEnd == Qt::FlatCap && m_array.count() == 0 && m_drawingClosedPath )
			penStyle = penStyle | PS_ENDCAP_SQUARE;
		else if( m_lineEnd == Qt::FlatCap )
			penStyle = penStyle | PS_ENDCAP_FLAT;
		if( m_lineJoin == Qt::RoundJoin )
			penStyle = penStyle | PS_JOIN_ROUND;
		else if( m_lineJoin == Qt::BevelJoin )
			penStyle = penStyle | PS_JOIN_BEVEL;
		else if( m_lineJoin == Qt::MiterJoin )
			penStyle = penStyle | PS_JOIN_MITER;

		HPEN pen = ExtCreatePen(penStyle, penWidth, &logBrush, m_array.count(), dashes);
		HGDIOBJ obj = SelectObject( m_dc, pen);
		StrokePath( m_dc );
		SelectObject( m_dc, obj);
		DeleteObject( pen);
		if( dashes )
			delete [] dashes;
	}
	RestoreDC( m_dc, dcState );
}

void ScPainterEx_GDI::setClipPath()
{
	SelectClipPath( m_dc, RGN_AND );
}

void ScPainterEx_GDI::drawImage( ScImage *image, ScPainterExBase::ImageMode mode )
{
	save();
	int usage = 0;
	unsigned char* data = NULL;
	unsigned char* imageData = (unsigned char*) image->qImage().bits();
	XFORM xform = { m_matrix.m11(), m_matrix.m12(), m_matrix.m21(), m_matrix.m22(), m_matrix.dx(), m_matrix.dy() } ;
	ModifyWorldTransform(m_dc, &xform, MWT_LEFTMULTIPLY);
	BITMAPINFO256 bmpInfo;
	ZeroMemory( &bmpInfo, sizeof(bmpInfo));
	bmpInfo.bmiHeader.biSize = sizeof( bmpInfo.bmiHeader );
	bmpInfo.bmiHeader.biWidth = image->width();
	bmpInfo.bmiHeader.biHeight = -image->height();
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	if ( m_convertToGray )
	{
		int scanline = (image->width() + image->width() % 4);
		bmpInfo.bmiHeader.biBitCount = 8;
		bmpInfo.bmiHeader.biSizeImage = scanline * image->height();
		for( int i = 0; i < 256; i++ )
		{
			bmpInfo.bmiColors[i].rgbRed = i;
			bmpInfo.bmiColors[i].rgbGreen = i;
			bmpInfo.bmiColors[i].rgbBlue = i;
		}
		QImage imTemp = image->qImage();
		data = new uchar[bmpInfo.bmiHeader.biSizeImage];
		transformImage( &imTemp, data, scanline );
		imageData = data;
		usage = DIB_RGB_COLORS;
	}
	StretchDIBits( m_dc, 0, 0 , image->width(), image->height(), 0, 0, image->width(), image->height(), 
		imageData, (BITMAPINFO*) &bmpInfo, usage, SRCCOPY );
	if( data )
		delete[] data;
	restore();
}

void ScPainterEx_GDI::setupPolygon(FPointArray *points, bool closed)
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
				moveTo( np.x(), np.y() );
				nPath = false;
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
			EndPath( m_dc );
			m_pathIsClosed = true;
			m_drawingClosedPath = true;
		}
	}
}

void ScPainterEx_GDI::drawPolygon()
{
	fillPath();
}

void ScPainterEx_GDI::drawPolyLine()
{
	strokePath();
}

void ScPainterEx_GDI::drawLine(FPoint start, FPoint end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainterEx_GDI::drawRect(double x, double y, double w, double h)
{
	newPath();
	moveTo( x, y );
	lineTo( x+w, y );
	lineTo( x+w, y+h );
	lineTo( x, y+h );
	lineTo( x, y );
	EndPath(m_dc);
	m_pathIsClosed = true;
	m_drawingClosedPath = true;
	fillPath();
	strokePath();
}

void ScPainterEx_GDI::drawGradient( VGradientEx& gradient )
{
	QRect clipPathRect;
	SaveDC( m_dc );
	SelectClipPath( m_dc, RGN_COPY );
	getClipPathDimensions( clipPathRect );
	bool haveMsImg32 = ( m_hMsImg32 != NULL && m_gradientFill != NULL );
	if ( gradient.type() == VGradientEx::linear && haveMsImg32 )
		drawLinearGradient_GradientFill( gradient, clipPathRect );
	else if ( gradient.type() == VGradientEx::linear )
		drawLinearGradient( gradient, clipPathRect );
	else if (gradient.type() == VGradientEx::radial)
		drawCircularGradient( gradient, clipPathRect );
	RestoreDC( m_dc, -1 );
}

void ScPainterEx_GDI::drawLinearGradient( VGradientEx& gradient, const QRect& rect )
{
	XFORM xform;
	int xCurrent;
	int increment;
	int r1, g1, b1, a1;
	int r2, g2, b2, a2;
	int r3, g3, b3, a3;
	double colorCoeff;
	double cosa, sina;
	double ramp1, ramp2;
	double scale1, scale2;
	double length, scale;
	double x1, y1, x2, y2, dx, dy;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[0] );
	VColorStopEx stop2( *colorStops[0] );
	FPoint p1, p1a, p2, p2a;
	QColor color;

	if ( gradient.Stops() < 2 ) 
		return;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt((double) (clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight)) * 2;
	if ( maxDim <= 0 ) return;
	
	scale1 = sqrt( m_matrix.m11() * m_matrix.m11() + m_matrix.m12() * m_matrix.m12() );
	scale2 = sqrt( m_matrix.m21() * m_matrix.m21() + m_matrix.m22() * m_matrix.m22() );
	increment = (int) max( scale1, scale2 );
	increment = (increment < 2) ? 2 : increment;

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

	ramp1 = stop1.rampPoint;
	color =  transformColor( ScColorShade(stop1.color, stop1.shade), 1.0 );
	r1 = color.red();
	g1 = color.green();
	b1 = color.blue();
	a1 = stop1.opacity * 255;
	drawRectangleStrip( m_matrix.dx(), m_matrix.dy(), maxDim, maxDim, r1, g1, b1, a1);

	xform.eM11 = cosa;
	xform.eM12 = sina;
	xform.eM21 = -sina;
	xform.eM22 = cosa;
	xform.eDx = x1;
	xform.eDy = y1;
	ModifyWorldTransform(m_dc, &xform, MWT_LEFTMULTIPLY);

	for( uint index = 1; index < gradient.Stops(); index++)
	{
		stop2 = *colorStops[index];
		ramp2 = stop2.rampPoint;
		color =  transformColor( ScColorShade(stop2.color, stop2.shade), 1.0 );
		r2 = color.red();
		g2 = color.green();
		b2 = color.blue();
		a2 = stop2.opacity * 255;
		int xMin = length * ramp1;
		int xMax = length * ramp2;
		int r4= -1, b4= -1, g4= -1, a4= -1;
		for( xCurrent = xMin; xCurrent < xMax; xCurrent += increment )
		{
			colorCoeff = ( xCurrent - xMin ) / ( (double) xMax - xMin ) ;
			r3 = ( 1 - colorCoeff ) * r1 + colorCoeff * r2;
			g3 = ( 1 - colorCoeff ) * g1 + colorCoeff * g2;
			b3 = ( 1 - colorCoeff ) * b1 + colorCoeff * b2;
			a3 = ( 1 - colorCoeff ) * a1 + colorCoeff * a2;
			if( (r3 != r4) || (g3 != g4) || (b3 != b4) )
			{
				drawRectangleStrip( xCurrent, -maxDim/2, maxDim, maxDim, r3, g3, b3, a3 );
				r4 = r3;
				g4 = g3;
				b4 = b3;
			}
		}
		stop1 = stop2;
		ramp1 = ramp2;
		r1 = r2;
		g1 = g2;
		b1 = b2;
		a1 = a2;
	}
}

void ScPainterEx_GDI::drawLinearGradient_GradientFill( VGradientEx& gradient, const QRect& rect )
{
	XFORM xform;
	int r1, g1, b1;
	int r2, g2, b2;
	double cosa, sina;
	double ramp1, ramp2;
	double length, scale;
	double x1, y1, x2, y2, dx, dy;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[0] );
	VColorStopEx stop2( *colorStops[0] );
	FPoint p1, p2;
	QColor color;

	if ( gradient.Stops() < 2 ) 
		return;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt( (double) (clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight) ) * 2;
	if ( maxDim <= 0 ) return;

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

	xform.eM11 = cosa;
	xform.eM12 = sina;
	xform.eM21 = -sina;
	xform.eM22 = cosa;
	xform.eDx = x1;
	xform.eDy = y1;
	ModifyWorldTransform(m_dc, &xform, MWT_LEFTMULTIPLY);

	x1 = -maxDim;
	stop1 = *colorStops[0];
	ramp1 = stop1.rampPoint;
	color =  transformColor( ScColorShade(stop1.color, stop1.shade), 1.0 );
	r1 = color.red();
	g1 = color.green();
	b1 = color.blue();

	int numMeshes = colorStops.size() + 1;
	int numVertices = numMeshes * 2;
	TRIVERTEX* vertices = new TRIVERTEX[numVertices];
	GRADIENT_RECT* meshes = new GRADIENT_RECT[numMeshes];

	vertices[0].x = x1;
	vertices[0].y = -maxDim / 2;
	vertices[0].Red = r1 << 8;
	vertices[0].Green = g1 << 8;
	vertices[0].Blue = b1 << 8;
	vertices[0].Alpha = 0;
	meshes[0].UpperLeft = 0;
	
	uint vIndex = 1;
	for( uint index = 0; index < gradient.Stops(); index++, vIndex += 2)
	{
		stop2 = *colorStops[index];
		ramp2 =  stop2.rampPoint;
		color =  transformColor( ScColorShade(stop2.color, stop2.shade), 1.0 );
		r2 = color.red() << 8;
		g2 = color.green() << 8;
		b2 = color.blue() << 8;
		x2 = /*maxDim +*/ length * ramp2;
		vertices[vIndex].x = x2;
		vertices[vIndex].y = maxDim / 2;
		vertices[vIndex].Red = r2;
		vertices[vIndex].Green = g2;
		vertices[vIndex].Blue = b2;
		vertices[vIndex].Alpha = 0;
		vertices[vIndex + 1].x = x2;
		vertices[vIndex + 1].y = -maxDim / 2;
		vertices[vIndex + 1].Red = r2;
		vertices[vIndex + 1].Green = g2;
		vertices[vIndex + 1].Blue = b2;
		vertices[vIndex + 1].Alpha = 0;
		meshes[index].LowerRight = vIndex;
		meshes[index + 1].UpperLeft = vIndex + 1;
	}

	vertices[numVertices - 1].x = length + maxDim;
	vertices[numVertices - 1].y = maxDim / 2;
	vertices[numVertices - 1].Red = r2 << 8;
	vertices[numVertices - 1].Green = g2 << 8;
	vertices[numVertices - 1].Blue = b2 << 8;
	vertices[numVertices - 1].Alpha = 0;
	meshes[numMeshes - 1].LowerRight = numVertices - 1;

	// Finally call the gradient fill function
	(*m_gradientFill)( m_dc, vertices, numVertices, meshes, numMeshes, GRADIENT_FILL_RECT_H );

	// Delete used arrays
	delete[] vertices;
	delete[] meshes;
}

void ScPainterEx_GDI::drawRectangleStrip( int x, int y, int w, int h, int r, int g, int b, int a )
{
	SaveDC( m_dc );
	HPEN hPen = CreatePen( PS_NULL, 0, RGB(255,255,255));
	HBRUSH hBrush = CreateSolidBrush( RGB(r, g, b));
	HGDIOBJ obj1 = SelectObject( m_dc, hPen );
	HGDIOBJ obj2 = SelectObject( m_dc, hBrush );
	Rectangle( m_dc, x, y, x + w, y + h );
	SelectObject( m_dc, obj2 );
	SelectObject( m_dc, obj1 );
	DeleteObject( hBrush );
	DeleteObject( hPen );
	RestoreDC( m_dc, -1 );
}

void ScPainterEx_GDI::drawCircularGradient( VGradientEx& gradient, const QRect& rect )
{
	int radius;
	int increment;
	int r1, g1, b1, a1;
	int r2, g2, b2, a2;
	int r3, g3, b3, a3;
	double colorCoeff;
	double ramp1, ramp2;
	double scale1, scale2;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[gradient.Stops() - 1] );
	VColorStopEx stop2( *colorStops[gradient.Stops() - 1] );
	QColor color;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt((double) (clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight)) * 2;
	if ( maxDim <= 0 ) return;

	scale1 = sqrt( m_matrix.m11() * m_matrix.m11() + m_matrix.m21() * m_matrix.m21() );
	scale2 = sqrt( m_matrix.m12() * m_matrix.m12() + m_matrix.m22() * m_matrix.m22() );
	increment = (int) max( scale1, scale2 );
	increment = (increment < 2) ? 2 : increment;

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
	double rad = sqrt( pow(pv.x() -pc.x(), 2) + pow(pv.y() - pc.y(), 2) );

	ramp2 = stop2.rampPoint;
	color =  transformColor( ScColorShade(stop2.color, stop2.shade), 1.0 );
	r2 = color.red();
	g2 = color.green();
	b2 = color.blue();
	a2 = stop2.opacity * 255;
	drawCircleStrip( cx - maxDim / 2, cy - maxDim / 2, maxDim, maxDim, color.red(), color.green(), color.blue(), a2);
	if ( gradient.Stops() < 2 ) return;

	for( int index = gradient.Stops() - 2; index >= 0; index--)
	{
		stop1 = *colorStops[index];
		ramp1 = stop1.rampPoint;
		color =  transformColor( ScColorShade(stop1.color, stop1.shade), 1.0 );
		r1 = color.red();
		g1 = color.green();
		b1 = color.blue();
		a1 = stop1.opacity * 255;
		int radMax = rad * ramp2;
		int radMin = rad * ramp1;
		int r4= -1, b4= -1, g4= -1, a4= -1;
		for( radius = radMax; radius > radMin; radius -= increment )
		{
			colorCoeff = ( radius - radMin ) / ( (double) radMax - radMin );
			r3 = (1 - colorCoeff) * r1 + colorCoeff * r2;
			g3 = (1 - colorCoeff) * g1 + colorCoeff * g2;
			b3 = (1 - colorCoeff) * b1 + colorCoeff * b2;
			a3 = (1 - colorCoeff) * a1 + colorCoeff * a2;
			if( (r3 != r4) || (g3 != g4) || (b3 != b4) )
			{
				drawCircleStrip( cx - radius, cy - radius, radius * 2, radius * 2, r3, g3, b3, a3 );
				r4 = r3;
				g4 = g3;
				b4 = b3;
			}
		}
		stop2 = stop1;
		ramp2 = ramp1;
		r2 = r1;
		g2 = g1;
		b2 = b1;
		a2 = a1;
	}
}

void ScPainterEx_GDI::drawCircleStrip( int x, int y, int w, int h, int r, int g, int b, int a )
{
	SaveDC( m_dc );
	HPEN hPen = CreatePen( PS_NULL, 0, RGB(255,255,255));
	HBRUSH hBrush = CreateSolidBrush( RGB(r, g, b));
	HGDIOBJ obj1 = SelectObject( m_dc, hPen );
	HGDIOBJ obj2 = SelectObject( m_dc, hBrush );
	Ellipse( m_dc, x, y, x + w, y + h );
	SelectObject( m_dc, obj2 );
	SelectObject( m_dc, obj1 );
	DeleteObject( hBrush );
	DeleteObject( hPen );
	RestoreDC( m_dc, -1 );
}

void ScPainterEx_GDI::getClipPathDimensions( QRect& r )
{
	RECT rect = { 0, 0, 0, 0 };
	int result = GetClipBox( m_dc, &rect );
	if( result != NULLREGION && result != ERROR )
	{
		r.setLeft( rect.left );
		r.setRight( rect.right );
		r.setBottom( rect.bottom );
		r.setTop( rect.top );	
	}
}

