#ifndef SCRIPTPLUG_H
#define SCRIPTPLUG_H

#include "pluginmanager.h"

/** Calls the Plugin with the main Application window as parent
  * and the main Application Class as parameter */
extern "C" void run(QWidget *d, ScribusApp *plug);

/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" QString name();

/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugin is a Import Plugin, which appears in the Import Menue
  * 3 = the Plugin is a Export Plugin, which appears in the Export Menue
  * 4 = the Plugin is a resident Plugin   */
extern "C" PluginManager::PluginType type();
extern "C" int ID();
extern "C" QString actionName();
extern "C" QString actionKeySequence();
extern "C" QString actionMenu();
extern "C" QString actionMenuAfterName();
extern "C" bool actionEnabledOnStartup();
extern "C" void languageChange();

/** Initializes the Plugin if it's a Plugin of Type 4 */
extern "C" void initPlug(QWidget *d, ScribusApp *plug);

/** Possible CleanUpOperations when closing the Plugin */
extern "C" void cleanUpPlug();

/** Some useful Subroutines */
static PyObject *scribus_retval(PyObject *self, PyObject* args);
static PyObject *scribus_getval(PyObject *self);

#endif // CMSPLUG_H
