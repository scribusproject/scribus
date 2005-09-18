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
#include "prefsstructs.h"
#include "scribusstructs.h"
#include "units.h"

TabGuides::TabGuides( QWidget* parent, struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex) : QWidget( parent, "tabguide", 0 )
{
	unit = unitGetSuffixFromIndex(unitIndex);
	precision = unitGetPrecisionFromIndex(unitIndex);
	unitRatio = unitGetRatioFromIndex(unitIndex);
	
	tabGuidesLayout = new QVBoxLayout( this, 0, 5, "tabViewLayout");

	commonBox = new QGroupBox( this, "commonBox" );
	commonBox->setTitle( tr( "Common Settings" ) );
	commonBox->setColumnLayout(0, Qt::Vertical );
	commonBox->layout()->setSpacing( 5 );
	commonBox->layout()->setMargin( 10 );
	commonBoxLayout = new QHBoxLayout( commonBox->layout() );

	buttonGroup1 = new QButtonGroup( commonBox, "buttonGroup1" );
	buttonGroup1->setExclusive( true );
	buttonGroup1->setColumnLayout(0, Qt::Horizontal );
	buttonGroup1->layout()->setSpacing( 5 );
	buttonGroup1->layout()->setMargin( 10 );
	buttonGroup1->setTitle( tr( "Placing in Documents" ) );
	buttonGroup1Layout = new QVBoxLayout( buttonGroup1->layout() );
	buttonGroup1Layout->setAlignment( Qt::AlignTop );
	inBackground = new QRadioButton( buttonGroup1, "inBackground" );
	inBackground->setText( tr( "In the Background" ) );
	buttonGroup1Layout->addWidget( inBackground );
	inForeground = new QRadioButton( buttonGroup1, "inForeground" );
	inForeground->setText( tr( "In the Foreground" ) );
	buttonGroup1Layout->addWidget( inForeground );
	inBackground->setChecked( prefsData->before );
	inForeground->setChecked( !prefsData->before );
	commonBoxLayout->addWidget( buttonGroup1 );

	snapBox = new QGroupBox( commonBox, "snapBox" );
	snapBox->setTitle( tr( "Snapping" ) );
	snapBox->setColumnLayout(0, Qt::Vertical );
	snapBox->layout()->setSpacing( 5 );
	snapBox->layout()->setMargin( 10 );
	snapBoxLayout = new QGridLayout( snapBox->layout() );
	textLabel8 = new QLabel( snapBox, "textLabel8" );
	textLabel8->setText( tr( "Snap Distance:" ) );
	snapBoxLayout->addWidget( textLabel8, 0, 0 );
	snapDistance = new MSpinBox( unitRatio, 1000 * unitRatio, snapBox, precision );
	snapDistance->setValue( prefsData->guideRad * unitRatio );
	snapDistance->setSuffix( unit );
	snapBoxLayout->addWidget( snapDistance, 0, 1 );
	textLabel82 = new QLabel( snapBox, "textLabel8" );
	textLabel82->setText( tr( "Grab Radius:" ) );
	snapBoxLayout->addWidget( textLabel82, 1, 0 );
	grabDistance = new QSpinBox( snapBox, "grabDistance" );
	grabDistance->setMaxValue( 1000 );
	grabDistance->setMinValue( 1 );
	grabDistance->setLineStep( 1 );
	grabDistance->setValue( prefsData->grabRad );
	grabDistance->setSuffix( tr( " px" ) );
	snapBoxLayout->addWidget( grabDistance, 1, 1 );
	commonBoxLayout->addWidget( snapBox );
	tabGuidesLayout->addWidget( commonBox );
	layout9 = new QHBoxLayout( 0, 0, 5, "layout9");
	guideBox = new QGroupBox( this, "guideBox" );
	guideBox->setTitle( tr( "Show Guides" ) );
	guideBox->setCheckable( true );
	guideBox->setChecked(prefsData->guidesShown);
	guideBox->setColumnLayout(0, Qt::Vertical );
	guideBox->layout()->setSpacing( 5 );
	guideBox->layout()->setMargin( 10 );
	guideBoxLayout = new QHBoxLayout( guideBox->layout() );
	checkGuides = new QLabel( guideBox, "checkGuides" );
	checkGuides->setText( tr( "Color:" ) );
	guideBoxLayout->addWidget( checkGuides );
	guideColor = new QPushButton( guideBox, "guideColor" );
	guideColor->setMinimumSize( QSize( 60, 20 ) );
	guideColor->setMaximumSize( QSize( 60, 20 ) );
	guideColor->setFlat( false );
	guideColor->setAutoDefault( false );
	QPixmap pm3(54, 14);
	pm3.fill(prefsData->guideColor);
	colorGuides = prefsData->guideColor;
	guideColor->setPixmap(pm3);
	guideColor->setText( QString::null );
	guideBoxLayout->addWidget( guideColor );
	layout9->addWidget( guideBox );
	marginBox = new QGroupBox( this, "guideBox" );
	marginBox->setCheckable( true );
	marginBox->setTitle( tr( "Show Margins" ) );
	marginBox->setChecked(prefsData->marginsShown);
	marginBox->setColumnLayout(0, Qt::Vertical );
	marginBox->layout()->setSpacing( 5 );
	marginBox->layout()->setMargin( 10 );
	marginBoxLayout = new QHBoxLayout( marginBox->layout() );
	checkMargin = new QLabel( marginBox, "checkMargin" );
	checkMargin->setText( tr( "Color:" ) );
	marginBoxLayout->addWidget( checkMargin );
	marginColor = new QPushButton( marginBox, "marginColor" );
	marginColor->setMinimumSize( QSize( 60, 20 ) );
	marginColor->setMaximumSize( QSize( 60, 20 ) );
	marginColor->setAutoDefault( false );
	QPixmap pm6(54, 14);
	pm6.fill(prefsData->margColor);
	colorMargin = prefsData->margColor;
	marginColor->setPixmap(pm6);
	marginColor->setFlat( false );
	marginColor->setText( QString::null );
	marginBoxLayout->addWidget( marginColor );
	layout9->addWidget( marginBox );
	tabGuidesLayout->addLayout( layout9 );

	checkGrid = new QGroupBox( this, "checkGrid" );
	checkGrid->setTitle( tr( "Show Page Grid" ) );
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
	QPixmap pm1(54, 14);
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
	majorSpace = new MSpinBox( 10 * unitRatio, 1000 * unitRatio, groupBox1, precision );
	majorSpace->setValue( prefsData->majorGrid * unitRatio );
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
	QPixmap pm(54, 14);
	pm.fill(prefsData->minorColor);
	colorMinorGrid = prefsData->minorColor;
	minorGridColor->setPixmap(pm);
	minorGridColor->setText( QString::null );
	groupBox2Layout->addWidget( minorGridColor, 1, 1 );
	textLabel7 = new QLabel( groupBox2, "textLabel7" );
	textLabel7->setText( tr( "Spacing:" ) );
	groupBox2Layout->addWidget( textLabel7, 0, 0 );
	minorSpace = new MSpinBox( unitRatio, 1000 * unitRatio, groupBox2, precision );
	minorSpace->setValue( prefsData->minorGrid  * unitRatio);
	minorSpace->setSuffix( unit );
	groupBox2Layout->addWidget( minorSpace, 0, 1 );
	checkGridLayout->addWidget( groupBox2, 0, 1 );
	tabGuidesLayout->addWidget( checkGrid );

	layout9a = new QHBoxLayout( 0, 0, 5, "layout9");
	baselineBox = new QGroupBox( this, "baselineBox" );
	baselineBox->setTitle( tr( "Show Baseline Grid" ) );
	baselineBox->setCheckable( true );
	baselineBox->setChecked(prefsData->baseShown);
	baselineBox->setColumnLayout(0, Qt::Vertical );
	baselineBox->layout()->setSpacing( 5 );
	baselineBox->layout()->setMargin( 10 );
	baselineBoxLayout = new QGridLayout( baselineBox->layout() );
	baselineBoxLayout->setAlignment( Qt::AlignTop );
	checkBaseline = new QLabel( baselineBox, "checkBaseline" );
	checkBaseline->setText( tr( "Color:" ) );
	baselineBoxLayout->addWidget( checkBaseline, 0, 0 );
	baselineColor = new QPushButton( baselineBox, "baselineColor" );
	baselineColor->setMinimumSize( QSize( 60, 20 ) );
	baselineColor->setMaximumSize( QSize( 60, 20 ) );
	baselineColor->setFlat( false );
	baselineColor->setAutoDefault( false );
	QPixmap pm4(54, 14);
	pm4.fill(prefsData->baseColor);
	colorBaselineGrid = prefsData->baseColor;
	baselineColor->setPixmap(pm4);
	baselineColor->setText( QString::null );
	baselineBoxLayout->addWidget( baselineColor, 0, 1 );
	layout9a->addWidget( baselineBox );

	baseGridBox = new QGroupBox( this, "baseGridBox" );
	baseGridBox->setTitle( tr( "Baseline Settings" ) );
	baseGridBox->setColumnLayout(0, Qt::Vertical );
	baseGridBox->layout()->setSpacing( 5 );
	baseGridBox->layout()->setMargin( 10 );
	baseGridBoxLayout = new QGridLayout( baseGridBox->layout() );
	baseGridBoxLayout->setAlignment( Qt::AlignTop );
	baseGrid = new MSpinBox( baseGridBox, precision );
	baseGrid->setMaxValue(1000);
	baseGrid->setMinValue(pts2value(1.0, unitIndex));
	baseGrid->setSuffix( unit );
	baseGrid->setValue(prefsData2->valueBaseGrid * unitRatio);
	baseGridBoxLayout->addWidget( baseGrid, 0, 1 );
	textLabel6a = new QLabel(baseGrid, tr( "Baseline &Grid:" ), baseGridBox, "textLabel6a" );
	baseGridBoxLayout->addWidget( textLabel6a, 0, 0 );
	baseOffset = new MSpinBox( 0, 1000, baseGridBox, precision );
	baseOffset->setSuffix( unit );
	baseOffset->setValue(prefsData2->offsetBaseGrid * unitRatio);
	baseGridBoxLayout->addWidget( baseOffset, 1, 1 );
	textLabel7a = new QLabel(baseOffset, tr( "Baseline &Offset:" ), baseGridBox, "textLabel7a" );
	baseGridBoxLayout->addWidget( textLabel7a, 1, 0 );
	layout9a->addWidget( baseGridBox );
	tabGuidesLayout->addLayout( layout9a );

	connect(majorGridColor, SIGNAL(clicked()), this, SLOT(changeMajorColor()));
	connect(minorGridColor, SIGNAL(clicked()), this, SLOT(changeMinorColor()));
	connect(baselineColor, SIGNAL(clicked()), this, SLOT(changeBaselineColor()));
	connect(guideColor, SIGNAL(clicked()), this, SLOT(changeGuideColor()));
	connect(marginColor, SIGNAL(clicked()), this, SLOT(changeMarginColor()));
	QToolTip::add( inBackground, tr( "Guides are not visible through objects on the page" ) );
	QToolTip::add( inForeground, tr( "Guides are visible above all objects on the page" ) );
	QToolTip::add( minorSpace, tr( "Distance between the minor grid lines" ) );
	QToolTip::add( majorSpace, tr( "Distance between the major grid lines" ) );
	QToolTip::add( snapDistance, tr( "Distance within which an object will snap to your placed guides" ) );
	QToolTip::add( grabDistance, tr( "Radius of the area where Scribus will allow you to grab an objects handles" ) );
	QToolTip::add( minorGridColor, tr( "Color of the minor grid lines" ) );
	QToolTip::add( majorGridColor, tr( "Color of the major grid lines" ) );
	QToolTip::add( guideColor, tr( "Color of the guide lines you insert" ) );
	QToolTip::add( marginColor, tr( "Color for the margin lines" ) );
	QToolTip::add( baselineColor, tr( "Color for the baseline grid" ) );
	QToolTip::add( baselineBox, tr("Turns the basegrid on or off"));
	QToolTip::add( baseGrid, tr("Distance between the lines of the baseline grid"));
	QToolTip::add( baseOffset, tr("Distance from the top of the page for the first baseline"));
	QToolTip::add( checkGrid, tr("Turns the gridlines on or off"));
	QToolTip::add( guideBox, tr("Turns the guides on or off"));
	QToolTip::add( marginBox, tr("Turns the margins on or off"));
}

