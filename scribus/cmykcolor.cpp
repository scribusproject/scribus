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

#include <qcolor.h>
#include <qstring.h>

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
	extern cmsHTRANSFORM stdTransG;
	extern cmsHTRANSFORM stdProofG;
	extern cmsHTRANSFORM stdTransCMYKG;
	extern cmsHTRANSFORM stdProofCMYKG;
	extern cmsHTRANSFORM stdTransRGBG;
	extern bool SoftProofing;
	extern bool CMSuse;
#endif
bool CMYKColor::UseProf = true;

CMYKColor::CMYKColor()
{
	setColor(0, 0, 0, 0);
	Spot = false;
}

CMYKColor::CMYKColor(int c, int m, int y, int k)
{
	setColor(c, m, y, k);
	Spot = false;
}
 
CMYKColor::CMYKColor(int r, int g, int b)
{
	setColorRGB(r, g, b);
	Spot = false;
}

void CMYKColor::setColor(int c, int m, int y, int k)
{
	C = c;
	M = m;
	Y = y;
	K = k;
	Model = colorModelCMYK;
	RecalcRGB();
}

void CMYKColor::setColorRGB(int r, int g, int b)
{
	R = r;
	G = g;
	B = b;
	Model = colorModelRGB;
	RecalcRGB();
}

void CMYKColor::setColorModel (colorModel cm)
{
	Model = cm;
	RecalcRGB();
}

colorModel CMYKColor::getColorModel ()
{
	return Model;
 }

void CMYKColor::fromQColor(QColor color)
{
	int r, g, b;
	color.rgb(&r, &g, &b);
	setColorRGB(r, g, b);
}

QColor CMYKColor::getRGBColor()
{
	return RGB;
}
 
void CMYKColor::getShadeColorCMYK(int *c, int *m, int *y, int *k, int level)
{
	if (Model == colorModelRGB)
	{
		int r, g, b;
		CMYKColor tmpR;
		getShadeColorRGB(&r, &g, &b, level);
		tmpR.setColorRGB(r, g, b);
		tmpR.getCMYK(c, m, y, k);
	}
	else
	{
		*c = C * level / 100;
		*m = M * level / 100;
		*y = Y * level / 100;
		*k = K * level / 100;
	}
}

void CMYKColor::getShadeColorRGB(int *r, int *g, int *b, int level)
{
	int h, s, v, snew;
	
	if (Model == colorModelCMYK)
	{
		int c, m, y, k;
		getShadeColorCMYK(&c, &m, &y, &k, level);
		CMYKColor tmpC(c, m, y, k);
		tmpC.getRGB(r, g, b);
	}
	else
	{
		QColor tmpR(R, G, B);
		tmpR.hsv(&h, &s, &v);
		
		if (R == G && G == B)
		{
			snew = 255 - ((255 - v) * level / 100);
			tmpR.setHsv(h, s, snew);
		}
		else
		{
			snew = s * level / 100;
			tmpR.setHsv(h, snew, v);
		}

		tmpR.getRgb(r, g, b);
	}
}

QColor CMYKColor::getShadeColorProof(int level)
{
	QColor tmp;
	CMYKColor tmp2;
	int r, g, b, c, m ,y, k;
	
	if (Model == colorModelRGB)
	{
		getShadeColorRGB(&r, &g, &b, level);
		tmp2.setColorRGB(r, g, b);
	}
	else
	{
		getShadeColorCMYK(&c, &m, &y, &k, level);
		tmp2.setColor(c, m, y, k);
	}
	
	return tmp2.getRGBColor();
}

void CMYKColor::getRawRGBColor(int *r, int *g, int *b)
{
	*r = 255-QMIN(255, C+K);
	*g = 255-QMIN(255, M+K);
	*b = 255-QMIN(255, Y+K);
}

void CMYKColor::getRGB(int *r, int *g, int *b)
{
	*r = R;
	*g = G;
	*b = B;
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
#ifdef HAVE_CMS
	if (!(CMSuse && UseProf))
	{
#endif
		int k = QMIN(QMIN(C, M), Y);
		C = C - k;
		M = M - k;
		Y = Y - k;
		K = QMIN((K + k), 255);
#ifdef HAVE_CMS
	}
#endif
}

QString CMYKColor::name()
{
	QString tmp, name="#";
	switch (Model) 
	{
	case colorModelCMYK:
		tmp.setNum(C, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(M, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(Y, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(K, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		return name;
		break;
	case colorModelRGB:
		tmp.setNum(R, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(G, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(B, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		return name;
	}
	return "";
}

void CMYKColor::setNamedColor(QString name)
{
	bool ok;
	if (name.length () == 9)
	{
		int c = name.mid(1,2).toInt(&ok, 16);
		int m = name.mid(3,2).toInt(&ok, 16);
		int y = name.mid(5,2).toInt(&ok, 16);
		int k = name.mid(7,2).toInt(&ok, 16);
		setColor(c, m, y, k);
	}
	else if (name.length () == 7)
	{
		int r = name.mid(1,2).toInt(&ok, 16);
		int g = name.mid(3,2).toInt(&ok, 16);
		int b = name.mid(5,2).toInt(&ok, 16);
		setColorRGB(r, g, b);
	}
}

void CMYKColor::RecalcRGB()
{
#ifdef HAVE_CMS
	WORD inC[4];
	WORD outC[4];
#endif
	if (Model == colorModelRGB)
	{
		// allow RGB greys to go got to CMYK greys without transform
		if (R == G && G == B)
		{
			C = M = Y = 0;
			K = 255 - R;
		}
		else
		{
#ifdef HAVE_CMS
			if (CMSuse && UseProf)
			{
				inC[0] = R * 257;
				inC[1] = G * 257;
				inC[2] = B * 257;
				cmsDoTransform(stdTransCMYKG, inC, outC, 1);
				C = outC[0] / 257;
				M = outC[1] / 257;
				Y = outC[2] / 257;
				K = outC[3] / 257;
			}
			else
#endif
			{
				int c = 255 - R;
				int m = 255 - G;
				int y = 255 - B;
				int k = QMIN(QMIN(c, m), y);
				C = c - k;
				M = m - k;
				Y = y - k;
				K = k;
			}
		}
	}
	else if (Model == colorModelCMYK)
 	{
#ifdef HAVE_CMS
		if (CMSuse && UseProf)
		{
			inC[0] = C * 257;
			inC[1] = M * 257;
			inC[2] = Y * 257;
			inC[3] = K * 257;
			cmsDoTransform(stdTransRGBG, inC, outC, 1);
			R = outC[0] / 257;
			G = outC[1] / 257;
			B = outC[2] / 257;
		}
		else
#endif
		{
			R = 255 - QMIN(255, C + K);
			G = 255 - QMIN(255, M + K);
			B = 255 - QMIN(255, Y + K);
		}
	}
#ifdef HAVE_CMS
	if (CMSuse && UseProf && SoftProofing)
	{
		cmsHTRANSFORM xformProof;
		if (Model == colorModelRGB)
		{
			inC[0] = R*257;
			inC[1] = G*257;
			inC[2] = B*257;
			xformProof = stdProofG;
		}
		else
		{
			inC[0] = C*257;
			inC[1] = M*257;
			inC[2] = Y*257;
			inC[3] = K*257;
			xformProof = stdProofCMYKG;
		}
		cmsDoTransform(xformProof, inC, outC, 1);
		RGB = QColor(outC[0]/257, outC[1]/257, outC[2]/257);
	}
	else
#endif
	{
		RGB = QColor(R, G, B);
	}
}
