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


#include "canvasmode_drawcalligraphic.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPoint>
#include <QRect>


#include "KarbonCurveFit.h"
#include "appmodes.h"
#include "canvas.h"
#include "canvasgesture_resize.h"
#include "fpoint.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/pageselector.h"
#include "ui/scrspinbox.h"
#include "undomanager.h"
#include "util.h"
#include "util_math.h"





CalligraphicMode::CalligraphicMode(ScribusView* view) : CanvasMode(view) 
{
	m_Mxp = m_Myp = -1;
	m_Dxp = m_Dyp = -1;
	m_MouseButtonPressed = false;
}


void CalligraphicMode::drawControls(QPainter* p) 
{
	//
}

inline bool CalligraphicMode::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}

void CalligraphicMode::enterEvent(QEvent *)
{
	if (!m_MouseButtonPressed)
	{
		setModeCursor();
	}
}


void CalligraphicMode::leaveEvent(QEvent *e)
{
}


void CalligraphicMode::activate(bool flag)
{
	m_Mxp = m_Myp = -1;
	m_Dxp = m_Dyp = -1;
	m_MouseButtonPressed = false;
	setModeCursor();
}

void CalligraphicMode::deactivate(bool flag)
{
	m_view->setRedrawMarkerShown(false);
}

void CalligraphicMode::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	mousePressEvent(m);
}


void CalligraphicMode::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	double newX, newY;
	PageItem *currItem;
	QPoint np, np2, mop;
	FPoint npf, npf2;
	QRect tx;
	m->accept();
	m_canvas->displayCorrectedXYHUD(m->globalPos(), mousePointDoc.x(), mousePointDoc.y());

	if (commonMouseMove(m))
		return;
	
	if (m_MouseButtonPressed && (m_doc->appMode == modeDrawCalligraphicLine))
	{
		double newXF = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
		double newYF = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
		if (m_RecordP.size() > 0)
		{
			if (FPoint(newXF, newYF) != m_RecordP.point(m_RecordP.size()-1))
				m_RecordP.addPoint(FPoint(newXF, newYF));
		}
		else
			m_RecordP.addPoint(FPoint(newXF, newYF));
		QPolygon& redrawPolygon(m_canvas->newRedrawPolygon());
		double mx = sin(m_doc->itemToolPrefs().calligraphicPenAngle / 180.0 * M_PI) * (m_doc->itemToolPrefs().calligraphicPenWidth / 2.0);
		double my = cos(m_doc->itemToolPrefs().calligraphicPenAngle / 180.0 * M_PI) * (m_doc->itemToolPrefs().calligraphicPenWidth / 2.0);
		for (int px = 0; px < m_RecordP.size()-1; ++px)
		{
			FPoint clp = m_RecordP.point(px);
			redrawPolygon.append(QPoint(qRound(clp.x() - mx), qRound(clp.y() - my)));
			redrawPolygon.prepend(QPoint(qRound(clp.x() + mx), qRound(clp.y() + my)));
		}
		redrawPolygon.append(QPoint(qRound(m_RecordP.point(m_RecordP.size()-1).x() + mx), qRound(m_RecordP.point(m_RecordP.size()-1).y() + my)));
//FIXME		
		m_canvas->m_viewMode.operItemResizing = true;
		QRect bRect = m_canvas->redrawPolygon().boundingRect();
		m_view->updateCanvas(bRect);
		return;
	}
	
	if (GetItem(&currItem))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		
		if (m_doc->DragP)
			return;
				
		if ((!m_MouseButtonPressed) && (m_doc->appMode != modeDrawBezierLine))
		{
			if (m_doc->m_Selection->isMultipleSelection())
			{
				setModeCursor();
				return;
			}
			for (int a = 0; a < m_doc->m_Selection->count(); ++a)
			{
				currItem = m_doc->m_Selection->itemAt(a);
				if (currItem->locked())
					break;
				setModeCursor();
			}
		}
	}
	else
	{
		if ((m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton))
		{
			newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
			newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
			m_SeRx = newX;
			m_SeRy = newY;
			QPoint startP = m_canvas->canvasToGlobal(m_doc->appMode == modeDrawTable2 ? QPointF(m_Dxp, m_Dyp) : QPointF(m_Mxp, m_Myp));
			m_view->redrawMarker->setGeometry(QRect(m_view->mapFromGlobal(startP), m_view->mapFromGlobal(m->globalPos())).normalized());
			m_view->setRedrawMarkerShown(true);
			m_view->HaveSelRect = true;
			return;
		}
	}
}

void CalligraphicMode::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	double Rxp = 0;
	double Ryp = 0;
	FPoint npf, npf2;
	QRect tx;
	QTransform pm;
	m_MouseButtonPressed = true;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	m_Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	m_Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	QRect mpo(m->x()-m_doc->guidesPrefs().grabRadius, m->y()-m_doc->guidesPrefs().grabRadius, m_doc->guidesPrefs().grabRadius*2, m_doc->guidesPrefs().grabRadius*2);
	Rxp = m_doc->ApplyGridF(FPoint(m_Mxp, m_Myp)).x();
	m_Mxp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(m_Mxp, m_Myp)).y();
	m_Myp = qRound(Ryp);
	m_SeRx = m_Mxp;
	m_SeRy = m_Myp;
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	if (m->button() != Qt::LeftButton)
	{
		m_view->stopGesture();
		return;
	}
	m_RecordP.resize(0);
	m_view->Deselect(false);
	m_Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	m_Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	m_SeRx = m_Mxp;
	m_SeRy = m_Myp;
	m_canvas->setRenderModeFillBuffer();
	undoManager->setUndoEnabled(false);
}



