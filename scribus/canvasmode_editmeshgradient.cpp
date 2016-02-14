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

#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hyphenator.h"
#include "pageitem_textframe.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
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


CanvasMode_EditMeshGradient::CanvasMode_EditMeshGradient(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	m_Mxp = m_Myp = -1;
	m_selectedMeshPoints.clear();
	m_gradientPoint = noPointDefined;
	m_old_mesh = new meshPoint();
	m_keyRepeat = false;
}

CanvasMode_EditMeshGradient::~CanvasMode_EditMeshGradient()
{
	delete m_old_mesh;
	m_old_mesh = NULL;
}

inline bool CanvasMode_EditMeshGradient::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0);
	return (*pi) != NULL;
}

void CanvasMode_EditMeshGradient::drawControls(QPainter* p)
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
	if (m_doc->appMode == modeEditMeshGradient)
	{
		drawControlsMeshGradient(p, m_doc->m_Selection->itemAt(0));
	}
	p->restore();
}

void CanvasMode_EditMeshGradient::drawControlsMeshGradient(QPainter* psx, PageItem* currItem)
{
	QPen p1b = QPen(Qt::blue, 1.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	QPen p1bd = QPen(Qt::blue, 1.0 / m_canvas->m_viewMode.scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
	QPen p8b = QPen(Qt::blue, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p8r = QPen(Qt::red, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p8m = QPen(Qt::magenta, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p14r = QPen(Qt::red, 18.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p14w = QPen(Qt::white, 18.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	psx->setTransform(currItem->getTransform(), true);
	psx->setPen(p1b);
	psx->setBrush(Qt::NoBrush);
	for (int grow = 0; grow < currItem->meshGradientArray.count()-1; grow++)
	{
		for (int gcol = 0; gcol < currItem->meshGradientArray[grow].count()-1; gcol++)
		{
			meshPoint mp1 = currItem->meshGradientArray[grow][gcol];
			meshPoint mp2 = currItem->meshGradientArray[grow][gcol+1];
			meshPoint mp3 = currItem->meshGradientArray[grow+1][gcol+1];
			meshPoint mp4 = currItem->meshGradientArray[grow+1][gcol];
			QPainterPath Bez;
			Bez.moveTo(mp1.gridPoint.x(), mp1.gridPoint.y());
			Bez.cubicTo(mp1.controlRight.x(), mp1.controlRight.y(), mp2.controlLeft.x(), mp2.controlLeft.y(), mp2.gridPoint.x(), mp2.gridPoint.y());
			Bez.cubicTo(mp2.controlBottom.x(), mp2.controlBottom.y(), mp3.controlTop.x(), mp3.controlTop.y(), mp3.gridPoint.x(), mp3.gridPoint.y());
			Bez.cubicTo(mp3.controlLeft.x(), mp3.controlLeft.y(), mp4.controlRight.x(), mp4.controlRight.y(), mp4.gridPoint.x(), mp4.gridPoint.y());
			Bez.cubicTo(mp4.controlTop.x(), mp4.controlTop.y(), mp1.controlBottom.x(), mp1.controlBottom.y(), mp1.gridPoint.x(), mp1.gridPoint.y());
			psx->setPen(p1b);
			psx->drawPath(Bez);
		}
	}
	for (int grow = 0; grow < currItem->meshGradientArray.count(); grow++)
	{
		for (int gcol = 0; gcol < currItem->meshGradientArray[grow].count(); gcol++)
		{
			bool isSelected = false;
			for (int se = 0; se < m_selectedMeshPoints.count(); se++)
			{
				if ((grow == m_selectedMeshPoints[se].first) && (gcol == m_selectedMeshPoints[se].second))
				{
					isSelected = true;
					break;
				}
			}
			meshPoint mp1 = currItem->meshGradientArray[grow][gcol];
			if (m_view->editStrokeGradient == 5)
			{
				if (isSelected)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()));
			}
			else if (m_view->editStrokeGradient == 6)
			{
				if (isSelected)
					psx->setPen(p14r);
				else
					psx->setPen(p14w);
				psx->drawPoint(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()));
				psx->setPen(QPen(mp1.color, 14.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
				psx->drawPoint(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()));
				if (isSelected)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp1.controlColor.x(), mp1.controlColor.y()));
			}
			else if (m_view->editStrokeGradient == 7)
			{
				psx->setPen(p8b);
				psx->drawPoint(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()));
			}
		}
	}
	if ((m_selectedMeshPoints.count() > 0) && (m_view->editStrokeGradient == 7))
	{
		int grow = m_selectedMeshPoints[0].first;
		int gcol = m_selectedMeshPoints[0].second;
		meshPoint mp1 = currItem->meshGradientArray[grow][gcol];
		psx->setPen(p8m);
		if (grow == 0)
		{
			if (gcol == 0)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlR)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlB)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
			}
			else if (gcol == currItem->meshGradientArray[grow].count()-1)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlL)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlB)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
			}
			else
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlL)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlB)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				if (m_gradientPoint == useControlR)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
			}
		}
		else if (grow == currItem->meshGradientArray.count()-1)
		{
			if (gcol == 0)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlR)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlT)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
			}
			else if (gcol == currItem->meshGradientArray[grow].count()-1)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlL)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlT)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
			}
			else
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlL)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlT)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				if (m_gradientPoint == useControlR)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
			}
		}
		else
		{
			if (gcol == 0)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlR)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlT)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlB)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
			}
			else if (gcol == currItem->meshGradientArray[grow].count()-1)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlL)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlT)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlB)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
			}
			else
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlL)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlLeft.x(), mp1.controlLeft.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlT)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlTop.x(), mp1.controlTop.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlB)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				psx->setPen(p8m);
				if (m_gradientPoint == useControlR)
					psx->setPen(p8r);
				psx->drawPoint(QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
			}
		}
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
	m_selectedMeshPoints.clear();
	m_keyRepeat = false;
	m_Mxp = m_Myp = -1;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_EditMeshGradient::deactivate(bool forGesture)
{
	m_view->setRedrawMarkerShown(false);
	m_selectedMeshPoints.clear();
	m_gradientPoint = noPointDefined;
	m_ScMW->propertiesPalette->updateColorSpecialGradient();
}

