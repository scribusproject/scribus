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
	//sellists.insert(0, QValueList< QGuardedPtr<PageItem> >()); //Create the default GUI selected item list
	//hasGroupSelection.insert(0, false);
	hasGroupSelection=false;
	//nextTemp=TempStart;
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

const int Selection::findItem(PageItem *item)//, int listNumber)
{
	//if (sellists.contains(listNumber))
		//return sellists[listNumber].findIndex(item);
		return sellists.findIndex(item);
	return -1;
}

uint Selection::count()//(int listNumber)
{
	//if (sellists.contains(listNumber))
		//return sellists[listNumber].count();
		return sellists.count();
	//return 0;
}

bool Selection::isEmpty()//(int listNumber)
{
	//if (sellists.contains(listNumber))
		//return (sellists[listNumber].count()==0);
		return (sellists.count()==0);
	//return false;
}

bool Selection::clear()//(int listNumber)
{
	//if (sellists.contains(listNumber))
	//{
		//if (!sellists[listNumber].isEmpty())
		if (!sellists.isEmpty())
		{
			//SelectionList::Iterator itend=sellists[listNumber].end();
			SelectionList::Iterator itend=sellists.end();
			//SelectionList::Iterator it=sellists[listNumber].begin();
			SelectionList::Iterator it=sellists.begin();
			while (it!=itend)
			{
				(*it)->setSelected(false);
				(*it)->isSingleSel=false;
				(*it)->disconnectFromGUI();
				++it;
			}
		}
		//sellists[listNumber].clear();
		sellists.clear();
		//hasGroupSelection[listNumber]=false;
		hasGroupSelection=false;
		//qDebug(QString("clearing %1").arg(listNumber));
		return true;
	//}
	return false;
}

/*
void Selection::clearAll()
{
	disconnectAllItemsFromGUI();
	ListOfSelections::Iterator itend=sellists.end();
	ListOfSelections::Iterator it=sellists.begin();
	while (it!=itend)
	{
		clear(it.key());
		++it;
	}
}
*/

bool Selection::connectItemToGUI()//(int listNumber)
{
	//if (sellists.contains(listNumber) && !sellists[listNumber].isEmpty())
	if (!sellists.isEmpty())
		//if (hasGroupSelection[listNumber]==false)
		if (hasGroupSelection==false)
		{
			//QGuardedPtr<PageItem> pi=sellists[listNumber].first();
			QGuardedPtr<PageItem> pi=sellists.first();
			//Quick check to see if the pointer is NULL, if its NULL, we should remove it from the list now
			if (pi.isNull())
			{
				//sellists[listNumber].remove(pi);
				sellists.remove(pi);
				return false;
			}
			return pi->connectToGUI();
		}
	return false;
}

bool Selection::disconnectAllItemsFromGUI()
{
	//ListOfSelections::Iterator itend=sellists.end();
	//ListOfSelections::Iterator it=sellists.begin();
	//while (it!=itend)
	//{
		//int listNumber=it.key();
		//if (!sellists[listNumber].isEmpty())
		if (!sellists.isEmpty())
		{
			//SelectionList::Iterator it2end=sellists[listNumber].end();
			SelectionList::Iterator it2end=sellists.end();
			//SelectionList::Iterator it2=sellists[listNumber].begin();
			SelectionList::Iterator it2=sellists.begin();
			while (it2!=it2end)
			{
				(*it2)->disconnectFromGUI();
				++it2;
			}
		}
		//++it;
	//}
	//Return true for now... 
	return true;
}

bool Selection::addItem(PageItem *item)//, int listNumber)
{
	if (item==NULL)
		return false;
	//if (sellists.contains(listNumber))
	{
		//bool listIsEmpty=sellists[listNumber].isEmpty();
		bool listIsEmpty=sellists.isEmpty();
		//if (listIsEmpty || !sellists[listNumber].contains(item))
		if (listIsEmpty || !sellists.contains(item))
		{
			//sellists[listNumber].append(item);
			sellists.append(item);
			//if (listIsEmpty && listNumber==0)
			if (listIsEmpty)
			{
				item->connectToGUI();
				item->emitAllToGUI();
			}
			item->setSelected(true);
			//hasGroupSelection[listNumber]=(sellists[listNumber].count()>1);
			hasGroupSelection=(sellists.count()>1);
			//emit selectionIsMultiple(listNumber, hasGroupSelection[listNumber]);
			emit selectionIsMultiple(hasGroupSelection);
			return true;
		}
	}
	return false;
}

bool Selection::prependItem(PageItem *item)//, int listNumber)
{
	if (item==NULL)
		return false;
	//if (sellists.contains(listNumber))
	{
		//if (!sellists[listNumber].contains(item))
		if (!sellists.contains(item))
		{
			//if (listNumber==0)
			//{
				//if (!sellists[listNumber].isEmpty())
				if (!sellists.isEmpty())
					//sellists[listNumber][0]->disconnectFromGUI();
					sellists[0]->disconnectFromGUI();
				item->connectToGUI();
				item->emitAllToGUI();
			//}
			//sellists[listNumber].prepend(item);
			sellists.prepend(item);
			item->setSelected(true);
			//hasGroupSelection[listNumber]=(sellists[listNumber].count()>1);
			hasGroupSelection=(sellists.count()>1);
			//emit selectionIsMultiple(listNumber, hasGroupSelection[listNumber]);
			emit selectionIsMultiple(hasGroupSelection);
			return true;
		}
	}
	return false;
}

