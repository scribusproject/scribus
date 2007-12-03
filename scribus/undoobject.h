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

#ifndef UNDOOBJECT_H
#define UNDOOBJECT_H

#include <QPixmap>
#include <QString>

#include "scribusapi.h"
#include "scguardedptr.h"

class UndoState;

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
 * @author Riku Leino riku@scribus.info
 * @date December 2004
 */
class SCRIBUS_API UndoObject
{
public:
	/** @brief Creates a new anonymous UndoObject instance  */
	UndoObject();

	/** @brief Creates a copy of an UndoObject instance  */
	UndoObject(const UndoObject& other);

	/** 
	 * @brief Creates a new UndoObject instance with the name <code>objectName</code>
	 * @param objectName Name of the UndoObject
	 */
	UndoObject(const QString &objectName, QPixmap *objectIcon = 0);

	/** @brief Destroys the object. */
	virtual ~UndoObject();

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
	ulong getUId() const;

	/**
	 * @brief Returns a guarded pointer
	 */
	const ScGuardedPtr<UndoObject>& undoObjectPtr();

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
private:
	/** @brief id number to be used with the next UndoObject */
	static ulong m_nextId;
	
	/** @brief unique id number */
	ulong m_id;
	
	/**
	 * @brief Name of the UndoObject
	 *
	 * This name will be used in UndoGui implementations
	 */
	QString m_uname;
	
	/**
	 * @brief Icon presenting the object.
	 *
	 * When used together with an UndoAction that has an image is this image
	 * drawn first then the action image is drawn on top of this.
	 */
	QPixmap *m_upixmap;

	/**
	 * @brief Guarded pointer
	 *
	 * Allows to warn undo system of an object deletion
	 */
	ScGuardedObject<UndoObject> m_objectPtr;
};
typedef ScGuardedPtr<UndoObject> UndoObjectPtr;

class SCRIBUS_API DummyUndoObject : public UndoObject
{
public:
	DummyUndoObject() {};
	virtual ~DummyUndoObject() {};
	//! \brief dummy implementation of the inherited one
	void restore(UndoState*, bool) {};
};

#endif
