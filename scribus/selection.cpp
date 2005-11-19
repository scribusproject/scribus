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



Selection::Selection()
{
	sellists.insert(0, QValueList< QGuardedPtr<PageItem> >()); //Create the default GUI selected item list
	hasGroupSelection.insert(0, false);
}


Selection::~Selection()
{
}

void Selection::setDoc(ScribusDoc* doc)
{
	m_Doc=doc;
}


int Selection::count(int listNumber)
{
	if (sellists.contains(listNumber))
		return sellists[listNumber].count();
	return -1;
}

bool Selection::clear(int listNumber)
{
	if (sellists.contains(listNumber))
	{
		if (!sellists[listNumber].isEmpty())
		{
			SelectionList::Iterator itend=sellists[listNumber].end();
			SelectionList::Iterator it=sellists[listNumber].begin();
			while (it!=itend)
			{
				(*it)->Select=false;
				(*it)->isSingleSel=false;
				(*it)->disconnectFromGUI();
				++it;
			}
		}
		sellists[listNumber].clear();
		hasGroupSelection[listNumber]=false;
		qDebug(QString("clearing %1").arg(listNumber));
		return true;
	}
	return false;
}

void Selection::clearAll()
{
	disconnectAllItemsFromGUI();
	ListOfSelections::Iterator itend=sellists.end();
	ListOfSelections::Iterator it=sellists.begin();
	while (it!=itend)
	{
		hasGroupSelection[it.key()]=false;
		clear(it.key());
		++it;
	}
}

bool Selection::connectItemToGUI(int listNumber)
{
	if (sellists.contains(listNumber) && !sellists[listNumber].isEmpty())
		if (hasGroupSelection[listNumber]==false)
		{
			QGuardedPtr<PageItem> pi=sellists[listNumber].first();
			//Quick check to see if the pointer is NULL, if its NULL, we should remove it from the list now
			if (pi.isNull())
			{
				//SelectionList::Iterator it=sellists[listNumber].at(0);
				sellists[listNumber].remove(pi);
				return false;
			}
			return pi->connectToGUI();
		}
	return false;
}

bool Selection::disconnectAllItemsFromGUI()
{
	ListOfSelections::Iterator itend=sellists.end();
	ListOfSelections::Iterator it=sellists.begin();
	while (it!=itend)
	{
		int listNumber=it.key();
		if (!sellists[listNumber].isEmpty())
		{
			SelectionList::Iterator it2end=sellists[listNumber].end();
			SelectionList::Iterator it2=sellists[listNumber].begin();
			while (it2!=it2end)
			{
				(*it2)->disconnectFromGUI();
				++it2;
			}
		}
		++it;
	}
	//Return true for now... 
	return true;
}

bool Selection::addItem(PageItem *item, int listNumber)
{
	if (item==NULL)
		return false;
	if (sellists.contains(listNumber))
	{
		bool listIsEmpty=sellists[listNumber].isEmpty();
		if (listIsEmpty || !sellists[listNumber].contains(item))
		{
			if (listIsEmpty && listNumber==0)
				item->connectToGUI();
			sellists[listNumber].append(item);
			item->Select=true;
			qDebug(QString("adding %1 %2").arg(listNumber).arg(item->ItemNr));
			return true;
		}
	}
	return false;
}

bool Selection::removeItem(PageItem *item, int listNumber)
{
	if (sellists.contains(listNumber))
		if (!sellists[listNumber].isEmpty() && sellists[listNumber].contains(item))
		{
			bool removeOk=sellists[listNumber].remove(item);
			if (removeOk)
			{
				item->Select=false;
				item->isSingleSel = false;
			}
			if (sellists[listNumber].count()==0)
				hasGroupSelection[listNumber]=false;
			return removeOk;
		}
	return false;
}

const bool Selection::primarySelectionIsMyself(const PageItem* item, int listNumber)
{
	if (sellists.contains(listNumber))
		if (!sellists[listNumber].isEmpty() && (item==sellists[listNumber].first()))
			return true;
	return false;
}

PageItem *Selection::itemAt(int index, int listNumber)
{
	if (sellists.contains(listNumber))
		if (!sellists[listNumber].isEmpty() && static_cast<uint>(index)<sellists[listNumber].count())
		{
			QGuardedPtr<PageItem> pi=sellists[listNumber][index];
			if (!pi.isNull())
				return pi;
			//If its NULL, we should remove it from the list now
			SelectionList::Iterator it=sellists[listNumber].at(index);
			sellists[listNumber].remove(it);
			return NULL;
		}
	return NULL;
}
