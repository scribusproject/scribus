#ifndef CMDVAR_H
#define CMDVAR_H

#include <Python.h>
#include "scribus.h"

/* These will go away in 1.3, but help readability in 1.2.1 code a LOT */
#define FRAME_IMAGE 2
#define FRAME_TEXT 4
#define FRAME_LINE 5
#define FRAME_VECTOR 6
#define FRAME_POLYLINE 7
#define FRAME_PATHTEXT 8

/* Static global Variables */
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

#endif

