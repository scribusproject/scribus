/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "gtframestyle.h"
#include "scribusstructs.h"

gtFrameStyle::gtFrameStyle(QString name) : gtParagraphStyle(name) 
{
	init();
}

gtFrameStyle::gtFrameStyle(const gtFrameStyle& f) : gtParagraphStyle(f)
{
	columns    = f.columns;
	columnsGap = f.columnsGap;
	bgColor    = f.bgColor;
	bgShade    = f.bgShade;
}

gtFrameStyle::gtFrameStyle(const gtParagraphStyle& p) : gtParagraphStyle(p)
{
	init();
}

gtFrameStyle::gtFrameStyle(const gtStyle& s) : gtParagraphStyle(s)
{
	init();
}

void gtFrameStyle::init()
{
	columns    = 1;
	columnsGap = 0;
	bgColor    = "White";
	bgShade    = 100;
}

int gtFrameStyle::getColumns()
{
	return columns;
}

void gtFrameStyle::setColumns(int newColumns)
{
	columns = newColumns;
}

double gtFrameStyle::getColumnsGap()
{
	return columnsGap;
}

void gtFrameStyle::setColumnsGap(double newColumnsGap)
{
	columnsGap = newColumnsGap;
}

QString gtFrameStyle::getBgColor()
{
	return bgColor;
}

void gtFrameStyle::setBgColor(QString newBgColor)
{
	bgColor = newBgColor;
}

int gtFrameStyle::getBgShade()
{
	return bgShade;
}

void gtFrameStyle::setBgShade(int newBgShade)
{
	bgShade = newBgShade;
}

QString gtFrameStyle::target()
{
	return QString("frame");
}

void gtFrameStyle::getParagraphStyle(gtParagraphStyle* pstyle)
{
	*pstyle = gtParagraphStyle(*this);
}

gtFrameStyle::~gtFrameStyle()
{

}
