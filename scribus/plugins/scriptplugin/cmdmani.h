#ifndef CMDMANI_H
#define CMDMANI_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/** Manipulating Objects */

/*! docstring */
PyDoc_STRVAR(scribus_moveobjrel__doc__,
    "moveObject(dx, dy [, \"name\"])\n\n\
Moves the object \"name\" by dx and dy relative to its origin.\
The distances are expressed in the actual measurement unit of\
the document (see UNIT constants). If \"name\" is not given\
the currently selected item is used. If the object \"name\"\
belongs to a group, the whole group is moved.");
/*! Move REL the object */
PyObject *scribus_moveobjrel(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_moveobjabs__doc__,
    "moveObjectAbs(x, y [, \"name\"])\n\n\
Moves the object \"name\" to a new location. The coordinates are\
expressed in the actual measurement unit of the document (see UNIT constants).\
If \"name\" is not given the currently selected item is used.\
If the object \"name\" belongs to a group, the whole group is moved.");
/*! Move ABS the object */
PyObject *scribus_moveobjabs(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_rotobjrel__doc__,
    "rotateObject(rot [, \"name\"])\n\n\
Rotates the object \"name\" by \"rot\" degrees relatively. Positve values mean\
counter clockwise rotation. If \"name\" is not given the currently\
selected Item is used.");
/*! Rotate REL the object */
PyObject *scribus_rotobjrel(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_rotobjabs__doc__,
    "rotateObjectAbs(rot [, \"name\"])\n\n\
Sets the rotation of the object \"name\" to \"rot\". Positve values\
mean counter clockwise rotation. If \"name\" is not given the currently\
selected item is used.");
/*! Rotate ABS the object */
PyObject *scribus_rotobjabs(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_sizeobjabs__doc__,
    "sizeObject(width, height [, \"name\"])\n\n\
Resizes the object \"name\" to the given width and height. If \"name\"\
is not given the currently selected item is used.");
/*! Resize ABS the object */
PyObject *scribus_sizeobjabs(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getselobjnam__doc__,
    "getSelectedObject([nr]) -> string\n\n\
Returns the name of the selected object. \"nr\" if given indicates\
the number of the selected object, e.g. 0 means the first selected object,\
1 means the second selected Object and so on.");
/*! Returns name of the selected object */
PyObject *scribus_getselobjnam(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_selcount__doc__,
    "selectionCount() -> integer\n\n\
Returns the number of selected objects.");
/*! Returns count of the selected object */
PyObject *scribus_selcount(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_selectobj__doc__,
    "selectObject(\"name\")\n\n\
Selects the object with the given \"name\".");
/*! Count selection */
PyObject *scribus_selectobj(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deselect__doc__,
    "deselectAll()\n\n\
Deselects all objects in the whole document.");
/*! Remove all selection */
PyObject *scribus_deselect(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_groupobj__doc__,
    "groupObjects(list)\n\n\
Groups the objects named in \"list\" together. \"list\" must contain\
the names of the objects to be grouped. If \"list\" is\
not given the currently selected items are used.");
/*! Group objects named in list. */
PyObject *scribus_groupobj(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_ungroupobj__doc__,
    "unGroupObjects(\"name\")\n\n\
Destructs the group the object \"name\" belongs to.\
If \"name\" is not given the currently selected item is used.");
/*! Ungroup objects named in list. */
PyObject *scribus_ungroupobj(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_scalegroup__doc__,
    "scaleGroup(factor [,\"name\"])\n\n\
Scales the group the object \"name\" belongs to. Values greater\
than 1 enlarge the group, values smaller than 1 make the group\
smaller e.g a value of 0.5 scales the group to 50 % of its original\
size, a value of 1.5 scales the group to 150 % of its original size.\
The value for \"factor\" must be greater than 0. If \"name\"\
is not given the currently selected item is used.");
/*! Scale group with object name */
PyObject *scribus_scalegroup(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_loadimage__doc__,
    "loadImage(\"filename\" [, \"name\"])\n\n\
Loads the picture \"picture\" into the image frame \"name\".\
If \"name\" is not given the currently selected item is used.");
/*! Loads image file into frame. */
PyObject *scribus_loadimage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_scaleimage__doc__,
    "scaleImage(x, y [, \"name\"])\n\n\
Sets the scaling factors of the picture in the image frame \"name\".\
If \"name\" is not given the currently selected item is used.\
A Number of 1 means 100 %.");
/*! Scale Image. */
PyObject *scribus_scaleimage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_lockobject__doc__,
    "lockObject([\"name\"]) -> bool\n\n\
Locks the object \"name\" if it's unlocked or unlock it if it's locked.\
If \"name\" is not given the currently selected item is used.\
Returns true if locked.");
/*! (Un)Lock the object 2004/7/10 pv.*/
PyObject *scribus_lockobject(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_islocked__doc__,
    "isLocked([\"name\"]) -> bool\n\n\
Returns true if is the object \"name\" locked.\
If \"name\" is not given the currently selected item is used.");
/*! Status of locking 2004/7/10 pv.*/
PyObject *scribus_islocked(PyObject *self, PyObject* args);

#endif

