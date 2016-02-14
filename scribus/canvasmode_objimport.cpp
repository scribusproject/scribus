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
#include <QMimeData>
#include <QDebug>

#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/pageselector.h"
#include "ui/scrspinbox.h"
#include "undomanager.h"
#include "util.h"
#include "util_math.h"


CanvasMode_ObjImport::CanvasMode_ObjImport(ScribusView* view) :
	CanvasMode(view)
{
	m_Mxp = m_Myp = -1;
	m_mimeData = NULL;
	m_trSettings = NULL;
}

CanvasMode_ObjImport::~CanvasMode_ObjImport()
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
	drawSelection(p, false);
}

void CanvasMode_ObjImport::enterEvent(QEvent *)
{
	if (m_mimeData)
	{
		setModeCursor();
		return;
	}
}

void CanvasMode_ObjImport::leaveEvent(QEvent *e)
{
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
	m_Mxp = m_Myp = -1;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_ObjImport::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_ObjImport::deactivate" << forGesture;
	setMimeData(NULL);
	setTransactionSettings(NULL);
	m_view->setRedrawMarkerShown(false);
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
	m_Mxp = mousePointDoc.x();
	m_Myp = mousePointDoc.y();
	Rxp = m_doc->ApplyGridF(FPoint(m_Mxp, m_Myp)).x();
	m_Mxp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(m_Mxp, m_Myp)).y();
	m_Myp = qRound(Ryp);
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
}

void CanvasMode_ObjImport::mouseReleaseEvent(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	if ((m->button() == Qt::LeftButton) && m_mimeData)
	{
		UndoTransaction undoTransaction;
		if (m_trSettings && UndoManager::undoEnabled())
		{
			undoTransaction = UndoManager::instance()->beginTransaction(*m_trSettings);
		}
		// Creating QDragEnterEvent outside of Qt is not recommended per docs :S
		QPoint dropPos = m_view->widget()->mapFromGlobal(m->globalPos());
		const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
		QDropEvent dropEvent(dropPos, Qt::CopyAction|Qt::MoveAction, m_mimeData, m->buttons(), m->modifiers());
		m_view->contentsDropEvent(&dropEvent);
		if (m_doc->m_Selection->count() > 0)
		{
			double gx, gy, gh, gw;
			m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			m_doc->moveGroup(mousePointDoc.x() - gx, mousePointDoc.y() -gy);
		}
		// Commit undo transaction if necessary
		if (undoTransaction)
		{
			undoTransaction.commit();
		}
		// Return to normal mode
		m_view->requestMode(modeNormal);
	}
//	m_view->stopDragTimer();
	
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	//Make sure the Zoom spinbox and page selector don't have focus if we click on the canvas
	m_view->m_ScMW->zoomSpinBox->clearFocus();
	m_view->m_ScMW->pageSelector->clearFocus();
}





