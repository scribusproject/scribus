#include "cmdvar.h"
#include "cmdutil.h"
#include "cmdcolor.h"

void ReplaceColor(QString col, QString rep)
{
	for (uint b = 0; b < Carrier->view->Pages.count(); b++)
		{
		for (uint c = 0; c < Carrier->view->Pages.at(b)->Items.count(); c++)
			{
			if (Carrier->view->Pages.at(b)->Items.at(c)->PType == 4)
				{
				for (uint d = 0; d < Carrier->view->Pages.at(b)->Items.at(c)->Ptext.count(); d++)
					{
					if (col == Carrier->view->Pages.at(b)->Items.at(c)->Ptext.at(d)->ccolor)
						{
						Carrier->view->Pages.at(b)->Items.at(c)->Ptext.at(d)->ccolor = rep;
						}
					}
				}
			if (col == Carrier->view->Pages.at(b)->Items.at(c)->Pcolor)
				Carrier->view->Pages.at(b)->Items.at(c)->Pcolor = rep;
			if (col == Carrier->view->Pages.at(b)->Items.at(c)->Pcolor2)
				Carrier->view->Pages.at(b)->Items.at(c)->Pcolor2 = rep;
			}
		}
}

PyObject *scribus_colornames(PyObject *self, PyObject* args)
{
	CListe edc;
	PyObject *l;
	int cc = 0;
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
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
		return NULL;
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
		return NULL;
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
		return NULL;
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
		return NULL;
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
		return NULL;
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

