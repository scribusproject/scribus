#ifndef CMDVAR_H
#define CMDVAR_H
/*
#ifdef HAVE_PYTHON_23
	#include <Python.h>
#endif          */

#include "scribus.h"

#define ERRPARAM QObject::tr("Oook! Wrong arguments! Call: ")

/* Static global Variables */
extern ScribusApp* Carrier;

/* Exceptions */
/*! Common scribus Exception */
extern PyObject* ScribusException;
/*! Exception raised when no document opened - see checkHaveDocument() in cmdutil.cpp */
extern PyObject* NoDocOpenError;

#endif

