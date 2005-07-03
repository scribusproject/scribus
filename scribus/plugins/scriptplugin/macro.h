/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MACRO_H
#define MACRO_H

// pulls in Python.h first
#include "cmdvar.h"

/*! \file macro.h
  \brief Scripter macro class

  \author Craig Ringer
  \date January 2005

  This file contains the Macro class.
  */

/*! \class Macro
  \brief Macro carries and manages a Python callable and related information.

  \author Craig Ringer
  \date January 2005

  A Macro instance represents a single macro - a Python callable. It stores and
  manages a Python callable object, providing the ability to execute it,
  replace it with a new callable, compile a new callable from source, and
  access various information about the callable.

  Macro instances are managed by, unsurprisingly, MacroManager.  Nothing else
  should manipulate them directly; all signals, slots, etc are exposed through
  MacroManager. It may be possible to safely use Macro instances from other
  classes, but you'll need to check that VERY carefully.

  Macro's primary job is to manage a Python callable object. It holds a
  reference to the callable, preventing it from being garbage collected
  by Python, and is responsible for releasing that reference when its
  done with the object. Macro may also know the source code used to
  compile the object and may compile a new callable from source code.

  Note that you can register any Python callable object as
  a macro; it doesn't have to be a Python function. Notable
  options are:
  - functions
  - lambda functions
  - bound methods
  - class instances with a __call__ method

  If you are registering a macro in source code form, Macro compiles the source
  into a new callable. If the compiling and executing the source results in a
  callable object (usually a function, though you can also use lambdas or any
  other definition that results in a callable) than that callable is saved as
  the macro's new callable.

  If passed source code evaluates to a type definition (a "new-style class"),
  the macro will notice this. Instead of storing the type object (which, while
  callable, isn't very useful), it instantiates the type and uses the instance
  if the instance is callable without arguments. Instances are callable if they
  provide a __call__ method. A new-style class is any class that inherits from
  'object' or a subclass of 'object'.

  When the macro is executed, the stored callable is called without arguments.

  The macro can return the source code of its callable (if it knows it), the
  docstring of the callable (if it has one), the QAction object associated
  with it, its own name, whether the last call raised an exception, and details
  about the last exception if any.

  Macro handles all Python exceptions it encounters, never returning to the
  caller when an exception is set (this guarantee does NOT apply to private
  methods). If it runs into an exception, it'll set its own state to reflect
  the fact then clear the exception and emit a signal (see below). The
  exception information can be accesed with Macro::exceptionOccurred(),
  Macro::exceptionType(), Macro::exceptionText(), and
  Macro::exceptionTraceback().

  Macro is _so_ not threadsafe, especially the way it reports exceptions.

  Macro is intended to be communicated with using event-based mechanisms,
  namely signals and slots. A minimal procedural interface is provided
  for Python/C code that cannot be connected to signals.

  Macro provides a QAction object, accessible through Macro::action(), that can
  be added to toolbars and menus. When activated, the action object executes
  the macro. The action object is deleted when Macro is, causing the macro
  to automatically disappear from all toolbars and menus when it is deleted.
  */
class Macro : public QObject
{
	Q_OBJECT

public:
	/*! \brief Create a new macro

	  Create a new macro with the specified name and (optional) accel
	  sequence. The macro will have no callable, and if called will do
	  nothing. A QAction, accessible with Macro::action(), will be
	  created for connecting the macro to the user interface

	  \param accel An accel key sequence for the menu item (may be ""). Eg "CTRL+F3".
	  */
	Macro(QString newName, QString accel);

	/*! \brief destroy the macro, relaeasing the held reference to the Python callable, if any.

	  Destroy the macro. If any Python callable is currently being held, the reference
	  to it is released. The macro's QAction is deleted, causing it to vanish from
	  all menus and toolbars.
	*/
	~Macro();

public slots:
	/*! \brief Set the callable of the macro to an existing Python callable object

	  Register the passed Python callable object as this
	  macro's callable. The existing callable, if any,
	  has its reference count decreased and is then
	  discarded.

	  The macro will not know its own source code and will
	  not be editable in the macro manager. It will not
	  be possible to save the macro.

	  newCallableError will be emitted if something goes wrong.

	  \param newCallable A pointer to the Python callable to call when the macro is triggered
	  \return true if successful.
	  */
	bool setCallable(PyObject* newCallable);

