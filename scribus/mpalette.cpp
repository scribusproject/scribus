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
#include "scribus.h"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;
extern ProfilesL InputProfiles;
extern ScribusApp* ScApp;

LabelButton::LabelButton(QWidget* parent, QString text1, QString text2) : QLabel(parent)
{
	state = true;
	setTexts(text1, text2);
}

void LabelButton::setTexts(QString text1, QString text2)
{
	TextA = text1;
	TextB = text2;
	if (state)
		setText(TextA);
	else
		setText(TextB);
}

bool LabelButton::getState()
{
	return state;
}

void LabelButton::enterEvent(QEvent*)
{
	setFrameStyle( QFrame::Panel | QFrame::Raised );
	setLineWidth( 2 );
}

void LabelButton::leaveEvent(QEvent*)
{
	setFrameStyle( QFrame::Panel | QFrame::Plain );
	setLineWidth( 0 );
}

void LabelButton::mousePressEvent(QMouseEvent*)
{
	setFrameStyle( QFrame::Panel | QFrame::Sunken );
	setLineWidth( 2 );
}

void LabelButton::mouseReleaseEvent(QMouseEvent*)
{
	state = !state;
	if (state)
		setText(TextA);
	else
		setText(TextB);
	setFrameStyle( QFrame::Panel | QFrame::Raised );
	setLineWidth( 2 );
	emit clicked();
}

