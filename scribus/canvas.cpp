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


#include <QDebug>


#include "canvas.h"

#include "page.h"
#include "pageitem_textframe.h"
#include "prefsmanager.h"
#include "scribusdoc.h"
#include "scpainter.h"
#include "selection.h"
#include "util.h"

#define DRAW_DEBUG_LINES 0

static QPoint contentsToViewport(QPoint p)
{
	return p;
}


void CanvasViewMode::init()
{	
	scale = 1;
	
	previewMode = false;
	viewAsPreview = false;
	previewVisual = -1;
	
	m_MouseButtonPressed = false;
	operItemMoving = false;
	operItemResizing = false;
	operItemResizeInEditMode = false;
	redrawPolygon.resize(0);
	linkedFramesToShow.clear();
	
	drawSelectedItemsWithControls = false;
	drawFramelinksWithContents = false;
	
	specialRendering = false;
	firstSpecial = false;
	forceRedraw = false;
	m_buffer = QPixmap();
	m_bufferRect = QRect();
	oldMinCanvasCoordinate = FPoint();
}
	
Canvas::Canvas(ScribusDoc* doc, QWidget* parent) : QWidget(parent), m_doc(doc)
{
	setAutoFillBackground(true);
	m_viewMode.init();
}


FPoint Canvas::localToCanvas(QPoint p) const
{
	return FPoint(p.x() / m_viewMode.scale + m_doc->minCanvasCoordinate.x() , 
				  p.y() / m_viewMode.scale + m_doc->minCanvasCoordinate.y());	
}


FPoint Canvas::globalToCanvas(QPoint p) const
{
	return localToCanvas(p - mapToGlobal(QPoint(0,0)));
}


QPoint Canvas::canvasToLocal(FPoint p) const
{
	return 	QPoint(qRound((p.x() - m_doc->minCanvasCoordinate.x()) * m_viewMode.scale),
				   qRound((p.y() - m_doc->minCanvasCoordinate.y()) * m_viewMode.scale));
}

QPoint Canvas::canvasToLocal(QPointF p) const
{
	return 	QPoint(qRound((p.x() - m_doc->minCanvasCoordinate.x()) * m_viewMode.scale),
				   qRound((p.y() - m_doc->minCanvasCoordinate.y()) * m_viewMode.scale));
}

QPoint Canvas::canvasToGlobal(FPoint p) const
{
	return mapToGlobal(QPoint(0,0)) + canvasToLocal(p);
}

QPoint Canvas::canvasToGlobal(QPointF p) const
{
	return mapToGlobal(QPoint(0,0)) + canvasToLocal(p);
}


QRectF Canvas::globalToCanvas(QRect p) const
{
	FPoint org = globalToCanvas(p.topLeft());
	return QRectF(org.x(), org.y(), p.width() / m_viewMode.scale, p.height() / m_viewMode.scale);
}


bool Canvas::hitsCanvasPoint(QPoint globalPoint, FPoint canvasPoint) const
{
	QPoint localPoint1 = globalPoint - mapToGlobal(QPoint(0,0));
	QPoint localPoint2 = canvasToLocal(canvasPoint);
	int radius = m_doc->guidesSettings.grabRad;
	return qAbs(localPoint1.x() - localPoint2.x()) < radius
		&& qAbs(localPoint1.y() - localPoint2.y()) < radius;
}

bool Canvas::hitsCanvasPoint(QPoint globalPoint, QPointF canvasPoint) const
{
	QPoint localPoint1 = globalPoint - mapToGlobal(QPoint(0,0));
	QPoint localPoint2 = canvasToLocal(canvasPoint);
	int radius = m_doc->guidesSettings.grabRad;
	return qAbs(localPoint1.x() - localPoint2.x()) < radius
		&& qAbs(localPoint1.y() - localPoint2.y()) < radius;
}


/*!
  returns -1 if canvasPoint is outside the frame + grabradius.
  returns frameHandle if canvasPoint is near a framehandle
  otherwise 0
 */
Canvas::FrameHandle Canvas::frameHitTest(QPointF canvasPoint, PageItem* item) const
{
	return frameHitTest(item->getTransform().inverted().map(canvasPoint), QRectF(0, 0, item->width(), item->height()));
}


static double length2(const QPointF& p)
{
	return p.x()*p.x() + p.y()*p.y();
}

Canvas::FrameHandle Canvas::frameHitTest(QPointF canvasPoint, QRectF frame) const
{
	FrameHandle result = INSIDE;
	const double radius = m_doc->guidesSettings.grabRad / m_viewMode.scale;
	const double radius2 = radius * radius;
	double resultDistance = radius2 * 10.0; // far off
	
	const double frameWidth = frame.width();
	const double frameHeight = frame.height();
	const QPointF frameOrigin = frame.topLeft();
	
	if (canvasPoint.x() < frameOrigin.x() - radius ||
		canvasPoint.x() > frameOrigin.x() + frameWidth + radius ||
		canvasPoint.y() < frameOrigin.y() - radius ||
		canvasPoint.y() > frameOrigin.y() + frameHeight + radius)
	{
		return OUTSIDE;
	}
	
	QPointF framePoint = frameOrigin;
	double distance = length2(canvasPoint - framePoint);
	if (distance < radius2 && distance < resultDistance)
	{
		result = NORTHWEST;
		resultDistance = distance;
	}
	
	framePoint.setX(frameOrigin.x() + frameWidth/2);
	distance = length2(canvasPoint - framePoint);
	if (distance < radius2 && distance < resultDistance)
	{
		result = NORTH;
		resultDistance = distance;
	}
	
	framePoint.setX(frameOrigin.x() + frameWidth);
	distance = length2(canvasPoint - framePoint);
	if (distance < radius2 && distance < resultDistance)
	{
		result = NORTHEAST;
		resultDistance = distance;
	}
	
	framePoint.setY(frameOrigin.y() + frameHeight/2);
	distance = length2(canvasPoint - framePoint);
	if (distance < radius2 && distance < resultDistance)
	{
		result = EAST;
		resultDistance = distance;
	}
	
	framePoint.setY(frameOrigin.y() + frameHeight);
	distance = length2(canvasPoint - framePoint);
	if (distance < radius2 && distance < resultDistance)
	{
		result = SOUTHEAST;
		resultDistance = distance;
	}
	
	framePoint.setX(frameOrigin.x() + frameWidth/2);
	distance = length2(canvasPoint - framePoint);
	if (distance < radius2 && distance < resultDistance)
	{
		result = SOUTH;
		resultDistance = distance;
	}
	
	framePoint.setX(frameOrigin.x());
	distance = length2(canvasPoint - framePoint);
	if (distance < radius2 && distance < resultDistance)
	{
		result = SOUTHWEST;
		resultDistance = distance;
	}
	
	framePoint.setY(frameOrigin.y() + frameHeight/2);
	distance = length2(canvasPoint - framePoint);
	if (distance < radius2 && distance < resultDistance)
	{
		result = WEST;
		//resultDistance = distance;
	}
	
	return result;
}


PageItem* Canvas::itemUnderCursor(QPoint globalPos, PageItem* itemAbove, bool allowInGroup, bool allowMasterItems) const
{
	PageItem* currItem;
	QRectF mouseArea = globalToCanvas(QRect(globalPos, QSize(2*m_doc->guidesSettings.grabRad, 2*m_doc->guidesSettings.grabRad)));
	// look for masterpage items first
	if (allowMasterItems && !m_doc->masterPageMode() && m_doc->currentPage()->FromMaster.count() != 0)
	{
		Page* Mp = m_doc->MasterPages.at(m_doc->MasterNames[m_doc->currentPage()->MPageNam]);
		// if itemAbove is given, we expect to find it among the masterpage items of this page
		int currNr = itemAbove? m_doc->currentPage()->FromMaster.indexOf(itemAbove)-1 : m_doc->currentPage()->FromMaster.count()-1;
		if (currNr < 0)
			return NULL;
		while (currNr >= 0)
		{
			currItem = m_doc->currentPage()->FromMaster.at(currNr);
			QMatrix itemPos;
			if ((currItem->LayerNr == m_doc->activeLayer()) && (!m_doc->layerLocked(currItem->LayerNr)))
			{
				if (!currItem->ChangedMasterItem)
				{
					itemPos.translate(-Mp->xOffset() + m_doc->currentPage()->xOffset(), -Mp->yOffset() + m_doc->currentPage()->yOffset());
				}
				currItem->getTransform(itemPos);
				QPainterPath currPath(itemPos.map(QPointF(0,0)));
				currPath.lineTo(itemPos.map(QPointF(currItem->width(), 0)));
				currPath.lineTo(itemPos.map(QPointF(currItem->width(), currItem->height())));
				currPath.lineTo(itemPos.map(QPointF(0, currItem->height())));
				currPath.closeSubpath();
				QPainterPath currClip;
				currClip.addPolygon(itemPos.map(QPolygonF(currItem->Clip)));
				currClip.closeSubpath();
				if (currPath.intersects(mouseArea) || currClip.intersects(mouseArea))
//				if ((QRegion(itemPos.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo.toRect())) ||
//					(QRegion(currItem->Clip * itemPos).contains(mpo.toRect())))
				{
					if (currItem->Groups.count() != 0 && !allowInGroup)
					{
						// look for the group control
						// FIXME: Items or MasterPageItems??
						for (int ga=0; ga < m_doc->Items->count(); ++ga)
						{
							if (m_doc->Items->at(ga)->Groups.count() != 0 
								&& m_doc->Items->at(ga)->Groups.top() == currItem->Groups.top() 
								&& m_doc->Items->at(ga)->controlsGroup())
							{
								currItem = m_doc->Items->at(ga);
								break;
							}
						}
					}
					return currItem;
				}
			}
			--currNr;
		}
	}
	// now look for normal items
	if (m_doc->Items->count() == 0)
		return NULL;

	int currNr = itemAbove? m_doc->Items->indexOf(currItem)-1 : m_doc->Items->count()-1;
	while (currNr >= 0)
	{
		currItem = m_doc->Items->at(currNr);
		if ((m_doc->masterPageMode())  && (!((currItem->OwnPage == -1) || (currItem->OwnPage == static_cast<int>(m_doc->currentPage()->pageNr())))))
			continue;
		if ((currItem->LayerNr == m_doc->activeLayer()) && (!m_doc->layerLocked(currItem->LayerNr)))
		{
			QMatrix itemPos = currItem->getTransform();
			QPainterPath currPath(itemPos.map(QPointF(0,0)));
			currPath.lineTo(itemPos.map(QPointF(currItem->width(), 0)));
			currPath.lineTo(itemPos.map(QPointF(currItem->width(), currItem->height())));
			currPath.lineTo(itemPos.map(QPointF(0, currItem->height())));
			currPath.closeSubpath();
			QPainterPath currClip;
			currClip.addPolygon(itemPos.map(QPolygonF(currItem->Clip)));
			currClip.closeSubpath();
			if (currPath.intersects(mouseArea) || currClip.intersects(mouseArea))
//				if ((QRegion(itemPos.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo.toRect())) ||
//				(QRegion(currItem->Clip * itemPos).contains(mpo.toRect())))
			{
				if (currItem->Groups.count() != 0 && !allowInGroup)
				{
					// look for the group control
					for (int ga=0; ga < m_doc->Items->count(); ++ga)
					{
						if (m_doc->Items->at(ga)->Groups.count() != 0 
							&& m_doc->Items->at(ga)->Groups.top() == currItem->Groups.top() 
							&& m_doc->Items->at(ga)->controlsGroup())
						{
							currItem = m_doc->Items->at(ga);
							break;
						}
					}
				}
				return currItem;				
			}
		}
		--currNr;
	}
	return NULL;
}


