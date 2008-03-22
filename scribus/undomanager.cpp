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

#include <QDebug>
#include <QList>
#include <QPixmap>

#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "scpaths.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undogui.h"
#include "undostack.h"
#include "util_icon.h"


/**************************************************************************************/

/**
  This struct is use instead of std::pair<TransactionObject*, TransactionState*> now.
  Just data, no methods.
*/
struct TransactionData : public Transaction::TransactionStateBase
{
	int stackLevel;
	UndoManager* UM;
	UndoObject* transactionObject;  // will be DummyUndoObject
	TransactionState* transactionState;
};


/**************************************************************************************/



UndoManager* UndoManager::instance_          = 0;
bool         UndoManager::undoEnabled_       = true;
int          UndoManager::undoEnabledCounter_ = 0;

UndoManager* UndoManager::instance()
{
	if (instance_ == 0)
		instance_ = new UndoManager();

	return instance_;
}

void UndoManager::setUndoEnabled(bool isEnabled)
{
	if (isEnabled && undoEnabledCounter_ == 0)
		return; // nothing to do undo is already enabled.
	else if (isEnabled && undoEnabledCounter_ > 0)
		--undoEnabledCounter_;
	else if (!isEnabled)
		++undoEnabledCounter_;

	undoEnabled_ = undoEnabledCounter_ == 0;
	if (undoEnabled_)
		connectGuis();
	else if (undoEnabledCounter_ == 1)
		disconnectGuis(); // disconnect only once when setUndoEnabled(false) has been called
		                  // no need to call again if next setUndoEnabled() call will also be false.
}

bool UndoManager::undoEnabled()
{
	return undoEnabled_;
}

UndoManager::UndoManager()
{
	currentUndoObjectId_ = -1;
	if (!UndoManager::IGuides)
		initIcons();
	prefs_ = PrefsManager::instance()->prefsFile->getContext("undo");
	languageChange();
	setUndoEnabled(prefs_->getBool("enabled", true));
}



UndoTransaction UndoManager::beginTransaction(const QString &targetName,
											  QPixmap *targetPixmap,
											  const QString &name,
											  const QString &description,
											  QPixmap *actionPixmap)
{
	if (!undoEnabled_)
		return UndoTransaction(NULL);
	
	/** @brief Dummy object for storing transaction target's name */
	UndoObject* transactionTarget_ = new DummyUndoObject();
	TransactionState* transactionState_ = new TransactionState();
	transactionTarget_->setUName(targetName); // Name which will be in action history
	if (targetPixmap)
		transactionTarget_->setUPixmap(targetPixmap);
	if (name.length() > 0)          // if left to 0 length action will be fetched from the
		transactionState_->setName(name); // last added UndoState in this transaction
	if (description.length() > 0)
		transactionState_->setDescription(description); // tool tip for action history
	if (actionPixmap)
		transactionState_->setPixmap(actionPixmap); // for action history

	// Holds the state and data of this transaction:
	TransactionData *transaction = new TransactionData();
	transaction->transactionObject = transactionTarget_;
	transaction->transactionState = transactionState_;
	transaction->stackLevel = transactions_.size();
	transaction->UM = this;
	
	transactions_.push_back(transaction);
	
//	qDebug() << "UndoManager::beginTransaction" << targetName << name << transaction;
	return UndoTransaction(transaction);
}

UndoTransaction::UndoTransaction(TransactionData* data) : Transaction(data) 
{};

UndoTransaction::~UndoTransaction() 
{
	if (m_data)
	{
		UndoTransaction::cancel(); // no virtual calls in destructor
		delete m_data; 
		m_data = 0;
	}
}

