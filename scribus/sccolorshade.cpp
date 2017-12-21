/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          sccolorshade.cpp  -  description
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

#include "sccolorshade.h"
#include "sccolorengine.h"

ScColorShade::ScColorShade(void)
{
	color.setColor( 0, 0, 0, 0 );
	shade = 100;
}

ScColorShade::ScColorShade( const QColor& c, int level )
{
	color = ScColor(c.red(), c.green(), c.blue());
	shade = level;
}

ScColorShade::ScColorShade( const ScColor& c, int level )
{
	color = c;
	shade = level;
}

ScColor ScColorShade::getShadedColor(void)
{
	ScColor value;
	if (color.getColorModel() == colorModelRGB)
	{
		RGBColorF rgb;
		ScColorEngine::getShadeColorRGB(color, NULL, rgb, shade);
		value.setRgbColorF(rgb.r, rgb.g, rgb.b);
	}
	else if (color.getColorModel() == colorModelCMYK)
	{
		CMYKColorF cmyk;
		ScColorEngine::getShadeColorCMYK(color, NULL, cmyk, shade);
		value.setColorF(cmyk.c, cmyk.m, cmyk.y, cmyk.k);
	}
	else if (color.getColorModel() == colorModelLab)
	{
		double L, a, b;
		color.getLab(&L, &a, &b);
		value.setLabColor(L * (shade / 100.0), a, b);
	}
	return value;
}
