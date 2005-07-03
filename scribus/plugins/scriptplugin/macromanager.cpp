/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "macromanager.h"

#include "macro.h"
#include "editmacrodialog.h"
#include "managemacrosdialog.h"
#include "scriptercore.h"

#include "qinputdialog.h"
#include "qmessagebox.h"

// This file is documented in macromanager.h; please refer
// to that or the doxygen API docs generated from it for
// information about the classes and functions defined here.

// init the static _instance var on the MacroManager class
// to null.
MacroManager* MacroManager::_instance    = 0;


/*
   Implementation notes

   See macro.cpp

   Also note that MacroManager should avoid Python/C API calls; these should be
   made in Macro and MacroManager should simply ask Macro for the result.

   MacroManger doesn't want to know anything about the Python/C API, that's
   Macro's job. It should never see any exceptions or anything more than
   PyObject*s that it just passes around.

   Eventually, MacroManager should no longer even require Python.h, and should
   be able to operate on macros without caring what they are. It might even
   support macros in other languages or with different concepts (such as
   "action list" based recorded macros).

   MacroManager may want to become two classes later - one that manages its
   dialogs, and another that just implements the core.

   While MacroManager is written with the expectation that it should be the
   only class working with its macros, it stores them them with guarded
   pointers and tries to ensure they're valid before accessing them. It should
   be able to cope with them being modified and deleted under it. It is,
   however, not currently possible to create a macro outside the macro manager
   and have the macro manager take ownership of it later. Just to be safe, it's
   probably best to let the macro manager protect and look after all macros.

   This class is not thread safe.
   */


MacroManager::MacroManager()
{
	this->setName("MacroManager");
	this->macroMenu.setName("macroMenu");

	// Set up the QAction for the "manage macros" menu item
	this->manageMacrosAction = new QAction(Carrier, "manageMacrosAction");
	assert(this->manageMacrosAction); // die promptly if the constructor fails
	this->manageMacrosAction->setText(tr("Manage Macros"));
	this->manageMacrosAction->setWhatsThis(
			tr("Brings up a graphical window for creating, deleting, "
				"editing, saving and loading macros."));
	this->manageMacrosAction->setToolTip(tr("Create, edit and delete macros"));
	connect(this->manageMacrosAction, SIGNAL(activated()),
			this, SLOT(manageMacrosDialog()) );
	// and add it to the menu
	this->manageMacrosAction->addTo(&(this->macroMenu));
	macroMenu.insertSeparator();

	// Now add our menu to the Scribus menu.
	// FIXME: makes assumptions about menu bar layout
	Carrier->menuBar()->insertItem( tr("&Macro"), &(this->macroMenu),
			-1, Carrier->menuBar()->count() - 2);

	// There are a few signals we may emit that we may want to listen to
	// ourselves. Hook 'em up.
	connect(this, SIGNAL(macroCreated(QString,QString)),
			this, SLOT(connectMacroToUI(QString)));
}


MacroManager::~MacroManager()
{
	// Unregister and release our macros
	this->clearMacros();
	// then destroy the QAction for the manage macros dialog.
	if (this->manageMacrosAction)
	{
		delete this->manageMacrosAction;
		this->manageMacrosAction = NULL;
	}
	else
		qDebug("MacroManager::manageMacrosAction already deleted!");
}


// return the stored MacroManager instance, or create one if necessary
MacroManager* MacroManager::instance()
{
	if (_instance == NULL)
		_instance = new MacroManager();
	return _instance;
}


// destroy the single instance of MacroManager.
void MacroManager::deleteInstance()
{
	if (_instance != NULL)
		delete _instance;
	_instance = NULL;
}


bool MacroManager::haveInstance()
{
	return _instance != NULL;
}


