/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2005 by Riku Leino                                      *
 *   riku@scribus.info                                                     *
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
#include "undoobject.h"

UndoState::UndoState(const QString& name, const QString& description, QPixmap* pixmap) :
actionName_(name),
actionDescription_(description),
actionPixmap_(pixmap),
undoObject_(0), transactionCode(0)
{

}

QString UndoState::getName()
{
	return actionName_;
}

void UndoState::setName(const QString &newName)
{
	actionName_ = newName;
}

QString UndoState::getDescription()
{
	return actionDescription_;
}

void UndoState::setDescription(const QString &newDescription)
{
	actionDescription_ = newDescription;
}

QPixmap* UndoState::getPixmap()
{
	return actionPixmap_;
}

void UndoState::setPixmap(QPixmap *pixmap)
{
	actionPixmap_ = pixmap;
}

void UndoState::undo()
{
	if (undoObject_) // if !undoObject_ there's an error, hmmm
		undoObject_->restore(this, true);
}

void UndoState::redo()
{
	if (undoObject_)
		undoObject_->restore(this, false);
}

void UndoState::setUndoObject(UndoObject *object)
{
	undoObject_ = object->undoObjectPtr();
}

UndoObject* UndoState::undoObject()
{
	return undoObject_;
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
	return values_.contains(key);
}

QString SimpleState::get(const QString& key, const QString& def)
{
	if (values_.contains(key))
		return values_[key];

	values_[key] = def;
	return values_[key];
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

uint SimpleState::getUInt(const QString& key, uint def)
{
	bool ok = false;
	QString retString = get(key, QString("%1").arg(def));
	uint ret = retString.toUInt(&ok);
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

bool SimpleState::getBool(const QString& key, bool def)
{
	bool ok = false;
	QString retString = get(key, QString("%1").arg(def));
	int ret = retString.toInt(&ok);
	if (!ok)
		ret = def;
	return ret;
}

void SimpleState::set(const QString& key, const QString& value)
{
	values_[key] = value;
}

void SimpleState::set(const QString& key, int value)
{
	values_[key] = QString("%1").arg(value);
}

void SimpleState::set(const QString& key, uint value)
{
	values_[key] = QString("%1").arg(value);
}

void SimpleState::set(const QString& key, double value)
{
	values_[key] = QString("%1").arg(value, 0, 'f', 20);
}

void SimpleState::set(const QString& key, bool value)
{
	values_[key] = QString("%1").arg(value);
}


SimpleState::~SimpleState()
{

}
