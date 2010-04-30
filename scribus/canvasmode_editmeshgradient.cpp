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
                 canvasmode_editmeshgradient.cpp  -  description
                             -------------------
    begin                : Sat Apr 24 2010
    copyright            : (C) 2010 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/


#include "canvasmode_editmeshgradient.h"

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
#include "hyphenator.h"
#include "pageitem_textframe.h"
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

CanvasMode_EditMeshGradient::CanvasMode_EditMeshGradient(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	Mxp = Myp = -1;
	arrayX = -1;
	arrayY = -1;
	m_gradientPoint = noPointDefined;
}

inline bool CanvasMode_EditMeshGradient::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0);
	return (*pi) != NULL;
}

void CanvasMode_EditMeshGradient::drawControls(QPainter* p)
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

void CanvasMode_EditMeshGradient::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_EditMeshGradient::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void CanvasMode_EditMeshGradient::activate(bool fromGesture)
{
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

void CanvasMode_EditMeshGradient::deactivate(bool forGesture)
{
	m_view->redrawMarker->hide();
	arrayX = -1;
	arrayY = -1;
	m_gradientPoint = noPointDefined;
	m_ScMW->propertiesPalette->updateColorSpecialGradient();
}

void CanvasMode_EditMeshGradient::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
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


void CanvasMode_EditMeshGradient::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m->accept();
	if ((arrayX == -1) || (arrayY == -1))
		return;
	if (m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		double newX = mousePointDoc.x();
		double newY = mousePointDoc.y();
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		FPoint npx(Mxp - newX, Myp - newY, 0, 0, currItem->rotation(), 1, 1, true);
		if ((arrayX > -1) && (arrayY > -1))
		{
			if (m_view->editStrokeGradient == 5)
			{
				currItem->meshGradientArray[arrayX][arrayY].moveRel(-npx.x(), -npx.y());
			}
			else if (m_view->editStrokeGradient == 7)
			{
				if (m_gradientPoint == useControlT)
					currItem->meshGradientArray[arrayX][arrayY].controlTop -= npx;
				else if (m_gradientPoint == useControlB)
					currItem->meshGradientArray[arrayX][arrayY].controlBottom -= npx;
				else if (m_gradientPoint == useControlL)
					currItem->meshGradientArray[arrayX][arrayY].controlLeft -= npx;
				else if (m_gradientPoint == useControlR)
					currItem->meshGradientArray[arrayX][arrayY].controlRight -= npx;
			}
			currItem->update();
			QRectF upRect;
			upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
			upRect.translate(currItem->xPos(), currItem->yPos());
			m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
		}
		Mxp = newX;
		Myp = newY;
	}
}

void CanvasMode_EditMeshGradient::mousePressEvent(QMouseEvent *m)
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
	arrayX = -1;
	arrayY = -1;
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
	QTransform itemMatrix;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	currItem->selectedMeshPointX = arrayX;
	currItem->selectedMeshPointY = arrayY;
	itemMatrix.translate(currItem->xPos(), currItem->yPos());
	itemMatrix.rotate(currItem->rotation());
	bool found = false;
	if ((m_view->editStrokeGradient == 5) || (m_view->editStrokeGradient == 6))
	{
		for (int grow = 0; grow < currItem->meshGradientArray.count(); grow++)
		{
			for (int gcol = 0; gcol < currItem->meshGradientArray[grow].count(); gcol++)
			{
				meshPoint mp = currItem->meshGradientArray[grow][gcol];
				QPointF gradientPoint = QPointF(mp.gridPoint.x(), mp.gridPoint.y());
				gradientPoint = itemMatrix.map(gradientPoint);
				if (m_canvas->hitsCanvasPoint(m->globalPos(), gradientPoint))
				{
					arrayX = grow;
					arrayY = gcol;
					currItem->selectedMeshPointX = arrayX;
					currItem->selectedMeshPointY = arrayY;
					found = true;
					break;
				}
			}
			if (found)
				break;
		}
	}
	else if (m_view->editStrokeGradient == 7)
	{
		for (int grow = 0; grow < currItem->meshGradientArray.count(); grow++)
		{
			for (int gcol = 0; gcol < currItem->meshGradientArray[grow].count(); gcol++)
			{
				meshPoint mp1 = currItem->meshGradientArray[grow][gcol];
				QPointF gradientPoint;
				if (grow == 0)
				{
					if (gcol == 0)
					{
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
					}
					else if (gcol == currItem->meshGradientArray[grow].count()-1)
					{
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
					}
					else
					{
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
					}
				}
				else if (grow == currItem->meshGradientArray.count()-1)
				{
					if (gcol == 0)
					{
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
					}
					else if (gcol == currItem->meshGradientArray[grow].count()-1)
					{
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
					}
					else
					{
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
					}
				}
				else
				{
					if (gcol == 0)
					{
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
					}
					else if (gcol == currItem->meshGradientArray[grow].count()-1)
					{
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
					}
					else
					{
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(m->globalPos(), itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
					}
				}
				if (found)
				{
					arrayX = grow;
					arrayY = gcol;
					currItem->selectedMeshPointX = arrayX;
					currItem->selectedMeshPointY = arrayY;
					currItem->selectedMeshControlPoint = static_cast<int>(m_gradientPoint);
					break;
				}
			}
			if (found)
				break;
		}
		if (!found)
		{
			for (int grow = 0; grow < currItem->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < currItem->meshGradientArray[grow].count(); gcol++)
				{
					meshPoint mp = currItem->meshGradientArray[grow][gcol];
					QPointF gradientPoint = QPointF(mp.gridPoint.x(), mp.gridPoint.y());
					gradientPoint = itemMatrix.map(gradientPoint);
					if (m_canvas->hitsCanvasPoint(m->globalPos(), gradientPoint))
					{
						arrayX = grow;
						arrayY = gcol;
						currItem->selectedMeshPointX = arrayX;
						currItem->selectedMeshPointY = arrayY;
						found = true;
						break;
					}
				}
				if (found)
					break;
			}
		}
	}
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_ScMW->propertiesPalette->updateColorSpecialGradient();
	qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
	QRectF upRect;
	upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
	upRect.translate(currItem->xPos(), currItem->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));

}

void CanvasMode_EditMeshGradient::mouseReleaseEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	arrayX = -1;
	arrayY = -1;
	m_gradientPoint = noPointDefined;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	currItem->update();
	QRectF upRect;
	upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
	upRect.translate(currItem->xPos(), currItem->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));

}