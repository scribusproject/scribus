
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

#include "scpainterex_ps2.h"

#include <cfloat>
#include <QPaintDevice>
#include <QPixmap>
#include <QImage>

#include <iostream>
#include <memory>
using namespace std;
 
#include <cmath>
#include "util.h"
#include "sccolorengine.h"

ScPs2OutputParams::ScPs2OutputParams(ScribusDoc* doc)
{
	document = doc;
	colorMode = ScPainterExBase::rgbMode;
	reloadImages = true;
	resolution = 72;
	useProfiles = false;
	toGray = false;
	mirrorH = false;
	mirrorV = false;
	hProfile = NULL;
	rgbToOutputColorTransform = NULL;
	rgbToOutputImageTransform = NULL;
	cmykToOutputColorTransform = NULL;
	cmykToOutputImageTransform = NULL;
}

ScPainterEx_Ps2::ScPainterEx_Ps2(QIODevice* iodev, QRect& rect, ScPs2OutputParams& options ) : ScPainterExBase()
{
	m_stream.setDevice( iodev );
	m_colorMode = options.colorMode;
	m_encoding  = Ascii85Encoding;
	m_options = options;
	m_width = rect.width();
	m_height= rect.height();
	m_x = rect.left();
	m_y = rect.top();
	m_fillColor = ScColorShade( QColor(0,0,0), 100 );
	m_strokeColor = ScColorShade( QColor(0,0,0), 100 );
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
	m_pageTrans = QMatrix();
	// Path state
	m_pathIsClosed = true;
	m_drawingClosedPath = false;
	// TODO : set proper values
	m_deviceDimX = 0;
	m_deviceDimY = 0;
	m_deviceResX = 0;
	m_deviceResY = 0;
	clear();
}

ScPainterEx_Ps2::~ScPainterEx_Ps2()
{
}

ScPainterExBase::ImageMode ScPainterEx_Ps2::imageMode()
{
	ImageMode imageMode;
	if (m_options.hProfile && m_options.rgbToOutputImageTransform && m_options.cmykToOutputImageTransform)
		imageMode = rawImages;
	else if (m_colorMode == cmykMode)
		imageMode = cmykImages;
	else
		imageMode = rgbImages;
	return imageMode;
}

void ScPainterEx_Ps2::transformImage( QImage* image, uchar* data, int scan)
{
	int rgb, grey;
	int imageWidth = image->width();
	int imageHeight = image->height();

	QRgb*  pscani;
	uchar *pscand;

	pscand = data;
	for ( int j = 0; j < imageHeight; j++ )
	{
		pscani = (QRgb *) image->scanLine(j);
		for (int i = 0; i < imageWidth; i++ )
		{
			rgb = pscani[i];
			grey = qRound(0.3 * qRed(rgb)) + qRound(0.59 * qGreen(rgb)) + qRound(0.11 * qBlue(rgb));
			pscand[i] = grey;
		}
		pscand += scan;
	}

}

void ScPainterEx_Ps2::begin()
{
}

void ScPainterEx_Ps2::end()
{

}

void ScPainterEx_Ps2::clear()
{

}

void ScPainterEx_Ps2::clear( ScColorShade &c )
{

}

const QMatrix ScPainterEx_Ps2::worldMatrix()
{
	return m_matrix;
}

void ScPainterEx_Ps2::setWorldMatrix( const QMatrix &mat )
{
	m_matrix = mat;
}

void ScPainterEx_Ps2::transformPoint( const FPoint& in, FPoint& out )
{
	double x, y;
	x = in.x() * m_matrix.m11() + in.y() * m_matrix.m21() + m_matrix.dx();
	y = in.x() * m_matrix.m12() + in.y() * m_matrix.m22() + m_matrix.dy();
	out.setX( x );
	out.setY( y );
}

void ScPainterEx_Ps2::transformPoints( const FPoint* ArrayIn, FPoint* ArrayOut, uint length )
{
	for( uint i = 0; i < length; i++ )
	{
		transformPoint( ArrayIn[i], ArrayOut[i] );
	}
}

void ScPainterEx_Ps2::translate( double x, double y )
{
	m_matrix.translate(x, y);
}

void ScPainterEx_Ps2::rotate( double r )
{
	m_matrix.rotate(r);
}

void ScPainterEx_Ps2::scale( double x, double y )
{
	m_matrix.scale(x, y);
}

void ScPainterEx_Ps2::moveTo( const double &x, const double &y )
{
	FPoint pp( x, y );
	transformPoint( pp, pp );
	m_stream << QString("%1 %2 moveto\n").arg(pp.x()).arg(pp.y());
}

void ScPainterEx_Ps2::lineTo( const double &x, const double &y )
{
	FPoint pp( x, y );
	transformPoint( pp, pp );
	m_stream << QString("%1 %2 lineto\n").arg(pp.x()).arg(pp.y());
}

void ScPainterEx_Ps2::curveTo( FPoint p1, FPoint p2, FPoint p3 )
{
	FPoint pps[3];
	pps[0].setXY( p1.x(), p1.y() );
	pps[1].setXY( p2.x(), p2.y() );
	pps[2].setXY( p3.x(), p3.y() );
	transformPoints( pps, pps, 3 );
	m_stream << QString("%1 %2 %3 %4 %5 %6 curveto\n").arg(pps[0].x()).arg(pps[0].y()).arg(pps[1].x()).arg(pps[1].y()).arg(pps[2].x()).arg(pps[2].y());
}

void ScPainterEx_Ps2::newPath()
{
   m_stream << "newpath\n";
   //pathIsClosed = false;
}

void ScPainterEx_Ps2::closePath()
{
	m_stream << "closepath\n";
	m_pathIsClosed = true;
}

void ScPainterEx_Ps2::setFillRule( bool fillRule )
{
	m_fillRule = fillRule;
}

void ScPainterEx_Ps2::setFillMode( int fill )
{
	m_fillMode = fill;
}

void ScPainterEx_Ps2::setGradient(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc)
{
	m_fillGradient.setType(mode);
	m_fillGradient.setOrigin(orig);
	m_fillGradient.setVector(vec);
	m_fillGradient.setFocalPoint(foc);
}

