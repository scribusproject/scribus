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

#include "undotransaction.h"

#include <QDebug>
#include <QPixmap>

#include "scconfig.h"
#include "undomanager.h"
#include "undoobject.h"
#include "undostate.h"
#include "undotransaction.h"
#include "util_icon.h"

UndoTransaction::UndoTransaction() : Transaction(0)
{};

UndoTransaction::UndoTransaction(TransactionData* data) : Transaction(data) 
{};

UndoTransaction::~UndoTransaction() 
{
	if (m_data && (m_data->ref == 1))
		UndoTransaction::cancel(); // no virtual calls in destructor
}

bool UndoTransaction::cancel()
{
	if (!m_data)
		return false;
	
	TransactionData* data = static_cast<TransactionData*>(m_data.data());
	UndoManager* UM = data->UM;
	int stackLevel = data->stackLevel;

	switch (m_data->m_status)
	{
		case Transaction::STATE_OPEN:
		case Transaction::STATE_WILLFAIL:
//			qDebug() << "UndoManager::cancelTransaction" << data << data->transactionObject->getUName() << data->transactionState->getName() << stackLevel;
			data->m_status = Transaction::STATE_FAILED;
			delete data->transactionObject;
			data->transactionObject = 0;
			delete data->transactionState;
			data->transactionState = 0;
			//brutal for now:
			assert (stackLevel + 1 == signed(UM->transactions_.size()));
			if (stackLevel < signed(UM->transactions_.size()))
			{
				UM->transactions_.erase(UM->transactions_.begin() + stackLevel);
			}
			return true;
		default:
			// do nothing;
//			qDebug() << "UndoManager::cancelTransaction ** already closed **";
			return false;
	}
}

bool UndoTransaction::commit(const QString &targetName,
							 QPixmap *targetPixmap,
							 const QString &name,
							 const QString &description,
							 QPixmap *actionPixmap)
{
	if (m_data && m_data->m_status == Transaction::STATE_OPEN)
	{
		TransactionData* data = static_cast<TransactionData*>(m_data.data());
		if (targetName.length() > 0)
			data->transactionObject->setUName(targetName);
		if (targetPixmap)
			data->transactionObject->setUPixmap(targetPixmap);
		if (name.length() > 0)
			data->transactionState->setName(name);
		if (description.length() > 0)
			data->transactionState->setDescription(description);
		if (actionPixmap)
			data->transactionState->setPixmap(actionPixmap);
	}
	return commit();
}
				
	
bool UndoTransaction::commit()
{
	if (!m_data)
		return false;
	TransactionData* data = static_cast<TransactionData*>(m_data.data());
	UndoManager* UM = data->UM;
	int stackLevel = data->stackLevel;

	if (!UM->undoEnabled_)
	{
		cancel();
		return false;
	}
	
	UndoObject *tmpu = UM->transactions_.at(stackLevel)->transactionObject;
	TransactionState *tmps = UM->transactions_.at(stackLevel)->transactionState;
	
	switch (m_data->m_status)
	{
		case Transaction::STATE_OPEN:
//			qDebug() << "UndoManager::commitTransaction" << data << data->transactionObject->getUName() << data->transactionState->getName() << stackLevel;
			m_data->m_status = Transaction::STATE_COMMITTED;

			// brutal for now:
			assert (stackLevel + 1 == signed(UM->transactions_.size()));
			
			if (stackLevel < signed(UM->transactions_.size()))
			{
				UM->transactions_.erase(UM->transactions_.begin() + stackLevel);
			}
				
			if (tmps->sizet() > 0) // are there any actions inside the commited transaction
			{
				if (tmps->getName().isEmpty())
					tmps->useActionName();
				UM->action(tmpu, tmps);
			} // if not just delete objects
			else
			{
				delete tmpu;
				tmpu = 0;
				delete tmps;
				tmps = 0;
			}
			return true;
			break;
		case STATE_WILLFAIL:
			return cancel();
			break;
		default:
//			qDebug() << "UndoManager::commitTransaction ** already closed **";
			// nothing
			break;
	}
	return false;
}
