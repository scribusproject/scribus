/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>

#include "frameedit.h"
#include "page.h"
#include "pageitem.h"
#include "scribusview.h"
#include "selection.h"
#include "units.h"
#include "undomanager.h"
#include "undostate.h"
#include "util_icon.h"
#include "lensdialog.h"

NodePalette::NodePalette( QWidget* parent) : ScrPaletteBase( parent, "nodePalette", false, 0)
{
	QSize iconSize = QSize(22, 22);
	doc=0;
	unitRatio=1.0;
	vboxLayout = new QVBoxLayout(this);
	vboxLayout->setSpacing(2);
	vboxLayout->setMargin(5);
	gridLayout = new QGridLayout();
	gridLayout->setSpacing(2);
	gridLayout->setMargin(0);
	MoveNode = new QToolButton(this);
	MoveNode->setText( "" );
	MoveNode->setIcon(QIcon(loadIcon("MoveNode.png")));
	MoveNode->setIconSize(iconSize);
	MoveNode->setContentsMargins(0, 0, 0, 0);
	MoveNode->setCheckable(true);
	MoveNode->setChecked(true);
	MoveNode->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(MoveNode, 0, 0, 1, 1);

	AddNode = new QToolButton(this);
	AddNode->setIcon(QIcon(loadIcon("AddNode.png")));
	AddNode->setIconSize(iconSize);
	AddNode->setContentsMargins(0, 0, 0, 0);
	AddNode->setEnabled(true);
	AddNode->setCheckable(true);
	AddNode->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(AddNode, 0, 1, 1, 1);

	DeleteNode = new QToolButton(this);
	DeleteNode->setIcon(QIcon(loadIcon("DeleteNode.png")));
	DeleteNode->setIconSize(iconSize);
	DeleteNode->setContentsMargins(0, 0, 0, 0);
	DeleteNode->setEnabled(true);
	DeleteNode->setCheckable(true);
	DeleteNode->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(DeleteNode, 0, 2, 1, 1);

	ResNode = new QToolButton(this);
	ResNode->setIcon(QIcon(loadIcon("ResetNode.png")));
	ResNode->setIconSize(iconSize);
	ResNode->setContentsMargins(0, 0, 0, 0);
	ResNode->setEnabled(true);
	ResNode->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(ResNode, 0, 3, 1, 1);

	MoveControl = new QToolButton(this);
	MoveControl->setIcon(QIcon(loadIcon("MoveKontrol.png")));
	MoveControl->setIconSize(iconSize);
	MoveControl->setContentsMargins(0, 0, 0, 0);
	MoveControl->setCheckable(true);
	MoveControl->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(MoveControl, 1, 0, 1, 1);

	AsymMove = new QToolButton(this);
	AsymMove->setIcon(QIcon(loadIcon("MoveAsym.png")));
	AsymMove->setIconSize(iconSize);
	AsymMove->setContentsMargins(0, 0, 0, 0);
	AsymMove->setEnabled(false);
	AsymMove->setCheckable(true);
	AsymMove->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(AsymMove, 1, 1, 1, 1);

	SymMove = new QToolButton(this);
	SymMove->setIcon(QIcon(loadIcon("MoveSym.png")));
	SymMove->setIconSize(iconSize);
	SymMove->setContentsMargins(0, 0, 0, 0);
	SymMove->setEnabled(false);
	SymMove->setCheckable(true);
	SymMove->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(SymMove, 1, 2, 1, 1);

	Res1Node = new QToolButton(this);
	Res1Node->setIcon(QIcon(loadIcon("Reset1Node.png")));
	Res1Node->setIconSize(iconSize);
	Res1Node->setContentsMargins(0, 0, 0, 0);
	Res1Node->setEnabled(false);
	Res1Node->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(Res1Node, 1, 3, 1, 1);

	PolySplit = new QToolButton(this);
	PolySplit->setEnabled( false );
	PolySplit->setCheckable( true );
	PolySplit->setIcon(QIcon(loadIcon("PolyCut.png")));
	PolySplit->setIconSize(iconSize);
	PolySplit->setContentsMargins(0, 0, 0, 0);
	PolySplit->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(PolySplit, 2, 0, 1, 1);

	BezierClose = new QToolButton(this);
	BezierClose->setEnabled( false );
	BezierClose->setIcon(QIcon(loadIcon("BezierClose.png")));
	BezierClose->setIconSize(iconSize);
	BezierClose->setContentsMargins(0, 0, 0, 0);
	BezierClose->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(BezierClose, 2, 1, 1, 1);

	PolyMirrorH = new QToolButton(this);
	PolyMirrorH->setIcon(QIcon(loadIcon("hmirror.png")));
	PolyMirrorH->setIconSize(iconSize);
	PolyMirrorH->setContentsMargins(0, 0, 0, 0);
	PolyMirrorH->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(PolyMirrorH, 2, 2, 1, 1);

	PolyMirrorV = new QToolButton(this);
	PolyMirrorV->setIcon(QIcon(loadIcon("vmirror.png")));
	PolyMirrorV->setIconSize(iconSize);
	PolyMirrorV->setContentsMargins(0, 0, 0, 0);
	PolyMirrorV->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(PolyMirrorV, 2, 3, 1, 1);

	PolyShearL = new QToolButton(this);
	PolyShearL->setIcon(QIcon(loadIcon("22/transform-shear-left.png")));
	PolyShearL->setIconSize(iconSize);
	PolyShearL->setContentsMargins(0, 0, 0, 0);
	PolyShearL->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	PolyShearL->setAutoRepeat(true);
	gridLayout->addWidget(PolyShearL, 3, 0, 1, 1);

	PolyShearR = new QToolButton(this);
	PolyShearR->setIcon(QIcon(loadIcon("22/transform-shear-right.png")));
	PolyShearR->setIconSize(iconSize);
	PolyShearR->setContentsMargins(0, 0, 0, 0);
	PolyShearR->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	PolyShearR->setAutoRepeat(true);
	gridLayout->addWidget(PolyShearR, 3, 1, 1, 1);

	PolyShearU = new QToolButton(this);
	PolyShearU->setIcon(QIcon(loadIcon("22/transform-shear-up.png")));
	PolyShearU->setIconSize(iconSize);
	PolyShearU->setContentsMargins(0, 0, 0, 0);
	PolyShearU->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	PolyShearU->setAutoRepeat(true);
	gridLayout->addWidget(PolyShearU, 3, 2, 1, 1);

	PolyShearD = new QToolButton(this);
	PolyShearD->setIcon(QIcon(loadIcon("22/transform-shear-down.png")));
	PolyShearD->setIconSize(iconSize);
	PolyShearD->setContentsMargins(0, 0, 0, 0);
	PolyShearD->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	PolyShearD->setAutoRepeat(true);
	gridLayout->addWidget(PolyShearD, 3, 3, 1, 1);
	vboxLayout->addLayout(gridLayout);

	gridLayout1 = new QGridLayout();
	gridLayout1->setSpacing(5);
	gridLayout1->setMargin(0);

	RotateCCW = new QToolButton(this);
	RotateCCW->setIcon(QIcon(loadIcon("rotate_ccw.png")));
	RotateCCW->setIconSize(iconSize);
	RotateCCW->setContentsMargins(0, 0, 0, 0);
	RotateCCW->setAutoRepeat(true);
	gridLayout1->addWidget(RotateCCW, 0, 0, 1, 1);

	RotateCW = new QToolButton(this);
	RotateCW->setIcon(QIcon(loadIcon("rotate_cw.png")));
	RotateCW->setIconSize(iconSize);
	RotateCW->setContentsMargins(0, 0, 0, 0);
	RotateCW->setAutoRepeat(true);
	gridLayout1->addWidget(RotateCW, 0, 1, 1, 1);

	RotVal = new QSpinBox(this);
	RotVal->setMinimum(1);
	RotVal->setMaximum(180);
	RotVal->setValue(1);
	RotVal->setSuffix( QString::fromUtf8(" °"));
	gridLayout1->addWidget(RotVal, 0, 2, 1, 1);

	Expand = new QToolButton(this);
	Expand->setIcon(QIcon(loadIcon("expand.png")));
	Expand->setIconSize(iconSize);
	Expand->setContentsMargins(0, 0, 0, 0);
	Expand->setAutoRepeat(true);
	gridLayout1->addWidget(Expand, 1, 0, 1, 1);

	Shrink = new QToolButton(this);
	Shrink->setIcon(QIcon(loadIcon("crop.png")));
	Shrink->setIconSize(iconSize);
	Shrink->setContentsMargins(0, 0, 0, 0);
	Shrink->setAutoRepeat(true);
	gridLayout1->addWidget(Shrink, 1, 1, 1, 1);

	scalePercentage = new QSpinBox(this);
	scalePercentage->setMinimum(1);
	scalePercentage->setMaximum(100);
	scalePercentage->setValue(10);
	gridLayout1->addWidget(scalePercentage, 1, 2, 1, 1);

	Enlarge = new QToolButton(this);
	Enlarge->setIcon(QIcon(loadIcon("expand.png")));
	Enlarge->setIconSize(iconSize);
	Enlarge->setContentsMargins(0, 0, 0, 0);
	Enlarge->setAutoRepeat(true);
	gridLayout1->addWidget(Enlarge, 2, 0, 1, 1);

	Reduce = new QToolButton(this);
	Reduce->setIcon(QIcon(loadIcon("crop.png")));
	Reduce->setIconSize(iconSize);
	Reduce->setContentsMargins(0, 0, 0, 0);
	Reduce->setAutoRepeat(true);
	gridLayout1->addWidget(Reduce, 2, 1, 1, 1);

	scaleDistance = new ScrSpinBox( 1, 30000, this, 2);
	scaleDistance->setValue(10);
	scaleDistance->setSuffix("");
	gridLayout1->addWidget(scaleDistance, 2, 2, 1, 1);
	vboxLayout->addLayout(gridLayout1);

	gridLayout2 = new QGridLayout();
	gridLayout2->setSpacing(5);
	gridLayout2->setMargin(0);

	lensEffect = new QPushButton(this);
	gridLayout2->addWidget(lensEffect, 0, 0, 1, 2);

	AbsMode = new QCheckBox( "&Absolute Coordinates", this );
	AbsMode->setChecked(false);
	gridLayout2->addWidget(AbsMode, 1, 0, 1, 2);

	TextLabel1 = new QLabel(this);
	gridLayout2->addWidget(TextLabel1, 2, 0, 1, 1);

	XSpin = new ScrSpinBox( 0, 30000, this, 2 );
	XSpin->setEnabled(false);
	gridLayout2->addWidget(XSpin, 2, 1, 1, 1);

	TextLabel2 = new QLabel(this);
	gridLayout2->addWidget(TextLabel2, 3, 0, 1, 1);

	YSpin = new ScrSpinBox( 0, 30000, this, 2 );
	YSpin->setEnabled(false);
	gridLayout2->addWidget(YSpin, 3, 1, 1, 1);
	TextLabel1->setBuddy(XSpin);
	TextLabel2->setBuddy(YSpin);

	EditCont = new QCheckBox(this);
	EditCont->setChecked(false);
	gridLayout2->addWidget(EditCont, 4, 0, 1, 2);

	ResetCont = new QPushButton(this);
	ResetCont->setEnabled(false);
	gridLayout2->addWidget(ResetCont, 5, 0, 1, 2);

	ResetContClip = new QPushButton(this);
	ResetContClip->setEnabled(true);
	ResetContClip->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(6), static_cast<QSizePolicy::Policy>(6)));
	ResetContClip->hide();
	gridLayout2->addWidget(ResetContClip, 6, 0, 1, 2);

	editEditButton = new QPushButton(this);
	editEditButton->setDefault(true);
	gridLayout2->addWidget(editEditButton, 7, 0, 1, 2);

	vboxLayout->addLayout(gridLayout2);
	resize(QSize(170, 380).expandedTo(minimumSizeHint()));

	languageChange();
}

