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
/** 
returns a tuple with page domensions in used system
e.g. when is the doc in picas returns picas ;)
(Petr Vanek 02/17/04) 
*/
PyObject *scribus_pagedimension(PyObject *self, PyObject* args);
/**
returns a list of tuples with items on the actual page
TODO: solve utf/iso chars in object names
(Petr Vanek 03/02/2004)
*/
PyObject *scribus_getpageitems(PyObject *self, PyObject* args);

#endif

