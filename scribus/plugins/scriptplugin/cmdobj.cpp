#include "cmdobj.h"
#include "cmdutil.h"

PyObject *scribus_newrect(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = NULL;
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &b, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (ItemExists(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists","python error"));
		return NULL;
	}
	int i = Carrier->doc->ActPage->PaintRect(ValueToPoint(x), ValueToPoint(y),
															ValueToPoint(b), ValueToPoint(h),
															Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	Carrier->doc->ActPage->SetRectFrame(Carrier->doc->ActPage->Items.at(i));
	if (Name != "")
		Carrier->doc->ActPage->Items.at(i)->AnName = QString::fromUtf8(Name);
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName.utf8());
}


PyObject *scribus_newellipse(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &b, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (ItemExists(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists","python error"));
		return NULL;
	}
	int i = Carrier->doc->ActPage->PaintEllipse(ValueToPoint(x), ValueToPoint(y), ValueToPoint(b), ValueToPoint(h),  Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	Carrier->doc->ActPage->SetOvalFrame(Carrier->doc->ActPage->Items.at(i));
	if (Name != "")
		Carrier->doc->ActPage->Items.at(i)->AnName = QString::fromUtf8(Name);
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName.utf8());
}


PyObject *scribus_newimage(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &b, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (ItemExists(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists","python error"));
		return NULL;
	}
	int i = Carrier->doc->ActPage->PaintPict(ValueToPoint(x), ValueToPoint(y), ValueToPoint(b), ValueToPoint(h));
	Carrier->doc->ActPage->SetRectFrame(Carrier->doc->ActPage->Items.at(i));
	if (Name != "")
		Carrier->doc->ActPage->Items.at(i)->AnName = QString::fromUtf8(Name);
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName.utf8());
}


PyObject *scribus_newtext(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &b, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (ItemExists(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists","python error"));
		return NULL;
	}
	int i = Carrier->doc->ActPage->PaintText(ValueToPoint(x), ValueToPoint(y),
															ValueToPoint(b), ValueToPoint(h),
															Carrier->doc->Dwidth, Carrier->doc->DpenText);
	Carrier->doc->ActPage->SetRectFrame(Carrier->doc->ActPage->Items.at(i));
	if (Name != "")
		Carrier->doc->ActPage->Items.at(i)->AnName = QString::fromUtf8(Name);
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName.utf8());
}


PyObject *scribus_newline(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &b, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (ItemExists(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists","python error"));
		return NULL;
	}
	x =	ValueToPoint(x);
	y = ValueToPoint(y);
	b = ValueToPoint(b);
	h = ValueToPoint(h);
	int i = Carrier->doc->ActPage->PaintPolyLine(x, y, 1, 1,	Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	PageItem *it = Carrier->doc->ActPage->Items.at(i);
	it->PoLine.resize(4);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	it->PoLine.setPoint(2, b-x, h-y);
	it->PoLine.setPoint(3, b-x, h-y);
	FPoint np2 = Carrier->doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
	{
		it->PoLine.translate(-np2.x(), 0);
		Carrier->doc->ActPage->MoveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		Carrier->doc->ActPage->MoveItem(0, np2.y(), it);
	}
	Carrier->doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), i, false, false);
	Carrier->doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString::fromUtf8(Name);
	return PyString_FromString(it->AnName.utf8());
}


PyObject *scribus_polyline(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il;
	// FIXME: PyList_Check failing will cause the function to return NULL w/o an exception. Separarate out the check.
	if ((!PyArg_ParseTuple(args, "O|es", &il, "utf-8", &Name)) || (!PyList_Check(il)))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	int len = PyList_Size(il);
	if (len < 4)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain at least two points (four values)","python error"));
		return NULL;
	}
	if ((len % 2) != 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain an even number of values","python error"));
		return NULL;
	}
	if (ItemExists(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists","python error"));
		return NULL;
	}
	double x, y, b, h;
	int i = 0;
	x = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = Carrier->doc->ActPage->PaintPolyLine(x, y, 1, 1,	Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	PageItem *it = Carrier->doc->ActPage->Items.at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	int pp = 6;
	for (i = 2; i < len - 2; i += 2)
	{
		b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
		h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+1))));
		it->PoLine.resize(pp);
		it->PoLine.setPoint(pp-4, b-x, h-y);
		it->PoLine.setPoint(pp-3, b-x, h-y);
		it->PoLine.setPoint(pp-2, b-x, h-y);
		it->PoLine.setPoint(pp-1, b-x, h-y);
		pp += 4;
	}
	pp -= 2;
	b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-2))));
	h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-1))));
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-2, b-x, h-y);
	it->PoLine.setPoint(pp-1, b-x, h-y);
	FPoint np2 = Carrier->doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
	{
		it->PoLine.translate(-np2.x(), 0);
		Carrier->doc->ActPage->MoveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		Carrier->doc->ActPage->MoveItem(0, np2.y(), it);
	}
	Carrier->doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false);
	Carrier->doc->ActPage->AdjustItemSize(it);
	if (Name != "")
	{
		it->AnName = QString::fromUtf8(Name);
	}
	return PyString_FromString(it->AnName.utf8());
}


