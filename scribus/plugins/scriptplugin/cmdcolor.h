#ifndef CMDCOLOR_H
#define CMDCOLOR_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/** Managing Colors */

/*! docstring */
PyDoc_STRVAR(scribus_colornames__doc__,
    "getColorNames() -> list\n\n\
Returns a list with the names of all defined colors.");
/** Returns a list with colours available in doc or in prefs. */
PyObject *scribus_colornames(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_getcolor__doc__,
    "getColor(\"name\") -> tuple\n\n\
Returns a tuple (C, M, Y, K) containing the four color components\
of the color \"name\".");
/** Returns a CMYK tuple of the specified color. */
PyObject *scribus_getcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolor__doc__,
    "changeColor(\"name\", c, m, y, k)\n\n\
Changes the color \"name\", The color value is defined via four\
components c = Cyan, m = Magenta, y = Yellow and k = Black.\
Color compontens should be in the range from 0 to 255.");
/** Sets named color with C,M,Y,K params. */
PyObject *scribus_setcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newcolor__doc__,
    "newColor(\"name\", c, m, y, k)\n\n\
Defines a new color \"name\". The color Value is defined via four\
components c = Cyan, m = Magenta, y = Yello and k = Black.\
color compontens should be in the range from 0 to 255.");
/** Creates new color with name, C, M, Y, K params. */
PyObject *scribus_newcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_delcolor__doc__,
    "deleteColor(\"name\", \"replace\")\n\n\
Deletes the color \"name\". Every occurence of that color\
is replaced by the color \"replace\".");
/** Deletes named color */
PyObject *scribus_delcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_replcolor__doc__,
    "replaceColor(\"name\", \"replace\")\n\n\
Every occurence of that color \"name\" is replaced by the\
color \"replace\".");
/** Replaces color with the 2nd one. */
PyObject *scribus_replcolor(PyObject *self, PyObject* args);

#endif

