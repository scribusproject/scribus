/***************************************************************************
                          cmykcolor.h  -  description
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

#ifndef CMYKCOLOR_H
#define CMYKCOLOR_H
#include <qcolor.h>
#include <qstring.h>

/**
  *@author Franz Schmid
  * This Class adds support for CMYK-Colors to QT.
  * its API is based on the API of QColor
  */

class CMYKColor 
{
public:
/** Constructs a CMYKColor with 4 Components
 * in the range from 0 - 255 */
	CMYKColor(int c=0, int m=0, int y=0, int k=0);
	~CMYKColor() {};
	
/** Same as the Constructor but for an existing Color */
	void setColor(int c, int m, int y, int k);
	
/** Computes a CMYKColor from an RGB-Color
 * Applies Gray-Component-Removal to the resulting CMYKColor */
	void setColorRGB(int r, int g, int b);
	
/** Computes a CMYKColor for a QColor */
	void fromQColor(QColor f);
	
/** Returns the RGB-Part of an CMYKColor */
	QColor getRGBColor();
	void getRawRGBColor(int *r, int *g, int *b);
	
/** Returns the 4 Values that form an CMYKColor */
	void getCMYK(int *c, int *m, int *y, int *k);
	
/** Applys Gray-Component-Removal to an CMYKColor */
	void applyGCR();
	
/** Returns the CMYKColor as an Hex-String in the Form #CCYYMMKK */
	QString name();
	
/** Sets the Values of an CMYKColor from an Hex-String in the Form #CCMMYYKK */
	void setNamedColor(QString nam);
	void RecalcRGB();
	
private:
/** Cyan-Component of Color */
	int C;
	
/** Magenta-Component of Color */
	int M;
	
/** Yellow-Component of Color */
	int Y;
	
/** Black-Component of Color */
	int K;
	
/** RGB-Equivalent of the CMYK-Values */
	QColor RGB;
	
/** Flag, true if the Color is a Spotcolor */
	bool Spot;
};

#endif
