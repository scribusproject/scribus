/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PAGEITEMITERATOR_H
#define PAGEITEMITERATOR_H

#include <QList>
#include <QStack>

#include "scribusapi.h"

class PageItem;
class ScribusDoc;

class SCRIBUS_API PageItemIterator
{
public:
	enum Options
	{
		IterateInGroups = 1,
		IterateInTables = 2
	};
	PageItemIterator(int options = IterateInGroups);
	PageItemIterator(const QList<PageItem*>& itemList, int options = IterateInGroups);
	PageItemIterator(ScribusDoc* doc, int options = IterateInGroups);

	bool isNull() const { return (m_current == nullptr); }
	PageItem* current() const { return m_current; }
	
	PageItem* begin(const QList<PageItem*>& itemList);
	PageItem* next();

	inline PageItem*  operator*() const { return m_current; }
	inline PageItem*  operator->() const { return m_current; }
	PageItemIterator& operator++() { next(); return *this; }

protected:
	struct State
	{
		QList<PageItem*> itemList;
		int   currentIndex;
	};

	int m_options { 0 };
	PageItem* m_current { nullptr };
	QStack<State> m_stateStack;
};

#endif // PAGEITEMITERATOR_H