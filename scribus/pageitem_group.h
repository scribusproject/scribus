/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem_symbol.h  -  description
                             -------------------
    copyright            : Scribus Team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PAGEITEM_GROUP_H
#define PAGEITEM_GROUP_H

#include <QString>
#include <QRectF>

#include "scribusapi.h"
#include "pageitem.h"
class ScPainter;
class ScribusDoc;


class SCRIBUS_API PageItem_Group : public PageItem
{
	Q_OBJECT

public:
	PageItem_Group(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
	PageItem_Group(const PageItem & p) : PageItem(p) {}
	~PageItem_Group();

	virtual PageItem_Group * asGroupFrame() { return this; }
	virtual bool isGroup() const { return true; }
	virtual ItemType realItemType() const { return PageItem::Group; }
	virtual QList<PageItem*> getItemList();
	virtual void getNamedResources(ResourceCollection& lists) const;
	virtual void replaceNamedResources(ResourceCollection& newNames);
	virtual void applicableActions(QStringList& actionList);
	virtual QString infoDescription();
	void adjustXYPosition();
	
protected:
	virtual void DrawObj_Item(ScPainter *p, QRectF e);

};

#endif 
