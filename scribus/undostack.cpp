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

#include "undomanager.h"
#include "undoobject.h"
#include "undostack.h"

UndoStack::UndoStack(int maxSize) : m_maxSize(maxSize)
{

}

bool UndoStack::action(UndoState *state)
{
	m_redoActions.clear();
	m_undoActions.insert(m_undoActions.begin(), state);
	bool needsPopping = checkSize(); // only store maxSize_ amount of actions

	return needsPopping;
}

bool UndoStack::undo(uint steps, int objectId)
{
	for (uint i = 0; i < steps && !m_undoActions.empty(); ++i)
	{
		UndoState *tmpUndoState = nullptr;
		if (objectId == Um::GLOBAL_UNDO_MODE)
		{
			tmpUndoState = m_undoActions[0];
			m_undoActions.erase(m_undoActions.begin());
		}
		else
		{
			// object specific mode (search the correct state)
			for (auto it = m_undoActions.begin(); it != m_undoActions.end(); ++it)
			{
				UndoObject *tmp = (*it)->undoObject();
				if (tmp && tmp->getUId() == static_cast<ulong>(objectId))
				{
					tmpUndoState = *it;
					m_undoActions.erase(it);
					break;
				}
				if ((*it)->isTransaction())
				{
					TransactionState *ts = dynamic_cast<TransactionState*>(*it);
					if(ts && ts->containsOnly(objectId))
					{
						tmpUndoState = *it;
						m_undoActions.erase(it);
						break;
					}
				}
			}
		}
		if (tmpUndoState)
		{
			m_redoActions.insert(m_redoActions.begin(), tmpUndoState); // push to the redo actions
			tmpUndoState->undo();
		}
	}
	return true;
}

bool UndoStack::redo(uint steps, int objectId)
{
	for (uint i = 0; i < steps && !m_redoActions.empty(); ++i)
	{
		UndoState *tmpRedoState = nullptr;
		if (objectId == Um::GLOBAL_UNDO_MODE)
		{
			tmpRedoState = m_redoActions[0];
			m_redoActions.erase(m_redoActions.begin());
		}
		else
		{ // object specific mode (search the correct state)
			StateList::iterator it;
			for (it = m_redoActions.begin(); it != m_redoActions.end(); ++it)
			{
				UndoObject *tmp = (*it)->undoObject();
				if (tmp && tmp->getUId() == static_cast<ulong>(objectId))
				{
					tmpRedoState = *it;
					m_redoActions.erase(it);
					break;
				}
				if((*it)->isTransaction())
				{
					TransactionState *ts = dynamic_cast<TransactionState*>(*it);
					if(ts && ts->containsOnly(objectId))
					{
						tmpRedoState = *it;
						m_redoActions.erase(it);
						break;
					}
				}
			}
		}
		if (tmpRedoState)
		{
			m_undoActions.insert(m_undoActions.begin(), tmpRedoState); // push to the undo actions
			tmpRedoState->redo();
		}
	}
	return true;
}

uint UndoStack::size() const
{
	return m_undoActions.size() + m_redoActions.size();
}

uint UndoStack::undoItems() const
{
	return m_undoActions.size();
}

uint UndoStack::redoItems() const
{
	return m_redoActions.size();
}

uint  UndoStack::maxSize() const
{
	return m_maxSize;
}

void UndoStack::setMaxSize(uint maxSize)
{
	m_maxSize = maxSize;
	checkSize(); // we may need to remove actions
}

bool UndoStack::checkSize()
{
	if (m_maxSize == 0) // 0 marks for infinite stack size
		return false;

	bool needsPopping = size () > m_maxSize;

	while (size() > m_maxSize)
	{
		if (!m_redoActions.empty()) // clear redo actions first
			m_redoActions.pop_back();
		else
			m_undoActions.pop_back();
	}

	return needsPopping;
}

void UndoStack::clear()
{
	for (size_t i = 0; i < m_undoActions.size(); ++i)
		delete m_undoActions[i];
	for (size_t i = 0; i < m_redoActions.size(); ++i)
		delete m_redoActions[i];
	m_undoActions.clear();
	m_redoActions.clear();
}

UndoState* UndoStack::getNextUndo(int objectId)
{
	if (m_undoActions.empty())
		return nullptr;

	if (objectId == Um::GLOBAL_UNDO_MODE)
		return m_undoActions[0];
	
	UndoState *state = nullptr;
	for (auto it = m_undoActions.begin(); it != m_undoActions.end(); ++it)
	{
		UndoState*  tmp  = *it;
		TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
		if ((tmp && tmp->undoObject() &&
				tmp->undoObject()->getUId() == static_cast<ulong>(objectId)) || (ts && ts->containsOnly(objectId)))
		{
			state = tmp;
			break;
		}
		if (ts && ts->contains(objectId))
			break;
	}
	return state;
}

UndoState* UndoStack::getNextRedo(int objectId)
{
	if (m_redoActions.empty())
		return nullptr;

	if (objectId == Um::GLOBAL_UNDO_MODE)
		return m_redoActions[0];
	
	UndoState *state = nullptr;
	for (auto it = m_redoActions.begin(); it != m_redoActions.end(); ++it)
	{
		UndoState*  tmp  = *it;
		TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
		if ((tmp && tmp->undoObject() &&
				tmp->undoObject()->getUId() == static_cast<ulong>(objectId)) || (ts && ts->containsOnly(objectId)))
		{
			state = tmp;
			break;
		}
		if (ts && ts->contains(objectId))
			break;
	}
	return state;
}

UndoStack::~UndoStack()
{
	// no dynamically allocated memory
}

