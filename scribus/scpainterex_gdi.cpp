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

#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

#include <valarray>
#include <windows.h>

#include <gdiplus.h>
using namespace Gdiplus;

#include "scpainterex_gdi.h"

#include <qpaintdevice.h>
#include <qpixmap.h>
#include <qpointarray.h>
#include <qimage.h>

#include <iostream>
using namespace std;
 
#include <math.h>

typedef struct { 
    BITMAPINFOHEADER bmiHeader; 
    RGBQUAD bmiColors[256]; 
} BITMAPINFO256;

ScPainterEx_GDI::ScPainterEx_GDI( HDC hDC, QRect& rect, bool gray )
{
	m_target = NULL;
	m_width = rect.width();
	m_height= rect.height();
	m_x = rect.left();
	m_y = rect.top();
	m_buffer = 0L;
	m_index = 0;
	m_stroke = ScColorShade( QColor(0,0,0), 100 );
	m_fill = ScColorShade( QColor(0,0,0), 100 );
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
	fill_gradient = VGradientEx(VGradientEx::linear);
	stroke_gradient = VGradientEx(VGradientEx::linear);
	m_zoomFactor = 1;
	m_matrix = QWMatrix();
	m_pageTrans = QWMatrix();
	// Grayscale conversion
	toGray = gray;
	// Initialization of Windows GDI data
	dc = hDC;
	hBmp = NULL;
	hMsImg32 = NULL;
	gradientFill = NULL;
	pathClosed = true;
	drawingClosedCurve = false;
	deviceDimX = GetDeviceCaps(dc, PHYSICALWIDTH);
	deviceDimY = GetDeviceCaps(dc, PHYSICALHEIGHT);
	deviceResX = GetDeviceCaps(dc, LOGPIXELSX);
	deviceResY = GetDeviceCaps(dc, LOGPIXELSY);
#ifdef SC_USE_GDIPLUS
	positionX = positionY = 0;
	graphics = new Gdiplus::Graphics( dc );
	graphics->SetCompositingMode( CompositingModeSourceOver );
	graphics->SetCompositingQuality( CompositingQualityHighQuality );
	graphics->SetSmoothingMode( SmoothingModeHighQuality );
	graphics->SetPageUnit( UnitPoint );
	graphicsPath = NULL;
#else
	SetGraphicsMode( dc, GM_ADVANCED );
	SetPolyFillMode(dc, ALTERNATE);
	loadMsImg32();
#endif
	clear();
}

ScPainterEx_GDI::~ScPainterEx_GDI()
{
	if ( hMsImg32 )
		FreeLibrary( hMsImg32 );
#ifdef SC_USE_GDIPLUS
	if ( graphicsPath )
		delete graphicsPath;
	if ( graphics )
		delete graphics;
#endif
}

void ScPainterEx_GDI::loadMsImg32( void )
{
	hMsImg32 = LoadLibrary( "msimg32.dll" );
	if( hMsImg32 )
		gradientFill = (gradientFillFunc) GetProcAddress( hMsImg32, "GradientFill" );
}

QColor ScPainterEx_GDI::transformColor( ScColorShade& colorShade, double trans )
{
	QColor c, color;
	c = colorShade.color.getShadeColorProof( colorShade.shade );
	color = qRgba( qRed(c.rgb()), qGreen(c.rgb()), qBlue(c.rgb()), qAlpha(trans * 255));
	if ( toGray )
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
#ifdef SC_USE_GDIPLUS
	graphics->ResetTransform();
#else
	ModifyWorldTransform(dc, NULL, MWT_IDENTITY);
#endif
}

void ScPainterEx_GDI::clear()
{
#ifdef SC_USE_GDIPLUS
	SolidBrush brush( Color(255,255,255) );
	graphics->FillRectangle(&brush, 0, 0, m_width, m_height);
#else
	RECT rect;
	HBRUSH brush = CreateSolidBrush( RGB(255, 255, 255) );
	rect.bottom = 0;
	rect.left = 0;
	rect.right = m_width;
	rect.top = m_height;
	FillRect(dc, &rect, brush);
	DeleteObject( brush );
#endif
}

