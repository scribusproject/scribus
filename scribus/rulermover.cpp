/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          rulermover.cpp  -  description
                             -------------------
    begin                : Sun Jul 24 2005
    copyright            : (C) 2005 by Franz Schmid
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

#include "rulermover.h"
#include "scribusview.h"
#include "scribusdoc.h"
#include <qcursor.h>
#include <qapplication.h>
#include <QMouseEvent>
#include <QPixmap>
#include "util_icon.h"

RulerMover::RulerMover(ScribusView *pa) : QWidget(pa)
{
	setBackgroundRole(QPalette::Window);
	setAutoFillBackground(true);
	QPalette palette;
	palette.setBrush(QPalette::Window, QColor(255, 255, 255));
	setPalette(palette);
 	setErasePixmap(loadIcon("mover.png"));
	currView = pa;
	Mpressed = false;
}

void RulerMover::mouseDoubleClickEvent(QMouseEvent *)
{
	currView->Doc->rulerXoffset = 0;
	currView->Doc->rulerYoffset = 0;
	currView->DrawNew();
}

void RulerMover::mousePressEvent(QMouseEvent *m)
{
	Mpressed = true;
	QPoint py = currView->viewport()->mapFromGlobal(m->globalPos());
	currView->DrVX = py.x();
	currView->DrHY = py.y();
	qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
	currView->redrawMode = 1;
	currView->specialRendering = true;
	currView->firstSpecial = true;
	currView->redrawPolygon.clear();
	currView->redrawPolygon << QPoint(-1, -1);
}

void RulerMover::mouseReleaseEvent(QMouseEvent *m)
{
	if ((Mpressed) && (m->pos().x() > width()) && (m->pos().y() > height()))
	{
		currView->DrVX = -1;
		currView->DrHY = -1;
		currView->setNewRulerOrigin(m);
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	Mpressed = false;
	currView->redrawMode = 0;
	currView->specialRendering = false;
	currView->firstSpecial = false;
}

void RulerMover::mouseMoveEvent(QMouseEvent *m)
{
	if ((Mpressed) && (m->pos().x() > width()) && (m->pos().y() > height()))
		currView->rulerMove(m);
}