// Create a new macro object. It will initially do nothing when called, but
// setSource() or setCallable() may be used to give it functionality.
bool MacroManager::newMacro(const QString macroName, const QString accel)
{
	if (nameRegistered(macroName))
	{
		qDebug("MacroManager::newMacro(): "
				"Requested name '%s' is already registered.",
				(const char*)(macroName.utf8()));
		emit newMacroFailed(macroName);
		// FIXME: we should probably emit nameConflictError also/instead
		return false;
	}

	// create the macro object
	QGuardedPtr<Macro> macro = new Macro(macroName, accel);
	if (!macro)
	{
		// new failed. Probably a memory problem, but we can't be sure
		// so try to continue running.
		qDebug("MacroManager::newMacro(): "
				"new Macro('%s') failed (memory problem)?",
				(const char*)(macroName.utf8()));
		emit newMacroFailed(macroName);
		return false;
	}

	// And immediately hook up all the signals we need
	connect( macro, SIGNAL(pythonError(Macro*)),
			SLOT(handlePythonError(Macro*)) );
	connect( macro, SIGNAL(executionError(Macro*)),
			SLOT(handleExecutionError(Macro*)) );
	connect( macro, SIGNAL(newSourceError(Macro*)),
			SLOT(handleNewSourceError(Macro*)) );
	connect( macro, SIGNAL(newCallableError(Macro*)),
			SLOT(handleNewCallableError(Macro*)) );
	connect( macro, SIGNAL(nameChanged(Macro*,QString)),
			SLOT(handleNameChanged(Macro*,QString)) );
	connect( macro, SIGNAL(accelChanged(Macro*)),
			SLOT(handleAccelChanged(Macro*)) );
	connect( macro, SIGNAL(deleting(Macro*)),
			SLOT(handleDeleting(Macro*)) );
	connect( macro, SIGNAL(callableChanged(Macro*)),
			SLOT(handleCallableChanged(Macro*)) );
	connect( macro, SIGNAL(sourceChanged(Macro*)),
			SLOT(handleSourceChanged(Macro*)) );
	connect( macro, SIGNAL(aboutTextChanged(Macro*)),
			SLOT(handleAboutTextChanged(Macro*)) );

	// insert the macro into macrosByName
	macrosByName.insert(macroName, macro);

	// Emit some signals to inform anybody who's listening that a new macro was
	// created. We do this here rather than proxying one emitted by the Macro
	// constructor because we want the macro to be fully set up with its
	// callable, source code, etc.
	emit macroCreated(macro->macroName(), macro->accel());

	return true;
}


// Add a menu entry for the passed macro by adding its QAction to the menu.
// This slot should be connected to the macroCreated() signal on MacroManager
void MacroManager::connectMacroToUI(QString macroName)
{
	Macro* macro = (*this)[macroName];
	if (macro == NULL)
		qDebug("MacroManager::connectMacroToUI(): Called with non-existent macro '%s'",
				(const char*)(macro->macroName().utf8()));

	// The macro has already created a QAction that can be hooked
	// up to whatever is appropriate. Hook it up to the macros menu.
	macro->action()->addTo(&macroMenu);
}


Macro* MacroManager::operator[](const QString macroName)
{
	// Make sure the macro exists
	if (!this->nameRegistered(macroName))
		// Nope; it's not registered or has been deleted
		return NULL;
	// and return a pointer to it
	return macrosByName[macroName];
}


void MacroManager::deleteMacro(QString macroName)
{
	//assert(macrosByName.autoDelete());

	// grab a pointer to the macro, making sure it's actually there
	Macro* macro = (*this)[macroName];
	if (!macro)
		return;

	// sanity check - macro name in dict matches passed macro name
	if (macro->macroName() != macroName)
		qDebug("MacroManager::deleteMacro(): Macro name "
				"in macros dict doesn't match what the macro thinks its name is!");

	// remove the macro from the dictionary
	macrosByName.remove(macroName);
	// and delete it.
	delete macro;
}


// returns the number of macros registered
int MacroManager::macrosRegistered()
{
	return macrosByName.count();
}


// Returns if a macro is registered
bool MacroManager::nameRegistered(QString macroName)
{
	// First, check the key is actually in the mapping. If we don't do this,
	// we'll get a dummy value in the mapping for the key when we look it up,
	// which is not what we want.
	if (macrosByName.find(macroName) == macrosByName.end())
		// The name isn't in our mapping at all
		return false;
	else if (macrosByName[macroName].isNull())
	{
		// It's in the mapping, but has been deleted. Take
		// it out of the mapping now, as it's rather useless.
		macrosByName.remove(macroName);
		return false;
	}
	else
		// It's in the mapping and not null
		return true;
}


