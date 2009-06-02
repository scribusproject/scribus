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

#include "canvasgesture_linemove.h"

//#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

#include "canvas.h"
#include "pageitem_line.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "util_math.h"


void LineMove::clear()
{
	m_haveLineItem = false;
}


void LineMove::prepare(QPointF start, QPointF end)
{
	m_haveLineItem = false;
	setStartPoint(start);
	setStartPoint(end);
}


void LineMove::prepare(PageItem_Line* line, bool useOriginAsEndpoint)
{
	m_haveLineItem = (line != NULL);
	if (!m_haveLineItem)
		return;
	m_useOriginAsEndpoint = useOriginAsEndpoint;
	m_line = line;
	setStartPoint(QPointF(m_line->xPos(), m_line->yPos()));
	setEndPoint(QPointF(m_line->xPos() + m_line->width(), m_line->yPos()));
	setRotation(m_line->rotation());
	if (m_useOriginAsEndpoint)
	{
		QPointF tmp = startPoint();
		setStartPoint(endPoint());
		setEndPoint(tmp);
	}
}


double LineMove::rotation() const
{
	double rot = xy2Deg(m_bounds.width(), m_bounds.height());
	if (rot < 0.0)
		return 360 + rot;
	else
		return rot;
}


void LineMove::setRotation(double rot)
{
	m_bounds.setSize(length() * QSizeF(cosd(rot), sind(rot)));
}


double LineMove::length() const
{
	return qMax(0.01, distance(m_bounds.width(), m_bounds.height()));
}


void LineMove::setStartPoint(QPointF p)
{
	m_bounds.setTopLeft(p);
}


void LineMove::setEndPoint(QPointF p)
{
	m_bounds.setBottomRight(p);
}


void LineMove::activate(bool flag)
{
//	qDebug() << "LineMove::activate" << flag << m_bounds;	
}



void LineMove::deactivate(bool flag)
{
//	qDebug() << "LineMove::deactivate" << flag;
	m_haveLineItem = false;
}



void LineMove::drawControls(QPainter* p) 
{ 
	p->save();
	p->scale(m_canvas->scale(), m_canvas->scale());
	p->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	p->setBrush(Qt::NoBrush);
	p->setPen(QPen(Qt::black, 1.0 / m_canvas->scale(), Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
	p->drawLine(m_bounds.topLeft(), m_bounds.bottomRight());
	p->restore();
}



void LineMove::mouseReleaseEvent(QMouseEvent *m)
{
	adjustBounds(m);
	if (m_haveLineItem)
	{
		doResize();
		m_doc->setRedrawBounding(m_line);
		m_view->resetMousePressed();
		m_line->checkChanges();
		m_line->update();
	}
	m->accept();
	m_canvas->update();
//	qDebug() << "LineMove::mouseRelease" << m_line->xPos() << "," << m_line->yPos() << "@" << m_line->rotation() << m_line->width() << "x" << m_line->height();
	m_view->stopGesture();
}


void LineMove::doResize()
{
	if (m_useOriginAsEndpoint)
	{
		m_line->setXYPos(m_bounds.right(), m_bounds.bottom());
		double rot = rotation();
		m_line->setRotation(rot < 180? rot + 180 : rot - 180);
	}
	else
	{
		m_line->setXYPos(m_bounds.x(), m_bounds.y());
		m_line->setRotation(rotation());
	}
	m_line->setWidth(length());
	m_line->setHeight(1.0);
	m_line->updateClip();
//	qDebug() << "LineMove::doresize" << m_line->xPos() << "," << m_line->yPos() << "@" << m_line->rotation() << m_line->width() << "x" << m_line->height();
}


void LineMove::mouseMoveEvent(QMouseEvent *m)
{
	adjustBounds(m);
	if (m_haveLineItem)
	{
		doResize();
		double angle = rotation();
		if (angle > 0)
			angle = 360 - angle;
		m_canvas->displaySizeHUD(m->globalPos(), length(), fabs(angle), true);
	}
	m->accept();
	m_canvas->repaint();
}



void LineMove::adjustBounds(QMouseEvent *m)
{
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	bool constrainRatio = ((m->modifiers() & Qt::ControlModifier) != Qt::NoModifier);

	double newX = mousePointDoc.x();
	double newY = mousePointDoc.y();
	
	if (m_doc->useRaster)
	{
		newX = qRound(newX / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
		newY = qRound(newY / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
	}
	//<<#8099
	FPoint np2 = m_doc->ApplyGridF(FPoint(newX, newY));
	double nx = np2.x();
	double ny = np2.y();
	m_doc->ApplyGuides(&nx, &ny);
	newX = qRound(nx);
	newY = qRound(ny);
	//>>#8099

	m_bounds.setBottomRight(QPointF(newX, newY));
	//Constrain rotation angle, when the mouse is being dragged around for a new line
	if (constrainRatio)
	{
		double newRot = rotation();
		newRot = constrainAngle(newRot, m_doc->toolSettings.constrain);
		setRotation(newRot);
	}

//	qDebug() << "LineMove::adjustBounds" << m_bounds << rotation() << length() << m_bounds.bottomRight();
	m_view->updateCanvas(m_bounds.normalized().adjusted(-10, -10, 20, 20));
}



void LineMove::mousePressEvent(QMouseEvent *m)
{
	PageItem_Line* line = m_doc->m_Selection->count() == 1 ? m_doc->m_Selection->itemAt(0)->asLine() : NULL;
	if (line)
	{
		bool hitsOrigin = m_canvas->hitsCanvasPoint(m->globalPos(), line->xyPos());
		prepare(line, hitsOrigin);
		// now we also know the line's endpoint:
		bool hitsEnd = m_canvas->hitsCanvasPoint(m->globalPos(), endPoint());
		m_haveLineItem = hitsOrigin || hitsEnd;
	}
	else
	{
		FPoint point = m_canvas->globalToCanvas(m->globalPos());
		setStartPoint(QPointF(point.x(), point.y()));
		setEndPoint(QPointF(point.x(), point.y()));
		m_haveLineItem = false;
	}
	if (m_haveLineItem)
		m->accept();
}
