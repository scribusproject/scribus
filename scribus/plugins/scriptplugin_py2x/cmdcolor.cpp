/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdcolor.h"
#include "cmdutil.h"
#include "prefsmanager.h"
#include "commonstrings.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "sccolorengine.h"

PyObject *scribus_colornames(PyObject* /* self */)
{
	ColorList edc;
	PyObject *l;
	int cc = 0;
	edc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc->PageColors : PrefsManager::instance().colorSet();
	ColorList::Iterator it;
	l = PyList_New(edc.count());
	for (it = edc.begin(); it != edc.end(); ++it)
	{
		PyList_SetItem(l, cc, PyString_FromString(it.key().toUtf8()));
		cc++;
	}
	return l;
}

PyObject *scribus_getcolor(PyObject* /* self */, PyObject* args)
{
	ColorList edc;
	char *Name = const_cast<char*>("");
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	edc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc->PageColors : PrefsManager::instance().colorSet();
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc : nullptr;
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	CMYKColor cmykValues;
	ScColorEngine::getCMYKValues(edc[col], currentDoc, cmykValues);
	cmykValues.getValues(c, m, y, k);
	return Py_BuildValue("(iiii)", static_cast<long>(c), static_cast<long>(m), static_cast<long>(y), static_cast<long>(k));
}

PyObject *scribus_getcolorfloat(PyObject* /* self */, PyObject* args)
{
	ColorList edc;
	char *Name = const_cast<char*>("");
	double c, m, y, k;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	edc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc->PageColors : PrefsManager::instance().colorSet();
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc : nullptr;
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	CMYKColorF cmykValues;
	ScColorEngine::getCMYKValues(edc[col], currentDoc, cmykValues);
	cmykValues.getValues(c, m, y, k);
	return Py_BuildValue("(dddd)", c * 100.0, m * 100.0, y * 100.0, k * 100.0);
}

PyObject *scribus_getcolorasrgb(PyObject* /* self */, PyObject* args)
{
	ColorList edc;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	edc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc->PageColors : PrefsManager::instance().colorSet();
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc : nullptr;
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QColor rgb = ScColorEngine::getRGBColor(edc[col], currentDoc);
	return Py_BuildValue("(iii)", static_cast<long>(rgb.red()), static_cast<long>(rgb.green()), static_cast<long>(rgb.blue()));
}

PyObject *scribus_getcolorasrgbfloat(PyObject* /* self */, PyObject* args)
{
	ColorList edc;
	double r, g, b;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot get a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	edc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc->PageColors : PrefsManager::instance().colorSet();
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->HaveDoc ? ScCore->primaryMainWindow()->doc : nullptr;
	QString col = QString::fromUtf8(Name);
	if (!edc.contains(col))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	RGBColorF rgbValues;
	ScColorEngine::getRGBValues(edc[col], currentDoc, rgbValues);
	rgbValues.getValues(r, g, b);
	return Py_BuildValue("(ddd)", r * 255.0, g * 255.0, b * 255.0);
}

PyObject *scribus_setcolor(PyObject*  self, PyObject* args)
{
	return scribus_setcolorcmyk(self, args);
}

