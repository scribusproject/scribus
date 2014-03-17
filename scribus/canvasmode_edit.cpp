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
#include "hruler.h"
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
	m_blinker = new QTimer(view);	
	connect(m_blinker, SIGNAL(timeout()), this, SLOT(blinkTextCursor()));
	connect(view->horizRuler, SIGNAL(MarkerMoved(double, double)), this, SLOT(rulerPreview(double, double)));
	m_RulerGuide = -1;
	m_longCursorTime=false;
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

void CanvasMode_Edit::keyPressEvent(QKeyEvent *e)
{
	PageItem* currItem;
	if (GetItem(&currItem))
	{
		PageItem_TextFrame* textframe = currItem->asTextFrame();
		if (textframe)
		{
			m_cursorVisible=true;
			int kk = e->key();
			switch (kk)
			{
				case Qt::Key_PageUp:
				case Qt::Key_PageDown:
				case Qt::Key_Up:
				case Qt::Key_Down:
				case Qt::Key_Home:
				case Qt::Key_End:
					m_longCursorTime=true;
					break;
				default:
					m_longCursorTime=false;
					break;
			}
			blinkTextCursor();
		}
	}
}

void CanvasMode_Edit::rulerPreview(double base, double xp)
{
	PageItem* currItem;
	if (m_doc->appMode == modeEdit && GetItem(&currItem))
	{
		QPoint oldP = m_canvas->canvasToLocal(QPointF(m_RulerGuide, currItem->yPos()));
		m_RulerGuide = base + xp;
		QPoint p = m_canvas->canvasToLocal(QPointF(m_RulerGuide, currItem->yPos() + currItem->height()));
		m_canvas->update(QRect(oldP.x()-2, oldP.y(), p.x()+2, p.y()));
	}
}

void CanvasMode_Edit::drawControls(QPainter* p)
{
	commonDrawControls(p, false);
	PageItem* currItem;
	if (GetItem(&currItem))
	{
		PageItem_TextFrame* textframe = currItem->asTextFrame();
		if (textframe)
		{
			if (m_RulerGuide >= 0)
			{
				p->save();
				p->translate(currItem->xPos(), currItem->yPos());
				p->rotate(currItem->rotation());
				p->setPen(QPen(Qt::blue, 1.0 / m_canvas->scale(), Qt::DashLine, Qt::FlatCap, Qt::MiterJoin));
				p->setClipRect(QRectF(0.0, 0.0, currItem->width(), currItem->height()));
				p->setBrush(Qt::NoBrush);
				p->setRenderHint(QPainter::Antialiasing);
				if (currItem->imageFlippedH())
				{
					p->translate(currItem->width(), 0);
					p->scale(-1, 1);
				}
				if (currItem->imageFlippedV())
				{
					p->translate(0, currItem->height());
					p->scale(1, -1);
				}
				p->drawLine(m_RulerGuide - currItem->xPos(), 0, m_RulerGuide - currItem->xPos(), currItem->height());
				p->restore();
			}
			drawTextCursor(p, textframe);
		}
	}
}


