/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "mpalette.h"
#include "mpalette.moc"

#include <cmath>
#include <qpoint.h>
#include <qwmatrix.h>
#include <qmessagebox.h>
#include <qradiobutton.h>
#include <qobjectlist.h>

#include "arrowchooser.h"
#include "autoform.h"
#include "commonstrings.h"
#include "cpalette.h"
#include "sccombobox.h"
#include "scfonts.h"
#include "scribus.h"
#include "scribusview.h"
#include "selection.h"
#include "spalette.h"
#include "styleselect.h"
#include "tabmanager.h"
#include "units.h"
#include "undomanager.h"
#include "util.h"

using namespace std;



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

void NameWidget::focusOutEvent(QFocusEvent *e)
{
	emit Leaved();
	QLineEdit::focusOutEvent(e);
}

Mpalette::Mpalette( QWidget* parent) : ScrPaletteBase( parent, "PropertiesPalette", false, 0)
{
	HaveDoc = false;
	HaveItem = false;
	RoVal = 0;
	Umrech = 1.0;
	setIcon( loadIcon("AppIcon.png") );
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );

	_userActionOn = false;
	userActionSniffer = new UserActionSniffer();
	connect(userActionSniffer, SIGNAL(actionStart()), this, SLOT(mspinboxStartUserAction()));
	connect(userActionSniffer, SIGNAL(actionEnd()), this, SLOT(mspinboxFinishUserAction()));

	MpalLayout = new QVBoxLayout( this, 5, 1, "MpalLayout");
	setOrientation(Qt::Vertical);
	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);

	TabStack = new QToolBox( this, "TabStack" );

	page = new QWidget( TabStack, "page" );
	pageLayout = new QVBoxLayout( page, 0, 5, "pageLayout");

	NameGroup = new QGroupBox( "Name", page, "NameGroup" );
	NameGroup->setColumnLayout(0, Qt::Vertical );
	NameGroup->layout()->setSpacing( 4 );
	NameGroup->layout()->setMargin( 5 );
	NameGroupLayout = new QHBoxLayout( NameGroup->layout() );
	NameGroupLayout->setAlignment( Qt::AlignTop );
	NameEdit = new NameWidget(NameGroup);
	NameGroupLayout->addWidget( NameEdit );
	pageLayout->addWidget( NameGroup );

	GeoGroup = new QGroupBox( "Geometry", page, "GeoGroup" );
	GeoGroup->setColumnLayout(0, Qt::Vertical );
	GeoGroup->layout()->setSpacing( 4 );
	GeoGroup->layout()->setMargin( 5 );
	GeoGroupLayout = new QGridLayout( GeoGroup->layout() );
	GeoGroupLayout->setAlignment( Qt::AlignTop );

	Xpos = new MSpinBox( -3000, 3000, GeoGroup, 2 );
	installSniffer(Xpos);
	GeoGroupLayout->addWidget( Xpos, 0, 1 );
	Ypos = new MSpinBox( -3000, 3000, GeoGroup, 2 );
	installSniffer(Ypos);
	GeoGroupLayout->addWidget( Ypos, 1, 1 );
	Width = new MSpinBox( GeoGroup, 2 );
	installSniffer(Width);
	GeoGroupLayout->addWidget( Width, 2, 1 );
	Height = new MSpinBox( GeoGroup, 2 );
	installSniffer(Height);
	GeoGroupLayout->addWidget( Height, 3, 1 );

	xposLabel = new QLabel( Xpos, "&X-Pos:", GeoGroup, "xposLabel" );
	GeoGroupLayout->addWidget( xposLabel, 0, 0 );
	yposLabel = new QLabel( Ypos, "&Y-Pos:", GeoGroup, "yposLabel" );
	GeoGroupLayout->addWidget( yposLabel, 1, 0 );
	widthLabel = new QLabel( Width, "&Width:", GeoGroup, "widthLabel" );
	GeoGroupLayout->addWidget( widthLabel, 2, 0 );
	heightLabel = new QLabel( Height, "&Height:", GeoGroup, "heightLabel" );
	GeoGroupLayout->addWidget( heightLabel, 3, 0 );

	keepFrameWHRatioButton = new LinkButton( GeoGroup );
	keepFrameWHRatioButton->setToggleButton( true );
	keepFrameWHRatioButton->setAutoRaise( true );
	keepFrameWHRatioButton->setMaximumSize( QSize( 15, 32767 ) );
	keepFrameWHRatioButton->setOn(true);
	GeoGroupLayout->addMultiCellWidget( keepFrameWHRatioButton, 2, 3, 2, 2 );
	Rot = new MSpinBox( GeoGroup, 2);
	Rot->setWrapping( true );
	installSniffer(Rot);
	rotationLabel = new QLabel( Rot, "&Rotation:", GeoGroup, "rotationLabel" );
	GeoGroupLayout->addWidget( rotationLabel, 4, 0 );
	GeoGroupLayout->addWidget( Rot, 4, 1 );
	basepointLabel = new QLabel( "Basepoint:", GeoGroup, "basepointLabel" );
	GeoGroupLayout->addWidget( basepointLabel, 5, 0 );
	RotationGroup = new QButtonGroup( "", GeoGroup, "RotationGroup" );
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

	LayerGroup = new QGroupBox( "Level", page, "LayerGroup" );
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
	LevelTxt = new QLabel( "  0", LayerGroup, "LevelTxt" );
	LevelTxt->setAlignment( Qt::AlignCenter );
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

	pageLayout->addLayout( layout60 );

	QSpacerItem* spacer13 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout->addItem( spacer13 );
	idXYZItem=TabStack->addItem( page, "X, Y, &Z" );

	page_2 = new QWidget( TabStack, "page_2" );
	pageLayout_2 = new QVBoxLayout( page_2, 0, 5, "pageLayout_2");

	ShapeGroup = new QButtonGroup( "", page_2, "ShapeGroup" );
	ShapeGroup->setFrameShape( QButtonGroup::NoFrame );
	ShapeGroup->setExclusive( true );
	ShapeGroup->setColumnLayout(0, Qt::Vertical );
	ShapeGroup->layout()->setSpacing( 2 );
	ShapeGroup->layout()->setMargin( 0 );
	ShapeGroupLayout = new QHBoxLayout( ShapeGroup->layout() );
	ShapeGroupLayout->setAlignment( Qt::AlignTop );
	SRect = new QLabel( "Shape:", ShapeGroup, "SRect" );
	ShapeGroupLayout->addWidget( SRect );
	SCustom = new Autoforms( ShapeGroup );
	ShapeGroupLayout->addWidget( SCustom );
	pageLayout_2->addWidget( ShapeGroup );

	EditShape = new QToolButton( page_2, "EditShape" );
	pageLayout_2->addWidget( EditShape );

	Layout13 = new QHBoxLayout( 0, 0, 2, "Layout13");

	RoundRect = new MSpinBox( page_2, 1 );
	rndcornersLabel = new QLabel( RoundRect, "R&ound\nCorners:", page_2, "rndcornersLabel" );
	Layout13->addWidget( rndcornersLabel );
	Layout13->addWidget( RoundRect );
	pageLayout_2->addLayout( Layout13 );

	TabStack2 = new QWidgetStack( page_2, "TabStack2" );
	TabStack2->setFrameShape( QWidgetStack::NoFrame );

	page_2a = new QWidget( TabStack2, "page" );
	pageLayout_2a = new QVBoxLayout( page_2a, 0, 5, "pageLayout_2");
	Distance = new QGroupBox( "Distance of Text", page_2a, "Distance" );
	Distance->setColumnLayout(0, Qt::Vertical );
	Distance->layout()->setSpacing( 2 );
	Distance->layout()->setMargin( 5 );
	DistanceLayout = new QGridLayout( Distance->layout() );
	DistanceLayout->setAlignment( Qt::AlignTop );

	DCol = new QSpinBox(Distance, "Cols" );
    DCol->setMaxValue( 3000 );
    DCol->setMinValue( 1 );
	columnsLabel = new QLabel( DCol, "Colu&mns:", Distance, "columnsLabel" );
	DistanceLayout->addWidget( columnsLabel, 0, 0 );
	DistanceLayout->addWidget( DCol, 0, 1 );

	dGap = new MSpinBox( 0, 300, Distance, 1 );
	colgapLabel = new LabelButton( Distance, "&Gap:", "&Width:");
	colgapLabel->setBuddy(dGap);
	DistanceLayout->addWidget( colgapLabel, 1, 0 );
	DistanceLayout->addWidget( dGap, 1, 1 );

	DTop = new MSpinBox( 0, 300, Distance, 1 );
	topLabel = new QLabel( DTop, "To&p:", Distance, "topLabel" );
	DistanceLayout->addWidget( topLabel, 2, 0 );
	DistanceLayout->addWidget( DTop, 2, 1 );

	DBottom = new MSpinBox( 0, 300, Distance, 1 );
	bottomLabel = new QLabel( DBottom, "&Bottom:", Distance, "bottomLabel" );
	DistanceLayout->addWidget( bottomLabel, 3, 0 );
	DistanceLayout->addWidget( DBottom, 3, 1 );

	DLeft = new MSpinBox( 0, 300, Distance, 1 );
	leftLabel = new QLabel( DLeft, "&Left:", Distance, "leftLabel" );
	DistanceLayout->addWidget( leftLabel, 4, 0 );
	DistanceLayout->addWidget( DLeft, 4, 1 );

	DRight = new MSpinBox( 0, 300, Distance, 1 );
	rightLabel = new QLabel( DRight, "&Right:", Distance, "rightLabel" );
	DistanceLayout->addWidget( rightLabel, 5, 0 );
	DistanceLayout->addWidget( DRight, 5, 1 );

	TabsButton = new QToolButton( Distance, "T&abulators..." );
	DistanceLayout->addMultiCellWidget( TabsButton, 6, 6, 0, 1 );
	pageLayout_2a->addWidget(Distance);
	TabStack2->addWidget( page_2a, 0 );

	page_2b = new QWidget( TabStack2, "page" );
	pageLayout_2b = new QVBoxLayout( page_2b, 0, 5, "pageLayout_2");
	Distance2 = new QGroupBox( "Path Text Properties", page_2b, "Distance" );
	Distance2->setColumnLayout(0, Qt::Vertical );
	Distance2->layout()->setSpacing( 2 );
	Distance2->layout()->setMargin( 5 );
	DistanceLayout2 = new QGridLayout( Distance2->layout() );
	DistanceLayout2->setAlignment( Qt::AlignTop );
	showcurveCheckBox = new QCheckBox( Distance2, "showcurveCheckBox" );
	showcurveCheckBox->setText( "Show Curve" );
	DistanceLayout2->addMultiCellWidget( showcurveCheckBox, 0, 0, 0, 1 );
	startoffsetLabel = new QLabel( "Start Offset:", Distance2, "startoffsetLabel" );
	DistanceLayout2->addWidget( startoffsetLabel, 1, 0);
	Dist = new MSpinBox( 0, 30000, Distance2, 1 );
	Dist->setLineStep(10);
	DistanceLayout2->addWidget( Dist, 1, 1);

	distfromcurveLabel = new QLabel( "Distance from Curve:", Distance2, "distfromcurveLabel" );
	DistanceLayout2->addWidget( distfromcurveLabel, 2, 0);
	LineW = new MSpinBox( -300, 300, Distance2, 1 );
	LineW->setLineStep(10);
	DistanceLayout2->addWidget( LineW, 2, 1);
	pageLayout_2b->addWidget(Distance2);
	TabStack2->addWidget( page_2b, 1 );

	pageLayout_2->addWidget( TabStack2 );

	textFlowsAroundFrame = new QButtonGroup( page_2, "textFlowsAroundFrame" );
	textFlowsAroundFrame->setColumnLayout(0, Qt::Vertical );
	textFlowsAroundFrame->layout()->setSpacing( 5 );
	textFlowsAroundFrame->layout()->setMargin( 10 );
	textFlowsAroundFrameLayout = new QVBoxLayout( textFlowsAroundFrame->layout() );
	textFlowsAroundFrameLayout->setAlignment( Qt::AlignTop );
	textFlowsAroundFrame->setCheckable( true );
	textFlowUsesBoundingBox = new QCheckBox( "Use &Bounding Box", textFlowsAroundFrame, "textFlowUsesBoundingBox" );
	textFlowsAroundFrameLayout->addWidget( textFlowUsesBoundingBox );
	Textflow3 = new QCheckBox( "&Use Contour Line", textFlowsAroundFrame, "Textflow3" );
	textFlowsAroundFrameLayout->addWidget( Textflow3 );
	pageLayout_2->addWidget( textFlowsAroundFrame );

	QSpacerItem* spacer6 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout_2->addItem( spacer6 );
	idShapeItem=TabStack->addItem( page_2, "&Shape" );

	page_3 = new QWidget( TabStack, "page_3" );
	pageLayout_3 = new QVBoxLayout( page_3, 0, 5, "pageLayout_3");
	pageLayout_3->setAlignment( Qt::AlignLeft );

	layout47 = new QHBoxLayout( 0, 0, 5, "layout47");

	layout46 = new QVBoxLayout( 0, 0, 5, "layout46");

	layout41 = new QGridLayout( 0, 1, 1, 0, 5, "layout41");
	layout41->setAlignment( Qt::AlignLeft );

	Fonts = new FontComboH(page_3);
	Fonts->setMaximumSize(200, 80);
	layout41->addMultiCellWidget( Fonts, 0, 0, 0, 3 );

	Size = new MSpinBox( 0.5, 2048, page_3, 1 );
	Size->setPrefix( "" );
	fontsizeLabel = new QLabel( "", page_3, "fontsizeLabel" );
	fontsizeLabel->setPixmap(loadIcon("Zeichen.xpm"));
	layout41->addWidget( fontsizeLabel, 1, 0 );
	layout41->addWidget( Size, 1, 1 );
	ChBase = new MSpinBox( -100, 100, page_3, 1 );
	ChBase->setValue( 0 );
	ChBaseTxt = new QLabel("", page_3, "ChBaseTxt" );
	ChBaseTxt->setPixmap(loadIcon("textbase.png"));
	layout41->addWidget( ChBaseTxt, 1, 2 );
	layout41->addWidget( ChBase, 1, 3 );
	LineSp = new MSpinBox( page_3, 1 );
	layout41->addWidget( LineSp, 2, 1 );
	lineSpacingPop = new QPopupMenu();
	lineSpacingPop->insertItem( tr("Fixed Linespacing"));
	lineSpacingPop->insertItem( tr("Automatic Linespacing"));
	lineSpacingPop->insertItem( tr("Align to Baseline Grid"));
	linespacingLabel = new QToolButton(page_3, "linespacingLabel" );
	linespacingLabel->setText("");
	linespacingLabel->setPixmap(loadIcon("linespacing.png"));
	linespacingLabel->setPopup(lineSpacingPop);
	linespacingLabel->setPopupDelay(400);
	linespacingLabel->setAutoRaise(true);
	layout41->addWidget( linespacingLabel, 2, 0 );
	Extra = new MSpinBox( page_3, 1 );
	layout41->addWidget( Extra, 2, 3 );
	trackingLabel = new QLabel( "", page_3, "trackingLabel" );
	trackingLabel->setText("");
	trackingLabel->setPixmap(loadIcon("textkern.png"));
	layout41->addWidget( trackingLabel, 2, 2 );
	ChScale = new MSpinBox( 10, 400, page_3, 1 );
	ChScale->setValue( 100 );
	ScaleTxt = new QLabel("", page_3, "ScaleTxt" );
	ScaleTxt->setPixmap(loadIcon("textscaleh.png"));
	layout41->addWidget( ScaleTxt, 3, 0 );
	layout41->addWidget( ChScale, 3 , 1 );
	ChScaleV = new MSpinBox( 10, 400, page_3, 1 );
	ChScaleV->setValue( 100 );
	ScaleTxtV = new QLabel("", page_3, "ScaleTxtV" );
	ScaleTxtV->setPixmap(loadIcon("textscalev.png"));
	layout41->addWidget( ScaleTxtV, 3, 2 );
	layout41->addWidget( ChScaleV, 3, 3 );

	layout46->addLayout( layout41 );

	layout23 = new QHBoxLayout( 0, 0, 5, "layout23");
	layout23->setAlignment( Qt::AlignLeft );
	StrokeIcon = new QLabel( "", page_3, "StrokeIcon" );
	StrokeIcon->setPixmap(loadIcon("Stiftalt.xpm"));
	StrokeIcon->setScaledContents( false );
	layout23->addWidget( StrokeIcon );
	TxStroke = new ScComboBox( false, page_3, "TxStroke" );
	layout23->addWidget( TxStroke );
	ShadeTxt1 = new QLabel( "", page_3, "ShadeTxt" );
	ShadeTxt1->setPixmap(loadIcon("shade.png"));
	layout23->addWidget( ShadeTxt1 );
	PM1 = new ShadeButton(page_3);
	layout23->addWidget( PM1 );
	layout46->addLayout( layout23 );
	layout24 = new QHBoxLayout( 0, 0, 5, "layout24");
	layout24->setAlignment( Qt::AlignLeft );
	FillIcon = new QLabel( "", page_3, "FillIcon" );
	FillIcon->setPixmap(loadIcon("fill.png"));
	layout24->addWidget( FillIcon );
	TxFill = new ScComboBox( false, page_3, "TxFill" );
	layout24->addWidget( TxFill );
	ShadeTxt2 = new QLabel("", page_3, "ShadeTxt" );
	ShadeTxt2->setPixmap(loadIcon("shade.png"));
	layout24->addWidget( ShadeTxt2 );
	PM2 = new ShadeButton(page_3);
	layout24->addWidget( PM2 );
	layout46->addLayout( layout24 );

	Layout1 = new QHBoxLayout( 0, 0, 0, "Layout1");
	Layout1->setAlignment( Qt::AlignLeft );
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

	GroupBox3aLayout = new QGridLayout( 0, 1, 1, 0, 5, "Layout25");
	GroupBox3aLayout->setAlignment( Qt::AlignLeft );
	Spal = new Spalette(page_3);
	styleLabel = new QLabel( Spal, "St&yle:", page_3, "styleLabel" );
	GroupBox3aLayout->addWidget( styleLabel, 0, 0 );
	GroupBox3aLayout->addWidget( Spal, 0, 1 );
	langCombo = new ScComboBox( false, page_3, "Lang" );
	langLabel = new QLabel( langCombo, "Lan&guage:", page_3, "langLabel" );
	GroupBox3aLayout->addWidget( langLabel, 1, 0 );
	GroupBox3aLayout->addWidget( langCombo, 1, 1 );
	pageLayout_3->addLayout(GroupBox3aLayout);

	QSpacerItem* spacer8 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout_3->addItem( spacer8 );
	idTextItem=TabStack->addItem( page_3, "&Text" );

	page_4 = new QWidget( TabStack, "page_4" );
	pageLayout_4 = new QVBoxLayout( page_4, 0, 5, "pageLayout_4");

	FreeScale = new QRadioButton( "&Free Scaling", page_4, "FreeScale" );
	FreeScale->setChecked( true );
	pageLayout_4->addWidget( FreeScale );

	layout43 = new QGridLayout( 0, 1, 1, 0, 5, "layout43");
	LXpos = new MSpinBox( page_4, 2 );
	xposImgLabel = new QLabel( LXpos, "&X-Pos:", page_4, "xposImgLabel" );
	layout43->addWidget( xposImgLabel, 0, 0 );
	layout43->addWidget( LXpos, 0, 1 );
	LYpos = new MSpinBox( page_4, 2 );
	yposImgLabel = new QLabel( LYpos, "&Y-Pos:", page_4, "yposImgLabel" );
	layout43->addWidget( yposImgLabel, 1, 0 );
	layout43->addWidget( LYpos, 1, 1 );
	ScaleX = new MSpinBox( page_4, 1 );
	xscaleLabel = new QLabel( ScaleX, "X-Sc&ale:", page_4, "xscaleLabel" );
	layout43->addWidget( xscaleLabel, 2, 0 );
	layout43->addWidget( ScaleX, 2, 1 );
	ScaleY = new MSpinBox( page_4, 1 );
	yscaleLabel = new QLabel( ScaleY, "Y-Scal&e:", page_4, "yscaleLabel" );
	layout43->addWidget( yscaleLabel, 3, 0 );
	layout43->addWidget( ScaleY, 3, 1 );
	keepImageWHRatioButton = new LinkButton( page_4 );
	keepImageWHRatioButton->setToggleButton( true );
	keepImageWHRatioButton->setAutoRaise( true );
	keepImageWHRatioButton->setMaximumSize( QSize( 15, 32767 ) );
	layout43->addMultiCellWidget( keepImageWHRatioButton, 2, 3, 2, 2 );
	imgDpiX = new MSpinBox( page_4, 1 );
	imgDPIXLabel = new QLabel( imgDpiX, "Actual X-DPI:", page_4, "imgDPIYLabel" );
	layout43->addWidget( imgDPIXLabel, 4, 0 );
	layout43->addWidget( imgDpiX, 4, 1 );
	imgDpiY = new MSpinBox( page_4, 1 );
	imgDPIYLabel = new QLabel( imgDpiY, "Actual Y-DPI:", page_4, "imgDPIYLabel" );
	layout43->addWidget( imgDPIYLabel, 5, 0 );
	layout43->addWidget( imgDpiY, 5, 1 );
	keepImageDPIRatioButton = new LinkButton( page_4 );
	keepImageDPIRatioButton->setToggleButton( true );
	keepImageDPIRatioButton->setAutoRaise( true );
	keepImageDPIRatioButton->setMaximumSize( QSize( 15, 32767 ) );
	layout43->addMultiCellWidget( keepImageDPIRatioButton, 4, 5, 2, 2 );
	pageLayout_4->addLayout( layout43 );

	Layout24 = new QVBoxLayout( 0, 0, 3, "Layout24");

	FrameScale = new QRadioButton( page_4, "FixedScale" );
	FrameScale->setText( "Scale &To Frame Size" );
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
	Aspect->setText( "P&roportional" );
	Aspect->setChecked( true );

	Layout18->addWidget( Aspect );
	Layout24->addLayout( Layout18 );
	pageLayout_4->addLayout( Layout24 );

	GroupBoxCM = new QGroupBox( "", page_4, "GroupBoxcm" );
	GroupBoxCM->setColumnLayout(0, Qt::Vertical );
	GroupBoxCM->layout()->setSpacing( 2 );
	GroupBoxCM->layout()->setMargin( 5 );
	GroupBoxCM->setFrameShape( QFrame::NoFrame );
	GroupBoxCM->setFrameShadow( QFrame::Plain );
	GroupBoxCMLayout = new QVBoxLayout( GroupBoxCM->layout() );
	GroupBoxCMLayout->setAlignment( Qt::AlignTop );
	TextCms1 = new QLabel( GroupBoxCM, "xposLabel" );
	GroupBoxCMLayout->addWidget( TextCms1 );
	InputP = new ScComboBox( false, GroupBoxCM, "InputP" );
	GroupBoxCMLayout->addWidget(InputP);
	TextCms2 = new QLabel( GroupBoxCM, "TextCms2" );
	GroupBoxCMLayout->addWidget(TextCms2);
	MonitorI = new ScComboBox( false, GroupBoxCM, "MonitorI" );
	GroupBoxCMLayout->addWidget(MonitorI);
	pageLayout_4->addWidget(GroupBoxCM);

	QSpacerItem* spacer9 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout_4->addItem( spacer9 );
	idImageItem=TabStack->addItem( page_4, "&Image" );

	page_5 = new QWidget( TabStack, "page_5" );
	pageLayout_5 = new QVBoxLayout( page_5, 0, 5, "pageLayout_5");

	Layout13_2 = new QHBoxLayout( 0, 0, 4, "Layout13_2");
	LineMode = new ScComboBox( false, page_5, "LMode" );
	LineModeT = new QLabel( LineMode, "&Basepoint:", page_5, "LModeText" );
	Layout13_2->addWidget( LineModeT );
	Layout13_2->addWidget( LineMode );
	LMode = false;
	QSpacerItem* spacer10 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout13_2->addItem( spacer10 );
	pageLayout_5->addLayout( Layout13_2 );

	Layout12_2 = new QGridLayout( 0, 1, 1, 0, 3, "Layout12_2");
	LStyle = new LineCombo(page_5);
	linetypeLabel = new QLabel( LStyle, "T&ype of Line:", page_5, "linetypeLabel" );
	Layout12_2->addWidget( linetypeLabel, 0, 0 );
	Layout12_2->addWidget( LStyle, 0, 1 );
	startArrow = new ArrowChooser(page_5, true);
	Layout12_2->addWidget( startArrow, 2, 0 );
	endArrow = new ArrowChooser(page_5, false);
	Layout12_2->addWidget( endArrow, 2, 1 );
	startArrowText = new QLabel( startArrow, "Start Arrow:", page_5, "startArrowText" );
	Layout12_2->addWidget( startArrowText, 1, 0 );
	endArrowText = new QLabel( endArrow, "End Arrow:", page_5, "endArrowText" );
	Layout12_2->addWidget( endArrowText, 1, 1 );
	LSize = new MSpinBox( page_5, 1 );
	linewidthLabel = new QLabel(LSize, "Line &Width:", page_5, "linewidthLabel" );
	Layout12_2->addWidget( linewidthLabel, 3, 0 );
	Layout12_2->addWidget( LSize, 3, 1 );
	LJoinStyle = new ScComboBox( false, page_5, "LJoin" );
	edgesLabel = new QLabel( LJoinStyle, "Ed&ges:", page_5, "edgesLabel" );
	Layout12_2->addWidget( edgesLabel, 4, 0 );
	Layout12_2->addWidget( LJoinStyle, 4, 1 );
	LEndStyle = new ScComboBox( false, page_5, "LCap" );
	endingsLabel = new QLabel( LEndStyle, "&Endings:", page_5, "endingsLabel" );
	Layout12_2->addWidget( endingsLabel, 5, 0 );
	Layout12_2->addWidget( LEndStyle, 5, 1 );
	pageLayout_5->addLayout( Layout12_2 );

	TabStack3 = new QWidgetStack( page_5, "TabStack3" );
	TabStack3->setFrameShape( QWidgetStack::NoFrame );

	page_5a = new QWidget( TabStack3, "page" );
	pageLayout_5a = new QVBoxLayout( page_5a, 0, 5, "pageLayout_5a");
	StyledLine = new QListBox(page_5a, "StyledL");
	StyledLine->insertItem( "No Style", 0);
	pageLayout_5a->addWidget(StyledLine);
	TabStack3->addWidget( page_5a, 0 );

	page_5b = new QWidget( TabStack3, "page" );
	pageLayout_5b = new QVBoxLayout( page_5b, 0, 5, "pageLayout_5a");
	TLines = new QGroupBox( "Cell Lines", page_5b, "Distance" );
	TLines->setColumnLayout(0, Qt::Vertical );
	TLines->layout()->setSpacing( 2 );
	TLines->layout()->setMargin( 5 );
	TLineLayout = new QVBoxLayout( TLines->layout() );
	TLineLayout->setAlignment( Qt::AlignTop );
	TopLine = new QCheckBox( TLines, "TopLine" );
	TopLine->setText( "Line at Top" );
	TLineLayout->addWidget(TopLine);
	LeftLine = new QCheckBox( TLines, "TopLine" );
	LeftLine->setText( "Line at the Left" );
	TLineLayout->addWidget(LeftLine);
	RightLine = new QCheckBox( TLines, "TopLine" );
	RightLine->setText( "Line at the Right " );
	TLineLayout->addWidget(RightLine);
	BottomLine = new QCheckBox( TLines, "TopLine" );
	BottomLine->setText( "Line at Bottom" );
	TLineLayout->addWidget(BottomLine);
	pageLayout_5b->addWidget(TLines);
	TabStack3->addWidget( page_5b, 1 );
	pageLayout_5->addWidget( TabStack3 );
	idLineItem=TabStack->addItem( page_5, "&Line" );
	Cpal = new Cpalette(this);
	idColorsItem=TabStack->addItem(Cpal, "&Colors" );
	MpalLayout->addWidget( TabStack );

	languageChange();

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
	connect(imgDpiX, SIGNAL(valueChanged(int)), this, SLOT(HChangeD()));
	connect(imgDpiY, SIGNAL(valueChanged(int)), this, SLOT(VChangeD()));
	connect(LSize, SIGNAL(valueChanged(int)), this, SLOT(NewLS()));
	connect(LStyle, SIGNAL(activated(int)), this, SLOT(NewLSty()));
	connect(LJoinStyle, SIGNAL(activated(int)), this, SLOT(NewLJoin()));
	connect(LEndStyle, SIGNAL(activated(int)), this, SLOT(NewLEnd()));
	connect(LineMode, SIGNAL(activated(int)), this, SLOT(NewLMode()));
	connect(keepImageWHRatioButton, SIGNAL(clicked()), this, SLOT(ToggleKette()));
	connect(keepImageDPIRatioButton, SIGNAL(clicked()), this, SLOT(ToggleKetteD()));
	connect(FlipH, SIGNAL(clicked()), this, SLOT(DoFlipH()));
	connect(FlipV, SIGNAL(clicked()), this, SLOT(DoFlipV()));
	connect(GroupAlign, SIGNAL(State(int)), this, SLOT(NewAli(int)));
	connect(Revert, SIGNAL(clicked()), this, SLOT(DoRevert()));
	connect(SeStyle, SIGNAL(State(int)), this, SLOT(setTypeStyle(int)));
	connect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowOffs()));
	connect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowOffs()));
	connect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newOutlineW()));
	connect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newUnderline()));
	connect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newUnderline()));
	connect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newStrike()));
	connect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newStrike()));
	connect(FreeScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
	connect(FrameScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
	connect(Aspect, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
	connect(Zup, SIGNAL(clicked()), this, SLOT(DoRaise()));
	connect(ZDown, SIGNAL(clicked()), this, SLOT(DoLower()));
	connect(ZTop, SIGNAL(clicked()), this, SLOT(DoFront()));
	connect(ZBottom, SIGNAL(clicked()), this, SLOT(DoBack()));
	connect(RotationGroup, SIGNAL(clicked(int)), this, SLOT(NewRotMode(int)));
	connect(textFlowsAroundFrame, SIGNAL(clicked(int)), this, SLOT(DoFlow(int)));
	connect(SCustom, SIGNAL(FormSel(int, int, double *)), this, SLOT(MakeIrre(int, int, double *)));
	connect(EditShape, SIGNAL(clicked()), this, SLOT(EditSh()));
	connect(dGap, SIGNAL(valueChanged(int)), this, SLOT(NewGap()));
	connect(DCol, SIGNAL(valueChanged(int)), this, SLOT(NewCols()));
	connect(DTop, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
	connect(DLeft, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
	connect(DRight, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
	connect(DBottom, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
	connect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));
	connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	connect(Fonts, SIGNAL(fontSelected(QString )), this, SLOT(NewTFont(QString)));
	connect(TxFill, SIGNAL(activated(int)), this, SLOT(newTxtFill()));
	connect(TxStroke, SIGNAL(activated(int)), this, SLOT(newTxtStroke()));
	connect(PM1, SIGNAL(clicked()), this, SLOT(setActShade()));
	connect(PM2, SIGNAL(clicked()), this, SLOT(setActShade()));
	connect(ChScale, SIGNAL(valueChanged(int)), this, SLOT(NewTScale()));
	connect(ChScaleV, SIGNAL(valueChanged(int)), this, SLOT(NewTScaleV()));
	connect(ChBase, SIGNAL(valueChanged(int)), this, SLOT(NewTBase()));
	connect(Locked, SIGNAL(clicked()), this, SLOT(handleLock()));
	connect(NoPrint, SIGNAL(clicked()), this, SLOT(handlePrint()));
	connect(NoResize, SIGNAL(clicked()), this, SLOT(handleResize()));
	connect(showcurveCheckBox, SIGNAL(clicked()), this, SLOT(handlePathLine()));
	connect(Dist, SIGNAL(valueChanged(int)), this, SLOT(handlePathDist()));
	connect(LineW, SIGNAL(valueChanged(int)), this, SLOT(handlePathOffs()));
	connect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChProf(const QString&)));
	connect(MonitorI, SIGNAL(activated(int)), this, SLOT(ChIntent()));
	connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	connect(langCombo, SIGNAL(activated(int)), this, SLOT(NewLanguage()));
	connect( TabsButton, SIGNAL( clicked() ), this, SLOT( ManageTabs() ) );
	connect( TopLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( LeftLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( RightLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( BottomLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( colgapLabel, SIGNAL( clicked() ), this, SLOT( HandleGapSwitch() ) );
	connect( Cpal, SIGNAL(NewSpecial(double, double, double, double )), this, SLOT(NewSpGradient(double, double, double, double )));
	connect( Cpal, SIGNAL(editGradient()), this, SLOT(toggleGradientEdit()));
	connect(startArrow, SIGNAL(activated(int)), this, SLOT(setStartArrow(int )));
	connect(endArrow, SIGNAL(activated(int)), this, SLOT(setEndArrow(int )));
	connect(lineSpacingPop, SIGNAL(activated(int)), this, SLOT(setLspMode(int )));
	
	connect(this, SIGNAL(DocChanged()), ScMW, SLOT(slotDocCh()));
	connect(this, SIGNAL(NewAbStyle(int)), ScMW, SLOT(setNewAbStyle(int)));
	connect(this, SIGNAL(Stellung(int)), ScMW, SLOT(setItemHoch(int)));
	connect(this, SIGNAL(EditCL()), ScMW, SLOT(ToggleFrameEdit()));
	connect(this, SIGNAL(NewTF(const QString&)), ScMW, SLOT(SetNewFont(const QString&)));
	connect(this, SIGNAL(UpdtGui(int)), ScMW, SLOT(HaveNewSel(int)));
	connect(this->Cpal, SIGNAL(NewPen(QString)), ScMW, SLOT(setPenFarbe(QString)));
	connect(this->Cpal, SIGNAL(NewBrush(QString)), ScMW, SLOT(setBrushFarbe(QString)));
	connect(this->Cpal, SIGNAL(NewPenShade(int)), ScMW, SLOT(setPenShade(int)));
	connect(this->Cpal, SIGNAL(NewBrushShade(int)), ScMW, SLOT(setBrushShade(int)));
	connect(this->Cpal, SIGNAL(NewTrans(double)), ScMW, SLOT(setItemFillTransparency(double)));
	connect(this->Cpal, SIGNAL(NewTransS(double)), ScMW, SLOT(setItemLineTransparency(double)));
	connect(this->Cpal, SIGNAL(NewGradient(int)), ScMW, SLOT(setGradFill(int)));
	connect(this->Cpal->gradEdit->Preview, SIGNAL(gradientChanged()), ScMW, SLOT(updtGradFill()));
	connect(this->Cpal, SIGNAL(gradientChanged()), ScMW, SLOT(updtGradFill()));
	connect(this->Cpal, SIGNAL(QueryItem()), ScMW, SLOT(GetBrushPen()));
	
	
	
	
	
	
	
	
	
	
	
	
	HaveItem = false;
	Xpos->setValue(0);
	Ypos->setValue(0);
	Width->setValue(0);
	Height->setValue(0);
	Rot->setValue(0);
	RoundRect->setValue(0);
	TabStack3->raiseWidget(0);
	TabStack2->raiseWidget(0);
	for (int ws = 1; ws < 6; ++ws)
		TabStack->setItemEnabled(ws, false);
	TabStack->setCurrentIndex(0);
	TabStack->item(0)->setEnabled(false);
	TabStack->setItemEnabled(0, false);
	StrokeIcon->setEnabled(false);
	TxStroke->setEnabled(false);
	PM1->setEnabled(false);
}

void Mpalette::SelTab(int t)
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem) && (t == 5))
	{
		Cpal->setActGradient(CurItem->GrType);
		updateColorSpecialGradient();
		Cpal->gradEdit->Preview->fill_gradient = CurItem->fill_gradient;
		Cpal->gradEdit->Preview->updateDisplay();
	}
}

void Mpalette::setDoc(ScribusDoc *d)
{
	doc = d;
	Umrech=doc->unitRatio();
	double maxXYWHVal= 30000 * Umrech;
	double minXYVal= -30000 * Umrech;
	HaveDoc = true;
	HaveItem = false;

	Xpos->setValues( minXYVal, maxXYWHVal, 100, minXYVal);
	Ypos->setValues( minXYVal, maxXYWHVal, 100, minXYVal);
	Width->setValues( Umrech, maxXYWHVal, 100, Umrech);
	Height->setValues( Umrech, maxXYWHVal, 100, Umrech);
	LXpos->setValues( -30000, maxXYWHVal, 100, 0);
	LYpos->setValues( -30000, maxXYWHVal, 100, 0);

	Rot->setValues( 0, 359.99, 100, 0);
	RoundRect->setValues( -300, 300, 10, 0);
	Extra->setValues( -300, 300, 10, 0);
	Size->setValues( 0.5, 2048, 10, 1);
	LineSp->setValues( 1, 2048, 10, 1);
	ScaleX->setValues( 1, 30000, 10, 1);
	ScaleY->setValues( 1, 30000, 10, 1);
	imgDpiX->setValues( 1, 30000, 10, 1);
	imgDpiY->setValues( 1, 30000, 10, 1);

	dGap->setDecimals(10);
	DTop->setDecimals(10);
	DLeft->setDecimals(10);
	DBottom->setDecimals(10);
	DRight->setDecimals(10);
	LSize->setMaxValue( 36 );
	LSize->setMinValue( 0 );
	Dist->setMaxValue( 30000 );
	Dist->setMinValue( 0 );
	Dist->setLineStep(10);
	LineW->setMaxValue( 300 );
	LineW->setMinValue( -300 );
	LineW->setLineStep(10);

	updateCList();
	
	updateSpinBoxConstants();
}

void Mpalette::unsetDoc()
{
	HaveDoc = false;
}

void Mpalette::setCurrentItem(PageItem *i)
{
	if (ScMW->ScriptRunning)
		return;
	//CB We shouldnt really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This wont work until when a canvas deselect happens, CurItem must be NULL.
	//if (CurItem == i)
	//	return;
	disconnect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	disconnect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	disconnect(startArrow, SIGNAL(activated(int)), this, SLOT(setStartArrow(int )));
	disconnect(endArrow, SIGNAL(activated(int)), this, SLOT(setEndArrow(int )));
	HaveItem = false;
	CurItem = i;
	
	NewSel(i->itemType());
	NameEdit->setText(i->itemName());
	setXY(i->xPos(), i->yPos());
	setBH(i->width(), i->height());
	setR(i->rotation());
	
	Revert->setOn(i->reversed());
	setDvals(i->textToFrameDistLeft(), i->textToFrameDistTop(), i->textToFrameDistBottom(), i->textToFrameDistRight());
	LevelTxt->setText(QString::number(i->ItemNr));
	textFlowsAroundFrame->setChecked(i->textFlowsAroundFrame());
	textFlowUsesBoundingBox->setChecked(i->textFlowUsesBoundingBox());
	RoundRect->setValue(i->cornerRadius()*Umrech);
	
	Textflow3->setChecked(i->textFlowUsesContourLine());
	disconnect(FlipH, SIGNAL(clicked()), this, SLOT(DoFlipH()));
	disconnect(FlipV, SIGNAL(clicked()), this, SLOT(DoFlipV()));
	FlipH->setOn(i->imageFlippedH());
	FlipV->setOn(i->imageFlippedV());
	connect(FlipH, SIGNAL(clicked()), this, SLOT(DoFlipH()));
	connect(FlipV, SIGNAL(clicked()), this, SLOT(DoFlipV()));
	langCombo->setCurrentText(ScMW->LangTransl[i->Language]);
	if (TabStack->currentIndex() == 5)
		Cpal->setActGradient(CurItem->GrType);
	updateColorSpecialGradient();
	Cpal->gradEdit->Preview->fill_gradient = CurItem->fill_gradient;
	Cpal->gradEdit->Preview->updateDisplay();
	if (i->FrameType == 0)
		SCustom->setPixmap(SCustom->getIconPixmap(0));
	if (i->FrameType == 1)
		SCustom->setPixmap(SCustom->getIconPixmap(1));
	if (i->FrameType > 3)
		SCustom->setPixmap(SCustom->getIconPixmap(i->FrameType-2));
	if ((i->asLine()) || (i->asPolyLine()))
	{
		startArrow->setEnabled(true);
		endArrow->setEnabled(true);
		startArrow->setCurrentItem(i->startArrowIndex());
		endArrow->setCurrentItem(i->endArrowIndex());
	}
	else
	{
		startArrow->setEnabled(false);
		endArrow->setEnabled(false);
	}
	PageItem_TextFrame* i2=i->asTextFrame();
	if (i2!=0)
	{
		DCol->setMaxValue(QMAX(qRound(i2->width() / QMAX(i2->ColGap, 10.0)), 1));
		DCol->setMinValue(1);
		DCol->setValue(i2->Cols);
		dGap->setMinValue(0);
		if (colgapLabel->getState())
		{
			dGap->setMaxValue(QMAX((i2->width() / i2->Cols - i2->textToFrameDistLeft() - i2->textToFrameDistRight())*Umrech, 0));
			dGap->setValue(i2->ColGap*Umrech);
		}
		else
		{
			dGap->setMaxValue(QMAX((i2->width() / i2->Cols)*Umrech, 0));
			dGap->setValue(i2->columnWidth()*Umrech);
		}
	}
	bool setter;
	if (i->NamedLStyle.isEmpty())
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
	connect(startArrow, SIGNAL(activated(int)), this, SLOT(setStartArrow(int )));
	connect(endArrow, SIGNAL(activated(int)), this, SLOT(setEndArrow(int )));
	NoPrint->setOn(!i->printable());
	setter = i->locked();
	//keepFrameWHRatioButton->setOn(false);
	Width->setReadOnly(setter);
	Height->setReadOnly(setter);
	RoundRect->setEnabled(!setter);
	EditShape->setEnabled(!setter);
	ShapeGroup->setEnabled(!setter);
	LayerGroup->setEnabled(!setter);
	Locked->setOn(setter);
	if ((i->isTableItem) && (i->isSingleSel))
	{
		setter = true;
		TabStack3->raiseWidget(1);
		TopLine->setChecked(i->TopLine);
		LeftLine->setChecked(i->LeftLine);
		RightLine->setChecked(i->RightLine);
		BottomLine->setChecked(i->BottomLine);
	}
	else
		TabStack3->raiseWidget(0);
	LayerGroup->setEnabled(!setter);
	Xpos->setReadOnly(setter);
	Ypos->setReadOnly(setter);
	Rot->setReadOnly(setter);
	setter = i->sizeLocked();
	NoResize->setOn(setter);
	if (!i->locked())
	{
		Width->setReadOnly(setter);
		Height->setReadOnly(setter);
	}
	if (i->asPathText())
	{
		TabStack2->raiseWidget(1);
		showcurveCheckBox->setChecked(i->PoShow);
		LineW->setValue(i->BaseOffs * -1);
		Dist->setValue(i->textToFrameDistLeft());
	}
	else
		TabStack2->raiseWidget(0);
	// Frame type 3 is obsolete: CR 2005-02-06
	//if (((i->itemType() == PageItem::TextFrame) || (i->itemType() == PageItem::ImageFrame) || (i->itemType() == 3)) &&  (!i->ClipEdited))
	if (((i->asTextFrame()) || (i->asImageFrame())) &&  (!i->ClipEdited))
		RoundRect->setEnabled(true);
	else
	{
		if ((i->asPolygon()) && ((i->FrameType == 0) || (i->FrameType == 2)))
			RoundRect->setEnabled(true);
		else
			RoundRect->setEnabled(false);
	}

	if ((i->itemType() == PageItem::Line) && LMode) {
		xposLabel->setText( tr( "&X1:" ) );
		widthLabel->setText( tr( "X&2:" ) );
		yposLabel->setText( tr( "Y&1:" ) );
		heightLabel->setText( tr( "&Y2:" ) );
		Rot->setEnabled(false);
	} else {
		xposLabel->setText( tr( "&X-Pos:" ) );
		widthLabel->setText( tr( "&Width:" ) );
		yposLabel->setText( tr( "&Y-Pos:" ) );
		heightLabel->setText( tr( "&Height:" ) );
		Rot->setEnabled(true);
	}
	HaveItem = true;
	if (i->asLine())
	{
		keepFrameWHRatioButton->setEnabled(false);
		if (LMode && !i->locked())
			Height->setEnabled(true);
		else
			Height->setEnabled(false);
	}
	else
	{
		Height->setEnabled(true);
		keepFrameWHRatioButton->setEnabled(true);
		if (i->asImageFrame())
		{
			updateCmsList();
			setter = i->ScaleType;
			FreeScale->setChecked(setter);
			FrameScale->setChecked(!setter);
			if (setter == true)
			{
				keepImageWHRatioButton->setOn(setter);
				keepImageDPIRatioButton->setOn(setter);
			}
			Aspect->setEnabled(!setter);
			Aspect->setChecked(i->AspectRatio);
			LXpos->setEnabled(setter);
			LYpos->setEnabled(setter);
			ScaleX->setEnabled(setter);
			ScaleY->setEnabled(setter);
			imgDpiX->setEnabled(setter);
			imgDpiY->setEnabled(setter);
		}
	}
	connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	connect(startArrow, SIGNAL(activated(int)), this, SLOT(setStartArrow(int )));
	connect(endArrow, SIGNAL(activated(int)), this, SLOT(setEndArrow(int )));	
}


void Mpalette::SetCurItem(PageItem *i)
{
	if (ScMW->ScriptRunning)
		return;
	//CB We shouldnt really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This wont work until when a canvas deselect happens, CurItem must be NULL.
	//if (CurItem == i)
	//	return;
	disconnect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	disconnect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	disconnect(startArrow, SIGNAL(activated(int)), this, SLOT(setStartArrow(int )));
	disconnect(endArrow, SIGNAL(activated(int)), this, SLOT(setEndArrow(int )));
	disconnect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));
	
	HaveItem = false;
	CurItem = i;
	if (TabStack->currentIndex() == 5)
		Cpal->setActGradient(CurItem->GrType);
	updateColorSpecialGradient();
	Cpal->gradEdit->Preview->fill_gradient = CurItem->fill_gradient;
	Cpal->gradEdit->Preview->updateDisplay();
	if (i->FrameType == 0)
		SCustom->setPixmap(SCustom->getIconPixmap(0));
	if (i->FrameType == 1)
		SCustom->setPixmap(SCustom->getIconPixmap(1));
	if (i->FrameType > 3)
		SCustom->setPixmap(SCustom->getIconPixmap(i->FrameType-2));
	if ((i->asLine()) || (i->asPolyLine()))
	{
		startArrow->setEnabled(true);
		endArrow->setEnabled(true);
		startArrow->setCurrentItem(i->startArrowIndex());
		endArrow->setCurrentItem(i->endArrowIndex());
	}
	else
	{
		startArrow->setEnabled(false);
		endArrow->setEnabled(false);
	}
	NameEdit->setText(i->itemName());
	RoundRect->setValue(i->cornerRadius()*Umrech);
	QString tm;
	LevelTxt->setText(tm.setNum(i->ItemNr));
	PageItem_TextFrame *i2=i->asTextFrame();
	if (i2!=0)
	{
		DCol->setMaxValue(QMAX(qRound(i2->width() / QMAX(i2->ColGap, 10.0)), 1));
		DCol->setMinValue(1);
		DCol->setValue(i2->Cols);
		dGap->setMinValue(0);
		if (colgapLabel->getState())
		{
			dGap->setMaxValue(QMAX((i2->width() / i2->Cols - i2->textToFrameDistLeft() - i2->textToFrameDistRight())*Umrech, 0));
			dGap->setValue(i2->ColGap*Umrech);
		}
		else
		{
			dGap->setMaxValue(QMAX((i2->width() / i2->Cols)*Umrech, 0));
			dGap->setValue(i2->columnWidth()*Umrech);
		}
		DLeft->setValue(i2->textToFrameDistLeft()*Umrech);
		DTop->setValue(i2->textToFrameDistTop()*Umrech);
		DBottom->setValue(i2->textToFrameDistBottom()*Umrech);
		DRight->setValue(i2->textToFrameDistRight()*Umrech);
	}
	Revert->setOn(i->reversed());
	textFlowsAroundFrame->setChecked(i->textFlowsAroundFrame());
	textFlowUsesBoundingBox->setChecked(i->textFlowUsesBoundingBox());
	Textflow3->setChecked(i->textFlowUsesContourLine());
	disconnect(FlipH, SIGNAL(clicked()), this, SLOT(DoFlipH()));
	disconnect(FlipV, SIGNAL(clicked()), this, SLOT(DoFlipV()));
	FlipH->setOn(i->imageFlippedH());
	FlipV->setOn(i->imageFlippedV());
	connect(FlipH, SIGNAL(clicked()), this, SLOT(DoFlipH()));
	connect(FlipV, SIGNAL(clicked()), this, SLOT(DoFlipV()));
	langCombo->setCurrentText(ScMW->LangTransl[i->Language]);
	bool setter;
	if (i->NamedLStyle.isEmpty())
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
	connect(startArrow, SIGNAL(activated(int)), this, SLOT(setStartArrow(int )));
	connect(endArrow, SIGNAL(activated(int)), this, SLOT(setEndArrow(int )));
	NoPrint->setOn(!i->printable());
	setter = i->locked();
	//keepFrameWHRatioButton->setOn(false);
	Width->setReadOnly(setter);
	Height->setReadOnly(setter);
	RoundRect->setEnabled(!setter);
	EditShape->setEnabled(!setter);
	ShapeGroup->setEnabled(!setter);
	LayerGroup->setEnabled(!setter);
	Locked->setOn(setter);
	if ((i->isTableItem) && (i->isSingleSel))
	{
		setter = true;
		TabStack3->raiseWidget(1);
		TopLine->setChecked(i->TopLine);
		LeftLine->setChecked(i->LeftLine);
		RightLine->setChecked(i->RightLine);
		BottomLine->setChecked(i->BottomLine);
	}
	else
		TabStack3->raiseWidget(0);
	LayerGroup->setEnabled(!setter);
	Xpos->setReadOnly(setter);
	Ypos->setReadOnly(setter);
	Rot->setReadOnly(setter);
	setter = i->sizeLocked();
	NoResize->setOn(setter);
	if (!i->locked())
	{
		Width->setReadOnly(setter);
		Height->setReadOnly(setter);
	}
	if (i->asPathText())
	{
		TabStack2->raiseWidget(1);
		showcurveCheckBox->setChecked(i->PoShow);
		LineW->setValue(i->BaseOffs * -1);
		Dist->setValue(i->textToFrameDistLeft());
	}
	else
		TabStack2->raiseWidget(0);
	// Frame type 3 is obsolete: CR 2005-02-06
	//if (((i->itemType() == PageItem::TextFrame) || (i->itemType() == PageItem::ImageFrame) || (i->itemType() == 3)) &&  (!i->ClipEdited))
	if (((i->asTextFrame()) || (i->asImageFrame())) &&  (!i->ClipEdited))
		RoundRect->setEnabled(true);
	else
	{
		if ((i->asPolygon()) && ((i->FrameType == 0) || (i->FrameType == 2)))
			RoundRect->setEnabled(true);
		else
			RoundRect->setEnabled(false);
	}

	if ((i->itemType() == PageItem::Line) && LMode) {
		xposLabel->setText( tr( "&X1:" ) );
		widthLabel->setText( tr( "X&2:" ) );
		yposLabel->setText( tr( "Y&1:" ) );
		heightLabel->setText( tr( "&Y2:" ) );
		Rot->setEnabled(false);
	} else {
		xposLabel->setText( tr( "&X-Pos:" ) );
		widthLabel->setText( tr( "&Width:" ) );
		yposLabel->setText( tr( "&Y-Pos:" ) );
		heightLabel->setText( tr( "&Height:" ) );
		Rot->setEnabled(true);
	}
	HaveItem = true;
	if (i->asLine())
	{
		keepFrameWHRatioButton->setEnabled(false);
		if (LMode && !i->locked())
			Height->setEnabled(true);
		else
			Height->setEnabled(false);
	}
	else
	{
		Height->setEnabled(true);
		keepFrameWHRatioButton->setEnabled(true);
		if (i->asImageFrame())
		{
			updateCmsList();
			setter = i->ScaleType;
			FreeScale->setChecked(setter);
			FrameScale->setChecked(!setter);
			if (setter == true)
			{
				keepImageWHRatioButton->setOn(setter);
				keepImageDPIRatioButton->setOn(setter);
			}
			Aspect->setEnabled(!setter);
			Aspect->setChecked(i->AspectRatio);
			LXpos->setEnabled(setter);
			LYpos->setEnabled(setter);
			ScaleX->setEnabled(setter);
			ScaleY->setEnabled(setter);
			imgDpiX->setEnabled(setter);
			imgDpiY->setEnabled(setter);
		}
	}
	setXY(i->xPos(), i->yPos());
	
	updateSpinBoxConstants();
}

void Mpalette::NewSel(int nr)
{
	if (ScMW->ScriptRunning)
		return;
	int visID;
	disconnect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));
	if (doc->selection->isMultipleSelection())
	{
		RoVal = 0;
		double gx, gy, gh, gw;
		ScMW->view->getGroupRect(&gx, &gy, &gw, &gh);
		if (TopLeft->isChecked())
			ScMW->view->RCenter = FPoint(gx, gy);
		if (TopRight->isChecked())
			ScMW->view->RCenter = FPoint(gx + gw, gy);
		if (Center->isChecked())
			ScMW->view->RCenter = FPoint(gx + gw / 2.0, gy + gh / 2.0);
		if (BottomLeft->isChecked())
			ScMW->view->RCenter = FPoint(gx, gy + gh);
		if (BottomRight->isChecked())
			ScMW->view->RCenter = FPoint(gx + gw, gy + gh);
		xposLabel->setText( tr( "&X-Pos:" ) );
		widthLabel->setText( tr( "&Width:" ) );
		yposLabel->setText( tr( "&Y-Pos:" ) );
		heightLabel->setText( tr( "&Height:" ) );
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
		TabStack->setCurrentIndex(0);
		TabStack->item(0)->setEnabled(true);
		TabStack->setItemEnabled(0, true);
		NameEdit->setEnabled(false);
		TabStack->setItemEnabled(5, true);
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
		visID = TabStack->currentIndex ();
		TabStack->item(0)->setEnabled(true);
		TabStack->setItemEnabled(0, true);
		TabStack->setItemEnabled(5, true);
		disconnect(FlipH, SIGNAL(clicked()), this, SLOT(DoFlipH()));
		disconnect(FlipV, SIGNAL(clicked()), this, SLOT(DoFlipV()));
		FlipH->setOn(false);
		FlipV->setOn(false);
		connect(FlipH, SIGNAL(clicked()), this, SLOT(DoFlipH()));
		connect(FlipV, SIGNAL(clicked()), this, SLOT(DoFlipV()));
		switch (nr)
		{
		case -1:
			xposLabel->setText( tr( "&X-Pos:" ) );
			widthLabel->setText( tr( "&Width:" ) );
			yposLabel->setText( tr( "&Y-Pos:" ) );
			heightLabel->setText( tr( "&Height:" ) );
			Rot->setEnabled(true);
			Height->setEnabled(true);
			HaveItem = false;
			Xpos->setValue(0);
			Ypos->setValue(0);
			Width->setValue(0);
			Height->setValue(0);
			Rot->setValue(0);
			RoundRect->setValue(0);
			for (int ws = 1; ws < 6; ++ws)
				TabStack->setItemEnabled(ws, false);
			TabStack->setCurrentIndex(0);
			TabStack->item(0)->setEnabled(false);
			TabStack->setItemEnabled(0, false);
			Cpal->ChooseGrad(0);
			break;
		case 2:
			TabStack->setItemEnabled(1, true);
			TabStack->setItemEnabled(2, false);
			TabStack->setItemEnabled(3, true);
			TabStack->setItemEnabled(4, true);
			FlipH->setEnabled(true);
			FlipV->setEnabled(true);
			ShapeGroup->setEnabled(true);
			//if ((ScMW->view->SelItem.at(0)->FrameType == 0) || (ScMW->view->SelItem.at(0)->FrameType == 2))
			if ((doc->selection->itemAt(0)->FrameType == 0) || (doc->selection->itemAt(0)->FrameType == 2))
				RoundRect->setEnabled(true);
			EditShape->setEnabled(true);
			if (visID == 2)
				TabStack->setCurrentIndex(0);
			HaveItem = true;
			break;
		case 4:
			TabStack->setItemEnabled(1, true);
			TabStack->setItemEnabled(2, true);
			TabStack->setItemEnabled(3, false);
			TabStack->setItemEnabled(4, true);
			FlipH->setEnabled(true);
			FlipV->setEnabled(true);
			ShapeGroup->setEnabled(true);
			//if ((ScMW->view->SelItem.at(0)->FrameType == 0) || (ScMW->view->SelItem.at(0)->FrameType == 2))
			if ((doc->selection->itemAt(0)->FrameType == 0) || (doc->selection->itemAt(0)->FrameType == 2))
				RoundRect->setEnabled(true);
			Distance->setEnabled(true);
			EditShape->setEnabled(true);
			if (visID == 3)
				TabStack->setCurrentIndex(0);
			HaveItem = true;
			break;
		case 5:
			TabStack->setItemEnabled(1, false);
			TabStack->setItemEnabled(2, false);
			TabStack->setItemEnabled(3, false);
			TabStack->setItemEnabled(4, true);
			LineMode->setEnabled(true);
			TopLeft->setEnabled(false);
			TopRight->setEnabled(false);
			BottomLeft->setEnabled(false);
			BottomRight->setEnabled(false);
			Center->setEnabled(false);
			if ((visID == 1) || (visID == 2) || (visID == 3))
				TabStack->setCurrentIndex(0);
			HaveItem = true;
			break;
		case 1:
		case 3:
		case 6:
			TabStack->setItemEnabled(1, true);
			TabStack->setItemEnabled(2, false);
			TabStack->setItemEnabled(3, false);
			TabStack->setItemEnabled(4, true);
			ShapeGroup->setEnabled(true);
			EditShape->setEnabled(true);
			FlipH->setEnabled(true);
			FlipV->setEnabled(true);
			//if ((ScMW->view->SelItem.at(0)->FrameType == 0) || (ScMW->view->SelItem.at(0)->FrameType == 2))
			if ((doc->selection->itemAt(0)->FrameType == 0) || (doc->selection->itemAt(0)->FrameType == 2))
				RoundRect->setEnabled(true);
			if ((visID == 2) || (visID == 3))
				TabStack->setCurrentIndex(0);
			HaveItem = true;
			break;
		case 7:
			FlipH->setEnabled(true);
			FlipV->setEnabled(true);
			TabStack->setItemEnabled(1, true);
			TabStack->setItemEnabled(2, false);
			TabStack->setItemEnabled(3, false);
			TabStack->setItemEnabled(4, true);
			EditShape->setEnabled(true);
			if ((visID == 2) || (visID == 3))
				TabStack->setCurrentIndex(0);
			HaveItem = true;
			break;
		case 8:
			TabStack->setItemEnabled(1, true);
			TabStack->setItemEnabled(2, true);
			TabStack->setItemEnabled(3, false);
			TabStack->setItemEnabled(4, true);
			EditShape->setEnabled(true);
			if (visID == 3)
				TabStack->setCurrentIndex(0);
			HaveItem = true;
			break;
		}
	}
	updateGeometry();
	setFocus();
	repaint();
	connect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));
}

