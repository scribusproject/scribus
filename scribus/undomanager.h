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

#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <vector>
#include <utility>
#include <QObject>
#include <QPixmap>

#include "scribusapi.h"
#include "transaction.h"
#include "undostate.h"
#include "undoobject.h"
#include "undostack.h"

class QString;
class QPixmap;
class UndoGui;
class PrefsContext;

struct TransactionData;

/** @brief Key is the doc name, value is it's undo stack */
typedef QMap<QString, UndoStack> StackMap;

class SCRIBUS_API TransactionSettings
{
public:
	QString  targetName;
	QPixmap *targetPixmap;
	QString  actionName;
	QString  description;
	QPixmap *actionPixmap;

	TransactionSettings(void) { targetPixmap = actionPixmap = NULL; }  
};

/**
 * @brief TransactionState provides a container where multiple UndoStates can be stored
 * @brief as a single action which then appears in the attached <code>UndoGui</code>s.
 * @author Riku Leino riku@scribus.info
 * @date January 2005
 */
class TransactionState : public UndoState
{
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
	UndoState* at(int index);
	/**
	 * @brief Returns true if this transaction contains UndoObject with the id <code>uid</code>
	 * @brief otherwise returns false.
	 * @return true if this transaction contains UndoObject with the ide <code>uid</code>
	 * @return otherwise returns false.
	 */
	bool contains(int uid) const;
	
	/**
	 * @brief Tells if this transaction contains only UndoObject with ID uid
	 * 
	 * If a transaction contains only single UndoObject it will be safe to include
	 * it in the object specific undo.
	 * @param uid UndoObject's id to look for
	 * @return true if this transaction only contains actions of the UndoObject whose
	 *         id is uid
	 */
	bool containsOnly(int uid) const;
	/**
	 * @brief Replace object with id uid with new UndoObject newUndoObject.
	 * @param uid id of the object that is wanted to be replaced
	 * @param newUndoObject object that is used for replacing
	 * @return UndoObject which was replaced
	 */
	UndoObject* replace(ulong uid, UndoObject *newUndoObject);

	/** @brief undo all UndoStates in this transaction */
	void undo();
	/** @brief redo all UndoStates in this transaction */
	void redo();
private:
	/** @brief Number of undo states stored in this transaction */
	uint size_;
	/** @brief vector to keep the states in */
	std::vector<UndoState*> states_;
};



/**
    Class which handles Undo transactions. No data, just methods.
 */
class UndoTransaction : public Transaction
{
public:
	UndoTransaction(TransactionData* data);
	virtual ~UndoTransaction();
	virtual bool commit();
	virtual bool cancel();
	bool commit(const QString &targetName,
				QPixmap *targetPixmap,
				const QString &name = "",
				const QString &description = "",
				QPixmap *actionPixmap = 0);
};


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
class SCRIBUS_API UndoManager : public QObject
{
	Q_OBJECT
public:
	friend class UndoTransaction;
	
	/** @brief Marks for a global undo mode where ever UndoOjbect id is requested. */
	static const int GLOBAL_UNDO_MODE = -1;
	
	/**
	 * @brief When object specific mode is requested but no suitable object is selected
	 * @brief this can be passed to showObject() to clear the undo stack representations.
	 */
	static const int NO_UNDO_STACK = -2;
	
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
	 * @param actionName name for the transaction (f.e. "Move" would make with the above
	 * "Move Selection")
	 * The result can only be used for initializing a Transaction object, eg. on the stack:
	 *      Transaction groupTransaction(undoManger->beginTransaction(...));
	 * or the heap:
	 *      Transaction* groupTransactionPtr = new Transaction(undoManger->beginTransaction(...));
	 * @param description description for the transaction
	 * @param actionPixmap icon for the action performed by the transaction
	 * @sa commit()
	 */
	UndoTransaction beginTransaction(const QString &targetName = "",
									 QPixmap *targetPixmap = 0,
									 const QString &actionName = "",
									 const QString &description = "",
									 QPixmap *actionPixmap = 0);

