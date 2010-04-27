/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFileDialog>
#include <QString>

#include "prefs_paths.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_Paths::Prefs_Paths(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	connect(docDirChangeButton, SIGNAL(clicked()), this, SLOT(changeDocs()));
	connect(profileDirChangeButton, SIGNAL(clicked()), this, SLOT(changeProfs()));
	connect(scriptDirChangeButton, SIGNAL(clicked()), this, SLOT(changeScripts()));
	connect(templateDirChangeButton, SIGNAL(clicked()), this, SLOT(changeDocumentTemplates()));
}

Prefs_Paths::~Prefs_Paths()
{
}

void Prefs_Paths::languageChange()
{
	docDirLineEdit->setToolTip( "<qt>" + tr( "Default documents directory" ) + "</qt>" );
	profileDirLineEdit->setToolTip( "<qt>" + tr( "Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc " ) + "</qt>" );
	scriptDirLineEdit->setToolTip( "<qt>" + tr( "Default Scripter scripts directory" ) + "</qt>" );
	templateDirLineEdit->setToolTip( "<qt>"+ tr("Additional directory for document templates")+"</qt>" );
}

void Prefs_Paths::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	docDirLineEdit->setText(prefsData->pathPrefs.documents);
	profileDirLineEdit->setText(prefsData->pathPrefs.colorProfiles);
	scriptDirLineEdit->setText(prefsData->pathPrefs.scripts);
	templateDirLineEdit->setText(prefsData->pathPrefs.documentTemplates);
}

void Prefs_Paths::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->pathPrefs.documents=docDirLineEdit->text();
	prefsData->pathPrefs.colorProfiles=profileDirLineEdit->text();
	prefsData->pathPrefs.scripts=scriptDirLineEdit->text();
	prefsData->pathPrefs.documentTemplates=templateDirLineEdit->text();
}

void Prefs_Paths::changeDocs()
{
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), docDirLineEdit->text());
	if (!s.isEmpty())
		docDirLineEdit->setText( QDir::convertSeparators(s) );
}

void Prefs_Paths::changeProfs()
{
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), profileDirLineEdit->text());
	if (!s.isEmpty())
		profileDirLineEdit->setText( QDir::convertSeparators(s) );
}

void Prefs_Paths::changeScripts()
{
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), scriptDirLineEdit->text());
	if (!s.isEmpty())
		scriptDirLineEdit->setText( QDir::convertSeparators(s) );
}

void Prefs_Paths::changeDocumentTemplates()
{
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), templateDirLineEdit->text());
	if (!s.isEmpty())
		templateDirLineEdit->setText( QDir::convertSeparators(s) );
}
