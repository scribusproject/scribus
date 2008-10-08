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

#include "canvasmode_nodeedit.h"

#include <QMenu>
#include <QMouseEvent>

#include "canvas.h"
#include "canvasgesture_rectselect.h"
#include "fpoint.h"
#include "pageitem.h"
#include "pageselector.h"
#include "scraction.h"
#include "scrapbookpalette.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribusXml.h"
#include "selection.h"
#include "undomanager.h"
#include "util_icon.h"
#include "util_math.h"


CanvasMode_NodeEdit::CanvasMode_NodeEdit(ScribusView* view) : CanvasMode(view), m_rectangleSelect(NULL)
{
//	SeRx = -1; // selected pos
//	SeRy = -1;
	Mxp = -1;  // last mouse position
	Myp = -1;
	Dxp = -1;  // last mouse press position for rectangle select
	Dyp = -1;
	GxM = -1;  // guide position
	GyM = -1;
	MoveGX = MoveGY = false;
	m_ScMW = m_view->m_ScMW;
}



void CanvasMode_NodeEdit::drawControls(QPainter* p) 
{
//	qDebug() << "NodeEdit::drawControls";
//	void Canvas::MarkClip(QPainter *p, PageItem *currItem, FPointArray cli, bool)
	double x, y;
	if (m_doc->m_Selection->count() == 0)
		return;
	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	FPointArray cli;
	
	p->save();
	p->scale(m_canvas->scale(), m_canvas->scale());
	p->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	p->translate(currItem->xPos(), currItem->yPos());
	p->rotate(currItem->rotation());			
	p->setPen(QPen(Qt::blue, 1 / m_canvas->m_viewMode.scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p->setBrush(Qt::NoBrush);

	if ((m_doc->nodeEdit.isContourLine) && (currItem->ContourLine.size() != 0))
		cli = currItem->ContourLine;
	else
		cli = currItem->PoLine;
	// draw curve
	const double scale = m_canvas->m_viewMode.scale;
	const double onePerScale = 1 / scale;
	if (cli.size() > 3)
	{
		for (uint poi=0; poi<cli.size()-3; poi += 4)
		{
			if (cli.point(poi).x() > 900000)
				continue;
			p->setPen(QPen(Qt::blue, onePerScale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			FPoint a1 = cli.point(poi);
			FPoint a2 = cli.point(poi+1);
			FPoint a3 = cli.point(poi+3);
			FPoint a4 =	cli.point(poi+2);
			QPainterPath Bez;
			Bez.moveTo(a1.x(), a1.y());
			Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
			p->drawPath(Bez);
			p->setPen(QPen(Qt::blue, onePerScale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
			p->drawLine(QPointF(a1.x(), a1.y()), QPointF(a2.x(), a2.y()));
			p->drawLine(QPointF(a3.x(), a3.y()), QPointF(a4.x(), a4.y()));
		}
	}
	// draw points
	for (uint a=0; a<cli.size()-1; a += 2)
	{
		if (cli.point(a).x() > 900000)
			continue;
		if (m_doc->nodeEdit.EdPoints)
		{
			if (m_doc->nodeEdit.ClRe == static_cast<int>(a+1))
				p->setPen(QPen(Qt::red, 8 / scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::magenta, 8 / scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a+1, &x, &y);
			p->drawPoint(QPointF(x, y));
			if (m_doc->nodeEdit.ClRe == static_cast<int>(a))
				p->setPen(QPen(Qt::red, 8 / scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::blue, 8 / scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a, &x, &y);
			p->drawPoint(QPointF(x, y));
		}
		else
		{
			if (m_doc->nodeEdit.ClRe == static_cast<int>(a))
				p->setPen(QPen(Qt::red, 8 / scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::blue, 8 / scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a, &x, &y);
			p->drawPoint(QPointF(x, y));
			if (m_doc->nodeEdit.ClRe == static_cast<int>(a+1))
				p->setPen(QPen(Qt::red, 8 / scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::magenta, 8 / scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a+1, &x, &y);
			p->drawPoint(QPointF(x, y));
		}
	}
	
	if (m_doc->nodeEdit.ClRe != -1)
	{
		p->setPen(QPen(Qt::red, 8 / scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
		cli.point(m_doc->nodeEdit.ClRe, &x, &y);
		p->drawPoint(QPointF(x, y));
		QList<int>::Iterator itm;
		for (itm = m_doc->nodeEdit.SelNode.begin(); itm != m_doc->nodeEdit.SelNode.end(); ++itm)
		{
			cli.point((*itm), &x, &y);
			p->drawPoint(QPointF(x, y));
		}
		emit m_view->HavePoint(true, m_doc->nodeEdit.MoveSym);
		cli.point(m_doc->nodeEdit.ClRe, &x, &y);
		emit m_view->ClipPo(x, y);
	}
	else
		emit m_view->HavePoint(false, m_doc->nodeEdit.MoveSym);
	p->restore();
}


void CanvasMode_NodeEdit::activate(bool fromGesture)
{
	if (fromGesture && m_rectangleSelect)
	{
		m_canvas->m_viewMode.m_MouseButtonPressed = false;
		// handle rectangle select
//	double sc = m_canvas->scale();
		PageItem* currItem = m_doc->m_Selection->itemAt(0);
		m_doc->nodeEdit.SelNode.clear();
		QRectF Sele = m_rectangleSelect->result(); //QRect(Dxp, Dyp, SeRx-Dxp, SeRy-Dyp).normalized();
		FPointArray Clip;
		bool firstPoint = false;
//	m_canvas->setScale(1.0);
		if (m_doc->nodeEdit.isContourLine)
			Clip = currItem->ContourLine;
		else
			Clip = currItem->PoLine;
		for (uint a = 0; a < Clip.size(); ++a)
		{
			if (Clip.point(a).x() > 900000)
				continue;
			FPoint np = Clip.point(a);
			FPoint npf2 = np.transformPoint(currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0, false);
			if ((Sele.contains(npf2.x(), npf2.y())) && ((a == 0) || (((a-2) % 4) == 0)))
			{
				if (a == 0)
					firstPoint = true;
				if ((firstPoint) && (a == Clip.size() - 2) && (!currItem->asPolyLine()))
					continue;
				m_doc->nodeEdit.ClRe = a;
				m_doc->nodeEdit.SelNode.append(a);
				m_doc->nodeEdit.update(Clip.pointQF(a));
			}
		}
//	m_canvas->setScale(sc);
//	m_view->HaveSelRect = false;
//	m_view->redrawMarker->hide();
		currItem->update();
//		m_view->updateContents();
		m_doc->nodeEdit.finishTransaction(currItem);
		delete m_rectangleSelect;
		m_rectangleSelect = NULL;
	}
	else
	{
		Mxp = -1;  // last mouse position
		Myp = -1;
		Dxp = -1;  // last mouse press position for rectangle select
		Dyp = -1;
		GxM = -1;  // guide position
		GyM = -1;
		MoveGX = MoveGY = false;
	}		
}


void CanvasMode_NodeEdit::deactivate(bool forGesture)
{
	if (!forGesture && m_rectangleSelect)
	{
		m_rectangleSelect->clear();
		delete m_rectangleSelect;
		m_rectangleSelect = NULL;
	}
}


inline bool CanvasMode_NodeEdit::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != NULL; 
}



void CanvasMode_NodeEdit::enterEvent(QEvent *)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
}


void CanvasMode_NodeEdit::leaveEvent(QEvent *e)
{
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}



void CanvasMode_NodeEdit::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m_view->stopDragTimer();
	m_view->requestMode(submodeEndNodeEdit);
}


void CanvasMode_NodeEdit::mouseMoveEvent(QMouseEvent *m)
{
	PageItem *currItem;
	QPoint np, np2, mop;
	FPoint npf, npf2;
	double sc = m_canvas->scale();
	m->accept();
	npf = m_canvas->globalToCanvas(m->globalPos());
#ifdef MODEDEBUG
//	qDebug() << "nodedit move event:" << m->x() << m->y() << "canvas:" << npf.x() << npf.y() << "@" << m_canvas->scale();
#endif
/*	if (m_doc->guidesSettings.guidesShown)
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
//	if (m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
//	{
//		m_ScMW->setAppMode(modePanning);
//	}
	if (GetItem(&currItem))
	{
		if (m_doc->DragP)
			return;
		//Operations run here:
		//Item resize, esp after creating a new one
		if (m_view->moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed 
			&& (m->buttons() & Qt::LeftButton) &&  (!currItem->locked()))
		{
#ifdef MODEDEBUG
//			qDebug() << "node edit drag 1";
#endif
			m_view->stopDragTimer();
			handleNodeEditDrag(m, currItem);
		}
		else if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		{
			if (!currItem->locked())
			{
				QMatrix p; // = currItem->getTransform();
				QRect mpo; // = QRect(m->globalX()-m_doc->guidesSettings.grabRad, m->globalY()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
				//				mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
				handleNodeEditMove(m, mpo, currItem, p);
			}
		}
//		else
//			qDebug() << "node edit drag 9:" << (m->buttons() & Qt::LeftButton);

	}
	else // shouldnt be in nodeedit mode now, should it??
	{
		npf = m_canvas->globalToCanvas(m->globalPos());
		if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton) && (GyM == -1) && (GxM == -1))
		{
//			SeRx = qRound(npf.x());
//			SeRy = qRound(npf.y());
//			m_view->redrawMarker->setGeometry(QRect(Mxp, Myp, m->x() - Mxp, m->y() - Myp).normalized());
//			if (!m_view->redrawMarker->isVisible())
//				m_view->redrawMarker->show();
//			m_view->HaveSelRect = true;
			if (!m_rectangleSelect)
			{
				m_rectangleSelect = new RectSelect(this);
			}
			m_rectangleSelect->prepare(m->globalPos());
			m_view->startGesture(m_rectangleSelect);
			return;
		}
		if ((m_doc->guidesSettings.guidesShown) && (!m_doc->GuideLock) 
			&& (m_doc->OnPage(npf.x(), npf.y()) != -1))
		{
			Guides::iterator it;
			Guides tmpGuides = m_doc->currentPage()->guides.horizontals(GuideManagerCore::Standard);
			for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it)
			{
				if ( (*it) + m_doc->currentPage()->yOffset() < npf.y() + m_doc->guidesSettings.grabRad * sc &&
					 (*it) + m_doc->currentPage()->yOffset() > npf.y() - m_doc->guidesSettings.grabRad * sc )
				{
					if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (GyM != -1))
						MoveGY = true;
					if (npf.x() < m_doc->currentPage()->xOffset() || 
						npf.x() >= m_doc->currentPage()->xOffset() + m_doc->currentPage()->width() - 1)
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					else
						qApp->changeOverrideCursor(QCursor(Qt::SplitVCursor));
					return;
				}
			}
//			qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
			tmpGuides = m_doc->currentPage()->guides.verticals(GuideManagerCore::Standard);
			for (it = tmpGuides.begin(); it!= tmpGuides.end(); ++it)
			{
				if ( (*it) + m_doc->currentPage()->xOffset() < npf.x() + m_doc->guidesSettings.grabRad * sc &&
					 (*it) + m_doc->currentPage()->xOffset() > npf.x() - m_doc->guidesSettings.grabRad * sc)
				{
					if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (GxM != -1))
						MoveGX = true;
					if (npf.y() < m_doc->currentPage()->yOffset() || 
						npf.y() >= m_doc->currentPage()->yOffset() + m_doc->currentPage()->height() - 1)
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					else
						qApp->changeOverrideCursor(QCursor(Qt::SplitHCursor));
					return;
				}
			}
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		}
	}
}

void CanvasMode_NodeEdit::mousePressEvent(QMouseEvent *m)
{
//	double Rxp = 0;
//	double Ryp = 0;
//	double Rxpd = 0;
//	double Rypd = 0;
	QPainter p;
//	m_canvas->PaintSizeRect(QRect());
//	FPoint npf, npf2;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
//	m_view->HaveSelRect = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	MoveGX = MoveGY = false;
//	oldClip = 0;
	m->accept();
	m_view->registerMousePress(m->globalPos());
//	npf = m_canvas->globalToCanvas(m->globalPos());
//	Mxp = qRound(npf.x()); // WTF? FIXME:av
//	Myp = qRound(npf.y());
	QRect mpo; //(m->globalX()-m_doc->guidesSettings.grabRad, m->globalY()-m_doc->guidesSettings.grabRad, m_doc->guidesSettings.grabRad*2, m_doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
/*	if (m_doc->appMode != modeEdit)
	{
		Rxp = m_doc->ApplyGrid(QPoint(Mxp, Myp)).x();
		Rxpd = Mxp - Rxp;
		Mxp = qRound(Rxp);
		Ryp = m_doc->ApplyGrid(QPoint(Mxp, Myp)).y();
		Rypd = Myp - Ryp;
		Myp = qRound(Ryp);
	}*/
//	SeRx = Mxp;
//	SeRy = Myp;
/*	if (m->button() == Qt::MidButton)
	{
		m_view->MidButt = true;
		m_view->DrawNew();
		return;
	}*/
//	npf = m_canvas->globalToCanvas(m->globalPos());
//	Mxp = qRound(npf.x()); // WTF?
//	Myp = qRound(npf.y());
//	SeRx = Mxp;
//	SeRy = Myp;
	
	// this has to be:
	Mxp = m->x();
	Myp = m->y();
	
	if (m_doc->m_Selection->count() != 0)
	{
		handleNodeEditPress(m, mpo);
	}			
	if ((m_doc->m_Selection->count() != 0) && (m->button() == Qt::RightButton))
	{
		m_canvas->m_viewMode.m_MouseButtonPressed = true;
		Dxp = Mxp;
		Dyp = Myp;
	}
}



void CanvasMode_NodeEdit::mouseReleaseEvent(QMouseEvent *m)
{
//	const double mouseX = m->globalX();
//	const double mouseY = m->globalY();
	const FPoint mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
	
//	QMenu* pmen3 = NULL;
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	m_view->stopDragTimer();
/*	if (m_doc->guidesSettings.guidesShown)
	{
		bool foundGuide = false;
		FPoint np = m_canvas->globalToCanvas(m->globalPos());
		double nx = np.x();
		double ny = np.y();
		double grabRadScale = m_doc->guidesSettings.grabRad / m_canvas->scale();
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
	/* see activated()
	if (!m_doc->nodeEdit.hasNodeSelected() && (m_view->HaveSelRect))
	{
		double sc = m_canvas->scale();
		currItem = m_doc->m_Selection->itemAt(0);
		m_doc->nodeEdit.SelNode.clear();
		QRect Sele = QRect(Dxp, Dyp, SeRx-Dxp, SeRy-Dyp).normalized();
		FPointArray Clip;
		m_canvas->setScale(1.0);
		if (m_doc->nodeEdit.isContourLine)
			Clip = currItem->ContourLine;
		else
			Clip = currItem->PoLine;
		for (uint a = 0; a < Clip.size(); ++a)
		{
			if (Clip.point(a).x() > 900000)
				continue;
			FPoint np = Clip.point(a);
			FPoint npf2 = np.transformPoint(currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0, false);
			QPoint npf = QPoint(qRound(npf2.x()), qRound(npf2.y()));
			if ((Sele.contains(npf)) && ((a == 0) || (((a-2) % 4) == 0)))
			{
				m_doc->nodeEdit.ClRe = a;
				m_doc->nodeEdit.SelNode.append(a);
				m_doc->nodeEdit.update(QPointF(Clip.point(a).x(), Clip.point(a).y()));
			}
		}
		m_canvas->setScale(sc);
		m_view->HaveSelRect = false;
		m_view->redrawMarker->hide();
		currItem->update();
		m_doc->nodeEdit.finishTransaction(currItem);
		return;
	}
	*/
/*	if (m_view->moveTimerElapsed() && (m_doc->appMode == modeEditClip) && (SegP1 == -1) && (SegP2 == -1))
	{
		currItem = m_doc->m_Selection->itemAt(0);
		if (operItemMoving)
		{
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			double nx = m->x()/m_canvas->scale() + m_doc->minCanvasCoordinate.x();
			double ny = m->y()/m_canvas->scale() + m_doc->minCanvasCoordinate.y();
			if (!m_doc->ApplyGuides(&nx, &ny))
			{
				FPoint npg(ApplyGridF(FPoint(nx, ny)));
				nx = npg.x();
				ny = npg.y();
			}
			FPoint np(nx, ny, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
			m_doc->nodeEdit.moveClipPoint(currItem, np);
		}
		m_doc->AdjustItemSize(currItem);
		emit DocChanged();
		updateContents();
		operItemMoving = false;
		return;
	}
	if (m_view->moveTimerElapsed() && (m_doc->appMode == modeEditClip) && (SegP1 != -1) && (SegP2 != -1)) */
	if (m_view->moveTimerElapsed())
	{
		m_view->stopDragTimer();
		if (m_doc->nodeEdit.SegP1 != -1 && m_doc->nodeEdit.SegP2 != -1)
		{
			m_doc->nodeEdit.deselect();
			m_doc->nodeEdit.SegP1 = -1;
			m_doc->nodeEdit.SegP2 = -1;
		}
		currItem = m_doc->m_Selection->itemAt(0);
		m_canvas->m_viewMode.operItemMoving = false;
		double xposOrig = currItem->xPos();
		double yposOrig = currItem->yPos();

		ItemState<QPair<FPointArray, FPointArray> > *state = NULL;
		
		state = m_doc->nodeEdit.finishTransaction1(currItem);
		xposOrig = currItem->xPos();
		yposOrig = currItem->yPos();
		if (m_doc->nodeEdit.hasNodeSelected() && (m_doc->nodeEdit.SelNode.count() == 1))
		{
			//FIXME:av
			FPoint newP = m_canvas->globalToCanvas(m->globalPos());
//			double newX = newP.x();
//			double newY = newP.y();
//			FPoint np(newX, newY, 0, 0, currItem->rotation(), 1, 1, true);
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			FPointArray Clip;
			if (m_doc->nodeEdit.isContourLine)
				Clip = currItem->ContourLine;
			else
				Clip = currItem->PoLine;
//			FPoint npf = FPoint(Clip.point(m_doc->nodeEdit.ClRe).x() + np.x() / m_canvas->scale(), Clip.point(m_doc->nodeEdit.ClRe).y() + np.y() / m_canvas->scale());
//			double nx = npf.x();
//			double ny = npf.y();
//			nx += currItem->xPos();
//			ny += currItem->yPos();
			FPoint npf;
			double nx = newP.x();
			double ny = newP.y();
			if (!m_doc->ApplyGuides(&nx, &ny))
				npf = m_doc->ApplyGridF(FPoint(nx, ny));
			else
				npf = FPoint(nx, ny);
//			npf = FPoint(npf.x() - currItem->xPos(), npf.y() - currItem->yPos());
			npf = FPoint(npf.x(), npf.y(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
			m_doc->nodeEdit.moveClipPoint(currItem, npf);
		}

		m_doc->AdjustItemSize(currItem);
		if (!m_doc->nodeEdit.isContourLine)
			currItem->ContourLine.translate(xposOrig - currItem->xPos(), yposOrig - currItem->yPos());
//		currItem->update();
		m_doc->regionsChanged()->update(QRectF());
//		emit DocChanged();
//		m_view->updateContents();
		if (state)
		{
			m_doc->nodeEdit.finishTransaction2(currItem, state);
		}
		return;
	}
/* 
	if ((!GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!m_doc->DragP) && (m_doc->appMode == modeNormal))
	{
		QMenu *pmen = new QMenu();
		if ((m_ScMW->Buffer2.startsWith("<SCRIBUSELEM")) || (m_ScMW->Buffer2.contains("<SCRIBUSFRAGMENT")) || (m_ScMW->scrapbookPalette->tempBView->objectMap.count() > 0))
		{
			Mxp = m->x();
			Myp = m->y();
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
	}
*/
	m_canvas->setRenderModeUseBuffer(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	m_view->MidButt = false;
//	m_doc->SubMode = -1;
	if (m_view->groupTransactionStarted())
	{
		for (int i = 0; i < m_doc->m_Selection->count(); ++i)
			m_doc->m_Selection->itemAt(i)->checkChanges(true);
		m_view->endGroupTransaction();
	}

//???	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
//??? av		m_doc->m_Selection->itemAt(i)->checkChanges(true);

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



void CanvasMode_NodeEdit::handleNodeEditPress(QMouseEvent* m, QRect)
{
	FPoint npf2;
	
	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	FPointArray Clip = m_doc->nodeEdit.beginTransaction(currItem);
	bool edited = false;
	bool pfound = false;
//	QMatrix pm;
//	pm.translate(-m_doc->minCanvasCoordinate.x()*m_canvas->scale(), -m_doc->minCanvasCoordinate.y()*m_canvas->scale());
//	m_canvas->Transform(currItem, pm);
//	npf2 = FPoint(m->pos() * pm.inverted());
	npf2 = m_canvas->globalToCanvas(m->globalPos()).transformPoint(currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0, true);
	QMatrix pm2 = currItem->getTransform();
	for (int a=0; a < signed(Clip.size()); ++a)
	{
		if (((m_doc->nodeEdit.EdPoints) && (a % 2 != 0)) || ((!m_doc->nodeEdit.EdPoints) && (a % 2 == 0)))
				continue;
		QPointF npf = pm2.map(Clip.pointQF(a));
//			QRect tx = QRect(static_cast<int>(npf.x()-3), static_cast<int>(npf.y()-3), 6, 6);
//			if (tx.intersects(mpo))
		if (m_canvas->hitsCanvasPoint(m->globalPos(), npf))
		{
#ifdef MODEDEBUG
			qDebug() << "nodeedit: selecting point" << a << "at" << npf.x() << npf.y();
#endif
			m_doc->nodeEdit.ClRe = a;
			if ((m_doc->nodeEdit.EdPoints) && (m_doc->nodeEdit.SelNode.contains(a) == 0))
			{
				if (m->modifiers() == Qt::ShiftModifier)
					m_doc->nodeEdit.SelNode.append(a);
				else
				{
					m_doc->nodeEdit.SelNode.clear();
					m_doc->nodeEdit.SelNode.append(a);
				}
			}
			m_doc->nodeEdit.update(Clip.pointQF(a));
			pfound = true;
			edited = true;
			break;
		}
	}
	if ((!pfound) || (!m_doc->nodeEdit.EdPoints))
		m_doc->nodeEdit.SelNode.clear();

	if ((m_doc->nodeEdit.submode == NodeEditContext::MOVE_POINT) && (m_doc->nodeEdit.ClRe2 != -1) && !m_doc->nodeEdit.hasNodeSelected())
	{
		m_doc->nodeEdit.SegP1 = m_doc->nodeEdit.ClRe2;
		m_doc->nodeEdit.SegP2 = m_doc->nodeEdit.ClRe2+2;
		m_doc->nodeEdit.ClRe = m_doc->nodeEdit.ClRe2;
	}
	FPointArray cli;
	uint EndInd = Clip.size();
	uint StartInd = 0;
	for (uint n = m_doc->nodeEdit.ClRe; n < Clip.size(); ++n)
	{
		if (Clip.point(n).x() > 900000)
		{
			EndInd = n;
			break;
		}
	}
	if (m_doc->nodeEdit.ClRe > 0)
	{
		for (uint n2 = m_doc->nodeEdit.ClRe; n2 > 0; n2--)
		{
			if (n2 == 0)
				break;
			if (Clip.point(n2).x() > 900000)
			{
				StartInd = n2 + 1;
				break;
			}
		}
	}
	if (m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH)
	{
		if (!m_doc->nodeEdit.EdPoints)
			return;
		if (!m_doc->nodeEdit.hasNodeSelected())	// We don't have a Point, try to add one onto the current curve segment
		{
			bool foundP = false;
			uint seg = 0;
			double absDist = 9999999999.9;
			FPoint point = FPoint(0, 0);
			FPoint normal = FPoint(0, 0);
			FPoint tangent = FPoint(0, 0);
			FPoint nearPoint = FPoint(0, 0);
			double nearT = 0.0;
			QRect mpo2(0, 0, m_doc->guidesSettings.grabRad*3, m_doc->guidesSettings.grabRad*3);
			mpo2.moveCenter(QPoint(qRound(npf2.x()), qRound(npf2.y())));
			for (uint poi=0; poi<Clip.size()-3; poi += 4)
			{
				FPoint a1 = Clip.point(poi);
				FPoint a2 = Clip.point(poi+1);
				FPoint a3 = Clip.point(poi+3);
				FPoint a4 = Clip.point(poi+2);
				QPainterPath Bez;
				Bez.moveTo(a1.x(), a1.y());
				Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
				QPolygon cli2 = Bez.toFillPolygon().toPolygon();
				for (int clp = 0; clp < cli2.size()-1; ++clp)
				{
					if (m_view->PointOnLine(cli2.point(clp), cli2.point(clp+1), mpo2))
					{
						seg = poi;
						double sp = 0.0;
						double spadd = 1.0 / (Clip.lenPathSeg(seg) * m_canvas->scale());
						while (sp < 1.0)
						{
							Clip.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
							double d1 = fabs(sqrt(pow(point.x() - npf2.x(), 2) + pow(point.y() - npf2.y() ,2)));
							if (d1 < absDist)
							{
								foundP = true;
								nearPoint = point;
								nearT = sp;
								absDist = d1;
							}
							sp += spadd;
						}
					}
				}
			}
			cli.putPoints(0, m_doc->nodeEdit.ClRe2+2, Clip);
			if (foundP)
			{
				npf2 = nearPoint;
				FPoint base = cli.point(cli.size()-2);
				FPoint c1 = cli.point(cli.size()-1);
				FPoint base2 =  Clip.point(m_doc->nodeEdit.ClRe2+2);
				FPoint c2 = Clip.point(m_doc->nodeEdit.ClRe2+3);
				if ((base == c1) && (base2 == c2))
				{
					cli.resize(cli.size()+4);
					cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
					cli.putPoints(cli.size(), Clip.size()-(m_doc->nodeEdit.ClRe2 + 2), Clip, m_doc->nodeEdit.ClRe2+2);
				}
				else
				{
					FPoint cn1 = (1.0 - nearT) * base + nearT * c1;
					FPoint cn2 = (1.0 - nearT) * cn1 + nearT * ((1.0 - nearT) * c1 + nearT * c2);
					FPoint cn3 = (1.0 - nearT) * ((1.0 - nearT) * c1 + nearT * c2) + nearT * ((1.0 - nearT) * c2 + nearT * base2);
					FPoint cn4 = (1.0 - nearT) * c2 + nearT * base2;
					cli.setPoint(cli.size()-1, cn1);
					cli.resize(cli.size()+4);
					uint basind = cli.size()+1;
					cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), cn2.x(), cn2.y(), npf2.x(), npf2.y(), cn3.x(), cn3.y());
					cli.putPoints(cli.size(), Clip.size()-(m_doc->nodeEdit.ClRe2 + 2), Clip, m_doc->nodeEdit.ClRe2+2);
					cli.setPoint(basind, cn4);
				}
				Clip = cli.copy();
				cli.resize(0);
				m_doc->nodeEdit.ClRe = m_doc->nodeEdit.ClRe2+2;
				m_doc->nodeEdit.ClRe2 = -1;
				EndInd = Clip.size();
				StartInd = 0;
				for (uint n = m_doc->nodeEdit.ClRe; n < Clip.size(); ++n)
				{
					if (Clip.point(n).x() > 900000)
					{
						EndInd = n;
						break;
					}
				}
				if (m_doc->nodeEdit.ClRe > 0)
				{
					for (uint n2 = m_doc->nodeEdit.ClRe; n2 > 0; n2--)
					{
						if (n2 == 0)
							break;
						if (Clip.point(n2).x() > 900000)
						{
							StartInd = n2 + 1;
							break;
						}
					}
				}
			}
			else
				m_doc->nodeEdit.deselect();
		}
		if (m_doc->nodeEdit.hasNodeSelected())
		{
			if (currItem->asPolygon())
			{
				if ((m_doc->nodeEdit.ClRe != 0) && (m_doc->nodeEdit.ClRe != static_cast<int>(EndInd-2)))
				{
					if (currItem->Segments.count() == 0)
					{
						cli.putPoints(0, EndInd-(m_doc->nodeEdit.ClRe+2), Clip, m_doc->nodeEdit.ClRe+2);
						cli.putPoints(cli.size(), m_doc->nodeEdit.ClRe+2, Clip);
					}
					else
					{
						cli.putPoints(0, EndInd-StartInd, Clip, StartInd);
						int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor(), true);
						PageItem* bb = m_doc->Items->at(z);
						if (m_doc->nodeEdit.isContourLine)
							bb->ContourLine.resize(0);
						else
							bb->PoLine.resize(0);
						if (StartInd != 0)
						{
							if (m_doc->nodeEdit.isContourLine)
							{
								bb->ContourLine.putPoints(0, StartInd - 4, Clip);
								bb->ContourLine.putPoints(bb->ContourLine.size(), Clip.size()-EndInd, Clip, EndInd);
							}
							else
							{
								bb->PoLine.putPoints(0, StartInd - 4, Clip);
								bb->PoLine.putPoints(bb->PoLine.size(), Clip.size()-EndInd, Clip, EndInd);
							}
						}
						else
						{
							if (m_doc->nodeEdit.isContourLine)
								bb->ContourLine.putPoints(0, Clip.size()-EndInd-4, Clip, EndInd+4);
							else
								bb->PoLine.putPoints(0, Clip.size()-EndInd-4, Clip, EndInd+4);
						}
						bb->setRotation(currItem->rotation());
						m_doc->AdjustItemSize(bb);
						bb->ClipEdited = true;
						PageItem *bx = m_doc->Items->takeAt(bb->ItemNr);
						m_doc->Items->insert(bb->ItemNr-1, bx);
					}
					currItem->PoLine = cli.copy();
				}
				m_doc->nodeEdit.deselect();
				currItem->ClipEdited = true;
				edited = true;
				m_doc->nodeEdit.submode = NodeEditContext::MOVE_POINT;
				PageItem* newItem=m_doc->convertItemTo(currItem, PageItem::PolyLine);
				currItem=newItem;
				m_doc->m_Selection->clear();
				m_doc->m_Selection->addItem(currItem);
				emit m_view->PolyOpen();
			}
			else
			{
				if ((currItem->asPolyLine()) || (currItem->asPathText()))
				{
					if ((m_doc->nodeEdit.ClRe > 1) && (m_doc->nodeEdit.ClRe < static_cast<int>(Clip.size()-2)))
					{
						int z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor(), true);
						PageItem* bb = m_doc->Items->at(z);
						if (m_doc->nodeEdit.isContourLine)
							bb->ContourLine.putPoints(0, Clip.size()-(m_doc->nodeEdit.ClRe+2), Clip, m_doc->nodeEdit.ClRe+2);
						else
							bb->PoLine.putPoints(0, Clip.size()-(m_doc->nodeEdit.ClRe+2), Clip, m_doc->nodeEdit.ClRe+2);
						bb->setRotation(currItem->rotation());
						m_doc->AdjustItemSize(bb);
						bb->ClipEdited = true;
						cli.resize(0);
						cli.putPoints(0, m_doc->nodeEdit.ClRe+2, Clip);
						currItem->PoLine = cli.copy();
						m_doc->m_Selection->clear();
						m_doc->m_Selection->addItem(currItem);
					}
					m_doc->nodeEdit.deselect();
					currItem->ClipEdited = true;
					edited = true;
					m_doc->nodeEdit.submode = NodeEditContext::MOVE_POINT;
					currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2.0, 1.0)));
					emit m_view->PolyOpen();
				}
			}
		}
	}
	if ((m_doc->nodeEdit.submode == NodeEditContext::DEL_POINT) && m_doc->nodeEdit.hasNodeSelected())
	{
		if (!m_doc->nodeEdit.EdPoints)
			return;
		if ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame()))
		{
			if ((currItem->Segments.count() == 0) && (Clip.size() <= 12))  // min. 3 points
				return;
		}
		else
		{
			if (Clip.size() <= 4)
				return;
		}
		if ((currItem->Segments.count() != 0) && ((EndInd - StartInd) <= 12))
		{
			if (StartInd != 0)
				cli.putPoints(0, StartInd-4, Clip);
			cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
		}
		else
		{
			if (m_doc->nodeEdit.ClRe == static_cast<int>(StartInd))
			{
				if ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame()))
				{
					FPoint kp(Clip.point(EndInd-3));
					cli.putPoints(0, StartInd, Clip);
					cli.putPoints(cli.size(), EndInd - StartInd - 4, Clip, StartInd);
					cli.setPoint(StartInd, cli.point(cli.size()-2));
					cli.setPoint(StartInd + 1, kp);
					cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
				}
				else
				{
					cli.putPoints(0, StartInd, Clip);
					cli.putPoints(cli.size(), EndInd - StartInd - 4, Clip, StartInd+4);
					cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
				}
			}
			else
			{
				if (m_doc->nodeEdit.ClRe != 0)
					cli.putPoints(0, m_doc->nodeEdit.ClRe, Clip);
				cli.putPoints(cli.size(), Clip.size()-(m_doc->nodeEdit.ClRe + 4), Clip, m_doc->nodeEdit.ClRe+4);
			}
		}
		if (m_doc->nodeEdit.isContourLine)
			currItem->ContourLine = cli.copy();
		else
			currItem->PoLine = cli.copy();
		m_doc->nodeEdit.deselect();
		currItem->ClipEdited = true;
		edited = true;
	}
	if ((m_doc->nodeEdit.submode == NodeEditContext::ADD_POINT) && (m_doc->nodeEdit.ClRe2 != -1))
	{
		bool foundP = false;
		uint seg = 0;
		double absDist = 9999999999.9;
		FPoint point = FPoint(0, 0);
		FPoint normal = FPoint(0, 0);
		FPoint tangent = FPoint(0, 0);
		FPoint nearPoint = FPoint(0, 0);
		double nearT = 0.0;
		QRect mpo2(0, 0, m_doc->guidesSettings.grabRad*3, m_doc->guidesSettings.grabRad*3);
		mpo2.moveCenter(QPoint(qRound(npf2.x()), qRound(npf2.y())));
		for (uint poi=0; poi<Clip.size()-3; poi += 4)
		{
			FPoint a1 = Clip.point(poi);
			FPoint a2 = Clip.point(poi+1);
			FPoint a3 = Clip.point(poi+3);
			FPoint a4 = Clip.point(poi+2);
			QPainterPath Bez;
			Bez.moveTo(a1.x(), a1.y());
			Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
			QPolygon cli2 = Bez.toFillPolygon().toPolygon();
			for (int clp = 0; clp < cli2.size()-1; ++clp)
			{
				if (m_view->PointOnLine(cli2.point(clp), cli2.point(clp+1), mpo2))
				{
					seg = poi;
					double sp = 0.0;
					double spadd = 1.0 / (Clip.lenPathSeg(seg) * m_canvas->scale());
					while (sp < 1.0)
					{
						Clip.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
						double d1 = fabs(sqrt(pow(point.x() - npf2.x(), 2) + pow(point.y() - npf2.y() ,2)));
						if (d1 < absDist)
						{
							foundP = true;
							nearPoint = point;
							nearT = sp;
							absDist = d1;
						}
						sp += spadd;
					}
				}
			}
		}
		cli.putPoints(0, m_doc->nodeEdit.ClRe2+2, Clip);
		if (foundP)
		{
			npf2 = nearPoint;
			FPoint base = cli.point(cli.size()-2);
			FPoint c1 = cli.point(cli.size()-1);
			FPoint base2 =  Clip.point(m_doc->nodeEdit.ClRe2+2);
			FPoint c2 = Clip.point(m_doc->nodeEdit.ClRe2+3);
			if ((base == c1) && (base2 == c2))
			{
				cli.resize(cli.size()+4);
				cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
						cli.putPoints(cli.size(), Clip.size()-(m_doc->nodeEdit.ClRe2 + 2), Clip, m_doc->nodeEdit.ClRe2+2);
			}
			else
			{
				FPoint cn1 = (1.0 - nearT) * base + nearT * c1;
				FPoint cn2 = (1.0 - nearT) * cn1 + nearT * ((1.0 - nearT) * c1 + nearT * c2);
				FPoint cn3 = (1.0 - nearT) * ((1.0 - nearT) * c1 + nearT * c2) + nearT * ((1.0 - nearT) * c2 + nearT * base2);
				FPoint cn4 = (1.0 - nearT) * c2 + nearT * base2;
				cli.setPoint(cli.size()-1, cn1);
				cli.resize(cli.size()+4);
				uint basind = cli.size()+1;
				cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), cn2.x(), cn2.y(), npf2.x(), npf2.y(), cn3.x(), cn3.y());
				cli.putPoints(cli.size(), Clip.size()-(m_doc->nodeEdit.ClRe2 + 2), Clip, m_doc->nodeEdit.ClRe2+2);
				cli.setPoint(basind, cn4);
			}
		}
		else
		{
			cli.resize(cli.size()+4);
			cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
			cli.putPoints(cli.size(), Clip.size()-(m_doc->nodeEdit.ClRe2 + 2), Clip, m_doc->nodeEdit.ClRe2+2);
		}
		if (m_doc->nodeEdit.isContourLine)
			currItem->ContourLine = cli.copy();
		else
			currItem->PoLine = cli.copy();
		m_doc->nodeEdit.ClRe2 = -1;
		currItem->ClipEdited = true;
		edited = true;
	}
	if (edited)
	{
		currItem->FrameType = 3;
		m_doc->AdjustItemSize(currItem);
		currItem->update();
	}
	if ((m_doc->nodeEdit.SelNode.count() != 0) || ((m_doc->nodeEdit.SegP1 != -1) && (m_doc->nodeEdit.SegP2 != -1)) || (m_doc->nodeEdit.hasNodeSelected() && (!m_doc->nodeEdit.EdPoints)))
	{
		Mxp = m->x();
		Myp = m->y();
		m_canvas->setRenderModeFillBuffer();
	}
	else
	{
		/*
		m_view->redrawMarker->setGeometry(m->x(), m->y(), 1, 1);
		m_view->redrawMarker->show();
		*/
		Mxp = m->x();
		Myp = m->y();
		Dxp = qRound(m->x()/m_canvas->scale());  // + m_doc->minCanvasCoordinate.x());
		Dyp = qRound(m->y()/m_canvas->scale());  // + m_doc->minCanvasCoordinate.y());
		if (!m_rectangleSelect)
			m_rectangleSelect = new RectSelect(this);
		m_rectangleSelect->prepare(m->globalPos());
		m_view->startGesture(m_rectangleSelect);
	}
	
}



