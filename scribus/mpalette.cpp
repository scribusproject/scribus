#include "mpalette.h"
#include "mpalette.moc"
#include <cmath>
#include <qpoint.h>
#include <qwmatrix.h>
#include <qmessagebox.h>
#include "query.h"
#include "scribusview.h"
#include "autoform.h"
#include "tabmanager.h"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;
extern ProfilesL InputProfiles;

NameWidget::NameWidget(QWidget* parent) : QLineEdit(parent)
{
}

void NameWidget::focusOutEvent(QFocusEvent *)
{
	emit Leaved();
}

Mpalette::Mpalette( QWidget* parent, preV *Prefs) : QDialog( parent, "Mdouble", false, 0)
{
    setCaption( tr( "Properties" ) );
    HaveDoc = false;
    HaveItem = false;
		RoVal = 0;
		Umrech = UmReFaktor;
    setIcon( loadIcon("AppIcon.png") );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );

    MpalLayout = new QVBoxLayout( this, 5, 1, "MpalLayout");
    setOrientation(Qt::Vertical);
    QFont f(font());
    f.setPointSize(f.pointSize()-1);
    setFont(f);
    buttonGroup5 = new QButtonGroup( this, "buttonGroup5" );
    buttonGroup5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, buttonGroup5->sizePolicy().hasHeightForWidth() ) );
    buttonGroup5->setFrameShape( QButtonGroup::NoFrame );
    buttonGroup5->setExclusive( true );
    buttonGroup5->setColumnLayout(0, Qt::Vertical );
    buttonGroup5->setTitle( QString::null );
    buttonGroup5->layout()->setSpacing( 0 );
    buttonGroup5->layout()->setMargin( 0 );
    buttonGroup5Layout = new QGridLayout( buttonGroup5->layout() );
    buttonGroup5Layout->setAlignment( Qt::AlignTop );
    SGeom = new QToolButton( buttonGroup5, "toolButton24" );
    SGeom->setToggleButton( true );
    SGeom->setOn( true );
    SGeom->setFont(f);
    SGeom->setText( tr( "X, Y, Z" ) );
    buttonGroup5Layout->addWidget( SGeom, 0, 0 );
    SShape = new QToolButton( buttonGroup5, "toolButton25" );
    SShape->setToggleButton( true );
    SShape->setFont(f);
    SShape->setText( tr( "Shape" ) );
    buttonGroup5Layout->addWidget( SShape, 0, 1 );
    SText = new QToolButton( buttonGroup5, "toolButton26" );
    SText->setToggleButton( true );
    SText->setFont(f);
    SText->setText( tr( "Text" ) );
    buttonGroup5Layout->addWidget( SText, 1, 0 );
    SImage = new QToolButton( buttonGroup5, "toolButton27" );
    SImage->setToggleButton( true );
    SImage->setFont(f);
    SImage->setText( tr( "Image" ) );
    buttonGroup5Layout->addWidget( SImage, 1, 1 );
    SLine = new QToolButton( buttonGroup5, "toolButton28" );
    SLine->setToggleButton( true );
    SLine->setFont(f);
    SLine->setText( tr( "Line" ) );
    buttonGroup5Layout->addWidget( SLine, 2, 0 );
    SColor = new QToolButton( buttonGroup5, "toolButton29" );
    SColor->setToggleButton( true );
    SColor->setFont(f);
    SColor->setText( tr( "Colors" ) );
    buttonGroup5Layout->addWidget( SColor, 2, 1 );
    MpalLayout->addWidget( buttonGroup5 );
    TabStack = new QWidgetStack( this, "TabStack" );
    TabStack->setFrameShape( QWidgetStack::NoFrame );

    page = new QWidget( TabStack, "page" );
    pageLayout = new QVBoxLayout( page, 0, 5, "pageLayout");

    NameGroup = new QGroupBox( page, "NameGroup" );
    NameGroup->setTitle( tr( "Name" ) );
    NameGroup->setColumnLayout(0, Qt::Vertical );
    NameGroup->layout()->setSpacing( 4 );
    NameGroup->layout()->setMargin( 5 );
    NameGroupLayout = new QHBoxLayout( NameGroup->layout() );
    NameGroupLayout->setAlignment( Qt::AlignTop );
		NameEdit = new NameWidget(NameGroup);
    NameGroupLayout->addWidget( NameEdit );
    pageLayout->addWidget( NameGroup );

    GeoGroup = new QGroupBox( page, "GeoGroup" );
    GeoGroup->setTitle( tr( "Geometry" ) );
    GeoGroup->setColumnLayout(0, Qt::Vertical );
    GeoGroup->layout()->setSpacing( 4 );
    GeoGroup->layout()->setMargin( 5 );
    GeoGroupLayout = new QGridLayout( GeoGroup->layout() );
    GeoGroupLayout->setAlignment( Qt::AlignTop );
    Text1 = new QLabel( GeoGroup, "Text1" );
    Text1->setText( tr( "X-Pos:" ) );
    GeoGroupLayout->addWidget( Text1, 0, 0 );
    Text1a = new QLabel( GeoGroup, "Text1a" );
    Text1a->setText( tr( "Y-Pos:" ) );
    GeoGroupLayout->addWidget( Text1a, 1, 0 );
    Text2 = new QLabel( GeoGroup, "Text2" );
    Text2->setText( tr( "Width:" ) );
    GeoGroupLayout->addWidget( Text2, 2, 0 );
    Text2a = new QLabel( GeoGroup, "Text2a" );
    Text2a->setText( tr( "Height:" ) );
    GeoGroupLayout->addWidget( Text2a, 3, 0 );
    Xpos = new MSpinBox( GeoGroup, 2 );
    Xpos->setSuffix( tr( " pt" ) );
		Xpos->setMaxValue( 300000 );
		Xpos->setMinValue( -300000 );
    GeoGroupLayout->addWidget( Xpos, 0, 1 );
    Ypos = new MSpinBox( GeoGroup, 2 );
    Ypos->setSuffix( tr( " pt" ) );
		Ypos->setMaxValue( 300000 );
		Ypos->setMinValue( -300000 );
    GeoGroupLayout->addWidget( Ypos, 1, 1 );
    Width = new MSpinBox( GeoGroup, 2 );
    Width->setSuffix( tr( " pt" ) );
    GeoGroupLayout->addWidget( Width, 2, 1 );
    Height = new MSpinBox( GeoGroup, 2 );
    Height->setSuffix( tr( " pt" ) );
    GeoGroupLayout->addWidget( Height, 3, 1 );
    Text3 = new QLabel( GeoGroup, "Text3" );
    Text3->setText( tr( "Rotation:" ) );
    GeoGroupLayout->addWidget( Text3, 4, 0 );
    Rot = new MSpinBox( GeoGroup, 2);
    Rot->setSuffix(" °");
    Rot->setWrapping( true );
    GeoGroupLayout->addWidget( Rot, 4, 1 );
    Text3r = new QLabel( GeoGroup, "Text3r" );
    Text3r->setText( tr( "Basepoint:" ) );
    GeoGroupLayout->addWidget( Text3r, 5, 0 );
    RotationGroup = new QButtonGroup( GeoGroup, "RotationGroup" );
    RotationGroup->setTitle( "" );
    RotationGroup->setFrameShape( QButtonGroup::NoFrame );
    RotationGroup->setColumnLayout(0, Qt::Vertical );
    RotationGroup->layout()->setSpacing( 0 );
    RotationGroup->layout()->setMargin( 0 );
    RotationGroupLayout = new QHBoxLayout( RotationGroup->layout() );
    RotationGroupLayout->setAlignment( Qt::AlignTop );
    Layout12 = new QGridLayout( 0, 1, 1, 0, 0, "Layout12");
    TopLeft = new QRadioButton( RotationGroup, "TopLeft" );
    TopLeft->setText( tr( "" ) );
    TopLeft->setChecked( true );
    Layout12->addWidget( TopLeft, 0, 0, Qt::AlignCenter );
    Line1 = new QFrame( RotationGroup, "Line1" );
    Line1->setMinimumSize( QSize( 20, 4 ) );
    Line1->setMaximumSize( QSize( 20, 4 ) );
    Line1->setFrameShape( QFrame::HLine );
    Line1->setFrameShadow( QFrame::Plain );
    Line1->setLineWidth( 3 );
    Line1->setFrameShape( QFrame::HLine );
    Layout12->addWidget( Line1, 0, 1, Qt::AlignCenter );
    TopRight = new QRadioButton( RotationGroup, "TopRight" );
    TopRight->setText( tr( "" ) );
    Layout12->addWidget( TopRight, 0, 2, Qt::AlignCenter );
    Line2 = new QFrame( RotationGroup, "Line2" );
    Line2->setMinimumSize( QSize( 4, 20 ) );
    Line2->setMaximumSize( QSize( 4, 20 ) );
    Line2->setFrameShape( QFrame::VLine );
    Line2->setFrameShadow( QFrame::Plain );
    Line2->setLineWidth( 3 );
    Line2->setFrameShape( QFrame::VLine );
    Layout12->addWidget( Line2, 1, 0, Qt::AlignCenter );
    Center = new QRadioButton( RotationGroup, "Center" );
    Center->setText( tr( "" ) );
    Layout12->addWidget( Center, 1, 1, Qt::AlignCenter );
    Line4 = new QFrame( RotationGroup, "Line4" );
    Line4->setMinimumSize( QSize( 4, 20 ) );
    Line4->setMaximumSize( QSize( 4, 20 ) );
    Line4->setFrameShadow( QFrame::Plain );
    Line4->setLineWidth( 3 );
    Line4->setFrameShape( QFrame::VLine );
    Layout12->addWidget( Line4, 1, 2, Qt::AlignCenter );
    BottomLeft = new QRadioButton( RotationGroup, "BottomLeft" );
    BottomLeft->setText( tr( "" ) );
    Layout12->addWidget( BottomLeft, 2, 0, Qt::AlignCenter );
    Line5 = new QFrame( RotationGroup, "Line5" );
    Line5->setMinimumSize( QSize( 20, 4 ) );
    Line5->setMaximumSize( QSize( 20, 4 ) );
    Line5->setFrameShape( QFrame::HLine );
    Line5->setFrameShadow( QFrame::Plain );
    Line5->setLineWidth( 3 );
    Line5->setFrameShape( QFrame::HLine );
    Layout12->addWidget( Line5, 2, 1, Qt::AlignCenter );
    BottomRight = new QRadioButton( RotationGroup, "BottomRight" );
    BottomRight->setText( tr( "" ) );
    Layout12->addWidget( BottomRight, 2, 2, Qt::AlignCenter );
    RotationGroupLayout->addLayout( Layout12 );
    QSpacerItem* spacer12 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    RotationGroupLayout->addItem( spacer12 );
    GeoGroupLayout->addWidget( RotationGroup, 5, 1 );
    pageLayout->addWidget( GeoGroup );

    layout60 = new QHBoxLayout( 0, 0, 5, "layout60");
    QSpacerItem* spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60->addItem( spacer1 );

    LayerGroup = new QGroupBox( page, "LayerGroup" );
    LayerGroup->setTitle( tr( "Level" ) );
    LayerGroup->setColumnLayout(0, Qt::Vertical );
    LayerGroup->layout()->setSpacing( 4 );
    LayerGroup->layout()->setMargin( 5 );
    LayerGroupLayout = new QGridLayout( LayerGroup->layout() );
    LayerGroupLayout->setAlignment( Qt::AlignTop );
    Zup = new QToolButton( LayerGroup, "Zup" );
    Zup->setMaximumSize( QSize( 22, 22 ) );
    Zup->setPixmap(loadIcon("up.png"));
    LayerGroupLayout->addWidget( Zup, 0, 0 );
    ZDown = new QToolButton( LayerGroup, "ZDown" );
    ZDown->setMaximumSize( QSize( 22, 22 ) );
    ZDown->setPixmap(loadIcon("down.png"));
    LayerGroupLayout->addWidget( ZDown, 1, 0 );
    ZTop = new QToolButton( LayerGroup, "ZTop" );
    ZTop->setMaximumSize( QSize( 22, 22 ) );
    ZTop->setPixmap(loadIcon("top.png"));
    LayerGroupLayout->addWidget( ZTop, 0, 1 );
    ZBottom = new QToolButton( LayerGroup, "ZBottom" );
    ZBottom->setMaximumSize( QSize( 22, 22 ) );
    ZBottom->setPixmap(loadIcon("bottom.png"));
    LayerGroupLayout->addWidget( ZBottom, 1, 1 );

    layout60->addWidget( LayerGroup );
    QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60->addItem( spacer2 );

    Layout44 = new QVBoxLayout( 0, 0, 4, "Layout44");
    QSpacerItem* spacer3 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout44->addItem( spacer3 );
    FlipH = new QToolButton( page, "MirrorH" );
    FlipH->setPixmap(loadIcon("FlipH.xpm"));
    FlipH->setToggleButton( true );
    Layout44->addWidget( FlipH );
    FlipV = new QToolButton( page, "MirrorV" );
    FlipV->setPixmap(loadIcon("FlipV.xpm"));
    FlipV->setToggleButton( true );
    Layout44->addWidget( FlipV );
    QSpacerItem* spacer4 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout44->addItem( spacer4 );
    layout60->addLayout( Layout44 );

    Layout44a = new QVBoxLayout( 0, 0, 4, "Layout44");
    QSpacerItem* spacer3a = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout44a->addItem( spacer3a );
    Locked = new QToolButton( page, "Lock" );
    Locked->setToggleButton( true );
		QIconSet a = QIconSet();
		a.setPixmap(loadIcon("locked.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::On);
		a.setPixmap(loadIcon("unlock.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::Off);
		Locked->setIconSet(a);
    Layout44a->addWidget( Locked );
    NoPrint = new QToolButton( page, "NoPrint" );
    NoPrint->setToggleButton( true );
		QIconSet a2 = QIconSet();
		a2.setPixmap(loadIcon("NoPrint.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::On);
		a2.setPixmap(loadIcon("DateiPrint16.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::Off);
		NoPrint->setIconSet(a2);
    Layout44a->addWidget( NoPrint );
    QSpacerItem* spacer4a = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout44a->addItem( spacer4a );
    layout60->addLayout( Layout44a );


    QSpacerItem* spacer5 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60->addItem( spacer5 );
    pageLayout->addLayout( layout60 );

    Textflow = new QCheckBox( page, "Textflow" );
    Textflow->setText( tr( "Text flows around Box" ) );
    pageLayout->addWidget( Textflow );
    Textflow2 = new QCheckBox( page, "Textflow2" );
    Textflow2->setText( tr( "Use Bounding Box" ) );
    pageLayout->addWidget( Textflow2 );

    QSpacerItem* spacer13 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout->addItem( spacer13 );
    TabStack->addWidget( page, 0 );

    page_2 = new QWidget( TabStack, "page_2" );
    pageLayout_2 = new QVBoxLayout( page_2, 0, 5, "pageLayout_2");

    ShapeGroup = new QButtonGroup( page_2, "ShapeGroup" );
    ShapeGroup->setFrameShape( QButtonGroup::NoFrame );
    ShapeGroup->setTitle( tr( "" ) );
    ShapeGroup->setExclusive( true );
    ShapeGroup->setColumnLayout(0, Qt::Vertical );
    ShapeGroup->layout()->setSpacing( 2 );
    ShapeGroup->layout()->setMargin( 0 );
    ShapeGroupLayout = new QHBoxLayout( ShapeGroup->layout() );
    ShapeGroupLayout->setAlignment( Qt::AlignTop );
    SRect = new QLabel(ShapeGroup, "Text4a" );
    SRect->setText( tr( "Shape:" ) );
    ShapeGroupLayout->addWidget( SRect );
    SCustom = new Autoforms( ShapeGroup );
    ShapeGroupLayout->addWidget( SCustom );
//    QSpacerItem* spacer2L = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
 //   layoutLang->addItem( spacer2L );
    pageLayout_2->addWidget( ShapeGroup );

    EditShape = new QToolButton( page_2, "EditShape" );
    EditShape->setText( tr( "Edit Shape..." ) );
    pageLayout_2->addWidget( EditShape );

    Layout13 = new QHBoxLayout( 0, 0, 2, "Layout13");

    Text4 = new QLabel( page_2, "Text4" );
    Text4->setText( tr( "Round\nCorners:" ) );
    Layout13->addWidget( Text4 );

    RoundRect = new MSpinBox( page_2, 1 );
    RoundRect->setSuffix( tr( " pt" ) );
    Layout13->addWidget( RoundRect );
    pageLayout_2->addLayout( Layout13 );


    TabStack2 = new QWidgetStack( page_2, "TabStack2" );
    TabStack2->setFrameShape( QWidgetStack::NoFrame );

    page_2a = new QWidget( TabStack2, "page" );
    pageLayout_2a = new QVBoxLayout( page_2a, 0, 5, "pageLayout_2");
    Distance = new QGroupBox( page_2a, "Distance" );
    Distance->setTitle( tr( "Distance of Text" ) );
    Distance->setColumnLayout(0, Qt::Vertical );
    Distance->layout()->setSpacing( 2 );
    Distance->layout()->setMargin( 5 );
    DistanceLayout = new QGridLayout( Distance->layout() );
    DistanceLayout->setAlignment( Qt::AlignTop );
    Text14a = new QLabel( Distance, "Text14a" );
    Text14a->setText( tr( "Columns:" ) );
    DistanceLayout->addWidget( Text14a, 0, 0 );
    Text14b = new QLabel( Distance, "Text14b" );
    Text14b->setText( tr( "Gap:" ) );
    DistanceLayout->addWidget( Text14b, 1, 0 );
    Text14 = new QLabel( Distance, "Text14" );
    Text14->setText( tr( "Top:" ) );
    DistanceLayout->addWidget( Text14, 2, 0 );
    Text15 = new QLabel( Distance, "Text15" );
    Text15->setText( tr( "Bottom:" ) );
    DistanceLayout->addWidget( Text15, 3, 0 );
    Text16 = new QLabel( Distance, "Text16" );
    Text16->setText( tr( "Left:" ) );
    DistanceLayout->addWidget( Text16, 4, 0 );
    Text17 = new QLabel( Distance, "Text17" );
    Text17->setText( tr( "Right:" ) );
    DistanceLayout->addWidget( Text17, 5, 0 );
    DCol = new QSpinBox( Distance, "Cols" );
    DCol->setMaxValue( 3000 );
    DCol->setMinValue( 1 );
    DistanceLayout->addWidget( DCol, 0, 1 );
    DGap = new MSpinBox( Distance, 1 );
    DGap->setSuffix( tr( " pt" ) );
    DGap->setMaxValue( 3000 );
    DGap->setMinValue( 0 );
    DistanceLayout->addWidget( DGap, 1, 1 );
    DTop = new MSpinBox( Distance, 1 );
    DTop->setSuffix( tr( " pt" ) );
    DTop->setMaxValue( 3000 );
    DTop->setMinValue( 0 );
    DistanceLayout->addWidget( DTop, 2, 1 );
    DBottom = new MSpinBox( Distance, 1 );
    DBottom->setSuffix( tr( " pt" ) );
    DBottom->setMaxValue( 3000 );
    DBottom->setMinValue( 0 );
    DistanceLayout->addWidget( DBottom, 3, 1 );
    DLeft = new MSpinBox( Distance, 1 );
    DLeft->setSuffix( tr( " pt" ) );
    DLeft->setMaxValue( 3000 );
    DLeft->setMinValue( 0 );
    DistanceLayout->addWidget( DLeft, 4, 1 );
    DRight = new MSpinBox( Distance, 1 );
    DRight->setSuffix( tr( " pt" ) );
    DRight->setMaxValue( 3000 );
    DRight->setMinValue( 0 );
    DistanceLayout->addWidget( DRight, 5, 1 );
		TabsButton = new QPushButton( Distance, "Tabul" );
		TabsButton->setText( tr( "Tabulators..." ) );
		TabsButton->setDefault( false );
		TabsButton->setAutoDefault( false );
		DistanceLayout->addMultiCellWidget( TabsButton, 6, 6, 0, 1 );
		pageLayout_2a->addWidget(Distance);
    TabStack2->addWidget( page_2a, 0 );

    page_2b = new QWidget( TabStack2, "page" );
    pageLayout_2b = new QVBoxLayout( page_2b, 0, 5, "pageLayout_2");
    Distance2 = new QGroupBox( page_2b, "Distance" );
    Distance2->setTitle( tr( "Path Text Properites" ) );
    Distance2->setColumnLayout(0, Qt::Vertical );
    Distance2->layout()->setSpacing( 2 );
    Distance2->layout()->setMargin( 5 );
    DistanceLayout2 = new QGridLayout( Distance2->layout() );
    DistanceLayout2->setAlignment( Qt::AlignTop );
 		NormText2 = new QCheckBox( Distance2, "NormText" );
 		NormText2->setText( tr( "Show Curve" ) );
 		DistanceLayout2->addMultiCellWidget( NormText2, 0, 0, 0, 1 );
 		TextLabel3 = new QLabel( Distance2, "TextLabel3" );
 		TextLabel3->setText( tr( "Start Offset:" ) );
 		DistanceLayout2->addWidget( TextLabel3, 1, 0);
 		Dist = new MSpinBox( Distance2, 1 );
 		Dist->setSuffix( tr( " pt" ) );
 		Dist->setMaxValue( 3000 );
 		Dist->setMinValue( 0 );
 		Dist->setLineStep(10);
 		DistanceLayout2->addWidget( Dist, 1, 1);
 		TextLabel2 = new QLabel( Distance2, "TextLabel2" );
 		TextLabel2->setText( tr( "Distance from Curve:" ) );
 		DistanceLayout2->addWidget( TextLabel2, 2, 0);
 		LineW = new MSpinBox( Distance2, 1 );
 		LineW->setSuffix( tr( " pt" ) );
 		LineW->setMaxValue( 3000 );
 		LineW->setMinValue( -3000 );
		LineW->setLineStep(10);
 		DistanceLayout2->addWidget( LineW, 2, 1);
		pageLayout_2b->addWidget(Distance2);
    TabStack2->addWidget( page_2b, 1 );

    pageLayout_2->addWidget( TabStack2 );
    QSpacerItem* spacer6 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout_2->addItem( spacer6 );
    TabStack->addWidget( page_2, 1 );

    page_3 = new QWidget( TabStack, "page_3" );
    pageLayout_3 = new QVBoxLayout( page_3, 0, 5, "pageLayout_3");

    layout47 = new QHBoxLayout( 0, 0, 5, "layout47");

    layout46 = new QVBoxLayout( 0, 0, 5, "layout46");

    layout41 = new QGridLayout( 0, 1, 1, 0, 5, "layout41");

    Fonts = new FontCombo(page_3, Prefs);
		Fonts->setMaximumSize(190, 30);
    layout41->addMultiCellWidget( Fonts, 0, 0, 0, 1 );

    Text20 = new QLabel( page_3, "Text20" );
    Text20->setText( tr("Size:"));
    layout41->addWidget( Text20, 1, 0 );
    Size = new MSpinBox( page_3, 1 );
    Size->setPrefix( "" );
    Size->setSuffix( tr( " pt" ) );
    Size->setMaxValue( 3000 );
    Size->setMinValue( -3000 );
    layout41->addWidget( Size, 1, 1 );
    layout46->addLayout( layout41 );

    layout22 = new QHBoxLayout( 0, 0, 5, "layout22");
    ScaleTxt = new QLabel( page_3, "ScaleTxt" );
		ScaleTxt->setText( tr("Width:"));
    layout22->addWidget( ScaleTxt );
    ChScale = new QSpinBox( page_3, "ChScale" );
    ChScale->setMaxValue( 400 );
    ChScale->setMinValue( 25 );
    ChScale->setValue( 100 );
    ChScale->setSuffix( tr( " %" ) );
    layout22->addWidget( ChScale );
    layout46->addLayout( layout22 );

    layout23 = new QHBoxLayout( 0, 0, 5, "layout23");
    StrokeIcon = new QLabel( page_3, "StrokeIcon" );
    StrokeIcon->setText("");
    StrokeIcon->setPixmap(loadIcon("Stiftalt.xpm"));
    StrokeIcon->setScaledContents( FALSE );
    layout23->addWidget( StrokeIcon );
    TxStroke = new QComboBox( true, page_3, "TxStroke" );
		TxStroke->setEditable(false);
    layout23->addWidget( TxStroke );
  	TxStrokeSh = new QPopupMenu();
		TxStrokeSh->insertItem( tr("Other..."));
		TxStrokeSh->insertItem("0 %");
		TxStrokeSh->insertItem("10 %");
		TxStrokeSh->insertItem("20 %");
		TxStrokeSh->insertItem("30 %");
		TxStrokeSh->insertItem("40 %");
		TxStrokeSh->insertItem("50 %");
		TxStrokeSh->insertItem("60 %");
		TxStrokeSh->insertItem("70 %");
		TxStrokeSh->insertItem("80 %");
		TxStrokeSh->insertItem("90 %");
		TxStrokeSh->insertItem("100 %");
    QSpacerItem* spacer2S = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout23->addItem( spacer2S );
		PM1 = new QToolButton(page_3, "Colors");
		PM1->setBackgroundMode(PaletteBackground);
		PM1->setPopup(TxStrokeSh);
		PM1->setPopupDelay(1);
		PM1->setText("100 %");
		PM1->setAutoRaise(true);
    layout23->addWidget( PM1 );
    layout46->addLayout( layout23 );
    layout24 = new QHBoxLayout( 0, 0, 5, "layout24");
    FillIcon = new QLabel( page_3, "FillIcon" );
		FillIcon->setText("");
    FillIcon->setPixmap(loadIcon("fill.png"));
    FillIcon->setScaledContents( FALSE );
    layout24->addWidget( FillIcon );
    TxFill = new QComboBox( true, page_3, "TxFill" );
		TxFill->setEditable(false);
    layout24->addWidget( TxFill );
  	TxFillSh = new QPopupMenu();
		TxFillSh->insertItem( tr("Other..."));
		TxFillSh->insertItem("0 %");
		TxFillSh->insertItem("10 %");
		TxFillSh->insertItem("20 %");
		TxFillSh->insertItem("30 %");
		TxFillSh->insertItem("40 %");
		TxFillSh->insertItem("50 %");
		TxFillSh->insertItem("60 %");
		TxFillSh->insertItem("70 %");
		TxFillSh->insertItem("80 %");
		TxFillSh->insertItem("90 %");
		TxFillSh->insertItem("100 %");
    QSpacerItem* spacer2Sf = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout24->addItem( spacer2Sf );
		PM2 = new QToolButton(page_3, "Colors");
		PM2->setBackgroundMode(PaletteBackground);
		PM2->setPopup(TxFillSh);
		PM2->setPopupDelay(1);
		PM2->setText("100 %");
		PM2->setAutoRaise(true);
    layout24->addWidget( PM2 );
    layout46->addLayout( layout24 );

    Layout1 = new QHBoxLayout( 0, 0, 0, "Layout1");

    Underline = new QToolButton( page_3, "Underline" );
    Underline->setMaximumSize( QSize( 22, 22 ) );
    Underline->setText("");
    Underline->setPixmap(loadIcon("Unter.xpm"));
    Underline->setToggleButton( true );
    Layout1->addWidget( Underline );

    ButtonGroup1 = new QButtonGroup( page_3, "ButtonGroup1" );
    ButtonGroup1->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup1->setTitle("");
    ButtonGroup1->setExclusive( true );
    ButtonGroup1->setRadioButtonExclusive( false );
    ButtonGroup1->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1->layout()->setSpacing( 0 );
    ButtonGroup1->layout()->setMargin( 0 );
    ButtonGroup1Layout = new QHBoxLayout( ButtonGroup1->layout() );
    ButtonGroup1Layout->setAlignment( Qt::AlignTop );

    Subs = new QToolButton( ButtonGroup1, "Subs" );
    Subs->setMaximumSize( QSize( 22, 22 ) );
    Subs->setText("");
    Subs->setPixmap(loadIcon("Tief.xpm"));
    Subs->setToggleButton( true );
    ButtonGroup1Layout->addWidget( Subs );

    Supers = new QToolButton( ButtonGroup1, "Supers" );
    Supers->setMaximumSize( QSize( 22, 22 ) );
    Supers->setText("");
    Supers->setPixmap(loadIcon("Hoch.xpm"));
    Supers->setToggleButton( true );
    ButtonGroup1Layout->addWidget( Supers );
    Layout1->addWidget( ButtonGroup1 );

    Kapital = new QToolButton( page_3, "Kapital" );
    Kapital->setMaximumSize( QSize( 22, 22 ) );
    Kapital->setText("");
    Kapital->setPixmap(loadIcon("Kapital.xpm"));
    Kapital->setToggleButton( true );
    Layout1->addWidget( Kapital );

    Strike = new QToolButton( page_3, "Strike" );
    Strike->setMaximumSize( QSize( 22, 22 ) );
    Strike->setText("");
    Strike->setPixmap(loadIcon("Strike.xpm"));
    Strike->setToggleButton( true );
    Layout1->addWidget( Strike );

    Outlined = new QToolButton( page_3, "Outlined" );
    Outlined->setMaximumSize( QSize( 22, 22 ) );
    Outlined->setText("");
    Outlined->setPixmap(loadIcon("outlined.png"));
    Outlined->setToggleButton( true );
    Layout1->addWidget( Outlined );

    Revert = new QToolButton( page_3, "Bold" );
    Revert->setMaximumSize( QSize( 22, 22 ) );
    Revert->setText("");
    Revert->setPixmap(loadIcon("Revers.png"));
    Revert->setToggleButton( true );
    Layout1->addWidget( Revert );
    QSpacerItem* spacer7 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer7 );
    layout46->addLayout( Layout1 );
    layout47->addLayout( layout46 );
    pageLayout_3->addLayout( layout47 );

    GroupAlign = new QButtonGroup( page_3, "GroupAlign" );
    GroupAlign->setFrameShape( QButtonGroup::NoFrame );
    GroupAlign->setTitle("");
    GroupAlign->setExclusive( true );
    GroupAlign->setColumnLayout(0, Qt::Vertical );
    GroupAlign->layout()->setSpacing( 0 );
    GroupAlign->layout()->setMargin( 0 );
    GroupAlignLayout = new QGridLayout( GroupAlign->layout() );
    GroupAlignLayout->setAlignment( Qt::AlignTop );

    TextL = new QToolButton( GroupAlign, "TextL" );
    TextL->setMaximumSize( QSize( 22, 22 ) );
    TextL->setPixmap(loadIcon("text_left.png"));
    TextL->setToggleButton( true );
    TextL->setOn( true );
    GroupAlignLayout->addWidget( TextL, 0, 0 );

    TextR = new QToolButton( GroupAlign, "TextR" );
    TextR->setMaximumSize( QSize( 22, 22 ) );
    TextR->setPixmap(loadIcon("text_right.png"));
    TextR->setToggleButton( true );
    GroupAlignLayout->addWidget( TextR, 0, 1 );

    TextC = new QToolButton( GroupAlign, "TextC" );
    TextC->setMaximumSize( QSize( 22, 22 ) );
    TextC->setPixmap(loadIcon("text_center.png"));
    TextC->setToggleButton( true );
    GroupAlignLayout->addWidget( TextC, 0, 2 );

    TextB = new QToolButton( GroupAlign, "TextB" );
    TextB->setMaximumSize( QSize( 22, 22 ) );
    TextB->setPixmap(loadIcon("text_block.png"));
    TextB->setToggleButton( true );
    GroupAlignLayout->addWidget( TextB, 0, 3 );

    TextF = new QToolButton( GroupAlign, "TextF" );
    TextF->setMaximumSize( QSize( 22, 22 ) );
    TextF->setPixmap(loadIcon("text_force.png"));
    TextF->setToggleButton( true );
    GroupAlignLayout->addWidget( TextF, 0, 4 );
    pageLayout_3->addWidget( GroupAlign );

    GroupBox3 = new QGroupBox( page_3, "GroupBox3" );
    GroupBox3->setTitle( tr( "Custom Spacing" ) );
    GroupBox3->setColumnLayout(0, Qt::Vertical );
    GroupBox3->layout()->setSpacing( 2 );
    GroupBox3->layout()->setMargin( 5 );
    GroupBox3Layout = new QGridLayout( GroupBox3->layout() );
    GroupBox3Layout->setAlignment( Qt::AlignTop );

    LineSp = new MSpinBox( GroupBox3, 1 );
    LineSp->setSuffix( tr( " pt" ) );
    GroupBox3Layout->addWidget( LineSp, 1, 1 );

    Extra = new MSpinBox( GroupBox3, 1 );
    Extra->setSuffix( tr( " pt" ) );
    GroupBox3Layout->addWidget( Extra, 0, 1 );

    Text5 = new QLabel( GroupBox3, "Text5" );
    Text5->setText( tr( "Kerning:" ) );
    GroupBox3Layout->addWidget( Text5, 0, 0 );

    Text6 = new QLabel( GroupBox3, "Text6" );
    Text6->setText( tr( "Line Spacing:" ) );

    GroupBox3Layout->addWidget( Text6, 1, 0 );
    pageLayout_3->addWidget( GroupBox3 );

    GroupBox3a = new QGroupBox( page_3, "GroupBox3" );
    GroupBox3a->setTitle( tr( "Styles" ) );
    GroupBox3a->setColumnLayout(0, Qt::Vertical );
    GroupBox3a->layout()->setSpacing( 2 );
    GroupBox3a->layout()->setMargin( 5 );
    GroupBox3aLayout = new QVBoxLayout( GroupBox3a->layout() );
    GroupBox3aLayout->setAlignment( Qt::AlignTop );
		Spal = new Spalette(GroupBox3a);
    GroupBox3aLayout->addWidget( Spal );
    pageLayout_3->addWidget( GroupBox3a );

    layoutLang = new QHBoxLayout( 0, 0, 5, "layout24");
    SprachT = new QLabel( page_3, "Sprache" );
		SprachT->setText( tr("Lang.:"));
    layoutLang->addWidget( SprachT );
    LangCombo = new QComboBox( true, page_3, "Lang" );
		LangCombo->setEditable(false);
    layoutLang->addWidget( LangCombo );
		pageLayout_3->addLayout(layoutLang);

    QSpacerItem* spacer8 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout_3->addItem( spacer8 );
    TabStack->addWidget( page_3, 2 );

    page_4 = new QWidget( TabStack, "page_4" );
    pageLayout_4 = new QVBoxLayout( page_4, 0, 5, "pageLayout_4");

    FreeScale = new QRadioButton( page_4, "FreeScale" );
    FreeScale->setText( tr( "Free Scaling" ) );
    FreeScale->setChecked( true );
    pageLayout_4->addWidget( FreeScale );

    layout43 = new QGridLayout( 0, 1, 1, 0, 5, "layout43");
    Text11 = new QLabel( page_4, "Text11" );
    Text11->setText( tr( "Position:" ) );
    layout43->addWidget( Text11, 0, 0 );
    LXpos = new MSpinBox( page_4, 2 );
    LXpos->setSuffix( tr( " pt" ) );
    layout43->addWidget( LXpos, 0, 1 );
    LYpos = new MSpinBox( page_4, 2 );
    LYpos->setSuffix( tr( " pt" ) );
    layout43->addWidget( LYpos, 1, 1 );
    Text12 = new QLabel( page_4, "Text12" );
    Text12->setText( tr( "Horiz. Scale:" ) );
    layout43->addWidget( Text12, 2, 0 );
    ScaleX = new MSpinBox( page_4, 1 );
    ScaleX->setSuffix( tr( " %" ) );
    layout43->addWidget( ScaleX, 2, 1 );
    Text13 = new QLabel( page_4, "Text13" );
    Text13->setText( tr( "Vert. Scale:" ) );
    layout43->addWidget( Text13, 3, 0 );
    ScaleY = new MSpinBox( page_4, 1 );
    ScaleY->setSuffix( tr( " %" ) );
    layout43->addWidget( ScaleY, 3, 1 );
    Kette = new LinkButton( page_4 );
    Kette->setToggleButton( true );
    Kette->setAutoRaise( true );
    Kette->setMaximumSize( QSize( 15, 32767 ) );
    layout43->addMultiCellWidget( Kette, 2, 3, 2, 2 );
    pageLayout_4->addLayout( layout43 );

    Layout24 = new QVBoxLayout( 0, 0, 3, "Layout24");

    FrameScale = new QRadioButton( page_4, "FixedScale" );
    FrameScale->setText( tr( "Scale to Frame Size" ) );
    Layout24->addWidget( FrameScale );

    Layout18 = new QHBoxLayout( 0, 0, 6, "Layout18");

    Frame4 = new QFrame( page_4, "Frame4" );
    Frame4->setMinimumSize( QSize( 15, 2 ) );
    Frame4->setMaximumSize( QSize( 15, 10 ) );
    Frame4->setFrameShape( QFrame::NoFrame );
    Frame4->setFrameShadow( QFrame::Plain );
    Layout18->addWidget( Frame4 );

    Aspect = new QCheckBox( page_4, "Aspect" );
    /*<<<<<<< mpalette.cpp*/
    Aspect->setEnabled( false );
    Aspect->setText( tr( "Keep Aspect Ratio" ) );
    Aspect->setChecked( true );
    /*=======*/
    Aspect->setEnabled( FALSE );
    Aspect->setText( tr( "Proportional" ) );
    Aspect->setChecked( TRUE );
    /*>>>>>>> 1.8*/
    Layout18->addWidget( Aspect );
    Layout24->addLayout( Layout18 );
    pageLayout_4->addLayout( Layout24 );

    GroupBoxCM = new QGroupBox( page_4, "GroupBoxcm" );
    GroupBoxCM->setTitle("");
    GroupBoxCM->setColumnLayout(0, Qt::Vertical );
    GroupBoxCM->layout()->setSpacing( 2 );
    GroupBoxCM->layout()->setMargin( 5 );
    GroupBoxCM->setFrameShape( QFrame::NoFrame );
    GroupBoxCM->setFrameShadow( QFrame::Plain );
    GroupBoxCMLayout = new QVBoxLayout( GroupBoxCM->layout() );
    GroupBoxCMLayout->setAlignment( Qt::AlignTop );
 		TextCms1 = new QLabel( GroupBoxCM, "Text1" );
    TextCms1->setText( tr( "Input Profile:" ) );
		GroupBoxCMLayout->addWidget( TextCms1 );
    InputP = new QComboBox( true, GroupBoxCM, "InputP" );
    InputP->setEditable(false);
   	GroupBoxCMLayout->addWidget(InputP);
    TextCms2 = new QLabel( GroupBoxCM, "Text2" );
    TextCms2->setText( tr( "Rendering Intent:" ) );
		GroupBoxCMLayout->addWidget(TextCms2);
    MonitorI = new QComboBox( true, GroupBoxCM, "MonitorI" );
    MonitorI->insertItem( tr( "Perceptual" ) );
    MonitorI->insertItem( tr( "Relative Colorimetric" ) );
    MonitorI->insertItem( tr( "Saturation" ) );
    MonitorI->insertItem( tr( "Absolute Colorimetric" ) );
    MonitorI->setEditable(false);
   	GroupBoxCMLayout->addWidget(MonitorI);
   	pageLayout_4->addWidget(GroupBoxCM);

    QSpacerItem* spacer9 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout_4->addItem( spacer9 );
    TabStack->addWidget( page_4, 3 );

    page_5 = new QWidget( TabStack, "page_5" );
    pageLayout_5 = new QVBoxLayout( page_5, 0, 5, "pageLayout_5");

    Layout13_2 = new QHBoxLayout( 0, 0, 4, "Layout13_2");

    LineMode = new QComboBox( true, page_5, "LMode" );
		LineMode->setEditable(false);
    LineMode->insertItem( tr( "Left Point" ) );
    LineMode->insertItem( tr( "End Points" ) );
    Layout13_2->addWidget( LineMode );
		LMode = false;
    QSpacerItem* spacer10 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout13_2->addItem( spacer10 );
    pageLayout_5->addLayout( Layout13_2 );

    Layout12_2 = new QGridLayout( 0, 1, 1, 0, 3, "Layout12_2");
    Text8 = new QLabel( page_5, "Text8" );
    Text8->setText( tr( "Line Style:" ) );
    Layout12_2->addWidget( Text8, 0, 0 );
    LStyle = new LineCombo(page_5);
    Layout12_2->addWidget( LStyle, 1, 0 );
    TextLabel1 = new QLabel( page_5, "TextLabel1" );
    TextLabel1->setText( tr( "Line Width:" ) );
    Layout12_2->addWidget( TextLabel1, 2, 0 );
    LSize = new MSpinBox( page_5, 1 );
    LSize->setSuffix( tr( " pt" ) );
		LSize->setDecimals(10);
    Layout12_2->addWidget( LSize, 3, 0 );
    Text9 = new QLabel( page_5, "Text9" );
    Text9->setText( tr( "Edges:" ) );
    Layout12_2->addWidget( Text9, 4, 0 );
    LJoinStyle = new QComboBox( true, page_5, "LJoin" );
    LJoinStyle->setEditable(false);
    LJoinStyle->insertItem(loadIcon("MiterJoin.png"), tr( "Miter Join" ) );
    LJoinStyle->insertItem(loadIcon("BevelJoin.png"), tr( "Bevel Join" ) );
    LJoinStyle->insertItem(loadIcon("RoundJoin.png"), tr( "Round Join" ) );
    Layout12_2->addWidget( LJoinStyle, 5, 0 );
    Text10 = new QLabel( page_5, "Text10" );
    Text10->setText( tr( "Endings:" ) );
    Layout12_2->addWidget( Text10, 6, 0 );
    LEndStyle = new QComboBox( true, page_5, "LCap" );
    LEndStyle->setEditable(false);
    LEndStyle->insertItem(loadIcon("ButtCap.png"), tr( "Flat Cap" ) );
    LEndStyle->insertItem(loadIcon("SquareCap.png"), tr( "Square Cap" ) );
    LEndStyle->insertItem(loadIcon("RoundCap.png"), tr( "Round Cap" ) );
    Layout12_2->addWidget( LEndStyle, 7, 0 );
    pageLayout_5->addLayout( Layout12_2 );

		StyledLine = new QListBox(page_5, "StyledL");
		StyledLine->insertItem( tr("No Style"));
		pageLayout_5->addWidget(StyledLine);

    QSpacerItem* spacer11 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pageLayout_5->addItem( spacer11 );
    TabStack->addWidget( page_5, 4 );
    Cpal = new Cpalette(this);
    TabStack->addWidget(Cpal, 5);
    MpalLayout->addWidget( TabStack );
    QToolTip::add( Size, tr( "Font Size" ) );
    QToolTip::add( LineSp, tr( "Line Spacing" ) );
    QToolTip::add( Extra, tr( "Manual Kerning" ) );
    QToolTip::add( Revert, tr( "Reverse Writing" ) );
    QToolTip::add( Underline, tr( "Underline" ) );
    QToolTip::add( Kapital, tr( "Small Caps" ) );
    QToolTip::add( Subs, tr( "Subscript" ) );
    QToolTip::add( Supers, tr( "Superscript" ) );
    QToolTip::add( Strike, tr( "Strikeout" ) );
    QToolTip::add( Outlined, tr( "Outline Text" ) );
    QToolTip::add( TextL, tr( "Align Text Left" ) );
    QToolTip::add( TextR, tr( "Align Text Right" ) );
    QToolTip::add( TextB, tr( "Align Text Block" ) );
    QToolTip::add( TextC, tr( "Align Text Center" ) );
    QToolTip::add( FlipH, tr( "Flip horizontal" ) );
    QToolTip::add( FlipV, tr( "Flip vertical" ) );
    QToolTip::add( Zup, tr( "Move one Level up" ) );
    QToolTip::add( ZDown, tr( "Move one Level down" ) );
    QToolTip::add( ZTop, tr( "Move to Front" ) );
    QToolTip::add( ZBottom, tr( "Move to Back" ) );
    QToolTip::add( Locked, tr( "Locks or unlocks the Object" ) );
    QToolTip::add( NoPrint, tr( "Enables or disables printing of the Object" ) );;
    connect(Xpos, SIGNAL(valueChanged(int)), this, SLOT(NewX()));
    connect(Ypos, SIGNAL(valueChanged(int)), this, SLOT(NewY()));
    connect(Width, SIGNAL(valueChanged(int)), this, SLOT(NewW()));
    connect(Height, SIGNAL(valueChanged(int)), this, SLOT(NewH()));
    connect(Rot, SIGNAL(valueChanged(int)), this, SLOT(NewR()));
    connect(RoundRect, SIGNAL(valueChanged(int)), this, SLOT(NewRR()));
    connect(LineSp, SIGNAL(valueChanged(int)), this, SLOT(NewLsp()));
    connect(Size, SIGNAL(valueChanged(int)), this, SLOT(NewSize()));
    connect(Extra, SIGNAL(valueChanged(int)), this, SLOT(NewExtra()));
    connect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
    connect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
    connect(LXpos, SIGNAL(valueChanged(int)), this, SLOT(NewLocalXY()));
    connect(LYpos, SIGNAL(valueChanged(int)), this, SLOT(NewLocalXY()));
    connect(LSize, SIGNAL(valueChanged(int)), this, SLOT(NewLS()));
    connect(LStyle, SIGNAL(activated(int)), this, SLOT(NewLSty()));
    connect(LJoinStyle, SIGNAL(activated(int)), this, SLOT(NewLJoin()));
    connect(LEndStyle, SIGNAL(activated(int)), this, SLOT(NewLEnd()));
    connect(LineMode, SIGNAL(activated(int)), this, SLOT(NewLMode()));
    connect(Kette, SIGNAL(clicked()), this, SLOT(ToggleKette()));
    connect(FlipH, SIGNAL(clicked()), this, SLOT(DoFlipH()));
    connect(FlipV, SIGNAL(clicked()), this, SLOT(DoFlipV()));
    connect(GroupAlign, SIGNAL(clicked(int)), this, SLOT(NewAli()));
    connect(Kapital, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Revert, SIGNAL(clicked()), this, SLOT(DoRevert()));
    connect(Underline, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Subs, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Strike, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Supers, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Outlined, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(FreeScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
    connect(FrameScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
    connect(Aspect, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
    connect(Zup, SIGNAL(clicked()), this, SLOT(DoRaise()));
    connect(ZDown, SIGNAL(clicked()), this, SLOT(DoLower()));
    connect(ZTop, SIGNAL(clicked()), this, SLOT(DoFront()));
    connect(ZBottom, SIGNAL(clicked()), this, SLOT(DoBack()));
    connect(RotationGroup, SIGNAL(clicked(int)), this, SLOT(NewRotMode(int)));
    connect(Textflow, SIGNAL(clicked()), this, SLOT(DoFlow()));
    connect(Textflow2, SIGNAL(clicked()), this, SLOT(DoFlow2()));
    connect(SCustom, SIGNAL(FormSel(int, int, double *)), this, SLOT(MakeIrre(int, int, double *)));
    connect(EditShape, SIGNAL(clicked()), this, SLOT(EditSh()));
    connect(DGap, SIGNAL(valueChanged(int)), this, SLOT(NewGap()));
    connect(DCol, SIGNAL(valueChanged(int)), this, SLOT(NewCols()));
    connect(DTop, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
    connect(DLeft, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
    connect(DRight, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
    connect(DBottom, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
    connect(buttonGroup5, SIGNAL(clicked(int)), this, SLOT(SelTab(int)));
  	connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
  	connect(StyledLine, SIGNAL(selected(int)), this, SIGNAL(EditLSt()));
    connect(Fonts, SIGNAL(activated(int)), this, SLOT(NewTFont(int)));
    connect(TxFill, SIGNAL(activated(int)), this, SLOT(newTxtFill()));
    connect(TxStroke, SIGNAL(activated(int)), this, SLOT(newTxtStroke()));
  	connect(TxFillSh, SIGNAL(activated(int)), this, SLOT(setActShade(int)));
  	connect(TxStrokeSh, SIGNAL(activated(int)), this, SLOT(setActShade(int)));
    connect(ChScale, SIGNAL(valueChanged(int)), this, SLOT(NewTScale()));
    connect(Locked, SIGNAL(clicked()), this, SLOT(handleLock()));
    connect(NoPrint, SIGNAL(clicked()), this, SLOT(handlePrint()));
    connect(NormText2, SIGNAL(clicked()), this, SLOT(handlePathLine()));
    connect(Dist, SIGNAL(valueChanged(int)), this, SLOT(handlePathDist()));
    connect(LineW, SIGNAL(valueChanged(int)), this, SLOT(handlePathOffs()));
   	connect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChProf(const QString&)));
    connect(MonitorI, SIGNAL(activated(int)), this, SLOT(ChIntent()));
		connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
    connect(LangCombo, SIGNAL(activated(int)), this, SLOT(NewLanguage()));
    connect( TabsButton, SIGNAL( clicked() ), this, SLOT( ManageTabs() ) );
		HaveItem = false;
		Xpos->setValue(0);
		Ypos->setValue(0);
		Width->setValue(0);
		Height->setValue(0);
		Rot->setValue(0);
		RoundRect->setValue(0);
		TabStack2->raiseWidget(0);
		TabStack->raiseWidget(0);
		TabStack->widget(0)->setEnabled(false);
		SGeom->setEnabled(false);
		SShape->setEnabled(false);
		SText->setEnabled(false);
		SImage->setEnabled(false);
		SLine->setEnabled(false);
		SColor->setEnabled(false);
		StrokeIcon->setEnabled(false);
		TxStroke->setEnabled(false);
		PM1->setEnabled(false);
}


void Mpalette::closeEvent(QCloseEvent *ce)
{
	emit Schliessen();
	ce->accept();
}

void Mpalette::SelTab(int t)
{
	TabStack->raiseWidget(t);
	if (HaveItem)
		{
		Cpal->GradCombo->setCurrentItem(CurItem->GrType);
		Cpal->ChooseGrad(CurItem->GrType);
		Cpal->updateGeometry();
		Cpal->repaint();
		}
}

void Mpalette::SetDoc(ScribusDoc *d)
{
	doc = d;
	HaveDoc = true;
	Xpos->setMaxValue( 300000 );
	Xpos->setMinValue( -300000 );
	Ypos->setMaxValue( 300000 );
	Ypos->setMinValue( -300000 );
	Width->setMaxValue( 300000 );
	Width->setMinValue( 1 );
	Height->setMaxValue( 300000 );
	Height->setMinValue( 1 );
	Rot->setMaxValue( 36000 );
	Rot->setMinValue( 0 );
	RoundRect->setMaxValue( 30000 );
	RoundRect->setMinValue( -30000);
	LYpos->setMaxValue( 300000 );
	LYpos->setMinValue( -300000 );
	ScaleY->setMaxValue( 3000 );
	ScaleY->setMinValue( 1 );
	ScaleX->setMaxValue( 3000 );
	ScaleX->setMinValue( 1 );
	LXpos->setMaxValue( 300000 );
	LXpos->setMinValue( -300000 );
	Size->setMaxValue( 10240 );
	Size->setMinValue( 1 );
	LineSp->setMaxValue( 3000 );
	LineSp->setMinValue( 1 );
	Extra->setMaxValue( 3000 );
	Extra->setMinValue( -3000 );
	LSize->setMaxValue( 360 );
	LSize->setMinValue( 1 );
  Xpos->setDecimals(100);
  Ypos->setDecimals(100);
  Width->setDecimals(100);
  Height->setDecimals(100);
  Rot->setDecimals(100);
	RoundRect->setDecimals(10);
  Extra->setDecimals(10);
  Size->setDecimals(10);
  LineSp->setDecimals(10);
  LXpos->setDecimals(100);
  LYpos->setDecimals(100);
  ScaleX->setDecimals(10);
  ScaleY->setDecimals(10);
  DGap->setDecimals(10);
  DTop->setDecimals(10);
  DLeft->setDecimals(10);
  DBottom->setDecimals(10);
  DRight->setDecimals(10);
	Dist->setMaxValue( 3000 );
	Dist->setMinValue( 0 );
	Dist->setLineStep(10);
	LineW->setMaxValue( 3000 );
	LineW->setMinValue( -3000 );
	LineW->setLineStep(10);
	updateCList();
}

void Mpalette::UnsetDoc()
{
	HaveDoc = false;
}

void Mpalette::SetCurItem(PageItem *i)
{
 	disconnect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	disconnect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	HaveItem = false;
	CurItem = i;
  NameEdit->setText(i->AnName);
	RoundRect->setValue(qRound(i->RadRect*UmReFaktor*RoundRect->Decimals));
	DCol->setValue(i->Cols);
	DGap->setValue(qRound(i->ColGap*UmReFaktor*DGap->Decimals));
	DCol->setMaxValue(QMAX(qRound(i->Width / QMAX(i->ColGap, 10.0)), 1));
	DGap->setMaxValue(QMAX(qRound((i->Width / i->Cols - i->Extra - i->RExtra)*UmReFaktor*DGap->Decimals), 0));
	DCol->setMinValue(1);
	DGap->setMinValue(0);
  DLeft->setValue(qRound(i->Extra*UmReFaktor*DLeft->Decimals));
  DTop->setValue(qRound(i->TExtra*UmReFaktor*DTop->Decimals));
  DBottom->setValue(qRound(i->BExtra*UmReFaktor*DBottom->Decimals));
  DRight->setValue(qRound(i->RExtra*UmReFaktor*DRight->Decimals));
  Revert->setOn(i->Reverse);
	Textflow->setChecked(i->Textflow);
	Textflow2->setChecked(i->Textflow2);
	ToggleFlow();
	LangCombo->setCurrentText(i->Language);
	bool setter;
	if (i->NamedLStyle == "")
		{
			StyledLine->setCurrentItem(0);
			setter = true;
		}
	else
		{
			StyledLine->setSelected(StyledLine->findItem(i->NamedLStyle), true);
			setter = false;
		}
	LStyle->setEnabled(setter);
	LSize->setEnabled(setter);
	LJoinStyle->setEnabled(setter);
	LEndStyle->setEnabled(setter);
  connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	if (i->isPrintable)
		NoPrint->setOn(false);
	else
		NoPrint->setOn(true);
	if (i->Locked)
		setter = false;
	else
		setter = true;
	Xpos->setEnabled(setter);
	Ypos->setEnabled(setter);
	Width->setEnabled(setter);
	Height->setEnabled(setter);
	Rot->setEnabled(setter);
	RoundRect->setEnabled(setter);
	EditShape->setEnabled(setter);
	ShapeGroup->setEnabled(setter);
	LayerGroup->setEnabled(setter);
	Locked->setOn(!setter);
	if (i->Locked)
		{
		HaveItem = true;
		return;
		}
	if (i->PType == 8)
		{
		TabStack2->raiseWidget(1);
		NormText2->setChecked(i->PoShow);
 		LineW->setValue( qRound(i->BaseOffs * -10) );
 		Dist->setValue(static_cast<int>(i->Extra*10));
		}
	else
		TabStack2->raiseWidget(0);
	if (((i->PType == 4) || (i->PType == 2) || (i->PType == 3)) &&  (!i->ClipEdited))
		RoundRect->setEnabled(true);
	else
		{
		if ((i->PType == 6) && (i->FrameType == 0))
			RoundRect->setEnabled(true);
		else
			RoundRect->setEnabled(false);
		}
	if (i->PType == 5)
		{
		if (LMode)
			{
    	Text1->setText( tr( "X1:" ) );
    	Text2->setText( tr( "X2:" ) );
    	Text1a->setText( tr( "Y1:" ) );
    	Text2a->setText( tr( "Y2:" ) );
			Rot->setEnabled(false);
			Height->setEnabled(true);
			}
		else
			{
    	Text1->setText( tr( "X-Pos:" ) );
    	Text2->setText( tr( "Width:" ) );
    	Text1a->setText( tr( "Y-Pos:" ) );
    	Text2a->setText( tr( "Height:" ) );
			Rot->setEnabled(true);
			Height->setEnabled(false);
			}
		HaveItem = true;
		return;
		}
	else
		{
		if (i->PType == 2)
			{
			updateCmsList();
			if (i->ScaleType)
				setter = true;
			else
				setter = false;
			FreeScale->setChecked(setter);
			FrameScale->setChecked(!setter);
				if (setter == false)
					Kette->setOn(setter);
				Aspect->setEnabled(!setter);
				Aspect->setChecked(i->AspectRatio);
			}
		}
	HaveItem = true;
}

void Mpalette::NewSel(int nr)
{
	int visID;
	if (doc->ActPage->GroupSel)
		{
		RoVal = 0;
		double gx, gy, gh, gw;
		doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
    if (TopLeft->isChecked())
			doc->ActPage->RCenter = FPoint(gx, gy);
    if (TopRight->isChecked())
			doc->ActPage->RCenter = FPoint(gx + gw, gy);
    if (Center->isChecked())
			doc->ActPage->RCenter = FPoint(gx + gw / 2.0, gy + gh / 2.0);
    if (BottomLeft->isChecked())
			doc->ActPage->RCenter = FPoint(gx, gy + gh);
    if (BottomRight->isChecked())
			doc->ActPage->RCenter = FPoint(gx + gw, gy + gh);
    Text1->setText( tr( "X-Pos:" ) );
    Text2->setText( tr( "Width:" ) );
    Text1a->setText( tr( "Y-Pos:" ) );
    Text2a->setText( tr( "Height:" ) );
		HaveItem = false;
		Xpos->setValue(0);
		Ypos->setValue(0);
		Width->setValue(0);
		Height->setValue(0);
		Rot->setValue(0);
		RoundRect->setValue(0);
		HaveItem = true;
		Xpos->setEnabled(true);
		Ypos->setEnabled(true);
		Width->setEnabled(true);
		Height->setEnabled(true);
		Rot->setEnabled(true);
		SGeom->setEnabled(true);
		TabStack->widget(0)->setEnabled(true);
		NameEdit->setEnabled(false);
		}
	else
		{
		NameEdit->setEnabled(true);
		FlipH->setEnabled(false);
		FlipV->setEnabled(false);
		ShapeGroup->setEnabled(false);
		RoundRect->setEnabled(false);
		Distance->setEnabled(false);
		EditShape->setEnabled(false);
		LineMode->setEnabled(false);
		TopLeft->setEnabled(true);
		TopRight->setEnabled(true);
		BottomLeft->setEnabled(true);
		BottomRight->setEnabled(true);
		Center->setEnabled(true);
		TabStack->widget(0)->setEnabled(true);
		TabStack->widget(1)->setEnabled(false);
		TabStack->widget(2)->setEnabled(false);
		TabStack->widget(3)->setEnabled(false);
		TabStack->widget(4)->setEnabled(false);
		TabStack->widget(5)->setEnabled(true);
		SColor->setEnabled(true);
		visID = TabStack->id(TabStack->visibleWidget());
		switch (nr)
			{
			case -1:
    		Text1->setText( tr( "X-Pos:" ) );
    		Text2->setText( tr( "Width:" ) );
    		Text1a->setText( tr( "Y-Pos:" ) );
    		Text2a->setText( tr( "Height:" ) );
				Rot->setEnabled(true);
				Height->setEnabled(true);
				HaveItem = false;
				Xpos->setValue(0);
				Ypos->setValue(0);
				Width->setValue(0);
				Height->setValue(0);
				Rot->setValue(0);
				RoundRect->setValue(0);
				for (int ws = 0; ws < 6; ++ws)
					{
					TabStack->widget(ws)->setEnabled(false);
					}
				SGeom->setEnabled(false);
				SShape->setEnabled(false);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(false);
				SColor->setEnabled(false);
				SGeom->setOn(true);
				break;
			case 1:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				ShapeGroup->setEnabled(true);
				EditShape->setEnabled(true);
				if ((visID == 2) || (visID == 3))
					{
					TabStack->raiseWidget(0);
					SGeom->setOn(true);
					}
				HaveItem = true;
				break;
			case 2:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(true);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(3)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				FlipH->setEnabled(true);
				FlipV->setEnabled(true);
				ShapeGroup->setEnabled(true);
 				if ((doc->ActPage->SelItem.at(0)->FrameType == 0) || (doc->ActPage->SelItem.at(0)->FrameType == 2))
					RoundRect->setEnabled(true);
				EditShape->setEnabled(true);
				if (visID == 2)
					{
					TabStack->raiseWidget(0);
					SGeom->setOn(true);
					}
				HaveItem = true;
				break;
			case 3:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				ShapeGroup->setEnabled(true);
				RoundRect->setEnabled(true);
				EditShape->setEnabled(true);
				if ((visID == 2) || (visID == 3))
					{
					TabStack->raiseWidget(0);
					SGeom->setOn(true);
					}
				HaveItem = true;
				break;
			case 4:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(true);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(2)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				FlipH->setEnabled(true);
				FlipV->setEnabled(true);
				ShapeGroup->setEnabled(true);
 				if ((doc->ActPage->SelItem.at(0)->FrameType == 0) || (doc->ActPage->SelItem.at(0)->FrameType == 2))
					RoundRect->setEnabled(true);
				Distance->setEnabled(true);
				EditShape->setEnabled(true);
				if (visID == 3)
					{
					TabStack->raiseWidget(0);
					SGeom->setOn(true);
					}
				HaveItem = true;
				break;
			case 5:
				SGeom->setEnabled(true);
				SShape->setEnabled(false);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				LineMode->setEnabled(true);
				TopLeft->setEnabled(false);
				TopRight->setEnabled(false);
				BottomLeft->setEnabled(false);
				BottomRight->setEnabled(false);
				Center->setEnabled(false);
				if ((visID == 1) || (visID == 2) || (visID == 3))
					{
					TabStack->raiseWidget(0);
					SGeom->setOn(true);
					}
				HaveItem = true;
				break;
			case 6:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				ShapeGroup->setEnabled(true);
				EditShape->setEnabled(true);
 				if ((doc->ActPage->SelItem.at(0)->FrameType == 0) || (doc->ActPage->SelItem.at(0)->FrameType == 2))
					RoundRect->setEnabled(true);
				if ((visID == 2) || (visID == 3))
					{
					TabStack->raiseWidget(0);
					SGeom->setOn(true);
					}
				HaveItem = true;
				break;
			case 7:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				EditShape->setEnabled(true);
				if ((visID == 2) || (visID == 3))
					{
					TabStack->raiseWidget(0);
					SGeom->setOn(true);
					}
				HaveItem = true;
				break;
			case 8:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(true);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(2)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				EditShape->setEnabled(true);
				if (visID == 3)
					{
					TabStack->raiseWidget(0);
					SGeom->setOn(true);
					}
				HaveItem = true;
				break;
			}
		}
	updateGeometry();
	setFocus();
	repaint();
}

void Mpalette::UnitChange()
{
	double old = Umrech;
	bool tmp = HaveItem;
	HaveItem = false;
	QString ein;
	double oldX = static_cast<double>(Xpos->value()) / old / Xpos->Decimals;
	double oldY = static_cast<double>(Ypos->value()) / old / Ypos->Decimals;
	double oldW = static_cast<double>(Width->value()) / old / Width->Decimals;
	double oldH = static_cast<double>(Height->value()) / old / Height->Decimals;
	double oldLX = static_cast<double>(LXpos->value()) / old / LXpos->Decimals;
	double oldLY = static_cast<double>(LYpos->value()) / old / LYpos->Decimals;
	double oldG = static_cast<double>(DGap->value()) / old / DGap->Decimals;
	double oldGM = static_cast<double>(DGap->maxValue()) / old / DGap->Decimals;
	double oldDT = static_cast<double>(DTop->value()) / old / DTop->Decimals;
	double oldDL = static_cast<double>(DLeft->value()) / old / DLeft->Decimals;
	double oldDB = static_cast<double>(DBottom->value()) / old / DBottom->Decimals;
	double oldDR = static_cast<double>(DRight->value()) / old / DRight->Decimals;
	double oldRR = static_cast<double>(RoundRect->value()) / old / RoundRect->Decimals;
	double oldRM = static_cast<double>(RoundRect->maxValue()) / old / RoundRect->Decimals;
	Umrech = UmReFaktor;
  Width->setLineStep(100);
  Height->setLineStep(100);
  Xpos->setDecimals(100);
  Ypos->setDecimals(100);
  Width->setDecimals(100);
  Height->setDecimals(100);
  LXpos->setDecimals(100);
  LYpos->setDecimals(100);
  DGap->setDecimals(10);
  DLeft->setDecimals(10);
  DTop->setDecimals(10);
  DBottom->setDecimals(10);
  DRight->setDecimals(10);
	RoundRect->setDecimals(10);
	switch (doc->Einheit)
		{
		case 0:
			ein = " pt";
			break;
		case 1:
    	ein = " mm";
			break;
		case 2:
    	ein = " in";
  		Width->setLineStep(10);
  		Height->setLineStep(10);
			Xpos->setDecimals(10000);
			Ypos->setDecimals(10000);
  		Width->setDecimals(10000);
  		Height->setDecimals(10000);
  		LXpos->setDecimals(10000);
  		LYpos->setDecimals(10000);
  		DGap->setDecimals(10000);
  		DLeft->setDecimals(10000);
  		DTop->setDecimals(10000);
  		DBottom->setDecimals(10000);
  		DRight->setDecimals(10000);
			RoundRect->setDecimals(10000);
			break;
		case 3:
    	ein = " p";
			break;
		}
	Xpos->setSuffix( ein );
	Ypos->setSuffix( ein );
	Width->setSuffix( ein );
	Height->setSuffix( ein );
	LXpos->setSuffix( ein );
	LYpos->setSuffix( ein );
	DGap->setSuffix( ein );
	DLeft->setSuffix( ein );
	DTop->setSuffix( ein );
	DBottom->setSuffix( ein );
	DRight->setSuffix( ein );
	RoundRect->setSuffix( ein );
	Xpos->setValue(qRound(oldX * UmReFaktor * Xpos->Decimals));
	Ypos->setValue(qRound(oldY * UmReFaktor * Ypos->Decimals));
	Width->setValue(qRound(oldW * UmReFaktor * Width->Decimals));
	Height->setValue(qRound(oldH * UmReFaktor * Height->Decimals));
	LXpos->setValue(qRound(oldLX * UmReFaktor * LXpos->Decimals));
	LYpos->setValue(qRound(oldLY * UmReFaktor * LYpos->Decimals));
	DGap->setMaxValue(qRound(oldGM * UmReFaktor * DGap->Decimals));
	DGap->setValue(qRound(oldG * UmReFaktor * DGap->Decimals));
	DLeft->setValue(qRound(oldDL * UmReFaktor * DLeft->Decimals));
	DTop->setValue(qRound(oldDT * UmReFaktor * DTop->Decimals));
	DBottom->setValue(qRound(oldDB * UmReFaktor * DBottom->Decimals));
	DRight->setValue(qRound(oldDR * UmReFaktor * DRight->Decimals));
	RoundRect->setMaxValue(qRound(oldRM * UmReFaktor * RoundRect->Decimals));
	RoundRect->setValue(qRound(oldRR * UmReFaktor * RoundRect->Decimals));
	HaveItem = tmp;
}

void Mpalette::setXY(double x, double y)
{
	bool tmp = HaveItem;
	double inX, inY, b, h, r, dummy1, dummy2;
	QWMatrix ma;
	FPoint n;
	if (HaveItem)
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->getGroupRect(&dummy1, &dummy2, &b, &h);
			r = 0.0;
			}
		else
			{
			b = CurItem->Width;
			h = CurItem->Height;
			r = CurItem->Rot;
			}
		}
	else
		{
		b = 0.0;
		h = 0.0;
		r = 0.0;
		}
	HaveItem = false;
	ma.translate(x, y);
	ma.rotate(r);
  if (TopLeft->isChecked())
		n = FPoint(0.0, 0.0);
	if (TopRight->isChecked())
		n = FPoint(b, 0.0);
  if (Center->isChecked())
		n = FPoint(b / 2.0, h / 2.0);
	if (BottomLeft->isChecked())
		n = FPoint(0.0, h);
	if (BottomRight->isChecked())
		n = FPoint(b, h);
	inX = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
	inY = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
	Xpos->setValue(qRound(inX*UmReFaktor*Xpos->Decimals));
	Ypos->setValue(qRound(inY*UmReFaktor*Ypos->Decimals));
	if ((LMode) && (tmp))
		setBH(CurItem->Width, CurItem->Height);
	HaveItem = tmp;
}

void Mpalette::setBH(double x, double y)
{
	bool tmp = HaveItem;
	HaveItem = false;
	QWMatrix ma;
	QPoint dp;
	if ((LMode) && (CurItem->PType == 5))
		{
		ma.translate(static_cast<double>(Xpos->value()) / UmReFaktor / Xpos->Decimals, static_cast<double>(Ypos->value()) / UmReFaktor / Ypos->Decimals);
		ma.rotate(static_cast<double>(Rot->value())/100.0*(-1));
		dp = ma * QPoint(static_cast<int>(x), static_cast<int>(y));
		Width->setValue(qRound(dp.x()*UmReFaktor*Width->Decimals));
		Height->setValue(qRound(dp.y()*UmReFaktor*Height->Decimals));
		}
	else
		{
		RoundRect->setMaxValue(qRound(QMIN(x, y)/2*UmReFaktor*RoundRect->Decimals));
		Width->setValue(qRound(x*UmReFaktor*Width->Decimals));
		Height->setValue(qRound(y*UmReFaktor*Height->Decimals));
		}
	HaveItem = tmp;
}

void Mpalette::setR(double r)
{
	bool tmp = HaveItem;
	double rr = r;
	if (r > 0)
		rr = 360 - rr;
	HaveItem = false;
	Rot->setValue(qRound(fabs(rr)*100));
	HaveItem = tmp;
}

void Mpalette::setRR(double r)
{
	bool tmp = HaveItem;
	HaveItem = false;
	RoundRect->setValue(qRound(r*UmReFaktor*RoundRect->Decimals));
	HaveItem = tmp;
}

void Mpalette::setCols(int r, double g)
{
	bool tmp = HaveItem;
	HaveItem = false;
	DCol->setValue(r);
	DGap->setValue(qRound(g*UmReFaktor*DGap->Decimals));
	if (tmp)
		{
		DCol->setMaxValue(QMAX(qRound(CurItem->Width / QMAX(CurItem->ColGap, 10.0)), 1));
		DGap->setMaxValue(QMAX(qRound((CurItem->Width / CurItem->Cols - CurItem->Extra - CurItem->RExtra)*DGap->Decimals), 0));
		}
	DCol->setMinValue(1);
	DGap->setMinValue(0);
	HaveItem = tmp;
}

void Mpalette::setLsp(double r)
{
	bool tmp = HaveItem;
	HaveItem = false;
	LineSp->setValue(qRound(r*10));
	HaveItem = tmp;
}

void Mpalette::setSize(int s)
{
	bool tmp = HaveItem;
	HaveItem = false;
	Size->setValue(s);
	HaveItem = tmp;
}

void Mpalette::setExtra(double e)
{
	bool tmp = HaveItem;
	HaveItem = false;
	Extra->setValue(qRound(e*10));
	HaveItem = tmp;
}

void Mpalette::ChangeScaling()
{
	if (FreeScale == sender())
		{
		FrameScale->setChecked(false);
		FreeScale->setChecked(true);
		Aspect->setEnabled(false);
		}
	if (FrameScale == sender())
		{
		FrameScale->setChecked(true);
		FreeScale->setChecked(false);
		Aspect->setEnabled(true);
		}
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->ScaleType = FreeScale->isChecked();
		CurItem->AspectRatio = Aspect->isChecked();
		if (!CurItem->ScaleType)
			doc->ActPage->AdjustPictScale(CurItem);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		setFocus();
		}
}

void Mpalette::setLvalue(double scx, double scy, double x, double y)
{
	bool tmp = HaveItem;
	HaveItem = false;
	LXpos->setValue(qRound(x * UmReFaktor * LXpos->Decimals));
	LYpos->setValue(qRound(y * UmReFaktor * LYpos->Decimals));
	ScaleX->setValue(qRound(scx * 1000));
	ScaleY->setValue(qRound(scy * 1000));
	HaveItem = tmp;
}

void Mpalette::setSvalue(double s)
{
	bool tmp = HaveItem;
	HaveItem = false;
	LSize->setValue(qRound(s*10));
	HaveItem = tmp;
}

void Mpalette::setLIvalue(PenStyle p, PenCapStyle pc, PenJoinStyle pj)
{
	bool tmp = HaveItem;
	HaveItem = false;
	switch (p)
		{
		case SolidLine:
			LStyle->setCurrentItem(0);
			break;
		case DashLine:
			LStyle->setCurrentItem(1);
			break;
		case DotLine:
			LStyle->setCurrentItem(2);
			break;
		case DashDotLine:
			LStyle->setCurrentItem(3);
			break;
		case DashDotDotLine:
			LStyle->setCurrentItem(4);
			break;
		default:
			LStyle->setCurrentItem(0);
			break;
		}
	switch (pc)
		{
		case FlatCap:
			LEndStyle->setCurrentItem(0);
			break;
		case SquareCap:
			LEndStyle->setCurrentItem(1);
			break;
		case RoundCap:
			LEndStyle->setCurrentItem(2);
			break;
		default:
			LEndStyle->setCurrentItem(0);
			break;
		}
	switch (pj)
		{
		case MiterJoin:
			LJoinStyle->setCurrentItem(0);
			break;
		case BevelJoin:
			LJoinStyle->setCurrentItem(1);
			break;
		case RoundJoin:
			LJoinStyle->setCurrentItem(2);
			break;
		default:
			LJoinStyle->setCurrentItem(0);
			break;
		}
	HaveItem = tmp;
}

void Mpalette::setStil(int s)
{
	Supers->setOn(false);
	Subs->setOn(false);
	Strike->setOn(false);
	Underline->setOn(false);
	Kapital->setOn(false);
	Outlined->setOn(false);
	StrokeIcon->setEnabled(false);
	TxStroke->setEnabled(false);
	PM1->setEnabled(false);
	if (s & 1)
		Supers->setOn(true);
	if (s & 2)
		Subs->setOn(true);
	if (s & 4)
		{
		Outlined->setOn(true);
		StrokeIcon->setEnabled(true);
		TxStroke->setEnabled(true);
		PM1->setEnabled(true);
		}
	if (s & 8)
		Underline->setOn(true);
	if (s & 16)
		Strike->setOn(true);
	if (s & 64)
		Kapital->setOn(true);
}

void Mpalette::setAli(int e)
{
	bool tmp = HaveItem;
	HaveItem = false;
	switch (e)
		{
		case 0:
			TextL->setOn(true);
			break;
		case 2:
			TextR->setOn(true);
			break;
		case 1:
			TextC->setOn(true);
			break;
		case 3:
			TextB->setOn(true);
			break;
		case 4:
			TextF->setOn(true);
			break;
		default:
			TextL->setOn(false);
			TextR->setOn(false);
			TextC->setOn(false);
			TextB->setOn(false);
			TextF->setOn(false);
			break;
		}
	HaveItem = tmp;
}

void Mpalette::setTScale(int e)
{
	bool tmp = HaveItem;
	HaveItem = false;
  ChScale->setValue(e);
	HaveItem = tmp;
}

void Mpalette::NewTScale()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ItemTextScale(ChScale->value());
		doc->CurrTextScale = ChScale->value();
		emit DocChanged();
		}
}

void Mpalette::NewX()
{
	double x,y,w,h, gx, gy, gh, gw, base;
	QWMatrix ma;
	x = static_cast<double>(Xpos->value()) / UmReFaktor / Xpos->Decimals;
	y = static_cast<double>(Ypos->value()) / UmReFaktor / Ypos->Decimals;
	w = static_cast<double>(Width->value()) / UmReFaktor / Width->Decimals;
	h = static_cast<double>(Height->value()) / UmReFaktor / Height->Decimals;
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
 			if ((TopLeft->isChecked()) || (BottomLeft->isChecked()))
				base = gx;
  		if (Center->isChecked())
				base = gx + gw / 2.0;
  		if ((TopRight->isChecked()) || (BottomRight->isChecked()))
				base = gx + gw;
			doc->ActPage->moveGroup(x - base, 0, true);
			}
		else
			{
			if ((CurItem->PType == 5) && (LMode))
				{
				double r = atan2(h-y,w-x)*(180.0/3.1415927);
				w = sqrt(pow(w-x,2)+pow(h-y,2));
				doc->ActPage->MoveItem(x - CurItem->Xpos, 0, CurItem, true);
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
				doc->ActPage->RotateItem(r, CurItem->ItemNr);
				}
			else
				{
				ma.translate(CurItem->Xpos, CurItem->Ypos);
				ma.rotate(CurItem->Rot);
  			if (TopLeft->isChecked())
					base = CurItem->Xpos;
  			if (Center->isChecked())
					base = ma.m11() * (CurItem->Width / 2.0) + ma.m21() * (CurItem->Height / 2.0) + ma.dx();
  			if (TopRight->isChecked())
					base = ma.m11() * CurItem->Width + ma.m21() * 0.0 + ma.dx();
  			if (BottomRight->isChecked())
					base = ma.m11() * CurItem->Width + ma.m21() * CurItem->Height + ma.dx();
  			if (BottomLeft->isChecked())
					base = ma.m11() * 0.0 + ma.m21() * CurItem->Height + ma.dx();
				doc->ActPage->MoveItem(x - base, 0, CurItem, true);
				}
			}
		emit DocChanged();
		}
}

void Mpalette::NewY()
{
	double x,y,w,h, gx, gy, gh, gw, base;
	QWMatrix ma;
	x = static_cast<double>(Xpos->value()) / UmReFaktor / Xpos->Decimals;
	y = static_cast<double>(Ypos->value()) / UmReFaktor / Ypos->Decimals;
	w = static_cast<double>(Width->value()) / UmReFaktor / Width->Decimals;
	h = static_cast<double>(Height->value()) / UmReFaktor / Height->Decimals;
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
 			if ((TopLeft->isChecked()) || (TopRight->isChecked()))
				base = gy;
  		if (Center->isChecked())
				base = gy + gh / 2.0;
  		if ((BottomLeft->isChecked()) || (BottomRight->isChecked()))
				base = gy + gh;
			doc->ActPage->moveGroup(0, y - base, true);
			}
		else
			{
			if ((CurItem->PType == 5) && (LMode))
				{
				double r = atan2(h-y,w-x)*(180.0/3.1415927);
				w = sqrt(pow(w-x,2)+pow(h-y,2));
				doc->ActPage->MoveItem(0, y - CurItem->Ypos, CurItem, true);
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
				doc->ActPage->RotateItem(r, CurItem->ItemNr);
				}
			else
				{
				ma.translate(CurItem->Xpos, CurItem->Ypos);
				ma.rotate(CurItem->Rot);
  			if (TopLeft->isChecked())
					base = CurItem->Ypos;
  			if (Center->isChecked())
					base = ma.m22() * (CurItem->Height / 2.0) + ma.m12() * (CurItem->Width / 2.0) + ma.dy();
  			if (TopRight->isChecked())
					base = ma.m22() * 0.0 + ma.m12() * CurItem->Width + ma.dy();
  			if (BottomRight->isChecked())
					base = ma.m22() * CurItem->Height + ma.m12() * CurItem->Width + ma.dy();
  			if (BottomLeft->isChecked())
					base = ma.m22() * CurItem->Height + ma.m12() * 0.0 + ma.dy();
				doc->ActPage->MoveItem(0, y - base, CurItem);
				}
			}
		emit DocChanged();
		}
}

void Mpalette::NewW()
{
	double x,y,w,h, gx, gy, gh, gw;
	x = static_cast<double>(Xpos->value()) / UmReFaktor / Xpos->Decimals;
	y = static_cast<double>(Ypos->value()) / UmReFaktor / Ypos->Decimals;
	w = static_cast<double>(Width->value()) / UmReFaktor / Width->Decimals;
	h = static_cast<double>(Height->value()) / UmReFaktor / Height->Decimals;
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			doc->ActPage->HowTo = 1;
			doc->ActPage->scaleGroup(w / gw, w / gw);
			setBH(w, (w / gw) * gh);
			}
		else
			{
			CurItem->OldB2 = CurItem->Width;
			CurItem->OldH2 = CurItem->Height;
			if (CurItem->PType == 5)
				{
				if (LMode)
					{
					double r = atan2(h-y,w-x)*(180.0/3.1415927);
					doc->ActPage->RotateItem(r, CurItem->ItemNr);
					w = sqrt(pow(w-x,2)+pow(h-y,2));
					}
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
				}
			else
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
			emit DocChanged();
			}
		}
}

void Mpalette::NewH()
{
	double x,y,w,h, gx, gy, gh, gw;
	x = static_cast<double>(Xpos->value()) / UmReFaktor / Xpos->Decimals;
	y = static_cast<double>(Ypos->value()) / UmReFaktor / Ypos->Decimals;
	w = static_cast<double>(Width->value()) / UmReFaktor / Width->Decimals;
	h = static_cast<double>(Height->value()) / UmReFaktor / Height->Decimals;
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			doc->ActPage->HowTo = 1;
			doc->ActPage->scaleGroup(h / gh, h / gh);
			setBH((h / gh) * gw, h);
			}
		else
			{
			CurItem->OldB2 = CurItem->Width;
			CurItem->OldH2 = CurItem->Height;
			if (CurItem->PType == 5)
				{
				if (LMode)
					{
					double r = atan2(h-y,w-x)*(180.0/3.1415927);
					doc->ActPage->RotateItem(r, CurItem->ItemNr);
					w = sqrt(pow(w-x,2)+pow(h-y,2));
					}
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
				}
			else
				doc->ActPage->SizeItem(CurItem->Width, h, CurItem->ItemNr, true);
			}
		emit DocChanged();
		}
}

