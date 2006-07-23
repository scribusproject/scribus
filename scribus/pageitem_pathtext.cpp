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

#include "pageitem_pathtext.h"
#include "pageitem_pathtext.moc"
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
#include "commonstrings.h"
#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include "util.h"

#include "text/nlsconfig.h"

using namespace std;

PageItem_PathText::PageItem_PathText(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::PathText, x, y, w, h, w2, fill, outline)
{
}

void PageItem_PathText::DrawObj_Item(ScPainter *p, QRect /*e*/, double sc)
{
	int a;
	int chs;
	double wide;
	QString chstr, chstr2, chstr3;
	ScText *hl;
	double dx;
	double sp = 0;
	double oldSp = 0;
	double oCurX = 0;
	FPoint point = FPoint(0, 0);
	FPoint normal = FPoint(0, 0);
	FPoint tangent = FPoint(0, 0);
	FPoint extPoint = FPoint(0, 0);
	bool ext = false;
	bool first = true;
	double fsx = 0;
	uint seg = 0;
	double segLen = 0;
	double distCurX;
	CurX = Extra;
	if (!m_Doc->layerOutline(LayerNr))
	{
		if (lineColor() != CommonStrings::None && PoShow)
		{
			p->setupPolygon(&PoLine, false);
			p->strokePath();
		}
	}
	if (itemText.length() != 0)
		CurX += itemText.charStyle(0).fontSize() * itemText.charStyle(0).tracking() / 10000.0;
	segLen = PoLine.lenPathSeg(seg);
#ifndef NLS_PROTO
	for (a = 0; a < itemText.length(); ++a)
	{
		CurY = 0;
		hl = itemText.item(a);
		chstr = hl->ch;
		if ((chstr == QChar(30)) || (chstr == QChar(13)) || (chstr == QChar(9)) || (chstr == QChar(28)))
			continue;
		chs = hl->fontSize();
//		SetZeichAttr(*hl, &chs, &chstr);		//FIXME: layoutglyphs
		if (chstr == QChar(29))
			chstr2 = " ";
		else if (chstr == QChar(24))
			chstr2 = "-";
		else
			chstr2 = chstr;
		if (a < itemText.length()-1)
		{
			if (itemText.text(a+1) == QChar(29))
				chstr3 = " ";
			else if (itemText.text(a+1) == QChar(24))
				chstr3 = "-";
			else
				chstr3 = itemText.text(a+1, 1);
			wide = hl->font().charWidth(chstr2[0], chs, chstr3[0]);
		}
		else
			wide = hl->font().charWidth(chstr2[0], chs);
		wide = wide * (hl->scaleH() / 1000.0);
		dx = wide / 2.0;
		CurX += dx;
		ext = false;
		while ( (seg < PoLine.size()-3) && (CurX > fsx + segLen))
		{
			fsx += segLen;
			seg += 4;
			if (seg > PoLine.size()-3)
				break;
			segLen = PoLine.lenPathSeg(seg);
			ext = true;
		}
		if (seg > PoLine.size()-3)
			break;
		if (CurX > fsx + segLen)
			break;
		if (ext)
		{
			sp = 0;
			distCurX = PoLine.lenPathDist(seg, 0, sp);
			while (distCurX <= ((CurX - oCurX) - (fsx - oCurX)))
			{
				sp += 0.001;
				distCurX = PoLine.lenPathDist(seg, 0, sp);
			}
			PoLine.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
			CurX = (CurX - (CurX - fsx)) + distCurX;
			oldSp = sp;
			ext = false;
		}
		else
		{
			if( seg < PoLine.size()-3 )
			{
				if (CurX > fsx + segLen)
					break;
				distCurX = PoLine.lenPathDist(seg, oldSp, sp);
				while (distCurX <= (CurX - oCurX))
				{
					sp += 0.001;
					if (sp >= 1.0)
					{
						sp = 0.9999;
						break;
					}
					distCurX = PoLine.lenPathDist(seg, oldSp, sp);
				}
				PoLine.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
				CurX = oCurX + distCurX;
				oldSp = sp;
			}
			else
				break;
		}
		hl->glyph.xoffset = point.x();
		hl->glyph.yoffset = point.y();
		hl->PtransX = tangent.x();
		hl->PtransY = tangent.y();
		hl->PRot = dx;
		QWMatrix trafo = QWMatrix( 1, 0, 0, -1, -dx*sc, 0 );
		trafo *= QWMatrix( tangent.x(), tangent.y(), tangent.y(), -tangent.x(), point.x()*sc, point.y()*sc );
		QWMatrix sca = p->worldMatrix();
		trafo *= sca;
		p->save();
		QWMatrix savWM = p->worldMatrix();
		p->setWorldMatrix(trafo);
/*		Zli = new ZZ;
		Zli->Zeich = chstr;
		if (hl->fillColor() != CommonStrings::None)
		{
			QColor tmp;
			SetFarbe(&tmp, hl->fillColor(), hl->fillShade());
			p->setBrush(tmp);
		}
		if (hl->strokeColor() != CommonStrings::None)
		{
			QColor tmp;
			SetFarbe(&tmp, hl->strokeColor(), hl->strokeShade());
			p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
		}
		Zli->Farb = hl->fillColor();
		Zli->Farb2 = hl->strokeColor();
		Zli->shade = hl->fillShade();
		Zli->shade2 = hl->strokeShade();
		Zli->xco = 0;
		Zli->yco = BaseOffs;
		Zli->Sele = itemText.selected(a);
		Zli->Siz = chs;
		Zli->realSiz = hl->fontSize();
		Zli->Style = hl->effects();
		Zli->ZFo = hl->font();
		Zli->wide = wide;
		Zli->kern = hl->fontSize() * hl->tracking() / 10000.0;
		Zli->scale = hl->scaleH();
		Zli->scalev = hl->scaleV();
		Zli->base = hl->baselineOffset();
		Zli->shadowX = hl->shadowXOffset();
		Zli->shadowY = hl->shadowYOffset();
		Zli->outline = hl->outlineWidth();
		Zli->underpos = hl->underlineOffset();
		Zli->underwidth = hl->underlineWidth();
		Zli->strikepos = hl->strikethruOffset();
		Zli->strikewidth = hl->strikethruWidth();
		Zli->embedded = 0;
		if (!m_Doc->RePos)
			DrawZeichenS(p, Zli); // FIXME: drawglyphs
		delete Zli;
*/		p->setWorldMatrix(savWM);
		p->restore();
		p->setZoomFactor(sc);
		MaxChars = a+1;
		oCurX = CurX;
		CurX -= dx;
		CurX += wide+hl->fontSize() * hl->tracking() / 10000.0;
		first = false;
	}
#endif
}
