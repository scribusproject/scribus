/*
 * Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>
 *
 * For general Scribus (>=1.3.2) copyright and licensing information please refer
 * to the COPYING file provided with the program. Following this notice may exist
 * a copyright and/or license notice that predates the release of Scribus 1.3.2
 * for which a new license (GPL+exception) is in place.
 */

#include "cmdcell.h"
#include "cmdutil.h"
#include "pageitem_table.h"
#include "tableborder.h"

PyObject *scribus_setcelltext(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	char *text;
	if (!PyArg_ParseTuple(args, "iies|es", &row, &column, "utf-8", &text, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell text on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}
	table->cellAt(row, column).setText(QString::fromUtf8(text));
	Py_RETURN_NONE;
}

PyObject *scribus_getcellstyle(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	if (!PyArg_ParseTuple(args, "ii|es", &row, &column, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get cell style on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}
	return PyString_FromString(table->cellAt(row, column).style().toUtf8());
}

PyObject *scribus_setcellstyle(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	char *style;
	if (!PyArg_ParseTuple(args, "iies|es", &row, &column, "utf-8", &style, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell style on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}
	table->cellAt(row, column).setStyle(QString::fromUtf8(style));
	Py_RETURN_NONE;
}

PyObject *scribus_getcellrowspan(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	if (!PyArg_ParseTuple(args, "ii|es", &row, &column, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get cell row span from non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(table->cellAt(row, column).rowSpan()));
}

PyObject *scribus_getcellcolumnspan(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	if (!PyArg_ParseTuple(args, "ii|es", &row, &column, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get cell column span from non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(table->cellAt(row, column).columnSpan()));
}

PyObject *scribus_getcellfillcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	if (!PyArg_ParseTuple(args, "ii|es", &row, &column, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get cell fill color on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}
	return PyString_FromString(table->cellAt(row, column).fillColor().toUtf8());
}

PyObject *scribus_setcellfillcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	char *color;
	if (!PyArg_ParseTuple(args, "iies|es", &row, &column, "utf-8", &color, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell fill color on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}
	table->cellAt(row, column).setFillColor(QString::fromUtf8(color));
	Py_RETURN_NONE;
}

PyObject *scribus_setcellleftborder(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	PyObject* borderLines;
	if (!PyArg_ParseTuple(args, "iiO|es", &row, &column, &borderLines, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell left border on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}

	bool ok = false;
	TableBorder border = parseBorder(borderLines, &ok);
	if (ok)
		table->cellAt(row, column).setLeftBorder(border);
	else
		return NULL;

	Py_RETURN_NONE;
}

PyObject *scribus_setcellrightborder(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	PyObject* borderLines;
	if (!PyArg_ParseTuple(args, "iiO|es", &row, &column, &borderLines, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell right border on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}

	bool ok = false;
	TableBorder border = parseBorder(borderLines, &ok);
	if (ok)
		table->cellAt(row, column).setRightBorder(border);
	else
		return NULL;

	Py_RETURN_NONE;
}

PyObject *scribus_setcelltopborder(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	PyObject* borderLines;
	if (!PyArg_ParseTuple(args, "iiO|es", &row, &column, &borderLines, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell top border on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}

	bool ok = false;
	TableBorder border = parseBorder(borderLines, &ok);
	if (ok)
		table->cellAt(row, column).setTopBorder(border);
	else
		return NULL;

	Py_RETURN_NONE;
}

PyObject *scribus_setcellbottomborder(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	PyObject* borderLines;
	if (!PyArg_ParseTuple(args, "iiO|es", &row, &column, &borderLines, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell bottom border on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}

	bool ok = false;
	TableBorder border = parseBorder(borderLines, &ok);
	if (ok)
		table->cellAt(row, column).setBottomBorder(border);
	else
		return NULL;

	Py_RETURN_NONE;
}

PyObject *scribus_setcellleftpadding(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	double padding;
	if (!PyArg_ParseTuple(args, "iid|es", &row, &column, &padding, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell left padding on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}
	if (padding < 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cell padding must be >= 0.0", "python error").toLocal8Bit().constData());
		return NULL;
	}
	table->cellAt(row, column).setLeftPadding(padding);

	Py_RETURN_NONE;
}

PyObject *scribus_setcellrightpadding(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	double padding;
	if (!PyArg_ParseTuple(args, "iid|es", &row, &column, &padding, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell right padding on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}
	if (padding < 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cell padding must be >= 0.0", "python error").toLocal8Bit().constData());
		return NULL;
	}
	table->cellAt(row, column).setRightPadding(padding);

	Py_RETURN_NONE;
}

PyObject *scribus_setcelltoppadding(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	double padding;
	if (!PyArg_ParseTuple(args, "iid|es", &row, &column, &padding, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell top padding on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}
	if (padding < 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cell padding must be >= 0.0", "python error").toLocal8Bit().constData());
		return NULL;
	}
	table->cellAt(row, column).setTopPadding(padding);

	Py_RETURN_NONE;
}

PyObject *scribus_setcellbottompadding(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int row, column;
	double padding;
	if (!PyArg_ParseTuple(args, "iid|es", &row, &column, &padding, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	PageItem_Table *table = i->asTable();
	if (!table)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set cell bottom padding on a non-table item.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (column < 0 || column >= table->columns() || row < 0 || row >= table->rows())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("The cell %1,%2 does not exist in table", "python error").arg(row).arg(column).toLocal8Bit().constData());
		return NULL;
	}
	if (padding < 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cell padding must be >= 0.0", "python error").toLocal8Bit().constData());
		return NULL;
	}
	table->cellAt(row, column).setBottomPadding(padding);

	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdcelldocwarnings()
{
	QStringList s;
	s << scribus_setcelltext__doc__ << scribus_getcellstyle__doc__
	  << scribus_setcellstyle__doc__ << scribus_getcellrowspan__doc__
	  << scribus_getcellcolumnspan__doc__ << scribus_getcellfillcolor__doc__
	  << scribus_setcellfillcolor__doc__ << scribus_setcellleftborder__doc__
	  << scribus_setcellrightborder__doc__ << scribus_setcelltopborder__doc__
	  << scribus_setcellbottomborder__doc__ << scribus_setcellleftpadding__doc__
	  << scribus_setcellrightpadding__doc__ << scribus_setcelltoppadding__doc__
	  << scribus_setcellbottompadding__doc__;
}