NameWidget::NameWidget(QWidget* parent) : QLineEdit(parent)
{}

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
	Xpos->setMaxValue( 3000 );
	Xpos->setMinValue( -3000 );
	GeoGroupLayout->addWidget( Xpos, 0, 1 );
	Ypos = new MSpinBox( GeoGroup, 2 );
	Ypos->setSuffix( tr( " pt" ) );
	Ypos->setMaxValue( 3000 );
	Ypos->setMinValue( -3000 );
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
	Kette2 = new LinkButton( GeoGroup );
	Kette2->setToggleButton( true );
	Kette2->setAutoRaise( true );
	Kette2->setMaximumSize( QSize( 15, 32767 ) );
	GeoGroupLayout->addMultiCellWidget( Kette2, 2, 3, 2, 2 );
	Rot = new MSpinBox( GeoGroup, 2);
	Rot->setSuffix( QString::fromUtf8(" °"));
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
	TopLeft->setText( "" );
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
	TopRight->setText( "" );
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
	Center->setText( "" );
	Layout12->addWidget( Center, 1, 1, Qt::AlignCenter );
	Line4 = new QFrame( RotationGroup, "Line4" );
	Line4->setMinimumSize( QSize( 4, 20 ) );
	Line4->setMaximumSize( QSize( 4, 20 ) );
	Line4->setFrameShadow( QFrame::Plain );
	Line4->setLineWidth( 3 );
	Line4->setFrameShape( QFrame::VLine );
	Layout12->addWidget( Line4, 1, 2, Qt::AlignCenter );
	BottomLeft = new QRadioButton( RotationGroup, "BottomLeft" );
	BottomLeft->setText( "" );
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
	BottomRight->setText( "" );
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
	LevelTxt = new QLabel( LayerGroup, "LevelTxt" );
	LevelTxt->setAlignment( Qt::AlignCenter );
	LevelTxt->setText( "  0");
	LayerGroupLayout->addMultiCellWidget( LevelTxt, 0, 1, 2, 2 );

	layout60->addWidget( LayerGroup );
	QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout60->addItem( spacer2 );

	Layout44 = new QGridLayout( 0, 1, 1, 8, 4, "Layout44");
	FlipH = new QToolButton( page, "MirrorH" );
	FlipH->setPixmap(loadIcon("FlipH.xpm"));
	FlipH->setToggleButton( true );
	Layout44->addWidget( FlipH, 0, 0 );
	FlipV = new QToolButton( page, "MirrorV" );
	FlipV->setPixmap(loadIcon("FlipV.xpm"));
	FlipV->setToggleButton( true );
	Layout44->addWidget( FlipV, 1, 0 );
	Locked = new QToolButton( page, "Lock" );
	Locked->setToggleButton( true );
	QIconSet a = QIconSet();
	a.setPixmap(loadIcon("locked.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::On);
	a.setPixmap(loadIcon("unlock.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::Off);
	Locked->setIconSet(a);
	Layout44->addWidget( Locked, 0, 1 );
	NoPrint = new QToolButton( page, "NoPrint" );
	NoPrint->setToggleButton( true );
	QIconSet a2 = QIconSet();
	a2.setPixmap(loadIcon("NoPrint.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::On);
	a2.setPixmap(loadIcon("DateiPrint16.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::Off);
	NoPrint->setIconSet(a2);
	Layout44->addWidget( NoPrint, 1, 1 );
	NoResize = new QToolButton( page, "NoResize" );
	NoResize->setToggleButton( true );
	QIconSet a3 = QIconSet();
	a3.setPixmap(loadIcon("framenoresize.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::On);
	a3.setPixmap(loadIcon("frameresize.png"), QIconSet::Automatic, QIconSet::Normal, QIconSet::Off);
	NoResize->setIconSet(a3);
	Layout44->addWidget( NoResize, 0, 2 );
	layout60->addLayout( Layout44 );

	QSpacerItem* spacer5 = new QSpacerItem( 0, 0, QSizePolicy::Expanding,
	                                        QSizePolicy::Minimum );
	layout60->addItem( spacer5 );
	pageLayout->addLayout( layout60 );

	Textflow = new QCheckBox( page, "Textflow" );
	Textflow->setText( tr( "Text flows around frame" ) );
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
	ShapeGroup->setTitle( "" );
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
	Text14b = new LabelButton( Distance, tr("Gap:"), tr("Width:"));
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
	DGap->setMaxValue( 300 );
	DGap->setMinValue( 0 );
	DistanceLayout->addWidget( DGap, 1, 1 );
	DTop = new MSpinBox( Distance, 1 );
	DTop->setSuffix( tr( " pt" ) );
	DTop->setMaxValue( 300 );
	DTop->setMinValue( 0 );
	DistanceLayout->addWidget( DTop, 2, 1 );
	DBottom = new MSpinBox( Distance, 1 );
	DBottom->setSuffix( tr( " pt" ) );
	DBottom->setMaxValue( 300 );
	DBottom->setMinValue( 0 );
	DistanceLayout->addWidget( DBottom, 3, 1 );
	DLeft = new MSpinBox( Distance, 1 );
	DLeft->setSuffix( tr( " pt" ) );
	DLeft->setMaxValue( 300 );
	DLeft->setMinValue( 0 );
	DistanceLayout->addWidget( DLeft, 4, 1 );
	DRight = new MSpinBox( Distance, 1 );
	DRight->setSuffix( tr( " pt" ) );
	DRight->setMaxValue( 300 );
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
	Distance2->setTitle( tr( "Path Text Properties" ) );
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
	Dist->setMaxValue( 300 );
	Dist->setMinValue( 0 );
	Dist->setLineStep(10);
	DistanceLayout2->addWidget( Dist, 1, 1);
	TextLabel2 = new QLabel( Distance2, "TextLabel2" );
	TextLabel2->setText( tr( "Distance from Curve:" ) );
	DistanceLayout2->addWidget( TextLabel2, 2, 0);
	LineW = new MSpinBox( Distance2, 1 );
	LineW->setSuffix( tr( " pt" ) );
	LineW->setMaxValue( 300 );
	LineW->setMinValue( -300 );
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
	Text20->setText( tr("Font Size:"));
	layout41->addWidget( Text20, 1, 0 );
	Size = new MSpinBox( page_3, 1 );
	Size->setPrefix( "" );
	Size->setSuffix( tr( " pt" ) );
	Size->setMaxValue( 1024 );
	Size->setMinValue( 1 );
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
	StrokeIcon->setScaledContents( false );
	layout23->addWidget( StrokeIcon );
	TxStroke = new QComboBox( true, page_3, "TxStroke" );
	TxStroke->setEditable(false);
	layout23->addWidget( TxStroke );
	QSpacerItem* spacer2S = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout23->addItem( spacer2S );
	ShadeTxt1 = new QLabel( page_3, "ShadeTxt" );
	ShadeTxt1->setText( tr( "Shade:" ) );
	layout23->addWidget( ShadeTxt1 );
	PM1 = new ShadeButton(page_3);
	layout23->addWidget( PM1 );
	layout46->addLayout( layout23 );
	layout24 = new QHBoxLayout( 0, 0, 5, "layout24");
	FillIcon = new QLabel( page_3, "FillIcon" );
	FillIcon->setText("");
	FillIcon->setPixmap(loadIcon("fill.png"));
	FillIcon->setScaledContents( false );
	layout24->addWidget( FillIcon );
	TxFill = new QComboBox( true, page_3, "TxFill" );
	TxFill->setEditable(false);
	layout24->addWidget( TxFill );
	QSpacerItem* spacer2Sf = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout24->addItem( spacer2Sf );
	ShadeTxt2 = new QLabel( page_3, "ShadeTxt" );
	ShadeTxt2->setText( tr( "Shade:" ) );
	layout24->addWidget( ShadeTxt2 );
	PM2 = new ShadeButton(page_3);
	layout24->addWidget( PM2 );
	layout46->addLayout( layout24 );

	Layout1 = new QHBoxLayout( 0, 0, 0, "Layout1");
	SeStyle = new StyleSelect(page_3);
	Layout1->addWidget(SeStyle);
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

	GroupAlign = new AlignSelect(page_3);
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

	GroupBox3aLayout = new QGridLayout( 0, 1, 1, 0, 5, "Layout25");
	StilT = new QLabel( page_3, "Stil" );
	StilT->setText( tr("Style:"));
	GroupBox3aLayout->addWidget( StilT, 0, 0 );
	Spal = new Spalette(page_3);
	GroupBox3aLayout->addWidget( Spal, 0, 1 );
	SprachT = new QLabel( page_3, "Sprache" );
	SprachT->setText( tr("Language:"));
	GroupBox3aLayout->addWidget( SprachT, 1, 0 );
	LangCombo = new QComboBox( true, page_3, "Lang" );
	LangCombo->setEditable(false);
	GroupBox3aLayout->addWidget( LangCombo, 1, 1 );
	pageLayout_3->addLayout(GroupBox3aLayout);

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
	Text11->setText( tr( "X-Pos.:" ) );
	layout43->addWidget( Text11, 0, 0 );
	LXpos = new MSpinBox( page_4, 2 );
	LXpos->setSuffix( tr( " pt" ) );
	layout43->addWidget( LXpos, 0, 1 );
	Text11a = new QLabel( page_4, "Text11a" );
	Text11a->setText( tr( "Y-Pos.:" ) );
	layout43->addWidget( Text11a, 1, 0 );
	LYpos = new MSpinBox( page_4, 2 );
	LYpos->setSuffix( tr( " pt" ) );
	layout43->addWidget( LYpos, 1, 1 );
	Text12 = new QLabel( page_4, "Text12" );
	Text12->setText( tr( "X-Scale:" ) );
	layout43->addWidget( Text12, 2, 0 );
	ScaleX = new MSpinBox( page_4, 1 );
	ScaleX->setSuffix( tr( " %" ) );
	layout43->addWidget( ScaleX, 2, 1 );
	Text13 = new QLabel( page_4, "Text13" );
	Text13->setText( tr( "Y-Scale:" ) );
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
	Aspect->setEnabled( false );
	Aspect->setText( tr( "Proportional" ) );
	Aspect->setChecked( true );

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
	LineModeT = new QLabel( page_5, "LModeText" );
	LineModeT->setText( tr( "Basepoint:" ) );
	Layout13_2->addWidget( LineModeT );
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

	TabStack3 = new QWidgetStack( page_5, "TabStack3" );
	TabStack3->setFrameShape( QWidgetStack::NoFrame );

	page_5a = new QWidget( TabStack3, "page" );
	pageLayout_5a = new QVBoxLayout( page_5a, 0, 5, "pageLayout_5a");
	StyledLine = new QListBox(page_5a, "StyledL");
	StyledLine->insertItem( tr("No Style"), 0);
	pageLayout_5a->addWidget(StyledLine);
	TabStack3->addWidget( page_5a, 0 );

	page_5b = new QWidget( TabStack3, "page" );
	pageLayout_5b = new QVBoxLayout( page_5b, 0, 5, "pageLayout_5a");
	TLines = new QGroupBox( page_5b, "Distance" );
	TLines->setTitle( tr( "Cell Lines" ) );
	TLines->setColumnLayout(0, Qt::Vertical );
	TLines->layout()->setSpacing( 2 );
	TLines->layout()->setMargin( 5 );
	TLineLayout = new QVBoxLayout( TLines->layout() );
	TLineLayout->setAlignment( Qt::AlignTop );
	TopLine = new QCheckBox( TLines, "TopLine" );
	TopLine->setText( tr( "Line at Top" ) );
	TLineLayout->addWidget(TopLine);
	LeftLine = new QCheckBox( TLines, "TopLine" );
	LeftLine->setText( tr( "Line at the Left" ) );
	TLineLayout->addWidget(LeftLine);
	RightLine = new QCheckBox( TLines, "TopLine" );
	RightLine->setText( tr( "Line at the Right " ) );
	TLineLayout->addWidget(RightLine);
	BottomLine = new QCheckBox( TLines, "TopLine" );
	BottomLine->setText( tr( "Line at Bottom" ) );
	TLineLayout->addWidget(BottomLine);
	pageLayout_5b->addWidget(TLines);
	TabStack3->addWidget( page_5b, 1 );
	pageLayout_5->addWidget( TabStack3 );
	TabStack->addWidget( page_5, 4 );
	Cpal = new Cpalette(this);
	TabStack->addWidget(Cpal, 5);
	MpalLayout->addWidget( TabStack );
	QToolTip::add( NameEdit, tr( "Name of selected object" ) );
	QToolTip::add( Xpos, tr( "Horizontal position of current basepoint" ) );
	QToolTip::add( Ypos, tr( "Vertical position of current basepoint" ) );
	QToolTip::add( Width, tr( "Width" ) );
	QToolTip::add( Height, tr( "Height" ) );
	QToolTip::add( Rot, tr( "Rotation of object at current basepoint" ) );
	QToolTip::add( Text3r, tr( "Point from which measurements or rotation angles are referenced" ) );
	QToolTip::add( TopLeft, tr( "Select top left for basepoint" ) );
	QToolTip::add( TopRight, tr( "Select top right for basepoint" ) );
	QToolTip::add( BottomLeft, tr( "Select bottom left for basepoint" ) );
	QToolTip::add( BottomRight, tr( "Select bottom right for basepoint" ) );
	QToolTip::add( Center, tr( "Select center for basepoint" ) );
	QToolTip::add( FlipH, tr( "Flip Horizontal" ) );
	QToolTip::add( FlipV, tr( "Flip Vertical" ) );
	QToolTip::add( Zup, tr( "Move one level up" ) );
	QToolTip::add( ZDown, tr( "Move one level down" ) );
	QToolTip::add( ZTop, tr( "Move to front" ) );
	QToolTip::add( ZBottom, tr( "Move to back" ) );
	QToolTip::add( LevelTxt, tr( "Indicates the level the object is on, 0 means the object is at the bottom" ) );
	QToolTip::add( Locked, tr( "Lock or unlock the object" ) );
	QToolTip::add( NoResize, tr( "Lock or unlock the size of the object" ) );
	QToolTip::add( NoPrint, tr( "Enable or disable printing of the object" ) );
	QToolTip::add( Textflow, tr( "Make text in lower frames flow around the object shape" ) );
	QToolTip::add( Textflow2, tr( "Use the frame shape instead of the object one" ) );

	QToolTip::add( Fonts, tr( "Font of selected text or object" ) );
	QToolTip::add( Size, tr( "Font Size" ) );
	QToolTip::add( ChScale, tr( "Scaling width of characters" ) );
	QToolTip::add( TxStroke, tr( "Color of text stroke" ) );
	QToolTip::add( TxFill, tr( "Color of text fill" ) );
	QToolTip::add( PM1, tr( "Saturation of color of text stroke" ) );
	QToolTip::add( PM2, tr( "Saturation of color of text fill" ) );
	QToolTip::add( Revert, tr( "Reverse Writing" ) );
	QToolTip::add( Extra, tr( "Manual Kerning" ) );
	QToolTip::add( LineSp, tr( "Line Spacing" ) );
	QToolTip::add( Spal, tr( "Style of current paragraph" ) );
	QToolTip::add( LangCombo, tr( "Language of object" ) );

	QToolTip::add( LineMode, tr( "Change settings for left or end points" ) );
	QToolTip::add( LStyle, tr( "Pattern of line" ) );
	QToolTip::add( LSize, tr( "Thickness of line" ) );
	QToolTip::add( LJoinStyle, tr( "Type of line joins" ) );
	QToolTip::add( LEndStyle, tr( "Type of line end" ) );
	QToolTip::add( StyledLine, tr( "Line style of current object" ) );

	QToolTip::add( SCustom, tr( "Choose the shape of frame..." ) );
	QToolTip::add( EditShape, tr( "Edit shape of the frame..." ) );
	QToolTip::add( RoundRect, tr( "Set radius of corner rounding" ) );
	QToolTip::add( DCol, tr( "Number of columns in text frame" ) );
	QToolTip::add( Text14b, tr("Switches between Gap or Column width"));
	QToolTip::add( DGap, tr( "Distance between columns" ) );
	QToolTip::add( DTop, tr( "Distance of text from top of frame" ) );
	QToolTip::add( DBottom, tr( "Distance of text from bottom of frame" ) );
	QToolTip::add( DLeft, tr( "Distance of text from left of frame" ) );
	QToolTip::add( DRight, tr( "Distance of text from right of frame" ) );
	QToolTip::add( TabsButton, tr( "Edit tab settings of text frame..." ) );

	QToolTip::add( FreeScale, tr( "Allow the image to be a different size to the frame" ) );
	QToolTip::add( LXpos, tr( "Horizontal offset of image within frame" ) );
	QToolTip::add( LYpos, tr( "Vertical offset of image within frame" ) );
	QToolTip::add( ScaleX, tr( "Resize the image horizontally" ) );
	QToolTip::add( ScaleY, tr( "Resize the image vertically" ) );
	QToolTip::add( Kette, tr( "Keep the X and Y scaling the same" ) );
	QToolTip::add( Kette2, tr( "Keep the aspect ratio" ) );
	QToolTip::add( FrameScale, tr( "Make the image fit within the size of the frame" ) );
	QToolTip::add( Aspect, tr( "Use image proportions rather than those of the frame" ) );
	QToolTip::add( InputP, tr( "Source profile of the image" ) );
	QToolTip::add( MonitorI, tr( "Rendering intent for the image" ) );

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
	connect(GroupAlign, SIGNAL(State(int)), this, SLOT(NewAli(int)));
	connect(Revert, SIGNAL(clicked()), this, SLOT(DoRevert()));
	connect(SeStyle, SIGNAL(State(int)), this, SLOT(setTypeStyle(int)));
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
	connect(Fonts, SIGNAL(activated(int)), this, SLOT(NewTFont(int)));
	connect(TxFill, SIGNAL(activated(int)), this, SLOT(newTxtFill()));
	connect(TxStroke, SIGNAL(activated(int)), this, SLOT(newTxtStroke()));
	connect(PM1, SIGNAL(clicked()), this, SLOT(setActShade()));
	connect(PM2, SIGNAL(clicked()), this, SLOT(setActShade()));
	connect(ChScale, SIGNAL(valueChanged(int)), this, SLOT(NewTScale()));
	connect(Locked, SIGNAL(clicked()), this, SLOT(handleLock()));
	connect(NoPrint, SIGNAL(clicked()), this, SLOT(handlePrint()));
	connect(NoResize, SIGNAL(clicked()), this, SLOT(handleResize()));
	connect(NormText2, SIGNAL(clicked()), this, SLOT(handlePathLine()));
	connect(Dist, SIGNAL(valueChanged(int)), this, SLOT(handlePathDist()));
	connect(LineW, SIGNAL(valueChanged(int)), this, SLOT(handlePathOffs()));
	connect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChProf(const QString&)));
	connect(MonitorI, SIGNAL(activated(int)), this, SLOT(ChIntent()));
	connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	connect(LangCombo, SIGNAL(activated(int)), this, SLOT(NewLanguage()));
	connect( TabsButton, SIGNAL( clicked() ), this, SLOT( ManageTabs() ) );
	connect( TopLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( LeftLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( RightLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( BottomLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( Text14b, SIGNAL( clicked() ), this, SLOT( HandleGapSwitch() ) );
	HaveItem = false;
	Xpos->setValue(0);
	Ypos->setValue(0);
	Width->setValue(0);
	Height->setValue(0);
	Rot->setValue(0);
	RoundRect->setValue(0);
	TabStack3->raiseWidget(0);
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

void Mpalette::reject()
{
	emit Schliessen();
	QDialog::reject();
}

void Mpalette::SelTab(int t)
{
	if (ScApp->ScriptRunning)
		return;
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
	HaveItem = false;
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
	Xpos->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	Xpos->setMinValue( -3000  * UmReFaktor);
	Ypos->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	Ypos->setMinValue( -3000  * UmReFaktor);
	Width->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	Width->setMinValue( 1  * UmReFaktor);
	Height->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	Height->setMinValue( 1  * UmReFaktor);
	Rot->setMaxValue( 360 );
	Rot->setMinValue( 0 );
	RoundRect->setMaxValue( 300 );
	RoundRect->setMinValue( -300);
	LYpos->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	LYpos->setMinValue( -3000 );
	ScaleY->setMaxValue( 3000 );
	ScaleY->setMinValue( 1 );
	ScaleX->setMaxValue( 3000 );
	ScaleX->setMinValue( 1 );
	LXpos->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	LXpos->setMinValue( -3000 );
	Size->setMaxValue( 1024 );
	Size->setMinValue( 1 );
	LineSp->setMaxValue( 300 );
	LineSp->setMinValue( 1 );
	Extra->setMaxValue( 300 );
	Extra->setMinValue( -300 );
	LSize->setMaxValue( 36 );
	LSize->setMinValue( 0 );
	Dist->setMaxValue( 300 );
	Dist->setMinValue( 0 );
	Dist->setLineStep(10);
	LineW->setMaxValue( 300 );
	LineW->setMinValue( -300 );
	LineW->setLineStep(10);
	updateCList();
}

void Mpalette::UnsetDoc()
{
	HaveDoc = false;
}

void Mpalette::SetCurItem(PageItem *i)
{
	if (ScApp->ScriptRunning)
		return;
	disconnect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	disconnect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	HaveItem = false;
	CurItem = i;
	if (i->FrameType == 0)
		SCustom->setPixmap(SCustom->getIconPixmap(0));
	if (i->FrameType == 1)
		SCustom->setPixmap(SCustom->getIconPixmap(1));
	if (i->FrameType > 3)
		SCustom->setPixmap(SCustom->getIconPixmap(i->FrameType-2));
	NameEdit->setText(i->AnName);
	RoundRect->setValue(i->RadRect*UmReFaktor);
	QString tm;
	LevelTxt->setText(tm.setNum(i->ItemNr));
	DCol->setMaxValue(QMAX(qRound(i->Width / QMAX(i->ColGap, 10.0)), 1));
	DCol->setMinValue(1);
	DGap->setMinValue(0);
	DCol->setValue(i->Cols);
	if (Text14b->getState())
	{
		DGap->setMaxValue(QMAX((i->Width / i->Cols - i->Extra - i->RExtra)*UmReFaktor, 0));
		DGap->setValue(i->ColGap*UmReFaktor);
	}
	else
	{
		double lineCorr;
		if (i->Pcolor2 != "None")
			lineCorr = i->Pwidth;
		else
			lineCorr = 0;
		double ColWidth = (i->Width - (i->ColGap * (i->Cols - 1)) - i->Extra - i->RExtra - lineCorr) / i->Cols;
		DGap->setMaxValue(QMAX((i->Width / i->Cols)*UmReFaktor, 0));
		DGap->setValue(ColWidth*UmReFaktor);
	}
	DLeft->setValue(i->Extra*UmReFaktor);
	DTop->setValue(i->TExtra*UmReFaktor);
	DBottom->setValue(i->BExtra*UmReFaktor);
	DRight->setValue(i->RExtra*UmReFaktor);
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
	NoPrint->setOn(!i->isPrintable);
	setter = i->Locked ? false : true;
	Kette2->setOn(false);
	Width->setEnabled(setter);
	Height->setEnabled(setter);
	RoundRect->setEnabled(setter);
	EditShape->setEnabled(setter);
	ShapeGroup->setEnabled(setter);
	LayerGroup->setEnabled(setter);
	Locked->setOn(!setter);
	if ((i->isTableItem) && (i->isSingleSel))
	{
		setter = false;
		TabStack3->raiseWidget(1);
		TopLine->setChecked(i->TopLine);
		LeftLine->setChecked(i->LeftLine);
		RightLine->setChecked(i->RightLine);
		BottomLine->setChecked(i->BottomLine);
	}
	else
		TabStack3->raiseWidget(0);
	LayerGroup->setEnabled(setter);
	Xpos->setEnabled(setter);
	Ypos->setEnabled(setter);
	Rot->setEnabled(setter);
	setter = i->LockRes;
	NoResize->setOn(setter);
	if (!i->Locked)
	{
		Width->setEnabled(!setter);
		Height->setEnabled(!setter);
	}
	if (i->Locked)
	{
		HaveItem = true;
		return;
	}
	if (i->PType == 8)
	{
		TabStack2->raiseWidget(1);
		NormText2->setChecked(i->PoShow);
		LineW->setValue(i->BaseOffs * -1);
		Dist->setValue(i->Extra);
	}
	else
		TabStack2->raiseWidget(0);
	if (((i->PType == 4) || (i->PType == 2) || (i->PType == 3)) &&  (!i->ClipEdited))
		RoundRect->setEnabled(true);
	else
	{
		if ((i->PType == 6) && ((i->FrameType == 0) || (i->FrameType == 2)))
			RoundRect->setEnabled(true);
		else
			RoundRect->setEnabled(false);
	}
	if (i->PType == 5)
	{
		Kette2->setEnabled(false);
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
		Kette2->setEnabled(true);
		if (i->PType == 2)
		{
			updateCmsList();
			setter = i->ScaleType;
			FreeScale->setChecked(setter);
			FrameScale->setChecked(!setter);
			if (setter == false)
				Kette->setOn(setter);
			Aspect->setEnabled(!setter);
			Aspect->setChecked(i->AspectRatio);
			LXpos->setEnabled(setter);
			LYpos->setEnabled(setter);
			ScaleX->setEnabled(setter);
			ScaleY->setEnabled(setter);
		}
	}
	HaveItem = true;
}

void Mpalette::NewSel(int nr)
{
	if (ScApp->ScriptRunning)
		return;
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
		visID = TabStack->id(TabStack->visibleWidget());
		TabStack->widget(0)->setEnabled(true);
		TabStack->widget(1)->setEnabled(false);
		TabStack->widget(2)->setEnabled(false);
		TabStack->widget(3)->setEnabled(false);
		TabStack->widget(4)->setEnabled(false);
		TabStack->widget(5)->setEnabled(true);
		SColor->setEnabled(true);
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
				TabStack->widget(ws)->setEnabled(false);
			SGeom->setEnabled(false);
			SShape->setEnabled(false);
			SText->setEnabled(false);
			SImage->setEnabled(false);
			SLine->setEnabled(false);
			SColor->setEnabled(false);
			//				SGeom->setOn(true);
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
			FlipH->setEnabled(true);
			FlipV->setEnabled(true);
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
			FlipH->setEnabled(true);
			FlipV->setEnabled(true);
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
			FlipH->setEnabled(true);
			FlipV->setEnabled(true);
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
			FlipH->setEnabled(true);
			FlipV->setEnabled(true);
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
	double oldX = Xpos->value() / old;
	double oldY = Ypos->value() / old;
	double oldW = Width->value() / old;
	double oldH = Height->value() / old;
	double oldLX = LXpos->value() / old;
	double oldLY = LYpos->value() / old;
	double oldG = DGap->value() / old;
	double oldGM = DGap->maxValue() / old;
	double oldDT = DTop->value() / old;
	double oldDL = DLeft->value() / old;
	double oldDB = DBottom->value() / old;
	double oldDR = DRight->value() / old;
	double oldRR = RoundRect->value() / old;
	double oldRM = RoundRect->maxValue() / old;
	Umrech = UmReFaktor;
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
	QString point[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	if (doc->Einheit > 3)
		doc->Einheit = 0;
	ein = point[doc->Einheit];
	int val = doc->Einheit == 1 ? 1000 : 10000;
	switch (doc->Einheit)
	{
	case 1:
	case 2:
		Xpos->setDecimals(val);
		Ypos->setDecimals(val);
		Width->setDecimals(val);
		Height->setDecimals(val);
		LXpos->setDecimals(val);
		LYpos->setDecimals(val);
		DGap->setDecimals(val);
		DLeft->setDecimals(val);
		DTop->setDecimals(val);
		DBottom->setDecimals(val);
		DRight->setDecimals(val);
		RoundRect->setDecimals(val);
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
	Xpos->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	Xpos->setMinValue( -3000 * UmReFaktor );
	Ypos->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	Ypos->setMinValue( -3000 * UmReFaktor );
	Width->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	Width->setMinValue( 1 * UmReFaktor );
	Height->setMaxValue( (QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor );
	Height->setMinValue( 1 * UmReFaktor );
	Xpos->setValue(oldX * UmReFaktor);
	Ypos->setValue(oldY * UmReFaktor);
	Width->setValue(oldW * UmReFaktor);
	Height->setValue(oldH * UmReFaktor);
	LXpos->setMaxValue((QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor);
	LYpos->setMaxValue((QMAX(doc->PageB, doc->PageH) + QMAX(doc->PageB, doc->PageH) * 0.1) * UmReFaktor);
	LXpos->setValue(oldLX * UmReFaktor);
	LYpos->setValue(oldLY * UmReFaktor);
	DGap->setMaxValue(oldGM * UmReFaktor);
	DGap->setValue(oldG * UmReFaktor);
	DTop->setMaxValue( 300 );
	DLeft->setMaxValue( 300 );
	DRight->setMaxValue( 300 );
	DBottom->setMaxValue( 300 );
	DLeft->setValue(oldDL * UmReFaktor);
	DTop->setValue(oldDT * UmReFaktor);
	DBottom->setValue(oldDB * UmReFaktor);
	DRight->setValue(oldDR * UmReFaktor);
	RoundRect->setMaxValue(oldRM * UmReFaktor);
	RoundRect->setValue(oldRR * UmReFaktor);
	HaveItem = tmp;
}

void Mpalette::setLevel(uint l)
{
	QString tm;
	LevelTxt->setText(tm.setNum(l));
}

void Mpalette::setXY(double x, double y)
{
	if (ScApp->ScriptRunning)
		return;
	disconnect(Xpos, SIGNAL(valueChanged(int)), this, SLOT(NewX()));
	disconnect(Ypos, SIGNAL(valueChanged(int)), this, SLOT(NewY()));
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
	Xpos->setValue(inX*UmReFaktor);
	Ypos->setValue(inY*UmReFaktor);
	if ((LMode) && (tmp))
		setBH(CurItem->Width, CurItem->Height);
	HaveItem = tmp;
	connect(Xpos, SIGNAL(valueChanged(int)), this, SLOT(NewX()));
	connect(Ypos, SIGNAL(valueChanged(int)), this, SLOT(NewY()));
}

void Mpalette::setBH(double x, double y)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	QWMatrix ma;
	QPoint dp;
	if ((LMode) && (CurItem->PType == 5))
	{
		ma.translate(static_cast<double>(Xpos->value()) / UmReFaktor, static_cast<double>(Ypos->value()) / UmReFaktor);
		ma.rotate(static_cast<double>(Rot->value())*(-1));
		dp = ma * QPoint(static_cast<int>(x), static_cast<int>(y));
		Width->setValue(dp.x()*UmReFaktor);
		Height->setValue(dp.y()*UmReFaktor);
	}
	else
	{
		RoundRect->setMaxValue(QMIN(x, y)/2*UmReFaktor);
		Width->setValue(x*UmReFaktor);
		Height->setValue(y*UmReFaktor);
	}
	HaveItem = tmp;
}

void Mpalette::setR(double r)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	double rr = r;
	if (r > 0)
		rr = 360 - rr;
	HaveItem = false;
	Rot->setValue(fabs(rr));
	HaveItem = tmp;
}

void Mpalette::setRR(double r)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	RoundRect->setValue(r*UmReFaktor);
	HaveItem = tmp;
}

void Mpalette::setCols(int r, double g)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	DCol->setValue(r);
	DGap->setValue(g*UmReFaktor);
	if (tmp)
	{
		DCol->setMaxValue(QMAX(qRound(CurItem->Width / QMAX(CurItem->ColGap, 10.0)), 1));
		if (Text14b->getState())
		{
			DGap->setMaxValue(QMAX((CurItem->Width / CurItem->Cols - CurItem->Extra - CurItem->RExtra)*UmReFaktor, 0));
			DGap->setValue(CurItem->ColGap*UmReFaktor);
		}
		else
		{
			double lineCorr;
			if (CurItem->Pcolor2 != "None")
				lineCorr = CurItem->Pwidth;
			else
				lineCorr = 0;
			double ColWidth = (CurItem->Width - (CurItem->ColGap * (CurItem->Cols - 1)) - CurItem->Extra - CurItem->RExtra - lineCorr) / CurItem->Cols;
			DGap->setMaxValue(QMAX((CurItem->Width / CurItem->Cols)*UmReFaktor, 0));
			DGap->setValue(ColWidth*UmReFaktor);
		}
	}
	DCol->setMinValue(1);
	DGap->setMinValue(0);
	HaveItem = tmp;
}

void Mpalette::setLsp(double r)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	LineSp->setValue(r);
	HaveItem = tmp;
}

void Mpalette::setDvals(double left, double top, double bottom, double right)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	DLeft->setValue(left*UmReFaktor);
	DTop->setValue(top*UmReFaktor);
	DBottom->setValue(bottom*UmReFaktor);
	DRight->setValue(right*UmReFaktor);
	HaveItem = tmp;
}

void Mpalette::setSize(int s)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	Size->setValue(s / 10.0);
	HaveItem = tmp;
}

void Mpalette::setExtra(double e)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	Extra->setValue(e);
	HaveItem = tmp;
}

void Mpalette::ChangeScaling()
{
	if (ScApp->ScriptRunning)
		return;
	if (FreeScale == sender())
	{
		FrameScale->setChecked(false);
		FreeScale->setChecked(true);
		Aspect->setEnabled(false);
		LXpos->setEnabled(true);
		LYpos->setEnabled(true);
		ScaleX->setEnabled(true);
		ScaleY->setEnabled(true);
	}
	if (FrameScale == sender())
	{
		FrameScale->setChecked(true);
		FreeScale->setChecked(false);
		Aspect->setEnabled(true);
		LXpos->setEnabled(false);
		LYpos->setEnabled(false);
		ScaleX->setEnabled(false);
		ScaleY->setEnabled(false);
	}
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->ScaleType = FreeScale->isChecked();
		CurItem->AspectRatio = Aspect->isChecked();
		if (!CurItem->ScaleType)
			doc->ActPage->AdjustPictScale(CurItem);
		doc->ActPage->RefreshItem(CurItem);
		emit UpdtGui(2);
		emit DocChanged();
		setFocus();
	}
}

void Mpalette::setLvalue(double scx, double scy, double x, double y)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	LXpos->setValue(x * UmReFaktor);
	LYpos->setValue(y * UmReFaktor);
	ScaleX->setValue(scx * 100);
	ScaleY->setValue(scy * 100);
	HaveItem = tmp;
}

void Mpalette::setSvalue(double s)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	LSize->setValue(s);
	HaveItem = tmp;
}

void Mpalette::setLIvalue(PenStyle p, PenCapStyle pc, PenJoinStyle pj)
{
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
	StrokeIcon->setEnabled(false);
	TxStroke->setEnabled(false);
	PM1->setEnabled(false);
	SeStyle->setStyle(s);
	if (s & 4)
	{
		StrokeIcon->setEnabled(true);
		TxStroke->setEnabled(true);
		PM1->setEnabled(true);
	}
}

void Mpalette::setAli(int e)
{
	if (ScApp->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	if (e < 5)
	{
		GroupAlign->setEnabled(true);
		GroupAlign->setStyle(e);
	}
	else
		GroupAlign->setEnabled(false);
	Spal->setFormat(e);
	HaveItem = tmp;
}

void Mpalette::setTScale(int e)
{
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
	double x,y,w,h, gx, gy, gh, gw, base;
	QWMatrix ma;
	x = Xpos->value() / UmReFaktor;
	y = Ypos->value() / UmReFaktor;
	w = Width->value() / UmReFaktor;
	h = Height->value() / UmReFaktor;
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
	if (ScApp->ScriptRunning)
		return;
	double x,y,w,h, gx, gy, gh, gw, base;
	QWMatrix ma;
	x = Xpos->value() / UmReFaktor;
	y = Ypos->value() / UmReFaktor;
	w = Width->value() / UmReFaktor;
	h = Height->value() / UmReFaktor;
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
				doc->ActPage->MoveItem(0, y - base, CurItem, true);
			}
		}
		emit DocChanged();
	}
}

void Mpalette::NewW()
{
	if (ScApp->ScriptRunning)
		return;
	double x,y,w,h, gx, gy, gh, gw;
	x = Xpos->value() / UmReFaktor;
	y = Ypos->value() / UmReFaktor;
	w = Width->value() / UmReFaktor;
	h = Height->value() / UmReFaktor;
	if ((HaveDoc) && (HaveItem))
	{
		if (doc->ActPage->GroupSel)
		{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			if (Kette2->isOn())
			{
				doc->ActPage->HowTo = 1;
				doc->ActPage->scaleGroup(w / gw, w / gw);
				setBH(w, (w / gw) * gh);
			}
			else
			{
				doc->ActPage->HowTo = 6;
				doc->ActPage->scaleGroup(w / gw, 1.0);
			}
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
			{
				if (CurItem->isTableItem)
				{
					int rmo = doc->RotMode;
					doc->RotMode = 0;
					double dist = w - CurItem->Width;
					double oldW = CurItem->Width;
					PageItem* bb2;
					PageItem* bb = CurItem;
					while (bb->TopLink != 0)
					{
						bb = bb->TopLink;
					}
					while (bb->BottomLink != 0)
					{
						bb2 = bb;
						while (bb2->RightLink != 0)
						{
							doc->ActPage->MoveRotated(bb2->RightLink, FPoint(dist, 0), true);
							bb2 = bb2->RightLink;
						}
						doc->ActPage->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr, true);
						bb = bb->BottomLink;
					}
					bb2 = bb;
					while (bb2->RightLink != 0)
					{
						doc->ActPage->MoveRotated(bb2->RightLink, FPoint(dist, 0), true);
						bb2 = bb2->RightLink;
					}
					doc->ActPage->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr, true);
					doc->RotMode = rmo;
					if (Kette2->isOn())
					{
						Kette2->setOn(false);
						setBH(w, (w / oldW) * CurItem->Height);
						NewH();
						Kette2->setOn(true);
					}
				}
				else
				{
					if (Kette2->isOn())
					{
						setBH(w, (w / CurItem->Width) * CurItem->Height);
						doc->ActPage->SizeItem(w, (w / CurItem->Width) * CurItem->Height, CurItem->ItemNr, true);
					}
					else
						doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
				}
			}
			emit DocChanged();
		}
	}
}

void Mpalette::NewH()
{
	if (ScApp->ScriptRunning)
		return;
	double x,y,w,h, gx, gy, gh, gw;
	x = Xpos->value() / UmReFaktor;
	y = Ypos->value() / UmReFaktor;
	w = Width->value() / UmReFaktor;
	h = Height->value() / UmReFaktor;
	if ((HaveDoc) && (HaveItem))
	{
		if (doc->ActPage->GroupSel)
		{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			if (Kette2->isOn())
			{
				doc->ActPage->HowTo = 1;
				doc->ActPage->scaleGroup(h / gh, h / gh);
				setBH((h / gh) * gw, h);
			}
			else
			{
				doc->ActPage->HowTo = 5;
				doc->ActPage->scaleGroup(1.0, h / gh);
			}
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
			{
				if (CurItem->isTableItem)
				{
					int rmo = doc->RotMode;
					doc->RotMode = 0;
					double dist = h - CurItem->Height;
					double oldH = CurItem->Height;
					PageItem* bb2;
					PageItem* bb = CurItem;
					while (bb->LeftLink != 0)
					{
						bb = bb->LeftLink;
					}
					while (bb->RightLink != 0)
					{
						bb2 = bb;
						while (bb2->BottomLink != 0)
						{
							doc->ActPage->MoveRotated(bb2->BottomLink, FPoint(0, dist), true);
							bb2 = bb2->BottomLink;
						}
						doc->ActPage->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr, true);
						bb = bb->RightLink;
					}
					bb2 = bb;
					while (bb2->BottomLink != 0)
					{
						doc->ActPage->MoveRotated(bb2->BottomLink, FPoint(0, dist), true);
						bb2 = bb2->BottomLink;
					}
					doc->ActPage->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr, true);
					doc->RotMode = rmo;
					if (Kette2->isOn())
					{
						Kette2->setOn(false);
						setBH((h / oldH) * CurItem->Width, h);
						NewW();
						Kette2->setOn(true);
					}
				}
				else
				{
					if (Kette2->isOn())
					{
						setBH((h / CurItem->Height) * CurItem->Width, h);
						doc->ActPage->SizeItem((h / CurItem->Height) * CurItem->Width, h, CurItem->ItemNr, true);
					}
					else
						doc->ActPage->SizeItem(CurItem->Width, h, CurItem->ItemNr, true);
				}
			}
		}
		emit DocChanged();
	}
}

