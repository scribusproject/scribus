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

#ifndef GTFRAMESTYLE_H
#define GTFRAMESTYLE_H

#include "scribusapi.h"
#include "gtparagraphstyle.h"

class SCRIBUS_API gtFrameStyle : public gtParagraphStyle
{
private:
	int     columns;
	double  columnsGap;
	QString bgColor;
	int     bgShade;
	void init();
public:
	gtFrameStyle(QString name);
	gtFrameStyle(const gtFrameStyle& f);
	gtFrameStyle(const gtParagraphStyle& p);
	gtFrameStyle(const gtStyle& s);
	~gtFrameStyle();
	int getColumns();
	void setColumns(int newColumns);
	double getColumnsGap();
	void setColumnsGap(double newColumnsGap);
	QString getBgColor();
	void setBgColor(QString newBgColor);
	int getBgShade();
	void setBgShade(int newBgShade);
	void getParagraphStyle(gtParagraphStyle* pstyle);
	QString target();
};

#endif // GTFRAMESTYLE_H
