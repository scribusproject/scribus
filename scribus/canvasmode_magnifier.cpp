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
//	qDebug() << "CanvasMode_Magnifier::drawControls";
	if (m_canvas->m_viewMode.operItemMoving)
	{
		drawOutline(p);
	}
	else
	{
		drawSelection(p);
	}
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
		m_canvas->m_viewMode.operItemResizeInEditMode = false;
		m_view->update();
	}
}

void CanvasMode_Magnifier::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_Magnifier::deactivate" << forGesture;
	m_view->redrawMarker->hide();
}

void CanvasMode_Magnifier::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_view->stopDragTimer();
}


void CanvasMode_Magnifier::mouseMoveEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m_lastPosWasOverGuide = false;
	double newX, newY;
	QPoint np, np2, mop;
	bool erf = false;
	m->accept();
//	qDebug() << "legacy mode move:" << m->x() << m->y() << m_canvas->globalToCanvas(m->globalPos()).x() << m_canvas->globalToCanvas(m->globalPos()).y();
//	emit MousePos(m->x()/m_canvas->scale(),// + m_doc->minCanvasCoordinate.x(), 
//				  m->y()/m_canvas->scale()); // + m_doc->minCanvasCoordinate.y());

	if (m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
	{
		m_ScMW->setAppMode(modePanning);
	}
	if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modePanning))
	{
		double sc = m_canvas->scale();
		int scroX = qRound((mousePointDoc.x() - Mxp) * sc);
		int scroY = qRound((mousePointDoc.y() - Myp) * sc);
		m_view->scrollBy(-scroX, -scroY);
//		Mxp = static_cast<int>((m->x()-scroX)/sc);
//		Myp = static_cast<int>((m->y()-scroY)/sc);
		Mxp = mousePointDoc.x();
		Myp = mousePointDoc.y();
		return;
	}
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
		m_view->redrawMarker->setGeometry(QRect(startP, m->globalPos()).normalized());
		if (!m_view->redrawMarker->isVisible())
			m_view->redrawMarker->show();
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
	Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	Rxp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).x();
	Mxp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).y();
	Myp = qRound(Ryp);
	SeRx = Mxp;
	SeRy = Myp;
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
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
	m_view->redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
}



void CanvasMode_Magnifier::mouseReleaseEvent(QMouseEvent *m)
{
	clearPixmapCache();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->stopDragTimer();
	if (m_doc->appMode == modePanning)
	{
		m_ScMW->setAppMode(modeMagnifier);
		return;
	}
	if (m_doc->appMode == modeMagnifier)
	{
		double sc = m_canvas->scale();
		if (m_view->HaveSelRect)
		{
			QRect geom = m_view->redrawMarker->geometry().normalized();
			FPoint nx = m_canvas->globalToCanvas(QPoint(geom.x() + geom.width() / 2, geom.y() + geom.height() / 2));
			double scaleAdjust = m_view->visibleWidth() / static_cast<double>(qMax(geom.width(), 1));
			/*m_view->setScale(m_canvas->scale() * scaleAdjust);
			m_view->slotDoZoom();
			m_view->SetCCPo(nx.x(), nx.y());*/
			m_view->zoom(nx.x(), nx.y(), m_canvas->scale() * scaleAdjust, false);
			if (sc == m_canvas->scale())
			{
				m_view->HaveSelRect = false;
				m_view->redrawMarker->hide();
				m_view->requestMode(submodePaintingDone);
			}
			m_view->redrawMarker->hide();
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
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
				else
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
			}
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
