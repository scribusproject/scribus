/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef OBJIMGEXPORT_H
#define OBJIMGEXPORT_H

// Pulls in <Python.h> first
#include "cmdvar.h"

extern PyTypeObject ImageExport_Type;

// docstrings
PyDoc_STRVAR(imgexp__doc__,"Image export\n\
\n\
Class ImageExport() provides the bitmap graphics exporting\n\
for Python scripting as you know it from Export/Save as Image\n\
menu. See related class PDFfile() and procedure savePageAsEPS().\n\
Example:\n\
i = ImageExport()\n\
i.type = 'PNG' # select one from i.allTypes list\n\
i.scale = 200 # I want to have 200%\n\
i.name = '/home/subik/test.png'\n\
i.save()\n\
\n\
two last lines should be replaced with:\n\
i.saveAs('/home/subik/test.png')");

PyDoc_STRVAR(imgexp_dpi__doc__, "This value will be used for export as DPI. Read/write integer.");
PyDoc_STRVAR(imgexp_scale__doc__, "This is the scaling of the image. 100 = 100% etc. Read/write iteger.");
PyDoc_STRVAR(imgexp_quality__doc__, "Quality/compression: minimum 1 (poor), maximum 100 (qaulity). Read/write integer.");
PyDoc_STRVAR(imgexp_filename__doc__, "Filename of the image. With or without path. Read/write string.");
PyDoc_STRVAR(imgexp_type__doc__, "Bitmap type. See allTypes list for more info. Read/write string.");
PyDoc_STRVAR(imgexp_alltypes__doc__, "Available types. Read only list of strings.");

PyDoc_STRVAR(imgexp_save__doc__, "save() -> boolean\n\nSaves image under previously set 'name'.");
PyDoc_STRVAR(imgexp_saveas__doc__, "saveAs('filename') -> boolean\n\nSaves image as 'filename'.");

// Nest items are not needed but are here for me to exercise
// writing complete python objects

#define ImageExport_Check(op) ((op)->ob_type == &ImageExport_Type)

#endif /* OBJIMGEXPORT_H */
