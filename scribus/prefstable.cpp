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

#include "prefstable.h"

PrefsTable::PrefsTable(QString tableName)
{
	name = tableName;
	rowCount = 0;
	colCount = 0;
}

QString PrefsTable::getName()
{
	return name;
}

int PrefsTable::height()
{
	return rowCount;
}

int PrefsTable::getRowCount()
{
	return rowCount;
}

int PrefsTable::width()
{
	return colCount;
}

int PrefsTable::getColCount()
{
	return colCount;
}

QString PrefsTable::get(int row, int col, const QString& defValue)
{
	checkSize(row, col, defValue);
	if ((*table[row])[col] == "__NOT__SET__")
		table[row]->insert(table[row]->begin()+col, defValue);

	return (*table[row])[col];
}

void PrefsTable::set(int row, int col, const char* value)
{
	set(row, col, QString(value));
}

void PrefsTable::set(int row, int col, const std::string& value)
{
	set(row, col, QString(value.c_str()));
}

void PrefsTable::set(int row, int col, const QString& value)
{
	checkSize(row, col, "__NOT__SET__");
	table[row]->insert(table[row]->begin()+col, value);
}

int PrefsTable::getInt(int row, int col, int defValue)
{
	QString stringValue = get(row, col, QString("%1").arg(defValue));
	bool ok = false;
	int ivalue = stringValue.toInt(&ok);
	if (!ok)
		ivalue = defValue;
	return ivalue;
}

void PrefsTable::set(int row, int col, int value)
{
	set(row, col, QString("%1").arg(value));
}

void PrefsTable::set(int row, int col, uint value)
{
	set(row, col, QString("%1").arg(value));
}

uint PrefsTable::getUInt(int row, int col, uint defValue)
{
	QString stringValue = get(row, col, QString("%1").arg(defValue));
	bool ok = false;
	int uivalue = stringValue.toUInt(&ok);
	if (!ok)
		uivalue = defValue;
	return uivalue;
}

double PrefsTable::getDouble(int row, int col, double defValue)
{
	QString stringValue = get(row, col, QString("%1").arg(defValue));
	bool ok = false;
	double dvalue = stringValue.toDouble(&ok);
	if (!ok)
		dvalue = defValue;
	return dvalue;
}

void PrefsTable::set(int row, int col, double value)
{
	set(row, col, QString("%1").arg(value));
}

bool PrefsTable::getBool(int row, int col, bool defValue)
{
	QString stringValue = get(row, col, QString("%1").arg(defValue));
	bool ok = false;
	int ivalue = stringValue.toInt(&ok);
	if (!ok)
		ivalue = defValue;
	return ivalue;
}

void PrefsTable::set(int row, int col, bool value)
{
	set(row, col, QString("%1").arg(value));
}

int PrefsTable::find(int searchCol, const QString& what)
{
	if ((searchCol < 0) || (searchCol >= width()))
		return -1;

	int rowi = -1;
	for (int i = 0; i < height(); ++i)
	{
		if ((get(i, searchCol, "__NOT__SET__") == what) &&
		    (get(i, searchCol, "__NOT__SET__") != "__NOT__SET__"))
		{
			rowi = i;
			break;
		}
	}

	return rowi;
}

void PrefsTable::removeRow(int colIndex, const QString& what)
{
	if ((colIndex < 0) || (colIndex >= width()))
		return;

	Table::iterator it = table.begin();
	for (int i = 0; i < height(); ++i)
	{
		if (get(i, colIndex, "__NOT__SET__") == what)
		{
			it = table.erase(it);
			--rowCount;
		}
		else {
			++it;
		}
	}
}

void PrefsTable::checkSize(int rowIndex, int colIndex, QString defValue)
{
	checkHeight(rowIndex);
	checkWidth(rowIndex, colIndex, defValue);
}

void PrefsTable::checkHeight(int rowIndex)
{
	if (rowCount < (rowIndex + 1))
	{
		for (int i = 0; i < ((rowIndex + 1) - rowCount); ++i)
			table.push_back(new QStringList());
		rowCount = rowIndex + 1;
	}
}

void PrefsTable::checkWidth(int rowIndex, int colIndex, QString defValue)
{
	if (static_cast<int>(table[rowIndex]->size()) <= (colIndex + 1))
	{
		for (int i = 0; i < ((colIndex + 1) - static_cast<int>(table[rowIndex]->size())); ++i)
		{
			if (i == colIndex - static_cast<int>(table[rowIndex]->size()))
				table[rowIndex]->push_back(defValue);
			else
				table[rowIndex]->push_back("__NOT__SET__");
		}
		colCount = colIndex + 1;
	}
}

void PrefsTable::clear()
{
	rowCount = 0;
	colCount = 0;
	table.clear();
}

PrefsTable::~PrefsTable()
{
}
