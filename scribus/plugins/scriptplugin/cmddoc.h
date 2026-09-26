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
                        unit, pagesType, firstPageOrder, numPages, bindingDirection) -> bool\n\
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
PAGE_2 is for facing pages documents, PAGE_3 is for 3 pages fold and\n\
PAGE_4 is 4-fold.\n\
\n\
firstPageOrder = What is position of first page in the document.\n\
Indexed from 0 (0 = first).\n\
\n\
numPages = Number of pages to be created.\n\
\n\
bindingDirection = 0 = LTR, 1 = RTL (optional, defaults to 0).\n\
\n\
The values for width, height and the margins are expressed in the given unit\n\
for the document. PAPER_* constants are expressed in points. If your document\n\
is not in points, make sure to account for this.\n\
Use UNIT_MM if you use PAPER_A*_MM or PAPER_B*_MM constants. PAPER_A0_MM through PAPER_A9_MM\n\
and PAPER_B0_MM through PAPER_B10_MM are available.\n\
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
PyDoc_STRVAR(scribus_revertdoc__doc__,
QT_TR_NOOP("revertDoc()\n\
\n\
Revert the current document to its last saved state.\n\
"));
PyObject *scribus_revertdoc(PyObject * /*self*/);

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
PyDoc_STRVAR(scribus_getinfo__doc__,
QT_TR_NOOP("getInfo() -> \"author\", \"info\", \"description\"\n\
\n\
Gets the document information. \"Author\", \"Info\", \"Description\" are\n\
strings.\n\
"));
/** Gets document infos - author, title and description */
PyObject *scribus_getinfo(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_getbleeds__doc__,
QT_TR_NOOP("getBleeds() -> (lr, rr, tr, br)\n\
\n\
Gets the bleeds of the document. Left(lr), Right(rr), Top(tr) and Bottom(br)\n\
bleeds are given in the measurement units of the document - see UNIT_<type>\n\
constants.\n\
"));
/** Sets document bleeds - left, right, top and bottom. */
PyObject *scribus_getbleeds(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setbleeds__doc__,
QT_TR_NOOP("setBleeds(lr, rr, tr, br)\n\
\n\
Sets the bleeds of the document. Left(lr), Right(rr), Top(tr) and Bottom(br)\n\
bleeds are given in the measurement units of the document - see UNIT_<type>\n\
constants.\n\
"));
/** Sets document bleeds - left, right, top and bottom. */
PyObject *scribus_setbleeds(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getmargins__doc__,
QT_TR_NOOP("getMargins() -> (lr, rr, tr, br)\n\
\n\
Gets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)\n\
margins are given in the measurement units of the document - see UNIT_<type>\n\
constants.\n\
"));
/** Sets document margins - left, right, top and bottom. */
PyObject *scribus_getmargins(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setmargins__doc__,
QT_TR_NOOP("setMargins(lr, rr, tr, br)\n\
\n\
Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)\n\
margins are given in the measurement units of the document - see UNIT_<type>\n\
constants.\n\
"));
/** Sets document margins - left, right, top and bottom. */
PyObject *scribus_setmargins(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getbaseline__doc__,
    QT_TR_NOOP("getBaseLine() -> (grid, offset)\n\
\n\
Gets the base line settings of the document, grid spacing(grid), grid offset(offset).\n\
Values are given in the measurement units of the document - see UNIT_<type>\n\
constants.\n\
"));
/** Sets document baseline settings - grid and offset. */
PyObject* scribus_getbaseline(PyObject* /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_setbaseline__doc__,
QT_TR_NOOP("setBaseLine(grid, offset)\n\
\n\
Sets the base line settings of the document, grid spacing(grid), grid offset(offset).\n\
Values are given in the measurement units of the document - see UNIT_<type>\n\
constants.\n\
"));
/** Sets document baseline settings - grid and offset. */
PyObject *scribus_setbaseline(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setunit__doc__,
QT_TR_NOOP("setUnit(type)\n\
\n\
Changes the measurement unit of the document. Possible values for \"unit\" are\n\
defined as constants UNIT_<type>.\n\
\n\
May raise ValueError if an invalid unit is passed.\n\
"));
/** Changes the document unit. */
PyObject *scribus_setunit(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getunit__doc__,
QT_TR_NOOP("getUnit() -> integer (Scribus unit constant)\n\
\n\
Returns the measurement units of the document. The returned value will be one\n\
of the UNIT_* constants:\n\
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.\n\
"));
/** Returns actual document unit. */
PyObject *scribus_getunit(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_pointstodocunit__doc__,
QT_TR_NOOP("pointsToDocUnit(points) -> value\n\
\n\
Returns a value in the measurement units of the document converted from points.\n\
"));
/** Converts from points to the document unit. */
PyObject *scribus_pointstodocunit(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_docunittopoints__doc__,
QT_TR_NOOP("docUnitToPoints(value) -> points\n\
\n\
Returns a value in points converted from the measurement units of the document.\n\
"));
/** Converts from the document unit to points. */
PyObject *scribus_docunittopoints(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_stringvaluetopoints__doc__,
QT_TR_NOOP("stringValueToPoints(\"10mm\") -> points\n\
\n\
Returns a value in points converted from a string value (\"5mm\", \"2in\" et.c.).\n\
"));
/** Converts a string value ("5mm", "2in" et.c.) to points. */
PyObject *scribus_stringvaluetopoints(PyObject * /*self*/, PyObject *args);

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

