#ifndef IMPORTSVG_H
#define IMPORTSVG_H

// Brings in <Python.h> first
#include "cmdvar.h"

PyDoc_STRVAR(scribus_importsvg__doc__,
QT_TR_NOOP("importSVG(\"string\")\n\
\n\
The \"string\" must be a valid filename for a SVG image. The text\n\
must be UTF8 encoded or 'unicode' string(recommended).\n\
"));
/**
Imports a SVG image file.
(Henning Schroeder 2005-06-29)
*/
PyObject *scribus_importsvg(PyObject* /*self*/, PyObject* args);


#endif
