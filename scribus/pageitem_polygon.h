/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem.h  -  description
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

#ifndef PAGEITEM_POLYGON_H
#define PAGEITEM_POLYGON_H

#include <QString>
#include <QRect>

#include "scribusapi.h"
#include "pageitem.h"
class ScPainter;
class ScribusDoc;


class SCRIBUS_API PageItem_Polygon : public PageItem
{
	Q_OBJECT

public:
	PageItem_Polygon(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
	PageItem_Polygon(const PageItem & p) : PageItem(p) {}
	~PageItem_Polygon() {};

	virtual PageItem_Polygon * asPolygon() { return this; }
	virtual bool createContextMenu(QMenu *, int);
	virtual void applicableActions(QStringList& actionList);
	
protected:
	virtual void DrawObj_Item(ScPainter *p, QRect e, double sc);

};

#endif
