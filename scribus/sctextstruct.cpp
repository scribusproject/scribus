/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QList>
#include "sctextstruct.h"
#include "pageitem.h"
#include "scribusdoc.h"


PageItem* InlineFrame::getPageItem(const ScribusDoc* doc) const
{
	if (doc->FrameItems.contains(m_object_id))
		return doc->FrameItems[m_object_id];
	return nullptr;
}


ScText::~ScText() 
{
	delete parstyle;
	parstyle = nullptr;
	mark = nullptr;
}

bool ScText::hasObject(const ScribusDoc *doc) const
{
	if (this->ch == SpecialChars::OBJECT)
		return ((embedded > 0) && (doc->FrameItems.contains(embedded)));
	return false;
}

bool ScText::hasMark(const Mark* mrk) const
{
	if (this->ch == SpecialChars::OBJECT)
	{
		if (mrk == nullptr)
			return mark != nullptr;
		return mark == mrk;
	}
	return false;
}

QList<PageItem*> ScText::getGroupedItems(const ScribusDoc *doc) const
{
	QList<PageItem*> result;
	if ((embedded > 0) && (doc->FrameItems.contains(embedded)))
	{
		PageItem* dItem = doc->FrameItems[embedded];
		result.append(dItem);
		if (dItem->isGroup())
			result = dItem->getAllChildren();
	}
	return result;
}

PageItem* ScText::getItem(const ScribusDoc *doc) const
{
	if ((embedded > 0) && (doc->FrameItems.contains(embedded)))
		return doc->FrameItems[embedded];
	return nullptr;
}

void ScText::setNewMark(Mark *mrk)
{
	if (!mrk->isUnique())
		mark = mrk;
}
