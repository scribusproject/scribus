/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDGETPROP_H
#define CMDGETPROP_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Query-Functions */

/*! docstring */
PyDoc_STRVAR(scribus_getobjecttype__doc__,
QT_TR_NOOP("getObjectType([\"name\"]) -> string\n\
\n\
Get type of object \"name\" as a string.\n\
"));
/** Get Object Type of name. */
PyObject *scribus_getobjecttype(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfillcolor__doc__,
QT_TR_NOOP("getFillColor([\"name\"]) -> string\n\
\n\
Returns the name of the fill color of the object \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Returns fill color of the object */
PyObject *scribus_getfillcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfilltrans__doc__,
QT_TR_NOOP("getFillTransparency([\"name\"]) -> float\n\
\n\
Returns the fill transparency of the object \"name\". If \"name\"\n\
is not given the currently selected Item is used.\n\
"));
/*! Returns fill transparency of the object */
PyObject *scribus_getfilltrans(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfillblend__doc__,
QT_TR_NOOP("getFillBlendmode([\"name\"]) -> integer\n\
\n\
Returns the fill blendmode of the object \"name\". If \"name\"\n\
is not given the currently selected Item is used.\n\
"));
/*! Returns fill blendmode of the object */
PyObject *scribus_getfillblend(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinecolor__doc__,
QT_TR_NOOP("getLineColor([\"name\"]) -> string\n\
\n\
Returns the name of the line color of the object \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Returns color of the line */
PyObject *scribus_getlinecolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinetrans__doc__,
QT_TR_NOOP("getLineTransparency([\"name\"]) -> float\n\
\n\
Returns the line transparency of the object \"name\". If \"name\"\n\
is not given the currently selected Item is used.\n\
"));
/*! Returns line transparency of the object */
PyObject *scribus_getlinetrans(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlineblend__doc__,
QT_TR_NOOP("getLineBlendmode([\"name\"]) -> integer\n\
\n\
Returns the line blendmode of the object \"name\". If \"name\"\n\
is not given the currently selected Item is used.\n\
"));
/*! Returns line blendmode of the object */
PyObject *scribus_getlineblend(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinewidth__doc__,
QT_TR_NOOP("getLineWidth([\"name\"]) -> integer\n\
\n\
Returns the line width of the object \"name\". If \"name\"\n\
is not given the currently selected Item is used.\n\
"));
/*! Returns width of the line */
PyObject *scribus_getlinewidth(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlineshade__doc__,
QT_TR_NOOP("getLineShade([\"name\"]) -> integer\n\
\n\
Returns the shading value of the line color of the object \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Returns shading of the line */
PyObject *scribus_getlineshade(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinejoin__doc__,
QT_TR_NOOP("getLineJoin([\"name\"]) -> integer (see constants)\n\
\n\
Returns the line join style of the object \"name\". If \"name\" is not given\n\
the currently selected item is used.  The join types are:\n\
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND\n\
"));
/*! Returns join type of the line */
PyObject *scribus_getlinejoin(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinecap__doc__,
QT_TR_NOOP("getLineCap([\"name\"]) -> integer (see constants)\n\
\n\
Returns the line cap style of the object \"name\". If \"name\" is not given the\n\
currently selected item is used. The cap types are:\n\
CAP_FLAT, CAP_ROUND, CAP_SQUARE\n\
"));
/*! Returns cap type of the line */
PyObject *scribus_getlinecap(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinestyle__doc__,
QT_TR_NOOP("getLineStyle([\"name\"]) -> integer (see constants)\n\
\n\
Returns the line style of the object \"name\". If \"name\" is not given the\n\
currently selected item is used. Line style constants are:\n\
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID\n\
"));
/*! Returns style type of the line */
PyObject *scribus_getlinestyle(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfillshade__doc__,
QT_TR_NOOP("getFillShade([\"name\"]) -> integer\n\
\n\
Returns the shading value of the fill color of the object \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Returns fill shade of the object */
PyObject *scribus_getfillshade(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcornerrad__doc__,
QT_TR_NOOP("getCornerRadius([\"name\"]) -> integer\n\
\n\
Returns the corner radius of the object \"name\". The radius is\n\
expressed in points. If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Returns corner radius of the object */
PyObject *scribus_getcornerrad(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getimgscale__doc__,
QT_TR_NOOP("getImageScale([\"name\"]) -> (x,y)\n\
\n\
Returns a (x, y) tuple containing the scaling values of the image frame\n\
\"name\".  If \"name\" is not given the currently selected item is used.\n\
"));
/*! Returns image scale of the object */
PyObject *scribus_getimgscale(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getimgname__doc__,
QT_TR_NOOP("getImageName([\"name\"]) -> string\n\
\n\
Returns the filename for the image in the image frame. If \"name\" is not\n\
given the currently selected item is used.\n\
"));
/*! Returns image name of the object */
PyObject *scribus_getimgname(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getposi__doc__,
QT_TR_NOOP("getPosition([\"name\"]) -> (x,y)\n\
\n\
Returns a (x, y) tuple with the position of the object \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
The position is expressed in the actual measurement unit of the document\n\
- see UNIT_<type> for reference.\n\
"));
/*! Returns position of the object */
PyObject *scribus_getposi(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getsize__doc__,
QT_TR_NOOP("getSize([\"name\"]) -> (width,height)\n\
\n\
Returns a (width, height) tuple with the size of the object \"name\".\n\
If \"name\" is not given the currently selected item is used. The size is\n\
expressed in the current measurement unit of the document - see UNIT_<type>\n\
for reference.\n\
"));
/*! Returns size of the object */
PyObject *scribus_getsize(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getrotation__doc__,
QT_TR_NOOP("getRotation([\"name\"]) -> integer\n\
\n\
Returns the rotation of the object \"name\". The value is expressed in degrees,\n\
and clockwise is positive. If \"name\" is not given the currently selected item\n\
is used.\n\
"));
/*! Returns rotation of the object */
PyObject *scribus_getrotation(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getallobj__doc__,
QT_TR_NOOP("getAllObjects() -> list\n\
\n\
Returns a list containing the names of all objects on the current page.\n\
"));
/*! Returns a list with all objects in page */
PyObject *scribus_getallobj(PyObject * /*self*/, PyObject* args);

#endif

