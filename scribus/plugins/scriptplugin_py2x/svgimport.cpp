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
#include "fileloader.h"
#include "../formatidlist.h"
#include "loadsaveplugin.h"
#include "scribuscore.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/propertiespalette.h"
#include "ui/propertiespalette_line.h"

#include <QString>

PyObject *scribus_placevec(PyObject* /* self */, PyObject* args)
{
	char *Image;
	double x = 0.0;
	double y = 0.0;
	if (!PyArg_ParseTuple(args, "es|dd", "utf-8", &Image, &x, &y))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	QStringList allFormatsV = LoadSavePlugin::getExtensionsForImport(FORMATID_FIRSTUSER);
	QString fName = QString::fromUtf8(Image);
	QFileInfo fi = QFileInfo(fName);
	QString ext = fi.suffix().toLower();
	if (!allFormatsV.contains(ext))
	{
		PyErr_SetString(PyExc_Exception, "Requested Import plugin not available");
		return nullptr;
	}
	FileLoader *fileLoader = new FileLoader(fName);
	int testResult = fileLoader->testFile();
	delete fileLoader;
	if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
	{
		const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
		if (fmt)
		{
			fmt->loadFile(fName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
			if (ScCore->primaryMainWindow()->doc->m_Selection->count() >= 1)
			{
				double x2, y2, w, h;
				ScCore->primaryMainWindow()->doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
				ScCore->primaryMainWindow()->view->startGroupTransaction();
				ScCore->primaryMainWindow()->doc->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
				ScCore->primaryMainWindow()->view->endGroupTransaction();
				ScCore->primaryMainWindow()->requestUpdate(reqColorsUpdate | reqLineStylesUpdate | reqTextStylesUpdate);
			}
		}
	}
	else
	{
		PyErr_SetString(PyExc_Exception, "Requested File cannot be imported");
		return nullptr;
	}
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void svgimportdocwarnings()
{
	QStringList s;
	s << scribus_placeeps__doc__
	  << scribus_placeodg__doc__
	  << scribus_placesvg__doc__
	  << scribus_placesxd__doc__
	  << scribus_placevec__doc__;
}
