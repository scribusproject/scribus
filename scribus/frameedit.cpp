#include "frameedit.h"
#include "frameedit.moc"
#include "page.h"

extern QPixmap loadIcon(QString nam);
extern float UmReFaktor;

NodePalette::NodePalette( QWidget* parent)
    : QDialog( parent, "Npal", false, WStyle_Customize | WStyle_DialogBorder)
{
//    resize( 155, 165 );
    setCaption( tr( "Nodes" ) );
  	setIcon(loadIcon("AppIcon.xpm"));
    NodePaletteLayout = new QVBoxLayout( this ); 
    NodePaletteLayout->setSpacing( 2 );
    NodePaletteLayout->setMargin( 5 );

    ButtonGroup1 = new QButtonGroup( this, "ButtonGroup1" );
    ButtonGroup1->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup1->setFrameShadow( QButtonGroup::Plain );
    ButtonGroup1->setTitle( tr( "" ) );
    ButtonGroup1->setExclusive( true );
    ButtonGroup1->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1->layout()->setSpacing( 0 );
    ButtonGroup1->layout()->setMargin( 0 );
    ButtonGroup1Layout = new QHBoxLayout( ButtonGroup1->layout() );
    ButtonGroup1Layout->setAlignment( Qt::AlignTop );
    ButtonGroup1Layout->setSpacing( 2 );
    ButtonGroup1Layout->setMargin( 0 );

    MoveNode = new QToolButton( ButtonGroup1, "MoveNode" );
    MoveNode->setText( tr( "" ) );
    MoveNode->setPixmap(loadIcon("MoveNode.png"));
    MoveNode->setToggleButton( true );
		MoveNode->setOn(true);
    ButtonGroup1Layout->addWidget( MoveNode );
    MoveControl = new QToolButton( ButtonGroup1, "MoveControl" );
    MoveControl->setText( tr( "" ) );
    MoveControl->setPixmap(loadIcon("MoveKontrol.png"));
    MoveControl->setToggleButton( true );
    ButtonGroup1Layout->addWidget( MoveControl );
    AddNode = new QToolButton( ButtonGroup1, "AddNode" );
    AddNode->setText( tr( "" ) );
    AddNode->setPixmap(loadIcon("AddNode.png"));
    AddNode->setToggleButton( true );
    ButtonGroup1Layout->addWidget( AddNode );
    DeleteNode = new QToolButton( ButtonGroup1, "DeleteNode" );
    DeleteNode->setText( tr( "" ) );
    DeleteNode->setPixmap(loadIcon("DeleteNode.png"));
    DeleteNode->setToggleButton( true );
    ButtonGroup1Layout->addWidget( DeleteNode );
    NodePaletteLayout->addWidget( ButtonGroup1 );

    ButtonGroup2 = new QButtonGroup( this, "ButtonGroup2" );
    ButtonGroup2->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup2->setFrameShadow( QButtonGroup::Plain );
    ButtonGroup2->setTitle( tr( "" ) );
    ButtonGroup2->setExclusive( true );
    ButtonGroup2->setColumnLayout(0, Qt::Vertical );
    ButtonGroup2->layout()->setSpacing( 2 );
    ButtonGroup2->layout()->setMargin( 0 );
    ButtonGroup2Layout = new QHBoxLayout( ButtonGroup2->layout() );
    ButtonGroup2Layout->setAlignment( Qt::AlignTop );
    AsymMove = new QToolButton( ButtonGroup2, "AsymMove" );
    AsymMove->setEnabled( false );
    AsymMove->setText( tr( "" ) );
    AsymMove->setPixmap(loadIcon("MoveAsym.png"));
    AsymMove->setToggleButton( true );
    ButtonGroup2Layout->addWidget( AsymMove );
    SymMove = new QToolButton( ButtonGroup2, "SymMove" );
    SymMove->setEnabled( false );
    SymMove->setText( tr( "" ) );
    SymMove->setPixmap(loadIcon("MoveSym.png"));
    SymMove->setToggleButton( true );
    ButtonGroup2Layout->addWidget( SymMove );
    ResNode = new QToolButton( ButtonGroup2, "resnode" );
    ResNode->setEnabled( false );
    ResNode->setText( tr( "" ) );
    ResNode->setPixmap(loadIcon("ResetNode.png"));
    ButtonGroup2Layout->addWidget( ResNode );
    Res1Node = new QToolButton( ButtonGroup2, "res1node" );
    Res1Node->setEnabled( false );
    Res1Node->setText( tr( "" ) );
    Res1Node->setPixmap(loadIcon("Reset1Node.png"));
    ButtonGroup2Layout->addWidget( Res1Node );
    NodePaletteLayout->addWidget( ButtonGroup2 );

    ButtonGroup3 = new QButtonGroup( this, "ButtonGroup3" );
    ButtonGroup3->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup3->setFrameShadow( QButtonGroup::Plain );
    ButtonGroup3->setTitle( tr( "" ) );
    ButtonGroup3->setExclusive( true );
    ButtonGroup3->setColumnLayout(0, Qt::Vertical );
    ButtonGroup3->layout()->setSpacing( 2 );
    ButtonGroup3->layout()->setMargin( 0 );
    ButtonGroup3Layout = new QHBoxLayout( ButtonGroup3->layout() );
    ButtonGroup3Layout->setAlignment( Qt::AlignTop );
    PolySplit = new QToolButton( ButtonGroup3, "PolySplit" );
    PolySplit->setEnabled( false );
    PolySplit->setText( tr( "" ) );
    PolySplit->setToggleButton( true );
    PolySplit->setPixmap(loadIcon("PolyCut.png"));
    ButtonGroup3Layout->addWidget( PolySplit );
    BezierClose = new QToolButton( ButtonGroup3, "BezierClose" );
    BezierClose->setEnabled( false );
    BezierClose->setText( tr( "" ) );
    BezierClose->setPixmap(loadIcon("BezierClose.png"));
    ButtonGroup3Layout->addWidget( BezierClose );

    PolyMirrorH = new QToolButton( ButtonGroup3, "MirrorH" );
    PolyMirrorH->setText( tr( "" ) );
    PolyMirrorH->setPixmap(loadIcon("hmirror.png"));
    ButtonGroup3Layout->addWidget( PolyMirrorH );

    PolyMirrorV = new QToolButton( ButtonGroup3, "MirrorV" );
    PolyMirrorV->setText( tr( "" ) );
    PolyMirrorV->setPixmap(loadIcon("vmirror.png"));
    ButtonGroup3Layout->addWidget( PolyMirrorV );

    NodePaletteLayout->addWidget( ButtonGroup3 );

    ButtonGroup4 = new QButtonGroup( this, "ButtonGroup4" );
    ButtonGroup4->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup4->setFrameShadow( QButtonGroup::Plain );
    ButtonGroup4->setTitle( tr( "" ) );
    ButtonGroup4->setExclusive( true );
    ButtonGroup4->setColumnLayout(0, Qt::Vertical );
    ButtonGroup4->layout()->setSpacing( 2 );
    ButtonGroup4->layout()->setMargin( 0 );
    ButtonGroup4Layout = new QHBoxLayout( ButtonGroup4->layout() );
    ButtonGroup4Layout->setAlignment( Qt::AlignTop );

    PolyShearL = new QToolButton( ButtonGroup4, "ShearL" );
		PolyShearL->setAutoRepeat(true);
    PolyShearL->setText( tr( "" ) );
    PolyShearL->setPixmap(loadIcon("shear_left.png"));
    ButtonGroup4Layout->addWidget( PolyShearL );
    PolyShearR = new QToolButton( ButtonGroup4, "ShearR" );
		PolyShearR->setAutoRepeat(true);
    PolyShearR->setText( tr( "" ) );
    PolyShearR->setPixmap(loadIcon("shear_right.png"));
    ButtonGroup4Layout->addWidget( PolyShearR );
    PolyShearU = new QToolButton( ButtonGroup4, "ShearU" );
		PolyShearU->setAutoRepeat(true);
    PolyShearU->setText( tr( "" ) );
    PolyShearU->setPixmap(loadIcon("shear_up.png"));
    ButtonGroup4Layout->addWidget( PolyShearU );
    PolyShearD = new QToolButton( ButtonGroup4, "ShearD" );
		PolyShearD->setAutoRepeat(true);
    PolyShearD->setText( tr( "" ) );
    PolyShearD->setPixmap(loadIcon("shear_down.png"));
    ButtonGroup4Layout->addWidget( PolyShearD );

/*    QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ButtonGroup4Layout->addItem( spacer_2 );  */
    NodePaletteLayout->addWidget( ButtonGroup4 );

    Layout2 = new QGridLayout( 0, 1, 1, 0, 5, "Layout2");
    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "X-Pos:" ) );
    Layout2->addWidget( TextLabel1, 0, 0 );
    YSpin = new MSpinBox( this, 2 );
		YSpin->setMaxValue(300000);
		YSpin->setEnabled(false);
    YSpin->setLineStep(100);
    Layout2->addWidget( YSpin, 1, 1 );
    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( tr( "Y-Pos:" ) );
    Layout2->addWidget( TextLabel2, 1, 0 );
    XSpin = new MSpinBox( this, 2 );
		XSpin->setMaxValue(300000);
		XSpin->setEnabled(false);
    XSpin->setLineStep(100);
    Layout2->addWidget( XSpin, 0, 1 );
    NodePaletteLayout->addLayout( Layout2 );

    PushButton1 = new QPushButton( this, "PushButton1" );
    PushButton1->setText( tr( "End Editing" ) );
    NodePaletteLayout->addWidget( PushButton1 );
    QToolTip::add(  MoveNode, tr( "Move Nodes" ) );
    QToolTip::add(  MoveControl, tr( "Move Control Points" ) );
    QToolTip::add(  AddNode, tr( "Add Nodes" ) );
    QToolTip::add(  DeleteNode, tr( "Delete Nodes" ) );
    QToolTip::add(  AsymMove, tr( "Move Control Points independently" ) );
    QToolTip::add(  SymMove, tr( "Move Control Points symmetrical" ) );
    QToolTip::add(  ResNode, tr( "Reset Control Points" ) );
    QToolTip::add(  Res1Node, tr( "Reset this Control Points" ) );
    QToolTip::add(  PolySplit, tr( "Opens a Polygon or cuts a Bezier Curve" ) );
    QToolTip::add(  BezierClose, tr( "Closes this Bezier Curve" ) );
    QToolTip::add(  PolyMirrorH, tr( "Mirrors the Path horizontal" ) );
    QToolTip::add(  PolyMirrorV, tr( "Mirrors the Path vertical" ) );
    QToolTip::add(  PolyShearR, tr( "Shears the Path horizotal to the right" ) );
    QToolTip::add(  PolyShearL, tr( "Shears the Path horizotal to the left" ) );
    QToolTip::add(  PolyShearU, tr( "Shears the Path vertical up" ) );
    QToolTip::add(  PolyShearD, tr( "Shears the Path vertical down" ) );

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
}

