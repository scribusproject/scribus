/***************************************************************************
                          scriptplugin.cpp  -  description
                             -------------------
    begin                : Thu Oct 3 08:00:00 CEST 2002
    copyright            : (C) 2002 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// include cmdvar.h first, as it pulls in <Python.h>
#include "cmdvar.h"

#include "cmddialog.h"
#include "cmddoc.h"
#include "cmdpage.h"
#include "cmdobj.h"
#include "cmdgetprop.h"
#include "cmdsetprop.h"
#include "cmdtext.h"
#include "cmdmani.h"
#include "cmdcolor.h"
#include "cmdmisc.h"
#include "scriptplugin.h"
#include "scriptplugin.moc"
#include "cmdutil.h"
#include "objprinter.h"
#include "objpdffile.h"
#include "guiapp.h"
#include "customfdialog.h"
#include "helpbrowser.h"
#include "mpalette.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qdom.h>
#include <qtextstream.h>
#include <cstdlib>

#include <iostream>

// Exceptions; visible from cmdvar.h, set up in initscribus()
PyObject* ScribusException;
PyObject* NoDocOpenError;
PyObject* WrongFrameTypeError;
PyObject* NoValidObjectError;
PyObject* NotFoundError;
PyObject* NameExistsError;

QString Name()
{
	return QObject::tr("S&cripter Manual...");
}

int Type()
{
	return 4;
}

int ID()
{
	return 8;
}

void InitPlug(QWidget *d, ScribusApp *plug)
{
	QString cm;
	Py_Initialize();
	if (PyUnicode_SetDefaultEncoding("utf-8"))
	{
		qDebug("Failed to set default encoding to utf-8.\n");
		PyErr_Clear();
	}
	Carrier = plug;
	RetVal = 0;
	initscribus(Carrier);
	Tes = new MenuTest(d);
	men = new QPopupMenu();
	Tes->rmen = new QPopupMenu();
	Tes->smen = new QPopupMenu();
	Tes->SavedRecentScripts.clear();
	Tes->ReadPlugPrefs();
	QString pfad = SCRIPTSDIR;
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad);
	QDir ds(pfad2, "*.py", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((ds.exists()) && (ds.count() != 0))
	{
		for (uint dc = 0; dc < ds.count(); ++dc)
		{
			QFileInfo fs(ds[dc]);
			Tes->smen->insertItem(fs.baseName(true));
		}
	}
	Tes->RecentScripts.clear();
	if (Tes->SavedRecentScripts.count() != 0)
	{
		uint max = QMIN(Carrier->Prefs.RecentDCount, Tes->SavedRecentScripts.count());
		for (uint m = 0; m < max; ++m)
		{
			QFileInfo fd(Tes->SavedRecentScripts[m]);
			if (fd.exists())
			{
				Tes->RecentScripts.append(Tes->SavedRecentScripts[m]);
				Tes->rmen->insertItem(Tes->SavedRecentScripts[m]);
			}
		}
	}
	Tes->pcon = new PConsole(d);
	Tes->smenid = men->insertItem(QObject::tr("&Scribus Scripts"), Tes->smen);
	men->insertItem(QObject::tr("&Execute Script..."), Tes, SLOT(slotTest()));
	Tes->rmenid = men->insertItem(QObject::tr("&Recent Scripts"), Tes->rmen);
	men->insertSeparator();
	Tes->cons = men->insertItem(QObject::tr("Show &Console"), Tes, SLOT(slotInteractiveScript()));
	Tes->about = men->insertItem(QObject::tr("&About Script..."), Tes, SLOT(aboutScript()));
	plug->menuBar()->insertItem(QObject::tr("S&cript"), men, -1, plug->menuBar()->count() - 2);
	QObject::connect(Tes->pcon->OutWin, SIGNAL(returnPressed()), Tes, SLOT(slotExecute()));
	QObject::connect(Tes->pcon, SIGNAL(Schliessen()), Tes, SLOT(slotInteractiveScript()));
	QObject::connect(Tes->rmen, SIGNAL(activated(int)), Tes, SLOT(RecentScript(int)));
	QObject::connect(Tes->smen, SIGNAL(activated(int)), Tes, SLOT(StdScript(int)));
}

void CleanUpPlug()
{
	Py_Finalize();
	Tes->SavePlugPrefs();
}

void Run(QWidget *d, ScribusApp *plug)
{
	QString pfad = DOCDIR;
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad + "en/Scripter/index.html");
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("Online Reference"), Carrier->getGuiLanguage(), "scripter");
	dia->show();
}


void MenuTest::FinishScriptRun()
{
	if (Carrier->HaveDoc)
	{
		Carrier->Mpal->SetDoc(Carrier->doc);
		Carrier->Mpal->updateCList();
		Carrier->Mpal->Spal->setFormats(Carrier->doc);
		Carrier->Mpal->SetLineFormats(Carrier->doc);
		Carrier->Mpal->Cpal->SetColors(Carrier->doc->PageColors);
		Carrier->Lpal->setLayers(&Carrier->doc->Layers, &Carrier->doc->ActiveLayer);
		Carrier->Tpal->BuildTree(Carrier->view);
		Carrier->Sepal->SetView(Carrier->view);
		Carrier->Sepal->Rebuild();
		if (Carrier->view->SelItem.count() != 0)
			Carrier->HaveNewSel(Carrier->view->SelItem.at(0)->PType);
		else
			Carrier->HaveNewSel(-1);
		Carrier->view->DrawNew();
	}
}

void MenuTest::slotTest()
{
	QString fileName;
	QString CurDirP = QDir::currentDirPath();
	QString scriptDir = Carrier->Prefs.ScriptDir;
	if (scriptDir == "")
		scriptDir = CurDirP;
	CustomFDialog diaf((QWidget*)parent(), scriptDir, QObject::tr("Open"), QObject::tr("Python Scripts (*.py);; All Files (*)"));
	if (diaf.exec())
	{
		fileName = diaf.selectedFile();
		slotRunScriptFile(fileName);
		rmen->clear();
		if (RecentScripts.findIndex(fileName) == -1)
			RecentScripts.prepend(fileName);
		else
		{
			RecentScripts.remove(fileName);
			RecentScripts.prepend(fileName);
		}
		uint max = QMIN(Carrier->Prefs.RecentDCount, RecentScripts.count());
		for (uint m = 0; m < max; m++)
		{
			rmen->insertItem(RecentScripts[m]);
		}
	}
	QDir::setCurrent(CurDirP);
	FinishScriptRun();
}

void MenuTest::StdScript(int id)
{
	QString pfad = SCRIPTSDIR;
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad);
	QString fn = pfad2+smen->text(id)+".py";
	QFileInfo fd(fn);
	if (!fd.exists())
		return;
	slotRunScriptFile(fn);
	FinishScriptRun();
}

void MenuTest::RecentScript(int id)
{
	QString fn = rmen->text(id);
	QFileInfo fd(fn);
	if (!fd.exists())
	{
		RecentScripts.remove(fn);
		rmen->clear();
		uint max = QMIN(Carrier->Prefs.RecentDCount, RecentScripts.count());
		for (uint m = 0; m < max; m++)
		{
			rmen->insertItem(RecentScripts[m]);
		}
		return;
	}
	slotRunScriptFile(fn);
	FinishScriptRun();
}

void MenuTest::slotRunScriptFile(QString fileName)
{
	Carrier->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	char* comm[1];
	QFileInfo fi(fileName);
	QCString na = fi.fileName().latin1();
	QDir::setCurrent(fi.dirPath(true));
	PyThreadState *stateo = PyEval_SaveThread();
	PyThreadState *state = Py_NewInterpreter();
	initscribus(Carrier);
	comm[0] = na.data();
	// call python script
	PySys_SetArgv(1, comm);
	PyObject* m = PyImport_AddModule("__main__");
	if (m == NULL)
		qDebug("Failed to get __main__ - aborting script");
	else
	{
		// FIXME: If filename contains chars outside 7bit ascii, might be problems
		PyObject* globals = PyModule_GetDict(m);
		// Build the Python code to run the script
		QString cm = QString("import sys,StringIO,traceback\n");
		cm        += QString("sys.path[0] = \"%1\"\n").arg(fi.dirPath(true));
		cm        += QString("try:\n");
		cm        += QString("    execfile(\"%1\")\n").arg(fileName);
		cm        += QString("except SystemExit:\n");
		cm        += QString("    pass\n");
		// Capture the text of any other exception that's raised by the interpreter
		// into a StringIO buffer for later extraction.
		cm        += QString("except Exception, err:\n");
		cm        += QString("    f=StringIO.StringIO()\n");
		cm        += QString("    traceback.print_exc(file=f)\n");
		cm        += QString("    errorMsg = f.getvalue()\n");
		cm        += QString("    del(f)\n");
		// We re-raise the exception so the return value of PyRun_String reflects
		// the fact that an exception has ocurred.
		cm        += QString("    raise\n");
		// FIXME: if cmd contains chars outside 7bit ascii, might be problems
		QCString cmd = cm.latin1();
		// Now run the script in the interpreter's global scope
		PyObject* result = PyRun_String(cmd.data(), Py_file_input, globals, globals);
		// NULL is returned if an exception is set. We don't care about any
		// other return value (most likely None anyway) and can ignore it.
		if (result == NULL)
		{
			PyObject* errorMsgPyStr = PyMapping_GetItemString(globals, "errorMsg");
			if (errorMsgPyStr == NULL)
			{
				// It's rather unlikely that this will ever be reached - to get here
				// we'd have to fail to retrive the string we just created.
				qDebug("Error retrieving error message content after script exception!");
				qDebug("Exception was:");
				PyErr_Print();
			}
			else
			{
				QString errorMsg = PyString_AsString(errorMsgPyStr);
				// Display a dialog to the user with the exception
				QClipboard *cp = QApplication::clipboard();
				cp->setText(errorMsg);
				QMessageBox::warning(Carrier,
									tr("Script error"),
									tr("If you are running an official script report it at <a href=\"http://bugs.scribus.net\">bugs.scribus.net</a> please.")
									+ "<pre>" +errorMsg + "</pre>"
									+ tr("This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker."));
			}
		} // end if result == NULL
		// Because 'result' may be NULL, not a PyObject*, we must call PyXDECREF not Py_DECREF
		Py_XDECREF(result);
	} // end if m == NULL
	Py_EndInterpreter(state);
	PyEval_RestoreThread(stateo);
	Carrier->ScriptRunning = false;
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
}

QString MenuTest::slotRunScript(QString Script)
{
	Carrier->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	char* comm[1];
	QString cm;
	InValue = Script;
	QString CurDir = QDir::currentDirPath();
	if(PyThreadState_Get() != NULL)
	{
		initscribus(Carrier);
		if (RetVal == 0)
		{
			// FIXME: if CurDir contains chars outside 7bit ascii, might be problems
			cm = "import sys\nsys.path[0] = \""+CurDir+"\"\n";
			cm += "import cStringIO\n";
			cm += "from scribus import *\n";
			cm += "bu = cStringIO.StringIO()\n";
			cm += "sys.stdout = bu\n";
			cm += "sys.stderr = bu\n";
			cm += "import code\n";
			cm += "ia = code.InteractiveConsole(globals())\n";
		}
		cm += "sc = getval()\n";
		cm += "rv = ia.push(sc)\n";
		cm += "if rv == 1:\n";
		cm += "\tre = \"...\"\n";
		cm += "else:\n";
		cm += "\tre = bu.getvalue()\n";
		cm += "retval(re, rv)\n";
	}
	// FIXME: if cmd contains chars outside 7bit ascii, might be problems
	QCString cmd = cm.latin1();
	comm[0] = "scribus";
	PySys_SetArgv(1, comm);
	PyRun_SimpleString(cmd.data());
	if (RetVal == 0)
	{
		RetString += ">>>";
		pcon->OutWin->Prompt = ">>>";
	}
	else
		pcon->OutWin->Prompt = "...";
	Carrier->ScriptRunning = false;
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	return RetString;
}

void MenuTest::slotInteractiveScript()
{
	if (pcon->isVisible())
	{
		men->changeItem(cons, tr("Show &Console"));
		pcon->hide();
	}
	else
	{
		men->changeItem(cons, tr("Hide &Console"));
		pcon->show();
	}
}

void MenuTest::slotExecute()
{
	pcon->OutWin->append(slotRunScript(pcon->OutWin->LastComm));
	pcon->OutWin->moveCursor(QTextEdit::MoveEnd, false);
	pcon->OutWin->scrollToBottom();
	pcon->OutWin->ensureCursorVisible();
	FinishScriptRun();
}

void MenuTest::ReadPlugPrefs()
{
	QDomDocument docu("scriptrc");
	QString ho = QDir::homeDirPath();
	QFile f(QDir::convertSeparators(ho+"/.scribus/scripter.rc"));
	if(!f.open(IO_ReadOnly))
		return;
	if(!docu.setContent(&f))
	{
		f.close();
		return;
	}
	f.close();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIPTRC")
		return;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		if (dc.tagName()=="RECENT")
			SavedRecentScripts.append(dc.attribute("NAME"));
		DOC=DOC.nextSibling();
	}
}

void MenuTest::SavePlugPrefs()
{
	QDomDocument docu("scriptrc");
	QString st="<SCRIPTRC></SCRIPTRC>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	for (uint rd=0; rd < Tes->RecentScripts.count(); ++rd)
	{
		QDomElement rde=docu.createElement("RECENT");
		rde.setAttribute("NAME",Tes->RecentScripts[rd]);
		elem.appendChild(rde);
	}
	QString ho = QDir::homeDirPath();
	QFile f(QDir::convertSeparators(ho+"/.scribus/scripter.rc"));
	if(!f.open(IO_WriteOnly))
		return;
	QTextStream s(&f);
	s<<docu.toCString();
	f.close();
}

/* 11/1/2004 pv - Show docstring of the script to the user.
 * I don't know how to get docstring via e.g. pydoc because of
 * it needs to run script => error cannot find scribus module
 */
