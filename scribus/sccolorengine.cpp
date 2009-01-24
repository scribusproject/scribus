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

#include "sccolorengine.h"
#include "scribuscore.h"
#include CMS_INC

QColor ScColorEngine::getRGBColor(const ScColor& color, const ScribusDoc* doc)
{
	RGBColor rgb;
	getRGBValues(color, doc, rgb);
	return QColor(rgb.r, rgb.g, rgb.b);
}

ScColor ScColorEngine::convertToModel(const ScColor& color, const ScribusDoc* doc, colorModel model)
{
	colorModel oldModel = color.getColorModel();
	if( oldModel == model )
		return ScColor(color);
	ScColor newCol;
	if( model == colorModelRGB )
	{
		RGBColor rgb;
		getRGBValues(color, doc, rgb);
		newCol.setColorRGB(rgb.r, rgb.g, rgb.b);
	}
	else
	{
		CMYKColor cmyk;
		getCMYKValues(color, doc, cmyk);
		newCol.setColor(cmyk.c, cmyk.m, cmyk.y, cmyk.k);
	}
	return newCol;
}

void ScColorEngine::getRGBValues(const ScColor& color, const ScribusDoc* doc, RGBColor& rgb)
{
	colorModel    model = color.getColorModel();
	cmsHTRANSFORM transRGB;
	bool doTrans = false;
	if (doc)
	{
		transRGB = doc->stdTransRGB;
		doTrans = doc->HasCMS;
	}
	else
	{
		transRGB = ScCore->defaultCMYKToRGBTrans;
		doTrans = true;
	}
	if (ScCore->haveCMS() && transRGB && doTrans)
	{
		if (model == colorModelRGB)
		{
			rgb.r = color.CR;
			rgb.g = color.MG;
			rgb.b = color.YB;
		}
		else
		{
			WORD inC[4];
			WORD outC[4];
			inC[0] = color.CR * 257;
			inC[1] = color.MG * 257;
			inC[2] = color.YB * 257;
			inC[3] = color.K * 257;
			cmsDoTransform(transRGB, inC, outC, 1);
			rgb.r = outC[0] / 257;
			rgb.g = outC[1] / 257;
			rgb.b = outC[2] / 257;
		}
	}
	else if (model == colorModelCMYK)
	{
		rgb.r = 255 - qMin(255, color.CR + color.K);
		rgb.g = 255 - qMin(255, color.MG + color.K);
		rgb.b = 255 - qMin(255, color.YB + color.K);
	}
	else
	{
		rgb.r = color.CR;
		rgb.g = color.MG;
		rgb.b = color.YB;
	}
}

void ScColorEngine::getCMYKValues(const ScColor& color, const ScribusDoc* doc, CMYKColor& cmyk)
{
	WORD inC[4];
	WORD outC[4];
	colorModel model = color.getColorModel();
	cmsHTRANSFORM transCMYK = doc ? doc->stdTransCMYK : ScCore->defaultRGBToCMYKTrans;
	if (ScCore->haveCMS() && transCMYK)
	{
		if (model == colorModelRGB)
		{
			// allow RGB greys to go got to CMYK greys without transform
			if (color.CR == color.MG && color.MG == color.YB)
			{
				cmyk.c = cmyk.m = cmyk.y = 0;
				cmyk.k = 255 - color.CR;
			}
			else
			{
				inC[0] = color.CR * 257;
				inC[1] = color.MG * 257;
				inC[2] = color.YB * 257;
				cmsDoTransform(transCMYK, inC, outC, 1);
				cmyk.c = outC[0] / 257;
				cmyk.m = outC[1] / 257;
				cmyk.y = outC[2] / 257;
				cmyk.k = outC[3] / 257;
			}
		}
		else
		{
			cmyk.c = color.CR;
			cmyk.m = color.MG;
			cmyk.y = color.YB;
			cmyk.k = color.K;
		}
	}
	else if (model == colorModelRGB)
	{
		cmyk.k = qMin(qMin(255 - color.CR, 255 - color.MG), 255 - color.YB);
		cmyk.c = 255 - color.CR - cmyk.k;
		cmyk.m = 255 - color.MG - cmyk.k;
		cmyk.y = 255 - color.YB - cmyk.k;
	}
	else
	{
		cmyk.c = color.CR;
		cmyk.m = color.MG;
		cmyk.y = color.YB;
		cmyk.k = color.K;
	}
}