void ScPainterEx_GDI::clear( ScColorShade &c )
{
	QColor color = transformColor( c, 1.0 );
#ifdef SC_USE_GDIPLUS
	QRgb cs = color.rgb();
	SolidBrush brush( Color(qAlpha(cs), qRed(cs), qGreen(cs),qBlue(cs)) );
	graphics->FillRectangle(&brush, 0, 0, m_width, m_height);
#else
	RECT rect;
	HBRUSH brush = CreateSolidBrush( RGB( qRed(color.rgb()), qGreen(color.rgb()), qBlue(color.rgb()) ) );
	rect.bottom = 0;
	rect.left = 0;
	rect.right = m_width;
	rect.top = m_height;
	FillRect(dc, &rect, brush);
	DeleteObject( brush );
#endif
}

const QWMatrix ScPainterEx_GDI::worldMatrix()
{
	return m_matrix;
}

void ScPainterEx_GDI::setWorldMatrix( const QWMatrix &mat )
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

void ScPainterEx_GDI::setZoomFactor( double zoomFactor )
{
	m_zoomFactor = zoomFactor;
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
	FPoint pp( x * m_zoomFactor, y * m_zoomFactor );
	transformPoint( pp, pp );
#ifdef SC_USE_GDIPLUS
	positionX = pp.x();
	positionY = pp.y();
#else
	MoveToEx(dc, (int) pp.x(), (int) pp.y(), NULL);
#endif
}

void ScPainterEx_GDI::lineTo( const double &x, const double &y )
{
	FPoint pp( x * m_zoomFactor, y * m_zoomFactor );
	transformPoint( pp, pp );
#ifdef SC_USE_GDIPLUS
	graphicsPath->AddLine( (REAL) positionX, (REAL) positionY, pp.x(), pp.y() );
	positionX = pp.x();
	positionY = pp.y();
#else
	LineTo(dc, (int) pp.x(), (int) pp.y());
#endif
}

void ScPainterEx_GDI::curveTo( FPoint p1, FPoint p2, FPoint p3 )
{
	FPoint fpoints[3];
	fpoints[0].setXY( p1.x() * m_zoomFactor, p1.y() * m_zoomFactor );
	fpoints[1].setXY( p2.x() * m_zoomFactor, p2.y() * m_zoomFactor );
	fpoints[2].setXY( p3.x() * m_zoomFactor, p3.y() * m_zoomFactor );
	transformPoints( fpoints, fpoints, 3 );
#ifdef SC_USE_GDIPLUS
	double p1x = fpoints[0].x();
	double p1y = fpoints[0].y();
	double p2x = fpoints[1].x();
	double p2y = fpoints[1].y();
	double p3x = fpoints[2].x();
	double p3y = fpoints[2].y();
	graphicsPath->AddBezier( (REAL) positionX, (REAL) positionY, p1x, p1y, p2x, p2y, p3x, p3y );
	positionX = p3x;
	positionY = p3y;
#else
	POINT points[3];
	points[0].x = fpoints[0].x();
	points[0].y = fpoints[0].y();
	points[1].x = fpoints[1].x();
	points[1].y = fpoints[1].y();
	points[2].x = fpoints[2].x();
	points[2].y = fpoints[2].y();
	PolyBezierTo(dc, points, 3);
#endif
}

void ScPainterEx_GDI::newPath()
{
#ifdef SC_USE_GDIPLUS
	if( graphicsPath )
		delete graphicsPath;
	graphicsPath = new Gdiplus::GraphicsPath(); 
	pathClosed = false;
	drawingClosedCurve = false;
#else
	bool done = BeginPath( dc );
	if(!done)
		cout << "BeginPath fuction has failed." << endl;
	pathClosed = false;
	drawingClosedCurve = false;
#endif
}

void ScPainterEx_GDI::setFillRule( bool fillRule )
{
	m_fillRule = fillRule;
}

void ScPainterEx_GDI::setFillMode( int fill )
{
	fillMode = fill;
}

void ScPainterEx_GDI::setGradient(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc)
{
	fill_gradient.setType(mode);
	fill_gradient.setOrigin(orig);
	fill_gradient.setVector(vec);
	fill_gradient.setFocalPoint(foc);
}

void ScPainterEx_GDI::fillTextPath()
{
	drawVPath( 0 );
}

void ScPainterEx_GDI::strokeTextPath()
{
	if( LineWidth == 0 )
		return;
	drawVPath( 1 );
}