void CalligraphicMode::mouseReleaseEvent(QMouseEvent *m)
{
	undoManager->setUndoEnabled(true);
	PageItem *currItem;
	m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	
	if (m_doc->appMode == modeDrawCalligraphicLine)
	{
		if (m_RecordP.size() > 1)
		{
			UndoTransaction createTransaction;
			if (UndoManager::undoEnabled())
				createTransaction = UndoManager::instance()->beginTransaction();
			uint z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_Mxp, m_Myp, 1, 1, m_doc->itemToolPrefs().calligraphicPenLineWidth, m_doc->itemToolPrefs().calligraphicPenFillColor, m_doc->itemToolPrefs().calligraphicPenLineColor, true);
			currItem = m_doc->Items->at(z);
			currItem->PoLine.resize(0);
			QList<QPointF> clipU;
			QList<QPointF> clipL;
			double mx = sin(m_doc->itemToolPrefs().calligraphicPenAngle / 180.0 * M_PI) * (m_doc->itemToolPrefs().calligraphicPenWidth / 2.0);
			double my = cos(m_doc->itemToolPrefs().calligraphicPenAngle / 180.0 * M_PI) * (m_doc->itemToolPrefs().calligraphicPenWidth / 2.0);
			for (int px = 0; px < m_RecordP.size()-1; ++px)
			{
				FPoint clp = m_RecordP.point(px);
				clipU.append(QPointF(clp.x() - mx, clp.y() - my));
				clipL.prepend(QPointF(clp.x() + mx, clp.y() + my));
			}
			QPainterPath ppU = bezierFit(clipU, 5.0);
			QPainterPath ppL = bezierFit(clipL, 5.0);
			QPainterPath pp;
			pp.addPath(ppU);
			pp.connectPath(ppL);
			pp.closeSubpath();
			currItem->PoLine.fromQPainterPath(pp);
			FPoint tp2(getMinClipF(&currItem->PoLine));
			currItem->setXYPos(tp2.x(), tp2.y(), true);
			currItem->PoLine.translate(-tp2.x(), -tp2.y());
			FPoint tp(getMaxClipF(&currItem->PoLine));
			m_doc->SizeItem(tp.x(), tp.y(), currItem, false, false, false);
			m_doc->AdjustItemSize(currItem);
			m_doc->m_Selection->clear();
			m_doc->m_Selection->addItem(currItem);
			currItem->ClipEdited = true;
			currItem->FrameType = 3;
			currItem->OwnPage = m_doc->OnPage(currItem);
			currItem->PLineArt = Qt::PenStyle(m_doc->itemToolPrefs().calligraphicPenStyle);
			currItem->setFillShade(m_doc->itemToolPrefs().calligraphicPenFillColorShade);
			currItem->setLineShade(m_doc->itemToolPrefs().calligraphicPenLineColorShade);
			currItem->setFillEvenOdd(true);
			m_view->resetMousePressed();
			currItem->checkChanges();
			QString targetName = Um::ScratchSpace;
			if (currItem->OwnPage > -1)
				targetName = m_doc->Pages->at(currItem->OwnPage)->getUName();
			if (createTransaction)
				createTransaction.commit(targetName, currItem->getUPixmap(), Um::Create + " " + currItem->getUName(),  "", Um::ICreate);
			//FIXME	
			m_canvas->m_viewMode.operItemResizing = false;
			m_doc->changed();
		}
		if (!PrefsManager::instance()->appPrefs.uiPrefs.stickyTools)
		{
			m_view->requestMode(modeNormal);
		}
		else
			m_view->requestMode(m_doc->appMode);
		return;
	}

	m_canvas->setRenderModeUseBuffer(false);
	
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_view->MidButt = false;
	if (m_view->groupTransactionStarted())
	{
		for (int i = 0; i < m_doc->m_Selection->count(); ++i)
			m_doc->m_Selection->itemAt(i)->checkChanges(true);
		m_view->endGroupTransaction();
	}

	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
		m_doc->m_Selection->itemAt(i)->checkChanges(true);

	//Commit drag created items to undo manager.
	if (m_doc->m_Selection->itemAt(0)!=NULL)
	{
		m_doc->itemAddCommit(m_doc->m_Selection->itemAt(0));
	}
	//Make sure the Zoom spinbox and page selector dont have focus if we click on the canvas
	m_view->m_ScMW->zoomSpinBox->clearFocus();
	m_view->m_ScMW->pageSelector->clearFocus();
	if (m_doc->m_Selection->itemAt(0) != 0) // is there the old clip stored for the undo action
	{
		currItem = m_doc->m_Selection->itemAt(0);
		m_doc->nodeEdit.finishTransaction(currItem);
	}
}

void CalligraphicMode::selectPage(QMouseEvent *m)
{
	m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_Mxp = mousePointDoc.x(); //static_cast<int>(m->x()/m_canvas->scale());
	m_Myp = mousePointDoc.y(); //static_cast<int>(m->y()/m_canvas->scale());
	QRect mpo(m->x()-m_doc->guidesPrefs().grabRadius, m->y()-m_doc->guidesPrefs().grabRadius, m_doc->guidesPrefs().grabRadius*2, m_doc->guidesPrefs().grabRadius*2);
	m_doc->nodeEdit.deselect();
	m_view->Deselect(false);
	if (!m_doc->masterPageMode())
	{
		int i = m_doc->OnPage(m_Mxp, m_Myp);
		if (i!=-1)
		{
			uint docCurrPageNo=m_doc->currentPageNumber();
			uint j=static_cast<uint>(i);
			if (docCurrPageNo != j)
			{
				m_doc->setCurrentPage(m_doc->Pages->at(j));
				m_view->m_ScMW->slotSetCurrentPage(j);
				m_view->DrawNew();
			}
		}
	}
}
