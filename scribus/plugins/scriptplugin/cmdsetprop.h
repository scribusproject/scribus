#ifndef CMDSETPROP_H
#define CMDSETPROP_H

#include <Python.h>

/** Setting Object Properties */
PyObject *scribus_setgradfill(PyObject *self, PyObject* args);
PyObject *scribus_setfillcolor(PyObject *self, PyObject* args);
PyObject *scribus_setlinecolor(PyObject *self, PyObject* args);
PyObject *scribus_setlinewidth(PyObject *self, PyObject* args);
PyObject *scribus_setlineshade(PyObject *self, PyObject* args);
PyObject *scribus_setlinejoin(PyObject *self, PyObject* args);
PyObject *scribus_setlineend(PyObject *self, PyObject* args);
PyObject *scribus_setlinestyle(PyObject *self, PyObject* args);
PyObject *scribus_setfillshade(PyObject *self, PyObject* args);
PyObject *scribus_setcornerrad(PyObject *self, PyObject* args);
PyObject *scribus_setmultiline(PyObject *self, PyObject* args);

#endif

