#ifndef CMDMANI_H
#define CMDMANI_H


/** Manipulating Objects */
PyObject *scribus_moveobjrel(PyObject *self, PyObject* args);
PyObject *scribus_moveobjabs(PyObject *self, PyObject* args);
PyObject *scribus_rotobjrel(PyObject *self, PyObject* args);
PyObject *scribus_rotobjabs(PyObject *self, PyObject* args);
PyObject *scribus_sizeobjabs(PyObject *self, PyObject* args);
PyObject *scribus_getselobjnam(PyObject *self, PyObject* args);
PyObject *scribus_selcount(PyObject *self, PyObject* args);
PyObject *scribus_selectobj(PyObject *self, PyObject* args);
PyObject *scribus_deselect(PyObject *self, PyObject* args);
PyObject *scribus_groupobj(PyObject *self, PyObject* args);
PyObject *scribus_ungroupobj(PyObject *self, PyObject* args);
PyObject *scribus_scalegroup(PyObject *self, PyObject* args);
PyObject *scribus_loadimage(PyObject *self, PyObject* args);
PyObject *scribus_scaleimage(PyObject *self, PyObject* args);

#endif

