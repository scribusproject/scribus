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

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

// #include <QDebug>
#include <QToolTip>
#include <QWidget>

#include "appmodes.h"
#include "canvas.h"
#include "canvasmode.h"
#include "pageitem_textframe.h"
#include "pageitem_group.h"
#include "prefsmanager.h"
#include "scpage.h"
#include "scpainter.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/hruler.h"
#include "ui/vruler.h"
#include "util.h"
#include "util_math.h"
#include "units.h"

#define DRAW_DEBUG_LINES 0

static QPoint contentsToViewport(QPoint p)
{
	return p;
}

QDataStream &operator<< ( QDataStream & ds, const CanvasViewMode & vm )
{
	ds << vm.scale
	<< vm.previewMode
	<< vm.viewAsPreview
	<< vm.previewVisual
	<< vm.m_MouseButtonPressed
	<< vm.operItemMoving
	<< vm.operItemResizing
	<< vm.operItemSelecting
	<< vm.redrawPolygon
	<< vm.linkedFramesToShow
	<< vm.drawSelectedItemsWithControls
	<< vm.drawFramelinksWithContents
	<< vm.forceRedraw;
	return ds;
}

QDataStream &operator>> ( QDataStream & ds, CanvasViewMode & vm )
{
	ds >> vm.scale
	>> vm.previewMode
	>> vm.viewAsPreview
	>> vm.previewVisual
	>> vm.m_MouseButtonPressed
	>> vm.operItemMoving
	>> vm.operItemResizing
	>> vm.operItemSelecting
	>> vm.redrawPolygon
	>> vm.linkedFramesToShow
	>> vm.drawSelectedItemsWithControls
	>> vm.drawFramelinksWithContents
	>> vm.forceRedraw;
	return ds;
}
	

Canvas::Canvas(ScribusDoc* doc, ScribusView* parent) : QWidget(parent), m_doc(doc), m_view(parent)
{
	setAutoFillBackground(true);
	setAttribute(Qt::WA_OpaquePaintEvent, true);
	setAttribute(Qt::WA_NoSystemBackground, true);
	m_buffer = QPixmap();
	m_bufferRect = QRect();
	m_renderMode = RENDER_NORMAL;
}

void Canvas::setPreviewVisual(int mode)
{
	m_viewMode.previewVisual = qMax(0, mode);
	m_viewMode.viewAsPreview = (mode >= 0);
}
// ______________________________
// Coordinate Conversion Routines:


FPoint Canvas::localToCanvas(QPoint p) const
{

/* Allow the user to select the exact coordinate represented by a ruler mark
   when the mouse is lined up with the ruler, rather than returning the 
   coordinate represented by the mathematical centre of the pixel which
   may not be exactly the same as the coordinate represented by the ruler.
*/

// (xmin, xmax) = canvas top-left
	double xmin = m_doc->minCanvasCoordinate.x();
	double ymin = m_doc->minCanvasCoordinate.y();

// (xoff, yoff) = ruler origin relative to canvas top-left
	double xoff = m_doc->rulerXoffset - xmin;
	double yoff = m_doc->rulerYoffset - ymin;
	if (m_doc->guidesPrefs().rulerMode) {
		xoff += m_doc->currentPage()->xOffset();
		yoff += m_doc->currentPage()->yOffset();
	}
// (xsp, ysp) = spacing of ruler divisions
	double xsp = m_doc->view()->horizRuler->ruleSpacing();
	double ysp = m_doc->view()->vertRuler->ruleSpacing();

	double sc = m_viewMode.scale;

// number of ruler divisions from ruler origin to ruler mark closest to the
// selected mouse coordinate
	double xn = qRound((p.x()/sc - xoff)/xsp);
	double yn = qRound((p.y()/sc - yoff)/ysp);

// xn*xsp + xoff, yn*ysp + yoff = distance in canvas coordinates
// from canvas top-left to ruler mark closest to selected mouse coordinate

// If these round to the selected mouse coordinate, use these to define
// (x,y), the coordinates of the desired point relative to the canvas top-left.
// Otherwise, simply scale the mouse coordinates.
	double x = ( qRound(sc*(xn*xsp + xoff)) == p.x() ? 
	             xn * xsp + xoff : p.x() / sc );
	double y = ( qRound(sc*(yn*ysp + yoff)) == p.y() ? 
	             yn * ysp + yoff : p.y() / sc );
		
// Finally, add xmin,ymin to get the absolute canvas coordinates of the
// desired point.
	return FPoint(x + xmin, y + ymin);
}


/*
FPoint Canvas::localToCanvas(QPointF p) const
{
	return FPoint(p.x() / m_viewMode.scale + m_doc->minCanvasCoordinate.x() , 
				  p.y() / m_viewMode.scale + m_doc->minCanvasCoordinate.y());	
}
*/


QPoint Canvas::canvasToLocal(const FPoint& p) const
{
	return { qRound((p.x() - m_doc->minCanvasCoordinate.x()) * m_viewMode.scale),
	         qRound((p.y() - m_doc->minCanvasCoordinate.y()) * m_viewMode.scale) };
}


QPoint Canvas::canvasToLocal(QPointF p) const
{
	return { qRound((p.x() - m_doc->minCanvasCoordinate.x()) * m_viewMode.scale),
	         qRound((p.y() - m_doc->minCanvasCoordinate.y()) * m_viewMode.scale) };
}


QRect Canvas::canvasToLocal(const QRectF& p) const
{
	return { qRound((p.x() - m_doc->minCanvasCoordinate.x()) * m_viewMode.scale),
	         qRound((p.y() - m_doc->minCanvasCoordinate.y()) * m_viewMode.scale),
	         qRound(p.width() * m_viewMode.scale),
	         qRound(p.height() * m_viewMode.scale) };
}

QRectF Canvas::canvasToLocalF(const QRectF& p) const
{
	return { (p.x() - m_doc->minCanvasCoordinate.x()) * m_viewMode.scale,
	         (p.y() - m_doc->minCanvasCoordinate.y()) * m_viewMode.scale,
	         p.width() * m_viewMode.scale,
	         p.height() * m_viewMode.scale };
}


QPoint Canvas::canvasToGlobal(const FPoint& p) const
{
	return mapToParent(QPoint(0,0)) + parentWidget()->mapToGlobal(QPoint(0, 0)) + canvasToLocal(p);
}


QPoint Canvas::canvasToGlobal(QPointF p) const
{
	return mapToParent(QPoint(0,0)) + parentWidget()->mapToGlobal(QPoint(0, 0)) + canvasToLocal(p);
}


QRect Canvas::canvasToGlobal(const QRectF& p) const
{
	return { mapToParent(QPoint(0,0) + canvasToLocal(p.topLeft())) + parentWidget()->mapToGlobal(QPoint(0, 0)),
	         QSize(qRound(p.width() * m_viewMode.scale), qRound(p.height() * m_viewMode.scale)) };
}


FPoint Canvas::globalToCanvas(QPoint p) const
{
	return localToCanvas(p - (mapToParent(QPoint(0, 0)) + parentWidget()->mapToGlobal(QPoint(0, 0))));
}


/*
FPoint Canvas::globalToCanvas(QPointF p) const
{
	return localToCanvas(p - mapToGlobal(QPoint(0, 0)));
}
*/


QRectF Canvas::globalToCanvas(QRect p) const
{
	FPoint org = globalToCanvas(p.topLeft());
	return { org.x(), org.y(), p.width() / m_viewMode.scale, p.height() / m_viewMode.scale };
}


/*
QRectF Canvas::globalToCanvas(QRectF p) const
{
	FPoint org = globalToCanvas(p.topLeft());
	return QRectF(org.x(), org.y(), p.width() / m_viewMode.scale, p.height() / m_viewMode.scale);
}
*/


// ________________________
// Tests for Finding Things:


bool Canvas::hitsCanvasPoint(QPoint globalPoint, const FPoint& canvasPoint) const
{
	QPoint localPoint1 = globalPoint - (mapToParent(QPoint(0,0)) + parentWidget()->mapToGlobal(QPoint(0, 0)));
	QPoint localPoint2 = canvasToLocal(canvasPoint);
	int radius = m_doc->guidesPrefs().grabRadius;
	return qAbs(localPoint1.x() - localPoint2.x()) < radius && qAbs(localPoint1.y() - localPoint2.y()) < radius;
}


bool Canvas::hitsCanvasPoint(QPoint globalPoint, QPointF canvasPoint) const
{
	QPoint localPoint1 = globalPoint - (mapToParent(QPoint(0,0)) + parentWidget()->mapToGlobal(QPoint(0, 0)));
	QPoint localPoint2 = canvasToLocal(canvasPoint);
	int radius = m_doc->guidesPrefs().grabRadius;
	return qAbs(localPoint1.x() - localPoint2.x()) < radius && qAbs(localPoint1.y() - localPoint2.y()) < radius;
}

bool Canvas::hitsCanvasPoint(const FPoint& globalPoint, const QPointF& canvasPoint) const
{
	double radius = m_doc->guidesPrefs().grabRadius;
	return qAbs(globalPoint.x() - canvasPoint.x()) < radius && qAbs(globalPoint.y() - canvasPoint.y()) < radius;
}

QRect Canvas::exposedRect() const
{
	int ex( -(x() / m_viewMode.scale) + m_doc->minCanvasCoordinate.x());
	int ey( -(y() / m_viewMode.scale) + m_doc->minCanvasCoordinate.y());
	int ew( (m_view->visibleWidth() * 1.2) / m_viewMode.scale);
	int eh( (m_view->visibleHeight() * 1.2) / m_viewMode.scale);
	
	return { ex, ey, ew, eh };
}


/// Little helper to calculate |p|^2

static double length2(const QPointF& p)
{
	return p.x()*p.x() + p.y()*p.y();
}



/*!
  returns -1 if canvasPoint is outside the frame + grabradius.
  returns frameHandle if canvasPoint is near a framehandle
  otherwise 0
 */
