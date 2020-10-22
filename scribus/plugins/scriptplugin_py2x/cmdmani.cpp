/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdmani.h"
#include "cmdutil.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstream.h"
#include "selection.h"
#include "undomanager.h"

PyObject *scribus_loadimage(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Image;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Image, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->doc->loadPict(QString::fromUtf8(Image), item);
	Py_RETURN_NONE;
}

PyObject *scribus_scaleimage(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);

	// scale
	currentDoc->itemSelection_SetImageScale(x, y); //CB why when this is done above?
	currentDoc->updatePic();

	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection=tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_setimagescale(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);

	// scale
	double newScaleX = x / item->pixm.imgInfo.xres * 72.0;
	double newScaleY = y / item->pixm.imgInfo.yres * 72.0;
	currentDoc->itemSelection_SetImageScale(newScaleX, newScaleY); //CB why when this is done above?
	currentDoc->updatePic();

	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection=tempSelection;

	Py_RETURN_NONE;
}
PyObject *scribus_setimageoffset(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*ScCore->primaryMainWindow()->doc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);

	// offset
	double newOffsetX = x / ((item->imageXScale() != 0.0) ? item->imageXScale() : 1);
	double newOffsetY = y / ((item->imageYScale() != 0.0) ? item->imageYScale() : 1);
	currentDoc->itemSelection_SetImageOffset(newOffsetX, newOffsetY); //CB why when this is done above?
	currentDoc->updatePic();

	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection=tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_setimagebrightness(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double n;
	if (!PyArg_ParseTuple(args, "d|es", &n, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	ImageEffect ef;
	ef.effectCode = ImageEffect::EF_BRIGHTNESS;
	ScTextStream fp(&ef.effectParameters, QIODevice::WriteOnly);
	fp << n;

	item->effectsInUse.append(ef);
	item->pixm.applyEffect(item->effectsInUse, ScCore->primaryMainWindow()->doc->PageColors, false);
	
	ScCore->primaryMainWindow()->doc->updatePic();
	Py_RETURN_NONE;
}

PyObject *scribus_setimagegrayscale(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	ImageEffect ef;
	ef.effectCode = ImageEffect::EF_GRAYSCALE;

	item->effectsInUse.append(ef);
	item->pixm.applyEffect(item->effectsInUse, ScCore->primaryMainWindow()->doc->PageColors, false);
	
	ScCore->primaryMainWindow()->doc->updatePic();
	Py_RETURN_NONE;
}

PyObject *scribus_moveobjectrel(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item==nullptr)
		return nullptr;

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);
	// Move the item, or items
	if (currentDoc->m_Selection->count() > 1)
	{
		currentView->startGroupTransaction(Um::Move, "", Um::IMove);
		currentDoc->moveGroup(ValueToPoint(x), ValueToPoint(y));
		currentView->endGroupTransaction();
	}
	else {
		currentDoc->moveItem(ValueToPoint(x), ValueToPoint(y), item);
		}
	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection=tempSelection;
	Py_RETURN_NONE;
}

