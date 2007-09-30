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

/*! docstring */
PyDoc_STRVAR(scribus_placesvg__doc__,
QT_TR_NOOP("placeSVG(\"filename\", x, y)\n\
\n\
Places the SVG \"filename\" onto the current page,\n\
x and y specify the coordinate of the topleft corner of the SVG placed on the page\n\
\n\
If loading was successful, the selection contains the imported SVG\n\
"));
/*! Places an SVG file. */
PyObject *scribus_placesvg(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_placeeps__doc__,
QT_TR_NOOP("placeEPS(\"filename\", x, y)\n\
\n\
Places the EPS \"filename\" onto the current page,\n\
x and y specify the coordinate of the topleft corner of the EPS placed on the page\n\
\n\
If loading was successful, the selection contains the imported EPS\n\
"));
/*! Places an EPS file. */
PyObject *scribus_placeeps(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_placesxd__doc__,
QT_TR_NOOP("placeSXD(\"filename\", x, y)\n\
\n\
Places the SXD \"filename\" onto the current page,\n\
x and y specify the coordinate of the topleft corner of the SXD placed on the page\n\
\n\
If loading was successful, the selection contains the imported SXD\n\
"));
/*! Places an SXD file. */
PyObject *scribus_placesxd(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_placeodg__doc__,
QT_TR_NOOP("placeODG(\"filename\", x, y)\n\
\n\
Places the ODG \"filename\" onto the current page,\n\
x and y specify the coordinate of the topleft corner of the ODG placed on the page\n\
\n\
If loading was successful, the selection contains the imported ODG\n\
"));
/*! Places an ODG file. */
PyObject *scribus_placeodg(PyObject * /*self*/, PyObject* args);

#endif
