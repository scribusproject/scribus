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

#include "undomanager.h"
#include "undomanager.moc"
#include <qvaluelist.h>

UndoManager* UndoManager::_instance    = 0;
bool         UndoManager::_undoEnabled = true;

UndoManager* UndoManager::instance()
{
	if (_instance == 0)
		_instance = new UndoManager();

	return _instance;
}

void UndoManager::setUndoEnabled(bool isEnabled)
{
	_undoEnabled = isEnabled;
	if (isEnabled)
		connectGuis();
	else
		disconnectGuis();
}

bool UndoManager::undoEnabled()
{
	return _undoEnabled;
}

UndoManager::UndoManager()
{
	currentUndoObjectId = -1;
	historyLength = 20;
}

void UndoManager::registerGui(UndoGui* gui)
{
	if (gui == NULL)
		return;

	bool tmpUndoEnabled = _undoEnabled;
	setUndoEnabled(false);
	setState(gui);
	undoGuis.push_back(gui);
	setUndoEnabled(tmpUndoEnabled);
}

void UndoManager::setState(UndoGui* gui, int uid)
{
	gui->clear();
	ActionList::iterator it;
	for (it = stacks[currentDoc].second.end() - 1; it > stacks[currentDoc].second.begin() - 1; --it)
	{
		ActionPair pair = *it;
		UndoObject* target = pair.first;
		UndoState*  state  = pair.second;
		if (it > stacks[currentDoc].first - 1)
			gui->insertUndoItem(target, state);
		else
			gui->insertRedoItem(target, state);
	}
}

void UndoManager::connectGuis()
{
	for (uint i = 0; i < undoGuis.size(); ++i)
	{
		UndoGui *gui = undoGuis[i];

		connect(gui, SIGNAL(undo(int)), this, SLOT(doUndo(int)));
		connect(gui, SIGNAL(redo(int)), this, SLOT(doRedo(int)));
		connect(this, SIGNAL(newAction(UndoObject*, UndoState*)),
                gui, SLOT(insertUndoItem(UndoObject*, UndoState*)));
		connect(this, SIGNAL(popBack()), gui, SLOT(popBack()));
		connect(this, SIGNAL(undoSignal(int)), gui, SLOT(updateUndo(int)));
		connect(this, SIGNAL(redoSignal(int)), gui, SLOT(updateRedo(int)));
	}
}

void UndoManager::disconnectGuis()
{
	for (uint i = 0; i < undoGuis.size(); ++i)
	{
		UndoGui *gui = undoGuis[i];

		disconnect(gui, SIGNAL(undo(int)), this, SLOT(doUndo(int)));
		disconnect(gui, SIGNAL(redo(int)), this, SLOT(doRedo(int)));
		disconnect(this, SIGNAL(newAction(UndoObject*, UndoState*)),
                   gui, SLOT(insertUndoItem(UndoObject*, UndoState*)));
		disconnect(this, SIGNAL(popBack()), gui, SLOT(popBack()));
		disconnect(this, SIGNAL(undoSignal(int)), gui, SLOT(updateUndo(int)));
		disconnect(this, SIGNAL(redoSignal(int)), gui, SLOT(updateRedo(int)));
	}
}

void UndoManager::removeGui(UndoGui* gui)
{
	std::vector<UndoGui*>::iterator it;
	for (it = undoGuis.begin(); it != undoGuis.end(); ++it)
		if (*it == gui)
			undoGuis.erase(it);
}

void UndoManager::switchStack(const QString& stackName)
{
	currentDoc = stackName;
	if (!stacks.contains(currentDoc))
	{
		ActionList alist;
		ActionList::iterator it = alist.begin();
		stacks[currentDoc] = StackPair(it, alist);
	}

	for (uint i = 0; i < undoGuis.size(); ++i)
		setState(undoGuis[i]);
}

void UndoManager::rename(const QString& newName)
{
	StackPair tmp = stacks[currentDoc];
	stacks.erase(currentDoc);
	stacks[newName] = tmp;
	currentDoc = newName;
}

