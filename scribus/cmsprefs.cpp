/****************************************************************************
** Form implementation generated from reading ui file 'CMSpref.ui'
**
** Created: Tue Jan 1 10:37:25 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "cmsprefs.h"
#include "cmsprefs.moc"
extern QPixmap loadIcon(QString nam);

CMSPrefs::CMSPrefs( QWidget* parent, CMSset *Vor,
										ProfilesL *InputProfiles, ProfilesL *PrinterProfiles, 
										ProfilesL *MonitorProfiles)
	: QDialog( parent, "CMS", true, 0 )
{
	setCaption( tr( "Color Management Settings" ) );
	setIcon(loadIcon("AppIcon.png"));
	Prefs = Vor;
	Changed = false;
	CMSPrefsLayout = new QVBoxLayout( this ); 
	CMSPrefsLayout->setSpacing( 6 );
	CMSPrefsLayout->setMargin( 11 );

	CheckBox1 = new QCheckBox( tr( "&Activate Color Management" ), this, "CheckBox1" );
	CheckBox1->setChecked(Vor->CMSinUse);
	CMSPrefsLayout->addWidget( CheckBox1 );

	SysProfiles = new QGroupBox( tr( "System Profiles" ), this, "SysProfiles" );
	if (!CheckBox1->isChecked())
		SysProfiles->setEnabled( false );
	SysProfiles->setColumnLayout(0, Qt::Vertical );
	SysProfiles->layout()->setSpacing( 0 );
	SysProfiles->layout()->setMargin( 0 );
	SysProfilesLayout = new QGridLayout( SysProfiles->layout() );
	SysProfilesLayout->setAlignment( Qt::AlignTop );
	SysProfilesLayout->setSpacing( 6 );
	SysProfilesLayout->setMargin( 11 );

	ProfilesL::Iterator it;

	InputP = new QComboBox( true, SysProfiles, "InputP" );
	InputP->setMinimumSize( QSize( 190, 22 ) );
	InputP->setEditable(false);
	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		InputP->insertItem(it.key());
		if (it.key() == Vor->DefaultInputProfile)
			InputP->setCurrentItem(InputP->count()-1);
	}
	Text1 = new QLabel( InputP, tr( "&Pictures:" ), SysProfiles, "Text1" );
	SysProfilesLayout->addWidget( Text1, 0, 0 );
	SysProfilesLayout->addWidget( InputP, 0, 1 );

	InputP2 = new QComboBox( true, SysProfiles, "InputP2" );
	InputP2->setMinimumSize( QSize( 190, 22 ) );
	InputP2->setEditable(false);
	for (it = InputProfiles->begin(); it != InputProfiles->end(); ++it)
	{
		InputP2->insertItem(it.key());
		if (it.key() == Vor->DefaultInputProfile2)
			InputP2->setCurrentItem(InputP2->count()-1);
	}
	Text4 = new QLabel( InputP2, tr( "&Solid Colors:" ), SysProfiles, "Text4" );
	SysProfilesLayout->addWidget( Text4, 1, 0 );
	SysProfilesLayout->addWidget( InputP2, 1, 1 );

	MonitorP = new QComboBox( true, SysProfiles, "MonitorP" );
	MonitorP->setMinimumSize( QSize( 190, 22 ) );
	MonitorP->setEditable(false);
	for (it = MonitorProfiles->begin(); it != MonitorProfiles->end(); ++it)
	{
		MonitorP->insertItem(it.key());
		if (it.key() == Vor->DefaultMonitorProfile)
			MonitorP->setCurrentItem(MonitorP->count()-1);
	}
	Text2 = new QLabel( MonitorP, tr( "&Monitor:" ), SysProfiles, "Text2" );
	SysProfilesLayout->addWidget( Text2, 2, 0 );
	SysProfilesLayout->addWidget( MonitorP, 2, 1 );

	PrinterP = new QComboBox( true, SysProfiles, "PrinterP" );
	PrinterP->setMinimumSize( QSize( 190, 22 ) );
	PrinterP->setEditable(false);
	for (it = PrinterProfiles->begin(); it != PrinterProfiles->end(); ++it)
	{
		PrinterP->insertItem(it.key());
		if (it.key() == Vor->DefaultPrinterProfile)
			PrinterP->setCurrentItem(PrinterP->count()-1);
	}
	Text3 = new QLabel( PrinterP, tr( "P&rinter:" ), SysProfiles, "Text3" );
	SysProfilesLayout->addWidget( Text3, 3, 0 );
	SysProfilesLayout->addWidget( PrinterP, 3, 1 );
	CMSPrefsLayout->addWidget( SysProfiles );

	Render = new QGroupBox( tr( "Rendering Intents" ), this, "Render" );
	if (!CheckBox1->isChecked())
		Render->setEnabled( false );
	Render->setColumnLayout(0, Qt::Vertical );
	Render->layout()->setSpacing( 0 );
	Render->layout()->setMargin( 0 );
	RenderLayout = new QGridLayout( Render->layout() );
	RenderLayout->setAlignment( Qt::AlignTop );
	RenderLayout->setSpacing( 6 );
	RenderLayout->setMargin( 11 );

	MonitorI = new QComboBox( true, Render, "MonitorI" );
	QString tmp_mp[] = { tr("Perceptual"), tr("Relative Colorimetric"), 
						tr("Saturation"), tr("Absolute Colorimetric")};
	size_t array = sizeof(tmp_mp) / sizeof(*tmp_mp);
	/* PFJ - 29.02.04 - Changed from uint to int and var name */
	for (uint prop = 0; prop < array; ++prop)
		MonitorI->insertItem(tmp_mp[prop]);
	MonitorI->setMinimumSize( QSize( 190, 22 ) );
	MonitorI->setEditable(false);
	MonitorI->setCurrentItem(Vor->DefaultIntentMonitor);
	Text21 = new QLabel( MonitorI, tr( "M&onitor:" ), Render, "Text21" );
	RenderLayout->addWidget( Text21, 0, 0 );
	RenderLayout->addWidget( MonitorI, 0, 1 );

	PrinterI = new QComboBox( true, Render, "PrinterI" );
	for (uint prop = 0; prop < array; ++prop)
		PrinterI->insertItem(tmp_mp[prop]);
	PrinterI->setMinimumSize( QSize( 190, 22 ) );
	PrinterI->setEditable(false);
	PrinterI->setCurrentItem(Vor->DefaultIntentPrinter);
	Text22 = new QLabel( PrinterI, tr( "Pr&inter:" ), Render, "Text22" );
	RenderLayout->addWidget( Text22, 1, 0 );
	RenderLayout->addWidget( PrinterI, 1, 1 );

	CMSPrefsLayout->addWidget( Render );

	Simulate = new QCheckBox( tr( "Sim&ulate Printer on the Screen" ), this, "Simulate" );
	Simulate->setChecked(Vor->SoftProofOn);
	CMSPrefsLayout->addWidget( Simulate );

	GamutC = new QCheckBox( tr( "Mark Colors out of &Gamut" ), this, "Gamut" );
	GamutC->setChecked(Vor->GamutCheck);
	CMSPrefsLayout->addWidget( GamutC );

	BlackP = new QCheckBox( tr( "Use &Blackpoint Compensation" ), this, "Black" );