void NodePalette::setDoc(ScribusDoc *dc)
{
	doc = dc;
	switch (doc->Einheit)
		{
		case 0:
   		YSpin->setSuffix( tr( " pt" ) );
   		XSpin->setSuffix( tr( " pt" ) );
			break;
		case 1:
   		YSpin->setSuffix( tr( " mm" ) );
   		XSpin->setSuffix( tr( " mm" ) );
			break;
		case 2:
   		YSpin->setSuffix( tr( " in" ) );
   		XSpin->setSuffix( tr( " in" ) );
			break;
		case 3:
   		YSpin->setSuffix( tr( " p" ) );
   		XSpin->setSuffix( tr( " p" ) );
			break;
		}
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
}

void NodePalette::PolyStatus(int typ, uint size)
{
	switch (typ)
		{
		case 6:
			PolySplit->setEnabled(true);
			BezierClose->setEnabled(false);
			break;
		case 7:
			if (size > 7)
				{
				BezierClose->setEnabled(true);
				PolySplit->setEnabled(true);
				}
			else
				{
				BezierClose->setEnabled(false);
				PolySplit->setEnabled(false);
				}
			break;
		default:
			BezierClose->setEnabled(false);
			PolySplit->setEnabled(false);
			break;
		}
	if (doc->EditClipMode != 0)
		MoveN();
	MoveControl->setEnabled(true);
}