void ScPainterEx_Ps2::setPattern( ScPattern* pattern, QMatrix& patternTransform )
{
	m_pattern = pattern;
	m_patternTransform = patternTransform;
}

void ScPainterEx_Ps2::fillTextPath()
{
	drawVPath( 0 );
}

void ScPainterEx_Ps2::strokeTextPath()
{
	if( m_lineWidth == 0 )
		return;
	drawVPath( 1 );
}

void ScPainterEx_Ps2::fillPath()
{
	if(!m_pathIsClosed)
		closePath();
	if( m_fillMode != 0)
		drawVPath( 0 );
}

void ScPainterEx_Ps2::strokePath()
{
	if( m_lineWidth == 0 )
		return;
	save();
	if(!m_pathIsClosed)
		closePath();
	drawVPath( 1 );
	restore();
}

ScColorShade ScPainterEx_Ps2::pen()
{
	return m_strokeColor;
}

ScColorShade ScPainterEx_Ps2::brush()
{
	return m_fillColor;
}

void ScPainterEx_Ps2::setPen( const ScColorShade &c )
{
	m_strokeColor = c;
}

void ScPainterEx_Ps2::setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo )
{
	m_offset = 0;
	m_strokeColor = c;
	m_lineWidth = w;
	m_lineEnd = ca;
	m_lineJoin = jo;
	double Dt = qMax(2*w, 1.0);
	double Da = qMax(6*w, 1.0);
	m_array.clear();
	if (st == Qt::DashLine)
		m_array << Da << Dt;
	else if (st == Qt::DotLine)
		m_array << Dt;
	else if (st == Qt::DashDotLine)
		m_array << Da << Dt << Dt << Dt;
	else if (st == Qt::DashDotDotLine)
		m_array << Da << Dt << Dt << Dt << Dt << Dt; 
}

void ScPainterEx_Ps2::setLineWidth( double w )
{
	m_lineWidth = w;
}

void ScPainterEx_Ps2::setPenOpacity( double op )
{
	m_strokeTrans = op;
}

void ScPainterEx_Ps2::setDash(const QVector<double>& array, double ofs)
{
	m_array = array;
	m_offset = ofs;
}

void ScPainterEx_Ps2::setBrush( const ScColorShade &c )
{
	m_fillColor = c;
}

void ScPainterEx_Ps2::setBrushOpacity( double op )
{
	m_fillTrans = op;
}

void ScPainterEx_Ps2::setOpacity( double op )
{
	m_fillTrans = op;
	m_strokeTrans = op;
}

void ScPainterEx_Ps2::setFont( const QFont &f)
{
	m_font = f;
}

QFont ScPainterEx_Ps2::font()
{
	return m_font;
}

void ScPainterEx_Ps2::save()
{
	m_stack.push( m_matrix );
	m_stream << "gsave\n";
}

void ScPainterEx_Ps2::restore()
{
	m_matrix = m_stack.pop();
	m_stream << "grestore\n";
}

void ScPainterEx_Ps2::setRasterOp( int   )
{
}

void ScPainterEx_Ps2::drawVPath( int mode )
{
	save();
	if (mode == 0)
	{
		if (m_fillMode == ScPainterExBase::Gradient)
			drawGradient( m_fillGradient );
		else
			putColor( m_fillColor, true );
	}
	else
	{
		double m11 = m_matrix.m11();
		double m12 = m_matrix.m12();
		double m21 = m_matrix.m21();
		double m22 = m_matrix.m22();
		double norm2 = m11 * m11 + m12 * m12 + m21 * m21 + m22 * m22;
		double penScale = sqrt(norm2 /2.0);
		double penWidth = (int) (m_lineWidth * penScale);
		m_stream << QString("%1 setlinewidth\n").arg(penWidth);

		m_stream << "[";
		for( int i = 0; i < m_array.count();++ i )
		{
			m_stream << QString("%1").arg(m_array[i]) << " ";
		}
		m_stream << "]";
		m_stream << " 0 setdash\n";

		if( m_lineEnd == Qt::RoundCap )
			m_stream << "1 setlinecap\n";
		else if( m_lineEnd == Qt::SquareCap )
			m_stream << "2 setlinecap\n";
		else if( m_lineEnd == Qt::FlatCap )
			m_stream << "0 setlinecap\n";
		else
			m_stream << "0 setlinecap\n";

		if( m_lineJoin == Qt::RoundJoin )
			m_stream << "1 setlinejoin\n";
		else if( m_lineJoin == Qt::BevelJoin )
			m_stream << "2 setlinejoin\n";
		else if( m_lineJoin == Qt::MiterJoin )
			m_stream << "0 setlinejoin\n";
		else
			m_stream << "0 setlinejoin\n";

		putColor(m_strokeColor, false);
	}
	restore();
}

void ScPainterEx_Ps2::setClipPath()
{
	//TODO eoclip or clip?
	m_stream << "clip newpath\n";
	m_clipBBox = m_pathBBox;
}

