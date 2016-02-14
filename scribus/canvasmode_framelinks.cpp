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

#include "appmodes.h"
#include "canvas.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "pageitem_textframe.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/contextmenu.h"
#include "ui/pageselector.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"



CanvasMode_FrameLinks::CanvasMode_FrameLinks(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	m_Mxp = m_Myp = -1;
	m_Dxp = m_Dyp = -1;
	m_frameResizeHandle = -1;
}

inline bool CanvasMode_FrameLinks::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}

void CanvasMode_FrameLinks::drawControls(QPainter* p)
{
	commonDrawControls(p, false);
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
	m_Mxp = m_Myp = -1;
	m_Dxp = m_Dyp = -1;
	m_frameResizeHandle = -1;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_FrameLinks::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_FrameLinks::deactivate" << forGesture;
	m_view->setRedrawMarkerShown(false);
}

void CanvasMode_FrameLinks::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
//	m_view->stopDragTimer();
}

void CanvasMode_FrameLinks::mouseMoveEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m->accept();

	if (commonMouseMove(m))
		return;
	if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton))
	{
		QPoint startP = m_canvas->canvasToGlobal(m_doc->appMode == modeDrawTable2 ? QPointF(m_Dxp, m_Dyp) : QPointF(m_Mxp, m_Myp));
		m_view->redrawMarker->setGeometry(QRect(m_view->mapFromGlobal(startP), m_view->mapFromGlobal(m->globalPos())).normalized());
		m_view->setRedrawMarkerShown(true);
		m_view->HaveSelRect = true;
		return;
	}
}

void CanvasMode_FrameLinks::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());

	double Rxp = 0;
	double Ryp = 0;
	m_canvas->PaintSizeRect(QRect());
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	m_Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	m_Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	Rxp = m_doc->ApplyGridF(FPoint(m_Mxp, m_Myp)).x();
	m_Mxp = qRound(Rxp);
	Ryp = m_doc->ApplyGridF(FPoint(m_Mxp, m_Myp)).y();
	m_Myp = qRound(Ryp);
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}
	PageItem *currItem=NULL, *bb=NULL;
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
				
				if (currItem->nextInChain() == NULL && currItem != bblast) //possibility to insert empty frames into chain
				{
					if (bb->prevInChain() != NULL)
					{
						ScMessageBox msgBox(QMessageBox::Question, tr("Linking Text Frames"),
										   "<qt>" + tr("You are trying to insert a frame into an existing text chain, where would you like to insert it?") + "<qt>");
						QPushButton *cancelButton = msgBox.addButton(CommonStrings::tr_Cancel, QMessageBox::RejectRole);
						QPushButton *beforeButton = msgBox.addButton(tr("Before"), QMessageBox::AcceptRole);
						QPushButton *afterButton = msgBox.addButton(tr("After"), QMessageBox::AcceptRole);
						msgBox.setDefaultBatchButton(afterButton);
						msgBox.exec();
						if ((QPushButton *) msgBox.clickedButton() == cancelButton)
							break;
						if ((QPushButton *) msgBox.clickedButton() == beforeButton)
						{
							if (currItem->prevInChain())
								currItem->prevInChain()->unlink();
							PageItem *prev = bb->prevInChain();
							prev->unlink();
							prev->link(currItem);
						}
						else if ((QPushButton *) msgBox.clickedButton() == afterButton)
						{
							if (bb->nextInChain() != NULL)
							{
								if (currItem->prevInChain())
									currItem->prevInChain()->unlink();
								PageItem *next = bb->nextInChain();
								bb->unlink();
								bb->link(currItem);
								bb = next;
							}
							else
							{ // link at end of last frame in chain - switch currItem <-> bb
								PageItem* tmp = currItem;
								currItem = bb;
								bb = tmp;
							}
						}
					}
					if (bb->prevInChain())
						bb->prevInChain()->unlink();
					currItem->link(bb);

					/*
					// #133881: Remove item renumbering on link as 1.5.x has better drawing methods
					// than 1.3.3.x did at the time (#3488). Leaving in in case we find we need it back
					//
					int cid = m_doc->Items->indexOf(currItem);
					int bid = m_doc->Items->indexOf(bb);
					// CB We need to do this because we draw in the order of the item list
					// Which is also item number list.. but #3488: we must also renumber the items
					if (bid < cid)
					{
						m_doc->Items->insert(cid+1, bb);
						bb = m_doc->Items->takeAt(bid);
					}
					*/

					// m_view->updateContents();
					// link calls PageItem::update	
					//ECE no, link() not force update
					currItem->update();
					m_view->DrawNew();
					//emit DocChanged();
					m_doc->ElemToLink = bb;
				}
				else if (currItem == bblast)
				{
					//CB Mouse is released when this messagebox takes focus
					m_canvas->m_viewMode.m_MouseButtonPressed = false;
					ScMessageBox::warning(m_view, tr("Linking Text Frames"),
										"<qt>" + tr("You are trying to link a frame to itself.") + "</qt>");
				}
				else
				{
					//CB Mouse is released when this messagebox takes focus
					m_canvas->m_viewMode.m_MouseButtonPressed = false;
					ScMessageBox::warning(m_view, tr("Linking Text Frames"),
										 "<qt>" + tr("You are trying to link a non-empty frame to frame which is already linked.") + "</qt>");
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
//	m_view->stopDragTimer();
	//m_canvas->update(); //ugly in a mouseReleaseEvent!!!!!!!
	if ((GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!m_doc->DragP))
	{
		createContextMenu(currItem, mousePointDoc.x(), mousePointDoc.y());
		return;
	}
	if ((m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames))
	{
		m_view->updateContents();
		if (!PrefsManager::instance()->appPrefs.uiPrefs.stickyTools || m_doc->ElemToLink == 0)
			m_view->requestMode(submodePaintingDone);
		return;
	}
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
		m_doc->m_Selection->itemAt(i)->checkChanges(true);
	//Make sure the Zoom spinbox and page selector don't have focus if we click on the canvas
	m_view->m_ScMW->zoomSpinBox->clearFocus();
	m_view->m_ScMW->pageSelector->clearFocus();
}

