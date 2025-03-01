/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QButtonGroup>
#include <QEvent>

#include "nodeeditpalette.h"
#include "pageitem.h"
#include "scpage.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "scribusview.h" 
#include "selection.h"
#include "undomanager.h"
#include "undostate.h"
#include "units.h"
#include "iconmanager.h"

NodePalette::NodePalette( QWidget* parent) : ScrPaletteBase(parent, "nodePalette", false, Qt::WindowFlags())
{
	setupUi(this);

	MoveNode->setCheckable(true);
	MoveNode->setChecked(true);

	AddNode->setEnabled(true);
	AddNode->setCheckable(true);

	DeleteNode->setEnabled(true);
	DeleteNode->setCheckable(true);

	EditControl->setCheckable(true);
	EditControl->setChecked(true);

	AsymMove->setCheckable(true);
	AsymMove->setChecked(false);

	SymMove->setCheckable(true);
	SymMove->setChecked(false);

	IndependentMove->setCheckable(true);
	IndependentMove->setChecked(false);

	PolySplit->setEnabled(false);
	PolySplit->setCheckable(true);

	BezierClose->setEnabled(false);

	PolyShearL->setAutoRepeat(true);
	PolyShearR->setAutoRepeat(true);
	PolyShearU->setAutoRepeat(true);
	PolyShearD->setAutoRepeat(true);
	RotateCCW->setAutoRepeat(true);
	RotateCW->setAutoRepeat(true);

	RotVal->setMinimum(1);
	RotVal->setMaximum(180);
	RotVal->setValue(1);
	RotVal->setSuffix( QString::fromUtf8(" °"));

	Expand->setAutoRepeat(true);
	Shrink->setAutoRepeat(true);

	scalePercentage->setMinimum(1);
	scalePercentage->setMaximum(100);
	scalePercentage->setValue(10);

	Enlarge->setAutoRepeat(true);
	Reduce->setAutoRepeat(true);

	scaleDistance->setMaximumSize(114, 16777215);
	scaleDistance->setValues(-16777215, 16777215, 2, 30);

	XSpin->setMinimum(0);
	XSpin->setMaximum(16777215);
	XSpin->setNewUnit(2);
	XSpin->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	XSpin->setEnabled(false);

	YSpin->setMinimum(0);
	YSpin->setMaximum(16777215);
	YSpin->setNewUnit(2);
	YSpin->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	YSpin->setEnabled(false);

	TextLabel1->setBuddy(XSpin);
	TextLabel2->setBuddy(YSpin);

	EditCont->setChecked(false);

	PreviewMode->setChecked(false);

	ResetCont->setEnabled(false);

	ResetContClip->setEnabled(true);
	ResetContClip->hide();

	ResetShape2Clip->setEnabled(true);
	ResetShape2Clip->hide();

	resetDefaultButton->setEnabled(true);

	editEditButton->setEnabled(true);
	editEditButton->setDefault(true);

	cancelEditButton->setEnabled(true);

	QButtonGroup *btngpTools = new QButtonGroup();
	btngpTools->setExclusive(true);
	btngpTools->addButton(MoveNode);
	btngpTools->addButton(AddNode);
	btngpTools->addButton(DeleteNode);
	btngpTools->addButton(PolySplit);
	btngpTools->addButton(BezierClose);
	btngpTools->addButton(EditControl);

	resize(QSize(170, 380).expandedTo(minimumSizeHint()));
	layout()->setSizeConstraint(QLayout::SetFixedSize);
	iconSetChange();
	languageChange();

	PosOriginButton->setCurrentIndex(m_posOrigin);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void NodePalette::connectSignals()
{
	// signals and slots connections
	connect(editEditButton, SIGNAL(clicked()), this, SLOT(EndEdit()));
	connect(cancelEditButton, SIGNAL(clicked()), this, SLOT(CancelEdit()));
	connect(resetDefaultButton, SIGNAL(clicked()), this, SLOT(ResetToEditDefaults()));
	connect(DeleteNode, SIGNAL(clicked()), this, SLOT(DelN()));
	connect(AddNode, SIGNAL(clicked()), this, SLOT(AddN()));
	connect(MoveNode, SIGNAL(clicked()), this, SLOT(MoveN()));
	connect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
	connect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
	connect(IndependentMove, SIGNAL(clicked()), this, SLOT(SetIndependentMove()));
	connect(EditControl, SIGNAL(clicked()), this, SLOT(editControl()));
	connect(BezierClose, SIGNAL(clicked()), this, SLOT(CloseBezier()));
	connect(PolySplit, SIGNAL(clicked()), this, SLOT(SplitPoly()));
	connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	connect(PolyMirrorH, SIGNAL(clicked()), this, SLOT(MirrorH()));
	connect(PolyMirrorV, SIGNAL(clicked()), this, SLOT(MirrorV()));
	connect(PolyShearR, SIGNAL(clicked()), this, SLOT(ShearR()));
	connect(PolyShearL, SIGNAL(clicked()), this, SLOT(ShearL()));
	connect(PolyShearU, SIGNAL(clicked()), this, SLOT(ShearU()));
	connect(PolyShearD, SIGNAL(clicked()), this, SLOT(ShearD()));
	connect(RotateCCW, SIGNAL(clicked()), this, SLOT(doRotCCW()));
	connect(RotateCW, SIGNAL(clicked()), this, SLOT(doRotCW()));
	connect(Shrink, SIGNAL(clicked()), this, SLOT(doShrink()));
	connect(Expand, SIGNAL(clicked()), this, SLOT(doExpand()));
	connect(Reduce, SIGNAL(clicked()), this, SLOT(doReduce()));
	connect(Enlarge, SIGNAL(clicked()), this, SLOT(doEnlarge()));
	connect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleContourMode()));
	connect(ResetCont, SIGNAL(clicked()), this, SLOT(ResetContour()));
	connect(ResetContClip, SIGNAL(clicked()), this, SLOT(ResetContourToImageClip()));
	connect(ResetShape2Clip, SIGNAL(clicked()), this, SLOT(ResetShapeToImageClip()));
	connect(PreviewMode, SIGNAL(clicked()), this, SLOT(TogglePreview()));
	connect(PosOriginButton, SIGNAL(currentIndexChanged(int)), this, SLOT(changePosOrigin(int)));
}