void NodePalette::connectSignals()
{
	// signals and slots connections
	connect(editEditButton, SIGNAL(clicked()), this, SLOT(EndEdit()));
	connect(DeleteNode, SIGNAL(clicked()), this, SLOT(DelN()));
	connect(AddNode, SIGNAL(clicked()), this, SLOT(AddN()));
	connect(MoveNode, SIGNAL(clicked()), this, SLOT(MoveN()));
	connect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
	connect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
	connect(ResNode, SIGNAL(clicked()), this, SLOT(ResetControl()));
	connect(Res1Node, SIGNAL(clicked()), this, SLOT(Reset1Control()));
	connect(BezierClose, SIGNAL(clicked()), this, SLOT(CloseBezier()));
	connect(PolySplit, SIGNAL(clicked()), this, SLOT(SplitPoly()));
	connect(MoveControl, SIGNAL(clicked()), this, SLOT(MoveK()));
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
	connect(lensEffect, SIGNAL(clicked()), this, SLOT(doLensEffect()));
	connect(AbsMode, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	connect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleConMode()));
	connect(ResetCont, SIGNAL(clicked()), this, SLOT(ResetContour()));
	connect(ResetContClip, SIGNAL(clicked()), this, SLOT(ResetContourToImageClip()));
}

void NodePalette::disconnectSignals()
{
	// signals and slots disconnetions
	disconnect(editEditButton, SIGNAL(clicked()), this, SLOT(EndEdit()));
	disconnect(DeleteNode, SIGNAL(clicked()), this, SLOT(DelN()));
	disconnect(AddNode, SIGNAL(clicked()), this, SLOT(AddN()));
	disconnect(MoveNode, SIGNAL(clicked()), this, SLOT(MoveN()));
	disconnect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
	disconnect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
	disconnect(ResNode, SIGNAL(clicked()), this, SLOT(ResetControl()));
	disconnect(Res1Node, SIGNAL(clicked()), this, SLOT(Reset1Control()));
	disconnect(BezierClose, SIGNAL(clicked()), this, SLOT(CloseBezier()));
	disconnect(PolySplit, SIGNAL(clicked()), this, SLOT(SplitPoly()));
	disconnect(MoveControl, SIGNAL(clicked()), this, SLOT(MoveK()));
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
	disconnect(lensEffect, SIGNAL(clicked()), this, SLOT(doLensEffect()));
	//	disconnect(AbsMode, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	//	disconnect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleConMode()));
	disconnect(ResetCont, SIGNAL(clicked()), this, SLOT(ResetContour()));
	disconnect(ResetContClip, SIGNAL(clicked()), this, SLOT(ResetContourToImageClip()));
}

