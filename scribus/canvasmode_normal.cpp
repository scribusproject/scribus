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


#include "canvasmode_normal.h"

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
#include <QToolTip>
#include <QWidgetAction>
#include <QDrag>
#include <QDebug>

#include "appmodes.h"
#include "canvas.h"
#include "canvasgesture_linemove.h"
#include "canvasgesture_resize.h"
#include "canvasgesture_rulermove.h"
#include "fileloader.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hyphenator.h"
#include "iconmanager.h"
#include "loadsaveplugin.h"
#include "pageitem_arc.h"
#include "pageitem_line.h"
#include "pageitem_regularpolygon.h"
#include "pageitem_textframe.h"
#include "plugins/formatidlist.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scmimedata.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/aligndistribute.h"
#include "ui/contextmenu.h"
#include "ui/customfdialog.h"
#include "ui/insertTable.h"
#include "ui/pageselector.h"
#include "ui/polyprops.h"
#include "ui/propertiespalette.h"
#include "ui/propertiespalette_line.h"
#include "ui/propertiespalette_text.h"
#include "ui/propertiespalette_xyz.h"
#include "undomanager.h"
#include "util.h"
#include "util_math.h"

CanvasMode_Normal::CanvasMode_Normal(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	m_frameResizeHandle = -1;
	m_dragConstrainInitPtX = 0;
	m_dragConstrainInitPtY = 0;
	m_lastPosWasOverGuide = false;
	m_shiftSelItems = false;
	m_resizeGesture = NULL;
	m_lineMoveGesture  = NULL;
	m_guideMoveGesture = NULL;
	m_mousePressPoint.setXY(0, 0);
	m_mouseCurrentPoint.setXY(0, 0);
	m_mouseSavedPoint.setXY(0, 0);
	m_objectDeltaPos.setXY(0,0 );
	ySnap = 0;
	xSnap = 0;
	m_hoveredItem = NULL;
}

inline bool CanvasMode_Normal::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}

void CanvasMode_Normal::drawControls(QPainter* p)
{
//	qDebug() << "CanvasMode_Normal::drawControls";
	if (m_canvas->m_viewMode.operItemMoving)
		drawOutline(p, 1.0, 1.0, m_objectDeltaPos.x(), m_objectDeltaPos.y());
	else
		drawSelection(p, !m_doc->drawAsPreview);
}

void CanvasMode_Normal::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_Normal::leaveEvent(QEvent *e)
{
}


void CanvasMode_Normal::activate(bool fromGesture)
{
//	qDebug() << "CanvasMode_Normal::activate" << fromGesture;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	m_mousePressPoint.setXY(0, 0);
	m_mouseCurrentPoint.setXY(0, 0);
	m_mouseSavedPoint.setXY(0, 0);
	m_objectDeltaPos.setXY(0,0 );
	m_frameResizeHandle = -1;
	m_shiftSelItems = false;
	m_hoveredItem = NULL;
	setModeCursor();
	if (fromGesture)
	{
		m_view->update();
	}
}

void CanvasMode_Normal::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_Normal::deactivate" << forGesture;
	m_view->setRedrawMarkerShown(false);
}

void CanvasMode_Normal::mouseDoubleClickEvent(QMouseEvent *m)
{
	PageItem *currItem = 0;
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	if (m_doc->drawAsPreview)
	{
		if (GetItem(&currItem))
		{
			if (currItem->isAnnotation())
			{
				if (currItem->annotation().Type() == Annotation::Text)
				{
					currItem->annotation().setOpen(!currItem->annotation().IsOpen());
					currItem->asTextFrame()->setTextAnnotationOpen(currItem->annotation().IsOpen());
					currItem->update();
					m_view->updateContents();
				}
			}
		}
		return;
	}
	m_canvas->resetRenderMode();
//	m_view->stopDragTimer();
	if (m_doc->m_Selection->isMultipleSelection())
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
	if (GetItem(&currItem))
	{
		if (currItem->asLatexFrame()) 
		{
			if (currItem->locked()) // || (!currItem->ScaleType))
			{
				return;
			}
			if (currItem->imageVisible())
				m_view->requestMode(modeEdit);
		}
#ifdef HAVE_OSG
		else if (currItem->asOSGFrame())
		{
			m_view->requestMode(submodeEditExternal);
		}
#endif
		else if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::PolyLine) || (currItem->itemType() == PageItem::Group) || (currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::PathText))
		{
			if (currItem->locked()) //|| (!currItem->ScaleType))
			{
				//mousePressEvent(m);
				return;
			}
			//If we double click on an image frame and theres no image assigned, open the
			//load picture dialog, else put it into edit mode if the frame is set to show the image
			if (currItem->itemType() == PageItem::ImageFrame)
			{
				if (currItem->Pfile.isEmpty())
					m_view->requestMode(submodeLoadPic);
				else if (!currItem->imageIsAvailable)
					m_view->requestMode(submodeStatusPic);
				else if (currItem->imageVisible())
					m_view->requestMode(modeEdit);
 			}
 			else if (currItem->itemType() == PageItem::TextFrame)
			{
 				m_view->requestMode(modeEdit);
			}
			else
			{
				m_view->requestMode(modeEditClip);
				m_ScMW->scrActions["itemUngroup"]->setEnabled(false);
			}
		}
		else if (currItem->itemType() == PageItem::TextFrame)
		{
			// See if double click was on a frame handle
			FPoint p = m_canvas->globalToCanvas(m->globalPos());
			Canvas::FrameHandle fh = m_canvas->frameHitTest(QPointF(p.x(),p.y()), currItem);
			//CB old code
			//emit currItem->isAnnotation() ? AnnotProps() : Amode(modeEdit);
			//mousePressEvent(m);
			//CB if annotation, open the annotation dialog
			if (currItem->isAnnotation())
			{
				m_view->requestMode(submodeAnnotProps);
				//mousePressEvent(m);
			}
			else if (fh == Canvas::SOUTH)
				currItem->asTextFrame()->setTextFrameHeight();
			//else if not in mode edit, set mode edit
			else if (m_doc->appMode != modeEdit)
			{
				m_view->requestMode(modeEdit);
				m_view->slotSetCurs(m->globalPos().x(), m->globalPos().y());
				//used for updating Notes Styles Editor and menu actions for marks
				//if cursor is in mark`s place
				m_ScMW->setTBvals(currItem);
				//CB ignore the double click and go with a single one
				//if we werent in mode edit before.
				//unsure if this is correct, but its ok given we had no
				//double click select until now.
//				mousePressEvent(m);
			}
		}
		else if (currItem->asSymbolFrame())
		{
			if (!m_doc->symbolEditMode())
				m_view->requestMode(submodeEditSymbol);
		}
		else if (currItem->asArc())
		{
			m_view->requestMode(modeEditArc);
		}
		else if (currItem->asRegularPolygon())
		{
			m_view->requestMode(modeEditPolygon);
		}
		else if (currItem->asSpiral())
		{
			m_view->requestMode(modeEditSpiral);
		}
		else if (currItem->asTable())
		{
			m_view->requestMode(modeEditTable);
			m_view->slotSetCurs(m->globalPos().x(), m->globalPos().y());
		}
	}
}


