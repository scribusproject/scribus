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

#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "pageitem_regularpolygon.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/aligndistribute.h"
#include "ui/contextmenu.h"
#include "ui/pageselector.h"
#include "ui/polygonwidget.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"


CanvasMode_EditPolygon::CanvasMode_EditPolygon(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	m_Mxp = m_Myp = -1;
	m_blockUpdateFromItem = 0;
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
		drawOutline(p);
	else
		drawSelection(p, false);
	if (m_doc->appMode == modeEditPolygon)
		drawControlsPolygon(p, m_doc->m_Selection->itemAt(0));
	p->restore();
}

void CanvasMode_EditPolygon::drawControlsPolygon(QPainter* psx, PageItem* currItem)
{
	QPen p1b = QPen(Qt::blue, 1.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	QPen p1bd = QPen(Qt::red, 1.0 / m_canvas->m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
	QPen p8b = QPen(Qt::blue, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p8r = QPen(Qt::red, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	psx->setTransform(currItem->getTransform(), true);
	psx->setPen(p1b);
	psx->setBrush(Qt::NoBrush);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	QPainterPath path = RegularPolygonPath(item->width(), item->height(), m_polyCorners, m_polyUseFactor, m_polyFactor, m_polyRotation, m_polyCurvature, m_polyInnerRot, m_polyOuterCurvature);
	psx->drawPath(path);
	psx->setPen(p1bd);
	psx->drawLine(m_startPoint, m_endPoint);
	if (m_polyUseFactor)
	{
		psx->drawLine(m_endPoint, m_innerCPoint);
		psx->drawLine(m_startPoint, m_outerCPoint);
	}
	psx->setPen(p8b);
	if (m_polygonPoint == useControlOuter)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(m_startPoint);
	if (m_polygonPoint == useControlInner)
		psx->setPen(p8r);
	else
		psx->setPen(p8b);
	psx->drawPoint(m_endPoint);
	if (m_polyUseFactor)
	{
		if (m_polygonPoint == useControlInnerCurve)
			psx->setPen(p8r);
		else
			psx->setPen(p8b);
		psx->drawPoint(m_innerCPoint);
		if (m_polygonPoint == useControlOuterCurve)
			psx->setPen(p8r);
		else
			psx->setPen(p8b);
		psx->drawPoint(m_outerCPoint);
	}
}

void CanvasMode_EditPolygon::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		setModeCursor();
}

void CanvasMode_EditPolygon::leaveEvent(QEvent *e)
{
}

void CanvasMode_EditPolygon::updateFromItem()
{
	if (updateFromItemBlocked())
		return;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	m_centerPoint = QPointF(currItem->width() / 2.0, currItem->height() / 2.0);
	m_startPoint = currItem->PoLine.pointQF(0);
	m_endPoint = currItem->PoLine.pointQF(2);
	m_polyCorners = item->polyCorners;
	m_polyUseFactor = item->polyUseFactor;
	m_polyFactor = item->polyFactor;
	m_polyRotation = item->polyRotation;
	m_polyCurvature = item->polyCurvature;
	m_polyInnerRot = item->polyInnerRot;
	m_polyOuterCurvature = item->polyOuterCurvature;
	m_VectorDialog->polyWidget->blockSignals(true);
	m_VectorDialog->setValues(m_polyCorners, m_polyFactor, m_polyUseFactor, m_polyRotation, m_polyCurvature, m_polyInnerRot, m_polyOuterCurvature);
	m_VectorDialog->polyWidget->blockSignals(false);
	uint cx = m_polyUseFactor ? m_polyCorners * 2 : m_polyCorners;
	double seg = 360.0 / cx;
	double trueLength = sqrt(pow(sin(seg / 180.0 * M_PI) * (item->width() / 2.0), 2) + pow(cos(seg / 180.0 * M_PI) * (item->height() / 2.0) + (item->height()/2.0) - item->height(), 2));
	QLineF innerLine = QLineF(m_endPoint, m_centerPoint);
	innerLine.setAngle(innerLine.angle() + 90);
	innerLine.setLength(trueLength * m_polyCurvature);
	m_innerCPoint = innerLine.p2();
	QLineF outerLine = QLineF(m_startPoint, currItem->PoLine.pointQF(6));
	outerLine.setLength(outerLine.length() * m_polyOuterCurvature);
	m_outerCPoint = outerLine.p2();
	m_view->update();
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
	m_Mxp = m_Myp = -1;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	m_centerPoint = QPointF(currItem->width() / 2.0, currItem->height() / 2.0);
	m_startPoint = currItem->PoLine.pointQF(0);
	m_endPoint = currItem->PoLine.pointQF(2);
	m_polyCorners = item->polyCorners;
	m_polyUseFactor = item->polyUseFactor;
	m_polyFactor = item->polyFactor;
	m_polyRotation = item->polyRotation;
	m_polyCurvature = item->polyCurvature;
	m_polyInnerRot = item->polyInnerRot;
	m_polyOuterCurvature = item->polyOuterCurvature;
	m_VectorDialog = new PolyVectorDialog(m_ScMW, m_polyCorners, m_polyFactor, m_polyUseFactor, m_polyRotation, m_polyCurvature, m_polyInnerRot, m_polyOuterCurvature);
	m_VectorDialog->show();
	uint cx = m_polyUseFactor ? m_polyCorners * 2 : m_polyCorners;
	double seg = 360.0 / cx;
	double trueLength = sqrt(pow(sin(seg / 180.0 * M_PI) * (item->width() / 2.0), 2) + pow(cos(seg / 180.0 * M_PI) * (item->height() / 2.0) + (item->height()/2.0) - item->height(), 2));
	QLineF innerLine = QLineF(m_endPoint, m_centerPoint);
	innerLine.setAngle(innerLine.angle() + 90);
	innerLine.setLength(trueLength * m_polyCurvature);
	m_innerCPoint = innerLine.p2();
	QLineF outerLine = QLineF(m_startPoint, currItem->PoLine.pointQF(6));
	outerLine.setLength(outerLine.length() * m_polyOuterCurvature);
	m_outerCPoint = outerLine.p2();
	setModeCursor();
	if (fromGesture)
		m_view->update();
	connect(m_doc, SIGNAL(docChanged()), this, SLOT(updateFromItem()));
	
	connect(m_VectorDialog, SIGNAL(NewVectors(int, double, bool, double, double, double, double)), this, SLOT(applyValues(int, double, bool, double, double, double, double)));
	connect(m_VectorDialog, SIGNAL(endEdit()), this, SLOT(endEditing()));
	connect(m_VectorDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
}

void CanvasMode_EditPolygon::deactivate(bool forGesture)
{
	disconnect(m_VectorDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
	m_VectorDialog->close();
	m_VectorDialog->deleteLater();
	m_view->setRedrawMarkerShown(false);
	m_polygonPoint = noPointDefined;
	disconnect(m_doc, SIGNAL(docChanged()), this, SLOT(updateFromItem()));
}

void CanvasMode_EditPolygon::endEditing(bool active)
{
	if (!active)
		endEditing();
}

void CanvasMode_EditPolygon::endEditing()
{
	m_view->requestMode(modeNormal);
}

void CanvasMode_EditPolygon::applyValues(int polyC, double polyF, bool polyUseCF, double polyR, double polyCur, double polyIRot, double polyOCur)
{
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	QRectF oldRect = item->getBoundingRect();
	m_polyCorners = polyC;
	m_polyFactor = polyF;
	m_polyRotation = polyR;
	m_polyCurvature = polyCur;
	m_polyInnerRot = polyIRot;
	m_polyOuterCurvature = polyOCur;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::EditPolygon,"",Um::IPolygon);
		ss->set("POLYGON");
		ss->set("NEW_CORNER",polyC);
		ss->set("NEW_USEFACTOR",polyUseCF);
		ss->set("NEW_FACTOR",m_polyFactor);
		ss->set("NEW_ROTATION",m_polyRotation);
		ss->set("NEW_CURV",m_polyCurvature);
		ss->set("NEW_INNER",m_polyInnerRot);
		ss->set("NEW_OUTER",m_polyOuterCurvature);
		ss->set("OLD_CORNER",item->polyCorners);
		ss->set("OLD_USEFACTOR",item->polyUseFactor);
		ss->set("OLD_FACTOR",item->polyFactor);
		ss->set("OLD_ROTATION",item->polyRotation);
		ss->set("OLD_CURV",item->polyCurvature);
		ss->set("OLD_INNER",item->polyInnerRot);
		ss->set("OLD_OUTER",item->polyOuterCurvature);
		undoManager->action(currItem,ss);
	}
	item->polyCorners = polyC;
	item->polyUseFactor = polyUseCF;
	item->polyFactor = m_polyFactor;
	item->polyRotation = m_polyRotation;
	item->polyCurvature = m_polyCurvature;
	item->polyInnerRot = m_polyInnerRot;
	item->polyOuterCurvature = m_polyOuterCurvature;
	item->recalcPath();
	updateFromItem();
	QTransform itemMatrix = currItem->getTransform();
	QPainterPath path = itemMatrix.map(RegularPolygonPath(item->width(), item->height(), m_polyCorners, m_polyUseFactor, m_polyFactor, m_polyRotation, m_polyCurvature, m_polyInnerRot, m_polyOuterCurvature));
	QRectF updateRect = oldRect.united(path.boundingRect());
	m_doc->regionsChanged()->update(updateRect.adjusted(-5, -5, 10, 10));
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
	return sqrt(pow(1.0,2.0)-pow(((sin((360.0/(m_polyCorners*2))/180* M_PI)* 2.0)/2.0),2.0));
}

double CanvasMode_EditPolygon::GetMaxFactor()
{
	double win = (360.0/(m_polyCorners*2)) / 180.0 * M_PI;
	double ret;
	if ((360.0/(m_polyCorners*2)) > 45)
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
	m_view->requestMode(modeNormal);
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
		QTransform itemMatrix = currItem->getTransform();
		QPointF cPoint = itemMatrix.map(m_centerPoint);
		QLineF stLinA = QLineF(cPoint, QPointF(newX, newY));
		
		uint cx = m_polyUseFactor ? m_polyCorners * 2 : m_polyCorners;
		double seg = 360.0 / cx;
		double trueLength = sqrt(pow(sin(seg / 180.0 * M_PI) * (currItem->width() / 2.0), 2) + pow(cos(seg / 180.0 * M_PI) * (currItem->height() / 2.0) + (currItem->height()/2.0) - currItem->height(), 2));
		
		if (m_polygonPoint == useControlInner)
		{
			m_polyInnerRot = stLinA.angle() - 90 - m_polyRotation - seg;
			double factor = stLinA.length() / sqrt(pow(sin(stLinA.angle() * M_PI / 180.0) * currItem->height() / 2.0, 2) + pow(cos(stLinA.angle() * M_PI / 180.0) * currItem->width() / 2.0, 2));
			int maxF = qRound(getUserValFromFactor(factor));
			if (maxF <= 100)
				m_polyFactor = factor;
		}
		if (m_polygonPoint == useControlOuter)
		{
			m_polyRotation = stLinA.angle() - 90;
			if (m_polyRotation < -180)
				m_polyRotation += 360;
			if (m_polyRotation > 180)
				m_polyRotation -= 360;
		}
		if (m_polygonPoint == useControlInnerCurve)
		{
			QPointF ePoint = itemMatrix.map(m_endPoint);
			QLineF stLinC = QLineF(ePoint, QPointF(newX, newY));
			m_polyCurvature = stLinC.length() / trueLength;
		}
		if (m_polygonPoint == useControlOuterCurve)
		{
			QPointF sPoint = itemMatrix.map(m_startPoint);
			QPointF sPoint2 = itemMatrix.map(currItem->PoLine.pointQF(6));
			QLineF stLinCo = QLineF(sPoint, QPointF(newX, newY));
			QLineF stLinCo2 = QLineF(sPoint, sPoint2);
			m_polyOuterCurvature = stLinCo.length() / stLinCo2.length();
		}
		QPainterPath path = RegularPolygonPath(currItem->width(), currItem->height(), m_polyCorners, m_polyUseFactor, m_polyFactor, m_polyRotation, m_polyCurvature, m_polyInnerRot, m_polyOuterCurvature);
		FPointArray ar;
		ar.fromQPainterPath(path);
		m_endPoint = ar.pointQF(2);
		m_startPoint = ar.pointQF(0);
		QLineF innerLine = QLineF(m_endPoint, m_centerPoint);
		innerLine.setAngle(innerLine.angle() + 90);
		innerLine.setLength(trueLength * m_polyCurvature);
		m_innerCPoint = innerLine.p2();
		QLineF outerLine = QLineF(m_startPoint, ar.pointQF(6));
		outerLine.setLength(outerLine.length() * m_polyOuterCurvature);
		m_outerCPoint = outerLine.p2();
		m_VectorDialog->setValues(m_polyCorners, m_polyFactor, m_polyUseFactor, m_polyRotation, m_polyCurvature, m_polyInnerRot, m_polyOuterCurvature);
		blockUpdateFromItem(true);
		currItem->update();
		blockUpdateFromItem(false);
		path = itemMatrix.map(path);
		m_doc->regionsChanged()->update(path.boundingRect().adjusted(-5, -5, 10, 10));
	}
	m_Mxp = newX;
	m_Myp = newY;
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
	QPointF shPoint = m_innerCPoint;
	shPoint = itemMatrix.map(shPoint);
	QPointF sPoint = m_outerCPoint;
	sPoint = itemMatrix.map(sPoint);
	bool useOuter = m_canvas->hitsCanvasPoint(m->globalPos(), stPoint);
	bool useInner = m_canvas->hitsCanvasPoint(m->globalPos(), swPoint);
	bool useInnerC = m_canvas->hitsCanvasPoint(m->globalPos(), shPoint);
	bool useOuterC = m_canvas->hitsCanvasPoint(m->globalPos(), sPoint);
	if (useOuter && useOuterC)
	{
		if (m->modifiers() == Qt::ShiftModifier)
			m_polygonPoint = useControlOuterCurve;
		else
			m_polygonPoint = useControlOuter;
	}
	else if (useOuter || useOuterC)
	{
		if (useOuterC)
			m_polygonPoint = useControlOuterCurve;
		else
			m_polygonPoint = useControlOuter;
	}
	else if (useInner && useInnerC)
	{
		if (m->modifiers() == Qt::ShiftModifier)
			m_polygonPoint = useControlInnerCurve;
		else
			m_polygonPoint = useControlInner;
	}
	else if (useInner || useInnerC)
	{
		if (useInnerC)
			m_polygonPoint = useControlInnerCurve;
		else
			m_polygonPoint = useControlInner;
	}
	else
		m_polygonPoint = noPointDefined;
	if (m_polygonPoint != noPointDefined && UndoManager::undoEnabled())
		m_transaction = undoManager->beginTransaction(Um::Polygon, Um::IPolygon, Um::EditPolygon, "", Um::IPolygon);
	m_view->setCursor(QCursor(Qt::CrossCursor));
	QPainterPath path = itemMatrix.map(RegularPolygonPath(currItem->width(), currItem->height(), m_polyCorners, m_polyUseFactor, m_polyFactor, m_polyRotation, m_polyCurvature, m_polyInnerRot, m_polyOuterCurvature));
	m_doc->regionsChanged()->update(path.boundingRect().adjusted(-5, -5, 10, 10));
}

void CanvasMode_EditPolygon::mouseReleaseEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	QTransform itemMatrix = currItem->getTransform();
	if ((m_polygonPoint == useControlInner) || (m_polygonPoint == useControlOuter) || (m_polygonPoint == useControlInnerCurve) || (m_polygonPoint == useControlOuterCurve))
	{
		double newX = mousePointDoc.x();
		double newY = mousePointDoc.y();
		QPointF cPoint = itemMatrix.map(m_centerPoint);
		QLineF stLinA = QLineF(cPoint, QPointF(newX, newY));
		uint cx = m_polyUseFactor ? m_polyCorners * 2 : m_polyCorners;
		double seg = 360.0 / cx;
		double trueLength = sqrt(pow(sin(seg / 180.0 * M_PI) * (currItem->width() / 2.0), 2) + pow(cos(seg / 180.0 * M_PI) * (currItem->height() / 2.0) + (currItem->height()/2.0) - currItem->height(), 2));
		if (m_polygonPoint == useControlInner)
		{
			m_polyInnerRot = stLinA.angle() - 90 - m_polyRotation - seg;
			m_polyFactor = stLinA.length() / sqrt(pow(sin(stLinA.angle() * M_PI / 180.0) * currItem->height() / 2.0, 2) + pow(cos(stLinA.angle() * M_PI / 180.0) * currItem->width() / 2.0, 2));
		}
		if (m_polygonPoint == useControlOuter)
			m_polyRotation = stLinA.angle() - 90;
		if (m_polygonPoint == useControlInnerCurve)
		{
			QPointF ePoint = itemMatrix.map(m_endPoint);
			QLineF stLinC = QLineF(ePoint, QPointF(newX, newY));
			m_polyCurvature = stLinC.length() / trueLength;
		}
		if (m_polygonPoint == useControlOuterCurve)
		{
			QPointF sPoint = itemMatrix.map(m_startPoint);
			QPointF sPoint2 = itemMatrix.map(currItem->PoLine.pointQF(6));
			QLineF stLinCo = QLineF(sPoint, QPointF(newX, newY));
			QLineF stLinCo2 = QLineF(sPoint, sPoint2);
			m_polyOuterCurvature = stLinCo.length() / stLinCo2.length();
		}
		item->polyFactor = m_polyFactor;
		item->polyRotation = m_polyRotation;
		item->polyCurvature = m_polyCurvature;
		item->polyInnerRot = m_polyInnerRot;
		item->polyOuterCurvature = m_polyOuterCurvature;
		item->recalcPath();
		m_VectorDialog->setValues(m_polyCorners, m_polyFactor, m_polyUseFactor, m_polyRotation, m_polyCurvature, m_polyInnerRot, m_polyOuterCurvature);
		if (m_transaction)
		{
			m_transaction.commit();
			m_transaction.reset();
		}
	}
	QPainterPath path = itemMatrix.map(RegularPolygonPath(item->width(), item->height(), m_polyCorners, m_polyUseFactor, m_polyFactor, m_polyRotation, m_polyCurvature, m_polyInnerRot, m_polyOuterCurvature));
	m_doc->regionsChanged()->update(path.boundingRect().adjusted(-5, -5, 10, 10));
}