/**
  Actually we have at least three super-layers:
  - background (page outlines, guides if below)
  - content (master & page objects)
  - controls (selection, mode specific mrks, guides if above)
  For export we only need the contents.
*/
void Canvas::paintEvent ( QPaintEvent * p )
{
	if (m_doc->isLoading())
		return;

	QPainter qp(this);
	
	if ((m_viewMode.firstSpecial) || ((!m_viewMode.m_bufferRect.contains(p->rect())) || (m_doc->minCanvasCoordinate != m_viewMode.oldMinCanvasCoordinate) && !(m_doc->appMode == modeEditClip)))
	{
		if (m_viewMode.m_bufferRect.intersects(p->rect()))
			m_viewMode.m_bufferRect = m_viewMode.m_bufferRect.united(p->rect());
		else
			m_viewMode.m_bufferRect = p->rect();
		m_viewMode.m_buffer = QPixmap(m_viewMode.m_bufferRect.width(), m_viewMode.m_bufferRect.height());
		QPainter bufp(&m_viewMode.m_buffer);
		bufp.translate(-m_viewMode.m_bufferRect.x(), -m_viewMode.m_bufferRect.y());
		qDebug() << "fill Buffer:" << m_viewMode.m_bufferRect << "special:" << m_viewMode.specialRendering;
		bufp.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		drawContents(&bufp, 
					 m_viewMode.m_bufferRect.x() + m_doc->minCanvasCoordinate.x() * m_viewMode.scale,
					 m_viewMode.m_bufferRect.y() + m_doc->minCanvasCoordinate.y() * m_viewMode.scale, 
					 m_viewMode.m_bufferRect.width(), m_viewMode.m_bufferRect.height());
		bufp.end();
		m_viewMode.firstSpecial = false;
		m_viewMode.oldMinCanvasCoordinate = m_doc->minCanvasCoordinate;
	}
	if (m_viewMode.specialRendering)
	{
		QPainter pixmapp;
		QPixmap pixmap = m_viewMode.m_buffer; // copy
		pixmapp.begin(&pixmap);
		pixmapp.translate(-m_viewMode.m_bufferRect.x(), -m_viewMode.m_bufferRect.y());
		pixmapp.setRenderHint(QPainter::Antialiasing, true);
		pixmapp.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		drawControls(&pixmapp);
#if DRAW_DEBUG_LINES
		pixmapp.setPen(Qt::red);
		pixmapp.drawLine(p->rect().x(), p->rect().y(), p->rect().x() + p->rect().width(), p->rect().y() + p->rect().height());
		pixmapp.drawLine(p->rect().x() + p->rect().width(), p->rect().y(), p->rect().x(), p->rect().y() + p->rect().height());
#endif
		pixmapp.end();
//		qp.resetMatrix();
#if DRAW_DEBUG_LINES
		qDebug() << "specialRendering:" << p->rect() << "from" << m_viewMode.m_bufferRect.x() << m_viewMode.m_bufferRect.y();
#endif
		int xV = p->rect().x() - m_viewMode.m_bufferRect.x();
		int yV = p->rect().y() - m_viewMode.m_bufferRect.y();
		int wV = p->rect().width();
		int hV = p->rect().height();
		if (xV < 0)
		{
			wV += xV;
			xV = 0;
		}
		if (yV < 0)
		{
			hV += yV;
			yV = 0;
		}
		if (hV > 0 && wV > 0)
		{
			qp.drawPixmap(p->rect().x(), p->rect().y(), pixmap, xV, yV,  wV, hV);		
		}
#if DRAW_DEBUG_LINES
		qDebug() << "specialRendering" << xV << yV << wV << hV << "at" << p->rect().x() << p->rect().y();
		qp.setPen(Qt::green);
		qp.drawLine(p->rect().x(), p->rect().y(), p->rect().x() + p->rect().width(), p->rect().y() + p->rect().height());
		qp.drawLine(p->rect().x() + p->rect().width(), p->rect().y(), p->rect().x(), p->rect().y() + p->rect().height());
#endif
	}
	else
	{
		//drawContents(&qp, p->rect().x(), p->rect().y(), p->rect().width(), p->rect().height());
		qp.drawPixmap(p->rect().x(), p->rect().y(), m_viewMode.m_buffer, p->rect().x() - m_viewMode.m_bufferRect.x(), p->rect().y() - m_viewMode.m_bufferRect.y(),  p->rect().width(), p->rect().height());
#if DRAW_DEBUG_LINES
		qp.setPen(Qt::blue);
		qp.drawLine(p->rect().x(), p->rect().y(), p->rect().x() + p->rect().width(), p->rect().y() + p->rect().height());
		qp.drawLine(p->rect().x() + p->rect().width(), p->rect().y(), p->rect().x(), p->rect().y() + p->rect().height());
#endif
		if ((m_doc->m_Selection->count() != 0) && !(m_viewMode.operItemMoving || m_viewMode.operItemResizing) && (m_doc->appMode != modeDrawBezierLine))
		{
			qp.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
			drawControlsSelection(&qp, m_doc->m_Selection->itemAt(0));
		}		
	}
	m_viewMode.forceRedraw = false;
}


void Canvas::drawContents(QPainter *psx, int clipx, int clipy, int clipw, int cliph)
{
	uint docPagesCount=m_doc->Pages->count();
	QPoint vr = contentsToViewport(QPoint(clipx, clipy));
	ScPainter *painter=0;
	QImage img = QImage(clipw, cliph, QImage::Format_ARGB32);
	painter = new ScPainter(&img, img.width(), img.height(), 1.0, 0);
	painter->clear(palette().color(QPalette::Window));
	painter->newPath();
	painter->moveTo(0, 0);
	painter->lineTo(clipw, 0);
	painter->lineTo(clipw, cliph);
	painter->lineTo(0, cliph);
	painter->closePath();
	painter->setClipPath();
	painter->translate(-clipx, -clipy);
	painter->setZoomFactor(m_viewMode.scale);
//	painter->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	painter->setLineWidth(1);
	painter->setFillMode(ScPainter::Solid);
	if (!m_doc->masterPageMode())
	{
		drawBackgroundPageOutlines(painter, clipx, clipy, clipw, cliph);
		
		if (m_viewMode.viewAsPreview)
		{
			FPointArray PoLine;
			getClipPathForPages(&PoLine);
			painter->beginLayer(1.0, 0, &PoLine);
		}
		else
			painter->beginLayer(1.0, 0);
		
		if ((m_doc->guidesSettings.before) && (!m_viewMode.viewAsPreview))
		{
			drawGuides(painter, clipx, clipy, clipw, cliph);
		}
		for (uint a = 0; a < docPagesCount; ++a)
		{
			DrawMasterItems(painter, m_doc->Pages->at(a), QRect(clipx, clipy, clipw, cliph));
		}
		DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		painter->endLayer();
		if ((!m_doc->guidesSettings.before) && (!m_viewMode.viewAsPreview))
		{
			drawGuides(painter, clipx, clipy, clipw, cliph);
		}
	}
	else // masterPageMode
	{			
		drawBackgroundMasterpage(painter, clipx, clipy, clipw, cliph);
		
		painter->beginLayer(1.0, 0);
		DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		painter->endLayer();
		
		double x = m_doc->scratch.Left * m_viewMode.scale;
		double y = m_doc->scratch.Top * m_viewMode.scale;
		double w = m_doc->currentPage()->width() * m_viewMode.scale;
		double h = m_doc->currentPage()->height() * m_viewMode.scale;
		QRectF drawRect = QRectF(x, y, w+5, h+5);
//		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if ((!m_doc->guidesSettings.before) && (drawRect.intersects(QRect(clipx, clipy, clipw, cliph))))
			DrawPageMarks(painter, m_doc->currentPage(), QRect(clipx, clipy, clipw, cliph));
	}
	if (((m_doc->m_Selection->count() != 0) || (m_viewMode.linkedFramesToShow.count() != 0))  && (!m_viewMode.viewAsPreview))
	{
		drawFrameLinks(painter);
	}
	painter->end();
	psx->drawImage(clipx, clipy, img);
	delete painter;
	painter=NULL;
}

void Canvas::drawControls(QPainter *psx)
{
				/*if (redrawMode == 1) // move ruler origin...
{
				pp.resetMatrix();
				pp.setBrush(Qt::NoBrush);
				pp.setPen(QPen(Qt::black, 1.0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
				QPoint nXY = redrawPolygon.point(0);
				pp.drawLine(0, nXY.y(), this->width(), nXY.y());
				pp.drawLine(nXY.x(), 0, nXY.x(), this->height());
				redrawPolygon.clear();
}*/
	psx->save();
	if ((m_doc->appMode == modeDrawBezierLine) && (!m_viewMode.redrawPolygon.isEmpty()) && (m_doc->m_Selection->count() != 0))
	{
		drawControlsBezierCurve(psx, m_doc->m_Selection->itemAt(0));
	}
	if (m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeMeasurementTool))
	{
		drawControlsMeasurementLine(psx);
	}
	if (m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeDrawLine))
	{
		drawControlsDrawLine(psx);
	}
	if (m_doc->appMode == modeEditGradientVectors)
	{
		drawControlsGradientVectors(psx, m_doc->m_Selection->itemAt(0));
	}	
	if (m_viewMode.operItemMoving || m_viewMode.operItemResizing)
	{
		if (m_viewMode.operItemResizing)
		{
			if (!m_viewMode.redrawPolygon.isEmpty())
			{
				if (m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeDrawFreehandLine))
				{
					drawControlsFreehandLine(psx);
				}
				else
				{
					drawControlsHighlightRect(psx);
				}
			}
		}
		else
		{
			drawControlsMovingItemsRect(psx);
		}
	}
	else
	{
		if ((m_doc->m_Selection->count() != 0) && (m_doc->appMode != modeDrawBezierLine))
		{
			drawControlsSelection(psx, m_doc->m_Selection->itemAt(0));
			/*
			 PageItem *currItem = m_doc->m_Selection->itemAt(0);
			 if ((m_doc->appMode == modeEditClip) && (currItem->isSelected()))
			 {
				 drawControlsNodeEditPoints(psx, currItem);
			 }
			 else
			 {
				 drawControlsSelectionSpecial(psx, currItem);
			 }*/
		}
	}
	psx->restore();
}	



