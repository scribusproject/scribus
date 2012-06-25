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

#include "canvasgesture_resize.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QRubberBand>

#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"
#include "pageitem_arc.h"
#include "pageitem_spiral.h"
#include "pageitem_table.h"
#include "util_math.h"

ResizeGesture::ResizeGesture (CanvasMode* parent) : CanvasGesture(parent)
{
	m_transactionStarted = NULL;
}

void ResizeGesture::prepare(Canvas::FrameHandle framehandle)
{
	if (framehandle > 0)
		m_handle = framehandle;
	
	if (m_doc->m_Selection->count() == 0)
	{
		m_handle = Canvas::OUTSIDE;
		return;
	}
	else if (m_doc->m_Selection->isMultipleSelection())
	{
		double ex, ey, eh, ew;
		m_doc->m_Selection->getGroupRect(&ex, &ey, &ew, &eh);
		double gx, gy, gh, gw;
		m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
		m_bounds = QRectF(QPointF(gx,gy), QSizeF(gw, gh));
		m_rotation = 0.0;
		m_extraX = ex - gx;
		m_extraY = ey - gy;
		m_extraWidth = gw - ew;
		m_extraHeight = gh - eh;
	}
	else // we keep m_bounds non-rotated
	{
		PageItem* currItem = m_doc->m_Selection->itemAt(0);
		m_bounds = QRectF(-currItem->visualLineWidth() / 2.0, -currItem->visualLineWidth() / 2.0, currItem->visualWidth(), currItem->visualHeight());
		QTransform mm = currItem->getTransform();
		QPointF itPos = mm.map(QPointF(0, 0));
		double m_scaleX, m_scaleY;
		getScaleFromMatrix(mm, m_scaleX, m_scaleY);
		QTransform m;
		m.scale(m_scaleX, m_scaleY);
		m_bounds = m.mapRect(m_bounds);
		m_bounds.moveTopLeft(itPos);
		if (mm.m11() < 0)
		{
			m_rotation = -getRotationDFromMatrix(mm) - 180.0;
			m_bounds.translate(-currItem->visualWidth() * m_scaleX, 0);
		}
		else
			m_rotation = -getRotationDFromMatrix(mm);
		if (mm.m22() < 0)
		{
			m_bounds.translate(0, -currItem->visualHeight() * m_scaleY);
		}
		currItem->OldB2 = currItem->width();
		currItem->OldH2 = currItem->height();
		m_extraWidth = currItem->visualWidth() - currItem->width();
		m_extraHeight = currItem->visualHeight() - currItem->height();
		m_extraX =  m_extraY = m_extraWidth / 2.0;
	}
	m_origRatio = m_bounds.width() / m_bounds.height();
	m_origBounds = m_bounds;
}


void ResizeGesture::clear()
{
	m_handle = Canvas::OUTSIDE;
	if (m_transactionStarted)
	{
		m_transactionStarted->cancel();
		delete m_transactionStarted;
		m_transactionStarted = NULL;
	}
}

void ResizeGesture::activate(bool flag)
{
}



void ResizeGesture::deactivate(bool forgesture)
{
	if (!forgesture)
		clear();
}



