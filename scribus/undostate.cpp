/***************************************************************************
 *   Copyright (C) 2005 by Riku Leino                                      *
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

#include "undostate.h"

UndoState::UndoState(const QString& name, const QString& description, QPixmap* pixmap)
{
	actionName        = name;
	actionDescription = description;
	actionPixmap      = pixmap;
}

QString UndoState::getName()
{
	return actionName;
}

QString UndoState::getDescription()
{
	return actionDescription;
}

QPixmap* UndoState::getPixmap()
{
	return actionPixmap;	
}

UndoState::~UndoState()
{
	
}

/*** SimpleState **************************************************************/

SimpleState::SimpleState(const QString& name, const QString& description, QPixmap* pixmap)
: UndoState(name, description, pixmap)
{

}

bool SimpleState::contains(const QString& key)
{
	return values.contains(key);
}

QString SimpleState::get(const QString& key, const QString& def)
{
	if (values.contains(key))
		return values[key];

	values[key] = def;
	return values[key];
}

int SimpleState::getInt(const QString& key, int def)
{
	bool ok = false;
	QString retString = get(key, QString("%1").arg(def));
	int ret = retString.toInt(&ok);
	if (!ok)
		ret = def;
	return ret;
}

double SimpleState::getDouble(const QString& key, double def)
{
	bool ok = false;
	QString retString = get(key, QString("%1").arg(def));
	double ret = retString.toDouble(&ok);
	if (!ok)
		ret = def;
	return ret;
}

void SimpleState::set(const QString& key, const QString& value)
{
	values[key] = value;
}

void SimpleState::set(const QString& key, int value)
{
	values[key] = QString("%1").arg(value);
}

void SimpleState::set(const QString& key, double value)
{
	values[key] = QString("%1").arg(value, 0, 'f', 20);
}


SimpleState::~SimpleState() 
{

}