void Mpalette::NewR()
{
	if (ScApp->ScriptRunning)
		return;
	double gx, gy, gh, gw;
	if ((HaveDoc) && (HaveItem))
	{
		if (doc->ActPage->GroupSel)
		{
			doc->ActPage->RotateGroup((Rot->value() - RoVal)*(-1));
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			setXY(gx, gy);
		}
		else
			doc->ActPage->RotateItem(Rot->value()*(-1), CurItem->ItemNr);
		emit DocChanged();
		RoVal = Rot->value();
	}
}

void Mpalette::NewRR()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->RadRect = RoundRect->value() / UmReFaktor;
		doc->ActPage->SetFrameRounded();
		emit DocChanged();
	}
}

void Mpalette::NewLsp()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		doc->ActPage->ChLineSpa(LineSp->value());
		emit DocChanged();
	}
}

void Mpalette::HandleGapSwitch()
{
	if ((HaveDoc) && (HaveItem))
	{
		setCols(CurItem->Cols, CurItem->ColGap);
		QToolTip::remove(DGap);
		if (Text14b->getState())
			QToolTip::add( DGap, tr( "Distance between columns" ) );
		else
			QToolTip::add( DGap, tr( "Column width" ) );
	}
}

void Mpalette::NewCols()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->Cols = DCol->value();
		setCols(CurItem->Cols, CurItem->ColGap);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::NewGap()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if (Text14b->getState())
			CurItem->ColGap = DGap->value() / UmReFaktor;
		else
		{
			double lineCorr;
			if (CurItem->Pcolor2 != "None")
				lineCorr = CurItem->Pwidth;
			else
				lineCorr = 0;
			double newWidth = DGap->value() / UmReFaktor;
			double newGap = QMAX(((CurItem->Width - CurItem->Extra - CurItem->RExtra - lineCorr) - (newWidth * CurItem->Cols)) / (CurItem->Cols - 1), 0);
			CurItem->ColGap = newGap;
		}
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::NewSize()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		doc->ActPage->chFSize(qRound(Size->value()*10.0));
		emit DocChanged();
	}
}

