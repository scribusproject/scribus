/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdpage.h"
#include "cmdutil.h"
#include "page.h"
#include "scribuscore.h"
#include "commonstrings.h"


PyObject *scribus_actualpage(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->currentPageNumber() + 1));
}

PyObject *scribus_redraw(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScCore->primaryMainWindow()->view->DrawNew();
 //	Py_INCREF(Py_None);
 //	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_pageposition(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	e--;
	if ((e < 0) || (e > static_cast<int>(ScCore->primaryMainWindow()->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->locationOfPage(e)));
}

PyObject *scribus_savepageeps(PyObject* /* self */, PyObject* args)
{
	char *Name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	QString epsError;
	bool ret = ScCore->primaryMainWindow()->DoSaveAsEps(QString::fromUtf8(Name), epsError);
	if (!ret)
	{
		QString message = QObject::tr("Failed to save EPS.","python error");
		if (!epsError.isEmpty())
			message += QString("\n%1").arg(epsError);
		PyErr_SetString(ScribusException, message.toLocal8Bit().constData());
		return NULL;
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
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	e--;
	if ((e < 0) || (e > static_cast<int>(ScCore->primaryMainWindow()->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
		return NULL;
	}
	ScCore->primaryMainWindow()->DeletePage2(e);
// 	Py_INCREF(Py_None);
// 	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_gotopage(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	e--;
	if ((e < 0) || (e > static_cast<int>(ScCore->primaryMainWindow()->doc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
		return NULL;
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
		return NULL;
	if(!checkHaveDocument())
		return NULL;

	int loc = (e > -1) ? e : ScCore->primaryMainWindow()->doc->Pages->count();
	if (ScCore->primaryMainWindow()->doc->pageSets[ScCore->primaryMainWindow()->doc->currentPageLayout].Columns != 1)
	{
		switch (ScCore->primaryMainWindow()->doc->locationOfPage(loc))
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

	if (!ScCore->primaryMainWindow()->doc->MasterNames.contains(qName))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Given master page name does not match any existing.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (e < 0)
		ScCore->primaryMainWindow()->slotNewPageP(loc, qName);
	else
	{
		e--;
		if ((e < 0) || (e > static_cast<int>(loc - 1)))
		{
			PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error").toLocal8Bit().constData());
			return NULL;
		}
		ScCore->primaryMainWindow()->slotNewPageP(e, qName);
	}
// 	Py_INCREF(Py_None);
 //	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_pagecount(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->Pages->count()));
}

PyObject *scribus_pagedimension(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	PyObject *t;
	t = Py_BuildValue(
			"(dd)",
			PointToValue(ScCore->primaryMainWindow()->doc->pageWidth), // it's just view scale... * ScCore->primaryMainWindow()->doc->Scale),
			PointToValue(ScCore->primaryMainWindow()->doc->pageHeight)  // * ScCore->primaryMainWindow()->doc->Scale)
		);
	return t;
}

PyObject *scribus_getpageitems(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	if (ScCore->primaryMainWindow()->doc->Items->count() == 0)
		return Py_BuildValue((char*)"[]");
	uint counter = 0;
	int pageNr = ScCore->primaryMainWindow()->doc->currentPageNumber();
	for (int lam2 = 0; lam2 < ScCore->primaryMainWindow()->doc->Items->count(); ++lam2)
	{
		if (pageNr == ScCore->primaryMainWindow()->doc->Items->at(lam2)->OwnPage)
			counter++;
	}
	PyObject *l = PyList_New(counter);
	PyObject *row;
	counter = 0;
	for (int i = 0; i<ScCore->primaryMainWindow()->doc->Items->count(); ++i)
	{
		if (pageNr == ScCore->primaryMainWindow()->doc->Items->at(i)->OwnPage)
		{
			row = Py_BuildValue((char*)"(sii)",
			                    ScCore->primaryMainWindow()->doc->Items->at(i)->itemName().toAscii().constData(),
			                    ScCore->primaryMainWindow()->doc->Items->at(i)->itemType(),
			                    ScCore->primaryMainWindow()->doc->Items->at(i)->ItemNr
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
	Guides g = ScCore->primaryMainWindow()->doc->currentPage()->guides.horizontals(GuideManagerCore::Standard);
	int n = g.count();//ScCore->primaryMainWindow()->doc->currentPage->YGuides.count();
	if (n == 0)
		return Py_BuildValue((char*)"[]");
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
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int i, n;
	n = PyList_Size(l);
	double guide;
	ScCore->primaryMainWindow()->doc->currentPage()->guides.clearHorizontals(GuideManagerCore::Standard);
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains non-numeric values: must be list of float values.","python error").toLocal8Bit().constData());
			return NULL;
		}
		ScCore->primaryMainWindow()->doc->currentPage()->guides.addHorizontal(ValueToPoint(guide), GuideManagerCore::Standard);
	}
 	Py_INCREF(Py_None);
 	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_getVguides(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	Guides g = ScCore->primaryMainWindow()->doc->currentPage()->guides.verticals(GuideManagerCore::Standard);
	int n = g.count();//ScCore->primaryMainWindow()->doc->currentPage->XGuides.count();
	if (n == 0)
		return Py_BuildValue((char*)"[]");
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
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (!PyList_Check(l))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int i, n;
	n = PyList_Size(l);
	double guide;
	ScCore->primaryMainWindow()->doc->currentPage()->guides.clearVerticals(GuideManagerCore::Standard);
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyList_GetItem(l, i), "d", &guide))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains no-numeric values: must be list of float values.","python error").toLocal8Bit().constData());
			return NULL;
		}
		ScCore->primaryMainWindow()->doc->currentPage()->guides.addVertical(ValueToPoint(guide), GuideManagerCore::Standard);
	}
// 	Py_INCREF(Py_None);
// 	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_getpagemargins(PyObject* /* self */)
{
	PyObject *margins = NULL;
	if(!checkHaveDocument())
		return NULL;
	margins = Py_BuildValue("ffff", PointToValue(ScCore->primaryMainWindow()->doc->pageMargins.Top),
									PointToValue(ScCore->primaryMainWindow()->doc->pageMargins.Left),
									PointToValue(ScCore->primaryMainWindow()->doc->pageMargins.Right),
									PointToValue(ScCore->primaryMainWindow()->doc->pageMargins.Bottom));
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
void import_addpages(int total, int pos) {
	for (int i=0; i<total; i++) {
		int locreal = pos + i;
		int loc = pos + i + 1;

		if (loc > ScCore->primaryMainWindow()->doc->Pages->count()) {
			loc = ScCore->primaryMainWindow()->doc->Pages->count();
		}
		QString qName(CommonStrings::trMasterPageNormal);

		if (ScCore->primaryMainWindow()->doc->pageSets[ScCore->primaryMainWindow()->doc->currentPageLayout].Columns != 1) {
			ScCore->primaryMainWindow()->doc->locationOfPage(loc);
			switch (ScCore->primaryMainWindow()->doc->locationOfPage(loc))
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
	char *doc = NULL;
	PyObject *pages = NULL;
	int createPageI = 1;
	int importWhere = 2;
	int importWherePage = 0;

	if (!PyArg_ParseTuple(args, "sO|iii", &doc, &pages, &createPageI, &importWhere, &importWherePage))
		return NULL;
	if(!checkHaveDocument())
		return NULL;

	if (!PyTuple_Check(pages))
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("second argument is not tuple: must be tuple of int values.","python error").toLocal8Bit().constData());
		return NULL;
	}

	Py_INCREF(pages);
	std::vector<int> pageNs;
	int i, n, p;
	n = PyTuple_Size(pages);
	for (i=0; i<n; i++)
	{
		if (!PyArg_Parse(PyTuple_GetItem(pages, i), "i", &p))
		{
			PyErr_SetString(PyExc_TypeError, QObject::tr("second argument contains non-numeric values: must be list of int values.","python error").toLocal8Bit().constData());
			Py_DECREF(pages);
			return NULL;
		}
		pageNs.push_back(p);
	}
	Py_DECREF(pages);
 
	QString fromDoc = QString(doc);
	bool createPage = (createPageI != 0);

	int startPage=0, nrToImport=pageNs.size();
	bool doIt = true;

	if (ScCore->primaryMainWindow()->doc->masterPageMode())
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
			startPage = ScCore->primaryMainWindow()->doc->DocPages.count() + 1;

		import_addpages(nrToImport, startPage);
	}
	else
	{
		startPage = ScCore->primaryMainWindow()->doc->currentPage()->pageNr() + 1;
		if (nrToImport > (ScCore->primaryMainWindow()->doc->DocPages.count() - ScCore->primaryMainWindow()->doc->currentPage()->pageNr()))
		{
			int tmp=nrToImport - (ScCore->primaryMainWindow()->doc->DocPages.count() - ScCore->primaryMainWindow()->doc->currentPage()->pageNr());
			import_addpages(tmp, ScCore->primaryMainWindow()->doc->DocPages.count());
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
    s << scribus_newpage__doc__        << scribus_pageposition__doc__
	  << scribus_actualpage__doc__     << scribus_redraw__doc__
	  << scribus_savepageeps__doc__    << scribus_deletepage__doc__
	  << scribus_gotopage__doc__       << scribus_pagecount__doc__
	  << scribus_getHguides__doc__     << scribus_setHguides__doc__
	  << scribus_getVguides__doc__     << scribus_setVguides__doc__
	  << scribus_pagedimension__doc__  << scribus_getpageitems__doc__
	  << scribus_getpagemargins__doc__ << scribus_importpage__doc__;
}
