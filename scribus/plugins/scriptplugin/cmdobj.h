#ifndef CMDOBJ_H
#define CMDOBJ_H


/** Object related Commands */

/** Creates a rectangular with params X, Y (base position)
 b, h (width, height) and optional name of the object.
 */
PyObject *scribus_newrect(PyObject *self, PyObject* args);
/** Creates an ellipse with x, y, b and h - name optionally
 params.
 */
PyObject *scribus_newellipse(PyObject *self, PyObject* args);
/** Creates an image frame - x, y, b, h and opt. name. */
PyObject *scribus_newimage(PyObject *self, PyObject* args);
/** Creates a text frame - x, y, b, h and opt. name. */
PyObject *scribus_newtext(PyObject *self, PyObject* args);
/** Creates a line object - x, y, b, h and opt. name. */
PyObject *scribus_newline(PyObject *self, PyObject* args);
/** Creates a polygon line - list with points and opt. name as params. */
PyObject *scribus_polyline(PyObject *self, PyObject* args);
/** Creates a polygon - list with points and opt. name as params. */
PyObject *scribus_polygon(PyObject *self, PyObject* args);
/** Creates a Bezier line - list with points and opt. name as params. */
PyObject *scribus_bezierline(PyObject *self, PyObject* args);
/** Joins 2 objects - textframe and line - into text on path.
 Uses x, y (base of the new object), name of the text frame,
 name of the line and opt. new name as params. */
PyObject *scribus_pathtext(PyObject *self, PyObject* args);
/** Deletes an object - if is the name given the named object is
 deleted else the active object erased. */
PyObject *scribus_deleteobj(PyObject *self, PyObject* args);
/**
Enables/disables "Text Flows Around Box" feature for object.
Called with params string objectName and voluntary 1|0.
When 1 set flowing to true (0 to false). When is second param
empty flowing is reverted.
02/28/2004 petr vanek
 */
PyObject *scribus_textflow(PyObject *self, PyObject* args);
/**
User test if an object with specified name really exists in
the doc. Object name as param.
03/29/2004 petr vanek
ObjectName is now optional. When none set, search for selection...
07/11/2004 pv
*/
PyObject *scribus_objectexists(PyObject *self, PyObject* args);
/**
 Craig Ringer, 2004-09-09
 Apply the named style to the currently selected object.
 pv, 2004-09-13, optionaly param objectName + "check the page" stuff
 FIXME: should handled explicitly passed object name too.
 */
PyObject *scribus_setstyle(PyObject *self, PyObject* args);
/**
 Craig Ringer, 2004-09-09
 Enumerate all known paragraph styles
*/
PyObject *scribus_getstylenames(PyObject *self, PyObject* args);

#endif

