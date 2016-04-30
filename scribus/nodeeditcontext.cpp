#include "nodeeditcontext.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "util_math.h"

NodeEditContext::NodeEditContext() :
	oldClip(NULL),
	nodeTransaction(NULL),
	m_submode(MOVE_POINT),
	m_isContourLine(false),
	m_ClRe(-1),
	m_ClRe2(-1),
	m_SegP1(-1),
	m_SegP2(-1),
	m_EdPoints(true),
	m_MoveSym(false),
	m_SelNode(),
	m_oldItemX(0.0),
	m_oldItemY(0.0),
	m_preview(false)
{
}

bool NodeEditContext::hasNodeSelected() 
{ 
	return m_ClRe != -1;
}


void NodeEditContext::deselect() 
{ 
	m_ClRe = -1;
	m_SelNode.clear();
}

void NodeEditContext::reset()
{
	m_submode = MOVE_POINT;
	m_isContourLine = false;
	m_ClRe = -1;
	m_ClRe2 = -1;
	m_SegP1 = -1;
	m_SegP2 = -1;
	delete oldClip;
	oldClip = NULL;
	nodeTransaction.reset();
	m_MoveSym = false;
	m_SelNode.clear();
	m_preview = false;
}

void NodeEditContext::setPreviewMode(bool mode)
{
	m_preview = mode;
}

FPointArray NodeEditContext::beginTransaction(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();
	
	FPointArray Clip;
	QString uAction;
	if (Doc->nodeEdit.m_isContourLine)
	{
		Clip = currItem->ContourLine;
		//					m_isContourLine = true;
		uAction = Um::EditContour;
	}
	else
	{
		Clip = currItem->PoLine;
		//					m_isContourLine = false;
		uAction = Um::EditShape;
	}
	oldClip = new FPointArray(Clip);
	m_oldItemX = currItem->xPos();
	m_oldItemY = currItem->yPos();
	if (UndoManager::undoEnabled())
		nodeTransaction = undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(), uAction);
	Doc->nodeEdit.deselect();
	return Clip;
}



void NodeEditContext::finishTransaction(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();
	
	if (nodeTransaction) // is there the old clip stored for the undo action
	{
		FPointArray newClip(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine : currItem->PoLine);
		if (*oldClip != newClip && UndoManager::undoEnabled())
		{
			QString name = Doc->nodeEdit.m_isContourLine ? Um::EditContour : Um::EditShape;
			ScItemState<QPair<FPointArray, FPointArray> > *state =
				new ScItemState<QPair<FPointArray, FPointArray> >(name);
			state->set("EDIT_SHAPE_OR_CONTOUR");
			state->set("IS_CONTOUR", Doc->nodeEdit.m_isContourLine);
			state->setItem(qMakePair(*oldClip, newClip));
			state->set("OLD_X", m_oldItemX);
			state->set("OLD_Y", m_oldItemY);
			state->set("NEW_X", currItem->xPos());
			state->set("NEW_Y", currItem->yPos());
			undoManager->action(currItem, state);
			nodeTransaction.commit();
		}
		else
			nodeTransaction.cancel();
		
		nodeTransaction.reset();
		delete oldClip;
		oldClip = 0;
	}
}


/**
  first part: create a new UndoState or cancel the current transaction
 */
ScItemState<QPair<FPointArray, FPointArray> >* NodeEditContext::finishTransaction1(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();
	ScItemState<QPair<FPointArray, FPointArray> >* state = NULL;
	
	if (nodeTransaction) // is there the old clip stored for the undo action
	{
		FPointArray newClip(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine : currItem->PoLine);
		if (*oldClip != newClip && UndoManager::undoEnabled())
		{
			QString name = Doc->nodeEdit.m_isContourLine ? Um::EditContour : Um::EditShape;
			state = new ScItemState<QPair<FPointArray, FPointArray> >(name);
			state->set("EDIT_SHAPE_OR_CONTOUR");
			state->set("IS_CONTOUR", Doc->nodeEdit.m_isContourLine);
			state->setItem(qMakePair(*oldClip, newClip));
			undoManager->setUndoEnabled(false);
		}
		else
		{
			delete oldClip;
			oldClip = 0;
			nodeTransaction.cancel();
		}
	}
	return state;
}

