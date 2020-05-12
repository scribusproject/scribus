/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdobj.h"

#include "appmodes.h"
#include "cmdutil.h"
#include "commonstrings.h"
#include "pageitem_table.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "util_math.h"


PyObject *scribus_newrect(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");

	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error"));
//		return nullptr;
//	}
	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	int i = doc->itemAdd(PageItem::Polygon, PageItem::Rectangle,
						pageUnitXToDocX(x), pageUnitYToDocY(y),
						ValueToPoint(w), ValueToPoint(h),
						doc->itemToolPrefs().shapeLineWidth,
						doc->itemToolPrefs().shapeFillColor, doc->itemToolPrefs().shapeLineColor);
//	ScCore->primaryMainWindow()->doc->setRedrawBounding(ScCore->primaryMainWindow()->doc->Items->at(i));
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			doc->Items->at(i)->setItemName(objName);
	}
	return PyUnicode_FromString(doc->Items->at(i)->itemName().toUtf8());
}


PyObject *scribus_newellipse(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::Polygon, PageItem::Ellipse,
										pageUnitXToDocX(x),
										pageUnitYToDocY(y),
										ValueToPoint(w),
										ValueToPoint(h),
										ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth,
										ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeFillColor,
										ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineColor);
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyUnicode_FromString(ScCore->primaryMainWindow()->doc->Items->at(i)->itemName().toUtf8());
}


PyObject *scribus_newimage(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified,
									pageUnitXToDocX(x),
									pageUnitYToDocY(y),
									ValueToPoint(w),
									ValueToPoint(h),
									1, ScCore->primaryMainWindow()->doc->itemToolPrefs().imageFillColor,
									ScCore->primaryMainWindow()->doc->itemToolPrefs().imageStrokeColor);
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyUnicode_FromString(ScCore->primaryMainWindow()->doc->Items->at(i)->itemName().toUtf8());
}


PyObject *scribus_newtext(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified,
								pageUnitXToDocX(x),
								pageUnitYToDocY(y),
								ValueToPoint(w),
								ValueToPoint(h),
								ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth, CommonStrings::None,
								ScCore->primaryMainWindow()->doc->itemToolPrefs().textColor);
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyUnicode_FromString(ScCore->primaryMainWindow()->doc->Items->at(i)->itemName().toUtf8());
}

PyObject *scribus_newtable(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	int numRows, numColumns;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "ddddii|es", &x, &y, &w, &h, &numRows, &numColumns, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (numRows < 1 || numColumns < 1)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Both numRows and numColumns must be greater than 0.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::Table, PageItem::Unspecified,
								pageUnitXToDocX(x),
								pageUnitYToDocY(y),
								ValueToPoint(w),
								ValueToPoint(h),
								0,                    // Unused.
								CommonStrings::None,  // Unused.
								CommonStrings::None); // Unused.
	PageItem_Table *table = ScCore->primaryMainWindow()->doc->Items->at(i)->asTable();
	table->insertRows(0, numRows - 1);
	table->insertColumns(0, numColumns - 1);
	table->adjustTableToFrame();
	table->adjustFrameToTable();
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyUnicode_FromString(table->itemName().toUtf8());
}

PyObject *scribus_newline(PyObject* /* self */, PyObject* args)
{
	double x, y, w, h;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "dddd|es", &x, &y, &w, &h, "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	x = pageUnitXToDocX(x);
	y = pageUnitYToDocY(y);
	w = pageUnitXToDocX(w);
	h = pageUnitYToDocY(h);
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError,
//						QObject::tr("An object with the requested name already exists.",
//									"python error"));
//		return nullptr;
//	}
	int i = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::Line, PageItem::Unspecified,
							   x, y, w, h,
							   ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth,
							   ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeFillColor,
							   ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineColor);
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
	ScCore->primaryMainWindow()->view->SizeItem(it->PoLine.widthHeight().x(),
						 it->PoLine.widthHeight().y(), i, false, false, false);
	ScCore->primaryMainWindow()->view->AdjustItemSize(it);*/
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(i)->setItemName(objName);
	}
	return PyUnicode_FromString(it->itemName().toUtf8());
}