PyObject *scribus_setcolorcmyk(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "esiiii", "utf-8", &Name, &c, &m, &y, &k))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot change a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		ScCore->primaryMainWindow()->doc->PageColors[col].setCmykColor(c, m, y, k);
	}
	else
	{
		ColorList* colorList=PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		(*colorList)[col].setCmykColor(c, m, y, k);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_setcolorcmykfloat(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double c, m, y, k;
	if (!PyArg_ParseTuple(args, "esdddd", "utf-8", &Name, &c, &m, &y, &k))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot change a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	c = qMax(0.0, qMin(c , 100.0)) / 100.0;
	m = qMax(0.0, qMin(m , 100.0)) / 100.0;
	y = qMax(0.0, qMin(y , 100.0)) / 100.0;
	k = qMax(0.0, qMin(k , 100.0)) / 100.0;
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		ScCore->primaryMainWindow()->doc->PageColors[col].setCmykColorF(c, m, y, k);
	}
	else
	{
		ColorList* colorList=PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		(*colorList)[col].setCmykColorF(c, m, y, k);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_setcolorrgb(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int r, g, b;
	if (!PyArg_ParseTuple(args, "esiii", "utf-8", &Name, &r, &g, &b))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot change a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		ScCore->primaryMainWindow()->doc->PageColors[col].setRgbColor(r, g, b);
	}
	else
	{
		ColorList* colorList=PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		(*colorList)[col].setRgbColor(r, g, b);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_setcolorrgbfloat(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double r, g, b;
	if (!PyArg_ParseTuple(args, "esddd", "utf-8", &Name, &r, &g, &b))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot change a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	r = qMax(0.0, qMin(r, 255.0)) / 255.0;
	g = qMax(0.0, qMin(g, 255.0)) / 255.0;
	b = qMax(0.0, qMin(b, 255.0)) / 255.0;
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		ScCore->primaryMainWindow()->doc->PageColors[col].setRgbColorF(r, g, b);
	}
	else
	{
		ColorList* colorList = PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error").toLocal8Bit().constData());
			return nullptr;
		}
		(*colorList)[col].setRgbColorF(r, g, b);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_setcolorlab(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double L, a, b;
	if (!PyArg_ParseTuple(args, "esddd", "utf-8", &Name, &L, &a, &b))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot change a color with an empty name.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	L = qMax(0.0, qMin(L, 100.0));
	a = qMax(-128.0, qMin(a, 128.0));
	b = qMax(-128.0, qMin(b, 128.0));
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.", "python error").toLocal8Bit().constData());
			return nullptr;
		}
		ScCore->primaryMainWindow()->doc->PageColors[col].setLabColor(L, a, b);
	}
	else
	{
		ColorList* colorList = PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.", "python error").toLocal8Bit().constData());
			return nullptr;
		}
		(*colorList)[col].setLabColor(L, a, b);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_newcolor(PyObject* self, PyObject* args)
{
	return scribus_newcolorcmyk(self, args);
}

PyObject *scribus_newcolorcmyk(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int c, m, y, k;
	if (!PyArg_ParseTuple(args, "esiiii", "utf-8", &Name, &c, &m, &y, &k))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
			ScCore->primaryMainWindow()->doc->PageColors.insert(col, ScColor(c, m, y, k));
		else
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			ScCore->primaryMainWindow()->doc->PageColors[col].setCmykColor(c, m, y, k);
	}
	else
	{
		ColorList* colorList=PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
			colorList->insert(col, ScColor(c, m, y, k));
		else
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			(*colorList)[col].setCmykColor(c, m, y, k);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_newcolorcmykfloat(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double c, m, y, k;
	if (!PyArg_ParseTuple(args, "esdddd", "utf-8", &Name, &c, &m, &y, &k))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	c = qMax(0.0, qMin(c , 100.0)) / 100.0;
	m = qMax(0.0, qMin(m , 100.0)) / 100.0;
	y = qMax(0.0, qMin(y , 100.0)) / 100.0;
	k = qMax(0.0, qMin(k , 100.0)) / 100.0;
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		{
			ScColor tmp;
			tmp.setCmykColorF(c, m, y, k);
			ScCore->primaryMainWindow()->doc->PageColors.insert(col, tmp);
		}
		else
		{
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			ScCore->primaryMainWindow()->doc->PageColors[col].setCmykColorF(c, m, y, k);
		}
	}
	else
	{
		ColorList* colorList = PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
		{
			ScColor tmp;
			tmp.setCmykColorF(c, m, y, k);
			colorList->insert(col, tmp);
		}
		else
		{
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			(*colorList)[col].setCmykColorF(c, m, y, k);
		}
	}

	Py_RETURN_NONE;
}

PyObject *scribus_newcolorrgb(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int r, g, b;
	if (!PyArg_ParseTuple(args, "esiii", "utf-8", &Name, &r, &g, &b))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
			ScCore->primaryMainWindow()->doc->PageColors.insert(col, ScColor(r, g, b));
		else
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			ScCore->primaryMainWindow()->doc->PageColors[col].setRgbColor(r, g, b);
	}
	else
	{
		ColorList* colorList=PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
			colorList->insert(col, ScColor(r, g, b));
		else
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			(*colorList)[col].setRgbColor(r, g, b);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_newcolorrgbfloat(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double r, g, b;
	if (!PyArg_ParseTuple(args, "esddd", "utf-8", &Name, &r, &g, &b))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	r = qMax(0.0, qMin(r, 255.0)) / 255.0;
	g = qMax(0.0, qMin(g, 255.0)) / 255.0;
	b = qMax(0.0, qMin(b, 255.0)) / 255.0;
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		{
			ScColor tmp;
			tmp.setRgbColorF(r, g, b);
			ScCore->primaryMainWindow()->doc->PageColors.insert(col, tmp);
		}
		else
		{
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			ScCore->primaryMainWindow()->doc->PageColors[col].setRgbColorF(r, g, b);
		}
	}
	else
	{
		ColorList* colorList = PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
		{
			ScColor tmp;
			tmp.setRgbColorF(r, g, b);
			colorList->insert(col, tmp);
		}
		else
		{
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			(*colorList)[col].setRgbColorF(r, g, b);
		}
	}

	Py_RETURN_NONE;
}

PyObject *scribus_newcolorlab(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	double L, a, b;
	if (!PyArg_ParseTuple(args, "esddd", "utf-8", &Name, &L, &a, &b))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot create a color with an empty name.", "python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	L = qMax(0.0, qMin(L, 100.0));
	a = qMax(-128.0, qMin(a, 128.0));
	b = qMax(-128.0, qMin(b, 128.0));
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		if (!ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		{
			ScColor tmp;
			tmp.setLabColor(L, a, b);
			ScCore->primaryMainWindow()->doc->PageColors.insert(col, tmp);
		}
		else
		{
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			ScCore->primaryMainWindow()->doc->PageColors[col].setLabColor(L, a, b);
		}
	}
	else
	{
		ColorList* colorList = PrefsManager::instance().colorSetPtr();
		if (!colorList->contains(col))
		{
			ScColor tmp;
			tmp.setLabColor(L, a, b);
			colorList->insert(col, tmp);
		}
		else
		{
			// FIXME: Given that we have a changeColour function, should we really be
			// silently changing colours in newColour?
			(*colorList)[col].setLabColor(L, a, b);
		}
	}

	Py_RETURN_NONE;
}

PyObject *scribus_delcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Repl = const_cast<char*>(CommonStrings::None.toLatin1().constData());
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Name, "utf-8", &Repl))
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot delete a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	QString rep = QString::fromUtf8(Repl);
	if (ScCore->primaryMainWindow()->HaveDoc)
	{
		ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
		if (currentDoc->PageColors.contains(col) && (currentDoc->PageColors.contains(rep) || (rep == CommonStrings::None)))
		{
			currentDoc->PageColors.remove(col);
			ReplaceColor(col, rep);
		}
		else
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in document.","python error").toLocal8Bit().constData());
			return nullptr;
		}
	}
	else
	{
		ColorList* colorList=PrefsManager::instance().colorSetPtr();
		if (colorList->contains(col))
			colorList->remove(col);
		else
		{
			PyErr_SetString(NotFoundError, QObject::tr("Color not found in default colors.","python error").toLocal8Bit().constData());
			return nullptr;
		}
	}

	Py_RETURN_NONE;
}