Canvas::FrameHandle Canvas::frameHitTest(QPointF canvasPoint, PageItem* item) const
{
	// As item->getTransform() will translate to Pos, we need to adjust the rect passed
	// to frameHitTest in order to take line width, which in very *creative* design can
	// be huge, into account.
	// ### might be interesting to investigate if it would be painless to just change 
	// PageItem::getTransform.
	double extraS = - item->visualLineWidth() / 2.0;
//	if (item->lineColor() != CommonStrings::None)
//		extraS = (item->lineWidth() / -2.0);
	if (item->isTextFrame() && (m_doc->appMode == modeEdit) && !item->asTextFrame()->availableRegion().contains(item->getTransform().inverted().map(canvasPoint.toPoint())))
		return OUTSIDE;
	QRectF visualRect = item->isLine() ? QRectF(0, extraS, item->visualWidth(), item->visualHeight()) 
		                               : QRectF(extraS, extraS, item->visualWidth(), item->visualHeight());
	Canvas::FrameHandle result = frameHitTest(item->getTransform().inverted().map(canvasPoint), visualRect);
//	qDebug() << "frameHitTest for item" << item->ItemNr 
//		<< item->getTransform().inverted().map(canvasPoint) 
//		<< item->getTransform().inverted() 
//		<< QRectF(0, 0, item->width(), item->height());
	return result;
}

Canvas::FrameHandle Canvas::frameHitTest(QPointF canvasPoint, const QRectF& frame) const
{
	FrameHandle result = INSIDE;
	const double radius = m_doc->guidesPrefs().grabRadius / m_viewMode.scale;
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
//		qDebug() << "framehittest" << canvasPoint << frame << "-> OUTSIDE";
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
	
//	qDebug() << "framehittest" << canvasPoint << frame << "->" << result;
	return result;
}


PageItem* Canvas::itemUnderCursor(QPoint globalPos, PageItem* itemAbove, bool allowInGroup, bool allowMasterItems) const
{
	PageItem* currItem;
	QRectF mouseArea = globalToCanvas(QRect(globalPos, QSize(2*m_doc->guidesPrefs().grabRadius, 2*m_doc->guidesPrefs().grabRadius)));
	// look for masterpage items first
	if (allowMasterItems && !m_doc->masterPageMode() && m_doc->currentPage()->FromMaster.count() != 0)
	{
		ScPage* Mp = m_doc->MasterPages.at(m_doc->MasterNames[m_doc->currentPage()->masterPageName()]);
		// if itemAbove is given, we expect to find it among the masterpage items of this page
		int currNr = itemAbove? m_doc->currentPage()->FromMaster.indexOf(itemAbove)-1 : m_doc->currentPage()->FromMaster.count()-1;
		if (currNr < 0)
			return nullptr;
		while (currNr >= 0)
		{
			currItem = m_doc->currentPage()->FromMaster.at(currNr);
			QTransform itemPos;
			if (m_doc->canSelectItemOnLayer(currItem->m_layerID))
			{
				if (!currItem->ChangedMasterItem)
				{
					itemPos.translate(-Mp->xOffset() + m_doc->currentPage()->xOffset(), -Mp->yOffset() + m_doc->currentPage()->yOffset());
				}
				currItem->getTransform(itemPos);
				QPainterPath currPath;
				if (currItem->isLine())
				{
					double visualLineWidth = currItem->visualLineWidth();
					currPath.moveTo(itemPos.map(QPointF(0.0, -visualLineWidth / 2.0)));
					currPath.lineTo(itemPos.map(QPointF(currItem->width(), -visualLineWidth / 2.0)));
					currPath.lineTo(itemPos.map(QPointF(currItem->width(),  visualLineWidth / 2.0)));
					currPath.lineTo(itemPos.map(QPointF(0.0,  visualLineWidth / 2.0)));
				}
				else
				{
					currPath.moveTo(itemPos.map(QPointF(0, 0)));
					currPath.lineTo(itemPos.map(QPointF(currItem->width(), 0)));
					currPath.lineTo(itemPos.map(QPointF(currItem->width(), currItem->height())));
					currPath.lineTo(itemPos.map(QPointF(0, currItem->height())));
				}
				currPath.closeSubpath();
				QPainterPath currClip;
				currClip.addPolygon(itemPos.map(QPolygonF(currItem->Clip)));
				currClip.closeSubpath();
				if (currPath.intersects(mouseArea) || currClip.intersects(mouseArea))
				{
					if (currItem->isGroup() && allowInGroup)
					{
						currItem->asGroupFrame()->adjustXYPosition();
						PageItem* ret = itemInGroup(currItem, mouseArea);
						if (ret != nullptr)
							return ret;
					}
					return currItem;
				}
			}
			--currNr;
		}
	}
	// now look for normal items
	if (m_doc->Items->count() == 0)
		return nullptr;

	QList<PageItem*> *itemList = (itemAbove && itemAbove->isGroupChild()) ? &itemAbove->parentGroup()->groupItemList : m_doc->Items;
	int currNr = itemAbove ? itemList->indexOf(itemAbove) - 1 : itemList->count() - 1;
	while (currNr >= 0)
	{
		currItem = itemList->at(currNr);
		if ((m_doc->masterPageMode())  && (!((currItem->OwnPage == -1) || (currItem->OwnPage == static_cast<int>(m_doc->currentPage()->pageNr())))))
		{
			--currNr;
			continue;
		}
		if ((m_doc->drawAsPreview && !m_doc->editOnPreview) && !(currItem->isAnnotation() || currItem->isGroup()))
		{
			--currNr;
			continue;
		}
		if (m_doc->canSelectItemOnLayer(currItem->m_layerID))
		{
			QTransform itemPos = currItem->getTransform();
			QPainterPath currPath;
			if (currItem->isLine())
			{
				double visualLineWidth = currItem->visualLineWidth();
				currPath.moveTo(itemPos.map(QPointF(0.0, -visualLineWidth / 2.0)));
				currPath.lineTo(itemPos.map(QPointF(currItem->width(), -visualLineWidth / 2.0)));
				currPath.lineTo(itemPos.map(QPointF(currItem->width(),  visualLineWidth / 2.0)));
				currPath.lineTo(itemPos.map(QPointF(0.0,  visualLineWidth / 2.0)));
			}
			else
			{
				currPath.moveTo(itemPos.map(QPointF(0, 0)));
				currPath.lineTo(itemPos.map(QPointF(currItem->width(), 0)));
				currPath.lineTo(itemPos.map(QPointF(currItem->width(), currItem->height())));
				currPath.lineTo(itemPos.map(QPointF(0, currItem->height())));
			}
			currPath.closeSubpath();
			QPainterPath currClip;
			currClip.addPolygon(itemPos.map(QPolygonF(currItem->Clip)));
			currClip.closeSubpath();
			if (currPath.intersects(mouseArea) || currClip.intersects(mouseArea))
			{
				if (currItem->isGroup() && allowInGroup)
				{
					currItem->asGroupFrame()->adjustXYPosition();
					PageItem* ret = itemInGroup(currItem, mouseArea);
					if (ret != nullptr)
					{
						if ((m_doc->drawAsPreview && !m_doc->editOnPreview) && !ret->isAnnotation())
							return nullptr;
						return ret;
					}
				}
				return currItem;
			}
		}
		--currNr;
	}
	return nullptr;
}

bool Canvas::cursorOverTextFrameControl(QPoint globalPos, PageItem* frame)
{
	FPoint mp=globalToCanvas(globalPos);
	qreal sideLength = 10 / qMax(m_viewMode.scale, 1.0);
	qreal left  = frame->xPos() + frame->width() - sideLength;// / 2;
	qreal right = left + sideLength;
	qreal top   = frame->yPos() + frame->height() - sideLength;// * 1.5;
	qreal bottom = top + sideLength;
	return mp.x()>left && mp.x()<right && mp.y()>top && mp.y()<bottom;
}

bool Canvas::cursorOverFrameControl(QPoint globalPos, const QRectF& targetRect, PageItem* frame)
{
	FPoint mp = globalToCanvas(globalPos);
	QRectF tg = targetRect.translated(frame->xPos(), frame->yPos());
	return tg.contains(QPointF(mp.x(), mp.y()));
}

PageItem* Canvas::itemInGroup(PageItem* group, const QRectF& mouseArea) const
{
	int currNr = group->groupItemList.count() - 1;
	while (currNr >= 0)
	{
		PageItem* embedded = group->groupItemList.at(currNr);
		QTransform itemPosN = embedded->getTransform();
		QPainterPath currPath(itemPosN.map(QPointF(0, 0)));
		currPath.lineTo(itemPosN.map(QPointF(embedded->width(), 0)));
		currPath.lineTo(itemPosN.map(QPointF(embedded->width(), embedded->height())));
		currPath.lineTo(itemPosN.map(QPointF(0, embedded->height())));
		currPath.closeSubpath();
		QPainterPath currClip;
		currClip.addPolygon(itemPosN.map(QPolygonF(embedded->Clip)));
		currClip.closeSubpath();
		currClip.translate(embedded->gXpos, embedded->gYpos);
		if (currPath.intersects(mouseArea) || currClip.intersects(mouseArea))
		{
			if (embedded->isGroup())
			{
				PageItem* ret = itemInGroup(embedded, mouseArea);
				if (ret != nullptr)
					return ret;
			}
			else
				return embedded;
		}
		--currNr;
	}
	return nullptr;
}

PageItem * Canvas::itemUnderItem(PageItem * item, int& index) const
{
	int indice = qMin(index, m_doc->Items->count());
	if (index < 0 || indice < 0)
		return nullptr;

	int itemid = m_doc->Items->indexOf(item);
	QRectF baseRect(item->getBoundingRect());
	int itemLevel = m_doc->layerLevelFromID(item->m_layerID);
	if (itemLevel < 0)
		return nullptr;

	for(index = indice - 1; index >= 0; --index)
	{
		PageItem* item1 = m_doc->Items->at(index);
		int item1id = m_doc->Items->indexOf(item1);
		int level = m_doc->layerLevelFromID(item1->m_layerID);
		if ((item != item1) && (level >= 0) && (level <= itemLevel))
		{
			if ((level == itemLevel) && (item1id > itemid))
				continue;
			QRectF uRect(item1->getBoundingRect());
			if (baseRect.intersects(uRect))
				return item1;
		}
	}
	return nullptr;
}

// __________________
// Buffered rendering:

/*
 Rendermodes:
 
 m_buffer holds the current page(s)
 m_bufferRect describes the contents in local coordinates:

 minCanvasCoordinate |-> local (0,0) 
 
 (0,0) |-> local (scale*minCanvasCoordinate) 
 
 local m_bufferRect.topLeft |-> buffer (0,0)
 
 
 */

void Canvas::setRenderMode(RenderMode mode)
{
//	qDebug() << "setRenderMode" << m_renderMode << "-->" << mode;
	if ( (mode < RENDER_SELECTION_SEPARATE) != (m_renderMode < RENDER_SELECTION_SEPARATE) )
	{
		clearBuffers();
	}
	m_renderMode = mode;
}


void Canvas::clearBuffers()
{
	m_buffer = QPixmap();
	m_bufferRect = QRect();
	m_selectionBuffer = QPixmap();
	m_selectionRect = QRect();
}

