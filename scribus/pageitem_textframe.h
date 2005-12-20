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

#ifndef PAGEITEMTEXTFRAME_H
#define PAGEITEMTEXTFRAME_H

#include <qevent.h>
#include <qobject.h>

#include <qpointarray.h>
#include <qptrlist.h>
#include <qpixmap.h>
#include <qrect.h>
#include <qstring.h>
#include <qvaluestack.h>
#include <qvaluelist.h>
#include <qwidget.h>

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

class SCRIBUS_API PageItem_TextFrame : public PageItem
{
	Q_OBJECT

public:
	PageItem_TextFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
	PageItem_TextFrame(const PageItem & p) : PageItem(p) {}
	~PageItem_TextFrame() {};

	virtual PageItem_TextFrame * asTextFrame() { return this; }
	
	virtual void clearContents();
	
	virtual void handleModeEditKey(QKeyEvent *k, bool& keyRepeat);
	void deleteSelectedTextFromFrame();
	void setNewPos(int oldPos, int len, int dir);
	void ExpandSel(int dir, int oldPos);
	void deselectAll();
	
	double columnWidth();
	
protected:
	virtual void DrawObj_Item(ScPainter *p, QRect e, double sc);
	virtual void DrawObj_Post(ScPainter *p);
	void drawOverflowMarker(ScPainter *p);
	
	bool unicodeTextEditMode;
	int unicodeInputCount;
	QString unicodeInputString;

};

#endif
