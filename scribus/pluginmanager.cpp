/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pluginmanager.h"
#include "pluginmanager.moc"
#include "scplugin.h"
#include "loadsaveplugin.h"

#include <qdir.h>

#include "scconfig.h"

#include "scribus.h"
#include "scribuscore.h"
#include "menumanager.h"
#include "scraction.h"
#include "splash.h"
#include "mpalette.h"
#include "tree.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "scpaths.h"
#include "commonstrings.h"

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#elif defined(DLL_USE_NATIVE_API) && defined(_WIN32)
#include <windows.h>
#else
#include <qlibrary.h>
#endif


extern ScribusQApp *ScQApp;

PluginManager::PluginManager() :
	QObject(0),
	prefs(PrefsManager::instance()->prefsFile->getPluginContext("pluginmanager"))
{
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
		const char* error = dlerror();
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
	for (PluginMap::Iterator it = pluginMap.begin(); it != pluginMap.end(); ++it)
		prefs->set(it.data().pluginName, it.data().enableOnStartup);
}

QCString PluginManager::getPluginName(QString fileName)
{
	// Must return plug-in name. Note that this may be platform dependent;
	// it's likely to need some adjustment for platform naming schemes.
	// It currently handles:
	//    (lib)?pluginname(\.pluginext)?
	QFileInfo fi(fileName);
	QString baseName(fi.baseName());
	if (baseName.startsWith("lib"))
		baseName = baseName.remove(0,3);
	if (baseName.endsWith(platformDllExtension()))
		baseName = baseName.left(baseName.length() - 1 - platformDllExtension().length());
	// check name
	for (int i = 0; i < (int)baseName.length(); i++)
		if (! baseName[i].isLetterOrNumber() && baseName[i] != '_' )
		{
			qDebug("Invalid character in plugin name for %s; skipping",
					fileName.local8Bit().data());
			return QCString();
		}
	return baseName.latin1();
}

int PluginManager::initPlugin(const QString fileName)
{
	PluginData pda;
	pda.pluginFile = QString("%1/%2").arg(ScPaths::instance().pluginDir()).arg(fileName);
	pda.pluginName = getPluginName(pda.pluginFile);
	if (pda.pluginName.isNull())
		// Couldn't determine plugname from filename. We've already complained, so
		// move on to the next one.
		return 0;
	pda.plugin = 0;
	pda.pluginDLL = 0;
	pda.enabled = false;
	pda.enableOnStartup = prefs->getBool(pda.pluginName, true);
	ScCore->setSplashStatus(tr("Plugin: loading %1", "plugin manager").arg(pda.pluginName));
	if (loadPlugin(pda))
	{
		if (pda.enableOnStartup)
			enablePlugin(pda);
		pluginMap.insert(pda.pluginName, pda);
		return 1;
	}
	return 0;
}

