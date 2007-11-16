/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem_pathtext.h  -  description
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

#ifndef PAGEITEM_PATHTEXT_H
#define PAGEITEM_PATHTEXT_H

#include <QString>
#include <QRectF>

#include "scribusapi.h"
#include "pageitem.h"
class ScPainter;
class ScribusDoc;

class SCRIBUS_API PageItem_PathText : public PageItem
{
	Q_OBJECT

public:
	PageItem_PathText(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
	PageItem_PathText(const PageItem & p) : PageItem(p) {}
	~PageItem_PathText() {};
	
	virtual PageItem_PathText * asPathText() { return this; }
	virtual void layout();
	virtual bool createInfoGroup(QFrame *, QGridLayout *);
	virtual bool createContextMenu(QMenu *, int);
	virtual void applicableActions(QStringList& actionList);
	virtual QString infoDescription();

protected:
	virtual void DrawObj_Item(ScPainter *p, QRectF e, double sc);

};

#endif
