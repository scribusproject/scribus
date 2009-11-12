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

#include "ui/aligndistribute.h"
#include "canvas.h"
#include "ui/contextmenu.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hyphenator.h"
//#include "insertTable.h"
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
		drawSelection(p);
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
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
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
		m_canvas->m_viewMode.operItemResizeInEditMode = false;
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
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m->accept();
//	qDebug() << "legacy mode move:" << m->x() << m->y() << m_canvas->globalToCanvas(m->globalPos()).x() << m_canvas->globalToCanvas(m->globalPos()).y();
//	emit MousePos(m->x()/m_canvas->scale(),// + m_doc->minCanvasCoordinate.x(), 
//				  m->y()/m_canvas->scale()); // + m_doc->minCanvasCoordinate.y());

	if (m_gradientPoint == noPointDefined)
		return;

	if (m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		double newX = mousePointDoc.x(); //m->x();
		double newY = mousePointDoc.y(); //m->y();
//		double dx = fabs(Mxp - newX) + 5.0 / m_canvas->scale();
//		double dy = fabs(Myp - newY) + 5.0 / m_canvas->scale();
		FPoint np(Mxp - newX, Myp - newY, 0, 0, currItem->rotation(), 1, 1, true);
		if (m_view->editStrokeGradient)
		{
			if (m_gradientPoint == useGradientStart)
			{
				currItem->GrStrokeStartX -= np.x(); // (Mxp - newX); // / m_canvas->scale();
				currItem->GrStrokeStartY -= np.y(); // (Myp - newY); // / m_canvas->scale();
				currItem->GrStrokeFocalX -= np.x();
				currItem->GrStrokeFocalY -= np.y();
				if (currItem->GrTypeStroke == 7)
				{
					double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
					double radFoc = distance(currItem->GrStrokeFocalX - currItem->GrStrokeStartX, currItem->GrStrokeFocalY - currItem->GrStrokeStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrStrokeStartX += np.x(); // (Mxp - newX); // / m_canvas->scale();
						currItem->GrStrokeStartY += np.y(); // (Myp - newY); // / m_canvas->scale();
						currItem->GrStrokeFocalX += np.x(); // (Mxp - newX); // / m_canvas->scale();
						currItem->GrStrokeFocalY += np.y(); // (Myp - newY); // / m_canvas->scale();
					}
				}
			}
			if (m_gradientPoint == useGradientFocal)
			{
				currItem->GrStrokeFocalX -= np.x();
				currItem->GrStrokeFocalY -= np.y();
				if (currItem->GrTypeStroke == 7)
				{
					double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
					double radFoc = distance(currItem->GrStrokeFocalX - currItem->GrStrokeStartX, currItem->GrStrokeFocalY - currItem->GrStrokeStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrStrokeFocalX += np.x(); // (Mxp - newX); // / m_canvas->scale();
						currItem->GrStrokeFocalY += np.y(); // (Myp - newY); // / m_canvas->scale();
					}
				}
			}
			if (m_gradientPoint == useGradientSkew)
			{
				double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
				double rotEnd = xy2Deg(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
				QTransform m;
				m.translate(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
				m.rotate(rotEnd);
				m.rotate(-90);
				m.rotate(currItem->GrStrokeSkew);
				m.translate(radEnd * currItem->GrStrokeScale, 0);
				QPointF shP = m.map(QPointF(0,0));
				QPointF shR = QPointF(shP.x() -np.x(), shP.y() - np.y());
				double radNew = distance(shR.x() - currItem->GrStrokeStartX, shR.y() - currItem->GrStrokeStartY);
				double rotNew = xy2Deg(shR.x() - currItem->GrStrokeStartX, shR.y() - currItem->GrStrokeStartY) + 90;
				currItem->GrStrokeSkew = rotNew - rotEnd;
				double newScale = radNew / radEnd;
				if ((newScale > 0) && (newScale <= 1))
					currItem->GrStrokeScale = newScale;
			}
			if (m_gradientPoint == useGradientEnd)
			{
				currItem->GrStrokeEndX -= np.x(); // (Mxp - newX); // / m_canvas->scale();
				currItem->GrStrokeEndY -= np.y(); // (Myp - newY); // / m_canvas->scale();
				if (currItem->GrTypeStroke == 7)
				{
					double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
					double radFoc = distance(currItem->GrStrokeFocalX - currItem->GrStrokeStartX, currItem->GrStrokeFocalY - currItem->GrStrokeStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrStrokeEndX += np.x(); // (Mxp - newX); // / m_canvas->scale();
						currItem->GrStrokeEndY += np.y(); // (Myp - newY); // / m_canvas->scale();
					}
				}
			}
		}
		else
		{
			if (m_gradientPoint == useGradientStart)
			{
				currItem->GrStartX -= np.x();
				currItem->GrStartY -= np.y();
				currItem->GrFocalX -= np.x();
				currItem->GrFocalY -= np.y();
				if (currItem->GrType == 7)
				{
					double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
					double radFoc = distance(currItem->GrFocalX - currItem->GrStartX, currItem->GrFocalY - currItem->GrStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrStartX += np.x(); // (Mxp - newX); // / m_canvas->scale();
						currItem->GrStartY += np.y(); // (Myp - newY); // / m_canvas->scale();
						currItem->GrFocalX += np.x(); // (Mxp - newX); // / m_canvas->scale();
						currItem->GrFocalY += np.y(); // (Myp - newY); // / m_canvas->scale();
					}
				}
			}
			if (m_gradientPoint == useGradientFocal)
			{
				currItem->GrFocalX -= np.x();
				currItem->GrFocalY -= np.y();
				if (currItem->GrType == 7)
				{
					double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
					double radFoc = distance(currItem->GrFocalX - currItem->GrStartX, currItem->GrFocalY - currItem->GrStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrFocalX += np.x(); // (Mxp - newX); // / m_canvas->scale();
						currItem->GrFocalY += np.y(); // (Myp - newY); // / m_canvas->scale();
					}
				}
			}
			if (m_gradientPoint == useGradientSkew)
			{
				double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
				double rotEnd = xy2Deg(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
				QTransform m;
				m.translate(currItem->GrStartX, currItem->GrStartY);
				m.rotate(rotEnd);
				m.rotate(-90);
				m.rotate(currItem->GrSkew);
				m.translate(radEnd * currItem->GrScale, 0);
				QPointF shP = m.map(QPointF(0,0));
				QPointF shR = QPointF(shP.x() -np.x(), shP.y() - np.y());
				double radNew = distance(shR.x() - currItem->GrStartX, shR.y() - currItem->GrStartY);
				double rotNew = xy2Deg(shR.x() - currItem->GrStartX, shR.y() - currItem->GrStartY) + 90;
				currItem->GrSkew = rotNew - rotEnd;
				double newScale = radNew / radEnd;
				if ((newScale > 0) && (newScale <= 1))
					currItem->GrScale = newScale;
			}
			if (m_gradientPoint == useGradientEnd)
			{
				currItem->GrEndX -= np.x();
				currItem->GrEndY -= np.y();
				if (currItem->GrType == 7)
				{
					double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
					double radFoc = distance(currItem->GrFocalX - currItem->GrStartX, currItem->GrFocalY - currItem->GrStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrEndX += np.x(); // (Mxp - newX); // / m_canvas->scale();
						currItem->GrEndY += np.y(); // (Myp - newY); // / m_canvas->scale();
					}
				}
			}
		}
		Mxp = newX;
		Myp = newY;
//		m_view->RefreshGradient(currItem, dx * m_canvas->scale(), dy * m_canvas->scale());
		m_ScMW->propertiesPalette->updateColorSpecialGradient();
		currItem->update();
		QRectF upRect;
		if (m_view->editStrokeGradient)
		{
			upRect = QRectF(QPointF(currItem->GrStrokeStartX, currItem->GrStrokeStartY), QPointF(currItem->GrStrokeEndX, currItem->GrStrokeEndY));
			double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
			double rotEnd = xy2Deg(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
			QTransform m;
			m.translate(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
			m.rotate(rotEnd);
			m.rotate(-90);
			m.rotate(currItem->GrStrokeSkew);
			m.translate(radEnd * currItem->GrStrokeScale, 0);
			QPointF shP = m.map(QPointF(0,0));
			upRect |= upRect.united(QRectF(shP, QPointF(currItem->GrStrokeEndX, currItem->GrStrokeEndY)).normalized());
			upRect |= upRect.united(QRectF(shP, QPointF(currItem->GrStrokeStartX, currItem->GrStrokeStartY)).normalized());
			upRect |= QRectF(shP, QPointF(0, 0)).normalized();
			upRect |= QRectF(shP, QPointF(currItem->width(), currItem->height())).normalized();
		}
		else
		{
			upRect = QRectF(QPointF(currItem->GrStartX, currItem->GrStartY), QPointF(currItem->GrEndX, currItem->GrEndY));
			double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
			double rotEnd = xy2Deg(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
			QTransform m;
			m.translate(currItem->GrStartX, currItem->GrStartY);
			m.rotate(rotEnd);
			m.rotate(-90);
			m.rotate(currItem->GrSkew);
			m.translate(radEnd * currItem->GrScale, 0);
			QPointF shP = m.map(QPointF(0,0));
			upRect |= QRectF(shP, QPointF(currItem->GrEndX, currItem->GrEndY)).normalized();
			upRect |= QRectF(shP, QPointF(currItem->GrStartX, currItem->GrStartY)).normalized();
			upRect |= QRectF(shP, QPointF(0, 0)).normalized();
			upRect |= QRectF(shP, QPointF(currItem->width(), currItem->height())).normalized();
		}
		upRect.translate(currItem->xPos(), currItem->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
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
	QTransform itemMatrix;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	itemMatrix.translate(currItem->xPos(), currItem->yPos());
	itemMatrix.rotate(currItem->rotation());
	QPointF gradientStart, gradientEnd, gradientFocal, gradientScale;
	if (m_view->editStrokeGradient)
	{
		gradientStart = QPointF(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
		gradientEnd = QPointF(currItem->GrStrokeEndX, currItem->GrStrokeEndY);
		gradientFocal = QPointF(currItem->GrStrokeFocalX, currItem->GrStrokeFocalY);
		double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
		double rotEnd = xy2Deg(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
		QTransform m;
		m.translate(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(currItem->GrStrokeSkew);
		m.translate(radEnd * currItem->GrStrokeScale, 0);
		gradientScale = m.map(QPointF(0,0));
	}
	else
	{
		gradientStart = QPointF(currItem->GrStartX, currItem->GrStartY);
		gradientEnd = QPointF(currItem->GrEndX, currItem->GrEndY);
		gradientFocal = QPointF(currItem->GrFocalX, currItem->GrFocalY);
		double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
		double rotEnd = xy2Deg(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
		QTransform m;
		m.translate(currItem->GrStartX, currItem->GrStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(currItem->GrSkew);
		m.translate(radEnd * currItem->GrScale, 0);
		gradientScale = m.map(QPointF(0,0));
	}
	gradientStart = itemMatrix.map(gradientStart);
	gradientEnd = itemMatrix.map(gradientEnd);
	gradientFocal = itemMatrix.map(gradientFocal);
	gradientScale = itemMatrix.map(gradientScale);
	if (m_canvas->hitsCanvasPoint(m->globalPos(), gradientStart))
		m_gradientPoint = useGradientStart;
	else if (m_canvas->hitsCanvasPoint(m->globalPos(), gradientEnd))
		m_gradientPoint = useGradientEnd;
	else if (m_canvas->hitsCanvasPoint(m->globalPos(), gradientFocal))
		m_gradientPoint = useGradientFocal;
	else if (m_canvas->hitsCanvasPoint(m->globalPos(), gradientScale))
		m_gradientPoint = useGradientSkew;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
}

void CanvasMode_EditGradient::mouseReleaseEvent(QMouseEvent *m)
{
#ifdef GESTURE_FRAME_PREVIEW
        clearPixmapCache();
#endif // GESTURE_FRAME_PREVIEW
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_gradientPoint = noPointDefined;
//	m_view->stopDragTimer();
}
