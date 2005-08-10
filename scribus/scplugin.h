#ifndef _SCPLUGIN_H
#define _SCPLUGIN_H

#include "qobject.h"
#include "qstring.h"
#include "qpixmap.h"
#include "qdatetime.h"
#include "scribusapi.h"

class QWidget;
class ScribusApp;
class DeferredTask;

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

		// Return an instance of the plugin info class.
		virtual ScPlugin* instance();

		/** \brief Human readable enumertion of the plugin types */
		// FIXME: duplicated in pluginmanager
		enum PluginType {
			Persistent = 4,
			Import = 7,
			Standard = 6, // WTF is this?
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

		// Plug-in type, inited by ctor.
		const PluginType pluginType;

		// Plug-in ID, inited by ctor. This must be unique across all plug-ins.
		const int m_id;

		// Plug-in's short name, for internal use. This MUST be a string that
		// could be used as part of a valid C identifier. Stick to [a-z], _,
		// and numbers please, with the first char being a letter. This must be
		// unique across all plug-ins.  Set up by ctor.
		const char* m_name;

		// Plug-in's human-readable, translated name. Please don't use this for
		// anything except display to the user. Set up by ctor.
		const QString m_fullTrName;

		// Information for prefs dialog, to be set up by ctor.
		const bool hasPrefsUI;
		const QPixmap prefsPanelIcon;
		const QString prefsPanelName();

		// Methods to create and destroy the UI pane for the plugin
		virtual QWidget* newPrefsPanelWidget() = 0;
		virtual void destroyPrefsPanelWidget(QWidget* prefsPanelWidget) = 0;

		// Returns a structure containing descriptive information about the
		// plug-in. This information is used in places like the Help->About
		// Plug-ins menu item. The stucture MUST be deleted using
		// deleteAboutData(AboutData* about) when finished with.
		//
		// Note: About data isn't stored as const members because (a) it can be
		// big, and (b) it isn't needed much.
		virtual AboutData* getAboutData();
		virtual void deleteAboutData(AboutData* about);

	protected:
		/**
		 * @brief ctor, returns a new ScPlugin instance
		 *
		 * @param int id          Unique plugin ID, usually a static
		 *                        const int defined in the plugin.
		 * @param QString name    Name of the plugin as shown in the
		 *                        Start-up dialog and plugin mgr. This is
		 *                        the "friendly" human-readable name.
		 *                        PluginManager should already know the
		 *                        internal name.
		 */
		ScPlugin(int id, QString& name);

		/** @brief Pure virtual destructor - this is an abstract class */
		virtual ~ScPlugin() = 0;

		// Singleton instance.
		static ScPlugin* m_instance;
};


class SCRIBUS_API ScImportExportPlugin : public ScPlugin
{
	Q_OBJECT

	/*
	 * @brief A plug-in that's loaded for data import/export duties
	 *
	 * ScImportExportPlugin describes a plug-in that is loaded on demand
	 * to perform a data import/export task such as importing an SVG
	 * image or exporting a page to EPS format. It'll generally by unloaded
	 * after being queried, then loaded on demand when it needs to run.
	 */

	public:

		/**
		 * @brief Run the plug-in's main action.
		 *
		 * Run the plug-in's default action. That usually means prompting the
		 * user for a target (file to save as / file to import) then performing
		 * the import/export action the plug-in is written for. It should
		 * generally call run(..., QIODevice* target) or run(..., QString
		 * target) to do the work.
		 *
		 * The plug-in must clean up neatly after this method completes as it
		 * may be immediately unloaded. It should not assume it will be
		 * unloaded however
		 * - this method may be called multiple times without unloading the
		 *   plugin.  Some platforms may also not permit plugins to be
		 *   unloaded.
		 *
		 * The optional target argument is a plugin-defined string indicating
		 * what the plugin should operate on. It will typically be a filename
		 * to import from / export to. Nothing stops a plugin doing funky
		 * things like accepting a WebDAV URI, etc, however.
		 *
		 * Plug-in authors might want to consider implementing this using the
		 * DeferredTask subclass used in runAsync, if the plug-in supports it.
		 *
		 * @param QWidget& parent Parent widget to this plugin. Usually the
		 * main window.  @param ScribusApp& mainWindow The main application
		 * window.  @param QString target Optional target to operate on (see
		 * above) @returns bool True for success.
		 *
		 */
		virtual bool run(QWidget& parent, ScribusApp& mainWindow, QString target = QString::null);

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
		virtual bool run(QWidget& parent, ScribusApp& mainWindow, QIODevice* target);

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
		virtual DeferredTask* runAsync(QWidget& parent, ScribusApp& mainWindow, QString target = QString::null);

		/**
		 * @brief Run the plugin asynchronously
		 *
		 * This method is much the same as the above, but takes a QIODevice* as
		 * the target. Please see the documentation for run(..., QIODevice*
		 * target) and runAsync(..., QString target) for details.
		 */
		virtual DeferredTask* runAsync(QWidget& parent, ScribusApp& mainWindow, QIODevice* target);

		// const members defining information about actions, to be
		// populated by the ctor.
		const QString m_actionName;
		const QString m_actionKeySequence;
		const QString m_actionMenu;
		const QString m_actionMenuAfterName;
		const bool m_actionEnabledOnStartup;

	protected:
		// Protected ctor for singleton
		ScImportExportPlugin(int id, QString& name);
		// Pure virtual dtor - abstract class
		virtual ~ScImportExportPlugin() = 0;

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
		 * @param QWidget& parent Parent widget to this plugin. Usually the
		 * main window.  @parem ScribusApp& mainWindow The main application
		 * window.  @returns bool True for success.
		 */
		virtual bool initPlug(QWidget& parent, ScribusApp& mainWindow) = 0;

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
		virtual bool cleanupPlug() = 0;

	protected:

		ScPersistentPlugin(int id, QString& name);
		// Pure virtual dtor for abstract class
		virtual ~ScPersistentPlugin() = 0;

};

#endif
