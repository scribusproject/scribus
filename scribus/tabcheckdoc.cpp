/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tabcheckdoc.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>

#include "prefsstructs.h"
#include "util.h"

TabCheckDoc::TabCheckDoc( QWidget* parent, CheckerPrefsList prefsData, QString prefProfile ) : QWidget( parent )
{
	TabCheckDocLayout = new QVBoxLayout(this);
	TabCheckDocLayout->setMargin(0);
	TabCheckDocLayout->setSpacing(5);
	TabCheckDocLayout->setAlignment( Qt::AlignTop );
	curCheckProfile = new QComboBox(this);
	curCheckProfile->setEditable(true);
	curCheckProfile->setDuplicatesEnabled(false);

	TabCheckDocLayout->addWidget( curCheckProfile );
	ignoreErrors = new QCheckBox( this );
	ignoreErrors->setText( tr( "Ignore all errors" ) );
	TabCheckDocLayout->addWidget( ignoreErrors );
	automaticCheck = new QCheckBox( this );
	automaticCheck->setText( tr( "Automatic check before printing or exporting" ) );
	TabCheckDocLayout->addWidget( automaticCheck );
	missingGlyphs = new QCheckBox( this );
	missingGlyphs->setText( tr( "Check for missing glyphs" ) );
	TabCheckDocLayout->addWidget( missingGlyphs );
	checkOrphans = new QCheckBox( this );
	checkOrphans->setText( tr( "Check for items not on a page" ) );
	TabCheckDocLayout->addWidget( checkOrphans );
	textOverflow = new QCheckBox( this );
	textOverflow->setText( tr( "Check for overflow in text frames" ) );
	TabCheckDocLayout->addWidget( textOverflow );
	tranparentObjects = new QCheckBox( this );
	tranparentObjects->setText( tr( "Check for used transparencies" ) );
	TabCheckDocLayout->addWidget( tranparentObjects );
	missingPictures = new QCheckBox( this );
	missingPictures->setText( tr( "Check for missing images" ) );
	TabCheckDocLayout->addWidget( missingPictures );
	pictResolution = new QGroupBox( this );
	pictResolution->setTitle( tr( "Check image resolution" ) );
	pictResolution->setCheckable( true );
	pictResolutionLayout = new QGridLayout(pictResolution);
	pictResolutionLayout->setMargin(10);
	pictResolutionLayout->setSpacing(5);
	pictResolutionLayout->setAlignment( Qt::AlignTop );
	textLabel1 = new QLabel( pictResolution );
	textLabel1->setText( tr( "Lowest allowed resolution" ) );
	pictResolutionLayout->addWidget( textLabel1, 0, 0 );
	resolutionValue = new QSpinBox( pictResolution );
	resolutionValue->setMaximum( 4800 );
	resolutionValue->setMinimum( 10 );
	resolutionValue->setSuffix( tr( " dpi" ) );
	pictResolutionLayout->addWidget( resolutionValue, 0, 1 );
	textLabel1m = new QLabel( pictResolution );
	textLabel1m->setText( tr( "Highest allowed resolution" ) );
	pictResolutionLayout->addWidget( textLabel1m, 0, 2 );
	resolutionValueM = new QSpinBox( pictResolution );
	resolutionValueM->setMaximum( 4800 );
	resolutionValueM->setMinimum( 10 );
	resolutionValueM->setSuffix( tr( " dpi" ) );
	pictResolutionLayout->addWidget( resolutionValueM, 0, 3 );
	TabCheckDocLayout->addWidget( pictResolution );
	rasterPDF = new QCheckBox( this );
	rasterPDF->setText( tr( "Check for placed PDF Files" ) );
	TabCheckDocLayout->addWidget( rasterPDF );
	checkForGIF = new QCheckBox(this);
	checkForGIF->setText( tr("Check for GIF images"));
	TabCheckDocLayout->addWidget(checkForGIF);
	useAnnotations = new QCheckBox( this );
	useAnnotations->setText( tr( "Check for PDF Annotations and Fields" ) );
	TabCheckDocLayout->addWidget( useAnnotations );
	ignoreOffLayers = new QCheckBox( this );
	ignoreOffLayers->setText( tr( "Ignore non-printable Layers" ) );
	TabCheckDocLayout->addWidget( ignoreOffLayers );
	checkOffConflictLayers = new QCheckBox( this );
	checkOffConflictLayers->setText( tr( "Check for Visible/Printable Mismatch in Layers" ) );
	TabCheckDocLayout->addWidget( checkOffConflictLayers );
	layout1 = new QHBoxLayout;
	layout1->setMargin(0);
	layout1->setSpacing(5);
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer );
	addProfile = new QPushButton( tr( "Add Profile" ), this );
	layout1->addWidget( addProfile );
	removeProfile = new QPushButton( tr( "Remove Profile" ), this );
	layout1->addWidget( removeProfile );
	QSpacerItem* spacer2 = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer2 );
	TabCheckDocLayout->addLayout( layout1 );

	// switched off as it's called in main prefs classes - PV
	//restoreDefaults(&prefsData, prefProfile);

	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(curCheckProfile, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	
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
	connect(checkForGIF, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(resolutionValue, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	connect(resolutionValueM, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	connect(ignoreOffLayers, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkOffConflictLayers, SIGNAL(clicked()), this, SLOT(putProfile()));	
	connect(removeProfile, SIGNAL(clicked()), this, SLOT(delProf()));
	connect(addProfile, SIGNAL(clicked()), this, SLOT(addProf()));
}

void TabCheckDoc::restoreDefaults(CheckerPrefsList *prefsData, QString prefProfile)
{
	checkerProfile = *prefsData;
	curCheckProfile->clear();
	CheckerPrefsList::Iterator it;
	curCheckProfile->clear();
	for (it = checkerProfile.begin(); it != checkerProfile.end(); ++it)
		curCheckProfile->addItem(it.key());
	setCurrentComboItem(curCheckProfile, prefProfile);
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
	checkForGIF->setChecked(checkerProfile[prefProfile].checkForGIF);
	ignoreOffLayers->setChecked(checkerProfile[prefProfile].ignoreOffLayers);
	checkOffConflictLayers->setChecked(checkerProfile[prefProfile].checkOffConflictLayers);
	resolutionValue->setValue( qRound(checkerProfile[prefProfile].minResolution) );
	resolutionValueM->setValue( qRound(checkerProfile[prefProfile].maxResolution) );
	currentProfile = prefProfile;
	if (checkerProfile.count() == 1)
		removeProfile->setEnabled(false);
	addProfile->setEnabled(false);
	tempNewProfileName="";
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
		checkerProfile[currentProfile].maxResolution = resolutionValueM->value();
		checkerProfile[currentProfile].checkAnnotations = useAnnotations->isChecked();
		checkerProfile[currentProfile].checkRasterPDF = rasterPDF->isChecked();
		checkerProfile[currentProfile].checkForGIF = checkForGIF->isChecked();
		checkerProfile[currentProfile].ignoreOffLayers = ignoreOffLayers->isChecked();
		checkerProfile[currentProfile].checkOffConflictLayers = checkOffConflictLayers->isChecked();
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
	disconnect(resolutionValueM, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	disconnect(rasterPDF, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkForGIF, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(useAnnotations, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(ignoreOffLayers, SIGNAL(clicked()), this, SLOT(putProfile()));
	disconnect(checkOffConflictLayers, SIGNAL(clicked()), this, SLOT(putProfile()));
	ignoreErrors->setChecked(checkerProfile[name].ignoreErrors);
	automaticCheck->setChecked(checkerProfile[name].autoCheck);
	missingGlyphs->setChecked(checkerProfile[name].checkGlyphs);
	checkOrphans->setChecked(checkerProfile[name].checkOrphans);
	textOverflow->setChecked(checkerProfile[name].checkOverflow);
	tranparentObjects->setChecked(checkerProfile[name].checkTransparency);
	missingPictures->setChecked(checkerProfile[name].checkPictures);
	pictResolution->setChecked(checkerProfile[name].checkResolution);
	resolutionValue->setValue( qRound(checkerProfile[name].minResolution) );
	resolutionValueM->setValue( qRound(checkerProfile[name].maxResolution) );
	useAnnotations->setChecked(checkerProfile[name].checkAnnotations);
	rasterPDF->setChecked(checkerProfile[name].checkRasterPDF);
	checkForGIF->setChecked(checkerProfile[name].checkForGIF);
	ignoreOffLayers->setChecked(checkerProfile[name].ignoreOffLayers);
	checkOffConflictLayers->setChecked(checkerProfile[name].checkOffConflictLayers);
	currentProfile = name;
	connect(ignoreErrors, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(automaticCheck, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingGlyphs, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkOrphans, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(missingPictures, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(pictResolution, SIGNAL(toggled(bool)), this, SLOT(putProfile()));
	connect(tranparentObjects, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(resolutionValue, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	connect(resolutionValueM, SIGNAL(valueChanged(int)), this, SLOT(putProfile()));
	connect(rasterPDF, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkForGIF, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(useAnnotations, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(ignoreOffLayers, SIGNAL(clicked()), this, SLOT(putProfile()));
	connect(checkOffConflictLayers, SIGNAL(clicked()), this, SLOT(putProfile()));
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
	checkerSettings.maxResolution = resolutionValueM->value();
	checkerSettings.checkAnnotations = useAnnotations->isChecked();
	checkerSettings.checkRasterPDF = rasterPDF->isChecked();
	checkerSettings.checkForGIF = checkForGIF->isChecked();
	checkerSettings.ignoreOffLayers = ignoreOffLayers->isChecked();
	checkerSettings.checkOffConflictLayers = checkOffConflictLayers->isChecked();
	checkerProfile.insert(tempNewProfileName, checkerSettings);
	currentProfile = tempNewProfileName;
	if (checkerProfile.count() > 1)
		removeProfile->setEnabled(true);
	addProfile->setEnabled(false);
	curCheckProfile->clear();
	
	disconnect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	disconnect(curCheckProfile, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));	
	CheckerPrefsList::Iterator it;
	int j,i=0;
	for (it = checkerProfile.begin(), j=0; it != checkerProfile.end(); ++it, ++j)
	{
		curCheckProfile->addItem(it.key());
		if (it.key()==currentProfile)
			i=j;
	}
	curCheckProfile->setCurrentIndex(i);
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(curCheckProfile, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	tempNewProfileName="";
}

void TabCheckDoc::delProf()
{
	disconnect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	disconnect(curCheckProfile, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	checkerProfile.remove(currentProfile);
	updateProfile(checkerProfile.begin().key());
	curCheckProfile->clear();
	CheckerPrefsList::Iterator it;
	CheckerPrefsList::Iterator itend=checkerProfile.end();
	for (it = checkerProfile.begin(); it != itend; ++it)
		curCheckProfile->addItem(it.key());
	setCurrentComboItem(curCheckProfile, currentProfile);
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(setProfile(const QString&)));
	connect(curCheckProfile, SIGNAL(editTextChanged(const QString&)), this, SLOT(setProfile(const QString&)));
	if (checkerProfile.count() == 1)
		removeProfile->setEnabled(false);
}
