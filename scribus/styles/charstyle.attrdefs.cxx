/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


//Syntax: ATTRDEF( datatype, gettername, name, defaultvalue)

ATTRDEF(double, fontSize, FontSize, 200, true)
ATTRDEF(double, fillShade, FillShade, 100, false)
ATTRDEF(double, strokeShade, StrokeShade, 100, false)
ATTRDEF(QString, fontFeatures, FontFeatures, "", false)
ATTRDEF(double, scaleH, ScaleH, 1000, true)
ATTRDEF(double, scaleV, ScaleV, 1000, true)
ATTRDEF(double, baselineOffset, BaselineOffset, 0, true)
ATTRDEF(double, shadowXOffset, ShadowXOffset, 0, false)
ATTRDEF(double, shadowYOffset, ShadowYOffset, 0, false)
ATTRDEF(double, outlineWidth, OutlineWidth, 0, false)
ATTRDEF(double, underlineOffset, UnderlineOffset, 0, false)
ATTRDEF(double, underlineWidth, UnderlineWidth, 0, false)
ATTRDEF(double, strikethruOffset, StrikethruOffset, 0, false)
ATTRDEF(double, strikethruWidth, StrikethruWidth, 0, false)
ATTRDEF(double, tracking, Tracking, 0, false)
ATTRDEF(double, wordTracking, WordTracking, 1.0, false)
ATTRDEF(QString, fillColor, FillColor, "None", false)
ATTRDEF(QString, strokeColor, StrokeColor, "Black", false)
ATTRDEF(QString, backColor, BackColor, "None", false)
ATTRDEF(double, backShade, BackShade, 100, false)
ATTRDEF(QString, language, Language, "", true)
ATTRDEF(QString, fontVariant, FontVariant, "", true)
ATTRDEF(ScFace, font, Font, ScFace::none(), true)
ATTRDEF(QStringList, features, Features, QStringList(CharStyle::INHERIT), false)
ATTRDEF(int, hyphenWordMin, HyphenWordMin, 3, false)
ATTRDEF(uint, hyphenChar, HyphenChar, 0x2010, false)
