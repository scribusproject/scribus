#ifndef CMDPAGE_H
#define CMDPAGE_H


/** Page related Commands */
PyObject *scribus_newpage(PyObject *self, PyObject* args);
PyObject *scribus_actualpage(PyObject *self, PyObject* args);
PyObject *scribus_redraw(PyObject *self, PyObject* args);
PyObject *scribus_savepageeps(PyObject *self, PyObject* args);
PyObject *scribus_deletepage(PyObject *self, PyObject* args);
PyObject *scribus_gotopage(PyObject *self, PyObject* args);
PyObject *scribus_pagecount(PyObject *self, PyObject* args);

#endif

