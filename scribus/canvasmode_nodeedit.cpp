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


#include <QMenu>
#include <QMouseEvent>
#include <QPainter>

#include "canvas.h"
#include "canvasgesture_pan.h"
#include "canvasgesture_rectselect.h"
#include "canvasmode_nodeedit.h"
#include "fpoint.h"
#include "iconmanager.h"
#include "nodeeditcontext.h"
#include "pageitem.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/nodeeditpalette.h"
#include "ui/pageselector.h"
#include "ui/scrspinbox.h"

CanvasMode_NodeEdit::CanvasMode_NodeEdit(ScribusView* view) : CanvasMode(view)
{
	m_ScMW = m_view->m_ScMW;
}

void CanvasMode_NodeEdit::drawControls(QPainter* p)
{
	if (m_doc->m_Selection->isEmpty() || m_doc->nodeEdit.previewMode())
		return;

	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	FPointArray cli;

	p->save();
	p->scale(m_canvas->scale(), m_canvas->scale());
	p->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	p->setTransform(currItem->getTransform(), true);
	p->setBrush(Qt::NoBrush);
	p->setRenderHint(QPainter::Antialiasing);

	if ((m_doc->nodeEdit.isContourLine()) && (!currItem->ContourLine.empty()))
		cli = currItem->ContourLine;
	else
	{
		cli = currItem->PoLine;
		if (currItem->isSymbol() || currItem->isGroup())
		{
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
		}
	}
	const double scale = m_canvas->m_viewMode.scale;

	double nx = 0.0;
	double ny = 0.0;
	double cx = 0.0;
	double cy = 0.0;

	QPen penControl = pens().value("node");
	bool isActiveNode = false;
	bool isActiveControl = false;
	bool highlight = false;
	bool mergedNodes = true;
	int nr, nl, inr;
	FPoint pn, pc;

	// Draw Vector Path
	if (cli.size() > 3)
	{
		for (int i = 0; i < cli.size() - 3; i += 4)
		{
			if (cli.isMarker(i))
				continue;

			const FPoint& a1 = cli.point(i);
			const FPoint& a2 = cli.point(i + 1);
			const FPoint& a3 = cli.point(i + 3);
			const FPoint& a4 = cli.point(i + 2);
			QPainterPath Bez;
			Bez.moveTo(a1.x(), a1.y());
			Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());

			// Vector Path
			p->setPen(pens().value("vector"));
			p->drawPath(Bez);

		}
	}

	// Draw Nodes
	for (int i = 0; i < cli.size() - 1; i += 2)
	{
		if (cli.isMarker(i))
			continue;

		nr = NodeEditContext::indexOfNode(m_doc->nodeEdit.clre(), NodeEditContext::NodeType::NodeRight, cli.size());
		nl = NodeEditContext::indexOfNode(m_doc->nodeEdit.clre(), NodeEditContext::NodeType::NodeLeft, cli.size());
		inr = NodeEditContext::indexOfNode(i, NodeEditContext::NodeType::NodeRight, cli.size());

		mergedNodes = NodeEditContext::hasTwoNodes(i, cli);
		highlight = (i == nr || i == nl) && NodeEditContext::hasTwoNodes(m_doc->nodeEdit.clre(), cli);

		isActiveControl = m_doc->nodeEdit.clre() == i + 1 ? true : false;
		isActiveNode =	m_doc->nodeEdit.clre() == i ||
						m_doc->nodeEdit.selNode().contains(i) ||
						m_doc->nodeEdit.selNode().contains(i - 2) ||
						(m_doc->nodeEdit.selNode().contains(inr) && mergedNodes) ? true : false;

		cli.point(i, &nx, &ny);
		cli.point(i + 1, &cx, &cy);

		// Draw Control Handle Lines
		if (FPoint(cx, cy) != FPoint(nx, ny))
		{
			// changes color to highlight if angle is same as opponent handle
			if (highlight && state.isSameAngle && state.clickedOn)
				p->setPen(pens().value("highlight"));
			else
				p->setPen(pens().value("node-handle"));

			p->setRenderHint(QPainter::Antialiasing, true);
			p->drawLine(QPointF(nx, ny), QPointF(cx, cy));
		}

		p->setRenderHint(QPainter::Antialiasing, false);

		// Draw Node Handle
		drawNodeHandle(p, QPointF(nx, ny), pens().value("node"), scale, isActiveNode);

		// Draw Control Handle
		if (FPoint(cx, cy) != FPoint(nx, ny))
		{
			// changes color to highlight if length is same as opponent handle
			if (highlight && state.isSameLength && state.isSameAngle && state.clickedOn)
				penControl = pens().value("highlight");
			else
				penControl = pens().value("node");

			drawNodeControl(p, QPointF(cx, cy), penControl, scale, isActiveControl);
		}
	}


	emit m_view->HavePoint(m_doc->nodeEdit.clre() != -1);

	if (m_doc->nodeEdit.clre() != -1)
	{
		cli.point(m_doc->nodeEdit.clre(), &nx, &ny);
		emit m_view->ClipPo(nx, ny);
	}

	p->restore();
}

QMap<int, int> CanvasMode_NodeEdit::selectedNodesList(FPointArray points)
{
	QMap<int, int> selList;
	const auto& selNodes = m_doc->nodeEdit.selNode();

	if (points.size() > 3)
	{
		for (int i = 0; i < points.size() - 1; i += 2)
		{
			if (points.isMarker(i))
				continue;

			// i = path start;
			// i + 2 = path end OR left part of a node
			// i - 2 = right part of a node
			if (selNodes.contains(i) || selNodes.contains(i + 2) || selNodes.contains(i - 2) || selNodes.contains(i - 4))
			{
				selList.insert(i, i + 1);
			}
		}
	}

	return selList;
}