void NodePalette::disconnectSignals()
{
	// signals and slots disconnetions
	disconnect(editEditButton, SIGNAL(clicked()), this, SLOT(EndEdit()));
	disconnect(cancelEditButton, SIGNAL(clicked()), this, SLOT(CancelEdit()));
	disconnect(resetDefaultButton, SIGNAL(clicked()), this, SLOT(ResetToEditDefaults()));
	disconnect(DeleteNode, SIGNAL(clicked()), this, SLOT(DelN()));
	disconnect(AddNode, SIGNAL(clicked()), this, SLOT(AddN()));
	disconnect(MoveNode, SIGNAL(clicked()), this, SLOT(MoveN()));
	disconnect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
	disconnect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
	disconnect(IndependentMove, SIGNAL(clicked()), this, SLOT(SetIndependentMove()));
	disconnect(EditControl, SIGNAL(clicked()), this, SLOT(editControl()));
	disconnect(BezierClose, SIGNAL(clicked()), this, SLOT(CloseBezier()));
	disconnect(PolySplit, SIGNAL(clicked()), this, SLOT(SplitPoly()));
	disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	disconnect(PolyMirrorH, SIGNAL(clicked()), this, SLOT(MirrorH()));
	disconnect(PolyMirrorV, SIGNAL(clicked()), this, SLOT(MirrorV()));
	disconnect(PolyShearR, SIGNAL(clicked()), this, SLOT(ShearR()));
	disconnect(PolyShearL, SIGNAL(clicked()), this, SLOT(ShearL()));
	disconnect(PolyShearU, SIGNAL(clicked()), this, SLOT(ShearU()));
	disconnect(PolyShearD, SIGNAL(clicked()), this, SLOT(ShearD()));
	disconnect(RotateCCW, SIGNAL(clicked()), this, SLOT(doRotCCW()));
	disconnect(RotateCW, SIGNAL(clicked()), this, SLOT(doRotCW()));
	disconnect(Shrink, SIGNAL(clicked()), this, SLOT(doShrink()));
	disconnect(Expand, SIGNAL(clicked()), this, SLOT(doExpand()));
	disconnect(Reduce, SIGNAL(clicked()), this, SLOT(doReduce()));
	disconnect(Enlarge, SIGNAL(clicked()), this, SLOT(doEnlarge()));
	disconnect(ResetCont, SIGNAL(clicked()), this, SLOT(ResetContour()));
	disconnect(ResetContClip, SIGNAL(clicked()), this, SLOT(ResetContourToImageClip()));
	disconnect(ResetShape2Clip, SIGNAL(clicked()), this, SLOT(ResetShapeToImageClip()));
	disconnect(PreviewMode, SIGNAL(clicked()), this, SLOT(TogglePreview()));
	disconnect(PosOriginButton, SIGNAL(currentIndexChanged(int)), this, SLOT(changePosOrigin(int)));
}

