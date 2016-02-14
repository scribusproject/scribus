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
				 canvasmode_editmeshpatch.cpp  -  description
							 -------------------
	begin                : Sun Sep 25 2011
	copyright            : (C) 2011 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/


#include "canvasmode_editmeshpatch.h"

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


CanvasMode_EditMeshPatch::CanvasMode_EditMeshPatch(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW)
{
	m_Mxp = m_Myp = -1;
	m_patchPoint = noPointDefined;
	m_gradientPoint = noControlPointDefined;
	m_keyRepeat = false;
	m_click_count = 0;
	m_old_mesh = new meshPoint();
	m_currItem = NULL;
}

CanvasMode_EditMeshPatch::~CanvasMode_EditMeshPatch()
{
	delete m_old_mesh;
	m_old_mesh = NULL;
}

inline bool CanvasMode_EditMeshPatch::GetItem(PageItem** pi)
{
	*pi = m_doc->m_Selection->itemAt(0);
	return (*pi) != NULL;
}

void CanvasMode_EditMeshPatch::drawControls(QPainter* p)
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
	if (m_doc->appMode == modeEditMeshPatch)
	{
		drawControlsMeshPatch(p, m_doc->m_Selection->itemAt(0));
	}
	p->restore();
}

void CanvasMode_EditMeshPatch::drawControlsMeshPoint(QPainter* psx, const meshPoint& mp, bool isSelected)
{
	QPen p8r = QPen(Qt::red, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p8m = QPen(Qt::magenta, 8.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p14r = QPen(Qt::red, 18.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	QPen p14w = QPen(Qt::white, 18.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
	if (m_view->editStrokeGradient == 8)
	{
		if (isSelected)
			psx->setPen(p14r);
		else
			psx->setPen(p14w);
		psx->drawPoint(QPointF(mp.gridPoint.x(), mp.gridPoint.y()));
		psx->setPen(QPen(mp.color, 14.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
		psx->drawPoint(QPointF(mp.gridPoint.x(), mp.gridPoint.y()));
		if (isSelected)
			psx->setPen(p8r);
		else
			psx->setPen(p8m);
		psx->drawPoint(QPointF(mp.controlColor.x(), mp.controlColor.y()));
	}
	else if (m_view->editStrokeGradient == 9)
	{
		if (isSelected)
			psx->setPen(p8r);
		else
			psx->setPen(p8m);
		psx->drawPoint(QPointF(mp.gridPoint.x(), mp.gridPoint.y()));
	}
}

void CanvasMode_EditMeshPatch::drawControlsMeshPatch(QPainter* psx, PageItem* currItem)
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

	for (int col = 0; col < currItem->meshGradientPatches.count(); col++)
	{
		meshGradientPatch patch = currItem->meshGradientPatches[col];
		meshPoint mp1 = patch.TL;
		meshPoint mp2 = patch.TR;
		meshPoint mp3 = patch.BR;
		meshPoint mp4 = patch.BL;
		if (col != currItem->selectedMeshPointX)
		{
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
	if (currItem->selectedMeshPointX >= 0)
	{
		meshGradientPatch patch = currItem->meshGradientPatches[currItem->selectedMeshPointX];
		meshPoint mp1 = patch.TL;
		meshPoint mp2 = patch.TR;
		meshPoint mp3 = patch.BR;
		meshPoint mp4 = patch.BL;
		QPainterPath Bez;
		Bez.moveTo(mp1.gridPoint.x(), mp1.gridPoint.y());
		Bez.cubicTo(mp1.controlRight.x(), mp1.controlRight.y(), mp2.controlLeft.x(), mp2.controlLeft.y(), mp2.gridPoint.x(), mp2.gridPoint.y());
		Bez.cubicTo(mp2.controlBottom.x(), mp2.controlBottom.y(), mp3.controlTop.x(), mp3.controlTop.y(), mp3.gridPoint.x(), mp3.gridPoint.y());
		Bez.cubicTo(mp3.controlLeft.x(), mp3.controlLeft.y(), mp4.controlRight.x(), mp4.controlRight.y(), mp4.gridPoint.x(), mp4.gridPoint.y());
		Bez.cubicTo(mp4.controlTop.x(), mp4.controlTop.y(), mp1.controlBottom.x(), mp1.controlBottom.y(), mp1.gridPoint.x(), mp1.gridPoint.y());
		psx->setPen(QPen(Qt::red, 1.0 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		psx->drawPath(Bez);
		drawControlsMeshPoint(psx, mp1, m_patchPoint == useTL);
		drawControlsMeshPoint(psx, mp2, m_patchPoint == useTR);
		drawControlsMeshPoint(psx, mp3, m_patchPoint == useBR);
		drawControlsMeshPoint(psx, mp4, m_patchPoint == useBL);
		if (m_view->editStrokeGradient == 10)
		{
			if (m_patchPoint == useTL)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				psx->drawLine(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()), QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
				if (m_gradientPoint == useControlB)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()));
				if (m_gradientPoint == useControlR)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp1.controlRight.x(), mp1.controlRight.y()));
			}
			else if (m_patchPoint == useTR)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp2.gridPoint.x(), mp2.gridPoint.y()), QPointF(mp2.controlBottom.x(), mp2.controlBottom.y()));
				psx->drawLine(QPointF(mp2.gridPoint.x(), mp2.gridPoint.y()), QPointF(mp2.controlLeft.x(), mp2.controlLeft.y()));
				if (m_gradientPoint == useControlL)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp2.controlLeft.x(), mp2.controlLeft.y()));
				if (m_gradientPoint == useControlB)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp2.controlBottom.x(), mp2.controlBottom.y()));
			}
			else if (m_patchPoint == useBR)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp3.gridPoint.x(), mp3.gridPoint.y()), QPointF(mp3.controlTop.x(), mp3.controlTop.y()));
				psx->drawLine(QPointF(mp3.gridPoint.x(), mp3.gridPoint.y()), QPointF(mp3.controlLeft.x(), mp3.controlLeft.y()));
				if (m_gradientPoint == useControlL)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp3.controlLeft.x(), mp3.controlLeft.y()));
				if (m_gradientPoint == useControlT)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp3.controlTop.x(), mp3.controlTop.y()));
			}
			else if (m_patchPoint == useBL)
			{
				psx->setPen(p1bd);
				psx->drawLine(QPointF(mp4.gridPoint.x(), mp4.gridPoint.y()), QPointF(mp4.controlTop.x(), mp4.controlTop.y()));
				psx->drawLine(QPointF(mp4.gridPoint.x(), mp4.gridPoint.y()), QPointF(mp4.controlRight.x(), mp4.controlRight.y()));
				if (m_gradientPoint == useControlR)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp4.controlRight.x(), mp4.controlRight.y()));
				if (m_gradientPoint == useControlT)
					psx->setPen(p8r);
				else
					psx->setPen(p8m);
				psx->drawPoint(QPointF(mp4.controlTop.x(), mp4.controlTop.y()));
			}
		}
	}
	if (m_view->editStrokeGradient == 11)
	{
		if (m_clickPointPolygon.size() > 1)
		{
			psx->setPen(p1b);
			psx->drawPolyline(m_clickPointPolygon);
		}
		psx->setPen(p8r);
		for (int col = 0; col < m_clickPointPolygon.size(); col++)
		{
			psx->drawPoint(m_clickPointPolygon.value(col));
		}
		if (m_clickPointPolygon.size() > 0)
		{
			psx->setPen(p1bd);
			psx->drawLine(m_clickPointPolygon.value(m_clickPointPolygon.count() - 1), m_currentPoint);
			if (m_clickPointPolygon.size() == 3)
				psx->drawLine(m_clickPointPolygon.value(0), m_currentPoint);
		}
		psx->setPen(p8r);
		psx->drawPoint(m_currentPoint);
	}
}

