/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "guiapp.h"
#include "cmdutil.h"
#include "pyesstring.h"
#include "scribuscore.h"
#include "scribusview.h"

#include <QApplication>
#include <QCursor>
#include <QProgressBar>
#include <QString>

PyObject *scribus_statusmessage(PyObject* /* self */, PyObject* args)
{
	PyESString aText;
	if (!PyArg_ParseTuple(args, "es", "utf-8", aText.ptr()))
	{
		aText.resetDontFree();
		return nullptr;
	}
	ScCore->primaryMainWindow()->setStatusBarInfoText(QString::fromUtf8(aText.c_str()));
	Py_RETURN_NONE;
}

PyObject *scribus_progressreset(PyObject* /* self */)
{
	ScCore->primaryMainWindow()->mainWindowProgressBar->reset();
	QApplication::processEvents();
	Py_RETURN_NONE;
}

PyObject *scribus_progresssettotalsteps(PyObject* /* self */, PyObject* args)
{
	int steps;
	if (!PyArg_ParseTuple(args, "i", &steps))
		return nullptr;
	ScCore->primaryMainWindow()->mainWindowProgressBar->setMaximum(steps);
	ScCore->primaryMainWindow()->mainWindowProgressBar->setValue(0);
	QApplication::processEvents();
	Py_RETURN_NONE;
}

PyObject *scribus_progresssetprogress(PyObject* /* self */, PyObject* args)
{
	int position;
	if (!PyArg_ParseTuple(args, "i", &position))
		return nullptr;
	if (position > ScCore->primaryMainWindow()->mainWindowProgressBar->maximum())
	{
		PyErr_SetString(PyExc_ValueError, QString("Tried to set progress > maximum progress").toLocal8Bit().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->mainWindowProgressBar->setValue(position);
	QApplication::processEvents();
	Py_RETURN_NONE;
}


PyObject *scribus_setcursor(PyObject* /* self */, PyObject* args)
{
	PyESString aCursor;
	qDebug("WARNING! SetCursor() is not stable!");
	if (!PyArg_ParseTuple(args, "es", "ascii", aCursor.ptr()))
	{
		aCursor.resetDontFree();
		return nullptr;
	}
	if (strcmp(aCursor.c_str(), "wait") == 0)
		QApplication::changeOverrideCursor(Qt::WaitCursor);
//	else
//		QApplication::restoreOverrideCursor();
	Py_RETURN_NONE;
}

PyObject *scribus_docchanged(PyObject* /* self */, PyObject* args)
{
	int aValue;
	if (!PyArg_ParseTuple(args, "i", &aValue))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->slotDocCh(static_cast<bool>(aValue));
	/*
	if (aValue>0)
		ScCore->primaryMainWindow()->slotDocCh(true);
	else
		ScCore->primaryMainWindow()->slotDocCh(false);*/
	Py_RETURN_NONE;
}

PyObject *scribus_zoomdocument(PyObject* /* self */, PyObject* args)
{
	double zoomFactor;
	if (!PyArg_ParseTuple(args, "d", &zoomFactor))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (zoomFactor > 0.0 || zoomFactor == -100.0)
		ScCore->primaryMainWindow()->slotZoom(zoomFactor);
	else
	{
		PyErr_SetString(PyExc_ValueError, QString("The zoom factor should be greater than 0.0 or equal to -100.0. See help(zoomFactor).").toLocal8Bit().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

/*
 * Gives the possibility to scroll the document.
 * 13.12.2007: Joachim Neu
 */
PyObject *scribus_scrolldocument(PyObject*,PyObject* args)
{
	int moveX = 0, moveY = 0;
	if (!PyArg_ParseTuple(args, "ii", &moveX, &moveY))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->view->scrollBy(moveX,moveY);
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void guiappdocwarnings()
{
	QStringList s;
	s << scribus_docchanged__doc__
	  << scribus_progressreset__doc__
	  << scribus_progresssetprogress__doc__
	  << scribus_progresssettotalsteps__doc__
	  << scribus_scrolldocument__doc__
	  << scribus_setcursor__doc__
	  << scribus_statusmessage__doc__
	  << scribus_zoomdocument__doc__;
}