void MacroManager::clearMacros()
{
	typedef QValueList<MacroGuardedPtr> MacroList;
	// Get a list of all macros we have registered
	MacroList macros = macrosByName.values();
	// And delete them all
	MacroList::iterator it;
	for (it = macros.begin(); it != macros.end(); ++it)
		if (*it)
			delete (*it);
	// and clear the macro dict
	macrosByName.clear();

	// now tell anybody who's listening that we've nuked
	// the lot.
	emit(macrosCleared());
}


// endManager is really just a non-static form of the
// singleton's destructor that can be called as a slot. It
// simply calls MacroManager::deleteInstance();
void MacroManager::endManager()
{
	MacroManager::deleteInstance();
}


// Display a dialog for editing macros.
// TODO: This probably wants to move into ManageMacrosDialog its self later.
void MacroManager::manageMacrosDialog()
{
	ManageMacrosDialog* dia = new ManageMacrosDialog(Carrier, "manageMacrosDialog");

	// Iterate over the contents of the name->id macro mapping, inserting the
	// macros and their descriptions into the macro table.
	// We iterate over the keys, doing lookups for each, because that way each
	// macro gets checked to see if it's been deleted when we look it up.
	QStringList macroNames = macrosByName.keys();
	QStringList::Iterator it;
	for ( it = macroNames.begin() ; it != macroNames.end() ; ++it )
	{
		// Look up the macro. If it exists, add it to the table.
		Macro* macro = (*this)[*it];
		if (macro)
			dia->appendRow(macro->macroName(), macro->aboutText(), macro->accel(), macro->source());
	}

	// Adjust the table layout so all the items fit
	dia->adjust();

	// Connect some signals the dialog may emit to methods we implement.
	connect( dia, SIGNAL(deleteAll()), SLOT(clearMacros()) );
	connect( dia, SIGNAL(exportMacros(QString)), SLOT(exportMacros(QString)) );
	connect( dia, SIGNAL(importMacros(QString)), SLOT(importMacros(QString)) );
	connect( dia, SIGNAL(editMacro(QString)), SLOT(editMacroDialog(QString)) );
	connect( dia, SIGNAL(newMacro()), SLOT(newMacroDialog()) );
	connect( dia, SIGNAL(deleteMacro(QString)), SLOT(deleteMacro(QString)) );

	// And connect up others to slots we proxy for Macro
	connect( dia, SIGNAL(renameMacro(QString,QString)),
			SLOT(setMacroName(QString,QString)) );
	connect( dia, SIGNAL(changeMacroAccel(QString,QString)),
			SLOT(setAccel(QString,QString)) );

	// Connect signals we may emit to the dialog
	connect( this, SIGNAL(macrosCleared()), dia, SLOT(clearTable()) );
	connect( this, SIGNAL(macroCreated(QString,QString)),
			dia, SLOT(appendRow(QString,QString)) );
	connect( this, SIGNAL(nameConflictError(QString)),
			dia, SLOT(renameFailed(QString)) );

	// Connect signals we proxy for macro to slots on the dialog.
	connect( this, SIGNAL(macroDeleting(QString)),
			dia, SLOT(deleteRow(QString)) );
	connect( this, SIGNAL(macroAccelChanged(QString,QString)),
			dia, SLOT(updateAccel(QString,QString)) );
	connect( this, SIGNAL(macroNameChanged(QString,QString)),
			dia, SLOT(updateName(QString,QString)) );
	connect( this, SIGNAL(macroSourceChanged(QString,QString)),
			dia, SLOT(updateEditable(QString,QString)) );
	connect( this, SIGNAL(macroAboutTextChanged(QString,QString)),
			dia, SLOT(updateDescription(QString,QString)) );

	// Then display the dialog
	dia->exec();

	// Dialog exited. That's it, we don't need to apply
	// state changes. They're done while the dialog runs.
	delete dia;
}


