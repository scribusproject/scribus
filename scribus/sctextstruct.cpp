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

struct InlineFrameData
{
	QPointer<PageItem> item;
	int refs;
	
	void reserve()  
	{ 
		++refs; 
	}
	
	bool release()
	{
		--refs;
		if (refs == 0 && item != NULL)
		{
			item->doc()->FrameItems.removeAll(item);
			item->doc()->updateFrameItems();
			delete item;
			item = NULL;
		}
		return refs == 0;
	}
};

InlineFrame::InlineFrame(PageItem* item)
{
	d = new InlineFrameData;
	d->item = item;
	d->refs = 1;
}

InlineFrame::InlineFrame(const InlineFrame& other)
{
	d = other.d;
	d->reserve();	
}

InlineFrame& InlineFrame::operator= (const InlineFrame& other)
{
	if (this != &other)
	{
		if (d->release())
			delete d;
		d = other.d;
		d->reserve();
	}
	return *this;
}

InlineFrame::~InlineFrame()
{
	if (d->release())
		delete d;
}

bool InlineFrame::hasItem() const
{
	return d->item != NULL;
}

bool InlineFrame::isShared() const
{
	return d->refs > 1;
}

PageItem* InlineFrame::getItem()
{
	return d->item;
}

QList<PageItem*> InlineFrame::getGroupedItems()
{
	QList<PageItem*> result;
//	result.setAutoDelete(false);
	if (hasItem())
	{
		PageItem* dItem = d->item;
		ScribusDoc& doc(*dItem->doc());
		result.append(d->item);
		if (dItem->Groups.count() != 0)
		{
			for (int ga=0; ga < doc.FrameItems.count(); ++ga)
			{
				if (doc.FrameItems.at(ga)->Groups.count() != 0)
				{
					if (doc.FrameItems.at(ga)->Groups.top() == dItem->Groups.top())
					{
						if (doc.FrameItems.at(ga)->ItemNr != dItem->ItemNr)
						{
							if (!result.contains(doc.FrameItems.at(ga)))
								result.append(doc.FrameItems.at(ga));
						}
					}
				}
			}
		}
	}
	return result;
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
		return this->embedded.hasItem();
	return false;
}
