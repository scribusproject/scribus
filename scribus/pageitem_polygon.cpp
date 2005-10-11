/***************************************************************************
                          pageitem.cpp  -  description
                             -------------------
    begin                : Sat Apr 7 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pageitem_polygon.h"
#include "pageitem_polygon.moc"
#include <qpainter.h>
#include <qpen.h>
#include <qfont.h>
#include <qregion.h>
#include <qpoint.h>
#include <qfileinfo.h>
#include <qdrawutil.h>
#include <qbitmap.h>
#include <qregexp.h>
#include <qmessagebox.h>
#include <cmath>
#include <cassert>

#include "mpalette.h"
#include "page.h"
#include "pageitem.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusdoc.h"

#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include <ft2build.h>
#include FT_GLYPH_H

#include "scfontmetrics.h"
#include "util.h"

using namespace std;

PageItem_Polygon::PageItem_Polygon(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::Polygon, x, y, w, h, w2, fill, outline)
{
}

void PageItem_Polygon::DrawObj_Item(ScPainter *p)
{
	if (!Doc->RePos)
	{
		p->setupPolygon(&PoLine);
		p->fillPath();
	}
}