void NodePalette::CloseBezier()
{
	MoveN();
	doc->ActPage->Bezier2Poly();
	BezierClose->setEnabled(false);
	PolySplit->setEnabled(true);
}

void NodePalette::ShearR()
{
	if (doc != 0)
		doc->ActPage->ShearPolyHR();
}

void NodePalette::ShearL()
{
	if (doc != 0)
		doc->ActPage->ShearPolyHL();
}

void NodePalette::ShearU()
{
	if (doc != 0)
		doc->ActPage->ShearPolyVU();
}

void NodePalette::ShearD()
{
	if (doc != 0)
		doc->ActPage->ShearPolyVD();
}

void NodePalette::MirrorH()
{
	if (doc != 0)
		doc->ActPage->MirrorPolyH();
}

void NodePalette::MirrorV()
{
	if (doc != 0)
		doc->ActPage->MirrorPolyV();
}

void NodePalette::ResetControl()
{
	doc->ActPage->ResetControl();
}

void NodePalette::Reset1Control()
{
	doc->ActPage->Reset1Control();
}

void NodePalette::MovePoint()
{
	if (doc->EditClipMode == 0)
		doc->ActPage->MoveClipPoint(doc->ActPage->SelItem.at(0), FPoint(XSpin->value()/UmReFaktor/100.0, YSpin->value()/UmReFaktor/100.0));
}

