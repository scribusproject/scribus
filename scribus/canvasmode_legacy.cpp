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


#include "canvasmode_legacy.h"

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


#include "canvas.h"
#include "canvasgesture_resize.h"
#include "canvasgesture_rulermove.h"
#include "contextmenu.h"
#include "customfdialog.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hruler.h"
#include "vruler.h"
#include "hyphenator.h"
#include "insertTable.h"
#include "oneclick.h"
#include "pageitem_textframe.h"
#include "pageselector.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "scraction.h"
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



LegacyMode::LegacyMode(ScribusView* view) : QObject(), CanvasMode(view), m_ScMW(view->m_ScMW) 
{
	GxM = GyM = -1;
	Mxp = Myp = -1;
	Dxp = Dyp = -1;
	MoveGX = MoveGY = false;
	oldCp = Cp = -1;
	frameResizeHandle = -1;
	RotMode = 0;
	inItemCreation = false;
	shiftSelItems = false;
	FirstPoly = true;
	resizeGesture = NULL;
	guideMoveGesture = NULL;
	m_blinker = new QTimer(view);
	connect(m_blinker, SIGNAL(timeout()), this, SLOT(blinkTextCursor()));
}

inline bool LegacyMode::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}


void LegacyMode::blinkTextCursor()
{
	PageItem* currItem;
	if (m_doc->appMode == modeEdit && GetItem(&currItem))
	{
		QRectF brect = currItem->getBoundingRect();
//		qDebug() << "update cursor" << brect;
		m_canvas->update(QRectF(m_canvas->canvasToLocal(brect.topLeft()), QSizeF(brect.width(),brect.height())*m_canvas->scale()).toRect());
	}
}



void LegacyMode::drawControls(QPainter* p)
{
//	qDebug() << "LegacyMode::drawControls";
	if (m_canvas->m_viewMode.operItemMoving)
	{
		drawOutline(p);
	}
	else
	{
		drawSelection(p);
	}
	PageItem* currItem;
	if (m_doc->appMode == modeEdit && GetItem(&currItem))
	{
		PageItem_TextFrame* textframe = currItem->asTextFrame();
		if (textframe)
			drawTextCursor(p, textframe);
	}
}


void LegacyMode::drawTextCursor(QPainter *p, PageItem_TextFrame* textframe)
{
	int x, y, y1;
	if (textframe->CPos > textframe->itemText.length())
	{
		textframe->CPos = textframe->itemText.length();
	}
	if (textframe->lastInFrame() >= signed(textframe->itemText.nrOfItems()) 
		|| textframe->itemText.length() == 0)
	{
		x = 0;
		y = 0;
		y1 = static_cast<int>(textframe->itemText.defaultStyle().charStyle().fontSize() / 10);
	}
	else if ( textframe->CPos > textframe->itemText.endOfItem(textframe->lastInFrame())
			  || (textframe->CPos >= textframe->itemText.length() && textframe->itemText.text(textframe->itemText.length()-1) != SpecialChars::PARSEP) )
	{
		FRect bbox = textframe->itemText.boundingBox(qMax(0,qMin(textframe->lastInFrame(), textframe->itemText.length()-1)));
		x = static_cast<int>(bbox.x() + textframe->itemText.item(qMax(0,qMin(textframe->lastInFrame(), textframe->itemText.length()-1)))->glyph.wide());
		y = static_cast<int>(bbox.y());
		if (bbox.height() <= 2)
			y1 = static_cast<int>(bbox.y() + textframe->itemText.defaultStyle().charStyle().fontSize() / 30);
		else
			y1 = static_cast<int>(bbox.y() + bbox.height());
	}
	else
	{
		FRect bbox = textframe->itemText.boundingBox(qMax(0,qMin(textframe->CPos, textframe->itemText.length())));
		x = static_cast<int>(bbox.x());
		y = static_cast<int>(bbox.y());
		if (bbox.height() <= 2) 
			y1 = static_cast<int>(bbox.y() + textframe->itemText.charStyle(textframe->CPos).fontSize() / 30);
		else
			y1 = static_cast<int>(bbox.y() + bbox.height());
	}

	// avoid displaying the cursor on the frameborder
	if (x < 1)
		x = 1;
				
	p->save();
	p->translate(textframe->xPos(), textframe->yPos());
	p->rotate(textframe->rotation());
	p->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	if (m_blinkTime.elapsed() > 400)
	{
//		qDebug() << "blink";
		m_cursorVisible = !m_cursorVisible;
		m_blinkTime.restart();
	}
//	else
//		qDebug() << "no blink" << m_blinkTime.elapsed() ;
	if (m_cursorVisible)
	{
		p->drawLine(x, qMin(qMax(y,0),static_cast<int>(textframe->height())), 
				   x, qMin(qMax(y1,0),static_cast<int>(textframe->height())));
	}
	p->restore();
}

void LegacyMode::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		setModeCursor();
	}
}


void LegacyMode::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}


void LegacyMode::activate(bool fromGesture)
{
	qDebug() << "LegacyMode::activate" << fromGesture;
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
	RotMode = 0;
	inItemCreation = false;
	shiftSelItems = false;
	FirstPoly = true;
	setModeCursor();
	if (m_doc->appMode == modeEdit)
	{
		m_blinker->start(500);
		m_blinkTime.start();
		m_cursorVisible = true;
	}
}

void LegacyMode::deactivate(bool forGesture)
{
	qDebug() << "LegacyMode::deactivate" << forGesture;
	m_view->redrawMarker->hide();
	if (!forGesture)
		m_blinker->stop();
}

void LegacyMode::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_view->stopDragTimer();
	PageItem *currItem = 0;
	if ((m_doc->m_Selection->isMultipleSelection()) || (m_doc->appMode != modeNormal))
	{
		if ((m_doc->m_Selection->isMultipleSelection()) && (m_doc->appMode == modeNormal))
		{
			if (GetItem(&currItem))
			{
				if (currItem->isTableItem)
				{
					m_view->Deselect(false);
					m_doc->m_Selection->addItem(currItem);
					currItem->isSingleSel = true;
					//CB FIXME dont call this if the added item is item 0
					if (!m_doc->m_Selection->primarySelectionIs(currItem))
						currItem->emitAllToGUI();
					m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				}
			}
			return;
		}
		else
		{
			if (!(GetItem(&currItem) && (m_doc->appMode == modeEdit) && currItem->asTextFrame()))
			{
				mousePressEvent(m);
				return;
			}
		}
	}
	if (GetItem(&currItem))
	{
		if (currItem->asLatexFrame()) {
			if ((currItem->locked()) || (!currItem->ScaleType))
			{
				return;
			}
			if (currItem->imageShown())
				m_view->requestMode(modeEdit);
		} else if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::PolyLine) || (currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::PathText))
		{
			if ((currItem->locked()) || (!currItem->ScaleType))
			{
				//				mousePressEvent(m);
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
		else
			if (currItem->itemType() == PageItem::TextFrame)
			{
				//CB old code
				//emit currItem->isAnnotation() ? AnnotProps() : Amode(modeEdit);
				//mousePressEvent(m);
				//CB if annotation, open the annotation dialog
				if (currItem->isAnnotation())
				{
					m_view->requestMode(submodeAnnotProps);
					//					mousePressEvent(m);
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
//					mousePressEvent(m);
				}
				//otherwise, select between the whitespace
				else
				{	//Double click in a frame to select a word
					PageItem_TextFrame *cItem=currItem->asTextFrame();
					bool inText = m_view->slotSetCurs(m->x(), m->y());
					if (!inText)
					{
						m_view->Deselect(true);
//						m_view->slotDoCurs(true);
						m_view->requestMode(modeNormal);
						return;
					}
					int a=cItem->CPos;
					while(a>0)
					{
						if (cItem->itemText.text(a-1).isLetterOrNumber())
							--a;
						else
							break;
					}
					int b=cItem->CPos;
					while(b<cItem->itemText.length())
					{
						if (cItem->itemText.text(b).isLetterOrNumber())
							++b;
						else
							break;
					}
					oldCp = a;
					cItem->CPos=b;
					cItem->ExpandSel(1, oldCp);
//					m_view->slotDoCurs(true);
				}
			}
	}
}


void LegacyMode::mouseMoveEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	double newX, newY;
	double nx, ny;
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
/*	if (false && m_doc->guidesSettings.guidesShown)
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
	}*/
/*	if ((!m_doc->DragP) && (m_canvas->m_viewMode.m_MouseButtonPressed) && (SelItem.count() != 0))
	{
		int cx = Anz->contentsX() - Anz->childX(doku->currentPage->parentWidget());
		int cy = Anz->contentsY() - Anz->childY(doku->currentPage->parentWidget());
		int cw = Anz->visibleWidth();
		int ch = Anz->visibleHeight();
		if ((m->y() < cy+10) || ((ch - (m->y() - cy)) - 10 < 0) || (m->x() < cx + 10) || ((cw - (m->x() - cx)) - 10 < 0))
		{
			if (m->y() < cy+10)
				Anz->scrollBy(0, m->y() - cy - 10);
			else if ((ch - (m->y() - cy)) - 10 < 0)
				Anz->scrollBy(0, m->y() - cy - ch +10);
			else if (m->x() < cx + 10)
				Anz->scrollBy(m->x() - cx - 10, 0);
			else if ((cw - (m->x() - cx)) - 10 < 0)
				Anz->scrollBy(m->x() - cx - cw + 10, 0);
			BlockLeave = true;
			update();
		}
	else
		BlockLeave = false;
	} */

	if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeEditGradientVectors))
	{
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		newX = mousePointDoc.x(); //m->x();
		newY = mousePointDoc.y(); //m->y();
		double dx = fabs(Mxp - newX) + 5.0 / m_canvas->scale();
		double dy = fabs(Myp - newY) + 5.0 / m_canvas->scale();
		if (m->buttons() & Qt::LeftButton)
		{
			currItem->GrStartX -= (Mxp - newX); // / m_canvas->scale();
			currItem->GrStartY -= (Myp - newY); // / m_canvas->scale();
		}
		if (m->buttons() & Qt::RightButton)
		{
			currItem->GrEndX -= (Mxp - newX); // / m_canvas->scale();
			currItem->GrEndY -= (Myp - newY); // / m_canvas->scale();
		}
		Mxp = newX;
		Myp = newY;
		m_view->RefreshGradient(currItem, dx * m_canvas->scale(), dy * m_canvas->scale());
		m_ScMW->propertiesPalette->updateColorSpecialGradient();
		return;
	}
	if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeMeasurementTool))
	{
		// bloody hack since signal MVals is declared with ScribusView for now
		// measurements palette should be controlled directly by MeasurementMode
//FIXME:av		m_view->contentsMouseMoveEvent(m);
		
/*
		newX = m->x();
		newY = m->y();
		m_canvas->newRedrawPolygon() << QPoint(Dxp, Dyp) << QPoint(newX, newY);
		m_view->updateContents(QRect(QPoint(Dxp, Dyp), QPoint(newX, newY)).normalized().adjusted(-10, -10, 20, 20));
		Mxp = newX;
		Myp = newY;
		double dxp = Dxp / sc - m_doc->currentPage()->xOffset();//+ m_doc->minCanvasCoordinate.x();
			double dyp = Dyp / sc - m_doc->currentPage()->yOffset();//+ m_doc->minCanvasCoordinate.y();
				double nxp = newX / sc - m_doc->currentPage()->xOffset();//+ m_doc->minCanvasCoordinate.x();
					double nyp = newY / sc - m_doc->currentPage()->yOffset();//+ m_doc->minCanvasCoordinate.y();
		emit MVals(dxp, dyp, nxp, nyp, -xy2Deg(newX/sc - Dxp/sc, newY/sc - Dyp/sc), sqrt(pow(newX/sc - Dxp/sc,2)+pow(newY/sc - Dyp/sc,2)), m_doc->unitIndex());
*/
		return;
	}
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
	if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeDrawFreehandLine))
	{
		//newX = m->x();
		//newY = m->y();
		double newXF = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
		double newYF = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
		if (RecordP.size() > 0)
		{
			if (FPoint(newXF, newYF) != RecordP.point(RecordP.size()-1))
				RecordP.addPoint(FPoint(newXF, newYF));
		}
		else
			RecordP.addPoint(FPoint(newXF, newYF));
		QPolygon& redrawPolygon(m_canvas->newRedrawPolygon());
		for (uint pp = 0; pp < RecordP.size(); pp++)
		{
			redrawPolygon << RecordP.pointQ(pp);
		}
		m_canvas->m_viewMode.operItemResizing = true;
		QRect bRect = m_canvas->redrawPolygon().boundingRect();
//		QPoint in(qRound(bRect.x()*m_canvas->scale()), qRound(bRect.y()*m_canvas->scale()));
//		in -= QPoint(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
//		QPoint out = contentsToViewport(in);
//		m_view->updateContents(QRect(out.x()+0*contentsX(), out.y()+0*contentsY(), qRound(bRect.width()*m_canvas->scale()), qRound(bRect.height()*m_canvas->scale())).adjusted(-10, -10, 20, 20));
		m_view->updateContents(bRect);
		return;
	}
	if ((GetItem(&currItem)) && (!shiftSelItems))
	{
		newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		if ((((m_view->dragTimerElapsed()) && (m->buttons() & Qt::LeftButton)) || (m_view->moveTimerElapsed() && (m->buttons() & Qt::RightButton)))
			&& (m_canvas->m_viewMode.m_MouseButtonPressed)
			&& (!m_doc->DragP) 
			&& (m_doc->appMode == modeNormal) 
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
				QMimeData* md = new QMimeData();
				md->setText(ss->WriteElem(m_doc, m_view, m_doc->m_Selection));
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
		if (m_view->moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeRotation))
		{
			m_view->stopDragTimer();
			double newW = xy2Deg(mousePointDoc.x()-m_view->RCenter.x(), mousePointDoc.y()-m_view->RCenter.y()); //xy2Deg(m->x()/sc - m_view->RCenter.x(), m->y()/sc - m_view->RCenter.y());
			if (m->modifiers() & Qt::ControlModifier)
			{
				newW=constrainAngle(newW, m_doc->toolSettings.constrain);
				m_view->oldW=constrainAngle(m_view->oldW, m_doc->toolSettings.constrain);
				//RotateGroup uses MoveBy so its pretty hard to constrain the result
				if (m_doc->m_Selection->isMultipleSelection())
					m_view->RotateGroup(newW-m_view->oldW);
				else
					m_doc->RotateItem(newW, currItem->ItemNr);
			}
			else
			{
				if (m_doc->m_Selection->isMultipleSelection())
					m_view->RotateGroup(newW - m_view->oldW);
				else
					m_doc->RotateItem(currItem->rotation() - (m_view->oldW - newW), currItem->ItemNr);
			}
			m_view->oldW = newW;
// rotate calls PageItem::update			emit DocChanged();
		}
		if (m_doc->appMode == modeDrawBezierLine)
		{
			if ((m_doc->useRaster) && (m_doc->OnPage(currItem) != -1))
			{
				newX = qRound(newX / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
				newY = qRound(newY / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
			}
			m_canvas->newRedrawPolygon() << QPoint(qRound(newX - currItem->xPos()), qRound(newY - currItem->yPos()));
			m_view->updateContents();
			Mxp = newX;
			Myp = newY;
		}
		if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeDrawRegularPolygon))
		{
			if (m->modifiers() & Qt::ShiftModifier)
			{
//				mop = QPoint(m->x(), static_cast<int>((currItem->yPos() + (newX - currItem->xPos())) * sc));
				newY = static_cast<int>(currItem->yPos() + (newX - currItem->xPos()));
				QCursor::setPos(m_canvas->canvasToGlobal(FPoint(newX, newY)));
//				newY = static_cast<int>(mop.y()/sc);
			}
			if ((m_doc->useRaster) && (m_doc->OnPage(currItem) != -1))
			{
				newX = qRound(newX / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
				newY = qRound(newY / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
			}
			QMatrix mp = currItem->getTransform();
//			mp.translate(currItem->xPos(),// - m_doc->minCanvasCoordinate.x(), 
//						 currItem->yPos()); // - m_doc->minCanvasCoordinate.y());
//			mp.rotate(currItem->rotation());
			np2 = QPoint(static_cast<int>(newX), static_cast<int>(newY)) * mp.inverted();
			QMatrix pm;
			m_canvas->Transform(currItem, pm);
			m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), np2)));