void NodePalette::setDoc(ScribusDoc *dc, ScribusView *vi)
{
	doc = dc;
	view = vi;

	unitChange();
	disconnect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleConMode()));
	disconnect(AbsMode, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	if (doc!=0)
	{
		YSpin->setNewUnit(doc->unitIndex());
		XSpin->setNewUnit(doc->unitIndex());
	}
	AbsMode->setChecked(false);
	EditCont->setChecked(false);
	if (doc==0)
		disconnectSignals();
	else
		connectSignals();
}

void NodePalette::SplitPoly()
{
	if (doc==0)
		return;
	MoveN();
	doc->nodeEdit.submode = NodeEditContext::SPLIT_PATH;
	PolySplit->setChecked(true);
}

void NodePalette::IsOpen()
{
	if (doc==0)
		return;
	PolySplit->setChecked(false);
	BezierClose->setEnabled(true);
	PolySplit->setEnabled(true);
	EditCont->setEnabled(false);
}

void NodePalette::PolyStatus(int typ, uint size)
{
	if (doc==0)
		return;
	bool setter;
	switch (typ)
	{
	case 6:
		PolySplit->setEnabled(true);
		BezierClose->setEnabled(false);
		break;
	case 7:
		setter = size > 7 ? true : false;
		BezierClose->setEnabled(setter);
		PolySplit->setEnabled(setter);
		break;
	default:
		BezierClose->setEnabled(false);
		PolySplit->setEnabled(false);
		break;
	}
}

