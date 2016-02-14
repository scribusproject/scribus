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


#include "canvasmode_drawbezier.h"

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
#include <QWidgetAction>


#include "appmodes.h"
#include "canvas.h"
#include "canvasgesture_resize.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hyphenator.h"
#include "pageitem_textframe.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/insertTable.h"
#include "ui/oneclick.h"
#include "ui/pageselector.h"
#include "ui/propertiespalette.h"
#include "ui/scrapbookpalette.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"





BezierMode::BezierMode(ScribusView* view) : CanvasMode(view) 
{
	Mxp = Myp = -1;
	SeRx = SeRy = -1;
	MoveGX = MoveGY = false;
	inItemCreation = false;
	m_MouseButtonPressed = false;
	shiftSelItems = false;
	FirstPoly = true;
}


void BezierMode::drawControls(QPainter* p) 
{
	//
}

inline bool BezierMode::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}

void BezierMode::finalizeItem(PageItem* currItem)
{
	if (currItem->PoLine.size() >= 2)
		currItem->PoLine.resize(currItem->PoLine.size()-2);
	if (currItem->PoLine.size() < 4)
	{
//		emit DelObj(m_doc->currentPage->pageNr(), currItem->ItemNr);
		m_doc->Items->removeOne(currItem);
		m_doc->m_Selection->removeFirst();
		//emit HaveSel();
	}
	else
	{
		m_doc->sizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem, false, false);
//		currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2.0, 1)));
		m_doc->adjustItemSize(currItem);
		currItem->ContourLine = currItem->PoLine.copy();
	}
	m_view->resetMousePressed();
	currItem->checkChanges();
	currItem->update();
	currItem->emitAllToGUI();
}

// the following code was moved from scribusview.cpp:

void BezierMode::enterEvent(QEvent *)
{
	if (!m_MouseButtonPressed)
	{
		setModeCursor();
	}
}


void BezierMode::leaveEvent(QEvent *e)
{
}


void BezierMode::activate(bool flag)
{
//	qDebug() << "DrawBezierMode::activate" << flag;
	Mxp = Myp = -1;
	MoveGX = MoveGY = false;
	inItemCreation = false;
	shiftSelItems = false;
	FirstPoly = true;
	setModeCursor();
}

void BezierMode::deactivate(bool flag)
{
//	qDebug() << "BezierMode::deactivate" << flag;
//	m_view->stopDragTimer();
	PageItem* currItem = m_doc->m_Selection->itemAt(0);

	//When only one node(size=2) was created; it's not a valid line(min valid PoLine size is 6), delete it
	if (currItem)
	{
		currItem->PoLine.resize(qMax(0, static_cast<int>(currItem->PoLine.size())-2));
		if (currItem->PoLine.size() < 4)
		{
			m_view->Deselect(false);
			Selection tempSelection(m_doc, false);
			tempSelection.addItem(currItem);
			m_doc->itemSelection_DeleteItem(&tempSelection, true);
			currItem = 0;
		}
	}

	undoManager->setUndoEnabled(true);
	if (currItem && UndoManager::undoEnabled())
	{
		ScItemState<PageItem*> *is = new ScItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM");
		is->setItem(currItem);
		//Undo target rests with the Page for object specific undo
		UndoObject *target = m_doc->Pages->at(0);
		if (currItem->OwnPage > -1)
			target = m_doc->Pages->at(currItem->OwnPage);
		undoManager->action(target, is);
	}

	m_view->setRedrawMarkerShown(false);
	if (!currItem)
		return;

	m_doc->sizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem, false, false);
	currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2.0, 1.0)));
	m_doc->adjustItemSize(currItem);
	currItem->ContourLine = currItem->PoLine.copy();
	currItem->ClipEdited = true;
	currItem->FrameType = 3;
}

void BezierMode::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	mousePressEvent(m);
}


void BezierMode::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	double newX, newY;
	PageItem *currItem;
	QPoint np, np2, mop;
	FPoint npf, npf2;
	QRect tx;
	m->accept();
	m_canvas->displayCorrectedXYHUD(m->globalPos(), mousePointDoc.x(), mousePointDoc.y());
