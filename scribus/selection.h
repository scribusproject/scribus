/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#ifndef SELECTION_H
#define SELECTION_H

#include <qguardedptr.h>
#include <qobject.h>
#include <qmap.h>
#include <qvaluevector.h>

#include "pageitem.h"
#include "scribusapi.h"

typedef QValueList< QGuardedPtr<PageItem> > SelectionList;

class SCRIBUS_API Selection : public QObject
{
	Q_OBJECT
	public:
		/**
		 * Create an empty selection that is not a GUI selection
		 * @param  parent
		 * @return Selection
		 */
		Selection(QObject*);
		/**
		 * Create an empty selection that may be a GUI selection
		 * @param  parent
		 * @param  If the selection is to be a GUI selection
		 * @return Selection
		 */
		Selection(QObject*, bool);
		/**
		 * Copy a selection
		 * NOTE: We are leaving the connections of the items in place
		 * and the isGUISelection set in the copy. We cannot disconnect them
		 * as they may be connected via the main GUI selection.
		 * @param  Other selection
		 * @return New selection
		 */
		Selection(const Selection&);
		Selection& operator=( const Selection & );
		~Selection();
		
		bool connectItemToGUI();
		/**
		 * Disconnect all items from the GUI slots. 
		 * This should not really be necessary if all things are going ok
		 * except for within the clearAll function.
		 * @return 
		 */
		bool disconnectAllItemsFromGUI();
		/**
		 * @brief Add an item to the selection. 
		 * If its added to a GUI selection selection and its item 0, its connected to the GUI too
		 * @param item Item to add
		 * @param ignoreGUI Dont connect Item To GUI even if this is a GUI selection
		 * @return If the item was added
		 */
		bool addItem(PageItem *item, bool ignoreGUI=false);
		/**
		 * @brief Prepend an item to the selection. 
		 * If its added to a GUI selection selection and its item 0, its connected to the GUI too
		 * @param item Item to add
		 * @return If the item was added
		 */
		bool prependItem(PageItem *item);
		/**
		 * Unused
		 * @return 
		 */
		bool addGroup();
		/**
		 * Remove an item from list
		 * @param item 
		 * @return 
		 */
		bool removeItem(PageItem *item);
		/**
		 * Remove the first item from the list
		 * @return If the remove was successful
		 */
		bool removeFirst();
		/**
		 * Unused
		 * @return 
		 */
		bool removeGroup();
		/**
		 * Remove an item from list listNumber and return a pointer to it
		 * @param itemIndex Index of the item in the list
		 * @return Item
		 */
		PageItem* takeItem(uint itemIndex);
		/**
		 * Find an item from the selection and return an index to it
		 * @param item Item pointer to find in the list
		 * @return Item
		 */
		int findItem(PageItem *item) const { return m_SelList.findIndex(item); }
		/**
		 * Return the count of the selection
		 * @return 
		 */
		uint count() const { return m_SelList.count(); }
		/**
		 * Check if the selection is empty.
		 * @return
		 */
		bool isEmpty() const { return m_SelList.count()==0; }
		/**
		 * Clear a list
		 * @return 
		 */
		bool clear();
		/**
		 * See if the first selected item is "me", usually called from an item object with "this".
		 * @param item 
		 * @return 
		 */
		bool primarySelectionIs(const PageItem* item) const { return (!m_SelList.isEmpty() && (item==m_SelList.first())); }
		PageItem *itemAt(int index=0);
		QStringList getSelectedItemsByName();
		bool isMultipleSelection() const { return m_hasGroupSelection; }
		bool isGUISelection() const { return m_isGUISelection; }
		void setIsGUISelection(bool guiSelection) { m_isGUISelection=guiSelection; }
		
	protected:
		SelectionList m_SelList;
		bool m_hasGroupSelection;
		bool m_isGUISelection;
		
	signals:
		void selectionIsMultiple(bool);
};

#endif