//			emit ItemGeom(newX - currItem->xPos(), newY - currItem->yPos());
			Mxp = newX;
			Myp = newY;
		}
		if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeDrawLine))
		{
			if (m_doc->useRaster)
			{
				newX = qRound(newX / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
				newY = qRound(newY / m_doc->guidesSettings.minorGrid) * m_doc->guidesSettings.minorGrid;
			}
			double newRot=xy2Deg(newX - currItem->xPos(), newY - currItem->yPos());
			//Constrain rotation angle, when the mouse is being dragged around for a new line
			if (m->modifiers() & Qt::ControlModifier)
			{
				//Flip our angles around here
				if (newRot<0.0)
					newRot=-newRot;
				else
					newRot=360-newRot;
				newRot=constrainAngle(newRot, m_doc->toolSettings.constrain);
				double hlen=sqrt(pow(newX - currItem->xPos(),2)+pow(newY - currItem->yPos(),2));
				newX = qRound(currItem->xPos()+(hlen * cos(newRot/(180.0/M_PI))));
				newY = qRound(currItem->yPos()-(hlen * sin(newRot/(180.0/M_PI))));
			}
//			emit SetAngle(newRot);
//			emit ItemGeom(sqrt(pow(newX - currItem->xPos(),2)+pow(newY - currItem->yPos(),2)), 0);
			m_canvas->newRedrawPolygon() << QPoint(qRound(currItem->xPos()), qRound(currItem->yPos())) << QPoint(qRound(newX), qRound(newY));
			newX = mousePointDoc.x(); //m->x();
			newY = mousePointDoc.y(); //m->y();
			m_view->updateCanvas(QRectF(QPointF(Dxp, Dyp), QPointF(newX, newY)).normalized().adjusted(-10, -10, 20, 20));
			Mxp = newX;
			Myp = newY;
		}
		if (m_canvas->m_viewMode.m_MouseButtonPressed && (m_doc->appMode == modeEdit) && (!m_canvas->m_viewMode.operItemResizeInEditMode))
		{
			if (currItem->asImageFrame())
			{
				QMatrix ro;
				ro.rotate(-currItem->rotation());
				QPointF rota = ro.map(QPointF(newX-Mxp,newY-Myp));
				currItem->moveImageInFrame(rota.x()/currItem->imageXScale(), rota.y()/currItem->imageYScale());
				m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				Mxp = newX;
				Myp = newY;
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
				currItem->update();
				if(currItem->HasSel)
					m_ScMW->EnableTxEdit();
				else
					m_ScMW->DisableTxEdit();
			}
		}
		//Operations run here:
		//Item resize, esp after creating a new one
		if (m_view->moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::LeftButton) && 
			((m_doc->appMode == modeNormal) || ((m_doc->appMode == modeEdit) && m_canvas->m_viewMode.operItemResizeInEditMode)) && (!currItem->locked()))
		{
			m_view->stopDragTimer();
			if (m_canvas->m_viewMode.operItemResizing)
			{
//				newX = static_cast<int>(m->x()/sc);
//				newY = static_cast<int>(m->y()/sc);
				m_canvas->m_viewMode.operItemMoving = false;
				if (m_doc->m_Selection->isMultipleSelection())
				{
//					newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
//					newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
					double gx, gy, gh, gw;
					m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
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
					//qDebug(QString("frameResizeHandle %1").arg(frameResizeHandle));
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
							m_doc->m_Selection->setGroupRect();
							double gx, gy, gh, gw;
							m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
							m_view->moveGroup(dX, dY, false);
							if (m_doc->SnapGuides)
							{
								double nx = gx;
								double ny = gy;
								m_doc->ApplyGuides(&nx, &ny);
								m_view->moveGroup(nx-gx, ny-gy, false);
								m_doc->m_Selection->setGroupRect();
								m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
								nx = gx+gw;
								ny = gy+gh;
								m_doc->ApplyGuides(&nx, &ny);
								m_view->moveGroup(nx-(gx+gw), ny-(gy+gh), false);
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
									m_view->moveGroup(gx-gxo, gy-gyo, false);
							}
						}
					}
				}
				else
				{
					m_doc->m_Selection->setGroupRect();
					double gx, gy, gh, gw;
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
					m_view->moveGroup(dX, dY, false);
					if (m_doc->SnapGuides)
					{
						double nx = gx;
						double ny = gy;
						m_doc->ApplyGuides(&nx, &ny);
						m_view->moveGroup(nx-gx, ny-gy, false);
						m_doc->m_Selection->setGroupRect();
						m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
						nx = gx+gw;
						ny = gy+gh;
						m_doc->ApplyGuides(&nx, &ny);
						m_view->moveGroup(nx-(gx+gw), ny-(gy+gh), false);
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
							m_view->moveGroup(gx-gxo, gy-gyo, false);
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
					m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					m_doc->adjustCanvas(FPoint(gx,gy), FPoint(gx+gw, gy+gh));
					m_canvas->repaint();
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
				m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				int how = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), QRectF(gx, gy, gw, gh));
//				if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
				if (how >= 0
					&& ((m_doc->appMode == modeNormal) || (m_doc->appMode == modeRotation)))
				{
/*					int how = 0;
//					gx -= m_doc->minCanvasCoordinate.x();
//					gy -= m_doc->minCanvasCoordinate.y();
					QMap<double,int> distance;
					double d1 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
					if (d1 < m_doc->guidesSettings.grabRad)
						distance.insert(d1, 1);
					double d2 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
					if (d2 < m_doc->guidesSettings.grabRad)
						distance.insert(d2, 2);
					double d3 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
					if (d3 < m_doc->guidesSettings.grabRad)
						distance.insert(d3, 3);
					double d4 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
					if (d4 < m_doc->guidesSettings.grabRad)
						distance.insert(d4, 4);
					double d5 = sqrt(pow(((gx+gw/2) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
					if (d5 < m_doc->guidesSettings.grabRad)
						distance.insert(d5, 5);
					double d6 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh/2) * m_canvas->scale()) - m->y(),2));
					if (d6 < m_doc->guidesSettings.grabRad)
						distance.insert(d6, 6);
					double d7 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow(((gy+gh/2) * m_canvas->scale()) - m->y(),2));
					if (d7 < m_doc->guidesSettings.grabRad)
						distance.insert(d7, 7);
					double d8 = sqrt(pow(((gx+gw/2) * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
					if (d8 < m_doc->guidesSettings.grabRad)
						distance.insert(d8, 8);
					QList<int> result = distance.values();
					if (result.count() != 0)
					{
						how = result[0];
						}
						*/
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
				if ((QRegion(p.map(QPolygon(QRect(-3, -3, static_cast<int>(currItem->width()+6), static_cast<int>(currItem->height()+6))))).contains(mpo))
					&& ((m_doc->appMode == modeNormal) || (m_doc->appMode == modeRotation) || (m_doc->appMode == modeEdit)))
				{
					tx = p.mapRect(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
					if ((tx.intersects(mpo)) && (!currItem->locked()))
					{
						if (m_doc->appMode == modeRotation)
							qApp->changeOverrideCursor(QCursor(loadIcon("Rotieren2.png")));
						else
						if (m_doc->appMode == modeEdit)
						{
							if (currItem->asTextFrame())
								qApp->changeOverrideCursor(QCursor(Qt::IBeamCursor));
							if (currItem->asImageFrame())
								qApp->changeOverrideCursor(QCursor(loadIcon("HandC.xpm")));
						}
						else
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
					setResizeCursor(how);
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
		if ((m_doc->guidesSettings.guidesShown) && (m_doc->appMode == modeNormal) && (!m_doc->GuideLock) && (m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y()) != -1) && (!GetItem(&currItem)))
		{
			if (!guideMoveGesture)
				guideMoveGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
			if (guideMoveGesture->mouseHitsGuide(mousePointDoc))
			{
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
			/*double grabRadScale=m_doc->guidesSettings.grabRad / m_canvas->scale();
			if (0 <= m_doc->currentPage()->guides.isMouseOnHorizontal(mousePointDoc.y() + grabRadScale, mousePointDoc.y() - grabRadScale, GuideManagerCore::Standard))
			{
				if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (GyM != -1))
					MoveGY = true;
				if (!m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y())) // ((m->x()/sc) < m_doc->currentPage()->xOffset()- 0*m_doc->minCanvasCoordinate.x()) || ((m->x()/sc) >= m_doc->currentPage()->width()-1+m_doc->currentPage()->xOffset()- 0*m_doc->minCanvasCoordinate.x()))
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				else
					qApp->changeOverrideCursor(QCursor(Qt::SplitVCursor));
				return;
			}
			if (0 <= m_doc->currentPage()->guides.isMouseOnVertical(mousePointDoc.x() + grabRadScale, mousePointDoc.x() - grabRadScale, GuideManagerCore::Standard))
			{
				if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (GxM != -1))
					MoveGX = true;
				if (!m_doc->OnPage(mousePointDoc.x(), mousePointDoc.y())) // ((m->y()/sc) < m_doc->currentPage()->yOffset()- 0*m_doc->minCanvasCoordinate.x()) || ((m->y()/sc) >= m_doc->currentPage()->height()-1+m_doc->currentPage()->yOffset()- 0*m_doc->minCanvasCoordinate.y()))
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				else
					qApp->changeOverrideCursor(QCursor(Qt::SplitHCursor));
				return;
			}*/	
/*
				Guides::iterator it;
			Guides tmpGuides = m_doc->currentPage()->guides.horizontals(GuideManagerCore::Standard);
			for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it)
			{
				if (((*it)+m_doc->currentPage()->yOffset()- 0*m_doc->minCanvasCoordinate.y() < ((m->y()+m_doc->guidesSettings.grabRad) / sc)) &&
								   ((*it)+m_doc->currentPage()->yOffset()- 0*m_doc->minCanvasCoordinate.y() > ((m->y()-m_doc->guidesSettings.grabRad) / sc)))
				{
				}
			}
//			qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
			tmpGuides = m_doc->currentPage()->guides.verticals(GuideManagerCore::Standard);
			for (it = tmpGuides.begin(); it!= tmpGuides.end(); ++it)
			{
				if (((*it)+m_doc->currentPage()->xOffset()- 0*m_doc->minCanvasCoordinate.x() < ((m->x()+m_doc->guidesSettings.grabRad) / sc)) &&
								   ((*it)+m_doc->currentPage()->xOffset()- 0*m_doc->minCanvasCoordinate.x() > ((m->x()-m_doc->guidesSettings.grabRad) / sc)))
				{
				}
			}
*/
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		}
	}
}

void LegacyMode::mousePressEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
	bool inText;
	int z;
	double Rxp = 0;
	double Ryp = 0;
	double Rxpd = 0;
	double Rypd = 0;
	PageItem *currItem;
	PageItem *bb;
	QPainter p;
	m_canvas->PaintSizeRect(QRect());
	FPoint npf, npf2;
	QRect tx;
	QMatrix pm;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	MoveGX = MoveGY = false;
	inItemCreation = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
	Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
	QRect mpo(m->x()-m_doc->guidesSettings.grabRad, m->y()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
	if (m_doc->appMode != modeEdit)
	{
		Rxp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).x();
		Rxpd = Mxp - Rxp;
		Mxp = qRound(Rxp);
		Ryp = m_doc->ApplyGridF(FPoint(Mxp, Myp)).y();
		Rypd = Myp - Ryp;
		Myp = qRound(Ryp);
	}
	else
	{
		Ryp = Myp;
		Rxp = Mxp;
	}
	SeRx = Mxp;
	SeRy = Myp;
	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		m_view->DrawNew();
		return;
	}
	switch (m_doc->appMode)
	{
		case modeNormal:
			Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale());
			Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale());
			SeRx = Mxp;
			SeRy = Myp;
			if (GetItem(&currItem))
			{
				if (!resizeGesture)
					resizeGesture = new ResizeGesture(this);
				resizeGesture->mousePressEvent(m);
				if (resizeGesture->frameHandle() > 0)
				{
					m_view->startGesture(resizeGesture);
					return;
				}
#if 1				
				if (m_doc->m_Selection->isMultipleSelection())
				{
					m_canvas->PaintSizeRect(QRect());
					double gx, gy, gh, gw;
					bool shiftSel = true;
					m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					dragConstrainInitPtX = qRound(gx);
					dragConstrainInitPtY = qRound(gy);
					frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), QRectF(gx, gy, gw, gh));