void Mpalette::setMultipleSelection(bool isMultiple)
{
	FlipH->setEnabled(!isMultiple);
	FlipV->setEnabled(!isMultiple);
}

void Mpalette::UnitChange()
{
	double oldRatio = Umrech;
	Umrech = doc->unitRatio();
	bool tmp = HaveItem;
	HaveItem = false;
	double maxXYWHVal=30000 * Umrech;
	double minXYVal=-30000 * Umrech;

	double ratioDivisor = Umrech / oldRatio;
	double newX = Xpos->value() * ratioDivisor;
	double newY = Ypos->value() * ratioDivisor;
	double newW = Width->value() * ratioDivisor;
	double newH = Height->value() * ratioDivisor;
	double newLX = LXpos->value() * ratioDivisor;
	double newLY = LYpos->value() * ratioDivisor;
	double newG = dGap->value() * ratioDivisor;
	double newGM = dGap->maxValue() * ratioDivisor;
	double newDT = DTop->value() * ratioDivisor;
	double newDL = DLeft->value() * ratioDivisor;
	double newDB = DBottom->value() * ratioDivisor;
	double newDR = DRight->value() * ratioDivisor;
	double newRR = RoundRect->value() * ratioDivisor;
	double newRM = RoundRect->maxValue() * ratioDivisor;

	if (doc->unitIndex() > unitGetMaxIndex())
		doc->setUnitIndex(0);
	QString ein = unitGetSuffixFromIndex(doc->unitIndex());

	Xpos->setSuffix( ein );
	Ypos->setSuffix( ein );
	Width->setSuffix( ein );
	Height->setSuffix( ein );
	LXpos->setSuffix( ein );
	LYpos->setSuffix( ein );
	dGap->setSuffix( ein );
	DLeft->setSuffix( ein );
	DTop->setSuffix( ein );
	DBottom->setSuffix( ein );
	DRight->setSuffix( ein );
	RoundRect->setSuffix( ein );

	int decimals = unitGetDecimalsFromIndex(doc->unitIndex());

	Xpos->setValues( minXYVal, maxXYWHVal, decimals, newX );
	Ypos->setValues( minXYVal, maxXYWHVal, decimals, newY );
	Width->setValues( Umrech, maxXYWHVal, decimals, newW );
	Height->setValues( Umrech, maxXYWHVal, decimals, newH );

	LXpos->setDecimals(decimals);
	LXpos->setMaxValue( maxXYWHVal );
	LXpos->setValue(newLX);

	LYpos->setDecimals(decimals);
	LYpos->setMaxValue( maxXYWHVal );
	LYpos->setValue(newLY);

	dGap->setDecimals(decimals);
	dGap->setMaxValue(newGM);
	dGap->setValue(newG);

	DLeft->setDecimals(decimals);
	DLeft->setMaxValue( 300 );
	DLeft->setValue(newDL);

	DTop->setDecimals(decimals);
	DTop->setMaxValue( 300 );
	DTop->setValue(newDT);

	DBottom->setDecimals(decimals);
	DBottom->setMaxValue( 300 );
	DBottom->setValue(newDB);

	DRight->setDecimals(decimals);
	DRight->setMaxValue( 300 );
	DRight->setValue(newDR);

	RoundRect->setValues(-newRM, newRM, decimals, newRR);
	Cpal->UnitChange(oldRatio, Umrech, doc->unitIndex());
	HaveItem = tmp;
}

