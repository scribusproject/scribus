#ifndef CMDOBJ_H
#define CMDOBJ_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Object related Commands */

/*! docstring */
PyDoc_STRVAR(scribus_newrect__doc__,
    "createRect(x, y, width, height, [\"name\"]) -> string\n\n\
Creates a new rectangle on the actual page and returns its name.\
The coordinates are given in the actual measurement unit of the\
document (see UNIT constants). \"name\" should be a unique identifier for the object\
because you need this name for further referencing of that object.\
If \"name\" is not given Scribus will create one for you.");
/** Creates a rectangular with params X, Y (base position)
 b, h (width, height) and optional name of the object.
 */
PyObject *scribus_newrect(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newellipse__doc__,
    "createEllipse(x, y, width, height, [\"name\"]) -> string\n\n\
Creates a new ellipse on the actual page and returns its name.\
The coordinates are given in the actual measurement unit of the\
document (see UNIT constants). \"name\" should be a unique identifier for the object\
because you need this name for further referencing of that object.\
If \"name\" is not given Scribus will create one for you.");
/** Creates an ellipse with x, y, b and h - name optionally
 params.
 */
PyObject *scribus_newellipse(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newimage__doc__,
    "createImage(x, y, width, height, [\"name\"]) -> string\n\n\
Creates a new picture on the actual page and returns its name.\
The coordinates are given in the actual measurement unit of the\
document (see UNIT constants). \"name\" should be a unique identifier for the object\
because you need this name for further referencing of that object.\
If \"name\" is not given Scribus will create one for you.");
/** Creates an image frame - x, y, b, h and opt. name. */
PyObject *scribus_newimage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newtext__doc__,
    "createText(x, y, width, height, [\"name\"]) -> string\n\n\
Creates a new textframe on the actual page and returns its name.\
The coordinates are given in the actual measurement unit of the\
document (see UNIT constants). \"name\" should be a unique identifier for the object\
because you need this name for further referencing of that object.\
If \"name\" is not given Scribus will create one for you.");
/** Creates a text frame - x, y, b, h and opt. name. */
PyObject *scribus_newtext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newline__doc__,
    "createLine(x1, y1, x2, y2, [\"name\"]) -> string\n\n\
Creates a new line from the point(x1, y1) to the point(x2, y2)\
and returns its name. The coordinates are given in the actual\
measurement unit of the document (see UNIT constants). \"name\" should be a unique\
identifier for the object because you need this name for further\
referencing of that object. If \"name\" is not given Scribus\
will create one for you.");
/** Creates a line object - x, y, b, h and opt. name. */
PyObject *scribus_newline(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_polyline__doc__,
    "createPolyLine(list, [\"name\"]) -> string\n\n\
Creates a new polyline and returns its name. The pPoints for the\
polyline are stored in the list \"list\" in the following order:\
[x1, y1, x2, y2...xn. yn]. The coordinates are given in the actual\
measurement unit of the document (see UNIT constants). \"name\" should be a unique\
identifier for the object because you need this name for further\
referencing of that object. If \"name\" is not given Scribus will\
create one for you.");
/** Creates a polygon line - list with points and opt. name as params. */
PyObject *scribus_polyline(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_polygon__doc__,
    "createPolygon(list, [\"name\"]) -> string\n\n\
Creates a new polygon and returns its name. The points for the\
polygon are stored in the list \"list\" in the following order:\
[x1, y1, x2, y2...xn. yn]. At least three points are required. There\
is no need to repeat the first point to close the polygon. The polygon\
is automatically closed by connecting the first and the last point.\
The coordinates are given in the actual measurement unit of the document (see UNIT constants).\
\"name\" should be a unique identifier for the object because you need\
this name for further referencing of that object. If \"name\" is not\
given Scribus will create one for you.");
/** Creates a polygon - list with points and opt. name as params. */
PyObject *scribus_polygon(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_bezierline__doc__,
    "createBezierLine(list, [\"name\"]) -> string\n\n\
Creates a new bezier curve and returns its name. The points for\
the bezier curve are stored in the list \"list\" in the following\
order: [x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn].\
Where x and y mean the x and y coordinates of the point and kx and\
ky meaning the controlpoint for the curve. The coordinates are given\
in the actual measurement unit of the document (see UNIT constants). \"name\" should be a\
unique identifier for the object because you need this name for\
further referencing of that object. If \"name\" is not given Scribus\
will create one for you.");
/** Creates a Bezier line - list with points and opt. name as params. */
PyObject *scribus_bezierline(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_pathtext__doc__,
    "createPathText(x, y, \"textbox\", \"beziercurve\", [\"name\"]) -> string\n\n\
Creates a new pathText by merging the 2 objects \"textbox\" and\
\"beziercurve\" and returns its name. The coordinates are given\
in the actual measurement unit of the document (see UNIT constants). \"name\" should\
be a unique identifier for the object because you need this name\
for further referencing of that object. If \"name\" is not given\
Scribus will create one for you.");
/** Joins 2 objects - textframe and line - into text on path.
 Uses x, y (base of the new object), name of the text frame,
 name of the line and opt. new name as params. */
PyObject *scribus_pathtext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deleteobj__doc__,
    "deleteObject([\"name\"])\n\n\
Deletes the item with the name \"name\". If \"name\" is not\
given the currently selected item is deleted.");
/** Deletes an object - if is the name given the named object is
 deleted else the active object erased. */
PyObject *scribus_deleteobj(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_textflow__doc__,
    "textFlowsAroundFrame(\"name\" [, state])\n\n\
Enables/disables \"Text Flows Around Frame\" feature for object \"name\".\
Called with parameters string name and voluntary boolean state 1|0. When 1 set flowing\
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
    "objectExists([\"name\"]) -> bool\n\n\
User test if an object with specified name really exists in the document.\
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
    "setStyle(\"style\" [, \"name\"])\n\n\
Apply the named \"style\" to the object named \"name\". If is no\
name given, it's applied on the selected object.");
/**
 Craig Ringer, 2004-09-09
 Apply the named style to the currently selected object.
 pv, 2004-09-13, optionaly param objectName + "check the page" stuff
 FIXME: should handled explicitly passed object name too.
 */
PyObject *scribus_setstyle(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getstylenames__doc__,
    "getAllStyles() -> list\n\n\
Enumerate all known paragraph styles.");
/**
 Craig Ringer, 2004-09-09
 Enumerate all known paragraph styles
*/
PyObject *scribus_getstylenames(PyObject *self);

#endif

