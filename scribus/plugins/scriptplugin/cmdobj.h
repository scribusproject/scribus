#ifndef CMDOBJ_H
#define CMDOBJ_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/** Object related Commands */

/*! docstring */
PyDoc_STRVAR(scribus_newrect__doc__,
    "createRect()\n\n\
    Creates a new Rectangle on the actual Page and returns its Name.\
    The Coordinates are given in the actual measurement Unit of the\
    Document. \"name\" should be a unique Identifier for the Object\
    because you need this Name for further referencing of that Object.\
    If \"name\" is not given Scribus will create one for you.");
/** Creates a rectangular with params X, Y (base position)
 b, h (width, height) and optional name of the object.
 */
PyObject *scribus_newrect(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newellipse__doc__,
    "createEllipse()\n\n\
    Creates a new Ellipse on the actual Page and returns its Name.\
    The Coordinates are given in the actual measurement Unit of the\
    Document. \"name\" should be a unique Identifier for the Object\
    because you need this Name for further referencing of that Object.\
    If \"name\" is not given Scribus will create one for you.");
/** Creates an ellipse with x, y, b and h - name optionally
 params.
 */
PyObject *scribus_newellipse(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newimage__doc__,
    "createImage()\n\n\
    Creates a new Picture on the actual Page and returns its Name.\
    The Coordinates are given in the actual measurement Unit of the\
    Document. \"name\" should be a unique Identifier for the Object\
    because you need this Name for further referencing of that Object.\
    If \"name\" is not given Scribus will create one for you.");
/** Creates an image frame - x, y, b, h and opt. name. */
PyObject *scribus_newimage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newtext__doc__,
    "createText()\n\n\
    Creates a new Textframe on the actual Page and returns its Name.\
    The Coordinates are given in the actual measurement Unit of the\
    Document. \"name\" should be a unique Identifier for the Object\
    because you need this Name for further referencing of that Object.\
    If \"name\" is not given Scribus will create one for you.");
/** Creates a text frame - x, y, b, h and opt. name. */
PyObject *scribus_newtext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newline__doc__,
    "createLine()\n\n\
    Creates a new Line from the Point(x1, y1) to the Point(x2, y2)\
    and returns its Name. The Coordinates are given in the actual\
    measurement Unit of the Document. \"name\" should be a unique\
    Identifier for the Object because you need this Name for further\
    referencing of that Object. If \"name\" is not given Scribus\
    will create one for you.");
/** Creates a line object - x, y, b, h and opt. name. */
PyObject *scribus_newline(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_polyline__doc__,
    "createPolyLine()\n\n\
    Creates a new Polyline and returns its Name. The Points for the\
    Polyline are stored in the List \"list\" in the following Order:\
    x1, y1, x2, y2...xn. yn. The Coordinates are given in the actual\
    measurement Unit of the Document. \"name\" should be a unique\
    Identifier for the Object because you need this Name for further\
    referencing of that Object. If \"name\" is not given Scribus will\
    create one for you.");
/** Creates a polygon line - list with points and opt. name as params. */
PyObject *scribus_polyline(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_polygon__doc__,
    "createPolygon()\n\n\
    Creates a new Polygon and returns its Name. The Points for the\
    Polygon are stored in the List \"list\" in the following Order:\
    x1, y1, x2, y2...xn. yn. At least three Points are required. There\
    is no need to repeat the first Point to close the Polygon. The Polygon\
    is automatically closed by connecting the first and the last Point.\
    The Coordinates are given in the actual measurement Unit of the Document.\
    \"name\" should be a unique Identifier for the Object because you need\
    this Name for further referencing of that Object. If \"name\" is not\
    given Scribus will create one for you.");
/** Creates a polygon - list with points and opt. name as params. */
PyObject *scribus_polygon(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_bezierline__doc__,
    "createBezierLine()\n\n\
    Creates a new Bezier Curve and returns its Name. The Points for\
    the Bezier Curve are stored in the List \"list\" in the following\
    Order: x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn.\
    Where x and y mean the x and y Coordinates of the Point and kx and\
    ky meaning the Controlpoint for the Curve. The Coordinates are given\
    in the actual measurement Unit of the Document. \"name\" should be a\
    unique Identifier for the Object because you need this Name for\
    further referencing of that Object. If \"name\" is not given Scribus\
    will create one for you.");
/** Creates a Bezier line - list with points and opt. name as params. */
PyObject *scribus_bezierline(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_pathtext__doc__,
    "createPathText()\n\n\
    Creates a new PathText by merging the 2 Objects \"textbox\" and\
    \"beziercurve\" and returns its Name. The Coordinates are given\
    in the actual measurement Unit of the Document \"name\" should\
    be a unique Identifier for the Object because you need this Name\
    for further referencing of that Object. If \"name\" is not given\
    Scribus will create one for you.");
/** Joins 2 objects - textframe and line - into text on path.
 Uses x, y (base of the new object), name of the text frame,
 name of the line and opt. new name as params. */
PyObject *scribus_pathtext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deleteobj__doc__,
    "deleteObject()\n\n\
    Deletes the Item with the Name \"name\". If \"name\" is not\
    given the currently selected Item is deleted.");
/** Deletes an object - if is the name given the named object is
 deleted else the active object erased. */
PyObject *scribus_deleteobj(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_textflow__doc__,
    "textFlowsAroundFrame()\n\n\
    Enables/disables \"Text Flows Around Frame\" feature for object.\
    Called with params string name and voluntary 1|0. When 1 set flowing\
    to true (0 to false). When is second param empty flowing is reverted.");
/**
Enables/disables "Text Flows Around Box" feature for object.
Called with params string objectName and voluntary 1|0.
When 1 set flowing to true (0 to false). When is second param
empty flowing is reverted.
02/28/2004 petr vanek
 */
PyObject *scribus_textflow(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_objectexists__doc__,
    "textFlowsAroundFrame()\n\n\
    User test if an object with specified name really exists in the doc.\
    Optional parameter is the object name. When no param given returns\
    if there is something selected.");
/**
User test if an object with specified name really exists in
the doc. Object name as param.
03/29/2004 petr vanek
ObjectName is now optional. When none set, search for selection...
07/11/2004 pv
*/
PyObject *scribus_objectexists(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setstyle__doc__,
    "setStyle()\n\n\
    TODO: docstring");
/**
 Craig Ringer, 2004-09-09
 Apply the named style to the currently selected object.
 pv, 2004-09-13, optionaly param objectName + "check the page" stuff
 FIXME: should handled explicitly passed object name too.
 */
PyObject *scribus_setstyle(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getstylenames__doc__,
    "getAllStyles()\n\n\
    TODO: docstring");
/**
 Craig Ringer, 2004-09-09
 Enumerate all known paragraph styles
*/
PyObject *scribus_getstylenames(PyObject *self);

#endif

