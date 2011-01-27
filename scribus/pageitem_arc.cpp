/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem_arc.cpp  -  description
                             -------------------
    begin                : Mon Jan 3 2011
    copyright            : (C) 2011 by Franz Schmid
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

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <cassert>

#include "page.h"
#include "pageitem.h"
#include "pageitem_arc.h"
#include "prefsmanager.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusdoc.h"

#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include "util.h"
#include "util_math.h"

using namespace std;

PageItem_Arc::PageItem_Arc(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::Arc, x, y, w, h, w2, fill, outline)
{
	arcHeight = h;
	arcWidth = w;
	arcStartAngle = m_Doc->itemToolPrefs().arcStartAngle;
	arcSweepAngle = m_Doc->itemToolPrefs().arcSweepAngle;
	recalcPath();
}

void PageItem_Arc::DrawObj_Item(ScPainter *p, QRectF /*e*/)
{
	if (!m_Doc->RePos)
	{
		p->setupPolygon(&PoLine);
		p->fillPath();
	}
}

void PageItem_Arc::recalcPath()
{
	QPainterPath path;
	path.moveTo(arcWidth / 2.0, arcHeight / 2.0);
	path.arcTo(0.0, 0.0, arcWidth, arcHeight, arcStartAngle, arcSweepAngle);
	path.closeSubpath();
	PoLine.fromQPainterPath(path);
	Clip = FlattenPath(PoLine, Segments);
}

void PageItem_Arc::applicableActions(QStringList & actionList)
{
	actionList << "itemConvertToBezierCurve";
	actionList << "itemConvertToPolygon";
	actionList << "itemConvertToImageFrame";
	actionList << "itemConvertToTextFrame";
}

QString PageItem_Arc::infoDescription()
{
	return QString();
}
