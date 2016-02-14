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
				 canvasmode_editweldpoint.cpp  -  description
							 -------------------
	begin                : Sun Oct 23 2011
	copyright            : (C) 2011 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/


#include "canvasmode_editweldpoint.h"

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
#include "hyphenator.h"
#include "pageitem_textframe.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "sccolorengine.h"
#include "sclimits.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/aligndistribute.h"
#include "ui/contextmenu.h"
#include "ui/pageselector.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"


CanvasMode_EditWeldPoint::CanvasMode_EditWeldPoint(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW)
{
	m_Mxp = m_Myp = -1;
	m_selectedPoint = -1;
	m_editWeldMode = true;
}

inline bool CanvasMode_EditWeldPoint::GetItem(PageItem** pi)
{
	*pi = m_doc->m_Selection->itemAt(0);
	return (*pi) != NULL;
}

void CanvasMode_EditWeldPoint::drawControls(QPainter* p)
{
	p->save();
	if (m_canvas->m_viewMode.operItemMoving)
	{
		drawOutline(p);
	}
	else
	{
		drawSelection(p, false);
	}
	if (m_doc->appMode == modeEditWeldPoint)
	{
		drawControlsWeldPoint(p, m_doc->m_Selection->itemAt(0));
	}
	p->restore();
}

