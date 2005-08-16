#include "cmsprefs.h"
#include "cmsprefs.moc"

CMSPrefs::CMSPrefs( QWidget* parent, CMSData *Vor, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles)
	: QWidget( parent, "CMS" )
{
	prefs = Vor;
	changed = false;
	cmsPrefsLayout = new QVBoxLayout( this ); 
	cmsPrefsLayout->setAlignment( Qt::AlignTop );
	cmsPrefsLayout->setSpacing( 5 );
	cmsPrefsLayout->setMargin( 0 );

	checkBox1 = new QCheckBox( tr( "&Activate Color Management" ), this, "CheckBox1" );
	checkBox1->setChecked(Vor->CMSinUse);
	cmsPrefsLayout->addWidget( checkBox1 );

	sysProfiles = new QGroupBox( tr( "System Profiles" ), this, "SysProfiles" );
	if (!checkBox1->isChecked())
		sysProfiles->setEnabled( false );
	sysProfiles->setColumnLayout(0, Qt::Vertical );
	sysProfiles->layout()->setSpacing( 0 );
	sysProfiles->layout()->setMargin( 0 );
	sysProfilesLayout = new QGridLayout( sysProfiles->layout() );
	sysProfilesLayout->setAlignment( Qt::AlignTop );
	sysProfilesLayout->setSpacing( 5 );
	sysProfilesLayout->setMargin( 10 );

	ProfilesL::Iterator it;

	inputP = new QComboBox( true, sysProfiles, "InputP" );
	inputP->setMinimumSize( QSize( 190, 22 ) );
	inputP->setEditable(false);
	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		inputP->insertItem(it.key());
		if (it.key() == Vor->DefaultImageRGBProfile)
			inputP->setCurrentItem(inputP->count()-1);
	}
	text1 = new QLabel( inputP, tr( "&RGB Pictures:" ), sysProfiles, "Text1" );
	sysProfilesLayout->addWidget( text1, 0, 0 );
	sysProfilesLayout->addWidget( inputP, 0, 1 );

	inputPCMYK = new QComboBox( true, sysProfiles, "inputPCMYK" );
	inputPCMYK->setMinimumSize( QSize( 190, 22 ) );
	inputPCMYK->setEditable(false);
	for (it = InputProfilesCMYK->begin(); it != InputProfilesCMYK->end(); ++it)
	{
		inputPCMYK->insertItem(it.key());
		if (it.key() == Vor->DefaultImageCMYKProfile)
			inputPCMYK->setCurrentItem(inputPCMYK->count()-1);
	}
	text1CMYK = new QLabel( inputPCMYK, tr( "&CMYK Pictures:" ), sysProfiles, "text1cmyk" );
	sysProfilesLayout->addWidget( text1CMYK, 1, 0 );
	sysProfilesLayout->addWidget( inputPCMYK, 1, 1 );

	inputP2 = new QComboBox( true, sysProfiles, "InputP2" );
	inputP2->setMinimumSize( QSize( 190, 22 ) );
	inputP2->setEditable(false);
	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		inputP2->insertItem(it.key());
		if (it.key() == Vor->DefaultSolidColorProfile)
			inputP2->setCurrentItem(inputP2->count()-1);
	}
	text4 = new QLabel( inputP2, tr( "&Solid Colors:" ), sysProfiles, "Text4" );
	sysProfilesLayout->addWidget( text4, 2, 0 );
	sysProfilesLayout->addWidget( inputP2, 2, 1 );

	monitorP = new QComboBox( true, sysProfiles, "MonitorP" );
	monitorP->setMinimumSize( QSize( 190, 22 ) );
	monitorP->setEditable(false);
	for (it = MonitorProfiles->begin(); it != MonitorProfiles->end(); ++it)
	{
		monitorP->insertItem(it.key());
		if (it.key() == Vor->DefaultMonitorProfile)
			monitorP->setCurrentItem(monitorP->count()-1);
	}
	text2 = new QLabel( monitorP, tr( "&Monitor:" ), sysProfiles, "Text2" );
	sysProfilesLayout->addWidget( text2, 3, 0 );
	sysProfilesLayout->addWidget( monitorP, 3, 1 );

	printerP = new QComboBox( true, sysProfiles, "PrinterP" );
	printerP->setMinimumSize( QSize( 190, 22 ) );
	printerP->setEditable(false);
	for (it = PrinterProfiles->begin(); it != PrinterProfiles->end(); ++it)
	{
		printerP->insertItem(it.key());
		if (it.key() == Vor->DefaultPrinterProfile)
			printerP->setCurrentItem(printerP->count()-1);
	}
	text3 = new QLabel( printerP, tr( "P&rinter:" ), sysProfiles, "Text3" );
	sysProfilesLayout->addWidget( text3, 4, 0 );
	sysProfilesLayout->addWidget( printerP, 4, 1 );
	cmsPrefsLayout->addWidget( sysProfiles );

	render = new QGroupBox( tr( "Rendering Intents" ), this, "Render" );
	if (!checkBox1->isChecked())
		render->setEnabled( false );
	render->setColumnLayout(0, Qt::Vertical );
	render->layout()->setSpacing( 0 );
	render->layout()->setMargin( 0 );
	renderLayout = new QGridLayout( render->layout() );
	renderLayout->setAlignment( Qt::AlignTop );
	renderLayout->setSpacing( 5 );
	renderLayout->setMargin( 10 );

	monitorI = new QComboBox( true, render, "MonitorI" );
	QString tmp_mp[] = { tr("Perceptual"), tr("Relative Colorimetric"), 
						tr("Saturation"), tr("Absolute Colorimetric")};
	size_t array = sizeof(tmp_mp) / sizeof(*tmp_mp);
	/* PFJ - 29.02.04 - Changed from uint to int and var name */
	for (uint prop = 0; prop < array; ++prop)
		monitorI->insertItem(tmp_mp[prop]);
	monitorI->setMinimumSize( QSize( 190, 22 ) );
	monitorI->setEditable(false);
	monitorI->setCurrentItem(Vor->DefaultIntentMonitor);
	text21 = new QLabel( monitorI, tr( "M&onitor:" ), render, "Text21" );
	renderLayout->addWidget( text21, 0, 0 );
	renderLayout->addWidget( monitorI, 0, 1 );

	printerI = new QComboBox( true, render, "PrinterI" );
	for (uint prop = 0; prop < array; ++prop)
		printerI->insertItem(tmp_mp[prop]);
	printerI->setMinimumSize( QSize( 190, 22 ) );
	printerI->setEditable(false);
	printerI->setCurrentItem(Vor->DefaultIntentPrinter);
	text22 = new QLabel( printerI, tr( "Pr&inter:" ), render, "Text22" );
	renderLayout->addWidget( text22, 1, 0 );
	renderLayout->addWidget( printerI, 1, 1 );

	cmsPrefsLayout->addWidget( render );

	simulate = new QCheckBox( tr( "Sim&ulate Printer on the Screen" ), this, "Simulate" );
	simulate->setChecked(Vor->SoftProofOn);
	cmsPrefsLayout->addWidget( simulate );

	gamutC = new QCheckBox( tr( "Mark Colors out of &Gamut" ), this, "Gamut" );
	gamutC->setChecked(Vor->GamutCheck);
	cmsPrefsLayout->addWidget( gamutC );

	blackP = new QCheckBox( tr( "Use &Blackpoint Compensation" ), this, "Black" );
	blackP->setChecked(Vor->BlackPoint);
	cmsPrefsLayout->addWidget( blackP );

	if (!checkBox1->isChecked())
	{
		simulate->setEnabled( false );
		gamutC->setEnabled( false );
		blackP->setEnabled( false );
	}

	QToolTip::add( inputPCMYK, tr( "Default color profile for imported cmyk images" ) );
	QToolTip::add( inputP, tr( "Default color profile for imported rgb images" ) );
	QToolTip::add( inputP2, tr( "Default color profile for solid colors on the page" ) );
	QToolTip::add( monitorP, tr( "Color profile that you have generated or received from the manufacturer.\nThis profile should be specific to your monitor and not a generic profile (i.e. sRGB)." ) );
	QToolTip::add( printerP, tr( "Color profile for your printer model from the manufacturer.\nThis profile should be specific to your printer and not a generic profile (i.e. sRGB)." ) );
	QToolTip::add( monitorI, tr( "Default rendering intent for your monitor. Unless you know why to change it,\nRelative Colorimetric or Perceptual should be chosen." ) );
	QToolTip::add( printerI, tr( "Default rendering intent for your printer. Unless you know why to change it,\nRelative Colorimetric or Perceptual should be chosen." ) );
	QToolTip::add( simulate, tr( "Enable 'soft proofing' of how your document colors will print,\nbased on the chosen printer profile." ) );
	QToolTip::add( gamutC, tr( "Method of showing colors on the screen which may not print properly.\nThis requires very accurate profiles and serves only as a warning." ) );
	QToolTip::add( blackP, tr( "Black Point Compensation is a method of improving contrast in photos.\nIt is recommended that you enable this if you have photos in your document." ) );

	// signals and slots connections
	connect( checkBox1, SIGNAL( clicked() ), this, SLOT( slotCMSon() ) );
	connect( simulate, SIGNAL( clicked() ), this, SLOT( slotSimula() ) );
}

