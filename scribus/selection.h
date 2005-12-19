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
		Selection(QObject*);
		Selection(QObject*, bool);
		Selection(const Selection&);
		Selection& operator=( const Selection &a );
		~Selection();
		
		void setDoc(ScribusDoc*);
		
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
		 * If its added to the GUISelection listNumber and its item 0, its connected to the GUI too
		 * @param item Item to add
		 * @param listNumber List to add the item to
		 * @return If the item was added
		 */
		bool addItem(PageItem *item);
		/**
		 * @brief Prepend an item to the selection. 
		 * If its added to the GUISelection listNumber and its item 0, its connected to the GUI too
		 * @param item Item to add
		 * @param listNumber List to add the item to
		 * @return If the item was added
		 */
		bool prependItem(PageItem *item);
		/**
		 * Unused
		 * @return 
		 */
		bool addGroup();
		/**
		 * Remove an item from list listNumber
		 * @param item 
		 * @param listNumber List to remove the item from 
		 * @return 
		 */
		bool removeItem(PageItem *item);
		/**
		 * Remove the first item from the list
		 * @param listNumber List to remove the item
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
		 * @param listNumber List to remove the item from 
		 * @return Item
		 */
		PageItem* takeItem(uint itemIndex);
		/**
		 * Find an item from list listNumber and return an index to it
		 * @param item Item pointer to find in the list
		 * @param listNumber List to find th item in
		 * @return Item
		 */
		int findItem(PageItem *item) const { return m_SelList.findIndex(item); }
		/**
		 * Return the count of a list
		 * @param listNumber List to count
		 * @return 
		 */
		uint count() const { return m_SelList.count(); }
		/**
		 * Check if the list specified is empty.
		 * @param listNumber List to check empty state
		 * @return
		 */
		bool isEmpty() const { return (m_SelList.count()==0); }
		/**
		 * Clear a list
		 * @param listNumber List to clear
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
