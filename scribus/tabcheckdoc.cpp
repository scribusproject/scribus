#include "tabcheckdoc.h"
#include "tabcheckdoc.moc"
#include <qvariant.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "scribusstructs.h"

TabCheckDoc::TabCheckDoc( QWidget* parent, struct checkerPrefs *prefsData ) : QWidget( parent, "tabcheckDoc", 0 )
{
	TabCheckDocLayout = new QVBoxLayout( this, 10, 5, "TabCheckDocLayout");
	TabCheckDocLayout->setAlignment( Qt::AlignTop );
	ignoreErrors = new QCheckBox( this, "ignoreErrors" );
	ignoreErrors->setText( tr( "Ignore all Errors" ) );
	ignoreErrors->setChecked(prefsData->ignoreErrors);
	TabCheckDocLayout->addWidget( ignoreErrors );
	automaticCheck = new QCheckBox( this, "automaticCheck" );
	automaticCheck->setText( tr( "Automatic Check before Printing or Exporting" ) );
	automaticCheck->setChecked(prefsData->autoCheck);
	TabCheckDocLayout->addWidget( automaticCheck );
	missingGlyphs = new QCheckBox( this, "missingGlyphs" );
	missingGlyphs->setText( tr( "Check for missing Glyphs" ) );
	missingGlyphs->setChecked(prefsData->checkGlyphs);
	TabCheckDocLayout->addWidget( missingGlyphs );
	checkOrphans = new QCheckBox( this, "checkOrphans" );
	checkOrphans->setText( tr( "Check for Objects not on a Page" ) );
	checkOrphans->setChecked(prefsData->checkOrphans);
	TabCheckDocLayout->addWidget( checkOrphans );
	textOverflow = new QCheckBox( this, "textOverflow" );
	textOverflow->setText( tr( "Check for overflow in Textframes" ) );
	textOverflow->setChecked(prefsData->checkOverflow);
	TabCheckDocLayout->addWidget( textOverflow );
	tranparentObjects = new QCheckBox( this, "tranparentObjects" );
	tranparentObjects->setText( tr( "Check for Transparencys used" ) );
	tranparentObjects->setChecked(prefsData->checkTransparency);
	TabCheckDocLayout->addWidget( tranparentObjects );
	missingPictures = new QCheckBox( this, "missingPictures" );
	missingPictures->setText( tr( "Check for missing Images" ) );
	missingPictures->setChecked(prefsData->checkPictures);
	TabCheckDocLayout->addWidget( missingPictures );
	pictResolution = new QGroupBox( this, "pictResolution" );
	pictResolution->setTitle( tr( "Check Image Resolution" ) );
	pictResolution->setCheckable( true );
	pictResolution->setChecked(prefsData->checkResolution);
	pictResolution->setColumnLayout(0, Qt::Vertical );
	pictResolution->layout()->setSpacing( 5 );
	pictResolution->layout()->setMargin( 10 );
	pictResolutionLayout = new QHBoxLayout( pictResolution->layout() );
	pictResolutionLayout->setAlignment( Qt::AlignTop );
	textLabel1 = new QLabel( pictResolution, "textLabel1" );
	textLabel1->setText( tr( "Lowest allowed Resolution" ) );
	pictResolutionLayout->addWidget( textLabel1 );
	resolutionValue = new QSpinBox( pictResolution, "resolutionValue" );
	resolutionValue->setMaxValue( 3600 );
	resolutionValue->setMinValue( 10 );
	resolutionValue->setValue( qRound(prefsData->minResolution) );
	resolutionValue->setSuffix( tr( " dpi" ) );
	pictResolutionLayout->addWidget( resolutionValue );
	TabCheckDocLayout->addWidget( pictResolution );
	clearWState( WState_Polished );
}
