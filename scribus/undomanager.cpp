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
#include "undogui.h"
#include "config.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include <qvaluelist.h>

extern PrefsFile *prefsFile;
extern QPixmap loadIcon(QString nam);

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
	if (!UndoManager::IGuides)
		initIcons();
	prefs = prefsFile->getContext("undo");
	historyLength = prefs->getInt("historyLength", 10);
	if (historyLength < 0)
		historyLength = 10;
	transaction = NULL;
	transactionTarget = NULL;
}

void UndoManager::beginTransaction(const QString &targetName,
                                   const QString &name,
                                   const QString &description,
                                   QPixmap *pixmap)
{
	// TODO What if there already is a transaction? -rl
	if (transaction)
		commit();
	transaction = new TransactionState();
	transactionTarget = new TransactionObject();
	transactionTarget->setUName(targetName);
	if (name.length() > 0)
		transaction->setName(name);
	if (description.length() > 0)
		transaction->setDescription(description);
	if (pixmap)
		transaction->setPixmap(pixmap);
}

void UndoManager::cancelTransaction()
{
	delete transaction;
	transaction = NULL;
	delete transactionTarget;
	transactionTarget = NULL;
}

void UndoManager::commit(const QString &targetName,
                         const QString &name,
                         const QString &description,
                         QPixmap *pixmap)
{
	if (!transaction || !transactionTarget)
	{
		cancelTransaction();
		return;
	}
	if (targetName.length() > 0)
		transactionTarget->setUName(targetName);
	if (name.length() > 0)
		transaction->setName(name);
	if (description.length() > 0)
		transaction->setDescription(description);
	if (pixmap)
		transaction->setPixmap(pixmap);

	UndoObject *tmpu = transactionTarget;
	TransactionState *tmps = transaction;
	transaction = NULL;
	transactionTarget = NULL;
	if (tmps->sizet() > 0)
	{
		if (tmps->getName() == "")
			tmps->useActionName();
		action(tmpu, tmps);
	}
	else
	{
		delete tmpu;
		tmpu = NULL;
		delete tmps;
		tmps = NULL;
	}
}

bool UndoManager::isTransactionMode()
{
	return transaction ? true : false;
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
	if (currentDoc == newName)
		return;
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
			currentDoc = "__no_name__";
		}
	}
}

void UndoManager::action(UndoObject* target, UndoState* state)
{
	if ((!transaction) &&
        (currentUndoObjectId == -1 || currentUndoObjectId == static_cast<long>(target->getUId())))
		emit newAction(target, state); // send action to the guis

	if (stacks[currentDoc].second.size() > 1) // delete redo states
	{
		ActionList::iterator it;
		for (it = stacks[currentDoc].second.begin(); it != stacks[currentDoc].first; ++it)
		{
			TransactionState *ts = dynamic_cast<TransactionState*>((*it).second);
			if (ts)
				delete (*it).first; // delete TransactionObject
			delete (*it).second;
			(*it).second = NULL;
		}
		stacks[currentDoc].second.erase(stacks[currentDoc].second.begin(), stacks[currentDoc].first);
	}

	if (transaction)
		transaction->pushBack(target, state);
	else
	{
		checkStackLength(); // check that history length
		stacks[currentDoc].second.insert(stacks[currentDoc].second.begin(), ActionPair(target, state));
		stacks[currentDoc].first = stacks[currentDoc].second.begin();
	}
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
	if (steps > 0 && _undoEnabled && stacks.size() > 0)
	{
		bool tmpUndoEnabled = _undoEnabled;
		setUndoEnabled(false);
		UndoState* tmpUndoState = NULL;
		for (int i = 0; i < steps; ++i)
		{
			ActionPair aPair = *stacks[currentDoc].first;
			UndoObject* tmpUndoObject = aPair.first;
			tmpUndoState = aPair.second;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmpUndoState);
			if (tmpUndoState && !ts)
				tmpUndoObject->restore(tmpUndoState, true);
			else if (ts)
				doTransactionUndo(ts);
			++stacks[currentDoc].first;
		}
		if (tmpUndoState)
		{
			setUndoEnabled(tmpUndoEnabled);
			emit undoSignal(steps);
			emit undoRedoDone();
		}
	}
}

