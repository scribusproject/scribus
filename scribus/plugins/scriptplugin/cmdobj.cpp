#include <Python.h>

#include "cmdobj.h"
#include "cmdutil.h"
#include "cmdvar.h"

// commented out in the code due the Scribus Scripts backward compatibility
#define ERROBJ "Oook! Object with given name exists already. Default name given."

PyObject *scribus_newrect(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createRect(x, y, w, h [, objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	int i = Carrier->doc->ActPage->PaintRect(ValueToPoint(x), ValueToPoint(y),
															ValueToPoint(b), ValueToPoint(h),
															Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	Carrier->doc->ActPage->SetRectFrame(Carrier->doc->ActPage->Items.at(i));
	if (GetUniqueItem(QString(Name)) == NULL)
		(Name != "") ? Carrier->doc->ActPage->Items.at(i)->AnName = QString(Name) : NULL;
/*	else
	{
		PyErr_SetString(PyExc_Exception, ERROBJ);
		return NULL;
	}*/
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName);
}


PyObject *scribus_newellipse(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createEllipse(x, y, w, h [, objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	int i = Carrier->doc->ActPage->PaintEllipse(ValueToPoint(x), ValueToPoint(y),
																 ValueToPoint(b), ValueToPoint(h),
																 Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	Carrier->doc->ActPage->SetOvalFrame(Carrier->doc->ActPage->Items.at(i));
	if (GetUniqueItem(QString(Name)) == NULL)
		if (Name != "")
			Carrier->doc->ActPage->Items.at(i)->AnName = QString(Name);
/*	else
	{
		PyErr_SetString(PyExc_Exception, ERROBJ);
		return NULL;
	}*/
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName);
}


PyObject *scribus_newimage(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createImage(x, y, w, h [, objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	int i = Carrier->doc->ActPage->PaintPict(ValueToPoint(x), ValueToPoint(y), ValueToPoint(b), ValueToPoint(h));
	Carrier->doc->ActPage->SetRectFrame(Carrier->doc->ActPage->Items.at(i));
	if (GetUniqueItem(QString(Name)) == NULL)
		if (Name != "")
			Carrier->doc->ActPage->Items.at(i)->AnName = QString(Name);
/*	else
	{
		PyErr_SetString(PyExc_Exception, ERROBJ);
		return NULL;
	}*/
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName);
}


PyObject *scribus_newtext(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createText(x, y, w, h [, objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	int i = Carrier->doc->ActPage->PaintText(ValueToPoint(x), ValueToPoint(y),
															ValueToPoint(b), ValueToPoint(h),
															Carrier->doc->Dwidth, Carrier->doc->DpenText);
	Carrier->doc->ActPage->SetRectFrame(Carrier->doc->ActPage->Items.at(i));
	if (GetUniqueItem(QString(Name)) == NULL)
	{
		if (Name != "")
			Carrier->doc->ActPage->Items.at(i)->AnName = QString(Name);
	}
/*	else
	{
		PyErr_SetString(PyExc_Exception, ERROBJ);
		return NULL;
	}*/
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName);
}


PyObject *scribus_newline(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createLine(x, y, w, h [, objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
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
	if (GetUniqueItem(QString(Name)) == NULL)
		if (Name != "")
			it->AnName = QString(Name);
/*	else
	{
		PyErr_SetString(PyExc_Exception, ERROBJ);
		return NULL;
	}*/
	return PyString_FromString(it->AnName);
}


PyObject *scribus_polyline(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il;
	if ((!PyArg_ParseTuple(args, "O|s", &il, &Name)) || (!PyList_Check(il)))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createPolyLine(pointlist [, objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	int len = PyList_Size(il);
	if ((len < 4) || ((len % 2) != 0))
		return PyString_FromString("");
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
	if (GetUniqueItem(QString(Name)) == NULL)
		if (Name != "")
			it->AnName = QString(Name);
/*	else
	{
		PyErr_SetString(PyExc_Exception, ERROBJ);
		return NULL;
	}*/
	return PyString_FromString(it->AnName);
}


PyObject *scribus_polygon(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il;
	if ((!PyArg_ParseTuple(args, "O|s", &il, &Name)) || (!PyList_Check(il)))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createPolygon(pointlist [, objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
	Py_INCREF(Py_None);
		return Py_None;
	}
	int len = PyList_Size(il);
	if ((len < 6) || ((len % 2) != 0))
		return PyString_FromString("");
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
	if (GetUniqueItem(QString(Name)) == NULL)
		if (Name != "")
			it->AnName = QString(Name);
/*	else
	{
		PyErr_SetString(PyExc_Exception, ERROBJ);
		return NULL;
	}*/
	return PyString_FromString(it->AnName);
}

PyObject *scribus_bezierline(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il;
	if ((!PyArg_ParseTuple(args, "O|s", &il, &Name)) || (!PyList_Check(il)))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createBezierLine(pointlist [, objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	int len = PyList_Size(il);
	if ((len < 8) || ((len % 6) != 0))
		return PyString_FromString("");
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
	if (GetUniqueItem(QString(Name)) == NULL)
		if (Name != "")
			it->AnName = QString(Name);
/*	else
	{
		PyErr_SetString(PyExc_Exception, ERROBJ);
		return NULL;
	}*/
	return PyString_FromString(it->AnName);
}


/* 03/31/2004 - xception handling
 */
PyObject *scribus_pathtext(PyObject *self, PyObject* args)
{
	double x, y;
	char *Name = "";
	char *TextB = "";
	char *PolyB = "";
	if (!PyArg_ParseTuple(args, "ddss|s", &x, &y, &TextB, &PolyB, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createPathText(x, y, textbox, beziercurve [, objectname])"));
		return NULL;
	}
	if ((!Carrier->HaveDoc) || ((TextB == "") || (PolyB == "")))
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	int i = GetItem(QString(TextB));
	int ii = GetItem(QString(PolyB));
	if ((i == -1) || (ii == -1))
	{
		PyErr_SetString(PyExc_Exception,
			QObject::tr("Oook! You're calling an object doesn't exist!"));
		return NULL;
	}
	Carrier->doc->ActPage->SelItem.clear();
	Carrier->doc->ActPage->SelItem.append(Carrier->doc->ActPage->Items.at(i));
	Carrier->doc->ActPage->SelItem.append(Carrier->doc->ActPage->Items.at(ii));
	PageItem *it = Carrier->doc->ActPage->Items.at(i);
	Carrier->doc->ActPage->ToPathText();
	Carrier->doc->ActPage->MoveItem(ValueToPoint(x) - it->Xpos, ValueToPoint(y) - it->Ypos, it);
	if (GetUniqueItem(QString(Name)) == NULL)
		if (Name != "")
			it->AnName = QString(Name);
/*	else
	{
		PyErr_SetString(PyExc_Exception, ERROBJ);
		return NULL;
	}*/
	return PyString_FromString(it->AnName);
}


/* 03/21/2004 - exception raised when Name doesn't exists. Doesn't crash then. (subik)
 */
PyObject *scribus_deleteobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("deleteObject([objectname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if (Name != NULL)
	{
		i->OwnPage->SelItem.clear();
		i->OwnPage->SelItem.append(i);
		i->OwnPage->DeleteItem();
	}
	else
	{
		PyErr_SetString(PyExc_Exception,
			QObject::tr("Oook! You're trying to erase an object doesn't exist!"));
		Py_DECREF(Py_None);
		return NULL;
	}
	return Py_None;
}


/* 03/21/2004 - exception raises by non existent name (subik)
 */
PyObject *scribus_textflow(PyObject *self, PyObject* args)
{
	char *name = "";
	int state = -1;

	if (!PyArg_ParseTuple(args, "s|i", &name, &state))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("textFlowsAroundFrame(objectname [,state])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	PageItem *i = GetUniqueItem(QString(name));
	if (i == NULL)
	{
		PyErr_SetString(PyExc_Exception, QObject::tr("Oook! An object you're trying to textflow doesn't exist!"));
		Py_DECREF(Py_None);
		return NULL;
	}
	if (state == -1)
		i->Textflow = !i->Textflow;
	else
		state ? i->Textflow = true : i->Textflow = false;
	Carrier->view->DrawNew();
	Carrier->slotDocCh(true);
	return Py_None;
}


PyObject *scribus_objectexists(PyObject *self, PyObject* args)
{
	char* name = "";

	if (!PyArg_ParseTuple(args, "|s", &name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("objectExists([objectname])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	if (GetUniqueItem(QString(name))==NULL)
		return PyInt_FromLong(static_cast<long>(false));
	return PyInt_FromLong(static_cast<long>(true));
}

/*
 * Craig Ringer, 2004-09-09
 * Apply the named style to the currently selected object.
 * pv, 2004-09-13, optionaly param objectName + "check the page" stuff
 * FIXME: should handled explicitly passed object name too.
 */
PyObject *scribus_setstyle(PyObject *self, PyObject* args)
{
	char *Style = "";
	char *name = "";
	if (!PyArg_ParseTuple(args, "s|s", &Style, &name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setStyle(style, [objectName])"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	PageItem *item = GetUniqueItem(QString(name));
	if ((item != NULL) && (item->PType == 4))
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
			if (Carrier->doc->Vorlagen[i].Vname == Style) {
				found = true;
				styleid = i;
				break;
			}
		}
		if (!found) {
			// whoops, the user specified an invalid style, complain loudly.
			PyErr_SetString(PyExc_Exception, ERRPARAM + QString("Style not found"));
			return NULL;
		}
		// quick hack to always apply on the right frame - pv
		Carrier->doc->ActPage = item->OwnPage;
		Carrier->doc->ActPage->Deselect(true);
		Carrier->doc->ActPage->SelectItemNr(item->ItemNr);
		// Now apply the style.
		Carrier->setNewAbStyle(styleid);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

/*
 * Craig Ringer, 2004-09-09
 * Enumerate all known paragraph styles
 */
PyObject *scribus_getstylenames(PyObject *self, PyObject* args)
{
	PyObject *styleList;
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getAllStyles()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	styleList = PyList_New(0);
	/*
	We start at 5 because the lower styles are internal names.
	pv - changet to get all (with system) objects
	FIXME: this should be a constant
	*/
	for (uint i=0; i < Carrier->doc->Vorlagen.count(); ++i)
	{
		if (PyList_Append(styleList, PyString_FromString(Carrier->doc->Vorlagen[i].Vname)))
		{
			// An exception will have already been set by PyList_Append apparently.
			return NULL;
		}
	}
	return styleList;
}
