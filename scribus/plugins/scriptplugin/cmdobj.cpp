/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdobj.h"
#include "cmdutil.h"
#include "selection.h"
#include "util_math.h"
#include "commonstrings.h"
#include "scribuscore.h"


PyObject *scribus_newrect(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");

	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error"));
//		return NULL;
//	}
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::Polygon, PageItem::Rectangle,
								pageUnitXToDocX(x), pageUnitYToDocY(y),
								ValueToPoint(w), ValueToPoint(h),
								ScCore->primaryMainWindow()->doc->toolSettings.dWidth,
								ScCore->primaryMainWindow()->doc->toolSettings.dBrush, ScCore->primaryMainWindow()->doc->toolSettings.dPen, true);
//	ScCore->primaryMainWindow()->doc->setRedrawBounding(ScCore->primaryMainWindow()->doc->Items->at(i));
	if (Name != EMPTY_STRING)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
//	if (Name != EMPTY_STRING)
//		ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(QString::fromUtf8(Name));
	return PyString_FromString(ScCore->primaryMainWindow()->doc->Items->at(i)->itemName().toUtf8());
}


PyObject *scribus_newellipse(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::Polygon, PageItem::Ellipse,
										pageUnitXToDocX(x),
										pageUnitYToDocY(y),
										ValueToPoint(w),
										ValueToPoint(h),
										ScCore->primaryMainWindow()->doc->toolSettings.dWidth,
										ScCore->primaryMainWindow()->doc->toolSettings.dBrush,
										ScCore->primaryMainWindow()->doc->toolSettings.dPen,
										true);
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error"));
//		return NULL;
//	}
//	ScCore->primaryMainWindow()->doc->setRedrawBounding(ScCore->primaryMainWindow()->doc->Items->at(i));
//	if (Name != EMPTY_STRING)
//		ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(QString::fromUtf8(Name));
	if (Name != EMPTY_STRING)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyString_FromString(ScCore->primaryMainWindow()->doc->Items->at(i)->itemName().toUtf8());
}


PyObject *scribus_newimage(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified,
									pageUnitXToDocX(x),
									pageUnitYToDocY(y),
									ValueToPoint(w),
									ValueToPoint(h),
									1, ScCore->primaryMainWindow()->doc->toolSettings.dBrushPict,
									CommonStrings::None, true);
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error"));
//		return NULL;
//	}
//	ScCore->primaryMainWindow()->doc->setRedrawBounding(ScCore->primaryMainWindow()->doc->Items->at(i));
//	if (Name != EMPTY_STRING)
//		ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(QString::fromUtf8(Name));
	if (Name != EMPTY_STRING)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyString_FromString(ScCore->primaryMainWindow()->doc->Items->at(i)->itemName().toUtf8());
}


PyObject *scribus_newtext(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified,
								pageUnitXToDocX(x),
								pageUnitYToDocY(y),
								ValueToPoint(w),
								ValueToPoint(h),
								ScCore->primaryMainWindow()->doc->toolSettings.dWidth, CommonStrings::None,
								ScCore->primaryMainWindow()->doc->toolSettings.dPenText, true);
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error"));
//		return NULL;
//	}
//	ScCore->primaryMainWindow()->doc->setRedrawBounding(ScCore->primaryMainWindow()->doc->Items->at(i));
	if (Name != EMPTY_STRING)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyString_FromString(ScCore->primaryMainWindow()->doc->Items->at(i)->itemName().toUtf8());
}

PyObject *scribus_newline(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	x = pageUnitXToDocX(x);
	y = pageUnitYToDocY(y);
	w = pageUnitXToDocX(w);
	h = pageUnitYToDocY(h);
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError,
//						QObject::tr("An object with the requested name already exists.",
//									"python error"));
//		return NULL;
//	}
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::Line, PageItem::Unspecified,
							   x, y, w, h,
							   ScCore->primaryMainWindow()->doc->toolSettings.dWidth,
							   ScCore->primaryMainWindow()->doc->toolSettings.dBrush,
							   ScCore->primaryMainWindow()->doc->toolSettings.dPen, true);
	PageItem *it = ScCore->primaryMainWindow()->doc->Items->at(i);
	it->setRotation(xy2Deg(w-x, h-y));
	it->setWidthHeight(sqrt(pow(x-w, 2.0) + pow(y-h, 2.0)), 1.0);
	it->Sizing = false;
	it->updateClip();
	it->setRedrawBounding();