void ScPainterEx_GDI::fillPath()
{
#ifndef SC_USE_GDIPLUS
	if( !pathClosed )
		EndPath( dc );
#endif
	pathClosed = true;
	if( fillMode != 0)
		drawVPath( 0 );
}

void ScPainterEx_GDI::strokePath()
{
	if( LineWidth == 0 )
		return;
	save();
// Hackery so that gdi+ rendering matches the libart one :
// if the current clip path is stroked, the pen width won't be entirely visible
// contrary to libart where it will
#ifdef SC_USE_GDIPLUS
	graphics->ResetClip();
#else
	SelectClipRgn( dc, NULL );
	if( !pathClosed )
		EndPath( dc );
#endif
	pathClosed = true;
	drawVPath( 1 );
	restore();
}

ScColorShade ScPainterEx_GDI::pen()
{
	return m_stroke;
}

ScColorShade ScPainterEx_GDI::brush()
{
	return m_fill;
}

void ScPainterEx_GDI::setPen( const ScColorShade &c )
{
	m_stroke = c;
}

void ScPainterEx_GDI::setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo )
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
#ifdef SC_USE_GDIPLUS
			m_array.append(Dt);
#endif
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

void ScPainterEx_GDI::setLineWidth( double w )
{
	LineWidth = w;
}

void ScPainterEx_GDI::setPenOpacity( double op )
{
	stroke_trans = op;
}


void ScPainterEx_GDI::setDash(const QValueList<double>& array, double ofs)
{
	m_array = array;
	m_offset = ofs;
}

void ScPainterEx_GDI::setBrush( const ScColorShade &c )
{
	m_fill = c;
}

void ScPainterEx_GDI::setBrushOpacity( double op )
{
	fill_trans = op;
}

void ScPainterEx_GDI::setOpacity( double op )
{
	fill_trans = op;
	stroke_trans = op;
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
	MStack.push( m_matrix );
#ifdef SC_USE_GDIPLUS
	gStates.push( graphics->Save() );
#else
	SaveDC( dc );
#endif
}

void ScPainterEx_GDI::restore()
{
	m_matrix = MStack.pop();
#ifdef SC_USE_GDIPLUS
	graphics->Restore( gStates.pop() );
#else
	RestoreDC( dc, -1 );
#endif
}

void ScPainterEx_GDI::setRasterOp( int   )
{
}

