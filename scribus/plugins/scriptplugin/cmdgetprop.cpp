#include <Python.h>
#include "cmdgetprop.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_getfillcolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getFillColor([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyString_FromString(i->Pcolor) : PyString_FromString("");
}

PyObject *scribus_getlinecolor(PyObject *self, PyObject* args)
{
	char *Name = "";
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getLineColor([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	it = GetUniqueItem(QString(Name));
	if ((it != NULL) && (it->HasSel) && ((it->PType == 4) || (it->PType == 8)))
	{
		for (uint b = 0; b < it->Ptext.count(); ++b)
		{
			if (it->Ptext.at(b)->cselect)
				return PyString_FromString(it->Ptext.at(b)->ccolor);
		}
	}
	else
		return PyString_FromString(it->Pcolor2);
	return PyString_FromString("");
}

PyObject *scribus_getlinewidth(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getLineWidth([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(i->Pwidth)) : PyFloat_FromDouble(0.0);
}

PyObject *scribus_getlineshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getLineShade([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	it = GetUniqueItem(QString(Name));
	if ((it != NULL) && (it->HasSel) && ((it->PType == 4) || (it->PType == 8)))
	{
		for (uint b = 0; b < it->Ptext.count(); ++b)
		{
			if (it->Ptext.at(b)->cselect)
				return PyInt_FromLong(static_cast<long>(it->Ptext.at(b)->cshade));
		}
	}
	else
		return PyInt_FromLong(static_cast<long>(it->Shade2));
	return PyInt_FromLong(0L);
}

PyObject *scribus_getlinejoin(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getLineJoin([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->PLineJoin)) : PyInt_FromLong(0L);
}

PyObject *scribus_getlineend(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getLineEnd([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->PLineEnd)) : PyInt_FromLong(0L);
}

PyObject *scribus_getlinestyle(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getLineStyle([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->PLineArt)) : PyInt_FromLong(0L);
}

PyObject *scribus_getfillshade(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getFillShade([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->Shade)) : PyInt_FromLong(0L);
}

PyObject *scribus_getcornerrad(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getCornerRadius([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->RadRect)) : PyInt_FromLong(0L);
}

PyObject *scribus_getimgscale(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getImageScale([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? Py_BuildValue("(ff)", i->LocalScX, i->LocalScY) : Py_BuildValue("(ff)", 1.0, 1.0);
}

PyObject *scribus_getimgname(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getImageFile([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyString_FromString(i->Pfile) : PyString_FromString("");
}

PyObject *scribus_getposi(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getPosition([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return (i != NULL) ? Py_BuildValue("(ff)", PointToValue(i->Xpos), PointToValue(i->Ypos)) : Py_BuildValue("(ff)", 0.0, 0.0);
}

PyObject *scribus_getsize(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getSize([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return (i != NULL) ? Py_BuildValue("(ff)", PointToValue(i->Width), PointToValue(i->Height)) : Py_BuildValue("(ff)", 0.0, 0.0);
}

PyObject *scribus_getrotation(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getRotation([objectname])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(i->Rot * -1)) : PyFloat_FromDouble(0.0);
}

PyObject *scribus_getallobj(PyObject *self, PyObject* args)
{
	PyObject *l;
	int typ = -1;
	uint counter = 0;
	uint counter2 = 0;
	if (!PyArg_ParseTuple(args, "|i", &typ))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getAllObjects([objecttype])"));
		return NULL;
	}
	if(!checkHaveDocument())
		return NULL;
	// have doc already
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
	return l;
}
