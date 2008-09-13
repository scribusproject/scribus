/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _SCPLUGIN_H
#define _SCPLUGIN_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QDateTime>
#include <QIcon>
#include "scribusapi.h"

class ScribusDoc;
class ScribusMainWindow;
class DeferredTask;
class PrefsPanel;

/**
 * \brief Abstract super class for all Scribus plug-ins
 *
 * ScPlugin is a class that provides information about a plug-in, and a gateway
 * into any plugin-specific functionality.  It should not generally be
 * inherited directly by actual plugin implementations - use one of the
 * provided base classes instead. See below for info on the creation of
 * new types of plugin.
 *
 * Plugins create and return a instance of a subclass of ScPlugin when asked
 * for it by the plugin manager. The plugin manager is responsible for tracking
 * these instances. The ScPlugin instance is used by the plugin manager for
 * tasks such as hooking the plugin up to the UI where appropriate. It's also
 * used by other code that talks to plugins, usually by dynamic_cast<>ing it to
 * a more specific subclass and working with that. The file loader system will
 * provide an example of this.
 *
 * A given plugin implementation's ScPlugin subclass is *not* the best place
 * to implement the bulk of the plugin's functionality. It must be lightweight
 * and maintain as little state as possible. Most functionality should be
 * implemented in a private subclass that is not exposed in the header of the
 * scplugin subclass defined by the plugin. That helps keep the plugin's
 * implementation separate from its inteface to Scribus.
 *
 * Note that the two subclases ScActionPlugin and ScPersistentPlugin, defined
 * below, create two quite different kinds of plugins. One is for import/export
 * of data and for single function actions (like providing a particular dialog
 * or function) which generally do nothing when not in active use. A second for
 * persistent plugins that are loaded (generally when the app starts) and
 * remain resident, possibly active in the background or hooked into many
 * parts of the user interface.
 *
 * More plugin classes - such as plugins for text import and document import
 * - may be added in future.
 *
 * New types of plugin can be created by subclassing ScPlugin or one of its
 * subclasses and adding the additional interfaces required. The two existing
 * subclasses provide examples of how this can work. Of course, once you have
 * the plugin interface, you still need to actually use the plugins. You'll need
 * to extend pluginmanager.cpp a little to handle any low-level init required by
 * the new plugin type (please keep this minimal!). You'll then need to extend
 * the appropriate classes to make use of the new plugin type where required.
 * The new file loader mechanism will provide a good example of this.
 *
 * In addition to the API described here, a plugin is expected to implement
 * the following "extern C" functions, where 'pluginname' is the base name
 * of the plug-in (where the filename is libpluginname.so on UNIX):
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
	 */
	public:

		/*! \brief A struct providing the information returned by getAboutData(), for
		the "About Plug-ins" menu item. */
		struct AboutData
		{
			//! Author name(s) and email address(es)
			QString authors;
			//! One-liner description of the function of the plugin
			QString shortDescription;
			//! Longer description of the plugin
			QString description;
			//! Text plug-in version string
			QString version;
			//! Date of the release of this version
			QDateTime releaseDate;
			//! Copyright string
			QString copyright;
			//! License text / name / URL / whatever. Please don't include
			//! the GPL here, just reference it. Remember that Scribus is
			//! licensed under the GPL, so plug-in authors must take that
			//! into account.
			QString license;
		};

		/**
		 * @brief ctor, returns a new ScPlugin instance
		 *
		 * Only pluginname_getPlugin(...) may call this. Calls by other code
		 * will have undefined results.
		 */
		ScPlugin();

		/** @brief Destroy the ScPlugin instance
		 *
		 * Pure virtual destructor - this is an abstract class.
		 *
		 * Only pluginname_freePlugin(...) may call this. Calls by other code
		 * will have undefined results.
		 */
		virtual ~ScPlugin() = 0;

		/** @brief Plug-in's human-readable, translated name.
		 *
		 * Please don't use this for anything except display to the user.
		 * The results of testing the value of this can not be guaranteed,
		 * as its value may change depending on locale and change at runtime.
		 */
		virtual const QString fullTrName() const = 0;

		/**
		 * \brief Create and return a prefs UI panel for the plugin.
		 *
		 * Override this method if you want to provide a panel for the
		 * preferences dialog.
		 *
		 * The caller takes ownership of the panel. Qt is responsible for
		 * deleting it when it's parent is deleted. `parent' must be the dialog
		 * the widget will be added to or a child of it, otherwise the panel
		 * won't be deleted correctly when the dialog is.
		 *
		 * See prefspanel.h for info on implementing the panel.
		 *
		 * This method must return false (the default) if the plugin does
		 * not provide a prefs panel. If true is returned, caption, panel,
		 * and icon MUST have been assigned.
		 *
		 * \param parent Parent widget, to be passed to panel ctor
		 * \param panel (out) the panel created
		 * \param caption (out) caption for icon in panel list
		 * \param icon (out) icon for use in panel list
		 * \return true if panel was created, false if not.
		 *
		 * By default, returns 0 to indicate no prefs UI.
		*/
		virtual bool newPrefsPanelWidget(QWidget* parent, PrefsPanel*& panel,
										 QString& caption, QPixmap& icon);

		/*! @brief Return descriptive information about the plug-in
		 *
		 * Returns a structure containing descriptive information about the
		 * plug-in. This information is used in places like the Help->About
		 * Plug-ins menu item. The stucture MUST be deleted using
		 * deleteAboutData(AboutData* about) when finished with.
		 *
		 * Every plugin MUST reimplement getAboutData(...) and deleteAboutData(...).
		 *
		 * @sa ScPlugin::deleteAboutData
		 */
		virtual const AboutData* getAboutData() const = 0;
		virtual void deleteAboutData(const AboutData* about) const = 0;

		/*! @brief Text summary of last error encountered
		 *
		 * Return the human readable, translated text of last error to be
		 * encountered by this plugin. DO NOT ATTEMPT TO USE THIS VALUE TO MAKE
		 * DECISIONS IN PROGRAM CODE. The value returned for a given error
		 * may change depending on locale, and may change at runtime.
		 */
		const QString & lastError() const;

		/*! @brief Update all user-visible text to reflect current UI language
		 *
		 * Updates the text on all widgets on the plug-in to reflect the new
		 * language. You should generally use this method to set all the widget
		 * text during construction. That ensures that text is not duplicated,
		 * and that when this method is called the text of all persistent
		 * widgets is correctly changed.
		 *
		 * This method only needs to affect text in widgets that currently
		 * exists (displayed or otherwise).
		 */
		virtual void languageChange() = 0;

		//! \brief Returns human readable plugin type from plug-in's pluginType
		const QString pluginTypeName() const;

		//! \brief Allow plugins to add to a main menu
		virtual void addToMainWindowMenu(ScribusMainWindow *) = 0;
	protected:
		//! \brief Human readable, translated version of last error to occur.
		QString m_lastError;
};


