/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDDOC_H
#define CMDDOC_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Document related Commands */

PyDoc_STRVAR(scribus_newdocument__doc__,
QT_TR_NOOP("newDocument(size, margins, orientation, firstPageNumber,\n\
                        unit, pagesType, firstPageOrder, numPages) -> bool\n\
\n\
Creates a new document and returns true if successful. The parameters have the\n\
following meaning:\n\
\n\
size = A tuple (width, height) describing the size of the document. You can\n\
use predefined constants named PAPER_<paper_type> e.g. PAPER_A4 etc.\n\
\n\
margins = A tuple (left, right, top, bottom) describing the document\n\
margins\n\
\n\
orientation = the page orientation - constants PORTRAIT, LANDSCAPE\n\
\n\
firstPageNumer = is the number of the first page in the document used for\n\
pagenumbering. While you'll usually want 1, it's useful to have higher\n\
numbers if you're creating a document in several parts.\n\
\n\
unit: this value sets the measurement units used by the document. Use a\n\
predefined constant for this, one of: UNIT_INCHES, UNIT_MILLIMETERS,\n\
UNIT_PICAS, UNIT_POINTS.\n\
\n\
pagesType = One of the predefined constants PAGE_n. PAGE_1 is single page,\n\
PAGE_2 is for double sided documents, PAGE_3 is for 3 pages fold and\n\
PAGE_4 is 4-fold.\n\
\n\
firstPageOrder = What is position of first page in the document.\n\
Indexed from 0 (0 = first).\n\
\n\
numPage = Number of pages to be created.\n\
\n\
The values for width, height and the margins are expressed in the given unit\n\
for the document. PAPER_* constants are expressed in points. If your document\n\
is not in points, make sure to account for this.\n\
\n\
example: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,\n\
PAGE_4, 3, 1)\n\
\n\
May raise ScribusError if is firstPageOrder bigger than allowed by pagesType.\n\
"));
/** Creates a new document e.g. (Paper_A4, Margins, 1, 1, 1, NoFacingPages, FirstPageLeft)
 first 2 args are lists (tuples) */
PyObject *scribus_newdocument(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newdoc__doc__,
QT_TR_NOOP("newDoc(size, margins, orientation, firstPageNumber,\n\
                   unit, facingPages, firstSideLeft) -> bool\n\
\n\
WARNING: Obsolete procedure! Use newDocument instead.\n\
\n\
Creates a new document and returns true if successful. The parameters have the\n\
following meaning:\n\
\n\
    size = A tuple (width, height) describing the size of the document. You can\n\
    use predefined constants named PAPER_<paper_type> e.g. PAPER_A4 etc.\n\
\n\
    margins = A tuple (left, right, top, bottom) describing the document\n\
    margins\n\
\n\
    orientation = the page orientation - constants PORTRAIT, LANDSCAPE\n\
\n\
    firstPageNumer = is the number of the first page in the document used for\n\
    pagenumbering. While you'll usually want 1, it's useful to have higher\n\
    numbers if you're creating a document in several parts.\n\
\n\
    unit: this value sets the measurement units used by the document. Use a\n\
    predefined constant for this, one of: UNIT_INCHES, UNIT_MILLIMETERS,\n\
    UNIT_PICAS, UNIT_POINTS.\n\
\n\
    facingPages = FACINGPAGES, NOFACINGPAGES\n\
\n\
    firstSideLeft = FIRSTPAGELEFT, FIRSTPAGERIGHT\n\
\n\
The values for width, height and the margins are expressed in the given unit\n\
for the document. PAPER_* constants are expressed in points. If your document\n\
is not in points, make sure to account for this.\n\
\n\
example: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,\n\
                FACINGPAGES, FIRSTPAGERIGHT)\n\
"));
/** Creates a new document e.g. (Paper_A4, Margins, 1, 1, 1, NoFacingPages, FirstPageLeft)
 first 2 args are lists (tuples) */
