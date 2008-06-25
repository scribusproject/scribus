/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          sccolor.cpp  -  description
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

#include "sccolor.h"

#include "scconfig.h"

#ifdef HAVE_CMS
	#include CMS_INC
	extern cmsHTRANSFORM stdTransRGBDoc2CMYKG;
	extern cmsHTRANSFORM stdTransCMYK2RGBDocG;
	extern cmsHTRANSFORM stdTransCMYK2MonG;
	extern cmsHTRANSFORM stdTransRGBDoc2MonG;
	extern cmsHTRANSFORM stdProofRGBG;
	extern cmsHTRANSFORM stdProofCMYKG;
	extern cmsHTRANSFORM stdProofRGBGCG;
	extern cmsHTRANSFORM stdProofCMYKGCG;
	extern bool Gamut;
	extern bool SoftProofing;
	extern bool CMSuse;
#endif
extern bool CMSavail;
bool ScColor::UseProf = true;

ScColor::ScColor()
{
//	setColor(0, 0, 0, 0);
	Model = colorModelCMYK;
	C = M = Y = K = 0;
	R = G = B = 255;
	RGB = Qt::white;

	Spot = false;
	outOfGamutFlag = false;
	Regist = false;
}

ScColor::ScColor(int c, int m, int y, int k)
{
	setColor(c, m, y, k);
	Spot = false;
	outOfGamutFlag = false;
	Regist = false;
}
 
ScColor::ScColor(int r, int g, int b)
{
	setColorRGB(r, g, b);
	Spot = false;
	outOfGamutFlag = false;
	Regist = false;
}

bool ScColor::operator==(const ScColor& rhs) const
{
	if (Model!=rhs.Model)
		return false;
	if (Spot!=rhs.Spot)
		return false;
	if (Regist!=rhs.Regist)
		return false;
	if (Model==colorModelRGB)
	{
		return (R==rhs.R && G==rhs.G && B==rhs.B);
	}
	if (Model==colorModelCMYK)
	{
		return (C==rhs.C && M==rhs.M && Y==rhs.Y && K==rhs.K);
	}
	return false;
}

void ScColor::setColor(int c, int m, int y, int k)
{
	C = c;
	M = m;
	Y = y;
	K = k;
	Model = colorModelCMYK;
	RecalcRGB();
}

void ScColor::setColorRGB(int r, int g, int b)
{
	R = r;
	G = g;
	B = b;
	Model = colorModelRGB;
	RecalcRGB();
}

void ScColor::setColorModel (colorModel cm)
{
	Model = cm;
	RecalcRGB();
}

colorModel ScColor::getColorModel ()
{
	return Model;
 }

void ScColor::fromQColor(QColor color)
{
	int r, g, b;
	color.rgb(&r, &g, &b);
	setColorRGB(r, g, b);
}

QColor ScColor::getRGBColor()
{
	return RGB;
}

QColor ScColor::getDisplayColor() const
{
	if (Model == colorModelRGB)
		return getDisplayColor(R, G, B);
	return getDisplayColor(C, M, Y, K);
}

QColor ScColor::getDisplayColor(int level) const
{
	QColor tmp;
	if (Model == colorModelRGB)
	{
		int r, g, b;
		getShadeColorRGB(&r, &g, &b, level);
		tmp = getDisplayColor(r, g, b);
	}
	else
	{
		int c, m, y, k;
		getShadeColorCMYK(&c, &m, &y, &k, level);
		tmp = getDisplayColor(c, m, y, k);
	}
	return tmp;
}

QColor ScColor::getDisplayColorGC()
{
	QColor tmp;
	if( SoftProofing && Gamut )
	{
		checkGamut();
		tmp = outOfGamutFlag ? QColor(0, 255, 0) : getDisplayColor();
	}
	else
		tmp = getDisplayColor();
	return tmp;
}