void Mpalette::NewR()
{
	double gx, gy, gh, gw;
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->RotateGroup(static_cast<double>(Rot->value() - RoVal)/100.0*(-1));
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			setXY(gx, gy);
			}
		else
			doc->ActPage->RotateItem(static_cast<double>(Rot->value())/100.0*(-1), CurItem->ItemNr);
		emit DocChanged();
		RoVal = Rot->value();
		}
}

void Mpalette::NewRR()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->RadRect = static_cast<double>(RoundRect->value()) / UmReFaktor / RoundRect->Decimals;
		doc->ActPage->SetFrameRounded();
		emit DocChanged();
		}
}

void Mpalette::NewLsp()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineSpa(static_cast<double>(LineSp->value())/10.0);
		emit DocChanged();
		}
}

void Mpalette::NewCols()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->Cols = DCol->value();
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		}
}

void Mpalette::NewGap()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->ColGap = static_cast<double>(DGap->value()) / UmReFaktor / DGap->Decimals;
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		}
}

void Mpalette::NewSize()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->chFSize(Size->value());
		emit DocChanged();
		}
}

void Mpalette::NewExtra()
{
	if ((HaveDoc) && (HaveItem))
		{
		if ((CurItem->HasSel) || (doc->AppMode == 1))
			{
			doc->ActPage->chKerning(static_cast<double>(Extra->value())/10);
			emit DocChanged();
			}
		else
			{
			if (uint(CurItem->CPos) != CurItem->Ptext.count())
				{
				CurItem->Ptext.at(CurItem->CPos)->cextra = static_cast<double>(Extra->value())/10;
				CurItem->Dirty = true;
				doc->ActPage->RefreshItem(CurItem);
				emit DocChanged();
				}
			}
		}
}

