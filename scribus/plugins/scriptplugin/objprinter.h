#ifndef OBJPRINTER_H
#define OBJPRINTER_H

PyAPI_DATA(PyTypeObject) Printer_Type;

// Nest items are not needed but are here for me to exercise
// writing complete python objects

#define Printer_Check(op) ((op)->ob_type == &Printer_Type)


#endif /* OBJPRINTER_H */
