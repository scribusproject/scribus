#ifndef CMDCOLOR_H
#define CMDCOLOR_H


/** Managing Colors */

/** Returns a list with colours available in doc or in prefs. */
PyObject *scribus_colornames(PyObject *self);
/** Returns a CMYK tuple of the specified color. */
PyObject *scribus_getcolor(PyObject *self, PyObject* args);
/** Sets named color with C,M,Y,K params. */
PyObject *scribus_setcolor(PyObject *self, PyObject* args);
/** Creates new color with name, C, M, Y, K params. */
PyObject *scribus_newcolor(PyObject *self, PyObject* args);
/** Deletes named color */
PyObject *scribus_delcolor(PyObject *self, PyObject* args);
/** Replaces color with the 2nd one. */
PyObject *scribus_replcolor(PyObject *self, PyObject* args);

#endif

