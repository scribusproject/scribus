/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QObject>
#include <QList>
#include <QPointer>
#include "sctextstruct.h"
#include "scfonts.h"
#include "pageitem.h"
#include "scribusdoc.h"

void GlyphLayout::growWithTabLayout()
{
	if (more)
		shrink();
	more = new TabLayout();
}

ScText::~ScText() 
{
	// delete the linked list if present
	GlyphLayout * more = glyph.more;
	while (more) {
		glyph.more = glyph.more->more;
		delete more;
		more = glyph.more;
	}
	if (parstyle)
		delete parstyle;
	parstyle = NULL;
}

bool ScText::hasObject() const
{
	if (this->ch == SpecialChars::OBJECT)
		return this->embedded != NULL;
	return false;
}

QList<PageItem*> ScText::getGroupedItems()
{
	QList<PageItem*> result;
	if (this->embedded != NULL)
	{
		PageItem* dItem = this->embedded;
		result.append(dItem);
		if (dItem->isGroup())
			result = dItem->getItemList();
	}
	return result;
}

PageItem* ScText::getItem()
{
	return embedded;
}
