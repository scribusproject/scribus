#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "scribus.h"

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

#endif
