/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MACROMANAGER_H
#define MACROMANAGER_H

// pulls in <Python.h> first
#include "cmdvar.h"

#include "qguardedptr.h"

// forward-declare Macro; we don't actually need anything
// more here.
class Macro;

//typedef QGuardedPtr<Macro> GuardedMacro;

/*! \file macromanager.h
  \brief Scripter macro manager class

  \author Craig Ringer
  \date January 2005

  This file contains the MacroManager class.
  */

/*! \class MacroManager
  \brief  MacroManager registers and manages Python macros

  \author Craig Ringer
  \date January 2005

  MacroManager is a singleton; only one instance may ever exist.  It manages
  macros, providing methods to create, edit, run and destroy them.

  MacroManager has methods to register and unregister macros exposed by the
  python callable functions \ref register_macro, \ref register_macro_code and
  \ref unregister_macro. See their definitions and docstrings fore more info.

  Creating a macro causes MacroManager to make a new \ref Macro instance,
  which it keeps a pointer to. MacroManager inserts the QAction of the
  newly created macro into the Macro menu when the macro is created.

  In addition to this fairly basic functionality, MacroManger provides
  access to the user interface to manage macros (\ref ManageMacrosDialog)
  and to edit them (\ref EditMacroDialog).

  MacroManager proxies signals and slots from the Macro objects it controls.
  You should never need to access a macro object directly (or in fact be able
  to) from outside MacroManager. This lets MacroManager provide a "single point
  of contact" for information about macros, and a single object to connect to
  for signalling.

  It may now be safe to work with macros outside the macro manager, as the
  macro manager checks that the macros it is working on have not been deleted
  before doing anything with them. Macros should not be created by other
  classes, however.

  MacroManager should never need to care about Python specifics. It should
  eventually be able to support macros written in different languages, or
  even macros with totally different concepts (think "action list" macros
  recorded using the undo framework).
  */
class MacroManager : public QObject
{
	Q_OBJECT

	// typedefs
	typedef QGuardedPtr<Macro> MacroGuardedPtr;

public:

	/*! \brief Singleton constructor

	  \return The MacroManager instance, which is created if required.
	  \warning Try not to hang on to references to the returned
	           instance for more than a function call - call the
	           constructor again if you need to access the object.
	  */
	static MacroManager* instance();

	/// \brief Singleton destructor. Destroy the stored MacroManager instance, if any.
	static void deleteInstance();

	/// \brief Returns true if there is currently a MacroManger instance
	static bool haveInstance();

	/// \brief Return the number of macros registered with MacroManager
	int macrosRegistered();

public slots:

	/*! \brief Create a new macro that initially does nothing

	  \param macroName The name to give the macro in the macro menu
	  \param accel An accel key sequence for the menu item (may be ""). Eg "CTRL+F3".
	  \return true for success

	  See also Macro::Macro(), MacroManager::setSource(), MacroManager::setCallable()
	  */
	bool newMacro(const QString macroName, const QString accel);

	/*! \brief Checks if the named macro exists and returns true if so

	  This function checks the passed macro name, ensuring that it
	  is registered and that, if it is, the corresponding macro still
	  exists. If there is a valid macro by that name, it returns true.

	  If the name is registered but the corresponding macro has been
	  deleted, nameRegistered notices this and removes the entry for
	  the macro before returning false.

	  \attention This function performs it's cleanup of the list
	             of registered macros as a side effect. This should
	             not be a problem, as nobody outside the macro manager
	             should ever know or care about the difference beteween
	             "registered but deleted" and "not registered", but is
	             worth keeping in mind especially for internal use.

	  \param macroName The name to check for
	  \returns true if there is a valid macro by the given name
	*/
	bool nameRegistered(QString macroName);

	/*! \brief Unregister all macros

	  This slot unregisters all macros, destroying all macro objects under the
	  macro manager's control and clearing the macro menu.  It does not destroy
	  MacroManager or delete its menus.
	  */
	void clearMacros();

	/*! \brief Shut down the macro manager.

	  Close down the macro manager, clearing all macros, deleting the macro
	  menu, and destroying the macro mananger.
	  */
	void endManager();

	/*! \brief Display a dialog to edit the named macro

	  Display a dialog to edit the macro with the specified name. If the macro
	  does not exist, an error dialog is displayed. If the macro doesn't know
	  its source code, an error dialog is displayed.

	  The editing dialog handles all Python errors.

	  \param macroName The name of the macro to edit
	  */
	bool editMacroDialog(QString macroName);