void Mpalette::NewExtra()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->HasSel) || (doc->AppMode == 1))
		{
			doc->ActPage->chKerning(Extra->value());
			emit DocChanged();
		}
		else
		{
			if (uint(CurItem->CPos) != CurItem->Ptext.count())
			{
				CurItem->Ptext.at(CurItem->CPos)->cextra = Extra->value();
				CurItem->Dirty = true;
				doc->ActPage->RefreshItem(CurItem);
				emit DocChanged();
			}
		}
	}
}

void Mpalette::NewLocalXY()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		doc->ActPage->ChLocalXY(LXpos->value() / UmReFaktor, LYpos->value() / UmReFaktor);
		emit DocChanged();
	}
}

void Mpalette::NewLocalSC()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		doc->ActPage->ChLocalSc(ScaleX->value() / 100.0, ScaleY->value() / 100.0);
		emit DocChanged();
	}
}

void Mpalette::NewLS()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		doc->ActPage->ChLineWidth(LSize->value());
		emit DocChanged();
	}
}

void Mpalette::NewLSty()
{
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
	if (Kette->isOn())
	{
		ScaleY->setValue(ScaleX->value());
		NewLocalSC();
	}
}

void Mpalette::HChange()
{
	if (Kette->isOn())
		ScaleY->setValue(ScaleX->value());
	NewLocalSC();
}

