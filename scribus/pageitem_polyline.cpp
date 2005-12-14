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

#include "pageitem_polyline.h"
#include "pageitem_polyline.moc"
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

PageItem_PolyLine::PageItem_PolyLine(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::PolyLine, x, y, w, h, w2, fill, outline)
{
}

void PageItem_PolyLine::DrawObj_Item(ScPainter *p)
{
	if (!m_Doc->RePos && PoLine.size()>=4)
	{
		if ((fillColor() != "None") || (GrType != 0))
		{
			FPointArray cli;
			FPoint Start;
			bool firstp = true;
			for (uint n = 0; n < PoLine.size()-3; n += 4)
			{
				if (firstp)
				{
					Start = PoLine.point(n);
					firstp = false;
				}
				if (PoLine.point(n).x() > 900000)
				{
					cli.addPoint(PoLine.point(n-2));
					cli.addPoint(PoLine.point(n-2));
					cli.addPoint(Start);
					cli.addPoint(Start);
					cli.setMarker();
					firstp = true;
					continue;
				}
				cli.addPoint(PoLine.point(n));
				cli.addPoint(PoLine.point(n+1));
				cli.addPoint(PoLine.point(n+2));
				cli.addPoint(PoLine.point(n+3));
			}
			if (cli.size() > 2)
			{
				FPoint l1 = cli.point(cli.size()-2);
				cli.addPoint(l1);
				cli.addPoint(l1);
				cli.addPoint(Start);
				cli.addPoint(Start);
			}
			p->setupPolygon(&cli);
			p->fillPath();
		}
		p->setupPolygon(&PoLine, false);
		if (NamedLStyle.isEmpty())
			p->strokePath();
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
				p->strokePath();
			}
		}
		if (m_startArrowIndex != 0)
		{
			FPoint Start = PoLine.point(0);
			for (uint xx = 1; xx < PoLine.size(); xx += 2)
			{
				FPoint Vector = PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
					QWMatrix arrowTrans;
					FPointArray arrow = (*m_Doc->arrowStyles.at(m_startArrowIndex-1)).points.copy();
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(Pwidth, Pwidth);
					arrow.map(arrowTrans);
					p->setBrush(p->pen());
					p->setBrushOpacity(1.0 - lineTransparency());
					p->setLineWidth(0);
					p->setFillMode(ScPainter::Solid);
					p->setupPolygon(&arrow);
					p->fillPath();
					break;
				}
			}
		}
		if (m_endArrowIndex != 0)
		{
			FPoint End = PoLine.point(PoLine.size()-2);
			for (uint xx = PoLine.size()-1; xx > 0; xx -= 2)
			{
				FPoint Vector = PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
					QWMatrix arrowTrans;
					FPointArray arrow = (*m_Doc->arrowStyles.at(m_endArrowIndex-1)).points.copy();
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(Pwidth, Pwidth);
					arrow.map(arrowTrans);
					p->setBrush(p->pen());
					p->setBrushOpacity(1.0 - lineTransparency());
					p->setLineWidth(0);
					p->setFillMode(ScPainter::Solid);
					p->setupPolygon(&arrow);
					p->fillPath();
					break;
				}
			}
		}
	}
}

