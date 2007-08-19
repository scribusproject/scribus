/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "runscriptdialog.h"
#include "prefsmanager.h"

RunScriptDialog::RunScriptDialog(QWidget* parent = 0, bool extEnable = false) :
	QDialog(parent)
{
	setupUi(this);

	m_extEnable = extEnable;
	PrefsManager *prefsManager=PrefsManager::instance();
	fileWidget->setDir(!prefsManager->appPrefs.ScriptDir.isEmpty() ? prefsManager->appPrefs.ScriptDir : QDir::currentPath());
	fileWidget->setFilter( tr("Python Scripts (*.py *.PY);; All Files (*)"));

	if (!extEnable)
		extChk->setVisible(false);

	connect(fileWidget, SIGNAL(accepted()), this, SLOT(accept()));
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
