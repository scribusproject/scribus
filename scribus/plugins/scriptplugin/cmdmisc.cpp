#include <Python.h>
#include "cmdmisc.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_setredraw(PyObject *self, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	Carrier->doc->DoDrawing = static_cast<bool>(e);
	return Py_None;
}

PyObject *scribus_fontnames(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	PyObject *l = PyList_New(Carrier->Prefs.AvailFonts.count());
	SCFontsIterator it(Carrier->Prefs.AvailFonts);
	int cc = 0;
	for ( ; it.current() ; ++it)
		{
		PyList_SetItem(l, cc, PyString_FromString(it.currentKey()));
		cc++;
		}
	return l;
}

PyObject *scribus_getlayers(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	PyObject *l;	
	if (Carrier->HaveDoc)
		{
		l = PyList_New(Carrier->doc->Layers.count());
		for (uint lam=0; lam < Carrier->doc->Layers.count(); lam++)
			{
			PyList_SetItem(l, lam, PyString_FromString(Carrier->doc->Layers[lam].Name));
			}
		}
	else
		l = PyList_New(0);
	return l;
}

PyObject *scribus_setactlayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == ""))
		return Py_None;
	int i = -1;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
		{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
			{
			i = static_cast<int>(lam);
			Carrier->doc->ActiveLayer = i;
			Carrier->changeLayer(i);
			break;
			}
		}
	return Py_None;
}

PyObject *scribus_getactlayer(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	return PyString_FromString(Carrier->doc->Layers[Carrier->doc->ActiveLayer].Name);
}

PyObject *scribus_senttolayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Layer = "";
	if (!PyArg_ParseTuple(args, "s|s", &Layer, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Layer == ""))
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		{
		PageItem *b = Carrier->doc->ActPage->Items.at(i);
		Carrier->doc->ActPage->SelectItemNr(i);
		for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
			{
			if (Carrier->doc->Layers[lam].Name == QString(Layer))
				{
				b->LayerNr = static_cast<int>(lam);
				break;
				}
			}
		}
	return Py_None;
}

PyObject *scribus_layervisible(PyObject *self, PyObject* args)
{
	char *Name = "";
	int vis = 1;
	if (!PyArg_ParseTuple(args, "si", &Name, &vis))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == ""))
		return Py_None;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
		{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
			{
			Carrier->doc->Layers[lam].Sichtbar = vis;
			break;
			}
		}
	return Py_None;
}

PyObject *scribus_layerprint(PyObject *self, PyObject* args)
{
	char *Name = "";
	int vis = 1;
	if (!PyArg_ParseTuple(args, "si", &Name, &vis))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == ""))
		return Py_None;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
		{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
			{
			Carrier->doc->Layers[lam].Drucken = vis;
			break;
			}
		}
	return Py_None;
}

PyObject *scribus_glayervisib(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	if ((!Carrier->HaveDoc) || (Name == ""))
		return PyInt_FromLong(0L);
	int i = 0;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); lam++)
		{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
			{
			i = static_cast<int>(Carrier->doc->Layers[lam].Sichtbar);
			break;
			}
		}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_glayerprint(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	if ((!Carrier->HaveDoc) || (Name == ""))
		return PyInt_FromLong(0L);
	int i = 0;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
		{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
			{
			i = static_cast<int>(Carrier->doc->Layers[lam].Drucken);
			break;
			}
		}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_removelayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == "") || (Carrier->doc->Layers.count() == 1))
		return Py_None;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
		{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
			{
			QValueList<Layer>::iterator it2 = Carrier->doc->Layers.at(lam);
			int num2 = (*it2).LNr;
			if (!num2)
				return Py_None;
			int num = (*it2).Level;
			Carrier->doc->Layers.remove(it2);
			QValueList<Layer>::iterator it;
			for (uint l = 0; l < Carrier->doc->Layers.count(); l++)
				{
				it = Carrier->doc->Layers.at(l);
				if ((*it).Level > num)
					(*it).Level -= 1;
				}
			Carrier->LayerRemove(num2);
			Carrier->doc->ActiveLayer = 0;
			Carrier->changeLayer(0);
			break;
			}
		}
	return Py_None;
}

PyObject *scribus_createlayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == ""))
		return Py_None;
	QString tmp;
	struct Layer ll;
	ll.LNr = Carrier->doc->Layers.last().LNr + 1;
	ll.Level = Carrier->doc->Layers.count();
	ll.Name = QString(Name);
	ll.Sichtbar = true;
	ll.Drucken = true;
	Carrier->doc->Layers.append(ll);
	Carrier->doc->ActiveLayer = ll.LNr;
	Carrier->changeLayer(ll.LNr);
	return Py_None;
}

