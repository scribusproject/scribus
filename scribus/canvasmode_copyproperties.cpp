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


#include "canvasmode_copyproperties.h"

#include <QApplication>
#include <QCursor>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>
#include <QDebug>

#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "selectionrubberband.h"
#include "ui/contextmenu.h"
#include "util.h"
#include "util_math.h"


CanvasMode_CopyProperties::CanvasMode_CopyProperties(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
}

inline bool CanvasMode_CopyProperties::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != nullptr;
}

void CanvasMode_CopyProperties::drawControls(QPainter* p)
{
	commonDrawControls(p, false);
}

void CanvasMode_CopyProperties::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_CopyProperties::leaveEvent(QEvent *e)
{
}


void CanvasMode_CopyProperties::activate(bool fromGesture)
{
//	qDebug() << "CanvasMode_CopyProperties::activate" << fromGesture;
	CanvasMode::activate(fromGesture);

	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	frameResizeHandle = -1;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_CopyProperties::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_CopyProperties::deactivate" << forGesture;
	m_view->setRedrawMarkerShown(false);
	CanvasMode::deactivate(forGesture);
}

void CanvasMode_CopyProperties::keyPressEvent(QKeyEvent *e)
{
	commonkeyPressEvent_Default(e);
}

void CanvasMode_CopyProperties::keyReleaseEvent(QKeyEvent *e)
{
	commonkeyReleaseEvent(e);
}

void CanvasMode_CopyProperties::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
//	m_view->stopDragTimer();
}

void CanvasMode_CopyProperties::mouseMoveEvent(QMouseEvent *m)
{
	m->accept();
	if (commonMouseMove(m))
		return;
	if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton))
	{
		QPoint startP = m_canvas->canvasToGlobal(QPointF(Mxp, Myp));
		m_view->redrawMarker->setGeometry(QRect(startP, m->globalPos()).normalized());
		m_view->setRedrawMarkerShown(true);
		m_view->HaveSelRect = true;
		return;
	}
}

void CanvasMode_CopyProperties::mousePressEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
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
	double Rxp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).x();
	Mxp = qRound(Rxp);
	double Ryp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).y();
	Myp = qRound(Ryp);
	if (m->button() == Qt::MiddleButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	if (m->button() != Qt::LeftButton)
		return;
	SeleItem(m);
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		double sx, sy, ex, ey, fx, fy, ss, sk;
		currItem->m_columnGap = m_doc->ElemToLink->m_columnGap;
		currItem->m_columns = m_doc->ElemToLink->m_columns;
		currItem->setTextToFrameDist(m_doc->ElemToLink->textToFrameDistLeft(), m_doc->ElemToLink->textToFrameDistRight(), m_doc->ElemToLink->textToFrameDistTop(), m_doc->ElemToLink->textToFrameDistBottom());
		// Stroke Properties
		currItem->setLineStyle(m_doc->ElemToLink->lineStyle());
		currItem->setLineWidth(m_doc->ElemToLink->lineWidth());
		currItem->setLineTransparency(m_doc->ElemToLink->lineTransparency());
		currItem->setLineShade(m_doc->ElemToLink->lineShade());
		currItem->setLineColor(m_doc->ElemToLink->lineColor());
		currItem->setLineEnd(m_doc->ElemToLink->lineEnd());
		currItem->setLineJoin(m_doc->ElemToLink->lineJoin());
		currItem->setCustomLineStyle(m_doc->ElemToLink->customLineStyle());
		currItem->setEndArrowIndex(m_doc->ElemToLink->endArrowIndex());
		currItem->setStartArrowIndex(m_doc->ElemToLink->startArrowIndex());
		currItem->setEndArrowScale(m_doc->ElemToLink->endArrowScale());
		currItem->setStartArrowScale(m_doc->ElemToLink->startArrowScale());
		// Fill Properties
		currItem->setFillColor(m_doc->ElemToLink->fillColor());
		currItem->setFillShade(m_doc->ElemToLink->fillShade());
		currItem->setFillTransparency(m_doc->ElemToLink->fillTransparency());
		// Gradient Properties
		currItem->fill_gradient = m_doc->ElemToLink->fill_gradient;
		currItem->setGradient(m_doc->ElemToLink->gradient());
		m_doc->ElemToLink->gradientVector(sx, sy, ex, ey, fx, fy, ss, sk);
		currItem->setGradientVector(sx, sy, ex, ey, fx, fy, ss, sk);
		// Pattern Properties
		currItem->setPattern(m_doc->ElemToLink->pattern());
		currItem->setPatternTransform(m_doc->ElemToLink->patternTransform());
		currItem->setStrokePattern(m_doc->ElemToLink->strokePattern());
		currItem->setStrokePatternTransform(m_doc->ElemToLink->strokePatternTransform());
		currItem->setStrokePatternToPath(m_doc->ElemToLink->isStrokePatternToPath());
		// Set Gradient type after all properties
		currItem->setGradientType(m_doc->ElemToLink->gradientType());

		currItem->stroke_gradient = m_doc->ElemToLink->stroke_gradient;
		currItem->setStrokeGradient(m_doc->ElemToLink->strokeGradient());
		m_doc->ElemToLink->strokeGradientVector(sx, sy, ex, ey, fx, fy, ss, sk);
		currItem->setStrokeGradientVector(sx, sy, ex, ey, fx, fy, ss, sk);
		currItem->setStrokeGradientType(m_doc->ElemToLink->strokeGradientType());
		// Update Item
		m_doc->ElemToLink = currItem;
		currItem->update();
//		emit DocChanged();
//		m_view->updateContents();
	}
	else
	{
		m_doc->ElemToLink = nullptr;
		m_view->requestMode(submodePaintingDone);
		m_view->canvasMode()->mousePressEvent(m);
	}
}

