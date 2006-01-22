/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "guiapp.h"
#include "cmdutil.h"

#include <qstring.h>
#include <qcursor.h>

PyObject *scribus_messagebartext(PyObject* /* self */, PyObject* args)
{
	char *aText;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &aText))
		return NULL;
	ScMW->mainWindowStatusLabel->setText(QString::fromUtf8(aText));
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_progressreset(PyObject* /* self */)
{
	ScMW->mainWindowProgressBar->reset();
	qApp->processEvents();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_progresssettotalsteps(PyObject* /* self */, PyObject* args)
{
	int steps;
	if (!PyArg_ParseTuple(args, "i", &steps))
		return NULL;
	ScMW->mainWindowProgressBar->setTotalSteps(steps);
	ScMW->mainWindowProgressBar->setProgress(0);
	qApp->processEvents();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_progresssetprogress(PyObject* /* self */, PyObject* args)
{
	int position;
	if (!PyArg_ParseTuple(args, "i", &position))
		return NULL;
	if (position > ScMW->mainWindowProgressBar->totalSteps())
	{
		PyErr_SetString(PyExc_ValueError, QString("Tried to set progress > maximum progress"));
		return NULL;
	}
	ScMW->mainWindowProgressBar->setProgress(position);
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
	ScMW->slotDocCh(static_cast<bool>(aValue));
	/*
	if (aValue>0)
		ScMW->slotDocCh(true);
	else
		ScMW->slotDocCh(false);*/
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *scribus_zoomdocument(PyObject* /* self */, PyObject* args)
{
	double zoomFactor;
	if (!PyArg_ParseTuple(args, "d", &zoomFactor))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (zoomFactor > 0.0 || zoomFactor == -100.0)
		ScMW->slotZoom(zoomFactor);
	else
	{
		PyErr_SetString(PyExc_ValueError, QString("The zoom factor should be greater than 0.0 or equal to -100.0. See help(zoomFactor)."));
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}
