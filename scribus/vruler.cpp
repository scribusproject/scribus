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
#include "scribusdoc.h"
#include "units.h"

#if QT_VERSION  > 0x030102
	#define SPLITVC SplitHCursor
#else
	#define SPLITVC SplitVCursor
#endif

extern ScribusApp* ScApp;

Vruler::Vruler(ScribusView *pa, ScribusDoc *doc) : QWidget(pa)
{
	setEraseColor(QColor(255,255,255));
	currDoc = doc;
	currView = pa;
	offs = -12;
	Markp = 0;
	oldMark = 0;
	Mpressed = false;
	unitChange();
}

void Vruler::mousePressEvent(QMouseEvent *m)
{
	Mpressed = true;
	if (ScApp->Prefs.guidesSettings.guidesShown)
	{
		QPoint py = currView->viewport()->mapFromGlobal(m->globalPos());
		currView->DrVX = py.x();
		qApp->setOverrideCursor(QCursor(SPLITVC), true);
	}
}

void Vruler::mouseReleaseEvent(QMouseEvent *m)
{
	if ((Mpressed) && (m->pos().x() > width()))
	{
		currView->DrVX = -1;
		currView->SetXGuide(m, -1);
	}
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	Mpressed = false;
}

void Vruler::mouseMoveEvent(QMouseEvent *m)
{
	if ((Mpressed) && (m->pos().x() > width()))
		currView->FromVRuler(m);
}

void Vruler::paintEvent(QPaintEvent *)
{
	if (currDoc->loading)
		return;
	int xx, pc;
	double of, xl;
	double sc = currView->getScale();
	QFont ff = font();
	ff.setPointSize(8);
	setFont(ff);
	QPainter p;
	p.begin(this);
	p.drawLine(24, 0, 24, height());
	p.translate(0, -offs);
	p.setBrush(black);
	p.setPen(black);
	p.setFont(font());
	if (currDoc->PageFP)
	{
		if (currDoc->FirstPageLeft)
		{
			if (currDoc->pageCount % 2 == 0)
				pc = currDoc->pageCount / 2;
			else
				pc = (currDoc->pageCount+1) / 2;
		}
		else
		{
			if (currDoc->pageCount % 2 == 0)
				pc = currDoc->pageCount / 2 + 1;
			else
				pc = currDoc->pageCount / 2 + 1;
		}
	}
	else
		pc = currDoc->pageCount;
	for (xx = 0; xx < pc; ++xx)
	{
		of = xx * (currDoc->pageHeight+currDoc->ScratchBottom+currDoc->ScratchTop);
		for (xl = 0; xl < currDoc->pageHeight; xl += iter)
		{
			int markerY=qRound((xl+of)*sc)+1;
			if (( markerY > offs) && (markerY < offs+height()))
				p.drawLine(18, markerY, 24, markerY);
		}
		for (xl = 0; xl < currDoc->pageHeight+(iter2/2); xl += iter2)
		{
			int markerY=qRound((xl+of)*sc)+1;
			if ((markerY > offs) && (markerY < offs+height()))
			{
				p.drawLine(11, markerY, 24, markerY);
				int textY=qRound((xl+of+10/sc) * sc);
				switch (currDoc->docUnitIndex)
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
						p.drawText(9, textY, tx);
						break;
					}
					case 3:
						p.drawText(9, textY, QString::number(xl / iter / cor));
						break;
					case 4:
						p.drawText(9, textY, QString::number(xl / iter / 10 / cor));
						break;						
					default:
						p.drawText(9, textY, QString::number(xl / iter * 10 / cor));
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
	Markp = wo-qRound(currDoc->ScratchTop*currView->getScale());
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

void Vruler::unitChange()
{
	double sc = currView->getScale();
	cor=1;
	switch (currDoc->docUnitIndex)
	{
		case 0:
			if (sc > 1 && sc <= 4)
				cor = 2;
			if (sc > 4)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(currDoc->docUnitIndex) / cor;
	  		iter2 = unitRulerGetIter2FromIndex(currDoc->docUnitIndex) / cor;
			break;
		case 1:
			if (sc > 1)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(currDoc->docUnitIndex) / cor;
  			iter2 = unitRulerGetIter2FromIndex(currDoc->docUnitIndex) / cor;
			break;
		case 2:
			iter = unitRulerGetIter1FromIndex(currDoc->docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(currDoc->docUnitIndex);
			if (sc > 1 && sc <= 4)
			{
				cor = 2;
				iter /= cor;
				iter2 /= cor;
			}
			if (sc > 4)
			{
				cor = 4;
				iter /= cor;
				iter2 /= cor;
			}
			break;
		case 3:
			iter = unitRulerGetIter1FromIndex(currDoc->docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(currDoc->docUnitIndex);
			if (sc > 1 && sc <= 4)
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
		case 4:
			if (sc > 1 && sc <= 4)
				cor = 1;
			if (sc > 4)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(currDoc->docUnitIndex) / cor;
			iter2 = unitRulerGetIter2FromIndex(currDoc->docUnitIndex) / cor;
			break;
		default:
			if (sc > 1 && sc <= 4)
				cor = 2;
			if (sc > 4)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(0) / cor;
	 		iter2 = unitRulerGetIter2FromIndex(0) / cor;
			break;
	}
}
