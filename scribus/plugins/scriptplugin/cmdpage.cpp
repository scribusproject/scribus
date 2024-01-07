/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdpage.h"
#include "cmdutil.h"
#include "commonstrings.h"
#include "scpage.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"

#include <QApplication>

PyObject *scribus_currentpage(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	return PyLong_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->currentPageNumber() + 1));
}


PyObject *scribus_currentpagenumberforsection(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	int i = ScCore->primaryMainWindow()->doc->currentPageNumber();
	return PyUnicode_FromString(ScCore->primaryMainWindow()->doc->getSectionPageNumberForPageIndex(i).toUtf8());
}

PyObject *scribus_redraw(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->view->DrawNew();
	qApp->processEvents();

	Py_RETURN_NONE;
}

PyObject *scribus_getpagetype(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	e--;
	if ((e < 0) || (e > static_cast<int>(ScCore->primaryMainWindow()->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->locationOfPage(e)));
}

PyObject *scribus_savepageeps(PyObject* /* self */, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	QString epsError;
	bool ret = ScCore->primaryMainWindow()->DoSaveAsEps(QString::fromUtf8(Name), epsError);
	if (!ret)
	{
		QString message = QObject::tr("Failed to save EPS.","python error");
		if (!epsError.isEmpty())
			message += QString("\n%1").arg(epsError);
		PyErr_SetString(ScribusException, message.toLocal8Bit().constData());
		return nullptr;
	}
// 	Py_INCREF(Py_True); // return True not None for backward compat
// 	return Py_True;
//	Py_RETURN_TRUE;
	return PyBool_FromLong(static_cast<long>(true));
}

PyObject *scribus_deletepage(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	e--;
	if ((e < 0) || (e > static_cast<int>(ScCore->primaryMainWindow()->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->deletePage2(e);
// 	Py_INCREF(Py_None);
// 	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_gotopage(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	e--;
	if ((e < 0) || (e > static_cast<int>(ScCore->primaryMainWindow()->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->view->GotoPage(e);
// 	Py_INCREF(Py_None);
// 	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_newpage(PyObject* /* self */, PyObject* args)
{
	int e;
	char *name = const_cast<char*>("");
	QString qName(CommonStrings::trMasterPageNormal);
	if (!PyArg_ParseTuple(args, "i|es", &e, "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusMainWindow* mainWin = ScCore->primaryMainWindow();
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	int loc = (e > -1) ? e : currentDoc->Pages->count();
	if (currentDoc->pageSets()[currentDoc->pagePositioning()].Columns != 1)
	{
		switch (currentDoc->locationOfPage(loc))
		{
			case LeftPage:
				qName = CommonStrings::trMasterPageNormalLeft;
				break;
			case RightPage:
				qName = CommonStrings::trMasterPageNormalRight;
				break;
			case MiddlePage:
				qName = CommonStrings::trMasterPageNormalMiddle;
				break;
		}
	}
	if (QString(name).length() != 0)
		qName = QString::fromUtf8(name);

	if (!currentDoc->MasterNames.contains(qName))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Given master page name does not match any existing.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (e < 0)
		mainWin->slotNewPageP(loc, qName);
	else
	{
		e--;
		if ((e < 0) || (e > static_cast<int>(loc - 1)))
		{
			PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		mainWin->slotNewPageP(e, qName);
	}
// 	Py_INCREF(Py_None);
 //	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_pagecount(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	return PyLong_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->Pages->count()));
}

PyObject *scribus_getpagesize(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	PyObject *t;
	t = Py_BuildValue(
			"(dd)",
			PointToValue(currentDoc->pageWidth()), // it's just view scale... * ScCore->primaryMainWindow()->doc->Scale),
			PointToValue(currentDoc->pageHeight())  // * ScCore->primaryMainWindow()->doc->Scale)
		);
	return t;
}

PyObject *scribus_getpagensize(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	e--;
	if ((e < 0) || (e > static_cast<int>(currentDoc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PyObject *t;
	t = Py_BuildValue(
			"(dd)",
			PointToValue(currentDoc->Pages->at(e)->width()),
			PointToValue(currentDoc->Pages->at(e)->height())
		);
	return t;
}

PyObject *scribus_getpagenmargins(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	e--;
	if ((e < 0) || (e > static_cast<int>(currentDoc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PyObject *margins = nullptr;
	margins = Py_BuildValue("ffff", PointToValue(currentDoc->Pages->at(e)->Margins.top()),
									PointToValue(currentDoc->Pages->at(e)->Margins.left()),
									PointToValue(currentDoc->Pages->at(e)->Margins.right()),
									PointToValue(currentDoc->Pages->at(e)->Margins.bottom()));
	return margins;
}

PyObject *scribus_getpageitems(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	if (currentDoc->Items->count() == 0)
		return Py_BuildValue("[]");
	uint counter = 0;
	int pageNr = currentDoc->currentPageNumber();
	for (int lam2 = 0; lam2 < currentDoc->Items->count(); ++lam2)
	{
		if (pageNr == currentDoc->Items->at(lam2)->OwnPage)
			counter++;
	}
	PyObject *l = PyList_New(counter);
	PyObject *row;
	counter = 0;
	for (int i = 0; i<currentDoc->Items->count(); ++i)
	{
		if (pageNr == currentDoc->Items->at(i)->OwnPage)
		{
			row = Py_BuildValue("(sii)",
			                    currentDoc->Items->at(i)->itemName().toUtf8().constData(),
			                    currentDoc->Items->at(i)->itemType(),
								currentDoc->Items->at(i)->uniqueNr
			                   );
			PyList_SetItem(l, counter, row);
			counter++;
		}
	} // for
	return l;
}

PyObject *scribus_getHguides(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	Guides g = ScCore->primaryMainWindow()->doc->currentPage()->guides.horizontals(GuideManagerCore::Standard);
	int n = g.count();//ScCore->primaryMainWindow()->doc->currentPage->YGuides.count();
	if (n == 0)
		return Py_BuildValue("[]");
	int i;
	double tmp;
	PyObject *l, *guide;
	l = PyList_New(0);
	for (i=0; i<n; i++)
	{
		tmp = g[i];
		guide = Py_BuildValue("d", PointToValue(tmp));
		PyList_Append(l, guide);
	}
	return l;
}

PyObject *scribus_setHguides(PyObject* /* self */, PyObject* args)
{
	PyObject *l;
	if (!PyArg_ParseTuple(args, "O", &l))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	int i, n;
	n = PyList_Size(l);
	double guide;
	currentDoc->currentPage()->guides.clearHorizontals(GuideManagerCore::Standard);
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains non-numeric values: must be list of float values.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		currentDoc->currentPage()->guides.addHorizontal(ValueToPoint(guide), GuideManagerCore::Standard);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_getVguides(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	Guides g = ScCore->primaryMainWindow()->doc->currentPage()->guides.verticals(GuideManagerCore::Standard);
	int n = g.count();//ScCore->primaryMainWindow()->doc->currentPage->XGuides.count();
	if (n == 0)
		return Py_BuildValue("[]");
	int i;
	double tmp;
	PyObject *l, *guide;
	l = PyList_New(0);
	for (i=0; i<n; i++)
	{
		tmp = g[i];
		guide = Py_BuildValue("d", PointToValue(tmp));
		PyList_Append(l, guide);
	}
	return l;
}

PyObject *scribus_setVguides(PyObject* /* self */, PyObject* args)
{
	PyObject *l;
	if (!PyArg_ParseTuple(args, "O", &l))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	int i, n;
	n = PyList_Size(l);
	double guide;
	currentDoc->currentPage()->guides.clearVerticals(GuideManagerCore::Standard);
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains no-numeric values: must be list of float values.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		currentDoc->currentPage()->guides.addVertical(ValueToPoint(guide), GuideManagerCore::Standard);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_getColumnGuides(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	auto guides = PyDict_New();

	auto page = ScCore->primaryMainWindow()->doc->currentPage();

	{
		const char keyString[] = "number";
		PyObject *key = PyUnicode_FromString(keyString);
		PyDict_SetItem(guides, key, PyLong_FromLong((long) page->guides.verticalAutoCount()));
	}
	{
		const char keyString[] = "gap";
		PyObject *key = PyUnicode_FromString(keyString);
		PyDict_SetItem(guides, key, Py_BuildValue("d", PointToValue(page->guides.verticalAutoGap())));
	}
	{
		const char keyString[] = "refer_to";
		PyObject *key = PyUnicode_FromString(keyString);
		PyDict_SetItem(guides, key, PyLong_FromLong((long) page->guides.verticalAutoRefer()));
	}
	{
		auto rowsList = PyList_New(0);
		for (auto column: page->guides.getAutoVerticals(page)) // QList<double> Guides
		{
			PyList_Append(rowsList, Py_BuildValue("d", PointToValue(column)));
		}
		const char keyString[] = "guides";
		PyObject *key = PyUnicode_FromString(keyString);
		PyDict_SetItem(guides, key, rowsList);
	}

	return guides;
}

PyObject *scribus_getRowGuides(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	auto guides = PyDict_New();

	auto page = ScCore->primaryMainWindow()->doc->currentPage();

	{
		const char keyString[] = "number";
		PyObject *key = PyUnicode_FromString(keyString);
		PyDict_SetItem(guides, key, PyLong_FromLong((long) page->guides.horizontalAutoCount()));
	}
	{
		const char keyString[] = "gap";
		PyObject *key = PyUnicode_FromString(keyString);
		PyDict_SetItem(guides, key, Py_BuildValue("d", PointToValue(page->guides.horizontalAutoGap())));
	}
	{
		const char keyString[] = "refer_to";
		PyObject *key = PyUnicode_FromString(keyString);
		PyDict_SetItem(guides, key, PyLong_FromLong((long) page->guides.horizontalAutoRefer()));
	}
	{
		auto rowsList = PyList_New(0);
		for (auto row: page->guides.getAutoHorizontals(page)) // QList<double> Guides
		{
			PyList_Append(rowsList, Py_BuildValue("d", PointToValue(row)));
		}
		const char keyString[] = "guides";
		PyObject *key = PyUnicode_FromString(keyString);
		PyDict_SetItem(guides, key, rowsList);
	}
	return guides;
}

PyObject *scribus_setColumnGuides(PyObject* /* self */, PyObject* args, PyObject* kw)
{
	if (!checkHaveDocument())
		return nullptr;

	int number;
	double gap = 0.0;
	int referTo = 0;
	char* kwargs[] = {const_cast<char*>("number"), const_cast<char*>("gap"), const_cast<char*>("refer_to"), nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "i|di", kwargs, &number, &gap, &referTo))
		return nullptr;

	auto doc = ScCore->primaryMainWindow()->doc;

	if (referTo < 0 || referTo > 2)
	{
		QString message = QObject::tr("refer_to=0|1|2.","python error");
		PyErr_SetString(ScribusException, message.toLocal8Bit().constData());
		return nullptr;
	}
	else if (referTo == 2)
	{
		// refertTo a selection is only active, when there is a selection in the document
		if (doc->m_Selection->isEmpty())
		{
			QString message = QObject::tr("setColumnGuides() with refer_to=2 needs a selection.","python error");
			PyErr_SetString(ScribusException, message.toLocal8Bit().constData());
			return nullptr;
		}
		doc->currentPage()->guides.resetSelectionForPage(doc->currentPage());
	}


	// If there are no guides, reset the values for the gap and for the reference
	if (number == 0)
	{
		doc->currentPage()->guides.setVerticalAutoGap(0);
		doc->currentPage()->guides.setVerticalAutoRefer(0);
		doc->currentPage()->guides.setVerticalAutoCount(0);
		Py_RETURN_NONE;
	}

	doc->currentPage()->guides.setVerticalAutoGap(ValueToPoint(gap));
	doc->currentPage()->guides.setVerticalAutoRefer(referTo);
	doc->currentPage()->guides.setVerticalAutoCount(number);

	Py_RETURN_NONE;
}

PyObject *scribus_setRowGuides(PyObject* /* self */, PyObject* args, PyObject* kw)
{
	if (!checkHaveDocument())
		return nullptr;

	int number;
	double gap = 0.0;
	int referTo = 0;
	char* kwargs[] = {const_cast<char*>("number"), const_cast<char*>("gap"), const_cast<char*>("refer_to"), nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "i|di", kwargs, &number, &gap, &referTo))
		return nullptr;

	auto doc = ScCore->primaryMainWindow()->doc;

	if (referTo < 0 || referTo > 2)
	{
		QString message = QObject::tr("refer_to=0|1|2.","python error");
		PyErr_SetString(ScribusException, message.toLocal8Bit().constData());
		return nullptr;
	}
	else if (referTo == 2)
	{
		if (doc->m_Selection->isEmpty())
		{
			// refertTo a selection is only active, when there is a selection in the document
			QString message = QObject::tr("setRowGuides() with refer_to=2 needs a selection.","python error");
			PyErr_SetString(ScribusException, message.toLocal8Bit().constData());
			return nullptr;
		}
		doc->currentPage()->guides.resetSelectionForPage(doc->currentPage());
	}

	doc->currentPage()->guides.setHorizontalAutoCount(number);

	// If there are no guides, reset the values for the gap and for the reference
	if (number == 0)
	{
		doc->currentPage()->guides.setHorizontalAutoGap(0);
		doc->currentPage()->guides.setHorizontalAutoRefer(0);
		Py_RETURN_NONE;
	}

	doc->currentPage()->guides.setHorizontalAutoGap(ValueToPoint(gap));
	doc->currentPage()->guides.setHorizontalAutoRefer(referTo);

	Py_RETURN_NONE;
}

PyObject *scribus_getpagemargins(PyObject* /* self */)
{
	PyObject *margins = nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	margins = Py_BuildValue("ffff", PointToValue(currentDoc->margins()->top()),
									PointToValue(currentDoc->margins()->left()),
									PointToValue(currentDoc->margins()->right()),
									PointToValue(currentDoc->margins()->bottom()));
	return margins;
}

/*!
 \fn import_addpages(int total, int pos)
 \author Alessandro Pira <alex@alessandropira.org>
 \date 11-11-2007
 \param total number of pages to add, pos: position in the document
 \param pos position of the imported pages
 \retval void
 */
// This function is used by scribus_importpage() to add new pages
void import_addpages(int total, int pos)
{
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	for (int i=0; i<total; i++)
	{
		int locreal = pos + i;
		int loc = pos + i + 1;

		if (loc > currentDoc->Pages->count())
			loc = currentDoc->Pages->count();

		QString qName(CommonStrings::trMasterPageNormal);

		if (currentDoc->pageSets()[currentDoc->pagePositioning()].Columns != 1) {
			currentDoc->locationOfPage(loc);
			switch (currentDoc->locationOfPage(loc))
			{
				case LeftPage:
					qName = CommonStrings::trMasterPageNormalLeft;
					break;
				case RightPage:
					qName = CommonStrings::trMasterPageNormalRight;
					break;
				case MiddlePage:
					qName = CommonStrings::trMasterPageNormalMiddle;
					break;
			}
		}
		ScCore->primaryMainWindow()->slotNewPageP(locreal, qName);
	}
}

/*!
 \fn scribus_importpage(PyObject*,  PyObject* args)
 \author Alessandro Pira <alex@alessandropira.org>
 \date 11-11-2007
 \param PyObject unused reference
 \param args Python function args ("fromDoc", (pageList), [create, imortwhere, importwherePage])
 \retval Py_RETURN_NONE if ok, null if error
 */
PyObject *scribus_importpage(PyObject* /* self */, PyObject* args)
{
	char *doc = nullptr;
	PyObject *pages = nullptr;
	int createPageI = 1;
	int importWhere = 2;
	int importWherePage = 0;

	if (!PyArg_ParseTuple(args, "sO|iii", &doc, &pages, &createPageI, &importWhere, &importWherePage))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	if (!PyTuple_Check(pages))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("second argument is not tuple: must be tuple of integer values.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	Py_INCREF(pages);
	std::vector<int> pageNs;
	int i, n, p;
	n = PyTuple_Size(pages);
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyTuple_GetItem(pages, i), "i", &p))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("second argument contains non-numeric values: must be list of integer values.","python error").toLocal8Bit().constData());
			Py_DECREF(pages);
			return nullptr;
		}
		pageNs.push_back(p);
	}
	Py_DECREF(pages);
 
	QString fromDoc = QString(doc);
	bool createPage = (createPageI != 0);

	int startPage = 0;
	int nrToImport = pageNs.size();
	bool doIt = true;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if (currentDoc->masterPageMode())
	{
		if (nrToImport > 1)
			ScCore->primaryMainWindow()->loadPage(fromDoc, pageNs[0] - 1, false);
		doIt = false;
	}
	else if (createPage)
	{
		if (importWhere == 0) //Before page
			startPage = importWherePage;
		else if (importWhere == 1) //After page
			startPage = importWherePage + 1;
		else //at end
			startPage = currentDoc->DocPages.count();// + 1;

		import_addpages(nrToImport, startPage);
	}
	else
	{
		startPage = currentDoc->currentPage()->pageNr() + 1;
		if (nrToImport > (currentDoc->DocPages.count() - currentDoc->currentPage()->pageNr()))
		{
			int tmp = nrToImport - (currentDoc->DocPages.count() - currentDoc->currentPage()->pageNr());
			import_addpages(tmp, currentDoc->DocPages.count());
		}
	}

	if (doIt)
	{
		if (nrToImport > 0)
		{
			int counter = startPage + 1;
			for (int i = 0; i < nrToImport; ++i)
			{
				ScCore->primaryMainWindow()->view->GotoPa(counter);
				ScCore->primaryMainWindow()->loadPage(fromDoc, pageNs[i] - 1, false);
				counter++;
			}
		}
	}

	Py_RETURN_NONE;
}


/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdpagedocwarnings()
{
	QStringList s;
	s << scribus_currentpage__doc__
	  << scribus_deletepage__doc__
	  << scribus_getColumnGuides__doc__
	  << scribus_getHguides__doc__
	  << scribus_getpageitems__doc__
	  << scribus_getpagemargins__doc__
	  << scribus_getpagenmargins__doc__ 
	  << scribus_getpagensize__doc__ 
	  << scribus_getpagesize__doc__
	  << scribus_getpagetype__doc__
	  << scribus_getRowGuides__doc__
	  << scribus_getVguides__doc__
	  << scribus_gotopage__doc__
	  << scribus_importpage__doc__
	  << scribus_newpage__doc__
	  << scribus_pagecount__doc__
	  << scribus_redraw__doc__
	  << scribus_savepageeps__doc__           
	  << scribus_setColumnGuides__doc__
	  << scribus_setHguides__doc__
	  << scribus_setRowGuides__doc__
	  << scribus_setVguides__doc__;
}
