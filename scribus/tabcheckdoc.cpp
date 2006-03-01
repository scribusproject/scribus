/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include <qpushbutton.h>

#include "prefsstructs.h"

TabCheckDoc::TabCheckDoc( QWidget* parent, CheckerPrefsList prefsData, QString prefProfile ) : QWidget( parent, "tabcheckDoc", 0 )
{
	checkerProfile = prefsData;
	TabCheckDocLayout = new QVBoxLayout( this, 0, 5, "TabCheckDocLayout");
	TabCheckDocLayout->setAlignment( Qt::AlignTop );
	curCheckProfile = new QComboBox( true, this, "Profiles" );
	curCheckProfile->setEditable(true);
	curCheckProfile->setDuplicatesEnabled(false);
	CheckerPrefsList::Iterator it;
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
	rasterPDF = new QCheckBox( this, "rasterPDF" );
	rasterPDF->setText( tr( "Check for placed PDF Files" ) );
	TabCheckDocLayout->addWidget( rasterPDF );
	useAnnotations = new QCheckBox( this, "useAnnotations" );
	useAnnotations->setText( tr( "Check for PDF Annotations and Fields" ) );
	TabCheckDocLayout->addWidget( useAnnotations );
	layout1 = new QHBoxLayout( 0, 0, 5, "layout1");
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer );
	addProfile = new QPushButton( tr( "Add Profile" ), this, "addProfile" );
	layout1->addWidget( addProfile );
	removeProfile = new QPushButton( tr( "Remove Profile" ), this, "removeProfile" );
	layout1->addWidget( removeProfile );
	QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer2 );
	TabCheckDocLayout->addLayout( layout1 );
	ignoreErrors->setChecked(checkerProfile[prefProfile].ignoreErrors);
	automaticCheck->setChecked(checkerProfile[prefProfile].autoCheck);
	missingGlyphs->setChecked(checkerProfile[prefProfile].checkGlyphs);
	checkOrphans->setChecked(checkerProfile[prefProfile].checkOrphans);
	textOverflow->setChecked(checkerProfile[prefProfile].checkOverflow);
	tranparentObjects->setChecked(checkerProfile[prefProfile].checkTransparency);
	missingPictures->setChecked(checkerProfile[prefProfile].checkPictures);
	pictResolution->setChecked(checkerProfile[prefProfile].checkResolution);
	useAnnotations->setChecked(checkerProfile[prefProfile].checkAnnotations);
	rasterPDF->setChecked(checkerProfile[prefProfile].checkRasterPDF);
	resolutionValue->setValue( qRound(checkerProfile[prefProfile].minResolution) );
	currentProfile = prefProfile;
	if (checkerProfile.count() == 1)
		removeProfile->setEnabled(false);
	addProfile->setEnabled(false);
	tempNewProfileName="";
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(curCheckProfile, SIGNAL(textChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	
	connect(ignoreErrors, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(automaticCheck, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingGlyphs, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkOrphans, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(textOverflow, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(tranparentObjects, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingPictures, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(pictResolution, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	connect(useAnnotations, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(rasterPDF, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(resolutionValue, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	
	connect(removeProfile, SIGNAL(clicked()), this, SLOT(delProf()));
	connect(addProfile, SIGNAL(clicked()), this, SLOT(addProf()));
	clearWState( WState_Polished );
}

void TabCheckDoc::restoreDefaults()
{
}

void TabCheckDoc::putProfile()
{
	if (checkerProfile.contains(currentProfile))
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
		checkerProfile[currentProfile].checkAnnotations = useAnnotations->isChecked();
		checkerProfile[currentProfile].checkRasterPDF = rasterPDF->isChecked();
	}
}

void TabCheckDoc::setProfile(const QString& name)
{
	if (checkerProfile.contains(name))
	{
		putProfile();
		updateProfile(name);
		tempNewProfileName="";
		addProfile->setEnabled(false);
	}
	else
	{
		tempNewProfileName=name;
		addProfile->setEnabled(true);
	}
}

void TabCheckDoc::updateProfile(const QString& name)
{
	disconnect(ignoreErrors, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(automaticCheck, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(missingGlyphs, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkOrphans, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(missingPictures, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(pictResolution, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	disconnect(tranparentObjects, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(resolutionValue, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	disconnect(rasterPDF, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(useAnnotations, SIGNAL(clicked()), this, SLOT(putProfile()));
	ignoreErrors->setChecked(checkerProfile[name].ignoreErrors);
	automaticCheck->setChecked(checkerProfile[name].autoCheck);
	missingGlyphs->setChecked(checkerProfile[name].checkGlyphs);
	checkOrphans->setChecked(checkerProfile[name].checkOrphans);
	textOverflow->setChecked(checkerProfile[name].checkOverflow);
	tranparentObjects->setChecked(checkerProfile[name].checkTransparency);
	missingPictures->setChecked(checkerProfile[name].checkPictures);
	pictResolution->setChecked(checkerProfile[name].checkResolution);
	resolutionValue->setValue( qRound(checkerProfile[name].minResolution) );
	useAnnotations->setChecked(checkerProfile[name].checkAnnotations);
	rasterPDF->setChecked(checkerProfile[name].checkRasterPDF);
	currentProfile = name;
	connect(ignoreErrors, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(automaticCheck, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingGlyphs, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkOrphans, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingPictures, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(pictResolution, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	connect(tranparentObjects, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(resolutionValue, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	connect(rasterPDF, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(useAnnotations, SIGNAL(clicked()), this, SLOT(putProfile()));
}

void TabCheckDoc::addProf()
{
	struct checkerPrefs checkerSettings;
	checkerSettings.ignoreErrors = ignoreErrors->isChecked();
	checkerSettings.autoCheck = automaticCheck->isChecked();
	checkerSettings.checkGlyphs = missingGlyphs->isChecked();
	checkerSettings.checkOrphans = checkOrphans->isChecked();
	checkerSettings.checkOverflow = textOverflow->isChecked();
	checkerSettings.checkPictures = missingPictures->isChecked();
	checkerSettings.checkResolution = pictResolution->isChecked();
	checkerSettings.checkTransparency =  tranparentObjects->isChecked();
	checkerSettings.minResolution = resolutionValue->value();
	checkerSettings.checkAnnotations = useAnnotations->isChecked();
	checkerSettings.checkRasterPDF = rasterPDF->isChecked();
	checkerProfile.insert(tempNewProfileName, checkerSettings);
	currentProfile = tempNewProfileName;
	if (checkerProfile.count() > 1)
		removeProfile->setEnabled(true);
	addProfile->setEnabled(false);
	curCheckProfile->clear();
	
	disconnect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	disconnect(curCheckProfile, SIGNAL(textChanged(const QString&)), this, SLOT(setProfile(const QString&)));	
	CheckerPrefsList::Iterator it;
	int j,i=0;
	for (it = checkerProfile.begin(), j=0; it != checkerProfile.end(); ++it, ++j)
	{
		curCheckProfile->insertItem(it.key());
		if (it.key()==currentProfile)
			i=j;
	}
	curCheckProfile->setCurrentItem(i);
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(curCheckProfile, SIGNAL(textChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	tempNewProfileName="";
}

void TabCheckDoc::delProf()
{
	disconnect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	disconnect(curCheckProfile, SIGNAL(textChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	checkerProfile.remove(currentProfile);
	updateProfile(checkerProfile.begin().key());
	curCheckProfile->clear();
	CheckerPrefsList::Iterator it;
	CheckerPrefsList::Iterator itend=checkerProfile.end();
	for (it = checkerProfile.begin(); it != itend; ++it)
		curCheckProfile->insertItem(it.key());
	curCheckProfile->setCurrentText(currentProfile);
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(curCheckProfile, SIGNAL(textChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	if (checkerProfile.count() == 1)
		removeProfile->setEnabled(false);
}
