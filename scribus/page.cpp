/***************************************************************************
                          page.cpp  -  description
                             -------------------
    begin                : Sat Apr 7 2001
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

#include "page.h"

Page::Page(double x, double y, double b, double h)
{
	Xoffset = x;
	Yoffset = y;
	Width = b;
	Height = h;
	Margins.Top = 9;
	Margins.Left = 9;
	Margins.Right = 9;
	Margins.Bottom = 40;
	LeftPg = false;
	PageNam = "";
	XGuides.clear();
	YGuides.clear();
	FromMaster.clear();
}