void Mpalette::setLevel(uint l)
{
	QString tm;
	LevelTxt->setText(tm.setNum(l));
}

void Mpalette::setXY(double x, double y)
{
	if (ScMW->ScriptRunning)
		return;
	disconnect(Xpos, SIGNAL(valueChanged(int)), this, SLOT(NewX()));
	disconnect(Ypos, SIGNAL(valueChanged(int)), this, SLOT(NewY()));
	bool tmp = HaveItem;
	double inX, inY, b, h, r, dummy1, dummy2;
	QWMatrix ma;
	FPoint n;
	if (HaveItem)
	{
		if (doc->selection->isMultipleSelection())
		{
			ScMW->view->getGroupRect(&dummy1, &dummy2, &b, &h);
			r = 0.0;
		}
		else
		{
			b = CurItem->width();
			h = CurItem->height();
			r = CurItem->rotation();
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
	if (tmp)
	{
		inX -= doc->rulerXoffset;
		inY -= doc->rulerYoffset;
		if (doc->guidesSettings.rulerMode)
		{
			inX -= doc->currentPage->xOffset();
			inY -= doc->currentPage->yOffset();
		}
	}
	Xpos->setValue(inX*Umrech);
	Ypos->setValue(inY*Umrech);
	if ((LMode) && (tmp))
		setBH(CurItem->width(), CurItem->height());
	HaveItem = tmp;
	connect(Xpos, SIGNAL(valueChanged(int)), this, SLOT(NewX()));
	connect(Ypos, SIGNAL(valueChanged(int)), this, SLOT(NewY()));
}

void Mpalette::setBH(double x, double y)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	QWMatrix ma;
	QPoint dp;
	if ((LMode) && (CurItem->asLine()))
	{
		ma.translate(static_cast<double>(Xpos->value()) / Umrech, static_cast<double>(Ypos->value()) / Umrech);
		ma.rotate(static_cast<double>(Rot->value())*(-1));
		dp = ma * QPoint(static_cast<int>(x), static_cast<int>(y));
		Width->setValue(dp.x()*Umrech);
		Height->setValue(dp.y()*Umrech);
	}
	else
	{
		RoundRect->setMaxValue(QMIN(x, y)/2*Umrech);
		RoundRect->setMinValue(-QMIN(x, y)/2*Umrech);
		Width->setValue(x*Umrech);
		Height->setValue(y*Umrech);
	}
	HaveItem = tmp;
}

void Mpalette::setR(double r)
{
	if (ScMW->ScriptRunning)
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
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	RoundRect->setValue(r*Umrech);
	HaveItem = tmp;
}

void Mpalette::setCols(int r, double g)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	DCol->setValue(r);
	dGap->setValue(g*Umrech);
	if (tmp)
	{
		PageItem_TextFrame *i2=CurItem->asTextFrame();
		if (i2!=0)
		{
			DCol->setMaxValue(QMAX(qRound(i2->width() / QMAX(i2->ColGap, 10.0)), 1));
			if (colgapLabel->getState())
			{
				dGap->setMaxValue(QMAX((i2->width() / i2->Cols - i2->textToFrameDistLeft() - i2->textToFrameDistRight())*Umrech, 0));
				dGap->setValue(i2->ColGap*Umrech);
			}
			else
			{
				dGap->setMaxValue(QMAX((i2->width() / i2->Cols)*Umrech, 0));
				dGap->setValue(i2->columnWidth()*Umrech);
			}
		}
	}
	DCol->setMinValue(1);
	dGap->setMinValue(0);
	HaveItem = tmp;
}

