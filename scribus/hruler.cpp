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
#include <qcolor.h>
#include <qrect.h>
#include <qpointarray.h>

Hruler::Hruler(QScrollView *pa, ScribusDoc *doc) : QWidget(pa)
{
	setEraseColor(QColor(255,255,255));
	doku = doc;
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
	double of, xl, iter, iter2;
	double sc = doku->Scale;
	int cor = 1;
	switch (doku->Einheit)
	{
		case 0:
			if (sc > 1)
				cor = 2;
			if (sc > 4)
				cor = 10;
			iter = 10.0 / cor;
	  		iter2 = iter * 10.0;
			break;
		case 1:
			if (sc > 1)
				cor = 10;
			iter = ((10.0 / 25.4) * 72.0) / cor;
  			iter2 = iter * 10.0;
			break;
		case 2:
			iter = 18.0;
			iter2 = 72.0;
			if (sc > 1)
				{
				cor = 2;
				iter = 9.0;
				iter2 = 36.0;
				}
			if (sc > 4)
				{
				iter = 9.0;
				iter2 = 18.0;
				cor = 4;
				}
			break;
		case 3:
			iter = 12.0;
			iter2 = 120.0;
			if (sc > 1)
				{
				cor = 1;
				iter = 12.0;
				iter2 = 60.0;
				}
			if (sc > 4)
				{
				cor = 2;
				iter = 6.0;
				iter2 = 12.0;
				}
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
	p.setFont(font());
	((doku->PageFP) && (doku->PagesSbS)) ? pc = 2 : pc = 1;
	if (doku->MasterP)
		pc = 1;
	for (xx = 0; xx < pc; ++xx)
	{
		of = xx * (doku->PageB+30.0);
		for (xl = 0; xl < doku->PageB; xl += iter)
			p.drawLine(qRound((xl+of)*sc), 18, qRound((xl+of)*sc), 24);
		for (xl = 0; xl < doku->PageB+(iter2/2); xl += iter2)
		{
			p.drawLine(qRound((xl+of)*sc), 11, qRound((xl+of)*sc), 24);
			switch (doku->Einheit)
			{
				case 2:
					p.drawText(qRound((xl+of+2/sc) * sc), 17, QString::number(xl / iter2 / cor));
					break;
				case 3:
					p.drawText(qRound((xl+of+2/sc) * sc), 17, QString::number(xl / iter / cor));
					break;
				default:
					p.drawText(qRound((xl+of+2/sc) * sc), 17, QString::number(xl / iter * 10 / cor));
					break;
			}
		}
	}
	p.end();
}

/** Zeichnet den Pfeil */
void Hruler::Draw(int wo)
{
	repX = true;
	Markp = wo-qRound(10*doku->Scale);
	repaint(QRect(0, 0, width(), 9));
}
