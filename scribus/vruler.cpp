/***************************************************************************
                          vruler.cpp  -  description
                             -------------------
    begin                : Wed Apr 11 2001
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

#include "vruler.h"
#include "vruler.moc"
#include "page.h"
#include <qcursor.h>
Vruler::Vruler(QScrollView *pa, ScribusDoc *doc) : QWidget(pa)
{
	setEraseColor(QColor(255,255,255));
	doku = doc;
	rfont = font();
	rfont.setPointSize(9);
	offs = -12;
	Markp = 0;
	oldMark = 0;
	Mpressed = false;
}

void Vruler::mousePressEvent(QMouseEvent *)
{
	Mpressed = true;
	qApp->setOverrideCursor(QCursor(SplitVCursor), true);
}

void Vruler::mouseReleaseEvent(QMouseEvent *m)
{
	Mpressed = false;
	doku->ActPage->DrVX = -1;
	doku->ActPage->SetXGuide(m);
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
}

void Vruler::mouseMoveEvent(QMouseEvent *m)
{
	if ((Mpressed) && (m->pos().x() > width()))
		doku->ActPage->FromVRuler(m);
}

void Vruler::paintEvent(QPaintEvent *)
{
	int xx, pc;
	double of, xl, iter, iter2;
	double sc = doku->Scale;
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
	p.drawLine(24, 0, 24, height());
	p.translate(0, -offs);
	p.setBrush(black);
	p.setPen(black);
	p.setFont(rfont);
	p.scale(1.0, sc);
	if ((doku->PageFP) && (doku->PagesSbS))
		{
		if (doku->FirstPageLeft)
			{
			if (doku->PageC % 2 == 0)
				pc = doku->PageC / 2;
			else
				pc = (doku->PageC+1) / 2;
			}
		else
			{
			if (doku->PageC % 2 == 0)
				pc = doku->PageC / 2 + 1;
			else
				pc = doku->PageC / 2 + 1;
			}
		}
	else
		pc = doku->PageC;
	for (xx = 0; xx < pc; ++xx)
		{
		of = xx * (doku->PageH+30.0);
		for (xl = 0; xl < doku->PageH; xl += iter)
			{
			p.drawLine(18, static_cast<int>(xl+of), 24, static_cast<int>(xl+of));
			}
		for (xl = 0; xl < doku->PageH+(iter2/2); xl += iter2)
			{
			p.drawLine(11, static_cast<int>(xl+of), 24, static_cast<int>(xl+of));
			p.save();
			p.scale(1.0, 1.0 / sc);
			switch (doku->Einheit)
				{
				case 2:
					p.drawText(9, static_cast<int>((xl+of+qRound(10/sc)) * sc), QString::number(xl / iter2));
					break;
				case 3:
					p.drawText(9, static_cast<int>((xl+of+qRound(10/sc)) * sc), QString::number(xl / iter));
					break;
				default:
					p.drawText(9, static_cast<int>((xl+of+qRound(10/sc)) * sc), QString::number(xl / iter * 10));
					break;
				}
			p.restore();
			}
		}
	p.end();
}
/** Zeichnet den Pfeil */
void Vruler::Draw(int wo)
{
	Markp = wo-qRound(10*doku->Scale);
	QPainter p;
	p.begin(this);
	p.translate(0, -offs);
	p.setPen(white);
	p.setBrush(white);
	p.drawRect(0, oldMark-3, 10, 6);
	p.setPen(red);
	p.setBrush(red);
	QPointArray cr;
	cr.setPoints(3, 9, Markp, 0, Markp+2, 0, Markp-2);
	p.drawPolygon(cr);
	p.end();
	oldMark = Markp;
}

