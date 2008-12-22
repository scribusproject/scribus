/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "propertiespalette.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QEvent>
#include <QFocusEvent>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QListView>
#include <QMatrix>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExp>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTimer>
#include <QToolBox>
#include <QToolTip>
#include <QVBoxLayout>
#include <QValidator>
#include <QWidget>


#include <cmath>
#include "arrowchooser.h"
#include "autoform.h"
#include "basepointwidget.h"
#include "commonstrings.h"
#include "colorlistbox.h"
#include "sccolorengine.h"
#include "cpalette.h"
#include "pageitem_textframe.h"
#include "sccombobox.h"
#include "scfonts.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scraction.h"
#include "scribusview.h"
#include "selection.h"
#include "spalette.h"
#include "styleselect.h"
#include "tabmanager.h"
#include "units.h"
#include "undomanager.h"
#include "util.h"
#include "util_icon.h"
#include "text/nlsconfig.h"
#include "dasheditor.h"

using namespace std;


LineFormatValue::LineFormatValue() : m_Line(), m_doc(NULL), m_name() {};

LineFormatValue::LineFormatValue( const multiLine& line, ScribusDoc* doc, const QString name ) : m_Line(line), m_doc(doc), m_name(name) {};

LineFormatValue::LineFormatValue(const LineFormatValue& other)
{
	m_name = other.m_name;
	m_Line = other.m_Line;
	m_doc = other.m_doc;
}

LineFormatValue& LineFormatValue::operator= (const LineFormatValue& other)
{
	m_name = other.m_name;
	m_Line = other.m_Line;
	m_doc = other.m_doc;
	return *this;
}


void LineFormatItemDelegate::redraw(const QVariant& data) const  
{
	const LineFormatValue& item(data.value<LineFormatValue>());
	QColor tmpf;
	pmap->fill(Qt::white);
	QPainter p;
	p.begin(pmap.get());
	for (int its = item.m_Line.size()-1; its > -1; its--)
	{
		const ScColor& col = item.m_doc->PageColors[item.m_Line[its].Color];
		tmpf = ScColorEngine::getDisplayColor(col, item.m_doc, item.m_Line[its].Shade);
		QPen pen;
		QVector<double> m_array;
		if (item.m_Line[its].Dash == 1)
			pen.setStyle(Qt::SolidLine);
		else
		{
			getDashArray(item.m_Line[its].Dash, 1, m_array);
			pen.setDashPattern(m_array);
		}
		pen.setColor(tmpf);
		pen.setWidth(qMax(static_cast<int>(item.m_Line[its].Width), 1));
		pen.setCapStyle(static_cast<Qt::PenCapStyle>(item.m_Line[its].LineEnd));
		pen.setJoinStyle(static_cast<Qt::PenJoinStyle>(item.m_Line[its].LineJoin));
		p.setPen(pen);
		p.drawLine(0, 18, 37, 18);
	}
	p.end();
}

QString LineFormatItemDelegate::text(const QVariant& data) const
{
	return data.toString();
}

NameWidget::NameWidget(QWidget* parent) : QLineEdit(parent)
{
	setObjectName("namewidget");
	QRegExp rx( "\\w+" );
	QValidator* validator = new QRegExpValidator( rx, this );
	setValidator( validator );
}

void NameWidget::focusOutEvent(QFocusEvent *e)
{
	emit Leaved();
	QLineEdit::focusOutEvent(e);
}

PropertiesPalette::PropertiesPalette( QWidget* parent) : ScrPaletteBase( parent, "PropertiesPalette", false, 0)
{
	m_ScMW=0;
	doc=0;
	HaveDoc = false;
	HaveItem = false;
	RoVal = 0;
	m_unitRatio = 1.0;
	tmpSelection = new Selection(this, false);
	tmpSelection->clear();
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	_userActionOn = false;
	userActionSniffer = new UserActionSniffer(this);
	connect(userActionSniffer, SIGNAL(actionStart()), this, SLOT(spinboxStartUserAction()));
	connect(userActionSniffer, SIGNAL(actionEnd()), this, SLOT(spinboxFinishUserAction()));

	MpalLayout = new QVBoxLayout( this );
	MpalLayout->setMargin(5);
	MpalLayout->setSpacing(1);
	setOrientation(Qt::Vertical);
	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);

	TabStack = new QToolBox( this );

	page = new QWidget( TabStack );
	pageLayout = new QVBoxLayout( page );
	pageLayout->setMargin(0);
	pageLayout->setSpacing(5);

	NameGroup = new QGroupBox( page );
	NameGroupLayout = new QHBoxLayout( NameGroup );
	NameGroupLayout->setMargin(5);
	NameGroupLayout->setSpacing(4);
	NameGroupLayout->setAlignment( Qt::AlignTop );
	NameEdit = new NameWidget(NameGroup);
	NameEdit->setFocusPolicy(Qt::ClickFocus);
	NameGroupLayout->addWidget( NameEdit );
	pageLayout->addWidget( NameGroup );

	GeoGroup = new QGroupBox(page);
	GeoGroupLayout = new QGridLayout( GeoGroup );
	GeoGroupLayout->setMargin(5);
	GeoGroupLayout->setSpacing(4);
	GeoGroupLayout->setAlignment( Qt::AlignTop );

	Xpos = new ScrSpinBox( -3000, 3000, GeoGroup, 0 );
	installSniffer(Xpos);
	GeoGroupLayout->addWidget( Xpos, 0, 1 );
	Ypos = new ScrSpinBox( -3000, 3000, GeoGroup, 0 );
	installSniffer(Ypos);
	GeoGroupLayout->addWidget( Ypos, 1, 1 );
	Width = new ScrSpinBox( GeoGroup, 0 );
	installSniffer(Width);
	GeoGroupLayout->addWidget( Width, 2, 1 );
	Height = new ScrSpinBox( GeoGroup, 0 );
	installSniffer(Height);
	GeoGroupLayout->addWidget( Height, 3, 1 );

	xposLabel = new QLabel( "&X-Pos:", GeoGroup );
	xposLabel->setBuddy(Xpos);
	GeoGroupLayout->addWidget( xposLabel, 0, 0 );
	yposLabel = new QLabel( "&Y-Pos:", GeoGroup );
	yposLabel->setBuddy(Ypos);
	GeoGroupLayout->addWidget( yposLabel, 1, 0 );
	widthLabel = new QLabel( "&Width:", GeoGroup );
	widthLabel->setBuddy(Width);
	GeoGroupLayout->addWidget( widthLabel, 2, 0 );
	heightLabel = new QLabel( "&Height:", GeoGroup );
	heightLabel->setBuddy(Height);
	GeoGroupLayout->addWidget( heightLabel, 3, 0 );

	keepFrameWHRatioButton = new LinkButton( GeoGroup );
	keepFrameWHRatioButton->setCheckable( true );
	keepFrameWHRatioButton->setAutoRaise( true );
	keepFrameWHRatioButton->setMaximumSize( QSize( 15, 32767 ) );
	keepFrameWHRatioButton->setChecked(true);
	GeoGroupLayout->addWidget( keepFrameWHRatioButton, 2, 2, 2, 1 );
	Rotation = new ScrSpinBox( GeoGroup, 6);
	Rotation->setWrapping( true );
	installSniffer(Rotation);
	rotationLabel = new QLabel( "&Rotation:", GeoGroup );
	rotationLabel->setBuddy(Rotation);
	GeoGroupLayout->addWidget( rotationLabel, 4, 0 );
	GeoGroupLayout->addWidget( Rotation, 4, 1 );
	basepointLabel = new QLabel( "Basepoint:", GeoGroup );
	GeoGroupLayout->addWidget( basepointLabel, 5, 0 );
	RotationGroup = new BasePointWidget(GeoGroup, 0);
/*	RotationGroup = new QButtonGroup( GeoGroup );
	Layout12 = new QGridLayout;
	Layout12->setMargin(0);
	Layout12->setSpacing(0);
	TopLeft = new QRadioButton( GeoGroup );
	RotationGroup->addButton(TopLeft, 0);
	TopLeft->setText( "" );
	TopLeft->setChecked( true );
	TopLeft->setLayoutDirection(Qt::RightToLeft);
	TopLeft->setMaximumSize( TopLeft->iconSize() );
	Layout12->addWidget( TopLeft, 0, 0, Qt::AlignCenter );
	Line1 = new QFrame( GeoGroup);
	Line1->setMinimumSize( QSize( 20, 4 ) );
	Line1->setMaximumSize( QSize( 20, 4 ) );
	Line1->setFrameShape( QFrame::HLine );
	Line1->setFrameShadow( QFrame::Plain );
	Line1->setLineWidth( 3 );
	Line1->setFrameShape( QFrame::HLine );
	Layout12->addWidget( Line1, 0, 1, Qt::AlignCenter );
	TopRight = new QRadioButton( GeoGroup );
	RotationGroup->addButton(TopRight, 1);
	TopRight->setText( "" );
	TopRight->setMaximumSize( TopRight->iconSize() );
	Layout12->addWidget( TopRight, 0, 2, Qt::AlignCenter );
	Line2 = new QFrame( GeoGroup );
	Line2->setMinimumSize( QSize( 4, 20 ) );
	Line2->setMaximumSize( QSize( 4, 20 ) );
	Line2->setFrameShape( QFrame::VLine );
	Line2->setFrameShadow( QFrame::Plain );
	Line2->setLineWidth( 3 );
	Line2->setFrameShape( QFrame::VLine );
	Layout12->addWidget( Line2, 1, 0, Qt::AlignCenter );
	Center = new QRadioButton( GeoGroup );
	RotationGroup->addButton(Center, 2);
	Center->setText( "" );
	Center->setMaximumSize( Center->iconSize() );
	Layout12->addWidget( Center, 1, 1, Qt::AlignCenter );
	Line4 = new QFrame( GeoGroup );
	Line4->setMinimumSize( QSize( 4, 20 ) );
	Line4->setMaximumSize( QSize( 4, 20 ) );
	Line4->setFrameShadow( QFrame::Plain );
	Line4->setLineWidth( 3 );
	Line4->setFrameShape( QFrame::VLine );
	Layout12->addWidget( Line4, 1, 2, Qt::AlignCenter );
	BottomLeft = new QRadioButton( GeoGroup );
	RotationGroup->addButton(BottomLeft, 3);
	BottomLeft->setText( "" );
	BottomLeft->setLayoutDirection(Qt::RightToLeft);
	BottomLeft->setMaximumSize( BottomLeft->iconSize() );
	Layout12->addWidget( BottomLeft, 2, 0, Qt::AlignCenter );
	Line5 = new QFrame( GeoGroup );
	Line5->setMinimumSize( QSize( 20, 4 ) );
	Line5->setMaximumSize( QSize( 20, 4 ) );
	Line5->setFrameShape( QFrame::HLine );
	Line5->setFrameShadow( QFrame::Plain );
	Line5->setLineWidth( 3 );
	Line5->setFrameShape( QFrame::HLine );
	Layout12->addWidget( Line5, 2, 1, Qt::AlignCenter );
	BottomRight = new QRadioButton( GeoGroup );
	RotationGroup->addButton(BottomRight, 4);
	BottomRight->setText( "" );
	BottomRight->setMaximumSize( BottomRight->iconSize() );
	Layout12->addWidget( BottomRight, 2, 2, Qt::AlignCenter );
	GeoGroupLayout->addLayout( Layout12, 5, 1, 1, 1, Qt::AlignLeft); */
	GeoGroupLayout->addWidget( RotationGroup, 5, 1, 1, 1, Qt::AlignLeft);
	pageLayout->addWidget( GeoGroup );

	layout60 = new QHBoxLayout;
	layout60->setMargin(0);
	layout60->setSpacing(5);

	LayerGroup = new QGroupBox( "Level", page );
	LayerGroupLayout = new QGridLayout( LayerGroup );
	LayerGroupLayout->setSpacing( 4 );
	LayerGroupLayout->setMargin( 5 );
	LayerGroupLayout->setAlignment( Qt::AlignTop );
	Zup = new QToolButton( LayerGroup );
	Zup->setMaximumSize( QSize( 22, 22 ) );
	Zup->setIcon(QIcon(loadIcon("16/go-up.png")));
	LayerGroupLayout->addWidget( Zup, 0, 0 );
	ZDown = new QToolButton( LayerGroup );
	ZDown->setMaximumSize( QSize( 22, 22 ) );
	ZDown->setIcon(QIcon(loadIcon("16/go-down.png")));
	LayerGroupLayout->addWidget( ZDown, 1, 0 );
	ZTop = new QToolButton( LayerGroup );
	ZTop->setMaximumSize( QSize( 22, 22 ) );
	ZTop->setIcon(QIcon(loadIcon("16/go-top.png")));
	LayerGroupLayout->addWidget( ZTop, 0, 1 );
	ZBottom = new QToolButton( LayerGroup );
	ZBottom->setMaximumSize( QSize( 22, 22 ) );
	ZBottom->setIcon(QIcon(loadIcon("16/go-bottom.png")));
	LayerGroupLayout->addWidget( ZBottom, 1, 1 );
	LevelTxt = new QLabel( "  1", LayerGroup );
	LevelTxt->setAlignment( Qt::AlignCenter );
	LayerGroupLayout->addWidget( LevelTxt, 0, 2, 2, 1 );

	layout60->addWidget( LayerGroup );
	QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout60->addItem( spacer2 );

	Layout44 = new QGridLayout;
	Layout44->setSpacing( 4 );
	Layout44->setMargin( 5 );

	DoGroup = new QToolButton( page );
	DoGroup->setIcon(QIcon(loadIcon("group.png")));
	Layout44->addWidget( DoGroup, 0, 0 );
	DoUnGroup = new QToolButton( page );
	DoUnGroup->setIcon(QIcon(loadIcon("ungroup.png")));
	Layout44->addWidget( DoUnGroup, 1, 0 );

	FlipH = new QToolButton( page );
	FlipH->setIcon(QIcon(loadIcon("16/flip-object-horizontal.png")));
	FlipH->setCheckable( true );
	Layout44->addWidget( FlipH, 0, 1 );
	FlipV = new QToolButton( page );
	FlipV->setIcon(QIcon(loadIcon("16/flip-object-vertical.png")));
	FlipV->setCheckable( true );
	Layout44->addWidget( FlipV, 1, 1 );
	Locked = new QToolButton( page );
	Locked->setCheckable( true );
	QIcon a = QIcon();
	a.addPixmap(loadIcon("16/lock.png"), QIcon::Normal, QIcon::On);
	a.addPixmap(loadIcon("16/lock-unlocked.png"), QIcon::Normal, QIcon::Off);
	Locked->setIcon(a);
	Layout44->addWidget( Locked, 0, 2 );
	NoPrint = new QToolButton( page );
	NoPrint->setCheckable( true );
	QIcon a2 = QIcon();
	a2.addPixmap(loadIcon("NoPrint.png"), QIcon::Normal, QIcon::On);
	a2.addPixmap(loadIcon("16/document-print.png"), QIcon::Normal, QIcon::Off);
	NoPrint->setIcon(a2);
	Layout44->addWidget( NoPrint, 1, 2 );
	NoResize = new QToolButton( page );
	NoResize->setCheckable( true );
	QIcon a3 = QIcon();
	a3.addPixmap(loadIcon("framenoresize.png"), QIcon::Normal, QIcon::On);
	a3.addPixmap(loadIcon("frameresize.png"), QIcon::Normal, QIcon::Off);
	NoResize->setIcon(a3);
	Layout44->addWidget( NoResize, 0, 3 );
	layout60->addLayout( Layout44 );

	pageLayout->addLayout( layout60 );

	QSpacerItem* spacer13 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout->addItem( spacer13 );
	idXYZItem = TabStack->addItem( page, "X, Y, &Z" );

	page_2 = new QWidget( TabStack );
	pageLayout_2 = new QVBoxLayout( page_2 );
	pageLayout_2->setSpacing( 5 );
	pageLayout_2->setMargin( 0 );

	ShapeGroup = new QGroupBox( page_2 );
	ShapeGroupLayout = new QHBoxLayout( ShapeGroup );
	ShapeGroupLayout->setSpacing( 2 );
	ShapeGroupLayout->setMargin( 0 );
	ShapeGroupLayout->setAlignment( Qt::AlignTop );
	SRect = new QLabel( "Shape:", ShapeGroup );
	ShapeGroupLayout->addWidget( SRect );
	SCustom = new Autoforms( ShapeGroup );
	ShapeGroupLayout->addWidget( SCustom );

	EditShape = new QToolButton( ShapeGroup );
//	EditShape->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5)));
	ShapeGroupLayout->addWidget( EditShape );
	pageLayout_2->addWidget( ShapeGroup );

	Layout13 = new QHBoxLayout;
	Layout13->setSpacing( 2 );
	Layout13->setMargin( 0 );

	RoundRect = new ScrSpinBox( page_2, 0 );
	rndcornersLabel = new QLabel( "R&ound\nCorners:", page_2 );
	rndcornersLabel->setBuddy(RoundRect);
	Layout13->addWidget( rndcornersLabel );
	Layout13->addWidget( RoundRect );
	pageLayout_2->addLayout( Layout13 );

	textFlowOptions = new QGroupBox( page_2 );
	textFlowOptionsLayout = new QVBoxLayout( textFlowOptions );
	textFlowOptionsLayout->setSpacing( 5 );
	textFlowOptionsLayout->setMargin( 5 );
	textFlowOptionsLayout->setAlignment( Qt::AlignTop );
	textFlowDisabled = new QToolButton( textFlowOptions );
	textFlowDisabled->setCheckable( true );
	textFlowDisabled->setAutoExclusive(true);
	textFlowDisabled->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowDisabled->setText( "Disabled" );
	textFlowDisabled->setIcon(QIcon(loadIcon("flow-none.png")));
	textFlowOptionsLayout->addWidget( textFlowDisabled );
	textFlowUsesFrameShape  = new QToolButton( textFlowOptions );
	textFlowUsesFrameShape->setCheckable( true );
	textFlowUsesFrameShape->setAutoExclusive(true);
	textFlowUsesFrameShape->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowUsesFrameShape->setText( "Use &Frame Shape" );
	textFlowUsesFrameShape->setIcon(QIcon(loadIcon("flow-frame.png")));
	textFlowOptionsLayout->addWidget( textFlowUsesFrameShape );
	textFlowUsesBoundingBox = new QToolButton( textFlowOptions );
	textFlowUsesBoundingBox->setCheckable( true );
	textFlowUsesBoundingBox->setAutoExclusive(true);
	textFlowUsesBoundingBox->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowUsesBoundingBox->setText( "Use &Bounding Box" );
	textFlowUsesBoundingBox->setIcon(QIcon(loadIcon("flow-bounding.png")));
	textFlowOptionsLayout->addWidget( textFlowUsesBoundingBox );
	textFlowUsesContourLine = new QToolButton( textFlowOptions );
	textFlowUsesContourLine->setCheckable( true );
	textFlowUsesContourLine->setAutoExclusive(true);
	textFlowUsesContourLine->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowUsesContourLine->setText( "&Use Contour Line" );
	textFlowUsesContourLine->setIcon(QIcon(loadIcon("flow-contour.png")));
	textFlowOptionsLayout->addWidget( textFlowUsesContourLine );
	textFlowUsesImageClipping = new QToolButton( textFlowOptions );
	textFlowUsesImageClipping->setCheckable( true );
	textFlowUsesImageClipping->setAutoExclusive(true);
	textFlowUsesImageClipping->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowUsesImageClipping->setText( "Use Image Clip Path" );
	textFlowUsesImageClipping->setIcon(QIcon(loadIcon("flow-contour.png")));
	textFlowOptionsLayout->addWidget( textFlowUsesImageClipping );
	pageLayout_2->addWidget( textFlowOptions  );
	textFlowDisabled->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesFrameShape->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesBoundingBox->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesContourLine->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesImageClipping->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowOptionsB = new QButtonGroup( page_2 );
	textFlowOptionsB->addButton(textFlowDisabled, 0);
	textFlowOptionsB->addButton(textFlowUsesFrameShape, 1);
	textFlowOptionsB->addButton(textFlowUsesBoundingBox, 2);
	textFlowOptionsB->addButton(textFlowUsesContourLine, 3);
	textFlowOptionsB->addButton(textFlowUsesImageClipping, 4);

	TabStack2 = new QStackedWidget( page_2 );

	page_2a = new QWidget( TabStack2 );
/*	pageLayout_2a = new QVBoxLayout( page_2a );
	pageLayout_2a->setSpacing( 5 );
	pageLayout_2a->setMargin( 0 );
	Distance = new QGroupBox( page_2a );
	DistanceLayout = new QGridLayout( Distance );
	DistanceLayout->setSpacing( 2 );
	DistanceLayout->setMargin( 5 );
	DistanceLayout->setAlignment( Qt::AlignTop );

	DCol = new QSpinBox(Distance );
	DCol->setMaximum( 3000 );
	DCol->setMinimum( 1 );
	columnsLabel = new QLabel( "Colu&mns:", Distance );
	columnsLabel->setBuddy(DCol);
	DistanceLayout->addWidget( columnsLabel, 0, 0 );
	DistanceLayout->addWidget( DCol, 0, 1 );

	dGap = new ScrSpinBox( 0, 300, Distance, 0 );
	colgapLabel = new ScComboBox( Distance );
	DistanceLayout->addWidget( colgapLabel, 1, 0); //, Qt::AlignLeft );
	DistanceLayout->addWidget( dGap, 1, 1 );

	DTop = new ScrSpinBox( 0, 300, Distance, 0 );
	topLabel = new QLabel( "To&p:", Distance );
	topLabel->setBuddy(DTop);
	DistanceLayout->addWidget( topLabel, 2, 0 );
	DistanceLayout->addWidget( DTop, 2, 1 );

	DBottom = new ScrSpinBox( 0, 300, Distance, 0 );
	bottomLabel = new QLabel( "&Bottom:", Distance );
	bottomLabel->setBuddy(DBottom);
	DistanceLayout->addWidget( bottomLabel, 3, 0 );
	DistanceLayout->addWidget( DBottom, 3, 1 );

	DLeft = new ScrSpinBox( 0, 300, Distance, 0 );
	leftLabel = new QLabel( "&Left:", Distance );
	leftLabel->setBuddy(DLeft);
	DistanceLayout->addWidget( leftLabel, 4, 0 );
	DistanceLayout->addWidget( DLeft, 4, 1 );

	DRight = new ScrSpinBox( 0, 300, Distance, 0 );
	rightLabel = new QLabel( "&Right:", Distance );
	rightLabel->setBuddy(DRight);
	DistanceLayout->addWidget( rightLabel, 5, 0 );
	DistanceLayout->addWidget( DRight, 5, 1 );

	TabsButton = new QToolButton( Distance );
	TabsButton->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	DistanceLayout->addWidget( TabsButton, 6, 0, 1, 2 );
	pageLayout_2a->addWidget(Distance);

	flopBox = new QGroupBox(tr("First Line Offset"), page_2a);
	flopLayout = new QGridLayout(flopBox);
	flopGroup = new QButtonGroup(flopBox);
	flopRealHeight = new QRadioButton(tr("Maximum Ascent"), flopBox);
	flopFontAscent = new QRadioButton(tr("Font Ascent"), flopBox);
	flopLineSpacing = new QRadioButton(tr("Line Spacing"),flopBox); 
	flopGroup->addButton(flopRealHeight, 0);
	flopGroup->addButton(flopFontAscent, 1);
	flopGroup->addButton(flopLineSpacing, 2);
	flopLayout->addWidget(flopRealHeight);
	flopLayout->addWidget(flopFontAscent);
	flopLayout->addWidget(flopLineSpacing);
	flopRealHeight->setChecked(true);
	pageLayout_2a->addWidget(flopBox); */
	TabStack2->addWidget( page_2a );

/*	page_2b = new QWidget( TabStack2 );
	pageLayout_2b = new QVBoxLayout( page_2b );
	pageLayout_2b->setSpacing( 5 );
	pageLayout_2b->setMargin( 0 );
	Distance2 = new QGroupBox( "Path Text Properties", page_2b );
	DistanceLayout2 = new QGridLayout( Distance2 );
	DistanceLayout2->setSpacing( 2 );
	DistanceLayout2->setMargin( 5 );
	DistanceLayout2->setAlignment( Qt::AlignTop );

	pathTextType = new ScComboBox( Distance2 );
	DistanceLayout2->addWidget( pathTextType, 0, 1);
	pathTextTypeLabel = new QLabel( "Type:", Distance2 );
	DistanceLayout2->addWidget( pathTextTypeLabel, 0, 0);
	
	startoffsetLabel = new QLabel( "Start Offset:", Distance2 );
	DistanceLayout2->addWidget( startoffsetLabel, 1, 0);
	Dist = new ScrSpinBox( 0, 30000, Distance2, 0 );
	Dist->setSingleStep(10);
	DistanceLayout2->addWidget( Dist, 1, 1);

	distfromcurveLabel = new QLabel( "Distance from Curve:", Distance2 );
	DistanceLayout2->addWidget( distfromcurveLabel, 2, 0);
	LineW = new ScrSpinBox( -300, 300, Distance2, 0 );
	LineW->setSingleStep(10);
	DistanceLayout2->addWidget( LineW, 2, 1);

	flippedPathText = new QCheckBox( Distance2 );
	flippedPathText->setText( "Flip Text" );
	DistanceLayout2->addWidget( flippedPathText, 3, 0, 1, 2 );

	showcurveCheckBox = new QCheckBox( Distance2 );
	showcurveCheckBox->setText( "Show Curve" );
	DistanceLayout2->addWidget( showcurveCheckBox, 4, 0, 1, 2 );

	pageLayout_2b->addWidget(Distance2);
	TabStack2->addWidget( page_2b );
*/
	page_2c = new QWidget( TabStack2 );
	pageLayout_2c = new QVBoxLayout( page_2c );
	pageLayout_2c->setSpacing( 5 );
	pageLayout_2c->setMargin( 0 );
	Distance3 = new QGroupBox( page_2c );
	DistanceLayout3 = new QVBoxLayout( Distance3 );
	DistanceLayout3->setSpacing( 2 );
	DistanceLayout3->setMargin( 5 );
	DistanceLayout3->setAlignment( Qt::AlignTop );
	EvenOdd = new QRadioButton( "Even-Odd", Distance3 );
	DistanceLayout3->addWidget( EvenOdd );
	NonZero = new QRadioButton( "Non Zero", Distance3 );
	DistanceLayout3->addWidget( NonZero );
	EvenOdd->setChecked( true );
	Distance3->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
	pageLayout_2c->addWidget(Distance3);
	TabStack2->addWidget( page_2c );

	pageLayout_2->addWidget( TabStack2 );

	QSpacerItem* spacer6 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout_2->addItem( spacer6 );
	idShapeItem = TabStack->addItem( page_2, "&Shape" );

	page_group = new QWidget(TabStack);
	page_group_layout = new QVBoxLayout( page_group );
	page_group_layout->setSpacing( 5 );
	page_group_layout->setMargin( 0 );

	ShapeGroup2 = new QGroupBox( page_group );
	ShapeGroupLayout2 = new QHBoxLayout( ShapeGroup2 );
	ShapeGroupLayout2->setSpacing( 2 );
	ShapeGroupLayout2->setMargin( 0 );
	ShapeGroupLayout2->setAlignment( Qt::AlignTop );
	SRect2 = new QLabel( "Shape:", ShapeGroup2 );
	ShapeGroupLayout2->addWidget( SRect2 );
	SCustom2 = new Autoforms( ShapeGroup2 );
	ShapeGroupLayout2->addWidget( SCustom2 );
	EditShape2 = new QToolButton( ShapeGroup2 );
