#include "cmdmani.h"
#include "cmdutil.h"

PyObject *scribus_loadimage(PyObject */*self*/, PyObject* args)
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
	if (item->PType != FRAME_IMAGE)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.","python error"));
		return NULL;
	}
	item->OwnPage->LoadPict(QString::fromUtf8(Image), item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_scaleimage(PyObject */*self*/, PyObject* args)
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
	if (!item->PType == FRAME_IMAGE)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.","python error"));
		return NULL;
	}
	item->LocalScX = x;
	item->LocalScY = y;
	item->OwnPage->ChLocalSc(x, y);
	item->OwnPage->UpdatePic();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_moveobjrel(PyObject */*self*/, PyObject* args)
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
	QPtrList<PageItem> oldSelection = item->OwnPage->SelItem;
	// Clear the selection
	item->OwnPage->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	item->OwnPage->SelectItemNr(item->ItemNr);
	// Move the item, or items
	if (item->OwnPage->SelItem.count() > 1)
		item->OwnPage->moveGroup(ValueToPoint(x), ValueToPoint(y));
	else
		item->OwnPage->MoveItem(ValueToPoint(x), ValueToPoint(y), item);
	// Now restore the selection. We just have to go through and select
	// each and every item, unfortunately.
	item->OwnPage->Deselect();
	for ( oldSelection.first(); oldSelection.current(); oldSelection.next() )
		item->OwnPage->SelectItemNr(oldSelection.current()->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_moveobjabs(PyObject */*self*/, PyObject* args)
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
	QPtrList<PageItem> oldSelection = item->OwnPage->SelItem;
	// Clear the selection
	item->OwnPage->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	item->OwnPage->SelectItemNr(item->ItemNr);
	// Move the item, or items
	if (item->OwnPage->SelItem.count() > 1)
	{
		double x2, y2, w, h;
		item->OwnPage->getGroupRect(&x2, &y2, &w, &h);
		item->OwnPage->moveGroup(ValueToPoint(x) - x2, ValueToPoint(y) - y2);
	}
	else
		item->OwnPage->MoveItem(ValueToPoint(x) - item->Xpos, ValueToPoint(y) - item->Ypos, item);
	// Now restore the selection. We just have to go through and select
	// each and every item, unfortunately.
	item->OwnPage->Deselect();
	for ( oldSelection.first(); oldSelection.current(); oldSelection.next() )
		item->OwnPage->SelectItemNr(oldSelection.current()->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_rotobjrel(PyObject */*self*/, PyObject* args)
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
	item->OwnPage->RotateItem(item->Rot - x, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_rotobjabs(PyObject */*self*/, PyObject* args)
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
	item->OwnPage->RotateItem(x * -1.0, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_sizeobjabs(PyObject */*self*/, PyObject* args)
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
	item->OwnPage->SizeItem(ValueToPoint(x) - item->Xpos, ValueToPoint(y) - item->Ypos, item->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_groupobj(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il = 0;
	if (!PyArg_ParseTuple(args, "|O", &il))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	Page *p = Carrier->doc->ActPage;
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
			// FIXME: We might need to explicitly get this string as utf8
			// but as sysdefaultencoding is utf8 it should be a no-op to do
			// so anyway.
			Name = PyString_AsString(PyList_GetItem(il, i));
			PageItem *ic = GetUniqueItem(QString::fromUtf8(Name));
			if (ic == NULL)
				return NULL;
			ic->OwnPage->SelectItemNr(ic->ItemNr);
			p = ic->OwnPage;
		}
	}
	if (p->SelItem.count() != 0)
	{
		Carrier->view->GotoPage(p->PageNr);
		Carrier->GroupObj();
		p->Deselect();
		Carrier->view->GotoPage(ap);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_ungroupobj(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	uint p = Carrier->doc->ActPage->PageNr;
	Carrier->view->GotoPage(i->OwnPage->PageNr);
	Carrier->UnGroupObj();
	Carrier->view->GotoPage(p);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_scalegroup(PyObject */*self*/, PyObject* args)
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
	i->OwnPage->Deselect();
	i->OwnPage->SelectItemNr(i->ItemNr);
	int h = i->OwnPage->HowTo;
	i->OwnPage->HowTo = 1;
	i->OwnPage->scaleGroup(sc, sc);
	i->OwnPage->HowTo = h;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getselobjnam(PyObject */*self*/, PyObject* args)
{
	int i = 0;
	if (!PyArg_ParseTuple(args, "|i", &i))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((i < static_cast<int>(Carrier->doc->ActPage->SelItem.count())) && (i > -1))
		return PyString_FromString(Carrier->doc->ActPage->SelItem.at(i)->AnName);
	else
		// FIXME: Should probably return None if no selection?
		return PyString_FromString("");
}

PyObject *scribus_selcount(PyObject */*self*/)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->SelItem.count()));
}

PyObject *scribus_selectobj(PyObject */*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	i->OwnPage->SelectItemNr(i->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_deselect(PyObject */*self*/)
{
	if(!checkHaveDocument())
		return NULL;
	for (uint i = 0; i < Carrier->view->Pages.count(); i++)
		Carrier->view->Pages.at(i)->Deselect();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_lockobject(PyObject */*self*/, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	// FIXME: Rather than toggling the lock, we should probably let the user set the lock state
	// and instead provide a different function like toggleLock()
	item->Locked = !item->Locked;
	if (item->Locked)
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}

PyObject *scribus_islocked(PyObject */*self*/, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if (item->Locked)
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}

PyObject *scribus_setscaleimagetoframe(PyObject */*self*/, PyObject* args, PyObject* kw)
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
	if (item->PType != FRAME_IMAGE)
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
	item->OwnPage->AdjustPictScale(item);
	item->OwnPage->AdjustPreview(item, false);
	item->OwnPage->RefreshItem(item);
	Py_INCREF(Py_None);
	return Py_None;
}
