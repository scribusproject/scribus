#include <Python.h>
#include "cmdmani.h"
#include "cmdutil.h"
#include "cmdvar.h"

/*
07/10/2004 exceptions raised when wrong params called, most of the procs. are
handling the all pages - not just actual - now. pv.
 */

PyObject *scribus_loadimage(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Image;
	if (!PyArg_ParseTuple(args, "s|s", &Image, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("LoadImage(filename [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item == NULL)
	{
		PyErr_SetString(PyExc_Exception, QObject::tr("Oook! You're trying to load image into an object doesn't exist or isn't selected!"));
		return NULL;
	}
	item->OwnPage->LoadPict(QString(Image), item->ItemNr);
	return Py_None;
}

PyObject *scribus_scaleimage(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("ScaleImage(x, y [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *item = GetUniqueItem(Name);
	if (item != NULL && item->PType == 2)
	{
		item->LocalScX = x;
		item->LocalScY = y;
	}
	return Py_None;
}

PyObject *scribus_moveobjrel(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("MoveObject(x, y [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item!=NULL)
	{
		if (item->OwnPage->GroupSel)
			item->OwnPage->moveGroup(ValueToPoint(x), ValueToPoint(y));
		else
			item->OwnPage->MoveItem(ValueToPoint(x), ValueToPoint(y), item);
	}
	return Py_None;
}

PyObject *scribus_moveobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("MoveObjectAbs(x, y [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item != NULL)
	{
		if (item->OwnPage->GroupSel)
		{
			double x2, y2, w, h;
			item->OwnPage->getGroupRect(&x2, &y2, &w, &h);
			item->OwnPage->moveGroup(ValueToPoint(x) - x2, ValueToPoint(y) - y2);
		}
		else
			//PageItem *b = Carrier->doc->ActPage->Items.at(i);
			item->OwnPage->MoveItem(ValueToPoint(x) - item->Xpos, ValueToPoint(y) - item->Ypos, item);
	}
	return Py_None;
}

PyObject *scribus_rotobjrel(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x;
	if (!PyArg_ParseTuple(args, "d|s", &x, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("RotateObject(angle [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item != NULL)
		item->OwnPage->RotateItem(item->Rot - x, item->ItemNr);
	return Py_None;
}

PyObject *scribus_rotobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x;
	if (!PyArg_ParseTuple(args, "d|s", &x, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("RotateObjectAbs(angle [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item != NULL)
		item->OwnPage->RotateItem(x * -1.0, item->ItemNr);
	return Py_None;
}

PyObject *scribus_sizeobjabs(PyObject *self, PyObject* args)
{
	char *Name = "";
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|s", &x, &y, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SizeObject(x, y [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *item = GetUniqueItem(QString(Name));
	if (item != NULL)
	{
		item->OwnPage->SizeItem(ValueToPoint(x) - item->Xpos, ValueToPoint(y) - item->Ypos, item->ItemNr);
	}
	return Py_None;
}

PyObject *scribus_groupobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il = 0;
	if (!PyArg_ParseTuple(args, "|O", &il))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GroupObjects([list_of_objectnames])"));
		return NULL;
	}
	Page *p = Carrier->doc->ActPage;
	uint ap = Carrier->doc->ActPage->PageNr;
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
			PageItem *ic = GetUniqueItem(QString(Name));
			if (ic != NULL)
			{
				ic->OwnPage->SelectItemNr(ic->ItemNr);
				p = ic->OwnPage;
			}
		}
	}
	else
	{
		Py_INCREF(Py_None);
		if (!Carrier->HaveDoc)
			return Py_None;
	}
	if (p->SelItem.count() != 0)
	{
		Carrier->view->GotoPage(p->PageNr);
		Carrier->GroupObj();
		p->Deselect();
		Carrier->view->GotoPage(ap);
	}
	return Py_None;
}

PyObject *scribus_ungroupobj(PyObject *self, PyObject* args)
{
	char *Name = "";;
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("UnGroupObject([objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
	{
		uint p = Carrier->doc->ActPage->PageNr;
		Carrier->view->GotoPage(i->OwnPage->PageNr);
		Carrier->UnGroupObj();
		Carrier->view->GotoPage(p);
	}
	return Py_None;
}

PyObject *scribus_scalegroup(PyObject *self, PyObject* args)
{
	char *Name = "";
	double sc;
	if (!PyArg_ParseTuple(args, "d|s", &sc, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("ScaleGroup(scale [, objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	else
	if (sc == 0.0)
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
	{
		i->OwnPage->Deselect();
		i->OwnPage->SelectItemNr(i->ItemNr);
		int h = i->OwnPage->HowTo;
		i->OwnPage->HowTo = 1;
		i->OwnPage->scaleGroup(sc, sc);
		i->OwnPage->HowTo = h;
	}
	return Py_None;
}

PyObject *scribus_getselobjnam(PyObject *self, PyObject* args)
{
	int i = 0;
	if (!PyArg_ParseTuple(args, "|i", &i))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("GetSelectedObject([objectnumber])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	if ((i < static_cast<int>(Carrier->doc->ActPage->SelItem.count())) && (i > -1))
		return PyString_FromString(Carrier->doc->ActPage->SelItem.at(i)->AnName);
	else
		return PyString_FromString("");
}

PyObject *scribus_selcount(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SelectionCount()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	return PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->SelItem.count()));
}

PyObject *scribus_selectobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("SelectObject(objectname)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	if (Name == "")
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->OwnPage->SelectItemNr(i->ItemNr);
	return Py_None;
}

PyObject *scribus_deselect(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("DeselectAll()"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	for (uint i = 0; i < Carrier->view->Pages.count(); i++)
		Carrier->view->Pages.at(i)->Deselect();
	return Py_None;
}


PyObject *scribus_lockobject(PyObject *self, PyObject* args)
{
	char *name = "";
	if (!PyArg_ParseTuple(args, "|s", &name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("LockObject([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	PageItem *item = GetUniqueItem(QString(name));
	if (item == NULL)
	{
		PyErr_SetString(PyExc_Exception,
			QObject::tr("Oook! You're trying to (un)lock an object doesn't exist! None selected too."));
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
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("IsLocked([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
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
