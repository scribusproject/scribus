#ifndef CMDMISC_H
#define CMDMISC_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Other Commands */

/*! docstring */
PyDoc_STRVAR(scribus_setredraw__doc__,
QT_TR_NOOP("setRedraw(bool)\n\
\n\
Disables page redraw when bool = False, otherwise redrawing is enabled.\n\
This change will persist even after the script exits, so make sure to call\n\
setRedraw(True) in a finally: clause at the top level of your script.\n\
"));
/*! Enable/disable page redrawing. */
PyObject *scribus_setredraw(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_fontnames__doc__,
QT_TR_NOOP("getFontNames() -> list\n\
\n\
Returns a list with the names of all available fonts.\n\
"));
/*! simple list of font names. */
PyObject *scribus_fontnames(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_xfontnames__doc__,
QT_TR_NOOP("getXFontNames() -> list of tuples\n\
\n\
Returns a larger font info. It's a list of the tuples with:\n\
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]\n\
"));
/*!
 return a list of the tuples with
 Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file
*/
PyObject *scribus_xfontnames(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_renderfont__doc__,
QT_TR_NOOP("rendeFont(\"name\", \"filename\", \"sample\", size) -> bool\n\
\n\
Creates an image preview of font \"name\" with given text \"sample\" and size.\n\
Image is saved into \"filename\". Returns true when success.\n\
\n\
May raise NotFoundError if the specified font can't be found.\n\
May raise ValueError if an empty sample or filename is passed.\n\
"));
/*! Font example to image. */
PyObject *scribus_renderfont(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlayers__doc__,
QT_TR_NOOP("getLayers() -> list\n\
\n\
Returns a list with the names of all defined layers.\n\
"));
/*! List of the layers */
PyObject *scribus_getlayers(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_setactlayer__doc__,
QT_TR_NOOP("setActiveLayer(\"name\")\n\
\n\
Sets the active layer to the layer named \"name\".\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Move into layer */
PyObject *scribus_setactlayer(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getactlayer__doc__,
QT_TR_NOOP("getActiveLayer() -> string\n\
\n\
Returns the name of the current active layer.\n\
"));
/*! Get layer name */
PyObject *scribus_getactlayer(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_senttolayer__doc__,
QT_TR_NOOP("sentToLayer(\"layer\" [, \"name\"])\n\
\n\
Sends the object \"name\" to the layer \"layer\". The layer must exist.\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Move object from one layer to other one */
PyObject *scribus_senttolayer(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_layervisible__doc__,
QT_TR_NOOP("setLayerVisible(\"layer\", visible)\n\
\n\
Sets the layer \"layer\" to be visible or not. If is the visible set to false\n\
the layer is invisible.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer visible */
PyObject *scribus_layervisible(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_layerprint__doc__,
QT_TR_NOOP("setLayerPrintable(\"layer\", printable)\n\
\n\
Sets the layer \"layer\" to be printable or not. If is the printable set to\n\
false the layer won't be printed.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer printable */
PyObject *scribus_layerprint(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_glayervisib__doc__,
QT_TR_NOOP("isLayerPrintable(\"layer\") -> bool\n\
\n\
Returns wether the Layer \"layer\" is visible or not, a value of True means\n\
that the layer \"layer\" is visible, a value of False means that the layer\n\
\"layer\" is invisible.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer visible */
PyObject *scribus_glayervisib(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_glayerprint__doc__,
QT_TR_NOOP("isLayerPrintable(\"layer\") -> bool\n\
\n\
Returns wether the layer \"layer\" is printable or not, a value of True means\n\
that the layer \"layer\" can be printed, a value of False means that printing\n\
the layer \"layer\" is disabled.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer printable */
PyObject *scribus_glayerprint(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_removelayer__doc__,
QT_TR_NOOP("deleteLayer(\"layer\")\n\
\n\
Deletes the layer with the name \"layer\". Nothing happens if the layer doesn't\n\
exists or if it's the only layer in the document.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Remove layer */
PyObject *scribus_removelayer(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createlayer__doc__,
QT_TR_NOOP("createLayer(layer)\n\
\n\
Creates a new layer with the name \"name\".\n\
\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! New layer */
PyObject *scribus_createlayer(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlanguage__doc__,
QT_TR_NOOP("getGuiLanguage() -> string\n\
\n\
Returns a string with the -lang value.\n\
"));
/*! Language of the GUI */
PyObject *scribus_getlanguage(PyObject *self);

#endif

