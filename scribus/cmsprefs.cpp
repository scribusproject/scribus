/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmsprefs.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QToolTip>
#include <QLabel>


CMSPrefs::CMSPrefs( QWidget* parent, CMSData *Vor, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles)
	: QWidget( parent )
{
	prefs = Vor;
	changed = false;
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	cmsPrefsLayout = new QVBoxLayout( this );
	cmsPrefsLayout->setAlignment( Qt::AlignTop );
	cmsPrefsLayout->setSpacing( 5 );
	cmsPrefsLayout->setMargin( 0 );

	checkBox1 = new QCheckBox( tr( "&Activate Color Management" ), this );
	cmsPrefsLayout->addWidget( checkBox1 );

	sysProfiles = new QGroupBox( tr( "System Profiles" ), this);
	sysProfilesLayout = new QGridLayout(sysProfiles);
	sysProfilesLayout->setAlignment( Qt::AlignTop );
	sysProfilesLayout->setSpacing( 5 );
	sysProfilesLayout->setMargin( 10 );

	inputPRGBIm = new QComboBox( sysProfiles );
	inputPRGBIm->setMinimumSize( QSize( 190, 22 ) );
	inputPRGBIm->setEditable(false);
	text1 = new QLabel( tr( "&RGB Images:" ), sysProfiles );
	text1->setBuddy(inputPRGBIm);
	sysProfilesLayout->addWidget( text1, 0, 0 );
	sysProfilesLayout->addWidget( inputPRGBIm, 0, 1 );

	inputPCMYKIm = new QComboBox( sysProfiles );
	inputPCMYKIm->setMinimumSize( QSize( 190, 22 ) );
	inputPCMYKIm->setEditable(false);
	text1CMYK = new QLabel( tr( "&CMYK Images:" ), sysProfiles );
	text1CMYK->setBuddy(inputPCMYKIm);
	sysProfilesLayout->addWidget( text1CMYK, 1, 0 );
	sysProfilesLayout->addWidget( inputPCMYKIm, 1, 1 );

	inputPRGB = new QComboBox( sysProfiles );
	inputPRGB->setMinimumSize( QSize( 190, 22 ) );
	inputPRGB->setEditable(false);
	text4 = new QLabel( tr( "&RGB Solid Colors:" ), sysProfiles );
	text4->setBuddy(inputPRGB);
	sysProfilesLayout->addWidget( text4, 2, 0 );
	sysProfilesLayout->addWidget( inputPRGB, 2, 1 );

	inputPCMYK = new QComboBox( sysProfiles );
	inputPCMYK->setMinimumSize( QSize( 190, 22 ) );
	inputPCMYK->setEditable(false);
	text5 = new QLabel( tr( "&CMYK Solid Colors:" ), sysProfiles );
	text5->setBuddy(inputPCMYK);
	sysProfilesLayout->addWidget( text5, 3, 0 );
	sysProfilesLayout->addWidget( inputPCMYK, 3, 1 );

	monitorP = new QComboBox( sysProfiles );
	monitorP->setMinimumSize( QSize( 190, 22 ) );
	monitorP->setEditable(false);
	text2 = new QLabel( tr( "&Monitor:" ), sysProfiles );
	text2->setBuddy(monitorP);
	sysProfilesLayout->addWidget( text2, 4, 0 );
	sysProfilesLayout->addWidget( monitorP, 4, 1 );

	printerP = new QComboBox( sysProfiles );
	printerP->setMinimumSize( QSize( 190, 22 ) );
	printerP->setEditable(false);
	text3 = new QLabel( tr( "P&rinter:" ), sysProfiles );
	text3->setBuddy(printerP);
	sysProfilesLayout->addWidget( text3, 5, 0 );
	sysProfilesLayout->addWidget( printerP, 5, 1 );
	cmsPrefsLayout->addWidget( sysProfiles );

	render = new QGroupBox( tr( "Rendering Intents" ), this );
	renderLayout = new QGridLayout(render);
	renderLayout->setAlignment( Qt::AlignTop );
	renderLayout->setSpacing( 5 );
	renderLayout->setMargin( 10 );

	imagesI = new QComboBox( render );
	imagesI->setMinimumSize( QSize( 190, 22 ) );
	imagesI->setEditable(false);
	text21 = new QLabel( tr( "Images:" ), render );
	text21->setBuddy(imagesI);
	renderLayout->addWidget( text21, 0, 0 );
	renderLayout->addWidget( imagesI, 0, 1 );

	colorsI = new QComboBox( render );
	colorsI->setMinimumSize( QSize( 190, 22 ) );
	colorsI->setEditable(false);
	text22 = new QLabel( tr( "Sol&id Colors:" ), render );
	text22->setBuddy(colorsI);
	renderLayout->addWidget( text22, 1, 0 );
	renderLayout->addWidget( colorsI, 1, 1 );

	cmsPrefsLayout->addWidget( render );

	simulate = new QCheckBox( tr( "Sim&ulate Printer on the Screen" ), this );
	cmsPrefsLayout->addWidget( simulate );

	convertLayout = new QHBoxLayout;
	convertLayout->setAlignment( Qt::AlignTop );
	convertLayout->setMargin( 0 );
	convertLayout->addSpacing( 30 );

	convertAll = new QCheckBox( tr( "Convert all colors to printer space" ), this);
	convertLayout->addWidget( convertAll );
	cmsPrefsLayout->addLayout( convertLayout );

	gamutLayout = new QHBoxLayout;
	gamutLayout->setAlignment( Qt::AlignTop );
	gamutLayout->setMargin( 0 );
	gamutLayout->addSpacing( 30 );

	gamutC = new QCheckBox( tr( "Mark Colors out of &Gamut" ), this );
	gamutLayout->addWidget( gamutC );
	cmsPrefsLayout->addLayout( gamutLayout );

	blackP = new QCheckBox( tr( "Use &Blackpoint Compensation" ), this );
	cmsPrefsLayout->addWidget( blackP );
	// switched off as it's called in main prefs classes - PV
	restoreDefaults(prefs, InputProfiles, InputProfilesCMYK, PrinterProfiles, MonitorProfiles);

	inputPCMYKIm->setToolTip( tr( "Default color profile for imported CMYK images" ) );
	inputPRGBIm->setToolTip( tr( "Default color profile for imported RGB images" ) );
	inputPRGB->setToolTip( tr( "Default color profile for solid RGB colors on the page" ) );
	inputPCMYK->setToolTip( tr( "Default color profile for solid CMYK colors on the page" ) );
	monitorP->setToolTip( tr( "Color profile that you have generated or received from the manufacturer.\nThis profile should be specific to your monitor and not a generic profile (i.e. sRGB)." ) );
	printerP->setToolTip( tr( "Color profile for your printer model from the manufacturer.\nThis profile should be specific to your printer and not a generic profile (i.e. sRGB)." ) );
	colorsI->setToolTip( tr( "Default rendering intent for solid colors. Unless you know why to change it,\nRelative Colorimetric or Perceptual should be chosen." ) );
	imagesI->setToolTip( tr( "Default rendering intent for images. Unless you know why to change it,\nRelative Colorimetric or Perceptual should be chosen." ) );
	simulate->setToolTip( tr( "Enable 'soft proofing' of how your document colors will print,\nbased on the chosen printer profile." ) );
	convertAll->setToolTip( tr( "Simulate a full color managed environment :\nall colors, rgb or cmyk, are converted to printer color space." ) );
	gamutC->setToolTip( tr( "Method of showing colors on the screen which may not print properly.\nThis requires very accurate profiles and serves only as a warning." ) );
	blackP->setToolTip( tr( "Black Point Compensation is a method of improving contrast in photos.\nIt is recommended that you enable this if you have photos in your document." ) );

	// signals and slots connections
	connect( checkBox1, SIGNAL( clicked() ), this, SLOT( slotCMSon() ) );
	connect( simulate, SIGNAL( clicked() ), this, SLOT( slotSimula() ) );
}

