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
#include "Python.h"
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
#include "cmdvar.h"
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

// Exceptions; visible from cmdvar.h, set up in initscribus()
PyObject* ScribusException;
PyObject* NoDocOpenError;

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
	Carrier = plug;
	RetVal = 0;
	initscribus(Carrier);
	Tes = new MenuTest(d);
	men = new QPopupMenu();
	Tes->rmen = new QPopupMenu();
	Tes->smen = new QPopupMenu();
	Tes->SavedRecentScripts.clear();
	Tes->ReadPlugPrefs();
	QString pfad = PREL;
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad + "/share/scribus/scripts/");
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
	QString pfad = PREL;
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad + "/share/scribus/doc/en/Scripter/index.html");
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("Online Reference"), Carrier->GuiLanguage, "scripter");
	dia->show();
}


void MenuTest::FinishScriptRun()
{
	if (Carrier->HaveDoc)
	{
		Carrier->Mpal->SetDoc(Carrier->doc);
		Carrier->Mpal->updateCList();
		Carrier->Mpal->Spal->SetFormats(Carrier->doc);
		Carrier->Mpal->SetLineFormats(Carrier->doc);
		Carrier->Mpal->Cpal->SetColors(Carrier->doc->PageColors);
		Carrier->Lpal->setLayers(&Carrier->doc->Layers, &Carrier->doc->ActiveLayer);
		Carrier->Tpal->BuildTree(Carrier->view);
		Carrier->Sepal->SetView(Carrier->view);
		Carrier->Sepal->Rebuild();
		if (Carrier->doc->ActPage->SelItem.count() != 0)
			Carrier->HaveNewSel(Carrier->doc->ActPage->SelItem.at(0)->PType);
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
	QString pfad = PREL;
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad + "/share/scribus/scripts/");
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
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("About Script") + " " + fi.fileName(), "", html);
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
	QCString wsData = wrapperFunc.latin1();
	// And run it in the namespace of the scribus module
	/*
	PyObject* m = PyImport_AddModule("__main__");
	if (m == NULL)
	{
		qDebug("Failed to import __main__!");
		return;
	}
	PyObject* globals = PyModule_GetDict(m);
	*/
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

/****************************************************************************************/
/*                                                                                      */
/*   Definitions of the Python commands                                                 */
/*                                                                                      */
/****************************************************************************************/

static PyObject *scribus_retval(PyObject *self, PyObject* args)
{
	char *Name;
	int retV;
	if (!PyArg_ParseTuple(args, "si", &Name, &retV))
		return NULL;
	RetString = QString(Name);
	RetVal = retV;
	return PyInt_FromLong(0L);
}

static PyObject *scribus_getval(PyObject *self, PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	return PyString_FromString(InValue);
}

