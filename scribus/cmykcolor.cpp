/***************************************************************************
                          cmykcolor.cpp  -  description
                             -------------------
    begin                : Sun Sep 9 2001
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

/* PFJ - 29.02.04 - Added iostream, iomanip and sstream for CMYK::name */
/*
#include <iostream>
#include <iomanip>
#include <sstream> */
#include "cmykcolor.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#ifdef HAVE_CMS
	#include CMS_INC
extern cmsHTRANSFORM stdTrans;
extern cmsHTRANSFORM stdProof;
extern bool SoftProofing;
extern bool CMSuse;
#endif

CMYKColor::CMYKColor(int c, int m, int y, int k)
{
	setColor(c, m, y, k);
	Spot = false;
}

void CMYKColor::setColor(int c, int m, int y, int k)
{
	C = c;
	M = m;
	Y = y;
	K = k;
	RecalcRGB();
}

void CMYKColor::setColorRGB(int r, int g, int b)
{
	int c = 255 - r;
	int m = 255 - g;
	int y = 255 - b;
	int k = QMIN(QMIN(c, m), y);
	C = c - k;
	M = m - k;
	Y = y - k;
	K = k;
	RGB = QColor(r, g, b);
}

void CMYKColor::fromQColor(QColor f)
{
	int r, g, b;
	f.rgb(&r, &g, &b);
	setColorRGB(r, g, b);
}

QColor CMYKColor::getRGBColor()
{
	return RGB;
}

void CMYKColor::getRawRGBColor(int *r, int *g, int *b)
{
	*r = 255-QMIN(255, C+K);
	*g = 255-QMIN(255,M+K);
	*b = 255-QMIN(255,Y+K);
}

void CMYKColor::getCMYK(int *c, int *m, int *y, int *k)
{
	*c = C;
	*m = M;
	*y = Y;
	*k = K;
}

void CMYKColor::applyGCR()
{
	int k = QMIN(QMIN(C, M), Y);
	C = C - k;
	M = M - k;
	Y = Y - k;
	K = QMIN((K + k), 255);
}

QString CMYKColor::name()
{
	/* PFJ - 29.02.04 - rewritten to use setw and setfill */
	// doesn't compile here, so i replaced it with the old code F.S.
/*	QString tmp2;
	std::ostringstream tmp;
	tmp << std::setw(2) << std::setfill('0') << C 
		<< std::setw(2) << M
		<< std::setw(2) << Y
		<< std::setw(2) << K;
	tmp2 = "#" + tmp.str();
	return tmp2;
	*/
	QString tmp, tmp2;
	tmp2 = "#";
	tmp.setNum(C, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	tmp2 += tmp;
	tmp.setNum(M, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	tmp2 += tmp;
	tmp.setNum(Y, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	tmp2 += tmp;
	tmp.setNum(K, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	tmp2 += tmp;
	return tmp2;
}

void CMYKColor::setNamedColor(QString nam)
{
	bool ok;
	int c = nam.mid(1,2).toInt(&ok, 16);
	int m = nam.mid(3,2).toInt(&ok, 16);
	int y = nam.mid(5,2).toInt(&ok, 16);
	int k = nam.mid(7,2).toInt(&ok, 16);
	setColor(c, m, y, k);
}

void CMYKColor::RecalcRGB()
{
	RGB = QColor(255-QMIN(255, C+K), 255-QMIN(255,M+K), 255-QMIN(255,Y+K));
	if ((C == M) && (M == Y))
		return;
#ifdef HAVE_CMS
	WORD inC[3];
	WORD outC[3];
	int r, g, b;
	RGB.rgb(&r, &g, &b);
	inC[0] = r*257;
	inC[1] = g*257;
	inC[2] = b*257;
	if ((CMSuse) && (SoftProofing))
	{
		cmsDoTransform(stdProof, inC, outC, 1);
		RGB = QColor(outC[0]/257, outC[1]/257, outC[2]/257);
	}
	else
	{
		if (CMSuse)
		{
			cmsDoTransform(stdTrans, inC, outC, 1);
			RGB = QColor(outC[0]/257, outC[1]/257, outC[2]/257);
		}
	}
#endif
}