/*!
 * @brief A plug-in that performs a single action
 *
 * ScActionPlugin describes a plug-in that is used to perform a single well
 * defined action. Uses include data import/export, displaying a dialog, or
 * doing a single batch task. It is automatically connected to the user
 * interface and has simple methods to trigger the plug-in's supported action.
 *
 * If you need more complex user interface integration, persistent operation,
 * etc, you may be better off with ScPersistentPlugin.
 */
class SCRIBUS_API ScActionPlugin : public ScPlugin
{
	Q_OBJECT

	public:

		/** @brief ctor
		 *
		 * @sa ScPlugin::ScPlugin()
		 */
		ScActionPlugin();

		//! \brief Pure virtual dtor - abstract class
		virtual ~ScActionPlugin() = 0;

		// Information about actions, to be returned by actionInfo()
		struct ActionInfo {
			QString name;  // name of action
			QString text;  // text to display to user
			QString keySequence;
			QString menu;
			QString menuAfterName;
			QString parentMenu;
			QString subMenuName;
			QString toolbar;			// Name of the ToolBar the action is to be inserted, if that toolbar doesn't exits it will be created
			QString toolBarName;		// translateable ToolBar title
			QPixmap icon1;
			QPixmap icon2;
			QList<int> notSuitableFor;		// a list of PageItem type values which the plugin can *not* handle
			QList<int> forAppMode;			// a list of AppMode values for which the plugin will be active, an empty list indicates that the plugin is always active
			int needsNumObjects;			// plugin needs this number of selected Objects. -1 = needs no Object, num > 2 any number of Objects is allowed
			QList<int> firstObjectType;		// when needsNumObjects is 2 this list contains the Object Types of the first object on the selection
			QList<int> secondObjectType;	// when needsNumObjects is 2 this list contains the Object Types of the second object on the selection
											// -1 indicates that any kind of object is possible. Otherwise the selection must contain the 2 Object Types
											// for the Plugin Action to be enabled
			bool enabledOnStartup;
		};

		// Return an ActionInfo instance to the caller
		const ActionInfo & actionInfo() const;
		/**
		* @brief handle the documents selection.
		*
		* This function is dedicated for handling the documents selection
		* By default this uses the notSuitableFor, forAppMode, needsNumObjects,
		* firstObjectType and secondObjectType variables.
		* If you need more control over the selection reimplement this function.
		*
		* @returns bool true when the Action should be enabled.
		*
		*/
		virtual bool handleSelection(ScribusDoc* doc, int SelectedType = -1);