PyDoc_STRVAR(scribus_createfacingmasterpair__doc__,
QT_TR_NOOP("createFacingMasterPair(leftPageName, rightPageName)\n\
\n\
Creates coordinated left and right master pages in a facing-page document.\n\
The two created master-page names are returned as a tuple.\n\
"));
PyObject* scribus_createfacingmasterpair(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_deletemasterpage__doc__,
QT_TR_NOOP("deleteMasterPage(pageName)\n\
\n\
Delete the named master page.\n\
"));
PyObject* scribus_deletemasterpage(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_getmasterpage__doc__,
QT_TR_NOOP("getMasterPage(pageNr)\n\
\n\
Returns the name of master page applied to page \"nr\".\n\
\n\
May raise IndexError if the page number is out of range.\n\
"));
/*! Get Master Page Name */
PyObject *scribus_getmasterpage(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_applymasterpage__doc__,
QT_TR_NOOP("applyMasterPage(masterPageName, pageNumber)\n\
\n\
Apply master page masterPageName on page pageNumber.\n\
"));
PyObject* scribus_applymasterpage(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_exportdocumentcheck__doc__,
QT_TR_NOOP("exportDocumentCheck([jsonFilePath, checkProfileName=\"\", nonPrintingLayers=False])\n\
\n\
Export the result of the preflight verifier into a JSON string or a JSON file.\n\
\n\
jsonFilePath: the path to the json file to be written. If empty, a JSON string \n\
is returned.\n\
checkProfileName is the name of an existing preflight verifier profile.\n\
\n\
The resulting JSON always has five sections:\n\
- freeItems: a list of page items that are in no page;\n\
- layers: a list of errors in layers;\n\
- marks: errors relating to the marks;\n\
- masterPages: list of erroneous page items for each master page;\n\
- pages: list of erroneous page items for each page.\n\
\n\
The errors are strings, mostly names from the PreflightError enum in scribusstruct.h:\n\
- MissingGlyph\n\
- TextOverflow\n\
- ObjectNotOnPage\n\
- MissingImage\n\
- ImageDPITooLow\n\
- Transparency\n\
- PDFAnnotField\n\
- PlacedPDF\n\
- ImageDPITooHigh\n\
- ImageIsGIF\n\
- BlendMode\n\
- WrongFontInAnnotation\n\
- NotCMYKOrSpot\n\
- DeviceColorsAndOutputIntent\n\
- FontNotEmbedded\n\
- EmbeddedFontIsOpenType\n\
- OffConflictLayers\n\
- PartFilledImageFrame\n\
- MarksChanged\n\
- AppliedMasterDifferentSide\n\
- EmptyTextFrame\n\
- ImageHasProgressiveEncoding\n\
- MissingStyle\n\
- BrokenCrossReference\n\
- DocumentModifiedAfterMarksUpdate\n\
"));
PyObject* scribus_exportdocumentcheck(PyObject* self, PyObject* args, PyObject* kw);


