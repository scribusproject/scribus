#ifndef OBJPRINTER_H
#define OBJPRINTER_H

// Pulls in <Python.h> first
#include "cmdvar.h"

PyAPI_DATA(PyTypeObject) Printer_Type;

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
