/****************************************************************************
** Form implementation generated from reading ui file './scripterprefs.ui'
**
** Created: Tue Feb 15 00:34:02 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "scripterprefs.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "./scripterprefs.ui.h"

/*
 *  Constructs a ScripterPreferences as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ScripterPreferences::ScripterPreferences( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ScripterPreferences" );
    ScripterPreferencesLayout = new QGridLayout( this, 1, 1, 11, 6, "ScripterPreferencesLayout"); 

    extPythonChk = new QCheckBox( this, "extPythonChk" );

    ScripterPreferencesLayout->addMultiCellWidget( extPythonChk, 0, 0, 0, 2 );

    startupScriptGroup = new QGroupBox( this, "startupScriptGroup" );
    startupScriptGroup->setEnabled( FALSE );
    startupScriptGroup->setCheckable( TRUE );
    startupScriptGroup->setChecked( FALSE );
    startupScriptGroup->setColumnLayout(0, Qt::Vertical );
    startupScriptGroup->layout()->setSpacing( 6 );
    startupScriptGroup->layout()->setMargin( 11 );
    startupScriptGroupLayout = new QGridLayout( startupScriptGroup->layout() );
    startupScriptGroupLayout->setAlignment( Qt::AlignTop );

    startupScriptBrowseBtn = new QPushButton( startupScriptGroup, "startupScriptBrowseBtn" );
    startupScriptBrowseBtn->setEnabled( FALSE );

    startupScriptGroupLayout->addWidget( startupScriptBrowseBtn, 0, 2 );

    startupScriptPath = new QLineEdit( startupScriptGroup, "startupScriptPath" );
    startupScriptPath->setEnabled( FALSE );

    startupScriptGroupLayout->addWidget( startupScriptPath, 0, 1 );

    textLabel1 = new QLabel( startupScriptGroup, "textLabel1" );
    textLabel1->setEnabled( FALSE );

    startupScriptGroupLayout->addWidget( textLabel1, 0, 0 );

    ScripterPreferencesLayout->addMultiCellWidget( startupScriptGroup, 1, 1, 0, 2 );

    cancelButton = new QPushButton( this, "cancelButton" );

    ScripterPreferencesLayout->addWidget( cancelButton, 3, 2 );
    spacer1 = new QSpacerItem( 211, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ScripterPreferencesLayout->addItem( spacer1, 3, 0 );

    okButton = new QPushButton( this, "okButton" );
    okButton->setDefault( TRUE );

    ScripterPreferencesLayout->addWidget( okButton, 3, 1 );

    advancedOptionsGroup = new QGroupBox( this, "advancedOptionsGroup" );
    advancedOptionsGroup->setColumnLayout(0, Qt::Vertical );
    advancedOptionsGroup->layout()->setSpacing( 6 );
    advancedOptionsGroup->layout()->setMargin( 11 );
    advancedOptionsGroupLayout = new QGridLayout( advancedOptionsGroup->layout() );
    advancedOptionsGroupLayout->setAlignment( Qt::AlignTop );

    importNamesChk = new QCheckBox( advancedOptionsGroup, "importNamesChk" );
    importNamesChk->setChecked( TRUE );

    advancedOptionsGroupLayout->addWidget( importNamesChk, 0, 0 );

    legacyAliasesChk = new QCheckBox( advancedOptionsGroup, "legacyAliasesChk" );
    legacyAliasesChk->setChecked( TRUE );

    advancedOptionsGroupLayout->addWidget( legacyAliasesChk, 1, 0 );

    useFakeStdinChk = new QCheckBox( advancedOptionsGroup, "useFakeStdinChk" );
    useFakeStdinChk->setChecked( TRUE );

    advancedOptionsGroupLayout->addWidget( useFakeStdinChk, 2, 0 );

    ScripterPreferencesLayout->addMultiCellWidget( advancedOptionsGroup, 2, 2, 0, 2 );
    languageChange();
    resize( QSize(526, 308).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( importNamesChk, SIGNAL( clicked() ), importNamesChk, SLOT( setFocus() ) );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( extPythonChk, SIGNAL( clicked() ), extPythonChk, SLOT( setFocus() ) );
    connect( extPythonChk, SIGNAL( toggled(bool) ), startupScriptGroup, SLOT( setEnabled(bool) ) );
    connect( startupScriptGroup, SIGNAL( toggled(bool) ), textLabel1, SLOT( setEnabled(bool) ) );
    connect( startupScriptBrowseBtn, SIGNAL( clicked() ), startupScriptPath, SLOT( setFocus() ) );
    connect( startupScriptGroup, SIGNAL( toggled(bool) ), startupScriptPath, SLOT( setEnabled(bool) ) );
    connect( this, SIGNAL( pathChanged(const QString &) ), startupScriptPath, SLOT( setText(const QString&) ) );
    connect( startupScriptGroup, SIGNAL( toggled(bool) ), startupScriptBrowseBtn, SLOT( setEnabled(bool) ) );
    connect( startupScriptBrowseBtn, SIGNAL( clicked() ), this, SLOT( browseForScript() ) );

    // tab order
    setTabOrder( extPythonChk, startupScriptPath );
    setTabOrder( startupScriptPath, startupScriptBrowseBtn );
    setTabOrder( startupScriptBrowseBtn, importNamesChk );

    // buddies
    textLabel1->setBuddy( startupScriptPath );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ScripterPreferences::~ScripterPreferences()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ScripterPreferences::languageChange()
{
    setCaption( tr( "Scribus - Scripter Preferences" ) );
    QWhatsThis::add( this, QString::null );
    extPythonChk->setText( tr( "Enable Scripter Extensions" ) );
    QToolTip::add( extPythonChk, tr( "Turn on extension scripts and macros" ) );
    QWhatsThis::add( extPythonChk, tr( "<qt><p>Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the <tt>Load Extension Script</tt> item in the Script menu.</p>\n"
"<p>\n"
"Only scripts written to be run as extension scripts should be used with <tt>Load Extension Script</tt> or as start-up scripts. See the scripter documentation for more details.</p></qt>" ) );
    startupScriptGroup->setTitle( tr( "Use a Startup Script" ) );
    QWhatsThis::add( startupScriptGroup, tr( "<qt><p>If <tt>Use a Startup Script</tt> is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.</p>\n"
"<p><tt>Use a Startup Script</tt> will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.</p></qt>" ) );
    startupScriptBrowseBtn->setText( tr( "Browse..." ) );
    QToolTip::add( startupScriptBrowseBtn, tr( "Browse for a new script file" ) );
    QWhatsThis::add( startupScriptBrowseBtn, tr( "<qt>Browse for a new script file</qt>" ) );
    startupScriptPath->setText( QString::null );
    QToolTip::add( startupScriptPath, tr( "What script file to load at start-up" ) );
    QWhatsThis::add( startupScriptPath, tr( "<qt><p>The file containing the Python script to run as an extension script at start-up.</p>\n"
"<p>Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.</p></qt>" ) );
    textLabel1->setText( tr( "Script File:" ) );
    cancelButton->setText( tr( "&Cancel" ) );
    cancelButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    QToolTip::add( cancelButton, tr( "Close without saving changes" ) );
    QWhatsThis::add( cancelButton, tr( "Close without saving changes" ) );
    okButton->setText( tr( "&Ok" ) );
    okButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    QToolTip::add( okButton, tr( "Save changes and close" ) );
    QWhatsThis::add( okButton, tr( "Save changes and close" ) );
    advancedOptionsGroup->setTitle( tr( "Advanced Options" ) );
    importNamesChk->setText( tr( "Import All Names at Startup" ) );
    QToolTip::add( importNamesChk, tr( "Run 'from scribus import *' in the script console at start-up" ) );
    QWhatsThis::add( importNamesChk, tr( "<qt><p><tt>Import All Names at Startup</tt> is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.</p>\n"
"<p>Unchecking this option will prevent the scripter from running its usual <tt>from scribus import *</tt> command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.</p>\n"
"<p>This option does not take effect until Scribus is restarted.</p></qt>" ) );
    legacyAliasesChk->setText( tr( "Enable Legacy Name Aliases" ) );
    QToolTip::add( legacyAliasesChk, tr( "<qt>Enable the use of OldStyle function names</qt>" ) );
    QWhatsThis::add( legacyAliasesChk, tr( "<qt><p><tt>Enable Legacy Aliases</tt> is an advanced option. You should probably leave it how it is.</p>\n"
"<p>If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.</p>\n"
"<p>This option does not take effect until Scribus is restarted.</p></qt>" ) );
    useFakeStdinChk->setText( tr( "Use Fake Stdin" ) );
    QToolTip::add( useFakeStdinChk, tr( "<qt>Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.</qt>" ) );
    QWhatsThis::add( useFakeStdinChk, tr( "<qt><p><tt>Use Fake Stdin</tt> is an advanced option. You should probably leave it how it is.</p>\n"
"<p>Normally, scribus will provide Python with a fake file object for <tt>sys.stdin</tt>, so that reads from stdin always return an empty string. If the real <tt>sys.stdin</tt> is left in place, scripts that try to read from it will block - and in turn block scribus's execution, making the app appear to hang - until input arrives on stdin. It's unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.</p>\n"
"<p>You can disable this option if you want to accept input on stdin. Generally you should use <tt>os.popen</tt> to make a pipe instead, or use some other input mechanism, but this option is here just in case.</p></qt>" ) );
}

#include "scripterprefs.moc"
