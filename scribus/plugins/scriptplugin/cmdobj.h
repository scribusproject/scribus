#ifndef CMDOBJ_H
#define CMDOBJ_H


/** Object related Commands */
PyObject *scribus_newrect(PyObject *self, PyObject* args);
PyObject *scribus_newellipse(PyObject *self, PyObject* args);
PyObject *scribus_newimage(PyObject *self, PyObject* args);
PyObject *scribus_newtext(PyObject *self, PyObject* args);
PyObject *scribus_newline(PyObject *self, PyObject* args);
PyObject *scribus_polyline(PyObject *self, PyObject* args);
PyObject *scribus_polygon(PyObject *self, PyObject* args);
PyObject *scribus_bezierline(PyObject *self, PyObject* args);
PyObject *scribus_pathtext(PyObject *self, PyObject* args);
PyObject *scribus_deleteobj(PyObject *self, PyObject* args);
/**
Enables/disables "Text Flows Around Box" feature for object.
Called with params string objectName and voluntary 1|0.
When 1 set flowing to true (0 to false). When is second param
empty flowing is reverted.
02/28/2004 petr vanek
 */
PyObject *scribus_textflow(PyObject *self, PyObject* args);

#endif

