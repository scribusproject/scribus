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
	m_isGUISelection(false),
	m_delaySignals(0),
	m_sigSelectionChanged(false),
	m_sigSelectionIsMultiple(false)
{
}

Selection::Selection(QObject* parent, bool guiSelection) 
	: QObject(parent),
	m_hasGroupSelection(false),
	m_isGUISelection(guiSelection),
	m_delaySignals(0),
	m_sigSelectionChanged(false),
	m_sigSelectionIsMultiple(false)
{
}

Selection::Selection(const Selection& other) :
	QObject(other.parent()),
	m_SelList(other.m_SelList),
	m_hasGroupSelection(other.m_hasGroupSelection),
	// We do not copy m_isGUISelection as :
	// 1) copy ctor is used for temporary selections
	// 2) having two GUI selections for same doc should really be avoided
	m_isGUISelection(false),
	// We do not copy m_delaySignals as that can potentially
	// cause much trouble balancing delaySignalOff/On right
	m_delaySignals(0), 
	m_sigSelectionChanged(other.m_sigSelectionChanged),
	m_sigSelectionIsMultiple(other.m_sigSelectionIsMultiple)
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
	if (m_isGUISelection)
	{
		SelectionList::Iterator itend = m_SelList.end();
		for (SelectionList::Iterator it = m_SelList.begin(); it != itend; ++it)
			(*it)->setSelected(false);
	}
	m_SelList=other.m_SelList;
	m_hasGroupSelection=other.m_hasGroupSelection;
	// Do not copy m_isGUISelection for consistency with cpy ctor
	/* m_isGUISelection = other.m_isGUISelection; */
	// We do not copy m_delaySignals as that can potentially
	// cause much trouble balancing delaySignalOff/On right
	m_sigSelectionChanged = other.m_sigSelectionChanged;
	m_sigSelectionIsMultiple = other.m_sigSelectionIsMultiple;
	if (m_isGUISelection && !m_SelList.isEmpty())
	{
		m_SelList[0]->connectToGUI();
		m_SelList[0]->emitAllToGUI();
		m_SelList[0]->setSelected(true);
		emit selectionIsMultiple(m_hasGroupSelection);
	}
	return *this;
}

void Selection::copy(Selection& other, bool emptyOther)
{
	if (&other==this)
		return;
	if (m_isGUISelection)
	{
		SelectionList::Iterator itend = m_SelList.end();
		for (SelectionList::Iterator it = m_SelList.begin(); it != itend; ++it)
			(*it)->setSelected(false);
	}
	m_SelList=other.m_SelList;
	m_hasGroupSelection=other.m_hasGroupSelection;
	if (m_isGUISelection && !m_SelList.isEmpty())
		m_sigSelectionIsMultiple = true;
	if (emptyOther)
		other.clear();
	sendSignals();
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
			(*it)->isSingleSel=false;
			if (m_isGUISelection)
			{
				(*it)->setSelected(false);
				(*it)->disconnectFromGUI();
			}
			++it;
		}
	}
	m_SelList.clear();
	m_hasGroupSelection   = false;
	m_sigSelectionChanged = true;
	sendSignals();
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
		m_sigSelectionChanged = true;
	}
	else
	{
		ret = m_SelList.first()->connectToGUI();
		m_SelList.first()->emitAllToGUI();
		m_sigSelectionChanged    = true;
		m_sigSelectionIsMultiple = true;
	}
	sendSignals(false);
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
		if (m_isGUISelection)
		{
			item->setSelected(true);
			m_sigSelectionChanged = true;
			m_sigSelectionIsMultiple = true;
		}
		m_hasGroupSelection = (m_SelList.count()>1);	
		sendSignals();
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
		if (m_isGUISelection /*&& doEmit*/)
		{
			item->setSelected(true);
			m_sigSelectionChanged = true;
			m_sigSelectionIsMultiple = true;
		}
		m_hasGroupSelection = (m_SelList.count()>1);	
		sendSignals();
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
		if (m_isGUISelection && m_SelList.first())
			m_SelList.first()->setSelected(false);
		removeItem(m_SelList.first());
		if (m_SelList.isEmpty())
			return true;
		if (m_isGUISelection)
			m_sigSelectionChanged = true;
		sendSignals();
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
			if (m_isGUISelection)
			{
				item->setSelected(false);
				item->disconnectFromGUI();
			}
			item->isSingleSel = false;
		}
		if (m_SelList.isEmpty())
			m_hasGroupSelection=false;
		else if (m_isGUISelection)
		{
			m_sigSelectionChanged = true;
			sendSignals();
		}
		return removeOk;
	}
	return false;
}

PageItem* Selection::takeItem(int itemIndex)
{
	if (!m_SelList.isEmpty() && itemIndex<m_SelList.count())
	{
		PageItem *item =  m_SelList[itemIndex];
		bool removeOk  = (m_SelList.removeAll(item) == 1);
		if (removeOk)
		{
			item->isSingleSel = false;
			if (m_isGUISelection)
			{
				item->setSelected(false);
				m_sigSelectionChanged = true;
				if (itemIndex == 0)
					item->disconnectFromGUI();
			}
			if (m_SelList.isEmpty())
				m_hasGroupSelection=false;
			sendSignals();
			return item;
		}
	}
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

bool Selection::signalsDelayed(void)
{
	return (m_isGUISelection && (m_delaySignals > 0));
}

void Selection::delaySignalsOn(void)
{
	++m_delaySignals;
}

void Selection::delaySignalsOff(void)
{
	--m_delaySignals;
	if (m_delaySignals <= 0)
		sendSignals();
}

void Selection::sendSignals(bool guiConnect)
{
	if (m_isGUISelection && (m_delaySignals <= 0))
	{
		if (guiConnect)
			connectItemToGUI();
		if (m_sigSelectionChanged)
			emit selectionChanged();
		if (m_sigSelectionIsMultiple)
			emit selectionIsMultiple(m_hasGroupSelection);
		m_sigSelectionChanged = false;
		m_sigSelectionIsMultiple = false;
	}
}
