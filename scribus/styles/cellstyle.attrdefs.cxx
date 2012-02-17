/*
 Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

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

ATTRDEF(QString, fillColor, FillColor, CommonStrings::None)
ATTRDEF(double, fillShade, FillShade, 100)
ATTRDEF(TableBorder, leftBorder, LeftBorder, TableBorder())
ATTRDEF(TableBorder, rightBorder, RightBorder, TableBorder())
ATTRDEF(TableBorder, topBorder, TopBorder, TableBorder())
ATTRDEF(TableBorder, bottomBorder, BottomBorder, TableBorder())
ATTRDEF(double, leftPadding, LeftPadding, 0.0)
ATTRDEF(double, rightPadding, RightPadding, 0.0)
ATTRDEF(double, topPadding, TopPadding, 0.0)
ATTRDEF(double, bottomPadding, BottomPadding, 0.0)