void CanvasMode_NodeEdit::activate(bool fromGesture)
{
	CanvasMode::activate(fromGesture);
	if (fromGesture && m_rectangleSelect)
	{
		m_canvas->m_viewMode.m_MouseButtonPressed = false;
		// handle rectangle select
		PageItem* currItem = m_doc->m_Selection->itemAt(0);
		m_doc->nodeEdit.selNode().clear();
		QRectF Sele = m_rectangleSelect->result();
		FPointArray Clip;
		bool firstPoint = false;
		QTransform pm2 = currItem->getTransform();
		if (m_doc->nodeEdit.isContourLine())
			Clip = currItem->ContourLine;
		else
		{
			Clip = currItem->PoLine;
			if (currItem->isSymbol() || currItem->isGroup())
			{
				if (currItem->imageFlippedH())
				{
					pm2.translate(currItem->width(), 0);
					pm2.scale(-1, 1);
				}
				if (currItem->imageFlippedV())
				{
					pm2.translate(0, currItem->height());
					pm2.scale(1, -1);
				}
			}
		}
		for (int i = 0; i < Clip.size(); ++i)
		{
			if (Clip.isMarker(i))
				continue;
			const FPoint& np = Clip.point(i);
			FPoint npf2 = np.transformPoint(pm2, false);
			if ((Sele.contains(npf2.x(), npf2.y())) && ((i == 0) || (((i - 2) % 4) == 0)))
			{
				if (i == 0)
					firstPoint = true;
				if ((firstPoint) && (i == Clip.size() - 2) && (!currItem->asPolyLine()))
					continue;
				m_doc->nodeEdit.selectNode(i);
				m_doc->nodeEdit.update(Clip.pointQF(i));
			}
		}
		currItem->update();
		m_doc->nodeEdit.finishTransaction(currItem);
		delete m_rectangleSelect;
		m_rectangleSelect = nullptr;
	}
	else
	{
		m_Mxp = -1;  // last mouse position
		m_Myp = -1;
		m_Dxp = -1;  // last mouse press position for rectangle select
		m_Dyp = -1;
		m_GxM = -1;  // guide position
		m_GyM = -1;
		m_MoveGX = m_MoveGY = false;
	}
}

void CanvasMode_NodeEdit::deactivate(bool forGesture)
{
	if (!forGesture && m_rectangleSelect)
	{
		m_rectangleSelect->clear();
		delete m_rectangleSelect;
		m_rectangleSelect = nullptr;
	}
	CanvasMode::deactivate(forGesture);
}


inline bool CanvasMode_NodeEdit::GetItem(PageItem** pi)
{ 
	*pi = m_doc->m_Selection->itemAt(0); 
	return (*pi) != nullptr;
}

FPoint CanvasMode_NodeEdit::constraintControlAngle(FPoint anchor, FPoint mouse, double angleStep)
{
	QLineF line(anchor.toQPointF(), mouse.toQPointF());
	line.setAngle(constrainAngle(line.angle(), angleStep));

	return line.p2();
}

FPoint CanvasMode_NodeEdit::snapControlAngle(FPoint anchor, FPoint mouse, double angle)
{
	double snap = m_doc->prefsData().guidesPrefs.guideRad / m_canvas->scale();
	QLineF lineAM(anchor.x(), anchor.y(), mouse.x(), mouse.y()); // anchor to mouse
	QLineF lineAC = lineAM; // anchor to opposite control point
	lineAC.setAngle(360.0 - angle); // invert angle, Qt angle is counter-clockwise

	if (qMax(lineAM.angle(), lineAC.angle()) - qMin(lineAM.angle(), lineAC.angle()) <= 0.00001)
		return lineAC.p2();

	return QLineF(lineAM.p2(), lineAC.p2()).length() <= snap ? lineAC.p2() : mouse;
}

FPoint CanvasMode_NodeEdit::snapControlLength(FPoint anchor, FPoint mouse, double length)
{
	double snap = m_doc->prefsData().guidesPrefs.guideRad / m_canvas->scale();
	QLineF line(anchor.toQPointF(), mouse.toQPointF());

	if (length - snap <= line.length() && length + snap >= line.length())
		line.setLength(length);

	return line.p2();
}

FPoint CanvasMode_NodeEdit::snapControlOrigin(FPoint anchor, FPoint mouse)
{
	double snap = m_doc->prefsData().guidesPrefs.guideRad / m_canvas->scale();
	QLineF line(anchor.toQPointF(), mouse.toQPointF());

	return (line.length() <= snap) ? anchor : mouse;

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
	double sc = m_canvas->scale();
	FPoint npf = m_canvas->globalToCanvas(m->globalPosition());
	m_doc->nodeEdit.setScale(sc);

	m->accept();

	PageItem* currItem { nullptr };
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
				QTransform p;
				QRect mpo;
				handleNodeEditMove(m, mpo, currItem, p);
			}
		}

	}
	else // shouldn't be in nodeedit mode now, should it??
	{
		npf = m_canvas->globalToCanvas(m->globalPosition());
		if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->buttons() & Qt::LeftButton) && (m_GyM == -1) && (m_GxM == -1))
		{
			if (!m_rectangleSelect)
			{
				m_rectangleSelect = new RectSelect(this);
			}
			m_rectangleSelect->prepare(m->globalPosition());
			m_view->startGesture(m_rectangleSelect);
			return;
		}
		if ((m_doc->guidesPrefs().guidesShown) && (!m_doc->GuideLock)
			&& (m_doc->OnPage(npf.x(), npf.y()) != -1))
		{
			Guides::iterator it;
			Guides tmpGuides = m_doc->currentPage()->guides.horizontals(GuideManagerCore::Standard);
			for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it)
			{
				if ( (*it) + m_doc->currentPage()->yOffset() < npf.y() + m_doc->guidesPrefs().grabRadius * sc &&
					 (*it) + m_doc->currentPage()->yOffset() > npf.y() - m_doc->guidesPrefs().grabRadius * sc )
				{
					if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m_GyM != -1))
						m_MoveGY = true;
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
				if ( (*it) + m_doc->currentPage()->xOffset() < npf.x() + m_doc->guidesPrefs().grabRadius * sc &&
					 (*it) + m_doc->currentPage()->xOffset() > npf.x() - m_doc->guidesPrefs().grabRadius * sc)
				{
					if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m_GxM != -1))
						m_MoveGX = true;
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
	m_doc->nodeEdit.setScale(m_canvas->scale());
	m_MoveGX = m_MoveGY = false;
	m->accept();
	m_view->registerMousePress(m->globalPosition());
	QRect mpo;
	m_Mxp = m->position().x();
	m_Myp = m->position().y();

	if (((m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier)) || ((!(m->modifiers() & Qt::ControlModifier)) && (m->buttons() & Qt::MiddleButton)))
	{
		if (!m_panGesture)
		{
			m_panGesture = new PanGesture(this);
		}
		m_view->startGesture(m_panGesture);
		m_panGesture->mousePressEvent(m); // Not an error, this is used to register current canvas point
		return;
	}
	
	if (m_doc->m_Selection->count() != 0)
	{
		handleNodeEditPress(m, mpo);
	}

	if ((m_doc->m_Selection->count() != 0) && (m->button() == Qt::RightButton))
	{
		m_canvas->m_viewMode.m_MouseButtonPressed = true;
		m_Dxp = m_Mxp;
		m_Dyp = m_Myp;
	}
}

