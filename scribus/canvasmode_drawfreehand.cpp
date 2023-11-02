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


#include "canvasmode_drawfreehand.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPoint>
#include <QRect>

#include "KarbonCurveFit.h"
#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "prefsmanager.h"
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


FreehandMode::FreehandMode(ScribusView* view) : CanvasMode(view) 
{
}

void FreehandMode::drawControls(QPainter* p) 
{
	//
}

inline bool FreehandMode::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != nullptr;
}

void FreehandMode::enterEvent(QEvent *e)
{
	if (!m_mouseButtonPressed)
	{
		setModeCursor();
	}
}


void FreehandMode::leaveEvent(QEvent *e)
{
}


void FreehandMode::activate(bool fromGesture)
{
	CanvasMode::activate(fromGesture);
	m_xp = m_yp = -1;
	m_mouseButtonPressed = false;
	setModeCursor();
}

void FreehandMode::deactivate(bool forGesture)
{
	m_view->setRedrawMarkerShown(false);
	CanvasMode::deactivate(forGesture);
}

void FreehandMode::keyPressEvent(QKeyEvent *e)
{
	commonkeyPressEvent_Default(e);
}

void FreehandMode::keyReleaseEvent(QKeyEvent *e)
{
	commonkeyReleaseEvent(e);
}

void FreehandMode::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_mouseButtonPressed = false;
	m_canvas->resetRenderMode();
	mousePressEvent(m);
}

void FreehandMode::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	PageItem *currItem;
	m->accept();
	m_canvas->displayCorrectedXYHUD(m->globalPos(), mousePointDoc.x(), mousePointDoc.y());
	if (commonMouseMove(m))
		return;
	if (m_mouseButtonPressed && (m_doc->appMode == modeDrawFreehandLine))
	{
		double newXF = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
		double newYF = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
		if (!m_poly.empty())
		{
			if (FPoint(newXF, newYF) != m_poly.point(m_poly.size()-1))
				m_poly.addPoint(FPoint(newXF, newYF));
		}
		else
			m_poly.addPoint(FPoint(newXF, newYF));
		QPolygon& redrawPolygon(m_canvas->newRedrawPolygon());
		for (int pp = 0; pp < m_poly.size(); pp++)
		{
			redrawPolygon << m_poly.pointQ(pp);
		}
		m_canvas->m_viewMode.operItemResizing = true;
		QRect bRect = m_canvas->redrawPolygon().boundingRect();
		m_view->updateCanvas(bRect);
		return;
	}
	if (GetItem(&currItem))
	{
		if (m_doc->DragP)
			return;
				
		if ((!m_mouseButtonPressed) && (m_doc->appMode != modeDrawBezierLine))
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
		if ((m_mouseButtonPressed) && (m->buttons() & Qt::LeftButton))
		{
			QPoint startP = m_canvas->canvasToGlobal(QPointF(m_xp, m_yp));
			m_view->redrawMarker->setGeometry(QRect(m_view->mapFromGlobal(startP), m_view->mapFromGlobal(m->globalPos())).normalized());
			m_view->setRedrawMarkerShown(true);
			m_view->HaveSelRect = true;
			return;
		}
	}
}

void FreehandMode::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	double Rxp = 0;
	double Ryp = 0;
	QTransform pm;
	m_mouseButtonPressed = true;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	m_xp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	m_yp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
//	QRect mpo(m->x()-m_doc->guidesPrefs().grabRadius, m->y()-m_doc->guidesPrefs().grabRadius, m_doc->guidesPrefs().grabRadius*2, m_doc->guidesPrefs().grabRadius*2);
	Rxp = m_doc->ApplyGridF(FPoint(m_xp, m_yp)).x();
	m_xp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(m_xp, m_yp)).y();
	m_yp = qRound(Ryp);
	if (m->button() == Qt::MiddleButton)
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
	m_poly.resize(0);
	m_view->deselectItems(false);
	m_xp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	m_yp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	m_canvas->setRenderModeFillBuffer();
	undoManager->setUndoEnabled(false);
}



