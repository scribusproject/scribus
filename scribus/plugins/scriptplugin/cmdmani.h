#ifndef CMDMANI_H
#define CMDMANI_H


/** Manipulating Objects */
PyObject *scribus_moveobjrel(PyObject *self, PyObject* args);
PyObject *scribus_moveobjabs(PyObject *self, PyObject* args);
PyObject *scribus_rotobjrel(PyObject *self, PyObject* args);
PyObject *scribus_rotobjabs(PyObject *self, PyObject* args);
PyObject *scribus_sizeobjabs(PyObject *self, PyObject* args);
PyObject *scribus_getselobjnam(PyObject *self, PyObject* args);
PyObject *scribus_selcount(PyObject *self);
PyObject *scribus_selectobj(PyObject *self, PyObject* args);
PyObject *scribus_deselect(PyObject *self);
PyObject *scribus_groupobj(PyObject *self, PyObject* args);
PyObject *scribus_ungroupobj(PyObject *self, PyObject* args);
PyObject *scribus_scalegroup(PyObject *self, PyObject* args);
PyObject *scribus_loadimage(PyObject *self, PyObject* args);
PyObject *scribus_scaleimage(PyObject *self, PyObject* args);
/** (Un)Lock the object
2004/7/10 pv.*/
PyObject *scribus_lockobject(PyObject *self, PyObject* args);
PyObject *scribus_islocked(PyObject *self, PyObject* args);
#endif

