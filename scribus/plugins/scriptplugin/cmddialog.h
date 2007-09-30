/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDDIALOG_H
#define CMDDIALOG_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Calling Dialogs from Scribus */

/*! docstring */
PyDoc_STRVAR(scribus_newdocdia__doc__,
QT_TR_NOOP("newDocDialog() -> bool\n\
\n\
Displays the \"New Document\" dialog box. Creates a new document if the user\n\
accepts the settings. Does not create a document if the user presses cancel.\n\
Returns true if a new document was created.\n\
"));
/** Raises the Scribus New Document dialog */
PyObject *scribus_newdocdia(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_filedia__doc__,
QT_TR_NOOP("fileDialog(\"caption\", [\"filter\", \"defaultname\", haspreview, issave, isdir]) -> string with filename\n\
\n\
Shows a File Open dialog box with the caption \"caption\". Files are filtered\n\
with the filter string \"filter\". A default filename or file path can also\n\
supplied, leave this string empty when you don't want to use it.  A value of\n\
True for haspreview enables a small preview widget in the FileSelect box.  When\n\
the issave parameter is set to True the dialog acts like a \"Save As\" dialog\n\
otherwise it acts like a \"File Open Dialog\". When the isdir parameter is True\n\
the dialog shows and returns only directories. The default for all of the\n\
optional parameters is False.\n\
\n\
The filter, if specified, takes the form 'comment (*.type *.type2 ...)'.\n\
For example 'Images (*.png *.xpm *.jpg)'.\n\
\n\
Refer to the Qt-Documentation for QFileDialog for details on filters.\n\
\n\
Example: fileDialog('Open input', 'CSV files (*.csv)')\n\
Example: fileDialog('Save report', defaultname='report.txt', issave=True)\n\
"));
/** Raises file dialog.
 Params - caption, filter, default name and opt. pre, mode. */
PyObject *scribus_filedia(PyObject * /*self*/, PyObject* args, PyObject* kw);
/* duplicity Sends a string into the Message Bar
PyObject *scribus_mess(PyObject *self, PyObject* args);
*/

/*! docstring */
PyDoc_STRVAR(scribus_messdia__doc__,
QT_TR_NOOP("messageBox(\"caption\", \"message\",\n\
    icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT,\n\
    button2=BUTTON_NONE, button3=BUTTON_NONE) -> integer\n\
\n\
Displays a message box with the title \"caption\", the message \"message\", and\n\
an icon \"icon\" and up to 3 buttons. By default no icon is used and a single\n\
button, OK, is displayed. Only the caption and message arguments are required,\n\
though setting an icon and appropriate button(s) is strongly\n\
recommended. The message text may contain simple HTML-like markup.\n\
\n\
Returns the number of the button the user pressed. Button numbers start\n\
at 1.\n\
\n\
For the icon and the button parameters there are predefined constants available\n\
with the same names as in the Qt Documentation. These are the BUTTON_* and\n\
ICON_* constants defined in the module. There are also two extra constants that\n\
can be binary-ORed with button constants:\n\
    BUTTONOPT_DEFAULT   Pressing enter presses this button.\n\
    BUTTONOPT_ESCAPE    Pressing escape presses this button.\n\
\n\
Usage examples:\n\
result = messageBox('Script failed',\n\
                    'This script only works when you have a text frame selected.',\n\
                    ICON_ERROR)\n\
result = messageBox('Monkeys!', 'Something went ook! <i>Was it a monkey?</i>',\n\
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,\n\
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)\n\
\n\
Defined button and icon constants:\n\
BUTTON_NONE, BUTTON_ABORT, BUTTON_CANCEL, BUTTON_IGNORE, BUTTON_NO,\n\
BUTTON_NOALL, BUTTON_OK, BUTTON_RETRY, BUTTON_YES, BUTTON_YESALL,\n\
ICON_NONE, ICON_INFORMATION, ICON_WARNING, ICON_CRITICAL.\n\
"));
/** Displays a message box with - caption, message, icon, button
 and two more buttons optional. */
PyObject *scribus_messdia(PyObject * /*self*/, PyObject* args, PyObject* kw);

/*! docstring */
PyDoc_STRVAR(scribus_valdialog__doc__,
QT_TR_NOOP("valueDialog(caption, message [,defaultvalue]) -> string\n\
\n\
Shows the common 'Ask for string' dialog and returns its value as a string\n\
Parameters: window title, text in the window and optional 'default' value.\n\
\n\
Example: valueDialog('title', 'text in the window', 'optional')\n\
"));
/* 09/24/2004 petr vanek */
PyObject *scribus_valdialog(PyObject * /*self*/, PyObject* args);


PyDoc_STRVAR(scribus_newstyledialog__doc__,
QT_TR_NOOP("newStyleDialog() -> string\n\
\n\
Shows 'Create new paragraph style' dialog. Function returns real\n\
style name or None when user cancels the dialog.\n\
"));
/* 09/24/2004 petr vanek */
PyObject *scribus_newstyledialog(PyObject * /*self*/, PyObject* args);

#endif
