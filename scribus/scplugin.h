#ifndef _SCPLUGIN_H
#define _SCPLUGIN_H

#include "qobject.h"
#include "qstring.h"
#include "qpixmap.h"
#include "qdatetime.h"
#include "qiconset.h"
#include "scribusapi.h"

class QWidget;
class ScribusApp;
class DeferredTask;
class PrefsPanel;

/*
 * The idea here is that a ScPlugin is an information class stored by the
 * plug-in and returned to callers. It is a singleton that is always owned
 * by the plugin implementation.
 *
 * Note that the implementation below really creates two quite different
 * kinds of plugins. One for import/export of data which is generally
 * loaded, run, and unloaded. A second for persistent plugins that are loaded
 * (generally when the app starts) and remain resident.
 *
 * More plugin classes - such as plugins for text import and document import
 * - may be added in future.
 *
 * In addition to the API described here, a plugin is expected to implement
 * the following "extern C" functions, where 'pluginname' is the base name
 * of the plug-in:
 *
 * int pluginname_getPluginAPIVersion();
 *    Return an integer indicating the plug-in API version implemented.
 *    If the version does not exactly match the running plugin API version,
 *    the plugin will not be initialised.
 *
 * ScPlugin* pluginname_getPlugin();
 *    Create and return an instance of the plug-ins ScPlugin subclass.
 *    Nothing should be done here that's overly slow or uses much memory,
 *    and the plugin should not initialize its self here beyond what is
 *    required to return a ScPlugin instance.
 *
 * void pluginname_freePlugin(ScPlugin* plugin);
 *    Free the scplugin instance returned earlier.
 *
 * TODO: define "standard" plug-ins better
 * TODO: better discovery of what plug-ins can import / export what formats, file exts, names, etc.
 *
 */

class SCRIBUS_API ScPlugin : public QObject
{
	Q_OBJECT

	/**
	 * @brief ScPlugin provides an interface to ask plugins for information
	 * about themselves.
	 *
	 * Plugins return a pointer to their ScPlugin instance when
	 * asked for it by the plugin manager. This class provides general
	 * information about the plugin. It should not generally be inherited
	 * directly by plugins - use one of the provided base classes instead.
	 */
	public:

		/** \brief Human readable enumertion of the plugin types
		 *
		 * This might get replaced with checking inheritance
		 * with QObject.
		 *
		 * PluginType_Persistent:
		 *    The plug-in is loaded and initialised on app startup. No
		 *    automatic connection to the GUI is made, and the plugin
		 *    essentially becomes part of the application, maintaining
		 *    its state as long as it's loaded.
		 * 
		 * PluginType_Import:
		 *    The plug-in provides the facility to import file type(s).
		 *    It keeps no state.
		 *
		 * PluginType_Export:
		 *    The plugin provides a facility to export file type(s).
		 *    It keeps no state.
		 *
		 * PluginType_Action:
		 *    The plugin has a single specific action it can take that
		 *    is not import or export. It's automatically hooked into
		 *    the GUI and keeps no state.
		 */
		enum PluginType {
			PluginType_Persistent = 4,
			PluginType_Import = 7,
			PluginType_Export = 8,
			PluginType_Action = 9
		};

		// A struct providing the information returned by getAboutData(), for
		// the "About Plug-ins" menu item.
		struct AboutData
		{
			// Author name(s) and email address(es)
			QString authors;
			// One-liner description of the function of the plugin
			QString shortDescription;
			// Longer description of the plugin
			QString description;
			// Text plug-in version string
			QString version;
			// Date of the release of this version
			QDateTime releaseDate;
			// Copyright string
			QString copyright;
			// License text / name / URL / whatever. Please don't include
			// the GPL here, just reference it. Remember that Scribus is
			// licensed under the GPL, so plug-in authors must take that
			// into account.
			QString license;
		};

		/**
		 * @brief ctor, returns a new ScPlugin instance
		 *
		 * @param pluginType      plugin type enum, used by plugin
		 *                        manager to identify plugin.
		 *
		 * Only the actual plugin implmementation should call this, from
		 * its setup function.
		 */
		ScPlugin(PluginType pluginType);

		/** @brief Pure virtual destructor - this is an abstract class */
		virtual ~ScPlugin() = 0;

		// Plug-in type, inited by ctor.
		const PluginType pluginType;

		// Plug-in's human-readable, translated name. Please don't use this for
		// anything except display to the user.
		virtual const QString fullTrName() const = 0;

