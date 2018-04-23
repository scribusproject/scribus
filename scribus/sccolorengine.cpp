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
#include "scribusdoc.h"
#include "colormgmt/sccolormgmtengine.h"
#include <cmath>

QColor ScColorEngine::getRGBColor(const ScColor& color, const ScribusDoc* doc)
{
	RGBColor rgb;
	getRGBValues(color, doc, rgb);
	return QColor(rgb.r, rgb.g, rgb.b);
}

ScColor ScColorEngine::convertToModel(const ScColor& color, const ScribusDoc* doc, colorModel model)
{
	colorModel oldModel = color.getColorModel();
	if (oldModel == model)
		return ScColor(color);
	ScColor newCol;
	if (model == colorModelRGB)
	{
		RGBColorF rgb;
		getRGBValues(color, doc, rgb);
		newCol.setRgbColorF(rgb.r, rgb.g, rgb.b);
	}
	else if (model == colorModelCMYK)
	{
		CMYKColorF cmyk;
		getCMYKValues(color, doc, cmyk);
		newCol.setColorF(cmyk.c, cmyk.m, cmyk.y, cmyk.k);
	}
	else if (model == colorModelLab)
	{
		ScColorMgmtEngine engine(ScCore->defaultEngine);
		if (oldModel == colorModelRGB)
		{
			ScColorProfile profRGB = doc ? doc->DocInputRGBProf : ScCore->defaultRGBProfile;
			ScColorProfile profLab = ScCore->defaultLabProfile;
			ScColorTransform trans = engine.createTransform(profRGB, Format_RGB_16, profLab, Format_Lab_Dbl, Intent_Perceptual, 0);
			double outC[3];
			unsigned short inC[3];
			inC[0] = qRound(color.m_values[0] * 65535);
			inC[1] = qRound(color.m_values[1] * 65535);
			inC[2] = qRound(color.m_values[2] * 65535);
			trans.apply(inC, outC, 1);
			newCol.setLabColor(outC[0], outC[1], outC[2]);
		}
		else
		{
			ScColorProfile profCMYK = doc ? doc->DocInputCMYKProf : ScCore->defaultCMYKProfile;
			ScColorProfile profLab = ScCore->defaultLabProfile;
			ScColorTransform trans = engine.createTransform(profCMYK, Format_CMYK_16, profLab, Format_Lab_Dbl, Intent_Perceptual, 0);
			double outC[3];
			unsigned short inC[4];
			inC[0] = qRound(color.m_values[0] * 65535);
			inC[1] = qRound(color.m_values[1] * 65535);
			inC[2] = qRound(color.m_values[2] * 65535);
			inC[3] = qRound(color.m_values[3] * 65535);
			trans.apply(inC, outC, 1);
			newCol.setLabColor(outC[0], outC[1], outC[2]);
		}
	}
	return newCol;
}

void ScColorEngine::getRGBValues(const ScColor& color, const ScribusDoc* doc, RGBColor& rgb)
{
	colorModel    model = color.getColorModel();
	ScColorTransform transRGB = doc ? doc->stdTransRGB : ScCore->defaultCMYKToRGBTrans;
	if (ScCore->haveCMS() && transRGB)
	{
		if (model == colorModelRGB)
		{
			rgb.r = qRound(color.m_values[0] * 255.0);
			rgb.g = qRound(color.m_values[1] * 255.0);
			rgb.b = qRound(color.m_values[2] * 255.0);
		}
		else if (model == colorModelCMYK)
		{
			unsigned short inC[4];
			unsigned short outC[4];
			inC[0] = qRound(color.m_values[0] * 65535);
			inC[1] = qRound(color.m_values[1] * 65535);
			inC[2] = qRound(color.m_values[2] * 65535);
			inC[3] = qRound(color.m_values[3] * 65535);
			transRGB.apply(inC, outC, 1);
			rgb.r = outC[0] / 257;
			rgb.g = outC[1] / 257;
			rgb.b = outC[2] / 257;
		}
		else if (model == colorModelLab)
		{
			ScColorTransform trans = doc ? doc->stdLabToRGBTrans : ScCore->defaultLabToRGBTrans;
			double inC[3];
			inC[0] = color.m_L_val;
			inC[1] = color.m_a_val;
			inC[2] = color.m_b_val;
			quint16 outC[3];
			trans.apply(inC, outC, 1);
			rgb.r = outC[0] / 257;
			rgb.g = outC[1] / 257;
			rgb.b = outC[2] / 257;
		}
	}
	else if (model == colorModelCMYK)
	{
		double r = 1.0 - qMin(1.0, color.m_values[0] + color.m_values[3]);
		double g = 1.0 - qMin(1.0, color.m_values[1] + color.m_values[3]);
		double b = 1.0 - qMin(1.0, color.m_values[2] + color.m_values[3]);
		rgb.r = qRound(r * 255.0);
		rgb.g = qRound(g * 255.0);
		rgb.b = qRound(b * 255.0);
	}
	else if (model == colorModelRGB)
	{
		rgb.r = qRound(color.m_values[0] * 255.0);
		rgb.g = qRound(color.m_values[1] * 255.0);
		rgb.b = qRound(color.m_values[2] * 255.0);
	}
	else if (model == colorModelLab)
	{
		// First step: Lab -> XYZ
		double var_Y = (color.m_L_val + 16) / 116.0;
		double var_X = color.m_a_val / 500.0 + var_Y;
		double var_Z = var_Y - color.m_b_val / 200.0;
		if (pow(var_Y, 3) > 0.008856)
			var_Y = pow(var_Y, 3);
		else
			var_Y = (var_Y - 16.0 / 116.0) / 7.787;
		if (pow(var_X, 3) > 0.008856)
			var_X = pow(var_X, 3);
		else
			var_X = (var_X - 16.0 / 116.0) / 7.787;
		if (pow(var_Z, 3) > 0.008856)
			var_Z = pow(var_Z, 3);
		else
			var_Z = (var_Z - 16.0 / 116.0) / 7.787;
		// Second step: XYZ -> RGB
		// Whitepoint D50
		var_X = 0.990720 * var_X;
		var_Y = 1.00000  * var_Y;
		var_Z = 0.825210 * var_Z;
		double var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
		double var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
		double var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;
		if (var_R > 0.0031308)
			var_R = 1.055 * (pow(var_R, (1.0 / 2.4))) - 0.055;
		else
			var_R = 12.92 * var_R;
		if (var_G > 0.0031308)
			var_G = 1.055 * (pow(var_G, (1.0 / 2.4))) - 0.055;
		else
			var_G = 12.92 * var_G;
		if (var_B > 0.0031308)
			var_B = 1.055 * (pow(var_B, (1.0 / 2.4))) - 0.055;
		else
			var_B = 12.92 * var_B;
		rgb.r = qRound(qMax(qMin(var_R, 1.0), 0.0) * 255.0);
		rgb.g = qRound(qMax(qMin(var_G, 1.0), 0.0) * 255.0);
		rgb.b = qRound(qMax(qMin(var_B, 1.0), 0.0) * 255.0);
	/*	ScColorTransform trans = doc ? doc->stdLabToRGBTrans : ScCore->defaultLabToRGBTrans;
		double inC[3];
		inC[0] = color.L_val;
		inC[1] = color.a_val;
		inC[2] = color.b_val;
		quint16 outC[3];
		trans.apply(inC, outC, 1);
		rgb.r = outC[0] / 257;
		rgb.g = outC[1] / 257;
		rgb.b = outC[2] / 257;*/
	}
}

