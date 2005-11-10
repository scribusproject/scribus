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
//Added by qt3to4:
#include <QApplication>
#include <QPixmap>
#include <QMouseEvent>
extern QPixmap loadIcon(QString nam);

RulerMover::RulerMover(ScribusView *pa) : QWidget(pa)
{
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
	qApp->setOverrideCursor(QCursor(Qt::SizeAllCursor), true);
}

void RulerMover::mouseReleaseEvent(QMouseEvent *m)
{
	if ((Mpressed) && (m->pos().x() > width()) && (m->pos().y() > height()))
	{
		currView->DrVX = -1;
		currView->DrHY = -1;
		currView->setRuler(m);
	}
	qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
	Mpressed = false;
}

void RulerMover::mouseMoveEvent(QMouseEvent *m)
{
	if ((Mpressed) && (m->pos().x() > width()) && (m->pos().y() > height()))
		currView->rulerMove(m);
}