/**
  second part: take the UndoState returned from finishTransaction1() and commit it
*/
void NodeEditContext::finishTransaction2(PageItem* currItem, ScItemState<QPair<FPointArray, FPointArray> >* state)
{
	UndoManager* undoManager = UndoManager::instance();

	state->set("OLD_X", m_oldItemX);
	state->set("OLD_Y", m_oldItemY);
	state->set("NEW_X", currItem->xPos());
	state->set("NEW_Y", currItem->yPos());
	undoManager->setUndoEnabled(true);
	undoManager->action(currItem, state);
	nodeTransaction.commit();
	nodeTransaction.reset();
	delete oldClip;
	oldClip = 0;				
}	
	

//CB-->Doc
void NodeEditContext::moveClipPoint(PageItem *currItem, FPoint ip)
{
	ScribusDoc* Doc = currItem->doc();
	if (((m_EdPoints) && (m_ClRe % 2 != 0)) || ((!m_EdPoints) && (m_ClRe % 2 == 0)))
		return;
	double xposOrig = currItem->xPos();
	double yposOrig = currItem->yPos();
	currItem->ClipEdited = true;
	FPointArray Clip;
	if (m_isContourLine)
		Clip = currItem->ContourLine.copy();
	else
		Clip = currItem->PoLine.copy();
	currItem->FrameType = 3;
	uint EndInd = Clip.size();
	uint StartInd = 0;
	for (int n = m_ClRe; n < Clip.size(); ++n)
	{
		if (Clip.isMarker(n))
		{
			EndInd = n;
			break;
		}
	}
	if (m_ClRe > 0)
	{
		for (uint n2 = m_ClRe; n2 > 0; n2--)
		{
			if (n2 == 0)
				break;
			if (Clip.isMarker(n2))
			{
				StartInd = n2 + 1;
				break;
			}
		}
	}
	FPoint np(ip);
	if (hasNodeSelected())
	{
		if ((np.x() < 0) && (!m_isContourLine) && (!(currItem->isGroup() || currItem->isSymbol())))
		{
			Doc->sizeItem(currItem->width() - np.x(), currItem->height(), currItem, false, false, false);
			if (currItem->rotation() != 0)
			{
				FPoint npv(np.x(), 0);
				Doc->moveRotated(currItem, npv);
			}
			else
				Doc->moveItem(np.x(), 0, currItem);
			Clip.translate(-np.x(), 0);
			if (!currItem->imageFlippedH())
				currItem->moveImageInFrame(-np.x()/currItem->imageXScale(), 0);
			np.setX(0);
		}
		if ((np.y() < 0) && (!m_isContourLine) && (!(currItem->isGroup() || currItem->isSymbol())))
		{
			Doc->sizeItem(currItem->width(), currItem->height() - np.y(), currItem, false, false, false);
			if (currItem->rotation() != 0)
			{
				FPoint npv(0, np.y());
				Doc->moveRotated(currItem, npv);
			}
			else
				Doc->moveItem(0, np.y(), currItem);
			Clip.translate(0, -np.y());
			if (!currItem->imageFlippedV())
				currItem->moveImageInFrame(0, -np.y()/currItem->imageYScale());
			np.setY(0);
		}
		update(QPointF(np.x(), np.y()));
		if ((m_ClRe+1 < static_cast<int>(EndInd)) && (m_ClRe % 2 == 0))
		{
			FPoint ap(Clip.point(m_ClRe));
			FPoint ap2(Clip.point(m_ClRe+1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(m_ClRe+1, ap2);
		}
		Clip.setPoint(m_ClRe, np);
		if (((m_ClRe % 4 != 0) && (m_ClRe % 2 == 0)) && (m_ClRe+3 < static_cast<int>(EndInd)) && (m_ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(m_ClRe+2));
			FPoint ap2(Clip.point(m_ClRe+3));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(m_ClRe+3, ap2);
			Clip.setPoint(m_ClRe+2, np);
		}
		if ((m_ClRe % 4 == 0) && (m_ClRe+3 < static_cast<int>(EndInd)) && (m_ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(m_ClRe-2));
			FPoint ap2(Clip.point(m_ClRe-1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(m_ClRe-1, ap2);
			Clip.setPoint(m_ClRe-2, np);
		}
		if (((m_ClRe == static_cast<int>(StartInd)) || (m_ClRe == static_cast<int>(EndInd-2))) &&
			((currItem->itemType() == PageItem::Polygon)
			|| (currItem->itemType() == PageItem::Group)
			|| (currItem->itemType() == PageItem::Symbol)
			|| (currItem->itemType() == PageItem::TextFrame)
			|| (currItem->itemType() == PageItem::ImageFrame)))
		{
			if (m_ClRe == static_cast<int>(StartInd))
			{
				FPoint ap(Clip.point(EndInd-2));
				FPoint ap2(Clip.point(EndInd-1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(EndInd-2, Clip.point(StartInd));
				Clip.setPoint(EndInd-1, ap2);
			}
			else
			{
				FPoint ap(Clip.point(StartInd));
				FPoint ap2(Clip.point(StartInd + 1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(StartInd, Clip.point(EndInd-2));
				Clip.setPoint(StartInd + 1, ap2);
			}
		}
		if (((m_ClRe == static_cast<int>(StartInd+1)) || (m_ClRe == static_cast<int>(EndInd-1))) &&
			((currItem->itemType() == PageItem::Polygon)
			|| (currItem->itemType() == PageItem::Group)
			|| (currItem->itemType() == PageItem::Symbol)
			|| (currItem->itemType() == PageItem::TextFrame)
			|| (currItem->itemType() == PageItem::ImageFrame)) && 
			(m_MoveSym))
		{
			uint kon = 0;
			if (m_ClRe == static_cast<int>(StartInd+1))
				kon = EndInd-1;
			else
				kon = StartInd + 1;
			FPoint lxy(Clip.point(m_ClRe-1));
			FPoint lk(Clip.point(m_ClRe));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			Clip.setPoint(kon, lk);
		}
		if ((m_ClRe % 2 != 0) && (m_MoveSym) && (m_ClRe != static_cast<int>(StartInd + 1)) && (m_ClRe != static_cast<int>(EndInd-1)))
		{
			uint kon = 0;
			if ((m_ClRe+1) % 4 == 0)
				kon = m_ClRe + 2;
			else
				kon = m_ClRe - 2;
			FPoint lxy(Clip.point(m_ClRe-1));
			FPoint lk(Clip.point(m_ClRe));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			Clip.setPoint(kon, lk);
		}
		if (Doc->nodeEdit.m_isContourLine)
			currItem->ContourLine = Clip.copy();
		else
			currItem->PoLine = Clip.copy();
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	}
	if (!m_isContourLine)
		currItem->ContourLine.translate(xposOrig - currItem->xPos(), yposOrig - currItem->yPos());
}


void NodeEditContext::reset1Control(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	if ((m_ClRe < 0) || (m_ClRe % 2 == 0))
		return;
	UndoManager* undoManager = UndoManager::instance();

	// do no record anything else but the core reset points action
	undoManager->setUndoEnabled(false);
	
	oldClip = new FPointArray(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine.copy() : currItem->PoLine.copy());
	m_oldItemX = currItem->xPos();
	m_oldItemY = currItem->yPos();
	currItem->ClipEdited = true;
	FPoint np;
	if (Doc->nodeEdit.m_isContourLine)
		np = currItem->ContourLine.point(Doc->nodeEdit.m_ClRe-1);
	else
		np = currItem->PoLine.point(Doc->nodeEdit.m_ClRe-1);
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if (Doc->nodeEdit.m_isContourLine)
	{
		currItem->ContourLine.setPoint(Doc->nodeEdit.m_ClRe, np);
		Doc->regionsChanged()->update(QRectF());
		currItem->FrameOnly = true;
		currItem->update();
	}
	else
	{
		currItem->PoLine.setPoint(Doc->nodeEdit.m_ClRe, np);
	//	if (!(currItem->isGroup() || currItem->isSymbol()))
			Doc->adjustItemSize(currItem, true, true);
		Doc->regionsChanged()->update(QRectF());
	}
	undoManager->setUndoEnabled(true);
	FPointArray newClip(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine : currItem->PoLine);
	if (*oldClip != newClip && UndoManager::undoEnabled())
	{
		ScItemState<QPair<FPointArray, FPointArray> > *state =
		new ScItemState<QPair<FPointArray, FPointArray> >(Um::ResetControlPoint, "",
														currItem->getUPixmap());
		state->set("EDIT_SHAPE_OR_CONTOUR");
		state->set("IS_CONTOUR", Doc->nodeEdit.m_isContourLine);
		state->setItem(qMakePair(*oldClip, newClip));
		state->set("OLD_X", m_oldItemX);
		state->set("OLD_Y", m_oldItemY);
		state->set("NEW_X", currItem->xPos());
		state->set("NEW_Y", currItem->yPos());
		undoManager->action(currItem, state);
	}
	delete oldClip;
	oldClip = 0;
}	


void NodeEditContext::resetControl(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	if (m_ClRe < 0)
		return;
	UndoManager* undoManager = UndoManager::instance();
	
	// do no record anything else but the core reset points action
	undoManager->setUndoEnabled(false);
	
	oldClip = new FPointArray(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine.copy() : currItem->PoLine.copy());
	m_oldItemX = currItem->xPos();
	m_oldItemY = currItem->yPos();
	currItem->ClipEdited = true;
	FPointArray Clip;
	if (Doc->nodeEdit.m_isContourLine)
		Clip = currItem->ContourLine.copy();
	else
		Clip = currItem->PoLine.copy();
	int EndInd = Clip.size();
	int StartInd = 0;
	for (int n = m_ClRe; n < Clip.size(); ++n)
	{
		if (Clip.isMarker(n))
		{
			EndInd = n;
			break;
		}
	}
	if (m_ClRe > 0)
	{
		for (uint n2 = m_ClRe; n2 > 0; n2--)
		{
			if (n2 == 0)
				break;
			if (Clip.isMarker(n2))
			{
				StartInd = n2 + 1;
				break;
			}
		}
	}
	FPoint np = Clip.point(Doc->nodeEdit.clre());
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if ((Doc->nodeEdit.clre() == StartInd) || (Doc->nodeEdit.clre() == EndInd-2))
	{
		if (currItem->asPolyLine())
		{
			if (Doc->nodeEdit.clre() == StartInd)
				Clip.setPoint(StartInd+1, np);
			else
				Clip.setPoint(EndInd-1, np);
		}
		else
		{
			Clip.setPoint(StartInd+1, np);
			Clip.setPoint(EndInd-1, np);
		}
	}
	else
	{
		Clip.setPoint(Doc->nodeEdit.m_ClRe+1, np);
		Clip.setPoint((Doc->nodeEdit.m_ClRe % 4 != 0 ? Doc->nodeEdit.m_ClRe + 3 : Doc->nodeEdit.m_ClRe - 1), np);
	}
	if (!Doc->nodeEdit.m_isContourLine)
	{
		currItem->PoLine = Clip.copy();
	//	if (!(currItem->isGroup() || currItem->isSymbol()))
			Doc->adjustItemSize(currItem, true, true);
		Doc->regionsChanged()->update(QRectF());
	}
	else
	{
		currItem->ContourLine = Clip.copy();
		Doc->regionsChanged()->update(QRectF());
	}
	Doc->update();
	
	undoManager->setUndoEnabled(true);
	FPointArray newClip(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine : currItem->PoLine);
	if (*oldClip != newClip && UndoManager::undoEnabled())
	{
		ScItemState<QPair<FPointArray, FPointArray> > *state =
		new ScItemState<QPair<FPointArray, FPointArray> >(Um::ResetControlPoints, "",
														currItem->getUPixmap());
		state->set("EDIT_SHAPE_OR_CONTOUR");
		state->set("IS_CONTOUR", Doc->nodeEdit.m_isContourLine);
		state->setItem(qMakePair(*oldClip, newClip));
		state->set("OLD_X", m_oldItemX);
		state->set("OLD_Y", m_oldItemY);
		state->set("NEW_X", currItem->xPos());
		state->set("NEW_Y", currItem->yPos());
		undoManager->action(currItem, state);
	}
	delete oldClip;
	oldClip = 0;
	
}

