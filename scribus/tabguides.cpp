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
#include "units.h"

TabGuides::TabGuides( QWidget* parent, struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex) : QWidget( parent, "tabguide", 0 )
{
	unit = unitGetSuffixFromIndex(unitIndex);
	precision = unitGetPrecisionFromIndex(unitIndex);
	unitRatio = unitGetRatioFromIndex(unitIndex);
	
	tabGuidesLayout = new QVBoxLayout( this, 10, 5, "tabViewLayout");

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
	guideBox->setTitle( tr( "Guides" ) );
	guideBox->setColumnLayout(0, Qt::Vertical );
	guideBox->layout()->setSpacing( 5 );
	guideBox->layout()->setMargin( 10 );
	guideBoxLayout = new QHBoxLayout( guideBox->layout() );
	checkGuides = new QCheckBox( guideBox, "checkGuides" );
	checkGuides->setText( tr( "Show Guides" ) );
	checkGuides->setChecked(prefsData->guidesShown);
	guideBoxLayout->addWidget( checkGuides );
	guideColor = new QPushButton( guideBox, "guideColor" );
	guideColor->setMinimumSize( QSize( 60, 20 ) );
	guideColor->setMaximumSize( QSize( 60, 20 ) );
	guideColor->setFlat( false );
	guideColor->setAutoDefault( false );
	QPixmap pm3 = QPixmap(54, 14);
	pm3.fill(prefsData->guideColor);
	colorGuides = prefsData->guideColor;
	guideColor->setPixmap(pm3);
	guideColor->setText( QString::null );
	guideBoxLayout->addWidget( guideColor );
	layout9->addWidget( guideBox );
	marginBox = new QGroupBox( this, "guideBox" );
	marginBox->setTitle( tr( "Margins" ) );
	marginBox->setColumnLayout(0, Qt::Vertical );
	marginBox->layout()->setSpacing( 5 );
	marginBox->layout()->setMargin( 10 );
	marginBoxLayout = new QHBoxLayout( marginBox->layout() );
	checkMargin = new QCheckBox( marginBox, "checkMargin" );
	checkMargin->setText( tr( "Show Margins" ) );
	checkMargin->setChecked(prefsData->marginsShown);
	marginBoxLayout->addWidget( checkMargin );
	marginColor = new QPushButton( marginBox, "marginColor" );
	marginColor->setMinimumSize( QSize( 60, 20 ) );
	marginColor->setMaximumSize( QSize( 60, 20 ) );
	marginColor->setAutoDefault( false );
	QPixmap pm6 = QPixmap(54, 14);
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
	QPixmap pm = QPixmap(54, 14);
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

	baselineBox = new QGroupBox( this, "baselineBox" );
	baselineBox->setTitle( tr( "Baseline Grid" ) );
	baselineBox->setColumnLayout(0, Qt::Vertical );
	baselineBox->layout()->setSpacing( 5 );
	baselineBox->layout()->setMargin( 10 );
	baselineBoxLayout = new QGridLayout( baselineBox->layout() );
	baselineBoxLayout->setAlignment( Qt::AlignTop );
	checkBaseline = new QCheckBox( baselineBox, "checkBaseline" );
	checkBaseline->setText( tr( "Show Baseline Grid" ) );
	checkBaseline->setChecked(prefsData->baseShown);
	baselineBoxLayout->addWidget( checkBaseline, 0, 0 );
	baselineColor = new QPushButton( baselineBox, "baselineColor" );
	baselineColor->setMinimumSize( QSize( 60, 20 ) );
	baselineColor->setMaximumSize( QSize( 60, 20 ) );
	baselineColor->setFlat( false );
	baselineColor->setAutoDefault( false );
	QPixmap pm4 = QPixmap(54, 14);
	pm4.fill(prefsData->baseColor);
	colorBaselineGrid = prefsData->baseColor;
	baselineColor->setPixmap(pm4);
	baselineColor->setText( QString::null );
	baselineBoxLayout->addWidget( baselineColor, 0, 1 );
	autoLine = new QSpinBox( baselineBox, "autoLine" );
	autoLine->setMaxValue( 100 );
	autoLine->setMinValue( 1 );
	autoLine->setValue( prefsData2->autoLineSpacing );
	autoLine->setSuffix( tr( " %" ) );
	baselineBoxLayout->addWidget( autoLine, 1, 1 );
	textLabel8a = new QLabel( autoLine, tr( "Automatic &Line Spacing:" ), baselineBox, "textLabel8a" );
	baselineBoxLayout->addWidget( textLabel8a, 1, 0 );
	baseGrid = new MSpinBox( 1, 1000, baselineBox, precision );
	baseGrid->setSuffix( unit );
	baseGrid->setValue(prefsData2->valueBaseGrid * unitRatio);
	baselineBoxLayout->addWidget( baseGrid, 0, 3 );
	textLabel6a = new QLabel(baseGrid, tr( "Baseline &Grid:" ), baselineBox, "textLabel6a" );
	baselineBoxLayout->addWidget( textLabel6a, 0, 2 );
	baseOffset = new MSpinBox( 0, 1000, baselineBox, precision );
	baseOffset->setSuffix( unit );
	baseOffset->setValue(prefsData2->offsetBaseGrid * unitRatio);
	baselineBoxLayout->addWidget( baseOffset, 1, 3 );
	textLabel7a = new QLabel(baseOffset, tr( "Baseline &Offset:" ), baselineBox, "textLabel7a" );
	baselineBoxLayout->addWidget( textLabel7a, 1, 2 );
	tabGuidesLayout->addWidget( baselineBox );

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
	QToolTip::add( autoLine, tr( "Percentage increase over the font size for the line spacing" ) );
}

void TabGuides::restoreDefaults(struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex)
{
	unit = unitGetSuffixFromIndex(unitIndex);
	precision = unitGetPrecisionFromIndex(unitIndex);
	unitRatio = unitGetRatioFromIndex(unitIndex);
	QPixmap pm3 = QPixmap(54, 14);
	pm3.fill(prefsData->guideColor);
	colorGuides = prefsData->guideColor;
	guideColor->setPixmap(pm3);
	QPixmap pm6 = QPixmap(54, 14);
	pm6.fill(prefsData->margColor);
	colorMargin = prefsData->margColor;
	marginColor->setPixmap(pm6);
	QPixmap pm1 = QPixmap(54, 14);
	pm1.fill(prefsData->majorColor);
	colorMajorGrid = prefsData->majorColor;
	majorGridColor->setPixmap(pm1);
	QPixmap pm = QPixmap(54, 14);
	pm.fill(prefsData->minorColor);
	colorMinorGrid = prefsData->minorColor;
	minorGridColor->setPixmap(pm);
	QPixmap pm4 = QPixmap(54, 14);
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
	checkBaseline->setChecked(prefsData->baseShown);
	checkGrid->setChecked(prefsData->gridShown);
	checkMargin->setChecked(prefsData->marginsShown);
	checkGuides->setChecked(prefsData->guidesShown);
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