void ScPainterEx_Ps2::putColor( ScColorShade& colorShade, bool doFill )
{
	ColorMode colorMode = m_colorMode;
	int r = 0, g = 0, b = 0, gray = 0;
	int c = 0, m = 0, y = 0, k = 0;
	if ( m_options.toGray )
	{
		RGBColor rgb;
		ScColorEngine::getShadeColorRGB(colorShade.color, m_options.document, rgb, colorShade.shade);
		rgb.getValues(r, g, b);
		gray = qRound(0.3 * r + 0.59 * g + 0.11 * b);
		if ( doFill && m_fillRule )
			m_stream << QString("%1 setgray eofill\n").arg(gray/255.0);
		else if ( doFill )
			m_stream << QString("%1 setgray fill\n").arg(gray/255.0);
		else
			m_stream << QString("%1 setgray stroke\n").arg(gray/255.0);
		return;
	}
	else if ( m_options.hProfile && m_options.rgbToOutputColorTransform && m_options.cmykToOutputColorTransform)
	{
		DWORD colorIn[4];
		DWORD colorOut[4];
		cmsHTRANSFORM cmsTranform = NULL;
		if( colorShade.color.getColorModel() == colorModelRGB )
		{
			RGBColor rgb;
			ScColorEngine::getShadeColorRGB(colorShade.color, m_options.document, rgb, colorShade.shade);
			colorIn[0] = rgb.r;
			colorIn[1] = rgb.g;
			colorIn[2] = rgb.b;
			colorMode = rgbMode;
			cmsTranform = m_options.rgbToOutputColorTransform;
		} 
		else
		{
			CMYKColor cmyk;
			ScColorEngine::getShadeColorCMYK(colorShade.color, m_options.document, cmyk, colorShade.shade);
			colorIn[0] = cmyk.c;
			colorIn[1] = cmyk.m;
			colorIn[2] = cmyk.y;
			colorIn[3] = cmyk.k;
			colorMode = cmykMode;
			cmsTranform = m_options.cmykToOutputColorTransform;
		}
		cmsDoTransform( cmsTranform, colorIn, colorOut, 1 );
		if (static_cast<int>(cmsGetColorSpace(m_options.hProfile)) == icSigRgbData)
		{
			r = colorOut[0] / 257;
			g = colorOut[1] / 257;
			b = colorOut[2] / 257;
			colorMode = rgbMode;
		}
		else if (static_cast<int>(cmsGetColorSpace(m_options.hProfile)) == icSigCmykData)
		{
			c = colorOut[0] / 257;
			m = colorOut[1] / 257;
			y = colorOut[2] / 257;
			k = colorOut[3] / 257;
			colorMode = cmykMode;
		}
	}
	else if ( m_colorMode == rgbMode )
	{
		RGBColor rgb;
		colorMode = rgbMode;
		ScColorEngine::getShadeColorRGB(colorShade.color, m_options.document, rgb, colorShade.shade);
		rgb.getValues(r, g, b);
	}
	else if ( m_colorMode == cmykMode )
	{
		CMYKColor cmyk;
		colorMode = cmykMode;
		ScColorEngine::getShadeColorCMYK(colorShade.color, m_options.document, cmyk, colorShade.shade);
		cmyk.getValues(c, m, y, k);
	}

	// Finally output to file
	if ( colorMode == rgbMode )
	{
		if ( doFill && m_fillRule )
			m_stream << QString("%1 %2 %3 setrgbcolor eofill\n").arg(r/255.0).arg(g/255.0).arg(b/255.0);
		else if ( doFill )
			m_stream << QString("%1 %2 %3 setrgbcolor fill\n").arg(r/255.0).arg(g/255.0).arg(b/255.0);
		else
			m_stream << QString("%1 %2 %3 setrgbcolor stroke\n").arg(r/255.0).arg(g/255.0).arg(b/255.0);			
	}
	else if ( colorMode == cmykMode )
	{
		QString colstr1 = QString("%1 %2 %3 %4 setcmykcolor").arg(c/255.0).arg(m/255.0).arg(y/255.0).arg(k/255.0);
		if ( doFill && m_fillRule )
			m_stream << colstr1 << " eofill\n";
		else if ( doFill )
			m_stream << colstr1 << " fill\n";
		else
			m_stream << colstr1 << " stroke\n";	
	}
}

void ScPainterEx_Ps2::drawImage( ScImage *image, ScPainterExBase::ImageMode mode )
{
	bool rgbMode = (mode == ScPainterExBase::rgbImages || mode == ScPainterExBase::rgbProofImages);
	bool rgbPicture = rgbMode || (mode == ScPainterExBase::rawImages && image->imgInfo.colorspace == ColorSpaceRGB);
	bool cmykPicture = (mode == ScPainterExBase::cmykImages || (mode == ScPainterExBase::rawImages && image->imgInfo.colorspace == ColorSpaceCMYK)); 

	if( rgbPicture )
		drawRGBImage_ps2( image );
	else if( cmykPicture )
		drawCMYKImage_ps2( image );
	else
		qDebug( "ScPainterEx_Ps2::drawImage() : unknown image type" );
}

