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

#if (_MSC_VER >= 1200)
 #include "win-config.h"
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
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("Online Reference"), pfad2);
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
	QString cm = "import sys\nsys.path[0] = \""+fi.dirPath(true)+"\"\n";
	cm += "try:\n\texecfile(\""+fileName+"\")\nexcept SystemExit:\n\tpass\n";
	QCString cmd = cm.latin1();
	comm[0] = na.data();
	// this code run the script and handles stderr redirection
	PyRun_SimpleString( "import sys, StringIO\nsys.stderr=sys._capture=StringIO.StringIO()\n");
	// call python script
	PySys_SetArgv(1, comm);
	PyRun_SimpleString(cmd.data());
	// and restore stderr
	PyObject* sysmod = PyImport_ImportModule("sys");
	PyObject* capobj = PyObject_GetAttrString(sysmod, "_capture");
	PyObject* strres = PyObject_CallMethod(capobj, "getvalue", 0);
	QString cres = QString(PyString_AsString(strres));
	// just tell the truth :)
	if (cres.length() > 0)
	{
		QClipboard *cp = QApplication::clipboard();
		cp->setText(cres);
		QMessageBox::warning(Carrier,
		                     tr("Script error"),
		                     tr("If you are running an official script report it at <a href=\"http://bugs.scribus.net\">bugs.scribus.net</a> please.")
		                     + "<pre>"
		                     + cres + "</pre>" + tr("This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker."));
	}
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
	outputstream << "<pre>" << endl;
	outputstream << intputstream.read();
	outputstream << "</pre>" << endl;
	output.close();
	input.close();
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("About Script") + " " + fi.fileName(), html);
	dia->show();
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

