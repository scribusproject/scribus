/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdgetprop.h"
#include "cmdutil.h"

PyObject *scribus_getfillcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyString_FromString(i->fillColor().utf8()) : NULL;
}

PyObject *scribus_getlinecolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if ((it->HasSel) && ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText)))
	{
		for (uint b = 0; b < it->itemText.count(); ++b)
		{
			if (it->itemText.at(b)->cselect)
				return PyString_FromString(it->itemText.at(b)->ccolor.utf8());
		}
	}
	else
		return PyString_FromString(it->lineColor().utf8());
	PyErr_SetString(NotFoundError, QObject::tr("Color not found - python error", "python error"));
	return NULL;
}

PyObject *scribus_getlinewidth(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(i->lineWidth())) : NULL;
}

PyObject *scribus_getlineshade(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PageItem *it;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	it = GetUniqueItem(QString::fromUtf8(Name));
	if (it == NULL)
		return NULL;
	if ((it->HasSel) && ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText)))
	{
		for (uint b = 0; b < it->itemText.count(); ++b)
		{
			if (it->itemText.at(b)->cselect)
				return PyInt_FromLong(static_cast<long>(it->itemText.at(b)->cshade));
		}
	}
	else
		return PyInt_FromLong(static_cast<long>(it->lineShade()));
	return PyInt_FromLong(0L);
}

PyObject *scribus_getlinejoin(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->PLineJoin)) : NULL;
}

PyObject *scribus_getlineend(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->PLineEnd)) : NULL;
}

PyObject *scribus_getlinestyle(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->PLineArt)) : NULL;
}

PyObject *scribus_getfillshade(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->fillShade())) : NULL;
}

PyObject *scribus_getcornerrad(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->cornerRadius())) : NULL;
}

PyObject *scribus_getimgscale(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? Py_BuildValue("(ff)", i->imageXScale(), i->imageYScale()) : NULL;
}

PyObject *scribus_getimgname(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyString_FromString(i->Pfile.utf8()) : NULL;
}

PyObject *scribus_getposi(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (!i)
		return NULL;
	return Py_BuildValue("(ff)", docUnitXToPageX(i->xPos()),
								 docUnitYToPageY(i->yPos()));
}

PyObject *scribus_getsize(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return (i != NULL) ? Py_BuildValue("(ff)", PointToValue(i->width()), PointToValue(i->height())) : NULL;
}

PyObject *scribus_getrotation(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(i->rotation() * -1)) : NULL;
}

PyObject *scribus_getallobj(PyObject* /* self */, PyObject* args)
{
	PyObject *l;
	int typ = -1;
	uint counter = 0;
	uint counter2 = 0;
	uint pageNr = ScMW->doc->currentPageNumber();
	if (!PyArg_ParseTuple(args, "|i", &typ))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	// have doc already
	if (typ != -1)
	{
		for (uint lam2 = 0; lam2 < ScMW->doc->Items->count(); ++lam2)
		{
			if ((ScMW->doc->Items->at(lam2)->itemType() == typ) && (pageNr == static_cast<uint>(ScMW->doc->Items->at(lam2)->OwnPage)))
				counter++;
		}
	}
	else
	{
		for (uint lam2 = 0; lam2 < ScMW->doc->Items->count(); ++lam2)
		{
			if (pageNr == static_cast<uint>(ScMW->doc->Items->at(lam2)->OwnPage))
				counter++;
		}
	}

	l = PyList_New(counter);
	for (uint lam=0; lam < ScMW->doc->Items->count(); ++lam)
	{
		if  (pageNr == static_cast<uint>(ScMW->doc->Items->at(lam)->OwnPage))
		{
			if (typ != -1)
			{
				if (ScMW->doc->Items->at(lam)->itemType() == typ)
				{
					PyList_SetItem(l, counter2, PyString_FromString(ScMW->doc->Items->at(lam)->itemName().utf8()));
					counter2++;
				}
			}
			else
			{
				PyList_SetItem(l, counter2, PyString_FromString(ScMW->doc->Items->at(lam)->itemName().utf8()));
				counter2++;
			}
		}
	}
	return l;
}