void ScPainterEx_Ps2::drawRGBImage_ps2( ScImage *image )
{
	save();
	QString sc("[%1 %2 %3 %4 %5 %6] concat\n");
	m_stream << sc.arg(m_matrix.m11()).arg(m_matrix.m12()).arg(m_matrix.m21()).arg(m_matrix.m22()).arg(m_matrix.dx()).arg(m_matrix.dy());
	m_stream << QString("%1 %2 scale\n").arg(image->width()).arg(image->height());
	m_stream << "/DeviceRGB setcolorspace\n";
	if (hasAlphaChannel(image))
	{
		QImage imgTemp = image->qImage();
		QImage mask = imgTemp.createAlphaMask();
		if (m_encoding == Ascii85Encoding)
		{
			m_stream << "currentfile /ASCII85Decode filter /ReusableStreamDecode filter\n";
			writeRGBImageToStream_Ascii85(image);
		}
		else
		{
			m_stream << "currentfile /ASCIIHexDecode filter /ReusableStreamDecode filter\n";
			writeRGBImageToStream_AsciiHex(image);
		}
		m_stream << "/myImage exch def\n";
		m_stream << "currentfile /ASCIIHexDecode filter /ReusableStreamDecode filter\n";
		writeMaskToStream(&mask);
		m_stream << "/myMask exch def\n";
		m_stream << QString("<<\n");
		m_stream << QString("    /PaintType   1\n");
		m_stream << QString("    /PatternType 1\n");
		m_stream << QString("    /TilingType  3\n");
		m_stream << QString("    /BBox        [ 0 0 1 1 ]\n");
		m_stream << QString("    /XStep       2\n");
		m_stream << QString("    /YStep       2\n");
		m_stream << QString("    /PaintProc   {\n");
		m_stream << QString("        pop\n");
		m_stream << QString("        0 0 0 setrgbcolor\n");
		m_stream << QString("        <<\n");
		m_stream << QString("            /ImageType 1\n");
		m_stream << QString("            /Width     %1\n").arg(image->width());
		m_stream << QString("            /Height    %1\n").arg(image->height());
		m_stream << QString("            /ImageMatrix [ %1 0 0 %2 0 0 ]\n").arg(image->width()).arg(image->height());
		m_stream << QString("            /BitsPerComponent 8\n");
		m_stream << QString("            /Decode [0 1 0 1 0 1]\n");
		m_stream << QString("            /DataSource myImage\n");
		m_stream << QString("        >>\n");
		m_stream << QString("        image\n");
		m_stream << QString("    }\n");
		m_stream << QString(">> matrix makepattern setpattern\n");
		m_stream << QString("<<\n");
		m_stream << QString("    /ImageType 1\n");
		m_stream << QString("    /Width %1 \n").arg(image->width());
		m_stream << QString("    /Height %1 \n").arg(image->height());
		m_stream << QString("    /BitsPerComponent 1\n");
		m_stream << QString("    /Decode [1 0]\n");
		m_stream << QString("    /ImageMatrix [ %1 0 0 %2 0 0 ]\n").arg(image->width()).arg(image->height());
		m_stream << QString("    /DataSource myMask\n");
		m_stream << QString(">>\n");
		m_stream << QString("imagemask\n");
	}
	else
	{
		m_stream << QString("<<\n");
		m_stream << QString("    /ImageType 1\n");
		m_stream << QString("    /Width %1\n").arg(image->width());
		m_stream << QString("    /Height %1\n").arg(image->height());
		m_stream << QString("    /ImageMatrix [ %1 0 0 %2 0 0 ]\n").arg(image->width()).arg(image->height());
		m_stream << QString("    /Decode [0 1 0 1 0 1]\n");
		m_stream << QString("    /BitsPerComponent 8\n");
		//TODO case where compression is available
		if (m_encoding == Ascii85Encoding)
		{
			m_stream << QString("    /DataSource currentfile /ASCII85Decode filter\n");
			m_stream << QString(">>\n");
			m_stream << "image\n";
			writeRGBImageToStream_Ascii85(image);
		}
		else
		{
			m_stream << QString("    /DataSource currentfile /ASCIIHexDecode filter\n");
			m_stream << QString(">>\n");
			m_stream << "image\n";
			writeRGBImageToStream_AsciiHex(image);
		}
	}
	restore();
}

void ScPainterEx_Ps2::drawCMYKImage_ps2( ScImage *image )
{
	save();
	QString sc("[%1 %2 %3 %4 %5 %6] concat\n");
	m_stream << sc.arg(m_matrix.m11()).arg(m_matrix.m12()).arg(m_matrix.m21()).arg(m_matrix.m22()).arg(m_matrix.dx()).arg(m_matrix.dy());
	m_stream << QString("%1 %2 scale\n").arg(image->width()).arg(image->height());
	m_stream << "/DeviceCMYK setcolorspace\n";
	// TODO case where image have a mask
	bool haveMask = false;
	if (haveMask)
	{

	}
	else
	{
		m_stream << "<<\n";
		m_stream << QString("    /ImageType 1\n");
		m_stream << QString("    /Width %1\n").arg(image->width());
		m_stream << QString("    /Height %1\n").arg(image->height());
		m_stream << QString("    /ImageMatrix [ %1 0 0 %2 0 0 ]\n").arg(image->width()).arg(image->height());
		m_stream << QString("    /Decode [0 1 0 1 0 1 0 1]\n");
		m_stream << QString("    /BitsPerComponent 8\n");
		//TODO case where compression is available
		if (m_encoding == Ascii85Encoding)
		{
			m_stream << QString("    /DataSource currentfile /ASCII85Decode filter\n");
			m_stream << QString(">>\n");
			m_stream << "image\n";
			writeCMYKImageToStream_Ascii85(image);
		}
		else
		{
			m_stream << QString("    /DataSource currentfile /ASCIIHexDecode filter\n");
			m_stream << QString(">>\n");
			m_stream << "image\n";
			writeCMYKImageToStream_AsciiHex(image);
		}
	}
	restore();
}

bool ScPainterEx_Ps2::hasAlphaChannel( ScImage* image )
{
	bool   hasAlpha = false;
	int    width = image->width();
	int    height = image->height();
	for( int y = 0; y < height; y++ )
	{
		QRgb* imageBits = (QRgb*)(image->qImage().scanLine(y));
		for( int x = 0; x < width; ++x )
		{
			if( qAlpha(*imageBits) != 255 )
			{
				hasAlpha = true;
				break;
			}
			imageBits++;
		}
	}
	return hasAlpha;
}

void ScPainterEx_Ps2::writeMaskToStream( QImage* image )
{
	int    length = 0;
	unsigned char bits;
	int    width = image->width() / 8;
	int    height = image->height();
	if ((image->width() % 8) != 0)
		width++;
	for( int y = 0; y < height; ++y )
	{
		unsigned char* imageBits = image->scanLine(y);
		for( int x = 0; x < width; x++ )
		{
			length++;
			bits = imageBits[x];
			m_stream << toHex(bits);
			if ( (length % 49) == 0 )
				m_stream << "\n";
		}
	}
	m_stream << "\n>\n";
}