/* Now we're using the more pyhtonic convention for names:
	class - ClassName
	procedure/function/method - procedureName
etc. */
static PyMethodDef scribus_methods[] = {
	// 2004/10/03 pv - aliases with common Python syntax - ClassName methodName
	// 2004-11-06 cr - move aliasing to dynamically generated wrapper functions, sort methoddef
	{"changeColor", scribus_setcolor, METH_VARARGS, "Changes the Color \"name\", The Color Value is defined via four Components c = Cyan, m = Magenta, y = Yello and k = Black. Color Compontens should be in the range from 0 to 255."},
	{"closeDoc", (PyCFunction)scribus_closedoc, METH_NOARGS, "Closes the current Document. Returns true if successful."},
	{"createBezierLine", scribus_bezierline, METH_VARARGS, "Creates a new Bezier Curve and returns its Name. The Points for the Bezier Curve are stored in the List \"list\" in the following Order: x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn. Where x and y mean the x and y Coordinates of the Point and kx and ky meaning the Controlpoint for the Curve. The Coordinates are given in the actual measurement Unit of the Document. \"name\" should be a unique Identifier for the Object because you need this Name for further referencing of that Object. If \"name\" is not given Scribus will create one for you."},
	{"createEllipse", scribus_newellipse, METH_VARARGS, "Creates a new Ellipse on the actual Page and returns its Name. The Coordinates are given in the actual measurement Unit of the Document. \"name\" should be a unique Identifier for the Object because you need this Name for further referencing of that Object. If \"name\" is not given Scribus will create one for you."},
	{"createImage", scribus_newimage, METH_VARARGS, "Creates a new Picture on the actual Page and returns its Name. The Coordinates are given in the actual measurement Unit of the Document. \"name\" should be a unique Identifier for the Object because you need this Name for further referencing of that Object. If \"name\" is not given Scribus will create one for you."},
	{"createLayer", scribus_createlayer, METH_VARARGS, "Creates a new Layer with the Name \"name\"."},
	{"createLine", scribus_newline, METH_VARARGS, "Creates a new Line from the Point(x1, y1) to the Point(x2, y2) and returns its Name. The Coordinates are given in the actual measurement Unit of the Document. \"name\" should be a unique Identifier for the Object because you need this Name for further referencing of that Object. If \"name\" is not given Scribus will create one for you."},
	{"createPathText", scribus_pathtext, METH_VARARGS, "Creates a new PathText by merging the 2 Objects \"textbox\" and \"beziercurve\" and returns its Name. The Coordinates are given in the actual measurement Unit of the Document \"name\" should be a unique Identifier for the Object because you need this Name for further referencing of that Object. If \"name\" is not given Scribus will create one for you."},
	{"createPolygon", scribus_polygon, METH_VARARGS, "Creates a new Polygon and returns its Name. The Points for the Polygon are stored in the List \"list\" in the following Order: x1, y1, x2, y2...xn. yn. At least three Points are required. There is no need to repeat the first Point to close the Polygon. The Polygon is automatically closed by connecting the first and the last Point. The Coordinates are given in the actual measurement Unit of the Document. \"name\" should be a unique Identifier for the Object because you need this Name for further referencing of that Object. If \"name\" is not given Scribus will create one for you."},
	{"createPolyLine", scribus_polyline, METH_VARARGS, "Creates a new Polyline and returns its Name. The Points for the Polyline are stored in the List \"list\" in the following Order: x1, y1, x2, y2...xn. yn. The Coordinates are given in the actual measurement Unit of the Document. \"name\" should be a unique Identifier for the Object because you need this Name for further referencing of that Object. If \"name\" is not given Scribus will create one for you."},
	{"createRect", scribus_newrect, METH_VARARGS, "Creates a new Rectangle on the actual Page and returns its Name. The Coordinates are given in the actual measurement Unit of the Document. \"name\" should be a unique Identifier for the Object because you need this Name for further referencing of that Object. If \"name\" is not given Scribus will create one for you."},
	{"createText", scribus_newtext, METH_VARARGS, "Creates a new Rectangle on the actual Page and returns its Name. The Coordinates are given in the actual measurement Unit of the Document. \"name\" should be a unique Identifier for the Object because you need this Name for further referencing of that Object. If \"name\" is not given Scribus will create one for you."},
	{"currentPage", (PyCFunction)scribus_actualpage, METH_NOARGS, "Returns the Number of the current working Page. Pagenumbers are counted from 1 upwards."},
	{"defineColor", scribus_newcolor, METH_VARARGS, "Defines a new Color \"name\". The Color Value is defined via four Components c = Cyan, m = Magenta, y = Yello and k = Black. Color Compontens should be in the range from 0 to 255."},
	{"deleteColor", scribus_delcolor, METH_VARARGS, "Deletes the Color \"name\". Every occurence of that Color is replaced by the Color \"replace\"."},
	{"deleteLayer", scribus_removelayer, METH_VARARGS, "Deletes the Layer with the Name \"name\". Nothing happens if the Layer doesn't exists or if it's the only Layer in the Document."},
	{"deleteObject", scribus_deleteobj, METH_VARARGS, "Deletes the Item with the Name \"name\". If \"name\" is not given the currently selected Item is deleted."},
	{"deletePage", scribus_deletepage, METH_VARARGS, "Deletes the given Page, does nothing if the Document contains only one Page. Pagenumbers are counted from 1 upwards."},
	{"deleteText", scribus_deletetext, METH_VARARGS, "Deletes the Text of the Textframe \"name\". If there is some Text selected, this Text will be deleted. If \"name\" is not given the currently selected Item is used."},
	{"deselectAll", (PyCFunction)scribus_deselect, METH_NOARGS, "Deselects all Objects."},
	{"docChanged", scribus_docchanged, METH_VARARGS, "Enable/disable save icon."},
	{"fileDialog", scribus_filedia, METH_VARARGS, "Shows a FileSelect Box with the Caption \"caption\". Files are filtered with the Filter String \"filter\", refer to the Qt-Documentation for it's use. A default Filename can also supplied, leave this string empty when you don't want to use it. A Value of 1 for preview enables a small Preview Widget in the FileSelect Box. When the mode parameter is set to 1 the Dialog acts like a \"Save As\" Dialog otherwise it acts like a \"File Open Dialog\". The default for both of the opional Parameters is 0."},
	{"getActiveLayer", (PyCFunction)scribus_getactlayer, METH_NOARGS, "Returns the Name of the current active Layer."},
	{"getAllObjects", scribus_getallobj, METH_VARARGS, "Returns a List containing the Names of all Objects on the actual Page."},
	{"getAllStyles", (PyCFunction)scribus_getstylenames, METH_NOARGS, "TODO: docstring"},
	{"getAllText", scribus_gettext, METH_VARARGS, "Returns the Text of the Textframe \"name\" and of all Textframes which are linked with this Frame. If this Textframe has some Text selected, this Text is returned. If \"name\" is not given the currently selected Item is used."},
	{"getColorNames", (PyCFunction)scribus_colornames, METH_NOARGS, "Returns a List with the Names of all defined Colors."},
	{"getColor", scribus_getcolor, METH_VARARGS, "Returns a Tuple containing the four Color Components of the Color \"name\"."},
	{"getColumnGap", scribus_getcolumngap, METH_VARARGS, "Gets the Column Gap of the Text Frame \"name\" expressed in Points. If \"name\" is not given the currently selected Item is used."},
	{"getColumns", scribus_getcolumns, METH_VARARGS, "Gets the number of Columns of the Text Frame \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getCornerRadius", scribus_getcornerrad, METH_VARARGS, "Returns the Corner Radius of the Object \"name\" The Radius is expressed in Points. If \"name\" is not given the currently selected Item is used."},
	{"getFillColor", scribus_getfillcolor, METH_VARARGS, "Returns the name of the Fill Color of the Object \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getFillShade", scribus_getfillshade, METH_VARARGS, "Returns the shading Value of the Fill Color of the Object \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getFontNames", (PyCFunction)scribus_fontnames, METH_NOARGS, "Returns a List with the Names of all available Fonts."},
	{"getFont", scribus_getfont, METH_VARARGS, "Returns the Font for the Textframe \"name\". If this Textframe has some Text selected the Value assigned to the first Character of the Selection is returned. If \"name\" is not given the currently selected Item is used."},
	{"getFontSize", scribus_getfontsize, METH_VARARGS, "Returns the Fontsize for the Textframe \"name\". If this Textframe has some Text selected the Value assigned to the first Character of the Selection is returned. If \"name\" is not given the currently selected Item is used."},
	{"getGuiLanguage", (PyCFunction)scribus_getlanguage, METH_NOARGS, "Returns a string with the -lang value."},
	{"getHGuides", (PyCFunction)scribus_getHguides, METH_NOARGS, "TODO: docstring"},
	{"getImageFile", scribus_getimgname, METH_VARARGS, "Returns the Filename for the Image in the Image Frame. If \"name\" is not given the currently selected Item is used."},
	{"getImageScale", scribus_getimgscale, METH_VARARGS, "Returns a Tuple containing the Scaling Values of the Image Frame \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getLayers", (PyCFunction)scribus_getlayers, METH_NOARGS, "Returns a List with the Names of all defined Layers."},
	{"getLineCap", scribus_getlineend, METH_VARARGS, "Returns the Line Cap Style of the Object \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getLineColor", scribus_getlinecolor, METH_VARARGS, "Returns the name of the Line Color of the Object \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getLineJoin", scribus_getlinejoin, METH_VARARGS, "Returns the Line Join Style of the Object \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getLineShade", scribus_getlineshade, METH_VARARGS, "Returns the shading Value of the Line Color of the Object \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getLineSpacing", scribus_getlinespace, METH_VARARGS, "Gets the Linespacing of the Text Frame \"name\" expressed in Points. If \"name\" is not given the currently selected Item is used."},
	{"getLineStyle", scribus_getlinestyle, METH_VARARGS, "Returns the Line Style of the Object \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getLineWidth", scribus_getlinewidth, METH_VARARGS, "Returns the Line Width of the Object \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getPageItems", (PyCFunction)scribus_getpageitems, METH_NOARGS, "Returns a list of tuples with items on the actual page. (name, objectType, order) E.g. [('Text1', 4, 0), ('Image1', 2, 1)] means that object named 'Text1' is a text frame (type 4) and is the first at the page..."},
	{"getPageMargins", (PyCFunction)scribus_getpagemargins, METH_NOARGS, "TODO: docstring"},
	{"getPageSize", (PyCFunction)scribus_pagedimension, METH_NOARGS, "Returns a tuple with page dimensions in used system e.g. when the document's page is in picas - picas are returned"}, // just an alias to PageDimension()
	{"getPosition", scribus_getposi, METH_VARARGS, "Returns a tuple with the actual Position of the Object \"name\" If \"name\" is not given the currently selected Item is used. The Position is expressed in the actual Measurement Unit of the Document."},
	{"getRotation", scribus_getrotation, METH_VARARGS, "Returns the Rotation of the Object \"name\". The value is expressed in Degrees. If \"name\" is not given the currently selected Item is used."},
	{"getSelectedObject", scribus_getselobjnam, METH_VARARGS, "Returns the Name of the selecteted Object. \"nr\" if given indicates the Number of the selected Object, e.g. 0 means the first selected Object, 1 means the second selected Object and so on."},
	{"getSize", scribus_getsize, METH_VARARGS, "Returns a tuple with the actual Size of the Object \"name\" If \"name\" is not given the currently selected Item is used. The Size is expressed in the actual Measurement Unit of the Document."},
	{"getTextColor", scribus_getlinecolor, METH_VARARGS, "Returns the name of the Text Color of the Object \"name\". If this Textframe has some Text selected the Value assigned to the first Character of the Selection is returned. If \"name\" is not given the currently selected Item is used."},
	{"getTextLength", scribus_gettextsize, METH_VARARGS, "Returns the Length of the Text in the Textframe \"name\". If \"name\" is not given the currently selected Item is used."},
	{"getText", scribus_getframetext, METH_VARARGS, "Returns the Text of the Textframe \"name\". If this Textframe has some Text selected, this Text is returned. If \"name\" is not given the currently selected Item is used."},
	{"getTextShade", scribus_getlineshade, METH_VARARGS, "Returns the shading Value of the Text Color of the Object \"name\". If this Textframe has some Text selected the Value assigned to the first Character of the Selection is returned. If \"name\" is not given the currently selected Item is used."},
	{"getUnit", (PyCFunction)scribus_getunit, METH_NOARGS, "Returns the Measurement Unit of the Document.\nPossible Values for Unit are:\n\t0 = Typographic Points\n\t1 = Millimeters\n\t2 = Inches\n\t3 = Picas "},
	{"getVGuides", (PyCFunction)scribus_getVguides, METH_NOARGS, "TODO: docstring"},
	{"getXFontNames", (PyCFunction)scribus_xfontnames, METH_NOARGS, "Returns a list of the tuples with: [ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]"},
	{"gotoPage", scribus_gotopage, METH_VARARGS, "Moves to the Page \"nr\". If \"nr\" is outside the current rage of Pages nothing happens."},
	{"groupObjects", scribus_groupobj, METH_VARARGS, "Groups the Objects in \"list\" together. \"list\" must contain the Names of the Objects to be grouped. If \"list\" is not given the currently selected Items are used."},
	{"haveDoc", (PyCFunction)scribus_havedoc, METH_NOARGS, "Returns true if there is a Document open."},
	{"insertText", scribus_inserttext, METH_VARARGS, "Inserts the Text \"text\" at the Position \"pos\" into the Textframe The first Character has an Index of 0. \"name\" If \"name\" is not given the currently selected Item is used."},
	{"isLayerPrintable", scribus_glayerprint, METH_VARARGS, "Returns wether the Layer \"layer\" is printable or not, a Value of 1 means that the Layer \"layer\" can be printed, a Value of 0 means that printing the Layer \"layer\" is disabled."},
	{"isLayerVisible", scribus_glayervisib, METH_VARARGS, "Returns wether the Layer \"layer\" is visible or not, a Value of 1 means that the Layer \"layer\" is visible, a Value of 0 means that the Layer \"layer\" is invisible."},
	{"isLocked", scribus_islocked, METH_VARARGS, "TODO: docstring"},
	{"linkTextFrames", scribus_linktextframes, METH_VARARGS, "Create the linked text frames. Parameters are the object names."},
	{"loadImage", scribus_loadimage, METH_VARARGS, "Loads the Picture \"picture\" into the Image Frame \"name\". If \"name\" is not given the currently selected Item is used."},
	{"loadStylesFromFile", scribus_loadstylesfromfile, METH_VARARGS, "TODO: docstring"},
	{"lockObject", scribus_lockobject, METH_VARARGS, "TODO: docstring"},
	{"messagebarText", scribus_messagebartext, METH_VARARGS, "Writes the param string into the Scribus message bar (status line)."},
	{"messageBox", scribus_messdia, METH_VARARGS, "Shows a Message Box with the Title \"caption\", the Message \"message\", and an Icon icon and up to 3 Buttons. Button1 is always needed. For the icon and the Button Parameters there are predefined Constants available with the same Names as in the Qt Documentation. Returns the number of the selected Button."},
	{"moveObjectAbs", scribus_moveobjabs, METH_VARARGS, "Moves the Object \"name\" to a new Location. The Coordinates are expressed in the actual Measurement Unit of the Document. If \"name\" is not given the currently selected Item is used. If the Object \"name\" belongs to a Group, the whole Group is moved."},
	{"moveObject", scribus_moveobjrel, METH_VARARGS, "Moves the Object \"name\" by dx and dy relative to its origin. The Distances are expressed in the actual Measurement Unit of the Document. If \"name\" is not given the currently selected Item is used. If the Object \"name\" belongs to a Group, the whole Group is moved."},
	{"newDocDialog", (PyCFunction)scribus_newdocdia, METH_NOARGS, "Shows the \"New Document\" Dialog Box. Returns true if a new Document was created."},
	{"newDoc", scribus_newdoc, METH_VARARGS, "Creates a new Document. The Parameters have the following Meaning:\n\tsize = A Tuple (width, height) describing the Size of the Document.\n\tmargins = A Tuple (Left, Right, Top, Bottom) describing the Margins of the Document.\n\torientation = the Page Orientation, 0 means Portrait, 1 is Landscape\n\tfirstPageNumer is the Number of the first Page in the Document used for Pagenumbering\n\tunit: this Value sets the Measurement Unit of the Document\n\n\t0 = Typographic Points\n\t1 = Millimeters\n\t2 = Inches\n\t3 = Picas\n\n\tFacingPages: 1 means FacingPages turned on, 0 means FacingPages turned off\n\tFirstSideLeft: 1 means that the first Page in the Document is a left Page, 0 means a right Page as first Page\n\tThe values for Width, Height and the Margins are expressed in the given unit for the Document."},
	{"newPage", scribus_newpage, METH_VARARGS, "Creates a new Page If \"where\" is -1 the new Page is appended to the Document, otherwise the new Page is inserted at \"where\". The Pagenumbers are counted from 1 upwards. The optional Parameter \"template\" specifies the Name of the Template Page for the new Page."},
	{"objectExists",scribus_objectexists, METH_VARARGS, "User test if an object with specified name really exists in the doc. Optional parameter is the object name. When no param given returns if there is something selected."},
	{"openDoc", scribus_opendoc, METH_VARARGS, "Opens the Document \"name\". Returns true if successful."},
	{"pageCount", (PyCFunction)scribus_pagecount, METH_NOARGS, "Returns the Number of Pages in the Document."},
	{"pageDimension", (PyCFunction)scribus_pagedimension, METH_NOARGS, "Returns a tuple with page dimensions in used system e.g. when the document's page is in picas - picas are returned"},
	{"progressReset", scribus_progressreset, METH_VARARGS, "Cleans up the Scribus progress bar previous settings. It is called before the new progress bar use."},
	{"progressSet", scribus_progresssetprogress, METH_VARARGS, "Progress bar handling. The progress bar uses the concept of steps; you give it the total number of steps and the number of steps completed so far and it will display the percentage of steps that have been completed. You can specify the total number of steps in the constructor or later with progressTotal(). The current number of steps is set with progressSet(). The progress bar can be rewound to the beginning with progressReset(). [taken from Trolltech's Qt docs]"},
	{"progressTotal", scribus_progresssettotalsteps, METH_VARARGS, "Sets the progress bar's maximum steps value to the specified number."},
	{"redrawAll", (PyCFunction)scribus_redraw, METH_NOARGS, "Redraws all Pages."},
	{"renderFont", scribus_renderfont, METH_VARARGS, "Creates an image preview of font with given text"},
	{"replaceColor", scribus_replcolor, METH_VARARGS, "Every occurence of that Color is replaced by the Color \"replace\"."},
	{"rotateObjectAbs", scribus_rotobjabs, METH_VARARGS, "Sets the Rotation of the Object \"name\" to \"rot\". Positve Values mean counter clockwise Rotation. If \"name\" is not given the currently selected Item is used."},
	{"rotateObject", scribus_rotobjrel, METH_VARARGS, "Rotates the Object \"name\" by \"rot\" Degrees. Positve Values mean counter clockwise Rotation. If \"name\" is not given the currently selected Item is used."},
	{"saveDocAs", scribus_savedocas, METH_VARARGS, "Saves the actual Document under the new Name \"name\". Returns true if successful."},
	{"saveDoc", (PyCFunction)scribus_savedoc, METH_NOARGS, "Saves the Document under its actual Name, returns true if successful."},
	{"savePageAsEPS", scribus_savepageeps, METH_VARARGS, "Saves the actual Page as an EPS, returns true if successful."},
	{"scaleGroup", scribus_scalegroup, METH_VARARGS, "Scales the Group the Object \"name\" belongs to. Values greater than 1 enlarge the Group, Values smaller than 1 make the Group smaller e.g a Value of 0.5 scales the Group to 50 % of is original Size, a Value of 1.5 scales the Group to 150 % of its original Size. The Value for \"factor\" must be greater than 0. If \"name\" is not given the currently selected Item is used."},
	{"scaleImage", scribus_scaleimage, METH_VARARGS, "Sets the Scaling Factors of the Picture in the Image Frame \"name\". If \"name\" is not given the currently selected Item is used. A Number of 1 means 100 %."},
	{"selectionCount", (PyCFunction)scribus_selcount, METH_NOARGS, "Returns the Number of selected Objects."},
	{"selectObject", scribus_selectobj, METH_VARARGS, "Selects the Object with the given Name."},
	{"selectText", scribus_selecttext, METH_VARARGS, "Selects \"count\" Characters Text of the Textframe \"name\" starting from the Character \"start\". Character Counting starts at 0. If \"count\" is zero, any Text Selection will be cleared. If \"name\" is not given the currently selected Item is used."},
	{"sentToLayer", scribus_senttolayer, METH_VARARGS, "Sends the Object \"name\" to the Layer \"layer\". The Layer must exist. If \"name\" is not given the currently selected Item is used."},
	{"setActiveLayer", scribus_setactlayer, METH_VARARGS, "Sets the active Layer to the Layer named \"name\"."},
	{"setColumnGap", scribus_setcolumngap, METH_VARARGS, "Sets the Column Gap of the Text Frame \"name\" to the Value \"size\". If \"name\" is not given the currently selected Item is used."},
	{"setColumns", scribus_setcolumns, METH_VARARGS, "Sets the number of Columns of the Text Frame \"name\" to the Value \"nr\". If \"name\" is not given the currently selected Item is used."},
	{"setCornerRadius", scribus_setcornerrad, METH_VARARGS, "Sets the Corner Radius of the Object \"name\" The Radius is expressed in Points. If \"name\" is not given the currently selected Item is used."},
	{"setCursor", scribus_setcursor, METH_VARARGS, "TODO: docstring"},
	{"setDocType", scribus_setdoctype, METH_VARARGS, "Sets the Type of the Documents, to get Facing Pages set the first Parameter to 1, to switch FacingPages off use 0 instead. If you want to be the first Page a left Side set the second Parameter to 1, for a right Page use 0."},
	{"setFillColor", scribus_setfillcolor, METH_VARARGS, "Sets the Fill Color of the Object \"name\" to the Color \"color\". \"color\" is the Name of one of the defined Colors. If \"name\" is not given the currently selected Item is used."},
	{"setFillShade", scribus_setfillshade, METH_VARARGS, "Sets the shading of the Fill Color of the Object \"name\" to \"shade\". \"shade\" must be an Integer Value in the range from 0 (lightest) to 100 (full Color intensity). If \"name\" is not given the currently selected Item is used."},
	{"setFont", scribus_setfont, METH_VARARGS, "Sets the Font of the Text Frame \"name\" to \"font\", if there is some Text selected only the selected Text is changed. If \"name\" is not given the currently selected Item is used."},
	{"setFontSize", scribus_setfontsize, METH_VARARGS, "Sets the Fontsize of the Text Frame \"name\" to the Pointsize \"size\", if there is some Text selected only the selected Text is changed. \"size\" must be in the Range 1 to 512. If \"name\" is not given the currently selected Item is used."},
	{"setGradientFill", scribus_setgradfill, METH_VARARGS, "Sets the Gradient Fill of the Object \"name\" to type. Color Descriptions are the same as for \"setFillColor\" and \"setFillShade\"."},
	{"setHGuides", scribus_setHguides, METH_VARARGS, "TODO: docstring"},
	{"setInfo", scribus_setinfo, METH_VARARGS, "Sets the Document Information. \"Author\", \"Info\", \"Description\""},
	{"setLayerPrintable", scribus_layerprint, METH_VARARGS, "Sets the Layer \"layer\" to be printable or not. A Value of 1 for \"flag\" means that the Layer \"layer\" can be printed, a Value of 0 means that printing the Layer \"layer\" is disabled."},
	{"setLayerVisible", scribus_layervisible, METH_VARARGS, "Sets the Layer \"layer\" to be visible or not. A Value of 1 for \"flag\" means that the Layer \"layer\" is visible, a Value of 0 means that the Layer \"layer\" is invisible."},
	{"setLineCap", scribus_setlineend, METH_VARARGS, "Sets the Line Cap Style of the Object \"name\" to the Style \"cap\". If \"name\" is not given the currently selected Item is used. There are predefined Constants for \"cap\"."},
	{"setLineColor", scribus_setlinecolor, METH_VARARGS, "Sets the Line Color of the Object \"name\" to the Color \"color\". If \"name\" is not given the currently selected Item is used."},
	{"setLineJoin", scribus_setlinejoin, METH_VARARGS, "Sets the Line Join Style of the Object \"name\" to the Style \"join\". If \"name\" is not given the currently selected Item is used. There are predefined Constants for \"join\"."},
	{"setLineShade", scribus_setlineshade, METH_VARARGS, "Sets the shading of the Line Color of the Object \"name\" to \"shade\". \"shade\" must be an Integer Value in the range from 0 (lightest) to 100 (full Color intensity). If \"name\" is not given the currently selected Item is used."},
	{"setLineSpacing", scribus_setlinespace, METH_VARARGS, "Sets the Linespacing of the Text Frame \"name\" to the Pointsize \"size\". If \"name\" is not given the currently selected Item is used."},
	{"setLineStyle", scribus_setlinestyle, METH_VARARGS, "Sets the Line Style of the Object \"name\" to the Style \"style\". If \"name\" is not given the currently selected Item is used. There are predefined Constants for \"style\"."},
	{"setLineWidth", scribus_setlinewidth, METH_VARARGS, "Sets Line Width of the Object \"name\" to \"width\". \"width\" must be in the range from 0.0 to 12.0 inclusive. If \"name\" is not given the currently selected Item is used."},
	{"setMargins", scribus_setmargins, METH_VARARGS, "Sets the Print margins of the Document, Left(lr), Right(rr), Top(tr) and Bottom(br) Margins are given in the measurement unit of the Document."},
	{"setMultiLine", scribus_setmultiline, METH_VARARGS, "Sets the Line Style of the Object \"name\" to the Named Style \"namedStyle\". If \"name\" is not given the currently selected Item is used."},
	{"setMultiLine", scribus_setmultiline, METH_VARARGS, "TODO: docstring"},
	{"setRedraw", scribus_setredraw, METH_VARARGS, "Disables Page Redraw when bool = 0, otherwise redrawing is enabled."},
	{"setSelectedObject", scribus_getselobjnam, METH_VARARGS, "Returns the Name of the selecteted Object. \"nr\" if given indicates the Number of the selected Object, e.g. 0 means the first selected Object, 1 means the second selected Object and so on."},
	{"setStyle", scribus_setstyle, METH_VARARGS, "TODO: docstring"},
	{"setTextAlignment", scribus_setalign, METH_VARARGS, "Sets the Textalignment of the Text Frame \"name\" to the specified Alignment. If \"name\" is not given the currently selected Item is used. \"align\" can have the following values:\n\t0 = Left Aligned\n\t1 = Centered\n\t2 = Right Aligned\n\t3 = Forced"},
	{"setTextColor", scribus_settextfill, METH_VARARGS, "Sets the Text Color of the Object \"name\" to the Color \"color\", if there is some Text selected only the selected Text is changed. If \"name\" is not given the currently selected Item is used."},
	{"setText", scribus_setboxtext, METH_VARARGS, "Sets the Text of the Text Frame \"name\" to the Text of the String \"text\". If \"name\" is not given the currently selected Item is used."},
	{"setTextShade", scribus_settextshade, METH_VARARGS, "Sets the shading of the Text Color of the Object \"name\" to \"shade\", if there is some Text selected only the selected Text is changed. \"shade\" must be an Integer Value in the range from 0 (lightest) to 100 (full Color intensity). If \"name\" is not given the currently selected Item is used."},
	{"setTextStroke", scribus_settextstroke, METH_VARARGS, "TODO: docstring"},
	{"setUnit", scribus_setunit, METH_VARARGS, "Changes the Measurement Unit of the Document. Possible Values for Unit are:\n\t0 = Typographic Points\n\t1 = Millimeters\n\t2 = Inches\n\t3 = Picas "},
	{"setVGuides", scribus_setVguides, METH_VARARGS, "TODO: docstring"},
	{"sizeObject", scribus_sizeobjabs, METH_VARARGS, "Resizes the Object \"name\" to the given Width and Height. If \"name\" is not given the currently selected Item is used."},
	{"statusMessage", scribus_messagebartext, METH_VARARGS, "Displays the Message \"string\" in the StatusBar."},
	{"textFlowsAroundFrame", scribus_textflow, METH_VARARGS, "Enables/disables \"Text Flows Around Frame\" feature for object. Called with params string name and voluntary 1|0. When 1 set flowing to true (0 to false). When is second param empty flowing is reverted."},
	{"traceText", scribus_tracetext, METH_VARARGS, "TODO: docstring"},
	{"unGroupObject", scribus_ungroupobj, METH_VARARGS, "Destructs the Group the Object \"name\" belongs to. If \"name\" is not given the currently selected Item is used."},
	{"unlinkTextFrames", scribus_unlinktextframes, METH_VARARGS, "Remove the specified (named) object from the text frame flows/linkage."},
	{"valueDialog", scribus_valdialog, METH_VARARGS, "TODO: docstring"},
	// end of aliases
	{"retval", scribus_retval, METH_VARARGS, "TODO: docstring"},
	{"getval", scribus_getval, METH_VARARGS, "TODO: docstring"},
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
	// legacy constants
	PyDict_SetItemString(d, "Points", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "Millimeters", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "Inches", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "Picas", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "Portrait", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "Landscape", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "NoFacingPages", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "FacingPages",  Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "FirstPageRight", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "FirstPageLeft", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "LeftAlign", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "RightAlign", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "Centered", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "Forced", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "NoGradient", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "HorizontalGradient", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "VerticalGradient", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "DiagonalGradient", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "CrossDiagonalGradient", Py_BuildValue("i", 4));
	PyDict_SetItemString(d, "RadialGradient", Py_BuildValue("i", 5));
	PyDict_SetItemString(d, "SolidLine", Py_BuildValue("i", Qt::SolidLine));
	PyDict_SetItemString(d, "DashLine", Py_BuildValue("i", Qt::DashLine));
	PyDict_SetItemString(d, "DotLine", Py_BuildValue("i", Qt::DotLine));
	PyDict_SetItemString(d, "DashDotLine", Py_BuildValue("i", Qt::DashDotLine));
	PyDict_SetItemString(d, "DashDotDotLine", Py_BuildValue("i", Qt::DashDotDotLine));
	PyDict_SetItemString(d, "MiterJoin", Py_BuildValue("i", Qt::MiterJoin));
	PyDict_SetItemString(d, "BevelJoin", Py_BuildValue("i", Qt::BevelJoin));
	PyDict_SetItemString(d, "RoundJoin", Py_BuildValue("i", Qt::RoundJoin));
	PyDict_SetItemString(d, "FlatCap", Py_BuildValue("i", Qt::FlatCap));
	PyDict_SetItemString(d, "SquareCap", Py_BuildValue("i", Qt::SquareCap));
	PyDict_SetItemString(d, "RoundCap", Py_BuildValue("i", Qt::RoundCap));
	PyDict_SetItemString(d, "NoButton", Py_BuildValue("i", QMessageBox::NoButton));
	PyDict_SetItemString(d, "Ok", Py_BuildValue("i", QMessageBox::Ok));
	PyDict_SetItemString(d, "Cancel", Py_BuildValue("i", QMessageBox::Cancel));
	PyDict_SetItemString(d, "Yes", Py_BuildValue("i", QMessageBox::Yes));
	PyDict_SetItemString(d, "No", Py_BuildValue("i", QMessageBox::No));
	PyDict_SetItemString(d, "Abort", Py_BuildValue("i", QMessageBox::Abort));
	PyDict_SetItemString(d, "Retry", Py_BuildValue("i", QMessageBox::Retry));
	PyDict_SetItemString(d, "Ignore", Py_BuildValue("i", QMessageBox::Ignore));
	PyDict_SetItemString(d, "NoIcon", Py_BuildValue("i", QMessageBox::NoIcon));
	PyDict_SetItemString(d, "Information", Py_BuildValue("i", QMessageBox::Information));
	PyDict_SetItemString(d, "Warning", Py_BuildValue("i", QMessageBox::Warning));
	PyDict_SetItemString(d, "Critical", Py_BuildValue("i", QMessageBox::Critical));
	PyDict_SetItemString(d, "Paper_A0", Py_BuildValue("(ff)", 2380.0, 3368.0));
	PyDict_SetItemString(d, "Paper_A1", Py_BuildValue("(ff)", 1684.0, 2380.0));
	PyDict_SetItemString(d, "Paper_A2", Py_BuildValue("(ff)", 1190.0, 1684.0));
	PyDict_SetItemString(d, "Paper_A3", Py_BuildValue("(ff)", 842.0, 1190.0));
	PyDict_SetItemString(d, "Paper_A4", Py_BuildValue("(ff)", 595.0, 842.0));
	PyDict_SetItemString(d, "Paper_A5", Py_BuildValue("(ff)", 421.0, 595.0));
	PyDict_SetItemString(d, "Paper_A6", Py_BuildValue("(ff)", 297.0, 421.0));
	PyDict_SetItemString(d, "Paper_A7", Py_BuildValue("(ff)", 210.0, 297.0));
	PyDict_SetItemString(d, "Paper_A8", Py_BuildValue("(ff)", 148.0, 210.0));
	PyDict_SetItemString(d, "Paper_A9", Py_BuildValue("(ff)", 105.0, 148.0));
	PyDict_SetItemString(d, "Paper_B0", Py_BuildValue("(ff)", 2836.0, 4008.0));
	PyDict_SetItemString(d, "Paper_B1", Py_BuildValue("(ff)", 2004.0, 2836.0));
	PyDict_SetItemString(d, "Paper_B2", Py_BuildValue("(ff)", 1418.0, 2004.0));
	PyDict_SetItemString(d, "Paper_B3", Py_BuildValue("(ff)", 1002.0, 1418.0));
	PyDict_SetItemString(d, "Paper_B4", Py_BuildValue("(ff)", 709.0, 1002.0));
	PyDict_SetItemString(d, "Paper_B5", Py_BuildValue("(ff)", 501.0, 709.0));
	PyDict_SetItemString(d, "Paper_B6", Py_BuildValue("(ff)", 355.0, 501.0));
	PyDict_SetItemString(d, "Paper_B7", Py_BuildValue("(ff)", 250.0, 355.0));
	PyDict_SetItemString(d, "Paper_B8", Py_BuildValue("(ff)", 178.0, 250.0));
	PyDict_SetItemString(d, "Paper_B9", Py_BuildValue("(ff)", 125.0, 178.0));
	PyDict_SetItemString(d, "Paper_B10", Py_BuildValue("(ff)", 89.0, 125.0));
	PyDict_SetItemString(d, "Paper_C5E", Py_BuildValue("(ff)", 462.0, 649.0));
	PyDict_SetItemString(d, "Paper_Comm10E", Py_BuildValue("(ff)", 298.0, 683.0));
	PyDict_SetItemString(d, "Paper_DLE",  Py_BuildValue("(ff)", 312.0, 624.0));
	PyDict_SetItemString(d, "Paper_Executive", Py_BuildValue("(ff)", 542.0, 720.0));
	PyDict_SetItemString(d, "Paper_Folio", Py_BuildValue("(ff)", 595.0, 935.0));
	PyDict_SetItemString(d, "Paper_Ledger", Py_BuildValue("(ff)", 1224.0, 792.0));
	PyDict_SetItemString(d, "Paper_Legal", Py_BuildValue("(ff)", 612.0, 1008.0));
	PyDict_SetItemString(d, "Paper_Letter", Py_BuildValue("(ff)", 612.0, 792.0));
	PyDict_SetItemString(d, "Paper_Tabloid", Py_BuildValue("(ff)", 792.0, 1224.0));
	// end of legacy
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
}

