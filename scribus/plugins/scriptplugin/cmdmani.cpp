#include "cmdmani.h"
#include "cmdutil.h"

PyObject *scribus_loadimage(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Image;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Image, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == NULL)
		return NULL;
	if (item->itemType() != PageItem::ImageFrame)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.","python error"));
		return NULL;
	}
	Carrier->view->LoadPict(QString::fromUtf8(Image), item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_scaleimage(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == NULL)
		return NULL;
	if (item->itemType() != PageItem::ImageFrame)
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error"));
		return NULL;
	}
	item->LocalScX = x;
	item->LocalScY = y;
	Carrier->view->ChLocalSc(x, y);
	Carrier->view->UpdatePic();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_moveobjrel(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item==NULL)
		return NULL;
	// Grab the old selection
	QPtrList<PageItem> oldSelection = Carrier->view->SelItem;
	// Clear the selection
	Carrier->view->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	Carrier->view->SelectItemNr(item->ItemNr);
	// Move the item, or items
	if (Carrier->view->SelItem.count() > 1)
		Carrier->view->moveGroup(ValueToPoint(x), ValueToPoint(y));
	else
		Carrier->view->MoveItem(ValueToPoint(x), ValueToPoint(y), item);
	// Now restore the selection. We just have to go through and select
	// each and every item, unfortunately.
	Carrier->view->Deselect();
	for ( oldSelection.first(); oldSelection.current(); oldSelection.next() )
		Carrier->view->SelectItemNr(oldSelection.current()->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_moveobjabs(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == NULL)
		return NULL;
	// Grab the old selection
	QPtrList<PageItem> oldSelection = Carrier->view->SelItem;
	// Clear the selection
	Carrier->view->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	Carrier->view->SelectItemNr(item->ItemNr);
	// Move the item, or items
	if (Carrier->view->SelItem.count() > 1)
	{
		double x2, y2, w, h;
		Carrier->view->getGroupRect(&x2, &y2, &w, &h);
		Carrier->view->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
	}
	else
		Carrier->view->MoveItem(pageUnitXToDocX(x) - item->Xpos, pageUnitYToDocY(y) - item->Ypos, item);
	// Now restore the selection. We just have to go through and select
	// each and every item, unfortunately.
	Carrier->view->Deselect();
	for ( oldSelection.first(); oldSelection.current(); oldSelection.next() )
		Carrier->view->SelectItemNr(oldSelection.current()->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_rotobjrel(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x;
	if (!PyArg_ParseTuple(args, "d|es", &x, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == NULL)
		return NULL;
	Carrier->view->RotateItem(item->Rot - x, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_rotobjabs(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x;
	if (!PyArg_ParseTuple(args, "d|es", &x, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == NULL)
		return NULL;
	Carrier->view->RotateItem(x * -1.0, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_sizeobjabs(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == NULL)
		return NULL;
	Carrier->view->SizeItem(ValueToPoint(x), ValueToPoint(y), item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_groupobj(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il = 0;
	if (!PyArg_ParseTuple(args, "|O", &il))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	uint ap = Carrier->doc->currentPage->PageNr;
	// If we were passed a list of items to group...
	if (il != 0)
	{
		int len = PyList_Size(il);
		if (len < 2)
		{
			// We can't very well group only one item
			PyErr_SetString(NoValidObjectError, QObject::tr("Cannot group less than two items", "python error"));
			return NULL;
		}
		QStringList oldSelection = getSelectedItemsByName();
		Carrier->view->Deselect();
		for (int i = 0; i < len; i++)
		{
			// FIXME: We might need to explicitly get this string as utf8
			// but as sysdefaultencoding is utf8 it should be a no-op to do
			// so anyway.
			Name = PyString_AsString(PyList_GetItem(il, i));
			PageItem *ic = GetUniqueItem(QString::fromUtf8(Name));
			if (ic == NULL)
				return NULL;
			Carrier->view->SelectItemNr(ic->ItemNr);
		}
		Carrier->GroupObj();
		setSelectedItemsByName(oldSelection);
	}
	// or if no argument list was given but there is a selection...
	else if (Carrier->view->SelItem.count() != 0)
	{
		if (Carrier->view->SelItem.count() < 2)
		{
			// We can't very well group only one item
			PyErr_SetString(NoValidObjectError, QObject::tr("Can't group less than two items", "python error"));
			return NULL;
		}
		Carrier->GroupObj();
		Carrier->view->GotoPage(ap);
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("Need selection or argument list of items to group", "python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_ungroupobj(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	Carrier->UnGroupObj();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_scalegroup(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double sc;
	if (!PyArg_ParseTuple(args, "d|es", &sc, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (sc == 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot scale by 0%.","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
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

PyObject *scribus_getselobjnam(PyObject* /* self */, PyObject* args)
{
	int i = 0;
	if (!PyArg_ParseTuple(args, "|i", &i))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((i < static_cast<int>(Carrier->view->SelItem.count())) && (i > -1))
		return PyString_FromString(Carrier->view->SelItem.at(i)->itemName().utf8());
	else
		// FIXME: Should probably return None if no selection?
		return PyString_FromString("");
}

PyObject *scribus_selcount(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(Carrier->view->SelItem.count()));
}

PyObject *scribus_selectobj(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	Carrier->view->SelectItemNr(i->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_deselect(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	Carrier->view->Deselect();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_lockobject(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	item->toggleLock();
	if (item->locked())
		return PyInt_FromLong(1);
	return PyInt_FromLong(0);
}

PyObject *scribus_islocked(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	// FIXME: Rather than toggling the lock, we should probably let the user set the lock state
	// and instead provide a different function like toggleLock()
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if (item->locked())
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}

PyObject *scribus_setscaleimagetoframe(PyObject* /* self */, PyObject* args, PyObject* kw)
{
	char *name = const_cast<char*>("");
	long int scaleToFrame = 0;
	long int proportional = 1;
	char* kwargs[] = {"scaletoframe", "proportional", "name", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "i|ies", kwargs, &scaleToFrame, &proportional, "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if (item->itemType() != PageItem::ImageFrame)
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error"));
		return NULL;
	}
	// Set the item to scale if appropriate. ScaleType 1 is free
	// scale, 0 is scale to frame.
	item->ScaleType = scaleToFrame == 0;
	// Now, if the user has chosen to set the proportional mode,
	// set it. 1 is proportional, 0 is free aspect.
	if (proportional != -1)
		item->AspectRatio = proportional > 0;
	// Force the braindead app to notice the changes
	Carrier->view->AdjustPictScale(item);
	Carrier->view->RefreshItem(item);
	Py_INCREF(Py_None);
	return Py_None;
}