void TabGuides::restoreDefaults(struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex)
{
	unit = unitGetSuffixFromIndex(unitIndex);
	precision = unitGetPrecisionFromIndex(unitIndex);
	unitRatio = unitGetRatioFromIndex(unitIndex);
	QPixmap pm3(54, 14);
	pm3.fill(prefsData->guideColor);
	colorGuides = prefsData->guideColor;
	guideColor->setPixmap(pm3);
	QPixmap pm6(54, 14);
	pm6.fill(prefsData->margColor);
	colorMargin = prefsData->margColor;
	marginColor->setPixmap(pm6);
	QPixmap pm1(54, 14);
	pm1.fill(prefsData->majorColor);
	colorMajorGrid = prefsData->majorColor;
	majorGridColor->setPixmap(pm1);
	QPixmap pm(54, 14);
	pm.fill(prefsData->minorColor);
	colorMinorGrid = prefsData->minorColor;
	minorGridColor->setPixmap(pm);
	QPixmap pm4(54, 14);
	pm4.fill(prefsData->baseColor);
	colorBaselineGrid = prefsData->baseColor;
	baselineColor->setPixmap(pm4);
	minorSpace->setValue(prefsData->minorGrid  * unitRatio);
	majorSpace->setValue(prefsData->majorGrid * unitRatio);
	snapDistance->setValue(prefsData->guideRad * unitRatio);
	grabDistance->setValue(prefsData->grabRad);
	baseGrid->setValue(prefsData2->valueBaseGrid * unitRatio);
	baseOffset->setValue(prefsData2->offsetBaseGrid * unitRatio);
	inBackground->setChecked( prefsData->before );
	inForeground->setChecked( !prefsData->before );
	baselineBox->setChecked(prefsData->baseShown);
	checkGrid->setChecked(prefsData->gridShown);
	marginBox->setChecked(prefsData->marginsShown);
	guideBox->setChecked(prefsData->guidesShown);
}

void TabGuides::changeMajorColor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(colorMajorGrid, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
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
		QPixmap pm(54, 14);
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
		QPixmap pm(54, 14);
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
		QPixmap pm(54, 14);
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
		QPixmap pm(54, 14);
		pm.fill(neu);
		colorMargin = neu;
		marginColor->setPixmap(pm);
	}
}
