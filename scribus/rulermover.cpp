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

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QPalette>

#include "canvas.h"
#include "canvasgesture_rulermove.h"
#include "scribusview.h"
#include "scribusdoc.h"

#include "util_icon.h"

RulerMover::RulerMover(ScribusView *pa) : QWidget(pa)
{
	setBackgroundRole(QPalette::Window);
	setAutoFillBackground(true);
	QPalette palette;
	palette.setBrush(QPalette::Window, QColor(255, 255, 255));
	palette.setBrush(backgroundRole(), QBrush(loadIcon("mover.png")));
	setPalette(palette);
//	setErasePixmap(loadIcon("mover.png"));
	currView = pa;
	rulerGesture = new RulerGesture(currView, RulerGesture::ORIGIN);
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
	currView->startGesture(rulerGesture);
}

void RulerMover::mouseReleaseEvent(QMouseEvent *m)
{
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	if (Mpressed)
	{
		rulerGesture->mouseReleaseEvent(m);
		Mpressed = false;
	}
}

void RulerMover::mouseMoveEvent(QMouseEvent *m)
{
	if (Mpressed)
	{
		rulerGesture->mouseMoveEvent(m);
	}
}


void RulerMover::paintEvent(QPaintEvent* e)
{
}
