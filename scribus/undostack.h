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

#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include <vector>

class UndoState;
class TransactionState;

typedef std::vector<UndoState*> StateList;

class SCRIBUS_API UndoStack
{
public:
    explicit UndoStack(int maxSize = 20);
    ~UndoStack();

    /* Used to push a new action to the stack. UndoState in the parameter will then
     * become the first undo action in the stack and all the redo actions will be
     * cleared. If maximum size of the stack is hit and an action needs to be removed
     * this function returns true. */
    bool action(UndoState *state);

    /* undo number of steps actions (these will then become redo actions) */
    bool undo(uint steps, int objectId);
    /* redo number of steps actions (these will then become undo actions) */
    bool redo(uint steps, int objectId);

    /* number of actions stored in the stack mostly for testing */
    uint size() const;
    uint undoItems() const;
    uint redoItems() const;

    /* maximum number of actions stored in the stack */
    uint  maxSize() const;
    /* Change the maximum number of actions stored in the stack. If there are
     * both undo and redo actions available and stack size is decreased redo
     * actions will be popped out first starting from the oldest one. 
     * 0 is used to mark infinite stack size. If one wants to disable undo/redo
     * function setUndoEnabled(bool) from UndoManager should be used */
    void setMaxSize(uint maxSize);

    void clear();

    UndoState* getNextUndo(int objectId);
    UndoState* getNextRedo(int objectId);

private:
    /* When an action happens it is pushed to the undoActions_ and the redoActions_
     * is cleared. When undo is requested action is popped from undoActions_ and
     * pushed to the redoActions_ (and vice versa). */
    StateList undoActions_; /* stack would probably be enough for this but vector */
    StateList redoActions_; /* will give more options in future */

    /* maximum amount of actions stored, 0 for no limit */
    uint maxSize_;

    /* returns true if an action was popped from the stack */
    /* assures that we only hold the maxSize_ number of UndoStates */
    bool checkSize();

    friend class UndoManager; // UndoManager needs access to undoActions_ and redoActions_
                              // for updating the attached UndoGui widgets

};

#endif // UNDOSTACK_H