//	ScCore->primaryMainWindow()->doc->setRedrawBounding(it);
/* WTF? maybe I'll examine who's author later. Or maybe I'll remove it later ;)
	it->PoLine.resize(4);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	it->PoLine.setPoint(2, w-x, h-y);
	it->PoLine.setPoint(3, w-x, h-y);
	FPoint np2 = getMinClipF(&it->PoLine);
	if (np2.x() < 0)
	{
		it->PoLine.translate(-np2.x(), 0);
		ScCore->primaryMainWindow()->view->MoveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		ScCore->primaryMainWindow()->view->MoveItem(0, np2.y(), it);
	}
	ScCore->primaryMainWindow()->view->SizeItem(it->PoLine.WidthHeight().x(),
						 it->PoLine.WidthHeight().y(), i, false, false, false);
	ScCore->primaryMainWindow()->view->AdjustItemSize(it);*/
//	if (Name != EMPTY_STRING)
//		it->setItemName(QString::fromUtf8(Name));
	if (Name != EMPTY_STRING)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyString_FromString(it->itemName().toUtf8());
}


PyObject *scribus_polyline(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il;
	// FIXME: PyList_Check failing will cause the function to return NULL w/o an exception. Separarate out the check.
	if ((!PyArg_ParseTuple(args, "O|es", &il, "utf-8", &Name)) || (!PyList_Check(il)))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	int len = PyList_Size(il);
	if (len < 4)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain at least two points (four values).","python error").toLocal8Bit().constData());
		return NULL;
	}
	if ((len % 2) != 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain an even number of values.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error").toLocal8Bit().constData());
//		return NULL;
//	}
	double x, y, w, h;
	int i = 0;
	x = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, 1, 1,	ScCore->primaryMainWindow()->doc->toolSettings.dWidth, ScCore->primaryMainWindow()->doc->toolSettings.dBrush, ScCore->primaryMainWindow()->doc->toolSettings.dPen, true);
	PageItem *it = ScCore->primaryMainWindow()->doc->Items->at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	int pp = 6;
	for (i = 2; i < len - 2; i += 2)
	{
		w = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
		h = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+1))));
		it->PoLine.resize(pp);
		it->PoLine.setPoint(pp-4, w-x, h-y);
		it->PoLine.setPoint(pp-3, w-x, h-y);
		it->PoLine.setPoint(pp-2, w-x, h-y);
		it->PoLine.setPoint(pp-1, w-x, h-y);
		pp += 4;
	}
	pp -= 2;
	w = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-2))));
	h = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-1))));
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-2, w-x, h-y);
	it->PoLine.setPoint(pp-1, w-x, h-y);
	FPoint np2 = getMinClipF(&it->PoLine);
	if (np2.x() < 0)
	{
		it->PoLine.translate(-np2.x(), 0);
		ScCore->primaryMainWindow()->doc->MoveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		ScCore->primaryMainWindow()->doc->MoveItem(0, np2.y(), it);
	}
	ScCore->primaryMainWindow()->doc->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false, false);
	ScCore->primaryMainWindow()->doc->AdjustItemSize(it);
//	if (Name != EMPTY_STRING)
//	{
//		it->setItemName(QString::fromUtf8(Name));
//	}
	if (Name != EMPTY_STRING)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(ic)->setItemName(objName);
	}
	return PyString_FromString(it->itemName().toUtf8());
}