void PluginManager::initPlugs()
{
	Q_ASSERT(!pluginMap.count());
	QString libPattern = QString("*.%1*").arg(platformDllExtension());
	QMap<QString,int> allPlugs;
	uint loaded = 0;
	uint changes = 1;
	QStringList failedPlugs; // output string for warn dialog

	/*! \note QDir::Reversed is there due the Mac plugin dependency.
	barcode depends on psimport. and load on that platform expect the
	psimp before barcode.You know, security by obscurity ;) PV */
	QDir dirList(ScPaths::instance().pluginDir(),
				 libPattern, QDir::Name | QDir::Reversed,
				 QDir::Files | QDir::Executable | QDir::NoSymLinks);

	if ((!dirList.exists()) || (dirList.count() == 0))
		return;
	for (uint dc = 0; dc < dirList.count(); ++dc)
	{
		int res = initPlugin(dirList[dc]);
		allPlugs[dirList[dc]] = res;
		if (res != 0)
			++loaded;
		else
			failedPlugs.append(dirList[dc]);
	}
	/* Re-try the failed plugins again and again until it promote
	any progress (changes variable is changing ;)) */
	while (loaded < allPlugs.count() && changes!=0)
	{
		changes = 0;
		QMap<QString,int>::Iterator it;
		for (it = allPlugs.begin(); it != allPlugs.end(); ++it)
		{
			if (it.data() != 0)
				continue;
			int res = initPlugin(it.key());
			allPlugs[it.key()] = res;
			if (res == 1)
			{
				++loaded;
				++changes;
				failedPlugs.remove(it.key());
			}
		}
	}
	if (loaded != allPlugs.count())
	{
		if (ScCore->usingGUI())
		{
			bool splashShown = ScCore->splashShowing();
			QString failedStr("<ul>");
			for (QStringList::Iterator it = failedPlugs.begin(); it != failedPlugs.end(); ++it)
				failedStr += "<li>" + *it + "</li>";
			failedStr += "</ul>";
			if (splashShown)
				ScCore->showSplash(false);
			QMessageBox::warning(ScMW, CommonStrings::trWarning,
								 "<qt>" + tr("There is a problem loading %1 of %2 plugins. %3 This is probably caused by some kind of dependency. Report it as a bug, please."
										).arg(allPlugs.count()-loaded).arg(allPlugs.count()).arg(failedStr)
									 + "</qt>",
							 CommonStrings::tr_OK);
			if (splashShown)
				ScCore->showSplash(true);
		}
	}
}

// After a plug-in has been initialized, this method calls its setup
// routines and connects it to the application.
void PluginManager::enablePlugin(PluginData & pda)
{
	Q_ASSERT(pda.enabled == false);
	QString failReason;
	bool isActionPlugin=false;
	if (pda.plugin->inherits("ScActionPlugin"))
	{
		ScActionPlugin* plugin = dynamic_cast<ScActionPlugin*>(pda.plugin);
		Q_ASSERT(plugin);
		isActionPlugin=true;
		/*
		pda.enabled = setupPluginActions(plugin);
		if (!pda.enabled)
			failReason = tr("init failed", "plugin load error");
		*/
	}
	else if (pda.plugin->inherits("ScPersistentPlugin"))
	{
		ScPersistentPlugin* plugin = dynamic_cast<ScPersistentPlugin*>(pda.plugin);
		Q_ASSERT(plugin);
		pda.enabled = plugin->initPlugin();
		if (!pda.enabled)
			failReason = tr("init failed", "plugin load error");
	}
/* temporary hack to enable the import plugins */
	else if (pda.plugin->inherits("LoadSavePlugin"))
		pda.enabled = true;
	else
		failReason = tr("unknown plugin type", "plugin load error");
	if (pda.enabled || isActionPlugin)
		ScCore->setSplashStatus(
				tr("Plugin: %1 loaded", "plugin manager")
				.arg(pda.plugin->fullTrName()));
	else
		ScCore->setSplashStatus(
				tr("Plugin: %1 failed to load: %2", "plugin manager")
				.arg(pda.plugin->fullTrName()).arg(failReason));
}

