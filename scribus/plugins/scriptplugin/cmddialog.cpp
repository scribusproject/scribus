#include <Python.h>
#include "cmddialog.h"
#include "cmdvar.h"
#include <qmessagebox.h>

PyObject *scribus_newdocdia(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	bool ret = Carrier->slotFileNew();
//	qApp->processEvents();
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

PyObject *scribus_mess(PyObject *self, PyObject* args)
{
	char *caption;
	if (!PyArg_ParseTuple(args, "s", &caption))
		return NULL;
	Carrier->FMess->setText(caption);
	return Py_None;
}

PyObject *scribus_messdia(PyObject *self, PyObject* args)
{
	char *caption = "";
	char *message = "";
	QMessageBox::Icon ico = QMessageBox::NoIcon;
	int butt1 = QMessageBox::NoButton;
	int butt2 = QMessageBox::NoButton;
	int butt3 = QMessageBox::NoButton;
	if (!PyArg_ParseTuple(args, "ssii|ii", &caption, &message, &ico, &butt1, &butt2, &butt3))
		return NULL;
	QMessageBox mb(caption, message, ico, butt1, butt2, butt3, Carrier);
	return PyInt_FromLong(static_cast<long>(mb.exec()));
}

