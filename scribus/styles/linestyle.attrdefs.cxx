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

ATTRDEF(double, width, Width, 0)
ATTRDEF(QString, color, Color, "Black")
ATTRDEF(double, shade, Shade, 100)
ATTRDEF(int, transparency, Transparency, 0)
ATTRDEF(int, blendMode, BlendMode, 0)
ATTRDEF(Qt::PenStyle, style, Style, Qt::SolidLine)
ATTRDEF(Qt::PenCapStyle, cap, Cap, Qt::FlatCap)
ATTRDEF(Qt::PenJoinStyle, join, Join, Qt::MiterJoin)
ATTRDEF(QList<double>, dashes, Dashes, QList<double>())
ATTRDEF(double, dashOffset, DashOffset, 0)
ATTRDEF(int, startArrow, StartArrow, 0)
ATTRDEF(int, endArrow, EndArrow, 0)
ATTRDEF(QList<LineStyle>, sublines, Sublines, QList<LineStyle>())
