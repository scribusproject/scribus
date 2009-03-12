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


#include "canvasmode_panning.h"

#include <QCursor>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>
#include <QDebug>

#include "canvas.h"
#include "canvasgesture_pan.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "scribusview.h"
#include "util.h"
#include "util_icon.h"

CanvasMode_Panning::CanvasMode_Panning(ScribusView* view) : CanvasMode(view) 
{
	m_panGesture = new PanGesture(this);
}

void CanvasMode_Panning::drawControls(QPainter* p)
{
	commonDrawControls(p);
}

void CanvasMode_Panning::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}


void CanvasMode_Panning::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void CanvasMode_Panning::activate(bool fromGesture)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	setModeCursor();
	if (fromGesture)
	{
		m_canvas->m_viewMode.operItemResizeInEditMode = false;
		m_view->update();
	}
}

void CanvasMode_Panning::deactivate(bool forGesture)
{
	m_view->redrawMarker->hide();
}

void CanvasMode_Panning::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
//	m_view->stopDragTimer();
}

void CanvasMode_Panning::mousePressEvent(QMouseEvent *m)
{
	/*m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());*/
	
	m_view->startGesture(m_panGesture);
	m_panGesture->mousePressEvent(m);
}