void ScPainterEx_GDI::drawVPath( int mode )
{
#ifdef SC_USE_GDIPLUS
	save();
	if (mode == 0)
	{
		if( m_fillRule )
			graphicsPath->SetFillMode( FillModeAlternate);
		else
			graphicsPath->SetFillMode( FillModeWinding);
		if (fillMode == 2)
		{
			drawGradient( fill_gradient );
		}
		else
		{
			QColor fillColor = transformColor(m_fill, 1.0);
			SolidBrush fill_brush( Color(fill_trans * 255, fillColor.red(), fillColor.green(), fillColor.blue()) );
			graphics->FillPath( &fill_brush, graphicsPath );
		}
	}
	else
	{
		double m11 = m_matrix.m11();
		double m12 = m_matrix.m12();
		double m21 = m_matrix.m21();
		double m22 = m_matrix.m22();
		double norm2 = m11 * m11 + m12 * m12 + m21 * m21 + m22 * m22;
		double penScale = sqrt(norm2 / 2.0) * m_zoomFactor;
		double penWidth =  LineWidth * penScale;
		QColor strokeColor = transformColor( m_stroke, 1.0 );
		SolidBrush stroke_brush( Color(stroke_trans * 255, strokeColor.red(), strokeColor.green(), strokeColor.blue()) );
		Pen stroke_pen( &stroke_brush, penWidth );
		REAL *dashes = NULL;
		
		if( m_array.count() > 0 )
		{
			dashes = new REAL[ m_array.count() ];
			for( uint i = 0; i < m_array.count();++ i )
			{
				dashes[i] = (REAL) ( m_array[i] / (double) LineWidth );
				// The following lines are needed so that gdi+ rendering matches the libart one
				if( PLineEnd == Qt::RoundCap || PLineEnd == Qt::SquareCap )
				{
					if( (i % 2) == 0 ) 
						dashes[i] = dashes[i] + 1.0;
					else
						dashes[i] = dashes[i] - 1.0;
				}
			}
			// The following two lines are needed so that gdi+ rendering matches the libart one
			if( PLineEnd == Qt::SquareCap && m_array.count() >= 2)
				stroke_pen.SetDashOffset( dashes[1]/2 );
			stroke_pen.SetDashPattern( dashes, m_array.count() );
		}
		if( PLineEnd == Qt::RoundCap )
			stroke_pen.SetLineCap( LineCapRound, LineCapRound, DashCapRound );
		else if( PLineEnd == Qt::SquareCap )
			stroke_pen.SetLineCap( LineCapSquare, LineCapSquare, DashCapFlat );
		else if( PLineEnd == Qt::FlatCap && m_array.count() == 0 && drawingClosedCurve )
			stroke_pen.SetLineCap( LineCapSquare, LineCapSquare, DashCapFlat );
		else if( PLineEnd == Qt::FlatCap )
			stroke_pen.SetLineCap( LineCapFlat, LineCapFlat, DashCapFlat );
		if( PLineJoin == Qt::RoundJoin )
			stroke_pen.SetLineJoin( LineJoinRound );
		else if( PLineJoin == Qt::BevelJoin )
			stroke_pen.SetLineJoin( LineJoinBevel );
		else if( PLineJoin == Qt::MiterJoin )
			stroke_pen.SetLineJoin( LineJoinMiter );

		graphics->DrawPath( &stroke_pen, graphicsPath );
		if( dashes )
			delete [] dashes;
	}
	restore();
#else
	int dcState;
	dcState = SaveDC( dc );
	if (mode == 0)
	{
		if( m_fillRule )
			SetPolyFillMode(dc, ALTERNATE);
		else
			SetPolyFillMode(dc, WINDING);
		if (fillMode == 2)
		{
			drawGradient( fill_gradient );
		}
		else
		{
			QColor fillColor = transformColor( m_fill, 1.0 );
			HBRUSH brush = CreateSolidBrush( RGB( qRed(fillColor.rgb()), qGreen(fillColor.rgb()), qBlue(fillColor.rgb()) ) );
			HGDIOBJ obj = SelectObject(dc, brush);
			FillPath(dc);
			SelectObject(dc, obj);
			DeleteObject(brush);
		}
	}
	else
	{
		DWORD penStyle = PS_GEOMETRIC;
		DWORD penWidth = 0;
		LOGBRUSH logBrush;
		DWORD *dashes = NULL;

		QColor strokeColor = transformColor( m_stroke, 1.0 );
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor = RGB( qRed(strokeColor.rgb()), qGreen(strokeColor.rgb()), qBlue(strokeColor.rgb()) );
		logBrush.lbHatch = 0;

		double m11 = m_matrix.m11();
		double m12 = m_matrix.m12();
		double m21 = m_matrix.m21();
		double m22 = m_matrix.m22();
		double norm2 = m11 * m11 + m12 * m12 + m21 * m21 + m22 * m22;
		double penScale = sqrt(norm2 /2.0) * m_zoomFactor;
		penWidth = (int) (LineWidth * penScale);
		
		if( m_array.count() > 0 )
		{
			dashes = new DWORD[ m_array.count() ];
			for( uint i = 0; i < m_array.count();++ i )
			{
				dashes[i] = (DWORD) (m_array[i] * penScale);
			}
			penStyle = penStyle | PS_USERSTYLE; 
		}
		if( PLineEnd == Qt::RoundCap )
			penStyle = penStyle | PS_ENDCAP_ROUND;
		else if( PLineEnd == Qt::SquareCap )
			penStyle = penStyle | PS_ENDCAP_SQUARE;
		else if( PLineEnd == Qt::FlatCap && m_array.count() == 0 && drawingClosedCurve )
			penStyle = penStyle | PS_ENDCAP_SQUARE;
		else if( PLineEnd == Qt::FlatCap )
			penStyle = penStyle | PS_ENDCAP_FLAT;
		if( PLineJoin == Qt::RoundJoin )
			penStyle = penStyle | PS_JOIN_ROUND;
		else if( PLineJoin == Qt::BevelJoin )
			penStyle = penStyle | PS_JOIN_BEVEL;
		else if( PLineJoin == Qt::MiterJoin )
			penStyle = penStyle | PS_JOIN_MITER;

		HPEN pen = ExtCreatePen(penStyle, penWidth, &logBrush, m_array.count(), dashes);
		HGDIOBJ obj = SelectObject( dc, pen);
		StrokePath( dc );
		SelectObject( dc, obj);
		DeleteObject( pen);
		if( dashes )
			delete [] dashes;
	}
	RestoreDC( dc, dcState );
#endif
}

