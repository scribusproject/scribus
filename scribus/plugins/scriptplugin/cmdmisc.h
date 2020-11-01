/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDMISC_H
#define CMDMISC_H

// Pulls in <Python.h> first
#include "cmdvar.h"
//Added by qt3to4:
#include <QPixmap>

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
PyObject *scribus_setredraw(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfontnames__doc__,
QT_TR_NOOP("getFontNames() -> list\n\
\n\
Returns a list with the names of all available fonts.\n\
"));
/*! simple list of font names. */
PyObject *scribus_getfontnames(PyObject * /*self*/);

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
PyObject *scribus_xfontnames(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_renderfont__doc__,
QT_TR_NOOP("renderFont(\"name\", \"filename\", \"sample\", size, format=\"PPM\") -> bool\n\
\n\
Creates an image preview of font \"name\" with given text \"sample\" and size.\n\
If \"filename\" is not \"\", image is saved into \"filename\". Otherwise\n\
image data is returned as a string. The optional \"format\" argument\n\
specifies the image format to generate, and supports any format allowed\n\
by QPixmap.save(). Common formats are PPM, JPEG, PNG and XPM.\n\
\n\
May raise NotFoundError if the specified font can't be found.\n\
May raise ValueError if an empty sample or filename is passed.\n\
"));
/*! Font example to image. */
PyObject *scribus_renderfont(PyObject* /*self*/, PyObject* args, PyObject* kw);

