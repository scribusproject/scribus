/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PAGEITEMITERATOR_H
#define PAGEITEMITERATOR_H

#include <vector>
#include <QList>

#include "scribusapi.h"

class PageItem;
class ScribusDoc;

class SCRIBUS_API PageItemIterator
{
public:
	enum Options
	{
		IterateInGroups = 1,
		IterateInTables = 2,
		IterateInDocItems = 4,
		IterateInMasterItems = 8,
		IterateInFrameItems = 16,
		IterateInDocPatterns = 32,
		IterateInDocNoPatterns = IterateInGroups + IterateInDocItems + IterateInMasterItems + IterateInFrameItems,
		IterateInDocDefaults = IterateInGroups + IterateInDocItems + IterateInMasterItems + IterateInFrameItems + IterateInDocPatterns,
		IterateAll = IterateInGroups + IterateInTables + IterateInDocItems + IterateInMasterItems + IterateInFrameItems + IterateInDocPatterns
	};
	PageItemIterator(int options = IterateInGroups);
	PageItemIterator(const QList<PageItem*>& itemList, int options = IterateInGroups);
	PageItemIterator(const ScribusDoc* doc, int options = IterateInDocDefaults);

	bool isNull() const { return (m_current == nullptr); }
	PageItem* current() const { return m_current; }
	
	PageItem* begin(ScribusDoc* doc, int options = IterateInDocDefaults);
	PageItem* begin(const QList<PageItem*>& itemList);
	PageItem* next();

	PageItem* moveTo(PageItem* item);
	PageItem* movePast(PageItem* item);

	inline PageItem*  operator*() const { return m_current; }
	inline PageItem*  operator->() const { return m_current; }
	PageItemIterator& operator++() { next(); return *this; }

protected:
	struct State
	{
		State() {}
		State(const QList<PageItem*> items, int startIndex)
		{
			itemList = items;
			currentIndex = startIndex;
		}
		QList<PageItem*> itemList;
		int   currentIndex { 0 };
	};

	int m_options { 0 };
	PageItem* m_current { nullptr };
	std::vector<State> m_stateStack;
};

#endif // PAGEITEMITERATOR_H