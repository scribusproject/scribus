#include "cmdvar.h"
#include "cmdutil.h"
#include "cmdobj.h"

PyObject *scribus_newrect(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	int i;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = doc->ActPage->PaintRect(ValueToPoint(x), ValueToPoint(y),
															ValueToPoint(b), ValueToPoint(h),
															doc->Dwidth, doc->Dbrush, doc->Dpen);
	doc->ActPage->SetRectFrame(doc->ActPage->Items.at(i));
	if (Name != "")
		doc->ActPage->Items.at(i)->AnName = QString(Name);
	return PyString_FromString(doc->ActPage->Items.at(i)->AnName);
}

PyObject *scribus_newellipse(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	int i;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = doc->ActPage->PaintEllipse(ValueToPoint(x), ValueToPoint(y),
																 ValueToPoint(b), ValueToPoint(h),
																 doc->Dwidth, doc->Dbrush, doc->Dpen);
	doc->ActPage->SetOvalFrame(doc->ActPage->Items.at(i));
	if (Name != "")
		doc->ActPage->Items.at(i)->AnName = QString(Name);
	return PyString_FromString(doc->ActPage->Items.at(i)->AnName);
}

PyObject *scribus_newimage(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	int i;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = doc->ActPage->PaintPict(ValueToPoint(x), ValueToPoint(y), ValueToPoint(b), ValueToPoint(h));
	doc->ActPage->SetRectFrame(doc->ActPage->Items.at(i));
	if (Name != "")
		doc->ActPage->Items.at(i)->AnName = QString(Name);
	return PyString_FromString(doc->ActPage->Items.at(i)->AnName);
}

PyObject *scribus_newtext(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	int i;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = doc->ActPage->PaintText(ValueToPoint(x), ValueToPoint(y),
															ValueToPoint(b), ValueToPoint(h),
															doc->Dwidth, doc->DpenText);
	doc->ActPage->SetRectFrame(doc->ActPage->Items.at(i));
	if (Name != "")
		doc->ActPage->Items.at(i)->AnName = QString(Name);
	return PyString_FromString(doc->ActPage->Items.at(i)->AnName);
}

PyObject *scribus_newline(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	int i;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	x =	ValueToPoint(x);
	y = ValueToPoint(y);
	b = ValueToPoint(b);
	h = ValueToPoint(h);
	i = doc->ActPage->PaintPolyLine(x, y, 1, 1,	doc->Dwidth, doc->Dbrush, doc->Dpen);
	PageItem *it = doc->ActPage->Items.at(i);
	it->PoLine.resize(4);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	it->PoLine.setPoint(2, b-x, h-y);
	it->PoLine.setPoint(3, b-x, h-y);
	FPoint np2 = doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
		{
		it->PoLine.translate(-np2.x(), 0);
		doc->ActPage->MoveItem(np2.x(), 0, it);
		}
	if (np2.y() < 0)
		{
		it->PoLine.translate(0, -np2.y());
		doc->ActPage->MoveItem(0, np2.y(), it);
		}
	doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), i, false, false);
	doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

PyObject *scribus_polyline(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	int i = 0;
	int ic, pp, len;
	char *Name = "";
	PyObject *il;
	if ((!PyArg_ParseTuple(args, "O|s", &il, &Name)) || (!PyList_Check(il)))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	len = PyList_Size(il);
	if ((len < 4) || ((len % 2) != 0))
		return PyString_FromString("");
	x = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	ic = doc->ActPage->PaintPolyLine(x, y, 1, 1,	doc->Dwidth, doc->Dbrush, doc->Dpen);
	PageItem *it = doc->ActPage->Items.at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	pp = 6;
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
	FPoint np2 = doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
		{
		it->PoLine.translate(-np2.x(), 0);
		doc->ActPage->MoveItem(np2.x(), 0, it);
		}
	if (np2.y() < 0)
		{
		it->PoLine.translate(0, -np2.y());
		doc->ActPage->MoveItem(0, np2.y(), it);
		}
	doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false);
	doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

PyObject *scribus_polygon(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	int i = 0;
	int ic, pp, len;
	char *Name = "";
	PyObject *il;
	if ((!PyArg_ParseTuple(args, "O|s", &il, &Name)) || (!PyList_Check(il)))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	len = PyList_Size(il);
	if ((len < 6) || ((len % 2) != 0))
		return PyString_FromString("");
	x = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	ic = doc->ActPage->PaintPoly(x, y, 1, 1,	doc->Dwidth, doc->Dbrush, doc->Dpen);
	PageItem *it = doc->ActPage->Items.at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	pp = 6;
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
	FPoint np2 = doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
		{
		it->PoLine.translate(-np2.x(), 0);
		doc->ActPage->MoveItem(np2.x(), 0, it);
		}
	if (np2.y() < 0)
		{
		it->PoLine.translate(0, -np2.y());
		doc->ActPage->MoveItem(0, np2.y(), it);
		}
	doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false);
	doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

PyObject *scribus_bezierline(PyObject *self, PyObject* args)
{
	double x, y, b, h, kx, ky, kx2, ky2;
	int i = 0;
	int ic, pp, len;
	char *Name = "";
	PyObject *il;
	if ((!PyArg_ParseTuple(args, "O|s", &il, &Name)) || (!PyList_Check(il)))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	len = PyList_Size(il);
	if ((len < 8) || ((len % 6) != 0))
		return PyString_FromString("");
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
	ic = doc->ActPage->PaintPolyLine(x, y, 1, 1,	doc->Dwidth, doc->Dbrush, doc->Dpen);
	PageItem *it = doc->ActPage->Items.at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, kx-x, ky-y);
	pp = 6;
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
	FPoint np2 = doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
		{
		it->PoLine.translate(-np2.x(), 0);
		doc->ActPage->MoveItem(np2.x(), 0, it);
		}
	if (np2.y() < 0)
		{
		it->PoLine.translate(0, -np2.y());
		doc->ActPage->MoveItem(0, np2.y(), it);
		}
	doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false);
	doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

PyObject *scribus_pathtext(PyObject *self, PyObject* args)
{
	int i, ii;
	double x, y;
	char *Name = "";
	char *TextB = "";
	char *PolyB = "";
	if (!PyArg_ParseTuple(args, "ddss|s", &x, &y, &TextB, &PolyB, &Name))
		return NULL;
	if ((!Carrier->HaveDoc) || ((TextB == "") || (PolyB == "")))
		return PyString_FromString("");
	i = GetItem(QString(TextB));
	ii = GetItem(QString(PolyB));
	if ((i == -1) || (ii == -1))
		return PyString_FromString("");
	doc->ActPage->SelItem.clear();
	doc->ActPage->SelItem.append(doc->ActPage->Items.at(i));
	doc->ActPage->SelItem.append(doc->ActPage->Items.at(ii));
	PageItem *it = doc->ActPage->Items.at(i);
	doc->ActPage->ToPathText();
	doc->ActPage->MoveItem(ValueToPoint(x) - it->Xpos, ValueToPoint(y) - it->Ypos, it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

PyObject *scribus_deleteobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	if (Name != "")
		{
		doc->ActPage->SelItem.clear();
		i = GetItem(QString(Name));
		if (i != -1)
			doc->ActPage->SelItem.append(doc->ActPage->Items.at(i));
		}
	doc->ActPage->DeleteItem();
	return Py_None;
}