void CanvasMode_NodeEdit::mouseReleaseEvent(QMouseEvent *m)
{
	PageItem *currItem;

	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();

	// Deselect selected node if SHIFT is pressed
	if (state.clickedOn == NodeHandle &&
		m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT &&
		!state.wasNewlySelected &&
		m->modifiers() == Qt::ShiftModifier)
	{
		m_doc->nodeEdit.deselectNode(m_doc->nodeEdit.clre());
		m_doc->regionsChanged()->update(QRectF());
	}

	if (m_view->moveTimerElapsed())
	{
		// Path Segment
		if (m_doc->nodeEdit.segP1() != -1 && m_doc->nodeEdit.segP2() != -1)
		{
			m_doc->nodeEdit.deselect();
			m_doc->nodeEdit.setSegP1(-1);
			m_doc->nodeEdit.setSegP2(-1);
		}

		m_canvas->m_viewMode.operItemMoving = false;
		currItem = m_doc->m_Selection->itemAt(0);
		double xposOrig = currItem->xPos();
		double yposOrig = currItem->yPos();

		ScOldNewState<FPointArray> *onState = m_doc->nodeEdit.finishTransaction1(currItem);

		if (m_doc->nodeEdit.hasNodeSelected() && (m_doc->nodeEdit.selectionCount() == 1))
		{

			//FIXME:av
			// FPoint newP = m_canvas->globalToCanvas(m->globalPosition());
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			// currItem->ClipEdited = true;
			// FPointArray Clip;
			// FPoint npf;
			// double nx = newP.x();
			// double ny = newP.y();
			// if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
			// 	npf = m_doc->ApplyGridF(FPoint(nx, ny));
			// else
			// 	npf = FPoint(nx, ny);
			// QTransform pp = currItem->getTransform();
			// npf = npf.transformPoint(pp, true);
			// if (m_doc->nodeEdit.isContourLine())
			// 	Clip = currItem->ContourLine;
			// else
			// {
			// 	Clip = currItem->PoLine;
			// 	if (currItem->isSymbol() || currItem->isGroup())
			// 	{
			// 		if (currItem->imageFlippedH())
			// 		{
			// 			QTransform p;
			// 			p.translate(currItem->width(), 0);
			// 			p.scale(-1, 1);
			// 			npf = npf.transformPoint(p, false);
			// 		}
			// 		if (currItem->imageFlippedV())
			// 		{
			// 			QTransform p;
			// 			p.translate(0, currItem->height());
			// 			p.scale(1, -1);
			// 			npf = npf.transformPoint(p, false);
			// 		}
			// 	}
			// }
			// m_doc->nodeEdit.moveClipPoint(currItem, npf);
		}
		// nitramr: Scribus crashes if ESC key is pressed during mouse drag operation
		m_doc->adjustItemSize(currItem, true);
		if (!m_doc->nodeEdit.isContourLine())
		{
			// Move the contour accordingly in the item's coordinate space
			QTransform m = QTransform().rotate(-currItem->rotation());
			QPointF delta = m.map(QPointF(xposOrig, yposOrig)) - m.map(QPointF(currItem->xPos(), currItem->yPos()));
			currItem->ContourLine.translate(delta.x(), delta.y());
		}
		m_doc->regionsChanged()->update(QRectF());
		if (onState)
			m_doc->nodeEdit.finishTransaction2(currItem, onState);

	}
	else
	{
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
		if (m_doc->m_Selection->itemAt(0) != nullptr)
			m_doc->itemAddCommit(m_doc->m_Selection->itemAt(0));
		//Make sure the Zoom spinbox and page selector don't have focus if we click on the canvas
		m_view->m_ScMW->zoomSpinBox->clearFocus();
		m_view->m_ScMW->pageSelector->clearFocus();
		if (m_doc->m_Selection->itemAt(0) != nullptr) // is there the old clip stored for the undo action
		{
			currItem = m_doc->m_Selection->itemAt(0);
			m_doc->nodeEdit.finishTransaction(currItem);
		}
	}

	m_doc->nodeEdit.setMoveMode(state.originalMoveMode);
	state.reset();
}

void CanvasMode_NodeEdit::keyPressEvent(QKeyEvent *e)
{
	commonkeyPressEvent_NormalNodeEdit(e);

	int kk = e->key();
//	Qt::KeyboardModifiers buttonModifiers = e->modifiers();
//	ScribusMainWindow* mainWindow = m_view->m_ScMW;
	PageItem* currItem = m_doc->m_Selection->itemAt(0);

	if (kk == Qt::Key_Delete || kk == Qt::Key_Backspace)
	{
		if (currItem && m_doc->nodeEdit.hasNodeSelected())
		{
			// delete Nodes
			refreshPath(deleteNodes());
		}
	}
}

void CanvasMode_NodeEdit::keyReleaseEvent(QKeyEvent *e)
{
	commonkeyReleaseEvent(e);
}

