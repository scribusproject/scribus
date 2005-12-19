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
#include "selection.h"
#include "selection.moc"



Selection::Selection(QObject* parent) : QObject(parent)
{
	hasGroupSelection=false;
}

Selection::Selection(const Selection& other) :
	QObject(other.parent()),
	m_Doc(other.m_Doc),
	sellists(other.sellists),
	hasGroupSelection(other.hasGroupSelection)
{
	
}

Selection& Selection::operator=( const Selection &other )
{
	if (&other==this)
		return *this;
	m_Doc=other.m_Doc;
	sellists=other.sellists;
	hasGroupSelection=other.hasGroupSelection;
	return *this;
}

Selection::~Selection()
{
}

void Selection::setDoc(ScribusDoc* doc)
{
	m_Doc=doc;
}

const int Selection::findItem(PageItem *item)
{
	return sellists.findIndex(item);
}

uint Selection::count()
{
	return sellists.count();
}

bool Selection::isEmpty()
{
	return (sellists.count()==0);
}

bool Selection::clear()
{
		if (!sellists.isEmpty())
		{
			SelectionList::Iterator itend=sellists.end();
			SelectionList::Iterator it=sellists.begin();
			while (it!=itend)
			{
				(*it)->setSelected(false);
				(*it)->isSingleSel=false;
				(*it)->disconnectFromGUI();
				++it;
			}
		}
		sellists.clear();
		hasGroupSelection=false;
		return true;
}

bool Selection::connectItemToGUI()
{
	if (sellists.isEmpty())
		return false;
	if (hasGroupSelection==false)
	{
		QGuardedPtr<PageItem> pi=sellists.first();
		//Quick check to see if the pointer is NULL, if its NULL, we should remove it from the list now
		if (pi.isNull())
		{
			sellists.remove(pi);
			return false;
		}
		return pi->connectToGUI();
	}
	return false;
}

bool Selection::disconnectAllItemsFromGUI()
{
	if (!sellists.isEmpty())
	{
		SelectionList::Iterator it2end=sellists.end();
		SelectionList::Iterator it2=sellists.begin();
		while (it2!=it2end)
		{
			(*it2)->disconnectFromGUI();
			++it2;
		}
	}
	return true;
}

bool Selection::addItem(PageItem *item)
{
	if (item==NULL)
		return false;
	bool listIsEmpty=sellists.isEmpty();
	if (listIsEmpty || !sellists.contains(item))
	{
		sellists.append(item);
		if (listIsEmpty)
		{
			item->connectToGUI();
			item->emitAllToGUI();
		}
		item->setSelected(true);
		hasGroupSelection=(sellists.count()>1);
		emit selectionIsMultiple(hasGroupSelection);
		return true;
	}
	return false;
}

bool Selection::prependItem(PageItem *item)
{
	if (item==NULL)
		return false;
	if (!sellists.contains(item))
	{
		if (!sellists.isEmpty())
			sellists[0]->disconnectFromGUI();
		item->connectToGUI();
		item->emitAllToGUI();
		sellists.prepend(item);
		item->setSelected(true);
		hasGroupSelection=(sellists.count()>1);
		emit selectionIsMultiple(hasGroupSelection);
		return true;
	}
	return false;
}

const bool Selection::primarySelectionIs(const PageItem* item)
{
	if (!sellists.isEmpty() && (item==sellists.first()))
		return true;
	return false;
}

PageItem *Selection::itemAt(int index)
{
	if (!sellists.isEmpty() && static_cast<uint>(index)<sellists.count())
	{
		QGuardedPtr<PageItem> pi=sellists[index];
		if (!pi.isNull())
			return pi;
		//If its NULL, we should remove it from the list now
		SelectionList::Iterator it=sellists.at(index);
		sellists.remove(it);
		return NULL;
	}
	return NULL;
}

bool Selection::removeFirst()
{
	if (!sellists.isEmpty())
	{
		removeItem(sellists.first());
		if (sellists.isEmpty())
			return true;
		sellists.first()->connectToGUI();
	}
	return false;
}

bool Selection::removeItem(PageItem *item)
{
	if (!sellists.isEmpty() && sellists.contains(item))
	{
		bool removeOk=sellists.remove(item);
		if (removeOk)
		{
			item->setSelected(false);
			item->isSingleSel = false;
		}
		if (sellists.count()==0)
			hasGroupSelection=false;
		else
			sellists.first()->connectToGUI();
		return removeOk;
	}
	return false;
}

PageItem* Selection::takeItem(uint itemIndex)
{
	if (!sellists.isEmpty() && itemIndex<sellists.count())
	{
		PageItem *item=sellists[itemIndex];
		bool removeOk=sellists.remove(item);
		if (removeOk)
		{
			item->setSelected(false);
			item->isSingleSel = false;
			if (itemIndex==0)
			{
				item->disconnectFromGUI();
				if (sellists.count()>0)
					sellists[0]->connectToGUI();
			}
			if (sellists.count()==0)
				hasGroupSelection=false;
			return item;
		}
	}
	return NULL;
}

QStringList Selection::getSelectedItemsByName()
{
	QStringList names;
	SelectionList::Iterator it=sellists.begin();
	SelectionList::Iterator itend=sellists.end();
	for ( ; it!=itend ; ++it)
		names.append((*it)->itemName());
	return names;
}

bool Selection::isMultipleSelection()
{
	return hasGroupSelection;
}

