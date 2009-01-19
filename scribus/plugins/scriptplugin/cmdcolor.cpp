/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdcolor.h"
#include "cmdutil.h"
#include "prefsmanager.h"
#include "commonstrings.h"
#include "scribuscore.h"
#include "sccolorengine.h"

PyObject *scribus_colornames(PyObject* /* self */)
{
	ColorList edc;
	PyObject *l;
	int cc = 0;
	edc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc->PageColors : PrefsManager::instance()->colorSet();
	ColorList::Iterator it;
	l = PyList_New(edc.count());
	for (it = edc.begin(); it != edc.end(); ++it)
	{
		PyList_SetItem(l, cc, PyString_FromString(it.key().toUtf8()));
		cc++;
	}
	return l;
}

PyObject *scribus_getcolor(PyObject* /* self */, PyObject* args)
{
	ColorList edc;
	char *Name = const_cast<char*>("");
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a color with an empty name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	edc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc->PageColors : PrefsManager::instance()->colorSet();
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc : NULL;
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	CMYKColor cmykValues;
	ScColorEngine::getCMYKValues(edc[col], currentDoc, cmykValues);
	cmykValues.getValues(c, m, y, k);
	return Py_BuildValue("(iiii)", static_cast<long>(c), static_cast<long>(m), static_cast<long>(y), static_cast<long>(k));
}

PyObject *scribus_getcolorasrgb(PyObject* /* self */, PyObject* args)
{
	ColorList edc;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a color with an empty name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	edc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc->PageColors : PrefsManager::instance()->colorSet();
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc : NULL;
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QColor rgb = ScColorEngine::getRGBColor(edc[col], currentDoc);
	return Py_BuildValue("(iii)", static_cast<long>(rgb.red()), static_cast<long>(rgb.green()), static_cast<long>(rgb.blue()));
}

PyObject *scribus_setcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "esiiii", "utf-8", &Name, &c, &m, &y, &k))
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot change a color with an empty name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error").toLocal8Bit().constData());
			return NULL;
		}
		ScCore->primaryMainWindow()->doc->PageColors[col].setColor(c, m, y, k);
	}
	else
	{
		ColorList* colorList=PrefsManager::instance()->colorSetPtr();
		if (!colorList->contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error").toLocal8Bit().constData());
			return NULL;
		}
		(*colorList)[col].setColor(c, m, y, k);
	}
// 	Py_INCREF(Py_None);
// 	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_newcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "esiiii", "utf-8", &Name, &c, &m, &y, &k))
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create a color with an empty name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->HaveDoc)
		{
			if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
				ScCore->primaryMainWindow()->doc->PageColors.insert(col, ScColor(c, m, y, k));
			else
				// FIXME: Given that we have a changeColour function, should we really be
				// silently changing colours in newColour?
				ScCore->primaryMainWindow()->doc->PageColors[col].setColor(c, m, y, k);
		}
	else
		{
			ColorList* colorList=PrefsManager::instance()->colorSetPtr();
			if (!colorList->contains(col))
				colorList->insert(col, ScColor(c, m, y, k));
			else
				// FIXME: Given that we have a changeColour function, should we really be
				// silently changing colours in newColour?
				(*colorList)[col].setColor(c, m, y, k);
		}
 //	Py_INCREF(Py_None);
 //	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_delcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Repl = const_cast<char*>(CommonStrings::None.toLatin1().constData());
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Name, "utf-8", &Repl))
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot delete a color with an empty name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	QString rep = QString::fromUtf8(Repl);
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (ScCore->primaryMainWindow()->doc->PageColors.contains(col) && (ScCore->primaryMainWindow()->doc->PageColors.contains(rep) || (rep == CommonStrings::None)))
			{
				ScCore->primaryMainWindow()->doc->PageColors.remove(col);
				ReplaceColor(col, rep);
			}
		else
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error").toLocal8Bit().constData());
			return NULL;
		}
	}
	else
	{
		ColorList* colorList=PrefsManager::instance()->colorSetPtr();
		if (colorList->contains(col))
			colorList->remove(col);
		else
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error").toLocal8Bit().constData());
			return NULL;
		}
	}
// 	Py_INCREF(Py_None);
// 	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_replcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Repl = const_cast<char*>(CommonStrings::None.toLatin1().constData());
	//FIXME: this should definitely use keyword arguments
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Name, "utf-8", &Repl))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot replace a color with an empty name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	QString rep = QString::fromUtf8(Repl);
	if (ScCore->primaryMainWindow()->doc->PageColors.contains(col) && (ScCore->primaryMainWindow()->doc->PageColors.contains(rep) || (rep == CommonStrings::None)))
		ReplaceColor(col, rep);
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
// 	Py_INCREF(Py_None);
// 	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_isspotcolor(PyObject * /*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Color name cannot be an empty string.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->doc->PageColors.contains(col))
	{
		return PyBool_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->PageColors[col].isSpotColor()));
	}
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
// 	Py_RETURN_NONE;
}

PyObject *scribus_setspotcolor(PyObject * /*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int enable;

	if (!PyArg_ParseTuple(args, "esi", "utf-8", &Name, &enable))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Color name cannot be an empty string.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->doc->PageColors.contains(col))
	{
		ScCore->primaryMainWindow()->doc->PageColors[col].setSpotColor(static_cast<bool>(enable));
	}
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	Py_RETURN_NONE;
}



/*! HACK: this removes "warning: 'blash' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdcolordocswarnings()
{
    QStringList s;
    s << scribus_colornames__doc__ << scribus_getcolor__doc__ << scribus_getcolorasrgb__doc__;
    s << scribus_setcolor__doc__ << scribus_newcolor__doc__ << scribus_delcolor__doc__;
	s << scribus_replcolor__doc__ << scribus_isspotcolor__doc__ << scribus_setspotcolor__doc__;
}
