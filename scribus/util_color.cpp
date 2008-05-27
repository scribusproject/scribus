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

#include "util_color.h"

#include <QBitmap>
#include <QDateTime> 
#include <QPainter>
#include <QCheckBox>
#include <QPixmap>
#include <QTextStream>
#include <QDomDocument>

#include "scconfig.h"

#include "scribusdoc.h"
#include "scpixmapcache.h"
#include "commonstrings.h"
#include "fileloader.h"
#include "plugins/formatidlist.h"
#include "util_icon.h"
#include "util.h"
#include "util_formats.h"
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
	if( colName.isEmpty() || colName == CommonStrings::None || !doc->PageColors.contains(colName))
		return;

	const ScColor& scCol1(doc->PageColors[colName]);
	
	if( (shade == 100) || (scCol1.getColorModel() != colorModelRGB) )
		return;
	scCol1.getRGB(&r, &g, &b);
	QColor col1 = getOldColorShade(r, g, b, shade), col2;
	ColorList::Iterator it, itEnd = doc->PageColors.end();
	for( it = doc->PageColors.begin(); it != itEnd; it++)
	{
		if ( it.value().getColorModel() == colorModelRGB )
		{
			it.value().getRGB(&r, &g, &b);
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

QColor getOldColorShade(uchar red, uchar green, uchar blue, int shade)
{
	int h, s, v, snew;
	QColor color(red, green, blue);
	color.getHsv(&h, &s, &v);
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

static quint64 code64(const ScColor & col) {
	int C, M, Y, K, R, G, B;
	quint64 result=0;
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
	static ScPixmapCache<quint64> pxCache;

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

	quint64 res=code64(col);
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
//	if (useMask)
//	{
//		if (target.mask().isNull())
//			target.setMask(QBitmap(target.width(), target.height(), useMask));
//	}
	QPainter p;
	p.begin(&target);
	p.drawPixmap(x, y, toPaint);
	if (useMask)
	{
// Qt4 FIXME: Qt4 can use better alpha setting. see colorlistbox.cpp
// 		QPainter alpha; // transparency handling
// 		alpha.begin(target.mask());
// 		alpha.setBrush(Qt::color1);
// 		alpha.setPen(Qt::color1);
// 		alpha.drawRect(x, y, 15, 15);
// 		if (!toPaint.mask().isNull())
// 			alpha.drawPixmap(x, y, toPaint.mask());
// 		alpha.end();
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
		max = qMax( r, b );
		min = qMin( g, b );
	}
	else
	{
		max = qMax( g, b );
		min = qMin( r, b );
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
	double var_R = ( red / 255.0 );
	double var_G = ( green / 255.0 );
	double var_B = ( blue / 255.0 );
	double var_Min = qMin( var_R, qMin(var_G, var_B) );    //Min. value of RGB
	double var_Max = qMax( var_R, qMax(var_G, var_B) );    //Max. value of RGB
	double del_Max = var_Max - var_Min;             //Delta RGB value
	double L = ( var_Max + var_Min ) / 2.0;
	double H = 0;
	double S = 0;
	double del_R = 0;
	double del_G = 0;
	double del_B = 0;
	if ( del_Max == 0 )
	{
		H = 0;
		S = 0;
	}
	else
	{
		if ( L < 0.5 )
			S = del_Max / ( var_Max + var_Min );
		else
			S = del_Max / ( 2 - var_Max - var_Min );
		del_R = ( ( ( var_Max - var_R ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
		del_G = ( ( ( var_Max - var_G ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
		del_B = ( ( ( var_Max - var_B ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
		if ( var_R == var_Max )
			H = del_B - del_G;
	   else if ( var_G == var_Max )
			H = ( 1.0 / 3.0 ) + del_R - del_B;
		else if ( var_B == var_Max )
			H = ( 2.0 / 3.0 ) + del_G - del_R;
		if ( H < 0 )
			H += 1;
		if ( H > 1 )
			H -= 1;
	}
	red = qRound(H * 255);
	green = qRound(L * 255);
	blue = qRound(S * 255);
}

double HLSVALUE ( double n1, double n2, double hue )
{
	if ( hue < 0 )
		hue += 1;
	if ( hue > 1 )
		hue -= 1;
	if ( ( 6 * hue ) < 1 )
		return n1 + ( n2 - n1 ) * 6 * hue;
	if ( ( 2 * hue ) < 1 )
		return n2;
	if ( ( 3 * hue ) < 2 )
		return n1 + ( n2 - n1 ) * ( ( 2.0 / 3.0 ) - hue ) * 6;
	return n1;
}

void HLSTORGB ( uchar& hue, uchar& lightness, uchar& saturation )
{
	double H = ( hue / 255.0 );
	double L = ( lightness / 255.0 );
	double S = ( saturation / 255.0 );
	if (S == 0)
	{
		hue = qRound(255 * L);
		saturation = qRound(255 * L);
		return;
	}
	double var_1 = 0;
	double var_2 = 0;
	if ( L < 0.5 )
		var_2 = L * ( 1 + S );
	else
		var_2 = ( L + S ) - ( S * L );
	var_1 = 2 * L - var_2;
	hue = qRound(255 * HLSVALUE( var_1, var_2, H + ( 1.0 / 3.0 ) ));
	lightness = qRound(255 * HLSVALUE( var_1, var_2, H ));
	saturation = qRound(255 * HLSVALUE( var_1, var_2, H - ( 1.0 / 3.0 ) ));
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
	double n = qMin(red, qMin(green, blue));
	double x = qMax(red, qMax(green, blue));
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

QString colorSpaceText(int cs)
{
	if (cs==0)
		return CommonStrings::trRGB;
	if (cs==1)
		return CommonStrings::trCMYK;
	if (cs==2)
		return CommonStrings::trGrayscale;
	if (cs==3)
		return CommonStrings::trDuotone;
	return CommonStrings::trUnknownCS;
}

bool importColorsFromFile(QString fileName, ColorList &EditColors)
{
	int oldCount = EditColors.count();
	if (!fileName.isEmpty())
	{
		QFileInfo fi = QFileInfo(fileName);
		QString ext = fi.suffix().toLower();
		if (extensionIndicatesEPSorPS(ext) || (ext == "ai"))
		{
			QString tmp, tmp2, FarNam;
			double c, m, y, k;
			ScColor cc;
			QFile f(fileName);
			if (f.open(QIODevice::ReadOnly))
			{
				bool isAtend = false;
				QDataStream ts(&f);
				while (!ts.atEnd())
				{
					tmp = readLinefromDataStream(ts);
					if ((tmp.startsWith("%%CMYKCustomColor")) || (tmp.startsWith("%%CMYKProcessColor")))
					{
						if (tmp.contains("(atend)"))
							isAtend = true;
						else
						{
							if (tmp.startsWith("%%CMYKCustomColor"))
								tmp = tmp.remove(0,18);
							else if (tmp.startsWith("%%CMYKProcessColor"))
								tmp = tmp.remove(0,19);
							QTextStream ts2(&tmp, QIODevice::ReadOnly);
							ts2 >> c >> m >> y >> k;
							FarNam = ts2.readAll();
							FarNam = FarNam.trimmed();
							FarNam = FarNam.remove(0,1);
							FarNam = FarNam.remove(FarNam.length()-1,1);
							FarNam = FarNam.simplified();
							cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * y), qRound(255 * k));
							cc.setSpotColor(true);
							if ((!EditColors.contains(FarNam)) && (!FarNam.isEmpty()))
								EditColors.insert(FarNam, cc);
							while (!ts.atEnd())
							{
								quint64 oldPos = ts.device()->pos();
								tmp = readLinefromDataStream(ts);
								if (!tmp.startsWith("%%+"))
								{
									ts.device()->seek(oldPos);
									break;
								}
								tmp = tmp.remove(0,3);
								QTextStream ts2(&tmp, QIODevice::ReadOnly);
								ts2 >> c >> m >> y >> k;
								FarNam = ts2.readAll();
								FarNam = FarNam.trimmed();
								FarNam = FarNam.remove(0,1);
								FarNam = FarNam.remove(FarNam.length()-1,1);
								FarNam = FarNam.simplified();
								cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * y), qRound(255 * k));
								cc.setSpotColor(true);
								if ((!EditColors.contains(FarNam)) && (!FarNam.isEmpty()))
									EditColors.insert(FarNam, cc);
							}
						}
					}
					if (tmp.startsWith("%%RGBCustomColor"))
					{
						if (tmp.contains("(atend)"))
							isAtend = true;
						else
						{
							tmp = tmp.remove(0,17);
							QTextStream ts2(&tmp, QIODevice::ReadOnly);
							ts2 >> c >> m >> y;
							FarNam = ts2.readAll();
							FarNam = FarNam.trimmed();
							FarNam = FarNam.remove(0,1);
							FarNam = FarNam.remove(FarNam.length()-1,1);
							FarNam = FarNam.simplified();
							cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * y));
							if ((!EditColors.contains(FarNam)) && (!FarNam.isEmpty()))
								EditColors.insert(FarNam, cc);
							while (!ts.atEnd())
							{
								quint64 oldPos = ts.device()->pos();
								tmp = readLinefromDataStream(ts);
								if (!tmp.startsWith("%%+"))
								{
									ts.device()->seek(oldPos);
									break;
								}
								tmp = tmp.remove(0,3);
								QTextStream ts2(&tmp, QIODevice::ReadOnly);
								ts2 >> c >> m >> y;
								FarNam = ts2.readAll();
								FarNam = FarNam.trimmed();
								FarNam = FarNam.remove(0,1);
								FarNam = FarNam.remove(FarNam.length()-1,1);
								FarNam = FarNam.simplified();
								cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * y));
								if ((!EditColors.contains(FarNam)) && (!FarNam.isEmpty()))
									EditColors.insert(FarNam, cc);
							}
						}
					}
					if (tmp.startsWith("%%EndComments"))
					{
						if (ext == "ai")
						{
							while (!ts.atEnd())
							{
								bool isX = false;
								tmp = readLinefromDataStream(ts);
								if ((tmp.endsWith("Xa") || tmp.endsWith(" k") || tmp.endsWith(" x")) && (tmp.length() > 4))
								{
									QTextStream ts2(&tmp, QIODevice::ReadOnly);
									ts2 >> c >> m >> y >> k;
									if (tmp.endsWith(" x"))
									{
										isX = true;
										int an = tmp.indexOf("(");
										int en = tmp.lastIndexOf(")");
										FarNam = tmp.mid(an+1, en-an-1);
										FarNam = FarNam.simplified();
									}
									tmp = readLinefromDataStream(ts);
									if (tmp.endsWith("Pc"))
									{
										if (!isX)
										{
											tmp = tmp.trimmed();
											tmp = tmp.remove(0,1);
											int en = tmp.indexOf(")");
											FarNam = tmp.mid(0, en);
											FarNam = FarNam.simplified();
										}
										cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * y), qRound(255 * k));
										cc.setSpotColor(true);
										if (!EditColors.contains(FarNam))
											EditColors.insert(FarNam, cc);
									}
								}
							}
						}
						if (!isAtend)
							break;
					}
				}
				f.close();
			}
		}
		else
		{
			FileLoader fl(fileName);
			int test = fl.TestFile();
			if ((test == FORMATID_SLA134IMPORT) || (test == FORMATID_SLA13XIMPORT) || (test == FORMATID_SLA12XIMPORT))
			{
				ColorList LColors;
				if (fl.ReadColors(fileName, LColors))
				{
					ColorList::Iterator it;
					for (it = LColors.begin(); it != LColors.end(); ++it)
					{
						if (!EditColors.contains(it.key()))
							EditColors.insert(it.key(), it.value());
					}
				}
			}
			else
			{
				QFile fiC(fileName);
				if (fiC.open(QIODevice::ReadOnly))
				{
					QString ColorEn, Cname;
					int Rval, Gval, Bval, Kval;
					QTextStream tsC(&fiC);
					ColorEn = tsC.readLine();
					bool cus = false;
					if (ColorEn.contains("OpenOffice"))
						cus = true;
					if (ColorEn.startsWith("<?xml version="))
					{
						QByteArray docBytes("");
						loadRawText(fileName, docBytes);
						QString docText("");
						docText = QString::fromUtf8(docBytes);
						QDomDocument docu("scridoc");
						docu.setContent(docText);
						ScColor lf = ScColor();
						QDomElement elem = docu.documentElement();
						QDomNode PAGE = elem.firstChild();
						while(!PAGE.isNull())
						{
							QDomElement pg = PAGE.toElement();
							if(pg.tagName()=="COLOR" && pg.attribute("NAME")!=CommonStrings::None)
							{
								if (pg.hasAttribute("CMYK"))
									lf.setNamedColor(pg.attribute("CMYK"));
								else
									lf.fromQColor(QColor(pg.attribute("RGB")));
								if (pg.hasAttribute("Spot"))
									lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
								else
									lf.setSpotColor(false);
								if (pg.hasAttribute("Register"))
									lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
								else
									lf.setRegistrationColor(false);
								if (!EditColors.contains(pg.attribute("NAME")))
									EditColors.insert(pg.attribute("NAME"), lf);
							}
							else if (pg.tagName()=="draw:color" && pg.attribute("draw:name")!=CommonStrings::None)
							{
								if (pg.hasAttribute("draw:color"))
									lf.setNamedColor(pg.attribute("draw:color"));
								lf.setSpotColor(false);
								lf.setRegistrationColor(false);
								QString nam = pg.attribute("draw:name");
								if ((!EditColors.contains(nam)) && (!nam.isEmpty()))
									EditColors.insert(nam, lf);
							}
							PAGE=PAGE.nextSibling();
						}
					}
					else
					{
						while (!tsC.atEnd())
						{
							ScColor tmp;
							ColorEn = tsC.readLine();
							if (ColorEn.length()>0 && ColorEn[0]==QChar('#'))
								continue;
							QTextStream CoE(&ColorEn, QIODevice::ReadOnly);
							CoE >> Rval;
							CoE >> Gval;
							CoE >> Bval;
							if (cus)
							{
								CoE >> Kval;
								Cname = CoE.readAll().trimmed();
								tmp.setColor(Rval, Gval, Bval, Kval);
							}
							else
							{
								Cname = CoE.readAll().trimmed();
								tmp.setColorRGB(Rval, Gval, Bval);
							}
		
							if (Cname.length()==0)
							{
								if (!cus)
									Cname=QString("#%1%2%3").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).toUpper();
								else
									Cname=QString("#%1%2%3%4").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).arg(Kval,2,16).toUpper();
								Cname.replace(" ","0");
							}
							if (!EditColors.contains(Cname))
								EditColors.insert(Cname, tmp);
						}
					}
					fiC.close();
				}
			}
		}
	}
	return (EditColors.count() != oldCount);
}