void NodePalette::initPosOriginMenu()
{
	IconManager& im = IconManager::instance();
	QSignalBlocker sigPosOriginButton(PosOriginButton);

	int index = PosOriginButton->currentIndex();

	PosOriginButton->clear();
	PosOriginButton->addAction(im.loadIcon("position-origin-canvas"), tr("Canvas"))->setData(PositionOrigin::Canvas);
	PosOriginButton->addAction(im.loadIcon("position-origin-page"), tr("Page"))->setData(PositionOrigin::Page);
	PosOriginButton->addAction(im.loadIcon("position-origin-item"), tr("Item"))->setData(PositionOrigin::Item);

	PosOriginButton->setCurrentIndex(index);

}

void NodePalette::setDoc(ScribusDoc *dc, ScribusView *vi)
{
	m_doc = dc;
	m_view = vi;

	unitChange();
	disconnect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleContourMode()));
	disconnect(PosOriginButton, SIGNAL(currentIndexChanged(int)), this, SLOT(changePosOrigin(int)));
	if (m_doc != nullptr)
	{
		YSpin->setNewUnit(m_doc->unitIndex());
		XSpin->setNewUnit(m_doc->unitIndex());
	}
	PosOriginButton->setCurrentIndex(Page);

	EditCont->setChecked(false);
	if (m_doc == nullptr)
		disconnectSignals();
	else
		connectSignals();
}

void NodePalette::IsOpen()
{
	if (m_doc == nullptr)
		return;
	MoveNode->setChecked(true);
	BezierClose->setEnabled(true);
	PolySplit->setEnabled(true);
	EditCont->setEnabled(false);
}

void NodePalette::PolyStatus(int typ, uint size)
{
	if (m_doc == nullptr)
		return;
	bool setter;
	switch (typ)
	{
		case PageItem::Polygon:
			PolySplit->setEnabled(true);
			BezierClose->setEnabled(false);
			break;
		case PageItem::PolyLine:
			setter = size > 7;
			BezierClose->setEnabled(setter);
			PolySplit->setEnabled(setter);
			break;
		default:
			BezierClose->setEnabled(false);
			PolySplit->setEnabled(false);
			break;
	}
}

void NodePalette::doRotCCW()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(0, RotVal->value());
}

void NodePalette::doRotCW()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(1, RotVal->value());
}

void NodePalette::doShrink()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(2, 1, scalePercentage->value());
}

void NodePalette::doExpand()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(3, 1, scalePercentage->value());
}

void NodePalette::doReduce()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(8, 1, scaleDistance->getValue(SC_PT));
}

void NodePalette::doEnlarge()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(9, 1, scaleDistance->getValue(SC_PT));
}

void NodePalette::ShearR()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(4);
}

void NodePalette::ShearL()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(5);
}

void NodePalette::ShearU()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(6);
}

void NodePalette::ShearD()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(7);
}

void NodePalette::MirrorH()
{
	if (m_doc == nullptr)
		return;
	m_doc->MirrorPolyH(m_doc->m_Selection->itemAt(0));
}

void NodePalette::MirrorV()
{
	if (m_doc == nullptr)
		return;
	m_doc->MirrorPolyV(m_doc->m_Selection->itemAt(0));
}

void NodePalette::ResetContour()
{
	if (m_doc == nullptr)
		return;

	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	if (UndoManager::undoEnabled())
	{
		ScItemState<FPointArray> *is = new ScItemState<FPointArray>(Um::ResetContourLine, "",Um::IBorder);
		is->set("RESET_CONTOUR");
		is->setItem(m_doc->m_Selection->itemAt(0)->ContourLine);
		UndoManager::instance()->action(currItem, is);
	}
	//FIXME make an internal item copy poline to contourline member
	currItem->ContourLine = currItem->PoLine.copy();
	currItem->ClipEdited = true;
	m_doc->regionsChanged()->update(QRectF());
	emit DocChanged();
}

