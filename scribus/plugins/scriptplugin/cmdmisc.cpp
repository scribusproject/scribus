#include <Python.h>
#include "cmdmisc.h"
#include "cmdutil.h"
#include "cmdvar.h"

extern QPixmap FontSample(QString da, int s, QString ts, QColor back);

PyObject *scribus_setredraw(PyObject *self, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setRedraw(e)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	Carrier->doc->DoDrawing = static_cast<bool>(e);
	return Py_None;
}

PyObject *scribus_fontnames(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getFontNames()"));
		return NULL;
	}
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

PyObject *scribus_xfontnames(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getXFontNames()"));
		return NULL;
	}
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
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("renderFont(name, filename, sample, size)"));
		return NULL;
	}
	if (!Carrier->Prefs.AvailFonts.find(QString(Name)))
		return PyInt_FromLong(static_cast<long>(ret));
	QString ts = QString(Sample);
	if ((ts == "") || (QString(FileName) == ""))
		return PyInt_FromLong(static_cast<long>(ret));
	QString da = Carrier->Prefs.AvailFonts[QString(Name)]->Datei;
	QPixmap pm = FontSample(da, Size, ts, Qt::white);
	ret = pm.save(QString(FileName), "PPM");
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_getlayers(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getLayers()"));
		return NULL;
	}
	PyObject *l;
	if (Carrier->HaveDoc)
	{
		l = PyList_New(Carrier->doc->Layers.count());
		for (uint lam=0; lam < Carrier->doc->Layers.count(); lam++)
		{
			PyList_SetItem(l, lam, PyString_FromString(Carrier->doc->Layers[lam].Name));
		}
	}
	else
		l = PyList_New(0);
	return l;
}

PyObject *scribus_setactlayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setActiveLayer(layername)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == ""))
		return Py_None;
	int i = -1;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			i = static_cast<int>(lam);
			Carrier->doc->ActiveLayer = i;
			Carrier->changeLayer(i);
			break;
		}
	}
	return Py_None;
}

PyObject *scribus_getactlayer(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getActiveLayer()"));
		return NULL;
	}
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	return PyString_FromString(Carrier->doc->Layers[Carrier->doc->ActiveLayer].Name);
}

PyObject *scribus_senttolayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	char *Layer = "";
	if (!PyArg_ParseTuple(args, "s|s", &Layer, &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("sentToLayer(layername [,obejctname])"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Layer == ""))
		return Py_None;
	PageItem *i = GetUniqueItem(QString(Name));
	if (i != NULL)
	{
		i->OwnPage->SelectItemNr(i->ItemNr);
		for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
			if (Carrier->doc->Layers[lam].Name == QString(Layer))
			{
				i->LayerNr = static_cast<int>(lam);
				break;
			}
	}
	return Py_None;
}

PyObject *scribus_layervisible(PyObject *self, PyObject* args)
{
	char *Name = "";
	int vis = 1;
	if (!PyArg_ParseTuple(args, "si", &Name, &vis))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setLayerVisible(layername, visible)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == ""))
		return Py_None;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			Carrier->doc->Layers[lam].Sichtbar = vis;
			break;
		}
	}
	return Py_None;
}

PyObject *scribus_layerprint(PyObject *self, PyObject* args)
{
	char *Name = "";
	int vis = 1;
	if (!PyArg_ParseTuple(args, "si", &Name, &vis))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setLayerPrintable(layername, Printable)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == ""))
		return Py_None;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			Carrier->doc->Layers[lam].Drucken = vis;
			break;
		}
	}
	return Py_None;
}

PyObject *scribus_glayervisib(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("isLayerVisible(layername)"));
		return NULL;
	}
	if ((!Carrier->HaveDoc) || (Name == ""))
		return PyInt_FromLong(0L);
	int i = 0;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); lam++)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			i = static_cast<int>(Carrier->doc->Layers[lam].Sichtbar);
			break;
		}
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_glayerprint(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("isLayerPrintable(layername)"));
		return NULL;
	}
	if ((!Carrier->HaveDoc) || (Name == ""))
		return PyInt_FromLong(0L);
	int i = 0;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			i = static_cast<int>(Carrier->doc->Layers[lam].Drucken);
			break;
		}
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_removelayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("deleteLayer(layername)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == "") || (Carrier->doc->Layers.count() == 1))
		return Py_None;
	for (uint lam=0; lam < Carrier->doc->Layers.count(); ++lam)
	{
		if (Carrier->doc->Layers[lam].Name == QString(Name))
		{
			QValueList<Layer>::iterator it2 = Carrier->doc->Layers.at(lam);
			int num2 = (*it2).LNr;
			if (!num2)
				return Py_None;
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
			break;
		}
	}
	return Py_None;
}

PyObject *scribus_createlayer(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "s", &Name))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("createLayer(layername)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if ((!Carrier->HaveDoc) || (Name == ""))
		return Py_None;
	QString tmp;
	struct Layer ll;
	ll.LNr = Carrier->doc->Layers.last().LNr + 1;
	ll.Level = Carrier->doc->Layers.count();
	ll.Name = QString(Name);
	ll.Sichtbar = true;
	ll.Drucken = true;
	Carrier->doc->Layers.append(ll);
	Carrier->doc->ActiveLayer = ll.LNr;
	Carrier->changeLayer(ll.LNr);
	return Py_None;
}

PyObject *scribus_getlanguage(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("getGuiLanguage()"));
		return NULL;
	}
	return PyString_FromString(Carrier->GuiLanguage);
}
