#include "cmddoc.h"
#include "cmdutil.h"
#include "units.h"
#include "documentinformation.h"

PyObject *scribus_newdoc(PyObject* /* self */, PyObject* args)
{
	double b, h, lr, tpr, btr, rr, ebr;
	int unit, ds, fsl, fNr, ori;
	PyObject *p, *m;
	if ((!PyArg_ParseTuple(args, "OOiiiii", &p, &m, &ori, &fNr, &unit, &ds, &fsl)) ||
	        (!PyArg_ParseTuple(p, "dd", &b, &h)) ||
	        (!PyArg_ParseTuple(m, "dddd", &lr, &rr, &tpr, &btr)))
		return NULL;
	b = value2pts(b, unit);
	h = value2pts(h, unit);
	if (ori == 1)
	{
		ebr = b;
		b = h;
		h = ebr;
	}
	tpr = value2pts(tpr, unit);
	lr = value2pts(lr, unit);
	rr = value2pts(rr, unit);
	btr = value2pts(btr, unit);
	bool ret = ScApp->doFileNew(b, h, tpr, lr, rr, btr, 0, 1, false, ds, unit, fsl, ori, fNr, "Custom");
	//	qApp->processEvents();
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_setmargins(PyObject* /* self */, PyObject* args)
{
	double lr, tpr, btr, rr;
	if (!PyArg_ParseTuple(args, "dddd", &lr, &rr, &tpr, &btr))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	tpr = ValueToPoint(tpr);
	lr = ValueToPoint(lr);
	rr = ValueToPoint(rr);
	btr = ValueToPoint(btr);
	ScApp->doc->resetPage(tpr, lr, rr, btr, ScApp->doc->currentPageLayout);
	ScApp->view->reformPages();
	ScApp->doc->setModified(true);
	ScApp->view->GotoPage(ScApp->doc->currentPage->pageNr());
	ScApp->view->DrawNew();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_closedoc(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScApp->doc->setModified(false);
	bool ret = ScApp->slotFileClose();
	qApp->processEvents();
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_havedoc(PyObject* /* self */)
{
	return PyInt_FromLong(static_cast<long>(ScApp->HaveDoc));
}

PyObject *scribus_opendoc(PyObject* /* self */, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	bool ret = ScApp->loadDoc(QString::fromUtf8(Name));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to open document.","python error"));
		return NULL;
	}
	Py_INCREF(Py_True); // compatibility: return true, not none, on success
	return Py_True;
}

PyObject *scribus_savedoc(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScApp->slotFileSave();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_savedocas(PyObject* /* self */, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	bool ret = ScApp->DoFileSave(QString::fromUtf8(Name));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to save document.","python error"));
		return NULL;
	}
	Py_INCREF(Py_True); // compatibility: return true, not none, on success
	return Py_True;
}

PyObject *scribus_setinfo(PyObject* /* self */, PyObject* args)
{
	char *Author;
	char *Title;
	char *Desc;
	// z means string, but None becomes a NULL value. QString()
	// will correctly handle NULL.
	if (!PyArg_ParseTuple(args, "zzz", &Author, &Title, &Desc))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	ScApp->doc->documentInfo.setAuthor(QString::fromUtf8(Author));
	ScApp->doc->documentInfo.setTitle(QString::fromUtf8(Title));
	ScApp->doc->documentInfo.setComments(QString::fromUtf8(Desc));
	ScApp->slotDocCh();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setunit(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((e < 0) || (e > 3))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Unit out of range. Use one of the scribus.UNIT_* constants.","python error"));
		return NULL;
	}
	ScApp->slotChangeUnit(e);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getunit(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(ScApp->doc->unitIndex()));
}

PyObject *scribus_loadstylesfromfile(PyObject* /* self */, PyObject *args)
{
	char *fileName;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &fileName))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	ScApp->doc->loadStylesFromFile(QString::fromUtf8(fileName));
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setdoctype(PyObject* /* self */, PyObject* args)
{
	int fp, fsl;
	if (!PyArg_ParseTuple(args, "ii", &fp, &fsl))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (ScApp->doc->currentPageLayout = fp)
		ScApp->doc->pageSets[ScApp->doc->currentPageLayout].FirstPage = fsl;
	ScApp->view->reformPages();
	ScApp->view->GotoPage(ScApp->doc->currentPage->pageNr()); // is this needed?
	ScApp->view->DrawNew();   // is this needed?
	//CB TODO ScApp->pagePalette->RebuildPage(); // is this needed?
	ScApp->slotDocCh();
	Py_INCREF(Py_None);
	return Py_None;
}