void CMSPrefs::restoreDefaults()
{
}

void CMSPrefs::setValues()
{
	if ((prefs->DefaultImageRGBProfile != inputP->currentText()) ||
		(prefs->DefaultImageCMYKProfile != inputPCMYK->currentText()) ||
		(prefs->DefaultSolidColorProfile != inputP2->currentText()) ||
		(prefs->DefaultMonitorProfile != monitorP->currentText()) ||
		(prefs->DefaultPrinterProfile != printerP->currentText()) ||
		(prefs->DefaultIntentPrinter != printerI->currentItem()) ||
		(prefs->DefaultIntentMonitor != monitorI->currentItem()) ||
		(prefs->SoftProofOn != simulate->isChecked()) ||
		(prefs->GamutCheck != gamutC->isChecked()) ||
		(prefs->BlackPoint != blackP->isChecked()) ||
		(prefs->CMSinUse != checkBox1->isChecked()))
			changed = true;
	prefs->DefaultImageRGBProfile = inputP->currentText();
	prefs->DefaultImageCMYKProfile = inputPCMYK->currentText();
	prefs->DefaultSolidColorProfile = inputP2->currentText();
	prefs->DefaultMonitorProfile = monitorP->currentText();
	prefs->DefaultPrinterProfile = printerP->currentText();
	prefs->DefaultIntentPrinter = printerI->currentItem();
	prefs->DefaultIntentMonitor = monitorI->currentItem();
	prefs->DefaultIntentImages = monitorI->currentItem();
	prefs->SoftProofOn = simulate->isChecked();
	prefs->GamutCheck = gamutC->isChecked();
	prefs->CMSinUse = checkBox1->isChecked();
	prefs->BlackPoint = blackP->isChecked();
}

void CMSPrefs::slotSimula()
{
	bool setter = simulate->isChecked() ? true : false;
	gamutC->setEnabled(setter);
}

void CMSPrefs::slotCMSon()
{
	bool setter = checkBox1->isChecked() ? true : false;
	sysProfiles->setEnabled(setter);
	render->setEnabled(setter);
	simulate->setEnabled(setter);
	blackP->setEnabled(setter);
	if (setter == true)
		slotSimula();
	else
		gamutC->setEnabled(setter);
	emit cmsOn(setter);
}