void ScColorEngine::getShadeColorCMYK(const ScColor& color, const ScribusDoc* doc, 
										  CMYKColor& cmyk, double level)
{
	if (color.getColorModel() == colorModelRGB)
	{
		RGBColor rgb;
		getShadeColorRGB(color, doc, rgb, level);
		ScColor tmpR(rgb.r, rgb.g, rgb.b);
		getCMYKValues(tmpR, doc, cmyk);
	}
	else
	{
		cmyk.c = qRound(color.CR * level / 100.0);
		cmyk.m = qRound(color.MG * level / 100.0);
		cmyk.y = qRound(color.YB * level / 100.0);
		cmyk.k = qRound(color.K * level / 100.0);
	}
}

void ScColorEngine::getShadeColorRGB(const ScColor& color, const ScribusDoc* doc, RGBColor& rgb, double level)
{
	if (color.getColorModel() == colorModelCMYK)
	{
		CMYKColor cmyk;
		getShadeColorCMYK(color, doc, cmyk, level);
		ScColor tmpC(cmyk.c, cmyk.m, cmyk.y, cmyk.k);
		getRGBValues(tmpC, doc, rgb);
	}
	else
	{
		int h, s, v, snew, vnew;
		QColor tmpR(color.CR, color.MG, color.YB);
		tmpR.getHsv(&h, &s, &v);
		snew = qRound(s * level / 100.0);
		vnew = 255 - qRound(((255 - v) * level / 100.0));
		tmpR.setHsv(h, snew, vnew);
		tmpR.getRgb(&rgb.r, &rgb.g, &rgb.b);
		//We could also compute rgb shade using rgb directly
		/*rgb.CR = 255 - ((255 - color.CR) * level / 100);
		rgb.MG = 255 - ((255 - color.MG) * level / 100);
		rgb.YB = 255 - ((255 - color.YB) * level / 100);*/
	}
}

QColor ScColorEngine::getDisplayColor(const ScColor& color, const ScribusDoc* doc)
{
	QColor tmp;
	if (color.getColorModel() == colorModelRGB)
	{
		RGBColor rgb;
		rgb.r = color.CR;
		rgb.g = color.MG;
		rgb.b = color.YB;
		tmp = getDisplayColor(rgb, doc, color.isSpotColor());
	}
	else
	{
		CMYKColor cmyk;
		cmyk.c = color.CR;
		cmyk.m = color.MG;
		cmyk.y = color.YB;
		cmyk.k = color.K;
		tmp = getDisplayColor(cmyk, doc, color.isSpotColor());
	}
	return tmp;
}

QColor ScColorEngine::getDisplayColor(const ScColor& color, const ScribusDoc* doc, double level)
{
	QColor tmp;
	if (color.getColorModel() == colorModelRGB)
	{
		RGBColor rgb;
		rgb.r = color.CR;
		rgb.g = color.MG;
		rgb.b = color.YB;
		getShadeColorRGB(color, doc, rgb, level);
		tmp = getDisplayColor(rgb, doc, color.isSpotColor());
	}
	else
	{
		CMYKColor cmyk;
		cmyk.c = color.CR;
		cmyk.m = color.MG;
		cmyk.y = color.YB;
		cmyk.k = color.K;
		getShadeColorCMYK(color, doc, cmyk, level);
		tmp = getDisplayColor(cmyk, doc, color.isSpotColor());
	}
	return tmp;
}

QColor ScColorEngine::getDisplayColorGC(const ScColor& color, const ScribusDoc* doc)
{
	QColor tmp;
	bool doSoftProofing = doc ? doc->SoftProofing : false;
	bool doGamutCheck   = doc ? doc->Gamut : false;
	if ( doSoftProofing && doGamutCheck )
	{
		bool outOfGamutFlag = isOutOfGamut(color, doc);
		tmp = outOfGamutFlag ? QColor(0, 255, 0) : getDisplayColor(color, doc);
	}
	else
		tmp = getDisplayColor(color, doc);
	return tmp;
}

QColor ScColorEngine::getColorProof(const ScColor& color, const ScribusDoc* doc, bool gamutCheck)
{
	QColor tmp;
	bool gamutChkEnabled = doc ? doc->Gamut : false;
	bool spot = color.isSpotColor();
	if (color.getColorModel() == colorModelRGB)
	{
		// Match 133x behavior (RGB greys map to cmyk greys) until we are able to make rgb profiled output
		if ( color.CR == color.MG && color.MG == color.YB )
			gamutChkEnabled = false;
		RGBColor rgb;
		rgb.r = color.CR;
		rgb.g = color.MG;
		rgb.b = color.YB;
		tmp = getColorProof(rgb, doc, spot, gamutCheck & gamutChkEnabled);
	}
	else
	{
		CMYKColor cmyk;
		cmyk.c = color.CR;
		cmyk.m = color.MG;
		cmyk.y = color.YB;
		cmyk.k = color.K;
		tmp = getColorProof(cmyk, doc, spot, gamutCheck & gamutChkEnabled);
	}
	return tmp;
}

