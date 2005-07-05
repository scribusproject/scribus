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
#include "scconfig.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include <qvaluelist.h>

#include "scpaths.h"

extern PrefsFile *prefsFile;
extern QPixmap loadIcon(QString nam);

UndoManager* UndoManager::_instance          = 0;
bool         UndoManager::_undoEnabled       = true;
int          UndoManager::undoEnabledCounter = 0;

UndoManager* UndoManager::instance()
{
	if (_instance == 0)
		_instance = new UndoManager();

	return _instance;
}

void UndoManager::setUndoEnabled(bool isEnabled)
{
	if (isEnabled && undoEnabledCounter == 0)
		return; // nothing to do undo is already enabled.
	else if (isEnabled && undoEnabledCounter > 0)
		--undoEnabledCounter;
	else if (!isEnabled)
		++undoEnabledCounter;

	_undoEnabled = undoEnabledCounter == 0;
	if (_undoEnabled)
		connectGuis();
	else if (undoEnabledCounter == 1)
		disconnectGuis(); // disconnect only once when setUndoEnabled(false) has been called
		                  // no need to call again if next setUndoEnabled() call will also be false.
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
	historyLength = prefs->getInt("historyLength", 20);
	if (historyLength < 0)
		historyLength = 10;
	transaction = 0;
	transactionTarget = 0;
	languageChange();
}

void UndoManager::beginTransaction(const QString &targetName,
                                   QPixmap *targetPixmap,
                                   const QString &name,
                                   const QString &description,
                                   QPixmap *actionPixmap)
{
	if (!_undoEnabled)
		return;
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
	transaction = 0;
	delete transactionTarget;
	transactionTarget = 0;
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
	if (!transaction || !transactionTarget || !_undoEnabled)
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
		transaction = 0;
		transactionTarget = 0;
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
		tmpu = 0;
		delete tmps;
		tmps = 0;
	}
}

bool UndoManager::isTransactionMode()
{
	return transaction ? true : false;
}

void UndoManager::registerGui(UndoGui* gui)
{
	if (gui == 0)
		return;

	setUndoEnabled(false);
	setState(gui);
	undoGuis.push_back(gui);
	setUndoEnabled(true);
}

