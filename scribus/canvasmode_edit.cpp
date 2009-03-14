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


#include "canvasmode_edit.h"

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
#include "canvasgesture_resize.h"
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
#include "sccolorengine.h"
#include "scmimedata.h"
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


CanvasMode_Edit::CanvasMode_Edit(ScribusView* view) : CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	oldCp = Cp = -1;
	frameResizeHandle = -1;
	resizeGesture = NULL;
	m_blinker = new QTimer(view);	
	connect(m_blinker, SIGNAL(timeout()), this, SLOT(blinkTextCursor()));
}

inline bool CanvasMode_Edit::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}


void CanvasMode_Edit::blinkTextCursor()
{
	PageItem* currItem;
	if (m_doc->appMode == modeEdit && GetItem(&currItem))
	{
		QRectF brect = currItem->getBoundingRect();
//		qDebug() << "update cursor" << brect;
		m_canvas->update(QRectF(m_canvas->canvasToLocal(brect.topLeft()), QSizeF(brect.width(),brect.height())*m_canvas->scale()).toRect());
	}
}

void CanvasMode_Edit::drawControls(QPainter* p)
{
	commonDrawControls(p);
	PageItem* currItem;
	if (GetItem(&currItem))
	{
		PageItem_TextFrame* textframe = currItem->asTextFrame();
		if (textframe)
			drawTextCursor(p, textframe);
	}
}