void Mpalette::setLspMode(int id)
{
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ChLineSpaMode(lineSpacingPop->indexOf(id));
		emit DocChanged();
	}
}

void Mpalette::setLsp(double r)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	LineSp->setValue(r);
	if (tmp)
	{
		if (CurItem->LineSpMode > 0)
			LineSp->setEnabled(false);
		else
			LineSp->setEnabled(true);
		for (uint al = 0; al < lineSpacingPop->count(); ++al)
		{
			lineSpacingPop->setItemChecked(lineSpacingPop->idAt(al), false);
		}
		lineSpacingPop->setItemChecked(lineSpacingPop->idAt(CurItem->LineSpMode), true);
	}
	HaveItem = tmp;
}

void Mpalette::setDvals(double left, double top, double bottom, double right)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	DLeft->setValue(left*Umrech);
	DTop->setValue(top*Umrech);
	DBottom->setValue(bottom*Umrech);
	DRight->setValue(right*Umrech);
	HaveItem = tmp;
}

void Mpalette::setFontFace(QString newFont)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	Fonts->setCurrentFont(newFont);
	HaveItem = tmp;
}


void Mpalette::setSize(int s)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	Size->setValue(s / 10.0);
	HaveItem = tmp;
}

void Mpalette::setExtra(int e)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	Extra->setValue(e / 10.0);
	HaveItem = tmp;
}

