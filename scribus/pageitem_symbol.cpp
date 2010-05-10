/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem_symbol.cpp  -  description
                             -------------------
    begin                : Sat May 8 2010
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

#include <cmath>
#include <cassert>

#include "commonstrings.h"
#include "page.h"
#include "pageitem.h"
#include "pageitem_symbol.h"
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

using namespace std;

PageItem_Symbol::PageItem_Symbol(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::Symbol, x, y, w, h, 0, CommonStrings::None, CommonStrings::None)
{
}

void PageItem_Symbol::DrawObj_Item(ScPainter *p, QRectF /*e*/)
{
	if (!m_Doc->RePos)
	{
		if ((!patternVal.isEmpty()) && (m_Doc->docPatterns.contains(patternVal)))
		{
			p->save();
			if ((maskType() == 1) || (maskType() == 2) || (maskType() == 4) || (maskType() == 5))
			{
				if ((maskType() == 1) || (maskType() == 2))
					p->setMaskMode(1);
				else
					p->setMaskMode(3);
				if ((!gradientMask().isEmpty()) && (!m_Doc->docGradients.contains(gradientMask())))
					gradientMaskVal = "";
				if (!(gradientMask().isEmpty()) && (m_Doc->docGradients.contains(gradientMask())))
					mask_gradient = m_Doc->docGradients[gradientMask()];
				p->mask_gradient = mask_gradient;
				if ((maskType() == 1) || (maskType() == 4))
					p->setGradientMask(VGradient::linear, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskStartX, GrMaskStartY), GrMaskScale, GrMaskSkew);
				else
					p->setGradientMask(VGradient::radial, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskFocalX, GrMaskFocalY), GrMaskScale, GrMaskSkew);
			}
			else if ((maskType() == 3) || (maskType() == 6))
			{
				if ((patternMask().isEmpty()) || (!m_Doc->docPatterns.contains(patternMask())))
					p->setMaskMode(0);
				else
				{
					p->setPatternMask(&m_Doc->docPatterns[patternMask()], patternMaskScaleX, patternMaskScaleY, patternMaskOffsetX + xPos(), patternMaskOffsetY + yPos(), patternMaskRotation, patternMaskSkewX, patternMaskSkewY, patternMaskMirrorX, patternMaskMirrorY);
					if (maskType() == 3)
						p->setMaskMode(2);
					else
						p->setMaskMode(4);
				}
			}
			else
				p->setMaskMode(0);
			p->beginLayer(1.0 - fillTransparency(), fillBlendmode(), &PoLine);
			p->setMaskMode(0);
			ScPattern pat = m_Doc->docPatterns[patternVal];
			p->scale(Width / pat.width, Height / pat.height);
			p->translate(pat.items.at(0)->gXpos, pat.items.at(0)->gYpos);
			QStack<PageItem*> groupStack;
			groupStack.clear();
			for (int em = 0; em < pat.items.count(); ++em)
			{
				PageItem* embedded = pat.items.at(em);
				if (embedded->isGroupControl)
				{
					p->save();
					QTransform mm;
					mm.translate(embedded->gXpos, embedded->gYpos);
					mm.rotate(embedded->rotation());
					if ((embedded->GrMask == 1) || (embedded->GrMask == 2) || (embedded->GrMask == 4) || (embedded->GrMask == 5))
					{
						if ((embedded->GrMask == 1) || (embedded->GrMask == 2))
							p->setMaskMode(1);
						else
							p->setMaskMode(3);
						if ((!embedded->gradientMaskVal.isEmpty()) && (!m_Doc->docGradients.contains(embedded->gradientMaskVal)))
							embedded->gradientMaskVal = "";
						if (!(embedded->gradientMaskVal.isEmpty()) && (m_Doc->docGradients.contains(embedded->gradientMaskVal)))
							embedded->mask_gradient = m_Doc->docGradients[embedded->gradientMaskVal];
						p->mask_gradient = embedded->mask_gradient;
						if ((embedded->GrMask == 1) || (embedded->GrMask == 4))
							p->setGradientMask(VGradient::linear, FPoint(embedded->GrMaskStartX, embedded->GrMaskStartY).transformPoint(mm, false), FPoint(embedded->GrMaskEndX, embedded->GrMaskEndY).transformPoint(mm, false), FPoint(embedded->GrMaskStartX, embedded->GrMaskStartY).transformPoint(mm, false), embedded->GrMaskScale, embedded->GrMaskSkew);
						else
							p->setGradientMask(VGradient::radial, FPoint(embedded->GrMaskStartX, embedded->GrMaskStartY).transformPoint(mm, false), FPoint(embedded->GrMaskEndX, embedded->GrMaskEndY).transformPoint(mm, false), FPoint(embedded->GrMaskFocalX, embedded->GrMaskFocalY).transformPoint(mm, false), embedded->GrMaskScale, embedded->GrMaskSkew);
					}
					else if ((embedded->GrMask == 3) || (embedded->GrMask == 6))
					{
						if ((embedded->patternMaskVal.isEmpty()) || (!m_Doc->docPatterns.contains(embedded->patternMaskVal)))
							p->setMaskMode(0);
						else
						{
							p->setPatternMask(&m_Doc->docPatterns[embedded->patternMask()], embedded->patternMaskScaleX, embedded->patternMaskScaleY, embedded->patternMaskOffsetX + embedded->xPos(), embedded->patternMaskOffsetY + embedded->yPos(), embedded->patternMaskRotation, embedded->patternMaskSkewX, embedded->patternMaskSkewY, embedded->patternMaskMirrorX, embedded->patternMaskMirrorY);
							if (embedded->GrMask == 3)
								p->setMaskMode(2);
							else
								p->setMaskMode(4);
						}
					}
					else
						p->setMaskMode(0);
					FPointArray cl = embedded->PoLine.copy();
					cl.map( mm );
					p->beginLayer(1.0 - embedded->fillTransparency(), embedded->fillBlendmode(), &cl);
					groupStack.push(embedded->groupsLastItem);
					continue;
				}
				p->save();
				p->translate(embedded->gXpos, embedded->gYpos);
				embedded->isEmbedded = true;
				embedded->invalid = true;
				embedded->DrawObj(p, QRectF());
				embedded->isEmbedded = false;
				p->restore();
				if (groupStack.count() != 0)
				{
					while (embedded == groupStack.top())
					{
						p->endLayer();
						p->restore();
						groupStack.pop();
						if (groupStack.count() == 0)
							break;
					}
				}
			}
			for (int em = 0; em < pat.items.count(); ++em)
			{
				PageItem* embedded = pat.items.at(em);
				if (!embedded->isTableItem)
					continue;
				p->save();
				p->translate(embedded->gXpos, embedded->gYpos);
				p->rotate(embedded->rotation());
				embedded->isEmbedded = true;
				embedded->invalid = true;
				if ((embedded->lineColor() != CommonStrings::None) && (embedded->lineWidth() != 0.0))
				{
					QColor tmp;
					embedded->SetQColor(&tmp, embedded->lineColor(), embedded->lineShade());
					if ((embedded->TopLine) || (embedded->RightLine) || (embedded->BottomLine) || (embedded->LeftLine))
					{
						p->setPen(tmp, embedded->lineWidth(), embedded->PLineArt, Qt::SquareCap, embedded->PLineJoin);
						if (embedded->TopLine)
							p->drawLine(FPoint(0.0, 0.0), FPoint(embedded->width(), 0.0));
						if (embedded->RightLine)
							p->drawLine(FPoint(embedded->width(), 0.0), FPoint(embedded->width(), embedded->height()));
						if (embedded->BottomLine)
							p->drawLine(FPoint(embedded->width(), embedded->height()), FPoint(0.0, embedded->height()));
						if (embedded->LeftLine)
							p->drawLine(FPoint(0.0, embedded->height()), FPoint(0.0, 0.0));
					}
				}
				embedded->isEmbedded = false;
				p->restore();
			}
			p->endLayer();
			p->restore();
		}
	}
}

void PageItem_Symbol::applicableActions(QStringList & actionList)
{
}

QString PageItem_Symbol::infoDescription()
{
	return QString();
}
