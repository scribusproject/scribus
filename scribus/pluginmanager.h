#ifndef _PLUGIN_MANAGER_
#define _PLUGIN_MANAGER_

#include <qobject.h>
#include <qstring.h>
#include <qmap.h>
#include "scribus.h"

/**
 * \brief PluginManager handles plugin loading, unloading, and running.
 *
 * It contains methods and attributes for plugin running and its settings
 * and returning values.
 *
 * Derived from Franz's ScribusApp stuff (petr vanek)
 *
 */
class PluginManager : public QObject
{
	Q_OBJECT

public:
	/** \brief Human readable enumertion of the plugin types */
	// FIXME: what the hell is type5?
	enum PluginType {
		Persistent = 4,
		Import = 7,
		Standard = 6,
		Type5 = 5
	};

	/**
	 * \brief PluginData is structure for plugin related informations.
	 * \param pluginFile path to the share library (with name).
	 * \param name a string which will be shown at menu
	 * \param index black magic? FIXME
	 * \param type PluginType element
	 * \param menuID id for menu system
	 * \param actName name of the action for this plugin
	 * \param actKeySequence menu GUI key combination
	 * \param actMenu first level menu
	 * \param actMenuAfterName 2nd level menu
	 * \param actEnabledOnStartup run it at start FIXME
	 * \param loadPlugin enable or disable plugin for user
	 * \param loaded is the plug really loaded?
	 */
	struct PluginData
	{
		QString pluginFile;// Datei;
		QString name;
		void *index; //Zeiger;
		PluginType type;
		int menuID;
		QString actName;
		QString actKeySequence;
		QString actMenu;
		QString actMenuAfterName;
		bool actEnabledOnStartup;
		bool loadPlugin;
		bool loaded;
	};

	PluginManager();
	~PluginManager();
	/*! \brief Ininitalization of all plugins. It's called at scribus start. */
	void initPlugs();
	/*! \brief Run plugin by its id from pluginMap */
	void callDLL(int pluginID);
	/*! \brief Checks if is the plug in plugin map.
	 * \return bool
	 */
	bool DLLexists(int pluginID);
	/*! unused/obsolete */
	void callDLLbyMenu(int pluginID);
	/*! \brief Reads available info and fills PluginData structure */
	bool DLLname(QString name, QString *pluginName, PluginType *type, void **index, int *idNr, QString *actName, QString *actKeySequence, QString *actMenu, QString *actMenuAfterName, bool *actEnabledOnStartup, bool loadPlugin);
	/*! \brief Shutdowns all plugins. Called at scribus quit */
	void finalizePlugs();
	/*! \brief Shutdowns one plugin.
	 * \param pluginID key from the pluginMap. Plugin identifier
	 */
	void finalizePlug(int pluginID);
	/** \brief Returns human readable plugin type */
	QString getPluginType(PluginType aType);
	/** \brief Saves plugin preferences */
	void savePreferences();

	/*! \brief Input variable to the plug. */
	QString dllInput;
	/*! \brief Returning variable from the plug. */
	QString dllReturn;

	/*! \brief Plugin mapping.
	 * Each plugin has its record key() -> PluginData */
	QMap<int, PluginData> pluginMap;

	/*! \brief Return file extension used for shared libs on this platform */
	static QCString PluginManager::platformDllExtension();

public slots:
	/*! obsolete */
	void callDLLBySlot(int pluginID);

private:
	/** \brief Configuration structure */
	PrefsContext* prefs;
};

#endif