PyObject *scribus_polygon(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il;
	// FIXME: PyList_Check failing will cause the function to return NULL w/o an exception. Separarate out the check.
	if ((!PyArg_ParseTuple(args, "O|es", &il, "utf-8", &Name)) || (!PyList_Check(il)))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	int len = PyList_Size(il);
	if (len < 6)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain at least three points (six values)","python error"));
		return NULL;
	}
	if ((len % 2) != 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain an even number of values","python error"));
		return NULL;
	}
	if (ItemExists(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists","python error"));
		return NULL;
	}
	double x, y, b, h;
	int i = 0;
	x = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = Carrier->doc->ActPage->PaintPoly(x, y, 1, 1,	Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	PageItem *it = Carrier->doc->ActPage->Items.at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	int pp = 6;
	for (i = 2; i < len - 2; i += 2)
	{
		b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
		h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+1))));
		it->PoLine.resize(pp);
		it->PoLine.setPoint(pp-4, b-x, h-y);
		it->PoLine.setPoint(pp-3, b-x, h-y);
		it->PoLine.setPoint(pp-2, b-x, h-y);
		it->PoLine.setPoint(pp-1, b-x, h-y);
		pp += 4;
	}
	b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-2))));
	h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-1))));
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-4, b-x, h-y);
	it->PoLine.setPoint(pp-3, b-x, h-y);
	it->PoLine.setPoint(pp-2, b-x, h-y);
	it->PoLine.setPoint(pp-1, b-x, h-y);
	pp += 2;
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-2, 0, 0);
	it->PoLine.setPoint(pp-1, 0, 0);
	FPoint np2 = Carrier->doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
	{
		it->PoLine.translate(-np2.x(), 0);
		Carrier->doc->ActPage->MoveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		Carrier->doc->ActPage->MoveItem(0, np2.y(), it);
	}
	Carrier->doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false);
	Carrier->doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString::fromUtf8(Name);
	return PyString_FromString(it->AnName.utf8());
}

PyObject *scribus_bezierline(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il;
	// FIXME: PyList_Check failing will cause the function to return NULL w/o an exception. Separarate out the check.
	if ((!PyArg_ParseTuple(args, "O|es", &il, "utf-8", &Name)) || (!PyList_Check(il)))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	int len = PyList_Size(il);
	if (len < 8)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain at least four points (eight values)","python error"));
		return NULL;
	}
	if ((len % 6) != 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must have a multiple of six values","python error"));
		return NULL;
	}
	if (ItemExists(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists","python error"));
		return NULL;
	}
	double x, y, b, h, kx, ky, kx2, ky2;
	int i = 0;
	x = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	kx = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	ky = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	kx2 = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	ky2 = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = Carrier->doc->ActPage->PaintPolyLine(x, y, 1, 1,	Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	PageItem *it = Carrier->doc->ActPage->Items.at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, kx-x, ky-y);
	int pp = 6;
	for (i = 6; i < len - 6; i += 6)
	{
		b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
		h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+1))));
		kx = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+2))));
		ky = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+3))));
		kx2 = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+4))));
		ky2 = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+5))));
		it->PoLine.resize(pp);
		it->PoLine.setPoint(pp-4, b-x, h-y);
		it->PoLine.setPoint(pp-3, kx-x, ky-y);
		it->PoLine.setPoint(pp-2, it->PoLine.point(pp-4));
		it->PoLine.setPoint(pp-1, kx2-x, ky2-y);
		pp += 4;
	}
	pp -= 2;
	b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-6))));
	h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-5))));
	kx = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-4))));
	ky = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-3))));
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-2, b-x, h-y);
	it->PoLine.setPoint(pp-1, kx-x, ky-y);
	FPoint np2 = Carrier->doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
	{
		it->PoLine.translate(-np2.x(), 0);
		Carrier->doc->ActPage->MoveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		Carrier->doc->ActPage->MoveItem(0, np2.y(), it);
	}
	Carrier->doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false);
	Carrier->doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString::fromUtf8(Name);
	return PyString_FromString(it->AnName.utf8());
}


/* 03/31/2004 - xception handling
 */
