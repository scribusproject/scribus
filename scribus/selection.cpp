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
	nextTemp=TempStart;
}


Selection::~Selection()
{
}

void Selection::setDoc(ScribusDoc* doc)
{
	m_Doc=doc;
}

const int Selection::findItem(PageItem *item, int listNumber)
{
	if (sellists.contains(listNumber))
		return sellists[listNumber].findIndex(item);
	return -1;
}

uint Selection::count(int listNumber)
{
	if (sellists.contains(listNumber))
		return sellists[listNumber].count();
	return 0;
}

bool Selection::isEmpty(int listNumber)
{
	if (sellists.contains(listNumber))
		return (sellists[listNumber].count()==0);
	return false;
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
				(*it)->setSelected(false);
				(*it)->isSingleSel=false;
				(*it)->disconnectFromGUI();
				++it;
			}
		}
		sellists[listNumber].clear();
		hasGroupSelection[listNumber]=false;
		//qDebug(QString("clearing %1").arg(listNumber));
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
		//qDebug(QString("trying to add %1").arg(item->ItemNr));
		bool listIsEmpty=sellists[listNumber].isEmpty();
		//if (listIsEmpty) qDebug("list is empty");
		//if (sellists[listNumber].contains(item)) qDebug(QString("list already contains %1").arg(item->ItemNr));
		if (listIsEmpty || !sellists[listNumber].contains(item))
		{
			//qDebug(QString("adding %1").arg(item->ItemNr));
			sellists[listNumber].append(item);
			if (listIsEmpty && listNumber==0)
			{
				item->connectToGUI();
				item->emitAllToGUI();
			}
			item->setSelected(true);
			//qDebug(QString("adding %1 %2").arg(listNumber).arg(item->ItemNr));
			return true;
		}
	}
	return false;
}

bool Selection::prependItem(PageItem *item, int listNumber)
{
	if (item==NULL)
		return false;
	if (sellists.contains(listNumber))
	{
		if (!sellists[listNumber].contains(item))
		{
			if (listNumber==0)
			{
				if (!sellists[listNumber].isEmpty())
					sellists[listNumber][0]->disconnectFromGUI();
				item->connectToGUI();
				item->emitAllToGUI();
			}
			sellists[listNumber].prepend(item);
			item->setSelected(true);
			return true;
		}
	}
	return false;
}

const bool Selection::primarySelectionIs(const PageItem* item, int listNumber)
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

bool Selection::removeFirst(int listNumber)
{
	if (sellists.contains(listNumber) && !sellists[listNumber].isEmpty())
	{
		//qDebug(QString("%1").arg("removing first"));
		removeItem(sellists[listNumber].first(), listNumber);
		if (sellists[listNumber].isEmpty() || listNumber!=0)
			return true;
		sellists[listNumber].first()->connectToGUI();
	}
	return false;
}

bool Selection::removeItem(PageItem *item, int listNumber)
{
	if (sellists.contains(listNumber))
		if (!sellists[listNumber].isEmpty() && sellists[listNumber].contains(item))
		{
			//qDebug(QString("removing item %1").arg(item->ItemNr));
			bool removeOk=sellists[listNumber].remove(item);
			if (removeOk)
			{
				item->setSelected(false);
				item->isSingleSel = false;
			}
			if (sellists[listNumber].count()==0)
				hasGroupSelection[listNumber]=false;
			else
				sellists[listNumber].first()->connectToGUI();
			return removeOk;
		}
	return false;
}

PageItem* Selection::takeItem(uint itemIndex, int listNumber)
{
	if (itemIndex>=0 && sellists.contains(listNumber))
		if (!sellists[listNumber].isEmpty() && itemIndex<sellists[listNumber].count())
		{
			PageItem *item=sellists[listNumber][itemIndex];
			bool removeOk=sellists[listNumber].remove(item);
			if (removeOk)
			{
				item->setSelected(false);
				item->isSingleSel = false;
				if (listNumber==0 && itemIndex==0)
				{
					item->disconnectFromGUI();
					if (sellists[listNumber].count()>0)
						sellists[listNumber][0]->connectToGUI();
				}
				if (sellists[listNumber].count()==0)
					hasGroupSelection[listNumber]=false;
				return item;
			}
		}
	return NULL;
}

int Selection::backupToTempList(int listNumber)
{
	if (!sellists.contains(listNumber))
		return -1;
	
	sellists.insert(nextTemp, sellists[listNumber]);
	hasGroupSelection.insert(nextTemp, hasGroupSelection[listNumber]);
	int temp=nextTemp++;
	return temp;
}

bool Selection::restoreFromTempList(int listNumber, int fromTempListNumber)
{
	if (!sellists.contains(listNumber) || !sellists.contains(fromTempListNumber))
		return false;
	sellists[listNumber].clear();
	sellists[listNumber]=sellists[fromTempListNumber];
	sellists[fromTempListNumber].clear();
	sellists.remove(fromTempListNumber);
	hasGroupSelection.remove(fromTempListNumber);
	if (listNumber==0)
	{
		SelectionList::Iterator it=sellists[listNumber].begin();
		SelectionList::Iterator itend=sellists[listNumber].end();
		for ( ; it!=itend ; ++it)
			(*it)->setSelected(true);
	}
	--nextTemp;
	return true;
}

QStringList Selection::getSelectedItemsByName(int listNumber)
{
	if (!sellists.contains(listNumber))
		return QStringList();
	QStringList names;
	SelectionList::Iterator it=sellists[listNumber].begin();
	SelectionList::Iterator itend=sellists[listNumber].end();
	for ( ; it!=itend ; ++it)
		names.append((*it)->itemName());
	return names;
}
