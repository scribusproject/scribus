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
		currItem->update();
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

}


void CanvasMode_NodeEdit::leaveEvent(QEvent *e)
{

}



void CanvasMode_NodeEdit::mouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
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
	if (GetItem(&currItem))
	{
		if (m_doc->DragP)
			return;
		//Operations run here:
		//Item resize, esp after creating a new one
		if (m_view->moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed 
			&& (m->buttons() & Qt::LeftButton) &&  (!currItem->locked()))
		{
			handleNodeEditDrag(m, currItem);
		}
		else if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		{
			if (!currItem->locked())
			{
				QMatrix p;
				QRect mpo;
				handleNodeEditMove(m, mpo, currItem, p);
			}
		}

	}
	else // shouldnt be in nodeedit mode now, should it??
	{
		npf = m_canvas->globalToCanvas(m->globalPos());
		if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton) && (GyM == -1) && (GxM == -1))
		{
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
						m_view->setCursor(QCursor(Qt::ArrowCursor));
					else
						m_view->setCursor(QCursor(Qt::SplitVCursor));
					return;
				}
			}
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
						m_view->setCursor(QCursor(Qt::ArrowCursor));
					else
						m_view->setCursor(QCursor(Qt::SplitHCursor));
					return;
				}
			}
			m_view->setCursor(QCursor(Qt::ArrowCursor));
		}
	}
}

void CanvasMode_NodeEdit::mousePressEvent(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	MoveGX = MoveGY = false;
	m->accept();
	m_view->registerMousePress(m->globalPos());
	QRect mpo;
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
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	if (m_view->moveTimerElapsed())
	{
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
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			FPointArray Clip;
			if (m_doc->nodeEdit.isContourLine)
				Clip = currItem->ContourLine;
			else
				Clip = currItem->PoLine;
			FPoint npf;
			double nx = newP.x();
			double ny = newP.y();
			if (!m_doc->ApplyGuides(&nx, &ny))
				npf = m_doc->ApplyGridF(FPoint(nx, ny));
			else
				npf = FPoint(nx, ny);
			npf = FPoint(npf.x(), npf.y(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
			m_doc->nodeEdit.moveClipPoint(currItem, npf);
		}

		m_doc->AdjustItemSize(currItem);
		if (!m_doc->nodeEdit.isContourLine)
			currItem->ContourLine.translate(xposOrig - currItem->xPos(), yposOrig - currItem->yPos());
		m_doc->regionsChanged()->update(QRectF());
		if (state)
		{
			m_doc->nodeEdit.finishTransaction2(currItem, state);
		}
		return;
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



void CanvasMode_NodeEdit::handleNodeEditPress(QMouseEvent* m, QRect)
{
	FPoint npf2;
	
	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	FPointArray Clip = m_doc->nodeEdit.beginTransaction(currItem);
	bool edited = false;
	bool pfound = false;
	npf2 = m_canvas->globalToCanvas(m->globalPos()).transformPoint(currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0, true);
	QMatrix pm2 = currItem->getTransform();
	for (int a=0; a < signed(Clip.size()); ++a)
	{
		if (((m_doc->nodeEdit.EdPoints) && (a % 2 != 0)) || ((!m_doc->nodeEdit.EdPoints) && (a % 2 == 0)))
			continue;
		QPointF npf = pm2.map(Clip.pointQF(a));
		if (m_canvas->hitsCanvasPoint(m->globalPos(), npf))
		{
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
		if ((!m_doc->nodeEdit.hasNodeSelected()) && (m_doc->nodeEdit.ClRe2 != -1)) // We don't have a Point, try to add one onto the current curve segment
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
						bb->setFrameType(currItem->frameType());
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
		emit m_view->PolyStatus(currItem->itemType(), currItem->PoLine.size());
	}
	if ((m_doc->nodeEdit.SelNode.count() != 0) || ((m_doc->nodeEdit.SegP1 != -1) && (m_doc->nodeEdit.SegP2 != -1)) || (m_doc->nodeEdit.hasNodeSelected() && (!m_doc->nodeEdit.EdPoints)))
	{
		Mxp = m->x();
		Myp = m->y();
		m_canvas->setRenderModeFillBuffer();
	}
	else
	{
		Mxp = m->x();
		Myp = m->y();
		Dxp = qRound(m->x()/m_canvas->scale());
		Dyp = qRound(m->y()/m_canvas->scale());
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
	if ((m_doc->nodeEdit.submode == NodeEditContext::DEL_POINT) || 
		(m_doc->nodeEdit.submode == NodeEditContext::MOVE_POINT) || 
		(m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH))
	{
		for (int a=0; a < signed(Clip.size()); ++a)
		{
			if (((m_doc->nodeEdit.EdPoints) && (a % 2 != 0)) || ((!m_doc->nodeEdit.EdPoints) && (a % 2 == 0)))
				continue;
			QPointF point = Clip.pointQF(a);
			if (m_canvas->hitsCanvasPoint(m->globalPos(), itemPos.map(point)))
			{
				if (m_doc->nodeEdit.submode == NodeEditContext::MOVE_POINT)
					m_view->setCursor(QCursor(Qt::SizeAllCursor));
				if (m_doc->nodeEdit.submode == NodeEditContext::DEL_POINT)
					m_view->setCursor(QCursor(loadIcon("DelPoint.png"), 1, 1));
				if (m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH)
					m_view->setCursor(QCursor(loadIcon("Split.png"), 1, 1));
				return true;
			}
		}
	}
	if ((m_doc->nodeEdit.submode == NodeEditContext::ADD_POINT) || 
		(m_doc->nodeEdit.submode == NodeEditContext::MOVE_POINT) || 
		(m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH && m_doc->nodeEdit.EdPoints))
	{
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
							m_view->setCursor(QCursor(loadIcon("HandC.xpm")));
						else if (m_doc->nodeEdit.submode == NodeEditContext::ADD_POINT)
							m_view->setCursor(QCursor(loadIcon("AddPoint.png"), 1, 1));
						else if (m_doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH)
							m_view->setCursor(QCursor(loadIcon("Split.png"), 1, 1));
						else
							m_view->setCursor(QCursor(Qt::ArrowCursor));
						m_doc->nodeEdit.ClRe2 = poi;
						return true;
					}
				}
			}
		}
	}
	m_view->setCursor(QCursor(Qt::ArrowCursor));
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
		Mxp = newX;
		Myp = newY;
	}
	m_canvas->m_viewMode.operItemMoving = false;
	m_doc->regionsChanged()->update(QRectF());
}

