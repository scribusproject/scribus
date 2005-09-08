#ifndef SCRIPTERCORE_H
#define SCRIPTERCORE_H

#include "cmdvar.h"

#include "qmap.h"
#include "qguardedptr.h"

#include "menumanager.h"
#include "pconsole.h"

class ScrAction;
class MenuManager;


class ScripterCore : public QObject
{
	Q_OBJECT

public:
	ScripterCore(QWidget* parent);
	~ScripterCore();

public slots:
	void runScriptDialog();
	void StdScript(QString filebasename);
	void RecentScript(QString fn);
	void slotRunScriptFile(QString fileName, bool inMainInterpreter = false);
	QString slotRunScript(QString Script);
	void slotInteractiveScript(bool);
	void slotExecute();
	void aboutScript();
	/** Does setup for the main interpreter, particularly the interactive console. True for success. */
	bool setupMainInterpreter();
	/** Sets up the plugin for extension scripts, if enabled */
	void initExtensionScripts();
	/** Runs the startup script, if enabled */
	void runStartupScript();
	void languageChange();

	const QString & startupScript() const;
	bool extensionsEnabled() const;
	void setStartupScript(const QString& newScript);
	void setExtensionsEnabled(bool enable);

protected:
	// Private helper functions
	void FinishScriptRun();
	void ReadPlugPrefs();
	void SavePlugPrefs();
	void rebuildRecentScriptsMenu();
	void buildScribusScriptsMenu();
	void buildRecentScriptsMenu();
	void rebuildScribusScriptsMenu();

	//Internal members
	//! Reference to the "IDE" widget
	PythonConsole *pcon;
	int cons;
	int about;
	QStringList SavedRecentScripts;
	QStringList RecentScripts;
	MenuManager *menuMgr;
	QMap<QString, QGuardedPtr<ScrAction> > scrScripterActions;
	QMap<QString, QGuardedPtr<ScrAction> > scrRecentScriptActions;

	// Preferences
	/** pref: Enable access to main interpreter and 'extension scripts' */
	bool m_enableExtPython;
	/** pref: Run 'from scribus import *' at scripter startup */
	bool m_importAllNames;
	/** pref: Load this script on startup */
	QString m_startupScript;
};

#endif
