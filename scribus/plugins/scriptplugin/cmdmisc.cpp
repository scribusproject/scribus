/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdmisc.h"
#include "cmdutil.h"

#include <string>

#include <QBuffer>
#include <QList>
#include <QPixmap>

#include "prefsmanager.h"
#include "pyesstring.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "fonts/scfontmetrics.h"
#include "pdfoptionsio.h"

PyObject *scribus_setredraw(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->doc->DoDrawing = static_cast<bool>(e);
	Py_RETURN_NONE;
}

PyObject *scribus_getfontnames(PyObject* /* self */)
{
	int cc2 = 0;
	SCFontsIterator it2(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts);
	for ( ; it2.hasNext() ; it2.next())
	{
		if (it2.current().usable())
			cc2++;
	}
	PyObject *l = PyList_New(cc2);
	SCFontsIterator it(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts);
	int cc = 0;
	for ( ; it.hasNext() ; it.next())
	{
		if (it.current().usable())
		{
			PyList_SetItem(l, cc, PyUnicode_FromString(it.currentKey().toUtf8()));
			cc++;
		}
	}
	return l;
}

PyObject *scribus_xfontnames(PyObject* /* self */)
{
	PyObject *l = PyList_New(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts.count());
	SCFontsIterator it(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts);
	int cc = 0;
	PyObject *row;
	for ( ; it.hasNext() ; it.next())
	{
		row = Py_BuildValue("(sssiis)",
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
	PyESString name;
	PyESString fileName;
	PyESString sample;
	PyESString format;
	int Size;
	bool ret = false;
	char *kwargs[] = {const_cast<char*>("fontname"),
					  const_cast<char*>("filename"),
					  const_cast<char*>("sample"),
					  const_cast<char*>("size"),
					  const_cast<char*>("format"),
					  nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "esesesi|es", kwargs,
				"utf-8", name.ptr(), "utf-8", fileName.ptr(), "utf-8", sample.ptr(), &Size, "ascii", format.ptr()))
		return nullptr;
	if (!PrefsManager::instance().appPrefs.fontPrefs.AvailFonts.contains(QString::fromUtf8(name.c_str())))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Font not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QVector<uint> ts = QString::fromUtf8(sample.c_str()).toUcs4();
	if (ts.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot render an empty sample.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	std::string strFormat(format.c_str());
	if (strFormat.empty())
		// User specified no format, so use the historical default of PPM format.
		strFormat =  "PPM";
	QPixmap pm = FontSample(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[QString::fromUtf8(name.c_str())], Size, ts, Qt::white);
	// If the user specified an empty filename, return the image data as bytes. Otherwise, save it to disk.
	if (fileName.isEmpty())
	{
		QByteArray buffer_string = "";
		QBuffer buffer(&buffer_string);
		buffer.open(QIODevice::WriteOnly);
		bool ret = pm.save(&buffer, strFormat.c_str());
		if (!ret)
		{
			PyErr_SetString(ScribusException, QObject::tr("Unable to save pixmap","scripter error").toLocal8Bit().constData());
			return nullptr;
		}
		int bufferSize = buffer.size();
		buffer.close();
		// Now make a Python string from the data we generated
		PyObject* stringPython = PyBytes_FromStringAndSize(buffer_string, bufferSize);
		// Return even if the result is nullptr (error) since an exception will have been
		// set in that case.
		return stringPython;
	}

	// Save the pixmap to a file, since the filename is non-empty
	ret = pm.save(QString::fromUtf8(fileName.c_str()), strFormat.c_str());
	if (!ret)
	{
		PyErr_SetString(PyExc_Exception, QObject::tr("Unable to save pixmap","scripter error").toLocal8Bit().constData());
		return nullptr;
	}
	// For historical reasons, we need to return true on success.
	//		Py_INCREF(Py_True);
	//		return Py_True;
	//		Py_RETURN_TRUE;
	return PyBool_FromLong(static_cast<long>(true));
}

PyObject *scribus_getlayers(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;
	PyObject *l = PyList_New(doc->Layers.count());
	for (int i = 0; i < doc->Layers.count(); i++)
		PyList_SetItem(l, i, PyUnicode_FromString(doc->Layers[i].Name.toUtf8()));
	return l;
}

PyObject *scribus_setactivelayer(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	bool found = ScCore->primaryMainWindow()->doc->setActiveLayer(QString::fromUtf8(name.c_str()));
	if (found)
		ScCore->primaryMainWindow()->changeLayer(ScCore->primaryMainWindow()->doc->activeLayer());
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_getactivelayer(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	return PyUnicode_FromString(ScCore->primaryMainWindow()->doc->activeLayerName().toUtf8());
}

PyObject *scribus_loweractivelayer(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	int activeLayer = ScCore->primaryMainWindow()->doc->activeLayer();
	ScCore->primaryMainWindow()->doc->lowerLayer(activeLayer);
	Py_RETURN_NONE;
}

PyObject *scribus_raiseactivelayer(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	int activeLayer = ScCore->primaryMainWindow()->doc->activeLayer();
	ScCore->primaryMainWindow()->doc->raiseLayer(activeLayer);
	Py_RETURN_NONE;
}

PyObject *scribus_sendtolayer(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	char *Layer = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Layer, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (strlen(Layer) == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	ScribusDoc* currentDoc   = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	const ScLayer *scLayer = currentDoc->Layers.layerByName( QString::fromUtf8(Layer) );
	if (!scLayer)
	{
		PyErr_SetString(ScribusException, QString("Layer not found").toLocal8Bit().constData());
		return nullptr;
	}
	// If no name have been specified in args, process whole selection
	currentView->selectItem(item);
	if (name.isEmpty())
	{
		for (int i = 0; i < currentDoc->m_Selection->count(); ++i)
		{
			item = currentDoc->m_Selection->itemAt(i);
			item->m_layerID = scLayer->ID;
		}
	}
	else
	{
		item->m_layerID = scLayer->ID;
	}

	Py_RETURN_NONE;
}

PyObject *scribus_setlayervisible(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", name.ptr(), &vis))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QString("Cannot have an empty layer name").toLocal8Bit().constData());
		return nullptr;
	}
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			ScCore->primaryMainWindow()->doc->Layers[i].isViewable = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_setlayerprintable(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", name.ptr(), &vis))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			ScCore->primaryMainWindow()->doc->Layers[i].isPrintable = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_setlayerlocked(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", name.ptr(), &vis))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			ScCore->primaryMainWindow()->doc->Layers[i].isEditable = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_setlayeroutlined(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", name.ptr(), &vis))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			ScCore->primaryMainWindow()->doc->Layers[i].outlineMode = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_setlayerflow(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	int vis = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", name.ptr(), &vis))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			ScCore->primaryMainWindow()->doc->Layers[i].flowControl = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_setlayerblendmode(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	int vis = 0;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", name.ptr(), &vis))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QString("Cannot have an empty layer name").toLocal8Bit().constData());
		return nullptr;
	}
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			ScCore->primaryMainWindow()->doc->Layers[i].blendMode = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_setlayertransparency(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double vis = 1.0;
	if (!PyArg_ParseTuple(args, "esd", "utf-8", name.ptr(), &vis))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QString("Cannot have an empty layer name").toLocal8Bit().constData());
		return nullptr;
	}
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			ScCore->primaryMainWindow()->doc->Layers[i].transparency = vis;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_islayervisible(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	int j = 0;
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); i++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			j = static_cast<int>(ScCore->primaryMainWindow()->doc->Layers[i].isViewable);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(j));
}

