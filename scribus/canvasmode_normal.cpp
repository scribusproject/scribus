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
#include <QWidgetAction>
#include <QDebug>

#include "aligndistribute.h"
#include "canvas.h"
#include "canvasgesture_linemove.h"
#include "canvasgesture_resize.h"
#include "canvasgesture_rulermove.h"
#include "contextmenu.h"
#include "customfdialog.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hyphenator.h"
#include "insertTable.h"
#include "pageitem_textframe.h"
#include "pageselector.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "scmimedata.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribusXml.h"
#include "selection.h"
#include "stencilreader.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"
#include "loadsaveplugin.h"
#include "fileloader.h"
#include "plugins/formatidlist.h"



CanvasMode_Normal::CanvasMode_Normal(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	GxM = GyM = -1;
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	MoveGX = MoveGY = false;
	oldCp = Cp = -1;
	frameResizeHandle = -1;
	shiftSelItems = false;
	FirstPoly = true;
	resizeGesture = NULL;
	lineMoveGesture = NULL;
	guideMoveGesture = NULL;
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
	{
		drawOutline(p);
	}
	else
	{
		drawSelection(p);
	}
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
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
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
	GxM = GyM = -1;
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	MoveGX = MoveGY = false;
	oldCp = Cp = -1;
	frameResizeHandle = -1;
	shiftSelItems = false;
	FirstPoly = true;
	setModeCursor();
	if (fromGesture)
	{
		m_canvas->m_viewMode.operItemResizeInEditMode = false;
		m_view->update();
	}
}

void CanvasMode_Normal::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_Normal::deactivate" << forGesture;
	m_view->redrawMarker->hide();
}

void CanvasMode_Normal::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_view->stopDragTimer();
	PageItem *currItem = 0;
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
				//CB FIXME dont call this if the added item is item 0
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
			if ((currItem->locked()) || (!currItem->ScaleType))
			{
				return;
			}
			if (currItem->imageShown())
				m_view->requestMode(modeEdit);
		} 
		else if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::PolyLine) || (currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::PathText))
		{
			if ((currItem->locked()) || (!currItem->ScaleType))
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
				else if (!currItem->PicAvail)
					m_view->requestMode(submodeStatusPic);
				else if (currItem->imageShown())
					m_view->requestMode(modeEdit);
 			}
 			else if (currItem->itemType() == PageItem::TextFrame)
 				m_view->requestMode(modeEdit);
			else
				m_view->requestMode(modeEditClip);				
		}
		else if (currItem->itemType() == PageItem::TextFrame)
		{
			//CB old code
			//emit currItem->isAnnotation() ? AnnotProps() : Amode(modeEdit);
			//mousePressEvent(m);
			//CB if annotation, open the annotation dialog
			if (currItem->isAnnotation())
			{
				m_view->requestMode(submodeAnnotProps);
				//mousePressEvent(m);
			}
			//else if not in mode edit, set mode edit
			else if (m_doc->appMode != modeEdit)
			{
				m_view->requestMode(modeEdit);
				m_view->slotSetCurs(m->x(), m->y());
				//CB ignore the double click and go with a single one
				//if we werent in mode edit before.
				//unsure if this is correct, but its ok given we had no
				//double click select until now.
//				mousePressEvent(m);
			}
		}
	}
}


void CanvasMode_Normal::mouseMoveEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	m_lastPosWasOverGuide = false;
	double newX, newY;
	double nx, ny;
	PageItem *currItem;
	QPoint np, np2, mop;
	FPoint npf, npf2;
	QPainter p;
	QRect tx;
	bool erf = false;
	m->accept();
