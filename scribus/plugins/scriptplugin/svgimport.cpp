#include "svgimport.h"
#include "cmdvar.h"
#include "cmdutil.h"

#include "pluginmanager.h"
#include <qstring.h>


PyObject *scribus_importsvg(PyObject* /* self */, PyObject* args)
{
	char *aText;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &aText))
		return NULL;

	if(!checkHaveDocument())
		return NULL;

	if (!Carrier->pluginManager->DLLexists(10))
		return NULL;

	Carrier->pluginManager->dllInput = QString::fromUtf8(aText);
	Carrier->pluginManager->callDLL(10);
	Carrier->doc->loading = false;

	Py_INCREF(Py_None);
	return Py_None;
}

