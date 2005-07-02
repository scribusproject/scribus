#include "pluginmanager.h"
#include "pluginmanager.moc"

#include <qdir.h>

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#include "scribus.h"
#include "scribusapp.h"
#include "menumanager.h"
#include "scraction.h"
#include "splash.h"
#include "mpalette.h"
#include "tree.h"
#include "prefsfile.h"
#include "scpaths.h"

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#elif defined(DLL_USE_NATIVE_API) && defined(_WIN32)
#include <windows.h>
#else
#include <qlibrary.h>
#endif

extern ScribusApp *ScApp;
extern ScribusQApp *ScQApp;
extern PrefsFile *prefsFile;


PluginManager::PluginManager()
{
	dllInput = "";
	dllReturn = "";
	prefs = prefsFile->getPluginContext("pluginmanager");
}

PluginManager::~PluginManager()
{
}

void* PluginManager::loadDLL( QString plugin )
{
	void* lib = NULL;
#ifdef HAVE_DLFCN_H
	QString libpath = QDir::convertSeparators( plugin );
	lib = dlopen(libpath, RTLD_LAZY | RTLD_GLOBAL);
	if (!lib)
	{
		char* error = dlerror();
		qDebug("%s: %s",
				tr("Cannot find plugin", "plugin manager").local8Bit().data(),
				error ? error : tr("unknown error","plugin manager").local8Bit().data());
	}
	dlerror();
#elif defined(DLL_USE_NATIVE_API) && defined(_WIN32)
	QString libpath = QDir::convertSeparators( plugin );
	HINSTANCE hdll = LoadLibrary( (const char*) libpath );
	lib = (void*) hdll;
#else
	if( QFile::exists(plugin) )
		lib = (void*) new QLibrary( plugin );
	else
		qDebug("%s \"%s\"", tr("Cannot find plugin", "plugin manager").local8Bit().data(), plugin.local8Bit().data());
#endif
	return lib;
}

void* PluginManager::resolveSym( void* plugin, const char* sym )
{
	void* symAddr = NULL;
#ifdef HAVE_DLFCN_H
	const char* error;
	dlerror();
	symAddr = dlsym( plugin, sym );
	if ((error = dlerror()) != NULL)
	{
		qDebug("%s", tr("Cannot find symbol (%1)", "plugin manager").arg(error).local8Bit().data());
		symAddr = NULL;
	}
#elif defined(DLL_USE_NATIVE_API) && defined(_WIN32)
	symAddr = (void* ) GetProcAddress( (HMODULE) plugin, sym);
	if ( symAddr == NULL)
		qDebug("%s", tr("Cannot find symbol (%1)", "plugin manager").arg(sym).local8Bit().data());
#else
	QLibrary* qlib = (QLibrary*) plugin;
	if( plugin ) symAddr = qlib->resolve(sym);
	if ( symAddr == NULL)
		qDebug("%s", tr("Cannot find symbol (%1)", "plugin manager").arg(sym).local8Bit().data());
#endif
	return symAddr;
}

void  PluginManager::unloadDLL( void* plugin )
{
#ifdef HAVE_DLFCN_H
	dlclose( plugin );
	dlerror();
#elif defined(DLL_USE_NATIVE_API) && defined(_WIN32)
	FreeLibrary( (HMODULE) plugin );
#else
	delete ( (QLibrary*) plugin );
#endif
}

void PluginManager::savePreferences()
{
	// write configuration
	for (QMap<int, PluginData>::Iterator it = pluginMap.begin(); it != pluginMap.end(); ++it)
		prefs->set(it.data().pluginFile, it.data().loadPlugin);
}

