#ifndef CMDOBJ_H
#define CMDOBJ_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Object related Commands */

/*! docstring */
PyDoc_STRVAR(scribus_newrect__doc__,
QT_TR_NOOP("createRect(x, y, width, height, [\"name\"]) -> string\n\
\n\
Creates a new rectangle on the current page and returns its name. The\n\
coordinates are given in the current measurement units of the document\n\
(see UNIT constants). \"name\" should be a unique identifier for the object\n\
because you need this name to reference that object in future. If \"name\"\n\
is not given Scribus will create one for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
"));
/** Creates a rectangular with params X, Y (base position)
 b, h (width, height) and optional name of the object.
 */
PyObject *scribus_newrect(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newellipse__doc__,
QT_TR_NOOP("createEllipse(x, y, width, height, [\"name\"]) -> string\n\
\n\
Creates a new ellipse on the current page and returns its name.\n\
The coordinates are given in the current measurement units of the document\n\
(see UNIT constants). \"name\" should be a unique identifier for the object\n\
because you need this name for further referencing of that object. If \"name\"\n\
is not given Scribus will create one for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
"));
/** Creates an ellipse with x, y, b and h - name optionally
 params.
 */
PyObject *scribus_newellipse(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newimage__doc__,
QT_TR_NOOP("createImage(x, y, width, height, [\"name\"]) -> string\n\
\n\
Creates a new picture frame on the current page and returns its name. The\n\
coordinates are given in the current measurement units of the document.\n\
\"name\" should be a unique identifier for the object because you need this\n\
name for further access to that object. If \"name\" is not given Scribus will\n\
create one for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
"));
/** Creates an image frame - x, y, b, h and opt. name. */
PyObject *scribus_newimage(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newtext__doc__,
QT_TR_NOOP("createText(x, y, width, height, [\"name\"]) -> string\n\
\n\
Creates a new text frame on the actual page and returns its name.\n\
The coordinates are given in the actual measurement unit of the document (see\n\
UNIT constants). \"name\" should be a unique identifier for the object because\n\
you need this name for further referencing of that object. If \"name\" is not\n\
given Scribus will create one for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
"));
/** Creates a text frame - x, y, b, h and opt. name. */
PyObject *scribus_newtext(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newline__doc__,
QT_TR_NOOP("createLine(x1, y1, x2, y2, [\"name\"]) -> string\n\
\n\
Creates a new line from the point(x1, y1) to the point(x2, y2) and returns\n\
its name. The coordinates are given in the current measurement unit of the\n\
document (see UNIT constants). \"name\" should be a unique identifier for the\n\
object because you need this name for further access to that object. If\n\
\"name\" is not given Scribus will create one for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
"));
/** Creates a line object - x, y, b, h and opt. name. */
PyObject *scribus_newline(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_polyline__doc__,
QT_TR_NOOP("createPolyLine(list, [\"name\"]) -> string\n\
\n\
Creates a new polyline and returns its name. The points for the polyline are\n\
stored in the list \"list\" in the following order: [x1, y1, x2, y2...xn. yn].\n\
The coordinates are given in the current measurement units of the document (see\n\
UNIT constants). \"name\" should be a unique identifier for the object because\n\
you need this name for further access to that object. If \"name\" is not given\n\
Scribus will create one for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
May raise ValueError if an insufficient number of points is passed or if\n\
the number of values passed don't group into points without leftovers.\n\
"));
/** Creates a polygon line - list with points and opt. name as params. */
PyObject *scribus_polyline(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_polygon__doc__,
QT_TR_NOOP("createPolygon(list, [\"name\"]) -> string\n\
\n\
Creates a new polygon and returns its name. The points for the polygon are\n\
stored in the list \"list\" in the following order: [x1, y1, x2, y2...xn. yn].\n\
At least three points are required. There is no need to repeat the first point\n\
to close the polygon. The polygon is automatically closed by connecting the\n\
first and the last point.  The coordinates are given in the current measurement\n\
units of the document (see UNIT constants).  \"name\" should be a unique\n\
identifier for the object because you need this name for further access to that\n\
object. If \"name\" is not given Scribus will create one for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
May raise ValueError if an insufficient number of points is passed or if\n\
the number of values passed don't group into points without leftovers.\n\
"));
/** Creates a polygon - list with points and opt. name as params. */
PyObject *scribus_polygon(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_bezierline__doc__,
QT_TR_NOOP("createBezierLine(list, [\"name\"]) -> string\n\
\n\
Creates a new bezier curve and returns its name. The points for the bezier\n\
curve are stored in the list \"list\" in the following order:\n\
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn]\n\
In the points list, x and y mean the x and y coordinates of the point and kx\n\
and ky meaning the control point for the curve.  The coordinates are given in\n\
the current measurement units of the document (see UNIT constants). \"name\"\n\
should be a unique identifier for the object because you need this name for\n\
further access to that object. If \"name\" is not given Scribus will create one\n\
for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
May raise ValueError if an insufficient number of points is passed or if\n\
the number of values passed don't group into points without leftovers.\n\
"));
/** Creates a Bezier line - list with points and opt. name as params. */
PyObject *scribus_bezierline(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_pathtext__doc__,
QT_TR_NOOP("createPathText(x, y, \"textbox\", \"beziercurve\", [\"name\"]) -> string\n\
\n\
Creates a new pathText by merging the two objects \"textbox\" and\n\
\"beziercurve\" and returns its name. The coordinates are given in the current\n\
measurement unit of the document (see UNIT constants). \"name\" should be a\n\
unique identifier for the object because you need this name for further access\n\
to that object. If \"name\" is not given Scribus will create one for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
May raise NotFoundError if one or both of the named base object don't exist.\n\
"));
/** Joins 2 objects - textframe and line - into text on path.
 Uses x, y (base of the new object), name of the text frame,
 name of the line and opt. new name as params. */
PyObject *scribus_pathtext(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deleteobj__doc__,
QT_TR_NOOP("deleteObject([\"name\"])\n\
\n\
Deletes the item with the name \"name\". If \"name\" is not given the currently\n\
selected item is deleted.\n\
"));
/** Deletes an object - if is the name given the named object is
 deleted else the active object erased. */
PyObject *scribus_deleteobj(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_textflow__doc__,
QT_TR_NOOP("textFlowsAroundFrame(\"name\" [, state])\n\
\n\
Enables/disables \"Text Flows Around Frame\" feature for object \"name\".\n\
Called with parameters string name and optional boolean \"state\". If \"state\"\n\
is not passed, text flow is toggled.\n\
"));
/**
Enables/disables "Text Flows Around Box" feature for object.
Called with params string objectName and voluntary 1|0.
When 1 set flowing to true (0 to false). When is second param
empty flowing is reverted.
02/28/2004 petr vanek
 */
PyObject *scribus_textflow(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_objectexists__doc__,
QT_TR_NOOP("objectExists([\"name\"]) -> bool\n\
\n\
Test if an object with specified name really exists in the document.\n\
The optional parameter is the object name. When no object name is given,\n\
returns True if there is something selected.\n\
"));
/**
User test if an object with specified name really exists in
the doc. Object name as param.
03/29/2004 petr vanek
ObjectName is now optional. When none set, search for selection...
07/11/2004 pv
*/
PyObject *scribus_objectexists(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setstyle__doc__,
QT_TR_NOOP("setStyle(\"style\" [, \"name\"])\n\
\n\
Apply the named \"style\" to the object named \"name\". If is no object name\n\
given, it's applied on the selected object.\n\
"));
/**
 Craig Ringer, 2004-09-09
 Apply the named style to the currently selected object.
 pv, 2004-09-13, optionaly param objectName + "check the page" stuff
 FIXME: should handled explicitly passed object name too.
 */
PyObject *scribus_setstyle(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getstylenames__doc__,
QT_TR_NOOP("getAllStyles() -> list\n\
\n\
Return a list of the names of all paragraph styles in the current document.\n\
"));
/**
 Craig Ringer, 2004-09-09
 Enumerate all known paragraph styles
*/
PyObject *scribus_getstylenames(PyObject */*self*/);

/* Internal function not intended for general use; no docstring */
PyObject* scribus_getframetype(PyObject* self, PyObject* args, PyObject* kw);

#endif

