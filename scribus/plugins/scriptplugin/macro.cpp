/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "macro.h"
#include "cmdutil.h"

// This file is documented in macro.h; please refer to
// that or the doxygen API docs generated from it for
// information about the classes and functions defined here.

/*
   Implementation notes

   It is important that Macro have no, or extremely minimal, knowledge of
   MacroManager.  MacroManager must hold the only references to macro
   instances.

   Macro sends signals with a pointer to its self. These are intended for use
   by MacroManager; nobody else should be using them (or able to see macro
   instances). MacroManager will receive them and emit a signal with the macro
   name and changed properties for others to handle, so anybody with an
   interest in a macro should connect to MacroManager. The first argument
   of any signal from MacroManager is always the name of the macro it relates
   to, so if you only care about some macros you can filter the signals
   easily.

   Macro contains all the Python/C API use; MacroManager should not need
   to be concerned with Python data types or exceptions. At most it should
   be exposed to PyObject*.

   MacroManager and Macro communicate using signals to the greatest
   extent possible. This has proved to be the simplest way to handle
   a number of problems, so it's probaby best to stick to this model
   with future additions.

   If a macro instance has its QAction deleted, it will respond by deleting its
   self. It's recommended that you only store Macros using guarded pointers.

   Currently, not all the QAction's properties are touched. For example,
   the descriptive text is ignored.
   */



// Must not be called with an exception set
// Must not return with an exception set
Macro::Macro(QString newName, QString accel)
{
	assert(!PyErr_Occurred());

	if ((newName == QString::null) or (newName == ""))
		qDebug("Macro::Macro() called with NULL or '' name");

	// set the macro name
	this->macroNameVar = newName;
	// maintain our QObject name too
	this->setName(newName);
	// sanity check this?
	this->accelSeq = accel;

	// Set a clean exception state
	this->excPyType = NULL;
	this->excType = QString::null;
	this->excText = QString::null;
	this->excTraceback = QString::null;
	this->excOccurred = false;

	// Initially macros have no callable and no source
	this->sourceCode = QString::null;
	this->callable = NULL;
	this->docString = QString::null;

	// Create a QAction that can be added to whatever user
	// interface elements someone wants to hook us up to.
	this->actionObj = new QAction(newName, accel, Carrier, 
		QString("macro action - '%1'").arg(newName));
	// and connect it to our execute() slot, so that when it
	// gets triggered, we run.
	connect(this->actionObj, SIGNAL(activated()), SLOT(execute()) );
	// Delete ourselves when our action is deleted
	connect(this->actionObj, SIGNAL(destroyed()), SLOT(deleteLater()) );

	// We need to update the action when our description text
	// changes (we change the WhatsThis text) so connect ourselves
	// to our aboutTextChanged() signal.
	connect(this, SIGNAL(aboutTextChanged(Macro*)),
			SLOT(updateWhatsThis()) );

	assert(!PyErr_Occurred());
}


// Must not be called with an exception set
// Must not return with an exception set
Macro::~Macro()
{
	assert(!PyErr_Occurred());

	// Note: we check for null and use Py_DECREF rather than Py_XDECREF
	// because we WANT the immediate cleanup offered by Py_DECREF
	if (this->callable)
	{
		Py_DECREF(this->callable);
		this->callable = NULL;
	}

	// clean up our QAction. Doing so will remove it from
	// wherever it is referenced.
	if (this->actionObj)
		delete this->actionObj;
	this->actionObj = NULL;

	emit deleting(this);
	assert(!PyErr_Occurred());
}


// Must not be called with an exception set
// Must not return with an exception set
bool Macro::setCallable(PyObject* newCallable)
{
	assert(!PyErr_Occurred());
	assert(this->macroNameVar != QString::null);

	if (PyType_Check(newCallable))
	{
		// We've been passed a type object, not a function, instance, or bound
		// method. Types are callable, but not very useful to call since they
		// just return an instance. We accept types as source code because we
		// instantiate them, but don't accept them when passed as Python
		// objects.
		//
		// If you're in a position to call with an existing type object, you
		// can instantiate it yourself instead. It's most likely that passing a
		// type is just an error. We'll treat it as such.

		PyErr_SetString(PyExc_TypeError,
			"Passed object is a type not an instance, bound method, or function.");
		setExceptionState();
		emit newCallableError(this);
		return false;
	}

	// The passed object looks reasonable, so call our helper to change everything
	// over to it. Note that setMacroAndSource handles all exception state
	// updates, but does not emit a signal on failure.
	if (!setMacroCallableAndSource(newCallable, QString::null))
	{
		// DO NOT call setExceptionState; setMacroCallableAndSource has done
		// that already
		assert(!PyErr_Occurred());
		emit newCallableError(this);
		return false;
	}
	else
	{
		// DO NOT call clearExceptionState; setMacroCallableAndSource has done
		// that already
		assert(!PyErr_Occurred());
		return true;
	}
}


