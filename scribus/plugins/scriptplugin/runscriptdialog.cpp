#include "runscriptdialog.h"
#include "qdir.h"
#include "prefsmanager.h"

RunScriptDialog::RunScriptDialog(QWidget* parent = 0, bool extEnable = false) :
	QFileDialog(parent, "runScriptDialog", true)
{
	this->extEnable = extEnable;
	PrefsManager *prefsManager=PrefsManager::instance();
	setDir(prefsManager->appPrefs.ScriptDir != "" ? prefsManager->appPrefs.ScriptDir : QDir::currentDirPath());
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
