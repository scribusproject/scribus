#ifndef CMDDOC_H
#define CMDDOC_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/** Document related Commands */

/*! docstring */
PyDoc_STRVAR(scribus_newdoc__doc__,
    "newDoc(size, margins, orientation, firstPageNumber, unit, facingPages, firstSideLeft) -> bool\n\n\
Creates a new document and returns true if successful. The parameters have the following meaning:\n\
size = A Tuple (width, height) describing the Size of the Document.\
You can use predefined cosntants named PAPER_<paper_type> e.g. PAPER_A4 etc.\n\
margins = A Tuple (Left, Right, Top, Bottom) describing the Margins of the Document.\n\
orientation = the Page Orientation - constants PORTRAIT, LANDSCAPE\n\
firstPageNumer = is the number of the first page in the document used for pagenumbering\n\
unit: this Value sets the Measurement Unit of the Document\
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS\n\
facingPages = FACINGPAGES, NOFACINGPAGES\n\
firstSideLeft = FIRSTPAGELEFT, FIRSTPAGERIGHT\n\
The values for Width, Height and the Margins are expressed in the given unit for the document.\n\n\
example: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_PICAS, FACINGPAGES, FIRSTPAGERIGHT)");
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
Returns true if there is a document open.");
/** Checks if is a document opened. */
PyObject *scribus_havedoc(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_opendoc__doc__,
    "openDoc(name) -> bool\n\n\
Opens the document \"name\". Returns true if successful.");
/** Opens a document with given name. */
PyObject *scribus_opendoc(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_savedoc__doc__,
    "saveDoc() -> bool\n\n\
Saves the document with its actual name, returns true if successful.");
PyObject *scribus_savedoc(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_savedocas__doc__,
    "saveDocAs(name) -> bool\n\n\
    Saves the actual Document under the new Name \"name\". Returns true if successful.");
/** Saves active document with given name */
PyObject *scribus_savedocas(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setinfo__doc__,
    "saveDocAs(author, info, description) -> bool\n\n\
Sets the document information. \"Author\", \"Info\", \"Description\" are strings.\
Returns true if successful.");
/** Sets document infos - author, title and description */
PyObject *scribus_setinfo(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setmargins__doc__,
    "setMargins(lr, rr, tr, br)\n\n\
Sets the print margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)\
Margins are given in the measurement unit of the document - see UNIT_<type> constants.");
/** Sets document margins - left, right, top and bottom. */
PyObject *scribus_setmargins(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setunit__doc__,
    "setUnit(type)\n\n\
Changes the Measurement Unit of the Document. Possible Values for Unit are\
defined as constants UNIT_<type>.");
/** Changes unit scale. */
PyObject *scribus_setunit(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getunit__doc__,
    "getUnit() -> Scribus typo unit\n\n\
Returns the Measurement Unit of the Document.\
Possible Values for Unit are defined as constants.\
Constants are:\n\
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.");
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
	"setDocType(facingPages, firstPageLeft)\n\n\
Sets the type of the documents, to get facing pages set the first parameter\
to FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.\
If you want to be the first page a left side set the second parameter\
to FIRSTPAGELEFT, for a right rage use FIRSTPAGERIGHT.");
/*! TODO: comment */
PyObject *scribus_setdoctype(PyObject *self, PyObject* args);

#endif

