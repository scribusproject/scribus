#ifndef CMDDIALOG_H
#define CMDDIALOG_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Calling Dialogs from Scribus */

/*! docstring */
PyDoc_STRVAR(scribus_newdocdia__doc__,
    "newDocDialog() -> bool\n\n\
Shows the \"New Document\" dialog box. Returns true if\
a new document was created.");
/** Raises the Scribus New Document dialog */
PyObject *scribus_newdocdia(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_filedia__doc__,
    "fileDialog(\"caption\", \"filter\", \"defaultName\" [,preview, mode]) -> string with filename\n\n\
Shows a FileSelect box with the caption \"caption\". Files are\
filtered with the filter string \"filter\", refer to the\
Qt-Documentation for it's use. A default filename can also\
supplied, leave this string empty when you don't want to use it.\
A Value of 1 for preview enables a small preview widget in the\
FileSelect box. When the mode parameter is set to 1 the dialog\
acts like a \"Save As\" dialog otherwise it acts like\
a \"File Open Dialog\". The default for both of the opional\
Parameters is 0.");
/** Raises file dialog.
 Params - caption, filter, default name and opt. pre, mode. */
PyObject *scribus_filedia(PyObject *self, PyObject* args);
/* diplicity Sends a string into the Message Bar
PyObject *scribus_mess(PyObject *self, PyObject* args);
*/

/*! docstring */
PyDoc_STRVAR(scribus_messdia__doc__,
    "messageBox(\"caption\", \"message\", icon, Button1 [, Button2, Button3]) -> integer (see constants below)\n\n\
Shows a message box with the title \"caption\", the message \"message\",\
and an icon \"icon\" and up to 3 Buttons. Button1 is always needed. For\
the icon and the button parameters there are predefined constants\
available with the same names as in the Qt Documentation. Returns\
the number of the selected button. Buttons and returning values:\
BUTTON_ABORT, BUTTON_CANCEL, BUTTON_IGNORE, BUTTON_NO, BUTTON_NONE,\
BUTTON_OK, BUTTON_RETRY, BUTTON_YES");
/** Displays a message box with - caption, message, icon, button
 and two more buttons optional. */
PyObject *scribus_messdia(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_valdialog__doc__,
    "valueDialog(caption, message [,defaultvalue]) -> string\n\n\
Shows the common 'Ask for string' dialog and returns its value as string\
Parameters: window title, text in the window and optional 'default' value.\
Example: valueDialog('title', 'text in the window', 'optional'");
/** Raises the common 'Ask for string' dialog and returns its value
params: window title, text in the window and optional 'default value.
ValueDialog('title', 'text in the window', 'optional')
09/24/2004 petr vanek */
PyObject *scribus_valdialog(PyObject *self, PyObject* args);

#endif

