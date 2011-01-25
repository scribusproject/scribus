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
	psx->translate(static_cast<int>(currItem->xPos()), static_cast<int>(currItem->yPos()));
	psx->rotate(currItem->rotation());
	psx->setPen(p1b);
	psx->setBrush(Qt::NoBrush);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	QPainterPath path = RegularPolygonPath(item->width(), item->height(), polyCorners, polyUseFactor, polyFactor, polyRotation, polyCurvature, polyInnerRot, polyOuterCurvature);
	psx->drawPath(path);
	psx->setPen(p1bd);
	psx->drawLine(startPoint, endPoint);
	if (polyUseFactor)
	{
		psx->drawLine(endPoint, innerCPoint);
		psx->drawLine(startPoint, outerCPoint);
	}
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
	if (polyUseFactor)
	{
		if (m_polygonPoint == useControlInnerCurve)
			psx->setPen(p8r);
		else
			psx->setPen(p8b);
		psx->drawPoint(innerCPoint);
		if (m_polygonPoint == useControlOuterCurve)
			psx->setPen(p8r);
		else
			psx->setPen(p8b);
		psx->drawPoint(outerCPoint);
	}
}

void CanvasMode_EditPolygon::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		setModeCursor();
}

void CanvasMode_EditPolygon::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void CanvasMode_EditPolygon::updateFromItem()
{
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	centerPoint = QPointF(currItem->width() / 2.0, currItem->height() / 2.0);
	startPoint = currItem->PoLine.pointQF(0);
	endPoint = currItem->PoLine.pointQF(2);
	polyCorners = item->polyCorners;
	polyUseFactor = item->polyUseFactor;
	polyFactor = item->polyFactor;
	polyRotation = item->polyRotation;
	polyCurvature = item->polyCurvature;
	polyInnerRot = item->polyInnerRot;
	polyOuterCurvature = item->polyOuterCurvature;
	uint cx = polyUseFactor ? polyCorners * 2 : polyCorners;
	double seg = 360.0 / cx;
	double trueLength = sqrt(pow(sin(seg / 180.0 * M_PI) * (item->width() / 2.0), 2) + pow(cos(seg / 180.0 * M_PI) * (item->height() / 2.0) + (item->height()/2.0) - item->height(), 2));
	QLineF innerLine = QLineF(endPoint, centerPoint);
	innerLine.setAngle(innerLine.angle() + 90);
	innerLine.setLength(trueLength * polyCurvature);
	innerCPoint = innerLine.p2();
	QLineF outerLine = QLineF(startPoint, currItem->PoLine.pointQF(6));
	outerLine.setLength(outerLine.length() * polyOuterCurvature);
	outerCPoint = outerLine.p2();
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
	Mxp = Myp = -1;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem_RegularPolygon* item = currItem->asRegularPolygon();
	centerPoint = QPointF(currItem->width() / 2.0, currItem->height() / 2.0);
	startPoint = currItem->PoLine.pointQF(0);
	endPoint = currItem->PoLine.pointQF(2);
	polyCorners = item->polyCorners;
	polyUseFactor = item->polyUseFactor;
	polyFactor = item->polyFactor;
	polyRotation = item->polyRotation;
	polyCurvature = item->polyCurvature;
	polyInnerRot = item->polyInnerRot;
	polyOuterCurvature = item->polyOuterCurvature;
	VectorDialog = new PolyVectorDialog(m_ScMW, polyCorners, polyFactor, polyUseFactor, polyRotation, polyCurvature, polyInnerRot, polyOuterCurvature);
	VectorDialog->show();
	uint cx = polyUseFactor ? polyCorners * 2 : polyCorners;
	double seg = 360.0 / cx;
	double trueLength = sqrt(pow(sin(seg / 180.0 * M_PI) * (item->width() / 2.0), 2) + pow(cos(seg / 180.0 * M_PI) * (item->height() / 2.0) + (item->height()/2.0) - item->height(), 2));
	QLineF innerLine = QLineF(endPoint, centerPoint);
	innerLine.setAngle(innerLine.angle() + 90);
	innerLine.setLength(trueLength * polyCurvature);
	innerCPoint = innerLine.p2();
	QLineF outerLine = QLineF(startPoint, currItem->PoLine.pointQF(6));
	outerLine.setLength(outerLine.length() * polyOuterCurvature);
	outerCPoint = outerLine.p2();
	setModeCursor();
	if (fromGesture)
		m_view->update();
	connect(m_doc, SIGNAL(updateEditItem()), this, SLOT(updateFromItem()));
	connect(m_ScMW->propertiesPalette, SIGNAL(updateEditItem()), this, SLOT(updateFromItem()));
	
	connect(VectorDialog, SIGNAL(NewVectors(int, double, bool, double, double, double, double)), this, SLOT(applyValues(int, double, bool, double, double, double, double)));
	connect(VectorDialog, SIGNAL(endEdit()), this, SLOT(endEditing()));
	connect(VectorDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
}

void CanvasMode_EditPolygon::deactivate(bool forGesture)
{
	disconnect(VectorDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
	VectorDialog->close();
	delete VectorDialog;
	m_view->redrawMarker->hide();
	m_polygonPoint = noPointDefined;
	disconnect(m_doc, SIGNAL(updateEditItem()), this, SLOT(updateFromItem()));
	disconnect(m_ScMW->propertiesPalette, SIGNAL(updateEditItem()), this, SLOT(updateFromItem()));
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
	polyCorners = polyC;
	polyFactor = polyF;
	polyRotation = polyR;
	polyCurvature = polyCur;
	polyInnerRot = polyIRot;
	polyOuterCurvature = polyOCur;
	item->polyCorners = polyC;
	item->polyUseFactor = polyUseCF;
	item->polyFactor = polyFactor;
	item->polyRotation = polyRotation;
	item->polyCurvature = polyCurvature;
	item->polyInnerRot = polyInnerRot;
	item->polyOuterCurvature = polyOuterCurvature;
	item->recalcPath();
	updateFromItem();
	QRectF upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
	upRect.translate(currItem->xPos(), currItem->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
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
		QTransform itemMatrix;
		itemMatrix.translate(currItem->xPos(), currItem->yPos());
		itemMatrix.rotate(currItem->rotation());
		QPointF cPoint = itemMatrix.map(centerPoint);
		QLineF stLinA = QLineF(cPoint, QPointF(newX, newY));
		
		uint cx = polyUseFactor ? polyCorners * 2 : polyCorners;
		double seg = 360.0 / cx;
		double trueLength = sqrt(pow(sin(seg / 180.0 * M_PI) * (currItem->width() / 2.0), 2) + pow(cos(seg / 180.0 * M_PI) * (currItem->height() / 2.0) + (currItem->height()/2.0) - currItem->height(), 2));
		
		if (m_polygonPoint == useControlInner)
		{
			polyInnerRot = stLinA.angle() - 90 - polyRotation - seg;
			double factor = stLinA.length() / sqrt(pow(sin(stLinA.angle() * M_PI / 180.0) * currItem->height() / 2.0, 2) + pow(cos(stLinA.angle() * M_PI / 180.0) * currItem->width() / 2.0, 2));
			int maxF = qRound(getUserValFromFactor(factor));
			if (maxF <= 100)
				polyFactor = factor;
		}
		if (m_polygonPoint == useControlOuter)
		{
			polyRotation = stLinA.angle() - 90;
			if (polyRotation < -180)
				polyRotation += 360;
			if (polyRotation > 180)
				polyRotation -= 360;
		}
		if (m_polygonPoint == useControlInnerCurve)
		{
			QPointF ePoint = itemMatrix.map(endPoint);
			QLineF stLinC = QLineF(ePoint, QPointF(newX, newY));
			polyCurvature = stLinC.length() / trueLength;
		}
		if (m_polygonPoint == useControlOuterCurve)
		{
			QPointF sPoint = itemMatrix.map(startPoint);
			QPointF sPoint2 = itemMatrix.map(currItem->PoLine.pointQF(6));
			QLineF stLinCo = QLineF(sPoint, QPointF(newX, newY));
			QLineF stLinCo2 = QLineF(sPoint, sPoint2);
			polyOuterCurvature = stLinCo.length() / stLinCo2.length();
		}
		QPainterPath path = RegularPolygonPath(currItem->width(), currItem->height(), polyCorners, polyUseFactor, polyFactor, polyRotation, polyCurvature, polyInnerRot, polyOuterCurvature);
		FPointArray ar;
		ar.fromQPainterPath(path);
		endPoint = ar.pointQF(2);
		startPoint = ar.pointQF(0);
		QLineF innerLine = QLineF(endPoint, centerPoint);
		innerLine.setAngle(innerLine.angle() + 90);
		innerLine.setLength(trueLength * polyCurvature);
		innerCPoint = innerLine.p2();
		QLineF outerLine = QLineF(startPoint, ar.pointQF(6));
		outerLine.setLength(outerLine.length() * polyOuterCurvature);
		outerCPoint = outerLine.p2();
		VectorDialog->setValues(polyCorners, polyFactor, polyUseFactor, polyRotation, polyCurvature, polyInnerRot, polyOuterCurvature);
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
	QPointF shPoint = innerCPoint;
	shPoint = itemMatrix.map(shPoint);
	QPointF sPoint = outerCPoint;
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
	if ((m_polygonPoint == useControlInner) || (m_polygonPoint == useControlOuter) || (m_polygonPoint == useControlInnerCurve) || (m_polygonPoint == useControlOuterCurve))
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
		double trueLength = sqrt(pow(sin(seg / 180.0 * M_PI) * (currItem->width() / 2.0), 2) + pow(cos(seg / 180.0 * M_PI) * (currItem->height() / 2.0) + (currItem->height()/2.0) - currItem->height(), 2));
		if (m_polygonPoint == useControlInner)
		{
			polyInnerRot = stLinA.angle() - 90 - polyRotation - seg;
			polyFactor = stLinA.length() / sqrt(pow(sin(stLinA.angle() * M_PI / 180.0) * currItem->height() / 2.0, 2) + pow(cos(stLinA.angle() * M_PI / 180.0) * currItem->width() / 2.0, 2));
		}
		if (m_polygonPoint == useControlOuter)
			polyRotation = stLinA.angle() - 90;
		if (m_polygonPoint == useControlInnerCurve)
		{
			QPointF ePoint = itemMatrix.map(endPoint);
			QLineF stLinC = QLineF(ePoint, QPointF(newX, newY));
			polyCurvature = stLinC.length() / trueLength;
		}
		if (m_polygonPoint == useControlOuterCurve)
		{
			QPointF sPoint = itemMatrix.map(startPoint);
			QPointF sPoint2 = itemMatrix.map(currItem->PoLine.pointQF(6));
			QLineF stLinCo = QLineF(sPoint, QPointF(newX, newY));
			QLineF stLinCo2 = QLineF(sPoint, sPoint2);
			polyOuterCurvature = stLinCo.length() / stLinCo2.length();
		}
		item->polyFactor = polyFactor;
		item->polyRotation = polyRotation;
		item->polyCurvature = polyCurvature;
		item->polyInnerRot = polyInnerRot;
		item->polyOuterCurvature = polyOuterCurvature;
		item->recalcPath();
		VectorDialog->setValues(polyCorners, polyFactor, polyUseFactor, polyRotation, polyCurvature, polyInnerRot, polyOuterCurvature);
	}
	QRectF upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
	upRect.translate(currItem->xPos(), currItem->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
}