void Canvas::setScale(double scale)
{
	if (m_viewMode.scale == scale)
		return;
	m_viewMode.scale = scale;
	clearBuffers();
	update();
}


bool Canvas::adjustBuffer()
{
	bool ret = false;
	QRect viewport(-x(), -y(), m_view->viewport()->width(), m_view->viewport()->height());
// 	qDebug()<<"Canvas::adjustBuffer"<<viewport<<m_viewMode.forceRedraw<<m_viewMode.operItemSelecting;
	QPoint minCanvasCoordinate = canvasToLocal(QPointF(0.0, 0.0));
	if (minCanvasCoordinate != m_oldMinCanvasCoordinate)
	{
		m_bufferRect.translate(minCanvasCoordinate.x() - m_oldMinCanvasCoordinate.x(),
							   minCanvasCoordinate.y() - m_oldMinCanvasCoordinate.y());
		m_oldMinCanvasCoordinate = minCanvasCoordinate;
	}
#if DRAW_DEBUG_LINES
//	qDebug() << "adjust buffer" << m_bufferRect << "for viewport" << viewport;
#endif
	if (!m_bufferRect.isValid())
	{
//		qDebug() << "adjust buffer: invalid buffer, viewport" << viewport;
		m_bufferRect = viewport;
		m_buffer = createPixmap(m_bufferRect.width(), m_bufferRect.height());
		fillBuffer(&m_buffer, m_bufferRect.topLeft(), m_bufferRect);
		ret = true;
#if DRAW_DEBUG_LINES
		QPainter p(&m_buffer);
		p.setPen(Qt::blue);
		p.drawLine(0, 0, m_buffer.width(), m_buffer.height());
		p.drawLine(m_buffer.width(), 0, 0, m_buffer.height());
		p.end();
#endif
	}
	else if (!m_bufferRect.contains(viewport))
	{
		QRect newRect(m_bufferRect);
		if (m_bufferRect.left() > viewport.left())
			newRect.translate(viewport.left() - m_bufferRect.left(), 0);
		if (m_bufferRect.right() < viewport.right())
			newRect.translate(viewport.right() - m_bufferRect.right(), 0);
		if (m_bufferRect.top() > viewport.top())
			newRect.translate(0, viewport.top() - m_bufferRect.top());
		if (m_bufferRect.bottom() < viewport.bottom())
			newRect.translate(0, viewport.bottom() - m_bufferRect.bottom());
//		qDebug() << "adjust buffer: " << m_bufferRect << "outside viewport" << viewport << " new rect:" << newRect;
		
		if (!m_bufferRect.intersects(newRect))
		{
//			qDebug() << "adjust buffer: fresh buffer" << m_bufferRect << "-->" << newRect;
			m_bufferRect = newRect;
			m_buffer = createPixmap(m_bufferRect.width(), m_bufferRect.height());
			fillBuffer(&m_buffer, m_bufferRect.topLeft(), m_bufferRect);
			ret = true;
#if DRAW_DEBUG_LINES
			QPainter p(&m_buffer);
			p.setPen(Qt::blue);
			p.drawLine(0, 0, m_buffer.width(), m_buffer.height());
			p.drawLine(m_buffer.width(), 0, 0, m_buffer.height());
			p.end();
#endif			
		}
		else
		{
			// copy buffer:
			QPixmap newBuffer = createPixmap(newRect.width(), newRect.height());
			QPainter p(&newBuffer);
			int xpos = m_bufferRect.x() - newRect.x();
			int ypos = m_bufferRect.y() - newRect.y();
			int x = 0;
			int y = 0;
			int width = m_bufferRect.width();
			int height = m_bufferRect.height();
			if (xpos < 0)
			{
				x = -xpos;
				width -= x;
				xpos = 0;
			}
			if (ypos < 0)
			{
				y = -ypos;
				height -= y;
				ypos = 0;
			}
			if (xpos + width > newRect.width())
			{
				width = newRect.width() - xpos;
			}
			if (ypos + height > newRect.height())
			{
				height = newRect.height() - ypos;
			}

			drawPixmap(p, xpos, ypos, m_buffer, x, y, width + 1, height + 1);
#if DRAW_DEBUG_LINES
			p.setPen(Qt::blue);
			p.drawLine(xpos, ypos+height/2, xpos+width/2, ypos);
			p.drawLine(xpos+width, ypos+height/2, xpos+width/2, ypos);
			p.drawLine(xpos, ypos+height/2, xpos+width/2, ypos+height);
			p.drawLine(xpos+width, ypos+height/2, xpos+width/2, ypos+height);
//			qDebug() << "adjust buffer old" << m_bufferRect << "@" << xpos << ypos << "--> new" << newRect;
#endif
			p.end();

			// #8548, #13470 do not return true in this case, we only partially update buffer content
			// and the redraw rect from paintEvent may not be included in the updated area if
			// canvas has just been resized, after an object has been put in scrap area for eg.
			if (newRect.top() < m_bufferRect.top())
			{
				fillBuffer(&newBuffer, newRect.topLeft(), QRect(newRect.left(), newRect.top(), newRect.width(), m_bufferRect.top() - newRect.top() + 2));
				//ret = true;
			}
			if (newRect.bottom() > m_bufferRect.bottom())
			{
				fillBuffer(&newBuffer, newRect.topLeft(), QRect(newRect.left(), m_bufferRect.bottom() - 1, newRect.width(), newRect.bottom() - m_bufferRect.bottom() + 2));
				//ret = true;
			}
			if (newRect.left() < m_bufferRect.left())
			{
				fillBuffer(&newBuffer, newRect.topLeft(), QRect(newRect.left(), m_bufferRect.top(), m_bufferRect.left() - newRect.left() + 2, m_bufferRect.height()));
				//ret = true;
			}
			if (newRect.right() > m_bufferRect.right())
			{
				fillBuffer(&newBuffer, newRect.topLeft(), QRect(m_bufferRect.right() - 1, m_bufferRect.top(), newRect.right() - m_bufferRect.right() + 2, m_bufferRect.height()));
				//ret = true;
			}
			m_buffer = newBuffer;
			m_bufferRect = newRect;
#if DRAW_DEBUG_LINES
			QPainter p2(&m_buffer);
			p2.setPen(Qt::blue);
			p2.drawRect(xpos, ypos, width, height);
			p2.end();
#endif
		}
	}
//	else
//		qDebug() << "adjustBuffer: reusing" << m_bufferRect;
// 	qDebug() << "Canvas::adjustBuffer"<<ret;
	return ret;
}

void Canvas::fillBuffer(QPaintDevice* buffer, QPoint bufferOrigin, QRect clipRect)
{
// 	qDebug()<<"Canvas::fillBuffer"<<clipRect<<m_viewMode.forceRedraw<<m_viewMode.operItemSelecting;
	QPainter painter(buffer);
	painter.translate(-bufferOrigin.x(), -bufferOrigin.y());
	drawContents(&painter, clipRect.x(), clipRect.y(), clipRect.width(), clipRect.height());
	painter.end();
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
//	qDebug()<<"Canvas::paintEvent"<<p->rect()<<m_viewMode.forceRedraw<<m_viewMode.operItemSelecting;
// 	printBacktrace(62);
	if (m_doc->isLoading())
		return;
// #define SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
	QTime t;
	QString dmode("");
	int t1,t2,t3,t4,t5,t6;
	t1 = t2=t3=t4=t5 =t6= 0;
	t.start();
#endif
	// fill buffer if necessary
	bool bufferFilled = adjustBuffer();
	QPainter qp(this);
	switch (m_renderMode)
	{
		case RENDER_NORMAL:
		{
#if DRAW_DEBUG_LINES
//			qDebug() << "update Buffer:" << m_bufferRect << p->rect() << m_viewMode.forceRedraw;
#endif
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
			dmode = "NORMAL";
			t1 = t.elapsed();
			t.start();
#endif
			if ((m_viewMode.forceRedraw || m_viewMode.operTextSelecting) && (!bufferFilled))
			{
//				qDebug() << "Canvas::paintEvent: forceRedraw=" << m_viewMode.forceRedraw << "bufferFilled=" << bufferFilled;
				fillBuffer(&m_buffer, m_bufferRect.topLeft(), p->rect());
			}
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
			t2 = t.elapsed();
			t.start();
#endif
			int xV = p->rect().x() - m_bufferRect.x();
			int yV = p->rect().y() - m_bufferRect.y();
			int wV = p->rect().width();
			int hV = p->rect().height();
			if (hV > 0 && wV > 0)
			{
				drawPixmap(qp, p->rect().x(), p->rect().y(), m_buffer, xV, yV,  wV, hV);
#if DRAW_DEBUG_LINES
//				qDebug() << "normal rendering" << xV << yV << wV << hV << "at" << p->rect().x() << p->rect().y();
				qp.setPen(Qt::blue);
				qp.drawLine(p->rect().x(), p->rect().y(), p->rect().x() + p->rect().width(), p->rect().y() + p->rect().height());
				qp.drawLine(p->rect().x() + p->rect().width(), p->rect().y(), p->rect().x(), p->rect().y() + p->rect().height());
#endif
			}
		}
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
			t3 = t.elapsed();
			t.start();
#endif
			if (m_doc->appMode != modeNormal)
			{
				qp.save();
				qp.scale(m_viewMode.scale, m_viewMode.scale);
				qp.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
				drawControls( &qp );
				qp.restore();
			}	
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
			t4 = t.elapsed();
			t.start();
#endif			
			break;
		case RENDER_BUFFERED:
		{
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
			dmode = "BUFFER";
			t1 = t.elapsed();
			t.start();
#endif
				int xV = p->rect().x() - m_bufferRect.x();
				int yV = p->rect().y() - m_bufferRect.y();
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
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
				t2 = t.elapsed();
				t.start();
#endif
				if (hV > 0 && wV > 0)
				{
					drawPixmap(qp, p->rect().x(), p->rect().y(), m_buffer, xV, yV,  wV, hV);
	#if DRAW_DEBUG_LINES
//					qDebug() << "buffered rendering" << xV << yV << wV << hV << "at" << p->rect().x() << p->rect().y();
					qp.setPen(Qt::green);
					qp.drawLine(p->rect().x(), p->rect().y(), p->rect().x() + p->rect().width(), p->rect().y() + p->rect().height());
					qp.drawLine(p->rect().x() + p->rect().width(), p->rect().y(), p->rect().x(), p->rect().y() + p->rect().height());
	#endif
				}
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
				t3 = t.elapsed();
				t.start();
#endif
				if (m_doc->appMode != modeNormal)
				{
					qp.save();
					qp.scale(m_viewMode.scale, m_viewMode.scale);
					qp.translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
					drawControls( &qp );
					qp.restore();
				}
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
				t4 = t.elapsed();
				t.start();
#endif
			}
			break;
		case RENDER_SELECTION_SEPARATE:
			break;
		case RENDER_SELECTION_BUFFERED:
			break;
		case RENDER_LEGACY:
		default:
			assert (false);
	}
	
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
	t5 = t.elapsed();
	t.start();
#endif
	// does mode specific rendering, currently selection in legacymode and nodes in nodeedit
	m_view->m_canvasMode->drawControls(&qp);
	m_view->m_canvasMode->drawSnapLine(&qp);
#ifdef SHOW_ME_WHAT_YOU_GET_IN_D_CANVA
	t6 = t.elapsed();
	qDebug()<<dmode<<t1<<t2<<t3<<t4<<t5<<t6<<"-" <<t1+t2+t3+t4+t5+t6;
#endif
	m_viewMode.forceRedraw = false;
	m_viewMode.operItemSelecting = false;
	m_viewMode.operTextSelecting = false;
}


