#ifndef CMDVAR_H
#define CMDVAR_H

#if defined(_XOPEN_SOURCE)
#undef _XOPEN_SOURCE
#endif

#if defined(HAVE_BOOST_PYTHON)
#include <boost/python.hpp>
#else
#include <Python.h>
#endif

#include <qstring.h>

#include "scribus.h"

class ScripterCore;

// Globals for testing Qt properties and probably other more intresting future
// uses.
/** @brief A PyCObject containing a pointer to qApp */
extern PyObject* wrappedQApp;
/** @brief A PyCObject containing a pointer to the main window ('Carrier') */
extern PyObject* wrappedMainWindow;

/** @brief A pointer to the ScripterCore instance */
extern ScripterCore* scripterCore;

/** @brief Initialize the 'scribus' Python module in the currently active interpreter */
extern "C" void initscribus(ScribusMainWindow *pl);

/** @brief String representation of result returned by last python console command */
extern QString RetString;
/** @brief String representation of line of code to be passed to the Python interactive interpreter */
extern QString InValue;
/** @brief Wish I knew, too... */
extern int RetVal;

/* Exceptions */
/*! Common scribus Exception */
extern PyObject* ScribusException;
/*! Exception raised when no document opened - see checkHaveDocument() in cmdutil.cpp */
extern PyObject* NoDocOpenError;
/*! Exception raised when an operation is performed on a frame type that doesn't support it.*/
extern PyObject* WrongFrameTypeError;
/*! Exception raised by GetUniqueItem when it can't find a valid frame or a suitable selection to use. */
extern PyObject* NoValidObjectError;
/*! A general exception for when objects such as colors and fonts cannot be found. */
extern PyObject* NotFoundError;
/*! Exception raised when the user tries to create an object with the same name as one that already exists */
extern PyObject* NameExistsError;

#endif

