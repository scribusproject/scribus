/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "guiapp.h"
#include "cmdutil.h"
#include "scribuscore.h"
#include <qstring.h>
#include <qcursor.h>

PyObject *scribus_messagebartext(PyObject* /* self */, PyObject* args)
{
	char *aText;
	if (!PyArg_ParseTuple(args, "es", "utf-8", &aText))
		return NULL;
	ScCore->primaryMainWindow()->setStatusBarInfoText(QString::fromUtf8(aText));
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_progressreset(PyObject* /* self */)
{
	ScCore->primaryMainWindow()->mainWindowProgressBar->reset();
	qApp->processEvents();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_progresssettotalsteps(PyObject* /* self */, PyObject* args)
{
	int steps;
	if (!PyArg_ParseTuple(args, "i", &steps))
		return NULL;
	ScCore->primaryMainWindow()->mainWindowProgressBar->setTotalSteps(steps);
	ScCore->primaryMainWindow()->mainWindowProgressBar->setProgress(0);
	qApp->processEvents();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_progresssetprogress(PyObject* /* self */, PyObject* args)
{
	int position;
	if (!PyArg_ParseTuple(args, "i", &position))
		return NULL;
	if (position > ScCore->primaryMainWindow()->mainWindowProgressBar->totalSteps())
	{
		PyErr_SetString(PyExc_ValueError, QString("Tried to set progress > maximum progress"));
		return NULL;
	}
	ScCore->primaryMainWindow()->mainWindowProgressBar->setProgress(position);
	qApp->processEvents();
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
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
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_docchanged(PyObject* /* self */, PyObject* args)
{
	int aValue;
	if (!PyArg_ParseTuple(args, "i", &aValue))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	ScCore->primaryMainWindow()->slotDocCh(static_cast<bool>(aValue));
	/*
	if (aValue>0)
		ScCore->primaryMainWindow()->slotDocCh(true);
	else
		ScCore->primaryMainWindow()->slotDocCh(false);*/
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

PyObject *scribus_zoomdocument(PyObject* /* self */, PyObject* args)
{
	double zoomFactor;
	if (!PyArg_ParseTuple(args, "d", &zoomFactor))
		return NULL;
	if(!checkHaveDocument())
		return NULL;
	if (zoomFactor > 0.0 || zoomFactor == -100.0)
		ScCore->primaryMainWindow()->slotZoom(zoomFactor);
	else
	{
		PyErr_SetString(PyExc_ValueError, QString("The zoom factor should be greater than 0.0 or equal to -100.0. See help(zoomFactor)."));
		return NULL;
	}
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void guiappdocwarnings()
{
    QStringList s;
    s << scribus_messagebartext__doc__ << scribus_progressreset__doc__ << scribus_progresssettotalsteps__doc__ << scribus_progresssetprogress__doc__ << scribus_setcursor__doc__ << scribus_docchanged__doc__ << scribus_zoomdocument__doc__;
}
