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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "gtframestyle.h"
#include "scribusstructs.h"

gtFrameStyle::gtFrameStyle(QString name) : gtParagraphStyle(name) 
{
	init();
}

gtFrameStyle::gtFrameStyle(const gtFrameStyle& f) : gtParagraphStyle(f)
{
	m_columns    = f.m_columns;
	m_columnsGap = f.m_columnsGap;
	m_bgColor    = f.m_bgColor;
	m_bgShade    = f.m_bgShade;
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
	m_columns    = 1;
	m_columnsGap = 0;
	m_bgColor    = "White";
	m_bgShade    = 100;
}

int gtFrameStyle::getColumns()
{
	return m_columns;
}

void gtFrameStyle::setColumns(int newColumns)
{
	m_columns = newColumns;
}

double gtFrameStyle::getColumnsGap()
{
	return m_columnsGap;
}

void gtFrameStyle::setColumnsGap(double newColumnsGap)
{
	m_columnsGap = newColumnsGap;
}

QString gtFrameStyle::getBgColor()
{
	return m_bgColor;
}

void gtFrameStyle::setBgColor(QString newBgColor)
{
	m_bgColor = newBgColor;
}

int gtFrameStyle::getBgShade()
{
	return m_bgShade;
}

void gtFrameStyle::setBgShade(int newBgShade)
{
	m_bgShade = newBgShade;
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
