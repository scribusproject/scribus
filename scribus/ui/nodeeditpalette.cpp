/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

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
	QSize iconSize = QSize(22, 22);
	QSize buttonSize = QSize(24, 24);
	QSize maxButtonSize = QSize(16777215, buttonSize.height());
	QSizePolicy::Policy buttonSizePolicy = static_cast<QSizePolicy::Policy>(QSizePolicy::GrowFlag | QSizePolicy::ExpandFlag);

	m_doc = nullptr;
	m_unitRatio = 1.0;
	vboxLayout = new QVBoxLayout(this);
	vboxLayout->setSpacing(6);
	vboxLayout->setContentsMargins(6, 6, 6, 6);

	MoveNode = new QToolButton(this);
	MoveNode->setIconSize(iconSize);
	MoveNode->setMinimumSize(buttonSize);
	MoveNode->setMaximumSize(maxButtonSize);
	MoveNode->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	MoveNode->setCheckable(true);
	MoveNode->setChecked(true);

	AddNode = new QToolButton(this);
	AddNode->setIconSize(iconSize);
	AddNode->setMinimumSize(buttonSize);
	AddNode->setMaximumSize(maxButtonSize);
	AddNode->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	AddNode->setEnabled(true);
	AddNode->setCheckable(true);

	DeleteNode = new QToolButton(this);
	DeleteNode->setIconSize(iconSize);
	DeleteNode->setMinimumSize(buttonSize);
	DeleteNode->setMaximumSize(maxButtonSize);
	DeleteNode->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	DeleteNode->setEnabled(true);
	DeleteNode->setCheckable(true);

	ResNode = new QToolButton(this);
	ResNode->setIconSize(iconSize);
	ResNode->setMinimumSize(buttonSize);
	ResNode->setMaximumSize(maxButtonSize);
	ResNode->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	ResNode->setEnabled(true);

	MoveControl = new QToolButton(this);
	MoveControl->setIconSize(iconSize);
	MoveControl->setMinimumSize(buttonSize);
	MoveControl->setMaximumSize(maxButtonSize);
	MoveControl->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	MoveControl->setCheckable(true);

	AsymMove = new QToolButton(this);
	AsymMove->setIconSize(iconSize);
	AsymMove->setMinimumSize(buttonSize);
	AsymMove->setMaximumSize(maxButtonSize);
	AsymMove->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	AsymMove->setEnabled(false);
	AsymMove->setCheckable(true);

	SymMove = new QToolButton(this);
	SymMove->setIconSize(iconSize);
	SymMove->setMinimumSize(buttonSize);
	SymMove->setMaximumSize(maxButtonSize);
	SymMove->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	SymMove->setEnabled(false);
	SymMove->setCheckable(true);

	Res1Node = new QToolButton(this);
	Res1Node->setIconSize(iconSize);
	Res1Node->setMinimumSize(buttonSize);
	Res1Node->setMaximumSize(maxButtonSize);
	Res1Node->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	Res1Node->setEnabled(false);

	PolySplit = new QToolButton(this);
	PolySplit->setEnabled(false);
	PolySplit->setCheckable(true);
	PolySplit->setIconSize(iconSize);
	PolySplit->setMinimumSize(buttonSize);
	PolySplit->setMaximumSize(maxButtonSize);
	PolySplit->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));

	BezierClose = new QToolButton(this);
	BezierClose->setEnabled(false);
	BezierClose->setIconSize(iconSize);
	BezierClose->setMinimumSize(buttonSize);
	BezierClose->setMaximumSize(maxButtonSize);
	BezierClose->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));

	PolyMirrorH = new QToolButton(this);
	PolyMirrorH->setIconSize(iconSize);
	PolyMirrorH->setMinimumSize(buttonSize);
	PolyMirrorH->setMaximumSize(maxButtonSize);
	PolyMirrorH->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));

	PolyMirrorV = new QToolButton(this);
	PolyMirrorV->setIconSize(iconSize);
	PolyMirrorV->setMinimumSize(buttonSize);
	PolyMirrorV->setMaximumSize(maxButtonSize);
	PolyMirrorV->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));

	PolyShearL = new QToolButton(this);
	PolyShearL->setIconSize(iconSize);
	PolyShearL->setMinimumSize(buttonSize);
	PolyShearL->setMaximumSize(maxButtonSize);
	PolyShearL->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	PolyShearL->setAutoRepeat(true);

	PolyShearR = new QToolButton(this);
	PolyShearR->setIconSize(iconSize);
	PolyShearR->setMinimumSize(buttonSize);
	PolyShearR->setMaximumSize(maxButtonSize);
	PolyShearR->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	PolyShearR->setAutoRepeat(true);

	PolyShearU = new QToolButton(this);
	PolyShearU->setIconSize(iconSize);
	PolyShearU->setMinimumSize(buttonSize);
	PolyShearU->setMaximumSize(maxButtonSize);
	PolyShearU->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	PolyShearU->setAutoRepeat(true);

	PolyShearD = new QToolButton(this);
	PolyShearD->setIconSize(iconSize);
	PolyShearD->setMinimumSize(buttonSize);
	PolyShearD->setMaximumSize(maxButtonSize);
	PolyShearD->setSizePolicy(QSizePolicy(buttonSizePolicy, QSizePolicy::Fixed));
	PolyShearD->setAutoRepeat(true);

	RotateCCW = new QToolButton(this);
	RotateCCW->setIconSize(iconSize);
	RotateCCW->setMinimumSize(buttonSize);
	RotateCCW->setMaximumSize(buttonSize);
	RotateCCW->setAutoRepeat(true);

	RotateCW = new QToolButton(this);
	RotateCW->setIconSize(iconSize);
	RotateCW->setMinimumSize(buttonSize);
	RotateCW->setMaximumSize(buttonSize);
	RotateCW->setAutoRepeat(true);

	RotVal = new QSpinBox(this);
	RotVal->setMinimum(1);
	RotVal->setMaximum(180);
	RotVal->setValue(1);
	RotVal->setSuffix( QString::fromUtf8(" °"));

	Expand = new QToolButton(this);
	Expand->setIconSize(iconSize);
	Expand->setMinimumSize(buttonSize);
	Expand->setMaximumSize(buttonSize);
	Expand->setAutoRepeat(true);

	Shrink = new QToolButton(this);
	Shrink->setIconSize(iconSize);
	Shrink->setMinimumSize(buttonSize);
	Shrink->setMaximumSize(buttonSize);
	Shrink->setAutoRepeat(true);

	scalePercentage = new QSpinBox(this);
	scalePercentage->setMinimum(1);
	scalePercentage->setMaximum(100);
	scalePercentage->setValue(10);

	Enlarge = new QToolButton(this);
	Enlarge->setIconSize(iconSize);
	Enlarge->setMinimumSize(buttonSize);
	Enlarge->setMaximumSize(buttonSize);
	Enlarge->setAutoRepeat(true);

	Reduce = new QToolButton(this);
	Reduce->setIconSize(iconSize);
	Reduce->setMinimumSize(buttonSize);
	Reduce->setMaximumSize(buttonSize);
	Reduce->setAutoRepeat(true);

	scaleDistance = new ScrSpinBox( 1, 16777215, this, 0);
	scaleDistance->setMaximumSize(114, 16777215); // ScrSpinBox is bigger for 4px than expected
	scaleDistance->setValues(-16777215, 16777215, 2, 30);

	gridLayout = new QGridLayout();
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	gridLayout->addWidget(MoveNode,        0, 1, 1, 1);
	gridLayout->addWidget(AddNode,         0, 2, 1, 1);
	gridLayout->addWidget(DeleteNode,      0, 3, 1, 1);
	gridLayout->addWidget(ResNode,         0, 4, 1, 1);
	gridLayout->addWidget(MoveControl,     1, 1, 1, 1);
	gridLayout->addWidget(AsymMove,        1, 2, 1, 1);
	gridLayout->addWidget(SymMove,         1, 3, 1, 1);
	gridLayout->addWidget(Res1Node,        1, 4, 1, 1);
	gridLayout->addWidget(PolySplit,       2, 1, 1, 1);
	gridLayout->addWidget(BezierClose,     2, 2, 1, 1);
	gridLayout->addWidget(PolyMirrorH,     2, 3, 1, 1);
	gridLayout->addWidget(PolyMirrorV,     2, 4, 1, 1);
	gridLayout->addWidget(PolyShearL,      3, 1, 1, 1);
	gridLayout->addWidget(PolyShearR,      3, 2, 1, 1);
	gridLayout->addWidget(PolyShearU,      3, 3, 1, 1);
	gridLayout->addWidget(PolyShearD,      3, 4, 1, 1);
	vboxLayout->addLayout(gridLayout);

	gridLayout2 = new QGridLayout();
	gridLayout2->setSpacing(6);
	gridLayout2->setContentsMargins(0, 0, 0, 0);
	gridLayout2->addWidget(RotVal,          0, 0, 1, 4);
	gridLayout2->addWidget(RotateCCW,       0, 4, 1, 1);
	gridLayout2->addWidget(RotateCW,        0, 5, 1, 1);
	gridLayout2->addWidget(scalePercentage, 1, 0, 1, 4);
	gridLayout2->addWidget(Expand,          1, 4, 1, 1);
	gridLayout2->addWidget(Shrink,          1, 5, 1, 1);
	gridLayout2->addWidget(scaleDistance,   2, 0, 1, 4);
	gridLayout2->addWidget(Enlarge,         2, 4, 1, 1);
	gridLayout2->addWidget(Reduce,          2, 5, 1, 1);
	vboxLayout->addLayout(gridLayout2);

	QSpacerItem* spacer = new QSpacerItem( 6, 6, QSizePolicy::Fixed, QSizePolicy::Fixed );
	vboxLayout->addItem(spacer);

	AbsMode = new QGroupBox( "&Absolute Coordinates", this );
	AbsMode->setCheckable(true);
	AbsMode->setChecked(false);
	vboxLayout1 = new QVBoxLayout(AbsMode);
	vboxLayout1->setSpacing(6);
	vboxLayout1->setContentsMargins(9, 9, 9, 9);
	absToCanvas = new QRadioButton( tr("to Canvas"), AbsMode);
	vboxLayout1->addWidget(absToCanvas);
	absToPage = new QRadioButton( tr("to Page"), AbsMode);
	vboxLayout1->addWidget(absToPage);
	absToCanvas->setChecked(true);
	vboxLayout->addWidget(AbsMode);

	gridLayout3 = new QGridLayout();
	gridLayout3->setSpacing(6);
	gridLayout3->setContentsMargins(0, 0, 0, 0);
	gridLayout3->setColumnStretch(0, 0);
	gridLayout3->setColumnStretch(1, 1);

	TextLabel1 = new QLabel(this);
	gridLayout3->addWidget(TextLabel1, 0, 0, 1, 1);

	XSpin = new ScrSpinBox( 0, 16777215, this, 2 );
	XSpin->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	XSpin->setEnabled(false);
	gridLayout3->addWidget(XSpin, 0, 1, 1, 1);

	TextLabel2 = new QLabel(this);
	gridLayout3->addWidget(TextLabel2, 1, 0, 1, 1);

	YSpin = new ScrSpinBox( 0, 16777215, this, 2 );
	YSpin->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	YSpin->setEnabled(false);
	gridLayout3->addWidget(YSpin, 1, 1, 1, 1);
	vboxLayout->addLayout(gridLayout3);

	TextLabel1->setBuddy(XSpin);
	TextLabel2->setBuddy(YSpin);

	EditCont = new QCheckBox(this);
	EditCont->setChecked(false);
	vboxLayout->addWidget(EditCont);

	PreviewMode = new QCheckBox(this);
	PreviewMode->setChecked(false);
	vboxLayout->addWidget(PreviewMode);

	ResetCont = new QPushButton(this);
	ResetCont->setEnabled(false);
	vboxLayout->addWidget(ResetCont);

	ResetContClip = new QPushButton(this);
	ResetContClip->setEnabled(true);
	ResetContClip->hide();
	vboxLayout->addWidget(ResetContClip);

	ResetShape2Clip = new QPushButton(this);
	ResetShape2Clip->setEnabled(true);
	ResetShape2Clip->hide();
	vboxLayout->addWidget(ResetShape2Clip);

	resetDefaultButton = new QPushButton(this);
	resetDefaultButton->setEnabled(true);
	vboxLayout->addWidget(resetDefaultButton);

	hboxLayout = new QHBoxLayout();
	hboxLayout->setSpacing(6);
	hboxLayout->setContentsMargins(0, 0, 0, 0);
	editEditButton = new QPushButton(IconManager::instance().loadIcon("22/exit.png"), tr("OK"), this);
	editEditButton->setEnabled(true);
	editEditButton->setDefault(true);
	hboxLayout->addWidget(editEditButton);
	cancelEditButton = new QPushButton( tr("Cancel"), this);
	cancelEditButton->setEnabled(true);
	hboxLayout->addWidget(cancelEditButton);
	vboxLayout->addLayout(hboxLayout);

	resize(QSize(170, 380).expandedTo(minimumSizeHint()));
	layout()->setSizeConstraint(QLayout::SetFixedSize);
	iconSetChange();
	languageChange();

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
	connect(AbsMode, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	connect(absToCanvas, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	connect(absToPage, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	connect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleConMode()));
	connect(ResetCont, SIGNAL(clicked()), this, SLOT(ResetContour()));
	connect(ResetContClip, SIGNAL(clicked()), this, SLOT(ResetContourToImageClip()));
	connect(ResetShape2Clip, SIGNAL(clicked()), this, SLOT(ResetShapeToImageClip()));
	connect(PreviewMode, SIGNAL(clicked()), this, SLOT(TogglePreview()));
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
	disconnect(ResetCont, SIGNAL(clicked()), this, SLOT(ResetContour()));
	disconnect(ResetContClip, SIGNAL(clicked()), this, SLOT(ResetContourToImageClip()));
	disconnect(ResetShape2Clip, SIGNAL(clicked()), this, SLOT(ResetShapeToImageClip()));
	disconnect(PreviewMode, SIGNAL(clicked()), this, SLOT(TogglePreview()));
}

