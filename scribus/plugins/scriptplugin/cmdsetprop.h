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
    "setGradientFill(type, \"color1\", shade1, \"color2\", shade2, [\"name\"])\n\n\
Sets the gradient fill of the object \"name\" to type.\
Color descriptions are the same as for setFillColor()\
and setFillShade(). See the constants for available types (FILL_<type>).");
/*! Set gradient */
PyObject *scribus_setgradfill(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfillcolor__doc__,
    "setFillColor(\"color\", [\"name\"])\n\n\
Sets the fill color of the object \"name\" to the color \"color\".\
\"color\" is the name of one of the defined colors. If \"name\"\
is not given the currently selected item is used.");
/*! Set fill color */
PyObject *scribus_setfillcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinecolor__doc__,
    "setLineColor(\"color\", [\"name\"])\n\n\
Sets the line color of the object \"name\" to the color \"color\".\
If \"name\" is not given the currently selected item is used.");
/*! Set line color */
PyObject *scribus_setlinecolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinewidth__doc__,
    "setLineWidth(width, [\"name\"])\n\n\
Sets line width of the object \"name\" to \"width\". \"width\" must\
be in the range from 0.0 to 12.0 inclusive. If \"name\" is not given\
the currently selected item is used.");
/*! Set line width */
PyObject *scribus_setlinewidth(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlineshade__doc__,
    "setLineShade(shade, [\"name\"])\n\n\
Sets the shading of the line color of the object \"name\" to \"shade\".\
\"shade\" must be an integer value in the range from 0 (lightest) to\
100 (full color intensity). If \"name\" is not given the currently\
selected item is used.");
/*! Set line shade */
PyObject *scribus_setlineshade(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinejoin__doc__,
    "setLineJoin(join, [\"name\"])\n\n\
Sets the line join style of the object \"name\" to the style \"join\".\
If \"name\" is not given the currently selected item is used. There\
are predefined constants for join - JOIN_<type>.");
/*! Set line join */
PyObject *scribus_setlinejoin(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlineend__doc__,
    "setLineEnd(endtype, [\"name\"])\n\n\
Sets the line cap style of the object \"name\" to the style \"cap\".\
If \"name\" is not given the currently selected item is used. There\
are predefined constants for \"cap\" - CAP_<type>.");
/*! Set line end */
PyObject *scribus_setlineend(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinestyle__doc__,
    "setLineStyle(style, [\"name\"])\n\n\
Sets the line style of the object \"name\" to the style \"style\".\
If \"name\" is not given the currently selected item is used. There\
are predefined constants for \"style\" - LINE_<style>.");
/*! Set line end */
PyObject *scribus_setlinestyle(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfillshade__doc__,
    "setFillShade(shade, [\"name\"])\n\n\
Sets the shading of the fill color of the object \"name\" to \"shade\".\
\"shade\" must be an integer value in the range from 0 (lightest) to 100\
(full Color intensity). If \"name\" is not given the currently selected\
Item is used.");
/*! Set fill shade */
PyObject *scribus_setfillshade(PyObject *self, PyObject* args);

/*! docstringscribus_setmultiline__doc__ */
PyDoc_STRVAR(scribus_setcornerrad__doc__,
    "setCornerRadius(radius, [\"name\"])\n\n\
Sets the corner radius of the object \"name\". The radius is expressed\
in points. If \"name\" is not given the currently selected item is used.");
/*! Set corner radius */
PyObject *scribus_setcornerrad(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setmultiline__doc__,
    "setMultiLine(\"namedStyle\", [\"name\"])\n\n\
Sets the line style of the object \"name\" to the named style \"namedStyle\".\
If \"name\" is not given the currently selected item is used.");
/*! Set multiline */
PyObject *scribus_setmultiline(PyObject *self, PyObject* args);

#endif