void Mpalette::NewLocalXY()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLocalXY(static_cast<double>(LXpos->value()) / UmReFaktor / LXpos->Decimals, static_cast<double>(LYpos->value()) / UmReFaktor / LYpos->Decimals);
		emit DocChanged();
		}
}

void Mpalette::NewLocalSC()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLocalSc(static_cast<double>(ScaleX->value()) / 1000.0, static_cast<double>(ScaleY->value()) / 1000.0);
		emit DocChanged();
		}
}

void Mpalette::NewLS()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineWidth(static_cast<double>(LSize->value())/10.0);
		emit DocChanged();
		}
}

void Mpalette::NewLSty()
{
	PenStyle c = SolidLine;
	switch (LStyle->currentItem())
		{
		case 0:
			c = SolidLine;
			break;
		case 1:
			c = DashLine;
			break;
		case 2:
			c = DotLine;
			break;
		case 3:
			c = DashDotLine;
			break;
		case 4:
			c = DashDotDotLine;
			break;
		}
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineArt(c);
		emit DocChanged();
		}
}

void Mpalette::NewLMode()
{
	if (LineMode->currentItem() == 0)
		{
    Text1->setText( tr( "X-Pos:" ) );
    Text2->setText( tr( "Width:" ) );
    Text1a->setText( tr( "Y-Pos:" ) );
    Text2a->setText( tr( "Height:" ) );
		Rot->setEnabled(true);
		Height->setEnabled(false);
		LMode = false;
		}
	else
		{
    Text1->setText( tr( "X1:" ) );
    Text2->setText( tr( "X2:" ) );
    Text1a->setText( tr( "Y1:" ) );
    Text2a->setText( tr( "Y2:" ) );
		Rot->setEnabled(false);
		Height->setEnabled(true);
		LMode = true;
		}
	setBH(CurItem->Width, CurItem->Height);
	updateGeometry();
	setFocus();
	repaint();
}

