/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pageitemiterator.h"
#include "pageitem.h"
#include "pageitem_group.h"

PageItemIterator::PageItemIterator(const QList<PageItem*>& itemList)
{
	m_current = nullptr;

	if (itemList.count() > 0)
	{
		State state = { &itemList, 0 };
		m_stateStack.push(state);
	}
}

PageItem* PageItemIterator::next()
{
	while (m_stateStack.count() > 0)
	{
		State& currentState = m_stateStack.top();
		if (currentState.currentIndex >= currentState.pItemList->count())
		{
			m_stateStack.pop();
			continue;
		}

		PageItem* item = currentState.pItemList->at(currentState.currentIndex);
		if (!item)
		{
			currentState.currentIndex++;
			continue;
		}

		PageItem_Group* groupItem = item->asGroupFrame();
		if (groupItem)
		{
			m_current = groupItem;
			currentState.currentIndex++;
			if (groupItem->groupItemList.count() > 0)
			{
				State groupState = { &groupItem->groupItemList, 0 };
				m_stateStack.push(groupState);
			}
			break;
		}

		m_current = item;
		currentState.currentIndex++;
		break;
	}

	if (m_stateStack.isEmpty())
		m_current = nullptr;
	return m_current;
}