void MenuTest::aboutScript()
{
	QString fname = Carrier->CFileDialog(".", "about", "Scripts (*.py)", "", 0, 0, 0, 0);
	QFileInfo fi = QFileInfo(fname);
	QString html = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/aboutScript.html");
	QFile input(fname);
	if(!input.open(IO_ReadOnly))
		return;
	QFile output(html);
	if(!output.open(IO_WriteOnly))
		return;
	QTextStream intputstream(&input);
	QTextStream outputstream(&output);
	QString content = intputstream.read();
	QString docstring = content.section("\"\"\"", 1, 1);
	if (docstring != "")
	{
		outputstream << "<h1>Documentation for: " << fi.fileName() << "</h1><p>";
		outputstream << docstring.replace("\n\n", "<p>");
	}
	else
	{
		outputstream << "<pre>" << endl;
		outputstream << "<p><b>Script "<< fi.fileName() << " doesn't contain any docstring!</b></p>" << content;
		outputstream << "</pre>" << endl;
	}
	output.close();
	input.close();
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("About Script") + " " + fi.fileName(), "en", "", html);
	dia->show();
}

// This function builds a Python wrapper function called newName around the
// python function called oldName. The wrapper function prints a warning, then
// calls oldName with all passed arguments and returns the result from oldName.
// The wrapper is stored in the module dictionary passed, so it appears in the
// `scribus' module and will be imported by 'from scribus import'. A docstring
// is provided to direct the user to the correct function.
// By default the warning gets output only on the first use of the function in a given
// interpreter instance, but user scripts can change this.
void deprecatedFunctionAlias(PyObject* scribusdict, char* oldName, char* newName)
{
	// Build the Python code to create the wrapper function
	QString wrapperFunc = "";
	wrapperFunc += QString("def %1(*args, **kwargs):\n").arg(newName);
	wrapperFunc += QString("    \"\"\"Deprecated alias for function %1 - see help(%2).\"\"\"\n").arg(oldName).arg(oldName);
	wrapperFunc += QString("    warnings.warn(\"Warning, script function %1 is deprecated, use %2 instead.\\n\",exceptions.DeprecationWarning)\n").arg(newName).arg(oldName);
	wrapperFunc += QString("    return %1(*args,**kwargs)\n").arg(oldName);
	QCString wsData = wrapperFunc.latin1();	//this should probably be utf8 now
	// And run it in the namespace of the scribus module
	PyObject* result = PyRun_String(wsData, Py_file_input, scribusdict, scribusdict);
	// NULL is returned if an exception is set. We don't care about any other return value and
	// can ignore it.
	if (result == NULL)
	{
		qDebug("Failed to alias %s to %s in Python scripter - exception raised!", oldName, newName);
		PyErr_Print();
	}
	// Because 'result' may be NULL, not a PyObject*, we must call PyXDECREF not Py_DECREF
	Py_XDECREF(result);
}