/*					mpo = QRect(qRound(m->x() / m_canvas->scale()) - m_doc->guidesSettings.grabRad, qRound(m->y() / m_canvas->scale()) - m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//					mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x()), qRound(m_doc->minCanvasCoordinate.y()));
					if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					      && (m->modifiers() != (Qt::ControlModifier | Qt::AltModifier)) && (m->modifiers() != Qt::ShiftModifier))
					{
						frameResizeHandle = 0;
						QMap<double,int> distance;
//						gx -= m_doc->minCanvasCoordinate.x();
//						gy -= m_doc->minCanvasCoordinate.y();
						double d1 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
						if (d1 < m_doc->guidesSettings.grabRad)
							distance.insert(d1, 1);
						double d2 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
						if (d2 < m_doc->guidesSettings.grabRad)
							distance.insert(d2, 2);
						double d3 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
						if (d3 < m_doc->guidesSettings.grabRad)
							distance.insert(d3, 3);
						double d4 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
						if (d4 < m_doc->guidesSettings.grabRad)
							distance.insert(d4, 4);
						double d5 = sqrt(pow(((gx+gw/2) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
						if (d5 < m_doc->guidesSettings.grabRad)
							distance.insert(d5, 5);
						double d6 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh/2) * m_canvas->scale()) - m->y(),2));
						if (d6 < m_doc->guidesSettings.grabRad)
							distance.insert(d6, 6);
						double d7 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow(((gy+gh/2) * m_canvas->scale()) - m->y(),2));
						if (d7 < m_doc->guidesSettings.grabRad)
							distance.insert(d7, 7);
						double d8 = sqrt(pow(((gx+gw/2) * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
						if (d8 < m_doc->guidesSettings.grabRad)
							distance.insert(d8, 8);
						QList<int> result = distance.values();
						if (result.count() != 0)
						{
							frameResizeHandle = result[0];
							setResizeCursor(frameResizeHandle);
						}
						*/
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
						shiftSel = SeleItem(m);
					}
					if (((m_doc->m_Selection->count() == 0) || (!shiftSel)) && (m->modifiers() == Qt::ShiftModifier))
					{
						shiftSelItems = true;
						Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
						Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
						SeRx = Mxp;
						SeRy = Myp;
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
					if (frameResizeHandle < 0 || m->modifiers() != Qt::NoModifier)
					{
						SeleItem(m); //Where we send the mouse press event to select an item
						if (GetItem(&currItem))
							frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem);
						else
							frameResizeHandle = 0;
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
			break;
		case modeDrawShapes:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			switch (m_doc->SubMode)
			{
			case 0:
				if (m->modifiers() == Qt::ShiftModifier)
				{
					z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					m_doc->Items->at(z)->FrameType = 0;
					SetupDrawNoResize(z);
				}
				else
				{
					m_doc->ApplyGuides(&Rxp, &Ryp);
					z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					m_doc->Items->at(z)->FrameType = 0;
					SetupDraw(z);
				}
				break;
			case 1:
				if (m->modifiers() == Qt::ShiftModifier)
				{
					z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					m_doc->Items->at(z)->FrameType = 1;
					SetupDrawNoResize(z);
				}
				else
				{
					m_doc->ApplyGuides(&Rxp, &Ryp);
					z = m_doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					m_doc->Items->at(z)->FrameType = 1;
					SetupDraw(z);
				}
				break;
			default:
				if (m->modifiers() == Qt::ShiftModifier)
				{
					z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					m_doc->Items->at(z)->SetFrameShape(m_doc->ValCount, m_doc->ShapeValues);
					m_doc->setRedrawBounding(m_doc->Items->at(z));
					m_doc->Items->at(z)->FrameType = m_doc->SubMode+2;
					SetupDrawNoResize(z);
				}
				else
				{
					m_doc->ApplyGuides(&Rxp, &Ryp);
					z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					m_doc->Items->at(z)->SetFrameShape(m_doc->ValCount, m_doc->ShapeValues);
					m_doc->setRedrawBounding(m_doc->Items->at(z));
					m_doc->Items->at(z)->FrameType = m_doc->SubMode+2;
					SetupDraw(z);
				}
				break;
			}
			break;
		case modeDrawLatex:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->modifiers() == Qt::ShiftModifier)
			{
				z = m_doc->itemAddArea(PageItem::LatexFrame, PageItem::Unspecified, Rxp, Ryp, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::LatexFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeDrawImage:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->modifiers() == Qt::ShiftModifier)
			{
				z = m_doc->itemAddArea(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, 1, m_doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeDrawText:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->modifiers() == Qt::ShiftModifier)
			{
				z = m_doc->itemAddArea(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}	
			else
			{
				m_doc->ApplyGuides(&Rxp, &Ryp);
				z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeMagnifier:
			m_canvas->m_viewMode.m_MouseButtonPressed = true;
			if ((m->modifiers() == Qt::ShiftModifier) || (m->button() == Qt::RightButton))
			{
				m_view->Magnify = false;
				qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
			}
			else
			{
				m_view->Magnify = true;
				qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
			}
			Mxp = mousePointDoc.x(); //m->globalPos().x();
			Myp = mousePointDoc.y(); //m->globalPos().y();
			SeRx = Mxp;
			SeRy = Myp;
			m_view->redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
			break;
		case modeEdit:
			{
				frameResizeHandle = 0;
				m_canvas->m_viewMode.operItemResizeInEditMode = false;
				int oldP=0;
				if (GetItem(&currItem))
				{
//					m_view->slotDoCurs(false);
					if (!currItem->locked())
					{
						frameResizeHandle = m_canvas->frameHitTest(QPointF(mousePointDoc.x(),mousePointDoc.y()), currItem); // HandleSizer(currItem, mpo, m);
						if (frameResizeHandle < 0)
						{
							SeleItem(m);
							m_view->requestMode(modeNormal);
							return;
						}
						else if (frameResizeHandle > 0)
						{
							m_canvas->m_viewMode.operItemResizeInEditMode = true;
//							m_view->slotDoCurs(true);
							return;
						}
					}
					oldP = currItem->CPos;
				}
				//CB Where we set the cursor for a click in text frame
				inText = m_view->slotSetCurs(m->x(), m->y());
				//CB If we clicked outside a text frame to go out of edit mode and deselect the frame
				if (!inText)
				{
					//<<CB Redraw subsequent frames after being in edit mode
					//No intelligence, may slow things down when drawing unnecessarily
					PageItem *nextItem=currItem;
					while (nextItem != 0)
					{
						if (nextItem->nextInChain() != 0)
						{
							nextItem = nextItem->nextInChain();
							nextItem->update();
						}
						else
							break;
					}
					//>>
					m_view->Deselect(true);
//					m_view->slotDoCurs(true);
					m_view->requestMode(modeNormal);
					return;
				}
				//<<CB Add in shift select to text frames
				if (m->modifiers() & Qt::ShiftModifier)
				{
					int dir=1;
					if (oldCp>currItem->CPos)
						dir=-1;
					if (currItem->asTextFrame())
						currItem->asTextFrame()->ExpandSel(dir, oldP);
					oldCp = oldP;
				}
				else //>>CB
					oldCp = currItem->CPos;
				currItem = m_doc->m_Selection->itemAt(0);
//				m_view->slotDoCurs(true);
				if ((!inText) && ((currItem->asTextFrame()) || (currItem->asImageFrame())))
				{
					m_view->Deselect(true);
					if (SeleItem(m))
					{
						currItem = m_doc->m_Selection->itemAt(0);
						if ((currItem->asTextFrame()) || (currItem->asImageFrame()))
							m_view->requestMode(modeEdit);
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
				else
				{
					if ((m->button() == Qt::MidButton) && (currItem->asTextFrame()))
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
							if (m_ScMW->Buffer2.startsWith("<SCRIBUSTEXT"))
								m_ScMW->slotEditPaste();
						}
						currItem->update();
					}
					if (currItem->asImageFrame() && !tx.contains(m->x(), m->y()))
					{
						m_view->Deselect(true);
						if (SeleItem(m))
						{
							currItem = m_doc->m_Selection->itemAt(0);
							if ((currItem->asTextFrame()) || (currItem->asImageFrame()))
								m_view->requestMode(modeEdit);
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
			break;
		case modeDrawLine:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			m_doc->ApplyGuides(&Rxp, &Ryp);
			z = m_doc->itemAdd(PageItem::Line, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, Rypd, m_doc->toolSettings.dWidthLine, CommonStrings::None, m_doc->toolSettings.dPenLine, !m_canvas->m_viewMode.m_MouseButtonPressed);
			currItem = m_doc->Items->at(z);
			qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
			m_doc->m_Selection->clear();
			m_doc->m_Selection->addItem(currItem);
			m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
			m_canvas->m_viewMode.operItemMoving = true;
			inItemCreation = true;
			m_canvas->m_viewMode.operItemResizing = false;
			m_canvas->setRenderModeFillBuffer();
			Dxp = mousePointDoc.x(); //m->x();
			Dyp = mousePointDoc.y(); //m->y();
			Mxp = mousePointDoc.x(); //m->x();
			Myp = mousePointDoc.y(); //m->y();
			break;
		case modeRotation:
			if (m->button() != Qt::LeftButton)
				break;
			if (GetItem(&currItem))
			{
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
//					RCenter = FPoint(currItem->xPos()+currItem->width()/2, currItem->yPos()+currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, true);
					m_view->RCenter = FPoint(currItem->width()/2, currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, false);
//					if (!currItem->asLine())
//					{
						if (QRegion(mat.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo))
						{
							if (mat.mapRect(QRect(0, 0, 6, 6)).intersects(mpo))
							{
//								RCenter = FPoint(currItem->xPos()+currItem->width(), currItem->yPos()+currItem->height(), 0, 0, currItem->rotation(), 1, 1, true);
								m_view->RCenter = FPoint(currItem->width(), currItem->height(), 0, 0, currItem->rotation(), 1, 1, false);
								m_doc->RotMode = 4;
							}
							else if (mat.mapRect(QRect(static_cast<int>(currItem->width())-6, 0, 6, 6)).intersects(mpo))
							{
//								RCenter = FPoint(currItem->xPos(), currItem->yPos()+currItem->height(), 0, 0, currItem->rotation(), 1, 1, true);
								m_view->RCenter = FPoint(0, currItem->height(), 0, 0, currItem->rotation(), 1, 1, false);
								m_doc->RotMode = 3;
							}
							else if (mat.mapRect(QRect(static_cast<int>(currItem->width())-6, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
							{
//								RCenter = FPoint(currItem->xPos(), currItem->yPos());
								m_view->RCenter = FPoint(0, 0);
								m_doc->RotMode = 0;
							}
							else if (mat.mapRect(QRect(0, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
							{
//								RCenter = FPoint(currItem->xPos()+currItem->width(), currItem->yPos(), 0, 0, currItem->rotation(), 1, 1, true);
								m_view->RCenter = FPoint(currItem->width(), 0, 0, 0, currItem->rotation(), 1, 1, false);
								m_doc->RotMode = 1;
							}
						}
//					}
					m_view->RCenter = FPoint(currItem->xPos()+m_view->RCenter.x(), currItem->yPos()+ m_view->RCenter.y());
					m_view->oldW = xy2Deg(mousePointDoc.x() - m_view->RCenter.x(), mousePointDoc.y() - m_view->RCenter.y());
				}
			}
			break;
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
//					m_view->updateContents();
// link calls PageItem::update					emit DocChanged();
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
//				m_view->updateContents();
			}
			break;
		case modeDrawRegularPolygon:
			{
				if (m->button() != Qt::LeftButton)
					break;
				selectPage(m);
				if (m->modifiers() == Qt::ShiftModifier)
					z = m_doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
				else
				{
					m_doc->ApplyGuides(&Rxp, &Ryp);
					z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, m_doc->toolSettings.dBrush, m_doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
				}
				currItem = m_doc->Items->at(z);
				FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), m_doc->toolSettings.polyC, m_doc->toolSettings.polyS, m_doc->toolSettings.polyF, m_doc->toolSettings.polyR);
				FPoint np(cli.point(0));
				currItem->PoLine.resize(2);
				currItem->PoLine.setPoint(0, np);
				currItem->PoLine.setPoint(1, np);
				for (uint ax = 1; ax < cli.size(); ++ax)
				{
					np = FPoint(cli.point(ax));
					currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
				}
				np = FPoint(cli.point(0));
				currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
				currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
				qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
				m_doc->m_Selection->clear();
				m_doc->m_Selection->addItem(currItem);
				m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				inItemCreation = true;
				m_canvas->m_viewMode.operItemResizing = true;
				if (m->modifiers() == Qt::ShiftModifier)
				{
					m_view->requestMode(modeNormal);
// itemAdd calls PageItem::update					emit DocChanged();
					currItem->Sizing =  currItem->asLine() ? false : true;
					inItemCreation = false;
//					m_view->updateContents();
				}
				else
				{
					m_canvas->m_viewMode.operItemMoving = true;
					m_canvas->setRenderModeFillBuffer();
				}
				break;
			}
		case modeDrawBezierLine:
			if (m->button() == Qt::RightButton)
				break;
			if (FirstPoly)
			{
				selectPage(m);
				z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenLine, !m_canvas->m_viewMode.m_MouseButtonPressed);
				currItem = m_doc->Items->at(z);
				m_doc->m_Selection->clear();
				m_doc->m_Selection->addItem(currItem);
				qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
				m_canvas->setRenderModeFillBuffer();
			}
			currItem = m_doc->m_Selection->itemAt(0);
//			pm.translate(-m_doc->minCanvasCoordinate.x()*m_canvas->scale(), -m_doc->minCanvasCoordinate.y()*m_canvas->scale());
			m_canvas->Transform(currItem, pm);
			npf = FPoint(m->pos() * pm.inverted());
			npf = m_doc->ApplyGridF(npf);
			currItem->PoLine.addPoint(npf);
			npf2 = getMinClipF(&currItem->PoLine);
			if (npf2.x() < 0)
			{
				currItem->PoLine.translate(-npf2.x(), 0);
				m_doc->MoveItem(npf2.x(), 0, currItem);
			}
			if (npf2.y() < 0)
			{
				currItem->PoLine.translate(0, -npf2.y());
				m_doc->MoveItem(0, npf2.y(), currItem);
			}
			m_doc->SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false, false);
			currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2, 1.0)));
			m_canvas->newRedrawPolygon();
			break;
		case modeInsertPDFButton:
		case modeInsertPDFTextfield:
		case modeInsertPDFCheckbox:
		case modeInsertPDFCombobox:
		case modeInsertPDFListbox:
		case modeInsertPDFTextAnnotation:
		case modeInsertPDFLinkAnnotation:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			m_doc->ApplyGuides(&Rxp, &Ryp);
			z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, !m_canvas->m_viewMode.m_MouseButtonPressed);
			currItem = m_doc->Items->at(z);
			currItem->setIsAnnotation(true);
			currItem->AutoName = false;
			switch (m_doc->appMode)
			{
			case modeInsertPDFButton:
				currItem->annotation().setType(2);
				currItem->annotation().setFlag(65536);
				break;
			case modeInsertPDFTextfield:
				currItem->annotation().setType(3);
				break;
			case modeInsertPDFCheckbox:
				currItem->annotation().setType(4);
				break;
			case modeInsertPDFCombobox:
				currItem->annotation().setType(5);
				currItem->annotation().setFlag(131072);
				break;
			case modeInsertPDFListbox:
				currItem->annotation().setType(6);
				break;
			case modeInsertPDFTextAnnotation:
				currItem->annotation().setType(10);
				break;
			case modeInsertPDFLinkAnnotation:
				currItem->annotation().setType(11);
				currItem->annotation().setZiel(m_doc->currentPage()->pageNr());
				currItem->annotation().setAction("0 0");
				currItem->setTextFlowMode(PageItem::TextFlowDisabled);
				break;
			}
			SetupDraw(z);
			break;
		case modeDrawFreehandLine:
			if (m->button() != Qt::LeftButton)
				break;
			RecordP.resize(0);
			m_view->Deselect(false);
			Mxp = mousePointDoc.x(); //qRound(m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x());
			Myp = mousePointDoc.y(); //qRound(m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y());
			SeRx = Mxp;
			SeRy = Myp;
			m_canvas->setRenderModeFillBuffer();
			break;
		case modeDrawTable:
			if (m->button() != Qt::LeftButton)
				break;
			m_view->Deselect(false);
//			Mxp = qRound(m->x()/m_canvas->scale() + m_doc->minCanvasCoordinate.x());
//			Myp = qRound(m->y()/m_canvas->scale() + m_doc->minCanvasCoordinate.y());
			Rxp = mousePointDoc.x(); //m->x()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x();
			Ryp = mousePointDoc.y(); //m->y()/m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y();
			npf = m_doc->ApplyGridF(FPoint(Rxp, Ryp));
			Rxp = npf.x();
			Ryp = npf.y();
			m_doc->ApplyGuides(&Rxp, &Ryp);
			GyM = -1;
			GxM = -1;
			Mxp = qRound(Rxp);
			Myp = qRound(Ryp);
			SeRx = Mxp;
			SeRy = Myp;
			Dxp = m->globalPos().x();
			Dyp = m->globalPos().y();
			m_view->redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
			m_view->redrawMarker->show();
			break;
		case modePanning:
			break;
		case modeMeasurementTool:
			m_canvas->setRenderModeFillBuffer();
		case modeEditGradientVectors:
			if (m->button() != Qt::LeftButton)
				break;
			m_canvas->m_viewMode.m_MouseButtonPressed = true;
			qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
			Dxp = mousePointDoc.x(); //m->x();
			Dyp = mousePointDoc.y(); //m->y();
			Mxp = mousePointDoc.x(); //m->x();
			Myp = mousePointDoc.y(); //m->y();
			break;
		case modeCopyProperties:
			if (m->button() != Qt::LeftButton)
				break;
			SeleItem(m);
			if (GetItem(&currItem))
			{
				currItem->ColGap = m_doc->ElemToLink->ColGap;
				currItem->Cols = m_doc->ElemToLink->Cols;
				currItem->BottomLine = m_doc->ElemToLink->BottomLine;
				currItem->TopLine = m_doc->ElemToLink->TopLine;
				currItem->LeftLine = m_doc->ElemToLink->LeftLine;
				currItem->RightLine = m_doc->ElemToLink->RightLine;
				currItem->setTextToFrameDist(m_doc->ElemToLink->textToFrameDistLeft(), m_doc->ElemToLink->textToFrameDistRight(), m_doc->ElemToLink->textToFrameDistTop(), m_doc->ElemToLink->textToFrameDistBottom());
				currItem->setLineStyle(m_doc->ElemToLink->lineStyle());
				currItem->setLineWidth(m_doc->ElemToLink->lineWidth());
				currItem->setLineTransparency(m_doc->ElemToLink->lineTransparency());
				currItem->setLineShade(m_doc->ElemToLink->lineShade());
				currItem->setLineColor(m_doc->ElemToLink->lineColor());
				currItem->setLineEnd(m_doc->ElemToLink->lineEnd());
				currItem->setLineJoin(m_doc->ElemToLink->lineJoin());
				currItem->setCustomLineStyle(m_doc->ElemToLink->customLineStyle());
				currItem->setEndArrowIndex(m_doc->ElemToLink->endArrowIndex());
				currItem->setStartArrowIndex(m_doc->ElemToLink->startArrowIndex());
				currItem->setFillColor(m_doc->ElemToLink->fillColor());
				currItem->setFillShade(m_doc->ElemToLink->fillShade());
				currItem->setFillTransparency(m_doc->ElemToLink->fillTransparency());
				m_doc->ElemToLink = currItem;
				currItem->update();
//				emit DocChanged();
//				m_view->updateContents();
			}
			else
			{
				m_doc->ElemToLink = NULL;
				m_view->requestMode(submodePaintingDone);
			}
			break;
	}
}



void LegacyMode::mouseReleaseEvent(QMouseEvent *m)
{
// 	const double mouseX = m->globalX();
// 	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
// 	QMenu* pmen3 = NULL;
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->stopDragTimer();
	// will be executed later
	m_canvas->update();
/*	if ((m_doc->appMode == modeNormal) && m_doc->guidesSettings.guidesShown)
	{
		bool foundGuide = false;
		double nx = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
		double ny = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
		double grabRadScale=m_doc->guidesSettings.grabRad / m_canvas->scale();
		if (0 <= m_doc->currentPage()->guides.isMouseOnHorizontal(ny + grabRadScale, ny - grabRadScale, GuideManagerCore::Standard)
			|| 0 <= m_doc->currentPage()->guides.isMouseOnVertical(nx + grabRadScale, nx - grabRadScale, GuideManagerCore::Standard))
			foundGuide = true;
		if ((foundGuide) && (m->button() == Qt::RightButton) && (!GetItem(&currItem)))
		{
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			MoveGY = false;
			MoveGX = false;
			return;
		}
		if (MoveGY)
		{
			m_view->SetYGuide(m, GyM);
			MoveGY = false;
			m_view->redrawMarker->hide();
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			m_view->updateContents();
			GyM = -1;
			return;
		}
		if (MoveGX)
		{
			m_view->SetXGuide(m, GxM);
			MoveGX = false;
			m_view->redrawMarker->hide();
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			m_view->updateContents();
			GxM = -1;
			return;
		}
	}*/
	if (m_doc->appMode == modeEditGradientVectors)
		return;
	if (m_doc->appMode == modeCopyProperties)
		return;
	if (m_doc->appMode == modeMeasurementTool)
	{
		m_canvas->newRedrawPolygon();
		m_view->updateCanvas(QRectF(QPointF(Dxp, Dyp), QPointF(Mxp, Mxp)).normalized().adjusted(-10, -10, 20, 20));
//		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		//emit PaintingDone();
		return;
	}
	if (m_doc->appMode == modePanning)
	{
		if ((m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
		{
			m_ScMW->setAppMode(modeNormal);
		}
		return;
	}
	if (m_doc->appMode == modeDrawTable)
	{
		if ((m_doc->m_Selection->count() == 0) && (m_view->HaveSelRect) && (!m_view->MidButt))
		{
			QRect AreaR = QRect(static_cast<int>(Mxp), static_cast<int>(Myp), static_cast<int>(SeRx-Mxp), static_cast<int>(SeRy-Myp)).normalized();
			m_view->HaveSelRect = false;
			double Tx, Ty, Tw, Th;
			FPoint np2 = m_doc->ApplyGridF(FPoint(Mxp, Myp));
			Tx = np2.x();
			Ty = np2.y();
			m_doc->ApplyGuides(&Tx, &Ty);
			Mxp = qRound(Tx);
			Myp = qRound(Ty);
			np2 = m_doc->ApplyGridF(FPoint(SeRx, SeRy));
			Tw = np2.x();
			Th = np2.y();
			m_doc->ApplyGuides(&Tw, &Th);
			SeRx = qRound(Tw);
			SeRy = qRound(Th);
			Tw = Tw - Tx;
			Th = Th - Ty;
			int z;
			int Cols, Rows;
			double deltaX, deltaY, offX, offY;
			if ((Th < 6) || (Tw < 6))
			{
				m_view->redrawMarker->hide();
				m_view->requestMode(submodePaintingDone);
				return;
			}
			InsertTable *dia = new InsertTable(m_view, static_cast<int>(Th / 6), static_cast<int>(Tw / 6));
			if (!dia->exec())
			{
				m_view->redrawMarker->hide();
				m_view->requestMode(submodePaintingDone);
				delete dia;
				dia=NULL;
				return;
			}
			m_view->redrawMarker->hide();
			Cols = dia->Cols->value();
			Rows = dia->Rows->value();
			delete dia;
			dia=NULL;
			deltaX = Tw / Cols;
			deltaY = Th / Rows;
			offX = 0.0;
			offY = 0.0;
			m_doc->m_Selection->clear();
			if (UndoManager::undoEnabled())
				m_view->undoManager->beginTransaction(m_doc->currentPage()->getUName(),
											  Um::ITable, Um::CreateTable,
											  QString(Um::RowsCols).arg(Rows).arg(Cols),
											  Um::ICreate);
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					//z = PaintText(Tx + offX, Ty + offY, deltaX, deltaY, m_doc->toolSettings.dWidth, m_doc->toolSettings.dPenText);
					z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Tx + offX, Ty + offY, deltaX, deltaY, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenText, !m_canvas->m_viewMode.m_MouseButtonPressed);
					currItem = m_doc->Items->at(z);
					currItem->isTableItem = true;
					//currItem->setTextFlowsAroundFrame(true);
					//currItem->setTextFlowUsesBoundingBox(true);
					currItem->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
					m_doc->m_Selection->addItem(currItem);
					offX += deltaX;
				}
				offY += deltaY;
				offX = 0.0;
			}
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					currItem = m_doc->m_Selection->itemAt((rc * Cols) + cc);
					if (rc == 0)
						currItem->TopLink = 0;
					else
						currItem->TopLink = m_doc->m_Selection->itemAt(((rc-1)*Cols)+cc);
					if (rc == Rows-1)
						currItem->BottomLink = 0;
					else
						currItem->BottomLink = m_doc->m_Selection->itemAt(((rc+1)*Cols)+cc);
					if (cc == 0)
						currItem->LeftLink = 0;
					else
						currItem->LeftLink = m_doc->m_Selection->itemAt((rc*Cols)+cc-1);
					if (cc == Cols-1)
						currItem->RightLink = 0;
					else
						currItem->RightLink = m_doc->m_Selection->itemAt((rc*Cols)+cc+1);
				}
			}
			m_ScMW->GroupObj();
			if (UndoManager::undoEnabled())
				m_view->undoManager->commit();
			m_doc->changed();
		}
		if (!PrefsManager::instance()->appPrefs.stickyTools)
		{
			m_view->requestMode(modeNormal);
//			m_view->requestMode(submodePaintingDone);
		}
		else
			m_view->requestMode(m_doc->appMode);
// itemAdd calls PageItem::update		emit DocChanged();
//		m_view->updateContents();
		return;
	}
	if (m_doc->appMode == modeDrawFreehandLine)
	{
		if (RecordP.size() > 1)
		{
			uint z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, Mxp, Myp, 1, 1, m_doc->toolSettings.dWidth, CommonStrings::None, m_doc->toolSettings.dPenLine, !m_canvas->m_viewMode.m_MouseButtonPressed);
			currItem = m_doc->Items->at(z);
			currItem->PoLine.resize(0);
			currItem->PoLine.addPoint(RecordP.point(0));
			currItem->PoLine.addPoint(RecordP.point(0));
			for (uint px = 1; px < RecordP.size()-1; ++px)
			{
				currItem->PoLine.addPoint(RecordP.point(px));
				currItem->PoLine.addPoint(RecordP.point(px));
				currItem->PoLine.addPoint(RecordP.point(px));
				currItem->PoLine.addPoint(RecordP.point(px));
			}
			currItem->PoLine.addPoint(RecordP.point(RecordP.size()-1));
			currItem->PoLine.addPoint(RecordP.point(RecordP.size()-1));
			FPoint tp2(getMinClipF(&currItem->PoLine));
			currItem->setXYPos(tp2.x(), tp2.y(), true);
			currItem->PoLine.translate(-tp2.x(), -tp2.y());
			FPoint tp(getMaxClipF(&currItem->PoLine));
			m_doc->SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
			m_doc->AdjustItemSize(currItem);
			m_doc->m_Selection->clear();
			m_doc->m_Selection->addItem(currItem);
			currItem->ClipEdited = true;
			currItem->FrameType = 3;
			currItem->OwnPage = m_doc->OnPage(currItem);
			m_canvas->m_viewMode.operItemMoving = false;
			m_canvas->m_viewMode.operItemResizing = false;
			inItemCreation = false;
			m_doc->changed();
//			m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()).adjusted(-10, -10, 20, 20));
		}
		if (!PrefsManager::instance()->appPrefs.stickyTools)
		{
			m_view->requestMode(modeNormal);
//			m_view->requestMode(submodePaintingDone);
		}
		else
			m_view->requestMode(m_doc->appMode);
