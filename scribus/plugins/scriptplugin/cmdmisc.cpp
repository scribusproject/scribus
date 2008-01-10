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
//Added by qt3to4:
#include <QList>

#include "scribuscore.h"
#include "fonts/scfontmetrics.h"
#include "prefsmanager.h"

PyObject *scribus_setredraw(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	ScCore->primaryMainWindow()->doc->DoDrawing = static_cast<bool>(e);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_fontnames(PyObject* /* self */)
{
	int cc2 = 0;
	SCFontsIterator it2(PrefsManager::instance()->appPrefs.AvailFonts);
	for ( ; it2.hasNext() ; it2.next())
	{
		if (it2.current().usable())
			cc2++;
	}
	PyObject *l = PyList_New(cc2);
	SCFontsIterator it(PrefsManager::instance()->appPrefs.AvailFonts);
	int cc = 0;
	for ( ; it.hasNext() ; it.next())
	{
		if (it.current().usable())
		{
			PyList_SetItem(l, cc, PyString_FromString(it.currentKey().toUtf8()));
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
	for ( ; it.hasNext() ; it.next())
	{
		row = Py_BuildValue((char*)"(sssiis)",
							it.currentKey().toUtf8().data(),
							it.current().family().toUtf8().data(),
							it.current().psName().toUtf8().data(),
							it.current().subset(),
							it.current().embedPs(),
							it.current().fontFilePath().toUtf8().data()
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
	if (!PrefsManager::instance()->appPrefs.AvailFonts.contains(QString::fromUtf8(Name)))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Font not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	QString ts = QString::fromUtf8(Sample);
	if (ts.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot render an empty sample.","python error").toLocal8Bit().constData());
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
		QByteArray buffer_string = "";
		QBuffer buffer(&buffer_string);
		buffer.open(QIODevice::WriteOnly);
		bool ret = pm.save(&buffer, format);
		if (!ret)
		{
			PyErr_SetString(ScribusException, QObject::tr("Unable to save pixmap","scripter error").toLocal8Bit().constData());
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
			PyErr_SetString(PyExc_Exception, QObject::tr("Unable to save pixmap","scripter error").toLocal8Bit().constData());
			return NULL;
		}
		// For historical reasons, we need to return true on success.
//		Py_INCREF(Py_True);
//		return Py_True;
//		Py_RETURN_TRUE;
		return PyBool_FromLong(static_cast<long>(true));
	}
}

PyObject *scribus_getlayers(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	PyObject *l;
	l = PyList_New(ScCore->primaryMainWindow()->doc->Layers.count());
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); lam++)
		PyList_SetItem(l, lam, PyString_FromString(ScCore->primaryMainWindow()->doc->Layers[lam].Name.toUtf8()));
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	bool found = ScCore->primaryMainWindow()->doc->setActiveLayer(QString::fromUtf8(Name));
	if (found)
		ScCore->primaryMainWindow()->changeLayer(ScCore->primaryMainWindow()->doc->activeLayer());
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_getactlayer(PyObject* /* self */)
{
	if(!checkHaveDocument())
		return NULL;
	return PyString_FromString(ScCore->primaryMainWindow()->doc->activeLayerName().toUtf8());
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(Name));
	if (i == NULL)
		return NULL;
	ScCore->primaryMainWindow()->view->SelectItemNr(i->ItemNr);
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		ScCore->primaryMainWindow()->view->SelectItemNr(i->ItemNr);
		for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
			if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Layer))
			{
				i->LayerNr = static_cast<int>(lam);
				found = true;
				break;
			}
	}
	if (!found)
	{
		PyErr_SetString(ScribusException, QString("Layer not found").toLocal8Bit().constData());
		return NULL;
	}

//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
		PyErr_SetString(PyExc_ValueError, QString("Cannot have an empty layer name").toLocal8Bit().constData());
		return NULL;
	}
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScCore->primaryMainWindow()->doc->Layers[lam].isViewable = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScCore->primaryMainWindow()->doc->Layers[lam].isPrintable = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_layerlock(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", &Name, &vis))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScCore->primaryMainWindow()->doc->Layers[lam].isEditable = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_layeroutline(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", &Name, &vis))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScCore->primaryMainWindow()->doc->Layers[lam].outlineMode = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_layerflow(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", &Name, &vis))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScCore->primaryMainWindow()->doc->Layers[lam].flowControl = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_layerblend(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int vis = 0;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", &Name, &vis))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QString("Cannot have an empty layer name").toLocal8Bit().constData());
		return NULL;
	}
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScCore->primaryMainWindow()->doc->Layers[lam].blendMode = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_layertrans(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double vis = 1.0;
	if (!PyArg_ParseTuple(args, "esd", "utf-8", &Name, &vis))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QString("Cannot have an empty layer name").toLocal8Bit().constData());
		return NULL;
	}
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScCore->primaryMainWindow()->doc->Layers[lam].transparency = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); lam++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			i = static_cast<int>(ScCore->primaryMainWindow()->doc->Layers[lam].isViewable);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			i = static_cast<int>(ScCore->primaryMainWindow()->doc->Layers[lam].isPrintable);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_glayerlock(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); lam++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			i = static_cast<int>(ScCore->primaryMainWindow()->doc->Layers[lam].isEditable);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_glayeroutline(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); lam++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			i = static_cast<int>(ScCore->primaryMainWindow()->doc->Layers[lam].outlineMode);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_glayerflow(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); lam++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			i = static_cast<int>(ScCore->primaryMainWindow()->doc->Layers[lam].flowControl);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_glayerblend(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	int i = 0;
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); lam++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			i = ScCore->primaryMainWindow()->doc->Layers[lam].blendMode;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyInt_FromLong(static_cast<long>(i));
}

