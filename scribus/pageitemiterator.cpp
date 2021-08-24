/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pageitemiterator.h"
#include "pageitem.h"
#include "pageitem_group.h"
#include "pageitem_table.h"
#include "scpattern.h"
#include "scribusdoc.h"

PageItemIterator::PageItemIterator(int options) :
	m_options(options)
{
	m_stateStack.reserve(16);
}

PageItemIterator::PageItemIterator(const QList<PageItem*>& itemList, int options) :
	m_options(options)
{
	if (itemList.count() > 0)
	{
		State state = { itemList, 0 };
		m_stateStack.reserve(16);
		m_stateStack.push_back(state);
		m_current = next();
	}
}

PageItemIterator::PageItemIterator(const ScribusDoc* doc, int options) :
	m_options(options)
{
	int stackItemCount = doc->docPatterns.count() + 3;
	m_stateStack.reserve(stackItemCount);

	if ((m_options & IterateInDocPatterns) && (doc->docPatterns.count() > 0))
	{
		auto docPatternEnd = doc->docPatterns.constEnd();
		for (auto it = doc->docPatterns.constBegin(); it != docPatternEnd; ++it)
		{
			const ScPattern& pattern = it.value();
			if (pattern.items.count() > 0)
				m_stateStack.emplace_back(pattern.items, 0);
		}
	}

	if ((m_options & IterateInFrameItems) && (doc->FrameItems.count() > 0))
		m_stateStack.emplace_back(doc->FrameItems.values(), 0);

	if ((m_options & IterateInMasterItems) && (doc->MasterItems.count() > 0))
		m_stateStack.emplace_back(doc->MasterItems, 0);

	if ((m_options & IterateInDocItems) && (doc->DocItems.count() > 0))
		m_stateStack.emplace_back(doc->DocItems, 0);

	if (m_stateStack.size() > 0)
		m_current = next();
}

PageItem* PageItemIterator::begin(ScribusDoc* doc, int options)
{
	m_current = nullptr;
	m_options = options;
	m_stateStack.resize(0); // Not clear in order to keep the already allocated memory available

	size_t stackItemCount = static_cast<size_t>(doc->docPatterns.count() + 3);
	if (m_stateStack.capacity() < stackItemCount)
		m_stateStack.reserve(stackItemCount);

	if ((m_options & IterateInDocPatterns) && (doc->docPatterns.count() > 0))
	{
		auto docPatternEnd = doc->docPatterns.constEnd();
		for (auto it = doc->docPatterns.constBegin(); it != docPatternEnd; ++it)
		{
			const ScPattern& pattern = it.value();
			if (pattern.items.count() > 0)
				m_stateStack.emplace_back(pattern.items, 0);
		}
	}

	if ((m_options & IterateInFrameItems) && (doc->FrameItems.count() > 0))
		m_stateStack.emplace_back(doc->FrameItems.values(), 0);

	if ((m_options & IterateInMasterItems) && (doc->MasterItems.count() > 0))
		m_stateStack.emplace_back(doc->MasterItems, 0);

	if ((m_options & IterateInDocItems) && (doc->DocItems.count() > 0))
		m_stateStack.emplace_back(doc->DocItems, 0);

	if (m_stateStack.size() > 0)
		m_current = next();
	return m_current;
}

PageItem* PageItemIterator::begin(const QList<PageItem*>& itemList)
{
	m_current = nullptr;
	m_stateStack.resize(0); // Not clear in order to keep the already allocated memory available

	if (itemList.count() > 0)
	{
		m_stateStack.emplace_back(itemList, 0);
		m_current = next();
	}

	return m_current;
}

PageItem* PageItemIterator::next()
{
	while (m_stateStack.size() > 0)
	{
		State& currentState = m_stateStack.back();
		if (currentState.currentIndex >= currentState.itemList.count())
		{
			m_stateStack.pop_back();
			continue;
		}

		PageItem* item = currentState.itemList.at(currentState.currentIndex);
		if (!item)
		{
			currentState.currentIndex++;
			continue;
		}

		if (m_options & IterateInGroups)
		{
			PageItem_Group* groupItem = item->asGroupFrame();
			if (groupItem)
			{
				m_current = groupItem;
				currentState.currentIndex++;
				if (groupItem->groupItemList.count() > 0)
					m_stateStack.emplace_back(groupItem->groupItemList, 0);
				break;
			}
		}

		if (m_options & IterateInTables)
		{
			PageItem_Table* tableItem = item->asTable();
			if (tableItem)
			{
				m_current = tableItem;
				currentState.currentIndex++;
				QList<PageItem*> cellItems = tableItem->getChildren();
				if (cellItems.count() > 0)
					m_stateStack.emplace_back(cellItems, 0);
				break;
			}
		}

		m_current = item;
		currentState.currentIndex++;
		break;
	}

	if (m_stateStack.empty())
		m_current = nullptr;
	return m_current;
}

PageItem* PageItemIterator::moveTo(PageItem* item)
{
	if (current() == item)
		return item;

	PageItem* nextItem = next();
	while (nextItem)
	{
		if (nextItem == item)
			break;
		nextItem = next();
	}
	return nextItem;
}

PageItem* PageItemIterator::movePast(PageItem* item)
{
	if (current() == nullptr)
		return nullptr;

	PageItem* nextItem = moveTo(item);
	if (nextItem)
		nextItem = next();
	return nextItem;
}
