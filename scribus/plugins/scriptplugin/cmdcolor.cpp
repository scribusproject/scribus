#include "cmdcolor.h"
#include "cmdutil.h"

PyObject *scribus_colornames(PyObject */*self*/)
{
	CListe edc;
	PyObject *l;
	int cc = 0;
	edc = Carrier->HaveDoc ? Carrier->doc->PageColors : Carrier->Prefs.DColors;
	CListe::Iterator it;
	l = PyList_New(edc.count());
	for (it = edc.begin(); it != edc.end(); ++it)
	{
		PyList_SetItem(l, cc, PyString_FromString(it.key().utf8()));
		cc++;
	}
	return l;
}

PyObject *scribus_getcolor(PyObject */*self*/, PyObject* args)
{
	CListe edc;
	char *Name = const_cast<char*>("");
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a color with an empty name.","python error"));
		return NULL;
	}
	edc = Carrier->HaveDoc ? Carrier->doc->PageColors : Carrier->Prefs.DColors;
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error"));
		return NULL;
	}
	edc[col].getCMYK(&c, &m, &y, &k);
	return Py_BuildValue("(iiii)", static_cast<long>(c), static_cast<long>(m), static_cast<long>(y), static_cast<long>(k));
}

PyObject *scribus_getcolorasrgb(PyObject */*self*/, PyObject* args)
{
	CListe edc;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a colour with an empty name.","python error"));
		return NULL;
	}
	edc = Carrier->HaveDoc ? Carrier->doc->PageColors : Carrier->Prefs.DColors;
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Colour not found","python error"));
		return NULL;
	}
	QColor rgb = edc[col].getRGBColor();
	return Py_BuildValue("(iii)", static_cast<long>(rgb.red()), static_cast<long>(rgb.green()), static_cast<long>(rgb.blue()));
}

PyObject *scribus_setcolor(PyObject */*self*/, PyObject* args)
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
	if (Carrier->HaveDoc)
	{
		if (!Carrier->doc->PageColors.contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error"));
			return NULL;
		}
		Carrier->doc->PageColors[col].setColor(c, m, y, k);
	}
	else
	{
		if (!Carrier->Prefs.DColors.contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error"));
			return NULL;
		}
		Carrier->Prefs.DColors[col].setColor(c, m, y, k);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_newcolor(PyObject */*self*/, PyObject* args)
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
	if (Carrier->HaveDoc)
		{
			if (!Carrier->doc->PageColors.contains(col))
				Carrier->doc->PageColors.insert(col, CMYKColor(c, m, y, k));
			else
				// FIXME: Given that we have a changeColor function, should we really be
				// silently changing colors in newColor?
				Carrier->doc->PageColors[col].setColor(c, m, y, k);
		}
	else
		{
			if (!Carrier->Prefs.DColors.contains(col))
				Carrier->Prefs.DColors.insert(col, CMYKColor(c, m, y, k));
			else
				// FIXME: Given that we have a changeColor function, should we really be
				// silently changing colors in newColor?
				Carrier->Prefs.DColors[col].setColor(c, m, y, k);
		}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_delcolor(PyObject */*self*/, PyObject* args)
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
	if (Carrier->HaveDoc)
	{
		if (Carrier->doc->PageColors.contains(col) && (Carrier->doc->PageColors.contains(rep) || (rep == "None")))
			{
				Carrier->doc->PageColors.remove(col);
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
		if (Carrier->Prefs.DColors.contains(col))
			Carrier->Prefs.DColors.remove(col);
		else
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error"));
			return NULL;
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_replcolor(PyObject */*self*/, PyObject* args)
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
	if (Carrier->doc->PageColors.contains(col) && (Carrier->doc->PageColors.contains(rep) || (rep == "None")))
		ReplaceColor(col, rep);
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}
