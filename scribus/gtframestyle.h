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

#ifndef GTFRAMESTYLE_H
#define GTFRAMESTYLE_H

#include "scribusapi.h"
#include "gtparagraphstyle.h"

class SCRIBUS_API gtFrameStyle : public gtParagraphStyle
{
public:
	gtFrameStyle(const QString& name);
	gtFrameStyle(const gtFrameStyle& f);
	gtFrameStyle(const gtParagraphStyle& p);
	gtFrameStyle(const gtStyle& s);
	~gtFrameStyle() {};
	
	QString target() const override;

	gtParagraphStyle& asGtParagraphStyle() { return dynamic_cast<gtParagraphStyle&>(*this); }
	const gtParagraphStyle& asGtParagraphStyle() const { return dynamic_cast<const gtParagraphStyle&>(*this); }

	int getColumns() const;
	void setColumns(int newColumns);
	double getColumnsGap() const;
	void setColumnsGap(double newColumnsGap);
	QString getBgColor() const;
	void setBgColor(const QString& newBgColor);
	int getBgShade() const;
	void setBgShade(int newBgShade);
	void getParagraphStyle(gtParagraphStyle* pstyle);

private:
	int     m_columns { 1 };
	double  m_columnsGap { 0.0 };
	QString m_bgColor { "White" };
	int     m_bgShade { 100 };

	void init();
};

#endif // GTFRAMESTYLE_H
