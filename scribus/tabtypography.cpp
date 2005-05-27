#include "tabtypography.h"
#include "tabtypography.moc"
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include "mspinbox.h"
#include "scribusstructs.h"

TabTypograpy::TabTypograpy( QWidget* parent, struct typoPrefs *prefsData) : QWidget( parent, "tabtypo", 0 )
{
	tabTypoLayout = new QGridLayout( this, 1, 1, 0, 6, "tabTypoLayout");
	groupBox1a = new QGroupBox( this, "groupBox1a" );
	groupBox1a->setColumnLayout(0, Qt::Vertical );
	groupBox1a->layout()->setSpacing( 6 );
	groupBox1a->layout()->setMargin( 11 );
	groupBox1a->setTitle( tr( "Subscript" ) );
	groupBox1aLayout = new QGridLayout( groupBox1a->layout() );
	groupBox1aLayout->setAlignment( Qt::AlignTop );
	subDisplacement = new QSpinBox( groupBox1a, "subDisplacement" );
	subDisplacement->setMaxValue( 100 );
	subDisplacement->setSuffix( tr( " %" ) );
	groupBox1aLayout->addWidget( subDisplacement, 0, 1, Qt::AlignLeft );
	textLabel1a = new QLabel(subDisplacement, tr( "&Displacement:" ), groupBox1a, "textLabel1a" );
	groupBox1aLayout->addWidget( textLabel1a, 0, 0 );
	subScaling = new QSpinBox( groupBox1a, "subScaling" );
	subScaling->setMaxValue( 100 );
	subScaling->setMinValue( 1 );
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
	superDisplacement->setSuffix( tr( " %" ) );
	groupBox2aLayout->addWidget( superDisplacement, 0, 1, Qt::AlignLeft );
	textLabel3a = new QLabel(superDisplacement, tr( "D&isplacement:" ), groupBox2a, "textLabel3a" );
	groupBox2aLayout->addWidget( textLabel3a, 0, 0 );
	superScaling = new QSpinBox( groupBox2a, "superScaling" );
	superScaling->setMaxValue( 100 );
	superScaling->setMinValue( 1 );
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
	capsScaling->setSuffix( tr( " %" ) );
	groupBox3aLayout->addWidget( capsScaling, 0, 1, Qt::AlignLeft );
	textLabel5a = new QLabel(capsScaling, tr( "Sc&aling:" ), groupBox3a, "textLabel5a" );
	groupBox3aLayout->addWidget( textLabel5a, 0, 0 );
	tabTypoLayout->addWidget( groupBox3a, 2, 0 );

	underlineGroup = new QGroupBox( this, "underlineGroup" );
	underlineGroup->setTitle( tr( "Underline" ) );
	underlineGroup->setColumnLayout(0, Qt::Vertical );
	underlineGroup->layout()->setSpacing( 6 );
	underlineGroup->layout()->setMargin( 11 );
	underlineGroupLayout = new QGridLayout( underlineGroup->layout() );
	underlineGroupLayout->setAlignment( Qt::AlignTop );
	textLabel1 = new QLabel( underlineGroup, "textLabel1" );
	textLabel1->setText( tr( "Displacement:" ) );
	underlineGroupLayout->addWidget( textLabel1, 0, 0 );
	underlinePos = new MSpinBox( -0.1, 100, underlineGroup, 1 );
	underlinePos->setWrapping(true);
	underlinePos->setSuffix( tr( " %" ) );
	underlinePos->setSpecialValueText( tr( "Auto" ) );
	underlineGroupLayout->addWidget( underlinePos, 0, 1 );
	textLabel2 = new QLabel( underlineGroup, "textLabel2" );
	textLabel2->setText( tr( "Line Width:" ) );
	underlineGroupLayout->addWidget( textLabel2, 0, 2 );
	underlineWidth = new MSpinBox( -0.1, 100, underlineGroup, 1 );
	underlineWidth->setSuffix( tr( " %" ) );
	underlineWidth->setWrapping(true);
	underlineWidth->setSpecialValueText( tr( "Auto" ) );
	underlineGroupLayout->addWidget( underlineWidth, 0, 3 );
	tabTypoLayout->addWidget( underlineGroup, 3, 0 );

	strikethruGroup = new QGroupBox( this, "strikethruGroup" );
	strikethruGroup->setTitle( tr( "Strikethru" ) );
	strikethruGroup->setColumnLayout(0, Qt::Vertical );
	strikethruGroup->layout()->setSpacing( 6 );
	strikethruGroup->layout()->setMargin( 11 );
	strikethruGroupLayout = new QGridLayout( strikethruGroup->layout() );
	strikethruGroupLayout->setAlignment( Qt::AlignTop );
	textLabel1_2 = new QLabel( strikethruGroup, "textLabel1" );
	textLabel1_2->setText( tr( "Displacement:" ) );
	strikethruGroupLayout->addWidget( textLabel1_2, 0, 0 );
	strikethruPos = new MSpinBox( -0.1, 100, strikethruGroup, 1 );
	strikethruPos->setSuffix( tr( " %" ) );
	strikethruPos->setSpecialValueText( tr( "Auto" ) );
	strikethruPos->setWrapping(true);
	strikethruGroupLayout->addWidget( strikethruPos, 0, 1 );
	textLabel2_2 = new QLabel( strikethruGroup, "textLabel2" );
	textLabel2_2->setText( tr( "Line Width:" ) );
	strikethruGroupLayout->addWidget( textLabel2_2, 0, 2 );
	strikethruWidth = new MSpinBox( -0.1, 100, strikethruGroup, 1 );
	strikethruWidth->setWrapping(true);
	strikethruWidth->setSuffix( tr( " %" ) );
	strikethruWidth->setSpecialValueText( tr( "Auto" ) );
	strikethruGroupLayout->addWidget( strikethruWidth, 0, 3 );
	tabTypoLayout->addWidget( strikethruGroup, 4, 0 );

	groupBox4a = new QGroupBox( this, "groupBox3a" );
	groupBox4a->setColumnLayout(0, Qt::Vertical );
	groupBox4a->layout()->setSpacing( 6 );
	groupBox4a->layout()->setMargin( 11 );
	groupBox4a->setTitle( tr( "Automatic &Line Spacing" ) );
	groupBox4aLayout = new QGridLayout( groupBox4a->layout() );
	groupBox4aLayout->setAlignment( Qt::AlignTop );
	autoLine = new QSpinBox( groupBox4a, "autoLine" );
	autoLine->setMaxValue( 100 );
	autoLine->setMinValue( 1 );
	autoLine->setSuffix( tr( " %" ) );
	groupBox4aLayout->addWidget( autoLine, 1, 1 );
	textLabel8a = new QLabel( autoLine, tr( "Line Spacing:" ), groupBox4a, "textLabel8a" );
	groupBox4aLayout->addWidget( textLabel8a, 1, 0 );
	tabTypoLayout->addWidget( groupBox4a, 5, 0 );
	restoreDefaults(prefsData);
	QToolTip::add( superDisplacement, tr( "Displacement above the baseline of the font on a line" ) );
	QToolTip::add( superScaling, tr( "Relative size of the superscript compared to the normal font" ) );
	QToolTip::add( subDisplacement, tr( "Displacement below the baseline of the normal font on a line" ) );
	QToolTip::add( subScaling, tr( "Relative size of the subscript compared to the normal font" ) );
	QToolTip::add( capsScaling, tr( "Relative size of the small caps font compared to the normal font" ) );
	QToolTip::add( autoLine, tr( "Percentage increase over the font size for the line spacing" ) );
	QToolTip::add( underlinePos, tr( "Displacement below the baseline of the normal font expressed as a percentage of the fonts descender" ) );
	QToolTip::add( underlineWidth, tr( "Line width expressed as a percentage of the font size" ) );
	QToolTip::add( strikethruPos, tr( "Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender" ) );
	QToolTip::add( strikethruWidth, tr( "Line width expressed as a percentage of the font size" ) );
}

void TabTypograpy::restoreDefaults(struct typoPrefs *prefsData)
{
	subDisplacement->setValue( prefsData->valueSubScript );
	capsScaling->setValue( prefsData->valueSmallCaps );
	superScaling->setValue( prefsData->scalingSuperScript );
	subScaling->setValue( prefsData->scalingSubScript );
	autoLine->setValue( prefsData->autoLineSpacing );
	underlinePos->setValue(prefsData->valueUnderlinePos);
	underlineWidth->setValue(prefsData->valueUnderlineWidth);
	strikethruPos->setValue(prefsData->valueStrikeThruPos);
	strikethruWidth->setValue(prefsData->valueStrikeThruWidth);
}
