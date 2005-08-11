/* $Id$ */
#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include "pluginapi.h"

#include "scribus.h"
#include "pluginmanager.h"

/** \brief This is a simple "Color Theory" plugin for Scribus 1.3 and later.
Harmonious colors are colors that work well together, that produce a color
scheme that looks attractive; the color wheel can be used as a valuable
tool for determining harmonious colors.
More on: http://en.wikipedia.org/wiki/Color_wheel
\author Petr Vanek; petr@yarpen.cz
\date April 2005
*/

/** Calls the Plugin with the main Application window as parent
  * and the main Application Class as parameter */
extern "C" PLUGIN_API void run(QWidget *d, ScribusApp *plug);


/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" PLUGIN_API QString name();


/** Returns the Type of the Plugin. */
extern "C" PLUGIN_API PluginManager::PluginType type();
/** ID for plugin registry */
extern "C" PLUGIN_API int ID();

/** menu settings */
extern "C" PLUGIN_API QString actionName();
extern "C" PLUGIN_API QString actionKeySequence();
extern "C" PLUGIN_API QString actionMenu();
extern "C" PLUGIN_API QString actionMenuAfterName();
extern "C" PLUGIN_API  bool actionEnabledOnStartup();

#endif