	UndoTransaction beginTransaction(const TransactionSettings& settings);
	
	/**
	 * @brief Cancels the current transaction and deletes groupped <code>UndoState</code>s.
	 * @brief Nothing from canceled transaction will be sent to the undo gui widgets.
	 */
	//void cancelTransaction();
	
	/*
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
//	void commit(const QString &targetName = "",
//	            QPixmap *targetPixmap = 0,
//	            const QString &name = "",
//	            const QString &description = "",
//	            QPixmap *actionPixmap = 0);
	
	/**
	 * @brief Returns true if in transaction mode if not will return false.
	 * @return bool true if in transaction mode if not will return false
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

	/** clear the current undo stack */
	void clearStack();

	/**
	 * @brief Returns true if there are actions that can be undone otherwise returns false.
	 *
	 * This is useful when undo/redo actions are handled with a gui that is not attached to
	 * the UndoManager (f.e. menu items) and when those gui items are wanted to set enabled
	 * or disabled depending on the status of the undo stack.
	 * @return true if there are actions that can be undone otherwise returns false
	 */
	bool hasUndoActions(int uid = -1);
	
	/**
	 * @brief Returns true if there are actions that can be redone otherwise returns false.
	 * @return true if there are actions that can be redone otherwise returns false
	 * @sa UndoManager::hasUndoActions()
	 */
	bool hasRedoActions(int uid = -1);
	
	/**
	 * @brief Replace an UndoObject with the id uid with a new UndoObject new.
	 * @param uid Id for the UndoObject that is wanted to be replaced.
	 * @param newUndoObject UndoObject which will replace an old UndoObject in the stack.
	 * @return UndoObject which was replaced
	 */
	UndoObject* replaceObject(ulong uid, UndoObject *newUndoObject);
	
	/**
	 * @brief Returns the maximum length of the undostack.
	 * @return the maximum length of the undostack
	 */
	int getHistoryLength();

	/**
	 * @brief Returns true if in global mode and false if in object specific mode.
	 * @return true if in global mode and false if in object specific mode
	 */
	bool isGlobalMode();

private:
	/**
	 * @brief The only instance of UndoManager available.
	 *
	 * UndoManager is singleton and the instance can be queried with the method
	 * instance().
	 */
	static UndoManager* instance_;

	/** @brief Should undo states be stored or ignored */
	static bool undoEnabled_;

	/**
	 * @brief Tracks the state of _undoEnabled.
	 *
	 * This value is increased whenever setUndoEnabled(true) is called and decreased
	 * when setUndoEnabled(false) is called. This means _undoEnabled == true when this
	 * value is 0 and when its above zero _undoEnabled == false. Counting setUndoEnabled()
	 * calls this way guarantees that undo is not enabled accidentally calling
	 * setUndoEnabled(true) even it has been set false before this false-true pair touched it.
	 */
	static int undoEnabledCounter_;

	PrefsContext *prefs_;

	/** @brief Doc to which the currently active stack belongs */
	QString currentDoc_;

	/**
	 * @brief Id number of the object for what the object specific undo is shown
	 * @brief or -1 if global undo is used.
	 */
	int currentUndoObjectId_;

	/**
	 * @brief Stores the transactions which are currently started but not
	 * @brief canceled or commited.
	 */
	std::vector<TransactionData*> transactions_;

	/**
	 * @brief UndoGuis attached to this UndoManager
	 * @sa UndoGui
	 * @sa UndoWidget
	 * @sa UndoPalette
	 */
	std::vector<UndoGui*> undoGuis_;

	/**
	 * @brief Undo stacks for all open document
	 *
	 * Whenever current stack is used it's referred with <code>stacks_[currentDoc_]</code>
	 */
	StackMap stacks_;

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

