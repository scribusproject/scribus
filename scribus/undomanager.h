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

#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <vector>
#include <utility>
#include <qobject.h>
#include <qpixmap.h>
#include <qstring.h>
#include "undostate.h"
#include "undoobject.h"

class UndoGui;
class PrefsContext;

typedef std::pair<UndoObject*, UndoState*> ActionPair;
typedef std::vector<ActionPair> ActionList;
typedef std::pair<ActionList::iterator, ActionList> StackPair;
typedef QMap<QString, StackPair> StackMap;

/**
 * @brief UndoManager handles the undo stack.
 *
 * UndoManager is the engine of the undo/redo handling. When ever an undoable
 * action happens an UndoState object will be sent to the UndoManager which then
 * stores the state in the undo stack. When an undo is requested the state is 
 * send back to it's creator which is then responsible for doing the actual undo or
 * redo.
 *
 * UndoManager also handles the UndoGuis by linking them together and reporting
 * all undo/redo actions to the guis for them to update the visual representation.
 * For this to work all UndoGuis need to be registered to the UndoManager with the
 * registerGui() method.
 *
 * @author Riku Leino tsoots@gmail.com
 * @date December 2004
 */
class UndoManager : public QObject
{
	Q_OBJECT

private:
	/** 
	 * @brief The only instance of UndoManager available.
	 *
	 * UndoManager is singleton and the instance can be queried with the method
	 * instance().
	 */
	static UndoManager* _instance;

	/** @brief Should undo states be stored or ignored */
	static bool _undoEnabled;

	PrefsContext *prefs;

	/** @brief Doc to which the currently active stack belongs */
	QString currentDoc;

	/** 
	 * @brief Id number of the object for what the object specific undo is shown
	 * @brief or -1 if global undo is used.
	 */
	int currentUndoObjectId;

	/** 
	 * @brief UndoGuis attached to this UndoManager
	 * @sa UndoGui
	 * @sa UndoWidget
	 * @sa UndoPalette
	 */
	std::vector<UndoGui*> undoGuis;

	/** @brief Undo stacks for all open documents */
	StackMap stacks;

	/** @brief Maximum length of the undo stack */
	int historyLength;

	/** 
	 * @brief Initializes the UndoGui.
	 * @param gui UndoGui to be initialized
	 * @param uid UndoObject's id if in object specific mode or -1 to tell 
	 * that global undo is used.
	 */
	void setState(UndoGui* gui, int uid = -1);

	void connectGuis();
	void disconnectGuis();
	void initIcons();
	void checkStackLength();

public:
	/**
	 * @brief Returns a pointer to the UndoManager instance
	 * @return A pointer to the UndoManager instance
	 */
	static UndoManager* instance();

	/**
	 * @brief Deletes the UndoManager Instance
	 *
	 * Must be called when UndoManager is no more needed.
	 */
	static void deleteInstance();

	/**
	 * @brief Sets the undo action tracking enabled or disabled.
	 * @param isEnabled If true undo stack is updated with the states sent
	 * to the UndoManager else undo stack is not updated and attached UndoGuis
	 * are not informed of the actions.
	 */
	void setUndoEnabled(bool isEnabled);

	/**
	 * @brief Returns true if undo actions are stored, if not will return false.
	 * @return true if undo actions are stored, if not will return false
	 */
	static bool undoEnabled();

	/**
	 * @brief Register an UndoGui to the UndoManager.
	 *
	 * After registering a gui to the manager the gui will be updated with the
	 * undo action information received by the UndoManager. Actions done with the
	 * UndoGui to be registered are also handled after registering.
	 * @param gui A pointer to the UndoGui that is wanted to be registered.
	 */
	void registerGui(UndoGui* gui);

	/**
	 * @brief Removes an UndoGui from UndoManager.
	 * @param gui UndoGui to be removed from the UndoManager.
	 */
	void removeGui(UndoGui* gui);

	/**
	 * @brief Changes the active undo stack.
	 *
	 * Sets the stack connected to the name <code>stackName</code> active. Calling
	 * this method will send clear() signal to the attached <code>UndoGui</code>s and
	 * will update their undo stack representations.
	 * @param stackName Name of the stack to be used
	 */
	void switchStack(const QString& stackName);

	/**
	 * @brief Rename the current stack
	 * @param newName New name for the current stack.
	 */
	void rename(const QString& newName);

	/**
	 * @brief Remove the stack with the name <code>stackName</code>
	 * @param stackName Name of the stack that is wanted to be removed
	 */
	void remove(const QString& stackName);

	/**
	 * @brief Returns true if there are actions that can be undone otherwise returns false.
	 *
	 * This is useful when undo/redo actions are handled with a gui that is not attached to
	 * the UndoManager (f.e. menu items) and when those gui items are wanted to set enabled
	 * or disabled depending on the status of the undo stack.
	 * @return true if there are actions that can be undone otherwise returns false
	 */
	bool hasUndoActions();

	/**
	 * @brief Returns true if there are actions that can be redone otherwise returns false.
	 * @return true if there are actions that can be redone otherwise returns false
	 * @sa UndoManager::hasUndoActions()
	 */
	bool hasRedoActions();

