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
    "moveObject([objectName])\n\n\
    Moves the Object \"name\" by dx and dy relative to its origin.\
    The Distances are expressed in the actual Measurement Unit of\
    the Document. If \"name\" is not given the currently selected Item\
    is used. If the Object \"name\" belongs to a Group, the whole\
    Group is moved.");
/*! Move REL the object */
PyObject *scribus_moveobjrel(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_moveobjabs__doc__,
    "moveObjectAbs([objectName])\n\n\
    Moves the Object \"name\" to a new Location. The Coordinates are\
    expressed in the actual Measurement Unit of the Document.\
    If \"name\" is not given the currently selected Item is used.\
    If the Object \"name\" belongs to a Group, the whole Group is moved.");
/*! Move ABS the object */
PyObject *scribus_moveobjabs(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_rotobjrel__doc__,
    "rotateObject([objectName])\n\n\
    Rotates the Object \"name\" by \"rot\" Degrees. Positve Values mean\
    counter clockwise Rotation. If \"name\" is not given the currently\
    selected Item is used.");
/*! Rotate REL the object */
PyObject *scribus_rotobjrel(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_rotobjabs__doc__,
    "rotateObjectAbs([objectName])\n\n\
    Sets the Rotation of the Object \"name\" to \"rot\". Positve Values\
    mean counter clockwise Rotation. If \"name\" is not given the currently\
    selected Item is used.");
/*! Rotate ABS the object */
PyObject *scribus_rotobjabs(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_sizeobjabs__doc__,
    "sizeObject([objectName])\n\n\
    Resizes the Object \"name\" to the given Width and Height. If \"name\"\
    is not given the currently selected Item is used.");
/*! Resize ABS the object */
PyObject *scribus_sizeobjabs(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getselobjnam__doc__,
    "getSelectedObject([number]) -> string\n\n\
    Returns the Name of the selecteted Object. \"nr\" if given indicates\
    the Number of the selected Object, e.g. 0 means the first selected Object,\
    1 means the second selected Object and so on.");
/*! Returns name of the selected object */
PyObject *scribus_getselobjnam(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_selcount__doc__,
    "selectionCount() -> integer\n\n\
    Returns the Number of selected Objects.");
/*! Returns count of the selected object */
PyObject *scribus_selcount(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_selectobj__doc__,
    "selectObject(name)\n\n\
    Selects the Object with the given Name.");
/*! Count selection */
PyObject *scribus_selectobj(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deselect__doc__,
    "deselectAll()\n\n\
    Deselects all Objects.");
/*! Remove all selection */
PyObject *scribus_deselect(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_groupobj__doc__,
    "groupObjects(list)\n\n\
    Groups the Objects in \"list\" together. \"list\" must contain\
    the Names of the Objects to be grouped. If \"list\" is\
    not given the currently selected Items are used.");
/*! Group objects named in list. */
PyObject *scribus_groupobj(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_ungroupobj__doc__,
    "unGroupObjects(name)\n\n\
    Destructs the Group the Object \"name\" belongs to.\
    If \"name\" is not given the currently selected Item is used.");
/*! Ungroup objects named in list. */
PyObject *scribus_ungroupobj(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_scalegroup__doc__,
    "scaleGroup(name, float)\n\n\
    Scales the Group the Object \"name\" belongs to. Values greater\
    than 1 enlarge the Group, Values smaller than 1 make the Group\
    smaller e.g a Value of 0.5 scales the Group to 50 % of is original\
    Size, a Value of 1.5 scales the Group to 150 % of its original Size.\
    The Value for \"factor\" must be greater than 0. If \"name\"\
    is not given the currently selected Item is used.");
/*! Scale group with object name */
PyObject *scribus_scalegroup(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_loadimage__doc__,
    "loadImage(name)\n\n\
    Loads the Picture \"picture\" into the Image Frame \"name\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Loads image file into frame. */
PyObject *scribus_loadimage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_scaleimage__doc__,
    "scaleImage(name, number)\n\n\
    Sets the Scaling Factors of the Picture in the Image Frame \"name\".\
    If \"name\" is not given the currently selected Item is used.\
    A Number of 1 means 100 %.");
/*! Scale Image. */
PyObject *scribus_scaleimage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_lockobject__doc__,
    "lockObject()\n\n\
    TODO: docstring");
/*! (Un)Lock the object 2004/7/10 pv.*/
PyObject *scribus_lockobject(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_islocked__doc__,
    "isLocked()\n\n\
    TODO: docstring");
/*! (Un)Lock the object 2004/7/10 pv.*/
PyObject *scribus_islocked(PyObject *self, PyObject* args);

#endif