// Must not be called with an exception set
// Must not return with an exception set
bool Macro::setSource(QString newSourceCode)
{
	assert(!PyErr_Occurred());
	assert(this->macroNameVar != QString::null);

	bool result = true;
	PyObject* newCallable = NULL;

	// Make sure the passed object is not null, evaluate it to a callable,
	// and report any errors found in the process.
	if (newSourceCode == QString::null)
	{
		PyErr_SetString(PyExc_ValueError,
			QString("macro %s: Passed source was NULL").arg(this->macroName()));
		result = false;
	}
	else
	{
		newCallable = this->compileCode(newSourceCode); // returns new reference
		if (newCallable == NULL)
			// Evaluating the new code failed. A Python exception is already set
			result = false;
	}
	if (!result)
	{
		setExceptionState();
		emit newSourceError(this);
		return false;
	}

	// Evaluating the new code worked, so call our helper to change everything
	// over to it. Note that setMacroAndSource handles all exception state
	// updates but does not emit a signal on failure.
	result = setMacroCallableAndSource(newCallable, newSourceCode);
	assert(!PyErr_Occurred()); // even if something went wrong, it should've been handled inside

	// setMacroCallableAndSource has taken a reference to the new callable
	// object so we release ours.
	Py_DECREF(newCallable);

	if (!result)
	{
		// DO NOT call setExceptionState; setMacroCallableAndSource has done it
		// for us
		emit newSourceError(this);
		return false;
	}
	else
	{
		// DO NOT call clearExceptionState(); setMacroCallableAndSource has
		// done it for us.
		return true;
	}
}


// Must not be called with an exception set
// Must not return with an exception set
bool Macro::setMacroCallableAndSource(PyObject* newCallable, QString newSourceCode)
{
	// Sanity check - there must not be any exception condition when this function
	// is entered.
	assert(!PyErr_Occurred());

	// this method is only for internal use; it contains
	// the common functionality of setSource and
	// setCallable .

	if (!PyCallable_Check(newCallable))
	{
		PyErr_SetString(PyExc_TypeError,
				tr("Passed object is not callable","python error"));
		setExceptionState();
		return false;
	}

	// release the reference to the old callable, if any
	Py_XDECREF(this->callable);
	// then store the new one and grab a reference to it
	this->callable = newCallable;
	Py_INCREF(this->callable);

	// update our source code to match
	this->sourceCode = newSourceCode;

	// and get the docstring from the callable, if possible
	this->docString = this->getDocString();

	// make sure there's no info about old exceptions hanging around
	clearExceptionState();

	// then tell whoever's listening about the changes.
	emit callableChanged(this);
	emit sourceChanged(this);
	emit aboutTextChanged(this);

	// and return a success code
	return true;
}


QString Macro::accel()
{
	return this->accelSeq;
}


void Macro::setAccel(QString newAccel)
{
	this->accelSeq = newAccel;
	this->action()->setAccel(newAccel);
	emit accelChanged(this);
}


QAction* Macro::action()
{
	return this->actionObj;
}


QString Macro::aboutText()
{
	return this->docString;
}


QString Macro::source(bool repr)
{
	if (repr)
		return reprString(this->sourceCode);
	else
		return this->sourceCode;
}


QString Macro::macroName()
{
	return this->macroNameVar;
}


bool Macro::exceptionOccurred()
{
	return this->excOccurred;
}


QString Macro::exceptionType()
{
	return this->excType;
}


QString Macro::exceptionText()
{
	return this->excText;
}


QString Macro::exceptionTraceback()
{
	return this->excTraceback;
}

PyObject* Macro::exceptionPyType()
{
	return this->excPyType;
}


