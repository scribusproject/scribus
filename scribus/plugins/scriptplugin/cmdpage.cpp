#include "cmdpage.h"
#include "cmdutil.h"
#include "page.h"

PyObject *scribus_actualpage(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(ScApp->doc->currentPageNumber() + 1));
}

PyObject *scribus_redraw(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScApp->view->DrawNew();
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
	bool ret = ScApp->DoSaveAsEps(QString::fromUtf8(Name));
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
	if ((e < 0) || (e > static_cast<int>(ScApp->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
		return NULL;
	}
	ScApp->DeletePage2(e);
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
	if ((e < 0) || (e > static_cast<int>(ScApp->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
		return NULL;
	}
	ScApp->view->GotoPage(e);
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
		ScApp->slotNewPageP(ScApp->doc->Pages->count(), QString::fromUtf8(name));
	else
	{
		e--;
		if ((e < 0) || (e > static_cast<int>(ScApp->doc->Pages->count())-1))
		{
			PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
			return NULL;
		}
		ScApp->slotNewPageP(e, QString::fromUtf8(name));
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_pagecount(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(ScApp->doc->Pages->count()));
}

PyObject *scribus_pagedimension(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	PyObject *t;
	t = Py_BuildValue(
			"(dd)",
			PointToValue(ScApp->doc->pageWidth), // it's just view scale... * ScApp->doc->Scale),
			PointToValue(ScApp->doc->pageHeight)  // * ScApp->doc->Scale)
		);
	return t;
}

PyObject *scribus_getpageitems(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	if (ScApp->doc->Items->count() == 0)
		return Py_BuildValue((char*)"[]");
	uint counter = 0;
	uint pageNr = ScApp->doc->currentPageNumber();
	for (uint lam2 = 0; lam2 < ScApp->doc->Items->count(); ++lam2)
	{
		if (pageNr == ScApp->doc->Items->at(lam2)->OwnPage)
			counter++;
	}
	PyObject *l = PyList_New(counter);
	PyObject *row;
	counter = 0;
	for (uint i = 0; i<ScApp->doc->Items->count(); ++i)
	{
		if (pageNr == ScApp->doc->Items->at(i)->OwnPage)
		{
			row = Py_BuildValue((char*)"(sii)",
			                    ScApp->doc->Items->at(i)->itemName().ascii(),
			                    ScApp->doc->Items->at(i)->itemType(),
			                    ScApp->doc->Items->at(i)->ItemNr
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
	int n = ScApp->doc->currentPage->YGuides.count();
	if (n == 0)
		return Py_BuildValue((char*)"[]");
	int i;
	double tmp;
	PyObject *l, *guide;
	l = PyList_New(0);
	for (i=0; i<n; i++)
	{
		tmp = ScApp->doc->currentPage->YGuides[i];
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
	ScApp->doc->currentPage->YGuides.clear();
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains non-numeric values: must be list of float values.","python error"));
			return NULL;
		}
		ScApp->doc->currentPage->YGuides += ValueToPoint(guide);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getVguides(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	int n = ScApp->doc->currentPage->XGuides.count();
	if (n == 0)
		return Py_BuildValue((char*)"[]");
	int i;
	double tmp;
	PyObject *l, *guide;
	l = PyList_New(0);
	for (i=0; i<n; i++)
	{
		tmp = ScApp->doc->currentPage->XGuides[i];
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
	ScApp->doc->currentPage->XGuides.clear();
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains no-numeric values: must be list of float values.","python error"));
			return NULL;
		}
		ScApp->doc->currentPage->XGuides += ValueToPoint(guide);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getpagemargins(PyObject* /* self */)
{
	PyObject *margins = NULL;
	if(!checkHaveDocument())
		return NULL;
	margins = Py_BuildValue("ffff", PointToValue(ScApp->doc->pageMargins.Top),
									PointToValue(ScApp->doc->pageMargins.Left),
									PointToValue(ScApp->doc->pageMargins.Right),
									PointToValue(ScApp->doc->pageMargins.Bottom));
	return margins;
}