void CanvasMode_EditMeshPatch::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_EditMeshPatch::leaveEvent(QEvent *e)
{
}


void CanvasMode_EditMeshPatch::activate(bool fromGesture)
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
	m_Mxp = m_Myp = m_currItem->selectedMeshPointX = -1;
	m_click_count = 0;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
	m_currItem->snapToPatchGrid = false;
}

void CanvasMode_EditMeshPatch::deactivate(bool forGesture)
{
	m_view->setRedrawMarkerShown(false);
	m_patchPoint = noPointDefined;
	m_gradientPoint = noControlPointDefined;
	m_click_count = 0;
	m_currItem->selectedMeshPointX = -1;
	m_currItem->selectedMeshPointY = m_patchPoint;
	m_currItem->selectedMeshControlPoint = m_gradientPoint;
	m_ScMW->propertiesPalette->updateColorSpecialGradient();
	m_currItem->snapToPatchGrid = false;
}

void CanvasMode_EditMeshPatch::keyPressEvent(QKeyEvent *e)
{
	if (m_currItem->selectedMeshPointX < 0)
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
					if (m_view->editStrokeGradient == 8)
					{
						if (m_patchPoint == useTL)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.controlColor = currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.gridPoint;
						if (m_patchPoint == useTR)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.controlColor = currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.gridPoint;
						if (m_patchPoint == useBR)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.controlColor = currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.gridPoint;
						if (m_patchPoint == useBL)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.controlColor = currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.gridPoint;
						doUpdate = true;
					}
					else if (m_view->editStrokeGradient == 10)
					{
						if (m_patchPoint == useTL)
						{
							if (m_gradientPoint == useControlB)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.controlBottom = currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.gridPoint;
							else if (m_gradientPoint == useControlR)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.controlRight = currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.gridPoint;
						}
						else if (m_patchPoint == useTR)
						{
							if (m_gradientPoint == useControlB)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.controlBottom = currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.gridPoint;
							else if (m_gradientPoint == useControlL)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.controlLeft = currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.gridPoint;
						}
						else if (m_patchPoint == useBR)
						{
							if (m_gradientPoint == useControlT)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.controlTop = currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.gridPoint;
							else if (m_gradientPoint == useControlL)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.controlLeft = currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.gridPoint;
						}
						else if (m_patchPoint == useBL)
						{
							if (m_gradientPoint == useControlT)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.controlTop = currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.gridPoint;
							else if (m_gradientPoint == useControlR)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.controlRight = currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.gridPoint;
						}
						doUpdate = true;
					}
					break;
			}
			if (isMoving)
			{
				if (m_patchPoint != noPointDefined)
				{
					if (m_view->editStrokeGradient == 8)
					{
						if (m_patchPoint == useTL)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.controlColor += FPoint(moveX, moveY);
						if (m_patchPoint == useTR)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.controlColor += FPoint(moveX, moveY);
						if (m_patchPoint == useBR)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.controlColor += FPoint(moveX, moveY);
						if (m_patchPoint == useBL)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.controlColor += FPoint(moveX, moveY);
					}
					else if (m_view->editStrokeGradient == 9)
					{
						if (m_patchPoint == useTL)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.moveRel(moveX, moveY);
						if (m_patchPoint == useTR)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.moveRel(moveX, moveY);
						if (m_patchPoint == useBR)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.moveRel(moveX, moveY);
						if (m_patchPoint == useBL)
							currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.moveRel(moveX, moveY);
					}
					else if (m_view->editStrokeGradient == 10)
					{
						if (m_patchPoint == useTL)
						{
							if (m_gradientPoint == useControlB)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.controlBottom += FPoint(moveX, moveY);
							else if (m_gradientPoint == useControlR)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].TL.controlRight += FPoint(moveX, moveY);
						}
						else if (m_patchPoint == useTR)
						{
							if (m_gradientPoint == useControlB)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.controlBottom += FPoint(moveX, moveY);
							else if (m_gradientPoint == useControlL)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].TR.controlLeft += FPoint(moveX, moveY);
						}
						else if (m_patchPoint == useBR)
						{
							if (m_gradientPoint == useControlT)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.controlTop += FPoint(moveX, moveY);
							else if (m_gradientPoint == useControlL)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].BR.controlLeft += FPoint(moveX, moveY);
						}
						else if (m_patchPoint == useBL)
						{
							if (m_gradientPoint == useControlT)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.controlTop += FPoint(moveX, moveY);
							else if (m_gradientPoint == useControlR)
								currItem->meshGradientPatches[currItem->selectedMeshPointX].BL.controlRight += FPoint(moveX, moveY);
						}
					}
				}
			}
			if (doUpdate)
			{
				currItem->update();
				QTransform itemMatrix = currItem->getTransform();
				m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, currItem->width(), currItem->height())).adjusted(-currItem->width() / 2.0, -currItem->height() / 2.0, currItem->width(), currItem->height()));
			}
		}
	}
	m_keyRepeat = false;
}