void NodePalette::CloseBezier()
{
	if (doc==0)
		return;
	MoveN();
	view->Bezier2Poly();
	BezierClose->setEnabled(false);
	PolySplit->setEnabled(true);
	EditCont->setEnabled(true);
}

void NodePalette::doLensEffect()
{
	if (doc==0)
		return;
	FPointArray points;
	PageItem *currItem = doc->m_Selection->itemAt(0);
	if (doc->nodeEdit.isContourLine)
		points = currItem->ContourLine;
	else
		points = currItem->PoLine;
	LensDialog *dia = new LensDialog(0, points);
	if (dia->exec())
	{
		points.fromQPainterPath(dia->modifiedPath);
		if (doc->nodeEdit.isContourLine)
			currItem->ContourLine = points;
		else
			currItem->PoLine = points;
		doc->AdjustItemSize(doc->m_Selection->itemAt(0));
	}
	delete dia;
}

void NodePalette::doRotCCW()
{
	if (doc != 0)
		view->TransformPoly(0, RotVal->value());
}

void NodePalette::doRotCW()
{
	if (doc != 0)
		view->TransformPoly(1, RotVal->value());
}

void NodePalette::doShrink()
{
	if (doc != 0)
		view->TransformPoly(2, 1, scalePercentage->value());
}