		// Methods to create and destroy the UI pane for the plugin
		// A plugin MUST reimplment destroyPrefsPanelWidget if it
		// reimplements newPrefsPanelWidget .
		// parent should be set to the dialog it's being added to.
		//
		// The plugin prefs dialog must listen to one signal from
		// the parent widget, accepted(). If accepted() is recieved,
		// the panel widget must save any changed settings. Settings
		// must never be saved at any other time.
		//
		// By default, returns 0 to indicate no prefs UI.
		virtual PrefsPanel* newPrefsPanelWidget(QWidget* parent);
		virtual void destroyPrefsPanelWidget(PrefsPanel* prefsPanelWidget);

		// Return icon and caption to use for preferences panel in the prefs
		// dialog. Unless overridden, returns QString::null and an empty pixmap.
		virtual const QPixmap prefsPanelIcon() const;
		virtual const QString prefsPanelName() const;

		// Returns a structure containing descriptive information about the
		// plug-in. This information is used in places like the Help->About
		// Plug-ins menu item. The stucture MUST be deleted using
		// deleteAboutData(AboutData* about) when finished with.
		//
		// Every plug-in must re-implment these.
		virtual const AboutData* getAboutData() const = 0;
		virtual void deleteAboutData(const AboutData* about) const = 0;

		// Return human readable, translated text of last error to be
		// encountered. DO NOT ATTEMPT TO USE THIS VALUE TO MAKE
		// DECISIONS IN PROGRAM CODE.
		const QString & lastError() const;

		// Updates the text on all widgets on the plug-in to reflect the new
		// language. You should generally use this method to set all the widget
		// text during construction.
		virtual void languageChange() = 0;

		/// Returns human readable plugin type from plug-in's pluginType
		const QString pluginTypeName() const;

	protected:
		// Human readable, translated version of last error to occur.
		QString m_lastError;
};


class SCRIBUS_API ScActionPlugin : public ScPlugin
{
	Q_OBJECT

	/*
	 * @brief A plug-in that's loaded for data import/export duties
	 *
	 * ScActionPlugin describes a plug-in that is loaded on demand
	 * to perform a data import/export task such as importing an SVG
	 * image or exporting a page to EPS format. It'll generally by unloaded
	 * after being queried, then loaded on demand when it needs to run.
	 */

	public:

		/** @brief ctor
		 *
		 * @sa ScPlugin::ScPlugin()
		 */
		ScActionPlugin(PluginType pluginType);

		// Pure virtual dtor - abstract class
		virtual ~ScActionPlugin() = 0;

		// Information about actions, to be returned by actionInfo()
		struct ActionInfo {
			QString name;  // name of action
			QString text;  // text to display to user
			QString keySequence;
			QString menu;
			QString menuAfterName;
			QIconSet iconSet;
			bool enabledOnStartup;
		};

		// Return an ActionInfo instance to the caller
		const ActionInfo & actionInfo() const;

	public slots:
		/**
		 * @brief Run the plug-in's main action.
		 *
		 * Run the plug-in's default action. That usually means prompting the
		 * user for a target (file to save as / file to import) then
		 * performing the import/export action the plug-in is written for.
		 *
		 * It should generally call run(..., QIODevice* target) or run(...,
		 * QString target) to do the work. This is done by default, with the
		 * assumption that the target is a file. An open QFile with
		 * IO_ReadOnly or IO_WriteOnly mode, depending on plugin type,
		 * is supplied. Override this if you need more flexibility.
		 *
		 * The optional target argument is a plugin-defined string indicating
		 * what the plugin should operate on. It will typically be a filename
		 * to import from / export to. Nothing stops a plugin doing funky
		 * things like accepting a WebDAV URI, etc, however.
		 *
		 * Plug-in authors might want to consider implementing this using the
		 * DeferredTask subclass used in runAsync, if the plug-in supports it.
		 *
		 * @param QString target Optional target to operate on (see
		 *                above)
		 * @returns bool True for success.
		 *
		 */
		virtual bool run(QString target = QString::null);

		/**
		 * @brief Run the plugin on a QIODevice
		 *
		 * This method is essentially the same as the above, except that it
		 * accepts a QIODevice* to work with. This will generally be used for
		 * file I/O without the plugin having to know or care where from.
		 *
		 * A plug-in MUST return false if it cannot support this method or upon
		 * failure. A plug-in MUST supply a working version of this method if
		 * it accepts runWith(..., QString) on a local file, and should
		 * generally implement the former using this method.
		 *
		 * Remember that not all QIODevice*s are seekable, and try to write
		 * your code to handle a non-seekable stream.
		 */
		virtual bool run(QIODevice* target);

