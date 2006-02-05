/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdsetprop.h"
#include "cmdutil.h"

PyObject *scribus_setgradfill(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Color1;
	char *Color2;
	int typ, shade1, shade2;
	if (!PyArg_ParseTuple(args, "iesiesi|es", &typ, "utf-8", &Color1, &shade1, "utf-8", &Color2, &shade2, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *currItem = GetUniqueItem(QString::fromUtf8(Name));
	if (currItem == NULL)
		return NULL;
	QColor tmp;
	currItem->fill_gradient.clearStops();
	QString c1 = QString::fromUtf8(Color1);
	QString c2 = QString::fromUtf8(Color2);
	currItem->SetFarbe(&tmp, c1, shade1);
	currItem->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, c1, shade1);
	currItem->SetFarbe(&tmp, c2, shade2);
	currItem->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, c2, shade2);
	currItem->GrType = typ;
	//ScMW->view->updateGradientVectors(currItem);
	currItem->updateGradientVectors();
	ScMW->view->RefreshItem(currItem);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfillcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Color;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Color, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	i->setFillColor(QString::fromUtf8(Color));
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinecolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Color;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Color, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	it->setLineColor(QString::fromUtf8(Color));
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinewidth(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double w;
	if (!PyArg_ParseTuple(args, "d|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0.0) || (w > 12.0))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Line width out of bounds, must be 0 <= line_width <= 12.","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	i->Pwidth = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlineshade(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0) || (w > 100))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Line shade out of bounds, must be 0 <= shade <= 100.","python error"));
		return NULL;
	}
	PageItem *it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	it->setLineShade(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfillshade(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0) || (w > 100))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Fill shade out of bounds, must be 0 <= shade <= 100.","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	i->setFillShade(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinejoin(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	i->PLineJoin = Qt::PenJoinStyle(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlineend(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	i->PLineEnd = Qt::PenCapStyle(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinestyle(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	i->PLineArt = Qt::PenStyle(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setcornerrad(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int w;
	if (!PyArg_ParseTuple(args, "i|es", &w, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (w < 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Corner radius must be a positive number.","python error"));
		return NULL;
	}
	PageItem *currItem = GetUniqueItem(QString::fromUtf8(Name));
	if (currItem == NULL)
		return NULL;
	// apply rounding
	currItem->setCornerRadius(w);
	currItem->SetFrameRound();
	ScMW->view->setRedrawBounding(currItem);
	ScMW->view->SetFrameRounded();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setmultiline(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Style = NULL;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Style, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *currItem = GetUniqueItem(QString::fromUtf8(Name));
	if (currItem == NULL)
		return NULL;
	if (!ScMW->doc->MLineStyles.contains(QString::fromUtf8(Style)))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Line style not found.","python error"));
		return NULL;
	}
	currItem->NamedLStyle = QString::fromUtf8(Style);
	Py_INCREF(Py_None);
	return Py_None;
}
