/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem_group.cpp  -  description
                             -------------------
    begin                : Sat December 4 2010
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

#include <QApplication>

#include "commonstrings.h"
#include "page.h"
#include "pageitem_group.h"
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

PageItem_Group::PageItem_Group(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::Group, x, y, w, h, 0, CommonStrings::None, CommonStrings::None)
{
}

PageItem_Group::~PageItem_Group()
{
//	while (!groupItemList.isEmpty())
//	{
//		delete groupItemList.takeFirst();
//	}
}

void PageItem_Group::adjustXYPosition()
{
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		embedded->setXYPos(xPos() + embedded->gXpos, yPos() + embedded->gYpos, true);
		if (embedded->isGroup())
			embedded->asGroupFrame()->adjustXYPosition();
	}
}

QList<PageItem*> PageItem_Group::getItemList()
{
	QList<PageItem*> ret;
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		ret.append(embedded);
		if (embedded->isGroup())
			ret += embedded->getItemList();
	}
	return ret;
}

void PageItem_Group::replaceNamedResources(ResourceCollection& newNames)
{
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		embedded->replaceNamedResources(newNames);
	}
}

void PageItem_Group::getNamedResources(ResourceCollection& lists) const
{
	for (int em = 0; em < groupItemList.count(); ++em)
	{
		PageItem* embedded = groupItemList.at(em);
		embedded->getNamedResources(lists);
	}
}

void PageItem_Group::DrawObj_Item(ScPainter *p, QRectF /*e*/)
{
	if (!m_Doc->RePos)
	{
		if (!groupItemList.isEmpty())
		{
			p->save();
			if (imageFlippedH())
			{
				p->translate(Width, 0);
				p->scale(-1, 1);
			}
			if (imageFlippedV())
			{
				p->translate(0, Height);
				p->scale(1, -1);
			}
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
			p->scale(Width / groupWidth, Height / groupHeight);
			for (int em = 0; em < groupItemList.count(); ++em)
			{
				PageItem* embedded = groupItemList.at(em);
				p->save();
				p->translate(embedded->gXpos, embedded->gYpos);
				embedded->isEmbedded = true;
				embedded->invalidateLayout();
				embedded->DrawObj(p, QRectF());
				embedded->isEmbedded = false;
				p->restore();
			}
			for (int em = 0; em < groupItemList.count(); ++em)
			{
				PageItem* embedded = groupItemList.at(em);
				if (!embedded->isTableItem)
					continue;
				p->save();
				p->translate(embedded->gXpos, embedded->gYpos);
				p->rotate(embedded->rotation());
				embedded->isEmbedded = true;
				embedded->invalidateLayout();
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
			if (m_Doc->guidesPrefs().framesShown)
			{
				for (int em = 0; em < groupItemList.count(); ++em)
				{
					PageItem* embedded = groupItemList.at(em);
					double x = embedded->xPos();
					double y = embedded->yPos();
					embedded->setXYPos(embedded->gXpos, embedded->gYpos, true);
					embedded->DrawObj_Decoration(p);
					embedded->setXYPos(x, y, true);
				}
			}
		}
		else
		{
			if (m_Doc->guidesPrefs().framesShown)
			{
				p->save();
				p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				p->drawLine(FPoint(0, 0), FPoint(Width, Height));
				p->drawLine(FPoint(0, Height), FPoint(Width, 0));
				p->setFont(QApplication::font());
				p->drawLine(FPoint(0, 0), FPoint(Width, 0));
				p->drawLine(FPoint(Width, 0), FPoint(Width, Height));
				p->drawLine(FPoint(Width, Height), FPoint(0, Height));
				p->drawLine(FPoint(0, Height), FPoint(0, 0));
				p->setBrush(QColor(255, 255, 255));
				p->setBrushOpacity(0.0);
				p->drawText(QRectF(0.0, 0.0, Width, Height), "Empty Group");
				p->restore();
			}
		}
	}
}

void PageItem_Group::applicableActions(QStringList & actionList)
{
}

QString PageItem_Group::infoDescription()
{
	return QString();
}
