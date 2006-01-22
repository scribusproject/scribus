/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