//	qDebug() << "legacy mode move:" << m->x() << m->y() << m_canvas->globalToCanvas(m->globalPos()).x() << m_canvas->globalToCanvas(m->globalPos()).y();
//	emit MousePos(m->x()/m_canvas->scale(),// + m_doc->minCanvasCoordinate.x(), 
//				  m->y()/m_canvas->scale()); // + m_doc->minCanvasCoordinate.y());

	if (m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
	{
		m_ScMW->setAppMode(modePanning);
	}
	if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modePanning))
	{
		double sc = m_canvas->scale();
		int scroX = qRound((mousePointDoc.x() - Mxp) * sc);
		int scroY = qRound((mousePointDoc.y() - Myp) * sc);
		m_view->scrollBy(-scroX, -scroY);
//		Mxp = static_cast<int>((m->x()-scroX)/sc);
//		Myp = static_cast<int>((m->y()-scroY)/sc);
		Mxp = mousePointDoc.x();
		Myp = mousePointDoc.y();
		return;
	}
	if ((m_doc->guidesSettings.guidesShown) && (!m_doc->GuideLock) && (m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y()) != -1) )
	{
		if( ((m_doc->guidesSettings.before) && (m_canvas->itemUnderCursor(m->globalPos()))) == false )
		{
			if (!guideMoveGesture)
			{
				guideMoveGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
				connect(guideMoveGesture,SIGNAL(guideInfo(int, double)),
					m_ScMW->alignDistributePalette,SLOT(setGuide(int, double)));
			}
			if (guideMoveGesture->mouseHitsGuide(mousePointDoc))
			{
				m_lastPosWasOverGuide = true;
				switch (guideMoveGesture->getMode())
				{
					case RulerGesture::HORIZONTAL:
						qApp->changeOverrideCursor(QCursor(Qt::SplitVCursor));
						break;
					case RulerGesture::VERTICAL:
						qApp->changeOverrideCursor(QCursor(Qt::SplitHCursor));
						break;
					default:
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				}
				return;
			}
			// Here removed a bunch of comments which made reading code difficult,
			// there is svn for tracking changes after all. pm
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		}
	}
	if ((GetItem(&currItem)) && (!shiftSelItems))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		if ((((m_view->dragTimerElapsed()) && (m->buttons() & Qt::LeftButton)) || (m_view->moveTimerElapsed() && (m->buttons() & Qt::RightButton)))
			&& (m_canvas->m_viewMode.m_MouseButtonPressed)
			&& (!m_doc->DragP)  
			&& (!(currItem->isSingleSel)))
		{
			// start drag
			m_view->stopDragTimer();
			if ((fabs(Dxp - newX) > 10) || (fabs(Dyp - newY) > 10))
			{
				m_canvas->setRenderMode(Canvas::RENDER_NORMAL);
				m_view->resetDragTimer();
				m_doc->DragP = true;
				m_doc->leaveDrag = false;
				m_doc->DraggedElem = currItem;
				m_doc->DragElements.clear();
				for (int dre=0; dre<m_doc->m_Selection->count(); ++dre)
					m_doc->DragElements.append(m_doc->m_Selection->itemAt(dre)->ItemNr);
				ScriXmlDoc *ss = new ScriXmlDoc();
				//Q_3DragObject *dr = new Q_3TextDrag(ss->WriteElem(Doc, this, m_doc->m_Selection), this);
				ScElemMimeData* md = new ScElemMimeData();
				md->setScribusElem(ss->WriteElem(m_doc, m_view, m_doc->m_Selection));
				QDrag* dr = new QDrag(m_view);
				dr->setMimeData(md);
				const QPixmap& pm = loadIcon("DragPix.xpm");
				dr->setDragCursor(pm, Qt::CopyAction);
				dr->setDragCursor(pm, Qt::MoveAction);
				dr->setDragCursor(pm, Qt::LinkAction);
				dr->exec();
//				QImage drImg = currItem->DrawObj_toImage();
//				QPixmap pm;
//				pm.convertFromImage(drImg);
//				dr->setPixmap(pm);
//				dr->setPixmap(loadIcon("DragPix.xpm"));
//				dr->drag();
//				if (!dr->drag())
//					qDebug("ScribusView::contentsMouseMoveEvent: couldn't start drag operation!");
/* commented out the code above as the debug message is incorrect,
   see the Qt Reference: "The function returns TRUE if the caller should delete the original copy
    of the dragged data (but see target()); otherwise returns FALSE." */
				delete ss;
				ss=NULL;
				m_doc->DragP = false;
				m_doc->leaveDrag = false;
				m_canvas->m_viewMode.m_MouseButtonPressed = false;
				m_doc->DraggedElem = 0;
				m_doc->DragElements.clear();
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
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
			m_view->stopDragTimer();
			if (m_canvas->m_viewMode.operItemResizing)
			{
//				newX = static_cast<int>(m->x()/sc);
//				newY = static_cast<int>(m->y()/sc);
				m_canvas->m_viewMode.operItemMoving = false;
				if (m_doc->m_Selection->isMultipleSelection())
				{
					qDebug()<<"Resizing multiple selection"<<frameResizeHandle;
//					newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
//					newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
					double gx, gy, gh, gw;
					m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
					const double sc=1;
					int ox1 = qRound(gx*sc);
					int oy1 = qRound(gy*sc);
					int ox2 = qRound((gx+gw)*sc);
					int oy2 = qRound((gy+gh)*sc);
					FPoint np2 = FPoint(newX, newY);
					np2 = m_doc->ApplyGridF(mousePointDoc);
					double nx = np2.x();
					double ny = np2.y();
					m_doc->ApplyGuides(&nx, &ny);
					np2 = FPoint(qRound(nx*sc), qRound(ny*sc));
					QMatrix pm;
					switch (frameResizeHandle)
					{
					case 1:
						if (m->modifiers() & Qt::ControlModifier)
							np2 = FPoint(qRound(newX), qRound(gy+(gh * ((newX-gx) / gw))));
						else
							np2 = FPoint(qRound(newX), qRound(newY));
						np2 = m_doc->ApplyGridF(np2);
						nx = np2.x();
						ny = np2.y();
						m_doc->ApplyGuides(&nx, &ny);
						np2 = FPoint(qRound(nx*sc), qRound(ny*sc));
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(qRound(gx*sc), qRound(gy*sc)), QPoint(qRound(np2.x()), qRound(np2.y())))));
						break;
					case 2:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(qRound(np2.x()), qRound(np2.y())), QPoint(ox2,oy2))));
						break;
					case 3:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(qRound(np2.x()), qRound(np2.y())), QPoint(ox1, oy2))));
						break;
					case 4:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(qRound(np2.x()), qRound(np2.y())), QPoint(ox2, oy1))));
						break;
					case 5:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(ox1, oy1), QPoint(ox2, qRound(np2.y())))));
						break;
					case 6:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(qRound(np2.x()), oy2), QPoint(ox1,oy1))));
						break;
					case 7:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(qRound(np2.x()), oy1), QPoint(ox2, oy2))));
						break;
					case 8:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(ox1, qRound(qRound(np2.y()))), QPoint(ox2, oy2))));
						break;
					}
					Mxp = qRound(np2.x());
					Myp = qRound(np2.y());
				}
				else
				{
					qDebug()<<"frameResizeHandle"<<frameResizeHandle;
					for (int a = 0; a < m_doc->m_Selection->count(); ++a)
					{
						currItem = m_doc->m_Selection->itemAt(0);
						double nh = currItem->height();
						if ((frameResizeHandle == 1) || (frameResizeHandle == 2))
						{
							QMatrix mp;
							switch (frameResizeHandle)
							{
							case 1:
//								mp.translate(-m_doc->minCanvasCoordinate.x() * m_canvas->scale(),-m_doc->minCanvasCoordinate.y() * m_canvas->scale());
								m_canvas->Transform(currItem, mp);
								//Shift proportional square resize
								if ((m->modifiers() & Qt::ShiftModifier) && (!(m->modifiers() & Qt::ControlModifier)))
								{
									QMatrix ma;
									ma.translate(currItem->xPos(), currItem->yPos());
									ma.rotate(currItem->rotation());
									ma = ma.inverted();
									nh = ma.m11() * newX + ma.m21() * newY + ma.dx();
									mop = QPoint(m->x(), m->y());
								}
								else
								{
									//Control proportional resize
									if ((m->modifiers() & Qt::ControlModifier) && (!(m->modifiers() & Qt::ShiftModifier)))
									{
										QMatrix ma;
										ma.translate(currItem->xPos(), currItem->yPos());
										ma.rotate(currItem->rotation());
										ma = ma.inverted();
										double nX = ma.m11() * newX + ma.m21() * newY + ma.dx();
										nh = nX / currItem->OldB2 * currItem->OldH2;
										mop = QPoint(m->x(), m->y());
									}
									else
										mop = QPoint(m->x(), m->y());
								}
								np = mop * mp.inverted();
								nx = np.x();
								ny = np.y();
								if (!currItem->asLine())
								{
									if ((m_doc->useRaster) && (m_doc->OnPage(currItem) != -1))
									{
										nx += currItem->xPos();
										ny += currItem->yPos();
										npf = m_doc->ApplyGridF(FPoint(nx, ny));
										nx = npf.x() - currItem->xPos();
										ny = npf.y() - currItem->yPos();
									}
									if (m_doc->SnapGuides)
									{
										nx += currItem->xPos();
										ny += currItem->yPos();
										m_doc->ApplyGuides(&nx, &ny);
										nx -= currItem->xPos();
										ny -= currItem->yPos();
									}
									if ((m->modifiers() & Qt::ControlModifier) || ((m->modifiers() & Qt::ShiftModifier)))
										erf = m_doc->SizeItem(nx, nh, currItem->ItemNr);
									else
										erf = m_doc->SizeItem(nx, ny, currItem->ItemNr);
									QMatrix mp;
									mp.translate(currItem->xPos(),// - m_doc->minCanvasCoordinate.x(), 
												 currItem->yPos());// - m_doc->minCanvasCoordinate.y());
									mp.rotate(currItem->rotation());
									np2 = QPoint(qRound(nx), qRound(ny));
									QMatrix pm;
									m_canvas->Transform(currItem, pm);
									m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), np2)));
								}
								else
								{
									double rba = currItem->rotation();
									currItem->setRotation(0.0);
									QMatrix mp;
									m_canvas->Transform(currItem, mp);
//									mp.translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
									np = QPoint(m->x(), m->y()) * mp.inverted();
									double sizeItemX=np.x(), sizeItemY=np.y();
									//Constrain rotation angle, when the mouse is moving the non-origin point of a line
									if (m->modifiers() & Qt::ControlModifier)
									{
										double newRot=xy2Deg(np.x(), np.y());
										rba=constrainAngle(newRot, m_doc->toolSettings.constrain);
										double hlen=sqrt(pow(newX - currItem->xPos(),2)+pow(newY - currItem->yPos(),2));
										sizeItemX = hlen * cos(rba/(180.0/M_PI));
										sizeItemY = hlen * sin(rba/(180.0/M_PI));
									}
									currItem->setRotation(rba);
									np = m_doc->ApplyGrid(np);
									erf = m_doc->SizeItem(sizeItemX, sizeItemY, currItem->ItemNr);
									QMatrix pm;
//									pm.translate(-m_doc->minCanvasCoordinate.x() * m_canvas->scale(),-m_doc->minCanvasCoordinate.y() * m_canvas->scale());
									m_canvas->Transform(currItem, pm);
									m_canvas->newRedrawPolygon() << pm.map(QPoint(0, 0)) << pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())));
									m_view->updateContents(QRect(pm.map(QPoint(0, 0)), pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())))).normalized().adjusted(-10, -10, 20, 20));
								}
								break;
							case 2:
								if (currItem->asLine())
								{
									double sav = m_doc->SnapGuides;
									npf2 = FPoint(newX-Mxp, newY-Myp);
									//Constrain rotation on left point move, disabled for now in movesizeitem
									erf = m_doc->MoveSizeItem(npf2, FPoint(0, 0), currItem->ItemNr, false, (m->modifiers() & Qt::ControlModifier));
									m_doc->SnapGuides = sav;
									if (sav)
										currItem->Sizing = true;
									QMatrix pm;
//									pm.translate(-m_doc->minCanvasCoordinate.x() * m_canvas->scale(),-m_doc->minCanvasCoordinate.y() * m_canvas->scale());
									m_canvas->Transform(currItem, pm);
									m_canvas->newRedrawPolygon() << pm.map(QPoint(0, 0)) << pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())));
									m_view->updateContents(QRect(pm.map(QPoint(0, 0)), pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())))).normalized().adjusted(-10, -10, 20, 20));
								}
								else
								{
									newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
									newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
//									np2 = QPoint(newX, newY);
									FPoint newP = m_doc->ApplyGridF(FPoint(newX, newY));
									double nx = newP.x();
									double ny = newP.y();
									m_doc->ApplyGuides(&nx, &ny);
									QMatrix mp;
//									mp.translate(currItem->xPos() - m_doc->minCanvasCoordinate.x(), currItem->yPos() - m_doc->minCanvasCoordinate.y());
									mp.rotate(currItem->rotation());
									np2 = QPoint(qRound(nx), qRound(ny)) * mp.inverted();
									QMatrix pm;
									m_canvas->Transform(currItem, pm);
									m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(qRound(currItem->width()), qRound(currItem->height())))));