void NodePalette::ResetContourToImageClip()
{
	if (m_doc == nullptr)
		return;

	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	currItem->ContourLine = currItem->imageClip.copy();
	currItem->ClipEdited = true;
	m_doc->regionsChanged()->update(QRectF());
	emit DocChanged();
}

void NodePalette::ResetShapeToImageClip()
{
	if (m_doc == nullptr)
		return;

	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	currItem->PoLine = currItem->imageClip.copy();
	currItem->ClipEdited = true;
	currItem->FrameType = 3;
	m_doc->adjustItemSize(currItem);
	if (currItem->itemType() == PageItem::PathText)
		currItem->updatePolyClip();
	m_doc->regionsChanged()->update(QRectF());
	emit DocChanged();
}

void NodePalette::MovePoint()
{
	if (m_doc == nullptr)
		return;
	if (m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT)
	{
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		FPoint zp;
		FPoint np(XSpin->value() / m_doc->unitRatio(), YSpin->value() / m_doc->unitRatio());
		if (m_posOrigin == Canvas || m_posOrigin == Page)
		{
			if (m_posOrigin == Canvas)
				zp = FPoint(currItem->xPos(), currItem->yPos());
			else
				zp = FPoint(currItem->xPos() - m_doc->currentPage()->xOffset(), currItem->yPos() - m_doc->currentPage()->yOffset());
			np -= zp;
		}
		m_doc->nodeEdit.moveClipPoint(currItem, np);
		m_doc->adjustItemSize(currItem, true);
		if (currItem->itemType() == PageItem::PathText)
			currItem->updatePolyClip();
		m_doc->regionsChanged()->update(QRectF());
		emit DocChanged();
	}
}

void NodePalette::SetSym()
{
	AsymMove->setChecked(false);
	IndependentMove->setChecked(false);
	PolySplit->setChecked(false);

	if (m_doc == nullptr)
		return;

	if (SymMove->isChecked())
		m_doc->nodeEdit.setMoveMode(NodeEditContext::Symmetric);
	else
		m_doc->nodeEdit.setMoveMode(NodeEditContext::Auto);
}

void NodePalette::SetAsym()
{
	SymMove->setChecked(false);
	IndependentMove->setChecked(false);
	PolySplit->setChecked(false);

	if (m_doc == nullptr)
		return;

	if (AsymMove->isChecked())
		m_doc->nodeEdit.setMoveMode(NodeEditContext::Asymmetric);
	else
		m_doc->nodeEdit.setMoveMode(NodeEditContext::Auto);
}

void NodePalette::SetIndependentMove()
{
	SymMove->setChecked(false);
	AsymMove->setChecked(false);
	PolySplit->setChecked(false);

	if (m_doc == nullptr)
		return;

	if (IndependentMove->isChecked())
		m_doc->nodeEdit.setMoveMode(NodeEditContext::Independent);
	else
		m_doc->nodeEdit.setMoveMode(NodeEditContext::Auto);
}

void NodePalette::SetXY(double x, double y)
{
	if (m_doc == nullptr)
		return;
	FPoint zp(0.0, 0.0);
	disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));

	if (PosOriginButton->currentIndex() == Page)
		zp = FPoint(m_doc->m_Selection->itemAt(0)->xPos() - m_doc->currentPage()->xOffset(), m_doc->m_Selection->itemAt(0)->yPos() - m_doc->currentPage()->yOffset());
	else if (PosOriginButton->currentIndex() == Canvas)
		zp = FPoint(m_doc->m_Selection->itemAt(0)->xPos(), m_doc->m_Selection->itemAt(0)->yPos());

	XSpin->setValue((x + zp.x()) * m_doc->unitRatio());
	YSpin->setValue((y + zp.y()) * m_doc->unitRatio());
	connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
}

