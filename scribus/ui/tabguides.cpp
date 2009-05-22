/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tabguides.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QToolTip>
#include <QColorDialog>
#include <QColor>
#include <QPixmap>
#include "scrspinbox.h"
#include "prefsstructs.h"
#include "scribusstructs.h"
#include "units.h"

TabGuides::TabGuides( QWidget* parent, struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex) : QWidget( parent )
{
	double unitRatio = unitGetRatioFromIndex(unitIndex);
	QString unit = unitGetSuffixFromIndex(unitIndex);

	tabGuidesLayout = new QVBoxLayout( this );
	tabGuidesLayout->setMargin(0);
	tabGuidesLayout->setSpacing(5);

	commonBox = new QGroupBox( this );
	commonBox->setTitle( tr( "Common Settings" ) );
	commonBoxLayout = new QHBoxLayout( commonBox );
	commonBoxLayout->setMargin(10);
	commonBoxLayout->setSpacing(5);

	buttonGroup1 = new QGroupBox( commonBox );
	buttonGroup1->setTitle( tr( "Placing in Documents" ) );
	buttonGroup1Layout = new QVBoxLayout( buttonGroup1 );
	buttonGroup1Layout->setMargin(10);
	buttonGroup1Layout->setSpacing(5);
	buttonGroup1Layout->setAlignment( Qt::AlignTop );
	inBackground = new QRadioButton( buttonGroup1 );
	inBackground->setText( tr( "In the Background" ) );
	buttonGroup1Layout->addWidget( inBackground );
	inForeground = new QRadioButton( buttonGroup1 );
	inForeground->setText( tr( "In the Foreground" ) );
	buttonGroup1Layout->addWidget( inForeground );
	commonBoxLayout->addWidget( buttonGroup1 );

	snapBox = new QGroupBox( commonBox );
	snapBox->setTitle( tr( "Snapping" ) );
	snapBoxLayout = new QGridLayout( snapBox );
	snapBoxLayout->setMargin(10);
	snapBoxLayout->setSpacing(5);
	textLabel8 = new QLabel( snapBox );
	textLabel8->setText( tr( "Snap Distance:" ) );
	snapBoxLayout->addWidget( textLabel8, 0, 0 );
	snapDistance = new QSpinBox( snapBox );
	snapDistance->setMaximum( 1000 );
	snapDistance->setMinimum( 1 );
	snapDistance->setSingleStep( 1 );
	snapBoxLayout->addWidget( snapDistance, 0, 1 );
	textLabel82 = new QLabel( snapBox );
	textLabel82->setText( tr( "Grab Radius:" ) );
	snapBoxLayout->addWidget( textLabel82, 1, 0 );
	grabDistance = new QSpinBox( snapBox );
	grabDistance->setMaximum( 1000 );
	grabDistance->setMinimum( 1 );
	grabDistance->setSingleStep( 1 );
	snapBoxLayout->addWidget( grabDistance, 1, 1 );
	commonBoxLayout->addWidget( snapBox );
	tabGuidesLayout->addWidget( commonBox );
	layout9 = new QHBoxLayout;
	layout9->setMargin(0);
	layout9->setSpacing(5);
	guideBox = new QGroupBox( this );
	guideBox->setTitle( tr( "Show Guides" ) );
	guideBox->setCheckable( true );
	guideBoxLayout = new QHBoxLayout( guideBox );
	guideBoxLayout->setMargin(10);
	guideBoxLayout->setSpacing(5);
	checkGuides = new QLabel( guideBox );
	checkGuides->setText( tr( "Color:" ) );
	guideBoxLayout->addWidget( checkGuides );
	guideColor = new QPushButton( guideBox );
	guideColor->setMinimumSize( QSize( 60, 20 ) );
	guideColor->setMaximumSize( QSize( 60, 20 ) );
	guideColor->setFlat( false );
	guideColor->setAutoDefault( false );
	guideColor->setText( QString::null );
	guideBoxLayout->addWidget( guideColor );
	layout9->addWidget( guideBox );
	marginBox = new QGroupBox( this );
	marginBox->setCheckable( true );
	marginBox->setTitle( tr( "Show Margins" ) );
	marginBoxLayout = new QHBoxLayout( marginBox );
	marginBoxLayout->setMargin(10);
	marginBoxLayout->setSpacing(5);
	checkMargin = new QLabel( marginBox );
	checkMargin->setText( tr( "Color:" ) );
	marginBoxLayout->addWidget( checkMargin );
	marginColor = new QPushButton( marginBox );
	marginColor->setMinimumSize( QSize( 60, 20 ) );
	marginColor->setMaximumSize( QSize( 60, 20 ) );
	marginColor->setAutoDefault( false );
	marginColor->setFlat( false );
	marginColor->setText( QString::null );
	marginBoxLayout->addWidget( marginColor );
	layout9->addWidget( marginBox );
	tabGuidesLayout->addLayout( layout9 );

	checkGrid = new QGroupBox( this );
	checkGrid->setTitle( tr( "Show Page Grid" ) );
	checkGrid->setCheckable( true );
	checkGridLayout = new QGridLayout( checkGrid );
	checkGridLayout->setMargin(10);
	checkGridLayout->setSpacing(5);
	checkGridLayout->setAlignment( Qt::AlignTop );
	groupBox1 = new QGroupBox( checkGrid );
	groupBox1->setTitle( tr( "Major Grid" ) );
	groupBox1Layout = new QGridLayout( groupBox1 );
	groupBox1Layout->setMargin(10);
	groupBox1Layout->setSpacing(5);
	groupBox1Layout->setAlignment( Qt::AlignTop );
	majorGridColor = new QPushButton( groupBox1 );
	majorGridColor->setMinimumSize( QSize( 60, 20 ) );
	majorGridColor->setMaximumSize( QSize( 60, 20 ) );
	majorGridColor->setFlat( false );
	majorGridColor->setAutoDefault( false );
	majorGridColor->setText( QString::null );
	groupBox1Layout->addWidget( majorGridColor, 1, 1 );
	textLabel4 = new QLabel( groupBox1 );
	textLabel4->setText( tr( "Color:" ) );
	groupBox1Layout->addWidget( textLabel4, 1, 0 );
	textLabel6 = new QLabel( groupBox1 );
	textLabel6->setText( tr( "Spacing:" ) );
	groupBox1Layout->addWidget( textLabel6, 0, 0 );
	majorSpace = new ScrSpinBox( 10 * unitRatio, 1000 * unitRatio, groupBox1, unitIndex );
	groupBox1Layout->addWidget( majorSpace, 0, 1 );
	checkGridLayout->addWidget( groupBox1, 0, 0 );
	groupBox2 = new QGroupBox( checkGrid );
	groupBox2->setTitle( tr( "Minor Grid" ) );
	groupBox2Layout = new QGridLayout( groupBox2 );
	groupBox2Layout->setMargin(10);
	groupBox2Layout->setSpacing(5);
	groupBox2Layout->setAlignment( Qt::AlignTop );
	textLabel5 = new QLabel( groupBox2 );
	textLabel5->setText( tr( "Color:" ) );
	groupBox2Layout->addWidget( textLabel5, 1, 0 );
	minorGridColor = new QPushButton( groupBox2 );
	minorGridColor->setMinimumSize( QSize( 60, 20 ) );
	minorGridColor->setMaximumSize( QSize( 60, 20 ) );
	minorGridColor->setFlat( false );
	minorGridColor->setAutoDefault( false );
	minorGridColor->setText( QString::null );
	groupBox2Layout->addWidget( minorGridColor, 1, 1 );
	textLabel7 = new QLabel( groupBox2 );
	textLabel7->setText( tr( "Spacing:" ) );
	groupBox2Layout->addWidget( textLabel7, 0, 0 );
	minorSpace = new ScrSpinBox( unitRatio, 1000 * unitRatio, groupBox2, unitIndex );
	groupBox2Layout->addWidget( minorSpace, 0, 1 );
	checkGridLayout->addWidget( groupBox2, 0, 1 );
	tabGuidesLayout->addWidget( checkGrid );

	layout9a = new QHBoxLayout;
	layout9a->setMargin(0);
	layout9a->setSpacing(5);
	baselineBox = new QGroupBox( this );
	baselineBox->setTitle( tr( "Show Baseline Grid" ) );
	baselineBox->setCheckable( true );
	baselineBoxLayout = new QGridLayout( baselineBox );
	baselineBoxLayout->setMargin(10);
	baselineBoxLayout->setSpacing(5);
	baselineBoxLayout->setAlignment( Qt::AlignTop );
	checkBaseline = new QLabel( baselineBox );
	checkBaseline->setText( tr( "Color:" ) );
	baselineBoxLayout->addWidget( checkBaseline, 0, 0 );
	baselineColor = new QPushButton( baselineBox );
	baselineColor->setMinimumSize( QSize( 60, 20 ) );
	baselineColor->setMaximumSize( QSize( 60, 20 ) );
	baselineColor->setFlat( false );
	baselineColor->setAutoDefault( false );
	baselineColor->setText( QString::null );
	baselineBoxLayout->addWidget( baselineColor, 0, 1 );
	layout9a->addWidget( baselineBox );

	baseGridBox = new QGroupBox( this );
	baseGridBox->setTitle( tr( "Baseline Settings" ) );
	baseGridBoxLayout = new QGridLayout( baseGridBox );
	baseGridBoxLayout->setMargin(10);
	baseGridBoxLayout->setSpacing(5);
	baseGridBoxLayout->setAlignment( Qt::AlignTop );
	baseGrid = new ScrSpinBox( 1.0, 1000.0, baseGridBox, 0 );
	baseGridBoxLayout->addWidget( baseGrid, 0, 1 );
	textLabel6a = new QLabel( tr( "Baseline &Grid:" ), baseGridBox);
	textLabel6a->setBuddy(baseGrid);
	baseGridBoxLayout->addWidget( textLabel6a, 0, 0 );
	baseOffset = new ScrSpinBox( 0.0, 1000.0, baseGridBox, 0 );
	baseGridBoxLayout->addWidget( baseOffset, 1, 1 );
	textLabel7a = new QLabel( tr( "Baseline &Offset:" ), baseGridBox );
	textLabel7a->setBuddy(baseOffset);
	baseGridBoxLayout->addWidget( textLabel7a, 1, 0 );
	layout9a->addWidget( baseGridBox );
	tabGuidesLayout->addLayout( layout9a );

	// switched off as it's called in main prefs classes - PV
	//restoreDefaults(prefsData, prefsData2, unitIndex);

	connect(majorGridColor, SIGNAL(clicked()), this, SLOT(changeMajorColor()));
	connect(minorGridColor, SIGNAL(clicked()), this, SLOT(changeMinorColor()));
	connect(baselineColor, SIGNAL(clicked()), this, SLOT(changeBaselineColor()));
	connect(guideColor, SIGNAL(clicked()), this, SLOT(changeGuideColor()));
	connect(marginColor, SIGNAL(clicked()), this, SLOT(changeMarginColor()));
	inBackground->setToolTip( tr( "Guides are not visible through objects on the page" ) );
	inForeground->setToolTip( tr( "Guides are visible above all objects on the page" ) );
	minorSpace->setToolTip( tr( "Distance between the minor grid lines" ) );
	majorSpace->setToolTip( tr( "Distance between the major grid lines" ) );
	snapDistance->setToolTip(  "<qt>" + tr( "Distance within which an object will snap to your placed guides. After setting this you will need to restart Scribus to set this setting." ) + "</qt>");
	grabDistance->setToolTip( "<qt>" + tr( "Radius of the area where Scribus will allow you to grab an objects handles.After setting this you will need to restart Scribus to set this setting." ) + "</qt>");
	minorGridColor->setToolTip( tr( "Color of the minor grid lines" ) );
	majorGridColor->setToolTip( tr( "Color of the major grid lines" ) );
	guideColor->setToolTip( tr( "Color of the guide lines you insert" ) );
	marginColor->setToolTip( tr( "Color for the margin lines" ) );
	baselineColor->setToolTip( tr( "Color for the baseline grid" ) );
	baselineBox->setToolTip( tr("Turns the basegrid on or off"));
	baseGrid->setToolTip( tr("Distance between the lines of the baseline grid"));
	baseOffset->setToolTip( tr("Distance from the top of the page for the first baseline"));
	checkGrid->setToolTip( tr("Turns the gridlines on or off"));
	guideBox->setToolTip( tr("Turns the guides on or off"));
	marginBox->setToolTip( tr("Turns the margins on or off"));
}

