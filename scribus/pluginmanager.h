/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _PLUGIN_MANAGER_
#define _PLUGIN_MANAGER_

#include <QObject>
#include <QString>
#include <QMap>

class QEvent;

#include "scribusapi.h"

class ScribusDoc;
class ScribusMainWindow;
class ScPlugin;
class ScActionPlugin;
class ScPersistentPlugin;
class PrefsContext;

/**
 * \brief PluginManager handles plugin loading, unloading, and running.
		   *
		   * It contains methods and attributes for plugin running and its settings
		   * and returning values.
		   *
		   * Derived from Franz's ScribusMainWindow stuff (petr vanek)
		   *
 */
class SCRIBUS_API PluginManager : public QObject
{

	Q_OBJECT

public:

	PluginManager();
	~PluginManager();

	/*! \brief Static methods for loading, unloading plugins and resolving symbols
	 These methods are platform independent, but each platform uses a different
	 implementation. */
	static void* loadDLL( QString plugin );
	static void* resolveSym( void* plugin, const char* sym );
	static void  unloadDLL( void* plugin );

	/*! \brief Initalization of all plugins. It's called at scribus start.
	 *
	 * Walk through all plugins, try to init them and try to re-load failed
	 * ones then. See initPlugin for more info.
	 * Reload uses a "brute force" method - Repeat until there is something
	 * to load again. It's not elegant I know. But there are no additional
	 * dependancy relations addons (XML config, plugin classes change etc.).
	 */
	void initPlugs();

	/*! \brief Called at after initPlugs to hook the loaded plugin into the GUI.
	 * Run in main window startup
	 */
	bool setupPluginActions(ScribusMainWindow*);


	/*! \brief Checks if is the plugin is in the plugin map, is loaded, and is enabled.
	 *
	 * \param pluginName name of plugin to get
	 * \param includeDisabled return true if a plugin is loaded but not enabled
	 * \return bool
	 */
	bool DLLexists(QString pluginName, bool includeDisabled = false) const;

	/*! \brief Returns a pointer to the requested plugin, or 0 if not found.
	 *
	 * If includeDisabled is true, plugins that are loaded but not enabled
	 * are returned. If it is false, 0 is returned if the requested plugin
	 * is loaded but not enabled.
	 */
	ScPlugin* getPlugin(const QString & pluginName, bool includeDisabled) const;

	/*! \brief Shutdowns all plugins. Called at scribus quit */
	void cleanupPlugins();

	/** \brief Saves pluginManager preferences */
	void savePreferences();

	/*! \brief Return file extension used for shared libs on this platform */
	static QString platformDllExtension();

	/*! \brief Return file extension used for shared libs on this platform */
	static int platformDllSearchFlags();

	/*! \brief Return a pointer to this instance.
	 \note for now, returns a reference to (*ScMW->pluginManager); should
	 probably be turned into a singleton later. */
	static PluginManager & instance();

	/*! \brief Return the path to the file for the named plugin.
	An invalid plugin name is an error.*/
	const QString getPluginPath(const QString & pluginName) const;

	/*! \brief Whether the given plug-in will be enabled on start-up.
	Usable as an lvalue. An invalid plugin name is an error. */
	bool & enableOnStartup(const QString & pluginName);

	/*! \brief Whether the given plugin is currently enabled */
	bool enabled(const QString & pluginName);

	/** \brief Return a list of plugin names currently known.
	 *
	 * If includeNotLoaded is true, names are returned for plug-ins that are
	 * not loaded (ie we have no ScPlugin instance for them).
	 *
	 * \param includeDisabled  Should the names of plugins that are loaded, but
	 *                         disabled, be returned?
	 * \param inherits         Only return plugins that inherit from this parent
	 *                         (the text name of the type - uses QMetaObject).
	 * \return A list of internal plugin names suitable for use with getPlugin etc.
	 *
	 * \sa getPluginNamesT
	 * \sa getPlugin
	 */
	QStringList pluginNames(bool includeDisabled = false,
									 const char* inherits = 0) const;
	
	virtual void changeEvent(QEvent *e);

public slots:

	void languageChange();

protected:

	/**
	 * \brief PluginData is structure for plugin related informations.
	 * \param pluginFile path to the share library (with name).
	 * \param pluginName internal name of plug-in, used for prefix to dlsym() names
	 * \param pluginDLL reference to plug-in data for dynamic loading
	 * \param plugin is the pointer to the plugin instance
	 * \param enableOnStartup enable or disable plugin at app start-up
	 * \param enabled has the plug-in been set up and activated (gui connected, setup called, etc)?
	 *
	 * Note that there are some constraints on this structure.
	 * enabled == true depends on:
	 *     plugin != 0 which depends on:
	 *         pluginDLL != 0
	 *
	 * In other words, a plugin cannot be enabled unless we have an ScPlugin
	 * instance for it. We can't have an ScPlugin instance for a plugin unless
	 * it's linked.
	 */
	struct PluginData
	{
		QString pluginFile; // Datei;
		QString pluginName;
		void* pluginDLL;
		ScPlugin* plugin;
		bool enableOnStartup;
		bool enabled;
	};

	/*! \brief Init one plugin.
	 * This method loadDLL(...)'s each plug-in, creates a Plugin instance for
	 * them, stores a PluginData for the plugin, sets up the plug-in's
	 * actions, and connects them to any required signals.
	 * It doesn't ask plug-ins to do any time-consuming setup.
	 * It's called by initPlugs.
	\param fileName a filename of the plugin without path
	\retval int 0 init failed, 1 loaded.
	 */
	int initPlugin(const QString fileName);

	/*! \brief Reads available info and fills PluginData structure */
	bool loadPlugin(PluginData & pluginData);

	/*! \brief Determines the plugin name from the file name and returns it. */
	static QString getPluginName(QString fileName);

	/*! \brief Called by loadPlugin to hook the loaded plugin into the GUI,
	call its setup routine, etc. Not responsible for creating
	the ScPlugin instance or loading the plugin. */
	void enablePlugin(PluginData &);

	/*! \brief Called by finalizePlug when shutting down a plug-in. Unhooks
	 the plug-in from the GUI, calls its cleanup routine, etc.
	 DOES NOT destroy the ScPlugin instance or unload the plugin. */
	void disablePlugin(PluginData & pda);

	/*! \brief Runs plugin's languageChange() method, and returns main menu item text if one exists */
	QString callDLLForNewLanguage(const PluginData & pluginData);

	/*! \brief Shuts down one plug-in. The DLL may not be unloaded, but
	 *         it is cleaned up and its ScPlugin instance is destroyed.
	 *         The plug-in is marked unloaded in the map.
	 *  \param pda plugin data
	 */
	void finalizePlug(PluginData & pda);

	/** \brief Configuration structure */
	PrefsContext* prefs;

	/*! \brief Mapping of plugin names to plugin info structures. */
	typedef QMap<QString,PluginData> PluginMap;

	/*! \brief Plugin mapping.
	 * Each plugin has its record key() -> PluginData
	 */
	PluginMap pluginMap;
};

#endif