//									p.end();
								}
								break;
							}
						}
						else
						{
//							np2 = QPoint(newX, newY);
							FPoint newP = m_doc->ApplyGridF(FPoint(newX, newY));
							double nx = newP.x();
							double ny = newP.y();
							m_doc->ApplyGuides(&nx, &ny);
							QMatrix mp;
//							mp.translate(currItem->xPos() - m_doc->minCanvasCoordinate.x(), currItem->yPos() - m_doc->minCanvasCoordinate.y());
							mp.rotate(currItem->rotation());
							np2 = QPoint(qRound(nx), qRound(ny)) * mp.inverted();
							QMatrix pm;
							m_canvas->Transform(currItem, pm);
							switch (frameResizeHandle)
							{
							case 0:
								break;
							case 3:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(0, qRound(currItem->height())))));
								break;
							case 4:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(qRound(currItem->width()), 0))));
								break;
							case 5:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), QPoint(qRound(currItem->width()), np2.y()))));
								break;
							case 6:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), QPoint(np2.x(), qRound(currItem->height())))));
								break;
							case 7:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(np2.x(), 0), QPoint(qRound(currItem->width()), qRound(currItem->height())))));
								break;
							case 8:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, np2.y()), QPoint(qRound(currItem->width()), qRound(currItem->height())))));
								break;
							}
						}
					}
				}
				if ((erf) || (frameResizeHandle > 1))
				{
					Mxp = newX;
					Myp = newY;
				}
			}
			else
			{
				//Dragging an item (plus more?)
				QRectF newPlace;
				newX = mousePointDoc.x(); //static_cast<int>(m->x()/sc);
				newY = mousePointDoc.y(); //static_cast<int>(m->y()/sc);
				m_canvas->m_viewMode.operItemMoving = true;
				erf = false;
				int dX=qRound(newX-Mxp), dY=qRound(newY-Myp);
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
							dX+=qRound(dragConstrainInitPtX-currItem->xPos());
							dY+=qRound(dragConstrainInitPtY-currItem->yPos());
						}
						if (!(currItem->isTableItem && currItem->isSingleSel))
						{
							if (!m_view->groupTransactionStarted())
							{
								m_view->startGroupTransaction(Um::Move, "", Um::IMove);
							}
							double gx, gy, gh, gw;
							m_doc->moveGroup(dX, dY, false);
							m_doc->m_Selection->setGroupRect();
							m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
							if (m_doc->SnapGuides)
							{
								double nx = gx;
								double ny = gy;
								m_doc->ApplyGuides(&nx, &ny);
								m_doc->moveGroup(nx-gx, ny-gy, false);
								m_doc->m_Selection->setGroupRect();
								m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
								nx = gx+gw;
								ny = gy+gh;
								m_doc->ApplyGuides(&nx, &ny);
								m_doc->moveGroup(nx-(gx+gw), ny-(gy+gh), false);
							}
							if (m_doc->useRaster)
							{
								m_doc->m_Selection->setGroupRect();
								double gx, gy, gh, gw, gxo, gyo;
								m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
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
								if ((fabs(gx - gxo) < (m_doc->guidesSettings.guideRad / 2.0) / m_canvas->scale()) && (fabs(gy - gyo) < (m_doc->guidesSettings.guideRad / 2.0) / m_canvas->scale()))
									m_doc->moveGroup(gx-gxo, gy-gyo, false);
							}
						}
					}
				}
				else
				{
					if (!m_view->groupTransactionStarted())
					{
						m_view->startGroupTransaction(Um::Move, "", Um::IMove);
					}
					double gx, gy, gh, gw;
					m_doc->m_Selection->setGroupRect();
					m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					int dX=qRound(newX-Mxp), dY=qRound(newY-Myp);
					erf = true;
					if (m->modifiers() & Qt::ControlModifier)
					{
						if (abs(dX)>abs(dY))
							dY=0;
						else
						if (abs(dY)>abs(dX))
							dX=0;
						erf=false;
						dX+=dragConstrainInitPtX-qRound(gx);
						dY+=dragConstrainInitPtY-qRound(gy);
					}
					m_doc->moveGroup(dX, dY, false);
					m_doc->m_Selection->setGroupRect();
					m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					if (m_doc->SnapGuides)
					{
						double nx = gx;
						double ny = gy;
						m_doc->ApplyGuides(&nx, &ny);
						m_doc->moveGroup(nx-gx, ny-gy, false);
						m_doc->m_Selection->setGroupRect();
						m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
						nx = gx+gw;
						ny = gy+gh;
						m_doc->ApplyGuides(&nx, &ny);
						m_doc->moveGroup(nx-(gx+gw), ny-(gy+gh), false);
					}
					m_doc->m_Selection->setGroupRect();
					if (m_doc->useRaster)
					{
						m_doc->m_Selection->setGroupRect();
						double gx, gy, gh, gw, gxo, gyo;
						m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
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
						if ((fabs(gx - gxo) < (m_doc->guidesSettings.guideRad / 2.0) / m_canvas->scale()) && (fabs(gy - gyo) < (m_doc->guidesSettings.guideRad / 2.0) / m_canvas->scale()))
							m_doc->moveGroup(gx-gxo, gy-gyo, false);
						m_doc->m_Selection->setGroupRect();
					}
				}
				if (erf)
				{
					Mxp = newX;
					Myp = newY;
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
					m_canvas->displayXYHUD(m->globalPos());
				}
			}
		}
		if ((!m_canvas->m_viewMode.m_MouseButtonPressed) && (m_doc->appMode != modeDrawBezierLine))
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
					else
					{
						qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
					}
					if (m_doc->appMode == modeRotation)
					{
						qApp->changeOverrideCursor(QCursor(loadIcon("Rotieren2.png")));
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
				QMatrix p;
				m_canvas->Transform(currItem, p);
				QRect mpo = QRect(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//				mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
				if ((QRegion(p.map(QPolygon(QRect(-3, -3, static_cast<int>(currItem->width()+6), static_cast<int>(currItem->height()+6))))).contains(mpo)))
				{
					tx = p.mapRect(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
					if ((tx.intersects(mpo)) && (!currItem->locked()))
					{
						qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
						if (!currItem->sizeLocked())
							m_view->HandleCurs(currItem, mpo);
					}
				}
				else
				{
//					setModeCursor();
				}
			}
			if (GetItem(&currItem) && m_doc->appMode == modeNormal)
			{
				int how = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
				if (how > 0)
					setResizeCursor(how, currItem->rotation());
				else if (how == 0)
					qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
				else
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			}
		}
	}
	else
	{
		if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton) && (GyM == -1) && (GxM == -1))
		{
			newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
			if (m_doc->appMode == modeMagnifier)
				newY = qRound(Myp + ((SeRx - Mxp) * m_view->visibleHeight()) / m_view->visibleWidth());
			else
				newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
			if (m_doc->appMode == modeDrawTable)
			{
				FPoint np2 = m_doc->ApplyGridF(FPoint(newX, newY));
				double nx = np2.x();
				double ny = np2.y();
				m_doc->ApplyGuides(&nx, &ny);
				newX = qRound(nx);
				newY = qRound(ny);
				GyM = -1;
				GxM = -1;
			}
			SeRx = newX;
			SeRy = newY;
			/*
			if (m_doc->appMode == modeDrawTable)
				m_view->redrawMarker->setGeometry(QRect(Dxp, Dyp, m->globalPos().x() - Dxp, m->globalPos().y() - Dyp).normalized());
			else
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
		
	
}

void CanvasMode_Normal::mousePressEvent(QMouseEvent *m)
{
// 	qDebug("CanvasMode_Normal::mousePressEvent");
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());

	double Rxp = 0;
	double Ryp = 0;
	double Rxpd = 0;
	double Rypd = 0;
	PageItem *currItem;
	QPainter p;
// 	m_canvas->PaintSizeRect(QRect());
	FPoint npf, npf2;
	QRect tx;
	QMatrix pm;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	MoveGX = MoveGY = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
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
		m_view->DrawNew();
		return;
	}

	Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale());
	Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale());
	SeRx = Mxp;
	SeRy = Myp;
	Dxp = Mxp;
	Dyp = Myp;
	if ((GetItem(&currItem)) && (!m_lastPosWasOverGuide))
	{
		if ((currItem->asLine()) && (!m_doc->m_Selection->isMultipleSelection()))
		{
			if (!lineMoveGesture)
				lineMoveGesture = new LineMove(this);
			
			lineMoveGesture->mousePressEvent(m);
			if (lineMoveGesture->haveLineItem())
			{
				m_view->startGesture(lineMoveGesture);
				return;
			}
		}
		else
		{
			if (!resizeGesture)
				resizeGesture = new ResizeGesture(this);
			
			resizeGesture->mousePressEvent(m);
			if (resizeGesture->frameHandle() > 0)
			{
				m_view->startGesture(resizeGesture);
				return;
			}
		}
#if 1				
		if (m_doc->m_Selection->isMultipleSelection())
		{
			m_canvas->PaintSizeRect(QRect());
			double gx, gy, gh, gw;
			bool shiftSel = true;
			m_doc->m_Selection->getVisualGroupRect(&gx, &gy, &gw, &gh);
			dragConstrainInitPtX = qRound(gx);
			dragConstrainInitPtY = qRound(gy);
			frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), QRectF(gx, gy, gw, gh));
			if (frameResizeHandle > 0)
			{
				if (currItem->sizeLocked())
				{
					qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
					frameResizeHandle = 0;
				}
				m_canvas->m_viewMode.operItemResizing = true;
			}
			else if (frameResizeHandle == Canvas::OUTSIDE ||
					 (frameResizeHandle == Canvas::INSIDE && m->modifiers() != Qt::NoModifier))
			{
				frameResizeHandle = 0;
				m_doc->m_Selection->delaySignalsOn();
				m_view->updatesOn(false);
				shiftSel = SeleItem(m);
				m_view->updatesOn(true);
				m_doc->m_Selection->delaySignalsOff();
			}
			if (((m_doc->m_Selection->count() == 0) || (!shiftSel)) && (m->modifiers() == Qt::ShiftModifier))
			{
				shiftSelItems = true;
				Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
				Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
				SeRx = Mxp;
				SeRy = Myp;
				Dxp = Mxp;
				Dyp = Myp;
			}
			else
				shiftSelItems = false;
			m_canvas->setRenderModeFillBuffer();
		}
		else // not multiple selection
		{
			dragConstrainInitPtX = qRound(currItem->xPos());
			dragConstrainInitPtY = qRound(currItem->yPos());
			
			// dont call SeleItem() without need here:
			frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
			//#6797
			if (frameResizeHandle <= 0 || m->modifiers() != Qt::NoModifier)
			{
				m_doc->m_Selection->delaySignalsOn();
				m_view->updatesOn(false);
				SeleItem(m); //Where we send the mouse press event to select an item
				if (GetItem(&currItem))
					frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
				else
					frameResizeHandle = 0;
				m_view->updatesOn(true);
				m_doc->m_Selection->delaySignalsOff();
			}
			if (currItem && !currItem->locked() && frameResizeHandle > 0)
			{
				if (!currItem->asLine())
					currItem->Sizing = true;
				m_canvas->m_viewMode.operItemResizing = true;
			}
			/*
			if (m_doc->m_Selection->count() != 0)
			{
				currItem = m_doc->m_Selection->itemAt(0);
				if (!currItem->locked())
				{
					frameResizeHandle = m_canvas->frameHitTest(mousePointDoc, currItem); // HandleSizer(currItem, mpo, m);
					if (frameResizeHandle != 0)
					{
						if (!currItem->asLine())
							currItem->Sizing = true;
						m_canvas->m_viewMode.operItemResizing = true;
					}
				}
			}*/
			else
			{
				Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
				Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
				SeRx = Mxp;
				SeRy = Myp;
				Dxp = Mxp;
				Dyp = Myp;
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
			Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
			Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
			SeRx = Mxp;
			SeRy = Myp;
			m_view->redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
			m_view->redrawMarker->show();
//					Mxp = m->globalPos().x();
//					Myp = m->globalPos().y();
			Dxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
			Dyp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
		}
		else
		{
			m_canvas->setRenderModeFillBuffer();
		}
	}
