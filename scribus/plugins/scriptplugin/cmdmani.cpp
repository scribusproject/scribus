#include "cmdvar.h"
#include "cmdutil.h"
#include "cmdmani.h"

PyObject *scribus_loadimage(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Image;
	if (!PyArg_ParseTuple(args, "s|s", &Image, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		doc->ActPage->LoadPict(QString(Image), i);
	return Py_None;
}

PyObject *scribus_scaleimage(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if ((i != -1) && (doc->ActPage->Items.at(i)->PType == 2))
		{
		PageItem *b = doc->ActPage->Items.at(i);
		b->LocalScX = x;
		b->LocalScY = y;
		}
	return Py_None;
}

PyObject *scribus_moveobjrel(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		{
		if (doc->ActPage->GroupSel)
			doc->ActPage->moveGroup(ValueToPoint(x), ValueToPoint(y));
		else
			doc->ActPage->MoveItem(ValueToPoint(x), ValueToPoint(y), doc->ActPage->Items.at(i));
		}
	return Py_None;
}

PyObject *scribus_moveobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		{
		if (doc->ActPage->GroupSel)
			{
			double x2, y2, w, h;
			doc->ActPage->getGroupRect(&x2, &y2, &w, &h);
			doc->ActPage->moveGroup(ValueToPoint(x) - x2, ValueToPoint(y) - y2);
			}
		else
			{
			PageItem *b = doc->ActPage->Items.at(i);
			doc->ActPage->MoveItem(ValueToPoint(x) - b->Xpos, ValueToPoint(y) - b->Ypos, b);
			}
		}
	return Py_None;
}

PyObject *scribus_rotobjrel(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x;
	if (!PyArg_ParseTuple(args, "d|s", &x, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		doc->ActPage->RotateItem(doc->ActPage->Items.at(i)->Rot - x, i);
	return Py_None;
}

PyObject *scribus_rotobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x;
	if (!PyArg_ParseTuple(args, "d|s", &x, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		doc->ActPage->RotateItem(x * -1.0, i);
	return Py_None;
}

PyObject *scribus_sizeobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		{
		PageItem *b = doc->ActPage->Items.at(i);
		doc->ActPage->SizeItem(ValueToPoint(x) - b->Xpos, ValueToPoint(y) - b->Ypos, i);
		}
	return Py_None;
}

PyObject *scribus_groupobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il = 0;
	if (!PyArg_ParseTuple(args, "|O", &il))
		return NULL;
	if (il != 0)
		{
		if (!PyList_Check(il))
			return NULL;
		Py_INCREF(Py_None);
		if (!Carrier->HaveDoc)
			return Py_None;
		int len = PyList_Size(il);
		if (len == 0)
			return Py_None;
		for (int i = 0; i < len; i++)
			{
			Name = PyString_AsString(PyList_GetItem(il, i));
			int ic = GetItem(QString(Name));
			if (ic != -1)
				doc->ActPage->SelectItemNr(ic);
			}
		}
	else
		{
		Py_INCREF(Py_None);
		if (!Carrier->HaveDoc)
			return Py_None;
		}
	if (doc->ActPage->SelItem.count() != 0)
		{
		Carrier->GroupObj();
		doc->ActPage->Deselect();
		}
	return Py_None;
}

PyObject *scribus_ungroupobj(PyObject *self, PyObject* args)
{
	char *Name = "";;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		Carrier->UnGroupObj();
	return Py_None;
}

PyObject *scribus_scalegroup(PyObject *self, PyObject* args)
{
	char *Name = "";
	double sc;
	if (!PyArg_ParseTuple(args, "d|s", &sc, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	else
	if (sc == 0.0)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		{
		doc->ActPage->Deselect();
		doc->ActPage->SelectItemNr(i);
		int h = doc->ActPage->HowTo;
		doc->ActPage->HowTo = 1;
		doc->ActPage->scaleGroup(sc, sc);
		doc->ActPage->HowTo = h;
		}
	return Py_None;
}

PyObject *scribus_getselobjnam(PyObject *self, PyObject* args)
{
	int i = 0;
	if (!PyArg_ParseTuple(args, "|i", &i))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	if ((i < static_cast<int>(doc->ActPage->SelItem.count())) && (i > -1))
		return PyString_FromString(doc->ActPage->SelItem.at(i)->AnName);
	else
		return PyString_FromString("");
}

PyObject *scribus_selcount(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	return PyInt_FromLong(static_cast<long>(doc->ActPage->SelItem.count()));
}

PyObject *scribus_selectobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	if (Name == "")
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		doc->ActPage->SelectItemNr(i);
	return Py_None;
}

PyObject *scribus_deselect(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	Py_INCREF(Py_None);
	if (Carrier->HaveDoc)
		doc->ActPage->Deselect();
	return Py_None;
}

