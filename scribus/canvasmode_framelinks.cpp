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


#include "canvasmode_framelinks.h"

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

#include "canvas.h"
#include "contextmenu.h"
#include "customfdialog.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "pageitem_textframe.h"
#include "pageselector.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"

CanvasMode_FrameLinks::CanvasMode_FrameLinks(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	frameResizeHandle = -1;
}

inline bool CanvasMode_FrameLinks::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}

void CanvasMode_FrameLinks::drawControls(QPainter* p)
{
	commonDrawControls(p);
}

void CanvasMode_FrameLinks::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_FrameLinks::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void CanvasMode_FrameLinks::activate(bool fromGesture)
{
//	qDebug() << "CanvasMode_FrameLinks::activate" << fromGesture;
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
		m_canvas->m_viewMode.operItemResizeInEditMode = false;
		m_view->update();
	}
}

void CanvasMode_FrameLinks::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_FrameLinks::deactivate" << forGesture;
	m_view->redrawMarker->hide();
}

void CanvasMode_FrameLinks::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_view->stopDragTimer();
}

void CanvasMode_FrameLinks::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m->accept();
	commonMouseMove(m);

	if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton))
	{
		SeRx = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		SeRy = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		/*
		m_view->redrawMarker->setGeometry(QRect(Mxp, Myp, m->globalPos().x() - Mxp, m->globalPos().y() - Myp).normalized());
		*/
		QPoint startP = m_canvas->canvasToGlobal(m_doc->appMode == modeDrawTable? QPointF(Dxp, Dyp) : QPointF(Mxp, Myp));
		m_view->redrawMarker->setGeometry(QRect(startP, m->globalPos()).normalized());
		if (!m_view->redrawMarker->isVisible())
			m_view->redrawMarker->show();
		m_view->HaveSelRect = true;
		return;
	}
}

void CanvasMode_FrameLinks::mousePressEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());

	double Rxp = 0;
	double Ryp = 0;
	PageItem *currItem, *bb;
	m_canvas->PaintSizeRect(QRect());
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	Rxp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).x();
	Mxp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).y();
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
	switch (m_doc->appMode)
	{
		case modeLinkFrames:
			if (m->button() != Qt::LeftButton)
				break;
			currItem = m_doc->ElemToLink;
			if (currItem==NULL)
				break;
			SeleItem(m);
			if (GetItem(&bb) && (bb->asTextFrame()))
			{
				PageItem* bblast = bb;
				while (bblast->nextInChain())
					bblast = bblast->nextInChain();
				
				if (currItem->nextInChain() == 0 && bb->prevInChain() == 0 && currItem != bblast)
				{
					currItem->link(bb);
					// CB We need to do this because we draw in the order of the item list
					// Which is also item number list.. but #3488: we must also renumber the items
					if (bb->ItemNr < currItem->ItemNr)
					{
						m_doc->Items->insert(currItem->ItemNr+1, bb);
						bb = m_doc->Items->takeAt(bb->ItemNr);
						m_doc->renumberItemsInListOrder();
					}
					// m_view->updateContents();
					// link calls PageItem::update	
					// emit DocChanged();
					m_doc->ElemToLink = bb;
				}
				else if (currItem == bblast)
				{
					//CB Mouse is released when this messagebox takes focus
					m_canvas->m_viewMode.m_MouseButtonPressed = false;
					QMessageBox::warning(m_view, ScribusView::tr("Linking Text Frames"),
											 "<qt>" + ScribusView::tr("You are trying to link a frame to itself.") + "</qt>");
				}
				else
				{
					//CB Mouse is released when this messagebox takes focus
					m_canvas->m_viewMode.m_MouseButtonPressed = false;
					QMessageBox::warning(m_view, ScribusView::tr("Linking Text Frames"),
										 "<qt>" + ScribusView::tr("You are trying to link a frame which is already linked.") + "</qt>");
				}
			}
			else
				m_doc->ElemToLink = NULL;
			break;
		case modeUnlinkFrames:
			if (m->button() != Qt::LeftButton)
				break;
			SeleItem(m);
			if (GetItem(&currItem) && (currItem->asTextFrame()))
			{
				if (currItem->prevInChain() != 0)
				{
					currItem->prevInChain()->unlink();
				}
				// unlink calls PageItem::update				emit DocChanged();
				// m_view->updateContents();
			}
			break;
	}
}

void CanvasMode_FrameLinks::mouseReleaseEvent(QMouseEvent *m)
{
#ifdef GESTURE_FRAME_PREVIEW
        clearPixmapCache();
#endif // GESTURE_FRAME_PREVIEW
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->stopDragTimer();
	//m_canvas->update(); //ugly in a mouseReleaseEvent!!!!!!!
	if (m_doc->appMode == modePanning)
	{
		m_ScMW->setAppMode(m_oldAppMode);
		return;
	}
	if ((GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!m_doc->DragP))
	{
		createContextMenu(currItem, mousePointDoc.x(), mousePointDoc.y());
		return;
	}
	if ((m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames))
	{
		m_view->updateContents();
		if (m_doc->ElemToLink != 0)
			return;
		else
		{
			m_view->requestMode(submodePaintingDone);
			return;
		}
	}
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
		m_doc->m_Selection->itemAt(i)->checkChanges(true);
	//Make sure the Zoom spinbox and page selector dont have focus if we click on the canvas
	m_view->zoomSpinBox->clearFocus();
	m_view->pageSelector->clearFocus();
}

void CanvasMode_FrameLinks::selectPage(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	Mxp = mousePointDoc.x(); //static_cast<int>(m->x()/m_canvas->scale());
	Myp = mousePointDoc.y(); //static_cast<int>(m->y()/m_canvas->scale());
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
				m_view->setMenTxt(j);
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
		m_view->setRulerPos(m_view->contentsX(), m_view->contentsY());
	}
}

