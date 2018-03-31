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

class SCRIBUS_API PageItemIterator
{
public:
	PageItemIterator(const QList<PageItem*>& itemList);

	bool hasCurrent() const { return (m_current != 0); }
	PageItem* current() const { return m_current; }
	PageItem* next();

	inline PageItem*  operator*() const { return m_current; }
	inline PageItem*  operator->() const { return m_current; }
	PageItemIterator& operator++() { next(); return *this; }

protected:
	struct State
	{
		const QList<PageItem*> *pItemList;
		int   currentIndex;
	};

	PageItem* m_current;
	QStack<State> m_stateStack;
};

#endif // PAGEITEMITERATOR_H