PyObject *scribus_glayertrans(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	double i = 1.0;
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); lam++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			i = ScCore->primaryMainWindow()->doc->Layers[lam].transparency;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
	return PyFloat_FromDouble(i);
}

PyObject *scribus_removelayer(PyObject* /* self */, PyObject* args)
{
//FIXME: Use the docs remove layer code
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (Name == "")
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	if (ScCore->primaryMainWindow()->doc->Layers.count() == 1)
	{
		PyErr_SetString(ScribusException, QObject::tr("Cannot remove the last layer.","python error").toLocal8Bit().constData());
		return NULL;
	}
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == QString::fromUtf8(Name))
		{
			ScLayer it2 = ScCore->primaryMainWindow()->doc->Layers.at(lam);
			int num2 = it2.LNr;
			if (!num2)
			{
				// FIXME: WTF DOES THIS DO?
				Py_INCREF(Py_None);
				return Py_None;
			}
			int num = it2.Level;
			ScCore->primaryMainWindow()->doc->Layers.removeAt(lam);
			ScLayer it;
			for (int l = 0; l < ScCore->primaryMainWindow()->doc->Layers.count(); l++)
			{
				it = ScCore->primaryMainWindow()->doc->Layers.at(l);
				if (it.Level > num)
					it.Level -= 1;
			}
			ScCore->primaryMainWindow()->doc->removeLayer(num2);
			ScCore->primaryMainWindow()->doc->setActiveLayer(0);
			ScCore->primaryMainWindow()->changeLayer(0);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create layer without a name.","python error").toLocal8Bit().constData());
		return NULL;
	}
	ScCore->primaryMainWindow()->doc->addLayer(QString::fromUtf8(Name), true);
	ScCore->primaryMainWindow()->changeLayer(ScCore->primaryMainWindow()->doc->activeLayer());
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_getlanguage(PyObject* /* self */)
{
	return PyString_FromString(ScCore->getGuiLanguage().toUtf8());
}

/*! 04.01.2007 : Joachim Neu : Moves item selection to front. */
PyObject *scribus_moveselectiontofront(PyObject*)
{
	ScCore->primaryMainWindow()->doc->bringItemSelectionToFront();
	Py_RETURN_NONE;
}

/*! 04.01.2007 : Joachim Neu : Moves item selection to back. */
PyObject *scribus_moveselectiontoback(PyObject*)
{
	ScCore->primaryMainWindow()->doc->sendItemSelectionToBack();
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdmiscdocwarnings()
{
    QStringList s;
    s << scribus_setredraw__doc__ <<scribus_fontnames__doc__ << scribus_xfontnames__doc__ <<scribus_renderfont__doc__ << scribus_getlayers__doc__ << scribus_setactlayer__doc__ << scribus_getactlayer__doc__ << scribus_senttolayer__doc__ <<scribus_layervisible__doc__ <<scribus_layerprint__doc__ <<scribus_layerlock__doc__ <<scribus_layeroutline__doc__ <<scribus_layerflow__doc__ <<scribus_layerblend__doc__ <<scribus_layertrans__doc__ <<scribus_glayervisib__doc__ <<scribus_glayerprint__doc__ <<scribus_glayerlock__doc__ <<scribus_glayeroutline__doc__ <<scribus_glayerflow__doc__ <<scribus_glayerblend__doc__ <<scribus_glayertrans__doc__ <<scribus_removelayer__doc__ <<scribus_createlayer__doc__ <<scribus_getlanguage__doc__;
}