void Mpalette::ChangeScaling()
{
	if (ScMW->ScriptRunning)
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
		imgDpiX->setEnabled(true);
		imgDpiY->setEnabled(true);
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
		imgDpiX->setEnabled(false);
		imgDpiY->setEnabled(false);
	}
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setImageScalingMode(FreeScale->isChecked(), Aspect->isChecked());
		emit UpdtGui(2);
		emit DocChanged();
		setFocus();
	}
}

void Mpalette::setLvalue(double scx, double scy, double x, double y)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	if (tmp)
	{
		LXpos->setValue(x * Umrech * CurItem->imageXScale());
		LYpos->setValue(y * Umrech * CurItem->imageYScale());
		ScaleX->setValue(scx * 100 / 72.0 * CurItem->pixm.imgInfo.xres);
		ScaleY->setValue(scy * 100 / 72.0 * CurItem->pixm.imgInfo.yres);
		imgDpiX->setValue(qRound(720.0 / CurItem->imageXScale()) / 10.0);
		imgDpiY->setValue(qRound(720.0 / CurItem->imageXScale()) / 10.0); //CB I assume this douple X is right?
	}
	else
	{
		LXpos->setValue(x * Umrech);
		LYpos->setValue(y * Umrech);
		ScaleX->setValue(scx * 100);
		ScaleY->setValue(scy * 100);
		imgDpiX->setValue(72);
		imgDpiY->setValue(72);
	}
	HaveItem = tmp;
}

void Mpalette::setSvalue(double s)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	LSize->setValue(s);
	HaveItem = tmp;
}

void Mpalette::setLIvalue(PenStyle p, PenCapStyle pc, PenJoinStyle pj)
{
	if (ScMW->ScriptRunning)
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
	if (ScMW->ScriptRunning)
		return;
	StrokeIcon->setEnabled(false);
	TxStroke->setEnabled(false);
	PM1->setEnabled(false);
	SeStyle->setStyle(s);
	if ((s & 4) || (s & 256))
	{
		StrokeIcon->setEnabled(true);
		TxStroke->setEnabled(true);
		PM1->setEnabled(true);
	}
}

void Mpalette::setAli(int e)
{
	if (ScMW->ScriptRunning)
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

void Mpalette::setTScaleV(int e)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	ChScaleV->setValue(e / 10.0);
	HaveItem = tmp;
}

void Mpalette::NewTScaleV()
{
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ItemTextScaleV(qRound(ChScaleV->value() * 10));
		doc->CurrTextScaleV = qRound(ChScaleV->value() * 10);
		emit DocChanged();
	}
}

void Mpalette::NewTBase()
{
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->setItemTextBase(qRound(ChBase->value() * 10));
		doc->CurrTextBase = qRound(ChBase->value() * 10);
		emit DocChanged();
	}
}

void Mpalette::setTScale(int e)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	ChScale->setValue(e / 10.0);
	HaveItem = tmp;
}

void Mpalette::setTBase(int e)
{
	if (ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	ChBase->setValue(e / 10.0);
	HaveItem = tmp;
}

void Mpalette::NewTScale()
{
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ItemTextScale(qRound(ChScale->value() * 10));
		doc->CurrTextScale = qRound(ChScale->value() * 10);
		emit DocChanged();
	}
}

void Mpalette::NewX()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		double x,y,w,h, gx, gy, gh, gw, base;
		QWMatrix ma;
		x = Xpos->value() / Umrech;
		y = Ypos->value() / Umrech;
		w = Width->value() / Umrech;
		h = Height->value() / Umrech;
		base = 0;
		x += doc->rulerXoffset;
		y += doc->rulerYoffset;
		if (doc->guidesSettings.rulerMode)
		{
			x += doc->currentPage->xOffset();
			y += doc->currentPage->yOffset();
		}
		if (doc->selection->isMultipleSelection())
		{
			ScMW->view->getGroupRect(&gx, &gy, &gw, &gh);
			if ((TopLeft->isChecked()) || (BottomLeft->isChecked()))
				base = gx;
			if (Center->isChecked())
				base = gx + gw / 2.0;
			if ((TopRight->isChecked()) || (BottomRight->isChecked()))
				base = gx + gw;
			ScMW->view->moveGroup(x - base, 0, true);
		}
		else
		{
			if ((CurItem->asLine()) && (LMode))
			{
				double r = atan2(h-y,w-x)*(180.0/M_PI);
				w = sqrt(pow(w-x,2)+pow(h-y,2));
				ScMW->view->MoveItem(x - CurItem->xPos(), 0, CurItem, true);
				ScMW->view->SizeItem(w, CurItem->height(), CurItem->ItemNr, true);
				ScMW->view->RotateItem(r, CurItem->ItemNr);
			}
			else
			{
				ma.translate(CurItem->xPos(), CurItem->yPos());
				ma.rotate(CurItem->rotation());
				if (TopLeft->isChecked())
					base = CurItem->xPos();
				if (Center->isChecked())
					base = ma.m11() * (CurItem->width() / 2.0) + ma.m21() * (CurItem->height() / 2.0) + ma.dx();
				if (TopRight->isChecked())
					base = ma.m11() * CurItem->width() + ma.m21() * 0.0 + ma.dx();
				if (BottomRight->isChecked())
					base = ma.m11() * CurItem->width() + ma.m21() * CurItem->height() + ma.dx();
				if (BottomLeft->isChecked())
					base = ma.m11() * 0.0 + ma.m21() * CurItem->height() + ma.dx();
				ScMW->view->MoveItem(x - base, 0, CurItem, true);
			}
		}
		emit DocChanged();
	}
}

void Mpalette::NewY()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		double x,y,w,h, gx, gy, gh, gw, base;
		QWMatrix ma;
		x = Xpos->value() / Umrech;
		y = Ypos->value() / Umrech;
		w = Width->value() / Umrech;
		h = Height->value() / Umrech;
		base = 0;
		x += doc->rulerXoffset;
		y += doc->rulerYoffset;
		if (doc->guidesSettings.rulerMode)
		{
			x += doc->currentPage->xOffset();
			y += doc->currentPage->yOffset();
		}
		if (doc->selection->isMultipleSelection())
		{
			ScMW->view->getGroupRect(&gx, &gy, &gw, &gh);
			if ((TopLeft->isChecked()) || (TopRight->isChecked()))
				base = gy;
			if (Center->isChecked())
				base = gy + gh / 2.0;
			if ((BottomLeft->isChecked()) || (BottomRight->isChecked()))
				base = gy + gh;
			ScMW->view->moveGroup(0, y - base, true);
		}
		else
		{
			if ((CurItem->asLine()) && (LMode))
			{
				double r = atan2(h-y,w-x)*(180.0/M_PI);
				w = sqrt(pow(w-x,2)+pow(h-y,2));
				ScMW->view->MoveItem(0, y - CurItem->yPos(), CurItem, true);
				ScMW->view->SizeItem(w, CurItem->height(), CurItem->ItemNr, true);
				ScMW->view->RotateItem(r, CurItem->ItemNr);
			}
			else
			{
				ma.translate(CurItem->xPos(), CurItem->yPos());
				ma.rotate(CurItem->rotation());
				if (TopLeft->isChecked())
					base = CurItem->yPos();
				if (Center->isChecked())
					base = ma.m22() * (CurItem->height() / 2.0) + ma.m12() * (CurItem->width() / 2.0) + ma.dy();
				if (TopRight->isChecked())
					base = ma.m22() * 0.0 + ma.m12() * CurItem->width() + ma.dy();
				if (BottomRight->isChecked())
					base = ma.m22() * CurItem->height() + ma.m12() * CurItem->width() + ma.dy();
				if (BottomLeft->isChecked())
					base = ma.m22() * CurItem->height() + ma.m12() * 0.0 + ma.dy();
				ScMW->view->MoveItem(0, y - base, CurItem, true);
			}
		}
		emit DocChanged();
	}
}

void Mpalette::NewW()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		double x,y,w,h, gx, gy, gh, gw;	
		x = Xpos->value() / Umrech;
		y = Ypos->value() / Umrech;
		w = Width->value() / Umrech;
		h = Height->value() / Umrech;
		if (doc->selection->isMultipleSelection())
		{
			ScMW->view->getGroupRect(&gx, &gy, &gw, &gh);
			if (keepFrameWHRatioButton->isOn())
			{
				ScMW->view->HowTo = 1;
				ScMW->view->scaleGroup(w / gw, w / gw);
				setBH(w, (w / gw) * gh);
			}
			else
			{
				ScMW->view->HowTo = 6;
				ScMW->view->scaleGroup(w / gw, 1.0);
			}
		}
		else
		{
			CurItem->OldB2 = CurItem->width();
			CurItem->OldH2 = CurItem->height();
			if (CurItem->asLine())
			{
				if (LMode)
				{
					double r = atan2(h-y,w-x)*(180.0/M_PI);
					ScMW->view->RotateItem(r, CurItem->ItemNr);
					w = sqrt(pow(w-x,2)+pow(h-y,2));
				}
				ScMW->view->SizeItem(w, CurItem->height(), CurItem->ItemNr, true);
			}
			else
			{
				if (CurItem->isTableItem)
				{
					int rmo = doc->RotMode;
					doc->RotMode = 0;
					double dist = w - CurItem->width();
					double oldW = CurItem->width();
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
							ScMW->view->MoveRotated(bb2->RightLink, FPoint(dist, 0), true);
							bb2 = bb2->RightLink;
						}
						ScMW->view->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr, true);
						bb = bb->BottomLink;
					}
					bb2 = bb;
					while (bb2->RightLink != 0)
					{
						ScMW->view->MoveRotated(bb2->RightLink, FPoint(dist, 0), true);
						bb2 = bb2->RightLink;
					}
					ScMW->view->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr, true);
					doc->RotMode = rmo;
					if (keepFrameWHRatioButton->isOn())
					{
						keepFrameWHRatioButton->setOn(false);
						setBH(w, (w / oldW) * CurItem->height());
						NewH();
						keepFrameWHRatioButton->setOn(true);
					}
				}
				else
				{
					if (keepFrameWHRatioButton->isOn())
					{
						setBH(w, (w / CurItem->width()) * CurItem->height());
						ScMW->view->SizeItem(w, (w / CurItem->width()) * CurItem->height(), CurItem->ItemNr, true);
					}
					else
						ScMW->view->SizeItem(w, CurItem->height(), CurItem->ItemNr, true);
				}
			}
			emit DocChanged();
		}
	}
}

void Mpalette::NewH()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		double x,y,w,h, gx, gy, gh, gw;
		x = Xpos->value() / Umrech;
		y = Ypos->value() / Umrech;
		w = Width->value() / Umrech;
		h = Height->value() / Umrech;	
		if (doc->selection->isMultipleSelection())
		{
			ScMW->view->getGroupRect(&gx, &gy, &gw, &gh);
			if (keepFrameWHRatioButton->isOn())
			{
				ScMW->view->HowTo = 1;
				ScMW->view->scaleGroup(h / gh, h / gh);
				setBH((h / gh) * gw, h);
			}
			else
			{
				ScMW->view->HowTo = 5;
				ScMW->view->scaleGroup(1.0, h / gh);
			}
		}
		else
		{
			CurItem->OldB2 = CurItem->width();
			CurItem->OldH2 = CurItem->height();
			if (CurItem->asLine())
			{
				if (LMode)
				{
					double r = atan2(h-y,w-x)*(180.0/M_PI);
					ScMW->view->RotateItem(r, CurItem->ItemNr);
					w = sqrt(pow(w-x,2)+pow(h-y,2));
				}
				ScMW->view->SizeItem(w, CurItem->height(), CurItem->ItemNr, true);
			}
			else
			{
				if (CurItem->isTableItem)
				{
					int rmo = doc->RotMode;
					doc->RotMode = 0;
					double dist = h - CurItem->height();
					double oldH = CurItem->height();
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
							ScMW->view->MoveRotated(bb2->BottomLink, FPoint(0, dist), true);
							bb2 = bb2->BottomLink;
						}
						ScMW->view->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr, true);
						bb = bb->RightLink;
					}
					bb2 = bb;
					while (bb2->BottomLink != 0)
					{
						ScMW->view->MoveRotated(bb2->BottomLink, FPoint(0, dist), true);
						bb2 = bb2->BottomLink;
					}
					ScMW->view->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr, true);
					doc->RotMode = rmo;
					if (keepFrameWHRatioButton->isOn())
					{
						keepFrameWHRatioButton->setOn(false);
						setBH((h / oldH) * CurItem->width(), h);
						NewW();
						keepFrameWHRatioButton->setOn(true);
					}
				}
				else
				{
					if (keepFrameWHRatioButton->isOn())
					{
						setBH((h / CurItem->height()) * CurItem->width(), h);
						ScMW->view->SizeItem((h / CurItem->height()) * CurItem->width(), h, CurItem->ItemNr, true);
					}
					else
						ScMW->view->SizeItem(CurItem->width(), h, CurItem->ItemNr, true);
				}
			}
		}
		emit DocChanged();
	}
}