PyObject *scribus_moveobjectabs(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);
	// Move the item, or items
	if (currentDoc->m_Selection->count() > 1)
	{
		currentView->startGroupTransaction(Um::Move, "", Um::IMove);
		double x2, y2, w, h;
		currentDoc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
		currentDoc->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
		currentView->endGroupTransaction();
	}
	else
		currentDoc->moveItem(pageUnitXToDocX(x) - item->xPos(), pageUnitYToDocY(y) - item->yPos(), item);
	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection=tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_rotateobjectrel(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x;
	if (!PyArg_ParseTuple(args, "d|es", &x, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	ScCore->primaryMainWindow()->doc->rotateItem(item->rotation() - x, item);
	Py_RETURN_NONE;
}

PyObject *scribus_rotateobjectabs(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x;
	if (!PyArg_ParseTuple(args, "d|es", &x, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	ScCore->primaryMainWindow()->doc->rotateItem(x * -1.0, item);
	Py_RETURN_NONE;
}

PyObject *scribus_sizeobject(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	ScCore->primaryMainWindow()->doc->sizeItem(ValueToPoint(x), ValueToPoint(y), item);
	Py_RETURN_NONE;
}

PyObject *scribus_groupobjects(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il = nullptr;
	if (!PyArg_ParseTuple(args, "|O", &il))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (il == nullptr && ScCore->primaryMainWindow()->doc->m_Selection->count() < 2)
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("Need selection or argument list of items to group", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	Selection *tempSelection = nullptr;
	Selection *finalSelection = nullptr;
	//uint ap = ScCore->primaryMainWindow()->doc->currentPage()->pageNr();
	// If we were passed a list of items to group...
	if (il != nullptr)
	{
		int len = PyList_Size(il);
		tempSelection = new Selection(ScCore->primaryMainWindow(), false);
		for (int i = 0; i < len; i++)
		{
			// FIXME: We might need to explicitly get this string as utf8
			// but as sysdefaultencoding is utf8 it should be a no-op to do
			// so anyway.
			Name = PyString_AsString(PyList_GetItem(il, i));
			PageItem *ic = GetUniqueItem(QString::fromUtf8(Name));
			if (ic == nullptr)
			{
				delete tempSelection;
				return nullptr;
			}
			tempSelection->addItem (ic, true);
		}
		finalSelection = tempSelection;
	}
	else
		finalSelection = ScCore->primaryMainWindow()->doc->m_Selection;
	if (finalSelection->count() < 2)
	{
		// We can't very well group only one item
		PyErr_SetString(NoValidObjectError, QObject::tr("Cannot group less than two items", "python error").toLocal8Bit().constData());
		finalSelection=nullptr;
		delete tempSelection;
		return nullptr;
	}

	const PageItem* group = ScCore->primaryMainWindow()->doc->itemSelection_GroupObjects(false, false, finalSelection);
	finalSelection=nullptr;
	delete tempSelection;
	
	return (group ? PyString_FromString(group->itemName().toUtf8()) : nullptr);
}

PyObject *scribus_ungroupobjects(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == nullptr)
		return nullptr;

	ScribusMainWindow* currentWin = ScCore->primaryMainWindow();
	currentWin->view->deselectItems();
	currentWin->view->selectItem(i);
	currentWin->UnGroupObj();

	Py_RETURN_NONE;
}

PyObject *scribus_scalegroup(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double sc;
	if (!PyArg_ParseTuple(args, "d|es", &sc, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (sc == 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot scale by 0%.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == nullptr)
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;

	currentView->deselectItems();
	currentView->selectItem(i);
//	int h = currentView->frameResizeHandle;
//	currentView->frameResizeHandle = 1;
	currentView->startGroupTransaction(Um::Resize, "", Um::IResize);
	currentDoc->scaleGroup(sc, sc);
	currentView->endGroupTransaction();
//	currentView->frameResizeHandle = h;
	Py_RETURN_NONE;
}

PyObject *scribus_getselectedobject(PyObject* /* self */, PyObject* args)
{
	int i = 0;
	if (!PyArg_ParseTuple(args, "|i", &i))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	Selection * selection = ScCore->primaryMainWindow()->doc->m_Selection;
	if ((i < selection->count()) && (i > -1))
		return PyString_FromString(selection->itemAt(i)->itemName().toUtf8());
	// FIXME: Should probably return None if no selection?
	return PyString_FromString("");
}

PyObject *scribus_selectioncount(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	return PyInt_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->m_Selection->count()));
}

PyObject *scribus_selectobject(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == nullptr)
		return nullptr;
	ScCore->primaryMainWindow()->view->selectItem(i);
	Py_RETURN_NONE;
}

PyObject *scribus_deselectall(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->view->deselectItems();
	Py_RETURN_NONE;
}

PyObject *scribus_lockobject(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	// FIXME: Rather than toggling the lock, we should probably let the user set the lock state
	// and instead provide a different function like toggleLock()
	item->toggleLock();
	if (item->locked())
		return PyInt_FromLong(1);
	return PyInt_FromLong(0);
}

PyObject *scribus_islocked(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if (item->locked())
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}

PyObject *scribus_setscaleframetoimage(PyObject* /* self */, PyObject* args)
{
	if (!checkHaveDocument())
		return nullptr;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Specified item not an image frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Selection *sel = new Selection(ScCore->primaryMainWindow());
	sel->addItem(item);
	ScCore->primaryMainWindow()->doc->itemSelection_AdjustFrametoImageSize(sel);
	delete sel;

	Py_RETURN_NONE;
}

PyObject *scribus_setscaleimagetoframe(PyObject* /* self */, PyObject* args, PyObject* kw)
{
	char *name = const_cast<char*>("");
	long int scaleToFrame = 0;
	long int proportional = 1;
	char* kwargs[] = {const_cast<char*>("scaletoframe"),
		const_cast<char*>("proportional"), const_cast<char*>("name"), nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "i|ies", kwargs, &scaleToFrame, &proportional, "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	// Set the item to scale if appropriate. ScaleType 1 is free
	// scale, 0 is scale to frame.
	item->ScaleType = scaleToFrame == 0;
	// Now, if the user has chosen to set the proportional mode,
	// set it. 1 is proportional, 0 is free aspect.
	if (proportional != -1)
		item->AspectRatio = proportional > 0;

	item->adjustPictScale();
	item->update();

	Py_RETURN_NONE;
}
PyObject *scribus_flipobject(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double h, v;
	if (!PyArg_ParseTuple(args, "dd|es", &h, &v, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	
	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);

	// flip
	if (h == 1)
		currentDoc->itemSelection_FlipH();
	if (v == 1)
		currentDoc->itemSelection_FlipV();
	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection = tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_combinepolygons(PyObject * /* self */)
{
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	Selection* curSelection = currentDoc->m_Selection;
	if (curSelection->count() <= 1)
		Py_RETURN_NONE;

	bool canUniteItems = true;
	for (int i = 0; i < curSelection->count(); ++i)
	{
		PageItem* it = currentDoc->m_Selection->itemAt(i);
		if ((!it->isPolygon()) || (!it->isPolyLine()))
			canUniteItems = false;
	}

	if (!canUniteItems)
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Selection must contain only shapes or bezier curves.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	currentDoc->itemSelection_UniteItems(nullptr);

	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdmanidocwarnings()
{
	QStringList s;
	s << scribus_combinepolygons__doc__
	  << scribus_deselectall__doc__
	  << scribus_flipobject__doc__
	  << scribus_getselectedobject__doc__
	  << scribus_groupobjects__doc__
	  << scribus_islocked__doc__
	  << scribus_loadimage__doc__
	  << scribus_lockobject__doc__
      << scribus_moveobjectabs__doc__
	  << scribus_moveobjectrel__doc__
	  << scribus_rotateobjectabs__doc__
	  << scribus_rotateobjectrel__doc__
	  << scribus_scalegroup__doc__
	  << scribus_scaleimage__doc__
	  << scribus_selectioncount__doc__
	  << scribus_selectobject__doc__
	  << scribus_setimagebrightness__doc__
	  << scribus_setimagegrayscale__doc__
	  << scribus_setimageoffset__doc__
	  << scribus_setimagescale__doc__
	  << scribus_setscaleframetoimage__doc__
	  << scribus_setscaleimagetoframe__doc__
	  << scribus_sizeobject__doc__ 
	  << scribus_ungroupobjects__doc__;
}
