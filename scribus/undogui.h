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

#ifndef UNDOGUI_H
#define UNDOGUI_H

#include "undoobject.h"
#include "undostate.h"
#include <vector>
#include <qwidget.h>
#include <qdialog.h>
#include <qstring.h>
#include <qlistbox.h>
#include <qtoolbutton.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpopupmenu.h>

class PrefsContext;

/**
 * @brief UndoGui is a virtual superclass for undo/redo widgets.
 *
 * UndoGui is a virtual superclass for all the classes that wants to represent
 * undo/redo data. Subclasses must be registered to the UndoManager instance.
 * Subclasses are expected to keep the whole stack with them and they must be
 * able to work undo/redo situations by themselves. Only new undo actions and 
 * steps (undo or redo) are reported back.
 * 
 * @sa UndoWidget
 * @sa UndoPalette
 *
 * @author Riku Leino  tsoots@gmail.com
 * @date December 2004
 */
class UndoGui : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief Creates a new UndoGui instance. 
	 * @param parent Parent object for UndoGui
	 * @param name Name of the object
	 * @param f widget flags
	 */
	UndoGui(QWidget* parent = 0, const char* name = 0, WFlags f = 0);

	/** @brief Destroys the widget */
	virtual ~UndoGui() {};

	/** @brief Empties the undo stack representation. */
	virtual void clear() = 0;

public slots:
	/**
	 * @brief Insert a new undo item.
	 *
	 * Insert a new undo item. After an undoable action has happened and
	 * if this UndoGui is registered to the UndoManager this slot will
	 * be used to register the action to the widget.
	 * @param target Target of the undo action
	 * @param state State describing the action
	 */
	virtual void insertUndoItem(UndoObject* target, UndoState* state) = 0;

	/**
	 * @brief Insert a new redo item.
	 * @param target Target of the redo action
	 * @param state State describing the action
	 */
	virtual void insertRedoItem(UndoObject* target, UndoState* state) = 0;

	/**
	 * @brief Update undo stack representation with number of steps.
	 * 
	 * When some other UndoGui subclass has received an undo request this
	 * slot is used to inform this UndoGui to update it's UndoStack 
	 * representation.
	 * @param steps Number of steps to take
	 */
	virtual void updateUndo(int steps) = 0;

	/**
	 * @brief Update redo stack representation with number of steps.
	 * 
	 * When some other registered UndoGui subclass has received a redo
	 * request this slot is used to inform this UndoGui to update it's 
	 * undo stack representation.
	 * @param steps Number of steps to take
	 */
	virtual void updateRedo(int steps) = 0;

	/** @brief Remove the last (oldest) item from the undo stack representation. */
	virtual void popBack() = 0;
/* signals: do not implement these but emit when action happens
	virtual void undo(int steps) = 0;
	virtual void redo(int steps) = 0;
*/
};

/*** UndoGui implementations **************************************************/
/*** UndoWidget ***************************************************************/

/**
 * @brief Tool bar buttons with pop up menus for undo and redo.
 * 
 * Provides tool bar buttons for undo and redo. There is also a pop up menu
 * attached to these buttons for taking more than a single undo/redo step.
 *
 * @author Riku Leino tsoots@gmail.com
 * @date December 2004
 */
class UndoWidget : public UndoGui
{
	Q_OBJECT

private:
	/** @brief Items visible in the pop up menus */
	static const uint MENU_HEIGHT = 5;
	std::vector<QString> undoItems;
	std::vector<QString> redoItems;
	/* BnF buttons
	QToolButton* undoButton;
	QToolButton* redoButton;
	*/
	QPopupMenu* undoMenu;
	QPopupMenu* redoMenu;
	void updateUndoMenu();
	void updateRedoMenu();
	void clearRedoMenu();
public:
	/** @brief Creates a new UndoWidget instance. */
	UndoWidget(QWidget* parent = 0, const char* name = 0);

	/** @brief Destroys the widget */
	virtual ~UndoWidget();

	/** @brief Empties the undo stack for this widget. */
	void clear();
private slots:
	void undoClicked();
	void redoClicked();
	void undoMenuClicked(int id);
	void redoMenuClicked(int id);
public slots:
	/**
	 * @brief Insert a new undo item.
	 * 
	 * After an undoable action has happened and
	 * if this UndoGui is registered to the UndoManager this slot will
	 * be used to register the action to the widget.
	 * @param target Target of the undo action
	 * @param state State describing the action
	 */
	void insertUndoItem(UndoObject*, UndoState* state);

	/**
	 * @brief Insert a new redo item.
	 * @param target Target of the redo action
	 * @param state State describing the action
	 */
	void insertRedoItem(UndoObject*, UndoState* state);

	/**
	 * @brief Update undo stack representation with number of steps.
	 * 
	 * When some other UndoGui subclass has received an undo request this
	 * slot is used to inform this UndoGui to update it's UndoStack 
	 * representation.
	 * @param steps Number of steps to take
	 */
	void updateUndo(int steps);

	/**
	 * @brief Update redo stack representation with number of steps.
	 * 
	 * When some other UndoGui subclass has received a redo request this
	 * slot is used to inform this UndoGui to update it's UndoStack 
	 * representation.
	 * @param steps Number of steps to take
	 */
	void updateRedo(int steps);

	/** @brief Remove the last (oldest) item from the undo stack representation. */
	void popBack();
signals:
	/** 
	 * @brief Emitted when undo is requested.
	 * 
	 * When undo action is done by this class this signal is emitted.
	 * @param steps How many undo steps to take
	 */
	void undo(int steps);

	/** 
	 * @brief Emitted when redo is requested.
	 * 
	 * When redo action is done by this class this signal is emitted.
	 * @param steps How many redo steps to take
	 */
	void redo(int steps);
};

