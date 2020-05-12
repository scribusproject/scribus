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

#include <QApplication>
#include <QFontInfo>

#include "commonstrings.h"
#include "pageitem.h"
#include "pageitem_symbol.h"
#include "prefsmanager.h"
#include "scpage.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scraction.h"

#include "scribusstructs.h"
#include "scribusdoc.h"

#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include "util.h"

using namespace std;

PageItem_Symbol::PageItem_Symbol(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline)
	: PageItem(pa, PageItem::Symbol, x, y, w, h, 0, CommonStrings::None, CommonStrings::None)
{
}

void PageItem_Symbol::DrawObj_Item(ScPainter *p, QRectF /*e*/)
{
	if (m_Doc->RePos)
		return;
	if (!m_Doc->docPatterns.contains(m_patternName))
		m_patternName.clear();
	if (m_patternName.isEmpty())
	{
		if (m_Doc->guidesPrefs().framesShown)
		{
			p->save();
			p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->drawLine(FPoint(0, 0), FPoint(m_width, m_height));
			p->drawLine(FPoint(0, m_height), FPoint(m_width, 0));
			const QFont &font = QApplication::font();
			p->setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts.findFont(font.family(), QFontInfo(font).styleName()), font.pointSizeF());
			p->drawLine(FPoint(0, 0), FPoint(m_width, 0));
			p->drawLine(FPoint(m_width, 0), FPoint(m_width, m_height));
			p->drawLine(FPoint(m_width, m_height), FPoint(0, m_height));
			p->drawLine(FPoint(0, m_height), FPoint(0, 0));
			p->setBrush(QColor(255, 255, 255));
			p->setBrushOpacity(0.0);
			p->drawText(QRectF(0.0, 0.0, m_width, m_height), "Empty Symbol");
			p->restore();
		}
		return;
	}

	p->save();
	if (imageFlippedH())
	{
		p->translate(m_width, 0);
		p->scale(-1, 1);
	}
	if (imageFlippedV())
	{
		p->translate(0, m_height);
		p->scale(1, -1);
	}
	if ((maskType() == 1) || (maskType() == 2) || (maskType() == 4) || (maskType() == 5))
	{
		if ((maskType() == 1) || (maskType() == 2))
			p->setMaskMode(1);
		else
			p->setMaskMode(3);
		if ((!gradientMask().isEmpty()) && (!m_Doc->docGradients.contains(gradientMask())))
			gradientMaskVal.clear();
		if (!(gradientMask().isEmpty()) && (m_Doc->docGradients.contains(gradientMask())))
			mask_gradient = m_Doc->docGradients[gradientMask()];
		p->mask_gradient = mask_gradient;
		if ((maskType() == 1) || (maskType() == 4))
			p->setGradientMask(VGradient::linear, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskStartX, GrMaskStartY), GrMaskScale, GrMaskSkew);
		else
			p->setGradientMask(VGradient::radial, FPoint(GrMaskStartX, GrMaskStartY), FPoint(GrMaskEndX, GrMaskEndY), FPoint(GrMaskFocalX, GrMaskFocalY), GrMaskScale, GrMaskSkew);
	}
	else if ((maskType() == 3) || (maskType() == 6) || (maskType() == 7) || (maskType() == 8))
	{
		if ((patternMask().isEmpty()) || (!m_Doc->docPatterns.contains(patternMask())))
			p->setMaskMode(0);
		else
		{
			double scw = m_width / groupWidth;
			double sch = m_height / groupHeight;
			p->setPatternMask(&m_Doc->docPatterns[patternMask()], patternMaskScaleX * scw, patternMaskScaleY * sch, patternMaskOffsetX, patternMaskOffsetY, patternMaskRotation, patternMaskSkewX, patternMaskSkewY, patternMaskMirrorX, patternMaskMirrorY);
			if (maskType() == 3)
				p->setMaskMode(2);
			else if (maskType() == 6)
				p->setMaskMode(4);
			else if (maskType() == 7)
				p->setMaskMode(5);
			else
				p->setMaskMode(6);
		}
	}
	else
		p->setMaskMode(0);
	p->setFillRule(fillRule);
	p->beginLayer(1.0 - fillTransparency(), fillBlendmode(), &PoLine);
	p->setMaskMode(0);
	ScPattern pat = m_Doc->docPatterns[m_patternName];
	p->scale(m_width / pat.width, m_height / pat.height);
//		p->translate(pat.items.at(0)->gXpos, pat.items.at(0)->gYpos);
	for (int em = 0; em < pat.items.count(); ++em)
	{
		PageItem* embedded = pat.items.at(em);
		p->save();
		p->translate(embedded->gXpos, embedded->gYpos);
		embedded->isEmbedded = true;
		embedded->invalid = true;
		embedded->DrawObj(p, QRectF());
		embedded->isEmbedded = false;
		p->restore();
	}
	p->endLayer();
	p->restore();
	if (m_Doc->layerOutline(m_layerID))
	{
		p->setPen(m_Doc->layerMarker(m_layerID), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::None);
		p->setBrushOpacity(1.0);
		p->setPenOpacity(1.0);
		p->setupPolygon(&PoLine);
		p->strokePath();
	}
}

void PageItem_Symbol::applicableActions(QStringList & actionList)
{
}

QString PageItem_Symbol::infoDescription() const
{
	return QString();
}
