/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdcolor.h"
#include "cmdutil.h"
#include "prefsmanager.h"

PyObject *scribus_colornames(PyObject* /* self */)
{
	ColorList edc;
	PyObject *l;
	int cc = 0;
	edc = ScMW->HaveDoc ? ScMW->doc->PageColors : PrefsManager::instance()->colorSet();
	ColorList::Iterator it;
	l = PyList_New(edc.count());
	for (it = edc.begin(); it != edc.end(); ++it)
	{
		PyList_SetItem(l, cc, PyString_FromString(it.key().utf8()));
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a color with an empty name.","python error"));
		return NULL;
	}
	edc = ScMW->HaveDoc ? ScMW->doc->PageColors : PrefsManager::instance()->colorSet();
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error"));
		return NULL;
	}
	edc[col].getCMYK(&c, &m, &y, &k);
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a color with an empty name.","python error"));
		return NULL;
	}
	edc = ScMW->HaveDoc ? ScMW->doc->PageColors : PrefsManager::instance()->colorSet();
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error"));
		return NULL;
	}
	QColor rgb = edc[col].getRGBColor();
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot change a color with an empty name.","python error"));
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	if (ScMW->HaveDoc)
	{
		if (!ScMW->doc->PageColors.contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error"));
			return NULL;
		}
		ScMW->doc->PageColors[col].setColor(c, m, y, k);
	}
	else
	{
		ColorList* colorList=PrefsManager::instance()->colorSetPtr();
		if (!colorList->contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error"));
			return NULL;
		}
		(*colorList)[col].setColor(c, m, y, k);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_newcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "esiiii", "utf-8", &Name, &c, &m, &y, &k))
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create a color with an empty name.","python error"));
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	if (ScMW->HaveDoc)
		{
			if (!ScMW->doc->PageColors.contains(col))
				ScMW->doc->PageColors.insert(col, ScColor(c, m, y, k));
			else
				// FIXME: Given that we have a changeColour function, should we really be
				// silently changing colours in newColour?
				ScMW->doc->PageColors[col].setColor(c, m, y, k);
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
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_delcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Repl = const_cast<char*>("None");
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Name, "utf-8", &Repl))
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot delete a color with an empty name.","python error"));
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	QString rep = QString::fromUtf8(Repl);
	if (ScMW->HaveDoc)
	{
		if (ScMW->doc->PageColors.contains(col) && (ScMW->doc->PageColors.contains(rep) || (rep == "None")))
			{
				ScMW->doc->PageColors.remove(col);
				ReplaceColor(col, rep);
			}
		else
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error"));
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
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error"));
			return NULL;
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_replcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Repl = const_cast<char*>("None");
	//FIXME: this should definitely use keyword arguments
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Name, "utf-8", &Repl))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot replace a color with an empty name.","python error"));
		return NULL;
	}
	QString col = QString::fromUtf8(Name);
	QString rep = QString::fromUtf8(Repl);
	if (ScMW->doc->PageColors.contains(col) && (ScMW->doc->PageColors.contains(rep) || (rep == "None")))
		ReplaceColor(col, rep);
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}