void ScPainterEx_Ps2::writeRGBImageToStream_Ascii85( ScImage* image )
{
	int  cindex;
	int  pending = 0, written = 0;
	bool allZero  = true;
	unsigned char  four_tuple[4];
	const char* ascii85;
	quint32 value;

	int width = image->width();
	int height = image->height();
	for( int y = 0; y < height; ++y )
	{
		QRgb* imageBits = (QRgb*)(image->qImage().scanLine(y));
		for( int x = 0; x < width; ++x )
		{
			cindex = 0;
			while (cindex < 3)
			{
				if (cindex == 0)
					four_tuple[pending++] = (uchar) qRed(*imageBits);
				else if (cindex == 1)
					four_tuple[pending++] = (uchar) qGreen(*imageBits);
				else if (cindex == 2)
					four_tuple[pending++] = (uchar) qBlue(*imageBits);
				if (pending == 4) 
				{
					value   = four_tuple[0] << 24 | four_tuple[1] << 16 | four_tuple[2] << 8 | four_tuple[3];
					ascii85 = toAscii85(value, allZero);
					if (allZero)
						m_stream << "z";
					else
						m_stream << ascii85;
					written += ((allZero) ? 1 : 5);
					if (written > 75)
					{
						m_stream << "\n";
						written = 0;
					}
					pending = 0;
				}
				++cindex;
			}
			
		}
	}
	if (pending) 
	{
		unsigned char five_tuple[6];
		memset (four_tuple + pending, 0, 4 - pending);
		value   = four_tuple[0] << 24 | four_tuple[1] << 16 | four_tuple[2] << 8 | four_tuple[3];
		ascii85 = toAscii85(value, allZero);
		memcpy (five_tuple, ascii85, 5);
		five_tuple[pending + 1] = 0;
		m_stream << (const char*) five_tuple;
	}
	m_stream << "~>\n";
}

void ScPainterEx_Ps2::writeRGBImageToStream_AsciiHex ( ScImage* image )
{
	int  length = 0;
	int  width = image->width();
	int  height = image->height();
	for( int y = 0; y < height; ++y )
	{
		QRgb* imageBits = (QRgb*)(image->qImage().scanLine(y));
		for( int x = 0; x < width; ++x )
		{
			length++;
			uchar r = (uchar) qRed(*imageBits);
			m_stream << toHex(r);
			uchar g = (uchar) qGreen(*imageBits);
			m_stream << toHex(g);
			uchar b = (uchar) qBlue(*imageBits);
			m_stream << toHex(b);
			if ( (length % 17) == 0 )
				m_stream << "\n";
			imageBits++;
		}
	}
	m_stream << "\n>\n";
}

void ScPainterEx_Ps2::writeCMYKImageToStream_Ascii85( ScImage* image )
{
	int  written = 0;
	bool allZero  = true;
	unsigned char  four_tuple[4];
	const char* ascii85;
	quint32 value;

	int width = image->width();
	int height = image->height();
	for( int y = 0; y < height; y++ )
	{
		QRgb* imageBits = (QRgb*)(image->qImage().scanLine(y));
		for( int x = 0; x < width; x++ )
		{
			four_tuple[0] = (uchar) qRed(*imageBits);
			four_tuple[1] = (uchar) qGreen(*imageBits);
			four_tuple[2] = (uchar) qBlue(*imageBits);
			four_tuple[3] = (uchar) qAlpha(*imageBits);
			value   = four_tuple[0] << 24 | four_tuple[1] << 16 | four_tuple[2] << 8 | four_tuple[3];
			ascii85 = toAscii85(value, allZero);
			if (allZero)
				m_stream << "z";
			else
				m_stream << ascii85;
			written += ((allZero) ? 1 : 5);
			if (written > 75)
			{
				m_stream << "\n";
				written = 0;
			}
			imageBits++;
		}
	}
	m_stream << "~>\n";
}

void ScPainterEx_Ps2::writeCMYKImageToStream_AsciiHex( ScImage* image )
{
	int length = 0;
	int width = image->width();
	int height = image->height();
	for( int y = 0; y < height; ++y )
	{
		QRgb* imageBits = (QRgb*)(image->qImage().scanLine(y));
		for( int x = 0; x < width; ++x )
		{
			length++;
			uchar c = (uchar) qRed(*imageBits);
			m_stream << toHex(c);
			uchar m = (uchar) qGreen(*imageBits);
			m_stream << toHex(m);
			uchar y = (uchar) qBlue(*imageBits);
			m_stream << toHex(y);
			uchar k = (uchar) qAlpha(*imageBits);
			m_stream << toHex(k);
			if ( (length % 13) == 0 )
				m_stream << "\n";
			imageBits++;
		}
	}
	m_stream << "\n>\n";
}

void ScPainterEx_Ps2::setupPolygon(FPointArray *points, bool closed)
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
				// TODO startfigure needed?
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
			closePath();
			m_drawingClosedPath = true;
		}
	}
	getPathBoundingBox( points, m_pathBBox );
}

void ScPainterEx_Ps2::drawPolygon()
{
	fillPath();
}

void ScPainterEx_Ps2::drawPolyLine()
{
	strokePath();
}

