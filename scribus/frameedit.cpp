#include "frameedit.h"
#include "frameedit.moc"
#include "page.h"
#include "pageitem.h"
#include "scribusview.h"
#include "selection.h"
#include "units.h"
#include "undomanager.h"
#include "undostate.h"

extern QPixmap loadIcon(QString nam);

NodePalette::NodePalette( QWidget* parent) : ScrPaletteBase( parent, "nodePalette", false, 0)
{
	setIcon(loadIcon("AppIcon.png"));
	NodePaletteLayout = new QVBoxLayout( this );
	NodePaletteLayout->setSpacing( 2 );
	NodePaletteLayout->setMargin( 5 );

	ButtonGroup1 = new QButtonGroup( this, "ButtonGroup1" );
	ButtonGroup1->setFrameShape( QButtonGroup::NoFrame );
	ButtonGroup1->setFrameShadow( QButtonGroup::Plain );
	ButtonGroup1->setTitle( "" );
	ButtonGroup1->setExclusive( true );
	ButtonGroup1->setColumnLayout(0, Qt::Vertical );
	ButtonGroup1->layout()->setSpacing( 0 );
	ButtonGroup1->layout()->setMargin( 0 );
	ButtonGroup1Layout = new QGridLayout( ButtonGroup1->layout() );
	ButtonGroup1Layout->setAlignment( Qt::AlignTop );
	ButtonGroup1Layout->setSpacing( 0 );
	ButtonGroup1Layout->setMargin( 0 );

	MoveNode = new QToolButton( ButtonGroup1, "MoveNode" );
	MoveNode->setText( "" );
	MoveNode->setPixmap(loadIcon("MoveNode.png"));
	MoveNode->setToggleButton( true );
	MoveNode->setOn(true);
	ButtonGroup1Layout->addWidget( MoveNode, 0, 0 );
	MoveControl = new QToolButton( ButtonGroup1, "MoveControl" );
	MoveControl->setText( "" );
	MoveControl->setPixmap(loadIcon("MoveKontrol.png"));
	MoveControl->setToggleButton( true );
	ButtonGroup1Layout->addWidget( MoveControl, 0, 1 );
	AddNode = new QToolButton( ButtonGroup1, "AddNode" );
	AddNode->setText( "" );
	AddNode->setPixmap(loadIcon("AddNode.png"));
	AddNode->setToggleButton( true );
	ButtonGroup1Layout->addWidget( AddNode, 0, 2 );
	DeleteNode = new QToolButton( ButtonGroup1, "DeleteNode" );
	DeleteNode->setText( "" );
	DeleteNode->setPixmap(loadIcon("DeleteNode.png"));
	DeleteNode->setToggleButton( true );
	ButtonGroup1Layout->addWidget( DeleteNode, 0, 3 );

	AsymMove = new QToolButton( ButtonGroup1, "AsymMove" );
	AsymMove->setEnabled( false );
	AsymMove->setText( "" );
	AsymMove->setPixmap(loadIcon("MoveAsym.png"));
	AsymMove->setToggleButton( true );
	ButtonGroup1Layout->addWidget( AsymMove, 1, 0 );
	SymMove = new QToolButton( ButtonGroup1, "SymMove" );
	SymMove->setEnabled( false );
	SymMove->setText( "" );
	SymMove->setPixmap(loadIcon("MoveSym.png"));
	SymMove->setToggleButton( true );
	ButtonGroup1Layout->addWidget( SymMove, 1, 1 );
	ResNode = new QToolButton( ButtonGroup1, "resnode" );
	ResNode->setEnabled( false );
	ResNode->setText( "" );
	ResNode->setPixmap(loadIcon("ResetNode.png"));
	ButtonGroup1Layout->addWidget( ResNode, 1, 2 );
	Res1Node = new QToolButton( ButtonGroup1, "res1node" );
	Res1Node->setEnabled( false );
	Res1Node->setText( "" );
	Res1Node->setPixmap(loadIcon("Reset1Node.png"));
	ButtonGroup1Layout->addWidget( Res1Node, 1, 3 );

	PolySplit = new QToolButton( ButtonGroup1, "PolySplit" );
	PolySplit->setEnabled( false );
	PolySplit->setText( "" );
	PolySplit->setToggleButton( true );
	PolySplit->setPixmap(loadIcon("PolyCut.png"));
	ButtonGroup1Layout->addWidget( PolySplit, 2, 0 );
	BezierClose = new QToolButton( ButtonGroup1, "BezierClose" );
	BezierClose->setEnabled( false );
	BezierClose->setText( "" );
	BezierClose->setPixmap(loadIcon("BezierClose.png"));
	ButtonGroup1Layout->addWidget( BezierClose, 2, 1 );

	PolyMirrorH = new QToolButton( ButtonGroup1, "MirrorH" );
	PolyMirrorH->setText( "" );
	PolyMirrorH->setPixmap(loadIcon("hmirror.png"));
	ButtonGroup1Layout->addWidget( PolyMirrorH, 2, 2 );

	PolyMirrorV = new QToolButton( ButtonGroup1, "MirrorV" );
	PolyMirrorV->setText( "" );
	PolyMirrorV->setPixmap(loadIcon("vmirror.png"));
	ButtonGroup1Layout->addWidget( PolyMirrorV, 2, 3 );

	PolyShearL = new QToolButton( ButtonGroup1, "ShearL" );
	PolyShearL->setAutoRepeat(true);
	PolyShearL->setText( "" );
	PolyShearL->setPixmap(loadIcon("shear_left.png"));
	ButtonGroup1Layout->addWidget( PolyShearL, 3, 0 );
	PolyShearR = new QToolButton( ButtonGroup1, "ShearR" );
	PolyShearR->setAutoRepeat(true);
	PolyShearR->setText( "" );
	PolyShearR->setPixmap(loadIcon("shear_right.png"));
	ButtonGroup1Layout->addWidget( PolyShearR, 3, 1 );
	PolyShearU = new QToolButton( ButtonGroup1, "ShearU" );
	PolyShearU->setAutoRepeat(true);
	PolyShearU->setText( "" );
	PolyShearU->setPixmap(loadIcon("shear_up.png"));
	ButtonGroup1Layout->addWidget( PolyShearU, 3, 2 );
	PolyShearD = new QToolButton( ButtonGroup1, "ShearD" );
	PolyShearD->setAutoRepeat(true);
	PolyShearD->setText( "" );
	PolyShearD->setPixmap(loadIcon("shear_down.png"));
	ButtonGroup1Layout->addWidget( PolyShearD, 3, 3 );

	RotateCCW = new QToolButton( ButtonGroup1, "RotateCCW" );
	RotateCCW->setAutoRepeat(true);
	RotateCCW->setText( "" );
	RotateCCW->setPixmap(loadIcon("rotate_ccw.png"));
	ButtonGroup1Layout->addWidget( RotateCCW, 4, 0 );
	RotateCW = new QToolButton( ButtonGroup1, "RotateCW" );
	RotateCW->setAutoRepeat(true);
	RotateCW->setText( "" );
	RotateCW->setPixmap(loadIcon("rotate_cw.png"));
	ButtonGroup1Layout->addWidget( RotateCW, 4, 1 );
	RotVal = new QSpinBox( 1, 180, 1, ButtonGroup1, "RotVal");
	RotVal->setSuffix( QString::fromUtf8(" °"));
	ButtonGroup1Layout->addMultiCellWidget( RotVal, 4, 4, 2, 3 );

	Expand = new QToolButton( ButtonGroup1, "Expand" );
	Expand->setAutoRepeat(true);
	Expand->setText( "" );
	Expand->setPixmap(loadIcon("expand.png"));
	ButtonGroup1Layout->addWidget( Expand, 5, 0 );
	Crop = new QToolButton( ButtonGroup1, "Crop" );
	Crop->setAutoRepeat(true);
	Crop->setText( "" );
	Crop->setPixmap(loadIcon("crop.png"));
	ButtonGroup1Layout->addWidget( Crop, 5, 1 );
	ScaleVal = new QSpinBox( ButtonGroup1, "RotVal");
	ScaleVal->setMinValue(1);
	ScaleVal->setMaxValue(100);
	ScaleVal->setValue(10);
	ButtonGroup1Layout->addMultiCellWidget( ScaleVal, 5, 5, 2, 3 );

	/*    QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	    ButtonGroup4Layout->addItem( spacer_2 );  */
	NodePaletteLayout->addWidget( ButtonGroup1 );

	AbsMode = new QCheckBox( "&Absolute Coordinates", this, "Textflow" );
	AbsMode->setChecked(false);
	NodePaletteLayout->addWidget( AbsMode );

	Layout2 = new QGridLayout( 0, 1, 1, 0, 5, "Layout2");
	XSpin = new MSpinBox( this, 2 );
	XSpin->setMaxValue(30000);
	XSpin->setEnabled(false);
	YSpin = new MSpinBox( this, 2 );
	YSpin->setMaxValue(30000);
	YSpin->setEnabled(false);
	TextLabel1 = new QLabel( XSpin, "&X-Pos:", this, "TextLabel1" );
	TextLabel2 = new QLabel( YSpin, "&Y-Pos:", this, "TextLabel2" );
	Layout2->addWidget( TextLabel1, 0, 0 );
	Layout2->addWidget( TextLabel2, 1, 0 );
	Layout2->addWidget( XSpin, 0, 1 );
	Layout2->addWidget( YSpin, 1, 1 );
	NodePaletteLayout->addLayout( Layout2 );

	EditCont = new QCheckBox( "Edit &Contour Line", this, "EditCont" );
	EditCont->setChecked(false);
	NodePaletteLayout->addWidget( EditCont );

	ResetCont = new QPushButton( "&Reset Contour Line", this, "PushButton1" );
	NodePaletteLayout->addWidget( ResetCont );

	PushButton1 = new QPushButton( "&End Editing", this, "PushButton1" );
	PushButton1->setDefault(true);
	NodePaletteLayout->addWidget( PushButton1 );

	languageChange();
	
	// signals and slots connections
	connect(PushButton1, SIGNAL(clicked()), this, SLOT(EndEdit()));
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
	connect(XSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	connect(PolyMirrorH, SIGNAL(clicked()), this, SLOT(MirrorH()));
	connect(PolyMirrorV, SIGNAL(clicked()), this, SLOT(MirrorV()));
	connect(PolyShearR, SIGNAL(clicked()), this, SLOT(ShearR()));
	connect(PolyShearL, SIGNAL(clicked()), this, SLOT(ShearL()));
	connect(PolyShearU, SIGNAL(clicked()), this, SLOT(ShearU()));
	connect(PolyShearD, SIGNAL(clicked()), this, SLOT(ShearD()));
	connect(RotateCCW, SIGNAL(clicked()), this, SLOT(doRotCCW()));
	connect(RotateCW, SIGNAL(clicked()), this, SLOT(doRotCW()));
	connect(Crop, SIGNAL(clicked()), this, SLOT(doCrop()));
	connect(Expand, SIGNAL(clicked()), this, SLOT(doExpand()));
	connect(AbsMode, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	connect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleConMode()));
	connect(ResetCont, SIGNAL(clicked()), this, SLOT(ResetContour()));
}