void Mpalette::NewLJoin()
{
	PenJoinStyle c = MiterJoin;
	switch (LJoinStyle->currentItem())
		{
		case 0:
			c = MiterJoin;
			break;
		case 1:
			c = BevelJoin;
			break;
		case 2:
			c = RoundJoin;
			break;
		}
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineJoin(c);
		emit DocChanged();
		}
}

void Mpalette::NewLEnd()
{
	PenCapStyle c = FlatCap;
	switch (LEndStyle->currentItem())
		{
		case 0:
			c = FlatCap;
			break;
		case 1:
			c = SquareCap;
			break;
		case 2:
			c = RoundCap;
			break;
		}
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineEnd(c);
		emit DocChanged();
		}
}

void Mpalette::ToggleKette()
{
	if (Kette->isOn())
		{
		ScaleY->setValue(ScaleX->value());
		NewLocalSC();
		}
}

void Mpalette::HChange()
{
	if (Kette->isOn())
		{
		ScaleY->setValue(ScaleX->value());
		}
	NewLocalSC();
}

void Mpalette::VChange()
{
	if (Kette->isOn())
		{
		ScaleX->setValue(ScaleY->value());
		}
	NewLocalSC();
}

void Mpalette::DoFlipH()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->FlipImageH();
		emit DocChanged();
		}
}

