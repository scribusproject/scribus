#ifndef SCRIPTER_CORE_H
#define SCRIPTER_CORE_H

#include "cmdvar.h"
#include "qobject.h"
#include "qstring.h"
#include "qstringlist.h"

class PConsole;
class ScribusApp;
class QPopupMenu;

class ScripterCore : public QObject
{
    Q_OBJECT

public:
		ScripterCore(QWidget* parent);
		~ScripterCore();
		void ReadPlugPrefs();
		void SavePlugPrefs();
		void FinishScriptRun();
		PConsole *pcon;
		QPopupMenu* rmen;
		QPopupMenu* smen;
		int rmenid;
		int smenid;
		int cons;
		int about;
		QStringList SavedRecentScripts;
		QStringList RecentScripts;
		/// pref: Enable the "load script" menu item?
		bool enableExtPython;
		/// pref: Run 'from scribus import *' in the main interpreter at startup?
		bool importAllNames;
		/// pref: Make aliases of renamed functions for compatibility?
		bool legacyAliases;
		/// pref: Replace stdin with dummy cStringIO?
		bool useDummyStdin;
		/// pref: Run a script in the main interpreter at startup?
		bool startupScriptEnable;
		/// pref: Which script to run at startup, if enabled
		QString startupScript;
		// The script plugin's menu
		QPopupMenu* men;
public slots:
		void slotTest();
		void loadScript();
		void StdScript(int id);
		void RecentScript(int id);
		void slotRunScriptFile(QString fileName, bool inMainInterpreter = false);
		QString slotRunScript(QString Script);
		void slotInteractiveScript();
		void slotExecute();
		void aboutScript();
		void preferencesDialog();
		void runStartupScript();
};

#endif
