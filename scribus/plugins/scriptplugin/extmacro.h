#ifndef EXTMACRO_H
#define EXTMACRO_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/*! \file extmacro.h
  \brief Scripter functions related to \ref MacroManager

  \author Craig Ringer
  \date December 2004

  This file contains python functons used to register and unregister
  macros from the scripter.
  */

// These python functions provide access to some of the facilities
// of MacroManager.

/*! \brief A Python-callable wrapper around MacroManager::registerMacro()

  \return None
  */
PyDoc_STRVAR(register_macro_callable__doc__,
QT_TR_NOOP("register_macro_callable(name, callable, accel='')\n\
\n\
Create a macro called \"name\" with the existing callable object \"callable\".\n\
The passed callable must not require any arguments when called (it may take\n\
optional arguments, but will not be given any).\n\
If provided, the string \"accel\" will be used to set a keyboard shortcut\n\
for the macro.\n\
If the passed callable is a class, it will be rejected. Functions and bound\n\
methods are quite acceptable, as are instances of classes that provide a\n\
__call__ method with no arguments. There is no problem with registering\n\
a callable more than once, nor with registering multiple bound methods\n\
of a single instance.\n\
"));
PyObject* register_macro_callable(PyObject* /*self*/, PyObject* args, PyObject* kw);

/*! \brief A Python-callable wrapper around MacroManager::registerMacroCode()

  \return None
  */
PyDoc_STRVAR(register_macro_code__doc__,
QT_TR_NOOP("register_macro_code(name, sourcetext, accel='')\n\
\n\
Create a macro named \"name\" by evaluating the the source code \"sourcetext\".\n\
\"sourcetext\" must follow the same rules as macros created in the GUI.\n\
If provided, the string \"accel\" will be used to set a keyboard shortcut\n\
for the macro.\n\
"));
PyObject* register_macro_code(PyObject* /*self*/, PyObject* args, PyObject* kw);

/*! \brief A Python-callable wrapper around MacroManager::unregisterMacro()

  \return None
  */
PyObject* unregister_macro(PyObject* /*self*/, PyObject* args, PyObject* kw);

#endif
