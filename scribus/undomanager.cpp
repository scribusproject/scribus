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
#include "undotransaction.h"

UndoManager* UndoManager::m_instance          = nullptr;
bool         UndoManager::m_undoEnabled       = true;
int          UndoManager::m_undoEnabledCounter = 0;

UndoManager* UndoManager::instance()
{
	if (m_instance == nullptr)
		m_instance = new UndoManager();

	return m_instance;
}

void UndoManager::setUndoEnabled(bool isEnabled)
{
	if (isEnabled && m_undoEnabledCounter == 0)
		return; // nothing to do undo is already enabled.
	if (isEnabled && m_undoEnabledCounter > 0)
		--m_undoEnabledCounter;
	else if (!isEnabled)
		++m_undoEnabledCounter;

	m_undoEnabled = m_undoEnabledCounter == 0;
	if (m_undoEnabled)
		connectGuis();
	else if (m_undoEnabledCounter == 1)
		disconnectGuis(); // disconnect only once when setUndoEnabled(false) has been called
	// no need to call again if next setUndoEnabled() call will also be false.
}

bool UndoManager::undoEnabled()
{
	return m_undoEnabled;
}

UndoManager::UndoManager()
{
	if (!UndoManager::IGuides)
		initIcons();
	prefs_ = PrefsManager::instance().prefsFile->getContext("undo");
	languageChange();
	setUndoEnabled(prefs_->getBool("enabled", true));
}

UndoTransaction UndoManager::beginTransaction(const TransactionSettings& settings)
{
	return UndoManager::beginTransaction(settings.targetName, settings.targetPixmap, settings.actionName, 
	                                     settings.description, settings.actionPixmap);
}

UndoTransaction UndoManager::beginTransaction(const QString &targetName,
											  QPixmap *targetPixmap,
											  const QString &name,
											  const QString &description,
											  QPixmap *actionPixmap)
{
	if (!m_undoEnabled)
		return UndoTransaction(nullptr);
	
	/** @brief Dummy object for storing transaction target's name */
	UndoObject* transactionTarget = new DummyUndoObject();
	TransactionState* transactionState = new TransactionState();
	transactionTarget->setUName(targetName); // Name which will be in action history
	if (targetPixmap)
		transactionTarget->setUPixmap(targetPixmap);
	if (name.length() > 0)          // if left to 0 length action will be fetched from the
		transactionState->setName(name); // last added UndoState in this transaction
	if (description.length() > 0)
		transactionState->setDescription(description); // tool tip for action history
	if (actionPixmap)
		transactionState->setPixmap(actionPixmap); // for action history

	// Holds the state and data of this transaction:
	auto *transaction = new TransactionData();
	transaction->transactionObject = transactionTarget;
	transaction->transactionState = transactionState;
	transaction->stackLevel = m_transactions.size();
	transaction->UM = this;
	
	m_transactions.push_back(transaction);
	
//	qDebug() << "UndoManager::beginTransaction" << targetName << name << transaction;
	return UndoTransaction(transaction);
}

bool UndoManager::isTransactionMode() const
{
	return !m_transactions.empty();
}

void UndoManager::registerGui(UndoGui* gui)
{
	if (gui == nullptr)
		return;

	setUndoEnabled(false);
	setState(gui);
	m_undoGuis.push_back(gui);
	setUndoEnabled(true);
}