QColor ScColorEngine::getShadeColor(const ScColor& color, const ScribusDoc* doc, double level)
{
	RGBColor rgb;
	rgb.r = color.CR;
	rgb.g = color.MG;
	rgb.b = color.YB;
	getShadeColorRGB(color, doc, rgb, level);
	return QColor(rgb.r, rgb.g, rgb.b);
}

QColor ScColorEngine::getShadeColorProof(const ScColor& color, const ScribusDoc* doc, double level)
{
	QColor tmp;
	bool doGC = false;
	if (doc)
		doGC = doc->Gamut;
	
	if (color.getColorModel() == colorModelRGB)
	{
		RGBColor rgb;
		rgb.r = color.CR;
		rgb.g = color.MG;
		rgb.b = color.YB;
		getShadeColorRGB(color, doc, rgb, level);
		// Match 133x behavior for rgb grey until we are able to make rgb profiled output
		// (RGB greys map to cmyk greys)
		if ( rgb.r == rgb.g && rgb.g == rgb.b )
		{
			doGC = false;
			CMYKColor cmyk;
			cmyk.c = cmyk.m = cmyk.y = 0;
			cmyk.k = 255 - rgb.g;
			tmp = getColorProof(cmyk, doc, color.isSpotColor(), doGC);
		}
		else
			tmp = getColorProof(rgb, doc, color.isSpotColor(), doGC);
	}
	else
	{
		CMYKColor cmyk;
		cmyk.c = color.CR;
		cmyk.m = color.MG;
		cmyk.y = color.YB;
		cmyk.k = color.K;
		getShadeColorCMYK(color, doc, cmyk, level);
		tmp = getColorProof(cmyk, doc, color.isSpotColor(), doGC);
	}
	
	return tmp;
}