//	EditShape2->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	ShapeGroupLayout2->addWidget( EditShape2 );
	page_group_layout->addWidget( ShapeGroup2 );

	textFlowOptions2 = new QGroupBox( page_group );
	textFlowOptionsLayout2 = new QVBoxLayout( textFlowOptions2 );
	textFlowOptionsLayout2->setSpacing( 5 );
	textFlowOptionsLayout2->setMargin( 5 );
	textFlowOptionsLayout2->setAlignment( Qt::AlignTop );
	textFlowDisabled2 = new QToolButton( textFlowOptions2 );
	textFlowDisabled2->setCheckable( true );
	textFlowDisabled2->setAutoExclusive(true);
	textFlowDisabled2->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowDisabled2->setText( "Disabled" );
	textFlowDisabled2->setIcon(QIcon(loadIcon("flow-none.png")));
	textFlowOptionsLayout2->addWidget( textFlowDisabled2 );
	textFlowUsesFrameShape2  = new QToolButton( textFlowOptions2 );
	textFlowUsesFrameShape2->setCheckable( true );
	textFlowUsesFrameShape2->setAutoExclusive(true);
	textFlowUsesFrameShape2->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowUsesFrameShape2->setText( "Use &Frame Shape" );
	textFlowUsesFrameShape2->setIcon(QIcon(loadIcon("flow-frame.png")));
	textFlowOptionsLayout2->addWidget( textFlowUsesFrameShape2 );
	textFlowUsesBoundingBox2 = new QToolButton( textFlowOptions2 );
	textFlowUsesBoundingBox2->setCheckable( true );
	textFlowUsesBoundingBox2->setAutoExclusive(true);
	textFlowUsesBoundingBox2->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowUsesBoundingBox2->setText( "Use &Bounding Box" );
	textFlowUsesBoundingBox2->setIcon(QIcon(loadIcon("flow-bounding.png")));
	textFlowOptionsLayout2->addWidget( textFlowUsesBoundingBox2 );
	textFlowUsesContourLine2 = new QToolButton( textFlowOptions2 );
	textFlowUsesContourLine2->setCheckable( true );
	textFlowUsesContourLine2->setAutoExclusive(true);
	textFlowUsesContourLine2->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowUsesContourLine2->setText( "&Use Contour Line" );
	textFlowUsesContourLine2->setIcon(QIcon(loadIcon("flow-contour.png")));
	textFlowOptionsLayout2->addWidget( textFlowUsesContourLine2 );
	textFlowUsesImageClipping2 = new QToolButton( textFlowOptions2 );
	textFlowUsesImageClipping2->setCheckable( true );
	textFlowUsesImageClipping2->setAutoExclusive(true);
	textFlowUsesImageClipping2->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	textFlowUsesImageClipping2->setText( "Use Image Clip Path" );
	textFlowUsesImageClipping2->setIcon(QIcon(loadIcon("flow-contour.png")));
	textFlowOptionsLayout2->addWidget( textFlowUsesImageClipping2 );
	page_group_layout->addWidget( textFlowOptions2  );
	textFlowDisabled2->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesFrameShape2->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesBoundingBox2->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesContourLine2->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesImageClipping2->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowOptionsB2 = new QButtonGroup( page_group );
	textFlowOptionsB2->addButton(textFlowDisabled2, 0);
	textFlowOptionsB2->addButton(textFlowUsesFrameShape2, 1);
	textFlowOptionsB2->addButton(textFlowUsesBoundingBox2, 2);
	textFlowOptionsB2->addButton(textFlowUsesContourLine2, 3);
	textFlowOptionsB2->addButton(textFlowUsesImageClipping2, 4);

	TransGroup = new QGroupBox( tr( "Transparency Settings" ), page_group );
	Layout1t = new QGridLayout( TransGroup );
	Layout1t->setAlignment( Qt::AlignTop );
	Layout1t->setSpacing( 5 );
	Layout1t->setMargin( 5 );
	TransTxt = new QLabel( TransGroup );
	Layout1t->addWidget( TransTxt, 0, 0 );
	TransSpin = new QSpinBox( TransGroup );
	TransSpin->setMinimum(0);
	TransSpin->setMaximum(100);
	TransSpin->setSingleStep(10);
	TransSpin->setValue(100);
	Layout1t->addWidget(TransSpin, 0, 1);
	TransTxt2 = new QLabel( TransGroup );
	Layout1t->addWidget( TransTxt2, 1, 0 );
	blendMode = new ScComboBox( TransGroup );
	Layout1t->addWidget( blendMode, 1, 1 );
	page_group_layout->addWidget(TransGroup);
	QSpacerItem* spacerTr2 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	page_group_layout->addItem( spacerTr2 );
	idGroupItem = TabStack->addItem(page_group, "Groups");

	page_3 = new QWidget( TabStack );
	pageLayout_3 = new QVBoxLayout( page_3 );
	pageLayout_3->setSpacing( 5 );
	pageLayout_3->setMargin( 0 );
	pageLayout_3->setAlignment( Qt::AlignLeft );

	TextTree = new ScTreeWidget(page_3);

	layout41 = new QGridLayout();
	layout41->setSpacing( 3 );
	layout41->setMargin( 3 );
	layout41->setAlignment( Qt::AlignLeft );
	Fonts = new FontComboH(page_3);
	layout41->addWidget( Fonts, 0, 0, 1, 4 );
	Size = new ScrSpinBox( 0.5, 2048, page_3, 0 );
	Size->setPrefix( "" );
	fontsizeLabel = new QLabel( "", page_3 );
	fontsizeLabel->setPixmap(loadIcon("Zeichen.xpm"));
	layout41->addWidget( fontsizeLabel, 1, 0 );
	layout41->addWidget( Size, 1, 1 );
// 	lineSpacingPop = new QMenu();
// 	lineSpacingPop->addAction( tr("Fixed Linespacing"))->setCheckable(true);
// 	lineSpacingPop->addAction( tr("Automatic Linespacing"))->setCheckable(true);
// 	lineSpacingPop->addAction( tr("Align to Baseline Grid"))->setCheckable(true);
	lineSpacingLabel = new QLabel( "", page_3 );
// 	lineSpacingLabel->setText("");
	lineSpacingLabel->setPixmap(loadIcon("linespacing2.png"));
	lineSpacingModeCombo = new QComboBox( page_3 );
// 	linespacingButton->setMenu(lineSpacingPop);
// 	linespacingButton->setPopupMode(QToolButton::DelayedPopup);
// 	linespacingButton->setAutoRaise(true);
	LineSp = new ScrSpinBox( page_3, 0 );
	layout41->addWidget( LineSp, 2, 2 );
	layout41->addWidget( lineSpacingLabel, 2, 0 );
	layout41->addWidget( lineSpacingModeCombo, 2, 1 );
	Layout1AL = new QHBoxLayout;
	Layout1AL->setSpacing( 0 );
	Layout1AL->setMargin( 0 );
	Layout1AL->setAlignment( Qt::AlignLeft );
	GroupAlign = new AlignSelect(page_3);
	Layout1AL->addWidget(GroupAlign);
	QSpacerItem* spacer7AL = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1AL->addItem( spacer7AL );
	layout41->addLayout( Layout1AL, 3, 0, 1, 4 );
	pageLayout_3->addLayout( layout41 );

	colorWidgets = new QFrame();
	colorWidgets->setFrameStyle(QFrame::Box | QFrame::Plain);
	colorWidgets->setLineWidth(1);
	layout41c = new QGridLayout(colorWidgets);
	layout41c->setSpacing( 3 );
	layout41c->setMargin( 3 );
	layout41c->setAlignment( Qt::AlignLeft );
	layout23 = new QHBoxLayout;
	layout23->setSpacing( 3 );
	layout23->setMargin( 0 );
	layout23->setAlignment( Qt::AlignLeft );
	StrokeIcon = new QLabel( "", colorWidgets );
	StrokeIcon->setPixmap(loadIcon("16/color-stroke.png"));
	StrokeIcon->setScaledContents( false );
	layout23->addWidget( StrokeIcon );
	TxStroke = new ColorCombo( false, colorWidgets);
	layout23->addWidget( TxStroke );
	ShadeTxt1 = new QLabel( "", colorWidgets );
	ShadeTxt1->setPixmap(loadIcon("shade.png"));
	layout23->addWidget( ShadeTxt1 );
	PM1 = new ShadeButton(colorWidgets);
	layout23->addWidget( PM1 );
	QSpacerItem* spacer7s = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout23->addItem( spacer7s );
	layout41c->addLayout( layout23, 0, 0, 1, 4 );
	layout24 = new QHBoxLayout;
	layout24->setSpacing( 3 );
	layout24->setMargin( 0 );
	layout24->setAlignment( Qt::AlignLeft );
	FillIcon = new QLabel( "", colorWidgets );
	FillIcon->setPixmap(loadIcon("16/color-fill.png"));
	layout24->addWidget( FillIcon );
	TxFill = new ColorCombo( false, colorWidgets);
	layout24->addWidget( TxFill );
	ShadeTxt2 = new QLabel("", colorWidgets );
	ShadeTxt2->setPixmap(loadIcon("shade.png"));
	layout24->addWidget( ShadeTxt2 );
	PM2 = new ShadeButton(colorWidgets);
	layout24->addWidget( PM2 );
	QSpacerItem* spacer7f = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout24->addItem( spacer7f );
	layout41c->addLayout( layout24, 1, 0, 1, 4 );
	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 0 );
	Layout1->setMargin( 0 );
	Layout1->setAlignment( Qt::AlignLeft );
	SeStyle = new StyleSelect(colorWidgets);
	Layout1->addWidget(SeStyle);
	Revert = new QToolButton( colorWidgets );
	Revert->setMaximumSize( QSize( 22, 22 ) );
	Revert->setText("");
	Revert->setIcon(loadIcon("Revers.png"));
	Revert->setCheckable( true );
	Layout1->addWidget( Revert );
	QSpacerItem* spacer7 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer7 );
	layout41c->addLayout( Layout1, 2, 0, 1, 4 );
	colorWidgetsItem = TextTree->addWidget( tr("Color & Effects"), colorWidgets);

	advancedWidgets = new QFrame();
	advancedWidgets->setFrameStyle(QFrame::Box | QFrame::Plain);
	advancedWidgets->setLineWidth(1);
	layout41a = new QGridLayout(advancedWidgets);
	layout41a->setSpacing( 3 );
	layout41a->setMargin( 3 );
	layout41a->setAlignment( Qt::AlignLeft );
	ChBase = new ScrSpinBox( -100, 100, advancedWidgets, 0 );
	ChBase->setValue( 0 );
	ChBaseTxt = new QLabel("", advancedWidgets );
	ChBaseTxt->setPixmap(loadIcon("textbase.png"));
	layout41a->addWidget( ChBaseTxt, 0, 0 );
	layout41a->addWidget( ChBase, 0, 1 );
	Extra = new ScrSpinBox( advancedWidgets, 0 );
	layout41a->addWidget( Extra, 0, 3 );
	trackingLabel = new QLabel( "", advancedWidgets );
	trackingLabel->setText("");
	trackingLabel->setPixmap(loadIcon("textkern.png"));
	layout41a->addWidget( trackingLabel, 0, 2 );
	ChScale = new ScrSpinBox( 10, 400, advancedWidgets, 0 );
	ChScale->setValue( 100 );
	ScaleTxt = new QLabel("", advancedWidgets );
	ScaleTxt->setPixmap(loadIcon("textscaleh.png"));
	layout41a->addWidget( ScaleTxt, 1, 0 );
	layout41a->addWidget( ChScale, 1 , 1 );
	ChScaleV = new ScrSpinBox( 10, 400, advancedWidgets, 0 );
	ChScaleV->setValue( 100 );
	ScaleTxtV = new QLabel("", advancedWidgets );
	ScaleTxtV->setPixmap(loadIcon("textscalev.png"));
	layout41a->addWidget( ScaleTxtV, 1, 2 );
	layout41a->addWidget( ChScaleV, 1, 3 );

	wordTrackingLabel = new QLabel( "Word Spacing", advancedWidgets );
	layout41a->addWidget( wordTrackingLabel, 2, 0, 1, 4 );
	wordTrackingHLayout = new QHBoxLayout;
	wordTrackingHLayout->setSpacing( 3 );
	wordTrackingHLayout->setMargin( 0 );
	wordTrackingHLayout->setAlignment(Qt::AlignLeft);
	minWordTrackingSpinBox = new ScrSpinBox( 1, 200, advancedWidgets, 0 );
	minWordTrackingLabel = new QLabel( "Min:", advancedWidgets );
	minWordTrackingLabel->setBuddy(minWordTrackingSpinBox);
	wordTrackingHLayout->addWidget(minWordTrackingLabel);
	wordTrackingHLayout->addWidget(minWordTrackingSpinBox);
	normWordTrackingSpinBox = new ScrSpinBox( 1, 200, advancedWidgets, 0 );
	normWordTrackingLabel = new QLabel( "Norm:", advancedWidgets );
	normWordTrackingLabel->setBuddy(normWordTrackingSpinBox);
	wordTrackingHLayout->addWidget(normWordTrackingLabel);
	wordTrackingHLayout->addWidget(normWordTrackingSpinBox);
	layout41a->addLayout(wordTrackingHLayout, 3, 0, 1, 4);
	
	glyphExtensionLabel = new QLabel( "Glyph Extension", advancedWidgets );
	layout41a->addWidget( glyphExtensionLabel, 4, 0, 1, 4 );
	glyphExtensionHLayout = new QHBoxLayout;
	glyphExtensionHLayout->setSpacing( 3 );
	glyphExtensionHLayout->setMargin( 0 );
	glyphExtensionHLayout->setAlignment(Qt::AlignLeft);
	minGlyphExtSpinBox = new ScrSpinBox( 90, 110, advancedWidgets, 0 );
	minGlyphExtensionLabel = new QLabel( "Min:", advancedWidgets );
	minGlyphExtensionLabel->setBuddy(minGlyphExtSpinBox);
	glyphExtensionHLayout->addWidget(minGlyphExtensionLabel);
	glyphExtensionHLayout->addWidget(minGlyphExtSpinBox);
	maxGlyphExtSpinBox = new ScrSpinBox( 90, 110, advancedWidgets, 0 );
	maxGlyphExtensionLabel = new QLabel( "Max:", advancedWidgets );
	maxGlyphExtensionLabel->setBuddy(maxGlyphExtSpinBox);
	glyphExtensionHLayout->addWidget(maxGlyphExtensionLabel);
	glyphExtensionHLayout->addWidget(maxGlyphExtSpinBox);
	layout41a->addLayout(glyphExtensionHLayout, 5, 0, 1, 4);

	advancedWidgetsItem = TextTree->addWidget( tr("Advanced Settings"), advancedWidgets);

	styleWidgets = new QFrame();
	styleWidgets->setFrameStyle(QFrame::Box | QFrame::Plain);
	styleWidgets->setLineWidth(1);
	GroupBox3aLayout = new QGridLayout(styleWidgets);
	GroupBox3aLayout->setSpacing( 3 );
	GroupBox3aLayout->setMargin( 3 );
//	GroupBox3aLayout->setAlignment( Qt::AlignLeft );
	paraStyleCombo = new ParaStyleComboBox(styleWidgets);
	paraStyleLabel = new QLabel( "Paragraph St&yle:", styleWidgets );
	paraStyleLabel->setBuddy(paraStyleCombo);
	paraStyleClear = new QToolButton( styleWidgets );
	paraStyleClear->setMaximumSize( QSize( 22, 22 ) );
	paraStyleClear->setText("");
	paraStyleClear->setIcon(loadIcon("16/edit-clear.png"));
	GroupBox3aLayout->addWidget( paraStyleLabel, 0, 0, 1, 2 );
	GroupBox3aLayout->addWidget( paraStyleCombo, 1, 0 );
	GroupBox3aLayout->addWidget( paraStyleClear, 1, 1 );
	charStyleCombo = new CharStyleComboBox(styleWidgets);
	charStyleLabel = new QLabel( "Character St&yle:", styleWidgets );
	charStyleLabel->setBuddy(charStyleCombo);
	charStyleClear = new QToolButton( styleWidgets );
	charStyleClear->setMaximumSize( QSize( 22, 22 ) );
	charStyleClear->setText("");
	charStyleClear->setIcon(loadIcon("16/edit-clear.png"));
	GroupBox3aLayout->addWidget( charStyleLabel, 2, 0, 1, 2 );
	GroupBox3aLayout->addWidget( charStyleCombo, 3, 0 );
	GroupBox3aLayout->addWidget( charStyleClear, 3, 1 );
	
	styleWidgetsItem = TextTree->addWidget( tr("Style Settings"), styleWidgets);

	flopBox = new QFrame();
	flopBox->setFrameStyle(QFrame::Box | QFrame::Plain);
	flopBox->setLineWidth(1);
	flopLayout = new QGridLayout(flopBox);
	flopGroup = new QButtonGroup(flopBox);
	flopRealHeight = new QRadioButton(tr("Maximum Ascent"), flopBox);
	flopFontAscent = new QRadioButton(tr("Font Ascent"), flopBox);
	flopLineSpacing = new QRadioButton(tr("Line Spacing"),flopBox);
	flopGroup->addButton(flopRealHeight, 0);
	flopGroup->addButton(flopFontAscent, 1);
	flopGroup->addButton(flopLineSpacing, 2);
	flopLayout->addWidget(flopRealHeight);
	flopLayout->addWidget(flopFontAscent);
	flopLayout->addWidget(flopLineSpacing);
	flopRealHeight->setChecked(true);
	flopItem = TextTree->addWidget( tr("First Line Offset"), flopBox);

	Distance = new QFrame();
	Distance->setFrameStyle(QFrame::Box | QFrame::Plain);
	Distance->setLineWidth(1);
	DistanceLayout = new QGridLayout( Distance );
	DistanceLayout->setSpacing( 2 );
	DistanceLayout->setMargin( 5 );
	DistanceLayout->setAlignment( Qt::AlignTop );

	DCol = new QSpinBox(Distance );
	DCol->setMaximum( 3000 );
	DCol->setMinimum( 1 );
	columnsLabel = new QLabel( "Colu&mns:", Distance );
	columnsLabel->setBuddy(DCol);
	DistanceLayout->addWidget( columnsLabel, 0, 0 );
	DistanceLayout->addWidget( DCol, 0, 1 );

	dGap = new ScrSpinBox( 0, 300, Distance, 0 );
	colgapLabel = new ScComboBox( Distance );
	DistanceLayout->addWidget( colgapLabel, 1, 0); //, Qt::AlignLeft );
	DistanceLayout->addWidget( dGap, 1, 1 );

	DTop = new ScrSpinBox( 0, 300, Distance, 0 );
	topLabel = new QLabel( "To&p:", Distance );
	topLabel->setBuddy(DTop);
	DistanceLayout->addWidget( topLabel, 2, 0 );
	DistanceLayout->addWidget( DTop, 2, 1 );

	DBottom = new ScrSpinBox( 0, 300, Distance, 0 );
	bottomLabel = new QLabel( "&Bottom:", Distance );
	bottomLabel->setBuddy(DBottom);
	DistanceLayout->addWidget( bottomLabel, 3, 0 );
	DistanceLayout->addWidget( DBottom, 3, 1 );

	DLeft = new ScrSpinBox( 0, 300, Distance, 0 );
	leftLabel = new QLabel( "&Left:", Distance );
	leftLabel->setBuddy(DLeft);
	DistanceLayout->addWidget( leftLabel, 4, 0 );
	DistanceLayout->addWidget( DLeft, 4, 1 );

	DRight = new ScrSpinBox( 0, 300, Distance, 0 );
	rightLabel = new QLabel( "&Right:", Distance );
	rightLabel->setBuddy(DRight);
	DistanceLayout->addWidget( rightLabel, 5, 0 );
	DistanceLayout->addWidget( DRight, 5, 1 );

	optMarginCombo = new QComboBox(Distance);
	optMarginLabel = new QLabel( "Optical Margins:", Distance );
	optMarginLabel->setBuddy(optMarginCombo);
	DistanceLayout->addWidget( optMarginLabel, 6, 0 );
	DistanceLayout->addWidget( optMarginCombo, 6, 1 );

	TabsButton = new QToolButton( Distance );
	TabsButton->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	DistanceLayout->addWidget( TabsButton, 7, 0, 1, 2 );
    DistanceItem = TextTree->addWidget( tr("Columns & Text Distances"), Distance);

	Distance2 = new QFrame();
	Distance2->setFrameStyle(QFrame::Box | QFrame::Plain);
	Distance2->setLineWidth(1);
	DistanceLayout2 = new QGridLayout( Distance2 );
	DistanceLayout2->setSpacing( 2 );
	DistanceLayout2->setMargin( 5 );
	DistanceLayout2->setAlignment( Qt::AlignTop );

	pathTextType = new ScComboBox( Distance2 );
	DistanceLayout2->addWidget( pathTextType, 0, 1);
	pathTextTypeLabel = new QLabel( "Type:", Distance2 );
	DistanceLayout2->addWidget( pathTextTypeLabel, 0, 0);
	
	startoffsetLabel = new QLabel( "Start Offset:", Distance2 );
	DistanceLayout2->addWidget( startoffsetLabel, 1, 0);
	Dist = new ScrSpinBox( 0, 30000, Distance2, 0 );
	Dist->setSingleStep(10);
	DistanceLayout2->addWidget( Dist, 1, 1);

	distfromcurveLabel = new QLabel( "Distance from Curve:", Distance2 );
	DistanceLayout2->addWidget( distfromcurveLabel, 2, 0);
	LineW = new ScrSpinBox( -300, 300, Distance2, 0 );
	LineW->setSingleStep(10);
	DistanceLayout2->addWidget( LineW, 2, 1);

	flippedPathText = new QCheckBox( Distance2 );
	flippedPathText->setText( "Flip Text" );
	DistanceLayout2->addWidget( flippedPathText, 3, 0, 1, 2 );

	showcurveCheckBox = new QCheckBox( Distance2 );
	showcurveCheckBox->setText( "Show Curve" );
	DistanceLayout2->addWidget( showcurveCheckBox, 4, 0, 1, 2 );
	Distance2Item = TextTree->addWidget( tr("Path Text Properties"), Distance2);

	pageLayout_3->addWidget(TextTree);
