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
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		{ 
		Carrier->doc->ActPage->AdjItemGradient(Carrier->doc->ActPage->Items.at(i), typ, QString(Color1), shade1, QString(Color2), shade2);
		Carrier->doc->ActPage->RefreshItem(Carrier->doc->ActPage->Items.at(i));
		}
	return Py_None;
}

PyObject *scribus_setfillcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		Carrier->doc->ActPage->Items.at(i)->Pcolor = QString(Color);
	return Py_None;
}

PyObject *scribus_setlinecolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	PageItem *it;
	if (i != -1)
		{
		it = Carrier->doc->ActPage->Items.at(i);
		it->Pcolor2 = QString(Color);
		}
	return Py_None;
}

PyObject *scribus_setlinewidth(PyObject *self, PyObject* args)
{
	char *Name = "";
	double w;
	if (!PyArg_ParseTuple(args, "d|s", &w, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || ((w < 0.0) || (w > 12.0)))
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		Carrier->doc->ActPage->Items.at(i)->Pwidth = w;
	return Py_None;
}

PyObject *scribus_setlineshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || ((w < 0) || (w > 100)))
		return Py_None;
	int i = GetItem(QString(Name));
	PageItem *it;	
	if (i != -1)
		{
		it = Carrier->doc->ActPage->Items.at(i);
		it->Shade2 = w;
		}
	return Py_None;
}

PyObject *scribus_setfillshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || ((w < 0) || (w > 100)))
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		Carrier->doc->ActPage->Items.at(i)->Shade = w;
	return Py_None;
}

PyObject *scribus_setlinejoin(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		Carrier->doc->ActPage->Items.at(i)->PLineJoin = Qt::PenJoinStyle(w);
	return Py_None;
}

PyObject *scribus_setlineend(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		Carrier->doc->ActPage->Items.at(i)->PLineEnd = Qt::PenCapStyle(w);
	return Py_None;
}

PyObject *scribus_setlinestyle(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		Carrier->doc->ActPage->Items.at(i)->PLineArt = Qt::PenStyle(w);
	return Py_None;
}

PyObject *scribus_setcornerrad(PyObject *self, PyObject* args)
{
	char *Name = "";
	int w;
	if (!PyArg_ParseTuple(args, "i|s", &w, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (w < 0))
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		{
		PageItem *b = Carrier->doc->ActPage->Items.at(i);
		if ((b->PType == 2) || (b->PType == 3) || (b->PType == 4))
			{
			b->RadRect = w;
			if (w > 0)
				Carrier->doc->ActPage->SetFrameRound(b);
			else
				Carrier->doc->ActPage->SetRectFrame(b);
			}
		}
	return Py_None;
}

PyObject *scribus_setmultiline(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Color;
	if (!PyArg_ParseTuple(args, "s|s", &Color, &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	int i = GetItem(QString(Name));
	if (i != -1)
		{
		PageItem *b = Carrier->doc->ActPage->Items.at(i);
		if (Carrier->doc->MLineStyles.contains(QString(Color)))
			b->NamedLStyle = QString(Color);
		}
	return Py_None;
}

