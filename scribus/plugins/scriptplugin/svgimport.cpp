/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "svgimport.h"
#include "cmdvar.h"
#include "cmdutil.h"

// We need svgpluginid.h for the SVG format ID, and
// loadsaveplugin.h for the FileFormat interface.
#include "../formatidlist.h"
#include "loadsaveplugin.h"
#include "scribuscore.h"
#include "selection.h"
#include "propertiespalette.h"

#include <QString>

PyObject *scribus_placesvg(PyObject* /* self */, PyObject* args)
{
	char *Image;
	double x = 0.0;
	double y = 0.0;
	if (!PyArg_ParseTuple(args, "es|dd", "utf-8", &Image, &x, &y))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	const FileFormat * fmt = LoadSavePlugin::getFormatById(FORMATID_SVGIMPORT);
	if( fmt )
	{
		fmt->loadFile(QString::fromUtf8(Image), LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
		if (ScCore->primaryMainWindow()->doc->m_Selection->count() > 1)
		{
			double x2, y2, w, h;
			ScCore->primaryMainWindow()->doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
			ScCore->primaryMainWindow()->view->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
			ScCore->primaryMainWindow()->propertiesPalette->updateColorList();
			ScCore->primaryMainWindow()->propertiesPalette->paraStyleCombo->updateFormatList();
			ScCore->primaryMainWindow()->propertiesPalette->charStyleCombo->updateFormatList();
			ScCore->primaryMainWindow()->propertiesPalette->SetLineFormats(ScCore->primaryMainWindow()->doc);
		}
	}
	else
	{
		PyErr_SetString(PyExc_Exception, "SVG Import plugin not available");
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_placeeps(PyObject* /* self */, PyObject* args)
{
	char *Image;
	double x = 0.0;
	double y = 0.0;
	if (!PyArg_ParseTuple(args, "es|dd", "utf-8", &Image, &x, &y))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	const FileFormat * fmt = LoadSavePlugin::getFormatById(FORMATID_PSIMPORT);
	if( fmt )
	{
		fmt->loadFile(QString::fromUtf8(Image), LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
		if (ScCore->primaryMainWindow()->doc->m_Selection->count() > 1)
		{
			double x2, y2, w, h;
			ScCore->primaryMainWindow()->doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
			ScCore->primaryMainWindow()->view->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
			ScCore->primaryMainWindow()->propertiesPalette->updateColorList();
			ScCore->primaryMainWindow()->propertiesPalette->paraStyleCombo->updateFormatList();
			ScCore->primaryMainWindow()->propertiesPalette->charStyleCombo->updateFormatList();
			ScCore->primaryMainWindow()->propertiesPalette->SetLineFormats(ScCore->primaryMainWindow()->doc);
		}
	}
	else
	{
		PyErr_SetString(PyExc_Exception, "EPS Import plugin not available");
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_placesxd(PyObject* /* self */, PyObject* args)
{
	char *Image;
	double x = 0.0;
	double y = 0.0;
	if (!PyArg_ParseTuple(args, "es|dd", "utf-8", &Image, &x, &y))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	const FileFormat * fmt = LoadSavePlugin::getFormatById(FORMATID_SXDIMPORT);
	if( fmt )
	{
		fmt->loadFile(QString::fromUtf8(Image), LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
		if (ScCore->primaryMainWindow()->doc->m_Selection->count() > 1)
		{
			double x2, y2, w, h;
			ScCore->primaryMainWindow()->doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
			ScCore->primaryMainWindow()->view->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
			ScCore->primaryMainWindow()->propertiesPalette->updateColorList();
			ScCore->primaryMainWindow()->propertiesPalette->paraStyleCombo->updateFormatList();
			ScCore->primaryMainWindow()->propertiesPalette->charStyleCombo->updateFormatList();
			ScCore->primaryMainWindow()->propertiesPalette->SetLineFormats(ScCore->primaryMainWindow()->doc);
		}
	}
	else
	{
		PyErr_SetString(PyExc_Exception, "OpenOffice Import plugin not available");
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_placeodg(PyObject* /* self */, PyObject* args)
{
	char *Image;
	double x = 0.0;
	double y = 0.0;
	if (!PyArg_ParseTuple(args, "es|dd", "utf-8", &Image, &x, &y))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	const FileFormat * fmt = LoadSavePlugin::getFormatById(FORMATID_ODGIMPORT);
	if( fmt )
	{
		fmt->loadFile(QString::fromUtf8(Image), LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
		if (ScCore->primaryMainWindow()->doc->m_Selection->count() > 1)
		{
			double x2, y2, w, h;
			ScCore->primaryMainWindow()->doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
			ScCore->primaryMainWindow()->view->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
			ScCore->primaryMainWindow()->propertiesPalette->updateColorList();
			ScCore->primaryMainWindow()->propertiesPalette->paraStyleCombo->updateFormatList();
			ScCore->primaryMainWindow()->propertiesPalette->charStyleCombo->updateFormatList();
			ScCore->primaryMainWindow()->propertiesPalette->SetLineFormats(ScCore->primaryMainWindow()->doc);
		}
	}
	else
	{
		PyErr_SetString(PyExc_Exception, "OpenOffice Import plugin not available");
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void svgimportdocwarnings()
{
    QStringList s;
    s << scribus_placesvg__doc__ << scribus_placeeps__doc__ << scribus_placesxd__doc__ << scribus_placeodg__doc__;
}