/*
	layout47 = new QHBoxLayout;
	layout47->setSpacing( 5 );
	layout47->setMargin( 0 );

	layout46 = new QVBoxLayout;
	layout46->setSpacing( 5 );
	layout46->setMargin( 0 );

	layout41 = new QGridLayout;
	layout41->setSpacing( 5 );
	layout41->setMargin( 0 );
	layout41->setAlignment( Qt::AlignLeft );

	Fonts = new FontComboH(page_3);
	layout41->addWidget( Fonts, 0, 0, 1, 4 );

	Size = new ScrSpinBox( 0.5, 2048, page_3, 0 );
	Size->setPrefix( "" );
	fontsizeLabel = new QLabel( "", page_3 );
	fontsizeLabel->setPixmap(loadIcon("Zeichen.xpm"));
	layout41->addWidget( fontsizeLabel, 1, 0 );
	layout41->addWidget( Size, 1, 1 );
	ChBase = new ScrSpinBox( -100, 100, page_3, 0 );
	ChBase->setValue( 0 );
	ChBaseTxt = new QLabel("", page_3 );
	ChBaseTxt->setPixmap(loadIcon("textbase.png"));
	layout41->addWidget( ChBaseTxt, 1, 2 );
	layout41->addWidget( ChBase, 1, 3 );
	LineSp = new ScrSpinBox( page_3, 0 );
	layout41->addWidget( LineSp, 2, 1 );
	lineSpacingPop = new QMenu();
	lineSpacingPop->addAction( tr("Fixed Linespacing"))->setCheckable(true);
	lineSpacingPop->addAction( tr("Automatic Linespacing"))->setCheckable(true);
	lineSpacingPop->addAction( tr("Align to Baseline Grid"))->setCheckable(true);
	linespacingButton = new QToolButton(page_3 );
	linespacingButton->setText("");
	linespacingButton->setIcon(loadIcon("linespacing.png"));
	linespacingButton->setMenu(lineSpacingPop);
	linespacingButton->setPopupMode(QToolButton::DelayedPopup);
	linespacingButton->setAutoRaise(true);
	layout41->addWidget( linespacingButton, 2, 0 );
	Extra = new ScrSpinBox( page_3, 0 );
	layout41->addWidget( Extra, 2, 3 );
	trackingLabel = new QLabel( "", page_3 );
	trackingLabel->setText("");
	trackingLabel->setPixmap(loadIcon("textkern.png"));
	layout41->addWidget( trackingLabel, 2, 2 );
	ChScale = new ScrSpinBox( 10, 400, page_3, 0 );
	ChScale->setValue( 100 );
	ScaleTxt = new QLabel("", page_3 );
	ScaleTxt->setPixmap(loadIcon("textscaleh.png"));
	layout41->addWidget( ScaleTxt, 3, 0 );
	layout41->addWidget( ChScale, 3 , 1 );
	ChScaleV = new ScrSpinBox( 10, 400, page_3, 0 );
	ChScaleV->setValue( 100 );
	ScaleTxtV = new QLabel("", page_3 );
	ScaleTxtV->setPixmap(loadIcon("textscalev.png"));
	layout41->addWidget( ScaleTxtV, 3, 2 );
	layout41->addWidget( ChScaleV, 3, 3 );

	layout46->addLayout( layout41 );

	layout23 = new QHBoxLayout;
	layout23->setSpacing( 5 );
	layout23->setMargin( 0 );
	layout23->setAlignment( Qt::AlignLeft );
	StrokeIcon = new QLabel( "", page_3 );
	StrokeIcon->setPixmap(loadIcon("16/color-stroke.png"));
	StrokeIcon->setScaledContents( false );
	layout23->addWidget( StrokeIcon );
	TxStroke = new ColorCombo( false, page_3);
	layout23->addWidget( TxStroke );
	ShadeTxt1 = new QLabel( "", page_3 );
	ShadeTxt1->setPixmap(loadIcon("shade.png"));
	layout23->addWidget( ShadeTxt1 );
	PM1 = new ShadeButton(page_3);
	layout23->addWidget( PM1 );
	layout46->addLayout( layout23 );
	layout24 = new QHBoxLayout;
	layout24->setSpacing( 5 );
	layout24->setMargin( 0 );
	layout24->setAlignment( Qt::AlignLeft );
	FillIcon = new QLabel( "", page_3 );
	FillIcon->setPixmap(loadIcon("16/color-fill.png"));
	layout24->addWidget( FillIcon );
	TxFill = new ColorCombo( false, page_3);
	layout24->addWidget( TxFill );
	ShadeTxt2 = new QLabel("", page_3 );
	ShadeTxt2->setPixmap(loadIcon("shade.png"));
	layout24->addWidget( ShadeTxt2 );
	PM2 = new ShadeButton(page_3);
	layout24->addWidget( PM2 );
	layout46->addLayout( layout24 );

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 0 );
	Layout1->setMargin( 0 );
	Layout1->setAlignment( Qt::AlignLeft );
	SeStyle = new StyleSelect(page_3);
	Layout1->addWidget(SeStyle);
	Revert = new QToolButton( page_3 );
	Revert->setMaximumSize( QSize( 22, 22 ) );
	Revert->setText("");
	Revert->setIcon(loadIcon("Revers.png"));
	Revert->setCheckable( true );
	Layout1->addWidget( Revert );
	QSpacerItem* spacer7 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer7 );
	layout46->addLayout( Layout1 );
	layout47->addLayout( layout46 );
	pageLayout_3->addLayout( layout47 );

	Layout1AL = new QHBoxLayout;
	Layout1AL->setSpacing( 0 );
	Layout1AL->setMargin( 0 );
	Layout1AL->setAlignment( Qt::AlignLeft );
	GroupAlign = new AlignSelect(page_3);
	Layout1AL->addWidget(GroupAlign);
	QSpacerItem* spacer7AL = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1AL->addItem( spacer7AL );
	pageLayout_3->addLayout( Layout1AL );

	GroupBox3aLayout->setSpacing( 5 );
	GroupBox3aLayout->setMargin( 0 );
	GroupBox3aLayout->setAlignment( Qt::AlignLeft );
	paraStyleCombo = new ParaStyleComboBox(page_3);
	paraStyleLabel = new QLabel( "Paragraph St&yle:", page_3 );
	paraStyleLabel->setBuddy(paraStyleCombo);
	paraStyleClear = new QToolButton( page_3 );
	paraStyleClear->setMaximumSize( QSize( 22, 22 ) );
	paraStyleClear->setText("");
	paraStyleClear->setIcon(loadIcon("16/edit-clear.png"));
	GroupBox3aLayout->addWidget( paraStyleLabel, 0, 0 );
	GroupBox3aLayout->addWidget( paraStyleCombo, 0, 1 );
	GroupBox3aLayout->addWidget( paraStyleClear, 0, 2 );
	charStyleCombo = new CharStyleComboBox(page_3);
	charStyleLabel = new QLabel( "Character St&yle:", page_3 );
	charStyleLabel->setBuddy(charStyleCombo);
	charStyleClear = new QToolButton( page_3 );
	charStyleClear->setMaximumSize( QSize( 22, 22 ) );
	charStyleClear->setText("");
	charStyleClear->setIcon(loadIcon("16/edit-clear.png"));
	GroupBox3aLayout->addWidget( charStyleLabel, 1, 0 );
	GroupBox3aLayout->addWidget( charStyleCombo, 1, 1 );
	GroupBox3aLayout->addWidget( charStyleClear, 1, 2 );
	optMarginCombo = new QComboBox(page_3);
	optMarginLabel = new QLabel( "Optical Margins:", page_3 );
	optMarginLabel->setBuddy(optMarginCombo);
	GroupBox3aLayout->addWidget( optMarginLabel, 2, 0 );
	GroupBox3aLayout->addWidget( optMarginCombo, 2, 1 );
	
	wordTrackingLabel = new QLabel( "Word Spacing", page_3 );
	GroupBox3aLayout->addWidget( wordTrackingLabel, 3, 0 );
	wordTrackingHLayout = new QHBoxLayout;
	wordTrackingHLayout->setSpacing( 5 );
	wordTrackingHLayout->setMargin( 0 );
	wordTrackingHLayout->setAlignment(Qt::AlignLeft);
	minWordTrackingSpinBox = new ScrSpinBox( 1, 200, page_3, 0 );
	minWordTrackingLabel = new QLabel( "Min:", page_3 );
	minWordTrackingLabel->setBuddy(minWordTrackingSpinBox);
	wordTrackingHLayout->addWidget(minWordTrackingLabel);
	wordTrackingHLayout->addWidget(minWordTrackingSpinBox);
	normWordTrackingSpinBox = new ScrSpinBox( 1, 200, page_3, 0 );
	normWordTrackingLabel = new QLabel( "Norm:", page_3 );
	normWordTrackingLabel->setBuddy(normWordTrackingSpinBox);
	wordTrackingHLayout->addWidget(normWordTrackingLabel);
	wordTrackingHLayout->addWidget(normWordTrackingSpinBox);
	GroupBox3aLayout->addLayout(wordTrackingHLayout, 4, 0, 1, 2);
	
	glyphExtensionLabel = new QLabel( "Glyph Extension", page_3 );
	GroupBox3aLayout->addWidget( glyphExtensionLabel, 5, 0 );
	glyphExtensionHLayout = new QHBoxLayout;
	glyphExtensionHLayout->setSpacing( 5 );
	glyphExtensionHLayout->setMargin( 0 );
	glyphExtensionHLayout->setAlignment(Qt::AlignLeft);
	minGlyphExtSpinBox = new ScrSpinBox( 90, 110, page_3, 0 );
	minGlyphExtensionLabel = new QLabel( "Min:", page_3 );
	minGlyphExtensionLabel->setBuddy(minGlyphExtSpinBox);
	glyphExtensionHLayout->addWidget(minGlyphExtensionLabel);
	glyphExtensionHLayout->addWidget(minGlyphExtSpinBox);
	maxGlyphExtSpinBox = new ScrSpinBox( 90, 110, page_3, 0 );
	maxGlyphExtensionLabel = new QLabel( "Max:", page_3 );
	maxGlyphExtensionLabel->setBuddy(maxGlyphExtSpinBox);
	glyphExtensionHLayout->addWidget(maxGlyphExtensionLabel);
	glyphExtensionHLayout->addWidget(maxGlyphExtSpinBox);
	GroupBox3aLayout->addLayout(glyphExtensionHLayout, 6, 0, 1, 2);
	*/
/*	langCombo = new ScComboBox( page_3 );
	langLabel = new QLabel( langCombo, "Lan&guage:", page_3, "langLabel" );
	GroupBox3aLayout->addWidget( langLabel, 1, 0 );
	GroupBox3aLayout->addWidget( langCombo, 1, 1 ); */
//	pageLayout_3->addLayout(GroupBox3aLayout);

//	QSpacerItem* spacer8 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
//	pageLayout_3->addItem( spacer8 );
	idTextItem=TabStack->addItem( page_3, "&Text" );

	page_4 = new QWidget( TabStack );
	pageLayout_4 = new QVBoxLayout( page_4 );
	pageLayout_4->setSpacing( 5 );
	pageLayout_4->setMargin( 0 );

	imagePageNumberSelector = new QGridLayout();
	imagePageNumberSelector->setSpacing( 5 );
	imagePageNumberSelector->setMargin( 0 );
	imagePageNumber = new QSpinBox( page_4 );
	imagePageNumber->setMinimum(0);
	imagePageNumber->setSpecialValueText(tr( "Auto" ));
	imagePageNumberLabel = new QLabel( "&Page Number:", page_4 );
	imagePageNumberLabel->setBuddy(imagePageNumber);
	imagePageNumberSelector->addWidget( imagePageNumberLabel, 0, 0 );
	imagePageNumberSelector->addWidget( imagePageNumber, 0, 1);
	pageLayout_4->addLayout( imagePageNumberSelector );
	
	FreeScale = new QRadioButton( "&Free Scaling", page_4 );
	FreeScale->setChecked( true );
	pageLayout_4->addWidget( FreeScale );

	layout43 = new QGridLayout;
	layout43->setSpacing( 5 );
	layout43->setMargin( 0 );
	imageXOffsetSpinBox = new ScrSpinBox( page_4, 0 );
	installSniffer(imageXOffsetSpinBox);
	xposImgLabel = new QLabel( "&X-Pos:", page_4 );
	xposImgLabel->setBuddy(imageXOffsetSpinBox);
	layout43->addWidget( xposImgLabel, 0, 0 );
	layout43->addWidget( imageXOffsetSpinBox, 0, 1 );
	imageYOffsetSpinBox = new ScrSpinBox( page_4, 0 );
	installSniffer(imageYOffsetSpinBox);
	yposImgLabel = new QLabel( "&Y-Pos:", page_4 );
	yposImgLabel->setBuddy(imageYOffsetSpinBox);
	layout43->addWidget( yposImgLabel, 1, 0 );
	layout43->addWidget( imageYOffsetSpinBox, 1, 1 );
	imageXScaleSpinBox = new ScrSpinBox( page_4, 0 );
	installSniffer(imageXScaleSpinBox);
	xscaleLabel = new QLabel( "X-Sc&ale:", page_4 );
	xscaleLabel->setBuddy(imageXScaleSpinBox);
	layout43->addWidget( xscaleLabel, 2, 0 );
	layout43->addWidget( imageXScaleSpinBox, 2, 1 );
	imageYScaleSpinBox = new ScrSpinBox( page_4, 0 );
	installSniffer(imageYScaleSpinBox);
	yscaleLabel = new QLabel( "Y-Scal&e:", page_4 );
	yscaleLabel->setBuddy(imageYScaleSpinBox);
	layout43->addWidget( yscaleLabel, 3, 0 );
	layout43->addWidget( imageYScaleSpinBox, 3, 1 );
	keepImageWHRatioButton = new LinkButton( page_4 );
	keepImageWHRatioButton->setCheckable( true );
	keepImageWHRatioButton->setAutoRaise( true );
	keepImageWHRatioButton->setMaximumSize( QSize( 15, 32767 ) );
	layout43->addWidget( keepImageWHRatioButton, 2, 2, 2, 1 );
	imgDpiX = new ScrSpinBox( page_4, 0 );
	installSniffer(imgDpiX);
	imgDPIXLabel = new QLabel( "Actual X-DPI:", page_4 );
	imgDPIXLabel->setBuddy(imgDpiX);
	layout43->addWidget( imgDPIXLabel, 4, 0 );
	layout43->addWidget( imgDpiX, 4, 1 );
	imgDpiY = new ScrSpinBox( page_4, 0 );
	installSniffer(imgDpiY);
	imgDPIYLabel = new QLabel( "Actual Y-DPI:", page_4 );
	imgDPIYLabel->setBuddy(imgDpiY);
	layout43->addWidget( imgDPIYLabel, 5, 0 );
	layout43->addWidget( imgDpiY, 5, 1 );
	keepImageDPIRatioButton = new LinkButton( page_4 );
	keepImageDPIRatioButton->setCheckable( true );
	keepImageDPIRatioButton->setAutoRaise( true );
	keepImageDPIRatioButton->setMaximumSize( QSize( 15, 32767 ) );
	layout43->addWidget( keepImageDPIRatioButton, 4, 2, 2, 1 );
	pageLayout_4->addLayout( layout43 );

	Layout24 = new QVBoxLayout;
	Layout24->setSpacing( 3 );
	Layout24->setMargin( 0 );

	FrameScale = new QRadioButton( page_4 );
	FrameScale->setText( "Scale &To Frame Size" );
	Layout24->addWidget( FrameScale );

	Layout18 = new QHBoxLayout;
	Layout18->setSpacing( 5 );
	Layout18->setMargin( 0 );

	Frame4 = new QFrame( page_4 );
	Frame4->setMinimumSize( QSize( 15, 2 ) );
	Frame4->setMaximumSize( QSize( 15, 10 ) );
	Frame4->setFrameShape( QFrame::NoFrame );
	Frame4->setFrameShadow( QFrame::Plain );
	Layout18->addWidget( Frame4 );

	Aspect = new QCheckBox( page_4 );
	Aspect->setEnabled( false );
	Aspect->setText( "P&roportional" );
	Aspect->setChecked( true );

	Layout18->addWidget( Aspect );
	Layout24->addLayout( Layout18 );
	pageLayout_4->addLayout( Layout24 );

	EditEffects = new QToolButton( page_4);
	pageLayout_4->addWidget( EditEffects );

	EditPSDProps = new QToolButton( page_4);
	pageLayout_4->addWidget( EditPSDProps );

	GroupBoxCM = new QFrame( page_4 );
	GroupBoxCM->setFrameShape( QFrame::NoFrame );
	GroupBoxCM->setFrameShadow( QFrame::Plain );
	GroupBoxCMLayout = new QVBoxLayout( GroupBoxCM );
	GroupBoxCMLayout->setSpacing( 2 );
	GroupBoxCMLayout->setMargin( 5 );
	GroupBoxCMLayout->setAlignment( Qt::AlignTop );
	TextCms1 = new QLabel( GroupBoxCM );
	GroupBoxCMLayout->addWidget( TextCms1 );
	InputP = new ScComboBox( GroupBoxCM );
	GroupBoxCMLayout->addWidget(InputP);
	TextCms2 = new QLabel( GroupBoxCM );
	GroupBoxCMLayout->addWidget(TextCms2);
	MonitorI = new ScComboBox( GroupBoxCM );
	GroupBoxCMLayout->addWidget(MonitorI);
	pageLayout_4->addWidget(GroupBoxCM);

	QSpacerItem* spacer9 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout_4->addItem( spacer9 );
	idImageItem=TabStack->addItem( page_4, "&Image" );

	page_5 = new QWidget( TabStack );
	pageLayout_5 = new QVBoxLayout( page_5 );
	pageLayout_5->setSpacing( 5 );
	pageLayout_5->setMargin( 0 );

	LMode = false;

	Layout12_2 = new QGridLayout;
	Layout12_2->setSpacing( 3 );
	Layout12_2->setMargin( 0 );
	LStyle = new LineCombo(page_5);
	LStyle->addItem( tr("Custom"));
	LineMode = new ScComboBox( page_5 );
	LineModeT = new QLabel( "&Basepoint:", page_5 );
	LineModeT->setBuddy(LineMode);
	Layout12_2->addWidget( LineModeT, 0, 0 );
	Layout12_2->addWidget( LineMode, 0, 1 );
	linetypeLabel = new QLabel( "T&ype of Line:", page_5 );
	linetypeLabel->setBuddy(LStyle);
	Layout12_2->addWidget( linetypeLabel, 1, 0 );
	Layout12_2->addWidget( LStyle, 1, 1 );

	dashEditor = new DashEditor(page_5);
	Layout12_2->addWidget(dashEditor, 2, 0, 1, 2);

	startArrow = new ArrowChooser(page_5, true);
	Layout12_2->addWidget( startArrow, 4, 0 );
	endArrow = new ArrowChooser(page_5, false);
	Layout12_2->addWidget( endArrow, 4, 1 );
	startArrowText = new QLabel( "Start Arrow:", page_5 );
	startArrowText->setBuddy(startArrow);
	Layout12_2->addWidget( startArrowText, 3, 0 );
	endArrowText = new QLabel( "End Arrow:", page_5 );
	endArrowText->setBuddy(endArrow);
	Layout12_2->addWidget( endArrowText, 3, 1 );
	LSize = new ScrSpinBox( page_5, 0 );
	linewidthLabel = new QLabel( "Line &Width:", page_5 );
	linewidthLabel->setBuddy(LSize);
	Layout12_2->addWidget( linewidthLabel, 5, 0 );
	Layout12_2->addWidget( LSize, 5, 1 );
	LJoinStyle = new ScComboBox( page_5 );
	edgesLabel = new QLabel( "Ed&ges:", page_5 );
	edgesLabel->setBuddy(LJoinStyle);
	Layout12_2->addWidget( edgesLabel, 6, 0 );
	Layout12_2->addWidget( LJoinStyle, 6, 1 );
	LEndStyle = new ScComboBox( page_5 );
	endingsLabel = new QLabel( "&Endings:", page_5 );
	endingsLabel->setBuddy(LEndStyle);
	Layout12_2->addWidget( endingsLabel, 7, 0 );
	Layout12_2->addWidget( LEndStyle, 7, 1 );
	pageLayout_5->addLayout( Layout12_2 );

	TabStack3 = new QStackedWidget( page_5 );
	TabStack3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	page_5a = new QWidget( TabStack3 );
	pageLayout_5a = new QVBoxLayout( page_5a );
	pageLayout_5a->setSpacing( 5 );
	pageLayout_5a->setMargin( 0 );
	StyledLine = new QListWidget(page_5a);
	StyledLine->setItemDelegate(new LineFormatItemDelegate);
	StyledLine->addItem( "No Style" );
	pageLayout_5a->addWidget(StyledLine);
	TabStack3->addWidget( page_5a );

	page_5b = new QWidget( TabStack3 );
	pageLayout_5b = new QVBoxLayout( page_5b );
	pageLayout_5b->setSpacing( 5 );
	pageLayout_5b->setMargin( 0 );
	TLines = new QGroupBox( "Cell Lines", page_5b );
	TLineLayout = new QVBoxLayout( TLines );
	TLineLayout->setSpacing( 2 );
	TLineLayout->setMargin( 5 );
	TLineLayout->setAlignment( Qt::AlignTop );
	TopLine = new QCheckBox( TLines );
	TopLine->setText( "Line at Top" );
	TLineLayout->addWidget(TopLine);
	LeftLine = new QCheckBox( TLines );
	LeftLine->setText( "Line at the Left" );
	TLineLayout->addWidget(LeftLine);
	RightLine = new QCheckBox( TLines );
	RightLine->setText( "Line at the Right " );
	TLineLayout->addWidget(RightLine);
	BottomLine = new QCheckBox( TLines );
	BottomLine->setText( "Line at Bottom" );
	TLineLayout->addWidget(BottomLine);
	pageLayout_5b->addWidget(TLines);
	TabStack3->addWidget( page_5b );
	pageLayout_5->addWidget( TabStack3 );
	idLineItem=TabStack->addItem( page_5, "&Line" );
	
	page_6 = new QWidget( TabStack );
	pageLayout_6 = new QVBoxLayout( page_6 );
	pageLayout_6->setSpacing( 5 );
	pageLayout_6->setMargin( 0 );

	Cpal = new Cpalette(page_6);
	pageLayout_6->addWidget( Cpal );

	OverP = new QGroupBox( "Overprinting", page_6 );
	OverPLayout = new QVBoxLayout( OverP );
	OverPLayout->setSpacing( 2 );
	OverPLayout->setMargin( 5 );
	OverPLayout->setAlignment( Qt::AlignTop );
	KnockOut = new QRadioButton( "Knockout", OverP );
	OverPLayout->addWidget( KnockOut );
	Overprint = new QRadioButton( "Overprint", OverP );
	OverPLayout->addWidget( Overprint );
	KnockOut->setChecked( true );
	pageLayout_6->addWidget(OverP);

	idColorsItem = TabStack->addItem(page_6, "&Colors" );
	MpalLayout->addWidget( TabStack );

	languageChange();

	connect(Xpos, SIGNAL(valueChanged(double)), this, SLOT(NewX()));
	connect(Ypos, SIGNAL(valueChanged(double)), this, SLOT(NewY()));
	connect(Width, SIGNAL(valueChanged(double)), this, SLOT(NewW()));
	connect(Height, SIGNAL(valueChanged(double)), this, SLOT(NewH()));
	connect(Rotation, SIGNAL(valueChanged(double)), this, SLOT(setRotation()));
	connect(RoundRect, SIGNAL(valueChanged(double)), this, SLOT(NewCornerRadius()));
	connect(LineSp, SIGNAL(valueChanged(double)), this, SLOT(NewLineSpacing()));
	connect(Size, SIGNAL(valueChanged(double)), this, SLOT(NewSize()));
	connect(Extra, SIGNAL(valueChanged(double)), this, SLOT(NewTracking()));
	connect(imagePageNumber, SIGNAL(valueChanged(int)), this, SLOT(NewPage()));
	connect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	connect(imageXOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));
	connect(imageYOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));
	connect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
	connect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
	connect(LSize, SIGNAL(valueChanged(double)), this, SLOT(NewLineWidth()));
	connect(LStyle, SIGNAL(activated(int)), this, SLOT(NewLineStyle()));
	connect(LJoinStyle, SIGNAL(activated(int)), this, SLOT(NewLineJoin()));
	connect(LEndStyle, SIGNAL(activated(int)), this, SLOT(NewLineEnd()));
	connect(LineMode, SIGNAL(activated(int)), this, SLOT(NewLineMode()));
	connect(dashEditor, SIGNAL(dashChanged()), this, SLOT(dashChange()));
	connect(keepImageWHRatioButton, SIGNAL(clicked()), this, SLOT(ToggleKette()));
	connect(keepImageDPIRatioButton, SIGNAL(clicked()), this, SLOT(ToggleKetteD()));
	connect(FlipH, SIGNAL(clicked()), this, SLOT(handleFlipH()));
	connect(FlipV, SIGNAL(clicked()), this, SLOT(handleFlipV()));
	connect(GroupAlign, SIGNAL(State(int)), this, SLOT(NewAlignement(int)));
	connect(Revert, SIGNAL(clicked()), this, SLOT(DoRevert()));
	connect(charStyleClear, SIGNAL(clicked()), this, SLOT(doClearCStyle()));
	connect(paraStyleClear, SIGNAL(clicked()), this, SLOT(doClearPStyle()));
	connect(SeStyle, SIGNAL(State(int)), this, SLOT(setTypeStyle(int)));
	connect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowOffs()));
	connect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowOffs()));
	connect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newOutlineW()));
	connect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newUnderline()));
	connect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newUnderline()));
	connect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newStrike()));
	connect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newStrike()));
	connect(FreeScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
	connect(FrameScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
	connect(Aspect, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
	connect(EditEffects, SIGNAL(clicked()), this, SLOT(handleImageEffects()));
	connect(EditPSDProps, SIGNAL(clicked()), this, SLOT(handleExtImgProperties()));
	connect(Zup, SIGNAL(clicked()), this, SLOT(DoRaise()));
	connect(ZDown, SIGNAL(clicked()), this, SLOT(DoLower()));
	connect(ZTop, SIGNAL(clicked()), this, SLOT(DoFront()));
	connect(ZBottom, SIGNAL(clicked()), this, SLOT(DoBack()));
	connect(RotationGroup, SIGNAL(buttonClicked(int)), this, SLOT(NewRotMode(int)));
	connect(textFlowOptionsB , SIGNAL(buttonClicked(int)), this, SLOT(DoFlow()));
	connect(textFlowOptionsB2, SIGNAL(buttonClicked(int)), this, SLOT(DoFlow()));

	connect(SCustom, SIGNAL(FormSel(int, int, double *)), this, SLOT(MakeIrre(int, int, double *)));
	connect(EditShape, SIGNAL(clicked()), this, SLOT(handleShapeEdit()));
	connect(SCustom2, SIGNAL(FormSel(int, int, double *)), this, SLOT(MakeIrre(int, int, double *)));
	connect(EditShape2, SIGNAL(clicked()), this, SLOT(handleShapeEdit2()));
	connect(dGap, SIGNAL(valueChanged(double)), this, SLOT(NewGap()));
	connect(DCol, SIGNAL(valueChanged(int)), this, SLOT(NewCols()));
	connect(DTop, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	connect(DLeft, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	connect(DRight, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	connect(DBottom, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	connect(flopGroup, SIGNAL(buttonClicked( int )), this, SLOT(flop(int)));
	connect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));
	connect(StyledLine, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(SetSTline(QListWidgetItem*)));
	connect(Fonts, SIGNAL(fontSelected(QString )), this, SLOT(NewTFont(QString)));
	connect(TxFill, SIGNAL(activated(int)), this, SLOT(newTxtFill()));
	connect(TxStroke, SIGNAL(activated(int)), this, SLOT(newTxtStroke()));
	connect(PM1, SIGNAL(clicked()), this, SLOT(setActShade()));
	connect(PM2, SIGNAL(clicked()), this, SLOT(setActShade()));
	connect(ChScale, SIGNAL(valueChanged(double)), this, SLOT(NewTScale()));
	connect(ChScaleV, SIGNAL(valueChanged(double)), this, SLOT(NewTScaleV()));
	connect(ChBase, SIGNAL(valueChanged(double)), this, SLOT(NewTBase()));
	connect(Locked, SIGNAL(clicked()), this, SLOT(handleLock()));
	connect(NoPrint, SIGNAL(clicked()), this, SLOT(handlePrint()));
	connect(NoResize, SIGNAL(clicked()), this, SLOT(handleLockSize()));
	connect(showcurveCheckBox, SIGNAL(clicked()), this, SLOT(handlePathLine()));
	connect(pathTextType, SIGNAL(activated(int)), this, SLOT(handlePathType()));
	connect(flippedPathText, SIGNAL(clicked()), this, SLOT(handlePathFlip()));
	connect(Dist, SIGNAL(valueChanged(double)), this, SLOT(handlePathDist()));
	connect(LineW, SIGNAL(valueChanged(double)), this, SLOT(handlePathOffs()));
	connect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChangeProfile(const QString&)));
	connect(MonitorI, SIGNAL(activated(int)), this, SLOT(ChangeIntent()));
	connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
//	connect(langCombo, SIGNAL(activated(int)), this, SLOT(NewLanguage()));
	connect( TabsButton, SIGNAL( clicked() ), this, SLOT( ManageTabs() ) );
	connect( TopLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( LeftLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( RightLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
	connect( BottomLine, SIGNAL( clicked() ), this, SLOT( HandleTLines() ) );
//	connect( colgapLabel, SIGNAL( clicked() ), this, SLOT( HandleGapSwitch() ) );
	connect(colgapLabel, SIGNAL(activated(int)), this, SLOT(HandleGapSwitch()));
	connect( Cpal, SIGNAL(NewSpecial(double, double, double, double )), this, SLOT(NewSpGradient(double, double, double, double )));
	connect( Cpal, SIGNAL(editGradient()), this, SLOT(toggleGradientEdit()));
	connect(startArrow, SIGNAL(activated(int)), this, SLOT(setStartArrow(int )));
	connect(endArrow, SIGNAL(activated(int)), this, SLOT(setEndArrow(int )));
// 	connect(lineSpacingPop, SIGNAL(triggered(QAction *)), this, SLOT(setLspMode(QAction *)));
	connect(lineSpacingModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setLineSpacingMode(int)));
	connect( EvenOdd, SIGNAL( clicked() ), this, SLOT(handleFillRule() ) );
	connect( NonZero, SIGNAL( clicked() ), this, SLOT( handleFillRule() ) );
	connect( KnockOut, SIGNAL( clicked() ), this, SLOT( handleOverprint() ) );
	connect( Overprint, SIGNAL( clicked() ), this, SLOT( handleOverprint() ) );
	connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(setGroupTransparency(int)));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(setGroupBlending(int)));
	connect(DoGroup, SIGNAL(clicked()), this, SLOT(doGrouping()) );
	connect(optMarginCombo, SIGNAL(activated(int)), this, SLOT(setOpticalMargins(int)) );
	connect(minWordTrackingSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMinWordTracking()) );
	connect(normWordTrackingSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNormWordTracking()) );
	connect(minGlyphExtSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMinGlyphExtension()) );
	connect(maxGlyphExtSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMaxGlyphExtension()) );

	HaveItem = false;
	Xpos->setValue(0);
	Ypos->setValue(0);
	Width->setValue(0);
	Height->setValue(0);
	Rotation->setValue(0);
	RoundRect->setValue(0);
	TabStack3->setCurrentIndex(0);
	TabStack2->setCurrentIndex(0);
	for (int ws = 1; ws < 7; ++ws)
		TabStack->setItemEnabled(ws, false);
	TabStack->setCurrentIndex(0);
	TabStack->widget(0)->setEnabled(false);
	TabStack->setItemEnabled(0, false);
	StrokeIcon->setEnabled(false);
	TxStroke->setEnabled(false);
	PM1->setEnabled(false);
	colgapLabel->setCurrentIndex(0);
}

