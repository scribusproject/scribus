/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "cmdpage.h"
#include "cmdutil.h"

PyObject *scribus_actualpage(PyObject */*self*/)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->PageNr + 1));
}

PyObject *scribus_redraw(PyObject */*self*/)
{
	if(!checkHaveDocument())
		return NULL;
	Carrier->view->DrawNew();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_savepageeps(PyObject */*self*/, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	bool ret = Carrier->DoSaveAsEps(QString::fromUtf8(Name));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to save EPS","python error"));
		return NULL;
	}
	Py_INCREF(Py_True);	// return True not None for backward compat
	return Py_True;
}

PyObject *scribus_deletepage(PyObject */*self*/, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	e--;
	if ((e < 0) || (e > static_cast<int>(Carrier->view->Pages.count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range","python error"));
		return NULL;
	}
	Carrier->DeletePage2(e);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_gotopage(PyObject */*self*/, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	e--;
	if ((e < 0) || (e > static_cast<int>(Carrier->view->Pages.count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range","python error"));
		return NULL;
	}
	Carrier->view->GotoPage(e);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_newpage(PyObject */*self*/, PyObject* args)
{
	int e;
	char *name = const_cast<char*>("Normal");
	if (!PyArg_ParseTuple(args, "i|es", &e, "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (e < 0)
		Carrier->slotNewPageP(Carrier->view->Pages.count(), QString::fromUtf8(name));
	else
	{
		e--;
		if ((e < 0) || (e > static_cast<int>(Carrier->view->Pages.count())-1))
		{
			PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range","python error"));
			return NULL;
		}
		Carrier->slotNewPageP(e, QString::fromUtf8(name));
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_pagecount(PyObject */*self*/)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(Carrier->view->Pages.count()));
}

PyObject *scribus_pagedimension(PyObject */*self*/)
{
	if(!checkHaveDocument())
		return NULL;
	PyObject *t;
	t = Py_BuildValue(
			"(dd)",
			PointToValue(Carrier->doc->PageB), // it's just view scale... * Carrier->doc->Scale),
			PointToValue(Carrier->doc->PageH)  // * Carrier->doc->Scale)
		);
	return t;
}

PyObject *scribus_getpageitems(PyObject */*self*/)
{
	if(!checkHaveDocument())
		return NULL;
	if (Carrier->doc->ActPage->Items.count() == 0)
		return Py_BuildValue((char*)"[]");
	PyObject *l = PyList_New(Carrier->doc->ActPage->Items.count());
	PyObject *row;
	for (uint i = 0; i<Carrier->doc->ActPage->Items.count(); ++i)
	{
		row = Py_BuildValue((char*)"(sii)",
		                    Carrier->doc->ActPage->Items.at(i)->AnName.ascii(),
		                    Carrier->doc->ActPage->Items.at(i)->PType,
		                    Carrier->doc->ActPage->Items.at(i)->ItemNr
		                   );
		PyList_SetItem(l, i, row);
	} // for
	return l;
}

PyObject *scribus_getHguides(PyObject */*self*/)
{
	if(!checkHaveDocument())
		return NULL;
	int n = Carrier->doc->ActPage->YGuides.count();
	if (n == 0)
		return Py_BuildValue((char*)"[]");
	int i;
	double tmp;
	PyObject *l, *guide;
	l = PyList_New(0);
	for (i=0; i<n; i++)
	{
		tmp = Carrier->doc->ActPage->YGuides[i];
		guide = Py_BuildValue("d", PointToValue(tmp));
		PyList_Append(l, guide);
	}
	return l;
}

PyObject *scribus_setHguides(PyObject */*self*/, PyObject* args)
{
	PyObject *l;
	if (!PyArg_ParseTuple(args, "O", &l))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values","python error"));
		return NULL;
	}
	int i, n;
	n = PyList_Size(l);
	double guide;
	Carrier->doc->ActPage->YGuides.clear();
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains non-numeric values: must be list of float values","python error"));
			return NULL;
		}
		Carrier->doc->ActPage->YGuides += ValueToPoint(guide);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getVguides(PyObject */*self*/)
{
	if(!checkHaveDocument())
		return NULL;
	int n = Carrier->doc->ActPage->XGuides.count();
	if (n == 0)
		return Py_BuildValue((char*)"[]");
	int i;
	double tmp;
	PyObject *l, *guide;
	l = PyList_New(0);
	for (i=0; i<n; i++)
	{
		tmp = Carrier->doc->ActPage->XGuides[i];
		guide = Py_BuildValue("d", PointToValue(tmp));
		PyList_Append(l, guide);
	}
	return l;
}

PyObject *scribus_setVguides(PyObject */*self*/, PyObject* args)
{
	PyObject *l;
	if (!PyArg_ParseTuple(args, "O", &l))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values","python error"));
		return NULL;
	}
	int i, n;
	n = PyList_Size(l);
	double guide;
	Carrier->doc->ActPage->XGuides.clear();
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains non-numeric values: must be list of float values","python error"));
			return NULL;
		}
		Carrier->doc->ActPage->XGuides += ValueToPoint(guide);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getpagemargins(PyObject */*self*/)
{
	PyObject *margins = NULL;
	if(!checkHaveDocument())
		return NULL;
	margins = Py_BuildValue("ffff", PointToValue(Carrier->doc->PageM.Top),
									PointToValue(Carrier->doc->PageM.Left),
									PointToValue(Carrier->doc->PageM.Right),
									PointToValue(Carrier->doc->PageM.Bottom));
	return margins;
}