// Copy constants by coping the reference to them. Equivalent to
// scribus.__dict__[newName] = scribus.__dict__[oldName].
void constantAlias(PyObject* scribusdict, const char* oldName, const char* newName)
{
	/* Work around the braindead Python/C API not using 'const' for
	 * strings it won't change. */
	char* newNameTemp = strndup(newName, strlen(newName));
	char* oldNameTemp = strndup(oldName, strlen(oldName));
	assert(newNameTemp != NULL);
	assert(oldNameTemp != NULL);
	/* We use PyMapping_GetItemString because unlike PyDict_GetItemString it
	 * returns a new (not borrowed) reference, while PyDict_SetItemString
	 * consumes the reference. That means we don't have to Py_INCREF anything.
	 */
	PyObject* item = PyMapping_GetItemString(scribusdict, oldNameTemp);
	assert(item != NULL);
	PyDict_SetItemString(scribusdict, newNameTemp, item);
	free(newNameTemp);
	free(oldNameTemp);
}

/****************************************************************************************/
/*                                                                                      */
/*   Definitions of the Python commands                                                 */
/*                                                                                      */
/****************************************************************************************/

static PyObject *scribus_retval(PyObject *self, PyObject* args)
{
	char *Name = NULL;
	int retV = 0;
	if (!PyArg_ParseTuple(args, "si", &Name, &retV))
		return NULL;
	// Because sysdefaultencoding is not utf-8, Python is returning utf-8 encoded
	// 8-bit char* strings. Make sure Qt understands that the input is utf-8 not
	// the default local encoding (usually latin-1) by using QString::fromUtf8()
	RetString = QString::fromUtf8(Name);
	RetVal = retV;
	return PyInt_FromLong(0L);
}

static PyObject *scribus_getval(PyObject *self)
{
	return PyString_FromString(InValue.utf8().data());
}

/*!
 * Translate a docstring. Small helper function for use with the
 * PyMethodDef struct.
 */
char* tr(const char* docstringConstant)
{
	// Alas, there's a lot of wasteful string copying going on
	// here.
	QString translated = QObject::tr(docstringConstant, "scripter docstring");
	/*
	 * Python doesn't support 'unicode' object docstrings in the PyMethodDef,
	 * and has no way to specify what encoding docstrings are in. The passed C
	 * strings passed are made into 'str' objects as-is. These are interpreted
	 * as being in the Python sysdefaultencoding, usually 'ascii', when used.
	 * We now set systemdefaultencoding to 'utf-8' ...  so we're going to pass
	 * Python an 8-bit utf-8 encoded string in a char* .  With
	 * sysdefaultencoding set correctly, Python will interpret it correctly and
	 * we'll have our unicode docstrings. It's not as ugly a hack as it sounds,
	 * you just have to remember that C and Python strings can both be
	 * considered 8-bit strings of binary data that can be later interpreted as
	 * a text string in a particular text encoding.
	 */
	//QCString utfTranslated = translated.utf8();
	const char* trch = translated.utf8().data();
	return strndup(trch, strlen(trch));
}

/* Now we're using the more pythonic convention for names:
 * class - ClassName
 * procedure/function/method - procedureName
 * etc. */