void PropertiesPalette::closeEvent(QCloseEvent *closeEvent)
{
	if (m_ScMW && !m_ScMW->ScriptRunning)
	{
		if ((HaveDoc) && (HaveItem))
		{
			if (Cpal->gradEditButton->isChecked())
			{
				m_ScMW->view->requestMode(modeNormal);
				m_ScMW->view->RefreshGradient(CurItem);
			}
		}
	}
	ScrPaletteBase::closeEvent(closeEvent);
}

void PropertiesPalette::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW=mw;
	QPoint p1 = mapToGlobal(pos());
	QPoint p2 = m_ScMW->mapFromGlobal(p1);
	//Qt4 reparent(m_ScMW, this->getWFlags(), p2);
	setParent(m_ScMW);
	move(p2);

	connect(this, SIGNAL(DocChanged()), m_ScMW, SLOT(slotDocCh()));
//	connect(this, SIGNAL(NewParStyle(int)), m_ScMW, SLOT(setNewParStyle(int)));
	connect(this, SIGNAL(NewAlignment(int)), m_ScMW, SLOT(setNewAlignment(int)));
	connect(this, SIGNAL(NewEffects(int)), m_ScMW, SLOT(setItemHoch(int)));
//	connect(this, SIGNAL(ShapeEdit()), m_ScMW, SLOT(ToggleFrameEdit()));
	connect(this, SIGNAL(NewFont(const QString&)), m_ScMW, SLOT(SetNewFont(const QString&)));
	connect(this, SIGNAL(UpdtGui(int)), m_ScMW, SLOT(HaveNewSel(int)));
//CB unused in 135 	connect(this->Cpal, SIGNAL(modeChanged()), m_ScMW, SLOT(setCSMenu()));
	connect(this->Cpal->gradEdit->Preview, SIGNAL(gradientChanged()), m_ScMW, SLOT(updtGradFill()));
	connect(this->Cpal, SIGNAL(gradientChanged()), m_ScMW, SLOT(updtGradFill()));
	connect(DoUnGroup, SIGNAL(clicked()), m_ScMW, SLOT(UnGroupObj()) );
	
}

void PropertiesPalette::SelTab(int t)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem) && (t == idColorsItem))
	{
		Cpal->setActGradient(CurItem->GrType);
		updateColorSpecialGradient();
		Cpal->gradEdit->Preview->fill_gradient = CurItem->fill_gradient;
		Cpal->gradEdit->Preview->updateDisplay();
	}
	if ((HaveDoc) && (HaveItem))
	{
		if (t == idShapeItem)
			DCol->setMaximum(qMax(qRound(CurItem->width() / qMax(CurItem->ColGap, 10.0)), 1));
		if (t == idImageItem)
			imagePageNumber->setMaximum(CurItem->pixm.imgInfo.numberOfPages);
	}
}

void PropertiesPalette::setDoc(ScribusDoc *d)
{
	if(doc == d || (m_ScMW && m_ScMW->ScriptRunning))
		return;

	disconnect(this->Cpal, SIGNAL(NewTrans(double)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewTransS(double)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewPen(QString)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewBrush(QString)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewPenShade(int)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewBrushShade(int)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewGradient(int)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewBlend(int)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewBlendS(int)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewPattern(QString)), 0, 0);
	disconnect(this->Cpal, SIGNAL(NewPatternProps(double, double, double, double, double)), 0, 0);

	doc = d;
	CurItem = NULL;
	Cpal->setDocument(doc);
	Cpal->setCurrentItem(NULL);
	m_unitRatio=doc->unitRatio();
	m_unitIndex=doc->unitIndex();
	int precision = unitGetPrecisionFromIndex(m_unitIndex);
//qt4 FIXME here
	double maxXYWHVal= 16777215 * m_unitRatio;
	double minXYVal= -16777215 * m_unitRatio;
	HaveDoc = true;
	HaveItem = false;

	QMap<QString, double>* docConstants = doc? &doc->constants()  : NULL;
	Xpos->setValues( minXYVal, maxXYWHVal, precision, minXYVal);
	Xpos->setConstants(docConstants);
	Ypos->setValues( minXYVal, maxXYWHVal, precision, minXYVal);
	Ypos->setConstants(docConstants);
	Width->setValues( m_unitRatio, maxXYWHVal, precision, m_unitRatio);
	Width->setConstants(docConstants);
	Height->setValues( m_unitRatio, maxXYWHVal, precision, m_unitRatio);
	Height->setConstants(docConstants);
	imageXOffsetSpinBox->setValues( -16777215, maxXYWHVal, precision, 0);
	imageYOffsetSpinBox->setValues( -16777215, maxXYWHVal, precision, 0);

	Rotation->setValues( 0, 359.99, 1, 0);
	RoundRect->setValues( -300, 300, 2, 0);
	Extra->setValues( -300, 300, 2, 0);
	Size->setValues( 0.5, 2048, 2, 1);
	LineSp->setValues( 1, 2048, 2, 1);
	minWordTrackingSpinBox->setValues(1, 100, 2, 100);
	normWordTrackingSpinBox->setValues(1, 200, 2, 100);
	minGlyphExtSpinBox->setValues(90, 110, 2, 100);
	maxGlyphExtSpinBox->setValues(90, 110, 2, 100);
	imageXScaleSpinBox->setValues( 1, 30000, 2, 1);
	imageYScaleSpinBox->setValues( 1, 30000, 2, 1);
	imgDpiX->setValues( 1, 30000, 2, 1);
	imgDpiY->setValues( 1, 30000, 2, 1);

	dGap->setDecimals(2);
	DTop->setDecimals(2);
	DLeft->setDecimals(2);
	DBottom->setDecimals(2);
	DRight->setDecimals(2);
	LSize->setMaximum( 300 );
	LSize->setMinimum( 0 );
	Dist->setMaximum( 30000 );
	Dist->setMinimum( 0 );
	Dist->setSingleStep(10);
	LineW->setMaximum( 300 );
	LineW->setMinimum( -300 );
	LineW->setSingleStep(10);

	updateColorList();

	updateSpinBoxConstants();
	paraStyleCombo->setDoc(doc);
	charStyleCombo->setDoc(doc);
	SetLineFormats(doc);
	startArrow->rebuildList(&doc->arrowStyles);
	endArrow->rebuildList(&doc->arrowStyles);

	connect(this->Cpal, SIGNAL(NewTrans(double)), doc, SLOT(itemSelection_SetItemFillTransparency(double)));
	connect(this->Cpal, SIGNAL(NewTransS(double)), doc, SLOT(itemSelection_SetItemLineTransparency(double)));
	connect(this->Cpal, SIGNAL(NewBlend(int)), doc, SLOT(itemSelection_SetItemFillBlend(int)));
	connect(this->Cpal, SIGNAL(NewBlendS(int)), doc, SLOT(itemSelection_SetItemLineBlend(int)));
	connect(this->Cpal, SIGNAL(NewPen(QString)), doc, SLOT(itemSelection_SetItemPen(QString)));
	connect(this->Cpal, SIGNAL(NewBrush(QString)), doc, SLOT(itemSelection_SetItemBrush(QString)));
	connect(this->Cpal, SIGNAL(NewPenShade(int)), doc, SLOT(itemSelection_SetItemPenShade(int)));
	connect(this->Cpal, SIGNAL(NewBrushShade(int)), doc, SLOT(itemSelection_SetItemBrushShade(int)));
	connect(this->Cpal, SIGNAL(NewGradient(int)), doc, SLOT(itemSelection_SetItemGradFill(int)));
	connect(this->Cpal, SIGNAL(NewPattern(QString)), doc, SLOT(itemSelection_SetItemPatternFill(QString)));
	connect(this->Cpal, SIGNAL(NewPatternProps(double, double, double, double, double)), doc, SLOT(itemSelection_SetItemPatternProps(double, double, double, double, double)));
}

void PropertiesPalette::unsetDoc()
{
	HaveDoc = false;
	HaveItem = false;
	doc=NULL;
	CurItem = NULL;
	Cpal->setCurrentItem(NULL);
	Cpal->setDocument(NULL);
	Xpos->setConstants(NULL);
	Ypos->setConstants(NULL);
	Width->setConstants(NULL);
	Height->setConstants(NULL);
	paraStyleCombo->setDoc(0);
	charStyleCombo->setDoc(0);
	SetLineFormats(0);
	DoGroup->setEnabled(false);
	DoUnGroup->setEnabled(false);
	EditShape->setEnabled(false);
//	ShapeGroup->setEnabled(false);
	FlipH->setEnabled(false);
	FlipV->setEnabled(false);
	xposLabel->setText( tr( "&X-Pos:" ) );
	widthLabel->setText( tr( "&Width:" ) );
	yposLabel->setText( tr( "&Y-Pos:" ) );
	heightLabel->setText( tr( "&Height:" ) );
	RoundRect->setEnabled(false);
	HaveItem = false;
	Xpos->setValue(0);
	Ypos->setValue(0);
	Width->setValue(0);
	Height->setValue(0);
	Rotation->setValue(0);
	RoundRect->setValue(0);
	for (int ws = 1; ws < 7; ++ws)
		TabStack->setItemEnabled(ws, false);
	TabStack->widget(0)->setEnabled(false);
	TabStack->setItemEnabled(idXYZItem, false);
	Cpal->ChooseGrad(0);
}

void PropertiesPalette::unsetItem()
{
	HaveItem=false;
	CurItem = NULL;
	Cpal->setCurrentItem(NULL);
	dashEditor->hide();
	NewSel(-1);
}

void PropertiesPalette::setTextFlowMode(PageItem::TextFlowMode mode)
{
	if (!m_ScMW || m_ScMW->ScriptRunning || !HaveItem)
		return;
	if (CurItem->isGroupControl)
	{
		if (mode == PageItem::TextFlowDisabled)
			textFlowDisabled2->setChecked(true);
		else if (mode == PageItem::TextFlowUsesFrameShape)
			textFlowUsesFrameShape2->setChecked(true);
		else if (mode == PageItem::TextFlowUsesBoundingBox)
			textFlowUsesBoundingBox2->setChecked(true);
		else if (mode == PageItem::TextFlowUsesContourLine)
			textFlowUsesContourLine2->setChecked(true);
		else if (mode == PageItem::TextFlowUsesImageClipping)
			textFlowUsesImageClipping2->setChecked(true);
		if ((CurItem->asImageFrame()) && (CurItem->imageClip.size() != 0))
			textFlowUsesImageClipping2->setEnabled(true);
		else
			textFlowUsesImageClipping2->setEnabled(false);
	}
	else
	{
		if (mode == PageItem::TextFlowDisabled)
			textFlowDisabled->setChecked(true);
		else if (mode == PageItem::TextFlowUsesFrameShape)
			textFlowUsesFrameShape->setChecked(true);
		else if (mode == PageItem::TextFlowUsesBoundingBox)
			textFlowUsesBoundingBox->setChecked(true);
		else if (mode == PageItem::TextFlowUsesContourLine)
			textFlowUsesContourLine->setChecked(true);
		else if (mode == PageItem::TextFlowUsesImageClipping)
			textFlowUsesImageClipping->setChecked(true);
		if ((CurItem->asImageFrame()) && (CurItem->imageClip.size() != 0))
			textFlowUsesImageClipping->setEnabled(true);
		else
			textFlowUsesImageClipping->setEnabled(false);
	}
}

void PropertiesPalette::SetCurItem(PageItem *i)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	//CB We shouldnt really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This wont work until when a canvas deselect happens, CurItem must be NULL.
	//if (CurItem == i)
	//	return;

	if (!doc)
		setDoc(i->doc());

	disconnect(StyledLine, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(SetSTline(QListWidgetItem*)));
	disconnect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	disconnect(startArrow, SIGNAL(activated(int)), this, SLOT(setStartArrow(int )));
	disconnect(endArrow, SIGNAL(activated(int)), this, SLOT(setEndArrow(int )));
	disconnect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));

	HaveItem = false;
	CurItem = i;

	Cpal->setCurrentItem(CurItem);
	Cpal->updateFromItem();
/*	if (TabStack->currentIndex() == idColorsItem)
		Cpal->setActGradient(CurItem->GrType);
	updateColorSpecialGradient();
	Cpal->gradEdit->Preview->fill_gradient = CurItem->fill_gradient;
	Cpal->gradEdit->Preview->updateDisplay(); */
	if (CurItem->FrameType == 0)
		SCustom->setIcon(SCustom->getIconPixmap(0));
	if (CurItem->FrameType == 1)
		SCustom->setIcon(SCustom->getIconPixmap(1));
	if (CurItem->FrameType > 3)
		SCustom->setIcon(SCustom->getIconPixmap(CurItem->FrameType-2));
	if ((CurItem->asLine()) || (CurItem->asPolyLine()))
	{
		startArrow->setEnabled(true);
		endArrow->setEnabled(true);
		startArrow->setCurrentIndex(CurItem->startArrowIndex());
		endArrow->setCurrentIndex(CurItem->endArrowIndex());
	}
	else
	{
		startArrow->setEnabled(false);
		endArrow->setEnabled(false);
	}
	NameEdit->setText(CurItem->itemName());
	RoundRect->setValue(CurItem->cornerRadius()*m_unitRatio);
	QString tm;
	LevelTxt->setText(tm.setNum(CurItem->ItemNr + 1));
	PageItem_TextFrame *i2=CurItem->asTextFrame();
	if (i2!=0)
	{
		disconnect(dGap, SIGNAL(valueChanged(double)), this, SLOT(NewGap()));
		disconnect(DCol, SIGNAL(valueChanged(int)), this, SLOT(NewCols()));
		DCol->setMaximum(qMax(qRound(i2->width() / qMax(i2->ColGap, 10.0)), 1));
		DCol->setMinimum(1);
		DCol->setValue(i2->Cols);
		dGap->setMinimum(0);
		if (colgapLabel->currentIndex() == 0)
		{
			dGap->setMaximum(qMax((i2->width() / i2->Cols - i2->textToFrameDistLeft() - i2->textToFrameDistRight())*m_unitRatio, 0.0));
			dGap->setValue(i2->ColGap*m_unitRatio);
		}
		else
		{
			dGap->setMaximum(qMax((i2->width() / i2->Cols)*m_unitRatio, 0.0));
			dGap->setValue(i2->columnWidth()*m_unitRatio);
		}
		DLeft->setValue(i2->textToFrameDistLeft()*m_unitRatio);
		DTop->setValue(i2->textToFrameDistTop()*m_unitRatio);
		DBottom->setValue(i2->textToFrameDistBottom()*m_unitRatio);
		DRight->setValue(i2->textToFrameDistRight()*m_unitRatio);
		// I put it here because it’s visually grouped with these elements
		// but it’s a PageItem prop. and as such should be set without considering
		// the frame type.
		setFlop(CurItem->firstLineOffset());
		connect(dGap, SIGNAL(valueChanged(double)), this, SLOT(NewGap()));
		connect(DCol, SIGNAL(valueChanged(int)), this, SLOT(NewCols()));
	}
	if (CurItem->asImageFrame())
	{
		imagePageNumber->setMaximum(CurItem->pixm.imgInfo.numberOfPages);
	}
	Revert->setChecked(CurItem->reversed());
	setTextFlowMode(CurItem->textFlowMode());
	/*
	disconnect(FlipH, SIGNAL(clicked()), this, SLOT(handleFlipH()));
	disconnect(FlipV, SIGNAL(clicked()), this, SLOT(handleFlipV()));
	FlipH->setChecked(i->imageFlippedH());
	FlipV->setChecked(i->imageFlippedV());
	connect(FlipH, SIGNAL(clicked()), this, SLOT(handleFlipH()));
	connect(FlipV, SIGNAL(clicked()), this, SLOT(handleFlipV()));
	*/
//	langCombo->setCurrentText(m_ScMW->LangTransl[i->doc()->Language]);
	if (StyledLine->currentItem())
		StyledLine->currentItem()->setSelected(false);
	bool setter;
	if (CurItem->NamedLStyle.isEmpty())
	{
		setter = true;
	}
	else
	{
		QList<QListWidgetItem*> results (StyledLine->findItems(CurItem->NamedLStyle, Qt::MatchFixedString|Qt::MatchCaseSensitive));
		if (results.count() > 0)
			results[0]->setSelected(true); //Qt3-QListBox::ExactMatch));
		setter = false;
	}
	LStyle->setEnabled(setter);
	LSize->setEnabled(setter);
	LJoinStyle->setEnabled(setter);
	LEndStyle->setEnabled(setter);
	if (CurItem->dashes().count() == 0)
		dashEditor->hide();
	else
	{
		LStyle->setCurrentIndex(37);
		dashEditor->setDashValues(CurItem->dashes(), qMax(CurItem->lineWidth(), 0.001), CurItem->dashOffset());
		dashEditor->show();
	}
	connect(StyledLine, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(SetSTline(QListWidgetItem*)));
	connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	connect(startArrow, SIGNAL(activated(int)), this, SLOT(setStartArrow(int )));
	connect(endArrow, SIGNAL(activated(int)), this, SLOT(setEndArrow(int )));

//CB replaces old emits from PageItem::emitAllToGUI()
	disconnect(Xpos, SIGNAL(valueChanged(double)), this, SLOT(NewX()));
	disconnect(Ypos, SIGNAL(valueChanged(double)), this, SLOT(NewY()));
	disconnect(Width, SIGNAL(valueChanged(double)), this, SLOT(NewW()));
	disconnect(Height, SIGNAL(valueChanged(double)), this, SLOT(NewH()));
	disconnect(Locked, SIGNAL(clicked()), this, SLOT(handleLock()));
	disconnect(NoPrint, SIGNAL(clicked()), this, SLOT(handlePrint()));
	disconnect(NoResize, SIGNAL(clicked()), this, SLOT(handleLockSize()));
	disconnect(FlipH, SIGNAL(clicked()), this, SLOT(handleFlipH()));
	disconnect(FlipV, SIGNAL(clicked()), this, SLOT(handleFlipV()));
	disconnect(LSize, SIGNAL(valueChanged(double)), this, SLOT(NewLineWidth()));
	disconnect(LStyle, SIGNAL(activated(int)), this, SLOT(NewLineStyle()));
	disconnect(LJoinStyle, SIGNAL(activated(int)), this, SLOT(NewLineJoin()));
	disconnect(LEndStyle, SIGNAL(activated(int)), this, SLOT(NewLineEnd()));
	disconnect(Rotation, SIGNAL(valueChanged(double)), this, SLOT(setRotation()));
	disconnect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	disconnect(imageXOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));
	disconnect(imageYOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));
	disconnect(DTop, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	disconnect(DLeft, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	disconnect(DRight, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	disconnect(DBottom, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	setXY(i->xPos(), i->yPos());
	setBH(i->width(), i->height());
	NoPrint->setChecked(!i->printEnabled());
	setLocked(i->locked());
	setSizeLocked(i->sizeLocked());
	setFlippedH(i->imageFlippedH());
	setFlippedV(i->imageFlippedV());
	setLineWidth(i->lineWidth());
	setLIvalue(i->lineStyle(), i->lineEnd(), i->lineJoin());
	RoVal = i->rotation();
	double rr = i->rotation();
	if (i->rotation() > 0)
		rr = 360 - rr;
	Rotation->setValue(fabs(rr));
//	setScaleAndOffset(i->imageXScale(), i->imageYScale(), i->imageXOffset(), i->imageYOffset());
	setTextToFrameDistances(i->textToFrameDistLeft(),i->textToFrameDistTop(),i->textToFrameDistBottom(),i->textToFrameDistRight());
	double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation;
	i->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
	Cpal->setActPattern(i->pattern(), patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);

//CB TODO reconnect PP signals from here
	connect(Xpos, SIGNAL(valueChanged(double)), this, SLOT(NewX()));
	connect(Ypos, SIGNAL(valueChanged(double)), this, SLOT(NewY()));
	connect(Width, SIGNAL(valueChanged(double)), this, SLOT(NewW()));
	connect(Height, SIGNAL(valueChanged(double)), this, SLOT(NewH()));
	connect(Locked, SIGNAL(clicked()), this, SLOT(handleLock()));
	connect(NoPrint, SIGNAL(clicked()), this, SLOT(handlePrint()));
	connect(NoResize, SIGNAL(clicked()), this, SLOT(handleLockSize()));
	connect(FlipH, SIGNAL(clicked()), this, SLOT(handleFlipH()));
	connect(FlipV, SIGNAL(clicked()), this, SLOT(handleFlipV()));
	connect(LSize, SIGNAL(valueChanged(double)), this, SLOT(NewLineWidth()));
	connect(LStyle, SIGNAL(activated(int)), this, SLOT(NewLineStyle()));
	connect(LJoinStyle, SIGNAL(activated(int)), this, SLOT(NewLineJoin()));
	connect(LEndStyle, SIGNAL(activated(int)), this, SLOT(NewLineEnd()));
	connect(Rotation, SIGNAL(valueChanged(double)), this, SLOT(setRotation()));
	connect(DTop, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	connect(DLeft, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	connect(DRight, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));
	connect(DBottom, SIGNAL(valueChanged(double)), this, SLOT(NewTDist()));

	if ((CurItem->isTableItem) && (CurItem->isSingleSel))
	{
		setter = true;
		TabStack3->setCurrentIndex(1);
		TopLine->setChecked(CurItem->TopLine);
		LeftLine->setChecked(CurItem->LeftLine);
		RightLine->setChecked(CurItem->RightLine);
		BottomLine->setChecked(CurItem->BottomLine);
		Xpos->setEnabled(false);
		Ypos->setEnabled(false);
		Rotation->setEnabled(false);
	}
	else
	{
		setter = false;
		TabStack3->setCurrentIndex(0);
	}
	LayerGroup->setEnabled(!setter);
	disconnect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(setGroupTransparency(int)));
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(setGroupBlending(int)));
	if ((CurItem->isGroupControl) || ((CurItem->Groups.count() != 0) && (!CurItem->isSingleSel)))
	{
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idShapeItem, false);
		TabStack->setItemEnabled(idGroupItem, true);
		TabStack->setItemEnabled(idLineItem, true);
		TabStack->setItemEnabled(idColorsItem, true);
//		TabStack->setItemEnabled(idLineItem, false);
//		TabStack->setItemEnabled(idColorsItem, false);
		TabStack->setItemEnabled(idTextItem, false);
		TabStack->setItemEnabled(idImageItem, false);
		if (CurItem->FrameType == 0)
			SCustom2->setIcon(SCustom2->getIconPixmap(0));
		if (CurItem->FrameType == 1)
			SCustom2->setIcon(SCustom2->getIconPixmap(1));
		if (CurItem->FrameType > 3)
			SCustom2->setIcon(SCustom2->getIconPixmap(CurItem->FrameType-2));
		TransSpin->setValue(qRound(100 - (CurItem->fillTransparency() * 100)));
		blendMode->setCurrentIndex(CurItem->fillBlendmode());
	}
	else
		TabStack->setItemEnabled(idGroupItem, false);
	connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(setGroupTransparency(int)));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(setGroupBlending(int)));
	/*
	Xpos->setReadOnly(setter);
	Ypos->setReadOnly(setter);
	Rotation->setReadOnly(setter);
	*/
	if (CurItem->asPathText())
	{
		TabStack2->setCurrentIndex(0);
		pathTextType->setCurrentIndex(CurItem->textPathType);
		flippedPathText->setChecked(CurItem->textPathFlipped);
		showcurveCheckBox->setChecked(CurItem->PoShow);
		LineW->setValue(CurItem->BaseOffs * -1);
		Dist->setValue(CurItem->textToFrameDistLeft());
		flopItem->setHidden(true);
		DistanceItem->setHidden(true);
		Distance2Item->setHidden(false);
	}
	else if (CurItem->asTextFrame())
	{
		TabStack2->setCurrentIndex(0);
		flopItem->setHidden(false);
		DistanceItem->setHidden(false);
		Distance2Item->setHidden(true);
	}
	else
	{
		TabStack2->setCurrentIndex(1);
		Distance3->setVisible(i->itemType() != PageItem::ImageFrame);
		flopItem->setHidden(false);
		DistanceItem->setHidden(false);
		Distance2Item->setHidden(true);
	}
	NonZero->setChecked(!CurItem->fillRule);
	EvenOdd->setChecked(CurItem->fillRule);
	// Frame type 3 is obsolete: CR 2005-02-06
	//if (((i->itemType() == PageItem::TextFrame) || (i->itemType() == PageItem::ImageFrame) || (i->itemType() == 3)) &&  (!i->ClipEdited))
	if (((CurItem->asTextFrame()) || (CurItem->asImageFrame())) &&  (!CurItem->ClipEdited) && ((CurItem->FrameType == 0) || (CurItem->FrameType == 2)))
		RoundRect->setEnabled(true);
	else
	{
		RoundRect->setEnabled ((CurItem->asPolygon()) &&  (!CurItem->ClipEdited)  && ((CurItem->FrameType == 0) || (CurItem->FrameType == 2)));
	}
	KnockOut->setChecked(!CurItem->doOverprint);
	Overprint->setChecked(CurItem->doOverprint);
	if ((CurItem->itemType() == PageItem::Line) && LMode)
	{
		xposLabel->setText( tr( "&X1:" ) );
		widthLabel->setText( tr( "X&2:" ) );
		yposLabel->setText( tr( "Y&1:" ) );
		heightLabel->setText( tr( "&Y2:" ) );
		Rotation->setEnabled(false);
	}
	else
	{
		xposLabel->setText( tr( "&X-Pos:" ) );
		widthLabel->setText( tr( "&Width:" ) );
		yposLabel->setText( tr( "&Y-Pos:" ) );
		heightLabel->setText( tr( "&Height:" ) );
		Rotation->setEnabled(!((CurItem->isTableItem) && (CurItem->isSingleSel)));
	}
	HaveItem = true;
	if (CurItem->asLine())
	{
		keepFrameWHRatioButton->setEnabled(false);
		Height->setEnabled(LMode && !CurItem->locked());
	}
	else
	{
		Height->setEnabled(true);
		keepFrameWHRatioButton->setEnabled(true);
		if (CurItem->asImageFrame())
		{
			updateCmsList();
			EditEffects->setShown(CurItem->PicAvail && CurItem->isRaster);
			EditPSDProps->setShown(CurItem->PicAvail && CurItem->pixm.imgInfo.valid);
			setter = CurItem->ScaleType;
			FreeScale->setChecked(setter);
			FrameScale->setChecked(!setter);
			if (CurItem->asLatexFrame())
			{
				FreeScale->setEnabled(false);
				FrameScale->setEnabled(false);
				Aspect->setEnabled(false);
				imageXScaleSpinBox->setEnabled(false);
				imageYScaleSpinBox->setEnabled(false);
				imgDpiX->setEnabled(false);
				imgDpiY->setEnabled(false);
			}
			else
			{
				imageXScaleSpinBox->setEnabled(setter);
				imageYScaleSpinBox->setEnabled(setter);
				imgDpiX->setEnabled(setter);
				imgDpiY->setEnabled(setter);
				Aspect->setEnabled(!setter);
				Aspect->setChecked(CurItem->AspectRatio);
				FreeScale->setEnabled(true);
				FrameScale->setEnabled(true);
			}
//CB Why do we need this? Setting it too much here
// 			if (setter == true)
// 			{
// 				keepImageWHRatioButton->setChecked(setter);
// 				keepImageDPIRatioButton->setChecked(setter);
// 			}
			imageXOffsetSpinBox->setEnabled(setter);
			imageYOffsetSpinBox->setEnabled(setter);
		}
	}
	setXY(CurItem->xPos(), CurItem->yPos());
	setScaleAndOffset(i->imageXScale(), i->imageYScale(), i->imageXOffset(), i->imageYOffset());
	connect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	connect(imageXOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));
	connect(imageYOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));

	DoGroup->setEnabled(false);
	DoUnGroup->setEnabled(false);
	if (doc->m_Selection->count() > 1)
	{
		bool isGroup = true;
		int firstElem = -1;
		if (CurItem->Groups.count() != 0)
			firstElem = CurItem->Groups.top();
		for (int bx = 0; bx < doc->m_Selection->count(); ++bx)
		{
			if (doc->m_Selection->itemAt(bx)->Groups.count() != 0)
			{
				if (doc->m_Selection->itemAt(bx)->Groups.top() != firstElem)
					isGroup = false;
			}
			else
				isGroup = false;
		}
		if (!isGroup)
			DoGroup->setEnabled(true);
		else
		{
			if (CurItem->isGroupControl)
				NameEdit->setEnabled(true);
		}
		if ((CurItem->Groups.count() != 0) && (isGroup))
			DoUnGroup->setEnabled(true);
	}
	updateSpinBoxConstants();
}