PyObject *scribus_replcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	char *Repl = const_cast<char*>(CommonStrings::None.toLatin1().constData());
	//FIXME: this should definitely use keyword arguments
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", &Name, "utf-8", &Repl))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot replace a color with an empty name.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	QString rep = QString::fromUtf8(Repl);
	if (ScCore->primaryMainWindow()->doc->PageColors.contains(col) && (ScCore->primaryMainWindow()->doc->PageColors.contains(rep) || (rep == CommonStrings::None)))
		ReplaceColor(col, rep);
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}

	Py_RETURN_NONE;
}

PyObject *scribus_isspotcolor(PyObject * /*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	
	if (!PyArg_ParseTuple(args, "es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Color name cannot be an empty string.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->doc->PageColors.contains(col))
		return PyBool_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->PageColors[col].isSpotColor()));
	PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
	return nullptr;
// 	Py_RETURN_NONE;
}

PyObject *scribus_setspotcolor(PyObject * /*self*/, PyObject* args)
{
	char *Name = const_cast<char*>("");
	int enable;

	if (!PyArg_ParseTuple(args, "esi", "utf-8", &Name, &enable))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (strcmp(Name, "") == 0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Color name cannot be an empty string.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	QString col = QString::fromUtf8(Name);
	if (ScCore->primaryMainWindow()->doc->PageColors.contains(col))
	{
		ScCore->primaryMainWindow()->doc->PageColors[col].setSpotColor(static_cast<bool>(enable));
	}
	else
	{
		PyErr_SetString(NotFoundError, QObject::tr("Color not found.","python error").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}



/*! HACK: this removes "warning: 'blash' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdcolordocswarnings()
{
	QStringList s;
	s << scribus_colornames__doc__
	  << scribus_delcolor__doc__
	  << scribus_getcolor__doc__
	  << scribus_getcolorasrgb__doc__
	  << scribus_getcolorasrgbfloat__doc__
	  << scribus_getcolorfloat__doc__
	  << scribus_isspotcolor__doc__ 
	  << scribus_newcolor__doc__
	  << scribus_newcolorcmyk__doc__
	  << scribus_newcolorcmykfloat__doc__
	  << scribus_newcolorlab__doc__
	  << scribus_newcolorrgb__doc__
	  << scribus_newcolorrgbfloat__doc__
	  << scribus_replcolor__doc__
	  << scribus_setcolor__doc__
	  << scribus_setcolorcmyk__doc__
	  << scribus_setcolorcmykfloat__doc__
	  << scribus_setcolorlab__doc__
	  << scribus_setcolorrgb__doc__
	  << scribus_setcolorrgbfloat__doc__
	  << scribus_setspotcolor__doc__;
}
