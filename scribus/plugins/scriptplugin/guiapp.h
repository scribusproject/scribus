#ifndef GUIAPP_H
#define GUIAPP_H

/**
Changes the status bar string.
TODO: national chars handling.
(Petr Vanek 02/19/04)
*/
PyObject *scribus_messagebartext(PyObject *self, PyObject* args);

/**
Progressbar handling
TODO: check total vs. set values.
(Petr Vanek 02/19/04)
*/
PyObject *scribus_progressreset(PyObject *self, PyObject* args);
PyObject *scribus_progresssettotalsteps(PyObject *self, PyObject* args);
PyObject *scribus_progresssetprogress(PyObject *self, PyObject* args);

/**
Cursor handling
(Petr Vanek 02/19/04)
*/
PyObject *scribus_setcursor(PyObject *self, PyObject* args);

#endif