// export macros to a special Python script, not intended
// for user editing but easy to import into Scribus with
// source code attributes etc intact.
void MacroManager::exportMacros(QString filePath)
{
	QFile macroFile(filePath);
	if (macroFile.open( IO_WriteOnly ))
	{
		QTextStream stream(&macroFile);
		// print the header of the file
		stream << "#!/usr/bin/env python\n";
		stream << "# -*- coding: utf-8 -*-\n";
		stream << "# scribus macro file - not intended for direct editing\n\n";
		// import the scripter interface
		stream << "try:\n";
		stream << "    import scribus\n";
		stream << "except ImportError:\n";
		stream << "    print 'This is a Scribus macro file.'\n";
		stream << "    print 'It can\\'t be run as a normal Python script'\n\n";
		// FIXME: should write a version check out too. Hard - what about i18n?
		// Iterate over the list of all macro names, getting each macro and
		// writing it out. We iterate over the keys rather than iterating over
		// the key/value pairs because this lets us check for and clean up
		// deleted macros neatly.
		QStringList macroNames = macrosByName.keys();
		QStringList::Iterator it;
		for ( it = macroNames.begin() ; it != macroNames.end(); ++it )
		{
			// Look up the macro, and if it exists write it out.
			Macro* macro = (*this)[*it];
			if (macro)
			{
				// get the repr() of macro.__source__
				QString source = macro->source(true);
				// and the name of the macro
				QString name = macro->macroName();
				// we can't test for QString::null or QString("") because of the
				// quotes repr() adds. Instead, require length => 5 for macro source.
				if (source.length() >= 5)
				{
					// write out the function
					stream << "# definition of " << name << "\n";
					stream << "scribus.register_macro_code('" << name << "', " << source << ");\n\n";
				}
				else
					stream << "# " << name << " couldn't be written (no source stored)\n\n";
			}
		}
		macroFile.close();
	}
	else
		// TODO: report what the actual error was, eg Permission Denied
		QMessageBox::critical(Carrier,
				tr("Scribus - Macro Manager"),
				tr("Unable to open the requested file: %1")
					.arg( qApp->translate("QFile",macroFile.errorString()) ));
}


// import macros from a Python file, but formatted specially
// and not intended for user editing. While this should so
// more checking, currently it just runs the script in the
// main namespace.
void MacroManager::importMacros(QString filePath)
{
	// Use `load script from file' to execute the macro file
	scripterCore->slotRunScriptFile(filePath, true);
}


bool MacroManager::editMacroDialog(QString macroName)
{
	// make sure the macro exists
	Macro* macro = (*this)[macroName];
	if (macro == NULL)
		return false;

	// create the dialog
	EditMacroDialog* dia = new EditMacroDialog(Carrier, "editMacroDialog");

	// Set the dialog up with the macro's name and source
	dia->setName(macroName);
	dia->setSource(macro->source());
	dia->setCaption(tr("Scribus - Edit Macro"));

	// Connect up some signals it may send us
	connect( dia, SIGNAL(compile(QString,QString)),
			SLOT(setSource(QString,QString)) );

	// and some signals we may send it
	connect( this, SIGNAL(macroSourceChanged(QString,QString)),
			dia, SLOT(compileWorked(QString)) );
	connect( this, SIGNAL(macroNewSourceError(QString,QString,QString,QString)),
			dia, SLOT(compileFailed(QString,QString,QString,QString)) );

	// and display it
	bool result = dia->exec();
	if (result)
	{
		// User accepted the dialog. The code will have already been
		// compiled by the time the dialog returns, so we don't actually
		// need to do anything here. As a sanity check, we will make sure
		// the source the dialog has is the same as the source the macro
		// has.
		if (dia->getSource() != macro->source())
			qDebug("MacroManager::editMacroDialog(): '%s': "
					"Macro and dialog disagree about macro's source.",
					(const char*)(macro->macroName().utf8()) );
	}
	delete dia;
	return result;
}


