#ifndef CMDMISC_H
#define CMDMISC_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Other Commands */

/*! docstring */
PyDoc_STRVAR(scribus_setredraw__doc__,
    "setRedraw(bool)\n\n\
Disables page redraw when bool = 0, otherwise redrawing is enabled.");
/*! Enable/disable page redrawing. */
PyObject *scribus_setredraw(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_fontnames__doc__,
    "getFontNames() -> list\n\n\
Returns a list with the names of all available fonts.");
/*! simple list of font names. */
PyObject *scribus_fontnames(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_xfontnames__doc__,
    "getXFontNames() -> list of tuples\n\n\
Returns a larger font info. It's a list of the tuples with: \
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]");
/*!
 return a list of the tuples with
 Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file
*/
PyObject *scribus_xfontnames(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_renderfont__doc__,
    "rendeFont(\"name\", \"filename\", \"sample\", size) -> bool\n\n\
Creates an image preview of font \"name\" with given text \"sample\"\
and size. Image is saved into \"filename\". Returns true when success.");
/*! Font example to image. */
PyObject *scribus_renderfont(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlayers__doc__,
    "getLayers() -> list\n\n\
Returns a list with the names of all defined layers.");
/*! List of the layers */
PyObject *scribus_getlayers(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_setactlayer__doc__,
    "setActiveLayer(\"name\")\n\n\
Sets the active layer to the layer named \"name\".");
/*! Move into layer */
PyObject *scribus_setactlayer(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getactlayer__doc__,
    "getActiveLayer() -> string\n\n\
Returns the name of the current active layer.");
/*! Get layer name */
PyObject *scribus_getactlayer(PyObject *self);

/*! docstring */
PyDoc_STRVAR(scribus_senttolayer__doc__,
    "sentToLayer(\"layer\" [, \"name\"])\n\n\
Sends the object \"name\" to the layer \"layer\". The layer\
must exist. If \"name\" is not given the currently selected\
item is used.");
/*! Move object from one layer to other one */
PyObject *scribus_senttolayer(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_layervisible__doc__,
    "setLayerVisible(\"layer\", visible)\n\n\
Sets the layer \"layer\" to be visible or not. If is the\
visible set to false the layer is invisible.");
/*! Set layer visible */
PyObject *scribus_layervisible(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_layerprint__doc__,
    "setLayerPrintable(\"layer\", printable)\n\n\
Sets the layer \"layer\" to be printable or not. If is the\
printable set to false the layer won't be printed.");
/*! Set layer printable */
PyObject *scribus_layerprint(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_glayervisib__doc__,
    "isLayerPrintable(\"layer\") -> bool\n\n\
Returns wether the Layer \"layer\" is visible or not, a value\
of true means that the layer \"layer\" is visible, a Value of false\
means that the layer \"layer\" is invisible.");
/*! Set layer visible */
PyObject *scribus_glayervisib(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_glayerprint__doc__,
    "isLayerPrintable(\"layer\") -> bool\n\n\
Returns wether the layer \"layer\" is printable or not,\
a value of true means that the layer \"layer\" can be printed,\
a value of false means that printing the layer \"layer\"\
is disabled.");
/*! Set layer printable */
PyObject *scribus_glayerprint(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_removelayer__doc__,
    "deleteLayer(\"layer\")\n\n\
Deletes the layer with the name \"layer\". Nothing happens\
if the layer doesn't exists or if it's the only layer in\
the document.");
/*! Remove layer */
PyObject *scribus_removelayer(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createlayer__doc__,
    "createLayer(layer)\n\n\
Creates a new layer with the name \"name\".");
/*! New layer */
PyObject *scribus_createlayer(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlanguage__doc__,
    "getGuiLanguage() -> string\n\n\
Returns a string with the -lang value.");
/*! Language of the GUI */
PyObject *scribus_getlanguage(PyObject *self);

#endif