	/*! \brief Display a dialog to create a new macro

	  This function prompts the user for a name for a new macro. If the user
	  gives a name, the macro is created and editMacroDialog is called so the
	  user can set it up. If editMacroDialog is cancelled, the macro is deleted
	  again.
	  */
	bool newMacroDialog();


	/*! \brief Display a dialog to manage all defined macros
	*/
	void manageMacrosDialog();

	/*! \brief Unregister and delete the named macro

	  \param macroName The name of the macro to delete

	  See also \ref Macro::~Macro()
	  */
	void deleteMacro(QString macroName);

	/// Import the passed file as a Python extension script
	void importMacros(QString filePath);

	/// Save all save-able macros to the passed file
	void exportMacros(QString filePath);

	/****** proxies for slots on Macro ******/
	void setMacroName(QString macroName, QString newName);
	void setAccel(QString macroName, QString newAccel);
	bool setSource(QString macroName, QString newSource);
	bool setCallable(QString macroName, PyObject* newCallable);
	QString exceptionType(QString macroName);
	PyObject* exceptionPyType(QString macroName); // borrowed reference
	QString exceptionText(QString macroName);
	QString exceptionTraceback(QString macroName);

	/****** Macromanger Macro signal handlers and signal proxies ********/

	void handleDeleting(Macro* macro);
	void handleNameChanged(Macro* macro, QString oldName);
	void handleAccelChanged(Macro* macro);
	void handleCallableChanged(Macro* macro);
	void handleSourceChanged(Macro* macro);
	void handleAboutTextChanged(Macro* macro);
	void handlePythonError(Macro* macro);
	void handleExecutionError(Macro* macro);
	void handleNewSourceError(Macro* macro);
	void handleNewCallableError(Macro* macro);

protected:
	/*! \brief MacroManager's real constructor

	  Add the Macro menu to the main menu and init members.
	  */
	MacroManager();

	/*! \brief MacroManager's real destructor

	  Remove the macro menu from the main menu delete all managed
	  macros.
	  */
	~MacroManager();

	/*! \brief Return the macro object with the given name

	  \warning The macro may be deleted by someone else
	  after you receive it. Don't keep it.

	  \param macroName the name of the macro to fetch
	  \return pointer to the requested macro object, or NULL if not found.
	  */
	Macro* operator[](const QString macroName);

protected slots:

	/*! \brief Insert the named macro into the user interface
	*/
	void connectMacroToUI(QString macroName);

signals:
		/// Emitted when the macromanager clears all macros
		void macrosCleared();

		/// Emitted to signal the creation of a new macro
		void macroCreated(QString);

		/// Emitted to signal the creation of a new macro
		void macroCreated(QString,QString);

		/// Emitted when the creation of a new macro fails
		void newMacroFailed(QString);

		/// Emitted when the renaming of a macro fails because of a name conflict
		void nameConflictError(QString);

		// the following signals are all proxied for the
		// corresponding signals on Macro. See the Macro docs
		// for what they do. The purpose of proxying them like
		// this is so that the macromanager can be solely
		// responsible for tracking the macro instances, and can
		// be the only point other objects need to connect to to
		// be notified of changes to any macro.
		void macroDeleting(QString);
		void macroNameChanged(QString,QString);
		void macroSourceChanged(QString,QString);
		void macroCallableChanged(QString);
		void macroAccelChanged(QString,QString);
		void macroAboutTextChanged(QString,QString);
		// proxied python error notifications. All are (name, excType, excText, excTraceback)
		void macroPythonError(QString,QString,QString,QString);
		void macroExecutionError(QString,QString,QString,QString);
		void macroNewSourceError(QString,QString,QString,QString);
		void macroNewCallableError(QString,QString,QString,QString);


private:
	/// A mapping of macro names to guarded pointers to macro objects
	QMap<QString,MacroGuardedPtr> macrosByName;

	/// The singleton instance returned by MacroManager::instance()
	static MacroManager* _instance;

	/// the Macro menu created in the main menu toolbar
	QPopupMenu macroMenu;

	/// the id of the macro menu in the menu bar
	int macroMenuId;

	/// The QAction associated with the "manage macros" menu entry
	QGuardedPtr<QAction> manageMacrosAction;

};//class MacroManager

#endif
