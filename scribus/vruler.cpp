/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QCursor>
//#include <QDebug>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPolygon>
#include <QRect>
#include <QRubberBand>

#include "canvasgesture_rulermove.h"
#include "page.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "units.h"
#include "vruler.h"

#if QT_VERSION  > 0x030102
	#define SPLITVC Qt::SplitHCursor
#else
	#define SPLITVC Qt::SplitVCursor
#endif



Vruler::Vruler(ScribusView *pa, ScribusDoc *doc) : QWidget(pa)
{
	prefsManager=PrefsManager::instance();
	setBackgroundRole(QPalette::Window);
	setAutoFillBackground(true);
	QPalette palette;
	palette.setBrush(QPalette::Window, QColor(255, 255, 255));
	setPalette(palette);
	currDoc = doc;
	currView = pa;
	offs = 0;
	oldMark = 0;
	Mpressed = false;
	drawMark = false;
	rulerGesture = new RulerGesture(currView, RulerGesture::VERTICAL);
	unitChange();
}

void Vruler::mousePressEvent(QMouseEvent *m)
{
	Mpressed = true;
	if (currDoc->guidesSettings.guidesShown)
	{
		qApp->changeOverrideCursor(QCursor(SPLITVC));
		currView->startGesture(rulerGesture);
	}
}

void Vruler::mouseReleaseEvent(QMouseEvent *m)
{
	if (Mpressed)
	{
		rulerGesture->mouseReleaseEvent(m);
		Mpressed = false;
	}
}

void Vruler::mouseMoveEvent(QMouseEvent *m)
{
	if (Mpressed)
	{
		rulerGesture->mouseMoveEvent(m);
	}
}

