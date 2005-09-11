#include "svgimport.h"
#include "cmdvar.h"
#include "cmdutil.h"
#include "pluginmanager.h"

#include <qstring.h>


PyObject *scribus_importsvg(PyObject* /* self */, PyObject* args)
{
	char *aText;
	if (!PyArg_ParseTuple(args, const_cast<char*>("es"), const_cast<char*>("utf-8"), &aText))
		return NULL;

	if(!checkHaveDocument())
		return NULL;

	if (!ScApp->pluginManager->DLLexists("svgimplugin"))
		return NULL;

	ScApp->pluginManager->callImportExportPlugin("svgimplugin", QString::fromUtf8(aText));
	ScApp->doc->setLoading(false);

	Py_INCREF(Py_None);
	return Py_None;
}