void Mpalette::DoFlipV()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->FlipImageV();
		emit DocChanged();
		}
}

void Mpalette::NewAli()
{
	if ((HaveDoc) && (HaveItem))
		{
		if (TextL->isOn())
			emit NewAbStyle(0);
		if (TextR->isOn())
			emit NewAbStyle(2);
		if (TextC->isOn())
			emit NewAbStyle(1);
		if (TextB->isOn())
			emit NewAbStyle(3);
		if (TextF->isOn())
			emit NewAbStyle(4);
		}
}

void Mpalette::setTypeStyle()
{
	if (Supers == sender())
		{
		emit Stellung(1);
		Subs->setOn(false);
		emit BackHome();
		return;
		}
	if (Subs == sender())
		{
		emit Stellung(2);
		Supers->setOn(false);
		emit BackHome();
		return;
		}
	if (Outlined == sender())
		{
		emit Stellung(4);
		emit BackHome();
		return;
		}
	if (Underline == sender())
		{
		emit Stellung(8);
		emit BackHome();
		return;
		}
	if (Strike == sender())
		{
		emit Stellung(16);
		emit BackHome();
		return;
		}
	if (Kapital == sender())
		{
		emit Stellung(64);
		emit BackHome();
		return;
		}
	emit Stellung(0);
}

