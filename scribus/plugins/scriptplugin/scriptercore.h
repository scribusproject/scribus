#ifndef SCRIPTERCORE_H
#define SCRIPTERCORE_H

#include "cmdvar.h"

#include "qmap.h"
#include "qguardedptr.h"

#include "menumanager.h"
#include "pconsole.h"
#include "conswin.h"

class ScrAction;
class MenuManager;

class ScripterCore : public QObject
{
	Q_OBJECT

public:
	ScripterCore(QWidget* parent);
	~ScripterCore();

public slots:
	void slotTest();
	void StdScript(QString filebasename);
	void RecentScript(QString fn);
	void slotRunScriptFile(QString fileName);
	QString slotRunScript(QString Script);
	void slotInteractiveScript(bool);
	void slotExecute();
	void aboutScript();

protected:
	// Private helper functions
	void FinishScriptRun();
	void ReadPlugPrefs();
	void SavePlugPrefs();
	void rebuildRecentScriptsMenu();
	void buildScribusScriptsMenu();
	void buildRecentScriptsMenu();
	void rebuildScribusScriptsMenu();

	// Internal members
	PConsole pcon;
	int cons;
	int about;
	QStringList SavedRecentScripts;
	QStringList RecentScripts;
	MenuManager *menuMgr;
	QMap<QString, QGuardedPtr<ScrAction> > scrScripterActions;
	QMap<QString, QGuardedPtr<ScrAction> > scrRecentScriptActions;
};

#endif