PyObject *scribus_polyline(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il;
	// FIXME: PyList_Check failing will cause the function to return nullptr w/o an exception. Separarate out the check.
	if ((!PyArg_ParseTuple(args, "O|es", &il, "utf-8", &Name)) || (!PyList_Check(il)))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	int len = PyList_Size(il);
	if (len < 4)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain at least two points (four values).","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if ((len % 2) != 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain an even number of values.","python error").toLocal8Bit().constData());
		return nullptr;
	}
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error").toLocal8Bit().constData());
//		return nullptr;
//	}
	double x, y, w, h;
	int i = 0;
	x = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, 1, 1,	ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth, ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeFillColor, ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineColor);
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
		ScCore->primaryMainWindow()->doc->moveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		ScCore->primaryMainWindow()->doc->moveItem(0, np2.y(), it);
	}
	ScCore->primaryMainWindow()->doc->sizeItem(it->PoLine.widthHeight().x(), it->PoLine.widthHeight().y(), it, false, false, false);
	ScCore->primaryMainWindow()->doc->adjustItemSize(it);
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(ic)->setItemName(objName);
	}
	return PyUnicode_FromString(it->itemName().toUtf8());
}


PyObject *scribus_polygon(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il;
	// FIXME: PyList_Check failing will cause the function to return nullptr w/o an exception. Separarate out the check.
	if ((!PyArg_ParseTuple(args, "O|es", &il, "utf-8", &Name)) || (!PyList_Check(il)))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	int len = PyList_Size(il);
	if (len < 6)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain at least three points (six values).","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if ((len % 2) != 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain an even number of values.","python error").toLocal8Bit().constData());
		return nullptr;
	}
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error").toLocal8Bit().constData());
//		return nullptr;
//	}
	double x, y, w, h;
	int i = 0;
	x = pageUnitXToDocX(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = pageUnitYToDocY(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, 1, 1,	ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth, ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeFillColor, ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineColor);
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
		ScCore->primaryMainWindow()->doc->moveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		ScCore->primaryMainWindow()->doc->moveItem(0, np2.y(), it);
	}
	ScCore->primaryMainWindow()->doc->sizeItem(it->PoLine.widthHeight().x(), it->PoLine.widthHeight().y(), it, false, false, false);
	ScCore->primaryMainWindow()->doc->adjustItemSize(it);
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(ic)->setItemName(objName);
	}
	return PyUnicode_FromString(it->itemName().toUtf8());
}

PyObject *scribus_bezierline(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PyObject *il;
	// FIXME: PyList_Check failing will cause the function to return nullptr w/o an exception. Separarate out the check.
	if ((!PyArg_ParseTuple(args, "O|es", &il, "utf-8", &Name)) || (!PyList_Check(il)))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	int len = PyList_Size(il);
	if (len < 8)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must contain at least four points (eight values).","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if ((len % 6) != 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Point list must have a multiple of six values.","python error").toLocal8Bit().constData());
		return nullptr;
	}
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error").toLocal8Bit().constData());
//		return nullptr;
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
	int ic = ScCore->primaryMainWindow()->doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, 1, 1,	ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth, ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeFillColor, ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineColor);
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
		ScCore->primaryMainWindow()->doc->moveItem(np2.x(), 0, it);
	}
	if (np2.y() < 0)
	{
		it->PoLine.translate(0, -np2.y());
		ScCore->primaryMainWindow()->doc->moveItem(0, np2.y(), it);
	}
	ScCore->primaryMainWindow()->doc->sizeItem(it->PoLine.widthHeight().x(), it->PoLine.widthHeight().y(), it, false, false, false);
	ScCore->primaryMainWindow()->doc->adjustItemSize(it);
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			ScCore->primaryMainWindow()->doc->Items->at(ic)->setItemName(objName);
	}
	return PyUnicode_FromString(it->itemName().toUtf8());
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
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
//	if (ItemExists(QString::fromUtf8(Name)))
//	{
//		PyErr_SetString(NameExistsError, QObject::tr("An object with the requested name already exists.","python error"));
//		return nullptr;
//	}
	//FIXME: Why use GetItem not GetUniqueItem? Maybe use GetUniqueItem and use the exceptions
	// its sets for us?
	PageItem *i = GetItem(QString::fromUtf8(TextB));
	PageItem *ii = GetItem(QString::fromUtf8(PolyB));
	if ((i == nullptr) || (ii == nullptr))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Object not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(i);
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(ii);
	ScCore->primaryMainWindow()->view->ToPathText();
	ScCore->primaryMainWindow()->doc->moveItem(pageUnitXToDocX(x) - i->xPos(), pageUnitYToDocY(y) - i->yPos(), i);
	if (strlen(Name) > 0)
	{
		QString objName = QString::fromUtf8(Name);
		if (!ItemExists(objName))
			i->setItemName(objName);
	}
	return PyUnicode_FromString(i->itemName().toUtf8());
}