void ScPainterEx_Ps2::drawLine(FPoint start, FPoint end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainterEx_Ps2::drawRect(double x, double y, double w, double h)
{
	newPath();
	moveTo( x, y );
	lineTo( x+w, y );
	lineTo( x+w, y+h );
	lineTo( x, y+h );
	lineTo( x, y );
	closePath();
	m_drawingClosedPath = true;
	fillPath();
	strokePath();
}

void ScPainterEx_Ps2::drawGradient( VGradientEx& gradient )
{
	QRect clipPathRect;
	save();
	//setClipPath();
	m_stream << "clip\n";
	m_clipBBox = m_pathBBox;
	if ( gradient.type() == VGradientEx::linear )
		drawLinearGradient( gradient, m_clipBBox );
	else if ( gradient.type() == VGradientEx::radial )
		drawCircularGradient( gradient, m_clipBBox );
	restore();
}

void ScPainterEx_Ps2::drawLinearGradient( VGradientEx& gradient, const QRect& rect )
{
	if (m_colorMode == cmykMode)
		drawLinearGradient_CMYK(gradient, rect);
	else
		drawLinearGradient_RGB(gradient, rect);
}

void ScPainterEx_Ps2::drawLinearGradient_RGB( VGradientEx& gradient, const QRect& rect )
{
	int r1, g1, b1;
	int r2, g2, b2;
	double cosa, sina;
	double ramp1, ramp2;
	double scale1, scale2;
	double length, scale;
	double x1, y1, x2, y2, dx, dy;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[0] );
	VColorStopEx stop2( *colorStops[0] );
	RGBColor rgb;
	FPoint p1, p2;
	QColor color;

	if ( gradient.Stops() < 2 ) 
		return;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = qRound(sqrt( (double) (clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight) )) * 2;
	if ( maxDim <= 0 ) return;
	
	scale1 = sqrt( m_matrix.m11() * m_matrix.m11() + m_matrix.m12() * m_matrix.m12() );
	scale2 = sqrt( m_matrix.m21() * m_matrix.m21() + m_matrix.m22() * m_matrix.m22() );

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

	QString sc("[%1 %2 %3 %4 %5 %6] concat\n");
	m_stream << sc.arg(cosa).arg(sina).arg(-sina).arg(cosa).arg(x1).arg(y1);

	ramp1 = stop1.rampPoint;
	ScColorEngine::getShadeColorRGB(stop1.color, m_options.document, rgb, stop1.shade);
	rgb.getValues(r1, g1, b1);
	m_stream << QString("%1 %2 %3 setrgbcolor fill\n").arg(r1/255.0).arg(g1/255.0).arg(b1/255.0);

	for( uint index = 1; index < gradient.Stops(); index++)
	{
		stop2 = *colorStops[index];
		ramp2 = stop2.rampPoint;
		ScColorEngine::getShadeColorRGB(stop2.color, m_options.document, rgb, stop2.shade);
		rgb.getValues(r2, g2, b2);
		double xMin = length * ramp1;
		double xMax = length * ramp2;

		m_stream << QString("/x1 %1 def\n").arg(xMin);
		m_stream << QString("/x2 %1 def\n").arg(xMax);
		m_stream << QString("/r1 %1 def\n").arg(r1/255.0);
		m_stream << QString("/g1 %1 def\n").arg(g1/255.0);
		m_stream << QString("/b1 %1 def\n").arg(b1/255.0);
		m_stream << QString("/r2 %1 def\n").arg(r2/255.0);
		m_stream << QString("/g2 %1 def\n").arg(g2/255.0);
		m_stream << QString("/b2 %1 def\n").arg(b2/255.0);
		m_stream << QString("/maxd %1 def\n").arg(maxDim);
		m_stream << QString("/mmaxd %1 def\n").arg(-maxDim/2.0);
		m_stream << "r1 r2 sub abs 255 mul /dr exch def\n";
		m_stream << "g1 g2 sub abs 255 mul /dg exch def\n";
		m_stream << "b1 b2 sub abs 255 mul /db exch def\n";
		m_stream << "dr dg add db add /colsteps exch def\n";
		m_stream << "colsteps 0.0 eq { /colsteps 1 def } if\n";
		m_stream << "x2 x1 sub colsteps div /xinc exch def\n";
		m_stream << "xinc 2 mul /xinc2 exch def\n";
		m_stream << "x2 xinc2 lt { /xinc2 x2 def } if\n";
		m_stream << "r2 r1 sub colsteps div /rinc exch def\n";
		m_stream << "g2 g1 sub colsteps div /ginc exch def\n";
		m_stream << "b2 b1 sub colsteps div /binc exch def\n";
		//m_stream << "r1 g1 b1 setrgbcolor\n";
		//m_stream << "0 mmaxd maxd maxd rectfill\n";
		m_stream << "0 1 colsteps\n";
		m_stream << "{\n";
		m_stream << "    r1 g1 b1 setrgbcolor\n";
		m_stream << "    x1 mmaxd xinc2 maxd rectfill\n"; //rectfill is a ps2 operator
		m_stream << "    x1 xinc add /x1 exch def\n";
		m_stream << "    r1 rinc add /r1 exch def\n";
		m_stream << "    g1 ginc add /g1 exch def\n";
		m_stream << "    b1 binc add /b1 exch def\n";
		m_stream << "} for\n";
		m_stream << "x2 mmaxd maxd maxd rectfill\n";

		stop1 = stop2;
		ramp1 = ramp2;
		r1 = r2;
		g1 = g2;
		b1 = b2;
	}
}

