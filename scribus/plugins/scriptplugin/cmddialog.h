#ifndef CMDDIALOG_H
#define CMDDIALOG_H


/** Calling Dialogs from Scribus */
/** Raises the Scribus New Document dialog */
PyObject *scribus_newdocdia(PyObject *self, PyObject* args);
/** Raises file dialog.
 Params - caption, filter, default name and opt. pre, mode. */
PyObject *scribus_filedia(PyObject *self, PyObject* args);
/* diplicity Sends a string into the Message Bar
PyObject *scribus_mess(PyObject *self, PyObject* args);
*/
/** Displays a message box with - caption, message, icon, button
 and two more buttons optional. */
PyObject *scribus_messdia(PyObject *self, PyObject* args);
/** Raises the common 'Ask for string' dialog and returns its value
params: window title, text in the window and optional 'default value.
ValueDialog('title', 'text in the window', 'optional')
09/24/2004 petr vanek */
PyObject *scribus_valdialog(PyObject *self, PyObject* args);

#endif