void NodePalette::ToggleContourMode()
{
	if (m_doc == nullptr)
		return;

	disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));

	m_doc->nodeEdit.setIsContourLine(EditCont->isChecked());
	m_doc->nodeEdit.setSubMode(NodeEditContext::MOVE_POINT);
	m_doc->nodeEdit.setClre(-1);
	m_doc->nodeEdit.setClre2(-1);
	m_doc->nodeEdit.setSegP1(-1);
	m_doc->nodeEdit.setSegP2(-1);
	if (EditCont->isChecked())
	{
		BezierClose->setEnabled(false);
		PolySplit->setEnabled(false);
		ResetCont->setEnabled(true);
		ResetContClip->setEnabled(true);
		XSpin->setMinimum(-16777215);
		YSpin->setMinimum(-16777215);
	}
	else
	{
		BezierClose->setEnabled(false);
		PolySplit->setEnabled(true);
		ResetCont->setEnabled(false);
		ResetContClip->setEnabled(false);
		if (m_posOrigin == Canvas | m_posOrigin == Page)
		{
			XSpin->setMinimum(-16777215);
			YSpin->setMinimum(-16777215);
		}
		else
		{
			XSpin->setMinimum(0);
			YSpin->setMinimum(0);
		}
	}
	m_view->DrawNew();

	connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
}

void NodePalette::TogglePreview()
{
	if (m_doc == nullptr)
		return;
	m_doc->nodeEdit.setPreviewMode(PreviewMode->isChecked());
	m_doc->regionsChanged()->update(QRectF());
}

void NodePalette::changePosOrigin(int index)
{
	m_posOrigin = index;

	if (m_doc == nullptr)
		return;

	FPoint zp(0.0, 0.0);
	disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	FPointArray Clip;
	FPoint np(0.0, 0.0);

	if (EditCont->isChecked())
		Clip = m_doc->m_Selection->itemAt(0)->ContourLine;
	else
		Clip = m_doc->m_Selection->itemAt(0)->PoLine;

	if (m_doc->nodeEdit.selectionCount() != 0)
		np = Clip.point(m_doc->nodeEdit.selNode().at(0));

	if (m_posOrigin == Canvas || m_posOrigin == Page)
	{
		XSpin->setMinimum(-16777215);
		YSpin->setMinimum(-16777215);
		if (m_posOrigin == Canvas)
			zp = FPoint(m_doc->m_Selection->itemAt(0)->xPos(), m_doc->m_Selection->itemAt(0)->yPos());
		else
			zp = FPoint(m_doc->m_Selection->itemAt(0)->xPos() - m_doc->currentPage()->xOffset(), m_doc->m_Selection->itemAt(0)->yPos() - m_doc->currentPage()->yOffset());
	}
	else
	{
		XSpin->setMinimum(0);
		YSpin->setMinimum(0);
	}

	XSpin->setValue((np.x() + zp.x()) * m_doc->unitRatio());
	YSpin->setValue((np.y() + zp.y()) * m_doc->unitRatio());
	connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
}

void NodePalette::HaveNode(bool have)
{
	if (m_doc == nullptr)
		return;
	bool setter = have;
	if (m_doc->nodeEdit.selectionCount() > 1)
	{
		XSpin->setEnabled(false);
		YSpin->setEnabled(false);
	}
	else
	{
		XSpin->setEnabled(setter);
		YSpin->setEnabled(setter);
	}

	if (m_doc->nodeEdit.submode() == NodeEditContext::DEL_POINT)
	{
		uint cc;
		bool leaveEd = false;
		PageItem* currItem = m_doc->m_Selection->itemAt(0);
		if (m_doc->nodeEdit.isContourLine())
			cc = currItem->ContourLine.size();
		else
			cc = currItem->PoLine.size();
		if (currItem->isPolyLine())
		{
			if (cc < 5)
				leaveEd = true;
		}
		else
		{
			if (cc < 13)
				leaveEd = true;
		}
		if (leaveEd)
			MoveN();
	}
}

void NodePalette::MoveN()
{
	if (m_doc == nullptr)
		return;

	m_doc->nodeEdit.setSubMode(NodeEditContext::MOVE_POINT);
	m_doc->nodeEdit.setClre(-1);
	m_doc->nodeEdit.setClre2(-1);
	m_doc->nodeEdit.setSegP1(-1);
	m_doc->nodeEdit.setSegP2(-1);

	if (m_doc->m_Selection->count() > 0)
		m_doc->m_Selection->itemAt(0)->update();

	SymMove->setEnabled(true);
	AsymMove->setEnabled(true);
	IndependentMove->setEnabled(true);
	MoveNode->setChecked(true);
}

