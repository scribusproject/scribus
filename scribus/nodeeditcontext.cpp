#include "nodeeditcontext.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "util_math.h"

NodeEditContext::NodeEditContext() :
	nodeTransaction(nullptr)
{
}

bool NodeEditContext::hasNodeSelected() const
{ 
	return m_ClRe != -1;
}

void NodeEditContext::deselect() 
{ 
	m_ClRe = -1;
	m_SelNode.clear();
}

void NodeEditContext::selectNode(int i)
{
	m_ClRe = i;
	if (!m_SelNode.contains(i))
		m_SelNode.append(i);
}

void NodeEditContext::deselectNode(int i)
{
	int index = m_SelNode.indexOf(i);
	m_SelNode.removeAll(i);
	if (m_SelNode.count() == 0)
		m_ClRe = -1;
	else // select previous node or last one if there is no previous one
		m_ClRe = (index - 1 >= 0) ? m_SelNode.at(index - 1) : m_SelNode.last();
}

void NodeEditContext::deselectNodeControls()
{
	for (int i = 0; i < m_SelNode.count(); i++)
	{
		if (!isNode(m_SelNode.at(i)))
		{
			m_SelNode.removeAt(i);
			i --;
		}
	}
	if (m_SelNode.count() == 0)
		m_ClRe = -1;
}

int NodeEditContext::selectionCount() const
{
	return m_SelNode.count();
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
	oldClip = nullptr;
	nodeTransaction.reset();
	m_moveMode = Auto;
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
		uAction = Um::EditContour;
	}
	else
	{
		Clip = currItem->PoLine;
		uAction = Um::EditShape;
	}

	if (nodeTransaction)
		return (oldClip ? FPointArray(*oldClip) : Clip);

	delete oldClip;
	oldClip = new FPointArray(Clip);

	m_oldItemX = currItem->xPos();
	m_oldItemY = currItem->yPos();
	if (UndoManager::undoEnabled())
		nodeTransaction = undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(), uAction);
	return Clip;
}

void NodeEditContext::finishTransaction(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();

	if (nodeTransaction.isNull()) // is there the old clip stored for the undo action
		return;
	
	FPointArray newClip(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine : currItem->PoLine);
	if (UndoManager::undoEnabled() && oldClip && (*oldClip != newClip))
	{
		QString name = Doc->nodeEdit.m_isContourLine ? Um::EditContour : Um::EditShape;
		auto *state = new ScOldNewState<FPointArray>(name);
		state->set("EDIT_SHAPE_OR_CONTOUR");
		state->set("IS_CONTOUR", Doc->nodeEdit.m_isContourLine);
		state->setStates(*oldClip, newClip);
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
	oldClip = nullptr;
}

/**
  first part: create a new UndoState or cancel the current transaction
 */
ScOldNewState<FPointArray>* NodeEditContext::finishTransaction1(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();
	ScOldNewState<FPointArray>* state = nullptr;
	
	if (nodeTransaction.isNull()) // is there the old clip stored for the undo action
		return nullptr;

	FPointArray newClip(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine : currItem->PoLine);
	if (UndoManager::undoEnabled() && oldClip && (*oldClip != newClip))
	{
		QString name = Doc->nodeEdit.m_isContourLine ? Um::EditContour : Um::EditShape;
		state = new ScOldNewState<FPointArray>(name);
		state->set("EDIT_SHAPE_OR_CONTOUR");
		state->set("IS_CONTOUR", Doc->nodeEdit.m_isContourLine);
		state->setStates(*oldClip, newClip);
		undoManager->setUndoEnabled(false);
	}
	else
	{
		delete oldClip;
		oldClip = nullptr;
		nodeTransaction.cancel();
		nodeTransaction.reset();
	}
	return state;
}

/**
  second part: take the UndoState returned from finishTransaction1() and commit it
*/
void NodeEditContext::finishTransaction2(PageItem* currItem, ScOldNewState<FPointArray>* state)
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
	oldClip = nullptr;
}	

