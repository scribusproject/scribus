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

#include "gtstyle.h"

gtStyle::gtStyle(QString name)
{
	this->name = name;
}

gtStyle::gtStyle(const gtStyle& s)
{
	name = QString(s.name);
	font = gtFont(s.font);
}

void gtStyle::setFont(gtFont newFont)
{
	font = newFont;
}

gtFont* gtStyle::getFont()
{
	return &font;
}

QString gtStyle::getName()
{
	return name;
}

void gtStyle::setName(QString newName)
{
	name = newName;
}

QString gtStyle::target()
{
	return QString("character");
}

gtStyle::~gtStyle()
{

}