void Mpalette::VChange()
{
	if (Kette->isOn())
		ScaleX->setValue(ScaleY->value());
	NewLocalSC();
}

void Mpalette::DoFlipH()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->PType == 2) || (CurItem->PType == 4))
			doc->ActPage->FlipImageH();
		else
			doc->ActPage->MirrorPolyH();
		emit DocChanged();
	}
}

void Mpalette::DoFlipV()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->PType == 2) || (CurItem->PType == 4))
			doc->ActPage->FlipImageV();
		else
			doc->ActPage->MirrorPolyV();
		emit DocChanged();
	}
}

void Mpalette::NewAli(int a)
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if (doc->CurrentABStil < 5)
			emit NewAbStyle(a);
	}
}

void Mpalette::setTypeStyle(int s)
{
	if (ScApp->ScriptRunning)
		return;
	emit Stellung(s);
	emit BackHome();
}

void Mpalette::DoLower()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		doc->ActPage->LowerItem();
		emit DocChanged();
	}
}

void Mpalette::DoRaise()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		doc->ActPage->RaiseItem();
		emit DocChanged();
	}
}

void Mpalette::DoFront()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		doc->ActPage->ToFront();
		emit DocChanged();
	}
}

void Mpalette::DoBack()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		doc->ActPage->ToBack();
		emit DocChanged();
	}
}