	public slots:
		/**
		 * @brief Run the plug-in's main action.
		 *
		 * Run the plug-in's default action synchronously, blocking the rest of
		 * the application until this method returns.
		 *
		 * This method should generally call run(..., QIODevice* target) or
		 * run(..., QString target) to do the work. This is done by default,
		 * with the assumption that the target is a file. The argument is
		 * opened as QFile with IO_ReadOnly or IO_WriteOnly mode, depending on
		 * plugin type, and passed to run(..., QIODevice*). Override this if
		 * you need more flexibility.
		 *
		 * The optional target argument is a plugin-defined string indicating
		 * what the plugin should operate on. It will typically be a filename
		 * to import from / export to. Nothing stops a plugin doing funky
		 * things like accepting a WebDAV URI, etc, however.
		 *
		 * Plug-in authors might want to consider implementing this using the
		 * DeferredTask subclass used in runAsync, if the plug-in supports it.
		 *
		 * @param target Optional target to operate on (see
		 *                above)
		 * @returns bool True for success.
		 *
		 */
		virtual bool run(ScribusDoc* doc, QString target = QString::null) = 0;

		/**
		 * @brief Run the plug-in's main action.
		 *
		 * This method behaves as the previous one. Except there is
		 * a parent widget reference. It's useful e.g. when you need to
		 * open a dialog on a specific parent one.
		 */
		virtual bool run(QWidget* parent, ScribusDoc* doc, QString target = QString::null);

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
		virtual bool run(ScribusDoc* doc, QIODevice* target);

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
		 * cleaning it up using deleteLater(...) once it has finished. Remember
		 * that a DeferredTask is automatically deleted when its parent is, if
		 * it hasn't already been deleted. This approach is preferred where
		 * possible.
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
		///! Obsolete method - do not use this or rely on it in new code.
		const QString & runResult() const;

	protected:
		// Action info. To be set up by ctor.
		ActionInfo m_actionInfo;
		// Obsolete - see runResult()
		QString m_runResult;
		ScribusDoc* m_Doc;
};

/**
 * @brief A plug-in that is resident for the lifetime of the app
 *
 * ScPersistentPlugin describes a plugin that is to be kept resident for the
 * lifetime of the app (or until unloaded by a request to the plug-in manager).
 * Persistent plugins are useful where the plugin author needs to retain state,
 * needs more control of how they integrate with the Scribus user interface,
 * and/or wants to perform tasks in the background.
 *
 * Such plug-ins have an init method and a cleanup method; they have
 * no "run" method as such. It is up to the plugin to determine how
 * best to interfact with the user.
 */
class SCRIBUS_API ScPersistentPlugin : public ScPlugin
{
	Q_OBJECT

	public:

		/** @brief ctor
		 *
		 * @sa ScPlugin::ScPlugin()
		 */
		ScPersistentPlugin();

		//! \brief Pure virtual dtor for abstract class
		virtual ~ScPersistentPlugin() = 0;

		/**
		 * @brief Initialize the plugin
		 *
		 * This method must initialize the plugin. It is called at plug-in load
		 * time. This method will never be called twice without an intervening
		 * cleanupPlugin call, but there is no guarantee the plugin will actually
		 * be unloaded between initPlugin() and cleanupPlugin().
		 *
		 * It will usually instantiate a class and store a pointer to the
		 * instance in a static variable of the plugin or a member of its
		 * ScPersistentPlugin subclass, letting that class do the work of setup
		 * and integration into the app.
		 *
		 * Note that this is NOT the same as the init function of the plugin. That
		 * is always simple and lightweight, intended to just get an instance of this
		 * class back to the core app. This function is where any time-consuming or
		 * memory expensive init should be triggered, generally by the instantiation
		 * of the class that does the real work for the plugin.
		 *
		 * If this method returns failure, the plugin will be treated as dead and
		 * cleanupPlug() will not be called.
		 *
		 * @returns bool True for success.
		 */
		virtual bool initPlugin() = 0;

		/**
		 * @brief Deactivates the plugin for unloading / program quit
		 *
		 * This method will be called when the plug-in is about to be unloaded,
		 * or if the plug-in manager has been asked to disable the plug-in.
		 * This method will never be called unless initPlugin has been called
		 * first, but there is no guarantee the plugin will actually be
		 * unloaded after this is called, or before initPlugin is called again.
		 *
		 * @returns bool True for success.
		 */
		virtual bool cleanupPlugin() = 0;
};

// Plug-in API version used to check if we can load the plug-in. This
// does *NOT* ensure that the plug-in will be compatible with the internal
// Scribus APIs, only that the ScPlugin class and its standard subclasses
// will be compatible with what we expect, and that "extern C" functions
// we need will be present and work as expected. It's a preprocessor directive
// to make sure that it's compiled into each plugin rather than referenced
// from the main code.
//
// The API version is currently simply incremented with each incompatible
// change. Future versions may introduce a minor/major scheme if necessary.
#define PLUGIN_API_VERSION 0x00000008


#endif
