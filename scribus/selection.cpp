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

Selection::Selection(QObject* parent) :
	QObject(parent),
	m_isGUISelection(false),
	m_delaySignals(0),
	m_sigSelectionChanged(false)
{
	m_groupX   = m_groupY   = m_groupW   = m_groupH   = 0;
	m_visualGX = m_visualGY = m_visualGW = m_visualGH = 0;
}

Selection::Selection(QObject* parent, bool guiSelection) :
	QObject(parent),
	m_isGUISelection(guiSelection),
	m_delaySignals(0),
	m_sigSelectionChanged(false)
{
	m_groupX   = m_groupY   = m_groupW   = m_groupH   = 0;
	m_visualGX = m_visualGY = m_visualGW = m_visualGH = 0;
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
	m_sigSelectionChanged(other.m_sigSelectionChanged)
{
	if (m_isGUISelection && !m_SelList.isEmpty())
	{
		m_SelList[0]->connectToGUI();
		m_SelList[0]->emitAllToGUI();
		m_SelList[0]->setSelected(true);
		emit selectionChanged();
	}
	m_groupX = other.m_groupX;
	m_groupY = other.m_groupY;
	m_groupW = other.m_groupW;
	m_groupH = other.m_groupH;
	m_visualGX = other.m_visualGX;
	m_visualGY = other.m_visualGY;
	m_visualGW = other.m_visualGW;
	m_visualGH = other.m_visualGH;
}

Selection& Selection::operator=(const Selection &other)
{
	if (&other == this)
		return *this;
	if (m_isGUISelection)
	{
		SelectionList::Iterator itend = m_SelList.end();
		for (SelectionList::Iterator it = m_SelList.begin(); it != itend; ++it)
			(*it)->setSelected(false);
	}
	m_SelList = other.m_SelList;
	// Do not copy m_isGUISelection for consistency with cpy ctor
	/* m_isGUISelection = other.m_isGUISelection; */
	// We do not copy m_delaySignals as that can potentially
	// cause much trouble balancing delaySignalOff/On right
	m_sigSelectionChanged = other.m_sigSelectionChanged;
	if (m_isGUISelection && !m_SelList.isEmpty())
	{
		m_SelList[0]->connectToGUI();
		m_SelList[0]->emitAllToGUI();
		m_SelList[0]->setSelected(true);
		emit selectionChanged();
	}
	return *this;
}

void Selection::copy(Selection& other, bool emptyOther)
{
	if (&other == this)
		return;
	if (m_isGUISelection)
	{
		SelectionList::Iterator itend = m_SelList.end();
		for (SelectionList::Iterator it = m_SelList.begin(); it != itend; ++it)
			(*it)->setSelected(false);
	}
	m_SelList = other.m_SelList;
	if (m_isGUISelection)
		m_sigSelectionChanged = true;
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
		SelectionList::Iterator itend = m_SelList.end();
		SelectionList::Iterator it = m_SelList.begin();
		while (it != itend)
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
	if (!m_isGUISelection || m_SelList.isEmpty())
		return false;

	QPointer<PageItem> pi = m_SelList.first();
	//Quick check to see if the pointer is nullptr, if its nullptr, we should remove it from the list now
	while (pi.isNull())
	{
		m_SelList.removeAll(pi);
		if (m_SelList.isEmpty())
			break;
		pi = m_SelList.first();
	}

	if (pi.isNull())
		return false;

	bool ret = pi->connectToGUI();
	pi->emitAllToGUI();
	m_sigSelectionChanged = true;

	sendSignals(false);
	return ret;
}

bool Selection::disconnectAllItemsFromGUI()
{
	if (!m_isGUISelection || m_SelList.isEmpty())
		return false;
	SelectionList::Iterator it2end = m_SelList.end();
	SelectionList::Iterator it2 = m_SelList.begin();
	while (it2 != it2end)
	{
		(*it2)->disconnectFromGUI();
		++it2;
	}
	return true;
}

bool Selection::addItem(PageItem *item, bool /*ignoreGUI*/)
{
	if (item == nullptr)
		return false;
	bool listWasEmpty = m_SelList.isEmpty();
	if (listWasEmpty || !m_SelList.contains(item))
	{
		m_SelList.append(item);
		if (m_isGUISelection)
		{
			item->setSelected(true);
			m_sigSelectionChanged = true;
		}
		sendSignals();
		return true;
	}
	return false;
}

bool Selection::addItems(const QList<PageItem *> items)
{
	if (items.isEmpty())
		return false;

	QList< QPointer<PageItem> > toAdd;
	toAdd.reserve(items.count());
	for (int i = 0; i < items.count(); ++i)
	{
		PageItem* item = items.at(i);
		if (m_SelList.contains(item))
			continue;
		toAdd.append(item);
		item->setSelected(true);
	}

	if (toAdd.count() <= 0)
		return false;

	m_SelList.append(toAdd);
	if (m_isGUISelection)
		m_sigSelectionChanged = true;
	sendSignals();
	return true;
}

bool Selection::prependItem(PageItem *item)
{
	if (item == nullptr)
		return false;
	if (m_SelList.contains(item))
		return false;

	if (m_isGUISelection && !m_SelList.isEmpty())
		m_SelList[0]->disconnectFromGUI();
	m_SelList.prepend(item);
	if (m_isGUISelection)
	{
		item->setSelected(true);
		m_sigSelectionChanged = true;
	}	
	sendSignals();
	return true;
}

PageItem *Selection::itemAt_(int index)
{
	if (!m_SelList.isEmpty() && index < m_SelList.count())
	{
		QPointer<PageItem> pi = m_SelList[index];
		//If not nullptr return it, otherwise remove from the list and return nullptr
		if (!pi.isNull())
			return pi;
		m_SelList.removeAt(index);
	}
	return nullptr;
}

QList<PageItem*> Selection::items() const
{
	QList<PageItem*> selectedItems;
	for (int i = 0; i < m_SelList.count(); ++i)
	{
		QPointer<PageItem> pi = m_SelList.at(i);
		if (pi.isNull())
			continue;
		selectedItems.append(pi.data());
	}
	return selectedItems;
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
	if (m_SelList.isEmpty() || !m_SelList.contains(item))
		return false;

	bool removeOk = (m_SelList.removeAll(item) == 1);
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

bool Selection::removeItemsOfLayer(int layedID)
{
	if (m_SelList.isEmpty())
		return false;

	int oldSelCount = m_SelList.count();

	delaySignalsOn();
	
	int selIndex = 0;
	while (selIndex < m_SelList.count())
	{
		QPointer<PageItem> pi = m_SelList.at(selIndex);
		if (pi.isNull())
		{
			removeItem(itemAt(selIndex));
			continue;
		}

		if (pi->m_layerID != layedID)
		{
			++selIndex;
			continue;
		}

		removeItem(itemAt(selIndex));
	}

	delaySignalsOff();

	bool itemsRemoved = (oldSelCount != m_SelList.count());
	return itemsRemoved;
}

void Selection::replaceItem(PageItem* oldItem, PageItem* newItem)
{
	delaySignalsOn();

	int itemIndex = findItem(oldItem);
	if (itemIndex >= 0)
		m_SelList.replace(itemIndex, newItem);

	delaySignalsOff();
}

PageItem* Selection::takeItem(int itemIndex)
{
	if (m_SelList.isEmpty() || itemIndex >= m_SelList.count())
		return nullptr;

	PageItem *item =  m_SelList[itemIndex];
	bool removeOk = (m_SelList.removeAll(item) == 1);
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
	return nullptr;
}

QStringList Selection::getSelectedItemsByName() const
{
	QStringList names;
	SelectionList::ConstIterator itend = m_SelList.end();
	for (auto it = m_SelList.begin(); it != itend ; ++it)
		names.append((*it)->itemName());
	return names;
}

double Selection::width() const
{
	if (m_SelList.isEmpty())
		return 0.0;
	double minX =  std::numeric_limits<double>::max();
	double maxX = -std::numeric_limits<double>::max();
	double x1 = 0.0, x2 = 0.0, y1 = 0.0, y2 = 0.0;

	SelectionList::ConstIterator itend = m_SelList.end();
	for (auto it = m_SelList.begin(); it != itend ; ++it)
	{
		(*it)->getBoundingRect(&x1, &y1, &x2, &y2);
		if (x1 < minX)
			minX = x1;
		if (x2 > maxX)
			maxX = x2;
	}
	return maxX - minX;
}

double Selection::height() const
{
	if (m_SelList.isEmpty())
		return 0.0;
	double minY =  std::numeric_limits<double>::max();
	double maxY = -std::numeric_limits<double>::max();
	double x1 = 0.0, x2 = 0.0, y1 = 0.0, y2 = 0.0;

	SelectionList::ConstIterator itend = m_SelList.end();
	for (auto it = m_SelList.begin(); it != itend ; ++it)
	{
		(*it)->getBoundingRect(&x1, &y1, &x2, &y2);
		if (y1 < minY)
			minY = y1;
		if (y2 > maxY)
			maxY = y2;
	}
	return maxY - minY;
}

void Selection::setGroupRect()
{
	PageItem *currItem;
	int selectedItemCount = count();
	if (selectedItemCount == 0)
	{
		m_groupX   = m_groupY   = m_groupW   = m_groupH   = 0;
		m_visualGX = m_visualGY = m_visualGW = m_visualGH = 0;
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

	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = itemAt(i);
		if (currItem->rotation() != 0.0)
		{
			QRectF itRect(currItem->getBoundingRect());
			minx = qMin(minx, itRect.x());
			miny = qMin(miny, itRect.y());
			maxx = qMax(maxx, itRect.right());
			maxy = qMax(maxy, itRect.bottom());
			
			// Same for visual
			QRectF itVisualRect(currItem->getVisualBoundingRect());
			vminx = qMin(vminx, itVisualRect.x());
			vminy = qMin(vminy, itVisualRect.y());
			vmaxx = qMax(vmaxx, itVisualRect.right());
			vmaxy = qMax(vmaxy, itVisualRect.bottom());
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
	m_groupX = minx;
	m_groupY = miny;
	m_groupW = maxx - minx;
	m_groupH = maxy - miny;
	
	m_visualGX = vminx;
	m_visualGY = vminy;
	m_visualGW = vmaxx - vminx;
	m_visualGH = vmaxy - vminy;
}

void Selection::getGroupRect(double *x, double *y, double *w, double *h)
{
	setGroupRect();
	*x = m_groupX;
	*y = m_groupY;
	*w = m_groupW;
	*h = m_groupH;
}

QRectF Selection::getGroupRect()
{
	double x, y, w, h;
	getGroupRect(&x, &y, &w, &h);
	return QRectF(x, y, w, h);
}

void Selection::getVisualGroupRect(double * x, double * y, double * w, double * h)
{
	setGroupRect();
	*x = m_visualGX;
	*y = m_visualGY;
	*w = m_visualGW;
	*h = m_visualGH;
}

QRectF Selection::getVisualGroupRect()
{
	double x, y, w, h;
	getGroupRect(&x, &y, &w, &h);
	return QRectF(x, y, w, h);
}

bool Selection::containsItemType(PageItem::ItemType type) const
{
	if (m_SelList.isEmpty())
		return false;
	SelectionList::ConstIterator it = m_SelList.begin();
	SelectionList::ConstIterator itend = m_SelList.end();
	for (; it != itend; ++it)
	{
		if ((*it)->itemType() == type)
			return true;
	}
	return false;
}

bool Selection::itemsAreSameType() const
{
	//CB Putting count=1 before isempty test as its probably the most likely, given our view code.
	if (m_SelList.count() == 1)
		return true;
	if (m_SelList.isEmpty())
		return false;
	SelectionList::ConstIterator it = m_SelList.begin();
	SelectionList::ConstIterator itend = m_SelList.end();
	PageItem::ItemType itemType = (*it)->itemType();
	for ( ; it != itend ; ++it)
	{
		if ((*it)->isGroup())		// ignore GroupControl items
			continue;
		if ((*it)->itemType() != itemType)
			return false;
	}
	return true;
}

bool Selection::itemsAreOnSamePage() const
{
	//CB Putting count=1 before isempty test as its probably the most likely, given our view code.
	if (m_SelList.count() == 1)
		return true;
	if (m_SelList.isEmpty())
		return false;
	auto it = m_SelList.begin();
	auto itend = m_SelList.end();
	auto ownPage = (*it)->OwnPage;
	for ( ; it != itend ; ++it)
	{
		if ((*it)->OwnPage != ownPage)
			return false;
	}
	return true;
}

int Selection::objectsLayer() const
{
	if (m_SelList.isEmpty())
		return -1;
	int layerID = m_SelList.at(0)->m_layerID;
	for (int i = 1; i < m_SelList.count(); ++i)
	{
		if (m_SelList.at(i)->m_layerID != layerID)
		{
			layerID = -1;
			break;
		}
	}
	return layerID;
}

bool Selection::objectsHaveSameParent() const
{
	int selectedItemCount = m_SelList.count();
	if (selectedItemCount <= 1)
		return true;

	bool haveSameParent = true;
	const PageItem *firstItem = itemAt(0);
	for (int i = 1; i < selectedItemCount; ++i)
	{
		if (itemAt(i)->Parent != firstItem->Parent)
		{
			haveSameParent = false;
			break;
		}
	}
	return haveSameParent;
}

bool Selection::signalsDelayed()
{
	return (m_isGUISelection && (m_delaySignals > 0));
}

void Selection::delaySignalsOn()
{
	++m_delaySignals;
}

void Selection::delaySignalsOff()
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
		m_sigSelectionChanged = false;
	}
}