#ifdef cmsFLAGS_BLACKPOINTCOMPENSATION
	BlackP->setChecked(Vor->BlackPoint);
#else
	BlackP->setChecked(false);
	BlackP->hide();
#endif
	CMSPrefsLayout->addWidget( BlackP );

	if (!CheckBox1->isChecked())
	{
		Simulate->setEnabled( false );
		GamutC->setEnabled( false );
		BlackP->setEnabled( false );
	}

	Layout1 = new QHBoxLayout; 
	Layout1->setSpacing( 6 );
	Layout1->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );
	OK = new QPushButton( tr( "&OK" ), this, "OK" );
	OK->setDefault( true );
	Layout1->addWidget( OK );
	Cancel = new QPushButton( tr( "&Cancel" ), this, "Cancel" );
	Layout1->addWidget( Cancel );
	CMSPrefsLayout->addLayout( Layout1 );

	QToolTip::add( InputP, tr( "Default color profile for imported images" ) );
	QToolTip::add( InputP2, tr( "Default color profile for solid colors on the page" ) );
	QToolTip::add( MonitorP, tr( "Color profile that you have generated or received from the manufacturer.\nThis profile should be specific to your monitor and not a generic profile (i.e. sRGB)." ) );
	QToolTip::add( PrinterP, tr( "Color profile for your printer model from the manufacturer.\nThis profile should be specific to your printer and not a generic profile (i.e. sRGB)." ) );
	QToolTip::add( MonitorI, tr( "Default rendering intent for your monitor. Unless you know why to change it,\nRelative Colorimetric or Perceptual should be chosen." ) );
	QToolTip::add( PrinterI, tr( "Default rendering intent for your printer. Unless you know why to change it,\nRelative Colorimetric or Perceptual should be chosen." ) );
	QToolTip::add( Simulate, tr( "Enable 'soft proofing' of how your document colors will print,\nbased on the chosen printer profile." ) );
	QToolTip::add( GamutC, tr( "Method of showing colors on the screen which may not print properly.\nThis requires very accurate profiles and serves only as a warning." ) );
	QToolTip::add( BlackP, tr( "Black Point Compensation is a method of improving contrast in photos.\nIt is recommended that you enable this if you have photos in your document." ) );

	// signals and slots connections
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( OK, SIGNAL( clicked() ), this, SLOT( SetValues() ) );
	connect( CheckBox1, SIGNAL( clicked() ), this, SLOT( slotCMSon() ) );
	connect( Simulate, SIGNAL( clicked() ), this, SLOT( slotSimula() ) );
}

