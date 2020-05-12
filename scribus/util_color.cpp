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

#include <QDomDocument>
#include <QPainter>

#include "commonstrings.h"
#include "fileloader.h"
#include "iconmanager.h"
#include "loadsaveplugin.h"
#include "palettes/paletteloader_adobe_acb.h"
#include "palettes/paletteloader_adobe_aco.h"
#include "palettes/paletteloader_adobe_ase.h"
#include "palettes/paletteloader_autocad_acb.h"
#include "palettes/paletteloader_cxf.h"
#include "palettes/paletteloader_ps.h"
#include "palettes/paletteloader_sk1.h"
#include "palettes/paletteloader_swatchbook.h"
#include "plugins/formatidlist.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scpixmapcache.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"


using namespace std;

void handleOldColorShade(ScribusDoc* doc, QString& colName, int& shade)
{
	int r, g, b;
	bool found = false;
	if (colName.isEmpty() || colName == CommonStrings::None || !doc->PageColors.contains(colName))
		return;

	const ScColor& scCol1(doc->PageColors[colName]);
	
	if ((shade == 100) || (scCol1.getColorModel() != colorModelRGB) )
		return;
	scCol1.getRGB(&r, &g, &b);
	QColor col1 = getOldColorShade(r, g, b, shade), col2;
	ColorList::Iterator it, itEnd = doc->PageColors.end();
	for (it = doc->PageColors.begin(); it != itEnd; it++)
	{
		if (it.value().getColorModel() == colorModelRGB)
		{
			it.value().getRGB(&r, &g, &b);
			col2.setRgb(r, g, b);
			if (col1 == col2)
			{
				found = true;
				break;
			}
		}
	}
	if (!found)
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
			uchar* ptr = out.scanLine(i);
			doc->stdProofImg.apply(ptr, ptr, out.width());
		}
	}
	else
	{
		if (cmsUse)
		{
			int outheight=out.height();
			for (int i=0; i < outheight; ++i)
			{
				uchar* ptr = out.scanLine(i);
				doc->stdTransImg.apply(ptr, ptr, out.width());
			}
		}
	}
	return out;
}

QColor SetColor(ScribusDoc *currentDoc, const QString& color, int shad)
{
	if (color == CommonStrings::None)
		return QColor(0, 0, 0, 0);
	const ScColor& col = currentDoc->PageColors[color];
	return ScColorEngine::getShadeColorProof(col, currentDoc, shad);
}


/**
 * QPixmaps are really slow with Qt/Mac 3.3.4. Really, *really*, slow.
 * So we better cache them.
 */
QPixmap * getSmallPixmap(const QColor& rgb)
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

QPixmap * getWidePixmap(const QColor& rgb)
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