void ScPainterEx_Ps2::drawLinearGradient_CMYK( VGradientEx& gradient, const QRect& rect )
{
	int c1, m1, j1, n1;
	int c2, m2, j2, n2;
	double cosa, sina;
	double ramp1, ramp2;
	double scale1, scale2;
	double length, scale;
	double x1, y1, x2, y2, dx, dy;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[0] );
	VColorStopEx stop2( *colorStops[0] );
	CMYKColor cmyk;
	FPoint p1, p2;
	QColor color;

	if ( gradient.Stops() < 2 ) 
		return;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = qRound(sqrt( (double) (clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight) )) * 2;
	if ( maxDim <= 0 ) return;
	
	scale1 = sqrt( m_matrix.m11() * m_matrix.m11() + m_matrix.m12() * m_matrix.m12() );
	scale2 = sqrt( m_matrix.m21() * m_matrix.m21() + m_matrix.m22() * m_matrix.m22() );

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

	QString sc("[%1 %2 %3 %4 %5 %6] concat\n");
	m_stream << sc.arg(cosa).arg(sina).arg(-sina).arg(cosa).arg(x1).arg(y1);

	ramp1 = stop1.rampPoint;
	ScColorEngine::getShadeColorCMYK(stop1.color, m_options.document, cmyk, stop1.shade);
	cmyk.getValues(c1, m1, j1, n1);
	m_stream << QString("/DeviceCMYK setcolorspace\n");
	m_stream << QString("%1 %2 %3 %4 setcolor fill\n").arg(c1/255.0).arg(m1/255.0).arg(j1/255.0).arg(n1/255.0);

	for( uint index = 1; index < gradient.Stops(); index++)
	{
		stop2 = *colorStops[index];
		ramp2 = stop2.rampPoint;
		ScColorEngine::getShadeColorCMYK(stop2.color, m_options.document, cmyk, stop2.shade);
		cmyk.getValues(c2, m2, j2, n2);
		double xMin = length * ramp1;
		double xMax = length * ramp2;

		m_stream << QString("/x1 %1 def\n").arg(xMin);
		m_stream << QString("/x2 %1 def\n").arg(xMax);
		m_stream << QString("/c1 %1 def\n").arg(c1/255.0);
		m_stream << QString("/m1 %1 def\n").arg(m1/255.0);
		m_stream << QString("/y1 %1 def\n").arg(j1/255.0);
		m_stream << QString("/k1 %1 def\n").arg(n1/255.0);
		m_stream << QString("/c2 %1 def\n").arg(c2/255.0);
		m_stream << QString("/m2 %1 def\n").arg(m2/255.0);
		m_stream << QString("/y2 %1 def\n").arg(j2/255.0);
		m_stream << QString("/k2 %1 def\n").arg(n2/255.0);
		m_stream << QString("/maxd %1 def\n").arg(maxDim);
		m_stream << QString("/mmaxd %1 def\n").arg(-maxDim/2.0);
		m_stream << "c1 c2 sub abs 255 mul /dc exch def\n";
		m_stream << "m1 m2 sub abs 255 mul /dm exch def\n";
		m_stream << "y1 y2 sub abs 255 mul /dy exch def\n";
		m_stream << "k1 k2 sub abs 255 mul /dk exch def\n";
		m_stream << "dc dm add dy add dk add /colsteps exch def\n";
		m_stream << "colsteps 0.0 eq { /colsteps 1 def } if\n";
		m_stream << "x2 x1 sub colsteps div /xinc exch def\n";
		m_stream << "xinc 2 mul /xinc2 exch def\n";
		m_stream << "x2 xinc2 lt { /xinc2 x2 def } if\n";
		m_stream << "c2 c1 sub colsteps div /cinc exch def\n";
		m_stream << "m2 m1 sub colsteps div /minc exch def\n";
		m_stream << "y2 y1 sub colsteps div /yinc exch def\n";
		m_stream << "k2 k1 sub colsteps div /kinc exch def\n";
		//m_stream << "r1 g1 b1 setrgbcolor\n";
		//m_stream << "0 mmaxd maxd maxd rectfill\n";
		m_stream << "0 1 colsteps\n";
		m_stream << "{\n";
		m_stream << "    c1 m1 y1 k1 setcolor\n";
		m_stream << "    x1 mmaxd xinc2 maxd rectfill\n"; //rectfill is a ps2 operator
		m_stream << "    x1 xinc add /x1 exch def\n";
		m_stream << "    c1 cinc add /c1 exch def\n";
		m_stream << "    m1 minc add /m1 exch def\n";
		m_stream << "    y1 yinc add /y1 exch def\n";
		m_stream << "    k1 kinc add /k1 exch def\n";
		m_stream << "} for\n";
		m_stream << "x2 mmaxd maxd maxd rectfill\n";

		stop1 = stop2;
		ramp1 = ramp2;
		c1 = c2;
		m1 = m2;
		j1 = j2;
		n1 = n2;
	}
}

void ScPainterEx_Ps2::drawCircularGradient( VGradientEx& gradient, const QRect& rect )
{
	if (m_colorMode == cmykMode)
		drawCircularGradient_CMYK(gradient, rect);
	else
		drawCircularGradient_RGB(gradient, rect);
}

void ScPainterEx_Ps2::drawCircularGradient_RGB( VGradientEx& gradient, const QRect& rect )
{
	int r1, g1, b1;
	int r2, g2, b2;
	double ramp1, ramp2;
	double scale1, scale2;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[gradient.Stops() - 1] );
	VColorStopEx stop2( *colorStops[gradient.Stops() - 1] );
	RGBColor rgb;
	QColor color;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = qRound(sqrt( (double) (clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight) )) * 2;
	if ( maxDim <= 0 ) return;

	scale1 = sqrt( m_matrix.m11() * m_matrix.m11() + m_matrix.m21() * m_matrix.m21() );
	scale2 = sqrt( m_matrix.m12() * m_matrix.m12() + m_matrix.m22() * m_matrix.m22() );

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
	double rad = sqrt( pow(pv.x() -pc.x(), 2) + pow(pv.y() - pc.y(), 2) );

	ramp2 = stop2.rampPoint;
	ScColorEngine::getShadeColorRGB(stop2.color, m_options.document, rgb, stop2.shade);
	rgb.getValues(r2, g2, b2);
	m_stream << QString("%1 %2 %3 setrgbcolor fill\n").arg(r2).arg(g2).arg(b2);
	if ( gradient.Stops() < 2 ) return;

	for( int index = gradient.Stops() - 2; index >= 0; index--)
	{
		stop1 = *colorStops[index];
		ramp1 = stop1.rampPoint;
		ScColorEngine::getShadeColorRGB(stop1.color, m_options.document, rgb, stop1.shade);
		rgb.getValues(r1, g1, b1);
		int radMax = qRound(rad * ramp2);
		int radMin = qRound(rad * ramp1);
		
		m_stream << QString("/rad1 %1 def\n").arg(radMax);
		m_stream << QString("/rad2 %1 def\n").arg(radMin);
		m_stream << QString("/circx %1 def\n").arg(cx);
		m_stream << QString("/circy %1 def\n").arg(cy);
		m_stream << QString("/r1 %1 def\n").arg(r1/255.0);
		m_stream << QString("/g1 %1 def\n").arg(g1/255.0);
		m_stream << QString("/b1 %1 def\n").arg(b1/255.0);
		m_stream << QString("/r2 %1 def\n").arg(r2/255.0);
		m_stream << QString("/g2 %1 def\n").arg(g2/255.0);
		m_stream << QString("/b2 %1 def\n").arg(b2/255.0);
		m_stream << "r1 r2 sub abs 255 mul /dr exch def\n";
		m_stream << "g1 g2 sub abs 255 mul /dg exch def\n";
		m_stream << "b1 b2 sub abs 255 mul /db exch def\n";
		m_stream << "dr dg add db add /colsteps exch def\n";
		m_stream << "colsteps 0.0 eq { /colsteps 1 def } if\n";
		m_stream << "rad2 rad1 sub colsteps div /radinc exch def\n";
		m_stream << "r1 r2 sub colsteps div /rinc exch def\n";
		m_stream << "g1 g2 sub colsteps div /ginc exch def\n";
		m_stream << "b1 b2 sub colsteps div /binc exch def\n";
		m_stream << "0 1 colsteps\n";
		m_stream << "{\n";
		m_stream << "    r2 g2 b2 setrgbcolor\n";
		m_stream << "    circx circy rad1 0 360 arc closepath fill\n"; 
		m_stream << "    rad1 radinc add /rad1 exch def\n";
		m_stream << "    r2 rinc add /r2 exch def\n";
		m_stream << "    g2 ginc add /g2 exch def\n";
		m_stream << "    b2 binc add /b2 exch def\n";
		m_stream << "} for\n";

		stop2 = stop1;
		ramp2 = ramp1;
		r2 = r1;
		g2 = g1;
		b2 = b1;
	}
}