// Must not be called with an exception set
// Must not return with an exception set
void Macro::setMacroName(QString newName)
{
	if ((newName == QString::null) or (newName == ""))
	{
		qDebug("Macro::setMacroName(): called with NULL or '' name");
		return;
	}

	QString oldName = this->macroNameVar;
	this->macroNameVar = newName;
	// may as well also set our QObject name
	this->setName(newName);
	// update the text and name of our managed QAction too
	this->action()->setMenuText(newName);
	this->action()->setName(QString("macro action - '%1'").arg(newName));
	// tell anybody who's listening about the change
	emit nameChanged(this, oldName);
	// sanity check - there should not be any python exception set
	assert(!PyErr_Occurred());
}


void Macro::updateWhatsThis()
{
	this->action()->setWhatsThis(this->aboutText());
}


// Must not be called with an exception set
// Must not return with an exception set
void Macro::execute()
{
	assert(!PyErr_Occurred());

	// complain and exit if we have no callable
	if (this->callable == NULL)
	{
		// Perhaps we can notify the calling code better than this
		// now that we have our exception state and signalling code?
		qDebug("Macro::excecute(): '%s': called on a macro with no callable.", 
				(const char*)(this->macroName().utf8()));
		assert(!PyErr_Occurred());
		return;
	}

	// run our stored callable
	PyObject* result = PyObject_CallObject(this->callable, NULL);
	// and handle any exception it may have set
	if (result == NULL)
	{
		assert(PyErr_Occurred());
		if (PyErr_ExceptionMatches(PyExc_SystemExit))
		{
			// ignore the SystemExit exception
			PyErr_Clear();
			clearExceptionState();
		}
		else
		{
			setExceptionState();
			emit executionError(this);
		}
	}
	else
		clearExceptionState();

	Py_XDECREF(result);
	assert(!PyErr_Occurred());
	return;
}


// Must not be called with an exception set
// Must not return with an exception set
void Macro::clearExceptionState()
{
	assert(!PyErr_Occurred());

	// Clear any stored exception information - the last operation worked
	this->excOccurred = false;
	this->excType = QString::null;
	this->excText = QString::null;
	this->excTraceback = QString::null;
	Py_XDECREF(this->excPyType);
	this->excPyType = NULL;
	return;
}


// Must be called with an exception set
// Must not return with an exception set
void Macro::setExceptionState()
{
	assert(PyErr_Occurred());

	// Update our exception state. Note that formatCurrentException will steal
	// a reference to excPyType (even if it fails) so we don't need to DECREF
	// it.
	this->excOccurred = true;
	if (formatCurrentException(this->excPyType, this->excType, this->excText, this->excTraceback))
	{
		// formatCurrentException will have restored the old exception like
		// it's meant to.  In this case, we don't need it to be set anymore and
		// can safely clear it.
		assert(PyErr_Occurred());
		PyErr_Clear();
	}
	else
	{
		// formatting the error its self failed. That's bad.
		qWarning(
			"Macro::setExceptionState(): '%s': Formatting exception failed. "
			"Formatting error is:",
			(const char*)(this->macroName().utf8())
			);
		PyErr_Print();
		// Note: formatCurrentException has set our exception state to
		// reflect the fact that something went wrong but we couldn't
		// report it.
	}

	// sanity checks for exception state
	assert(!PyErr_Occurred());
	assert(this->excPyType);
	assert(this->excType != QString::null);
	assert(this->excText != QString::null);

	return;
}

/*  See the doxygen docs in the .h file for an overview of what this function
	does. In terms of implementation, the following Python code block is
	functionally the same as the C function below, though the C does its error
	checking differently and combines the compile and eval steps.

	Basically, we run the passed code snippet, which should define a single
	name (a callable object, such as a class defining __call__ or a function).
	Remember that 'def' and 'class' essentially assign the created class or
	function object to a name in the local namespace. For example, these
	two statements are equivalent:

	def f(x):
	return x

	f = lambda x: x

	(well, the lambda form doesn't set f.__name__, but we don't care about that
	here).

	We make sure there's only one object in the locals dict when the code
	snippet finishes, grab it, make sure it's a callable, and return it after
	storing its source code as an attribute of its self.

	def compileMacro(macroCode):
	compiledCode = compile(macroCode, 'macro', 'single')
	localDict = {}
	eval(compiledCode, globals(), localDict)
	assert(len(localDict) == 1)
	returned_func = localDict.values()[0]
	del(localDict)
	assert(callable(returned_func))
	assert(not isinstance(callable,type))
	returned_func.__source__ = macroCode
	return returned_func

	Tempted to write the fucker in Python and just extract the result?
	me too... but in the end this works out just as well.
	*/
