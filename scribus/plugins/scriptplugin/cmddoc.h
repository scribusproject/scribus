#ifndef CMDDOC_H
#define CMDDOC_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/**	Document related Commands */

/*! docstring */
PyDoc_STRVAR(scribus_newdoc__doc__,
    "newDoc()\n\n\
    Creates a new Document. The Parameters have the following Meaning:\
    see Scribus constants.\
    size = A Tuple (width, height) describing the Size of the Document.\
    margins = A Tuple (Left, Right, Top, Bottom) describing the Margins of the Document.\
    orientation = the Page Orientation, 0 means Portrait, 1 is Landscape\
    firstPageNumer is the Number of the first Page in the Document used for Pagenumbering\
    unit: this Value sets the Measurement Unit of the Document\
    \
    0 = Typographic Points\n1 = Millimeters\n2 = Inches\n3 = Picas\n\
    FacingPages: 1 means FacingPages turned on, 0 means FacingPages turned off\
    FirstSideLeft: 1 means that the first Page in the Document is a left Page,\
    0 means a right Page as first Page\n\tThe values for Width, Height and\
    the Margins are expressed in the given unit for the Document.");
/** Creates a new document e.g. (Paper_A4, Margins, 1, 1, 1, NoFacingPages, FirstPageLeft)
 first 2 args are lists (tuples) */
PyObject *scribus_newdoc(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_closedoc__doc__,
    "closeDoc() -> bool\n\n\
    Closes the current Document. Returns true if successful.");
/** Closes active doc. No params */
PyObject *scribus_closedoc(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_havedoc__doc__,
    "haveDoc() -> bool\n\n\
    Returns true if there is a Document open.");
/** Checks if is a document opened. */
PyObject *scribus_havedoc(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_opendoc__doc__,
    "openDoc(name) -> bool\n\n\
    Opens the Document \"name\". Returns true if successful.");
/** Opens a document with given name. */
PyObject *scribus_opendoc(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_savedoc__doc__,
    "saveDoc() -> bool\n\n\
    Saves the Document under its actual Name, returns true if successful.");
PyObject *scribus_savedoc(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_savedocas__doc__,
    "saveDocAs(name) -> bool\n\n\
    Saves the actual Document under the new Name \"name\". Returns true if successful.");
/** Saves active document with given name */
PyObject *scribus_savedocas(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setinfo__doc__,
    "saveDocAs(author, info, description)\n\n\
    Sets the Document Information. \"Author\", \"Info\", \"Description\"");
/** Sets document infos - author, title and description */
PyObject *scribus_setinfo(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setmargins__doc__,
    "setMargins(lr, rr, tr, br)\n\n\
    Sets the Print margins of the Document, Left(lr), Right(rr), Top(tr) and Bottom(br)\
    Margins are given in the measurement unit of the Document.");
/** Sets document margins - left, right, top and bottom. */
PyObject *scribus_setmargins(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setunit__doc__,
    "setUnit(type)\n\n\
    Changes the Measurement Unit of the Document. Possible Values for Unit are\
    defined as constants.");
/** Changes unit scale. */
PyObject *scribus_setunit(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getunit__doc__,
    "getUnit() -> Scribus typo unit\n\n\
    Returns the Measurement Unit of the Document.\nPossible Values for Unit are\
    defined as constants.");
/** Returns actual unit scale. */
PyObject *scribus_getunit(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_loadstylesfromfile__doc__,
    "loadStylesFromFile()\n\n\
    TODO: docstring");
/** Loads styles from another .sla file (craig r.)*/
PyObject *scribus_loadstylesfromfile(PyObject *self, PyObject *args);

/*! docstring */
PyDoc_STRVAR(scribus_setdoctype__doc__,
    "setDocType()\n\n\
    TODO: Sets the Type of the Documents, to get Facing Pages set the first Parameter\
    to 1, to switch FacingPages off use 0 instead. If you want to be the first Page a\
    left Side set the second Parameter to 1, for a right Page use 0.");
/*! TODO: comment */
PyObject *scribus_setdoctype(PyObject *self, PyObject* args);

#endif