void Mpalette::NewR()
{
	if (ScMW->ScriptRunning)
		return;
	double gx, gy, gh, gw;
	if ((HaveDoc) && (HaveItem))
	{
		if (doc->selection->isMultipleSelection())
		{
			ScMW->view->RotateGroup((Rot->value() - RoVal)*(-1));
			ScMW->view->getGroupRect(&gx, &gy, &gw, &gh);
			setXY(gx, gy);
		}
		else
			ScMW->view->RotateItem(Rot->value()*(-1), CurItem->ItemNr);
		emit DocChanged();
		RoVal = Rot->value();
	}
}

void Mpalette::NewRR()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setCornerRadius(RoundRect->value() / Umrech);
		ScMW->view->SetFrameRounded();
		emit DocChanged();
	}
}

void Mpalette::NewLsp()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ChLineSpa(LineSp->value());
		emit DocChanged();
	}
}

void Mpalette::HandleGapSwitch()
{
	if ((HaveDoc) && (HaveItem))
	{
		setCols(CurItem->Cols, CurItem->ColGap);
		QToolTip::remove(dGap);
		if (colgapLabel->getState())
			QToolTip::add( dGap, tr( "Distance between columns" ) );
		else
			QToolTip::add( dGap, tr( "Column width" ) );
	}
}

void Mpalette::NewCols()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->Cols = DCol->value();
		setCols(CurItem->Cols, CurItem->ColGap);
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::NewGap()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if (colgapLabel->getState())
			CurItem->ColGap = dGap->value() / Umrech;
		else
		{
			double lineCorr;
			if (CurItem->lineColor() != "None")
				lineCorr = CurItem->Pwidth;
			else
				lineCorr = 0;
			double newWidth = dGap->value() / Umrech;
			double newGap = QMAX(((CurItem->width() - CurItem->textToFrameDistLeft() - CurItem->textToFrameDistRight() - lineCorr) - (newWidth * CurItem->Cols)) / (CurItem->Cols - 1), 0);
			CurItem->ColGap = newGap;
		}
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::NewSize()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->chFSize(qRound(Size->value()*10.0));
		emit DocChanged();
	}
}

void Mpalette::NewExtra()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->HasSel) || (doc->appMode == modeNormal))
		{
			ScMW->view->chKerning(qRound(Extra->value() * 10.0));
			emit DocChanged();
		}
		else
		{
			if (uint(CurItem->CPos) != CurItem->itemText.count())
			{
				CurItem->itemText.at(CurItem->CPos)->cextra = qRound(Extra->value() * 10.0);
				ScMW->view->RefreshItem(CurItem);
				emit DocChanged();
			}
		}
	}
}

void Mpalette::NewLocalXY()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ChLocalXY(LXpos->value() / Umrech / CurItem->imageXScale(), LYpos->value() / Umrech / CurItem->imageYScale());
		emit DocChanged();
	}
}

void Mpalette::NewLocalSC()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ChLocalSc(ScaleX->value() / 100.0 / CurItem->pixm.imgInfo.xres * 72.0, ScaleY->value() / 100.0 / CurItem->pixm.imgInfo.yres * 72.0);
		ScMW->view->ChLocalXY(LXpos->value() / Umrech / CurItem->imageXScale(), LYpos->value() / Umrech / CurItem->imageYScale());
		disconnect(imgDpiX, SIGNAL(valueChanged(int)), this, SLOT(HChangeD()));
		disconnect(imgDpiY, SIGNAL(valueChanged(int)), this, SLOT(VChangeD()));
		imgDpiX->setValue(qRound(720.0 / CurItem->imageXScale()) / 10.0);
		imgDpiY->setValue(qRound(720.0 / CurItem->imageXScale()) / 10.0);
		connect(imgDpiX, SIGNAL(valueChanged(int)), this, SLOT(HChangeD()));
		connect(imgDpiY, SIGNAL(valueChanged(int)), this, SLOT(VChangeD()));
		emit DocChanged();
	}
}

void Mpalette::NewLocalDpi()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ChLocalSc(72.0 / imgDpiX->value(), 72.0 / imgDpiY->value());
		ScMW->view->ChLocalXY(LXpos->value() / Umrech / CurItem->imageXScale(), LYpos->value() / Umrech / CurItem->imageYScale());
		disconnect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
		disconnect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
		ScaleX->setValue(CurItem->imageXScale() * 100 / 72.0 * CurItem->pixm.imgInfo.xres);
		ScaleY->setValue(CurItem->imageYScale() * 100 / 72.0 * CurItem->pixm.imgInfo.yres);
		connect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
		connect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
		emit DocChanged();
	}
}

void Mpalette::NewLS()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ChLineWidth(LSize->value());
		emit DocChanged();
	}
}

void Mpalette::setStartArrow(int id)
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setStartArrowIndex(id);
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::setEndArrow(int id)
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setEndArrowIndex(id);
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::NewLSty()
{
	if (ScMW->ScriptRunning)
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
		ScMW->view->ChLineArt(c);
		emit DocChanged();
	}
}

void Mpalette::NewLMode()
{
	if (ScMW->ScriptRunning)
		return;
	if (LineMode->currentItem() == 0)
	{
		xposLabel->setText( tr( "&X-Pos:" ) );
		widthLabel->setText( tr( "&Width:" ) );
		yposLabel->setText( tr( "&Y-Pos:" ) );
		heightLabel->setText( tr( "&Height:" ) );
		Rot->setEnabled(true);
		Height->setEnabled(false);
		LMode = false;
	}
	else
	{
		xposLabel->setText( tr( "&X1:" ) );
		widthLabel->setText( tr( "X&2:" ) );
		yposLabel->setText( tr( "Y&1:" ) );
		heightLabel->setText( tr( "&Y2:" ) );
		Rot->setEnabled(false);
		Height->setEnabled(true);
		LMode = true;
	}
	setBH(CurItem->width(), CurItem->height());
	updateGeometry();
	setFocus();
	repaint();
}

void Mpalette::NewLJoin()
{
	if (ScMW->ScriptRunning)
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
		ScMW->view->ChLineJoin(c);
		emit DocChanged();
	}
}

void Mpalette::NewLEnd()
{
	if (ScMW->ScriptRunning)
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
		ScMW->view->ChLineEnd(c);
		emit DocChanged();
	}
}

void Mpalette::ToggleKette()
{
	if (ScMW->ScriptRunning)
		return;
	disconnect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
	disconnect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
	if (keepImageWHRatioButton->isOn())
	{
		ScaleY->setValue(ScaleX->value());
		NewLocalSC();
		keepImageDPIRatioButton->setOn(true);
	}
	else
		keepImageDPIRatioButton->setOn(false);
	connect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
	connect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
}

void Mpalette::HChange()
{
	disconnect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
	disconnect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
	if (keepImageWHRatioButton->isOn())
		ScaleY->setValue(ScaleX->value());
	NewLocalSC();
	connect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
	connect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
}

void Mpalette::VChange()
{
	disconnect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
	disconnect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
	if (keepImageWHRatioButton->isOn())
		ScaleX->setValue(ScaleY->value());
	NewLocalSC();
	connect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
	connect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
}

void Mpalette::ToggleKetteD()
{
	if (ScMW->ScriptRunning)
		return;
	disconnect(imgDpiX, SIGNAL(valueChanged(int)), this, SLOT(HChangeD()));
	disconnect(imgDpiY, SIGNAL(valueChanged(int)), this, SLOT(VChangeD()));
	if (keepImageDPIRatioButton->isOn())
	{
		imgDpiY->setValue(imgDpiX->value());
		NewLocalDpi();
		keepImageWHRatioButton->setOn(true);
	}
	else
		keepImageWHRatioButton->setOn(false);
	connect(imgDpiX, SIGNAL(valueChanged(int)), this, SLOT(HChangeD()));
	connect(imgDpiY, SIGNAL(valueChanged(int)), this, SLOT(VChangeD()));
}

void Mpalette::HChangeD()
{
	disconnect(imgDpiX, SIGNAL(valueChanged(int)), this, SLOT(HChangeD()));
	disconnect(imgDpiY, SIGNAL(valueChanged(int)), this, SLOT(VChangeD()));
	if (keepImageDPIRatioButton->isOn())
		imgDpiY->setValue(imgDpiX->value());
	NewLocalDpi();
	connect(imgDpiX, SIGNAL(valueChanged(int)), this, SLOT(HChangeD()));
	connect(imgDpiY, SIGNAL(valueChanged(int)), this, SLOT(VChangeD()));
}

void Mpalette::VChangeD()
{
	disconnect(imgDpiX, SIGNAL(valueChanged(int)), this, SLOT(HChangeD()));
	disconnect(imgDpiY, SIGNAL(valueChanged(int)), this, SLOT(VChangeD()));
	if (keepImageDPIRatioButton->isOn())
		imgDpiX->setValue(imgDpiY->value());
	NewLocalDpi();
	connect(imgDpiX, SIGNAL(valueChanged(int)), this, SLOT(HChangeD()));
	connect(imgDpiY, SIGNAL(valueChanged(int)), this, SLOT(VChangeD()));
}

void Mpalette::DoFlipH()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->itemType() == PageItem::ImageFrame) || (CurItem->itemType() == PageItem::TextFrame))
			ScMW->view->FlipImageH();
		else
			ScMW->view->MirrorPolyH();
		emit DocChanged();
	}
}

void Mpalette::DoFlipV()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->itemType() == PageItem::ImageFrame) || (CurItem->itemType() == PageItem::TextFrame))
			ScMW->view->FlipImageV();
		else
			ScMW->view->MirrorPolyV();
		emit DocChanged();
	}
}

void Mpalette::NewAli(int a)
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if (doc->currentParaStyle < 5)
			emit NewAbStyle(a);
	}
}

void Mpalette::setTypeStyle(int s)
{
	if (ScMW->ScriptRunning)
		return;
	emit Stellung(s);
}

void Mpalette::newShadowOffs()
{
	int x = qRound(SeStyle->ShadowVal->Xoffset->value() * 10.0);
	int y = qRound(SeStyle->ShadowVal->Yoffset->value() * 10.0);
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->setItemTextShadow(x, y);
		doc->CurrTextShadowX = x;
		doc->CurrTextShadowY = y;
		emit DocChanged();
	}
}

void Mpalette::setShadowOffs(int x, int y)
{
	if (ScMW->ScriptRunning)
		return;
	disconnect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowOffs()));
	disconnect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowOffs()));
	SeStyle->ShadowVal->Xoffset->setValue(x / 10.0);
	SeStyle->ShadowVal->Yoffset->setValue(y / 10.0);
	connect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowOffs()));
	connect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowOffs()));
}

void Mpalette::newUnderline()
{
	int x = qRound(SeStyle->UnderlineVal->LPos->value() * 10.0);
	int y = qRound(SeStyle->UnderlineVal->LWidth->value() * 10.0);
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->setItemTextUnderline(x, y);
		doc->CurrTextUnderPos = x;
		doc->CurrTextUnderWidth = y;
		emit DocChanged();
	}
}

void Mpalette::setUnderline(int p, int w)
{
	if (ScMW->ScriptRunning)
		return;
	disconnect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newUnderline()));
	disconnect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newUnderline()));
	SeStyle->UnderlineVal->LPos->setValue(p / 10.0);
	SeStyle->UnderlineVal->LWidth->setValue(w / 10.0);
	connect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newUnderline()));
	connect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newUnderline()));
}

void Mpalette::newStrike()
{
	int x = qRound(SeStyle->StrikeVal->LPos->value() * 10.0);
	int y = qRound(SeStyle->StrikeVal->LWidth->value() * 10.0);
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->setItemTextStrike(x, y);
		doc->CurrTextStrikePos = x;
		doc->CurrTextStrikeWidth = y;
		emit DocChanged();
	}
}

void Mpalette::setStrike(int p, int w)
{
	if (ScMW->ScriptRunning)
		return;
	disconnect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newStrike()));
	disconnect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newStrike()));
	SeStyle->StrikeVal->LPos->setValue(p / 10.0);
	SeStyle->StrikeVal->LWidth->setValue(w / 10.0);
	connect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newStrike()));
	connect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newStrike()));
}

void Mpalette::setOutlineW(int x)
{
	if (ScMW->ScriptRunning)
		return;
	disconnect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newOutlineW()));
	SeStyle->OutlineVal->LWidth->setValue(x / 10.0);
	connect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newOutlineW()));
}

void Mpalette::newOutlineW()
{
	int x = qRound(SeStyle->OutlineVal->LWidth->value() * 10.0);
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->setItemTextOutline(x);
		doc->CurrTextOutline = x;
		emit DocChanged();
	}
}

void Mpalette::DoLower()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->LowerItem();
		emit DocChanged();
	}
}

void Mpalette::DoRaise()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->RaiseItem();
		emit DocChanged();
	}
}

void Mpalette::DoFront()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ToFront();
		emit DocChanged();
	}
}

void Mpalette::DoBack()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ToBack();
		emit DocChanged();
	}
}

void Mpalette::NewRotMode(int m)
{
	if (ScMW->ScriptRunning)
		return;
	double inX, inY, gx, gy, gh, gw;
	inX = 0;
	inY = 0;
	if ((HaveDoc) && (HaveItem))
	{
		HaveItem = false;
		if (doc->selection->isMultipleSelection())
		{
			ScMW->view->setGroupRect();
			ScMW->view->getGroupRect(&gx, &gy, &gw, &gh);
			if (m == 0)
			{
				ScMW->view->RCenter = FPoint(gx, gy);
				inX = gx;
				inY = gy;
			}
			if (m == 1)
			{
				ScMW->view->RCenter = FPoint(gx+gw, gy);
				inX = gx+gw;
				inY = gy;
			}
			if (m == 2)
			{
				ScMW->view->RCenter = FPoint(gx + gw / 2.0, gy + gh / 2.0);
				inX = gx + gw / 2.0;
				inY = gy + gh / 2.0;
			}
			if (m == 3)
			{
				ScMW->view->RCenter = FPoint(gx, gy+gh);
				inX = gx;
				inY = gy+gh;
			}
			if (m == 4)
			{
				ScMW->view->RCenter = FPoint(gx+gw, gy+gh);
				inX = gx+gw;
				inY = gy+gh;
			}
			Xpos->setValue(inX*Umrech);
			Ypos->setValue(inY*Umrech);
		}
		else
		{
			double b, h, r;
			QWMatrix ma;
			FPoint n;
			b = CurItem->width();
			h = CurItem->height();
			r = CurItem->rotation();
			ma.translate(CurItem->xPos()-doc->getXOffsetForPage(CurItem->OwnPage), CurItem->yPos()-doc->getYOffsetForPage(CurItem->OwnPage));
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
			Xpos->setValue(inX*Umrech);
			Ypos->setValue(inY*Umrech);
		}
		HaveItem = true;
		doc->RotMode = m;
	}
}

void Mpalette::DoFlow(int id)
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		switch (id)
		{
			case 0:
				CurItem->setTextFlowsAroundFrame(textFlowsAroundFrame->isChecked());
				textFlowUsesBoundingBox->setChecked(CurItem->textFlowUsesBoundingBox());
				Textflow3->setChecked(CurItem->textFlowUsesContourLine());
				break;
			case 1:
				CurItem->setTextFlowUsesBoundingBox(textFlowUsesBoundingBox->isChecked());
				if (textFlowUsesBoundingBox->isChecked())
				{
					Textflow3->setChecked(!textFlowUsesBoundingBox->isChecked());
					CurItem->setTextFlowUsesContourLine(Textflow3->isChecked());
				}
				break;
			case 2:
				CurItem->setTextFlowUsesContourLine(Textflow3->isChecked());
				if (Textflow3->isChecked())
				{
					textFlowUsesBoundingBox->setChecked(!Textflow3->isChecked());
					CurItem->setTextFlowUsesBoundingBox(textFlowUsesBoundingBox->isChecked());
				}
				break;
		}
		ScMW->view->DrawNew();
		emit DocChanged();
	}
}