/* 03/21/2004 - exception raised when Name doesn't exists. Doesn't crash then. (subik)
 */
PyObject *scribus_deleteobj(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == nullptr)
		return nullptr;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(i);
	ScCore->primaryMainWindow()->doc->itemSelection_DeleteItem();

	Py_RETURN_NONE;
}

PyObject *scribus_gettextflowmode(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if (!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;

	return PyLong_FromLong(static_cast<int>(item->textFlowMode()));
};

/* 03/21/2004 - exception raises by non existent name (subik)
 */
PyObject *scribus_settextflowmode(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	int state = -1;

	if (!PyArg_ParseTuple(args, "es|i", "utf-8", &name, &state))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == nullptr)
		return nullptr;
	if (state == -1)
	{
		if (i->textFlowAroundObject())
			i->setTextFlowMode(PageItem::TextFlowDisabled);
		else
			i->setTextFlowMode(PageItem::TextFlowUsesFrameShape);
	}
	else if (state == (int) PageItem::TextFlowDisabled )
		i->setTextFlowMode(PageItem::TextFlowDisabled);
	else if (state == (int) PageItem::TextFlowUsesFrameShape )
		i->setTextFlowMode(PageItem::TextFlowUsesFrameShape);
	else if (state == (int) PageItem::TextFlowUsesBoundingBox )
		i->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
	else if (state == (int) PageItem::TextFlowUsesContourLine )
		i->setTextFlowMode(PageItem::TextFlowUsesContourLine);
	ScCore->primaryMainWindow()->view->DrawNew();
	ScCore->primaryMainWindow()->slotDocCh(true);

	Py_RETURN_NONE;
}


PyObject *scribus_objectexists(PyObject* /* self */, PyObject* args)
{
	char* name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (ItemExists(QString::fromUtf8(name)))
		return PyBool_FromLong(static_cast<long>(true));
	return PyBool_FromLong(static_cast<long>(false));
}

PyObject *scribus_getcharacterstyle(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if (!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if ((item->itemType() != PageItem::TextFrame) && (item->itemType() != PageItem::PathText))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get character style of a non-text frame.", "python error").toLocal8Bit().constData());
		return NULL;
	}

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	StoryText&  itemText = item->itemText;

	int selectionLength = itemText.selectionLength();	
	if ((selectionLength > 0) || (currentDoc->appMode == modeEdit))
	{
		int cursorPos = (selectionLength > 0) ? itemText.startOfSelection() : itemText.cursorPosition();
		const CharStyle& currentStyle = itemText.charStyle(cursorPos);
		if (currentStyle.hasParent())
			return PyUnicode_FromString(currentStyle.parentStyle()->name().toUtf8());
	}
	else
	{
		const CharStyle& itemDefaultStyle = itemText.defaultStyle().charStyle();
		if (itemDefaultStyle.hasParent())
			return PyUnicode_FromString(itemDefaultStyle.parentStyle()->name().toUtf8());
	}
	Py_RETURN_NONE;
};

/*
 * Vaclav Smilauer, 2017-21-21
 * Return style name of the object (or currently selected object)
 */
