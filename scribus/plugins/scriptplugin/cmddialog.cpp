#include <Python.h>
#include "cmddialog.h"
#include "cmdvar.h"
#include "cmdutil.h"
#include "valuedialog.h"
#include <qmessagebox.h>

PyObject *scribus_newdocdia(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("NewDocDialog()"));
		return NULL;
	}
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
	if (!PyArg_ParseTuple(args, "sss|ii", &caption, &filter, &defName, &pre, &mode))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("FileDialog(caption, filter, defaultname, pre, mode)"));
		return NULL;
	}
	return PyString_FromString(Carrier->CFileDialog(".", caption, filter, defName, static_cast<bool>(pre), static_cast<bool>(mode), 0, 0));
}

PyObject *scribus_mess(PyObject *self, PyObject* args)
{
	char *caption;
	if (!PyArg_ParseTuple(args, "s", &caption))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("StatusMessage(text)"));
		return NULL;
	}
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
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("MessageBox(caption, message, ico, butt1, butt2, butt3)"));
		return NULL;
	}
	QMessageBox mb(caption, message, ico, butt1, butt2, butt3, Carrier);
	return PyInt_FromLong(static_cast<long>(mb.exec()));
}

PyObject *scribus_valdialog(PyObject *self, PyObject* args)
{
	char *caption = "";
	char *message = "";
	char *value = "";
	if (!PyArg_ParseTuple(args, "ss|s", &caption, &message, &value))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("ValueDialog(caption, message [,defaultvalue])"));
		return NULL;
	}
	ValueDialog *d = new ValueDialog(Carrier, "d", TRUE, 0);
	d->dialogLabel->setText(message);
	d->valueEdit->setText(value);
	d->setCaption(caption);
	d->exec();
	return PyString_FromString(d->valueEdit->text().utf8());
}
