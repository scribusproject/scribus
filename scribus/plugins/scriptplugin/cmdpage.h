#ifndef CMDPAGE_H
#define CMDPAGE_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/** Page related Commands */

/*! docstring */
PyDoc_STRVAR(scribus_newpage__doc__,
    "newPage(where [,\"template\"])\n\n\
Creates a new page. If \"where\" is -1 the new Page is appended\
to the document, otherwise the new page is inserted at \"where\".\
The pagenumbers are counted from 1 upwards. The optional parameter\
\"template\" specifies the name of the template page for the new page.");
/*! new page */
PyObject *scribus_newpage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_actualpage__doc__,
    "currentPage() -> integer\n\n\
Returns the number of the current working page. Pagenumbers are\
counted from 1 upwards.");
/*! get actual page */
PyObject *scribus_actualpage(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_redraw__doc__,
    "redrawAll()\n\n\
Redraws all Pages.");
/*! redraw all */
PyObject *scribus_redraw(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_savepageeps__doc__,
    "savePageAsEPS(\"name\") -> bool\n\n\
Saves the actual page as an EPS with name, returns true if successful.");
/*! Export page as EPS file */
PyObject *scribus_savepageeps(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deletepage__doc__,
    "deletePage(nr)\n\n\
Deletes the given Page, does nothing if the Document contains\
only one Page. Pagenumbers are counted from 1 upwards.");
/*! Delete page */
PyObject *scribus_deletepage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gotopage__doc__,
    "gotoPage(nr)\n\n\
    Moves to the page \"nr\". If \"nr\" is outside the current\
rage of pages \"Page number out of range\" exception raised.");
/*! Go to page */
PyObject *scribus_gotopage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_pagecount__doc__,
    "pageCount() -> integer\n\n\
Returns the Number of Pages in the Document.");
/*! Go to page */
PyObject *scribus_pagecount(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_getHguides__doc__,
    "getHGuides() -> list\n\n\
Returns the list containing positions of the horizontal guides.\
Values are in specified typo unit - see UNIT_<type> constants.");
/*! get H guides */
PyObject *scribus_getHguides(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_setHguides__doc__,
    "setHGuides(list)\n\n\
Sets horizontal guides. Input parameter must be a list with typo\
units values - see UNIT_<type> constants.\n\
E.g.: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost");
/*! set H guides */
PyObject *scribus_setHguides(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getVguides__doc__,
    "getVGuides()\n\n\
Returns the list containing positions of the vertical guides.\
Values are in specified typo unit - see UNIT_<type> constants.");
/*! get V guides */
PyObject *scribus_getVguides(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_setVguides__doc__,
    "setVGuides()\n\n\
Sets vertical guides. Input parameter must be a list with typo\
units values - see UNIT_<type> constants.\n\
E.g.: setVGuides(getVGuides() + [200.0, 210.0] # add new guides without any lost");
/*! set V guides */
PyObject *scribus_setVguides(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_pagedimension__doc__,
    "getPageSize() -> tuple\n\n\
Returns a tuple with page dimensions in used system e.g. when\
the document's page is in picas - picas are returned. See UNIT_<type>\
constants and getPageMargins()");
/**
returns a tuple with page domensions in used system
e.g. when is the doc in picas returns picas ;)
(Petr Vanek 02/17/04)
*/
PyObject *scribus_pagedimension(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_getpageitems__doc__,
    "getPageItems() -> list\n\n\
Returns a list of tuples with items on the actual page.\
(name, objectType, order) E.g. [('Text1', 4, 0), ('Image1', 2, 1)]\
means that object named 'Text1' is a text frame (type 4)\
and is the first at the page...\
TODO: implement constants for types for item type etc.");
/**
returns a list of tuples with items on the actual page
TODO: solve utf/iso chars in object names
(Petr Vanek 03/02/2004)
*/
PyObject *scribus_getpageitems(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_getpagemargins__doc__,
    "getPageMargins()\n\n\
Returns a tuple with page margins in used system e.g. when\
the document's page is in picas - picas are returned. See UNIT_<type>\
constants and getPageSize().");
/**
returns a tuple with page margins
Craig Ringer, Petr Vanek 09/25/2004
*/
PyObject *scribus_getpagemargins(PyObject *self);

#endif

