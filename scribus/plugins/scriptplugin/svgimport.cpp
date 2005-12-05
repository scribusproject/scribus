#include "svgimport.h"
#include "cmdvar.h"
#include "cmdutil.h"

// We need svgplugin.h for the SVG format ID, and for
// the FileFormat interface.
#include "plugins/svgimplugin/svgplugin.h"

#include <qstring.h>

PyObject *scribus_importsvg(PyObject* /* self */, PyObject* args)
{
	char *aText = 0;
	if (!PyArg_ParseTuple(args, const_cast<char*>("es"), const_cast<char*>("utf-8"), &aText))
		return NULL;

	if(!checkHaveDocument())
		return NULL;

	const FileFormat * fmt
		= LoadSavePlugin::getFormatById(FORMATID_SVGIMPORT);
	if (!fmt)
	{
		PyErr_SetString(PyExc_Exception, "SVG Import plugin not available");
		return NULL;
	}
	if (!fmt->loadFile(QString::fromUtf8(aText)))
	{
		PyErr_SetString(PyExc_Exception, "Import failed");
		return NULL;
	}

	ScMW->doc->setLoading(false);

	Py_INCREF(Py_None);
	return Py_None;
}