void Vruler::paintEvent(QPaintEvent *e)
{
	if (currDoc->isLoading())
		return;
	QString tx = "";
	double xl, frac;
	double sc = currView->scale();
	QFont ff = font();
	ff.setPointSize(8);
	setFont(ff);
	QPainter p;
	p.begin(this);
	p.save();
	p.setClipRect(e->rect());
	p.drawLine(16, 0, 16, height());
	p.setBrush(Qt::black);
	p.setPen(Qt::black);
	p.setFont(font());
	double cc = height() / sc;
	double firstMark = ceil(offs / iter) * iter - offs;
	while (firstMark < cc)
	{
		p.drawLine(10, qRound(firstMark * sc), 16, qRound(firstMark * sc));
		firstMark += iter;
	}
	firstMark = ceil(offs / iter2) * iter2 - offs;
	int markC = static_cast<int>(ceil(offs / iter2));
	while (firstMark < cc)
	{
		p.drawLine(3, qRound(firstMark * sc), 16, qRound(firstMark * sc));
		int textY = qRound(firstMark * sc)+10;
		switch (currDoc->unitIndex())
		{
			case SC_MM:
				tx = QString::number(markC * iter2 / (iter2 / 100) / cor);
				break;
			case SC_IN:
				xl = (markC * iter2 / iter2) / cor;
				tx = QString::number(static_cast<int>(xl));
				frac = fabs(xl - static_cast<int>(xl));
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
			case SC_P:
			case SC_C:
				tx = QString::number(markC * iter2 / (iter2 /5) / cor);
				break;
			case SC_CM:
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
	p.restore();
	if (drawMark)
	{
		QPolygon cr;
#ifdef OPTION_SMOOTH_MARKERS
		// draw new marker to pixmap
		static const int SCALE = 16;
		static const QColor BACKGROUND(255, 255, 255);
		static QPixmap pix( 16*SCALE, 4*SCALE );
		static bool initpix = true;
		if (initpix)
		{
			initpix = false;
			QPainter pp;
			pp.begin( &pix );
			pp.setBrush( BACKGROUND );
			pp.drawRect( 0, 0, 16*SCALE, 4*SCALE );
	
			pp.setPen(Qt::red);
			pp.setBrush(Qt::red);
			cr.setPoints(3, 16*SCALE, 2*SCALE, 0, 4*SCALE, 0, 0);
			pp.drawPolygon(cr);
			pp.end();
		}
		// draw pixmap
		p.save();
		p.translate(0, -currView->contentsY());
		p.scale(1.0/(SCALE+1), 1.0/SCALE);
		p.drawPixmap(0, (where-2)*SCALE, pix);
		p.restore();
		// repaint marks
		p.setBrush(Qt::black);
		p.setPen(Qt::black);
		p.setFont(font());
		double sc = currView->getScale();
		double cc = height() / sc;
		double firstMark = ceil(offs / iter) * iter - offs;
		while (firstMark < cc)
		{
			p.drawLine(10, qRound(firstMark * sc), 16, qRound(firstMark * sc));
			firstMark += iter;
		}
#else
		// draw slim marker
		p.translate(0, -currView->contentsY());
		p.setPen(Qt::red);
		p.setBrush(Qt::red);
		cr.setPoints(5,  5, whereToDraw, 16, whereToDraw, 5, whereToDraw, 0, whereToDraw+2, 0, whereToDraw-2);
		p.drawPolygon(cr);
#endif
	}
	p.end();
}

void Vruler::drawNumber(QString num, int starty, QPainter *p)
{
	int textY = starty;
	for (int a = 0; a < num.length(); ++a)
	{
		QString txt = num.mid(a, 1);
#ifndef Q_WS_MAC
		p->drawText(1, textY, txt);
#else
		static const int SCALE = 16;
		QFontMetrics fm = p->fontMetrics();
		QRect bbox = fm.boundingRect(txt);
		static QPixmap pix;
		if (pix.width() < bbox.width()*SCALE || pix.height() < bbox.height()*SCALE)
			pix = QPixmap(bbox.width()*SCALE, bbox.height()*SCALE);
		QFont fnt = p->font();
		QPainter p2;
		pix.fill();
		p2.begin( &pix );
		if (fnt.pointSize() > 0)
			fnt.setPointSize(SCALE*fnt.pointSize()-SCALE/2);
		else if (fnt.pixelSize() > 0)
			fnt.setPixelSize(SCALE*fnt.pixelSize()-SCALE/2);
		else
			fnt.setPixelSize(SCALE);
		p2.setFont(fnt);
		p2.drawText(-bbox.x()*SCALE, -bbox.y()*SCALE, txt);
		p2.end();
		p->scale(1.0/SCALE,1.0/SCALE);
		p->drawPixmap(1*SCALE, (textY+bbox.top())*SCALE, pix, 0, 0, bbox.width()*SCALE, bbox.height()*SCALE);
		p->scale(SCALE,SCALE);
#endif
		textY += 11;
	}
}

void Vruler::Draw(int where)
{
	// erase old marker
	int currentCoor = where - currView->contentsY();
	whereToDraw = where;
	drawMark = true;
	repaint(0, oldMark-3, 17, 6);
//	drawMark = false;
	oldMark = currentCoor;
}

void Vruler::unitChange()
{
	double sc = currView->scale();
	cor=1;
	int docUnitIndex=currDoc->unitIndex();
	switch (docUnitIndex)
	{
		case SC_PT:
			if (sc > 1 && sc <= 4)
				cor = 2;
			if (sc > 4)
				cor = 10;
			if (sc < 0.3)
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) * 3;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) * 3;
			}
			else if (sc < 0.2)
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) * 2;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) * 2;
			}
			else
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
	  		}
			break;
		case SC_MM:
			if (sc > 1)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
			break;
		case SC_IN:
			iter = unitRulerGetIter1FromIndex(docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
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
			if (sc < 0.25)
			{
				cor = 0.5;
				iter = 72.0*16.0;
				iter2 = 72.0*2.0;
			}
			break;
		case SC_P:
			iter = unitRulerGetIter1FromIndex(docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
			if (sc >= 1 && sc <= 4)
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
			if (sc < 0.3)
			{
				cor = 0.25;
				iter = 12.0*4;
				iter2 = 60.0*4;
			}
			else
				if (sc < 1)
			{
				cor = 1;
				iter = 12.0;
				iter2 = 60.0;
			}
			break;
		case SC_CM:
			if (sc > 1 && sc <= 4)
				cor = 1;
			if (sc > 4)
				cor = 10;
			if (sc < 0.6)
			{
				cor=0.1;
				iter = 720.0/25.4;
				iter2 = 7200.0/25.4;
			}
			else
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
	  		}
			break;
		case SC_C:
			iter = unitRulerGetIter1FromIndex(docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
			if (sc >= 1 && sc <= 4)
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
			if (sc < 0.3)
			{
				cor = 0.1;
				iter = 72.0/25.4*4.512*10;
				iter2 = 72.0/25.4*4.512*5.0*10;
			}
			else
				if (sc < 1)
			{
				cor = 1;
				iter = 72.0/25.4*4.512;
				iter2 = 72.0/25.4*4.512*5.0;
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