// itemAdd calls PageItem::update		emit DocChanged();
		return;
	}
	
	if ((!GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!m_doc->DragP) 
		&& (m_doc->appMode == modeNormal))
	{
#if 0
		QMenu *pmen = new QMenu();
		if ((m_ScMW->Buffer2.startsWith("<SCRIBUSELEM")) || (m_ScMW->Buffer2.contains("<SCRIBUSFRAGMENT")) || (m_ScMW->scrapbookPalette->tempBView->objectMap.count() > 0))
		{
			Mxp = mousePointDoc.x(); //m->x();
			Myp = mousePointDoc.y(); //m->y();
			if ((m_ScMW->Buffer2.startsWith("<SCRIBUSELEM")) || (m_ScMW->Buffer2.contains("<SCRIBUSFRAGMENT")))
				pmen->addAction( ScribusView::tr("&Paste") , m_view, SLOT(PasteToPage()));
			if (m_ScMW->scrapbookPalette->tempBView->objectMap.count() > 0)
			{
				pmen3 = new QMenu();
				QMap<QString,BibView::Elem>::Iterator it;
				it = m_ScMW->scrapbookPalette->tempBView->objectMap.end();
				it--;
				for (int m = 0; m < m_ScMW->scrapbookPalette->tempBView->objectMap.count(); ++m)
				{
					QString strippedName = it.key();
					QPixmap pm = it.value().Preview;
// Qt4					pmen3->insertItem(pm, strippedName);
					pmen3->addAction(pm, strippedName);
					it--;
				}
				QObject::connect(pmen3, SIGNAL(activated(int)), m_view, SLOT(PasteRecentToPage(int)));
				QAction *act = pmen->addMenu(pmen3);
				act->setText( ScribusView::tr("Paste Recent"));
			}
			pmen->addSeparator();
		}
		m_view->setObjectUndoMode();
		pmen->addAction(m_ScMW->scrActions["editUndoAction"]);
		pmen->addAction(m_ScMW->scrActions["editRedoAction"]);
		pmen->addSeparator();
		pmen->addAction(m_ScMW->scrActions["viewShowMargins"]);
		pmen->addAction(m_ScMW->scrActions["viewShowFrames"]);
		pmen->addAction(m_ScMW->scrActions["viewShowLayerMarkers"]);
		pmen->addAction(m_ScMW->scrActions["viewShowImages"]);
		pmen->addAction(m_ScMW->scrActions["viewShowGrid"]);
		pmen->addAction(m_ScMW->scrActions["viewShowGuides"]);
		pmen->addAction(m_ScMW->scrActions["viewShowBaseline"]);
		pmen->addAction(m_ScMW->scrActions["viewShowTextChain"]);
		pmen->addAction(m_ScMW->scrActions["viewRulerMode"]);
		pmen->addSeparator();
		pmen->addAction(m_ScMW->scrActions["viewSnapToGrid"]);
		pmen->addAction(m_ScMW->scrActions["viewSnapToGuides"]);
		int pgNum = -1;
		int docPageCount = static_cast<int>(m_doc->Pages->count() - 1);
		double bleedRight = 0.0;
		double bleedLeft = 0.0;
		double bleedBottom = 0.0;
		double bleedTop = 0.0;
		bool drawBleed = false;
		int x2 = static_cast<int>(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		int y2 = static_cast<int>(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		if (((m_doc->bleeds.Bottom != 0.0) || (m_doc->bleeds.Top != 0.0) || (m_doc->bleeds.Left != 0.0) || (m_doc->bleeds.Right != 0.0)) && (m_doc->guidesSettings.showBleed))
			drawBleed = true;
		for (int a = docPageCount; a > -1; a--)
		{
			if (drawBleed)
				m_doc->getBleeds(a, &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
			int x = static_cast<int>(m_doc->Pages->at(a)->xOffset() - bleedLeft);
			int y = static_cast<int>(m_doc->Pages->at(a)->yOffset() - bleedTop);
			int w = static_cast<int>(m_doc->Pages->at(a)->width() + bleedLeft + bleedRight);
			int h = static_cast<int>(m_doc->Pages->at(a)->height() + bleedBottom + bleedTop);
			if (QRect(x, y, w, h).contains(x2, y2))
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
						if (QRect(xn, yn, wn, hn).contains(x2, y2))
						{
							pgNum = static_cast<int>(a2);
							break;
						}
					}
				}
				break;
			}
		}
		if (pgNum != -1)
		{
			pmen->addSeparator();
			pmen->addAction(m_ScMW->scrActions["pageApplyMasterPage"]);
			pmen->addAction(m_ScMW->scrActions["pageManageGuides"]);
			pmen->addAction(m_ScMW->scrActions["pageManageMargins"]);
			pmen->addSeparator();
			pmen->addAction(m_ScMW->scrActions["pageDelete"]);
		}
		pmen->exec(QCursor::pos());
		m_view->setGlobalUndoMode();
		delete pmen;
		pmen=NULL;
		if (m_ScMW->scrapbookPalette->tempBView->objectMap.count() > 0)
		{
			delete pmen3;
			pmen3=NULL;
		}
		return;
#endif
		createContextMenu(NULL, mousePointDoc.x(), mousePointDoc.y());
		return;
	}
	if ((m_doc->appMode != modeMagnifier) && (m_doc->appMode != modeDrawBezierLine))
	{
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
//				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				m_view->requestMode(submodePaintingDone);
				return;
			}
		}
		if (inItemCreation)
		{
			currItem = m_doc->m_Selection->itemAt(0);
			double itemX = 0.0;
			double itemY = 0.0;
			if (m_doc->appMode == modeDrawLine)
			{
				QMatrix p;
				m_canvas->Transform(currItem, p);
				QPoint np = m->pos() * p.inverted();
//				np += QPoint(qRound(m_doc->minCanvasCoordinate.x()), qRound(m_doc->minCanvasCoordinate.y()));
				np = m_doc->ApplyGrid(np);
				itemX = sqrt(pow(np.x(),2.0)+pow(np.y(),2.0));
				itemY = 1.0;
			}
			else if (m_doc->appMode == modeDrawRegularPolygon)
			{
				FPoint np1(mousePointDoc); //m_view->translateToDoc(m->x(), m->y()));
				np1 = m_doc->ApplyGridF(np1);
				itemX = fabs(np1.x() - currItem->xPos());
				itemY = fabs(np1.y() - currItem->yPos());
			}
			else
			{
				itemX = fabs(currItem->width());
				itemY = fabs(currItem->height());
			}
			if ((!m_view->moveTimerElapsed()) || ((itemX < 2.0) && (itemY < 2.0)) || ((m_doc->appMode == modeDrawLine) && (itemX < 2.0)))
			{
				int lmode = 0;
				if (m_doc->appMode == modeDrawLine)
					lmode = 1;
				PrefsContext* sizes = PrefsManager::instance()->prefsFile->getContext("ObjectSize");
				double xSize, ySize;
				int originPoint;
				if (lmode == 0)
				{
					xSize = sizes->getDouble("defWidth", 100.0);
					ySize = sizes->getDouble("defHeight", 100.0);
					originPoint = sizes->getInt("Origin", 0);
				}
				else
				{
					xSize = sizes->getDouble("defLength", 100.0);
					ySize = sizes->getDouble("defAngle", 0.0);
					originPoint = sizes->getInt("OriginL", 0);
				}
				bool doRemember = sizes->getBool("Remember", true);
				bool doCreate = false;
				if (m->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier))
					doCreate = true;
				else
				{
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					OneClick *dia = new OneClick(m_view, ScribusView::tr("Enter Object Size"), m_doc->unitIndex(), xSize, ySize, doRemember, originPoint, lmode);
					if (dia->exec())
					{
						doRemember = dia->checkRemember->isChecked();
						if (lmode == 0)
						{
							xSize = dia->spinWidth->value() / unitGetRatioFromIndex(m_doc->unitIndex());
							ySize = dia->spinHeight->value() / unitGetRatioFromIndex(m_doc->unitIndex());
							originPoint = dia->RotationGroup->checkedId();
							if (doRemember)
							{
								sizes->set("defWidth", xSize);
								sizes->set("defHeight", ySize);
								sizes->set("Origin", originPoint);
							}
						}
						else
						{
							xSize = dia->spinWidth->value() / unitGetRatioFromIndex(m_doc->unitIndex());
							ySize = dia->spinHeight->value();
							originPoint = dia->RotationGroup->checkedId();
							if (doRemember)
							{
								sizes->set("defLength", xSize);
								sizes->set("defAngle", ySize);
								sizes->set("OriginL", originPoint);
							}
						}
						sizes->set("Remember", doRemember);
						doCreate = true;
					}
					else
					{
						m_view->Deselect(false);
						m_doc->Items->removeAt(currItem->ItemNr);
					}
					delete dia;
				}
				if (doCreate)
				{
					bool oldSnap = m_doc->SnapGuides;
					m_doc->SnapGuides = false;
					if (m_doc->appMode == modeDrawLine)
					{
						currItem->setWidthHeight(xSize, 1);
						currItem->setRotation(-ySize);
						currItem->Sizing = false;
						currItem->updateClip();
					}
					else if (m_doc->appMode == modeDrawRegularPolygon)
					{
						currItem->setWidthHeight(xSize, ySize);
						FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), m_doc->toolSettings.polyC, m_doc->toolSettings.polyS, m_doc->toolSettings.polyF, m_doc->toolSettings.polyR);
						FPoint np(cli.point(0));
						currItem->PoLine.resize(2);
						currItem->PoLine.setPoint(0, np);
						currItem->PoLine.setPoint(1, np);
						for (uint ax = 1; ax < cli.size(); ++ax)
						{
							np = FPoint(cli.point(ax));
							currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
						}
						np = FPoint(cli.point(0));
						currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
						FPoint tp2(getMinClipF(&currItem->PoLine));
						if ((tp2.x() > -1) || (tp2.y() > -1))
							m_doc->SizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem->ItemNr, false, false, false);
						FPoint tp(getMaxClipF(&currItem->PoLine));
						m_doc->SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
