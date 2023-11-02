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
#include "scribusdoc.h"
#include "scribusview.h"

#include <QApplication>

/*
newDocument(size, margins, orientation, firstPageNumber,
            unit, pagesType, firstPageOrder)*/
PyObject *scribus_newdocument(PyObject* /* self */, PyObject* args)
{
	double topMargin, bottomMargin, leftMargin, rightMargin;
	double pageWidth, pageHeight;
//	int orientation, firstPageNr, unit, pagesType, facingPages, firstPageOrder, numPages;
	int orientation, firstPageNr, unit, pagesType, firstPageOrder, numPages;

	PyObject *p, *m;

	if ((!PyArg_ParseTuple(args, "OOiiiiii", &p, &m, &orientation,
											&firstPageNr, &unit,
											&pagesType,
											&firstPageOrder,
											&numPages)) ||
						(!PyArg_ParseTuple(p, "dd", &pageWidth, &pageHeight)) ||
						(!PyArg_ParseTuple(m, "dddd", &leftMargin, &rightMargin,
												&topMargin, &bottomMargin)))
		return nullptr;
	if (numPages <= 0)
		numPages = 1;
	if (pagesType == 0)
	{
	//	facingPages = 0;
		firstPageOrder = 0;
	}
//	else
//		facingPages = 1;
	// checking the bounds
	if (pagesType < firstPageOrder)
	{
		PyErr_SetString(ScribusException, QObject::tr("firstPageOrder is bigger than allowed.","python error").toLocal8Bit().constData());
		return nullptr;
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
	ScCore->primaryMainWindow()->doc->setPageSetFirstPage(pagesType, firstPageOrder);

	return PyLong_FromLong(static_cast<long>(ret));
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
		return nullptr;
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
	if (ds != 1 && fsl > 0)
		fsl = 0;
	// end of hack

	tpr = value2pts(tpr, unit);
	lr  = value2pts(lr, unit);
	rr  = value2pts(rr, unit);
	btr = value2pts(btr, unit);
	bool ret = ScCore->primaryMainWindow()->doFileNew(b, h, tpr, lr, rr, btr, 0, 1, false, ds, unit, fsl, ori, fNr, "Custom", true);
	//	qApp->processEvents();
	return PyLong_FromLong(static_cast<long>(ret));
}

PyObject *scribus_setbleeds(PyObject */* self */, PyObject *args)
{
	double lr, tpr, btr, rr;
	if (!PyArg_ParseTuple(args, "dddd", &lr, &rr, &tpr, &btr))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	MarginStruct bleeds(ValueToPoint(tpr), ValueToPoint(lr), ValueToPoint(btr), ValueToPoint(rr));

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	currentDoc->setBleeds(bleeds);
	currentView->reformPages();
	currentDoc->setModified(true);
	currentView->DrawNew();
	Py_RETURN_NONE;
}

PyObject *scribus_setmargins(PyObject* /* self */, PyObject* args)
{
	double lr, tpr, btr, rr;
	if (!PyArg_ParseTuple(args, "dddd", &lr, &rr, &tpr, &btr))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	MarginStruct margins(ValueToPoint(tpr), ValueToPoint(lr), ValueToPoint(btr), ValueToPoint(rr));

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	currentDoc->setMargins(margins);
	currentView->reformPages();
	currentDoc->setModified(true);
	currentView->GotoPage(currentDoc->currentPageNumber());
	currentView->DrawNew();

	Py_RETURN_NONE;
}

PyObject *scribus_setbaseline(PyObject* /* self */, PyObject* args)
{
	double grid, offset;
	if (!PyArg_ParseTuple(args, "dd", &grid, &offset))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	currentDoc->guidesPrefs().valueBaselineGrid = ValueToPoint(grid);
	currentDoc->guidesPrefs().offsetBaselineGrid = ValueToPoint(offset);
	//currentView->reformPages();
	currentDoc->setModified(true);
	//currentView->GotoPage(currentDoc->currentPageNumber());
	currentView->DrawNew();

	Py_RETURN_NONE;
}


PyObject *scribus_closedoc(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->doc->setModified(false);
	bool ret = ScCore->primaryMainWindow()->slotFileClose();
	qApp->processEvents();
	return PyLong_FromLong(static_cast<long>(ret));
}

PyObject *scribus_havedoc(PyObject* /* self */)
{
	return PyLong_FromLong(static_cast<long>(ScCore->primaryMainWindow()->HaveDoc));
}

PyObject *scribus_opendoc(PyObject* /* self */, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return nullptr;
	bool ret = ScCore->primaryMainWindow()->loadDoc(QString::fromUtf8(Name));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to open document: %1","python error").arg(Name).toLocal8Bit().constData());
		return nullptr;
	}
	return PyBool_FromLong(static_cast<long>(true));
//	Py_INCREF(Py_True); // compatibility: return true, not none, on success
//	return Py_True;
//	Py_RETURN_TRUE;
}

