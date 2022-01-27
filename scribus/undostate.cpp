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
	m_actionName(name),
	m_actionDescription(description),
	m_actionPixmap(pixmap),
	m_undoObject(nullptr)
{

}

const QString& UndoState::getName() const
{
	return m_actionName;
}

void UndoState::setName(const QString &newName)
{
	m_actionName = newName;
}

const QString& UndoState::getDescription() const
{
	return m_actionDescription;
}

void UndoState::setDescription(const QString &newDescription)
{
	m_actionDescription = newDescription;
}

QPixmap* UndoState::getPixmap()
{
	return m_actionPixmap;
}

void UndoState::setPixmap(QPixmap *pixmap)
{
	m_actionPixmap = pixmap;
}

void UndoState::undo()
{
	if (m_undoObject) // if !m_undoObject there's an error, hmmm
		m_undoObject->restore(this, true);
}

void UndoState::redo()
{
	if (m_undoObject)
		m_undoObject->restore(this, false);
}

void UndoState::setUndoObject(UndoObject *object)
{
	m_undoObject = object->undoObjectPtr();
}

UndoObject* UndoState::undoObject()
{
	return m_undoObject;
}

/*** SimpleState **************************************************************/

SimpleState::SimpleState(const QString& name, const QString& description, QPixmap* pixmap)
           : UndoState(name, description, pixmap)
{

}

bool SimpleState::contains(const QString& key) const
{
	return m_values.contains(key);
}

QVariant SimpleState::variant(const QString& key, const QVariant& def) const
{
	QMap<QString, QVariant>::const_iterator it = m_values.constFind(key);
	if (it != m_values.constEnd())
		return it.value();

	return def;
}

QString SimpleState::get(const QString& key, const QString& def) const
{
	QMap<QString, QVariant>::const_iterator it = m_values.constFind(key);
	if (it != m_values.constEnd())
		return it.value().toString();

	return def;
}

bool SimpleState::getBool(const QString& key, bool def) const
{
	bool ok = false;
	QVariant retVar = variant(key, QVariant(def));
	int ret = retVar.toInt(&ok);
	if (!ok)
		ret = def;
	return ret;
}

int SimpleState::getInt(const QString& key, int def) const
{
	bool ok = false;
	QVariant retVar = variant(key, QVariant(def));
	int ret = retVar.toInt(&ok);
	if (!ok)
		ret = def;
	return ret;
}

uint SimpleState::getUInt(const QString& key, uint def) const
{
	bool ok = false;
	QVariant retVar = variant(key, QVariant(def));
	uint ret = retVar.toUInt(&ok);
	if (!ok)
		ret = def;
	return ret;
}

double SimpleState::getDouble(const QString& key, double def) const
{
	bool ok = false;
	QVariant retVar = variant(key, QVariant(def));
	double ret = retVar.toDouble(&ok);
	if (!ok)
		ret = def;
	return ret;
}

void* SimpleState::getVoidPtr(const QString& key, void* def) const
{
	void* ret = nullptr;
	QVariant retVar = variant(key, QVariant::fromValue(def));
	if (retVar.canConvert<void*>())
		ret = retVar.value<void*>();
	return ret;
}


void SimpleState::set(const QString& key)
{
	m_values[key] = QVariant();
}

void SimpleState::set(const QString& key, const QString& value)
{
	m_values[key] = QVariant(value);
}

void SimpleState::set(const QString& key, bool value)
{
	m_values[key] = QVariant(value);
}

void SimpleState::set(const QString& key, int value)
{
	m_values[key] = QVariant(value);
}

void SimpleState::set(const QString& key, uint value)
{
	m_values[key] = QVariant(value);
}

void SimpleState::set(const QString& key, double value)
{
	m_values[key] = QVariant(value);
}

void SimpleState::set(const QString& key, void* ptr)
{
	m_values[key] = QVariant::fromValue<void*>(ptr);
}

/*** TransactionState *****************************************************/

TransactionState::TransactionState() : UndoState(QString())
{

}

UndoState* TransactionState::at(int index) const
{
	if (index >= 0 && static_cast<uint>(index) < sizet())
		return m_states[index];
	return nullptr;
}

UndoState* TransactionState::last() const
{
	if (!m_states.empty())
		return m_states.at(m_size - 1);
	return nullptr;
}

bool TransactionState::contains(int uid) const
{
	for (size_t i = 0; i < m_states.size(); ++i)
	{
		UndoObject* undoObject = m_states[i]->undoObject();
		if (undoObject && undoObject->getUId() == static_cast<uint>(uid))
			return true;
	}
	return false;
}

bool TransactionState::containsOnly(int uid) const
{
	for (size_t i = 0; i < m_states.size(); ++i)
	{
		UndoObject* undoObject = m_states[i]->undoObject();
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
		m_states.push_back(state);
		++m_size;
	}
}

uint TransactionState::sizet() const
{
	return m_size;
}

void TransactionState::useActionName()
{
	if (m_size > 0)
		setName(m_states[m_size - 1]->getName());
}

UndoObject* TransactionState::replace(ulong uid, UndoObject *newUndoObject)
{
	UndoObject *tmp = nullptr;
	for (size_t i = 0; i < m_states.size(); ++i)
	{
		TransactionState *ts = dynamic_cast<TransactionState*>(m_states[i]);
		if (ts) // are we having a transaction_inside a transaction
			ts->replace(uid, newUndoObject);
		else if (m_states[i]->undoObject() && m_states[i]->undoObject()->getUId() == uid)
		{
			tmp = m_states[i]->undoObject();
			m_states[i]->setUndoObject(newUndoObject);
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
	for (size_t i = 0; i < m_states.size(); ++i)
	{
		if (m_states[i])
		{
			delete m_states[i];
			m_states[i] = nullptr;
		}
	}
}

void* ScItemsState::getItem(const QString& itemname) const
{
	if (pointerMap.contains(itemname))
		return pointerMap.value(itemname, nullptr);
	return nullptr;
}