void ResizeGesture::drawControls(QPainter* p) 
{
	QColor drawColor = qApp->palette().color(QPalette::Active, QPalette::Highlight);
	QRect localRect = m_canvas->canvasToLocal(m_bounds.normalized());
	p->save();
	if (m_rotation != 0)
	{
		p->setRenderHint(QPainter::Antialiasing);
		p->translate(localRect.x(), localRect.y());
		p->rotate(m_rotation);
		p->translate(-localRect.x(), -localRect.y());
	}
	p->setPen(QPen(drawColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	drawColor.setAlpha(64);
	p->setBrush(drawColor);
	p->drawRect(localRect);

	if (m_origBounds != m_bounds)
	{
		if (m_doc->m_Selection->count() > 0)
		{
			p->setBrush(Qt::NoBrush);
			QPen out = QPen(Qt::gray, 1.0 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			out.setCosmetic(true);
			p->setPen(out);
			PageItem *currItem = m_doc->m_Selection->itemAt(0);
			QTransform m;
			m.translate(localRect.x(), localRect.y());
			m.scale(localRect.width() / currItem->width(), localRect.height() / currItem->height());
			QTransform mm = currItem->getTransform();
			if (mm.m11() < 0)
			{
				m.translate(currItem->width(), 0);
				m.scale(-1, 1);
			}
			if (mm.m22() < 0)
			{
				m.translate(0, currItem->height());
				m.scale(1, -1);
			}
			QPolygon clip = m.map(currItem->Clip);
			currItem->DrawPolyL(p, clip);
		}
	}
	p->restore();
}



void ResizeGesture::mouseReleaseEvent(QMouseEvent *m)
{
	adjustBounds(m);
	if (m_doc->m_Selection->count() != 0)
	{
		PageItem* currItem = m_doc->m_Selection->itemAt(0);
		if (m_bounds != m_mousePressBounds)
		{
			doResize(m->modifiers() & Qt::AltModifier);
			m_doc->setRedrawBounding(currItem);
			if (currItem->asImageFrame())
				currItem->AdjustPictScale();
		}
		//add action itemResizeToMargin for right click on item`s handlers
		else if (m->button() == Qt::RightButton)
		{
			const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
			Canvas::FrameHandle fh = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
			m_doc->itemResizeToMargin(currItem, fh);
		}
		m_view->resetMousePressed();
		// necessary since mousebutton is still recorded pressed, and otherwise checkchanges() will do nothing
		// we must check changes on whole selection otherwise resize operation won't undo correctly on groups
		if (m_bounds != m_mousePressBounds)
		{
			for (int i = 0; i < m_doc->m_Selection->count(); ++i)
				m_doc->m_Selection->itemAt(i)->checkChanges();
			m_doc->invalidateRegion(m_mousePressBounds.unite(m_bounds));
			m_doc->regionsChanged()->update(m_mousePressBounds.unite(m_bounds));
			m_doc->changed();
		}
	}
	if (m_transactionStarted)
	{
		m_transactionStarted->commit();
		delete m_transactionStarted;
		m_transactionStarted = NULL;
	}
	m->accept();
	m_canvas->update();
	m_view->stopGesture();
}


void ResizeGesture::doResize(bool scaleContent)
{
	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	QString targetName = Um::SelectionGroup;
	QPixmap* targetIcon = Um::IGroup;
	if (!m_doc->m_Selection->isMultipleSelection())
	{
		targetName = currItem->getUName();
		targetIcon = currItem->getUPixmap();
	}
	if (!m_transactionStarted)
		m_transactionStarted = new UndoTransaction(Um::instance()->beginTransaction(targetName, targetIcon, Um::Resize, "", Um::IResize));
	QRectF newBounds = m_bounds.normalized();
	double dw = (newBounds.width() - m_extraWidth) - currItem->width();
	double dh = (newBounds.height() - m_extraHeight) - currItem->height();
	double dsch = 1.0;
	double dscw = 1.0;
	if (currItem->isArc())
	{
		PageItem_Arc* item = currItem->asArc();
		if (currItem->height() != 0.0)
			dsch = item->arcHeight / currItem->height();
		if (currItem->width() != 0.0)
			dscw = item->arcWidth / currItem->width();
	}
	if (m_doc->m_Selection->isMultipleSelection())
	{
		int RotModeBack = m_doc->RotMode();
		m_doc->RotMode ( 0 );
		double gx, gy, gh, gw;
		m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		QRectF oldBounds(gx, gy, gw, gh);
		double scx = oldBounds.width() == 0? 1.0 : (newBounds.width() - m_extraWidth) / oldBounds.width();
		double scy = oldBounds.height() == 0? 1.0 : (newBounds.height() - m_extraHeight) / oldBounds.height();
		//CB #3012 only scale text in a group if alt is pressed
		if ((currItem->itemType() == PageItem::TextFrame) && scaleContent)
			m_doc->scaleGroup(scx, scy, true);
		else
			m_doc->scaleGroup(scx, scy, false);
		double dx = newBounds.x() - oldBounds.x();
		double dy = newBounds.y() - oldBounds.y();
		if (dx != 0 || dy != 0)
			m_doc->moveGroup(dx + m_extraX, dy + m_extraY);
		m_doc->RotMode ( RotModeBack );
	}
	else
	{
		if (currItem->itemType() == PageItem::ImageFrame && scaleContent)
		{
			double divX = (currItem->width() != 0) ? currItem->width() : 1.0;
			double divY = (currItem->height() != 0) ? currItem->height() : 1.0;
			double imgScX = (newBounds.width() - m_extraWidth) / divX * currItem->imageXScale();
			double imgScY = (newBounds.height() - m_extraHeight) / divY * currItem->imageYScale();
			// The aspect ratio has been fixed, so make the modification in the direction of the larger movement.
			if (currItem->keepAspectRatio() && currItem->fitImageToFrame()) 
			{
				if (qAbs((newBounds.width() - m_extraWidth) - currItem->width()) > qAbs((newBounds.height() - m_extraHeight) - currItem->height()))
					imgScY = imgScX;
				else
					imgScX = imgScY;
			}
			currItem->setImageXYScale(imgScX, imgScY);
		}
		else if (currItem->itemType() == PageItem::ImageFrame && currItem->PictureIsAvailable)
		{
			QTransform mm = currItem->getTransform();
			QPointF itPos = mm.map(QPointF(0, 0));
			double dx = ((newBounds.x() + m_extraX) - itPos.x());
			double dy = ((newBounds.y() + m_extraY) - itPos.y());
			if (currItem->Parent != 0)
			{
				double sx, sy;
				getScaleFromMatrix(mm, sx, sy);
				dx /= sx;
				dy /= sy;
			}
			double cosa = cos(currItem->rotation() * M_PI / 180.0);
			double sina = sin(currItem->rotation() * M_PI / 180.0);
			double xoff = -(cosa*dx + sina*dy);
			if (currItem->imageFlippedH())
				xoff += (currItem->width() - (newBounds.width() - m_extraWidth));
			double yoff = -(cosa*dy - sina*dx);
			if (currItem->imageFlippedV())
				yoff += (currItem->height() - (newBounds.height() - m_extraHeight));
			if (xoff != 0.0 || yoff != 0.0)
			{
				currItem->moveImageInFrame(xoff / currItem->imageXScale(), yoff / currItem->imageYScale());
			}
		}
		// We do not want to scale the text of a linked frame
		// as it would alter text in other frames of the string
		else if((currItem->itemType() == PageItem::TextFrame) 
				       && (currItem->nextInChain() == 0) 
				       && (currItem->prevInChain() == 0) 
				       && scaleContent)
		{
			double divX = (currItem->width() != 0) ? currItem->width() : 1.0;
			double divY = (currItem->height() != 0) ? currItem->height() : 1.0;
			double txtScX = (newBounds.width() - m_extraWidth) / divX;
			double txtScY = (newBounds.height() - m_extraHeight) / divY;
			if (currItem->itemText.length() != 0)
			{
				for (int aa = 0; aa < currItem->itemText.length(); ++aa)
				{
#if 0 // hard to decide if it’s batter to scale or to change font size
					currItem->itemText.item(aa)->setScaleV(
							qMax(qMin(qRound(currItem->itemText.item(aa)->scaleV()*txtScY), 4000), 100));
					currItem->itemText.item(aa)->setScaleH(
							qMax(qMin(qRound(currItem->itemText.item(aa)->scaleH() * txtScX), 4000), 100));
#else
					currItem->itemText.item(aa)->setFontSize(
							qMax(qMin(currItem->itemText.item(aa)->fontSize() * txtScY, 4000.0), 1.0));
					currItem->itemText.item(aa)->setScaleH(
							qMax(qMin(qRound(currItem->itemText.item(aa)->scaleH() * txtScX / txtScY), 4000), 100));
#endif

					// We need to scale the linespacing _only once_ per paragraph.
					if((aa == 0) 
						|| ( SpecialChars::isBreak(currItem->itemText.itemText(aa - 1).at(0))))
					{
						ParagraphStyle ps(currItem->itemText.paragraphStyle(aa));
						double oldLS(currItem->itemText.paragraphStyle(aa).lineSpacing());
						ps.setLineSpacing(qMax(qRound(oldLS * txtScY), 1));
						currItem->itemText.setStyle(aa,ps);
					}
				}
			}
		}
		QTransform mm = currItem->getTransform();
		QPointF itPos = mm.map(QPointF(0, 0));
		double m_scaleX, m_scaleY;
		getScaleFromMatrix(mm, m_scaleX, m_scaleY);
		double dx = (itPos.x() - newBounds.x()) / m_scaleX;
		double dy = (itPos.y() - newBounds.y()) / m_scaleY;
		if (mm.m11() < 0)
			dx *= -1;
		if (mm.m22() < 0)
			dy *= -1;
		currItem->moveBy(-dx, -dy, true);
		currItem->setWidth(newBounds.width() / m_scaleX - m_extraWidth);
		currItem->setHeight(newBounds.height() / m_scaleY - m_extraHeight);
		if (mm.m11() < 0)
			currItem->moveBy(-currItem->width(), 0);
		if (mm.m22() < 0)
			currItem->moveBy(0, -currItem->height());
		currItem->updateClip();
		if (currItem->isArc())
		{
			PageItem_Arc* item = currItem->asArc();
			item->arcWidth += dw * dscw;
			item->arcHeight += dh * dsch;
			item->recalcPath();
			FPoint tp2(getMinClipF(&currItem->PoLine));
			currItem->PoLine.translate(-tp2.x(), -tp2.y());
			m_doc->AdjustItemSize(currItem);
		}
		if (currItem->isSpiral())
		{
			PageItem_Spiral* item = currItem->asSpiral();
			item->recalcPath();
		}
		if (currItem->isTable())
			currItem->asTable()->adjustTable();
		// rotation does not change
	}
	m_origBounds = m_bounds;
}


void ResizeGesture::mouseMoveEvent(QMouseEvent *m)
{
	adjustBounds(m);
	FPoint where = m_canvas->globalToCanvas(m->globalPos());
	if (// directly after a create the shape of an item isnt really usable, so we fix here
		m_origBounds.width() < 20 || m_origBounds.height() < 20 ||
		// and if we resize outside of the current canvas dimensions, we need to enlarge it
		where.x() < m_doc->minCanvasCoordinate.x() || where.y() < m_doc->minCanvasCoordinate.y() ||
		where.x() > m_doc->maxCanvasCoordinate.x() || where.y() > m_doc->maxCanvasCoordinate.y())
	{
		doResize(m->modifiers() & Qt::AltModifier);
	}
	m->accept();
	QPoint point = m->globalPos() - (m_canvas->mapToParent(QPoint(0, 0)) + m_canvas->parentWidget()->mapToGlobal(QPoint(0, 0)));
	m_view->ensureVisible(point.x(), point.y(), 20, 20);
	m_canvas->repaint();
	QRectF newBounds = m_bounds.normalized();
	m_canvas->displaySizeHUD(m->globalPos(), newBounds.width() - m_extraWidth, newBounds.height() - m_extraHeight);
}



void ResizeGesture::adjustBounds(QMouseEvent *m)
{
	QTransform rotation;
	FPoint docPoint = m_canvas->globalToCanvas(m->globalPos());
	QPointF oldXY = m_bounds.topLeft();
	// proportional resize
	bool constrainRatio = ((m->modifiers() & Qt::ControlModifier) != Qt::NoModifier);
	
	if (m_mousePressPoint == m->globalPos())
	{
		m_bounds = m_mousePressBounds;
		return;
	}

	// snap to grid	+ snap to guides
	bool isCorner = m_handle == Canvas::NORTHWEST || m_handle == Canvas::NORTHEAST 
		|| m_handle == Canvas::SOUTHWEST || m_handle == Canvas::SOUTHEAST;
	if (m_rotation == 0 || isCorner)
	{
		FPoint snappedPoint = m_doc->ApplyGridF(docPoint);
		double x = snappedPoint.x(), y = snappedPoint.y();
		m_doc->ApplyGuides(&x, &y);
//		if (m_doc->ApplyGuides(&x, &y))
//			qDebug() << "guides applied:" << snappedPoint.x() << snappedPoint.y() << "to" << x << y;
		if (m_handle == Canvas::NORTH || m_handle == Canvas::SOUTH) 
			// only snap on y-axis
			docPoint = FPoint(docPoint.x(), y);
		else if (m_handle == Canvas::EAST || m_handle == Canvas::WEST)
			// only snap on x-axis
			docPoint = FPoint(x, docPoint.y());
		else 
			docPoint = FPoint(x,y);
//		qDebug() << "resize snap grid/guides:" << m->globalPos() << "-->" << m_canvas->canvasToGlobal(docPoint);
	}
	
	// un-rotate point
	if (m_rotation != 0)
	{
		// rotate point around item position
		rotation.translate(m_bounds.x(), m_bounds.y());
		rotation.rotate(m_rotation);
		rotation.translate(-m_bounds.x(), -m_bounds.y());
//		qDebug() << "resize rotated" << m_rotation << "°" << m_bounds << rotation << ":" << point-globalBounds.topLeft() << "-->" << rotation.map(point)-globalBounds.topLeft();
		QPointF qp = QPointF(docPoint.x(), docPoint.y());
		qp = rotation.inverted().map(qp);
		docPoint = FPoint(qp.x(), qp.y());
	}

	// adjust bounds vertically
	switch (m_handle)
	{
		case Canvas::NORTHWEST:
		case Canvas::NORTH:
		case Canvas::NORTHEAST:
//			qDebug() << "ResizeGesture: top to" << point.y();
			m_bounds.setTop(docPoint.y());
			break;
		case Canvas::SOUTHWEST:
		case Canvas::SOUTH:
		case Canvas::SOUTHEAST:
//			qDebug() << "ResizeGesture: bottom to" << point.y();
			m_bounds.setBottom(docPoint.y());
			break;
		default:
			break;
	}
	// adjust bounds horizontally
	switch (m_handle)
	{
		case Canvas::NORTHWEST:
		case Canvas::WEST:
		case Canvas::SOUTHWEST:
//			qDebug() << "ResizeGesture: left to" << point.x();
			m_bounds.setLeft(docPoint.x());
			break;
		case Canvas::NORTHEAST:
		case Canvas::EAST:
		case Canvas::SOUTHEAST:
//			qDebug() << "ResizeGesture: right to" << point.x();
			m_bounds.setRight(docPoint.x());
			break;
		default:
			break;
	}

	// constrain ratio
	double newRatio = double(m_bounds.width()) / double(m_bounds.height());
	if (constrainRatio && m_origRatio != newRatio)
	{
//		qDebug() << "constrain ratio:" << m_bounds << newRatio << "to" << m_origRatio; 
		int newWidth = qRound(m_bounds.height() * m_origRatio);
		int newHeight = qRound(m_bounds.width() / m_origRatio);
		switch (m_handle)
		{
			case Canvas::NORTHWEST:
				// axis: topleft + t*[origRatio, 1]    t:= y-top
				//       [x',y] = [left, top] + [(y-top)*origRatio, y-top]
				//              = [left + (y-top)*origRatio, y]
				// x < x'  => mouse is WEST, x > x'  => mouse is NORTH
				// x < left + (y-top)*origratio   <=> x - left < (y - top) * origratio
				
//				qDebug() << "NORTHWEST" << point << m_origBounds.topLeft() << m_origRatio
//				<< (point.x() - m_origBounds.left() < (point.y()-m_origBounds.top()) * m_origRatio);
				
				if (docPoint.x() - m_origBounds.left() < (docPoint.y()-m_origBounds.top()) * m_origRatio)
					m_bounds.setTop(m_bounds.top() - newHeight + m_bounds.height());
				else
					m_bounds.setLeft(m_bounds.left() - newWidth + m_bounds.width());
				break;
			case Canvas::SOUTHWEST:
				// axis: bottomleft + t*[origRatio, -1]    t:= bottom-y
				//       (x',y) = [left, bottom] + [(bottom-y)*origRatio, -bottom+y]
				//              = [left + (bottom-y)*origRatio, y]
				// x < x'  => mouse is WEST, x > x'  => mouse is SOUTH
				// x < left + (bottom-y)*origratio   <=> x - left < (bottom-y) * origratio
				
//				qDebug() << "SOUTHWEST" << point << m_origBounds.bottomLeft()  << m_origRatio
//				<< (point.x() - m_origBounds.left() < (m_origBounds.bottom() - point.y()) * m_origRatio);
				
				if (docPoint.x() - m_origBounds.left() < (m_origBounds.bottom() - docPoint.y()) * m_origRatio)
					m_bounds.setHeight(newHeight);
				else
					m_bounds.setLeft(m_bounds.left() - newWidth + m_bounds.width());
				break;
			case Canvas::NORTHEAST:
				// cf. SOUTHWEST
				if (docPoint.x() - m_origBounds.left() > (m_origBounds.bottom() - docPoint.y()) * m_origRatio)
					m_bounds.setTop(m_bounds.top() - newHeight + m_bounds.height());
				else
					m_bounds.setWidth(newWidth);
				break;
			case Canvas::SOUTHEAST:
				// cf. NORTHWEST
				if (docPoint.x() - m_origBounds.left() > (docPoint.y()-m_origBounds.top()) * m_origRatio)
					m_bounds.setHeight(newHeight);
				else
					m_bounds.setWidth(newWidth);
				break;
			case Canvas::WEST:
			case Canvas::EAST:
				// (origBounds.top + origBounds.bottom) / 2 is the horizontal axis
				// keep that fixed
				m_bounds.setTop(((m_origBounds.top() + m_origBounds.bottom()) / 2) - newHeight / 2);
				m_bounds.setHeight(newHeight);
				break;
			case Canvas::NORTH:
			case Canvas::SOUTH:
				// (origBounds.left + origBounds.right) / 2 is the vertical axis
				// keep that fixed
				m_bounds.setLeft(((m_origBounds.left() + m_origBounds.right()) / 2) - newWidth / 2);
				m_bounds.setWidth(newWidth);
				break;
			default:
				break;
		}
//		qDebug() << "constrained:" << m_bounds << double(m_bounds.width()) / m_bounds.height();
	}

	// re-rotate: if top left has changed, then it needs rotation
	if (m_rotation != 0 && oldXY != m_bounds.topLeft())
	{
		m_bounds.moveTo(rotation.map(m_bounds.topLeft()));
		// fix opposite corner to avoid aggregating rounding errors
		QPointF origFixPoint, newFixPoint;
		switch (m_handle)
		{
			case Canvas::NORTHWEST:
				origFixPoint = m_origBounds.bottomRight();
				newFixPoint = m_bounds.bottomRight();
				break;
			case Canvas::WEST:
				origFixPoint = m_origBounds.topRight() + QPointF(0, m_origBounds.height()/2);
				newFixPoint = m_bounds.topRight() + QPointF(0, m_bounds.height()/2);
				break;
			case Canvas::SOUTHWEST:
				origFixPoint = m_origBounds.topRight();
				newFixPoint = m_bounds.topRight();
				break;
			case Canvas::SOUTH:
				origFixPoint = m_origBounds.topLeft() + QPointF(m_origBounds.width()/2, 0);
				newFixPoint = m_bounds.topLeft() + QPointF(m_bounds.width()/2, 0);
				break;
			case Canvas::SOUTHEAST:
				origFixPoint = m_origBounds.topLeft();
				newFixPoint = m_bounds.topLeft();
				break;
			case Canvas::EAST:
				origFixPoint = m_origBounds.topLeft() + QPointF(0, m_origBounds.height()/2);
				newFixPoint = m_bounds.topLeft() + QPointF(0, m_bounds.height()/2);
				break;
			case Canvas::NORTHEAST:
				origFixPoint = m_origBounds.bottomLeft();
				newFixPoint = m_bounds.bottomLeft();
				break;
			case Canvas::NORTH:
				origFixPoint = m_origBounds.bottomLeft() + QPointF(m_origBounds.width()/2, 0);
				newFixPoint = m_bounds.bottomLeft() + QPointF(m_bounds.width()/2, 0);
				break;
			default:
				origFixPoint = m_origBounds.topLeft();
				newFixPoint = m_bounds.topLeft();
				break;
		}
		origFixPoint = m_origBounds.topLeft() + rotation.map(origFixPoint - m_origBounds.topLeft());
		newFixPoint = m_bounds.topLeft() + rotation.map(newFixPoint - m_bounds.topLeft());
		if (origFixPoint != newFixPoint)
			m_bounds.translate(origFixPoint - newFixPoint);
	}
}



void ResizeGesture::mousePressEvent(QMouseEvent *m)
{
	FPoint point = m_canvas->globalToCanvas(m->globalPos());
	m_mousePressPoint = m->globalPos();
	if (m_doc->m_Selection->count() == 0)
	{
		m_handle = Canvas::OUTSIDE;
	}
	else if (m_doc->m_Selection->isMultipleSelection())
	{
		double gx, gy, gh, gw;
		m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
		m_handle = m_canvas->frameHitTest(QPointF(point.x(), point.y()), QRectF(gx,gy,gw,gh));
	}
	else
	{
		m_handle = m_canvas->frameHitTest(QPointF(point.x(), point.y()), m_doc->m_Selection->itemAt(0));
		PageItem* currItem = m_doc->m_Selection->itemAt(0);
		QTransform mm = currItem->getTransform();
		if (mm.m11() < 0)
		{
			switch (m_handle)
			{
				case Canvas::NORTHEAST:
					m_handle = Canvas::NORTHWEST;
					break;
				case Canvas::NORTHWEST:
					m_handle = Canvas::NORTHEAST;
					break;
				case Canvas::EAST:
					m_handle = Canvas::WEST;
					break;
				case Canvas::WEST:
					m_handle = Canvas::EAST;
					break;
				case Canvas::SOUTHEAST:
					m_handle = Canvas::SOUTHWEST;
					break;
				case Canvas::SOUTHWEST:
					m_handle = Canvas::SOUTHEAST;
					break;
				default:
					break;
			}
		}
		if (mm.m22() < 0)
		{
			switch (m_handle)
			{
				case Canvas::NORTHEAST:
					m_handle = Canvas::SOUTHEAST;
					break;
				case Canvas::NORTHWEST:
					m_handle = Canvas::SOUTHWEST;
					break;
				case Canvas::SOUTH:
					m_handle = Canvas::NORTH;
					break;
				case Canvas::NORTH:
					m_handle = Canvas::SOUTH;
					break;
				case Canvas::SOUTHEAST:
					m_handle = Canvas::SOUTHWEST;
					break;
				case Canvas::SOUTHWEST:
					m_handle = Canvas::NORTHWEST;
					break;
				default:
					break;
			}
		}
	}
	if (m_handle > 0)
	{
		prepare(m_handle);
		m->accept();
		m_mousePressBounds = m_bounds;
	}
}
