#ifndef CMDGETPROP_H
#define CMDGETPROP_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/** Query-Functions */

/*! docstring */
PyDoc_STRVAR(scribus_getfillcolor__doc__,
    "getFillColor([\"name\"]) -> string\n\n\
Returns the name of the fill color of the object \"name\".\
If \"name\" is not given the currently selected item is used.");
/*! Returns fill color of the object */
PyObject *scribus_getfillcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinecolor__doc__,
    "getLineColor([\"name\"]) -> string\n\n\
Returns the name of the line color of the object \"name\".\
If \"name\" is not given the currently selected item is used.");
/*! Returns color of the line */
PyObject *scribus_getlinecolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinewidth__doc__,
    "getLineWidth([\"name\"]) -> integer\n\n\
Returns the line width of the object \"name\". If \"name\"\
is not given the currently selected Item is used.");
/*! Returns width of the line */
PyObject *scribus_getlinewidth(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlineshade__doc__,
    "getLineShade([\"name\"]) -> integer\n\n\
Returns the shading value of the line color of the object \"name\".\
If \"name\" is not given the currently selected item is used.");
/*! Returns shading of the line */
PyObject *scribus_getlineshade(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinejoin__doc__,
    "getLineJoin([\"name\"]) -> integer (see contants)\n\n\
Returns the line join style of the object \"name\". If \"name\"\
is not given the currently selected item is used.\
The join types are: JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND");
/*! Returns join type of the line */
PyObject *scribus_getlinejoin(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlineend__doc__,
    "getLineEnd([\"name\"]) -> integer (see constants)\n\n\
Returns the line cap style of the object \"name\". If \"name\"\
is not given the currently selected item is used.\
The cap types are: CAP_FLAT, CAP_ROUND, CAP_SQUARE");
/*! Returns cap type of the line */
PyObject *scribus_getlineend(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinestyle__doc__,
    "getLineStyle([\"name\"]) -> integer (see constants)\n\n\
Returns the line style of the object \"name\". If \"name\" is\
not given the currently selected item is used.\
Linestyles: LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID");
/*! Returns style type of the line */
PyObject *scribus_getlinestyle(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfillshade__doc__,
    "getFillShade([\"name\"]) -> integer\n\n\
Returns the shading value of the fill color of the object \"name\".\
If \"name\" is not given the currently selected item is used.");
/*! Returns fill shade of the object */
PyObject *scribus_getfillshade(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcornerrad__doc__,
    "getCornerRadius([\"name\"]) -> integer\n\n\
Returns the corner radius of the object \"name\". The radius is\
expressed in points. If \"name\" is not given the currently\
selected Item is used.");
/*! Returns corner radius of the object */
PyObject *scribus_getcornerrad(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getimgscale__doc__,
    "getImageScale([\"name\"]) -> tuple\n\n\
Returns a (x, y) tuple containing the scaling values of the image frame \"name\".\
If \"name\" is not given the currently selected Item is used.");
/*! Returns image scale of the object */
PyObject *scribus_getimgscale(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getimgname__doc__,
    "getImageName([\"name\"]) -> string\n\n\
Returns the filename for the image in the image frame.\
If \"name\" is not given the currently selected item is used.");
/*! Returns image name of the object */
PyObject *scribus_getimgname(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getposi__doc__,
    "getPosition([\"name\"]) -> tuple\n\n\
Returns a (x, y) tuple with the actual position of the object \"name\".\
If \"name\" is not given the currently selected item is used.\
The position is expressed in the actual measurement unit of the document\
- see UNIT_<type> for reference.");
/*! Returns position of the object */
PyObject *scribus_getposi(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getsize__doc__,
    "getSize([\"name\"]) -> tuple\n\n\
Returns a (width, height) tuple with the actual size of the object \"name\".\
If \"name\" is not given the currently selected item is used.\
The size is expressed in the actual measurement unit of the document\
- see UNIT_<type> for reference.");
/*! Returns size of the object */
PyObject *scribus_getsize(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getrotation__doc__,
    "getRotation([\"name\"]) -> integer\n\n\
Returns the rotation of the object \"name\". The value is expressed\
in degrees. If \"name\" is not given the currently selected item is used.");
/*! Returns rotation of the object */
PyObject *scribus_getrotation(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getallobj__doc__,
    "getAllObjects() -> list\n\n\
Returns a list containing the names of all objects on the actual page.");
/*! Returns a list with all objects in page */
PyObject *scribus_getallobj(PyObject *self, PyObject* args);

#endif

