#include <Python.h>
#include "cmdpage.h"
#include "cmdvar.h"
#include "cmdutil.h"

PyObject *scribus_actualpage(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("currentPage()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	return PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->PageNr + 1));
}

PyObject *scribus_redraw(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("redrawAll()"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	Carrier->view->DrawNew();
	return Py_None;
}

PyObject *scribus_savepageeps(PyObject *self, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("savePageAsEPS(filename)"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	bool ret = Carrier->DoSaveAsEps(QString(Name));
	//	qApp->processEvents();
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_deletepage(PyObject *self, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("deletePage(pagenumber)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	e--;
	if ((e < 0) || (e > static_cast<int>(Carrier->view->Pages.count())-1))
		return Py_None;
	Carrier->DeletePage2(e);
	//	qApp->processEvents();
	return Py_None;
}

PyObject *scribus_gotopage(PyObject *self, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("gotoPage(pagenumber)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	e--;
	if ((e < 0) || (e > static_cast<int>(Carrier->view->Pages.count())-1))
		return Py_None;
	Carrier->view->GotoPage(e);
	//	qApp->processEvents();
	return Py_None;
}

PyObject *scribus_newpage(PyObject *self, PyObject* args)
{
	int e;
	char *name = "Normal";
	if (!PyArg_ParseTuple(args, "i|s", &e, &name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("newPage(pagenumber [, pagename])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	if (e < 0)
		Carrier->slotNewPageP(Carrier->view->Pages.count(), QString(name));
	else
	{
		e--;
		if ((e < 0) || (e > static_cast<int>(Carrier->view->Pages.count())-1))
			return Py_None;
		Carrier->slotNewPageP(e, QString(name));
	}
	//	qApp->processEvents();
	return Py_None;
}

PyObject *scribus_pagecount(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("pageCount()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	return PyInt_FromLong(static_cast<long>(Carrier->view->Pages.count()));
}

PyObject *scribus_pagedimension(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getPageSize()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	PyObject *t;
	t = Py_BuildValue(
	        "(dd)",
	        PointToValue(Carrier->doc->PageB), // it's just view scale... * Carrier->doc->Scale),
	        PointToValue(Carrier->doc->PageH)  // * Carrier->doc->Scale)
	    );
	return t;
}

PyObject *scribus_getpageitems(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getPageItems()"));
		return NULL;
	}
	if ((!Carrier->HaveDoc) || (Carrier->doc->ActPage->Items.count() == 0))
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

PyObject *scribus_getHguides(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getHGuides()"));
		return NULL;
	}
	int n;
	if (!Carrier->HaveDoc || (n = Carrier->doc->ActPage->YGuides.count()) == 0)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
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

PyObject *scribus_setHguides(PyObject *self, PyObject* args)
{
	PyObject *l;
	if (!PyArg_ParseTuple(args, "O", &l))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setHGuides(guides)"));
		return NULL;
	}
	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QString("argument is not list: must be list of float values"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i, n;
	n = PyList_Size(l);
	double guide;
	Carrier->doc->ActPage->YGuides.clear();
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QString("argument contains no-numeric values: must be list of float values"));
			return NULL;
		}
		Carrier->doc->ActPage->YGuides += ValueToPoint(guide);
	}
	return Py_None;
}

PyObject *scribus_getVguides(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getVGuides()"));
		return NULL;
	}
	int n;
	if (!Carrier->HaveDoc || (n = Carrier->doc->ActPage->XGuides.count()) == 0)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
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

PyObject *scribus_setVguides(PyObject *self, PyObject* args)
{
	PyObject *l;
	if (!PyArg_ParseTuple(args, "O", &l))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setVGuides(guides)"));
		return NULL;
	}
	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QString("argument is not list: must be list of float values"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i, n;
	n = PyList_Size(l);
	double guide;
	Carrier->doc->ActPage->XGuides.clear();
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QString("argument contains no-numeric values: must be list of float values"));
			return NULL;
		}
		Carrier->doc->ActPage->XGuides += ValueToPoint(guide);
	}
	return Py_None;
}

PyObject *scribus_getpagemargins(PyObject *self,  PyObject* args)
{
	PyObject *margins = NULL;
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getPageMargins()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		PyErr_SetString(PyExc_Exception, "No document open");
		return NULL;
	}
	margins = Py_BuildValue("ffff", Carrier->doc->PageM.Top, Carrier->doc->PageM.Left,
							 Carrier->doc->PageM.Right, Carrier->doc->PageM.Bottom);
	if (margins == NULL)
	{
		PyErr_SetString(PyExc_Exception, "Building margin tuple failed!");
		return NULL;
	}
	Py_INCREF(margins);
	return margins;
}
