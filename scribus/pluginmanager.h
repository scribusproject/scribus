#ifndef _PLUGIN_MANAGER_
#define _PLUGIN_MANAGER_

#include <qobject.h>
#include <qstring.h>
#include <qmap.h>
#include "scribus.h"

/**
 * @brief PluginData is structure for plugin related informations.
 *
 */
// TODO: rename items
struct PluginData
{
	QString Datei;
	QString Name;
	void *Zeiger;
	int Typ;
	int MenuID;
	QString actName;
	QString actKeySequence;
	QString actMenu;
	QString actMenuAfterName;
	bool actEnabledOnStartup;
};

/**
 * @brief PluginManager handles plugin loading, unloading, and running.
 *
 * It contains methods and attributes for plugin running and its settings
 * and returning values.
 *
 */
class PluginManager : public QObject
{
	Q_OBJECT

public:
	PluginManager();
	~PluginManager();
	void initPlugs();
	void callDLL(int pluginID);
	bool DLLexists(int pluginID);
	void callDLLbyMenu(int pluginID);
	bool DLLname(QString name, QString *PName, int *typ, void **Zeig, int *idNr, QString *actName, QString *actKeySequence, QString *actMenu, QString *actMenuAfterName, bool *actEnabledOnStartup);
	void finalizePlugs();

	QString dllInput;
	QString dllReturn;

	QMap<int, PluginData> pluginMap;

public slots:
	void callDLLBySlot(int pluginID);

//private:
	//ScribusApp *ScApp;
};

#endif