void Mpalette::NewRotMode(int m)
{
	if (ScApp->ScriptRunning)
		return;
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
			Xpos->setValue(inX*UmReFaktor);
			Ypos->setValue(inY*UmReFaktor);
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
			Xpos->setValue(inX*UmReFaktor);
			Ypos->setValue(inY*UmReFaktor);
		}
		HaveItem = true;
		doc->RotMode = m;
	}
}

void Mpalette::DoFlow()
{
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->Textflow2 = Textflow2->isChecked();
		doc->ActPage->update();
		emit DocChanged();
	}
}

void Mpalette::MakeIrre(int f, int c, double *vals)
{
	if (ScApp->ScriptRunning)
		return;
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
			CurItem->FrameType = f+2;
			break;
		}
		ScApp->SCustom->setPixmap(ScApp->SCustom->getIconPixmap(f));
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
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->Extra = DLeft->value() / UmReFaktor;
		CurItem->TExtra = DTop->value() / UmReFaktor;
		CurItem->BExtra = DBottom->value() / UmReFaktor;
		CurItem->RExtra = DRight->value() / UmReFaktor;
		setCols(CurItem->Cols, CurItem->ColGap);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::NewTFont(int c)
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
		emit NewTF(Fonts->text(c));
}

void Mpalette::DoRevert()
{
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
	disconnect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	StyledLine->clear();
	if (dd != 0)
	{
		QMap<QString,multiLine>::Iterator it;
		for (it = dd->MLineStyles.begin(); it != dd->MLineStyles.end(); ++it)
		{
			QPixmap pm = QPixmap(37, 37);
			pm.fill(white);
			QPainter p;
			p.begin(&pm);
			QColor tmpf;
			int h, s, v, sneu;
			multiLine ml = it.data();
			for (int its = ml.size()-1; its > -1; its--)
			{
				dd->PageColors[ml[its].Color].getRGBColor().rgb(&h, &s, &v);
				if ((h == s) && (s == v))
				{
					dd->PageColors[ml[its].Color].getRGBColor().hsv(&h, &s, &v);
					sneu = 255 - ((255 - v) * ml[its].Shade / 100);
					tmpf.setHsv(h, s, sneu);
				}
				else
				{
					dd->PageColors[ml[its].Color].getRGBColor().hsv(&h, &s, &v);
					sneu = s * ml[its].Shade / 100;
					tmpf.setHsv(h, sneu, v);
				}
				p.setPen(QPen(tmpf,
								QMAX(static_cast<int>(ml[its].Width), 1),
								 static_cast<PenStyle>(ml[its].Dash),
								 static_cast<PenCapStyle>(ml[its].LineEnd),
								 static_cast<PenJoinStyle>(ml[its].LineJoin)));
				p.drawLine(0, 18, 37, 18);
				}
			p.end();
			StyledLine->insertItem(pm, it.key());
		}
		/* PFJ - 29.02.04 - Changed from TRUE to true */
		StyledLine->sort( true );
		StyledLine->insertItem( tr("No Style"), 0);
		StyledLine->setSelected(StyledLine->currentItem(), false);
	}
	connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
}

