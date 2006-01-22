/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdmisc.h"
#include "cmdutil.h"

#include "qbuffer.h"
#include "qpixmap.h"

#include "scfontmetrics.h"
#include "prefsmanager.h"

PyObject *scribus_setredraw(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	ScMW->doc->DoDrawing = static_cast<bool>(e);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_fontnames(PyObject* /* self */)
{
	int cc2 = 0;
	SCFontsIterator it2(PrefsManager::instance()->appPrefs.AvailFonts);
	for ( ; it2.current() ; ++it2)
	{
		if (it2.current()->UseFont)
			cc2++;
	}
	PyObject *l = PyList_New(cc2);
	SCFontsIterator it(PrefsManager::instance()->appPrefs.AvailFonts);
	int cc = 0;
	for ( ; it.current() ; ++it)
	{
		if (it.current()->UseFont)
		{
			PyList_SetItem(l, cc, PyString_FromString(it.currentKey().utf8()));
			cc++;
		}
	}
	return l;
}

PyObject *scribus_xfontnames(PyObject* /* self */)
{
	PyObject *l = PyList_New(PrefsManager::instance()->appPrefs.AvailFonts.count());
	SCFontsIterator it(PrefsManager::instance()->appPrefs.AvailFonts);
	int cc = 0;
	PyObject *row;
	for ( ; it.current() ; ++it)
	{
		row = Py_BuildValue((char*)"(sssiis)",
							it.currentKey().utf8().data(),
							it.current()->family().utf8().data(),
							it.current()->RealName().utf8().data(),
							it.current()->Subset,
							it.current()->EmbedPS,
							it.current()->fontFilePath().utf8().data()
						);
		PyList_SetItem(l, cc, row);
		cc++;
	} // for
	return l;
}

// This function is fairly complex because it can either save its output to a
// file, or return it as a Python string.
PyObject *scribus_renderfont(PyObject* /*self*/, PyObject* args, PyObject* kw)
{
	char *Name = const_cast<char*>("");
	char *FileName = const_cast<char*>("");
	char *Sample = const_cast<char*>("");
	char *format = NULL;
	int Size;
	bool ret = false;
	char *kwargs[] = {const_cast<char*>("fontname"),
					  const_cast<char*>("filename"),
					  const_cast<char*>("sample"),
					  const_cast<char*>("size"),
					  const_cast<char*>("format"),
					  NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "esesesi|es", kwargs,
				"utf-8", &Name, "utf-8", &FileName, "utf-8", &Sample, &Size, "ascii", &format))
		return NULL;
	if (!PrefsManager::instance()->appPrefs.AvailFonts.find(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Font not found.","python error"));
		return NULL;
	}
	QString ts = QString::fromUtf8(Sample);
	if (ts.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot render an empty sample.","python error"));
		return NULL;
	}
	if (!format)
		// User specified no format, so use the historical default of PPM format.
		format = "PPM";
	QPixmap pm = FontSample(PrefsManager::instance()->appPrefs.AvailFonts[QString::fromUtf8(Name)], Size, ts, Qt::white);
	// If the user specified an empty filename, return the image data as
	// a string. Otherwise, save it to disk.
	if (QString::fromUtf8(FileName).isEmpty())
	{
		QCString buffer_string = "";
		QBuffer buffer(buffer_string);
		buffer.open(IO_WriteOnly);
		bool ret = pm.save(&buffer, format);
		if (!ret)
		{
			PyErr_SetString(ScribusException, QObject::tr("Unable to save pixmap","scripter error"));
			return NULL;
		}
		int bufferSize = buffer.size();
		buffer.close();
		// Now make a Python string from the data we generated
		PyObject* stringPython = PyString_FromStringAndSize(buffer_string,bufferSize);
		// Return even if the result is NULL (error) since an exception will have been
		// set in that case.
		return stringPython;
	}
	else
	// Save the pixmap to a file, since the filename is non-empty
	{
		ret = pm.save(QString::fromUtf8(FileName), format);
		if (!ret)
		{
			PyErr_SetString(PyExc_Exception, QObject::tr("Unable to save pixmap","scripter error"));
			return NULL;
		}
		// For historical reasons, we need to return true on success.
		Py_INCREF(Py_True);
		return Py_True;
	}
}

PyObject *scribus_getlayers(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	PyObject *l;
	l = PyList_New(ScMW->doc->Layers.count());
	for (uint lam=0; lam < ScMW->doc->Layers.count(); lam++)
		PyList_SetItem(l, lam, PyString_FromString(ScMW->doc->Layers[lam].Name.utf8()));
	return l;
}

PyObject *scribus_setactlayer(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error"));
		return NULL;
	}
	bool found = ScMW->doc->setActiveLayer(QString::fromUtf8(Name));
	if (found)
		ScMW->changeLayer(ScMW->doc->activeLayer());
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getactlayer(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyString_FromString(ScMW->doc->activeLayerName().utf8());
}

PyObject *scribus_senttolayer(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Layer = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Layer, "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Layer == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error"));
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	ScMW->view->SelectItemNr(i->ItemNr);
	bool found = false;
	for (uint lam=0; lam < ScMW->doc->Layers.count(); ++lam)
	{
		ScMW->view->SelectItemNr(i->ItemNr);
		for (uint lam=0; lam < ScMW->doc->Layers.count(); ++lam)
			if (ScMW->doc->Layers[lam].Name == QString::fromUtf8(Layer))
			{
				i->LayerNr = static_cast<int>(lam);
				found = true;
				break;
			}
	}
	if (!found)
	{
		PyErr_SetString(ScribusException, QString("Layer not found"));
		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_layervisible(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", &Name, &vis))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QString("Cannot have an empty layer name"));
		return NULL;
	}
	bool found = false;
	for (uint lam=0; lam < ScMW->doc->Layers.count(); ++lam)
	{
		if (ScMW->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScMW->doc->Layers[lam].isViewable = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_layerprint(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", &Name, &vis))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error"));
		return NULL;
	}
	bool found = false;
	for (uint lam=0; lam < ScMW->doc->Layers.count(); ++lam)
	{
		if (ScMW->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScMW->doc->Layers[lam].isPrintable = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_glayervisib(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error"));
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (uint lam=0; lam < ScMW->doc->Layers.count(); lam++)
	{
		if (ScMW->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			i = static_cast<int>(ScMW->doc->Layers[lam].isViewable);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error"));
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_glayerprint(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error"));
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (uint lam=0; lam < ScMW->doc->Layers.count(); ++lam)
	{
		if (ScMW->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			i = static_cast<int>(ScMW->doc->Layers[lam].isPrintable);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error"));
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_removelayer(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error"));
		return NULL;
	}
	if (ScMW->doc->Layers.count() == 1)
	{
		PyErr_SetString(ScribusException, QObject::tr("Cannot remove the last layer.","python error"));
		return NULL;
	}
	bool found = false;
	for (uint lam=0; lam < ScMW->doc->Layers.count(); ++lam)
	{
		if (ScMW->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			QValueList<Layer>::iterator it2 = ScMW->doc->Layers.at(lam);
			int num2 = (*it2).LNr;
			if (!num2)
			{
				// FIXME: WTF DOES THIS DO?
				Py_INCREF(Py_None);
				return Py_None;
			}
			int num = (*it2).Level;
			ScMW->doc->Layers.remove(it2);
			QValueList<Layer>::iterator it;
			for (uint l = 0; l < ScMW->doc->Layers.count(); l++)
			{
				it = ScMW->doc->Layers.at(l);
				if ((*it).Level > num)
					(*it).Level -= 1;
			}
			ScMW->LayerRemove(num2);
			ScMW->doc->setActiveLayer(0);
			ScMW->changeLayer(0);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error"));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_createlayer(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create layer without a name.","python error"));
		return NULL;
	}
	ScMW->doc->addLayer(QString::fromUtf8(Name), true);
	ScMW->changeLayer(ScMW->doc->activeLayer());
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_getlanguage(PyObject* /* self */)
{
	return PyString_FromString(ScMW->getGuiLanguage().utf8());
}
