/***************************************************************************
                          hruler.cpp  -  description
                             -------------------
    begin                : Tue Apr 10 2001
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

#include "hruler.h"
#include "hruler.moc"
#include "page.h"
#include <qcursor.h>
Hruler::Hruler(QScrollView *pa, ScribusDoc *doc) : QWidget(pa)
{
	setEraseColor(QColor(255,255,255));
	doku = doc;
	rfont = font();
	rfont.setPointSize(9);
	offs = -10;
	Markp = 0;
	repX = false;
	Mpressed = false;
}

void Hruler::mousePressEvent(QMouseEvent *)
{
	Mpressed = true;
	qApp->setOverrideCursor(QCursor(SplitHCursor), true);
}

void Hruler::mouseReleaseEvent(QMouseEvent *m)
{
	Mpressed = false;
	doku->ActPage->DrHY = -1;
	doku->ActPage->SetYGuide(m);
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
}

void Hruler::mouseMoveEvent(QMouseEvent *m)
{
	if ((Mpressed) && (m->pos().y() > height()))
		doku->ActPage->FromHRuler(m);
}

void Hruler::paintEvent(QPaintEvent *)
{
	int pc, xx;
	float of, xl, iter, iter2;
	float sc = doku->Scale;
	switch (doku->Einheit)
		{
		case 0:
			iter = 10.0;
  		iter2 = iter * 10.0;
			break;
		case 1:
			iter = (10.0 / 25.4) * 72.0;
  		iter2 = iter * 10.0;
			break;
		case 2:
			iter = 18.0;
			iter2 = 72.0;
			break;
		case 3:
			iter = 12.0;
			iter2 = 120.0;
			break;
		}
	QPainter p;
	p.begin(this);
	p.drawLine(0, 24, width(), 24);
	p.translate(-offs, 0);
	if (repX)
		{
		p.setPen(red);
		p.setBrush(red);
		QPointArray cr;
		cr.setPoints(3, Markp, 9, Markp+2, 0, Markp-2, 0);
		p.drawPolygon(cr);
		p.end();
		repX = false;
		return;
		}
	p.setBrush(black);
	p.setPen(black);
	p.setFont(rfont);
	p.scale(sc, 1.0);
	((doku->PageFP) && (doku->PagesSbS)) ? pc = 2 : pc = 1;
	if (doku->MasterP)
		pc = 1;
	for (xx = 0; xx < pc; ++xx)
		{
		of = xx * (doku->PageB+30.0);
		for (xl = 0; xl < doku->PageB; xl += iter)
			{
			p.drawLine(static_cast<int>(xl+of), 18, static_cast<int>(xl+of), 24);
			}
		for (xl = 0; xl < doku->PageB+(iter2/2); xl += iter2)
			{
			p.drawLine(static_cast<int>(xl+of), 11, static_cast<int>(xl+of), 24);
			p.save();
			p.scale(1.0 / sc, 1.0);
			switch (doku->Einheit)
				{
				case 2:
					p.drawText(static_cast<int>((xl+of+qRound(2/sc)) * sc), 17, QString::number(xl / iter2));
					break;
				case 3:
					p.drawText(static_cast<int>((xl+of+qRound(2/sc)) * sc), 17, QString::number(xl / iter));
					break;
				default:
					p.drawText(static_cast<int>((xl+of+qRound(2/sc)) * sc), 17, QString::number(xl / iter * 10));
					break;
				}
			p.restore();
			}
		}
}

/** Zeichnet den Pfeil */
void Hruler::Draw(int wo)
{
	repX = true;
	Markp = wo-qRound(10*doku->Scale);
	repaint(QRect(0, 0, width(), 9));
}
