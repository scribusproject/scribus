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
#include <qcolor.h>
#include <qrect.h>
#include <qpointarray.h>
#include "scribus.h"

#if QT_VERSION  > 0x030102
	#define SPLITVC SplitHCursor
#else
	#define SPLITVC SplitVCursor
#endif

extern ScribusApp* ScApp;

Vruler::Vruler(QScrollView *pa, ScribusDoc *doc) : QWidget(pa)
{
	setEraseColor(QColor(255,255,255));
	doku = doc;
	offs = -12;
	Markp = 0;
	oldMark = 0;
	Mpressed = false;
}

void Vruler::mousePressEvent(QMouseEvent *)
{
	Mpressed = true;
	if (ScApp->Prefs.GuidesShown)
		qApp->setOverrideCursor(QCursor(SPLITVC), true);
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
	int cor = 1;
	QFont ff = font();
	ff.setPointSize(8);
	setFont(ff);
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
	p.drawLine(24, 0, 24, height());
	p.translate(0, -offs);
	p.setBrush(black);
	p.setPen(black);
	p.setFont(font());
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
			if ((qRound((xl+of)*sc) > offs) && (qRound((xl+of)*sc) < offs+height()))
				p.drawLine(18, qRound((xl+of)*sc), 24, qRound((xl+of)*sc));
		}
		for (xl = 0; xl < doku->PageH+(iter2/2); xl += iter2)
		{
			if ((qRound((xl+of)*sc) > offs) && (qRound((xl+of)*sc) < offs+height()))
			{
				p.drawLine(11, qRound((xl+of)*sc), 24, qRound((xl+of)*sc));
				switch (doku->Einheit)
				{
					case 2:
					{
						QString tx = "";
						int num1 = static_cast<int>(xl / iter2 / cor);
						if (num1 != 0)
							tx = QString::number(num1);
						double frac = (xl / iter2 / cor) - num1;
						if ((frac > 0.24) && (frac < 0.26))
							tx += QChar(0xBC);
						if ((frac > 0.49) && (frac < 0.51))
							tx += QChar(0xBD);
						if ((frac > 0.74) && (frac < 0.76))
							tx += QChar(0xBE);
						p.drawText(9, qRound((xl+of+10/sc) * sc), tx);
						break;
					}
					case 3:
						p.drawText(9, qRound((xl+of+10/sc) * sc), QString::number(xl / iter / cor));
						break;
					default:
						p.drawText(9, qRound((xl+of+10/sc) * sc), QString::number(xl / iter * 10 / cor));
						break;
				}
			}
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