static quint64 code64(const ScColor & col)
{
	int C = 0;
	int M = 0;
	int Y = 0;
	int K = 0;
	int R = 0;
	int G = 0;
	int B = 0;
	quint64 result = 0;
	if (col.getColorModel() == colorModelRGB)
	{
		col.getRGB( &R, &G, &B );
		QColor color = QColor(R, G, B);
		color.getCmyk(&C, &M, &Y, &K);
	}
	else if (col.getColorModel() == colorModelCMYK)
	{
		col.getCMYK( &C, &M, &Y, &K );
		R = 255-qMin(255, C + K);
		G = 255-qMin(255, M + K);
		B = 255-qMin(255, Y + K);
	}
	else
	{
		double L, a, b;
		col.getLab(&L, &a, &b);
		R = qRound(L);
		G = qRound(a + 128);
		B = qRound(b + 128);
	}
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

QPixmap * getFancyPixmap(const ScColor& col, ScribusDoc* doc)
{
	static ScPixmapCache<quint64> pxCache;

	static QPixmap alertIcon;
	static QPixmap cmykIcon;
	static QPixmap rgbIcon;
	static QPixmap labIcon;
	static QPixmap spotIcon;
	static QPixmap regIcon;
	static bool iconsInitialized = false;

	if ( !iconsInitialized )
	{
		IconManager& im=IconManager::instance();
		alertIcon = im.loadPixmap("alert.png");
		cmykIcon = im.loadPixmap("cmyk.png");
		rgbIcon = im.loadPixmap("rgb.png");
		labIcon = im.loadPixmap("lab.png");
		spotIcon = im.loadPixmap("spot.png");
		regIcon = im.loadPixmap("register.png");
		iconsInitialized = true;
	}

	quint64 res = code64(col);
	if (pxCache.contains(res))
		return pxCache[res];

	QPixmap *pa = new QPixmap(60, 15);
	QPixmap *pm = getSmallPixmap(ScColorEngine::getDisplayColor(col, doc));
//	QPixmap *pm=getSmallPixmap(col.getRawRGBColor());
	pa->fill(Qt::white);
	paintAlert(*pm, *pa, 0, 0);
	if (ScColorEngine::isOutOfGamut(col, doc))
		paintAlert(alertIcon, *pa, 15, 0);
	if (col.getColorModel() == colorModelCMYK)   // || (col.isSpotColor()))
		paintAlert(cmykIcon, *pa, 30, 0);
	else if (col.getColorModel() == colorModelRGB)
		paintAlert(rgbIcon, *pa, 30, 0);
	else if (col.getColorModel() == colorModelLab)
		paintAlert(labIcon, *pa, 30, 0);
	if (col.isSpotColor())
		paintAlert(spotIcon, *pa, 46, 2);
	if (col.isRegistrationColor())
		paintAlert(regIcon, *pa, 45, 0);
	pxCache.insert(res, pa);
	return pa;
}


void paintAlert(const QPixmap &toPaint, QPixmap &target, int x, int y, bool useMask)
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
		/* value   = value;*/
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
	if (curve.isEmpty())
		return 0.5;
	// First find curve segment
	p = curve.point(0);
	if (x < p.x())
		return p.y();
	p = curve.point(curve.size()-1);
	if (x >= p.x())
		return p.y();
	int cc = 0;
	// Find the four control points (two on each side of x)
	p = curve.point(0);
	while (x >= p.x())
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
	if (val < 0.0)
		val = 0.0;
	if (val > 1.0)
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
	if (cs==4)
		return CommonStrings::trMonochrome;
	return CommonStrings::trUnknownCS;
}