//						currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
						m_doc->AdjustItemSize(currItem);
					}
					else
					{
						m_doc->SizeItem(xSize, ySize, currItem->ItemNr, false, false, false);
						currItem->updateClip();
					}
					currItem->ContourLine = currItem->PoLine.copy();
					switch (originPoint)
					{
						case 0:
							break;
						case 1:
							if (lmode == 0)
								m_doc->MoveItem(-currItem->width(), 0.0, currItem, false);
							else
								m_doc->MoveRotated(currItem, FPoint(-currItem->width() / 2.0, 0.0), false);
							break;
						case 2:
							if (lmode == 0)
								m_doc->MoveItem(-currItem->width() / 2.0, -currItem->height() / 2.0, currItem, false);
							else
								m_doc->MoveRotated(currItem, FPoint(-currItem->width(), 0.0), false);
							break;
						case 3:
							m_doc->MoveItem(0.0, -currItem->height(), currItem, false);
							break;
						case 4:
							m_doc->MoveItem(-currItem->width(), -currItem->height(), currItem, false);
							break;
					}
					m_doc->setRedrawBounding(currItem);
					currItem->OwnPage = m_doc->OnPage(currItem);
					currItem->OldB2 = currItem->width();
					currItem->OldH2 = currItem->height();
					currItem->Sizing = false;
					m_doc->SnapGuides = oldSnap;
					if (!PrefsManager::instance()->appPrefs.stickyTools)
					{
						m_view->requestMode(modeNormal);
//						m_view->requestMode(submodePaintingDone);
					}
					else
					{
						int appMode = m_doc->appMode;
						if ((inItemCreation) && (appMode == modeNormal))
						{
							currItem = m_doc->m_Selection->itemAt(0);
							if (currItem->asTextFrame())
								appMode = modeDrawText;
							else if (currItem->asImageFrame())
								appMode = modeDrawImage;
							else if (m_doc->SubMode != -1)
								appMode = modeDrawShapes;
						}
						m_view->requestMode(appMode);
					}
					currItem->update();
					m_doc->changed();
//					emit DocChanged();
				}
				else
				{
					m_view->requestMode(submodePaintingDone);
//					emit HaveSel(-1);
				}
				inItemCreation = false;
				m_doc->DragP = false;
				m_doc->leaveDrag = false;
				m_canvas->m_viewMode.operItemMoving = false;
				m_canvas->m_viewMode.operItemResizing = false;
				m_canvas->setRenderModeUseBuffer(false);
				m_view->MidButt = false;
				shiftSelItems = false;
				m_view->updateContents();
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			}
		}
		if ((m_doc->appMode == modeDrawLine) && (inItemCreation))
		{
			currItem = m_doc->m_Selection->itemAt(0);
			QMatrix p;
			m_canvas->Transform(currItem, p);
			QPoint np = m->pos() * p.inverted();
//			np += QPoint(qRound(m_doc->minCanvasCoordinate.x()), qRound(m_doc->minCanvasCoordinate.y()));
			np = m_doc->ApplyGrid(np);
			double newRot=xy2Deg(np.x(), np.y());
			//Constrain rotation angle, when the mouse is released from drawing a line
			if (m->modifiers() & Qt::ControlModifier)
				newRot=constrainAngle(newRot, m_doc->toolSettings.constrain);
			currItem->setRotation(newRot);
			currItem->setWidthHeight(sqrt(pow(np.x(),2.0)+pow(np.y(),2.0)), 1.0);
			currItem->Sizing = false;
			currItem->updateClip();
			m_doc->setRedrawBounding(currItem);
			currItem->OwnPage = m_doc->OnPage(currItem);
			m_canvas->m_viewMode.operItemResizing = false;
			m_canvas->setRenderModeUseBuffer(false);
			m_view->updateContents();
			m_doc->changed();
		}
		if ((m_doc->appMode == modeDrawRegularPolygon) && (inItemCreation))
		{
			currItem = m_doc->m_Selection->itemAt(0);
			FPoint np1(mousePointDoc); //m_view->translateToDoc(m->x(), m->y()));
			np1 = m_doc->ApplyGridF(np1);
			double w = np1.x() - currItem->xPos();
			double h = np1.y()- currItem->yPos();
			currItem->setWidthHeight(fabs(w), fabs(h));
			if (w < 0.0)
				currItem->setXPos(currItem->xPos() - fabs(w), true);
			if (h < 0.0)
				currItem->setYPos(currItem->yPos() - fabs(h), true);
			FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), m_doc->toolSettings.polyC, m_doc->toolSettings.polyS, m_doc->toolSettings.polyF, m_doc->toolSettings.polyR);
			FPoint np(cli.point(0));
			currItem->PoLine.resize(2);
			currItem->PoLine.setPoint(0, np);
			currItem->PoLine.setPoint(1, np);
			for (uint ax = 1; ax < cli.size(); ++ax)
			{
				np = FPoint(cli.point(ax));
				currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
			}
			np = FPoint(cli.point(0));
			currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
			FPoint tp2(getMinClipF(&currItem->PoLine));
			if ((tp2.x() > -1) || (tp2.y() > -1))
			{
				m_doc->SizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem->ItemNr, false, false, false);
			}
			FPoint tp(getMaxClipF(&currItem->PoLine));
			m_doc->SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
