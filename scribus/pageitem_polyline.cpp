/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <cassert>

#include "page.h"
#include "pageitem.h"
#include "pageitem_polyline.h"
#include "prefsmanager.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "commonstrings.h"
#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include "util.h"
#include "util_math.h"

using namespace std;

PageItem_PolyLine::PageItem_PolyLine(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::PolyLine, x, y, w, h, w2, fill, outline)
{
}

void PageItem_PolyLine::DrawObj_Item(ScPainter *p, QRectF /*e*/, double /*sc*/)
{
	if (!m_Doc->RePos && PoLine.size()>=4)
	{
		if (!m_Doc->layerOutline(LayerNr))
		{
			if ((fillColor() != CommonStrings::None) || (GrType != 0))
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
			{
				if (lineColor() != CommonStrings::None)
					p->strokePath();
			}
			else
			{
				multiLine ml = m_Doc->MLineStyles[NamedLStyle];
				QColor tmp;
				for (int it = ml.size()-1; it > -1; it--)
				{
					if (ml[it].Color != CommonStrings::None) // && (ml[it].Width != 0))
					{
						SetQColor(&tmp, ml[it].Color, ml[it].Shade);
						p->setPen(tmp, ml[it].Width, static_cast<Qt::PenStyle>(ml[it].Dash), static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						p->strokePath();
					}
				}
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
					QMatrix arrowTrans;
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
					drawArrow(p, arrowTrans, m_startArrowIndex);
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
					QMatrix arrowTrans;
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(r);
					drawArrow(p, arrowTrans, m_endArrowIndex);
					break;
				}
			}
		}
	}
}

void PageItem_PolyLine::applicableActions(QStringList & actionList)
{
	actionList << "itemConvertToPolygon";
}

QString PageItem_PolyLine::infoDescription()
{
	return QString();
}

