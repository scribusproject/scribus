
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
#include "selectionrubberband.h"

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


void RectSelect::prepare(QPointF globalStartPos)
{
	if (!m_selectionRubberBand)
		m_selectionRubberBand = new SelectionRubberBand(QRubberBand::Rectangle, m_view);
	setStart(globalStartPos);
//FIXME Move to new code like SelectionRubberBand
	QPoint selectionPoint = m_view->mapFromGlobal(globalStartPos).toPoint();
	m_selectionRubberBand->setWindowOpacity(0.5);
	m_selectionRubberBand->setGeometry(QRect(selectionPoint, selectionPoint));
}

void RectSelect::clear()
{
	m_selectionRubberBand->hide();
	m_start = QPointF(0,0);
}


void RectSelect::activate(bool fromGesture)
{
	CanvasGesture::activate(fromGesture);
	prepare(m_start);
	m_selectionRubberBand->show();
}

void RectSelect::deactivate(bool fromGesture)
{
	m_selectionRubberBand->hide();
	CanvasGesture::deactivate(fromGesture);
}

void RectSelect::setStart(QPointF globalPos)
{
	m_start = globalPos;
}

void RectSelect::setEnd(QPointF globalPos)
{
	QPointF startPoint = m_view->mapFromGlobal(m_start);
	QPointF endPoint = m_view->mapFromGlobal(globalPos);
	m_selectionRubberBand->setGeometry(QRect(startPoint.toPoint(), endPoint.toPoint()).normalized());
}


QRectF RectSelect::result() const
{
	QRect geom = m_selectionRubberBand->geometry().normalized();
	geom = QRect(m_view->mapToGlobal(geom.topLeft()), m_view->mapToGlobal(geom.bottomRight()));
	return m_canvas->globalToCanvas(geom);
}

void RectSelect::mousePressEvent(QMouseEvent *m)
{
	prepare(m->globalPosition());
	m->accept();
}


void RectSelect::mouseReleaseEvent(QMouseEvent *m)
{
//	qDebug() << "RectSelect::mouseRelease" << m->globalPosition();
	setEnd(m->globalPosition());
	m->accept();
	m_view->stopGesture();
}

void RectSelect::mouseMoveEvent(QMouseEvent *m)
{
	setEnd(m->globalPosition());
	m->accept();
}

void RectSelect::drawControls(QPainter* p)
{
	m_delegate->drawControls(p);
}