void CMSPrefs::restoreDefaults(CMSData *prefs, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles)
{
	checkBox1->setChecked(prefs->CMSinUse);
	
	ProfilesL::Iterator it;
	inputPRGBIm->clear();
	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		inputPRGBIm->addItem(it.key());
		if (it.key() == prefs->DefaultImageRGBProfile)
			inputPRGBIm->setCurrentIndex(inputPRGBIm->count()-1);
	}
	inputPCMYKIm->clear();
	for (it = InputProfilesCMYK->begin(); it != InputProfilesCMYK->end(); ++it)
	{
		inputPCMYKIm->addItem(it.key());
		if (it.key() == prefs->DefaultImageCMYKProfile)
			inputPCMYKIm->setCurrentIndex(inputPCMYKIm->count()-1);
	}
	inputPRGB->clear();
	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		inputPRGB->addItem(it.key());
		if (it.key() == prefs->DefaultSolidColorRGBProfile)
			inputPRGB->setCurrentIndex(inputPRGB->count()-1);
	}
	inputPCMYK->clear();
	for (it = InputProfilesCMYK->begin(); it != InputProfilesCMYK->end(); ++it)
	{
		inputPCMYK->addItem(it.key());
		if (it.key() == prefs->DefaultSolidColorCMYKProfile)
			inputPCMYK->setCurrentIndex(inputPCMYK->count()-1);
	}
	monitorP->clear();
	for (it = MonitorProfiles->begin(); it != MonitorProfiles->end(); ++it)
	{
		monitorP->addItem(it.key());
		if (it.key() == prefs->DefaultMonitorProfile)
			monitorP->setCurrentIndex(monitorP->count()-1);
	}
	printerP->clear();
	for (it = PrinterProfiles->begin(); it != PrinterProfiles->end(); ++it)
	{
		printerP->addItem(it.key());
		if (it.key() == prefs->DefaultPrinterProfile)
			printerP->setCurrentIndex(printerP->count()-1);
	}

	QString tmp_mp[] = { tr("Perceptual"), tr("Relative Colorimetric"),
		tr("Saturation"), tr("Absolute Colorimetric")};
		size_t array = sizeof(tmp_mp) / sizeof(*tmp_mp);
	imagesI->clear();
	for (uint prop = 0; prop < array; ++prop)
		imagesI->addItem(tmp_mp[prop]);
	imagesI->setCurrentIndex(prefs->DefaultIntentImages);
	colorsI->clear();
	for (uint prop = 0; prop < array; ++prop)
		colorsI->addItem(tmp_mp[prop]);
	colorsI->setCurrentIndex(prefs->DefaultIntentColors);

	simulate->setChecked(prefs->SoftProofOn);
	convertAll->setChecked(prefs->SoftProofFullOn);
	gamutC->setChecked(prefs->GamutCheck);
	blackP->setChecked(prefs->BlackPoint);

	if (!checkBox1->isChecked())
	{
		simulate->setEnabled( false );
		convertAll->setEnabled( false );
		blackP->setEnabled( false );
		sysProfiles->setEnabled( false );
		render->setEnabled( false );
	}
	convertAll->setEnabled(simulate->isEnabled() && simulate->isChecked());
	gamutC->setEnabled(simulate->isEnabled() && simulate->isChecked());
}

