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
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	RotMode = 0;
}

inline bool CanvasMode_Rotate::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}

void CanvasMode_Rotate::drawControls(QPainter* p)
{
	drawSelection(p);
}

void CanvasMode_Rotate::activate(bool fromGesture)
{
//	qDebug() << "LegacyMode::activate" << fromGesture;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->leaveDrag = false;
	m_view->MidButt = false;
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	RotMode = 0;
	setModeCursor();
	if (fromGesture)
	{
		m_canvas->m_viewMode.operItemResizeInEditMode = false;
		m_view->update();
	}
}

void CanvasMode_Rotate::deactivate(bool)
{
	m_view->redrawMarker->hide();
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
	
	double Rxp = 0;
	double Ryp = 0;
	double Rxpd = 0;
	double Rypd = 0;
	PageItem *currItem;
	QPainter p;
	m_canvas->PaintSizeRect(QRect());
	FPoint npf, npf2;
	QRect tx;
	QMatrix pm;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->leaveDrag = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	Rxp  = m_doc->ApplyGridF(FPoint(Mxp, Myp)).x();
	Rxpd = Mxp - Rxp;
	Mxp  = qRound(Rxp);
	Ryp  = m_doc->ApplyGridF(FPoint(Mxp, Myp)).y();
	Rypd = Myp - Ryp;
	Myp  = qRound(Ryp);
	SeRx = Mxp;
	SeRy = Myp;
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		m_view->DrawNew();
		return;
	}
	if (m->button() != Qt::LeftButton)
		return;
	if (GetItem(&currItem))
	{
/*AV	if (UndoManager::undoEnabled())
		m_view->undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(),
											  Um::Rotate, "", Um::IRotate);
*/
		RotMode = m_doc->RotMode;
		if (m_doc->m_Selection->isMultipleSelection())
		{
			double gx, gy, gh, gw;
			double gxR, gyR, ghR, gwR;
			m_view->getGroupRectScreen(&gx, &gy, &gw, &gh);
			m_doc->m_Selection->getGroupRect(&gxR, &gyR, &gwR, &ghR);
			if (QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
			{
				m_doc->RotMode = 2;
				m_view->RCenter = FPoint(gxR+gwR/2.0, gyR+ghR/2.0);
				if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
				{
					m_view->RCenter = FPoint(gxR, gyR);
					m_doc->RotMode = 0;
				}
			}
			m_view->oldW = xy2Deg(mousePointDoc.x() - m_view->RCenter.x(), mousePointDoc.y() - m_view->RCenter.y());
		}
		else
		{
			QMatrix mat;
			m_canvas->Transform(currItem, mat);
			m_doc->RotMode = 2;
//			RCenter = FPoint(currItem->xPos()+currItem->width()/2, currItem->yPos()+currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, true);
			m_view->RCenter = FPoint(currItem->width()/2, currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, false);
//			if (!currItem->asLine())
//			{
				if (QRegion(mat.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo))
				{
					if (mat.mapRect(QRect(0, 0, 6, 6)).intersects(mpo))
					{
//						RCenter = FPoint(currItem->xPos()+currItem->width(), currItem->yPos()+currItem->height(), 0, 0, currItem->rotation(), 1, 1, true);
						m_view->RCenter = FPoint(currItem->width(), currItem->height(), 0, 0, currItem->rotation(), 1, 1, false);
						m_doc->RotMode = 4;
					}
					else if (mat.mapRect(QRect(static_cast<int>(currItem->width())-6, 0, 6, 6)).intersects(mpo))
					{
//						RCenter = FPoint(currItem->xPos(), currItem->yPos()+currItem->height(), 0, 0, currItem->rotation(), 1, 1, true);
						m_view->RCenter = FPoint(0, currItem->height(), 0, 0, currItem->rotation(), 1, 1, false);
						m_doc->RotMode = 3;
					}
					else if (mat.mapRect(QRect(static_cast<int>(currItem->width())-6, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
					{
//						RCenter = FPoint(currItem->xPos(), currItem->yPos());
						m_view->RCenter = FPoint(0, 0);
						m_doc->RotMode = 0;
					}
					else if (mat.mapRect(QRect(0, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
					{
//						RCenter = FPoint(currItem->xPos()+currItem->width(), currItem->yPos(), 0, 0, currItem->rotation(), 1, 1, true);
						m_view->RCenter = FPoint(currItem->width(), 0, 0, 0, currItem->rotation(), 1, 1, false);
						m_doc->RotMode = 1;
					}
				}
//			}
			m_view->RCenter = FPoint(currItem->xPos()+m_view->RCenter.x(), currItem->yPos()+ m_view->RCenter.y());
			m_view->oldW = xy2Deg(mousePointDoc.x() - m_view->RCenter.x(), mousePointDoc.y() - m_view->RCenter.y());
		}
	}
}

void CanvasMode_Rotate::mouseReleaseEvent(QMouseEvent *m)
{
	clearPixmapCache();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->stopDragTimer();
	//m_canvas->update(); //ugly in a mouseReleaseEvent!!!!!!!
	if ((GetItem(&currItem)) && (m->button() == Qt::RightButton))
	{
		createContextMenu(currItem, mousePointDoc.x(), mousePointDoc.y());
		return;
	}
	if (m_view->moveTimerElapsed() && (GetItem(&currItem)))
	{
		m_view->stopDragTimer();
		m_canvas->setRenderModeUseBuffer(false);
		if (!m_doc->m_Selection->isMultipleSelection())
		{
			m_doc->setRedrawBounding(currItem);
			currItem->OwnPage = m_doc->OnPage(currItem);
			if (currItem->asLine())
				m_view->updateContents();
		}
	}
	//CB Drag selection performed here
	if (((m_doc->m_Selection->count() == 0) && (m_view->HaveSelRect) && (!m_view->MidButt)))
	{
		QRectF Sele = QRectF(Dxp, Dyp, SeRx-Dxp, SeRy-Dyp).normalized();
		if (!m_doc->masterPageMode())
		{
			uint docPagesCount=m_doc->Pages->count();
			uint docCurrPageNo=m_doc->currentPageNumber();
			for (uint i = 0; i < docPagesCount; ++i)
			{
				if (QRectF(m_doc->Pages->at(i)->xOffset(), m_doc->Pages->at(i)->yOffset(), m_doc->Pages->at(i)->width(), m_doc->Pages->at(i)->height()).intersects(Sele))
				{
					if (docCurrPageNo != i)
					{
						m_doc->setCurrentPage(m_doc->Pages->at(i));
						m_view->setMenTxt(i);
					}
					break;
				}
			}
			m_view->setRulerPos(m_view->contentsX(), m_view->contentsY());
		}
		int docItemCount=m_doc->Items->count();
		if (docItemCount != 0)
		{
			m_doc->m_Selection->delaySignalsOn();
			for (int a = 0; a < docItemCount; ++a)
			{
				PageItem* docItem = m_doc->Items->at(a);
				QMatrix p;
				m_canvas->Transform(docItem, p);
				QRegion apr = QRegion(docItem->Clip * p);
				QRect apr2(docItem->getRedrawBounding(1.0));
				if ((m_doc->masterPageMode()) && (docItem->OnMasterPage != m_doc->currentPage()->pageName()))
					continue;
				if (((Sele.contains(apr.boundingRect())) || (Sele.contains(apr2))) && (docItem->LayerNr == m_doc->activeLayer()) && (!m_doc->layerLocked(docItem->LayerNr)))
				{
					bool redrawSelection=false;
					m_view->SelectItemNr(a, redrawSelection);
				}
			}
			m_doc->m_Selection->delaySignalsOff();
			if (m_doc->m_Selection->count() > 1)
			{
				m_doc->m_Selection->setGroupRect();
				double x, y, w, h;
				m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
				m_view->getGroupRectScreen(&x, &y, &w, &h);
			}
		}
		m_view->HaveSelRect = false;
		m_view->redrawMarker->hide();
		m_view->updateContents();
	}
	if (m_doc->appMode == modeRotation)
		m_doc->RotMode = RotMode;
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
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	double newX, newY;
	PageItem *currItem;
	QPoint np, np2, mop;
	FPoint npf, npf2;
	QPainter p;
	QRect tx;
	bool erf = false;
//	double sc = m_canvas->scale();
//	m_view->horizRuler->Draw(m->x());
//	m_view->vertRuler->Draw(m->y());
	m->accept();
//	qDebug() << "legacy mode move:" << m->x() << m->y() << m_canvas->globalToCanvas(m->globalPos()).x() << m_canvas->globalToCanvas(m->globalPos()).y();
//	emit MousePos(m->x()/m_canvas->scale(),// + m_doc->minCanvasCoordinate.x(), 
//				  m->y()/m_canvas->scale()); // + m_doc->minCanvasCoordinate.y());

	if (GetItem(&currItem))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		if (m_view->moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed)
		{
			m_view->stopDragTimer();
			if (!m_view->groupTransactionStarted() && m_doc->m_Selection->isMultipleSelection())
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
			// rotate calls PageItem::update			emit DocChanged();
		}
		if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		{
			if (m_doc->m_Selection->isMultipleSelection())
			{
//				QRect mpo = QRect(qRound(m->x()/m_canvas->scale())-m_doc->guidesSettings.grabRad, qRound(m->y()/m_canvas->scale())-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//				mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x()), qRound(m_doc->minCanvasCoordinate.y()));
				double gx, gy, gh, gw;
				m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
				int how = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), QRectF(gx, gy, gw, gh));
//				if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
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
//				mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
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
			SeRx = newX;
			SeRy = newY;
			QPoint startP = m_canvas->canvasToGlobal(QPointF(Mxp, Myp));
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
	Mxp = mx;
	Myp = my;
	if(currItem!=NULL)
		cmen = new ContextMenu(*(m_doc->m_Selection), m_view->m_ScMW, m_doc);
	else
		cmen = new ContextMenu(m_view->m_ScMW, m_doc, mx, my);
	if (cmen)
		cmen->exec(QCursor::pos());
	m_view->setGlobalUndoMode();
	delete cmen;
}

void CanvasMode_Rotate::setResizeCursor(int how)
{
	switch (how)
	{
		case 1:
		case 2:
			qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
			break;
		case 3:
		case 4:
			qApp->changeOverrideCursor(QCursor(Qt::SizeBDiagCursor));
			break;
		case 5:
		case 8:
			qApp->changeOverrideCursor(QCursor(Qt::SizeVerCursor));
			break;
		case 6:
		case 7:
			qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
			break;
		default:
			qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
			break;
	}
}