void CanvasMode_Edit::drawTextCursor ( QPainter *p, PageItem_TextFrame* textframe )
{
	if ((!m_longCursorTime && m_blinkTime.elapsed() > qApp->cursorFlashTime() / 2 ) ||
		(m_longCursorTime && m_blinkTime.elapsed() > qApp->cursorFlashTime() )
		)
	{
		m_cursorVisible = !m_cursorVisible;
		m_blinkTime.restart();
		m_longCursorTime=false;
	}
	if ( m_cursorVisible )
	{
		// Debug
// 		QString dbgString;
// 		int lif(qMax(textframe->itemText.cursorPosition() , textframe->lastInFrame()));
// 		for(int ti(textframe->firstInFrame());ti < lif; ++ti)
// 		{
// 			if(ti == textframe->itemText.cursorPosition() )
// 			{
// 				dbgString += "["+QString::number(textframe->itemText.cursorPosition())+"]";
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
		QPen cPen ( Qt::black, 1.5 , Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin );
		cPen.setCosmetic(true); // #10049 : make cursor line width constant on screen

		// normalize Current Position
		textframe->itemText.normalizeCursorPosition();
		int textCursorPos ( textframe->itemText.cursorPosition() );

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
			textCursorPos = qMax(0, textframe->lastInFrame());
			QChar textCursorChar = textframe->itemText.text(textCursorPos);
			if (textCursorChar == SpecialChars::PARSEP || textCursorChar == SpecialChars::LINEBREAK)
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
			else if (textCursorChar == SpecialChars::COLBREAK)
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
		if (textframe->imageFlippedH())
			dx = textframe->width() - dx;
		if (textframe->imageFlippedV())
		{
			dy  = textframe->height() - dy;
			dy1 = textframe->height() - dy1;
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
	if (fromGesture)
	{
		m_view->update();
	}
	m_RulerGuide = -1;
	PageItem * it(0);
	if (GetItem(&it))
	{
		if (it->asTextFrame())
		{
			m_canvas->setupEditHRuler(it, true);
			if (m_doc->appMode == modeEdit)
			{
				m_blinker->start(200);
				m_blinkTime.start();
				m_cursorVisible = true;
				blinkTextCursor();
			}
		}
	}
}

void CanvasMode_Edit::deactivate(bool forGesture)
{
//	qDebug() << "CanvasMode_Edit::deactivate" << forGesture;
	m_view->redrawMarker->hide();
	if (!forGesture)
	{
		m_RulerGuide = -1;
		m_blinker->stop();
	}
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
		currItem->asTextFrame()->lastUndoAction = PageItem::NOACTION;
		//CB if annotation, open the annotation dialog
		if (currItem->isAnnotation())
		{
			m_view->requestMode(submodeAnnotProps);
		}
		//otherwise, select between the whitespace
		else
		{
			if (m->modifiers() & Qt::ControlModifier)
			{
				int start=0, stop=0;

				if (m->modifiers() & Qt::ShiftModifier)
				{//Double click with Ctrl+Shift in a frame to select few paragraphs
					uint oldPar = currItem->itemText.nrOfParagraph(oldCp);
					uint newPar = currItem->itemText.nrOfParagraph();
					if (oldPar < newPar)
					{
						start = currItem->itemText.startOfParagraph(oldPar);
						stop = currItem->itemText.endOfParagraph(newPar);
					}
					else
					{
						start = currItem->itemText.startOfParagraph(newPar);
						stop = currItem->itemText.endOfParagraph(oldPar);
					}
				}
				else
				{//Double click with Ctrl in a frame to select a paragraph
					oldCp = currItem->itemText.cursorPosition();
					uint nrPar = currItem->itemText.nrOfParagraph(oldCp);
					start = currItem->itemText.startOfParagraph(nrPar);
					stop = currItem->itemText.endOfParagraph(nrPar);
				}
				currItem->itemText.deselectAll();
				currItem->itemText.extendSelection(start, stop);
				currItem->itemText.setCursorPosition(stop);
			}
			else
			{	//Double click in a frame to select a word
				oldCp = currItem->itemText.cursorPosition();
				currItem->itemText.setCursorPosition( currItem->itemText.selectWord(currItem->itemText.cursorPosition()) );
			}
			currItem->HasSel = (currItem->itemText.lengthOfSelection() > 0);
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
	PageItem *currItem;
	//bool erf = false;
	m->accept();
	if (commonMouseMove(m))
		return;
	if (GetItem(&currItem))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		if (m_doc->DragP)
			return;
		if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeEdit))
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
				int refStartSel(currItem->asTextFrame()->itemText.startOfSelection());
				int refEndSel(currItem->asTextFrame()->itemText.endOfSelection());
				currItem->itemText.deselectAll();
				currItem->HasSel = false;
				m_view->slotSetCurs(m->globalPos().x(), m->globalPos().y());
				//Make sure we dont go here if the old cursor position was not set
				if (oldCp!=-1 && currItem->itemText.length() > 0)
				{
					int cPos = currItem->itemText.cursorPosition();
					if (currItem->itemText.cursorPosition() < oldCp)
					{
						currItem->itemText.select(cPos, oldCp - cPos);
						currItem->HasSel = true;
					}
					if (currItem->itemText.cursorPosition() > oldCp)
					{
						currItem->itemText.select(oldCp, cPos - oldCp);
						currItem->HasSel = true;
					}
				}

				if(currItem->HasSel)
				{
					m_ScMW->EnableTxEdit();
					m_canvas->m_viewMode.operTextSelecting = true;
					if((refStartSel != currItem->asTextFrame()->itemText.startOfSelection())
						|| (refEndSel != currItem->asTextFrame()->itemText.endOfSelection()))
					{
						QRectF br(currItem->getBoundingRect());
						m_canvas->update(QRectF(m_canvas->canvasToLocal(br.topLeft()), br.size() * m_canvas->scale()).toRect());
					}
				}
				else
					m_ScMW->DisableTxEdit();

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
				int hitTest = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
				if(hitTest >= 0)
				{
					if(hitTest == Canvas::INSIDE)
					{
						if (currItem->asTextFrame())
							m_view->setCursor(QCursor(Qt::IBeamCursor));
						if (currItem->asImageFrame())
							m_view->setCursor(QCursor(loadIcon("HandC.xpm")));
					}
				}
				else
				{
// 					setModeCursor();
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
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());

	bool inText;
	PageItem *currItem;
	m_canvas->PaintSizeRect(QRect());
	FPoint npf, npf2;
	QMatrix pm;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	Mxp = mousePointDoc.x();
	Myp = mousePointDoc.y();
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
	int oldP=0;
	if (GetItem(&currItem))
	{
//		m_view->slotDoCurs(false);
		if ((!currItem->locked() || currItem->isTextFrame()) && !currItem->asLine())
		{
			FPoint canvasPoint = m_canvas->globalToCanvas(m->globalPos());
			if (m_canvas->frameHitTest(QPointF(canvasPoint.x(), canvasPoint.y()), currItem) < 0)
			{
				m_doc->m_Selection->delaySignalsOn();
				m_view->Deselect(true);
				bool wantNormal = true;
				if (SeleItem(m))
				{
					currItem = m_doc->m_Selection->itemAt(0);
					if ((currItem->asTextFrame()) || (currItem->asImageFrame()))
					{
						m_view->requestMode(modeEdit);
						wantNormal = false;
					}
					else
					{
						m_view->requestMode(submodePaintingDone);
						m_view->setCursor(QCursor(Qt::ArrowCursor));
					}
					if (currItem->asTextFrame())
					{
						m_view->slotSetCurs(m->globalPos().x(), m->globalPos().y());
						currItem->asTextFrame()->lastUndoAction = PageItem::NOACTION;
					}
				}
				else
				{
					m_view->requestMode(submodePaintingDone);
					m_view->setCursor(QCursor(Qt::ArrowCursor));
				}
				m_doc->m_Selection->delaySignalsOff();
				if (wantNormal)
					m_view->requestMode(modeNormal);
				return;
			}
		}
		oldP = currItem->itemText.cursorPosition();
		//CB Where we set the cursor for a click in text frame
		if (currItem->asTextFrame())
		{
			currItem->asTextFrame()->lastUndoAction = PageItem::NOACTION;
			inText = m_view->slotSetCurs(m->globalPos().x(), m->globalPos().y());
			//CB If we clicked outside a text frame to go out of edit mode and deselect the frame
			if (!inText)
			{
				currItem->invalidateLayout();
				m_view->Deselect(true);
				//m_view->slotDoCurs(true);
				m_view->requestMode(modeNormal);
				if (currItem->isTextFrame())
					currItem->asTextFrame()->lastUndoAction = PageItem::NOACTION;
				return;
			}

			if (m->button() != Qt::RightButton)
			{
				//currItem->asTextFrame()->deselectAll();
				//<<CB Add in shift select to text frames
				if (m->modifiers() & Qt::ShiftModifier)
				{
					if (currItem->itemText.lengthOfSelection() > 0)
					{
						if (currItem->itemText.cursorPosition() < (currItem->itemText.startOfSelection() + currItem->itemText.endOfSelection()) / 2)
						{
							if (m->modifiers() & Qt::ControlModifier)
								currItem->itemText.setCursorPosition( currItem->itemText.startOfParagraph() );
							oldP = currItem->itemText.startOfSelection();
						}
						else
						{
							if (m->modifiers() & Qt::ControlModifier)
								currItem->itemText.setCursorPosition( currItem->itemText.endOfParagraph() );
							oldP = currItem->itemText.endOfSelection();
						}
						currItem->asTextFrame()->itemText.extendSelection(oldP, currItem->itemText.cursorPosition());
						currItem->asTextFrame()->lastUndoAction = PageItem::NOACTION;
						oldCp = currItem->itemText.cursorPosition();
					}
					else
					{
						int dir=1;
						if (oldCp > currItem->itemText.cursorPosition())
							dir=-1;
						if (m->modifiers() & Qt::ControlModifier) //no selection but Ctrl+Shift+click still select paragraphs
						{
							if (dir == 1)
								currItem->itemText.setCursorPosition( currItem->itemText.endOfParagraph() );
							else
								currItem->itemText.setCursorPosition( currItem->itemText.startOfParagraph() );
						}
						currItem->asTextFrame()->ExpandSel(dir, oldP);
						currItem->asTextFrame()->lastUndoAction = PageItem::NOACTION;
						oldCp = oldP;
					}
				}
				else //>>CB
				{
					oldCp = currItem->itemText.cursorPosition();
					currItem->itemText.deselectAll();
					currItem->asTextFrame()->lastUndoAction = PageItem::NOACTION;
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
						currItem->oldCPos = 0;
						currItem->itemText.insertChars(0, cc, true);
						currItem->asTextFrame()->updateUndo(PageItem::INS,cc);
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
			m_doc->m_Selection->delaySignalsOn();
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
					m_view->setCursor(QCursor(Qt::ArrowCursor));
				}
			}
			else
			{
				m_view->requestMode(submodePaintingDone);
				m_view->setCursor(QCursor(Qt::ArrowCursor));
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
	QRectF mpo;
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	Mxp = mousePointDoc.x();
	Myp = mousePointDoc.y();
	double grabRadius = m_doc->guidesSettings.grabRad / m_canvas->scale();
	int MxpS = static_cast<int>(mousePointDoc.x());
	int MypS = static_cast<int>(mousePointDoc.y());
	mpo = QRectF(Mxp-grabRadius, Myp-grabRadius, grabRadius*2, grabRadius*2);
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
				m_view->setCursor(QCursor(Qt::SizeAllCursor));
		}
		else
		{
			m_view->setCursor(QCursor(Qt::SizeAllCursor));
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

void CanvasMode_Edit::createContextMenu(PageItem* currItem, double mx, double my)
{
	ContextMenu* cmen=NULL;
	m_view->setCursor(QCursor(Qt::ArrowCursor));
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
