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
#include "selection.h"
#include "selection.moc"

#include "scribusdoc.h"

Selection::Selection(QObject* parent)
	: QObject(parent),
	m_hasGroupSelection(false),
	m_isGUISelection(false)
{
}

Selection::Selection(QObject* parent, bool guiSelection) 
	: QObject(parent),
	m_hasGroupSelection(false),
	m_isGUISelection(guiSelection)
{
}

Selection::Selection(const Selection& other) :
	QObject(other.parent()),
	m_SelList(other.m_SelList),
	m_hasGroupSelection(other.m_hasGroupSelection),
	m_isGUISelection(other.m_isGUISelection)
{
	if (m_isGUISelection && !m_SelList.isEmpty())
	{
		m_SelList[0]->connectToGUI();
		m_SelList[0]->emitAllToGUI();
		m_SelList[0]->setSelected(true);
		emit selectionIsMultiple(m_hasGroupSelection);
	}
}

Selection& Selection::operator=( const Selection &other )
{
	if (&other==this)
		return *this;
	m_SelList=other.m_SelList;
	m_hasGroupSelection=other.m_hasGroupSelection;
	m_isGUISelection=other.m_isGUISelection;
	if (m_isGUISelection && !m_SelList.isEmpty())
	{
		m_SelList[0]->connectToGUI();
		m_SelList[0]->emitAllToGUI();
		m_SelList[0]->setSelected(true);
		emit selectionIsMultiple(m_hasGroupSelection);
	}
	return *this;
}

Selection::~Selection()
{
}

bool Selection::clear()
{
	if (!m_SelList.isEmpty())
	{
		SelectionList::Iterator itend=m_SelList.end();
		SelectionList::Iterator it=m_SelList.begin();
		while (it!=itend)
		{
			(*it)->setSelected(false);
			(*it)->isSingleSel=false;
			if (m_isGUISelection)
				(*it)->disconnectFromGUI();
			++it;
		}
	}
	m_SelList.clear();
	m_hasGroupSelection=false;
	return true;
}

bool Selection::connectItemToGUI()
{
	if (!m_isGUISelection || m_SelList.isEmpty())
		return false;
	if (m_hasGroupSelection==false)
	{
		QGuardedPtr<PageItem> pi=m_SelList.first();
		//Quick check to see if the pointer is NULL, if its NULL, we should remove it from the list now
		if (pi.isNull())
		{
			m_SelList.remove(pi);
			return false;
		}
		return pi->connectToGUI();
	}
	return false;
}

bool Selection::disconnectAllItemsFromGUI()
{
	if (!m_isGUISelection || m_SelList.isEmpty())
		return false;
	SelectionList::Iterator it2end=m_SelList.end();
	SelectionList::Iterator it2=m_SelList.begin();
	while (it2!=it2end)
	{
		(*it2)->disconnectFromGUI();
		++it2;
	}
	return true;
}

bool Selection::addItem(PageItem *item, bool ignoreGUI)
{
	if (item==NULL)
		return false;
	bool listWasEmpty=m_SelList.isEmpty();
	if (listWasEmpty || !m_SelList.contains(item))
	{
		m_SelList.append(item);
		if (m_isGUISelection)
			item->setSelected(true);
		m_hasGroupSelection=(m_SelList.count()>1);
		if (m_isGUISelection && !ignoreGUI)
		{
			if (listWasEmpty)
			{
				item->connectToGUI();
				item->emitAllToGUI();
			}
			emit selectionIsMultiple(m_hasGroupSelection);
		}
		return true;
	}
	return false;
}

bool Selection::prependItem(PageItem *item)
{
	if (item==NULL)
		return false;
	if (!m_SelList.contains(item))
	{
		if (m_isGUISelection && !m_SelList.isEmpty())
			m_SelList[0]->disconnectFromGUI();
		m_SelList.prepend(item);
		if (m_isGUISelection)
			item->setSelected(true);
		m_hasGroupSelection=(m_SelList.count()>1);
		if (m_isGUISelection)
		{
			item->connectToGUI();
			item->emitAllToGUI();
			emit selectionIsMultiple(m_hasGroupSelection);
		}
		return true;
	}
	return false;
}

PageItem *Selection::itemAt(int index)
{
	if (!m_SelList.isEmpty() && static_cast<uint>(index)<m_SelList.count())
	{
		QGuardedPtr<PageItem> pi=m_SelList[index];
		//If not NULL return it, otherwise remove from the list and return NULL
		if (!pi.isNull())
			return pi;
		SelectionList::Iterator it=m_SelList.at(index);
		m_SelList.remove(it);
	}
	return NULL;
}

bool Selection::removeFirst()
{
	if (!m_SelList.isEmpty())
	{
		removeItem(m_SelList.first());
		if (m_SelList.isEmpty())
			return true;
		if (m_isGUISelection)
			m_SelList.first()->connectToGUI();
	}
	return false;
}

bool Selection::removeItem(PageItem *item)
{
	if (!m_SelList.isEmpty() && m_SelList.contains(item))
	{
		bool removeOk=m_SelList.remove(item);
		if (removeOk)
		{
			item->setSelected(false);
			item->isSingleSel = false;
		}
		if (m_SelList.count()==0)
			m_hasGroupSelection=false;
		else if (m_isGUISelection)
			m_SelList.first()->connectToGUI();
		return removeOk;
	}
	return false;
}

PageItem* Selection::takeItem(uint itemIndex)
{
	if (!m_SelList.isEmpty() && itemIndex<m_SelList.count())
	{
		PageItem *item=m_SelList[itemIndex];
		bool removeOk=m_SelList.remove(item);
		if (removeOk)
		{
			item->setSelected(false);
			item->isSingleSel = false;
			if (itemIndex==0)
			{
				if (m_isGUISelection)
				{
					item->disconnectFromGUI();
					if (m_SelList.count()>0)
						m_SelList[0]->connectToGUI();
				}
			}
			if (m_SelList.count()==0)
				m_hasGroupSelection=false;
			return item;
		}
	}
	return NULL;
}

QStringList Selection::getSelectedItemsByName()
{
	QStringList names;
	SelectionList::Iterator it=m_SelList.begin();
	SelectionList::Iterator itend=m_SelList.end();
	for ( ; it!=itend ; ++it)
		names.append((*it)->itemName());
	return names;
}