PyObject *scribus_newdoc(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_closedoc__doc__,
QT_TR_NOOP("closeDoc()\n\
\n\
Closes the current document without prompting to save.\n\
\n\
May throw NoDocOpenError if there is no document to close\n\
"));
/** Closes active doc. No params */
PyObject *scribus_closedoc(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_havedoc__doc__,
QT_TR_NOOP("haveDoc() -> int\n\
\n\
Returns the quantity of open documents: 0 if none are opened.\n\
"));
/** Checks if is a document opened. */
PyObject *scribus_havedoc(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_opendoc__doc__,
QT_TR_NOOP("openDoc(\"name\")\n\
\n\
Opens the document \"name\".\n\
\n\
May raise ScribusError if the document could not be opened.\n\
"));
/** Opens a document with given name. */
PyObject *scribus_opendoc(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_savedoc__doc__,
QT_TR_NOOP("saveDoc()\n\
\n\
Saves the current document with its current name, returns true if successful.\n\
If the document has not already been saved, this may bring up an interactive\n\
save file dialog.\n\
\n\
If the save fails, there is currently no way to tell.\n\
"));
PyObject *scribus_savedoc(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_getdocname__doc__,
QT_TR_NOOP("getDocName() -> string\n\
\n\
Returns the name the document was saved under.\n\
If the document was not saved before the name is empty.\n\
"));
/** Saves active document with given name */
PyObject *scribus_getdocname(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_savedocas__doc__,
QT_TR_NOOP("saveDocAs(\"name\")\n\
\n\
Saves the current document under the new name \"name\" (which may be a full or\n\
relative path).\n\
\n\
May raise ScribusError if the save fails.\n\
"));
/** Saves active document with given name */
PyObject *scribus_savedocas(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setinfo__doc__,
QT_TR_NOOP("setInfo(\"author\", \"info\", \"description\") -> bool\n\
\n\
Sets the document information. \"Author\", \"Info\", \"Description\" are\n\
strings.\n\
"));
/** Sets document infos - author, title and description */
PyObject *scribus_setinfo(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setmargins__doc__,
QT_TR_NOOP("setMargins(lr, rr, tr, br)\n\
\n\
Sets the margins of the document, Qt::DockLeft(lr), Qt::DockRight(rr), Qt::DockTop(tr) and Qt::DockBottom(br)\n\
margins are given in the measurement units of the document - see UNIT_<type>\n\
constants.\n\
"));
/** Sets document margins - left, right, top and bottom. */
PyObject *scribus_setmargins(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setunit__doc__,
QT_TR_NOOP("setUnit(type)\n\
\n\
Changes the measurement unit of the document. Possible values for \"unit\" are\n\
defined as constants UNIT_<type>.\n\
\n\
May raise ValueError if an invalid unit is passed.\n\
"));
/** Changes unit scale. */
PyObject *scribus_setunit(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getunit__doc__,
QT_TR_NOOP("getUnit() -> integer (Scribus unit constant)\n\
\n\
Returns the measurement units of the document. The returned value will be one\n\
of the UNIT_* constants:\n\
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.\n\
"));
/** Returns actual unit scale. */
PyObject *scribus_getunit(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_loadstylesfromfile__doc__,
QT_TR_NOOP("loadStylesFromFile(\"filename\")\n\
\n\
Loads paragraph styles from the Scribus document at \"filename\" into the\n\
current document.\n\
"));
/** Loads styles from another .sla file (craig r.)*/
PyObject *scribus_loadstylesfromfile(PyObject * /*self*/, PyObject *args);

/*! docstring */
PyDoc_STRVAR(scribus_setdoctype__doc__,
QT_TR_NOOP("setDocType(facingPages, firstPageLeft)\n\
\n\
Sets the document type. To get facing pages set the first parameter to\n\
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want\n\
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for\n\
a right page use FIRSTPAGERIGHT.\n\
"));
PyObject *scribus_setdoctype(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_closemasterpage__doc__,
QT_TR_NOOP("closeMasterPage()\n\
\n\
Closes the currently active master page, if any, and returns editing\n\
to normal. Begin editing with editMasterPage().\n\
"));
PyObject* scribus_closemasterpage(PyObject* self);

PyDoc_STRVAR(scribus_masterpagenames__doc__,
QT_TR_NOOP("masterPageNames()\n\
\n\
Returns a list of the names of all master pages in the document.\n\
"));
PyObject* scribus_masterpagenames(PyObject* self);

PyDoc_STRVAR(scribus_editmasterpage__doc__,
QT_TR_NOOP("editMasterPage(pageName)\n\
\n\
Enables master page editing and opens the named master page\n\
for editing. Finish editing with closeMasterPage().\n\
"));
PyObject* scribus_editmasterpage(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_createmasterpage__doc__,
QT_TR_NOOP("createMasterPage(pageName)\n\
\n\
Creates a new master page named pageName and opens it for\n\
editing.\n\
"));
PyObject* scribus_createmasterpage(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_deletemasterpage__doc__,
QT_TR_NOOP("deleteMasterPage(pageName)\n\
\n\
Delete the named master page.\n\
"));
PyObject* scribus_deletemasterpage(PyObject* self, PyObject* args);

#endif

