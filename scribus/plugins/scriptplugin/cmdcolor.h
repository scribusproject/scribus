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
    Returns a List with the Names of all defined Colors.");
/** Returns a list with colours available in doc or in prefs. */
PyObject *scribus_colornames(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_getcolor__doc__,
    "getColor(name) -> tuple\n\n\
    Returns a Tuple containing the four Color Components of the Color \"name\".");
/** Returns a CMYK tuple of the specified color. */
PyObject *scribus_getcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolor__doc__,
    "changeColor(name, value)\n\n\
    Changes the Color \"name\", The Color Value is defined via four\
    Components c = Cyan, m = Magenta, y = Yello and k = Black.\
    Color Compontens should be in the range from 0 to 255.");
/** Sets named color with C,M,Y,K params. */
PyObject *scribus_setcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newcolor__doc__,
    "newColor(name, value)\n\n\
    Defines a new Color \"name\". The Color Value is defined via four\
    Components c = Cyan, m = Magenta, y = Yello and k = Black.\
    Color Compontens should be in the range from 0 to 255.");
/** Creates new color with name, C, M, Y, K params. */
PyObject *scribus_newcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_delcolor__doc__,
    "deleteColor(name)\n\n\
    Deletes the Color \"name\". Every occurence of that Color\
    is replaced by the Color \"replace\".");
/** Deletes named color */
PyObject *scribus_delcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_replcolor__doc__,
    "replaceColor(name, value)\n\n\
    Every occurence of that Color is replaced by the Color \"replace\".");
/** Replaces color with the 2nd one. */
PyObject *scribus_replcolor(PyObject *self, PyObject* args);

#endif