PyObject *scribus_getparagraphstyle(PyObject* /* self */, PyObject* args)
{
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return NULL;
	if (!checkHaveDocument())
		return NULL;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == NULL)
		return NULL;
	if ((item->itemType() != PageItem::TextFrame) && (item->itemType() != PageItem::PathText))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot get style of a non-text frame.", "python error").toLocal8Bit().constData());
		return NULL;
	}

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	StoryText&  itemText = item->itemText;

	int selectionLength = itemText.selectionLength();	
	if ((selectionLength > 0) || (currentDoc->appMode == modeEdit))
	{
		int cursorPos = (selectionLength > 0) ? itemText.startOfSelection() : itemText.cursorPosition();
		const ParagraphStyle& currentStyle = itemText.paragraphStyle(cursorPos);
		if (currentStyle.hasParent())
			return PyUnicode_FromString(currentStyle.parentStyle()->name().toUtf8());
	}
	else
	{
		const ParagraphStyle& itemDefaultStyle = item->itemText.defaultStyle();
		if (itemDefaultStyle.hasParent())
			return PyUnicode_FromString(itemDefaultStyle.parentStyle()->name().toUtf8());
	}
	Py_RETURN_NONE;
};



/*
 * Craig Ringer, 2004-09-09
 * Apply the named style to the currently selected object.
 * pv, 2004-09-13, optionaly param objectName + "check the page" stuff
 */
