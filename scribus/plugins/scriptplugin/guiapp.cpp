#include <Python.h>

#include "guiapp.h"
#include "cmdvar.h"
#include "cmdutil.h"
#include <qstring.h>
#include <qcursor.h>

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
	if (position > Carrier->FProg->totalSteps())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Tried to set progress > maximum progress"));
		return NULL;
	}
	Carrier->FProg->setProgress(position);
	qApp->processEvents();
	Py_INCREF(Py_None);
	return Py_None;
}


PyObject *scribus_setcursor(PyObject *self, PyObject* args)
{
	char *aCursor;
	qDebug("WARNING! SetCursor() is not stable!");
	if (!PyArg_ParseTuple(args, "s", &aCursor))
	{
		PyErr_SetString(PyExc_Exception, ERRPARAM + QString("setCursor(string)"));
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
	if(!checkHaveDocument())
		return NULL;
	if (aValue>0)
		Carrier->slotDocCh(true);
	else
		Carrier->slotDocCh(false);
	Py_INCREF(Py_None);
	return Py_None;
}