//			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
			m_doc->AdjustItemSize(currItem);
			currItem->ContourLine = currItem->PoLine.copy();
			m_doc->setRedrawBounding(currItem);
			currItem->OwnPage = m_doc->OnPage(currItem);
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			m_canvas->m_viewMode.operItemResizing = false;
			m_canvas->setRenderModeUseBuffer(false);
			m_view->updateContents();
		}
		if (m_view->moveTimerElapsed() && (GetItem(&currItem)))
		{
			m_view->stopDragTimer();
			m_canvas->setRenderModeUseBuffer(false);
			if (m_doc->m_Selection->isMultipleSelection())
			{
				if (m_canvas->m_viewMode.operItemResizing)
				{
					double gx, gy, gh, gw, nx, ny, scx, scy;
					m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					double sc = m_canvas->scale();
					scx = sc;
					scy = sc;
					QPoint np2;
					double newXF = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
					double newYF = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
					if (m->modifiers() & Qt::ControlModifier)
						np2 = QPoint(qRound(newXF), qRound(gy+(gh * ((newXF-gx) / gw))));
					else
						np2 = QPoint(qRound(newXF), qRound(newYF));
					nx = np2.x();
					ny = np2.y();
					if (!m_doc->ApplyGuides(&nx, &ny))
					{
						np2 = m_doc->ApplyGrid(np2);
						nx = np2.x();
						ny = np2.y();
					}
					switch (frameResizeHandle)
					{
						case 1:
							scy = fabs(ny-gy) / gh;
							scx = fabs(nx-gx) / gw;
							break;
						case 2:
							scx = fabs(nx-(gx+gw)) / gw;
							scy = fabs(ny-(gy+gh)) / gh;
							break;
						case 3:
							scx = fabs(nx-gx) / gw;
							scy = fabs(ny-(gy+gh)) / gh;
							break;
						case 4:
							scx = fabs(nx-(gx+gw)) / gw;
							scy = fabs(ny-gy) / gh;
							break;
						case 5:
							scx = 1.0;
							scy = fabs(ny-gy) / gh;
							break;
						case 6:
							scx = fabs(nx-gx) / gw;
							scy = 1.0;
							break;
						case 7:
							scx = fabs(nx-(gx+gw)) / gw;
							scy = 1.0;
							break;
						case 8:
							scx = 1.0;
							scy = fabs(ny-(gy+gh)) / gh;
							break;
					}
					RotMode = m_doc->RotMode;
					m_doc->RotMode = 0;
					//CB #3012 only scale text in a group if alt is pressed
					if ((currItem->itemType() == PageItem::TextFrame) && (m->modifiers() & Qt::AltModifier))
						m_view->scaleGroup(scx, scy, true);
					else
						m_view->scaleGroup(scx, scy, false);
					if ((frameResizeHandle == 3) || (frameResizeHandle == 8))
						m_view->moveGroup(0, ny-gy);
					if (frameResizeHandle == 2)
						m_view->moveGroup(nx-gx, ny-gy);
					if ((frameResizeHandle == 7) || (frameResizeHandle == 4))
						m_view->moveGroup(nx-gx, 0);
					m_doc->RotMode = RotMode;
//					evSpon = false;
					m_canvas->m_viewMode.operItemResizing = false;
//					m_view->updateContents();
// scaleGroup calls PageItem::update					emit DocChanged();
				}
			}
			else
			{
				//Where a single frame resize stops when the mouse buttno is released
				//FIXME: reduce code!
				if (currItem->Sizing)
				{
					FPoint npx;
					double nx = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
					double ny = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
					if (m_doc->SnapGuides)
					{
						m_doc->ApplyGuides(&nx, &ny);
						npx = FPoint(nx, ny, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
					}
					else
					{
						npx = m_doc->ApplyGridF(FPoint(nx, ny));
						npx = FPoint(npx.x(), npx.y(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
					}
					if ((frameResizeHandle == 1) && !(currItem->asLine()) && (m_doc->SnapGuides))
						m_doc->SizeItem(npx.x(), npx.y(), currItem->ItemNr);
					bool sav = m_doc->SnapGuides;
					m_doc->SnapGuides = false;
					if (UndoManager::undoEnabled())
					{
						m_view->undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(),
							Um::Resize, QString(Um::ResizeFromTo).arg(currItem->width()).arg(currItem->height()).arg(currItem->width() - npx.x()).arg(currItem->height() - npx.y()), Um::IResize);
					}
					m_canvas->m_viewMode.operItemResizing = false;
					switch (frameResizeHandle)
					{
					case 1:
						if (!currItem->asLine())
						{
							if (currItem->isTableItem)
							{
								double dist;
								if (currItem->LeftLink != 0)
									dist = npx.y() - currItem->LeftLink->height();
								else if (currItem->RightLink != 0)
									dist = npx.y() - currItem->RightLink->height();
								else
									dist = npx.y() - currItem->height();
								PageItem* bb2;
								PageItem* bb = currItem;
								while (bb->LeftLink != 0)
								{
									bb = bb->LeftLink;
								}
								while (bb->RightLink != 0)
								{
									bb2 = bb;
									while (bb2->BottomLink != 0)
									{
										m_doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
										bb2 = bb2->BottomLink;
									}
									if (bb != currItem)
										m_doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
									bb = bb->RightLink;
								}
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									m_doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								if (bb != currItem)
									m_doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = currItem;
								if (currItem->TopLink != 0)
									dist = npx.x() - currItem->TopLink->width();
								else if (currItem->BottomLink != 0)
									dist = npx.x() - currItem->BottomLink->width();
								else
									dist = npx.x() - currItem->width();
								while (bb->TopLink != 0)
								{
									bb = bb->TopLink;
								}
								while (bb->BottomLink != 0)
								{
									bb2 = bb;
									while (bb2->RightLink != 0)
									{
										m_doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
										bb2 = bb2->RightLink;
									}
									if (bb != currItem)
										m_doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
									bb = bb->BottomLink;
								}
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									m_doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								if (bb != currItem)
									m_doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
							}
							//<<Swap location if width/height is <0
							if (currItem->width()<0)
							{
								currItem->setWidth(-currItem->width());
								currItem->setXPos(currItem->xPos()-currItem->width());
							}
							if (currItem->height()<0)
							{
								currItem->setHeight(-currItem->height());
								currItem->setYPos(currItem->yPos()-currItem->height());
							}
							//>>
							if (currItem->imageFlippedH())
								currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
							if (currItem->imageFlippedV())
								currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						}
						else
						{
							if (sav)
							{
								double nx = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
								double ny = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
								if (m_doc->useRaster)
								{
									FPoint ra(m_doc->ApplyGridF(FPoint(nx, ny)));
									nx = ra.x();
									ny = ra.y();
								}
								m_doc->SnapGuides = sav;
								m_doc->ApplyGuides(&nx, &ny);
								m_doc->SnapGuides = false;
								double r = atan2(ny - currItem->yPos(), nx - currItem->xPos())*(180.0/M_PI);
								m_doc->RotateItem(r, currItem->ItemNr);
								double w = sqrt(pow(nx - currItem->xPos(), 2) + pow(ny - currItem->yPos(),2));
								m_doc->SizeItem(w, currItem->height(), currItem->ItemNr, true);
							}
						}
						currItem->Sizing = false;
						break;
					case 2:
						if (!currItem->asLine())
						{
							if (currItem->isTableItem)
							{
								PageItem* bb2;
								PageItem* bb = currItem;
								while (bb->TopLink != 0)
								{
									bb = bb->TopLink;
								}
								while (bb->BottomLink != 0)
								{
									bb2 = bb;
									while (bb2->LeftLink != 0)
									{
										m_doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
										bb2 = bb2->LeftLink;
									}
									m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
									bb = bb->BottomLink;
								}
								bb2 = bb;
								while (bb2->LeftLink != 0)
								{
									m_doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
									bb2 = bb2->LeftLink;
								}
								m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
								bb = currItem;
								while (bb->LeftLink != 0)
								{
									bb = bb->LeftLink;
								}
								while (bb->RightLink != 0)
								{
									bb2 = bb;
									while (bb2->TopLink != 0)
									{
										m_doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
										bb2 = bb2->TopLink;
									}
									m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
									bb = bb->RightLink;
								}
								bb2 = bb;
								while (bb2->TopLink != 0)
								{
									m_doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
									bb2 = bb2->TopLink;
								}
								m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
							}
							else
								m_doc->MoveSizeItem(npx, npx, currItem->ItemNr);
								
							//<<Swap location if width/height is <0
							if (currItem->width()<0)
							{
								currItem->setWidth(-currItem->width());
								currItem->setXPos(currItem->xPos()-currItem->width());
							}
							if (currItem->height()<0)
							{
								currItem->setHeight(-currItem->height());
								currItem->setYPos(currItem->yPos()-currItem->height());
							}
							//>>
								
							currItem->Sizing = false;
							if (!currItem->imageFlippedH())
							{
								currItem->moveImageInFrame((currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
								m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							}
							if (!currItem->imageFlippedV())
							{
								currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
								m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							}
						}
						else
						{
							if (sav)
							{
								double nx = mousePointDoc.x(); //m_view->translateToDoc(m->x(), m->y()).x();
								double ny = mousePointDoc.y(); //m_view->translateToDoc(m->x(), m->y()).y();
								if (m_doc->useRaster)
								{
									FPoint ra(m_doc->ApplyGridF(FPoint(nx, ny)));
									nx = ra.x();
									ny = ra.y();
								}
								m_doc->SnapGuides = sav;
								m_doc->ApplyGuides(&nx, &ny);
								m_doc->SnapGuides = false;
								QMatrix ma;
								ma.translate(currItem->xPos(), currItem->yPos());
								ma.rotate(currItem->rotation());
								double mx = ma.m11() * currItem->width() + ma.m21() * currItem->height() + ma.dx();
								double my = ma.m22() * currItem->height() + ma.m12() * currItem->width() + ma.dy();
								double r = atan2(my-ny,mx-nx)*(180.0/M_PI);
								double w = sqrt(pow(mx-nx,2)+pow(my-ny,2));
								m_doc->MoveItem(nx - currItem->xPos(), ny - currItem->yPos(), currItem, true);
								m_doc->SizeItem(w, currItem->height(), currItem->ItemNr, true);
								m_doc->RotateItem(r, currItem->ItemNr);
								currItem->Sizing = false;
							}
						}
						break;
					case 3:
						if (currItem->isTableItem)
						{
							double dist = npx.x() - currItem->width();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									m_doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								m_doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->RightLink != 0)
							{
								m_doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
								bb2 = bb2->RightLink;
							}
							m_doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
							bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->TopLink != 0)
								{
									m_doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
									bb2 = bb2->TopLink;
								}
								m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->TopLink != 0)
							{
								m_doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
								bb2 = bb2->TopLink;
							}
							m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
						}
						else
							m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(currItem->width() - npx.x(), npx.y()), currItem->ItemNr);
							
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (currItem->imageFlippedH())
							currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (!currItem->imageFlippedV())
							currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					case 4:
					
						if (currItem->isTableItem)
						{
							double dist = npx.y() - currItem->height();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									m_doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								m_doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->BottomLink != 0)
							{
								m_doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
								bb2 = bb2->BottomLink;
							}
							m_doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
							bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->LeftLink != 0)
								{
									m_doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
									bb2 = bb2->LeftLink;
								}
								m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->LeftLink != 0)
							{
								m_doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
								bb2 = bb2->LeftLink;
							}
							m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
						}
						else
							m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), currItem->height() - npx.y()), currItem->ItemNr);
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (!currItem->imageFlippedH())
							currItem->moveImageInFrame((currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (currItem->imageFlippedV())
							currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					case 5:
						if (currItem->isTableItem)
						{
							double dist = npx.y() - currItem->height();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									m_doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								m_doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->BottomLink != 0)
							{
								m_doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
								bb2 = bb2->BottomLink;
							}
							m_doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
						}
						else
							m_doc->MoveSizeItem(FPoint(0, 0), FPoint(0, currItem->height() - npx.y()), currItem->ItemNr);
						if (currItem->imageFlippedV())
							currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						break;
					case 6:
						if (currItem->isTableItem)
						{
							double dist = npx.x() - currItem->width();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									m_doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								m_doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->RightLink != 0)
							{
								m_doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
								bb2 = bb2->RightLink;
							}
							m_doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
						}
						else
							m_doc->MoveSizeItem(FPoint(0, 0), FPoint(currItem->width() - npx.x(), 0), currItem->ItemNr);
						if (currItem->imageFlippedH())
							currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
						
						currItem->Sizing = false;
						break;
					case 7:
						if (currItem->isTableItem)
						{
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->LeftLink != 0)
								{
									m_doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
									bb2 = bb2->LeftLink;
								}
								m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->LeftLink != 0)
							{
								m_doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
								bb2 = bb2->LeftLink;
							}
							m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
						}
						else
							m_doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), currItem->ItemNr);
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (!currItem->imageFlippedH())
							currItem->moveImageInFrame((currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (currItem->imageFlippedV())
							currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					case 8:
						if (currItem->isTableItem)
						{
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->TopLink != 0)
								{
									m_doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
									bb2 = bb2->TopLink;
								}
								m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->TopLink != 0)
							{
								m_doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
								bb2 = bb2->TopLink;
							}
							m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
						}
						else
							m_doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), currItem->ItemNr);
							
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (currItem->imageFlippedH())
							currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (!currItem->imageFlippedV())
							currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					}

					//TextFrame resize - Resize text with resize of frame
					//alt resize, free resize with text scaling
					//shift alt, square resize with text scaling
					//control alt, proportional resize with text scaling
					//if ((currItem->itemType() == PageItem::TextFrame) && (m->modifiers() & Qt::ShiftModifier) && (m->modifiers() & Qt::ControlModifier))
					//NOTE: this does not cover groups, strangely enough. Thats done in scaleGroup()
					if ((currItem->itemType() == PageItem::TextFrame) && (m->modifiers() & Qt::AltModifier))
					{
						double scx = currItem->width() / currItem->OldB2;
						double scy = currItem->height() / currItem->OldH2;
						scx = scx != scy ? scx / scy : 1.0;
						if (currItem->itemText.length() != 0)
						{
//							currItem->setFontSize(qMax(qRound(currItem->fontSize() * scy), 1));
//							currItem->setLineSpacing(((currItem->fontSize() / 10.0)* static_cast<double>(m_doc->typographicSettings.autoLineSpacing) / 100) + (currItem->fontSize() / 10.0));
//							currItem->TxtScale = qMin(qMax(qRound(currItem->TxtScale * scx), 25), 400);
/*							m_doc->currTextScale = currItem->TxtScale;
							m_doc->CurrFontSize = currItem->fontSize();
							m_doc->CurrTextBase = currItem->TxtBase;
							m_doc->CurrTextShadowX = currItem->TxtShadowX;
							m_doc->CurrTextShadowY = currItem->TxtShadowY;
							m_doc->CurrTextOutline = currItem->TxtOutline;
							m_doc->CurrTextUnderPos = currItem->TxtUnderPos;
							m_doc->CurrTextUnderWidth = currItem->TxtUnderWidth;
							m_doc->CurrTextStrikePos = currItem->TxtStrikePos;
							m_doc->CurrTextStrikeWidth = currItem->TxtStrikeWidth;
							emit ItemTextStrike(currItem->TxtStrikePos, currItem->TxtStrikeWidth);
							emit ItemTextUnderline(currItem->TxtUnderPos, currItem->TxtUnderWidth);
							emit ItemTextOutline(currItem->TxtOutline);
							emit ItemTextShadow(currItem->TxtShadowX, currItem->TxtShadowY);
							//emit ItemTextAtScribusView::tr(currItem->LineSp);
							emit ItemTextCols(currItem->Cols, currItem->ColGap);
							emit ItemTextSize(currItem->fontSize());
							emit ItemTextSca(currItem->TxtScale);
							emit ItemTextScaV(currItem->TxtScaleV);
							emit ItemTextBase(currItem->TxtBase);
*/
#ifndef NLS_PROTO
							for (int aa = 0; aa < currItem->itemText.length(); ++aa)
							{
								currItem->itemText.item(aa)->setFontSize(qMax(qRound(currItem->itemText.item(aa)->fontSize()*scy), 1));
								currItem->itemText.item(aa)->setScaleH(qMax(qMin(qRound(currItem->itemText.item(aa)->scaleH()*scx), 4000), 100));
							}
#endif
						}
					}
					if (currItem->itemType() == PageItem::ImageFrame)
					{
						currItem->AdjustPictScale();
						/*Dont need this now adjustpictscale will emit for 1st selected item
						emit SetLocalValues(currItem->LocalScX, currItem->LocalScY, currItem->LocalX, currItem->LocalY );
						*/
					}
					if (currItem->itemType() == PageItem::TextFrame)
						currItem->invalid = true;
					currItem->updateClip();
