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
#include <QButtonGroup>
#include <QCheckBox>
#include <QCursor>
#include <QEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPoint>
#include <QRect>
#include <QTimer>
#include <QWidgetAction>
#include <QDebug>

#include "ui/aligndistribute.h"
#include "canvas.h"
#include "ui/contextmenu.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hyphenator.h"
#include "pageitem_spiral.h"
#include "ui/pageselector.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "ui/propertiespalette.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribusXml.h"
#include "selection.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"

CanvasMode_EditSpiral::CanvasMode_EditSpiral(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	Mxp = Myp = -1;
	m_arcPoint = noPointDefined;
}

inline bool CanvasMode_EditSpiral::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0);
	return (*pi) != NULL;
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
	QPen p1b = QPen(Qt::blue, 1.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	QPen p1bd = QPen(Qt::blue, 1.0 / m_canvas->m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
	QPen p8b = QPen(Qt::blue, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p8r = QPen(Qt::red, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	psx->translate(static_cast<int>(currItem->xPos()), static_cast<int>(currItem->yPos()));
	psx->rotate(currItem->rotation());
	psx->setPen(p1b);
	psx->setBrush(Qt::NoBrush);
	psx->setPen(p8b);
	if (m_arcPoint == useControlStart)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(startPoint);
	if (m_arcPoint == useControlEnd)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(endPoint);
}

void CanvasMode_EditSpiral::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_EditSpiral::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void CanvasMode_EditSpiral::activate(bool fromGesture)
{
	VectorDialog = new SpiralVectorDialog(m_ScMW);
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	Mxp = Myp = -1;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_Spiral* item = currItem->asSpiral();
//	centerPoint = currItem->PoLine.pointQF(0);
	startPoint = currItem->PoLine.pointQF(0);
	endPoint = currItem->PoLine.pointQF(currItem->PoLine.size() - 2);
	startAngle = item->spiralStartAngle;
	endAngle = item->spiralEndAngle;
	VectorDialog->setValues(startAngle, endAngle, item->height(), item->width(), item->spiralFactor);
	VectorDialog->show();
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
	connect(VectorDialog, SIGNAL(NewVectors(double, double, double, double, double)), this, SLOT(applyValues(double, double, double, double, double)));
	connect(VectorDialog, SIGNAL(endEdit()), this, SLOT(endEditing()));
	connect(VectorDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
	connect(m_doc, SIGNAL(updateEditItem()), this, SLOT(updateFromItem()));
	connect(m_ScMW->propertiesPalette, SIGNAL(updateEditItem()), this, SLOT(updateFromItem()));
}

void CanvasMode_EditSpiral::updateFromItem()
{
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_Spiral* item = currItem->asSpiral();
	centerPoint = currItem->PoLine.pointQF(0);
	startPoint = currItem->PoLine.pointQF(0);
	endPoint = currItem->PoLine.pointQF(currItem->PoLine.size() - 2);
	startAngle = item->spiralStartAngle;
	endAngle = item->spiralEndAngle;
	VectorDialog->setValues(startAngle, endAngle, item->height(), item->width(), item->spiralFactor);
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

void CanvasMode_EditSpiral::applyValues(double start, double end, double height, double width, double factor)
{
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_Spiral *item = currItem->asSpiral();
	QPointF mPoint = item->PoLine.pointQF(0);
	QRectF upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
//	QRectF upRect2 = QRectF(mPoint.x() - item->arcWidth / 2.0, mPoint.y() - item->arcHeight / 2.0, item->arcWidth, item->arcHeight);
//	upRect = upRect2.united(upRect);
	upRect.translate(currItem->xPos(), currItem->yPos());
	item->spiralStartAngle = start;
	item->spiralEndAngle = end;
	item->spiralFactor = factor;
/*	QTransform bb;
	bb.scale(item->spiralHeight / item->spiralWidth, 1.0);
	QLineF inp = QLineF(QPointF(width / 2.0, height / 2.0), QPointF(width, height / 2.0));
	inp.setAngle(start);
	QLineF res = bb.map(inp);
	inp.setAngle(end);
	QLineF ena = bb.map(inp);
	startAngle = res.angle();
	endAngle = ena.angle();
	double nSweep = endAngle - startAngle;
	if (nSweep < 0)
		nSweep += 360;
	QPainterPath pp;
	pp.moveTo(mPoint);
	pp.arcTo(QRectF(mPoint.x() - item->arcWidth / 2.0, mPoint.y() - item->arcHeight / 2.0, item->arcWidth, item->arcHeight), startAngle, nSweep);
	pp.closeSubpath();
	currItem->PoLine.fromQPainterPath(pp); */
	item->setWidthHeight(width, height);
	item->recalcPath();
//	item->arcStartAngle = startAngle;
//	item->arcSweepAngle = endAngle - startAngle;
	startPoint = currItem->PoLine.pointQF(0);
	endPoint = currItem->PoLine.pointQF(currItem->PoLine.size() - 2);
	startAngle = item->spiralStartAngle;
	endAngle = item->spiralEndAngle;
//	centerPoint = currItem->PoLine.pointQF(0);
//	widthPoint = QPointF(centerPoint.x() - item->arcWidth / 2.0, centerPoint.y());
//	heightPoint = QPointF(centerPoint.x(), centerPoint.y() - item->arcHeight / 2.0);
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
}

void CanvasMode_EditSpiral::deactivate(bool forGesture)
{
	disconnect(VectorDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
	VectorDialog->close();
	delete VectorDialog;
	m_view->redrawMarker->hide();
	m_arcPoint = noPointDefined;
	disconnect(m_doc, SIGNAL(updateEditItem()), this, SLOT(updateFromItem()));
	disconnect(m_ScMW->propertiesPalette, SIGNAL(updateEditItem()), this, SLOT(updateFromItem()));
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
		PageItem_Spiral *item = currItem->asSpiral();
		QTransform itemMatrix;
		itemMatrix.translate(currItem->xPos(), currItem->yPos());
		itemMatrix.rotate(currItem->rotation());
		QPointF sPoint;
		if (m_arcPoint == useControlStart)
			sPoint = getSegment(startAngle);
		else if (m_arcPoint == useControlEnd)
			sPoint = getSegment(endAngle);
		QPointF smPoint = itemMatrix.map(sPoint);
		QLineF stLinA = QLineF(smPoint, QPointF(Mxp, Myp));
		QLineF stLinM = QLineF(smPoint, QPointF(newX, newY));
		double deltaAngle = stLinM.angle() - stLinA.angle();
		if (deltaAngle < -180)
			deltaAngle = deltaAngle + 360;
		else if (deltaAngle > 180)
			deltaAngle = deltaAngle - 360;
		if (m_arcPoint == useControlStart)
		{
			if (startAngle + deltaAngle >= 0)
			{
				startAngle += deltaAngle;
				item->spiralStartAngle = startAngle;
				item->recalcPath();
				startPoint = currItem->PoLine.pointQF(0);
			}
			VectorDialog->setValues(startAngle, endAngle, item->height(), item->width(), item->spiralFactor);
		}
		else if (m_arcPoint == useControlEnd)
		{
			if (endAngle + deltaAngle > startAngle)
			{
				endAngle += deltaAngle;
				item->spiralEndAngle = endAngle;
				item->recalcPath();
				endPoint = currItem->PoLine.pointQF(currItem->PoLine.size() - 2);
			}
			VectorDialog->setValues(startAngle, endAngle, item->height(), item->width(), item->spiralFactor);
		}
		currItem->update();
		QRectF upRect;
		upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
		upRect.translate(currItem->xPos(), currItem->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
	}
	Mxp = newX;
	Myp = newY;
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
	Mxp = mousePointDoc.x(); //m->x();
	Myp = mousePointDoc.y(); //m->y();
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	QTransform itemMatrix;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	itemMatrix.translate(currItem->xPos(), currItem->yPos());
	itemMatrix.rotate(currItem->rotation());
	QPointF stPoint = startPoint;
	stPoint = itemMatrix.map(stPoint);
	QPointF swPoint = endPoint;
	swPoint = itemMatrix.map(swPoint);
	if (m_canvas->hitsCanvasPoint(m->globalPos(), stPoint))
		m_arcPoint = useControlStart;
	else if (m_canvas->hitsCanvasPoint(m->globalPos(), swPoint))
		m_arcPoint = useControlEnd;
	else
		m_arcPoint = noPointDefined;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
	QRectF upRect;
	upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
	upRect.translate(currItem->xPos(), currItem->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
}

void CanvasMode_EditSpiral::mouseReleaseEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	QRectF upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
}
