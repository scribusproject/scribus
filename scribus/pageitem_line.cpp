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

void PageItem_Line::DrawObj_Item(ScPainter *p, QRectF /*e*/)
{
	if (!m_Doc->RePos)
	{
		if (m_Doc->layerOutline(LayerID))
			p->drawLine(FPoint(0, 0), FPoint(Width, 0));
		else
		{
			if (NamedLStyle.isEmpty())
			{
				if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)))
				{
					if (patternStrokePath)
					{
						QPainterPath guidePath;
						guidePath.moveTo(0, 0);
						guidePath.lineTo(Width, 0);
						DrawStrokePattern(p, guidePath);
					}
					else
					{
						p->setPattern(&m_Doc->docPatterns[patternStrokeVal], patternStrokeScaleX, patternStrokeScaleY, patternStrokeOffsetX, patternStrokeOffsetY, patternStrokeRotation, patternStrokeSkewX, patternStrokeSkewY, patternStrokeMirrorX, patternStrokeMirrorY);
						p->setStrokeMode(ScPainter::Pattern);
						p->drawLine(FPoint(0, 0), FPoint(Width, 0));
					}
				}
				else if (GrTypeStroke > 0)
				{
					if ((!gradientStrokeVal.isEmpty()) && (!m_Doc->docGradients.contains(gradientStrokeVal)))
						gradientStrokeVal = "";
					if (!(gradientStrokeVal.isEmpty()) && (m_Doc->docGradients.contains(gradientStrokeVal)))
						stroke_gradient = m_Doc->docGradients[gradientStrokeVal];
					if (stroke_gradient.Stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
					{
						if (lineColor() != CommonStrings::None)
						{
							p->setBrush(strokeQColor);
							p->setStrokeMode(ScPainter::Solid);
						}
						else
							p->setStrokeMode(ScPainter::None);
					}
					else
					{
						p->setStrokeMode(ScPainter::Gradient);
						p->stroke_gradient = stroke_gradient;
						if (GrTypeStroke == 6)
							p->setGradient(VGradient::linear, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeStartX, GrStrokeStartY), GrStrokeScale, GrStrokeSkew);
						else
							p->setGradient(VGradient::radial, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeFocalX, GrStrokeFocalY), GrStrokeScale, GrStrokeSkew);
					}
					p->drawLine(FPoint(0, 0), FPoint(Width, 0));
				}
				else if (lineColor() != CommonStrings::None)
				{
					p->setStrokeMode(ScPainter::Solid);
					p->drawLine(FPoint(0, 0), FPoint(Width, 0));
				}
			}
			else
			{
				p->setStrokeMode(ScPainter::Solid);
				multiLine ml = m_Doc->MLineStyles[NamedLStyle];
				QColor tmp;
				for (int it = ml.size()-1; it > -1; it--)
				{
					if (ml[it].Color != CommonStrings::None) // && (ml[it].Width != 0))
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
			QTransform arrowTrans;
			arrowTrans.translate(0, 0);
			arrowTrans.scale(-1,1);
			arrowTrans.scale(m_startArrowScale / 100.0, m_startArrowScale / 100.0);
			drawArrow(p, arrowTrans, m_startArrowIndex);
		}
		if (m_endArrowIndex != 0)
		{
			QTransform arrowTrans;
			arrowTrans.translate(Width, 0);
			arrowTrans.scale(m_endArrowScale / 100.0, m_endArrowScale / 100.0);
			drawArrow(p, arrowTrans, m_endArrowIndex);
		}
	}
}

void PageItem_Line::applicableActions(QStringList & actionList)
{
	actionList << "itemConvertToBezierCurve";
}

QString PageItem_Line::infoDescription()
{
	return QString();
}