void CanvasMode_EditMeshGradient::keyPressEvent(QKeyEvent *e)
{
	if (m_selectedMeshPoints.count() == 0)
		return;
	int kk = e->key();
	if (m_keyRepeat)
		return;
	m_keyRepeat = true;
	e->accept();
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
			if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
				moveBy=0.1;
			else if (!(buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
				moveBy=10.0;
			else if ((buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
				moveBy=0.01;
			moveBy/=m_doc->unitRatio();//Lets allow movement by the current doc ratio, not only points
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
				case Qt::Key_5:
					if (m_view->editStrokeGradient == 6)
					{
						QPair<int, int> selP = m_selectedMeshPoints[0];
						currItem->meshGradientArray[selP.first][selP.second].controlColor = currItem->meshGradientArray[selP.first][selP.second].gridPoint;
						doUpdate = true;
					}
					else if (m_view->editStrokeGradient == 7)
					{
						QPair<int, int> selP = m_selectedMeshPoints[0];
						if (m_gradientPoint == useControlT)
							currItem->meshGradientArray[selP.first][selP.second].controlTop = currItem->meshGradientArray[selP.first][selP.second].gridPoint;
						else if (m_gradientPoint == useControlB)
							currItem->meshGradientArray[selP.first][selP.second].controlBottom = currItem->meshGradientArray[selP.first][selP.second].gridPoint;
						else if (m_gradientPoint == useControlL)
							currItem->meshGradientArray[selP.first][selP.second].controlLeft = currItem->meshGradientArray[selP.first][selP.second].gridPoint;
						else if (m_gradientPoint == useControlR)
							currItem->meshGradientArray[selP.first][selP.second].controlRight = currItem->meshGradientArray[selP.first][selP.second].gridPoint;
						doUpdate = true;
					}
					break;
			}
			if (isMoving)
			{
				if (m_view->editStrokeGradient == 5)
				{
					for (int mo = 0; mo < m_selectedMeshPoints.count(); mo++)
					{
						QPair<int, int> selP = m_selectedMeshPoints[mo];
						currItem->meshGradientArray[selP.first][selP.second].moveRel(moveX, moveY);
					}
				}
				else if (m_view->editStrokeGradient == 6)
				{
					QPair<int, int> selP = m_selectedMeshPoints[0];
					currItem->meshGradientArray[selP.first][selP.second].controlColor += FPoint(moveX, moveY);
				}
				else if (m_view->editStrokeGradient == 7)
				{
					QPair<int, int> selP = m_selectedMeshPoints[0];
					if (m_gradientPoint == useControlT)
						currItem->meshGradientArray[selP.first][selP.second].controlTop += FPoint(moveX, moveY);
					else if (m_gradientPoint == useControlB)
						currItem->meshGradientArray[selP.first][selP.second].controlBottom += FPoint(moveX, moveY);
					else if (m_gradientPoint == useControlL)
						currItem->meshGradientArray[selP.first][selP.second].controlLeft += FPoint(moveX, moveY);
					else if (m_gradientPoint == useControlR)
						currItem->meshGradientArray[selP.first][selP.second].controlRight += FPoint(moveX, moveY);
				}
			}
			if (doUpdate)
			{
				currItem->update();
				QRectF upRect;
				upRect = QRectF(QPointF(0, 0), QPointF(currItem->width(), currItem->height())).normalized();
				upRect.translate(currItem->xPos(), currItem->yPos());
				m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - currItem->width() / 2.0, -10.0 - currItem->height() / 2.0, 10.0 + currItem->width() / 2.0, 10.0 + currItem->height() / 2.0));
			}
		}
	}
	m_keyRepeat = false;
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
	if (m_selectedMeshPoints.count() == 0)
		return;
	if (m_canvas->m_viewMode.m_MouseButtonPressed && m_view->moveTimerElapsed())
	{
		FPoint npfN;
		double nx = mousePointDoc.x();
		double ny = mousePointDoc.y();
		if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
			npfN = m_doc->ApplyGridF(FPoint(nx, ny));
		else
			npfN = FPoint(nx, ny);
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		QTransform pp = currItem->getTransform();
		FPoint npf = npfN.transformPoint(pp, true);
		if (m_view->editStrokeGradient == 6)
		{
			if (m_selectedMeshPoints.count() > 0)
			{
				QPair<int, int> selP = m_selectedMeshPoints[0];
				FPoint cP = currItem->meshGradientArray[selP.first][selP.second].controlColor;
				FPoint gP = currItem->meshGradientArray[selP.first][selP.second].gridPoint;
				m_canvas->displayXYHUD(m->globalPos(), cP.x() - gP.x(), cP.y() - gP.y());
			}
		}
		else
			m_canvas->displayXYHUD(m->globalPos(), npf.x(), npf.y());
		FPoint npx(m_Mxp - npfN.x(), m_Myp - npfN.y(), 0, 0, currItem->rotation(), 1, 1, true);
		if (m_selectedMeshPoints.count() > 0)
		{
			if (m_view->editStrokeGradient == 5)
			{
				for (int mo = 0; mo < m_selectedMeshPoints.count(); mo++)
				{
					QPair<int, int> selP = m_selectedMeshPoints[mo];
					currItem->meshGradientArray[selP.first][selP.second].moveRel(-npx.x(), -npx.y());
				}
			}
			else if (m_view->editStrokeGradient == 6)
			{
				QPair<int, int> selP = m_selectedMeshPoints[0];
				currItem->meshGradientArray[selP.first][selP.second].controlColor -= npx;
			}
			else if (m_view->editStrokeGradient == 7)
			{
				QPair<int, int> selP = m_selectedMeshPoints[0];
				if (m_gradientPoint == useControlT)
					currItem->meshGradientArray[selP.first][selP.second].controlTop -= npx;
				else if (m_gradientPoint == useControlB)
					currItem->meshGradientArray[selP.first][selP.second].controlBottom -= npx;
				else if (m_gradientPoint == useControlL)
					currItem->meshGradientArray[selP.first][selP.second].controlLeft -= npx;
				else if (m_gradientPoint == useControlR)
					currItem->meshGradientArray[selP.first][selP.second].controlRight -= npx;
			}
			currItem->update();
			QTransform itemMatrix = currItem->getTransform();
			m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, currItem->width(), currItem->height())).adjusted(-currItem->width() / 2.0, -currItem->height() / 2.0, currItem->width(), currItem->height()));
		}
		m_Mxp = npfN.x();
		m_Myp = npfN.y();
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
	m_Mxp = mousePointDoc.x(); //m->x();
	m_Myp = mousePointDoc.y(); //m->y();
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	QTransform itemMatrix = currItem->getTransform();
	bool found = false;
	QPair<int, int> selPoint;
	if (m_view->editStrokeGradient == 5)
	{
		m_gradientPoint = noPointDefined;
		for (int grow = 0; grow < currItem->meshGradientArray.count(); grow++)
		{
			for (int gcol = 0; gcol < currItem->meshGradientArray[grow].count(); gcol++)
			{
				meshPoint mp = currItem->meshGradientArray[grow][gcol];
				QPointF gradientPoint = QPointF(mp.gridPoint.x(), mp.gridPoint.y());
				gradientPoint = itemMatrix.map(gradientPoint);
				if (m_canvas->hitsCanvasPoint(mousePointDoc, gradientPoint))
				{
					selPoint.first = grow;
					selPoint.second = gcol;
					found = true;
					*m_old_mesh = mp;
					break;
				}
			}
			if (found)
				break;
		}
	}
	else if (m_view->editStrokeGradient == 6)
	{
		m_gradientPoint = noPointDefined;
		m_selectedMeshPoints.clear();
		for (int grow = 0; grow < currItem->meshGradientArray.count(); grow++)
		{
			for (int gcol = 0; gcol < currItem->meshGradientArray[grow].count(); gcol++)
			{
				meshPoint mp = currItem->meshGradientArray[grow][gcol];
				QPointF gradientPoint = QPointF(mp.gridPoint.x(), mp.gridPoint.y());
				gradientPoint = itemMatrix.map(gradientPoint);
				QPointF gradientColorPoint = QPointF(mp.controlColor.x(), mp.controlColor.y());
				gradientColorPoint = itemMatrix.map(gradientColorPoint);
				if (m_canvas->hitsCanvasPoint(mousePointDoc, gradientPoint) || m_canvas->hitsCanvasPoint(mousePointDoc, gradientColorPoint))
				{
					selPoint.first = grow;
					selPoint.second = gcol;
					currItem->selectedMeshPointX = grow;
					currItem->selectedMeshPointY = gcol;
					*m_old_mesh = mp;
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
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
					}
					else if (gcol == currItem->meshGradientArray[grow].count()-1)
					{
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
					}
					else
					{
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
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
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
					}
					else if (gcol == currItem->meshGradientArray[grow].count()-1)
					{
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
					}
					else
					{
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
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
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
					}
					else if (gcol == currItem->meshGradientArray[grow].count()-1)
					{
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
					}
					else
					{
						gradientPoint = QPointF(mp1.controlBottom.x(), mp1.controlBottom.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlB;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlTop.x(), mp1.controlTop.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlT;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlLeft.x(), mp1.controlLeft.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlL;
							found = true;
						}
						gradientPoint = QPointF(mp1.controlRight.x(), mp1.controlRight.y());
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(gradientPoint)))
						{
							m_gradientPoint = useControlR;
							found = true;
						}
					}
				}
				if (found)
				{
					selPoint.first = grow;
					selPoint.second = gcol;
					*m_old_mesh = mp1;
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
					if (m_canvas->hitsCanvasPoint(mousePointDoc, gradientPoint))
					{
						selPoint.first = grow;
						selPoint.second = gcol;
						m_gradientPoint = noPointDefined;
						found = true;
						break;
					}
				}
				if (found)
					break;
			}
		}
	}
	if (!found)
	{
		if (!(m->modifiers() & Qt::ShiftModifier))
		{
			m_gradientPoint = noPointDefined;
			m_selectedMeshPoints.clear();
			currItem->selectedMeshPointX = -1;
			currItem->selectedMeshPointY = -1;
			currItem->selectedMeshControlPoint = static_cast<int>(m_gradientPoint);
		}
	}
	else
	{
		bool isSelected = false;
		for (int se = 0; se < m_selectedMeshPoints.count(); se++)
		{
			if ((selPoint.first == m_selectedMeshPoints[se].first) && (selPoint.second == m_selectedMeshPoints[se].second))
			{
				isSelected = true;
				break;
			}
		}
		if ((!(m->modifiers() & Qt::ShiftModifier)) && (!isSelected))
			m_selectedMeshPoints.clear();
		if (!isSelected)
			m_selectedMeshPoints.append(selPoint);
		currItem->selectedMeshPointX = m_selectedMeshPoints[0].first;
		currItem->selectedMeshPointY = m_selectedMeshPoints[0].second;
		currItem->selectedMeshControlPoint = static_cast<int>(m_gradientPoint);
	}
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_ScMW->propertiesPalette->updateColorSpecialGradient();
	m_view->setCursor(QCursor(Qt::CrossCursor));
	m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, currItem->width(), currItem->height())).adjusted(-currItem->width() / 2.0, -currItem->height() / 2.0, currItem->width(), currItem->height()));
}

void CanvasMode_EditMeshGradient::mouseReleaseEvent(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	if (currItem->selectedMeshPointX >=0 && currItem->selectedMeshPointY >=0 && UndoManager::undoEnabled())
	{
		ScItemState<QPair<meshPoint,meshPoint> > *ss = new ScItemState<QPair<meshPoint,meshPoint> >(Um::GradPos);
		ss->set("MOVE_MESH_PATCH");
		ss->set("ARRAY",true);
		ss->set("X",currItem->selectedMeshPointX);
		ss->set("Y",currItem->selectedMeshPointY);
		if((*m_old_mesh) == currItem->meshGradientArray[currItem->selectedMeshPointX][currItem->selectedMeshPointY])
		{
			delete ss;
			ss=NULL;
		}
		else
			ss->setItem(qMakePair(*m_old_mesh,currItem->meshGradientArray[currItem->selectedMeshPointX][currItem->selectedMeshPointY]));
		if(ss)
			undoManager->action(currItem,ss);
	}
	currItem->update();
	QTransform itemMatrix = currItem->getTransform();
	m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, currItem->width(), currItem->height())).adjusted(-currItem->width() / 2.0, -currItem->height() / 2.0, currItem->width(), currItem->height()));
}
