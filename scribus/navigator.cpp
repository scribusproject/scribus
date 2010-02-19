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

#include <QPixmap>
#include <QMouseEvent>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include "scribusview.h"
#include "util_ghostscript.h"

Navigator::Navigator(QWidget *parent, int Size, int Seite, ScribusView* vie, QString fn) : QLabel(parent)
{
	setScaledContents(false);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	if (!fn.isEmpty())
	{
		QPixmap img = LoadPDF(fn, Seite, Size, &Width, &Height);
		if (!img.isNull())
			pmx = img;
		else
			pmx = LoadPDF(fn, 1, Size, &Width, &Height);
	}
	else
		pmx=QPixmap::fromImage(vie->PageToPixmap(Seite, Size));
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
	p.setBackground(QColor(255, 255, 255));
	p.eraseRect(pmx.rect());
	p.setClipRect(pmx.rect());
	p.drawPixmap(0, 0, pmx);
	p.setPen(QPen(QColor(Qt::black), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p.drawLine(Xp-5, Yp-5, Xp-1, Yp-1);
	p.drawLine(Xp-5, Yp+5, Xp-1, Yp+1);
	p.drawLine(Xp+2, Yp+2, Xp+6, Yp+6);
	p.drawLine(Xp+2, Yp-2, Xp+6, Yp-6);
	p.end();
}

void Navigator::drawMark(int x, int y)
{
	Xp = x;
	Yp = y;
	repaint();
}

bool Navigator::SetPage(int Seite, int Size, QString fn)
{
	bool ret = false;
	if (!fn.isEmpty())
	{
		QPixmap img = LoadPDF(fn, Seite, Size, &Width, &Height);
		if (!img.isNull())
		{
			pmx = img;
			ret = true;
		}
		else
			pmx = LoadPDF(fn, 1, Size, &Width, &Height);
	}
	else
	{
		pmx=QPixmap::fromImage(view->PageToPixmap(Seite, Size));
		ret = true;
	}
	resize(pmx.width(), pmx.height());
	repaint();
	return ret;
}