void UndoManager::remove(const QString& stackName)
{
	if (stacks.contains(stackName))
	{
		for (uint i = 0; i < stacks[stackName].second.size(); ++i)
		{
			delete stacks[stackName].second[i].second;
			stacks[stackName].second[i].second = NULL;
		}
		stacks.erase(stackName);
		if (currentDoc == stackName)
		{
			for (uint i = 0; i < undoGuis.size(); ++i)
				undoGuis[i]->clear();
		}
	}
}

void UndoManager::action(UndoObject* target, UndoState* state)
{
	if (currentUndoObjectId == -1 || currentUndoObjectId == static_cast<long>(target->getUId()))
		emit newAction(target, state);

	if (stacks[currentDoc].second.size() > 1)
	{
		ActionList::iterator it;
		for (it = stacks[currentDoc].second.begin(); it != stacks[currentDoc].first; ++it)
			delete (*it).second;
		stacks[currentDoc].second.erase(stacks[currentDoc].second.begin(), stacks[currentDoc].first);
	}

	if (stacks[currentDoc].second.size() == static_cast<uint>(historyLength))
	{
		ActionList::iterator it = stacks[currentDoc].second.end() - 1;
		delete (*it).second;
		stacks[currentDoc].second.erase(it);
		emit popBack();
	}
	stacks[currentDoc].second.insert(stacks[currentDoc].second.begin(), ActionPair(target, state));
	stacks[currentDoc].first = stacks[currentDoc].second.begin();
}

void UndoManager::undo(int steps)
{
	if (_undoEnabled);
		doUndo(steps);
}

void UndoManager::redo(int steps)
{
	if (_undoEnabled)
		doRedo(steps);
}

void UndoManager::doUndo(int steps)
{
	if (steps > 0 && _undoEnabled)
	{
		bool tmpUndoEnabled = _undoEnabled;
		setUndoEnabled(false);
		for (int i = 0; i < steps; ++i)
		{
			ActionPair aPair = *stacks[currentDoc].first;
			UndoObject* tmpUndoObject = aPair.first;
			UndoState* tmpUndoState = aPair.second;
			tmpUndoObject->restore(tmpUndoState, true);
			++stacks[currentDoc].first;
		}
		setUndoEnabled(tmpUndoEnabled);
		emit undoSignal(steps);
		emit undoRedoDone();
	}
}

void UndoManager::doRedo(int steps)
{
	if (steps > 0 && _undoEnabled)
	{
		bool tmpUndoEnabled = _undoEnabled;
		setUndoEnabled(false);
		for (int i = 0; i < steps; ++i)
		{
			--stacks[currentDoc].first;
			ActionPair aPair = *stacks[currentDoc].first;
			UndoObject* tmpUndoObject = aPair.first;
			UndoState* tmpUndoState = aPair.second;
			tmpUndoObject->restore(tmpUndoState, false);
		}
		setUndoEnabled(tmpUndoEnabled);
		emit redoSignal(steps);
		emit undoRedoDone();
	}
}

bool UndoManager::hasUndoActions()
{
	return stacks[currentDoc].first < stacks[currentDoc].second.end();
}

bool UndoManager::hasRedoActions()
{
	return stacks[currentDoc].first > stacks[currentDoc].second.begin();
}

void UndoManager::showObject(int uid)
{

}

void UndoManager::setHistoryLength(int steps)
{
	if (steps > 0)
		historyLength = steps;
}

int UndoManager::getHistoryLength()
{
	return historyLength;
}

void UndoManager::deleteInstance()
{
	if (_instance)
		delete _instance;
	_instance = NULL;
}

UndoManager::~UndoManager()
{
	StackMap::iterator it;
	for (it = stacks.begin(); it != stacks.end(); ++it)
	{
		for (uint i = 0; i < (*it).second.size(); ++i)
		{
			delete ((*it).second)[i].second;
			((*it).second)[i].second = NULL;
		}
	}
}