// Must not be called with an exception set
// May set a Python exception on return
PyObject* Macro::compileCode(QString sourceCode)
{
	assert(!PyErr_Occurred());

	PyObject* localDict = NULL;
	PyObject* mainModule = NULL;
	PyObject* globalDict = NULL;
	PyObject* result = NULL;
	PyObject* localsList = NULL;
	PyObject* callable = NULL;
	PyObject* type = NULL;
	PyCompilerFlags* flags = NULL;

	// Create a new, empty dict for the local scope of the executed code
	//      localDict = {}
	localDict = PyDict_New();
	if (localDict == NULL)
		goto error;

	// Get __main__.__dict__ for globals
	//      globalDict = globals()
	mainModule = PyImport_AddModule(const_cast<char*>("__main__")); // BORROWED ref
	if (mainModule == NULL)
		goto error;
	globalDict = PyModule_GetDict(mainModule); // BORROWED ref
	if (globalDict == NULL)
		goto error;
	// we're done with mainModule, so mark it so. DO NOT DECREF it because
	// AddModule returns a borrowed reference. [insert swearing at Python/C API]
	mainModule = NULL;
	// Take ownership of the borrowed reference to globalDict.
	Py_INCREF(globalDict);

	// Run the code with the local and global namespaces
	// set to the dictionaries we just obtained.
	//      func = compile(macroCode, 'macro', 'single')
	//      eval(func, globalDict, localDict)
	// Note that the division flag is set; this is the equivalent of running
	//      from __future__ import division
	// before executing the code.
	flags = new PyCompilerFlags;
	flags->cf_flags = CO_FUTURE_DIVISION;
	result = PyRun_StringFlags(sourceCode.utf8(), Py_single_input, globalDict, localDict, flags);
	delete flags;
	flags = NULL;
	if (result == NULL)
		goto error;

	// result should be None
	if (result != Py_None)
	{
		// No Python exception has been set. Set one, then jump to the normal cleanup code.
		// FIXME: right exception to use? Refcounts?
		PyErr_SetString(PyExc_Exception,
				QString("INTERNAL ERROR: Expected 'None' result from snippet, got %1")
				.arg( PyString_AsString(PyObject_Repr(result))) );
		goto error;
	}
	// we're done with the result, which is just None, now; get rid of it
	Py_DECREF(result);
	result = NULL;

	// The locals dict should have only one item - the newly defined callable - in it
	//      assert(len(localDict) == 1)
	if (PyDict_Size(localDict) != 1)
	{
		// No Python exception has been set. Set one before jumping to cleanup code.
		// FIXME: right exception to use? Refcounts?
		PyErr_SetString(PyExc_Exception,
				QString("INTERNAL ERROR: Locals should contain 1 item after macro eval; contained %1. Dict was: %2.")
				.arg(PyDict_Size(localDict))
				.arg(PyString_AsString(PyObject_Repr(localDict)))
				);
		goto error;
	}

	// Grab the single object out of the locals dict
	//      callable = localDict.values()[0]
	localsList = PyMapping_Values(localDict);  // new ref
	if (localsList == NULL)
		goto error;
	callable = PySequence_GetItem(localsList, 0); // new ref
	if (callable == NULL)
		goto error;

	// We can clean up some of our working variables
	Py_DECREF(localDict);
	localDict = NULL;
	Py_DECREF(globalDict);
	globalDict = NULL;
	Py_DECREF(localsList);
	localsList = NULL;

	if (PyType_Check(callable))
	{
		// We've been passed the source code to a type - a new-style class -
		// not a function. Calling a type isn't very useful for a macro, you
		// just get an instance of the type. However, we can instantiate it
		// here and use the instance instead of the type when the macro is
		// called. The instance holds a reference to the type, so it's fine
		// to just discard the type after creating the instance.
		type = callable;
		callable = NULL;

		callable = PyObject_CallObject(type, NULL);  // new ref
		if (callable == NULL)
			goto error;

		// Ok, we're done with the type object, so get rid of it
		Py_DECREF(type);
		type = NULL;
	}

	// Make sure the returned object is actually a callable
	if (!PyCallable_Check(callable))
	{
		// FIXME: right exception? Refcounts?
		PyErr_SetString(PyExc_Exception,
				QString("Object created by macro was not a callable. Object was: %1.")
				.arg(PyString_AsString(PyObject_Repr(callable))));
		goto error;
	}

	// Add the source code we were passed as an attribute
	// on the callable object.
	this->sourceCode = sourceCode;

	// and return the callable. The reference to the returned
	// callable is not released, so the caller must accept
	// responsibility for its reference.

	assert(!PyErr_Occurred());
	return callable;

error:
	// roll back through the function releasing references
	// before returning NULL on error. It's safe to
	// unconditionally DECREF all of them because we use
	// XDECREF (which ignores NULLs) and make sure to set any
	// variables to NULL if they should not be DECREFd.
	Py_XDECREF(type);
	Py_XDECREF(callable);
	Py_XDECREF(localsList);
	Py_XDECREF(result);
	// NOTE: Do NOT decref mainModule because it's a borrowed reference.
	Py_XDECREF(localDict);
	// sanity check: ensure an exception is set. The calling code must
	// handle the exception.
	assert(PyErr_Occurred());
	return NULL;

}