void Mpalette::DoLower()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->LowerItem();
		emit DocChanged();
		}
}

void Mpalette::DoRaise()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->RaiseItem();
		emit DocChanged();
		}
}

void Mpalette::DoFront()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ToFront();
		emit DocChanged();
		}
}

void Mpalette::DoBack()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ToBack();
		emit DocChanged();
		}
}

void Mpalette::NewRotMode(int m)
{
	double inX, inY, gx, gy, gh, gw;
	if ((HaveDoc) && (HaveItem))
		{
		HaveItem = false;
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->setGroupRect();
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			if (m == 0)
				{
				doc->ActPage->RCenter = FPoint(gx, gy);
				inX = gx;
				inY = gy;
				}
			if (m == 1)
				{
				doc->ActPage->RCenter = FPoint(gx+gw, gy);
				inX = gx+gw;
				inY = gy;
				}
			if (m == 2)
				{
				doc->ActPage->RCenter = FPoint(gx + gw / 2.0, gy + gh / 2.0);
				inX = gx + gw / 2.0;
				inY = gy + gh / 2.0;
				}
			if (m == 3)
				{
				doc->ActPage->RCenter = FPoint(gx, gy+gh);
				inX = gx;
				inY = gy+gh;
				}
			if (m == 4)
				{
				doc->ActPage->RCenter = FPoint(gx+gw, gy+gh);
				inX = gx+gw;
				inY = gy+gh;
				}
			Xpos->setValue(qRound(inX*UmReFaktor*Xpos->Decimals));
			Ypos->setValue(qRound(inY*UmReFaktor*Ypos->Decimals));
			}
		else
			{
			double b, h, r;
			QWMatrix ma;
			FPoint n;
			b = CurItem->Width;
			h = CurItem->Height;
			r = CurItem->Rot;
			ma.translate(CurItem->Xpos, CurItem->Ypos);
			ma.rotate(r);
		  if (TopLeft->isChecked())
				n = FPoint(0.0, 0.0);
			if (TopRight->isChecked())
				n = FPoint(b, 0.0);
		  if (Center->isChecked())
				n = FPoint(b / 2.0, h / 2.0);
			if (BottomLeft->isChecked())
				n = FPoint(0.0, h);
			if (BottomRight->isChecked())
				n = FPoint(b, h);
			inX = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
			inY = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
			Xpos->setValue(qRound(inX*UmReFaktor*Xpos->Decimals));
			Ypos->setValue(qRound(inY*UmReFaktor*Ypos->Decimals));
			}
		HaveItem = true;
		doc->RotMode = m;
		}
}

