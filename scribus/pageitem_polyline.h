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

#ifndef PAGEITEM_POLYLINE_H
#define PAGEITEM_POLYLINE_H

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
class ScPainter;
class ScribusDoc;
class UndoManager;
class UndoState;
class Foi;
struct CopyPasteBuffer;

class SCRIBUS_API PageItem_PolyLine : public PageItem
{
	Q_OBJECT

public:
	PageItem_PolyLine(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
	~PageItem_PolyLine() {};
	
	virtual PageItem_PolyLine * asPolyLine() { return this; }

protected:
	virtual void DrawObj_Item(ScPainter *p);

};

#endif
