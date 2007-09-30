/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDMANI_H
#define CMDMANI_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Manipulating Objects */

/*! docstring */
PyDoc_STRVAR(scribus_moveobjrel__doc__,
QT_TR_NOOP("moveObject(dx, dy [, \"name\"])\n\
\n\
Moves the object \"name\" by dx and dy relative to its current position. The\n\
distances are expressed in the current measurement unit of the document (see\n\
UNIT constants). If \"name\" is not given the currently selected item is used.\n\
If the object \"name\" belongs to a group, the whole group is moved.\n\
"));
/*! Move REL the object */
PyObject *scribus_moveobjrel(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_moveobjabs__doc__,
QT_TR_NOOP("moveObjectAbs(x, y [, \"name\"])\n\
\n\
Moves the object \"name\" to a new location. The coordinates are expressed in\n\
the current measurement unit of the document (see UNIT constants).  If \"name\"\n\
is not given the currently selected item is used.  If the object \"name\"\n\
belongs to a group, the whole group is moved.\n\
"));
/*! Move ABS the object */
PyObject *scribus_moveobjabs(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_rotobjrel__doc__,
QT_TR_NOOP("rotateObject(rot [, \"name\"])\n\
\n\
Rotates the object \"name\" by \"rot\" degrees relatively. The object is\n\
rotated by the vertex that is currently selected as the rotation point - by\n\
default, the top left vertex at zero rotation. Positive values mean counter\n\
clockwise rotation when the default rotation point is used. If \"name\" is not\n\
given the currently selected item is used.\n\
"));
/*! Rotate REL the object */
PyObject *scribus_rotobjrel(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_rotobjabs__doc__,
QT_TR_NOOP("rotateObjectAbs(rot [, \"name\"])\n\
\n\
Sets the rotation of the object \"name\" to \"rot\". Positive values\n\
mean counter clockwise rotation. If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Rotate ABS the object */
PyObject *scribus_rotobjabs(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_sizeobjabs__doc__,
QT_TR_NOOP("sizeObject(width, height [, \"name\"])\n\
\n\
Resizes the object \"name\" to the given width and height. If \"name\"\n\
is not given the currently selected item is used.\n\
"));
/*! Resize ABS the object */
PyObject *scribus_sizeobjabs(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getselobjnam__doc__,
QT_TR_NOOP("getSelectedObject([nr]) -> string\n\
\n\
Returns the name of the selected object. \"nr\" if given indicates the number\n\
of the selected object, e.g. 0 means the first selected object, 1 means the\n\
second selected Object and so on.\n\
"));
/*! Returns name of the selected object */
PyObject *scribus_getselobjnam(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_selcount__doc__,
QT_TR_NOOP("selectionCount() -> integer\n\
\n\
Returns the number of selected objects.\n\
"));
/*! Returns count of the selected object */
PyObject *scribus_selcount(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_selectobj__doc__,
QT_TR_NOOP("selectObject(\"name\")\n\
\n\
Selects the object with the given \"name\".\n\
"));
/*! Count selection */
PyObject *scribus_selectobj(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deselect__doc__,
QT_TR_NOOP("deselectAll()\n\
\n\
Deselects all objects in the whole document.\n\
"));
/*! Remove all selection */
PyObject *scribus_deselect(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_groupobj__doc__,
QT_TR_NOOP("groupObjects(list)\n\
\n\
Groups the objects named in \"list\" together. \"list\" must contain the names\n\
of the objects to be grouped. If \"list\" is not given the currently selected\n\
items are used.\n\
"));
/*! Group objects named in list. */
PyObject *scribus_groupobj(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_ungroupobj__doc__,
QT_TR_NOOP("unGroupObjects(\"name\")\n\n\
Destructs the group the object \"name\" belongs to.\
If \"name\" is not given the currently selected item is used."));
/*! Ungroup objects named in list. */
PyObject *scribus_ungroupobj(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_scalegroup__doc__,
QT_TR_NOOP("scaleGroup(factor [,\"name\"])\n\
\n\
Scales the group the object \"name\" belongs to. Values greater than 1 enlarge\n\
the group, values smaller than 1 make the group smaller e.g a value of 0.5\n\
scales the group to 50 % of its original size, a value of 1.5 scales the group\n\
to 150 % of its original size.  The value for \"factor\" must be greater than\n\
0. If \"name\" is not given the currently selected item is used.\n\
\n\
May raise ValueError if an invalid scale factor is passed.\n\
"));
/*! Scale group with object name */
PyObject *scribus_scalegroup(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_loadimage__doc__,
QT_TR_NOOP("loadImage(\"filename\" [, \"name\"])\n\
\n\
Loads the picture \"picture\" into the image frame \"name\". If \"name\" is\n\
not given the currently selected item is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Loads image file into frame. */
PyObject *scribus_loadimage(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_scaleimage__doc__,
QT_TR_NOOP("scaleImage(x, y [, \"name\"])\n\
\n\
Sets the scaling factors of the picture in the image frame \"name\".\n\
If \"name\" is not given the currently selected item is used. A number of 1\n\
means 100 %.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Scale Image. */
PyObject *scribus_scaleimage(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_lockobject__doc__,
QT_TR_NOOP("lockObject([\"name\"]) -> bool\n\
\n\
Locks the object \"name\" if it's unlocked or unlock it if it's locked.\n\
If \"name\" is not given the currently selected item is used. Returns true\n\
if locked.\n\
"));
/*! (Un)Lock the object 2004/7/10 pv.*/
PyObject *scribus_lockobject(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_islocked__doc__,
QT_TR_NOOP("isLocked([\"name\"]) -> bool\n\
\n\
Returns true if is the object \"name\" locked.  If \"name\" is not given the\n\
currently selected item is used.\n\
"));
/*! Status of locking 2004/7/10 pv.*/
PyObject *scribus_islocked(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_setscaleimagetoframe__doc__,
QT_TR_NOOP("setScaleImageToFrame(scaletoframe, proportional=None, name=<selection>)\n\
\n\
Sets the scale to frame on the selected or specified image frame to `scaletoframe'.\n\
If `proportional' is specified, set fixed aspect ratio scaling to `proportional'.\n\
Both `scaletoframe' and `proportional' are boolean.\n\
\n\
May raise WrongFrameTypeError.\n\
"));
PyObject *scribus_setscaleimagetoframe(PyObject * /*self*/, PyObject* args, PyObject* kwargs);

#endif
