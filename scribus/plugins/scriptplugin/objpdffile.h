#ifndef OBJPDFFILE_H
#define OBJPDFFILE_H

PyAPI_DATA(PyTypeObject) PDFfile_Type;

// Nest items are not needed but are here for me to exercise
// writing complete python objects

#define PDFfile_Check(op) ((op)->ob_type == &PDFfile_Type)


#endif /* OBJPDFFILE_H */
