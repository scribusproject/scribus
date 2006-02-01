/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          navigator.cpp  -  description
                             -------------------
    begin                : Fri Feb 22 2002
    copyright            : (C) 2002 by Franz Schmid
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

#include "navigator.h"
#include "navigator.moc"

#include <qpainter.h>
#include "scribusview.h"
#include "gsutil.h"

Navigator::Navigator(QWidget *parent, int Size, int Seite, ScribusView* vie, QString fn) : QLabel(parent)
{
	setScaledContents(false);
	setAlignment(static_cast<int>( QLabel::AlignLeft | QLabel::AlignTop) );
	if (!fn.isEmpty())
	{
		QPixmap img = LoadPDF(fn, Seite, Size, &Breite, &Hoehe);
		if (!img.isNull())
			pmx = img;
		else
			pmx = LoadPDF(fn, 1, Size, &Breite, &Hoehe);
	}
	else
		pmx.convertFromImage(vie->PageToPixmap(Seite, Size));
	resize(pmx.width(), pmx.height());
	Xp = 0;
	Yp = 0;
	drawMark(0, 0);
	view = vie;
}

void Navigator::mouseMoveEvent(QMouseEvent *m)
{
	drawMark(m->x(), m->y());
	emit Coords(static_cast<double>(m->x())/static_cast<double>(pmx.width()),
			    static_cast<double>(m->y())/static_cast<double>(pmx.height()));
}

void Navigator::mousePressEvent(QMouseEvent *m)
{
	drawMark(m->x(), m->y());
	emit Coords(static_cast<double>(m->x())/static_cast<double>(pmx.width()),
			    static_cast<double>(m->y())/static_cast<double>(pmx.height()));
}

void Navigator::mouseReleaseEvent(QMouseEvent *m)
{
	emit Coords(static_cast<double>(m->x())/static_cast<double>(pmx.width()),
			    static_cast<double>(m->y())/static_cast<double>(pmx.height()));
}

void Navigator::paintEvent(QPaintEvent *e)
{
	QPainter p;
	p.begin(this);
	p.setClipRect(e->rect());
	drawMark(Xp, Yp);
	p.end();
}

void Navigator::drawMark(int x, int y)
{
	QPainter p;
	p.begin(this);
	p.eraseRect(pmx.rect());
	p.setClipRect(pmx.rect());
	p.drawPixmap(0, 0, pmx);
	p.setPen(QPen(QColor(black), 1, SolidLine, FlatCap, MiterJoin));
	p.drawLine(x-5, y-5, x-1, y-1);
	p.drawLine(x-5, y+5, x-1, y+1);
	p.drawLine(x+2, y+2, x+6, y+6);
	p.drawLine(x+2, y-2, x+6, y-6);
	p.end();
}

bool Navigator::SetSeite(int Seite, int Size, QString fn)
{
	bool ret = false;
	if (!fn.isEmpty())
	{
		QPixmap img = LoadPDF(fn, Seite, Size, &Breite, &Hoehe);
		if (!img.isNull())
		{
			pmx = img;
			ret = true;
		}
		else
			pmx = LoadPDF(fn, 1, Size, &Breite, &Hoehe);
	}
	else
	{
		pmx.convertFromImage(view->PageToPixmap(Seite, Size));
		ret = true;
	}
	resize(pmx.width(), pmx.height());
	repaint();
	return ret;
}
