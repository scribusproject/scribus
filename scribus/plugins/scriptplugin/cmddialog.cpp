#include <Python.h>
#include "cmddialog.h"
#include "cmdvar.h"
#include "cmdutil.h"
#include "valuedialog.h"
#include <qmessagebox.h>
#include <qcursor.h>

PyObject *scribus_newdocdia(PyObject *self)
{
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
	bool ret = Carrier->slotFileNew();
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//	qApp->processEvents();
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_filedia(PyObject *self, PyObject* args)
{
	char *caption;
	char *filter;
	char *defName;
	QString fName;
	int pre = 0;
	int mode = 0;
	if (!PyArg_ParseTuple(args, "sss|ii", &caption, &filter, &defName, &pre, &mode))
		return NULL;
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
	fName = Carrier->CFileDialog(".", caption, filter, defName, static_cast<bool>(pre), static_cast<bool>(mode), 0, 0);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	return PyString_FromString(fName.utf8());
}

PyObject *scribus_messdia(PyObject *self, PyObject* args)
{
	char *caption = "";
	char *message = "";
	uint result;
	QMessageBox::Icon ico = QMessageBox::NoIcon;
	int butt1 = QMessageBox::NoButton;
	int butt2 = QMessageBox::NoButton;
	int butt3 = QMessageBox::NoButton;
	if (!PyArg_ParseTuple(args, "ssii|ii", &caption, &message, &ico, &butt1, &butt2, &butt3))
		return NULL;
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
	QMessageBox mb(caption, message, ico, butt1, butt2, butt3, Carrier);
	result = mb.exec();
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	return PyInt_FromLong(static_cast<long>(result));
}

PyObject *scribus_valdialog(PyObject *self, PyObject* args)
{
	char *caption = "";
	char *message = "";
	char *value = "";
	if (!PyArg_ParseTuple(args, "ss|s", &caption, &message, &value))
		return NULL;
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
	ValueDialog *d = new ValueDialog(Carrier, "d", TRUE, 0);
	d->dialogLabel->setText(message);
	d->valueEdit->setText(value);
	d->setCaption(caption);
	d->exec();
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	return PyString_FromString(d->valueEdit->text().utf8());
}