void ScPainterEx_GDI::setClipPath()
{
#ifdef SC_USE_GDIPLUS
	graphics->SetClip( graphicsPath, CombineModeIntersect );
#else
	SelectClipPath( dc, RGN_AND );
#endif
}

void ScPainterEx_GDI::drawImage( ScImage *image )
{
#ifdef SC_USE_GDIPLUS
	FPoint ulp( 0, 0 );
	FPoint urp( image->width() * m_zoomFactor, 0 );
	FPoint blp( 0, image->height() * m_zoomFactor );
	transformPoint( ulp, ulp );
	transformPoint( blp, blp );
	transformPoint( urp, urp );
	Point destinationPoint[] = { Point(ulp.x(), ulp.y()), Point(urp.x(), urp.y()), Point(blp.x(), blp.y()) };
	ColorMatrix colorMatrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, fill_trans, 0.0f,
                               0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	if ( toGray )
	{
		colorMatrix.m[0][0] = colorMatrix.m[0][1] = colorMatrix.m[0][2] = (REAL) 0.3;
		colorMatrix.m[1][0] = colorMatrix.m[1][1] = colorMatrix.m[1][2] = (REAL) 0.59;
		colorMatrix.m[2][0] = colorMatrix.m[2][1] = colorMatrix.m[2][2] = (REAL) 0.11;
	}
	ImageAttributes imageAtt;
	imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	Bitmap bitmap( image->width(), image->height(), 4 * image->width(), PixelFormat32bppARGB, image->bits() );
	graphics->DrawImage( &bitmap, destinationPoint, 3, 0, 0, image->width(), image->height(), UnitPixel, &imageAtt );
#else
	save();
	int usage = 0;
	unsigned char* data = NULL;
	unsigned char* imageData = image->bits();
	XFORM xform = { m_matrix.m11(), m_matrix.m12(), m_matrix.m21(), m_matrix.m22(), m_matrix.dx(), m_matrix.dy() } ;
	ModifyWorldTransform(dc, &xform, MWT_LEFTMULTIPLY);
	BITMAPINFO256 bmpInfo;
	ZeroMemory( &bmpInfo, sizeof(bmpInfo));
	bmpInfo.bmiHeader.biSize = sizeof( bmpInfo.bmiHeader );
	bmpInfo.bmiHeader.biWidth = image->width();
	bmpInfo.bmiHeader.biHeight = -image->height();
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	if ( toGray )
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
		data = new uchar[bmpInfo.bmiHeader.biSizeImage];
		transformImage( image, data, scanline );
		imageData = data;
		usage = DIB_RGB_COLORS;
	}
	StretchDIBits( dc, 0, 0 , image->width() * m_zoomFactor, image->height() * m_zoomFactor, 
		0, 0, image->width(), image->height(), imageData, (BITMAPINFO*) &bmpInfo, usage, SRCCOPY );
	if( data )
		delete[] data;
	restore();
#endif
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
#ifdef SC_USE_GDIPLUS
				graphicsPath->StartFigure();
#endif
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
#ifndef SC_USE_GDIPLUS
			EndPath( dc );
#endif
			pathClosed = true;
			drawingClosedCurve = true;
		}
	}
}

void ScPainterEx_GDI::setupTextPolygon(FPointArray *points)
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
#ifdef SC_USE_GDIPLUS
				graphicsPath->CloseFigure();
				graphicsPath->StartFigure();
