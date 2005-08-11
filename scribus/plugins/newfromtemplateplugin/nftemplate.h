#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "pluginapi.h"
#include "scribus.h"
#include "nftdialog.h"
#include "pluginmanager.h"

class ScrAction;

/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" PLUGIN_API QString name();


/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugin is a Import Plugin, which appears in the Import Menue
  * 3 = the Plugin is a Export Plugin, which appears in the Export Menue
  * 4 = the Plugin is a resident Plugin   */
extern "C" PLUGIN_API PluginManager::PluginType type();

///** Initializes the Plugin if it's a Plugin of Type 4 or 5 */
//extern "C" void InitPlug(QWidget *d, ScribusApp *plug);
/** Type 6 plugin needs this again */
extern "C" PLUGIN_API void run(QWidget *d, ScribusApp *plug);

/** Possible CleanUpOperations when closing the Plugin */
extern "C" PLUGIN_API void cleanUpPlug();
extern "C" PLUGIN_API int ID();

extern "C" PLUGIN_API QString actionName();
extern "C" PLUGIN_API QString actionKeySequence();
extern "C" PLUGIN_API QString actionMenu();
extern "C" PLUGIN_API QString actionMenuAfterName();
extern "C" PLUGIN_API bool actionEnabledOnStartup();

class MenuNFT : public QObject
{
	Q_OBJECT

public:
	MenuNFT(QWidget* /*parent*/) {};
    ~MenuNFT() {};

public slots:
	void RunNFTPlug();
};

static MenuNFT* Nft;

#endif
