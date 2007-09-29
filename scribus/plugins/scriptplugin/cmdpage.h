/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDPAGE_H
#define CMDPAGE_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Page related Commands */

/*! docstring */
PyDoc_STRVAR(scribus_newpage__doc__,
QT_TR_NOOP("newPage(where [,\"masterpage\"])\n\
\n\
Creates a new page. If \"where\" is -1 the new Page is appended to the\n\
document, otherwise the new page is inserted before \"where\". Page numbers are\n\
counted from 1 upwards, no matter what the displayed first page number of your\n\
document is. The optional parameter \"masterpage\" specifies the name of the\n\
master page for the new page.\n\
\n\
May raise IndexError if the page number is out of range\n\
"));
/*! new page */
PyObject *scribus_newpage(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_actualpage__doc__,
QT_TR_NOOP("currentPage() -> integer\n\
\n\
Returns the number of the current working page. Page numbers are counted from 1\n\
upwards, no matter what the displayed first page number of your document is.\n\
"));
/*! get actual page */
PyObject *scribus_actualpage(PyObject */*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_redraw__doc__,
QT_TR_NOOP("redrawAll()\n\
\n\
Redraws all pages.\n\
"));
/*! redraw all */
PyObject *scribus_redraw(PyObject */*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_pageposition__doc__,
QT_TR_NOOP("getPageType() -> integer\n\
\n\
Returns the type of the Page, 0 means left Page, 1 is a middle Page and 2 is a right Page\n\
"));
/*! Go to page */
PyObject *scribus_pageposition(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_savepageeps__doc__,
QT_TR_NOOP("savePageAsEPS(\"name\")\n\
\n\
Saves the current page as an EPS to the file \"name\".\n\
\n\
May raise ScribusError if the save failed.\n\
"));
/*! Export page as EPS file */
PyObject *scribus_savepageeps(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deletepage__doc__,
QT_TR_NOOP("deletePage(nr)\n\
\n\
Deletes the given page. Does nothing if the document contains only one page.\n\
Page numbers are counted from 1 upwards, no matter what the displayed first\n\
page number is.\n\
\n\
May raise IndexError if the page number is out of range\n\
"));
/*! Delete page */
PyObject *scribus_deletepage(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gotopage__doc__,
QT_TR_NOOP("gotoPage(nr)\n\
\n\
Moves to the page \"nr\" (that is, makes the current page \"nr\"). Note that\n\
gotoPage doesn't (currently) change the page the user's view is displaying, it\n\
just sets the page that script commands will operates on.\n\
\n\
May raise IndexError if the page number is out of range.\n\
"));
/*! Go to page */
PyObject *scribus_gotopage(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_pagecount__doc__,
QT_TR_NOOP("pageCount() -> integer\n\
\n\
Returns the number of pages in the document.\n\
"));
/*! Go to page */
PyObject *scribus_pagecount(PyObject */*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_getHguides__doc__,
QT_TR_NOOP("getHGuides() -> list\n\
\n\
Returns a list containing positions of the horizontal guides. Values are in the\n\
document's current units - see UNIT_<type> constants.\n\
"));
/*! get H guides */
PyObject *scribus_getHguides(PyObject */*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_setHguides__doc__,
QT_TR_NOOP("setHGuides(list)\n\
\n\
Sets horizontal guides. Input parameter must be a list of guide positions\n\
measured in the current document units - see UNIT_<type> constants.\n\
\n\
Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost\n\
         setHGuides([90,250]) # replace current guides entirely\n\
"));
/*! set H guides */
PyObject *scribus_setHguides(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getVguides__doc__,
QT_TR_NOOP("getVGuides()\n\
\n\
See getHGuides.\n\
"));
/*! get V guides */
PyObject *scribus_getVguides(PyObject */*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_setVguides__doc__,
QT_TR_NOOP("setVGuides()\n\
\n\
See setHGuides.\n\
"));
/*! set V guides */
PyObject *scribus_setVguides(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_pagedimension__doc__,
QT_TR_NOOP("getPageSize() -> tuple\n\
\n\
Returns a tuple with page dimensions measured in the document's current units.\n\
See UNIT_<type> constants and getPageMargins()\n\
"));
/**
returns a tuple with page domensions in used system
e.g. when is the doc in picas returns picas ;)
(Petr Vanek 02/17/04)
*/
PyObject *scribus_pagedimension(PyObject */*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_getpageitems__doc__,
QT_TR_NOOP("getPageItems() -> list\n\
\n\
Returns a list of tuples with items on the current page. The tuple is:\n\
(name, objectType, order) E.g. [('Text1', 4, 0), ('Image1', 2, 1)]\n\
means that object named 'Text1' is a text frame (type 4) and is the first at\n\
the page...\n\
"));
/**
returns a list of tuples with items on the actual page
TODO: solve utf/iso chars in object names
(Petr Vanek 03/02/2004)
*/
PyObject *scribus_getpageitems(PyObject */*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_getpagemargins__doc__,
QT_TR_NOOP("getPageMargins()\n\
\n\
Returns the page margins as a (top, left, right, bottom) tuple in the current\n\
units. See UNIT_<type> constants and getPageSize().\n\
"));
/**
returns a tuple with page margins
Craig Ringer, Petr Vanek 09/25/2004
*/
PyObject *scribus_getpagemargins(PyObject */*self*/);

#endif

