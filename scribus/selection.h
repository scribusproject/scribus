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

#include <QList>
#include <QMap>
#include <QObject>
#include <QPointer>

#include "pageitem.h"
#include "scribusapi.h"

typedef QList< QPointer<PageItem> > SelectionList;

class SCRIBUS_API Selection : public QObject
{
	Q_OBJECT
	public:
		/**
		 * \brief Create an empty selection that is not a GUI selection
		 * @param  parent QObject
		 */
		explicit Selection(QObject* parent);   // otherwise implicit conversion Selection* -> Selection& is possible
		/**
		 * \brief Create an empty selection that may be a GUI selection
		 * @param  parent QObject
		 * @param  guiSelection If the selection is to be a GUI selection
		 */
		Selection(QObject* parent, bool guiSelection);
		/**
		 * \brief Copy a selection
		 * \note We are leaving the connections of the items in place
		 * and the isGUISelection set in the copy. We cannot disconnect them
		 * as they may be connected via the main GUI selection.
		 * @param  other selection
		 */
		Selection(const Selection& other);
		Selection& operator=( const Selection & );
		~Selection();

		/**
		 * \brief Copy the selection of items from one selection to another
		 */
		void copy(Selection& other, bool emptyOther);
		
		bool connectItemToGUI();
		/**
		 * \brief Disconnect all items from the GUI slots. 
		 * This should not really be necessary if all things are going ok
		 * except for within the clearAll function.
		 * @return bool true on success
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
		 * @param doEmit call emitAllToGUI()
		 * @return If the item was added
		 */
		bool prependItem(PageItem *item, bool doEmit=true);

		bool containsItem(PageItem *item) const { return m_SelList.contains(item); }
		/**
		 * \brief Unused
		 */
		bool addGroup();
		/**
		 * \brief Remove an item from list
		 * @param item page item
		 */
		bool removeItem(PageItem *item);
		/**
		 * \brief Remove the first item from the list
		 * @return If the remove was successful
		 */
		bool removeFirst();
		/**
		 * \brief Unused
		 */
		bool removeGroup();
		/**
		 * \brief Remove an item from list listNumber and return a pointer to it
		 * @param itemIndex Index of the item in the list
		 * @return Item
		 */
		PageItem* takeItem(int itemIndex);
		/**
		 * \brief Find an item from the selection and return an index to it
		 * @param item Item pointer to find in the list
		 * @return Item
		 */
		int findItem(PageItem *item) const { return m_SelList.indexOf(item); }
		/**
		 * \brief Return the count of the selection
		 */
		int count() const { return m_SelList.count(); }
		/**
		 * \brief Check if the selection is empty.
		 */
		bool isEmpty() const { return m_SelList.count()==0; }
		/**
		 * \brief Clear a list
		 */
		bool clear();
		/**
		 * \brief See if the first selected item is "me", usually called from an item object with "this".
		 * @param item PageItem reference
		 */
		bool primarySelectionIs(const PageItem* item) const { return (!m_SelList.isEmpty() && (item==m_SelList.first())); }
		/**
		 * \brief Return item at specified index in the selection
		 * @param index index in selection
		 * @return Item
		 */
		PageItem *itemAt(int index=0) { return itemAt_(index); }
		/**
		 * \brief Return item at specified index in the selection
		 * @param index index in selection
		 * @return const Item
		 */
		const PageItem *itemAt(int index=0) const { return const_cast<Selection*>(this)->itemAt_(index); }
		/**
		 * \brief Get name of selected items
		 */
		QStringList getSelectedItemsByName() const;
		/**
		 * \brief Return range of selected items. If selection is empty, the lowest index returned
		 *        will be strictly superior to the highest
		 * @param lowest index of the lowest item number  in the selection
		 * @param highest indest of the higest item number in the selection
		 */
		void getItemRange(int& lowest, int & highest);

		bool isMultipleSelection() const { return m_hasGroupSelection; }
		bool isGUISelection() const { return m_isGUISelection; }
		double width() const;
		double height() const;
		//set the group rectangle properties
		void setGroupRect();
		void getGroupRect(double *x, double *y, double *w, double *h);
		void getVisualGroupRect(double *x, double *y, double *w, double *h);
		//!\brief Test to see if all items in the selection are the same typedef
		bool itemsAreSameType() const;

		/**
		 * \brief get the layer ID of items in the selection
		 * @return the layer ID or -1 if items do not belong to the same layer
		 */
		int objectsLayer(void) const;

		bool signalsDelayed(void);
		void delaySignalsOn(void);
		void delaySignalsOff(void);
		
	protected:
		PageItem *itemAt_(int index=0);
		SelectionList m_SelList;
		bool m_hasGroupSelection;
		bool m_isGUISelection;
		double groupX;
		double groupY;
		double groupW;
		double groupH;
		
		double visualGX;
		double visualGY;
		double visualGW;
		double visualGH;

		int  m_delaySignals;
		bool m_sigSelectionChanged;
		bool m_sigSelectionIsMultiple;

		void addItemInternal(PageItem* item);
		void prependItemInternal(PageItem* item);
		void addGroupItem(PageItem* item);
		void sendSignals(bool guiConnect = true);
		
	signals:
		void selectionIsMultiple(bool);
		void empty();
		void selectionChanged();
};

#endif