void Mpalette::SetSTline(QListBoxItem *c)
{
	if (ScApp->ScriptRunning)
		return;
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
	if (ScApp->ScriptRunning)
		return;
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
	TxFill->listBox()->setMinimumWidth(TxFill->listBox()->maxItemWidth()+24);
	TxStroke->listBox()->setMinimumWidth(TxStroke->listBox()->maxItemWidth()+24);
}

void Mpalette::updateCmsList()
{
	if (ScApp->ScriptRunning)
		return;
	if (HaveDoc)
	{
		if ((CMSavail) && (doc->CMSSettings.CMSinUse))
			GroupBoxCM->show();
		else
		{
			GroupBoxCM->hide();
			return;
		}
		disconnect(InputP, SIGNAL(activated(const QString&)), this,
		           SLOT(ChProf(const QString&)));
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
	if (ScApp->ScriptRunning)
		return;
	/* PFJ - 29.02.04 - Moved bool into if scope */
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->IProfile = InputP->currentText();
		/* PFJ - 29.02.04 - re-arranged the initialisation of EmbedP */
		bool EmbedP = prn.startsWith("Embedded") ? true : false;
		CurItem->UseEmbedded = EmbedP;
		doc->ActPage->LoadPict(CurItem->Pfile, CurItem->ItemNr);
		doc->ActPage->RefreshItem(CurItem);
	}
}