// TODO: This is all very procedural. Can we do a better, simpler job
// using our signalling better?
bool MacroManager::newMacroDialog()
{
	// ask the user what they want to call the macro. We do this
	// in a loop until they cancel or give a valid name. It'd be
	// better to validate the contents in the QInputDialog's accept()
	// slot, but that involves writing another dialog and I frankly
	// can't be assed right now. FIXME.
	bool dialogAccepted = true;
	bool nameOk = false;
	QString macroName = QString("");
	while (dialogAccepted && !nameOk)
	{
		nameOk = false;
		macroName = QInputDialog::getText(
				tr("Scribus - New Macro"), tr("Enter name for new macro: "),
				QLineEdit::Normal, macroName, &dialogAccepted, Carrier);
		if (dialogAccepted)
		{
			// User OK'd the dialog. Is their chosen name valid?
			if (nameRegistered(macroName))
			{
				// Nope; it's taken. Whine at the user then continue
				// the loop to ask them for a new name.
				QMessageBox::warning(Carrier, tr("Scribus - Macro Manager"),
						tr("<qt>The macro name you requested is already taken "
							" by another macro.</qt>"),
						QMessageBox::Ok|QMessageBox::Default,
						QMessageBox::NoButton);
			}
			else
				// Yep, it's OK. The loop will end.
				nameOk = true;
		}
	}
	if (!dialogAccepted)
		// user cancelled
		return false;

	if (!newMacro(macroName, QString::null))
	{
		// Whoops, creation failed. We don't handle the newMacroFailed signal
		// ourselves (because we don't always care), so we'll inform the user
		// here and now.
		QMessageBox::warning(Carrier, tr("Scribus - Macro Manager"),
				tr("<qt>Macro creation failed. The macro manager was "
					"unable to set up the macro.</qt>"),
				QMessageBox::Ok|QMessageBox::Default,
				QMessageBox::NoButton);
		return false;
	}

	// make sure the macro now exists
	assert(nameRegistered(macroName));

	// Call the edit macro dialog on the new macro
	if (!editMacroDialog(macroName))
	{
		// It failed; the user most likely cancelled. The newly created
		// macro isn't very useful without any function, so we remove it.
		deleteMacro(macroName);
		return false;
	}
	return true;
}





/************ SIGNAL HANDLERS AND SIGNAL/SLOT PROXIES FOR 'Macro' *****/

// Nobody is supposed to talk directly to Macro instances, so we relay
// signals and slot calls between the macro instances we manage and the
// outside world.


/* ===== Relay signals emitted by macro ===== */

// Each of these methods is called when the corresponding signal
// is emitted by one of the macros managed by this class. It
// emits the corresponding signal from MacroManager with the
// macro's name and in most cases useful arguments.


// Handle the deletion of a macro
void MacroManager::handleDeleting(Macro* macro)
{
	// pass on the signals to anybody we may have waiting
	// for them.
	emit macroDeleting(macro->macroName());

	// We don't remove the macro from the macro dict; as we
	// require that only MacroManager ever delete macros
	// and set the dict to autodelete, the macro is being
	// deleted _because_ it's been removed from the dict.
	// We don't need to remove it from the menu either, as
	// it will delete its QAction and hence remove its self
	// from everywhere that it's referenced.
}


// Handle the renaming of a macro
void MacroManager::handleNameChanged(Macro* macro, QString oldName)
{
	// pass on the signals to anybody we may have waiting
	// for them.
	emit macroNameChanged(macro->macroName(), oldName);
	// Ensure that the macro is actually registered under its old name
	if ((*this)[oldName] != macro)
		// oops! The passed macro isn't the same as the one we have in our
		// macro mapping for that name.
		qDebug("MacroManager::handleNameChanged(): Registered macro for name "
				"doesn't match passed macro");
	// Remove the macro from the mapping
	macrosByName.remove(oldName);
	// and insert the macro under its new name
	macrosByName.insert(macro->macroName(), macro);
	// Note that we don't have to hande the menus, etc; the macro
	// will rename its QAction its self.
}


// The macro has had its accelerator changed. Handle
// that.
void MacroManager::handleAccelChanged(Macro* macro)
{
	emit macroAccelChanged(macro->macroName(), macro->accel());
}



void MacroManager::handleCallableChanged(Macro* macro)
{
	emit macroCallableChanged(macro->macroName());
}


