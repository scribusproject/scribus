/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem_regularpolygon.cpp  -  description
                             -------------------
    begin                : Fri Dec 31 2010
    copyright            : (C) 2010 by Franz Schmid
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
#include "pageitem_regularpolygon.h"
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

PageItem_RegularPolygon::PageItem_RegularPolygon(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::RegularPolygon, x, y, w, h, w2, fill, outline)
{
	polyCorners = m_Doc->itemToolPrefs().polyCorners;
	polyFactor = m_Doc->itemToolPrefs().polyFactor;
	polyUseFactor = m_Doc->itemToolPrefs().polyUseFactor;
	polyRotation = m_Doc->itemToolPrefs().polyRotation;
	polyCurvature = m_Doc->itemToolPrefs().polyCurvature;
	polyInnerRot = m_Doc->itemToolPrefs().polyInnerRot;
	polyOuterCurvature = m_Doc->itemToolPrefs().polyOuterCurvature;
	QPainterPath path = RegularPolygonPath(w, h, polyCorners, polyUseFactor, polyFactor, polyRotation, polyCurvature, polyInnerRot, polyOuterCurvature);
	PoLine.fromQPainterPath(path);
	Clip = FlattenPath(PoLine, Segments);
}

void PageItem_RegularPolygon::DrawObj_Item(ScPainter *p, QRectF /*e*/)
{
	if (!m_Doc->RePos)
	{
		p->setupPolygon(&PoLine);
		p->fillPath();
	}
}

void PageItem_RegularPolygon::recalcPath()
{
	QPainterPath path = RegularPolygonPath(width(), height(), polyCorners, polyUseFactor, polyFactor, polyRotation, polyCurvature, polyInnerRot, polyOuterCurvature);
	PoLine.fromQPainterPath(path);
	Clip = FlattenPath(PoLine, Segments);
}

void PageItem_RegularPolygon::applicableActions(QStringList & actionList)
{
	actionList << "itemConvertToBezierCurve";
	actionList << "itemConvertToPolygon";
	actionList << "itemConvertToImageFrame";
	actionList << "itemConvertToTextFrame";
}

QString PageItem_RegularPolygon::infoDescription()
{
	return QString();
}