void NodePalette::doExpand()
{
	if (doc != 0)
		view->TransformPoly(3, 1, scalePercentage->value());
}

void NodePalette::doReduce()
{
	if (doc != 0)
		view->TransformPoly(8, 1, scaleDistance->value()/unitGetRatioFromIndex(doc->unitIndex()));
}

void NodePalette::doEnlarge()
{
	if (doc != 0)
		view->TransformPoly(9, 1, scaleDistance->value()/unitGetRatioFromIndex(doc->unitIndex()));
}

void NodePalette::ShearR()
{
	if (doc != 0)
		view->TransformPoly(5);
}

void NodePalette::ShearL()
{
	if (doc != 0)
		view->TransformPoly(4);
}

void NodePalette::ShearU()
{
	if (doc != 0)
		view->TransformPoly(6);
}

void NodePalette::ShearD()
{
	if (doc != 0)
		view->TransformPoly(7);
}

void NodePalette::MirrorH()
{
	if (doc != 0)
		doc->MirrorPolyH(doc->m_Selection->itemAt(0));
}

void NodePalette::MirrorV()
{
	if (doc != 0)
		doc->MirrorPolyV(doc->m_Selection->itemAt(0));
}

void NodePalette::ResetControl()
{
	if (doc != 0)
		doc->nodeEdit.resetControl(doc->m_Selection->itemAt(0));
}

void NodePalette::Reset1Control()
{
	if (doc != 0)
		doc->nodeEdit.reset1Control(doc->m_Selection->itemAt(0));
}