void PropertiesPalette::NewSel(int nr)
{
	if (!HaveDoc || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	int visID;
	PageItem *i=0;
	disconnect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));
	if (doc->m_Selection->count()>1)
	{
		RoVal = 0;
		double gx, gy, gh, gw;
		doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		int bp = RotationGroup->checkedId();
		if (bp == 0)
			m_ScMW->view->RCenter = FPoint(gx, gy);
		else if (bp == 1)
			m_ScMW->view->RCenter = FPoint(gx + gw, gy);
		else if (bp == 2)
			m_ScMW->view->RCenter = FPoint(gx + gw / 2.0, gy + gh / 2.0);
		else if (bp == 3)
			m_ScMW->view->RCenter = FPoint(gx, gy + gh);
		else if (bp == 0)
			m_ScMW->view->RCenter = FPoint(gx + gw, gy + gh);
		xposLabel->setText( tr( "&X-Pos:" ) );
		widthLabel->setText( tr( "&Width:" ) );
		yposLabel->setText( tr( "&Y-Pos:" ) );
		heightLabel->setText( tr( "&Height:" ) );
		HaveItem = false;
		Xpos->setValue(0);
		Ypos->setValue(0);
		Width->setValue(0);
		Height->setValue(0);
		Rotation->setValue(0);
		RoundRect->setValue(0);
		HaveItem = true;
		Xpos->setEnabled(true);
		Ypos->setEnabled(true);
		Width->setEnabled(true);
		Height->setEnabled(true);
		Rotation->setEnabled(true);
// 		TabStack->setCurrentIndex(0);
		for (int ws = 1; ws < 7; ++ws)
			TabStack->setItemEnabled(ws, false);
		TabStack->widget(0)->setEnabled(true);
		TabStack->setItemEnabled(idXYZItem, true);
		NameEdit->setEnabled(false);
		TabStack->setItemEnabled(idLineItem, true);
		TabStack->setItemEnabled(idColorsItem, true);
		FlipH->setCheckable( false );
		FlipV->setCheckable( false );
		FlipH->setChecked(false);
		FlipV->setChecked(false);
		flopRealHeight->setChecked(true);
	}
	else
	{
		if (nr!=-1)
		{
			i=doc->m_Selection->itemAt(0);
			HaveItem=true;
			EditShape->setEnabled(!i->locked());
			SCustom->setEnabled(nr!=5 && nr!=7 && nr!=8 && !i->locked());
		}
		else
		{
			DoGroup->setEnabled(false);
			DoUnGroup->setEnabled(false);
			EditShape->setEnabled(false);
			SCustom->setEnabled(false);
		}
		NameEdit->setEnabled(true);
// 		ShapeGroup->setEnabled(false);
// 		RoundRect->setEnabled(false);
//		Distance->setEnabled(false);
		LineMode->setEnabled(false);
		RotationGroup->setEnabled(true);
/*		TopLeft->setEnabled(true);
		TopRight->setEnabled(true);
		BottomLeft->setEnabled(true);
		BottomRight->setEnabled(true);
		Center->setEnabled(true); */
		visID = TabStack->currentIndex();
		TabStack->widget(0)->setEnabled(true);
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idColorsItem, true);
		/*
		disconnect(FlipH, SIGNAL(clicked()), this, SLOT(handleFlipH()));
		disconnect(FlipV, SIGNAL(clicked()), this, SLOT(handleFlipV()));
		FlipH->setChecked(false);
		FlipV->setChecked(false);
		connect(FlipH, SIGNAL(clicked()), this, SLOT(handleFlipH()));
		connect(FlipV, SIGNAL(clicked()), this, SLOT(handleFlipV()));
		*/
		
		//CB If Toggle is not possible, then we need to enable it so we can turn it off
		//It then gets reset below for items where its valid
		if ((nr>4) && (nr<9))
		{
			FlipH->setCheckable(true);
			FlipV->setCheckable(true);
			FlipH->setChecked(false);
			FlipV->setChecked(false);
		}
		FlipH->setCheckable((nr>=0) && (nr<5));
		FlipV->setCheckable((nr>=0) && (nr<5));
		//CB Why cant we do this for lines?
//		FlipH->setEnabled((nr!=-1) && (nr!=5));
//		FlipV->setEnabled((nr!=-1) && (nr!=5));
		FlipH->setEnabled(nr!=-1);
		FlipV->setEnabled(nr!=-1);
		switch (nr)
		{
		case -1:
			xposLabel->setText( tr( "&X-Pos:" ) );
			widthLabel->setText( tr( "&Width:" ) );
			yposLabel->setText( tr( "&Y-Pos:" ) );
			heightLabel->setText( tr( "&Height:" ) );
			//Rotation->setEnabled(true);
			//Height->setEnabled(true);
			RoundRect->setEnabled(false);
			HaveItem = false;
			Xpos->setValue(0);
			Ypos->setValue(0);
			Width->setValue(0);
			Height->setValue(0);
			Rotation->setValue(0);
			RoundRect->setValue(0);
			for (int ws = 1; ws < 7; ++ws)
				TabStack->setItemEnabled(ws, false);
// 			TabStack->setCurrentIndex(0);
			TabStack->widget(0)->setEnabled(false);
			TabStack->setItemEnabled(idXYZItem, false);
			Cpal->ChooseGrad(0);
			break;
		case 2:
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, true);
			TabStack->setItemEnabled(idLineItem, true);
			if ((!i->ClipEdited) && ((i->FrameType == 0) || (i->FrameType == 2)))
				RoundRect->setEnabled(!i->locked());
			else
				RoundRect->setEnabled(false);
			if ((doc->m_Selection->itemAt(0)->FrameType == 0) || (doc->m_Selection->itemAt(0)->FrameType == 2))
				RoundRect->setEnabled(!i->locked());
// 			if (visID == 2)
// 				TabStack->setCurrentIndex(0);
			break;
		case 4:
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, true);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			if ((!i->ClipEdited) && ((i->FrameType == 0) || (i->FrameType == 2)))
				RoundRect->setEnabled(!i->locked());
			else
				RoundRect->setEnabled(false);
//			Distance->setEnabled(true);
// 			if (visID == 3)
// 				TabStack->setCurrentIndex(0);
			break;
		case 5:
			TabStack->setItemEnabled(idShapeItem, false);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			RoundRect->setEnabled(false);
			LineMode->setEnabled(true);
			RotationGroup->setEnabled(false);
/*			TopLeft->setEnabled(false);
			TopRight->setEnabled(false);
			BottomLeft->setEnabled(false);
			BottomRight->setEnabled(false);
			Center->setEnabled(false); */
// 			if ((visID == 1) || (visID == 2) || (visID == 3))
// 				TabStack->setCurrentIndex(0);
			break;
		case 1:
		case 3:
		case 6:
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			if ((!i->ClipEdited) && ((i->FrameType == 0) || (i->FrameType == 2)))
				RoundRect->setEnabled(!i->locked());
			else
				RoundRect->setEnabled(false);
// 			if ((visID == 2) || (visID == 3))
// 				TabStack->setCurrentIndex(0);
			break;
		case 7:
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			RoundRect->setEnabled(false);
// 			if ((visID == 2) || (visID == 3))
// 				TabStack->setCurrentIndex(0);
			break;
		case 8:
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, true);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			RoundRect->setEnabled(false);
// 			if (visID == 3)
// 				TabStack->setCurrentIndex(0);
			break;
		}
	}
	updateGeometry();
//	setFocus();
	repaint();
	connect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));
}

void PropertiesPalette::setMultipleSelection(bool isMultiple)
{
	if (!HaveDoc)
		return;
	//CB Having added the selection and undo transaction to mirrorpolyh/v in doc,
	//these can be enabled all the time
	FlipH->setEnabled(true);
	FlipV->setEnabled(true);
	NameEdit->setEnabled(!isMultiple);
	if (doc->m_Selection->count() > 1)
	{
		FlipH->setCheckable( false );
		FlipV->setCheckable( false );
		PageItem *i;
		uint lowestItem = 999999;
		for (int a=0; a<doc->m_Selection->count(); ++a)
		{
			i = doc->m_Selection->itemAt(a);
			lowestItem = qMin(lowestItem, i->ItemNr);
		}
		i = doc->Items->at(lowestItem);
		SetCurItem(i);
	}
}

void PropertiesPalette::unitChange()
{
	if (!HaveDoc)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	double oldRatio = m_unitRatio;
	m_unitRatio = doc->unitRatio();
	m_unitIndex = doc->unitIndex();
    Xpos->setNewUnit( m_unitIndex );
    Ypos->setNewUnit( m_unitIndex );
    Width->setNewUnit( m_unitIndex );
    Height->setNewUnit( m_unitIndex );
    imageXOffsetSpinBox->setNewUnit( m_unitIndex );
    imageYOffsetSpinBox->setNewUnit( m_unitIndex );
    dGap->setNewUnit( m_unitIndex );
    DLeft->setNewUnit( m_unitIndex );
    DTop->setNewUnit( m_unitIndex );
    DBottom->setNewUnit( m_unitIndex );
    DRight->setNewUnit( m_unitIndex );
    RoundRect->setNewUnit( m_unitIndex );
    LSize->setNewUnit( m_unitIndex );
/*
	double maxXYWHVal=30000 * m_unitRatio;
	double minXYVal=-30000 * m_unitRatio;

	double ratioDivisor = m_unitRatio / oldRatio;
	double newX = Xpos->value() * ratioDivisor;
	double newY = Ypos->value() * ratioDivisor;
	double newW = Width->value() * ratioDivisor;
	double newH = Height->value() * ratioDivisor;
	double newLX = imageXOffsetSpinBox->value() * ratioDivisor;
	double newLY = imageYOffsetSpinBox->value() * ratioDivisor;
	double newG = dGap->value() * ratioDivisor;
	double newGM = dGap->maximum() * ratioDivisor;
	double newDT = DTop->value() * ratioDivisor;
	double newDL = DLeft->value() * ratioDivisor;
	double newDB = DBottom->value() * ratioDivisor;
	double newDR = DRight->value() * ratioDivisor;
	double newRR = RoundRect->value() * ratioDivisor;
	double newRM = RoundRect->maximum() * ratioDivisor;
	double newLZ = LSize->value() * ratioDivisor;

	if (doc->unitIndex() > unitGetMaxIndex())
		doc->setUnitIndex(0);
	QString ein = unitGetSuffixFromIndex(doc->unitIndex());

	Xpos->setSuffix( ein );
	Ypos->setSuffix( ein );
	Width->setSuffix( ein );
	Height->setSuffix( ein );
	imageXOffsetSpinBox->setSuffix( ein );
	imageYOffsetSpinBox->setSuffix( ein );
	dGap->setSuffix( ein );
	DLeft->setSuffix( ein );
	DTop->setSuffix( ein );
	DBottom->setSuffix( ein );
	DRight->setSuffix( ein );
	RoundRect->setSuffix( ein );
	LSize->setSuffix( ein );
	LSize->setValue(newLZ);

	int decimals = unitGetDecimalsFromIndex(doc->unitIndex());

	Xpos->setValues( minXYVal, maxXYWHVal, decimals, newX );
	Ypos->setValues( minXYVal, maxXYWHVal, decimals, newY );
	Width->setValues( m_unitRatio, maxXYWHVal, decimals, newW );
	Height->setValues( m_unitRatio, maxXYWHVal, decimals, newH );

	imageXOffsetSpinBox->setDecimals(decimals);
	imageXOffsetSpinBox->setMaximum( maxXYWHVal );
	imageXOffsetSpinBox->setValue(newLX);

	imageYOffsetSpinBox->setDecimals(decimals);
	imageYOffsetSpinBox->setMaximum( maxXYWHVal );
	imageYOffsetSpinBox->setValue(newLY);

	dGap->setDecimals(decimals);
	dGap->setMaximum(newGM);
	dGap->setValue(newG);

	DLeft->setDecimals(decimals);
	DLeft->setMaximum( 300 );
	DLeft->setValue(newDL);

	DTop->setDecimals(decimals);
	DTop->setMaximum( 300 );
	DTop->setValue(newDT);

	DBottom->setDecimals(decimals);
	DBottom->setMaximum( 300 );
	DBottom->setValue(newDB);

	DRight->setDecimals(decimals);
	DRight->setMaximum( 300 );
	DRight->setValue(newDR);

	RoundRect->setValues(-newRM, newRM, decimals, newRR);
*/
	Cpal->unitChange(oldRatio, m_unitRatio, doc->unitIndex());
	HaveItem = tmp;
}

void PropertiesPalette::setLevel(uint l)
{
	QString tm;
	LevelTxt->setText(tm.setNum(l + 1));
}

void PropertiesPalette::setXY(double x, double y)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	disconnect(Xpos, SIGNAL(valueChanged(double)), this, SLOT(NewX()));
	disconnect(Ypos, SIGNAL(valueChanged(double)), this, SLOT(NewY()));
	bool tmp = HaveItem;
	double inX, inY, b, h, r, dummy1, dummy2;
	QMatrix ma;
	FPoint n;
	if (HaveItem)
	{
		if (doc->m_Selection->isMultipleSelection())
		{
			doc->m_Selection->getGroupRect(&dummy1, &dummy2, &b, &h);
			r = 0.0;
			ma.translate(dummy1, dummy2);
		}
		else
		{
			b = CurItem->width();
			h = CurItem->height();
			r = CurItem->rotation();
			ma.translate(x, y);
		}
	}
	else
	{
		b = 0.0;
		h = 0.0;
		r = 0.0;
		ma.translate(x, y);
	}
	HaveItem = false;
//	ma.translate(x, y);
	ma.rotate(r);
	int bp = RotationGroup->checkedId();
	if (bp == 0)
		n = FPoint(0.0, 0.0);
	else if (bp == 1)
		n = FPoint(b, 0.0);
	else if (bp == 2)
		n = FPoint(b / 2.0, h / 2.0);
	else if (bp == 3)
		n = FPoint(0.0, h);
	else if (bp == 4)
		n = FPoint(b, h);
	inX = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
	inY = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
	if (tmp)
	{
		inX -= doc->rulerXoffset;
		inY -= doc->rulerYoffset;
		if (doc->guidesSettings.rulerMode)
		{
			inX -= doc->currentPage()->xOffset();
			inY -= doc->currentPage()->yOffset();
		}
	}
	Xpos->setValue(inX*m_unitRatio);
	Ypos->setValue(inY*m_unitRatio);
	if ((LMode) && (tmp))
		setBH(CurItem->width(), CurItem->height());
	HaveItem = tmp;
	connect(Xpos, SIGNAL(valueChanged(double)), this, SLOT(NewX()));
	connect(Ypos, SIGNAL(valueChanged(double)), this, SLOT(NewY()));
}

void PropertiesPalette::setBH(double x, double y)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	QMatrix ma;
	QPoint dp;
	if ((LMode) && (CurItem->asLine()))
	{
		ma.translate(static_cast<double>(Xpos->value()) / m_unitRatio, static_cast<double>(Ypos->value()) / m_unitRatio);
		ma.rotate(static_cast<double>(Rotation->value())*(-1));
		// Qt4 dp = ma * QPoint(static_cast<int>(x), static_cast<int>(y));
		dp = QPoint(static_cast<int>(x), static_cast<int>(y)) * ma;
		Width->setValue(dp.x()*m_unitRatio);
		Height->setValue(dp.y()*m_unitRatio);
	}
	else
	{
		RoundRect->setMaximum(qMin(x, y)/2*m_unitRatio);
		RoundRect->setMinimum(-qMin(x, y)/2*m_unitRatio);
		Width->setValue(x*m_unitRatio);
		Height->setValue(y*m_unitRatio);
	}
	HaveItem = tmp;
}

void PropertiesPalette::setR(double r)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	double rr = r;
	if (r > 0)
		rr = 360 - rr;
	HaveItem = false;
	Rotation->setValue(fabs(rr));
	HaveItem = tmp;
}

void PropertiesPalette::setRR(double r)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	RoundRect->setValue(r*m_unitRatio);
	HaveItem = tmp;
}

void PropertiesPalette::setCols(int r, double g)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	DCol->setValue(r);
	dGap->setValue(g*m_unitRatio);
	if (tmp)
	{
		PageItem_TextFrame *i2=CurItem->asTextFrame();
		if (i2!=0)
		{
			DCol->setMaximum(qMax(qRound(i2->width() / qMax(i2->ColGap, 10.0)), 1));
			if (colgapLabel->currentIndex() == 0)
			{
				dGap->setMaximum(qMax((i2->width() / i2->Cols - i2->textToFrameDistLeft() - i2->textToFrameDistRight())*m_unitRatio, 0.0));
				dGap->setValue(i2->ColGap*m_unitRatio);
			}
			else
			{
				dGap->setMaximum(qMax((i2->width() / i2->Cols)*m_unitRatio, 0.0));
				dGap->setValue(i2->columnWidth()*m_unitRatio);
			}
		}
	}
	DCol->setMinimum(1);
	dGap->setMinimum(0);
	HaveItem = tmp;
}

// NewLspMode?
// void PropertiesPalette::setLspMode(QAction *id)
// {
// 	if ((HaveDoc) && (HaveItem))
// 	{
// 		doc->itemSelection_SetLineSpacingMode(lineSpacingPop->actions().indexOf(id));
// 		updateStyle(doc->appMode == modeEdit? CurItem->currentStyle() : CurItem->itemText.defaultStyle());
// 	}
// }

// NewLspMode?
void PropertiesPalette::setLineSpacingMode(int id)
{
	if ((HaveDoc) && (HaveItem))
	{
		doc->itemSelection_SetLineSpacingMode(id);
		updateStyle(doc->appMode == modeEdit? CurItem->currentStyle() : CurItem->itemText.defaultStyle());
	}
}

void PropertiesPalette::setLsp(double r)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	LineSp->setValue(r);
	const ParagraphStyle& curStyle(tmp && doc->appMode == modeEdit? CurItem->currentStyle() : CurItem->itemText.defaultStyle());
	if (tmp)
	{
		setupLineSpacingSpinbox(curStyle.lineSpacingMode(), r);
		lineSpacingModeCombo->setCurrentIndex(curStyle.lineSpacingMode());
/*
		QList<QAction*> actList = lineSpacingPop->actions();
		for (int al = 0; al < actList.count(); ++al)
		{
			actList[al]->setChecked(false);
		}
		actList[curStyle.lineSpacingMode()]->setChecked(true);
*/
	}
	HaveItem = tmp;
}

void PropertiesPalette::setTextToFrameDistances(double left, double top, double bottom, double right)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	DLeft->setValue(left*m_unitRatio);
	DTop->setValue(top*m_unitRatio);
	DBottom->setValue(bottom*m_unitRatio);
	DRight->setValue(right*m_unitRatio);
	HaveItem = tmp;
}

void PropertiesPalette::setFontFace(const QString& newFont)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	if (CurItem != NULL)
		Fonts->RebuildList(doc, CurItem->isAnnotation());
	Fonts->setCurrentFont(newFont);
	HaveItem = tmp;
}


void PropertiesPalette::setSize(double s)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	Size->setValue(s / 10.0);
	HaveItem = tmp;
}

void PropertiesPalette::setExtra(double e)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	Extra->setValue(e / 10.0);
	HaveItem = tmp;
}

void PropertiesPalette::ChangeScaling()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if (FreeScale == sender())
	{
		FrameScale->setChecked(false);
		FreeScale->setChecked(true);
		Aspect->setEnabled(false);
		imageXOffsetSpinBox->setEnabled(true);
		imageYOffsetSpinBox->setEnabled(true);
		imageXScaleSpinBox->setEnabled(true);
		imageYScaleSpinBox->setEnabled(true);
		imgDpiX->setEnabled(true);
		imgDpiY->setEnabled(true);
	}
	if (FrameScale == sender())
	{
		FrameScale->setChecked(true);
		FreeScale->setChecked(false);
		Aspect->setEnabled(true);
		imageXOffsetSpinBox->setEnabled(false);
		imageYOffsetSpinBox->setEnabled(false);
		imageXScaleSpinBox->setEnabled(false);
		imageYScaleSpinBox->setEnabled(false);
		imgDpiX->setEnabled(false);
		imgDpiY->setEnabled(false);
	}
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setImageScalingMode(FreeScale->isChecked(), Aspect->isChecked());
		emit UpdtGui(PageItem::ImageFrame);
		emit DocChanged();
//		setFocus();
	}
}

void PropertiesPalette::setScaleAndOffset(double scx, double scy, double x, double y)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	disconnect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
	disconnect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
	disconnect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	disconnect(keepImageWHRatioButton, SIGNAL(clicked()), this, SLOT(ToggleKette()));
	disconnect(keepImageDPIRatioButton, SIGNAL(clicked()), this, SLOT(ToggleKetteD()));
	if (fabs(scx - scy) > 0.0002)
	{
		keepImageWHRatioButton->setChecked(false);
		keepImageDPIRatioButton->setChecked(false);
	}
	if (tmp)
	{
		imageXOffsetSpinBox->setValue(x * m_unitRatio * CurItem->imageXScale());
		imageYOffsetSpinBox->setValue(y * m_unitRatio * CurItem->imageYScale());
		imageXScaleSpinBox->setValue(scx * 100 / 72.0 * CurItem->pixm.imgInfo.xres);
		imageYScaleSpinBox->setValue(scy * 100 / 72.0 * CurItem->pixm.imgInfo.yres);
		imgDpiX->setValue(qRound(720.0 / CurItem->imageXScale()) / 10.0);
		imgDpiY->setValue(qRound(720.0 / CurItem->imageYScale()) / 10.0);
	}
	else
	{
		imageXOffsetSpinBox->setValue(x * m_unitRatio);
		imageYOffsetSpinBox->setValue(y * m_unitRatio);
		imageXScaleSpinBox->setValue(scx * 100);
		imageYScaleSpinBox->setValue(scy * 100);
		imgDpiX->setValue(72);
		imgDpiY->setValue(72);
	}
	connect(keepImageWHRatioButton, SIGNAL(clicked()), this, SLOT(ToggleKette()));
	connect(keepImageDPIRatioButton, SIGNAL(clicked()), this, SLOT(ToggleKetteD()));
	connect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
	connect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
	connect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	HaveItem = tmp;
}

void PropertiesPalette::setLineWidth(double s)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	LSize->setValue(s*m_unitRatio);
	if (tmp)
	{
		if (CurItem->dashes().count() != 0)
		{
			if (CurItem->lineWidth() != 0.0)
			{
				dashEditor->setDashValues(CurItem->dashes(), CurItem->lineWidth(), CurItem->dashOffset());
				dashEditor->setEnabled(true);
			}
			else
				dashEditor->setEnabled(false);
		}
	}
	HaveItem = tmp;
}