QColor ScColor::getDisplayColor(int r, int g, int b) const
{
#ifdef HAVE_CMS
	WORD inC[4];
	WORD outC[4];
	if (CMSavail && stdTransRGBDoc2MonG)
	{
		inC[0] = r * 257;
		inC[1] = g * 257;
		inC[2] = b * 257;
		cmsDoTransform(stdTransRGBDoc2MonG, inC, outC, 1);
		r = outC[0] / 257;
		g = outC[1] / 257;
		b = outC[2] / 257;
	}
#endif
	return QColor(r, g, b);
}

QColor ScColor::getDisplayColor(int c, int m, int y, int k) const
{
	int  r = 0, g = 0, b = 0;
#ifdef HAVE_CMS
	WORD inC[4];
	WORD outC[4];
	if (CMSavail && stdTransCMYK2MonG)
	{
		inC[0] = c * 257;
		inC[1] = m * 257;
		inC[2] = y * 257;
		inC[3] = k * 257;
		cmsDoTransform(stdTransCMYK2MonG, inC, outC, 1);
		r = outC[0] / 257;
		g = outC[1] / 257;
		b = outC[2] / 257;
	}
	else
	{
#endif
		r = 255 - QMIN(255, c + k);
		g = 255 - QMIN(255, m + k);
		b = 255 - QMIN(255, y + k);
#ifdef HAVE_CMS
	}
#endif
	return QColor(r, g, b);
}

QColor ScColor::getColorProof(bool gamutCheck) const
{
	if (Model == colorModelRGB)
		return getColorProof(R, G, B, gamutCheck & Gamut);
	return getColorProof(C, M, Y, K, gamutCheck & Gamut);
}

