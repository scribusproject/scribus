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

/****************************************************************************************/
/*                                                                                      */
/*   Definitionen der Python-Befehle                                                    */
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
	{"ValueDialog", scribus_valdialog, METH_VARARGS},
	{"valueDialog", scribus_valdialog, METH_VARARGS},
	{"GetPageSize", scribus_pagedimension, METH_VARARGS}, // just an alias to PageDimension()
	{"getPageSize", scribus_pagedimension, METH_VARARGS}, // just an alias to PageDimension()
	{"GetPageMargins", scribus_getpagemargins, METH_VARARGS},
	{"getPageMargins", scribus_getpagemargins, METH_VARARGS},
	// 2004/09/13 pv
	{"TraceText", scribus_tracetext, METH_VARARGS},
	{"traceText", scribus_tracetext, METH_VARARGS},
	{"LoadStylesFromFile", scribus_loadstylesfromfile, METH_VARARGS},
	{"loadStylesFromFile", scribus_loadstylesfromfile, METH_VARARGS},
	{"SetStyle", scribus_setstyle, METH_VARARGS},
	{"setStyle", scribus_setstyle, METH_VARARGS},
	{"GetAllStyles", scribus_getstylenames, METH_VARARGS},
	{"getAllStyles", scribus_getstylenames, METH_VARARGS},
	// before 2004/09/13 pv
	{"LockObject", scribus_lockobject, METH_VARARGS},
	{"lockObject", scribus_lockobject, METH_VARARGS},
	{"IsLocked", scribus_islocked, METH_VARARGS},
	{"isLocked", scribus_islocked, METH_VARARGS},
	{"ObjectExists",scribus_objectexists, METH_VARARGS},
	{"objectExists",scribus_objectexists, METH_VARARGS},
	{"GetPageItems", scribus_getpageitems, METH_VARARGS},
	{"getPageItems", scribus_getpageitems, METH_VARARGS},
	{"TextFlowsAroundFrame", scribus_textflow, METH_VARARGS},
	{"textFlowsAroundFrame", scribus_textflow, METH_VARARGS},
	{"GetXFontNames",scribus_xfontnames, METH_VARARGS},
	{"getXFontNames",scribus_xfontnames, METH_VARARGS},
	{"LinkTextFrames", scribus_linktextframes, METH_VARARGS},
	{"linkTextFrames", scribus_linktextframes, METH_VARARGS},
	{"UnlinkTextFrames", scribus_unlinktextframes, METH_VARARGS},
	{"unlinkTextFrames", scribus_unlinktextframes, METH_VARARGS},
	{"ProgressReset", scribus_progressreset, METH_VARARGS},
	{"progressReset", scribus_progressreset, METH_VARARGS},
	{"ProgressTotal", scribus_progresssettotalsteps, METH_VARARGS},
	{"progressTotal", scribus_progresssettotalsteps, METH_VARARGS},
	{"ProgressSet", scribus_progresssetprogress, METH_VARARGS},
	{"progressSet", scribus_progresssetprogress, METH_VARARGS},
	{"MessagebarText", scribus_messagebartext, METH_VARARGS},
	{"messagebarText", scribus_messagebartext, METH_VARARGS},
	{"DocChanged", scribus_docchanged, METH_VARARGS},
	{"docChanged", scribus_docchanged, METH_VARARGS},
	{"SetCursor", scribus_setcursor, METH_VARARGS},
	{"setCursor", scribus_setcursor, METH_VARARGS},
	{"PageDimension", scribus_pagedimension, METH_VARARGS},
	{"pageDimension", scribus_pagedimension, METH_VARARGS},
	{"NewDocDialog", scribus_newdocdia, METH_VARARGS},
	{"newDocDialog", scribus_newdocdia, METH_VARARGS},
	{"FileDialog", scribus_filedia, METH_VARARGS},
	{"fileDialog", scribus_filedia, METH_VARARGS},
	{"StatusMessage", scribus_mess, METH_VARARGS},
	{"statusMessage", scribus_mess, METH_VARARGS},
	{"MessageBox", scribus_messdia, METH_VARARGS},
	{"messageBox", scribus_messdia, METH_VARARGS},
	{"NewDoc", scribus_newdoc, METH_VARARGS},
	{"newDoc", scribus_newdoc, METH_VARARGS},
	{"CloseDoc", scribus_closedoc, METH_VARARGS},
	{"closeDoc", scribus_closedoc, METH_VARARGS},
	{"HaveDoc", scribus_havedoc, METH_VARARGS},
	{"haveDoc", scribus_havedoc, METH_VARARGS},
	{"OpenDoc", scribus_opendoc, METH_VARARGS},
	{"openDoc", scribus_opendoc, METH_VARARGS},
	{"SaveDoc", scribus_savedoc, METH_VARARGS},
	{"saveDoc", scribus_savedoc, METH_VARARGS},
	{"SaveDocAs", scribus_savedocas, METH_VARARGS},
	{"saveDocAs", scribus_savedocas, METH_VARARGS},
	{"SetInfo", scribus_setinfo, METH_VARARGS},
	{"setInfo", scribus_setinfo, METH_VARARGS},
	{"SetMargins", scribus_setmargins, METH_VARARGS},
	{"setMargins", scribus_setmargins, METH_VARARGS},
	{"SetUnit", scribus_setunit, METH_VARARGS},
	{"setUnit", scribus_setunit, METH_VARARGS},
	{"GetUnit", scribus_getunit, METH_VARARGS},
	{"getUnit", scribus_getunit, METH_VARARGS},
	{"CurrentPage", scribus_actualpage, METH_VARARGS},
	{"currentPage", scribus_actualpage, METH_VARARGS},
	{"SetRedraw", scribus_setredraw, METH_VARARGS},
	{"setRedraw", scribus_setredraw, METH_VARARGS},
	{"RedrawAll", scribus_redraw, METH_VARARGS},
	{"redrawAll", scribus_redraw, METH_VARARGS},
	{"SavePageAsEPS", scribus_savepageeps, METH_VARARGS},
	{"savePageAsEPS", scribus_savepageeps, METH_VARARGS},
	{"NewPage", scribus_newpage, METH_VARARGS},
	{"newPage", scribus_newpage, METH_VARARGS},
	{"DeletePage", scribus_deletepage, METH_VARARGS},
	{"deletePage", scribus_deletepage, METH_VARARGS},
	{"GotoPage", scribus_gotopage, METH_VARARGS},
	{"gotoPage", scribus_gotopage, METH_VARARGS},
	{"PageCount", scribus_pagecount, METH_VARARGS},
	{"pageCount", scribus_pagecount, METH_VARARGS},
	{"CreateRect", scribus_newrect, METH_VARARGS},
	{"createRect", scribus_newrect, METH_VARARGS},
	{"CreateEllipse", scribus_newellipse, METH_VARARGS},
	{"createEllipse", scribus_newellipse, METH_VARARGS},
	{"CreateImage", scribus_newimage, METH_VARARGS},
	{"createImage", scribus_newimage, METH_VARARGS},
	{"CreateLine", scribus_newline, METH_VARARGS},
	{"createLine", scribus_newline, METH_VARARGS},
	{"CreatePolyLine", scribus_polyline, METH_VARARGS},
	{"createPolyLine", scribus_polyline, METH_VARARGS},
	{"CreatePolygon", scribus_polygon, METH_VARARGS},
	{"createPolygon", scribus_polygon, METH_VARARGS},
	{"CreateBezierLine", scribus_bezierline, METH_VARARGS},
	{"createBezierLine", scribus_bezierline, METH_VARARGS},
	{"CreatePathText", scribus_pathtext, METH_VARARGS},
	{"createPathText", scribus_pathtext, METH_VARARGS},
	{"CreateText", scribus_newtext, METH_VARARGS},
	{"createText", scribus_newtext, METH_VARARGS},
	{"DeleteObject", scribus_deleteobj, METH_VARARGS},
	{"deleteObject", scribus_deleteobj, METH_VARARGS},
	{"GetFillColor", scribus_getfillcolor, METH_VARARGS},
	{"getFillColor", scribus_getfillcolor, METH_VARARGS},
	{"GetLineColor", scribus_getlinecolor, METH_VARARGS},
	{"getLineColor", scribus_getlinecolor, METH_VARARGS},
	{"GetLineWidth", scribus_getlinewidth, METH_VARARGS},
	{"getLineWidth", scribus_getlinewidth, METH_VARARGS},
	{"GetLineShade", scribus_getlineshade, METH_VARARGS},
	{"getLineShade", scribus_getlineshade, METH_VARARGS},
	{"GetLineJoin", scribus_getlinejoin, METH_VARARGS},
	{"getLineJoin", scribus_getlinejoin, METH_VARARGS},
	{"GetLineCap", scribus_getlineend, METH_VARARGS},
	{"getLineCap", scribus_getlineend, METH_VARARGS},
	{"GetLineStyle", scribus_getlinestyle, METH_VARARGS},
	{"getLineStyle", scribus_getlinestyle, METH_VARARGS},
	{"GetFillShade", scribus_getfillshade, METH_VARARGS},
	{"getFillShade", scribus_getfillshade, METH_VARARGS},
	{"GetCornerRadius", scribus_getcornerrad, METH_VARARGS},
	{"getCornerRadius", scribus_getcornerrad, METH_VARARGS},
	{"GetImageScale", scribus_getimgscale, METH_VARARGS},
	{"getImageScale", scribus_getimgscale, METH_VARARGS},
	{"GetImageFile", scribus_getimgname, METH_VARARGS},
	{"getImageFile", scribus_getimgname, METH_VARARGS},
	{"GetPosition", scribus_getposi, METH_VARARGS},
	{"getPosition", scribus_getposi, METH_VARARGS},
	{"GetSize", scribus_getsize, METH_VARARGS},
	{"getSize", scribus_getsize, METH_VARARGS},
	{"GetRotation", scribus_getrotation, METH_VARARGS},
	{"getRotation", scribus_getrotation, METH_VARARGS},
	{"GetFontSize", scribus_getfontsize, METH_VARARGS},
	{"getFontSize", scribus_getfontsize, METH_VARARGS},
	{"GetFont", scribus_getfont, METH_VARARGS},
	{"getFont", scribus_getfont, METH_VARARGS},
	{"GetTextLength", scribus_gettextsize, METH_VARARGS},
	{"getTextLength", scribus_gettextsize, METH_VARARGS},
	{"GetTextColor", scribus_getlinecolor, METH_VARARGS},
	{"getTextColor", scribus_getlinecolor, METH_VARARGS},
	{"GetTextShade", scribus_getlineshade, METH_VARARGS},
	{"getTextShade", scribus_getlineshade, METH_VARARGS},
	{"GetColumns", scribus_getcolumns, METH_VARARGS},
	{"getColumns", scribus_getcolumns, METH_VARARGS},
	{"GetColumnGap", scribus_getcolumngap, METH_VARARGS},
	{"getColumnGap", scribus_getcolumngap, METH_VARARGS},
	{"GetLineSpacing", scribus_getlinespace, METH_VARARGS},
	{"getLineSpacing", scribus_getlinespace, METH_VARARGS},
	{"GetText", scribus_getframetext, METH_VARARGS},
	{"getText", scribus_getframetext, METH_VARARGS},
	{"GetAllText", scribus_gettext, METH_VARARGS},
	{"getAllText", scribus_gettext, METH_VARARGS},
	{"GetAllObjects", scribus_getallobj, METH_VARARGS},
	{"getAllObjects", scribus_getallobj, METH_VARARGS},
	{"SetGradientFill", scribus_setgradfill, METH_VARARGS},
	{"setGradientFill", scribus_setgradfill, METH_VARARGS},
	{"SetFillColor", scribus_setfillcolor, METH_VARARGS},
	{"setFillColor", scribus_setfillcolor, METH_VARARGS},
	{"SetLineColor", scribus_setlinecolor, METH_VARARGS},
	{"setLineColor", scribus_setlinecolor, METH_VARARGS},
	{"SetMultiLine", scribus_setmultiline, METH_VARARGS},
	{"setMultiLine", scribus_setmultiline, METH_VARARGS},
	// aliases below
	{"SetLineWidth", scribus_setlinewidth, METH_VARARGS},
	{"SetLineShade", scribus_setlineshade, METH_VARARGS},
	{"SetLineJoin", scribus_setlinejoin, METH_VARARGS},
	{"SetLineCap", scribus_setlineend, METH_VARARGS},
	{"SetLineStyle", scribus_setlinestyle, METH_VARARGS},
	{"SetFillShade", scribus_setfillshade, METH_VARARGS},
	{"SetCornerRadius", scribus_setcornerrad, METH_VARARGS},
	{"LoadImage", scribus_loadimage, METH_VARARGS},
	{"ScaleImage", scribus_scaleimage, METH_VARARGS},
	{"SetText", scribus_setboxtext, METH_VARARGS},
	{"InsertText", scribus_inserttext, METH_VARARGS},
	{"SelectText", scribus_selecttext, METH_VARARGS},
	{"DeleteText", scribus_deletetext, METH_VARARGS},
	{"SetFont", scribus_setfont, METH_VARARGS},
	{"SetFontSize", scribus_setfontsize, METH_VARARGS},
	{"SetTextColor", scribus_settextfill, METH_VARARGS},
	{"SetTextStroke", scribus_settextstroke, METH_VARARGS},
	{"SetTextShade", scribus_settextshade, METH_VARARGS},
 	{"SetColumns", scribus_setcolumns, METH_VARARGS},
	{"SetColumnGap", scribus_setcolumngap, METH_VARARGS},
	{"SetLineSpacing", scribus_setlinespace, METH_VARARGS},
	{"SetTextAlignment", scribus_setalign, METH_VARARGS},
	{"MoveObject", scribus_moveobjrel, METH_VARARGS},
	{"MoveObjectAbs", scribus_moveobjabs, METH_VARARGS},
	{"RotateObject", scribus_rotobjrel, METH_VARARGS},
	{"RotateObjectAbs", scribus_rotobjabs, METH_VARARGS},
	{"SizeObject", scribus_sizeobjabs, METH_VARARGS},
	{"GroupObjects", scribus_groupobj, METH_VARARGS},
	{"UnGroupObject", scribus_ungroupobj, METH_VARARGS},
	{"ScaleGroup", scribus_scalegroup, METH_VARARGS},
	{"GetSelectedObject", scribus_getselobjnam, METH_VARARGS},
	{"SelectionCount", scribus_selcount, METH_VARARGS},
	{"SelectObject", scribus_selectobj, METH_VARARGS},
	{"DeselectAll", scribus_deselect, METH_VARARGS},
	{"GetColorNames", scribus_colornames, METH_VARARGS},
	{"GetColor", scribus_getcolor, METH_VARARGS},
	{"ChangeColor", scribus_setcolor, METH_VARARGS},
	{"DefineColor", scribus_newcolor, METH_VARARGS},
	{"DeleteColor", scribus_delcolor, METH_VARARGS},
	{"ReplaceColor", scribus_replcolor, METH_VARARGS},
	{"GetFontNames", scribus_fontnames, METH_VARARGS},
	{"RenderFont", scribus_renderfont, METH_VARARGS},
	{"GetLayers", scribus_getlayers, METH_VARARGS},
	{"SetActiveLayer", scribus_setactlayer, METH_VARARGS},
	{"GetActiveLayer", scribus_getactlayer, METH_VARARGS},
	{"SentToLayer", scribus_senttolayer, METH_VARARGS},
	{"SetLayerVisible", scribus_layervisible, METH_VARARGS},
	{"SetLayerPrintable", scribus_layerprint, METH_VARARGS},
	{"IsLayerVisible", scribus_glayervisib, METH_VARARGS},
	{"IsLayerPrintable", scribus_glayerprint, METH_VARARGS},
	{"CreateLayer", scribus_createlayer, METH_VARARGS},
	{"DeleteLayer", scribus_removelayer, METH_VARARGS},
	{"GetGuiLanguage", scribus_getlanguage, METH_VARARGS},
 	{"GetHGuides", scribus_getHguides, METH_VARARGS},
 	{"SetHGuides", scribus_setHguides, METH_VARARGS},
 	{"GetVGuides", scribus_getVguides, METH_VARARGS},
	{"SetVGuides", scribus_setVguides, METH_VARARGS},
	{"SetDocType", scribus_setdoctype, METH_VARARGS},
	// aliases
	{"setLineWidth", scribus_setlinewidth, METH_VARARGS},
	{"setLineShade", scribus_setlineshade, METH_VARARGS},
	{"setLineJoin", scribus_setlinejoin, METH_VARARGS},
	{"setLineCap", scribus_setlineend, METH_VARARGS},
	{"setLineStyle", scribus_setlinestyle, METH_VARARGS},
	{"setFillShade", scribus_setfillshade, METH_VARARGS},
	{"setCornerRadius", scribus_setcornerrad, METH_VARARGS},
	{"loadImage", scribus_loadimage, METH_VARARGS},
	{"scaleImage", scribus_scaleimage, METH_VARARGS},
	{"setText", scribus_setboxtext, METH_VARARGS},
	{"insertText", scribus_inserttext, METH_VARARGS},
	{"selectText", scribus_selecttext, METH_VARARGS},
	{"deleteText", scribus_deletetext, METH_VARARGS},
	{"setFont", scribus_setfont, METH_VARARGS},
	{"setFontSize", scribus_setfontsize, METH_VARARGS},
	{"setTextColor", scribus_settextfill, METH_VARARGS},
	{"setTextStroke", scribus_settextstroke, METH_VARARGS},
	{"setTextShade", scribus_settextshade, METH_VARARGS},
	{"setColumns", scribus_setcolumns, METH_VARARGS},
	{"setColumnGap", scribus_setcolumngap, METH_VARARGS},
	{"setLineSpacing", scribus_setlinespace, METH_VARARGS},
	{"setTextAlignment", scribus_setalign, METH_VARARGS},
	{"moveObject", scribus_moveobjrel, METH_VARARGS},
	{"moveObjectAbs", scribus_moveobjabs, METH_VARARGS},
	{"rotateObject", scribus_rotobjrel, METH_VARARGS},
	{"rotateObjectAbs", scribus_rotobjabs, METH_VARARGS},
	{"sizeObject", scribus_sizeobjabs, METH_VARARGS},
	{"groupObjects", scribus_groupobj, METH_VARARGS},
	{"unGroupObject", scribus_ungroupobj, METH_VARARGS},
	{"scaleGroup", scribus_scalegroup, METH_VARARGS},
	{"setSelectedObject", scribus_getselobjnam, METH_VARARGS},
	{"selectionCount", scribus_selcount, METH_VARARGS},
	{"selectObject", scribus_selectobj, METH_VARARGS},
	{"deselectAll", scribus_deselect, METH_VARARGS},
	{"getColorNames", scribus_colornames, METH_VARARGS},
	{"getColor", scribus_getcolor, METH_VARARGS},
	{"changeColor", scribus_setcolor, METH_VARARGS},
	{"defineColor", scribus_newcolor, METH_VARARGS},
	{"deleteColor", scribus_delcolor, METH_VARARGS},
	{"replaceColor", scribus_replcolor, METH_VARARGS},
	{"getFontNames", scribus_fontnames, METH_VARARGS},
	{"renderFont", scribus_renderfont, METH_VARARGS},
	{"getLayers", scribus_getlayers, METH_VARARGS},
	{"setActiveLayer", scribus_setactlayer, METH_VARARGS},
	{"getActiveLayer", scribus_getactlayer, METH_VARARGS},
	{"sentToLayer", scribus_senttolayer, METH_VARARGS},
	{"setLayerVisible", scribus_layervisible, METH_VARARGS},
	{"setLayerPrintable", scribus_layerprint, METH_VARARGS},
	{"isLayerVisible", scribus_glayervisib, METH_VARARGS},
	{"isLayerPrintable", scribus_glayerprint, METH_VARARGS},
	{"createLayer", scribus_createlayer, METH_VARARGS},
	{"deleteLayer", scribus_removelayer, METH_VARARGS},
	{"getGuiLanguage", scribus_getlanguage, METH_VARARGS},
	{"getHGuides", scribus_getHguides, METH_VARARGS},
	{"setHGuides", scribus_setHguides, METH_VARARGS},
	{"getVGuides", scribus_getVguides, METH_VARARGS},
	{"setVGuides", scribus_setVguides, METH_VARARGS},
	{"setDocType", scribus_setdoctype, METH_VARARGS},
	// end of aliases
	{"retval", scribus_retval, METH_VARARGS},
	{"getval", scribus_getval, METH_VARARGS},
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