PyObject *scribus_setparagraphstyle(PyObject* /* self */, PyObject* args)
{
	char *style = const_cast<char*>("");
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &style, "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if ((item->itemType() != PageItem::TextFrame) && (item->itemType() != PageItem::PathText))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set style on a non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	
	ScribusDoc*  currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	ScribusMainWindow* currentWin = ScCore->primaryMainWindow();

	// First, check if style name provided by user is available in document
	QString paraStyleName = QString::fromUtf8(style);
	if (!currentDoc->paragraphStyles().contains(paraStyleName))
	{
		// whoops, the user specified an invalid style, complain loudly.
		// FIXME: Should use a more specific exception.
		PyErr_SetString(NotFoundError, QObject::tr("Style not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	// For multiple selections or if current item has no text selected, we apply style in normal mode
	if (currentDoc->m_Selection->isMultipleSelection() || (item->itemText.selectionLength() <= 0))
	{
		int mode = currentDoc->appMode;
		currentDoc->appMode = modeNormal;
		currentDoc->itemSelection_SetNamedParagraphStyle(paraStyleName);
		currentDoc->appMode = mode;
		Py_RETURN_NONE;
	}

	// For single items which have a text selection, we apply style in edit mode
	// Store text selection as clearing object selection
	// will also clear text selection
	int selectionStart =  item->itemText.startOfSelection();
	int selectionLength = item->itemText.selectionLength();
	// Quick hack to always apply on the right frame - pv
	currentView->deselectItems(true);
	//CB I dont think we need to draw here. Its faster if we dont.
	currentView->selectItem(item, false);
	// Restore text selection if necessary
	if (selectionStart >= 0)
	{
		item->itemText.deselectAll();
		item->itemText.select(selectionStart, selectionLength);
		item->HasSel = true;
	}
	// Now apply the style.
	int mode = currentDoc->appMode;
	currentDoc->appMode = modeEdit;
	currentWin->setNewParStyle(paraStyleName);
	currentDoc->appMode = mode;

	Py_RETURN_NONE;
}

/*
* Jean Ghali, 2017-07-18
* Apply the named character style to the currently selected object.
* pv, 2004-09-13, optionaly param objectName + "check the page" stuff
*/
PyObject *scribus_setcharstyle(PyObject* /* self */, PyObject* args)
{
	char *style = const_cast<char*>("");
	char *name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &style, "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name));
	if (item == nullptr)
		return nullptr;
	if ((item->itemType() != PageItem::TextFrame) && (item->itemType() != PageItem::PathText))
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot set character style on a non-text frame.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	
	ScribusDoc*  currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	ScribusMainWindow* currentWin = ScCore->primaryMainWindow();

	// First, check if style name provided by user is available in document
	QString charStyleName = QString::fromUtf8(style);
	if (!currentDoc->charStyles().contains(charStyleName))
	{
		// whoops, the user specified an invalid style, complain loudly.
		// FIXME: Should use a more specific exception.
		PyErr_SetString(NotFoundError, QObject::tr("Character style not found.", "python error").toLocal8Bit().constData());
		return nullptr;
	}

	// For multiple selections or if current item has no text selected, we apply style in normal mode
	if (currentDoc->m_Selection->isMultipleSelection() || (item->itemText.selectionLength() <= 0))
	{
		int mode = currentDoc->appMode;
		currentDoc->appMode = modeNormal;
		currentDoc->itemSelection_SetNamedCharStyle(charStyleName);
		currentDoc->appMode = mode;
		Py_RETURN_NONE;
	}

	// For single items which have a text selection, we apply style in edit mode
	// Store text selection as clearing object selection
	// will also clear text selection
	int selectionStart =  item->itemText.startOfSelection();
	int selectionLength = item->itemText.selectionLength();

	// Quick hack to always apply on the right frame - pv
	currentView->deselectItems(true);
	//CB I dont think we need to draw here. Its faster if we dont.
	currentView->selectItem(item, false);
	// Restore text selection if necessary
	if (selectionStart >= 0)
	{
		item->itemText.deselectAll();
		item->itemText.select(selectionStart, selectionLength);
		item->HasSel = true;
	}	
	// Now apply the style.
	int mode = ScCore->primaryMainWindow()->doc->appMode;
	currentDoc->appMode = modeEdit;
	currentWin->setNewCharStyle(charStyleName);
	currentDoc->appMode = mode;

	Py_RETURN_NONE;
}

PyObject *scribus_duplicateobject(PyObject * /* self */, PyObject *args)
{
	char* name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	// Is there a special name given? Yes -> add this to selection
	ScribusMainWindow* currentWin = ScCore->primaryMainWindow();
	ScribusDoc* currentDoc = currentWin->doc;

	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == nullptr)
		return nullptr;
	currentDoc->m_Selection->clear();
	currentDoc->m_Selection->addItem(i);

	// do the duplicate
	currentWin->slotEditCopy();
	currentWin->slotEditPaste();

	return PyUnicode_FromString(currentDoc->m_Selection->itemAt(0)->itemName().toUtf8());
}

PyObject *scribus_copyobject(PyObject * /* self */, PyObject *args)
{
	char* name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	// Is there a special name given? Yes -> add this to selection
	ScribusMainWindow* currentWin = ScCore->primaryMainWindow();
	ScribusDoc* currentDoc = currentWin->doc;

	PageItem *i = GetUniqueItem(QString::fromUtf8(name));
	if (i == nullptr)
		return nullptr;
	currentDoc->m_Selection->clear();
	currentDoc->m_Selection->addItem(i);

	// do the copy
	currentWin->slotEditCopy();

	Py_RETURN_NONE;
}

PyObject *scribus_pasteobject(PyObject * /* self */, PyObject *args)
{
	char* name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	// do the paste
	ScCore->primaryMainWindow()->slotEditPaste();

	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdobjdocwarnings()
{
	QStringList s;
	s << scribus_bezierline__doc__
	  << scribus_copyobject__doc__
	  << scribus_deleteobj__doc__
	  << scribus_duplicateobject__doc__
	  << scribus_getcharacterstyle__doc__
	  << scribus_getparagraphstyle__doc__
	  << scribus_getstyle__doc__
	  << scribus_newellipse__doc__
	  << scribus_newimage__doc__
	  << scribus_newline__doc__
	  << scribus_newrect__doc__ 
	  << scribus_newtable__doc__
	  << scribus_newtext__doc__
	  << scribus_objectexists__doc__
	  << scribus_pasteobject__doc__
	  << scribus_pathtext__doc__
	  << scribus_polygon__doc__
	  << scribus_polyline__doc__
	  << scribus_setcharstyle__doc__
	  << scribus_setparagraphstyle__doc__
	  << scribus_setstyle__doc__
	  << scribus_settextflowmode__doc__
	  << scribus_textflowmode__doc__;
}