void Mpalette::DoFlow()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->Textflow = Textflow->isChecked();
		doc->ActPage->update();
		emit DocChanged();
		ToggleFlow();
		}
}

void Mpalette::DoFlow2()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->Textflow2 = Textflow2->isChecked();
		doc->ActPage->update();
		emit DocChanged();
		}
}

void Mpalette::MakeIrre(int f, int c, double *vals)
{
	if ((HaveDoc) && (HaveItem))
		{
		if ((CurItem->PType == 7) || (CurItem->PType == 8))
			return;
		switch (f)
			{
			case 0:
				doc->ActPage->SetRectFrame(CurItem);
				break;
			case 1:
				doc->ActPage->SetOvalFrame(CurItem);
				break;
			default:
				doc->ActPage->SetFrameShape(CurItem, c, vals);
				break;
			}
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		if ((CurItem->PType == 2) || (CurItem->PType == 4))
			return;
		CurItem->PType = 6;
		NewSel(6);
		TabStack->raiseWidget(1);
		}
}

void Mpalette::EditSh()
{
	if ((HaveDoc) && (HaveItem))
		{
		if ((CurItem->PType == 1) || (CurItem->PType == 3))
			{
			CurItem->PType = 6;
			NewSel(6);
			TabStack->raiseWidget(1);
			}
		emit EditCL();
		emit DocChanged();
		}
}