void TabGuides::restoreDefaults(struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex)
{
	QString unit = unitGetSuffixFromIndex(unitIndex);
	double unitRatio = unitGetRatioFromIndex(unitIndex);
	int decimals = unitGetPrecisionFromIndex(unitIndex);

	QPixmap pm3(54, 14);
	pm3.fill(prefsData->guideColor);
	colorGuides = prefsData->guideColor;
	guideColor->setIcon(pm3);
	QPixmap pm6(54, 14);
	pm6.fill(prefsData->margColor);
	colorMargin = prefsData->margColor;
	marginColor->setIcon(pm6);
	QPixmap pm1(54, 14);
	pm1.fill(prefsData->majorColor);
	colorMajorGrid = prefsData->majorColor;
	majorGridColor->setIcon(pm1);
	QPixmap pm(54, 14);
	pm.fill(prefsData->minorColor);
	colorMinorGrid = prefsData->minorColor;
	minorGridColor->setIcon(pm);
	QPixmap pm4(54, 14);
	pm4.fill(prefsData->baseColor);
	colorBaselineGrid = prefsData->baseColor;
	baselineColor->setIcon(pm4);
	minorSpace->setDecimals( decimals );
	minorSpace->setValue(prefsData->minorGrid  * unitRatio);
	minorSpace->setSuffix( unit );
	majorSpace->setDecimals( decimals );
	majorSpace->setValue(prefsData->majorGrid * unitRatio);
	majorSpace->setSuffix( unit );
	snapDistance->setValue(qRound(prefsData->guideRad));
	snapDistance->setSuffix( " " + tr( "px" ) );
	grabDistance->setValue(prefsData->grabRad);
	grabDistance->setSuffix( " " + tr( " px" ) );
	baseGrid->setValue(prefsData2->valueBaseGrid);
	baseOffset->setValue(prefsData2->offsetBaseGrid);
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
		majorGridColor->setIcon(pm);
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
		minorGridColor->setIcon(pm);
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
		baselineColor->setIcon(pm);
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
		guideColor->setIcon(pm);
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
		marginColor->setIcon(pm);
	}
}

void TabGuides::unitChange(QString unit, int docUnitIndex, double invUnitConversion)
{
	double oldMin, oldMax, val;
	int decimalsOld;
	int decimal = unitGetPrecisionFromIndex(docUnitIndex);

	minorSpace->setSuffix(unit);
	majorSpace->setSuffix(unit);
	
	minorSpace->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	minorSpace->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimal, val * invUnitConversion);
	majorSpace->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	majorSpace->setValues(oldMin * invUnitConversion, oldMax * invUnitConversion, decimal, val * invUnitConversion);
}