//					emit ItemTextCols(currItem->Cols, currItem->ColGap);
					m_doc->SnapGuides = sav;
//					evSpon = false;
					currItem->update(); // MoveSizeItem *should*  call PageItem::update
					m_view->updateContents();
//					emit DocChanged();
					currItem->checkChanges();
					if (UndoManager::undoEnabled())
						m_view->undoManager->commit();
				}
				m_doc->setRedrawBounding(currItem);
				currItem->OwnPage = m_doc->OnPage(currItem);
				m_canvas->m_viewMode.operItemResizing = false;
				if (currItem->asLine())
					m_view->updateContents();
			}
			if (m_canvas->m_viewMode.operItemMoving)
			{
				m_view->updatesOn(false);
//				evSpon = false;
				if (m_doc->m_Selection->isMultipleSelection())
				{
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
					m_view->moveGroup(nx-gx, ny-gy, false);
					m_doc->m_Selection->setGroupRect();
					m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					nx = gx+gw;
					ny = gy+gh;
					if (m_doc->ApplyGuides(&nx, &ny))
						m_view->moveGroup(nx-(gx+gw), ny-(gy+gh), false);
					m_doc->m_Selection->setGroupRect();
				}
				else
				{
					currItem = m_doc->m_Selection->itemAt(0);
/*					if (!currItem->asLine())
					{
						if (fabs(currItem->width()) < 5)
							currItem->setWidth(5.0);
						if (fabs(currItem->height()) < 5)
							currItem->setHeight(5.0);
					} */
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
				// moveGroup/moveItem calls PageItem::update				
//				emit DocChanged();
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
				m_doc->m_Selection->setIsGUISelection(false);
				for (int a = 0; a < docItemCount; ++a)
				{
					PageItem* docItem = m_doc->Items->at(a);
					QMatrix p;
					m_canvas->Transform(docItem, p);
					QRegion apr = QRegion(docItem->Clip * p);
					QRect apr2(docItem->getRedrawBounding(1.0));
					if ((m_doc->masterPageMode()) && (docItem->OnMasterPage != m_doc->currentPage()->pageName()))
						continue;
					//CB Finally Items are selected here
					if (((Sele.contains(apr.boundingRect())) || (Sele.contains(apr2))) && (docItem->LayerNr == m_doc->activeLayer()) && (!m_doc->layerLocked(docItem->LayerNr)))
					{
					//CB set draw to true to (dis)enable some actions via emit to HaveNewSel in scapp.
					//CB FIXME emit from selection when multiple selected instead
						//CB #3523 bool redrawSelection=(a==docItemCount-1);
						bool redrawSelection=false;
						m_view->SelectItemNr(a, redrawSelection);
					}
				}
				m_doc->m_Selection->setIsGUISelection(true);
				m_doc->m_Selection->connectItemToGUI();
				if (m_doc->m_Selection->count() > 1)
				{
					m_doc->m_Selection->setGroupRect();
					double x, y, w, h;
					m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
//					emit ItemPos(x, y);
//					emit ItemGeom(w, h);
					m_view->getGroupRectScreen(&x, &y, &w, &h);
				}
//				if (m_doc->m_Selection->count() > 0)
//					emit HaveSel(m_doc->m_Selection->itemAt(0)->itemType());
			}
			m_view->HaveSelRect = false;
			shiftSelItems = false;
			m_view->redrawMarker->hide();
			m_view->updateContents();
		}
		if (m_doc->appMode != modeEdit)
		{
			if (m_doc->appMode == modeRotation)
				m_doc->RotMode = RotMode;
			if (!PrefsManager::instance()->appPrefs.stickyTools)
			{
//				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				m_view->requestMode(modeNormal);
//				m_view->requestMode(submodePaintingDone);
			}
			else
			{
				int appMode = m_doc->appMode;
				if ((inItemCreation) && (appMode == modeNormal))
				{
					currItem = m_doc->m_Selection->itemAt(0);
					if (currItem->asTextFrame())
						appMode = modeDrawText;
					else if (currItem->asImageFrame())
						appMode = modeDrawImage;
					else if (m_doc->SubMode != -1)
						appMode = modeDrawShapes;
				}
				m_view->requestMode(appMode);
			}
		}
		if (GetItem(&currItem))
		{
			if (m_doc->m_Selection->count() > 1)
			{
				m_doc->m_Selection->setGroupRect();
				//CB shouldnt need this i think
				//emit HaveSel(currItem->itemType());
				double x, y, w, h;
				m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
//				emit ItemPos(x, y);
//				emit ItemGeom(w, h);
//				m_view->getGroupRectScreen(&x, &y, &w, &h);
				
				m_canvas->m_viewMode.operItemMoving = false;
				m_canvas->m_viewMode.operItemResizing = false;
				m_view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
			}
			else
				currItem->emitAllToGUI();
				
			/*else
			{
				//CB Dont think we need this here with the new selection code
				//For a select, deselect operation, this will cause 2x emit
				//emit HaveSel(currItem->itemType());
				//EmitValues(currItem);
				//currItem->emitAllToGUI();
			}*/
		}
	}
	
	if ((m_doc->appMode == modeEdit) && !m_canvas->m_viewMode.operItemResizeInEditMode)
	{
		currItem = m_doc->m_Selection->itemAt(0);
		if (currItem->asTextFrame())
		{
			if (oldCp == currItem->CPos)
			{
				currItem->itemText.deselectAll();
				currItem->HasSel = false;
				m_ScMW->DisableTxEdit();
				currItem->update();
			}
		}
		else
			currItem->update();
//			emit DocChanged();
	}
	if (m_doc->appMode == modeMagnifier)
	{
		double sc = m_canvas->scale();
		if (m_view->HaveSelRect)
		{
			QRect geom = m_view->redrawMarker->geometry().normalized();
			//QPoint xp = m_view->viewport()->mapFromGlobal(QPoint(geom.x() + geom.width() / 2, geom.y() + geom.height() / 2));
			FPoint nx = m_canvas->globalToCanvas(QPoint(geom.x() + geom.width() / 2, geom.y() + geom.height() / 2)); //m_view->translateToDoc(xp.x()+m_view->contentsX(), xp.y()+m_view->contentsY());
			double scaleAdjust = m_view->visibleWidth() / static_cast<double>(qMax(geom.width(), 1));
			m_view->setScale(m_canvas->scale() * scaleAdjust);
			m_view->slotDoZoom();
			m_view->SetCCPo(nx.x(), nx.y());
			if (sc == m_canvas->scale())
			{
				m_view->HaveSelRect = false;
				m_view->redrawMarker->hide();
//				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				m_view->requestMode(submodePaintingDone);
			}
			m_view->redrawMarker->hide();
		}
		else
		{
			FPoint nx = mousePointDoc; //m_view->translateToDoc(m->x(), m->y());
			int mx = qRound(nx.x());
			int my = qRound(nx.y());
			m_view->Magnify ? m_view->slotZoomIn(mx,my) : m_view->slotZoomOut(mx,my);
			if (sc == m_canvas->scale())
			{
				m_view->HaveSelRect = false;
//				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				m_view->requestMode(submodePaintingDone);
			}
			else
			{
				if (m->modifiers() & Qt::ShiftModifier)
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
				else
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
			}
		}
	}
	m_canvas->setRenderModeUseBuffer(false);
	if ((m_doc->appMode == modeDrawBezierLine) && (m->button() == Qt::LeftButton))
	{
		m_canvas->setRenderModeUseBuffer(true);
		currItem = m_doc->m_Selection->itemAt(0);
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		QMatrix pm;
//		pm.translate(-m_doc->minCanvasCoordinate.x()*m_canvas->scale(), -m_doc->minCanvasCoordinate.y()*m_canvas->scale());
		m_canvas->Transform(currItem, pm);
		FPoint npf = FPoint(m->pos() * pm.inverted());
		npf = m_doc->ApplyGridF(npf);
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
			m_doc->MoveItem(np2.x(), 0, currItem);
		}
		if (np2.y() < 0)
		{
			currItem->PoLine.translate(0, -np2.y());
			m_doc->MoveItem(0, np2.y(), currItem);
		}
		if (FirstPoly)
		{
			FirstPoly = false;
			currItem->Sizing = ssiz;
		}
		else
		{
			m_doc->SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false, false);
			m_doc->AdjustItemSize(currItem);
			currItem->Sizing = ssiz;
			currItem->ContourLine = currItem->PoLine.copy();
			m_canvas->setRenderModeUseBuffer(false);
			m_view->repaintContents(currItem->getRedrawBounding(m_canvas->scale()));
		}
		m_canvas->setRenderModeFillBuffer();
		int newX = qRound(mousePointDoc.x()); //m_view->translateToDoc(m->x(), m->y()).x());
		int newY = qRound(mousePointDoc.y()); //m_view->translateToDoc(m->x(), m->y()).y());
		m_canvas->newRedrawPolygon() << QPoint(newX - qRound(currItem->xPos()), newY - qRound(currItem->yPos()));
		m_view->updateContents();
	}
	if ((m_doc->appMode == modeDrawBezierLine) && (m->button() == Qt::RightButton))
	{
		currItem = m_doc->m_Selection->itemAt(0);
		if (currItem!=0)
		{
			currItem->PoLine.resize(currItem->PoLine.size()-2);
			if (currItem->PoLine.size() < 4)
			{
	//			emit DelObj(m_doc->currentPage->pageNr(), currItem->ItemNr);
				m_doc->Items->removeAt(currItem->ItemNr);
				m_doc->m_Selection->removeFirst();
				//emit HaveSel(-1);
			}
			else
			{
				m_doc->SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false);
//				currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2.0, 1)));
				m_doc->AdjustItemSize(currItem);
				currItem->ContourLine = currItem->PoLine.copy();
			}
		}
		if (!PrefsManager::instance()->appPrefs.stickyTools)
		{
//			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			m_view->requestMode(modeNormal);
//			m_view->requestMode(submodePaintingDone);
		}
		else
			m_view->requestMode(m_doc->appMode);
		currItem->update();
//		emit DocChanged();
		FirstPoly = true;
		m_canvas->setRenderModeUseBuffer(false);
//		m_view->updateContents();
	}
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
	shiftSelItems = false;
	inItemCreation = false;
//	m_doc->SubMode = -1;
	if (m_view->_groupTransactionStarted)
	{
		for (int i = 0; i < m_doc->m_Selection->count(); ++i)
			m_doc->m_Selection->itemAt(i)->checkChanges(true);
		m_view->undoManager->commit();
		m_view->_groupTransactionStarted = false;
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
	else
	{
		//delete oldClip;
		//oldClip = 0;
	}
}