PyObject *scribus_polygon(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il;
	// FIXME: PyList_Check failing will cause the function to return NULL w/o an exception. Separarate out the check.
	if ((!PyArg_ParseTuple(args, "O|es", &il, "utf-8", &Name)) || (!PyList_Check(il)))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	int len = PyList_Size(il);
	if (len < 6)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain at least three points (six values).","python error").toLocal8Bit().constData());
		return NULL;
	}
	if ((len % 2) != 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain an even number of values.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error").toLocal8Bit().constData());
//		return NULL;
//	}
	double x, y, w, h;
	int i = 0;
	x = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, 1, 1,	ScCore->primaryMainWindow()->doc->toolSettings.dWidth, ScCore->primaryMainWindow()->doc->toolSettings.dBrush, ScCore->primaryMainWindow()->doc->toolSettings.dPen, true);
	PageItem *it = ScCore->primaryMainWindow()->doc->Items->at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	int pp = 6;
	for (i = 2; i < len - 2; i += 2)
	{
		w = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
		h = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+1))));
		it->PoLine.resize(pp);
		it->PoLine.setPoint(pp-4, w-x, h-y);
		it->PoLine.setPoint(pp-3, w-x, h-y);
		it->PoLine.setPoint(pp-2, w-x, h-y);
		it->PoLine.setPoint(pp-1, w-x, h-y);
		pp += 4;
	}
	w = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-2))));
	h = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-1))));
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-4, w-x, h-y);
	it->PoLine.setPoint(pp-3, w-x, h-y);
	it->PoLine.setPoint(pp-2, w-x, h-y);
	it->PoLine.setPoint(pp-1, w-x, h-y);
	pp += 2;
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-2, 0, 0);
	it->PoLine.setPoint(pp-1, 0, 0);
	FPoint np2 = getMinClipF(&it->PoLine);
	if (np2.x() < 0)
	{
		it->PoLine.translate(-np2.x(), 0);
		ScCore->primaryMainWindow()->doc->MoveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		ScCore->primaryMainWindow()->doc->MoveItem(0, np2.y(), it);
	}
	ScCore->primaryMainWindow()->doc->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false, false);
	ScCore->primaryMainWindow()->doc->AdjustItemSize(it);
//	if (Name != EMPTY_STRING)
//		it->setItemName(QString::fromUtf8(Name));
	if (Name != EMPTY_STRING)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(ic)->setItemName(objName);
	}
	return PyString_FromString(it->itemName().toUtf8());
}

PyObject *scribus_bezierline(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il;
	// FIXME: PyList_Check failing will cause the function to return NULL w/o an exception. Separarate out the check.
	if ((!PyArg_ParseTuple(args, "O|es", &il, "utf-8", &Name)) || (!PyList_Check(il)))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	int len = PyList_Size(il);
	if (len < 8)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain at least four points (eight values).","python error").toLocal8Bit().constData());
		return NULL;
	}
	if ((len % 6) != 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must have a multiple of six values.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error").toLocal8Bit().constData());
//		return NULL;
//	}
	double x, y, w, h, kx, ky, kx2, ky2;
	int i = 0;
	x = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	kx = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	ky = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	kx2 = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	ky2 = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	//int ic = ScCore->primaryMainWindow()->view->PaintPolyLine(x, y, 1, 1,	ScCore->primaryMainWindow()->doc->toolSettings.dWidth, ScCore->primaryMainWindow()->doc->toolSettings.dBrush, ScCore->primaryMainWindow()->doc->toolSettings.dPen);
	int ic = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, 1, 1,	ScCore->primaryMainWindow()->doc->toolSettings.dWidth, ScCore->primaryMainWindow()->doc->toolSettings.dBrush, ScCore->primaryMainWindow()->doc->toolSettings.dPen, true);
	PageItem *it = ScCore->primaryMainWindow()->doc->Items->at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, kx-x, ky-y);
	int pp = 6;
	for (i = 6; i < len - 6; i += 6)
	{
		w = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
		h = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+1))));
		kx = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+2))));
		ky = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+3))));
		kx2 = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+4))));
		ky2 = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+5))));
		it->PoLine.resize(pp);
		it->PoLine.setPoint(pp-4, w-x, h-y);
		it->PoLine.setPoint(pp-3, kx-x, ky-y);
		it->PoLine.setPoint(pp-2, it->PoLine.point(pp-4));
		it->PoLine.setPoint(pp-1, kx2-x, ky2-y);
		pp += 4;
	}
	pp -= 2;
	w = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-6))));
	h = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-5))));
	kx = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-4))));
	ky = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-3))));
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-2, w-x, h-y);
	it->PoLine.setPoint(pp-1, kx-x, ky-y);
	FPoint np2 = getMinClipF(&it->PoLine);
	if (np2.x() < 0)
	{
		it->PoLine.translate(-np2.x(), 0);
		ScCore->primaryMainWindow()->doc->MoveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		ScCore->primaryMainWindow()->doc->MoveItem(0, np2.y(), it);
	}
	ScCore->primaryMainWindow()->doc->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false, false);
	ScCore->primaryMainWindow()->doc->AdjustItemSize(it);