	/**
	 * @brief Switches the state of UndoManager (global/object specific undo).
	 *
	 * If parameter uid is less than 0 global undo is used else the undo state
	 * is object specific using the object whose uid is given as a parameter.
	 * @param uid UndoObject's id for what the object specific undo is wanted or
	 * -1 if global undo is wanted.
	 */
	void showObject(int uid);

	/**
	 * @brief Returns the maximum length of the undostack.
	 * @return the maximum length of the undostack
	 */
	int getHistoryLength();

	/**
	* @name Action strings
	* Strings describing undo actions
	*/
	/*@{*/
	static const QString AddVGuide;
	static const QString AddHGuide;
	static const QString DelVGuide;
	static const QString DelHGuide;
	static const QString MoveVGuide;
	static const QString MoveHGuide;
	static const QString LockGuides;
	static const QString UnlockGuides;
	static const QString Move;
	static const QString Resize;
	static const QString FromXToY;
	static const QString FromHToW;
	static const QString From;
	static const QString To;
	static const QString X;
	static const QString Y;
	static const QString Text;
	static const QString Image;
	static const QString Line;
	static const QString Polygon;
	static const QString Polyline;
	static const QString PathText;
	/*@}*/

	/**
	 * @name Action icons
	 * Icons for undo actions
	 */
	/*@{*/
	static QPixmap *IGuides;
	static QPixmap *ILockGuides;
	static QPixmap *IMoveText;
	static QPixmap *IMoveImage;
	static QPixmap *IMoveLine;
	static QPixmap *IMovePolygon;
	static QPixmap *IMovePolyline;
	static QPixmap *IMovePathText;
	static QPixmap *IResizeText;
	static QPixmap *IResizeImage;
	static QPixmap *IResizeLine;
	static QPixmap *IResizePolygon;
	static QPixmap *IResizePolyline;
	static QPixmap *IResizePathText;
	/*@}*/

protected:
	/** @brief Creates a new UndoManager instance */
	UndoManager();

	/** @brief Destroys the UndoManager instance */
	~UndoManager();

private slots:
	/**
	 * @brief Take undo steps the given amount
	 *
	 * When an UndoGui is registered to the UndoManager gui's undo() signal
	 * is connected to this slot. Emitting an undo() signal will make the 
	 * UndoManager perform undo actions the given number of steps. 
	 * @param steps Number of undo actions to make
	 */
	void doUndo(int steps);

	/**
	 * @brief Take redo steps the given amount
	 * @param steps Number of redo actions to make
	 * @sa UndoManager::doUndo(int steps)
	 */
	void doRedo(int steps);

public slots:
	/**
	 * @brief Adds a new action to the undo stack.
	 *
	 * If _unodEnabled is true the action will be stored otherwise it will
	 * be just ignored. When a new action is added redo items from the stack
	 * will be removed and the current action will be set to the one which was
	 * added.
	 * @param target Source of the action. When undoing/redoing this action
	 * restore() method of this UndoObject will be called.
	 * @param state UndoSate describing the state (action).
	 */
	void action(UndoObject* target, UndoState* state);

	/**
	 * @brief Informs UndoManager to perform undo
	 *
	 * If an undo is wanted and the caller is not registered UndoGui this method
	 * can be used. Useful for example with the menu entry for undo.
	 * @param steps Number of undo steps to take
	 */
	void undo(int steps);

	/**
	 * @brief Informs UndoManager to perform redo
	 * @param steps Number of redo steps to take
	 * @sa UndoManager::undo(int steps)
	 */
	void redo(int steps);

	/**
	 * @brief Sets the length of the undo stack.
	 *
	 * Tells how many UndoStates are stored.
	 * @param steps number of UndoStates to store in the undo stack
	 */
	void setHistoryLength(int steps);

signals:
	/**
	 * @brief Emitted when a new undo action is stored to the undo stack.
	 *
	 * This signal is connected to the registered UndoGuis for them to update the 
	 * visual representation of the undo stack.
	 * @param target Source of the action
	 * @param state UndoState describing the action
	 */
	void newAction(UndoObject* target, UndoState* state);

	/**
	 * @brief Emitted when an undo action has been done.
	 *
	 * It is connected to attached guis to inform them to update the visual
	 * representation of the undo stack. This signal will be also sent to the
	 * <code>UndoGui</code> where it came from.
	 * @param steps Number of steps to take
	 */
	void undoSignal(int steps);

	/**
	 * @brief Emitted when a redo action has been done.
	 * @param steps Number of steps to take
	 * @sa UndoManager::undoSignal(int steps)
	 */
	void redoSignal(int steps);

	/**
	 * @brief This signal is used to notify registered UndoGui instances that
	 * @brief history limit is reached and the last item from the stack 
	 * @brief representation should be removed.
	 */
	void popBack();

	/**
	 * @brief This signal is emitted when all requested undo/redo actions have been done.
	 *
	 * It could be used in the application as a signal when the view can be rendered
	 * again after undo/redo.
	 */
	void undoRedoDone();

};

typedef UndoManager Um;

#endif
