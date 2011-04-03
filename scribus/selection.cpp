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

#include "sclimits.h"
#include "scribusdoc.h"
#include "selection.h"
#include <QDebug>

Selection::Selection(QObject* parent)
	: QObject(parent),
	m_isGUISelection(false),
	m_delaySignals(0),
	m_sigSelectionChanged(false),
	m_sigSelectionIsMultiple(false)
{
	groupX   = groupY   = groupW   = groupH   = 0;
	visualGX = visualGY = visualGW = visualGH = 0;
}

Selection::Selection(QObject* parent, bool guiSelection) 
	: QObject(parent),
	m_isGUISelection(guiSelection),
	m_delaySignals(0),
	m_sigSelectionChanged(false),
	m_sigSelectionIsMultiple(false)
{
	groupX   = groupY   = groupW   = groupH   = 0;
	visualGX = visualGY = visualGW = visualGH = 0;
}

Selection::Selection(const Selection& other) :
	QObject(other.parent()),
	m_SelList(other.m_SelList),
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
		emit selectionIsMultiple(m_SelList.count() > 1);
	}
	groupX = other.groupX;
	groupY = other.groupY;
	groupW = other.groupW;
	groupH = other.groupH;
	visualGX = other.visualGX;
	visualGY = other.visualGY;
	visualGW = other.visualGW;
	visualGH = other.visualGH;
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
		emit selectionIsMultiple(m_SelList.count() > 1);
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
	m_sigSelectionChanged = true;
	sendSignals();
	return true;
}

bool Selection::connectItemToGUI()
{
	bool ret = false;
	if (!m_isGUISelection || m_SelList.isEmpty())
		return ret;
	if (m_SelList.count() == 1)
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
    bool removeOk(false);
	if (!m_SelList.isEmpty() && m_SelList.contains(item))
	{
		removeOk=(m_SelList.removeAll(item)==1);
		if (removeOk)
		{
			if (m_isGUISelection)
			{
				item->setSelected(false);
				item->disconnectFromGUI();
			}
			item->isSingleSel = false;
		}

		if (m_isGUISelection)
		{
			m_sigSelectionChanged = true;
			sendSignals();
		}
		return removeOk;
	}
	return removeOk;
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
	double minX =  std::numeric_limits<double>::max();
	double maxX = -std::numeric_limits<double>::max();
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
	double minY =  std::numeric_limits<double>::max();
	double maxY = -std::numeric_limits<double>::max();
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
	uint selectedItemCount = count();
	if (selectedItemCount == 0)
	{
		groupX   = groupY   = groupW   = groupH   = 0;
		visualGX = visualGY = visualGW = visualGH = 0;
		return;
	}
	double minx  =  std::numeric_limits<double>::max();
	double miny  =  std::numeric_limits<double>::max();
	double maxx  = -std::numeric_limits<double>::max();
	double maxy  = -std::numeric_limits<double>::max();
	double vminx =  std::numeric_limits<double>::max();
	double vminy =  std::numeric_limits<double>::max();
	double vmaxx = -std::numeric_limits<double>::max();
	double vmaxy = -std::numeric_limits<double>::max();

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
			
			// Same for visual
// 			pb.setPoint(0, FPoint(currItem->visualXPos(), currItem->visualYPos()));
// 			pb.setPoint(1, FPoint(currItem->visualWidth(), 0.0, currItem->visualXPos(), currItem->visualYPos(), currItem->rotation(), 1.0, 1.0));
// 			pb.setPoint(2, FPoint(currItem->visualWidth(), currItem->visualHeight(), currItem->visualXPos(), currItem->visualYPos(), currItem->rotation(), 1.0, 1.0));
// 			pb.setPoint(3, FPoint(0.0, currItem->visualHeight(), currItem->visualXPos(), currItem->visualYPos(), currItem->rotation(), 1.0, 1.0));
			QRectF itRect(currItem->getVisualBoundingRect());
// 			for (uint pc = 0; pc < 4; ++pc)
			{
				vminx = qMin(vminx, itRect.x());
				vminy = qMin(vminy, itRect.y());
				vmaxx = qMax(vmaxx, itRect.right());
				vmaxy = qMax(vmaxy, itRect.bottom());
			}
		}
		else
		{
			minx = qMin(minx, currItem->xPos());
			miny = qMin(miny, currItem->yPos());
			maxx = qMax(maxx, currItem->xPos() + currItem->width());
			maxy = qMax(maxy, currItem->yPos() + currItem->height());
			
			vminx = qMin(vminx, currItem->visualXPos());
			vminy = qMin(vminy, currItem->visualYPos());
			vmaxx = qMax(vmaxx, currItem->visualXPos() + currItem->visualWidth());
			vmaxy = qMax(vmaxy, currItem->visualYPos() + currItem->visualHeight());
		}
	}
	groupX = minx;
	groupY = miny;
	groupW = maxx - minx;
	groupH = maxy - miny;
	
	visualGX = vminx;
	visualGY = vminy;
	visualGW = vmaxx - vminx;
	visualGH = vmaxy - vminy;
}

void Selection::getGroupRect(double *x, double *y, double *w, double *h)
{
	setGroupRect();
	*x = groupX;
	*y = groupY;
	*w = groupW;
	*h = groupH;
}

QRectF Selection::getGroupRect()
{
	double x, y, w, h;
	getGroupRect(&x, &y, &w, &h);
	return QRectF(x,y,w,h);
}

void Selection::getVisualGroupRect(double * x, double * y, double * w, double * h)
{
	setGroupRect();
	*x = visualGX;
	*y = visualGY;
	*w = visualGW;
	*h = visualGH;
}

QRectF Selection::getVisualGroupRect()
{
	double x, y, w, h;
	getGroupRect(&x, &y, &w, &h);
	return QRectF(x,y,w,h);
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
		if ((*it)->isGroup())		// ignore GroupControl items
			continue;
		if ((*it)->itemType() != itemType)
			return false;
	}
	return true;
}

int Selection::objectsLayer(void) const
{
	if (m_SelList.isEmpty())
		return -1;
	int layerID = m_SelList.at(0)->LayerID;
	for (int i = 1; i < m_SelList.count(); ++i)
	{
		if (m_SelList.at(i)->LayerID != layerID)
		{
			layerID = -1;
			break;
		}
	}
	return layerID;
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
		setGroupRect();
		// JG - We should probably add an m_sigSelectionChanged here
		// to avoid multiple connectItemToGUI() if sendSignals() is called 
		// several times successively (but does that happen?)
		if (guiConnect /*&& m_sigSelectionChanged*/)
			connectItemToGUI();
		if (m_sigSelectionChanged)
			emit selectionChanged();
		if (m_sigSelectionIsMultiple)
			emit selectionIsMultiple(m_SelList.count() > 1);
		m_sigSelectionChanged = false;
		m_sigSelectionIsMultiple = false;
	}
}