void CanvasMode_CopyProperties::mouseReleaseEvent(QMouseEvent *m)
{
#ifdef GESTURE_FRAME_PREVIEW
	clearPixmapCache();
#endif // GESTURE_FRAME_PREVIEW
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
//	m_view->stopDragTimer();
	if ((GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!m_doc->DragP))
	{
		createContextMenu(currItem, mousePointDoc.x(), mousePointDoc.y());
		return;
	}
}

//CB-->Doc/Fix
bool CanvasMode_CopyProperties::SeleItem(QMouseEvent *m)
{
	const unsigned SELECT_IN_GROUP = Qt::AltModifier;
	const unsigned SELECT_MULTIPLE = Qt::ShiftModifier;
	const unsigned SELECT_BENEATH = Qt::ControlModifier;
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	Mxp = mousePointDoc.x(); //m->x()/m_canvas->scale());
	Myp = mousePointDoc.y(); //m->y()/m_canvas->scale());
	int MxpS = static_cast<int>(mousePointDoc.x()); //m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	int MypS = static_cast<int>(mousePointDoc.y()); //m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	m_doc->nodeEdit.deselect();
	if (!m_doc->masterPageMode())
	{
		int pgNum = -1;
		int docPageCount = static_cast<int>(m_doc->Pages->count() - 1);
		MarginStruct pageBleeds;
		bool drawBleed = false;
		if (!m_doc->bleeds()->isNull() && m_doc->guidesPrefs().showBleed)
			drawBleed = true;
		for (int a = docPageCount; a > -1; a--)
		{
			if (drawBleed)
				m_doc->getBleeds(a, pageBleeds);
			int x = static_cast<int>(m_doc->Pages->at(a)->xOffset() - pageBleeds.left());
			int y = static_cast<int>(m_doc->Pages->at(a)->yOffset() - pageBleeds.top());
			int w = static_cast<int>(m_doc->Pages->at(a)->width() + pageBleeds.left() + pageBleeds.right());
			int h = static_cast<int>(m_doc->Pages->at(a)->height() + pageBleeds.bottom() + pageBleeds.top());
			if (QRect(x, y, w, h).contains(MxpS, MypS))
			{
				pgNum = static_cast<int>(a);
				if (drawBleed)  // check again if its really on the correct page
				{
					for (int a2 = docPageCount; a2 > -1; a2--)
					{
						int xn = static_cast<int>(m_doc->Pages->at(a2)->xOffset());
						int yn = static_cast<int>(m_doc->Pages->at(a2)->yOffset());
						int wn = static_cast<int>(m_doc->Pages->at(a2)->width());
						int hn = static_cast<int>(m_doc->Pages->at(a2)->height());
						if (QRect(xn, yn, wn, hn).contains(MxpS, MypS))
						{
							pgNum = static_cast<int>(a2);
							break;
						}
					}
				}
				break;
			}
		}
		if (pgNum >= 0)
		{
			if (m_doc->currentPageNumber() != pgNum)
			{
				m_doc->setCurrentPage(m_doc->Pages->at(pgNum));
				m_view->m_ScMW->slotSetCurrentPage(pgNum);
				m_view->DrawNew();
			}
		}
		m_view->setRulerPos(m_view->contentsX(), m_view->contentsY());
	}
	currItem = nullptr;
	if ((m->modifiers() & SELECT_BENEATH) != 0)
	{
		for (int i=0; i < m_doc->m_Selection->count(); ++i)
		{
			if (m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), m_doc->m_Selection->itemAt(i)) >= 0)
			{
				currItem = m_doc->m_Selection->itemAt(i);
				m_doc->m_Selection->removeItem(currItem);
				break;
			}
		}
	}
	else if ( (m->modifiers() & SELECT_MULTIPLE) == Qt::NoModifier || (m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames) )
	{
		m_view->deselectItems(false);
	}
	currItem = m_canvas->itemUnderCursor(m->globalPos(), currItem, (m->modifiers() & SELECT_IN_GROUP));
	if (currItem)
	{
		m_doc->m_Selection->delaySignalsOn();
		if (m_doc->m_Selection->containsItem(currItem))
		{
			m_doc->m_Selection->removeItem(currItem);
		}
		else
		{
			//CB: If we have a selection but the user clicks with control on another item that is not below the current
			//then clear and select the new item
			if ((m->modifiers() == SELECT_BENEATH) && m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem) >= 0)
				m_doc->m_Selection->clear();
			m_doc->m_Selection->addItem(currItem);
			if ( (m->modifiers() & SELECT_IN_GROUP) && (!currItem->isGroup()))
			{
				currItem->isSingleSel = true;
			}
		}

		currItem->update();
		m_doc->m_Selection->delaySignalsOff();
		if (m_doc->m_Selection->count() > 1)
		{
			for (int aa = 0; aa < m_doc->m_Selection->count(); ++aa)
			{
				PageItem *bb = m_doc->m_Selection->itemAt(aa);
				bb->update();
			}
			double x, y, w, h;
			m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			m_view->getGroupRectScreen(&x, &y, &w, &h);
		}
		if (m_doc->m_Selection->count() == 1)
		{
			frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
			if ((frameResizeHandle == Canvas::INSIDE) && (!currItem->locked()))
				m_view->setCursor(QCursor(Qt::SizeAllCursor));
		}
		else
		{
			m_view->setCursor(QCursor(Qt::SizeAllCursor));
			m_canvas->m_viewMode.operItemResizing = false;
		}
		return true;
	}
	m_doc->m_Selection->connectItemToGUI();
	if ( !(m->modifiers() & SELECT_MULTIPLE) || (m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames))
		m_view->deselectItems(true);
	return false;
}

void CanvasMode_CopyProperties::createContextMenu(PageItem* /*currItem*/, double mx, double my)
{
	ContextMenu* cmen = nullptr;
	m_view->setCursor(QCursor(Qt::ArrowCursor));
	m_view->setObjectUndoMode();
	Mxp = mx;
	Myp = my;
	cmen = new ContextMenu(*(m_doc->m_Selection), m_ScMW, m_doc);
	if (cmen)
		cmen->exec(QCursor::pos());
	m_view->setGlobalUndoMode();
	delete cmen;
}