void NodePalette::ResetContour()
{
	if (doc != 0)
	{
		if (UndoManager::undoEnabled())
		{
			ItemState<FPointArray> *is = new ItemState<FPointArray>(Um::ResetContourLine, "",Um::IBorder);
			is->set("RESET_CONTOUR", "reset_contour");
			//is->setItem(view->SelItem.at(0)->ContourLine);
			is->setItem(doc->m_Selection->itemAt(0)->ContourLine);
			//UndoManager::instance()->action(view->SelItem.at(0), is);
			UndoManager::instance()->action(doc->m_Selection->itemAt(0), is);
		}
		//view->SelItem.at(0)->ContourLine = view->SelItem.at(0)->PoLine.copy();
		//FIXME make an internal item copy poline to contourline member
		doc->m_Selection->itemAt(0)->ContourLine = doc->m_Selection->itemAt(0)->PoLine.copy();
		//view->SelItem.at(0)->ClipEdited = true;
		doc->m_Selection->itemAt(0)->ClipEdited = true;
		doc->m_Selection->itemAt(0)->update();
	}
}

void NodePalette::ResetContourToImageClip()
{
	if (doc != 0)
	{
		/*		if (UndoManager::undoEnabled())
				{
					ItemState<FPointArray> *is = new ItemState<FPointArray>(Um::ResetContourLine, "",Um::IBorder);
					is->set("RESET_CONTOUR", "reset_contour");
					is->setItem(doc->m_Selection->itemAt(0)->ContourLine);
					UndoManager::instance()->action(doc->m_Selection->itemAt(0), is);
				} */
		doc->m_Selection->itemAt(0)->ContourLine = doc->m_Selection->itemAt(0)->imageClip.copy();
		doc->m_Selection->itemAt(0)->ClipEdited = true;
		doc->m_Selection->itemAt(0)->update();
	}
}

void NodePalette::MovePoint()
{
	if (doc==0)
		return;
	if (doc->nodeEdit.submode == NodeEditContext::MOVE_POINT)
	{
		FPoint np(XSpin->value()/doc->unitRatio(), YSpin->value()/doc->unitRatio());
		FPoint zp(doc->m_Selection->itemAt(0)->xPos(), doc->m_Selection->itemAt(0)->yPos());
		if (AbsMode->isChecked())
			np -= zp;
		doc->nodeEdit.moveClipPoint(doc->m_Selection->itemAt(0), np);
		doc->AdjustItemSize(doc->m_Selection->itemAt(0));
		emit DocChanged();
	}
}

void NodePalette::SetSym()
{
	if (doc != 0)
		doc->nodeEdit.MoveSym = true;
	SymMove->setChecked(true);
	AsymMove->setChecked(false);
}

void NodePalette::SetAsym()
{
	if (doc != 0)
		doc->nodeEdit.MoveSym = false;
	SymMove->setChecked(false);
	AsymMove->setChecked(true);
}

void NodePalette::SetXY(double x, double y)
{
	if (doc==0)
		return;
	FPoint zp(0.0, 0.0);
	disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	if (AbsMode->isChecked())
		zp = FPoint(doc->m_Selection->itemAt(0)->xPos() - doc->currentPage()->xOffset(), doc->m_Selection->itemAt(0)->yPos() - doc->currentPage()->yOffset());
	XSpin->setValue((x + zp.x())*doc->unitRatio());
	YSpin->setValue((y + zp.y())*doc->unitRatio());
	connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
}

void NodePalette::ToggleAbsMode()
{
	if (doc==0)
		return;
	FPoint zp(doc->m_Selection->itemAt(0)->xPos() - doc->currentPage()->xOffset(), doc->m_Selection->itemAt(0)->yPos() - doc->currentPage()->yOffset());
	disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	double unitRatio=doc->unitRatio();
	FPoint np(XSpin->value()/unitRatio, YSpin->value()/unitRatio);
	if (AbsMode->isChecked())
		np += zp;
	else
		np -= zp;
	XSpin->setValue(np.x()*unitRatio);
	YSpin->setValue(np.y()*unitRatio);
	connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
}