void Canvas::drawContentsOld(QPainter *psx, int clipx, int clipy, int clipw, int cliph)
{
	if (m_doc->isLoading())
		return;
//	if (!updateOn)
//		return;
//	QTime tim;
//	tim.start();
	psx->setRenderHint(QPainter::Antialiasing, false);
	psx->setRenderHint(QPainter::SmoothPixmapTransform, false);
	if ((clipw > 0) && (cliph > 0))
	{

		if ((m_viewMode.specialRendering) || (m_doc->appMode == modeEditClip && m_viewMode.specialRendering))
		{
			if (m_viewMode.firstSpecial)
			{
#ifdef Q_WS_MAC
				QPoint viewportOrigin = mapToGlobal(QPoint(0,0));
				m_viewMode.m_buffer = QPixmap::grabWindow(this->winId(), viewportOrigin.x(), viewportOrigin.y(), this->width(), this->height());
				//m_buffer.resize(this->width(), this->height());
				//this->render(m_buffer);
//				qDebug(QString("drawContents:viewport (%1,%2) %3x%4 -> %5x%6").arg(viewportOrigin.x()).arg(viewportOrigin.y())
//					   .arg(this->width()).arg(this->height()).arg(m_buffer.width()).arg(m_buffer.height()));
#else
				m_viewMode.m_buffer = QPixmap::grabWindow(this->winId(), 0, 0);
#endif
				m_viewMode.firstSpecial = false;
			}
			QPainter pp;
			QPixmap ppx = m_viewMode.m_buffer;
			pp.begin(&ppx);
			pp.setRenderHint(QPainter::Antialiasing, true);
			drawControls(&pp);
			pp.end();
			psx->resetMatrix();
			psx->drawPixmap(0,0,ppx);
			return;
		}

		drawContents(psx, clipx, clipy, clipw, cliph);
	}
/*	psx->setRenderHint(QPainter::Antialiasing, true);
	if (operItemMoving || operItemResizing)
	{
		if (operItemResizing)
		{
			if (!redrawPolygon.isEmpty())
			{
				if (m_MouseButtonPressed && (m_doc->appMode == modeDrawFreehandLine))
				{
					psx->resetMatrix();
					QPoint out = contentsToViewport(QPoint(0, 0));
					psx->translate(out.x(), out.y());
					psx->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
					psx->scale(m_viewMode.scale, m_viewMode.scale);
					psx->setBrush(Qt::NoBrush);
					psx->setPen(QPen(Qt::black, 1.0 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
					psx->drawPolyline(redrawPolygon);
					redrawPolygon.clear();
				}
				else
				{
					QColor drawColor = qApp->palette().color(QPalette::Active, QColorGroup::Highlight);
					psx->setPen(QPen(drawColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
					drawColor.setAlpha(64);
					psx->setBrush(drawColor);
					psx->drawPolygon(redrawPolygon);
					redrawPolygon.clear();
				}
			}
		}
		else
		{
			if (m_doc->m_Selection->count() != 0)
			{
				uint selectedItemCount = m_doc->m_Selection->count();
				PageItem *currItem = m_doc->m_Selection->itemAt(0);
				if (selectedItemCount < moveWithBoxesOnlyThreshold)
				{
					for (uint cu = 0; cu < selectedItemCount; cu++)
					{
						currItem = m_doc->m_Selection->itemAt(cu);
						psx->resetMatrix();
						QPoint out = contentsToViewport(QPoint(0, 0));
						psx->translate(out.x(), out.y());
						psx->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
						Transform(currItem, psx);
						psx->setBrush(Qt::NoBrush);
						psx->setPen(QPen(Qt::black, 1.0 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
						if (selectedItemCount < moveWithFullOutlinesThreshold)
						{
							if (!(currItem->asLine()))
								currItem->DrawPolyL(psx, currItem->Clip);
							else
							{
								if (currItem->asLine())
								{
									int lw2 = 1;
									int lw = 1;
									Qt::PenCapStyle le = Qt::FlatCap;
									if (currItem->NamedLStyle.isEmpty())
									{
										lw2 = qRound(currItem->lineWidth()  / 2.0);
										lw = qRound(qMax(currItem->lineWidth(), 1.0));
										le = currItem->PLineEnd;
									}
									else
									{
										multiLine ml = m_doc->MLineStyles[currItem->NamedLStyle];
										lw2 = qRound(ml[ml.size()-1].Width  / 2.0);
										lw = qRound(qMax(ml[ml.size()-1].Width, 1.0));
										le = static_cast<Qt::PenCapStyle>(ml[ml.size()-1].LineEnd);
									}
									if (le != Qt::FlatCap)
										psx->drawRect(-lw2, -lw2, qRound(currItem->width())+lw, lw);
									else
										psx->drawRect(-1, -lw2, qRound(currItem->width()), lw);
								}
							}
						}
						else
							psx->drawRect(0, 0, static_cast<int>(currItem->width())+1, static_cast<int>(currItem->height())+1);
					}
				}
				else
				{
					double gx, gy, gw, gh;
					m_doc->m_Selection->setGroupRect();
					getGroupRectScreen(&gx, &gy, &gw, &gh);
					QPoint out = contentsToViewport(QPoint(0, 0));
					psx->resetMatrix();
					psx->translate(-out.x(), -out.y());
					psx->translate(qRound(gx), qRound(gy));
					psx->scale(m_viewMode.scale, m_viewMode.scale);
					psx->drawRect(QRect(0, 0, qRound(gw), qRound(gh)));
				}
			}
		}
	}
	else
	{ */
		if ((m_doc->m_Selection->count() != 0) && !(m_viewMode.operItemMoving || m_viewMode.operItemResizing) && (m_doc->appMode != modeDrawBezierLine))
		{
			drawControlsSelection(psx, m_doc->m_Selection->itemAt(0));
		}
/*	} */
//FIXME	if (m_doc->appMode == modeEdit)
//		slotDoCurs(true);
	if (m_doc->appMode == modeEditGradientVectors)
	{
		drawControlsGradientVectors(psx, m_doc->m_Selection->itemAt(0));
	}
//FIXME	evSpon = false;
//	forceRedraw = false;
//	qDebug( "Time elapsed: %d ms", tim.elapsed() );
}



