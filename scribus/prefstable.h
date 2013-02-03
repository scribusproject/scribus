/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   riku.leino@gmail.com                                                      *
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

#ifndef PREFSTABLE_H
#define PREFSTABLE_H

#include <string>
#include <vector>
#include <QMap>
#include <QString>
#include <QStringList>

#include "scribusapi.h"

typedef std::vector<QStringList*> Table;

class SCRIBUS_API PrefsTable
{
private:
 QString name;
 Table table;
 int rowCount;
 int colCount;
 void checkSize(int rowIndex, int colIndex, QString defValue = "");
 void checkHeight(int rowIndex);
 void checkWidth(int rowIndex, int colIndex, QString defValue = "");
public:
 PrefsTable(QString tableName);
 ~PrefsTable();
 QString getName();
 int     height();
 int     getRowCount();
 int     width();
 int     getColCount();
 QString get(int row, int col, const QString& defValue = "");
 void    set(int row, int col, const char* value);
 void    set(int row, int col, const std::string& value);
 void    set(int row, int col, const QString& value);
 int     getInt(int row, int col = 0, int defValue = -1);
 void    set(int row, int col, int value);
 void    set(int row, int col, uint value);
 uint    getUInt(int row, int col, uint defValue = 0);
 double  getDouble(int row, int col, double defValue = -1.0);
 void    set(int row, int col, double value);
 bool    getBool(int row, int col, bool defValue = false);
 void    set(int row, int col, bool value);
/*** finds what in searchCol and returns the row index if found if not will return -1 */
 int  find(int searchCol, const QString& what);
/*** Removes the row where in column colIndex can be found text what ********/
 void removeRow(int colIndex, const QString& what);
/*** Empties the table ***/
 void clear();
};

#endif
