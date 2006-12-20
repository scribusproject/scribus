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
	if( color.getColorModel() == colorModelRGB )
	{
		RGBColor rgb;
		ScColorEngine::getShadeColorRGB(color, NULL, rgb, shade);
		value.setColorRGB( rgb.r, rgb.g, rgb.b );
	}
	else
	{
		CMYKColor cmyk;
		ScColorEngine::getShadeColorCMYK(color, NULL, cmyk, shade);
		value.setColor( cmyk.c, cmyk.m, cmyk.y, cmyk.k );
	}
	return value;
}
