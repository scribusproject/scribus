#include "tabguides.h"
#include "tabguides.moc"
#include <qlayout.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qcolor.h>
#include <qcolordialog.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include "mspinbox.h"
#include "scribusstructs.h"

TabGuides::TabGuides( QWidget* parent, struct guidesPrefs *prefsData, double unitBase, QString unit) : QWidget( parent, "tabguide", 0 )
{
	tabGuidesLayout = new QVBoxLayout( this, 10, 5, "tabViewLayout");
	checkGrid = new QGroupBox( this, "checkGrid" );
	checkGrid->setTitle( tr( "Show Grid" ) );
	checkGrid->setCheckable( true );
	checkGrid->setChecked(prefsData->gridShown);
	checkGrid->setColumnLayout(0, Qt::Vertical );
	checkGrid->layout()->setSpacing( 5 );
	checkGrid->layout()->setMargin( 10 );
	checkGridLayout = new QGridLayout( checkGrid->layout() );
	checkGridLayout->setAlignment( Qt::AlignTop );
	groupBox1 = new QGroupBox( checkGrid, "groupBox1" );
	groupBox1->setColumnLayout(0, Qt::Vertical );
	groupBox1->layout()->setSpacing( 5 );
	groupBox1->layout()->setMargin( 10 );
	groupBox1->setTitle( tr( "Major Grid" ) );
	groupBox1Layout = new QGridLayout( groupBox1->layout() );
	groupBox1Layout->setAlignment( Qt::AlignTop );
	majorGridColor = new QPushButton( groupBox1, "majorGridColor" );
	majorGridColor->setMinimumSize( QSize( 60, 20 ) );
	majorGridColor->setMaximumSize( QSize( 60, 20 ) );
	majorGridColor->setFlat( false );
	majorGridColor->setAutoDefault( false );
	QPixmap pm1 = QPixmap(54, 14);
	pm1.fill(prefsData->majorColor);
	colorMajorGrid = prefsData->majorColor;
	majorGridColor->setPixmap(pm1);
	majorGridColor->setText( QString::null );
	groupBox1Layout->addWidget( majorGridColor, 1, 1 );
	textLabel4 = new QLabel( groupBox1, "textLabel4" );
	textLabel4->setText( tr( "Color:" ) );
	groupBox1Layout->addWidget( textLabel4, 1, 0 );
	textLabel6 = new QLabel( groupBox1, "textLabel6" );
	textLabel6->setText( tr( "Spacing:" ) );
	groupBox1Layout->addWidget( textLabel6, 0, 0 );
	majorSpace = new MSpinBox( 10 * unitBase, 1000 * unitBase, groupBox1, 2 );
	majorSpace->setValue( prefsData->majorGrid * unitBase );
	majorSpace->setSuffix( unit );
	groupBox1Layout->addWidget( majorSpace, 0, 1 );
	checkGridLayout->addWidget( groupBox1, 0, 0 );
	groupBox2 = new QGroupBox( checkGrid, "groupBox2" );
	groupBox2->setColumnLayout(0, Qt::Vertical );
	groupBox2->layout()->setSpacing( 5 );
	groupBox2->layout()->setMargin( 10 );
	groupBox2->setTitle( tr( "Minor Grid" ) );
	groupBox2Layout = new QGridLayout( groupBox2->layout() );
	groupBox2Layout->setAlignment( Qt::AlignTop );
	textLabel5 = new QLabel( groupBox2, "textLabel5" );
	textLabel5->setText( tr( "Color:" ) );
	groupBox2Layout->addWidget( textLabel5, 1, 0 );
	minorGridColor = new QPushButton( groupBox2, "minorGridColor" );
	minorGridColor->setMinimumSize( QSize( 60, 20 ) );
	minorGridColor->setMaximumSize( QSize( 60, 20 ) );
	minorGridColor->setFlat( false );
	minorGridColor->setAutoDefault( false );
	QPixmap pm = QPixmap(54, 14);
	pm.fill(prefsData->minorColor);
	colorMinorGrid = prefsData->minorColor;
	minorGridColor->setPixmap(pm);
	minorGridColor->setText( QString::null );
	groupBox2Layout->addWidget( minorGridColor, 1, 1 );
	textLabel7 = new QLabel( groupBox2, "textLabel7" );
	textLabel7->setText( tr( "Spacing:" ) );
	groupBox2Layout->addWidget( textLabel7, 0, 0 );
	minorSpace = new MSpinBox( unitBase, 1000 * unitBase, groupBox2, 2 );
	minorSpace->setValue( prefsData->minorGrid  * unitBase);
	minorSpace->setSuffix( unit );
	groupBox2Layout->addWidget( minorSpace, 0, 1 );
	checkGridLayout->addWidget( groupBox2, 0, 1 );
	tabGuidesLayout->addWidget( checkGrid );
	buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
	buttonGroup1->setExclusive( true );
	buttonGroup1->setColumnLayout(0, Qt::Vertical );
	buttonGroup1->layout()->setSpacing( 5 );
	buttonGroup1->layout()->setMargin( 10 );
	buttonGroup1->setTitle( tr( "Placing of Guides" ) );
	buttonGroup1Layout = new QHBoxLayout( buttonGroup1->layout() );
	buttonGroup1Layout->setAlignment( Qt::AlignTop );
	inBackground = new QRadioButton( buttonGroup1, "inBackground" );
	inBackground->setText( tr( "In the Background" ) );
	buttonGroup1Layout->addWidget( inBackground );
	inForeground = new QRadioButton( buttonGroup1, "inForeground" );
	inForeground->setText( tr( "In the Foreground" ) );
	buttonGroup1Layout->addWidget( inForeground );
	inBackground->setChecked( prefsData->before );
	inForeground->setChecked( !prefsData->before );
	tabGuidesLayout->addWidget( buttonGroup1 );
	layout11 = new QGridLayout( 0, 1, 1, 0, 5, "layout11");
	textLabel8 = new QLabel( this, "textLabel8" );
	textLabel8->setText( tr( "Guide Snap Distance:" ) );
	layout11->addWidget( textLabel8, 0, 0 );
	snapDistance = new MSpinBox( unitBase, 1000 * unitBase, this, 2 );
	snapDistance->setValue( prefsData->guideRad * unitBase );
	snapDistance->setSuffix( unit );
	layout11->addWidget( snapDistance, 0, 1, Qt::AlignLeft );
	textLabel82 = new QLabel( this, "textLabel8" );
	textLabel82->setText( tr( "Grab Radius:" ) );
	layout11->addWidget( textLabel82, 1, 0 );
	grabDistance = new QSpinBox( this, "grabDistance" );
	grabDistance->setMaxValue( 1000 );
	grabDistance->setMinValue( 1 );
	grabDistance->setLineStep( 1 );
	grabDistance->setValue( prefsData->grabRad );
	grabDistance->setSuffix( tr( " px" ) );
	layout11->addWidget( grabDistance, 1, 1, Qt::AlignLeft );

	checkMargin = new QCheckBox( this, "checkMargin" );
	checkMargin->setText( tr( "Show Margins" ) );
	checkMargin->setChecked(prefsData->marginsShown);
	layout11->addWidget( checkMargin, 2, 0 );
	checkGuides = new QCheckBox( this, "checkGuides" );
	checkGuides->setText( tr( "Show Guides" ) );
	checkGuides->setChecked(prefsData->guidesShown);
	layout11->addWidget( checkGuides, 3, 0 );
	checkBaseline = new QCheckBox( this, "checkBaseline" );
	checkBaseline->setText( tr( "Show Baseline Grid" ) );
	checkBaseline->setChecked(prefsData->baseShown);
	layout11->addWidget( checkBaseline, 4, 0 );
	layout2 = new QHBoxLayout( 0, 0, 5, "layout2");
	textLabel2 = new QLabel( this, "textLabel2" );
	textLabel2->setText( tr( "Color:" ) );
	layout2->addWidget( textLabel2 );
	baselineColor = new QPushButton( this, "baselineColor" );
	baselineColor->setMinimumSize( QSize( 60, 20 ) );
	baselineColor->setMaximumSize( QSize( 60, 20 ) );
	baselineColor->setFlat( false );
	baselineColor->setAutoDefault( false );
	QPixmap pm4 = QPixmap(54, 14);
	pm4.fill(prefsData->baseColor);
	colorBaselineGrid = prefsData->baseColor;
	baselineColor->setPixmap(pm4);
	baselineColor->setText( QString::null );
	layout2->addWidget( baselineColor );
	layout11->addLayout( layout2, 4, 1 );
	layout3 = new QHBoxLayout( 0, 0, 5, "layout3");
	textLabel3 = new QLabel( this, "textLabel3" );
	textLabel3->setText( tr( "Color:" ) );
	layout3->addWidget( textLabel3 );
	guideColor = new QPushButton( this, "guideColor" );
	guideColor->setMinimumSize( QSize( 60, 20 ) );
	guideColor->setMaximumSize( QSize( 60, 20 ) );
	guideColor->setFlat( false );
	guideColor->setAutoDefault( false );
	QPixmap pm3 = QPixmap(54, 14);
	pm3.fill(prefsData->guideColor);
	colorGuides = prefsData->guideColor;
	guideColor->setPixmap(pm3);
	guideColor->setText( QString::null );
	layout3->addWidget( guideColor );
	layout11->addLayout( layout3, 3, 1 );
	layout9 = new QHBoxLayout( 0, 0, 5, "layout9");
	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setText( tr( "Color:" ) );
	layout9->addWidget( textLabel1 );
	marginColor = new QPushButton( this, "marginColor" );
	marginColor->setMinimumSize( QSize( 60, 20 ) );
	marginColor->setMaximumSize( QSize( 60, 20 ) );
	marginColor->setAutoDefault( false );
	QPixmap pm6 = QPixmap(54, 14);
	pm6.fill(prefsData->margColor);
	colorMargin = prefsData->margColor;
	marginColor->setPixmap(pm6);
	marginColor->setFlat( false );
	marginColor->setText( QString::null );
	layout9->addWidget( marginColor );
	layout11->addLayout( layout9, 2, 1 );
	tabGuidesLayout->addLayout( layout11 );
	layout12 = new QGridLayout( 0, 1, 1, 0, 5, "layout12");
	checkLink = new QCheckBox( this, "checkLink" );
	checkLink->setText( tr( "Show Text Chains" ) );
	checkLink->setChecked(prefsData->linkShown);
	layout12->addWidget( checkLink, 1, 0 );
	checkFrame = new QCheckBox( this, "checkFrame" );
	checkFrame->setText( tr( "Show Frames" ) );
	checkFrame->setChecked(prefsData->framesShown);
	layout12->addWidget( checkFrame, 0, 0 );
	checkPictures = new QCheckBox( this, "checkPictures" );
	checkPictures->setText( tr( "Show Pictures" ) );
	layout12->addWidget( checkPictures, 0, 1 );
	checkPictures->setChecked(prefsData->showPic);
	tabGuidesLayout->addLayout( layout12 );
	connect(majorGridColor, SIGNAL(clicked()), this, SLOT(changeMajorColor()));
	connect(minorGridColor, SIGNAL(clicked()), this, SLOT(changeMinorColor()));
	connect(baselineColor, SIGNAL(clicked()), this, SLOT(changeBaselineColor()));
	connect(guideColor, SIGNAL(clicked()), this, SLOT(changeGuideColor()));
	connect(marginColor, SIGNAL(clicked()), this, SLOT(changeMarginColor()));
	QToolTip::add( minorSpace, tr( "Distance between the minor grid lines" ) );
	QToolTip::add( majorSpace, tr( "Distance between the major grid lines" ) );
	QToolTip::add( snapDistance, tr( "Distance within which an object will snap to your placed guides" ) );
	QToolTip::add( grabDistance, tr( "Radius of the area where Scribus will allow you to grab an objects handles" ) );
	QToolTip::add( minorGridColor, tr( "Color of the minor grid lines" ) );
	QToolTip::add( majorGridColor, tr( "Color of the major grid lines" ) );
	QToolTip::add( guideColor, tr( "Color of the guide lines you insert" ) );
	QToolTip::add( marginColor, tr( "Color for the margin lines" ) );
	QToolTip::add( baselineColor, tr( "Color for the basegrid lines" ) );
	QToolTip::add( checkBaseline, tr("Turns the basegrid on or off"));
	QToolTip::add( checkGrid, tr("Turns the gridlines on or off"));
	QToolTip::add( checkGuides, tr("Turns the guides on or off"));
	QToolTip::add( checkMargin, tr("Turns the margins on or off"));
	QToolTip::add( checkLink, tr("Turns the of linked frames on or off"));
	QToolTip::add( checkFrame, tr("Turns the display of frames on or off"));
	QToolTip::add( checkPictures, tr("Turns the display of pictures on or off"));
}

void TabGuides::changeMajorColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorMajorGrid, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorMajorGrid = neu;
		majorGridColor->setPixmap(pm);
	}
}

void TabGuides::changeMinorColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorMinorGrid, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorMinorGrid = neu;
		minorGridColor->setPixmap(pm);
	}
}

void TabGuides::changeBaselineColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorBaselineGrid, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorBaselineGrid = neu;
		baselineColor->setPixmap(pm);
	}
}

void TabGuides::changeGuideColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorGuides, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorGuides = neu;
		guideColor->setPixmap(pm);
	}
}

void TabGuides::changeMarginColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorMargin, this);
	if (neu.isValid())
	{
		QPixmap pm = QPixmap(54, 14);
		pm.fill(neu);
		colorMargin = neu;
		marginColor->setPixmap(pm);
	}
}