	void setTexts();

public:
	/**
	 * @brief Returns last saved Undo state
	 */
	UndoState* getLastUndoState();
	/**
	* @name Action strings
	* Strings describing undo actions
	*/
	/*@{*/
	static QString AddVGuide;
	static QString AddHGuide;
	static QString DelVGuide;
	static QString DelHGuide;
	static QString DelVAGuide;
	static QString DelHAGuide;
	static QString MoveVGuide;
	static QString MoveHGuide;
	static QString RemoveAllGuides;
	static QString RemoveAllPageGuides;
	static QString LockGuides;
	static QString UnlockGuides;
	static QString Move;
	static QString Resize;
	static QString Rotate;
	static QString MoveFromTo;
	static QString ResizeFromTo;
	static QString ImageOffset;
	static QString ImageScale;
	static QString ImageOffsetFromTo;
	static QString ImageScaleFromTo;
	static QString Selection;
	static QString Group;
	static QString SelectionGroup;
	static QString Create;
	static QString CreateTo;
	static QString AlignDistribute;
	static QString ItemsInvolved;
	static QString ItemsInvolved2;
	static uint    ItemsInvolvedLimit;
	static QString Cancel;
	static QString SetFill;
	static QString ColorFromTo;
	static QString SetShade;
	static QString SetLineColor;
	static QString SetLineShade;
	static QString FlipH;
	static QString FlipV;
	static QString Lock;
	static QString UnLock;
	static QString SizeLock;
	static QString SizeUnLock;
	static QString EnablePrint;
	static QString DisablePrint;
	static QString Ungroup;
	static QString Delete;
	static QString Rename;
	static QString FromTo;
	static QString ApplyMasterPage;
	static QString Paste;
	static QString Cut;
	static QString Transparency;
	static QString LineTransparency;
	static QString LineStyle;
	static QString LineEnd;
	static QString LineJoin;
	static QString LineWidth;
	static QString NoStyle;
	static QString CustomLineStyle;
	static QString NoLineStyle;
	static QString StartArrow;
	static QString EndArrow;
	static QString StartAndEndArrow;
	static QString CreateTable;
	static QString RowsCols;
	static QString SetFont;
	static QString SetFontSize;
	static QString SetFontWidth;
	static QString SetFontHeight;
	static QString SetFontFill;
	static QString SetFontStroke;
	static QString SetFontFillShade;
	static QString SetFontStrokeShade;
	static QString SetKerning;
	static QString SetLineSpacing;
	static QString SetStyle;
	static QString SetLanguage;
	static QString AlignText;
	static QString SetFontEffect;
	static QString ImageFrame;
	static QString TextFrame;
	static QString LatexFrame;
	static QString Polygon;
	static QString BezierCurve;
	static QString Polyline;
	static QString PathText;
	static QString ConvertTo;
	static QString ImportSVG;
	static QString ImportEPS;
	static QString ImportBarcode;
	static QString ImportOOoDraw;
	static QString ImportAI;
	static QString ImportXfig;
	static QString ImportWMF;
	static QString ScratchSpace;
	//static QString TextFlow;
	static QString ObjectFrame;
	static QString BoundingBox;
	static QString ContourLine;
	static QString ImageClip;
	static QString NoTextFlow;
	static QString NoObjectFrame;
	static QString NoBoundingBox;
	static QString NoContourLine;
	static QString PageNmbr;
	static QString ImageScaling;
	static QString FrameSize;
	static QString FreeScaling;
	static QString KeepRatio;
	static QString BreakRatio;
	static QString EditContourLine;
	static QString EditShape;
	static QString ChangeShapeType;
	static QString ResetContourLine;
	static QString AddPage;
	static QString AddPages;
	static QString DeletePage;
	static QString DeletePages;
	static QString ChangePageProps;
	static QString AddLayer;
	static QString DuplicateLayer;
	static QString DeleteLayer;
	static QString RenameLayer;
	static QString RaiseLayer;
	static QString LowerLayer;
	static QString SendToLayer;
	static QString PrintLayer;
	static QString DoNotPrintLayer;
	static QString SetLayerName;
	static QString FlowLayer;
	static QString DisableFlowLayer;
	static QString SetLayerBlendMode;
	static QString SetLayerTransparency;
	static QString SetLayerLocked;
	static QString SetLayerUnlocked;
	static QString GetImage;
	static QString ChangeFormula;
	static QString MultipleDuplicate;
	static QString ApplyTextStyle;
	static QString MenuUndo;
	static QString MenuUndoEmpty;
	static QString MenuRedo;
	static QString MenuRedoEmpty;
	static QString EditContour;
	static QString ResetControlPoint;
	static QString ResetControlPoints;
	static QString ImageEffects;
	static QString InsertFrame;
	static QString AdjustFrameToImage;
	static QString Copy;
	static QString CopyPage;
	static QString ToOutlines;
	static QString EditText;
	static QString ClearImage;
	static QString ClearContent;
	static QString LinkTextFrame;
	static QString UnlinkTextFrame;
	static QString MovePage;
	/*@}*/

