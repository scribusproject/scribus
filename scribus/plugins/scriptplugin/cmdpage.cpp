/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdpage.h"
#include "cmdutil.h"
#include "page.h"

PyObject *scribus_actualpage(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(ScMW->doc->currentPageNumber() + 1));
}

PyObject *scribus_redraw(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScMW->view->DrawNew();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_savepageeps(PyObject* /* self */, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	bool ret = ScMW->DoSaveAsEps(QString::fromUtf8(Name));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to save EPS.","python error"));
		return NULL;
	}
	Py_INCREF(Py_True);	// return True not None for backward compat
	return Py_True;
}

PyObject *scribus_deletepage(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	e--;
	if ((e < 0) || (e > static_cast<int>(ScMW->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
		return NULL;
	}
	ScMW->DeletePage2(e);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_gotopage(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	e--;
	if ((e < 0) || (e > static_cast<int>(ScMW->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
		return NULL;
	}
	ScMW->view->GotoPage(e);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_newpage(PyObject* /* self */, PyObject* args)
{
	int e;
	char *name = const_cast<char*>("Normal");
	if (!PyArg_ParseTuple(args, "i|es", &e, "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (e < 0)
		ScMW->slotNewPageP(ScMW->doc->Pages->count(), QString::fromUtf8(name));
	else
	{
		e--;
		if ((e < 0) || (e > static_cast<int>(ScMW->doc->Pages->count())-1))
		{
			PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
			return NULL;
		}
		ScMW->slotNewPageP(e, QString::fromUtf8(name));
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_pagecount(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(ScMW->doc->Pages->count()));
}

PyObject *scribus_pagedimension(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	PyObject *t;
	t = Py_BuildValue(
			"(dd)",
			PointToValue(ScMW->doc->pageWidth), // it's just view scale... * ScMW->doc->Scale),
			PointToValue(ScMW->doc->pageHeight)  // * ScMW->doc->Scale)
		);
	return t;
}

PyObject *scribus_getpageitems(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	if (ScMW->doc->Items->count() == 0)
		return Py_BuildValue((char*)"[]");
	uint counter = 0;
	uint pageNr = ScMW->doc->currentPageNumber();
	for (uint lam2 = 0; lam2 < ScMW->doc->Items->count(); ++lam2)
	{
		if (pageNr == ScMW->doc->Items->at(lam2)->OwnPage)
			counter++;
	}
	PyObject *l = PyList_New(counter);
	PyObject *row;
	counter = 0;
	for (uint i = 0; i<ScMW->doc->Items->count(); ++i)
	{
		if (pageNr == ScMW->doc->Items->at(i)->OwnPage)
		{
			row = Py_BuildValue((char*)"(sii)",
			                    ScMW->doc->Items->at(i)->itemName().ascii(),
			                    ScMW->doc->Items->at(i)->itemType(),
			                    ScMW->doc->Items->at(i)->ItemNr
			                   );
			PyList_SetItem(l, counter, row);
			counter++;
		}
	} // for
	return l;
}

PyObject *scribus_getHguides(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	int n = ScMW->doc->currentPage->YGuides.count();
	if (n == 0)
		return Py_BuildValue((char*)"[]");
	int i;
	double tmp;
	PyObject *l, *guide;
	l = PyList_New(0);
	for (i=0; i<n; i++)
	{
		tmp = ScMW->doc->currentPage->YGuides[i];
		guide = Py_BuildValue("d", PointToValue(tmp));
		PyList_Append(l, guide);
	}
	return l;
}

PyObject *scribus_setHguides(PyObject* /* self */, PyObject* args)
{
	PyObject *l;
	if (!PyArg_ParseTuple(args, "O", &l))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values.","python error"));
		return NULL;
	}
	int i, n;
	n = PyList_Size(l);
	double guide;
	ScMW->doc->currentPage->YGuides.clear();
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains non-numeric values: must be list of float values.","python error"));
			return NULL;
		}
		ScMW->doc->currentPage->YGuides += ValueToPoint(guide);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getVguides(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	int n = ScMW->doc->currentPage->XGuides.count();
	if (n == 0)
		return Py_BuildValue((char*)"[]");
	int i;
	double tmp;
	PyObject *l, *guide;
	l = PyList_New(0);
	for (i=0; i<n; i++)
	{
		tmp = ScMW->doc->currentPage->XGuides[i];
		guide = Py_BuildValue("d", PointToValue(tmp));
		PyList_Append(l, guide);
	}
	return l;
}

PyObject *scribus_setVguides(PyObject* /* self */, PyObject* args)
{
	PyObject *l;
	if (!PyArg_ParseTuple(args, "O", &l))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values.","python error"));
		return NULL;
	}
	int i, n;
	n = PyList_Size(l);
	double guide;
	ScMW->doc->currentPage->XGuides.clear();
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains no-numeric values: must be list of float values.","python error"));
			return NULL;
		}
		ScMW->doc->currentPage->XGuides += ValueToPoint(guide);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getpagemargins(PyObject* /* self */)
{
	PyObject *margins = NULL;
	if(!checkHaveDocument())
		return NULL;
	margins = Py_BuildValue("ffff", PointToValue(ScMW->doc->pageMargins.Top),
									PointToValue(ScMW->doc->pageMargins.Left),
									PointToValue(ScMW->doc->pageMargins.Right),
									PointToValue(ScMW->doc->pageMargins.Bottom));
	return margins;
}