PyObject *scribus_pathtext(PyObject *self, PyObject* args)
{
	double x, y;
	char *Name = "";
	char *TextB = "";
	char *PolyB = "";
	if (!PyArg_ParseTuple(args, "ddeses|es", &x, &y, "utf-8", &TextB, "utf-8", &PolyB, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (ItemExists(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists","python error"));
		return NULL;
	}
	//FIXME: Why use GetItem not GetUniqueItem? Maybe use GetUniqueItem and use the exceptions
	// its sets for us?
	int i = GetItem(QString::fromUtf8(TextB));
	int ii = GetItem(QString::fromUtf8(PolyB));
	if ((i == -1) || (ii == -1))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Object not found","python error"));
		return NULL;
	}
	Carrier->doc->ActPage->SelItem.clear();
	Carrier->doc->ActPage->SelItem.append(Carrier->doc->ActPage->Items.at(i));
	Carrier->doc->ActPage->SelItem.append(Carrier->doc->ActPage->Items.at(ii));
	PageItem *it = Carrier->doc->ActPage->Items.at(i);
	// FIXME: If this fails, because of eg wrong item types, we don't notice. Worse, change the
	// name of the item BEFORE we fail to notice. Looks like a core bug.
	Carrier->doc->ActPage->ToPathText();
	Carrier->doc->ActPage->MoveItem(ValueToPoint(x) - it->Xpos, ValueToPoint(y) - it->Ypos, it);
	if (Name != "")
		it->AnName = QString::fromUtf8(Name);
	return PyString_FromString(it->AnName.utf8());
}


/* 03/21/2004 - exception raised when Name doesn't exists. Doesn't crash then. (subik)
 */
PyObject *scribus_deleteobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	i->OwnPage->SelItem.clear();
	i->OwnPage->SelItem.append(i);
	i->OwnPage->DeleteItem();
	Py_INCREF(Py_None);
	return Py_None;
}


/* 03/21/2004 - exception raises by non existent name (subik)
 */
PyObject *scribus_textflow(PyObject *self, PyObject* args)
{
	char *name = "";
	int state = -1;

	if (!PyArg_ParseTuple(args, "es|i", "utf-8", &name, &state))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == NULL)
		return NULL;
	if (state == -1)
		i->Textflow = !i->Textflow;
	else
		state ? i->Textflow = true : i->Textflow = false;
	Carrier->view->DrawNew();
	Carrier->slotDocCh(true);
	Py_INCREF(Py_None);
	return Py_None;
}


PyObject *scribus_objectexists(PyObject *self, PyObject* args)
{
	char* name = "";
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (ItemExists(QString::fromUtf8(name)))
		return PyBool_FromLong(static_cast<long>(true));
	return PyBool_FromLong(static_cast<long>(false));
}

/*
 * Craig Ringer, 2004-09-09
 * Apply the named style to the currently selected object.
 * pv, 2004-09-13, optionaly param objectName + "check the page" stuff
 */
PyObject *scribus_setstyle(PyObject *self, PyObject* args)
{
	char *style = "";
	char *name = "";
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &style, "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if ((item->PType == FRAME_TEXT) || (item->PType == FRAME_PATHTEXT))
	{
		/*
		 * First, find the style number associated with the requested style
		 * by scanning through the styles looking for the name. If
		 * we can't find it, raise PyExc_Exception.
		 * FIXME: Should use a more specific exception.
		 */
		bool found = false;
		uint styleid = 0;
		// We start at zero here because it's OK to match an internal name
		for (uint i=0; i < Carrier->doc->Vorlagen.count(); ++i)
		{
			if (Carrier->doc->Vorlagen[i].Vname == QString::fromUtf8(style)) {
				found = true;
				styleid = i;
				break;
			}
		}
		if (!found) {
			// whoops, the user specified an invalid style, complain loudly.
			PyErr_SetString(NotFoundError, QObject::tr("Style not found","python error"));
			return NULL;
		}
		// quick hack to always apply on the right frame - pv
		Carrier->doc->ActPage = item->OwnPage;
		Carrier->doc->ActPage->Deselect(true);
		Carrier->doc->ActPage->SelectItemNr(item->ItemNr);
		// Now apply the style.
		Carrier->setNewAbStyle(styleid);
	}
	else
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Can't set style on a non-text frame","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

/*
 * Craig Ringer, 2004-09-09
 * Enumerate all known paragraph styles
 */
PyObject *scribus_getstylenames(PyObject *self)
{
	PyObject *styleList;
	if(!checkHaveDocument())
		return NULL;
	styleList = PyList_New(0);
	/*
	We start at 5 because the lower styles are internal names.
	pv - changet to get all (with system) objects
	FIXME: this should be a constant defined by the scribus core
	*/
	for (uint i=0; i < Carrier->doc->Vorlagen.count(); ++i)
	{
		if (PyList_Append(styleList, PyString_FromString(Carrier->doc->Vorlagen[i].Vname.utf8())))
		{
			// An exception will have already been set by PyList_Append apparently.
			return NULL;
		}
	}
	return styleList;
}
