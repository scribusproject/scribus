#ifndef SCRIPTPLUG_H
#define SCRIPTPLUG_H

#include "scribus.h"
#include "pconsole.h"


/** Calls the Plugin with the main Application window as parent
  * and the main Application Class as parameter */
extern "C" void Run(QWidget *d, ScribusApp *plug);

/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" QString Name();

/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugin is a Import Plugin, which appears in the Import Menue
  * 3 = the Plugin is a Export Plugin, which appears in the Export Menue
  * 4 = the Plugin is a resident Plugin   */
extern "C" int Type();
extern "C" int ID();

/** Initializes the Plugin if it's a Plugin of Type 4 */
extern "C" void InitPlug(QWidget *d, ScribusApp *plug);

/** Possible CleanUpOperations when closing the Plugin */
extern "C" void CleanUpPlug();

extern "C" void initscribus(ScribusApp *pl);

/** Some useful Subroutines */
static PyObject *scribus_retval(PyObject *self, PyObject* args);
static PyObject *scribus_getval(PyObject *self, PyObject* args);
QString RetString;
QString InValue;
int RetVal;

static QPopupMenu* men;

class MenuTest : public QObject
{
    Q_OBJECT

public:
    MenuTest(QWidget* parent) {};
    ~MenuTest() {};
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

public slots:
		void slotTest();
		void StdScript(int id);
		void RecentScript(int id);
		void slotRunScriptFile(QString fileName);
		QString slotRunScript(QString Script);
		void slotInteractiveScript();
		void slotExecute();
		void aboutScript();
};

static MenuTest* Tes;
#endif // CMSPLUG_H