void PluginManager::initPlugs()
{
	QString name = "";
	int id = 0;
	struct PluginData pda;
	QString libPattern = QString("*.%1*").arg(PluginManager::platformDllExtension());

	QDir dirList(ScPaths::instance().pluginDir(),
				 libPattern, QDir::Name,
				 QDir::Files | QDir::Executable | QDir::NoSymLinks);

	if ((dirList.exists()) && (dirList.count() != 0))
	{
		ScApp->scrMenuMgr->addMenuSeparator("Extras");
		for (uint dc = 0; dc < dirList.count(); ++dc)
		{
			pda.index = 0;
			pda.pluginFile = "";
			pda.menuID = 0;
			pda.pluginFile = dirList[dc];
			pda.loadPlugin = prefs->getBool(dirList[dc], true);

			if (DLLname(dirList[dc], &pda.name, &pda.type, &pda.index, &id, &pda.actName, &pda.actKeySequence, &pda.actMenu, &pda.actMenuAfterName, &pda.actEnabledOnStartup, pda.loadPlugin))
			{
				pda.actMenuText=pda.name;
				if (ScApp->splashScreen != NULL)
					ScApp->splashScreen->setStatus( tr(QString("Loading: %1").arg(pda.name), "plugin manager"));
				if (pda.loadPlugin)
				{
					if (pda.type == Persistent || pda.type == Standard || pda.type == Import)
					{
						//Add in ScrAction based plugin linkage
						//Insert DLL Action into Dictionary with values from plugin interface
						ScApp->scrActions.insert(pda.actName, new ScrAction(ScrAction::DLL, QIconSet(), pda.name, QKeySequence(pda.actKeySequence), ScApp, pda.actName, id));

						if (ScApp->scrActions[pda.actName])
						{
							ScApp->scrActions[pda.actName]->setEnabled(pda.actEnabledOnStartup);
							//Connect DLL Action's activated signal with ID to Scribus DLL loader
							connect( ScApp->scrActions[pda.actName], SIGNAL(activatedData(int)) , ScApp->pluginManager, SLOT(callDLLBySlot(int)) );
							//Get the menu manager to add the DLL's menu item to the right menu, after the chosen existing item
							if (QString(pda.actMenuAfterName).length()==0)
								ScApp->scrMenuMgr->addMenuItem(ScApp->scrActions[pda.actName], pda.actMenu);
							else
								ScApp->scrMenuMgr->addMenuItemAfter(ScApp->scrActions[pda.actName], pda.actMenu, pda.actMenuAfterName);
						}
					}
					else
						qDebug( tr(QString("Old type plugins are not supported anymore")), "plugin manager");
					pda.loaded = true;
				} // load
				else
					pda.loaded = false;
				pluginMap.insert(id, pda);
			}
		}
	}
}

void PluginManager::callDLLBySlot(int pluginID)
{
	//Run old type 2 Import pre call code
	if (pluginMap[pluginID].type == 7)
	{
		if (ScApp->HaveDoc)
			ScApp->doc->OpenNodes = ScApp->outlinePalette->buildReopenVals();
	}

	callDLL(pluginID);

	//Run old type 2 Import post call code
	if (pluginMap[pluginID].type == 7)
	{
		if (ScApp->HaveDoc)
		{
			ScApp->outlinePalette->BuildTree(ScApp->doc);
			ScApp->outlinePalette->reopenTree(ScApp->doc->OpenNodes);
			ScApp->propertiesPalette->updateCList();
		}
	}
}

void PluginManager::callDLL(int pluginID)
{
	void *mo;
	struct PluginData pda;
	pda = pluginMap[pluginID];
	typedef void (*sdem)(QWidget *d, ScribusApp *plug);
	sdem demo;
	QString plugDir = ScPaths::instance().pluginDir();
	if (pda.type != 4 && pda.type !=5)
	{
		plugDir += pda.pluginFile;
		mo = loadDLL(plugDir);
		if (!mo) return;
	}
	else
		mo = pda.index;

	demo = (sdem) resolveSym(mo, "run");
	if ( !demo )
	{
		unloadDLL(mo);
		return;
	}
	(*demo)(ScApp, ScApp);
	// FIXME: how is the menu organized? (*demo)(ScApp->scrActions[pluginMap[pluginID].actName], ScApp);
	if (pda.type != 4 && pda.type != 5)
		unloadDLL(mo);
	if (ScApp->HaveDoc)
		ScApp->view->DrawNew();
}

QString PluginManager::callDLLForNewLanguage(int pluginID)
{
	void *mo;
	bool unload = false;
	struct PluginData pda;
	pda = pluginMap[pluginID];
	typedef QString (*sdem)();
	typedef void (*sdem0)();
	sdem demo;
	sdem0 demo0;
	QString plugDir = ScPaths::instance().pluginDir();
	if (pda.type != 4 && pda.type !=5)
	{
		plugDir += pda.pluginFile;
		mo = loadDLL(plugDir);
		if (!mo)
			return QString::null;
		unload = true;
	}
	else
		mo = pda.index;

	// Grab the menu string from the plugin again
	demo = (sdem) resolveSym(mo, "name");
	if ( !demo )
	{
		if(unload) unloadDLL(mo);
		return QString::null;
	}
	QString retVal= (*demo)();	
	//If scripter, get it to update its scrActions.
	if (pluginID==8)
	{
		demo0 = (sdem0) resolveSym(mo, "languageChange");
		if ( !demo0 )
		{
			if(unload) unloadDLL(mo);
			return QString::null;
		}
		(*demo0)();
	}

	if (pda.type != 4 && pda.type != 5)
		unloadDLL(mo);
	return retVal;
}

bool PluginManager::DLLexists(int pluginID)
{
	return pluginMap.contains(pluginID);
}

// used anywhere?
void PluginManager::callDLLbyMenu(int pluginID)
{
	QMap<int, PluginData>::Iterator it;
	struct PluginData pda;
	for (it = pluginMap.begin(); it != pluginMap.end(); ++it)
	{
		if (it.data().menuID == pluginID)
		{
			callDLL(it.key());
			break;
		}
	}
}