void Canvas::drawContents(QPainter *psx, int clipx, int clipy, int clipw, int cliph)
{
//	int Tsetup, Toutlines , Tbackground, Tcontents;
//	Toutlines=0;
//	QTime tim;
//	tim.start();
// 	qDebug() << "Canvas::drawContents" << clipx << clipy << clipw << cliph<<m_viewMode.forceRedraw<<m_viewMode.operItemSelecting;
	int docPagesCount=m_doc->Pages->count();
	ScPainter *painter=nullptr;
	QImage img = QImage(clipw * devicePixelRatio(), cliph * devicePixelRatio(), QImage::Format_ARGB32_Premultiplied);
	img.setDevicePixelRatio(devicePixelRatio());
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
	painter->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	painter->setLineWidth(1);
	painter->setFillMode(ScPainter::Solid);

	ScLayer layer;
	layer.isViewable = false;
	layer.ID = 0;

	if (!m_doc->masterPageMode())
	{
		drawBackgroundPageOutlines(painter, clipx, clipy, clipw, cliph);
		m_viewMode.linkedFramesToShow.clear();
		QRectF clip = QRectF(clipx, clipy, clipw, cliph);
		DrawPageBorder(painter, clip);
		if (m_viewMode.viewAsPreview)
		{
			FPointArray PoLine;
			getClipPathForPages(&PoLine);
			painter->beginLayer(1.0, 0, &PoLine);
		}
		else
			painter->beginLayer(1.0, 0);
		int renderStackCount = m_doc->guidesPrefs().renderStackOrder.count();
		for (int r = 0; r < renderStackCount; r++)
		{
			int ri = m_doc->guidesPrefs().renderStackOrder[r];
			if (ri == 0)
			{
				if (!m_viewMode.viewAsPreview)
					DrawPageMargins(painter, clip);			// drawing stack id = 0
			}
			else if (ri == 1)
			{
				if (!m_viewMode.viewAsPreview)
					DrawPageBaselineGrid(painter, clip);	// drawing stack id = 1
			}
			else if (ri == 2)
			{
				if (!m_viewMode.viewAsPreview)
					DrawPageGrid(painter, clip);			// drawing stack id = 2
			}
			else if (ri == 3)
			{
				if (!m_viewMode.viewAsPreview)
					DrawPageGuides(painter, clip);			// drawing stack id = 3
			}
			else if (ri == 4)
			{
				int layerCount = m_doc->layerCount();	// drawing stack id = 4
				for (int layerLevel = 0; layerLevel < layerCount; ++layerLevel)
				{
					m_doc->Layers.levelToLayer(layer, layerLevel);
					for (int a = 0; a < docPagesCount; ++a)
					{
						DrawMasterItems(painter, m_doc->Pages->at(a), layer, QRect(clipx, clipy, clipw, cliph));
					}
					//first pass draws all except notes frames
					DrawPageItems(painter, layer, QRect(clipx, clipy, clipw, cliph), false);
					//seconf only for notes frames
					DrawPageItems(painter, layer, QRect(clipx, clipy, clipw, cliph), true);
				}
			}
		}
		if (!m_viewMode.viewAsPreview)
			DrawPageIndicator(painter, clip);
		painter->endLayer();
	}
	else // masterPageMode
	{			
		m_viewMode.linkedFramesToShow.clear();
		drawBackgroundMasterpage(painter, clipx, clipy, clipw, cliph);
		painter->beginLayer(1.0, 0);
		QRectF clip = QRectF(clipx, clipy, clipw, cliph);
		DrawPageBorder(painter, clip, true);
		int renderStackCount = m_doc->guidesPrefs().renderStackOrder.count();
		for (int r = 0; r < renderStackCount; r++)
		{
			int ri = m_doc->guidesPrefs().renderStackOrder[r];
			if (ri == 0)
			{
				DrawPageMargins(painter, clip, true);			// drawing stack id = 0
			}
			else if (ri == 1)
			{
				DrawPageBaselineGrid(painter, clip, true);	// drawing stack id = 1
			}
			else if (ri == 2)
			{
				DrawPageGrid(painter, clip, true);			// drawing stack id = 2
			}
			else if (ri == 3)
			{
				DrawPageGuides(painter, clip, true);			// drawing stack id = 3
			}
			else if (ri == 4)
			{
				int layerCount = m_doc->layerCount();	// drawing stack id = 4
				for (int layerLevel = 0; layerLevel < layerCount; ++layerLevel)
				{
					m_doc->Layers.levelToLayer(layer, layerLevel);
					//first pass draws all except notes frames
					DrawPageItems(painter, layer, QRect(clipx, clipy, clipw, cliph), false);
					//second pass draw only notes frames
					DrawPageItems(painter, layer, QRect(clipx, clipy, clipw, cliph), true);
				}
			}
		}
		DrawPageIndicator(painter, clip, true);
		painter->endLayer();
	}
	if (((m_doc->m_Selection->count() != 0) || (m_viewMode.linkedFramesToShow.count() != 0))  && (!m_viewMode.viewAsPreview))
	{
		drawFrameLinks(painter);
	}
	painter->end();
	psx->drawImage(clipx, clipy, img);
	delete painter;
	painter=nullptr;
// 	qDebug( "Time elapsed: %d ms, setup=%d, outlines=%d, background=%d, contents=%d, rest=%d", tim.elapsed(), Tsetup,Toutlines -Tsetup, Tbackground-Toutlines, Tcontents-Tbackground, tim.elapsed() - Tcontents );
}