void CMSPrefs::setValues()
{
	if ((prefs->DefaultImageRGBProfile != inputPRGBIm->currentText()) ||
		(prefs->DefaultImageCMYKProfile != inputPCMYKIm->currentText()) ||
		(prefs->DefaultSolidColorRGBProfile != inputPRGB->currentText()) ||
		(prefs->DefaultSolidColorCMYKProfile != inputPCMYK->currentText()) ||
		(prefs->DefaultMonitorProfile != monitorP->currentText()) ||
		(prefs->DefaultPrinterProfile != printerP->currentText()) ||
		(prefs->DefaultIntentColors != colorsI->currentIndex()) ||
		(prefs->DefaultIntentImages != imagesI->currentIndex()) ||
		(prefs->SoftProofOn != simulate->isChecked()) ||
		(prefs->SoftProofFullOn != convertAll->isChecked()) ||
		(prefs->GamutCheck != gamutC->isChecked()) ||
		(prefs->BlackPoint != blackP->isChecked()) ||
		(prefs->CMSinUse != checkBox1->isChecked()))
			changed = true;
	prefs->DefaultImageRGBProfile = inputPRGBIm->currentText();
	prefs->DefaultImageCMYKProfile = inputPCMYKIm->currentText();
	prefs->DefaultSolidColorRGBProfile = inputPRGB->currentText();
	prefs->DefaultSolidColorCMYKProfile = inputPCMYK->currentText();
	prefs->DefaultMonitorProfile = monitorP->currentText();
	prefs->DefaultPrinterProfile = printerP->currentText();
	prefs->DefaultIntentColors = colorsI->currentIndex();
	prefs->DefaultIntentImages = imagesI->currentIndex();
	prefs->SoftProofOn = simulate->isChecked();
	prefs->SoftProofFullOn = convertAll->isChecked();
	prefs->GamutCheck = gamutC->isChecked();
	prefs->CMSinUse = checkBox1->isChecked();
	prefs->BlackPoint = blackP->isChecked();
}

