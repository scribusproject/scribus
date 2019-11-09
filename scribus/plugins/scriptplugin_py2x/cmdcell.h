/*
 * Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>
 *
 * For general Scribus (>=1.3.2) copyright and licensing information please refer
 * to the COPYING file provided with the program. Following this notice may exist
 * a copyright and/or license notice that predates the release of Scribus 1.3.2
 * for which a new license (GPL+exception) is in place.
 */

// Pulls in <Python.h> first
#include "cmdvar.h"

/*! Table cell handling */

#ifndef CMDCELL_H
#define CMDCELL_H

/*! docstring */
PyDoc_STRVAR(scribus_setcelltext__doc__,
QT_TR_NOOP("setCellText(row, column, text, [\"name\"])\n\
\n\
Sets the text of the cell at \"row\", \"column\" in the table \"name\" to \"text\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if the cell does not exist.\n\
"));
/*! Set cell text */
PyObject *scribus_setcelltext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcellstyle__doc__,
QT_TR_NOOP("getCellStyle(row, column, [\"name\"]) -> string\n\
\n\
Returns the named style of the cell at \"row\", \"column\" in the table \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if the cell does not exist.\n\
"));
/*! Get cell style */
PyObject *scribus_getcellstyle(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcellstyle__doc__,
QT_TR_NOOP("setCellStyle(row, column, style, [\"name\"])\n\
\n\
Sets the named style of the cell at \"row\", \"column\" in the table \"name\" to \"style\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if the cell does not exist.\n\
"));
/*! Set cell style */
PyObject *scribus_setcellstyle(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcellrowspan__doc__,
QT_TR_NOOP("getCellRowSpan(row, column, [\"name\"]) -> int\n\
\n\
Returns the row span of the cell at \"row\", \"column\" in the table \"name\" or -1 if the\n\
cell does not exist. If the cell is covered by another spanning cell, the row span of the\n\
spanning cell is returned. If \"name\" is not given the currently selected item is used.\n\
"));
/*! Get cell row span */
PyObject *scribus_getcellrowspan(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcellcolumnspan__doc__,
QT_TR_NOOP("getCellColumnSpan(row, column, [\"name\"]) -> int\n\
\n\
Returns the column span of the cell at \"row\", \"column\" in the table \"name\" or -1 if the\n\
cell does not exist. If the cell is covered by another spanning cell, the column span of the\n\
spanning cell is returned. If \"name\" is not given the currently selected item is used.\n\
"));
/*! Get cell column span */
PyObject *scribus_getcellcolumnspan(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcellfillcolor__doc__,
QT_TR_NOOP("getCellFillColor(row, column, [\"name\"]) -> string\n\
\n\
Returns the fill color of the cell at \"row\", \"column\" in the table \"name\"\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Get cell fill color */
PyObject *scribus_getcellfillcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcellfillcolor__doc__,
QT_TR_NOOP("setCellFillColor(row, column, color, [\"name\"])\n\
\n\
Sets the fill color of the cell at \"row\", \"column\" in the table \"name\"\n\
to \"color\". If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError the cell does not exist.\n\
"));
/*! Set cell fill color */
PyObject *scribus_setcellfillcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcellleftborder__doc__,
QT_TR_NOOP("setCellLeftBorder(row, column, borderLines, [\"name\"])\n\
\n\
Sets the left border of the cell at \"row\", \"column\" in the table \"name\".\n\
The border is specified as a list of \"(width, style, color)\" tuples.\n\
\"style\" is one of the LINE_* constants. If \"name\" is not given\n\
the currently selected item is used.\n\
\n\
May throw ValueError the cell does not exist or if \"borderLines\" is of the wrong format.\n\
"));
/*! Set cell left border */
PyObject *scribus_setcellleftborder(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcellrightborder__doc__,
QT_TR_NOOP("setCellRightBorder(row, column, borderLines, [\"name\"])\n\
\n\
Sets the right border of the cell at \"row\", \"column\" in the table \"name\".\n\
The border is specified as a list of \"(width, style, color)\" tuples.\n\
\"style\" is one of the LINE_* constants. If \"name\" is not given\n\
the currently selected item is used.\n\
\n\
May throw ValueError the cell does not exist or if \"borderLines\" is of the wrong format.\n\
"));
/*! Set cell right border */
PyObject *scribus_setcellrightborder(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcelltopborder__doc__,
QT_TR_NOOP("setCellTopBorder(row, column, borderLines, [\"name\"])\n\
\n\
Sets the top border of the cell at \"row\", \"column\" in the table \"name\".\n\
The border is specified as a list of \"(width, style, color)\" tuples.\n\
\"style\" is one of the LINE_* constants. If \"name\" is not given\n\
the currently selected item is used.\n\
\n\
May throw ValueError the cell does not exist or if \"borderLines\" is of the wrong format.\n\
"));
/*! Set cell top border */
PyObject *scribus_setcelltopborder(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcellbottomborder__doc__,
QT_TR_NOOP("setCellBottomBorder(row, column, borderLines, [\"name\"])\n\
\n\
Sets the bottom border of the cell at \"row\", \"column\" in the table \"name\".\n\
The border is specified as a list of \"(width, style, color)\" tuples.\n\
\"style\" is one of the LINE_* constants. If \"name\" is not given\n\
the currently selected item is used.\n\
\n\
May throw ValueError the cell does not exist or if \"borderLines\" is of the wrong format.\n\
"));
/*! Set cell bottom border */
PyObject *scribus_setcellbottomborder(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcellleftpadding__doc__,
QT_TR_NOOP("setCellLeftPadding(row, column, padding, [\"name\"])\n\
\n\
Sets the left padding of the cell at \"row\", \"column\" in the table \"name\"\n\
to \"padding\". If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError the cell does not exist or if \"padding\" less than 0.\n\
"));
/*! Set cell left padding */
PyObject *scribus_setcellleftpadding(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcellrightpadding__doc__,
QT_TR_NOOP("setCellRightPadding(row, column, padding, [\"name\"])\n\
\n\
Sets the right padding of the cell at \"row\", \"column\" in the table \"name\"\n\
to \"padding\". If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError the cell does not exist or if \"padding\" less than 0.\n\
"));
/*! Set cell right padding */
PyObject *scribus_setcellrightpadding(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcelltoppadding__doc__,
QT_TR_NOOP("setCellTopPadding(row, column, padding, [\"name\"])\n\
\n\
Sets the top padding of the cell at \"row\", \"column\" in the table \"name\"\n\
to \"padding\". If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError the cell does not exist or if \"padding\" is less than 0.\n\
"));
/*! Set cell top padding */
PyObject *scribus_setcelltoppadding(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcellbottompadding__doc__,
QT_TR_NOOP("setCellBottomPadding(row, column, padding, [\"name\"])\n\
\n\
Sets the bottom padding of the cell at \"row\", \"column\" in the table \"name\"\n\
to \"padding\". If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError the cell does not exist or if \"padding\" is less than 0.\n\
"));
/*! Set cell bottom padding */
PyObject *scribus_setcellbottompadding(PyObject * /*self*/, PyObject* args);

#endif // CMDCELL_H
