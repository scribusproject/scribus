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

#include "scribusdoc.h"
#include "selection.h"

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

void Selection::copy(Selection& other, bool copyGUISelection, bool emptyOther)
{
	if (&other==this)
		return;
	m_SelList=other.m_SelList;
	m_hasGroupSelection=other.m_hasGroupSelection;
	if (copyGUISelection)
		m_isGUISelection=other.m_isGUISelection;
	if (m_isGUISelection && !m_SelList.isEmpty())
	{
		m_SelList[0]->connectToGUI();
		m_SelList[0]->emitAllToGUI();
		m_SelList[0]->setSelected(true);
		emit selectionIsMultiple(m_hasGroupSelection);
	}
	if (emptyOther)
		other.clear();
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
// 	if (m_isGUISelection)
// 		emit empty();
	if (m_isGUISelection)
		emit selectionChanged();
	return true;
}

bool Selection::connectItemToGUI()
{
	bool ret = false;
	if (!m_isGUISelection || m_SelList.isEmpty())
		return ret;
	if (m_hasGroupSelection==false)
	{
		QPointer<PageItem> pi=m_SelList.first();
		//Quick check to see if the pointer is NULL, if its NULL, we should remove it from the list now
		if (pi.isNull())
		{
			m_SelList.removeAll(pi);
			return ret;
		}
		ret = pi->connectToGUI();
		pi->emitAllToGUI();
		emit selectionChanged();
	}
	else
	{
		ret = m_SelList.first()->connectToGUI();
		m_SelList.first()->emitAllToGUI();
		emit selectionChanged();
		emit selectionIsMultiple(m_hasGroupSelection);
	}
	return ret;
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
		item->setSelected(true);
		if (m_isGUISelection && !ignoreGUI)
		{
			emit selectionChanged();
		}

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

bool Selection::prependItem(PageItem *item, bool doEmit)
{
	if (item==NULL)
		return false;
	if (!m_SelList.contains(item))
	{
		if (m_isGUISelection && !m_SelList.isEmpty())
			m_SelList[0]->disconnectFromGUI();
		m_SelList.prepend(item);
		item->setSelected(true);
		if (m_isGUISelection)
		{
			emit selectionChanged();
		}

		m_hasGroupSelection=(m_SelList.count()>1);
		if (m_isGUISelection)
		{
			item->connectToGUI();
			if (doEmit)
				item->emitAllToGUI();
			emit selectionIsMultiple(m_hasGroupSelection);
		}
		return true;
	}
	return false;
}

PageItem *Selection::itemAt_(int index)
{
	if (!m_SelList.isEmpty() && index<m_SelList.count())
	{
		QPointer<PageItem> pi=m_SelList[index];
		//If not NULL return it, otherwise remove from the list and return NULL
		if (!pi.isNull())
			return pi;
//		SelectionList::Iterator it=m_SelList.at(index);
		m_SelList.removeAt(index);
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
		{
			m_SelList.first()->connectToGUI();
			emit selectionChanged();
		}
	}
	return false;
}

bool Selection::removeItem(PageItem *item)
{
	if (!m_SelList.isEmpty() && m_SelList.contains(item))
	{
		bool removeOk=(m_SelList.removeAll(item)==1);
		if (removeOk)
		{
			item->setSelected(false);
			item->isSingleSel = false;
		}
		if (m_SelList.isEmpty())
		{
			m_hasGroupSelection=false;
// 			if (m_isGUISelection)
// 				emit empty();
		}
		else if (m_isGUISelection)
		{
			m_SelList.first()->connectToGUI();
			emit selectionChanged();
		}
		return removeOk;
	}
	return false;
}

PageItem* Selection::takeItem(int itemIndex)
{
	if (!m_SelList.isEmpty() && itemIndex<m_SelList.count())
	{
		PageItem *item=m_SelList[itemIndex];
		bool removeOk=(m_SelList.removeAll(item)==1);
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
			if (m_SelList.isEmpty())
			{
				m_hasGroupSelection=false;
// 				if (m_isGUISelection)
// 					emit empty();
			}
			if (m_isGUISelection)
				emit selectionChanged();
			return item;
		}
	}
	// JG Should not be needed here
	/*if (m_isGUISelection)
		emit selectionChanged();*/
	return NULL;
}