void PropertiesPalette::setLIvalue(Qt::PenStyle p, Qt::PenCapStyle pc, Qt::PenJoinStyle pj)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	if (tmp)
	{
		if (CurItem->dashes().count() != 0)
		{
			LStyle->setCurrentIndex(37);
			dashEditor->setDashValues(CurItem->dashes(), qMax(CurItem->lineWidth(), 0.001), CurItem->dashOffset());
		}
		else
			LStyle->setCurrentIndex(static_cast<int>(p) - 1);
	}
	else
		LStyle->setCurrentIndex(static_cast<int>(p) - 1);
	switch (pc)
	{
	case Qt::FlatCap:
		LEndStyle->setCurrentIndex(0);
		break;
	case Qt::SquareCap:
		LEndStyle->setCurrentIndex(1);
		break;
	case Qt::RoundCap:
		LEndStyle->setCurrentIndex(2);
		break;
	default:
		LEndStyle->setCurrentIndex(0);
		break;
	}
	switch (pj)
	{
	case Qt::MiterJoin:
		LJoinStyle->setCurrentIndex(0);
		break;
	case Qt::BevelJoin:
		LJoinStyle->setCurrentIndex(1);
		break;
	case Qt::RoundJoin:
		LJoinStyle->setCurrentIndex(2);
		break;
	default:
		LJoinStyle->setCurrentIndex(0);
		break;
	}
	HaveItem = tmp;
}

void PropertiesPalette::setFlop( FirstLineOffsetPolicy f )
{
	if(f == FLOPFontAscent)
		flopFontAscent->setChecked(true);
	else if(f == FLOPLineSpacing)
		flopLineSpacing->setChecked(true);
	else
		flopRealHeight->setChecked(true); //It’s historical behaviour.
}

void PropertiesPalette::setupLineSpacingSpinbox(int mode, double value)
{
	if (mode > 0)
	{
		if (mode==1)
			LineSp->setSpecialValueText( tr( "Auto" ) );
		if (mode==2)
			LineSp->setSpecialValueText( tr( "Baseline" ) );
		LineSp->setMinimum(0);
		LineSp->setValue(0);
		LineSp->setEnabled(false);
	}
	else
	{
		LineSp->setSpecialValueText("");
		LineSp->setMinimum(1);
		LineSp->setValue(value);
		LineSp->setEnabled(true);
	}
}

void PropertiesPalette::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	setupLineSpacingSpinbox(newCurrent.lineSpacingMode(), newCurrent.lineSpacing());
	lineSpacingModeCombo->setCurrentIndex(newCurrent.lineSpacingMode());
	/*
	QList<QAction*> actList = lineSpacingPop->actions();
	for (int al = 0; al < actList.count(); ++al)
	{
		actList[al]->setChecked(false);
	}
	actList[newCurrent.lineSpacingMode()]->setChecked(true);
	*/
	GroupAlign->setStyle(newCurrent.alignment());
	minWordTrackingSpinBox->setValue(newCurrent.minWordTracking() * 100.0);
	normWordTrackingSpinBox->setValue(newCurrent.charStyle().wordTracking() * 100.0);
	minGlyphExtSpinBox->setValue(newCurrent.minGlyphExtension() * 100.0);
	maxGlyphExtSpinBox->setValue(newCurrent.maxGlyphExtension() * 100.0);
	HaveItem = tmp;
	
	
}

void PropertiesPalette::setStil(int s)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
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

void PropertiesPalette::setAli(int e)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	GroupAlign->setEnabled(true);
	GroupAlign->setStyle(e);
	HaveItem = tmp;
}


void PropertiesPalette::setParStyle(const QString& name)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	paraStyleCombo->setFormat(name);
	HaveItem = tmp;
}


void PropertiesPalette::setCharStyle(const QString& name)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	charStyleCombo->setFormat(name);
	HaveItem = tmp;
}

void PropertiesPalette::setOpticalMargins(int i)
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetOpticalMargins(i==0 ? ParagraphStyle::OM_None : ParagraphStyle::OM_Default);
}

void PropertiesPalette::setMinWordTracking()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	ParagraphStyle newStyle;
	newStyle.setMinWordTracking(minWordTrackingSpinBox->value() / 100.0);
	doc->itemSelection_ApplyParagraphStyle(newStyle);
}


void PropertiesPalette::setNormWordTracking()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	ParagraphStyle newStyle;
//	newStyle.setNormWordTracking(percent / 100.0);
	newStyle.charStyle().setWordTracking(normWordTrackingSpinBox->value() / 100.0);
	doc->itemSelection_ApplyParagraphStyle(newStyle);
}

void PropertiesPalette::setMinGlyphExtension()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	ParagraphStyle newStyle;
	newStyle.setMinGlyphExtension(minGlyphExtSpinBox->value() / 100.0);
	doc->itemSelection_ApplyParagraphStyle(newStyle);
}

void PropertiesPalette::setMaxGlyphExtension()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	ParagraphStyle newStyle;
	newStyle.setMaxGlyphExtension(maxGlyphExtSpinBox->value() / 100.0);
	doc->itemSelection_ApplyParagraphStyle(newStyle);
}


void PropertiesPalette::setTScaleV(double e)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	ChScaleV->setValue(e / 10.0);
	HaveItem = tmp;
}

void PropertiesPalette::NewTScaleV()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetScaleV(qRound(ChScaleV->value() * 10));
}

void PropertiesPalette::NewTBase()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetBaselineOffset(qRound(ChBase->value() * 10));
}

void PropertiesPalette::setTScale(double e)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	ChScale->setValue(e / 10.0);
	HaveItem = tmp;
}

void PropertiesPalette::setTBase(double e)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool tmp = HaveItem;
	HaveItem = false;
	ChBase->setValue(e / 10.0);
	HaveItem = tmp;
}

void PropertiesPalette::NewTScale()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetScaleH(qRound(ChScale->value() * 10));
}

void PropertiesPalette::NewX()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		double x,y,w,h, gx, gy, gh, gw, base;
		QMatrix ma;
		x = Xpos->value() / m_unitRatio;
		y = Ypos->value() / m_unitRatio;
		w = Width->value() / m_unitRatio;
		h = Height->value() / m_unitRatio;
		base = 0;
		x += doc->rulerXoffset;
		y += doc->rulerYoffset;
		if (doc->guidesSettings.rulerMode)
		{
			x += doc->currentPage()->xOffset();
			y += doc->currentPage()->yOffset();
		}
		if (doc->m_Selection->isMultipleSelection())
		{
			doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			int bp = RotationGroup->checkedId();
			if ((bp == 0) || (bp == 3))
				base = gx;
			else if (bp == 2)
				base = gx + gw / 2.0;
			else if ((bp == 1) || (bp == 4))
				base = gx + gw;
			if (!_userActionOn)
				m_ScMW->view->startGroupTransaction();
			doc->moveGroup(x - base, 0, true);
			if (!_userActionOn)
			{
				m_ScMW->view->endGroupTransaction();
			}
		}
		else
		{
			if ((CurItem->asLine()) && (LMode))
			{
				w += doc->rulerXoffset;
				h += doc->rulerYoffset;
				if (doc->guidesSettings.rulerMode)
				{
					w += doc->currentPage()->xOffset();
					h += doc->currentPage()->yOffset();
				}
				double r = atan2(h-y,w-x)*(180.0/M_PI);
				w = sqrt(pow(w-x,2)+pow(h-y,2));
//				doc->MoveItem(x - CurItem->xPos(), 0, CurItem, true);
				CurItem->setXYPos(x, CurItem->yPos(), true);
				CurItem->setRotation(r, true);
				doc->SizeItem(w, CurItem->height(), CurItem->ItemNr, true);
//				doc->RotateItem(r, CurItem->ItemNr);
			}
			else
			{
				ma.translate(CurItem->xPos(), CurItem->yPos());
				ma.rotate(CurItem->rotation());
				int bp = RotationGroup->checkedId();
				if (bp == 0)
					base = CurItem->xPos();
				else if (bp == 2)
					base = ma.m11() * (CurItem->width() / 2.0) + ma.m21() * (CurItem->height() / 2.0) + ma.dx();
				else if (bp == 1)
					base = ma.m11() * CurItem->width() + ma.m21() * 0.0 + ma.dx();
				else if (bp == 4)
					base = ma.m11() * CurItem->width() + ma.m21() * CurItem->height() + ma.dx();
				else if (bp == 3)
					base = ma.m11() * 0.0 + ma.m21() * CurItem->height() + ma.dx();
				doc->MoveItem(x - base, 0, CurItem, true);
			}
		}
		doc->regionsChanged()->update(QRect());
		emit DocChanged();
	}
}

void PropertiesPalette::NewY()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;

	double x,y,w,h, gx, gy, gh, gw, base;
	QMatrix ma;
	x = Xpos->value() / m_unitRatio;
	y = Ypos->value() / m_unitRatio;
	w = Width->value() / m_unitRatio;
	h = Height->value() / m_unitRatio;
	base = 0;
	x += doc->rulerXoffset;
	y += doc->rulerYoffset;
	if (doc->guidesSettings.rulerMode)
	{
		x += doc->currentPage()->xOffset();
		y += doc->currentPage()->yOffset();
	}
	if (doc->m_Selection->isMultipleSelection())
	{
		doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		int bp = RotationGroup->checkedId();
		if ((bp == 0) || (bp == 1))
			base = gy;
		else if (bp == 2)
			base = gy + gh / 2.0;
		else if ((bp == 3) || (bp == 4))
			base = gy + gh;
		if (!_userActionOn)
			m_ScMW->view->startGroupTransaction();
		doc->moveGroup(0, y - base, true);
		if (!_userActionOn)
		{
			m_ScMW->view->endGroupTransaction();
		}
	}
	else
	{
		if ((CurItem->asLine()) && (LMode))
		{
			w += doc->rulerXoffset;
			h += doc->rulerYoffset;
			if (doc->guidesSettings.rulerMode)
			{
				w += doc->currentPage()->xOffset();
				h += doc->currentPage()->yOffset();
			}
			double r = atan2(h-y,w-x)*(180.0/M_PI);
			w = sqrt(pow(w-x,2)+pow(h-y,2));
			doc->MoveItem(0, y - CurItem->yPos(), CurItem, true);
			CurItem->setXYPos(CurItem->xPos(), y, true);
			CurItem->setRotation(r, true);
			doc->SizeItem(w, CurItem->height(), CurItem->ItemNr, true);
			doc->RotateItem(r, CurItem->ItemNr);
		}
		else
		{
			ma.translate(CurItem->xPos(), CurItem->yPos());
			ma.rotate(CurItem->rotation());
			int bp = RotationGroup->checkedId();
			if (bp == 0)
				base = CurItem->yPos();
			else if (bp == 2)
				base = ma.m22() * (CurItem->height() / 2.0) + ma.m12() * (CurItem->width() / 2.0) + ma.dy();
			else if (bp == 1)
				base = ma.m22() * 0.0 + ma.m12() * CurItem->width() + ma.dy();
			else if (bp == 4)
				base = ma.m22() * CurItem->height() + ma.m12() * CurItem->width() + ma.dy();
			else if (bp == 3)
				base = ma.m22() * CurItem->height() + ma.m12() * 0.0 + ma.dy();
			doc->MoveItem(0, y - base, CurItem, true);
		}
	}
	doc->regionsChanged()->update(QRect());
	emit DocChanged();
}

void PropertiesPalette::NewW()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	
	double x,y,w,h, gx, gy, gh, gw;
	x = Xpos->value() / m_unitRatio;
	y = Ypos->value() / m_unitRatio;
	w = Width->value() / m_unitRatio;
	h = Height->value() / m_unitRatio;
	if (doc->m_Selection->isMultipleSelection())
	{
		if (!_userActionOn)
			m_ScMW->view->startGroupTransaction();
		doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		if (keepFrameWHRatioButton->isChecked())
		{
//			m_ScMW->view->frameResizeHandle = 1;
			doc->scaleGroup(w / gw, w / gw);
			setBH(w, (w / gw) * gh);
		}
		else
		{
//			m_ScMW->view->frameResizeHandle = 6;
			doc->scaleGroup(w / gw, 1.0);
			doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			setBH(gw, gh);
		}
		if (!_userActionOn)
		{
			m_ScMW->view->endGroupTransaction();
		}
	}
	else
	{
		bool oldS = CurItem->Sizing;
		CurItem->Sizing = false;
		CurItem->OldB2 = CurItem->width();
		CurItem->OldH2 = CurItem->height();
		if (CurItem->asLine())
		{
			if (LMode)
			{
				double r = atan2(h-y,w-x)*(180.0/M_PI);
				CurItem->setRotation(r, true);
				w = sqrt(pow(w-x,2)+pow(h-y,2));
			}
			doc->SizeItem(w, CurItem->height(), CurItem->ItemNr, true, true, false);
		}
		else
		{
			double oldW = (CurItem->width() != 0.0) ? CurItem->width() : 1.0;
			if (CurItem->isTableItem)
			{
				int rmo = doc->RotMode;
				doc->RotMode = 0;
				double dist = w - CurItem->width();
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
						doc->MoveRotated(bb2->RightLink, FPoint(dist, 0), true);
						bb2 = bb2->RightLink;
					}
					doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr, true);
					bb = bb->BottomLink;
				}
				bb2 = bb;
				while (bb2->RightLink != 0)
				{
					doc->MoveRotated(bb2->RightLink, FPoint(dist, 0), true);
					bb2 = bb2->RightLink;
				}
				doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr, true);
				doc->RotMode = rmo;
				if (keepFrameWHRatioButton->isChecked())
				{
					keepFrameWHRatioButton->setChecked(false);
					setBH(w, (w / oldW) * CurItem->height());
					NewH();
					keepFrameWHRatioButton->setChecked(true);
				}
			}
			else
			{
				if (keepFrameWHRatioButton->isChecked())
				{
					setBH(w, (w / oldW) * CurItem->height());
					doc->SizeItem(w, (w / oldW) * CurItem->height(), CurItem->ItemNr, true, true, false);
				}
				else
					doc->SizeItem(w, CurItem->height(), CurItem->ItemNr, true, true, false);
			}
		}
		CurItem->Sizing = oldS;
	}
	emit DocChanged();
	doc->regionsChanged()->update(QRect());
}

void PropertiesPalette::NewH()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		double x,y,w,h, gx, gy, gh, gw;
		x = Xpos->value() / m_unitRatio;
		y = Ypos->value() / m_unitRatio;
		w = Width->value() / m_unitRatio;
		h = Height->value() / m_unitRatio;
		if (doc->m_Selection->isMultipleSelection())
		{
			if (!_userActionOn)
				m_ScMW->view->startGroupTransaction();
			doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			if (keepFrameWHRatioButton->isChecked())
			{
//				m_ScMW->view->frameResizeHandle = 1;
				doc->scaleGroup(h / gh, h / gh);
				setBH((h / gh) * gw, h);
			}
			else
			{
//				m_ScMW->view->frameResizeHandle = 5;
				doc->scaleGroup(1.0, h / gh);
				doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				setBH(gw, gh);
			}
			if (!_userActionOn)
			{
				m_ScMW->view->endGroupTransaction();
			}
		}
		else
		{
			bool oldS = CurItem->Sizing;
			CurItem->Sizing = false;
			CurItem->OldB2 = CurItem->width();
			CurItem->OldH2 = CurItem->height();
			if (CurItem->asLine())
			{
				if (LMode)
				{
					double r = atan2(h-y,w-x)*(180.0/M_PI);
					CurItem->setRotation(r, true);
					w = sqrt(pow(w-x,2)+pow(h-y,2));
				}
				doc->SizeItem(w, CurItem->height(), CurItem->ItemNr, true, true, false);
			}
			else
			{
				double oldH = (CurItem->height() != 0.0) ? CurItem->height() : 1.0;
				if (CurItem->isTableItem)
				{
					int rmo = doc->RotMode;
					doc->RotMode = 0;
					double dist = h - CurItem->height();
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
							doc->MoveRotated(bb2->BottomLink, FPoint(0, dist), true);
							bb2 = bb2->BottomLink;
						}
						doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr, true);
						bb = bb->RightLink;
					}
					bb2 = bb;
					while (bb2->BottomLink != 0)
					{
						doc->MoveRotated(bb2->BottomLink, FPoint(0, dist), true);
						bb2 = bb2->BottomLink;
					}
					doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr, true);
					doc->RotMode = rmo;
					if (keepFrameWHRatioButton->isChecked())
					{
						keepFrameWHRatioButton->setChecked(false);
						setBH((h / oldH) * CurItem->width(), h);
						NewW();
						keepFrameWHRatioButton->setChecked(true);
					}
				}
				else
				{
					if (keepFrameWHRatioButton->isChecked())
					{
						setBH((h / oldH) * CurItem->width(), h);
						doc->SizeItem((h / oldH) * CurItem->width(), h, CurItem->ItemNr, true, true, false);
					}
					else
						doc->SizeItem(CurItem->width(), h, CurItem->ItemNr, true, true, false);
				}
			}
			CurItem->Sizing = oldS;
		}
		emit DocChanged();
		doc->regionsChanged()->update(QRect());
	}
}

void PropertiesPalette::setRotation()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	double gx, gy, gh, gw;
	if ((HaveDoc) && (HaveItem))
	{
		if (!_userActionOn)
			m_ScMW->view->startGroupTransaction(Um::Rotate, "", Um::IRotate);
		if (doc->m_Selection->isMultipleSelection())
		{
			doc->rotateGroup((Rotation->value() - RoVal)*(-1), m_ScMW->view->RCenter);
			doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			setXY(gx, gy);
		}
		else
			doc->RotateItem(Rotation->value()*(-1), CurItem->ItemNr);
		if (!_userActionOn)
		{
			for (int i = 0; i < doc->m_Selection->count(); ++i)
				doc->m_Selection->itemAt(i)->checkChanges(true);
			m_ScMW->view->endGroupTransaction();
		}
		emit DocChanged();
		doc->regionsChanged()->update(QRect());
		RoVal = Rotation->value();
	}
}

void PropertiesPalette::NewCornerRadius()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	CurItem->setCornerRadius(RoundRect->value() / m_unitRatio);
	m_ScMW->view->SetFrameRounded();
	emit DocChanged();
	doc->regionsChanged()->update(QRect());
}

void PropertiesPalette::NewLineSpacing()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetLineSpacing(LineSp->value());
}

void PropertiesPalette::HandleGapSwitch()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	setCols(CurItem->Cols, CurItem->ColGap);
	dGap->setToolTip("");
//	if (colgapLabel->getState())
	if (colgapLabel->currentIndex() == 0)
		dGap->setToolTip( tr( "Distance between columns" ) );
	else
		dGap->setToolTip( tr( "Column width" ) );
}

void PropertiesPalette::NewCols()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	CurItem->Cols = DCol->value();
	setCols(CurItem->Cols, CurItem->ColGap);
	CurItem->update();
	emit DocChanged();
}

void PropertiesPalette::NewPage()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool reallynew = (CurItem->pixm.imgInfo.actualPageNumber != imagePageNumber->value());
	CurItem->pixm.imgInfo.actualPageNumber = imagePageNumber->value();
	if (reallynew)
		CurItem->loadImage(CurItem->externalFile(), true);
	CurItem->update();
}

void PropertiesPalette::NewGap()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	if (colgapLabel->currentIndex() == 0)
		CurItem->ColGap = dGap->value() / m_unitRatio;
	else
	{
		double lineCorr;
		if (CurItem->lineColor() != CommonStrings::None)
			lineCorr = CurItem->lineWidth();
		else
			lineCorr = 0;
		double newWidth = dGap->value() / m_unitRatio;
		double newGap = qMax(((CurItem->width() - CurItem->textToFrameDistLeft() - CurItem->textToFrameDistRight() - lineCorr) - (newWidth * CurItem->Cols)) / (CurItem->Cols - 1), 0.0);
		CurItem->ColGap = newGap;
	}
	CurItem->update();
	emit DocChanged();
}

void PropertiesPalette::NewSize()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetFontSize(qRound(Size->value()*10.0));
}

void PropertiesPalette::NewTracking()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetTracking(qRound(Extra->value() * 10.0));
}

void PropertiesPalette::NewLocalXY()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetImageOffset(imageXOffsetSpinBox->value() / m_unitRatio / CurItem->imageXScale(), imageYOffsetSpinBox->value() / m_unitRatio / CurItem->imageYScale());
}

void PropertiesPalette::NewLocalSC()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		/*doc->itemSelection_SetImageScale(imageXScaleSpinBox->value() / 100.0 / CurItem->pixm.imgInfo.xres * 72.0, imageYScaleSpinBox->value() / 100.0 / CurItem->pixm.imgInfo.yres * 72.0);
		doc->itemSelection_SetImageOffset(imageXOffsetSpinBox->value() / m_unitRatio / CurItem->imageXScale(), imageYOffsetSpinBox->value() / m_unitRatio / CurItem->imageYScale());
		*/
		//CB Dont pass in the scale to the offset change as its taken from the new scale
		doc->itemSelection_SetImageScaleAndOffset(imageXScaleSpinBox->value() / 100.0 / CurItem->pixm.imgInfo.xres * 72.0, imageYScaleSpinBox->value() / 100.0 / CurItem->pixm.imgInfo.yres * 72.0, imageXOffsetSpinBox->value() / m_unitRatio, imageYOffsetSpinBox->value() / m_unitRatio);
		disconnect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
		disconnect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
		imgDpiX->setValue(qRound(720.0 / CurItem->imageXScale()) / 10.0);
		imgDpiY->setValue(qRound(720.0 / CurItem->imageYScale()) / 10.0);
		connect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
		connect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
	}
}

void PropertiesPalette::NewLocalDpi()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		/*
		doc->itemSelection_SetImageScale(72.0 / imgDpiX->value(), 72.0 / imgDpiY->value());
		doc->itemSelection_SetImageOffset(imageXOffsetSpinBox->value() / m_unitRatio / CurItem->imageXScale(), imageYOffsetSpinBox->value() / m_unitRatio / CurItem->imageYScale());
		*/
		//CB Dont pass in the scale to the offset change as its taken from the new scale
		doc->itemSelection_SetImageScaleAndOffset(72.0 / imgDpiX->value(), 72.0 / imgDpiY->value(), imageXOffsetSpinBox->value() / m_unitRatio, imageYOffsetSpinBox->value() / m_unitRatio);
		disconnect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
		disconnect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
		imageXScaleSpinBox->setValue(CurItem->imageXScale() * 100 / 72.0 * CurItem->pixm.imgInfo.xres);
		imageYScaleSpinBox->setValue(CurItem->imageYScale() * 100 / 72.0 * CurItem->pixm.imgInfo.yres);
		connect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
		connect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	}
}

void PropertiesPalette::handleImageEffects()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->ImageEffects();
}

void PropertiesPalette::handleExtImgProperties()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->view->editExtendedImageProperties();
	emit DocChanged();
}

void PropertiesPalette::NewLineWidth()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		double oldL = CurItem->lineWidth();
		doc->itemSelection_SetLineWidth(LSize->value() / m_unitRatio);
		if (CurItem->dashes().count() != 0)
		{
			if ((oldL != 0.0) && (CurItem->lineWidth() != 0.0))
			{
				for (int a = 0; a < CurItem->DashValues.count(); a++)
				{
					CurItem->DashValues[a] = CurItem->DashValues[a] / oldL * CurItem->lineWidth();
				}
				CurItem->setDashOffset(CurItem->dashOffset() / oldL * CurItem->lineWidth());
			}
			if (CurItem->lineWidth() != 0.0)
			{
				dashEditor->setDashValues(CurItem->dashes(), CurItem->lineWidth(), CurItem->dashOffset());
				dashEditor->setEnabled(true);
			}
			else
				dashEditor->setEnabled(false);
		}
// 		emit DocChanged();
	}
}

void PropertiesPalette::setStartArrow(int id)
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_ApplyArrowHead(id,-1);
}

void PropertiesPalette::setEndArrow(int id)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
		doc->itemSelection_ApplyArrowHead(-1, id);
}

void PropertiesPalette::NewLineStyle()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if (LStyle->currentIndex() == 37)
		{
			if (CurItem->dashes().count() == 0)
			{
				if ((CurItem->lineStyle() == 0) || (CurItem->lineStyle() == 1))
				{
					CurItem->DashValues.append(4.0 * qMax(CurItem->lineWidth(), 1.0));
					CurItem->DashValues.append(2.0 * qMax(CurItem->lineWidth(), 1.0));
				}
				else
					getDashArray(CurItem->lineStyle(), qMax(CurItem->lineWidth(), 1.0), CurItem->DashValues);
			}
			if (CurItem->lineWidth() != 0.0)
				dashEditor->setDashValues(CurItem->dashes(), CurItem->lineWidth(), CurItem->dashOffset());
			else
			{
				dashEditor->setEnabled(false);
				dashEditor->setDashValues(CurItem->dashes(), 1.0, CurItem->dashOffset());
			}
			dashEditor->show();
			CurItem->update();
		}
		else
		{
			CurItem->DashValues.clear();
			dashEditor->hide();
			doc->itemSelection_SetLineArt(static_cast<Qt::PenStyle>(LStyle->currentIndex()+1));
		}
	}
}

void PropertiesPalette::dashChange()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if (CurItem->lineWidth() != 0.0)
		{
			CurItem->setDashes(dashEditor->getDashValues(CurItem->lineWidth()));
			CurItem->setDashOffset(dashEditor->Offset->value() * CurItem->lineWidth());
		}
		CurItem->update();
		emit DocChanged();
	}
}

void PropertiesPalette::NewLineMode()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if (LineMode->currentIndex() == 0)
	{
		xposLabel->setText( tr( "&X-Pos:" ) );
		widthLabel->setText( tr( "&Width:" ) );
		yposLabel->setText( tr( "&Y-Pos:" ) );
		heightLabel->setText( tr( "&Height:" ) );
		Rotation->setEnabled(true);
		Height->setEnabled(false);
		LMode = false;
	}
	else
	{
		xposLabel->setText( tr( "&X1:" ) );
		widthLabel->setText( tr( "X&2:" ) );
		yposLabel->setText( tr( "Y&1:" ) );
		heightLabel->setText( tr( "&Y2:" ) );
		Rotation->setEnabled(false);
		Height->setEnabled(true);
		LMode = true;
	}
	setBH(CurItem->width(), CurItem->height());
	updateGeometry();
//	setFocus();
	repaint();
}

void PropertiesPalette::NewLineJoin()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	Qt::PenJoinStyle c = Qt::MiterJoin;
	switch (LJoinStyle->currentIndex())
	{
	case 0:
		c = Qt::MiterJoin;
		break;
	case 1:
		c = Qt::BevelJoin;
		break;
	case 2:
		c = Qt::RoundJoin;
		break;
	}
	if ((HaveDoc) && (HaveItem))
		doc->itemSelection_SetLineJoin(c);
}

void PropertiesPalette::NewLineEnd()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	Qt::PenCapStyle c = Qt::FlatCap;
	switch (LEndStyle->currentIndex())
	{
	case 0:
		c = Qt::FlatCap;
		break;
	case 1:
		c = Qt::SquareCap;
		break;
	case 2:
		c = Qt::RoundCap;
		break;
	}
	if ((HaveDoc) && (HaveItem))
		doc->itemSelection_SetLineEnd(c);
}