void Mpalette::NewTDist()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->Extra = static_cast<double>(DLeft->value()) / UmReFaktor / DLeft->Decimals;
		CurItem->TExtra = static_cast<double>(DTop->value()) / UmReFaktor / DTop->Decimals;
		CurItem->BExtra = static_cast<double>(DBottom->value()) / UmReFaktor / DBottom->Decimals;
		CurItem->RExtra = static_cast<double>(DRight->value()) / UmReFaktor / DRight->Decimals;
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		}
}

void Mpalette::NewTFont(int c)
{
	if ((HaveDoc) && (HaveItem))
		emit NewTF(Fonts->text(c));
}

void Mpalette::DoRevert()
{
	if ((HaveDoc) && (HaveItem))
		{
		if (Revert->isOn())
			{
			CurItem->flippedH = 1;
			CurItem->Reverse = true;
			}
		else
			{
			CurItem->flippedH = 0;
			CurItem->Reverse = false;
			}
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		}
}

void Mpalette::SetLineFormats(ScribusDoc *dd)
{
  disconnect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	StyledLine->clear();
	if (dd != 0)
		{
		StyledLine->insertItem( tr("No Style"));
		QMap<QString,multiLine>::Iterator it;
		for (it = doc->MLineStyles.begin(); it != doc->MLineStyles.end(); ++it)
			{
			StyledLine->insertItem(it.key());
			}
		StyledLine->setSelected(StyledLine->currentItem(), false);
		}
  connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
}

void Mpalette::SetSTline(QListBoxItem *c)
{
	if (c == NULL)
  	return;
  bool setter = c->listBox()->currentItem() == 0 ? true : false;
  if (setter == true)
		CurItem->NamedLStyle = "";
	else
		CurItem->NamedLStyle = c->text();
	LStyle->setEnabled(setter);
	LSize->setEnabled(setter);
	LJoinStyle->setEnabled(setter);
	LEndStyle->setEnabled(setter);
	doc->ActPage->RefreshItem(CurItem);
	emit DocChanged();
}

void Mpalette::updateCList()
{
	if (!HaveDoc)
		return;
	TxFill->clear();
	TxStroke->clear();
	CListe::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	TxFill->insertItem( tr("None"));
	TxStroke->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		TxFill->insertItem(pm, it.key());
		TxStroke->insertItem(pm, it.key());
		}
}

void Mpalette::updateCmsList()
{
	if (HaveDoc)
		{
		if ((CMSavail) && (doc->CMSSettings.CMSinUse))
			GroupBoxCM->show();
		else
			{
			GroupBoxCM->hide();
			return;
			}
   	disconnect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChProf(const QString&)));
		InputP->clear();
		ProfilesL::Iterator itP;
		for (itP = InputProfiles.begin(); itP != InputProfiles.end(); ++itP)
			{
			InputP->insertItem(itP.key());
  		if (itP.key() == CurItem->IProfile)
  			InputP->setCurrentItem(InputP->count()-1);
			}
		if (!InputProfiles.contains(CurItem->IProfile))
			{
			InputP->insertItem(CurItem->IProfile);
  		InputP->setCurrentItem(InputP->count()-1);
			}
		else
			{
			if (CurItem->EmProfile != "")
				InputP->insertItem(CurItem->EmProfile);
			}
    MonitorI->setCurrentItem(CurItem->IRender);
   	connect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChProf(const QString&)));
		}
}

void Mpalette::ChProf(const QString& prn)
{
	bool EmbedP;
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->IProfile = InputP->currentText();
		if (prn.startsWith("Embedded"))
    	EmbedP = true;
		else
    	EmbedP = false;
		CurItem->UseEmbedded = EmbedP;
		doc->ActPage->LoadPict(CurItem->Pfile, CurItem->ItemNr);
		doc->ActPage->RefreshItem(CurItem);
		}
}

void Mpalette::ChIntent()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->IRender = MonitorI->currentItem();
		doc->ActPage->LoadPict(CurItem->Pfile, CurItem->ItemNr);
		doc->ActPage->RefreshItem(CurItem);
		}
}

void Mpalette::ShowCMS()
{
	if (HaveItem)
		{
		HaveItem = false;
		updateCmsList();
		HaveItem = true;
		}
	else
		{
		if ((CMSavail) && (doc->CMSSettings.CMSinUse))
			{
			GroupBoxCM->show();
/*			TextCms1->show();
			InputP->show();
			TextCms2->show();
			MonitorI->show();    */
			}
		else
			{
			GroupBoxCM->hide();
/*			TextCms1->hide();
			TextCms2->hide();
			InputP->hide();
			MonitorI->hide();    */
			}
		}
}

void Mpalette::newTxtFill()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ItemTextBrush(TxFill->currentText());
		doc->CurrTextFill = TxFill->currentText();
		emit DocChanged();
		}
}

void Mpalette::newTxtStroke()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ItemTextPen(TxStroke->currentText());
		doc->CurrTextStroke = TxStroke->currentText();
		emit DocChanged();
		}
}

void Mpalette::setActShade(int id)
{
	bool ok = false;
	uint a;
	int c, b;
	for (a = 0; a < TxFillSh->count(); ++a)
		{
		TxFillSh->setItemChecked(TxFillSh->idAt(a), false);
		}
	for (a = 0; a < TxFillSh->count(); ++a)
		{
		TxStrokeSh->setItemChecked(TxStrokeSh->idAt(a), false);
		}
	if (TxFillSh == sender())
		{
		c = TxFillSh->indexOf(id);
		TxFillSh->setItemChecked(id, true);
		}
	else
		{
		c = TxStrokeSh->indexOf(id);
		TxStrokeSh->setItemChecked(id, true);
		}
	if (c > 0)
		b = (c-1) * 10;
	if (c == 0)
		{
    Query* dia = new Query(this, "New", 1, 0, "Shade:", "Shade");
    if (dia->exec())
    	{
			c = dia->Answer->text().toInt(&ok);
			if (ok)
				{
				b = c;
				if (TxFillSh == sender())
					PM2->setText(dia->Answer->text()+" %");
				else
					PM1->setText(dia->Answer->text()+" %");
				}
			delete dia;
      }
     else
     	{
     	delete dia;
     	return;
     	}
		}
	if (TxFillSh == sender())
		{
		PM2->setText(QString::number(b)+" %");
		doc->ActPage->ItemTextBrushS(b);
		doc->CurrTextFillSh = b;
		}
	else
		{
		PM1->setText(QString::number(b)+" %");
		doc->ActPage->ItemTextPenS(b);
		doc->CurrTextStrokeSh = b;
		}
	emit DocChanged();
}

void Mpalette::setActFarben(QString p, QString b, int shp, int shb)
{
	uint a;
	CListe::Iterator it;
	int c = 1;
	for (a = 0; a < TxFillSh->count(); ++a)
		{
		TxFillSh->setItemChecked(TxFillSh->idAt(a), false);
		}
	for (a = 0; a < TxFillSh->count(); ++a)
		{
		TxStrokeSh->setItemChecked(TxStrokeSh->idAt(a), false);
		}
	if ((shb % 10) == 0)
		TxFillSh->setItemChecked(TxFillSh->idAt(shb/10+1), true);
	else
		TxFillSh->setItemChecked(TxFillSh->idAt(0), true);
	PM2->setText(QString::number(shb)+" %");
	if ((shp % 10) == 0)
		TxStrokeSh->setItemChecked(TxStrokeSh->idAt(shp/10+1), true);
	else
		TxStrokeSh->setItemChecked(TxStrokeSh->idAt(0), true);
	PM1->setText(QString::number(shp)+" %");
	if ((b != "None") && (b != ""))
		{
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
			{
			if (it.key() == b)
				break;
			c++;
			}
		}
	TxFill->setCurrentItem(c);
	c = 1;
	if ((p != "None") && (p != ""))
		{
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
			{
			if (it.key() == p)
				break;
			c++;
			}
		}
	TxStroke->setCurrentItem(c);
}

void Mpalette::handleLock()
{
	if ((HaveDoc) && (HaveItem))
  	doc->ActPage->ToggleLock();
}

void Mpalette::handlePrint()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->isPrintable = !NoPrint->isOn();
		emit DocChanged();
		}
}

void Mpalette::ToggleFlow()
{
	if (Textflow->isChecked())
		Textflow2->setEnabled(true);
	else
		Textflow2->setEnabled(false);
}

void Mpalette::handlePathLine()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->PoShow = NormText2->isChecked();
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		}
}

void Mpalette::handlePathDist()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->Extra = static_cast<double>(Dist->value()) / 10;
		doc->ActPage->UpdatePolyClip(CurItem);
		doc->ActPage->AdjustItemSize(CurItem);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		}
}

void Mpalette::handlePathOffs()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->BaseOffs = static_cast<double>(-LineW->value()) / 10;
		doc->ActPage->UpdatePolyClip(CurItem);
		doc->ActPage->AdjustItemSize(CurItem);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		}
}

void Mpalette::NewName()
{
	QString NameOld = CurItem->AnName;
	QString NameNew = NameEdit->text();
	bool found = false;
	ScribusView* view = (ScribusView*)doc->ActPage->Anz;
	for (uint a = 0; a < view->Pages.count(); ++a)
		{
		for (uint b = 0; b < view->Pages.at(a)->Items.count(); ++b)
			{
			if ((NameNew == view->Pages.at(a)->Items.at(b)->AnName) && (view->Pages.at(a)->Items.at(b) != CurItem))
				{
				found = true;
				break;
				}
			}
		if (found)
			break;
		}
	if (found)
		{
		QMessageBox::warning(this, tr("Warning"), tr("Name \"%1\" isn't unique.\nPlease choose another.").arg(NameNew), tr("OK"));
		NameEdit->setText(NameOld);
		NameEdit->setFocus();
		}
	else
		{
		if (CurItem->AnName != NameEdit->text())
			{
			CurItem->AnName = NameEdit->text();
			CurItem->AutoName = false;
			emit DocChanged();
			}
		}
}

void Mpalette::fillLangCombo(QMap<QString,QString> Sprachen)
{
	LangCombo->clear();
	QMap<QString,QString>::Iterator it;
	for (it = Sprachen.begin(); it != Sprachen.end(); ++it)
		{
		LangCombo->insertItem(it.key());
		}
}

void Mpalette::NewLanguage()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->Language = LangCombo->currentText();
		emit DocChanged();
		}
}

void Mpalette::ManageTabs()
{
	if ((HaveDoc) && (HaveItem))
		{
		TabManager *dia = new TabManager(this, doc->Einheit, CurItem->TabValues);
		if (dia->exec())
			{
			CurItem->TabValues = dia->tmpTab;
			doc->ActPage->RefreshItem(CurItem);
			emit DocChanged();
			}
		delete dia;
		}
}