//	qDebug() << "legacy mode move:" << m->x() << m->y() << m_canvas->globalToCanvas(m->globalPos()).x() << m_canvas->globalToCanvas(m->globalPos()).y();
//	emit MousePos(m->x()/m_canvas->scale(),// + m_doc->minCanvasCoordinate.x(), 
//				  m->y()/m_canvas->scale()); // + m_doc->minCanvasCoordinate.y());
/*	if (m_doc->guidesPrefs().guidesShown)
	{
		if (MoveGY)
		{
			m_view->FromHRuler(m);
			return;
		}
		if (MoveGX)
		{
			m_view->FromVRuler(m);
			return;
		}
	}
*/
	
	if (commonMouseMove(m))
		return;
	
	if (inItemCreation)
	{
		if ((GetItem(&currItem)) && (!shiftSelItems))
		{
			newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
			newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
			
			if (m_doc->DragP)
				return;
			
			if (m_doc->appMode == modeDrawBezierLine)
			{
				if ((m_doc->SnapGrid) && (m_doc->OnPage(currItem) != -1))
				{
					newX = qRound(newX / m_doc->guidesPrefs().minorGridSpacing) * m_doc->guidesPrefs().minorGridSpacing;
					newY = qRound(newY / m_doc->guidesPrefs().minorGridSpacing) * m_doc->guidesPrefs().minorGridSpacing;
				}
				m_canvas->newRedrawPolygon() << QPoint(qRound(newX - currItem->xPos()), qRound(newY - currItem->yPos()));
				m_view->updateCanvas();
				Mxp = newX;
				Myp = newY;
			}
			
		}
		else
		{
			if ((m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton))
			{
				newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
				newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
				SeRx = newX;
				SeRy = newY;
				QPoint startP = m_canvas->canvasToGlobal(QPointF(Mxp, Myp));
				m_view->redrawMarker->setGeometry(QRect(m_view->mapFromGlobal(startP), m_view->mapFromGlobal(m->globalPos())).normalized());
				m_view->setRedrawMarkerShown(true);
				m_view->HaveSelRect = true;
				return;
			}
		}
	}
}

void BezierMode::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	int z;
	double Rxp = 0;
	double Ryp = 0;
	double Rxpd = 0;
	double Rypd = 0;
	PageItem *currItem;
//	m_canvas->PaintSizeRect(QRect());
	FPoint npf, npf2;
	QRect tx;
	QTransform pm;
	m_MouseButtonPressed = true;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	MoveGX = MoveGY = false;
//	inItemCreation = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	QRect mpo(m->x()-m_doc->guidesPrefs().grabRadius, m->y()-m_doc->guidesPrefs().grabRadius, m_doc->guidesPrefs().grabRadius*2, m_doc->guidesPrefs().grabRadius*2);
//	mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	Rxp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).x();
	Rxpd = Mxp - Rxp;
	Mxp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).y();
	Rypd = Myp - Ryp;
	Myp = qRound(Ryp);
	SeRx = Mxp;
	SeRy = Myp;
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	if (m->button() == Qt::RightButton)
	{
		m_view->stopGesture();
		return;
	}
	if (FirstPoly)
	{
		selectPage(m);
		undoManager->setUndoEnabled(false);
		z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->itemToolPrefs().lineWidth, CommonStrings::None, m_doc->itemToolPrefs().lineColor);
		currItem = m_doc->Items->at(z);
		m_doc->m_Selection->clear();
		m_doc->m_Selection->addItem(currItem);
		m_view->setCursor(QCursor(Qt::CrossCursor));
		m_canvas->setRenderModeFillBuffer();
		inItemCreation = true;
	}
	currItem = m_doc->m_Selection->itemAt(0);
	//			pm.translate(-m_doc->minCanvasCoordinate.x()*m_canvas->scale(), -m_doc->minCanvasCoordinate.y()*m_canvas->scale());
	pm = currItem->getTransform();
	npf = m_doc->ApplyGridF(mousePointDoc).transformPoint(pm, true);
	currItem->PoLine.addPoint(npf);
	npf2 = getMinClipF(&currItem->PoLine);
	if (npf2.x() < 0)
	{
		currItem->PoLine.translate(-npf2.x(), 0);
		m_doc->moveItem(npf2.x(), 0, currItem);
	}
	if (npf2.y() < 0)
	{
		currItem->PoLine.translate(0, -npf2.y());
		m_doc->moveItem(0, npf2.y(), currItem);
	}
	m_doc->sizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem, false, false, false);
	currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2, 1.0)));
	m_canvas->newRedrawPolygon();
	undoManager->setUndoEnabled(false);
}



void BezierMode::mouseReleaseEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	undoManager->setUndoEnabled(true);
	PageItem *currItem;
	m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
