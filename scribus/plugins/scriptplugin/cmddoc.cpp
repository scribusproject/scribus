#include <Python.h>
#include "cmddoc.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_newdoc(PyObject *self, PyObject* args)
{
	double b, h, lr, tpr, btr, rr, ebr;
	int unit, ds, fsl, fNr, ori;
	PyObject *p, *m;
	if ((!PyArg_ParseTuple(args, "OOiiiii", &p, &m, &ori, &fNr, &unit, &ds, &fsl)) ||
	        (!PyArg_ParseTuple(p, "dd", &b, &h)) ||
	        (!PyArg_ParseTuple(m, "dddd", &lr, &rr, &tpr, &btr)))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("newDoc(size,margins,orientation,firstPageNumber,unit,FacingPages,FirstSideLeft)"));
		return NULL;
	}
	b = ValToPts(b, unit);
	h = ValToPts(h, unit);
	if (ori == 1)
	{
		ebr = b;
		b = h;
		h = ebr;
	}
	tpr = ValToPts(tpr, unit);
	lr = ValToPts(lr, unit);
	rr = ValToPts(rr, unit);
	btr = ValToPts(btr, unit);
	bool ret = Carrier->doFileNew(b, h, tpr, lr, rr, btr, 0, 1, false, ds, unit, fsl, ori, fNr, "Custom");
	//	qApp->processEvents();
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_setmargins(PyObject *self, PyObject* args)
{
	double lr, tpr, btr, rr;
	if (!PyArg_ParseTuple(args, "dddd", &lr, &rr, &tpr, &btr))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setMargins(lr,rr,tr,br)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	tpr = ValueToPoint(tpr);
	lr = ValueToPoint(lr);
	rr = ValueToPoint(rr);
	btr = ValueToPoint(btr);
	Carrier->doc->resetPage(tpr, lr, rr, btr, Carrier->doc->PageFP);
	Carrier->view->reformPages();
	Carrier->doc->setModified();
	Carrier->view->GotoPage(Carrier->doc->ActPage->PageNr);
	Carrier->view->DrawNew();
	return Py_None;
}

PyObject *scribus_closedoc(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("closeDoc()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	Carrier->doc->setUnModified();
	bool ret = Carrier->slotFileClose();
	qApp->processEvents();
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_havedoc(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("haveDoc()"));
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(Carrier->HaveDoc));
}

PyObject *scribus_opendoc(PyObject *self, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("openDoc(docname)"));
		return NULL;
	}
	bool ret = Carrier->LadeDoc(QString(Name));
	//	qApp->processEvents();
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_savedoc(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("saveDoc()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	Carrier->slotFileSave();
	//	qApp->processEvents();
	return PyInt_FromLong(0L);
}

PyObject *scribus_savedocas(PyObject *self, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("saveDocAs(docname)"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	bool ret = Carrier->DoFileSave(QString(Name));
	//	qApp->processEvents();
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_setinfo(PyObject *self, PyObject* args)
{
	char *Author;
	char *Title;
	char *Desc;
	if (!PyArg_ParseTuple(args, "zzz", &Author, &Title, &Desc))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setInfo(Author, Info, Description)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	Carrier->doc->DocAutor = QString(Author);
	Carrier->doc->DocTitel = QString(Title);
	Carrier->doc->DocComments = QString(Desc);
	Carrier->slotDocCh();
	return Py_None;
}

PyObject *scribus_setunit(PyObject *self, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setUnit(unit)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || ((e < 0) || (e > 3)))
		return Py_None;
	Carrier->slotChangeUnit(e);
	return Py_None;
}

PyObject *scribus_getunit(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getUnit()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	return PyInt_FromLong(static_cast<long>(Carrier->doc->Einheit));
}

PyObject *scribus_loadstylesfromfile(PyObject *self, PyObject *args)
{
	char *fileName;
	if (!PyArg_ParseTuple(args, "s", &fileName))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("loadStylesFromFile(filename)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	Carrier->doc->loadStylesFromFile(QString(fileName));
	return Py_None;
}

PyObject *scribus_setdoctype(PyObject *self, PyObject* args)
{
	int fp, fsl;
	if (!PyArg_ParseTuple(args, "ii", &fp, &fsl))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setDocType(FacingPages, FirstPageLeft)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	if (Carrier->doc->PageFP = fp)
		Carrier->doc->FirstPageLeft = fsl;
	Carrier->view->reformPages();
	Carrier->view->GotoPage(Carrier->doc->ActPage->PageNr); // is this needed?
	Carrier->view->DrawNew();   // is this needed?
	Carrier->Sepal->RebuildPage(); // is this needed?
	Carrier->slotDocCh();
	return Py_None;
}