bool CanvasMode_NodeEdit::handleNodeEditMove(QMouseEvent* m, QRect, PageItem* currItem, QMatrix)
{
	QMatrix itemPos = currItem->getTransform();
	FPointArray Clip;
	m_doc->nodeEdit.ClRe2 = -1;
	m_doc->nodeEdit.SegP1 = -1;
	m_doc->nodeEdit.SegP2 = -1;
	if (m_doc->nodeEdit.isContourLine)
		Clip = currItem->ContourLine;
	else
		Clip = currItem->PoLine;
#ifdef MODEDEBUG
//	qDebug() << "handle nodeedit move:" << m_doc->nodeEdit.submode << m_doc->nodeEdit.EdPoints;
#endif	
	if ((m_doc->nodeEdit.submode == NodeEditContext::DEL_POINT) || 
		(m_doc->nodeEdit.submode == NodeEditContext::MOVE_POINT) || 
		(m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH))
	{
		for (int a=0; a < signed(Clip.size()); ++a)
		{
			if (((m_doc->nodeEdit.EdPoints) && (a % 2 != 0)) || ((!m_doc->nodeEdit.EdPoints) && (a % 2 == 0)))
				continue;
			//QPoint np = p.map(Clip.pointQ(a));
			//QRect tx = QRect(np.x()-3, np.y()-3, 6, 6);
			//if (tx.intersects(mpo))
			QPointF point = Clip.pointQF(a);
			if (m_canvas->hitsCanvasPoint(m->globalPos(), itemPos.map(point)))
			{
				if (m_doc->nodeEdit.submode == NodeEditContext::MOVE_POINT)
					qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
				if (m_doc->nodeEdit.submode == NodeEditContext::DEL_POINT)
					qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png"), 1, 1));
				if (m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH)
					qApp->changeOverrideCursor(QCursor(loadIcon("Split.png"), 1, 1));
				return true;
			}
		}
	}
	if ((m_doc->nodeEdit.submode == NodeEditContext::ADD_POINT) || 
		(m_doc->nodeEdit.submode == NodeEditContext::MOVE_POINT) || 
		(m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH && m_doc->nodeEdit.EdPoints))
	{
		FPoint mousePoint = m_canvas->globalToCanvas(m->globalPos());
		for (uint poi=0; poi<Clip.size()-3; poi += 4)
		{
			// create bezier curve in canvas coords
			QPointF a1 = itemPos.map(Clip.pointQF(poi));
			QPointF a2 = itemPos.map(Clip.pointQF(poi+1));
			QPointF a3 = itemPos.map(Clip.pointQF(poi+3));
			QPointF a4 = itemPos.map(Clip.pointQF(poi+2));
			QPainterPath Bez;
			Bez.moveTo(a1);
			Bez.cubicTo(a2, a3, a4);
			double len = Bez.length();
			if (len > 0.0)
			{
				double delta = 2.0 / len;
				for (double d = 0.0; d <= 1.0; d += delta)
				{
					QPointF pl = Bez.pointAtPercent(d);
					if (m_canvas->hitsCanvasPoint(m->globalPos(), FPoint(pl.x(), pl.y())))
					{
						if (m_doc->nodeEdit.submode == NodeEditContext::MOVE_POINT)
							qApp->changeOverrideCursor(QCursor(loadIcon("HandC.xpm")));
						else if (m_doc->nodeEdit.submode == NodeEditContext::ADD_POINT)
							qApp->changeOverrideCursor(QCursor(loadIcon("AddPoint.png"), 1, 1));
						else if (m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH)
							qApp->changeOverrideCursor(QCursor(loadIcon("Split.png"), 1, 1));
						else
							qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
						m_doc->nodeEdit.ClRe2 = poi;
						return true;
					}
				}
			}
			// as polygon
//			QPolygonF cli = Bez.toSubpathPolygons().first();
//			for (int clp = 0; clp < cli.size()-1; ++clp)
//			{
				// project mousepoint on line
//				FPoint pointOnLine = projectPointOnLine(mousePoint, cli.at(clp), cli.at(clp+1));
				//if (m_view->PointOnLine(cli.point(clp), cli.point(clp+1), p.inverted().mapRect(mpo)))
//				if (m_canvas->hitsCanvasPoint(m->globalPos(), pointOnLine))
//				{
//					if (m_doc->nodeEdit.submode == NodeEditContext::MOVE_POINT)
//						qApp->changeOverrideCursor(QCursor(loadIcon("HandC.xpm")));
//					if (m_doc->nodeEdit.submode == NodeEditContext::ADD_POINT)
//						qApp->changeOverrideCursor(QCursor(loadIcon("AddPoint.png"), 1, 1));
//					if (m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH)
//						qApp->changeOverrideCursor(QCursor(loadIcon("Split.png"), 1, 1));
//					m_doc->nodeEdit.ClRe2 = poi;
//					return true;
//				}
//			}
		}
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	return false;
}

void CanvasMode_NodeEdit::handleNodeEditDrag(QMouseEvent* m, PageItem* currItem)
{
	FPoint npf;
	if ((m_canvas->m_viewMode.m_MouseButtonPressed) && !m_doc->nodeEdit.hasNodeSelected() && (m_doc->nodeEdit.SegP1 == -1) && (m_doc->nodeEdit.SegP2 == -1))
	{
		if (!m_rectangleSelect)
		{
			m_rectangleSelect = new RectSelect(this);
		}
		m_rectangleSelect->prepare(m->globalPos());
		m_view->startGesture(m_rectangleSelect);
		return;
	}
	int newX = m->x();
	int newY = m->y();
	FPoint np(newX-Mxp, newY-Myp, 0, 0, currItem->rotation(), 1, 1, true);
	np = np * (1.0 / m_canvas->scale());
	m_canvas->m_viewMode.operItemMoving = true;
	currItem = m_doc->m_Selection->itemAt(0);
	if ((m_doc->nodeEdit.SegP1 != -1) && (m_doc->nodeEdit.SegP2 != -1))
	{
		if (m_doc->nodeEdit.isContourLine)
			npf = currItem->ContourLine.point(m_doc->nodeEdit.SegP2) + np;
		else
			npf = currItem->PoLine.point(m_doc->nodeEdit.SegP2) + np;
		m_doc->nodeEdit.ClRe = m_doc->nodeEdit.SegP2;
		m_doc->nodeEdit.moveClipPoint(currItem, npf);
		if (m_doc->nodeEdit.isContourLine)
			npf = currItem->ContourLine.point(m_doc->nodeEdit.SegP1) + np;
		else
			npf = currItem->PoLine.point(m_doc->nodeEdit.SegP1) + np;
		m_doc->nodeEdit.ClRe = m_doc->nodeEdit.SegP1;
		m_doc->nodeEdit.moveClipPoint(currItem, npf);
		Mxp = newX;
		Myp = newY;
	}
	else
	{
		if ((m_doc->nodeEdit.SelNode.count() != 0) && (m_doc->nodeEdit.EdPoints))
		{
			int storedClRe = m_doc->nodeEdit.ClRe;
			if (m_doc->nodeEdit.SelNode.count() == 1)
			{
				npf = m_canvas->globalToCanvas(m->globalPos());
				double nx = npf.x();
				double ny = npf.y();
				if (!m_doc->ApplyGuides(&nx, &ny))
					npf = m_doc->ApplyGridF(FPoint(nx, ny));
				else
					npf = FPoint(nx, ny);
				npf = FPoint(npf.x(), npf.y(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
				m_doc->nodeEdit.moveClipPoint(currItem, npf);
				m_canvas->displayXYHUD(m->globalPos(), npf.x(), npf.y());
			}
			else
			{
				for (int itm = 0; itm < m_doc->nodeEdit.SelNode.count(); ++itm)
				{
					m_doc->nodeEdit.ClRe = m_doc->nodeEdit.SelNode.at(itm);
					if (m_doc->nodeEdit.isContourLine)
						npf = currItem->ContourLine.point(m_doc->nodeEdit.ClRe) + np;
					else
						npf = currItem->PoLine.point(m_doc->nodeEdit.ClRe) + np;
					m_doc->nodeEdit.moveClipPoint(currItem, npf);
				}
			}
			m_doc->nodeEdit.ClRe = storedClRe;
		}
		else
		{
			if (m_doc->nodeEdit.isContourLine)
				npf = currItem->ContourLine.point(m_doc->nodeEdit.ClRe) + np;
			else
				npf = currItem->PoLine.point(m_doc->nodeEdit.ClRe) + np;
			double nx = npf.x();
			double ny = npf.y();
			nx += currItem->xPos();
			ny += currItem->yPos();
			if (!m_doc->ApplyGuides(&nx, &ny))
				npf = m_doc->ApplyGridF(FPoint(nx, ny));
			else
				npf = FPoint(nx, ny);
			npf = FPoint(npf.x(), npf.y(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
			m_doc->nodeEdit.moveClipPoint(currItem, npf);
			m_canvas->displayXYHUD(m->globalPos(), npf.x(), npf.y());
		}
		Mxp = newX;
		Myp = newY;
	}
	m_canvas->m_viewMode.operItemMoving = false;
	m_doc->regionsChanged()->update(QRectF());
}