PyDoc_STRVAR(scribus_getrtl__doc__,
QT_TR_NOOP("getRTL() -> bool\n\
\n\
Returns whether the current document is right-to-left.\n\
"));
PyObject *scribus_getrtl(PyObject * /*self*/);

PyDoc_STRVAR(scribus_setrtl__doc__,
QT_TR_NOOP("setRTL(rtl)\n\
\n\
Sets whether the current document is right-to-left. \"rtl\" is a boolean.\n\
"));
PyObject *scribus_setrtl(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_createcrossreferencetarget__doc__,
QT_TR_NOOP("createCrossReferenceTarget(name, [objectName, position=-1]) -> str\n\nCreates a named cross-reference target in a text frame and returns its name. A position of -1 appends it."));
PyObject *scribus_createcrossreferencetarget(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_deletecrossreferencetarget__doc__,
QT_TR_NOOP("deleteCrossReferenceTarget(target)\n\nDeletes a named target. Existing page-reference fields are preserved and reported by Preflight until they are repaired or deleted."));
PyObject *scribus_deletecrossreferencetarget(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_insertcrossreference__doc__,
QT_TR_NOOP("insertCrossReference(target, [objectName, position=-1, label='', format='page', prefix='', suffix='']) -> str\n\nInserts a dynamic reference to a named target and returns the reference label. Format is 'page' or 'paragraph'. A position of -1 appends it."));
PyObject *scribus_insertcrossreference(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_gotocrossreferencetarget__doc__,
QT_TR_NOOP("goToCrossReferenceTarget(reference)\n\nNavigates to the target of a cross-reference, selects its text frame, and places the text cursor at the target."));
PyObject *scribus_gotocrossreferencetarget(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_getcrossreferencetext__doc__,
QT_TR_NOOP("getCrossReferenceText(target) -> str\n\nReturns the current paragraph text containing a named cross-reference target."));
PyObject *scribus_getcrossreferencetext(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_getcrossreferencepage__doc__,
QT_TR_NOOP("getCrossReferencePage(target) -> str\n\nReturns the current section-formatted page number for a named cross-reference target."));
PyObject *scribus_getcrossreferencepage(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_listcrossreferencetargets__doc__,
QT_TR_NOOP("listCrossReferenceTargets() -> list\n\nReturns the names of all cross-reference targets in the document."));
PyObject *scribus_listcrossreferencetargets(PyObject *self);