void UndoManager::doTransactionUndo(TransactionState *tstate)
{
	for (int i = tstate->sizet() - 1; i > -1; --i)
	{
		ActionPair *pair = tstate->at(i);
		if (pair && pair->first && pair->second)
			pair->first->restore(pair->second, true);
	}
}

void UndoManager::doRedo(int steps)
{
	if (steps > 0 && _undoEnabled && stacks.size() > 0)
	{
		bool tmpUndoEnabled = _undoEnabled;
		setUndoEnabled(false);
		UndoState* tmpUndoState = NULL;
		for (int i = 0; i < steps; ++i)
		{
			--stacks[currentDoc].first;
			ActionPair aPair = *stacks[currentDoc].first;
			UndoObject* tmpUndoObject = aPair.first;
			tmpUndoState = aPair.second;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmpUndoState);
			if (tmpUndoState && !ts)
				tmpUndoObject->restore(tmpUndoState, false);
			else if (ts)
				doTransactionRedo(ts);
		}
		if (tmpUndoState)
		{
			setUndoEnabled(tmpUndoEnabled);
			emit redoSignal(steps);
			emit undoRedoDone();
		}
	}
}

void UndoManager::doTransactionRedo(TransactionState *tstate)
{
	ActionPair *pair = NULL;
	for (uint i = 0; i < tstate->sizet(); ++i)
	{
		pair = tstate->at(i);
		pair->first->restore(pair->second, false);
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
	if (steps >= 0)
	{
		historyLength = steps;
		prefs->set("historyLength", historyLength);
		checkStackLength();
	}
}

int UndoManager::getHistoryLength()
{
	if (stacks.size() > 0 && stacks[currentDoc].first != stacks[currentDoc].second.begin())
		return -1;
	return historyLength;
}

void UndoManager::checkStackLength()
{
	if ((stacks[currentDoc].second.size() >= static_cast<uint>(historyLength)) && (historyLength != 0))
	{
		while (stacks[currentDoc].second.size() >= static_cast<uint>(historyLength))
		{
			ActionList::iterator it = stacks[currentDoc].second.end() - 1;
			if (stacks[currentDoc].first == it)
				stacks[currentDoc].first = it - 1;
			TransactionState *ts = dynamic_cast<TransactionState*>((*it).second);
			if (ts)
				delete (*it).first; // delete TransactionObject
			delete (*it).second;
			stacks[currentDoc].second.erase(it);
			emit popBack();
		}
	}
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

/*** UndoManager::TransactionState *****************************************************/

UndoManager::TransactionState::TransactionState() : UndoState("")
{
	_size = 0;
}

ActionPair* UndoManager::TransactionState::at(int index)
{
	if (index >= 0 && static_cast<uint>(index) < sizet())
		return states[index];
	else
		return NULL;
}

void UndoManager::TransactionState::pushBack(UndoObject *target, UndoState *state)
{
	if (target && state)
	{
		states.push_back(new ActionPair(target, state));
		++_size;
	}
}

uint UndoManager::TransactionState::sizet()
{
	return _size;
}

void UndoManager::TransactionState::useActionName()
{
	if (_size > 0)
	{
		setName(states[_size - 1]->second->getName());
	}
}

UndoManager::TransactionState::~TransactionState()
{
	for (uint i = 0; i < states.size(); ++i)
	{
		if (states[i])
		{
			if (states[i]->second)
			{
				delete states[i]->second;
				states[i]->second = NULL;
			}
			delete states[i];
			states[i] = NULL;
		}
	}
}

/*************************************************************************************/

void UndoManager::initIcons()
{
	QString iconDir = ICONDIR;
	UndoManager::IGuides         = new QPixmap(iconDir + "u_margins.png");
	UndoManager::ILockGuides     = new QPixmap(iconDir + "u_margins_locked.png");
	UndoManager::IMoveText       = new QPixmap(iconDir + "u_move_text.png");
	UndoManager::IMoveImage      = new QPixmap(iconDir + "u_move_image.png");
	UndoManager::IMoveLine       = new QPixmap(iconDir + "u_move_line.png");
	UndoManager::IMovePolygon    = new QPixmap(iconDir + "u_move_polygon.png");
	UndoManager::IMovePolyline   = new QPixmap(iconDir + "u_move_polyline.png");
// 	UndoManager::IMovePathText   = new QPixmap(iconDir + "u_move_pathtext.png");
	UndoManager::IResizeText     = new QPixmap(iconDir + "u_resize_text.png");
	UndoManager::IResizeImage    = new QPixmap(iconDir + "u_resize_image.png");
	UndoManager::IResizeLine     = new QPixmap(iconDir + "u_resize_line.png");
	UndoManager::IResizePolygon  = new QPixmap(iconDir + "u_resize_polygon.png");
	UndoManager::IResizePolyline = new QPixmap(iconDir + "u_resize_polyline.png");
// 	UndoManager::IResizePathText = new QPixmap(iconDir + "u_resize_pathtext.png");
	UndoManager::IRotateText     = new QPixmap(iconDir + "u_rotate_text.png");
	UndoManager::IRotateImage    = new QPixmap(iconDir + "u_rotate_image.png");
	UndoManager::IRotateLine     = new QPixmap(iconDir + "u_rotate_line.png");
	UndoManager::IRotatePolygon  = new QPixmap(iconDir + "u_rotate_polygon.png");
	UndoManager::IRotatePolyline = new QPixmap(iconDir + "u_rotate_polyline.png");
// 	UndoManager::IRotatePathText = new QPixmap(iconDir + "u_rotate_pathtext.png");
}

const QString UndoManager::AddVGuide    = tr("Add vertical guide");
const QString UndoManager::AddHGuide    = tr("Add horizontal guide");
const QString UndoManager::DelVGuide    = tr("Remove vertical guide");
const QString UndoManager::DelHGuide    = tr("Remove horizontal guide");
const QString UndoManager::MoveVGuide   = tr("Move vertical guide");
const QString UndoManager::MoveHGuide   = tr("Move horizontal guide");
const QString UndoManager::LockGuides   = tr("Lock guides");
const QString UndoManager::UnlockGuides = tr("Unlock guides");
const QString UndoManager::Move         = tr("Move");
const QString UndoManager::Resize       = tr("Resize");
const QString UndoManager::Rotate       = tr("Rotate");
const QString UndoManager::MoveFromTo   = tr("X1: %1, Y1: %2, Page %3\nX2: %4, Y2: %5, Page %6");
const QString UndoManager::ResizeFromTo = tr("W1: %1, H1: %2\nW2: %3, H2: %4");
const QString UndoManager::RotateFromTo = tr("From %1 to %2");
const QString UndoManager::Selection    = tr("Selection");
const QString UndoManager::Group        = tr("Group");

QPixmap *UndoManager::IGuides         = NULL;
QPixmap *UndoManager::ILockGuides     = NULL;
QPixmap *UndoManager::IMoveText       = NULL;
QPixmap *UndoManager::IMoveImage      = NULL;
QPixmap *UndoManager::IMoveLine       = NULL;
QPixmap *UndoManager::IMovePolygon    = NULL;
QPixmap *UndoManager::IMovePolyline   = NULL;
QPixmap *UndoManager::IMovePathText   = NULL;
QPixmap *UndoManager::IResizeText     = NULL;
QPixmap *UndoManager::IResizeImage    = NULL;
QPixmap *UndoManager::IResizeLine     = NULL;
QPixmap *UndoManager::IResizePolygon  = NULL;
QPixmap *UndoManager::IResizePolyline = NULL;
QPixmap *UndoManager::IResizePathText = NULL;
QPixmap *UndoManager::IRotateText     = NULL;
QPixmap *UndoManager::IRotateImage    = NULL;
QPixmap *UndoManager::IRotateLine     = NULL;
QPixmap *UndoManager::IRotatePolygon  = NULL;
QPixmap *UndoManager::IRotatePolyline = NULL;
QPixmap *UndoManager::IRotatePathText = NULL;
