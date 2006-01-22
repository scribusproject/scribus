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

#include "pageitem_imageframe.h"
#include "pageitem_imageframe.moc"
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

PageItem_ImageFrame::PageItem_ImageFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::ImageFrame, x, y, w, h, w2, fill, outline)
{
}

void PageItem_ImageFrame::DrawObj_Item(ScPainter *p, double sc)
{
	if(!m_Doc->RePos)
	{
		if ((fillColor() != "None") || (GrType != 0))
		{
			p->setupPolygon(&PoLine);
			p->fillPath();
		}
		if (Pfile.isEmpty())
		{
			if ((Frame) && (m_Doc->guidesSettings.framesShown))
			{
				p->setPen(black, 1, SolidLine, FlatCap, MiterJoin);
				p->drawLine(FPoint(0, 0), FPoint(Width, Height));
				p->drawLine(FPoint(0, Height), FPoint(Width, 0));
			}
		}
		else
		{
			if ((!PicArt) || (!PicAvail))
			{
				if ((Frame) && (m_Doc->guidesSettings.framesShown))
				{
					p->setPen(red, 1, SolidLine, FlatCap, MiterJoin);
					p->drawLine(FPoint(0, 0), FPoint(Width, Height));
					p->drawLine(FPoint(0, Height), FPoint(Width, 0));
				}
			}
			else
			{
				p->save();
				if (imageClip.size() != 0)
					p->setupPolygon(&imageClip);
				else
					p->setupPolygon(&PoLine);
				p->setClipPath();
				if (imageFlippedH())
				{
					p->translate(Width * sc, 0);
					p->scale(-1, 1);
				}
				if (imageFlippedV())
				{
					p->translate(0, Height * sc);
					p->scale(1, -1);
				}
				p->translate(LocalX*LocalScX*sc, LocalY*LocalScY*sc);
				p->scale(LocalScX, LocalScY);
				if (pixm.imgInfo.lowResType != 0)
					p->scale(pixm.imgInfo.lowResScale, pixm.imgInfo.lowResScale);
				p->drawImage(&pixm);
				p->restore();
			}
		}
	}
}

void PageItem_ImageFrame::clearContents()
{
	effectsInUse.clear();
	PicAvail = false;
	Pfile = "";
	pixm = ScImage();
	
	LocalScX = 1;
	LocalScY = 1;
	OrigW = 0;
	OrigH = 0;
	LocalX = 0;
	LocalY = 0;
	setImageFlippedH(false);
	setImageFlippedV(false);
	textAlignment = 0;
	EmProfile = "";
	ScaleType = true;
	AspectRatio = true;
	setFillTransparency(0.0);
	setLineTransparency(0.0);
	imageClip.resize(0);
	//				emit UpdtObj(Doc->currentPage->pageNr(), ItemNr); 
}

