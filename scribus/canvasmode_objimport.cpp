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


#include "canvasmode_objimport.h"

#include <QApplication>
#include <QCursor>
#include <QDropEvent>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>
#include <QDebug>

#include "canvas.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "pageselector.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "undomanager.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"

CanvasMode_ObjImport::CanvasMode_ObjImport(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	Mxp = Myp = -1;
	m_mimeData = NULL;
	m_trSettings = NULL;
}

CanvasMode_ObjImport::~CanvasMode_ObjImport(void)
{
	if (m_mimeData)
		delete m_mimeData;
	if (m_trSettings)
		delete m_trSettings;
}

void CanvasMode_ObjImport::setMimeData(QMimeData* mimeData)
{
	if (m_mimeData)
		delete m_mimeData;
	m_mimeData = mimeData;
}

void CanvasMode_ObjImport:: setTransactionSettings(TransactionSettings* settings)
{
	if (m_trSettings)
		delete m_trSettings;
	m_trSettings = settings;
}

void CanvasMode_ObjImport::drawControls(QPainter* p)
{
//	qDebug() << "CanvasMode_ObjImport::drawControls";
	drawSelection(p);
}

void CanvasMode_ObjImport::enterEvent(QEvent *)
{
	if (m_mimeData)
	{
		setModeCursor();
		return;
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void CanvasMode_ObjImport::leaveEvent(QEvent *e)
{
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void CanvasMode_ObjImport::activate(bool fromGesture)
{
//	qDebug() << "CanvasMode_ObjImport::activate" << fromGesture;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	Mxp = Myp = -1;
	setModeCursor();
	if (fromGesture)
	{
		m_canvas->m_viewMode.operItemResizeInEditMode = false;
		m_view->update();
	}
}

void CanvasMode_ObjImport::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_ObjImport::deactivate" << forGesture;
	setMimeData(NULL);
	setTransactionSettings(NULL);
	m_view->redrawMarker->hide();
}

void CanvasMode_ObjImport::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
//	m_view->stopDragTimer();
}

void CanvasMode_ObjImport::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m->accept();

	if (commonMouseMove(m))
		return;
}

void CanvasMode_ObjImport::mousePressEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());

	double Rxp = 0, Ryp = 0;
	m_canvas->PaintSizeRect(QRect());
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	Mxp = mousePointDoc.x();
	Myp = mousePointDoc.y();
	Rxp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).x();
	Mxp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).y();
	Myp = qRound(Ryp);
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	if ((m->button() == Qt::LeftButton) && m_mimeData)
	{
		UndoTransaction* undoTransaction = NULL;
		if (m_trSettings && UndoManager::undoEnabled())
		{
			undoTransaction = new UndoTransaction(UndoManager::instance()->beginTransaction(*m_trSettings));
		}
		// Creating QDragEnterEvent outside of Qt is not recommended per docs :S
		QPoint dropPos = m_view->widget()->mapFromGlobal(m->globalPos());
		QDropEvent dropEvent(dropPos, Qt::CopyAction|Qt::MoveAction, m_mimeData, m->buttons(), m->modifiers());
		m_view->contentsDropEvent(&dropEvent);
		// Commit undo transaction if necessary
		if (undoTransaction)
		{
			undoTransaction->commit();
			delete undoTransaction;
		}
		// Return to normal mode
		m_view->requestMode(modeNormal);
	}
}

void CanvasMode_ObjImport::mouseReleaseEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
//	m_view->stopDragTimer();
	
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	//Make sure the Zoom spinbox and page selector dont have focus if we click on the canvas
	m_view->zoomSpinBox->clearFocus();
	m_view->pageSelector->clearFocus();
}