void CanvasMode_Edit::drawTextCursor ( QPainter *p, PageItem_TextFrame* textframe )
{
	if(textframe->lastInFrame() < 0)
		return;
	if ( m_blinkTime.elapsed() > qApp->cursorFlashTime() / 2 )
	{
		m_cursorVisible = !m_cursorVisible;
		m_blinkTime.restart();
	}
	if ( m_cursorVisible )
	{
		// Debug
// 		QString dbgString;
// 		int lif(qMax(textframe->CPos , textframe->lastInFrame()));
// 		for(int ti(textframe->firstInFrame());ti < lif; ++ti)
// 		{
// 			if(ti == textframe->CPos )
// 			{
// 				dbgString += "["+QString::number(textframe->CPos)+"]";
// 			}
// 			dbgString += textframe->itemText.text(ti);
// 		}
// 		dbgString +="]"+QString::number(textframe->lastInFrame())+"[";
// 		qDebug()<<"==============================================================";
// 		qDebug()<<textframe->CPos<<textframe->lastInFrame();
// 		qDebug()<<dbgString;
// 		qDebug()<<"==============================================================";
		// end debug
		double dx, dy, dy1;
		QPen cPen ( Qt::black, 0.9 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin );

		// normalize Current Position
		textframe->CPos = qMax ( 0,qMin ( textframe->CPos, textframe->itemText.length() ) );
		int textCursorPos ( textframe->CPos );

		if ( textframe->lastInFrame() >= signed ( textframe->itemText.nrOfItems() )
		        || textframe->itemText.length() == 0 )
		{
			dx = textframe->textToFrameDistLeft();
			dy = textframe->textToFrameDistTop();
			dy1 = textframe->textToFrameDistTop() + (textframe->itemText.defaultStyle().charStyle().fontSize() / 10.0);

			cPen.setColor ( ScColorEngine::getRGBColor ( m_doc->PageColors[textframe->itemText.defaultStyle().charStyle().fillColor() ], m_doc ) );
		}
		else if ( textCursorPos > textframe->lastInFrame() )
		{
			// Happens often when typing directly into frame.
			// And the cursor curses nothing, vertigo.
			textCursorPos = textframe->lastInFrame();
			if(textframe->itemText.text(textCursorPos) == SpecialChars::PARSEP)
			{
				// The cursor must be moved to the beginning of the next line
				FRect bbox = textframe->itemText.boundingBox ( textCursorPos );
				double lineSpacing(textframe->itemText.paragraphStyle(textCursorPos).lineSpacing());
				
				// take care if cursor is not in first column
				int curCol(1);
				double ccPos(textframe->itemText.boundingBox ( textCursorPos ).x());
				double leftOffset(textframe->textToFrameDistLeft());
				for(int ci(1); ci <= textframe->columns(); ++ci)
				{
					double cLeft(((ci-1) * textframe->columnWidth()) + ((ci -1) * textframe->columnGap()) + leftOffset);
					double cRight((ci * textframe->columnWidth()) + ((ci -1) * textframe->columnGap()) + leftOffset);
					if((cLeft <= ccPos) && (ccPos <= cRight))
					{
						curCol = ci;
						break;
					}
				}
				dx = (textframe->columnWidth() * (curCol - 1)) + (textframe->columnGap() * (curCol - 1))  + leftOffset;
				dy = bbox.y();
				
				if ( bbox.height() <= 2 )
					dy1 = bbox.y() + textframe->itemText.charStyle ( textCursorPos ).fontSize() / 30.0;
				else
					dy1 = bbox.y() + bbox.height();
				
				dy  += lineSpacing;
				dy1 += lineSpacing;
			}
			else if((textframe->itemText.text(textCursorPos) == SpecialChars::COLBREAK))
			{
				// A bit tricky :)
				// We want to position the cursor at the beginning of the next column, if any.
				// At first we need to know in which column the cursor is.
				int curCol(1);
				double ccPos(textframe->itemText.boundingBox ( textCursorPos ).x());
				double leftOffset(textframe->textToFrameDistLeft());
				for(int ci(1); ci <= textframe->columns(); ++ci)
				{
					double cLeft(((ci-1) * textframe->columnWidth()) + ((ci -1) * textframe->columnGap()) + leftOffset);
					double cRight((ci * textframe->columnWidth()) + ((ci -1) * textframe->columnGap()) + leftOffset);
					if((cLeft <= ccPos) && (ccPos <= cRight))
					{
						curCol = ci;
						break;
					}
				}
				if(textframe->columns() > curCol)
				{
					dx = (textframe->columnWidth() * curCol) + (textframe->columnGap() * curCol)  + leftOffset;
					dy = textframe->textToFrameDistTop();
					dy1 = textframe->textToFrameDistTop() + textframe->itemText.boundingBox ( textCursorPos ).height();
				}
				else // there is no column after the current column
				{
					FRect bbox = textframe->itemText.boundingBox ( textCursorPos );
					dx = bbox.x();
					dy = bbox.y();
					dx += textframe->itemText.item ( textCursorPos )->glyph.wide();
					if ( bbox.height() <= 2 )
						dy1 = bbox.y() + textframe->itemText.charStyle ( textCursorPos ).fontSize() / 30.0;
					else
						dy1 = bbox.y() + bbox.height();
				}
			}
			else
			{
				FRect bbox = textframe->itemText.boundingBox ( textCursorPos );
				dx = bbox.x();
				dy = bbox.y();
				dx += textframe->itemText.item ( textCursorPos )->glyph.wide();
				if ( bbox.height() <= 2 )
					dy1 = bbox.y() + textframe->itemText.charStyle ( textCursorPos ).fontSize() / 30.0;
				else
					dy1 = bbox.y() + bbox.height();
	
			}
			cPen.setColor ( ScColorEngine::getRGBColor ( m_doc->PageColors[textframe->itemText.charStyle ( textCursorPos ).fillColor() ], m_doc ) );
		}
		else
		{
			FRect bbox = textframe->itemText.boundingBox ( textCursorPos );
			dx = bbox.x();
			dy = bbox.y();
			if ( bbox.height() <= 2 )
				dy1 = bbox.y() + textframe->itemText.charStyle ( textCursorPos ).fontSize() / 30.0;
			else
				dy1 = bbox.y() + bbox.height();

			cPen.setColor ( ScColorEngine::getRGBColor ( m_doc->PageColors[textframe->itemText.charStyle ( textCursorPos ).fillColor() ], m_doc ) );
		}
		//handle Right to Left writing
		if ( textframe->reversed() )
		{
			dx=textframe->width()-dx;
		}

		p->save();
		p->translate ( textframe->xPos(), textframe->yPos() );
		p->rotate ( textframe->rotation() );


		p->setPen ( cPen );
		p->setRenderHint ( QPainter::Antialiasing, true );
		// avoid displaying the cursor on the frameborder
		dx = qMax ( ( cPen.widthF() / 2.0 ), dx );

		dy =  qMin ( qMax ( dy,0.0 ) , textframe->height() );
		dy1 = qMin ( qMax ( dy1,0.0 ), textframe->height() );

		p->drawLine ( QLineF ( dx,dy,dx,dy1 ) );

		p->restore();
	}
}

