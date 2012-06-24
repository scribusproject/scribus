/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDSETPROP_H
#define CMDSETPROP_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Setting Object Properties */

/*! docstring */
PyDoc_STRVAR(scribus_setgradfill__doc__,
QT_TR_NOOP("setGradientFill(type, \"color1\", shade1, \"color2\", shade2, [\"name\"])\n\
\n\
Sets the gradient fill of the object \"name\" to type. Color descriptions are\n\
the same as for setFillColor() and setFillShade(). See the constants for\n\
available types (FILL_<type>).\n\
"));
/*! Set gradient */
PyObject *scribus_setgradfill(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setgradstop__doc__,
QT_TR_NOOP("setGradientStop(\"color\", shade, opacity, ramppoint, [\"name\"])\n\
\n\
Set or add a gradient stop to the gradient fill of the object \"name\" at position ramppoint.\n\
Color descriptions are the same as for setFillColor() and setFillShade(). setGradientFill()\n\
must have been called previously for the gradient fill to be visible.\n\
"));
/*! Set gradient */
PyObject *scribus_setgradstop(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfillcolor__doc__,
QT_TR_NOOP("setFillColor(\"color\", [\"name\"])\n\
\n\
Sets the fill color of the object \"name\" to the color \"color\". \"color\"\n\
is the name of one of the defined colors. If \"name\" is not given the\n\
currently selected item is used.\n\
"));
/*! Set fill color */
PyObject *scribus_setfillcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfilltrans__doc__,
QT_TR_NOOP("setFillTransparency(transparency, [\"name\"])\n\
\n\
Sets the fill transparency of the object \"name\" to transparency\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Set fill transparency */
PyObject *scribus_setfilltrans(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfillblend__doc__,
QT_TR_NOOP("setFillBlendmode(blendmode, [\"name\"])\n\
\n\
Sets the fill blendmode of the object \"name\" to blendmode\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Set fill blendmode */
PyObject *scribus_setfillblend(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinecolor__doc__,
QT_TR_NOOP("setLineColor(\"color\", [\"name\"])\n\
\n\
Sets the line color of the object \"name\" to the color \"color\". If \"name\"\n\
is not given the currently selected item is used.\n\
"));
/*! Set line color */
PyObject *scribus_setlinecolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinetrans__doc__,
QT_TR_NOOP("setLineTransparency(transparency, [\"name\"])\n\
\n\
Sets the line transparency of the object \"name\" to transparency\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Set line transparency */
PyObject *scribus_setlinetrans(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlineblend__doc__,
QT_TR_NOOP("setLineBlendmode(blendmode, [\"name\"])\n\
\n\
Sets the line blendmode of the object \"name\" to blendmode\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Set line blendmode */
PyObject *scribus_setlineblend(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinewidth__doc__,
QT_TR_NOOP("setLineWidth(width, [\"name\"])\n\
\n\
Sets line width of the object \"name\" to \"width\". \"width\" must be in the\n\
range from 0.0 to 12.0 inclusive, and is measured in points. If \"name\" is not\n\
given the currently selected item is used.\n\
\n\
May raise ValueError if the line width is out of bounds.\n\
"));
/*! Set line width */
PyObject *scribus_setlinewidth(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlineshade__doc__,
QT_TR_NOOP("setLineShade(shade, [\"name\"])\n\
\n\
Sets the shading of the line color of the object \"name\" to \"shade\".\n\
\"shade\" must be an integer value in the range from 0 (lightest) to 100\n\
(full color intensity). If \"name\" is not given the currently selected item\n\
is used.\n\
\n\
May raise ValueError if the line shade is out of bounds.\n\
"));
/*! Set line shade */
PyObject *scribus_setlineshade(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinejoin__doc__,
QT_TR_NOOP("setLineJoin(join, [\"name\"])\n\
\n\
Sets the line join style of the object \"name\" to the style \"join\".\n\
If \"name\" is not given the currently selected item is used. There are\n\
predefined constants for join - JOIN_<type>.\n\
"));
/*! Set line join */
PyObject *scribus_setlinejoin(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinecap__doc__,
QT_TR_NOOP("setLineCap(endtype, [\"name\"])\n\
\n\
Sets the line cap style of the object \"name\" to the style \"cap\".\n\
If \"name\" is not given the currently selected item is used. There are\n\
predefined constants for \"cap\" - CAP_<type>.\n\
"));
/*! Set line end */
PyObject *scribus_setlinecap(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinestyle__doc__,
QT_TR_NOOP("setLineStyle(style, [\"name\"])\n\
\n\
Sets the line style of the object \"name\" to the style \"style\". If \"name\"\n\
is not given the currently selected item is used. There are predefined\n\
constants for \"style\" - LINE_<style>.\n\
"));
/*! Set line end */
PyObject *scribus_setlinestyle(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfillshade__doc__,
QT_TR_NOOP("setFillShade(shade, [\"name\"])\n\
\n\
Sets the shading of the fill color of the object \"name\" to \"shade\".\n\
\"shade\" must be an integer value in the range from 0 (lightest) to 100\n\
(full Color intensity). If \"name\" is not given the currently selected\n\
Item is used.\n\
\n\
May raise ValueError if the fill shade is out of bounds.\n\
"));
/*! Set fill shade */
PyObject *scribus_setfillshade(PyObject * /*self*/, PyObject* args);

/*! docstringscribus_setmultiline__doc__ */
PyDoc_STRVAR(scribus_setcornerrad__doc__,
QT_TR_NOOP("setCornerRadius(radius, [\"name\"])\n\
\n\
Sets the corner radius of the object \"name\". The radius is expressed\n\
in points. If \"name\" is not given the currently selected item is used.\n\
\n\
May raise ValueError if the corner radius is negative.\n\
"));
/*! Set corner radius */
PyObject *scribus_setcornerrad(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setmultiline__doc__,
QT_TR_NOOP("setMultiLine(\"namedStyle\", [\"name\"])\n\
\n\
Sets the line style of the object \"name\" to the named style \"namedStyle\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May raise NotFoundError if the line style doesn't exist.\n\
"));
/*! Set multiline */
PyObject *scribus_setmultiline(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setnewname__doc__,
QT_TR_NOOP("setNewName(newname, [\"name\"])\n\
\n\
Sets the new name of the object \"name\" to the newname.\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May raise NotFoundError if the line style doesn't exist.\n\
"));
/*! Set newname */
PyObject *scribus_setnewname(PyObject * /*self*/, PyObject* args);
#endif