void CanvasMode_FrameLinks::selectPage(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_Mxp = mousePointDoc.x(); //static_cast<int>(m->x()/m_canvas->scale());
	m_Myp = mousePointDoc.y(); //static_cast<int>(m->y()/m_canvas->scale());
	m_doc->nodeEdit.deselect();
	m_view->Deselect(false);
	if (!m_doc->masterPageMode())
	{
		int i = m_doc->OnPage(m_Mxp, m_Myp);
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
		m_view->setRulerPos(m_view->contentsX(), m_view->contentsY());
	}
}

//CB-->Doc/Fix
bool CanvasMode_FrameLinks::SeleItem(QMouseEvent *m)
{
	const unsigned SELECT_IN_GROUP = Qt::AltModifier;
	const unsigned SELECT_MULTIPLE = Qt::ShiftModifier;
	const unsigned SELECT_BENEATH = Qt::ControlModifier;
	QTransform p;
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_Mxp = mousePointDoc.x(); //m->x()/m_canvas->scale());
	m_Myp = mousePointDoc.y(); //m->y()/m_canvas->scale());
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
				m_doc->setCurrentPage(m_doc->Pages->at(unsigned(pgNum)));
				m_view->m_ScMW->slotSetCurrentPage(unsigned(pgNum));
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
				m_doc->m_Selection->removeItem(currItem);
				break;
			}
		}
	}
	else if ( (m->modifiers() & SELECT_MULTIPLE) == Qt::NoModifier || (m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames) )
	{
		m_view->Deselect(false);
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
			//CB: #7186: This was prependItem, does not seem to need to be anymore with current select code
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
			m_doc->m_Selection->setGroupRect();
			double x, y, w, h;
			m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			m_view->getGroupRectScreen(&x, &y, &w, &h);
		}
		if (m_doc->m_Selection->count() == 1)
		{
			m_frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
			if ((m_frameResizeHandle == Canvas::INSIDE) && (!currItem->locked()))
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
		m_view->Deselect(true);
	return false;
}

void CanvasMode_FrameLinks::createContextMenu(PageItem* currItem, double mx, double my)
{
	ContextMenu* cmen=NULL;
	m_view->setObjectUndoMode();
	m_Mxp = mx;
	m_Myp = my;
	if(currItem!=NULL)
	{
		cmen = new ContextMenu(*(m_doc->m_Selection), m_ScMW, m_doc);
		cmen->exec(QCursor::pos());
		m_view->setGlobalUndoMode();
		delete cmen;
	}
}
