#include <Python.h>
#include "cmdcolor.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_colornames(PyObject *self, PyObject* args)
{
	CListe edc;
	PyObject *l;
	int cc = 0;
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getColorNames()"));
		return NULL;
	}
	edc = Carrier->HaveDoc ? Carrier->doc->PageColors : Carrier->Prefs.DColors;
	CListe::Iterator it;
	l = PyList_New(edc.count());
	for (it = edc.begin(); it != edc.end(); ++it)
		{
		PyList_SetItem(l, cc, PyString_FromString(it.key()));
		cc++;
		}
	return l;
}

PyObject *scribus_getcolor(PyObject *self, PyObject* args)
{
	CListe edc;
	char *Name = "";
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getColor(name)"));
		return NULL;
	}
	if (Name == "")
		return Py_BuildValue("(iiii)", 0, 0, 0, 0);
	edc = Carrier->HaveDoc ? Carrier->doc->PageColors : Carrier->Prefs.DColors;
	QString col = QString(Name);
	if (!edc.contains(col))
		return Py_BuildValue("(iiii)", 0, 0, 0, 0);
  edc[col].getCMYK(&c, &m, &y, &k);
	return Py_BuildValue("(iiii)", static_cast<long>(c), static_cast<long>(m), static_cast<long>(y), static_cast<long>(k));
}

PyObject *scribus_setcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "siiii", &Name, &c, &m, &y, &k))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("changeColor(colorname, c, m, y, k)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (Name == "")
		return Py_None;
	QString col = QString(Name);
	if (Carrier->HaveDoc)
	{
		if (!Carrier->doc->PageColors.contains(col))
			return Py_None;
		Carrier->doc->PageColors[col].setColor(c, m, y, k);
	}
	else
	{
		if (!Carrier->Prefs.DColors.contains(col))
			return Py_None;
		Carrier->Prefs.DColors[col].setColor(c, m, y, k);
	}
	return Py_None;
}

PyObject *scribus_newcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "siiii", &Name, &c, &m, &y, &k))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("defineColor(colorname, c, m, y, k)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (Name == "")
		return Py_None;
	QString col = QString(Name);
	if (Carrier->HaveDoc)
		{
		if (!Carrier->doc->PageColors.contains(col))
			Carrier->doc->PageColors.insert(col, CMYKColor(c, m, y, k));
		else
  		Carrier->doc->PageColors[col].setColor(c, m, y, k);
		}
	else
		{
		if (!Carrier->Prefs.DColors.contains(col))
			Carrier->Prefs.DColors.insert(col, CMYKColor(c, m, y, k));
		else
			Carrier->Prefs.DColors[col].setColor(c, m, y, k);
		}
	return Py_None;
}

PyObject *scribus_delcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Repl = "None";
	if (!PyArg_ParseTuple(args, "s|s", &Name, &Repl))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("deleteColor(colorname [, replacewithname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (Name == "")
		return Py_None;
	QString col = QString(Name);
	QString rep = QString(Repl);
	if (Carrier->HaveDoc)
		{
		if (Carrier->doc->PageColors.contains(col) && (Carrier->doc->PageColors.contains(rep) || (rep == "None")))
			{
			Carrier->doc->PageColors.remove(col);
			ReplaceColor(col, rep);
			}
		}
	else
		{
		if (Carrier->Prefs.DColors.contains(col))
			Carrier->Prefs.DColors.remove(col);
		}
	return Py_None;
}

PyObject *scribus_replcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Repl = "None";
	if (!PyArg_ParseTuple(args, "s|s", &Name, &Repl))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("replaceColor(colorname [, replacewithname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (Name == "")
		return Py_None;
	QString col = QString(Name);
	QString rep = QString(Repl);
	if (!Carrier->HaveDoc)
		return Py_None;
	if (Carrier->doc->PageColors.contains(col) && (Carrier->doc->PageColors.contains(rep) || (rep == "None")))
		ReplaceColor(col, rep);
	return Py_None;
}

