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
    "newPage()\n\n\
    Creates a new Page If \"where\" is -1 the new Page is appended\
    to the Document, otherwise the new Page is inserted at \"where\".\
    The Pagenumbers are counted from 1 upwards. The optional Parameter\
    \"template\" specifies the Name of the Template Page for the new Page.");
/*! new page */
PyObject *scribus_newpage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_actualpage__doc__,
    "currentPage()\n\n\
    Returns the Number of the current working Page. Pagenumbers are\
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
    "savePageAsEPS()\n\n\
    Saves the actual Page as an EPS, returns true if successful.");
/*! Export page as EPS file */
PyObject *scribus_savepageeps(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deletepage__doc__,
    "deletePage()\n\n\
    Deletes the given Page, does nothing if the Document contains\
    only one Page. Pagenumbers are counted from 1 upwards.");
/*! Delete page */
PyObject *scribus_deletepage(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gotopage__doc__,
    "gotoPage()\n\n\
    Moves to the Page \"nr\". If \"nr\" is outside the current\
    rage of Pages nothing happens.");
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
    "getHGuides()\n\n\
    TODO: docstring");
/*! get H guides */
PyObject *scribus_getHguides(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_setHguides__doc__,
    "getHGuides()\n\n\
    TODO: docstring");
/*! set H guides */
PyObject *scribus_setHguides(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getVguides__doc__,
    "getVGuides()\n\n\
    TODO: docstring");
/*! get V guides */
PyObject *scribus_getVguides(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_setVguides__doc__,
    "getVGuides()\n\n\
    TODO: docstring");
/*! set V guides */
PyObject *scribus_setVguides(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_pagedimension__doc__,
    "getPageSize()\n\n\
    Returns a tuple with page dimensions in used system e.g. when\
    the document's page is in picas - picas are returned");
/**
returns a tuple with page domensions in used system
e.g. when is the doc in picas returns picas ;)
(Petr Vanek 02/17/04)
*/
PyObject *scribus_pagedimension(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_getpageitems__doc__,
    "getPageItems()\n\n\
    Returns a list of tuples with items on the actual page.\
    (name, objectType, order) E.g. [('Text1', 4, 0), ('Image1', 2, 1)]\
    means that object named 'Text1' is a text frame (type 4)\
    and is the first at the page...\
    TODO: constants for types");
/**
returns a list of tuples with items on the actual page
TODO: solve utf/iso chars in object names
(Petr Vanek 03/02/2004)
*/
PyObject *scribus_getpageitems(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_getpagemargins__doc__,
    "getPageMargins()\n\n\
    TODO: docstring");
/**
returns a tuple with page margins
Craig Ringer, Petr Vanek 09/25/2004
*/
PyObject *scribus_getpagemargins(PyObject *self);

#endif

