#include <Python.h>
#include "cmdsetprop.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_setgradfill(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color1;
	char *Color2;
	int typ, shade1, shade2;
	if (!PyArg_ParseTuple(args, "isisi|s", &typ, &Color1, &shade1, &Color2, &shade2, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setGradientFill(type, color1, shade1, color2, shade2 [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *b = GetUniqueItem(QString(Name));
	if (b != NULL)
	{
		QColor tmp;
		b->fill_gradient.clearStops();
		QString c1 = QString(Color1);
		QString c2 = QString(Color2);
		b->SetFarbe(&tmp, c1, shade1);
		b->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, c1, shade1);
		b->SetFarbe(&tmp, c2, shade2);
		b->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, c2, shade2);
		b->GrType = typ;
		b->OwnPage->updateGradientVectors(b);
		b->OwnPage->RefreshItem(b);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfillcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setFillColor(color [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->Pcolor = QString(Color);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinecolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setLineColor(color [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *it = GetUniqueItem(QString(Name));
	if (it != NULL)
		it->Pcolor2 = QString(Color);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinewidth(PyObject *self, PyObject* args)
{
	char *Name = "";
	double w;
	if (!PyArg_ParseTuple(args, "d|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setLineWidth(width [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0.0) || (w > 12.0))
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->Pwidth = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlineshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setLineShade(shade [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0) || (w > 100))
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	PageItem *it = GetUniqueItem(QString(Name));
	if (it != NULL)
		it->Shade2 = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setfillshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setFillShade(shade [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	if ((w < 0) || (w > 100))
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->Shade = w;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinejoin(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setLineJoin(jointype [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->PLineJoin = Qt::PenJoinStyle(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlineend(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setLineEnd(endtype [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->PLineEnd = Qt::PenCapStyle(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setlinestyle(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setLineStyle(style [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
		i->PLineArt = Qt::PenStyle(w);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setcornerrad(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setCornerRadius(radius [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	if (w < 0)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	PageItem *b = GetUniqueItem(QString(Name));
	if (b != NULL)
	{
		if ((b->PType == 2) || (b->PType == 3) || (b->PType == 4))
		{
			b->RadRect = w;
			if (w > 0)
				b->OwnPage->SetFrameRound(b);
		}
		else
				b->OwnPage->SetRectFrame(b);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_setmultiline(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setMultiLine(color [, objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *b = GetUniqueItem(QString(Name));
	if (b != NULL)
		if (Carrier->doc->MLineStyles.contains(QString(Color)))
			b->NamedLStyle = QString(Color);
	Py_INCREF(Py_None);
	return Py_None;
}