void CanvasMode_EditWeldPoint::drawControlsWeldPoint(QPainter* psx, PageItem* currItem)
{
	psx->translate(static_cast<int>(currItem->xPos()), static_cast<int>(currItem->yPos()));
	psx->rotate(currItem->rotation());
	psx->setBrush(Qt::NoBrush);
	for (int i = 0 ; i <  currItem->weldList.count(); i++)
	{
		PageItem::WeldingInfo wInf =  currItem->weldList.at(i);
		if (i == m_selectedPoint)
			psx->setPen(QPen(Qt::red, 8.0 / m_canvas->scale(), Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
		else
			psx->setPen(QPen(Qt::yellow, 8.0 / m_canvas->scale(), Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
		psx->drawPoint(QPointF(wInf.weldPoint.x(), wInf.weldPoint.y()));
	}
}

void CanvasMode_EditWeldPoint::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_EditWeldPoint::leaveEvent(QEvent *e)
{
}


void CanvasMode_EditWeldPoint::activate(bool fromGesture)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	m_keyRepeat = false;
	m_currItem = m_doc->m_Selection->itemAt(0);
	m_weldToList = m_currItem->itemsWeldedTo();
	m_weldToList.append(m_currItem);
	m_Mxp = m_Myp = -1;
	m_selectedPoint = -1;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
	m_ModeDialog = new WeldEditDialog(m_ScMW);
	m_ModeDialog->show();
	connect(m_ModeDialog, SIGNAL(endEdit()), this, SLOT(endEditing()));
	connect(m_ModeDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
	connect(m_ModeDialog, SIGNAL(modeMoveWeld()), this, SLOT(setWeldMode()));
	connect(m_ModeDialog, SIGNAL(modeMoveObject()), this, SLOT(setObjectMode()));
}

void CanvasMode_EditWeldPoint::deactivate(bool forGesture)
{
	m_view->setRedrawMarkerShown(false);
	m_selectedPoint = -1;
	m_weldToList.clear();
	disconnect(m_ModeDialog, SIGNAL(paletteShown(bool)), this, SLOT(endEditing(bool)));
	m_ModeDialog->close();
	delete m_ModeDialog;
}

void CanvasMode_EditWeldPoint::endEditing(bool active)
{
	if (!active)
		endEditing();
}

void CanvasMode_EditWeldPoint::endEditing()
{
	m_view->requestMode(modeNormal);
}

void CanvasMode_EditWeldPoint::setWeldMode()
{
	m_editWeldMode = true;
}

void CanvasMode_EditWeldPoint::setObjectMode()
{
	m_editWeldMode = false;
}

void CanvasMode_EditWeldPoint::keyPressEvent(QKeyEvent *e)
{
	if (m_selectedPoint < 0)
		return;
	int kk = e->key();
	if (m_keyRepeat)
		return;
	m_keyRepeat = true;
	e->accept();

	if (e->key() == Qt::Key_Escape)
	{
		// Go back to normal mode.
		m_view->requestMode(modeNormal);
		return;
	}

	Qt::KeyboardModifiers buttonModifiers = e->modifiers();
	if ((!m_view->m_ScMW->zoomSpinBox->hasFocus()) && (!m_view->m_ScMW->pageSelector->hasFocus()))
	{
		if (m_doc->m_Selection->count() != 0)
		{
			double moveBy = 1.0;
			double moveX = 0.0;
			double moveY = 0.0;
			bool isMoving = false;
			bool doUpdate = false;
			if (m_doc->unitIndex()!=SC_INCHES)
			{
				if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.1;
				else if (!(buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=10.0;
				else if ((buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.01;
				moveBy/=m_doc->unitRatio();//Lets allow movement by the current doc ratio, not only points
			}
			else
			{
				if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.1/m_doc->unitRatio();
				else if (!(buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=1.0/m_doc->unitRatio();
				else if ((buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.01/m_doc->unitRatio();
			}
			moveBy /= m_canvas->m_viewMode.scale;
			PageItem *currItem = m_doc->m_Selection->itemAt(0);
			switch (kk)
			{
				case Qt::Key_7:
					moveX = -moveBy;
					moveY = -moveBy;
					isMoving = true;
					doUpdate = true;
					break;
				case Qt::Key_9:
					moveX = moveBy;
					moveY = -moveBy;
					isMoving = true;
					doUpdate = true;
					break;
				case Qt::Key_3:
					moveX = moveBy;
					moveY = moveBy;
					isMoving = true;
					doUpdate = true;
					break;
				case Qt::Key_1:
					moveX = -moveBy;
					moveY = moveBy;
					isMoving = true;
					doUpdate = true;
					break;
				case Qt::Key_Left:
				case Qt::Key_4:
					moveX = -moveBy;
					isMoving = true;
					doUpdate = true;
					break;
				case Qt::Key_Right:
				case Qt::Key_6:
					moveX = moveBy;
					isMoving = true;
					doUpdate = true;
					break;
				case Qt::Key_Up:
				case Qt::Key_8:
					moveY = -moveBy;
					isMoving = true;
					doUpdate = true;
					break;
				case Qt::Key_Down:
				case Qt::Key_2:
					moveY = moveBy;
					isMoving = true;
					doUpdate = true;
					break;
			}
			if (isMoving)
			{
				if (m_editWeldMode)
				{
					currItem->weldList[m_selectedPoint].weldPoint += FPoint(moveX, moveY);
				}
				else
				{
					currItem->setXYPos(currItem->xPos() + moveX, currItem->yPos() + moveY, true);
					currItem->setRedrawBounding();
					currItem->OwnPage = m_doc->OnPage(currItem);
				}
			}
			if (doUpdate)
			{
				currItem->update();
				m_doc->regionsChanged()->update(getUpdateRect());
			}
		}
	}
	m_keyRepeat = false;
}

void CanvasMode_EditWeldPoint::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	if ((m_doc->m_Selection->isMultipleSelection()) || (m_doc->appMode != modeNormal))
	{
		if ((m_doc->m_Selection->isMultipleSelection()) && (m_doc->appMode == modeNormal))
		{
			if (GetItem(&m_currItem))
			{
				/* CB: old code, removing this as shift-alt select on an unselected table selects a cell now.
				//#6789 is closed by sorting this.
				if (currItem->isTableItem)
				{
					m_view->Deselect(false);
					m_doc->m_Selection->addItem(currItem);
					currItem->isSingleSel = true;
					//CB FIXME don't call this if the added item is item 0
					if (!m_doc->m_Selection->primarySelectionIs(currItem))
						currItem->emitAllToGUI();
					m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				}*/
			}
			return;
		}
		else
		{
			if (!(GetItem(&m_currItem) && (m_doc->appMode == modeEdit) && m_currItem->asTextFrame()))
			{
				mousePressEvent(m);
				return;
			}
		}
	}
}

void CanvasMode_EditWeldPoint::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m->accept();
	FPoint npfN;
	double nx = mousePointDoc.x();
	double ny = mousePointDoc.y();
	if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
		npfN = m_doc->ApplyGridF(FPoint(nx, ny));
	else
		npfN = FPoint(nx, ny);
	m_currItem = m_doc->m_Selection->itemAt(0);
	FPoint npf = FPoint(npfN.x(), npfN.y(), m_currItem->xPos(), m_currItem->yPos(), m_currItem->rotation(), 1, 1, true);
	FPoint npx(m_Mxp - npfN.x(), m_Myp - npfN.y(), 0, 0, m_currItem->rotation(), 1, 1, true);
	m_canvas->displayXYHUD(m->globalPos(), npf.x(), npf.y());
	if (m_canvas->m_viewMode.m_MouseButtonPressed && m_view->moveTimerElapsed())
	{
		if (m_editWeldMode)
		{
			if (m_selectedPoint != -1)
			{
				m_canvas->displayXYHUD(m->globalPos(), npf.x(), npf.y());
				FPoint mp, mp_orig;
				mp_orig = m_currItem->weldList[m_selectedPoint].weldPoint;
				mp = mp_orig - npx;
				double xx = mp.x();
				double yy = mp.y();
				snapToEdgePoints(xx, yy);
				m_currItem->weldList[m_selectedPoint].weldPoint = FPoint(xx, yy);
			}
		}
		else
		{
			m_currItem->setXYPos(m_currItem->xPos() - npx.x(), m_currItem->yPos() - npx.y(), true);
			m_currItem->setRedrawBounding();
			m_currItem->OwnPage = m_doc->OnPage(m_currItem);
		}
		m_doc->regionsChanged()->update(getUpdateRect());
	}
	m_Mxp = npfN.x();
	m_Myp = npfN.y();
}

void CanvasMode_EditWeldPoint::mousePressEvent(QMouseEvent *m)
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
	m_Mxp = mousePointDoc.x(); //m->x();
	m_Myp = mousePointDoc.y(); //m->y();
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	QTransform itemMatrix;
	m_currItem = m_doc->m_Selection->itemAt(0);
	itemMatrix.translate(m_currItem->xPos(), m_currItem->yPos());
	itemMatrix.rotate(m_currItem->rotation());
	m_selectedPoint = -1;
	for (int i = 0 ; i <  m_currItem->weldList.count(); i++)
	{
		PageItem::WeldingInfo wInf = m_currItem->weldList.at(i);
		if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(wInf.weldPoint.x(), wInf.weldPoint.y()))))
		{
			m_selectedPoint = i;
			break;
		}
	}
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_view->setCursor(QCursor(Qt::CrossCursor));
	m_doc->regionsChanged()->update(getUpdateRect());
}

void CanvasMode_EditWeldPoint::mouseReleaseEvent(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_currItem = m_doc->m_Selection->itemAt(0);
	m_currItem->update();
	m_doc->regionsChanged()->update(getUpdateRect());
}

void CanvasMode_EditWeldPoint::snapToEdgePoints(double &x, double &y)
{
	int radius = m_doc->guidesPrefs().grabRadius;
	if (qAbs(0.0 - x) < radius && qAbs(0.0 - y) < radius)
	{
		x = 0.0;
		y = 0.0;
		return;
	}
	else if ((qAbs((m_currItem->width() / 2.0) - x) < radius) && (qAbs(0.0 - y) < radius))
	{
		x = m_currItem->width() / 2.0;
		y = 0.0;
		return;
	}
	else if ((qAbs(m_currItem->width() - x) < radius) && (qAbs(0.0 - y) < radius))
	{
		x = m_currItem->width();
		y = 0.0;
		return;
	}
	else if ((qAbs(0.0 - x) < radius) && (qAbs((m_currItem->height() / 2.0) - y) < radius))
	{
		x = 0.0;
		y = m_currItem->height() / 2.0;
		return;
	}
	else if ((qAbs((m_currItem->width() / 2.0) - x) < radius) && (qAbs((m_currItem->height() / 2.0) - y) < radius))
	{
		x = m_currItem->width() / 2.0;
		y = m_currItem->height() / 2.0;
		return;
	}
	else if ((qAbs(m_currItem->width() - x) < radius) && (qAbs((m_currItem->height() / 2.0) - y) < radius))
	{
		x = m_currItem->width();
		y = m_currItem->height() / 2.0;
		return;
	}
	else if ((qAbs(0.0 - x) < radius) && (qAbs(m_currItem->height() - y) < radius))
	{
		x = 0.0;
		y = m_currItem->height();
		return;
	}
	else if ((qAbs((m_currItem->width() / 2.0) - x) < radius) && (qAbs(m_currItem->height() - y) < radius))
	{
		x = m_currItem->width() / 2.0;
		y = m_currItem->height();
		return;
	}
	else if ((qAbs(m_currItem->width() - x) < radius) && (qAbs(m_currItem->height() - y) < radius))
	{
		x = m_currItem->width();
		y = m_currItem->height();
		return;
	}
}

QRectF CanvasMode_EditWeldPoint::getUpdateRect()
{
	PageItem *item;
	uint selectedItemCount = m_weldToList.count();
	if (selectedItemCount == 0)
		return QRectF();
	double vminx =  std::numeric_limits<double>::max();
	double vminy =  std::numeric_limits<double>::max();
	double vmaxx = -std::numeric_limits<double>::max();
	double vmaxy = -std::numeric_limits<double>::max();

	for (uint gc = 0; gc < selectedItemCount; ++gc)
	{
		item = m_weldToList.at(gc);
		if (item->rotation() != 0)
		{
			QRectF itRect(item->getVisualBoundingRect());
			vminx = qMin(vminx, itRect.x());
			vminy = qMin(vminy, itRect.y());
			vmaxx = qMax(vmaxx, itRect.right());
			vmaxy = qMax(vmaxy, itRect.bottom());
		}
		else
		{
			vminx = qMin(vminx, item->visualXPos());
			vminy = qMin(vminy, item->visualYPos());
			vmaxx = qMax(vmaxx, item->visualXPos() + item->visualWidth());
			vmaxy = qMax(vmaxy, item->visualYPos() + item->visualHeight());
		}
	}
	return QRectF(vminx, vminy, vmaxx - vminx, vmaxy - vminy).adjusted(-20, -20, 40, 40);
}
