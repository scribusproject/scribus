#ifndef CMDDOC_H
#define CMDDOC_H


/**	Document related Commands */
PyObject *scribus_newdoc(PyObject *self, PyObject* args);
PyObject *scribus_closedoc(PyObject *self, PyObject* args);
PyObject *scribus_havedoc(PyObject *self, PyObject* args);
PyObject *scribus_opendoc(PyObject *self, PyObject* args);
PyObject *scribus_savedoc(PyObject *self, PyObject* args);
PyObject *scribus_savedocas(PyObject *self, PyObject* args);
PyObject *scribus_setinfo(PyObject *self, PyObject* args);
PyObject *scribus_setmargins(PyObject *self, PyObject* args);
PyObject *scribus_setunit(PyObject *self, PyObject* args);
PyObject *scribus_getunit(PyObject *self, PyObject* args);

#endif

