/****************************************************************************
 ** ui.h extension file, included from the uic-generated form implementation.
 **
 ** If you want to add, delete, or rename functions or slots, use
 ** Qt Designer to update this file, preserving your code.
 **
 ** You should not define a constructor or destructor in this file.
 ** Instead, write your code in functions called init() and destroy().
 ** These will automatically be called by the form's constructor and
 ** destructor.
 *****************************************************************************/

#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qdir.h>

/* PLEASE continue using Qt designer to create this file rather than hacking
 * the generated C++. Doing so will make maintaining the prefs dialog, and
 * site-specific customisation, much easier. */

void ScripterPreferences::browseForScript()
{
	QString currentScript = this->startupScriptPath->text();
	if (! QFileInfo(currentScript).exists() )
	{
		currentScript = QDir::homeDirPath();
	}
	QString scriptPath = QFileDialog::getOpenFileName(
			currentScript, "Python Scripts (*.py)", this,
			"Open File Dialog", "Select a script to use as a startup script");
	if (scriptPath != "")
	{
		emit pathChanged(scriptPath);
	}
}