void CanvasMode_Edit::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}

void CanvasMode_Edit::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void CanvasMode_Edit::activate(bool fromGesture)
{
//	qDebug() << "CanvasMode_Edit::activate" << fromGesture;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	oldCp = Cp = -1;
	frameResizeHandle = -1;
	setModeCursor();
	if (m_doc->appMode == modeEdit)
	{
		m_blinker->start(500);
		m_blinkTime.start();
		m_cursorVisible = true;
	}
	if (fromGesture)
	{
		m_canvas->m_viewMode.operItemResizeInEditMode = false;
		m_view->update();
	}
	PageItem * it(0);
	if(GetItem(&it))
	{
		m_canvas->setupEditHRuler(it, true);
	}
}

void CanvasMode_Edit::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_Edit::deactivate" << forGesture;
	m_view->redrawMarker->hide();
	if (!forGesture)
		m_blinker->stop();
}

void CanvasMode_Edit::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
//	m_view->stopDragTimer();
	PageItem *currItem = 0;
	if (GetItem(&currItem) && (m_doc->appMode == modeEdit) && currItem->asTextFrame())
	{
		//CB if annotation, open the annotation dialog
		if (currItem->isAnnotation())
		{
			m_view->requestMode(submodeAnnotProps);
		}
		//otherwise, select between the whitespace
		else
		{	//Double click in a frame to select a word
			oldCp = currItem->CPos;
			currItem->CPos = currItem->itemText.selectWord(currItem->CPos);
		}
	}
	else
	{
		mousePressEvent(m);
		return;
	}
}


