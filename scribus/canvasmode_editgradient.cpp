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

#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/contextmenu.h"
#include "undomanager.h"
#include "util_math.h"
#include "util_gui.h"


CanvasMode_EditGradient::CanvasMode_EditGradient(ScribusView* view) : CanvasMode(view)
	// , m_ScMW(view->m_ScMW)
{

}

inline bool CanvasMode_EditGradient::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0);
	return (*pi) != nullptr;
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
	if (m_doc->appMode == modeEditGradientVectors)
	{
		drawControlsGradientVectors(p, m_doc->m_Selection->itemAt(0));
	}
}

void CanvasMode_EditGradient::drawControlsGradientVectors(QPainter* psx, PageItem *currItem)
{
	psx->setTransform(currItem->getTransform(), true);
	psx->setRenderHint(QPainter::Antialiasing);
	psx->setPen(pens().value("vector"));
	psx->setBrush(Qt::NoBrush);
	if (m_view->editStrokeGradient == GradientEdit::Gradient_Stroke)
	{
		// Draw Start + End Nodes
		psx->drawLine(QPointF(currItem->GrStrokeStartX, currItem->GrStrokeStartY), QPointF(currItem->GrStrokeEndX, currItem->GrStrokeEndY));
		drawNodeHandle(psx, QPointF(currItem->GrStrokeStartX, currItem->GrStrokeStartY), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrStrokeEndX, currItem->GrStrokeEndY), pens().value("node"), m_canvas->m_viewMode.scale);

		double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
		double rotEnd = xy2Deg(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);

		// Draw Focal Control
		if (currItem->GrTypeStroke == Gradient_Radial)
		{
			QTransform qmatrix;
			qmatrix.translate(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
			qmatrix.rotate(rotEnd);
			double mask_gradientSkew = 0.0;
			if (currItem->GrStrokeSkew == 90)
				mask_gradientSkew = 1;
			else if (currItem->GrStrokeSkew == 180)
				mask_gradientSkew = 0;
			else if (currItem->GrStrokeSkew == 270)
				mask_gradientSkew = -1;
			else if (currItem->GrStrokeSkew == 360)
				mask_gradientSkew = 0;
			else
				mask_gradientSkew = tan(M_PI / 180.0 * currItem->GrStrokeSkew);
			qmatrix.shear(mask_gradientSkew, 0);
			qmatrix.translate(0, currItem->GrStrokeStartY * (1.0 - currItem->GrStrokeScale));
			qmatrix.translate(-currItem->GrStrokeStartX, -currItem->GrStrokeStartY);
			qmatrix.scale(1, currItem->GrStrokeScale);

			QPointF focal = qmatrix.map(QPointF(currItem->GrStrokeFocalX, currItem->GrStrokeFocalY));
			psx->setPen(pens().value("node-dash"));
			psx->drawLine(QPointF(currItem->GrStrokeStartX, currItem->GrStrokeStartY), focal);
			drawNodeControl(psx, focal, pens().value("node"), m_canvas->m_viewMode.scale);
		}

		// Draw Shear Control
		QTransform m;
		m.translate(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(currItem->GrStrokeSkew);
		m.translate(radEnd * currItem->GrStrokeScale, 0);
		QPointF shP = m.map(QPointF(0,0));
		psx->setPen(pens().value("node-handle"));
		psx->drawLine(QPointF(currItem->GrStrokeStartX, currItem->GrStrokeStartY), shP);
		drawNodeControl(psx, shP, pens().value("node"), m_canvas->m_viewMode.scale);

	}
	else if (m_view->editStrokeGradient == GradientEdit::Gradient_Mask)
	{
		// Draw Start + End Nodes
		psx->drawLine(QPointF(currItem->GrMaskStartX, currItem->GrMaskStartY), QPointF(currItem->GrMaskEndX, currItem->GrMaskEndY));
		drawNodeHandle(psx, QPointF(currItem->GrMaskStartX, currItem->GrMaskStartY), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrMaskEndX, currItem->GrMaskEndY), pens().value("node"), m_canvas->m_viewMode.scale);

		double radEnd = distance(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
		double rotEnd = xy2Deg(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);

		// Draw Focal Control
		if ((currItem->GrMask == GradMask_Radial) || (currItem->GrMask == GradMask_RadialLumAlpha))
		{
			QTransform qmatrix;
			qmatrix.translate(currItem->GrMaskStartX, currItem->GrMaskStartY);
			qmatrix.rotate(rotEnd);
			double mask_gradientSkew = 0.0;
			if (currItem->GrMaskSkew == 90)
				mask_gradientSkew = 1;
			else if (currItem->GrMaskSkew == 180)
				mask_gradientSkew = 0;
			else if (currItem->GrMaskSkew == 270)
				mask_gradientSkew = -1;
			else if (currItem->GrMaskSkew == 360)
				mask_gradientSkew = 0;
			else
				mask_gradientSkew = tan(M_PI / 180.0 * currItem->GrMaskSkew);
			qmatrix.shear(mask_gradientSkew, 0);
			qmatrix.translate(0, currItem->GrMaskStartY * (1.0 - currItem->GrMaskScale));
			qmatrix.translate(-currItem->GrMaskStartX, -currItem->GrMaskStartY);
			qmatrix.scale(1, currItem->GrMaskScale);

			QPointF focal = qmatrix.map(QPointF(currItem->GrMaskFocalX, currItem->GrMaskFocalY));
			psx->setPen(pens().value("node-dash"));
			psx->drawLine(QPointF(currItem->GrMaskStartX, currItem->GrMaskStartY), focal);
			drawNodeControl(psx, focal, pens().value("node"), m_canvas->m_viewMode.scale);
		}
		// Draw Shear Control
		QTransform m;
		m.translate(currItem->GrMaskStartX, currItem->GrMaskStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(currItem->GrMaskSkew);
		m.translate(radEnd * currItem->GrMaskScale, 0);
		QPointF shP = m.map(QPointF(0,0));
		psx->setPen(pens().value("node-handle"));
		psx->drawLine(QPointF(currItem->GrMaskStartX, currItem->GrMaskStartY), shP);
		drawNodeControl(psx, shP, pens().value("node"), m_canvas->m_viewMode.scale);
	}
	else if (m_view->editStrokeGradient == GradientEdit::Gradient)
	{
		// Draw Start + End Nodes
		psx->drawLine(QPointF(currItem->GrStartX, currItem->GrStartY), QPointF(currItem->GrEndX, currItem->GrEndY));
		drawNodeHandle(psx, QPointF(currItem->GrStartX, currItem->GrStartY), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrEndX, currItem->GrEndY), pens().value("node"), m_canvas->m_viewMode.scale);

		double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
		double rotEnd = xy2Deg(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);

		// Draw Focal Control
		if ((currItem->GrType == Gradient_Radial) || (currItem->GrType == Gradient_Conical))
		{
			QTransform qmatrix;
			qmatrix.translate(currItem->GrStartX, currItem->GrStartY);
			qmatrix.rotate(rotEnd);
			if (currItem->GrType != Gradient_Conical)
			{
				double mask_gradientSkew = 0.0;
				if (currItem->GrSkew == 90)
					mask_gradientSkew = 1;
				else if (currItem->GrSkew == 180)
					mask_gradientSkew = 0;
				else if (currItem->GrSkew == 270)
					mask_gradientSkew = -1;
				else if (currItem->GrSkew == 360)
					mask_gradientSkew = 0;
				else
					mask_gradientSkew = tan(M_PI / 180.0 * currItem->GrSkew);
				qmatrix.shear(mask_gradientSkew, 0);
			}
			qmatrix.translate(0, currItem->GrStartY * (1.0 - currItem->GrScale));
			qmatrix.translate(-currItem->GrStartX, -currItem->GrStartY);
			qmatrix.scale(1, currItem->GrScale);

			QPointF focal = qmatrix.map(QPointF(currItem->GrFocalX, currItem->GrFocalY));
			psx->setPen(pens().value("node-dash"));
			psx->drawLine(QPointF(currItem->GrStartX, currItem->GrStartY), focal);
			drawNodeControl(psx, focal, pens().value("node"), m_canvas->m_viewMode.scale);
		}

		// Draw Shear Control
		QTransform m;
		m.translate(currItem->GrStartX, currItem->GrStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(currItem->GrSkew);
		m.translate(radEnd * currItem->GrScale, 0);
		QPointF shP = m.map(QPointF(0,0));
		psx->setPen(pens().value("node-handle"));
		psx->drawLine(QPointF(currItem->GrStartX, currItem->GrStartY), shP);
		drawNodeControl(psx, shP, pens().value("node"), m_canvas->m_viewMode.scale);

	}
	else if (m_view->editStrokeGradient == GradientEdit::FourColors)
	{
		drawNodeHandle(psx, QPointF(currItem->GrControl1.x(), currItem->GrControl1.y()), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrControl2.x(), currItem->GrControl2.y()), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrControl3.x(), currItem->GrControl3.y()), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrControl4.x(), currItem->GrControl4.y()), pens().value("node"), m_canvas->m_viewMode.scale);
	}
	else if (m_view->editStrokeGradient == GradientEdit::Diamond)
	{
		drawNodeHandle(psx, QPointF(currItem->GrControl1.x(), currItem->GrControl1.y()), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrControl2.x(), currItem->GrControl2.y()), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrControl3.x(), currItem->GrControl3.y()), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrControl4.x(), currItem->GrControl4.y()), pens().value("node"), m_canvas->m_viewMode.scale);
		drawNodeHandle(psx, QPointF(currItem->GrControl5.x(), currItem->GrControl5.y()), pens().value("node"), m_canvas->m_viewMode.scale);
	}
}

