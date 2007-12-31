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

#include <cmath>
#include <cassert>

#include "page.h"
#include "pageitem.h"
#include "pageitem_line.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scpainter.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "commonstrings.h"

#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include "util.h"

using namespace std;

PageItem_Line::PageItem_Line(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::Line, x, y, w, h, w2, fill, outline)
{
}

void PageItem_Line::DrawObj_Item(ScPainter *p, QRectF /*e*/, double /*sc*/)
{
	if (!m_Doc->RePos)
	{
		if (m_Doc->layerOutline(LayerNr))
			p->drawLine(FPoint(0, 0), FPoint(Width, 0));
		else
		{
			if (NamedLStyle.isEmpty())
				p->drawLine(FPoint(0, 0), FPoint(Width, 0));
			else
			{
				multiLine ml = m_Doc->MLineStyles[NamedLStyle];
				QColor tmp;
				for (int it = ml.size()-1; it > -1; it--)
				{
					if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
					{
						SetQColor(&tmp, ml[it].Color, ml[it].Shade);
						p->setPen(tmp, ml[it].Width, static_cast<Qt::PenStyle>(ml[it].Dash), static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						p->drawLine(FPoint(0, 0), FPoint(Width, 0));
					}
				}
			}
		}
		if (m_startArrowIndex != 0)
		{
			QMatrix arrowTrans;
			FPointArray arrow = m_Doc->arrowStyles.at(m_startArrowIndex-1).points.copy();
			arrowTrans.translate(0, 0);
			arrowTrans.scale(m_lineWidth, m_lineWidth);
			arrowTrans.scale(-1,1);
			arrow.map(arrowTrans);
			p->setupPolygon(&arrow);
			if (m_Doc->layerOutline(LayerNr))
				p->strokePath();
			else
			{
				p->setBrush(p->pen());
				p->setBrushOpacity(1.0 - lineTransparency());
				p->setLineWidth(0);
				p->setFillMode(ScPainter::Solid);
				p->fillPath();
			}
		}
		if (m_endArrowIndex != 0)
		{
			QMatrix arrowTrans;
			FPointArray arrow = m_Doc->arrowStyles.at(m_endArrowIndex-1).points.copy();
			arrowTrans.translate(Width, 0);
			arrowTrans.scale(m_lineWidth, m_lineWidth);
			arrow.map(arrowTrans);
			p->setupPolygon(&arrow);
			if (m_Doc->layerOutline(LayerNr))
				p->strokePath();
			else
			{
				p->setBrush(p->pen());
				p->setBrushOpacity(1.0 - lineTransparency());
				p->setLineWidth(0);
				p->setFillMode(ScPainter::Solid);
				p->fillPath();
			}
		}
	}
}

void PageItem_Line::applicableActions(QStringList & actionList)
{
}

QString PageItem_Line::infoDescription()
{
	return QString();
}