void NodePalette::SetSym()
{
	doc->ActPage->MoveSym = true;
}

void NodePalette::SetAsym()
{
	doc->ActPage->MoveSym = false;
}

void NodePalette::SetXY(float x, float y)
{
	disconnect(XSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	disconnect(YSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	XSpin->setValue(qRound(x*UmReFaktor*100));
	YSpin->setValue(qRound(y*UmReFaktor*100));
	connect(XSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
	connect(YSpin, SIGNAL(valueChanged(int)), this, SLOT(MovePoint()));
}

void NodePalette::HaveNode(bool have, bool mov)
{
	if (have)
		{
		XSpin->setEnabled(true);
		YSpin->setEnabled(true);
		if (doc->ActPage->EdPoints)
  		ResNode->setEnabled(true);
		else
  		Res1Node->setEnabled(true);
		}
	else
		{
		XSpin->setEnabled(false);
		YSpin->setEnabled(false);
  	ResNode->setEnabled(false);
  	Res1Node->setEnabled(false);
		}
  disconnect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
  disconnect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
	if (mov)
		SymMove->setOn(true);
	else
		AsymMove->setOn(true);
  connect(AsymMove, SIGNAL(clicked()), this, SLOT(SetAsym()));
  connect(SymMove, SIGNAL(clicked()), this, SLOT(SetSym()));
}

void NodePalette::MoveK()
{
	doc->EditClipMode = 0;
	doc->ActPage->EdPoints = false;
	doc->ActPage->MarkClip(doc->ActPage->SelItem.at(0));
  SymMove->setEnabled(true);
  AsymMove->setEnabled(true);
  ResNode->setEnabled(false);
  Res1Node->setEnabled(false);
}

void NodePalette::MoveN()
{
	doc->EditClipMode = 0;
	doc->ActPage->EdPoints = true;
	doc->ActPage->MarkClip(doc->ActPage->SelItem.at(0));
	MoveNode->setOn(true);
  SymMove->setEnabled(false);
  AsymMove->setEnabled(false);
  ResNode->setEnabled(false);
  Res1Node->setEnabled(false);
}

void NodePalette::AddN()
{
	doc->EditClipMode = 1;
	doc->ActPage->EdPoints = true;
  SymMove->setEnabled(false);
  AsymMove->setEnabled(false);
  ResNode->setEnabled(false);
  Res1Node->setEnabled(false);
}

void NodePalette::DelN()
{
	doc->EditClipMode = 2;
	doc->ActPage->EdPoints = true;
  SymMove->setEnabled(false);
  AsymMove->setEnabled(false);
  ResNode->setEnabled(false);
  Res1Node->setEnabled(false);
}

void NodePalette::closeEvent(QCloseEvent *)
{
	if (doc != 0)
		{
		MoveN();
		doc->ActPage->ClRe = -1;
		}
  PolySplit->setEnabled( false );
  BezierClose->setEnabled( false );
	emit Schliessen();
}

void NodePalette::EndEdit()
{
	if (doc != 0)
		{
		MoveN();
		doc->ActPage->ClRe = -1;
		}
  PolySplit->setEnabled( false );
  BezierClose->setEnabled( false );
	emit Schliessen();
}