void CanvasMode_EditMeshPatch::mouseDoubleClickEvent(QMouseEvent *m)
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

void CanvasMode_EditMeshPatch::mouseMoveEvent(QMouseEvent *m)
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
	QTransform pp = m_currItem->getTransform();
	FPoint npf = npfN.transformPoint(pp, true);
	FPoint npx(m_Mxp - npfN.x(), m_Myp - npfN.y(), 0, 0, m_currItem->rotation(), 1, 1, true);
	if (m_view->editStrokeGradient == 11)
	{
		m_currentPoint = QPointF(npf.x(), npf.y());
		m_canvas->displayXYHUD(m->globalPos(), npf.x(), npf.y());
		m_currItem->update();
		QRectF upRect;
		upRect = QRectF(QPointF(0, 0), QPointF(m_currItem->width(), m_currItem->height())).normalized();
		upRect.translate(m_currItem->xPos(), m_currItem->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0 - m_currItem->width() / 2.0, -10.0 - m_currItem->height() / 2.0, 10.0 + m_currItem->width() / 2.0, 10.0 + m_currItem->height() / 2.0));
	}
	if (m_canvas->m_viewMode.m_MouseButtonPressed && m_view->moveTimerElapsed())
	{
		if (m_currItem->selectedMeshPointX >= 0)
		{
			if (m_patchPoint != noPointDefined)
			{
				if (m_view->editStrokeGradient == 8)
				{
					FPoint cP;
					FPoint gP;
					if (m_patchPoint == useTL)
					{
						cP = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TL.controlColor;
						gP = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TL.gridPoint;
					}
					if (m_patchPoint == useTR)
					{
						cP = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TR.controlColor;
						gP = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TR.gridPoint;
					}
					if (m_patchPoint == useBR)
					{
						cP = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BR.controlColor;
						gP = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BR.gridPoint;
					}
					if (m_patchPoint == useBL)
					{
						cP = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BL.controlColor;
						gP = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BL.gridPoint;
					}
					m_canvas->displayXYHUD(m->globalPos(), cP.x() - gP.x(), cP.y() - gP.y());
					if (m_patchPoint == useTL)
						m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TL.controlColor -= npx;
					if (m_patchPoint == useTR)
						m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TR.controlColor -= npx;
					if (m_patchPoint == useBR)
						m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BR.controlColor -= npx;
					if (m_patchPoint == useBL)
						m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BL.controlColor -= npx;
				}
				else if (m_view->editStrokeGradient == 9)
				{
					m_canvas->displayXYHUD(m->globalPos(), npf.x(), npf.y());
					if (m_view->editStrokeGradient == 9)
					{
						FPoint mp, mp_orig;
						double xx, yy;
						if (m_patchPoint == useTL)
						{
							mp_orig = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TL.gridPoint;
							mp = mp_orig - npx;
							xx = mp.x();
							yy = mp.y();
							if (m_currItem->snapToPatchGrid)
								snapToOtherPatch(xx, yy);
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TL.moveAbs(xx, yy);
						}
						if (m_patchPoint == useTR)
						{
							mp_orig = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TR.gridPoint;
							mp = mp_orig - npx;
							xx = mp.x();
							yy = mp.y();
							if (m_currItem->snapToPatchGrid)
								snapToOtherPatch(xx, yy);
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TR.moveAbs(xx, yy);
						}
						if (m_patchPoint == useBR)
						{
							mp_orig = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BR.gridPoint;
							mp = mp_orig - npx;
							xx = mp.x();
							yy = mp.y();
							if (m_currItem->snapToPatchGrid)
								snapToOtherPatch(xx, yy);
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BR.moveAbs(xx, yy);
						}
						if (m_patchPoint == useBL)
						{
							mp_orig = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BL.gridPoint;
							mp = mp_orig - npx;
							xx = mp.x();
							yy = mp.y();
							if (m_currItem->snapToPatchGrid)
								snapToOtherPatch(xx, yy);
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BL.moveAbs(xx, yy);
						}
						if (m->modifiers() & Qt::ShiftModifier)
						{
							for (int col = 0; col < m_currItem->meshGradientPatches.count(); col++)
							{
								if (col != m_currItem->selectedMeshPointX)
								{
									if (m_currItem->meshGradientPatches[col].TL.gridPoint == mp_orig)
										m_currItem->meshGradientPatches[col].TL.moveAbs(xx, yy);
									if (m_currItem->meshGradientPatches[col].TR.gridPoint == mp_orig)
										m_currItem->meshGradientPatches[col].TR.moveAbs(xx, yy);
									if (m_currItem->meshGradientPatches[col].BR.gridPoint == mp_orig)
										m_currItem->meshGradientPatches[col].BR.moveAbs(xx, yy);
									if (m_currItem->meshGradientPatches[col].BL.gridPoint == mp_orig)
										m_currItem->meshGradientPatches[col].BL.moveAbs(xx, yy);
								}
							}
						}
					}
				}
				else if (m_view->editStrokeGradient == 10)
				{
					if (m_patchPoint == useTL)
					{
						if (m_gradientPoint == useControlB)
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TL.controlBottom -= npx;
						else if (m_gradientPoint == useControlR)
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TL.controlRight -= npx;
					}
					else if (m_patchPoint == useTR)
					{
						if (m_gradientPoint == useControlB)
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TR.controlBottom -= npx;
						else if (m_gradientPoint == useControlL)
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TR.controlLeft -= npx;
					}
					else if (m_patchPoint == useBR)
					{
						if (m_gradientPoint == useControlT)
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BR.controlTop -= npx;
						else if (m_gradientPoint == useControlL)
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BR.controlLeft -= npx;
					}
					else if (m_patchPoint == useBL)
					{
						if (m_gradientPoint == useControlT)
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BL.controlTop -= npx;
						else if (m_gradientPoint == useControlR)
							m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BL.controlRight -= npx;
					}
				}
				m_currItem->update();
				QTransform itemMatrix = m_currItem->getTransform();
				m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, m_currItem->width(), m_currItem->height())).adjusted(-m_currItem->width() / 2.0, -m_currItem->height() / 2.0, m_currItem->width(), m_currItem->height()));
			}
		}
	}
	m_Mxp = npfN.x();
	m_Myp = npfN.y();
}

