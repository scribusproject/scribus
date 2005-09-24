#ifndef _PLUGIN_MANAGER_
#define _PLUGIN_MANAGER_

#include <qobject.h>
#include <qstring.h>
#include <qmap.h>
#include <qvaluelist.h>

#include "scribusapi.h"

/**
 * \brief PluginManager handles plugin loading, unloading, and running.
 *
 * It contains methods and attributes for plugin running and its settings
 * and returning values.
 *
 * Derived from Franz's ScribusApp stuff (petr vanek)
 *
 */

class ScPlugin;
class ScActionPlugin;
class ScPersistentPlugin;
class PrefsContext;

class SCRIBUS_API PluginManager : public QObject
{

	Q_OBJECT

public:

	PluginManager();
	~PluginManager();

	// Static methods for loading, unloading plugins and resolving symbols
	// These methods are platform independent, but each platform uses a different
	// implementation.
	static void* loadDLL( QString plugin );
	static void* resolveSym( void* plugin, const char* sym );
	static void  unloadDLL( void* plugin );

	/*! \brief Ininitalization of all plugins. It's called at scribus start.
	 * 
	 * This method loadDLL(...)'s each plug-in, creates a Plugin instance for
	 * them, stores a PluginData for the plugin, sets up the plug-in's
	 * actions, and connects them to any required signals.
	 * It doesn't ask plug-ins to do any time-consuming setup.
	 */
	void initPlugs();

	// Return a list of import filters, in the form used by a QFileDialog
	// or similar, for all supported plug-in imported formats.
	const QString getImportFilterString() const;

	/*! \brief Checks if is the plugin is in the plugin map, is loaded, and is enabled.
	 *
	 * \param pluginName name of plugin to get
	 * \param includeDisabled return true if a plugin is loaded but not enabled
	 * \return bool
	 */
	bool DLLexists(QCString pluginName, bool includeDisabled = false) const;

	/*! \brief Returns a pointer to the requested plugin, or 0 if not found.
	 *
	 * If includeDisabled is true, plugins that are loaded but not enabled
	 * are returned. If it is false, 0 is returned if the requested plugin
	 * is loaded but not enabled.
	 *
	 * If your code only cares about plugins of one specific type,
	 * you are probably better off using getPluginT<PluginType> .
	 *
	 * \sa getPluginT
	 */
	ScPlugin* getPlugin(const QCString & pluginName, bool includeDisabled) const;

	/*! \brief Shutdowns all plugins. Called at scribus quit */
	void cleanupPlugins();

	/** \brief Saves pluginManager preferences */
	void savePreferences();

	/*! \brief Return file extension used for shared libs on this platform */
	static QCString platformDllExtension();

	/*! \brief Call the named plugin with "arg" and return true for success.
	 *
	 * Note that failure might be caused by the plug-in being unknown,
	 * the plug-in not being loaded, the plug-in not being enabled, or
	 * by by failure of the call its self.
	 *
	 * This is a bit of a compatability kludge.
	 */
	bool callImportExportPlugin(const QCString pluginName, const QString & arg);

	/*! \brief  Call the named plugin with "arg" and return true for success, storing the return string in retval
	 *
	 * This is a lot of a compatibility kludge. Avoid using it in new code, you should probably
	 * prefer to dynamic_cast<> to the plugin class and call a plugin specific method.
	 */
	bool callImportExportPlugin(const QCString pluginName, const QString & arg, QString & retval);

	/// Return a pointer to this instance.
	//
	// Note: for now, returns a reference to (*ScApp->pluginManager); should
	// probably be turned into a singleton later.
	static PluginManager & instance();

	// Return the path to the file for the named plugin. An invalid
	// plugin name is an error.
	const QString & getPluginPath(const QCString pluginName) const;

	// Whether the given plug-in will be enabled on start-up.
	// Usable as an lvalue. An invalid plugin name is an error.
	bool & enableOnStartup(const QCString pluginName);
	
	// Whether the given plugin is currently enabled
	bool enabled(const QCString pluginName);

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
	 * If your code only cares about plugins of one specific type,
	 * you are probably better off using getPluginNamesT<PluginType> .
	 *
	 * \sa getPluginNamesT
	 * \sa getPlugin
	 */
	QValueList<QCString> pluginNames(bool includeDisabled = false,
									 const char* inherits = 0) const;

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
		QCString pluginName;
		void* pluginDLL;
		ScPlugin* plugin;
		bool enableOnStartup;
		bool enabled;
	};

	/*! \brief Reads available info and fills PluginData structure */
	bool loadPlugin(PluginData & pluginData);

	// Determines the plugin name from the file name and returns it.
	static QCString getPluginName(QString fileName);

	// Called by loadPlugin to hook the loaded plugin into the GUI,
	// call its setup routine, etc. Not responsible for creating
	// the ScPlugin instance or loading the plugin.
	void enablePlugin(PluginData &);

	// Called by finalizePlug when shutting down a plug-in. Unhooks
	// the plug-in from the GUI, calls its cleanup routine, etc.
	// DOES NOT destroy the ScPlugin instance or unload the plugin.
	void disablePlugin(PluginData & pda);

	// Called by enablePlugin to hook the loaded plugin into the GUI.
	bool setupPluginActions(ScActionPlugin*);

	/*! \brief Runs plugin's languageChange() method, and returns main menu item text if one exists */	
	QString callDLLForNewLanguage(const PluginData & pluginData);

	/*! \brief Shuts down one plug-in. The DLL may not be unloaded, but
	 *         it is cleaned up and its ScPlugin instance is destroyed.
	 *         The plug-in is marked unloaded in the map.
	 *  \param pluginID key from the pluginMap. Plugin identifier
	 */
	void finalizePlug(PluginData & pda);

	/** \brief Configuration structure */
	PrefsContext* prefs;

	// Mapping of plugin names to plugin info structures.
	typedef QMap<QCString,PluginData> PluginMap;

	/*! \brief Plugin mapping.
	 * Each plugin has its record key() -> PluginData
	 */
	PluginMap pluginMap;
};

#endif
