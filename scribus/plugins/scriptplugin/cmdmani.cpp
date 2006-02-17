/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdmani.h"
#include "cmdutil.h"
#include "mpalette.h" //CB argh.. noooooooooooooooooooooooooooooooooooo FIXME see other FIXME
#include "selection.h"

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
	if (!item->asImageFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.","python error"));
		return NULL;
	}
	ScMW->doc->LoadPict(QString::fromUtf8(Image), item->ItemNr);
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
	if (! item->asImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error"));
		return NULL;
	}
	item->setImageXYScale(x, y);
	ScMW->doc->ChLocalSc(x, y);
	ScMW->view->UpdatePic();
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
	//QPtrList<PageItem> oldSelection = ScMW->view->SelItem;
	//int tempList=ScMW->doc->selection->backupToTempList(0);
	Selection tempSelection(*ScMW->doc->selection);
	ScMW->doc->selection->clear();
	// Clear the selection
	ScMW->view->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	ScMW->view->SelectItemNr(item->ItemNr);
	// Move the item, or items
	//if (ScMW->view->SelItem.count() > 1)
	if (ScMW->doc->selection->count() > 1)
		ScMW->view->moveGroup(ValueToPoint(x), ValueToPoint(y));
	else
		ScMW->view->MoveItem(ValueToPoint(x), ValueToPoint(y), item);
	// Now restore the selection. We just have to go through and select
	// each and every item, unfortunately.
	//ScMW->doc->selection->restoreFromTempList(0, tempList);
	*ScMW->doc->selection=tempSelection;
	//ScMW->view->Deselect();
	//for ( oldSelection.first(); oldSelection.current(); oldSelection.next() )
	//	ScMW->view->SelectItemNr(oldSelection.current()->ItemNr);
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
	//QPtrList<PageItem> oldSelection = ScMW->view->SelItem;
	//int tempList=ScMW->doc->selection->backupToTempList(0);
	Selection tempSelection(*ScMW->doc->selection);
	// Clear the selection
	ScMW->view->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	ScMW->view->SelectItemNr(item->ItemNr);
	// Move the item, or items
	//if (ScMW->view->SelItem.count() > 1)
	if (ScMW->doc->selection->count() > 1)
	{
		double x2, y2, w, h;
		ScMW->view->getGroupRect(&x2, &y2, &w, &h);
		ScMW->view->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
	}
	else
		ScMW->view->MoveItem(pageUnitXToDocX(x) - item->xPos(), pageUnitYToDocY(y) - item->yPos(), item);
	// Now restore the selection. We just have to go through and select
	// each and every item, unfortunately.
	//ScMW->doc->selection->restoreFromTempList(0, tempList);
	*ScMW->doc->selection=tempSelection;
	//ScMW->view->Deselect();
	//for ( oldSelection.first(); oldSelection.current(); oldSelection.next() )
//		ScMW->view->SelectItemNr(oldSelection.current()->ItemNr);
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
	ScMW->view->RotateItem(item->rotation() - x, item->ItemNr);
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
	ScMW->view->RotateItem(x * -1.0, item->ItemNr);
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
	ScMW->view->SizeItem(ValueToPoint(x), ValueToPoint(y), item->ItemNr);
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
	uint ap = ScMW->doc->currentPage->pageNr();
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
		ScMW->view->Deselect();
		for (int i = 0; i < len; i++)
		{
			// FIXME: We might need to explicitly get this string as utf8
			// but as sysdefaultencoding is utf8 it should be a no-op to do
			// so anyway.
			Name = PyString_AsString(PyList_GetItem(il, i));
			PageItem *ic = GetUniqueItem(QString::fromUtf8(Name));
			if (ic == NULL)
				return NULL;
			ScMW->view->SelectItemNr(ic->ItemNr);
		}
		ScMW->GroupObj();
		setSelectedItemsByName(oldSelection);
	}
	// or if no argument list was given but there is a selection...
	//else if (ScMW->view->SelItem.count() != 0)
	else if (ScMW->doc->selection->count() != 0)
	{
		//if (ScMW->view->SelItem.count() < 2)
		if (ScMW->doc->selection->count() < 2)
		{
			// We can't very well group only one item
			PyErr_SetString(NoValidObjectError, QObject::tr("Can't group less than two items", "python error"));
			return NULL;
		}
		ScMW->GroupObj();
		ScMW->view->GotoPage(ap);
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
	ScMW->UnGroupObj();
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
	ScMW->view->Deselect();
	ScMW->view->SelectItemNr(i->ItemNr);
	int h = ScMW->view->frameResizeHandle;
	ScMW->view->frameResizeHandle = 1;
	ScMW->view->scaleGroup(sc, sc);
	ScMW->view->frameResizeHandle = h;
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
	//if ((i < static_cast<int>(ScMW->view->SelItem.count())) && (i > -1))
	if ((i < static_cast<int>(ScMW->doc->selection->count())) && (i > -1))
		//return PyString_FromString(ScMW->view->SelItem.at(i)->itemName().utf8());
		return PyString_FromString(ScMW->doc->selection->itemAt(i)->itemName().utf8());
	else
		// FIXME: Should probably return None if no selection?
		return PyString_FromString("");
}

PyObject *scribus_selcount(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	//return PyInt_FromLong(static_cast<long>(ScMW->view->SelItem.count()));
	return PyInt_FromLong(static_cast<long>(ScMW->doc->selection->count()));
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
	ScMW->view->SelectItemNr(i->ItemNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_deselect(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScMW->view->Deselect();
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
	if (! item->asImageFrame())
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
	
	//FIXME emit or something so we dont need this
	ScMW->propertiesPalette->setLvalue(item->imageXScale(), item->imageYScale(), item->imageXOffset(), item->imageYOffset());
	item->AdjustPictScale();
	//ScMW->view->AdjustPictScale(item);
	
	ScMW->view->RefreshItem(item);
	Py_INCREF(Py_None);
	return Py_None;
}