void CanvasMode_EditMeshPatch::mousePressEvent(QMouseEvent *m)
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
	m_currItem = m_doc->m_Selection->itemAt(0);
	QTransform itemMatrix = m_currItem->getTransform();
	bool found = false;
	if (m_view->editStrokeGradient == 11)
	{
		m_gradientPoint = noControlPointDefined;
		m_patchPoint = noPointDefined;
		if (m_click_count == 0)
			m_clickPointPolygon.clear();
		m_currItem->selectedMeshPointX = -1;
		QTransform invItemMatrix = itemMatrix.inverted();
		QPointF clickPoint = invItemMatrix.map(QPointF(mousePointDoc.x(), mousePointDoc.y()));
		m_clickPointPolygon.append(clickPoint);
		m_click_count++;
		if (m_click_count > 3)
		{
			const ScColor& col = m_doc->PageColors["Black"];
			QColor qcol = ScColorEngine::getRGBColor(col, m_doc);
			meshPoint mgP;
			meshGradientPatch patch;
			mgP.resetTo(FPoint(m_clickPointPolygon.value(0).x(), m_clickPointPolygon.value(0).y()));
			mgP.transparency = 1.0;
			mgP.shade = 100;
			mgP.colorName = "Black";
			mgP.color = qcol;
			patch.TL = mgP;
			mgP.resetTo(FPoint(m_clickPointPolygon.value(1).x(), m_clickPointPolygon.value(1).y()));
			patch.TR = mgP;
			mgP.resetTo(FPoint(m_clickPointPolygon.value(2).x(), m_clickPointPolygon.value(2).y()));
			patch.BR = mgP;
			mgP.resetTo(FPoint(m_clickPointPolygon.value(3).x(), m_clickPointPolygon.value(3).y()));
			patch.BL = mgP;
			m_currItem->meshGradientPatches.append(patch);
			m_currItem->selectedMeshPointX = m_currItem->meshGradientPatches.count() - 1;
			m_view->editStrokeGradient = 9;
			m_click_count = 0;
			m_clickPointPolygon.clear();
			m_ScMW->propertiesPalette->endPatchAdd();
		}
	}
	else
	{
		if (m_currItem->selectedMeshPointX >= 0)
		{
			if (m_view->editStrokeGradient == 8)
			{
				meshGradientPatch patch = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX];
				meshPoint mp1 = patch.TL;
				meshPoint mp2 = patch.TR;
				meshPoint mp3 = patch.BR;
				meshPoint mp4 = patch.BL;
				if ((m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y())))) || (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp1.controlColor.x(), mp1.controlColor.y())))))
				{
					m_patchPoint = useTL;
					*m_old_mesh = mp1;
				}
				else if ((m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp2.gridPoint.x(), mp2.gridPoint.y())))) || (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp2.controlColor.x(), mp2.controlColor.y())))))
				{
					*m_old_mesh = mp2;
					m_patchPoint = useTR;
				}
				else if ((m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp3.gridPoint.x(), mp3.gridPoint.y())))) || (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp3.controlColor.x(), mp3.controlColor.y())))))
				{
					*m_old_mesh = mp3;
					m_patchPoint = useBR;
				}
				else if ((m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp4.gridPoint.x(), mp4.gridPoint.y())))) || (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp4.controlColor.x(), mp4.controlColor.y())))))
				{
					*m_old_mesh = mp4;
					m_patchPoint = useBL;
				}
				else
					m_patchPoint = noPointDefined;
			}
			else if (m_view->editStrokeGradient == 9)
			{
				meshGradientPatch patch = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX];
				meshPoint mp1 = patch.TL;
				meshPoint mp2 = patch.TR;
				meshPoint mp3 = patch.BR;
				meshPoint mp4 = patch.BL;
				if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp1.gridPoint.x(), mp1.gridPoint.y()))))
				{
					*m_old_mesh = mp1;
					m_patchPoint = useTL;
				}
				else if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp2.gridPoint.x(), mp2.gridPoint.y()))))
				{
					*m_old_mesh = mp2;
					m_patchPoint = useTR;
				}
				else if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp3.gridPoint.x(), mp3.gridPoint.y()))))
				{
					*m_old_mesh = mp3;
					m_patchPoint = useBR;
				}
				else if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp4.gridPoint.x(), mp4.gridPoint.y()))))
				{
					*m_old_mesh = mp4;
					m_patchPoint = useBL;
				}
				else
					m_patchPoint = noPointDefined;
			}
			else if (m_view->editStrokeGradient == 10)
			{
				if (m_patchPoint != noPointDefined)
				{
					meshGradientPatch patch = m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX];
					meshPoint mp1 = patch.TL;
					meshPoint mp2 = patch.TR;
					meshPoint mp3 = patch.BR;
					meshPoint mp4 = patch.BL;
					if (m_patchPoint == useTL)
					{
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp1.controlBottom.x(), mp1.controlBottom.y()))))
							m_gradientPoint = useControlB;
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp1.controlRight.x(), mp1.controlRight.y()))))
							m_gradientPoint = useControlR;
					}
					else if (m_patchPoint == useTR)
					{
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp2.controlBottom.x(), mp2.controlBottom.y()))))
							m_gradientPoint = useControlB;
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp2.controlLeft.x(), mp2.controlLeft.y()))))
							m_gradientPoint = useControlL;
					}
					else if (m_patchPoint == useBR)
					{
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp3.controlTop.x(), mp3.controlTop.y()))))
							m_gradientPoint = useControlT;
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp3.controlLeft.x(), mp3.controlLeft.y()))))
							m_gradientPoint = useControlL;
					}
					else if (m_patchPoint == useBL)
					{
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp4.controlTop.x(), mp4.controlTop.y()))))
							m_gradientPoint = useControlT;
						if (m_canvas->hitsCanvasPoint(mousePointDoc, itemMatrix.map(QPointF(mp4.controlRight.x(), mp4.controlRight.y()))))
							m_gradientPoint = useControlR;
					}
					else
						m_gradientPoint = noControlPointDefined;
				}
			}
			if (m_patchPoint == noPointDefined)
			{
				for (int col = 0; col < m_currItem->meshGradientPatches.count(); col++)
				{
					meshGradientPatch patch = m_currItem->meshGradientPatches[col];
					meshPoint mp1 = patch.TL;
					meshPoint mp2 = patch.TR;
					meshPoint mp3 = patch.BR;
					meshPoint mp4 = patch.BL;
					QPainterPath Bez;
					Bez.moveTo(mp1.gridPoint.x(), mp1.gridPoint.y());
					Bez.cubicTo(mp1.controlRight.x(), mp1.controlRight.y(), mp2.controlLeft.x(), mp2.controlLeft.y(), mp2.gridPoint.x(), mp2.gridPoint.y());
					Bez.cubicTo(mp2.controlBottom.x(), mp2.controlBottom.y(), mp3.controlTop.x(), mp3.controlTop.y(), mp3.gridPoint.x(), mp3.gridPoint.y());
					Bez.cubicTo(mp3.controlLeft.x(), mp3.controlLeft.y(), mp4.controlRight.x(), mp4.controlRight.y(), mp4.gridPoint.x(), mp4.gridPoint.y());
					Bez.cubicTo(mp4.controlTop.x(), mp4.controlTop.y(), mp1.controlBottom.x(), mp1.controlBottom.y(), mp1.gridPoint.x(), mp1.gridPoint.y());
					QPainterPath pat = itemMatrix.map(Bez);
					if (pat.contains(QPointF(mousePointDoc.x(), mousePointDoc.y())))
					{
						m_currItem->selectedMeshPointX = col;
						found = true;
						break;
					}
				}
				m_gradientPoint = noControlPointDefined;
				if (!found)
					m_currItem->selectedMeshPointX = -1;
			}
		}
		else
		{
			for (int col = 0; col < m_currItem->meshGradientPatches.count(); col++)
			{
				meshGradientPatch patch = m_currItem->meshGradientPatches[col];
				meshPoint mp1 = patch.TL;
				meshPoint mp2 = patch.TR;
				meshPoint mp3 = patch.BR;
				meshPoint mp4 = patch.BL;
				QPainterPath Bez;
				Bez.moveTo(mp1.gridPoint.x(), mp1.gridPoint.y());
				Bez.cubicTo(mp1.controlRight.x(), mp1.controlRight.y(), mp2.controlLeft.x(), mp2.controlLeft.y(), mp2.gridPoint.x(), mp2.gridPoint.y());
				Bez.cubicTo(mp2.controlBottom.x(), mp2.controlBottom.y(), mp3.controlTop.x(), mp3.controlTop.y(), mp3.gridPoint.x(), mp3.gridPoint.y());
				Bez.cubicTo(mp3.controlLeft.x(), mp3.controlLeft.y(), mp4.controlRight.x(), mp4.controlRight.y(), mp4.gridPoint.x(), mp4.gridPoint.y());
				Bez.cubicTo(mp4.controlTop.x(), mp4.controlTop.y(), mp1.controlBottom.x(), mp1.controlBottom.y(), mp1.gridPoint.x(), mp1.gridPoint.y());
				QPainterPath pat = itemMatrix.map(Bez);
				if (pat.contains(QPointF(mousePointDoc.x(), mousePointDoc.y())))
				{
					m_currItem->selectedMeshPointX = col;
					found = true;
					break;
				}
			}
			m_patchPoint = noPointDefined;
			m_gradientPoint = noControlPointDefined;
			if (!found)
				m_currItem->selectedMeshPointX = -1;
		}
		m_currItem->selectedMeshPointY = m_patchPoint;
		m_currItem->selectedMeshControlPoint = m_gradientPoint;
	}
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_ScMW->propertiesPalette->updateColorSpecialGradient();
	m_view->setCursor(QCursor(Qt::CrossCursor));
	m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, m_currItem->width(), m_currItem->height())).adjusted(-m_currItem->width() / 2.0, -m_currItem->height() / 2.0, m_currItem->width(), m_currItem->height()));

}

