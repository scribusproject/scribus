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

#ifndef UNDOOBJECT_H
#define UNDOOBJECT_H

#include <qstring.h>
#include <qpixmap.h>
#include "undostate.h"

/**
 * @brief Superclass for all objects that are wanted to have undoable actions.
 * 
 * The most important feature of UndoObject is the restore() method which must be
 * implemented in the subclass. When an action that is wanted to be undoable 
 * happens UndoObject subclass must use UndoManager::action() method to store the
 * action using UndoState object. Then when a user undos this action UndoManager
 * will send the UndoState object back to this UndoObject by using the restore() 
 * method.
 *
 * What is needed for an undo/redo:
 <ol>
	<li>UndoObject creates an UndoState object describing the action</li>
	<li>Sends it to the UndoManager</li>
	<li>When an undo/redo is requested UndoManager will send this very same UndoState
        object back to the UndoObject which then uses it to restore the state.</li>
 </ol>
 *
 * @author Riku Leino tsoots@gmail.com
 * @date December 2004
 */
class UndoObject
{
private:
	/** @brief id number to be used with the next UndoObject */
	static ulong nextId;

	/** @brief unique id number */
	ulong _id;

	/**
	 * @brief Name of the UndoObject
	 *
	 * This name will be used in UndoGui implementations
	 */
	QString uname;

	/**
	 * @brief Icon presenting the object.
	 *
	 * When used together with an UndoAction that has an image is this image
	 * drawn first then the action image is drawn on top of this.
	 */
	QPixmap *upixmap;
public:
	/** @brief Creates a new anonymous UndoObject instance  */
	UndoObject();

	/** 
	 * @brief Creates a new UndoObject instance with the name <code>objectName</code>
	 * @param objectName Name of the UndoObject
	 */
	UndoObject(const QString &objectName, QPixmap *objectIcon = 0);

	/** @brief Destroys the object. */
	virtual ~UndoObject() {};

	/**
	 * @brief Returns the name of the UndoObject.
	 * @return the name of the UndoObject
	 */
	virtual QString getUName();

	/**
	 * @brief Set the name of the UndoObject
	 * @param newUName New name for the UndoObject
	 */
	virtual void setUName(QString newUName);

	/**
	 * @brief Returns the pixmap connected to this object.
	 * @return pixmap connected to this object
	 */
	virtual QPixmap* getUPixmap();

	/**
	 * @brief Set the pixmap for this object.
	 * @param newUPixmap pixmap for this object
	 */
	virtual void setUPixmap(QPixmap *newUPixmap);

	/**
	 * @brief Returns an unique identifier number for the UndoObject
	 * @return unique identifier number for the UndoObjet
	 */
	virtual ulong getUId();

	/**
	 * @brief Method used when an undo/redo is requested.
	 * 
	 * UndoObject must know how to handle the UndoState object given as a 
	 * parameter. It is the same object that was send from the UndoObject to
	 * the UndoManager when the action happened.
	 * @param state State describing the action that is wanted to be undone/redone
	 * @param isUndo If true undo is wanted else if false redo.
	 */
	virtual void restore(UndoState* state, bool isUndo) = 0;
};

#endif