void FreehandMode::mouseReleaseEvent(QMouseEvent *m)
{
	undoManager->setUndoEnabled(true);
	PageItem *currItem;
	m_mouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	if (m_doc->appMode == modeDrawFreehandLine)
	{
		if (m_poly.size() > 1)
		{
			UndoTransaction createTransaction(UndoManager::instance()->beginTransaction());
			uint z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_xp, m_yp, 1, 1, m_doc->itemToolPrefs().lineWidth, CommonStrings::None, m_doc->itemToolPrefs().lineColor);
			currItem = m_doc->Items->at(z);
			currItem->PoLine.resize(0);
			if (m->modifiers() & Qt::ControlModifier)
			{
				QList<QPointF> clip;
				for (int px = 0; px < m_poly.size()-1; ++px)
				{
					FPoint clp = m_poly.point(px);
					clip.append(QPointF(clp.x(), clp.y()));
				}
				QPainterPath pp = bezierFit(clip, 5.0);
				currItem->PoLine.fromQPainterPath(pp);
			}
			else
			{
				currItem->PoLine.addPoint(m_poly.point(0));
				currItem->PoLine.addPoint(m_poly.point(0));
				for (int px = 1; px < m_poly.size()-1; ++px)
				{
					currItem->PoLine.addPoint(m_poly.point(px));
					currItem->PoLine.addPoint(m_poly.point(px));
					currItem->PoLine.addPoint(m_poly.point(px));
					currItem->PoLine.addPoint(m_poly.point(px));
				}
				currItem->PoLine.addPoint(m_poly.point(m_poly.size()-1));
				currItem->PoLine.addPoint(m_poly.point(m_poly.size()-1));
			}
			FPoint tp2(getMinClipF(&currItem->PoLine));
			currItem->setXYPos(tp2.x(), tp2.y(), true);
			currItem->PoLine.translate(-tp2.x(), -tp2.y());
			FPoint tp(getMaxClipF(&currItem->PoLine));
			m_doc->sizeItem(tp.x(), tp.y(), currItem, false, false, false);
			m_doc->adjustItemSize(currItem);
			m_doc->m_Selection->clear();
			m_doc->m_Selection->addItem(currItem);
			currItem->ClipEdited = true;
			currItem->FrameType = 3;
			currItem->OwnPage = m_doc->OnPage(currItem);
			m_view->resetMousePressed();
			currItem->checkChanges();
			QString targetName = Um::ScratchSpace;
			if (currItem->OwnPage > -1)
				targetName = m_doc->Pages->at(currItem->OwnPage)->getUName();
			createTransaction.commit(targetName, currItem->getUPixmap(), Um::Create + " " + currItem->getUName(),  "", Um::ICreate);
			//FIXME	
			m_canvas->m_viewMode.operItemResizing = false;
			m_doc->changed();
		}
		if (!PrefsManager::instance().appPrefs.uiPrefs.stickyTools)
		{
			m_view->requestMode(modeNormal);
		}
		else
			m_view->requestMode(m_doc->appMode);
// itemAdd calls PageItem::update		emit DocChanged();
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
	if (m_doc->m_Selection->itemAt(0) != nullptr)
	{
		m_doc->itemAddCommit(m_doc->m_Selection->itemAt(0));
	}
	//Make sure the Zoom spinbox and page selector don't have focus if we click on the canvas
	m_view->m_ScMW->zoomSpinBox->clearFocus();
	m_view->m_ScMW->pageSelector->clearFocus();
	if (m_doc->m_Selection->itemAt(0) != nullptr) // is there the old clip stored for the undo action
	{
		currItem = m_doc->m_Selection->itemAt(0);
		m_doc->nodeEdit.finishTransaction(currItem);
	}
}

void FreehandMode::selectPage(QMouseEvent *m)
{
	m_mouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_xp = mousePointDoc.x(); //static_cast<int>(m->x()/m_canvas->scale());
	m_yp = mousePointDoc.y(); //static_cast<int>(m->y()/m_canvas->scale());
	m_doc->nodeEdit.deselect();
	m_view->deselectItems(false);

	if (m_doc->masterPageMode())
		return;

	int i = m_doc->OnPage(m_xp, m_yp);
	if (i < 0)
		return;

	uint docCurrPageNo = m_doc->currentPageNumber();
	uint j = static_cast<uint>(i);
	if (docCurrPageNo != j)
	{
		m_doc->setCurrentPage(m_doc->Pages->at(j));
		m_view->m_ScMW->slotSetCurrentPage(j);
		m_view->DrawNew();
	}
}
