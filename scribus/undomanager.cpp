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
                                   QPixmap *targetPixmap,
                                   const QString &name,
                                   const QString &description,
                                   QPixmap *actionPixmap)
{
	if (transaction) // begin a transaction inside transaction
		transactions.push_back(
			std::pair<TransactionObject*, TransactionState*>(transactionTarget, transaction));
	transaction = new TransactionState();
	transactionTarget = new TransactionObject();
	transactionTarget->setUName(targetName); // Name which will be in action history
	if (targetPixmap)
		transactionTarget->setUPixmap(targetPixmap);
	if (name.length() > 0)          // if left to 0 length action will be fetched from the
		transaction->setName(name); // last added UndoState in this transaction
	if (description.length() > 0)
		transaction->setDescription(description); // tool tip for action history
	if (actionPixmap)
		transaction->setPixmap(actionPixmap); // for action history
}

void UndoManager::cancelTransaction()
{
	delete transaction;
	transaction = NULL;
	delete transactionTarget;
	transactionTarget = NULL;
	if (!transactions.empty())
	{
		// fetch the next transaction from the vector
		transactionTarget = transactions[transactions.size() - 1].first;
		transaction = transactions[transactions.size() - 1].second;
// 		delete transactions[transactions.size() - 1];
		transactions.erase(transactions.end() - 1);
	}
}