#endif
				moveTo( np.x() / m_zoomFactor, np.y() / m_zoomFactor );
				nPath = false;
			}
			np.setXY( points->point(poi).x() / m_zoomFactor, points->point(poi).y() / m_zoomFactor );
			np1.setXY( points->point(poi+1).x() / m_zoomFactor, points->point(poi+1).y() / m_zoomFactor );
			np2.setXY( points->point(poi+3).x() / m_zoomFactor, points->point(poi+3).y() / m_zoomFactor );
			np3.setXY( points->point(poi+2).x() / m_zoomFactor, points->point(poi+2).y() / m_zoomFactor );
			if ((np == np1) && (np2 == np3))
				lineTo( np3.x(), np3.y() );
			else
				curveTo( np1, np2, np3 );
		}
#ifdef SC_USE_GDIPLUS
		graphicsPath->CloseFigure();
#else
    	EndPath( dc );
#endif
		pathClosed = true;
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
#ifndef SC_USE_GDIPLUS
	EndPath(dc);
#endif
	pathClosed = true;
	drawingClosedCurve = true;
	fillPath();
	strokePath();
}

void ScPainterEx_GDI::drawGradient( VGradientEx& gradient )
{
	QRect clipPathRect;
#ifdef SC_USE_GDIPLUS
	save();
	setClipPath();
	getClipPathDimensions( clipPathRect );
	if ( gradient.type() == VGradientEx::linear )
		drawLinearGradient( gradient, clipPathRect );
	else if ( gradient.type() == VGradientEx::radial )
		drawCircularGradient( gradient, clipPathRect );
	restore();
#else
	SaveDC( dc );
	SelectClipPath( dc, RGN_COPY );
	getClipPathDimensions( clipPathRect );
	bool haveMsImg32 = ( hMsImg32 != NULL && gradientFill != NULL );
	if ( gradient.type() == VGradientEx::linear && haveMsImg32 )
		drawLinearGradient_GradientFill( gradient, clipPathRect );
	else if ( gradient.type() == VGradientEx::linear )
		drawLinearGradient_GradientFill( gradient, clipPathRect );
	else if (gradient.type() == VGradientEx::radial)
		drawCircularGradient( gradient, clipPathRect );
	RestoreDC( dc, -1 );
#endif
}

void ScPainterEx_GDI::drawLinearGradient( VGradientEx& gradient, const QRect& rect )
{
#ifdef SC_USE_GDIPLUS
	Matrix matrix;
	int index = 0;
	int numElements;
	int r, g, b, a;
	double cosa, sina;
	double length, scale;
	double x1, y1, x2, y2, dx, dy;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QPtrVector<VColorStopEx> colorStops = gradient.colorStops();
	VColorStopEx stop( *colorStops[0] );
	FPoint p1, p1a, p2, p2a;
	QColor color;

	if ( gradient.Stops() < 2 )
		return;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt( clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight ) * 2;
	if ( maxDim <= 0 ) 
		return;

	x1 = m_matrix.dx() + gradient.origin().x() * m_matrix.m11() * m_zoomFactor;
	y1 = m_matrix.dy() + gradient.origin().y() * m_matrix.m22() * m_zoomFactor;
	x2 = m_matrix.dx() + gradient.vector().x() * m_matrix.m11() * m_zoomFactor;
	y2 = m_matrix.dy() + gradient.vector().y() * m_matrix.m22() * m_zoomFactor;
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
	graphics->FillPath( &gradientBrush, graphicsPath );

	delete[] colors;
	delete[] positions;
#else
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
	QPtrVector<VColorStopEx> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[0] );
	VColorStopEx stop2( *colorStops[0] );
	FPoint p1, p1a, p2, p2a;
	QColor color;
	int rgb;

	if ( gradient.Stops() < 2 ) 
		return;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt( clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight ) * 2;
	if ( maxDim <= 0 ) return;
	
	scale1 = sqrt( m_matrix.m11() * m_matrix.m11() + m_matrix.m12() * m_matrix.m12() );
	scale2 = sqrt( m_matrix.m21() * m_matrix.m21() + m_matrix.m22() * m_matrix.m22() );
	increment = (int) max( scale1, scale2 );
	increment = (increment < 2) ? 2 : increment;

	x1 = m_matrix.dx() + gradient.origin().x() * m_matrix.m11() * m_zoomFactor;
	y1 = m_matrix.dy() + gradient.origin().y() * m_matrix.m22() * m_zoomFactor;
	x2 = m_matrix.dx() + gradient.vector().x() * m_matrix.m11() * m_zoomFactor;
	y2 = m_matrix.dy() + gradient.vector().y() * m_matrix.m22() * m_zoomFactor;
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

	p1a.setXY( p1.x() - cosa * maxDim, p1.y() - sina * maxDim);
	p2a.setXY( p2.x() + cosa * maxDim, p2.y() + sina * maxDim );

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
	ModifyWorldTransform(dc, &xform, MWT_LEFTMULTIPLY);

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
		for( xCurrent = xMin; xCurrent < xMax; xCurrent += increment )
		{
			colorCoeff = ( xCurrent - xMin ) / ( (double) xMax - xMin ) ;
			r3 = ( 1 - colorCoeff ) * r1 + colorCoeff * r2;
			g3 = ( 1 - colorCoeff ) * g1 + colorCoeff * g2;
			b3 = ( 1 - colorCoeff ) * b1 + colorCoeff * b2;
			a3 = ( 1 - colorCoeff ) * a1 + colorCoeff * a2;
			drawRectangleStrip( xCurrent, -maxDim/2, maxDim, maxDim, r3, g3, b3, a3 );
		}
		stop1 = stop2;
		ramp1 = ramp2;
		r1 = r2;
		g1 = g2;
		b1 = b2;
		a1 = a2;
	}
