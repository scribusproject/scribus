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

typedef enum {
	GUISelection=0,
	NonGUISelection=1,
	Delete=2,
	CustomStart=10
} SelectionListType;

typedef QValueList< QGuardedPtr<PageItem> > SelectionList;
typedef QMap<int, SelectionList > ListOfSelections;
typedef QMap<int, bool> ListBool;

class Selection : public QObject
{
	Q_OBJECT
	public:
		Selection();
		~Selection();
		
		void setDoc(ScribusDoc*);
		
		QPtrList<PageItem>* requestList();
		bool connectItemToGUI(int listNumber);
		/**
		 * Disconnect all items from the GUI slots. 
		 * This should not really be necessary if all things are going ok
		 * except for within the clearAll function.
		 * @return 
		 */
		bool disconnectAllItemsFromGUI();
		bool addItem(PageItem *item, int listNumber=GUISelection);
		bool addGroup();
		bool removeItem(PageItem *item, int listNumber=GUISelection);
		bool removeGroup();
		int count(int listNumber=GUISelection);
		bool clear(int listNumber=GUISelection);
		void clearAll();
		/**
		 * See if the first selected item is "me", called from an item object with "this".
		 * @param item 
		 * @return 
		 */
		const bool primarySelectionIsMyself(const PageItem* item, int listNumber);
		PageItem *itemAt(int index=0, int listNumber=GUISelection);
		
	protected:
		ScribusDoc* m_Doc;
		ListOfSelections sellists;
		ListBool hasGroupSelection;
};

#endif
