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


#include "canvasmode_editgradient.h"

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

#include "aligndistribute.h"
#include "canvas.h"
#include "contextmenu.h"
#include "fpoint.h"
#include "fpointarray.h"
// #include "hruler.h"
// #include "vruler.h"
#include "hyphenator.h"
#include "insertTable.h"
// #include "oneclick.h"
#include "pageitem_textframe.h"
#include "pageselector.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
// #include "scraction.h"
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

CanvasMode_EditGradient::CanvasMode_EditGradient(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	Mxp = Myp = -1;
	m_gradientPoint = noPointDefined;
}

inline bool CanvasMode_EditGradient::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}

void CanvasMode_EditGradient::drawControls(QPainter* p)
{
	if (m_canvas->m_viewMode.operItemMoving)
	{
		drawOutline(p);
	}
	else
	{
		drawSelection(p, false);
	}
}

void CanvasMode_EditGradient::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_EditGradient::leaveEvent(QEvent *e)
{

}


void CanvasMode_EditGradient::activate(bool fromGesture)
{
//	qDebug() << "CanvasMode_EditGradient::activate" << fromGesture;
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
		m_view->update();
	}
}

void CanvasMode_EditGradient::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_EditGradient::deactivate" << forGesture;
	m_view->redrawMarker->hide();
}

void CanvasMode_EditGradient::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
//	m_view->stopDragTimer();
	PageItem *currItem = 0;
	if ((m_doc->m_Selection->isMultipleSelection()) || (m_doc->appMode != modeNormal))
	{
		if ((m_doc->m_Selection->isMultipleSelection()) && (m_doc->appMode == modeNormal))
		{
			if (GetItem(&currItem))
			{
				/* CB: old code, removing this as shift-alt select on an unselected table selects a cell now.
				//#6789 is closed by sorting this.
				if (currItem->isTableItem)
				{
					m_view->Deselect(false);
					m_doc->m_Selection->addItem(currItem);
					currItem->isSingleSel = true;
					//CB FIXME dont call this if the added item is item 0
					if (!m_doc->m_Selection->primarySelectionIs(currItem))
						currItem->emitAllToGUI();
					m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				}*/
			}
			return;
		}
		else
		{
			if (!(GetItem(&currItem) && (m_doc->appMode == modeEdit) && currItem->asTextFrame()))
			{
				mousePressEvent(m);
				return;
			}
		}
	}
}


void CanvasMode_EditGradient::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m->accept();
//	qDebug() << "legacy mode move:" << m->x() << m->y() << m_canvas->globalToCanvas(m->globalPos()).x() << m_canvas->globalToCanvas(m->globalPos()).y();

	if (m_gradientPoint == noPointDefined)
		return;

	if (m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		double newX = mousePointDoc.x(); //m->x();
		double newY = mousePointDoc.y(); //m->y();
		double dx = fabs(Mxp - newX) + 5.0 / m_canvas->scale();
		double dy = fabs(Myp - newY) + 5.0 / m_canvas->scale();
		FPoint np(Mxp - newX, Myp - newY, 0, 0, currItem->rotation(), 1, 1, true);
		if (m_gradientPoint == useGradientStart)
		{
			currItem->GrStartX -= np.x(); // (Mxp - newX); // / m_canvas->scale();
			currItem->GrStartY -= np.y(); // (Myp - newY); // / m_canvas->scale();
		}
		if (m_gradientPoint == useGradientEnd)
		{
			currItem->GrEndX -= np.x(); // (Mxp - newX); // / m_canvas->scale();
			currItem->GrEndY -= np.y(); // (Myp - newY); // / m_canvas->scale();
		}
		Mxp = newX;
		Myp = newY;
		m_view->RefreshGradient(currItem, dx * m_canvas->scale(), dy * m_canvas->scale());
		m_ScMW->propertiesPalette->updateColorSpecialGradient();
		currItem->update();
	}
}

void CanvasMode_EditGradient::mousePressEvent(QMouseEvent *m)
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
	m_gradientPoint = noPointDefined;
	Mxp = mousePointDoc.x(); //m->x();
	Myp = mousePointDoc.y(); //m->y();
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	QMatrix itemMatrix;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	itemMatrix.translate(currItem->xPos(), currItem->yPos());
	itemMatrix.rotate(currItem->rotation());
	QPointF gradientStart(currItem->GrStartX, currItem->GrStartY);
	gradientStart = itemMatrix.map(gradientStart);
	QPointF gradientEnd(currItem->GrEndX, currItem->GrEndY);
	gradientEnd = itemMatrix.map(gradientEnd);
	if (m_canvas->hitsCanvasPoint(m->globalPos(), gradientStart))
		m_gradientPoint = useGradientStart;
	else if (m_canvas->hitsCanvasPoint(m->globalPos(), gradientEnd))
		m_gradientPoint = useGradientEnd;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_view->setCursor(QCursor(Qt::CrossCursor));
}

void CanvasMode_EditGradient::mouseReleaseEvent(QMouseEvent *m)
{
#ifdef GESTURE_FRAME_PREVIEW
	clearPixmapCache();
#endif // GESTURE_FRAME_PREVIEW
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_gradientPoint = noPointDefined;
//	m_view->stopDragTimer();
}