	/*! \brief Set the callable of the macro to a callable created from the passed sourcecode

	  Compile and evaluate the passed Python source code, which must be a
	  single (probably compound) statement such as a function or class
	  definition. The resulting callable object is used as the new callable
	  for the macro. See Macro::compileCode() for details.

	  The existing callable, if any, has its reference count decreased and
	  is then discarded.

	  When the macro's callable is set using this slot, the macro will know
	  its own source code. It will know how to save its self, and will be
	  editable in the macro manager.

	  newSourceError will be emitted if something goes wrong.

	  \param newSourceCode The Python source to compile to the new callable
	  \return true if succcessful.
	  */
	bool setSource(QString newSourceCode);

	/// return the accelerator sequence assigned to the macro
	QString accel();

	/// Set a new accelerator sequence for the macro
	void setAccel(QString newAccel);

	/// Return the docstring of the macro's callable, or "" if none
	QString aboutText();

	/*! \brief Return the source of the macro's callable, or "" if none

	  Return the source code used to construct the macro's
	  current callable, or "" if the source is unknown.

	  \param repr if true, return the Python string literal representation of the source
	  \return the source code of the macro, or "" if none
	  */
	QString source(bool repr=false);

	/*! \brief Execute the macro if a callable is assigned

	  Executes the currently assigned callable.

	  If there is no callable assigned, prints a warning on
	  stderr and exists.

	  If the callable raises an exception, report the exeception with a
	  signal, then clear the exception and return.
	  */
	void execute();

	/// Return the name of the macro
	QString macroName();

	/// Set the name of the macro
	void setMacroName(QString newName);

	/// Did an exception occurr since the last Python API call?
	bool exceptionOccurred();

	/// Return the name of the last exception, or QString::null if none.
	QString exceptionType();

	/// Return the text of the last exception set, or QString::null if none
	QString exceptionText();

	/// Return the text of the traceback from the last exception, or QString::null if none
	QString exceptionTraceback();

	/*! \brief Return a pointer to the Python type object of the
			   last exception set, or NULL if none.

	  \warning Returns a borrowed reference - DO NOT DECREF.
	*/
	PyObject* exceptionPyType();

	/// Return a pointer to our QAction
	QAction* action();

private:
	/*! \brief Compile the passed Python source to a Python callable

	  Evaluate the passed source code in a private namespace and return the
	  callable object it defines in that namespace. The source code must be
	  a single statement or compound statement ('def', 'class', lambda
	  expression, etc), and may contain leading comments and/or white
	  space.

	  If the code evaluates to a type object ("new style class"),
	  instantiate it and return the resulting instance.  See the notes in
	  the implementation for details.

	  \warning  If this function fails, it sets an exception (or leaves
	  one set) and returns NULL.  The calling code is expected to handle
	  the Python exception. Failure to handle the exception will result in
	  spurious exceptions being reported later and/or a crash.

	  \param sourceCode The source code to evaluate
	  \return The resulting callable object, or NULL on
	  error, in which case a Python exception
	  will be set and must be handled.
	  */
	PyObject* compileCode(QString sourceCode);

	/*! \brief Return the python string literal representation of the passed string.

	  Use PyObject_Repr to convert the passed string into a Python string
	  literal representation of it, suitable for use in Python source code.

	  If an exception is encountered, it is handled internally and cleared.

	  \param fromString String to convert
	  \return Converted string, or QString::null if something goes wrong.
	  */
	QString reprString(QString fromString);