PyMethodDef scribus_methods[] = {
	// 2004/10/03 pv - aliases with common Python syntax - ClassName methodName
	// 2004-11-06 cr - move aliasing to dynamically generated wrapper functions, sort methoddef
	{"changeColor", scribus_setcolor, METH_VARARGS, tr(scribus_setcolor__doc__)},
	{"closeDoc", (PyCFunction)scribus_closedoc, METH_NOARGS, tr(scribus_closedoc__doc__)},
	{"createBezierLine", scribus_bezierline, METH_VARARGS, tr(scribus_bezierline__doc__)},
	{"createEllipse", scribus_newellipse, METH_VARARGS, tr(scribus_newellipse__doc__)},
	{"createImage", scribus_newimage, METH_VARARGS, tr(scribus_newimage__doc__)},
	{"createLayer", scribus_createlayer, METH_VARARGS, tr(scribus_createlayer__doc__)},
	{"createLine", scribus_newline, METH_VARARGS, tr(scribus_newline__doc__)},
	{"createPathText", scribus_pathtext, METH_VARARGS, tr(scribus_pathtext__doc__)},
	{"createPolygon", scribus_polygon, METH_VARARGS, tr(scribus_polygon__doc__)},
	{"createPolyLine", scribus_polyline, METH_VARARGS, tr(scribus_polyline__doc__)},
	{"createRect", scribus_newrect, METH_VARARGS, tr(scribus_newrect__doc__)},
	{"createText", scribus_newtext, METH_VARARGS, tr(scribus_newtext__doc__)},
	{"currentPage", (PyCFunction)scribus_actualpage, METH_NOARGS, tr(scribus_actualpage__doc__)},
	{"defineColor", scribus_newcolor, METH_VARARGS, tr(scribus_newcolor__doc__)},
	{"deleteColor", scribus_delcolor, METH_VARARGS, tr(scribus_delcolor__doc__)},
	{"deleteLayer", scribus_removelayer, METH_VARARGS, tr(scribus_removelayer__doc__)},
	{"deleteObject", scribus_deleteobj, METH_VARARGS, tr(scribus_deleteobj__doc__)},
	{"deletePage", scribus_deletepage, METH_VARARGS, tr(scribus_deletepage__doc__)},
	{"deleteText", scribus_deletetext, METH_VARARGS, tr(scribus_deletetext__doc__)},
	{"deselectAll", (PyCFunction)scribus_deselect, METH_NOARGS, tr(scribus_deselect__doc__)},
	{"docChanged", scribus_docchanged, METH_VARARGS, tr(scribus_docchanged__doc__)},
	{"fileDialog", (PyCFunction)scribus_filedia, METH_VARARGS|METH_KEYWORDS, tr(scribus_filedia__doc__)},
	{"getActiveLayer", (PyCFunction)scribus_getactlayer, METH_NOARGS, tr(scribus_getactlayer__doc__)},
	{"getAllObjects", scribus_getallobj, METH_VARARGS, tr(scribus_getallobj__doc__)},
	{"getAllStyles", (PyCFunction)scribus_getstylenames, METH_NOARGS, tr(scribus_getstylenames__doc__)},
	{"getAllText", scribus_gettext, METH_VARARGS, tr(scribus_gettext__doc__)},
	{"getColorNames", (PyCFunction)scribus_colornames, METH_NOARGS, tr(scribus_colornames__doc__)},
	{"getColor", scribus_getcolor, METH_VARARGS, tr(scribus_getcolor__doc__)},
	{"getColumnGap", scribus_getcolumngap, METH_VARARGS, tr(scribus_getcolumngap__doc__)},
	{"getColumns", scribus_getcolumns, METH_VARARGS, tr(scribus_getcolumns__doc__)},
	{"getCornerRadius", scribus_getcornerrad, METH_VARARGS, tr(scribus_getcornerrad__doc__)},
	{"getFillColor", scribus_getfillcolor, METH_VARARGS, tr(scribus_getfillcolor__doc__)},
	{"getFillShade", scribus_getfillshade, METH_VARARGS, tr(scribus_getfillshade__doc__)},
	{"getFontNames", (PyCFunction)scribus_fontnames, METH_NOARGS, tr(scribus_fontnames__doc__)},
	{"getFont", scribus_getfont, METH_VARARGS, tr(scribus_getfont__doc__)},
	{"getFontSize", scribus_getfontsize, METH_VARARGS, tr(scribus_getfontsize__doc__)},
	{"getGuiLanguage", (PyCFunction)scribus_getlanguage, METH_NOARGS, tr(scribus_getlanguage__doc__)},
	{"getHGuides", (PyCFunction)scribus_getHguides, METH_NOARGS, tr(scribus_getHguides__doc__)},
	{"getImageFile", scribus_getimgname, METH_VARARGS, tr(scribus_getimgname__doc__)},
	{"getImageScale", scribus_getimgscale, METH_VARARGS, tr(scribus_getimgscale__doc__)},
	{"getLayers", (PyCFunction)scribus_getlayers, METH_NOARGS, tr(scribus_getlayers__doc__)},
	{"getLineCap", scribus_getlineend, METH_VARARGS, tr(scribus_getlineend__doc__)},
	{"getLineColor", scribus_getlinecolor, METH_VARARGS, tr(scribus_getlinecolor__doc__)},
	{"getLineJoin", scribus_getlinejoin, METH_VARARGS, tr(scribus_getlinejoin__doc__)},
	{"getLineShade", scribus_getlineshade, METH_VARARGS, tr(scribus_getlineshade__doc__)},
	{"getLineSpacing", scribus_getlinespace, METH_VARARGS, tr(scribus_getlinespace__doc__)},
	{"getLineStyle", scribus_getlinestyle, METH_VARARGS, tr(scribus_getlinestyle__doc__)},
	{"getLineWidth", scribus_getlinewidth, METH_VARARGS, tr(scribus_getlinewidth__doc__)},
	{"getPageItems", (PyCFunction)scribus_getpageitems, METH_NOARGS, tr(scribus_getpageitems__doc__)},
	{"getPageMargins", (PyCFunction)scribus_getpagemargins, METH_NOARGS, tr(scribus_getpagemargins__doc__)},
	{"getPageSize", (PyCFunction)scribus_pagedimension, METH_NOARGS, tr(scribus_pagedimension__doc__)}, // just an alias to PageDimension()
	{"getPosition", scribus_getposi, METH_VARARGS, tr(scribus_getposi__doc__)},
	{"getRotation", scribus_getrotation, METH_VARARGS, tr(scribus_getrotation__doc__)},
	{"getSelectedObject", scribus_getselobjnam, METH_VARARGS, tr(scribus_getselobjnam__doc__)},
	{"getSize", scribus_getsize, METH_VARARGS, tr(scribus_getsize__doc__)},
	{"getTextColor", scribus_getlinecolor, METH_VARARGS, tr(scribus_getlinecolor__doc__)},
	{"getTextLength", scribus_gettextsize, METH_VARARGS, tr(scribus_gettextsize__doc__)},
	{"getText", scribus_getframetext, METH_VARARGS, tr(scribus_getframetext__doc__)},
	{"getTextShade", scribus_getlineshade, METH_VARARGS, tr(scribus_getlineshade__doc__)},
	{"getUnit", (PyCFunction)scribus_getunit, METH_NOARGS, tr(scribus_getunit__doc__)},
	{"getVGuides", (PyCFunction)scribus_getVguides, METH_NOARGS, tr(scribus_getVguides__doc__)},
	{"getXFontNames", (PyCFunction)scribus_xfontnames, METH_NOARGS, tr(scribus_xfontnames__doc__)},
	{"gotoPage", scribus_gotopage, METH_VARARGS, tr(scribus_gotopage__doc__)},
	{"groupObjects", scribus_groupobj, METH_VARARGS, tr(scribus_groupobj__doc__)},
	{"haveDoc", (PyCFunction)scribus_havedoc, METH_NOARGS, tr(scribus_havedoc__doc__)},
	{"insertText", scribus_inserttext, METH_VARARGS, tr(scribus_inserttext__doc__)},
	{"isLayerPrintable", scribus_glayerprint, METH_VARARGS, tr(scribus_glayerprint__doc__)},
	{"isLayerVisible", scribus_glayervisib, METH_VARARGS, tr(scribus_glayervisib__doc__)},
	{"isLocked", scribus_islocked, METH_VARARGS, tr(scribus_islocked__doc__)},
	{"linkTextFrames", scribus_linktextframes, METH_VARARGS, tr(scribus_linktextframes__doc__)},
	{"loadImage", scribus_loadimage, METH_VARARGS, tr(scribus_loadimage__doc__)},
	{"loadStylesFromFile", scribus_loadstylesfromfile, METH_VARARGS, tr(scribus_loadstylesfromfile__doc__)},
	{"lockObject", scribus_lockobject, METH_VARARGS, tr(scribus_lockobject__doc__)},
	{"messagebarText", scribus_messagebartext, METH_VARARGS, tr(scribus_messagebartext__doc__)},
	{"messageBox", (PyCFunction)scribus_messdia, METH_VARARGS|METH_KEYWORDS, tr(scribus_messdia__doc__)},
	{"moveObjectAbs", scribus_moveobjabs, METH_VARARGS, tr(scribus_moveobjabs__doc__)},
	{"moveObject", scribus_moveobjrel, METH_VARARGS, tr(scribus_moveobjrel__doc__)},
	{"newDocDialog", (PyCFunction)scribus_newdocdia, METH_NOARGS, tr(scribus_newdocdia__doc__)},
	{"newDoc", scribus_newdoc, METH_VARARGS, tr(scribus_newdoc__doc__)},
	{"newPage", scribus_newpage, METH_VARARGS, tr(scribus_newpage__doc__)},
	{"objectExists",scribus_objectexists, METH_VARARGS, tr(scribus_objectexists__doc__)},
	{"openDoc", scribus_opendoc, METH_VARARGS, tr(scribus_opendoc__doc__)},
	{"pageCount", (PyCFunction)scribus_pagecount, METH_NOARGS, tr(scribus_pagecount__doc__)},
	{"pageDimension", (PyCFunction)scribus_pagedimension, METH_NOARGS, tr(scribus_pagedimension__doc__)},
	{"progressReset", (PyCFunction)scribus_progressreset, METH_NOARGS, tr(scribus_progressreset__doc__)},
	{"progressSet", scribus_progresssetprogress, METH_VARARGS, tr(scribus_progresssetprogress__doc__)},
	{"progressTotal", scribus_progresssettotalsteps, METH_VARARGS, tr(scribus_progresssettotalsteps__doc__)},
	{"redrawAll", (PyCFunction)scribus_redraw, METH_NOARGS, tr(scribus_redraw__doc__)},
	{"renderFont", scribus_renderfont, METH_VARARGS, tr(scribus_renderfont__doc__)},
	{"replaceColor", scribus_replcolor, METH_VARARGS, tr(scribus_replcolor__doc__)},
	{"rotateObjectAbs", scribus_rotobjabs, METH_VARARGS, tr(scribus_rotobjabs__doc__)},
	{"rotateObject", scribus_rotobjrel, METH_VARARGS, tr(scribus_rotobjrel__doc__)},
	{"saveDocAs", scribus_savedocas, METH_VARARGS, tr(scribus_savedocas__doc__)},
	{"saveDoc", (PyCFunction)scribus_savedoc, METH_NOARGS, tr(scribus_savedoc__doc__)},
	{"savePageAsEPS", scribus_savepageeps, METH_VARARGS, tr(scribus_savepageeps__doc__)},
	{"scaleGroup", scribus_scalegroup, METH_VARARGS, tr(scribus_scalegroup__doc__)},
	{"scaleImage", scribus_scaleimage, METH_VARARGS, tr(scribus_scaleimage__doc__)},
	{"selectionCount", (PyCFunction)scribus_selcount, METH_NOARGS, tr(scribus_selcount__doc__)},
	{"selectObject", scribus_selectobj, METH_VARARGS, tr(scribus_selectobj__doc__)},
	{"selectText", scribus_selecttext, METH_VARARGS, tr(scribus_selecttext__doc__)},
	{"sentToLayer", scribus_senttolayer, METH_VARARGS, tr(scribus_senttolayer__doc__)},
	{"setActiveLayer", scribus_setactlayer, METH_VARARGS, tr(scribus_setactlayer__doc__)},
	{"setColumnGap", scribus_setcolumngap, METH_VARARGS, tr(scribus_setcolumngap__doc__)},
	{"setColumns", scribus_setcolumns, METH_VARARGS, tr(scribus_setcolumns__doc__)},
	{"setCornerRadius", scribus_setcornerrad, METH_VARARGS, tr(scribus_setcornerrad__doc__)},
	{"setCursor", scribus_setcursor, METH_VARARGS, tr(scribus_setcursor__doc__)},
	{"setDocType", scribus_setdoctype, METH_VARARGS, tr(scribus_setdoctype__doc__)},
	{"setFillColor", scribus_setfillcolor, METH_VARARGS, tr(scribus_setfillcolor__doc__)},
	{"setFillShade", scribus_setfillshade, METH_VARARGS, tr(scribus_setfillshade__doc__)},
	{"setFont", scribus_setfont, METH_VARARGS, tr(scribus_setfont__doc__)},
	{"setFontSize", scribus_setfontsize, METH_VARARGS, tr(scribus_setfontsize__doc__)},
	{"setGradientFill", scribus_setgradfill, METH_VARARGS, tr(scribus_setgradfill__doc__)},
	{"setHGuides", scribus_setHguides, METH_VARARGS, tr(scribus_setHguides__doc__)},
	{"setInfo", scribus_setinfo, METH_VARARGS, tr(scribus_setinfo__doc__)},
	{"setLayerPrintable", scribus_layerprint, METH_VARARGS, tr(scribus_layerprint__doc__)},
	{"setLayerVisible", scribus_layervisible, METH_VARARGS, tr(scribus_layervisible__doc__)},
	{"setLineCap", scribus_setlineend, METH_VARARGS, tr(scribus_setlineend__doc__)},
	{"setLineColor", scribus_setlinecolor, METH_VARARGS, tr(scribus_setlinecolor__doc__)},
	{"setLineJoin", scribus_setlinejoin, METH_VARARGS, tr(scribus_setlinejoin__doc__)},
	{"setLineShade", scribus_setlineshade, METH_VARARGS, tr(scribus_setlineshade__doc__)},
	{"setLineSpacing", scribus_setlinespace, METH_VARARGS, tr(scribus_setlinespace__doc__)},
	{"setLineStyle", scribus_setlinestyle, METH_VARARGS, tr(scribus_setlinestyle__doc__)},
	{"setLineWidth", scribus_setlinewidth, METH_VARARGS, tr(scribus_setlinewidth__doc__)},
	{"setMargins", scribus_setmargins, METH_VARARGS, tr(scribus_setmargins__doc__)},
	{"setMultiLine", scribus_setmultiline, METH_VARARGS, tr(scribus_setmultiline__doc__)},
	// duplicity? {"setMultiLine", scribus_setmultiline, METH_VARARGS, "TODO: docstring"},
	{"setRedraw", scribus_setredraw, METH_VARARGS, tr(scribus_setredraw__doc__)},
	// missing? {"setSelectedObject", scribus_setselobjnam, METH_VARARGS, "Returns the Name of the selecteted Object. \"nr\" if given indicates the Number of the selected Object, e.g. 0 means the first selected Object, 1 means the second selected Object and so on."},
	{"setStyle", scribus_setstyle, METH_VARARGS, tr(scribus_setstyle__doc__)},
	{"setTextAlignment", scribus_setalign, METH_VARARGS, tr(scribus_setalign__doc__)},
	{"setTextColor", scribus_settextfill, METH_VARARGS, tr(scribus_settextfill__doc__)},
	{"setText", scribus_setboxtext, METH_VARARGS, tr(scribus_setboxtext__doc__)},
	{"setTextShade", scribus_settextshade, METH_VARARGS, tr(scribus_settextshade__doc__)},
	{"setTextStroke", scribus_settextstroke, METH_VARARGS, tr(scribus_settextstroke__doc__)},
	{"setUnit", scribus_setunit, METH_VARARGS, tr(scribus_setunit__doc__)},
	{"setVGuides", scribus_setVguides, METH_VARARGS, tr(scribus_setVguides__doc__)},
	{"sizeObject", scribus_sizeobjabs, METH_VARARGS, tr(scribus_sizeobjabs__doc__)},
	{"statusMessage", scribus_messagebartext, METH_VARARGS, tr(scribus_messagebartext__doc__)},
	{"textFlowsAroundFrame", scribus_textflow, METH_VARARGS, tr(scribus_textflow__doc__)},
	{"traceText", scribus_tracetext, METH_VARARGS, tr(scribus_tracetext__doc__)},
	{"unGroupObject", scribus_ungroupobj, METH_VARARGS, tr(scribus_ungroupobj__doc__)},
	{"unlinkTextFrames", scribus_unlinktextframes, METH_VARARGS, tr(scribus_unlinktextframes__doc__)},
	{"valueDialog", scribus_valdialog, METH_VARARGS, tr(scribus_valdialog__doc__)},
	// end of aliases
	{"retval", scribus_retval, METH_VARARGS, "TODO: docstring"},
	{"getval", (PyCFunction)scribus_getval, METH_NOARGS, "TODO: docstring"},
	{"frametype", (PyCFunction)scribus_getframetype, METH_VARARGS|METH_KEYWORDS, "Return the internal type ID of the frame\n"},
	{NULL,		NULL}		/* sentinel */
};

