#include "scripterprefsgui.h"
#include "scriptercore.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <prefspanel.h>

ScripterPrefsGui::ScripterPrefsGui( QWidget* parent  )
	: PrefsPanel( parent, "ScripterPrefsGui" )
{
	ScripterPrefsGuiBaseLayout = new QGridLayout( this, 1, 1, 11, 6, "ScripterPrefsGuiBaseLayout"); 

	extGroup = new QGroupBox( this, "extGroup" );
	extGroup->setColumnLayout(0, Qt::Vertical );
	extGroup->layout()->setSpacing( 6 );
	extGroup->layout()->setMargin( 11 );
	extGroupLayout = new QVBoxLayout( extGroup->layout() );
	extGroupLayout->setAlignment( Qt::AlignTop );

	extensionScriptsChk = new QCheckBox( extGroup, "extensionScriptsChk" );
	extGroupLayout->addWidget( extensionScriptsChk );

	startupScriptFrame = new QFrame( extGroup, "startupScriptFrame" );
	startupScriptFrame->setFrameShape( QFrame::StyledPanel );
	startupScriptFrame->setFrameShadow( QFrame::Raised );
	startupScriptFrameLayout = new QHBoxLayout( startupScriptFrame, 11, 6, "startupScriptFrameLayout"); 

	startupScriptEditLabel = new QLabel( startupScriptFrame, "startupScriptEditLabel" );
	startupScriptFrameLayout->addWidget( startupScriptEditLabel );

	startupScriptEdit = new QLineEdit( startupScriptFrame, "startupScriptEdit" );
	startupScriptFrameLayout->addWidget( startupScriptEdit );
	extGroupLayout->addWidget( startupScriptFrame );
	extScriptSpacer = new QSpacerItem( 20, 111, QSizePolicy::Minimum, QSizePolicy::Expanding );
	extGroupLayout->addItem( extScriptSpacer );

	ScripterPrefsGuiBaseLayout->addWidget( extGroup, 0, 0 );
	languageChange();
	resize( QSize(423, 176).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

	// Set the state of the ext script enable checkbox
	extensionScriptsChk->setChecked(scripterCore->extensionsEnabled());
	// The startup script box should be disabled  if ext scripts are off
	startupScriptEdit->setEnabled(extensionScriptsChk->isChecked());
	connect(extensionScriptsChk, SIGNAL(toggled(bool)),
			startupScriptEdit, SLOT(setEnabled(bool)));

	// signals and slots connections
	connect( extensionScriptsChk, SIGNAL( toggled(bool) ), startupScriptEdit, SLOT( setEnabled(bool) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ScripterPrefsGui::~ScripterPrefsGui()
{
	// no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ScripterPrefsGui::languageChange()
{
	setCaption( tr( "Scripter Preferences" ) );
	extGroup->setTitle( tr( "Extension Scripts" ) );
	extensionScriptsChk->setText( tr( "Enable Extension Scripts" ) );
	startupScriptEditLabel->setText( tr( "Startup Script" ) );
}

// Apply changes to prefs. Auto connected.
void ScripterPrefsGui::apply()
{
	scripterCore->setExtensionsEnabled(extensionScriptsChk->isChecked());
	scripterCore->setStartupScript(startupScriptEdit->text());
}

#include "scripterprefsgui.moc"