		/**
		 * @brief Run the plugin asynchronously
		 *
		 * This call is similar to the one above, but instead of doing all the
		 * work in one big batch, it creates an instance of a DeferredTask
		 * subclass to keep track of the job, does some setup, and returns
		 * immediately. The DeferredTask, a pointer to which is returned, has
		 * already been started when this method returns.
		 *
		 * The caller of this method is responsible for handling the aborted()
		 * and finished() signals from the DeferredTask, and (if necessary) for
		 * cleaning it up once it has finished. Remember that a DeferredTask is
		 * automatically deleted when its parent is, if it hasn't already been
		 * deleted.
		 *
		 * FIXME: The above is probably utterly broken on win32.
		 *
		 * See the DeferredTask documentation for details on how it works.
		 *
		 * If your plugin implementation does not support asynchronous
		 * operation, you must return null. This is the default implementation,
		 * so simply ignore this method unless you wish to support it. It is
		 * recommended that all new plugins support this interface where
		 * possible, even if they can only do part of their work progressively.
		 *
		 * If you implement this, you may want to implement run(...) using the
		 * same deferred operation and a modal dialog. That'll permit the event
		 * loop to continue running for redraws, permit you to update the
		 * progress bar, etc.
		 *
		 * NOTE: In current Scribus code (August 2005), callers of this method
		 * will generally create a modal dialog - possibly with progress meter
		 * - then attach signals from the DeferredTask, and exec() the dialog.
		 *   A caller is not required to do this, but with the current (August
		 *   2005) core code it's the only way to operate safely. This is
		 *   likely to change in future and you should avoid relying on it if
		 *   at all posssible.
		 *
		 * If this method is used, the plugin must not be unloaded until all
		 * DeferredTask instances have been deleted.
		 */
		virtual DeferredTask* runAsync(QString target = QString::null);

		/**
		 * @brief Run the plugin asynchronously
		 *
		 * This method is much the same as the above, but takes a QIODevice* as
		 * the target. Please see the documentation for run(..., QIODevice*
		 * target) and runAsync(..., QString target) for details.
		 */
		virtual DeferredTask* runAsync(QIODevice* target);

		// Compat kludge ... we store a QString result from any successful
		// run(...) call in m_runResult and let callers retrive it here.
		// DO NOT USE THIS INTERFACE FOR NEW PLUG-INS; you should
		// dynamic_cast<> to the plugin type then call a plug-in specific
		// method instead.
		const QString & runResult() const;

	protected:
		// Action info. To be set up by ctor.
		ActionInfo m_actionInfo;
		QString m_runResult;
};

class SCRIBUS_API ScPersistentPlugin : public ScPlugin
{
	Q_OBJECT

	/**
	 * @brief A plug-in that is resident for the lifetime of the app
	 *
	 * ScPersistentPlugin describes a plugin that is to be kept resident
	 * for the lifetime of the app (or until unloaded by a request to
	 * the plug-in manager).
	 *
	 * Such plug-ins have an init method and a cleanup method; they have
	 * no "run" method as such.
	 */
	public:

		/** @brief ctor
		 *
		 * @sa ScPlugin::ScPlugin()
		 */
		ScPersistentPlugin();

		// Pure virtual dtor for abstract class
		virtual ~ScPersistentPlugin() = 0;

		/**
		 * @brief Initialize the plugin
		 *
		 * This method must initialize the plugin. It is called at plug-in load
		 * time. This method will never be called twice without an intervening
		 * cleanupPlug call (but not necessarily unloading the plugin).
		 *
		 * It will usually instantiate a class and store a pointer to the
		 * instance in a static variable of the plugin or a member of its
		 * ScPersistentPlugin subclass, letting that class do the work of setup
		 * and integration into the app.
		 *
		 * Note that this is NOT the same as the init function of the plugin. That
		 * is always simple and lightweight, intended to just get an instance of this
		 * class back to the core app. This function is where any time-consuming or
		 * memory expensive init should be done.
		 *
		 * If this method returns failure, the plugin will be treated as dead and
		 * cleanupPlug() will not be called.
		 *
		 * @returns bool True for success.
		 */
		virtual bool initPlugin() = 0;

		/**
		 * @brief Cleans up the plugin so it can be safely unloaded.
		 *
		 * This method will be called when the plug-in is about to be unloaded,
		 * or if the plug-in manager has been asked to disable the plug-in.
		 * This method will never be called unless initPlug has been called
		 * first, but there is no guarantee the plugin will be unloaded after
		 * this is called, or before initPlug is called again.
		 *
		 * @returns bool True for success.
		 */
		virtual bool cleanupPlugin() = 0;
};

#endif
