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

/*! Table handling */

#ifndef CMDTABLE_H
#define CMDTABLE_H

/*! docstring */
PyDoc_STRVAR(scribus_gettablerows__doc__,
QT_TR_NOOP("getTableRows([\"name\"]) -> integer\n\
\n\
Gets the number of rows in the table \"name\". If \"name\" is not\n\
given the currently selected item is used.\n\
"));
/*! Get number of table rows */
PyObject *scribus_gettablerows(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettablecolumns__doc__,
QT_TR_NOOP("getTableColumns([\"name\"]) -> integer\n\
\n\
Gets the number of columns in the table \"name\". If \"name\" is not\n\
given the currently selected item is used.\n\
"));
/*! Get number of table columns */
PyObject *scribus_gettablecolumns(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_inserttablerows__doc__,
QT_TR_NOOP("insertTableRows(index, numRows, [\"name\"])\n\
\n\
Inserts \"numRows\" rows before the row at \"index\" in the table \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if number of rows is not at least one or index is out of bounds.\n\
"));
/*! Insert table rows */
PyObject *scribus_inserttablerows(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_removetablerows__doc__,
QT_TR_NOOP("removeTableRows(index, numRows, [\"name\"])\n\
\n\
Removes \"numRows\" rows from the table \"name\" starting with the row at \"index\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if number of rows is not at least one or the range to be deleted is out of bounds.\n\
"));
/*! Remove table rows */
PyObject *scribus_removetablerows(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettablerowheight__doc__,
QT_TR_NOOP("getTableRowHeight(row, [\"name\"]) -> float\n\
\n\
Returns the row height of \"row\" in the table \"name\" expressed in\n\
points, or 0.0 if the row does not exist. If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Get table row height */
PyObject *scribus_gettablerowheight(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_resizetablerow__doc__,
QT_TR_NOOP("resizeTableRow(row, height, [\"name\"])\n\
\n\
Resizes \"row\" to \"height\" in the table \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if the height is less than 0 or the row does not exist.\n\
"));
/*! Resize table row */
PyObject *scribus_resizetablerow(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_inserttablecolumns__doc__,
QT_TR_NOOP("insertTableColumns(index, numColumns, [\"name\"])\n\
\n\
Inserts \"numColumns\" columns before the column at \"index\" in the table \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if number of columns is not at least one or index is out of bounds.\n\
"));
/*! Insert table columns */
PyObject *scribus_inserttablecolumns(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_removetablecolumns__doc__,
QT_TR_NOOP("removeTableColumns(index, numColumns, [\"name\"])\n\
\n\
Removes \"numColumns\" columns from the table \"name\" starting with the column at \"index\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if number of columns is not at least one or the range to be deleted is out of bounds.\n\
"));
/*! Remove table columns */
PyObject *scribus_removetablecolumns(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettablecolumnwidth__doc__,
QT_TR_NOOP("getTableColumnWidth(column, [\"name\"]) -> float\n\
\n\
Returns the column width of \"column\" in the table \"name\" expressed in\n\
points, or 0.0 if the column does not exist. If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Get table column width */
PyObject *scribus_gettablecolumnwidth(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_resizetablecolumn__doc__,
QT_TR_NOOP("resizeTableColumn(column, width, [\"name\"])\n\
\n\
Resizes \"column\" to \"width\" in the table \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if the width is less than 0 or the column does not exist.\n\
"));
/*! Resize table column */
PyObject *scribus_resizetablecolumn(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_mergetablecells__doc__,
QT_TR_NOOP("mergeTableCells(row, column, numRows, numColumns, [\"name\"])\n\
\n\
Merges the cell at the specified \"row\" and \"column\" with the adjacent cells into one cell.\n\
\n\
May throw ValueError if number if numRows or numColumns is less than 1 or the specified area is out of bounds.\n\
"));
/*! Merge table cells */
PyObject *scribus_mergetablecells(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettablestyle__doc__,
QT_TR_NOOP("getTableStyle([\"name\"]) -> string\n\
\n\
Returns the named style of the table \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Get table style */
PyObject *scribus_gettablestyle(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settablestyle__doc__,
QT_TR_NOOP("setTableStyle(style, [\"name\"])\n\
\n\
Sets the named style of the table \"name\" to \"style\".\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Set table style */
PyObject *scribus_settablestyle(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettablefillcolor__doc__,
QT_TR_NOOP("getTableFillColor([\"name\"]) -> string\n\
\n\
Returns the fill color of the table \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Get table fill color */
PyObject *scribus_gettablefillcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settablefillcolor__doc__,
QT_TR_NOOP("setTableFillColor(color, [\"name\"])\n\
\n\
Sets the fill color of the table \"name\" to \"color\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError the table does not exist.\n\
"));
/*! Set table fill color */
PyObject *scribus_settablefillcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settableleftborder__doc__,
QT_TR_NOOP("setTableLeftBorder(borderLines, [\"name\"])\n\
\n\
Sets the left border of the table \"name\".\n\
The border is specified as a list of \"(width, style, color)\" tuples.\n\
\"style\" is one of the LINE_* constants. If \"name\" is not given\n\
the currently selected item is used.\n\
\n\
May throw ValueError if \"borderLines\" is of the wrong format.\n\
"));
/*! Set table left border */
PyObject *scribus_settableleftborder(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settablerightborder__doc__,
QT_TR_NOOP("setTableRightBorder(borderLines, [\"name\"])\n\
\n\
Sets the right border of the table \"name\".\n\
The border is specified as a list of \"(width, style, color)\" tuples.\n\
\"style\" is one of the LINE_* constants. If \"name\" is not given\n\
the currently selected item is used.\n\
\n\
May throw ValueError if \"borderLines\" is of the wrong format.\n\
"));
/*! Set table right border */
PyObject *scribus_settablerightborder(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settabletopborder__doc__,
QT_TR_NOOP("setTableTopBorder(borderLines, [\"name\"])\n\
\n\
Sets the top border of the table \"name\".\n\
The border is specified as a list of \"(width, style, color)\" tuples.\n\
\"style\" is one of the LINE_* constants. If \"name\" is not given\n\
the currently selected item is used.\n\
\n\
May throw ValueError if \"borderLines\" is of the wrong format.\n\
"));
/*! Set table top border */
PyObject *scribus_settabletopborder(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settablebottomborder__doc__,
QT_TR_NOOP("setTableBottomBorder(borderLines, [\"name\"])\n\
\n\
Sets the bottom border of the table \"name\".\n\
The border is specified as a list of \"(width, style, color)\" tuples.\n\
\"style\" is one of the LINE_* constants. If \"name\" is not given\n\
the currently selected item is used.\n\
\n\
May throw ValueError if \"borderLines\" is of the wrong format.\n\
"));
/*! Set table bottom border */
PyObject *scribus_settablebottomborder(PyObject * /*self*/, PyObject* args);

#endif // CMDTABLE_H
