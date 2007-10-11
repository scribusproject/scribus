/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdmani.h"
#include "cmdutil.h"
#include "propertiespalette.h" //CB argh.. noooooooooooooooooooooooooooooooooooo FIXME see other FIXME
#include "selection.h"
#include "scribuscore.h"

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
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	ScCore->primaryMainWindow()->doc->LoadPict(QString::fromUtf8(Image), item->ItemNr);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
	item->setImageXYScale(x, y);
	ScCore->primaryMainWindow()->doc->itemSelection_SetImageScale(x, y); //CB why when this is done above?
	ScCore->primaryMainWindow()->doc->updatePic();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
	//QPtrList<PageItem> oldSelection = ScCore->primaryMainWindow()->view->SelItem;
	//int tempList=ScCore->primaryMainWindow()->doc->selection->backupToTempList(0);
	Selection tempSelection(*ScCore->primaryMainWindow()->doc->m_Selection);
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	// Clear the selection
	ScCore->primaryMainWindow()->view->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	ScCore->primaryMainWindow()->view->SelectItemNr(item->ItemNr);
	// Move the item, or items
	if (ScCore->primaryMainWindow()->doc->m_Selection->count() > 1)
		ScCore->primaryMainWindow()->view->moveGroup(ValueToPoint(x), ValueToPoint(y));
	else
		ScCore->primaryMainWindow()->doc->MoveItem(ValueToPoint(x), ValueToPoint(y), item);
	// Now restore the selection. We just have to go through and select
	// each and every item, unfortunately.
	//ScCore->primaryMainWindow()->doc->selection->restoreFromTempList(0, tempList);
	*ScCore->primaryMainWindow()->doc->m_Selection=tempSelection;
	//ScCore->primaryMainWindow()->view->Deselect();
	//for ( oldSelection.first(); oldSelection.current(); oldSelection.next() )
	//	ScCore->primaryMainWindow()->view->SelectItemNr(oldSelection.current()->ItemNr);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
	//QPtrList<PageItem> oldSelection = ScCore->primaryMainWindow()->view->SelItem;
	//int tempList=ScCore->primaryMainWindow()->doc->selection->backupToTempList(0);
	Selection tempSelection(*ScCore->primaryMainWindow()->doc->m_Selection);
	// Clear the selection
	ScCore->primaryMainWindow()->view->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	ScCore->primaryMainWindow()->view->SelectItemNr(item->ItemNr);
	// Move the item, or items
	if (ScCore->primaryMainWindow()->doc->m_Selection->count() > 1)
	{
		double x2, y2, w, h;
		ScCore->primaryMainWindow()->doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
		ScCore->primaryMainWindow()->view->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
	}
	else
		ScCore->primaryMainWindow()->doc->MoveItem(pageUnitXToDocX(x) - item->xPos(), pageUnitYToDocY(y) - item->yPos(), item);
	// Now restore the selection. We just have to go through and select
	// each and every item, unfortunately.
	//ScCore->primaryMainWindow()->doc->selection->restoreFromTempList(0, tempList);
	*ScCore->primaryMainWindow()->doc->m_Selection=tempSelection;
	//ScCore->primaryMainWindow()->view->Deselect();
	//for ( oldSelection.first(); oldSelection.current(); oldSelection.next() )
