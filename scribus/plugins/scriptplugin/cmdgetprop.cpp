/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdgetprop.h"
#include "cmdutil.h"
#include "scribuscore.h"

/* getObjectType(name) */
PyObject *scribus_getobjecttype(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PageItem *item = NULL;
	QString result = "";

	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;

	if(!checkHaveDocument())
		return NULL;

	item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == NULL)
		return NULL;

	if (item->itemType() == PageItem::TextFrame) {
		result = "TextFrame";
	} else if (item->itemType() == PageItem::PathText) {
		result = "PathText";
	} else if (item->itemType() == PageItem::ImageFrame) {
		result = "ImageFrame";
	} else if (item->itemType() == PageItem::Line) {
		result = "Line";
	} else if (item->itemType() == PageItem::Polygon) {
		result = "Polygon";
	} else if (item->itemType() == PageItem::PolyLine) {
		result = "Polyline";
	} else if (item->itemType() == PageItem::LatexFrame) {
		result = "LatexFrame";
	} else if (item->itemType() == PageItem::Multiple) {
		result = "Multiple";
	}

	return PyString_FromString(result.toUtf8());
}

PyObject *scribus_getfillcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyString_FromString(i->fillColor().toUtf8()) : NULL;
}

PyObject *scribus_getfilltrans(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(1.0 - i->fillTransparency())) : NULL;
}

PyObject *scribus_getfillblend(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->fillBlendmode())) : NULL;
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
		for (int b = 0; b < it->itemText.length(); ++b)
		{
			if (it->itemText.selected(b))
				return PyString_FromString(it->itemText.charStyle(b).fillColor().toUtf8());
		}
	}
	else
		return PyString_FromString(it->lineColor().toUtf8());
	PyErr_SetString(NotFoundError, QObject::tr("Color not found - python error", "python error").toLocal8Bit().constData());
	return NULL;
}

PyObject *scribus_getlinetrans(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyFloat_FromDouble(static_cast<double>(1.0 - i->lineTransparency())) : NULL;
}

PyObject *scribus_getlineblend(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyInt_FromLong(static_cast<long>(i->lineBlendmode())) : NULL;
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
		for (int b = 0; b < it->itemText.length(); ++b)
		{
			if (it->itemText.selected(b))
				return PyInt_FromLong(static_cast<long>(it->itemText.charStyle(b).fillShade()));
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

PyObject *scribus_getlinecap(PyObject* /* self */, PyObject* args)
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
	return i != NULL ? Py_BuildValue("(ff)", i->imageXScale() / 72.0 * i->pixm.imgInfo.xres, i->imageYScale() / 72.0 * i->pixm.imgInfo.yres) : NULL;
}

PyObject *scribus_getimgname(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	return i != NULL ? PyString_FromString(i->Pfile.toUtf8()) : NULL;
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
	uint pageNr = ScCore->primaryMainWindow()->doc->currentPageNumber();
	if (!PyArg_ParseTuple(args, "|i", &typ))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	// have doc already
	if (typ != -1)
	{
		for (int lam2 = 0; lam2 < ScCore->primaryMainWindow()->doc->Items->count(); ++lam2)
		{
			if ((ScCore->primaryMainWindow()->doc->Items->at(lam2)->itemType() == typ) && (pageNr == static_cast<uint>(ScCore->primaryMainWindow()->doc->Items->at(lam2)->OwnPage)))
				counter++;
		}
	}
	else
	{
		for (int lam2 = 0; lam2 < ScCore->primaryMainWindow()->doc->Items->count(); ++lam2)
		{
			if (pageNr == static_cast<uint>(ScCore->primaryMainWindow()->doc->Items->at(lam2)->OwnPage))
				counter++;
		}
	}

	l = PyList_New(counter);
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Items->count(); ++lam)
	{
		if  (pageNr == static_cast<uint>(ScCore->primaryMainWindow()->doc->Items->at(lam)->OwnPage))
		{
			if (typ != -1)
			{
				if (ScCore->primaryMainWindow()->doc->Items->at(lam)->itemType() == typ)
				{
					PyList_SetItem(l, counter2, PyString_FromString(ScCore->primaryMainWindow()->doc->Items->at(lam)->itemName().toUtf8()));
					counter2++;
				}
			}
			else
			{
				PyList_SetItem(l, counter2, PyString_FromString(ScCore->primaryMainWindow()->doc->Items->at(lam)->itemName().toUtf8()));
				counter2++;
			}
		}
	}
	return l;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdgetpropdocwarnings()
{
    QStringList s;
    s << scribus_getobjecttype__doc__ << scribus_getfillcolor__doc__ 
	  << scribus_getfilltrans__doc__ << scribus_getfillblend__doc__ 
	  << scribus_getlinecolor__doc__ << scribus_getlinetrans__doc__ 
	  << scribus_getlineblend__doc__ << scribus_getlinewidth__doc__ 
	  << scribus_getlineshade__doc__ << scribus_getlinejoin__doc__ 
	  << scribus_getlinecap__doc__ << scribus_getlinestyle__doc__ 
	  << scribus_getfillshade__doc__ << scribus_getcornerrad__doc__ 
	  << scribus_getimgscale__doc__ << scribus_getimgname__doc__ 
	  << scribus_getposi__doc__ << scribus_getsize__doc__ 
	  << scribus_getrotation__doc__ << scribus_getallobj__doc__;
}