void NodePalette::ToggleConMode()
{
	disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	if (doc != 0)
	{
		doc->nodeEdit.isContourLine = EditCont->isChecked();
		doc->m_Selection->itemAt(0)->update();
		if (EditCont->isChecked())
		{
			BezierClose->setEnabled(false);
			PolySplit->setEnabled(false);
			ResetCont->setEnabled(true);
			ResetContClip->setEnabled(true);
			XSpin->setMinimum(-3000);
			YSpin->setMinimum(-3000);
		}
		else
		{
			BezierClose->setEnabled(false);
			PolySplit->setEnabled(true);
			ResetCont->setEnabled(false);
			ResetContClip->setEnabled(false);
			XSpin->setMinimum(0);
			YSpin->setMinimum(0);
		}
	}
	connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
}

void NodePalette::HaveNode(bool have, bool mov)
{
	if (doc==0)
		return;
	bool setter = have ? true : false;
	XSpin->setEnabled(setter);
	YSpin->setEnabled(setter);
	if (setter == true)
	{
		if (doc->nodeEdit.EdPoints)
			ResNode->setEnabled(setter);
		else
			Res1Node->setEnabled(setter);
	}
	else
	{
		ResNode->setEnabled(setter);
		Res1Node->setEnabled(setter);
	}
	if (doc->nodeEdit.submode == NodeEditContext::MOVE_POINT)
	{
		disconnect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
		disconnect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
		if (mov)
		{
			SymMove->setChecked(true);
			AsymMove->setChecked(false);
		}
		else
		{
			AsymMove->setChecked(true);
			SymMove->setChecked(false);
		}
		connect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
		connect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
	}
	if (doc->nodeEdit.submode == NodeEditContext::DEL_POINT)
	{
		uint cc;
		bool leaveEd = false;
		PageItem*currItem=doc->m_Selection->itemAt(0);
		if (doc->nodeEdit.isContourLine)
			cc = currItem->ContourLine.size();
		else
			cc = currItem->PoLine.size();
		if (currItem->asPolyLine())
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
		{
			MoveN();
			MoveControl->setEnabled(true);
		}
	}
}

void NodePalette::MoveK()
{
	if (doc==0)
		return;
	doc->nodeEdit.submode = NodeEditContext::MOVE_POINT;
	doc->nodeEdit.EdPoints = false;
	if (doc->m_Selection->count() > 0)
		doc->m_Selection->itemAt(0)->update();
	SymMove->setEnabled(true);
	AsymMove->setEnabled(true);
	Res1Node->setEnabled(true);
	ResNode->setEnabled(false);
	AddNode->setEnabled(false);
	DeleteNode->setEnabled(false);
	MoveNode->setChecked(false);
	MoveControl->setChecked(true);
	DeleteNode->setChecked(false);
	AddNode->setChecked(false);
}

void NodePalette::MoveN()
{
	if (doc==0)
		return;
	doc->nodeEdit.submode = NodeEditContext::MOVE_POINT;
	doc->nodeEdit.EdPoints = true;
	doc->nodeEdit.ClRe = -1;
	doc->nodeEdit.ClRe2 = -1;
	doc->nodeEdit.SegP1 = -1;
	doc->nodeEdit.SegP2 = -1;
	if (doc->m_Selection->count() > 0)
		doc->m_Selection->itemAt(0)->update();
	ResNode->setEnabled(true);
	AddNode->setEnabled(true);
	DeleteNode->setEnabled(true);
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	Res1Node->setEnabled(false);
	MoveNode->setChecked(true);
	MoveControl->setChecked(false);
	DeleteNode->setChecked(false);
	AddNode->setChecked(false);
}

void NodePalette::AddN()
{
	if (doc==0)
		return;
	doc->nodeEdit.submode = NodeEditContext::ADD_POINT;
	doc->nodeEdit.EdPoints = true;
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	ResNode->setEnabled(false);
	Res1Node->setEnabled(false);
	AddNode->setChecked(true);
	DeleteNode->setChecked(false);
}

