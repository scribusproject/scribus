#include "guiapp.h"
#include "cmdutil.h"

#include <qstring.h>
#include <qcursor.h>

PyObject *scribus_messagebartext(PyObject* /* self */, PyObject* args)
{
	char *aText;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &aText))
		return NULL;
	ScApp->mainWindowStatusLabel->setText(QString::fromUtf8(aText));
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_progressreset(PyObject* /* self */)
{
	ScApp->mainWindowProgressBar->reset();
	qApp->processEvents();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_progresssettotalsteps(PyObject* /* self */, PyObject* args)
{
	int steps;
	if (!PyArg_ParseTuple(args, "i", &steps))
		return NULL;
	ScApp->mainWindowProgressBar->setTotalSteps(steps);
	ScApp->mainWindowProgressBar->setProgress(0);
	qApp->processEvents();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_progresssetprogress(PyObject* /* self */, PyObject* args)
{
	int position;
	if (!PyArg_ParseTuple(args, "i", &position))
		return NULL;
	if (position > ScApp->mainWindowProgressBar->totalSteps())
	{
		PyErr_SetString(PyExc_ValueError, QString("Tried to set progress > maximum progress"));
		return NULL;
	}
	ScApp->mainWindowProgressBar->setProgress(position);
	qApp->processEvents();
	Py_INCREF(Py_None);
	return Py_None;
}


PyObject *scribus_setcursor(PyObject* /* self */, PyObject* args)
{
	char *aCursor;
	qDebug("WARNING! SetCursor() is not stable!");
	if (!PyArg_ParseTuple(args, "es", "ascii", &aCursor))
		return NULL;
	if (strcmp(aCursor, "wait") == 0)
		qApp->setOverrideCursor(Qt::WaitCursor);
	else
		qApp->restoreOverrideCursor();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_docchanged(PyObject* /* self */, PyObject* args)
{
	int aValue;
	if (!PyArg_ParseTuple(args, "i", &aValue))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	ScApp->slotDocCh(static_cast<bool>(aValue));
	/*
	if (aValue>0)
		ScApp->slotDocCh(true);
	else
		ScApp->slotDocCh(false);*/
	Py_INCREF(Py_None);
	return Py_None;
}

