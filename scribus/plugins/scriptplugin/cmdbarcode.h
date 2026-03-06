/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDBARCODE_H
#define CMDBARCODE_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Barcode related Commands */

/*! docstring */
PyDoc_STRVAR(scribus_createbarcode__doc__,
QT_TR_NOOP("createBarcode(\"encoder\", \"data\", \"options\", x, y, [\"name\"]) -> string\n\
\n\
Creates a new barcode on the current page and returns its name.\n\
\n\
\"encoder\" is the BWIPP encoder name, e.g. \"qrcode\", \"ean13\", \"code128\".\n\
 \"data\" is the barcode content string.\n\
 \"options\" is the BWIPP options string, e.g. \"eclevel=H includetext\".\n\
x and y are the position coordinates in the current measurement units.\n\
\"name\" is an optional unique identifier for the object.\n\
\n\
Requires Ghostscript to be available.\n\
\n\
May raise ScribusError if Ghostscript is not available or barcode\n\
generation fails.\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
"));
PyObject *scribus_createbarcode(PyObject * /*self*/, PyObject* args);

#endif