// Must not be called with an exception set
// Must not return with an exception set
QString Macro::getDocString()
{
	assert(!PyErr_Occurred());

	// if there's no callable, return
	if (this->callable == NULL)
		return QString::null;

	// if the callable has no __doc__ attr, return.
	// This is unusual; usually "no doc string" actually
	// means __doc__ == None, not no __doc__ .
	if (!PyObject_HasAttrString(this->callable, const_cast<char*>("__doc__")))
	{
		assert(!PyErr_Occurred());
		return QString::null;
	}

	// grab the __doc__ attribute
	PyObject* docStringObj = PyObject_GetAttrString(this->callable, const_cast<char*>("__doc__"));
	if (docStringObj == NULL)
	{
		setExceptionState();
		emit pythonError(this);
		assert(!PyErr_Occurred());
		return QString::null;
	}

	if (docStringObj == Py_None)
	{
		// object has no docstring
		Py_DECREF(docStringObj);
		assert(!PyErr_Occurred());
		return QString::null;
	}

	// extract the text of the __doc__ object and return it.
	char* docString = PyString_AsString(docStringObj);
	Py_DECREF(docStringObj);
	docStringObj = NULL;
	if (docString == NULL)
	{
		setExceptionState();
		emit pythonError(this);
		assert(!PyErr_Occurred());
		return QString::null;
	}

	assert(!PyErr_Occurred());
	return QString::fromUtf8(docString);
}


// The guts of this, minus the error processing, should probably move to cmdutil.cpp later
// Must not be called with an exception set
// Must not return with an exception set
QString Macro::reprString(QString fromString)
{
	assert(!PyErr_Occurred());

	PyObject* fromStringObj = NULL;
	PyObject* reprString = NULL;
	char* reprText = NULL;
	int textLength = -1;
	QString reprQString = QString::null;//DEBUG

	// First, make a Python string from the input
	fromStringObj = PyString_FromString(fromString.utf8());
	if (fromStringObj == NULL)
		goto error;

	// then repr() it
	reprString = PyObject_Repr(fromStringObj);
	if (reprString == NULL)
		goto error;

	Py_DECREF(fromStringObj);

	// now get the text from the repr()d string. The returned  char* is a
	// pointer to the internal buffer of reprString, so if reprString is
	// deleted it'll go away.
	if (PyString_AsStringAndSize(reprString, &reprText, &textLength) == -1)
		goto error;

	clearExceptionState();
	assert(!PyErr_Occurred());
	reprQString = QString::fromUtf8(reprText);
	Py_DECREF(reprString); // Now that we have a _copy_ of the string we can DECREF it
	return reprQString;

error:
	// clean up
	Py_XDECREF(fromStringObj);
	Py_XDECREF(reprString);
	// tell anybody who's listening that something broke
	setExceptionState();
	emit pythonError(this);
	// and return
	assert(!PyErr_Occurred());
	return QString::null;
}


// This should probably move to cmdutil.cpp later It formats the currently set
// Python exception as three strings: The exception class, the exception text
// (like the last line of the traceback) and the whole traceback. The results
// are returned in the parameters.
// Returns true on success, false on failure (in which case the arguments will
// be set to QString::null). An exception will always be set when this code
// returns (if it fails, it sets its own exception or passes one on).  Note
// that the the exception is NOT cleared; the calling code must handle that.
//
// The currently set exception type will be DECREFd, if set, and the new
// exception type will be INCREFd. In other words, we steal a reference and
// return a new reference.