#endif
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
	QPtrVector<VColorStopEx> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[0] );
	VColorStopEx stop2( *colorStops[0] );
	FPoint p1, p1a, p2, p2a;
	QColor color;

	if ( gradient.Stops() < 2 ) 
		return;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt( clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight ) * 2;
	if ( maxDim <= 0 ) return;

	x1 = m_matrix.dx() + gradient.origin().x() * m_matrix.m11() * m_zoomFactor;
	y1 = m_matrix.dy() + gradient.origin().y() * m_matrix.m22() * m_zoomFactor;
	x2 = m_matrix.dx() + gradient.vector().x() * m_matrix.m11() * m_zoomFactor;
	y2 = m_matrix.dy() + gradient.vector().y() * m_matrix.m22() * m_zoomFactor;
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

	p1a.setXY( p1.x() - cosa * maxDim, p1.y() - sina * maxDim);
	p2a.setXY( p2.x() + cosa * maxDim, p2.y() + sina * maxDim );

	xform.eM11 = cosa;
	xform.eM12 = sina;
	xform.eM21 = -sina;
	xform.eM22 = cosa;
	xform.eDx = x1;
	xform.eDy = y1;
	ModifyWorldTransform(dc, &xform, MWT_LEFTMULTIPLY);

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
	(*gradientFill)( dc, vertices, numVertices, meshes, numMeshes, GRADIENT_FILL_RECT_H );

	// Delete used arrays
	delete[] vertices;
	delete[] meshes;
}

void ScPainterEx_GDI::drawRectangleStrip( int x, int y, int w, int h, int r, int g, int b, int a )
{
#ifndef SC_USE_GDIPLUS
	SaveDC( dc );
	HPEN hPen = CreatePen( PS_NULL, 0, RGB(255,255,255));
	HBRUSH hBrush = CreateSolidBrush( RGB(r, g, b));
	HGDIOBJ obj1 = SelectObject( dc, hPen );
	HGDIOBJ obj2 = SelectObject( dc, hBrush );
	Rectangle( dc, x, y, x + w, y + h );
	SelectObject( dc, obj2 );
	SelectObject( dc, obj1 );
	DeleteObject( hBrush );
	DeleteObject( hPen );
	RestoreDC( dc, -1 );
#endif
}