void Canvas::drawControls(QPainter *psx)
{
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
	if (m_viewMode.operItemMoving || m_viewMode.operItemResizing)
	{
		if (m_viewMode.operItemResizing)
		{
			if (!m_viewMode.redrawPolygon.isEmpty())
			{
				if (m_viewMode.m_MouseButtonPressed && ((m_doc->appMode == modeDrawFreehandLine) || (m_doc->appMode == modeDrawCalligraphicLine)))
				{
					drawControlsFreehandLine(psx);
				}
				else if (m_doc->appMode != modeDrawFreehandLine)
				{
//					qDebug() << "XXX drawControls - operItemResizing";
					assert(false);
//					drawControlsHighlightRect(psx);
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
//			drawControlsSelection(psx, m_doc->m_Selection->itemAt(0));
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



/**
 
*/
void Canvas::drawControlsMovingItemsRect(QPainter* pp)
{
	int selectedItemCount = m_doc->m_Selection->count();
	if (selectedItemCount <= 0)
		return;
		
	PageItem *currItem = nullptr;
	if (selectedItemCount >= moveWithBoxesOnlyThreshold)
	{
		double gx, gy, gw, gh;
		m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		pp->translate(gx, gy);
		pp->drawRect(QRectF(0.0, 0.0, gw, gh));
		return;
	}

	for (int cu = 0; cu < selectedItemCount; cu++)
	{
		currItem = m_doc->m_Selection->itemAt(cu);
		pp->save();
		pp->translate(currItem->xPos(), currItem->yPos());
		pp->rotate(currItem->rotation());							
		pp->setBrush(Qt::NoBrush);
		pp->setPen(QPen(Qt::black, 1.0 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
		if (currItem->isGroup())
		{
			PageItem_Group* gItem = currItem->asGroupFrame();
			pp->scale(gItem->width() / gItem->groupWidth, gItem->height() / gItem->groupHeight);
			int itemCountG = gItem->groupItemList.count();
			if (itemCountG < moveWithFullOutlinesThreshold)
			{
				for (int cg = 0; cg < itemCountG; cg++)
				{
					currItem = gItem->groupItemList.at(cg);
					if (!(currItem->isLine()))
						currItem->DrawPolyL(pp, currItem->Clip);
					else
					{
						if (currItem->isLine())
						{
							int lw2 = 1;
							int lw = 1;
							Qt::PenCapStyle le = Qt::FlatCap;
							if (currItem->NamedLStyle.isEmpty())
							{
								if (currItem->lineColor() != CommonStrings::None)
								{
									lw2 = qRound(currItem->lineWidth()  / 2.0);
									lw = qRound(qMax(currItem->lineWidth(), 1.0));
								}
								le = currItem->PLineEnd;
							}
							else
							{
								multiLine ml = m_doc->docLineStyles[currItem->NamedLStyle];
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
			}
		}
		else if (selectedItemCount < moveWithFullOutlinesThreshold)
		{
			if (!currItem->asLine())
				currItem->DrawPolyL(pp, currItem->Clip);
			else
			{
				int lw2 = 1;
				int lw = 1;
				Qt::PenCapStyle le = Qt::FlatCap;
				if (currItem->NamedLStyle.isEmpty())
				{
					if (currItem->lineColor() != CommonStrings::None)
					{
						lw2 = qRound(currItem->lineWidth()  / 2.0);
						lw = qRound(qMax(currItem->lineWidth(), 1.0));
					}
					le = currItem->PLineEnd;
				}
				else
				{
					multiLine ml = m_doc->docLineStyles[currItem->NamedLStyle];
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
		else
			pp->drawRect(0, 0, static_cast<int>(currItem->width())+1, static_cast<int>(currItem->height())+1);
		pp->restore();
	}
}

/**
  draws the bezier curve in edit bezier mode
  */
void Canvas::drawControlsBezierCurve(QPainter* pp, PageItem* currItem)
{
	pp->setBrush(Qt::NoBrush);
	pp->setPen(QPen(Qt::black, 1.0 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
	pp->translate(static_cast<int>(currItem->xPos()), static_cast<int>(currItem->yPos()));
	pp->rotate(currItem->rotation());
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
  draws the measurement line in measure mode
 */
void Canvas::drawControlsMeasurementLine(QPainter* pp)
{
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
	pp->setBrush(Qt::NoBrush);
	pp->setPen(QPen(Qt::black, 1.0 / m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
	pp->drawPolyline(m_viewMode.redrawPolygon);
	m_viewMode.redrawPolygon.clear();
}


/**
  draws masterpage items of a specific layer
 */
void Canvas::DrawMasterItems(ScPainter *painter, ScPage *page, ScLayer& layer, QRect clip)
{
	if ((m_viewMode.previewMode) && (!layer.isPrintable))
		return;
	if ((m_viewMode.viewAsPreview) && (!layer.isPrintable))
		return;
	if (!layer.isViewable)
		return;
	if (page->masterPageNameEmpty())
		return;
	if (page->FromMaster.count() <= 0)
		return;

	FPoint orig = localToCanvas(clip.topLeft());
	QRectF cullingArea = QRectF(static_cast<int>(orig.x()), static_cast<int>(orig.y()), 
							  qRound(clip.width() / m_viewMode.scale + 0.5), qRound(clip.height() / m_viewMode.scale + 0.5));

	PageItem *currItem;
	ScPage* Mp = m_doc->MasterPages.at(m_doc->MasterNames[page->masterPageName()]);
	int layerCount = m_doc->layerCount();
	if ((layerCount > 1) && ((layer.blendMode != 0) || (layer.transparency != 1.0)) && (!layer.outlineMode))
		painter->beginLayer(layer.transparency, layer.blendMode);
	int pageFromMasterCount=page->FromMaster.count();
	for (int a = 0; a < pageFromMasterCount; ++a)
	{
		currItem = page->FromMaster.at(a);
		if (currItem->m_layerID != layer.ID)
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
		currItem->savedOwnPage = currItem->OwnPage;
		currItem->OwnPage = page->pageNr();
//FIXME						if (!evSpon || forceRedraw)
//					currItem->invalid = true;
		if (cullingArea.intersects(currItem->getBoundingRect().adjusted(0.0, 0.0, 1.0, 1.0)))
		{
			if (!((m_viewMode.operItemMoving) && (currItem->isSelected())))
			{
				if (m_viewMode.forceRedraw)
					currItem->invalidateLayout();
				currItem->DrawObj(painter, cullingArea);
				currItem->DrawObj_Decoration(painter);
			}
//							else 
//								qDebug() << "skip masterpage item (move/resizeEdit/selected)" << m_viewMode.operItemMoving << currItem->isSelected();
		}
		currItem->OwnPage = currItem->savedOwnPage;
		if (!currItem->ChangedMasterItem)
		{
			//Hack to not check for undo changes, indicate drawing only
			currItem->setXYPos(OldX, OldY, true);
			currItem->BoundingX = OldBX;
			currItem->BoundingY = OldBY;
		}
	}
	if ((layerCount > 1) && ((layer.blendMode != 0) || (layer.transparency != 1.0)) && (!layer.outlineMode))
		painter->endLayer();
}


/**
  draws page items contained in a specific Layer
 */
void Canvas::DrawPageItems(ScPainter *painter, ScLayer& layer, QRect clip, bool notesFramesPass)
{
	if ((m_viewMode.previewMode) && (!layer.isPrintable))
		return;
	if ((m_viewMode.viewAsPreview) && (!layer.isPrintable))
		return;
	if (!layer.isViewable)
		return;
	if (m_doc->Items->count() <= 0)
		return;

// 	qDebug()<<"Canvas::DrawPageItems"<<m_viewMode.forceRedraw<<m_viewMode.operItemSelecting;
	FPoint orig = localToCanvas(clip.topLeft());
	QRectF cullingArea = QRectF(static_cast<int>(orig.x()), static_cast<int>(orig.y()), 
							  qRound(clip.width() / m_viewMode.scale + 0.5), qRound(clip.height() / m_viewMode.scale + 0.5));

	PageItem *currItem;
	int layerCount = m_doc->layerCount();
	int docCurrPageNo=static_cast<int>(m_doc->currentPageNumber());
	if ((layerCount > 1) && ((layer.blendMode != 0) || (layer.transparency != 1.0)) && (!layer.outlineMode))
		painter->beginLayer(layer.transparency, layer.blendMode);

	//if notes are used
	//then we must be sure that text frames are valid and all notes frames are created before we start drawing
	if (!notesFramesPass && !m_doc->notesList().isEmpty())
	{
		for (auto it = m_doc->Items->begin(); it != m_doc->Items->end(); ++it)
		{
			PageItem* currItem = *it;
			if ( !currItem->isTextFrame()
				|| currItem->isNoteFrame()
				|| !currItem->invalid
				|| (currItem->m_layerID != layer.ID)
				|| (m_viewMode.previewMode && !currItem->printEnabled())
				|| (m_viewMode.viewAsPreview && (!currItem->printEnabled()))
				|| (m_doc->masterPageMode() && ((currItem->OwnPage != -1) && (currItem->OwnPage != docCurrPageNo)))
				|| ((!m_doc->masterPageMode() && !currItem->OnMasterPage.isEmpty()) && (currItem->OnMasterPage != m_doc->currentPage()->pageName())))
				continue;
			if (cullingArea.intersects(currItem->getBoundingRect().adjusted(0.0, 0.0, 1.0, 1.0)))
				currItem->layout();
		}
	}
	for (int it = 0; it < m_doc->Items->count(); ++it)
	{
		currItem = m_doc->Items->at(it);
		if (notesFramesPass && !currItem->isNoteFrame())
			continue;
		if (!notesFramesPass && currItem->isNoteFrame())
			continue;
		if (currItem->m_layerID != layer.ID)
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
		if (cullingArea.intersects(currItem->getBoundingRect().adjusted(0.0, 0.0, 1.0, 1.0)))
		{
//FIXME		if (!evSpon || forceRedraw) 
//				currItem->invalid = true;
//			if ((!m_MouseButtonPressed) || (m_doc->appMode == modeEditClip))
			if (((m_viewMode.operItemMoving || m_viewMode.drawSelectedItemsWithControls) && currItem->isSelected()))
			{
//					qDebug() << "skipping pageitem (move/resizeEdit/selected)" << m_viewMode.operItemMoving << currItem->isSelected();
			}
			else if (m_viewMode.operItemSelecting)
			{
				currItem->invalid = false;
				currItem->DrawObj(painter, cullingArea);
				currItem->DrawObj_Decoration(painter);
			}
			else
			{
				// I comment it because the "view" should not
				// alter the "data". And it really prevents optimisation - pm
// 				if (m_viewMode.forceRedraw)
// 					currItem->invalidateLayout();
				currItem->DrawObj(painter, cullingArea);
				currItem->DrawObj_Decoration(painter);
			}
			getLinkedFrames(currItem);
/*			if ((currItem->isTextFrame()) && ((currItem->nextInChain() != 0) || (currItem->prevInChain() != 0)))
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
			}*/
			/* FIXME:av -
			what to fix exactly? - pm
			*/
			if ((m_doc->appMode == modeEdit) && (currItem->isSelected()) && (currItem->itemType() == PageItem::TextFrame))
			{
				setupEditHRuler(currItem);
			}
		}
	}
	if ((layerCount > 1) && ((layer.blendMode != 0) || (layer.transparency != 1.0)) && (!layer.outlineMode))
		painter->endLayer();
}

/**
  Draws the canvas background for masterpages, incl. bleeds
 */
void Canvas::drawBackgroundMasterpage(ScPainter* painter, int clipx, int clipy, int clipw, int cliph)
{
	const ScPage* currentPage = m_doc->currentPage();
	double x = currentPage->xOffset() * m_viewMode.scale;
	double y = currentPage->yOffset() * m_viewMode.scale;
	double w = currentPage->width() * m_viewMode.scale;
	double h = currentPage->height() * m_viewMode.scale;
	QRectF drawRect = QRectF(x, y, w+5, h+5);
	drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
	if (!drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
		return;

	painter->setFillMode(ScPainter::Solid);
	painter->setBrush(QColor(128,128,128));
	MarginStruct pageBleeds;
	if (!m_doc->bleeds()->isNull() && m_doc->guidesPrefs().showBleed)
		m_doc->getBleeds(currentPage, pageBleeds);
	double px = currentPage->xOffset() - pageBleeds.left();
	double py = currentPage->yOffset() - pageBleeds.top();
	double pw = currentPage->width() + pageBleeds.left() + pageBleeds.right();
	double ph = currentPage->height() + pageBleeds.bottom() + pageBleeds.top();
	painter->setAntialiasing(false);
	painter->setPen(Qt::black, 1 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	if (PrefsManager::instance().appPrefs.displayPrefs.showPageShadow)
		painter->drawRect(px + 5, py + 5, pw, ph);
	painter->setBrush(m_doc->paperColor());
	painter->drawRect(px, py, pw, ph);
	painter->setAntialiasing(true);
}



/**
  draws the page outlines on canvas, including bleed area
 */
void Canvas::drawBackgroundPageOutlines(ScPainter* painter, int clipx, int clipy, int clipw, int cliph)
{
	int docPagesCount=m_doc->Pages->count();
	if (PrefsManager::instance().appPrefs.displayPrefs.showPageShadow)
	{
		painter->setBrush(QColor(128,128,128));
		painter->setAntialiasing(false);
		painter->setPen(Qt::black, 1.0 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		ScPage *actPg;
		MarginStruct pageBleeds;
		for (int a = 0; a < static_cast<int>(docPagesCount); ++a)
		{
			actPg = m_doc->Pages->at(a);
			m_doc->getBleeds(actPg, pageBleeds);
			if (m_viewMode.viewAsPreview)
				pageBleeds.resetToZero();
			double blx = (actPg->xOffset() - pageBleeds.left()) * m_viewMode.scale;
			double bly = (actPg->yOffset() - pageBleeds.top()) * m_viewMode.scale;
			double blw = (actPg->width() + pageBleeds.left() + pageBleeds.right()) * m_viewMode.scale;
			double blh = (actPg->height() + pageBleeds.bottom() + pageBleeds.top()) * m_viewMode.scale;
			
			QRectF drawRect = QRectF(blx-1, bly-1, blw+6, blh+6);
			drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
			if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
			{
				painter->setFillMode(ScPainter::Solid);
				double blx2 = actPg->xOffset();
				double bly2 = actPg->yOffset();
				double blw2 = actPg->width();
				double blh2 = actPg->height();
				if (m_doc->guidesPrefs().showBleed)
				{
					blx2 -= pageBleeds.left();
					bly2 -= pageBleeds.top();
					blw2 += pageBleeds.left() + pageBleeds.right();
					blh2 += pageBleeds.bottom() + pageBleeds.top();
				}
				painter->drawRect(blx2 + 5, bly2 + 5, blw2, blh2);
				if (!m_doc->bleeds()->isNull() && m_doc->guidesPrefs().showBleed)
				{
					painter->setFillMode(ScPainter::None);
					painter->setPen(Qt::black, 1.0 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					painter->drawRect(blx2 - 1 / m_viewMode.scale, bly2 - 1 / m_viewMode.scale, blw2 + 2 / m_viewMode.scale, blh2 + 2 / m_viewMode.scale);
				}
			}
		}
		painter->setAntialiasing(true);
	}
	painter->setFillMode(ScPainter::Solid);
	ScPage *actPg;
	MarginStruct pageBleeds;
	for (int a = 0; a < static_cast<int>(docPagesCount); ++a)
	{
		actPg = m_doc->Pages->at(a);
		double x = actPg->xOffset();
		double y = actPg->yOffset();
		double w = actPg->width();
		double h = actPg->height();
		bool drawBleed = false;
		if (!m_doc->bleeds()->isNull() && m_doc->guidesPrefs().showBleed)
		{
			drawBleed = true;
			m_doc->getBleeds(a, pageBleeds);
		}
		else
			pageBleeds.resetToZero();
		double blx = (actPg->xOffset() - pageBleeds.left()) * m_viewMode.scale;
		double bly = (actPg->yOffset() - pageBleeds.top()) * m_viewMode.scale;
		double blw = (actPg->width() + pageBleeds.left() + pageBleeds.right()) * m_viewMode.scale;
		double blh = (actPg->height() + pageBleeds.bottom() + pageBleeds.top()) * m_viewMode.scale;
		
		QRectF drawRect = QRectF(blx, bly, blw+5, blh+5);
		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
		{
			painter->setFillMode(ScPainter::Solid);
			painter->setPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			painter->setAntialiasing(false);
			painter->setLineWidth(0.0);
			painter->setBrush(m_doc->paperColor());
			if (!m_viewMode.viewAsPreview)
			{
				double blx2 = actPg->xOffset() - pageBleeds.left();
				double bly2 = actPg->yOffset() - pageBleeds.top();
				double blw2 = actPg->width() + pageBleeds.left() + pageBleeds.right();
				double blh2 = actPg->height() + pageBleeds.bottom() + pageBleeds.top();
				painter->drawRect(blx2, bly2, blw2, blh2);
				if (drawBleed)
					painter->drawRect(x, y, w, h);
			}
			else
				painter->drawRect(x, y, w, h);
			painter->setAntialiasing(true);
		}
	}
}

void Canvas::getLinkedFrames(PageItem* currItem)
{
	if (currItem->isGroup())
	{
		for (int em = 0; em < currItem->groupItemList.count(); ++em)
		{
			PageItem* embedded = currItem->groupItemList.at(em);
			getLinkedFrames(embedded);
		}

	}
	else if ((currItem->isTextFrame()) && ((currItem->nextInChain() != nullptr) || (currItem->prevInChain() != nullptr)))
	{
		PageItem *nextItem = currItem;
		while (nextItem != nullptr)
		{
			if (nextItem->prevInChain() != nullptr)
				nextItem = nextItem->prevInChain();
			else
				break;
		}
		if (!m_viewMode.linkedFramesToShow.contains(nextItem))
			m_viewMode.linkedFramesToShow.append(nextItem);
	}
}

/**
 fills PoLine with a clippath for the doc pages
 */
void Canvas::getClipPathForPages(FPointArray* PoLine)
{
	PoLine->resize(0);
	bool first = true;
	ScPage *actPg;
	
	int firstPage = 0;
	int lastPage  = m_doc->Pages->count();
	if (m_doc->Pages == &m_doc->MasterPages)
	{
		firstPage = m_doc->currentPage()->pageNr();
		lastPage  = firstPage + 1;
	}

	for (int i = firstPage; i < lastPage; ++i)
	{
		if (!first)
			PoLine->setMarker();
		first = false;
		actPg = m_doc->Pages->at(i);
		double x = actPg->xOffset();
		double y = actPg->yOffset();
		double w = actPg->width();
		double h = actPg->height();
		static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
			0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
		for (int j = 0; j < 29; j += 4)
		{
			PoLine->addPoint(x + w * rect[j], y + h * rect[j+1]);
			PoLine->addPoint(x + w * rect[j+2], y + h * rect[j+3]);
		}
	}
}

/**
  draws page border
  */
void Canvas::DrawPageBorderSub(ScPainter *p, ScPage *page)
{
	p->save();
	p->setAntialiasing(false);
	p->translate(page->xOffset(), page->yOffset());
	double lineWidth = 1.0 / m_viewMode.scale;
	double pageHeight = page->height();
	double pageWidth = page->width();
	p->setFillMode(ScPainter::None);
	p->setStrokeMode(ScPainter::Solid);
	p->setPen(Qt::black, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->drawRect(0, 0, pageWidth, pageHeight);
	p->setAntialiasing(true);
	p->restore();
}

void Canvas::DrawPageBorder(ScPainter *p, const QRectF& clip, bool master)
{
	if (master)
	{
		ScPage *page = m_doc->currentPage();
		double x = m_doc->scratch()->left() * m_viewMode.scale;
		double y = m_doc->scratch()->top() * m_viewMode.scale;
		double w = page->width() * m_viewMode.scale;
		double h = page->height() * m_viewMode.scale;
		QRectF drawRect = QRectF(x, y, w+5, h+5);
		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if (drawRect.intersects(clip))
			DrawPageBorderSub(p, page);
	}
	else
	{
		int docPagesCount = m_doc->Pages->count();
		for (int a = 0; a < docPagesCount; ++a)
		{
			ScPage *page = m_doc->Pages->at(a);
			QRectF drawRect = QRectF(page->xOffset() * m_viewMode.scale, page->yOffset() * m_viewMode.scale, page->width() * m_viewMode.scale + 5, page->height() * m_viewMode.scale + 5);
			drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
			if (drawRect.intersects(clip))
				DrawPageBorderSub(p, page);
		}
	}
}

/**
  draws margins
 */
void Canvas::DrawPageMarginsGridSub(ScPainter *p, ScPage *page)
{
	p->save();
	p->setAntialiasing(false);
	p->translate(page->xOffset(), page->yOffset());
	double lineWidth = 1.0 / m_viewMode.scale;
	double pageHeight = page->height();
	double pageWidth = page->width();
	p->setFillMode(ScPainter::None);
	p->setStrokeMode(ScPainter::Solid);
	p->setPen(Qt::black, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	if (m_doc->guidesPrefs().marginsShown)
	{
		p->setPen(m_doc->guidesPrefs().marginColor);
		if (m_doc->marginColored())
		{
			p->setFillMode(ScPainter::Solid);
			p->setBrush(m_doc->guidesPrefs().marginColor);
			p->drawRect(0, 0, pageWidth, page->Margins.top());
			p->drawRect(0, page->Margins.top(), page->Margins.left(), pageHeight - page->Margins.top());
			p->drawRect(page->Margins.left(), pageHeight - page->Margins.bottom(), pageWidth - page->Margins.right() - page->Margins.left(), page->Margins.bottom());
			p->drawRect(pageWidth - page->Margins.right(), page->Margins.top(), page->Margins.right(), pageHeight-page->Margins.top());
			p->setFillMode(ScPainter::None);
		}
		p->setFillMode(ScPainter::None);
		p->drawRect(page->Margins.left(), page->Margins.top(), pageWidth - page->Margins.left() - page->Margins.right(), pageHeight - page->Margins.top() - page->Margins.bottom());
	}
	p->setAntialiasing(true);
	p->restore();
}

void Canvas::DrawPageMargins(ScPainter *p, const QRectF& clip, bool master)
{
	if (master)
	{
		ScPage *page = m_doc->currentPage();
		double x = m_doc->scratch()->left() * m_viewMode.scale;
		double y = m_doc->scratch()->top() * m_viewMode.scale;
		double w = page->width() * m_viewMode.scale;
		double h = page->height() * m_viewMode.scale;
		QRectF drawRect = QRectF(x, y, w+5, h+5);
		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if (drawRect.intersects(clip))
			DrawPageMarginsGridSub(p, page);
	}
	else
	{
		int docPagesCount = m_doc->Pages->count();
		for (int a = 0; a < docPagesCount; ++a)
		{
			ScPage *page = m_doc->Pages->at(a);
			QRectF drawRect = QRectF(page->xOffset() * m_viewMode.scale, page->yOffset() * m_viewMode.scale, page->width() * m_viewMode.scale + 5, page->height() * m_viewMode.scale + 5);
			drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
			if (drawRect.intersects(clip))
				DrawPageMarginsGridSub(p, page);
		}
	}
}

/**
  draws baseline grid
 */
void Canvas::DrawPageBaselineGridSub(ScPainter *p, ScPage *page)
{
	if (!m_doc->guidesPrefs().baselineGridShown)
		return;

	p->save();
	p->setAntialiasing(false);
	p->translate(page->xOffset(), page->yOffset());
	double lineWidth = 1.0 / m_viewMode.scale;
	double pageHeight = page->height();
	double pageWidth = page->width();
	p->setFillMode(ScPainter::None);
	p->setStrokeMode(ScPainter::Solid);
	p->setPen(m_doc->guidesPrefs().baselineGridColor, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	for (double yg = m_doc->guidesPrefs().offsetBaselineGrid; yg < pageHeight; yg += m_doc->guidesPrefs().valueBaselineGrid)
		p->drawLine(FPoint(0, yg), FPoint(pageWidth, yg));
	p->setAntialiasing(true);
	p->restore();
}

void Canvas::DrawPageBaselineGrid(ScPainter *p, const QRectF& clip, bool master)
{
	if (!m_doc->guidesPrefs().baselineGridShown)
		return;

	if (master)
	{
		ScPage *page = m_doc->currentPage();
		double x = m_doc->scratch()->left() * m_viewMode.scale;
		double y = m_doc->scratch()->top() * m_viewMode.scale;
		double w = page->width() * m_viewMode.scale;
		double h = page->height() * m_viewMode.scale;
		QRectF drawRect = QRectF(x, y, w+5, h+5);
		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if (drawRect.intersects(clip))
			DrawPageBaselineGridSub(p, page);
	}
	else
	{
		int docPagesCount = m_doc->Pages->count();
		for (int a = 0; a < docPagesCount; ++a)
		{
			ScPage *page = m_doc->Pages->at(a);
			QRectF drawRect = QRectF(page->xOffset() * m_viewMode.scale, page->yOffset() * m_viewMode.scale, page->width() * m_viewMode.scale + 5, page->height() * m_viewMode.scale + 5);
			drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
			if (drawRect.intersects(clip))
				DrawPageBaselineGridSub(p, page);
		}
	}
}

/**
  draws grid
 */
void Canvas::DrawPageGridSub(ScPainter *p, ScPage *page, const QRectF& clip)
{
	if (!m_doc->guidesPrefs().gridShown)
		return;

	p->save();
	FPointArray PoLine;
	getClipPathForPages(&PoLine);
	p->beginLayer(1.0, 0, &PoLine);
	p->setAntialiasing(false);
	p->translate(page->xOffset(), page->yOffset());
	double lineWidth = 1.0 / m_viewMode.scale;
	double pageHeight = page->height();
	double pageWidth = page->width();
	p->setFillMode(ScPainter::None);
	p->setStrokeMode(ScPainter::Solid);
	p->setPen(Qt::black, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

	double lowerBx = qMax(clip.x() / m_viewMode.scale + m_doc->minCanvasCoordinate.x() - page->xOffset(), 0.0);
	double lowerBy = qMax(clip.y() / m_viewMode.scale + m_doc->minCanvasCoordinate.y() - page->yOffset(), 0.0);
	double highBx = qMin(lowerBx + clip.width() / m_viewMode.scale, pageWidth);
	double highBy = qMin(lowerBy + clip.height() / m_viewMode.scale, pageHeight);
	if (m_viewMode.scale > 0.49)
	{
		if (m_doc->guidesPrefs().gridType == 0)
		{
			double i,start;
			i = m_doc->guidesPrefs().majorGridSpacing;
			p->setPen(m_doc->guidesPrefs().majorGridColor, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			start = floor(lowerBy / i);
			start *= i;
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
			i = m_doc->guidesPrefs().minorGridSpacing;
			p->setPen(m_doc->guidesPrefs().minorGridColor, lineWidth, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
			start = floor(lowerBy / i);
			start *= i;
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
		else if (m_doc->guidesPrefs().gridType == 1)
		{
			double i, startX, startY;
			i = m_doc->guidesPrefs().minorGridSpacing;
			p->setPen(m_doc->guidesPrefs().minorGridColor, 3.0 / m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
			startY = floor(lowerBy / i);
			startY *= i;
			startX = floor(lowerBx / i);
			startX *= i;
			for (double b = startY; b <= highBy; b += i)
			{
				for (double bb = startX; bb <= highBx; bb += i)
				{
					p->drawLine(FPoint(bb, b), FPoint(bb, b));
				}
			}
			i = m_doc->guidesPrefs().majorGridSpacing;
			p->setPen(m_doc->guidesPrefs().majorGridColor, lineWidth * 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			startY = floor(lowerBy / i);
			startY *= i;
			startX = floor(lowerBx / i);
			startX *= i;
			for (double b = startY; b <= highBy; b += i)
			{
				for (double bb = startX; bb <= highBx; bb += i)
				{
					p->drawLine(FPoint(bb - 4, b), FPoint(bb + 4, b));
					p->drawLine(FPoint(bb, b - 4), FPoint(bb, b + 4));
				}
			}
		}
	}

	p->setAntialiasing(true);
	p->endLayer();
	p->restore();
}

void Canvas::DrawPageGrid(ScPainter *p, const QRectF& clip, bool master)
{
	if (!m_doc->guidesPrefs().gridShown)
		return;

	if (master)
	{
		ScPage *page = m_doc->currentPage();
		double x = m_doc->scratch()->left() * m_viewMode.scale;
		double y = m_doc->scratch()->top() * m_viewMode.scale;
		double w = page->width() * m_viewMode.scale;
		double h = page->height() * m_viewMode.scale;
		QRectF drawRect = QRectF(x, y, w+5, h+5);
		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if (drawRect.intersects(clip))
			DrawPageGridSub(p, page, clip);
	}
	else
	{
		int docPagesCount = m_doc->Pages->count();
		for (int a = 0; a < docPagesCount; ++a)
		{
			ScPage *page = m_doc->Pages->at(a);
			QRectF drawRect = QRectF(page->xOffset() * m_viewMode.scale, page->yOffset() * m_viewMode.scale, page->width() * m_viewMode.scale + 5, page->height() * m_viewMode.scale + 5);
			drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
			if (drawRect.intersects(clip))
				DrawPageGridSub(p, page, clip);
		}
	}
}

/**
  draws guides
 */
void Canvas::DrawPageGuidesSub(ScPainter *p, ScPage *page)
{
	if (!m_doc->guidesPrefs().guidesShown)
		return;

	p->save();
	p->setAntialiasing(false);
	p->translate(page->xOffset(), page->yOffset());
	double lineWidth = 1.0 / m_viewMode.scale;
	p->setFillMode(ScPainter::None);
	p->setStrokeMode(ScPainter::Solid);
	p->setPen(Qt::black, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	page->guides.drawPage(p, m_doc, lineWidth);
	p->setAntialiasing(true);
	p->restore();
}

void Canvas::DrawPageGuides(ScPainter *p, const QRectF& clip, bool master)
{
	if (!m_doc->guidesPrefs().guidesShown)
		return;

	if (master)
	{
		ScPage *page = m_doc->currentPage();
		double x = m_doc->scratch()->left() * m_viewMode.scale;
		double y = m_doc->scratch()->top() * m_viewMode.scale;
		double w = page->width() * m_viewMode.scale;
		double h = page->height() * m_viewMode.scale;
		QRectF drawRect = QRectF(x, y, w+5, h+5);
		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if (drawRect.intersects(clip))
			DrawPageGuidesSub(p, page);
	}
	else
	{
		int docPagesCount = m_doc->Pages->count();
		for (int a = 0; a < docPagesCount; ++a)
		{
			ScPage *page = m_doc->Pages->at(a);
			QRectF drawRect = QRectF(page->xOffset() * m_viewMode.scale, page->yOffset() * m_viewMode.scale, page->width() * m_viewMode.scale + 5, page->height() * m_viewMode.scale + 5);
			drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
			if (drawRect.intersects(clip))
				DrawPageGuidesSub(p, page);
		}
	}
}

/**
  draws actual page indicator frame
 */
void Canvas::DrawPageIndicatorSub(ScPainter *p, ScPage *page)
{
	p->save();
	int fm = p->fillMode();
	p->setAntialiasing(false);
	p->translate(page->xOffset(), page->yOffset());
	double lineWidth = 1.0 / m_viewMode.scale;
	double pageHeight = page->height();
	double pageWidth = page->width();
	p->setFillMode(ScPainter::None);
	p->setStrokeMode(ScPainter::Solid);
	p->setPen(Qt::black, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setPen(PrefsManager::instance().appPrefs.displayPrefs.pageBorderColor, 1 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->drawRect(0, 0, pageWidth, pageHeight);
	p->setAntialiasing(true);
	p->setFillMode(fm);
	p->restore();
}

void Canvas::DrawPageIndicator(ScPainter *p, const QRectF& clip, bool master)
{
	if (master)
	{
		ScPage *page = m_doc->currentPage();
		double x = m_doc->scratch()->left() * m_viewMode.scale;
		double y = m_doc->scratch()->top() * m_viewMode.scale;
		double w = page->width() * m_viewMode.scale;
		double h = page->height() * m_viewMode.scale;
		QRectF drawRect = QRectF(x, y, w+5, h+5);
		drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
		if (drawRect.intersects(clip))
			DrawPageIndicatorSub(p, page);
	}
	else
	{
		int docPagesCount = m_doc->Pages->count();
		for (int a = 0; a < docPagesCount; ++a)
		{
			ScPage *page = m_doc->Pages->at(a);
			if (page == m_doc->currentPage())
			{
				QRectF drawRect = QRectF(page->xOffset() * m_viewMode.scale, page->yOffset() * m_viewMode.scale, page->width() * m_viewMode.scale + 5, page->height() * m_viewMode.scale + 5);
				drawRect.translate(-m_doc->minCanvasCoordinate.x() * m_viewMode.scale, -m_doc->minCanvasCoordinate.y() * m_viewMode.scale);
				if (drawRect.intersects(clip))
					DrawPageIndicatorSub(p, page);
			}
		}
	}
}

/**
  draws the links between textframe chains.
  needs the list of visible textframes in m_viewMode.linkedFramesToShow
 */
void Canvas::drawFrameLinks(ScPainter* painter)
{
	PageItem *currItem = nullptr;
	if ((m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames))
	{
		if (m_doc->m_Selection->count() > 0)
			currItem = m_doc->m_Selection->itemAt(0);
		else if (m_viewMode.linkedFramesToShow.count() > 0)
			currItem = m_viewMode.linkedFramesToShow.at(0);
		if (currItem && (currItem->itemType() != PageItem::TextFrame))
			currItem = nullptr;
	}

	//Draw the frame links
	painter->save();
	if (m_doc->guidesPrefs().linkShown || m_viewMode.drawFramelinksWithContents)
	{
		for (int i = 0; i < m_viewMode.linkedFramesToShow.count(); ++i)
		{
			PageItem* nextItem = m_viewMode.linkedFramesToShow.at(i);
			while (nextItem != nullptr)
			{
				if (nextItem->nextInChain() != nullptr)
				{
					FPoint start, end;
					calculateFrameLinkPoints(nextItem, nextItem->nextInChain(), start, end);
					drawLinkFrameLine(painter, start, end);
				}
				nextItem = nextItem->nextInChain();
			}
		}
	}
	else if ((((m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames)) && (currItem != nullptr)))
	{
		PageItem *nextItem = currItem->firstInChain();
		while (nextItem != nullptr)
		{
			if (nextItem->nextInChain() != nullptr)
			{
				FPoint start, end;
				calculateFrameLinkPoints(nextItem, nextItem->nextInChain(), start, end);
				drawLinkFrameLine(painter, start, end);
			}
			nextItem = nextItem->nextInChain();
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
	Q_ASSERT(painter!=nullptr);
	painter->setPen(Qt::black, 1.0 / m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	painter->setPenOpacity(1.0);
	painter->drawLine(start, end);
	QTransform arrowTrans;
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
  this is a wrapper around repaint()
 */
void Canvas::PaintSizeRect(QRect newRect)
{
	static QRect oldRect;
	if (!newRect.isNull())
	{
		if (!oldRect.isNull())
			newRect = newRect.united(oldRect);
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
			newRect = newRect.united(oldRect);
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
	p->translate(currItem->xPos()*m_viewMode.scale, currItem->yPos()*m_viewMode.scale);
	p->scale(m_viewMode.scale, m_viewMode.scale);
	p->rotate(currItem->rotation());
}

void Canvas::Transform(PageItem *currItem, QTransform& m)
{
	m.translate(currItem->xPos()*m_viewMode.scale, currItem->yPos()*m_viewMode.scale);
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
	if (pi1==nullptr || pi2==nullptr)
		return;
	//Calculate the link points of the frames
	double x11 = pi1->xPos();
	double y11 = pi1->yPos();
	double x12 = x11 + pi1->width();
	double y12 = y11 + pi1->height();
	if (pi1->isGroupChild())
	{
		QTransform itemTrans = pi1->getTransform();
		QPointF itPos = itemTrans.map(QPointF(0, 0));
		x11 = itPos.x();
		y11 = itPos.y();
		double grScXi = 1.0;
		double grScYi = 1.0;
		getScaleFromMatrix(itemTrans, grScXi, grScYi);
		if (itemTrans.m11() < 0)
			x11 -= pi1->width() * grScXi;
		if (itemTrans.m22() < 0)
			y11 -= pi1->height() * grScYi;
		x12 = x11 + pi1->width() * grScXi;
		y12 = y11 + pi1->height() * grScYi;
	}
	double x1mid = x11 + (x12 - x11) / 2.0;
	double y1mid = y11 + (y12 - y11) / 2.0;
					
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
	double x22 = x21 + pi2->width();
	double y22 = y21 + pi2->height();
	if (pi2->isGroupChild())
	{
		QTransform itemTrans = pi2->getTransform();
		QPointF itPos = itemTrans.map(QPointF(0, 0));
		x21 = itPos.x();
		y21 = itPos.y();
		double grScXi = 1.0;
		double grScYi = 1.0;
		getScaleFromMatrix(itemTrans, grScXi, grScYi);
		if (itemTrans.m11() < 0)
			x21 -= pi2->width() * grScXi;
		if (itemTrans.m22() < 0)
			y11 -= pi2->height() * grScYi;
		x22 = x21 + pi2->width() * grScXi;
		y22 = y21 + pi2->height() * grScYi;
	}

	double x2mid = x21 + (x22 - x21) / 2.0;
	double y2mid = y21 + (y22 - y21) / 2.0;
					
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
	
	//When our points (in pt) are exactly the same, cover this too. #3634
	if (x11==x21) { a1 = x1mid; a2 = x2mid; }
	if (y11==y21) { b1 = y1mid; b2 = y2mid; }
	
	//Set the link frame lines' endpoints
	start.setXY(a1-pi1->xPos(), b1-pi1->yPos());
	start.transform(pi1->xPos(), pi1->yPos(), pi1->rotation(), 1, 1, false);
	end.setXY(a2-pi2->xPos(), b2-pi2->yPos());
	end.transform(pi2->xPos(), pi2->yPos(), pi2->rotation(), 1, 1, false);
}

QPixmap Canvas::createPixmap(double w, double h)
{
	QPixmap p(w * devicePixelRatio(), h * devicePixelRatio());
	p.setDevicePixelRatio(devicePixelRatio());
	return p;
}

void Canvas::drawPixmap(QPainter& painter, double x, double y, const QPixmap& pixmap, double sx, double sy, double sw, double sh)
{
	sx *= devicePixelRatio();
	sy *= devicePixelRatio();
	sw *= devicePixelRatio();
	sh *= devicePixelRatio();
	painter.drawPixmap(x, y, pixmap, sx, sy, sw, sh);
}

void Canvas::displayXYHUD(QPoint m)
{
	if (!PrefsManager::instance().appPrefs.displayPrefs.showMouseCoordinates)
		return;
	double gx, gy, gh, gw, r;
	if (m_doc->m_Selection->isMultipleSelection())
	{
		m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		r = 0.0;
	}
	else
	{
		PageItem* currItem = m_doc->m_Selection->itemAt(0);
		gx = currItem->xPos();
		gy = currItem->yPos();
		gw = currItem->width();
		gh = currItem->height();
		r = currItem->rotation();
	}
	QTransform ma;
	ma.translate(gx, gy);
	ma.rotate(r);
	FPoint n;
	if (m_doc->rotationMode() == 0)
		n = FPoint(0.0, 0.0);
	else if (m_doc->rotationMode() == 1)
		n = FPoint(gw, 0.0);
	else if (m_doc->rotationMode() == 2)
		n = FPoint(gw / 2.0, gh / 2.0);
	else if (m_doc->rotationMode() == 3)
		n = FPoint(0.0, gh);
	else if (m_doc->rotationMode() == 4)
		n = FPoint(gw, gh);
	gx = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
	gy = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
	if (m_doc->guidesPrefs().rulerMode)
	{
		gx -= m_doc->currentPage()->xOffset();
		gy -= m_doc->currentPage()->yOffset();
	}
	gx -= m_doc->rulerXoffset;
	gy -= m_doc->rulerYoffset;
	displayXYHUD(m, gx, gy);
}

void Canvas::displayCorrectedXYHUD(QPoint m, double x, double y)
{
	if (!PrefsManager::instance().appPrefs.displayPrefs.showMouseCoordinates)
		return;
	double gx = x;
	double gy = y;
	if (m_doc->guidesPrefs().rulerMode)
	{
		gx -= m_doc->currentPage()->xOffset();
		gy -= m_doc->currentPage()->yOffset();
	}
	gx -= m_doc->rulerXoffset;
	gy -= m_doc->rulerYoffset;
	QToolTip::showText(m + QPoint(5, 5), tr("X: %1\nY: %2").arg(value2String(gx, m_doc->unitIndex(), true, true), value2String(gy, m_doc->unitIndex(), true, true)), this);
}

void Canvas::displayCorrectedSingleHUD(QPoint m, double val, bool isX)
{
	if (!PrefsManager::instance().appPrefs.displayPrefs.showMouseCoordinates)
		return;
	double gx = val;
	if (isX)
	{
		if (m_doc->guidesPrefs().rulerMode)
			gx -= m_doc->currentPage()->xOffset();
		gx -= m_doc->rulerXoffset;
		QToolTip::showText(m + QPoint(5, 5), tr("X: %1").arg(value2String(gx, m_doc->unitIndex(), true, true)), this);
	}
	else
	{
		if (m_doc->guidesPrefs().rulerMode)
			gx -= m_doc->currentPage()->yOffset();
		gx -= m_doc->rulerYoffset;
		QToolTip::showText(m + QPoint(5, 5), tr("Y: %1").arg(value2String(gx, m_doc->unitIndex(), true, true)), this);
	}
}

void Canvas::displayXYHUD(QPoint m, double x, double y)
{
	if (!PrefsManager::instance().appPrefs.displayPrefs.showMouseCoordinates)
		return;
	QToolTip::showText(m + QPoint(5, 5), tr("X: %1\nY: %2").arg(value2String(x, m_doc->unitIndex(), true, true), value2String(y, m_doc->unitIndex(), true, true)), this);
}

void Canvas::displaySizeHUD(QPoint m, double x, double y, bool isLine)
{
	if (!PrefsManager::instance().appPrefs.displayPrefs.showMouseCoordinates)
		return;
	if (isLine)
		QToolTip::showText(m + QPoint(5, 5), tr("Length: %1\nAngle: %2").arg(value2String(x, m_doc->unitIndex(), true, true), value2String(y, SC_DEGREES, true, true)), this);
	else
		QToolTip::showText(m + QPoint(5, 5), tr("Width: %1\nHeight: %2").arg(value2String(x, m_doc->unitIndex(), true, true), value2String(y, m_doc->unitIndex(), true, true)), this);
}

void Canvas::displayRotHUD(QPoint m, double rot)
{
	if (!PrefsManager::instance().appPrefs.displayPrefs.showMouseCoordinates)
		return;
	double r;
	if (rot < 0.0)
		r = rot * -1.0;
	else
		r = 360.0 - rot;
	QToolTip::showText(m + QPoint(5, 5), tr("Angle: %1").arg(value2String(r, SC_DEGREES, true, true)), this);
}

void Canvas::displayRealRotHUD(QPoint m, double rot)
{
	if (!PrefsManager::instance().appPrefs.displayPrefs.showMouseCoordinates)
		return;
	QToolTip::showText(m + QPoint(5, 5), tr("Angle: %1").arg(value2String(rot, SC_DEGREES, true, true)), this);
}

void Canvas::displayDoubleHUD(QPoint point, const QString& label, double value)
{
	if (!PrefsManager::instance().appPrefs.displayPrefs.showMouseCoordinates)
		return;
	QToolTip::showText(point + QPoint(5, 5), QString("%1: %2").arg(label, value2String(value, m_doc->unitIndex(), true, true)), this);
}

void Canvas::setupEditHRuler(PageItem * item, bool forceAndReset)
{
	static QString rulerItemRef;
	static double rulerDumbHash(0.0);
	
	if ((rulerItemRef != item->itemName()) 
		   || forceAndReset )
	{
		rulerItemRef = item->itemName();
		rulerDumbHash = 0.0;
	}
	
	double controlHash(0.0);
	controlHash = item->xPos() 
			+ item->yPos()				* 1.0
			+ item->m_columnGap 				* 2.0
			+ item->m_columns 				* 3.0
			+ item->textToFrameDistLeft()		* 4.0 
			+ item->textToFrameDistRight()		* 5.0
			+ item->currentStyle().firstIndent()	* 6.0
			+ item->currentStyle().leftMargin()	* 7.0
			+ item->width()				* 8.0
			+ item->currentStyle().rightMargin()	* 9.0
			+ (item->imageFlippedH() ? 32.32 : 13.13);
	
	const ParagraphStyle& currParaStyle = item->currentStyle();
	for (const ParagraphStyle::TabRecord& tabrec : currParaStyle.tabValues())
	{
		controlHash += tabrec.tabPosition;
	}
// 	qDebug()<<"Canvas::setupEditHRuler"<<rulerItemRef<<controlHash<<rulerDumbHash;
	if (controlHash == rulerDumbHash)
		return;
	
	rulerDumbHash = controlHash;
	m_view->horizRuler->setItem(item);
	m_view->horizRuler->update();
}
