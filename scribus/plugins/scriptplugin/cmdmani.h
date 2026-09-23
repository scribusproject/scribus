/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDMANI_H
#define CMDMANI_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/** Manipulating Objects */

/*! docstring */
PyDoc_STRVAR(scribus_moveobjectrel__doc__,
QT_TR_NOOP("moveObject(dx, dy [, \"name\"])\n\
\n\
Moves the object \"name\" by dx and dy relative to its current position. The\n\
distances are expressed in the current measurement unit of the document (see\n\
UNIT constants). If \"name\" is not given the currently selected item is used.\n\
If the object \"name\" belongs to a group, the whole group is moved.\n\
"));
/*! Move REL the object */
PyObject *scribus_moveobjectrel(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_moveobjectabs__doc__,
QT_TR_NOOP("moveObjectAbs(x, y [, \"name\"])\n\
\n\
Moves the object \"name\" to a new location. The coordinates are expressed in\n\
the current measurement unit of the document (see UNIT constants).  If \"name\"\n\
is not given the currently selected item is used.  If the object \"name\"\n\
belongs to a group, the whole group is moved.\n\
"));
/*! Move ABS the object */
PyObject *scribus_moveobjectabs(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_rotateobjectrel__doc__,
QT_TR_NOOP("rotateObject(rot [, \"name\"])\n\
\n\
Rotates the object \"name\" by \"rot\" degrees relatively. The object is\n\
rotated by the vertex that is currently selected as the rotation point - by\n\
default, the top left vertex at zero rotation. Positive values mean counter\n\
clockwise rotation when the default rotation point is used. If \"name\" is not\n\
given the currently selected item is used.\n\
"));
/*! Rotate REL the object */
PyObject *scribus_rotateobjectrel(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_rotateobjectabs__doc__,
QT_TR_NOOP("rotateObjectAbs(rot [, \"name\"])\n\
\n\
Sets the rotation of the object \"name\" to \"rot\". Positive values\n\
mean counter clockwise rotation. If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Rotate ABS the object */
PyObject *scribus_rotateobjectabs(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_setrotation__doc__,
QT_TR_NOOP("setRotation(rotation [, name=\"\", basepoint=None])\n\
\n\
Sets the rotation of the object \"name\" to \"rotation\". Positive values\n\
 mean counter clockwise rotation. If \"name\" is not given the currently\n\
 selected item is used.\n\
\n\
If basepoint is not set, the current basepoint is used.\n\
Valid values for basepoint are:\n\
BASEPOINT_TOPLEFT, BASEPOINT_TOP, BASEPOINT_TOPRIGHT,\n\
BASEPOINT_LEFT, BASEPOINT_CENTER, BASEPOINT_RIGHT,\n\
BASEPOINT_BOTTOMLEFT, BASEPOINT_BOTTOM, BASEPOINT_BOTTOMRIGHT\n\
 "));
/*! Set the rotation of the object */
PyObject *scribus_setrotation(PyObject * /*self*/, PyObject* args, PyObject* kw);

/*! docstring */
PyDoc_STRVAR(scribus_sizeobject__doc__,
QT_TR_NOOP("sizeObject(width, height [, \"name\"])\n\
\n\
Resizes the object \"name\" to the given width and height. If \"name\"\n\
is not given the currently selected item is used.\n\
"));
/*! Resize ABS the object */
PyObject *scribus_sizeobject(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getselectedobject__doc__,
QT_TR_NOOP("getSelectedObject([nr]) -> string\n\
\n\
Returns the name of the selected object. \"nr\" if given indicates the number\n\
of the selected object, e.g. 0 means the first selected object, 1 means the\n\
second selected Object and so on.\n\
"));
/*! Returns name of the selected object */
PyObject *scribus_getselectedobject(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_selectioncount__doc__,
QT_TR_NOOP("selectionCount() -> integer\n\
\n\
Returns the number of selected objects.\n\
"));
/*! Returns count of the selected object */
PyObject *scribus_selectioncount(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_selectobject__doc__,
QT_TR_NOOP("selectObject(\"name\")\n\
\n\
Adds the object with the given \"name\" to the current selection.\n\
\n\
Lots of scripter function use the concept of \"currently selected item\" if an object name\n\
is not provided. In the case of multiple selections, the currently selected item is always\n\
the first item in the selection. As a consequence if you are planning to use object \"name\"\n\
as the currently selected item for following operations and current selection is not empty,\n\
you will have to call deselectAll() before calling this function.\n\
"));
/*! Count selection */
PyObject *scribus_selectobject(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deselectall__doc__,
QT_TR_NOOP("deselectAll()\n\
\n\
Deselects all objects in the whole document.\n\
"));
/*! Remove all selection */
PyObject *scribus_deselectall(PyObject * /*self*/);

/*! docstring */
PyDoc_STRVAR(scribus_groupobjects__doc__,
QT_TR_NOOP("groupObjects(list) -> string\n\
\n\
Groups the objects named in \"list\" together. \"list\" must contain the names\n\
of the objects to be grouped. If \"list\" is not given the currently selected\n\
items are used. Returns the group name for further referencing.\n\
"));
/*! Group objects named in list. */
PyObject *scribus_groupobjects(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_ungroupobjects__doc__,
QT_TR_NOOP("unGroupObjects(\"name\")\n\n\
Destructs the group the object \"name\" belongs to.\
If \"name\" is not given the currently selected item is used."));
/*! Ungroup objects named in list. */
PyObject *scribus_ungroupobjects(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_scalegroup__doc__,
QT_TR_NOOP("scaleGroup(factor [,\"name\"])\n\
\n\
Scales the group the object \"name\" belongs to. Values greater than 1 enlarge\n\
the group, values smaller than 1 make the group smaller e.g a value of 0.5\n\
scales the group to 50 % of its original size, a value of 1.5 scales the group\n\
to 150 % of its original size.  The value for \"factor\" must be greater than\n\
0. If \"name\" is not given the currently selected item is used.\n\
\n\
May raise ValueError if an invalid scale factor is passed.\n\
"));
/*! Scale group with object name */
PyObject *scribus_scalegroup(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getGroupItems__doc__,
QT_TR_NOOP("getGroupItems([\"name\", recursive=False, type=0]) -> list\n\n\
Return the list of items in the group.\n\
\n\
`recursive`: if True and some of the items are groups, also include their items (recursively).\n\
`type`: if not 0, only retain items of this type.\n\
\n\
Each item is defined as a tuple containing:\n\
`(name : str, objectType : int, order : int)`\n\
E.g. [('Text1', 4, 0), ('Image1', 2, 1)]\n\
means that object named 'Text1' is a text frame (type 4) and is the first at\n\
the page...\n\
\n\
If \"name\" is not given the currently selected item is used."));
/*! List the items in a group. */
PyObject *scribus_getGroupItems(PyObject * /*self*/, PyObject* args, PyObject* kw);

/*! docstring */
PyDoc_STRVAR(scribus_loadimage__doc__,
QT_TR_NOOP("loadImage(\"filename\" [, \"name\"])\n\
\n\
Loads the picture \"picture\" into the image frame \"name\". If \"name\" is\n\
not given the currently selected item is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Loads image file into frame. */
PyObject *scribus_loadimage(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_relinkimage__doc__,
QT_TR_NOOP("relinkImage(\"filename\" [, \"name\"]) -> bool\n\
\n\
Replaces the external image used by image frame \"name\" while preserving the\n\
frame's crop, scale, effects, transparency and colour-management settings.\n\
The operation is undoable. If the replacement cannot be loaded, the original\n\
link and image are kept and False is returned. Embedded images cannot be\n\
relinked.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame.\n\
"));
/*! Relink an external image without changing frame-level image settings. */
PyObject *scribus_relinkimage(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_replaceimagelinks__doc__,
QT_TR_NOOP("replaceImageLinks(\"source\", \"replacement\" [, dryRun=False, scope=\"document\"]) -> (matched, replaced, failed)\n\
\n\
Replace every external image frame linked to the exact source path, including\n\
frames on master pages and inside groups. Embedded and empty frames are skipped.\n\
Each successful relink preserves frame crop and scale; all changes form one\n\
undo step. A dry run counts matches without changing the document. The tuple\n\
reports matches, successful replacements and load failures. Original image\n\
files are never changed.\n\
scope may be document, page (current page only), or masters.\n\
"));
PyObject *scribus_replaceimagelinks(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_generateimagealphacontour__doc__,
QT_TR_NOOP("generateImageAlphaContour([threshold=128, padding=0.0, wrap=True, name=\"\"]) -> bool\n\
\n\
Build an editable contour from the loaded image's transparency, clipped to\n\
the frame. Threshold is 1–255; padding is a clearance in points. When wrap\n\
is True, text flows around the contour. The change is undoable and does not\n\
modify the image file. Raises ScribusException if no usable alpha exists.\n\
"));
PyObject *scribus_generateimagealphacontour(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_generateimagecontour__doc__,
QT_TR_NOOP("generateImageContour([source=\"alpha\", threshold=128, padding=0.0, smoothing=0, simplification=0.0, wrap=True, name=\"\"]) -> bool\n\
\n\
Build an editable image contour from alpha, clip, luminance, or edge contrast.\n\
The edge mode compares pixels with the average corner background colour.\n\
Smoothing rounds outward in points; simplification reduces source detail before\n\
tracing. The contour is clipped to the image frame before padding is applied.\n\
The operation is undoable and does not modify the image file.\n\
"));
PyObject *scribus_generateimagecontour(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_exportimageascmykcopy__doc__,
QT_TR_NOOP("exportImageAsCMYKCopy(\"filename\" [, \"name\", relink=False]) -> bool\n\
\n\
Exports a non-destructive CMYK TIFF copy of the RGB raster image in frame\n\
\"name\" using the document's CMYK output ICC profile. The source image and\n\
frame link remain unchanged unless relink is True. A relink uses the new TIFF's\n\
embedded ICC profile and can be undone without deleting the exported file.\n\
Color management must be enabled. The destination\n\
must end in .tif or .tiff and must not exist. Images with transparency or\n\
frame effects are not supported.\n\
Raises ScribusException on conversion or file errors. Returns False if the\n\
export succeeds but the optional relink fails. Embedded images cannot be\n\
relinked through this command.\n\
"));
PyObject *scribus_exportimageascmykcopy(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_batchexportimagesascmyk__doc__,
QT_TR_NOOP("batchExportImagesAsCMYK(directory [, sourceProfile='', destinationProfile='', intent=-1, blackPoint=-1, backup=True, relink=False, dryRun=False]) -> tuple\n\
\n\
Export eligible RGB raster image frames as separate profile-embedded CMYK TIFFs.\n\
Returns (eligible, exported, relinked, failed, reportPath). Dry run writes no files.\n\
The source image files are never overwritten. Optional frame relinks are grouped\n\
in one undo step. A JSON report is saved in the output directory on export.\n\
intent and blackPoint use -1 for each frame's current defaults.\n\
"));
PyObject *scribus_batchexportimagesascmyk(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_embedimage__doc__,
QT_TR_NOOP("embedImage([\"name\"]) -> bool\n\
\n\
Embeds the image used by image frame \"name\" in the document without\n\
re-encoding it. If \"name\" is not given the currently selected item is used.\n\
Returns True when the image is embedded.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame.\n\
"));
/*! Embed an externally linked image in the document. */
PyObject *scribus_embedimage(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_extractembeddedimage__doc__,
QT_TR_NOOP("extractEmbeddedImage(\"filename\" [, relink=False, overwrite=False, \"name\"]) -> bool\n\
\n\
Copies the exact stored bytes of the embedded image in frame \"name\" to\n\
\"filename\". Set relink to True to replace the embedded data with a link to\n\
the extracted file; that document change is undoable. Existing files are kept\n\
unless overwrite is True. If \"name\" is not given the currently selected item\n\
is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame.\n\
"));
/*! Extract an embedded image, optionally relinking its frame. */
PyObject *scribus_extractembeddedimage(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_isimageembedded__doc__,
QT_TR_NOOP("isImageEmbedded([\"name\"]) -> bool\n\
\n\
Returns True if image frame \"name\" stores its image inside the document. If\n\
\"name\" is not given the currently selected item is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame.\n\
"));
/*! Report whether an image frame contains embedded image data. */
PyObject *scribus_isimageembedded(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_scaleimage__doc__,
QT_TR_NOOP("scaleImage(x, y [, \"name\"])\n\
\n\
Sets the internal scaling factors of the picture in the image frame \"name\".\n\
If \"name\" is not given the currently selected item is used. A number of 1\n\
means 100 %. Internal scaling factors are different from the values shown on \n\
properties palette. Note : deprecated, use setImageScale() instead.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Scale Image. */
PyObject *scribus_scaleimage(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setimageoffset__doc__,
QT_TR_NOOP("setImageOffset(x, y [, \"name\"])\n\
\n\
Sets the position of the picture in the image frame \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
The specified offset values are equal to the values shown on \n\
properties palette when point unit is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Scale Image. */
PyObject *scribus_setimageoffset(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setimagepage__doc__,
QT_TR_NOOP("setImagePage(page [, \"name\"])\n\
\n\
Set the page for multiple page images (like PDFs) in the image frame \"name\".\n\
If \"name\" is not given the currently selected item is used. The number must be between 0\n\
(\"auto\") and the actual number of pages.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
PyObject *scribus_setimagepage(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setimagepreviewresolution__doc__,
QT_TR_NOOP("setImagePreviewResolution(resolutionType, [, \"name\"])\n\
\n\
Sets preview resolution of the picture in the image frame \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
The resolutionType shall be one of:\n\
- IMAGE_PREVIEW_RESOLUTION_FULL,\n\
- IMAGE_PREVIEW_RESOLUTION_NORMAL,\n\
- IMAGE_PREVIEW_RESOLUTION_LOW,\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Scale Image. */
PyObject *scribus_setimagepreviewresolution(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setimagescale__doc__,
QT_TR_NOOP("setImageScale(x, y [, \"name\"])\n\
\n\
Sets the scaling factors of the picture in the image frame \"name\".\n\
If \"name\" is not given the currently selected item is used. A number of 1\n\
means 100 %. Scaling factors are equal to the values shown on properties palette.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Scale Image. */
PyObject *scribus_setimagescale(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setimagerotation__doc__,
QT_TR_NOOP("setImageRotation(deg [, \"name\"])\n\
\n\
Sets the rotation of the picture in the image frame \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Rotate Image. */
PyObject *scribus_setimagerotation(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setimagebrightness__doc__,
QT_TR_NOOP("setImageBrightness(n [, \"name\"])\n\
\n\
Set image brightness effect of the picture in the image frame \"name\".\n\
If \"name\" is not given the currently selected item is used. A number of 1\n\
means 100 %. Brightness factor is equal to the value shown on properties palette.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Set Image Brightness. */
PyObject *scribus_setimagebrightness(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setimagegrayscale__doc__,
QT_TR_NOOP("setImageGrayscale([\"name\"])\n\
\n\
Set image grayscale effect of the picture in the image frame \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an image frame\n\
"));
/*! Set Image Brightness. */
PyObject *scribus_setimagegrayscale(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setexportableobject__doc__,
QT_TR_NOOP("setExportableObject([\"name\", exportable]) -> bool\n\
\n\
Sets the object \"name\" as exportable if exportable parameter is true.\n\
If \"name\" is not given the currently selected item is used. Returns true\n\
if exportable.\n\
"));
/*! Set item exportable*/
PyObject *scribus_setexportableobject(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_isexportable__doc__,
QT_TR_NOOP("isExportable([\"name\"]) -> bool\n\
\n\
Returns true if is the object \"name\" is exportable.  If \"name\" is not given the\n\
currently selected item is used.\n\
"));
/*! Check if item is exportable*/
PyObject *scribus_isexportable(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_lockobject__doc__,
QT_TR_NOOP("lockObject([\"name\"]) -> bool\n\
\n\
Locks the object \"name\" if it's unlocked or unlock it if it's locked.\n\
If \"name\" is not given the currently selected item is used. Returns true\n\
if locked.\n\
"));
/*! (Un)Lock the object 2004/7/10 pv.*/
PyObject *scribus_lockobject(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_islocked__doc__,
QT_TR_NOOP("isLocked([\"name\"]) -> bool\n\
\n\
Returns true if is the object \"name\" locked.  If \"name\" is not given the\n\
currently selected item is used.\n\
"));
/*! Status of locking 2004/7/10 pv.*/
PyObject *scribus_islocked(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_setscaleframetoimage__doc__,
QT_TR_NOOP("setScaleFrameToImage([name])\n\
\n\
Set frame size on the selected or specified image frame to image size.\n\
\n\
May raise WrongFrameTypeError.\n\
"));
PyObject *scribus_setscaleframetoimage(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_setscaleimagetoframe__doc__,
QT_TR_NOOP("setScaleImageToFrame(scaletoframe, proportional=None, name=<selection>)\n\
\n\
Sets the scale to frame on the selected or specified image frame to 'scaletoframe'.\n\
If 'proportional' is specified, set fixed aspect ratio scaling to 'proportional'.\n\
Both 'scaletoframe' and 'proportional' are boolean.\n\
\n\
May raise WrongFrameTypeError.\n\
"));
PyObject *scribus_setscaleimagetoframe(PyObject * /*self*/, PyObject* args, PyObject* kwargs);

/*! docstring */
PyDoc_STRVAR(scribus_flipobject__doc__,
QT_TR_NOOP("flipObject(H,V,[\"name\"])\n\
\n\
Toggle the object \"name\" horizontal and/or vertical flip.\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Flip the object 2010/5/18.*/
PyObject *scribus_flipobject(PyObject * /*self*/, PyObject* args);

PyDoc_STRVAR(scribus_combinepolygons__doc__,
QT_TR_NOOP("combinePolygons()\n\
\n\
Combine two or more selected Polygons\n\
"));
PyObject *scribus_combinepolygons(PyObject * /* self */);

PyDoc_STRVAR(scribus_seteditmode__doc__,
QT_TR_NOOP("setEditMode()\n\
\n\
Start the edit mode for the current item.\n\
"));
PyObject *scribus_seteditmode(PyObject * /*self*/);

PyDoc_STRVAR(scribus_setnormalmode__doc__,
QT_TR_NOOP("setNormalMode()\n\
\n\
Set the current item in normal mode (out of edit mode).\n\
"));
PyObject *scribus_setnormalmode(PyObject * /*self*/);

#endif