QStringList Selection::getSelectedItemsByName() const
{
	QStringList names;
	SelectionList::ConstIterator it=m_SelList.begin();
	SelectionList::ConstIterator itend=m_SelList.end();
	for ( ; it!=itend ; ++it)
		names.append((*it)->itemName());
	return names;
}

double Selection::width() const
{
	if (m_SelList.isEmpty())
		return 0.0;
	double minX=9999999.9;
	double maxX=-9999999.9;
	SelectionList::ConstIterator it=m_SelList.begin();
	SelectionList::ConstIterator itend=m_SelList.end();
	double x1=0.0,x2=0.0,y1=0.0,y2=0.0;
	for ( ; it!=itend ; ++it)
	{
		(*it)->getBoundingRect(&x1, &y1, &x2, &y2);
		if (x1<minX)
			minX=x1;
		if (x2>maxX)
			maxX=x2;
	}
	return maxX-minX;
}

double Selection::height() const
{
	if (m_SelList.isEmpty())
		return 0.0;
	double minY=9999999.9;
	double maxY=-9999999.9;
	SelectionList::ConstIterator it=m_SelList.begin();
	SelectionList::ConstIterator itend=m_SelList.end();
	double x1=0.0,x2=0.0,y1=0.0,y2=0.0;
	for ( ; it!=itend ; ++it)
	{
		(*it)->getBoundingRect(&x1, &y1, &x2, &y2);
		if (y1<minY)
			minY=y1;
		if (y2>maxY)
			maxY=y2;
	}
	return maxY-minY;
}

void Selection::setGroupRect()
{
	PageItem *currItem;
	double minx = 99999.9;
	double miny = 99999.9;
	double maxx = -99999.9;
	double maxy = -99999.9;
	uint selectedItemCount=count();
	for (uint gc = 0; gc < selectedItemCount; ++gc)
	{
		currItem = itemAt(gc);
		if (currItem->rotation() != 0)
		{
			FPointArray pb(4);
			pb.setPoint(0, FPoint(currItem->xPos(), currItem->yPos()));
			pb.setPoint(1, FPoint(currItem->width(), 0.0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0));
			pb.setPoint(2, FPoint(currItem->width(), currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0));
			pb.setPoint(3, FPoint(0.0, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0));
			for (uint pc = 0; pc < 4; ++pc)
			{
				minx = qMin(minx, pb.point(pc).x());
				miny = qMin(miny, pb.point(pc).y());
				maxx = qMax(maxx, pb.point(pc).x());
				maxy = qMax(maxy, pb.point(pc).y());
			}
		}
		else
		{
			minx = qMin(minx, currItem->xPos());
			miny = qMin(miny, currItem->yPos());
			maxx = qMax(maxx, currItem->xPos() + currItem->width());
			maxy = qMax(maxy, currItem->yPos() + currItem->height());
		}
	}
	groupX = minx;
	groupY = miny;
	groupW = maxx - minx;
	groupH = maxy - miny;
}

void Selection::getGroupRect(double *x, double *y, double *w, double *h)
{
	*x = groupX;
	*y = groupY;
	*w = groupW;
	*h = groupH;
}

bool Selection::itemsAreSameType() const
{
	//CB Putting count=1 before isempty test as its probably the most likely, given our view code.
	if (m_SelList.count()==1)
		return true;
	if (m_SelList.isEmpty())
		return false;
	SelectionList::ConstIterator it=m_SelList.begin();
	SelectionList::ConstIterator itend=m_SelList.end();
	PageItem::ItemType itemType = (*it)->itemType();
	for ( ; it!=itend ; ++it)
	{
		if ((*it)->isGroupControl)		// ignore GroupControl items
			continue;
		if ((*it)->itemType() != itemType)
			return false;
	}
	return true;
}