void UndoManager::setState(UndoGui* gui, int uid)
{
	gui->clear();

	if ( m_stacks[m_currentDoc].size() == 0 )
		return;

	UndoStack& currentStack = m_stacks[m_currentDoc];

	StateList::iterator itstartU = currentStack.m_undoActions.begin(); // undo actions
	StateList::iterator itendU   = currentStack.m_undoActions.end();

	StateList::iterator itstartR = currentStack.m_redoActions.begin(); // redo actions
	StateList::iterator itendR   = currentStack.m_redoActions.end();

	if (uid > -1)
	{
		// find the range from where actions are added when in obj. spec. mode
		for (auto it2  = currentStack.m_undoActions.begin();
			 it2 != currentStack.m_undoActions.end(); ++it2)
		{
			UndoState*  tmp  = *it2;
			TransactionState *ts = dynamic_cast<TransactionState*>(tmp);
			if (ts && !ts->containsOnly(uid))
			{
				if (it2 != currentStack.m_undoActions.begin())
					itendU = --it2;
				break;
			}
		}
		for (auto it3  = currentStack.m_redoActions.begin();
			 it3 != currentStack.m_redoActions.end(); ++it3)
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

	if (currentStack.undoItems() > 0)
	{
		if (itendU == currentStack.m_undoActions.end())
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

	if (currentStack.redoItems() > 0)
	{
		if (itendR > itstartR)
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
	for (uint i = 0; i < m_undoGuis.size(); ++i)
	{
		UndoGui *gui = m_undoGuis[i];

		connect(gui, SIGNAL(undo(int)), this, SLOT(undo(int)));
		connect(gui, SIGNAL(redo(int)), this, SLOT(redo(int)));
		connect(this, SIGNAL(newAction(UndoObject*,UndoState*)), gui, SLOT(insertUndoItem(UndoObject*,UndoState*)));
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
	for (uint i = 0; i < m_undoGuis.size(); ++i)
	{
		UndoGui *gui = m_undoGuis[i];

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
	std::vector<UndoGui*>::iterator it = m_undoGuis.begin();
	while (it != m_undoGuis.end())
	{
		if (*it == gui)
			it = m_undoGuis.erase(it);
		else  
			++it;
	}
}


void UndoManager::switchStack(const QString& stackName)
{
	if (stackName == m_currentDoc)
		return; // already current stack
	m_currentDoc = stackName;
	if (!m_stacks.contains(m_currentDoc))
		m_stacks[m_currentDoc] = UndoStack();

	m_stacks[m_currentDoc].setMaxSize(prefs_->getInt("historylength", 100));
	for (size_t i = 0; i < m_undoGuis.size(); ++i)
		setState(m_undoGuis[i]);

	setTexts();
}

void UndoManager::renameStack(const QString& newName)
{
	if (m_currentDoc == newName)
		return;

	if (m_stacks[m_currentDoc].size() == 0)
	{
		m_currentDoc = newName;
		return;
	}
	
	UndoStack tmp(m_stacks[m_currentDoc]);
	m_stacks.remove(m_currentDoc);
	m_stacks[newName] = tmp;
	m_currentDoc = newName;
}

void UndoManager::removeStack(const QString& stackName)
{
	auto stackIt = m_stacks.find(stackName);
	if (stackIt == m_stacks.end())
		return;

	stackIt->clear();
	m_stacks.remove(stackName);
	if (m_currentDoc == stackName)
	{
		for (size_t i = 0; i < m_undoGuis.size(); ++i)
			m_undoGuis[i]->clear();
		m_currentDoc = "__no_name__";
	}
}

void UndoManager::clearStack()
{
	m_stacks[m_currentDoc].clear();
	for (size_t i = 0; i < m_undoGuis.size(); ++i)
	{
		m_undoGuis[i]->clear();
		setState(m_undoGuis[i]);
	}
}

void UndoManager::action(UndoObject* target, UndoState* state, QPixmap *targetPixmap)
{
	QPixmap *oldIcon = nullptr;
	if (targetPixmap)
	{
		oldIcon = target->getUPixmap();
		target->setUPixmap(targetPixmap);
	}

	if (!m_undoEnabled) // if so flush down the state
	{
		auto *ts = dynamic_cast<TransactionState*>(state);
		if (ts) // flush the TransactionObject too
			delete target;
		delete state;
		return;
	}

	if (!isTransactionMode() &&
		(m_currentUndoObjectId == -1 || m_currentUndoObjectId == static_cast<long>(target->getUId())))
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
		m_transactions.back()->transactionState->pushBack(target, state);
	}
	else
	{
//		qDebug() << "UndoManager: Action executed:" << target->getUName() << state->getName();
		state->setUndoObject(target);
		if (m_stacks[m_currentDoc].action(state))
			emit popBack();
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

UndoState* UndoManager::getLastUndo()
{
	UndoState* state = m_stacks[m_currentDoc].getNextUndo(Um::GLOBAL_UNDO_MODE);
	return state;
}

void UndoManager::undo(int steps)
{
	if (!m_undoEnabled)
		return;

	emit undoRedoBegin();
	setUndoEnabled(false);
	m_stacks[m_currentDoc].undo(steps, m_currentUndoObjectId);
	setUndoEnabled(true);
	emit undoSignal(steps);
	emit undoRedoDone();
	setTexts();
}

void UndoManager::redo(int steps)
{
	if (!m_undoEnabled)
		return;

	emit undoRedoBegin();
	setUndoEnabled(false);
	m_stacks[m_currentDoc].redo(steps, m_currentUndoObjectId);
	setUndoEnabled(true);
	emit redoSignal(steps);
	emit undoRedoDone();
	setTexts();
}

bool UndoManager::hasUndoActions(int ) const
{
	// TODO Needs to fixed for object specific mode
	auto currentStackIt = m_stacks.constFind(m_currentDoc);
	if (currentStackIt != m_stacks.constEnd())
		return (currentStackIt->undoItems() > 0);
	return false;
}

bool UndoManager::hasRedoActions(int ) const
{
	// TODO Needs to be fixed for object specific mode
	auto currentStackIt = m_stacks.constFind(m_currentDoc);
	if (currentStackIt != m_stacks.constEnd())
		return (currentStackIt->redoItems() > 0);
	return false;
}

void UndoManager::showObject(int uid)
{
	if (m_currentUndoObjectId == uid)
		return;
	setUndoEnabled(false);
	m_currentUndoObjectId = uid;
	for (uint i = 0; i < m_undoGuis.size(); ++i)
	{
		if (uid == -2)
			m_undoGuis[i]->clear();
		else
			setState(m_undoGuis[i], m_currentUndoObjectId);
	}
	setUndoEnabled(true);
	setTexts();
}

UndoObject* UndoManager::replaceObject(ulong uid, UndoObject *newUndoObject)
{
	UndoObject *tmp = nullptr;
	TransactionState* transaction_ = nullptr;
	if (!m_transactions.empty())
		transaction_ = m_transactions.at(m_transactions.size()-1)->transactionState;
	for (uint i = 0; i < m_stacks[m_currentDoc].m_undoActions.size(); ++i)
	{
		UndoState *tmpState = m_stacks[m_currentDoc].m_undoActions[i];
		TransactionState *ts = dynamic_cast<TransactionState*>(tmpState);
		if (ts)
			tmp = ts->replace(uid, newUndoObject);
		else if (tmpState->undoObject() && tmpState->undoObject()->getUId() == uid)
		{
			tmp = tmpState->undoObject();
			tmpState->setUndoObject(newUndoObject);
		}
	}
	for (uint i = 0; i < m_stacks[m_currentDoc].m_redoActions.size(); ++i)
	{
		UndoState *tmpState = m_stacks[m_currentDoc].m_redoActions[i];
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
		m_stacks[m_currentDoc].setMaxSize(static_cast<uint>(steps));
		prefs_->set("historylength", m_stacks[m_currentDoc].maxSize());
	}
}

void UndoManager::setAllHistoryLengths(int steps)
{
	if (steps >= 0)
	{
		for (StackMap::Iterator it = m_stacks.begin(); it != m_stacks.end(); ++it )
		{
			it.value().setMaxSize(static_cast<uint>(steps));
		}
		prefs_->set("historylength", steps);
	}
}

int UndoManager::getHistoryLength() const
{
	auto currentStackIt = m_stacks.constFind(m_currentDoc);
	if ((currentStackIt != m_stacks.constEnd()) && (currentStackIt->redoItems() <= 0))
		return static_cast<int>(currentStackIt->maxSize());
	return -1;
}

bool UndoManager::isGlobalMode() const
{
	return m_currentUndoObjectId == -1;
}

void UndoManager::setTexts()
{
	ScribusMainWindow* scMW = ScCore->primaryMainWindow();
	UndoStack& currentStack = m_stacks[m_currentDoc];

	if (currentStack.undoItems() > 0)
	{
		const UndoState *state = currentStack.getNextUndo(m_currentUndoObjectId);
		if (state)
			scMW->scrActions["editUndoAction"]->setTexts(QString(Um::MenuUndo).arg(state->getName()));
		else
			scMW->scrActions["editUndoAction"]->setTexts(Um::MenuUndoEmpty);
	}
	else
		scMW->scrActions["editUndoAction"]->setTexts(Um::MenuUndoEmpty);

	if (currentStack.redoItems() > 0)
	{
		const UndoState *state = currentStack.getNextRedo(m_currentUndoObjectId);
		if (state)
			scMW->scrActions["editRedoAction"]->setTexts(QString(Um::MenuRedo).arg(state->getName()));
		else
			scMW->scrActions["editRedoAction"]->setTexts(Um::MenuRedoEmpty);
	}
	else
		scMW->scrActions["editRedoAction"]->setTexts(Um::MenuRedoEmpty);
}

void UndoManager::deleteInstance()
{
	delete m_instance;
	m_instance = nullptr;
}

UndoManager::~UndoManager()
{
	StackMap::iterator it;
	for (it = m_stacks.begin(); it != m_stacks.end(); ++it)
	{
		for (uint i = 0; i < it->size(); ++i)
			it->clear();
	}
	m_stacks.clear();
}

/*************************************************************************************/

void UndoManager::languageChange()
{
	UndoManager::ConnectPath        = tr("Connect path");
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
	UndoManager::Overprint          = tr("Change overprint");
	UndoManager::BlendMode          = tr("Change blend mode");
	UndoManager::ActionPDF          = tr("Change action PDF");
	UndoManager::Move               = tr("Move");
	UndoManager::NewMasterPage      = tr("Add master page");
	UndoManager::DelMasterPage      = tr("Del master page");
	UndoManager::ImportMasterPage   = tr("Import master page");
	UndoManager::DuplicateMasterPage= tr("Duplicate master page");
	UndoManager::ApplyMasterPage    = tr("Apply Master Page");
	UndoManager::RenameMasterPage   = tr("Rename Master Page");
	UndoManager::UniteItem          = tr("Combine Polygons");
	UndoManager::SplitItem          = tr("Split Polygons");
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
	UndoManager::LevelUp            = tr("Level up");
	UndoManager::LevelDown          = tr("Level down");
	UndoManager::LevelTop           = tr("Send to front");
	UndoManager::LevelBottom        = tr("Send to bottom");
	UndoManager::CreateTo           = tr("X: %1, Y: %2\nW: %3, H: %4");
	UndoManager::AlignDistribute    = tr("Align/Distribute");
	UndoManager::ItemsInvolved      = tr("Items involved");
	UndoManager::ItemsInvolved2     = tr("More than 20 items involved");
	UndoManager::Cancel             = tr("Cancel");
	UndoManager::SetFill            = tr("Set fill color");
	UndoManager::ColorFromTo        = tr("Color1: %1, Color2: %2");
	UndoManager::SetShade           = tr("Set fill color shade");
	UndoManager::SetLineColor       = tr("Set line color");
	UndoManager::SetLineShade       = tr("Set line color shade");
	UndoManager::FlipH              = tr("Flip horizontally");
	UndoManager::FlipV              = tr("Flip vertically");
	UndoManager::Lock               = tr("Lock");
	UndoManager::ResTyp             = tr("Change image resolution");
	UndoManager::UnLock             = tr("Unlock");
	UndoManager::SizeLock           = tr("Lock size");
	UndoManager::GradTypeMask       = tr("Set mask gradient type");
	UndoManager::SizeUnLock         = tr("Unlock size");
	UndoManager::EnablePrint        = tr("Enable Item Printing");
	UndoManager::DisablePrint       = tr("Disable Item Printing");
	UndoManager::Ungroup            = tr("Ungroup");
	UndoManager::Delete             = tr("Delete");
	UndoManager::Rename             = tr("Rename");
	UndoManager::FromTo             = tr("From %1\nto %2");
	UndoManager::Paste              = tr("Paste");
	UndoManager::Cut                = tr("Cut");
	UndoManager::RoundCorner        = tr("Change round corner");
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
	UndoManager::CellBorders        = tr("Set cell borders");
	UndoManager::CellFillColor      = tr("Set cell fill color");
	UndoManager::CellFillShade      = tr("Set cell fill shade");
	UndoManager::CellStyle          = tr("Set cell style");
	UndoManager::TableFillColor     = tr("Set table fill color");
	UndoManager::TableFillColorRst  = tr("Reset table fill color");
	UndoManager::TableFillShade     = tr("Set table fill shade");
	UndoManager::TableFillShadeRst  = tr("Reset table fill shade");
	UndoManager::TableBorders       = tr("Set table borders");
	UndoManager::TableLeftBorder    = tr("Set table left border");
	UndoManager::TableLeftBorderRst = tr("Reset table left border");
	UndoManager::TableRightBorder   = tr("Set table right border");
	UndoManager::TableRightBorderRst = tr("Reset table right border");
	UndoManager::TableBottomBorder  = tr("Set table bottom border");
	UndoManager::TableBottomBorderRst  = tr("Reset table bottom border");
	UndoManager::TableTopBorder     = tr("Set table top border");
	UndoManager::TableTopBorderRst  = tr("Reset table top border");
	UndoManager::TableStyle         = tr("Set table style");
	UndoManager::TableRowHeight     = tr("Set height of table row");
	UndoManager::TableColumnWidth   = tr("Set width of table column");
	UndoManager::SetFont            = tr("Set font");
	UndoManager::SetFontSize        = tr("Set font size");
	UndoManager::StartArrowScale    = tr("Set start arrow scale");
	UndoManager::EndArrowScale      = tr("Set end arrow scale");
	UndoManager::SetFontSize        = tr("Set font size");
	UndoManager::SetFontWidth       = tr("Set font width");
	UndoManager::SetFontHeight      = tr("Set font height");
	UndoManager::GradType           = tr("Change fill gradient type");
	UndoManager::GradVal            = tr("Change fill gradient values");
	UndoManager::GradValStroke      = tr("Change stroke gradient values");
	UndoManager::GradCol            = tr("Change gradient color");
	UndoManager::GradTypeStroke     = tr("Change stroke gradient type");
	UndoManager::GradExtent         = tr("Change gradient extent");
	UndoManager::GradExtentStroke   = tr("Change stroke gradient extent");
	UndoManager::PatternVal         = tr("Change fill pattern");
	UndoManager::PatternValStroke   = tr("Change stroke pattern");
	UndoManager::PatternValMask     = tr("Change mask pattern");
	UndoManager::PatternStrokeToPath = tr("Change pattern follows path");
	UndoManager::PatternFlip        = tr("Change pattern flip");
	UndoManager::PatternTransform   = tr("Change pattern transform");
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
	UndoManager::Layer              = tr("Layer");
	UndoManager::LatexFrame         = tr("Render frame");
	UndoManager::Polygon            = tr("Polygon");
	UndoManager::EditPolygon        = tr("Edit polygon");
	UndoManager::EditArc            = tr("Edit arc");
	UndoManager::EditSpiral         = tr("Edit spiral");
	UndoManager::BezierCurve        = tr("Bezier curve");
	UndoManager::Polyline           = tr("Polyline");
	UndoManager::PathText           = tr("Text on a Path");
	UndoManager::ConvertTo          = tr("Convert to");
	UndoManager::ImportAI           = tr("Import AI drawing");
	UndoManager::ImportApplePages   = tr("Import Apple Pages document");
	UndoManager::ImportBarcode      = tr("Import Barcode");
	UndoManager::ImportCDR          = tr("Import CorelDraw drawing");
	UndoManager::ImportCGM          = tr("Import CGM drawing");
	UndoManager::ImportCVG          = tr("Import Calamus CVG drawing");
	UndoManager::ImportDRW          = tr("Import Micrografx Draw drawing");
	UndoManager::ImportEMF          = tr("Import EMF drawing");
	UndoManager::ImportEPS          = tr("Import EPS image");
	UndoManager::ImportFreehand     = tr("Import FreeHand document");
	UndoManager::ImportIDML         = tr("Import IDML document");
	UndoManager::ImportOOoDraw      = tr("Import OpenOffice.org Draw image");
	UndoManager::ImportPageMaker    = tr("Import PageMaker document");
	UndoManager::ImportPDF          = tr("Import PDF document");
	UndoManager::ImportPict         = tr("Import Macintosh Pict drawing");
	UndoManager::ImportPublisher    = tr("Import MS Publisher document");
	UndoManager::ImportQXP          = tr("Import QuarkXPress document");
	UndoManager::ImportShape        = tr("Import Shape drawing");
	UndoManager::ImportSML          = tr("Import Kivio stencil");
	UndoManager::ImportSVG          = tr("Import SVG image");
	UndoManager::ImportSVM          = tr("Import SVM image");
	UndoManager::ImportUniconv      = tr("Import Uniconvertor image");
	UndoManager::ImportViva         = tr("Import Viva Designer document");
	UndoManager::ImportVSD          = tr("Import Visio document");
	UndoManager::ImportWMF          = tr("Import WMF drawing");
	UndoManager::ImportWPG          = tr("Import WordPerfect graphic");
	UndoManager::ImportXara         = tr("Import Xara image");
	UndoManager::ImportXfig         = tr("Import XFig drawing");
	UndoManager::ImportXPS          = tr("Import XPS document");
	UndoManager::ImportZMF          = tr("Import Zoner Draw image");
	UndoManager::Columns            = tr("Change columns");
	UndoManager::ColumnsGap         = tr("Change columns gap");
	UndoManager::TextFrameDist      = tr("Change text to frame distance");
	UndoManager::ScratchSpace       = tr("Scratch space");
	UndoManager::ObjectFrame        = tr("Text flows around the frame");
	UndoManager::BoundingBox        = tr("Text flows around bounding box");
	UndoManager::ContourLine        = tr("Text flows around contour line");
	UndoManager::ImageClip          = tr("Text flows around image clipping path");
	UndoManager::NoTextFlow         = tr("No text flow");
	UndoManager::NoObjectFrame      = tr("No object frame");
	UndoManager::NoBoundingBox      = tr("No bounding box");
	UndoManager::NoContourLine      = tr("No contour line");
	UndoManager::ShowImage          = tr("Show image");
	UndoManager::PageNmbr           = tr("Page %1");
	UndoManager::ImageScaling       = tr("Set image scaling");
	UndoManager::FrameSize          = tr("Frame size");
	UndoManager::MeshGradient       = tr("Create mesh gradient");
	UndoManager::ChangeMeshGradient = tr("Change mesh gradient");
	UndoManager::Mode               = tr("Change Mode");
	UndoManager::FreeScaling        = tr("Free scaling");
	UndoManager::KeepRatio          = tr("Keep aspect ratio");
	UndoManager::BreakRatio         = tr("Break aspect ratio");
	UndoManager::EditContourLine    = tr("Edit contour line");
	UndoManager::EditShape          = tr("Edit shape");
	UndoManager::ChangeShapeType    = tr("Change shape type");
	UndoManager::ResetContourLine   = tr("Reset contour line");
	UndoManager::AddPage            = tr("Add page");
	UndoManager::AddPages           = tr("Add pages");
	UndoManager::ReplaceText        = tr("Replace text");
	UndoManager::FirstLineOffset    = tr("Change First Line Offset");
	UndoManager::DeleteText         = tr("Delete text");
	UndoManager::InsertText         = tr("Insert text");
	UndoManager::AppendText         = tr("Append text");
	UndoManager::ImportText         = tr("Import text");
	UndoManager::ClearText          = tr("Clear text");
	UndoManager::TruncateText       = tr("Truncate text");
	UndoManager::AddLoremIpsum      = tr("Add Lorem Ipsum");
	UndoManager::InsertMark         = tr("Insert mark");
	UndoManager::InsertNote         = tr("Insert note");
	UndoManager::EditMark           = tr("Edit mark");
	UndoManager::DeleteMark         = tr("Delete mark");
	UndoManager::DeleteNote         = tr("Delete note");
	UndoManager::NewNotesStyle      = tr("Add note style");
	UndoManager::EditNotesStyle     = tr("Edit note style");
	UndoManager::DeleteNotesStyle   = tr("Delete note style");
	UndoManager::DeleteNotesStyle   = tr("Delete note style");
	UndoManager::DeletePage         = tr("Delete page");
	UndoManager::DeletePages        = tr("Delete pages");
	UndoManager::ChangePageProps    = tr("Change page properties");
	UndoManager::AddLayer           = tr("Add layer");
	UndoManager::DuplicateLayer     = tr("Duplicate layer %1");
	UndoManager::DeleteLayer        = tr("Delete layer");
	UndoManager::RenameLayer        = tr("Rename layer");
	UndoManager::RaiseLayer         = tr("Raise layer");
	UndoManager::GradPos            = tr("Change gradient position");
	UndoManager::LowerLayer         = tr("Lower layer");
	UndoManager::SendToLayer        = tr("Send to layer");
	UndoManager::PrintLayer         = tr("Enable printing of layer");
	UndoManager::DoNotPrintLayer    = tr("Disable printing of layer");
	UndoManager::SetLayerName       = tr("Change name of the layer");
	UndoManager::FlowLayer          = tr("Enable text flow around for lower layers");
	UndoManager::DisableFlowLayer   = tr("Disable text flow around for lower layers");
	UndoManager::SetLayerBlendMode  = tr("Set layer blend mode");
	UndoManager::SetLayerTransparency = tr("Set layer opacity");
	UndoManager::SetLayerLocked     = tr("Lock layer");
	UndoManager::SetLayerUnlocked   = tr("Unlock layer");
	UndoManager::GetImage           = tr("Get image");
	UndoManager::ChangeFormula      = tr("Change formula");
	UndoManager::Duplicate          = tr("Duplicate");
	UndoManager::MultipleDuplicate  = tr("Multiple duplicate");
	UndoManager::RemoveMeshPatch    = tr("Remove mesh patch");
	UndoManager::ApplyTextStyle     = tr("Apply text style");
	UndoManager::RemoveTextStyle    = tr("Remove text style");
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
	UndoManager::RemoveAllPageGuides= tr("Remove page guides");
	UndoManager::Copy               = tr("Copy");
	UndoManager::CopyPage           = tr("Copy page");
	UndoManager::MovePage           = tr("Move page");
	UndoManager::SwapPage           = tr("Swap page");
	UndoManager::ImportPage         = tr("Import page");
	UndoManager::ToOutlines         = tr("Convert to outlines");
	UndoManager::LinkTextFrame      = tr("Link text frame");
	UndoManager::UnlinkTextFrame    = tr("Unlink text frame");
	UndoManager::ClearImage         = tr("Clear image frame content");
	UndoManager::PathOperation      = tr("Path Operation");
	UndoManager::ChangePageAttrs    = tr("Change Page Attributes");
	UndoManager::Transform          = tr("Transform");
	UndoManager::WeldItems          = tr("Weld Items");
	UndoManager::UnweldItems        = tr("Unweld Items");
	UndoManager::SoftShadow         = tr("Drop Shadow");
	UndoManager::SoftShadowColor    = tr("Drop Shadow Color");
	UndoManager::SoftShadowShade    = tr("Drop Shadow Shade");
	UndoManager::SoftShadowBlurRadius= tr("Drop Shadow Blur Radius");
	UndoManager::SoftShadowXOffset  = tr("Drop Shadow X Offset");
	UndoManager::SoftShadowYOffset  = tr("Drop Shadow Y Offset");
	UndoManager::SoftShadowOpacity  = tr("Drop Shadow Opacity");
	UndoManager::SoftShadowBlendMode= tr("Drop Shadow Blend Mode");
}

void UndoManager::initIcons()
{
	QString iconDir = ScPaths::instance().iconDir();

/*** Icons for UndoObjects *******************************************/
	UndoManager::IImageFrame      = new QPixmap(iconDir + "16/insert-image.png");
	UndoManager::ITextFrame       = new QPixmap(iconDir + "16/insert-text-frame.png");
	UndoManager::ILatexFrame      = new QPixmap(iconDir + "16/insert-latex.png");
	UndoManager::ILine            = new QPixmap(iconDir + "stift.png");
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
	UndoManager::IAI              = new QPixmap(iconDir + "u_eps.png"); // using the icon for EPS for now
	UndoManager::IEPS             = new QPixmap(iconDir + "u_eps.png");
	UndoManager::IImportOOoDraw   = new QPixmap(iconDir + "ooo_draw.png");
	UndoManager::ISVG             = new QPixmap(iconDir + "u_svg.png");
	UndoManager::IUniconv         = new QPixmap(iconDir + "u_svg.png"); // using the icon for SVG for now
	UndoManager::IWMF             = new QPixmap(iconDir + "u_eps.png"); // using the icon for EPS for now
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
QString UndoManager::ConnectPath        = "";
QString UndoManager::AddVGuide          = "";
QString UndoManager::AddHGuide          = "";
QString UndoManager::DelVGuide          = "";
QString UndoManager::DelHGuide          = "";
QString UndoManager::DelVAGuide         = "";
QString UndoManager::DelHAGuide         = "";
QString UndoManager::Mode               = "";
QString UndoManager::MoveVGuide         = "";
QString UndoManager::MoveHGuide         = "";
QString UndoManager::UniteItem          = "";
QString UndoManager::SplitItem          = "";
QString UndoManager::LockGuides         = "";
QString UndoManager::UnlockGuides       = "";
QString UndoManager::Overprint          = "";
QString UndoManager::BlendMode          = "";
QString UndoManager::ActionPDF          = "";
QString UndoManager::Move               = "";
QString UndoManager::NewMasterPage      = "";
QString UndoManager::GradType           = "";
QString UndoManager::GradPos            = "";
QString UndoManager::GradVal            = "";
QString UndoManager::GradValStroke      = "";
QString UndoManager::GradCol            = "";
QString UndoManager::GradTypeStroke     = "";
QString UndoManager::GradExtent         = "";
QString UndoManager::GradExtentStroke   = "";
QString UndoManager::PatternVal         = "";
QString UndoManager::PatternValStroke   = "";
QString UndoManager::PatternValMask     = "";
QString UndoManager::PatternStrokeToPath = "";
QString UndoManager::PatternFlip        = "";
QString UndoManager::PatternTransform   = "";
QString UndoManager::ImportMasterPage   = "";
QString UndoManager::DuplicateMasterPage= "";
QString UndoManager::DelMasterPage      = "";
QString UndoManager::ApplyMasterPage    = "";
QString UndoManager::RenameMasterPage   = "";
QString UndoManager::Resize             = "";
QString UndoManager::Rotate             = "";
QString UndoManager::MoveFromTo         = "";
QString UndoManager::ImageOffset        = "";
QString UndoManager::ImageScale         = "";
QString UndoManager::ResTyp             = "";
QString UndoManager::ShowImage          = "";
QString UndoManager::RemoveMeshPatch    = "";
QString UndoManager::StartArrowScale    = "";
QString UndoManager::EndArrowScale      = "";
QString UndoManager::ImageOffsetFromTo  = "";
QString UndoManager::ImageScaleFromTo   = "";
QString UndoManager::ResizeFromTo       = "";
QString UndoManager::Selection          = "";
QString UndoManager::Group              = "";
QString UndoManager::SelectionGroup     = "";
QString UndoManager::Create             = "";
QString UndoManager::RoundCorner        = "";
QString UndoManager::CreateTo           = "";
QString UndoManager::AlignDistribute    = "";
QString UndoManager::ItemsInvolved      = "";
QString UndoManager::ItemsInvolved2     = "";
int     UndoManager::ItemsInvolvedLimit = 20;
QString UndoManager::Cancel             = "";
QString UndoManager::TextFrameDist      = "";
QString UndoManager::SetFill            = "";
QString UndoManager::ColorFromTo        = "";
QString UndoManager::SetShade           = "";
QString UndoManager::SetLineColor       = "";
QString UndoManager::SetLineShade       = "";
QString UndoManager::FlipH              = "";
QString UndoManager::FlipV              = "";
QString UndoManager::Lock               = "";
QString UndoManager::LevelUp            = "";
QString UndoManager::LevelTop           = "";
QString UndoManager::LevelBottom        = "";
QString UndoManager::LevelDown          = "";
QString UndoManager::UnLock             = "";
QString UndoManager::SizeLock           = "";
QString UndoManager::SizeUnLock         = "";
QString UndoManager::EnablePrint        = "";
QString UndoManager::DisablePrint       = "";
QString UndoManager::Ungroup            = "";
QString UndoManager::Delete             = "";
QString UndoManager::Rename             = "";
QString UndoManager::FromTo             = "";
QString UndoManager::Paste              = "";
QString UndoManager::Cut                = "";
QString UndoManager::Transparency       = "";
QString UndoManager::LineTransparency   = "";
QString UndoManager::LineStyle          = "";
QString UndoManager::FirstLineOffset    = "";
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
QString UndoManager::CellBorders        = "";
QString UndoManager::CellFillColor      = "";
QString UndoManager::CellFillShade      = "";
QString UndoManager::CellStyle          = "";
QString UndoManager::TableFillColor     = "";
QString UndoManager::TableFillColorRst  = "";
QString UndoManager::TableFillShade     = "";
QString UndoManager::TableFillShadeRst  = "";
QString UndoManager::TableBorders       = "";
QString UndoManager::TableLeftBorder    = "";
QString UndoManager::TableLeftBorderRst = "";
QString UndoManager::TableRightBorder   = "";
QString UndoManager::TableRightBorderRst = "";
QString UndoManager::TableBottomBorder  = "";
QString UndoManager::TableBottomBorderRst  = "";
QString UndoManager::TableTopBorder     = "";
QString UndoManager::TableTopBorderRst  = "";
QString UndoManager::TableStyle         = "";
QString UndoManager::TableRowHeight     = "";
QString UndoManager::TableColumnWidth   = "";
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
QString UndoManager::Layer              = "";
QString UndoManager::LatexFrame         = "";
QString UndoManager::Polygon            = "";
QString UndoManager::EditPolygon        = "";
QString UndoManager::EditArc            = "";
QString UndoManager::EditSpiral         = "";
QString UndoManager::BezierCurve        = "";
QString UndoManager::Polyline           = "";
QString UndoManager::PathText           = "";
QString UndoManager::ConvertTo          = "";
QString UndoManager::ImportAI           = "";
QString UndoManager::ImportApplePages   = "";
QString UndoManager::ImportBarcode      = "";
QString UndoManager::ImportCDR          = "";
QString UndoManager::ImportCGM          = "";
QString UndoManager::ImportCVG          = "";
QString UndoManager::ImportDRW          = "";
QString UndoManager::ImportEMF          = "";
QString UndoManager::ImportEPS          = "";
QString UndoManager::ImportFreehand     = "";
QString UndoManager::ImportIDML         = "";
QString UndoManager::ImportOOoDraw      = "";
QString UndoManager::ImportPageMaker    = "";
QString UndoManager::ImportPDF          = "";
QString UndoManager::ImportPict         = "";
QString UndoManager::ImportPublisher    = "";
QString UndoManager::ImportQXP          = "";
QString UndoManager::ImportShape        = "";
QString UndoManager::ImportSML          = "";
QString UndoManager::ImportSVG          = "";
QString UndoManager::ImportSVM          = "";
QString UndoManager::ImportUniconv      = "";
QString UndoManager::ImportViva         = "";
QString UndoManager::ImportVSD          = "";
QString UndoManager::ImportWMF          = "";
QString UndoManager::ImportWPG          = "";
QString UndoManager::ImportXara         = "";
QString UndoManager::ImportXfig         = "";
QString UndoManager::ImportXPS          = "";
QString UndoManager::ImportZMF          = "";
QString UndoManager::ScratchSpace       = "";
QString UndoManager::ObjectFrame        = "";
QString UndoManager::BoundingBox        = "";
QString UndoManager::MeshGradient       = "";
QString UndoManager::ChangeMeshGradient = "";
QString UndoManager::ContourLine        = "";
QString UndoManager::ImageClip          = "";
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
QString UndoManager::ChangeShapeType    = "";
QString UndoManager::ResetContourLine   = "";
QString UndoManager::GradTypeMask       = "";
QString UndoManager::AddPage            = "";
QString UndoManager::AddPages           = "";
QString UndoManager::DeleteText         = "";
QString UndoManager::AppendText         = "";
QString UndoManager::ImportText         = "";
QString UndoManager::ClearText          = "";
QString UndoManager::TruncateText       = "";
QString UndoManager::ReplaceText        = "";
QString UndoManager::InsertText         = "";
QString UndoManager::AddLoremIpsum      = "";
QString UndoManager::EditMark           = "";
QString UndoManager::InsertMark         = "";
QString UndoManager::DeleteMark         = "";
QString UndoManager::InsertNote         = "";
QString UndoManager::DeleteNote         = "";
QString UndoManager::NewNotesStyle      = "";
QString UndoManager::EditNotesStyle     = "";
QString UndoManager::DeleteNotesStyle   = "";
QString UndoManager::DeletePage         = "";
QString UndoManager::DeletePages        = "";
QString UndoManager::ChangePageProps    = "";
QString UndoManager::AddLayer           = "";
QString UndoManager::DuplicateLayer     = "";
QString UndoManager::DeleteLayer        = "";
QString UndoManager::RenameLayer        = "";
QString UndoManager::RaiseLayer         = "";
QString UndoManager::LowerLayer         = "";
QString UndoManager::SendToLayer        = "";
QString UndoManager::PrintLayer         = "";
QString UndoManager::DoNotPrintLayer    = "";
QString UndoManager::SetLayerName       = "";
QString UndoManager::FlowLayer          = "";
QString UndoManager::DisableFlowLayer   = "";
QString UndoManager::SetLayerBlendMode  = "";
QString UndoManager::SetLayerTransparency="";
QString UndoManager::SetLayerLocked     = "";
QString UndoManager::SetLayerUnlocked   = "";
QString UndoManager::GetImage           = "";
QString UndoManager::ChangeFormula      = "";
QString UndoManager::Duplicate          = "";
QString UndoManager::MultipleDuplicate  = "";
QString UndoManager::ApplyTextStyle     = "";
QString UndoManager::RemoveTextStyle    = "";
QString UndoManager::MenuUndo           = "";
QString UndoManager::MenuUndoEmpty      = "";
QString UndoManager::MenuRedo           = "";
QString UndoManager::MenuRedoEmpty      = "";
QString UndoManager::EditContour        = "";
QString UndoManager::Columns            = "";
QString UndoManager::ColumnsGap         = "";
QString UndoManager::ResetControlPoint  = "";
QString UndoManager::ResetControlPoints = "";
QString UndoManager::ImageEffects       = "";
QString UndoManager::InsertFrame        = "";
QString UndoManager::AdjustFrameToImage = "";
QString UndoManager::RemoveAllGuides    = "";
QString UndoManager::RemoveAllPageGuides= "";
QString UndoManager::Copy               = "";
QString UndoManager::CopyPage           = "";
QString UndoManager::ImportPage         = "";
QString UndoManager::MovePage           = "";
QString UndoManager::SwapPage           = "";
QString UndoManager::ToOutlines         = "";
QString UndoManager::LinkTextFrame      = "";
QString UndoManager::UnlinkTextFrame    = "";
QString UndoManager::ClearImage         = "";
QString UndoManager::PathOperation      = "";
QString UndoManager::ChangePageAttrs    = "";
QString UndoManager::Transform          = "";
QString UndoManager::WeldItems          = "";
QString UndoManager::UnweldItems        = "";
QString UndoManager::SoftShadow         = "";
QString UndoManager::SoftShadowColor    = "";
QString UndoManager::SoftShadowShade    = "";
QString UndoManager::SoftShadowBlurRadius="";
QString UndoManager::SoftShadowXOffset  = "";
QString UndoManager::SoftShadowYOffset  = "";
QString UndoManager::SoftShadowOpacity  = "";
QString UndoManager::SoftShadowBlendMode= "";
QString UndoManager::SoftShadowErase    = "";
QString UndoManager::SoftShadowObjectTrans = "";

/*** Icons for UndoObjects *******************************************/
QPixmap *UndoManager::IImageFrame      = nullptr;
QPixmap *UndoManager::ITextFrame       = nullptr;
QPixmap *UndoManager::ILatexFrame      = nullptr;
QPixmap *UndoManager::ILine            = nullptr;
QPixmap *UndoManager::IPolygon         = nullptr;
QPixmap *UndoManager::IPolyline        = nullptr;
QPixmap *UndoManager::IPathText        = nullptr;
QPixmap *UndoManager::IGroup           = nullptr;
QPixmap *UndoManager::ITable           = nullptr;
/*** Icons for actions ***********************************************/
QPixmap *UndoManager::IMove            = nullptr;
QPixmap *UndoManager::IResize          = nullptr;
QPixmap *UndoManager::IRotate          = nullptr;
QPixmap *UndoManager::IGuides          = nullptr;
QPixmap *UndoManager::ILockGuides      = nullptr;
QPixmap *UndoManager::IAlignDistribute = nullptr;
QPixmap *UndoManager::IFill            = nullptr;
QPixmap *UndoManager::IShade           = nullptr;
QPixmap *UndoManager::IFlipH           = nullptr;
QPixmap *UndoManager::IFlipV           = nullptr;
QPixmap *UndoManager::ILock            = nullptr;
QPixmap *UndoManager::IUnLock          = nullptr;
QPixmap *UndoManager::IEnablePrint     = nullptr;
QPixmap *UndoManager::IDisablePrint    = nullptr;
QPixmap *UndoManager::IDelete          = nullptr;
QPixmap *UndoManager::ICreate          = nullptr;
QPixmap *UndoManager::IPaste           = nullptr;
QPixmap *UndoManager::ICut             = nullptr;
QPixmap *UndoManager::ITransparency    = nullptr;
QPixmap *UndoManager::ILineStyle       = nullptr;
QPixmap *UndoManager::IArrow           = nullptr;
QPixmap *UndoManager::IFont            = nullptr;
QPixmap *UndoManager::ISVG             = nullptr;
QPixmap *UndoManager::IUniconv         = nullptr;
QPixmap *UndoManager::IAI              = nullptr;
QPixmap *UndoManager::IEPS             = nullptr;
QPixmap *UndoManager::IImportOOoDraw   = nullptr;
QPixmap *UndoManager::IWMF             = nullptr;
QPixmap *UndoManager::IXFIG            = nullptr;
QPixmap *UndoManager::IImageScaling    = nullptr;
QPixmap *UndoManager::IBorder          = nullptr;
QPixmap *UndoManager::IDocument        = nullptr;
QPixmap *UndoManager::ILayer           = nullptr;
QPixmap *UndoManager::ILayerAction     = nullptr;
QPixmap *UndoManager::IUp              = nullptr;
QPixmap *UndoManager::IDown            = nullptr;
QPixmap *UndoManager::IPrint           = nullptr;
QPixmap *UndoManager::IGetImage        = nullptr;
QPixmap *UndoManager::IChangeFormula   = nullptr;
QPixmap *UndoManager::IMultipleDuplicate = nullptr;



