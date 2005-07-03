/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef CMDVAR_H
#define CMDVAR_H

#if defined(_XOPEN_SOURCE)
#undef _XOPEN_SOURCE
#endif

#include <Python.h>
#include "scribus.h"

/* Python.h may or may not define CO_FUTURE_DIVISION, so if it doesn't
 * we need to include compile.h directly. */
#ifndef CO_FUTURE_DIVISION
#include "compile.h"
#endif

/* These will go away in 1.3, but help readability in 1.2.1 code a LOT */
#define FRAME_IMAGE 2
#define FRAME_TEXT 4
#define FRAME_LINE 5
#define FRAME_VECTOR 6
#define FRAME_POLYLINE 7
#define FRAME_PATHTEXT 8

// Foward declarations
class ScripterCore;

/* Static global Variables */
// the QApplication instance passed to the plugin setup
extern ScribusApp* Carrier;

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
/*! Raised when a normal script tries to run code reserved for extension scripts, or when extension scripts are disabled */
extern PyObject* AccessDeniedError;

// The following globals are used by the script console
// to handle input and output.
extern QString RetString;
extern QString InValue;
extern int RetVal;

/// A global pointer to the class containing most of the scripter's core workings
extern ScripterCore* scripterCore;

/// initscribus, the module init function, must be defined here because the
/// scriptercore needs to call it.

extern "C" void initscribus(ScribusApp *pl);

#endif

