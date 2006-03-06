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

#ifndef STYLEITEM_H
#define STYLEITEM_H

#include <qobject.h>

class QString;
class QStringList;
class ScribusDoc;
class QTabWidget;

/**
 * @brief Represents a style type that can be added to the Style Manager
 * @brief palette.
 *
 * This class is ment to be used as a parent class for any style types
 * that are wanted to be configured using the Style Manager palette.
 * @author Riku Leino <riku@scribus.info>
 * @date November 2005
 */
class StyleItem : public QObject {
	Q_OBJECT
public:
	StyleItem() : QObject() {};
	virtual ~StyleItem() {};

	/** 
	 * @brief return the QWidget for editing style's properties
	 *
	 * This is the widget where the attributes of style are edited. It
	 * will be placed on the main tab widget's Properties page.
	 */
	virtual QTabWidget* widget() = 0;

	/** @brief name of the style (Paragraph style, Character style...) */
	virtual QString typeName() = 0;

	/**
	 * @brief Whenever style manager is shown attached StyleItems get the current doc
	 * @brief passed to them with this function.
	 */
	virtual void currentDoc(ScribusDoc *doc) = 0;

	/** @brief existing styles in this category */
	virtual QStringList styles() = 0;

	/**
	 * @brief Whenever this function is called StyleItem must update the main
	 * @brief widget with the corresponding data.
	 *
	 * Whenever user selects a style from the style list in the manager
	 * this slot is called from the StyleItem. StyleItem must then update
	 * the main widget with the data related to the selected item and then
	 * just wait for apply() to apply the changes user made (with the
	 * main widget).
	 * @param styleNames styles selected for editing
	 */
	virtual void selected(const QStringList &styleNames) = 0;

	/**
	 * @brief apply changes made to the currently selected style(s)
	 *
	 * When a user has edited a style (s)he can either cancel or apply
	 * changes. On cancel and ok button clicks no action is needed from
	 * the StyleItem but if a user chooses to press button apply StyleItem's
	 * function apply() is called and StyleItem must upgrade the style
	 * and apply it where ever that style is used in the document.
	 */
	virtual void apply() = 0;

	/**
	 * @brief User has requested to delete all the selected styles
	 */
	virtual void deleteSelected() = 0;

	/** @brief Called when the currently selected style's name has changed */
	virtual void nameChanged(const QString &newName) = 0;

private:
	/* hide these two, StyleItem is not ment to be copied */
	StyleItem(const StyleItem&) : QObject() {};
	void operator=(const StyleItem&) {};

};

#endif