/*			if (m->button() == MidButton)
	{
		MidButt = true;
		if (m_doc->m_Selection->count() != 0)
			m_view->Deselect(true);
		DrawNew();
	} */
	if ((m_doc->m_Selection->count() != 0) && (m->button() == Qt::RightButton))
	{
		m_canvas->m_viewMode.m_MouseButtonPressed = true;
		Dxp = Mxp;
		Dyp = Myp;
	}
	if ((m_doc->m_Selection->count() != 0) && (m->button() == Qt::LeftButton) && (frameResizeHandle == 0))
	{
		m_view->startDragTimer();
	}
	m_canvas->PaintSizeRect(QRect());
}



void CanvasMode_Normal::mouseReleaseEvent(QMouseEvent *m)
{
// 	qDebug("CanvasMode_Normal::mouseReleaseEvent");
	clearPixmapCache();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->stopDragTimer();
	//m_canvas->update(); //ugly in a mouseReleaseEvent!!!!!!!
	if (m_doc->appMode == modePanning)
	{
		if ((m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
			m_ScMW->setAppMode(modeNormal);
		return;
	}
	if ((!GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!m_doc->DragP))
	{
		createContextMenu(NULL, mousePointDoc.x(), mousePointDoc.y());
		return;
	}
	if ((GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!m_doc->DragP))
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
			m_canvas->m_viewMode.operItemResizing = false;
			if (currItem->asLine())
				m_view->updateContents();
		}
		if (m_canvas->m_viewMode.operItemMoving)
		{
			// we want to invalidate all frames under the moved frame
			// hm, I will try to be more explicit :) - pm
			PageItem* underItem( m_canvas->itemUnderItem(currItem) );
			while(underItem)
			{
				if(underItem->asTextFrame())
					underItem->asTextFrame()->invalidateLayout();
				else
					underItem->invalidateLayout();
				
				underItem =  m_canvas->itemUnderItem(underItem);
			}
			
			m_view->updatesOn(false);
			if (m_doc->m_Selection->isMultipleSelection())
			{
				if (!m_view->groupTransactionStarted())
				{
					m_view->startGroupTransaction(Um::Move, "", Um::IMove);
				}
				m_doc->m_Selection->setGroupRect();
				double gx, gy, gh, gw;
				m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				double nx = gx;
				double ny = gy;
				if (!m_doc->ApplyGuides(&nx, &ny))
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
				m_doc->moveGroup(nx-gx, ny-gy, false);
				m_doc->m_Selection->setGroupRect();
				m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				nx = gx+gw;
				ny = gy+gh;
				if (m_doc->ApplyGuides(&nx, &ny))
					m_doc->moveGroup(nx-(gx+gw), ny-(gy+gh), false);
				m_doc->m_Selection->setGroupRect();
			}
			else
			{
				currItem = m_doc->m_Selection->itemAt(0);
				if (m_doc->useRaster)
				{
					double nx = currItem->xPos();
					double ny = currItem->yPos();
					if (!m_doc->ApplyGuides(&nx, &ny))
					{
						m_doc->m_Selection->setGroupRect();
						double gx, gy, gh, gw;
						m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
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
					m_doc->MoveItem(nx-currItem->xPos(), ny-currItem->yPos(), currItem);
				}
				else
					m_doc->MoveItem(0, 0, currItem, false);
			}
			m_canvas->m_viewMode.operItemMoving = false;
			if (m_doc->m_Selection->isMultipleSelection())
			{
				double gx, gy, gh, gw;
				m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				FPoint maxSize(gx+gw+m_doc->scratch.Right, gy+gh+m_doc->scratch.Bottom);
				FPoint minSize(gx-m_doc->scratch.Left, gy-m_doc->scratch.Top);
				m_doc->adjustCanvas(minSize, maxSize);
			}
			m_doc->setRedrawBounding(currItem);
			currItem->OwnPage = m_doc->OnPage(currItem);
			if (currItem->OwnPage != -1)
			{
				m_doc->setCurrentPage(m_doc->Pages->at(currItem->OwnPage));
				m_view->setMenTxt(currItem->OwnPage);
			}
			//CB done with emitAllToGUI
			//emit HaveSel(currItem->itemType());
			//EmitValues(currItem);
			//CB need this for? a moved item will send its new data with the new xpos/ypos emits
			//CB TODO And what if we have dragged to a new page. Items X&Y are not updated anyway now
			//currItem->emitAllToGUI();
			m_view->updatesOn(true);
			m_view->updateContents();
		}
	}
	//CB Drag selection performed here
	if (((m_doc->m_Selection->count() == 0) && (m_view->HaveSelRect) && (!m_view->MidButt)) || ((shiftSelItems) && (m_view->HaveSelRect) && (!m_view->MidButt)))
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
		shiftSelItems = false;
		m_view->redrawMarker->hide();
		m_view->updateContents();
	}
	if (m_doc->appMode != modeEdit)
	{
		if (!PrefsManager::instance()->appPrefs.stickyTools)
			m_view->requestMode(modeNormal);
		else
		{
			int appMode = m_doc->appMode;
			m_view->requestMode(appMode);
		}
	}
	if (GetItem(&currItem))
	{
		if (m_doc->m_Selection->count() > 1)
		{
			m_doc->m_Selection->setGroupRect();
			double x, y, w, h;
			m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			m_canvas->m_viewMode.operItemMoving = false;
			m_canvas->m_viewMode.operItemResizing = false;
			m_view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
		}
		else
			currItem->emitAllToGUI();
	}
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	shiftSelItems = false;
	if (m_view->groupTransactionStarted())
	{
		for (int i = 0; i < m_doc->m_Selection->count(); ++i)
			m_doc->m_Selection->itemAt(i)->checkChanges(true);
		m_view->endGroupTransaction();
	}
	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
		m_doc->m_Selection->itemAt(i)->checkChanges(true);
	//Commit drag created items to undo manager.
	if (m_doc->m_Selection->itemAt(0)!=NULL)
	{
		m_doc->itemAddCommit(m_doc->m_Selection->itemAt(0)->ItemNr);
	}
	//Make sure the Zoom spinbox and page selector dont have focus if we click on the canvas
	m_view->zoomSpinBox->clearFocus();
	m_view->pageSelector->clearFocus();
	if (m_doc->m_Selection->itemAt(0) != 0) // is there the old clip stored for the undo action
	{
		currItem = m_doc->m_Selection->itemAt(0);
		m_doc->nodeEdit.finishTransaction(currItem);
	}
}


