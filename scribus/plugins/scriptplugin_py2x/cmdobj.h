/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDOBJ_H
#define CMDOBJ_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Object related Commands */

/*! docstring */
PyDoc_STRVAR(scribus_createrect__doc__,
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
PyObject *scribus_createrect(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createellipse__doc__,
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
PyObject *scribus_createellipse(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createimage__doc__,
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
PyObject *scribus_createimage(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createtext__doc__,
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
PyObject *scribus_createtext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createtable__doc__,
QT_TR_NOOP("createTable(x, y, width, height, numRows, numColumns, [\"name\"]) -> string\n\
\n\
Creates a new table with the given number of rows and columns on the actual page\n\
and returns its name. The coordinates are given in the actual measurement unit of\n\
the document (see UNIT constants). \"name\" should be a unique identifier for\n\
the object because you need this name for further referencing of that object. If\n\
\"name\" is not given Scribus will create one for you.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
May raise ValueError if an insufficient number of rows or columns is passed.\n\
"));
/** Creates a table - x, y, width, height, numRows, numColumns and opt. name. */
PyObject *scribus_createtable(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createline__doc__,
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
PyObject *scribus_createline(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createpolyline__doc__,
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
PyObject *scribus_createpolyline(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createpolygon__doc__,
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
PyObject *scribus_createpolygon(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createbezierline__doc__,
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
PyObject *scribus_createbezierline(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createpathtext__doc__,
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
PyObject *scribus_createpathtext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deleteobject__doc__,
QT_TR_NOOP("deleteObject([\"name\"])\n\
\n\
Deletes the item with the name \"name\". If \"name\" is not given the currently\n\
selected item is deleted.\n\
"));
/** Deletes an object - if is the name given the named object is
 deleted else the active object erased. */
PyObject *scribus_deleteobject(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_gettextflowmode__doc__,
QT_TR_NOOP("getTextFlowMode([\"name\"]) -> integer\n\
\n\
Return the current text flow mode used by item \"name\" as an integer.\n\
If \"name\" is not given, the currently selected object is used.\n\
\n\
The function will return one of the following value:\n\
- 0 : text flow around frame is disabled\n\
- 1 : text flow around frame shape\n\
- 2 : text flow around frame bounding box\n\
- 3 : text flow around frame contour line\n\
- 4 : text flow around image clip path\n\
"));
PyObject *scribus_gettextflowmode(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextflowmode__doc__,
QT_TR_NOOP("setTextFlowMode(\"name\" [, state])\n\
\n\
Enables/disables \"Text Flows Around Frame\" feature for object \"name\".\n\
Called with parameters string name and optional int \"state\" (0 <= state <= 3).\n\
Setting \"state\" to 0 will disable text flow.\n\
Setting \"state\" to 1 will make text flow around object frame.\n\
Setting \"state\" to 2 will make text flow around bounding box.\n\
Setting \"state\" to 3 will make text flow around contour line.\n\
If \"state\" is not passed, text flow is toggled.\n\
"));
PyDoc_STRVAR(scribus_textflowmode__doc__,
QT_TR_NOOP("textFlowMode(\"name\" [, state])\n\
\n\
Deprecated. Use setTextFlowMode() instead.\n\
"));
/**
Enables/disables "Text Flows Around Object" feature for object.
Called with params string objectName and state 0|1|2|3.
When set to 0 disable flowing, 1 text flows around frame, 
2 around bounding box, 3 around contour line. When is second param
empty flowing is reverted.
02/28/2004 petr vanek
 */
PyObject *scribus_settextflowmode(PyObject * /*self*/, PyObject* args);

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
PyObject *scribus_objectexists(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcharacterstyle__doc__,
QT_TR_NOOP("getCharacterStyle([\"name\"])\n\
\n\
Return name of character style applied to object named \"name\". If \"name\" is not given,\n\
the currently selected object is used. If current object has a text selection, \n\
the name of style applied to start of selection is returned. Otherwise the name \n\
of the item default character style is returned.\n\
"));
PyObject *scribus_getcharacterstyle(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getparagraphstyle__doc__,
QT_TR_NOOP("getParagraphStyle([\"name\"])\n\
\n\
Return name of paragraph style applied to object named \"name\". If \"name\" is not given,\n\
the currently selected object is used. If current object has a text selection, \n\
the name of style applied to start of selection is returned. Otherwise the name \n\
of the item default style is returned.\n\
"));

PyDoc_STRVAR(scribus_getstyle__doc__,
QT_TR_NOOP("getStyle([\"name\"])\n\
\n\
Deprecated. Use getParagraphStyle() instead.\n\
"));
/**
 Vaclav Smilauer, 2017-12-21
 Return style name of the object (or currently selected object)
 */
PyObject *scribus_getparagraphstyle(PyObject * /*self*/, PyObject* args);


/*! docstring */
PyDoc_STRVAR(scribus_setparagraphstyle__doc__,
QT_TR_NOOP("setParagraphStyle(\"style\" [, \"name\"])\n\
\n\
Apply the named paragraph \"style\" to the object named \"name\".\n\
If  object name is not provided, style is applied on current object selection.\n\
If multiple objects are selected or if selected object has no text selection,\n\
style is applied on selected objects. Otherwise style is applied to the current\n\
text selection.\n\
"));

PyDoc_STRVAR(scribus_setstyle__doc__,
QT_TR_NOOP("setStyle(\"style\" [, \"name\"])\n\
\n\
Deprecated. Use setParagraphStyle() instead.\n\
"));
/**
 Craig Ringer, 2004-09-09
 Apply the named style to the currently selected object.
 pv, 2004-09-13, optionally param objectName + "check the page" stuff
 */
PyObject *scribus_setparagraphstyle(PyObject * /*self*/, PyObject* args);


/*! docstring */
PyDoc_STRVAR(scribus_setcharstyle__doc__,
QT_TR_NOOP("setCharacterStyle(\"style\" [, \"name\"])\n\
\n\
Apply the named character \"style\" to the object named \"name\".\n\
If  object name is not provided, style is applied on current object selection.\n\
If multiple objects are selected or if selected object has no text selection,\n\
style is applied on selected objects. Otherwise style is applied to the current\n\
text selection.\n\
"));
/**
Apply the named character style to the currently selected object.
*/
PyObject *scribus_setcharstyle(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_duplicateobject__doc__,
QT_TR_NOOP("duplicateObject([\"name\"]) -> string\n\
\n\
Creates a duplicate of the specified object or of first item of selection if \"name\" is not given.\n\
Returns name of new object.\n\
Deprecated. Use duplicateObjects() instead.\n\
"));
/**
 Christian Hausknecht, 2006-07-12
 duplicate an object
*/
PyObject *scribus_duplicateobject(PyObject * /* self */, PyObject *args);

/*! docstring */
PyDoc_STRVAR(scribus_duplicateobjects__doc__,
QT_TR_NOOP("duplicateObjects([names]) -> list\n\
\n\
Creates a duplicate of the specified objects or of the current selection if no names are given.\n\
The names of objects to duplicate can be provided as a string to duplicate a single object\n\
or as a list of strings to duplicate several objects at once.\n\
Returns a list of the names of the newly created objects.\n\
"));
PyObject *scribus_duplicateobjects(PyObject * /* self */, PyObject *args);

/*! docstring */
PyDoc_STRVAR(scribus_copyobject__doc__,
QT_TR_NOOP("copyObject([\"name\"]) -> string\n\
\n\
Copies the specified object or first item of selection if \"name\" is not given.\n\
Deprecated. Use copyObjects instead.\n\
"));
/**
 Gregory Pittman, 2012-01-12
 copy an object
*/
PyObject *scribus_copyobject(PyObject * /* self */, PyObject *args);

/*! docstring */
PyDoc_STRVAR(scribus_copyobjects__doc__,
QT_TR_NOOP("copyObjects([names])\n\
\n\
Copies the specified objects or the current object selection if no item names are given.\n\
The names of objects to copy can be provided as a string for copying a single object\n\
or as a list of strings to copy several objects at once.\n\
"));
PyObject *scribus_copyobjects(PyObject * /* self */, PyObject *args);

/*! docstring */
PyDoc_STRVAR(scribus_pasteobject__doc__,
QT_TR_NOOP("pasteObject() -> string\n\
\n\
Pastes an object from the clipboard. This will be used only or most\n\
sensibly following copyObject(...), since otherwise there will likely\n\
be nothing in the clipboard to paste.\n\
Returns the names of the newly created object in a comma separated string.\n\
Deprecated. Use pasteObjects() instead.\n\
"));
/**
 Gregory Pittman, 2012-01-29
 pastes an object
*/
PyObject *scribus_pasteobject(PyObject * /* self */, PyObject *args);

/*! docstring */
PyDoc_STRVAR(scribus_pasteobjects__doc__,
QT_TR_NOOP("pasteObjects() -> list\n\
\n\
Pastes the content of clipboard to canvas. This will be used only or most\n\
sensibly following copyObjects(...), since otherwise there will likely\n\
be nothing in the clipboard to paste.\n\
Returns the names of the newly created object in a list.\n\
"));
PyObject *scribus_pasteobjects(PyObject * /* self */, PyObject *args);

#endif

