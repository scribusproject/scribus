/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tabtypography.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include <QToolTip>
#include "scrspinbox.h"
#include "prefsstructs.h"
#include "scribusstructs.h"

TabTypograpy::TabTypograpy( QWidget* parent, struct typoPrefs *prefsData) : QWidget( parent )
{
	tabTypoLayout = new QGridLayout( this );
	tabTypoLayout->setMargin(0);
	tabTypoLayout->setSpacing(5);
	groupBox1a = new QGroupBox( this );
	groupBox1a->setTitle( tr( "Subscript" ) );
	groupBox1aLayout = new QGridLayout( groupBox1a );
	groupBox1aLayout->setMargin(10);
	groupBox1aLayout->setSpacing(5);
	groupBox1aLayout->setAlignment( Qt::AlignTop );
	subDisplacement = new QSpinBox( groupBox1a );
	subDisplacement->setMaximum( 100 );
	subDisplacement->setSuffix( tr( " %" ) );
	groupBox1aLayout->addWidget( subDisplacement, 0, 1, Qt::AlignLeft );
	textLabel1a = new QLabel( tr( "&Displacement:" ), groupBox1a );
	textLabel1a->setBuddy(subDisplacement);
	groupBox1aLayout->addWidget( textLabel1a, 0, 0 );
	subScaling = new QSpinBox( groupBox1a );
	subScaling->setMaximum( 100 );
	subScaling->setMinimum( 1 );
	subScaling->setSuffix( tr( " %" ) );
	groupBox1aLayout->addWidget( subScaling, 0, 3, Qt::AlignLeft );
	textLabel2a = new QLabel( tr( "&Scaling:" ), groupBox1a );
	textLabel2a->setBuddy(subScaling);
	groupBox1aLayout->addWidget( textLabel2a, 0, 2 );
	tabTypoLayout->addWidget( groupBox1a, 0, 0, 1, 2 );

	groupBox2a = new QGroupBox( this );
	groupBox2a->setTitle( tr( "Superscript" ) );
	groupBox2aLayout = new QGridLayout( groupBox2a );
	groupBox2aLayout->setMargin(10);
	groupBox2aLayout->setSpacing(5);
	groupBox2aLayout->setAlignment( Qt::AlignTop );
	superDisplacement = new QSpinBox( groupBox2a );
	superDisplacement->setMaximum( 100 );
	superDisplacement->setSuffix( tr( " %" ) );
	groupBox2aLayout->addWidget( superDisplacement, 0, 1, Qt::AlignLeft );
	textLabel3a = new QLabel( tr( "D&isplacement:" ), groupBox2a );
	textLabel3a->setBuddy(superDisplacement);
	groupBox2aLayout->addWidget( textLabel3a, 0, 0 );
	superScaling = new QSpinBox( groupBox2a );
	superScaling->setMaximum( 100 );
	superScaling->setMinimum( 1 );
	superScaling->setSuffix( tr( " %" ) );
	groupBox2aLayout->addWidget( superScaling, 0, 3, Qt::AlignLeft );
	textLabel4a = new QLabel( tr( "S&caling:" ), groupBox2a );
	textLabel4a->setBuddy(superScaling);
	groupBox2aLayout->addWidget( textLabel4a, 0, 2 );
	tabTypoLayout->addWidget( groupBox2a, 1, 0, 1, 2 );

	underlineGroup = new QGroupBox( this );
	underlineGroup->setTitle( tr( "Underline" ) );
	underlineGroupLayout = new QGridLayout( underlineGroup );
	underlineGroupLayout->setMargin(10);
	underlineGroupLayout->setSpacing(5);
	underlineGroupLayout->setAlignment( Qt::AlignTop );
	textLabel1 = new QLabel( underlineGroup );
	textLabel1->setText( tr( "Displacement:" ) );
	underlineGroupLayout->addWidget( textLabel1, 0, 0 );
	underlinePos = new ScrSpinBox( -0.1, 100, underlineGroup, 7 );
	underlinePos->setWrapping(true);
	underlinePos->setSuffix( tr( " %" ) );
	underlinePos->setSpecialValueText( tr( "Auto" ) );
	underlineGroupLayout->addWidget( underlinePos, 0, 1, Qt::AlignLeft );
	textLabel2 = new QLabel( underlineGroup );
	textLabel2->setText( tr( "Line Width:" ) );
	underlineGroupLayout->addWidget( textLabel2, 0, 2 );
	underlineWidth = new ScrSpinBox( -0.1, 100, underlineGroup, 7 );
	underlineWidth->setSuffix( tr( " %" ) );
	underlineWidth->setWrapping(true);
	underlineWidth->setSpecialValueText( tr( "Auto" ) );
	underlineGroupLayout->addWidget( underlineWidth, 0, 3, Qt::AlignLeft );
	tabTypoLayout->addWidget( underlineGroup, 2, 0, 1, 2 );

	strikethruGroup = new QGroupBox( this );
	strikethruGroup->setTitle( tr( "Strikethru" ) );
	strikethruGroupLayout = new QGridLayout( strikethruGroup );
	strikethruGroupLayout->setMargin(10);
	strikethruGroupLayout->setSpacing(5);
	strikethruGroupLayout->setAlignment( Qt::AlignTop );
	textLabel1_2 = new QLabel( strikethruGroup );
	textLabel1_2->setText( tr( "Displacement:" ) );
	strikethruGroupLayout->addWidget( textLabel1_2, 0, 0 );
	strikethruPos = new ScrSpinBox( -0.1, 100, strikethruGroup, 7 );
	strikethruPos->setSuffix( tr( " %" ) );
	strikethruPos->setSpecialValueText( tr( "Auto" ) );
	strikethruPos->setWrapping(true);
	strikethruGroupLayout->addWidget( strikethruPos, 0, 1, Qt::AlignLeft );
	textLabel2_2 = new QLabel( strikethruGroup );
	textLabel2_2->setText( tr( "Line Width:" ) );
	strikethruGroupLayout->addWidget( textLabel2_2, 0, 2 );
	strikethruWidth = new ScrSpinBox( -0.1, 100, strikethruGroup, 7 );
	strikethruWidth->setWrapping(true);
	strikethruWidth->setSuffix( tr( " %" ) );
	strikethruWidth->setSpecialValueText( tr( "Auto" ) );
	strikethruGroupLayout->addWidget( strikethruWidth, 0, 3, Qt::AlignLeft );
	tabTypoLayout->addWidget( strikethruGroup, 3, 0, 1, 2 );

	groupBox3a = new QGroupBox( this );
	groupBox3a->setTitle( tr( "Small Caps" ) );
	groupBox3aLayout = new QGridLayout( groupBox3a );
	groupBox3aLayout->setMargin(10);
	groupBox3aLayout->setSpacing(5);
	groupBox3aLayout->setAlignment( Qt::AlignTop );
	capsScaling = new QSpinBox( groupBox3a );
	capsScaling->setMaximum( 100 );
	capsScaling->setMinimum( 1 );
	capsScaling->setSuffix( tr( " %" ) );
	groupBox3aLayout->addWidget( capsScaling, 0, 1, Qt::AlignLeft );
	textLabel5a = new QLabel( tr( "Sc&aling:" ), groupBox3a );
	textLabel5a->setBuddy(capsScaling);
	groupBox3aLayout->addWidget( textLabel5a, 0, 0 );
	tabTypoLayout->addWidget( groupBox3a, 4, 0 );

	groupBox4a = new QGroupBox( this );
	groupBox4a->setTitle( tr( "Automatic &Line Spacing" ) );
	groupBox4aLayout = new QGridLayout( groupBox4a );
	groupBox4aLayout->setMargin(10);
	groupBox4aLayout->setSpacing(5);
	groupBox4aLayout->setAlignment( Qt::AlignTop );
	autoLine = new QSpinBox( groupBox4a );
	autoLine->setMaximum( 100 );
	autoLine->setMinimum( 1 );
	autoLine->setSuffix( tr( " %" ) );
	groupBox4aLayout->addWidget( autoLine, 1, 1, Qt::AlignLeft );
	textLabel8a = new QLabel( tr( "Line Spacing:" ), groupBox4a );
	textLabel8a->setBuddy(autoLine);
	groupBox4aLayout->addWidget( textLabel8a, 1, 0 );
	tabTypoLayout->addWidget( groupBox4a, 4, 1 );
	// switched off as it's called in main prefs classes - PV
	//restoreDefaults(prefsData);
	superDisplacement->setToolTip( tr( "Displacement above the baseline of the font on a line" ) );
	superScaling->setToolTip( tr( "Relative size of the superscript compared to the normal font" ) );
	subDisplacement->setToolTip( tr( "Displacement below the baseline of the normal font on a line" ) );
	subScaling->setToolTip( tr( "Relative size of the subscript compared to the normal font" ) );
	capsScaling->setToolTip( tr( "Relative size of the small caps font compared to the normal font" ) );
	autoLine->setToolTip( tr( "Percentage increase over the font size for the line spacing" ) );
	underlinePos->setToolTip( tr( "Displacement below the baseline of the normal font expressed as a percentage of the fonts descender" ) );
	underlineWidth->setToolTip( tr( "Line width expressed as a percentage of the font size" ) );
	strikethruPos->setToolTip( tr( "Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender" ) );
	strikethruWidth->setToolTip( tr( "Line width expressed as a percentage of the font size" ) );
}

void TabTypograpy::restoreDefaults(struct typoPrefs *prefsData)
{
	capsScaling->setValue( prefsData->valueSmallCaps );
	subDisplacement->setValue( prefsData->valueSubScript );
	subScaling->setValue( prefsData->scalingSubScript );
	superDisplacement->setValue( prefsData->valueSuperScript);
	superScaling->setValue( prefsData->scalingSuperScript );
	autoLine->setValue( prefsData->autoLineSpacing );
	underlinePos->setValue(prefsData->valueUnderlinePos / 10.0);
	underlineWidth->setValue(prefsData->valueUnderlineWidth / 10.0);
	strikethruPos->setValue(prefsData->valueStrikeThruPos / 10.0);
	strikethruWidth->setValue(prefsData->valueStrikeThruWidth / 10.0);
}
