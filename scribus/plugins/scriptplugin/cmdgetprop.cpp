#include <Python.h>
#include "cmdgetprop.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_getfillcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = GetItem(QString(Name));
	return i != -1 ? PyString_FromString(Carrier->doc->ActPage->Items.at(i)->Pcolor) : PyString_FromString("");
}

PyObject *scribus_getlinecolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = Carrier->doc->ActPage->Items.at(i);
		if ((it->HasSel) && ((it->PType == 4) || (it->PType == 8)))
			{
			for (uint b = 0; b < it->Ptext.count(); ++b)
				{
				if (it->Ptext.at(b)->cselect)
					return PyString_FromString(it->Ptext.at(b)->ccolor);
				}
			}
		else
			return PyString_FromString(it->Pcolor2);
		}
	return PyString_FromString("");
}

PyObject *scribus_getlinewidth(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyFloat_FromDouble(0.0);
	i = GetItem(QString(Name));
	return i != -1 ? PyFloat_FromDouble(static_cast<double>(Carrier->doc->ActPage->Items.at(i)->Pwidth)) :
									 PyFloat_FromDouble(0.0);
}

PyObject *scribus_getlineshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	i = GetItem(QString(Name));
	if (i != -1)
		{
		it = Carrier->doc->ActPage->Items.at(i);
		if ((it->HasSel) && ((it->PType == 4) || (it->PType == 8)))
			{
			for (uint b = 0; b < it->Ptext.count(); ++b)
				{
				if (it->Ptext.at(b)->cselect)
					return PyInt_FromLong(static_cast<long>(it->Ptext.at(b)->cshade));
				}
			}
		else
			return PyInt_FromLong(static_cast<long>(it->Shade2));
		}
	return PyInt_FromLong(0L);
}

PyObject *scribus_getlinejoin(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	i = GetItem(QString(Name));
	return i != 1 ? PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->Items.at(i)->PLineJoin)) :
									PyInt_FromLong(0L);
}

PyObject *scribus_getlineend(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	i = GetItem(QString(Name));
	return i != -1 ? PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->Items.at(i)->PLineEnd)) :
									 PyInt_FromLong(0L);
}

PyObject *scribus_getlinestyle(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	i = GetItem(QString(Name));
	return i != -1 ? PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->Items.at(i)->PLineArt)) :
									 PyInt_FromLong(0L);
}

PyObject *scribus_getfillshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	i = GetItem(QString(Name));
	return i != -1 ? PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->Items.at(i)->Shade)) :
									 PyInt_FromLong(0L);
}

PyObject *scribus_getcornerrad(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyInt_FromLong(0L);
	i = GetItem(QString(Name));
	return i != -1 ? PyInt_FromLong(static_cast<long>(Carrier->doc->ActPage->Items.at(i)->RadRect)) :
									 PyInt_FromLong(0L);
}

PyObject *scribus_getimgscale(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return Py_BuildValue("(ff)", 1.0, 1.0);
	i = GetItem(QString(Name));
	if (i != -1)
		{
		PageItem *b = Carrier->doc->ActPage->Items.at(i);
		return Py_BuildValue("(ff)", b->LocalScX, b->LocalScY);
		}
	else
		return Py_BuildValue("(ff)", 1.0, 1.0);
}

PyObject *scribus_getimgname(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = GetItem(QString(Name));
	return i != -1 ? PyString_FromString(Carrier->doc->ActPage->Items.at(i)->Pfile) : PyString_FromString("");
}

PyObject *scribus_getposi(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return Py_BuildValue("(ff)", 0.0, 0.0);
	i = GetItem(QString(Name));
	if (i != -1)
		{
		PageItem *it = Carrier->doc->ActPage->Items.at(i);
		return Py_BuildValue("(ff)", PointToValue(it->Xpos), PointToValue(it->Ypos));
		}
	else
		return Py_BuildValue("(ff)", 0.0, 0.0);
}

PyObject *scribus_getsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return Py_BuildValue("(ff)", 0.0, 0.0);
	i = GetItem(QString(Name));
	if (i != -1)
		{
		PageItem *it = Carrier->doc->ActPage->Items.at(i);
		return Py_BuildValue("(ff)", PointToValue(it->Width), PointToValue(it->Height));
		}
	else
		return Py_BuildValue("(ff)", 0.0, 0.0);
}

PyObject *scribus_getrotation(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyFloat_FromDouble(0.0);
	i = GetItem(QString(Name));
	return i != -1 ? PyFloat_FromDouble(static_cast<double>(Carrier->doc->ActPage->Items.at(i)->Rot * -1)) :
									 PyFloat_FromDouble(0.0);
}

PyObject *scribus_getallobj(PyObject *self, PyObject* args)
{
	PyObject *l;
	int typ = -1;
	uint counter = 0;
	uint counter2 = 0;
	if (!PyArg_ParseTuple(args, "|i", &typ))
		return NULL;
	if (Carrier->HaveDoc)
		{
		if (typ != -1)
		{
			for (uint lam2 = 0; lam2 < Carrier->doc->ActPage->Items.count(); ++lam2)
			{
				if (Carrier->doc->ActPage->Items.at(lam2)->PType == typ)
					counter++;
			}
		}
		else
			counter = Carrier->doc->ActPage->Items.count();
		l = PyList_New(counter);
		for (uint lam=0; lam < Carrier->doc->ActPage->Items.count(); ++lam)
			{
			if (typ != -1)
			{
				if (Carrier->doc->ActPage->Items.at(lam)->PType == typ)
				{
					PyList_SetItem(l, counter2, PyString_FromString(Carrier->doc->ActPage->Items.at(lam)->AnName));
					counter2++;
				}
			}
			else
				PyList_SetItem(l, lam, PyString_FromString(Carrier->doc->ActPage->Items.at(lam)->AnName));
			}
		}
	else
		l = PyList_New(0);
	return l;
}