void ScColorEngine::getRGBValues(const ScColor& color, const ScribusDoc* doc, RGBColorF& rgb)
{
	colorModel    model = color.getColorModel();
	ScColorTransform transRGB = doc ? doc->stdTransRGB : ScCore->defaultCMYKToRGBTrans;
	if (ScCore->haveCMS() && transRGB)
	{
		if (model == colorModelRGB)
		{
			rgb.r = color.m_values[0];
			rgb.g = color.m_values[1];
			rgb.b = color.m_values[2];
		}
		else if (model == colorModelCMYK)
		{
			unsigned short inC[4];
			unsigned short outC[4];
			inC[0] = qRound(color.m_values[0] * 65535);
			inC[1] = qRound(color.m_values[1] * 65535);
			inC[2] = qRound(color.m_values[2] * 65535);
			inC[3] = qRound(color.m_values[3] * 65535);
			transRGB.apply(inC, outC, 1);
			rgb.r = outC[0] / 65535.0;
			rgb.g = outC[1] / 65535.0;
			rgb.b = outC[2] / 65535.0;
		}
		else if (model == colorModelLab)
		{
			ScColorTransform trans = doc ? doc->stdLabToRGBTrans : ScCore->defaultLabToRGBTrans;
			double inC[3];
			inC[0] = color.m_L_val;
			inC[1] = color.m_a_val;
			inC[2] = color.m_b_val;
			quint16 outC[3];
			trans.apply(inC, outC, 1);
			rgb.r = outC[0] / 65535.0;
			rgb.g = outC[1] / 65535.0;
			rgb.b = outC[2] / 65535.0;
		}
	}
	else if (model == colorModelCMYK)
	{
		rgb.r = 1.0 - qMin(1.0, color.m_values[0] + color.m_values[3]);
		rgb.g = 1.0 - qMin(1.0, color.m_values[1] + color.m_values[3]);
		rgb.b = 1.0 - qMin(1.0, color.m_values[2] + color.m_values[3]);
	}
	else if (model == colorModelRGB)
	{
		rgb.r = color.m_values[0];
		rgb.g = color.m_values[1];
		rgb.b = color.m_values[2];
	}
	else if (model == colorModelLab)
	{
		// First step: Lab -> XYZ
		double var_Y = (color.m_L_val + 16) / 116.0;
		double var_X = color.m_a_val / 500.0 + var_Y;
		double var_Z = var_Y - color.m_b_val / 200.0;
		if (pow(var_Y, 3) > 0.008856)
			var_Y = pow(var_Y, 3);
		else
			var_Y = (var_Y - 16.0 / 116.0) / 7.787;
		if (pow(var_X, 3) > 0.008856)
			var_X = pow(var_X, 3);
		else
			var_X = (var_X - 16.0 / 116.0) / 7.787;
		if (pow(var_Z, 3) > 0.008856)
			var_Z = pow(var_Z, 3);
		else
			var_Z = (var_Z - 16.0 / 116.0) / 7.787;
		// Second step: XYZ -> RGB
		// Whitepoint D50
		var_X = 0.990720 * var_X;
		var_Y = 1.00000  * var_Y;
		var_Z = 0.825210 * var_Z;
		double var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
		double var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
		double var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;
		if (var_R > 0.0031308)
			var_R = 1.055 * (pow(var_R, (1.0 / 2.4))) - 0.055;
		else
			var_R = 12.92 * var_R;
		if (var_G > 0.0031308)
			var_G = 1.055 * (pow(var_G, (1.0 / 2.4))) - 0.055;
		else
			var_G = 12.92 * var_G;
		if (var_B > 0.0031308)
			var_B = 1.055 * (pow(var_B, (1.0 / 2.4))) - 0.055;
		else
			var_B = 12.92 * var_B;
		rgb.r = qMax(0.0, qMin(var_R, 1.0));
		rgb.g = qMax(0.0, qMin(var_G, 1.0));
		rgb.b = qMax(0.0, qMin(var_B, 1.0));
	/*	ScColorTransform trans = doc ? doc->stdLabToRGBTrans : ScCore->defaultLabToRGBTrans;
		double inC[3];
		inC[0] = color.L_val;
		inC[1] = color.a_val;
		inC[2] = color.b_val;
		quint16 outC[3];
		trans.apply(inC, outC, 1);
		rgb.r = outC[0] / 65535.0;
		rgb.g = outC[1] / 65535.0;
		rgb.b = outC[2] / 65535.0;*/
	}
}

