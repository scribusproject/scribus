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

#include "pageitem_line.h"
#include "pageitem_line.moc"
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

PageItem_Line::PageItem_Line(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::Line, x, y, w, h, w2, fill, outline)
{
}

void PageItem_Line::DrawObj_Item(ScPainter *p)
{
	if (!m_Doc->RePos)
	{
		if (NamedLStyle.isEmpty())
			p->drawLine(FPoint(0, 0), FPoint(Width, 0));
		else
		{
			multiLine ml = m_Doc->MLineStyles[NamedLStyle];
			QColor tmp;
			for (int it = ml.size()-1; it > -1; it--)
			{
				SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
				p->setPen(tmp, ml[it].Width,
							static_cast<PenStyle>(ml[it].Dash),
							static_cast<PenCapStyle>(ml[it].LineEnd),
							static_cast<PenJoinStyle>(ml[it].LineJoin));
				p->drawLine(FPoint(0, 0), FPoint(Width, 0));
			}
		}
		if (m_startArrowIndex != 0)
		{
			QWMatrix arrowTrans;
			FPointArray arrow = (*m_Doc->arrowStyles.at(m_startArrowIndex-1)).points.copy();
			arrowTrans.translate(0, 0);
			arrowTrans.scale(Pwidth, Pwidth);
			arrowTrans.scale(-1,1);
			arrow.map(arrowTrans);
			p->setBrush(p->pen());
			p->setBrushOpacity(1.0 - lineTransparency());
			p->setLineWidth(0);
			p->setFillMode(ScPainter::Solid);
			p->setupPolygon(&arrow);
			p->fillPath();
		}
		if (m_endArrowIndex != 0)
		{
			QWMatrix arrowTrans;
			FPointArray arrow = (*m_Doc->arrowStyles.at(m_endArrowIndex-1)).points.copy();
			arrowTrans.translate(Width, 0);
			arrowTrans.scale(Pwidth, Pwidth);
			arrow.map(arrowTrans);
			p->setBrush(p->pen());
			p->setBrushOpacity(1.0 - lineTransparency());
			p->setLineWidth(0);
			p->setFillMode(ScPainter::Solid);
			p->setupPolygon(&arrow);
			p->fillPath();
		}
	}
}