void UndoManager::commit(const QString &targetName,
                         QPixmap *targetPixmap,
                         const QString &name,
                         const QString &description,
                         QPixmap *actionPixmap)
{
	if (!transaction || !transactionTarget)
	{
		cancelTransaction();
		return;
	}
	if (targetName.length() > 0)
		transactionTarget->setUName(targetName);
	if (targetPixmap)
		transactionTarget->setUPixmap(targetPixmap);
	if (name.length() > 0)
		transaction->setName(name);
	if (description.length() > 0)
		transaction->setDescription(description);
	if (actionPixmap)
		transaction->setPixmap(actionPixmap);

	UndoObject *tmpu = transactionTarget;
	TransactionState *tmps = transaction;

	if (!transactions.empty())
	{
		// fetch the next transaction to be an active transaction
		transactionTarget = transactions[transactions.size() - 1].first;
		transaction = transactions[transactions.size() - 1].second;
// 		delete transactions[transactions.size() - 1]
		transactions.erase(transactions.end() - 1);
	}
	else
	{
		transaction = NULL;
		transactionTarget = NULL;
	}

	if (tmps->sizet() > 0) // are there any actions inside the commited transaction
	{
		if (tmps->getName() == "")
			tmps->useActionName();
		action(tmpu, tmps);
	} // if not just delete objects
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

void UndoManager::renameStack(const QString& newName)
{
	if (currentDoc == newName)
		return;
	StackPair tmp = stacks[currentDoc];
	stacks.erase(currentDoc);
	stacks[newName] = tmp;
	currentDoc = newName;
}

void UndoManager::removeStack(const QString& stackName)
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
	if (!_undoEnabled) // if so flush down the state
	{
		TransactionState *ts = dynamic_cast<TransactionState*>(state);
		if (ts) // flush the TransactionObject too
			delete target;
		delete state;
		return;
	}

	if ((!transaction) &&
        (currentUndoObjectId == -1 || currentUndoObjectId == static_cast<long>(target->getUId())))
		emit newAction(target, state); // send action to the guis

	if (stacks[currentDoc].second.size() > 1 && !transaction) // delete redo states
	{
		ActionList::iterator it;
		for (it = stacks[currentDoc].second.begin(); it != stacks[currentDoc].first; ++it)
		{
			TransactionState *ts = dynamic_cast<TransactionState*>((*it).second);
			if (ts)
			{
				delete (*it).first; // delete TransactionObject
				(*it).first = NULL;
			}
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
			if (tmpUndoObject && tmpUndoState && !ts)
				tmpUndoObject->restore(tmpUndoState, true);
			else if (tmpUndoState && ts)
				doTransactionUndo(ts);
			ts = NULL;
			++stacks[currentDoc].first;
		}

		setUndoEnabled(tmpUndoEnabled);

		if (tmpUndoState)
		{
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
		for (int i = 0; i < steps; ++i) // TODO compare to stack size too
		{
			--stacks[currentDoc].first;
			ActionPair aPair = *stacks[currentDoc].first;
			UndoObject* tmpUndoObject = aPair.first;
			tmpUndoState = aPair.second;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmpUndoState);
			if (tmpUndoObject && tmpUndoState && !ts)
				tmpUndoObject->restore(tmpUndoState, false);
			else if (tmpUndoState && ts)
				doTransactionRedo(ts);
			ts = NULL;
		}

		setUndoEnabled(tmpUndoEnabled);

		if (tmpUndoState)
		{
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

void UndoManager::replaceObject(ulong uid, UndoObject *newUndoObject)
{
	for (uint i = 0; i < stacks[currentDoc].second.size(); ++i)
	{
		ActionPair &apair = stacks[currentDoc].second[i];
		TransactionState *ts = dynamic_cast<TransactionState*>(apair.second);
		if (ts)
			ts->replace(uid, newUndoObject);
		else if (apair.first->getUId() == uid)
			apair.first = newUndoObject;
	}
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

void UndoManager::TransactionState::replace(ulong uid, UndoObject *newUndoObject)
{
	for (uint i = 0; i < states.size(); ++i)
		if (states[i]->first->getUId() == uid)
			states[i]->first = newUndoObject;
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

/*** Icons for UndoObjects *******************************************/
	UndoManager::IImageFrame      = new QPixmap(iconDir + "Bild.xpm");
	UndoManager::ITextFrame       = new QPixmap(iconDir + "Text.xpm");
	UndoManager::ILine            = new QPixmap(iconDir + "Stift.xpm");
	UndoManager::IPolygon         = new QPixmap(iconDir + "spline.png");
	UndoManager::IPolyline        = new QPixmap(iconDir + "beziertool.png");
// 	UndoManager::IPathText        = new QPixmap(iconDir + "?";
	UndoManager::IGroup           = new QPixmap(iconDir + "u_group.png");
/*** Icons for actions ***********************************************/
	UndoManager::IMove            = new QPixmap(iconDir + "u_move.png");
	UndoManager::IResize          = new QPixmap(iconDir + "u_resize.png");
	UndoManager::IRotate          = new QPixmap(iconDir + "u_rotate.png");
	UndoManager::IAlignDistribute = new QPixmap(iconDir + "u_align.png");
	UndoManager::IGuides          = new QPixmap(iconDir + "u_margins.png");
	UndoManager::ILockGuides      = new QPixmap(iconDir + "u_margins_locked.png");
}

const QString UndoManager::AddVGuide         = tr("Add vertical guide");
const QString UndoManager::AddHGuide         = tr("Add horizontal guide");
const QString UndoManager::DelVGuide         = tr("Remove vertical guide");
const QString UndoManager::DelHGuide         = tr("Remove horizontal guide");
const QString UndoManager::MoveVGuide        = tr("Move vertical guide");
const QString UndoManager::MoveHGuide        = tr("Move horizontal guide");
const QString UndoManager::LockGuides        = tr("Lock guides");
const QString UndoManager::UnlockGuides      = tr("Unlock guides");
const QString UndoManager::Move              = tr("Move");
const QString UndoManager::Resize            = tr("Resize");
const QString UndoManager::Rotate            = tr("Rotate");
const QString UndoManager::MoveFromTo        = tr("X1: %1, Y1: %2, Page %3\nX2: %4, Y2: %5, Page %6");
const QString UndoManager::ResizeFromTo      = tr("W1: %1, H1: %2\nW2: %3, H2: %4");
const QString UndoManager::RotateFromTo      = tr("From %1 to %2");
const QString UndoManager::Selection         = tr("Selection");
const QString UndoManager::Group             = tr("Group");
const QString UndoManager::Create            = tr("Create");
const QString UndoManager::CreateTo          = tr("X: %1, Y: %2\nW: %3, H: %4");
const QString UndoManager::AlignDistribute   = tr("Align/Distribute");
const QString UndoManager::ItemsInvolved     = tr("Items involved");
const QString UndoManager::Cancel            = tr("Cancel");

/*** Icons for UndoObjects *******************************************/
QPixmap *UndoManager::IImageFrame      = NULL;
QPixmap *UndoManager::ITextFrame       = NULL;
QPixmap *UndoManager::ILine            = NULL;
QPixmap *UndoManager::IPolygon         = NULL;
QPixmap *UndoManager::IPolyline        = NULL;
QPixmap *UndoManager::IPathText        = NULL;
QPixmap *UndoManager::IGroup           = NULL;
/*** Icons for actions ***********************************************/
QPixmap *UndoManager::IMove            = NULL;
QPixmap *UndoManager::IResize          = NULL;
QPixmap *UndoManager::IRotate          = NULL;
QPixmap *UndoManager::IGuides          = NULL;
QPixmap *UndoManager::ILockGuides      = NULL;
QPixmap *UndoManager::IAlignDistribute = NULL;
