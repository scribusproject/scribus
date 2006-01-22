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

#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include <ft2build.h>
#include FT_GLYPH_H

#include "scfontmetrics.h"
#include "util.h"

using namespace std;

PageItem_PathText::PageItem_PathText(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::PathText, x, y, w, h, w2, fill, outline)
{
}

void PageItem_PathText::DrawObj_Item(ScPainter *p, double sc)
{
	uint a;
	int chs;
	double wide;
	QString chx, chx2, chx3;
	struct ScText *hl;
	struct ZZ *Zli;
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
	if (lineColor() != "None" && PoShow)
	{
		p->setupPolygon(&PoLine, false);
		p->strokePath();
	}
	if (itemText.count() != 0)
		CurX += itemText.at(0)->csize * itemText.at(0)->cextra / 10000.0;
	segLen = PoLine.lenPathSeg(seg);
	for (a = 0; a < itemText.count(); ++a)
	{
		CurY = 0;
		hl = itemText.at(a);
		chx = hl->ch;
		if ((chx == QChar(30)) || (chx == QChar(13)) || (chx == QChar(9)) || (chx == QChar(28)))
			continue;
		chs = hl->csize;
		SetZeichAttr(hl, &chs, &chx);
		if (chx == QChar(29))
			chx2 = " ";
		else if (chx == QChar(24))
			chx2 = "-";
		else
			chx2 = chx;
		if (a < itemText.count()-1)
		{
			if (itemText.at(a+1)->ch == QChar(29))
				chx3 = " ";
			else if (itemText.at(a+1)->ch == QChar(24))
				chx3 = "-";
			else
				chx3 = itemText.at(a+1)->ch;
			wide = Cwidth(m_Doc, hl->cfont, chx2, chs, chx3);
		}
		else
			wide = Cwidth(m_Doc, hl->cfont, chx2, chs);
		wide = wide * (hl->cscale / 1000.0);
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
		hl->xp = point.x();
		hl->yp = point.y();
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
		Zli = new ZZ;
		Zli->Zeich = chx;
		if (hl->ccolor != "None")
		{
			QColor tmp;
			SetFarbe(&tmp, hl->ccolor, hl->cshade);
			p->setBrush(tmp);
		}
		if (hl->cstroke != "None")
		{
			QColor tmp;
			SetFarbe(&tmp, hl->cstroke, hl->cshade2);
			p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
		}
		Zli->Farb = hl->ccolor;
		Zli->Farb2 = hl->cstroke;
		Zli->shade = hl->cshade;
		Zli->shade2 = hl->cshade2;
		Zli->xco = 0;
		Zli->yco = BaseOffs;
		Zli->Sele = hl->cselect;
		Zli->Siz = chs;
		Zli->realSiz = hl->csize;
		Zli->Style = hl->cstyle;
		Zli->ZFo = hl->cfont;
		Zli->wide = wide;
		Zli->kern = hl->csize * hl->cextra / 10000.0;
		Zli->scale = hl->cscale;
		Zli->scalev = hl->cscalev;
		Zli->base = hl->cbase;
		Zli->shadowX = hl->cshadowx;
		Zli->shadowY = hl->cshadowx;
		Zli->outline = hl->coutline;
		Zli->underpos = hl->cunderpos;
		Zli->underwidth = hl->cunderwidth;
		Zli->strikepos = hl->cstrikepos;
		Zli->strikewidth = hl->cstrikewidth;
		Zli->embedded = 0;
		if (!m_Doc->RePos)
			DrawZeichenS(p, Zli);
		delete Zli;
		p->setWorldMatrix(savWM);
		p->restore();
		p->setZoomFactor(sc);
		MaxChars = a+1;
		oCurX = CurX;
		CurX -= dx;
		CurX += wide+hl->csize * hl->cextra / 10000.0;
		first = false;
	}
}
