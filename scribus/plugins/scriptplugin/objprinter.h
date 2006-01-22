/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef OBJPRINTER_H
#define OBJPRINTER_H

// Pulls in <Python.h> first
#include "cmdvar.h"

extern PyTypeObject Printer_Type;

// Nest items are not needed but are here for me to exercise
// writing complete python objects

#define Printer_Check(op) ((op)->ob_type == &Printer_Type)

PyDoc_STRVAR(printer__doc__,"Printing\n\
\n\
Class Printer() provides printing for Python scripting.\n\
\n\
Example:\n\
p = Printer()\n\
p.print()");

#endif /* OBJPRINTER_H */
