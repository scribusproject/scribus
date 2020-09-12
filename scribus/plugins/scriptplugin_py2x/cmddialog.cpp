/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmddialog.h"
#include "cmdutil.h"
#include "scribuscore.h"
#include "ui/customfdialog.h"
#include "ui/scmessagebox.h"
#include "ui/stylemanager.h"

#include <QMessageBox>
#include <QCursor>
#include <QInputDialog>


PyObject *scribus_newdocdialog(PyObject* /* self */)
{
	QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
	bool ret = ScCore->primaryMainWindow()->slotFileNew();
	QApplication::changeOverrideCursor(Qt::ArrowCursor);
	return PyInt_FromLong(static_cast<long>(ret));
}

PyObject *scribus_filedialog(PyObject* /* self */, PyObject* args, PyObject* kw)
{
	char *caption = const_cast<char*>("");
	char *filter = const_cast<char*>("");
	char *defName = const_cast<char*>("");
	int haspreview = 0;
	int issave = 0;
	int isdir = 0;
	// FIXME: parsing named params failure. e.g. fileDialog(caption="foo", issave=True)
	// FIXME: it's a bug in Python. I'm monitoring it
	// https://sourceforge.net/tracker/index.php?func=detail&aid=893549&group_id=5470&atid=105470
	char* kwargs[] = {const_cast<char*>("caption"), const_cast<char*>("filter"),
						const_cast<char*>("defaultname"), const_cast<char*>("haspreview"),
						const_cast<char*>("issave"), const_cast<char*>("isdir"),
						nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "es|esesiii", kwargs,
									 "utf-8", &caption, "utf-8", &filter, "utf-8", &defName,
									 &haspreview, &issave, &isdir))
	{
		return nullptr;
	}
	QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
	/* nobool = Nothing doing boolean for CFileDialog last attrs.
	Due the 'isdir' parameter. CFileDialog needs the last 2 pointers
	initialized. */
	bool nobool = false;
	int optionFlags = 0;
	if (haspreview)
		optionFlags |= fdShowPreview;
	if (issave)
		optionFlags |= fdExistingFiles;
	if (isdir)
		optionFlags |= fdDirectoriesOnly;
	QString fName = ScCore->primaryMainWindow()->CFileDialog(".",
										 QString::fromUtf8(caption),
										 QString::fromUtf8(filter),
										 QString::fromUtf8(defName),
										 optionFlags,
										 &nobool,
										 &nobool,
										 &nobool
										);
//	QApplication::restoreOverrideCursor();
	// FIXME: filename return unicode OK?
	return PyString_FromString(fName.toUtf8());
}

PyObject *scribus_messagebox(PyObject* /* self */, PyObject* args, PyObject* kw)
{
	char *caption = const_cast<char*>("");
	char *message = const_cast<char*>("");
	uint result;
	QMessageBox::Icon ico = QMessageBox::NoIcon;
	int butt[3] = { QMessageBox::Ok|QMessageBox::Default, QMessageBox::NoButton, QMessageBox::NoButton };
	QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(nullptr);
	enum QMessageBox::StandardButton defaultButton = QMessageBox::NoButton;
	char* kwargs[] = {const_cast<char*>("caption"), const_cast<char*>("message"),
						const_cast<char*>("icon"), const_cast<char*>("button1"),
						const_cast<char*>("button2"), const_cast<char*>("button3"), nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "eses|iiii", kwargs, "utf-8", &caption, "utf-8", &message, &ico, &butt[0], &butt[1], &butt[2]))
		return nullptr;
	QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
	// ScMessageBox mb(ico, butt1, butt2, butt3, ScCore->primaryMainWindow());
	for (int bi = 0; bi < 3; bi++) {
		enum QMessageBox::StandardButton b = static_cast<QMessageBox::StandardButton>(butt[bi]);
		if (b != QMessageBox::NoButton) {
			if ((b & QMessageBox::Default) != 0) {
				b = QMessageBox::StandardButton(b & ~QMessageBox::Default);
				defaultButton = b;
			}
			buttons |= b;
		}
	}
	ScMessageBox mb(ico, QString::fromUtf8(caption), QString::fromUtf8(message), buttons, ScCore->primaryMainWindow());
	if (defaultButton != QMessageBox::NoButton) {
		mb.setDefaultButton(defaultButton);
	}
	result = mb.exec();
//	QApplication::restoreOverrideCursor();
	return PyInt_FromLong(static_cast<long>(result));
}

PyObject *scribus_valuedialog(PyObject* /* self */, PyObject* args)
{
	char *caption = const_cast<char*>("");
	char *message = const_cast<char*>("");
	char *value = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "eses|es", "utf-8", &caption, "utf-8", &message, "utf-8", &value))
		return nullptr;
	QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
	QString txt = QInputDialog::getText(ScCore->primaryMainWindow(),
										QString::fromUtf8(caption),
										QString::fromUtf8(message),
										QLineEdit::Normal,
										QString::fromUtf8(value));
//	QApplication::restoreOverrideCursor();
	return PyString_FromString(txt.toUtf8());
}

PyObject *scribus_newstyledialog(PyObject*, PyObject* args)
{
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc *d = ScCore->primaryMainWindow()->doc;
	bool ok;
	QString s = QInputDialog::getText(ScCore->primaryMainWindow(), "New Paragraph Style",
			"Enter name of the new paragraph style:", QLineEdit::Normal,
			QString(), &ok);

	if (ok && !s.isEmpty())
	{
		StyleSet<ParagraphStyle> st;
		st.redefine(d->paragraphStyles(), true);
		ParagraphStyle p;
		p.setName(s);
		st.create(p);
		d->redefineStyles(st, false);
		ScCore->primaryMainWindow()->styleMgr()->setDoc(d);
		return PyString_FromString(s.toUtf8());
	}
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blash' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmddialogdocwarnings()
{
	QStringList s;
	s << scribus_filedialog__doc__
	  << scribus_messagebox__doc__
	  << scribus_newdocdialog__doc__ 
	  << scribus_newstyledialog__doc__
	  << scribus_valuedialog__doc__ ;
}