bool PluginManager::setupPluginActions(ScribusMainWindow *mw)
{
	Q_CHECK_PTR(mw);
	mw->scrMenuMgr->addMenuSeparator("Extras");
	for (PluginMap::Iterator it = pluginMap.begin(); it != pluginMap.end(); ++it)
	{
		if (it.data().plugin->inherits("ScActionPlugin"))
		{
			//Add in ScrAction based plugin linkage
			//Insert DLL Action into Dictionary with values from plugin interface
			ScActionPlugin* plugin = dynamic_cast<ScActionPlugin*>(it.data().plugin);
			Q_ASSERT(plugin);
			ScActionPlugin::ActionInfo ai(plugin->actionInfo());
			ScrAction* action = new ScrAction(
					ScrAction::DLL, ai.iconSet, ai.text, ai.keySequence,
					mw, ai.name);
			Q_CHECK_PTR(action);
			mw->scrActions.insert(ai.name, action);
		
			// then enable and connect up the action
			mw->scrActions[ai.name]->setEnabled(ai.enabledOnStartup);
			// Connect action's activated signal with the plugin's run method
			it.data().enabled = connect( mw->scrActions[ai.name], SIGNAL(activated()),
							plugin, SLOT(run()) );
			//Get the menu manager to add the DLL's menu item to the right menu, after the chosen existing item
			if ( ai.menuAfterName.isEmpty() )
				mw->scrMenuMgr->addMenuItem(mw->scrActions[ai.name], ai.menu);
			else
				mw->scrMenuMgr->addMenuItemAfter(mw->scrActions[ai.name], ai.menu, ai.menuAfterName);
			if (it.data().enabled)
				ScCore->setSplashStatus(tr("Plugin: %1 initialized ok ", "plugin manager")
						.arg(plugin->fullTrName()));
			else
				ScCore->setSplashStatus(tr("Plugin: %1 failed post initialization", "plugin manager")
						.arg(plugin->fullTrName()));
		}
		else
		{
			it.data().plugin->addToMainWindowMenu(mw);
		}
		
	}
	return true;
}

bool PluginManager::DLLexists(QCString name, bool includeDisabled) const
{
	// the plugin name must be known
	if (pluginMap.contains(name))
	{
		// the plugin must be loaded
		if (pluginMap[name].plugin)
		{
			// and the plugin must be enabled unless we were told otherwise
			if (pluginMap[name].enabled)
				return true;
			else
				return includeDisabled;
		}
	}
	return false;
}

bool PluginManager::loadPlugin(PluginData & pda)
{
	typedef int (*getPluginAPIVersionPtr)();
	typedef ScPlugin* (*getPluginPtr)();
	getPluginAPIVersionPtr getPluginAPIVersion;
	getPluginPtr getPlugin;

	Q_ASSERT(pda.plugin == 0);
	Q_ASSERT(pda.pluginDLL == 0);
	Q_ASSERT(!pda.enabled);
	pda.plugin = 0;

	pda.pluginDLL = loadDLL(pda.pluginFile);
	if (!pda.pluginDLL)
		return false;

	getPluginAPIVersion = (getPluginAPIVersionPtr)
		resolveSym(pda.pluginDLL, pda.pluginName + "_getPluginAPIVersion");
	if (getPluginAPIVersion)
	{
		int gotVersion = (*getPluginAPIVersion)();
		if ( gotVersion != PLUGIN_API_VERSION )
		{
			qDebug("API version mismatch when loading %s: Got %i, expected %i",
					pda.pluginFile.local8Bit().data(), gotVersion, PLUGIN_API_VERSION);
		}
		else
		{
			getPlugin = (getPluginPtr)
				resolveSym(pda.pluginDLL, pda.pluginName + "_getPlugin");
			if (getPlugin)
			{
				pda.plugin = (*getPlugin)();
				if (!pda.plugin)
				{
					qDebug("Unable to get ScPlugin when loading %s",
							pda.pluginFile.local8Bit().data());
				}
				else
					return true;
			}
		}
	}
	unloadDLL(pda.pluginDLL);
	pda.pluginDLL = 0;
	Q_ASSERT(!pda.plugin);
	return false;
}

void PluginManager::cleanupPlugins()
{
	for (PluginMap::Iterator it = pluginMap.begin(); it != pluginMap.end(); ++it)
		if (it.data().enabled == true)
			finalizePlug(it.data());
}

void PluginManager::finalizePlug(PluginData & pda)
{
	typedef void (*freePluginPtr)(ScPlugin* plugin);
	if (pda.plugin)
	{
		if (pda.enabled)
			disablePlugin(pda);
		Q_ASSERT(!pda.enabled);
		freePluginPtr freePlugin =
			(freePluginPtr) resolveSym(pda.pluginDLL, pda.pluginName + "_freePlugin");
		if ( freePlugin )
			(*freePlugin)( pda.plugin );
		pda.plugin = 0;
	}
	Q_ASSERT(!pda.enabled);
	if (pda.pluginDLL)
	{
		unloadDLL(pda.pluginDLL);
		pda.pluginDLL = 0;
	}
}

