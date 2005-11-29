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

typedef enum {
	GUISelection=0,
	NonGUISelection=1,
	Delete=2,
	CustomStart=10,
	TempStart=100
} SelectionListType;

typedef QValueList< QGuardedPtr<PageItem> > SelectionList;
typedef QMap<int, SelectionList > ListOfSelections;
typedef QMap<int, bool> ListBool;

class SCRIBUS_API Selection : public QObject
{
	Q_OBJECT
	public:
		Selection();
		~Selection();
		
		void setDoc(ScribusDoc*);
		
		QPtrList<PageItem>* requestList();
		int backupToTempList(int listNumber);
		bool restoreFromTempList(int listNumber, int fromTempListNumber);
		
		
		bool connectItemToGUI(int listNumber);
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
		bool addItem(PageItem *item, int listNumber=GUISelection);
		/**
		 * @brief Prepend an item to the selection. 
		 * If its added to the GUISelection listNumber and its item 0, its connected to the GUI too
		 * @param item Item to add
		 * @param listNumber List to add the item to
		 * @return If the item was added
		 */
		bool prependItem(PageItem *item, int listNumber=GUISelection);
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
		bool removeItem(PageItem *item, int listNumber=GUISelection);
		/**
		 * Remove the first item from the list
		 * @param listNumber List to remove the item
		 * @return If the remove was successful
		 */
		bool removeFirst(int listNumber=GUISelection);
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
		PageItem* takeItem(uint itemIndex, int listNumber=GUISelection);
		/**
		 * Find an item from list listNumber and return an index to it
		 * @param item Item pointer to find in the list
		 * @param listNumber List to find th item in
		 * @return Item
		 */
		const int findItem(PageItem *item, int listNumber=GUISelection);
		/**
		 * Return the count of a list
		 * @param listNumber List to count
		 * @return 
		 */
		uint count(int listNumber=GUISelection);
		/**
		 * Check if the list specified is empty.
		 * @param listNumber List to check empty state
		 * @return
		 */
		bool isEmpty(int listNumber=GUISelection);
		/**
		 * Clear a list
		 * @param listNumber List to clear
		 * @return 
		 */
		bool clear(int listNumber=GUISelection);
		void clearAll();
		/**
		 * See if the first selected item is "me", usually called from an item object with "this".
		 * @param item 
		 * @return 
		 */
		const bool primarySelectionIsMyself(const PageItem* item, int listNumber=GUISelection);
		PageItem *itemAt(int index=0, int listNumber=GUISelection);
		QStringList getSelectedItemsByName(int listNumber=GUISelection);
		
	protected:
		ScribusDoc* m_Doc;
		ListOfSelections sellists;
		ListBool hasGroupSelection;
		int nextTemp;
};

#endif