void initscribus(ScribusApp *pl)
{
	PyObject *m, *d;
	PyImport_AddModule("scribus");
	PyType_Ready(&Printer_Type);
	PyType_Ready(&PDFfile_Type);
	m = Py_InitModule("scribus", scribus_methods);
	Py_INCREF(&Printer_Type);
	PyModule_AddObject(m, "Printer", (PyObject *) &Printer_Type);
	Py_INCREF(&PDFfile_Type);
	PyModule_AddObject(m, "PDFfile", (PyObject *) &PDFfile_Type);
	d = PyModule_GetDict(m);

	// Set up the module exceptions
	// common exc.
	ScribusException = PyErr_NewException("scribus.ScribusException", NULL, NULL);
	Py_INCREF(ScribusException);
	PyModule_AddObject(m, "ScribusException", ScribusException);
	// no doc open
	NoDocOpenError = PyErr_NewException("scribus.NoDocOpenError", ScribusException, NULL);
	Py_INCREF(NoDocOpenError);
	PyModule_AddObject(m, "NoDocOpenError", NoDocOpenError);
	// wrong type of frame for operation
	WrongFrameTypeError = PyErr_NewException((char*)"scribus.WrongFrameTypeError", ScribusException, NULL);
	Py_INCREF(WrongFrameTypeError);
	PyModule_AddObject(m, (char*)"WrongFrameTypeError", WrongFrameTypeError);
	// Couldn't find named object, or no named object and no selection
	NoValidObjectError = PyErr_NewException((char*)"scribus.NoValidObjectError", ScribusException, NULL);
	Py_INCREF(NoValidObjectError);
	PyModule_AddObject(m, (char*)"NoValidObjectError", NoValidObjectError);
	// Couldn't find the specified resource - font, color, etc.
	NotFoundError = PyErr_NewException((char*)"scribus.NotFoundError", ScribusException, NULL);
	Py_INCREF(NotFoundError);
	PyModule_AddObject(m, (char*)"NotFoundError", NotFoundError);
	// Tried to create an object with the same name as one that already exists
	NameExistsError = PyErr_NewException((char*)"scribus.NameExistsError", ScribusException, NULL);
	Py_INCREF(NameExistsError);
	PyModule_AddObject(m, (char*)"NameExistsError", NameExistsError);
	// Done with exception setup

	// CONSTANTS
	PyDict_SetItemString(d, "UNIT_POINTS", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "UNIT_MILLIMETERS", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "UNIT_INCHES", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "UNIT_PICAS", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "PORTRAIT", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "LANDSCAPE", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "NOFACINGPAGES", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "FACINGPAGES",  Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "FIRSTPAGERIGHT", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "FIRSTPAGELEFT", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "ALIGN_LEFT", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "ALIGN_RIGHT", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "ALIGN_CENTERED", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "ALIGN_FORCED", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "ALIGN_BLOCK", Py_BuildValue("i", 4));
	PyDict_SetItemString(d, "FILL_NOG", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "FILL_HORIZONTALG", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "FILL_VERTICALG", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "FILL_DIAGONALG", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "FILL_CROSSDIAGONALG", Py_BuildValue("i", 4));
	PyDict_SetItemString(d, "FILL_RADIALG", Py_BuildValue("i", 5));
	PyDict_SetItemString(d, "LINE_SOLID", Py_BuildValue("i", Qt::SolidLine));
	PyDict_SetItemString(d, "LINE_DASH", Py_BuildValue("i", Qt::DashLine));
	PyDict_SetItemString(d, "LINE_DOT", Py_BuildValue("i", Qt::DotLine));
	PyDict_SetItemString(d, "LINE_DASHDOT", Py_BuildValue("i", Qt::DashDotLine));
	PyDict_SetItemString(d, "LINE_DASHDOTDOT", Py_BuildValue("i", Qt::DashDotDotLine));
	PyDict_SetItemString(d, "JOIN_MITTER", Py_BuildValue("i", Qt::MiterJoin));
	PyDict_SetItemString(d, "JOIN_BEVEL", Py_BuildValue("i", Qt::BevelJoin));
	PyDict_SetItemString(d, "JOIN_ROUND", Py_BuildValue("i", Qt::RoundJoin));
	PyDict_SetItemString(d, "CAP_FLAT", Py_BuildValue("i", Qt::FlatCap));
	PyDict_SetItemString(d, "CAP_SQUARE", Py_BuildValue("i", Qt::SquareCap));
	PyDict_SetItemString(d, "CAP_ROUND", Py_BuildValue("i", Qt::RoundCap));
	PyDict_SetItemString(d, "BUTTON_NONE", Py_BuildValue("i", QMessageBox::NoButton));
	PyDict_SetItemString(d, "BUTTON_OK", Py_BuildValue("i", QMessageBox::Ok));
	PyDict_SetItemString(d, "BUTTON_CANCEL", Py_BuildValue("i", QMessageBox::Cancel));
	PyDict_SetItemString(d, "BUTTON_YES", Py_BuildValue("i", QMessageBox::Yes));
	PyDict_SetItemString(d, "BUTTON_NO", Py_BuildValue("i", QMessageBox::No));
	PyDict_SetItemString(d, "BUTTON_ABORT", Py_BuildValue("i", QMessageBox::Abort));
	PyDict_SetItemString(d, "BUTTON_RETRY", Py_BuildValue("i", QMessageBox::Retry));
	PyDict_SetItemString(d, "BUTTON_IGNORE", Py_BuildValue("i", QMessageBox::Ignore));
	PyDict_SetItemString(d, "ICON_NONE", Py_BuildValue("i", QMessageBox::NoIcon));
	PyDict_SetItemString(d, "ICON_INFORMATION", Py_BuildValue("i", QMessageBox::Information));
	PyDict_SetItemString(d, "ICON_WARNING", Py_BuildValue("i", QMessageBox::Warning));
	PyDict_SetItemString(d, "ICON_CRITICAL", Py_BuildValue("i", QMessageBox::Critical));
	PyDict_SetItemString(d, "PAPER_A0", Py_BuildValue("(ff)", 2380.0, 3368.0));
	PyDict_SetItemString(d, "PAPER_A1", Py_BuildValue("(ff)", 1684.0, 2380.0));
	PyDict_SetItemString(d, "PAPER_A2", Py_BuildValue("(ff)", 1190.0, 1684.0));
	PyDict_SetItemString(d, "PAPER_A3", Py_BuildValue("(ff)", 842.0, 1190.0));
	PyDict_SetItemString(d, "PAPER_A4", Py_BuildValue("(ff)", 595.0, 842.0));
	PyDict_SetItemString(d, "PAPER_A5", Py_BuildValue("(ff)", 421.0, 595.0));
	PyDict_SetItemString(d, "PAPER_A6", Py_BuildValue("(ff)", 297.0, 421.0));
	PyDict_SetItemString(d, "PAPER_A7", Py_BuildValue("(ff)", 210.0, 297.0));
	PyDict_SetItemString(d, "PAPER_A8", Py_BuildValue("(ff)", 148.0, 210.0));
	PyDict_SetItemString(d, "PAPER_A9", Py_BuildValue("(ff)", 105.0, 148.0));
	PyDict_SetItemString(d, "PAPER_B0", Py_BuildValue("(ff)", 2836.0, 4008.0));
	PyDict_SetItemString(d, "PAPER_B1", Py_BuildValue("(ff)", 2004.0, 2836.0));
	PyDict_SetItemString(d, "PAPER_B2", Py_BuildValue("(ff)", 1418.0, 2004.0));
	PyDict_SetItemString(d, "PAPER_B3", Py_BuildValue("(ff)", 1002.0, 1418.0));
	PyDict_SetItemString(d, "PAPER_B4", Py_BuildValue("(ff)", 709.0, 1002.0));
	PyDict_SetItemString(d, "PAPER_B5", Py_BuildValue("(ff)", 501.0, 709.0));
	PyDict_SetItemString(d, "PAPER_B6", Py_BuildValue("(ff)", 355.0, 501.0));
	PyDict_SetItemString(d, "PAPER_B7", Py_BuildValue("(ff)", 250.0, 355.0));
	PyDict_SetItemString(d, "PAPER_B8", Py_BuildValue("(ff)", 178.0, 250.0));
	PyDict_SetItemString(d, "PAPER_B9", Py_BuildValue("(ff)", 125.0, 178.0));
	PyDict_SetItemString(d, "PAPER_B10", Py_BuildValue("(ff)", 89.0, 125.0));
	PyDict_SetItemString(d, "PAPER_C5E", Py_BuildValue("(ff)", 462.0, 649.0));
	PyDict_SetItemString(d, "PAPER_COMM10E", Py_BuildValue("(ff)", 298.0, 683.0));
	PyDict_SetItemString(d, "PAPER_DLE",  Py_BuildValue("(ff)", 312.0, 624.0));
	PyDict_SetItemString(d, "PAPER_EXECUTIVE", Py_BuildValue("(ff)", 542.0, 720.0));
	PyDict_SetItemString(d, "PAPER_FOLIO", Py_BuildValue("(ff)", 595.0, 935.0));
	PyDict_SetItemString(d, "PAPER_LEDGER", Py_BuildValue("(ff)", 1224.0, 792.0));
	PyDict_SetItemString(d, "PAPER_LEGAL", Py_BuildValue("(ff)", 612.0, 1008.0));
	PyDict_SetItemString(d, "PAPER_LETTER", Py_BuildValue("(ff)", 612.0, 792.0));
	PyDict_SetItemString(d, "PAPER_TABLOID", Py_BuildValue("(ff)", 792.0, 1224.0));

	Carrier = pl;
	// Function aliases for compatibility
	// We need to import the __builtins__, warnings and exceptions modules to be able to run
	// the generated Python functions from inside the `scribus' module's context.
	// This code makes it possible to extend the `scribus' module by running Python code
	// from C in other ways too.
	PyObject* builtinModule = PyImport_ImportModuleEx("__builtin__", d, d, Py_BuildValue("[]"));
	if (builtinModule == NULL)
	{
		qDebug("Failed to import __builtin__ module. Something is probably broken with your Python.");
		return;
	}
	PyDict_SetItemString(d, "__builtin__", builtinModule);
	PyObject* exceptionsModule = PyImport_ImportModuleEx("exceptions", d, d, Py_BuildValue("[]"));
	if (exceptionsModule == NULL)
	{
		qDebug("Failed to import exceptions module. Something is probably broken with your Python.");
		return;
	}
	PyDict_SetItemString(d, "exceptions", exceptionsModule);
	PyObject* warningsModule = PyImport_ImportModuleEx("warnings", d, d, Py_BuildValue("[]"));
	if (warningsModule == NULL)
	{
		qDebug("Failed to import warnings module. Something is probably broken with your Python.");
		return;
	}
	PyDict_SetItemString(d, "warnings", warningsModule);
	// Now actually add the aliases
	deprecatedFunctionAlias(d, "changeColor", "ChangeColor");
	deprecatedFunctionAlias(d, "closeDoc", "CloseDoc");
	deprecatedFunctionAlias(d, "createBezierLine", "CreateBezierLine");
	deprecatedFunctionAlias(d, "createEllipse", "CreateEllipse");
	deprecatedFunctionAlias(d, "createImage", "CreateImage");
	deprecatedFunctionAlias(d, "createLayer", "CreateLayer");
	deprecatedFunctionAlias(d, "createLine", "CreateLine");
	deprecatedFunctionAlias(d, "createPathText", "CreatePathText");
	deprecatedFunctionAlias(d, "createPolygon", "CreatePolygon");
	deprecatedFunctionAlias(d, "createPolyLine", "CreatePolyLine");
	deprecatedFunctionAlias(d, "createRect", "CreateRect");
	deprecatedFunctionAlias(d, "createText", "CreateText");
	deprecatedFunctionAlias(d, "currentPage", "CurrentPage");
	deprecatedFunctionAlias(d, "defineColor", "DefineColor");
	deprecatedFunctionAlias(d, "deleteColor", "DeleteColor");
	deprecatedFunctionAlias(d, "deleteLayer", "DeleteLayer");
	deprecatedFunctionAlias(d, "deleteObject", "DeleteObject");
	deprecatedFunctionAlias(d, "deletePage", "DeletePage");
	deprecatedFunctionAlias(d, "deleteText", "DeleteText");
	deprecatedFunctionAlias(d, "deselectAll", "DeselectAll");
	deprecatedFunctionAlias(d, "docChanged", "DocChanged");
	deprecatedFunctionAlias(d, "fileDialog", "FileDialog");
	deprecatedFunctionAlias(d, "getActiveLayer", "GetActiveLayer");
	deprecatedFunctionAlias(d, "getAllObjects", "GetAllObjects");
	deprecatedFunctionAlias(d, "getAllStyles", "GetAllStyles");
	deprecatedFunctionAlias(d, "getAllText", "GetAllText");
	deprecatedFunctionAlias(d, "getColor", "GetColor");
	deprecatedFunctionAlias(d, "getColorNames", "GetColorNames");
	deprecatedFunctionAlias(d, "getColumnGap", "GetColumnGap");
	deprecatedFunctionAlias(d, "getColumns", "GetColumns");
	deprecatedFunctionAlias(d, "getCornerRadius", "GetCornerRadius");
	deprecatedFunctionAlias(d, "getFillColor", "GetFillColor");
	deprecatedFunctionAlias(d, "getFillShade", "GetFillShade");
	deprecatedFunctionAlias(d, "getFont", "GetFont");
	deprecatedFunctionAlias(d, "getFontNames", "GetFontNames");
	deprecatedFunctionAlias(d, "getFontSize", "GetFontSize");
	deprecatedFunctionAlias(d, "getGuiLanguage", "GetGuiLanguage");
	deprecatedFunctionAlias(d, "getHGuides", "GetHGuides");
	deprecatedFunctionAlias(d, "getImageFile", "GetImageFile");
	deprecatedFunctionAlias(d, "getImageScale", "GetImageScale");
	deprecatedFunctionAlias(d, "getLayers", "GetLayers");
	deprecatedFunctionAlias(d, "getLineCap", "GetLineCap");
	deprecatedFunctionAlias(d, "getLineColor", "GetLineColor");
	deprecatedFunctionAlias(d, "getLineJoin", "GetLineJoin");
	deprecatedFunctionAlias(d, "getLineShade", "GetLineShade");
	deprecatedFunctionAlias(d, "getLineSpacing", "GetLineSpacing");
	deprecatedFunctionAlias(d, "getLineStyle", "GetLineStyle");
	deprecatedFunctionAlias(d, "getLineWidth", "GetLineWidth");
	deprecatedFunctionAlias(d, "getPageItems", "GetPageItems");
	deprecatedFunctionAlias(d, "getPageMargins", "GetPageMargins");
	deprecatedFunctionAlias(d, "getPageSize", "GetPageSize");
	deprecatedFunctionAlias(d, "getPosition", "GetPosition");
	deprecatedFunctionAlias(d, "getRotation", "GetRotation");
	deprecatedFunctionAlias(d, "getSelectedObject", "GetSelectedObject");
	deprecatedFunctionAlias(d, "getSize", "GetSize");
	deprecatedFunctionAlias(d, "getText", "GetText");
	deprecatedFunctionAlias(d, "getTextColor", "GetTextColor");
	deprecatedFunctionAlias(d, "getTextLength", "GetTextLength");
	deprecatedFunctionAlias(d, "getTextShade", "GetTextShade");
	deprecatedFunctionAlias(d, "getUnit", "GetUnit");
	deprecatedFunctionAlias(d, "getVGuides", "GetVGuides");
	deprecatedFunctionAlias(d, "getXFontNames", "GetXFontNames");
	deprecatedFunctionAlias(d, "gotoPage", "GotoPage");
	deprecatedFunctionAlias(d, "groupObjects", "GroupObjects");
	deprecatedFunctionAlias(d, "haveDoc", "HaveDoc");
	deprecatedFunctionAlias(d, "insertText", "InsertText");
	deprecatedFunctionAlias(d, "isLayerPrintable", "IsLayerPrintable");
	deprecatedFunctionAlias(d, "isLayerVisible", "IsLayerVisible");
	deprecatedFunctionAlias(d, "isLocked", "IsLocked");
	deprecatedFunctionAlias(d, "linkTextFrames", "LinkTextFrames");
	deprecatedFunctionAlias(d, "loadImage", "LoadImage");
	deprecatedFunctionAlias(d, "loadStylesFromFile", "LoadStylesFromFile");
	deprecatedFunctionAlias(d, "lockObject", "LockObject");
	deprecatedFunctionAlias(d, "messagebarText", "MessagebarText");
	deprecatedFunctionAlias(d, "messageBox", "MessageBox");
	deprecatedFunctionAlias(d, "moveObject", "MoveObject");
	deprecatedFunctionAlias(d, "moveObjectAbs", "MoveObjectAbs");
	deprecatedFunctionAlias(d, "newDoc", "NewDoc");
	deprecatedFunctionAlias(d, "newDocDialog", "NewDocDialog");
	deprecatedFunctionAlias(d, "newPage", "NewPage");
	deprecatedFunctionAlias(d, "objectExists", "ObjectExists");
	deprecatedFunctionAlias(d, "openDoc", "OpenDoc");
	deprecatedFunctionAlias(d, "pageCount", "PageCount");
	deprecatedFunctionAlias(d, "pageDimension", "PageDimension");
	deprecatedFunctionAlias(d, "progressReset", "ProgressReset");
	deprecatedFunctionAlias(d, "progressSet", "ProgressSet");
	deprecatedFunctionAlias(d, "progressTotal", "ProgressTotal");
	deprecatedFunctionAlias(d, "redrawAll", "RedrawAll");
	deprecatedFunctionAlias(d, "renderFont", "RenderFont");
	deprecatedFunctionAlias(d, "replaceColor", "ReplaceColor");
	deprecatedFunctionAlias(d, "rotateObject", "RotateObject");
	deprecatedFunctionAlias(d, "rotateObjectAbs", "RotateObjectAbs");
	deprecatedFunctionAlias(d, "saveDoc", "SaveDoc");
	deprecatedFunctionAlias(d, "saveDocAs", "SaveDocAs");
	deprecatedFunctionAlias(d, "savePageAsEPS", "SavePageAsEPS");
	deprecatedFunctionAlias(d, "scaleGroup", "ScaleGroup");
	deprecatedFunctionAlias(d, "scaleImage", "ScaleImage");
	deprecatedFunctionAlias(d, "selectionCount", "SelectionCount");
	deprecatedFunctionAlias(d, "selectObject", "SelectObject");
	deprecatedFunctionAlias(d, "selectText", "SelectText");
	deprecatedFunctionAlias(d, "sentToLayer", "SentToLayer");
	deprecatedFunctionAlias(d, "setActiveLayer", "SetActiveLayer");
	deprecatedFunctionAlias(d, "setColumnGap", "SetColumnGap");
	deprecatedFunctionAlias(d, "setColumns", "SetColumns");
	deprecatedFunctionAlias(d, "setCornerRadius", "SetCornerRadius");
	deprecatedFunctionAlias(d, "setCursor", "SetCursor");
	deprecatedFunctionAlias(d, "setDocType", "SetDocType");
	deprecatedFunctionAlias(d, "setFillColor", "SetFillColor");
	deprecatedFunctionAlias(d, "setFillShade", "SetFillShade");
	deprecatedFunctionAlias(d, "setFont", "SetFont");
	deprecatedFunctionAlias(d, "setFontSize", "SetFontSize");
	deprecatedFunctionAlias(d, "setGradientFill", "SetGradientFill");
	deprecatedFunctionAlias(d, "setHGuides", "SetHGuides");
	deprecatedFunctionAlias(d, "setInfo", "SetInfo");
	deprecatedFunctionAlias(d, "setLayerPrintable", "SetLayerPrintable");
	deprecatedFunctionAlias(d, "setLayerVisible", "SetLayerVisible");
	deprecatedFunctionAlias(d, "setLineCap", "SetLineCap");
	deprecatedFunctionAlias(d, "setLineColor", "SetLineColor");
	deprecatedFunctionAlias(d, "setLineJoin", "SetLineJoin");
	deprecatedFunctionAlias(d, "setLineShade", "SetLineShade");
	deprecatedFunctionAlias(d, "setLineSpacing", "SetLineSpacing");
	deprecatedFunctionAlias(d, "setLineStyle", "SetLineStyle");
	deprecatedFunctionAlias(d, "setLineWidth", "SetLineWidth");
	deprecatedFunctionAlias(d, "setMargins", "SetMargins");
	deprecatedFunctionAlias(d, "setMultiLine", "SetMultiLine");
	deprecatedFunctionAlias(d, "setMultiLine", "SetMultiLine");
	deprecatedFunctionAlias(d, "setRedraw", "SetRedraw");
	deprecatedFunctionAlias(d, "setSelectedObject", "SetSelectedObject");
	deprecatedFunctionAlias(d, "setStyle", "SetStyle");
	deprecatedFunctionAlias(d, "setText", "SetText");
	deprecatedFunctionAlias(d, "setTextAlignment", "SetTextAlignment");
	deprecatedFunctionAlias(d, "setTextColor", "SetTextColor");
	deprecatedFunctionAlias(d, "setTextShade", "SetTextShade");
	deprecatedFunctionAlias(d, "setTextStroke", "SetTextStroke");
	deprecatedFunctionAlias(d, "setUnit", "SetUnit");
	deprecatedFunctionAlias(d, "setVGuides", "SetVGuides");
	deprecatedFunctionAlias(d, "sizeObject", "SizeObject");
	deprecatedFunctionAlias(d, "statusMessage", "StatusMessage");
	deprecatedFunctionAlias(d, "textFlowsAroundFrame", "TextFlowsAroundFrame");
	deprecatedFunctionAlias(d, "traceText", "TraceText");
	deprecatedFunctionAlias(d, "unGroupObject", "UnGroupObject");
	deprecatedFunctionAlias(d, "unlinkTextFrames", "UnlinkTextFrames");
	deprecatedFunctionAlias(d, "valueDialog", "ValueDialog");
	// end function aliases
	// legacy constants - alas, we can't print warnings when these
	// are used.
	constantAlias(d, "UNIT_POINTS", "Points");
	constantAlias(d, "UNIT_MILLIMETERS", "Millimeters");
	constantAlias(d, "UNIT_INCHES", "Inches");
	constantAlias(d, "UNIT_PICAS", "Picas");
	constantAlias(d, "PORTRAIT", "Portrait");
	constantAlias(d, "LANDSCAPE", "Landscape");
	constantAlias(d, "NOFACINGPAGES", "NoFacingPages");
	constantAlias(d, "FACINGPAGES", "FacingPages");
	constantAlias(d, "FIRSTPAGERIGHT", "FirstPageRight");
	constantAlias(d, "FIRSTPAGELEFT", "FirstPageLeft");
	constantAlias(d, "ALIGN_LEFT", "LeftAlign");
	constantAlias(d, "ALIGN_RIGHT", "RightAlign");
	constantAlias(d, "ALIGN_CENTERED", "Centered");
	constantAlias(d, "ALIGN_FORCED", "Forced");
	constantAlias(d, "FILL_NOG", "NoGradient");
	constantAlias(d, "FILL_HORIZONTALG", "HorizontalGradient");
	constantAlias(d, "FILL_VERTICALG", "VerticalGradient");
	constantAlias(d, "FILL_DIAGONALG", "DiagonalGradient");
	constantAlias(d, "FILL_CROSSDIAGONALG", "CrossDiagonalGradient");
	constantAlias(d, "FILL_RADIALG", "RadialGradient");
	constantAlias(d, "LINE_SOLID", "SolidLine");
	constantAlias(d, "LINE_DASH", "DashLine");
	constantAlias(d, "LINE_DOT", "DotLine");
	constantAlias(d, "LINE_DASHDOT", "DashDotLine");
	constantAlias(d, "LINE_DASHDOTDOT", "DashDotDotLine");
	constantAlias(d, "JOIN_MITTER", "MiterJoin");
	constantAlias(d, "JOIN_BEVEL", "BevelJoin");
	constantAlias(d, "JOIN_ROUND", "RoundJoin");
	constantAlias(d, "CAP_FLAT", "FlatCap");
	constantAlias(d, "CAP_SQUARE", "SquareCap");
	constantAlias(d, "CAP_ROUND", "RoundCap");
	constantAlias(d, "BUTTON_NONE", "NoButton");
	constantAlias(d, "BUTTON_OK", "Ok");
	constantAlias(d, "BUTTON_CANCEL", "Cancel");
	constantAlias(d, "BUTTON_YES", "Yes");
	constantAlias(d, "BUTTON_NO", "No");
	constantAlias(d, "BUTTON_ABORT", "Abort");
	constantAlias(d, "BUTTON_RETRY", "Retry");
	constantAlias(d, "BUTTON_IGNORE", "Ignore");
	constantAlias(d, "ICON_NONE", "NoIcon");
	constantAlias(d, "ICON_INFORMATION", "Information");
	constantAlias(d, "ICON_WARNING", "Warning");
	constantAlias(d, "ICON_CRITICAL", "Critical");
	constantAlias(d, "PAPER_A0", "Paper_A0");
	constantAlias(d, "PAPER_A1", "Paper_A1");
	constantAlias(d, "PAPER_A2", "Paper_A2");
	constantAlias(d, "PAPER_A3", "Paper_A3");
	constantAlias(d, "PAPER_A4", "Paper_A4");
	constantAlias(d, "PAPER_A5", "Paper_A5");
	constantAlias(d, "PAPER_A6", "Paper_A6");
	constantAlias(d, "PAPER_A7", "Paper_A7");
	constantAlias(d, "PAPER_A8", "Paper_A8");
	constantAlias(d, "PAPER_A9", "Paper_A9");
	constantAlias(d, "PAPER_B0", "Paper_B0");
	constantAlias(d, "PAPER_B1", "Paper_B1");
	constantAlias(d, "PAPER_B2", "Paper_B2");
	constantAlias(d, "PAPER_B3", "Paper_B3");
	constantAlias(d, "PAPER_B4", "Paper_B4");
	constantAlias(d, "PAPER_B5", "Paper_B5");
	constantAlias(d, "PAPER_B6", "Paper_B6");
	constantAlias(d, "PAPER_B7", "Paper_B7");
	constantAlias(d, "PAPER_B8", "Paper_B8");
	constantAlias(d, "PAPER_B9", "Paper_B9");
	constantAlias(d, "PAPER_B10", "Paper_B10");
	constantAlias(d, "PAPER_C5E", "Paper_C5E");
	constantAlias(d, "PAPER_COMM10E", "Paper_Comm10E");
	constantAlias(d, "PAPER_DLE", "Paper_DLE");
	constantAlias(d, "PAPER_EXECUTIVE", "Paper_Executive");
	constantAlias(d, "PAPER_FOLIO", "Paper_Folio");
	constantAlias(d, "PAPER_LEDGER", "Paper_Ledger");
	constantAlias(d, "PAPER_LEGAL", "Paper_Legal");
	constantAlias(d, "PAPER_LETTER", "Paper_Letter");
	constantAlias(d, "PAPER_TABLOID", "Paper_Tabloid");
	// end of deprecated cosntants

	// Create the module-level docstring. This can be a proper unicode string, unlike
	// the others, because we can just create a Unicode object and insert it in our
	// module dictionary.
	QString docstring = QObject::tr("Scribus Python interface module\n\
		\n\
		This module is the Python interface for Scribus. It provides functions\n\
		to control scribus and to manipulate objects on the canvas. Each\n\
		function is documented individually below.\n\
		\n\
		A few things are common across most of the interface.\n\
		\n\
		Most functions operate on frames. Frames are identified by their name,\n\
		a string - they are not real Python objects. Many functions take an\n\
		optional (non-keyword) parameter, a frame name.\n\
		Many exceptions are also common across most functions. These are\n\
		not currently documented in the docstring for each function.\n\
		\n\
		    - Many functions will raise a NoDocOpenError if you try to use them\n\
		      without a document to operate on.\n\
		\n\
		    - If you do not pass a frame name to a function that requires one,\n\
		      the function will use the currently selected frame, if any, or\n\
		      raise a NoValidObjectError if it can't find anything to operate\n\
		      on.\n\
		\n\
		    - Many functions will raise WrongFrameTypeError if you try to use them\n\
		      on a frame type that they do not make sense with. For example, setting\n\
		      the text colour on a graphics frame doesn't make sense, and will result\n\
		      in this exception being raised.\n\
		\n\
		    - Errors resulting from calls to the underlying Python API will be\n\
		      passed through unaltered. As such, the list of exceptions thrown by\n\
		      any function as provided here and in its docstring is incomplete.\n\
		\n\
		Details of what exceptions each function may throw are provided on the\n\
		function's documentation.\n\
		");

	// Py_UNICODE is a typedef for unsigned short
	PyObject* uniDocStr = Py_BuildValue("u", (Py_UNICODE*)(docstring.ucs2()));
	if (uniDocStr == NULL)
		qDebug("Failed to create module-level docstring object!");
	else
		PyDict_SetItemString(d, "__doc__", uniDocStr);
	Py_DECREF(uniDocStr);
}