void MacroManager::handleSourceChanged(Macro* macro)
{
	emit macroSourceChanged(macro->macroName(), macro->source());
}


void MacroManager::handleAboutTextChanged(Macro* macro)
{
	emit macroAboutTextChanged(macro->macroName(), macro->aboutText());
}


void MacroManager::handlePythonError(Macro* macro)
{
	emit macroPythonError(macro->macroName(), macro->exceptionType(),
			macro->exceptionText(), macro->exceptionTraceback());
	QMessageBox::warning(Carrier, tr("Scribus - Macro Manager"),
			tr("The macro '%1' has reported a minor error.\n"
				"The error is: %2\n"
				"A full traceback follows:\n\n%3\n")
				.arg(macro->macroName())
				.arg(macro->exceptionText())
				.arg(macro->exceptionTraceback()),
			QMessageBox::Ok|QMessageBox::Default,
			QMessageBox::NoButton);
}


void MacroManager::handleExecutionError(Macro* macro)
{
	emit macroExecutionError(macro->macroName(), macro->exceptionType(),
			macro->exceptionText(), macro->exceptionTraceback());
	QMessageBox::warning(Carrier, tr("Scribus - Macro Manager"),
			tr("The macro '%1' failed to execute correctly.\n"
				"The error is: %2\n"
				"A full traceback follows:\n\n%3\n")
				.arg(macro->macroName())
				.arg(macro->exceptionText())
				.arg(macro->exceptionTraceback()),
			QMessageBox::Ok|QMessageBox::Default,
			QMessageBox::NoButton);
}


void MacroManager::handleNewSourceError(Macro* macro)
{
	emit macroNewSourceError(macro->macroName(), macro->exceptionType(),
			macro->exceptionText(), macro->exceptionTraceback());
	// We let any active dialog handle this, or let it be handled by
	// checking the return values of the related functions. There's not
	// much point in lobbing a generic dialog at the user for this.
}


void MacroManager::handleNewCallableError(Macro* macro)
{
	emit macroNewCallableError(macro->macroName(), macro->exceptionType(),
			macro->exceptionText(), macro->exceptionTraceback());
	// see notes on handleNewSourceError
}



/* ===== Dispatch slot calls to Macro ===== */

// These methods should all be named the same as the corresponding method on
// Macro, and have the same signature with an extra argument, the macro name.
// This gives other classes a single target for connections.

// Each of these methods just passes the slot call on to the named
// macro and, if appropriate, returns the result.

void MacroManager::setMacroName(QString macroName, QString newName)
{
	Macro* macro = (*this)[macroName];
	if (macro)
	{
		if (!nameRegistered(newName))
			macro->setMacroName(newName);
		else
			emit nameConflictError(macroName);
	}
}


void MacroManager::setAccel(QString macroName, QString newAccel)
{
	Macro* macro = (*this)[macroName];
	if (macro)
		macro->setAccel(newAccel);
}

bool MacroManager::setSource(QString macroName, QString newSource)
{
	Macro* macro = (*this)[macroName];
	if (macro)
		return macro->setSource(newSource);
	else
		// report to user too?
		return false;
}

bool MacroManager::setCallable(QString macroName, PyObject* newCallable)
{
	Macro* macro = (*this)[macroName];
	if (macro)
		return macro->setCallable(newCallable);
	else
		// report to user too?
		return false;
}


QString MacroManager::exceptionType(QString macroName)
{
	Macro* macro = (*this)[macroName];
	if (macro)
		return macro->exceptionType();
	else
		return QString::null;
}


// note: returns borrowed reference
PyObject* MacroManager::exceptionPyType(QString macroName)
{
	Macro* macro = (*this)[macroName];
	if (macro)
		return macro->exceptionPyType();
	else
		return NULL;
}


QString MacroManager::exceptionText(QString macroName)
{
	Macro* macro = (*this)[macroName];
	if (macro)
		return macro->exceptionText();
	else
		return QString::null;
}


QString MacroManager::exceptionTraceback(QString macroName)
{
	Macro* macro = (*this)[macroName];
	if (macro)
		return macro->exceptionTraceback();
	else
		return QString::null;
}

