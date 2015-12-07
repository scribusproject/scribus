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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "undostate.h"
#include "undoobject.h"

UndoState::UndoState(const QString& name, const QString& description, QPixmap* pixmap) :
transactionCode(0),
m_actionName_(name),
m_actionDescription_(description),
m_actionPixmap_(pixmap),
m_undoObject_(0)
{

}

QString UndoState::getName()
{
	return m_actionName_;
}

void UndoState::setName(const QString &newName)
{
	m_actionName_ = newName;
}

QString UndoState::getDescription()
{
	return m_actionDescription_;
}

void UndoState::setDescription(const QString &newDescription)
{
	m_actionDescription_ = newDescription;
}

QPixmap* UndoState::getPixmap()
{
	return m_actionPixmap_;
}

void UndoState::setPixmap(QPixmap *pixmap)
{
	m_actionPixmap_ = pixmap;
}

void UndoState::undo()
{
	if (m_undoObject_) // if !undoObject_ there's an error, hmmm
		m_undoObject_->restore(this, true);
}

void UndoState::redo()
{
	if (m_undoObject_)
		m_undoObject_->restore(this, false);
}

void UndoState::setUndoObject(UndoObject *object)
{
	m_undoObject_ = object->undoObjectPtr();
}

UndoObject* UndoState::undoObject()
{
	return m_undoObject_;
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

QVariant SimpleState::variant(const QString& key, const QVariant& def)
{
	QMap<QString, QVariant>::const_iterator it = values_.find(key);
	if (it != values_.end())
		return it.value();

	values_[key] = def;
	return def;
}

QString SimpleState::get(const QString& key, const QString& def)
{
	QMap<QString, QVariant>::const_iterator it = values_.find(key);
	if (it != values_.end())
		return it.value().toString();

	values_[key] = def;
	return def;
}

int SimpleState::getInt(const QString& key, int def)
{
	bool ok = false;
	QVariant retVar = variant(key, QVariant(def));
	int ret = retVar.toInt(&ok);
	if (!ok)
		ret = def;
	return ret;
}

uint SimpleState::getUInt(const QString& key, uint def)
{
	bool ok = false;
	QVariant retVar = variant(key, QVariant(def));
	uint ret = retVar.toUInt(&ok);
	if (!ok)
		ret = def;
	return ret;
}

double SimpleState::getDouble(const QString& key, double def)
{
	bool ok = false;
	QVariant retVar = variant(key, QVariant(def));
	double ret = retVar.toDouble(&ok);
	if (!ok)
		ret = def;
	return ret;
}

bool SimpleState::getBool(const QString& key, bool def)
{
	bool ok = false;
	QVariant retVar = variant(key, QVariant(def));
	int ret = retVar.toInt(&ok);
	if (!ok)
		ret = def;
	return ret;
}

void SimpleState::set(const QString& key, const QString& value)
{
	values_[key] = QVariant(value);
}

void SimpleState::set(const QString& key, int value)
{
	values_[key] = QVariant(value);
}

void SimpleState::set(const QString& key, uint value)
{
	values_[key] = QVariant(value);
}

void SimpleState::set(const QString& key, double value)
{
	values_[key] = QVariant(value);
}

void SimpleState::set(const QString& key, bool value)
{
	values_[key] = QVariant(value);
}


SimpleState::~SimpleState()
{

}

/*** TransactionState *****************************************************/

TransactionState::TransactionState() : UndoState("")
{
	m_size_ = 0;
}

UndoState* TransactionState::at(int index) const
{
	if (index >= 0 && static_cast<uint>(index) < sizet())
		return m_states_[index];
	else
		return 0;
}

UndoState* TransactionState::last() const
{
	if (m_states_.size() > 0)
		return m_states_.at(m_size_ - 1);
	return 0;
}

bool TransactionState::contains(int uid) const
{
	for (uint i = 0; i < m_states_.size(); ++i)
	{
		UndoObject* undoObject = m_states_[i]->undoObject();
		if (undoObject && undoObject->getUId() == static_cast<uint>(uid))
			return true;
	}
	return false;
}

bool TransactionState::containsOnly(int uid) const
{
	for (uint i = 0; i < m_states_.size(); ++i)
	{
		UndoObject* undoObject = m_states_[i]->undoObject();
		if (undoObject && undoObject->getUId() != static_cast<uint>(uid))
			return false;
	}
	return true;
}

void TransactionState::pushBack(UndoObject *target, UndoState *state)
{
	if (target && state)
	{
		state->setUndoObject(target);
		m_states_.push_back(state);
		++m_size_;
	}
}

uint TransactionState::sizet() const
{
	return m_size_;
}

void TransactionState::useActionName()
{
	if (m_size_ > 0)
		setName(m_states_[m_size_ - 1]->getName());
}

UndoObject* TransactionState::replace(ulong uid, UndoObject *newUndoObject)
{
	UndoObject *tmp = 0;
	for (uint i = 0; i < m_states_.size(); ++i)
	{
		TransactionState *ts = dynamic_cast<TransactionState*>(m_states_[i]);
		if (ts) // are we having a transaction_inside a transaction
			ts->replace(uid, newUndoObject);
		else if (m_states_[i]->undoObject() && m_states_[i]->undoObject()->getUId() == uid)
		{
			tmp = m_states_[i]->undoObject();
			m_states_[i]->setUndoObject(newUndoObject);
		}
	}
	return tmp;
}

void TransactionState::undo() // undo all attached states
{
	for (int i = sizet() - 1; i > -1; --i)
	{
		if ((sizet() - 1) == 0)
			at(i)->transactionCode = 0;
		else
		{
			if (i == static_cast<int>(sizet() - 1))
				at(i)->transactionCode = 1;
			else if (i == 0)
				at(i)->transactionCode = 2;
			else
				at(i)->transactionCode = 3;
		}
		if (transactionCode != 0)
			at(i)->transactionCode = transactionCode;
		at(i)->undo();
	}
}

void TransactionState::redo() // redo all attached states
{
	for (uint i = 0; i < sizet(); ++i)
	{
		if ((sizet() - 1) == 0)
			at(i)->transactionCode = 0;
		else
		{
			if (i == 0)
				at(i)->transactionCode = 1;
			else if (i == static_cast<uint>(sizet() - 1))
				at(i)->transactionCode = 2;
			else
				at(i)->transactionCode = 3;
		}
		if (transactionCode != 0)
			at(i)->transactionCode = transactionCode;
		at(i)->redo();
	}
}

TransactionState::~TransactionState()
{
	for (uint i = 0; i < m_states_.size(); ++i)
	{
		if (m_states_[i])
		{
			delete m_states_[i];
			m_states_[i] = 0;
		}
	}
}