// Must be called with an exception set
// Must return with an exception set
bool Macro::formatCurrentException( PyObject*& typeObject, QString& exceptionType, QString& exceptionText, QString& tracebackText)
{
	PyObject* exc_type = NULL;
	PyObject* exc_value = NULL;
	PyObject* exc_traceback = NULL;
	PyObject* tracebackModule = NULL;
	PyObject* formattedExceptionType = NULL;
	PyObject* formatExceptionFunc = NULL;
	PyObject* formattedTraceback = NULL;
	PyObject* formattedException = NULL;
	PyObject* flattenedTraceback = NULL;
	char* tempTraceBackText = NULL;
	char* tempExcText = NULL;
	char* tempExceptionType = NULL;

	// Make sure there's an exception set
	if (!PyErr_Occurred())
	{
		PyErr_SetString(PyExc_Exception, "formatCurrentException called without an exception set. Well, here's your exception!");
		goto error;
	}

	// steal a reference to typeObject, if any
	Py_XDECREF(typeObject);
	// then set a new reference to the current exception, which must not be NULL
	typeObject = PyErr_Occurred();
	assert(typeObject);
	Py_INCREF(typeObject);

	// Grab the current exception. Doing so clears it.
	// sets args to new references.
	PyErr_Fetch(&exc_type, &exc_value, &exc_traceback);
	if (!exc_type)
	{
		qDebug("Macro::formatCurrentException(): '%s': What the fuck? PyErr_Occurred() was true, but PyErr_Fetch returned NULL for exc_type!",
				(const char*)(this->macroName().utf8()));
		qDebug("Macro::formatCurrentException(): '%s': Let's see if Python can tell us what happened:",
				(const char*)(this->macroName().utf8()));
		PyErr_Print();
		qFatal("Macro::formatCurrentException(): '%s': too confused to continue",
				(const char*)(this->macroName().utf8()));
	}
	assert(exc_type);
	assert(exc_value);
	// Note that PyErr_Fetch doesn't guarantee to return a traceback object.
	// traceback.format_exception can handle not having a traceback object
	// so that's OK.

	// format the exception type
	// FIXME: Is there a better way to do this?
	formattedExceptionType = PyObject_Repr(exc_type);
	if (formattedExceptionType == NULL)
		goto error;

	// and get the text out of the resulting PyObject
	tempExceptionType = PyString_AsString(formattedExceptionType);
	if (tempExceptionType == NULL)
		goto error;

	exceptionType = QString::fromUtf8(tempExceptionType);

	// we're done with the formatted exception type
	Py_DECREF(formattedExceptionType);
	formattedExceptionType = NULL;

	// Now we need to format the traceback and exception value, which
	// takes more than just calling repr()

	// Get the traceback module
	tracebackModule = PyImport_ImportModule(const_cast<char*>("traceback")); // new ref
	if (tracebackModule == NULL)
		goto error;

	// get traceback.format_exception
	formatExceptionFunc = PyObject_GetAttrString(tracebackModule, const_cast<char*>("format_exception")); // new ref
	if (formatExceptionFunc == NULL)
		goto error;

	Py_DECREF(tracebackModule);
	tracebackModule = NULL;

	// and call traceback.format_exception with it
	// exc_traceback may be NULL, in which case traceback.format_exception expects Py_None.
	formattedTraceback = PyObject_CallFunction(formatExceptionFunc, const_cast<char*>("OOO"),
			exc_type, exc_value,
			exc_traceback ? exc_traceback : Py_None); // returns new ref
	if (formattedTraceback == NULL)
		goto error;

	Py_DECREF(formatExceptionFunc);
	formatExceptionFunc = NULL;

	// The last item in the list will be the exception its self; get that string object
	formattedException = PySequence_GetItem(formattedTraceback, -1); // new ref
	if (formattedException == NULL)
		goto error;

	// Get the text of the exception its self out of the string object we just grabbed
	tempExcText = PyString_AsString(formattedException);
	if (tempExcText == NULL)
		goto error;
	exceptionText = QString::fromUtf8(tempExcText);
	tempExcText = NULL;

	Py_DECREF(formattedException);
	formattedException = NULL;

	// If we don't have a traceback object, there's no point in
	// getting the traceback string, it'll just be the same as the
	// formatted exception.
	if (exc_traceback)
	{
		// The formatted traceback is a sequence of strings, with newlines,
		// that can be concatinated to get the traceback text.  There's no C
		// interface to str.join, so we concatinate the results ourselves.
		// Hideously inefficiently, I might add, but nobody will care. Really.
		//
		// Python code equivalent:
		//
		//  count = len(formattedTraceback)
		//  flattenedTraceback = ""
		//  for i in range(count):
		//      flattenedTraceback += formattedTraceback[i]
		//
		flattenedTraceback = PyString_FromString("");
		if (flattenedTraceback == NULL)
			goto error;
		for (int i = 0; i < PySequence_Length(formattedTraceback); i ++)
			// This loop is a little light on error checking, but REALLY shouldn't fail
			PyString_ConcatAndDel(&flattenedTraceback, PySequence_GetItem(formattedTraceback, i));

		// Get the text out of the flattened traceback
		tempTraceBackText = PyString_AsString(flattenedTraceback);
		if (tempTraceBackText == NULL)
			goto error;

		// we're done with the contcatinated traceback string
		Py_DECREF(flattenedTraceback);
		flattenedTraceback = NULL;

		// set the caller's traceback variable to the resulting
		// traceback
		tracebackText = QString::fromUtf8(tempTraceBackText);
		tempTraceBackText = NULL;
	}
	else
		// There's no traceback, so clear the caller's traceback variable
		tracebackText = QString::null;

	// we're done with the list of strings that was returned by format_exception
	Py_DECREF(formattedTraceback);
	formattedTraceback = NULL;


	// Restore the exception that was set when we were called.
	// steals reference to args. Before calling it, we should
	// not have any exeption set. After, we should.
	assert(!PyErr_Occurred());
	PyErr_Restore(exc_type, exc_value, exc_traceback);

	// tracebackText may be QString::null; that's OK
	// but the rest of the caller's vars should all be set
	assert(exceptionType != QString::null);
	assert(exceptionText != QString::null);
	assert(typeObject);
	assert(PyErr_Occurred());
	return true;

error:
	// Sanity check - an exception MUST be set when we enter the error handler
	assert(PyErr_Occurred());

	// Clean up our working variables
	Py_XDECREF(formattedExceptionType);
	Py_XDECREF(flattenedTraceback);
	Py_XDECREF(formattedException);
	Py_XDECREF(formattedTraceback);
	Py_XDECREF(formatExceptionFunc);
	Py_XDECREF(tracebackModule);

	// We failed to format the exception, but we don't want to lose that information.
	// Temporarily save the new exception we've set and restore the exception that was
	// set when we were called. We can then tell Python to dump it to stderr and put our
	// new exception back in place.
	PyObject* temp_exc_type = NULL;
	PyObject* temp_exc_value = NULL;
	PyObject* temp_exc_traceback = NULL;
	PyErr_Fetch(&temp_exc_type, &temp_exc_value, &temp_exc_traceback);
	// note that PyErr_Restore steals references to these objects, so we don't
	// need to DECREF them
	PyErr_Restore(exc_type, exc_value, exc_traceback);
	exc_type = NULL;
	exc_value = NULL;
	exc_traceback = NULL;
	assert(PyErr_Occurred());
	qDebug("Macro::formatCurrentException(): '%s': Arrggh, failed to format exception. Printing it instead.",
			(const char*)(this->macroName().utf8()));
	PyErr_Print();
	// restore the exception our own failure set
	PyErr_Restore(temp_exc_type, temp_exc_value, temp_exc_traceback);
	temp_exc_type = NULL;
	temp_exc_value = NULL;
	temp_exc_traceback = NULL;

	// Set the caller's variables to reflect the fact that an exception was
	// set when we were called, but we couldn't format it.
	exceptionType = QString("SystemError");
	exceptionText = QString("SystemError: formatCurrentException failed.");
	tracebackText = QString::null;
	// We DECREF the typeObject, if set, because it either contains a
	// reference to the value it held when passed (which we promise to steal a
	// reference to before disposing of) or to the exception type object that
	// we assigned to it (which is about to be cleared). We then set it to
	// SystemError (and take a reference to it) to indicate our failure.
	Py_XDECREF(typeObject);
	typeObject = PyExc_SystemError;
	Py_INCREF(typeObject);
	// Sanity check - an exception MUST be set when we return
	assert(PyErr_Occurred());
	return false;
}