void ScColorEngine::getCMYKValues(const ScColor& color, const ScribusDoc* doc, CMYKColor& cmyk)
{
	unsigned short inC[4];
	unsigned short outC[4];
	colorModel model = color.getColorModel();
	ScColorTransform transCMYK = doc ? doc->stdTransCMYK : ScCore->defaultRGBToCMYKTrans;
	if (ScCore->haveCMS() && transCMYK)
	{
		if (model == colorModelRGB)
		{
			// allow RGB greys to go to CMYK greys without transform
			if (color.m_values[0] == color.m_values[1] && color.m_values[1] == color.m_values[2])
			{
				cmyk.c = cmyk.m = cmyk.y = 0;
				cmyk.k = 255 - qRound(color.m_values[0] * 255.0);
			}
			else
			{
				inC[0] = qRound(color.m_values[0] * 65535.0);
				inC[1] = qRound(color.m_values[1] * 65535.0);
				inC[2] = qRound(color.m_values[2] * 65535.0);
				transCMYK.apply(inC, outC, 1);
				cmyk.c = outC[0] / 257;
				cmyk.m = outC[1] / 257;
				cmyk.y = outC[2] / 257;
				cmyk.k = outC[3] / 257;
			}
		}
		else if (model == colorModelCMYK)
		{
			cmyk.c = qRound(color.m_values[0] * 255.0);
			cmyk.m = qRound(color.m_values[1] * 255.0);
			cmyk.y = qRound(color.m_values[2] * 255.0);
			cmyk.k = qRound(color.m_values[3] * 255.0);
		}
		else if (model == colorModelLab)
		{
			ScColorTransform trans = doc ? doc->stdLabToCMYKTrans : ScCore->defaultLabToCMYKTrans;
			double inC[3];
			inC[0] = color.m_L_val;
			inC[1] = color.m_a_val;
			inC[2] = color.m_b_val;
			quint16 outC[4];
			trans.apply(inC, outC, 1);
			cmyk.c = outC[0] / 257;
			cmyk.m = outC[1] / 257;
			cmyk.y = outC[2] / 257;
			cmyk.k = outC[3] / 257;
		}
	}
	else if (model == colorModelRGB)
	{
		double k = qMin(qMin(1.0 - color.m_values[0], 1.0 - color.m_values[1]), 1.0 - color.m_values[2]);
		cmyk.k = qRound(k * 255.0);
		cmyk.c = 255 - qRound((color.m_values[0] - k) * 255.0);
		cmyk.m = 255 - qRound((color.m_values[1] - k) * 255.0);
		cmyk.y = 255 - qRound((color.m_values[2] - k) * 255.0);
	}
	else if (model == colorModelCMYK)
	{
		cmyk.c = qRound(color.m_values[0] * 255.0);
		cmyk.m = qRound(color.m_values[1] * 255.0);
		cmyk.y = qRound(color.m_values[2] * 255.0);
		cmyk.k = qRound(color.m_values[3] * 255.0);
	}
	else if (model == colorModelLab)
	{
		ScColorTransform trans = doc ? doc->stdLabToCMYKTrans : ScCore->defaultLabToCMYKTrans;
		double inC[3];
		inC[0] = color.m_L_val;
		inC[1] = color.m_a_val;
		inC[2] = color.m_b_val;
		quint16 outC[4];
		trans.apply(inC, outC, 1);
		cmyk.c = outC[0] / 257;
		cmyk.m = outC[1] / 257;
		cmyk.y = outC[2] / 257;
		cmyk.k = outC[3] / 257;
	}
}

void ScColorEngine::getCMYKValues(const ScColor& color, const ScribusDoc* doc, CMYKColorF& cmyk)
{
	unsigned short inC[4];
	unsigned short outC[4];
	colorModel model = color.getColorModel();
	ScColorTransform transCMYK = doc ? doc->stdTransCMYK : ScCore->defaultRGBToCMYKTrans;
	if (ScCore->haveCMS() && transCMYK)
	{
		if (model == colorModelRGB)
		{
			// allow RGB greys to go to CMYK greys without transform
			if (color.m_values[0] == color.m_values[1] && color.m_values[1] == color.m_values[2])
			{
				cmyk.c = cmyk.m = cmyk.y = 0;
				cmyk.k = 1.0 - color.m_values[0];
			}
			else
			{
				inC[0] = qRound(color.m_values[0] * 65535.0);
				inC[1] = qRound(color.m_values[1] * 65535.0);
				inC[2] = qRound(color.m_values[2] * 65535.0);
				transCMYK.apply(inC, outC, 1);
				cmyk.c = outC[0] / 65535.0;
				cmyk.m = outC[1] / 65535.0;
				cmyk.y = outC[2] / 65535.0;
				cmyk.k = outC[3] / 65535.0;
			}
		}
		else if (model == colorModelCMYK)
		{
			cmyk.c = color.m_values[0];
			cmyk.m = color.m_values[1];
			cmyk.y = color.m_values[2];
			cmyk.k = color.m_values[3];
		}
		else if (model == colorModelLab)
		{
			ScColorTransform trans = doc ? doc->stdLabToCMYKTrans : ScCore->defaultLabToCMYKTrans;
			double inC[3];
			inC[0] = color.m_L_val;
			inC[1] = color.m_a_val;
			inC[2] = color.m_b_val;
			quint16 outC[4];
			trans.apply(inC, outC, 1);
			cmyk.c = outC[0] / 65535.0;
			cmyk.m = outC[1] / 65535.0;
			cmyk.y = outC[2] / 65535.0;
			cmyk.k = outC[3] / 65535.0;
		}
	}
	else if (model == colorModelRGB)
	{
		cmyk.k = qMin(qMin(1.0 - color.m_values[0], 1.0 - color.m_values[1]), 1.0 - color.m_values[2]);
		cmyk.c = 1.0 - (color.m_values[0] - cmyk.k);
		cmyk.m = 1.0 - (color.m_values[1] - cmyk.k);
		cmyk.y = 1.0 - (color.m_values[2] - cmyk.k);
	}
	else if (model == colorModelCMYK)
	{
		cmyk.c = color.m_values[0];
		cmyk.m = color.m_values[1];
		cmyk.y = color.m_values[2];
		cmyk.k = color.m_values[3];
	}
	else if (model == colorModelLab)
	{
		ScColorTransform trans = doc ? doc->stdLabToCMYKTrans : ScCore->defaultLabToCMYKTrans;
		double inC[3];
		inC[0] = color.m_L_val;
		inC[1] = color.m_a_val;
		inC[2] = color.m_b_val;
		quint16 outC[4];
		trans.apply(inC, outC, 1);
		cmyk.c = outC[0] / 65535.0;
		cmyk.m = outC[1] / 65535.0;
		cmyk.y = outC[2] / 65535.0;
		cmyk.k = outC[3] / 65535.0;
	}
}

