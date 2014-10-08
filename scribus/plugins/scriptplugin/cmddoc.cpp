/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmddoc.h"
#include "cmdutil.h"
#include "units.h"
#include "documentinformation.h"
#include "scribuscore.h"

/*
newDocument(size, margins, orientation, firstPageNumber,
            unit, pagesType, firstPageOrder)*/
PyObject *scribus_newdocument(PyObject* /* self */, PyObject* args)
{
	double topMargin, bottomMargin, leftMargin, rightMargin;
	double pageWidth, pageHeight;
	int orientation, firstPageNr, unit, pagesType, facingPages, firstPageOrder, numPages;

	PyObject *p, *m;

	if ((!PyArg_ParseTuple(args, "OOiiiiii", &p, &m, &orientation,
											&firstPageNr, &unit,
											&pagesType,
											&firstPageOrder,
											&numPages)) ||
						(!PyArg_ParseTuple(p, "dd", &pageWidth, &pageHeight)) ||
						(!PyArg_ParseTuple(m, "dddd", &leftMargin, &rightMargin,
												&topMargin, &bottomMargin)))
		return NULL;
	if (numPages <= 0)
		numPages = 1;
	if (pagesType == 0)
	{
		facingPages = 0;
		firstPageOrder = 0;
	}
	else
		facingPages = 1;
	// checking the bounds
	if (pagesType < firstPageOrder)
	{
		PyErr_SetString(ScribusException, QObject::tr("firstPageOrder is bigger than allowed.","python error").toLocal8Bit().constData());
		return NULL;
	}

	pageWidth  = value2pts(pageWidth, unit);
	pageHeight = value2pts(pageHeight, unit);
	if (orientation == 1)
	{
		double x = pageWidth;
		pageWidth = pageHeight;
		pageHeight = x;
	}
	leftMargin   = value2pts(leftMargin, unit);
	rightMargin  = value2pts(rightMargin, unit);
	topMargin    = value2pts(topMargin, unit);
	bottomMargin = value2pts(bottomMargin, unit);

	bool ret = ScCore->primaryMainWindow()->doFileNew(pageWidth, pageHeight,
								topMargin, leftMargin, rightMargin, bottomMargin,
								// autoframes. It's disabled in python
								// columnDistance, numberCols, autoframes,
								0, 1, false,
								pagesType, unit, firstPageOrder,
								orientation, firstPageNr, "Custom", true, numPages);
	ScCore->primaryMainWindow()->doc->pageSets[pagesType].FirstPage = firstPageOrder;

	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_newdoc(PyObject* /* self */, PyObject* args)
{
	qDebug("WARNING: newDoc() procedure is obsolete, it will be removed in a forthcoming release. Use newDocument() instead.");
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
	/*! \todo Obsolete! In the case of no facing pages use only firstpageleft
	scripter is not new-page-size ready.
	What is it: don't allow to use wrong FSL constant in the case of
	onesided document. */
	if (ds!=1 && fsl>0)
		fsl = 0;
	// end of hack

	tpr = value2pts(tpr, unit);
	lr  = value2pts(lr, unit);
	rr  = value2pts(rr, unit);
	btr = value2pts(btr, unit);

	bool ret = ScCore->primaryMainWindow()->doFileNew(b, h, tpr, lr, rr, btr, 0, 1, false, ds, unit, fsl, ori, fNr, "Custom", true);
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
	MarginStruct margins(ValueToPoint(tpr), ValueToPoint(lr), ValueToPoint(btr), ValueToPoint(rr));
	ScCore->primaryMainWindow()->doc->resetPage(margins, ScCore->primaryMainWindow()->doc->currentPageLayout);
	ScCore->primaryMainWindow()->view->reformPages();
	ScCore->primaryMainWindow()->doc->setModified(true);
	ScCore->primaryMainWindow()->view->GotoPage(ScCore->primaryMainWindow()->doc->currentPageNumber());
	ScCore->primaryMainWindow()->view->DrawNew();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_closedoc(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScCore->primaryMainWindow()->doc->setModified(false);
	bool ret = ScCore->primaryMainWindow()->slotFileClose();
	qApp->processEvents();
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_havedoc(PyObject* /* self */)
{
	return PyInt_FromLong(static_cast<long>(ScCore->primaryMainWindow()->HaveDoc));
}

PyObject *scribus_opendoc(PyObject* /* self */, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	bool ret = ScCore->primaryMainWindow()->loadDoc(QString::fromUtf8(Name));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to open document: %1","python error").arg(Name).toLocal8Bit().constData());
		return NULL;
	}
	return PyBool_FromLong(static_cast<long>(true));
//	Py_INCREF(Py_True); // compatibility: return true, not none, on success
//	return Py_True;
//	Py_RETURN_TRUE;
}

PyObject *scribus_savedoc(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScCore->primaryMainWindow()->slotFileSave();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_getdocname(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	if (! ScCore->primaryMainWindow()->doc->hasName)
	{
		return PyString_FromString("");
	}
	return PyString_FromString(ScCore->primaryMainWindow()->doc->DocName.toUtf8());
}

PyObject *scribus_savedocas(PyObject* /* self */, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	bool ret = ScCore->primaryMainWindow()->DoFileSave(QString::fromUtf8(Name));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to save document.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyBool_FromLong(static_cast<long>(true));
//	Py_INCREF(Py_True); // compatibility: return true, not none, on success
//	return Py_True;
//	Py_RETURN_TRUE;
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
	ScCore->primaryMainWindow()->doc->documentInfo.setAuthor(QString::fromUtf8(Author));
	ScCore->primaryMainWindow()->doc->documentInfo.setTitle(QString::fromUtf8(Title));
	ScCore->primaryMainWindow()->doc->documentInfo.setComments(QString::fromUtf8(Desc));
	ScCore->primaryMainWindow()->slotDocCh();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_setunit(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((e < UNITMIN) || (e > UNITMAX))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Unit out of range. Use one of the scribus.UNIT_* constants.","python error").toLocal8Bit().constData());
		return NULL;
	}
	ScCore->primaryMainWindow()->slotChangeUnit(e);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_getunit(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->unitIndex()));
}

PyObject *scribus_loadstylesfromfile(PyObject* /* self */, PyObject *args)
{
	char *fileName;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &fileName))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	ScCore->primaryMainWindow()->doc->loadStylesFromFile(QString::fromUtf8(fileName));
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_setdoctype(PyObject* /* self */, PyObject* args)
{
	int fp, fsl;
	if (!PyArg_ParseTuple(args, "ii", &fp, &fsl))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (ScCore->primaryMainWindow()->doc->currentPageLayout == fp)
		ScCore->primaryMainWindow()->doc->pageSets[ScCore->primaryMainWindow()->doc->currentPageLayout].FirstPage = fsl;
	ScCore->primaryMainWindow()->view->reformPages();
	ScCore->primaryMainWindow()->view->GotoPage(ScCore->primaryMainWindow()->doc->currentPageNumber()); // is this needed?
	ScCore->primaryMainWindow()->view->DrawNew();   // is this needed?
	//CB TODO ScCore->primaryMainWindow()->pagePalette->RebuildPage(); // is this needed?
	ScCore->primaryMainWindow()->slotDocCh();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_closemasterpage(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScCore->primaryMainWindow()->view->hideMasterPage();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_masterpagenames(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	PyObject* names = PyList_New(ScCore->primaryMainWindow()->doc->MasterPages.count());
	QMap<QString,int>::const_iterator it(ScCore->primaryMainWindow()->doc->MasterNames.constBegin());
	QMap<QString,int>::const_iterator itEnd(ScCore->primaryMainWindow()->doc->MasterNames.constEnd());
	int n = 0;
	for ( ; it != itEnd; ++it )
	{
		PyList_SET_ITEM(names, n++, PyString_FromString(it.key().toUtf8().data()) );
	}
	return names;
}

PyObject *scribus_editmasterpage(PyObject* /* self */, PyObject* args)
{
	char* name = 0;
	if (!PyArg_ParseTuple(args, "es", const_cast<char*>("utf-8"), &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	const QString masterPageName(name);
	const QMap<QString,int>& masterNames(ScCore->primaryMainWindow()->doc->MasterNames);
	const QMap<QString,int>::const_iterator it(masterNames.find(masterPageName));
	if ( it == masterNames.constEnd() )
	{
		PyErr_SetString(PyExc_ValueError, "Master page not found");
		return NULL;
	}
	ScCore->primaryMainWindow()->view->showMasterPage(*it);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject* scribus_createmasterpage(PyObject* /* self */, PyObject* args)
{
	char* name = 0;
	if (!PyArg_ParseTuple(args, "es", const_cast<char*>("utf-8"), &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	const QString masterPageName(name);
	if (ScCore->primaryMainWindow()->doc->MasterNames.contains(masterPageName))
	{
		PyErr_SetString(PyExc_ValueError, "Master page already exists");
		return NULL;
	}
	ScCore->primaryMainWindow()->doc->addMasterPage(ScCore->primaryMainWindow()->doc->MasterPages.count(), masterPageName);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject* scribus_deletemasterpage(PyObject* /* self */, PyObject* args)
{
	char* name = 0;
	if (!PyArg_ParseTuple(args, "es", const_cast<char*>("utf-8"), &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	const QString masterPageName(name);
	if (!ScCore->primaryMainWindow()->doc->MasterNames.contains(masterPageName))
	{
		PyErr_SetString(PyExc_ValueError, "Master page does not exist");
		return NULL;
	}
	if (masterPageName == "Normal")
	{
		PyErr_SetString(PyExc_ValueError, "Can not delete the Normal master page");
		return NULL;
	}
	bool oldMode = ScCore->primaryMainWindow()->doc->masterPageMode();
	ScCore->primaryMainWindow()->doc->setMasterPageMode(true);
	ScCore->primaryMainWindow()->deletePage2(ScCore->primaryMainWindow()->doc->MasterNames[masterPageName]);
	ScCore->primaryMainWindow()->doc->setMasterPageMode(oldMode);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject* scribus_applymasterpage(PyObject* /* self */, PyObject* args)
{
	char* name = 0;
	int page = 0;
	if (!PyArg_ParseTuple(args, "esi", const_cast<char*>("utf-8"), &name, &page))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	const QString masterPageName(name);
	if (!ScCore->primaryMainWindow()->doc->MasterNames.contains(masterPageName))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Master page does not exist: '%1'","python error").arg(masterPageName).toLocal8Bit().constData());
		return NULL;
	}
	if ((page < 1) || (page > static_cast<int>(ScCore->primaryMainWindow()->doc->Pages->count())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range: %1.","python error").arg(page).toLocal8Bit().constData());
		return NULL;
	}

	if (!ScCore->primaryMainWindow()->doc->applyMasterPage(masterPageName, page-1))
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to apply masterpage '%1' on page: %2","python error").arg(masterPageName).arg(page).toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmddocdocwarnings()
{
    QStringList s;
    s << scribus_newdocument__doc__ << scribus_newdoc__doc__ <<  scribus_closedoc__doc__ << scribus_havedoc__doc__ << scribus_opendoc__doc__ << scribus_savedoc__doc__ << scribus_getdocname__doc__ << scribus_savedocas__doc__ << scribus_setinfo__doc__ <<scribus_setmargins__doc__ <<scribus_setunit__doc__ <<scribus_getunit__doc__ <<scribus_loadstylesfromfile__doc__ <<scribus_setdoctype__doc__ <<scribus_closemasterpage__doc__ <<scribus_masterpagenames__doc__ <<scribus_editmasterpage__doc__ <<scribus_createmasterpage__doc__ <<scribus_deletemasterpage__doc__;
    s << scribus_applymasterpage__doc__;
}
