#ifndef CMDCOLOR_H
#define CMDCOLOR_H


/** Managing Colors */
PyObject *scribus_colornames(PyObject *self, PyObject* args);
PyObject *scribus_getcolor(PyObject *self, PyObject* args);
PyObject *scribus_setcolor(PyObject *self, PyObject* args);
PyObject *scribus_newcolor(PyObject *self, PyObject* args);
PyObject *scribus_delcolor(PyObject *self, PyObject* args);
PyObject *scribus_replcolor(PyObject *self, PyObject* args);

#endif

