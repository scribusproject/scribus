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

#ifndef UNDOSTATE_H
#define UNDOSTATE_H

#include <QMap>
#include <QPixmap>

#include "scribusapi.h"
#include "undoobject.h"

class QString;
class PageItem;

/**
 * @brief UndoState describes an undoable state (action).
 *
 * Undoable objects implement undo/redo by sending and receiving UndoState
 * subclasses. This means that they will need to produce some sort of information
 * for the UndoState subclass (if no suitable subclass exists it must be also
 * created) and later they will need to be able to restore the state described
 * by the subclass to apply undo/redo.
 *
 * UndoManager will handle the deletion of UndoState objects.
 *
 * @sa SimpleState
 * @author Riku Leino riku@scribus.info
 * @date December 2004
 */
class SCRIBUS_API UndoState
{
public:
	/**
	 * @brief Creates a new UndoState instance.
	 * @param name Name of the state (action). Will be used when describing the
	 * state in UndoGui subclasses.
	 * @param description Description of the state (action)
	 * @param pixmap Pointer to an icon describing the action visually.
	 */
	UndoState(const QString& name, const QString& description = 0, QPixmap* pixmap = 0);

	virtual ~UndoState();

	/**
	 * @brief Returns name of the state (action).
	 * @return name of the state
	 */
	virtual QString getName();

	/**
	 * @brief Set the name for this UndoState.
	 * @param newName name for this UndoState
	 */
	virtual void setName(const QString &newName);

	/**
	 * @brief Returns description of the state.
	 * @return description of the state
	 */
	virtual QString getDescription();

	/**
	 * @brief Set the description for this UndoState
	 * @param newDescription description for this UndoState
	 */
	virtual void setDescription(const QString &newDescription);

	/**
	 * @brief Returns a pointer to the icon attached to the state.
	 * @return A pointer to the icon attached to the state
	 */
	virtual QPixmap* getPixmap();

	/**
	 * @brief Set the icon for this UndoState.
	 * @param newPixmap icon for this UndoState
	 */
	virtual void setPixmap(QPixmap *newPixmap);

	/** @brief undo the state described by this UndoState,
	 *  @brief requires related UndoObject */
	virtual void undo();
	/** @brief redo the state described by this UndoState,
	 *  @brief requires the related UndoObject */
	virtual void redo();

	/** @brief Set the UndoObject this state belongs to */
	virtual void setUndoObject(UndoObject *object);
	/** @brief return the UndoObject this state belongs to */
	virtual UndoObject* undoObject();
	int transactionCode;

private:
	/** @brief Name of the state (operation) (f.e. Move object) */
	QString actionName_;
	/** @brief Detailed description of the state (operation). */
	QString actionDescription_;
	/** @brief Icon related to the state (operation) */
	QPixmap *actionPixmap_;
	/** @brief UndoObject this state belongs to */
	UndoObjectPtr undoObject_;
};

/*** SimpleState **************************************************************************/

/**
 * @brief SimpleState provides a simple implementation of the UndoState.
 *
 * SimpleState uses a <code>QMap</code> to store key-value pairs that can be queried and
 * set using it's get() and set() methods.
 *
 * @author Riku Leino tsoots@gmail.com
 * @date December 2004
 */
class SCRIBUS_API SimpleState : public UndoState
{
public:
	/**
	 * @brief Creates a new SimpleState instance.
	 * @param name Name of the state (action). Will be used when describing the
	 * state in UndoGui subclasses.
	 * @param description Description of the state (action)
	 * @param pixmap Pointer to an icon describing the state (action) visually.
	 */
	SimpleState(const QString& name, const QString& description = 0, QPixmap* pixmap = 0);

	virtual ~SimpleState();

	/**
	 * @brief Returns true if parameter key exists in the map.
	 * @param key Key that is searched from the map
	 * @return true if parameter key exists in the map if not false
	 */
	bool contains(const QString& key);

	/**
	 * @brief Returns the QString value attached to the key.
	 *
	 * If key is not found from the map it will be added there with the
	 * value given as a parameter def. In such case <code>def</code> will also
	 * be returned.
	 * @param key Key that is searched from the map
	 * @param def Default value to be used if key is not found from the map
	 * @return Value attached to the key in the map. If the key is not found
	 * from the map it will be added with the value described in the param
	 * <code>def</code> which is then returned.
	 */
	QString get(const QString& key, const QString& def = "");

