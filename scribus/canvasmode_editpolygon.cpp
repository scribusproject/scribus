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
                 canvasmode_editpolygon.cpp  -  description
                             -------------------
    begin                : Wed Jan 12 2011
    copyright            : (C) 2011 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/


#include "canvasmode_editpolygon.h"

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
#include "pageitem_regularpolygon.h"
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

CanvasMode_EditPolygon::CanvasMode_EditPolygon(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	Mxp = Myp = -1;
	m_polygonPoint = noPointDefined;
}

inline bool CanvasMode_EditPolygon::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0);
	return (*pi) != NULL;
}

void CanvasMode_EditPolygon::drawControls(QPainter* p)
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
	if (m_doc->appMode == modeEditPolygon)
	{
		drawControlsPolygon(p, m_doc->m_Selection->itemAt(0));
	}
	p->restore();
}

void CanvasMode_EditPolygon::drawControlsPolygon(QPainter* psx, PageItem* currItem)
{
	QPen p1b = QPen(Qt::blue, 1.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	QPen p1bd = QPen(Qt::red, 1.0 / m_canvas->m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
	QPen p8b = QPen(Qt::blue, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p8r = QPen(Qt::red, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	psx->translate(static_cast<int>(currItem->xPos()), static_cast<int>(currItem->yPos()));
	psx->rotate(currItem->rotation());
	psx->setPen(p1b);
	psx->setBrush(Qt::NoBrush);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	QPainterPath path = RegularPolygonPath(item->width(), item->height(), polyCorners, polyUseFactor, polyFactor, polyRotation, polyCurvature, polyInnerRot, polyOuterCurvature);
	psx->drawPath(path);
	psx->setPen(p1bd);
	psx->drawLine(startPoint, endPoint);
	psx->setPen(p8b);
	if (m_polygonPoint == useControlOuter)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(startPoint);
	if (m_polygonPoint == useControlInner)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(endPoint);
/*	if (m_arcPoint == useControlWidth)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(widthPoint);
	if (m_arcPoint == useControlHeight)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(heightPoint); */
}

void CanvasMode_EditPolygon::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_EditPolygon::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void CanvasMode_EditPolygon::activate(bool fromGesture)
{
	m_polygonPoint = noPointDefined;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	Mxp = Myp = -1;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	centerPoint = QPointF(currItem->width() / 2.0, currItem->height() / 2.0);
	startPoint = currItem->PoLine.pointQF(0);
	endPoint = currItem->PoLine.pointQF(3);
	polyCorners = item->polyCorners;
	polyUseFactor = item->polyUseFactor;
	polyFactor = item->polyFactor;
	polyRotation = item->polyRotation;
	polyCurvature = item->polyCurvature;
	polyInnerRot = item->polyInnerRot;
	polyOuterCurvature = item->polyOuterCurvature;
//	widthPoint = QPointF(centerPoint.x() - item->arcWidth / 2.0, centerPoint.y());
//	heightPoint = QPointF(centerPoint.x(), centerPoint.y() - item->arcHeight / 2.0);
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_EditPolygon::deactivate(bool forGesture)
{
	m_view->redrawMarker->hide();
	m_polygonPoint = noPointDefined;
}

double CanvasMode_EditPolygon::getUserValFromFactor(double factor)
{
	double userVal = 0.0;
	double mi = GetZeroFactor();
	double ma = GetMaxFactor();
	if ((factor / mi * 100.0 - 100.0) > 0)
		userVal = (factor - mi) / (ma - mi) * 100.0;
	else
		userVal = factor / mi * 100.0 - 100.0;
	return userVal;
}

double CanvasMode_EditPolygon::GetZeroFactor()
{
	return sqrt(pow(1.0,2.0)-pow(((sin((360.0/(polyCorners*2))/180* M_PI)* 2.0)/2.0),2.0));
}

double CanvasMode_EditPolygon::GetMaxFactor()
{
	double win = (360.0/(polyCorners*2)) / 180.0 * M_PI;
	double ret;
	if ((360.0/(polyCorners*2)) > 45)
		ret = 1/sin(win);
	else
		ret = 1/cos(win);
	return ret;
}

void CanvasMode_EditPolygon::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	PageItem *currItem = 0;
	if ((m_doc->m_Selection->isMultipleSelection()) || (m_doc->appMode != modeNormal))
	{
		if ((m_doc->m_Selection->isMultipleSelection()) && (m_doc->appMode == modeNormal))
		{
			if (GetItem(&currItem))
			{
				/* CB: old code, removing this as shift-alt select on an unselected table selects a cell now.
				//#6789 is closed by sorting this.
				if (currItem->isTableItem)
				{
					m_view->Deselect(false);
					m_doc->m_Selection->addItem(currItem);
					currItem->isSingleSel = true;
					//CB FIXME dont call this if the added item is item 0
					if (!m_doc->m_Selection->primarySelectionIs(currItem))
						currItem->emitAllToGUI();
					m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				}*/
			}
			return;
		}
		else
		{
			if (!(GetItem(&currItem) && (m_doc->appMode == modeEdit) && currItem->asTextFrame()))
			{
				mousePressEvent(m);
				return;
			}
		}
	}
}

void CanvasMode_EditPolygon::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m->accept();
	double newX = mousePointDoc.x();
	double newY = mousePointDoc.y();
	if (m_canvas->m_viewMode.m_MouseButtonPressed && m_view->moveTimerElapsed())
	{
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		QTransform itemMatrix;
		itemMatrix.translate(currItem->xPos(), currItem->yPos());
		itemMatrix.rotate(currItem->rotation());
		QPointF cPoint = itemMatrix.map(centerPoint);
		QLineF stLinA = QLineF(cPoint, QPointF(newX, newY));
		
		uint cx = polyUseFactor ? polyCorners * 2 : polyCorners;
		double seg = 360.0 / cx;
		
		if (m_polygonPoint == useControlInner)
		{
			polyInnerRot = stLinA.angle() - 90 - polyRotation - seg;
			double factor = stLinA.length() / sqrt(pow(sin(stLinA.angle() * M_PI / 180.0) * currItem->height() / 2.0, 2) + pow(cos(stLinA.angle() * M_PI / 180.0) * currItem->width() / 2.0, 2));
			int maxF = qRound(getUserValFromFactor(factor));
			if (maxF <= 100)
				polyFactor = factor;
		}
		QPainterPath path = RegularPolygonPath(currItem->width(), currItem->height(), polyCorners, polyUseFactor, polyFactor, polyRotation, polyCurvature, polyInnerRot, polyOuterCurvature);
		FPointArray ar;
		ar.fromQPainterPath(path);
		if (m_polygonPoint == useControlInner)
			endPoint = ar.pointQF(3);
/*		QPainterPath pp;
		if (m_arcPoint == useControlStart)
			startAngle += deltaAngle;
		else if (m_arcPoint == useControlSweep)
			endAngle += deltaAngle;
		else if (m_arcPoint == useControlHeight)
			heightPoint = QPointF(heightPoint.x(), heightPoint.y() + (newY - Myp));
		else if (m_arcPoint == useControlWidth)
			widthPoint = QPointF(widthPoint.x() + (newX - Mxp), widthPoint.y());
		double nSweep = endAngle - startAngle;
		double nWidth = sPoint.x() - widthPoint.x();
		double nHeight = sPoint.y() - heightPoint.y();
		pp.moveTo(sPoint);
		pp.arcTo(QRectF(sPoint.x() - nWidth, sPoint.y() - nHeight, nWidth * 2, nHeight * 2), startAngle, nSweep);
		pp.closeSubpath();
		FPointArray ar;
		ar.fromQPainterPath(pp);
		if (m_arcPoint == useControlStart)
			startPoint = ar.pointQF(3);
		else if (m_arcPoint == useControlSweep)
			endPoint = ar.pointQF(ar.size() - 4); */
		currItem->update();
		QRectF upRect;
		upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
		upRect.translate(currItem->xPos(), currItem->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
	}
	Mxp = newX;
	Myp = newY;
}

void CanvasMode_EditPolygon::mousePressEvent(QMouseEvent *m)
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
/*	QPointF shPoint = heightPoint;
	shPoint = itemMatrix.map(shPoint);
	QPointF sPoint = widthPoint;
	sPoint = itemMatrix.map(sPoint); */
	if (m_canvas->hitsCanvasPoint(m->globalPos(), stPoint))
		m_polygonPoint = useControlOuter;
	else if (m_canvas->hitsCanvasPoint(m->globalPos(), swPoint))
		m_polygonPoint = useControlInner;
/*	else if (m_canvas->hitsCanvasPoint(m->globalPos(), shPoint))
		m_arcPoint = useControlHeight;
	else if (m_canvas->hitsCanvasPoint(m->globalPos(), sPoint))
		m_arcPoint = useControlWidth; */
	else
		m_polygonPoint = noPointDefined;
	qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
	QRectF upRect;
	upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
	upRect.translate(currItem->xPos(), currItem->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
}

void CanvasMode_EditPolygon::mouseReleaseEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	if ((m_polygonPoint == useControlInner) || (m_polygonPoint == useControlOuter))
	{
		double newX = mousePointDoc.x();
		double newY = mousePointDoc.y();
		QTransform itemMatrix;
		itemMatrix.translate(currItem->xPos(), currItem->yPos());
		itemMatrix.rotate(currItem->rotation());
		QPointF cPoint = itemMatrix.map(centerPoint);
		QLineF stLinA = QLineF(cPoint, QPointF(newX, newY));
		uint cx = polyUseFactor ? polyCorners * 2 : polyCorners;
		double seg = 360.0 / cx;
		if (m_polygonPoint == useControlInner)
		{
			polyInnerRot = stLinA.angle() - 90 - polyRotation - seg;
			polyFactor = stLinA.length() / sqrt(pow(sin(stLinA.angle() * M_PI / 180.0) * currItem->height() / 2.0, 2) + pow(cos(stLinA.angle() * M_PI / 180.0) * currItem->width() / 2.0, 2));
		}
		item->polyFactor = polyFactor;
		item->polyRotation = polyRotation;
		item->polyCurvature = polyCurvature;
		item->polyInnerRot = polyInnerRot;
		item->polyOuterCurvature = polyOuterCurvature;
		item->recalcPath();
	}
	QRectF upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
	upRect.translate(currItem->xPos(), currItem->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
}