void ScPainterEx_Ps2::drawCircularGradient_CMYK( VGradientEx& gradient, const QRect& rect )
{
	int c1, m1, y1, k1;
	int c2, m2, y2, k2;
	double ramp1, ramp2;
	double scale1, scale2;
	int clipBoxWidth, clipBoxHeight, maxDim;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx stop1( *colorStops[gradient.Stops() - 1] );
	VColorStopEx stop2( *colorStops[gradient.Stops() - 1] );
	CMYKColor cmyk;
	QColor color;

	clipBoxWidth = rect.width();
	clipBoxHeight = rect.height();
	maxDim = qRound(sqrt( (double) (clipBoxWidth * clipBoxWidth + clipBoxHeight * clipBoxHeight) )) * 2;
	if ( maxDim <= 0 ) return;

	scale1 = sqrt( m_matrix.m11() * m_matrix.m11() + m_matrix.m21() * m_matrix.m21() );
	scale2 = sqrt( m_matrix.m12() * m_matrix.m12() + m_matrix.m22() * m_matrix.m22() );

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
	double rad = sqrt( pow(pv.x() -pc.x(), 2) + pow(pv.y() - pc.y(), 2) );

	ramp2 = stop2.rampPoint;
	ScColorEngine::getShadeColorCMYK(stop2.color, m_options.document, cmyk, stop2.shade);
	cmyk.getValues(c2, m2, y2, k2);
	m_stream << QString("/DeviceCMYK setcolorspace\n");
	m_stream << QString("%0 %1 %2 %3 setcolor fill\n").arg(c2).arg(m2).arg(y2).arg(k2);
	if ( gradient.Stops() < 2 ) return;

	for( int index = gradient.Stops() - 2; index >= 0; index--)
	{
		stop1 = *colorStops[index];
		ramp1 = stop1.rampPoint;
		ScColorEngine::getShadeColorCMYK(stop1.color, m_options.document, cmyk, stop1.shade);
		cmyk.getValues(c1, m1, y1, k1);
		int radMax = qRound(rad * ramp2);
		int radMin = qRound(rad * ramp1);
		
		m_stream << QString("/rad1 %1 def\n").arg(radMax);
		m_stream << QString("/rad2 %1 def\n").arg(radMin);
		m_stream << QString("/circx %1 def\n").arg(cx);
		m_stream << QString("/circy %1 def\n").arg(cy);
		m_stream << QString("/c1 %1 def\n").arg(c1/255.0);
		m_stream << QString("/m1 %1 def\n").arg(m1/255.0);
		m_stream << QString("/y1 %1 def\n").arg(y1/255.0);
		m_stream << QString("/k1 %1 def\n").arg(k1/255.0);
		m_stream << QString("/c2 %1 def\n").arg(c2/255.0);
		m_stream << QString("/m2 %1 def\n").arg(m2/255.0);
		m_stream << QString("/y2 %1 def\n").arg(y2/255.0);
		m_stream << QString("/k2 %1 def\n").arg(k2/255.0);
		m_stream << "c1 c2 sub abs 255 mul /dc exch def\n";
		m_stream << "m1 m2 sub abs 255 mul /dm exch def\n";
		m_stream << "y1 y2 sub abs 255 mul /dy exch def\n";
		m_stream << "k1 k2 sub abs 255 mul /dk exch def\n";
		m_stream << "dc dm add dy add dk add /colsteps exch def\n";
		m_stream << "colsteps 0.0 eq { /colsteps 1 def } if\n";
		m_stream << "rad2 rad1 sub colsteps div /radinc exch def\n";
		m_stream << "c1 c2 sub colsteps div /cinc exch def\n";
		m_stream << "m1 m2 sub colsteps div /minc exch def\n";
		m_stream << "y1 y2 sub colsteps div /yinc exch def\n";
		m_stream << "k1 k2 sub colsteps div /kinc exch def\n";
		m_stream << "0 1 colsteps\n";
		m_stream << "{\n";
		m_stream << "    c2 m2 y2 k2 setcolor\n";
		m_stream << "    circx circy rad1 0 360 arc closepath fill\n"; 
		m_stream << "    rad1 radinc add /rad1 exch def\n";
		m_stream << "    c2 cinc add /c2 exch def\n";
		m_stream << "    m2 minc add /m2 exch def\n";
		m_stream << "    y2 yinc add /y2 exch def\n";
		m_stream << "    k2 kinc add /k2 exch def\n";
		m_stream << "} for\n";

		stop2 = stop1;
		ramp2 = ramp1;
		c2 = c1;
		m2 = m1;
		y2 = y1;
		k2 = k1;
	}
}

void ScPainterEx_Ps2::getPathBoundingBox( FPointArray* points, QRect& r )
{
	FPoint point;
	double bottom = DBL_MAX, top   = DBL_MIN;
	double left   = DBL_MAX, right = DBL_MIN;

	r.setCoords(0, 0, 0, 0);

	for( uint i = 0; i < points->size(); i++ )
	{
		point = points->point(i);
		if( point.x() > 900000 )
			continue;
		transformPoint(point, point);
		if( point.x() < left ) left = point.x();
		if( point.x() > right ) right = point.x();
		if( point.y() < bottom ) bottom = point.y();
		if( point.y() > top ) top = point.y();
	}
	r.setCoords( qRound(left), qRound(bottom), qRound(right), qRound(top) );
}

