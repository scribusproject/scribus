#ifndef OBJPDFFILE_H
#define OBJPDFFILE_H

// Pulls in <Python.h> first
#include "cmdvar.h"

PyAPI_DATA(PyTypeObject) PDFfile_Type;

// Nest items are not needed but are here for me to exercise
// writing complete python objects

#define PDFfile_Check(op) ((op)->ob_type == &PDFfile_Type)

PyDoc_STRVAR(pdffile__doc__,"Exporting PDF\n\
\n\
Class PDFfile() provides the PDF exporting\n\
for Python scripting as you know it from Save as PDF\n\
menu. \n\
Example:\n\
pdf = PDFfile()\n\
pdf.thumbnails = 1 # generate thumbnails too\n\
pdf.file = 'mypdf.pdf' \n\
pdf.save()");

#endif /* OBJPDFFILE_H */
