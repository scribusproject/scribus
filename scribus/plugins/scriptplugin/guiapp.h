#ifndef GUIAPP_H
#define GUIAPP_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/*! Scribus GUI tool */

/*! docstring */
PyDoc_STRVAR(scribus_messagebartext__doc__,
    "messagebarText(\"string\")\n\n\
Writes the \"string\" into the Scribus message bar (status line).\
The text must be UTF8 encoded.");
/**
Changes the status bar string.
TODO: national chars handling.
(Petr Vanek 02/19/04)
*/
PyObject *scribus_messagebartext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_progressreset__doc__,
    "progressReset()\n\n\
Cleans up the Scribus progress bar previous settings. It is called\
before the new progress bar use.");
/**
Progressbar handling
TODO: check total vs. set values.
(Petr Vanek 02/19/04)
*/
PyObject *scribus_progressreset(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_progresssettotalsteps__doc__,
    "progressTotal(max)\n\n\
Sets the progress bar's maximum steps value to the specified number.");
PyObject *scribus_progresssettotalsteps(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_progresssetprogress__doc__,
    "progressSet(nr)\n\n\
Progress bar handling. The progress bar uses the concept of steps;\
you give it the total number of steps and the number of steps completed\
so far and it will display the percentage of steps that have been\
completed. You can specify the total number of steps in the constructor\
or later with progressTotal(). The current number of steps is set with\
progressSet(). The progress bar can be rewound to the beginning with\
progressReset(). [taken from Trolltech's Qt docs]");
PyObject *scribus_progresssetprogress(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcursor__doc__,
    "setCursor()\n\n\
[UNSUPPORTED!]");
/**
Cursor handling
(Petr Vanek 02/19/04)
*/
PyObject *scribus_setcursor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_docchanged__doc__,
    "docChanged(bool)\n\n\
Enable/disable save icon in the Scribus icon bar. It's useful to call\
this procedure when you're changing the document.");
/**
Enable/disable save icon
(Petr Vanek 02/20/04)
*/
PyObject *scribus_docchanged(PyObject *self, PyObject* args);

#endif
