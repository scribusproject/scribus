/* $Id$ */
#ifndef COLORWHEEL_H
#define COLORWHEEL_H

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
extern "C" void run(QWidget *d, ScribusApp *plug);


/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" QString name();


/** Returns the Type of the Plugin. */
extern "C" PluginManager::PluginType type();
/** ID for plugin registry */
extern "C" int ID();

/** menu settings */
extern "C" QString actionName();
extern "C" QString actionKeySequence();
extern "C" QString actionMenu();
extern "C" QString actionMenuAfterName();
extern "C" bool actionEnabledOnStartup();

#endif
