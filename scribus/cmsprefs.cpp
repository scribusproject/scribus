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

    CheckBox1 = new QCheckBox( this, "CheckBox1" );
    CheckBox1->setText( tr( "Activate Color Management" ) );
    CheckBox1->setChecked(Vor->CMSinUse);
    CMSPrefsLayout->addWidget( CheckBox1 );

    SysProfiles = new QGroupBox( this, "SysProfiles" );
	if (!CheckBox1->isChecked())
    	SysProfiles->setEnabled( false );
    SysProfiles->setTitle( tr( "System Profiles" ) );
    SysProfiles->setColumnLayout(0, Qt::Vertical );
    SysProfiles->layout()->setSpacing( 0 );
    SysProfiles->layout()->setMargin( 0 );
    SysProfilesLayout = new QGridLayout( SysProfiles->layout() );
    SysProfilesLayout->setAlignment( Qt::AlignTop );
    SysProfilesLayout->setSpacing( 6 );
    SysProfilesLayout->setMargin( 11 );

    Text1 = new QLabel( SysProfiles, "Text1" );
    Text1->setText( tr( "Pictures:" ) );
    SysProfilesLayout->addWidget( Text1, 0, 0 );

    Text4 = new QLabel( SysProfiles, "Text4" );
    Text4->setText( tr( "Solid Colors:" ) );
    SysProfilesLayout->addWidget( Text4, 1, 0 );

    Text2 = new QLabel( SysProfiles, "Text2" );
    Text2->setText( tr( "Monitor:" ) );
    SysProfilesLayout->addWidget( Text2, 2, 0 );

    Text3 = new QLabel( SysProfiles, "Text3" );
    Text3->setText( tr( "Printer:" ) );
    SysProfilesLayout->addWidget( Text3, 3, 0 );
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

    SysProfilesLayout->addWidget( PrinterP, 3, 1 );
    CMSPrefsLayout->addWidget( SysProfiles );

    Render = new QGroupBox( this, "Render" );
	if (!CheckBox1->isChecked())
    	Render->setEnabled( false );
    Render->setTitle( tr( "Rendering Intents" ) );
    Render->setColumnLayout(0, Qt::Vertical );
    Render->layout()->setSpacing( 0 );
    Render->layout()->setMargin( 0 );
    RenderLayout = new QGridLayout( Render->layout() );
    RenderLayout->setAlignment( Qt::AlignTop );
    RenderLayout->setSpacing( 6 );
    RenderLayout->setMargin( 11 );

    Text21 = new QLabel( Render, "Text21" );
    Text21->setText( tr( "Monitor:" ) );
    RenderLayout->addWidget( Text21, 0, 0 );

    Text22 = new QLabel( Render, "Text22" );
    Text22->setText( tr( "Printer:" ) );
    RenderLayout->addWidget( Text22, 1, 0 );

    MonitorI = new QComboBox( true, Render, "MonitorI" );
    MonitorI->insertItem( tr( "Perceptual" ) );
    MonitorI->insertItem( tr( "Relative Colorimetric" ) );
    MonitorI->insertItem( tr( "Saturation" ) );
    MonitorI->insertItem( tr( "Absolute Colorimetric" ) );
    MonitorI->setMinimumSize( QSize( 190, 22 ) );
    MonitorI->setEditable(false);
    MonitorI->setCurrentItem(Vor->DefaultIntentMonitor);

    RenderLayout->addWidget( MonitorI, 0, 1 );

    PrinterI = new QComboBox( true, Render, "PrinterI" );
    PrinterI->insertItem( tr( "Perceptual" ) );
    PrinterI->insertItem( tr( "Relative Colorimetric" ) );
    PrinterI->insertItem( tr( "Saturation" ) );
    PrinterI->insertItem( tr( "Absolute Colorimetric" ) );
    PrinterI->setMinimumSize( QSize( 190, 22 ) );
    PrinterI->setEditable(false);
    PrinterI->setCurrentItem(Vor->DefaultIntentPrinter);

    RenderLayout->addWidget( PrinterI, 1, 1 );

    CMSPrefsLayout->addWidget( Render );

    Simulate = new QCheckBox( this, "Simulate" );
    Simulate->setText( tr( "Simulate Printer on the Screen" ) );
    Simulate->setChecked(Vor->SoftProofOn);
    CMSPrefsLayout->addWidget( Simulate );

    GamutC = new QCheckBox( this, "Gamut" );
    GamutC->setText( tr( "Mark Colors out of Gamut" ) );
    GamutC->setChecked(Vor->GamutCheck);
    CMSPrefsLayout->addWidget( GamutC );

    BlackP = new QCheckBox( this, "Black" );
    BlackP->setText( tr( "Use Blackpoint Compensation" ) );
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
    OK = new QPushButton( this, "OK" );
    OK->setText( tr( "OK" ) );
    OK->setDefault( true );
    Layout1->addWidget( OK );

    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer_2 );
    Cancel = new QPushButton( this, "Cancel" );
    Cancel->setText( tr( "Cancel" ) );
    Layout1->addWidget( Cancel );

    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer_3 );
    CMSPrefsLayout->addLayout( Layout1 );

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

