#include "tabtypography.h"
#include "tabtypography.moc"
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include "mspinbox.h"

TabTypograpy::TabTypograpy( QWidget* parent, struct typoStruct *prefsData, double unitBase, QString unit) : QWidget( parent, "tabtypo", 0 )
{
	tabTypoLayout = new QGridLayout( this, 1, 1, 11, 6, "tabTypoLayout");
	groupBox1a = new QGroupBox( this, "groupBox1a" );
	groupBox1a->setColumnLayout(0, Qt::Vertical );
	groupBox1a->layout()->setSpacing( 6 );
	groupBox1a->layout()->setMargin( 11 );
	groupBox1a->setTitle( tr( "Subscript" ) );
	groupBox1aLayout = new QGridLayout( groupBox1a->layout() );
	groupBox1aLayout->setAlignment( Qt::AlignTop );
	subDisplacement = new QSpinBox( groupBox1a, "subDisplacement" );
	subDisplacement->setMaxValue( 100 );
	subDisplacement->setValue( prefsData->valueSubScript );
	subDisplacement->setSuffix( tr( " %" ) );
	groupBox1aLayout->addWidget( subDisplacement, 0, 1, Qt::AlignLeft );
	textLabel1a = new QLabel(subDisplacement, tr( "&Displacement:" ), groupBox1a, "textLabel1a" );
	groupBox1aLayout->addWidget( textLabel1a, 0, 0 );
	subScaling = new QSpinBox( groupBox1a, "subScaling" );
	subScaling->setMaxValue( 100 );
	subScaling->setMinValue( 1 );
	subScaling->setValue( prefsData->scalingSubScript );
	subScaling->setSuffix( tr( " %" ) );
	groupBox1aLayout->addWidget( subScaling, 0, 3, Qt::AlignLeft );
	textLabel2a = new QLabel(subScaling, tr( "&Scaling:" ), groupBox1a, "textLabel2a" );
	groupBox1aLayout->addWidget( textLabel2a, 0, 2 );
	tabTypoLayout->addWidget( groupBox1a, 0, 0 );
	groupBox2a = new QGroupBox( this, "groupBox2a" );
	groupBox2a->setColumnLayout(0, Qt::Vertical );
	groupBox2a->layout()->setSpacing( 6 );
	groupBox2a->layout()->setMargin( 11 );
	groupBox2a->setTitle( tr( "Superscript" ) );
	groupBox2aLayout = new QGridLayout( groupBox2a->layout() );
	groupBox2aLayout->setAlignment( Qt::AlignTop );
	superDisplacement = new QSpinBox( groupBox2a, "superDisplacement" );
	superDisplacement->setMaxValue( 100 );
	superDisplacement->setValue( prefsData->valueSuperScript );
	superDisplacement->setSuffix( tr( " %" ) );
	groupBox2aLayout->addWidget( superDisplacement, 0, 1, Qt::AlignLeft );
	textLabel3a = new QLabel(superDisplacement, tr( "D&isplacement:" ), groupBox2a, "textLabel3a" );
	groupBox2aLayout->addWidget( textLabel3a, 0, 0 );
	superScaling = new QSpinBox( groupBox2a, "superScaling" );
	superScaling->setMaxValue( 100 );
	superScaling->setMinValue( 1 );
	superScaling->setValue( prefsData->scalingSuperScript );
	superScaling->setSuffix( tr( " %" ) );
	groupBox2aLayout->addWidget( superScaling, 0, 3, Qt::AlignLeft );
	textLabel4a = new QLabel(superScaling, tr( "S&caling:" ), groupBox2a, "textLabel4a" );
	groupBox2aLayout->addWidget( textLabel4a, 0, 2 );
	tabTypoLayout->addWidget( groupBox2a, 1, 0 );
	groupBox3a = new QGroupBox( this, "groupBox3a" );
	groupBox3a->setColumnLayout(0, Qt::Vertical );
	groupBox3a->layout()->setSpacing( 6 );
	groupBox3a->layout()->setMargin( 11 );
	groupBox3a->setTitle( tr( "Small Caps" ) );
	groupBox3aLayout = new QGridLayout( groupBox3a->layout() );
	groupBox3aLayout->setAlignment( Qt::AlignTop );
	capsScaling = new QSpinBox( groupBox3a, "capsScaling" );
	capsScaling->setMaxValue( 100 );
	capsScaling->setMinValue( 1 );
	capsScaling->setValue( prefsData->valueSmallCaps );
	capsScaling->setSuffix( tr( " %" ) );
	groupBox3aLayout->addWidget( capsScaling, 0, 1, Qt::AlignLeft );
	textLabel5a = new QLabel(capsScaling, tr( "Sc&aling:" ), groupBox3a, "textLabel5a" );
	groupBox3aLayout->addWidget( textLabel5a, 0, 0 );
	tabTypoLayout->addWidget( groupBox3a, 2, 0 );
	groupBox4a = new QGroupBox( this, "groupBox4a" );
	groupBox4a->setColumnLayout(0, Qt::Vertical );
	groupBox4a->layout()->setSpacing( 6 );
	groupBox4a->layout()->setMargin( 11 );
	groupBox4a->setTitle( tr( "Other" ) );
	groupBox4aLayout = new QGridLayout( groupBox4a->layout() );
	groupBox4aLayout->setAlignment( Qt::AlignTop );
	baseGrid = new MSpinBox( 1, 1000, groupBox4a, 1 );
	baseGrid->setSuffix( unit );
	baseGrid->setValue(prefsData->valueBaseGrid * unitBase);
	groupBox4aLayout->addWidget( baseGrid, 0, 1, Qt::AlignLeft );
	textLabel6a = new QLabel(baseGrid, tr( "Baseline &Grid:" ),groupBox4a, "textLabel6a" );
	groupBox4aLayout->addWidget( textLabel6a, 0, 0 );
	baseOffset = new MSpinBox( 0, 1000, groupBox4a, 1 );
	baseOffset->setSuffix( unit );
	baseOffset->setValue(prefsData->offsetBaseGrid * unitBase);
	groupBox4aLayout->addWidget( baseOffset, 1, 1, Qt::AlignLeft );
	textLabel7a = new QLabel(baseOffset, tr( "Baseline &Offset:" ),groupBox4a, "textLabel7a" );
	groupBox4aLayout->addWidget( textLabel7a, 1, 0 );
	autoLine = new QSpinBox( groupBox4a, "autoLine" );
	autoLine->setMaxValue( 100 );
	autoLine->setMinValue( 1 );
	autoLine->setValue( prefsData->autoLineSpacing );
	autoLine->setSuffix( tr( " %" ) );
	groupBox4aLayout->addWidget( autoLine, 2, 1, Qt::AlignLeft );
	textLabel8a = new QLabel( autoLine, tr( "Automatic &Line Spacing:" ), groupBox4a, "textLabel8a" );
	groupBox4aLayout->addWidget( textLabel8a, 2, 0 );
	tabTypoLayout->addWidget( groupBox4a, 3, 0 );
	QToolTip::add( superDisplacement, tr( "Displacement above the baseline of the font on a line" ) );
	QToolTip::add( superScaling, tr( "Relative size of the superscript compared to the normal font" ) );
	QToolTip::add( subDisplacement, tr( "Displacement below the baseline of the normal font on a line" ) );
	QToolTip::add( subScaling, tr( "Relative size of the subscript compared to the normal font" ) );
	QToolTip::add( capsScaling, tr( "Relative size of the small caps font compared to the normal font" ) );
	QToolTip::add( autoLine, tr( "Percentage increase over the font size for the line spacing" ) );
}