//	if (Name != EMPTY_STRING)
//		it->setItemName(QString::fromUtf8(Name));
	if (Name != EMPTY_STRING)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(ic)->setItemName(objName);
	}
	return PyString_FromString(it->itemName().toUtf8());
}


/* 03/31/2004 - xception handling
 */
PyObject *scribus_pathtext(PyObject* /* self */, PyObject* args)
{
	double x, y;
	char *Name = const_cast<char*>("");
	char *TextB = const_cast<char*>("");
	char *PolyB = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "ddeses|es", &x, &y, "utf-8", &TextB, "utf-8", &PolyB, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error"));
//		return NULL;
//	}
	//FIXME: Why use GetItem not GetUniqueItem? Maybe use GetUniqueItem and use the exceptions
	// its sets for us?
	int i = GetItem(QString::fromUtf8(TextB));
	int ii = GetItem(QString::fromUtf8(PolyB));
	if ((i == -1) || (ii == -1))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Object not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(ScCore->primaryMainWindow()->doc->Items->at(i));
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(ScCore->primaryMainWindow()->doc->Items->at(ii));
	PageItem *it = ScCore->primaryMainWindow()->doc->Items->at(i);
	ScCore->primaryMainWindow()->view->ToPathText();
	ScCore->primaryMainWindow()->doc->MoveItem(pageUnitXToDocX(x) - it->xPos(), pageUnitYToDocY(y) - it->yPos(), it);
//	if (Name != EMPTY_STRING)
//		it->setItemName(QString::fromUtf8(Name));
	if (Name != EMPTY_STRING)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyString_FromString(it->itemName().toUtf8());
}


/* 03/21/2004 - exception raised when Name doesn't exists. Doesn't crash then. (subik)
 */
PyObject *scribus_deleteobj(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(i);
	ScCore->primaryMainWindow()->doc->itemSelection_DeleteItem();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}


/* 03/21/2004 - exception raises by non existent name (subik)
 */
PyObject *scribus_textflow(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	int state = -1;

	if (!PyArg_ParseTuple(args, "es|i", "utf-8", &name, &state))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == NULL)
		return NULL;
	if (state == -1)
	{
		if (i->textFlowAroundObject())
			i->setTextFlowMode(PageItem::TextFlowDisabled);
		else
			i->setTextFlowMode(PageItem::TextFlowUsesFrameShape);
	}
	else if( state == (int) PageItem::TextFlowDisabled )
		i->setTextFlowMode(PageItem::TextFlowDisabled);
	else if( state == (int) PageItem::TextFlowUsesFrameShape )
		i->setTextFlowMode(PageItem::TextFlowUsesFrameShape);
	else if( state == (int) PageItem::TextFlowUsesBoundingBox )
		i->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
	else if( state == (int) PageItem::TextFlowUsesContourLine )
		i->setTextFlowMode(PageItem::TextFlowUsesContourLine);
	ScCore->primaryMainWindow()->view->DrawNew();
	ScCore->primaryMainWindow()->slotDocCh(true);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}