/*! docstring */
PyDoc_STRVAR(scribus_getlayers__doc__,
QT_TR_NOOP("getLayers() -> list\n\
\n\
Returns a list with the names of all defined layers.\n\
"));
/*! List of the layers */
PyObject *scribus_getlayers(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_setactivelayer__doc__,
QT_TR_NOOP("setActiveLayer(\"name\")\n\
\n\
Sets the active layer to the layer named \"name\".\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Move into layer */
PyObject *scribus_setactivelayer(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getactivelayer__doc__,
QT_TR_NOOP("getActiveLayer() -> string\n\
\n\
Returns the name of the current active layer.\n\
"));
/*! Get layer name */
PyObject *scribus_getactivelayer(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_loweractivelayer__doc__,
QT_TR_NOOP("lowerActiveLayer()\n\
\n\
Lowers the current active layer.\n\
"));
/*! Lower active Layer */
PyObject *scribus_loweractivelayer(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_raiseactivelayer__doc__,
QT_TR_NOOP("raiseActiveLayer()\n\
\n\
Raises the current active layer.\n\
"));
/*! Raise active layer */
PyObject *scribus_raiseactivelayer(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_sendtolayer__doc__,
QT_TR_NOOP("sendToLayer(\"layer\" [, \"name\"])\n\
\n\
Sends the object \"name\" to the layer \"layer\". The layer must exist.\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Move object from one layer to other one */
PyObject *scribus_sendtolayer(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlayervisible__doc__,
QT_TR_NOOP("setLayerVisible(\"layer\", visible)\n\
\n\
Sets the layer \"layer\" to be visible or not. If is the visible set to false\n\
the layer is invisible.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer visible */
PyObject *scribus_setlayervisible(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlayerprintable__doc__,
QT_TR_NOOP("setLayerPrintable(\"layer\", printable)\n\
\n\
Sets the layer \"layer\" to be printable or not. If is the\n\
printable set to false the layer won't be printed.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer printable */
PyObject *scribus_setlayerprintable(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlayerlocked__doc__,
QT_TR_NOOP("setLayerLocked(\"layer\", locked)\n\
\n\
Sets the layer \"layer\" to be locked or not. If locked is set to\n\
true the layer will be locked.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer printable */
PyObject *scribus_setlayerlocked(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlayeroutlined__doc__,
QT_TR_NOOP("setLayerOutlined(\"layer\", outline)\n\
\n\
Sets the layer \"layer\" to be locked or not. If outline is set to\n\
true the layer will be displayed outlined.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer printable */
PyObject *scribus_setlayeroutlined(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlayerflow__doc__,
QT_TR_NOOP("setLayerFlow(\"layer\", flow)\n\
\n\
Sets the layers \"layer\"  flowcontrol to flow. If flow is set to\n\
true text in layers above this one will flow around objects on this layer.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer printable */
PyObject *scribus_setlayerflow(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlayerblendmode__doc__,
QT_TR_NOOP("setLayerBlendmode(\"layer\", blend)\n\
\n\
Sets the layers \"layer\"  blendmode to blend.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer printable */
PyObject *scribus_setlayerblendmode(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlayertransparency__doc__,
QT_TR_NOOP("setLayerTransparency(\"layer\", trans)\n\
\n\
Sets the layers \"layer\"  transparency to trans.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer printable */
PyObject *scribus_setlayertransparency(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_islayervisible__doc__,
QT_TR_NOOP("isLayerVisible(\"layer\") -> bool\n\
\n\
Returns whether the layer \"layer\" is visible or not, a value of True means\n\
that the layer \"layer\" is visible, a value of False means that the layer\n\
\"layer\" is invisible.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer visible */
PyObject *scribus_islayervisible(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_islayerprintable__doc__,
QT_TR_NOOP("isLayerPrintable(\"layer\") -> bool\n\
\n\
Returns whether the layer \"layer\" is printable or not, a value of True means\n\
that the layer \"layer\" can be printed, a value of False means that printing\n\
the layer \"layer\" is disabled.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer printable */
PyObject *scribus_islayerprintable(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_islayerlocked__doc__,
QT_TR_NOOP("isLayerLocked(\"layer\") -> bool\n\
\n\
Returns whether the layer \"layer\" is locked or not, a value of True means\n\
that the layer \"layer\" is editable, a value of False means that the layer\n\
\"layer\" is locked.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer visible */
PyObject *scribus_islayerlocked(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_islayeroutlined__doc__,
QT_TR_NOOP("isLayerOutlined(\"layer\") -> bool\n\
\n\
Returns whether the layer \"layer\" is outlined or not, a value of True means\n\
that the layer \"layer\" is outlined, a value of False means that the layer\n\
\"layer\" is normal.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer visible */
PyObject *scribus_islayeroutlined(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_islayerflow__doc__,
QT_TR_NOOP("isLayerFlow(\"layer\") -> bool\n\
\n\
Returns whether text flows around objects on layer \"layer\", a value of True means\n\
that text flows around, a value of False means that the text does not flow around.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer visible */
PyObject *scribus_islayerflow(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlayerblendmode__doc__,
QT_TR_NOOP("getLayerBlendmode(\"layer\") -> int\n\
\n\
Returns the \"layer\" layer blendmode,\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer visible */
PyObject *scribus_getlayerblendmode(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlayertransparency__doc__,
QT_TR_NOOP("getLayerTransparency(\"layer\") -> float\n\
\n\
Returns the \"layer\" layer transparency,\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Set layer visible */
PyObject *scribus_getlayertransparency(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deletelayer__doc__,
QT_TR_NOOP("deleteLayer(\"layer\")\n\
\n\
Deletes the layer with the name \"layer\". Nothing happens if the layer doesn't\n\
exists or if it's the only layer in the document.\n\
\n\
May raise NotFoundError if the layer can't be found.\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! Remove layer */
PyObject *scribus_deletelayer(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_createlayer__doc__,
QT_TR_NOOP("createLayer(layer)\n\
\n\
Creates a new layer with the name \"name\".\n\
\n\
May raise ValueError if the layer name isn't acceptable.\n\
"));
/*! New layer */
PyObject *scribus_createlayer(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_filequit__doc__,
QT_TR_NOOP("fileQuit()\n\
\n\
Quit Scribus.\n\
"));
/*! Quit Scribus */
PyObject *scribus_filequit(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlanguage__doc__,
QT_TR_NOOP("getGuiLanguage() -> string\n\
\n\
Returns a string with the -lang value.\n\
"));
/*! Language of the GUI */
PyObject *scribus_getlanguage(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_moveselectiontofront__doc__,
QT_TR_NOOP("moveSelectionToFront()\n\
\n\
Moves current selection to front.\n\
"));
/*! 04.01.2007 : Joachim Neu : Moves item selection to front. */
PyObject *scribus_moveselectiontofront(PyObject*);

/*! docstring */
PyDoc_STRVAR(scribus_moveselectiontoback__doc__,
QT_TR_NOOP("moveSelectionToBack()\n\
\n\
Moves current selection to back.\n\
"));
/*! 04.01.2007 : Joachim Neu : Moves item selection to back. */
PyObject *scribus_moveselectiontoback(PyObject*);

PyDoc_STRVAR(scribus_savepdfoptions__doc__,
QT_TR_NOOP("savePDFOptions(fileName)\n\
\n\
Save PDF options to fileName.\n\
"));
PyObject *scribus_savepdfoptions(PyObject* /* self */, PyObject* args);

PyDoc_STRVAR(scribus_readpdfoptions__doc__,
QT_TR_NOOP("readPDFOptions(fileName)\n\
\n\
Read PDF options from fileName.\n\
"));
PyObject *scribus_readpdfoptions(PyObject* /* self */, PyObject* args);

#endif


