#ifndef CMDDOC_H
#define CMDDOC_H


/**	Document related Commands */
/** Creates a new document e.g. (Paper_A4, Margins, 1, 1, 1, NoFacingPages, FirstPageLeft)
 first 2 args are lists (tuples) */
PyObject *scribus_newdoc(PyObject *self, PyObject* args);
/** Closes active doc. No params */
PyObject *scribus_closedoc(PyObject *self, PyObject* args);
/** Checks if is a document opened. */
PyObject *scribus_havedoc(PyObject *self, PyObject* args);
/** Opens a document with given name. */
PyObject *scribus_opendoc(PyObject *self, PyObject* args);
/** Saves active document (only save slot call). */
PyObject *scribus_savedoc(PyObject *self, PyObject* args);
/** Saves active document with given name */
PyObject *scribus_savedocas(PyObject *self, PyObject* args);
/** Sets document infos - author, title and description */
PyObject *scribus_setinfo(PyObject *self, PyObject* args);
/** Sets document margins - left, right, top and bottom. */
PyObject *scribus_setmargins(PyObject *self, PyObject* args);
/** Changes unit scale. */
PyObject *scribus_setunit(PyObject *self, PyObject* args);
/** Returns actual unit scale. */
PyObject *scribus_getunit(PyObject *self, PyObject* args);
/** Loads styles from another .sla file (craig r.)*/
PyObject *scribus_loadstylesfromfile(PyObject *self, PyObject *args);
PyObject *scribus_setdoctype(PyObject *self, PyObject* args);

#endif