void CanvasMode_EditGradient::enterEvent(QEvent* /*e*/)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_EditGradient::leaveEvent(QEvent* /*e*/)
{
}


void CanvasMode_EditGradient::activate(bool fromGesture)
{
	//	qDebug() << "CanvasMode_EditGradient::activate" << fromGesture;
	CanvasMode::activate(fromGesture);

	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	m_Mxp = m_Myp = -1;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_EditGradient::deactivate(bool forGesture)
{
	//	qDebug() << "CanvasMode_EditGradient::deactivate" << forGesture;
	m_view->setRedrawMarkerShown(false);
	CanvasMode::deactivate(forGesture);
}

void CanvasMode_EditGradient::keyPressEvent(QKeyEvent *e)
{
	commonkeyPressEvent_Default(e);
}

void CanvasMode_EditGradient::keyReleaseEvent(QKeyEvent *e)
{
	commonkeyReleaseEvent(e);
}

void CanvasMode_EditGradient::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	//	m_view->stopDragTimer();
	PageItem *currItem = nullptr;
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
		if (!(GetItem(&currItem) && (m_doc->appMode == modeEdit) && currItem->isTextFrame()))
		{
			mousePressEvent(m);
			return;
		}
	}
}

void CanvasMode_EditGradient::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPosition());
	m->accept();
	if (m_gradientPoint == noPointDefined)
		return;

	if (m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		double newX = mousePointDoc.x();
		double newY = mousePointDoc.y();
		FPoint npx(m_Mxp - newX, m_Myp - newY, 0, 0, currItem->rotation(), 1, 1, true);
		QPointF np(npx.x(), npx.y());
		if (m_view->editStrokeGradient == GradientEdit::Gradient_Stroke)
		{
			if (m_gradientPoint == useGradientStart)
			{
				currItem->GrStrokeStartX -= np.x();
				currItem->GrStrokeStartY -= np.y();
				currItem->GrStrokeFocalX -= np.x();
				currItem->GrStrokeFocalY -= np.y();
				if (currItem->strokeGradientType() == Gradient_Radial)
				{
					double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
					double radFoc = distance(currItem->GrStrokeFocalX - currItem->GrStrokeStartX, currItem->GrStrokeFocalY - currItem->GrStrokeStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrStrokeStartX += np.x();
						currItem->GrStrokeStartY += np.y();
						currItem->GrStrokeFocalX += np.x();
						currItem->GrStrokeFocalY += np.y();
					}
				}
			}
			if (m_gradientPoint == useGradientFocal)
			{
				double rotEnd = xy2Deg(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
				QTransform qmatrix;
				qmatrix.translate(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
				qmatrix.rotate(rotEnd);
				double mask_gradientSkew = 0.0;
				if (currItem->GrStrokeSkew == 90)
					mask_gradientSkew = 1;
				else if (currItem->GrStrokeSkew == 180)
					mask_gradientSkew = 0;
				else if (currItem->GrStrokeSkew == 270)
					mask_gradientSkew = -1;
				else if (currItem->GrStrokeSkew == 360)
					mask_gradientSkew = 0;
				else
					mask_gradientSkew = tan(M_PI / 180.0 * currItem->GrStrokeSkew);
				qmatrix.shear(mask_gradientSkew, 0);
				qmatrix.translate(0, currItem->GrStrokeStartY * (1.0 - currItem->GrStrokeScale));
				qmatrix.translate(-currItem->GrStrokeStartX, -currItem->GrStrokeStartY);
				qmatrix.scale(1, currItem->GrStrokeScale);
				QTransform m = qmatrix.inverted();
				QPointF shR = qmatrix.map(QPointF(currItem->GrStrokeFocalX, currItem->GrStrokeFocalY));
				QPointF shRe = m.map(shR - np);
				currItem->GrStrokeFocalX = shRe.x();
				currItem->GrStrokeFocalY = shRe.y();
				if (currItem->strokeGradientType() == Gradient_Radial)
				{
					double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
					double radFoc = distance(currItem->GrStrokeFocalX - currItem->GrStrokeStartX, currItem->GrStrokeFocalY - currItem->GrStrokeStartY);
					if (radFoc >= radEnd)
					{
						QPointF shR = qmatrix.map(QPointF(currItem->GrStrokeFocalX, currItem->GrStrokeFocalY));
						QPointF shRe = m.map(shR + np);
						currItem->GrStrokeFocalX = shRe.x();
						currItem->GrStrokeFocalY = shRe.y();
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
				QPointF shR(shP.x() -np.x(), shP.y() - np.y());
				double radNew = distance(shR.x() - currItem->GrStrokeStartX, shR.y() - currItem->GrStrokeStartY);
				double rotNew = xy2Deg(shR.x() - currItem->GrStrokeStartX, shR.y() - currItem->GrStrokeStartY) + 90;
				currItem->GrStrokeSkew = rotNew - rotEnd;
				double newScale = radNew / radEnd;
				if ((newScale > 0) && (newScale <= 1))
					currItem->GrStrokeScale = newScale;
			}
			if (m_gradientPoint == useGradientEnd)
			{
				currItem->GrStrokeEndX -= np.x();
				currItem->GrStrokeEndY -= np.y();
				if (currItem->strokeGradientType() == Gradient_Radial)
				{
					double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
					double radFoc = distance(currItem->GrStrokeFocalX - currItem->GrStrokeStartX, currItem->GrStrokeFocalY - currItem->GrStrokeStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrStrokeEndX += np.x();
						currItem->GrStrokeEndY += np.y();
					}
				}
			}
		}
		else if (m_view->editStrokeGradient == GradientEdit::Gradient_Mask)
		{
			if (m_gradientPoint == useGradientStart)
			{
				currItem->GrMaskStartX -= np.x();
				currItem->GrMaskStartY -= np.y();
				currItem->GrMaskFocalX -= np.x();
				currItem->GrMaskFocalY -= np.y();
				if ((currItem->GrMask == GradMask_Radial) || (currItem->GrMask == GradMask_RadialLumAlpha))
				{
					double radEnd = distance(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
					double radFoc = distance(currItem->GrMaskFocalX - currItem->GrMaskStartX, currItem->GrMaskFocalY - currItem->GrMaskStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrMaskStartX += np.x();
						currItem->GrMaskStartY += np.y();
						currItem->GrMaskFocalX += np.x();
						currItem->GrMaskFocalY += np.y();
					}
				}
			}
			if (m_gradientPoint == useGradientFocal)
			{
				double rotEnd = xy2Deg(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
				QTransform qmatrix;
				qmatrix.translate(currItem->GrMaskStartX, currItem->GrMaskStartY);
				qmatrix.rotate(rotEnd);
				double mask_gradientSkew = 0.0;
				if (currItem->GrMaskSkew == 90)
					mask_gradientSkew = 1;
				else if (currItem->GrMaskSkew == 180)
					mask_gradientSkew = 0;
				else if (currItem->GrMaskSkew == 270)
					mask_gradientSkew = -1;
				else if (currItem->GrMaskSkew == 360)
					mask_gradientSkew = 0;
				else
					mask_gradientSkew = tan(M_PI / 180.0 * currItem->GrMaskSkew);
				qmatrix.shear(mask_gradientSkew, 0);
				qmatrix.translate(0, currItem->GrMaskStartY * (1.0 - currItem->GrMaskScale));
				qmatrix.translate(-currItem->GrMaskStartX, -currItem->GrMaskStartY);
				qmatrix.scale(1, currItem->GrMaskScale);
				QTransform m = qmatrix.inverted();
				QPointF shR = qmatrix.map(QPointF(currItem->GrMaskFocalX, currItem->GrMaskFocalY));
				QPointF shRe = m.map(shR - np);
				currItem->GrMaskFocalX = shRe.x();
				currItem->GrMaskFocalY = shRe.y();
				if ((currItem->GrMask == GradMask_Radial) || (currItem->GrMask == GradMask_RadialLumAlpha))
				{
					double radEnd = distance(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
					double radFoc = distance(currItem->GrMaskFocalX - currItem->GrMaskStartX, currItem->GrMaskFocalY - currItem->GrMaskStartY);
					if (radFoc >= radEnd)
					{
						QPointF shR = qmatrix.map(QPointF(currItem->GrMaskFocalX, currItem->GrMaskFocalY));
						QPointF shRe = m.map(shR + np);
						currItem->GrMaskFocalX = shRe.x();
						currItem->GrMaskFocalY = shRe.y();
					}
				}
			}
			if (m_gradientPoint == useGradientSkew)
			{
				double radEnd = distance(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
				double rotEnd = xy2Deg(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
				QTransform m;
				m.translate(currItem->GrMaskStartX, currItem->GrMaskStartY);
				m.rotate(rotEnd);
				m.rotate(-90);
				m.rotate(currItem->GrMaskSkew);
				m.translate(radEnd * currItem->GrMaskScale, 0);
				QPointF shP = m.map(QPointF(0,0));
				QPointF shR(shP.x() -np.x(), shP.y() - np.y());
				double radNew = distance(shR.x() - currItem->GrMaskStartX, shR.y() - currItem->GrMaskStartY);
				double rotNew = xy2Deg(shR.x() - currItem->GrMaskStartX, shR.y() - currItem->GrMaskStartY) + 90;
				currItem->GrMaskSkew = rotNew - rotEnd;
				double newScale = radNew / radEnd;
				if ((newScale > 0) && (newScale <= 1))
					currItem->GrMaskScale = newScale;
			}
			if (m_gradientPoint == useGradientEnd)
			{
				currItem->GrMaskEndX -= np.x();
				currItem->GrMaskEndY -= np.y();
				if ((currItem->GrMask == GradMask_Radial) || (currItem->GrMask == GradMask_RadialLumAlpha))
				{
					double radEnd = distance(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
					double radFoc = distance(currItem->GrMaskFocalX - currItem->GrMaskStartX, currItem->GrMaskFocalY - currItem->GrMaskStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrMaskEndX += np.x();
						currItem->GrMaskEndY += np.y();
					}
				}
			}
		}
		else if (m_view->editStrokeGradient == GradientEdit::Gradient)
		{
			if (m_gradientPoint == useGradientStart)
			{
				currItem->GrStartX -= np.x();
				currItem->GrStartY -= np.y();
				currItem->GrFocalX -= np.x();
				currItem->GrFocalY -= np.y();
				if ((currItem->gradientType() == Gradient_Radial) || (currItem->gradientType() == Gradient_Conical))
				{
					double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
					double radFoc = distance(currItem->GrFocalX - currItem->GrStartX, currItem->GrFocalY - currItem->GrStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrStartX += np.x();
						currItem->GrStartY += np.y();
						currItem->GrFocalX += np.x();
						currItem->GrFocalY += np.y();
					}
				}
			}
			if (m_gradientPoint == useGradientFocal)
			{
				double rotEnd = xy2Deg(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
				QTransform qmatrix;
				qmatrix.translate(currItem->GrStartX, currItem->GrStartY);
				qmatrix.rotate(rotEnd);
				if (currItem->GrType != Gradient_Conical)
				{
					double mask_gradientSkew = 0.0;
					if (currItem->GrSkew == 90)
						mask_gradientSkew = 1;
					else if (currItem->GrSkew == 180)
						mask_gradientSkew = 0;
					else if (currItem->GrSkew == 270)
						mask_gradientSkew = -1;
					else if (currItem->GrSkew == 360)
						mask_gradientSkew = 0;
					else
						mask_gradientSkew = tan(M_PI / 180.0 * currItem->GrSkew);
					qmatrix.shear(mask_gradientSkew, 0);
				}
				qmatrix.translate(0, currItem->GrStartY * (1.0 - currItem->GrScale));
				qmatrix.translate(-currItem->GrStartX, -currItem->GrStartY);
				qmatrix.scale(1, currItem->GrScale);
				QTransform m = qmatrix.inverted();
				QPointF shR = qmatrix.map(QPointF(currItem->GrFocalX, currItem->GrFocalY));
				QPointF shRe = m.map(shR - np);
				currItem->GrFocalX = shRe.x();
				currItem->GrFocalY = shRe.y();
				if ((currItem->GrType == Gradient_Radial) || (currItem->GrType == Gradient_Conical))
				{
					double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
					double radFoc = distance(currItem->GrFocalX - currItem->GrStartX, currItem->GrFocalY - currItem->GrStartY);
					if (radFoc >= radEnd)
					{
						QPointF shR = qmatrix.map(QPointF(currItem->GrFocalX, currItem->GrFocalY));
						QPointF shRe = m.map(shR + np);
						currItem->GrFocalX = shRe.x();
						currItem->GrFocalY = shRe.y();
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
				if (currItem->GrType != Gradient_Conical)
					m.rotate(currItem->GrSkew);
				m.translate(radEnd * currItem->GrScale, 0);
				QPointF shP = m.map(QPointF(0,0));
				QPointF shR(shP.x() -np.x(), shP.y() - np.y());
				double radNew = distance(shR.x() - currItem->GrStartX, shR.y() - currItem->GrStartY);
				double rotNew = xy2Deg(shR.x() - currItem->GrStartX, shR.y() - currItem->GrStartY) + 90;
				if (currItem->GrType != Gradient_Conical)
					currItem->GrSkew = rotNew - rotEnd;
				else
					currItem->GrSkew = 0;
				double newScale = radNew / radEnd;
				if ((newScale > 0) && (newScale <= 1))
					currItem->GrScale = newScale;
			}
			if (m_gradientPoint == useGradientEnd)
			{
				currItem->GrEndX -= np.x();
				currItem->GrEndY -= np.y();
				if ((currItem->GrType == Gradient_Radial) || (currItem->GrType == Gradient_Conical))
				{
					double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
					double radFoc = distance(currItem->GrFocalX - currItem->GrStartX, currItem->GrFocalY - currItem->GrStartY);
					if (radFoc >= radEnd)
					{
						currItem->GrEndX += np.x();
						currItem->GrEndY += np.y();
					}
				}
			}
		}
		else if (m_view->editStrokeGradient == GradientEdit::FourColors)
		{
			if (m_gradientPoint == useControl1)
				currItem->GrControl1 -= npx;
			else if (m_gradientPoint == useControl2)
				currItem->GrControl2 -= npx;
			else if (m_gradientPoint == useControl3)
				currItem->GrControl3 -= npx;
			else if (m_gradientPoint == useControl4)
				currItem->GrControl4 -= npx;
		}
		else if (m_view->editStrokeGradient == GradientEdit::Diamond)
		{
			if (m_gradientPoint == useControl1)
				currItem->GrControl1 -= npx;
			else if (m_gradientPoint == useControl2)
				currItem->GrControl2 -= npx;
			else if (m_gradientPoint == useControl3)
				currItem->GrControl3 -= npx;
			else if (m_gradientPoint == useControl4)
				currItem->GrControl4 -= npx;
			else if (m_gradientPoint == useControl5)
				currItem->GrControl5 -= npx;
		}
		m_Mxp = newX;
		m_Myp = newY;
		//		m_view->RefreshGradient(currItem, dx * m_canvas->scale(), dy * m_canvas->scale());
		//		m_ScMW->propertiesPalette->updateColorSpecialGradient();
		currItem->update();
		/*		QRectF upRect;
		if (m_view->editStrokeGradient == 1)
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
		else if (m_view->editStrokeGradient == 2)
		{
			upRect = QRectF(QPointF(currItem->GrMaskStartX, currItem->GrMaskStartY), QPointF(currItem->GrMaskEndX, currItem->GrMaskEndY));
			double radEnd = distance(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
			double rotEnd = xy2Deg(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
			QTransform m;
			m.translate(currItem->GrMaskStartX, currItem->GrMaskStartY);
			m.rotate(rotEnd);
			m.rotate(-90);
			m.rotate(currItem->GrMaskSkew);
			m.translate(radEnd * currItem->GrMaskScale, 0);
			QPointF shP = m.map(QPointF(0,0));
			upRect |= upRect.united(QRectF(shP, QPointF(currItem->GrMaskEndX, currItem->GrMaskEndY)).normalized());
			upRect |= upRect.united(QRectF(shP, QPointF(currItem->GrMaskStartX, currItem->GrMaskStartY)).normalized());
			upRect |= QRectF(shP, QPointF(0, 0)).normalized();
			upRect |= QRectF(shP, QPointF(currItem->width(), currItem->height())).normalized();
		}
		else if (m_view->editStrokeGradient == 0)
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
		else if ((m_view->editStrokeGradient == 3) || (m_view->editStrokeGradient == 4))
		{
			upRect = QRectF(QPointF(-currItem->width(), -currItem->height()), QPointF(currItem->width() * 2, currItem->height() * 2)).normalized();
		}
		QTransform itemMatrix = currItem->getTransform();
		upRect = itemMatrix.mapRect(upRect);
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0)); */
		if (currItem->GrType == Gradient_Conical)
			currItem->createConicalMesh();
		m_doc->regionsChanged()->update(QRectF());
	}
}

void CanvasMode_EditGradient::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPosition());

	m_canvas->PaintSizeRect(QRect());
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPosition());
	m_gradientPoint = noPointDefined;
	m_Mxp = mousePointDoc.x();
	m_Myp = mousePointDoc.y();
	if (m->button() == Qt::MiddleButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	QTransform itemMatrix = currItem->getTransform();
	QPointF gradientStart, gradientEnd, gradientFocal, gradientScale, gradientCenter;
	if (m_view->editStrokeGradient == GradientEdit::Gradient_Stroke)
	{
		gradientStart = QPointF(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
		gradientEnd = QPointF(currItem->GrStrokeEndX, currItem->GrStrokeEndY);
		double radEnd = distance(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
		double rotEnd = xy2Deg(currItem->GrStrokeEndX - currItem->GrStrokeStartX, currItem->GrStrokeEndY - currItem->GrStrokeStartY);
		QTransform m;
		m.translate(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(currItem->GrStrokeSkew);
		m.translate(radEnd * currItem->GrStrokeScale, 0);
		gradientScale = m.map(QPointF(0,0));
		QTransform qmatrix;
		qmatrix.translate(currItem->GrStrokeStartX, currItem->GrStrokeStartY);
		qmatrix.rotate(rotEnd);
		double mask_gradientSkew = 0.0;
		if (currItem->GrStrokeSkew == 90)
			mask_gradientSkew = 1;
		else if (currItem->GrStrokeSkew == 180)
			mask_gradientSkew = 0;
		else if (currItem->GrStrokeSkew == 270)
			mask_gradientSkew = -1;
		else if (currItem->GrStrokeSkew == 360)
			mask_gradientSkew = 0;
		else
			mask_gradientSkew = tan(M_PI / 180.0 * currItem->GrStrokeSkew);
		qmatrix.shear(mask_gradientSkew, 0);
		qmatrix.translate(0, currItem->GrStrokeStartY * (1.0 - currItem->GrStrokeScale));
		qmatrix.translate(-currItem->GrStrokeStartX, -currItem->GrStrokeStartY);
		qmatrix.scale(1, currItem->GrStrokeScale);
		gradientFocal = qmatrix.map(QPointF(currItem->GrStrokeFocalX, currItem->GrStrokeFocalY));
	}
	else if (m_view->editStrokeGradient == GradientEdit::Gradient_Mask)
	{
		gradientStart = QPointF(currItem->GrMaskStartX, currItem->GrMaskStartY);
		gradientEnd = QPointF(currItem->GrMaskEndX, currItem->GrMaskEndY);
		double radEnd = distance(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
		double rotEnd = xy2Deg(currItem->GrMaskEndX - currItem->GrMaskStartX, currItem->GrMaskEndY - currItem->GrMaskStartY);
		QTransform m;
		m.translate(currItem->GrMaskStartX, currItem->GrMaskStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(currItem->GrMaskSkew);
		m.translate(radEnd * currItem->GrMaskScale, 0);
		gradientScale = m.map(QPointF(0,0));
		QTransform qmatrix;
		qmatrix.translate(currItem->GrMaskStartX, currItem->GrMaskStartY);
		qmatrix.rotate(rotEnd);
		double mask_gradientSkew = 0.0;
		if (currItem->GrMaskSkew == 90)
			mask_gradientSkew = 1;
		else if (currItem->GrMaskSkew == 180)
			mask_gradientSkew = 0;
		else if (currItem->GrMaskSkew == 270)
			mask_gradientSkew = -1;
		else if (currItem->GrMaskSkew == 360)
			mask_gradientSkew = 0;
		else
			mask_gradientSkew = tan(M_PI / 180.0 * currItem->GrMaskSkew);
		qmatrix.shear(mask_gradientSkew, 0);
		qmatrix.translate(0, currItem->GrMaskStartY * (1.0 - currItem->GrMaskScale));
		qmatrix.translate(-currItem->GrMaskStartX, -currItem->GrMaskStartY);
		qmatrix.scale(1, currItem->GrMaskScale);
		gradientFocal = qmatrix.map(QPointF(currItem->GrMaskFocalX, currItem->GrMaskFocalY));
	}
	else if (m_view->editStrokeGradient == GradientEdit::Gradient)
	{
		gradientStart = QPointF(currItem->GrStartX, currItem->GrStartY);
		gradientEnd = QPointF(currItem->GrEndX, currItem->GrEndY);
		double radEnd = distance(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
		double rotEnd = xy2Deg(currItem->GrEndX - currItem->GrStartX, currItem->GrEndY - currItem->GrStartY);
		QTransform m;
		m.translate(currItem->GrStartX, currItem->GrStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(currItem->GrSkew);
		m.translate(radEnd * currItem->GrScale, 0);
		gradientScale = m.map(QPointF(0,0));
		QTransform qmatrix;
		qmatrix.translate(currItem->GrStartX, currItem->GrStartY);
		qmatrix.rotate(rotEnd);
		if (currItem->GrType != Gradient_Conical)
		{
			double mask_gradientSkew = 0.0;
			if (currItem->GrSkew == 90)
				mask_gradientSkew = 1;
			else if (currItem->GrSkew == 180)
				mask_gradientSkew = 0;
			else if (currItem->GrSkew == 270)
				mask_gradientSkew = -1;
			else if (currItem->GrSkew == 360)
				mask_gradientSkew = 0;
			else
				mask_gradientSkew = tan(M_PI / 180.0 * currItem->GrSkew);
			qmatrix.shear(mask_gradientSkew, 0);
		}
		qmatrix.translate(0, currItem->GrStartY * (1.0 - currItem->GrScale));
		qmatrix.translate(-currItem->GrStartX, -currItem->GrStartY);
		qmatrix.scale(1, currItem->GrScale);
		gradientFocal = qmatrix.map(QPointF(currItem->GrFocalX, currItem->GrFocalY));
	}
	else if (m_view->editStrokeGradient == GradientEdit::FourColors)
	{
		gradientStart = QPointF(currItem->GrControl1.x(), currItem->GrControl1.y());
		gradientEnd = QPointF(currItem->GrControl2.x(), currItem->GrControl2.y());
		gradientFocal = QPointF(currItem->GrControl3.x(), currItem->GrControl3.y());
		gradientScale = QPointF(currItem->GrControl4.x(), currItem->GrControl4.y());
	}
	else if (m_view->editStrokeGradient == GradientEdit::Diamond)
	{
		gradientStart = QPointF(currItem->GrControl1.x(), currItem->GrControl1.y());
		gradientEnd = QPointF(currItem->GrControl2.x(), currItem->GrControl2.y());
		gradientFocal = QPointF(currItem->GrControl3.x(), currItem->GrControl3.y());
		gradientScale = QPointF(currItem->GrControl4.x(), currItem->GrControl4.y());
		gradientCenter = QPointF(currItem->GrControl5.x(), currItem->GrControl5.y());
	}
	gradientStart = itemMatrix.map(gradientStart);
	gradientEnd = itemMatrix.map(gradientEnd);
	gradientFocal = itemMatrix.map(gradientFocal);
	gradientScale = itemMatrix.map(gradientScale);
	gradientCenter = itemMatrix.map(gradientCenter);
	if (m_view->editStrokeGradient == GradientEdit::FourColors)
	{
		if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientStart))
			m_gradientPoint = useControl1;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientEnd))
			m_gradientPoint = useControl2;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientFocal))
			m_gradientPoint = useControl3;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientScale))
			m_gradientPoint = useControl4;
	}
	else if (m_view->editStrokeGradient == GradientEdit::Diamond)
	{
		if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientStart))
			m_gradientPoint = useControl1;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientEnd))
			m_gradientPoint = useControl2;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientFocal))
			m_gradientPoint = useControl3;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientScale))
			m_gradientPoint = useControl4;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientCenter))
			m_gradientPoint = useControl5;
	}
	else
	{
		if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientStart))
			m_gradientPoint = useGradientStart;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientEnd))
			m_gradientPoint = useGradientEnd;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientFocal))
			m_gradientPoint = useGradientFocal;
		else if (m_canvas->hitsCanvasPoint(m->globalPosition(), gradientScale))
			m_gradientPoint = useGradientSkew;
	}
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	if (m_gradientPoint != noPointDefined){
		OldGrStartX = currItem->gradientStartX();
		OldGrStartY = currItem->gradientStartY();
		OldGrEndX = currItem->gradientEndX();
		OldGrEndY = currItem->gradientEndY();
		OldGrFocalX = currItem->gradientFocalX();
		OldGrFocalY = currItem->gradientFocalY();
		OldGrScale = currItem->gradientScale();
		OldGrSkew = currItem->gradientSkew();
		OldGrControl1 = currItem->gradientControl1();
		OldGrControl2 = currItem->gradientControl2();
		OldGrControl3 = currItem->gradientControl3();
		OldGrControl4 = currItem->gradientControl4();
		OldGrControl5 = currItem->gradientControl5();
		OldGrStrokeStartX = currItem->gradientStrokeStartX();
		OldGrStrokeStartY = currItem->gradientStrokeStartY();
		OldGrStrokeEndX = currItem->gradientStrokeEndX();
		OldGrStrokeEndY = currItem->gradientStrokeEndY();
		OldGrStrokeFocalX = currItem->gradientStrokeFocalX();
		OldGrStrokeFocalY = currItem->gradientStrokeFocalY();
		OldGrStrokeScale = currItem->gradientStrokeScale();
		OldGrStrokeSkew = currItem->gradientStrokeSkew();
		OldGrMaskStartX = currItem->GrMaskStartX;
		OldGrMaskStartY = currItem->GrMaskStartY;
		OldGrMaskEndX = currItem->GrMaskEndX;
		OldGrMaskEndY = currItem->GrMaskEndY;
		OldGrMaskFocalX = currItem->GrMaskFocalX;
		OldGrMaskFocalY = currItem->GrMaskFocalY;
		OldGrMaskScale = currItem->GrMaskScale;
		OldGrMaskSkew = currItem->GrMaskSkew;
	}
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
	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	if (m_gradientPoint != noPointDefined && UndoManager::undoEnabled())
	{
		QList<FPoint> tmp;
		ScItemState<QList<FPoint> > *is = new ScItemState<QList<FPoint> >(Um::GradPos + "i");
		is->set("GRAD_POS");
		is->set("OLDSTARTX", OldGrStartX);
		is->set("STARTX", currItem->gradientStartX());
		is->set("OLDSTARTY", OldGrStartY);
		is->set("STARTY", currItem->gradientStartY());
		is->set("OLDENDX", OldGrEndX);
		is->set("ENDX", currItem->gradientEndX());
		is->set("OLDENDY", OldGrEndY);
		is->set("ENDY", currItem->gradientEndY());
		is->set("OLDFOCALX", OldGrFocalX);
		is->set("FOCALX", currItem->gradientFocalX());
		is->set("OLDFOCALY", OldGrFocalY);
		is->set("FOCALY", currItem->gradientFocalY());
		is->set("OLDSCALE", OldGrScale);
		is->set("SCALE", currItem->gradientScale());
		is->set("OLDSKEW", OldGrSkew);
		is->set("SKEW", currItem->gradientSkew());
		tmp.append(OldGrControl1);
		tmp.append(currItem->gradientControl1());
		tmp.append(OldGrControl2);
		tmp.append(currItem->gradientControl2());
		tmp.append(OldGrControl3);
		tmp.append(currItem->gradientControl3());
		tmp.append(OldGrControl4);
		tmp.append(currItem->gradientControl4());
		tmp.append(OldGrControl5);
		tmp.append(currItem->gradientControl5());
		is->setItem(tmp);
		is->set("OLDSTROKESTARTX", OldGrStrokeStartX);
		is->set("STROKESTARTX", currItem->gradientStrokeStartX());
		is->set("OLDSTROKESTARTY", OldGrStrokeStartY);
		is->set("STROKESTARTY", currItem->gradientStrokeStartY());
		is->set("OLDSTROKEENDX", OldGrStrokeEndX);
		is->set("STROKEENDX", currItem->gradientStrokeEndX());
		is->set("OLDSTROKEENDY", OldGrStrokeEndY);
		is->set("STROKEENDY", currItem->gradientStrokeEndY());
		is->set("OLDSTROKEFOCALX", OldGrStrokeFocalX);
		is->set("STROKEFOCALX", currItem->gradientStrokeFocalX());
		is->set("OLDSTROKEFOCALY", OldGrStrokeFocalY);
		is->set("STROKEFOCALY", currItem->gradientStrokeFocalY());
		is->set("OLDSTROKESCALE", OldGrStrokeScale);
		is->set("STROKESCALE", currItem->gradientStrokeScale());
		is->set("OLDSTROKESKEW", OldGrStrokeSkew);
		is->set("STROKESKEW", currItem->gradientStrokeSkew());
		is->set("OLDMARKSTARTX", OldGrMaskStartX);
		is->set("MARKSTARTX", currItem->GrMaskStartX);
		is->set("OLDMARKSTARTY", OldGrMaskStartY);
		is->set("MARKSTARTY", currItem->GrMaskStartY);
		is->set("OLDMARKENDX", OldGrMaskEndX);
		is->set("MARKENDX", currItem->GrMaskEndX);
		is->set("OLDMARKENDY", OldGrMaskEndY);
		is->set("MARKENDY", currItem->GrMaskEndY);
		is->set("OLDMARKFOCALX", OldGrMaskFocalX);
		is->set("MARKFOCALX", currItem->GrMaskFocalX);
		is->set("OLDMARKFOCALY", OldGrMaskFocalY);
		is->set("MARKFOCALY", currItem->GrMaskFocalY);
		is->set("OLDMARKSCALE", OldGrMaskScale);
		is->set("MARKSCALE", currItem->GrMaskScale);
		is->set("OLDMARKSKEW", OldGrMaskSkew);
		is->set("MARKSKEW", currItem->GrMaskSkew);
		undoManager->action(currItem, is);
	}
	m_gradientPoint = noPointDefined;
	//	m_view->stopDragTimer();
}