PyObject *scribus_islayerprintable(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	int j = 0;
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			j = static_cast<int>(ScCore->primaryMainWindow()->doc->Layers[i].isPrintable);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(j));
}

PyObject *scribus_islayerlocked(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	int j = 0;
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); i++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			j = static_cast<int>(!ScCore->primaryMainWindow()->doc->Layers[i].isEditable);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(j));
}

PyObject *scribus_islayeroutlined(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	int j = 0;
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); i++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			j = static_cast<int>(ScCore->primaryMainWindow()->doc->Layers[i].outlineMode);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(j));
}

PyObject *scribus_islayerflow(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	int j = 0;
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); i++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			j = static_cast<int>(ScCore->primaryMainWindow()->doc->Layers[i].flowControl);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(j));
}

PyObject *scribus_getlayerblendmode(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	int j = 0;
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); i++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			j = ScCore->primaryMainWindow()->doc->Layers[i].blendMode;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyLong_FromLong(static_cast<long>(j));
}

PyObject *scribus_getlayertransparency(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	double j = 1.0;
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); i++)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			j = ScCore->primaryMainWindow()->doc->Layers[i].transparency;
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	return PyFloat_FromDouble(j);
}

PyObject *scribus_deletelayer(PyObject* /* self */, PyObject* args)
{
//FIXME: Use the docs remove layer code
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty layer name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	if (ScCore->primaryMainWindow()->doc->Layers.count() == 1)
	{
		PyErr_SetString(ScribusException, QObject::tr("Cannot remove the last layer.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	bool found = false;
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == QString::fromUtf8(name.c_str()))
		{
			ScLayer it2 = ScCore->primaryMainWindow()->doc->Layers.at(i);
			int num2 = it2.ID;
			if (!num2)
			{
				// FIXME: WTF DOES THIS DO?
				Py_INCREF(Py_None);
				return Py_None;
			}
			ScCore->primaryMainWindow()->doc->removeLayer(num2);
			ScCore->primaryMainWindow()->doc->Layers.removeLayerByID(num2);
			ScCore->primaryMainWindow()->doc->setActiveLayer(0);
			ScCore->primaryMainWindow()->changeLayer(0);
			found = true;
			break;
		}
	}
	if (!found)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Layer not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_createlayer(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create layer without a name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->doc->addLayer(QString::fromUtf8(name.c_str()), true);
	ScCore->primaryMainWindow()->changeLayer(ScCore->primaryMainWindow()->doc->activeLayer());
	Py_RETURN_NONE;
}

PyObject *scribus_filequit(PyObject* /* self */, PyObject* args)
{
	QMetaObject::invokeMethod(ScCore->primaryMainWindow(), "slotFileQuit", Qt::QueuedConnection);
	Py_RETURN_NONE;
}

PyObject *scribus_getlanguage(PyObject* /* self */)
{
	return PyUnicode_FromString(ScCore->getGuiLanguage().toUtf8());
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

PyObject *scribus_savepdfoptions(PyObject* /* self */, PyObject* args)
{
	PyESString file;
	if (!checkHaveDocument())
		return nullptr;
	if (!PyArg_ParseTuple(args, const_cast<char*>("es"), "utf-8", file.ptr()))
		return nullptr;

	PDFOptionsIO io(ScCore->primaryMainWindow()->doc->pdfOptions());
	if (!io.writeTo(file.c_str()))
	{
		PyErr_SetString(ScribusException, io.lastError().toUtf8());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_readpdfoptions(PyObject* /* self */, PyObject* args)
{
	PyESString file;
	if (!checkHaveDocument())
		return nullptr;
	if (!PyArg_ParseTuple(args, const_cast<char*>("es"), "utf-8", file.ptr()))
		return nullptr;

	PDFOptionsIO io(ScCore->primaryMainWindow()->doc->pdfOptions());
	if (!io.readFrom(file.c_str()))
	{
		PyErr_SetString(ScribusException, io.lastError().toUtf8());
		return nullptr;
	}
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdmiscdocwarnings()
{
	QStringList s;
	s << scribus_createlayer__doc__
	  << scribus_deletelayer__doc__
	  << scribus_filequit__doc__
	  << scribus_getfontnames__doc__
	  << scribus_getactivelayer__doc__ 
	  << scribus_getlanguage__doc__
	  << scribus_getlayerblendmode__doc__
	  << scribus_getlayers__doc__
	  << scribus_getlayertransparency__doc__
	  << scribus_islayerflow__doc__
	  << scribus_islayerlocked__doc__
	  << scribus_islayeroutlined__doc__
	  << scribus_islayerprintable__doc__
	  << scribus_islayervisible__doc__
	  << scribus_loweractivelayer__doc__
	  << scribus_moveselectiontoback__doc__ 
	  << scribus_moveselectiontofront__doc__
	  << scribus_raiseactivelayer__doc__
	  << scribus_readpdfoptions__doc__
	  << scribus_renderfont__doc__
	  << scribus_savepdfoptions__doc__
	  << scribus_sendtolayer__doc__
	  << scribus_setactivelayer__doc__
	  << scribus_setlayerblendmode__doc__
	  << scribus_setlayerflow__doc__ 
	  << scribus_setlayerlocked__doc__
	  << scribus_setlayeroutlined__doc__
	  << scribus_setlayerprintable__doc__
	  << scribus_setlayertransparency__doc__
	  << scribus_setlayervisible__doc__ 
	  << scribus_setredraw__doc__  
	  << scribus_xfontnames__doc__;
}