/**
 
*/
void Canvas::drawControlsMovingItemsRect(QPainter* pp)
{
	if (m_doc->m_Selection->count() != 0)
	{
		uint selectedItemCount = m_doc->m_Selection->count();
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		if (selectedItemCount < moveWithBoxesOnlyThreshold)
		{
			for (uint cu = 0; cu < selectedItemCount; cu++)
			{
				currItem = m_doc->m_Selection->itemAt(cu);
//				pp->resetMatrix();
//				QPoint out = contentsToViewport(QPoint(0, 0));
//				pp->translate(out.x(), out.y());
//				pp->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
				Transform(currItem, pp);
				pp->setBrush(Qt::NoBrush);
				pp->setPen(QPen(Qt::black, 1.0 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
				if (selectedItemCount < moveWithFullOutlinesThreshold)
				{
					if (!(currItem->asLine()))
						currItem->DrawPolyL(pp, currItem->Clip);
					else
					{
						if (currItem->asLine())
						{
							int lw2 = 1;
							int lw = 1;
							Qt::PenCapStyle le = Qt::FlatCap;
							if (currItem->NamedLStyle.isEmpty())
							{
								lw2 = qRound(currItem->lineWidth()  / 2.0);
								lw = qRound(qMax(currItem->lineWidth(), 1.0));
								le = currItem->PLineEnd;
							}
							else
							{
								multiLine ml = m_doc->MLineStyles[currItem->NamedLStyle];
								lw2 = qRound(ml[ml.size()-1].Width  / 2.0);
								lw = qRound(qMax(ml[ml.size()-1].Width, 1.0));
								le = static_cast<Qt::PenCapStyle>(ml[ml.size()-1].LineEnd);
							}
							if (le != Qt::FlatCap)
								pp->drawRect(-lw2, -lw2, qRound(currItem->width())+lw, lw);
							else
								pp->drawRect(-1, -lw2, qRound(currItem->width()), lw);
						}
					}
				}
				else
					pp->drawRect(0, 0, static_cast<int>(currItem->width())+1, static_cast<int>(currItem->height())+1);
			}
		}
		else
		{
			double gx, gy, gw, gh;
			m_doc->m_Selection->setGroupRect();
			m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			QPoint out = contentsToViewport(QPoint(0, 0));
//			pp->resetMatrix();
//			pp->translate(out.x(), out.y());
//			pp->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
			pp->scale(m_viewMode.scale, m_viewMode.scale);
			pp->translate(qRound(gx), qRound(gy));
			pp->drawRect(QRect(0, 0, qRound(gw), qRound(gh)));
		}
	}
}


/**
 */
void Canvas::drawControlsHighlightRect(QPainter* pp)
{
	QColor drawColor = qApp->palette().color(QPalette::Active, QPalette::Highlight);
	pp->setPen(QPen(drawColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	drawColor.setAlpha(64);
//	pp->resetMatrix();
//	QPoint out = contentsToViewport(QPoint(0, 0));
//	pp->translate(out.x(), out.y());
//	pp->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
	pp->setBrush(drawColor);
	pp->drawPolygon(m_viewMode.redrawPolygon);
	m_viewMode.redrawPolygon.clear();
}

/**
 */
void Canvas::drawControlsGradientVectors(QPainter* psx, PageItem *currItem)
{
//	psx->resetMatrix();
//	QPoint out = contentsToViewport(QPoint(0, 0));
//	psx->translate(out.x(), out.y());
//	psx->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
	Transform(currItem, psx);
	psx->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	psx->setBrush(Qt::NoBrush);
	psx->drawLine(QPointF(currItem->GrStartX, currItem->GrStartY), QPointF(currItem->GrEndX, currItem->GrEndY));
	psx->setPen(QPen(Qt::magenta, 8, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	psx->drawPoint(QPointF(currItem->GrStartX, currItem->GrStartY));
	psx->drawPoint(QPointF(currItem->GrEndX, currItem->GrEndY));
}


/**
  draws the bezier curve in edit bezier mode
  */
void Canvas::drawControlsBezierCurve(QPainter* pp, PageItem* currItem)
{
	pp->setBrush(Qt::NoBrush);
	pp->setPen(QPen(Qt::black, 1.0 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
//	pp->resetMatrix();
//	QPoint out = contentsToViewport(QPoint(0, 0));
//	pp->translate(out.x(), out.y());
//	pp->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
	Transform(currItem, pp);
	QPainterPath Bez;
	if (currItem->PoLine.size() > 1)
	{
		QPoint nXY = m_viewMode.redrawPolygon.point(0);
		if (!m_viewMode.m_MouseButtonPressed)
		{
			FPoint a1 = currItem->PoLine.point(currItem->PoLine.size()-2);
			FPoint a2 = currItem->PoLine.point(currItem->PoLine.size()-1);
			Bez.moveTo(a1.x(), a1.y());
			Bez.cubicTo(a2.x(), a2.y(), nXY.x(), nXY.y(), nXY.x(), nXY.y());
			pp->drawPath(Bez);
		}
		else
		{
			FPoint a2 = currItem->PoLine.point(currItem->PoLine.size()-1);
			if (currItem->PoLine.size() > 2)
			{
				FPoint a1 = currItem->PoLine.point(currItem->PoLine.size()-2);
				FPoint a3 = currItem->PoLine.point(currItem->PoLine.size()-3);
				Bez.moveTo(a3.x(), a3.y());
				Bez.cubicTo(a1.x(), a1.y(), nXY.x(), nXY.y(), a2.x(), a2.y());
				pp->drawPath(Bez);
			}
			pp->drawLine(QPoint(qRound(a2.x()), qRound(a2.y())), nXY);
		}
	}
	else
	{
		QPoint a2 = currItem->PoLine.pointQ(currItem->PoLine.size()-1);
		QPoint nXY = m_viewMode.redrawPolygon.point(0);
		pp->drawLine(a2, nXY);
	}
	m_viewMode.redrawPolygon.clear();
}


/**
 draws the control points for node edit - currently unused, see drawControlsSelection()
 */
void Canvas::drawControlsNodeEditPoints(QPainter* pp, PageItem* currItem)
{
	if (m_doc->nodeEdit.isContourLine)
		MarkClip(pp, currItem, currItem->ContourLine, true);
	else
		MarkClip(pp, currItem, currItem->PoLine, true);
}


/**
  draws the measurement line in measure mode
 */
void Canvas::drawControlsMeasurementLine(QPainter* pp)
{
//	pp->resetMatrix();
//	QPoint out = contentsToViewport(QPoint(0, 0));
//	pp->translate(out.x(), out.y());
	pp->setBrush(Qt::NoBrush);
	pp->setPen(QPen(Qt::black, 1.0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
	pp->drawPolyline(m_viewMode.redrawPolygon);
	m_viewMode.redrawPolygon.clear();
}


/**
  draws the line in drawline mode
 */
void Canvas::drawControlsDrawLine(QPainter* pp)
{
//	QPoint out = contentsToViewport(QPoint(0, 0));
//	pp->resetMatrix();
//	pp->translate(out.x(), out.y());
//	pp->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
	pp->scale(m_viewMode.scale, m_viewMode.scale);
	pp->setBrush(Qt::NoBrush);
	pp->setPen(QPen(Qt::black, 1.0 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
	pp->drawPolyline(m_viewMode.redrawPolygon);
	m_viewMode.redrawPolygon.clear();
}


/**
  draws the freehand curve in freehand mode
 */
void Canvas::drawControlsFreehandLine(QPainter* pp)
{
//	pp->resetMatrix();
//	QPoint out = contentsToViewport(QPoint(0, 0));
//	pp->translate(out.x(), out.y());
//	pp->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
	pp->scale(m_viewMode.scale, m_viewMode.scale);
	pp->setBrush(Qt::NoBrush);
	pp->setPen(QPen(Qt::black, 1.0 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
	pp->drawPolyline(m_viewMode.redrawPolygon);
	m_viewMode.redrawPolygon.clear();
}


/**
  draws masterpage items in layer order
 */
void Canvas::DrawMasterItems(ScPainter *painter, Page *page, QRect clip)
{
	QRect oldR;
	FPoint orig = localToCanvas(clip.topLeft());
	QRect cullingArea = QRect(static_cast<int>(orig.x()), static_cast<int>(orig.y()), 
							  qRound(clip.width() / m_viewMode.scale + 0.5), qRound(clip.height() / m_viewMode.scale + 0.5));
	QStack<PageItem*> groupStack;
	QStack<PageItem*> groupStack2;
	if (!page->MPageNam.isEmpty())
	{
		Page* Mp = m_doc->MasterPages.at(m_doc->MasterNames[page->MPageNam]);
		if (page->FromMaster.count() != 0)
		{
			int Lnr;
			ScLayer ll;
			PageItem *currItem;
			ll.isViewable = false;
			ll.LNr = 0;
			Lnr = 0;
			uint layerCount=m_doc->layerCount();
			for (uint la = 0; la < layerCount; ++la)
			{
				m_doc->Layers.levelToLayer(ll, Lnr);
				bool pr = true;
				if ((m_viewMode.previewMode) && (!ll.isPrintable))
					pr = false;
				if ((m_viewMode.viewAsPreview) && (!ll.isPrintable))
					pr = false;
				if ((ll.isViewable) && (pr))
				{
					if ((layerCount > 1) || (ll.transparency != 1.0))
						painter->beginLayer(ll.transparency, ll.blendMode);
					uint pageFromMasterCount=page->FromMaster.count();
					for (uint a = 0; a < pageFromMasterCount; ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->pageNr())))
							continue;
						if ((m_viewMode.previewMode) && (!currItem->printEnabled()))
							continue;
						if ((m_viewMode.viewAsPreview) && (!currItem->printEnabled()))
							continue;
						double OldX = currItem->xPos();
						double OldY = currItem->yPos();
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->moveBy(-Mp->xOffset() + page->xOffset(), -Mp->yOffset() + page->yOffset(), true);
							currItem->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
							currItem->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
						}
						oldR = currItem->getRedrawBounding(m_viewMode.scale);
						oldR.translate(qRound(m_doc->minCanvasCoordinate.x() * m_viewMode.scale), qRound(m_doc->minCanvasCoordinate.y() * m_viewMode.scale));
						if (currItem->isGroupControl)
						{
							painter->save();
							currItem->savedOwnPage = currItem->OwnPage;
							currItem->OwnPage = page->pageNr();
							if ((clip.intersects(oldR)) && (m_doc->guidesSettings.layerMarkersShown) && (m_doc->layerCount() > 1))
								currItem->DrawObj(painter, cullingArea);
							FPointArray cl = currItem->PoLine.copy();
							QMatrix mm;
							mm.translate(currItem->xPos(), currItem->yPos());
							mm.rotate(currItem->rotation());
							cl.map( mm );
							painter->beginLayer(1.0 - currItem->fillTransparency(), currItem->fillBlendmode(), &cl);
							groupStack.push(currItem->groupsLastItem);
							groupStack2.push(currItem);
							currItem->OwnPage = currItem->savedOwnPage;
							if (!currItem->ChangedMasterItem)
							{
								//Hack to not check for undo changes, indicate drawing only
								currItem->setXYPos(OldX, OldY, true);
								currItem->BoundingX = OldBX;
								currItem->BoundingY = OldBY;
							}
							continue;
						}
						currItem->savedOwnPage = currItem->OwnPage;
						currItem->OwnPage = page->pageNr();
//FIXME						if (!evSpon || forceRedraw)
		//					currItem->invalid = true;
						if (clip.intersects(oldR))
						{
							if (!((m_viewMode.operItemMoving || m_viewMode.operItemResizeInEditMode) && (currItem->isSelected())))
							{
								if ((m_viewMode.forceRedraw) && (currItem->asTextFrame()))
									currItem->asTextFrame()->invalidateLayout();
								currItem->DrawObj(painter, cullingArea);
							}
							else 
								qDebug() << "skip masterpage item (move/resizeEdit/selected)" << m_viewMode.operItemMoving << m_viewMode.operItemResizeInEditMode << currItem->isSelected();
						}
						currItem->OwnPage = currItem->savedOwnPage;
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->setXYPos(OldX, OldY, true);
							currItem->BoundingX = OldBX;
							currItem->BoundingY = OldBY;
						}
						if (groupStack.count() != 0)
						{
							while (currItem == groupStack.top())
							{
								painter->endLayer();
								painter->restore();
								PageItem *cite = groupStack2.pop();
								double OldX = cite->xPos();
								double OldY = cite->yPos();
								double OldBX = cite->BoundingX;
								double OldBY = cite->BoundingY;
								if (!cite->ChangedMasterItem)
								{
									//Hack to not check for undo changes, indicate drawing only
									cite->moveBy(-Mp->xOffset() + page->xOffset(), -Mp->yOffset() + page->yOffset(), true);
									cite->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
									cite->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
								}
								oldR = cite->getRedrawBounding(m_viewMode.scale);
								if ((clip.intersects(oldR)) && (m_doc->guidesSettings.layerMarkersShown) && (m_doc->layerCount() > 1))
									cite->DrawObj(painter, cullingArea);
								cite->OwnPage = cite->savedOwnPage;
								if (!currItem->ChangedMasterItem)
								{
									//Hack to not check for undo changes, indicate drawing only
									cite->setXYPos(OldX, OldY, true);
									cite->BoundingX = OldBX;
									cite->BoundingY = OldBY;
								}
								groupStack.pop();
								if (groupStack.count() == 0)
									break;
							}
						}
					}
					for (uint a = 0; a < pageFromMasterCount; ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if (!currItem->isTableItem)
							continue;
						if ((m_viewMode.previewMode) && (!currItem->printEnabled()))
							continue;
						if ((m_viewMode.viewAsPreview) && (!currItem->printEnabled()))
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->pageNr())))
							continue;
						if (currItem->isGroupControl)
							continue;
						double OldX = currItem->xPos();
						double OldY = currItem->yPos();
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->setXYPos(OldX - Mp->xOffset() + page->xOffset(), OldY - Mp->yOffset() + page->yOffset(), true);
							currItem->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
							currItem->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
						}
						oldR = currItem->getRedrawBounding(m_viewMode.scale);
						oldR.translate(qRound(m_doc->minCanvasCoordinate.x() * m_viewMode.scale), qRound(m_doc->minCanvasCoordinate.y() * m_viewMode.scale));
						if (clip.intersects(oldR))
						{
							painter->save();
							painter->translate(currItem->xPos(), currItem->yPos());
							painter->rotate(currItem->rotation());
							if ((currItem->lineColor() != CommonStrings::None) && (currItem->lineWidth() != 0.0))
							{
								QColor tmp;
								currItem->SetFarbe(&tmp, currItem->lineColor(), currItem->lineShade());
								if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
								{
									painter->setPen(tmp, currItem->lineWidth(), currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
									if (currItem->TopLine)
										painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->width(), 0.0));
									if (currItem->RightLine)
										painter->drawLine(FPoint(currItem->width(), 0.0), FPoint(currItem->width(), currItem->height()));
									if (currItem->BottomLine)
										painter->drawLine(FPoint(currItem->width(), currItem->height()), FPoint(0.0, currItem->height()));
									if (currItem->LeftLine)
										painter->drawLine(FPoint(0.0, currItem->height()), FPoint(0.0, 0.0));
								}
							}
							painter->restore();
						}
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->setXYPos(OldX, OldY, true);
							currItem->BoundingX = OldBX;
							currItem->BoundingY = OldBY;
						}
					}
					if ((layerCount > 1) || (ll.transparency != 1.0))
						painter->endLayer();
				}
				Lnr++;
			}
		}
	}
}


/**
  draws page items in layer order
 */
void Canvas::DrawPageItems(ScPainter *painter, QRect clip)
{
	m_viewMode.linkedFramesToShow.clear();
	QRect oldR;
	FPoint orig = localToCanvas(clip.topLeft());
	QRect cullingArea = QRect(static_cast<int>(orig.x()), static_cast<int>(orig.y()), 
							  qRound(clip.width() / m_viewMode.scale + 0.5), qRound(clip.height() / m_viewMode.scale + 0.5));
	QStack<PageItem*> groupStack;
	QStack<PageItem*> groupStack2;
	if (m_doc->Items->count() != 0)
	{
		int Lnr=0;
		ScLayer ll;
		PageItem *currItem;
		ll.isViewable = false;
		ll.LNr = 0;
		uint layerCount=m_doc->layerCount();
		int docCurrPageNo=static_cast<int>(m_doc->currentPageNumber());
		for (uint la2 = 0; la2 < layerCount; ++la2)
		{
			m_doc->Layers.levelToLayer(ll, Lnr);
			bool pr = true;
			if ((m_viewMode.previewMode) && (!ll.isPrintable))
				pr = false;
			if ((m_viewMode.viewAsPreview) && (!ll.isPrintable))
				pr = false;
			if ((ll.isViewable) && (pr))
			{
				if ((layerCount > 1) || (ll.transparency != 1.0))
					painter->beginLayer(ll.transparency, ll.blendMode);
//				QListIterator<PageItem*> docItem(*m_doc->Items);
//				while ( (currItem = docItem.current()) != 0)
//				{
//					++docItem;
				for (int it = 0; it < m_doc->Items->count(); ++it)
				{
					currItem = m_doc->Items->at(it);
					if (currItem->LayerNr != ll.LNr)
						continue;
					if ((m_viewMode.previewMode) && (!currItem->printEnabled()))
						continue;
					if ((m_viewMode.viewAsPreview) && (!currItem->printEnabled()))
						continue;
					if ((m_doc->masterPageMode()) && ((currItem->OwnPage != -1) && (currItem->OwnPage != docCurrPageNo)))
						continue;
					if (!m_doc->masterPageMode() && !currItem->OnMasterPage.isEmpty())
					{
						if (currItem->OnMasterPage != m_doc->currentPage()->pageName())
							continue;
					}
					oldR = currItem->getRedrawBounding(m_viewMode.scale);
					oldR.translate(qRound(m_doc->minCanvasCoordinate.x() * m_viewMode.scale), qRound(m_doc->minCanvasCoordinate.y() * m_viewMode.scale));
					if (currItem->isGroupControl)
					{
						painter->save();
						FPointArray cl = currItem->PoLine.copy();
						QMatrix mm;
						mm.translate(currItem->xPos(), currItem->yPos());
						mm.rotate(currItem->rotation());
						cl.map( mm );
						painter->beginLayer(1.0 - currItem->fillTransparency(), currItem->fillBlendmode(), &cl);
						groupStack.push(currItem->groupsLastItem);
						groupStack2.push(currItem);
						continue;
					}
					if (clip.intersects(oldR))
					{
//FIXME						if (!evSpon || forceRedraw) 
		//					currItem->invalid = true;
//						if ((!m_MouseButtonPressed) || (m_doc->appMode == modeEditClip))
						if (((m_viewMode.operItemMoving || m_viewMode.operItemResizeInEditMode || m_viewMode.drawSelectedItemsWithControls) && currItem->isSelected() && !m_viewMode.firstSpecial))
						{
							qDebug() << "skipping pageitem (move/resizeEdit/selected)" << m_viewMode.operItemMoving << m_viewMode.operItemResizeInEditMode << currItem->isSelected();
						}
						else
						{
							if ((m_viewMode.forceRedraw) && (currItem->asTextFrame()))
								currItem->asTextFrame()->invalidateLayout();
							currItem->DrawObj(painter, cullingArea);
						}
//						currItem->Redrawn = true;
						if ((currItem->asTextFrame()) && ((currItem->nextInChain() != 0) || (currItem->prevInChain() != 0)))
						{
							PageItem *nextItem = currItem;
							while (nextItem != 0)
							{
								if (nextItem->prevInChain() != 0)
									nextItem = nextItem->prevInChain();
								else
									break;
							}
							if (!m_viewMode.linkedFramesToShow.contains(nextItem))
								m_viewMode.linkedFramesToShow.append(nextItem);
						}
						/* FIXME:av
						if ((m_doc->appMode == modeEdit) && (currItem->isSelected()) && (currItem->itemType() == PageItem::TextFrame))
						{
							//CB 230305 Stop redrawing the horizontal ruler if it hasnt changed when typing text!!!
							if ((qRound(horizRuler->ItemPos*10000) != qRound((currItem->xPos())*10000)) || (qRound(horizRuler->ItemEndPos*10000) != qRound(((currItem->xPos()+currItem->width()) )*10000)))
							{
								horizRuler->setItem(currItem);
								if (currItem->lineColor() != CommonStrings::None)
									horizRuler->lineCorr = currItem->lineWidth() / 2.0;
								else
									horizRuler->lineCorr = 0;
								horizRuler->ColGap = currItem->ColGap;
								horizRuler->Cols = currItem->Cols;
								horizRuler->Extra = currItem->textToFrameDistLeft();
								horizRuler->RExtra = currItem->textToFrameDistRight();
								horizRuler->First = currItem->currentStyle().firstIndent();
								horizRuler->Indent = currItem->currentStyle().leftMargin();
								double columnWidth = (currItem->width() - (currItem->columnGap() * (currItem->columns() - 1)) 
									- currItem->textToFrameDistLeft() - currItem->textToFrameDistLeft() 
									- 2*horizRuler->lineCorr) / currItem->columns();
								horizRuler->RMargin = columnWidth - currItem->currentStyle().rightMargin();
								if (currItem->imageFlippedH() || (currItem->reversed()))
									horizRuler->Revers = true;
								else
									horizRuler->Revers = false;
								horizRuler->ItemPosValid = true;
								horizRuler->TabValues = currItem->currentStyle().tabValues();
								horizRuler->update();
							}
						}
						*/
					}
					if (groupStack.count() != 0)
					{
						while (currItem == groupStack.top())
						{
							painter->endLayer();
							painter->restore();
							PageItem *cite = groupStack2.pop();
							oldR = cite->getRedrawBounding(m_viewMode.scale);
							if ((clip.intersects(oldR)) && (((m_doc->guidesSettings.layerMarkersShown) && (m_doc->layerCount() > 1)) || (cite->textFlowUsesContourLine())))
								cite->DrawObj(painter, cullingArea);
							groupStack.pop();
							if (groupStack.count() == 0)
								break;
						}
					}
				}
//				QListIterator<PageItem*> docItem2(*m_doc->Items);
//				while ( (currItem = docItem2.current()) != 0 )
//				{
//					++docItem2;
				for (int it = 0; it < m_doc->Items->count(); ++it)
				{
					currItem = m_doc->Items->at(it);
					if (currItem->LayerNr != ll.LNr)
						continue;
					if (!currItem->isTableItem)
						continue;
					if ((m_viewMode.previewMode) && (!currItem->printEnabled()))
						continue;
					if ((m_viewMode.viewAsPreview) && (!currItem->printEnabled()))
						continue;
					oldR = currItem->getRedrawBounding(m_viewMode.scale);
					oldR.translate(qRound(m_doc->minCanvasCoordinate.x() * m_viewMode.scale), qRound(m_doc->minCanvasCoordinate.y() * m_viewMode.scale));
					if (clip.intersects(oldR))
					{
						painter->save();
						painter->translate(currItem->xPos(), currItem->yPos());
						painter->rotate(currItem->rotation());
						if ((currItem->lineColor() != CommonStrings::None) && (currItem->lineWidth() != 0.0))
						{
							QColor tmp;
							currItem->SetFarbe(&tmp, currItem->lineColor(), currItem->lineShade());
							if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
							{
								painter->setPen(tmp, currItem->lineWidth(), currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
								if (currItem->TopLine)
									painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->width(), 0.0));
								if (currItem->RightLine)
									painter->drawLine(FPoint(currItem->width(), 0.0), FPoint(currItem->width(), currItem->height()));
								if (currItem->BottomLine)
									painter->drawLine(FPoint(currItem->width(), currItem->height()), FPoint(0.0, currItem->height()));
								if (currItem->LeftLine)
									painter->drawLine(FPoint(0.0, currItem->height()), FPoint(0.0, 0.0));
							}
						}
						painter->restore();
					}
				}
				if ((layerCount > 1) || (ll.transparency != 1.0))
					painter->endLayer();
			}
			Lnr++;
		}
	}
}


/**
  Draws the canvas background for masterpages, incl. bleeds
 */
void Canvas::drawBackgroundMasterpage(ScPainter* painter, int clipx, int clipy, int clipw, int cliph)
{
	double x = m_doc->scratch.Left * m_viewMode.scale;
	double y = m_doc->scratch.Top * m_viewMode.scale;
	double w = m_doc->currentPage()->width() * m_viewMode.scale;
	double h = m_doc->currentPage()->height() * m_viewMode.scale;
	QRectF drawRect = QRectF(x, y, w+5, h+5);
//	drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
	if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
	{
		painter->setFillMode(ScPainter::Solid);
		painter->setBrush(QColor(128,128,128));
		double bleedRight = 0.0;
		double bleedLeft = 0.0;
		double bleedBottom = 0.0;
		double bleedTop = 0.0;
		m_doc->getBleeds(m_doc->currentPage(), &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
		painter->beginLayer(1.0, 0);
		painter->setAntialiasing(false);
		painter->setPen(Qt::black, 1 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		if (((m_doc->bleeds.Bottom != 0.0) || (m_doc->bleeds.Top != 0.0) || (m_doc->bleeds.Left != 0.0) || (m_doc->bleeds.Right != 0.0)) && (m_doc->guidesSettings.showBleed))
		{
			painter->drawRect(m_doc->scratch.Left - bleedLeft+5 / m_viewMode.scale, m_doc->scratch.Top - bleedTop+5 / m_viewMode.scale, m_doc->currentPage()->width() + bleedLeft + bleedRight, m_doc->currentPage()->height() + bleedBottom + bleedTop);
			painter->setBrush(m_doc->papColor);
			painter->drawRect(m_doc->scratch.Left - bleedLeft, m_doc->scratch.Top - bleedTop, m_doc->currentPage()->width() + bleedLeft + bleedRight, m_doc->currentPage()->height() + bleedBottom + bleedTop);
		}
		else
		{
			painter->drawRect(m_doc->scratch.Left+5 / m_viewMode.scale, m_doc->scratch.Top+5 / m_viewMode.scale, m_doc->currentPage()->width(), m_doc->currentPage()->height());
			painter->setBrush(m_doc->papColor);
			painter->drawRect(m_doc->scratch.Left, m_doc->scratch.Top, m_doc->currentPage()->width(), m_doc->currentPage()->height());
		}
		painter->setAntialiasing(true);
		if (m_doc->guidesSettings.before)
			DrawPageMarks(painter, m_doc->currentPage(), QRect(clipx, clipy, clipw, cliph));
		painter->endLayer();
	}
}



/**
  draws the page outlines on canvas, including bleed area
 */
void Canvas::drawBackgroundPageOutlines(ScPainter* painter, int clipx, int clipy, int clipw, int cliph)
{
	uint docPagesCount=m_doc->Pages->count();
	if (!m_viewMode.viewAsPreview)
	{
		painter->setBrush(QColor(128,128,128));
		painter->setAntialiasing(false);
		painter->beginLayer(1.0, 0);
		painter->setPen(Qt::black, 1.0 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		Page *actPg;
		for (int a = 0; a < static_cast<int>(docPagesCount); ++a)
		{
			actPg = m_doc->Pages->at(a);
			double bleedRight = 0.0;
			double bleedLeft = 0.0;
			double bleedBottom = 0.0;
			double bleedTop = 0.0;
			m_doc->getBleeds(actPg, &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
			double blx = (actPg->xOffset() - bleedLeft) * m_viewMode.scale;
			double bly = (actPg->yOffset() - bleedTop) * m_viewMode.scale;
			double blw = (actPg->width() + bleedLeft + bleedRight) * m_viewMode.scale;
			double blh = (actPg->height() + bleedBottom + bleedTop) * m_viewMode.scale;
			
			QRectF drawRect = QRectF(blx-1, bly-1, blw+6, blh+6);
//			drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
			if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
			{
				painter->setFillMode(ScPainter::Solid);
				double blx2 = actPg->xOffset();
				double bly2 = actPg->yOffset();
				double blw2 = actPg->width();
				double blh2 = actPg->height();
				if (m_doc->guidesSettings.showBleed)
				{
					blx2 -= bleedLeft;
					bly2 -= bleedTop;
					blw2 += bleedLeft + bleedRight;
					blh2 += bleedBottom + bleedTop;
				}
				painter->drawRect(blx2 + 5 / m_viewMode.scale, bly2 + 5 / m_viewMode.scale, blw2, blh2);
				if (((m_doc->bleeds.Bottom != 0.0) || (m_doc->bleeds.Top != 0.0) || (m_doc->bleeds.Left != 0.0) || (m_doc->bleeds.Right != 0.0)) && (m_doc->guidesSettings.showBleed))
				{
					painter->setFillMode(ScPainter::None);
					painter->setPen(Qt::black, 1.0 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					painter->drawRect(blx2, bly2 - 1 / m_viewMode.scale, blw2 + 1 / m_viewMode.scale, blh2 + 2 / m_viewMode.scale);
				}
			}
		}
		painter->endLayer();
		painter->setAntialiasing(true);
	}
	painter->setFillMode(ScPainter::Solid);
	Page *actPg;
	for (int a = 0; a < static_cast<int>(docPagesCount); ++a)
	{
		actPg = m_doc->Pages->at(a);
		double x = actPg->xOffset();
		double y = actPg->yOffset();
		double w = actPg->width();
		double h = actPg->height();
		double bleedRight = 0.0;
		double bleedLeft = 0.0;
		double bleedBottom = 0.0;
		double bleedTop = 0.0;
		bool drawBleed = false;
		if (((m_doc->bleeds.Bottom != 0.0) || (m_doc->bleeds.Top != 0.0) || (m_doc->bleeds.Left != 0.0) || (m_doc->bleeds.Right != 0.0)) && (m_doc->guidesSettings.showBleed))
		{
			drawBleed = true;
			m_doc->getBleeds(a, &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
		}
		double blx = (actPg->xOffset() - bleedLeft) * m_viewMode.scale;
		double bly = (actPg->yOffset() - bleedTop) * m_viewMode.scale;
		double blw = (actPg->width() + bleedLeft + bleedRight) * m_viewMode.scale;
		double blh = (actPg->height() + bleedBottom + bleedTop) * m_viewMode.scale;
		
		QRectF drawRect = QRectF(blx, bly, blw+5, blh+5);
//		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
		{
			painter->setFillMode(ScPainter::Solid);
			painter->setPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			painter->setAntialiasing(false);
			painter->beginLayer(1.0, 0);
			painter->setLineWidth(0.0);
			painter->setBrush(m_doc->papColor);
			if (!m_viewMode.viewAsPreview)
			{
				double blx2 = actPg->xOffset() - bleedLeft;
				double bly2 = actPg->yOffset() - bleedTop;
				double blw2 = actPg->width() + bleedLeft + bleedRight;
				double blh2 = actPg->height() + bleedBottom + bleedTop;
				painter->drawRect(blx2, bly2, blw2, blh2);
				if (drawBleed)
					painter->drawRect(x, y, w, h);
			}
			else
				painter->drawRect(x, y, w, h);
			painter->endLayer();
			painter->setAntialiasing(true);
		}
	}
}

/**
 fills PoLine with a clippath for the doc pages
 */
void Canvas::getClipPathForPages(FPointArray* PoLine)
{
	PoLine->resize(0);
	uint docPagesCount=m_doc->Pages->count();
	bool first = true;
	Page *actPg;
	for (int a = 0; a < static_cast<int>(docPagesCount); ++a)
	{
		if (!first)
			PoLine->setMarker();
		first = false;
		actPg = m_doc->Pages->at(a);
		double x = actPg->xOffset();
		double y = actPg->yOffset();
		double w = actPg->width();
		double h = actPg->height();
		static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
			0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
		for (int a = 0; a < 29; a += 4)
		{
			PoLine->addPoint(x + w * rect[a], y + h * rect[a+1]);
			PoLine->addPoint(x + w * rect[a+2], y + h * rect[a+3]);
		}
	}
}


void Canvas::drawGuides(ScPainter* painter, int clipx, int clipy, int clipw, int cliph)
{
	uint docPagesCount=m_doc->Pages->count();
	Page *actPg;
	for (uint a = 0; a < docPagesCount; ++a)
	{
		actPg = m_doc->Pages->at(a);
		double x = actPg->xOffset() * m_viewMode.scale;
		double y = actPg->yOffset() * m_viewMode.scale;
		double w = actPg->width() * m_viewMode.scale;
		double h = actPg->height() * m_viewMode.scale;
		QRectF drawRect = QRectF(x, y, w+5, h+5);
//		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
			DrawPageMarks(painter, m_doc->Pages->at(a), QRect(clipx, clipy, clipw, cliph));
	}				
}


/**
  draws guides, margins, grid and baseline grid
 */
void Canvas::DrawPageMarks(ScPainter *p, Page *page, QRect clip)
{
	p->save();
	p->setAntialiasing(false);
	p->translate(page->xOffset(), page->yOffset());
	double lineWidth = 1.0 / m_viewMode.scale;
	double pageHeight=page->height();
	double pageWidth=page->width();
	p->setFillMode(ScPainter::None);
	p->setPen(Qt::black, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->drawRect(0, 0, pageWidth, pageHeight);
	//Draw the margins
	if (m_doc->guidesSettings.marginsShown)
	{
		p->setPen(m_doc->guidesSettings.margColor);
		if (m_doc->marginColored)
		{
			p->setBrush(m_doc->guidesSettings.margColor);
			p->drawRect(0, 0, pageWidth, page->Margins.Top);
			p->drawRect(0, page->Margins.Top, page->Margins.Left, pageHeight - page->Margins.Top);
			p->drawRect(page->Margins.Left, pageHeight - page->Margins.Bottom, pageWidth - page->Margins.Right - page->Margins.Left, page->Margins.Bottom);
			p->drawRect(pageWidth - page->Margins.Right, page->Margins.Top, page->Margins.Right, pageHeight-page->Margins.Top);
		}
//		p->setPen(m_doc->guidesSettings.margColor);
		p->setFillMode(ScPainter::None);
		p->drawRect(page->Margins.Left, page->Margins.Top, pageWidth - page->Margins.Left - page->Margins.Right, pageHeight - page->Margins.Top - page->Margins.Bottom);
//		p->drawLine(FPoint(0, page->Margins.Top), FPoint(pageWidth, page->Margins.Top));
//		p->drawLine(FPoint(0, pageHeight - page->Margins.Bottom), FPoint(pageWidth, pageHeight - page->Margins.Bottom));
//		p->drawLine(FPoint(page->Margins.Left, 0), FPoint(page->Margins.Left, pageHeight));
//		p->drawLine(FPoint(pageWidth - page->Margins.Right, 0), FPoint(pageWidth - page->Margins.Right, pageHeight));
	}
	//Draw the baseline grid
	if (m_doc->guidesSettings.baseShown)
	{
		p->setPen(m_doc->guidesSettings.baseColor, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		for (double yg = m_doc->typographicSettings.offsetBaseGrid; yg < pageHeight; yg += m_doc->typographicSettings.valueBaseGrid)
			p->drawLine(FPoint(0, yg), FPoint(pageWidth, yg));
	}
	//Draw the grid lines
	if (m_doc->guidesSettings.gridShown)
	{
		double lowerBx = qMax(clip.x() / m_viewMode.scale + m_doc->minCanvasCoordinate.x() - page->xOffset(), 0.0);
		double lowerBy = qMax(clip.y() / m_viewMode.scale + m_doc->minCanvasCoordinate.y() - page->yOffset(), 0.0);
		double highBx = qMin(lowerBx + clip.width() / m_viewMode.scale, pageWidth);
		double highBy = qMin(lowerBy + clip.height() / m_viewMode.scale, pageHeight);
		if (m_viewMode.scale > 0.49)
		{
			double i,start;
			i = m_doc->guidesSettings.majorGrid;
			p->setPen(m_doc->guidesSettings.majorColor, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			start=floor(lowerBy/i);
			start*=i;
			for (double b = start; b <= highBy; b+=i)
			{
				p->drawLine(FPoint(qMax(lowerBx, 0.0), b), FPoint(qMin(pageWidth, highBx), b));
			}
			start=floor(lowerBx/i);
			start*=i;
			for (double b = start; b <= highBx; b+=i)
			{
				p->drawLine(FPoint(b, qMax(lowerBy, 0.0)), FPoint(b, qMin(pageHeight, highBy)));
			}
			i = m_doc->guidesSettings.minorGrid;
			p->setPen(m_doc->guidesSettings.minorColor, lineWidth, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
			start=floor(lowerBy/i);
			start*=i;
			for (double b = start; b <= highBy; b+=i)
			{
				p->drawLine(FPoint(qMax(lowerBx, 0.0), b), FPoint(qMin(pageWidth, highBx), b));
			}
			start=floor(lowerBx/i);
			start*=i;
			for (double b = start; b <= highBx; b+=i)
			{
				p->drawLine(FPoint(b, qMax(lowerBy, 0.0)), FPoint(b, qMin(pageHeight, highBy)));
			}
		}
	}
	//Draw the guides
	if (m_doc->guidesSettings.guidesShown)
		page->guides.drawPage(p, m_doc, lineWidth);
	if (m_doc->currentPage() == page)
	{
		p->setPen(PrefsManager::instance()->appPrefs.DPageBorderColor, 2 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawRect(0, 0, pageWidth, pageHeight);
	}
	p->setAntialiasing(true);

	p->restore();
}



/**
  - currently unused, see drawControlsSelection()
 */
void Canvas::drawControlsSelectionSpecial(QPainter* pp, PageItem *currItem)
{
	if (m_doc->m_Selection->isMultipleSelection())
	{
//		pp->resetMatrix();
//		QPoint out = contentsToViewport(QPoint(0, 0));
//		pp->translate(out.x(), out.y());
//		pp->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
		double x, y, w, h;
		m_doc->m_Selection->setGroupRect();
		m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
		x *= m_viewMode.scale;
		y *= m_viewMode.scale;
		w *= m_viewMode.scale;
		h *= m_viewMode.scale;
		pp->setPen(QPen(Qt::red, 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
		pp->setBrush(Qt::NoBrush);
		pp->drawRect(QRectF(x, y, w, h));
		pp->setBrush(Qt::red);
		pp->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		pp->drawRect(QRectF(x+w-6, y+h-6, 6, 6));
		pp->drawRect(QRectF(x+w/2 - 3, y+h-6, 6, 6));
		pp->drawRect(QRectF(x+w/2 - 3, y, 6, 6));
		pp->drawRect(QRectF(x+w-6, y+h/2 - 3, 6, 6));
		pp->drawRect(QRectF(x+w-6, y, 6, 6));
		pp->drawRect(QRectF(x, y, 6, 6));
		pp->drawRect(QRectF(x, y+h/2 - 3, 6, 6));
		pp->drawRect(QRectF(x, y+h-6, 6, 6));
	}
	else
	{
//		pp->resetMatrix();
//		QPoint out = contentsToViewport(QPoint(0, 0));
//		pp->translate(out.x(), out.y());
//		pp->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
		Transform(currItem, pp);
		currItem->paintObj(pp);
	}
}


/**
  draws the marks for the currently active selection
 */
void Canvas::drawControlsSelection(QPainter* psx, PageItem *currItem)
{
	if ((m_doc->appMode == modeEditClip) && (currItem->isSelected())) // && (!m_viewMode.specialRendering))
	{
		if (m_doc->nodeEdit.isContourLine)
			MarkClip(psx, currItem, currItem->ContourLine, true);
		else
			MarkClip(psx, currItem, currItem->PoLine, true);
	}
	else
	{
		if (m_doc->m_Selection->isMultipleSelection())
		{
//			psx->resetMatrix();
//			QPoint out = contentsToViewport(QPoint(0, 0));
//			psx->translate(out.x(), out.y());
//			psx->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
			if (m_doc->m_Selection->count() != 0)
			{
				uint docSelectionCount = m_doc->m_Selection->count();
				PageItem *currItem;
				for (uint a=0; a<docSelectionCount; ++a)
				{
					currItem = m_doc->m_Selection->itemAt(a);
					psx->save();
					Transform(currItem, psx);
					currItem->paintObj(psx);
					psx->restore();
				}
			}
//			psx->resetMatrix();
//			psx->translate(out.x(), out.y());
//			psx->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
			double x, y, w, h;
			m_doc->m_Selection->setGroupRect();
			m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			x *= m_viewMode.scale;
			y *= m_viewMode.scale;
			w *= m_viewMode.scale;
			h *= m_viewMode.scale;
			psx->setPen(QPen(Qt::red, 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
			psx->setBrush(Qt::NoBrush);
			psx->drawRect(QRectF(x, y, w, h));
			psx->setBrush(Qt::red);
			psx->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			psx->drawRect(QRectF(x+w-6, y+h-6, 6, 6));
			psx->drawRect(QRectF(x+w/2 - 3, y+h-6, 6, 6));
			psx->drawRect(QRectF(x+w/2 - 3, y, 6, 6));
			psx->drawRect(QRectF(x+w-6, y+h/2 - 3, 6, 6));
			psx->drawRect(QRectF(x+w-6, y, 6, 6));
			psx->drawRect(QRectF(x, y, 6, 6));
			psx->drawRect(QRectF(x, y+h/2 - 3, 6, 6));
			psx->drawRect(QRectF(x, y+h-6, 6, 6));
		}
		else
		{
//			psx->resetMatrix();
//			QPoint out = contentsToViewport(QPoint(0, 0));
//			psx->translate(out.x(), out.y());
//			psx->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
			Transform(currItem, psx);
			currItem->paintObj(psx);
		}
	}
}

	
/**
  draws the control points for node editing
 */
void Canvas::MarkClip(QPainter *p, PageItem *currItem, FPointArray cli, bool)
{
	double x, y;
	p->save();
//	p->resetMatrix();
//	QPoint out = contentsToViewport(QPoint(0, 0));
//	p->translate(out.x(), out.y());
//	p->translate(-qRound(m_doc->minCanvasCoordinate.x()*m_viewMode.scale), -qRound(m_doc->minCanvasCoordinate.y()*m_viewMode.scale));
	Transform(currItem, p);
	p->setPen(QPen(Qt::blue, 1 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p->setBrush(Qt::NoBrush);
	if ((m_doc->nodeEdit.isContourLine) && (currItem->ContourLine.size() != 0))
		cli = currItem->ContourLine;
	else
		cli = currItem->PoLine;
	if (cli.size() > 3)
	{
		for (uint poi=0; poi<cli.size()-3; poi += 4)
		{
			if (cli.point(poi).x() > 900000)
				continue;
			p->setPen(QPen(Qt::blue, 1 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			FPoint a1 = cli.point(poi);
			FPoint a2 = cli.point(poi+1);
			FPoint a3 = cli.point(poi+3);
			FPoint a4 =	cli.point(poi+2);
			QPainterPath Bez;
			Bez.moveTo(a1.x(), a1.y());
			Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
			p->drawPath(Bez);
			p->setPen(QPen(Qt::blue, 1 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
			p->drawLine(QPointF(a1.x(), a1.y()), QPointF(a2.x(), a2.y()));
			p->drawLine(QPointF(a3.x(), a3.y()), QPointF(a4.x(), a4.y()));
		}
	}
	for (uint a=0; a<cli.size()-1; a += 2)
	{
		if (cli.point(a).x() > 900000)
			continue;
		if (m_doc->nodeEdit.EdPoints)
		{
			if (m_doc->nodeEdit.ClRe == static_cast<int>(a+1))
				p->setPen(QPen(Qt::red, 8 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::magenta, 8 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a+1, &x, &y);
			p->drawPoint(QPointF(x, y));
			if (m_doc->nodeEdit.ClRe == static_cast<int>(a))
				p->setPen(QPen(Qt::red, 8 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::blue, 8 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a, &x, &y);
			p->drawPoint(QPointF(x, y));
		}
		else
		{
			if (m_doc->nodeEdit.ClRe == static_cast<int>(a))
				p->setPen(QPen(Qt::red, 8 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::blue, 8 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a, &x, &y);
			p->drawPoint(QPointF(x, y));
			if (m_doc->nodeEdit.ClRe == static_cast<int>(a+1))
				p->setPen(QPen(Qt::red, 8 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::magenta, 8 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a+1, &x, &y);
			p->drawPoint(QPointF(x, y));
		}
	}
	if (m_doc->nodeEdit.ClRe != -1)
	{
		p->setPen(QPen(Qt::red, 8 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
		cli.point(m_doc->nodeEdit.ClRe, &x, &y);
		p->drawPoint(QPointF(x, y));
		QList<int>::Iterator itm;
		for (itm = m_doc->nodeEdit.SelNode.begin(); itm != m_doc->nodeEdit.SelNode.end(); ++itm)
		{
			cli.point((*itm), &x, &y);
			p->drawPoint(QPointF(x, y));
		}
		// FIXME:av emit HavePoint(true, MoveSym);
	}
	else {
		// FIXME:av emit HavePoint(false, MoveSym);
	}
	p->restore();
}


/**
  draws the links between texframe chains.
  needs the list of visible textframes in m_viewMode.linkedFramesToShow
 */
void Canvas::drawFrameLinks(ScPainter* painter)
{
	painter->save();
	PageItem *currItem;
	if ((m_doc->guidesSettings.linkShown || m_viewMode.drawFramelinksWithContents) && (m_viewMode.linkedFramesToShow.count() != 0))
		currItem = m_viewMode.linkedFramesToShow.at(0);
	else
	{
		if (m_viewMode.linkedFramesToShow.count() != 0)
			currItem = m_viewMode.linkedFramesToShow.at(0);
		else
			currItem = m_doc->m_Selection->itemAt(0);
	}
	//Draw the frame links
	if ((((m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames))
		 && (currItem->itemType() == PageItem::TextFrame)) || (m_doc->guidesSettings.linkShown || m_viewMode.drawFramelinksWithContents))
	{
		PageItem *nextItem = currItem;
		if (m_doc->guidesSettings.linkShown || m_viewMode.drawFramelinksWithContents)
		{
			for (int lks = 0; lks < m_viewMode.linkedFramesToShow.count(); ++lks)
			{
				nextItem = m_viewMode.linkedFramesToShow.at(lks);
				while (nextItem != 0)
				{
					if (nextItem->nextInChain() != NULL)
					{
						FPoint start, end;
						calculateFrameLinkPoints(nextItem, nextItem->nextInChain(), start, end);
						drawLinkFrameLine(painter, start, end);
					}
					nextItem = nextItem->nextInChain();
				}
			}
		}
		else
		{
			while (nextItem != 0)
			{
				if (nextItem->prevInChain() != 0)
					nextItem = nextItem->prevInChain();
				else
					break;
			}
			while (nextItem != 0)
			{
				if (nextItem->nextInChain() != NULL)
				{
					FPoint start, end;
					calculateFrameLinkPoints(nextItem, nextItem->nextInChain(), start, end);
					drawLinkFrameLine(painter, start, end);
				}
				nextItem = nextItem->nextInChain();
			}
		}
	}
	painter->setLineWidth(1);
	painter->setPenOpacity(1.0);
	painter->restore();
}


/**
  draws one linkline between textframes
  */
void Canvas::drawLinkFrameLine(ScPainter* painter, FPoint &start, FPoint &end)
{
	//CB FIXME Add some checking that the painter is setup?
	Q_ASSERT(painter!=NULL);
	painter->setPen(Qt::black, 1.0 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	painter->setPenOpacity(1.0);
	painter->drawLine(start, end);
	QMatrix arrowTrans;
	arrowTrans.translate(end.x(), end.y());
	double r = atan2(end.y()-start.y(), end.x()-start.x())*(180.0/M_PI);
	arrowTrans.rotate(r);
	double sc = 0.8 / m_viewMode.scale;
	arrowTrans.scale(sc, sc);
	FPointArray arrow;
	arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
	arrow.addQuadPoint(-15, -5, -15, -5, -15, -5, -15, -5);
	arrow.addQuadPoint(0, 0, 0, 0, 0, 0, 0, 0);
	arrow.addQuadPoint(-15, 5, -15, 5, -15, 5, -15, 5);
	arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
	arrow.map(arrowTrans);
	painter->setBrush(painter->pen());
	painter->setBrushOpacity(1.0);
	painter->setLineWidth(0);
	painter->setFillMode(ScPainter::Solid);
	painter->setupPolygon(&arrow);
	painter->fillPath();
}


/**
  draws the selection rectangle for a group
*/
void Canvas::paintGroupRect(bool norm)
{
	double x, y, w, h;
	getGroupRectScreen(&x, &y, &w, &h);
	QPainter pgc;
	pgc.begin(this);
	pgc.setPen(QPen((norm ? Qt::red : Qt::black), 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
	pgc.setBrush(Qt::NoBrush);
	pgc.drawRect(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h));
	pgc.setBrush(Qt::red);
	if (norm)
	{
		pgc.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		pgc.drawRect(qRound(x+w-6), qRound(y+h-6), 6, 6);
		pgc.drawRect(qRound(x+w/2 - 3), qRound(y+h-6), 6, 6);
		pgc.drawRect(qRound(x+w/2 - 3), qRound(y), 6, 6);
		pgc.drawRect(qRound(x+w-6), qRound(y+h/2 - 3), 6, 6);
		pgc.drawRect(qRound(x+w-6), qRound(y), 6, 6);
		pgc.drawRect(qRound(x), qRound(y), 6, 6);
		pgc.drawRect(qRound(x), qRound(y+h/2 - 3), 6, 6);
		pgc.drawRect(qRound(x), qRound(y+h-6), 6, 6);
	}
	pgc.end();
}


/**
  this is a wrapper around repaint()
 */
void Canvas::PaintSizeRect(QRect newRect)
{
	static QRect oldRect;
	if (!newRect.isNull())
	{
		if (!oldRect.isNull())
			newRect.unite(oldRect);
		repaint(newRect.adjusted(-10, -10, 20, 20));
	}
	oldRect = newRect;
}


/// ???
void Canvas::PaintSizeRect(QPolygon newRect)
{
	static QRect oldRect;
	QRect newR = newRect.boundingRect();
	if (!newR.isNull())
	{
		if (!oldRect.isNull())
			newR.unite(oldRect);
//		newR.moveBy(qRound(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale), qRound(-m_doc->minCanvasCoordinate.y() * m_viewMode.scale));
		m_viewMode.redrawPolygon = newRect;
//		m_viewMode.redrawPolygon.translate(qRound(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale), qRound(-m_doc->minCanvasCoordinate.y() * m_viewMode.scale));
		repaint(newR.adjusted(-20, -20, 40, 40));
	}
	oldRect = newR;
}

void Canvas::getGroupRectScreen(double *x, double *y, double *w, double *h)
{
	double gx, gy, gh, gw;
	m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
	QPoint in(qRound(gx* m_viewMode.scale), qRound(gy* m_viewMode.scale));
//	in -= QPoint(qRound(m_doc->minCanvasCoordinate.x() * m_viewMode.scale), qRound(m_doc->minCanvasCoordinate.y() * m_viewMode.scale));
	QPoint out = contentsToViewport(in);
	*x = static_cast<double>(out.x());
	*y = static_cast<double>(out.y());
	*w = gw* m_viewMode.scale;
	*h = gh* m_viewMode.scale;
}


void Canvas::Transform(PageItem *currItem, QPainter *p)
{
	p->translate(static_cast<int>(currItem->xPos()*m_viewMode.scale), static_cast<int>(currItem->yPos()*m_viewMode.scale));
	p->scale(m_viewMode.scale, m_viewMode.scale);
	p->rotate(currItem->rotation());
}

void Canvas::Transform(PageItem *currItem, QMatrix& m)
{
	m.translate(static_cast<int>(currItem->xPos()*m_viewMode.scale), static_cast<int>(currItem->yPos()*m_viewMode.scale));
	m.scale(m_viewMode.scale, m_viewMode.scale);
	m.rotate(currItem->rotation());
}

void Canvas::TransformM(PageItem *currItem, QPainter *p)
{
	if (currItem->imageFlippedH())
	{
		p->translate(currItem->width(), 0);
		p->scale(-1, 1);
	}
	if (currItem->imageFlippedV())
	{
		p->translate(0, currItem->height());
		p->scale(1, -1);
	}
}

void Canvas::calculateFrameLinkPoints(PageItem *pi1, PageItem *pi2, FPoint & start, FPoint & end)
{
	if (pi1==0 || pi2==0)
		return;
	//Calculate the link points of the frames
	double x11 = pi1->xPos();
	double y11 = pi1->yPos();
	double x12 = x11+pi1->width();
	double y12 = y11+pi1->height();
	double x1mid = x11+(x12-x11)/2;
	double y1mid = y11+(y12-y11)/2;
					
	if (pi1->rotation()!=0.000)
	{
		FPoint tempPoint(0,0, x11, y11, pi1->rotation(), 1, 1);
		x11=tempPoint.x();
		y11=tempPoint.y();
		FPoint tempPoint2(0,0, x12, y12, pi1->rotation(), 1, 1);
		x12=tempPoint2.x();
		y12=tempPoint2.y();
		FPoint tempPoint3(0,0, x1mid, y1mid, pi1->rotation(), 1, 1);
		x1mid=tempPoint3.x();
		y1mid=tempPoint3.y();
	}
					
					
	double a1, b1, a2, b2;
	a1 = a2 = b1 = b2 = 0;
	double x21 = pi2->xPos();
	double y21 = pi2->yPos();
	double x22 = x21+pi2->width();
	double y22 = y21+pi2->height();
	double x2mid = x21 + pi2->width()/2;
	double y2mid = y21 + pi2->height()/2;
					
	if (pi2->rotation()!=0.000)
	{
		FPoint tempPoint(0,0, x21, y21, pi2->rotation(), 1, 1);
		x21=tempPoint.x();
		y21=tempPoint.y();
		FPoint tempPoint2(0,0, x22, y22, pi2->rotation(), 1, 1);
		x22=tempPoint2.x();
		y22=tempPoint2.y();
		FPoint tempPoint3(0,0, x2mid, y2mid, pi2->rotation(), 1, 1);
		x2mid=tempPoint3.x();
		y2mid=tempPoint3.y();
	}
					
	if (x22<x11) { a1 = x11; a2 = x22; }
	if (x21>x12) { a1 = x12; a2 = x21; }
	if (y22<y11) { b1 = y11; b2 = y22; }
	if (y21>y12) { b1 = y12; b2 = y21; }
					
	if (x21<x12 && x21>x11) { a1 = x1mid; a2 = x2mid; }
	if (x21<x11 && x22>x11) { a1 = x1mid; a2 = x2mid; }
					
	if (y21<y12 && y21>y11) { b1 = y1mid; b2 = y2mid; }
	if (y21<y11 && y22>y11) { b1 = y1mid; b2 = y2mid; }
	//Set the link frame lines' endpoints
	start.setXY(a1-pi1->xPos(), b1-pi1->yPos());
	start.transform(pi1->xPos(), pi1->yPos(), pi1->rotation(), 1, 1, false);
	end.setXY(a2-pi2->xPos(), b2-pi2->yPos());
	end.transform(pi2->xPos(), pi2->yPos(), pi2->rotation(), 1, 1, false);
}