void CMSPrefs::SetValues()
{
	if ((Prefs->DefaultInputProfile != InputP->currentText()) ||
		(Prefs->DefaultInputProfile2 != InputP2->currentText()) ||
		(Prefs->DefaultMonitorProfile != MonitorP->currentText()) ||
		(Prefs->DefaultPrinterProfile != PrinterP->currentText()) ||
		(Prefs->DefaultIntentPrinter != PrinterI->currentItem()) ||
		(Prefs->DefaultIntentMonitor != MonitorI->currentItem()) ||
		(Prefs->SoftProofOn != Simulate->isChecked()) ||
		(Prefs->GamutCheck != GamutC->isChecked()) ||
		(Prefs->BlackPoint != BlackP->isChecked()) ||
		(Prefs->CMSinUse != CheckBox1->isChecked()))
			Changed = true;
	Prefs->DefaultInputProfile = InputP->currentText();
	Prefs->DefaultInputProfile2 = InputP2->currentText();
	Prefs->DefaultMonitorProfile = MonitorP->currentText();
	Prefs->DefaultPrinterProfile = PrinterP->currentText();
	Prefs->DefaultIntentPrinter = PrinterI->currentItem();
	Prefs->DefaultIntentMonitor = MonitorI->currentItem();
	Prefs->DefaultIntentMonitor2 = MonitorI->currentItem();
	Prefs->SoftProofOn = Simulate->isChecked();
	Prefs->GamutCheck = GamutC->isChecked();
	Prefs->CMSinUse = CheckBox1->isChecked();
	Prefs->BlackPoint = BlackP->isChecked();
	accept();
}

void CMSPrefs::slotSimula()
{
	bool setter = Simulate->isChecked() ? true : false;
	GamutC->setEnabled(setter);
}

void CMSPrefs::slotCMSon()
{
	bool setter = CheckBox1->isChecked() ? true : false;
	SysProfiles->setEnabled(setter);
	Render->setEnabled(setter);
	Simulate->setEnabled(setter);
	BlackP->setEnabled(setter);
	if (setter == true)
		slotSimula();
	else
		GamutC->setEnabled(setter);
}