bool UndoTransaction::cancel()
{
	if (!m_data)
		return false;
	
	TransactionData* data = static_cast<TransactionData*>(m_data);
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
		TransactionData* data = static_cast<TransactionData*>(m_data);
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
	TransactionData* data = static_cast<TransactionData*>(m_data);
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

bool UndoManager::isTransactionMode()
{
	return transactions_.size() > 0;
}

void UndoManager::registerGui(UndoGui* gui)
{
	if (gui == 0)
		return;

	setUndoEnabled(false);
	setState(gui);
	undoGuis_.push_back(gui);
	setUndoEnabled(true);
}

void UndoManager::setState(UndoGui* gui, int uid)
{
	gui->clear();

	if ( stacks_[currentDoc_].size() == 0 )
		return;

	StateList::iterator itstartU = stacks_[currentDoc_].undoActions_.begin(); // undo actions
	StateList::iterator itendU   = stacks_[currentDoc_].undoActions_.end();

	StateList::iterator itstartR = stacks_[currentDoc_].redoActions_.begin(); // redo actions
	StateList::iterator itendR   = stacks_[currentDoc_].redoActions_.end();

	if (uid > -1)
	{ // find the range from where actions are added when in obj. spec. mode
		StateList::iterator it2;
		for (it2 = stacks_[currentDoc_].undoActions_.begin();
		     it2 != stacks_[currentDoc_].undoActions_.end(); ++it2)
		{
			UndoState*  tmp  = *it2;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
			if (ts && !ts->containsOnly(uid))
			{
				if (it2 != stacks_[currentDoc_].undoActions_.begin())
					itendU = --it2;
				break;
			}
		}
		StateList::iterator it3;
		for (it3 = stacks_[currentDoc_].redoActions_.begin();
		     it3 != stacks_[currentDoc_].redoActions_.end(); ++it3)
		{
			UndoState*  tmp  = *it3;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
			if (ts && !ts->containsOnly(uid))
			{
				itendR = it3;
				break;
			}
		}
	}

	if (stacks_[currentDoc_].undoItems() > 0)
	{
		if (itendU == stacks_[currentDoc_].undoActions_.end())
			--itendU;
		for (; itendU >= itstartU; --itendU) // insert undo actions
		{
			UndoState*  state  = *itendU;
			UndoObject* target = state->undoObject();

			if (target && (uid == -1 || target->getUId() == static_cast<uint>(uid)))
				gui->insertUndoItem(target, state);
			if (itendU == itstartU)
				break;
		}
	}

	if (stacks_[currentDoc_].redoItems() > 0)
	{
		--itendR;
		for (; itstartR <= itendR; ++itstartR) // insert redo actions
		{
			UndoState*  state  = *itstartR;
			UndoObject* target = state->undoObject();

			if (target && (uid == -1 || target->getUId() == static_cast<uint>(uid)))
				gui->insertRedoItem(target, state);
			if (itendR == itstartR)
				break;
		}
	}
}

void UndoManager::connectGuis()
{
	for (uint i = 0; i < undoGuis_.size(); ++i)
	{
		UndoGui *gui = undoGuis_[i];

		connect(gui, SIGNAL(undo(int)), this, SLOT(undo(int)));
		connect(gui, SIGNAL(redo(int)), this, SLOT(redo(int)));
		connect(this, SIGNAL(newAction(UndoObject*, UndoState*)),
                gui, SLOT(insertUndoItem(UndoObject*, UndoState*)));
		connect(this, SIGNAL(popBack()), gui, SLOT(popBack()));
		connect(this, SIGNAL(undoSignal(int)), gui, SLOT(updateUndo(int)));
		connect(this, SIGNAL(redoSignal(int)), gui, SLOT(updateRedo(int)));
		connect(this, SIGNAL(clearRedo()), gui, SLOT(clearRedo()));
		gui->setEnabled(true);
		gui->updateUndoActions();
	}
}

void UndoManager::disconnectGuis()
{
	for (uint i = 0; i < undoGuis_.size(); ++i)
	{
		UndoGui *gui = undoGuis_[i];

		disconnect(gui, SIGNAL(undo(int)), this, SLOT(undo(int)));
		disconnect(gui, SIGNAL(redo(int)), this, SLOT(redo(int)));
		disconnect(this, SIGNAL(newAction(UndoObject*, UndoState*)),
                   gui, SLOT(insertUndoItem(UndoObject*, UndoState*)));
		disconnect(this, SIGNAL(popBack()), gui, SLOT(popBack()));
		disconnect(this, SIGNAL(undoSignal(int)), gui, SLOT(updateUndo(int)));
		disconnect(this, SIGNAL(redoSignal(int)), gui, SLOT(updateRedo(int)));
		disconnect(this, SIGNAL(clearRedo()), gui, SLOT(clearRedo()));
		gui->setEnabled(false);
	}
}

void UndoManager::removeGui(UndoGui* gui)
{
	std::vector<UndoGui*>::iterator it = undoGuis_.begin();
	while(it != undoGuis_.end())
	{
		if (*it == gui)
			it = undoGuis_.erase(it);
		else  
			++it;
	}
}


void UndoManager::switchStack(const QString& stackName)
{
	if (stackName == currentDoc_)
		return; // already current stack
	currentDoc_ = stackName;
	if (!stacks_.contains(currentDoc_))
		stacks_[currentDoc_] = UndoStack();

	stacks_[currentDoc_].setMaxSize(prefs_->getInt("historylength", 20));
	for (uint i = 0; i < undoGuis_.size(); ++i)
		setState(undoGuis_[i]);

	setTexts();
}

void UndoManager::renameStack(const QString& newName)
{
	if (currentDoc_ == newName)
		return;

	if (stacks_[currentDoc_].size() == 0) {
		currentDoc_ = newName;
		return;
	}
	
	UndoStack tmp(stacks_[currentDoc_]);
	stacks_.remove(currentDoc_);
	stacks_[newName] = tmp;
	currentDoc_ = newName;
}

void UndoManager::removeStack(const QString& stackName)
{
	if (stacks_.contains(stackName))
	{
		stacks_[stackName].clear();
		stacks_.remove(stackName);
		if (currentDoc_ == stackName)
		{
			for (uint i = 0; i < undoGuis_.size(); ++i)
				undoGuis_[i]->clear();
			currentDoc_ = "__no_name__";
		}
	}
}

void UndoManager::clearStack()
{
	stacks_[currentDoc_].clear();
	for (uint i = 0; i < undoGuis_.size(); ++i)
	{
		undoGuis_[i]->clear();
		setState(undoGuis_[i]);
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

	if (!undoEnabled_) // if so flush down the state
	{
		TransactionState *ts = dynamic_cast<TransactionState*>(state);
		if (ts) // flush the TransactionObject too
			delete target;
		delete state;
		return;
	}

	if (!isTransactionMode() &&
        (currentUndoObjectId_ == -1 || currentUndoObjectId_ == static_cast<long>(target->getUId())))
	{
//		qDebug() << "UndoManager: new Action" << state->getName() << "for" << currentUndoObjectId_;
		emit newAction(target, state); // send action to the guis
	}
	else
	{
		emit clearRedo();
	}
	if (isTransactionMode())
	{
//		qDebug() << "UndoManager: Action stored for transaction:" << transactions_.back() << target->getUName() << state->getName();
		transactions_.back()->transactionState->pushBack(target, state);
	}
	else
	{
//		qDebug() << "UndoManager: Action executed:" << target->getUName() << state->getName();
		state->setUndoObject(target);
		stacks_[currentDoc_].action(state);
	}
	if (targetPixmap)
		target->setUPixmap(oldIcon);

	setTexts();
}

void UndoManager::action(UndoObject* target, UndoState* state,
                         const QString &targetName, QPixmap *targetPixmap)
{
	QString oldName = target->getUName();
	if (!targetName.isEmpty())
		target->setUName(targetName);
	action(target, state, targetPixmap);
	target->setUName(oldName);
}

void UndoManager::undo(int steps)
{
	if (!undoEnabled_)
		return;

	setUndoEnabled(false);
	stacks_[currentDoc_].undo(steps, currentUndoObjectId_);
	setUndoEnabled(true);
	emit undoSignal(steps);
	emit undoRedoDone();
	setTexts();
}

void UndoManager::redo(int steps)
{
	if (!undoEnabled_)
		return;

	setUndoEnabled(false);
	stacks_[currentDoc_].redo(steps, currentUndoObjectId_);
	setUndoEnabled(true);
	emit redoSignal(steps);
	emit undoRedoDone();
	setTexts();
}

bool UndoManager::hasUndoActions(int )
{
	// TODO Needs to fixed for object specific mode
	return stacks_[currentDoc_].undoItems() > 0;
}

bool UndoManager::hasRedoActions(int )
{
	// TODO Needs to be fixed for object specific mode
	return stacks_[currentDoc_].redoItems() > 0;
}

void UndoManager::showObject(int uid)
{
	if (currentUndoObjectId_ == uid)
		return;
	setUndoEnabled(false);
	currentUndoObjectId_ = uid;
	for (uint i = 0; i < undoGuis_.size(); ++i)
	{
		if (uid == -2)
			undoGuis_[i]->clear();
		else
			setState(undoGuis_[i], currentUndoObjectId_);
	}
	setUndoEnabled(true);
	setTexts();
}

UndoObject* UndoManager::replaceObject(ulong uid, UndoObject *newUndoObject)
{
	UndoObject *tmp = 0;
	TransactionState* transaction_ = NULL;
	if (transactions_.size() > 0)
		transaction_ = transactions_.at(transactions_.size()-1)->transactionState;
	for (uint i = 0; i < stacks_[currentDoc_].undoActions_.size(); ++i)
	{
		UndoState *tmpState = stacks_[currentDoc_].undoActions_[i];
		TransactionState *ts = dynamic_cast<TransactionState*>(tmpState);
		if (ts)
			tmp = ts->replace(uid, newUndoObject);
		else if (tmpState->undoObject() && tmpState->undoObject()->getUId() == uid)
		{
			tmp = tmpState->undoObject();
			tmpState->setUndoObject(newUndoObject);
		}
	}
	if (transaction_) // replace also in the currently open transaction
		tmp = transaction_->replace(uid, newUndoObject);
	return tmp;
}

void UndoManager::setHistoryLength(int steps)
{
	if (steps >= 0)
	{
		stacks_[currentDoc_].setMaxSize(static_cast<uint>(steps));
		prefs_->set("historylength", stacks_[currentDoc_].maxSize());
	}
}

void UndoManager::setAllHistoryLengths(int steps)
{
	if (steps >= 0)
	{
		for (StackMap::Iterator it = stacks_.begin(); it != stacks_.end(); ++it )
		{
			it.value().setMaxSize(static_cast<uint>(steps));
		}
		prefs_->set("historylength", steps);
	}
}

int UndoManager::getHistoryLength()
{
	if (stacks_.size() > 0 && stacks_[currentDoc_].redoItems() > 0)
		return -1;
	return static_cast<int>(stacks_[currentDoc_].maxSize());
}

bool UndoManager::isGlobalMode()
{
	return currentUndoObjectId_ == -1;
}

void UndoManager::setTexts()
{
	if (stacks_[currentDoc_].undoItems() > 0)
	{
		UndoState *state = stacks_[currentDoc_].getNextUndo(currentUndoObjectId_);
		if (state)
			ScCore->primaryMainWindow()->scrActions["editUndoAction"]->setTexts(QString(Um::MenuUndo).arg(state->getName()));
		else
			ScCore->primaryMainWindow()->scrActions["editUndoAction"]->setTexts(Um::MenuUndoEmpty);
	}
	else
		ScCore->primaryMainWindow()->scrActions["editUndoAction"]->setTexts(Um::MenuUndoEmpty);

	if (stacks_[currentDoc_].redoItems() > 0)
	{
		UndoState *state = stacks_[currentDoc_].getNextRedo(currentUndoObjectId_);
		if (state)
			ScCore->primaryMainWindow()->scrActions["editRedoAction"]->setTexts(QString(Um::MenuRedo).arg(state->getName()));
		else
			ScCore->primaryMainWindow()->scrActions["editRedoAction"]->setTexts(Um::MenuRedoEmpty);
	}
	else
		ScCore->primaryMainWindow()->scrActions["editRedoAction"]->setTexts(Um::MenuRedoEmpty);
}

void UndoManager::deleteInstance()
{
	if (instance_)
		delete instance_;
	instance_ = 0;
}

UndoManager::~UndoManager()
{
	StackMap::iterator it;
	for (it = stacks_.begin(); it != stacks_.end(); ++it)
	{
		for (uint i = 0; i < (*it).size(); ++i)
		{
			(*it).clear();
		}
	}
	stacks_.clear();
}

/*** TransactionState *****************************************************/

TransactionState::TransactionState() : UndoState("")
{
	size_ = 0;
}

UndoState* TransactionState::at(int index)
{
	if (index >= 0 && static_cast<uint>(index) < sizet())
		return states_[index];
	else
		return 0;
}

bool TransactionState::contains(int uid) const
{
	for (uint i = 0; i < states_.size(); ++i)
	{
		if (states_[i]->undoObject()->getUId() == static_cast<uint>(uid))
			return true;
	}
	return false;
}

bool TransactionState::containsOnly(int uid) const
{
	for (uint i = 0; i < states_.size(); ++i)
	{
		if (states_[i]->undoObject()->getUId() != static_cast<uint>(uid))
			return false;
	}
	return true;
}

void TransactionState::pushBack(UndoObject *target, UndoState *state)
{
	if (target && state)
	{
		state->setUndoObject(target);
		states_.push_back(state);
		++size_;
	}
}

uint TransactionState::sizet()
{
	return size_;
}

void TransactionState::useActionName()
{
	if (size_ > 0)
		setName(states_[size_ - 1]->getName());
}

UndoObject* TransactionState::replace(ulong uid, UndoObject *newUndoObject)
{
	UndoObject *tmp = 0;
	for (uint i = 0; i < states_.size(); ++i)
	{
		TransactionState *ts = dynamic_cast<TransactionState*>(states_[i]);
		if (ts) // are we having a transaction_inside a transaction
			ts->replace(uid, newUndoObject);
		else if (states_[i]->undoObject() && states_[i]->undoObject()->getUId() == uid)
		{
			tmp = states_[i]->undoObject();
			states_[i]->setUndoObject(newUndoObject);
		}
	}
	return tmp;
}

void TransactionState::undo() // undo all attached states
{
	for (int i = sizet() - 1; i > -1; --i)
		at(i)->undo();
}

void TransactionState::redo() // redo all attached states
{
	for (uint i = 0; i < sizet(); ++i)
		at(i)->redo();
}

TransactionState::~TransactionState()
{
	for (uint i = 0; i < states_.size(); ++i)
	{
		if (states_[i])
		{
			delete states_[i];
			states_[i] = 0;
		}
	}
}

/*************************************************************************************/

void UndoManager::languageChange()
{
	UndoManager::AddVGuide          = tr("Add vertical guide");
	UndoManager::AddHGuide          = tr("Add horizontal guide");
	UndoManager::DelVGuide          = tr("Remove vertical guide");
	UndoManager::DelHGuide          = tr("Remove horizontal guide");
	UndoManager::DelVAGuide         = tr("Remove vertical auto guide");
	UndoManager::DelHAGuide         = tr("Remove horizontal auto guide");
	UndoManager::MoveVGuide         = tr("Move vertical guide");
	UndoManager::MoveHGuide         = tr("Move horizontal guide");
	UndoManager::LockGuides         = tr("Lock guides");
	UndoManager::UnlockGuides       = tr("Unlock guides");
	UndoManager::Move               = tr("Move");
	UndoManager::Resize             = tr("Resize");
	UndoManager::Rotate             = tr("Rotate");
	UndoManager::MoveFromTo         = tr("X1: %1, Y1: %2, %3\nX2: %4, Y2: %5, %6");
	UndoManager::ResizeFromTo       = tr("W1: %1, H1: %2\nW2: %3, H2: %4");
	UndoManager::ImageOffset        = tr("Change Image Offset");
	UndoManager::ImageScale         = tr("Change Image Scale");
	UndoManager::ImageOffsetFromTo  = tr("X1: %1, Y1: %2\nX2: %4, Y2: %5");
	UndoManager::ImageScaleFromTo   = tr("X: %1, Y: %2\nX: %4, Y: %5");
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
	UndoManager::EnablePrint        = tr("Enable Item Printing");
	UndoManager::DisablePrint       = tr("Disable Item Printing");
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
	UndoManager::StartAndEndArrow   = tr("Set start and end arrows");
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
	UndoManager::LatexFrame         = tr("Render frame");
	UndoManager::Polygon            = tr("Polygon");
	UndoManager::BezierCurve        = tr("Bezier curve");
	UndoManager::Polyline           = tr("Polyline");
	UndoManager::PathText           = tr("Text on a Path");
	UndoManager::ConvertTo          = tr("Convert to");
	UndoManager::ImportSVG          = tr("Import SVG image");
	UndoManager::ImportEPS          = tr("Import EPS image");
	UndoManager::ImportOOoDraw      = tr("Import OpenOffice.org Draw image");
	UndoManager::ImportAI           = tr("Import AI drawing");
	UndoManager::ImportXfig         = tr("Import XFig drawing");
	UndoManager::ScratchSpace       = tr("Scratch space");
	//UndoManager::TextFlow           = tr("Text flows around the frame");
	UndoManager::ObjectFrame        = tr("Text flows around the frame");
	UndoManager::BoundingBox        = tr("Text flows around bounding box");
	UndoManager::ContourLine        = tr("Text flows around contour line");
	UndoManager::ImageClip        = tr("Text flows around image clipping path");
	UndoManager::NoTextFlow         = tr("No text flow");
	UndoManager::NoObjectFrame      = tr("No object frame");
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
	UndoManager::ChangeFormula      = tr("Change formula");
	UndoManager::MultipleDuplicate  = tr("Multiple duplicate");
	UndoManager::ApplyTextStyle     = tr("Apply text style");
	UndoManager::MenuUndo           = tr("&Undo: %1", "f.e. Undo: Move");
	UndoManager::MenuUndoEmpty      = tr("&Undo");
	UndoManager::MenuRedo           = tr("&Redo: %1", "f.e. Redo: Move");
	UndoManager::MenuRedoEmpty      = tr("&Redo");
	UndoManager::EditContour        = tr("Edit contour line");
	UndoManager::ResetControlPoint  = tr("Reset control point");
	UndoManager::ResetControlPoints = tr("Reset control points");
	UndoManager::ImageEffects       = tr("Apply image effects");
	UndoManager::InsertFrame        = tr("Insert frame");
	UndoManager::AdjustFrameToImage = tr("Adjust frame to the image size");
	UndoManager::RemoveAllGuides    = tr("Remove all guides");
	UndoManager::RemoveAllPageGuides = tr("Remove page guides");
	UndoManager::Copy               = tr("Copy");
	UndoManager::CopyPage           = tr("Copy page");
	UndoManager::ToOutlines         = tr("Convert to outlines");
}

void UndoManager::initIcons()
{
	QString iconDir = ScPaths::instance().iconDir();

/*** Icons for UndoObjects *******************************************/
	UndoManager::IImageFrame      = new QPixmap(iconDir + "16/insert-image.png");
	UndoManager::ITextFrame       = new QPixmap(iconDir + "16/insert-text-frame.png");
	UndoManager::ILatexFrame      = new QPixmap(iconDir + "16/insert-latex.png");
	UndoManager::ILine            = new QPixmap(iconDir + "Stift.xpm");
	UndoManager::IPolygon         = new QPixmap(iconDir + "16/draw-polygon.png");
	UndoManager::IPolyline        = new QPixmap(iconDir + "16/draw-bezier-curves.png");
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
	UndoManager::IEnablePrint     = new QPixmap(iconDir + "u_enableprint.png");
	UndoManager::IDisablePrint    = new QPixmap(iconDir + "u_disableprint.png");
	UndoManager::IDelete          = new QPixmap(iconDir + "u_delete.png");
	UndoManager::ICreate          = new QPixmap(iconDir + "u_create.png");
	UndoManager::IPaste           = new QPixmap(iconDir + "16/edit-paste.png");
	UndoManager::ICut             = new QPixmap(iconDir + "u_cut.png");
	UndoManager::ITransparency    = new QPixmap(iconDir + "u_transp.png");
	UndoManager::ILineStyle       = new QPixmap(iconDir + "u_line.png");
	UndoManager::IArrow           = new QPixmap(iconDir + "u_arrow.png");
	UndoManager::ITable           = new QPixmap(iconDir + "frame_table.png");
	UndoManager::IFont            = new QPixmap(iconDir + "u_font.png");
	UndoManager::IImportOOoDraw   = new QPixmap(iconDir + "ooo_draw.png");
	UndoManager::ISVG             = new QPixmap(iconDir + "u_svg.png");
	UndoManager::IEPS             = new QPixmap(iconDir + "u_eps.png");
	UndoManager::IAI              = new QPixmap(iconDir + "u_eps.png"); // using the icon for EPS for now
	UndoManager::IXFIG            = new QPixmap(iconDir + "u_eps.png"); // using the icon for EPS for now
	UndoManager::IImageScaling    = new QPixmap(iconDir + "u_scale_image.png");
	UndoManager::IBorder          = new QPixmap(iconDir + "u_shape.png");
	UndoManager::IDocument        = new QPixmap(iconDir + "16/document-new.png");
// 	UndoManager::ILayer           = new QPixmap(iconDir + "u_layer.png");
// 	UndoManager::ILayerAction     = new QPixmap(iconDir + "u_layer_action.png");
// 	UndoManager::IUp              = new QPixmap(iconDir + "u_up.png");
// 	UndoManager::IDown            = new QPixmap(iconDir + "u_down.png");
// 	UndoManager::IGetImage        = new QPixmap(iconDir + "u_get_image.png");
	UndoManager::IMultipleDuplicate = new QPixmap(iconDir + "u_multiple.png");
}
QString UndoManager::AddVGuide          = "";
QString UndoManager::AddHGuide          = "";
QString UndoManager::DelVGuide          = "";
QString UndoManager::DelHGuide          = "";
QString UndoManager::DelVAGuide         = "";
QString UndoManager::DelHAGuide         = "";
QString UndoManager::MoveVGuide         = "";
QString UndoManager::MoveHGuide         = "";
QString UndoManager::LockGuides         = "";
QString UndoManager::UnlockGuides       = "";
QString UndoManager::Move               = "";
QString UndoManager::Resize             = "";
QString UndoManager::Rotate             = "";
QString UndoManager::MoveFromTo         = "";
QString UndoManager::ImageOffset        = "";
QString UndoManager::ImageScale         = "";
QString UndoManager::ImageOffsetFromTo  = "";
QString UndoManager::ImageScaleFromTo   = "";
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
QString UndoManager::EnablePrint        = "";
QString UndoManager::DisablePrint       = "";
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
QString UndoManager::StartAndEndArrow   = "";
QString UndoManager::CreateTable        = "";
QString UndoManager::RowsCols           = "";
QString UndoManager::SetFont            = "";
QString UndoManager::SetFontSize        = "";
QString UndoManager::SetFontWidth       = "";
QString UndoManager::SetFontHeight      = "";
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
QString UndoManager::LatexFrame         = "";
QString UndoManager::Polygon            = "";
QString UndoManager::BezierCurve        = "";
QString UndoManager::Polyline           = "";
QString UndoManager::PathText           = "";
QString UndoManager::ConvertTo          = "";
QString UndoManager::ImportSVG          = "";
QString UndoManager::ImportEPS          = "";
QString UndoManager::ImportOOoDraw      = "";
QString UndoManager::ImportAI           = "";
QString UndoManager::ImportXfig         = "";
QString UndoManager::ScratchSpace       = "";
//QString UndoManager::TextFlow           = "";
QString UndoManager::ObjectFrame        = "";
QString UndoManager::BoundingBox        = "";
QString UndoManager::ContourLine        = "";
QString UndoManager::ImageClip        = "";
QString UndoManager::NoTextFlow         = "";
QString UndoManager::NoObjectFrame      = "";
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
QString UndoManager::ChangeFormula      = "";
QString UndoManager::MultipleDuplicate  = "";
QString UndoManager::ApplyTextStyle     = "";
QString UndoManager::MenuUndo           = "";
QString UndoManager::MenuUndoEmpty      = "";
QString UndoManager::MenuRedo           = "";
QString UndoManager::MenuRedoEmpty      = "";
QString UndoManager::EditContour        = "";
QString UndoManager::ResetControlPoint  = "";
QString UndoManager::ResetControlPoints = "";
QString UndoManager::ImageEffects       = "";
QString UndoManager::InsertFrame        = "";
QString UndoManager::AdjustFrameToImage = "";
QString UndoManager::RemoveAllGuides    = "";
QString UndoManager::RemoveAllPageGuides = "";
QString UndoManager::Copy               = "";
QString UndoManager::CopyPage           = "";
QString UndoManager::ToOutlines         = "";

/*** Icons for UndoObjects *******************************************/
QPixmap *UndoManager::IImageFrame      = 0;
QPixmap *UndoManager::ITextFrame       = 0;
QPixmap *UndoManager::ILatexFrame      = 0;
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
QPixmap *UndoManager::IEnablePrint     = 0;
QPixmap *UndoManager::IDisablePrint    = 0;
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
QPixmap *UndoManager::IAI              = 0;
QPixmap *UndoManager::IXFIG            = 0;
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
QPixmap *UndoManager::IChangeFormula   = 0;
QPixmap *UndoManager::IMultipleDuplicate = 0;