void CanvasMode_EditMeshPatch::mouseReleaseEvent(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	if (m_view->editStrokeGradient != 11 && m_currItem->selectedMeshPointX >=0 && m_patchPoint != noPointDefined && UndoManager::undoEnabled())
	{
		ScItemState<QPair<meshPoint,meshPoint> > *ss = new ScItemState<QPair<meshPoint,meshPoint> >(Um::GradPos);
		ss->set("MOVE_MESH_PATCH");
		ss->set("X",m_currItem->selectedMeshPointX);
		ss->set("Y",m_currItem->selectedMeshPointY);
		ss->set("ARRAY",false);
		switch(m_currItem->selectedMeshPointY){
			case 1:
				if((*m_old_mesh) == m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TL)
				{
					delete ss;
					ss=NULL;
				}
				else
					ss->setItem(qMakePair(*m_old_mesh,m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TL));
				break;
			case 2:
				if((*m_old_mesh) == m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TR)
				{
					delete ss;
					ss=NULL;
				}
				else
					ss->setItem(qMakePair(*m_old_mesh,m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].TR));
				break;
			case 3:
				if((*m_old_mesh) == m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BR)
				{
					delete ss;
					ss=NULL;
				}
				else
					ss->setItem(qMakePair(*m_old_mesh,m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BR));
				break;
			case 4:
				if((*m_old_mesh) == m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BL)
				{
					delete ss;
					ss=NULL;
				}
				else
					ss->setItem(qMakePair(*m_old_mesh,m_currItem->meshGradientPatches[m_currItem->selectedMeshPointX].BL));
				break;
		}
		if(ss)
			undoManager->action(m_currItem,ss);
	}
	m_currItem = m_doc->m_Selection->itemAt(0);
	m_currItem->update();
	QTransform itemMatrix = m_currItem->getTransform();
	m_doc->regionsChanged()->update(itemMatrix.mapRect(QRectF(0, 0, m_currItem->width(), m_currItem->height())).adjusted(-m_currItem->width() / 2.0, -m_currItem->height() / 2.0, m_currItem->width(), m_currItem->height()));
}