static PyMethodDef scribus_methods[] = {
	// 2004/10/03 pv - aliases with common Python syntax - ClassName methodName
	// 2004/09/26 pv
	{"ValueDialog", scribus_valdialog, METH_VARARGS, "TODO: docstring"},
	{"valueDialog", scribus_valdialog, METH_VARARGS, "TODO: docstring"},
	{"GetPageSize", scribus_pagedimension, METH_VARARGS, "TODO: docstring"}, // just an alias to PageDimension()
	{"getPageSize", scribus_pagedimension, METH_VARARGS, "TODO: docstring"}, // just an alias to PageDimension()
	{"GetPageMargins", scribus_getpagemargins, METH_VARARGS, "TODO: docstring"},
	{"getPageMargins", scribus_getpagemargins, METH_VARARGS, "TODO: docstring"},
	// 2004/09/13 pv
	{"TraceText", scribus_tracetext, METH_VARARGS, "TODO: docstring"},
	{"traceText", scribus_tracetext, METH_VARARGS, "TODO: docstring"},
	{"LoadStylesFromFile", scribus_loadstylesfromfile, METH_VARARGS, "TODO: docstring"},
	{"loadStylesFromFile", scribus_loadstylesfromfile, METH_VARARGS, "TODO: docstring"},
	{"SetStyle", scribus_setstyle, METH_VARARGS, "TODO: docstring"},
	{"setStyle", scribus_setstyle, METH_VARARGS, "TODO: docstring"},
	{"GetAllStyles", scribus_getstylenames, METH_VARARGS, "TODO: docstring"},
	{"getAllStyles", scribus_getstylenames, METH_VARARGS, "TODO: docstring"},
	// before 2004/09/13 pv
	{"LockObject", scribus_lockobject, METH_VARARGS, "TODO: docstring"},
	{"lockObject", scribus_lockobject, METH_VARARGS, "TODO: docstring"},
	{"IsLocked", scribus_islocked, METH_VARARGS, "TODO: docstring"},
	{"isLocked", scribus_islocked, METH_VARARGS, "TODO: docstring"},
	{"ObjectExists",scribus_objectexists, METH_VARARGS, "TODO: docstring"},
	{"objectExists",scribus_objectexists, METH_VARARGS, "TODO: docstring"},
	{"GetPageItems", scribus_getpageitems, METH_VARARGS, "TODO: docstring"},
	{"getPageItems", scribus_getpageitems, METH_VARARGS, "TODO: docstring"},
	{"TextFlowsAroundFrame", scribus_textflow, METH_VARARGS, "TODO: docstring"},
	{"textFlowsAroundFrame", scribus_textflow, METH_VARARGS, "TODO: docstring"},
	{"GetXFontNames",scribus_xfontnames, METH_VARARGS, "TODO: docstring"},
	{"getXFontNames",scribus_xfontnames, METH_VARARGS, "TODO: docstring"},
	{"LinkTextFrames", scribus_linktextframes, METH_VARARGS, "TODO: docstring"},
	{"linkTextFrames", scribus_linktextframes, METH_VARARGS, "TODO: docstring"},
	{"UnlinkTextFrames", scribus_unlinktextframes, METH_VARARGS, "TODO: docstring"},
	{"unlinkTextFrames", scribus_unlinktextframes, METH_VARARGS, "TODO: docstring"},
	{"ProgressReset", scribus_progressreset, METH_VARARGS, "TODO: docstring"},
	{"progressReset", scribus_progressreset, METH_VARARGS, "TODO: docstring"},
	{"ProgressTotal", scribus_progresssettotalsteps, METH_VARARGS, "TODO: docstring"},
	{"progressTotal", scribus_progresssettotalsteps, METH_VARARGS, "TODO: docstring"},
	{"ProgressSet", scribus_progresssetprogress, METH_VARARGS, "TODO: docstring"},
	{"progressSet", scribus_progresssetprogress, METH_VARARGS, "TODO: docstring"},
	{"MessagebarText", scribus_messagebartext, METH_VARARGS, "TODO: docstring"},
	{"messagebarText", scribus_messagebartext, METH_VARARGS, "TODO: docstring"},
	{"DocChanged", scribus_docchanged, METH_VARARGS, "TODO: docstring"},
	{"docChanged", scribus_docchanged, METH_VARARGS, "TODO: docstring"},
	{"SetCursor", scribus_setcursor, METH_VARARGS, "TODO: docstring"},
	{"setCursor", scribus_setcursor, METH_VARARGS, "TODO: docstring"},
	{"PageDimension", scribus_pagedimension, METH_VARARGS, "TODO: docstring"},
	{"pageDimension", scribus_pagedimension, METH_VARARGS, "TODO: docstring"},
	{"NewDocDialog", scribus_newdocdia, METH_VARARGS, "TODO: docstring"},
	{"newDocDialog", scribus_newdocdia, METH_VARARGS, "TODO: docstring"},
	{"FileDialog", scribus_filedia, METH_VARARGS, "TODO: docstring"},
	{"fileDialog", scribus_filedia, METH_VARARGS, "TODO: docstring"},
	{"StatusMessage", scribus_mess, METH_VARARGS, "TODO: docstring"},
	{"statusMessage", scribus_mess, METH_VARARGS, "TODO: docstring"},
	{"MessageBox", scribus_messdia, METH_VARARGS, "TODO: docstring"},
	{"messageBox", scribus_messdia, METH_VARARGS, "TODO: docstring"},
	{"NewDoc", scribus_newdoc, METH_VARARGS, "Creates a new Document. The Parameters have the following Meaning:\n\tsize = A Tuple (width, height) describing the Size of the Document.\n\tmargins = A Tuple (Left, Right, Top, Bottom) describing the Margins of the Document.\n\torientation = the Page Orientation, 0 means Portrait, 1 is Landscape\n\tfirstPageNumer is the Number of the first Page in the Document used for Pagenumbering\n\tunit: this Value sets the Measurement Unit of the Document\n\n\t0 = Typographic Points\n\t1 = Millimeters\n\t2 = Inches\n\t3 = Picas\n\n\tFacingPages: 1 means FacingPages turned on, 0 means FacingPages turned off\n\tFirstSideLeft: 1 means that the first Page in the Document is a left Page, 0 means a right Page as first Page\n\tThe values for Width, Height and the Margins are expressed in the given unit for the Document."},
	{"newDoc", scribus_newdoc, METH_VARARGS, "Creates a new Document. The Parameters have the following Meaning:\n\tsize = A Tuple (width, height) describing the Size of the Document.\n\tmargins = A Tuple (Left, Right, Top, Bottom) describing the Margins of the Document.\n\torientation = the Page Orientation, 0 means Portrait, 1 is Landscape\n\tfirstPageNumer is the Number of the first Page in the Document used for Pagenumbering\n\tunit: this Value sets the Measurement Unit of the Document\n\n\t0 = Typographic Points\n\t1 = Millimeters\n\t2 = Inches\n\t3 = Picas\n\n\tFacingPages: 1 means FacingPages turned on, 0 means FacingPages turned off\n\tFirstSideLeft: 1 means that the first Page in the Document is a left Page, 0 means a right Page as first Page\n\tThe values for Width, Height and the Margins are expressed in the given unit for the Document."},
	{"CloseDoc", scribus_closedoc, METH_VARARGS, "Closes the current Document. Returns true if successful."},
	{"closeDoc", scribus_closedoc, METH_VARARGS, "Closes the current Document. Returns true if successful."},
	{"HaveDoc", scribus_havedoc, METH_VARARGS, "Returns true if there is a Document open."},
	{"haveDoc", scribus_havedoc, METH_VARARGS, "Returns true if there is a Document open."},
	{"OpenDoc", scribus_opendoc, METH_VARARGS, "Opens the Document \"name\". Returns true if successful."},
	{"openDoc", scribus_opendoc, METH_VARARGS, "Opens the Document \"name\". Returns true if successful."},
	{"SaveDoc", scribus_savedoc, METH_VARARGS, "Saves the Document under its actual Name, returns true if successful."},
	{"saveDoc", scribus_savedoc, METH_VARARGS, "Saves the Document under its actual Name, returns true if successful."},
	{"SaveDocAs", scribus_savedocas, METH_VARARGS, "Saves the actual Document under the new Name \"name\". Returns true if successful."},
	{"saveDocAs", scribus_savedocas, METH_VARARGS, "Saves the actual Document under the new Name \"name\". Returns true if successful."},
	{"SetInfo", scribus_setinfo, METH_VARARGS, "Sets the Document Information. \"Author\", \"Info\", \"Description\""},
	{"setInfo", scribus_setinfo, METH_VARARGS, "Sets the Document Information. \"Author\", \"Info\", \"Description\""},
	{"SetMargins", scribus_setmargins, METH_VARARGS, "Sets the Print margins of the Document, Left(lr), Right(rr), Top(tr) and Bottom(br) Margins are given in the measurement unit of the Document."},
	{"setMargins", scribus_setmargins, METH_VARARGS, "Sets the Print margins of the Document, Left(lr), Right(rr), Top(tr) and Bottom(br) Margins are given in the measurement unit of the Document."},
	{"SetUnit", scribus_setunit, METH_VARARGS, "Changes the Measurement Unit of the Document. Possible Values for Unit are:\n\t0 = Typographic Points\n\t1 = Millimeters\n\t2 = Inches\n\t3 = Picas "},
	{"setUnit", scribus_setunit, METH_VARARGS, "Changes the Measurement Unit of the Document. Possible Values for Unit are:\n\t0 = Typographic Points\n\t1 = Millimeters\n\t2 = Inches\n\t3 = Picas "},
	{"GetUnit", scribus_getunit, METH_VARARGS, "Returns the Measurement Unit of the Document.\nPossible Values for Unit are:\n\t0 = Typographic Points\n\t1 = Millimeters\n\t2 = Inches\n\t3 = Picas "},
	{"getUnit", scribus_getunit, METH_VARARGS, "Returns the Measurement Unit of the Document.\nPossible Values for Unit are:\n\t0 = Typographic Points\n\t1 = Millimeters\n\t2 = Inches\n\t3 = Picas "},
	{"CurrentPage", scribus_actualpage, METH_VARARGS, "TODO: docstring"},
	{"currentPage", scribus_actualpage, METH_VARARGS, "TODO: docstring"},
	{"SetRedraw", scribus_setredraw, METH_VARARGS, "TODO: docstring"},
	{"setRedraw", scribus_setredraw, METH_VARARGS, "TODO: docstring"},
	{"RedrawAll", scribus_redraw, METH_VARARGS, "TODO: docstring"},
	{"redrawAll", scribus_redraw, METH_VARARGS, "TODO: docstring"},
	{"SavePageAsEPS", scribus_savepageeps, METH_VARARGS, "TODO: docstring"},
	{"savePageAsEPS", scribus_savepageeps, METH_VARARGS, "TODO: docstring"},
	{"NewPage", scribus_newpage, METH_VARARGS, "TODO: docstring"},
	{"newPage", scribus_newpage, METH_VARARGS, "TODO: docstring"},
	{"DeletePage", scribus_deletepage, METH_VARARGS, "TODO: docstring"},
	{"deletePage", scribus_deletepage, METH_VARARGS, "TODO: docstring"},
	{"GotoPage", scribus_gotopage, METH_VARARGS, "TODO: docstring"},
	{"gotoPage", scribus_gotopage, METH_VARARGS, "TODO: docstring"},
	{"PageCount", scribus_pagecount, METH_VARARGS, "TODO: docstring"},
	{"pageCount", scribus_pagecount, METH_VARARGS, "TODO: docstring"},
	{"CreateRect", scribus_newrect, METH_VARARGS, "TODO: docstring"},
	{"createRect", scribus_newrect, METH_VARARGS, "TODO: docstring"},
	{"CreateEllipse", scribus_newellipse, METH_VARARGS, "TODO: docstring"},
	{"createEllipse", scribus_newellipse, METH_VARARGS, "TODO: docstring"},
	{"CreateImage", scribus_newimage, METH_VARARGS, "TODO: docstring"},
	{"createImage", scribus_newimage, METH_VARARGS, "TODO: docstring"},
	{"CreateLine", scribus_newline, METH_VARARGS, "TODO: docstring"},
	{"createLine", scribus_newline, METH_VARARGS, "TODO: docstring"},
	{"CreatePolyLine", scribus_polyline, METH_VARARGS, "TODO: docstring"},
	{"createPolyLine", scribus_polyline, METH_VARARGS, "TODO: docstring"},
	{"CreatePolygon", scribus_polygon, METH_VARARGS, "TODO: docstring"},
	{"createPolygon", scribus_polygon, METH_VARARGS, "TODO: docstring"},
	{"CreateBezierLine", scribus_bezierline, METH_VARARGS, "TODO: docstring"},
	{"createBezierLine", scribus_bezierline, METH_VARARGS, "TODO: docstring"},
	{"CreatePathText", scribus_pathtext, METH_VARARGS, "TODO: docstring"},
	{"createPathText", scribus_pathtext, METH_VARARGS, "TODO: docstring"},
	{"CreateText", scribus_newtext, METH_VARARGS, "TODO: docstring"},
	{"createText", scribus_newtext, METH_VARARGS, "TODO: docstring"},
	{"DeleteObject", scribus_deleteobj, METH_VARARGS, "TODO: docstring"},
	{"deleteObject", scribus_deleteobj, METH_VARARGS, "TODO: docstring"},
	{"GetFillColor", scribus_getfillcolor, METH_VARARGS, "TODO: docstring"},
	{"getFillColor", scribus_getfillcolor, METH_VARARGS, "TODO: docstring"},
	{"GetLineColor", scribus_getlinecolor, METH_VARARGS, "TODO: docstring"},
	{"getLineColor", scribus_getlinecolor, METH_VARARGS, "TODO: docstring"},
	{"GetLineWidth", scribus_getlinewidth, METH_VARARGS, "TODO: docstring"},
	{"getLineWidth", scribus_getlinewidth, METH_VARARGS, "TODO: docstring"},
	{"GetLineShade", scribus_getlineshade, METH_VARARGS, "TODO: docstring"},
	{"getLineShade", scribus_getlineshade, METH_VARARGS, "TODO: docstring"},
	{"GetLineJoin", scribus_getlinejoin, METH_VARARGS, "TODO: docstring"},
	{"getLineJoin", scribus_getlinejoin, METH_VARARGS, "TODO: docstring"},
	{"GetLineCap", scribus_getlineend, METH_VARARGS, "TODO: docstring"},
	{"getLineCap", scribus_getlineend, METH_VARARGS, "TODO: docstring"},
	{"GetLineStyle", scribus_getlinestyle, METH_VARARGS, "TODO: docstring"},
	{"getLineStyle", scribus_getlinestyle, METH_VARARGS, "TODO: docstring"},
	{"GetFillShade", scribus_getfillshade, METH_VARARGS, "TODO: docstring"},
	{"getFillShade", scribus_getfillshade, METH_VARARGS, "TODO: docstring"},
	{"GetCornerRadius", scribus_getcornerrad, METH_VARARGS, "TODO: docstring"},
	{"getCornerRadius", scribus_getcornerrad, METH_VARARGS, "TODO: docstring"},
	{"GetImageScale", scribus_getimgscale, METH_VARARGS, "TODO: docstring"},
	{"getImageScale", scribus_getimgscale, METH_VARARGS, "TODO: docstring"},
	{"GetImageFile", scribus_getimgname, METH_VARARGS, "TODO: docstring"},
	{"getImageFile", scribus_getimgname, METH_VARARGS, "TODO: docstring"},
	{"GetPosition", scribus_getposi, METH_VARARGS, "TODO: docstring"},
	{"getPosition", scribus_getposi, METH_VARARGS, "TODO: docstring"},
	{"GetSize", scribus_getsize, METH_VARARGS, "TODO: docstring"},
	{"getSize", scribus_getsize, METH_VARARGS, "TODO: docstring"},
	{"GetRotation", scribus_getrotation, METH_VARARGS, "TODO: docstring"},
	{"getRotation", scribus_getrotation, METH_VARARGS, "TODO: docstring"},
	{"GetFontSize", scribus_getfontsize, METH_VARARGS, "TODO: docstring"},
	{"getFontSize", scribus_getfontsize, METH_VARARGS, "TODO: docstring"},
	{"GetFont", scribus_getfont, METH_VARARGS, "TODO: docstring"},
	{"getFont", scribus_getfont, METH_VARARGS, "TODO: docstring"},
	{"GetTextLength", scribus_gettextsize, METH_VARARGS, "TODO: docstring"},
	{"getTextLength", scribus_gettextsize, METH_VARARGS, "TODO: docstring"},
	{"GetTextColor", scribus_getlinecolor, METH_VARARGS, "TODO: docstring"},
	{"getTextColor", scribus_getlinecolor, METH_VARARGS, "TODO: docstring"},
	{"GetTextShade", scribus_getlineshade, METH_VARARGS, "TODO: docstring"},
	{"getTextShade", scribus_getlineshade, METH_VARARGS, "TODO: docstring"},
	{"GetColumns", scribus_getcolumns, METH_VARARGS, "TODO: docstring"},
	{"getColumns", scribus_getcolumns, METH_VARARGS, "TODO: docstring"},
	{"GetColumnGap", scribus_getcolumngap, METH_VARARGS, "TODO: docstring"},
	{"getColumnGap", scribus_getcolumngap, METH_VARARGS, "TODO: docstring"},
	{"GetLineSpacing", scribus_getlinespace, METH_VARARGS, "TODO: docstring"},
	{"getLineSpacing", scribus_getlinespace, METH_VARARGS, "TODO: docstring"},
	{"GetText", scribus_getframetext, METH_VARARGS, "TODO: docstring"},
	{"getText", scribus_getframetext, METH_VARARGS, "TODO: docstring"},
	{"GetAllText", scribus_gettext, METH_VARARGS, "TODO: docstring"},
	{"getAllText", scribus_gettext, METH_VARARGS, "TODO: docstring"},
	{"GetAllObjects", scribus_getallobj, METH_VARARGS, "TODO: docstring"},
	{"getAllObjects", scribus_getallobj, METH_VARARGS, "TODO: docstring"},
	{"SetGradientFill", scribus_setgradfill, METH_VARARGS, "TODO: docstring"},
	{"setGradientFill", scribus_setgradfill, METH_VARARGS, "TODO: docstring"},
	{"SetFillColor", scribus_setfillcolor, METH_VARARGS, "TODO: docstring"},
	{"setFillColor", scribus_setfillcolor, METH_VARARGS, "TODO: docstring"},
	{"SetLineColor", scribus_setlinecolor, METH_VARARGS, "TODO: docstring"},
	{"setLineColor", scribus_setlinecolor, METH_VARARGS, "TODO: docstring"},
	{"SetMultiLine", scribus_setmultiline, METH_VARARGS, "TODO: docstring"},
	{"setMultiLine", scribus_setmultiline, METH_VARARGS, "TODO: docstring"},
	// aliases below
	{"SetLineWidth", scribus_setlinewidth, METH_VARARGS, "TODO: docstring"},
	{"SetLineShade", scribus_setlineshade, METH_VARARGS, "TODO: docstring"},
	{"SetLineJoin", scribus_setlinejoin, METH_VARARGS, "TODO: docstring"},
	{"SetLineCap", scribus_setlineend, METH_VARARGS, "TODO: docstring"},
	{"SetLineStyle", scribus_setlinestyle, METH_VARARGS, "TODO: docstring"},
	{"SetFillShade", scribus_setfillshade, METH_VARARGS, "TODO: docstring"},
	{"SetCornerRadius", scribus_setcornerrad, METH_VARARGS, "TODO: docstring"},
	{"LoadImage", scribus_loadimage, METH_VARARGS, "TODO: docstring"},
	{"ScaleImage", scribus_scaleimage, METH_VARARGS, "TODO: docstring"},
	{"SetText", scribus_setboxtext, METH_VARARGS, "TODO: docstring"},
	{"InsertText", scribus_inserttext, METH_VARARGS, "TODO: docstring"},
	{"SelectText", scribus_selecttext, METH_VARARGS, "TODO: docstring"},
	{"DeleteText", scribus_deletetext, METH_VARARGS, "TODO: docstring"},
	{"SetFont", scribus_setfont, METH_VARARGS, "TODO: docstring"},
	{"SetFontSize", scribus_setfontsize, METH_VARARGS, "TODO: docstring"},
	{"SetTextColor", scribus_settextfill, METH_VARARGS, "TODO: docstring"},
	{"SetTextStroke", scribus_settextstroke, METH_VARARGS, "TODO: docstring"},
	{"SetTextShade", scribus_settextshade, METH_VARARGS, "TODO: docstring"},
 	{"SetColumns", scribus_setcolumns, METH_VARARGS, "TODO: docstring"},
	{"SetColumnGap", scribus_setcolumngap, METH_VARARGS, "TODO: docstring"},
	{"SetLineSpacing", scribus_setlinespace, METH_VARARGS, "TODO: docstring"},
	{"SetTextAlignment", scribus_setalign, METH_VARARGS, "TODO: docstring"},
	{"MoveObject", scribus_moveobjrel, METH_VARARGS, "TODO: docstring"},
	{"MoveObjectAbs", scribus_moveobjabs, METH_VARARGS, "TODO: docstring"},
	{"RotateObject", scribus_rotobjrel, METH_VARARGS, "TODO: docstring"},
	{"RotateObjectAbs", scribus_rotobjabs, METH_VARARGS, "TODO: docstring"},
	{"SizeObject", scribus_sizeobjabs, METH_VARARGS, "TODO: docstring"},
	{"GroupObjects", scribus_groupobj, METH_VARARGS, "TODO: docstring"},
	{"UnGroupObject", scribus_ungroupobj, METH_VARARGS, "TODO: docstring"},
	{"ScaleGroup", scribus_scalegroup, METH_VARARGS, "TODO: docstring"},
	{"GetSelectedObject", scribus_getselobjnam, METH_VARARGS, "TODO: docstring"},
	{"SelectionCount", scribus_selcount, METH_VARARGS, "TODO: docstring"},
	{"SelectObject", scribus_selectobj, METH_VARARGS, "TODO: docstring"},
	{"DeselectAll", scribus_deselect, METH_VARARGS, "TODO: docstring"},
	{"GetColorNames", scribus_colornames, METH_VARARGS, "TODO: docstring"},
	{"GetColor", scribus_getcolor, METH_VARARGS, "TODO: docstring"},
	{"ChangeColor", scribus_setcolor, METH_VARARGS, "TODO: docstring"},
	{"DefineColor", scribus_newcolor, METH_VARARGS, "TODO: docstring"},
	{"DeleteColor", scribus_delcolor, METH_VARARGS, "TODO: docstring"},
	{"ReplaceColor", scribus_replcolor, METH_VARARGS, "TODO: docstring"},
	{"GetFontNames", scribus_fontnames, METH_VARARGS, "TODO: docstring"},
	{"RenderFont", scribus_renderfont, METH_VARARGS, "TODO: docstring"},
	{"GetLayers", scribus_getlayers, METH_VARARGS, "TODO: docstring"},
	{"SetActiveLayer", scribus_setactlayer, METH_VARARGS, "TODO: docstring"},
	{"GetActiveLayer", scribus_getactlayer, METH_VARARGS, "TODO: docstring"},
	{"SentToLayer", scribus_senttolayer, METH_VARARGS, "TODO: docstring"},
	{"SetLayerVisible", scribus_layervisible, METH_VARARGS, "TODO: docstring"},
	{"SetLayerPrintable", scribus_layerprint, METH_VARARGS, "TODO: docstring"},
	{"IsLayerVisible", scribus_glayervisib, METH_VARARGS, "TODO: docstring"},
	{"IsLayerPrintable", scribus_glayerprint, METH_VARARGS, "TODO: docstring"},
	{"CreateLayer", scribus_createlayer, METH_VARARGS, "TODO: docstring"},
	{"DeleteLayer", scribus_removelayer, METH_VARARGS, "TODO: docstring"},
	{"GetGuiLanguage", scribus_getlanguage, METH_VARARGS, "TODO: docstring"},
 	{"GetHGuides", scribus_getHguides, METH_VARARGS, "TODO: docstring"},
 	{"SetHGuides", scribus_setHguides, METH_VARARGS, "TODO: docstring"},
 	{"GetVGuides", scribus_getVguides, METH_VARARGS, "TODO: docstring"},
	{"SetVGuides", scribus_setVguides, METH_VARARGS, "TODO: docstring"},
	{"SetDocType", scribus_setdoctype, METH_VARARGS, "Sets the Type of the Documents, to get Facing Pages set the first Parameter to 1, to switch FacingPages off use 0 instead. If you want to be the first Page a left Side set the second Parameter to 1, for a right Page use 0."},
	// aliases
	{"setLineWidth", scribus_setlinewidth, METH_VARARGS, "TODO: docstring"},
	{"setLineShade", scribus_setlineshade, METH_VARARGS, "TODO: docstring"},
	{"setLineJoin", scribus_setlinejoin, METH_VARARGS, "TODO: docstring"},
	{"setLineCap", scribus_setlineend, METH_VARARGS, "TODO: docstring"},
	{"setLineStyle", scribus_setlinestyle, METH_VARARGS, "TODO: docstring"},
	{"setFillShade", scribus_setfillshade, METH_VARARGS, "TODO: docstring"},
	{"setCornerRadius", scribus_setcornerrad, METH_VARARGS, "TODO: docstring"},
	{"loadImage", scribus_loadimage, METH_VARARGS, "TODO: docstring"},
	{"scaleImage", scribus_scaleimage, METH_VARARGS, "TODO: docstring"},
	{"setText", scribus_setboxtext, METH_VARARGS, "TODO: docstring"},
	{"insertText", scribus_inserttext, METH_VARARGS, "TODO: docstring"},
	{"selectText", scribus_selecttext, METH_VARARGS, "TODO: docstring"},
	{"deleteText", scribus_deletetext, METH_VARARGS, "TODO: docstring"},
	{"setFont", scribus_setfont, METH_VARARGS, "TODO: docstring"},
	{"setFontSize", scribus_setfontsize, METH_VARARGS, "TODO: docstring"},
	{"setTextColor", scribus_settextfill, METH_VARARGS, "TODO: docstring"},
	{"setTextStroke", scribus_settextstroke, METH_VARARGS, "TODO: docstring"},
	{"setTextShade", scribus_settextshade, METH_VARARGS, "TODO: docstring"},
	{"setColumns", scribus_setcolumns, METH_VARARGS, "TODO: docstring"},
	{"setColumnGap", scribus_setcolumngap, METH_VARARGS, "TODO: docstring"},
	{"setLineSpacing", scribus_setlinespace, METH_VARARGS, "TODO: docstring"},
	{"setTextAlignment", scribus_setalign, METH_VARARGS, "TODO: docstring"},
	{"moveObject", scribus_moveobjrel, METH_VARARGS, "TODO: docstring"},
	{"moveObjectAbs", scribus_moveobjabs, METH_VARARGS, "TODO: docstring"},
	{"rotateObject", scribus_rotobjrel, METH_VARARGS, "TODO: docstring"},
	{"rotateObjectAbs", scribus_rotobjabs, METH_VARARGS, "TODO: docstring"},
	{"sizeObject", scribus_sizeobjabs, METH_VARARGS, "TODO: docstring"},
	{"groupObjects", scribus_groupobj, METH_VARARGS, "TODO: docstring"},
	{"unGroupObject", scribus_ungroupobj, METH_VARARGS, "TODO: docstring"},
	{"scaleGroup", scribus_scalegroup, METH_VARARGS, "TODO: docstring"},
	{"setSelectedObject", scribus_getselobjnam, METH_VARARGS, "TODO: docstring"},
	{"selectionCount", scribus_selcount, METH_VARARGS, "TODO: docstring"},
	{"selectObject", scribus_selectobj, METH_VARARGS, "TODO: docstring"},
	{"deselectAll", scribus_deselect, METH_VARARGS, "TODO: docstring"},
	{"getColorNames", scribus_colornames, METH_VARARGS, "TODO: docstring"},
	{"getColor", scribus_getcolor, METH_VARARGS, "TODO: docstring"},
	{"changeColor", scribus_setcolor, METH_VARARGS, "TODO: docstring"},
	{"defineColor", scribus_newcolor, METH_VARARGS, "TODO: docstring"},
	{"deleteColor", scribus_delcolor, METH_VARARGS, "TODO: docstring"},
	{"replaceColor", scribus_replcolor, METH_VARARGS, "TODO: docstring"},
	{"getFontNames", scribus_fontnames, METH_VARARGS, "TODO: docstring"},
	{"renderFont", scribus_renderfont, METH_VARARGS, "TODO: docstring"},
	{"getLayers", scribus_getlayers, METH_VARARGS, "TODO: docstring"},
	{"setActiveLayer", scribus_setactlayer, METH_VARARGS, "TODO: docstring"},
	{"getActiveLayer", scribus_getactlayer, METH_VARARGS, "TODO: docstring"},
	{"sentToLayer", scribus_senttolayer, METH_VARARGS, "TODO: docstring"},
	{"setLayerVisible", scribus_layervisible, METH_VARARGS, "TODO: docstring"},
	{"setLayerPrintable", scribus_layerprint, METH_VARARGS, "TODO: docstring"},
	{"isLayerVisible", scribus_glayervisib, METH_VARARGS, "TODO: docstring"},
	{"isLayerPrintable", scribus_glayerprint, METH_VARARGS, "TODO: docstring"},
	{"createLayer", scribus_createlayer, METH_VARARGS, "TODO: docstring"},
	{"deleteLayer", scribus_removelayer, METH_VARARGS, "TODO: docstring"},
	{"getGuiLanguage", scribus_getlanguage, METH_VARARGS, "TODO: docstring"},
	{"getHGuides", scribus_getHguides, METH_VARARGS, "TODO: docstring"},
	{"setHGuides", scribus_setHguides, METH_VARARGS, "TODO: docstring"},
	{"getVGuides", scribus_getVguides, METH_VARARGS, "TODO: docstring"},
	{"setVGuides", scribus_setVguides, METH_VARARGS, "TODO: docstring"},
	{"setDocType", scribus_setdoctype, METH_VARARGS, "Sets the Type of the Documents, to get Facing Pages set the first Parameter to 1, to switch FacingPages off use 0 instead. If you want to be the first Page a left Side set the second Parameter to 1, for a right Page use 0."},
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
}

