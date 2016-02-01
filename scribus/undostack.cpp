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

UndoStack::UndoStack(int maxSize) : m_maxSize_(maxSize)
{

}

bool UndoStack::action(UndoState *state)
{
    m_redoActions_.clear();
    m_undoActions_.insert(m_undoActions_.begin(), state);
    bool needsPopping = checkSize(); // only store maxSize_ amount of actions

    return needsPopping;
}

bool UndoStack::undo(uint steps, int objectId)
{
	for (uint i = 0; i < steps && !m_undoActions_.empty(); ++i) {
		UndoState *tmpUndoState = 0;
		if (objectId == Um::GLOBAL_UNDO_MODE)
		{
			tmpUndoState = m_undoActions_[0];
			m_undoActions_.erase(m_undoActions_.begin());
		}
		else
		{ // object specific mode (search the correct state)
			StateList::iterator it;
			for (it = m_undoActions_.begin(); it != m_undoActions_.end(); ++it)
			{
				UndoObject *tmp = (*it)->undoObject();
				if (tmp && tmp->getUId() == static_cast<ulong>(objectId))
				{
					tmpUndoState = *it;
					m_undoActions_.erase(it);
					break;
				}
				else if((*it)->isTransaction())
				{
					TransactionState *ts = dynamic_cast<TransactionState*>(*it);
					if(ts && ts->containsOnly(objectId))
					{
						tmpUndoState = *it;
						m_undoActions_.erase(it);
						break;
					}
				}
			}
		}
		if (tmpUndoState)
		{
			m_redoActions_.insert(m_redoActions_.begin(), tmpUndoState); // push to the redo actions
			tmpUndoState->undo();
		}
	}
	return true;
}

bool UndoStack::redo(uint steps, int objectId)
{
	for (uint i = 0; i < steps && !m_redoActions_.empty(); ++i) {
		UndoState *tmpRedoState = 0;
		if (objectId == Um::GLOBAL_UNDO_MODE)
		{
			tmpRedoState = m_redoActions_[0];
			m_redoActions_.erase(m_redoActions_.begin());
		}
		else
		{ // object specific mode (search the correct state)
			StateList::iterator it;
			for (it = m_redoActions_.begin(); it != m_redoActions_.end(); ++it)
			{
				UndoObject *tmp = (*it)->undoObject();
				if (tmp && tmp->getUId() == static_cast<ulong>(objectId))
				{
					tmpRedoState = *it;
					m_redoActions_.erase(it);
					break;
				}
				else if((*it)->isTransaction())
				{
					TransactionState *ts = dynamic_cast<TransactionState*>(*it);
					if(ts && ts->containsOnly(objectId))
					{
						tmpRedoState = *it;
						m_redoActions_.erase(it);
						break;
					}
				}
			}
		}
		if (tmpRedoState)
		{
			m_undoActions_.insert(m_undoActions_.begin(), tmpRedoState); // push to the undo actions
			tmpRedoState->redo();
		}
	}
	return true;
}

uint UndoStack::size() const
{
    return m_undoActions_.size() + m_redoActions_.size();
}

uint UndoStack::undoItems() const
{
    return m_undoActions_.size();
}

uint UndoStack::redoItems() const
{
    return m_redoActions_.size();
}

uint  UndoStack::maxSize() const
{
    return m_maxSize_;
}

void UndoStack::setMaxSize(uint maxSize)
{
    m_maxSize_ = maxSize;
    checkSize(); // we may need to remove actions
}

bool UndoStack::checkSize() {
    if (m_maxSize_ == 0) // 0 marks for infinite stack size
        return false;

    bool needsPopping = size () > m_maxSize_;

    while (size() > m_maxSize_) {
        if (m_redoActions_.size() > 0) // clear redo actions first
            m_redoActions_.pop_back();
        else
            m_undoActions_.pop_back();
    }

    return needsPopping;
}

void UndoStack::clear()
{
	for (uint i = 0; i < m_undoActions_.size(); ++i)
		delete m_undoActions_[i];
	for (uint i = 0; i < m_redoActions_.size(); ++i)
		delete m_redoActions_[i];
	m_undoActions_.clear();
	m_redoActions_.clear();
}

UndoState* UndoStack::getNextUndo(int objectId)
{
	UndoState *state = 0;
	if (!m_undoActions_.empty())
	{
		if (objectId == Um::GLOBAL_UNDO_MODE)
			state = m_undoActions_[0];
		else
		{
			StateList::iterator it;
			for (it = m_undoActions_.begin(); it != m_undoActions_.end(); ++it)
			{
				UndoState*  tmp  = *it;
				TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
				if ((tmp && tmp->undoObject() &&
						 tmp->undoObject()->getUId() == static_cast<ulong>(objectId)) || (ts && ts->containsOnly(objectId)))
				{
					state = tmp;
					break;
				}
				else if (ts && ts->contains(objectId))
					break;
			}
		}
	}
	return state;
}

UndoState* UndoStack::getNextRedo(int objectId)
{
	UndoState *state = 0;
	if (!m_redoActions_.empty())
	{
		if (objectId == Um::GLOBAL_UNDO_MODE)
			state = m_redoActions_[0];
		else
		{
			StateList::iterator it;
			for (it = m_redoActions_.begin(); it != m_redoActions_.end(); ++it)
			{
				UndoState*  tmp  = *it;
				TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
				if ((tmp && tmp->undoObject() &&
						 tmp->undoObject()->getUId() == static_cast<ulong>(objectId)) || (ts && ts->containsOnly(objectId)))
				{
					state = tmp;
					break;
				}
				else if (ts && ts->contains(objectId))
					break;
			}
		}
	}
	return state;
}

UndoStack::~UndoStack()
{
    // no dynamically allocated memory
}

