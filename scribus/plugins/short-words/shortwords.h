/*! This is the Scribus Short Words plugin interface specification.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>

This program is free software - see LICENSE file in the distribution
or documentation
*/

#ifndef _SCRIBUS_SHORTWORDS_H_
#define _SCRIBUS_SHORTWORDS_H_

#include "config.h"
#include "configuration.h"
#include "parse.h"
#include "vlnadialog.h"

#include "scribus.h"

/** Calls the Plugin with the main Application window as parent
and the main Application Class as parameter. Loads translator too.
*/
extern "C" void Run(QWidget *d, ScribusApp *plug);


/** Returns the Name of the Plugin.
 This name appears in the relevant Menue-Entrys
 */
extern "C" QString Name();


/** Returns the Type of the Plugin.
  \retval 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  */
extern "C" int Type();

/** Returns the Id of the Plugin.
  \retval 11 = id from the plugin registry
 */
extern "C" int ID();

extern "C" QString actionName();
extern "C" QString actionKeySequence();
extern "C" QString actionMenu();
extern "C" QString actionMenuAfterName();
extern "C" bool actionEnabledOnStartup();

class Parse;

/** Base class of this plugin */
class ShortWords : public QObject
{
	Q_OBJECT

public:
	/** inicialization - Raise dialog, handles parsing etc. */
	ShortWords();
	/** clear memaory, write cfg. */
	~ShortWords();

	/** actual page when user calls this plug */
	uint originalPage;
	/** All available configuration object */
	Config *cfg;
	/** all available "engine" */
	Parse *parse;
};


#endif