void NodePalette::setDoc(ScribusDoc *dc, ScribusView *vi)
{
	m_doc = dc;
	m_view = vi;

	unitChange();
	disconnect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleConMode()));
	disconnect(AbsMode, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	if (m_doc != nullptr)
	{
		YSpin->setNewUnit(m_doc->unitIndex());
		XSpin->setNewUnit(m_doc->unitIndex());
	}
	AbsMode->setChecked(false);
	EditCont->setChecked(false);
	if (m_doc == nullptr)
		disconnectSignals();
	else
		connectSignals();
}

void NodePalette::SplitPoly()
{
	if (m_doc == nullptr)
		return;
	MoveN();
	m_doc->nodeEdit.setSubMode(NodeEditContext::SPLIT_PATH);
	PolySplit->setChecked(true);
}

void NodePalette::IsOpen()
{
	if (m_doc == nullptr)
		return;
	PolySplit->setChecked(false);
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
	m_view->TransformPoly(8, 1, value2pts(scaleDistance->value(),m_doc->unitIndex()));
}

void NodePalette::doEnlarge()
{
	if (m_doc == nullptr)
		return;
	m_view->TransformPoly(9, 1, value2pts(scaleDistance->value(),m_doc->unitIndex()));
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

void NodePalette::ResetControl()
{
	if (m_doc == nullptr)
		return;
	m_doc->nodeEdit.resetControl(m_doc->m_Selection->itemAt(0));
}

void NodePalette::Reset1Control()
{
	if (m_doc == nullptr)
		return;
	m_doc->nodeEdit.reset1Control(m_doc->m_Selection->itemAt(0));
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
		if (AbsMode->isChecked())
		{
			if (absToCanvas->isChecked())
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
	if (m_doc != nullptr)
		m_doc->nodeEdit.setMoveSym(true);
	SymMove->setChecked(true);
	AsymMove->setChecked(false);
	PolySplit->setChecked(false);
}

void NodePalette::SetAsym()
{
	if (m_doc != nullptr)
		m_doc->nodeEdit.setMoveSym(false);
	SymMove->setChecked(false);
	AsymMove->setChecked(true);
	PolySplit->setChecked(false);
}

void NodePalette::SetXY(double x, double y)
{
	if (m_doc == nullptr)
		return;
	FPoint zp(0.0, 0.0);
	disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	if (AbsMode->isChecked())
	{
		if (absToCanvas->isChecked())
			zp = FPoint(m_doc->m_Selection->itemAt(0)->xPos(), m_doc->m_Selection->itemAt(0)->yPos());
		else
			zp = FPoint(m_doc->m_Selection->itemAt(0)->xPos() - m_doc->currentPage()->xOffset(), m_doc->m_Selection->itemAt(0)->yPos() - m_doc->currentPage()->yOffset());
	}
	XSpin->setValue((x + zp.x()) * m_doc->unitRatio());
	YSpin->setValue((y + zp.y()) * m_doc->unitRatio());
	connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(MovePoint()));
}

void NodePalette::ToggleAbsMode()
{
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
	if (AbsMode->isChecked())
	{
		XSpin->setMinimum(-16777215);
		YSpin->setMinimum(-16777215);
		if (absToCanvas->isChecked())
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

void NodePalette::ToggleConMode()
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
		if (AbsMode->isChecked())
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

void NodePalette::HaveNode(bool have, bool mov)
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
	if (setter)
	{
		if (m_doc->nodeEdit.edPoints())
			ResNode->setEnabled(setter);
		else
			Res1Node->setEnabled(setter);
	}
	else
	{
		ResNode->setEnabled(setter);
		Res1Node->setEnabled(setter);
	}
	if (m_doc->nodeEdit.submode() == NodeEditContext::MOVE_POINT)
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
		{
			MoveN();
			MoveControl->setEnabled(true);
		}
	}
}

void NodePalette::MoveK()
{
	if (m_doc == nullptr)
		return;
	m_doc->nodeEdit.setSubMode(NodeEditContext::MOVE_POINT);
	m_doc->nodeEdit.setEdPoints(false);
	m_doc->nodeEdit.setClre(-1);
	m_doc->nodeEdit.setClre2(-1);
	m_doc->nodeEdit.setSegP1(-1);
	m_doc->nodeEdit.setSegP2(-1);
	if (m_doc->m_Selection->count() > 0)
		m_doc->m_Selection->itemAt(0)->update();
	SymMove->setEnabled(true);
	AsymMove->setEnabled(true);
	Res1Node->setEnabled(false);
	ResNode->setEnabled(false);
	AddNode->setEnabled(false);
	DeleteNode->setEnabled(false);
	MoveNode->setChecked(false);
	MoveControl->setChecked(true);
	DeleteNode->setChecked(false);
	AddNode->setChecked(false);
	PolySplit->setChecked(false);
}

void NodePalette::MoveN()
{
	if (m_doc == nullptr)
		return;
	m_doc->nodeEdit.setSubMode(NodeEditContext::MOVE_POINT);
	m_doc->nodeEdit.setEdPoints(true);
	m_doc->nodeEdit.setClre(-1);
	m_doc->nodeEdit.setClre2(-1);
	m_doc->nodeEdit.setSegP1(-1);
	m_doc->nodeEdit.setSegP2(-1);
	if (m_doc->m_Selection->count() > 0)
		m_doc->m_Selection->itemAt(0)->update();
	ResNode->setEnabled(false);
	AddNode->setEnabled(true);
	DeleteNode->setEnabled(true);
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	Res1Node->setEnabled(false);
	MoveNode->setChecked(true);
	MoveControl->setChecked(false);
	DeleteNode->setChecked(false);
	AddNode->setChecked(false);
	PolySplit->setChecked(false);
}

void NodePalette::AddN()
{
	if (m_doc == nullptr)
		return;
	m_doc->nodeEdit.setSubMode(NodeEditContext::ADD_POINT);
	m_doc->nodeEdit.setEdPoints(true);
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	ResNode->setEnabled(false);
	Res1Node->setEnabled(false);
	AddNode->setChecked(true);
	DeleteNode->setChecked(false);
	PolySplit->setChecked(false);
}

void NodePalette::DelN()
{
	if (m_doc == nullptr)
		return;
	m_doc->nodeEdit.setSubMode(NodeEditContext::DEL_POINT);
	m_doc->nodeEdit.setEdPoints(true);
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	ResNode->setEnabled(false);
	Res1Node->setEnabled(false);
	AddNode->setChecked(false);
	DeleteNode->setChecked(true);
	PolySplit->setChecked(false);
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
		ToggleConMode();
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
		ToggleConMode();
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

	MoveNode->setIcon(im.loadIcon("MoveNode.png"));
	AddNode->setIcon(im.loadIcon("AddNode.png"));
	DeleteNode->setIcon(im.loadIcon("DeleteNode.png"));
	ResNode->setIcon(im.loadIcon("ResetNode.png"));
	MoveControl->setIcon(im.loadIcon("MoveKontrol.png"));
	AsymMove->setIcon(im.loadIcon("MoveAsym.png"));
	SymMove->setIcon(im.loadIcon("MoveSym.png"));
	Res1Node->setIcon(im.loadIcon("Reset1Node.png"));
	PolySplit->setIcon(im.loadIcon("PolyCut.png"));
	BezierClose->setIcon(im.loadIcon("BezierClose.png"));
	PolyMirrorH->setIcon(im.loadIcon("hmirror.png"));
	PolyMirrorV->setIcon(im.loadIcon("vmirror.png"));
	PolyShearL->setIcon(im.loadIcon("22/transform-shear-left.png"));
	PolyShearR->setIcon(im.loadIcon("22/transform-shear-right.png"));
	PolyShearU->setIcon(im.loadIcon("22/transform-shear-up.png"));
	PolyShearD->setIcon(im.loadIcon("22/transform-shear-down.png"));
	RotateCCW->setIcon(im.loadIcon("rotate_ccw.png"));
	RotateCW->setIcon(im.loadIcon("rotate_cw.png"));
	Expand->setIcon(im.loadIcon("expand.png"));
	Shrink->setIcon(im.loadIcon("crop.png"));
	Enlarge->setIcon(im.loadIcon("expand.png"));
	Reduce->setIcon(im.loadIcon("crop.png"));
}

void NodePalette::languageChange()
{
	setWindowTitle( tr( "Nodes" ) );
	scalePercentage->setSuffix( tr(" %"));

	AbsMode->setTitle( tr("&Absolute Coordinates"));
	absToCanvas->setText( tr("to Canvas"));
	absToPage->setText( tr("to Page"));
	TextLabel1->setText( tr("&X-Pos:"));
	TextLabel2->setText( tr("&Y-Pos:"));
	PreviewMode->setText( tr("Hide Nodes"));
	EditCont->setText( tr("Edit &Contour Line"));
	ResetCont->setText( tr("&Reset Contour Line"));
	ResetContClip->setText( tr("Set Contour to Image Clip"));
	ResetShape2Clip->setText( tr("Set Shape to Image Clip"));
	editEditButton->setText( tr("OK"));
	cancelEditButton->setText( tr("Cancel"));
	resetDefaultButton->setText( tr("Reset all edits"));
//	editEditButton->setText( tr("&End Editing"));
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
	ResetShape2Clip->setToolTip( tr("Set the Shape to the Clipping Path of the Image"));
	AbsMode->setToolTip(  "<qt>" + tr("When checked use coordinates relative to the page, otherwise coordinates are relative to the Object") + "</qt>");
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