void NodePalette::AddN()
{
	if (m_doc == nullptr)
		return;

	m_doc->nodeEdit.setSubMode(NodeEditContext::ADD_POINT);
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	IndependentMove->setEnabled(false);
	AddNode->setChecked(true);
}

void NodePalette::DelN()
{
	if (m_doc == nullptr)
		return;

	m_doc->nodeEdit.setSubMode(NodeEditContext::DEL_POINT);
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	IndependentMove->setEnabled(false);
	DeleteNode->setChecked(true);
}

void NodePalette::SplitPoly()
{
	if (m_doc == nullptr)
		return;

	m_doc->nodeEdit.setSubMode(NodeEditContext::SPLIT_PATH);
	m_doc->nodeEdit.setClre(-1);
	m_doc->nodeEdit.setClre2(-1);
	m_doc->nodeEdit.setSegP1(-1);
	m_doc->nodeEdit.setSegP2(-1);

	if (m_doc->m_Selection->count() > 0)
		m_doc->m_Selection->itemAt(0)->update();

	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	IndependentMove->setEnabled(false);
	PolySplit->setChecked(true);
}

void NodePalette::editControl()
{
	if (m_doc == nullptr)
		return;

	m_doc->nodeEdit.setSubMode(NodeEditContext::EDIT_POINT);
	m_doc->nodeEdit.setClre(-1);
	m_doc->nodeEdit.setClre2(-1);
	m_doc->nodeEdit.setSegP1(-1);
	m_doc->nodeEdit.setSegP2(-1);

	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	IndependentMove->setEnabled(false);
	EditControl->setChecked(true);

}

void NodePalette::CloseBezier()
{
	if (m_doc == nullptr)
		return;

	MoveN();
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	PageItem* newItem = m_doc->convertItemTo(currItem, PageItem::Polygon);
	m_doc->m_Selection->clear();
	m_doc->m_Selection->addItem(newItem);
	newItem->update();
	BezierClose->setEnabled(false);
	PolySplit->setEnabled(true);
	EditCont->setEnabled(true);
}

void NodePalette::closeEvent(QCloseEvent *ce)
{
	if (m_doc != nullptr)
	{
		MoveN();
		m_doc->nodeEdit.setClre(-1);
		m_doc->nodeEdit.setClre2(-1);
		m_doc->nodeEdit.setSegP1(-1);
		m_doc->nodeEdit.setSegP2(-1);
		m_doc->nodeEdit.selNode().clear();
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		if (currItem->itemType() == PageItem::PathText)
			currItem->updatePolyClip();
	}
	PolySplit->setEnabled(false);
	BezierClose->setEnabled(false);
	emit paletteClosed();
	ScrPaletteBase::closeEvent(ce);
}

void NodePalette::EndEdit()
{
	if (m_doc != nullptr)
	{
		MoveN();
		m_doc->nodeEdit.setClre(-1);
		m_doc->nodeEdit.setClre2(-1);
		m_doc->nodeEdit.setSegP1(-1);
		m_doc->nodeEdit.setSegP2(-1);
		m_doc->nodeEdit.selNode().clear();
		m_doc->nodeEdit.setPreviewMode(false);
		EditCont->setChecked(false);
		ToggleContourMode();
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		if (currItem && currItem->itemType() == PageItem::PathText)
			currItem->updatePolyClip();
	}
	PolySplit->setEnabled(false);
	BezierClose->setEnabled(false);
	EditCont->setChecked(false);
	emit paletteClosed();
}

void NodePalette::CancelEdit()
{
	if (m_doc != nullptr)
	{
		MoveN();
		m_doc->nodeEdit.setClre(-1);
		m_doc->nodeEdit.setClre2(-1);
		m_doc->nodeEdit.setSegP1(-1);
		m_doc->nodeEdit.setSegP2(-1);
		m_doc->nodeEdit.selNode().clear();
		m_doc->nodeEdit.setPreviewMode(false);
		EditCont->setChecked(false);
		ToggleContourMode();
		PageItem *currItem = m_doc->m_Selection->itemAt(0);

		// Calculate the difference of the current position and the original
		// position in the item's coordinate space (which is rotated and translated,
		// but the translation does not matter for the delta)
		QTransform m = QTransform().rotate(-currItem->rotation());
		QPointF delta = m.map(QPointF(m_xPos, m_yPos)) - m.map(QPointF(currItem->xPos(), currItem->yPos()));
		// During operation the image offsets and possibly other values are changed.
		// To not remember everything we move the clip path to the original position
		// relative to the current position (in the item's coordinate space).
		// adjustItemSize will then take care of moving the position and changing
		// image offsets, etc.
		currItem->PoLine = m_itemPath.copy();
		currItem->PoLine.translate(delta.x(), delta.y());
		currItem->ContourLine = m_itemContourPath.copy();
		m_doc->adjustItemSize(currItem);
		if (currItem->itemType() == PageItem::PathText)
			currItem->updatePolyClip();
		m_doc->regionsChanged()->update(QRectF());
	}
	PolySplit->setEnabled(false);
	BezierClose->setEnabled(false);
	EditCont->setChecked(false);
	emit paletteClosed();
}