bool PluginManager::DLLname(QString name, QString *pluginName, PluginType *type, void **index, int *idNr, QString *actName, QString *actKeySequence, QString *actMenu, QString *actMenuAfterName, bool *actEnabledOnStartup, bool loadPlugin)
{
	void *mo;
	typedef QString (*sdem0)();
	typedef PluginType (*sdem1)();
	typedef void (*sdem2)(QWidget *d, ScribusApp *plug);
	typedef bool (*sdem3)();
	typedef int (*sdemID)();
	sdem0 demo;
	sdem1 demo1;
	sdem2 demo2;
	sdem3 demo3;
	sdemID plugID;
	QString plugName = ScPaths::instance().pluginDir();
	plugName += name;

	mo = loadDLL(plugName);
	if (!mo)
	{
		return false;
	}

	demo = (sdem0) resolveSym(mo, "name");
	if ( !demo )
	{
		unloadDLL(mo);
		return false;
	}
	*pluginName = (*demo)();
	demo1 = (sdem1) resolveSym(mo, "type");
	if ( !demo1 )
	{
		unloadDLL(mo);
		return false;
	}
	*type = (*demo1)();
	*index = mo;
	plugID = (sdemID) resolveSym(mo, "ID");
	if ( !plugID)
	{
		unloadDLL(mo);
		return false;
	}
	*idNr = (*plugID)();
	//ScrAction based plugins
	if (*type == Persistent || *type == Standard || *type == Import)
	{
		demo = (sdem0) resolveSym(mo, "actionName");
		if ( !demo )
		{
			unloadDLL(mo);
			return false;
		}
		*actName = (*demo)();
		demo = (sdem0) resolveSym(mo, "actionKeySequence");
		if ( !demo )
		{
			unloadDLL(mo);
			return false;
		}
		*actKeySequence = (*demo)();
		demo = (sdem0) resolveSym(mo, "actionMenu");
		if ( !demo )
		{
			unloadDLL(mo);
			return false;
		}
		*actMenu = (*demo)();
		demo = (sdem0) resolveSym(mo, "actionMenuAfterName");
		if ( !demo )
		{
			unloadDLL(mo);
			return false;
		}
		*actMenuAfterName = (*demo)();
		demo3 = (sdem3) resolveSym(mo, "actionEnabledOnStartup");
		if ( !demo3 )
		{
			unloadDLL(mo);
			return false;
		}
		*actEnabledOnStartup = (*demo3)();
	}
	else
	{
		*actName = QString::null;
		*actKeySequence = QString::null;
		*actMenu = QString::null;
		*actMenuAfterName = QString::null;
		*actEnabledOnStartup = false;
	}
	if (*type != Persistent && *type!= Type5)
		unloadDLL(mo);
	else
	{
		if (loadPlugin)
		{
			demo2 = (sdem2) resolveSym(mo, "initPlug");
			if ( !demo2)
			{
				unloadDLL(mo);
				return false;
			}
			(*demo2)(ScApp, ScApp);
		}
	}

	return true;
}

void PluginManager::finalizePlugs()
{
	for (QMap<int, PluginData>::Iterator it = pluginMap.begin(); it != pluginMap.end(); ++it)
		if (it.data().loaded == true)
			finalizePlug(it.key());
}

void PluginManager::finalizePlug(int pluginID)
{
	struct PluginData pda;
	typedef void (*sdem2)();
	sdem2 demo2;
	PluginData plug = pluginMap[pluginID];
	if (plug.type == Persistent || plug.type == Type5)
	{
		demo2 = (sdem2) resolveSym(plug.index, "cleanUpPlug");
		if ( demo2 )
			(*demo2)();
		unloadDLL(plug.index);
	}
}

QString PluginManager::getPluginType(PluginType aType)
{
	switch(aType)
	{
		case Persistent:
			return tr("Persistent", "plugin manager");
			break;
		case Import:
			return tr("Import", "plugin manager");
			break;
		case Standard:
			return tr("Standard", "plugin manager");
			break;
		default:
			return tr("Unknown", "plugin manager");
	}
}

QCString PluginManager::platformDllExtension()
{
#ifdef __hpux
	// HP/UX
	return "sl";
#elif defined(__APPLE__) && defined(__MACH__)
	// MacOS/X, Darwin

	// MacOS/X may actually use both 'so' and 'dylib'. .so is usually used
	// for plugins etc, dylib for system and app libraries We need to
	// support this distinction in the plugin manager, but for now it's
	// most appropriate to return the extension used by plugins -- CR

	//return "dylib";
	return "so";
#elif defined(_WIN32) || defined(_WIN64)
	return "dll";
#else
	// Generic *NIX
	return "so";
#endif
}

void PluginManager::languageChange()
{
	for (QMap<int, PluginData>::Iterator it = pluginMap.begin(); it != pluginMap.end(); ++it)
	{
		QString fromTranslator=callDLLForNewLanguage(it.key());
		ScrAction* pluginAction=ScApp->scrActions[(*it).actName];
		if (fromTranslator!=QString::null)
			(*it).name=fromTranslator;
		if (pluginAction!=NULL)
			pluginAction->setMenuText(fromTranslator);
	}
}