void UndoManager::setState(UndoGui* gui, int uid)
{
	gui->clear();

	ActionList::iterator itstart = stacks[currentDoc].second.begin() - 1;;
	ActionList::iterator itend   = stacks[currentDoc].second.end() - 1;;
	if (uid > -1)
	{ // find the range from where actions are added when in obj. spec. mode
		ActionList::iterator it2;
		for (it2 = stacks[currentDoc].first - 1; it2 != stacks[currentDoc].second.begin() - 1; --it2)
		{
			UndoState*  tmp  = (*it2).second;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
			if (ts && ts->contains(uid))
			{
				itstart = it2 - 1;
				break;
			}
		}
		for (it2 = stacks[currentDoc].first; it2 != stacks[currentDoc].second.end(); ++it2)
		{
			UndoState*  tmp  = (*it2).second;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
			if (ts && ts->contains(uid))
			{
				itend = it2 - 1;
				break;
			}
		}
	}

	for (; itend > itstart; --itend)
	{
		ActionPair pair = *itend;
		UndoObject* target = pair.first;
		UndoState*  state  = pair.second;

		if (uid == -1 || target->getUId() == static_cast<uint>(uid))
		{
			if (itend > stacks[currentDoc].first - 1)
				gui->insertUndoItem(target, state);
			else
				gui->insertRedoItem(target, state);
		}
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
		connect(this, SIGNAL(clearRedo()), gui, SLOT(clearRedo()));
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
		disconnect(this, SIGNAL(clearRedo()), gui, SLOT(clearRedo()));
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

	if (stacks[currentDoc].second.empty()) {
		currentDoc = newName;
		return;
	}
	
	ActionPair pair = *(stacks[currentDoc].first);
	StackPair tmp(stacks[currentDoc].first, stacks[currentDoc].second);
	stacks.erase(currentDoc);
	stacks[newName] = tmp;
	currentDoc = newName;
	ActionList::iterator it;
	for (it = stacks[currentDoc].second.begin(); it != stacks[currentDoc].second.end(); ++it)
	{
		if ((*it) == pair)
			break;
	}
	stacks[currentDoc].first = it;
}

void UndoManager::removeStack(const QString& stackName)
{
	if (stacks.contains(stackName))
	{
		for (uint i = 0; i < stacks[stackName].second.size(); ++i)
		{
			delete stacks[stackName].second[i].second;
			stacks[stackName].second[i].second = 0;
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

void UndoManager::action(UndoObject* target, UndoState* state, QPixmap *targetPixmap)
{
	QPixmap *oldIcon = 0;
	if (targetPixmap)
	{
		oldIcon = target->getUPixmap();
		target->setUPixmap(targetPixmap);
	}

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
	else
		emit clearRedo();

	if (stacks[currentDoc].second.size() > 1 && !transaction) // delete redo states
	{
		ActionList::iterator it;
		for (it = stacks[currentDoc].second.begin(); it != stacks[currentDoc].first; ++it)
		{
			TransactionState *ts = dynamic_cast<TransactionState*>((*it).second);
			if (ts)
			{
				delete (*it).first; // delete TransactionObject
				(*it).first = 0;
			}
			delete (*it).second;
			(*it).second = 0;
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
	if (targetPixmap)
		target->setUPixmap(oldIcon);
}

void UndoManager::action(UndoObject* target, UndoState* state,
                         const QString &targetName, QPixmap *targetPixmap)
{
	QString oldName = target->getUName();
	if (targetName != "")
		target->setUName(targetName);
	action(target, state, targetPixmap);
	target->setUName(oldName);
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
		setUndoEnabled(false);
		UndoState* tmpUndoState = 0;
		currentAction = stacks[currentDoc].first;
		for (int i = 0; i < steps; ++i)
		{
			ActionPair &aPair = getNextUndoPair();
			UndoObject* tmpUndoObject = aPair.first;
			tmpUndoState = aPair.second;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmpUndoState);

			if (tmpUndoObject && tmpUndoState && !ts)
				tmpUndoObject->restore(tmpUndoState, true);
			else if (tmpUndoState && ts)
				doTransactionUndo(ts);
			ts = 0;
		}
		reorderUndoStack(steps);
		setUndoEnabled(true);
		if (tmpUndoState)
		{
			emit undoSignal(steps);
			emit undoRedoDone();
		}
	}
}

ActionPair& UndoManager::getNextUndoPair()
{
	if (currentUndoObjectId == GLOBAL_UNDO_MODE)
		++currentAction;
	else
		for (; currentAction != stacks[currentDoc].second.end(); ++currentAction)
			if ((*currentAction).first->getUId() == static_cast<uint>(currentUndoObjectId))
			{
				++currentAction;
				break;
			}
	return *(currentAction - 1);
}

void UndoManager::reorderUndoStack(int steps)
{
	if (currentUndoObjectId == GLOBAL_UNDO_MODE)
		stacks[currentDoc].first += steps;
	else
	{
		int pos = -1;
		ActionList::iterator it;
		for (it = stacks[currentDoc].second.begin(); it != stacks[currentDoc].first + 1; ++it)
		{
			++pos;
		}

		// TODO Move the items around here
		for (int i = 0; i < steps; ++i)
		{
			ActionList::iterator it = stacks[currentDoc].second.begin() + pos;
			while ((*it).first->getUId() != static_cast<uint>(currentUndoObjectId)) ++it;
			ActionPair pair = *it;
			stacks[currentDoc].second.erase(it);
			stacks[currentDoc].second.insert(stacks[currentDoc].second.begin() + pos, pair);
			++pos;
		}

		stacks[currentDoc].first = stacks[currentDoc].second.begin() += pos;
	}
}

void UndoManager::doTransactionUndo(TransactionState *tstate)
{
	for (int i = tstate->sizet() - 1; i > -1; --i)
	{
		ActionPair *pair = tstate->at(i);
		TransactionState *ts = dynamic_cast<TransactionState*>(pair->second);
		if (pair && pair->first && pair->second && !ts)
			pair->first->restore(pair->second, true);
		else if (pair && pair->second && ts)
			doTransactionUndo(ts);
	}
}

void UndoManager::doRedo(int steps)
{
	if (steps > 0 && _undoEnabled && stacks.size() > 0)
	{
		setUndoEnabled(false);
		UndoState* tmpUndoState = 0;
		currentAction = stacks[currentDoc].first;
		for (int i = 0; i < steps; ++i) // TODO compare to stack size too
		{
			ActionPair &aPair = getNextRedoPair();
			UndoObject* tmpUndoObject = aPair.first;
			tmpUndoState = aPair.second;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmpUndoState);

			if (tmpUndoObject && tmpUndoState && !ts)
				tmpUndoObject->restore(tmpUndoState, false);
			else if (tmpUndoState && ts)
				doTransactionRedo(ts);
			ts = 0;
		}

		reorderRedoStack(steps);
		setUndoEnabled(true);

		if (tmpUndoState)
		{
			emit redoSignal(steps);
			emit undoRedoDone();
		}
	}
}

ActionPair& UndoManager::getNextRedoPair()
{
	if (currentUndoObjectId == GLOBAL_UNDO_MODE)
		--currentAction;
	else
		for (--currentAction; currentAction != stacks[currentDoc].second.begin() - 1; --currentAction)
			if ((*currentAction).first->getUId() == static_cast<uint>(currentUndoObjectId))
				break;

	return *currentAction;
}

void UndoManager::reorderRedoStack(int steps)
{
	if (currentUndoObjectId == GLOBAL_UNDO_MODE)
		stacks[currentDoc].first -= steps;
	else
	{
		int pos = -1;
		ActionList::iterator it;
		for (it = stacks[currentDoc].second.begin(); it != stacks[currentDoc].first + 1; ++it)
			++pos;
		for (int i = 0; i < steps; ++i)
		{
			ActionList::iterator it = stacks[currentDoc].second.begin() + pos - 1;
			while ((*it).first->getUId() != static_cast<uint>(currentUndoObjectId)) --it;
			ActionPair pair = *it;
			stacks[currentDoc].second.erase(it);
			--pos;
			stacks[currentDoc].second.insert(stacks[currentDoc].second.begin() + pos, pair);
		}
		stacks[currentDoc].first = stacks[currentDoc].second.begin() += pos;
	}
}

void UndoManager::doTransactionRedo(TransactionState *tstate)
{
	for (uint i = 0; i < tstate->sizet(); ++i)
	{
		ActionPair *pair = tstate->at(i);
		TransactionState *ts = dynamic_cast<TransactionState*>(pair->second);
		if (pair && pair->first && pair->second && !ts)
			pair->first->restore(pair->second, false);
		else if (pair && pair->second && ts)
			doTransactionRedo(ts);
	}
}

bool UndoManager::hasUndoActions(int uid)
{
	// TODO Needs to fixed for object specific mode
	return stacks[currentDoc].first < stacks[currentDoc].second.end();
}

bool UndoManager::hasRedoActions(int uid)
{
	// TODO Needs to be fixed for object specific mode
	return stacks[currentDoc].first > stacks[currentDoc].second.begin();
}

void UndoManager::showObject(int uid)
{
	if (currentUndoObjectId == uid)
		return;
	setUndoEnabled(false);
	currentUndoObjectId = uid;
	for (uint i = 0; i < undoGuis.size(); ++i)
	{
		if (uid == -2)
			undoGuis[i]->clear();
		else
			setState(undoGuis[i], currentUndoObjectId);
	}
	setUndoEnabled(true);
}

UndoObject* UndoManager::replaceObject(ulong uid, UndoObject *newUndoObject)
{
	UndoObject *tmp = 0;
	for (uint i = 0; i < stacks[currentDoc].second.size(); ++i)
	{
		ActionPair &apair = stacks[currentDoc].second[i];
		TransactionState *ts = dynamic_cast<TransactionState*>(apair.second);
		if (ts)
			tmp = ts->replace(uid, newUndoObject);
		else if (apair.first && apair.first->getUId() == uid)
		{
			tmp = apair.first;
			apair.first = newUndoObject;
		}
	}
	if (transaction) // replace also in the currently open transaction
		tmp = transaction->replace(uid, newUndoObject);
	return tmp;
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

bool UndoManager::isGlobalMode()
{
	return currentUndoObjectId == -1;
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
	_instance = 0;
}

UndoManager::~UndoManager()
{
	StackMap::iterator it;
	for (it = stacks.begin(); it != stacks.end(); ++it)
	{
		for (uint i = 0; i < (*it).second.size(); ++i)
		{
			delete ((*it).second)[i].second;
			((*it).second)[i].second = 0;
		}
	}
}


void UndoManager::languageChange()
{
	UndoManager::AddVGuide          = tr("Add vertical guide");
	UndoManager::AddHGuide          = tr("Add horizontal guide");
	UndoManager::DelVGuide          = tr("Remove vertical guide");
	UndoManager::DelHGuide          = tr("Remove horizontal guide");
	UndoManager::MoveVGuide         = tr("Move vertical guide");
	UndoManager::MoveHGuide         = tr("Move horizontal guide");
	UndoManager::LockGuides         = tr("Lock guides");
	UndoManager::UnlockGuides       = tr("Unlock guides");
	UndoManager::Move               = tr("Move");
	UndoManager::Resize             = tr("Resize");
	UndoManager::Rotate             = tr("Rotate");
	UndoManager::MoveFromTo         = tr("X1: %1, Y1: %2, %3\nX2: %4, Y2: %5, %6");
	UndoManager::ResizeFromTo       = tr("W1: %1, H1: %2\nW2: %3, H2: %4");
	UndoManager::Selection          = tr("Selection");
	UndoManager::Group              = tr("Group");
	UndoManager::SelectionGroup     = tr("Selection/Group");
	UndoManager::Create             = tr("Create");
	UndoManager::CreateTo           = tr("X: %1, Y: %2\nW: %3, H: %4");
	UndoManager::AlignDistribute    = tr("Align/Distribute");
	UndoManager::ItemsInvolved      = tr("Items involved");
	UndoManager::Cancel             = tr("Cancel");
	UndoManager::SetFill            = tr("Set fill color");
	UndoManager::ColorFromTo        = tr("Color1: %1, Color2: %2");
	UndoManager::SetShade           = tr("Set fill color shade");
	UndoManager::SetLineColor       = tr("Set line color");
	UndoManager::SetLineShade       = tr("Set line color shade");
	UndoManager::FlipH              = tr("Flip horizontally");
	UndoManager::FlipV              = tr("Flip vertically");
	UndoManager::Lock               = tr("Lock");
	UndoManager::UnLock             = tr("Unlock");
	UndoManager::SizeLock           = tr("Lock size");
	UndoManager::SizeUnLock         = tr("Unlock size");
	UndoManager::Ungroup            = tr("Ungroup");
	UndoManager::Delete             = tr("Delete");
	UndoManager::Rename             = tr("Rename");
	UndoManager::FromTo             = tr("From %1\nto %2");
	UndoManager::ApplyMasterPage    = tr("Apply Master Page");
	UndoManager::Paste              = tr("Paste");
	UndoManager::Cut                = tr("Cut");
	UndoManager::Transparency       = tr("Set fill color transparency");
	UndoManager::LineTransparency   = tr("Set line color transparency");
	UndoManager::LineStyle          = tr("Set line style");
	UndoManager::LineEnd            = tr("Set the style of line end");
	UndoManager::LineJoin           = tr("Set the style of line join");
	UndoManager::LineWidth          = tr("Set line width");
	UndoManager::NoStyle            = tr("No style");
	UndoManager::CustomLineStyle    = tr("Set custom line style");
	UndoManager::NoLineStyle        = tr("Do not use custom line style");
	UndoManager::StartArrow         = tr("Set start arrow");
	UndoManager::EndArrow           = tr("Set end arrow");
	UndoManager::CreateTable        = tr("Create table");
	UndoManager::RowsCols           = tr("Rows: %1, Cols: %2");
	UndoManager::SetFont            = tr("Set font");
	UndoManager::SetFontSize        = tr("Set font size");
	UndoManager::SetFontWidth       = tr("Set font width");
	UndoManager::SetFontHeight       = tr("Set font height");
	UndoManager::SetFontFill        = tr("Set font fill color");
	UndoManager::SetFontStroke      = tr("Set font stroke color");
	UndoManager::SetFontFillShade   = tr("Set font fill color shade");
	UndoManager::SetFontStrokeShade = tr("Set font stroke color shade");
	UndoManager::SetKerning         = tr("Set kerning");
	UndoManager::SetLineSpacing     = tr("Set line spacing");
	UndoManager::SetStyle           = tr("Set paragraph style");
	UndoManager::SetLanguage        = tr("Set language");
	UndoManager::AlignText          = tr("Align text");
	UndoManager::SetFontEffect      = tr("Set font effect");
	UndoManager::ImageFrame         = tr("Image frame");
	UndoManager::TextFrame          = tr("Text frame");
	UndoManager::Polygon            = tr("Polygon");
	UndoManager::BezierCurve        = tr("Bezier curve");
	UndoManager::Polyline           = tr("Polyline");
	UndoManager::ConvertTo          = tr("Convert to");
	UndoManager::ImportSVG          = tr("Import SVG image");
	UndoManager::ImportEPS          = tr("Import EPS image");
	UndoManager::ImportOOoDraw      = tr("Import OpenOffice.org Draw image");
	UndoManager::ScratchSpace       = tr("Scratch space");
	UndoManager::TextFlow           = tr("Text flows around the frame");
	UndoManager::BoundingBox        = tr("Text flows around bounding box");
	UndoManager::ContourLine        = tr("Text flows around contour line");
	UndoManager::NoTextFlow         = tr("No text flow");
	UndoManager::NoBoundingBox      = tr("No bounding box");
	UndoManager::NoContourLine      = tr("No contour line");
	UndoManager::PageNmbr           = tr("Page %1");
	UndoManager::ImageScaling       = tr("Set image scaling");
	UndoManager::FrameSize          = tr("Frame size");
	UndoManager::FreeScaling        = tr("Free scaling");
	UndoManager::KeepRatio          = tr("Keep aspect ratio");
	UndoManager::BreakRatio         = tr("Break aspect ratio");
	UndoManager::EditContourLine    = tr("Edit contour line");
	UndoManager::EditShape          = tr("Edit shape");
	UndoManager::ResetContourLine   = tr("Reset contour line");
	UndoManager::AddPage            = tr("Add page");
	UndoManager::AddPages           = tr("Add pages");
	UndoManager::DeletePage         = tr("Delete page");
	UndoManager::DeletePages        = tr("Delete pages");
	UndoManager::AddLayer           = tr("Add layer");
	UndoManager::DeleteLayer        = tr("Delete layer");
	UndoManager::RenameLayer        = tr("Rename layer");
	UndoManager::RaiseLayer         = tr("Raise layer");
	UndoManager::LowerLayer         = tr("Lower layer");
	UndoManager::SendToLayer        = tr("Send to layer");
	UndoManager::PrintLayer         = tr("Enable printing of layer");
	UndoManager::DoNotPrintLayer    = tr("Disable printing of layer");
	UndoManager::SetLayerName       = tr("Change name of the layer");
	UndoManager::GetImage           = tr("Get image");
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
		return 0;
}

bool UndoManager::TransactionState::contains(int uid)
{
	bool ret = false;
	for (uint i = 0; i < states.size(); ++i)
	{
		if (states[i]->first->getUId() == static_cast<uint>(uid))
		{
			ret = true;
			break;
		}
	}
	return ret;
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

UndoObject* UndoManager::TransactionState::replace(ulong uid, UndoObject *newUndoObject)
{
	UndoObject *tmp = 0;
	for (uint i = 0; i < states.size(); ++i)
	{
		TransactionState *ts = dynamic_cast<TransactionState*>(states[i]->second);
		if (ts) // are we having a transaction inside a transaction
			ts->replace(uid, newUndoObject);
		else if (states[i]->first && states[i]->first->getUId() == uid)
		{
			tmp = states[i]->first;
			states[i]->first = newUndoObject;
		}
	}
	return tmp;
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
				states[i]->second = 0;
			}
			delete states[i];
			states[i] = 0;
		}
	}
}

/*************************************************************************************/

void UndoManager::initIcons()
{
	QString iconDir = ScPaths::instance().iconDir();

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
	UndoManager::IFill            = new QPixmap(iconDir + "u_fill.png");
	UndoManager::IShade           = new QPixmap(iconDir + "u_shade.png");
	UndoManager::IFlipH           = new QPixmap(iconDir + "u_fliph.png");
	UndoManager::IFlipV           = new QPixmap(iconDir + "u_flipv.png");
	UndoManager::ILock            = new QPixmap(iconDir + "u_lock.png");
	UndoManager::IUnLock          = new QPixmap(iconDir + "u_unlock.png");
	UndoManager::IDelete          = new QPixmap(iconDir + "u_delete.png");
	UndoManager::ICreate          = new QPixmap(iconDir + "u_create.png");
	UndoManager::IPaste           = new QPixmap(iconDir + "editpaste.png");
	UndoManager::ICut             = new QPixmap(iconDir + "u_cut.png");
	UndoManager::ITransparency    = new QPixmap(iconDir + "u_transp.png");
	UndoManager::ILineStyle       = new QPixmap(iconDir + "u_line.png");
	UndoManager::IArrow           = new QPixmap(iconDir + "u_arrow.png");
	UndoManager::ITable           = new QPixmap(iconDir + "frame_table.png");
	UndoManager::IFont            = new QPixmap(iconDir + "u_font.png");
	UndoManager::IImportOOoDraw   = new QPixmap(iconDir + "ooo_draw.png");
	UndoManager::ISVG             = new QPixmap(iconDir + "u_svg.png");
	UndoManager::IEPS             = new QPixmap(iconDir + "u_eps.png");
	UndoManager::IImageScaling    = new QPixmap(iconDir + "u_scale_image.png");
	UndoManager::IBorder          = new QPixmap(iconDir + "u_shape.png");
	UndoManager::IDocument        = new QPixmap(iconDir + "DateiNeu.xpm");
// 	UndoManager::ILayer           = new QPixmap(iconDir + "u_layer.png");
// 	UndoManager::ILayerAction     = new QPixmap(iconDir + "u_layer_action.png");
// 	UndoManager::IUp              = new QPixmap(iconDir + "u_up.png");
// 	UndoManager::IDown            = new QPixmap(iconDir + "u_down.png");
// 	UndoManager::IGetImage        = new QPixmap(iconDir + "u_get_image.png");
}
QString UndoManager::AddVGuide          = "";
QString UndoManager::AddHGuide          = "";
QString UndoManager::DelVGuide          = "";
QString UndoManager::DelHGuide          = "";
QString UndoManager::MoveVGuide         = "";
QString UndoManager::MoveHGuide         = "";
QString UndoManager::LockGuides         = "";
QString UndoManager::UnlockGuides       = "";
QString UndoManager::Move               = "";
QString UndoManager::Resize             = "";
QString UndoManager::Rotate             = "";
QString UndoManager::MoveFromTo         = "";
QString UndoManager::ResizeFromTo       = "";
QString UndoManager::Selection          = "";
QString UndoManager::Group              = "";
QString UndoManager::SelectionGroup     = "";
QString UndoManager::Create             = "";
QString UndoManager::CreateTo           = "";
QString UndoManager::AlignDistribute    = "";
QString UndoManager::ItemsInvolved      = "";
QString UndoManager::Cancel             = "";
QString UndoManager::SetFill            = "";
QString UndoManager::ColorFromTo        = "";
QString UndoManager::SetShade           = "";
QString UndoManager::SetLineColor       = "";
QString UndoManager::SetLineShade       = "";
QString UndoManager::FlipH              = "";
QString UndoManager::FlipV              = "";
QString UndoManager::Lock               = "";
QString UndoManager::UnLock             = "";
QString UndoManager::SizeLock           = "";
QString UndoManager::SizeUnLock         = "";
QString UndoManager::Ungroup            = "";
QString UndoManager::Delete             = "";
QString UndoManager::Rename             = "";
QString UndoManager::FromTo             = "";
QString UndoManager::ApplyMasterPage    = "";
QString UndoManager::Paste              = "";
QString UndoManager::Cut                = "";
QString UndoManager::Transparency       = "";
QString UndoManager::LineTransparency   = "";
QString UndoManager::LineStyle          = "";
QString UndoManager::LineEnd            = "";
QString UndoManager::LineJoin           = "";
QString UndoManager::LineWidth          = "";
QString UndoManager::NoStyle            = "";
QString UndoManager::CustomLineStyle    = "";
QString UndoManager::NoLineStyle        = "";
QString UndoManager::StartArrow         = "";
QString UndoManager::EndArrow           = "";
QString UndoManager::CreateTable        = "";
QString UndoManager::RowsCols           = "";
QString UndoManager::SetFont            = "";
QString UndoManager::SetFontSize        = "";
QString UndoManager::SetFontWidth       = "";
QString UndoManager::SetFontHeight       = "";
QString UndoManager::SetFontFill        = "";
QString UndoManager::SetFontStroke      = "";
QString UndoManager::SetFontFillShade   = "";
QString UndoManager::SetFontStrokeShade = "";
QString UndoManager::SetKerning         = "";
QString UndoManager::SetLineSpacing     = "";
QString UndoManager::SetStyle           = "";
QString UndoManager::SetLanguage        = "";
QString UndoManager::AlignText          = "";
QString UndoManager::SetFontEffect      = "";
QString UndoManager::ImageFrame         = "";
QString UndoManager::TextFrame          = "";
QString UndoManager::Polygon            = "";
QString UndoManager::BezierCurve        = "";
QString UndoManager::Polyline           = "";
QString UndoManager::ConvertTo          = "";
QString UndoManager::ImportSVG          = "";
QString UndoManager::ImportEPS          = "";
QString UndoManager::ImportOOoDraw      = "";
QString UndoManager::ScratchSpace       = "";
QString UndoManager::TextFlow           = "";
QString UndoManager::BoundingBox        = "";
QString UndoManager::ContourLine        = "";
QString UndoManager::NoTextFlow         = "";
QString UndoManager::NoBoundingBox      = "";
QString UndoManager::NoContourLine      = "";
QString UndoManager::PageNmbr           = "";
QString UndoManager::ImageScaling       = "";
QString UndoManager::FrameSize          = "";
QString UndoManager::FreeScaling        = "";
QString UndoManager::KeepRatio          = "";
QString UndoManager::BreakRatio         = "";
QString UndoManager::EditContourLine    = "";
QString UndoManager::EditShape          = "";
QString UndoManager::ResetContourLine   = "";
QString UndoManager::AddPage            = "";
QString UndoManager::AddPages           = "";
QString UndoManager::DeletePage         = "";
QString UndoManager::DeletePages        = "";
QString UndoManager::AddLayer           = "";
QString UndoManager::DeleteLayer        = "";
QString UndoManager::RenameLayer        = "";
QString UndoManager::RaiseLayer         = "";
QString UndoManager::LowerLayer         = "";
QString UndoManager::SendToLayer        = "";
QString UndoManager::PrintLayer         = "";
QString UndoManager::DoNotPrintLayer    = "";
QString UndoManager::SetLayerName       = "";
QString UndoManager::GetImage           = "";
/*
const QString UndoManager::AddVGuide          = tr("Add vertical guide");
const QString UndoManager::AddHGuide          = tr("Add horizontal guide");
const QString UndoManager::DelVGuide          = tr("Remove vertical guide");
const QString UndoManager::DelHGuide          = tr("Remove horizontal guide");
const QString UndoManager::MoveVGuide         = tr("Move vertical guide");
const QString UndoManager::MoveHGuide         = tr("Move horizontal guide");
const QString UndoManager::LockGuides         = tr("Lock guides");
const QString UndoManager::UnlockGuides       = tr("Unlock guides");
const QString UndoManager::Move               = tr("Move");
const QString UndoManager::Resize             = tr("Resize");
const QString UndoManager::Rotate             = tr("Rotate");
const QString UndoManager::MoveFromTo         = tr("X1: %1, Y1: %2, %3\nX2: %4, Y2: %5, %6");
const QString UndoManager::ResizeFromTo       = tr("W1: %1, H1: %2\nW2: %3, H2: %4");
const QString UndoManager::Selection          = tr("Selection");
const QString UndoManager::Group              = tr("Group");
const QString UndoManager::SelectionGroup     = tr("Selection/Group");
const QString UndoManager::Create             = tr("Create");
const QString UndoManager::CreateTo           = tr("X: %1, Y: %2\nW: %3, H: %4");
const QString UndoManager::AlignDistribute    = tr("Align/Distribute");
const QString UndoManager::ItemsInvolved      = tr("Items involved");
const QString UndoManager::Cancel             = tr("Cancel");
const QString UndoManager::SetFill            = tr("Set fill color");
const QString UndoManager::ColorFromTo        = tr("Color1: %1, Color2: %2");
const QString UndoManager::SetShade           = tr("Set fill color shade");
const QString UndoManager::SetLineColor       = tr("Set line color");
const QString UndoManager::SetLineShade       = tr("Set line color shade");
const QString UndoManager::FlipH              = tr("Flip horizontally");
const QString UndoManager::FlipV              = tr("Flip vertically");
const QString UndoManager::Lock               = tr("Lock");
const QString UndoManager::UnLock             = tr("Unlock");
const QString UndoManager::SizeLock           = tr("Lock size");
const QString UndoManager::SizeUnLock         = tr("Unlock size");
const QString UndoManager::Ungroup            = tr("Ungroup");
const QString UndoManager::Delete             = tr("Delete");
const QString UndoManager::Rename             = tr("Rename");
const QString UndoManager::FromTo             = tr("From %1\nto %2");
const QString UndoManager::ApplyMasterPage    = tr("Apply Master Page");
const QString UndoManager::Paste              = tr("Paste");
const QString UndoManager::Cut                = tr("Cut");
const QString UndoManager::Transparency       = tr("Set fill color transparency");
const QString UndoManager::LineTransparency   = tr("Set line color transparency");
const QString UndoManager::LineStyle          = tr("Set line style");
const QString UndoManager::LineEnd            = tr("Set the style of line end");
const QString UndoManager::LineJoin           = tr("Set the style of line join");
const QString UndoManager::LineWidth          = tr("Set line width");
const QString UndoManager::NoStyle            = tr("No style");
const QString UndoManager::CustomLineStyle    = tr("Set custom line style");
const QString UndoManager::NoLineStyle        = tr("Do not use custom line style");
const QString UndoManager::StartArrow         = tr("Set start arrow");
const QString UndoManager::EndArrow           = tr("Set end arrow");
const QString UndoManager::CreateTable        = tr("Create table");
const QString UndoManager::RowsCols           = tr("Rows: %1, Cols: %2");
const QString UndoManager::SetFont            = tr("Set font");
const QString UndoManager::SetFontSize        = tr("Set font size");
const QString UndoManager::SetFontWidth       = tr("Set font width");
const QString UndoManager::SetFontFill        = tr("Set font fill color");
const QString UndoManager::SetFontStroke      = tr("Set font stroke color");
const QString UndoManager::SetFontFillShade   = tr("Set font fill color shade");
const QString UndoManager::SetFontStrokeShade = tr("Set font stroke color shade");
const QString UndoManager::SetKerning         = tr("Set kerning");
const QString UndoManager::SetLineSpacing     = tr("Set line spacing");
const QString UndoManager::SetStyle           = tr("Set paragraph style");
const QString UndoManager::SetLanguage        = tr("Set language");
const QString UndoManager::AlignText          = tr("Align text");
const QString UndoManager::SetFontEffect      = tr("Set font effect");
const QString UndoManager::ImageFrame         = tr("Image frame");
const QString UndoManager::TextFrame          = tr("Text frame");
const QString UndoManager::Polygon            = tr("Polygon");
const QString UndoManager::BezierCurve        = tr("Bezier curve");
const QString UndoManager::Polyline           = tr("Polyline");
const QString UndoManager::ConvertTo          = tr("Convert to");
const QString UndoManager::ImportSVG          = tr("Import SVG image");
const QString UndoManager::ImportEPS          = tr("Import EPS image");
const QString UndoManager::ImportOOoDraw      = tr("Import OpenOffice.org Draw image");
const QString UndoManager::ScratchSpace       = tr("Scratch space");
const QString UndoManager::TextFlow           = tr("Text flows around the frame");
const QString UndoManager::BoundingBox        = tr("Text flows around bounding box");
const QString UndoManager::ContourLine        = tr("Text flows around contour line");
const QString UndoManager::NoTextFlow         = tr("No text flow");
const QString UndoManager::NoBoundingBox      = tr("No bounding box");
const QString UndoManager::NoContourLine      = tr("No contour line");
const QString UndoManager::PageNmbr           = tr("Page %1");
const QString UndoManager::ImageScaling       = tr("Set image scaling");
const QString UndoManager::FrameSize          = tr("Frame size");
const QString UndoManager::FreeScaling        = tr("Free scaling");
const QString UndoManager::KeepRatio          = tr("Keep aspect ratio");
const QString UndoManager::BreakRatio         = tr("Break aspect ratio");
const QString UndoManager::EditContourLine    = tr("Edit contour line");
const QString UndoManager::EditShape          = tr("Edit shape");
const QString UndoManager::ResetContourLine   = tr("Reset contour line");
const QString UndoManager::AddPage            = tr("Add page");
const QString UndoManager::AddPages           = tr("Add pages");
const QString UndoManager::DeletePage         = tr("Delete page");
const QString UndoManager::DeletePages        = tr("Delete pages");
const QString UndoManager::AddLayer           = tr("Add layer");
const QString UndoManager::DeleteLayer        = tr("Delete layer");
const QString UndoManager::RenameLayer        = tr("Rename layer");
const QString UndoManager::RaiseLayer         = tr("Raise layer");
const QString UndoManager::LowerLayer         = tr("Lower layer");
const QString UndoManager::SendToLayer        = tr("Send to layer");
const QString UndoManager::PrintLayer         = tr("Enable printing of layer");
const QString UndoManager::DoNotPrintLayer    = tr("Disable printing of layer");
const QString UndoManager::SetLayerName       = tr("Change name of the layer");
const QString UndoManager::GetImage           = tr("Get image");
*/
/*** Icons for UndoObjects *******************************************/
QPixmap *UndoManager::IImageFrame      = 0;
QPixmap *UndoManager::ITextFrame       = 0;
QPixmap *UndoManager::ILine            = 0;
QPixmap *UndoManager::IPolygon         = 0;
QPixmap *UndoManager::IPolyline        = 0;
QPixmap *UndoManager::IPathText        = 0;
QPixmap *UndoManager::IGroup           = 0;
QPixmap *UndoManager::ITable           = 0;
/*** Icons for actions ***********************************************/
QPixmap *UndoManager::IMove            = 0;
QPixmap *UndoManager::IResize          = 0;
QPixmap *UndoManager::IRotate          = 0;
QPixmap *UndoManager::IGuides          = 0;
QPixmap *UndoManager::ILockGuides      = 0;
QPixmap *UndoManager::IAlignDistribute = 0;
QPixmap *UndoManager::IFill            = 0;
QPixmap *UndoManager::IShade           = 0;
QPixmap *UndoManager::IFlipH           = 0;
QPixmap *UndoManager::IFlipV           = 0;
QPixmap *UndoManager::ILock            = 0;
QPixmap *UndoManager::IUnLock          = 0;
QPixmap *UndoManager::IDelete          = 0;
QPixmap *UndoManager::ICreate          = 0;
QPixmap *UndoManager::IPaste           = 0;
QPixmap *UndoManager::ICut             = 0;
QPixmap *UndoManager::ITransparency    = 0;
QPixmap *UndoManager::ILineStyle       = 0;
QPixmap *UndoManager::IArrow           = 0;
QPixmap *UndoManager::IFont            = 0;
QPixmap *UndoManager::ISVG             = 0;
QPixmap *UndoManager::IEPS             = 0;
QPixmap *UndoManager::IImportOOoDraw   = 0;
QPixmap *UndoManager::IImageScaling    = 0;
QPixmap *UndoManager::IBorder          = 0;
QPixmap *UndoManager::IDocument        = 0;
QPixmap *UndoManager::ILayer           = 0;
QPixmap *UndoManager::ILayerAction     = 0;
QPixmap *UndoManager::IUp              = 0;
QPixmap *UndoManager::IDown            = 0;
QPixmap *UndoManager::IPrint           = 0;
QPixmap *UndoManager::IGetImage        = 0;