void CanvasMode_Edit::mouseMoveEvent(QMouseEvent *m)
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
	QRect tx;
	bool erf = false;
	m->accept();
	if (commonMouseMove(m))
		return;
	if (GetItem(&currItem))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		if (m_doc->DragP)
			return;
		if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeEdit) && (!m_canvas->m_viewMode.operItemResizeInEditMode))
		{
			if (currItem->asImageFrame())
			{
				QMatrix ro;
				ro.rotate(-currItem->rotation());
				QPointF rota = ro.map(QPointF(newX-Mxp,newY-Myp));
				currItem->moveImageInFrame(rota.x()/currItem->imageXScale(), rota.y()/currItem->imageYScale());
		//		m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				currItem->update();
				Mxp = newX;
				Myp = newY;
				m_canvas->displayXYHUD(m->globalPos(), currItem->imageXOffset() * currItem->imageXScale(), currItem->imageYOffset() * currItem->imageYScale());
			}
			if (currItem->asTextFrame())
			{
				currItem->itemText.deselectAll();
				currItem->HasSel = false;
				m_view->slotSetCurs(m->x(), m->y());
				//Make sure we dont go here if the old cursor position was not set
				if (oldCp!=-1 && currItem->itemText.length() > 0)
				{
					if (currItem->CPos < oldCp)
					{
						currItem->itemText.select(currItem->CPos, oldCp - currItem->CPos);
						currItem->HasSel = true;
					}
					if (currItem->CPos > oldCp)
					{
						currItem->itemText.select(oldCp, currItem->CPos - oldCp);
						currItem->HasSel = true;
					}
				}
// 				currItem->update();
				if(currItem->HasSel)
				{
					m_ScMW->EnableTxEdit();
					m_canvas->m_viewMode.operTextSelecting = true;
				}
				else
					m_ScMW->DisableTxEdit();
				m_canvas->update();
			}
		}
		//Operations run here:
		//Item resize, esp after creating a new one
		if (m_view->moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::LeftButton) && 
			((m_doc->appMode == modeEdit) && m_canvas->m_viewMode.operItemResizeInEditMode) && (!currItem->locked()))
		{
//			m_view->stopDragTimer();
			if (m_canvas->m_viewMode.operItemResizing)
			{
//				newX = static_cast<int>(m->x()/sc);
//				newY = static_cast<int>(m->y()/sc);
				m_canvas->m_viewMode.operItemMoving = false;
				if (m_doc->m_Selection->isMultipleSelection())
				{
// 					qDebug()<<"Resizing multiple selection"<<frameResizeHandle;
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
// 					qDebug()<<"frameResizeHandle"<<frameResizeHandle;
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
		if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		{
			if (m_doc->m_Selection->isMultipleSelection())
			{
				setModeCursor();
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
				if ((QRegion(p.map(QPolygon(QRect(-3, -3, static_cast<int>(currItem->width()+6), static_cast<int>(currItem->height()+6))))).contains(mpo))
					&& m_doc->appMode == modeEdit)
				{
					tx = p.mapRect(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
					if ((tx.intersects(mpo)) && (!currItem->locked()))
					{
						if (currItem->asTextFrame())
							qApp->changeOverrideCursor(QCursor(Qt::IBeamCursor));
						if (currItem->asImageFrame())
							qApp->changeOverrideCursor(QCursor(loadIcon("HandC.xpm")));
						if (!currItem->sizeLocked())
						{
// 							m_view->HandleCurs(currItem, mpo);
							int how = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
							if (how > 0)
								setResizeCursor(how, currItem->rotation());
						}
					}
				}
				else
				{
// 					setModeCursor();
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
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
			/*
			m_view->redrawMarker->setGeometry(QRect(Mxp, Myp, m->globalPos().x() - Mxp, m->globalPos().y() - Myp).normalized());
			*/
			QPoint startP = m_canvas->canvasToGlobal(QPointF(Mxp, Myp));
			m_view->redrawMarker->setGeometry(QRect(startP, m->globalPos()).normalized());
			if (!m_view->redrawMarker->isVisible())
				m_view->redrawMarker->show();
			m_view->HaveSelRect = true;
			return;
		}
	}
}

void CanvasMode_Edit::mousePressEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());

	bool inText;
	double Rxp = 0;
	double Ryp = 0;
// 	Not used said gcc
// 	double Rxpd = 0;
// 	double Rypd = 0;
	PageItem *currItem;
	m_canvas->PaintSizeRect(QRect());
	FPoint npf, npf2;
	QRect tx;
	QMatrix pm;
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
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	Ryp = Myp;
	Rxp = Mxp;
	SeRx = Mxp;
	SeRy = Myp;
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		if (m->modifiers() & Qt::ControlModifier)
			m_view->DrawNew();
		return;
	}

	frameResizeHandle = 0;
	m_canvas->m_viewMode.operItemResizeInEditMode = false;
	int oldP=0;
	if (GetItem(&currItem))
	{
//		m_view->slotDoCurs(false);
		if (!currItem->locked() && !currItem->asLine())
		{
			if (!resizeGesture)
				resizeGesture = new ResizeGesture(this);
			
			resizeGesture->mousePressEvent(m);
			if (resizeGesture->frameHandle() > 0)
			{
				m_canvas->m_viewMode.operItemResizeInEditMode = true;
				m_view->startGesture(resizeGesture);
				return;
			}
			else if (resizeGesture->frameHandle() < 0)
			{
				m_view->Deselect(true);
				if (SeleItem(m))
				{
					currItem = m_doc->m_Selection->itemAt(0);
					if ((currItem->asTextFrame()) || (currItem->asImageFrame()))
					{
						m_view->requestMode(modeEdit);
					}
					else
					{
						m_view->requestMode(submodePaintingDone);
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					}
				}
				else
				{
					m_view->requestMode(submodePaintingDone);
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				}
				m_view->requestMode(modeNormal);
				return;
			}
		}
		oldP = currItem->CPos;
		//CB Where we set the cursor for a click in text frame
		if (currItem->asTextFrame())
		{
			inText = m_view->slotSetCurs(m->x(), m->y());
			//CB If we clicked outside a text frame to go out of edit mode and deselect the frame
			if (!inText)
			{
				currItem->invalidateLayout();
				m_view->Deselect(true);
				//m_view->slotDoCurs(true);
				m_view->requestMode(modeNormal);
				return;
			}
			if (m->button() != Qt::RightButton)
			{
				//<<CB Add in shift select to text frames
				if (m->modifiers() & Qt::ShiftModifier && currItem->itemText.lengthOfSelection() > 0)
				{
					if (currItem->CPos < (currItem->itemText.startOfSelection() + currItem->itemText.endOfSelection()) / 2)
						oldP = currItem->itemText.startOfSelection();
					else 
						oldP = currItem->itemText.endOfSelection();
					currItem->asTextFrame()->itemText.extendSelection(oldP, currItem->CPos);
					oldCp = currItem->CPos;
				}
				else //>>CB
				{
					oldCp = currItem->CPos;
					currItem->itemText.deselectAll();
					currItem->HasSel = false;
				}
				currItem->emitAllToGUI();
				if (m->button() == Qt::MidButton)
				{
					m_canvas->m_viewMode.m_MouseButtonPressed = false;
					m_view->MidButt = false;
					QString cc;
					cc = QApplication::clipboard()->text(QClipboard::Selection);
					if (cc.isNull())
						cc = QApplication::clipboard()->text(QClipboard::Clipboard);
					if (!cc.isNull())
					{
						// K.I.S.S.:
						currItem->itemText.insertChars(0, cc, true);
						if (m_doc->docHyphenator->AutoCheck)
							m_doc->docHyphenator->slotHyphenate(currItem);
						m_ScMW->BookMarkTxT(currItem);
						//							m_ScMW->outlinePalette->BuildTree();
					}
					else
					{
						if (ScMimeData::clipboardHasScribusText())
							m_ScMW->slotEditPaste();
					}
					currItem->update();
				}
			}
		}
		else if (!currItem->asImageFrame() || 
				 m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem) < 0)
		{
			m_view->Deselect(true);
			if (SeleItem(m))
			{
				currItem = m_doc->m_Selection->itemAt(0);
				if ((currItem->asTextFrame()) || (currItem->asImageFrame()))
				{
					m_view->requestMode(modeEdit);
				}
				else
				{
					m_view->requestMode(submodePaintingDone);
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				}
			}
			else
			{
				m_view->requestMode(submodePaintingDone);
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			}
		}
	}
}



