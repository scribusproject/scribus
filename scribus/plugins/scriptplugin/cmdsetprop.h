#ifndef CMDSETPROP_H
#define CMDSETPROP_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/** Setting Object Properties */

/*! docstring */
PyDoc_STRVAR(scribus_setgradfill__doc__,
    "setGradientFill()\n\n\
    Sets the Gradient Fill of the Object \"name\" to type.\
    Color Descriptions are the same as for \"setFillColor\"\
    and \"setFillShade\".");
/*! Set gradient */
PyObject *scribus_setgradfill(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfillcolor__doc__,
    "setFillColor()\n\n\
    Sets the Fill Color of the Object \"name\" to the Color \"color\".\
    \"color\" is the Name of one of the defined Colors. If \"name\"\
    is not given the currently selected Item is used.");
/*! Set fill color */
PyObject *scribus_setfillcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinecolor__doc__,
    "setLineColor()\n\n\
    Sets the Line Color of the Object \"name\" to the Color \"color\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Set line color */
PyObject *scribus_setlinecolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinewidth__doc__,
    "setLineWidth()\n\n\
    Sets Line Width of the Object \"name\" to \"width\". \"width\" must\
    be in the range from 0.0 to 12.0 inclusive. If \"name\" is not given\
    the currently selected Item is used.");
/*! Set line width */
PyObject *scribus_setlinewidth(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlineshade__doc__,
    "setLineShade()\n\n\
    Sets the shading of the Line Color of the Object \"name\" to \"shade\".\
    \"shade\" must be an Integer Value in the range from 0 (lightest) to\
    100 (full Color intensity). If \"name\" is not given the currently\
    selected Item is used.");
/*! Set line shade */
PyObject *scribus_setlineshade(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinejoin__doc__,
    "setLineJoin()\n\n\
    Sets the Line Join Style of the Object \"name\" to the Style \"join\".\
    If \"name\" is not given the currently selected Item is used. There\
    are predefined Constants for \"join\".");
/*! Set line join */
PyObject *scribus_setlinejoin(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlineend__doc__,
    "setLineEnd()\n\n\
    Sets the Line Cap Style of the Object \"name\" to the Style \"cap\".\
    If \"name\" is not given the currently selected Item is used. There\
    are predefined Constants for \"cap\".");
/*! Set line end */
PyObject *scribus_setlineend(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinestyle__doc__,
    "setLineStyle()\n\n\
    Sets the Line Style of the Object \"name\" to the Style \"style\".\
    If \"name\" is not given the currently selected Item is used. There\
    are predefined Constants for \"style\".");
/*! Set line end */
PyObject *scribus_setlinestyle(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfillshade__doc__,
    "setFillShade()\n\n\
    Sets the shading of the Fill Color of the Object \"name\" to \"shade\".\
    \"shade\" must be an Integer Value in the range from 0 (lightest) to 100\
    (full Color intensity). If \"name\" is not given the currently selected\
    Item is used.");
/*! Set fill shade */
PyObject *scribus_setfillshade(PyObject *self, PyObject* args);

/*! docstringscribus_setmultiline__doc__ */
PyDoc_STRVAR(scribus_setcornerrad__doc__,
    "setCornerRadius()\n\n\
    Sets the Corner Radius of the Object \"name\" The Radius is expressed\
    in Points. If \"name\" is not given the currently selected Item is used.");
/*! Set corner radius */
PyObject *scribus_setcornerrad(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setmultiline__doc__,
    "setMultiLine()\n\n\
    Sets the Line Style of the Object \"name\" to the Named Style \"namedStyle\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Set multiline */
PyObject *scribus_setmultiline(PyObject *self, PyObject* args);

#endif