QColor ScColor::getColorProof(int r, int g, int b, bool gamutCkeck) const
{
#ifdef HAVE_CMS
	WORD inC[4];
	WORD outC[4];
	bool hasTransforms = (stdProofRGBGCG && stdProofRGBG && stdTransRGBDoc2MonG);
	if (CMSavail && hasTransforms)
	{
		inC[0] = r * 257;
		inC[1] = g * 257;
		inC[2] = b * 257;
		if (CMSuse && !Spot && SoftProofing)
		{
			cmsHTRANSFORM xform = gamutCkeck ? stdProofRGBGCG : stdProofRGBG;
			cmsDoTransform(xform, inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
		else
		{
			cmsDoTransform(stdTransRGBDoc2MonG, inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
	}
#endif
	return QColor(r, g, b);
}

QColor ScColor::getColorProof(int c, int m, int y, int k, bool gamutCkeck) const
{
	int  r = 0, g = 0, b = 0;
#ifdef HAVE_CMS
	WORD inC[4];
	WORD outC[4];
	bool hasTransforms = (stdProofCMYKGCG && stdProofCMYKG && stdTransCMYK2MonG);
	if (CMSavail && hasTransforms)
	{
		inC[0] = c * 257;
		inC[1] = m * 257;
		inC[2] = y * 257;
		inC[3] = k * 257;
		if (CMSuse && !Spot && SoftProofing)
		{
			cmsHTRANSFORM xform = gamutCkeck ? stdProofCMYKGCG : stdProofCMYKG;
			cmsDoTransform(xform, inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
		else
		{
			cmsDoTransform(stdTransCMYK2MonG, inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
	}
	else
	{
#endif
		r = 255 - QMIN(255, c + k);
		g = 255 - QMIN(255, m + k);
		b = 255 - QMIN(255, y + k);
#ifdef HAVE_CMS
	}
#endif
	return QColor(r, g, b);
}
 
void ScColor::getShadeColorCMYK(int *c, int *m, int *y, int *k, int level) const
{
	if (Model == colorModelRGB)
	{
		int r, g, b;
		ScColor tmpR;
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

void ScColor::getShadeColorRGB(int *r, int *g, int *b, int level) const
{
	int h, s, v, snew;
	
	if (Model == colorModelCMYK)
	{
		int c, m, y, k;
		getShadeColorCMYK(&c, &m, &y, &k, level);
		ScColor tmpC(c, m, y, k);
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

QColor ScColor::getShadeColorProof(int level)
{
	QColor tmp;
	ScColor tmp2;
	int r, g, b, c, m ,y, k;
	
	tmp2.Spot = Spot;
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

void ScColor::getRawRGBColor(int *r, int *g, int *b)
{
	*r = 255-QMIN(255, C+K);
	*g = 255-QMIN(255, M+K);
	*b = 255-QMIN(255, Y+K);
}

QColor ScColor::getRawRGBColor()
{
	return QColor(255-QMIN(255, C+K), 255-QMIN(255, M+K), 255-QMIN(255, Y+K));
}

void ScColor::getRGB(int *r, int *g, int *b)
{
	*r = R;
	*g = G;
	*b = B;
}

void ScColor::getCMYK(int *c, int *m, int *y, int *k)
{
	*c = C;
	*m = M;
	*y = Y;
	*k = K;
}

void ScColor::applyGCR()
{
#ifdef HAVE_CMS
	if (!(CMSuse && CMSavail))
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

QString ScColor::name()
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

QString ScColor::nameCMYK()
{
	QString tmp, name="#";
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
}

QString ScColor::nameRGB()
{
	QString tmp, name="#";
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

void ScColor::setNamedColor(QString name)
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

bool ScColor::isOutOfGamut()
{
	return outOfGamutFlag;
}

void ScColor::checkGamut()
{
	outOfGamutFlag = false;
	if (Spot)
		return;
#ifdef HAVE_CMS
	WORD inC[4];
	WORD outC[4];
	if (CMSuse && CMSavail)
	{
		bool alert = true;
		cmsHTRANSFORM xformProof;
		if (Model == colorModelRGB)
		{
			inC[0] = R*257;
			inC[1] = G*257;
			inC[2] = B*257;
			xformProof = stdProofRGBGCG;
			if ((R == 0) && (B == 0) && (G == 255))
				alert = false;
			if ((R == G && G == B))
				alert = false;
		}
		else
		{
			inC[0] = C*257;
			inC[1] = M*257;
			inC[2] = Y*257;
			inC[3] = K*257;
			xformProof = stdProofCMYKGCG;
			if ((M == 0) && (K == 0) && (C == 255) && (Y == 255))
				alert = false;
			if ((M == 0) && (C == 0) && (Y == 0))
				alert = false;
			if ((M == C) && (C == Y) && (Y == K))
				alert = false;
		}
		if (alert)
		{
			cmsDoTransform(xformProof, inC, outC, 1);
			if ((alert) && ((outC[0] == 0) && (outC[1] == 65535) && (outC[2] == 0)))
				outOfGamutFlag = true;
		}
	}
#endif
}

void ScColor::RecalcRGB()
{
	outOfGamutFlag = false;
	bool alert = true;
#ifdef HAVE_CMS
	WORD inC[4];
	WORD outC[4];
	bool rgbTransAvail  = (stdTransRGBDoc2CMYKG && stdProofRGBGCG  && stdProofRGBG);
	bool cmykTransAvail = (stdTransCMYK2RGBDocG && stdProofCMYKGCG && stdProofCMYKG);
	if (CMSavail && rgbTransAvail && cmykTransAvail)
	{
		if (Model == colorModelRGB)
		{
			// allow RGB greys to go got to CMYK greys without transform
			RGB = QColor(R, G, B);
			if (R == G && G == B)
			{
				C = M = Y = 0;
				K = 255 - R;
				if (CMSuse && !Spot && SoftProofing)
				{
					inC[0] = C * 257;
					inC[1] = M * 257;
					inC[2] = Y * 257;
					inC[3] = K * 257;
					if ((M == 0) && (K == 0) && (C == 255) && (Y == 255))
						alert = false;
					if ((M == 0) && (C == 0) && (Y == 0))
						alert = false;
					if ((M == C) && (C == Y) && (Y == K))
						alert = false;
					cmsDoTransform(Gamut? stdProofCMYKGCG : stdProofCMYKG, inC, outC, 1);
					int Ro = outC[0] / 257;
					int Go = outC[1] / 257;
					int Bo = outC[2] / 257;
					if ((alert) && (Ro == 0) && (Bo == 0) && (Go == 255))
						outOfGamutFlag = true;
					RGB = QColor(Ro, Go, Bo);
				}
			}
			else
			{
				inC[0] = R * 257;
				inC[1] = G * 257;
				inC[2] = B * 257;
				cmsDoTransform(stdTransRGBDoc2CMYKG, inC, outC, 1);
				C = outC[0] / 257;
				M = outC[1] / 257;
				Y = outC[2] / 257;
				K = outC[3] / 257;
				if (CMSuse && !Spot && SoftProofing)
				{
					if ((R == 0) && (B == 0) && (G == 255))
						alert = false;
					cmsDoTransform(Gamut? stdProofRGBGCG : stdProofRGBG, inC, outC, 1);
					int Ro = outC[0] / 257;
					int Go = outC[1] / 257;
					int Bo = outC[2] / 257;
					if ((alert) && (Ro == 0) && (Bo == 0) && (Go == 255))
						outOfGamutFlag = true;
					RGB = QColor(Ro, Go, Bo);
				}
			}
		}
		else
		{
			inC[0] = C * 257;
			inC[1] = M * 257;
			inC[2] = Y * 257;
			inC[3] = K * 257;
			cmsDoTransform(stdTransCMYK2RGBDocG, inC, outC, 1);
			R = outC[0] / 257;
			G = outC[1] / 257;
			B = outC[2] / 257;
			RGB = QColor(R, G, B);
			if (CMSuse && !Spot && SoftProofing)
			{
				if ((M == 0) && (K == 0) && (C == 255) && (Y == 255))
					alert = false;
				if ((M == 0) && (C == 0) && (Y == 0))
					alert = false;
				if ((M == C) && (C == Y) && (Y == K))
					alert = false;
				cmsDoTransform(Gamut? stdProofCMYKGCG : stdProofCMYKG, inC, outC, 1);
				int Ro = outC[0] / 257;
				int Go = outC[1] / 257;
				int Bo = outC[2] / 257;
				if ((alert) && (Ro == 0) && (Bo == 0) && (Go == 255))
					outOfGamutFlag = true;
				RGB = QColor(Ro, Go, Bo);
			}
		}
	}
	else
	{
#endif
		if (Model == colorModelRGB)
		{
			K = QMIN(QMIN(255 - R, 255 - G), 255 - B);
			C = 255 - R - K;
			M = 255 - G - K;
			Y = 255 - B - K;
		}
		else
		{
			R = 255 - QMIN(255, C + K);
			G = 255 - QMIN(255, M + K);
			B = 255 - QMIN(255, Y + K);
		}
		RGB = QColor(R, G, B);
#ifdef HAVE_CMS
	}
#endif
}

bool ScColor::isRegistrationColor()
{
	return Regist;
}

void ScColor::setRegistrationColor(bool s)
{
	Regist = s;
}

bool ScColor::isSpotColor()
{
	return Spot;
}

void ScColor::setSpotColor(bool s)
{
	Spot = s;
}

void ColorList::ensureBlackAndWhite(void)
{
	ensureBlack();
	ensureWhite();
}

void ColorList::ensureBlack(void)
{
	bool addBlack = true;
	ColorList::Iterator itb = find("Black");
	if (itb != end())
	{
		ScColor& black = itb.data();
		colorModel model = black.getColorModel();
		if (model == colorModelCMYK)
		{
			int c, m, y, k;
			black.getCMYK(&c, &m, &y, &k);
			if (c == 0 && m == 0 && y == 0 && k == 255)
				addBlack = false;
		}
	}
	if (addBlack)
		insert("Black", ScColor(0, 0, 0, 255));
}

void ColorList::ensureWhite(void)
{
	bool addWhite = true;
	ColorList::Iterator itw = find("White");
	if (itw != end())
	{
		ScColor& white = itw.data();
		colorModel model = white.getColorModel();
		if (model == colorModelCMYK)
		{
			int c, m, y, k;
			white.getCMYK(&c, &m, &y, &k);
			if (c == 0 && m == 0 && y == 0 && k == 0)
				addWhite = false;
		}
	}
	if (addWhite)
		insert("White", ScColor(0, 0, 0, 0));
}