PyDoc_STRVAR(scribus_renamecrossreferencetarget__doc__,
QT_TR_NOOP("renameCrossReferenceTarget(target, newName)\n\nRenames a cross-reference target and safely retargets every page reference that points to it."));
PyObject *scribus_renamecrossreferencetarget(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_createvariable__doc__,
QT_TR_NOOP("createVariable(name, value) -> str\n\nCreates a user-defined dynamic variable and returns its stable ID."));
PyObject *scribus_createvariable(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_createrunningheadervariable__doc__,
QT_TR_NOOP("createRunningHeaderVariable(name, paragraphStyle, mode, [textCase='as-entered', removeTrailingPunctuation=False, fallback='none']) -> str\n\nCreates a running-header variable and returns its stable ID. Mode must be 'first-on-page', 'last-on-page', 'first-on-spread', 'last-on-spread', or 'most-recent'. Text case may be 'as-entered', 'uppercase', 'lowercase', or 'title-case'. For page and spread modes, fallback may be 'none', 'section', or 'document'."));
PyObject *scribus_createrunningheadervariable(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_deletevariable__doc__,
QT_TR_NOOP("deleteVariable(variable)\n\nDeletes a user-defined dynamic variable identified by name or stable ID."));
PyObject *scribus_deletevariable(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_getvariable__doc__,
QT_TR_NOOP("getVariable(variable, [objectName]) -> str\n\nReturns the resolved value of a dynamic variable. The argument may be a name, stable ID, or built-in type. Supply a page item name for page-sensitive variables such as current page and running headers."));
PyObject *scribus_getvariable(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_insertvariable__doc__,
QT_TR_NOOP("insertVariable(variable, [objectName, position=-1]) -> str\n\nInserts a dynamic variable in a text frame and returns its stable ID. A position of -1 appends it."));
PyObject *scribus_insertvariable(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_listvariables__doc__,
QT_TR_NOOP("listVariables() -> list\n\nReturns user-defined variables as (stable ID, name, value) tuples."));
PyObject *scribus_listvariables(PyObject *self);

PyDoc_STRVAR(scribus_renamevariable__doc__,
QT_TR_NOOP("renameVariable(variable, newName)\n\nRenames a user-defined dynamic variable identified by name or stable ID."));
PyObject *scribus_renamevariable(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_setvariable__doc__,
QT_TR_NOOP("setVariable(variable, value)\n\nChanges a user-defined dynamic variable identified by name or stable ID."));
PyObject *scribus_setvariable(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_applydatarecord__doc__,
QT_TR_NOOP("applyDataRecord(record, [strict=True]) -> int\n\nApplies a dictionary of string fields to existing user-defined dynamic variables by name or stable ID. Validates the whole record before changing the document, groups changes into one undo step, and returns the number of matched fields. With strict=False, unknown fields are ignored; computed variables remain read-only."));
PyObject *scribus_applydatarecord(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_loaddatasource__doc__,
QT_TR_NOOP("loadDataSource(path, [format='', limit=-1]) -> list\n\nReads a UTF-8 CSV or JSON data source and returns a list of dictionaries containing string fields. Format is inferred from the filename when omitted; JSON must be an array of objects, and CSV must have a unique, rectangular header. Limit restricts the number of records returned; use -1 for all records."));
PyObject *scribus_loaddatasource(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_exportdatamergepdfs__doc__,
QT_TR_NOOP("exportDataMergePDFs(sourcePath, outputDirectory, [mapping=None, prefix='', firstRecord=1, lastRecord=-1, filenameField='', failOnPreflight=False]) -> list\n\nExports one PDF per selected CSV/JSON record using the active document's PDF settings and returns the created paths. Mapping pairs source-field names with user-variable names or stable IDs; by default, matching names are mapped automatically. firstRecord and lastRecord are inclusive, one-based positions; -1 means through the final record. filenameField optionally appends a sanitized source value to each numbered PDF name. When failOnPreflight is True, critical errors stop export before that record's PDF. Existing files are not overwritten. The document's variable values, PDF settings, and modified state are restored, including after an export error."));
PyObject *scribus_exportdatamergepdfs(PyObject *self, PyObject* args);

PyDoc_STRVAR(scribus_setrunningheadervariable__doc__,
QT_TR_NOOP("setRunningHeaderVariable(variable, name, paragraphStyle, mode, [textCase, removeTrailingPunctuation, fallback])\n\nUpdates a running-header variable identified by name or stable ID. Omitted formatting and fallback options retain their current values. For page and spread modes, fallback may be 'none', 'section', or 'document'."));
PyObject *scribus_setrunningheadervariable(PyObject *self, PyObject* args);

#endif
