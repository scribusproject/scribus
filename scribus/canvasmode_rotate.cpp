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

#include "canvasmode_rotate.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

#include "canvas.h"
#include "contextmenu.h"
#include "fpoint.h"
#include "pageitem.h"
#include "pageselector.h"
#include "prefsmanager.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"
#include "util_icon.h"
#include "util_math.h"

CanvasMode_Rotate::CanvasMode_Rotate(ScribusView* view) : CanvasMode(view)
{
	m_canvasPressCoord.setXY(-1.0, -1.0);
	m_oldRotMode   = 0;
	m_oldRotCenter = FPoint(0.0, 0.0);
	m_angleConstrained = false;
	m_inItemRotation = false;

	m_rotMode    = 0;
	m_rotCenter  = FPoint(0.0, 0.0);
	m_startAngle = 0.0;
}

inline bool CanvasMode_Rotate::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}

void CanvasMode_Rotate::drawControls(QPainter* p)
{
	drawSelection(p, true);
	if (m_inItemRotation)
	{
		drawItemOutlines(p);
	}
}

void CanvasMode_Rotate::drawItemOutlines(QPainter* p)
{
	FPoint itemPos;
	double itemRotation;

	p->save();

	QColor  drawColor = qApp->palette().color(QPalette::Active, QPalette::Highlight);
	p->setRenderHint(QPainter::Antialiasing);
	p->setBrush(Qt::NoBrush);
	p->setPen(QPen(drawColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

	uint docSelectionCount = m_doc->m_Selection->count();
	for (uint i = 0; i < docSelectionCount; ++i)
	{
		PageItem * currItem = m_doc->m_Selection->itemAt(i);
		getNewItemPosition(currItem, itemPos, itemRotation);
		p->save();
		p->translate(itemPos.x(), itemPos.y());
		p->rotate(itemRotation);
		currItem->DrawPolyL(p, currItem->Clip);
		p->restore();
	}

	p->restore();
}

void CanvasMode_Rotate::getNewItemPosition(PageItem* item, FPoint& pos, double& rotation)
{
	double newAngle = xy2Deg(m_canvasCurrCoord.x() - m_rotCenter.x(), m_canvasCurrCoord.y() - m_rotCenter.y());
	if (m_angleConstrained)
	{
		newAngle = constrainAngle(newAngle, m_doc->toolSettings.constrain);
		/*double oldAngle = constrainAngle(m_startAngle, m_doc->toolSettings.constrain);
		newAngle = m_doc->m_Selection->isMultipleSelection() ? (newAngle - oldAngle) : newAngle;*/
		m_view->oldW = constrainAngle(m_view->oldW, m_doc->toolSettings.constrain);
		newAngle = m_doc->m_Selection->isMultipleSelection() ? (newAngle - m_view->oldW) : newAngle;
	}
	else if (m_doc->m_Selection->isMultipleSelection())
		newAngle = (newAngle - m_startAngle);
	else
		newAngle = item->rotation() - (m_startAngle - newAngle);
	if (m_doc->m_Selection->isMultipleSelection())
	{
		QMatrix ma;
		ma.translate(m_rotCenter.x(), m_rotCenter.y());
		ma.scale(1, 1);
		ma.rotate(newAngle);
		FPoint n(item->xPos() - m_rotCenter.x(), item->yPos() - m_rotCenter.y());
		pos.setXY(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
		rotation = item->rotation() + newAngle;
	}
	else if (m_rotMode != 0)
	{
		FPoint n(0,0);
		QMatrix ma;
		ma.translate(item->xPos(), item->yPos());
		ma.scale(1, 1);
		ma.rotate(item->rotation());
		double ro = newAngle - item->rotation();
		switch (m_rotMode)
		{
		case 2:
			ma.translate(item->width()/2.0, item->height()/2.0);
			n = FPoint(-item->width()/2.0, -item->height()/2.0);
			break;
		case 4:
			ma.translate(item->width(), item->height());
			n = FPoint(-item->width(), -item->height());
			break;
		case 3:
			ma.translate(0, item->height());
			n = FPoint(0, -item->height());
			break;
		case 1:
			ma.translate(item->width(), 0);
			n = FPoint(-item->width(), 0);
			break;
		}
		ma.rotate(ro);
		pos.setXY(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
		rotation = newAngle;
	}
	else
	{
		pos.setXY(item->xPos(), item->yPos());
		rotation = newAngle;
	}
}

void CanvasMode_Rotate::activate(bool fromGesture)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->leaveDrag = false;
	m_view->MidButt  = false;
	m_inItemRotation = false;
	m_canvasPressCoord.setXY(-1.0, -1.0);
	m_oldRotMode   = m_rotMode   = 0;
	m_oldRotCenter = m_rotCenter = FPoint(0.0, 0.0);
	m_startAngle   = 0.0;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_Rotate::deactivate(bool)
{
	m_view->redrawMarker->hide();
	m_inItemRotation = false;
}

void CanvasMode_Rotate::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_Rotate::leaveEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void CanvasMode_Rotate::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvasPressCoord = mousePointDoc;
	
	double Rxp = 0,  Ryp = 0;
	double Rxpd = 0, Rypd = 0;
	PageItem *currItem;
	m_canvas->PaintSizeRect(QRect());
	QRect tx;
	QMatrix pm;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
	Rxp  = m_doc->ApplyGridF(m_canvasPressCoord).x();
	Rxpd = m_canvasPressCoord.x() - Rxp;
	m_canvasPressCoord.setX( qRound(Rxp) );
	Ryp  = m_doc->ApplyGridF(m_canvasPressCoord).y();
	Rypd = m_canvasPressCoord.y()  - Ryp;
	m_canvasPressCoord.setY( qRound(Ryp) );
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	if (m->button() != Qt::LeftButton)
		return;
	if (GetItem(&currItem))
	{
		m_inItemRotation = true;
		m_oldRotMode   = m_rotMode   = m_doc->RotMode;
		m_oldRotCenter = m_rotCenter = m_view->RCenter;
		if (m_doc->m_Selection->isMultipleSelection())
		{
			double gx, gy, gh, gw;
			double gxR, gyR, ghR, gwR;
			m_view->getGroupRectScreen(&gx, &gy, &gw, &gh);
			m_doc->m_Selection->getGroupRect(&gxR, &gyR, &gwR, &ghR);
			if (QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
			{
				m_rotMode   = 2;
				m_rotCenter = FPoint(gxR+gwR/2.0, gyR+ghR/2.0);
				if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
				{
					m_rotCenter = FPoint(gxR, gyR);
					m_rotMode   = 0;
				}
				m_doc->RotMode  = m_rotMode;
				m_view->RCenter = m_rotCenter;
			}
			m_startAngle = xy2Deg(mousePointDoc.x() - m_view->RCenter.x(), mousePointDoc.y() - m_view->RCenter.y());
			m_view->oldW = m_startAngle;
		}
		else
		{
			QMatrix mat;
			m_canvas->Transform(currItem, mat);
			m_rotMode   = 2;
			m_rotCenter = FPoint(currItem->width()/2, currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, false);
//			if (!currItem->asLine())
//			{
				if (QRegion(mat.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo))
				{
					if (mat.mapRect(QRect(0, 0, 6, 6)).intersects(mpo))
					{
						m_rotCenter = FPoint(currItem->width(), currItem->height(), 0, 0, currItem->rotation(), 1, 1, false);
						m_rotMode   = 4;
					}
					else if (mat.mapRect(QRect(static_cast<int>(currItem->width())-6, 0, 6, 6)).intersects(mpo))
					{
						m_rotCenter = FPoint(0, currItem->height(), 0, 0, currItem->rotation(), 1, 1, false);
						m_rotMode   = 3;
					}
					else if (mat.mapRect(QRect(static_cast<int>(currItem->width())-6, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
					{
						m_rotCenter = FPoint(0, 0);
						m_rotMode   = 0;
					}
					else if (mat.mapRect(QRect(0, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
					{
						m_rotCenter = FPoint(currItem->width(), 0, 0, 0, currItem->rotation(), 1, 1, false);
						m_rotMode   = 1;
					}	
				}
				m_doc->RotMode  = m_rotMode;
				m_view->RCenter = m_rotCenter;
//			}
			m_view->RCenter = m_rotCenter = FPoint(currItem->xPos()+ m_view->RCenter.x(), currItem->yPos()+ m_view->RCenter.y()); //?????
			m_view->oldW = m_startAngle = xy2Deg(mousePointDoc.x() - m_view->RCenter.x(), mousePointDoc.y() - m_view->RCenter.y());
		}
	}
}

void CanvasMode_Rotate::mouseReleaseEvent(QMouseEvent *m)
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
	if ((GetItem(&currItem)) && (m->button() == Qt::RightButton))
	{
		createContextMenu(currItem, mousePointDoc.x(), mousePointDoc.y());
		return;
	}
	m_inItemRotation = false;
	if (m_view->moveTimerElapsed() && (GetItem(&currItem)))
	{
//		m_view->stopDragTimer();
		//Always start group transaction as a rotate action is generally a combination of
		//a change of rotation + a change of position
		if (!m_view->groupTransactionStarted() /*&& m_doc->m_Selection->isMultipleSelection()*/)
		{
			m_view->startGroupTransaction(Um::Rotate, "", Um::IRotate);
		}
		double newW = xy2Deg(mousePointDoc.x()-m_view->RCenter.x(), mousePointDoc.y()-m_view->RCenter.y()); //xy2Deg(m->x()/sc - m_view->RCenter.x(), m->y()/sc - m_view->RCenter.y());
		if (m->modifiers() & Qt::ControlModifier)
		{
			newW=constrainAngle(newW, m_doc->toolSettings.constrain);
			m_view->oldW=constrainAngle(m_view->oldW, m_doc->toolSettings.constrain);
			//RotateGroup uses MoveBy so its pretty hard to constrain the result
			if (m_doc->m_Selection->isMultipleSelection())
				m_doc->rotateGroup(newW-m_view->oldW, m_view->RCenter);
			else
				m_doc->RotateItem(newW, currItem->ItemNr);
		}
		else
		{
			if (m_doc->m_Selection->isMultipleSelection())
				m_doc->rotateGroup(newW - m_view->oldW, m_view->RCenter);
			else
				m_doc->RotateItem(currItem->rotation() - (m_view->oldW - newW), currItem->ItemNr);
		}
		m_view->oldW = newW;
		m_canvas->setRenderModeUseBuffer(false);
		if (!m_doc->m_Selection->isMultipleSelection())
		{
			m_doc->setRedrawBounding(currItem);
			currItem->OwnPage = m_doc->OnPage(currItem);
			if (currItem->asLine())
				m_view->updateContents();
		}
	}
	m_doc->RotMode  = m_oldRotMode;
	m_view->RCenter = m_oldRotCenter;
	if (!PrefsManager::instance()->appPrefs.stickyTools)
		m_view->requestMode(modeNormal);
	else
	{
		int appMode = m_doc->appMode;
		m_view->requestMode(appMode);
	}
	if (GetItem(&currItem))
	{
		if (m_doc->m_Selection->count() > 1)
		{
			m_doc->m_Selection->setGroupRect();
			double x, y, w, h;
			m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			m_view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
		}
		// Handled normally automatically by Selection in sendSignals()
		/*else
			currItem->emitAllToGUI();*/
	}
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->leaveDrag = false;
	m_view->MidButt  = false;
	if (m_view->groupTransactionStarted())
	{
		for (int i = 0; i < m_doc->m_Selection->count(); ++i)
			m_doc->m_Selection->itemAt(i)->checkChanges(true);
		m_view->endGroupTransaction();
	}
	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
		m_doc->m_Selection->itemAt(i)->checkChanges(true);
	//Make sure the Zoom spinbox and page selector dont have focus if we click on the canvas
	m_view->zoomSpinBox->clearFocus();
	m_view->pageSelector->clearFocus();
}

void CanvasMode_Rotate::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_canvasCurrCoord  = mousePointDoc;
	m_angleConstrained = false;
	
	double newX, newY;
	PageItem *currItem;
	QRect tx;
	m->accept();

	if (GetItem(&currItem))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		m_angleConstrained = ((m->modifiers() & Qt::ControlModifier) != Qt::NoModifier);
		if (m_view->moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed)
		{
			m_canvas->repaint();
			double itemRotation;
			FPoint itemPos;
			getNewItemPosition(currItem, itemPos, itemRotation);
			m_canvas->displayRotHUD(m->globalPos(), itemRotation);
		}
		if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		{
			if (m_doc->m_Selection->isMultipleSelection())
			{
				double gx, gy, gh, gw;
				m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
				int how = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), QRectF(gx, gy, gw, gh));
				if (how >= 0)
				{
					if (how > 0)
					{
						setResizeCursor(how);
					}
					qApp->changeOverrideCursor(QCursor(loadIcon("Rotieren2.png")));
				}
				else
				{
					setModeCursor();
				}
				return;
			}
			for (int a = 0; a < m_doc->m_Selection->count(); ++a)
			{
				currItem = m_doc->m_Selection->itemAt(a);
				if (currItem->locked())
					break;
				QMatrix p;
				m_canvas->Transform(currItem, p);
				QRect mpo = QRect(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
				if ((QRegion(p.map(QPolygon(QRect(-3, -3, static_cast<int>(currItem->width()+6), static_cast<int>(currItem->height()+6))))).contains(mpo)))
				{
					tx = p.mapRect(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
					if ((tx.intersects(mpo)) && (!currItem->locked()))
					{
						qApp->changeOverrideCursor(QCursor(loadIcon("Rotieren2.png")));
						if (!currItem->sizeLocked())
							m_view->HandleCurs(currItem, mpo);
					}
				}
				else
				{
//					setModeCursor();
				}
			}
		}
	}
	else
	{
		if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton))
		{
			newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
			newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
			QPoint startP = m_canvas->canvasToGlobal(m_canvasPressCoord);
			m_view->redrawMarker->setGeometry(QRect(startP, m->globalPos()).normalized());
			if (!m_view->redrawMarker->isVisible())
				m_view->redrawMarker->show();
			m_view->HaveSelRect = true;
			return;
		}
	}
}

void CanvasMode_Rotate::createContextMenu(PageItem* currItem, double mx, double my)
{
	ContextMenu* cmen=NULL;
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	m_view->setObjectUndoMode();
	m_canvasPressCoord.setXY(mx, my);
	if(currItem!=NULL)
		cmen = new ContextMenu(*(m_doc->m_Selection), m_view->m_ScMW, m_doc);
	else
		cmen = new ContextMenu(m_view->m_ScMW, m_doc, mx, my);
	if (cmen)
		cmen->exec(QCursor::pos());
	m_view->setGlobalUndoMode();
	delete cmen;
}

// void CanvasMode_Rotate::setResizeCursor(int how)
// {
// 	switch (how)
// 	{
// 		case 1:
// 		case 2:
// 			qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
// 			break;
// 		case 3:
// 		case 4:
// 			qApp->changeOverrideCursor(QCursor(Qt::SizeBDiagCursor));
// 			break;
// 		case 5:
// 		case 8:
// 			qApp->changeOverrideCursor(QCursor(Qt::SizeVerCursor));
// 			break;
// 		case 6:
// 		case 7:
// 			qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
// 			break;
// 		default:
// 			qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
// 			break;
// 	}
// }
