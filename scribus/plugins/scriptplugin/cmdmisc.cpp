#include "cmdmisc.h"
#include "cmdutil.h"

extern QPixmap FontSample(QString da, int s, QString ts, QColor back);

PyObject *scribus_setredraw(PyObject *self, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	Carrier->doc->DoDrawing = static_cast<bool>(e);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_fontnames(PyObject *self)
{
	int cc2 = 0;
	SCFontsIterator it2(Carrier->Prefs.AvailFonts);
	for ( ; it2.current() ; ++it2)
	{
		if (it2.current()->UseFont)
			cc2++;
	}
	PyObject *l = PyList_New(cc2);
	SCFontsIterator it(Carrier->Prefs.AvailFonts);
	int cc = 0;
	for ( ; it.current() ; ++it)
	{
		if (it.current()->UseFont)
		{
			PyList_SetItem(l, cc, PyString_FromString(it.currentKey()));
			cc++;
		}
	}
	return l;
}

PyObject *scribus_xfontnames(PyObject *self)
{
	PyObject *l = PyList_New(Carrier->Prefs.AvailFonts.count());
	SCFontsIterator it(Carrier->Prefs.AvailFonts);
	int cc = 0;
	PyObject *row;
	for ( ; it.current() ; ++it)
	{
		row = Py_BuildValue((char*)"(sssiis)",
		                    it.currentKey().ascii(),// it.currentKey().length(),
		                    it.current()->Family.ascii(),
		                    it.current()->RealName().ascii(),
		                    it.current()->Subset,
		                    it.current()->EmbedPS,
		                    it.current()->Datei.ascii()//, it.current()->Datei.length()
		                   );
		PyList_SetItem(l, cc, row);//PyString_FromString(it.currentKey()));
		cc++;
	} // for
	return l;
}

PyObject *scribus_renderfont(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *FileName = "";
	char *Sample = "";
	int Size;
	bool ret = false;
	if (!PyArg_ParseTuple(args, "sssi", &Name, &FileName, &Sample, &Size))
		return NULL;
	if (!Carrier->Prefs.AvailFonts.find(QString(Name)))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Font not found","python error"));
		return NULL;
	}
	QString ts = QString(Sample);
	if (ts == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't render an empty sample","python error"));
		return NULL;
	}
	if (QString(FileName) == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't save to a blank filename","python error"));
		return NULL;
	}
	QString da = Carrier->Prefs.AvailFonts[QString(Name)]->Datei;
	QPixmap pm = FontSample(da, Size, ts, Qt::white);
	ret = pm.save(QString(FileName), "PPM");
	// FIXME: we should probably return None on success and throw an exception on failure
	// rather than returning an error code here.
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_getlayers(PyObject *self)
{
	if(!checkHaveDocument())
		return NULL;
	PyObject *l;
	l = PyList_New(Carrier->doc->Layers.count());
	for (uint lam=0; lam < Carrier->doc->Layers.count(); lam++)
		PyList_SetItem(l, lam, PyString_FromString(Carrier->doc->Layers[lam].Name));
	return l;
}

PyObject *scribus_setactlayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't have an empty layer name","python error"));
		return NULL;
	}
	int i = -1;
	bool found = false;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			i = static_cast<int>(lam);
			Carrier->doc->ActiveLayer = i;
			Carrier->changeLayer(i);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getactlayer(PyObject *self)
{
	if(!checkHaveDocument())
		return NULL;
	return PyString_FromString(Carrier->doc->Layers[Carrier->doc->ActiveLayer].Name);
}

PyObject *scribus_senttolayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Layer = "";
	if (!PyArg_ParseTuple(args, "s|s", &Layer, &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Layer, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't have an empty layer name","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString(Name));
	if (i == NULL)
		return NULL;
	i->OwnPage->SelectItemNr(i->ItemNr);
	bool found = false;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Layer))
		{
			i->LayerNr = static_cast<int>(lam);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_layervisible(PyObject *self, PyObject* args)
{
	char *Name = "";
	int vis = 1;
	if (!PyArg_ParseTuple(args, "si", &Name, &vis))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't have an empty layer name","python error"));
		return NULL;
	}
	bool found = false;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			Carrier->doc->Layers[lam].Sichtbar = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_layerprint(PyObject *self, PyObject* args)
{
	char *Name = "";
	int vis = 1;
	if (!PyArg_ParseTuple(args, "si", &Name, &vis))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't have an empty layer name","python error"));
		return NULL;
	}
	bool found = false;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			Carrier->doc->Layers[lam].Drucken = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_glayervisib(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't have an empty layer name","python error"));
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); lam++)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			i = static_cast<int>(Carrier->doc->Layers[lam].Sichtbar);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found","python error"));
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_glayerprint(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't have an empty layer name","python error"));
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			i = static_cast<int>(Carrier->doc->Layers[lam].Drucken);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found","python error"));
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_removelayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, ""))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't have an empty layer name","python error"));
		return NULL;
	}
	if (Carrier->doc->Layers.count() == 1)
	{
		PyErr_SetString(ScribusException, QObject::tr("Can't remove the last layer","python error"));
		return NULL;
	}
	bool found = false;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			QValueList<Layer>::iterator it2 = Carrier->doc->Layers.at(lam);
			int num2 = (*it2).LNr;
			if (!num2)
			{
				// FIXME: WTF DOES THIS DO?
				Py_INCREF(Py_None);
				return Py_None;
			}
			int num = (*it2).Level;
			Carrier->doc->Layers.remove(it2);
			QValueList<Layer>::iterator it;
			for (uint l = 0; l < Carrier->doc->Layers.count(); l++)
			{
				it = Carrier->doc->Layers.at(l);
				if ((*it).Level > num)
					(*it).Level -= 1;
			}
			Carrier->LayerRemove(num2);
			Carrier->doc->ActiveLayer = 0;
			Carrier->changeLayer(0);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_createlayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Can't create layer without a name","python error"));
		return NULL;
	}
	QString tmp;
	struct Layer ll;
	ll.LNr = Carrier->doc->Layers.last().LNr + 1;
	ll.Level = Carrier->doc->Layers.count();
	// FIXME: what if the name exists?
	ll.Name = QString(Name);
	ll.Sichtbar = true;
	ll.Drucken = true;
	Carrier->doc->Layers.append(ll);
	Carrier->doc->ActiveLayer = ll.LNr;
	Carrier->changeLayer(ll.LNr);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getlanguage(PyObject *self)
{
	return PyString_FromString(Carrier->GuiLanguage);
}

