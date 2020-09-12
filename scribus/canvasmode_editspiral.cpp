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
/***************************************************************************
                 canvasmode_editspiral.cpp  -  description
                             -------------------
    begin                : Wed Jan 27 2011
    copyright            : (C) 2011 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/


#include "canvasmode_editspiral.h"

#include <QApplication>
#include <QCursor>
#include <QEvent>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPoint>
#include <QRect>
#include <QTimer>
#include <QWidgetAction>

#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "pageitem_spiral.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "util.h"
#include "util_math.h"


CanvasMode_EditSpiral::CanvasMode_EditSpiral(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
}

inline bool CanvasMode_EditSpiral::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0);
	return (*pi) != nullptr;
}

void CanvasMode_EditSpiral::drawControls(QPainter* p)
{
	p->save();
	if (m_canvas->m_viewMode.operItemMoving)
	{
		drawOutline(p);
	}
	else
	{
		drawSelection(p, false);
	}
	if (m_doc->appMode == modeEditSpiral)
	{
		drawControlsSpiral(p, m_doc->m_Selection->itemAt(0));
	}
	p->restore();
}

void CanvasMode_EditSpiral::drawControlsSpiral(QPainter* psx, PageItem* currItem)
{
	QPen p8b = QPen(Qt::blue, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p8r = QPen(Qt::red, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	psx->setTransform(currItem->getTransform(), true);
	psx->setBrush(Qt::NoBrush);
	psx->setPen(p8b);
	if (m_arcPoint == useControlStart)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(m_startPoint);
	if (m_arcPoint == useControlEnd)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(m_endPoint);
}

void CanvasMode_EditSpiral::enterEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_EditSpiral::leaveEvent(QEvent *e)
{
}


void CanvasMode_EditSpiral::activate(bool fromGesture)
{
	CanvasMode::activate(fromGesture);

	m_VectorDialog = new SpiralVectorDialog(m_ScMW);
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	m_Mxp = m_Myp = -1;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_Spiral* item = currItem->asSpiral();
	m_startPoint = currItem->PoLine.pointQF(0);
	m_endPoint = currItem->PoLine.pointQF(currItem->PoLine.size() - 2);
	m_startAngle = item->spiralStartAngle;
	m_endAngle = item->spiralEndAngle;
	m_VectorDialog->setValues(computeRealAngle(m_startAngle, false), computeRealAngle(m_endAngle, false), item->spiralFactor);
	m_VectorDialog->show();
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
	connect(m_VectorDialog, SIGNAL(NewVectors(double, double, double)), this, SLOT(applyValues(double, double, double)));
	connect(m_VectorDialog, SIGNAL(endEdit()), this, SLOT(endEditing()));
	connect(m_VectorDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
	connect(m_doc, SIGNAL(docChanged()), this, SLOT(updateFromItem()));
}

void CanvasMode_EditSpiral::deactivate(bool forGesture)
{
	disconnect(m_VectorDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
	m_VectorDialog->close();
	m_VectorDialog->deleteLater();
	m_view->setRedrawMarkerShown(false);
	m_arcPoint = noPointDefined;
	disconnect(m_doc, SIGNAL(docChanged()), this, SLOT(updateFromItem()));

	CanvasMode::deactivate(forGesture);
}

void CanvasMode_EditSpiral::updateFromItem()
{
	if (updateFromItemBlocked())
		return;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_Spiral* item = currItem->asSpiral();
	m_startPoint = currItem->PoLine.pointQF(0);
	m_endPoint = currItem->PoLine.pointQF(currItem->PoLine.size() - 2);
	m_startAngle = item->spiralStartAngle;
	m_endAngle = item->spiralEndAngle;
	m_VectorDialog->setValues(computeRealAngle(m_startAngle, false), computeRealAngle(m_endAngle, false), item->spiralFactor);
	m_view->update();
}

void CanvasMode_EditSpiral::endEditing(bool active)
{
	if (!active)
		endEditing();
}

void CanvasMode_EditSpiral::endEditing()
{
	m_view->requestMode(modeNormal);
}

QPointF CanvasMode_EditSpiral::getSegment(double angle)
{
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_Spiral *item = currItem->asSpiral();
	double ww = item->width();
	double wws = 0.0;
	double wwn = ww;
	double hh = item->height() - (item->height() / (item->spiralFactor + 1.0));
	double segStart = 0.0;
	double segEnd = 180;
	bool segPart = true;
	QPointF ret = QPointF(item->width() / 2.0, item->height() / 2.0);
	if (angle < 0)
		return ret;
	while (true)
	{
		QLineF lin = QLineF(QPointF(wws, hh), QPointF(wwn, hh));
		if ((angle <= segEnd) && (angle >= segStart))
		{
			ret = lin.pointAt(0.5);
			break;
		}
		ww /= item->spiralFactor;
		wws = wwn;
		if (segPart)
			wwn -= ww;
		else
			wwn += ww;
		segPart = !segPart;
		segStart += 180.0;
		segEnd += 180.0;
	}
	return ret;
}

double CanvasMode_EditSpiral::computeRealAngle(double angle, bool fromDia)
{
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	double ret = angle;
	int rev = static_cast<int>(angle / 360.0);
	double part = angle - (rev * 360);
	QTransform bb;
	bb.scale(currItem->width() / currItem->height(), 1.0);
	QLineF inp = QLineF(QPointF(currItem->width() / 2.0, currItem->height() / 2.0), QPointF(currItem->width(), currItem->height() / 2.0));
	inp.setAngle(part);
	if (fromDia)
	{
		QLineF res = bb.map(inp);
		ret = res.angle();
	}
	else
	{
		QTransform bt = bb.inverted();
		QLineF res = bt.map(inp);
		ret = res.angle();
	}
	ret += rev * 360;
	return ret;
}

void CanvasMode_EditSpiral::applyValues(double start, double end, double factor)
{
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_Spiral *item = currItem->asSpiral();
	SimpleState *ss = new SimpleState(Um::EditSpiral, "", Um::IPolygon);
	ss->set("SPIRAL");
	ss->set("OLD_START",item->spiralStartAngle);
	ss->set("OLD_END",item->spiralEndAngle);
	ss->set("OLD_FACTOR",item->spiralFactor);
	item->spiralStartAngle = computeRealAngle(start, true);
	item->spiralEndAngle = computeRealAngle(end, true);
	item->spiralFactor = factor;
	ss->set("NEW_START",item->spiralStartAngle);
	ss->set("NEW_END",item->spiralEndAngle);
	ss->set("NEW_FACTOR",item->spiralFactor);
	undoManager->action(item,ss);
	item->recalcPath();
	m_startPoint = currItem->PoLine.pointQF(0);
	m_endPoint = currItem->PoLine.pointQF(currItem->PoLine.size() - 2);
	m_startAngle = item->spiralStartAngle;
	m_endAngle = item->spiralEndAngle;
	QTransform itemMatrix = currItem->getTransform();
	m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, currItem->width(), currItem->height())).adjusted(-5, -5, 10, 10));
}

void CanvasMode_EditSpiral::keyPressEvent(QKeyEvent *e)
{
	commonkeyPressEvent_Default(e);
}

void CanvasMode_EditSpiral::keyReleaseEvent(QKeyEvent *e)
{
	commonkeyReleaseEvent(e);
}

void CanvasMode_EditSpiral::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_view->requestMode(modeNormal);
}

void CanvasMode_EditSpiral::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m->accept();
	double newX = mousePointDoc.x();
	double newY = mousePointDoc.y();
	if (m_canvas->m_viewMode.m_MouseButtonPressed && m_view->moveTimerElapsed() && (m_arcPoint != noPointDefined))
	{
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		QTransform itemMatrix = currItem->getTransform();
		PageItem_Spiral *item = currItem->asSpiral();
		QPointF sPoint;
		if (m_arcPoint == useControlStart)
			sPoint = getSegment(m_startAngle);
		else if (m_arcPoint == useControlEnd)
			sPoint = getSegment(m_endAngle);
		QPointF smPoint = itemMatrix.map(sPoint);
		QLineF stLinA = QLineF(smPoint, QPointF(m_Mxp, m_Myp));
		QLineF stLinM = QLineF(smPoint, QPointF(newX, newY));
		double deltaAngle = stLinM.angle() - stLinA.angle();
		if (deltaAngle < -180)
			deltaAngle = deltaAngle + 360;
		else if (deltaAngle > 180)
			deltaAngle = deltaAngle - 360;
		if (currItem->imageFlippedV())
			deltaAngle *= -1.0;
		if (currItem->imageFlippedH())
			deltaAngle *= -1.0;
		if (m_arcPoint == useControlStart)
		{
			if (m_startAngle + deltaAngle >= 0)
			{
				m_startAngle += deltaAngle;
				applyValues(m_startAngle,m_endAngle,item->spiralFactor);
				item->recalcPath();
				m_startPoint = currItem->PoLine.pointQF(0);
				m_canvas->displayRealRotHUD(m->globalPos(), m_startAngle);
			}
			m_VectorDialog->setValues(computeRealAngle(m_startAngle, false), computeRealAngle(m_endAngle, false), item->spiralFactor);
		}
		else if (m_arcPoint == useControlEnd)
		{
			if (m_endAngle + deltaAngle > m_startAngle)
			{
				m_endAngle += deltaAngle;
				applyValues(m_startAngle,m_endAngle,item->spiralFactor);
				item->recalcPath();
				m_endPoint = currItem->PoLine.pointQF(currItem->PoLine.size() - 2);
				m_canvas->displayRealRotHUD(m->globalPos(), m_endAngle);
			}
			m_VectorDialog->setValues(computeRealAngle(m_startAngle, false), computeRealAngle(m_endAngle, false), item->spiralFactor);
		}
		currItem->update();
		m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, currItem->width(), currItem->height())).adjusted(-5, -5, 10, 10));
	}
	m_Mxp = newX;
	m_Myp = newY;
}

void CanvasMode_EditSpiral::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvas->PaintSizeRect(QRect());
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	m_Mxp = mousePointDoc.x(); //m->x();
	m_Myp = mousePointDoc.y(); //m->y();
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	QTransform itemMatrix = currItem->getTransform();
	QPointF stPoint = m_startPoint;
	stPoint = itemMatrix.map(stPoint);
	QPointF swPoint = m_endPoint;
	swPoint = itemMatrix.map(swPoint);
	if (m_canvas->hitsCanvasPoint(m->globalPos(), stPoint))
		m_arcPoint = useControlStart;
	else if (m_canvas->hitsCanvasPoint(m->globalPos(), swPoint))
		m_arcPoint = useControlEnd;
	else
		m_arcPoint = noPointDefined;
	if (m_arcPoint != noPointDefined)
		m_transaction = undoManager->beginTransaction(Um::Polygon, Um::IPolygon, Um::EditSpiral, "", Um::IPolygon);
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_view->setCursor(QCursor(Qt::CrossCursor));
	m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, currItem->width(), currItem->height())).adjusted(-5, -5, 10, 10));
}

void CanvasMode_EditSpiral::mouseReleaseEvent(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	if (m_transaction)
	{
		m_transaction.commit();
		m_transaction.reset();
	}
	QTransform itemMatrix = currItem->getTransform();
	m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, currItem->width(), currItem->height())).adjusted(-5, -5, 10, 10));
}