void ScColorEngine::getShadeColorCMYK(const ScColor& color, const ScribusDoc* doc, 
										  CMYKColor& cmyk, double level)
{
	if (color.getColorModel() == colorModelRGB)
	{
		RGBColorF rgb;
		getShadeColorRGB(color, doc, rgb, level);
		ScColor tmpR;
		tmpR.setRgbColorF(rgb.r, rgb.g, rgb.b);
		getCMYKValues(tmpR, doc, cmyk);
	}
	else if (color.getColorModel() == colorModelCMYK)
	{
		cmyk.c = qRound(color.m_values[0] * level / 100.0 * 255.0);
		cmyk.m = qRound(color.m_values[1] * level / 100.0 * 255.0);
		cmyk.y = qRound(color.m_values[2] * level / 100.0 * 255.0);
		cmyk.k = qRound(color.m_values[3] * level / 100.0 * 255.0);
	}
	else if (color.getColorModel() == colorModelLab)
	{
		ScColorTransform trans = doc ? doc->stdLabToCMYKTrans : ScCore->defaultLabToCMYKTrans;
		double inC[3];
		inC[0] = 100 - (100 - color.m_L_val) * (level / 100.0);
		inC[1] = color.m_a_val * (level / 100.0);
		inC[2] = color.m_b_val * (level / 100.0);
		quint16 outC[4];
		trans.apply(inC, outC, 1);
		cmyk.c = outC[0] / 257;
		cmyk.m = outC[1] / 257;
		cmyk.y = outC[2] / 257;
		cmyk.k = outC[3] / 257;
	}
}

