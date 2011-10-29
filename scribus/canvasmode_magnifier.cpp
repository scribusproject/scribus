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


#include "canvasmode_magnifier.h"

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

#include "canvas.h"
#include "contextmenu.h"
#include "customfdialog.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "pageitem_textframe.h"
#include "pageselector.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"

CanvasMode_Magnifier::CanvasMode_Magnifier(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
}

void CanvasMode_Magnifier::drawControls(QPainter* p)
{
	commonDrawControls(p, false);
}

void CanvasMode_Magnifier::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}


void CanvasMode_Magnifier::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void CanvasMode_Magnifier::activate(bool fromGesture)
{
//	qDebug() << "CanvasMode_Magnifier::activate" << fromGesture;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_Magnifier::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_Magnifier::deactivate" << forGesture;
	m_canvas->hideRectangleSelection();
}

void CanvasMode_Magnifier::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
//	m_view->stopDragTimer();
}


void CanvasMode_Magnifier::mouseMoveEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m_lastPosWasOverGuide = false;
	double newX, newY;
	m->accept();
	if (commonMouseMove(m))
		return;
	if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(Myp + ((SeRx - Mxp) * m_view->visibleHeight()) / m_view->visibleWidth());
		SeRx = newX;
		SeRy = newY;
		/*
		m_view->redrawMarker->setGeometry(QRect(Mxp, Myp, m->globalPos().x() - Mxp, m->globalPos().y() - Myp).normalized());
		*/
		QPoint startP = m_canvas->canvasToGlobal(m_doc->appMode == modeDrawTable? QPointF(Dxp, Dyp) : QPointF(Mxp, Myp));
		m_canvas->displayRectangleSelection(startP, m->globalPos());
	}
}

void CanvasMode_Magnifier::mousePressEvent(QMouseEvent *m)
{
	double Rxp = 0, Ryp = 0;
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m_canvas->PaintSizeRect(QRect());
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->hideRectangleSelection();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	Mxp = mousePointDoc.x();
	Myp = mousePointDoc.y();
	Rxp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).x();
	Mxp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).y();
	Myp = qRound(Ryp);
	SeRx = Mxp;
	SeRy = Myp;
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	if ((m->modifiers() == Qt::ShiftModifier) || (m->button() == Qt::RightButton))
	{
		m_view->Magnify = false;
		qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
	}
	else
	{
		m_view->Magnify = true;
		qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
	}
	Mxp = mousePointDoc.x(); //m->globalPos().x();
	Myp = mousePointDoc.y(); //m->globalPos().y();
	SeRx = Mxp;
	SeRy = Myp;
	m_canvas->displayRectangleSelection(m->globalPos(), m->globalPos() + QPoint(1, 1));
}



void CanvasMode_Magnifier::mouseReleaseEvent(QMouseEvent *m)
{
#ifdef GESTURE_FRAME_PREVIEW
        clearPixmapCache();
#endif // GESTURE_FRAME_PREVIEW
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
//	m_view->stopDragTimer();
	if (m_doc->appMode == modeMagnifier)
	{
		if (m_canvas->haveRectangleSelection())
		{
			QRect geom = m_canvas->getRectangleSelection();
			FPoint nx = m_canvas->globalToCanvas(QPoint(geom.x() + geom.width() / 2, geom.y() + geom.height() / 2));
			double scaleAdjust = m_view->visibleWidth() / static_cast<double>(qMax(geom.width(), 1));
			m_view->zoomRelative(nx.x(), nx.y(), scaleAdjust, false);
			m_canvas->hideRectangleSelection();
			if (qAbs(scaleAdjust - 1) < 0.001)
			{
				m_view->requestMode(submodePaintingDone);
			}
		}
		else
		{
			FPoint nx = mousePointDoc;
			int mx = qRound(nx.x());
			int my = qRound(nx.y());
			if (m_view->Magnify)
				m_view->slotZoomIn(mx,my);
			else
				m_view->slotZoomOut(mx,my);
			
			if (m->modifiers() & Qt::ShiftModifier)
				qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
			else
				qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
		}
	}
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