void NodePalette::DelN()
{
	if (doc==0)
		return;
	doc->nodeEdit.submode = NodeEditContext::DEL_POINT;
	doc->nodeEdit.EdPoints = true;
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	ResNode->setEnabled(false);
	Res1Node->setEnabled(false);
	AddNode->setChecked(false);
	DeleteNode->setChecked(true);
}

void NodePalette::closeEvent(QCloseEvent *ce)
{
	if (doc != 0)
	{
		MoveN();
		doc->nodeEdit.ClRe = -1;
		doc->nodeEdit.ClRe2 = -1;
		doc->nodeEdit.SegP1 = -1;
		doc->nodeEdit.SegP2 = -1;
		doc->nodeEdit.SelNode.clear();
	}
	PolySplit->setEnabled( false );
	BezierClose->setEnabled( false );
	emit Schliessen();
	ScrPaletteBase::closeEvent(ce);
}

void NodePalette::EndEdit()
{
	if (doc != 0)
	{
		MoveN();
		doc->nodeEdit.ClRe = -1;
		doc->nodeEdit.ClRe2 = -1;
		doc->nodeEdit.SegP1 = -1;
		doc->nodeEdit.SegP2 = -1;
		doc->nodeEdit.SelNode.clear();
		EditCont->setChecked(false);
		ToggleConMode();
	}
	PolySplit->setEnabled( false );
	BezierClose->setEnabled( false );
	EditCont->setChecked(false);
	emit Schliessen();
}

void NodePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
}

void NodePalette::languageChange()
{
	setWindowTitle( tr( "Nodes" ) );
	scalePercentage->setSuffix( tr(" %"));

	AbsMode->setText( tr("&Absolute Coordinates"));
	TextLabel1->setText( tr("&X-Pos:"));
	TextLabel2->setText( tr("&Y-Pos:"));
	EditCont->setText( tr("Edit &Contour Line"));
	ResetCont->setText( tr("&Reset Contour Line"));
	ResetContClip->setText( tr("Set Contour to Image Clip"));
//	ResetContClip->setText( tr("Image Clip = Contour"));
	lensEffect->setText( tr("Lens Effects..."));
	lensEffect->setToolTip( tr("Apply fancy Lens Effects"));
	editEditButton->setText( tr("&End Editing"));
	MoveNode->setToolTip( tr("Move Nodes"));
	MoveControl->setToolTip( tr("Move Control Points"));
	AddNode->setToolTip( tr("Add Nodes"));
	DeleteNode->setToolTip( tr("Delete Nodes"));
	AsymMove->setToolTip( tr("Move Control Points Independently"));
	SymMove->setToolTip( tr("Move Control Points Symmetrical"));
	ResNode->setToolTip( tr("Reset Control Points"));
	Res1Node->setToolTip( tr("Reset this Control Point"));
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
	EditCont->setToolTip( tr("Activate Contour Line Editing Mode"));
	ResetCont->setToolTip( tr("Reset the Contour Line to the Original Shape of the Frame"));
	ResetContClip->setToolTip( tr("Reset the Contour Line to the Clipping Path of the Image"));
	AbsMode->setToolTip(  "<qt>" + tr("When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.") + "</qt>");
}

void NodePalette::unitChange()
{
	if (doc==0)
		return;
	double oldRatio = unitRatio;
	unitRatio = doc->unitRatio();
	double maxVal=30000 * unitRatio;
	double minVal=-30000 * unitRatio;
	double newScaleDistance = scaleDistance->value() * unitRatio / oldRatio;
	scaleDistance->setSuffix( unitGetSuffixFromIndex(doc->unitIndex()) );
	int decimals = unitGetPrecisionFromIndex(doc->unitIndex());
	scaleDistance->setValues( minVal, maxVal, decimals, newScaleDistance );
}

ScribusDoc* NodePalette::currentDocument() const
{
	return doc;
}
