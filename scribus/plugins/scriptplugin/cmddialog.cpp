#include "cmdvar.h"
#include "cmddialog.h"

PyObject *scribus_newdocdia(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	bool ret = Carrier->slotFileNew();
	if (ret)
		doc = Carrier->doc;
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_filedia(PyObject *self, PyObject* args)
{
	char *caption;
	char *filter;
	char *defName;
	int pre = 0;
	int mode = 0;
	return !PyArg_ParseTuple(args, "sss|ii", &caption, &filter, &defName, &pre, &mode) ? NULL :
		PyString_FromString(Carrier->CFileDialog(caption, filter, defName, static_cast<bool>(pre), static_cast<bool>(mode), 0, 0));
}