	/**
	 * @brief Returns the int value attached to the key.
	 *
	 * Values are stored as <code>QString</code>s in the map and when queried
	 * with this method value attached to the key is converted to an int. If
	 * the conversion fails value of the parameter <code>def</code> will be returned.
	 * If key is not found from the map it will be added there with the
	 * value given as a parameter <code>def</code>. In such case <code>def</code>
	 * will also be returned.
	 * @param key Key that is searched from the map
	 * @param def Default value to be used if key is not found from the map
	 * @return <code>int</code> value attached to the key in the map. If the key is not found
	 * from the map it will be added with the value described in the param
	 * <code>def</code> which is then returned.
	 */
	int getInt(const QString& key, int def = 0);

	/**
	 * @brief Returns the uint value attached to the key.
	 *
	 * Values are stored as <code>QString</code>s in the map and when queried
	 * with this method value attached to the key is converted to an int. If
	 * the conversion fails value of the parameter <code>def</code> will be returned.
	 * If key is not found from the map it will be added there with the
	 * value given as a parameter <code>def</code>. In such case <code>def</code>
	 * will also be returned.
	 * @param key Key that is searched from the map
	 * @param def Default value to be used if key is not found from the map
	 * @return <code>uint</code> value attached to the key in the map. If the key is not found
	 * from the map it will be added with the value described in the param
	 * <code>def</code> which is then returned.
	 */
	uint getUInt(const QString& key, uint def = 0);

	/**
	 * @brief Returns the double value attached to the key.
	 *
	 * Values are stored as <code>QString</code>s in the map and when queried
	 * with this method value attached to the key is converted to a double. If
	 * the conversion fails value of the parameter <code>def</code> will be returned.
	 * If key is not found from the map it will be added there with the
	 * value given as a parameter def. In such case <code>def</code> will also be returned.
	 * @param key Key that is searched from the map
	 * @param def Default value to be used if key is not found from the map
	 * @return <code>Double</code> value attached to the key in the map. If the key is not found
	 * from the map it will be added with the value described in the parameter
	 * <code>def</code> which is then returned.
	 */
	double getDouble(const QString& key, double def = 0.0);

	/**
	 * @brief Returns the boolean value attached to the key.
	 *
	 * Values are stored as <code>QString</code>s in the map and when queried
	 * with this method value attached to the key is converted to a bool. If
	 * the conversion fails value of the parameter <code>def</code> will be returned.
	 * If key is not found from the map it will be added there with the
	 * value given as a parameter def. In such case <code>def</code> will also be returned.
	 * @param key Key that is searched from the map
	 * @param def Default value to be used if key is not found from the map
	 * @return <code>Bool</code> value attached to the key in the map. If the key is not found
	 * from the map it will be added with the value described in the parameter
	 * <code>def</code> which is then returned.
	 */
	bool getBool(const QString& key, bool def = false);

	/**
	 * @brief Set a value for the key.
	 * @param key Key that can be later used to query the value.
	 * @param value Value attached to the key.
	 */
	void set(const QString& key, const QString& value);

	/**
	 * @brief Set a value for the key.
	 * @param key Key that can be later used to query the value.
	 * @param value Value attached to the key.
	 */
	void set(const QString& key, int value);

	/**
	 * @brief Set a value for the key.
	 * @param key Key that can be later used to query the value.
	 * @param value Value attached to the key.
	 */
	void set(const QString& key, uint value);

	/**
	 * @brief Set a value for the key.
	 * @param key Key that can be later used to query the value.
	 * @param value Value attached to the key.
	 */
	void set(const QString& key, double value);

	/**
	 * @brief Set a value for the key.
	 * @param key Key that can be later used to query the value.
	 * @param value Value attached to the key.
	 */
	void set(const QString& key, bool value);

private:
	/** @brief QMap to store key-value pairs */
	QMap<QString, QString> values_;
};

/*** ItemState ***************************************************************************/

template<class C>
class ItemState : public SimpleState
{
public:
	ItemState(const QString& name, const QString& description = 0, QPixmap* pixmap = 0)
	: SimpleState(name, description, pixmap) {}
	~ItemState() {}
	void setItem(const C &c) { item_ = c; }
	C getItem() const { return item_; }
private:
	C item_;
};

#endif