void Mpalette::MakeIrre(int f, int c, double *vals)
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->itemType() == PageItem::PolyLine) || (CurItem->itemType() == PageItem::PathText))
			return;
		switch (f)
		{
		case 0:
			CurItem->SetRectFrame();
			ScMW->view->setRedrawBounding(CurItem);
			break;
		case 1:
			CurItem->SetOvalFrame();
			ScMW->view->setRedrawBounding(CurItem);
			break;
		default:
			CurItem->SetFrameShape(c, vals);
			ScMW->view->setRedrawBounding(CurItem);
			CurItem->FrameType = f+2;
			break;
		}
		ScMW->SCustom->setPixmap(ScMW->SCustom->getIconPixmap(f));
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
		if ((CurItem->itemType() == PageItem::ImageFrame) || (CurItem->itemType() == PageItem::TextFrame))
			return;
		CurItem->convertTo(PageItem::Polygon);
		NewSel(6);
		TabStack->setCurrentIndex(1);
	}
}

void Mpalette::EditSh()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		/* Frame types 1 and 3 are OBSOLETE: CR 2005-02-06
		if ((CurItem->PType == 1) || (CurItem->PType == 3))
		{
			CurItem->PType = 6;
			NewSel(6);
			TabStack->raiseWidget(1);
		}
		*/
		emit EditCL();
//		emit DocChanged();
	}
}

void Mpalette::NewTDist()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setTextToFrameDist(DLeft->value() / Umrech, DRight->value() / Umrech, DTop->value() / Umrech, DBottom->value() / Umrech);
		setCols(CurItem->Cols, CurItem->ColGap);
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::NewSpGradient(double x1, double y1, double x2, double y2)
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->GrStartX = x1 / Umrech;
		CurItem->GrStartY = y1 / Umrech;
		CurItem->GrEndX = x2 / Umrech;
		CurItem->GrEndY = y2 / Umrech;
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::toggleGradientEdit()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if (Cpal->gradEditButton->isOn())
			ScMW->setAppMode(modeEditGradientVectors);
		else
			ScMW->setAppMode(modeNormal);
		ScMW->view->RefreshItem(CurItem);
	}
}

void Mpalette::NewTFont(QString c)
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
		emit NewTF(c);
}

void Mpalette::DoRevert()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		bool setter=Revert->isOn();
		CurItem->setImageFlippedH(setter);
		CurItem->setReversed(setter);
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::SetLineFormats(ScribusDoc *dd)
{
	if (ScMW->ScriptRunning)
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
				dd->PageColors[ml[its].Color].getRawRGBColor().rgb(&h, &s, &v);
				if ((h == s) && (s == v))
				{
					dd->PageColors[ml[its].Color].getRawRGBColor().hsv(&h, &s, &v);
					sneu = 255 - ((255 - v) * ml[its].Shade / 100);
					tmpf.setHsv(h, s, sneu);
				}
				else
				{
					dd->PageColors[ml[its].Color].getRawRGBColor().hsv(&h, &s, &v);
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
		StyledLine->sort( true );
		StyledLine->insertItem( tr("No Style"), 0);
		StyledLine->setSelected(StyledLine->currentItem(), false);
	}
	connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
}

void Mpalette::SetSTline(QListBoxItem *c)
{
	if (ScMW->ScriptRunning)
		return;
	if (c == NULL)
		return;
	bool setter = c->listBox()->currentItem() == 0 ? true : false;
	if (setter == true)
		CurItem->setCustomLineStyle("");
	else
		CurItem->setCustomLineStyle(c->text());
	LStyle->setEnabled(setter);
	LSize->setEnabled(setter);
	LJoinStyle->setEnabled(setter);
	LEndStyle->setEnabled(setter);
	ScMW->view->RefreshItem(CurItem);
	emit DocChanged();
}

void Mpalette::updateColorList()
{
	if (!HaveDoc)
		return;
	Cpal->SetColors(doc->PageColors);
}

void Mpalette::updateCList()
{
	if (ScMW->ScriptRunning)
		return;
	if (!HaveDoc)
		return;
	TxFill->clear();
	TxStroke->clear();
	
	TxFill->insertItem( tr("None"));
	TxStroke->insertItem( tr("None"));
	ColorList::Iterator itend=doc->PageColors.end();
	for (ColorList::Iterator it = doc->PageColors.begin(); it != itend; ++it)
	{
		QPixmap * pm = getSmallPixmap(doc->PageColors[it.key()].getRawRGBColor());
		TxFill->insertItem(*pm, it.key());
		TxStroke->insertItem(*pm, it.key());
	}
	TxFill->listBox()->setMinimumWidth(TxFill->listBox()->maxItemWidth()+24);
	TxStroke->listBox()->setMinimumWidth(TxStroke->listBox()->maxItemWidth()+24);
}

void Mpalette::updateCmsList()
{
	if (ScMW->ScriptRunning)
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
		disconnect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChProf(const QString&)));
		disconnect(MonitorI, SIGNAL(activated(int)), this, SLOT(ChIntent()));
		InputP->clear();
		if (HaveItem)
		{
			if (CurItem->pixm.imgInfo.colorspace == 1)
			{
				ProfilesL::Iterator itP;
				ProfilesL::Iterator itPend=ScMW->InputProfilesCMYK.end();
				for (itP = ScMW->InputProfilesCMYK.begin(); itP != itPend; ++itP)
				{
					InputP->insertItem(itP.key());
					if (itP.key() == CurItem->IProfile)
						InputP->setCurrentItem(InputP->count()-1);
				}
				if (!ScMW->InputProfilesCMYK.contains(CurItem->IProfile))
				{
					InputP->insertItem(CurItem->IProfile);
					InputP->setCurrentItem(InputP->count()-1);
				}
				else
				{
					if (!CurItem->EmProfile.isEmpty())
						InputP->insertItem(CurItem->EmProfile);
				}
			}
			else
			{
				ProfilesL::Iterator itP;
				ProfilesL::Iterator itPend=ScMW->InputProfiles.end();
				for (itP = ScMW->InputProfiles.begin(); itP != itPend; ++itP)
				{
					InputP->insertItem(itP.key());
					if (itP.key() == CurItem->IProfile)
						InputP->setCurrentItem(InputP->count()-1);
				}
				if (!ScMW->InputProfiles.contains(CurItem->IProfile))
				{
					InputP->insertItem(CurItem->IProfile);
					InputP->setCurrentItem(InputP->count()-1);
				}
				else
				{
					if (!CurItem->EmProfile.isEmpty())
						InputP->insertItem(CurItem->EmProfile);
				}
			}
			MonitorI->setCurrentItem(CurItem->IRender);
		}
		connect(MonitorI, SIGNAL(activated(int)), this, SLOT(ChIntent()));
		connect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChProf(const QString&)));
	}
}

void Mpalette::ChProf(const QString& prn)
{
	if (ScMW->ScriptRunning)
		return;
	/* PFJ - 29.02.04 - Moved bool into if scope */
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->IProfile = InputP->currentText();
		/* PFJ - 29.02.04 - re-arranged the initialisation of EmbedP */
		bool EmbedP = prn.startsWith("Embedded") ? true : false;
		CurItem->UseEmbedded = EmbedP;
		doc->LoadPict(CurItem->Pfile, CurItem->ItemNr, true);
		ScMW->view->RefreshItem(CurItem);
	}
}

void Mpalette::ChIntent()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->IRender = MonitorI->currentItem();
		doc->LoadPict(CurItem->Pfile, CurItem->ItemNr, true);
		ScMW->view->RefreshItem(CurItem);
	}
}

void Mpalette::ShowCMS()
{
	if (ScMW->ScriptRunning)
		return;
	if (HaveItem)
		updateCmsList();
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
		ScMW->view->ItemTextBrush(TxFill->currentText());
		doc->CurrTextFill = TxFill->currentText();
		emit DocChanged();
	}
}

void Mpalette::newTxtStroke()
{
	if ((HaveDoc) && (HaveItem))
	{
		ScMW->view->ItemTextPen(TxStroke->currentText());
		doc->CurrTextStroke = TxStroke->currentText();
		emit DocChanged();
	}
}

void Mpalette::setActShade()
{
	if (ScMW->ScriptRunning)
		return;
	int b;
	if (PM1 == sender())
	{
		b = PM1->getValue();
		ScMW->view->ItemTextPenS(b);
		doc->CurrTextFillSh = b;
	}
	else
	{
		b = PM2->getValue();
		ScMW->view->ItemTextBrushS(b);
		doc->CurrTextStrokeSh = b;
	}
	emit DocChanged();
}

void Mpalette::setActFarben(QString p, QString b, int shp, int shb)
{
	if (ScMW->ScriptRunning)
		return;
	ColorList::Iterator it;
	int c = 0;
	PM2->setValue(shb);
	PM1->setValue(shp);
	if ((b != "None") && (!b.isEmpty()))
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
	if ((p != "None") && (!p.isEmpty()))
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
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		//uint selectedItemCount=ScMW->view->SelItem.count();
		uint selectedItemCount=doc->selection->count();
		if (selectedItemCount > 1)
		{
			//if (ScMW->view->SelItem.at(0)->locked())
			if (doc->selection->itemAt(0)->locked())
				ScMW->view->undoManager->beginTransaction(Um::SelectionGroup,
											  Um::IGroup, Um::UnLock, 0, Um::IUnLock);
			else
				ScMW->view->undoManager->beginTransaction(Um::SelectionGroup,
											  Um::IGroup, Um::Lock, 0, Um::ILock);
		}
		for ( uint a = 0; a < selectedItemCount; ++a)
		{
			//ScMW->view->SelItem.at(a)->setLocked(Locked->isOn());
			doc->selection->itemAt(a)->setLocked(Locked->isOn());
			//ScMW->view->RefreshItem(ScMW->view->SelItem.at(a));
			ScMW->view->RefreshItem(doc->selection->itemAt(a));
		}
		bool setter = Locked->isOn();
		Xpos->setReadOnly(setter);
		Ypos->setReadOnly(setter);
		Width->setReadOnly(setter);
		Height->setReadOnly(setter);
		Rot->setReadOnly(setter);
		RoundRect->setEnabled(!setter);
		EditShape->setEnabled(!setter);
		ShapeGroup->setEnabled(!setter);
		LayerGroup->setEnabled(!setter);
		emit DocChanged();
		if (selectedItemCount > 1)
			ScMW->view->undoManager->commit();
	}
}

void Mpalette::handlePrint()
{
	if ((HaveDoc) && (HaveItem))
	{
		//for ( uint a = 0; a < ScMW->view->SelItem.count(); ++a)
		for ( uint a = 0; a < doc->selection->count(); ++a)
			//ScMW->view->SelItem.at(a)->setPrintable(!NoPrint->isOn());
			doc->selection->itemAt(a)->setPrintable(!NoPrint->isOn());
		emit DocChanged();
	}
}

void Mpalette::handleResize()
{
	if ((HaveDoc) && (HaveItem))
	{
		//uint selectedItemCount=ScMW->view->SelItem.count();
		uint selectedItemCount=doc->selection->count();
		if (selectedItemCount > 1)
		{
			//if (ScMW->view->SelItem.at(0)->sizeLocked())
			if (doc->selection->itemAt(0)->sizeLocked())
				ScMW->view->undoManager->beginTransaction(Um::SelectionGroup,
											  Um::IGroup, Um::SizeUnLock, 0, Um::IUnLock);
			else
				ScMW->view->undoManager->beginTransaction(Um::SelectionGroup,
											  Um::IGroup, Um::SizeLock, 0, Um::ILock);
		}
		for ( uint a = 0; a < selectedItemCount; ++a)
		{
			//ScMW->view->SelItem.at(a)->setSizeLocked(NoResize->isOn());
			doc->selection->itemAt(a)->setSizeLocked(NoResize->isOn());
			//ScMW->view->RefreshItem(ScMW->view->SelItem.at(a));
			ScMW->view->RefreshItem(doc->selection->itemAt(a));
		}
		Width->setReadOnly(NoResize->isOn());
		Height->setReadOnly(NoResize->isOn());
		emit DocChanged();
		if (selectedItemCount > 1)
			ScMW->view->undoManager->commit();
	}
}

