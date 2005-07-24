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
#include "prefsmanager.h"

#if QT_VERSION  > 0x030102
	#define SPLITVC SplitHCursor
#else
	#define SPLITVC SplitVCursor
#endif

extern ScribusApp* ScApp;

Vruler::Vruler(ScribusView *pa, ScribusDoc *doc) : QWidget(pa)
{
	prefsManager=PrefsManager::instance();
	setEraseColor(QColor(255,255,255));
	currDoc = doc;
	currView = pa;
	offs = 0;
	oldMark = 0;
	Mpressed = false;
	unitChange();
}

void Vruler::mousePressEvent(QMouseEvent *m)
{
	Mpressed = true;
	if (prefsManager->appPrefs.guidesSettings.guidesShown)
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
	QString tx = "";
	double xl, frac;
	double sc = currView->getScale();
	QFont ff = font();
	ff.setPointSize(8);
	setFont(ff);
	QPainter p;
	p.begin(this);
	p.drawLine(24, 0, 24, height());
	p.setBrush(black);
	p.setPen(black);
	p.setFont(font());
	double cc = height() / sc;
	double firstMark = ceil(offs / iter) * iter - offs;
	while (firstMark < cc)
	{
		p.drawLine(18, qRound(firstMark * sc), 24, qRound(firstMark * sc));
		firstMark += iter;
	}
	firstMark = ceil(offs / iter2) * iter2 - offs;
	int markC = static_cast<int>(ceil(offs / iter2));
	while (firstMark < cc)
	{
		p.drawLine(11, qRound(firstMark * sc), 24, qRound(firstMark * sc));
		int textY = qRound(firstMark * sc)+10;
		switch (currDoc->docUnitIndex)
		{
			case 1:
				tx = QString::number(markC * iter2 / (iter2 / 100) / cor);
				break;
			case 2:
				xl = (markC * iter2 / iter2) / cor;
				tx = QString::number(static_cast<int>(xl));
				frac = xl - static_cast<int>(xl);
				if ((static_cast<int>(xl) == 0) && (frac > 0.1))
					tx = "";
				if ((frac > 0.24) && (frac < 0.26))
					tx += QChar(0xBC);
				if ((frac > 0.49) && (frac < 0.51))
					tx += QChar(0xBD);
				if ((frac > 0.74) && (frac < 0.76))
					tx += QChar(0xBE);
				tx = tx;
				break;
			case 3:
			case 5:
				tx = QString::number(markC * iter2 / (iter2 / 10) / cor);
				break;
			case 4:
				tx = QString::number(markC * iter2 / iter2 / cor);
				break;
			default:
				tx = QString::number(markC * iter2);
				break;
		}
		drawNumber(tx, textY, &p);
		firstMark += iter2;
		markC++;
	}
	p.end();
}

void Vruler::drawNumber(QString num, int starty, QPainter *p)
{
	int textY = starty;
	for (uint a = 0; a < num.length(); ++a)
	{
		QString c = num.mid(a, 1);
		p->drawText(9, textY, c);
		textY += 11;
	}
}

/** Zeichnet den Pfeil */
void Vruler::Draw(int wo)
{
	QPainter p;
	p.begin(this);
	p.translate(0, -currView->contentsY());
	p.setPen(white);
	p.setBrush(white);
	p.drawRect(0, oldMark-3, 9, 6);
	p.setPen(red);
	p.setBrush(red);
	QPointArray cr;
	cr.setPoints(3, 8, wo, 0, wo+2, 0, wo-2);
	p.drawPolygon(cr);
	p.end();
	oldMark = wo;
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
		case 5:
			iter = unitRulerGetIter1FromIndex(currDoc->docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(currDoc->docUnitIndex);
			if (sc > 1 && sc <= 4)
			{
				cor = 1;
				iter = 72.0/25.4*4.512;
				iter2 = 72.0/25.4*4.512*5.0;
			}
			if (sc > 4)
			{
				cor = 2;
				iter = 72.0/25.4*4.512/2.0;
				iter2 = 72.0/25.4*4.512;
			}
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