const bool Selection::primarySelectionIs(const PageItem* item)//, int listNumber)
{
	//if (sellists.contains(listNumber))
		//if (!sellists[listNumber].isEmpty() && (item==sellists[listNumber].first())) 
		if (!sellists.isEmpty() && (item==sellists.first()))
			return true;
	return false;
}

PageItem *Selection::itemAt(int index)//, int listNumber)
{
	//if (sellists.contains(listNumber))
		//if (!sellists[listNumber].isEmpty() && static_cast<uint>(index)<sellists[listNumber].count())
		if (!sellists.isEmpty() && static_cast<uint>(index)<sellists.count())
		{
			//QGuardedPtr<PageItem> pi=sellists[listNumber][index];
			QGuardedPtr<PageItem> pi=sellists[index];
			if (!pi.isNull())
				return pi;
			//If its NULL, we should remove it from the list now
			//SelectionList::Iterator it=sellists[listNumber].at(index);
			SelectionList::Iterator it=sellists.at(index);
			//sellists[listNumber].remove(it);
			sellists.remove(it);
			return NULL;
		}
	return NULL;
}

bool Selection::removeFirst()//(int listNumber)
{
	//if (sellists.contains(listNumber) && !sellists[listNumber].isEmpty())
	if (!sellists.isEmpty())
	{
		//qDebug(QString("%1").arg("removing first"));
		//removeItem(sellists[listNumber].first(), listNumber);
		removeItem(sellists.first());
		//if (sellists[listNumber].isEmpty() || listNumber!=0)
		if (sellists.isEmpty())
			return true;
		//sellists[listNumber].first()->connectToGUI();
		sellists.first()->connectToGUI();
	}
	return false;
}

bool Selection::removeItem(PageItem *item)//, int listNumber)
{
	//if (sellists.contains(listNumber))
		//if (!sellists[listNumber].isEmpty() && sellists[listNumber].contains(item))
		if (!sellists.isEmpty() && sellists.contains(item))
		{
			//qDebug(QString("removing item %1").arg(item->ItemNr));
			//bool removeOk=sellists[listNumber].remove(item);
			bool removeOk=sellists.remove(item);
			if (removeOk)
			{
				item->setSelected(false);
				item->isSingleSel = false;
			}
			//if (sellists[listNumber].count()==0)
			if (sellists.count()==0)
				//hasGroupSelection[listNumber]=false;
				hasGroupSelection=false;
			else
				//sellists[listNumber].first()->connectToGUI();
				sellists.first()->connectToGUI();
			return removeOk;
		}
	return false;
}

PageItem* Selection::takeItem(uint itemIndex)//, int listNumber)
{
	//if ((itemIndex>=0) && 
	//if (sellists.contains(listNumber))
		//if (!sellists[listNumber].isEmpty() && itemIndex<sellists[listNumber].count())
		if (!sellists.isEmpty() && itemIndex<sellists.count())
		{
			//PageItem *item=sellists[listNumber][itemIndex];
			PageItem *item=sellists[itemIndex];
			//bool removeOk=sellists[listNumber].remove(item);
			bool removeOk=sellists.remove(item);
			if (removeOk)
			{
				item->setSelected(false);
				item->isSingleSel = false;
				//if (listNumber==0 && itemIndex==0)
				if (itemIndex==0)
				{
					item->disconnectFromGUI();
					//if (sellists[listNumber].count()>0)
					if (sellists.count()>0)
						//sellists[listNumber][0]->connectToGUI();
						sellists[0]->connectToGUI();
				}
				//if (sellists[listNumber].count()==0)
				if (sellists.count()==0)
					//hasGroupSelection[listNumber]=false;
					hasGroupSelection=false;
				return item;
			}
		}
	return NULL;
}
/*
int Selection::backupToTempList()//(int listNumber)
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
*/
QStringList Selection::getSelectedItemsByName()//(int listNumber)
{
	//if (!sellists.contains(listNumber))
	//	return QStringList();
	QStringList names;
	//SelectionList::Iterator it=sellists[listNumber].begin();
	SelectionList::Iterator it=sellists.begin();
	//SelectionList::Iterator itend=sellists[listNumber].end();
	SelectionList::Iterator itend=sellists.end();
	for ( ; it!=itend ; ++it)
		names.append((*it)->itemName());
	return names;
}

bool Selection::isMultipleSelection()//(int listNumber)
{
	//if (hasGroupSelection.contains(listNumber))
		//return hasGroupSelection[listNumber];
		return hasGroupSelection;
	//return false;
}


