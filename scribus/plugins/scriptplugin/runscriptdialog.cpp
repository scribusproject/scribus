/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QDir>
#include "runscriptdialog.h"
#include "prefsmanager.h"

QString RunScriptDialog::m_lastScriptDir;

RunScriptDialog::RunScriptDialog(QWidget* parent = 0, bool extEnable = false) :
	QDialog(parent)
{
	setupUi(this);

	m_extEnable = extEnable;
	PrefsManager *prefsManager = PrefsManager::instance();
	QString       scriptDir    = prefsManager->appPrefs.ScriptDir;
	if (!m_lastScriptDir.isEmpty() && QDir(m_lastScriptDir).exists())
		fileWidget->setDirectory(m_lastScriptDir);
	else if (!scriptDir.isEmpty() && QDir(scriptDir).exists())
		fileWidget->setDirectory(scriptDir);
	else
		fileWidget->setDirectory(QDir::current());
	fileWidget->setFilter( tr("Python Scripts (*.py *.PY);; All Files (*)"));

	if (!extEnable)
		extChk->setVisible(false);

	connect(fileWidget, SIGNAL(accepted()), this, SLOT(accept()));
	connect(fileWidget, SIGNAL(rejected()), this, SLOT(reject()));
}

RunScriptDialog::~RunScriptDialog()
{
}

bool RunScriptDialog::extensionRequested()
{
	if (m_extEnable)
		return extChk->isChecked();
	else
		return false;
}

QString RunScriptDialog::selectedFile()
{
	return fileWidget->selectedFile();
}

void RunScriptDialog:: accept()
{
	m_lastScriptDir = fileWidget->directory().path();
	QDialog::accept();
}
