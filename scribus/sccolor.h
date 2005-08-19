/***************************************************************************
                          sccolor.h  -  description
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

#ifndef SCCOLOR_H
#define SCCOLOR_H

#include "scribusapi.h"

#include <qcolor.h>
class QString;

/**
  *@author Franz Schmid
  * This Class adds support for CMYK-Colors to QT.
  * its API is based on the API of QColor
  */

/** Scribus color models */
enum colorModel
{
	colorModelRGB,
	colorModelCMYK
};

class SCRIBUS_API ScColor 
{
public:
	ScColor();
/** Constructs a ScColor with 4 Components
 * in the range from 0 - 255 */
	ScColor(int c, int m, int y, int k);
/** Constructs a RGBColor with 3 Components
 * in the range from 0 - 255 */
	ScColor(int r, int g, int b);
	~ScColor() {};
/** flag to enable and disable use of color management (default: true) */
	static bool UseProf;

/** Same as the Constructor but for an existing Color */
	void setColor(int c, int m, int y, int k);

/** Computes a ScColor from an RGB-Color
 * Applies Gray-Component-Removal to the resulting ScColor
 * or if color management is enabled, an approriate transform */
	void setColorRGB(int r, int g, int b);

/** set the color model */
	void setColorModel (colorModel cm);

/** get the color model */
	colorModel getColorModel ();

/** Computes a ScColor for a QColor */
	void fromQColor(QColor color);

/** Returns the transformed/proofed RGB color  */
	QColor getRGBColor();
	void getRawRGBColor(int *r, int *g, int *b);

/** Returns the 4 Values that form an ScColor */
	void getCMYK(int *c, int *m, int *y, int *k);

/** Returns the 3 Values that form an RGBColor */
	void getRGB(int *r, int *g, int *b);

/** get CMYK values of a specified shade */
	void getShadeColorCMYK(int *c, int *m, int *y, int *k, int level);

/** get RGB values of a specified shade */
	void getShadeColorRGB(int *r, int *g, int *b, int level);

/** Return a proofed QColor with the specified shade */
	QColor getShadeColorProof(int level);

/** Applys Gray-Component-Removal to an ScColor */
	void applyGCR();

/** Returns the ScColor as an Hex-String in the Form #CCYYMMKK for
 * a CMYK color or ##RRGGBB for a RGB color*/
	QString name();

/** Sets the Values of a color from an Hex-String in the Form #CCMMYYKK
 * or #RRGGBB */
	void setNamedColor(QString nam);
	void RecalcRGB();
	bool isOutOfGamut();

private:
/** Cyan-Component of Color */
	int C;
	
/** Magenta-Component of Color */
	int M;

/** Yellow-Component of Color */
	int Y;

/** Black-Component of Color */
	int K;

/** Red-Component of Color */
	int R;

/** Green-Component of Color */
	int G;

/** Blue-Component of Color */
	int B;

/** RGB-Equivalent including color transforms */
	QColor RGB;

/** Flag, true if the Color is a Spotcolor */
	bool Spot;

/** Color model of the current color */
	colorModel Model;

/** Flag, true if out of Gamut */
	bool outOfGamutFlag;
};

#endif