void CanvasMode_Normal::selectPage(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	Mxp = mousePointDoc.x(); //static_cast<int>(m->x()/m_canvas->scale());
	Myp = mousePointDoc.y(); //static_cast<int>(m->y()/m_canvas->scale());
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
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
				m_view->setMenTxt(j);
				m_view->DrawNew();
			}
		}
		m_view->setRulerPos(m_view->contentsX(), m_view->contentsY());
	}
}

//CB-->Doc/Fix
bool CanvasMode_Normal::SeleItem(QMouseEvent *m)
{
	const unsigned SELECT_IN_GROUP = Qt::AltModifier;
	const unsigned SELECT_MULTIPLE = Qt::ShiftModifier;
	const unsigned SELECT_BENEATH = Qt::ControlModifier;
	QMatrix p;
	QRectF tx, mpo;
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	Mxp = mousePointDoc.x(); //m->x()/m_canvas->scale());
	Myp = mousePointDoc.y(); //m->y()/m_canvas->scale());
	double grabRadius = m_doc->guidesSettings.grabRad / m_canvas->scale();
	int MxpS = static_cast<int>(mousePointDoc.x()); //m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	int MypS = static_cast<int>(mousePointDoc.y()); //m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	mpo = QRectF(Mxp-grabRadius, Myp-grabRadius, grabRadius*2, grabRadius*2);
//	mpo.translate(m_doc->minCanvasCoordinate.x() * m_canvas->scale(), m_doc->minCanvasCoordinate.y() * m_canvas->scale());
	m_doc->nodeEdit.deselect();
// 	int a;
	if(!m_doc->guidesSettings.before) // guides are on foreground and want to be processed first
	{
		if ((m_doc->guidesSettings.guidesShown) /*&& (!m_doc->GuideLock)*/ && (m_doc->OnPage(MxpS, MypS) != -1))
		{
			if (!guideMoveGesture)
			{
				guideMoveGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
				connect(guideMoveGesture,SIGNAL(guideInfo(int, double)),
					m_ScMW->alignDistributePalette,SLOT(setGuide(int, double)));
			}
			if ( (!m_doc->GuideLock) && (guideMoveGesture->mouseHitsGuide(mousePointDoc)) )
			{
				m_view->startGesture(guideMoveGesture);
				guideMoveGesture->mouseMoveEvent(m);
			//m_doc->m_Selection->setIsGUISelection(true);
				m_doc->m_Selection->connectItemToGUI();
				return true;
			}
			else
			{
			// If we call startGesture now, a new guide is created each time.
			// ### could be a weakness to avoid calling it tho.
// 			m_view->startGesture(guideMoveGesture);
				guideMoveGesture->mouseSelectGuide(m);
			}
		}
	}
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
// 				m_view->DrawNew();
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
	else if ( (m->modifiers() & SELECT_MULTIPLE) == Qt::NoModifier)
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
		m_canvas->m_viewMode.operItemSelecting = true;
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
			//					emit ItemPos(x, y);
			//					emit ItemGeom(w, h);
			m_view->getGroupRectScreen(&x, &y, &w, &h);
			//					m_view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
			//					emit HaveSel(currItem->itemType());
		}
		if (m_doc->m_Selection->count() == 1)
		{
			frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem); // HandleSizer(currItem, mpo.toRect(), m);
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
	if ((m_doc->guidesSettings.guidesShown) /*&& (!m_doc->GuideLock)*/ && (m_doc->OnPage(MxpS, MypS) != -1))
	{
		if (!guideMoveGesture)
		{
			guideMoveGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
			connect(guideMoveGesture,SIGNAL(guideInfo(int, double)),
				m_ScMW->alignDistributePalette,SLOT(setGuide(int, double)));
		}
		if ( (!m_doc->GuideLock) && (guideMoveGesture->mouseHitsGuide(mousePointDoc)) )
		{
			m_view->startGesture(guideMoveGesture);
			guideMoveGesture->mouseMoveEvent(m);
			//m_doc->m_Selection->setIsGUISelection(true);
			m_doc->m_Selection->connectItemToGUI();
			return true;
		}
		else
		{
			// If we call startGesture now, a new guide is created each time.
			// ### could be a weakness to avoid calling it tho.
// 			m_view->startGesture(guideMoveGesture);
			guideMoveGesture->mouseSelectGuide(m);
		}
/*		GxM = -1;
		GyM = -1;
		QMap<double, uint> tmpGuidesSel;
		Guides tmpGuides = m_doc->currentPage()->guides.horizontals(GuideManagerCore::Standard);
		Guides::iterator it;
		uint yg = 0;
		uint xg = 0;
		double lowX = ((m->x() - m_doc->guidesSettings.grabRad) / m_canvas->scale()) + 0*m_doc->minCanvasCoordinate.x();
		double highX = ((m->x() + m_doc->guidesSettings.grabRad) / m_canvas->scale()) + 0*m_doc->minCanvasCoordinate.x();
		double lowY = ((m->y() - m_doc->guidesSettings.grabRad) / m_canvas->scale()) + 0*m_doc->minCanvasCoordinate.y();
		double highY = ((m->y() + m_doc->guidesSettings.grabRad) / m_canvas->scale()) + 0*m_doc->minCanvasCoordinate.y();
		for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++yg)
		{
			if (((*it) + m_doc->currentPage()->yOffset() < highY) && ((*it)+m_doc->currentPage()->yOffset() > lowY))
				tmpGuidesSel.insert(fabs(((*it)+m_doc->currentPage()->yOffset()) - MypS), yg);
		}
		if (tmpGuidesSel.count() != 0)
		{
			GyM = tmpGuidesSel.begin().value();
			QPoint py = m_view->viewport()->mapFromGlobal(m->globalPos());
			m_view->DrHY = py.y();
		}
		tmpGuidesSel.clear();
		tmpGuides = m_doc->currentPage()->guides.verticals(GuideManagerCore::Standard);
		for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++xg)
		{
			if (((*it) + m_doc->currentPage()->xOffset() < highX) && ((*it)+m_doc->currentPage()->xOffset() > lowX))
				tmpGuidesSel.insert(fabs(((*it)+m_doc->currentPage()->xOffset()) - MypS), xg);
		}
		if (tmpGuidesSel.count() != 0)
		{
			GxM = tmpGuidesSel.begin().value();
			QPoint py = m_view->viewport()->mapFromGlobal(m->globalPos());
			m_view->DrVX = py.x();
		}
		if (GxM!=-1 || GyM!=-1)
		{
			if (GxM==-1)
			{
				// Horizontal Guide
				MoveGY = true;
//FIXME:av				emit signalGuideInformation(0, qRound(m_doc->currentPage()->guides.horizontal(GyM, GuideManagerCore::Standard) * 10000.0) / 10000.0);
			}
			else
			{
				// Vertical Guide
				MoveGX = true;
//FIXME:av				emit signalGuideInformation(1, qRound(m_doc->currentPage()->guides.vertical(GxM, GuideManagerCore::Standard) * 10000.0) / 10000.0);
			}
		}
		*/
	}
	//m_doc->m_Selection->setIsGUISelection(true);
	m_doc->m_Selection->connectItemToGUI();
	if ( !(m->modifiers() & SELECT_MULTIPLE))
		m_view->Deselect(true);
	m_canvas->m_viewMode.forceRedraw = true;
	m_canvas->PaintSizeRect(m_canvas->exposedRect());
	return false;
}