void Mpalette::handlePathLine()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->PoShow = showcurveCheckBox->isChecked();
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::handlePathDist()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setTextToFrameDistLeft(Dist->value());
		ScMW->view->AdjustItemSize(CurItem);
		CurItem->UpdatePolyClip();
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::handlePathOffs()
{
	if (ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->BaseOffs = -LineW->value();
		ScMW->view->AdjustItemSize(CurItem);
		CurItem->UpdatePolyClip();
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::NewName()
{
	if (ScMW->ScriptRunning || !HaveDoc || !HaveItem)
		return;
	QString NameOld = CurItem->itemName();
	QString NameNew = NameEdit->text();
	if (NameNew.isEmpty())
	{
		NameEdit->setText(NameOld);
		return;
	}
	bool found = false;
	for (uint b = 0; b < doc->Items->count(); ++b)
	{
		if ((NameNew == doc->Items->at(b)->itemName()) && (doc->Items->at(b) != CurItem))
		{
			found = true;
			break;
		}
	}
	if (found)
	{
		QMessageBox::warning(this, CommonStrings::trWarning, "<qt>"+tr("Name \"%1\" isn't unique.<br/>Please choose another.").arg(NameNew)+"</qt>", CommonStrings::tr_OK);
		NameEdit->setText(NameOld);
		NameEdit->setFocus();
	}
	else
	{
		if (CurItem->itemName() != NameEdit->text())
		{
			CurItem->setItemName(NameEdit->text());
			CurItem->AutoName = false;
			emit DocChanged();
		}
	}
}

/*! 10/07/2004 - pv - rewritten to fix #1185.
Uses sortQStringList from utils.cpp - STL!
 \param QMap<QString,QString> langMap a structure with languages/hyphs*/
void Mpalette::fillLangCombo(QMap<QString,QString> langMap)
{
	QStringList sortList;
	QMap<QString,QString>::Iterator it;
	if (ScMW->ScriptRunning)
		return;
	langCombo->clear();
	for (it = langMap.begin(); it != langMap.end(); ++it)
		sortList.push_back(it.data());
	langCombo->insertStringList(sortQStringList(sortList));
	langCombo->listBox()->setMinimumWidth(langCombo->listBox()->maxItemWidth() + 24);
}

void Mpalette::NewLanguage()
{
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setLanguage(ScMW->GetLang(langCombo->currentText()));
		emit DocChanged();
	}
}

void Mpalette::ManageTabs()
{
	if ((HaveDoc) && (HaveItem))
	{
		PageItem_TextFrame *i2=CurItem->asTextFrame();
		if (i2==0)
			return;
		TabManager *dia = new TabManager(this, doc->unitIndex(), i2->TabValues, i2->columnWidth());
		if (dia->exec())
		{
			CurItem->TabValues = dia->tmpTab;
			ScMW->view->RefreshItem(CurItem);
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
		ScMW->view->RefreshItem(CurItem);
		emit DocChanged();
	}
}

void Mpalette::installSniffer(MSpinBox *spinBox)
{
	const QObjectList* list = spinBox->children();
	if (list)
	{
		QObjectListIterator it(*list);
		QObject *obj;
		while ((obj = it.current()) != 0)
		{
			++it;
			obj->installEventFilter(userActionSniffer);
		}
	}
}

bool Mpalette::userActionOn()
{
	return _userActionOn;
}

void Mpalette::mspinboxStartUserAction()
{
	_userActionOn = true;
}

void Mpalette::mspinboxFinishUserAction()
{
	_userActionOn = false;

	//for (uint i = 0; i < ScMW->view->SelItem.count(); ++i)
	for (uint i = 0; i < doc->selection->count(); ++i)
		//ScMW->view->SelItem.at(i)->checkChanges(true);
		doc->selection->itemAt(i)->checkChanges(true);
	if (ScMW->view->groupTransactionStarted())
	{
		UndoManager::instance()->commit();
		ScMW->view->setGroupTransactionStarted(false);
	}
}

void Mpalette::languageChange()
{
	setCaption( tr("Properties"));
	
	TabStack->setItemLabel(idXYZItem, tr("X, Y, &Z"));
	TabStack->setItemLabel(idTextItem, tr("&Text"));
	TabStack->setItemLabel(idImageItem, tr("&Image"));
	TabStack->setItemLabel(idShapeItem, tr("&Shape"));
	TabStack->setItemLabel(idLineItem, tr("&Line"));
	TabStack->setItemLabel(idColorsItem, tr("&Colors"));
	
	NameGroup->setTitle( tr("Name"));
	GeoGroup->setTitle( tr("Geometry"));
	xposLabel->setText( tr("&X-Pos:"));
	yposLabel->setText( tr("&Y-Pos:"));
	widthLabel->setText( tr("&Width:"));
	heightLabel->setText( tr("&Height:"));
	rotationLabel->setText( tr("&Rotation:"));
	basepointLabel->setText( tr("Basepoint:"));
	LayerGroup->setTitle( tr("Level"));
	SRect->setText( tr("Shape:"));
	EditShape->setText( tr("&Edit Shape..."));
	rndcornersLabel->setText( tr("R&ound\nCorners:"));
	Distance->setTitle( tr("Distance of Text"));
	columnsLabel->setText( tr("Colu&mns:"));
	colgapLabel->setTexts( tr("&Gap:"), tr("&Width:"));
	topLabel->setText( tr("To&p:"));
	bottomLabel->setText( tr("&Bottom:"));
	leftLabel->setText( tr("&Left:"));
	rightLabel->setText( tr("&Right:"));
	TabsButton->setText( tr("T&abulators..."));
	Distance2->setTitle( tr("Path Text Properties"));
	showcurveCheckBox->setText( tr("Show Curve"));
	startoffsetLabel->setText( tr("Start Offset:"));
	distfromcurveLabel->setText( tr("Distance from Curve:"));	
	textFlowsAroundFrame->setTitle( tr("Text &Flows Around Frame"));
	textFlowUsesBoundingBox->setText( tr("Use &Bounding Box"));
	Textflow3->setText( tr("&Use Contour Line"));
	styleLabel->setText( tr("St&yle:"));
	langLabel->setText( tr("Lan&guage:"));
	FreeScale->setText( tr("&Free Scaling"));
	imgDPIXLabel->setText( tr("Actual X-DPI:"));
	imgDPIYLabel->setText( tr("Actual Y-DPI:"));
	xposImgLabel->setText( tr("&X-Pos:"));
	yposImgLabel->setText( tr("&Y-Pos:"));
	xscaleLabel->setText( tr("X-Sc&ale:"));
	yscaleLabel->setText( tr("Y-Scal&e:"));
	FrameScale->setText( tr("Scale &To Frame Size"));
	Aspect->setText( tr("P&roportional"));
	TextCms1->setText( tr("Input Profile:"));
	TextCms2->setText( tr("Rendering Intent:"));
	lineSpacingPop->changeItem(lineSpacingPop->idAt(0), tr("Fixed Linespacing"));
	lineSpacingPop->changeItem(lineSpacingPop->idAt(1), tr("Automatic Linespacing"));
	lineSpacingPop->changeItem(lineSpacingPop->idAt(2), tr("Align to Baseline Grid"));
	int oldMonitorI=MonitorI->currentItem();
	MonitorI->clear();
	MonitorI->insertItem( tr("Perceptual"));
	MonitorI->insertItem( tr("Relative Colorimetric"));
	MonitorI->insertItem( tr("Saturation"));
	MonitorI->insertItem( tr("Absolute Colorimetric"));
	MonitorI->setCurrentItem(oldMonitorI);
	int oldLineMode=LineMode->currentItem();
	LineMode->clear();
	LineMode->insertItem( tr("Left Point"));
	LineMode->insertItem( tr("End Points"));
	LineMode->setCurrentItem(oldLineMode);
	LineModeT->setText( tr("&Basepoint:"));
	linetypeLabel->setText( tr("T&ype of Line:"));
	startArrowText->setText( tr("Start Arrow:"));
	endArrowText->setText( tr("End Arrow:"));
	if (HaveDoc)
	{
		int arrowItem=startArrow->currentItem();
		startArrow->rebuildList(&doc->arrowStyles);
		startArrow->setCurrentItem(arrowItem);
		arrowItem=endArrow->currentItem();
		endArrow->rebuildList(&doc->arrowStyles);
		endArrow->setCurrentItem(arrowItem);
	}
	linewidthLabel->setText( tr("Line &Width:"));
	edgesLabel->setText( tr("Ed&ges:"));
	int oldLJoinStyle=LJoinStyle->currentItem();
	LJoinStyle->clear();
	LJoinStyle->insertItem(loadIcon("MiterJoin.png"), tr("Miter Join"));
	LJoinStyle->insertItem(loadIcon("BevelJoin.png"), tr("Bevel Join"));
	LJoinStyle->insertItem(loadIcon("RoundJoin.png"), tr("Round Join"));
	LJoinStyle->setCurrentItem(oldLJoinStyle);
	
	int oldLEndStyle=LEndStyle->currentItem();
	LEndStyle->clear();
	LEndStyle->insertItem(loadIcon("ButtCap.png"), tr("Flat Cap"));
	LEndStyle->insertItem(loadIcon("SquareCap.png"), tr("Square Cap"));
	LEndStyle->insertItem(loadIcon("RoundCap.png"), tr("Round Cap"));
	LEndStyle->setCurrentItem(oldLEndStyle);
	endingsLabel->setText( tr("&Endings:"));

	TLines->setTitle( tr("Cell Lines"));
	TopLine->setText( tr("Line at Top"));
	LeftLine->setText( tr("Line at the Left"));
	RightLine->setText( tr("Line at the Right "));
	BottomLine->setText( tr("Line at Bottom"));

	QString pctSuffix=tr(" %");
	ChBase->setSuffix(pctSuffix);
	ChScale->setSuffix(pctSuffix);
	ChScaleV->setSuffix(pctSuffix);
	ScaleX->setSuffix(pctSuffix);
	ScaleY->setSuffix(pctSuffix);
	Extra->setSuffix(pctSuffix);
	
	QString ptSuffix = tr(" pt");
	Dist->setSuffix(ptSuffix);
	LineW->setSuffix(ptSuffix);
	Size->setSuffix(ptSuffix);
	LineSp->setSuffix(ptSuffix);
	LSize->setSuffix(ptSuffix);

	QString ein = (HaveDoc) ? unitGetSuffixFromIndex(doc->unitIndex()) : ptSuffix;
		
	Xpos->setSuffix(ein);
	Ypos->setSuffix(ein);
	Width->setSuffix(ein);
	Height->setSuffix(ein);
	LXpos->setSuffix(ein);
	LYpos->setSuffix(ein);
	dGap->setSuffix(ein);
	DLeft->setSuffix(ein);
	DTop->setSuffix(ein);
	DBottom->setSuffix(ein);
	DRight->setSuffix(ein);
	RoundRect->setSuffix(ein);
	
	SeStyle->languageChange();
	GroupAlign->languageChange();
	
	StyledLine->changeItem( tr("No Style"), 0);
	updateCList();
	updateCmsList();

	QToolTip::remove(NameEdit);
	QToolTip::remove(Xpos);
	QToolTip::remove(Ypos);
	QToolTip::remove(Width);
	QToolTip::remove(Height);
	QToolTip::remove(Rot);
	QToolTip::remove(basepointLabel);
	QToolTip::remove(TopLeft);
	QToolTip::remove(TopRight);
	QToolTip::remove(BottomLeft);
	QToolTip::remove(BottomRight);
	QToolTip::remove(Center);
	QToolTip::remove(FlipH);
	QToolTip::remove(FlipV);
	QToolTip::remove(Zup);
	QToolTip::remove(ZDown);
	QToolTip::remove(ZTop);
	QToolTip::remove(ZBottom);
	QToolTip::remove(LevelTxt);
	QToolTip::remove(Locked);
	QToolTip::remove(NoResize);
	QToolTip::remove(NoPrint);
	QToolTip::remove(textFlowsAroundFrame);
	QToolTip::remove(textFlowUsesBoundingBox);
	QToolTip::remove(Textflow3);

	QToolTip::remove(Fonts);
	QToolTip::remove(Size);
	QToolTip::remove(ChBase);
	QToolTip::remove(ChScale);
	QToolTip::remove(ChScaleV);
	QToolTip::remove(TxStroke);
	QToolTip::remove(TxFill);
	QToolTip::remove(PM1);
	QToolTip::remove(PM2);
	QToolTip::remove(Revert);
	QToolTip::remove(Extra);
	QToolTip::remove(LineSp);
	QToolTip::remove(Spal);
	QToolTip::remove(langCombo);

	QToolTip::remove(LineMode);
	QToolTip::remove(LStyle);
	QToolTip::remove(LSize);
	QToolTip::remove(LJoinStyle);
	QToolTip::remove(LEndStyle);
	QToolTip::remove(StyledLine);

	QToolTip::remove(SCustom);
	QToolTip::remove(EditShape);
	QToolTip::remove(RoundRect);
	QToolTip::remove(DCol);
	QToolTip::remove(colgapLabel);
	QToolTip::remove(dGap);
	QToolTip::remove(DTop);
	QToolTip::remove(DBottom);
	QToolTip::remove(DLeft);
	QToolTip::remove(DRight);
	QToolTip::remove(TabsButton);
	
	QToolTip::remove(FreeScale);
	QToolTip::remove(LXpos);
	QToolTip::remove(LYpos);
	QToolTip::remove(ScaleX);
	QToolTip::remove(ScaleY);
	QToolTip::remove(keepImageWHRatioButton);
	QToolTip::remove(keepFrameWHRatioButton);
	QToolTip::remove(FrameScale);
	QToolTip::remove(Aspect);
	QToolTip::remove(InputP);
	QToolTip::remove(MonitorI);
	
	QToolTip::add(NameEdit, tr("Name of selected object"));
	QToolTip::add(Xpos, tr("Horizontal position of current basepoint"));
	QToolTip::add(Ypos, tr("Vertical position of current basepoint"));
	QToolTip::add(Width, tr("Width"));
	QToolTip::add(Height, tr("Height"));
	QToolTip::add(Rot, tr("Rotation of object at current basepoint"));
	QToolTip::add(basepointLabel, tr("Point from which measurements or rotation angles are referenced"));
	QToolTip::add(TopLeft, tr("Select top left for basepoint"));
	QToolTip::add(TopRight, tr("Select top right for basepoint"));
	QToolTip::add(BottomLeft, tr("Select bottom left for basepoint"));
	QToolTip::add(BottomRight, tr("Select bottom right for basepoint"));
	QToolTip::add(Center, tr("Select center for basepoint"));
	QToolTip::add(FlipH, tr("Flip Horizontal"));
	QToolTip::add(FlipV, tr("Flip Vertical"));
	QToolTip::add(Zup, tr("Move one level up"));
	QToolTip::add(ZDown, tr("Move one level down"));
	QToolTip::add(ZTop, tr("Move to front"));
	QToolTip::add(ZBottom, tr("Move to back"));
	QToolTip::add(LevelTxt, tr("Indicates the level the object is on, 0 means the object is at the bottom"));
	QToolTip::add(Locked, tr("Lock or unlock the object"));
	QToolTip::add(NoResize, tr("Lock or unlock the size of the object"));
	QToolTip::add(NoPrint, tr("Enable or disable printing of the object"));
	QToolTip::add(textFlowsAroundFrame, tr("Make text in lower frames flow around the object shape"));
	QToolTip::add(textFlowUsesBoundingBox, tr("Use a surrounding box instead of the frame's shape for text flow"));
	QToolTip::add(Textflow3, tr("Use a second line originally based on the frame's shape for text flow"));

	QToolTip::add(Fonts, tr("Font of selected text or object"));
	QToolTip::add(Size, tr("Font Size"));
	QToolTip::add(ChBase, tr("Offset to baseline of characters"));
	QToolTip::add(ChScale, tr("Scaling width of characters"));
	QToolTip::add(ChScaleV, tr("Scaling height of characters"));
	QToolTip::add(TxStroke, tr("Color of text stroke. Only available with \"outline\" text decoration."));
	QToolTip::add(TxFill, tr("Color of text fill. Only available with \"outline\" text decoration."));
	QToolTip::add(PM1, tr("Saturation of color of text stroke"));
	QToolTip::add(PM2, tr("Saturation of color of text fill"));
	QToolTip::add(Revert, tr("Right to Left Writing"));
	QToolTip::add(Extra, tr("Manual Tracking"));
	QToolTip::add(LineSp, tr("Line Spacing"));
	QToolTip::add(Spal, tr("Style of current paragraph"));
	QToolTip::add(langCombo, tr("Hyphenation language of frame"));

	QToolTip::add(LineMode, tr("Change settings for left or end points"));
	QToolTip::add(LStyle, tr("Pattern of line"));
	QToolTip::add(LSize, tr("Thickness of line"));
	QToolTip::add(LJoinStyle, tr("Type of line joins"));
	QToolTip::add(LEndStyle, tr("Type of line end"));
	QToolTip::add(StyledLine, tr("Line style of current object"));

	QToolTip::add(SCustom, tr("Choose the shape of frame..."));
	QToolTip::add(EditShape, tr("Edit shape of the frame..."));
	QToolTip::add(RoundRect, tr("Set radius of corner rounding"));
	QToolTip::add(DCol, tr("Number of columns in text frame"));
	QToolTip::add(colgapLabel, tr("Switches between Gap or Column width"));
	QToolTip::add(dGap, tr("Distance between columns"));
	QToolTip::add(DTop, tr("Distance of text from top of frame"));
	QToolTip::add(DBottom, tr("Distance of text from bottom of frame"));
	QToolTip::add(DLeft, tr("Distance of text from left of frame"));
	QToolTip::add(DRight, tr("Distance of text from right of frame"));
	QToolTip::add(TabsButton, tr("Edit tab settings of text frame..."));

	QToolTip::add(FreeScale, tr("Allow the image to be a different size to the frame"));
	QToolTip::add(LXpos, tr("Horizontal offset of image within frame"));
	QToolTip::add(LYpos, tr("Vertical offset of image within frame"));
	QToolTip::add(ScaleX, tr("Resize the image horizontally"));
	QToolTip::add(ScaleY, tr("Resize the image vertically"));
	QToolTip::add(keepImageWHRatioButton, tr("Keep the X and Y scaling the same"));
	QToolTip::add(keepFrameWHRatioButton, tr("Keep the aspect ratio"));
	QToolTip::add(FrameScale, tr("Make the image fit within the size of the frame"));
	QToolTip::add(Aspect, tr("Use image proportions rather than those of the frame"));
	QToolTip::add(InputP, tr("Source profile of the image"));
	QToolTip::add(MonitorI, tr("Rendering intent for the image"));	
}


const VGradient Mpalette::getFillGradient()
{
	return Cpal->gradEdit->Preview->fill_gradient;
}

void Mpalette::setGradientEditMode(const bool on)
{
	Cpal->gradEditButton->setOn(on);
}

void Mpalette::updateColorSpecialGradient()
{
	if (!HaveDoc)
		return;
	if(doc->selection->count()==0)
		return;
	double dur=doc->unitRatio();
	PageItem *currItem=doc->selection->itemAt(0);
	Cpal->setSpecialGradient(currItem->GrStartX * dur, currItem->GrStartY * dur,
							currItem->GrEndX * dur, currItem->GrEndY * dur,
							currItem->width() * dur, currItem->height() * dur);
}

void Mpalette::updateSpinBoxConstants()
{
	if (!HaveDoc)
		return;
	if(doc->selection->count()==0)
		return;
	Width->setConstants(doc->constants());
	Height->setConstants(doc->constants());
	Xpos->setConstants(doc->constants());
	Ypos->setConstants(doc->constants());
}

UserActionSniffer::UserActionSniffer() : QObject (this)
{

}

bool UserActionSniffer::eventFilter(QObject*, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
		emit actionStart();
	else if (e->type() == QEvent::MouseButtonRelease)
		emit actionEnd();
	else if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent *k = dynamic_cast<QKeyEvent*>(e);
		if (k && !k->isAutoRepeat() && (k->key() == Key_Up || k->key() == Key_Down))
			emit actionStart();
	}
	else if (e->type() == QEvent::KeyRelease)
	{
		QKeyEvent *k = dynamic_cast<QKeyEvent*>(e);
		if (k && !k->isAutoRepeat() && (k->key() == Key_Up || k->key() == Key_Down))
			emit actionEnd();
	}
	return false;
}