	/**
	 * @name Action icons
	 * Icons for undo actions
	 */
	/*@{*/
/*** Icons for UndoObjects *******************************************/
	static QPixmap *IImageFrame;
	static QPixmap *ITextFrame;
	static QPixmap *ILatexFrame;
	static QPixmap *ILine;
	static QPixmap *IPolygon;
	static QPixmap *IPolyline;
	static QPixmap *IPathText;
	static QPixmap *IGroup;
	static QPixmap *ILayer;
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
	static QPixmap *IEnablePrint;
	static QPixmap *IDisablePrint;
	static QPixmap *IDelete;
	static QPixmap *ICreate;
	static QPixmap *IPaste;
	static QPixmap *ICut;
	static QPixmap *ITransparency;
	static QPixmap *ILineStyle;
	static QPixmap *IArrow;
	static QPixmap *ITable;
	static QPixmap *IFont;
	static QPixmap *ISVG;
	static QPixmap *IEPS;
	static QPixmap *IAI;
	static QPixmap *IXFIG;
	static QPixmap *IWMF;
	static QPixmap *IImportOOoDraw;
	static QPixmap *IImageScaling;
	static QPixmap *IBorder;
	static QPixmap *IDocument;
	static QPixmap *ILayerAction;
	static QPixmap *IUp;
	static QPixmap *IDown;
	static QPixmap *IPrint;
	static QPixmap *IGetImage;
	static QPixmap *IChangeFormula;
	static QPixmap *IMultipleDuplicate;
	/*@}*/

protected:
	/** @brief Creates a new UndoManager instance */
	UndoManager();

	/** @brief Destroys the UndoManager instance */
	~UndoManager();

public slots:
	/**
	 * @brief Updates strings when the GUI language is changed.
	 */
	void languageChange();
	
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
	 * @brief Switches the state of UndoManager (global/object specific undo).
	 *
	 * If parameter uid is less than 0 global undo is used else the undo state
	 * is object specific using the object whose uid is given as a parameter.
	 * @param uid UndoObject's id for what the object specific undo is wanted or
	 * -1 if global undo is wanted.
	 */
	void showObject(int uid);

	/**
	 * @brief Sets the length of the undo stack.
	 *
	 * Tells how many UndoStates are stored.
	 * @param steps number of UndoStates to store in the undo stack
	 */
	void setHistoryLength(int steps);
	void setAllHistoryLengths(int steps);

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
	 * @brief Emitted when a new actions comes in and UndoManager is in object
	 * @brief specific mode and action does not belong to the currently selected
	 * @brief object.
 	 */
	void clearRedo();

	/**
	 * @brief This signal is used to notify registered UndoGui instances that
	 * @brief history limit is reached and the last item from the stack
	 * @brief representation should be removed.
	 */
	void popBack();

	/**
	 * @brief This signal is emitted when beginning a series of undo/redo actions
	 *
	 * It could be used in the application as a signal to disable temporarily gui
	 * updates.
	 */
	void undoRedoBegin();

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