void PropertiesPalette::ToggleKette()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	disconnect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepImageWHRatioButton->isChecked())
	{
		imageYScaleSpinBox->setValue(imageXScaleSpinBox->value());
		NewLocalSC();
		keepImageDPIRatioButton->setChecked(true);
	}
	else
		keepImageDPIRatioButton->setChecked(false);
	connect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void PropertiesPalette::HChange()
{
	disconnect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepImageWHRatioButton->isChecked())
		imageYScaleSpinBox->setValue(imageXScaleSpinBox->value());
	NewLocalSC();
	connect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void PropertiesPalette::VChange()
{
	disconnect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepImageWHRatioButton->isChecked())
		imageXScaleSpinBox->setValue(imageYScaleSpinBox->value());
	NewLocalSC();
	connect(imageXScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(imageYScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void PropertiesPalette::ToggleKetteD()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	disconnect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
	disconnect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
	if (keepImageDPIRatioButton->isChecked())
	{
		imgDpiY->setValue(imgDpiX->value());
		NewLocalDpi();
		keepImageWHRatioButton->setChecked(true);
	}
	else
		keepImageWHRatioButton->setChecked(false);
	connect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
	connect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
}

void PropertiesPalette::HChangeD()
{
	disconnect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
	disconnect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
	if (keepImageDPIRatioButton->isChecked())
		imgDpiY->setValue(imgDpiX->value());
	NewLocalDpi();
	connect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
	connect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
}

void PropertiesPalette::VChangeD()
{
	disconnect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
	disconnect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
	if (keepImageDPIRatioButton->isChecked())
		imgDpiX->setValue(imgDpiY->value());
	NewLocalDpi();
	connect(imgDpiX, SIGNAL(valueChanged(double)), this, SLOT(HChangeD()));
	connect(imgDpiY, SIGNAL(valueChanged(double)), this, SLOT(VChangeD()));
}

void PropertiesPalette::NewAlignement(int a)
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetAlignment(a);
}

void PropertiesPalette::setTypeStyle(int s)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	emit NewEffects(s);
}

void PropertiesPalette::newShadowOffs()
{
	if ((HaveDoc) && (HaveItem))
	{
		int x = qRound(SeStyle->ShadowVal->Xoffset->value() * 10.0);
		int y = qRound(SeStyle->ShadowVal->Yoffset->value() * 10.0);
		doc->itemSelection_SetShadowOffsets(x, y);
	}
}

void PropertiesPalette::setShadowOffs(double x, double y)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	disconnect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowOffs()));
	disconnect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowOffs()));
	SeStyle->ShadowVal->Xoffset->setValue(x / 10.0);
	SeStyle->ShadowVal->Yoffset->setValue(y / 10.0);
	connect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowOffs()));
	connect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowOffs()));
}

void PropertiesPalette::newUnderline()
{
	if ((HaveDoc) && (HaveItem))
	{
		int x = qRound(SeStyle->UnderlineVal->LPos->value() * 10.0);
		int y = qRound(SeStyle->UnderlineVal->LWidth->value() * 10.0);
		doc->itemSelection_SetUnderline(x, y);
	}
}

void PropertiesPalette::setUnderline(double p, double w)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	disconnect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newUnderline()));
	disconnect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newUnderline()));
	SeStyle->UnderlineVal->LPos->setValue(p / 10.0);
	SeStyle->UnderlineVal->LWidth->setValue(w / 10.0);
	connect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newUnderline()));
	connect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newUnderline()));
}

void PropertiesPalette::newStrike()
{
	if ((HaveDoc) && (HaveItem))
	{
		int x = qRound(SeStyle->StrikeVal->LPos->value() * 10.0);
		int y = qRound(SeStyle->StrikeVal->LWidth->value() * 10.0);
		doc->itemSelection_SetStrikethru(x, y);
	}
}

void PropertiesPalette::setStrike(double p, double w)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	disconnect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newStrike()));
	disconnect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newStrike()));
	SeStyle->StrikeVal->LPos->setValue(p / 10.0);
	SeStyle->StrikeVal->LWidth->setValue(w / 10.0);
	connect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newStrike()));
	connect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newStrike()));
}

void PropertiesPalette::setOutlineW(double x)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	disconnect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newOutlineW()));
	SeStyle->OutlineVal->LWidth->setValue(x / 10.0);
	connect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newOutlineW()));
}

void PropertiesPalette::newOutlineW()
{
	int x = qRound(SeStyle->OutlineVal->LWidth->value() * 10.0);
	if ((HaveDoc) && (HaveItem))
		doc->itemSelection_SetOutlineWidth(x);
}

void PropertiesPalette::DoLower()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->view->LowerItem();
}

void PropertiesPalette::DoRaise()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->view->RaiseItem();
}

void PropertiesPalette::DoFront()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->view->ToFront();
}

void PropertiesPalette::DoBack()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->view->ToBack();
}

void PropertiesPalette::NewRotMode(int m)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	double inX, inY, gx, gy, gh, gw;
	inX = 0;
	inY = 0;
	if ((HaveDoc) && (HaveItem))
	{
		HaveItem = false;
		doc->RotMode = m;
		if (doc->m_Selection->isMultipleSelection())
		{
			doc->m_Selection->setGroupRect();
			doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			if (m == 0)
			{
				m_ScMW->view->RCenter = FPoint(gx, gy);
				inX = gx;
				inY = gy;
			}
			if (m == 1)
			{
				m_ScMW->view->RCenter = FPoint(gx+gw, gy);
				inX = gx+gw;
				inY = gy;
			}
			if (m == 2)
			{
				m_ScMW->view->RCenter = FPoint(gx + gw / 2.0, gy + gh / 2.0);
				inX = gx + gw / 2.0;
				inY = gy + gh / 2.0;
			}
			if (m == 3)
			{
				m_ScMW->view->RCenter = FPoint(gx, gy+gh);
				inX = gx;
				inY = gy+gh;
			}
			if (m == 4)
			{
				m_ScMW->view->RCenter = FPoint(gx+gw, gy+gh);
				inX = gx+gw;
				inY = gy+gh;
			}
			inX -= doc->rulerXoffset;
			inY -= doc->rulerYoffset;
			if (doc->guidesSettings.rulerMode)
			{
				inX -= doc->currentPage()->xOffset();
				inY -= doc->currentPage()->yOffset();
			}
			Xpos->setValue(inX*m_unitRatio);
			Ypos->setValue(inY*m_unitRatio);
		}
		else
		{
			double b, h, r;
			QMatrix ma;
			FPoint n;
			b = CurItem->width();
			h = CurItem->height();
			r = CurItem->rotation();
//			ma.translate(CurItem->xPos()-doc->getXOffsetForPage(CurItem->OwnPage), CurItem->yPos()-doc->getYOffsetForPage(CurItem->OwnPage));
			ma.translate(CurItem->xPos(), CurItem->yPos());
			ma.rotate(r);
			int bp = RotationGroup->checkedId();
			if (bp == 0)
				n = FPoint(0.0, 0.0);
			else if (bp == 1)
				n = FPoint(b, 0.0);
			else if (bp == 2)
				n = FPoint(b / 2.0, h / 2.0);
			else if (bp == 3)
				n = FPoint(0.0, h);
			else if (bp == 4)
				n = FPoint(b, h);
			inX = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
			inY = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
			inX -= doc->rulerXoffset;
			inY -= doc->rulerYoffset;
			if (doc->guidesSettings.rulerMode)
			{
				inX -= doc->currentPage()->xOffset();
				inY -= doc->currentPage()->yOffset();
			}
			Xpos->setValue(inX*m_unitRatio);
			Ypos->setValue(inY*m_unitRatio);
		}
		if (CurItem->itemType() == PageItem::ImageFrame)
		{
			if (!FreeScale->isChecked())
			{
				CurItem->AdjustPictScale();
				CurItem->update();
				disconnect(imageXOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));
				disconnect(imageYOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));
				imageXOffsetSpinBox->setValue(CurItem->imageXOffset() * CurItem->imageXScale() * m_unitRatio);
				imageYOffsetSpinBox->setValue(CurItem->imageYOffset() * CurItem->imageYScale() * m_unitRatio);
				connect(imageXOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));
				connect(imageYOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(NewLocalXY()));
				emit DocChanged();
			}
		}
		HaveItem = true;
	}
}

void PropertiesPalette::DoFlow()
{
	PageItem::TextFlowMode mode = PageItem::TextFlowDisabled;
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if (CurItem->isGroupControl)
		{
			if (textFlowDisabled2->isChecked())
				mode = PageItem::TextFlowDisabled;
			if (textFlowUsesFrameShape2->isChecked())
				mode = PageItem::TextFlowUsesFrameShape;
			if (textFlowUsesBoundingBox2->isChecked())
				mode = PageItem::TextFlowUsesBoundingBox;
			if (textFlowUsesContourLine2->isChecked())
				mode = PageItem::TextFlowUsesContourLine;
			if (textFlowUsesImageClipping2->isChecked())
				mode = PageItem::TextFlowUsesImageClipping;
		}
		else
		{
			if (textFlowDisabled->isChecked())
				mode = PageItem::TextFlowDisabled;
			if (textFlowUsesFrameShape->isChecked())
				mode = PageItem::TextFlowUsesFrameShape;
			if (textFlowUsesBoundingBox->isChecked())
				mode = PageItem::TextFlowUsesBoundingBox;
			if (textFlowUsesContourLine->isChecked())
				mode = PageItem::TextFlowUsesContourLine;
			if (textFlowUsesImageClipping->isChecked())
				mode = PageItem::TextFlowUsesImageClipping;
		}
		CurItem->setTextFlowMode(mode);
		m_ScMW->view->DrawNew();
		emit DocChanged();
	}
}

void PropertiesPalette::MakeIrre(int f, int c, double *vals)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->itemType() == PageItem::PolyLine) || (CurItem->itemType() == PageItem::PathText))
			return;
		switch (f)
		{
		case 0:
			CurItem->SetRectFrame();
			doc->setRedrawBounding(CurItem);
			break;
		case 1:
			CurItem->SetOvalFrame();
			doc->setRedrawBounding(CurItem);
			break;
		default:
			CurItem->SetFrameShape(c, vals);
			doc->setRedrawBounding(CurItem);
			CurItem->FrameType = f+2;
			break;
		}
//qt4		m_ScMW->SCustom->setPixmap(m_ScMW->SCustom->getIconPixmap(f));
		CurItem->update();
		emit DocChanged();
		if ((CurItem->itemType() == PageItem::ImageFrame) || (CurItem->itemType() == PageItem::TextFrame))
		{
			if (f != 0)
				RoundRect->setEnabled(false);
			else
				RoundRect->setEnabled(true);
			return;
		}
		CurItem->convertTo(PageItem::Polygon);
		NewSel(6);
		if (f != 0)
			RoundRect->setEnabled(false);
		else
			RoundRect->setEnabled(true);
// 		TabStack->setCurrentIndex(1);
	}
}

void PropertiesPalette::handleShapeEdit()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		tmpSelection->clear();
		m_ScMW->view->requestMode(modeEditClip);
		RoundRect->setEnabled(false);
	}
}

void PropertiesPalette::NewTDist()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setTextToFrameDist(DLeft->value() / m_unitRatio, DRight->value() / m_unitRatio, DTop->value() / m_unitRatio, DBottom->value() / m_unitRatio);
		setCols(CurItem->Cols, CurItem->ColGap);
		CurItem->update();
		emit DocChanged();
	}
}

void PropertiesPalette::NewSpGradient(double x1, double y1, double x2, double y2)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->GrStartX = x1 / m_unitRatio;
		CurItem->GrStartY = y1 / m_unitRatio;
		CurItem->GrEndX = x2 / m_unitRatio;
		CurItem->GrEndY = y2 / m_unitRatio;
		CurItem->update();
		QRectF upRect(QPointF(CurItem->GrStartX, CurItem->GrStartY), QPointF(CurItem->GrEndX, CurItem->GrEndY));
		upRect.translate(CurItem->xPos(), CurItem->yPos());
		doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
		emit DocChanged();
	}
}

void PropertiesPalette::toggleGradientEdit()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		if (Cpal->gradEditButton->isChecked())
			m_ScMW->view->requestMode(modeEditGradientVectors);
		else
			m_ScMW->view->requestMode(modeNormal);
		m_ScMW->view->RefreshGradient(CurItem);
	}
}

void PropertiesPalette::NewTFont(QString c)
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	emit NewFont(c);
}

void PropertiesPalette::DoRevert()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	bool setter=Revert->isChecked();
	CurItem->setImageFlippedH(setter);
	CurItem->setReversed(setter);
	CurItem->update();
	emit DocChanged();
}


void PropertiesPalette::doClearCStyle()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if (HaveDoc)
	{
		doc->itemSelection_EraseCharStyle();
	}
}


void PropertiesPalette::doClearPStyle()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if (HaveDoc)
	{
		doc->itemSelection_EraseParagraphStyle();
		CharStyle emptyCStyle;
		doc->itemSelection_SetCharStyle(emptyCStyle);
	}
}


void PropertiesPalette::SetLineFormats(ScribusDoc *dd)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	disconnect(StyledLine, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(SetSTline(QListWidgetItem*)));
	StyledLine->clear();
	if (dd != 0)
	{
		QMap<QString,multiLine>::Iterator it;
		for (it = dd->MLineStyles.begin(); it != dd->MLineStyles.end(); ++it)
			StyledLine->addItem( new LineFormatItem(dd, it.value(), it.key()) );
		StyledLine->sortItems();
		StyledLine->insertItem( 0, tr("No Style"));
		if (StyledLine->currentItem())
			StyledLine->currentItem()->setSelected(false);
	}
	connect(StyledLine, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(SetSTline(QListWidgetItem*)));
}

void PropertiesPalette::SetSTline(QListWidgetItem *c)
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if (c == NULL)
		return;
	bool setter = (c->listWidget()->currentRow() == 0);
	doc->itemSelection_SetNamedLineStyle(setter ? QString("") : c->text());
	LStyle->setEnabled(setter);
	LSize->setEnabled(setter);
	LJoinStyle->setEnabled(setter);
	LEndStyle->setEnabled(setter);
}

void PropertiesPalette::updateColorList()
{
	if (!HaveDoc || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	Cpal->SetColors(doc->PageColors);
	Cpal->SetPatterns(&doc->docPatterns);
	assert (doc->PageColors.document());
	TxFill->updateBox(doc->PageColors, ColorCombo::fancyPixmaps, true);
	TxStroke->updateBox(doc->PageColors, ColorCombo::fancyPixmaps, false);
	TxFill->view()->setMinimumWidth(TxFill->view()->maximumViewportSize().width() + 24);
	TxStroke->view()->setMinimumWidth(TxStroke->view()->maximumViewportSize().width() + 24);
}

void PropertiesPalette::updateCmsList()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if (HaveDoc)
	{
		if (ScCore->haveCMS() && doc->CMSSettings.CMSinUse)
			GroupBoxCM->show();
		else
		{
			GroupBoxCM->hide();
			return;
		}
		disconnect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChangeProfile(const QString&)));
		disconnect(MonitorI, SIGNAL(activated(int)), this, SLOT(ChangeIntent()));
		InputP->clear();
		if (HaveItem)
		{
			if (CurItem->pixm.imgInfo.colorspace == ColorSpaceCMYK)
			{
				ProfilesL::Iterator itP;
				ProfilesL::Iterator itPend=ScCore->InputProfilesCMYK.end();
				for (itP = ScCore->InputProfilesCMYK.begin(); itP != itPend; ++itP)
				{
					InputP->addItem(itP.key());
					if (itP.key() == CurItem->IProfile)
						InputP->setCurrentIndex(InputP->count()-1);
				}
				if (!ScCore->InputProfilesCMYK.contains(CurItem->IProfile))
				{
					InputP->addItem(CurItem->IProfile);
					InputP->setCurrentIndex(InputP->count()-1);
				}
				else
				{
					if (!CurItem->EmProfile.isEmpty())
						InputP->addItem(CurItem->EmProfile);
				}
			}
			else
			{
				ProfilesL::Iterator itP;
				ProfilesL::Iterator itPend=ScCore->InputProfiles.end();
				for (itP = ScCore->InputProfiles.begin(); itP != itPend; ++itP)
				{
					InputP->addItem(itP.key());
					if (itP.key() == CurItem->IProfile)
						InputP->setCurrentIndex(InputP->count()-1);
				}
				if (!ScCore->InputProfiles.contains(CurItem->IProfile))
				{
					InputP->addItem(CurItem->IProfile);
					InputP->setCurrentIndex(InputP->count()-1);
				}
				else
				{
					if (!CurItem->EmProfile.isEmpty())
						InputP->addItem(CurItem->EmProfile);
				}
			}
			MonitorI->setCurrentIndex(CurItem->IRender);
		}
		connect(MonitorI, SIGNAL(activated(int)), this, SLOT(ChangeIntent()));
		connect(InputP, SIGNAL(activated(const QString&)), this, SLOT(ChangeProfile(const QString&)));
	}
}

void PropertiesPalette::ChangeProfile(const QString& prn)
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetColorProfile(InputP->currentText());
}

void PropertiesPalette::ChangeIntent()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetRenderIntent(MonitorI->currentIndex());
}

void PropertiesPalette::ShowCMS()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if (HaveItem)
		updateCmsList();
	else
		GroupBoxCM->setVisible(ScCore->haveCMS() && doc->CMSSettings.CMSinUse);
}

void PropertiesPalette::newTxtFill()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetFillColor(TxFill->currentColor());
}

void PropertiesPalette::newTxtStroke()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetStrokeColor(TxStroke->currentColor());
}

void PropertiesPalette::setActShade()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	int b;
	if (PM1 == sender())
	{
		b = PM1->getValue();
		doc->itemSelection_SetStrokeShade(b);
	}
	else
	{
		b = PM2->getValue();
		doc->itemSelection_SetFillShade(b);
	}
}

void PropertiesPalette::setActFarben(QString p, QString b, double shp, double shb)
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	ColorList::Iterator it;
	int c = 0;
	PM2->setValue(qRound(shb));
	PM1->setValue(qRound(shp));
	if ((b != CommonStrings::None) && (!b.isEmpty()))
	{
		c++;
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			if (it.key() == b)
				break;
			c++;
		}
	}
	TxFill->setCurrentIndex(c);
	c = 0;
	if ((p != CommonStrings::None) && (!p.isEmpty()))
	{
//		c++;
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			if (it.key() == p)
				break;
			c++;
		}
	}
	TxStroke->setCurrentIndex(c);
}

void PropertiesPalette::handleLock()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->scrActions["itemLock"]->toggle();
}

void PropertiesPalette::handleLockSize()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->scrActions["itemLockSize"]->toggle();
}

void PropertiesPalette::handlePrint()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->scrActions["itemPrintingEnabled"]->toggle();
}

void PropertiesPalette::handleFlipH()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->scrActions["itemFlipH"]->toggle();
	/*
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->itemType() == PageItem::ImageFrame) || (CurItem->itemType() == PageItem::TextFrame))
			doc->FlipImageH();
		else
			doc->MirrorPolyH();
		emit DocChanged();
	}
	*/
}

void PropertiesPalette::handleFlipV()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	m_ScMW->scrActions["itemFlipV"]->toggle();
	/*
	if ((HaveDoc) && (HaveItem))
	{
		if ((CurItem->itemType() == PageItem::ImageFrame) || (CurItem->itemType() == PageItem::TextFrame))
			doc->FlipImageV();
		else
			doc->MirrorPolyV();
		emit DocChanged();
	}
	*/
}


void PropertiesPalette::handlePathType()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	CurItem->textPathType = pathTextType->currentIndex();
	CurItem->update();
	emit DocChanged();
}

void PropertiesPalette::handlePathFlip()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	CurItem->textPathFlipped = flippedPathText->isChecked();
	CurItem->updatePolyClip();
	CurItem->update();
	emit DocChanged();
}

void PropertiesPalette::handlePathLine()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	CurItem->PoShow = showcurveCheckBox->isChecked();
	CurItem->update();
	emit DocChanged();
}

void PropertiesPalette::handlePathDist()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	CurItem->setTextToFrameDistLeft(Dist->value());
	doc->AdjustItemSize(CurItem);
	CurItem->updatePolyClip();
	CurItem->update();
	emit DocChanged();
}

void PropertiesPalette::handlePathOffs()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	CurItem->BaseOffs = -LineW->value();
	doc->AdjustItemSize(CurItem);
	CurItem->updatePolyClip();
	CurItem->update();
	emit DocChanged();
}

void PropertiesPalette::handleFillRule()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	CurItem->fillRule = EvenOdd->isChecked();
	CurItem->update();
	emit DocChanged();
}

void PropertiesPalette::handleOverprint()
{
	if (!HaveDoc || !HaveItem || !m_ScMW || m_ScMW->ScriptRunning)
		return;
	doc->itemSelection_SetOverprint(Overprint->isChecked());
}

void PropertiesPalette::NewName()
{
	if (m_ScMW->ScriptRunning || !HaveDoc || !HaveItem)
		return;
	QString NameOld = CurItem->itemName();
	QString NameNew = NameEdit->text();
	if (NameNew.isEmpty())
	{
		NameEdit->setText(NameOld);
		return;
	}
	bool found = false;
	for (int b = 0; b < doc->Items->count(); ++b)
	{
		if ((NameNew == doc->Items->at(b)->itemName()) && (doc->Items->at(b) != CurItem))
		{
			found = true;
			break;
		}
	}
	if (found)
	{
		QMessageBox::warning(this, CommonStrings::trWarning, "<qt>"+ tr("Name \"%1\" isn't unique.<br/>Please choose another.").arg(NameNew)+"</qt>", CommonStrings::tr_OK);
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

void PropertiesPalette::fillLangCombo(QMap<QString,QString> langMap)
{
	QStringList sortList;
	QMap<QString,QString>::Iterator it;
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	langCombo->clear();
	for (it = langMap.begin(); it != langMap.end(); ++it)
		sortList.push_back(it.value());
	langCombo->addItems(sortQStringList(sortList));
	QListView *tmpView = dynamic_cast<QListView*>(langCombo->view()); Q_ASSERT(tmpView);
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
}

void PropertiesPalette::NewLanguage()
{
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->doc()->Language = (m_ScMW->GetLang(langCombo->currentText()));
		emit DocChanged();
	}
}

void PropertiesPalette::ManageTabs()
{
	if ((HaveDoc) && (HaveItem))
	{
		PageItem_TextFrame *i2=CurItem->asTextFrame();
		if (i2==0)
			return;
		TabManager *dia = new TabManager(this, doc->unitIndex(), i2->itemText.defaultStyle().tabValues(), i2->columnWidth());
		if (dia->exec())
		{
			ParagraphStyle newStyle(CurItem->itemText.defaultStyle());
			newStyle.setTabValues(dia->tmpTab);
			CurItem->itemText.setDefaultStyle(newStyle);
			CurItem->update();
			emit DocChanged();
		}
		delete dia;
	}
}

void PropertiesPalette::HandleTLines()
{
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->TopLine = TopLine->isChecked();
		CurItem->LeftLine = LeftLine->isChecked();
		CurItem->RightLine = RightLine->isChecked();
		CurItem->BottomLine = BottomLine->isChecked();
		CurItem->update();
		emit DocChanged();
	}
}

void PropertiesPalette::installSniffer(ScrSpinBox *spinBox)
{
	const QList<QObject*> list = spinBox->children();
	if (!list.isEmpty())
	{
		QListIterator<QObject*> it(list);
		QObject *obj;
		while (it.hasNext())
		{
			obj = it.next();
			obj->installEventFilter(userActionSniffer);
		}
	}
}

bool PropertiesPalette::userActionOn()
{
	return _userActionOn;
}

void PropertiesPalette::spinboxStartUserAction()
{
//	qDebug("sniffer: spinbox start action");
	_userActionOn = true;
}

void PropertiesPalette::spinboxFinishUserAction()
{
//	qDebug("sniffer: spinbox finish action");
	_userActionOn = false;

	for (int i = 0; i < doc->m_Selection->count(); ++i)
		doc->m_Selection->itemAt(i)->checkChanges(true);
	if (m_ScMW->view->groupTransactionStarted())
	{
		m_ScMW->view->endGroupTransaction();
	}
}

void PropertiesPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void PropertiesPalette::languageChange()
{
	setWindowTitle( tr("Properties"));

	TabStack->setItemText(idXYZItem, tr("X, Y, &Z"));
	TabStack->setItemText(idTextItem, tr("&Text"));
	TabStack->setItemText(idImageItem, tr("&Image"));
	TabStack->setItemText(idShapeItem, tr("&Shape"));
	TabStack->setItemText(idLineItem, tr("&Line"));
	TabStack->setItemText(idColorsItem, tr("&Colors"));
	TabStack->setItemText(idGroupItem, tr("&Group"));

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
//	EditShape->setText( tr("&Edit Shape..."));
//	EditShape2->setText( tr("&Edit Shape..."));
	EditShape->setText( tr("&Edit..."));
	EditShape2->setText( tr("&Edit..."));
	SRect2->setText( tr("Shape:"));
	TransGroup->setTitle( tr( "Transparency Settings" ));
	TransTxt->setText( tr( "Opacity:" ) );
	TransTxt2->setText( tr( "Blend Mode:" ) );
	blendMode->clear();
	blendMode->addItem( tr("Normal"));
	blendMode->addItem( tr("Darken"));
	blendMode->addItem( tr("Lighten"));
	blendMode->addItem( tr("Multiply"));
	blendMode->addItem( tr("Screen"));
	blendMode->addItem( tr("Overlay"));
	blendMode->addItem( tr("Hard Light"));
	blendMode->addItem( tr("Soft Light"));
	blendMode->addItem( tr("Difference"));
	blendMode->addItem( tr("Exclusion"));
	blendMode->addItem( tr("Color Dodge"));
	blendMode->addItem( tr("Color Burn"));
	blendMode->addItem( tr("Hue"));
	blendMode->addItem( tr("Saturation"));
	blendMode->addItem( tr("Color"));
	rndcornersLabel->setText( tr("R&ound\nCorners:"));
	columnsLabel->setText( tr("Colu&mns:"));
	int oldcolgapLabel = colgapLabel->currentIndex();
	colgapLabel->clear();
	colgapLabel->addItem( tr("Gap:"));
	colgapLabel->addItem( tr("Width:"));
	colgapLabel->setCurrentIndex(oldcolgapLabel);
//	colgapLabel->setTexts( tr("&Gap:"), tr("&Width:"));
	topLabel->setText( tr("To&p:"));
	bottomLabel->setText( tr("&Bottom:"));
	leftLabel->setText( tr("&Left:"));
	rightLabel->setText( tr("&Right:"));
	TabsButton->setText( tr("T&abulators..."));
//	Distance2->setTitle( tr("Path Text Properties"));
	pathTextType->clear();
	pathTextType->addItem( tr("Default"));
	pathTextType->addItem( tr("Stair Step"));
	pathTextType->addItem( tr("Skew"));
	flippedPathText->setText( tr("Flip Text"));
	showcurveCheckBox->setText( tr("Show Curve"));
	pathTextTypeLabel->setText( tr("Type:"));
	startoffsetLabel->setText( tr("Start Offset:"));
	distfromcurveLabel->setText( tr("Distance from Curve:"));
	Distance3->setTitle( tr("Fill Rule"));
	EvenOdd->setText( tr("Even-Odd"));
	NonZero->setText( tr("Non Zero"));
	textFlowOptions->setTitle( tr("Text &Flow Around Frame"));
	textFlowDisabled->setText( tr("Disabled"));
	textFlowUsesFrameShape->setText( tr("Use Frame &Shape"));
	textFlowUsesBoundingBox->setText( tr("Use &Bounding Box"));
	textFlowUsesContourLine->setText( tr("&Use Contour Line"));
	textFlowUsesImageClipping->setText( tr("Use Image Clip Path"));
	textFlowOptions2->setTitle( tr("Text &Flow Around Frame"));
	textFlowDisabled2->setText( tr("Disabled"));
	textFlowUsesFrameShape2->setText( tr("Use Frame &Shape"));
	textFlowUsesBoundingBox2->setText( tr("Use &Bounding Box"));
	textFlowUsesContourLine2->setText( tr("&Use Contour Line"));
	textFlowUsesImageClipping2->setText( tr("Use Image Clip Path"));
	paraStyleLabel->setText( tr("Paragraph St&yle:"));
	charStyleLabel->setText( tr("Character St&yle:"));
	optMarginLabel->setText( tr("Optical Margins:"));
	int c=optMarginCombo->currentIndex();
	optMarginCombo->clear();
	optMarginCombo->addItem( CommonStrings::trOpticalMarginsNone );
//	Out for 1.3.4
// 	optMarginCombo->insertItem( CommonStrings::trOpticalMarginsLeftProtruding );
// 	optMarginCombo->insertItem( CommonStrings::trOpticalMarginsRightProtruding );
// 	optMarginCombo->insertItem( CommonStrings::trOpticalMarginsLeftHangPunct );
// 	optMarginCombo->insertItem( CommonStrings::trOpticalMarginsRightHangPunct );
	optMarginCombo->addItem( CommonStrings::trOpticalMarginsDefault );
	optMarginCombo->setCurrentIndex(c);
	wordTrackingLabel->setText( tr("Word Tracking"));
	minWordTrackingLabel->setText( tr("Min:"));
	normWordTrackingLabel->setText( tr("Norm:"));
	glyphExtensionLabel->setText( tr("Glyph Extension"));
	minGlyphExtensionLabel->setText( tr("Min:"));
	maxGlyphExtensionLabel->setText( tr("Max:"));
	colorWidgetsItem->setText(0, tr("Color & Effects"));
	advancedWidgetsItem->setText(0, tr("Advanced Settings"));
	styleWidgetsItem->setText(0, tr("Style Settings"));
	flopItem->setText(0, tr("First Line Offset"));
    DistanceItem->setText(0, tr("Columns & Text Distances"));
	Distance2Item->setText(0, tr("Path Text Properties"));
	imagePageNumberLabel->setText( tr("&Page Number:"));
	FreeScale->setText( tr("&Free Scaling"));
	imgDPIXLabel->setText( tr("Actual X-DPI:"));
	imgDPIYLabel->setText( tr("Actual Y-DPI:"));
	xposImgLabel->setText( tr("&X-Pos:"));
	yposImgLabel->setText( tr("&Y-Pos:"));
	xscaleLabel->setText( tr("X-Sc&ale:"));
	yscaleLabel->setText( tr("Y-Scal&e:"));
	FrameScale->setText( tr("Scale &To Frame Size"));
	Aspect->setText( tr("P&roportional"));
	EditEffects->setText( tr("Image Effects"));
	EditPSDProps->setText( tr("Extended Image Properties"));
	TextCms1->setText( tr("Input Profile:"));
	TextCms2->setText( tr("Rendering Intent:"));
// 	QList<QAction*> actList = lineSpacingPop->actions();
// 	actList[0]->setText( tr("Fixed Linespacing"));
// 	actList[1]->setText( tr("Automatic Linespacing"));
// 	actList[2]->setText( tr("Align to Baseline Grid"));
	lineSpacingModeCombo->addItem( tr("Fixed Linespacing"));
	lineSpacingModeCombo->addItem( tr("Automatic Linespacing"));
	lineSpacingModeCombo->addItem( tr("Align to Baseline Grid"));
	int oldMonitorI=MonitorI->currentIndex();
	MonitorI->clear();
	MonitorI->addItem( tr("Perceptual"));
	MonitorI->addItem( tr("Relative Colorimetric"));
	MonitorI->addItem( tr("Saturation"));
	MonitorI->addItem( tr("Absolute Colorimetric"));
	MonitorI->setCurrentIndex(oldMonitorI);
	int oldLineStyle = LStyle->currentIndex();
	LStyle->clear();
	LStyle->updateList();
	LStyle->addItem( tr("Custom"));
	LStyle->setCurrentIndex(oldLineStyle);
	int oldLineMode=LineMode->currentIndex();
	LineMode->clear();
	LineMode->addItem( tr("Left Point"));
	LineMode->addItem( tr("End Points"));
	LineMode->setCurrentIndex(oldLineMode);
	LineModeT->setText( tr("&Basepoint:"));
	linetypeLabel->setText( tr("T&ype of Line:"));
	startArrowText->setText( tr("Start Arrow:"));
	endArrowText->setText( tr("End Arrow:"));
	if (HaveDoc)
	{
		int arrowItem=startArrow->currentIndex();
		startArrow->rebuildList(&doc->arrowStyles);
		startArrow->setCurrentIndex(arrowItem);
		arrowItem=endArrow->currentIndex();
		endArrow->rebuildList(&doc->arrowStyles);
		endArrow->setCurrentIndex(arrowItem);
	}
	linewidthLabel->setText( tr("Line &Width:"));
	edgesLabel->setText( tr("Ed&ges:"));
	int oldLJoinStyle=LJoinStyle->currentIndex();
	LJoinStyle->clear();
	LJoinStyle->addItem(loadIcon("16/stroke-join-miter.png"), tr("Miter Join"));
	LJoinStyle->addItem(loadIcon("16/stroke-join-bevel.png"), tr("Bevel Join"));
	LJoinStyle->addItem(loadIcon("16/stroke-join-round.png"), tr("Round Join"));
	LJoinStyle->setCurrentIndex(oldLJoinStyle);

	int oldLEndStyle=LEndStyle->currentIndex();
	LEndStyle->clear();
	LEndStyle->addItem(loadIcon("16/stroke-cap-butt.png"), tr("Flat Cap"));
	LEndStyle->addItem(loadIcon("16/stroke-cap-square.png"), tr("Square Cap"));
	LEndStyle->addItem(loadIcon("16/stroke-cap-round.png"), tr("Round Cap"));
	LEndStyle->setCurrentIndex(oldLEndStyle);
	endingsLabel->setText( tr("&Endings:"));

	TLines->setTitle( tr("Cell Lines"));
	TopLine->setText( tr("Line at Top"));
	LeftLine->setText( tr("Line at the Left"));
	RightLine->setText( tr("Line at the Right "));
	BottomLine->setText( tr("Line at Bottom"));
	
	OverP->setTitle( tr("Overprinting"));
	KnockOut->setText( tr("Knockout"));
	Overprint->setText( tr("Overprint"));

	QString pctSuffix=tr(" %");
	ChBase->setSuffix(pctSuffix);
	ChScale->setSuffix(pctSuffix);
	ChScaleV->setSuffix(pctSuffix);
	imageXScaleSpinBox->setSuffix(pctSuffix);
	imageYScaleSpinBox->setSuffix(pctSuffix);
	imgDpiX->setSuffix("");
	imgDpiY->setSuffix("");
	Extra->setSuffix(pctSuffix);
	minWordTrackingSpinBox->setSuffix(pctSuffix);
	normWordTrackingSpinBox->setSuffix(pctSuffix);
	minGlyphExtSpinBox->setSuffix(pctSuffix);
	maxGlyphExtSpinBox->setSuffix(pctSuffix);
	
	QString ptSuffix = tr(" pt");
	Dist->setSuffix(ptSuffix);
	LineW->setSuffix(ptSuffix);
	Size->setSuffix(ptSuffix);
	LineSp->setSuffix(ptSuffix);

	QString ein = (HaveDoc) ? unitGetSuffixFromIndex(doc->unitIndex()) : ptSuffix;

	LSize->setSuffix(ein);
	Xpos->setSuffix(ein);
	Ypos->setSuffix(ein);
	Width->setSuffix(ein);
	Height->setSuffix(ein);
	imageXOffsetSpinBox->setSuffix(ein);
	imageYOffsetSpinBox->setSuffix(ein);
	dGap->setSuffix(ein);
	DLeft->setSuffix(ein);
	DTop->setSuffix(ein);
	DBottom->setSuffix(ein);
	DRight->setSuffix(ein);
	RoundRect->setSuffix(ein);

	SeStyle->languageChange();
	GroupAlign->languageChange();

	if(StyledLine->count() > 0)
		StyledLine->item(0)->setText( tr("No Style") );
// 	updateCList();
// 	updateCmsList();
/* CB: doesnt look like we need to do this on languageChange anymore with qt4
	NameEdit->setToolTip("");
	Xpos->setToolTip("");
	Ypos->setToolTip("");
	Width->setToolTip("");
	Height->setToolTip("");
	Rotation->setToolTip("");
	basepointLabel->setToolTip("");
//	TopLeft->setToolTip("");
//	TopRight->setToolTip("");
//	BottomLeft->setToolTip("");
//	BottomRight->setToolTip("");
//	Center->setToolTip("");
	FlipH->setToolTip("");
	FlipV->setToolTip("");
	Zup->setToolTip("");
	ZDown->setToolTip("");
	ZTop->setToolTip("");
	ZBottom->setToolTip("");
	LevelTxt->setToolTip("");
	Locked->setToolTip("");
	NoResize->setToolTip("");
	NoPrint->setToolTip("");
	textFlowOptions->setToolTip("");
	textFlowDisabled->setToolTip("");
	textFlowUsesFrameShape->setToolTip("");
	textFlowUsesBoundingBox->setToolTip("");
	textFlowUsesContourLine->setToolTip("");
	textFlowUsesImageClipping->setToolTip("");
	textFlowOptions2->setToolTip("");
	textFlowDisabled2->setToolTip("");
	textFlowUsesFrameShape2->setToolTip("");
	textFlowUsesBoundingBox2->setToolTip("");
	textFlowUsesContourLine2->setToolTip("");
	textFlowUsesImageClipping2->setToolTip("");

	Fonts->setToolTip("");
	Size->setToolTip("");
	ChBase->setToolTip("");
	ChScale->setToolTip("");
	ChScaleV->setToolTip("");
	TxStroke->setToolTip("");
	TxFill->setToolTip("");
	PM1->setToolTip("");
	PM2->setToolTip("");
	Revert->setToolTip("");
	Extra->setToolTip("");
	LineSp->setToolTip("");
	linespacingButton->setToolTip("");
	paraStyleCombo->setToolTip("");
	charStyleCombo->setToolTip("");
	paraStyleClear->setToolTip("");
	charStyleClear->setToolTip("");
//	langCombo->setToolTip("");

	minWordTrackingSpinBox->setToolTip("");
	normWordTrackingSpinBox->setToolTip("");
	minGlyphExtSpinBox->setToolTip("");
	maxGlyphExtSpinBox->setToolTip("");
	optMarginCombo->setToolTip("");

	LineMode->setToolTip("");
	LStyle->setToolTip("");
	LSize->setToolTip("");
	LJoinStyle->setToolTip("");
	LEndStyle->setToolTip("");
	StyledLine->setToolTip("");

	SCustom->setToolTip("");
	EditShape->setToolTip("");
	RoundRect->setToolTip("");
	DCol->setToolTip("");
	colgapLabel->setToolTip("");
	dGap->setToolTip("");
	DTop->setToolTip("");
	DBottom->setToolTip("");
	DLeft->setToolTip("");
	DRight->setToolTip("");
	TabsButton->setToolTip("");

	FreeScale->setToolTip("");
	imageXOffsetSpinBox->setToolTip("");
	imageYOffsetSpinBox->setToolTip("");
	imageXScaleSpinBox->setToolTip("");
	imageYScaleSpinBox->setToolTip("");
	keepImageWHRatioButton->setToolTip("");
	keepFrameWHRatioButton->setToolTip("");
	FrameScale->setToolTip("");
	Aspect->setToolTip("");
	InputP->setToolTip("");
	MonitorI->setToolTip("");
*/
	NameEdit->setToolTip( tr("Name of selected object"));
	Xpos->setToolTip( tr("Horizontal position of current basepoint"));
	Ypos->setToolTip( tr("Vertical position of current basepoint"));
	Width->setToolTip( tr("Width"));
	Height->setToolTip( tr("Height"));
	Rotation->setToolTip( tr("Rotation of object at current basepoint"));
	basepointLabel->setToolTip( tr("Point from which measurements or rotation angles are referenced"));
//	TopLeft->setToolTip( tr("Select top left for basepoint"));
//	TopRight->setToolTip( tr("Select top right for basepoint"));
//	BottomLeft->setToolTip( tr("Select bottom left for basepoint"));
//	BottomRight->setToolTip( tr("Select bottom right for basepoint"));
//	Center->setToolTip( tr("Select center for basepoint"));
	DoGroup->setToolTip( tr("Group the selected objects"));
	DoUnGroup->setToolTip( tr("Ungroup the selected group"));
	FlipH->setToolTip( tr("Flip Horizontal"));
	FlipV->setToolTip( tr("Flip Vertical"));
	Zup->setToolTip( tr("Move one level up"));
	ZDown->setToolTip( tr("Move one level down"));
	ZTop->setToolTip( tr("Move to front"));
	ZBottom->setToolTip( tr("Move to back"));
	LevelTxt->setToolTip( tr("Indicates the level the object is on, 0 means the object is at the bottom"));
	Locked->setToolTip( tr("Lock or unlock the object"));
	NoResize->setToolTip( tr("Lock or unlock the size of the object"));
	NoPrint->setToolTip( tr("Enable or disable exporting of the object"));
	/*textFlowOptions->setToolTip( tr("Make text in lower frames flow around the object shape")); */
	textFlowDisabled->setToolTip( tr("Disable text flow from lower frames around object"));
	textFlowUsesFrameShape->setToolTip( tr("Use the frame shape for text flow of text frames below the object."));
	textFlowUsesBoundingBox->setToolTip(  "<qt>" + tr("Use the bounding box, which is always rectangular, instead of the frame's shape for text flow of text frames below the object. ") + "</qt>" );
	textFlowUsesContourLine->setToolTip(  "<qt>" + tr("When chosen, the contour line can be edited with the Edit Shape Tool on the palette further above. When edited via the shape palette, this becomes a second separate line originally based on the frame's shape for text flow of text frames below the object. T") + "</qt>" );
	textFlowUsesImageClipping->setToolTip(  "<qt>" + tr("Use the clipping path of the image") + "</qt>" );
	textFlowDisabled2->setToolTip( tr("Disable text flow from lower frames around object"));
	textFlowUsesFrameShape2->setToolTip( tr("Use the frame shape for text flow of text frames below the object."));
	textFlowUsesBoundingBox2->setToolTip(  "<qt>" + tr("Use the bounding box, which is always rectangular, instead of the frame's shape for text flow of text frames below the object. ") + "</qt>" );
	textFlowUsesContourLine2->setToolTip(  "<qt>" + tr("When chosen, the contour line can be edited with the Edit Shape Tool on the palette further above. When edited via the shape palette, this becomes a second separate line originally based on the frame's shape for text flow of text frames below the object. T") + "</qt>" );
	textFlowUsesImageClipping2->setToolTip(  "<qt>" + tr("Use the clipping path of the image") + "</qt>" );

	Fonts->setToolTip( tr("Font of selected text or object"));
	Size->setToolTip( tr("Font Size"));
	ChBase->setToolTip( tr("Offset to baseline of characters"));
	ChScale->setToolTip( tr("Scaling width of characters"));
	ChScaleV->setToolTip( tr("Scaling height of characters"));
	TxStroke->setToolTip( "<qt>" + tr("Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.") + "</qt>" );
	TxFill->setToolTip( "<qt>" + tr("Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.") + "</qt>" );
	PM1->setToolTip( tr("Saturation of color of text stroke"));
	PM2->setToolTip( tr("Saturation of color of text fill"));
	Revert->setToolTip( tr("Right to Left Writing"));
	Extra->setToolTip( tr("Manual Tracking"));
	LineSp->setToolTip( tr("Line Spacing"));
// 	linespacingButton->setToolTip( "<qt>" + tr("Click and hold down to select the line spacing mode.") + "</qt>" );
	lineSpacingModeCombo->setToolTip( tr("Select the line spacing mode.") );
	paraStyleCombo->setToolTip( tr("Paragraph style of currently selected text or paragraph"));
	charStyleCombo->setToolTip( tr("Character style of currently selected text or paragraph"));
	paraStyleClear->setToolTip( tr("Remove Direct Paragraph Formatting"));
	charStyleClear->setToolTip( tr("Remove Direct Character Formatting"));
//	langCombo->setToolTip( tr("Hyphenation language of frame"));

	flopRealHeight->setToolTip( "<qt>" + tr("Set the height of the first line of the text frame to use the tallest height of the included characters") + "</qt>" );
	flopFontAscent->setToolTip( "<qt>" + tr("Set the height of the first line of text frame to use the full ascent of the font(s) in use") + "</qt>" );
	flopLineSpacing->setToolTip( "<qt>" + tr("Set the height of the first line of the text frame to the specified line height") + "</qt>" );

	minWordTrackingSpinBox->setToolTip( tr("Minimal width of spaces between words"));
	normWordTrackingSpinBox->setToolTip( tr("Normal width of spaces between words"));
	minGlyphExtSpinBox->setToolTip( tr("Minimal shrinkage of glyphs for justification"));
	maxGlyphExtSpinBox->setToolTip( tr("Maximal extension of glyphs for justification"));
	optMarginCombo->setToolTip( tr("Uses hanging punctuation and margin kerning to achieve nicer looking columns"));

	LineMode->setToolTip( tr("Change settings for left or end points"));
	LStyle->setToolTip( tr("Pattern of line"));
	LSize->setToolTip( tr("Thickness of line"));
	LJoinStyle->setToolTip( tr("Type of line joins"));
	LEndStyle->setToolTip( tr("Type of line end"));
	StyledLine->setToolTip( tr("Line style of current object"));

	SCustom->setToolTip( tr("Choose the shape of frame..."));
	EditShape->setToolTip( tr("Edit shape of the frame..."));
	RoundRect->setToolTip( tr("Set radius of corner rounding"));
	DCol->setToolTip( tr("Number of columns in text frame"));
	colgapLabel->setToolTip( tr("Switches between Gap or Column width"));
	dGap->setToolTip( tr("Distance between columns"));
	DTop->setToolTip( tr("Distance of text from top of frame"));
	DBottom->setToolTip( tr("Distance of text from bottom of frame"));
	DLeft->setToolTip( tr("Distance of text from left of frame"));
	DRight->setToolTip( tr("Distance of text from right of frame"));
	TabsButton->setToolTip( tr("Edit tab settings of text frame..."));

	FreeScale->setToolTip( tr("Allow the image to be a different size to the frame"));
	imageXOffsetSpinBox->setToolTip( tr("Horizontal offset of image within frame"));
	imageYOffsetSpinBox->setToolTip( tr("Vertical offset of image within frame"));
	imageXScaleSpinBox->setToolTip( tr("Resize the image horizontally"));
	imageYScaleSpinBox->setToolTip( tr("Resize the image vertically"));
	keepImageWHRatioButton->setToolTip( tr("Keep the X and Y scaling the same"));
	keepFrameWHRatioButton->setToolTip( tr("Keep the aspect ratio"));
	FrameScale->setToolTip( tr("Make the image fit within the size of the frame"));
	Aspect->setToolTip( tr("Use image proportions rather than those of the frame"));
	InputP->setToolTip( tr("Source profile of the image"));
	MonitorI->setToolTip( tr("Rendering intent for the image"));
}


const VGradient PropertiesPalette::getFillGradient()
{
	return Cpal->gradEdit->Preview->fill_gradient;
}

void PropertiesPalette::setGradientEditMode(bool on)
{
	Cpal->gradEditButton->setChecked(on);
}

void PropertiesPalette::updateColorSpecialGradient()
{
	if (!HaveDoc)
		return;
	if(doc->m_Selection->isEmpty())
		return;
	double dur=doc->unitRatio();
	PageItem *currItem=doc->m_Selection->itemAt(0);
	if (currItem)
		Cpal->setSpecialGradient(currItem->GrStartX * dur, currItem->GrStartY * dur, currItem->GrEndX * dur, currItem->GrEndY * dur);
}

void PropertiesPalette::updateSpinBoxConstants()
{
	if (!HaveDoc)
		return;
	if(doc->m_Selection->count()==0)
		return;
/*qt4
	Width->setConstants(doc->constants());
	Height->setConstants(doc->constants());
	Xpos->setConstants(doc->constants());
	Ypos->setConstants(doc->constants());
*/
}

UserActionSniffer::UserActionSniffer(QObject* parent) : QObject (parent)
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
		if (k && !k->isAutoRepeat() && (k->key() == Qt::Key_Up || k->key() == Qt::Key_Down))
			emit actionStart();
	}
	else if (e->type() == QEvent::KeyRelease)
	{
		QKeyEvent *k = dynamic_cast<QKeyEvent*>(e);
		if (k && !k->isAutoRepeat() && (k->key() == Qt::Key_Up || k->key() == Qt::Key_Down))
			emit actionEnd();
	}
	return false;
}

void PropertiesPalette::setLocked(bool isLocked)
{
	Xpos->setReadOnly(isLocked);
	Ypos->setReadOnly(isLocked);
	Width->setReadOnly(isLocked);
	Height->setReadOnly(isLocked);
	Rotation->setReadOnly(isLocked);
	QPalette pal(qApp->palette());
	if (isLocked)
		pal.setCurrentColorGroup(QPalette::Disabled);

	Xpos->setPalette(pal);
	Ypos->setPalette(pal);
	Width->setPalette(pal);
	Height->setPalette(pal);
	Rotation->setPalette(pal);

	EditShape->setEnabled(!isLocked);
	LayerGroup->setEnabled(!isLocked);
	Locked->setChecked(isLocked);
	if ((HaveDoc) && (HaveItem))
	{
		SCustom->setEnabled(!CurItem->asLine() && !CurItem->asPolyLine() && !CurItem->asPathText() && !isLocked);
		if (((CurItem->asTextFrame()) || (CurItem->asImageFrame()) || (CurItem->asPolygon())) &&  (!CurItem->ClipEdited) && ((CurItem->FrameType == 0) || (CurItem->FrameType == 2)))
			RoundRect->setEnabled(!isLocked);
		else
			RoundRect->setEnabled(false);
	}
}

void PropertiesPalette::setSizeLocked(bool isSizeLocked)
{
	bool b=isSizeLocked;
	if (HaveItem && CurItem->locked())
		b=true;
	Width->setReadOnly(b);
	Height->setReadOnly(b);
	QPalette pal(qApp->palette());
	
	if (b)
		pal.setCurrentColorGroup(QPalette::Disabled);

	Width->setPalette(pal);
	Height->setPalette(pal);
	NoResize->setChecked(isSizeLocked);
}

void PropertiesPalette::setPrintingEnabled(bool isPrintingEnabled)
{
	NoPrint->setChecked(!isPrintingEnabled);
}

void PropertiesPalette::setFlippedH(bool isFlippedH)
{
	FlipH->setChecked(isFlippedH);
}

void PropertiesPalette::setFlippedV(bool isFlippedV)
{
	FlipV->setChecked(isFlippedV);
}

void PropertiesPalette::setGroupTransparency(int trans)
{
	if ((HaveDoc) && (HaveItem))
	{
		CurItem->setFillTransparency(static_cast<double>(100 - trans) / 100.0);
		CurItem->update();
		emit DocChanged();
	}
}

void PropertiesPalette::setGroupBlending(int blend)
{
		CurItem->setFillBlendmode(blend);
		CurItem->update();
		emit DocChanged();
}

void PropertiesPalette::doGrouping()
{
	m_ScMW->GroupObj();
	DoGroup->setEnabled(false);
	DoUnGroup->setEnabled(true);
	setMultipleSelection(true);
	double gx, gy, gh, gw;
	doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
	int bp = RotationGroup->checkedId();
	if (bp == 0)
		m_ScMW->view->RCenter = FPoint(gx, gy);
	else if (bp == 1)
		m_ScMW->view->RCenter = FPoint(gx + gw, gy);
	else if (bp == 2)
		m_ScMW->view->RCenter = FPoint(gx + gw / 2.0, gy + gh / 2.0);
	else if (bp == 3)
		m_ScMW->view->RCenter = FPoint(gx, gy + gh);
	else if (bp == 4)
		m_ScMW->view->RCenter = FPoint(gx + gw, gy + gh);
	TabStack->setItemEnabled(idShapeItem, false);
}

void PropertiesPalette::handleShapeEdit2()
{
	if (!m_ScMW || m_ScMW->ScriptRunning)
		return;
	if ((HaveDoc) && (HaveItem))
	{
		tmpSelection->clear();
		tmpSelection->copy(*doc->m_Selection, true);
		doc->m_Selection->clear();
		doc->m_Selection->addItem(CurItem);
		CurItem->isSingleSel = true;
		CurItem->update();
		m_ScMW->view->requestMode(modeEditClip);
		RoundRect->setEnabled(false);
	}
}

void PropertiesPalette::endEdit2()
{
	if ((HaveDoc) && (HaveItem))
	{
		if (tmpSelection->count() > 0)
		{
			doc->m_Selection->copy(*tmpSelection, false);
			doc->m_Selection->connectItemToGUI();
		}
		tmpSelection->clear();
	}
}

void PropertiesPalette::flop(int radioFlop)
{
	if (!m_ScMW || m_ScMW->ScriptRunning || !HaveDoc || !HaveItem)
		return;
// 	qDebug(QString("rF %1").arg(radioFlop).toAscii());
	if( radioFlop == 0)
		CurItem->setFirstLineOffset(FLOPRealGlyphHeight);
	else if( radioFlop == 1)
		CurItem->setFirstLineOffset(FLOPFontAscent);
	else if( radioFlop == 2)
		CurItem->setFirstLineOffset(FLOPLineSpacing);
	CurItem->update();
	emit DocChanged();
}

