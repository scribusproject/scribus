#include "pluginmanager.h"
#include "pluginmanager.moc"
#include <dlfcn.h>
#include <qdir.h>
#include "scribus.h"
#include "menumanager.h"
#include "scraction.h"
#include "splash.h"
#include "mpalette.h"
#include "tree.h"
#include "prefsfile.h"

extern ScribusApp *ScApp;
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
#if defined(__hpux)
	QDir dirList(PLUGINDIR, "*.sl*", QDir::Name, QDir::Files | QDir::Executable | QDir::NoSymLinks);
#else
	QDir dirList(PLUGINDIR, "*.so*", QDir::Name, QDir::Files | QDir::Executable | QDir::NoSymLinks);
#endif
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
				if (ScApp->splashScreen != NULL)
					ScApp->splashScreen->setStatus(tr(QString("Loading: %1").arg(pda.name), "plugin manager"));
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
						qDebug(tr(QString("Old type plugins are not supported anymore")), "plugin manager");
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
	const char *error;
	struct PluginData pda;
	pda = pluginMap[pluginID];
	typedef void (*sdem)(QWidget *d, ScribusApp *plug);
	sdem demo;
	QString plugDir = PLUGINDIR;
	if (pda.type != 4 && pda.type !=5)
	{
		plugDir += pda.pluginFile;
		mo = dlopen(plugDir, RTLD_LAZY | RTLD_GLOBAL);
		if (!mo)
		{
			qDebug(tr("Can't find Plug-in"), "plugin manager");
			return;
		}
	}
	else
		mo = pda.index;
	dlerror();
	demo = (sdem)dlsym(mo, "run");
	if ((error = dlerror()) != NULL)
	{
		qDebug(tr(QString("Can't find Symbol (%1)").arg(error)), "plugin manager");
		dlclose(mo);
		return;
	}
	(*demo)(ScApp, ScApp);
	// FIXME: how is the menu organized? (*demo)(ScApp->scrActions[pluginMap[pluginID].actName], ScApp);
	if (pda.type != 4 && pda.type != 5)
		dlclose(mo);
	if (ScApp->HaveDoc)
		ScApp->view->DrawNew();
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
	const char *error;
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
	QString plugName = PLUGINDIR;
	plugName += name;

	mo = dlopen(plugName, RTLD_LAZY | RTLD_GLOBAL);
	if (!mo)
	{
		qDebug(tr(QString("Error: %1").arg(dlerror())), "plugin manager");
		return false;
	}
	dlerror();
	demo = (sdem0)dlsym(mo, "name");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*pluginName = (*demo)();
	dlerror();
	demo1 = (sdem1)dlsym(mo, "type");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*type = (*demo1)();
	*index = mo;
	plugID = (sdemID)dlsym(mo, "ID");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*idNr = (*plugID)();
	//ScrAction based plugins
	if (*type == Persistent || *type == Standard || *type == Import)
	{
		demo = (sdem0)dlsym(mo, "actionName");
		if ((error = dlerror()) != NULL)
		{
			dlclose(mo);
			return false;
		}
		*actName = (*demo)();
		dlerror();
		demo = (sdem0)dlsym(mo, "actionKeySequence");
		if ((error = dlerror()) != NULL)
		{
			dlclose(mo);
			return false;
		}
		*actKeySequence = (*demo)();
		dlerror();
		demo = (sdem0)dlsym(mo, "actionMenu");
		if ((error = dlerror()) != NULL)
		{
			dlclose(mo);
			return false;
		}
		*actMenu = (*demo)();
		dlerror();
		demo = (sdem0)dlsym(mo, "actionMenuAfterName");
		if ((error = dlerror()) != NULL)
		{
			dlclose(mo);
			return false;
		}
		*actMenuAfterName = (*demo)();
		dlerror();
		demo3 = (sdem3)dlsym(mo, "actionEnabledOnStartup");
		if ((error = dlerror()) != NULL)
		{
			dlclose(mo);
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
		dlclose(mo);
	else
	{
		if (loadPlugin)
		{
			dlerror();
			demo2 = (sdem2)dlsym(mo, "initPlug");
			if ((error = dlerror()) != NULL)
			{
				dlclose(mo);
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
	const char *error;
	struct PluginData pda;
	typedef void (*sdem2)();
	sdem2 demo2;
	PluginData plug = pluginMap[pluginID];
	if (plug.type == Persistent || plug.type == Type5)
	{
		dlerror();
		demo2 = (sdem2)dlsym(plug.index, "cleanUpPlug");
		if ((error = dlerror()) != NULL)
		{
			dlclose(plug.index);
		}
		else
			(*demo2)();
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
