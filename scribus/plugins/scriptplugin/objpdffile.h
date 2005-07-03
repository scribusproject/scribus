/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef OBJPDFFILE_H
#define OBJPDFFILE_H

// Pulls in <Python.h> first
#include "cmdvar.h"

PyAPI_DATA(PyTypeObject) PDFfile_Type;

// Nest items are not needed but are here for me to exercise
// writing complete python objects

#define PDFfile_Check(op) ((op)->ob_type == &PDFfile_Type)


#endif /* OBJPDFFILE_H */
