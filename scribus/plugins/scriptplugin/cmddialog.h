#ifndef CMDDIALOG_H
#define CMDDIALOG_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/** Calling Dialogs from Scribus */

/*! docstring */
PyDoc_STRVAR(scribus_newdocdia__doc__,
    "newDocDialog()\n\n\
    Shows the \"New Document\" Dialog Box. Returns true if\
    a new Document was created.");
/** Raises the Scribus New Document dialog */
PyObject *scribus_newdocdia(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_filedia__doc__,
    "fileDialog()\n\n\
    Shows a FileSelect Box with the Caption \"caption\". Files are\
    filtered with the Filter String \"filter\", refer to the\
    Qt-Documentation for it's use. A default Filename can also\
    supplied, leave this string empty when you don't want to use it.\
    A Value of 1 for preview enables a small Preview Widget in the\
    FileSelect Box. When the mode parameter is set to 1 the Dialog\
    acts like a \"Save As\" Dialog otherwise it acts like\
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
    "messageBox() -> integer\n\n\
    Shows a Message Box with the Title \"caption\", the Message \"message\",\
    and an Icon icon and up to 3 Buttons. Button1 is always needed. For\
    the icon and the Button Parameters there are predefined Constants\
    available with the same Names as in the Qt Documentation. Returns\
    the number of the selected Button.");
/** Displays a message box with - caption, message, icon, button
 and two more buttons optional. */
PyObject *scribus_messdia(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_valdialog__doc__,
    "valueDialog() -> string\n\n\
    TODO: docstring");
/** Raises the common 'Ask for string' dialog and returns its value
params: window title, text in the window and optional 'default value.
ValueDialog('title', 'text in the window', 'optional')
09/24/2004 petr vanek */
PyObject *scribus_valdialog(PyObject *self, PyObject* args);

#endif