	/*! \brief Format the currently set Python exception to QStrings without clearing the exception

	  Formats the currently set Python exception and returns the text of it
	  as three QStrings - the exception type, the exception its self, and the
	  whole traceback. The exception type is also returned as a PyObject*.

	  \attention A Python exception will ALWAYS be set when this function
	  exits.  If you call it when there's no exception, one will be set and
	  the function will return false. If it fails, it'll set an exception
	  and return false. If it succeeds, it'll restore the exception that
	  was in place when it was called and return true. You MUST handle the
	  exception.

	  \param[out] typeObj PyObject* to set to a pointer to the new exception type object
	  \param[out] exceptionType QString to replace with the name of the exception type
	  \param[out] exceptionText QString to replace with the exception text
	  \param[out] tracebackText QString to replace with the traceback text
	  \returns error code. If false, the argument strings have not been set. An exception is always set.
	  */
	bool formatCurrentException( PyObject*& typeObj, QString& exceptionType, QString& exceptionText, QString& tracebackText);

	/*! \brief Update exception state to show an error and clear any current exception

	  Set the exception information members of the instance to the current
	  exception. An exception MUST be set when this function is called.

	  This function does not emit any signals; that's the caller's job.

	  Uses Macro::formatCurrentException()
	  */
	void setExceptionState();

	/*! \brief Update current exception state to show no error

	  Set the exception state menbers of the instance to indicate that
	  there has been no error. A Python exception MUST NOT be set when
	  this function is called.

	  This function does not emit any signals; that's the caller's job.
	  */
	void clearExceptionState();

	/// Helper for setMacroCode, setMacroCallable
	bool setMacroCallableAndSource(PyObject* newCallable, QString newSourceCode);

	/// Return the docstring of the current Python object, or QString::null
	QString getDocString();

	/// Return our name as a const char* ; convenience function for error formatting
	const char* charName();

	/// The Python source code of the macro
	QString sourceCode;

	/// The accelerator key sequence requested for the macro
	QString accelSeq;

	/// The name assigned to the macro
	QString macroNameVar;

	/// The Python callable to run when executed
	PyObject* callable;

	/// The docstring extracted from the callable
	QString docString;

	/// If last operation triggered an exception
	bool excOccurred;
	/// exception type after last operation, or QString::null
	QString excType;
	/// exception text after last operation, or QString::null
	QString excText;
	/// exception traceback after last operation, or QString::null
	QString excTraceback;
	/*! \brief Pointer to Python type object of last exception

	  This member variable contains a pointer to a Python type object
	  that defined the last exception to be set.
	*/
	PyObject* excPyType;

	/// The QAction managed by this macro, used to connect it to the user interface
	QGuardedPtr<QAction> actionObj;

private slots:
	/// Called when the description text is changed to set the whats this text on the QAction
	void updateWhatsThis();

signals:
		/*! \brief emitted when the macro is destroyed
		*/
		void deleting(Macro*);

		/*! \brief emitted when the macro is renamed

		  The second arg is the old name.
		  */
		void nameChanged(Macro*,QString);

		/*! \brief emitted when the source code of the macro changes
		*/
		void sourceChanged(Macro*);

		/*! \brief emitted when the macro's callable changes
		*/
		void callableChanged(Macro*);

		/*! \brief emitted when the accel sequence is changed
		*/
		void  accelChanged(Macro*);

		/*! \brief emitted when the docstring is changed
		*/
		void aboutTextChanged(Macro*);

		/*! \brief A Python error was encountered during execution of the callable

		  Information about the error is NOT sent with the signal, but may be
		  obtained by calling Macro::exceptionType() and related.
		  */
		void executionError(Macro*);

		/*! \brief A Python error was encountered while compiling/evaluating new source code for the macro

		  Information about the error is NOT sent with the signal, but may be
		  obtained by calling Macro::exceptionType() and related.
		  */
		void newSourceError(Macro*);

		/*! \brief A Python error was encountered while setting a new callable for the macro

		  Information about the error is NOT sent with the signal, but may be
		  obtained by calling Macro::exceptionType() and related.
		  */
		void newCallableError(Macro*);

		/// A Python error was encountered while doing something not covered by
		/// one of the more specific errors. This signal is really just for
		/// debugging and console reporting, it's sent by Macro's helper
		/// functions not its core functionality.
		void pythonError(Macro*);

};//class Macro

#endif
