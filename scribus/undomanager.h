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

/*** UndoManager::TransactionState ***************************************************/

	/**
	 * @brief TransactionState provides a container where multiple actions can be stored
	 * @brief as a single action which then appears in the attached <code>UndoGui</code>s.
	 * @author Riku Leino tsoots@gmail.com
	 * @date January 2005
	 */
	class TransactionState : public UndoState
	{
	private:
		/** @brief Number of undo states stored in this transaction */
		uint _size;
		/** @brief vector to keep the states in */
		std::vector<ActionPair*> states;
	public:
		/** @brief Creates a new TransactionState instance */
		TransactionState();
		/** @brief Destroys the TransactionState instance */
		~TransactionState();
		/** 
		 * @brief Add a new <code>UndoState</code> object to the transaction.
		 * @param state state to be added to the transaction
		 */
		void pushBack(UndoObject *target, UndoState *state);
		/**
		 * @brief Returns the count of the <code>UndoState</code> objects in this transaction.
		 * @return count of the <code>UndoState</code> objects in this transaction
		 */
		uint sizet();
		/** @brief Use the name from last action added to this <code>TransactionState</code> */
		void useActionName();
		/**
		 * @brief Returns an <code>UndoState</code> object at <code>index</code>.
		 * @param index index from where an <code>UndoState</code> object is returned.
		 * If <code>index</code> is out of scope <code>NULL</code> will be rerturned.
		 * @return <code>UndoState</code> object from <code>index</code> or <code>NULL</code> 
		 * if <code>index</code> is out of scope.
		 */
		ActionPair* at(int index);
		/**
		 * @brief Replace object with id uid with new UndoObject newUndoObject.
		 * @param uid id of the object that is wanted to be replaced
		 * @param newUndoObject object that is used for replacing
		 */
		void replace(ulong uid, UndoObject *newUndoObject);
	};

/*** UndoManager::TransactionObject ***************************************************/

	/**
	 * @brief Dummy subclass of <code>UndoObject</code> which is used for holding the name
	 * @brief for the transaction's target object(group) (f.e "Selection", "Group", "Script"...).
	 * @author Riku Leino tsoots@gmail.com
	 * @date January 2005
	 */
	class TransactionObject : public UndoObject
	{
	public:
		TransactionObject() {};
		virtual ~TransactionObject() {};
		void restore(UndoState*, bool) {};
	};