void NodePalette::ResetToEditDefaults()
{
	if (m_doc == nullptr)
		return;

	m_doc->nodeEdit.setClre(-1);
	m_doc->nodeEdit.setClre2(-1);
	m_doc->nodeEdit.setSegP1(-1);
	m_doc->nodeEdit.setSegP2(-1);
	m_doc->nodeEdit.selNode().clear();
	m_doc->nodeEdit.setPreviewMode(false);
	PageItem *currItem = m_doc->m_Selection->itemAt(0);

	// See comment in NodePalette::CancelEdit
	QTransform m = QTransform().rotate(-currItem->rotation());
	QPointF delta = m.map(QPointF(m_xPos, m_yPos)) - m.map(QPointF(currItem->xPos(), currItem->yPos()));
	if (EditCont->isChecked())
	{
		currItem->ContourLine = m_itemContourPath.copy();
		currItem->ContourLine.translate(delta.x(), delta.y());
	}
	else
	{
		// See comment in NodePalette::CancelEdit
		currItem->PoLine = m_itemPath;
 		currItem->PoLine.translate(delta.x(), delta.y());
 		currItem->ContourLine = m_itemContourPath;
		m_doc->adjustItemSize(currItem);
	}
	if (currItem->itemType() == PageItem::PathText)
		currItem->updatePolyClip();
	currItem->update();
	m_doc->regionsChanged()->update(QRectF());
}

void NodePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void NodePalette::reject()
{
	EndEdit();
	ScrPaletteBase::reject();
}

void NodePalette::setDefaults(PageItem* currItem)
{
	m_xPos = currItem->xPos();
	m_yPos = currItem->yPos();
	m_itemPath = currItem->PoLine.copy();
	m_itemContourPath = currItem->ContourLine.copy();
}

void NodePalette::iconSetChange()
{
	IconManager& im = IconManager::instance();

//<<<<<<< .mine
	initPosOriginMenu();

	MoveNode->setIcon(im.loadIcon("bezier-node-move"));
	AddNode->setIcon(im.loadIcon("bezier-node-add"));
	DeleteNode->setIcon(im.loadIcon("bezier-node-delete"));
	AsymMove->setIcon(im.loadIcon("bezier-control-asymetric"));
	SymMove->setIcon(im.loadIcon("bezier-control-symetric"));
	IndependentMove->setIcon(im.loadIcon("bezier-control-independent"));
	EditControl->setIcon(im.loadIcon("bezier-node-edit"));
	PolySplit->setIcon(im.loadIcon("bezier-split"));
	BezierClose->setIcon(im.loadIcon("bezier-close"));
	PolyMirrorH->setIcon(im.loadIcon("object-flip-horizontal"));
	PolyMirrorV->setIcon(im.loadIcon("object-flip-vertical"));
	PolyShearL->setIcon(im.loadIcon("transform-shear-left"));
	PolyShearR->setIcon(im.loadIcon("transform-shear-right"));
	PolyShearU->setIcon(im.loadIcon("transform-shear-up"));
	PolyShearD->setIcon(im.loadIcon("transform-shear-down"));
	RotateCCW->setIcon(im.loadIcon("rotate-ccw"));
	RotateCW->setIcon(im.loadIcon("rotate-cw"));
	Expand->setIcon(im.loadIcon("bezier-scale-up"));
	Shrink->setIcon(im.loadIcon("bezier-scale-down"));
	Enlarge->setIcon(im.loadIcon("bezier-scale-up"));
	Reduce->setIcon(im.loadIcon("bezier-scale-down"));

	EditCont->setIcon(im.loadIcon("path-contour-edit"));
	PreviewMode->setIcon(im.loadIcon("path-node-visibility"));
	ResetContClip->setIcon(im.loadIcon("path-contour-to-clip"));
	ResetShape2Clip->setIcon(im.loadIcon("path-shape-to-clip"));
	ResetCont->setIcon(im.loadIcon("path-contour-reset"));
	resetDefaultButton->setIcon(im.loadIcon("path-reset-all"));
}

