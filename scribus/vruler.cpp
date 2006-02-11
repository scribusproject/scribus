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
	currView->updateContents();
}

void Vruler::mouseMoveEvent(QMouseEvent *m)
{
	if ((Mpressed) && (m->pos().x() > width()))
		currView->FromVRuler(m);
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
	p.setClipRect(e->rect());
	p.drawLine(16, 0, 16, height());
	p.setBrush(black);
	p.setPen(black);
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
		QString txt = num.mid(a, 1);
#ifndef QT_MAC
		p->drawText(1, textY, txt);
#else
		static const int SCALE = 16;
		QFontMetrics fm = p->fontMetrics();
		QRect bbox = fm.boundingRect(txt);
		static QPixmap pix;
		if (pix.width() < bbox.width()*SCALE || pix.height() < bbox.height()*SCALE)
			pix.resize(bbox.width()*SCALE, bbox.height()*SCALE);
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
	repaint(0, oldMark-3, 17, 6);
	QPointArray cr;
	QPainter p;
#ifdef OPTION_SMOOTH_MARKERS
	// draw new marker to pixmap
	static const int SCALE = 16;
	static const QColor BACKGROUND(255, 255, 255);
	static QPixmap pix( 16*SCALE, 4*SCALE );
	static bool initpix = true;
	if (initpix) {
		initpix = false;
		p.begin( &pix );
		p.setBrush( BACKGROUND );
		p.drawRect( 0, 0, 16*SCALE, 4*SCALE );

		p.setPen(red);
		p.setBrush(red);
		cr.setPoints(3, 16*SCALE, 2*SCALE, 0, 4*SCALE, 0, 0);
		p.drawPolygon(cr);
		p.end();
	}
	// draw pixmap
	p.begin(this);
	p.translate(0, -currView->contentsY());
	p.scale(1.0/(SCALE+1), 1.0/SCALE);
	p.drawPixmap(0, (where-2)*SCALE, pix);
	p.end();
	// restore marks
	p.begin(this);
	p.setBrush(black);
	p.setPen(black);
	p.setFont(font());
	double sc = currView->getScale();
	double cc = height() / sc;
	double firstMark = ceil(offs / iter) * iter - offs;
	while (firstMark < cc)
	{
		p.drawLine(10, qRound(firstMark * sc), 16, qRound(firstMark * sc));
		firstMark += iter;
	}
	p.end();
#else
	// draw slim marker
	p.begin(this);
	p.translate(0, -currView->contentsY());
	p.setPen(red);
	p.setBrush(red);
	cr.setPoints(5,  5, where, 16, where, 5, where, 0, where+2, 0, where-2);
	p.drawPolygon(cr);
	p.end();
#endif

	oldMark = currentCoor;
}

void Vruler::unitChange()
{
	double sc = currView->scale();
	cor=1;
	int docUnitIndex=currDoc->unitIndex();
	switch (docUnitIndex)
	{
		case 0:
			if (sc > 1 && sc <= 4)
				cor = 2;
			if (sc > 4)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
	  		iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
			break;
		case 1:
			if (sc > 1)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
			break;
		case 2:
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
			break;
		case 3:
			iter = unitRulerGetIter1FromIndex(docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
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
			iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
			break;
		case 5:
			iter = unitRulerGetIter1FromIndex(docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
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