void NodePalette::setDoc(ScribusDoc *dc, ScribusView *vi)
{
	doc = dc;
	view = vi;
	disconnect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleConMode()));
	disconnect(AbsMode, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	YSpin->setSuffix(unitGetSuffixFromIndex(doc->unitIndex()));
	XSpin->setSuffix(unitGetSuffixFromIndex(doc->unitIndex()));
	AbsMode->setChecked(false);
	EditCont->setChecked(false);
	connect(AbsMode, SIGNAL(clicked()), this, SLOT(ToggleAbsMode()));
	connect(EditCont, SIGNAL(clicked()), this, SLOT(ToggleConMode()));
}

void NodePalette::SplitPoly()
{
	MoveN();
	doc->EditClipMode = 3;
	PolySplit->setOn(true);
}

void NodePalette::IsOpen()
{
	PolySplit->setOn(false);
	BezierClose->setEnabled(true);
	PolySplit->setEnabled(true);
	EditCont->setEnabled(false);
}

void NodePalette::PolyStatus(int typ, uint size)
{
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
	MoveN();
	view->Bezier2Poly();
	BezierClose->setEnabled(false);
	PolySplit->setEnabled(true);
	EditCont->setEnabled(true);
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

void NodePalette::doCrop()
{
	if (doc != 0)
		view->TransformPoly(2, 1, ScaleVal->value());
}

void NodePalette::doExpand()
{
	if (doc != 0)
		view->TransformPoly(3, 1, ScaleVal->value());
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
		view->MirrorPolyH();
}

void NodePalette::MirrorV()
{
	if (doc != 0)
		view->MirrorPolyV();
}

void NodePalette::ResetControl()
{
	view->ResetControl();
}

void NodePalette::Reset1Control()
{
	view->Reset1Control();
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
			is->setItem(doc->selection->itemAt(0)->ContourLine);
			//UndoManager::instance()->action(view->SelItem.at(0), is);
			UndoManager::instance()->action(doc->selection->itemAt(0), is);
		}
		//view->SelItem.at(0)->ContourLine = view->SelItem.at(0)->PoLine.copy();
		//FIXME make an internal item copy poline to contourline member
		doc->selection->itemAt(0)->ContourLine = doc->selection->itemAt(0)->PoLine.copy();
		//view->SelItem.at(0)->ClipEdited = true;
		doc->selection->itemAt(0)->ClipEdited = true;
		view->updateContents();
	}
}