void CanvasMode_Edit::mouseReleaseEvent(QMouseEvent *m)
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
	if ((m_doc->m_Selection->count() == 0) && (m_view->HaveSelRect) && (!m_view->MidButt))
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
		/*else
			currItem->emitAllToGUI();*/
	}
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
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

//CB-->Doc/Fix
bool CanvasMode_Edit::SeleItem(QMouseEvent *m)
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

// 		currItem->update();
		m_canvas->update();
		m_doc->m_Selection->delaySignalsOff();
		if (m_doc->m_Selection->count() > 1)
		{
// 			for (int aa = 0; aa < m_doc->m_Selection->count(); ++aa)
// 			{
// 				PageItem *bb = m_doc->m_Selection->itemAt(aa);
// 				bb->update();
// 			}
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
	if ( !(m->modifiers() & SELECT_MULTIPLE))
		m_view->Deselect(true);
	return false;
}

// void CanvasMode_Edit::setResizeCursor(int how, double rot)
// {
// 	switch (how)
// 	{
// 		case 1:
// 		case 2:
// 			qApp->changeOverrideCursor(ScResizeCursor(135 + rot));
// 			break;
// 		case 3:
// 		case 4:
// 			qApp->changeOverrideCursor(ScResizeCursor(45 + rot));
// 			break;
// 		case 5:
// 		case 8:
// 			qApp->changeOverrideCursor(ScResizeCursor(0 + rot));
// 			break;
// 		case 6:
// 		case 7:
// 			qApp->changeOverrideCursor(ScResizeCursor(90 + rot));
// 			break;
// 		default:
// 			qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
// 			break;
// 	}
// }

void CanvasMode_Edit::createContextMenu(PageItem* currItem, double mx, double my)
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