void NodePalette::languageChange()
{
	setWindowTitle( tr( "Nodes" ) );
	scalePercentage->setSuffix( tr(" %"));

	initPosOriginMenu();

	PosOriginLabel->setText( tr("Origin:"));
	TextLabel1->setText( tr("&X:"));
	TextLabel2->setText( tr("&Y:"));
	editEditButton->setText( tr("OK"));
	cancelEditButton->setText( tr("Cancel"));
	resetDefaultButton->setToolTip( tr("Reset all edits"));
	MoveNode->setToolTip( tr("Move Nodes"));
	AddNode->setToolTip( tr("Add Nodes"));
	DeleteNode->setToolTip( tr("Delete Nodes"));
	AsymMove->setToolTip( tr("Move Control Points Asymetrical"));
	SymMove->setToolTip( tr("Move Control Points Symmetrical"));
	IndependentMove->setToolTip( tr("Move Control Points Independently"));
	EditControl->setToolTip( tr("Edit Control Points"));
	PolySplit->setToolTip( tr("Open a Polygon or Cuts a Bezier Curve"));
	BezierClose->setToolTip( tr("Close this Bezier Curve"));
	PolyMirrorH->setToolTip( tr("Mirror the Path Horizontally"));
	PolyMirrorV->setToolTip( tr("Mirror the Path Vertically"));
	PolyShearR->setToolTip( tr("Shear the Path Horizontally to the Right"));
	PolyShearL->setToolTip( tr("Shear the Path Horizontally to the Left"));
	PolyShearU->setToolTip( tr("Shear the Path Vertically Up"));
	PolyShearD->setToolTip( tr("Shear the Path Vertically Down"));
	RotateCCW->setToolTip( tr("Rotate the Path Counter-Clockwise"));
	RotateCW->setToolTip( tr("Rotate the Path Clockwise"));
	Shrink->setToolTip( tr("Shrink the Size of the Path by shown %"));
	Expand->setToolTip( tr("Enlarge the Size of the Path by shown %"));
	Reduce->setToolTip( tr("Reduce the Size of the Path by the shown value"));
	Enlarge->setToolTip( tr("Enlarge the Size of the Path by the shown value"));
	RotVal->setToolTip( tr("Angle of Rotation"));
	scalePercentage->setToolTip( tr("% to Enlarge or Shrink By"));
	scaleDistance->setToolTip( tr("Value to Enlarge or Shrink By"));
	PreviewMode->setToolTip( tr("Hide Nodes"));
	EditCont->setToolTip( tr("Activate Contour Line Editing Mode"));
	ResetCont->setToolTip( tr("Reset the Contour Line to the Original Shape of the Frame"));
	ResetContClip->setToolTip( tr("Reset the Contour Line to the Clipping Path of the Image"));
	ResetShape2Clip->setToolTip( tr("Set the Shape to the Clipping Path of the Image"));
	PosOriginButton->setToolTip(  "<qt>" + tr("Use coordinates relative to the canvas, page, or to the Object") + "</qt>");
}

void NodePalette::unitChange()
{
	if (m_doc == nullptr)
		return;
	m_unitRatio = m_doc->unitRatio();

	bool sigBlocked1 = XSpin->blockSignals(true);
	bool sigBlocked2 = YSpin->blockSignals(true);
	bool sigBlocked3 = scaleDistance->blockSignals(true);

	XSpin->setNewUnit(m_doc->unitIndex());
	YSpin->setNewUnit(m_doc->unitIndex());
	scaleDistance->setNewUnit(m_doc->unitIndex());

	XSpin->blockSignals(sigBlocked1);
	YSpin->blockSignals(sigBlocked2);
	scaleDistance->blockSignals(sigBlocked3);
}

ScribusDoc* NodePalette::currentDocument() const
{
	return m_doc;
}