void CanvasMode_NodeEdit::handleNodeEditPress(QMouseEvent* m, QRect r)
{
	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	FPointArray Clip = m_doc->nodeEdit.beginTransaction(currItem);
	QTransform pm2 = currItem->getTransform();
	FPoint npf2 = m_canvas->globalToCanvas(m->globalPosition()).transformPoint(pm2, true);
	bool edited = false;

	state.originalMoveMode = m_doc->nodeEdit.moveMode();
	state.moveMode = state.originalMoveMode;
	state.clickedOn = None;
	m_doc->nodeEdit.oppositeControl.isValid = false;

	switch (state.hoveredOver)
	{
	case ControlHandle:
	case NodeHandle:
	{
		if ((currItem->isSymbol() || currItem->isGroup()) && (!m_doc->nodeEdit.isContourLine()))
		{
			if (currItem->imageFlippedH())
			{
				pm2.translate(currItem->width(), 0);
				pm2.scale(-1, 1);
			}
			if (currItem->imageFlippedV())
			{
				pm2.translate(0, currItem->height());
				pm2.scale(1, -1);
			}
		}

		for (int i = 0; i < Clip.size(); ++i)
		{
			if (m_canvas->hitsCanvasPoint(m->globalPosition(), pm2.map(Clip.pointQF(i))))
			{
				if (state.originalMoveMode == NodeEditContext::Auto)
				{
					bool sameAngle = m_doc->nodeEdit.sameAngle(currItem, i);
					bool sameLength = m_doc->nodeEdit.sameLength(currItem, i);

					if (sameLength && sameAngle)
						state.moveMode = NodeEditContext::Symmetric;
					else if (!sameLength && sameAngle)
						state.moveMode = NodeEditContext::Asymmetric;
					else if (!sameLength && !sameAngle)
						state.moveMode = NodeEditContext::Independent;
				}

				state.clickedOn = NodeEditContext::isNode(i) ? NodeHandle : ControlHandle;

				if (state.clickedOn == NodeHandle)
				{
					// Deselect all controls
					m_doc->nodeEdit.deselectNodeControls();

					if (!m_doc->nodeEdit.selNode().contains(i) && m->modifiers() != Qt::ShiftModifier)
						m_doc->nodeEdit.selNode().clear();

					state.wasNewlySelected = !m_doc->nodeEdit.selNode().contains(i);

					m_doc->nodeEdit.selectNode(i);

					if (m->modifiers() == Qt::AltModifier || m_doc->nodeEdit.submode() == NodeEditContext::EDIT_POINT)
					{
						// Reset Controls
						if (state.moveMode != NodeEditContext::Independent || !NodeEditContext::hasTwoNodes(i, Clip))
							m_doc->nodeEdit.resetControl(currItem);
						else // Make Control Handles symetric
							m_doc->nodeEdit.equalizeControls(currItem);
					}
				}

				if (state.clickedOn == ControlHandle)
				{
					m_doc->nodeEdit.selNode().clear();
					m_doc->nodeEdit.selectNode(i);
				}

				m_doc->nodeEdit.setClre(i);

				// Save position of opposite control handle
				int indexNode = NodeEditContext::indexOfNode(i, NodeEditContext::OppositeSibling, Clip.size());
				int indexControl = NodeEditContext::indexOfNode(i, NodeEditContext::Opposite, Clip.size());

				double dx = Clip.point(indexControl).x() - Clip.point(indexNode).x();
				double dy = Clip.point(indexControl).y() - Clip.point(indexNode).y();

				m_doc->nodeEdit.oppositeControl.isValid = NodeEditContext::hasTwoNodes(i, Clip);
				m_doc->nodeEdit.oppositeControl.relativePos = Clip.point(indexControl) - Clip.point(indexNode);
				m_doc->nodeEdit.oppositeControl.controlIndex = indexControl;
				m_doc->nodeEdit.oppositeControl.nodeIndex = indexNode;
				m_doc->nodeEdit.oppositeControl.distance = distance(dx, dy);
				m_doc->nodeEdit.oppositeControl.angle = xy2Deg(dx, dy);
				m_doc->nodeEdit.update(Clip.pointQF(i));
				edited = true;

				break;
			}

			//deselect all if no node or control has hit
			if (i == Clip.size() - 1)
				m_doc->nodeEdit.deselect();

		}

		break;
	}
	case PathSegment:
		m_doc->nodeEdit.deselect();
		state.clickedOn = PathSegment;
		break;
	case None:
	default:
		m_doc->nodeEdit.deselect();
		state.clickedOn = None;
		break;
	}

	if (m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT && !m_doc->nodeEdit.hasNodeSelected() && state.clickedOn == PathSegment)
	{
		m_doc->nodeEdit.setSegP1(m_doc->nodeEdit.clre2());
		m_doc->nodeEdit.setSegP2(m_doc->nodeEdit.clre2()+2);
		m_doc->nodeEdit.setClre(m_doc->nodeEdit.clre2());
	}


	FPointArray cli;
	int EndInd = Clip.size();
	int StartInd = 0;
	if (m_doc->nodeEdit.clre() > 0)
	{
		for (int n = m_doc->nodeEdit.clre(); n < Clip.size(); ++n)
		{
			if (Clip.isMarker(n))
			{
				EndInd = n;
				break;
			}
		}
		for (int n2 = m_doc->nodeEdit.clre(); n2 > 0; n2--)
		{
			if (Clip.isMarker(n2))
			{
				StartInd = n2 + 1;
				break;
			}
		}
	}

	if (m_doc->nodeEdit.submode() == NodeEditContext::SPLIT_PATH && (state.clickedOn == NodeHandle || state.clickedOn == PathSegment))
	{
		// Path Segment
		if ((!m_doc->nodeEdit.hasNodeSelected()) && (m_doc->nodeEdit.clre2() != -1)) // We don't have a Point, try to add one onto the current curve segment
		{
			bool foundP = false;
			int seg = 0;
			double absDist = 9999999999.9;
			FPoint point(0, 0);
			FPoint normal(0, 0);
			FPoint tangent(0, 0);
			FPoint nearPoint(0, 0);
			double nearT = 0.0;
			QRect mpo2(0, 0, m_doc->guidesPrefs().grabRadius * 3, m_doc->guidesPrefs().grabRadius * 3);
			mpo2.moveCenter(QPoint(qRound(npf2.x()), qRound(npf2.y())));
			for (int poi = 0; poi < Clip.size() - 3; poi += 4)
			{
				const FPoint& a1 = Clip.point(poi);
				const FPoint& a2 = Clip.point(poi + 1);
				const FPoint& a3 = Clip.point(poi + 3);
				const FPoint& a4 = Clip.point(poi + 2);
				QPainterPath Bez;
				Bez.moveTo(a1.x(), a1.y());
				Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
				QPolygon cli2 = Bez.toFillPolygon().toPolygon();
				for (int clp = 0; clp < cli2.size() - 1; ++clp)
				{
					if (m_view->PointOnLine(cli2.point(clp), cli2.point(clp + 1), mpo2))
					{
						seg = poi;
						double sp = 0.0;
						double spadd = 1.0 / (Clip.lenPathSeg(seg) * m_canvas->scale());
						while (sp < 1.0)
						{
							Clip.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
							double d1 = fabs(sqrt(pow(point.x() - npf2.x(), 2) + pow(point.y() - npf2.y(), 2)));
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
			cli.putPoints(0, m_doc->nodeEdit.clre2() + 2, Clip);
			if (foundP)
			{
				npf2 = nearPoint;
				FPoint base = cli.point(cli.size() - 2);
				FPoint c1 = cli.point(cli.size() - 1);
				FPoint base2 =  Clip.point(m_doc->nodeEdit.clre2() + 2);
				FPoint c2 = Clip.point(m_doc->nodeEdit.clre2() + 3);
				if ((base == c1) && (base2 == c2))
				{
					cli.resize(cli.size()+4);
					cli.putPoints(cli.size() - 4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
					cli.putPoints(cli.size(), Clip.size() - (m_doc->nodeEdit.clre2() + 2), Clip, m_doc->nodeEdit.clre2() + 2);
				}
				else
				{
					FPoint cn1 = (1.0 - nearT) * base + nearT * c1;
					FPoint cn2 = (1.0 - nearT) * cn1 + nearT * ((1.0 - nearT) * c1 + nearT * c2);
					FPoint cn3 = (1.0 - nearT) * ((1.0 - nearT) * c1 + nearT * c2) + nearT * ((1.0 - nearT) * c2 + nearT * base2);
					FPoint cn4 = (1.0 - nearT) * c2 + nearT * base2;
					cli.setPoint(cli.size()-1, cn1);
					cli.resize(cli.size() + 4);
					int basind = cli.size() + 1;
					cli.putPoints(cli.size() - 4, 4, npf2.x(), npf2.y(), cn2.x(), cn2.y(), npf2.x(), npf2.y(), cn3.x(), cn3.y());
					cli.putPoints(cli.size(), Clip.size() - (m_doc->nodeEdit.clre2() + 2), Clip, m_doc->nodeEdit.clre2() + 2);
					cli.setPoint(basind, cn4);
				}
				Clip = cli.copy();
				cli.resize(0);
				m_doc->nodeEdit.setClre(m_doc->nodeEdit.clre2() + 2);
				m_doc->nodeEdit.setClre2(-1);
				EndInd = Clip.size();
				StartInd = 0;
				for (int n = m_doc->nodeEdit.clre(); n < Clip.size(); ++n)
				{
					if (Clip.isMarker(n))
					{
						EndInd = n;
						break;
					}
				}
				if (m_doc->nodeEdit.clre() > 0)
				{
					for (int n2 = m_doc->nodeEdit.clre(); n2 > 0; n2--)
					{
						if (Clip.isMarker(n2))
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
		// Node Handle
		if (m_doc->nodeEdit.hasNodeSelected())
		{
			if (currItem->isPolygon())
			{
				if ((m_doc->nodeEdit.clre() != 0) && (m_doc->nodeEdit.clre() != (EndInd - 2)))
				{
					if (currItem->Segments.count() == 0)
					{
						cli.putPoints(0, EndInd - (m_doc->nodeEdit.clre() + 2), Clip, m_doc->nodeEdit.clre() + 2);
						cli.putPoints(cli.size(), m_doc->nodeEdit.clre() + 2, Clip);
					}
					else
					{
						cli.putPoints(0, EndInd-StartInd, Clip, StartInd);
						int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor());
						PageItem* bb = m_doc->Items->takeAt(z);
						if (m_doc->nodeEdit.isContourLine())
							bb->ContourLine.resize(0);
						else
							bb->PoLine.resize(0);
						if (StartInd != 0)
						{
							if (m_doc->nodeEdit.isContourLine())
							{
								bb->ContourLine.putPoints(0, StartInd - 4, Clip);
								bb->ContourLine.putPoints(bb->ContourLine.size(), Clip.size() -EndInd, Clip, EndInd);
							}
							else
							{
								bb->PoLine.putPoints(0, StartInd - 4, Clip);
								bb->PoLine.putPoints(bb->PoLine.size(), Clip.size() - EndInd, Clip, EndInd);
							}
						}
						else
						{
							if (m_doc->nodeEdit.isContourLine())
								bb->ContourLine.putPoints(0, Clip.size() - EndInd - 4, Clip, EndInd + 4);
							else
								bb->PoLine.putPoints(0, Clip.size() - EndInd - 4, Clip, EndInd + 4);
						}
						bb->setRotation(currItem->rotation());
						bb->ClipEdited = true;
						m_doc->Items->insert(m_doc->Items->indexOf(currItem), bb);
						m_doc->adjustItemSize(bb, true);
					}
					currItem->PoLine = cli.copy();
				}
				m_doc->nodeEdit.deselect();
				currItem->ClipEdited = true;
				edited = true;
				m_doc->nodeEdit.setSubMode(NodeEditContext::MOVE_POINT);
				PageItem* newItem = m_doc->convertItemTo(currItem, PageItem::PolyLine);
				currItem = newItem;
				m_doc->m_Selection->clear();
				m_doc->m_Selection->addItem(currItem);
				emit m_view->PolyOpen();
			}
			else
			{
				if ((currItem->isPolyLine()) || (currItem->isPathText()))
				{
					if ((m_doc->nodeEdit.clre() > 1) && (m_doc->nodeEdit.clre() < (Clip.size() - 2)))
					{
						int z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor());
						PageItem* bb = m_doc->Items->at(z);
						if (m_doc->nodeEdit.isContourLine())
							bb->ContourLine.putPoints(0, Clip.size() - (m_doc->nodeEdit.clre() + 2), Clip, m_doc->nodeEdit.clre() + 2);
						else
							bb->PoLine.putPoints(0, Clip.size() - (m_doc->nodeEdit.clre() + 2), Clip, m_doc->nodeEdit.clre() + 2);
						bb->setRotation(currItem->rotation());
						m_doc->adjustItemSize(bb, true);
						bb->ClipEdited = true;
						bb->setFrameType(currItem->frameType());
						cli.resize(0);
						cli.putPoints(0, m_doc->nodeEdit.clre() + 2, Clip);
						currItem->PoLine = cli.copy();
						m_doc->m_Selection->clear();
						m_doc->m_Selection->addItem(currItem);
					}
					m_doc->nodeEdit.deselect();
					currItem->ClipEdited = true;
					edited = true;
					m_doc->nodeEdit.setSubMode(NodeEditContext::MOVE_POINT);
					currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2.0, 1.0)));
					emit m_view->PolyOpen();
				}
			}
		}
	}

	if ((m_doc->nodeEdit.submode() == NodeEditContext::DEL_POINT || m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT && m->modifiers() == Qt::ControlModifier) && m_doc->nodeEdit.hasNodeSelected() && state.clickedOn == NodeHandle)
	{
		edited = deleteNodes();
	}

	// Add node id "add point" mode OR "move point" mode + CTRL
	if ((m_doc->nodeEdit.submode() == NodeEditContext::ADD_POINT || m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT && m->modifiers() == Qt::ControlModifier) && (m_doc->nodeEdit.clre2() != -1))
	{
		bool foundP = false;
		uint seg = 0;
		double absDist = 9999999999.9;
		FPoint point(0, 0);
		FPoint normal(0, 0);
		FPoint tangent(0, 0);
		FPoint nearPoint(0, 0);
		double nearT = 0.0;
		QRect mpo2(0, 0, m_doc->guidesPrefs().grabRadius * 3, m_doc->guidesPrefs().grabRadius * 3);
		mpo2.moveCenter(QPoint(qRound(npf2.x()), qRound(npf2.y())));
		for (int poi = 0; poi < Clip.size() - 3; poi += 4)
		{
			const FPoint& a1 = Clip.point(poi);
			const FPoint& a2 = Clip.point(poi + 1);
			const FPoint& a3 = Clip.point(poi + 3);
			const FPoint& a4 = Clip.point(poi + 2);
			QPainterPath Bez;
			Bez.moveTo(a1.x(), a1.y());
			Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
			QPolygon cli2 = Bez.toFillPolygon().toPolygon();
			for (int clp = 0; clp < cli2.size() - 1; ++clp)
			{
				if (m_view->PointOnLine(cli2.point(clp), cli2.point(clp + 1), mpo2))
				{
					seg = poi;
					double sp = 0.0;
					double spadd = 1.0 / (Clip.lenPathSeg(seg) * m_canvas->scale());
					while (sp < 1.0)
					{
						Clip.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
						double d1 = fabs(sqrt(pow(point.x() - npf2.x(), 2) + pow(point.y() - npf2.y(), 2)));
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
		cli.putPoints(0, m_doc->nodeEdit.clre2() + 2, Clip);
		if (foundP)
		{
			npf2 = nearPoint;
			FPoint base = cli.point(cli.size() - 2);
			FPoint c1 = cli.point(cli.size() - 1);
			const FPoint& base2 =  Clip.point(m_doc->nodeEdit.clre2() + 2);
			const FPoint& c2 = Clip.point(m_doc->nodeEdit.clre2() + 3);
			if ((base == c1) && (base2 == c2))
			{
				cli.resize(cli.size() + 4);
				cli.putPoints(cli.size() - 4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
				cli.putPoints(cli.size(), Clip.size() - (m_doc->nodeEdit.clre2() + 2), Clip, m_doc->nodeEdit.clre2() + 2);
			}
			else
			{
				FPoint cn1 = (1.0 - nearT) * base + nearT * c1;
				FPoint cn2 = (1.0 - nearT) * cn1 + nearT * ((1.0 - nearT) * c1 + nearT * c2);
				FPoint cn3 = (1.0 - nearT) * ((1.0 - nearT) * c1 + nearT * c2) + nearT * ((1.0 - nearT) * c2 + nearT * base2);
				FPoint cn4 = (1.0 - nearT) * c2 + nearT * base2;
				cli.setPoint(cli.size() - 1, cn1);
				cli.resize(cli.size() + 4);
				uint basind = cli.size() + 1;
				cli.putPoints(cli.size() - 4, 4, npf2.x(), npf2.y(), cn2.x(), cn2.y(), npf2.x(), npf2.y(), cn3.x(), cn3.y());
				cli.putPoints(cli.size(), Clip.size() - (m_doc->nodeEdit.clre2() + 2), Clip, m_doc->nodeEdit.clre2() + 2);
				cli.setPoint(basind, cn4);
			}
		}
		else
		{
			cli.resize(cli.size() + 4);
			cli.putPoints(cli.size() - 4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
			cli.putPoints(cli.size(), Clip.size() - (m_doc->nodeEdit.clre2() + 2), Clip, m_doc->nodeEdit.clre2() + 2);
		}
		if (m_doc->nodeEdit.isContourLine())
			currItem->ContourLine = cli.copy();
		else
			currItem->PoLine = cli.copy();
		m_doc->nodeEdit.setClre2(-1);
		currItem->ClipEdited = true;
		edited = true;
	}

	refreshPath(edited);

	if ((m_doc->nodeEdit.selectionCount() != 0) || ((m_doc->nodeEdit.segP1() != -1) && (m_doc->nodeEdit.segP2() != -1)) || (m_doc->nodeEdit.hasNodeSelected() && state.clickedOn == ControlHandle))
	{
		m_Mxp = m->position().x();
		m_Myp = m->position().y();
		m_canvas->setRenderModeFillBuffer();
	}
	else
	{
		m_Mxp = m->position().x();
		m_Myp = m->position().y();
		m_Dxp = qRound(m->position().x() / m_canvas->scale());  // + m_doc->minCanvasCoordinate.x());
		m_Dyp = qRound(m->position().y() / m_canvas->scale());  // + m_doc->minCanvasCoordinate.y());
		if (!m_rectangleSelect)
			m_rectangleSelect = new RectSelect(this);
		m_rectangleSelect->prepare(m->globalPosition());
		m_view->startGesture(m_rectangleSelect);
	}

}

bool CanvasMode_NodeEdit::handleNodeEditMove(QMouseEvent* m, QRect /*r*/, PageItem* currItem, const QTransform& /*t*/)
{
	QTransform itemPos = currItem->getTransform();
	if ((currItem->isSymbol() || currItem->isGroup()) && (!m_doc->nodeEdit.isContourLine()))
	{
		if (currItem->imageFlippedH())
		{
			itemPos.translate(currItem->width(), 0);
			itemPos.scale(-1, 1);
		}

		if (currItem->imageFlippedV())
		{
			itemPos.translate(0, currItem->height());
			itemPos.scale(1, -1);
		}
	}
	FPointArray Clip = m_doc->nodeEdit.clipFromItem(currItem);
	m_doc->nodeEdit.setClre2(-1);
	m_doc->nodeEdit.setSegP1(-1);
	m_doc->nodeEdit.setSegP2(-1);
	state.hoveredOver = None;

	// Hover mouse on node or control handle
	if ((m_doc->nodeEdit.submode() == NodeEditContext::DEL_POINT) ||
		(m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT) ||
		(m_doc->nodeEdit.submode() == NodeEditContext::SPLIT_PATH) ||
		(m_doc->nodeEdit.submode() == NodeEditContext::EDIT_POINT))
	{
		for (int i = 0; i < Clip.size(); ++i)
		{
			QPointF point = Clip.pointQF(i);
			if (m_canvas->hitsCanvasPoint(m->globalPosition(), itemPos.map(point)))
			{
				if (m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT)
					m_view->setCursor(IconManager::instance().loadCursor("cursor-move", 1, 1));				
				else if (m_doc->nodeEdit.submode() == NodeEditContext::EDIT_POINT)
					m_view->setCursor(IconManager::instance().loadCursor("cursor-node-edit", 1, 1));

				if (NodeEditContext::isNode(i))
				{
					if (m_doc->nodeEdit.submode() == NodeEditContext::DEL_POINT || (m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT && m->modifiers() == Qt::ControlModifier))
						m_view->setCursor(IconManager::instance().loadCursor("cursor-remove-point", 1, 1));
					else if (m_doc->nodeEdit.submode() == NodeEditContext::SPLIT_PATH)
						m_view->setCursor(IconManager::instance().loadCursor("cursor-split", 1, 1));

					state.hoveredOver = NodeHandle;
				}
				else
					state.hoveredOver = ControlHandle;

				return true;
			}
		}
	}

	// Hover mouse on path segment
	if ((m_doc->nodeEdit.submode() == NodeEditContext::ADD_POINT) ||
		(m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT) ||
		(m_doc->nodeEdit.submode() == NodeEditContext::SPLIT_PATH))
	{
		for (int poi = 0; poi < Clip.size() - 3; poi += 4)
		{
			// create bezier curve in canvas coords
			QPointF a1 = itemPos.map(Clip.pointQF(poi));
			QPointF a2 = itemPos.map(Clip.pointQF(poi + 1));
			QPointF a3 = itemPos.map(Clip.pointQF(poi + 3));
			QPointF a4 = itemPos.map(Clip.pointQF(poi + 2));
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
					if (m_canvas->hitsCanvasPoint(m->globalPosition(), FPoint(pl.x(), pl.y())))
					{
						if (m_doc->nodeEdit.submode() == NodeEditContext::ADD_POINT || (m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT && m->modifiers() == Qt::ControlModifier))
							m_view->setCursor(IconManager::instance().loadCursor("cursor-add-point", 1, 1));
						else if (m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT)
							m_view->setCursor(IconManager::instance().loadCursor("cursor-hand"));
						else if (m_doc->nodeEdit.submode() == NodeEditContext::SPLIT_PATH)
							m_view->setCursor(IconManager::instance().loadCursor("cursor-split", 1, 1));
						else
							m_view->setCursor(QCursor(Qt::ArrowCursor));

						m_doc->nodeEdit.setClre2(poi);
						state.hoveredOver = PathSegment;
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
	// Selection Frame
	if ((m_canvas->m_viewMode.m_MouseButtonPressed) && !m_doc->nodeEdit.hasNodeSelected() && (m_doc->nodeEdit.segP1()  == -1) && (m_doc->nodeEdit.segP2() == -1))
	{
		if (!m_rectangleSelect)
			m_rectangleSelect = new RectSelect(this);
		m_rectangleSelect->prepare(m->globalPosition());
		m_view->startGesture(m_rectangleSelect);
		return;
	}

	FPoint npf;
	double newX = m->position().x();
	double newY = m->position().y();
	FPoint np(newX - m_Mxp, newY - m_Myp, 0, 0, currItem->rotation(), 1, 1, true);
	np = np * (1.0 / m_canvas->scale());
	m_canvas->m_viewMode.operItemMoving = true;
	currItem = m_doc->m_Selection->itemAt(0);

	// Move Path Segment
	if ((m_doc->nodeEdit.segP1() != -1) && (m_doc->nodeEdit.segP2() != -1))
	{
		if ((currItem->isSymbol() || currItem->isGroup()) && (!m_doc->nodeEdit.isContourLine()))
		{
			if (currItem->imageFlippedH())
				np.setX(np.x() * -1);
			if (currItem->imageFlippedV())
				np.setY(np.y() * -1);
		}

		// First segment
		npf = m_doc->nodeEdit.clipFromItem(currItem).point(m_doc->nodeEdit.segP2()) + np;
		m_doc->nodeEdit.setClre(m_doc->nodeEdit.segP2());
		m_doc->nodeEdit.moveClipPoint(currItem, npf);

		// Second segment
		npf = m_doc->nodeEdit.clipFromItem(currItem).point(m_doc->nodeEdit.segP1()) + np;
		m_doc->nodeEdit.setClre(m_doc->nodeEdit.segP1());
		m_doc->nodeEdit.moveClipPoint(currItem, npf);
		m_Mxp = newX;
		m_Myp = newY;
	}
	// Move Nodes or Controls
	else
	{
		if ((m_doc->nodeEdit.selectionCount() > 0) && (state.clickedOn == NodeHandle || state.clickedOn == ControlHandle))
		{
			// Single Nodes
			if (m_doc->nodeEdit.selectionCount() == 1)
			{
				npf = m_canvas->globalToCanvas(m->globalPosition());
				double nx = npf.x();
				double ny = npf.y();
				if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny, true))
					npf = m_doc->ApplyGridF(FPoint(nx, ny));
				else
					npf = FPoint(nx, ny);

				QTransform pp = currItem->getTransform();
				npf = npf.transformPoint(pp, true);

				if ((currItem->isSymbol() || currItem->isGroup()) && (!m_doc->nodeEdit.isContourLine()))
				{
					if (currItem->imageFlippedH())
					{
						QTransform p;
						p.translate(currItem->width(), 0);
						p.scale(-1, 1);
						npf = npf.transformPoint(p, false);
					}
					if (currItem->imageFlippedV())
					{
						QTransform p;
						p.translate(0, currItem->height());
						p.scale(1, -1);
						npf = npf.transformPoint(p, false);
					}
				}

				int curr = m_doc->nodeEdit.clre();
				FPoint pOpposite = m_doc->nodeEdit.getPoint(currItem, m_doc->nodeEdit.oppositeControl.nodeIndex);
				FPoint pSibling = m_doc->nodeEdit.getPoint(currItem, curr, NodeEditContext::Sibling);

				if (m->modifiers() & Qt::ShiftModifier || m_doc->nodeEdit.submode() == NodeEditContext::EDIT_POINT)
				{
					// Shift + drag on control
					if (!NodeEditContext::isNode(curr) && state.clickedOn == ControlHandle)
					{
						m_doc->nodeEdit.setMoveMode(NodeEditContext::Independent);

						if (m_doc->nodeEdit.oppositeControl.isValid)
						{
							m_doc->nodeEdit.setClre(m_doc->nodeEdit.oppositeControl.controlIndex);
							m_doc->nodeEdit.moveClipPoint(currItem, pOpposite + m_doc->nodeEdit.oppositeControl.relativePos);
							m_doc->nodeEdit.setClre(curr);
						}
					}

					// Shift + drag on node
					if (NodeEditContext::isNode(curr) && state.clickedOn == NodeHandle)
					{
						m_doc->nodeEdit.setMoveMode(NodeEditContext::Symmetric);
						m_doc->nodeEdit.setClre(curr + 1);
					}
				}
				else
					m_doc->nodeEdit.setMoveMode(state.moveMode);

				if (m->modifiers() & Qt::ControlModifier)
				{
					// Snap control handle at 15° steps
					if (!NodeEditContext::isNode(curr))
						npf = constraintControlAngle(m_doc->nodeEdit.getPoint(currItem, curr, NodeEditContext::Sibling), npf, 15.0);

				}

				// Snap control handle
				if (!NodeEditContext::isNode(curr) && state.clickedOn == ControlHandle)
				{
					state.isSameAngle = m_doc->nodeEdit.sameAngle(currItem, curr);
					state.isSameLength = m_doc->nodeEdit.sameLength(currItem, curr);

					bool modifier = m->modifiers() & Qt::ShiftModifier || m_doc->nodeEdit.submode() == NodeEditContext::EDIT_POINT;

					// let's snap the control angle to opposite reference
					if (state.isSameAngle && (m_doc->nodeEdit.moveMode() == NodeEditContext::Auto || m_doc->nodeEdit.moveMode() == NodeEditContext::Independent || modifier))
						npf = snapControlAngle(pOpposite, npf, m_doc->nodeEdit.oppositeControl.angle - 180.0);

					// let's snap the control length to opposite reference
					if (state.isSameLength && state.isSameAngle && (m_doc->nodeEdit.moveMode() != NodeEditContext::Symmetric || modifier))
						npf = snapControlLength(pOpposite, npf, m_doc->nodeEdit.oppositeControl.distance);

					// let's snap the control to same position as control anchor if control is too close
					npf = snapControlOrigin(pSibling, npf);
				}

				// Snap node handle
				// if (NodeEditContext::isNode(curr) && state.clickedOn == NodeHandle)
				// {
				// 	FPointArray Clip = m_doc->nodeEdit.isContourLine() ? currItem->ContourLine : currItem->PoLine;
				// 	FPoint pOppositeNode = m_doc->nodeEdit.getPoint(currItem, curr, NodeEditContext::Opposite);

				// 	if (curr == 0 || curr == Clip.size() -2)
				// 	{
				//      // TODO: close bezier curve and merge endpoints
				// 		npf = snapControlOrigin(pOppositeNode, npf);
				// 	}
				// }

				m_doc->nodeEdit.moveClipPoint(currItem, npf);
				m_canvas->displayXYHUD(m->globalPosition(), npf.x(), npf.y());
			}
			// Multiple Nodes
			else
			{
				if ((currItem->isSymbol() || currItem->isGroup()) && (!m_doc->nodeEdit.isContourLine()))
				{
					if (currItem->imageFlippedH())
						np.setX(np.x() * -1);
					if (currItem->imageFlippedV())
						np.setY(np.y() * -1);
				}

				const auto& nodeSelection = m_doc->nodeEdit.selNode();
				for (int itm = 0; itm < nodeSelection.count(); ++itm)
				{
					m_doc->nodeEdit.setClre(nodeSelection.at(itm));
					npf = m_doc->nodeEdit.clipFromItem(currItem).point(m_doc->nodeEdit.clre()) + np;
					m_doc->nodeEdit.moveClipPoint(currItem, npf);
				}
			}
		}

		m_Mxp = newX;
		m_Myp = newY;

	}
	m_canvas->m_viewMode.operItemMoving = false;
	m_doc->regionsChanged()->update(QRectF());
}

bool CanvasMode_NodeEdit::deleteNodes()
{
	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	FPointArray Clip = m_doc->nodeEdit.beginTransaction(currItem);

	if (!currItem || Clip.size() == 0)
		return false;

	FPointArray cli;
	int EndInd = Clip.size();
	int StartInd = 0;
	if (m_doc->nodeEdit.clre() > 0)
	{
		for (int n = m_doc->nodeEdit.clre(); n < Clip.size(); ++n)
		{
			if (Clip.isMarker(n))
			{
				EndInd = n;
				break;
			}
		}
		for (int n2 = m_doc->nodeEdit.clre(); n2 > 0; n2--)
		{
			if (Clip.isMarker(n2))
			{
				StartInd = n2 + 1;
				break;
			}
		}
	}

	if (m_doc->nodeEdit.hasNodeSelected())
	{
		int minClipSize = 4;
		if (!currItem->isLine() && !currItem->isPathText() && !currItem->isPolyLine())
			minClipSize = 12;

		if (Clip.size() <= minClipSize)
		{
			m_doc->nodeEdit.deselect();
			return false;
		}

		int numPoints = (StartInd != 0) ? (StartInd-4) : 0;
		numPoints += (Clip.size() - EndInd);
		if ((currItem->Segments.count() > 0) && ((EndInd - StartInd) <= minClipSize) && (numPoints > 0))
		{
			if (StartInd != 0)
				cli.putPoints(0, StartInd - 4, Clip);
			cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
		}
		else if (m_doc->nodeEdit.clre() == StartInd)
		{
			if (!currItem->isLine() && !currItem->isPathText() && !currItem->isPolyLine())
			{
				const FPoint& kp(Clip.point(EndInd - 3));
				cli.putPoints(0, StartInd, Clip);
				cli.putPoints(cli.size(), EndInd - StartInd - 4, Clip, StartInd);
				cli.setPoint(StartInd, cli.point(cli.size() - 2));
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
		else if (m_doc->nodeEdit.clre() == (EndInd - 2))
		{
			Clip.remove(m_doc->nodeEdit.clre() - 2, 4);
			cli = Clip.copy();
		}
		else
		{
			if (m_doc->nodeEdit.clre() != 0)
				cli.putPoints(0, m_doc->nodeEdit.clre(), Clip);
			cli.putPoints(cli.size(), Clip.size() - (m_doc->nodeEdit.clre() + 4), Clip, m_doc->nodeEdit.clre() + 4);
		}
		while (cli.isMarker(0))
			cli.remove(0, 4);
		if (m_doc->nodeEdit.isContourLine())
			currItem->ContourLine = cli.copy();
		else
			currItem->PoLine = cli.copy();
		m_doc->nodeEdit.deselect();
		currItem->ClipEdited = true;
		return true;
	}


	return false;
}

void CanvasMode_NodeEdit::refreshPath(bool edited)
{
	PageItem* currItem = m_doc->m_Selection->itemAt(0);

	if (!currItem)
		return;

	if (edited)
	{
		currItem->FrameType = 3;
		double xp = currItem->xPos();
		double yp = currItem->yPos();
		double w = currItem->width();
		double h = currItem->height();
		m_doc->adjustItemSize(currItem, true);
		double xp2 = currItem->xPos();
		double yp2 = currItem->yPos();
		double w2 = currItem->width();
		double h2 = currItem->height();
		currItem->update();
		if ((xp != xp2) || (yp != yp2) || (w != w2) || (h != h2))
			m_view->DrawNew();
		emit m_view->PolyStatus(currItem->itemType(), currItem->PoLine.size());
	}
	else
	{
		currItem->update();
		m_view->DrawNew();
	}
}
