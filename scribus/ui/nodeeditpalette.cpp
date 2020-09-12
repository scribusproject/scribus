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

NodePalette::NodePalette( QWidget* parent) : ScrPaletteBase( parent, "nodePalette", false, nullptr)
{
	IconManager& im = IconManager::instance();
	QSize iconSize = QSize(22, 22);
	m_doc = nullptr;
	unitRatio = 1.0;
	vboxLayout = new QVBoxLayout(this);
	vboxLayout->setSpacing(2);
	vboxLayout->setMargin(5);
	gridLayout = new QGridLayout();
	gridLayout->setSpacing(2);
	gridLayout->setMargin(0);
	MoveNode = new QToolButton(this);
	MoveNode->setText( "" );
	MoveNode->setIcon(im.loadIcon("MoveNode.png"));
	MoveNode->setIconSize(iconSize);
	MoveNode->setContentsMargins(0, 0, 0, 0);
	MoveNode->setCheckable(true);
	MoveNode->setChecked(true);
	MoveNode->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(MoveNode, 0, 0, 1, 1);

	AddNode = new QToolButton(this);
	AddNode->setIcon(im.loadIcon("AddNode.png"));
	AddNode->setIconSize(iconSize);
	AddNode->setContentsMargins(0, 0, 0, 0);
	AddNode->setEnabled(true);
	AddNode->setCheckable(true);
	AddNode->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(AddNode, 0, 1, 1, 1);

	DeleteNode = new QToolButton(this);
	DeleteNode->setIcon(im.loadIcon("DeleteNode.png"));
	DeleteNode->setIconSize(iconSize);
	DeleteNode->setContentsMargins(0, 0, 0, 0);
	DeleteNode->setEnabled(true);
	DeleteNode->setCheckable(true);
	DeleteNode->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(DeleteNode, 0, 2, 1, 1);

	ResNode = new QToolButton(this);
	ResNode->setIcon(im.loadIcon("ResetNode.png"));
	ResNode->setIconSize(iconSize);
	ResNode->setContentsMargins(0, 0, 0, 0);
	ResNode->setEnabled(true);
	ResNode->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(ResNode, 0, 3, 1, 1);

	MoveControl = new QToolButton(this);
	MoveControl->setIcon(im.loadIcon("MoveKontrol.png"));
	MoveControl->setIconSize(iconSize);
	MoveControl->setContentsMargins(0, 0, 0, 0);
	MoveControl->setCheckable(true);
	MoveControl->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(MoveControl, 1, 0, 1, 1);

	AsymMove = new QToolButton(this);
	AsymMove->setIcon(im.loadIcon("MoveAsym.png"));
	AsymMove->setIconSize(iconSize);
	AsymMove->setContentsMargins(0, 0, 0, 0);
	AsymMove->setEnabled(false);
	AsymMove->setCheckable(true);
	AsymMove->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(AsymMove, 1, 1, 1, 1);

	SymMove = new QToolButton(this);
	SymMove->setIcon(im.loadIcon("MoveSym.png"));
	SymMove->setIconSize(iconSize);
	SymMove->setContentsMargins(0, 0, 0, 0);
	SymMove->setEnabled(false);
	SymMove->setCheckable(true);
	SymMove->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(SymMove, 1, 2, 1, 1);

	Res1Node = new QToolButton(this);
	Res1Node->setIcon(im.loadIcon("Reset1Node.png"));
	Res1Node->setIconSize(iconSize);
	Res1Node->setContentsMargins(0, 0, 0, 0);
	Res1Node->setEnabled(false);
	Res1Node->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(Res1Node, 1, 3, 1, 1);

	PolySplit = new QToolButton(this);
	PolySplit->setEnabled(false);
	PolySplit->setCheckable(true);
	PolySplit->setIcon(im.loadIcon("PolyCut.png"));
	PolySplit->setIconSize(iconSize);
	PolySplit->setContentsMargins(0, 0, 0, 0);
	PolySplit->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(PolySplit, 2, 0, 1, 1);

	BezierClose = new QToolButton(this);
	BezierClose->setEnabled(false);
	BezierClose->setIcon(im.loadIcon("BezierClose.png"));
	BezierClose->setIconSize(iconSize);
	BezierClose->setContentsMargins(0, 0, 0, 0);
	BezierClose->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(BezierClose, 2, 1, 1, 1);

	PolyMirrorH = new QToolButton(this);
	PolyMirrorH->setIcon(im.loadIcon("hmirror.png"));
	PolyMirrorH->setIconSize(iconSize);
	PolyMirrorH->setContentsMargins(0, 0, 0, 0);
	PolyMirrorH->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(PolyMirrorH, 2, 2, 1, 1);

	PolyMirrorV = new QToolButton(this);
	PolyMirrorV->setIcon(im.loadIcon("vmirror.png"));
	PolyMirrorV->setIconSize(iconSize);
	PolyMirrorV->setContentsMargins(0, 0, 0, 0);
	PolyMirrorV->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	gridLayout->addWidget(PolyMirrorV, 2, 3, 1, 1);

	PolyShearL = new QToolButton(this);
	PolyShearL->setIcon(im.loadIcon("22/transform-shear-left.png"));
	PolyShearL->setIconSize(iconSize);
	PolyShearL->setContentsMargins(0, 0, 0, 0);
	PolyShearL->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	PolyShearL->setAutoRepeat(true);
	gridLayout->addWidget(PolyShearL, 3, 0, 1, 1);

	PolyShearR = new QToolButton(this);
	PolyShearR->setIcon(im.loadIcon("22/transform-shear-right.png"));
	PolyShearR->setIconSize(iconSize);
	PolyShearR->setContentsMargins(0, 0, 0, 0);
	PolyShearR->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	PolyShearR->setAutoRepeat(true);
	gridLayout->addWidget(PolyShearR, 3, 1, 1, 1);

	PolyShearU = new QToolButton(this);
	PolyShearU->setIcon(im.loadIcon("22/transform-shear-up.png"));
	PolyShearU->setIconSize(iconSize);
	PolyShearU->setContentsMargins(0, 0, 0, 0);
	PolyShearU->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
	PolyShearU->setAutoRepeat(true);
	gridLayout->addWidget(PolyShearU, 3, 2, 1, 1);

	PolyShearD = new QToolButton(this);
	PolyShearD->setIcon(im.loadIcon("22/transform-shear-down.png"));
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
	RotateCCW->setIcon(im.loadIcon("rotate_ccw.png"));
	RotateCCW->setIconSize(iconSize);
	RotateCCW->setContentsMargins(0, 0, 0, 0);
	RotateCCW->setAutoRepeat(true);
	gridLayout1->addWidget(RotateCCW, 0, 0, 1, 1);

	RotateCW = new QToolButton(this);
	RotateCW->setIcon(im.loadIcon("rotate_cw.png"));
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
	Expand->setIcon(im.loadIcon("expand.png"));
	Expand->setIconSize(iconSize);
	Expand->setContentsMargins(0, 0, 0, 0);
	Expand->setAutoRepeat(true);
	gridLayout1->addWidget(Expand, 1, 0, 1, 1);

	Shrink = new QToolButton(this);
	Shrink->setIcon(im.loadIcon("crop.png"));
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
	Enlarge->setIcon(im.loadIcon("expand.png"));
	Enlarge->setIconSize(iconSize);
	Enlarge->setContentsMargins(0, 0, 0, 0);
	Enlarge->setAutoRepeat(true);
	gridLayout1->addWidget(Enlarge, 2, 0, 1, 1);

	Reduce = new QToolButton(this);
	Reduce->setIcon(im.loadIcon("crop.png"));
	Reduce->setIconSize(iconSize);
	Reduce->setContentsMargins(0, 0, 0, 0);
	Reduce->setAutoRepeat(true);
	gridLayout1->addWidget(Reduce, 2, 1, 1, 1);

	scaleDistance = new ScrSpinBox( 1, 16777215, this, 0);
	scaleDistance->setValues(-16777215, 16777215, 2, 30);
	gridLayout1->addWidget(scaleDistance, 2, 2, 1, 1);
	vboxLayout->addLayout(gridLayout1);

	gridLayout2 = new QGridLayout();
	gridLayout2->setSpacing(5);
	gridLayout2->setMargin(0);

	QSpacerItem* spacer = new QSpacerItem( 3, 3, QSizePolicy::Fixed, QSizePolicy::Fixed );
	vboxLayout->addItem(spacer);

	AbsMode = new QGroupBox( "&Absolute Coordinates", this );
	AbsMode->setCheckable(true);
	AbsMode->setChecked(false);
	vboxLayout1 = new QVBoxLayout(AbsMode);
	vboxLayout1->setSpacing(2);
	vboxLayout1->setMargin(5);
	absToCanvas = new QRadioButton( tr("to Canvas"), AbsMode);
	vboxLayout1->addWidget(absToCanvas);
	absToPage = new QRadioButton( tr("to Page"), AbsMode);
	vboxLayout1->addWidget(absToPage);
	absToCanvas->setChecked(true);
	gridLayout2->addWidget(AbsMode, 1, 0, 1, 2);

	TextLabel1 = new QLabel(this);
	gridLayout2->addWidget(TextLabel1, 2, 0, 1, 1);

	XSpin = new ScrSpinBox( 0, 16777215, this, 2 );
	XSpin->setEnabled(false);
	gridLayout2->addWidget(XSpin, 2, 1, 1, 1);

	TextLabel2 = new QLabel(this);
	gridLayout2->addWidget(TextLabel2, 3, 0, 1, 1);

	YSpin = new ScrSpinBox( 0, 16777215, this, 2 );
	YSpin->setEnabled(false);
	gridLayout2->addWidget(YSpin, 3, 1, 1, 1);
	TextLabel1->setBuddy(XSpin);
	TextLabel2->setBuddy(YSpin);

	EditCont = new QCheckBox(this);
	EditCont->setChecked(false);
	gridLayout2->addWidget(EditCont, 4, 0, 1, 2);

	PreviewMode = new QCheckBox(this);
	PreviewMode->setChecked(false);
	gridLayout2->addWidget(PreviewMode, 5, 0, 1, 2);

	ResetCont = new QPushButton(this);
	ResetCont->setEnabled(false);
	gridLayout2->addWidget(ResetCont, 6, 0, 1, 2);

	ResetContClip = new QPushButton(this);
	ResetContClip->setEnabled(true);
	ResetContClip->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(6), static_cast<QSizePolicy::Policy>(6)));
	ResetContClip->hide();
	gridLayout2->addWidget(ResetContClip, 7, 0, 1, 2);

	ResetShape2Clip = new QPushButton(this);
	ResetShape2Clip->setEnabled(true);
	ResetShape2Clip->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(6), static_cast<QSizePolicy::Policy>(6)));
	ResetShape2Clip->hide();
	gridLayout2->addWidget(ResetShape2Clip, 8, 0, 1, 2);

	resetDefaultButton = new QPushButton(this);
	resetDefaultButton->setEnabled(true);
	gridLayout2->addWidget(resetDefaultButton, 9, 0, 1, 2);

	editEditButton = new QPushButton(IconManager::instance().loadIcon("22/exit.png"), tr("OK"), this);
	editEditButton->setEnabled(true);
	editEditButton->setDefault(true);
	gridLayout2->addWidget(editEditButton, 10, 0, 1, 1);

	cancelEditButton = new QPushButton( tr("Cancel"), this);
	cancelEditButton->setEnabled(true);
	gridLayout2->addWidget(cancelEditButton, 10, 1, 1, 1);

	vboxLayout->addLayout(gridLayout2);
	resize(QSize(170, 380).expandedTo(minimumSizeHint()));

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
	if (m_doc!=nullptr)
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
		FPoint np(XSpin->value()/m_doc->unitRatio(), YSpin->value()/m_doc->unitRatio());
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
		PageItem*currItem=m_doc->m_Selection->itemAt(0);
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
		QPointF delta = m.map(QPointF(xPos, yPos)) - m.map(QPointF(currItem->xPos(), currItem->yPos()));
		// During operation the image offsets and possibly other values are changed.
		// To not remember everything we move the clip path to the original position
		// relative to the current position (in the item's coordinate space).
		// adjustItemSize will then take care of moving the position and changing
		// image offsets, etc.
		currItem->PoLine = itemPath.copy();
		currItem->PoLine.translate(delta.x(), delta.y());
		currItem->ContourLine = itemContourPath.copy();
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
	QPointF delta = m.map(QPointF(xPos, yPos)) - m.map(QPointF(currItem->xPos(), currItem->yPos()));
	if (EditCont->isChecked())
	{
		currItem->ContourLine = itemContourPath.copy();
		currItem->ContourLine.translate(delta.x(), delta.y());
	}
	else
	{
		// See comment in NodePalette::CancelEdit
		currItem->PoLine = itemPath;
 		currItem->PoLine.translate(delta.x(), delta.y());
 		currItem->ContourLine = itemContourPath;
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
	xPos = currItem->xPos();
	yPos = currItem->yPos();
	itemPath = currItem->PoLine.copy();
	itemContourPath = currItem->ContourLine.copy();
}

