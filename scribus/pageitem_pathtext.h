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

#ifndef PAGEITEM_PATHTEXT_H
#define PAGEITEM_PATHTEXT_H

#include <qobject.h>
#include <qwidget.h>
#include <qpointarray.h>
#include <qptrlist.h>
#include <qpixmap.h>
#include <qvaluestack.h>
#include <qvaluelist.h>

#include <qstring.h>
#include <qrect.h>

#include "scribusapi.h"
#include "undoobject.h"
#include "scimage.h"
#include "pagestructs.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
class ScPainter;
class ScribusDoc;
class UndoManager;
class UndoState;
class Foi;
struct CopyPasteBuffer;

class SCRIBUS_API PageItem_PathText : public PageItem
{
	Q_OBJECT

public:
	PageItem_PathText(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
	PageItem_PathText(const PageItem & p) : PageItem(p) {}
	~PageItem_PathText() {};
	
	virtual PageItem_PathText * asPathText() { return this; }

protected:
	virtual void DrawObj_Item(ScPainter *p, double sc);

};

#endif
