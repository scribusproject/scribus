#include <Python.h>
#include "cmdpage.h"
#include "cmdvar.h"
#include "cmdutil.h"

PyObject *scribus_actualpage(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	return PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->PageNr + 1));
}

PyObject *scribus_redraw(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
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
		return NULL;
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
		return NULL;
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
		return NULL;
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
		return NULL;
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
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	return PyInt_FromLong(static_cast<long>(Carrier->view->Pages.count()));
}

PyObject *scribus_pagedimension(PyObject *self, PyObject *args)
{
/* #include "cmdutil.h" needed for PointToValue function */
	if (!PyArg_ParseTuple(args, "")) {
		return NULL;
	}
	if (!Carrier->HaveDoc) {
		return PyInt_FromLong(0L);
	}
	
	PyObject *t;
	t = Py_BuildValue(
		"(dd)",
		PointToValue(Carrier->doc->PageB), // it's just view scale... * Carrier->doc->Scale),
		PointToValue(Carrier->doc->PageH)  // * Carrier->doc->Scale)
		);
	return t;
}

