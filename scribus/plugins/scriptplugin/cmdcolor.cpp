#include "cmdcolor.h"
#include "cmdutil.h"

PyObject *scribus_colornames(PyObject *self)
{
	CListe edc;
	PyObject *l;
	int cc = 0;
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
	if (Name == "")
	{
		PyErr_SetString(ScribusException, QString("Cannot change a colour with an empty name."));
		return NULL;
	}
	QString col = QString(Name);
	if (Carrier->HaveDoc)
	{
		if (!Carrier->doc->PageColors.contains(col))
		{
			PyErr_SetString(ScribusException, QString("Colour does not exist in document"));
			return NULL;
		}
		Carrier->doc->PageColors[col].setColor(c, m, y, k);
	}
	else
	{
		if (!Carrier->Prefs.DColors.contains(col))
		{
			PyErr_SetString(ScribusException, QString("Colour does not exist in preferences"));
			return NULL;
		}
		Carrier->Prefs.DColors[col].setColor(c, m, y, k);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_newcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "siiii", &Name, &c, &m, &y, &k))
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(ScribusException, QString("Cannot create a colour with an empty name."));
		return NULL;
	}
	QString col = QString(Name);
	if (Carrier->HaveDoc)
		{
			if (!Carrier->doc->PageColors.contains(col))
				Carrier->doc->PageColors.insert(col, CMYKColor(c, m, y, k));
			else
				// FIXME: Given that we have a changeColour function, should we really be
				// silently changing colours in newColour?
				Carrier->doc->PageColors[col].setColor(c, m, y, k);
		}
	else
		{
			if (!Carrier->Prefs.DColors.contains(col))
				Carrier->Prefs.DColors.insert(col, CMYKColor(c, m, y, k));
			else
				// FIXME: Given that we have a changeColour function, should we really be
				// silently changing colours in newColour?
				Carrier->Prefs.DColors[col].setColor(c, m, y, k);
		}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_delcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Repl = "None";
	if (!PyArg_ParseTuple(args, "s|s", &Name, &Repl))
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(ScribusException, QString("Cannot delete a colour with an empty name."));
		return NULL;
	}
	QString col = QString(Name);
	QString rep = QString(Repl);
	if (Carrier->HaveDoc)
	{
		// FIXME: should we raise an exception when the user tries to delete a colour that
		// does not exist?
		if (Carrier->doc->PageColors.contains(col) && (Carrier->doc->PageColors.contains(rep) || (rep == "None")))
			{
				Carrier->doc->PageColors.remove(col);
				ReplaceColor(col, rep);
			}
	}
	else
	{
		// FIXME: should we raise an exception when the user tries to delete a colour that
		// does not exist?
		if (Carrier->Prefs.DColors.contains(col))
			Carrier->Prefs.DColors.remove(col);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_replcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Repl = "None";
	//FIXME: this should definitely use keyword arguments
	if (!PyArg_ParseTuple(args, "s|s", &Name, &Repl))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(ScribusException, QString("Cannot replace a colour with an empty name."));
		return NULL;
	}
	QString col = QString(Name);
	QString rep = QString(Repl);
	// FIXME: should we raise an error when the user tries to replace a colour and the colour
	// they're trying to replace does not exist?
	if (Carrier->doc->PageColors.contains(col) && (Carrier->doc->PageColors.contains(rep) || (rep == "None")))
		ReplaceColor(col, rep);
	Py_INCREF(Py_None);
	return Py_None;
}