void CanvasMode_Normal::mouseMoveEvent(QMouseEvent *m)
{
// 	qDebug()<<"CanvasMode_Normal::mouseMoveEvent";
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m_lastPosWasOverGuide = false;
	double newX, newY;
	PageItem *currItem=NULL;
	bool erf = false;
	m->accept();
//	qDebug() << "legacy mode move:" << m->x() << m->y() << m_canvas->globalToCanvas(m->globalPos()).x() << m_canvas->globalToCanvas(m->globalPos()).y();
//	emit MousePos(m->x()/m_canvas->scale(),// + m_doc->minCanvasCoordinate.x(), 
//				  m->y()/m_canvas->scale()); // + m_doc->minCanvasCoordinate.y());

	if (commonMouseMove(m))
		return;
//	m_mouseCurrentPoint = mousePointDoc;
	bool movingOrResizing = (m_canvas->m_viewMode.operItemMoving || m_canvas->m_viewMode.operItemResizing);
	bool mouseIsOnPage    = (m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y()) != -1);
	if ((m_doc->guidesPrefs().guidesShown) && (!m_doc->GuideLock) && (!movingOrResizing) && (mouseIsOnPage) )
	{
		// #9002: Resize points undraggable when object is aligned to a guide
		// Allow item resize when guides are aligned to item while preserving
		// ability to drag guide when guis is in foreground and inside selection
		bool enableGuideGesture(false);
		Canvas::FrameHandle frameResizeHandle(Canvas::OUTSIDE);
		if (m_doc->m_Selection->count() > 0)
		{
			double gx(0.0), gy(0.0), gw(0.0), gh(0.0);
			m_doc->m_Selection->setGroupRect();
			m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
			frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), QRectF(gx, gy, gw, gh));
		}
		enableGuideGesture |= (frameResizeHandle == Canvas::OUTSIDE);
		enableGuideGesture |= ((m_doc->guidesPrefs().renderStackOrder.indexOf(3) > m_doc->guidesPrefs().renderStackOrder.indexOf(4)) && (frameResizeHandle == Canvas::INSIDE));
		if (enableGuideGesture)
		{
			if (!m_guideMoveGesture)
			{
				m_guideMoveGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
				connect(m_guideMoveGesture,SIGNAL(guideInfo(int, qreal)), m_ScMW->alignDistributePalette,SLOT(setGuide(int, qreal)));
			}
			if (m_guideMoveGesture->mouseHitsGuide(mousePointDoc))
			{
				m_lastPosWasOverGuide = true;
				switch (m_guideMoveGesture->getMode())
				{
					case RulerGesture::HORIZONTAL:
						m_view->setCursor(QCursor(Qt::SplitVCursor));
						break;
					case RulerGesture::VERTICAL:
						m_view->setCursor(QCursor(Qt::SplitHCursor));
						break;
					default:
						m_view->setCursor(QCursor(Qt::ArrowCursor));
				}
				return;
			}
			// Here removed a bunch of comments which made reading code difficult,
			// there is svn for tracking changes after all. pm
			m_view->setCursor(QCursor(Qt::ArrowCursor));
		}
	}
	else if (!mouseIsOnPage)
	{
		if ((m_view->cursor().shape() == Qt::SplitHCursor) || (m_view->cursor().shape() == Qt::SplitVCursor))
			m_view->setCursor(QCursor(Qt::ArrowCursor));
	}
	//<<#10116 Show overflow counter HUD
	if (!movingOrResizing && mouseIsOnPage)
	{
		PageItem* hoveredItem = NULL;
		hoveredItem = m_canvas->itemUnderCursor(m->globalPos(), hoveredItem, m->modifiers());
		if (hoveredItem)
		{
			if (m_doc->drawAsPreview)
			{
				if (hoveredItem->isAnnotation())
				{
					QString toolT = "";
					if (!hoveredItem->annotation().ToolTip().isEmpty())
						toolT = hoveredItem->annotation().ToolTip();
					if (hoveredItem->annotation().Type() == Annotation::Button)
					{
						if (!hoveredItem->annotation().RollOver().isEmpty())
							toolT = hoveredItem->annotation().RollOver();
						else if (!hoveredItem->annotation().Down().isEmpty())
							toolT = hoveredItem->annotation().Down();
					}
					else if (hoveredItem->annotation().Type() == Annotation::Link)
					{
						if (hoveredItem->annotation().ActionType() == Annotation::Action_GoTo)
							toolT = QString( tr("Go to Page %1").arg(hoveredItem->annotation().Ziel() + 1));
						else if (hoveredItem->annotation().ActionType() == Annotation::Action_URI)
							toolT = QString( tr("Go to URL %1").arg(hoveredItem->annotation().Extern()));
						else if ((hoveredItem->annotation().ActionType() == Annotation::Action_GoToR_FileAbs) || (hoveredItem->annotation().ActionType() == Annotation::Action_GoToR_FileRel))
							toolT = QString( tr("Go to Page %1 in File %2").arg(hoveredItem->annotation().Ziel() + 1).arg(hoveredItem->annotation().Extern()));
						m_view->setCursor(QCursor(Qt::PointingHandCursor));
					}
					if (toolT.isEmpty())
						toolT = hoveredItem->itemText.plainText();
					if (!toolT.isEmpty())
						QToolTip::showText(m->globalPos() + QPoint(5, 5), toolT, m_canvas);
					if (hoveredItem != m_hoveredItem)
					{
						if (m_hoveredItem)
							handleMouseLeave(m_hoveredItem);
						handleMouseEnter(hoveredItem);
					}
					m_hoveredItem = hoveredItem;
					if ((hoveredItem->annotation().Type() == Annotation::Text) && (hoveredItem->annotation().IsOpen()))
					{
						if (m_view->moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::LeftButton) && (!hoveredItem->locked()))
						{
							double dx = mousePointDoc.x() - m_mouseCurrentPoint.x();
							double dy = mousePointDoc.y() - m_mouseCurrentPoint.y();
							hoveredItem->setXYPos(hoveredItem->xPos() + dx, hoveredItem->yPos() + dy, true);
							m_mouseCurrentPoint = mousePointDoc;
							m_canvas->resetRenderMode();
							m_canvas->setRenderModeUseBuffer(false);
							m_canvas->repaint();
						}
					}
				}
				else
				{
					if (QToolTip::isVisible())
						QToolTip::hideText();
					m_hoveredItem = NULL;
				}
			}
			else
			{
				if (hoveredItem->asTextFrame() && hoveredItem->frameOverflows())
				{
					if (m_canvas->cursorOverTextFrameControl(m->globalPos(), hoveredItem))
					{
						QToolTip::showText(m->globalPos() + QPoint(5, 5), tr("Overflow Characters: %1 (%2 White Spaces)\nClick to link to existing text frame or auto-create new linked text frame").arg(hoveredItem->frameOverflowCount()).arg(hoveredItem->frameOverflowBlankCount()), m_canvas);
					}
				}
				else
					if (QToolTip::isVisible())
						QToolTip::hideText();
			}
		}
		else
		{
			if (QToolTip::isVisible())
				QToolTip::hideText();
			if (m_doc->drawAsPreview)
			{
				if (m_hoveredItem)
					handleMouseLeave(m_hoveredItem);
				m_hoveredItem = NULL;
			}
		}
	}
	//>>#10116
	if (m_doc->drawAsPreview)
		return;
	m_mouseCurrentPoint = mousePointDoc;
	if ((GetItem(&currItem)) && (!m_shiftSelItems))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		// #0007865
		if (/*(((m_view->dragTimerElapsed()) && (m->buttons() & Qt::LeftButton)) ||*/
			(m_view->moveTimerElapsed())
			&& (m->buttons() & Qt::RightButton)
			&& (m_canvas->m_viewMode.m_MouseButtonPressed)
			&& (!m_doc->DragP)  
			&& (!(currItem->isSingleSel)))
		{
			// start drag
//			m_view->stopDragTimer();
			if ((fabs(m_mousePressPoint.x() - newX) > 10) || (fabs(m_mousePressPoint.y() - newY) > 10))
			{
				m_canvas->setRenderMode(Canvas::RENDER_NORMAL);
//				m_view->resetDragTimer();
				m_doc->DragP = true;
				m_doc->leaveDrag = false;
				m_doc->DraggedElem = currItem;
				m_doc->DragElements.clear();
				for (int dre=0; dre<m_doc->m_Selection->count(); ++dre)
					m_doc->DragElements.append(m_doc->m_Selection->itemAt(dre));
				ScElemMimeData* md = ScriXmlDoc::WriteToMimeData(m_doc, m_doc->m_Selection);
				QDrag* dr = new QDrag(m_view);
				dr->setMimeData(md);
				const QPixmap& pm = IconManager::instance()->loadPixmap("DragPix.xpm");
				dr->setPixmap(pm);
			//	dr->setDragCursor(pm, Qt::CopyAction);
			//	dr->setDragCursor(pm, Qt::MoveAction);
			//	dr->setDragCursor(pm, Qt::LinkAction);
				dr->exec();
				m_doc->DragP = false;
				m_doc->leaveDrag = false;
				m_canvas->m_viewMode.m_MouseButtonPressed = false;
				m_doc->DraggedElem = 0;
				m_doc->DragElements.clear();
				m_view->setCursor(QCursor(Qt::ArrowCursor));
				m_view->updateContents();
			}
			return;
		}
		if (m_doc->DragP)
			return;
		//Operations run here:
		//Item resize, esp after creating a new one
		if (m_view->moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::LeftButton) && 
			((m_doc->appMode == modeNormal)) && (!currItem->locked()))
		{
//			m_view->stopDragTimer();
			if (!m_canvas->m_viewMode.operItemResizing)
			{
				//Dragging an item (plus more?)
				newX = mousePointDoc.x(); //static_cast<int>(m->x()/sc);
				newY = mousePointDoc.y(); //static_cast<int>(m->y()/sc);
				m_canvas->m_viewMode.operItemMoving = true;
				if (!(m_doc->drawAsPreview && !m_doc->editOnPreview))
					m_view->setCursor(Qt::ClosedHandCursor);
				else
					m_view->setCursor(QCursor(Qt::ArrowCursor));
				erf = false;
				int dX = qRound(newX - m_mousePressPoint.x()), dY = qRound(newY - m_mousePressPoint.y());
				if (!m_doc->m_Selection->isMultipleSelection())
				{
					erf=true;
					currItem = m_doc->m_Selection->itemAt(0);
					//Control Alt drag image in frame without being in edit mode
					if ((currItem->asImageFrame()) && (m->modifiers() & Qt::ControlModifier) && (m->modifiers() & Qt::AltModifier))
					{
						currItem->moveImageInFrame(dX/currItem->imageXScale(),dY/currItem->imageYScale());
						m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
					}
					else
					{
						//Dragging orthogonally - Ctrl Drag
						if ((m->modifiers() & Qt::ControlModifier) && !(m->modifiers() & Qt::ShiftModifier) && !(m->modifiers() & Qt::AltModifier))
						{
							if (abs(dX) > abs(dY))
								dY=0;
							else
							if (abs(dY) > abs(dX))
								dX=0;
							erf=false;
							dX+=qRound(m_dragConstrainInitPtX-currItem->xPos());
							dY+=qRound(m_dragConstrainInitPtY-currItem->yPos());
						}
						double gx, gy, gh, gw;
						m_objectDeltaPos.setXY(dX, dY);
					//	m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
						m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
						// #10677 : temporary hack : we need to introduce the
						// concept of item snapping points to handle better
						// the various types of items
						if (currItem->isLine())
						{
							QPointF startPoint = currItem->asLine()->startPoint();
							QPointF endPoint   = currItem->asLine()->endPoint();
							gx = qMin(startPoint.x(), endPoint.x());
							gy = qMin(startPoint.y(), endPoint.y());
							gw = fabs(startPoint.x() - endPoint.x());
							gh = fabs(startPoint.y() - endPoint.y());
						}
						if (m_doc->SnapGuides)
						{
							double nx = gx + m_objectDeltaPos.x();
							double ny = gy + m_objectDeltaPos.y();
							double nxo = nx, nyo = ny;
							m_doc->ApplyGuides(&nx, &ny);
							m_objectDeltaPos += FPoint(nx - nxo, ny - nyo);
							nx = nxo = gx + gw + m_objectDeltaPos.x();
							ny = nyo = gy + gh + m_objectDeltaPos.y();
							m_doc->ApplyGuides(&nx, &ny);
							m_objectDeltaPos += FPoint(nx-nxo, ny-nyo);
							if (false)
							{
								nx = nxo = gx + gw/2 + m_objectDeltaPos.x();
								ny = nyo = gy + gh/2 + m_objectDeltaPos.y();
								m_doc->ApplyGuides(&nx, &ny);
								m_objectDeltaPos += FPoint(nx-nxo, ny-nyo);
							}
						}
						if (m_doc->SnapElement)
						{
							xSnap = 0;
							ySnap = 0;
							double snapWidth[] = {0,gw,gw/2};
							double snapHeight[] = {0,gh,gh/2};
							if (m_objectDeltaPos.x() < 0)
								std::swap(snapWidth[0],snapWidth[2]);
							if (m_objectDeltaPos.y() < 0)
								std::swap(snapHeight[0],snapHeight[2]);
							double nx,ny,nyo,nxo;
							for (int i = 0;i<3;i++)
							{
								nx = gx + snapWidth[i] + m_objectDeltaPos.x();
								ny = gy + snapHeight[i] + m_objectDeltaPos.y();
								nxo = nx, nyo = ny;
								m_doc->ApplyGuides(&nx, &ny,true);
								m_objectDeltaPos += FPoint(nx - nxo, ny - nyo);
								if (ny != nyo)
									ySnap = ny;
								if (nx != nxo)
									xSnap = nx;
							}
						}
						if (m_doc->SnapGrid)
						{
							m_doc->m_Selection->setGroupRect();
							double gx, gy, gh, gw, gxo, gyo;
							m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
							gx += m_objectDeltaPos.x();
							gy += m_objectDeltaPos.y();
							gxo = gx;
							gyo = gy;
							FPoint npx = m_doc->ApplyGridF(FPoint(gx, gy));
							FPoint npw = m_doc->ApplyGridF(FPoint(gx+gw, gy+gh));
							if ((fabs(gx-npx.x())) > (fabs((gx+gw)-npw.x())))
								gx = npw.x() - gw;
							else
								gx = npx.x();
							if ((fabs(gy-npx.y())) > (fabs((gy+gh)-npw.y())))
								gy = npw.y() - gh;
							else
								gy = npx.y();
							if ((fabs(gx - gxo) < (m_doc->guidesPrefs().guideRad) / m_canvas->scale()) && (fabs(gy - gyo) < (m_doc->guidesPrefs().guideRad) / m_canvas->scale()))
								m_objectDeltaPos += FPoint(gx-gxo, gy-gyo);
						}
					}
				}
				else
				{
					double gx, gy, gh, gw;
					m_doc->m_Selection->setGroupRect();
				//	m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
					int dX=qRound(newX - m_mousePressPoint.x()), dY=qRound(newY - m_mousePressPoint.y());
					erf = true;
					if (m->modifiers() & Qt::ControlModifier)
					{
						if (abs(dX)>abs(dY))
							dY=0;
						else
						if (abs(dY)>abs(dX))
							dX=0;
						erf=false;
						dX+=m_dragConstrainInitPtX-qRound(gx);
						dY+=m_dragConstrainInitPtY-qRound(gy);
					}
					m_objectDeltaPos.setXY(dX, dY);
					if (m_doc->SnapGuides)
					{
						double nx = gx + m_objectDeltaPos.x();
						double ny = gy + m_objectDeltaPos.y();
						double nxo = nx, nyo = ny;
						m_doc->ApplyGuides(&nx, &ny);
						m_objectDeltaPos += FPoint(nx - nxo, ny - nyo);
						nx = nxo = gx + gw + m_objectDeltaPos.x();
						ny = nyo = gy + gh + m_objectDeltaPos.y();
						m_doc->ApplyGuides(&nx, &ny);
						m_objectDeltaPos += FPoint(nx-nxo, ny-nyo);
						if (false)
						{
							nx = nxo = gx + gw/2 + m_objectDeltaPos.x();
							ny = nyo = gy + gh/2 + m_objectDeltaPos.y();
							m_doc->ApplyGuides(&nx, &ny);
							m_objectDeltaPos += FPoint(nx-nxo, ny-nyo);
						}
					}
					if (m_doc->SnapElement)
					{
						xSnap = 0;
						ySnap = 0;
						double snapWidth[] = {0,gw,gw/2};
						double snapHeight[] = {0,gh,gh/2};
						if (m_objectDeltaPos.x() <0 )
							std::swap(snapWidth[0],snapWidth[2]);
						if (m_objectDeltaPos.y() < 0)
							std::swap(snapHeight[0],snapHeight[2]);
						double nx,ny,nyo,nxo;
						for (int i = 0;i<3;i++)
						{
							nx = gx + snapWidth[i] + m_objectDeltaPos.x();
							ny = gy + snapHeight[i] + m_objectDeltaPos.y();
							nxo = nx, nyo = ny;
							m_doc->ApplyGuides(&nx, &ny,true);
							m_objectDeltaPos += FPoint(nx - nxo, ny - nyo);
							if (ny != nyo)
								ySnap = ny;
							if (nx != nxo)
								xSnap = nx;
						}
					}
					if (m_doc->SnapGrid)
					{
						double gx, gy, gh, gw, gxo, gyo;
						m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
						gx += m_objectDeltaPos.x();
						gy += m_objectDeltaPos.y();
						gxo = gx;
						gyo = gy;
						FPoint npx = m_doc->ApplyGridF(FPoint(gx, gy));
						FPoint npw = m_doc->ApplyGridF(FPoint(gx+gw, gy+gh));
						if ((fabs(gx-npx.x())) > (fabs((gx+gw)-npw.x())))
							gx = npw.x() - gw;
						else
							gx = npx.x();
						if ((fabs(gy-npx.y())) > (fabs((gy+gh)-npw.y())))
							gy = npw.y() - gh;
						else
							gy = npx.y();
						if ((fabs(gx - gxo) < (m_doc->guidesPrefs().guideRad) / m_canvas->scale()) && (fabs(gy - gyo) < (m_doc->guidesPrefs().guideRad) / m_canvas->scale()))
							m_objectDeltaPos += FPoint(gx-gxo, gy-gyo);
					}
				}
				if (erf)
				{
					m_mouseCurrentPoint.setXY(newX, newY);
				}
				
				{
					double gx, gy, gh, gw;
					m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
					m_doc->adjustCanvas(FPoint(gx,gy), FPoint(gx+gw, gy+gh));
					QPoint selectionCenter = m_canvas->canvasToLocal(QPointF(gx+gw/2, gy+gh/2));
					QPoint localMousePos = m_canvas->canvasToLocal(mousePointDoc);
					int localwidth = static_cast<int>(gw * m_canvas->scale());
					int localheight = static_cast<int>(gh * m_canvas->scale());
					if (localwidth > 200)
					{
						localwidth = 0;
						selectionCenter.setX(localMousePos.x());
					}
					if (localheight > 200)
					{
						localheight = 0;
						selectionCenter.setY(localMousePos.y());
					}
					m_view->ensureVisible(selectionCenter.x(), selectionCenter.y(), localwidth/2 + 20, localheight/2 + 20);
					m_canvas->repaint();
					m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					m_canvas->displayCorrectedXYHUD(m->globalPos(), gx + m_objectDeltaPos.x(), gy + m_objectDeltaPos.y());
				}
			}
		}
		if ((!m_canvas->m_viewMode.m_MouseButtonPressed) && (m_doc->appMode != modeDrawBezierLine))
		{
			if (m_doc->m_Selection->isMultipleSelection())
			{
//				QRect mpo = QRect(qRound(m->x()/m_canvas->scale())-m_doc->guidesPrefs().grabRad, qRound(m->y()/m_canvas->scale())-m_doc->guidesPrefs().grabRad, m_doc->guidesPrefs().grabRad*2, m_doc->guidesPrefs().grabRad*2);
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
					else
					{
						m_view->setCursor(QCursor(Qt::OpenHandCursor));
					}
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
				QTransform p;
				m_canvas->Transform(currItem, p);
				QRect mpo = QRect(m->x()-m_doc->guidesPrefs().grabRadius, m->y()-m_doc->guidesPrefs().grabRadius, m_doc->guidesPrefs().grabRadius*2, m_doc->guidesPrefs().grabRadius*2);
//				mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
				if ((QRegion(p.map(QPolygon(QRect(-3, -3, static_cast<int>(currItem->width()+6), static_cast<int>(currItem->height()+6))))).contains(mpo)))
				{
					QRect tx = p.mapRect(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
					if ((tx.intersects(mpo)) && (!currItem->locked()))
					{
						m_view->setCursor(QCursor(Qt::OpenHandCursor));
						if (!currItem->sizeLocked())
							m_view->HandleCurs(currItem, mpo);
					}
				}
				else
				{
//					setModeCursor();
				}
			}
			if (GetItem(&currItem) && (m_doc->appMode == modeNormal))
			{
				int how = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
				if (how > 0)
				{
					if (currItem->asLine())
						m_view->setCursor(QCursor(Qt::SizeAllCursor));
					else if (!currItem->locked() && !currItem->sizeLocked())
					{
						if ((!currItem->sizeHLocked() && !currItem->sizeVLocked()) || (currItem->sizeHLocked() && (how == 5 || how == 8))
							|| (currItem->sizeVLocked() && (how == 6 || how == 7)))
						setResizeCursor(how, currItem->rotation());
					}
				}
				else if (how == 0)
				{
					if (!(m_doc->drawAsPreview && !m_doc->editOnPreview))
						m_view->setCursor(QCursor(Qt::OpenHandCursor));
					else
						m_view->setCursor(QCursor(Qt::ArrowCursor));
				}
				else
				{
					m_view->setCursor(QCursor(Qt::ArrowCursor));
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
			m_mouseSavedPoint.setXY(newX, newY);
			QPoint startP = m_canvas->canvasToGlobal(m_mousePressPoint);
			m_view->redrawMarker->setGeometry(QRect(m_view->mapFromGlobal(startP), m_view->mapFromGlobal(m->globalPos())).normalized());
			m_view->setRedrawMarkerShown(true);
			m_view->HaveSelRect = true;
			return;
		}
	}
}

void CanvasMode_Normal::mousePressEvent(QMouseEvent *m)
{
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	PageItem *currItem;

	m_objectDeltaPos  = FPoint(0, 0);
	m_mousePressPoint = m_mouseCurrentPoint = mousePointDoc;
	m_mouseSavedPoint = mousePointDoc;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	QRect mpo(m->x()-m_doc->guidesPrefs().grabRadius, m->y()-m_doc->guidesPrefs().grabRadius, m_doc->guidesPrefs().grabRadius*2, m_doc->guidesPrefs().grabRadius*2);
//	mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));

	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}

	if ((GetItem(&currItem)) && (!m_lastPosWasOverGuide))
	{
		if ((currItem->asLine()) && (!m_doc->m_Selection->isMultipleSelection()) && (!m_doc->drawAsPreview))
		{
			if (!m_lineMoveGesture)
				m_lineMoveGesture = new LineMove(this);
			
			m_lineMoveGesture->mousePressEvent(m);
			if (m_lineMoveGesture->haveLineItem())
			{
				m_view->startGesture(m_lineMoveGesture);
				return;
			}
		}
		else
		{
			bool isMS=m_doc->m_Selection->isMultipleSelection();
			if ((isMS || (!isMS && (!currItem->locked() && !currItem->sizeLocked()))) && (!m_doc->drawAsPreview))
			{
				if (!m_resizeGesture)
					m_resizeGesture = new ResizeGesture(this);

				m_resizeGesture->mousePressEvent(m);
				if (m_resizeGesture->frameHandle() > 0)
				{
					m_view->startGesture(m_resizeGesture);
					return;
				}
			}
//#7928			else
//#7928				return;
		}
		if (!(m_doc->drawAsPreview && !m_doc->editOnPreview))
			m_view->setCursor(Qt::ClosedHandCursor);
		else
			m_view->setCursor(QCursor(Qt::ArrowCursor));
#if 1				
		if (m_doc->m_Selection->isMultipleSelection() && (!m_doc->drawAsPreview))
		{
			m_canvas->PaintSizeRect(QRect());
			double gx, gy, gh, gw;
			bool shiftSel = true;
			m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
			m_dragConstrainInitPtX = qRound(gx);
			m_dragConstrainInitPtY = qRound(gy);
			m_frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), QRectF(gx, gy, gw, gh));
			if (m_frameResizeHandle == Canvas::OUTSIDE ||
				(m_frameResizeHandle == Canvas::INSIDE && m->modifiers() != Qt::NoModifier))
			{
				m_frameResizeHandle = 0;
				m_doc->m_Selection->delaySignalsOn();
				m_view->updatesOn(false);
				shiftSel = SeleItem(m);
				m_view->updatesOn(true);
				m_doc->m_Selection->delaySignalsOff();
			}
			if (((m_doc->m_Selection->count() == 0) || (!shiftSel)) && (m->modifiers() == Qt::ShiftModifier))
			{
				m_shiftSelItems = true;
				m_mouseCurrentPoint = m_mousePressPoint = m_mouseSavedPoint = mousePointDoc;
			}
			else
				m_shiftSelItems = false;
			m_canvas->setRenderModeFillBuffer();
		}
		else // not multiple selection
		{
			m_dragConstrainInitPtX = qRound(currItem->xPos());
			m_dragConstrainInitPtY = qRound(currItem->yPos());
			
			// don't call SeleItem() without need here:
			m_frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
			//#6797
			if (m_frameResizeHandle <= 0 || m->modifiers() != Qt::NoModifier)
			{
				m_doc->m_Selection->delaySignalsOn();
				m_view->updatesOn(false);
				SeleItem(m); //Where we send the mouse press event to select an item
				if (GetItem(&currItem) && (!m_doc->drawAsPreview))
					m_frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
				else
					m_frameResizeHandle = 0;
				m_view->updatesOn(true);
				m_doc->m_Selection->delaySignalsOff();
			}
			if ((currItem && !currItem->locked() && m_frameResizeHandle > 0) == false && (!m_doc->drawAsPreview))
			{
				m_mouseCurrentPoint = m_mousePressPoint = m_mouseSavedPoint = mousePointDoc;
			}
		}
		m_canvas->setRenderModeFillBuffer();
