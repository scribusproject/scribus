#include "extmacro.h"
#include "cmdutil.h"

#include "macro.h"
#include "macromanager.h"

// These Python functions provide access to some of the facilities of
// MacroManager.

PyObject* register_macro_callable(PyObject* /*self*/, PyObject* args, PyObject* kw)
{
	char* name = NULL;
	PyObject* callable = NULL;
	char* shortcut = "";
	static char* kwnames[] = {"name", "callable", "shortcut", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "esO|es", kwnames, "utf-8", &name, &callable, "utf-8", &shortcut))
		return NULL;

	/* DISABLED until MenuTest refactor
	   if (!extFunctionOk())
	   return NULL;
	   */

	QString utf_name = QString::fromUtf8(name);
	// get access to the macro manager
	MacroManager* mm = MacroManager::instance();
	// make a new macro
	if (!mm->newMacro(utf_name, QString::fromUtf8(shortcut)))
	{
		// MacroManager::newMacro failed. That's pretty odd.
		PyErr_SetString(ScribusException, QString("MacroManager::newMacro failed"));
		return NULL;
	}

	// Ok, now give the macro the callable we were passed
	if (!mm->setCallable(utf_name, callable))
	{
		// setCallable failed. While it has cleared any Python exception
		// it may have encountered, it kept the details and we can retrive them.
		PyObject* excType = mm->exceptionPyType(utf_name);
		QString exc_text = mm->exceptionText(utf_name);
		// Now that we've grabbed the exception info, dispose of the macro
		mm->deleteMacro(utf_name);
		// Raise an exception with the same type and the text based on the old one
		PyErr_SetString(excType, QString("Macro creation failed: %1").arg(exc_text));
		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}


PyObject* register_macro_code(PyObject* /*self*/, PyObject* args, PyObject* kw)
{
	char* name = NULL;
	char* sourceCode = NULL;
	char* shortcut = "";
	static char* kwnames[] = {"name", "sourcecode", "shortcut", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "eses|es", kwnames, "utf-8", &name, "utf-8", &sourceCode, "utf-8", &shortcut))
		return NULL;

	/* DISABLED until MenuTest refactor
	   if (!extFunctionOk())
	   return NULL;
	   */

	QString utf_name = QString::fromUtf8(name);
	// get access to the macro manager
	MacroManager* mm = MacroManager::instance();
	// make a new macro
	if (!mm->newMacro(utf_name, QString::fromUtf8(shortcut)))
	{
		// MacroManager::newMacro failed. That's pretty odd.
		PyErr_SetString(ScribusException, QString("MacroManager::newMacro failed"));
		return NULL;
	}

	// Ok, now give the macro the callable we were passed
	if (!mm->setSource(utf_name, sourceCode))
	{
		// setSource failed. While it has cleared any Python exception
		// it may have encountered, it kept the details and we can retrive them.
		PyObject* excType = mm->exceptionPyType(utf_name);
		QString exc_text = mm->exceptionText(utf_name);
		// Now that we've grabbed the exception info, dispose of the macro
		mm->deleteMacro(utf_name);
		// Raise an exception with the same type and the text based on the old one
		PyErr_SetString(excType, QString("Macro creation failed: %1").arg(exc_text));
		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}


PyObject* unregister_macro(PyObject* /*self*/, PyObject* args, PyObject* kw)
{
	char* name = NULL;
	static char* kwnames[] = {"name", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "es", kwnames, "utf-8", &name))
		return NULL;

	/* DISABLED until MenuTest refactor
	   if (!extFunctionOk())
	   return NULL;
	   */

	MacroManager* mm = MacroManager::instance();
	mm->deleteMacro(name);

	// If there are no more macros, unload the macro manager
	// Disabled for now, until we can access MenuTest and find out if the user
	// wants to keep it around or not.
	if (mm->macrosRegistered() == 0)
	    MacroManager::deleteInstance();

	Py_INCREF(Py_None);
	return Py_None;
}
