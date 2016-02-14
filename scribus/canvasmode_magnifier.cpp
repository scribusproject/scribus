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

#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "iconmanager.h"
#include "pageitem_textframe.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/contextmenu.h"
#include "ui/pageselector.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"

CanvasMode_Magnifier::CanvasMode_Magnifier(ScribusView* view) : CanvasMode(view)
{
	m_Mxp = m_Myp = -1;
	m_Dxp = m_Dyp = -1;
	m_SeRx = m_SeRy = -1;
	m_lastPosWasOverGuide = false;
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
	m_Mxp = m_Myp = -1;
	m_Dxp = m_Dyp = -1;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_Magnifier::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_Magnifier::deactivate" << forGesture;
	m_view->setRedrawMarkerShown(false);
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
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m_lastPosWasOverGuide = false;
	double newX, newY;
	m->accept();
	if (commonMouseMove(m))
		return;
	if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(m_Myp + ((m_SeRx - m_Mxp) * m_view->visibleHeight()) / m_view->visibleWidth());
		m_SeRx = newX;
		m_SeRy = newY;
		QPoint startP = m_canvas->canvasToGlobal(m_doc->appMode == modeDrawTable2 ? QPointF(m_Dxp, m_Dyp) : QPointF(m_Mxp, m_Myp));
		m_view->redrawMarker->setGeometry(QRect(m_view->mapFromGlobal(startP), m_view->mapFromGlobal(m->globalPos())).normalized());
		m_view->setRedrawMarkerShown(true);
		m_view->HaveSelRect = true;
	}
}

void CanvasMode_Magnifier::mousePressEvent(QMouseEvent *m)
{
	double Rxp = 0, Ryp = 0;
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m_canvas->PaintSizeRect(QRect());
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	m_Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	m_Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
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
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	if ((m->modifiers() == Qt::ShiftModifier) || (m->button() == Qt::RightButton))
	{
		m_view->Magnify = false;
		m_view->setCursor(IconManager::instance()->loadCursor("LupeZm.xpm"));
	}
	else
	{
		m_view->Magnify = true;
		m_view->setCursor(IconManager::instance()->loadCursor("LupeZ.xpm"));
	}
	m_Mxp = mousePointDoc.x(); //m->globalPos().x();
	m_Myp = mousePointDoc.y(); //m->globalPos().y();
	m_SeRx = m_Mxp;
	m_SeRy = m_Myp;
	m_view->redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
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
		double sc = m_canvas->scale();
		if (m_view->HaveSelRect)
		{
			QRect geom = m_view->redrawMarker->geometry().normalized();
			geom = QRect(m_view->mapToGlobal(geom.topLeft()), m_view->mapToGlobal(geom.bottomRight()));
			FPoint nx = m_canvas->globalToCanvas(QPoint(geom.x() + geom.width() / 2, geom.y() + geom.height() / 2));
			double scaleH = m_view->visibleWidth() / static_cast<double>(qMax(geom.width(), 1));
			double scaleV = m_view->visibleHeight() / static_cast<double>(qMax(geom.height(), 1));
			double scaleAdjust = qMax(0.5, qMin(scaleH, scaleV));
			m_view->zoom(nx.x(), nx.y(), m_canvas->scale() * scaleAdjust, false);
			if (sc == m_canvas->scale())
			{
				m_view->HaveSelRect = false;
				m_view->setRedrawMarkerShown(false);
				m_view->requestMode(submodePaintingDone);
			}
			m_view->setRedrawMarkerShown(false);
		}
		else
		{
			FPoint nx = mousePointDoc;
			int mx = qRound(nx.x());
			int my = qRound(nx.y());
			m_view->Magnify ? m_view->slotZoomIn(mx,my) : m_view->slotZoomOut(mx,my);
			if (sc == m_canvas->scale())
			{
				m_view->HaveSelRect = false;
				m_view->requestMode(submodePaintingDone);
			}
			else
			{
				if (m->modifiers() & Qt::ShiftModifier)
					m_view->setCursor(IconManager::instance()->loadCursor("LupeZm.xpm"));
				else
					m_view->setCursor(IconManager::instance()->loadCursor("LupeZ.xpm"));
			}
		}
	}
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
