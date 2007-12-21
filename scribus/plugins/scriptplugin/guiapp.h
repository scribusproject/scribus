/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef GUIAPP_H
#define GUIAPP_H

// Brings in <Python.h> first
#include "cmdvar.h"

/*! Scribus GUI tool */

/*! docstring */
PyDoc_STRVAR(scribus_messagebartext__doc__,
QT_TR_NOOP("messagebarText(\"string\")\n\
\n\
Writes the \"string\" into the Scribus message bar (status line). The text\n\
must be UTF8 encoded or 'unicode' string(recommended).\n\
"));
/**
Changes the status bar string.
(Petr Vanek 02/19/04)
*/
PyObject *scribus_messagebartext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_progressreset__doc__,
QT_TR_NOOP("progressReset()\n\
\n\
Cleans up the Scribus progress bar previous settings. It is called before the\n\
new progress bar use. See progressSet.\n\
"));
/*
Progressbar handling
(Petr Vanek 02/19/04)
*/
PyObject *scribus_progressreset(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_progresssettotalsteps__doc__,
QT_TR_NOOP("progressTotal(max)\n\
\n\
Sets the progress bar's maximum steps value to the specified number.\n\
See progressSet.\n\
"));
PyObject *scribus_progresssettotalsteps(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_progresssetprogress__doc__,
QT_TR_NOOP("progressSet(nr)\n\
\n\
Set the progress bar position to \"nr\", a value relative to the previously set\n\
progressTotal. The progress bar uses the concept of steps; you give it the\n\
total number of steps and the number of steps completed so far and it will\n\
display the percentage of steps that have been completed. You can specify the\n\
total number of steps with progressTotal(). The current number of steps is set\n\
with progressSet(). The progress bar can be rewound to the beginning with\n\
progressReset(). [based on info taken from Trolltech's Qt docs]\n\
"));
PyObject *scribus_progresssetprogress(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcursor__doc__,
QT_TR_NOOP("setCursor()\n\
\n\
[UNSUPPORTED!] This might break things, so steer clear for now.\n\
"));
/**
Cursor handling
(Petr Vanek 02/19/04)
*/
PyObject *scribus_setcursor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_docchanged__doc__,
QT_TR_NOOP("docChanged(bool)\n\
\n\
Enable/disable save icon in the Scribus icon bar and the Save menu item. It's\n\
useful to call this procedure when you're changing the document, because Scribus\n\
won't automatically notice when you change the document using a script.\n\
"));
/**
Enable/disable save icon
(Petr Vanek 02/20/04)
*/
PyObject *scribus_docchanged(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_zoomdocument__doc__,
QT_TR_NOOP("zoomDocument(double)\n\
\n\
Zoom the document in main GUI window. Actions have whole number\n\
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.\n\
"));
/**
Zooming the document
\author Petr Vanek <petr@yarpen.cz>
*/
PyObject *scribus_zoomdocument(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_scrolldocument__doc__,
QT_TR_NOOP("scrollDocument(x,y)\n\
\n\
Scroll the document in main GUI window by x and y.\n\
"));
/*
 * Gives the possibility to scroll the document.
 * 13.12.2007: Joachim Neu
 */
PyObject *scribus_scrolldocument(PyObject*, PyObject* args);

#endif