#endif
	}
	else // !GetItem()
	{
		SeleItem(m);
		if (m_doc->m_Selection->count() == 0)
		{
			m_mouseCurrentPoint = m_mousePressPoint = m_mouseSavedPoint = mousePointDoc;
			m_view->redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
			m_view->setRedrawMarkerShown(true);
		}
		else
		{
			if (!(m_doc->drawAsPreview && !m_doc->editOnPreview))
				m_canvas->setRenderModeFillBuffer();
		}
	}
/*	if (m->button() == MidButton)
	{
		MidButt = true;
		if (m_doc->m_Selection->count() != 0)
			m_view->Deselect(true);
		DrawNew();
	} */
	if ((m_doc->m_Selection->count() != 0) && (m->button() == Qt::RightButton))
	{
		m_canvas->m_viewMode.m_MouseButtonPressed = true;
		m_mousePressPoint = m_mouseCurrentPoint;
	}
	if (m_doc->drawAsPreview && !m_doc->editOnPreview)
	{
		m_canvas->setRenderModeUseBuffer(false);
		if (m_doc->m_Selection->count() == 1)
		{
			currItem = m_doc->m_Selection->itemAt(0);
			if (currItem->isAnnotation())
			{
				if (currItem->annotation().Type() == Annotation::Checkbox)
					handleCheckBoxPress(currItem);
				else if (currItem->annotation().Type() == Annotation::RadioButton)
					handleRadioButtonPress(currItem);
				else if (currItem->annotation().Type() == Annotation::Button)
					handlePushButtonPress(currItem);
			}
		}
	}