/*** UndoPalette **************************************************************/

/**
 * @brief UndoGui subclass which creates an undo history window
 * 
 * Provides a regular undo history window where a user can click on any action
 * and related undo/redo steps are taken. Undo steps are represented as a 
 * <code>QListBox</code> where there can be an image to describe the action
 * visually, the name of the object and the name of the action per row.
 *
 * @author Riku Leino tsoots@gmail.com
 * @date December 2004
 */
class UndoPalette : public UndoGui
{
	Q_OBJECT

private:
	int currentSelection;
	int redoItems;
	QListBox* undoList;
	QPushButton* undoButton;
	QPushButton* redoButton;
	QKeySequence initialUndoKS;
	QKeySequence initialRedoKS;
	PrefsContext* undoPrefs;
	void updateList();
	void removeRedoItems();
	void storePosition();
	
/*** UndoPalette::UndoItem ****************************************************/
	
	/** @brief UndoItem provides a custom QListBoxItem for the undo history view. */
	class UndoItem : public QListBoxItem
	{
	private:
		/** @brief An icon for the undo target */
		QPixmap *targetpixmap;
		/** @brief An icon for the undo state (action) */
		QPixmap *actionpixmap;
		/** @brief Name of the target of the state (action) */
		QString target;
		/** @brief Undo action's name */
		QString action;
		/** @brief Description of the action */
		QString description;
	public:
		/** @brief Create an empty UndoItem object */
		UndoItem();
		/**
		 * @brief Create a copy of <code>another</code> UndoItem instance.
		 * @param another UndoItem instance to copy
		 */
		UndoItem(const UndoItem &another);
		/**
		 * @brief Create an UndoItem instance.
		 * @param targetName Name of the target. Will appear on the first row of
		 * the item.
		 * @param actionName Name of the state (action). Will appear on the 
		 * second row of the item.
		 * @param actionDescription This description of the action will be used as a
		 * tooltip when the mouse cursor is over the item.
		 * @param actionPixmap Icon for the state (action). Will appear on front
		 * of the text parts.
		 */ 
		UndoItem(const QString &targetName,
                 const QString &actionName,
                 const QString &actionDescription,
                 QPixmap *targetPixmap,
                 QPixmap *actionPixmap);
		~UndoItem();
		void paint(QPainter *painter);
		int height(const QListBox*) const;
		int width(const QListBox*) const;
		QString getDescription();
	};
	
/******************************************************************************/

private slots:
	void undoClicked();
	void redoClicked();
	void undoListClicked(int i);
	void showToolTip(QListBoxItem *i);
	void removeToolTip();
	void objectCheckBoxClicked(bool on);

protected:
	/**
	 * @brief Captures the close event and changes it to hide.
	 * 
	 * UndoPalette should not be closed but hidden instead. Method closeEvent()
	 * will catch the close attempt and will transfer it to hide.
	 */
	void closeEvent(QCloseEvent* e);

	/** @brief Stores the geometry of the window when hiding. */
	void hideEvent(QHideEvent*);

	/** @brief Hide the window when ESC is pressed */
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

public:
	/** 
	 * @brief Creates a new UndoPalette instance.
	 * 
	 * Creates a new UndoPalette instance. After creation of an UndoPalette it must
	 * be registered to the UndoManager with UndoManager's registerGui() method.
	 */
	UndoPalette(QWidget* parent = 0, const char* name = 0);

	/** @brief Destroys the widget */
	~UndoPalette();

	/** @brief Empties the undo stack for this widget. */
	void clear();

public slots:
	/**
	 * @brief Insert a new undo item.
	 * 
	 * Insert a new undo item. After an undoable action has happened and
	 * if this UndoGui is registered to the UndoManager this slot will
	 * be used to register the action to the widget.
	 * @param target Target of the undo action
	 * @param state State describing the action
	 */
	void insertUndoItem(UndoObject* target, UndoState* state);

	/**
	 * @brief Insert a new redo item.
	 * @param target Target of the redo action
	 * @param state State describing the action
	 */
	void insertRedoItem(UndoObject* target, UndoState* state);

	/**
	 * @brief Update undo stack representation with number of steps.
	 * 
	 * When some other UndoGui subclass has received an undo request this
	 * slot is used to inform this UndoGui to update it's UndoStack 
	 * representation.
	 * @param steps Number of steps to take
	 */
	void updateUndo(int steps);

	/**
	 * @brief Update redo stack representation with number of steps.
	 * 
	 * When some other UndoGui subclass has received a redo request this
	 * slot is used to inform this UndoGui to update it's UndoStack 
	 * representation.
	 * @param steps Number of steps to take
	 */
	void updateRedo(int steps);

	/** @brief Remove the last (oldest) item from the undo stack representation. */
	void popBack();

	/** @brief Restore the size and position of the window when shown. */
	void show();
			   
	/** @brief Recieve prefsChanged() signal to update shortcuts. */
	void updateFromPrefs();

signals:

	/**
	 * @brief Emitted when undo behaviour should be changed from global undo
	 * @brief to object specific undo and other way around.
	 * @param isEnabled true if object specific undo is wanted, false for global undo
	 */
	void objectMode(bool isEnabled);
	/** 
	 * @brief Emitted when undo is requested.
	 * 
	 * When undo action is done by this class this signal is emitted.
	 * @param steps How many undo steps to take
	 */
	void undo(int steps);

	/** 
	 * @brief Emitted when redo is requested.
	 * 
	 * When redo action is done by this class this signal is emitted.
	 * @param steps How many redo steps to take
	 */
	void redo(int steps);
	/** 
	 * @brief Emitted when palette is closed.
	 * 
	 * When the user closes the palette this signal is emitted.
	 */
	void closePalette(bool);
};

#endif // UNDOGUI_H
