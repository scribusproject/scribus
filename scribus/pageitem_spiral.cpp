/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem_spiral.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2011
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

#include "commonstrings.h"
#include "page.h"
#include "pageitem.h"
#include "pageitem_spiral.h"
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

PageItem_Spiral::PageItem_Spiral(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::Spiral, x, y, w, h, w2, fill, outline)
{
	spiralStartAngle = m_Doc->itemToolPrefs().spiralStartAngle;
	spiralEndAngle = m_Doc->itemToolPrefs().spiralEndAngle;
	spiralFactor = m_Doc->itemToolPrefs().spiralFactor;
	recalcPath();
}

void PageItem_Spiral::DrawObj_Item(ScPainter *p, QRectF /*e*/)
{
	if (!m_Doc->RePos && PoLine.size()>=4)
	{
		if (!m_Doc->layerOutline(LayerID))
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
				if ((!patternStrokeVal.isEmpty()) && (m_Doc->docPatterns.contains(patternStrokeVal)))
				{
					if (patternStrokePath)
					{
						QPainterPath guidePath = PoLine.toQPainterPath(false);
						DrawStrokePattern(p, guidePath);
					}
					else
					{
						p->setPattern(&m_Doc->docPatterns[patternStrokeVal], patternStrokeScaleX, patternStrokeScaleY, patternStrokeOffsetX, patternStrokeOffsetY, patternStrokeRotation, patternStrokeSkewX, patternStrokeSkewY, patternStrokeMirrorX, patternStrokeMirrorY);
						p->setStrokeMode(ScPainter::Pattern);
						p->strokePath();
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
					p->strokePath();
				}
				else if (lineColor() != CommonStrings::None)
				{
					p->setStrokeMode(ScPainter::Solid);
					p->strokePath();
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
					QTransform arrowTrans;
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(m_startArrowScale / 100.0, m_startArrowScale / 100.0);
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
					QTransform arrowTrans;
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(m_endArrowScale / 100.0, m_endArrowScale / 100.0);
					drawArrow(p, arrowTrans, m_endArrowIndex);
					break;
				}
			}
		}
	}
}

void PageItem_Spiral::recalcPath()
{
	if (spiralStartAngle >= spiralEndAngle)
		return;
	double startAngleK = spiralStartAngle;
	double endAngleK = spiralEndAngle;
	double spiralHeight = height();
	double spiralWidth = width();
	QPainterPath path, path2;
	double sh = spiralHeight / (spiralFactor + 1.0);
	double sw = 0.0;
	double ww = spiralWidth;
	double hh = spiralHeight - sh;
	double segStart = 0.0;
	double segEnd = 180.0;
	double spanAngle = 180.0;
	double startAngle = 0.0;
	bool segPart = true;
	bool draw = false;
	QPointF tp;
	path2.moveTo(sw, sh);
	while (segStart < endAngleK)
	{
		if (startAngleK >= segEnd)
		{
			tp = path2.currentPosition();
			if (segPart)
			{
				sw = tp.x();
				sh = spiralHeight / (spiralFactor + 1.0);
				path2.arcTo(sw, sh - hh, ww, hh * 2, 180, 180);
			}
			else
			{
				sw = tp.x() - ww;
				sh = spiralHeight / (spiralFactor + 1.0) - hh;
				path2.arcTo(sw, sh, ww, hh * 2, 0, 180);
			}
			segPart = !segPart;
			ww /= spiralFactor;
			hh /= spiralFactor;
			segStart += 180.0;
			segEnd += 180.0;
			spanAngle = 180.0;
			continue;
		}
		if ((startAngleK >= segStart) && (startAngleK <= segEnd))
		{
			startAngle = startAngleK + 180;
			spanAngle = segEnd - startAngleK;
			if ((endAngleK >= segStart) && (endAngleK <= segEnd))
				spanAngle -= segEnd - endAngleK;
			tp = path2.currentPosition();
			if (segPart)
			{
				sw = tp.x();
				sh = spiralHeight / (spiralFactor + 1.0);
				path.arcMoveTo(sw, sh - hh, ww, hh * 2, startAngle);
				path.arcTo(sw, sh - hh, ww, hh * 2, startAngle, spanAngle);
				startAngle = 0.0;
			}
			else
			{
				sw = tp.x() - ww;
				sh = spiralHeight / (spiralFactor + 1.0) - hh;
				path.arcMoveTo(sw, sh, ww, hh * 2, startAngle);
				path.arcTo(sw, sh, ww, hh * 2, startAngle, spanAngle);
				startAngle = 180.0;
			}
			draw = true;
			segPart = !segPart;
			ww /= spiralFactor;
			hh /= spiralFactor;
			if ((endAngleK >= segStart) && (endAngleK <= segEnd))
				break;
			segStart += 180.0;
			segEnd += 180.0;
			spanAngle = 180.0;
			continue;
		}
		if ((endAngleK >= segStart) && (endAngleK <= segEnd))
			spanAngle -= segEnd - endAngleK;
		tp = path.currentPosition();
		if (segPart)
		{
			sw = tp.x();
			sh = spiralHeight / (spiralFactor + 1.0);
			if (draw)
				path.arcTo(sw, sh - hh, ww, hh * 2, startAngle, spanAngle);
			startAngle = 0.0;
		}
		else
		{
			sw = tp.x() - ww;
			sh = spiralHeight / (spiralFactor + 1.0) - hh;
			if (draw)
				path.arcTo(sw, sh, ww, hh * 2, startAngle, spanAngle);
			startAngle = 180.0;
		}
		segPart = !segPart;
		ww /= spiralFactor;
		hh /= spiralFactor;
		if ((endAngleK >= segStart) && (endAngleK <= segEnd))
			break;
		segStart += 180.0;
		segEnd += 180.0;
		spanAngle = 180.0;
	}
	PoLine.fromQPainterPath(path);
	if (imageFlippedV() || imageFlippedH())
	{
		if (imageFlippedV())
		{
			QTransform ma;
			ma.scale(1, -1);
			PoLine.map(ma);
			PoLine.translate(0, height());
		}
		if (imageFlippedH())
		{
			QTransform ma;
			ma.scale(-1, 1);
			PoLine.map(ma);
			PoLine.translate(width(), 0);
		}
	}
	Clip = FlattenPath(PoLine, Segments);
}

void PageItem_Spiral::applicableActions(QStringList & actionList)
{
	actionList << "itemConvertToBezierCurve";
	actionList << "itemConvertToPolygon";
}

QString PageItem_Spiral::infoDescription()
{
	return QString();
}
