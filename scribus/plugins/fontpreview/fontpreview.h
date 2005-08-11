#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "pluginapi.h"
#include "scribus.h"
#include "pluginmanager.h"

/** Calls the Plugin with the main Application window as parent
  * and the main Application Class as parameter */
extern "C" PLUGIN_API void run(QWidget *d, ScribusApp *plug);


/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" PLUGIN_API QString name();


/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugin is a Import Plugin, which appears in the Import Menue
  * 3 = the Plugin is a Export Plugin, which appears in the Export Menue
  * 4 = the Plugin is a resident Plugin   */
extern "C" PLUGIN_API PluginManager::PluginType type();
extern "C" PLUGIN_API int ID();
extern "C" PLUGIN_API QString actionName();
extern "C" PLUGIN_API QString actionKeySequence();
extern "C" PLUGIN_API QString actionMenu();
extern "C" PLUGIN_API QString actionMenuAfterName();
extern "C" PLUGIN_API bool actionEnabledOnStartup();

#endif