bool importColorsFromFile(const QString& fileName, ColorList &EditColors, QHash<QString, VGradient> *dialogGradients, bool merge)
{
	if (fileName.isEmpty())
		return false;
	int oldCount = EditColors.count();

	QFileInfo fi = QFileInfo(fileName);
	QString ext = fi.suffix().toLower();
	if (extensionIndicatesEPSorPS(ext))
	{
		PaletteLoader_PS psPalLoader;
		psPalLoader.setupTargets(&EditColors, dialogGradients);
		return psPalLoader.importFile(fileName, merge);
	}


	QStringList allFormatsV = LoadSavePlugin::getExtensionsForColors();
	if (allFormatsV.contains(ext))
	{
		FileLoader fl(fileName);
		int testResult = fl.testFile();
		if (testResult != -1)
		{
			ColorList LColors;
			if (fl.readColors(LColors))
			{
				ColorList::Iterator it;
				for (it = LColors.begin(); it != LColors.end(); ++it)
				{
					EditColors.tryAddColor(it.key(), it.value());
				}
				return (EditColors.count() != oldCount);
			}
		}
	}
	if (ext == "acb")			// Adobe color book format
	{
		PaletteLoader_Adobe_acb adobePalLoader;
		if (adobePalLoader.isFileSupported(fileName))
		{
			adobePalLoader.setupTargets(&EditColors, dialogGradients);
			return adobePalLoader.importFile(fileName, merge);
		}

		PaletteLoader_Autocad_acb autocadPalLoder;
		if (autocadPalLoder.isFileSupported(fileName))
		{
			autocadPalLoder.setupTargets(&EditColors, dialogGradients);
			return autocadPalLoder.importFile(fileName, merge);
		}
		return false;
	}
	if (ext == "aco")			// Adobe color swatch format
	{
		PaletteLoader_Adobe_aco adobePalLoader;
		if (adobePalLoader.isFileSupported(fileName))
		{
			adobePalLoader.setupTargets(&EditColors, dialogGradients);
			return adobePalLoader.importFile(fileName, merge);
		}
		return false;
	}
	if (ext == "ase")			// Adobe swatch exchange format
	{
		PaletteLoader_Adobe_ase adobePalLoader;
		if (adobePalLoader.isFileSupported(fileName))
		{
			adobePalLoader.setupTargets(&EditColors, dialogGradients);
			return adobePalLoader.importFile(fileName, merge);
		}
		return false;
	}
	if (ext == "cxf")			// Adobe swatch exchange format
	{
		PaletteLoader_CxF cxfLoader;
		if (cxfLoader.isFileSupported(fileName))
		{
			cxfLoader.setupTargets(&EditColors, dialogGradients);
			return cxfLoader.importFile(fileName, merge);
		}
		return false;
	}
	if (ext == "skp")			// Sk1 palette
	{
		PaletteLoader_sK1 sk1PalLoader;
		if (sk1PalLoader.isFileSupported(fileName))
		{
			sk1PalLoader.setupTargets(&EditColors, dialogGradients);
			return sk1PalLoader.importFile(fileName, merge);
		}
		return false;
	}
	if (ext == "sbz")
	{
		PaletteLoader_Swatchbook swatchbookLoader;
		if (swatchbookLoader.isFileSupported(fileName))
		{
			swatchbookLoader.setupTargets(&EditColors, dialogGradients);
			return swatchbookLoader.importFile(fileName, merge);
		}
		return false;
	}

	QFile fiC(fileName);
	if (fiC.open(QIODevice::ReadOnly))
	{
		QString ColorEn, Cname;
		int Rval, Gval, Bval, Kval;
		ScTextStream tsC(&fiC);
		ColorEn = tsC.readLine();
		bool cus = false;
		if (ColorEn.contains("OpenOffice"))
			cus = true;
		if ((ColorEn.startsWith("<?xml version=")) || (ColorEn.contains("VivaColors")))
		{
			QByteArray docBytes("");
			loadRawText(fileName, docBytes);
			QString docText(QString::fromUtf8(docBytes));
			QDomDocument docu("scridoc");
			docu.setContent(docText);
			ScColor lf = ScColor();
			QDomElement elem = docu.documentElement();
			QString dTag(elem.tagName());
			QString nameMask = "%1";
			nameMask = elem.attribute("mask", "%1");
			QDomNode PAGE = elem.firstChild();
			while (!PAGE.isNull())
			{
				QDomElement pg = PAGE.toElement();
				if (pg.tagName()=="COLOR" && pg.attribute("NAME")!=CommonStrings::None)
				{
					if (pg.hasAttribute("SPACE"))
					{
						QString space = pg.attribute("SPACE");
						if (space == "CMYK")
						{
							double c = pg.attribute("C", "0").toDouble() / 100.0;
							double m = pg.attribute("M", "0").toDouble() / 100.0;
							double y = pg.attribute("Y", "0").toDouble() / 100.0;
							double k = pg.attribute("K", "0").toDouble() / 100.0;
							lf.setCmykColorF(c, m, y, k);
						}
						else if (space == "RGB")
						{
							double r = pg.attribute("R", "0").toDouble() / 255.0;
							double g = pg.attribute("G", "0").toDouble() / 255.0;
							double b = pg.attribute("B", "0").toDouble() / 255.0;
							lf.setRgbColorF(r, g, b);
						}
						else if (space == "Lab")
						{
							double L = pg.attribute("L", "0").toDouble();
							double a = pg.attribute("A", "0").toDouble();
							double b = pg.attribute("B", "0").toDouble();
							lf.setLabColor(L, a, b);
						}
					}
					else if (pg.hasAttribute("CMYK"))
						lf.setNamedColor(pg.attribute("CMYK"));
					else if (pg.hasAttribute("RGB"))
						lf.fromQColor(QColor(pg.attribute("RGB")));
					else
					{
						double L = pg.attribute("L", "0").toDouble();
						double a = pg.attribute("A", "0").toDouble();
						double b = pg.attribute("B", "0").toDouble();
						lf.setLabColor(L, a, b);
					}
					if (pg.hasAttribute("Spot"))
						lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
					else
						lf.setSpotColor(false);
					if (pg.hasAttribute("Register"))
						lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
					else
						lf.setRegistrationColor(false);
					EditColors.tryAddColor(pg.attribute("NAME"), lf);
				}
				else if (pg.tagName() == "Gradient")
				{
					if (dialogGradients != nullptr)
					{
						VGradient gra = VGradient(VGradient::linear);
						gra.clearStops();
						QDomNode grad = pg.firstChild();
						while (!grad.isNull())
						{
							QDomElement stop = grad.toElement();
							QString name = stop.attribute("NAME");
							double ramp  = ScCLocale::toDoubleC(stop.attribute("RAMP"), 0.0);
							int shade    = stop.attribute("SHADE", "100").toInt();
							double opa   = ScCLocale::toDoubleC(stop.attribute("TRANS"), 1.0);
							QColor color;
							if (name == CommonStrings::None)
								color = QColor(255, 255, 255, 0);
							else
							{
								const ScColor& col = EditColors[name];
								color = ScColorEngine::getShadeColorProof(col, nullptr, shade);
							}
							gra.addStop(color, ramp, 0.5, opa, name, shade);
							grad = grad.nextSibling();
						}
						if ((!dialogGradients->contains(pg.attribute("Name"))) || (merge))
							dialogGradients->insert(pg.attribute("Name"), gra);
						else
						{
							QString tmp;
							QString name = pg.attribute("Name");
							name += "("+tmp.setNum(dialogGradients->count())+")";
							dialogGradients->insert(name, gra);
						}
					}
				}
				else if (pg.tagName()=="draw:color" && pg.attribute("draw:name")!=CommonStrings::None)
				{
					if (pg.hasAttribute("draw:color"))
						lf.setNamedColor(pg.attribute("draw:color"));
					lf.setSpotColor(false);
					lf.setRegistrationColor(false);
					QString nam = pg.attribute("draw:name");
					if (!nam.isEmpty())
						EditColors.tryAddColor(nam, lf);
				}
				else if (dTag == "VivaColors")
				{
					int cVal = 0;
					int mVal = 0;
					int yVal = 0;
					int kVal = 0;
					QString nam = nameMask.arg(pg.attribute("name"));
					if (pg.attribute("type") == "cmyk")
					{
						QDomNode colNode = pg.firstChild();
						while (!colNode.isNull())
						{
							QDomElement colVal = colNode.toElement();
							if (colVal.tagName() == "cyan")
								cVal = colVal.text().toInt();
							if (colVal.tagName() == "magenta")
								mVal = colVal.text().toInt();
							if (colVal.tagName() == "yellow")
								yVal = colVal.text().toInt();
							if (colVal.tagName() == "key")
								kVal = colVal.text().toInt();
							colNode = colNode.nextSibling();
						}
						lf.setColorF(cVal / 100.0, mVal / 100.0, yVal / 100.0, kVal / 100.0);
						lf.setSpotColor(false);
						lf.setRegistrationColor(false);
						if (!nam.isEmpty())
							EditColors.tryAddColor(nam, lf);
					}
					else if (pg.attribute("type") == "rgb")
					{
						QDomNode colNode = pg.firstChild();
						while (!colNode.isNull())
						{
							QDomElement colVal = colNode.toElement();
							if (colVal.tagName() == "red")
								cVal = colVal.text().toInt();
							if (colVal.tagName() == "green")
								mVal = colVal.text().toInt();
							if (colVal.tagName() == "blue")
								yVal = colVal.text().toInt();
							colNode = colNode.nextSibling();
						}
						lf.setRgbColor(cVal, mVal, yVal);
						lf.setSpotColor(false);
						lf.setRegistrationColor(false);
						if (!nam.isEmpty())
							EditColors.tryAddColor(nam, lf);
					}
				}
				PAGE=PAGE.nextSibling();
			}
		}
		else
		{
			QString paletteName = "";
			QString dummy;
			if (ColorEn.startsWith("GIMP Palette"))
			{
				ColorEn = tsC.readLine();
				ScTextStream CoE(&ColorEn, QIODevice::ReadOnly);
				CoE >> dummy >> paletteName;
			}
			while (!tsC.atEnd())
			{
				ScColor tmp;
				ColorEn = tsC.readLine();
				if (ColorEn.length()>0 && ColorEn[0]==QChar('#'))
					continue;
				ScTextStream CoE(&ColorEn, QIODevice::ReadOnly);
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
					tmp.setRgbColor(Rval, Gval, Bval);
				}
				if (Cname == "Untitled")
					Cname = "";
				if (Cname.length() == 0)
				{
					if (!cus)
						Cname = paletteName + QString("#%1%2%3").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).toUpper();
					else
						Cname = paletteName + QString("#%1%2%3%4").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).arg(Kval,2,16).toUpper();
					Cname.replace(" ","0");
				}
				EditColors.tryAddColor(Cname, tmp);
			}
		}
		fiC.close();
	}


	return (EditColors.count() != oldCount);
}