void CanvasMode_EditMeshPatch::snapToOtherPatch(double &x, double &y)
{
	int radius = m_doc->guidesPrefs().grabRadius;
	for (int col = 0; col < m_currItem->meshGradientPatches.count(); col++)
	{
		if (col != m_currItem->selectedMeshPointX)
		{
			meshGradientPatch patch = m_currItem->meshGradientPatches[col];
			QPointF mp1 = QPointF(patch.TL.gridPoint.x(), patch.TL.gridPoint.y());
			QPointF mp2 = QPointF(patch.TR.gridPoint.x(), patch.TR.gridPoint.y());
			QPointF mp3 = QPointF(patch.BR.gridPoint.x(), patch.BR.gridPoint.y());
			QPointF mp4 = QPointF(patch.BL.gridPoint.x(), patch.BL.gridPoint.y());
			if (qAbs(mp1.x() - x) < radius && qAbs(mp1.y() - y) < radius)
			{
				x = mp1.x();
				y = mp1.y();
				return;
			}
			if (qAbs(mp2.x() - x) < radius && qAbs(mp2.y() - y) < radius)
			{
				x = mp2.x();
				y = mp2.y();
				return;
			}
			if (qAbs(mp3.x() - x) < radius && qAbs(mp3.y() - y) < radius)
			{
				x = mp3.x();
				y = mp3.y();
				return;
			}
			if (qAbs(mp4.x() - x) < radius && qAbs(mp4.y() - y) < radius)
			{
				x = mp4.x();
				y = mp4.y();
				return;
			}
		}
	}
}
