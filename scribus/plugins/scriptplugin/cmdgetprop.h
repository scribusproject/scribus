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
    "getFillColor([objectName]) -> string\n\n\
    Returns the name of the Fill Color of the Object \"name\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Returns fill color of the object */
PyObject *scribus_getfillcolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinecolor__doc__,
    "getLineColor([objectName]) -> string\n\n\
    Returns the name of the Line Color of the Object \"name\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Returns color of the line */
PyObject *scribus_getlinecolor(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinewidth__doc__,
    "getLineWidth([objectName]) -> integer\n\n\
    Returns the Line Width of the Object \"name\". If \"name\"\
    is not given the currently selected Item is used.");
/*! Returns width of the line */
PyObject *scribus_getlinewidth(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlineshade__doc__,
    "getLineShade([objectName]) -> integer\n\n\
    Returns the shading Value of the Line Color of the Object \"name\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Returns shading of the line */
PyObject *scribus_getlineshade(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinejoin__doc__,
    "getLineJoin([objectName]) -> integer\n\n\
    Returns the Line Join Style of the Object \"name\". If \"name\"\
    is not given the currently selected Item is used.");
/*! Returns join type of the line */
PyObject *scribus_getlinejoin(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlineend__doc__,
    "getLineEnd([objectName]) -> integer\n\n\
    Returns the Line Cap Style of the Object \"name\". If \"name\"\
    is not given the currently selected Item is used.");
/*! Returns cap type of the line */
PyObject *scribus_getlineend(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinestyle__doc__,
    "getLineStyle([objectName]) -> integer\n\n\
    Returns the Line Style of the Object \"name\". If \"name\" is\
    not given the currently selected Item is used.");
/*! Returns style type of the line */
PyObject *scribus_getlinestyle(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfillshade__doc__,
    "getFillShade([objectName]) -> integer\n\n\
    Returns the shading Value of the Fill Color of the Object \"name\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Returns fill shade of the object */
PyObject *scribus_getfillshade(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcornerrad__doc__,
    "getCornerRadius([name]) -> integer\n\n\
    Returns the Corner Radius of the Object \"name\" The Radius is\
    expressed in Points. If \"name\" is not given the currently\
    selected Item is used.");
/*! Returns corner radius of the object */
PyObject *scribus_getcornerrad(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getimgscale__doc__,
    "getImageScale([name]) -> tuple\n\n\
    Returns a Tuple containing the Scaling Values of the Image Frame \"name\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Returns image scale of the object */
PyObject *scribus_getimgscale(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getimgname__doc__,
    "getImageName([name]) -> string\n\n\
    Returns the Filename for the Image in the Image Frame.\
    If \"name\" is not given the currently selected Item is used.");
/*! Returns image name of the object */
PyObject *scribus_getimgname(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getposi__doc__,
    "getPosition([name]) -> tuple\n\n\
    Returns a tuple with the actual Position of the Object \"name\".\
    If \"name\" is not given the currently selected Item is used.\
    The Position is expressed in the actual Measurement Unit of the Document.");
/*! Returns position of the object */
PyObject *scribus_getposi(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getsize__doc__,
    "getSize([name]) -> tuple\n\n\
    Returns a tuple with the actual Size of the Object \"name\".\
    If \"name\" is not given the currently selected Item is used.\
    The Size is expressed in the actual Measurement Unit of the Document.");
/*! Returns size of the object */
PyObject *scribus_getsize(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getrotation__doc__,
    "getRotation([name]) -> integer\n\n\
    Returns the Rotation of the Object \"name\". The value is expressed\
    in Degrees. If \"name\" is not given the currently selected Item is used.");
/*! Returns rotation of the object */
PyObject *scribus_getrotation(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getallobj__doc__,
    "getAllObjects() -> list\n\n\
    Returns a List containing the Names of all Objects on the actual Page.");
/*! Returns a list with all objects in page */
PyObject *scribus_getallobj(PyObject *self, PyObject* args);

#endif

