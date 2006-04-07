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

#include "undomanager.h"
#include "undoobject.h"
#include "undostack.h"

UndoStack::UndoStack(int maxSize) : maxSize_(maxSize)
{

}

bool UndoStack::action(UndoState *state)
{
    redoActions_.clear();
    undoActions_.insert(undoActions_.begin(), state);
    bool needsPopping = checkSize(); // only store maxSize_ amount of actions

    return needsPopping;
}

bool UndoStack::undo(uint steps, int objectId)
{
	for (uint i = 0; i < steps && !undoActions_.empty(); ++i) {
		UndoState *tmpUndoState = 0;
		if (objectId == Um::GLOBAL_UNDO_MODE)
		{
			tmpUndoState = undoActions_[0];
			undoActions_.erase(undoActions_.begin());
		}
		else
		{ // object specific mode (search the correct state)
			StateList::iterator it;
			for (it = undoActions_.begin(); it != undoActions_.end(); ++it)
			{
				UndoObject *tmp = (*it)->undoObject();
				if (tmp && tmp->getUId() == static_cast<ulong>(objectId))
				{
					tmpUndoState = *it;
					undoActions_.erase(it);
					break;
				}
			}
		}

		redoActions_.insert(redoActions_.begin(), tmpUndoState); // push to the redo actions
		tmpUndoState->undo();
	}

	return true;
}

bool UndoStack::redo(uint steps, int objectId)
{
	for (uint i = 0; i < steps && !redoActions_.empty(); ++i) {
		UndoState *tmpRedoState = 0;
		if (objectId == Um::GLOBAL_UNDO_MODE)
		{
			tmpRedoState = redoActions_[0];
			redoActions_.erase(redoActions_.begin());
		}
		else
		{ // object specific mode (search the correct state)
			StateList::iterator it;
			for (it = redoActions_.begin(); it != redoActions_.end(); ++it)
			{
				UndoObject *tmp = (*it)->undoObject();
				if (tmp && tmp->getUId() == static_cast<ulong>(objectId))
				{
					tmpRedoState = *it;
					redoActions_.erase(it);
					break;
				}
			}
		}
		
		undoActions_.insert(undoActions_.begin(), tmpRedoState); // push to the undo actions
		tmpRedoState->redo();
	}
	
	return true;
}

uint UndoStack::size() const
{
    return undoActions_.size() + redoActions_.size();
}

uint UndoStack::undoItems() const
{
    return undoActions_.size();
}

uint UndoStack::redoItems() const
{
    return redoActions_.size();
}

uint  UndoStack::maxSize() const
{
    return maxSize_;
}

void UndoStack::setMaxSize(uint maxSize)
{
    maxSize_ = maxSize;
    checkSize(); // we may need to remove actions
}

bool UndoStack::checkSize() {
    if (maxSize_ == 0) // 0 marks for infinite stack size
        return false;

    bool needsPopping = size () > maxSize_;

    while (size() > maxSize_) {
        if (redoActions_.size() > 0) // clear redo actions first
            redoActions_.pop_back();
        else
            undoActions_.pop_back();
    }

    return needsPopping;
}

void UndoStack::clear()
{
	for (uint i = 0; i < undoActions_.size(); ++i)
		delete undoActions_[i];
	for (uint i = 0; i < redoActions_.size(); ++i)
		delete redoActions_[i];
	undoActions_.clear();
	redoActions_.clear();
}

UndoState* UndoStack::getNextUndo(int objectId)
{
	UndoState *state = 0;
	if (!undoActions_.empty())
	{
		if (objectId == Um::GLOBAL_UNDO_MODE)
			state = undoActions_[0];
		else
		{
			StateList::iterator it;
			for (it = undoActions_.begin(); it != undoActions_.end(); ++it)
			{
				UndoState*  tmp  = *it;
				TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
				if (ts && ts->contains(objectId))
					break;
				else if (!ts && tmp->undoObject() && 
				         tmp->undoObject()->getUId() == static_cast<ulong>(objectId))
				{
					state = tmp;
					break;
				}
			}
		}
	}
	return state;
}

UndoState* UndoStack::getNextRedo(int objectId)
{
	UndoState *state = 0;
	if (!redoActions_.empty())
	{
		if (objectId == Um::GLOBAL_UNDO_MODE)
			state = redoActions_[0];
		else
		{
			StateList::iterator it;
			for (it = redoActions_.begin(); it != redoActions_.end(); ++it)
			{
				UndoState*  tmp  = *it;
				TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
				if (ts && ts->contains(objectId))
					break;
				else if (!ts && tmp->undoObject() && 
				         tmp->undoObject()->getUId() == static_cast<ulong>(objectId))
				{
					state = tmp;
					break;
				}
			}
		}
	}
	return state;
}

UndoStack::~UndoStack()
{
    // no dynamically allocated memory
}

