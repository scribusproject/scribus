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
#include <qcombobox.h>

TabCheckDoc::TabCheckDoc( QWidget* parent, QMap<QString, checkerPrefs> prefsData, QString prefProfile ) : QWidget( parent, "tabcheckDoc", 0 )
{
	checkerProfile = prefsData;
	TabCheckDocLayout = new QVBoxLayout( this, 10, 5, "TabCheckDocLayout");
	TabCheckDocLayout->setAlignment( Qt::AlignTop );
	curCheckProfile = new QComboBox( true, this, "Profiles" );
	curCheckProfile->setEditable(false);
	QMap<QString, checkerPrefs>::Iterator it;
	for (it = checkerProfile.begin(); it != checkerProfile.end(); ++it)
	{
		curCheckProfile->insertItem(it.key());
	}
	curCheckProfile->setCurrentText(prefProfile);
	TabCheckDocLayout->addWidget( curCheckProfile );
	ignoreErrors = new QCheckBox( this, "ignoreErrors" );
	ignoreErrors->setText( tr( "Ignore all errors" ) );
	TabCheckDocLayout->addWidget( ignoreErrors );
	automaticCheck = new QCheckBox( this, "automaticCheck" );
	automaticCheck->setText( tr( "Automatic check before printing or exporting" ) );
	TabCheckDocLayout->addWidget( automaticCheck );
	missingGlyphs = new QCheckBox( this, "missingGlyphs" );
	missingGlyphs->setText( tr( "Check for missing glyphs" ) );
	TabCheckDocLayout->addWidget( missingGlyphs );
	checkOrphans = new QCheckBox( this, "checkOrphans" );
	checkOrphans->setText( tr( "Check for objects not on a page" ) );
	TabCheckDocLayout->addWidget( checkOrphans );
	textOverflow = new QCheckBox( this, "textOverflow" );
	textOverflow->setText( tr( "Check for overflow in text frames" ) );
	TabCheckDocLayout->addWidget( textOverflow );
	tranparentObjects = new QCheckBox( this, "tranparentObjects" );
	tranparentObjects->setText( tr( "Check for transparencies used" ) );
	TabCheckDocLayout->addWidget( tranparentObjects );
	missingPictures = new QCheckBox( this, "missingPictures" );
	missingPictures->setText( tr( "Check for missing images" ) );
	TabCheckDocLayout->addWidget( missingPictures );
	pictResolution = new QGroupBox( this, "pictResolution" );
	pictResolution->setTitle( tr( "Check image resolution" ) );
	pictResolution->setCheckable( true );
	pictResolution->setColumnLayout(0, Qt::Vertical );
	pictResolution->layout()->setSpacing( 5 );
	pictResolution->layout()->setMargin( 10 );
	pictResolutionLayout = new QHBoxLayout( pictResolution->layout() );
	pictResolutionLayout->setAlignment( Qt::AlignTop );
	textLabel1 = new QLabel( pictResolution, "textLabel1" );
	textLabel1->setText( tr( "Lowest allowed resolution" ) );
	pictResolutionLayout->addWidget( textLabel1 );
	resolutionValue = new QSpinBox( pictResolution, "resolutionValue" );
	resolutionValue->setMaxValue( 3600 );
	resolutionValue->setMinValue( 10 );
	resolutionValue->setSuffix( tr( " dpi" ) );
	pictResolutionLayout->addWidget( resolutionValue );
	TabCheckDocLayout->addWidget( pictResolution );
	ignoreErrors->setChecked(checkerProfile[prefProfile].ignoreErrors);
	automaticCheck->setChecked(checkerProfile[prefProfile].autoCheck);
	missingGlyphs->setChecked(checkerProfile[prefProfile].checkGlyphs);
	checkOrphans->setChecked(checkerProfile[prefProfile].checkOrphans);
	textOverflow->setChecked(checkerProfile[prefProfile].checkOverflow);
	tranparentObjects->setChecked(checkerProfile[prefProfile].checkTransparency);
	missingPictures->setChecked(checkerProfile[prefProfile].checkPictures);
	pictResolution->setChecked(checkerProfile[prefProfile].checkResolution);
	resolutionValue->setValue( qRound(checkerProfile[prefProfile].minResolution) );
	currentProfile = prefProfile;
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(ignoreErrors, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(automaticCheck, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingGlyphs, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkOrphans, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingPictures, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(pictResolution, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	connect(tranparentObjects, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(resolutionValue, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	clearWState( WState_Polished );
}

void TabCheckDoc::putProfile()
{
	checkerProfile[currentProfile].ignoreErrors = ignoreErrors->isChecked();
	checkerProfile[currentProfile].autoCheck = automaticCheck->isChecked();
	checkerProfile[currentProfile].checkGlyphs = missingGlyphs->isChecked();
	checkerProfile[currentProfile].checkOrphans = checkOrphans->isChecked();
	checkerProfile[currentProfile].checkOverflow = textOverflow->isChecked();
	checkerProfile[currentProfile].checkPictures = missingPictures->isChecked();
	checkerProfile[currentProfile].checkResolution = pictResolution->isChecked();
	checkerProfile[currentProfile].checkTransparency = tranparentObjects->isChecked();
	checkerProfile[currentProfile].minResolution = resolutionValue->value();
}

void TabCheckDoc::setProfile(const QString& name)
{
	disconnect(ignoreErrors, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(automaticCheck, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(missingGlyphs, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkOrphans, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(missingPictures, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(pictResolution, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	disconnect(tranparentObjects, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(resolutionValue, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	putProfile();
	ignoreErrors->setChecked(checkerProfile[name].ignoreErrors);
	automaticCheck->setChecked(checkerProfile[name].autoCheck);
	missingGlyphs->setChecked(checkerProfile[name].checkGlyphs);
	checkOrphans->setChecked(checkerProfile[name].checkOrphans);
	textOverflow->setChecked(checkerProfile[name].checkOverflow);
	tranparentObjects->setChecked(checkerProfile[name].checkTransparency);
	missingPictures->setChecked(checkerProfile[name].checkPictures);
	pictResolution->setChecked(checkerProfile[name].checkResolution);
	resolutionValue->setValue( qRound(checkerProfile[name].minResolution) );
	currentProfile = name;
	connect(ignoreErrors, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(automaticCheck, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingGlyphs, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkOrphans, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingPictures, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(pictResolution, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	connect(tranparentObjects, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(resolutionValue, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
}