//CB-->Doc/Fix
bool CanvasMode_FrameLinks::SeleItem(QMouseEvent *m)
{
	const unsigned SELECT_IN_GROUP = Qt::AltModifier;
	const unsigned SELECT_MULTIPLE = Qt::ShiftModifier;
	const unsigned SELECT_BENEATH = Qt::ControlModifier;
	QMatrix p;
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
		if (m_doc->bleeds.hasNonZeroValue() && m_doc->guidesSettings.showBleed)
			drawBleed = true;
		for (int a = docPageCount; a > -1; a--)
		{
			if (drawBleed)
				m_doc->getBleeds(a, pageBleeds);
			int x = static_cast<int>(m_doc->Pages->at(a)->xOffset() - pageBleeds.Left);
			int y = static_cast<int>(m_doc->Pages->at(a)->yOffset() - pageBleeds.Top);
			int w = static_cast<int>(m_doc->Pages->at(a)->width() + pageBleeds.Left + pageBleeds.Right);
			int h = static_cast<int>(m_doc->Pages->at(a)->height() + pageBleeds.Bottom + pageBleeds.Top);
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
				m_doc->setCurrentPage(m_doc->Pages->at(unsigned(pgNum)));
				m_view->setMenTxt(unsigned(pgNum));
				m_view->DrawNew();
			}
		}
		m_view->setRulerPos(m_view->contentsX(), m_view->contentsY());
	}
	
	currItem = NULL;
	if ((m->modifiers() & SELECT_BENEATH) != 0)
	{
		for (int i=0; i < m_doc->m_Selection->count(); ++i)
		{
			if (m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), m_doc->m_Selection->itemAt(i)) >= 0)
			{
				currItem = m_doc->m_Selection->itemAt(i);
//				qDebug() << "select item: found BENEATH" << currItem << "groups" << currItem->Groups.count();
				if (currItem->Groups.count() > 0)
				{
					m_doc->m_Selection->delaySignalsOn();
					for (int ga=0; ga<m_doc->Items->count(); ++ga)
					{
						PageItem* item = m_doc->Items->at(ga);
						if (item->Groups.count() != 0)
						{
							if (item->Groups.top() == currItem->Groups.top())
							{
								if (m_doc->m_Selection->findItem(item) >= 0)
								{
									m_doc->m_Selection->removeItem(item);
								}
							}
						}
					}
					m_doc->m_Selection->delaySignalsOff();
				}
				else
				{
					m_doc->m_Selection->removeItem(currItem);
				}
				break;
			}
//			else
//				qDebug() << "select item: not BENEATH" << QPointF(mousePointDoc.x(),mousePointDoc.y()) 
//					<< m_doc->m_Selection->itemAt(i)->getTransform() 
//					<< m_doc->m_Selection->itemAt(i)->getBoundingRect();
		}
	}
	else if ( (m->modifiers() & SELECT_MULTIPLE) == Qt::NoModifier || (m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames) )
	{
		m_view->Deselect(false);
	}
	
//	qDebug() << "select item: beneath" << (m->modifiers() & SELECT_BENEATH) << currItem 
//		<< "multi" << (m->modifiers() & SELECT_MULTIPLE)
//		<< "current sel" << m_doc->m_Selection->count();
	currItem = m_canvas->itemUnderCursor(m->globalPos(), currItem, (m->modifiers() & SELECT_IN_GROUP));
//	qDebug() << "item under cursor: " << currItem;
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
			//CB: #7186: This was prependItem, does not seem to need to be anymore with current select code
			m_doc->m_Selection->addItem(currItem);
			if ( (m->modifiers() & SELECT_IN_GROUP) && (!currItem->isGroupControl))
			{
				currItem->isSingleSel = true;
			}
			else if (currItem->Groups.count() > 0)
			{
				for (int ga=0; ga<m_doc->Items->count(); ++ga)
				{
					PageItem* item = m_doc->Items->at(ga);
					if (item->Groups.count() != 0)
					{
						if (item->Groups.top() == currItem->Groups.top())
						{
							if (item->ItemNr != currItem->ItemNr)
							{
								if (m_doc->m_Selection->findItem(item) == -1)
								{
									m_doc->m_Selection->addItem(item, true);
								}
							}
							item->isSingleSel = false;
						}
					}
				}
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
			m_doc->m_Selection->setGroupRect();
			double x, y, w, h;
			m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			//	emit ItemPos(x, y);
			//	emit ItemGeom(w, h);
			m_view->getGroupRectScreen(&x, &y, &w, &h);
			//	m_view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
			//	emit HaveSel(currItem->itemType());
		}
		if (m_doc->m_Selection->count() == 1)
		{
			frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
			if ((frameResizeHandle == Canvas::INSIDE) && (!currItem->locked()))
				qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
		}
		else
		{
			qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
			m_canvas->m_viewMode.operItemResizing = false;
		}
		return true;
	}
	//m_doc->m_Selection->setIsGUISelection(true);
	m_doc->m_Selection->connectItemToGUI();
	if ( !(m->modifiers() & SELECT_MULTIPLE) || (m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames))
		m_view->Deselect(true);
	return false;
}

void CanvasMode_FrameLinks::createContextMenu(PageItem* currItem, double mx, double my)
{
	ContextMenu* cmen=NULL;
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	m_view->setObjectUndoMode();
	Mxp = mx;
	Myp = my;
	if(currItem!=NULL)
	{
		cmen = new ContextMenu(*(m_doc->m_Selection), m_ScMW, m_doc);
		cmen->exec(QCursor::pos());
		m_view->setGlobalUndoMode();
		delete cmen;
	}
}