void Mpalette::ChIntent()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->IRender = MonitorI->currentItem();
		doc->ActPage->LoadPict(CurItem->Pfile, CurItem->ItemNr);
		doc->ActPage->RefreshItem(CurItem);
	}
}

void Mpalette::ShowCMS()
{
	if (ScApp->ScriptRunning)
		return;
	if (HaveItem)
	{
		HaveItem = false;
		updateCmsList();
		HaveItem = true;
	}
	else
	{
		if ((CMSavail) && (doc->CMSSettings.CMSinUse))
			GroupBoxCM->show();
		else
			GroupBoxCM->hide();
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

void Mpalette::setActShade()
{
	if (ScApp->ScriptRunning)
		return;
	int b;
	if (PM1 == sender())
	{
		b = PM1->getValue();
		doc->ActPage->ItemTextPenS(b);
		doc->CurrTextFillSh = b;
	}
	else
	{
		b = PM2->getValue();
		doc->ActPage->ItemTextBrushS(b);
		doc->CurrTextStrokeSh = b;
	}
	emit DocChanged();
}

void Mpalette::setActFarben(QString p, QString b, int shp, int shb)
{
	if (ScApp->ScriptRunning)
		return;
	CListe::Iterator it;
	int c = 0;
	PM2->setValue(shb);
	PM1->setValue(shp);
	if ((b != "None") && (b != ""))
	{
		c++;
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			if (it.key() == b)
				break;
			c++;
		}
	}
	TxFill->setCurrentItem(c);
	c = 0;
	if ((p != "None") && (p != ""))
	{
		c++;
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
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		for ( uint a = 0; a < doc->ActPage->SelItem.count(); ++a)
		{
			doc->ActPage->SelItem.at(a)->Locked = Locked->isOn();
			doc->ActPage->RefreshItem(doc->ActPage->SelItem.at(a));
		}
		bool setter = Locked->isOn() ? false : true;
		Xpos->setEnabled(setter);
		Ypos->setEnabled(setter);
		Width->setEnabled(setter);
		Height->setEnabled(setter);
		Rot->setEnabled(setter);
		RoundRect->setEnabled(setter);
		EditShape->setEnabled(setter);
		ShapeGroup->setEnabled(setter);
		LayerGroup->setEnabled(setter);
		emit DocChanged();
	}
}

void Mpalette::handlePrint()
{
	if ((HaveDoc) && (HaveItem))
	{
		for ( uint a = 0; a < doc->ActPage->SelItem.count(); ++a)
		{
			doc->ActPage->SelItem.at(a)->isPrintable = !NoPrint->isOn();
		}
		emit DocChanged();
	}
}

void Mpalette::handleResize()
{
	if ((HaveDoc) && (HaveItem))
	{
		for ( uint a = 0; a < doc->ActPage->SelItem.count(); ++a)
		{
			doc->ActPage->SelItem.at(a)->LockRes = NoResize->isOn();
			doc->ActPage->RefreshItem(doc->ActPage->SelItem.at(a));
		}
		Width->setEnabled(!NoResize->isOn());
		Height->setEnabled(!NoResize->isOn());
		emit DocChanged();
	}
}

void Mpalette::ToggleFlow()
{
	Textflow2->setEnabled(Textflow->isChecked());
}

void Mpalette::handlePathLine()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->PoShow = NormText2->isChecked();
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::handlePathDist()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->Extra = Dist->value();
		doc->ActPage->UpdatePolyClip(CurItem);
		doc->ActPage->AdjustItemSize(CurItem);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::handlePathOffs()
{
	if (ScApp->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->BaseOffs = -LineW->value();
		doc->ActPage->UpdatePolyClip(CurItem);
		doc->ActPage->AdjustItemSize(CurItem);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::NewName()
{
	if (ScApp->ScriptRunning)
		return;
	QString NameOld = CurItem->AnName;
	QString NameNew = NameEdit->text();
	if (NameNew == "")
	{
		NameEdit->setText(NameOld);
		return;
	}
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
	if (ScApp->ScriptRunning)
		return;
	LangCombo->clear();
	QMap<QString,QString>::Iterator it;
	for (it = Sprachen.begin(); it != Sprachen.end(); ++it)
		LangCombo->insertItem(it.key());
	LangCombo->listBox()->setMinimumWidth(LangCombo->listBox()->maxItemWidth()+24);
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
		double lineCorr;
		if (CurItem->Pcolor2 != "None")
			lineCorr = CurItem->Pwidth;
		else
			lineCorr = 0;
		double ColWidth;
		if (CurItem->Cols > 1)
			ColWidth = (CurItem->Width - (CurItem->ColGap * (CurItem->Cols - 1)) - CurItem->Extra - CurItem->RExtra - lineCorr) / CurItem->Cols;
		else
			ColWidth = CurItem->Width;
		TabManager *dia = new TabManager(this, doc->Einheit, CurItem->TabValues, ColWidth);
		if (dia->exec())
		{
			CurItem->TabValues = dia->tmpTab;
			doc->ActPage->RefreshItem(CurItem);
			emit DocChanged();
		}
		delete dia;
	}
}

void Mpalette::HandleTLines()
{
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->TopLine = TopLine->isChecked();
		CurItem->LeftLine = LeftLine->isChecked();
		CurItem->RightLine = RightLine->isChecked();
		CurItem->BottomLine = BottomLine->isChecked();
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
	}
}
