#include <Python.h>
#include "cmdmani.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_loadimage(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Image;
	if (!PyArg_ParseTuple(args, "s|s", &Image, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item == NULL)
		return NULL;
	Carrier->view->LoadPict(QString(Image), item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_scaleimage(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(Name);
	if (item == NULL)
		return NULL;
	if (item->PType == 2)
	{
		item->LocalScX = x;
		item->LocalScY = y;
	}
	else
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_moveobjrel(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item==NULL)
		return NULL;
	if (Carrier->view->GroupSel)
		Carrier->view->moveGroup(ValueToPoint(x), ValueToPoint(y));
	else
		Carrier->view->MoveItem(ValueToPoint(x), ValueToPoint(y), item);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_moveobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item == NULL)
		return NULL;
	if (Carrier->view->GroupSel)
	{
		if (Carrier->view->GroupSel)
		{
			double x2, y2, w, h;
			Carrier->view->getGroupRect(&x2, &y2, &w, &h);
			Carrier->view->moveGroup(ValueToPoint(x) - x2, ValueToPoint(y) - y2);
		}
		else
			Carrier->view->MoveItem(ValueToPoint(x) - item->Xpos, ValueToPoint(y) - item->Ypos, item);
	}
	else
		Carrier->view->MoveItem(ValueToPoint(x) - item->Xpos, ValueToPoint(y) - item->Ypos, item);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_rotobjrel(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x;
	if (!PyArg_ParseTuple(args, "d|s", &x, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item == NULL)
		return NULL;
	Carrier->view->RotateItem(item->Rot - x, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_rotobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x;
	if (!PyArg_ParseTuple(args, "d|s", &x, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item == NULL)
		return NULL;
	Carrier->view->RotateItem(x * -1.0, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_sizeobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item == NULL)
		return NULL;
	Carrier->view->SizeItem(ValueToPoint(x) - item->Xpos, ValueToPoint(y) - item->Ypos, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_groupobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il = 0;
	if (!PyArg_ParseTuple(args, "|O", &il))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	uint ap = Carrier->doc->ActPage->PageNr;
	if (il != 0)
	{
		int len = PyList_Size(il);
		if (len == 0)
		{
			Py_INCREF(Py_None);
			return Py_None;
		}
		for (int i = 0; i < len; i++)
		{
			Name = PyString_AsString(PyList_GetItem(il, i));
			PageItem *ic = GetUniqueItem(QString(Name));
			if (ic == NULL)
				return NULL;
			Carrier->view->SelectItemNr(ic->ItemNr);
		}
	}
	if (Carrier->view->SelItem.count() != 0)
	{
		Carrier->GroupObj();
		Carrier->view->Deselect();
		Carrier->view->GotoPage(ap);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_ungroupobj(PyObject *self, PyObject* args)
{
	char *Name = "";;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	Carrier->UnGroupObj();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_scalegroup(PyObject *self, PyObject* args)
{
	char *Name = "";
	double sc;
	if (!PyArg_ParseTuple(args, "d|s", &sc, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (sc == 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QString("Can't scale by 0%"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	Carrier->view->Deselect();
	Carrier->view->SelectItemNr(i->ItemNr);
	int h = Carrier->view->HowTo;
	Carrier->view->HowTo = 1;
	Carrier->view->scaleGroup(sc, sc);
	Carrier->view->HowTo = h;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getselobjnam(PyObject *self, PyObject* args)
{
	int i = 0;
	if (!PyArg_ParseTuple(args, "|i", &i))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((i < static_cast<int>(Carrier->view->SelItem.count())) && (i > -1))
		return PyString_FromString(Carrier->view->SelItem.at(i)->AnName);
	else
		return PyString_FromString("");
}

PyObject *scribus_selcount(PyObject *self)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(Carrier->view->SelItem.count()));
}

PyObject *scribus_selectobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	Carrier->view->SelectItemNr(i->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_deselect(PyObject *self)
{
	if(!checkHaveDocument())
		return NULL;
	Carrier->view->Deselect();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_lockobject(PyObject *self, PyObject* args)
{
	char *name = "";
	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(name));
	if (item == NULL)
		return NULL;
	item->Locked = !item->Locked;
	if (item->Locked)
		return PyInt_FromLong(1);
	return PyInt_FromLong(0);
}

PyObject *scribus_islocked(PyObject *self, PyObject* args)
{
	char *name = "";
	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;
	// FIXME: Rather than toggling the lock, we should probably let the user set the lock state
	// and instead provide a different function like toggleLock()
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(name);
	if (item == NULL)
		return NULL;
	if (item->Locked)
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}