PyObject *scribus_savedoc(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->slotFileSave();
	Py_RETURN_NONE;
}

PyObject *scribus_revertdoc(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->slotFileRevert();
	Py_RETURN_NONE;
}

PyObject *scribus_getdocname(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	if (! ScCore->primaryMainWindow()->doc->hasName)
	{
		return PyUnicode_FromString("");
	}
	return PyUnicode_FromString(ScCore->primaryMainWindow()->doc->documentFileName().toUtf8());
}

PyObject *scribus_savedocas(PyObject* /* self */, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	bool ret = ScCore->primaryMainWindow()->DoFileSave(QString::fromUtf8(Name));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to save document.","python error").toLocal8Bit().constData());
		return nullptr;
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
	// z means string, but None becomes a nullptr value. QString()
	// will correctly handle nullptr.
	if (!PyArg_ParseTuple(args, "zzz", &Author, &Title, &Desc))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	DocumentInformation& docInfo = ScCore->primaryMainWindow()->doc->documentInfo();
	docInfo.setAuthor(QString::fromUtf8(Author));
	docInfo.setTitle(QString::fromUtf8(Title));
	docInfo.setComments(QString::fromUtf8(Desc));
	ScCore->primaryMainWindow()->slotDocCh();

	Py_RETURN_NONE;
}

PyObject *scribus_getinfo(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	if (! ScCore->primaryMainWindow()->doc->hasName)
	{
		return PyUnicode_FromString("");
	}

	DocumentInformation& docInfo = ScCore->primaryMainWindow()->doc->documentInfo();
	return Py_BuildValue("(sss)",
				docInfo.author().toUtf8().data(),
				docInfo.title().toUtf8().data(),
				docInfo.comments().toUtf8().data());
}

PyObject *scribus_setunit(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if ((e < UNITMIN) || (e > UNITMAX))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Unit out of range. Use one of the scribus.UNIT_* constants.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->slotChangeUnit(e);

	Py_RETURN_NONE;
}

PyObject *scribus_getunit(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	return PyLong_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->unitIndex()));
}

PyObject *scribus_loadstylesfromfile(PyObject* /* self */, PyObject *args)
{
	char *fileName;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &fileName))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->doc->loadStylesFromFile(QString::fromUtf8(fileName));

	Py_RETURN_NONE;
}

PyObject *scribus_setdoctype(PyObject* /* self */, PyObject* args)
{
	int fp, fsl;
	if (!PyArg_ParseTuple(args, "ii", &fp, &fsl))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;

	if (currentDoc->pagePositioning() == fp)
		currentDoc->setPageSetFirstPage(currentDoc->pagePositioning(), fsl);
	currentView->reformPages();
	currentView->GotoPage(currentDoc->currentPageNumber()); // is this needed?
	currentView->DrawNew();   // is this needed?
	//CB TODO ScCore->primaryMainWindow()->pagePalette->RebuildPage(); // is this needed?
	ScCore->primaryMainWindow()->slotDocCh();

	Py_RETURN_NONE;
}

PyObject *scribus_closemasterpage(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->view->hideMasterPage();

	Py_RETURN_NONE;
}

PyObject *scribus_masterpagenames(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	PyObject* names = PyList_New(currentDoc->MasterPages.count());
	QMap<QString,int>::const_iterator it(currentDoc->MasterNames.constBegin());
	QMap<QString,int>::const_iterator itEnd(currentDoc->MasterNames.constEnd());
	int n = 0;
	for ( ; it != itEnd; ++it )
	{
		PyList_SET_ITEM(names, n++, PyUnicode_FromString(it.key().toUtf8().data()) );
	}
	return names;
}