// Commented out to fix bug #7865
//	if ((m_doc->m_Selection->count() != 0) && (m->button() == Qt::LeftButton) && (frameResizeHandle == 0))
//	{
//		m_view->startDragTimer();
//	}
	m_canvas->PaintSizeRect(QRect());
}



void CanvasMode_Normal::mouseReleaseEvent(QMouseEvent *m)
{
// 	qDebug("CanvasMode_Normal::mouseReleaseEvent");
#ifdef GESTURE_FRAME_PREVIEW
        clearPixmapCache();
#endif // GESTURE_FRAME_PREVIEW
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->setRedrawMarkerShown(false);
//	m_view->stopDragTimer();
	//m_canvas->update(); //ugly in a mouseReleaseEvent!!!!!!!
	if ((!GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!m_doc->DragP) && (!m_doc->drawAsPreview))
	{
		createContextMenu(NULL, mousePointDoc.x(), mousePointDoc.y());
		return;
	}
	if ((GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!m_doc->DragP) && (!(m_doc->drawAsPreview && !m_doc->editOnPreview)))
	{
		createContextMenu(currItem, mousePointDoc.x(), mousePointDoc.y());
		return;
	}
	//<<#10116: Click on overflow icon to get into link frame mode
	PageItem* clickedItem=NULL;
	clickedItem = m_canvas->itemUnderCursor(m->globalPos(), clickedItem, m->modifiers());
	if (clickedItem && clickedItem->asTextFrame() && (!clickedItem->isAnnotation()) && (!m_doc->drawAsPreview))
	{
		if (clickedItem->frameOverflows())
		{
			if (m_canvas->cursorOverTextFrameControl(m->globalPos(), clickedItem))
			{
				m_view->requestMode(modeLinkFrames);
				return;
			}
		}
	}
	//>>#10116
	if (m_view->moveTimerElapsed() && (GetItem(&currItem)))
	{
//		m_view->stopDragTimer();
		m_canvas->setRenderModeUseBuffer(false);
		if (!m_doc->m_Selection->isMultipleSelection())
		{
			m_doc->setRedrawBounding(currItem);
			currItem->OwnPage = m_doc->OnPage(currItem);
			m_canvas->m_viewMode.operItemResizing = false;
			if (currItem->asLine())
				m_view->updateContents();
		}
		if (m_canvas->m_viewMode.operItemMoving)
		{
			// we want to invalidate all frames under the moved frame
			// hm, I will try to be more explicit :) - pm
			int itemIndex = m_doc->Items->count();
			PageItem* underItem( m_canvas->itemUnderItem(currItem, itemIndex) );
			while(underItem)
			{
				if (underItem->asTextFrame())
					underItem->asTextFrame()->invalidateLayout(false);
				else
					underItem->invalidateLayout();
				
				underItem =  m_canvas->itemUnderItem(currItem, itemIndex);
			}
			
			m_view->updatesOn(false);
			if (!m_view->groupTransactionStarted())
			{
				m_view->startGroupTransaction(Um::Move, "", Um::IMove);
			}
			if (m_doc->m_Selection->isMultipleSelection())
			{
				m_doc->moveGroup(m_objectDeltaPos.x(), m_objectDeltaPos.y());
				m_doc->m_Selection->setGroupRect();
				double gx, gy, gh, gw;
				m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
				double nx = gx;
				double ny = gy;
				if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
				{
					FPoint npx = m_doc->ApplyGridF(FPoint(gx, gy));
					FPoint npw = m_doc->ApplyGridF(FPoint(gx+gw, gy+gh));
					if ((fabs(gx-npx.x())) > (fabs((gx+gw)-npw.x())))
						nx = npw.x() - gw;
					else
						nx = npx.x();
					if ((fabs(gy-npx.y())) > (fabs((gy+gh)-npw.y())))
						ny = npw.y() - gh;
					else
						ny = npx.y();
				}
				m_doc->moveGroup(nx-gx, ny-gy);
				m_doc->m_Selection->setGroupRect();
				m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
				nx = gx+gw;
				ny = gy+gh;
				if (m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
					m_doc->moveGroup(nx-(gx+gw), ny-(gy+gh));
				m_doc->m_Selection->setGroupRect();
			}
			else
			{
				currItem = m_doc->m_Selection->itemAt(0);
				m_doc->moveItem(m_objectDeltaPos.x(), m_objectDeltaPos.y(), currItem);
				if (m_doc->SnapGrid)
				{
					double nx = currItem->xPos();
					double ny = currItem->yPos();
					if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
					{
						m_doc->m_Selection->setGroupRect();
						double gx, gy, gh, gw, gxo, gyo;
						m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
						gxo = gx;
						gyo = gy;
						FPoint npx = m_doc->ApplyGridF(FPoint(gx, gy));
						FPoint npw = m_doc->ApplyGridF(FPoint(gx+gw, gy+gh));
						if ((fabs(gx-npx.x())) > (fabs((gx+gw)-npw.x())))
							gx = npw.x() - gw;
						else
							gx = npx.x();
						if ((fabs(gy-npx.y())) > (fabs((gy+gh)-npw.y())))
							gy = npw.y() - gh;
						else
							gy = npx.y();
						if ((fabs(gx - gxo) < (m_doc->guidesPrefs().guideRad) / m_canvas->scale()) && (fabs(gy - gyo) < (m_doc->guidesPrefs().guideRad) / m_canvas->scale()))
						{
							nx += (gx - gxo);
							ny += (gy - gyo);
						}
					}
					m_doc->moveItem(nx-currItem->xPos(), ny-currItem->yPos(), currItem);
				}
			}
			m_canvas->m_viewMode.operItemMoving = false;
			if (m_doc->m_Selection->isMultipleSelection())
			{
				double gx, gy, gh, gw;
				m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				FPoint maxSize(gx+gw+m_doc->scratch()->right(), gy+gh+m_doc->scratch()->bottom());
				FPoint minSize(gx-m_doc->scratch()->left(), gy-m_doc->scratch()->top());
				m_doc->adjustCanvas(minSize, maxSize);
			}
			m_doc->setRedrawBounding(currItem);
			currItem->OwnPage = m_doc->OnPage(currItem);
			if (currItem->OwnPage != -1)
			{
				m_doc->setCurrentPage(m_doc->Pages->at(currItem->OwnPage));
				m_view->m_ScMW->slotSetCurrentPage(currItem->OwnPage);
			}
			//CB done with emitAllToGUI
			//emit HaveSel();
			//EmitValues(currItem);
			//CB need this for? a moved item will send its new data with the new xpos/ypos emits
			//CB TODO And what if we have dragged to a new page. Items X&Y are not updated anyway now
			//currItem->emitAllToGUI();
			m_view->updatesOn(true);
			m_view->updateContents();
			m_doc->changed();
		}
	}
	//CB Drag selection performed here
	if (((m_doc->m_Selection->count() == 0) && (m_view->HaveSelRect) && (!m_view->MidButt)) || ((m_shiftSelItems) && (m_view->HaveSelRect) && (!m_view->MidButt)))
	{
		double dx = m_mouseSavedPoint.x() - m_mousePressPoint.x();
		double dy = m_mouseSavedPoint.y() - m_mousePressPoint.y();
		QRectF canvasSele = QRectF(m_mousePressPoint.x(), m_mousePressPoint.y(), dx, dy).normalized();
		QRectF localSele  = m_canvas->canvasToLocal(canvasSele).normalized();
		if (!m_doc->masterPageMode())
		{
			uint docPagesCount=m_doc->Pages->count();
			uint docCurrPageNo=m_doc->currentPageNumber();
			for (uint i = 0; i < docPagesCount; ++i)
			{
				ScPage*  page = m_doc->Pages->at(i);
				QRectF pageRect(page->xOffset(), page->yOffset(), page->width(), page->height());
				if (pageRect.intersects(canvasSele))
				{
					if (docCurrPageNo != i)
					{
						m_doc->setCurrentPage(m_doc->Pages->at(i));
						m_view->m_ScMW->slotSetCurrentPage(i);
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
				if ((m_doc->masterPageMode()) && (docItem->OnMasterPage != m_doc->currentPage()->pageName()))
					continue;
				QRect  apr2 = m_canvas->canvasToLocal( docItem->getCurrentBoundingRect(docItem->lineWidth()) );
				if ((localSele.contains(apr2)) && ((docItem->LayerID == m_doc->activeLayer()) || (m_doc->layerSelectable(docItem->LayerID))) && (!m_doc->layerLocked(docItem->LayerID)))
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
		m_shiftSelItems = false;
//		m_view->redrawMarker->hide();
		m_view->updateContents();
	}
	if (m_doc->appMode != modeEdit)
	{
		if (!PrefsManager::instance()->appPrefs.uiPrefs.stickyTools)
			m_view->requestMode(modeNormal);
		else
		{
			int appMode = m_doc->appMode;
			m_view->requestMode(appMode);
		}
	}
	if (GetItem(&currItem))
	{
	//	qApp->changeOverrideCursor(QCursor(Qt::OpenHandCursor));
		if (m_doc->m_Selection->count() > 1)
		{
			m_doc->m_Selection->setGroupRect();
			double x, y, w, h;
			m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			m_canvas->m_viewMode.operItemMoving = false;
			m_canvas->m_viewMode.operItemResizing = false;
			m_view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
			//Now unuseful as PropertiesPalette_XYZ::setCurrentItem() handles multiple selection
			//m_ScMW->propertiesPalette->setXY(x,y);
			//m_ScMW->propertiesPalette->setBH(w,h);
		}
		/*else
			currItem->emitAllToGUI();*/
	}
	else
		m_view->Deselect(true);
	xSnap = ySnap = 0;
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	m_shiftSelItems = false;
	if (m_view->groupTransactionStarted())
	{
		for (int i = 0; i < m_doc->m_Selection->count(); ++i)
			m_doc->m_Selection->itemAt(i)->checkChanges(true);
		m_view->endGroupTransaction();
	}
	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
		m_doc->m_Selection->itemAt(i)->checkChanges(true);
	//Commit drag created items to undo manager.
	if (m_doc->m_Selection->count() > 0)
	{
		if (m_doc->m_Selection->itemAt(0)!=NULL)
		{
			m_doc->itemAddCommit(m_doc->m_Selection->itemAt(0));
		}
	}
	//Make sure the Zoom spinbox and page selector don't have focus if we click on the canvas
	m_view->m_ScMW->zoomSpinBox->clearFocus();
	m_view->m_ScMW->pageSelector->clearFocus();
	if (m_doc->m_Selection->count() > 0)
	{
		if (m_doc->m_Selection->itemAt(0) != 0) // is there the old clip stored for the undo action
		{
			currItem = m_doc->m_Selection->itemAt(0);
			m_doc->nodeEdit.finishTransaction(currItem);
		}
	}
	if (m_doc->drawAsPreview && !m_doc->editOnPreview)
	{
		if (m_doc->m_Selection->count() == 1)
		{
			currItem = m_doc->m_Selection->itemAt(0);
			if (currItem->isAnnotation())
			{
				if (currItem->annotation().Type() == Annotation::Text)
				{
					if (currItem->annotation().IsOpen())
					{
						if (m_canvas->cursorOverFrameControl(m->globalPos(), QRectF(245, 20, 11, 11), currItem))
						{
							currItem->annotation().setOpen(false);
							currItem->asTextFrame()->setTextAnnotationOpen(false);
							currItem->update();
							m_view->updateContents();
						}
					}
				}
				if (currItem->annotation().Type() == Annotation::Link)
					handleLinkAnnotation(currItem);
				else if (currItem->annotation().Type() == Annotation::Checkbox)
					handleCheckBoxRelease(currItem);
				else if (currItem->annotation().Type() == Annotation::RadioButton)
					handleRadioButtonRelease(currItem);
				else if (currItem->annotation().Type() == Annotation::Button)
					handlePushButtonRelease(currItem);
			}
		}
	}
}

void CanvasMode_Normal::handleCheckBoxPress(PageItem* currItem)
{
	m_view->m_AnnotChanged = true;
	currItem->annotation().setOnState(true);
	currItem->update();
	if (currItem->annotation().ActionType() == Annotation::Action_JavaScript)
		handleJavaAction(currItem, Annotation::Java_PressButton);
	m_doc->regionsChanged()->update(currItem->getVisualBoundingRect());
}

void CanvasMode_Normal::handlePushButtonPress(PageItem* currItem)
{
	currItem->annotation().setOnState(true);
	currItem->update();
	if (currItem->annotation().ActionType() == Annotation::Action_JavaScript)
		handleJavaAction(currItem, Annotation::Java_PressButton);
	m_doc->regionsChanged()->update(currItem->getVisualBoundingRect());
}

void CanvasMode_Normal::handleRadioButtonPress(PageItem* currItem)
{
	m_view->m_AnnotChanged = true;
	if (currItem->isGroupChild())
	{
		PageItem *group = currItem->Parent->asGroupFrame();
		for (int a = 0; a < group->groupItemList.count(); a++)
		{
			PageItem *gItem = group->groupItemList[a];
			if ((gItem->isAnnotation()) && (gItem->annotation().Type() == Annotation::RadioButton))
			{
				gItem->update();
			}
		}
	}
	else
	{
		int op = currItem->OwnPage;
		uint docItemCount = m_doc->Items->count();
		for (uint i = 0; i < docItemCount; ++i)
		{
			PageItem *gItem = m_doc->Items->at(i);
			if ((gItem->isAnnotation()) && (gItem->annotation().Type() == Annotation::RadioButton) && (gItem->OwnPage == op))
			{
				gItem->update();
			}
		}
	}
	currItem->annotation().setOnState(true);
	currItem->update();
	if (currItem->annotation().ActionType() == Annotation::Action_JavaScript)
		handleJavaAction(currItem, Annotation::Java_PressButton);
	m_view->updateContents();
}

void CanvasMode_Normal::handleCheckBoxRelease(PageItem* currItem)
{
	m_view->m_AnnotChanged = true;
	currItem->annotation().setOnState(false);
	currItem->annotation().setCheckState(!currItem->annotation().IsChecked());
	if (currItem->annotation().ActionType() == Annotation::Action_JavaScript)
		handleJavaAction(currItem, Annotation::Java_ReleaseButton);
	currItem->update();
	m_doc->regionsChanged()->update(currItem->getVisualBoundingRect());
}

void CanvasMode_Normal::handlePushButtonRelease(PageItem* currItem)
{
	m_view->m_AnnotChanged = true;
	currItem->annotation().setOnState(false);
	currItem->update();
	switch (currItem->annotation().ActionType())
	{
		case Annotation::Action_JavaScript:
			handleJavaAction(currItem, Annotation::Java_ReleaseButton);
			break;
		case Annotation::Action_GoTo:
		case Annotation::Action_URI:
		case Annotation::Action_GoToR_FileAbs:
		case Annotation::Action_GoToR_FileRel:
			handleLinkAnnotation(currItem);
			break;
		case Annotation::Action_ResetForm:
			m_doc->ResetFormFields();
			break;
		case Annotation::Action_ImportData:
			m_doc->ImportData();
			break;
		case Annotation::Action_SubmitForm:
			m_doc->SubmitForm();
			break;
		case Annotation::Action_Named:
			handleNamedAction(currItem);
		default:
			break;
	}
}

void CanvasMode_Normal::handleRadioButtonRelease(PageItem* currItem)
{
	m_view->m_AnnotChanged = true;
	if (currItem->isGroupChild())
	{
		PageItem *group = currItem->Parent->asGroupFrame();
		for (int a = 0; a < group->groupItemList.count(); a++)
		{
			PageItem *gItem = group->groupItemList[a];
			if ((gItem->isAnnotation()) && (gItem->annotation().Type() == Annotation::RadioButton))
			{
				gItem->annotation().setCheckState(false);
				gItem->update();
			}
		}
	}
	else
	{
		int op = currItem->OwnPage;
		uint docItemCount = m_doc->Items->count();
		for (uint i = 0; i < docItemCount; ++i)
		{
			PageItem *gItem = m_doc->Items->at(i);
			if ((gItem->isAnnotation()) && (gItem->annotation().Type() == Annotation::RadioButton) && (gItem->OwnPage == op))
			{
				gItem->annotation().setCheckState(false);
				gItem->update();
			}
		}
	}
	currItem->annotation().setCheckState(true);
	currItem->annotation().setOnState(false);
	if (currItem->annotation().ActionType() == Annotation::Action_JavaScript)
		handleJavaAction(currItem, Annotation::Java_ReleaseButton);
	currItem->update();
	m_view->updateContents();
}

void CanvasMode_Normal::handleJavaAction(PageItem* currItem, int event)
{
	QString scriptCode = "";
	switch (event)
	{
		case Annotation::Java_PressButton:
			scriptCode = currItem->annotation().D_act();
			break;
		case Annotation::Java_ReleaseButton:
			scriptCode = currItem->annotation().Action();
			break;
		case Annotation::Java_EnterWidget:
			scriptCode = currItem->annotation().E_act();
			break;
		case Annotation::Java_LeaveWidget:
			scriptCode = currItem->annotation().X_act();
			break;
		case Annotation::Java_FocusIn:
			scriptCode = currItem->annotation().Fo_act();
			break;
		case Annotation::Java_FocusOut:
			scriptCode = currItem->annotation().Bl_act();
			break;
		case Annotation::Java_SelectionChg:
			scriptCode = currItem->annotation().K_act();
			break;
		case Annotation::Java_FieldFormat:
			scriptCode = currItem->annotation().F_act();
			break;
		case Annotation::Java_FieldValidate:
			scriptCode = currItem->annotation().V_act();
			break;
		case Annotation::Java_FieldCalculate:
			scriptCode = currItem->annotation().C_act();
			break;
		default:
			break;
	}
	if (!scriptCode.isEmpty())
	{
		// Execute the JavasScript Code
//		qDebug() << "JavaScript:" << scriptCode;
	}
}

void CanvasMode_Normal::handleNamedAction(PageItem* currItem)
{
	m_view->Deselect(true);
	QString name = currItem->annotation().Action();
	if (name == "FirstPage")
		m_view->GotoPage(0);
	else if (name == "PrevPage")
		m_view->GotoPage(qMax(0, m_doc->currentPageNumber()-1));
	else if (name == "NextPage")
		m_view->GotoPage(qMin(m_doc->currentPageNumber()+1, m_doc->Pages->count()-1));
	else if (name == "LastPage")
		m_view->GotoPage(m_doc->Pages->count()-1);
	else if (name == "Print")
		m_ScMW->slotFilePrint();
	else if (name == "Close")
		m_ScMW->slotFileClose();
	else if (name == "Quit")
		m_ScMW->slotFileQuit();
	else if (name == "FitHeight")
		m_ScMW->slotZoom(-100);
	else if (name == "FitWidth")
		m_ScMW->slotZoom(-200);
}

void CanvasMode_Normal::handleLinkAnnotation(PageItem* currItem)
{
	if (currItem->annotation().ActionType() == Annotation::Action_GoTo)
	{
		m_view->Deselect(true);
		if (currItem->annotation().Ziel() < m_doc->Pages->count())
			m_view->GotoPage(currItem->annotation().Ziel());
		else
		{
			QString message = tr("Page %1 does not exist!").arg(currItem->annotation().Ziel() + 1);
			ScMessageBox::warning(m_view, CommonStrings::trWarning, message);
		}
	}
	else if (currItem->annotation().ActionType() == Annotation::Action_URI)
	{
		QString message = tr("Link Target is Web URL.\nURL: %1").arg(currItem->annotation().Extern());
		ScMessageBox::information(m_view, tr("Information"), message);
	}
	else if ((currItem->annotation().ActionType() == Annotation::Action_GoToR_FileAbs) || (currItem->annotation().ActionType() == Annotation::Action_GoToR_FileRel))
	{
		QString message = tr("Link Target is external File.\nFile: %1\nPage: %2").arg(currItem->annotation().Extern()).arg(currItem->annotation().Ziel() + 1);
		ScMessageBox::information(m_view, tr("Information"), message);
	}
}

void CanvasMode_Normal::handleFocusOut(PageItem* currItem)
{
	if (currItem->annotation().ActionType() == Annotation::Action_JavaScript)
		handleJavaAction(currItem, Annotation::Java_FocusOut);
}

void CanvasMode_Normal::handleFocusIn(PageItem* currItem)
{
	if (currItem->annotation().ActionType() == Annotation::Action_JavaScript)
		handleJavaAction(currItem, Annotation::Java_FocusIn);
}

void CanvasMode_Normal::handleMouseLeave(PageItem* currItem)
{
	if (currItem->annotation().ActionType() == Annotation::Action_JavaScript)
		handleJavaAction(currItem, Annotation::Java_LeaveWidget);
}

void CanvasMode_Normal::handleMouseEnter(PageItem* currItem)
{
	if (currItem->annotation().ActionType() == Annotation::Action_JavaScript)
		handleJavaAction(currItem, Annotation::Java_EnterWidget);
}

void CanvasMode_Normal::keyPressEvent(QKeyEvent *e)
{
	commonkeyPressEvent_NormalNodeEdit(e);
}

void CanvasMode_Normal::keyReleaseEvent(QKeyEvent *e)
{
	commonkeyReleaseEvent(e);
}

//CB-->Doc/Fix
bool CanvasMode_Normal::SeleItem(QMouseEvent *m)
{
	PageItem *previousSelectedItem = NULL;
	if (m_doc->m_Selection->count() != 0)
		previousSelectedItem = m_doc->m_Selection->itemAt(0);
	m_canvas->m_viewMode.operItemSelecting = true;
	const unsigned SELECT_IN_GROUP = Qt::AltModifier; // Qt::MetaModifier;
	const unsigned SELECT_MULTIPLE = Qt::ShiftModifier;
	const unsigned SELECT_BENEATH = Qt::ControlModifier;
	QTransform p;
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	m_mouseCurrentPoint  = mousePointDoc;
	double grabRadius = m_doc->guidesPrefs().grabRadius / m_canvas->scale();
	int MxpS = static_cast<int>(mousePointDoc.x()); //m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	int MypS = static_cast<int>(mousePointDoc.y()); //m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	QRectF mpo(m_mouseCurrentPoint.x()-grabRadius, m_mouseCurrentPoint.y()-grabRadius, grabRadius*2, grabRadius*2);
	m_doc->nodeEdit.deselect();

	if (m_doc->guidesPrefs().renderStackOrder.indexOf(3) > m_doc->guidesPrefs().renderStackOrder.indexOf(4)) // guides are on foreground and want to be processed first
	{
		if ((m_doc->guidesPrefs().guidesShown) && (m_doc->OnPage(MxpS, MypS) != -1))
		{
			// #9002: Resize points undraggable when object is aligned to a guide
			// Allow item resize when guides are aligned to item while preserving
			// ability to drag guide when guis is in foreground and inside selection
			bool enableGuideGesture(true);
			if (m_doc->m_Selection->count() > 0)
			{
				double gx(0.0), gy(0.0), gw(0.0), gh(0.0);
				m_doc->m_Selection->setGroupRect();
				m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
				Canvas::FrameHandle frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), QRectF(gx, gy, gw, gh));
				enableGuideGesture = (frameResizeHandle == Canvas::INSIDE);
			}
			if (enableGuideGesture)
			{
				if (!m_guideMoveGesture)
				{
					m_guideMoveGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
					connect(m_guideMoveGesture,SIGNAL(guideInfo(int, qreal)), m_ScMW->alignDistributePalette,SLOT(setGuide(int, qreal)));
				}
				if ( (!m_doc->GuideLock) && (m_guideMoveGesture->mouseHitsGuide(mousePointDoc)) )
				{
					m_view->startGesture(m_guideMoveGesture);
					m_guideMoveGesture->mouseMoveEvent(m);
					m_doc->m_Selection->connectItemToGUI();
				//	qDebug()<<"Out Of SeleItem"<<__LINE__;
					return true;
				}
				else
				{
				// If we call startGesture now, a new guide is created each time.
				// ### could be a weakness to avoid calling it tho.
	 			//	m_view->startGesture(guideMoveGesture);
					m_guideMoveGesture->mouseSelectGuide(m);
				}
			}
		}
	}
	bool pageChanged(false);
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
				pageChanged = true;
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
	else if ( (m->modifiers() & SELECT_MULTIPLE) == Qt::NoModifier)
	{
		m_view->Deselect(false);
	}
	currItem = m_canvas->itemUnderCursor(m->globalPos(), currItem, ((m->modifiers() & SELECT_IN_GROUP) || (m_doc->drawAsPreview && !m_doc->editOnPreview)));
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
			if (m_doc->drawAsPreview && !m_doc->editOnPreview)
				m_doc->m_Selection->clear();
			m_doc->m_Selection->addItem(currItem);
			if (((m->modifiers() & SELECT_IN_GROUP) || (m_doc->drawAsPreview && !m_doc->editOnPreview)) && (!currItem->isGroup()))
			{
				currItem->isSingleSel = true;
			}
		}
		if (pageChanged)
		{
			m_canvas->setForcedRedraw(true);
			m_canvas->update();
		}
		else
			m_canvas->update();
		
		m_doc->m_Selection->delaySignalsOff();
		if (m_doc->m_Selection->count() > 1)
		{
			m_doc->beginUpdate();
			for (int aa = 0; aa < m_doc->m_Selection->count(); ++aa)
			{
				PageItem *bb = m_doc->m_Selection->itemAt(aa);
				bb->update();
			}
			m_doc->endUpdate();
			m_doc->m_Selection->setGroupRect();
			double x, y, w, h;
			m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			m_view->getGroupRectScreen(&x, &y, &w, &h);
		}
		if (previousSelectedItem != NULL)
		{
			if (currItem != previousSelectedItem)
			{
				handleFocusOut(previousSelectedItem);
				handleFocusIn(currItem);
			}
		}
		else
			handleFocusIn(currItem);
		return true;
	}
	if ((m_doc->guidesPrefs().guidesShown) /*&& (!m_doc->GuideLock)*/ && (m_doc->OnPage(MxpS, MypS) != -1))
	{
		if (!m_guideMoveGesture)
		{
			m_guideMoveGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
			connect(m_guideMoveGesture,SIGNAL(guideInfo(int, qreal)), m_ScMW->alignDistributePalette,SLOT(setGuide(int, qreal)));
		}
		if ( (!m_doc->GuideLock) && (m_guideMoveGesture->mouseHitsGuide(mousePointDoc)) )
		{
			m_view->startGesture(m_guideMoveGesture);
			m_guideMoveGesture->mouseMoveEvent(m);
			m_doc->m_Selection->connectItemToGUI();
			return true;
		}
		else
		{
			// If we call startGesture now, a new guide is created each time.
			// ### could be a weakness to avoid calling it tho.
// 			m_view->startGesture(guideMoveGesture);
			m_guideMoveGesture->mouseSelectGuide(m);
		}
	}
	m_doc->m_Selection->connectItemToGUI();
	if ( !(m->modifiers() & SELECT_MULTIPLE))
	{
		if (m_doc->m_Selection->isEmpty())
		{
			m_canvas->setForcedRedraw(true);
			m_canvas->update();
		}
		else
			m_view->Deselect(true);
	}
	if (previousSelectedItem != NULL)
		handleFocusOut(previousSelectedItem);
	return false;
}

void CanvasMode_Normal::importToPage()
{
	QString fileName;
	QString allFormats = tr("All Supported Formats")+" (";
	QStringList formats;
	int fmtCode = FORMATID_FIRSTUSER;
	const FileFormat *fmt = LoadSavePlugin::getFormatById(fmtCode);
	while (fmt != 0)
	{
		if (fmt->load)
		{
			formats.append(fmt->filter);
			int an = fmt->filter.indexOf("(");
			int en = fmt->filter.indexOf(")");
			while (an != -1)
			{
				allFormats += fmt->filter.mid(an+1, en-an-1)+" ";
				an = fmt->filter.indexOf("(", en);
				en = fmt->filter.indexOf(")", an);
			}
		}
		fmtCode++;
		fmt = LoadSavePlugin::getFormatById(fmtCode);
	}
	allFormats += "*.sce *.SCE);;";
	formats.append("Scribus Objects (*.sce *.SCE)");
	qSort(formats);
	allFormats += formats.join(";;");
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("pastefile", ".");
	FPoint pastePoint = m_mouseCurrentPoint;
	CustomFDialog dia(m_view, wdir, tr("Open"), allFormats, fdExistingFiles | fdShowImportOptions);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		PrefsManager::instance()->prefsFile->getContext("dirs")->set("pastefile", fileName.left(fileName.lastIndexOf("/")));
		m_doc->setLoading(true);
		QFileInfo fi(fileName);
		bool savedAlignGrid = m_doc->SnapGrid;
		bool savedAlignGuides = m_doc->SnapGuides;
		bool savedAlignElement = m_doc->SnapElement;
		m_doc->SnapElement = false;
		m_doc->SnapGrid = false;
		m_doc->SnapGuides = false;
		if (fi.suffix().toLower() == "sce")
			m_ScMW->slotElemRead(fileName, pastePoint.x(), pastePoint.y(), true, false, m_doc, m_doc->view());
		else
		{
			m_doc->dontResize = true;
			FileLoader *fileLoader = new FileLoader(fileName);
			int testResult = fileLoader->testFile();
			delete fileLoader;
			if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
			{
				const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
				if (fmt)
				{
					fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
				}
			}
			m_doc->dontResize = false;
		}
		for (int a = 0; a < m_doc->m_Selection->count(); ++a)
		{
			PageItem *currItem = m_doc->m_Selection->itemAt(a);
			currItem->LayerID = m_doc->activeLayer();
		}
		if (m_doc->m_Selection->count() > 0)
		{
			PageItem *newItem = m_doc->m_Selection->itemAt(0);
			if (dia.TxCodeM->currentIndex() == 1)
			{
				if ((newItem->width() > m_doc->currentPage()->width()) || (newItem->height() > m_doc->currentPage()->height()))
					m_doc->rescaleGroup(newItem, qMin(qMin(m_doc->currentPage()->width() / newItem->width(), m_doc->currentPage()->height() / newItem->height()), 1.0));
			}
			else if (dia.TxCodeM->currentIndex() == 2)
			{
				m_doc->rescaleGroup(newItem, qMax(qMin(m_doc->currentPage()->width() / newItem->width(), m_doc->currentPage()->height() / newItem->height()), 1.0));
			}
			double x2, y2, w, h;
			m_doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
			m_doc->moveGroup(pastePoint.x() - x2, pastePoint.y() - y2);
			m_ScMW->requestUpdate(reqColorsUpdate | reqSymbolsUpdate | reqLineStylesUpdate | reqTextStylesUpdate);
		}
		m_doc->SnapGrid = savedAlignGrid;
		m_doc->SnapGuides = savedAlignGuides;
		m_doc->SnapElement = savedAlignElement;
		m_doc->setLoading(false);
		m_doc->view()->DrawNew();
		if (m_doc->m_Selection->count() > 0)
		{
			m_doc->m_Selection->connectItemToGUI();
			m_ScMW->HaveNewSel();
		}
	}
}

void CanvasMode_Normal::createContextMenu(PageItem* currItem, double mx, double my)
{
	ContextMenu* cmen=NULL;
//	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	m_view->setObjectUndoMode();
	m_mouseCurrentPoint.setXY(mx, my);
	if (currItem!=NULL)
		cmen = new ContextMenu(*(m_doc->m_Selection), m_ScMW, m_doc);
	else
		cmen = new ContextMenu(m_ScMW, m_doc, mx, my);
	if (cmen)
		cmen->exec(QCursor::pos());
	m_view->setGlobalUndoMode();
	delete cmen;
}