void CMSPrefs::updateDocSettings(ScribusDoc* doc)
{
	if ((doc->CMSSettings.DefaultImageRGBProfile != inputPRGBIm->currentText()) ||
		(doc->CMSSettings.DefaultImageCMYKProfile != inputPCMYKIm->currentText()) ||
		(doc->CMSSettings.DefaultSolidColorRGBProfile != inputPRGB->currentText()) ||
		(doc->CMSSettings.DefaultSolidColorCMYKProfile != inputPCMYK->currentText()) ||
		(doc->CMSSettings.DefaultMonitorProfile != monitorP->currentText()) ||
		(doc->CMSSettings.DefaultPrinterProfile != printerP->currentText()) ||
		(doc->CMSSettings.DefaultIntentColors != colorsI->currentIndex()) ||
		(doc->CMSSettings.DefaultIntentImages != imagesI->currentIndex()) ||
		(doc->CMSSettings.SoftProofOn != simulate->isChecked()) ||
		(doc->CMSSettings.SoftProofFullOn != convertAll->isChecked()) ||
		(doc->CMSSettings.GamutCheck != gamutC->isChecked()) ||
		(doc->CMSSettings.BlackPoint != blackP->isChecked()) ||
		(doc->CMSSettings.CMSinUse != checkBox1->isChecked()))
			changed = true;
	doc->CMSSettings.DefaultImageRGBProfile = inputPRGBIm->currentText();
	doc->CMSSettings.DefaultImageCMYKProfile = inputPCMYKIm->currentText();
	doc->CMSSettings.DefaultSolidColorRGBProfile = inputPRGB->currentText();
	doc->CMSSettings.DefaultSolidColorCMYKProfile = inputPCMYK->currentText();
	doc->CMSSettings.DefaultMonitorProfile = monitorP->currentText();
	doc->CMSSettings.DefaultPrinterProfile = printerP->currentText();
	doc->CMSSettings.DefaultIntentColors = colorsI->currentIndex();
	doc->CMSSettings.DefaultIntentImages = imagesI->currentIndex();
	doc->CMSSettings.SoftProofOn = simulate->isChecked();
	doc->CMSSettings.SoftProofFullOn = convertAll->isChecked();
	doc->CMSSettings.GamutCheck = gamutC->isChecked();
	doc->CMSSettings.CMSinUse = checkBox1->isChecked();
	doc->CMSSettings.BlackPoint = blackP->isChecked();
}

void CMSPrefs::slotSimula()
{
	bool setter = simulate->isChecked() ? true : false;
	convertAll->setEnabled(setter);
	if (!setter)
		convertAll->setChecked(false);
	gamutC->setEnabled(setter);
}

void CMSPrefs::slotCMSon()
{
	bool setter = checkBox1->isChecked() ? true : false;
	sysProfiles->setEnabled(setter);
	render->setEnabled(setter);
	simulate->setEnabled(setter);
	convertAll->setEnabled(setter);
	blackP->setEnabled(setter);
	if (setter == true)
		slotSimula();
	else
		gamutC->setEnabled(setter);
	emit cmsOn(setter);
}