PyObject *scribus_objectexists(PyObject* /* self */, PyObject* args)
{
	char* name = const_cast<char*>("");
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
PyObject *scribus_setstyle(PyObject* /* self */, PyObject* args)
{
	char *style = const_cast<char*>("");
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &style, "utf-8", &name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if ((item->itemType() == PageItem::TextFrame) || (item->itemType() == PageItem::PathText))
	{
		// First, find the style number associated with the requested style
		// by scanning through the styles looking for the name. If
		// we can't find it, raise PyExc_Exception.
		// FIXME: Should use a more specific exception.
		bool found = false;
		uint styleid = 0;
		// We start at zero here because it's OK to match an internal name
		int docParagraphStylesCount=ScCore->primaryMainWindow()->doc->paragraphStyles().count();
		for (int i=0; i < docParagraphStylesCount; ++i)
		{
			if (ScCore->primaryMainWindow()->doc->paragraphStyles()[i].name() == QString::fromUtf8(style)) {
				found = true;
				styleid = i;
				break;
			}
		}
		if (!found) {
			// whoops, the user specified an invalid style, complain loudly.
			PyErr_SetString(NotFoundError, QObject::tr("Style not found.","python error").toLocal8Bit().constData());
			return NULL;
		}
		// for current item only
		if (ScCore->primaryMainWindow()->doc->m_Selection->count() == 0 || name != EMPTY_STRING)
		{
			// quick hack to always apply on the right frame - pv
			ScCore->primaryMainWindow()->view->Deselect(true);
			//CB I dont think we need to draw here. Its faster if we dont.
			ScCore->primaryMainWindow()->view->SelectItem(item, false);
			// Now apply the style.
			int mode = ScCore->primaryMainWindow()->doc->appMode;
			ScCore->primaryMainWindow()->doc->appMode = modeEdit;
			ScCore->primaryMainWindow()->setNewParStyle(QString::fromUtf8(style));
			ScCore->primaryMainWindow()->doc->appMode = mode;
		}
		else // for multiple selection
		{
			int mode = ScCore->primaryMainWindow()->doc->appMode;
			ScCore->primaryMainWindow()->doc->appMode = modeNormal;
			ScCore->primaryMainWindow()->doc->itemSelection_ApplyParagraphStyle(ScCore->primaryMainWindow()->doc->paragraphStyles()[styleid]);
			ScCore->primaryMainWindow()->doc->appMode = mode;
		}
	}
	else
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set style on a non-text frame.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

/*
 * Craig Ringer, 2004-09-09
 * Enumerate all known paragraph styles
 */
PyObject *scribus_getstylenames(PyObject* /* self */)
{
	PyObject *styleList;
	if(!checkHaveDocument())
		return NULL;
	styleList = PyList_New(0);
	for (int i=0; i < ScCore->primaryMainWindow()->doc->paragraphStyles().count(); ++i)
	{
		if (PyList_Append(styleList, PyString_FromString(ScCore->primaryMainWindow()->doc->paragraphStyles()[i].name().toUtf8())))
		{
			// An exception will have already been set by PyList_Append apparently.
			return NULL;
		}
	}
	return styleList;
}

PyObject *scribus_duplicateobject(PyObject * /* self */, PyObject *args)
{
	char* name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name)) {
		return NULL;
	}
	if(!checkHaveDocument()) {
		return NULL;
	}
	// Is there a special name given? Yes -> add this to selection
	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i != NULL) {
		ScCore->primaryMainWindow()->doc->m_Selection->clear();
		ScCore->primaryMainWindow()->doc->m_Selection->addItem(i);
	}
	else
		return NULL;
	// do the duplicate
	ScCore->primaryMainWindow()->slotEditCopy();
	ScCore->primaryMainWindow()->slotEditPaste();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdobjdocwarnings()
{
    QStringList s;
    s << scribus_newrect__doc__ <<scribus_newellipse__doc__ << scribus_newimage__doc__ << scribus_newtext__doc__ << scribus_newline__doc__ <<scribus_polyline__doc__ << scribus_polygon__doc__ << scribus_bezierline__doc__ <<scribus_pathtext__doc__ <<scribus_deleteobj__doc__ <<scribus_textflow__doc__ <<scribus_objectexists__doc__ <<scribus_setstyle__doc__ <<scribus_getstylenames__doc__ <<scribus_duplicateobject__doc__;
}
