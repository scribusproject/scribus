#include "cmdvar.h"
#include "cmdutil.h"
#include "cmdgetprop.h"

PyObject *scribus_getfillcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	int i;
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	i = GetItem(QString(Name));
	if (i != -1)
		return PyString_FromString(doc->ActPage->Items.at(i)->Pcolor);
	else
		return PyString_FromString("");
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
		it = doc->ActPage->Items.at(i);
		if ((it->HasSel) && ((it->PType == 4) || (it->PType == 8)))
			{
			for (uint b = 0; b < it->Ptext.count(); b++)
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
	if (i != -1)
		return PyFloat_FromDouble(static_cast<double>(doc->ActPage->Items.at(i)->Pwidth));
	else
		return PyFloat_FromDouble(0.0);
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
		it = doc->ActPage->Items.at(i);
		if ((it->HasSel) && ((it->PType == 4) || (it->PType == 8)))
			{
			for (uint b = 0; b < it->Ptext.count(); b++)
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
	if (i != -1)
		return PyInt_FromLong(static_cast<long>(doc->ActPage->Items.at(i)->PLineJoin));
	else
		return PyInt_FromLong(0L);
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
	if (i != -1)
		return PyInt_FromLong(static_cast<long>(doc->ActPage->Items.at(i)->PLineEnd));
	else
		return PyInt_FromLong(0L);
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
	if (i != -1)
		return PyInt_FromLong(static_cast<long>(doc->ActPage->Items.at(i)->PLineArt));
	else
		return PyInt_FromLong(0L);
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
	if (i != -1)
		return PyInt_FromLong(static_cast<long>(doc->ActPage->Items.at(i)->Shade));
	else
		return PyInt_FromLong(0L);
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
	if (i != -1)
		return PyInt_FromLong(static_cast<long>(doc->ActPage->Items.at(i)->RadRect));
	else
		return PyInt_FromLong(0L);
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
		PageItem *b = doc->ActPage->Items.at(i);
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
	if (i != -1)
		return PyString_FromString(doc->ActPage->Items.at(i)->Pfile);
	else
		return PyString_FromString("");
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
		PageItem *it = doc->ActPage->Items.at(i);
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
		PageItem *it = doc->ActPage->Items.at(i);
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
	if (i != -1)
		return PyFloat_FromDouble(static_cast<double>(doc->ActPage->Items.at(i)->Rot * -1));
	else
		return PyFloat_FromDouble(0.0);
}

PyObject *scribus_getallobj(PyObject *self, PyObject* args)
{
	PyObject *l;
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	if (Carrier->HaveDoc)
		{
		l = PyList_New(doc->ActPage->Items.count());
		for (uint lam=0; lam < doc->ActPage->Items.count(); lam++)
			{
			PyList_SetItem(l, lam, PyString_FromString(doc->ActPage->Items.at(lam)->AnName));
			}
		}
	else
		l = PyList_New(0);
	return l;
}