//		ScCore->primaryMainWindow()->view->SelectItemNr(oldSelection.current()->ItemNr);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
	ScCore->primaryMainWindow()->doc->RotateItem(item->rotation() - x, item->ItemNr);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
	ScCore->primaryMainWindow()->doc->RotateItem(x * -1.0, item->ItemNr);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
	ScCore->primaryMainWindow()->doc->SizeItem(ValueToPoint(x), ValueToPoint(y), item->ItemNr);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_groupobj(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il = 0;
	if (!PyArg_ParseTuple(args, "|O", &il))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	uint ap = ScCore->primaryMainWindow()->doc->currentPage()->pageNr();
	// If we were passed a list of items to group...
	if (il != 0)
	{
		int len = PyList_Size(il);
		if (len < 2)
		{
			// We can't very well group only one item
			PyErr_SetString(NoValidObjectError, QObject::tr("Cannot group less than two items", "python error").toLocal8Bit().constData());
			return NULL;
		}
		QStringList oldSelection = getSelectedItemsByName();
		ScCore->primaryMainWindow()->view->Deselect();
		for (int i = 0; i < len; i++)
		{
			// FIXME: We might need to explicitly get this string as utf8
			// but as sysdefaultencoding is utf8 it should be a no-op to do
			// so anyway.
			Name = PyString_AsString(PyList_GetItem(il, i));
			PageItem *ic = GetUniqueItem(QString::fromUtf8(Name));
			if (ic == NULL)
				return NULL;
			ScCore->primaryMainWindow()->view->SelectItemNr(ic->ItemNr);
		}
		ScCore->primaryMainWindow()->GroupObj();
		setSelectedItemsByName(oldSelection);
	}
	// or if no argument list was given but there is a selection...
	else if (ScCore->primaryMainWindow()->doc->m_Selection->count() != 0)
	{
		if (ScCore->primaryMainWindow()->doc->m_Selection->count() < 2)
		{
			// We can't very well group only one item
			PyErr_SetString(NoValidObjectError, QObject::tr("Can't group less than two items", "python error").toLocal8Bit().constData());
			return NULL;
		}
		ScCore->primaryMainWindow()->GroupObj();
		ScCore->primaryMainWindow()->view->GotoPage(ap);
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("Need selection or argument list of items to group", "python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
	ScCore->primaryMainWindow()->UnGroupObj();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot scale by 0%.","python error").toLocal8Bit().constData());
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	ScCore->primaryMainWindow()->view->Deselect();
	ScCore->primaryMainWindow()->view->SelectItemNr(i->ItemNr);
//	int h = ScCore->primaryMainWindow()->view->frameResizeHandle;
//	ScCore->primaryMainWindow()->view->frameResizeHandle = 1;
	ScCore->primaryMainWindow()->view->scaleGroup(sc, sc);
//	ScCore->primaryMainWindow()->view->frameResizeHandle = h;
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_getselobjnam(PyObject* /* self */, PyObject* args)
{
	int i = 0;
	if (!PyArg_ParseTuple(args, "|i", &i))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((i < static_cast<int>(ScCore->primaryMainWindow()->doc->m_Selection->count())) && (i > -1))
		return PyString_FromString(ScCore->primaryMainWindow()->doc->m_Selection->itemAt(i)->itemName().toUtf8());
	else
		// FIXME: Should probably return None if no selection?
		return PyString_FromString("");
}

PyObject *scribus_selcount(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyInt_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->m_Selection->count()));
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
	ScCore->primaryMainWindow()->view->SelectItemNr(i->ItemNr);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_deselect(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	ScCore->primaryMainWindow()->view->Deselect();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toLocal8Bit().constData());
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
	ScCore->primaryMainWindow()->propertiesPalette->setLvalue(item->imageXScale(), item->imageYScale(), item->imageXOffset(), item->imageYOffset());
	item->AdjustPictScale();
	//ScCore->primaryMainWindow()->view->AdjustPictScale(item);
	
	item->update();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdmanidocwarnings()
{
    QStringList s;
    s << scribus_moveobjrel__doc__ << scribus_moveobjabs__doc__ << scribus_rotobjrel__doc__ << scribus_rotobjabs__doc__ << scribus_sizeobjabs__doc__ << scribus_getselobjnam__doc__ << scribus_selcount__doc__ << scribus_selectobj__doc__ <<scribus_deselect__doc__ <<scribus_groupobj__doc__ <<scribus_ungroupobj__doc__ <<scribus_scalegroup__doc__ <<scribus_loadimage__doc__ <<scribus_scaleimage__doc__ <<scribus_lockobject__doc__ <<scribus_islocked__doc__ <<scribus_setscaleimagetoframe__doc__;
}
