#include <Python.h>

#include "guiapp.h"
#include "cmdvar.h"
#include <qstring.h>
#include <qcursor.h>
//#include <iostream.h>


PyObject *scribus_messagebartext(PyObject *self, PyObject* args)
{
	char *aText;
	if (!PyArg_ParseTuple(args, "s", &aText))
		return NULL;
	Py_INCREF(Py_None);
	Carrier->FMess->setText(QString(aText));
	return Py_None;
}


PyObject *scribus_progressreset(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	Py_INCREF(Py_None);
	Carrier->FProg->reset();
	qApp->processEvents();
	return Py_None;
}


PyObject *scribus_progresssettotalsteps(PyObject *self, PyObject* args)
{
	int steps;
	if (!PyArg_ParseTuple(args, "i", &steps))
		return NULL;
	Py_INCREF(Py_None);
	Carrier->FProg->setTotalSteps(steps);
	Carrier->FProg->setProgress(0);
	qApp->processEvents();
	return Py_None;
}


PyObject *scribus_progresssetprogress(PyObject *self, PyObject* args)
{
	int position;
	if (!PyArg_ParseTuple(args, "i", &position))
		return NULL;
	Py_INCREF(Py_None);
	if (position > Carrier->FProg->totalSteps())
		return Py_None;
	Carrier->FProg->setProgress(position);
	qApp->processEvents();
	return Py_None;
}


PyObject *scribus_setcursor(PyObject *self, PyObject* args)
{
	char *aCursor;
	if (!PyArg_ParseTuple(args, "s", &aCursor))
		return NULL;
	Py_INCREF(Py_None);
	if (aCursor=="wait")
		qApp->setOverrideCursor(Qt::WaitCursor);
	else
		qApp->restoreOverrideCursor();
	return Py_None;
}
