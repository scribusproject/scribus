#ifndef CMDGETPROP_H
#define CMDGETPROP_H


/** Query-Functions */
PyObject *scribus_getfillcolor(PyObject *self, PyObject* args);
PyObject *scribus_getlinecolor(PyObject *self, PyObject* args);
PyObject *scribus_getlinewidth(PyObject *self, PyObject* args);
PyObject *scribus_getlineshade(PyObject *self, PyObject* args);
PyObject *scribus_getlinejoin(PyObject *self, PyObject* args);
PyObject *scribus_getlineend(PyObject *self, PyObject* args);
PyObject *scribus_getlinestyle(PyObject *self, PyObject* args);
PyObject *scribus_getfillshade(PyObject *self, PyObject* args);
PyObject *scribus_getcornerrad(PyObject *self, PyObject* args);
PyObject *scribus_getimgscale(PyObject *self, PyObject* args);
PyObject *scribus_getimgname(PyObject *self, PyObject* args);
PyObject *scribus_getposi(PyObject *self, PyObject* args);
PyObject *scribus_getsize(PyObject *self, PyObject* args);
PyObject *scribus_getrotation(PyObject *self, PyObject* args);
PyObject *scribus_getallobj(PyObject *self, PyObject* args);

#endif

