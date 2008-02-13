
/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/



#include "canvasgesture_rectselect.h"
#include "canvas.h"
#include "scribusview.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QInputMethodEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QRubberBand>

void RectSelect::enterEvent(QEvent * e){}
void RectSelect::leaveEvent(QEvent * e){}


void RectSelect::prepare(QPoint start)
{
	if (!m_rectangle)
		m_rectangle = new QRubberBand(QRubberBand::Rectangle);
	setStart(start);
	m_rectangle->setGeometry(m_start.x(), m_start.y(), 1, 1);
}

void RectSelect::clear()
{
	m_rectangle->hide();
	m_start = QPoint(0,0);
}


void RectSelect::activate(bool)
{
	prepare(m_start);
	m_rectangle->show();
}

void RectSelect::deactivate(bool)
{
	m_rectangle->hide();
}

void RectSelect::setStart(QPoint globalPos)
{
	m_start = globalPos;
}

void RectSelect::setEnd(QPoint globalPos)
{
	m_rectangle->setGeometry(QRect(m_start.x(), 
								   m_start.y(), 
								   globalPos.x()-m_start.x(), 
								   globalPos.y()-m_start.y())
							 .normalized());	
}


QRectF RectSelect::result() const
{ 
	return m_canvas->globalToCanvas(m_rectangle->geometry());
}

void RectSelect::mousePressEvent(QMouseEvent *m)
{
	prepare(m->globalPos());
	m->accept();
}


void RectSelect::mouseReleaseEvent(QMouseEvent *m)
{
//	qDebug() << "RectSelect::mouseRelease" << m->globalPos();
	setEnd(m->globalPos());
	m->accept();
	m_view->stopGesture();
}

void RectSelect::mouseMoveEvent(QMouseEvent *m)
{
	setEnd(m->globalPos());
	m->accept();
}
