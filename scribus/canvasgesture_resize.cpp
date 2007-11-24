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

#include "canvasgesture_resize.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QRubberBand>

#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"



void ResizeGesture::activate(bool flag)
{
	qDebug() << "ResizeGesture::activate" << flag;
	
	if (m_doc->m_Selection->count() == 0)
	{
		m_view->stopGesture();
	}
	else if (m_doc->m_Selection->isMultipleSelection())
	{
		double gx, gy, gh, gw;
		m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		m_bounds = QRect(m_canvas->canvasToGlobal(QPointF(gx,gy)), 
						 QSize(gw * m_canvas->scale(), 
							   gh * m_canvas->scale()));
		m_rotation = 0.0;
	}
	else // we keep m_bounds non-rotated
	{
		PageItem* currItem = m_doc->m_Selection->itemAt(0);
		m_bounds = QRect(m_canvas->canvasToGlobal(currItem->xyPos()), 
						 QSize(currItem->width() * m_canvas->scale(), 
							   currItem->height() * m_canvas->scale()));
		m_rotation = currItem->rotation();
		currItem->OldB2 = currItem->width();
		currItem->OldH2 = currItem->height();
	}
}



void ResizeGesture::deactivate(bool flag)
{
	qDebug() << "ResizeGesture::deactivate" << flag;
}



void ResizeGesture::drawControls(QPainter* p) 
{ 
	QColor drawColor = qApp->palette().color(QPalette::Active, QPalette::Highlight);
	QRect localRect = m_bounds.translated(-m_canvas->mapToGlobal(QPoint(0,0)));
	p->save();
//	p->setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::FlatCap, Qt::MiterJoin));
//	p->drawRect(localRect);
	if (m_rotation != 0)
	{
		p->translate(localRect.x(), localRect.y());
		p->rotate(m_rotation);
		p->translate(-localRect.x(), -localRect.y());
	}
	p->setPen(QPen(drawColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	drawColor.setAlpha(64);
	p->setBrush(drawColor);
	p->drawRect(localRect);
	//	p->setPen(Qt::darkMagenta);
	//	p->drawLine(localRect.topLeft(), localRect.bottomRight());
	p->restore();
}



void ResizeGesture::mouseReleaseEvent(QMouseEvent *m)
{
	adjustBounds(m);
	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	QRectF newBounds = m_canvas->globalToCanvas(m_bounds.normalized());
	
	qDebug() << "ResizeGesture::release: new bounds" << newBounds;	
	if (m_doc->m_Selection->isMultipleSelection())
	{
		double gx, gy, gh, gw;
		m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		QRectF oldBounds(gx, gy, gw, gh);
		double scx = oldBounds.width() == 0? 1.0 : newBounds.width() / oldBounds.width();
		double scy = oldBounds.height() == 0? 1.0 : newBounds.height() / oldBounds.height();
		//CB #3012 only scale text in a group if alt is pressed
		if ((currItem->itemType() == PageItem::TextFrame) && (m->modifiers() & Qt::AltModifier))
			m_view->scaleGroup(scx, scy, true);
		else
			m_view->scaleGroup(scx, scy, false);
		double dx = newBounds.x() - oldBounds.x();
		double dy = newBounds.y() - oldBounds.y();
		if (dx != 0 || dy != 0)
			m_view->moveGroup(dx, dy);
	}
	else
	{
		currItem->setXYPos(newBounds.x(), newBounds.y());
		currItem->setWidth(newBounds.width());
		currItem->setHeight(newBounds.height());
		// rotation does not change
	}
	currItem->updateClip();
	m_doc->setRedrawBounding(currItem);
	currItem->invalidateLayout();
	currItem->update();
	m_canvas->update();
	m_view->stopGesture();
}



void ResizeGesture::mouseMoveEvent(QMouseEvent *m)
{
	adjustBounds(m);
	m_canvas->repaint();
}



void ResizeGesture::adjustBounds(QMouseEvent *m)
{
	// snap to guides
	// snap to grid
	// proportional resize
	QPoint point = m->globalPos();
	QPoint oldXY = m_bounds.topLeft();
	QMatrix mat;
	
	if (m_rotation != 0)
	{
		// rotate point around item position
		mat.translate(m_bounds.x(), m_bounds.y());
		mat.rotate(-m_rotation);
		mat.translate(-m_bounds.x(), -m_bounds.y());
//		qDebug() << "resize rotated" << m_rotation << "°" << m_bounds << mat << ":" << point-m_bounds.topLeft() << "-->" << mat.map(point)-m_bounds.topLeft();
		point = mat.map(point);
	}
	switch (m_handle)
	{
		case Canvas::NORTHWEST:
		case Canvas::NORTH:
		case Canvas::NORTHEAST:
//			qDebug() << "ResizeGesture: top to" << point.y();
			m_bounds.setTop(point.y());
			break;
		case Canvas::SOUTHWEST:
		case Canvas::SOUTH:
		case Canvas::SOUTHEAST:
//			qDebug() << "ResizeGesture: bottom to" << point.y();
			m_bounds.setBottom(point.y());
			break;
		default:
			break;
	}
	switch (m_handle)
	{
		case Canvas::NORTHWEST:
		case Canvas::WEST:
		case Canvas::SOUTHWEST:
//			qDebug() << "ResizeGesture: left to" << point.x();
			m_bounds.setLeft(point.x());
			break;
		case Canvas::NORTHEAST:
		case Canvas::EAST:
		case Canvas::SOUTHEAST:
//			qDebug() << "ResizeGesture: right to" << point.x();
			m_bounds.setRight(point.x());
			break;
		default:
			break;
	}
	if (m_rotation != 0)
	{
		m_bounds.moveTo(mat.inverted().map(m_bounds.topLeft()));
	}
}



void ResizeGesture::mousePressEvent(QMouseEvent *m)
{
	FPoint point = m_canvas->globalToCanvas(m->globalPos());
	if (m_doc->m_Selection->isMultipleSelection())
	{
		double gx, gy, gh, gw;
		m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		m_handle = m_canvas->frameHitTest(QPointF(point.x(), point.y()), QRectF(gx,gy,gw,gh));
	}
	else
	{
		m_handle = m_canvas->frameHitTest(QPointF(point.x(), point.y()), m_doc->m_Selection->itemAt(0));
	}
}
