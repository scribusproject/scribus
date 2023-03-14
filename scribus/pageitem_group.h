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
	PageItem_Group(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
	PageItem_Group(const PageItem & p) : PageItem(p) {}
	~PageItem_Group();

	PageItem_Group * asGroupFrame() override { return this; }
	const PageItem_Group * asGroupFrame() const override { return this; }
	bool isGroup() const override { return true; }
	bool isTextContainer() const override;
	ItemType realItemType() const override { return PageItem::Group; }

	void adjustXYPosition();
	void setLayer(int layerId) override;
	void setMasterPage(int page, const QString& mpName) override;
	void setMasterPageName(const QString& mpName) override;
	void getNamedResources(ResourceCollection& lists) const override;
	void replaceNamedResources(ResourceCollection& newNames) override;
	void applicableActions(QStringList& actionList) override;
	 QString infoDescription() const override;

	/// Retrieve child items of this item
	QList<PageItem*> getChildren() const override { return groupItemList; }
	/// Retrieve all children of item, including children of children
	QList<PageItem*> getAllChildren() const override;

	void layout() override;
	
protected:
	void DrawObj_Item(ScPainter *p, const QRectF& e) override;

};

#endif 
