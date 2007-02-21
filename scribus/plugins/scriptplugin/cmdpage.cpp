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
#if ((PY_MAJOR_VERSION == 2) && (PY_MINOR_VERSION < 4))
 	Py_INCREF(Py_None);
 	return Py_None;
#else
	Py_RETURN_NONE;
#endif
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
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
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
	bool ret = ScCore->primaryMainWindow()->DoSaveAsEps(QString::fromUtf8(Name));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to save EPS.","python error"));
		return NULL;
	}
#if ((PY_MAJOR_VERSION == 2) && (PY_MINOR_VERSION < 4))
 	Py_INCREF(Py_True); // return True not None for backward compat
 	return Py_True;
#else
	Py_RETURN_TRUE;
#endif
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
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
		return NULL;
	}
	ScCore->primaryMainWindow()->DeletePage2(e);
#if ((PY_MAJOR_VERSION == 2) && (PY_MINOR_VERSION < 4))
 	Py_INCREF(Py_None);
 	return Py_None;
#else
	Py_RETURN_NONE;
#endif
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
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
		return NULL;
	}
	ScCore->primaryMainWindow()->view->GotoPage(e);
#if ((PY_MAJOR_VERSION == 2) && (PY_MINOR_VERSION < 4))
 	Py_INCREF(Py_None);
 	return Py_None;
#else
	Py_RETURN_NONE;
#endif
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
		PyErr_SetString(PyExc_IndexError, QObject::tr("Given master page name does not match any existing.","python error"));
		return NULL;
	}
	if (e < 0)
		ScCore->primaryMainWindow()->slotNewPageP(loc, qName);
	else
	{
		e--;
		if ((e < 0) || (e > static_cast<int>(loc - 1)))
		{
			PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range.","python error"));
			return NULL;
		}
		ScCore->primaryMainWindow()->slotNewPageP(e, qName);
	}
#if ((PY_MAJOR_VERSION == 2) && (PY_MINOR_VERSION < 4))
 	Py_INCREF(Py_None);
 	return Py_None;
#else
	Py_RETURN_NONE;
#endif
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
	for (uint lam2 = 0; lam2 < ScCore->primaryMainWindow()->doc->Items->count(); ++lam2)
	{
		if (pageNr == ScCore->primaryMainWindow()->doc->Items->at(lam2)->OwnPage)
			counter++;
	}
	PyObject *l = PyList_New(counter);
	PyObject *row;
	counter = 0;
	for (uint i = 0; i<ScCore->primaryMainWindow()->doc->Items->count(); ++i)
	{
		if (pageNr == ScCore->primaryMainWindow()->doc->Items->at(i)->OwnPage)
		{
			row = Py_BuildValue((char*)"(sii)",
			                    ScCore->primaryMainWindow()->doc->Items->at(i)->itemName().ascii(),
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
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values.","python error"));
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
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains non-numeric values: must be list of float values.","python error"));
			return NULL;
		}
		ScCore->primaryMainWindow()->doc->currentPage()->guides.addHorizontal(ValueToPoint(guide), GuideManagerCore::Standard);
	}
#if ((PY_MAJOR_VERSION == 2) && (PY_MINOR_VERSION < 4))
 	Py_INCREF(Py_None);
 	return Py_None;
#else
	Py_RETURN_NONE;
#endif
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
		PyErr_SetString(PyExc_TypeError, QObject::tr("argument is not list: must be list of float values.","python error"));
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
			PyErr_SetString(PyExc_TypeError, QObject::tr("argument contains no-numeric values: must be list of float values.","python error"));
			return NULL;
		}
		ScCore->primaryMainWindow()->doc->currentPage()->guides.addVertical(ValueToPoint(guide), GuideManagerCore::Standard);
	}
#if ((PY_MAJOR_VERSION == 2) && (PY_MINOR_VERSION < 4))
 	Py_INCREF(Py_None);
 	return Py_None;
#else
	Py_RETURN_NONE;
#endif
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

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdpagedocwarnings()
{
    QStringList s;
    s << scribus_newpage__doc__ << scribus_pageposition__doc__ << scribus_actualpage__doc__ << scribus_redraw__doc__ << scribus_savepageeps__doc__ << scribus_deletepage__doc__ << scribus_gotopage__doc__ << scribus_pagecount__doc__ << scribus_getHguides__doc__ <<scribus_setHguides__doc__ <<scribus_getVguides__doc__ <<scribus_setVguides__doc__ <<scribus_pagedimension__doc__ <<scribus_getpageitems__doc__ <<scribus_getpagemargins__doc__;
}