//CB-->Doc
void NodeEditContext::moveClipPoint(PageItem *currItem, const FPoint& ip)
{
	ScribusDoc* Doc = currItem->doc();

	double xposOrig = currItem->xPos();
	double yposOrig = currItem->yPos();
	currItem->ClipEdited = true;

	FPointArray Clip;
	FPoint np(ip);

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
		for (int n2 = m_ClRe; n2 > 0; n2--)
		{
			if (Clip.isMarker(n2))
			{
				StartInd = n2 + 1;
				break;
			}
		}
	}

	if (hasNodeSelected())
	{
		// expand item on left side if x is out of bound
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

		// expand item on top side if y is out of bound
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

		// update control of node
		if (isNode(m_ClRe) && (m_ClRe + 1 < static_cast<int>(EndInd)))
		{
			FPoint ap(Clip.point(m_ClRe));
			FPoint ap2(Clip.point(m_ClRe + 1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(m_ClRe + 1, ap2);
		}

		Clip.setPoint(m_ClRe, np);

		// update right node with control
		if (isLeftNode(m_ClRe) && (m_ClRe + 3 < static_cast<int>(EndInd)) && (m_ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(m_ClRe + 2));
			FPoint ap2(Clip.point(m_ClRe + 3));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(m_ClRe + 3, ap2);
			Clip.setPoint(m_ClRe + 2, np);
		}

		// update left node with control
		if (isRightNode(m_ClRe) && (m_ClRe + 3 < static_cast<int>(EndInd)) && (m_ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(m_ClRe - 2));
			FPoint ap2(Clip.point(m_ClRe - 1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(m_ClRe - 1, ap2);
			Clip.setPoint(m_ClRe - 2, np);
		}

		// update first or last node with control
		if (((m_ClRe == static_cast<int>(StartInd)) || (m_ClRe == static_cast<int>(EndInd - 2))) &&
			((currItem->itemType() == PageItem::Polygon)
			|| (currItem->itemType() == PageItem::Group)
			|| (currItem->itemType() == PageItem::Symbol)
			|| (currItem->itemType() == PageItem::TextFrame)
			|| (currItem->itemType() == PageItem::ImageFrame)))
		{
			if (m_ClRe == static_cast<int>(StartInd))
			{
				FPoint ap(Clip.point(EndInd - 2));
				FPoint ap2(Clip.point(EndInd - 1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(EndInd - 2, Clip.point(StartInd));
				Clip.setPoint(EndInd - 1, ap2);
			}
			else
			{
				FPoint ap(Clip.point(StartInd));
				FPoint ap2(Clip.point(StartInd + 1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(StartInd, Clip.point(EndInd - 2));
				Clip.setPoint(StartInd + 1, ap2);
			}
		}

		bool firstLastControl = ((m_ClRe == static_cast<int>(StartInd + 1)) || (m_ClRe == static_cast<int>(EndInd - 1)))
			&& ((currItem->itemType() == PageItem::Polygon)
			|| (currItem->itemType() == PageItem::Group)
			|| (currItem->itemType() == PageItem::Symbol)
			|| (currItem->itemType() == PageItem::TextFrame)
			|| (currItem->itemType() == PageItem::ImageFrame));

		bool anyControl = !isNode(m_ClRe) && (m_ClRe != static_cast<int>(StartInd + 1)) && (m_ClRe != static_cast<int>(EndInd - 1));

		// update control if there is an opposite control
		if (firstLastControl || anyControl)
		{
			int kon = indexOfNode(m_ClRe, Opposite, Clip.size());

			FPoint pn1(Clip.point(m_ClRe - 1));
			FPoint pc1(Clip.point(m_ClRe));
			FPoint pc2(pn1 + oppositeControl.relativePos);

			double dx = pn1.x() - pc1.x();
			double dy = pn1.y() - pc1.y();

			if (moveMode() == Symmetric)
			{
				pc2.setX(pc1.x() + dx * 2);
				pc2.setY(pc1.y() + dy * 2);
				Clip.setPoint(kon, pc2);
			}
			else if (moveMode() == Asymmetric)
			{
				double dx2 = pn1.x() - pc2.x();
				double dy2 = pn1.y() - pc2.y();

				double angle = xy2Deg(dx, dy);
				double dist = distance(dx2, dy2);
				Clip.setPoint(kon, getPointFromRotation(pn1, dist, angle));
			}
		}

		if (Doc->nodeEdit.m_isContourLine)
			currItem->ContourLine = Clip.copy();
		else
			currItem->PoLine = Clip.copy();

		currItem->Clip = flattenPath(currItem->PoLine, currItem->Segments);
	}

	if (!m_isContourLine)
	{
		// Move the contour accordingly in the item's coordinate space
		QTransform m = QTransform().rotate(-currItem->rotation());
		QPointF delta = m.map(QPointF(xposOrig, yposOrig)) - m.map(QPointF(currItem->xPos(), currItem->yPos()));
		currItem->ContourLine.translate(delta.x(), delta.y());
	}
}

void NodeEditContext::equalizeControls(PageItem* currItem)
{
	FPointArray Clip(clipFromItem(currItem));
	int old = m_ClRe;
	ControlMoveMode cmm = moveMode();

	FPoint cl, nl, nr, cr;
	getPoints(currItem, m_ClRe, cl, nl, nr, cr);

	// if there is no left control we use the right one as reference
	int indexControl = indexOfNode(m_ClRe, (nl == cl) ? ControlRight : ControlLeft, Clip.size());

	// apply changes to opposite control
	setMoveMode(Symmetric);
	setClre(indexControl);
	moveClipPoint(currItem, Clip.point(indexControl));

	// restore old values
	setClre(old);
	setMoveMode(cmm);
}

/**
 * @brief NodeEditContext::getPoints Map all handle indecies based on i. A bezier curve start with right node and ends with left node
 * @param currItem Current PageItem
 * @param i index of any point in page item path
 * @param ctrlLeft return index of left control handle
 * @param nodeLeft return index of left node
 * @param nodeRight return index of right node
 * @param ctrlRight return index of right control handle
 */
void NodeEditContext::getPoints(PageItem* currItem, int i, FPoint& ctrlLeft, FPoint& nodeLeft, FPoint& nodeRight, FPoint& ctrlRight)
{
	i = (i == -1) ? m_ClRe : i;

	if (i < 0)
		return;

	nodeLeft = getPoint(currItem, i, NodeType::NodeLeft);
	ctrlLeft = getPoint(currItem, i, NodeType::ControlLeft);
	nodeRight = getPoint(currItem, i, NodeType::NodeRight);
	ctrlRight = getPoint(currItem, i, NodeType::ControlRight);
}

FPoint NodeEditContext::getPoint(PageItem* currItem, int i, NodeType nodeType)
{
	FPointArray Clip(clipFromItem(currItem));
	return Clip.point(indexOfNode(i, nodeType, Clip.size()));
}

bool NodeEditContext::isSymetric(PageItem* currItem, int i)
{
	return sameAngle(currItem, i) && sameLength(currItem, i);
}

bool NodeEditContext::sameAngle(PageItem* currItem, int i)
{
	double snap = currItem->doc()->prefsData().guidesPrefs.guideRad / m_scale;

	FPoint pcl, pnl, pnr, pcr;
	getPoints(currItem, i, pcl, pnl, pnr, pcr);

	if (pnr != pnl)
		return false;

	QLineF lineLeft(pnl.toQPointF(), pcl.toQPointF());
	QLineF lineRight(pnr.toQPointF(), pcr.toQPointF());
	lineRight.setAngle(lineRight.angle() - 180);

	if (qMax(lineLeft.angle(), lineRight.angle()) - qMin(lineLeft.angle(), lineRight.angle()) <= 0.00001)
		return true;

	if (lineLeft.length() >= lineRight.length())
		lineRight.setLength(lineLeft.length());
	else
		lineLeft.setLength(lineRight.length());

	QLineF distance(lineLeft.p2(), lineRight.p2());

	return distance.length() <= snap ? true : false;
}

bool NodeEditContext::sameLength(PageItem* currItem, int i)
{
	double snap = currItem->doc()->prefsData().guidesPrefs.guideRad / m_scale;
	FPoint pcl, pnl, pnr, pcr;
	getPoints(currItem, i, pcl, pnl, pnr, pcr);

	if (pnl != pnr)
		return false;

	// length = 0
	if (pcl == pcr)
		return true;

	double l1 = QLineF(pnl.toQPointF(), pcl.toQPointF()).length();
	double l2 = QLineF(pnr.toQPointF(), pcr.toQPointF()).length();

	// Check if both controls have same length with difference
	return (qMax(l1, l2) - qMin(l1, l2) <= snap) ? true : false;
}

bool NodeEditContext::isLeftControl(int i)
{
	return (i + 1) % 4 == 0;
}

bool NodeEditContext::isLeftNode(int i)
{
	return (i % 4 != 0) && isNode(i);
}

bool NodeEditContext::isRightNode(int i)
{
	return i % 4 == 0;
}

bool NodeEditContext::isRightControl(int i)
{
	return (i - 1) % 4 == 0;
}

bool NodeEditContext::hasTwoNodes(int i, FPointArray clip)
{
	int ln = indexOfNode(i, NodeLeft, clip.size());
	int rn = indexOfNode(i, NodeRight, clip.size());

	return clip.point(ln) == clip.point(rn);
}

bool NodeEditContext::isSharpNode(int i, FPointArray clip)
{
	int lc = indexOfNode(i, ControlLeft, clip.size());
	int rc = indexOfNode(i, ControlRight, clip.size());
	int ln = indexOfNode(i, NodeLeft, clip.size());
	int rn = indexOfNode(i, NodeRight, clip.size());

	return clip.point(ln) == clip.point(rn) && clip.point(ln) == clip.point(lc) && clip.point(rn) == clip.point(rc);
}

/**
 * @brief NodeEditContext::indexOfNode Returns the node index based on the input index and node type.
 * If size is specified, an index between 0 and size is returned. Values ​​outside this range are corrected.
 * @param i Any index of FPointArray
 * @param type Node type for output
 * @param size Max size of node array
 * @return return index of requested node type based on input index.
 */
int NodeEditContext::indexOfNode(int i, NodeType type, int size)
{
	if (size != -1)
		i = qBound(0, i, size);

	if (isRightControl(i))
	{
		switch (type)
		{
		case NodeType::OppositeSibling:
		case NodeType::NodeLeft:
			i -= 3;
			break;
		case NodeType::Opposite:
		case NodeType::ControlLeft:
			i -= 2;
			break;
		case NodeType::Sibling:
		case NodeType::NodeRight:
			i -= 1;
			break;
		case NodeType::Self:
		case NodeType::ControlRight:
			break;
		}

		if (size != -1 && i < 0)
			i = size - abs(i);
	}
	else if (isLeftControl(i))
	{
		switch (type)
		{
		case NodeType::Sibling:
		case NodeType::NodeLeft:
			i -= 1;
			break;
		case NodeType::Self:
		case NodeType::ControlLeft:
			break;		
		case NodeType::OppositeSibling:
		case NodeType::NodeRight:
			i += 1;
			break;
		case NodeType::Opposite:
		case NodeType::ControlRight:
			i += 2;
			break;
		}

		if (size != -1 && i >= size)
			i -= size;
	}
	else if (isRightNode(i))
	{
		switch (type)
		{
		case NodeType::Opposite:
		case NodeType::NodeLeft:
			i -= 2;
			break;
		case NodeType::OppositeSibling:
		case NodeType::ControlLeft:
			i -= 1;
			break;
		case NodeType::Self:
		case NodeType::NodeRight:
			break;
		case NodeType::Sibling:
		case NodeType::ControlRight:
			i += 1;
			break;
		}

		if (size != -1 && i < 0)
			i = size - abs(i);
	}
	else if (isLeftNode(i))
	{
		switch (type)
		{
		case NodeType::Self:
		case NodeType::NodeLeft:
			break;
		case NodeType::Sibling:
		case NodeType::ControlLeft:
			i += 1;
			break;
		case NodeType::Opposite:
		case NodeType::NodeRight:
			i += 2;
			break;
		case NodeType::OppositeSibling:
		case NodeType::ControlRight:
			i += 3;
			break;
		}

		if (size != -1 && i >= size)
			i -= size;
	}

	return i;
}

void NodeEditContext::reset1Control(PageItem* currItem)
{	
	if ((m_ClRe < 0) || isNode(m_ClRe))
		return;

	ScribusDoc* Doc = currItem->doc();
	UndoManager* undoManager = UndoManager::instance();

	// do no record anything else but the core reset points action
	undoManager->setUndoEnabled(false);
	
	oldClip = new FPointArray(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine.copy() : currItem->PoLine.copy());
	m_oldItemX = currItem->xPos();
	m_oldItemY = currItem->yPos();
	currItem->ClipEdited = true;
	FPoint np;
	if (Doc->nodeEdit.m_isContourLine)
		np = currItem->ContourLine.point(Doc->nodeEdit.m_ClRe - 1);
	else
		np = currItem->PoLine.point(Doc->nodeEdit.m_ClRe - 1);
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
			Doc->adjustItemSize(currItem, true);
		Doc->regionsChanged()->update(QRectF());
	}
	undoManager->setUndoEnabled(true);
	FPointArray newClip(Doc->nodeEdit.m_isContourLine ? currItem->ContourLine : currItem->PoLine);
	if (*oldClip != newClip && UndoManager::undoEnabled())
	{
		auto *state = new ScOldNewState<FPointArray>(Um::ResetControlPoint, "", currItem->getUPixmap());
		state->set("EDIT_SHAPE_OR_CONTOUR");
		state->set("IS_CONTOUR", Doc->nodeEdit.m_isContourLine);
		state->setStates(*oldClip, newClip);
		state->set("OLD_X", m_oldItemX);
		state->set("OLD_Y", m_oldItemY);
		state->set("NEW_X", currItem->xPos());
		state->set("NEW_Y", currItem->yPos());
		undoManager->action(currItem, state);
	}
	delete oldClip;
	oldClip = nullptr;
}	


void NodeEditContext::resetControl(PageItem* currItem)
{
	ScribusDoc* Doc = currItem->doc();
	if (m_ClRe < 0 || !isNode(m_ClRe))
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
		for (int n2 = m_ClRe; n2 > 0; n2--)
		{
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
	if ((Doc->nodeEdit.clre() == StartInd) || (Doc->nodeEdit.clre() == EndInd - 2))
	{
		if (currItem->isPolyLine())
		{
			if (Doc->nodeEdit.clre() == StartInd)
				Clip.setPoint(StartInd + 1, np);
			else
				Clip.setPoint(EndInd - 1, np);
		}
		else
		{
			Clip.setPoint(StartInd + 1, np);
			Clip.setPoint(EndInd - 1, np);
		}
	}
	else
	{
		Clip.setPoint(indexOfNode(Doc->nodeEdit.m_ClRe, NodeType::ControlRight, Clip.size()), np);
		Clip.setPoint(indexOfNode(Doc->nodeEdit.m_ClRe, NodeType::ControlLeft, Clip.size()), np);
	}
	if (!Doc->nodeEdit.m_isContourLine)
	{
		currItem->PoLine = Clip.copy();
	//	if (!(currItem->isGroup() || currItem->isSymbol()))
			Doc->adjustItemSize(currItem, true);
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
		auto *state = new ScOldNewState<FPointArray>(Um::ResetControlPoints, "", currItem->getUPixmap());
		state->set("EDIT_SHAPE_OR_CONTOUR");
		state->set("IS_CONTOUR", Doc->nodeEdit.m_isContourLine);
		state->setStates(*oldClip, newClip);
		state->set("OLD_X", m_oldItemX);
		state->set("OLD_Y", m_oldItemY);
		state->set("NEW_X", currItem->xPos());
		state->set("NEW_Y", currItem->yPos());
		undoManager->action(currItem, state);
	}
	delete oldClip;
	oldClip = nullptr;
}

FPointArray NodeEditContext::clipFromItem(PageItem* currItem)
{
	return isContourLine() ? currItem->ContourLine : currItem->PoLine;
}