//CB Fix item->old* stuff
int CanvasMode_Normal::HandleSizer(PageItem *currItem, QRect mpo, QMouseEvent *m)
{
	currItem->OldB = currItem->width();
	currItem->OldH = currItem->height();
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	frameResizeHandle = 0;
	if (currItem->sizeLocked())
		return 0;
	m_canvas->PaintSizeRect(QRect());
	double d1;
	QMap<double,int> distance;
	FPoint n1(currItem->width(), currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
	//	n1 -= QPoint(qRound(m_doc->minCanvasCoordinate.x()), qRound(m_doc->minCanvasCoordinate.y()));
	d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
	if (d1 < m_doc->guidesSettings.grabRad)
		distance.insert(d1, 1);
	n1 = FPoint(0, 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
	//	n1 -= QPoint(qRound(m_doc->minCanvasCoordinate.x()), qRound(m_doc->minCanvasCoordinate.y()));
	d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
	if (d1 < m_doc->guidesSettings.grabRad)
		distance.insert(d1, 2);
	if (!currItem->asLine())
	{
		QPoint docMinCanvasCoordinate(qRound(m_doc->minCanvasCoordinate.x()), qRound(m_doc->minCanvasCoordinate.y()));
		n1 = FPoint(currItem->width(), 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < m_doc->guidesSettings.grabRad)
			distance.insert(d1, 3);
		n1 = FPoint(0, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < m_doc->guidesSettings.grabRad)
			distance.insert(d1, 4);
		n1 = FPoint(currItem->width()/2, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < m_doc->guidesSettings.grabRad)
			distance.insert(d1, 5);
		n1 = FPoint(currItem->width(), currItem->height()/2, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < m_doc->guidesSettings.grabRad)
			distance.insert(d1, 6);
		n1 = FPoint(0, currItem->height()/2, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < m_doc->guidesSettings.grabRad)
			distance.insert(d1, 7);
		n1 = FPoint(currItem->width()/2, 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < m_doc->guidesSettings.grabRad)
			distance.insert(d1, 8);
	}
	QList<int> result = distance.values();
	if (result.count() != 0)
		frameResizeHandle = result[0];
	//	mpo.moveBy(qRound(-m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	m_view->HandleCurs(currItem, mpo);
	if (frameResizeHandle != 0)
	{
		if (!currItem->asLine())
			currItem->Sizing = true;
		m_canvas->m_viewMode.operItemResizing = true;
	}
	return frameResizeHandle;
}


// void CanvasMode_Normal::setResizeCursor(int how, double rot )
// {
// 	switch (how)
// 	{
// 		case 1:
// 		case 2:
// 			qApp->changeOverrideCursor(/*QCursor(Qt::SizeFDiagCursor)*/ScResizeCursor(135 + rot));
// 			break;
// 		case 3:
// 		case 4:
// 			qApp->changeOverrideCursor(/*QCursor(Qt::SizeBDiagCursor)*/ScResizeCursor(45 + rot));
// 			break;
// 		case 5:
// 		case 8:
// 			qApp->changeOverrideCursor(/*QCursor(Qt::SizeVerCursor)*/ScResizeCursor(0 + rot));
// 			break;
// 		case 6:
// 		case 7:
// 			qApp->changeOverrideCursor(/*QCursor(Qt::SizeHorCursor)*/ScResizeCursor(90 + rot));
// 			break;
// 		default:
// 			qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
// 			break;
// 	}
// }

void CanvasMode_Normal::importToPage()
{
	QString fileName;
	QString allFormats = tr("All Supported Formats")+" (";
	QString formats = "";
	int fmtCode = FORMATID_ODGIMPORT;
	const FileFormat *fmt = LoadSavePlugin::getFormatById(fmtCode);
	while (fmt != 0)
	{
		if (fmt->load)
		{
			formats += fmt->filter + ";;";
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
	allFormats += "*.sce *.SCE ";
	allFormats += "*.shape *.SHAPE ";
	allFormats += "*.sml *.SML);;";
	formats += "Scribus Objects (*.sce *.SCE);;";
	formats += "Dia Shapes (*.shape *.SHAPE);;";
	formats += "Kivio Stencils (*.sml *.SML)";
	allFormats += formats;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("pastefile", ".");
	CustomFDialog dia(m_view, wdir, tr("Open"), allFormats, fdHidePreviewCheckBox | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		PrefsManager::instance()->prefsFile->getContext("dirs")->set("pastefile", fileName.left(fileName.lastIndexOf("/")));
		m_doc->setLoading(true);
		QFileInfo fi(fileName);
		if (fi.suffix().toLower() == "sml")
		{
			QString f = "";
			loadText(fileName, &f);
			StencilReader *pre = new StencilReader();
			fileName = pre->createObjects(f);
			delete pre;
		}
		else if (fi.suffix().toLower() == "shape")
		{
			QString f = "";
			loadText(fileName, &f);
			StencilReader *pre = new StencilReader();
			fileName = pre->createShape(f);
			delete pre;
		}
		bool savedAlignGrid = m_doc->useRaster;
		bool savedAlignGuides = m_doc->SnapGuides;
		m_doc->useRaster = false;
		m_doc->SnapGuides = false;
		if (fi.suffix().toLower() == "sce")
			m_ScMW->slotElemRead(fileName, Mxp, Myp, true, false, m_doc, m_doc->view());
		else if ((fi.suffix().toLower() == "shape") || (fi.suffix().toLower() == "sml"))
			m_ScMW->slotElemRead(fileName, Mxp, Myp, false, true, m_doc, m_doc->view());
		else
		{
			FileLoader *fileLoader = new FileLoader(fileName);
			int testResult = fileLoader->TestFile();
			delete fileLoader;
			if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT))
			{
				const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
				if( fmt )
					fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
			}
			if (m_doc->m_Selection->count() > 0)
			{
				double x2, y2, w, h;
				m_doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
				m_doc->moveGroup(Mxp - x2, Myp - y2);
				m_ScMW->propertiesPalette->updateColorList();
				m_ScMW->propertiesPalette->paraStyleCombo->updateFormatList();
				m_ScMW->propertiesPalette->charStyleCombo->updateFormatList();
				m_ScMW->propertiesPalette->SetLineFormats(m_doc);
			}
		}
		for (int a = 0; a < m_doc->m_Selection->count(); ++a)
		{
			PageItem *currItem = m_doc->m_Selection->itemAt(a);
			currItem->LayerNr = m_doc->activeLayer();
		}
		m_doc->useRaster = savedAlignGrid;
		m_doc->SnapGuides = savedAlignGuides;
		m_doc->setLoading(false);
		m_doc->view()->DrawNew();
		if (m_doc->m_Selection->count() > 0)
		{
			m_doc->m_Selection->connectItemToGUI();
			m_ScMW->HaveNewSel(m_doc->m_Selection->itemAt(0)->itemType());
		}
	}
}

void CanvasMode_Normal::createContextMenu(PageItem* currItem, double mx, double my)
{
	ContextMenu* cmen=NULL;
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	m_view->setObjectUndoMode();
	Mxp = mx;
	Myp = my;
	if(currItem!=NULL)
		cmen = new ContextMenu(*(m_doc->m_Selection), m_ScMW, m_doc);
	else
		cmen = new ContextMenu(m_ScMW, m_doc, mx, my);
	if (cmen)
		cmen->exec(QCursor::pos());
	m_view->setGlobalUndoMode();
	delete cmen;
}
