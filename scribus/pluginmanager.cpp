#include "pluginmanager.h"
//include "pluginmanager.moc"
#include <dlfcn.h>
#include <iostream>
#include <qdir.h>
#include "scribus.h"
#include "menumanager.h"
#include "scraction.h"
#include "splash.h"
#include "mpalette.h"
#include "tree.h"

extern ScribusApp *ScApp;


PluginManager::PluginManager()
{
	dllInput = "";
	dllReturn = "";
}

PluginManager::~PluginManager()
{
}

void PluginManager::initPlugs()
{
	QString nam = "";
	int id = 0;
	int ty = 0;
	int menid = 0;
	struct PluginData pda;
#if defined(__hpux)
	QDir d(PLUGINDIR, "*.sl*", QDir::Name, QDir::Files | QDir::Executable | QDir::NoSymLinks);
#else
	QDir d(PLUGINDIR, "*.so*", QDir::Name, QDir::Files | QDir::Executable | QDir::NoSymLinks);
#endif
	if ((d.exists()) && (d.count() != 0))
	{
		ScApp->scrMenuMgr->addMenuSeparator("Extras");
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			pda.Zeiger = 0;
			pda.Datei = "";
			pda.Name = "";
			pda.Typ = 0;
			pda.MenuID = 0;
			if (DLLname(d[dc], &nam, &ty, &pda.Zeiger, &id, &pda.actName, &pda.actKeySequence, &pda.actMenu, &pda.actMenuAfterName, &pda.actEnabledOnStartup))
			{
				if (ty == 1)
				{
					qDebug(QString("Type %1 plugins not supported anymore").arg(ty));
					break;
				}

				pda.Name = nam;
				pda.Datei = d[dc];
				pda.Typ = ty;
				if (ty < 5 && ty!=4)
					pda.MenuID = menid;
				if (ty==4 || ty==6 || ty==7)
				{
					//Add in ScrAction based plugin linkage
					//Insert DLL Action into Dictionary with values from plugin interface
					ScApp->scrActions.insert(pda.actName, new ScrAction(ScrAction::DLL, QIconSet(), pda.Name, QKeySequence(pda.actKeySequence), ScApp, pda.actName, id));
					
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
				pluginMap.insert(id, pda);
				if (ScApp->splashScreen != NULL)
					ScApp->splashScreen->setStatus( tr("Loading:")+" "+nam);
			}
		}
	}
}

void PluginManager::callDLLBySlot(int pluginID)
{
	//Run old type 2 Import pre call code
	if (pluginMap[pluginID].Typ==7)
	{
		if (ScApp->HaveDoc)
			ScApp->doc->OpenNodes = ScApp->outlinePalette->buildReopenVals();
	}

	callDLL(pluginID);

	//Run old type 2 Import post call code
	if (pluginMap[pluginID].Typ==7)
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
	if (pda.Typ != 4 && pda.Typ !=5)
	{
		plugDir += pda.Datei;
		mo = dlopen(plugDir, RTLD_LAZY | RTLD_GLOBAL);
		if (!mo)
		{
			std::cout << "Can't find Plug-in" << endl;
			return;
		}
	}
	else
		mo = pda.Zeiger;
	dlerror();
	demo = (sdem)dlsym(mo, "Run");
	if ((error = dlerror()) != NULL)
	{
		std::cout << "Can't find Symbol" << endl;
		dlclose(mo);
		return;
	}
	(*demo)(ScApp, ScApp);
	// FIXME: how is the menu organized? (*demo)(ScApp->scrActions[pluginMap[pluginID].actName], ScApp);
	if (pda.Typ != 4 && pda.Typ != 5)
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
		if (it.data().MenuID == pluginID)
		{
			callDLL(it.key());
			break;
		}
	}
}

bool PluginManager::DLLname(QString name, QString *PName, int *typ, void **Zeig, int *idNr, QString *actName, QString *actKeySequence, QString *actMenu, QString *actMenuAfterName, bool *actEnabledOnStartup)
{
	void *mo;
	const char *error;
	typedef QString (*sdem0)();
	typedef int (*sdem1)();
	typedef void (*sdem2)(QWidget *d, ScribusApp *plug);
	typedef bool (*sdem3)();
	sdem0 demo;
	sdem1 demo1;
	sdem2 demo2;
	sdem3 demo3;
	QString plugName = PLUGINDIR;
	plugName += name;
	mo = dlopen(plugName, RTLD_LAZY | RTLD_GLOBAL);
	if (!mo)
	{
		std::cout << dlerror() << endl;
		return false;
	}
	dlerror();
	demo = (sdem0)dlsym(mo, "Name");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*PName = (*demo)();
	dlerror();
	demo1 = (sdem1)dlsym(mo, "Type");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*typ = (*demo1)();
	*Zeig = mo;
	demo1 = (sdem1)dlsym(mo, "ID");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*idNr = (*demo1)();
	//ScrAction based plugins
	if (*typ==4 || *typ==6 || *typ==7)
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
	if (*typ != 4 && *typ!=5)
		dlclose(mo);
	else
	{
		dlerror();
		demo2 = (sdem2)dlsym(mo, "InitPlug");
		if ((error = dlerror()) != NULL)
		{
			dlclose(mo);
			return false;
		}
		(*demo2)(ScApp, ScApp);
		// FIXME: how is the menu organized? (*demo2)(ScApp->scrActions[pluginMap[idNr]], ScApp);
	}

	return true;
}

void PluginManager::finalizePlugs()
{
	const char *error;
	struct PluginData pda;
	typedef void (*sdem2)();
	sdem2 demo2;
	for (QMap<int, PluginData>::Iterator it = pluginMap.begin(); it != pluginMap.end(); ++it)
	{
		if (it.data().Typ == 4 || it.data().Typ == 5)
		{
			dlerror();
			demo2 = (sdem2)dlsym(it.data().Zeiger, "CleanUpPlug");
			if ((error = dlerror()) != NULL)
			{
				dlclose(it.data().Zeiger);
				continue;
			}
			else
			{
				(*demo2)();
			}
		}
	}
}