void PluginManager::disablePlugin(PluginData & pda)
{
	Q_ASSERT(pda.enabled);
	Q_ASSERT(pda.plugin);
	if (pda.plugin->inherits("ScActionPlugin"))
	{
		ScActionPlugin* plugin = dynamic_cast<ScActionPlugin*>(pda.plugin);
		Q_ASSERT(plugin);
		// FIXME: Correct way to delete action?
		delete ScMW->scrActions[plugin->actionInfo().name];
	}
	else if (pda.plugin->inherits("ScPersistentPlugin"))
	{
		ScPersistentPlugin* plugin = dynamic_cast<ScPersistentPlugin*>(pda.plugin);
		Q_ASSERT(plugin);
		plugin->cleanupPlugin();
	}
/* temporary hack to enable the import plugins */
	else if (pda.plugin->inherits("LoadSavePlugin"))
		pda.enabled = false;
	else
		Q_ASSERT(false); // We shouldn't ever have enabled an unknown plugin type.
	pda.enabled = false;
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
	for (PluginMap::Iterator it = pluginMap.begin(); it != pluginMap.end(); ++it)
	{
		ScPlugin* plugin = it.data().plugin;
		if (plugin)
		{
			plugin->languageChange();
			ScActionPlugin* ixplug = dynamic_cast<ScActionPlugin*>(plugin);
			if (ixplug)
			{
				ScActionPlugin::ActionInfo ai(ixplug->actionInfo());
				ScrAction* pluginAction = ScMW->scrActions[ai.name];
				if (pluginAction != 0)
					pluginAction->setMenuText( ai.text );
			}
		}
	}
}

ScPlugin* PluginManager::getPlugin(const QCString & pluginName, bool includeDisabled) const
{
	if (DLLexists(pluginName, includeDisabled))
		return pluginMap[pluginName].plugin;
	return 0;
}

// Compatability kludge
bool PluginManager::callSpecialActionPlugin(const QCString pluginName, const QString & arg, QString & retval)
{
	bool result = false;
	if (DLLexists(pluginName))
	{
		ScActionPlugin* plugin =
			dynamic_cast<ScActionPlugin*>(pluginMap[pluginName].plugin);
		if (plugin)
			result = plugin->run(arg);
	}
	if (result)
	{
		retval = dynamic_cast<ScActionPlugin*>(pluginMap[pluginName].plugin)->runResult();
		result = true;
	}
	return result;
}

PluginManager & PluginManager::instance()
{
	return (*ScCore->pluginManager);
}

const QString & PluginManager::getPluginPath(const QCString pluginName) const
{
	// It is not legal to call this function without a valid
	// plug in name.
	Q_ASSERT(pluginMap.contains(pluginName));
	return pluginMap[pluginName].pluginFile;
}

bool & PluginManager::enableOnStartup(const QCString pluginName)
{
	// It is not legal to call this function without a valid
	// plug in name.
	Q_ASSERT(pluginMap.contains(pluginName));
	return pluginMap[pluginName].enableOnStartup;
}

bool PluginManager::enabled(const QCString pluginName)
{
	// It is not legal to call this function without a valid
	// plug in name.
	Q_ASSERT(pluginMap.contains(pluginName));
	return pluginMap[pluginName].enabled;
}

QValueList<QCString> PluginManager::pluginNames(
		bool includeDisabled, const char* inherits) const
{
	// Scan the plugin map for plugins...
	QValueList<QCString> names;
	for (PluginMap::ConstIterator it = pluginMap.constBegin(); it != pluginMap.constEnd(); ++it)
		if (includeDisabled || it.data().enabled)
			// Only including plugins that inherit a named parent (if
			// specified), using the QMetaObject system.
			if (!inherits || it.data().plugin->inherits(inherits))
				names.append(it.data().pluginName);
	return names;
}
