/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "runscriptdialog.h"
//#include "runscriptdialog.moc"
#include "qdir.h"
#include "prefsmanager.h"

RunScriptDialog::RunScriptDialog(QWidget* parent = 0, bool extEnable = false) :
	QFileDialog(parent, "runScriptDialog", true)
{
	this->extEnable = extEnable;
	PrefsManager *prefsManager=PrefsManager::instance();
	setDir(!prefsManager->appPrefs.ScriptDir.isEmpty() ? prefsManager->appPrefs.ScriptDir : QDir::currentDirPath());
	setFilters(tr("Python Scripts (*.py);; All Files (*)"));

	if (extEnable)
	{
		extChk = new QCheckBox(tr("Run as Extension Script", "run script dialog"), this);
		extChk->setChecked(false);
		addWidgets(0, extChk, 0);
	}
}

RunScriptDialog::~RunScriptDialog()
{
}

bool RunScriptDialog::extensionRequested()
{
	if (extEnable)
		return extChk->isChecked();
	else
		return false;
}