QColor ScColorEngine::getColorProof(RGBColor& rgb, const ScribusDoc* doc, bool spot, bool gamutCkeck)
{
	WORD inC[4];
	WORD outC[4];
	int  r = rgb.r, g = rgb.g, b = rgb.b;
	cmsHTRANSFORM transRGBMon  = doc ? doc->stdTransRGBMon : ScCore->defaultRGBToScreenSolidTrans;
	cmsHTRANSFORM transProof   = doc ? doc->stdProof   : ScCore->defaultRGBToScreenSolidTrans;
	cmsHTRANSFORM transProofGC = doc ? doc->stdProofGC : ScCore->defaultRGBToScreenSolidTrans;
	bool cmsUse   = doc ? doc->HasCMS : false;
	bool cmsTrans = (transRGBMon && transProof && transProofGC); 
	if (ScCore->haveCMS() && cmsTrans)
	{
		inC[0] = rgb.r * 257;
		inC[1] = rgb.g * 257;
		inC[2] = rgb.b * 257;
		if (cmsUse && !spot && doc->SoftProofing)
		{
			cmsHTRANSFORM xform = gamutCkeck ? transProofGC : transProof;
			cmsDoTransform(xform, inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
		else
		{
			cmsDoTransform(transRGBMon, inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
	}
	return QColor(r, g, b);
}

QColor ScColorEngine::getColorProof(CMYKColor& cmyk, const ScribusDoc* doc, bool spot, bool gamutCkeck)
{
	int  r = 0, g = 0, b = 0;
	WORD inC[4];
	WORD outC[4];
	cmsHTRANSFORM transCMYKMon     = doc ? doc->stdTransCMYKMon : ScCore->defaultCMYKToRGBTrans;
	cmsHTRANSFORM transProofCMYK   = doc ? doc->stdProofCMYK   : ScCore->defaultCMYKToRGBTrans;
	cmsHTRANSFORM transProofCMYKGC = doc ? doc->stdProofCMYKGC : ScCore->defaultCMYKToRGBTrans;
	bool cmsUse   = doc ? doc->HasCMS : false;
	bool cmsTrans = (transCMYKMon && transProofCMYK && transProofCMYKGC); 
	if (ScCore->haveCMS() && cmsTrans)
	{
		inC[0] = cmyk.c * 257;
		inC[1] = cmyk.m * 257;
		inC[2] = cmyk.y * 257;
		inC[3] = cmyk.k * 257;
		if (cmsUse && !spot && doc->SoftProofing)
		{
			cmsHTRANSFORM xform = gamutCkeck ? transProofCMYKGC : transProofCMYK;
			cmsDoTransform(xform, inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
		else
		{
			cmsDoTransform(transCMYKMon, inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
	}
	else
	{
		r = 255 - qMin(255, cmyk.c + cmyk.k);
		g = 255 - qMin(255, cmyk.m + cmyk.k);
		b = 255 - qMin(255, cmyk.y + cmyk.k);
	}
	return QColor(r, g, b);
}

QColor ScColorEngine::getDisplayColor(RGBColor& rgb, const ScribusDoc* doc, bool spot)
{
	WORD inC[4];
	WORD outC[4];
	int r = rgb.r;
	int g = rgb.g;
	int b = rgb.b; 
	cmsHTRANSFORM transRGBMon = doc ? doc->stdTransRGBMon : ScCore->defaultRGBToScreenSolidTrans;
	if (ScCore->haveCMS() && transRGBMon)
	{
		inC[0] = r * 257;
		inC[1] = g * 257;
		inC[2] = b * 257;
		cmsDoTransform(transRGBMon, inC, outC, 1);
		r = outC[0] / 257;
		g = outC[1] / 257;
		b = outC[2] / 257;
	}
	return QColor(r, g, b);
}

QColor ScColorEngine::getDisplayColor(CMYKColor& cmyk, const ScribusDoc* doc, bool spot)
{
	int  r = 0, g = 0, b = 0;
	WORD inC[4];
	WORD outC[4];
	cmsHTRANSFORM transCMYKMon = doc ? doc->stdTransCMYKMon : ScCore->defaultCMYKToRGBTrans;
	if (ScCore->haveCMS() && transCMYKMon)
	{
		inC[0] = cmyk.c * 257;
		inC[1] = cmyk.m * 257;
		inC[2] = cmyk.y * 257;
		inC[3] = cmyk.k * 257;
		cmsDoTransform(transCMYKMon, inC, outC, 1);
		r = outC[0] / 257;
		g = outC[1] / 257;
		b = outC[2] / 257;
	}
	else
	{
		r = 255 - qMin(255, cmyk.c + cmyk.k);
		g = 255 - qMin(255, cmyk.m + cmyk.k);
		b = 255 - qMin(255, cmyk.y + cmyk.k);
	}
	return QColor(r, g, b);
}

bool ScColorEngine::isOutOfGamut(const ScColor& color, const ScribusDoc* doc)
{
	bool outOfGamutFlag = false;
	if (color.isSpotColor())
		return false;
	WORD inC[4];
	WORD outC[4];
	bool cmsUse = doc ? doc->HasCMS : false;
	if (ScCore->haveCMS() && cmsUse)
	{
		bool alert = true;
		cmsHTRANSFORM xformProof;
		if (color.getColorModel() == colorModelRGB)
		{
			inC[0] = color.CR * 257;
			inC[1] = color.MG * 257;
			inC[2] = color.YB * 257;
			xformProof = doc->stdProofGC;
			if ((color.CR == 0) && (color.YB == 0) && (color.MG == 255))
				alert = false;
			if ((color.CR == color.MG && color.MG == color.YB))
				alert = false;
		}
		else
		{
			inC[0] = color.CR * 257;
			inC[1] = color.MG * 257;
			inC[2] = color.YB * 257;
			inC[3] = color.K * 257;
			xformProof = doc->stdProofCMYKGC;
			if ((color.MG == 0) && (color.K == 0) && (color.CR == 255) && (color.YB == 255))
				alert = false;
			if ((color.MG == 0) && (color.CR == 0) && (color.YB == 0))
				alert = false;
			if ((color.MG == color.CR) && (color.CR == color.YB) && (color.YB == color.K))
				alert = false;
		}
		if (alert)
		{
			cmsDoTransform(xformProof, inC, outC, 1);
			if ((outC[0]/257 == 0) && (outC[1]/257 == 255) && (outC[2]/257 == 0))
				outOfGamutFlag = true;
		}
	}
	return outOfGamutFlag;
}

void ScColorEngine::applyGCR(ScColor& color, const ScribusDoc* doc)
{
	bool cmsUse = doc ? doc->HasCMS : false;
	if (!(ScCore->haveCMS() && cmsUse))
	{
		CMYKColor cmyk;
		getCMYKValues(color, doc, cmyk);
		int k = qMin(qMin(cmyk.c, cmyk.m), cmyk.y);
		color.CR = cmyk.c - k;
		color.MG = cmyk.m - k;
		color.YB = cmyk.y - k;
		color.K = qMin((cmyk.k + k), 255);
	}
}
