#include "cmdsetprop.h"
#include "cmdutil.h"

PyObject *scribus_setgradfill(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color1;
	char *Color2;
	int typ, shade1, shade2;
	if (!PyArg_ParseTuple(args, "isisi|s", &typ, &Color1, &shade1, &Color2, &shade2, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *b = GetUniqueItem(QString(Name));
	if (b == NULL)
		return NULL;
	QColor tmp;
	b->fill_gradient.clearStops();
	QString c1 = QString(Color1);
	QString c2 = QString(Color2);
	b->SetFarbe(&tmp, c1, shade1);
	b->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, c1, shade1);
	b->SetFarbe(&tmp, c2, shade2);
	b->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, c2, shade2);
	b->GrType = typ;
	Carrier->view->updateGradientVectors(b);
	Carrier->view->RefreshItem(b);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfillcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->Pcolor = QString(Color);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinecolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	it->Pcolor2 = QString(Color);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinewidth(PyObject *self, PyObject* args)
{
	char *Name = "";
	double w;
	if (!PyArg_ParseTuple(args, "d|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0.0) || (w > 12.0))
	{
		PyErr_SetString(PyExc_ValueError, QString("Line width out of bounds, must be 0 <= line_width <= 12"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->Pwidth = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlineshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0) || (w > 100))
	{
		PyErr_SetString(PyExc_ValueError, QString("Line shade out of bounds, must be 0 <= shade <= 100"));
		return NULL;
	}
	PageItem *it = GetUniqueItem(QString(Name));
	if (it == NULL)
		return NULL;
	it->Shade2 = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfillshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0) || (w > 100))
	{
		PyErr_SetString(PyExc_ValueError, QString("Fill shade out of bounds, must be 0 <= shade <= 100"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->Shade = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinejoin(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->PLineJoin = Qt::PenJoinStyle(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlineend(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->PLineEnd = Qt::PenCapStyle(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinestyle(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->PLineArt = Qt::PenStyle(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setcornerrad(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (w < 0)
	{
		PyErr_SetString(PyExc_ValueError, QString("Corner radius must be a positive number."));
		return NULL;
	}
	PageItem *b = GetUniqueItem(QString(Name));
	if (b == NULL)
		return NULL;
	if ((b->PType == 2) || (b->PType == 3) || (b->PType == 4))
	{
		if ((b->PType == 2) || (b->PType == 3) || (b->PType == 4))
		{
			b->RadRect = w;
			if (w > 0)
				Carrier->view->SetFrameRound(b);
		}
		else
				Carrier->view->SetRectFrame(b);
	}
	else
			Carrier->view->SetRectFrame(b);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setmultiline(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *b = GetUniqueItem(QString(Name));
	if (b == NULL)
		return NULL;
	if (!Carrier->doc->MLineStyles.contains(QString(Color)))
	{
		PyErr_SetString(ScribusException, QString("Color not found"));
		return NULL;
	}
	b->NamedLStyle = QString(Color);
	Py_INCREF(Py_None);
	return Py_None;
}
