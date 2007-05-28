/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmsprefs.h"
//#include "cmsprefs.moc"
#include "scribusdoc.h"
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

CMSPrefs::CMSPrefs( QWidget* parent, CMSData *Vor, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles)
	: QWidget( parent, "CMS" )
{
	prefs = Vor;
	changed = false;
	cmsPrefsLayout = new Q3VBoxLayout( this ); 
	cmsPrefsLayout->setAlignment( Qt::AlignTop );
	cmsPrefsLayout->setSpacing( 5 );
	cmsPrefsLayout->setMargin( 0 );

	checkBox1 = new QCheckBox( tr( "&Activate Color Management" ), this, "CheckBox1" );
// 	checkBox1->setChecked(prefs->CMSinUse);
	cmsPrefsLayout->addWidget( checkBox1 );

	sysProfiles = new Q3GroupBox( tr( "System Profiles" ), this, "SysProfiles" );
// 	if (!checkBox1->isChecked())
// 		sysProfiles->setEnabled( false );
	sysProfiles->setColumnLayout(0, Qt::Vertical );
	sysProfiles->layout()->setSpacing( 0 );
	sysProfiles->layout()->setMargin( 0 );
	sysProfilesLayout = new Q3GridLayout( sysProfiles->layout() );
	sysProfilesLayout->setAlignment( Qt::AlignTop );
	sysProfilesLayout->setSpacing( 5 );
	sysProfilesLayout->setMargin( 10 );

// 	ProfilesL::Iterator it;

	inputPRGBIm = new QComboBox( true, sysProfiles, "InputP" );
	inputPRGBIm->setMinimumSize( QSize( 190, 22 ) );
	inputPRGBIm->setEditable(false);
/*	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		inputPRGBIm->insertItem(it.key());
		if (it.key() == prefs->DefaultImageRGBProfile)
			inputPRGBIm->setCurrentItem(inputPRGBIm->count()-1);
	}*/
	text1 = new QLabel( inputPRGBIm, tr( "&RGB Pictures:" ), sysProfiles, "Text1" );
	sysProfilesLayout->addWidget( text1, 0, 0 );
	sysProfilesLayout->addWidget( inputPRGBIm, 0, 1 );

	inputPCMYKIm = new QComboBox( true, sysProfiles, "inputPCMYKIm" );
	inputPCMYKIm->setMinimumSize( QSize( 190, 22 ) );
	inputPCMYKIm->setEditable(false);
/*	for (it = InputProfilesCMYK->begin(); it != InputProfilesCMYK->end(); ++it)
	{
		inputPCMYKIm->insertItem(it.key());
		if (it.key() == prefs->DefaultImageCMYKProfile)
			inputPCMYKIm->setCurrentItem(inputPCMYKIm->count()-1);
	}*/
	text1CMYK = new QLabel( inputPCMYKIm, tr( "&CMYK Pictures:" ), sysProfiles, "text1cmyk" );
	sysProfilesLayout->addWidget( text1CMYK, 1, 0 );
	sysProfilesLayout->addWidget( inputPCMYKIm, 1, 1 );

	inputPRGB = new QComboBox( true, sysProfiles, "inputPRGB" );
	inputPRGB->setMinimumSize( QSize( 190, 22 ) );
	inputPRGB->setEditable(false);
/*	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		inputPRGB->insertItem(it.key());
		if (it.key() == prefs->DefaultSolidColorRGBProfile)
			inputPRGB->setCurrentItem(inputPRGB->count()-1);
	}*/
	text4 = new QLabel( inputPRGB, tr( "&RGB Solid Colors:" ), sysProfiles, "Text4" );
	sysProfilesLayout->addWidget( text4, 2, 0 );
	sysProfilesLayout->addWidget( inputPRGB, 2, 1 );

	inputPCMYK = new QComboBox( true, sysProfiles, "inputPCMYK" );
	inputPCMYK->setMinimumSize( QSize( 190, 22 ) );
	inputPCMYK->setEditable(false);
/*	for (it = InputProfilesCMYK->begin(); it != InputProfilesCMYK->end(); ++it)
	{
		inputPCMYK->insertItem(it.key());
		if (it.key() == prefs->DefaultSolidColorCMYKProfile)
			inputPCMYK->setCurrentItem(inputPCMYK->count()-1);
	}*/
	text5 = new QLabel( inputPCMYK, tr( "&CMYK Solid Colors:" ), sysProfiles, "Text5" );
	sysProfilesLayout->addWidget( text5, 3, 0 );
	sysProfilesLayout->addWidget( inputPCMYK, 3, 1 );

	monitorP = new QComboBox( true, sysProfiles, "MonitorP" );
	monitorP->setMinimumSize( QSize( 190, 22 ) );
	monitorP->setEditable(false);
/*	for (it = MonitorProfiles->begin(); it != MonitorProfiles->end(); ++it)
	{
		monitorP->insertItem(it.key());
		if (it.key() == prefs->DefaultMonitorProfile)
			monitorP->setCurrentItem(monitorP->count()-1);
	}*/
	text2 = new QLabel( monitorP, tr( "&Monitor:" ), sysProfiles, "Text2" );
	sysProfilesLayout->addWidget( text2, 4, 0 );
	sysProfilesLayout->addWidget( monitorP, 4, 1 );

	printerP = new QComboBox( true, sysProfiles, "PrinterP" );
	printerP->setMinimumSize( QSize( 190, 22 ) );
	printerP->setEditable(false);
/*	for (it = PrinterProfiles->begin(); it != PrinterProfiles->end(); ++it)
	{
		printerP->insertItem(it.key());
		if (it.key() == prefs->DefaultPrinterProfile)
			printerP->setCurrentItem(printerP->count()-1);
	}*/
	text3 = new QLabel( printerP, tr( "P&rinter:" ), sysProfiles, "Text3" );
	sysProfilesLayout->addWidget( text3, 5, 0 );
	sysProfilesLayout->addWidget( printerP, 5, 1 );
	cmsPrefsLayout->addWidget( sysProfiles );

	render = new Q3GroupBox( tr( "Rendering Intents" ), this, "Render" );
/*	if (!checkBox1->isChecked())
		render->setEnabled( false );*/
	render->setColumnLayout(0, Qt::Vertical );
	render->layout()->setSpacing( 0 );
	render->layout()->setMargin( 0 );
	renderLayout = new Q3GridLayout( render->layout() );
	renderLayout->setAlignment( Qt::AlignTop );
	renderLayout->setSpacing( 5 );
	renderLayout->setMargin( 10 );

	imagesI = new QComboBox( true, render, "ImagesI" );
// 	QString tmp_mp[] = { tr("Perceptual"), tr("Relative Colorimetric"), 
// 						tr("Saturation"), tr("Absolute Colorimetric")};
// 	size_t array = sizeof(tmp_mp) / sizeof(*tmp_mp);
	/* PFJ - 29.02.04 - Changed from uint to int and var name */
/*	for (uint prop = 0; prop < array; ++prop)
		imagesI->insertItem(tmp_mp[prop]);*/
	imagesI->setMinimumSize( QSize( 190, 22 ) );
	imagesI->setEditable(false);
// 	imagesI->setCurrentItem(prefs->DefaultIntentImages);
	text21 = new QLabel( imagesI, tr( "Pictures:" ), render, "Text21" );
	renderLayout->addWidget( text21, 0, 0 );
	renderLayout->addWidget( imagesI, 0, 1 );

	colorsI = new QComboBox( true, render, "colorsI" );
/*	for (uint prop = 0; prop < array; ++prop)
		colorsI->insertItem(tmp_mp[prop]);*/
	colorsI->setMinimumSize( QSize( 190, 22 ) );
	colorsI->setEditable(false);
// 	colorsI->setCurrentItem(prefs->DefaultIntentColors);
	text22 = new QLabel( colorsI, tr( "Sol&id Colors:" ), render, "Text22" );
	renderLayout->addWidget( text22, 1, 0 );
	renderLayout->addWidget( colorsI, 1, 1 );

	cmsPrefsLayout->addWidget( render );

	simulate = new QCheckBox( tr( "Sim&ulate Printer on the Screen" ), this, "Simulate" );
// 	simulate->setChecked(prefs->SoftProofOn);
	cmsPrefsLayout->addWidget( simulate );

	convertLayout = new Q3HBoxLayout( cmsPrefsLayout );
	convertLayout->setAlignment( Qt::AlignTop );
	convertLayout->setMargin( 0 );
	convertLayout->addSpacing( 30 );

	convertAll = new QCheckBox( tr( "Convert all colors to printer space" ), this, "fullSimulate" );
// 	convertAll->setChecked(prefs->SoftProofFullOn);
	convertLayout->addWidget( convertAll );

	gamutLayout = new Q3HBoxLayout( cmsPrefsLayout );
	gamutLayout->setAlignment( Qt::AlignTop );
	gamutLayout->setMargin( 0 );
	gamutLayout->addSpacing( 30 );

	gamutC = new QCheckBox( tr( "Mark Colors out of &Gamut" ), this, "Gamut" );
// 	gamutC->setChecked(prefs->GamutCheck);
	gamutLayout->addWidget( gamutC );

	blackP = new QCheckBox( tr( "Use &Blackpoint Compensation" ), this, "Black" );
// 	blackP->setChecked(prefs->BlackPoint);
	cmsPrefsLayout->addWidget( blackP );

/*	if (!checkBox1->isChecked())
	{
		simulate->setEnabled( false );
		convertAll->setEnabled( false );
		//gamutC->setEnabled( false );
		blackP->setEnabled( false );
	}*/
	
// 	convertAll->setEnabled(simulate->isEnabled() && simulate->isChecked());
// 	gamutC->setEnabled(simulate->isEnabled() && simulate->isChecked());

	// switched off as it's called in main prefs classes - PV
	restoreDefaults(prefs, InputProfiles, InputProfilesCMYK, PrinterProfiles, MonitorProfiles);

	QToolTip::add( inputPCMYKIm, tr( "Default color profile for imported CMYK images" ) );
	QToolTip::add( inputPRGBIm, tr( "Default color profile for imported RGB images" ) );
	QToolTip::add( inputPRGB, tr( "Default color profile for solid RGB colors on the page" ) );
	QToolTip::add( inputPCMYK, tr( "Default color profile for solid CMYK colors on the page" ) );
	QToolTip::add( monitorP, tr( "Color profile that you have generated or received from the manufacturer.\nThis profile should be specific to your monitor and not a generic profile (i.e. sRGB)." ) );
	QToolTip::add( printerP, tr( "Color profile for your printer model from the manufacturer.\nThis profile should be specific to your printer and not a generic profile (i.e. sRGB)." ) );
	QToolTip::add( colorsI, tr( "Default rendering intent for solid colors. Unless you know why to change it,\nRelative Colorimetric or Perceptual should be chosen." ) );
	QToolTip::add( imagesI, tr( "Default rendering intent for images. Unless you know why to change it,\nRelative Colorimetric or Perceptual should be chosen." ) );
	QToolTip::add( simulate, tr( "Enable 'soft proofing' of how your document colors will print,\nbased on the chosen printer profile." ) );
	QToolTip::add( convertAll, tr( "Simulate a full color managed environment :\nall colors, rgb or cmyk, are converted to printer color space." ) );
	QToolTip::add( gamutC, tr( "Method of showing colors on the screen which may not print properly.\nThis requires very accurate profiles and serves only as a warning." ) );
	QToolTip::add( blackP, tr( "Black Point Compensation is a method of improving contrast in photos.\nIt is recommended that you enable this if you have photos in your document." ) );

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
		inputPRGBIm->insertItem(it.key());
		if (it.key() == prefs->DefaultImageRGBProfile)
			inputPRGBIm->setCurrentItem(inputPRGBIm->count()-1);
	}
	inputPCMYKIm->clear();
	for (it = InputProfilesCMYK->begin(); it != InputProfilesCMYK->end(); ++it)
	{
		inputPCMYKIm->insertItem(it.key());
		if (it.key() == prefs->DefaultImageCMYKProfile)
			inputPCMYKIm->setCurrentItem(inputPCMYKIm->count()-1);
	}
	inputPRGB->clear();
	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		inputPRGB->insertItem(it.key());
		if (it.key() == prefs->DefaultSolidColorRGBProfile)
			inputPRGB->setCurrentItem(inputPRGB->count()-1);
	}
	inputPCMYK->clear();
	for (it = InputProfilesCMYK->begin(); it != InputProfilesCMYK->end(); ++it)
	{
		inputPCMYK->insertItem(it.key());
		if (it.key() == prefs->DefaultSolidColorCMYKProfile)
			inputPCMYK->setCurrentItem(inputPCMYK->count()-1);
	}
	monitorP->clear();
	for (it = MonitorProfiles->begin(); it != MonitorProfiles->end(); ++it)
	{
		monitorP->insertItem(it.key());
		if (it.key() == prefs->DefaultMonitorProfile)
			monitorP->setCurrentItem(monitorP->count()-1);
	}
	printerP->clear();
	for (it = PrinterProfiles->begin(); it != PrinterProfiles->end(); ++it)
	{
		printerP->insertItem(it.key());
		if (it.key() == prefs->DefaultPrinterProfile)
			printerP->setCurrentItem(printerP->count()-1);
	}

	QString tmp_mp[] = { tr("Perceptual"), tr("Relative Colorimetric"),
		tr("Saturation"), tr("Absolute Colorimetric")};
		size_t array = sizeof(tmp_mp) / sizeof(*tmp_mp);
	imagesI->clear();
	for (uint prop = 0; prop < array; ++prop)
		imagesI->insertItem(tmp_mp[prop]);
	imagesI->setCurrentItem(prefs->DefaultIntentImages);
	colorsI->clear();
	for (uint prop = 0; prop < array; ++prop)
		colorsI->insertItem(tmp_mp[prop]);
	colorsI->setCurrentItem(prefs->DefaultIntentColors);

	simulate->setChecked(prefs->SoftProofOn);
	convertAll->setChecked(prefs->SoftProofFullOn);
	gamutC->setChecked(prefs->GamutCheck);
	blackP->setChecked(prefs->BlackPoint);

	if (!checkBox1->isChecked())
	{
		simulate->setEnabled( false );
		convertAll->setEnabled( false );
		//gamutC->setEnabled( false );
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
		(prefs->DefaultIntentColors != colorsI->currentItem()) ||
		(prefs->DefaultIntentImages != imagesI->currentItem()) ||
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
	prefs->DefaultIntentColors = colorsI->currentItem();
	prefs->DefaultIntentImages = imagesI->currentItem();
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
		(doc->CMSSettings.DefaultIntentColors != colorsI->currentItem()) ||
		(doc->CMSSettings.DefaultIntentImages != imagesI->currentItem()) ||
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
	doc->CMSSettings.DefaultIntentColors = colorsI->currentItem();
	doc->CMSSettings.DefaultIntentImages = imagesI->currentItem();
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