void ScPainterEx_GDI::drawCircularGradient( VGradientEx& gradient, const QRect& rect )
{
#ifdef SC_USE_GDIPLUS
	int offset = 0;
	int numElements;
	int r, g, b, a;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QPtrVector<VColorStopEx> colorStops = gradient.colorStops();
	VColorStopEx stop( *colorStops[0] );
	QColor color;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt( clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight ) * 2;
	if ( maxDim <= 0 ) return;

	FPoint pc( gradient.origin().x(), gradient.origin().y() );
	FPoint pcz( gradient.origin().x() * m_zoomFactor, gradient.origin().y() * m_zoomFactor );
	FPoint pf( gradient.focalPoint().x(), gradient.focalPoint().y() );
	FPoint pfz( gradient.focalPoint().x() * m_zoomFactor, gradient.focalPoint().y() * m_zoomFactor );
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
	rad *= m_zoomFactor;

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
	graphics->FillPath( &gradientBrush, graphicsPath );

	delete[] colors;
	delete[] positions;
#else
	int radius;
	int increment;
	int r1, g1, b1, a1;
	int r2, g2, b2, a2;
	int r3, g3, b3, a3;
	double colorCoeff;
	double ramp1, ramp2;
	double scale1, scale2;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QPtrVector<VColorStopEx> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[gradient.Stops() - 1] );
	VColorStopEx stop2( *colorStops[gradient.Stops() - 1] );
	QColor color;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = sqrt( clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight ) * 2;
	if ( maxDim <= 0 ) return;

	scale1 = sqrt( m_matrix.m11() * m_matrix.m11() + m_matrix.m21() * m_matrix.m21() );
	scale2 = sqrt( m_matrix.m12() * m_matrix.m12() + m_matrix.m22() * m_matrix.m22() );
	increment = (int) max( scale1, scale2 );
	increment = (increment < 2) ? 2 : increment;

	FPoint pc( gradient.origin().x(), gradient.origin().y() );
	FPoint pcz( gradient.origin().x() * m_zoomFactor, gradient.origin().y() * m_zoomFactor );
	FPoint pf( gradient.focalPoint().x(), gradient.focalPoint().y() );
	FPoint pfz( gradient.focalPoint().x() * m_zoomFactor, gradient.focalPoint().y() * m_zoomFactor );
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
	rad *= m_zoomFactor;

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
		color = color =  transformColor( ScColorShade(stop1.color, stop1.shade), 1.0 );
		r1 = color.red();
		g1 = color.green();
		b1 = color.blue();
		a1 = stop1.opacity * 255;
		int radMax = rad * ramp2;
		int radMin = rad * ramp1;
		for( radius = radMax; radius > radMin; radius -= increment )
		{
			colorCoeff = ( radius - radMin ) / ( (double) radMax - radMin );
			r3 = (1 - colorCoeff) * r1 + colorCoeff * r2;
			g3 = (1 - colorCoeff) * g1 + colorCoeff * g2;
			b3 = (1 - colorCoeff) * b1 + colorCoeff * b2;
			a3 = (1 - colorCoeff) * a1 + colorCoeff * a2;
			drawCircleStrip( cx - radius, cy - radius, radius * 2, radius * 2, r3, g3, b3, a3 );
		}
		stop2 = stop1;
		ramp2 = ramp1;
		r2 = r1;
		g2 = g1;
		b2 = b1;
		a2 = a1;
	}
#endif
}

void ScPainterEx_GDI::drawCircleStrip( int x, int y, int w, int h, int r, int g, int b, int a )
{
#ifndef SC_USE_GDIPLUS
	SaveDC( dc );
	HPEN hPen = CreatePen( PS_NULL, 0, RGB(255,255,255));
	HBRUSH hBrush = CreateSolidBrush( RGB(r, g, b));
	HGDIOBJ obj1 = SelectObject( dc, hPen );
	HGDIOBJ obj2 = SelectObject( dc, hBrush );
	Ellipse( dc, x, y, x + w, y + h );
	SelectObject( dc, obj2 );
	SelectObject( dc, obj1 );
	DeleteObject( hBrush );
	DeleteObject( hPen );
	RestoreDC( dc, -1 );
#endif
}

void ScPainterEx_GDI::getClipPathDimensions( QRect& r )
{
#ifdef SC_USE_GDIPLUS
	Rect rect(0, 0, 0, 0);
	Status result = graphics->GetClipBounds( &rect );
	if( result == Ok )
	{
		r.setLeft( rect.GetLeft() );
		r.setRight( rect.GetRight() );
		r.setBottom( rect.GetBottom() );
		r.setTop( rect.GetTop() );
	}
#else
	RECT rect = { 0, 0, 0, 0 };
	int result = GetClipBox( dc, &rect );
	if( result != NULLREGION && result != ERROR )
	{
		r.setLeft( rect.left );
		r.setRight( rect.right );
		r.setBottom( rect.bottom );
		r.setTop( rect.top );	
	}
#endif
}