/**************************************************************************************/

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
	 * @brief Stores the transactions which are currently started but not
	 * @brief canceled or commited.
	 */
	std::vector<std::pair<TransactionObject*, TransactionState*> > transactions;

	/**
	 * @brief If in transaction mode this is the container for incoming <code>UndoState</code>
	 * @brief objects.
	 *
	 * It is also used to detect if we are in the transaction mode. When it is <code>NULL</code>
	 * normal mode is on.
	 */
	TransactionState *transaction;

	/** @brief Dummy object for storing transaction target's name */
	TransactionObject *transactionTarget;

	/** 
	 * @brief UndoGuis attached to this UndoManager
	 * @sa UndoGui
	 * @sa UndoWidget
	 * @sa UndoPalette
	 */
	std::vector<UndoGui*> undoGuis;

	/** @brief Undo stacks for all open documents */
	StackMap stacks;

	/** @brief Maximum length of the undo stack. 0 marks for infinite length. */
	int historyLength;

	/**
	 * @brief Initializes the UndoGui.
	 * @param gui UndoGui to be initialized
	 * @param uid UndoObject's id if in object specific mode or -1 to tell 
	 * that global undo is used.
	 */
	void setState(UndoGui* gui, int uid = -1);

	/**
	 * @brief Disconnect all attached UndoGui instances from signals provided by this
	 * @brief class and other UndoGui objects.
	 */
	void connectGuis();
	/**
	 * @brief Connect all attached UndoGui instances to signals provided by this
	 * @brief class and other UndoGui objects.
	 */
	void disconnectGuis();
	/**
	 * @brief Load icons needed for Action History window.
	 */
	void initIcons();
	/**
	 * @brief Checks the stack length against historyLength variable and removes
	 * @brief actions until it is of right length.
	 *
	 * This method is used when a user alters the history length option in preferences
	 * and whenever a new action is added to the stack.
	 */
	void checkStackLength();
	/**
	 * @brief Extracts actions from TransactionState object and undos them.
	 * @param tstate TransactionState object from where the actions are going to be extracted.
	 */
	void doTransactionUndo(TransactionState *tstate);
	/**
	 * @brief Extracts actions from TransactionState object and redos them.
	 * @param tstate TransactionState object from where the actions are going to be extracted.
	 */
	void doTransactionRedo(TransactionState *tstate);

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
	 * @brief Start a transaction. 
	 *
	 * After this method has been invoked <code>UndoManager</code> will switch to the
     * transaction (silent) mode where it does not report actions to the attached 
	 * <code>UndoGui</code> widgets but stores all incoming <code>UndoState</code> objects into
	 * the transaction container which after call to the method commit() will be sent
	 * to the guis as a single undo action. Transaction can be named when starting it or
	 * naming can be done when commiting it.
	 * @param targetName name for the target of this transaction (f.e. "Selection")
	 * @param targetPixmap Icon for the target on which this transaction works.
	 * this icon will be drawn first when the action is presented in Action History
	 * window and icon for the action will be drawn over this one.
	 * @param name name for the transaction (f.e. "Move" would make with the above
	 * "Move Selection")
	 * @param description description for the transaction
	 * @param actionPixmap icon for the action performed by the transaction
	 * @sa commit()
	 */
	void beginTransaction(const QString &targetName = "",
                          QPixmap *targetPixmap = 0,
                          const QString &actionName = "",
                          const QString &description = "",
                          QPixmap *actionPixmap = 0);

	/** 
	 * @brief Cancels the current transaction and deletes groupped <code>UndoState</code>s.
	 * @brief Nothing from canceled transaction will be sent to the undo gui widgets.
	 */
	void cancelTransaction();

	/** 
	 * @brief Commit the current transaction.
	 *
	 * Current transaction will be commited and <code>UndoManager</code> will be switched
	 * to the normal mode. Commited transaction will be sent to the attached undo gui 
	 * widgets and it will show up there as a single undo action. Details used as a parameter
	 * will be details shown in the gui widgets.
	 * @param targetName name for the target of this transaction (f.e. "Selection")
	 * @param targetPixmap Icon for the target on which this transaction works.
	 * this icon will be drawn first when the action is presented in Action History
	 * window and icon for the action will be drawn over this one.
	 * @param name name for the action
	 * @param description description for the action
	 * @param actionPixmap icon for the action performed by the transaction
	 * @sa beginTransaction()
	 */
	void commit(const QString &targetName = "",
                QPixmap *targetPixmap = 0,
                const QString &name = "",
                const QString &description = "",
                QPixmap *actionPixmap = 0);

	/**
	 * @brief Returns true if in transaction mode if not will return false.
	 * @return true if in transaction mode if not will return false
	 */
	bool isTransactionMode();

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
	void renameStack(const QString& newName);

	/**
	 * @brief Remove the stack with the name <code>stackName</code>
	 * @param stackName Name of the stack that is wanted to be removed
	 */
	void removeStack(const QString& stackName);

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
	 * @brief Replace an UndoObject with the id uid with a new UndoObject new.
	 * @param uid Id for the UndoObject that is wanted to be replaced.
	 * @param newUndoObject UndoObject which will replace an old UndoObject in the stack.
	 */
	void replaceObject(ulong uid, UndoObject *newUndoObject);

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
	static const QString Rotate;
	static const QString MoveFromTo;
	static const QString ResizeFromTo;
	static const QString RotateFromTo;
	static const QString Selection;
	static const QString Group;
	static const QString SelectionGroup;
	static const QString Create;
	static const QString CreateTo;
	static const QString AlignDistribute;
	static const QString ItemsInvolved;
	static const QString Cancel;
	static const QString SetFill;
	static const QString ColorFromTo;
	static const QString SetShade;
	static const QString ShadeFromTo;
	static const QString SetLineColor;
	static const QString SetLineShade;
	static const QString FlipH;
	static const QString FlipV;
	static const QString Lock;
	static const QString UnLock;
	static const QString SizeLock;
	static const QString SizeUnLock;
	static const QString Ungroup;
	static const QString Delete;
	static const QString Rename;
	static const QString FromTo;
	static const QString ApplyTemplate;
	static const QString Paste;
	static const QString Cut;
	static const QString Transparency;
	static const QString LineTransparency;
	static const QString LineStyle;
	static const QString LineEnd;
	static const QString LineJoin;
	static const QString LineWidth;
	static const QString NoStyle;
	static const QString CustomLineStyle;
	static const QString NoLineStyle;
	static const QString StartArrow;
	static const QString EndArrow;
	static const QString CreateTable;
	static const QString RowsCols;
	/*@}*/

	/**
	 * @name Action icons
	 * Icons for undo actions
	 */
	/*@{*/
/*** Icons for UndoObjects *******************************************/
	static QPixmap *IImageFrame;
	static QPixmap *ITextFrame;
	static QPixmap *ILine;
	static QPixmap *IPolygon;
	static QPixmap *IPolyline;
	static QPixmap *IPathText;
	static QPixmap *IGroup;
/*** Icons for actions ***********************************************/
	static QPixmap *IMove;
	static QPixmap *IResize;
	static QPixmap *IRotate;
	static QPixmap *IGuides;
	static QPixmap *ILockGuides;
	static QPixmap *IAlignDistribute;
	static QPixmap *IFill;
	static QPixmap *IShade;
	static QPixmap *IFlipH;
	static QPixmap *IFlipV;
	static QPixmap *ILock;
	static QPixmap *IUnLock;
	static QPixmap *IDelete;
	static QPixmap *ICreate;
	static QPixmap *IPaste;
	static QPixmap *ICut;
	static QPixmap *ITransparency;
	static QPixmap *ILineStyle;
	static QPixmap *IArrow;
	static QPixmap *ITable;
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
	 * @param targetPixmap Is used to override the default target icon in this action.
	 */
	void action(UndoObject* target, UndoState* state, QPixmap *targetPixmap = 0);

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
	 * @param targetName Is used to override the default target name in this action.
	 * @param targetPixmap Is used to override the default target icon in this action.
	 */
	void action(UndoObject* target, UndoState* state, const QString &targetName, QPixmap *targetPixmap = 0);

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
