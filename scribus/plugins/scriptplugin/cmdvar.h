#ifndef CMDVAR_H
#define CMDVAR_H

#include <Python.h>
#include "scribus.h"

/* Static global Variables */
extern ScribusApp* Carrier;

/* Exceptions */
/*! Common scribus Exception */
extern PyObject* ScribusException;
/*! Exception raised when no document opened - see checkHaveDocument() in cmdutil.cpp */
extern PyObject* NoDocOpenError;

#endif

