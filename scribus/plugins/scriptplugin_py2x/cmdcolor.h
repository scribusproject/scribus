/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDCOLOR_H
#define CMDCOLOR_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Managing Colors */

/*! docstring */
PyDoc_STRVAR(scribus_colornames__doc__,
QT_TR_NOOP("getColorNames() -> list\n\
\n\
Returns a list containing the names of all defined colors in the document.\n\
If no document is open, returns a list of the default document colors.\n\
"));
/** Returns a list with colours available in doc or in prefs. */
PyObject *scribus_colornames(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_getcolor__doc__,
QT_TR_NOOP("getColor(\"name\") -> tuple\n\
\n\
Returns a tuple (C, M, Y, K) containing the four color components of the\n\
color \"name\" from the current document. If no document is open, returns\n\
the value of the named color from the default document colors.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Returns a CMYK tuple of the specified color. */
PyObject *scribus_getcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcolorfloat__doc__,
QT_TR_NOOP("getColorFloat(\"name\") -> tuple\n\
\n\
Returns a tuple (C, M, Y, K) containing the four color components of the\n\
color \"name\" from the current document. Color components are floating point\n\
values between 0 and 100. If no document is open, returns the value of the\n\
named color from the default document colors.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Returns a CMYK tuple of the specified color. */
PyObject *scribus_getcolorfloat(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcolorasrgb__doc__,
QT_TR_NOOP("getColorAsRGB(\"name\") -> tuple\n\
\n\
Returns a tuple (R,G,B) containing the three color components of the\n\
color \"name\" from the current document, converted to the RGB color\n\
space. If no document is open, returns the value of the named color\n\
from the default document colors.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Returns a CMYK tuple of the specified color. */
PyObject *scribus_getcolorasrgb(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcolorasrgbfloat__doc__,
QT_TR_NOOP("getColorAsRGBFloat(\"name\") -> tuple\n\
\n\
Returns a tuple (R,G,B) containing the three color components of the\n\
color \"name\" from the current document, converted to the RGB color\n\
space. Color components are floating point values between 0 and 255.\n\
If no document is open, returns the value of the named color\n\
from the default document colors.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Returns a CMYK tuple of the specified color. */
PyObject *scribus_getcolorasrgbfloat(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolor__doc__,
QT_TR_NOOP("changeColor(\"name\", c, m, y, k)\n\
\n\
Changes the color \"name\" to the specified CMYK value. The color value is\n\
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.\n\
Color components should be in the range from 0 to 255.\n\
Note : deprecated, use changeColorCMYK() instead.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Sets named color with C,M,Y,K params. */
PyObject *scribus_setcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolorcmyk__doc__,
QT_TR_NOOP("changeColorCMYK(\"name\", c, m, y, k)\n\
\n\
Changes the color \"name\" to the specified CMYK value. The color value is\n\
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.\n\
Color components should be in the range from 0 to 255.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Sets named color with C,M,Y,K params. */
PyObject *scribus_setcolorcmyk(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolorcmykfloat__doc__,
QT_TR_NOOP("changeColorCMYKFloat(\"name\", c, m, y, k)\n\
\n\
Changes the color \"name\" to the specified CMYK value. The color value is\n\
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.\n\
Color components are floating point values between 0 and 100.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Sets named color with C,M,Y,K params. */
PyObject *scribus_setcolorcmykfloat(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolorrgb__doc__,
QT_TR_NOOP("changeColorRGB(\"name\", r, g, b)\n\
\n\
Changes the color \"name\" to the specified RGB value. The color value is\n\
defined via three components r = red, g = green, b = blue.\n\
Color components should be in the range from 0 to 255.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Sets named color with R, G, B params. */
PyObject *scribus_setcolorrgb(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolorrgbfloat__doc__,
QT_TR_NOOP("changeColorRGBFloat(\"name\", r, g, b)\n\
\n\
Changes the color \"name\" to the specified RGB value. The color value is\n\
defined via three components r = red, g = green, b = blue.\n\
Color components are floating point values between 0 and 255.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Sets named color with R, G, B params. */
PyObject *scribus_setcolorrgbfloat(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolorlab__doc__,
QT_TR_NOOP("changeColorLab(\"name\", r, g, b)\n\
\n\
Changes the color \"name\" to the specified CIELab values. The color value is defined\n\
via three components: L = luminosity, a = green/red, b = blue/yellow. Color components\n\
are floating point values with L between 0 and 100, a and b between -128 and 128.\n\
\n\
May raise NotFoundError if the named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Sets named color with R, G, B params. */
PyObject *scribus_setcolorlab(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newcolor__doc__,
QT_TR_NOOP("defineColor(\"name\", c, m, y, k)\n\
\n\
Defines a new color \"name\". The color Value is defined via four components:\n\
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in\n\
the range from 0 to 255.\n\
Note : deprecated, use defineColorCMYK() instead.\n\
\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Creates new color with name, C, M, Y, K params. */
PyObject *scribus_newcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newcolorcmyk__doc__,
QT_TR_NOOP("defineColorCMYK(\"name\", c, m, y, k)\n\
\n\
Defines a new color \"name\". The color Value is defined via four components:\n\
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in\n\
the range from 0 to 255.\n\
\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Creates new color with name, C, M, Y, K params. */
PyObject *scribus_newcolorcmyk(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newcolorcmykfloat__doc__,
QT_TR_NOOP("defineColorCMYKFloat(\"name\", c, m, y, k)\n\
\n\
Defines a new color \"name\". The color Value is defined via four components:\n\
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components are\n\
floating point values between 0 and 100.\n\
\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Creates new color with name, C, M, Y, K params. */
PyObject *scribus_newcolorcmykfloat(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newcolorrgb__doc__,
QT_TR_NOOP("defineColorRGB(\"name\", r, g, b)\n\
\n\
Defines a new color \"name\". The color Value is defined via three components:\n\
r = red, g = green, b = blue. Color components should be in\n\
the range from 0 to 255.\n\
\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Creates new color with name, R, G, B params. */
PyObject *scribus_newcolorrgb(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newcolorrgbfloat__doc__,
QT_TR_NOOP("defineColorRGBFloat(\"name\", r, g, b)\n\
\n\
Defines a new color \"name\". The color Value is defined via three components:\n\
r = red, g = green, b = blue. Color components are floating point values\n\
between 0 and 255.\n\
\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Creates new color with name, R, G, B params. */
PyObject *scribus_newcolorrgbfloat(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_newcolorlab__doc__,
QT_TR_NOOP("defineColorLab(\"name\", L, a, b)\n\
\n\
Defines a new color \"name\" using CIELab values. The color value is defined\n\
via three components: L = luminosity, a = green/red, b = blue/yellow. Color components\n\
are floating point values with L between 0 and 100, a and b between -128 and 128.\n\
\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Creates new color with name, L, a, b params. */
PyObject *scribus_newcolorlab(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_delcolor__doc__,
QT_TR_NOOP("deleteColor(\"name\", \"replace\")\n\
\n\
Deletes the color \"name\". Every occurrence of that color is replaced by the\n\
color \"replace\". If not specified, \"replace\" defaults to the color\n\
\"None\" - transparent.\n\
\n\
deleteColor works on the default document colors if there is no document open.\n\
In that case, \"replace\", if specified, has no effect.\n\
\n\
May raise NotFoundError if a named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Deletes named color */
PyObject *scribus_delcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_replcolor__doc__,
QT_TR_NOOP("replaceColor(\"name\", \"replace\")\n\
\n\
Every occurrence of the color \"name\" is replaced by the color \"replace\".\n\
\n\
May raise NotFoundError if a named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Replaces color with the 2nd one. */
PyObject *scribus_replcolor(PyObject * /*self*/, PyObject* args);


/*! docstring */
PyDoc_STRVAR(scribus_isspotcolor__doc__,
QT_TR_NOOP("isSpotColor(\"name\") -> bool\n\
\n\
Returns True if the color \"name\" is a spot color.\n\
See also setSpotColor()\n\
\n\
May raise NotFoundError if a named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Replaces color with the 2nd one. */
PyObject *scribus_isspotcolor(PyObject * /*self*/, PyObject* args);



/*! docstring */
PyDoc_STRVAR(scribus_setspotcolor__doc__,
QT_TR_NOOP("setSpotColor(\"name\", spot)\n\
\n\
Set the color \"name\" as a spot color if spot parameter is True.\n\
See also isSpotColor()\n\
\n\
May raise NotFoundError if a named color wasn't found.\n\
May raise ValueError if an invalid color name is specified.\n\
"));
/** Replaces color with the 2nd one. */
PyObject *scribus_setspotcolor(PyObject * /*self*/, PyObject* args);

#endif