//	m_view->stopDragTimer();
	
	m_canvas->setRenderModeUseBuffer(false);
	if ((m_doc->appMode == modeDrawBezierLine) && (m->button() == Qt::LeftButton))
	{
		m_canvas->setRenderModeUseBuffer(true);
		currItem = m_doc->m_Selection->itemAt(0);
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		QTransform pm = currItem->getTransform();
		FPoint npf = m_doc->ApplyGridF(mousePointDoc).transformPoint(pm, true);
		currItem->PoLine.addPoint(npf);
		bool ssiz = currItem->Sizing;
		currItem->Sizing = true;
		if ((currItem->PoLine.size() % 4 == 0) && (currItem->PoLine.size() > 3))
		{
			FPoint lxy(currItem->PoLine.point(currItem->PoLine.size()-2));
			FPoint lk(currItem->PoLine.point(currItem->PoLine.size()-1));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			currItem->PoLine.addPoint(lxy);
			currItem->PoLine.addPoint(lk);
		}
		FPoint np2(getMinClipF(&currItem->PoLine));
		if (np2.x() < 0)
		{
			currItem->PoLine.translate(-np2.x(), 0);
			m_doc->moveItem(np2.x(), 0, currItem);
		}
		if (np2.y() < 0)
		{
			currItem->PoLine.translate(0, -np2.y());
			m_doc->moveItem(0, np2.y(), currItem);
		}
		if (FirstPoly)
		{
			FirstPoly = false;
			currItem->Sizing = ssiz;
		}
		else
		{
			m_doc->sizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem, false, false, false);
			m_doc->adjustItemSize(currItem);
			currItem->Sizing = ssiz;
			currItem->ContourLine = currItem->PoLine.copy();
			m_canvas->setRenderModeUseBuffer(false);
			currItem->update();
		}
		m_canvas->setRenderModeFillBuffer();
		int newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		int newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		m_canvas->newRedrawPolygon() << QPoint(newX - qRound(currItem->xPos()), newY - qRound(currItem->yPos()));
		m_view->updateCanvas();
	}
	if ((m_doc->appMode == modeDrawBezierLine) && (m->button() == Qt::RightButton))
	{
		currItem = m_doc->m_Selection->itemAt(0);
		if (currItem!=0)
		{
			finalizeItem(currItem);
		}
		
		if (!PrefsManager::instance()->appPrefs.uiPrefs.stickyTools)
		{
//			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			m_view->requestMode(modeNormal);
//			m_view->requestMode(submodePaintingDone);
		}
		else
			m_view->requestMode(m_doc->appMode);
		m_doc->changed();
//		emit DocChanged();
		FirstPoly = true;
		inItemCreation = false;
		m_canvas->setRenderModeUseBuffer(false);
//		m_view->updateContents();
	}
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
//	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	shiftSelItems = false;
//	m_doc->SubMode = -1;
	if (m_view->groupTransactionStarted())
	{
		for (int i = 0; i < m_doc->m_Selection->count(); ++i)
			m_doc->m_Selection->itemAt(i)->checkChanges(true);
		m_view->endGroupTransaction();
	}

//???	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
//???		m_doc->m_Selection->itemAt(i)->checkChanges(true);

//	//Commit drag created items to undo manager.
//	if (m_doc->m_Selection->itemAt(0)!=NULL)
//	{
//		m_doc->itemAddCommit(m_doc->m_Selection->itemAt(0)->ItemNr);
//	}
	//Make sure the Zoom spinbox and page selector don't have focus if we click on the canvas
	m_view->m_ScMW->zoomSpinBox->clearFocus();
	m_view->m_ScMW->pageSelector->clearFocus();
	if (m_doc->m_Selection->itemAt(0) != 0) // is there the old clip stored for the undo action
	{
		currItem = m_doc->m_Selection->itemAt(0);
		m_doc->nodeEdit.finishTransaction(currItem);
	}
	else
	{
		//delete oldClip;
		//oldClip = 0;
	}
}



void BezierMode::selectPage(QMouseEvent *m)
{
	m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	Mxp = mousePointDoc.x(); //static_cast<int>(m->x()/m_canvas->scale());
	Myp = mousePointDoc.y(); //static_cast<int>(m->y()/m_canvas->scale());
	QRect mpo(m->x()-m_doc->guidesPrefs().grabRadius, m->y()-m_doc->guidesPrefs().grabRadius, m_doc->guidesPrefs().grabRadius*2, m_doc->guidesPrefs().grabRadius*2);
//	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	m_doc->nodeEdit.deselect();
	m_view->Deselect(false);
	if (!m_doc->masterPageMode())
	{
		int i = m_doc->OnPage(Mxp, Myp);
		if (i!=-1)
		{
			uint docCurrPageNo=m_doc->currentPageNumber();
			uint j=static_cast<uint>(i);
			if (docCurrPageNo != j)
			{
				m_doc->setCurrentPage(m_doc->Pages->at(j));
				m_view->m_ScMW->slotSetCurrentPage(j);
				m_view->DrawNew();
			}
		}
/*		uint docPagesCount=m_doc->Pages->count();
		uint docCurrPageNo=m_doc->currentPageNumber();
		for (uint i = 0; i < docPagesCount; ++i)
		{
			int x = static_cast<int>(m_doc->Pages->at(i)->xOffset() * m_canvas->scale());
			int y = static_cast<int>(m_doc->Pages->at(i)->yOffset() * m_canvas->scale());
			int w = static_cast<int>(m_doc->Pages->at(i)->width() * m_canvas->scale());
			int h = static_cast<int>(m_doc->Pages->at(i)->height() * m_canvas->scale());
			if (QRect(x, y, w, h).intersects(mpo))
			{
				if (docCurrPageNo != i)
				{
					m_doc->setCurrentPage(m_doc->Pages->at(i));
					setMenTxt(i);
					DrawNew();
				}
				break;
			}
		} */

		//FIXME m_view->setRulerPos(m_view->contentsX(), m_view->contentsY());
	}
}