void ScColorEngine::getShadeColorCMYK(const ScColor& color, const ScribusDoc* doc, 
										  CMYKColorF& cmyk, double level)
{
	if (color.getColorModel() == colorModelRGB)
	{
		RGBColorF rgb;
		getShadeColorRGB(color, doc, rgb, level);
		ScColor tmpR;
		tmpR.setRgbColorF(rgb.r, rgb.g, rgb.b);
		getCMYKValues(tmpR, doc, cmyk);
	}
	else if (color.getColorModel() == colorModelCMYK)
	{
		cmyk.c = color.m_values[0] * level / 100.0;
		cmyk.m = color.m_values[1] * level / 100.0;
		cmyk.y = color.m_values[2] * level / 100.0;
		cmyk.k = color.m_values[3] * level / 100.0;
	}
	else if (color.getColorModel() == colorModelLab)
	{
		ScColorTransform trans = doc ? doc->stdLabToCMYKTrans : ScCore->defaultLabToCMYKTrans;
		double inC[3];
		inC[0] = 100 - (100 - color.m_L_val) * (level / 100.0);
		inC[1] = color.m_a_val * (level / 100.0);
		inC[2] = color.m_b_val * (level / 100.0);
		quint16 outC[4];
		trans.apply(inC, outC, 1);
		cmyk.c = outC[0] / 65535.0;
		cmyk.m = outC[1] / 65535.0;
		cmyk.y = outC[2] / 65535.0;
		cmyk.k = outC[3] / 65535.0;
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
	else if (color.getColorModel() == colorModelRGB)
	{
		int h, s, v, snew, vnew;
		QColor tmpR = color.getRawRGBColor();
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
	else if (color.getColorModel() == colorModelLab)
	{
		ScColorTransform trans = doc ? doc->stdLabToRGBTrans : ScCore->defaultLabToRGBTrans;
		double inC[3];
		inC[0] = 100 - (100 - color.m_L_val) * (level / 100.0);
		inC[1] = color.m_a_val * (level / 100.0);
		inC[2] = color.m_b_val * (level / 100.0);
		quint16 outC[3];
		trans.apply(inC, outC, 1);
		rgb.r = outC[0] / 257;
		rgb.g = outC[1] / 257;
		rgb.b = outC[2] / 257;
	}
}

void ScColorEngine::getShadeColorRGB(const ScColor& color, const ScribusDoc* doc, RGBColorF& rgb, double level)
{
	if (color.getColorModel() == colorModelCMYK)
	{
		CMYKColorF cmyk;
		getShadeColorCMYK(color, doc, cmyk, level);
		ScColor tmpC;
		tmpC.setColorF(cmyk.c, cmyk.m, cmyk.y, cmyk.k);
		getRGBValues(tmpC, doc, rgb);
	}
	else if (color.getColorModel() == colorModelRGB)
	{
		int h, s, v, snew, vnew;
		QColor tmpR = color.getRawRGBColor();
		tmpR.getHsv(&h, &s, &v);
		snew = qRound(s * level / 100.0);
		vnew = 255 - qRound(((255 - v) * level / 100.0));
		RGBColor tmpRgb;
		tmpR.setHsv(h, snew, vnew);
		tmpR.getRgb(&tmpRgb.r, &tmpRgb.g, &tmpRgb.b);
		rgb.r = tmpRgb.r / 255.0;
		rgb.g = tmpRgb.g / 255.0;
		rgb.b = tmpRgb.b / 255.0;
		//We could also compute rgb shade using rgb directly
		/*rgb.r = 1.0 - ((1.0 - color.m_values[0]) * level / 100);
		rgb.g = 1.0 - ((1.0 - color.m_values[1]) * level / 100);
		rgb.b = 1.0 - ((1.0 - color.m_values[2]) * level / 100);*/
	}
	else if (color.getColorModel() == colorModelLab)
	{
		ScColorTransform trans = doc ? doc->stdLabToRGBTrans : ScCore->defaultLabToRGBTrans;
		double inC[3];
		inC[0] = 100 - (100 - color.m_L_val) * (level / 100.0);
		inC[1] = color.m_a_val * (level / 100.0);
		inC[2] = color.m_b_val * (level / 100.0);
		quint16 outC[3];
		trans.apply(inC, outC, 1);
		rgb.r = outC[0] / 65535.0;
		rgb.g = outC[1] / 65535.0;
		rgb.b = outC[2] / 65535.0;
	}
}

QColor ScColorEngine::getDisplayColor(const ScColor& color, const ScribusDoc* doc)
{
	QColor tmp;
	if (color.getColorModel() == colorModelRGB)
	{
		RGBColorF rgb;
		rgb.r = color.m_values[0];
		rgb.g = color.m_values[1];
		rgb.b = color.m_values[2];
		tmp = getDisplayColor(rgb, doc, color.isSpotColor());
	}
	else if (color.getColorModel() == colorModelCMYK)
	{
		CMYKColorF cmyk;
		cmyk.c = color.m_values[0];
		cmyk.m = color.m_values[1];
		cmyk.y = color.m_values[2];
		cmyk.k = color.m_values[3];
		tmp = getDisplayColor(cmyk, doc, color.isSpotColor());
	}
	else if (color.getColorModel() == colorModelLab)
	{
		bool cmsUse = doc ? doc->HasCMS : false;
		ScColorTransform trans  = doc ? doc->stdLabToScreenTrans : ScCore->defaultLabToRGBTrans;
		if (cmsUse && trans)
		{
			double inC[3];
			inC[0] = color.m_L_val;
			inC[1] = color.m_a_val;
			inC[2] = color.m_b_val;
			quint16 outC[3];
			trans.apply(inC, outC, 1);
			tmp = QColor(outC[0] / 257, outC[1] / 257, outC[2] / 257);
		}
		else
		{
			// First step: Lab -> XYZ
			double var_Y = (color.m_L_val + 16) / 116.0;
			double var_X = color.m_a_val / 500.0 + var_Y;
			double var_Z = var_Y - color.m_b_val / 200.0;
			if (pow(var_Y, 3) > 0.008856)
				var_Y = pow(var_Y, 3);
			else
				var_Y = (var_Y - 16.0 / 116.0) / 7.787;
			if (pow(var_X, 3) > 0.008856)
				var_X = pow(var_X, 3);
			else
				var_X = (var_X - 16.0 / 116.0) / 7.787;
			if (pow(var_Z, 3) > 0.008856)
				var_Z = pow(var_Z, 3);
			else
				var_Z = (var_Z - 16.0 / 116.0) / 7.787;
			// Second step: XYZ -> RGB
			// Whitepoint D50
			var_X = 0.990720 * var_X;
			var_Y = 1.00000  * var_Y;
			var_Z = 0.825210 * var_Z;
			double var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
			double var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
			double var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;
			if (var_R > 0.0031308)
				var_R = 1.055 * (pow(var_R, (1.0 / 2.4))) - 0.055;
			else
				var_R = 12.92 * var_R;
			if (var_G > 0.0031308)
				var_G = 1.055 * (pow(var_G, (1.0 / 2.4))) - 0.055;
			else
				var_G = 12.92 * var_G;
			if (var_B > 0.0031308)
				var_B = 1.055 * (pow(var_B, (1.0 / 2.4))) - 0.055;
			else
				var_B = 12.92 * var_B;
			tmp = QColor(0, 0, 0, 0);
			var_R = qMax(qMin(var_R, 1.0), 0.0);
			var_G = qMax(qMin(var_G, 1.0), 0.0);
			var_B = qMax(qMin(var_B, 1.0), 0.0);
			tmp.setRgbF(var_R, var_G, var_B);
		}
	}
	return tmp;
}

QColor ScColorEngine::getDisplayColor(const ScColor& color, const ScribusDoc* doc, double level)
{
	QColor tmp;
	if (color.getColorModel() == colorModelRGB)
	{
		RGBColorF rgb;
		rgb.r = color.m_values[0];
		rgb.g = color.m_values[1];
		rgb.b = color.m_values[2];
		getShadeColorRGB(color, doc, rgb, level);
		tmp = getDisplayColor(rgb, doc, color.isSpotColor());
	}
	else if (color.getColorModel() == colorModelCMYK)
	{
		CMYKColorF cmyk;
		cmyk.c = color.m_values[0];
		cmyk.m = color.m_values[1];
		cmyk.y = color.m_values[2];
		cmyk.k = color.m_values[3];
		getShadeColorCMYK(color, doc, cmyk, level);
		tmp = getDisplayColor(cmyk, doc, color.isSpotColor());
	}
	else if (color.getColorModel() == colorModelLab)
	{
		ScColorTransform trans  = doc ? doc->stdLabToScreenTrans : ScCore->defaultLabToRGBTrans;
		double inC[3];
		inC[0] = 100 - (100 - color.m_L_val) * (level / 100.0);
		inC[1] = color.m_a_val * (level / 100.0);
		inC[2] = color.m_b_val * (level / 100.0);
		quint16 outC[3];
		trans.apply(inC, outC, 1);
		tmp = QColor(outC[0] / 257, outC[1] / 257, outC[2] / 257);
	}
	return tmp;
}

QColor ScColorEngine::getDisplayColorGC(const ScColor& color, const ScribusDoc* doc, bool *outOfG)
{
	QColor tmp;
	bool doSoftProofing = doc ? doc->SoftProofing : false;
	bool doGamutCheck   = doc ? doc->Gamut : false;
	if (doSoftProofing && doGamutCheck)
	{
		bool outOfGamutFlag = isOutOfGamut(color, doc);
		if (outOfG != NULL)
			*outOfG = outOfGamutFlag;
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
		if (color.m_values[0] == color.m_values[1] && color.m_values[1] == color.m_values[2])
			gamutChkEnabled = false;
		RGBColor rgb;
		rgb.r = qRound(color.m_values[0] * 255.0);
		rgb.g = qRound(color.m_values[1] * 255.0);
		rgb.b = qRound(color.m_values[2] * 255.0);
		tmp = getColorProof(rgb, doc, spot, gamutCheck & gamutChkEnabled);
	}
	else
	{
		CMYKColor cmyk;
		cmyk.c = qRound(color.m_values[0] * 255.0);
		cmyk.m = qRound(color.m_values[1] * 255.0);
		cmyk.y = qRound(color.m_values[2] * 255.0);
		cmyk.k = qRound(color.m_values[3] * 255.0);
		tmp = getColorProof(cmyk, doc, spot, gamutCheck & gamutChkEnabled);
	}
	return tmp;
}

QColor ScColorEngine::getShadeColor(const ScColor& color, const ScribusDoc* doc, double level)
{
	RGBColor rgb;
	rgb.r = qRound(color.m_values[0] * 255.0);
	rgb.g = qRound(color.m_values[1] * 255.0);
	rgb.b = qRound(color.m_values[2] * 255.0);
	getShadeColorRGB(color, doc, rgb, level);
	return QColor(rgb.r, rgb.g, rgb.b);
}

QColor ScColorEngine::getShadeColorProof(const ScColor& color, const ScribusDoc* doc, double level)
{
	QColor tmp;
	bool doGC = doc ? doc->Gamut : false;
	bool cmsUse = doc ? doc->HasCMS : false;
	bool softProof = doc ? doc->SoftProofing : false;

	if (color.getColorModel() == colorModelRGB)
	{
		RGBColorF rgb;
		rgb.r = color.m_values[0];
		rgb.g = color.m_values[1];
		rgb.b = color.m_values[2];
		getShadeColorRGB(color, doc, rgb, level);
		// Match 133x behavior for rgb grey until we are able to make rgb profiled output
		// (RGB greys map to cmyk greys)
		if ((cmsUse && softProof) && (rgb.r == rgb.g && rgb.g == rgb.b))
		{
			doGC = false;
			CMYKColorF cmyk;
			cmyk.c = cmyk.m = cmyk.y = 0;
			cmyk.k = 1.0 - rgb.g;
			tmp = getColorProof(cmyk, doc, color.isSpotColor(), doGC);
		}
		else
			tmp = getColorProof(rgb, doc, color.isSpotColor(), doGC);
	}
	else if (color.getColorModel() == colorModelCMYK)
	{
		CMYKColorF cmyk;
		cmyk.c = color.m_values[0];
		cmyk.m = color.m_values[1];
		cmyk.y = color.m_values[2];
		cmyk.k = color.m_values[3];
		getShadeColorCMYK(color, doc, cmyk, level);
		tmp = getColorProof(cmyk, doc, color.isSpotColor(), doGC);
	}
	else if (color.getColorModel() == colorModelLab)
	{
		double inC[3];
		inC[0] = 100 - (100 - color.m_L_val) * (level / 100.0);
		inC[1] = color.m_a_val * (level / 100.0);
		inC[2] = color.m_b_val * (level / 100.0);
		quint16 outC[3];
		ScColorTransform trans  = doc ? doc->stdLabToScreenTrans : ScCore->defaultLabToRGBTrans;
		ScColorTransform transProof   = doc ? doc->stdProofLab   : ScCore->defaultLabToRGBTrans;
		ScColorTransform transProofGC = doc ? doc->stdProofLabGC : ScCore->defaultLabToRGBTrans;
		if (cmsUse && doc && doc->SoftProofing)
		{
			ScColorTransform xform = doGC ? transProofGC : transProof;
			xform.apply(inC, outC, 1);
			tmp = QColor(outC[0] / 257, outC[1] / 257, outC[2] / 257);
		}
		else
		{
			trans.apply(inC, outC, 1);
			tmp = QColor(outC[0] / 257, outC[1] / 257, outC[2] / 257);
		}
	}
	
	return tmp;
}

QColor ScColorEngine::getColorProof(RGBColor& rgb, const ScribusDoc* doc, bool spot, bool gamutCkeck)
{
	unsigned short inC[4];
	unsigned short outC[4];
	int  r = rgb.r, g = rgb.g, b = rgb.b;
	ScColorTransform transRGBMon  = doc ? doc->stdTransRGBMon : ScCore->defaultRGBToScreenSolidTrans;
	ScColorTransform transProof   = doc ? doc->stdProof   : ScCore->defaultRGBToScreenSolidTrans;
	ScColorTransform transProofGC = doc ? doc->stdProofGC : ScCore->defaultRGBToScreenSolidTrans;
	bool cmsUse   = doc ? doc->HasCMS : false;
	bool cmsTrans = (transRGBMon && transProof && transProofGC); 
	if (ScCore->haveCMS() && cmsTrans)
	{
		inC[0] = rgb.r * 257;
		inC[1] = rgb.g * 257;
		inC[2] = rgb.b * 257;
		if (cmsUse && !spot && doc->SoftProofing)
		{
			ScColorTransform xform = gamutCkeck ? transProofGC : transProof;
			xform.apply(inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
		else
		{
			transRGBMon.apply(inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
	}
	return QColor(r, g, b);
}

QColor ScColorEngine::getColorProof(RGBColorF& rgb, const ScribusDoc* doc, bool spot, bool gamutCkeck)
{
	unsigned short inC[4];
	unsigned short outC[4];
	int r = qRound(rgb.r * 255.0);
	int g = qRound(rgb.g * 255.0);
	int b = qRound(rgb.b * 255.0);
	ScColorTransform transRGBMon  = doc ? doc->stdTransRGBMon : ScCore->defaultRGBToScreenSolidTrans;
	ScColorTransform transProof   = doc ? doc->stdProof   : ScCore->defaultRGBToScreenSolidTrans;
	ScColorTransform transProofGC = doc ? doc->stdProofGC : ScCore->defaultRGBToScreenSolidTrans;
	bool cmsUse   = doc ? doc->HasCMS : false;
	bool cmsTrans = (transRGBMon && transProof && transProofGC); 
	if (ScCore->haveCMS() && cmsTrans)
	{
		inC[0] = rgb.r * 65535.0;
		inC[1] = rgb.g * 65535.0;
		inC[2] = rgb.b * 65535.0;
		if (cmsUse && !spot && doc->SoftProofing)
		{
			ScColorTransform xform = gamutCkeck ? transProofGC : transProof;
			xform.apply(inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
		else
		{
			transRGBMon.apply(inC, outC, 1);
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
	unsigned short inC[4];
	unsigned short outC[4];
	ScColorTransform transCMYKMon     = doc ? doc->stdTransCMYKMon : ScCore->defaultCMYKToRGBTrans;
	ScColorTransform transProofCMYK   = doc ? doc->stdProofCMYK   : ScCore->defaultCMYKToRGBTrans;
	ScColorTransform transProofCMYKGC = doc ? doc->stdProofCMYKGC : ScCore->defaultCMYKToRGBTrans;
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
			ScColorTransform xform = gamutCkeck ? transProofCMYKGC : transProofCMYK;
			xform.apply(inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
		else
		{
			transCMYKMon.apply(inC, outC, 1);
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

QColor ScColorEngine::getColorProof(CMYKColorF& cmyk, const ScribusDoc* doc, bool spot, bool gamutCkeck)
{
	int  r = 0, g = 0, b = 0;
	unsigned short inC[4];
	unsigned short outC[4];
	ScColorTransform transCMYKMon     = doc ? doc->stdTransCMYKMon : ScCore->defaultCMYKToRGBTrans;
	ScColorTransform transProofCMYK   = doc ? doc->stdProofCMYK   : ScCore->defaultCMYKToRGBTrans;
	ScColorTransform transProofCMYKGC = doc ? doc->stdProofCMYKGC : ScCore->defaultCMYKToRGBTrans;
	bool cmsUse   = doc ? doc->HasCMS : false;
	bool cmsTrans = (transCMYKMon && transProofCMYK && transProofCMYKGC); 
	if (ScCore->haveCMS() && cmsTrans)
	{
		inC[0] = cmyk.c * 65535.0;
		inC[1] = cmyk.m * 65535.0;
		inC[2] = cmyk.y * 65535.0;
		inC[3] = cmyk.k * 65535.0;
		if (cmsUse && !spot && doc->SoftProofing)
		{
			ScColorTransform xform = gamutCkeck ? transProofCMYKGC : transProofCMYK;
			xform.apply(inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
		else
		{
			transCMYKMon.apply(inC, outC, 1);
			r = outC[0] / 257;
			g = outC[1] / 257;
			b = outC[2] / 257;
		}
	}
	else
	{
		r = 255 - qMin(255, qRound((cmyk.c + cmyk.k) * 255.0));
		g = 255 - qMin(255, qRound((cmyk.m + cmyk.k) * 255.0));
		b = 255 - qMin(255, qRound((cmyk.y + cmyk.k) * 255.0));
	}
	return QColor(r, g, b);
}

QColor ScColorEngine::getDisplayColor(RGBColor& rgb, const ScribusDoc* doc, bool spot)
{
	unsigned short inC[4];
	unsigned short outC[4];
	int r = rgb.r;
	int g = rgb.g;
	int b = rgb.b; 
	ScColorTransform transRGBMon = doc ? doc->stdTransRGBMon : ScCore->defaultRGBToScreenSolidTrans;
	if (ScCore->haveCMS() && transRGBMon)
	{
		inC[0] = r * 257;
		inC[1] = g * 257;
		inC[2] = b * 257;
		transRGBMon.apply(inC, outC, 1);
		r = outC[0] / 257;
		g = outC[1] / 257;
		b = outC[2] / 257;
	}
	return QColor(r, g, b);
}

QColor ScColorEngine::getDisplayColor(RGBColorF& rgb, const ScribusDoc* doc, bool spot)
{
	unsigned short inC[4];
	unsigned short outC[4];
	int r = rgb.r * 255.0;
	int g = rgb.g * 255.0;
	int b = rgb.b * 255.0; 
	ScColorTransform transRGBMon = doc ? doc->stdTransRGBMon : ScCore->defaultRGBToScreenSolidTrans;
	if (ScCore->haveCMS() && transRGBMon)
	{
		inC[0] = rgb.r * 65535.0;
		inC[1] = rgb.g * 65535.0;
		inC[2] = rgb.b * 65535.0;
		transRGBMon.apply(inC, outC, 1);
		r = outC[0] / 257;
		g = outC[1] / 257;
		b = outC[2] / 257;
	}
	return QColor(r, g, b);
}

QColor ScColorEngine::getDisplayColor(CMYKColor& cmyk, const ScribusDoc* doc, bool spot)
{
	int  r = 0, g = 0, b = 0;
	unsigned short inC[4];
	unsigned short outC[4];
	ScColorTransform transCMYKMon = doc ? doc->stdTransCMYKMon : ScCore->defaultCMYKToRGBTrans;
	if (ScCore->haveCMS() && transCMYKMon)
	{
		inC[0] = cmyk.c * 257;
		inC[1] = cmyk.m * 257;
		inC[2] = cmyk.y * 257;
		inC[3] = cmyk.k * 257;
		transCMYKMon.apply(inC, outC, 1);
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

QColor ScColorEngine::getDisplayColor(CMYKColorF& cmyk, const ScribusDoc* doc, bool spot)
{
	int  r = 0, g = 0, b = 0;
	unsigned short inC[4];
	unsigned short outC[4];
	ScColorTransform transCMYKMon = doc ? doc->stdTransCMYKMon : ScCore->defaultCMYKToRGBTrans;
	if (ScCore->haveCMS() && transCMYKMon)
	{
		inC[0] = cmyk.c * 65535.0;
		inC[1] = cmyk.m * 65535.0;
		inC[2] = cmyk.y * 65535.0;
		inC[3] = cmyk.k * 65535.0;
		transCMYKMon.apply(inC, outC, 1);
		r = outC[0] / 257;
		g = outC[1] / 257;
		b = outC[2] / 257;
	}
	else
	{
		r = 255 - qMin(255, qRound((cmyk.c + cmyk.k) * 255.0));
		g = 255 - qMin(255, qRound((cmyk.m + cmyk.k) * 255.0));
		b = 255 - qMin(255, qRound((cmyk.y + cmyk.k) * 255.0));
	}
	return QColor(r, g, b);
}

bool ScColorEngine::isOutOfGamut(const ScColor& color, const ScribusDoc* doc)
{
	bool outOfGamutFlag = false;
	if (color.isSpotColor())
		return false;
	unsigned short inC[4];
	unsigned short outC[4];
	bool cmsUse = doc ? doc->HasCMS : false;
	if (ScCore->haveCMS() && cmsUse)
	{
		bool alert = true;
		ScColorTransform xformProof;
		if (color.getColorModel() == colorModelRGB)
		{
			inC[0] = qRound(color.m_values[0] * 65535.0);
			inC[1] = qRound(color.m_values[1] * 65535.0);
			inC[2] = qRound(color.m_values[2] * 65535.0);
			xformProof = doc->stdProofGC;
			if ((color.m_values[0] == 0) && (color.m_values[1] == 0) && (color.m_values[2] == 1.0))
				alert = false;
			if ((color.m_values[0] == color.m_values[1] && color.m_values[1] == color.m_values[2]))
				alert = false;
		}
		else if (color.getColorModel() == colorModelCMYK)
		{
			inC[0] = qRound(color.m_values[0] * 65535.0);
			inC[1] = qRound(color.m_values[1] * 65535.0);
			inC[2] = qRound(color.m_values[2] * 65535.0);
			inC[3] = qRound(color.m_values[3] * 65535.0);
			xformProof = doc->stdProofCMYKGC;
			if ((color.m_values[0] == 1.0) && (color.m_values[1] == 0) && (color.m_values[3] == 0) && (color.m_values[2] == 1.0))
				alert = false;
			if ((color.m_values[0] == 0.0) && (color.m_values[1] == 0) && (color.m_values[2] == 0))
				alert = false;
			if ((color.m_values[0] == color.m_values[1]) && (color.m_values[0] == color.m_values[2]) && (color.m_values[2] == color.m_values[3]))
				alert = false;
		}
		else if (color.getColorModel() == colorModelLab)
		{
			double inC[3];
			inC[0] = color.m_L_val;
			inC[1] = color.m_a_val;
			inC[2] = color.m_b_val;
			xformProof = doc->stdProofLabGC;
			xformProof.apply(inC, outC, 1);
			if ((outC[0]/257 == 0) && (outC[1]/257 == 255) && (outC[2]/257 == 0))
				outOfGamutFlag = true;
			alert = false;
		}
		if (alert)
		{
			xformProof.apply(inC, outC, 1);
			if ((outC[0]/257 == 0) && (outC[1]/257 == 255) && (outC[2]/257 == 0))
				outOfGamutFlag = true;
		}
	}
	return outOfGamutFlag;
}

void ScColorEngine::applyGCR(ScColor& color, const ScribusDoc* doc)
{
	bool cmsUse = doc ? doc->HasCMS : false;
	if (!(ScCore->haveCMS() && cmsUse) && (color.getColorModel() != colorModelLab))
	{
		CMYKColor cmyk;
		getCMYKValues(color, doc, cmyk);
		int k = qMin(qMin(cmyk.c, cmyk.m), cmyk.y);
		color.m_values[0] = (cmyk.c - k) / 255.0;
		color.m_values[1] = (cmyk.m - k) / 255.0;
		color.m_values[2] = (cmyk.y - k) / 255.0;
		color.m_values[3] = qMin((cmyk.k + k) / 255.0, 1.0);
	}
}