void NodePalette::MovePoint()
{
	if (doc->EditClipMode == 0)
	{
		FPoint np(XSpin->value()/doc->unitRatio(), YSpin->value()/doc->unitRatio());
		FPoint zp(doc->selection->itemAt(0)->xPos(), doc->selection->itemAt(0)->yPos());
		if (AbsMode->isChecked())
			np -= zp;
		view->MoveClipPoint(doc->selection->itemAt(0), np);
		view->AdjustItemSize(doc->selection->itemAt(0));
		emit DocChanged();
	}
}

void NodePalette::SetSym()
{
	view->MoveSym = true;
}

void NodePalette::SetAsym()
{
	view->MoveSym = false;
}

void NodePalette::SetXY(double x, double y)
{
	FPoint zp(0.0, 0.0);
	disconnect(XSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	if (AbsMode->isChecked())
		//zp = FPoint(view->SelItem.at(0)->xPos(), view->SelItem.at(0)->yPos());
		zp = FPoint(doc->selection->itemAt(0)->xPos(), doc->selection->itemAt(0)->yPos());
	XSpin->setValue((x + zp.x())*doc->unitRatio());
	YSpin->setValue((y + zp.y())*doc->unitRatio());
	connect(XSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
}

void NodePalette::ToggleAbsMode()
{
	//FPoint zp = FPoint(view->SelItem.at(0)->xPos(), view->SelItem.at(0)->yPos());
	FPoint zp(doc->selection->itemAt(0)->xPos(), doc->selection->itemAt(0)->yPos());
	disconnect(XSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	double unitRatio=doc->unitRatio();
	FPoint np(XSpin->value()/unitRatio, YSpin->value()/unitRatio);
	if (AbsMode->isChecked())
		np += zp;
	else
		np -= zp;
	XSpin->setValue(np.x()*unitRatio);
	YSpin->setValue(np.y()*unitRatio);
	connect(XSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
}

void NodePalette::ToggleConMode()
{
	disconnect(XSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	if (doc != 0)
	{
		view->EditContour = EditCont->isChecked();
		view->updateContents();
		if (EditCont->isChecked())
		{
			BezierClose->setEnabled(false);
			PolySplit->setEnabled(false);
			ResetCont->setEnabled(true);
			XSpin->setMinValue(-3000);
			YSpin->setMinValue(-3000);
		}
		else
		{
			BezierClose->setEnabled(false);
			PolySplit->setEnabled(true);
			ResetCont->setEnabled(false);
			XSpin->setMinValue(0);
			YSpin->setMinValue(0);
		}
	}
	connect(XSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
}

void NodePalette::HaveNode(bool have, bool mov)
{
	bool setter = have ? true : false;
	XSpin->setEnabled(setter);
	YSpin->setEnabled(setter);
	if (setter == true)
	{
		if (view->EdPoints)
			ResNode->setEnabled(setter);
		else
			Res1Node->setEnabled(setter);
	}
	else
	{
		ResNode->setEnabled(setter);
		Res1Node->setEnabled(setter);
	}
	if (doc->EditClipMode == 0)
	{
		disconnect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
		disconnect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
		if (mov)
			SymMove->setOn(true);
		else
			AsymMove->setOn(true);
		connect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
		connect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
	}
	if (doc->EditClipMode == 2)
	{
		uint cc;
		bool leaveEd = false;
		PageItem*currItem=doc->selection->itemAt(0);
		if (view->EditContour)
			//cc = view->SelItem.at(0)->ContourLine.size();
			cc = currItem->ContourLine.size();
		else
			//cc = view->SelItem.at(0)->PoLine.size();
			cc = currItem->PoLine.size();
		//if (view->SelItem.at(0)->asPolyLine())
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
	doc->EditClipMode = 0;
	view->EdPoints = false;
	//PageItem *currItem = view->SelItem.at(0);
	PageItem *currItem = doc->selection->itemAt(0);
	if (view->EditContour)
		view->MarkClip(currItem, currItem->ContourLine, true);
	else
		view->MarkClip(currItem, currItem->PoLine, true);
	SymMove->setEnabled(true);
	AsymMove->setEnabled(true);
	ResNode->setEnabled(false);
	Res1Node->setEnabled(false);
}

void NodePalette::MoveN()
{
	doc->EditClipMode = 0;
	view->EdPoints = true;
	//PageItem *currItem = view->SelItem.at(0);
	PageItem *currItem = doc->selection->itemAt(0);
	if (view->EditContour)
		view->MarkClip(currItem, currItem->ContourLine, true);
	else
		view->MarkClip(currItem, currItem->PoLine, true);
	MoveNode->setOn(true);
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	ResNode->setEnabled(false);
	Res1Node->setEnabled(false);
}

void NodePalette::AddN()
{
	doc->EditClipMode = 1;
	view->EdPoints = true;
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	ResNode->setEnabled(false);
	Res1Node->setEnabled(false);
}

void NodePalette::DelN()
{
	doc->EditClipMode = 2;
	view->EdPoints = true;
	SymMove->setEnabled(false);
	AsymMove->setEnabled(false);
	ResNode->setEnabled(false);
	Res1Node->setEnabled(false);
}

void NodePalette::closeEvent(QCloseEvent *ce)
{
	if (doc != 0)
	{
		MoveN();
		view->ClRe = -1;
		view->ClRe2 = -1;
		view->SegP1 = -1;
		view->SegP2 = -1;
		view->SelNode.clear();
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
		view->ClRe = -1;
		view->ClRe2 = -1;
		view->SegP1 = -1;
		view->SegP2 = -1;
		view->SelNode.clear();
		EditCont->setChecked(false);
		ToggleConMode();
	}
	PolySplit->setEnabled( false );
	BezierClose->setEnabled( false );
	EditCont->setChecked(false);
	emit Schliessen();
}

void NodePalette::languageChange()
{
	setCaption( tr( "Nodes" ) );
	ScaleVal->setSuffix( tr(" %"));
	
	AbsMode->setText( tr("&Absolute Coordinates"));
	TextLabel1->setText( tr("&X-Pos:"));
	TextLabel2->setText( tr("&Y-Pos:"));
	EditCont->setText( tr("Edit &Contour Line"));
	ResetCont->setText( tr("&Reset Contour Line"));
	PushButton1->setText( tr("&End Editing"));
	QToolTip::add(MoveNode, tr("Move Nodes"));
	QToolTip::add(MoveControl, tr("Move Control Points"));
	QToolTip::add(AddNode, tr("Add Nodes"));
	QToolTip::add(DeleteNode, tr("Delete Nodes"));
	QToolTip::add(AsymMove, tr("Move Control Points Independently"));
	QToolTip::add(SymMove, tr("Move Control Points Symmetrical"));
	QToolTip::add(ResNode, tr("Reset Control Points"));
	QToolTip::add(Res1Node, tr("Reset this Control Point"));
	QToolTip::add(PolySplit, tr("Open a Polygon or Cuts a Bezier Curve"));
	QToolTip::add(BezierClose, tr("Close this Bezier Curve"));
	QToolTip::add(PolyMirrorH, tr("Mirror the Path Horizontally"));
	QToolTip::add(PolyMirrorV, tr("Mirror the Path Vertically"));
	QToolTip::add(PolyShearR, tr("Shear the Path Horizontally to the Right"));
	QToolTip::add(PolyShearL, tr("Shear the Path Horizontally to the Left"));
	QToolTip::add(PolyShearU, tr("Shear the Path Vertically Up"));
	QToolTip::add(PolyShearD, tr("Shear the Path Vertically Down"));
	QToolTip::add(RotateCCW, tr("Rotate the Path Counter-Clockwise"));
	QToolTip::add(RotateCW, tr("Rotate the Path Clockwise"));
	QToolTip::add(Crop, tr("Reduce the Size of the Path by shown %"));
	QToolTip::add(Expand, tr("Enlarge the Size of the Path by shown %"));
	QToolTip::add(RotVal, tr("Angle of Rotation"));
	QToolTip::add(ScaleVal, tr("% to Enlarge or Reduce By"));
	QToolTip::add(EditCont, tr("Activate Contour Line Editing Mode"));
	QToolTip::add(ResetCont, tr("Reset the Contour Line to the Original Shape of the Frame"));
	QToolTip::add(AbsMode,  "<qt>" + tr("When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.") + "</qt>");
}