PyObject *scribus_editmasterpage(PyObject* /* self */, PyObject* args)
{
	char* name = nullptr;
	if (!PyArg_ParseTuple(args, "es", const_cast<char*>("utf-8"), &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString masterPageName(name);
	const QMap<QString,int>& masterNames(ScCore->primaryMainWindow()->doc->MasterNames);
	const QMap<QString,int>::const_iterator it(masterNames.find(masterPageName));
	if ( it == masterNames.constEnd() )
	{
		PyErr_SetString(PyExc_ValueError, "Master page not found");
		return nullptr;
	}
	ScCore->primaryMainWindow()->view->showMasterPage(*it);

	Py_RETURN_NONE;
}

PyObject* scribus_createmasterpage(PyObject* /* self */, PyObject* args)
{
	char* name = nullptr;
	if (!PyArg_ParseTuple(args, "es", const_cast<char*>("utf-8"), &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString masterPageName(name);

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if (currentDoc->MasterNames.contains(masterPageName))
	{
		PyErr_SetString(PyExc_ValueError, "Master page already exists");
		return nullptr;
	}
	currentDoc->addMasterPage(currentDoc->MasterPages.count(), masterPageName);

	Py_RETURN_NONE;
}

PyObject* scribus_deletemasterpage(PyObject* /* self */, PyObject* args)
{
	char* name = nullptr;
	if (!PyArg_ParseTuple(args, "es", const_cast<char*>("utf-8"), &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString masterPageName(name);

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if (!currentDoc->MasterNames.contains(masterPageName))
	{
		PyErr_SetString(PyExc_ValueError, "Master page does not exist");
		return nullptr;
	}
	if (masterPageName == "Normal")
	{
		PyErr_SetString(PyExc_ValueError, "Can not delete the Normal master page");
		return nullptr;
	}
	bool oldMode = currentDoc->masterPageMode();
	currentDoc->setMasterPageMode(true);
	ScCore->primaryMainWindow()->deletePage2(currentDoc->MasterNames[masterPageName]);
	currentDoc->setMasterPageMode(oldMode);

	Py_RETURN_NONE;
}

PyObject *scribus_getmasterpage(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	e--;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if ((e < 0) || (e > static_cast<int>(currentDoc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range: '%1'.","python error").arg(e+1).toLocal8Bit().constData());
		return nullptr;
	}
	return PyUnicode_FromString(currentDoc->DocPages.at(e)->masterPageName().toUtf8());
}

PyObject* scribus_applymasterpage(PyObject* /* self */, PyObject* args)
{
	char* name = nullptr;
	int page = 0;
	if (!PyArg_ParseTuple(args, "esi", const_cast<char*>("utf-8"), &name, &page))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString masterPageName(name);

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if (!currentDoc->MasterNames.contains(masterPageName))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Master page does not exist: '%1'","python error").arg(masterPageName).toLocal8Bit().constData());
		return nullptr;
	}
	if ((page < 1) || (page > static_cast<int>(currentDoc->Pages->count())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range: %1.","python error").arg(page).toLocal8Bit().constData());
		return nullptr;
	}

	if (!currentDoc->applyMasterPage(masterPageName, page-1))
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to apply masterpage '%1' on page: %2","python error").arg(masterPageName).arg(page).toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmddocdocwarnings()
{
	QStringList s;
	s << scribus_applymasterpage__doc__
	  << scribus_closedoc__doc__
	  << scribus_closemasterpage__doc__
	  << scribus_createmasterpage__doc__
	  << scribus_deletemasterpage__doc__
	  << scribus_editmasterpage__doc__ 
	  << scribus_getdocname__doc__
	  << scribus_getinfo__doc__
	  << scribus_getmasterpage__doc__
	  << scribus_getunit__doc__ 
	  << scribus_havedoc__doc__
	  << scribus_loadstylesfromfile__doc__
	  << scribus_masterpagenames__doc__ 
	  << scribus_newdoc__doc__ 
	  << scribus_newdocument__doc__
	  << scribus_opendoc__doc__
	  << scribus_revertdoc__doc__
	  << scribus_savedoc__doc__
	  << scribus_savedocas__doc__
	  << scribus_setbaseline__doc__
	  << scribus_setbleeds__doc__
	  << scribus_setdoctype__doc__ 
	  << scribus_setinfo__doc__
	  << scribus_setmargins__doc__
	  << scribus_setunit__doc__;
}
