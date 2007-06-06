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

ATTRDEF(double, fontSize, FontSize, 200)
ATTRDEF(double, fillShade, FillShade, 100)
ATTRDEF(double, strokeShade, StrokeShade, 100)
ATTRDEF(double, scaleH, ScaleH, 1000)
ATTRDEF(double, scaleV, ScaleV, 1000)
ATTRDEF(double, baselineOffset, BaselineOffset, 0)
ATTRDEF(double, shadowXOffset, ShadowXOffset, 0)
ATTRDEF(double, shadowYOffset, ShadowYOffset, 0)
ATTRDEF(double, outlineWidth, OutlineWidth, 0)
ATTRDEF(double, underlineOffset, UnderlineOffset, 0)
ATTRDEF(double, underlineWidth, UnderlineWidth, 0)
ATTRDEF(double, strikethruOffset, StrikethruOffset, 0)
ATTRDEF(double, strikethruWidth, StrikethruWidth, 0)
ATTRDEF(double, tracking, Tracking, 0)
ATTRDEF(double, wordTracking, WordTracking, 1.0)
ATTRDEF(QString, fillColor, FillColor, "undef")
ATTRDEF(QString, strokeColor, StrokeColor, "Black")
ATTRDEF(QString, language, Language, "")
ATTRDEF(QString, fontVariant, FontVariant, "")
ATTRDEF(ScFace, font, Font, ScFace::none())
ATTRDEF(QStringList, features, Features, QStringList(CharStyle::INHERIT))
