#include <Python.h>
#include "cmdmani.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_loadimage(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Image;
	if (!PyArg_ParseTuple(args, "s|s", &Image, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("loadImage(filename [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item == NULL)
	{
		PyErr_SetString(PyExc_Exception, QObject::tr("Oook! You're trying to load image into an object doesn't exist or isn't selected!"));
		return NULL;
	}
	Carrier->view->LoadPict(QString(Image), item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_scaleimage(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("scaleImage(x, y [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(Name);
	if (item != NULL && item->PType == 2)
	{
		item->LocalScX = x;
		item->LocalScY = y;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_moveobjrel(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("moveObject(x, y [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item!=NULL)
	{
		if (Carrier->view->GroupSel)
			Carrier->view->moveGroup(ValueToPoint(x), ValueToPoint(y));
		else
			Carrier->view->MoveItem(ValueToPoint(x), ValueToPoint(y), item);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_moveobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("moveObjectAbs(x, y [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item != NULL)
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
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_rotobjrel(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x;
	if (!PyArg_ParseTuple(args, "d|s", &x, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("rotateObject(angle [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item != NULL)
		Carrier->view->RotateItem(item->Rot - x, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_rotobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x;
	if (!PyArg_ParseTuple(args, "d|s", &x, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("rotateObjectAbs(angle [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item != NULL)
		Carrier->view->RotateItem(x * -1.0, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_sizeobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("sizeObject(x, y [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item != NULL)
		Carrier->view->SizeItem(ValueToPoint(x) - item->Xpos, ValueToPoint(y) - item->Ypos, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_groupobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il = 0;
	if (!PyArg_ParseTuple(args, "|O", &il))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("groupObjects([list_of_objectnames])"));
		return NULL;
	}
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
			if (ic != NULL)
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
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("unGroupObject([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
	{
		Carrier->UnGroupObj();
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_scalegroup(PyObject *self, PyObject* args)
{
	char *Name = "";
	double sc;
	if (!PyArg_ParseTuple(args, "d|s", &sc, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("scaleGroup(scale [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	if (sc == 0.0)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
	{
		Carrier->view->Deselect();
		Carrier->view->SelectItemNr(i->ItemNr);
		int h = Carrier->view->HowTo;
		Carrier->view->HowTo = 1;
		Carrier->view->scaleGroup(sc, sc);
		Carrier->view->HowTo = h;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getselobjnam(PyObject *self, PyObject* args)
{
	int i = 0;
	if (!PyArg_ParseTuple(args, "|i", &i))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getSelectedObject([objectnumber])"));
		return NULL;
	}
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
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("selectObject(objectname)"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
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
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("lockObject([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString(name));
	if (item == NULL)
	{
		PyErr_SetString(PyExc_Exception, QObject::tr("Oook! You're trying to (un)lock an object doesn't exist! None selected too."));
		return NULL;
	}
	item->Locked = !item->Locked;
	if (item->Locked)
		return PyInt_FromLong(1);
	return PyInt_FromLong(0);
}

PyObject *scribus_islocked(PyObject *self, PyObject* args)
{
	char *name = "";
	if (!PyArg_ParseTuple(args, "|s", &name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("isLocked([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(name);
	if (item == NULL)
	{
		PyErr_SetString(PyExc_Exception, QObject::tr("Oook! You're trying to query an object doesn't exist! None selected too."));
		return NULL;
	}
	if (item->Locked)
		return PyInt_FromLong(1);
	return PyInt_FromLong(0);
}
