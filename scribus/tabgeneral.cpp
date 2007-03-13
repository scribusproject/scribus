/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qcombobox.h>
#include <qstringlist.h>
#include <qpushbutton.h>
#include <q3groupbox.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qstylefactory.h>
#include <qtooltip.h>
#include <q3filedialog.h>

#include "tabgeneral.h"
//#include "tabgeneral.moc"

#include "prefsmanager.h"
#include "scribusapp.h"
#include "scribuscore.h"

extern ScribusQApp* ScQApp;


TabGeneral::TabGeneral(QWidget* parent, const char* name)
	: TabGeneralBase(parent, name, 0)
{
	// languages
	langMgr.init();
	QStringList languageList;
	langMgr.fillInstalledStringList(&languageList, true);
	languageList.sort();
	guiLangCombo->insertStringList( languageList );

	// qt styles
	QStringList STtest;
	STtest = QStyleFactory::keys();
	for (uint stt = 0; stt < STtest.count(); ++stt)
		GUICombo->insertItem(STtest[stt]);
	//GUICombo->setEditable(false);

	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		ProPfad->setEnabled(false);
		FileC2->setEnabled(false);
	}

	QWidget::setTabOrder( GFsize, GTFsize );
	QWidget::setTabOrder( GTFsize, wheelJumpSpin );
	QWidget::setTabOrder( wheelJumpSpin, recentDocs );
	QWidget::setTabOrder( recentDocs, Docs );
	QWidget::setTabOrder( Docs, FileC );
	QWidget::setTabOrder( FileC, ProPfad );
	QWidget::setTabOrder( ProPfad, FileC2  );
	QWidget::setTabOrder( FileC2, ScriptPfad );
	QWidget::setTabOrder( ScriptPfad, FileC3 );
	QWidget::setTabOrder( FileC3, DocumentTemplateDir );
	QWidget::setTabOrder( DocumentTemplateDir, FileC4 );

	QToolTip::add(guiLangCombo, "<qt>" + tr( "Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus" )+"</qt>");
	QToolTip::add(recentDocs, "<qt>" + tr("Number of recently edited documents to show in the File menu") + "</qt>" );
	QToolTip::add( wheelJumpSpin, "<qt>" + tr( "Number of lines Scribus will scroll for each move of the mouse wheel" ) + "</qt>");
	QToolTip::add( GUICombo, "<qt>" + tr( "Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins." ) + "</qt>" );
	QToolTip::add( GFsize, "<qt>" + tr( "Default font size for the menus and windows" ) + "</qt>" );
	QToolTip::add( GTFsize, "<qt>" + tr("Default font size for the tool windows") + "</qt>" );
	QToolTip::add( Docs, "<qt>" + tr( "Default documents directory" ) + "</qt>" );
	QToolTip::add( ProPfad, "<qt>" + tr( "Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc " ) + "</qt>" );
	QToolTip::add( ScriptPfad, "<qt>" + tr( "Default Scripter scripts directory" ) + "</qt>" );
	QToolTip::add( DocumentTemplateDir, "<qt>"+ tr("Additional directory for document templates")+"</qt>" );

	connect(guiLangCombo, SIGNAL(activated(const QString &)), this, SLOT(setSelectedGUILang(const QString &)));
	connect(FileC, SIGNAL(clicked()), this, SLOT(changeDocs()));
	connect(FileC2, SIGNAL(clicked()), this, SLOT(changeProfs()));
	connect(FileC3, SIGNAL(clicked()), this, SLOT(changeScripts()));
	connect(FileC4, SIGNAL(clicked()), this, SLOT(changeDocumentTemplates()));
}

void TabGeneral::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	selectedGUILang = prefsData->guiLanguage;
	guiLangCombo->setCurrentText(langMgr.getLangFromAbbrev(selectedGUILang));
	GUICombo->setCurrentText(prefsData->GUI);
	GFsize->setValue( prefsData->AppFontSize );
	GTFsize->setValue( prefsData->PaletteFontSize); // temp solution
	wheelJumpSpin->setValue( prefsData->Wheelval );
	spinTimeout->setValue(prefsData->moveTimeout);
	recentDocs->setValue( prefsData->RecentDCount );
	Docs->setText(QDir::convertSeparators(prefsData->DocDir));
	ProPfad->setText(QDir::convertSeparators(prefsData->ProfileDir));
	ScriptPfad->setText(QDir::convertSeparators(prefsData->ScriptDir));
	DocumentTemplateDir->setText(QDir::convertSeparators(prefsData->documentTemplatesDir));
	startUpDialog->setChecked(prefsData->showStartupDialog);
	showSplashCheckBox->setChecked( !ScQApp->neverSplashExists() );
}

void TabGeneral::setSelectedGUILang( const QString &newLang )
{
	selectedGUILang = langMgr.getAbbrevFromLang(newLang);
}

void TabGeneral::changeDocs()
{
	QString s = Q3FileDialog::getExistingDirectory(Docs->text(), this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
		Docs->setText( QDir::convertSeparators(s) );
}

void TabGeneral::changeProfs()
{
	QString s = Q3FileDialog::getExistingDirectory(ProPfad->text(), this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
		ProPfad->setText( QDir::convertSeparators(s) );
}

void TabGeneral::changeScripts()
{
	QString s = Q3FileDialog::getExistingDirectory(ScriptPfad->text(), this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
		ScriptPfad->setText( QDir::convertSeparators(s) );
}

void TabGeneral::changeDocumentTemplates()
{
	QString s = Q3FileDialog::getExistingDirectory(DocumentTemplateDir->text(), this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
		DocumentTemplateDir->setText( QDir::convertSeparators(s) );
}