void LegacyMode::selectPage(QMouseEvent *m)
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
bool LegacyMode::SeleItem(QMouseEvent *m)
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
		double bleedRight = 0.0;
		double bleedLeft = 0.0;
		double bleedBottom = 0.0;
		double bleedTop = 0.0;
		bool drawBleed = false;
		if (((m_doc->bleeds.Bottom != 0.0) || (m_doc->bleeds.Top != 0.0) || (m_doc->bleeds.Left != 0.0) || (m_doc->bleeds.Right != 0.0)) && (m_doc->guidesSettings.showBleed))
			drawBleed = true;
		for (int a = docPageCount; a > -1; a--)
		{
			if (drawBleed)
				m_doc->getBleeds(a, &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
			int x = static_cast<int>(m_doc->Pages->at(a)->xOffset() - bleedLeft);
			int y = static_cast<int>(m_doc->Pages->at(a)->yOffset() - bleedTop);
			int w = static_cast<int>(m_doc->Pages->at(a)->width() + bleedLeft + bleedRight);
			int h = static_cast<int>(m_doc->Pages->at(a)->height() + bleedBottom + bleedTop);
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
				qDebug() << "select item: found BENEATH" << currItem << "groups" << currItem->Groups.count();
				if (currItem->Groups.count() > 0)
				{
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
				}
				else
				{
					m_doc->m_Selection->removeItem(currItem);
				}
				break;
			}
			else
				qDebug() << "select item: not BENEATH" << QPointF(mousePointDoc.x(),mousePointDoc.y()) 
					<< m_doc->m_Selection->itemAt(i)->getTransform() 
					<< m_doc->m_Selection->itemAt(i)->getBoundingRect();
		}
	}
	else if ( (m->modifiers() & SELECT_MULTIPLE) == Qt::NoModifier || (m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames) )
	{
		m_view->Deselect(false);
	}
	
	qDebug() << "select item: beneath" << (m->modifiers() & SELECT_BENEATH) << currItem 
		<< "multi" << (m->modifiers() & SELECT_MULTIPLE)
		<< "current sel" << m_doc->m_Selection->count();
	currItem = m_canvas->itemUnderCursor(m->globalPos(), currItem, (m->modifiers() & SELECT_IN_GROUP));
	qDebug() << "item under cursor: " << currItem;
	if (currItem)
	{
		m_doc->m_Selection->setIsGUISelection(false);
		if (m_doc->m_Selection->containsItem(currItem))
		{
			m_doc->m_Selection->removeItem(currItem);
		}
		else			{
		{
			m_doc->m_Selection->prependItem(currItem);
			if ( (m->modifiers() & SELECT_IN_GROUP) )
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
		}
		
		currItem->update();
		
		m_doc->m_Selection->setIsGUISelection(true);
		m_doc->m_Selection->connectItemToGUI();
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
	
/*	
	if ((m->modifiers() == SELECT_SHIFTALT) && (!m_doc->masterPageMode()) && (m_doc->currentPage()->FromMaster.count() != 0))
	{
		currItem = m_canvas->itemUnderCursor(m->globalPos(), currItem, false, true);
		Page* Mp = m_doc->MasterPages.at(m_doc->MasterNames[m_doc->currentPage()->MPageNam]);
		currItem = m_doc->currentPage()->FromMaster.at(m_doc->currentPage()->FromMaster.count()-1);
		int currNr = m_doc->currentPage()->FromMaster.count()-1;
		for (a = 0; a < m_doc->currentPage()->FromMaster.count(); ++a)
		{
			p = QMatrix();
			if ((currItem->LayerNr == m_doc->activeLayer()) && (!m_doc->layerLocked(currItem->LayerNr)))
			{
				double OldX = currItem->xPos();
				double OldY = currItem->yPos();
				if (!currItem->ChangedMasterItem)
				{
					currItem->moveBy(-Mp->xOffset() + m_doc->currentPage()->xOffset(), -Mp->yOffset() + m_doc->currentPage()->yOffset());
				}
				m_canvas->Transform(currItem, p);
				if ((QRegion(p.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo.toRect())) ||
						(QRegion(currItem->Clip * p).contains(mpo.toRect())))
				{
					m_doc->m_Selection->setIsGUISelection(false);
					if (!currItem->isSelected())
					{
						if ((m->modifiers() != SELECT_MULTIPLE) || (m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames))
							m_view->Deselect(false);
						if (currItem->Groups.count() != 0)
						{
							if (m_doc->m_Selection->count() != 0)
							{
								if (m_doc->m_Selection->findItem(currItem) == -1)
									m_doc->m_Selection->addItem(currItem, true);
							}
							else
								m_doc->m_Selection->addItem(currItem, true);
							if (m->modifiers() != SELECT_IN_GROUP)
							{
								for (int ga=0; ga<m_doc->Items->count(); ++ga)
								{
									if (m_doc->Items->at(ga)->Groups.count() != 0)
									{
										if (m_doc->Items->at(ga)->Groups.top() == currItem->Groups.top())
										{
											if (m_doc->Items->at(ga)->ItemNr != currItem->ItemNr)
											{
												if (m_doc->m_Selection->findItem(m_doc->Items->at(ga)) == -1)
												{
													m_doc->m_Selection->addItem(m_doc->Items->at(ga), true);
												}

											}
											m_doc->Items->at(ga)->isSingleSel = false;
										}
									}
								}
							}
							else
							{
								currItem->isSingleSel = true;
								m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							}
						}
						else
						{
							m_doc->m_Selection->addItem(currItem, true);
							m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
						}
					}
					else
					{
						if (m_doc->m_Selection->count() > 1)
						{
							PageItem *bb = m_doc->m_Selection->itemAt(0);
							m_doc->m_Selection->removeItem(currItem);
							m_doc->m_Selection->prependItem(currItem);
							m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							m_view->updateContents(bb->getRedrawBounding(m_canvas->scale()));
						}
					}
					m_doc->m_Selection->setIsGUISelection(true);
					m_doc->m_Selection->connectItemToGUI();
					if (m_doc->m_Selection->count() > 1)
					{
						for (int aa = 0; aa < m_doc->m_Selection->count(); ++aa)
						{
							PageItem *bb = m_doc->m_Selection->itemAt(aa);
							m_view->updateContents(bb->getRedrawBounding(m_canvas->scale()));
						}
						m_doc->m_Selection->setGroupRect();
						double x, y, w, h;
						m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
//						emit ItemPos(x, y);
//						emit ItemGeom(w, h);
//						m_view->getGroupRectScreen(&x, &y, &w, &h);
//						m_view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
//						emit HaveSel(currItem->itemType());
					}
					else
					{
						currItem->emitAllToGUI();
					}
					if (!currItem->ChangedMasterItem)
					{
						currItem->setXYPos(OldX, OldY);
					}
					return true;
				}
				m_doc->m_Selection->setIsGUISelection(true);
				m_doc->m_Selection->connectItemToGUI();
				if (!currItem->ChangedMasterItem)
				{
					currItem->setXYPos(OldX, OldY);
				}
			}
			currNr--;
			currItem = m_doc->currentPage()->FromMaster.at(currNr);
		}
	}
	if (m_doc->Items->count() == 0)
		return false;
	if ((m_doc->m_Selection->count() != 0) && (m->modifiers() == SELECT_TOGGLE))
		currItem = m_doc->m_Selection->itemAt(0);
	else
		currItem = m_doc->Items->at(m_doc->Items->count()-1);
	if ((m->modifiers() == SELECT_BENEATH) && (m_doc->m_Selection->count() != 0))
	{
		int currNr = m_doc->Items->count();
		for (a = 0; a < m_doc->Items->count(); ++a)
		{
			if (currItem->isSelected())
			{
				if (currItem->ItemNr == 0)
				{
					currItem = m_doc->Items->at(m_doc->Items->count()-1);
					break;
				}
				currNr--;
				currItem = m_doc->Items->at(currNr);
				break;
			}
			currNr--;
			currItem = m_doc->Items->at(currNr);
		}
	}
	m_doc->m_Selection->setIsGUISelection(false);
	if (currItem == NULL)
	{
		m_doc->m_Selection->setIsGUISelection(true);
		m_doc->m_Selection->connectItemToGUI();
		m_view->Deselect(true);
		return false;
	}
*/
	//Where all basic selection occurs having found the click location and the current page
/*
	int currNr = m_doc->Items->indexOf(currItem);
	for (a = currNr; a > -1; a--)
	{
		currItem = m_doc->Items->at(a);
		if ((m_doc->masterPageMode())  && (!((currItem->OwnPage == -1) || (currItem->OwnPage == static_cast<int>(m_doc->currentPage()->pageNr())))))
			continue;
		if ((currItem->LayerNr == m_doc->activeLayer()) && (!m_doc->layerLocked(currItem->LayerNr)))
		{
			p = QMatrix();
			m_canvas->Transform(currItem, p);
			if ((QRegion(p.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo.toRect())) ||
			        (QRegion(currItem->Clip * p).contains(mpo.toRect())))
			{
				//If the clicked on item is not tagged as selected
				if (!currItem->isSelected())
				{
					if ((m->modifiers() != SELECT_MULTIPLE) || (m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames))
						m_view->Deselect(false);
					//If we are selecting an item that is part of a group...
					if (currItem->Groups.count() != 0)
					{
						if (m_doc->m_Selection->count() != 0)
						{
							if (m_doc->m_Selection->findItem(currItem) == -1)
								m_doc->m_Selection->addItem(currItem, true);
						}
						else
							m_doc->m_Selection->addItem(currItem, true);
						//CB This is where we add the items of an unselected group
						if (m->modifiers() != SELECT_IN_GROUP)
						{
							for (int ga=0; ga<m_doc->Items->count(); ++ga)
							{
								if (m_doc->Items->at(ga)->Groups.count() != 0)
								{
									if (m_doc->Items->at(ga)->Groups.top() == currItem->Groups.top())
									{
										if (m_doc->Items->at(ga)->ItemNr != currItem->ItemNr)
										{
											if (m_doc->m_Selection->findItem(m_doc->Items->at(ga)) == -1)
												m_doc->m_Selection->addItem(m_doc->Items->at(ga), true);
										}
										m_doc->Items->at(ga)->isSingleSel = false;
									}
								}
							}
						}
						else
						{
							currItem->isSingleSel = true;
							m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
						}
					}
					else
					//If we are just selecting one item
					{
						//CB 301206 We shouldnt be ignoring the GUI here...
						//m_doc->m_Selection->addItem(currItem, true);
						m_doc->m_Selection->addItem(currItem);
						m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
					}
				}
				else //If the clicked on item is tagged as selected
				{
					if (m_doc->m_Selection->count() > 1)
					{
						PageItem *bb = m_doc->m_Selection->itemAt(0);
						m_doc->m_Selection->removeItem(currItem);
						m_doc->m_Selection->prependItem(currItem);
						m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
						//CB dont think we need to paint here when we paint below
						//CB With the change of 301206, perhaps we need to?
						m_view->updateContents(bb->getRedrawBounding(m_canvas->scale()));
					}
				}
				m_doc->m_Selection->setIsGUISelection(true);
				m_doc->m_Selection->connectItemToGUI();
				if (m_doc->m_Selection->count() > 1)
				{
					for (int aa = 0; aa < m_doc->m_Selection->count(); ++aa)
					{
						PageItem *bb = m_doc->m_Selection->itemAt(aa);
						m_view->updateContents(bb->getRedrawBounding(m_canvas->scale()));
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
//CB 301206 Unsure why we need this if the above is no longer ignoring the GUI
// 				else
// 				{
// 					m_doc->m_Selection->connectItemToGUI();
// 					//CB Dont need this as creating the 0th selection does this
// 					currItem->paintObj();
// 				}
				if (m_doc->m_Selection->count() == 1)
				{
					frameResizeHandle = m_canvas->frameHitTest(mousePointDoc, currItem); // HandleSizer(currItem, mpo.toRect(), m);
					if ((frameResizeHandle == 0) && (!currItem->locked()))
						qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
				}
				else
				{
					qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
					m_canvas->m_viewMode.operItemResizing = false;
				}
				return true;
			}
		}
	}
 */
	if ((m_doc->guidesSettings.guidesShown) && (m_doc->appMode == modeNormal) && (!m_doc->GuideLock) && (m_doc->OnPage(MxpS, MypS) != -1))
	{
		if (!guideMoveGesture)
			guideMoveGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
		if (guideMoveGesture->mouseHitsGuide(mousePointDoc))
		{
			m_view->startGesture(guideMoveGesture);
			guideMoveGesture->mouseMoveEvent(m);
			m_doc->m_Selection->setIsGUISelection(true);
			m_doc->m_Selection->connectItemToGUI();
			return true;
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
	m_doc->m_Selection->setIsGUISelection(true);
	m_doc->m_Selection->connectItemToGUI();
	if ( !(m->modifiers() & SELECT_MULTIPLE) || (m_doc->appMode == modeLinkFrames) || (m_doc->appMode == modeUnlinkFrames))
		m_view->Deselect(true);
	return false;
}

void LegacyMode::SetupDraw(int nr)
{
	PageItem* currItem = m_doc->Items->at(nr);
	//	currItem->setFont(Doc->toolSettings.defFont);
	//	currItem->setFontSize(Doc->toolSettings.defSize);
	m_canvas->m_viewMode.operItemResizing = true;
	frameResizeHandle = 1;
	qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
	m_doc->m_Selection->setIsGUISelection(true);
	m_doc->m_Selection->clear();
	m_doc->m_Selection->addItem(currItem);
	m_doc->m_Selection->connectItemToGUI();
//	m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
	m_canvas->m_viewMode.operItemMoving = true;
	m_doc->appMode = modeNormal; // ???
//	emit DocChanged();
	currItem->Sizing =  currItem->asLine() ? false : true;
	inItemCreation = true;
	m_canvas->setRenderModeFillBuffer();
	m_view->resetMoveTimer();
}

void LegacyMode::SetupDrawNoResize(int nr)
{
	PageItem* currItem = m_doc->Items->at(nr);
	//	currItem->setFont(Doc->toolSettings.defFont);
	//	currItem->setFontSize(Doc->toolSettings.defSize);
	m_doc->m_Selection->setIsGUISelection(true);
	m_doc->m_Selection->clear();
	m_doc->m_Selection->addItem(currItem);
	m_doc->m_Selection->connectItemToGUI();
//	m_view->updateContents(currItem->getRedrawBounding(m_canvas->scale()));
	if (!PrefsManager::instance()->appPrefs.stickyTools)
		m_view->requestMode(modeNormal);
//	emit DocChanged();
	currItem->Sizing =  currItem->asLine() ? false : true;
	inItemCreation = false;
	m_view->resetMoveTimer();
}


//CB Fix item->old* stuff
int LegacyMode::HandleSizer(PageItem *currItem, QRect mpo, QMouseEvent *m)
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


void LegacyMode::setResizeCursor(int how)
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

void LegacyMode::importToPage()
{
	QString fileName;
	QString formats = "Scribus Objects (*.sce *.SCE);;";
	formats += "Dia Shapes (*.shape *.SHAPE);;";
	formats += "Kivio Stencils (*.sml *.SML);;";
	int fmtCode = FORMATID_ODGIMPORT;
	const FileFormat *fmt = LoadSavePlugin::getFormatById(fmtCode);
	while (fmt != 0)
	{
		if (fmt->load)
			formats += fmt->filter + ";;";
		fmtCode++;
		fmt = LoadSavePlugin::getFormatById(fmtCode);
	}
	formats += tr("All Files (*)");
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("pastefile", ".");
	CustomFDialog dia(m_view, wdir, tr("Open"), formats, fdExistingFiles);
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
				m_view->moveGroup(Mxp - x2, Myp - y2);
				m_ScMW->propertiesPalette->updateColorList();
				m_ScMW->propertiesPalette->paraStyleCombo->updateFormatList();
				m_ScMW->propertiesPalette->charStyleCombo->updateFormatList();
				m_ScMW->propertiesPalette->SetLineFormats(m_doc);
			}
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

void LegacyMode::createContextMenu(PageItem* currItem, double mx, double my)
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