void NodePalette::iconSetChange()
{
	IconManager& im = IconManager::instance();
	QSize iconSize = QSize(22, 22);

	MoveNode->setIcon(im.loadIcon("MoveNode.png"));
	MoveNode->setIconSize(iconSize);

	AddNode->setIcon(im.loadIcon("AddNode.png"));
	AddNode->setIconSize(iconSize);

	DeleteNode->setIcon(im.loadIcon("DeleteNode.png"));
	DeleteNode->setIconSize(iconSize);

	ResNode->setIcon(im.loadIcon("ResetNode.png"));
	ResNode->setIconSize(iconSize);

	MoveControl->setIcon(im.loadIcon("MoveKontrol.png"));
	MoveControl->setIconSize(iconSize);

	AsymMove->setIcon(im.loadIcon("MoveAsym.png"));
	AsymMove->setIconSize(iconSize);

	SymMove->setIcon(im.loadIcon("MoveSym.png"));
	SymMove->setIconSize(iconSize);

	Res1Node->setIcon(im.loadIcon("Reset1Node.png"));
	Res1Node->setIconSize(iconSize);

	PolySplit->setIcon(im.loadIcon("PolyCut.png"));
	PolySplit->setIconSize(iconSize);

	BezierClose->setIcon(im.loadIcon("BezierClose.png"));
	BezierClose->setIconSize(iconSize);

	PolyMirrorH->setIcon(im.loadIcon("hmirror.png"));
	PolyMirrorH->setIconSize(iconSize);

	PolyMirrorV->setIcon(im.loadIcon("vmirror.png"));
	PolyMirrorV->setIconSize(iconSize);

	PolyShearL->setIcon(im.loadIcon("22/transform-shear-left.png"));
	PolyShearL->setIconSize(iconSize);

	PolyShearR->setIcon(im.loadIcon("22/transform-shear-right.png"));
	PolyShearR->setIconSize(iconSize);

	PolyShearU->setIcon(im.loadIcon("22/transform-shear-up.png"));
	PolyShearU->setIconSize(iconSize);

	PolyShearD->setIcon(im.loadIcon("22/transform-shear-down.png"));
	PolyShearD->setIconSize(iconSize);

	RotateCCW->setIcon(im.loadIcon("rotate_ccw.png"));
	RotateCCW->setIconSize(iconSize);

	RotateCW->setIcon(im.loadIcon("rotate_cw.png"));
	RotateCW->setIconSize(iconSize);

	Expand->setIcon(im.loadIcon("expand.png"));
	Expand->setIconSize(iconSize);

	Shrink->setIcon(im.loadIcon("crop.png"));
	Shrink->setIconSize(iconSize);

	Enlarge->setIcon(im.loadIcon("expand.png"));
	Enlarge->setIconSize(iconSize);

	Reduce->setIcon(im.loadIcon("crop.png"));
	Reduce->setIconSize(iconSize);
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
	unitRatio = m_doc->unitRatio();

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
