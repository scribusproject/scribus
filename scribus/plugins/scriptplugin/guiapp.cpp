#include <Python.h>

#include "guiapp.h"
#include "cmdvar.h"
#include <qstring.h>
#include <qcursor.h>

/*
 2004/09/15 pv - exceptions added - small fix into 1.2.1
               - Py_None possible bugs fixed to be sure Scribus won't raise 11 SIG.
 */

PyObject *scribus_messagebartext(PyObject *self, PyObject* args)
{
	char *aText;
	if (!PyArg_ParseTuple(args, "s", &aText))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("messagebarText(textstring)"));
		return NULL;
	}
	Carrier->FMess->setText(QString(aText));
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_progressreset(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("progressReset()"));
		return NULL;
	}
	Carrier->FProg->reset();
	qApp->processEvents();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_progresssettotalsteps(PyObject *self, PyObject* args)
{
	int steps;
	if (!PyArg_ParseTuple(args, "i", &steps))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("progressTotal(number)"));
		return NULL;
	}
	Carrier->FProg->setTotalSteps(steps);
	Carrier->FProg->setProgress(0);
	qApp->processEvents();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_progresssetprogress(PyObject *self, PyObject* args)
{
	int position;
	if (!PyArg_ParseTuple(args, "i", &position))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("progressSet(number)"));
		return NULL;
	}
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
	qDebug("WARNING! SetCursor() is not stable!");
	if (!PyArg_ParseTuple(args, "s", &aCursor))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setCusrsor(string)"));
		return NULL;
	}
	if (aCursor=="wait")
		qApp->setOverrideCursor(Qt::WaitCursor);
	else
		qApp->restoreOverrideCursor();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_docchanged(PyObject *self, PyObject* args)
{
	int aValue;
	if (!PyArg_ParseTuple(args, "i", &aValue))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("docChanged(number)"));
		return NULL;
	}
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	if (aValue>0)
		Carrier->slotDocCh(true);
	else
		Carrier->slotDocCh(false);
	return Py_None;
}

