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
#include "guiapp.h"
#include "customfdialog.h"
#include "helpbrowser.h"

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
  return QObject::tr("Scripter Manual...");
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
	Tes->SavedRecentScripts.clear();
	Tes->ReadPlugPrefs();
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
	men->insertItem(QObject::tr("Execute Script..."), Tes, SLOT(slotTest()));
	Tes->rmenid = men->insertItem(QObject::tr("Recent Scripts"), Tes->rmen);
	Tes->cons = men->insertItem(QObject::tr("Show Console"), Tes, SLOT(slotInteractiveScript()));
	plug->menuBar()->insertItem(QObject::tr("Script"), men, -1, plug->menuBar()->count() - 2);
	QObject::connect(Tes->pcon->OutWin, SIGNAL(returnPressed()), Tes, SLOT(slotExecute()));
	QObject::connect(Tes->pcon, SIGNAL(Schliessen()), Tes, SLOT(slotInteractiveScript()));
	QObject::connect(Tes->rmen, SIGNAL(activated(int)), Tes, SLOT(RecentScript(int)));
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
	pfad2 = pfad + "/share/scribus/doc/en/Scripter/index.html";
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("Online Reference"), pfad2);
	dia->show();
}

void MenuTest::slotTest()
{
	QString fileName;
	QString CurDirP = QDir::currentDirPath();
	if (Carrier->Prefs.ScriptDir != "")
		QDir::setCurrent(Carrier->Prefs.ScriptDir);
	CustomFDialog diaf((QWidget*)parent(), QObject::tr("Open"), QObject::tr("Python Scripts (*.py);; All Files (*)"));
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
	PySys_SetArgv(1, comm);
	PyRun_SimpleString(cmd.data());
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
		men->changeItem(cons, tr("Show Console"));
		pcon->hide();
		}
	else
		{
		men->changeItem(cons, tr("Hide Console"));
		pcon->show();
		}
}

void MenuTest::slotExecute()
{
	pcon->OutWin->append(slotRunScript(pcon->OutWin->LastComm));
	pcon->OutWin->moveCursor(QTextEdit::MoveEnd, false);
	pcon->OutWin->scrollToBottom();
	pcon->OutWin->ensureCursorVisible();
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
		if (Carrier->doc->ActPage->SelItem.count() != 0)
			Carrier->HaveNewSel(Carrier->doc->ActPage->SelItem.at(0)->PType);
		else
			Carrier->HaveNewSel(-1);
		Carrier->Sepal->Rebuild();
		Carrier->view->DrawNew();
		}
}

void MenuTest::ReadPlugPrefs()
{
	QDomDocument docu("scriptrc");
	QString ho = getenv("HOME");
	QFile f(ho+"/.scribus/scripter.rc");
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
	QString ho = getenv("HOME");
	QFile f(ho+"/.scribus/scripter.rc");
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
    {"ProgressReset",    scribus_progressreset,    METH_VARARGS},
    {"ProgressTotal",    scribus_progresssettotalsteps,    METH_VARARGS},
    {"ProgressSet",    scribus_progresssetprogress,    METH_VARARGS},
    {"MessagebarText",    scribus_messagebartext,    METH_VARARGS},
    {"DocChanged",    scribus_docchanged,    METH_VARARGS},
    {"SetCursor",    scribus_setcursor,    METH_VARARGS},
	{"PageDimension",	scribus_pagedimension,	METH_VARARGS},
	{"NewDocDialog",			scribus_newdocdia,			METH_VARARGS},
	{"FileDialog",				scribus_filedia,				METH_VARARGS},
	{"StatusMessage",			scribus_mess,						METH_VARARGS},
	{"MessageBox",				scribus_messdia,				METH_VARARGS},
	{"NewDoc",						scribus_newdoc,					METH_VARARGS},
	{"CloseDoc",					scribus_closedoc,				METH_VARARGS},
	{"HaveDoc",						scribus_havedoc,				METH_VARARGS},
	{"OpenDoc",						scribus_opendoc,				METH_VARARGS},
	{"SaveDoc",						scribus_savedoc,				METH_VARARGS},
	{"SaveDocAs",					scribus_savedocas,			METH_VARARGS},
	{"SetInfo",						scribus_setinfo,				METH_VARARGS},
	{"SetMargins",				scribus_setmargins,			METH_VARARGS},
	{"SetUnit",						scribus_setunit,				METH_VARARGS},
	{"GetUnit",						scribus_getunit,				METH_VARARGS},
	{"CurrentPage",				scribus_actualpage,			METH_VARARGS},
	{"SetRedraw",					scribus_setredraw,			METH_VARARGS},
	{"RedrawAll",					scribus_redraw,					METH_VARARGS},
	{"SavePageAsEPS",			scribus_savepageeps,		METH_VARARGS},
	{"NewPage",						scribus_newpage,				METH_VARARGS},
	{"DeletePage",				scribus_deletepage,			METH_VARARGS},
	{"GotoPage",					scribus_gotopage,				METH_VARARGS},
	{"PageCount",					scribus_pagecount,			METH_VARARGS},
	{"CreateRect",				scribus_newrect,				METH_VARARGS},
	{"CreateEllipse",			scribus_newellipse,			METH_VARARGS},
	{"CreateImage",				scribus_newimage,				METH_VARARGS},
	{"CreateLine",				scribus_newline,				METH_VARARGS},
	{"CreatePolyLine",		scribus_polyline,				METH_VARARGS},
	{"CreatePolygon",			scribus_polygon,				METH_VARARGS},
	{"CreateBezierLine",	scribus_bezierline,			METH_VARARGS},
	{"CreatePathText",		scribus_pathtext,				METH_VARARGS},
	{"CreateText",				scribus_newtext,				METH_VARARGS},
	{"DeleteObject",			scribus_deleteobj,			METH_VARARGS},
	{"GetFillColor",			scribus_getfillcolor,		METH_VARARGS},
	{"GetLineColor",			scribus_getlinecolor,		METH_VARARGS},
	{"GetLineWidth",			scribus_getlinewidth,		METH_VARARGS},
	{"GetLineShade",			scribus_getlineshade,		METH_VARARGS},
	{"GetLineJoin",				scribus_getlinejoin,		METH_VARARGS},
	{"GetLineCap",				scribus_getlineend,			METH_VARARGS},
	{"GetLineStyle",			scribus_getlinestyle,		METH_VARARGS},
	{"GetFillShade",			scribus_getfillshade,		METH_VARARGS},
	{"GetCornerRadius",		scribus_getcornerrad,		METH_VARARGS},
	{"GetImageScale",			scribus_getimgscale,		METH_VARARGS},
	{"GetImageFile",			scribus_getimgname,			METH_VARARGS},
	{"GetPosition",				scribus_getposi,				METH_VARARGS},
	{"GetSize",						scribus_getsize,				METH_VARARGS},
	{"GetRotation",				scribus_getrotation,		METH_VARARGS},
	{"GetFontSize",				scribus_getfontsize,		METH_VARARGS},
	{"GetFont",						scribus_getfont,				METH_VARARGS},
	{"GetTextLength",			scribus_gettextsize,		METH_VARARGS},
	{"GetTextColor",			scribus_getlinecolor,		METH_VARARGS},
	{"GetTextShade",			scribus_getlineshade,		METH_VARARGS},
	{"GetColumns",				scribus_getcolumns,			METH_VARARGS},
	{"GetColumnGap",			scribus_getcolumngap,		METH_VARARGS},
	{"GetLineSpacing",		scribus_getlinespace,		METH_VARARGS},
	{"GetText",						scribus_getframetext,		METH_VARARGS},
	{"GetAllText",				scribus_gettext,				METH_VARARGS},
	{"GetAllObjects",			scribus_getallobj,			METH_VARARGS},
	{"SetGradientFill",		scribus_setgradfill,		METH_VARARGS},
	{"SetFillColor",			scribus_setfillcolor,		METH_VARARGS},
	{"SetLineColor",			scribus_setlinecolor,		METH_VARARGS},
	{"SetMultiLine",			scribus_setmultiline,		METH_VARARGS},
	{"SetLineWidth",			scribus_setlinewidth,		METH_VARARGS},
	{"SetLineShade",			scribus_setlineshade,		METH_VARARGS},
	{"SetLineJoin",				scribus_setlinejoin,		METH_VARARGS},
	{"SetLineCap",				scribus_setlineend,			METH_VARARGS},
	{"SetLineStyle",			scribus_setlinestyle,		METH_VARARGS},
	{"SetFillShade",			scribus_setfillshade,		METH_VARARGS},
	{"SetCornerRadius",		scribus_setcornerrad,		METH_VARARGS},
	{"LoadImage",					scribus_loadimage,			METH_VARARGS},
	{"ScaleImage",				scribus_scaleimage,			METH_VARARGS},
	{"SetText",						scribus_setboxtext,			METH_VARARGS},
	{"InsertText",				scribus_inserttext,			METH_VARARGS},
	{"SelectText",				scribus_selecttext,			METH_VARARGS},
	{"DeleteText",				scribus_deletetext,			METH_VARARGS},
	{"SetFont",						scribus_setfont,				METH_VARARGS},
	{"SetFontSize",				scribus_setfontsize,		METH_VARARGS},
	{"SetTextColor",			scribus_settextfill,		METH_VARARGS},
	{"SetTextStroke",			scribus_settextstroke,	METH_VARARGS},
	{"SetTextShade",			scribus_settextshade,		METH_VARARGS},
	{"SetColumns",				scribus_setcolumns,			METH_VARARGS},
	{"SetColumnGap",			scribus_setcolumngap,		METH_VARARGS},
	{"SetLineSpacing",		scribus_setlinespace,		METH_VARARGS},
	{"SetTextAlignment",	scribus_setalign,				METH_VARARGS},
	{"MoveObject",				scribus_moveobjrel,			METH_VARARGS},
	{"MoveObjectAbs",			scribus_moveobjabs,			METH_VARARGS},
	{"RotateObject",			scribus_rotobjrel,			METH_VARARGS},
	{"RotateObjectAbs",		scribus_rotobjabs,			METH_VARARGS},
	{"SizeObject",				scribus_sizeobjabs,			METH_VARARGS},
	{"GroupObjects",			scribus_groupobj,				METH_VARARGS},
	{"UnGroupObject",			scribus_ungroupobj,			METH_VARARGS},
	{"ScaleGroup",				scribus_scalegroup,			METH_VARARGS},
	{"GetSelectedObject",	scribus_getselobjnam,		METH_VARARGS},
	{"SelectionCount",		scribus_selcount,				METH_VARARGS},
	{"SelectObject",			scribus_selectobj,			METH_VARARGS},
	{"DeselectAll",				scribus_deselect,				METH_VARARGS},
	{"GetColorNames",			scribus_colornames,			METH_VARARGS},
	{"GetColor",					scribus_getcolor,				METH_VARARGS},
	{"ChangeColor",				scribus_setcolor,				METH_VARARGS},
	{"DefineColor",				scribus_newcolor,				METH_VARARGS},
	{"DeleteColor",				scribus_delcolor,				METH_VARARGS},
	{"ReplaceColor",			scribus_replcolor,			METH_VARARGS},
	{"GetFontNames",			scribus_fontnames,			METH_VARARGS},
	{"RenderFont",			scribus_renderfont,			METH_VARARGS},
	{"GetLayers",					scribus_getlayers,			METH_VARARGS},
	{"SetActiveLayer",		scribus_setactlayer,		METH_VARARGS},
	{"GetActiveLayer",		scribus_getactlayer,		METH_VARARGS},
	{"SentToLayer",				scribus_senttolayer,		METH_VARARGS},
	{"SetLayerVisible",		scribus_layervisible,		METH_VARARGS},
	{"SetLayerPrintable",	scribus_layerprint,			METH_VARARGS},
	{"IsLayerVisible",		scribus_glayervisib,		METH_VARARGS},
	{"IsLayerPrintable",	scribus_glayerprint,		METH_VARARGS},
	{"CreateLayer",				scribus_createlayer,		METH_VARARGS},
	{"DeleteLayer",				scribus_removelayer,		METH_VARARGS},
	{"retval",						scribus_retval,					METH_VARARGS},
	{"getval",						scribus_getval,					METH_VARARGS},
	{NULL,		NULL}		/* sentinel */
};

void initscribus(ScribusApp *pl)
{
	PyObject *m, *d;
	PyImport_AddModule("scribus");
	m = Py_InitModule("scribus", scribus_methods);
	d = PyModule_GetDict(m);
	PyDict_SetItemString(d, "Points", 								Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "Millimeters", 						Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "Inches", 								Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "Picas", 									Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "Portrait", 							Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "Landscape", 							Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "NoFacingPages", 					Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "FacingPages", 						Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "FirstPageRight", 				Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "FirstPageLeft", 					Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "LeftAlign",		 					Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "RightAlign", 						Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "Centered",				 				Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "Forced",				 					Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "NoGradient",							Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "HorizontalGradient",			Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "VerticalGradient",				Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "DiagonalGradient",				Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "CrossDiagonalGradient",	Py_BuildValue("i", 4));
	PyDict_SetItemString(d, "RadialGradient",					Py_BuildValue("i", 5));
	PyDict_SetItemString(d, "SolidLine",		 					Py_BuildValue("i", Qt::SolidLine));
	PyDict_SetItemString(d, "DashLine",			 					Py_BuildValue("i", Qt::DashLine));
	PyDict_SetItemString(d, "DotLine",			 					Py_BuildValue("i", Qt::DotLine));
	PyDict_SetItemString(d, "DashDotLine",	 					Py_BuildValue("i", Qt::DashDotLine));
	PyDict_SetItemString(d, "DashDotDotLine",					Py_BuildValue("i", Qt::DashDotDotLine));
	PyDict_SetItemString(d, "MiterJoin",		 					Py_BuildValue("i", Qt::MiterJoin));
	PyDict_SetItemString(d, "BevelJoin",		 					Py_BuildValue("i", Qt::BevelJoin));
	PyDict_SetItemString(d, "RoundJoin",		 					Py_BuildValue("i", Qt::RoundJoin));
	PyDict_SetItemString(d, "FlatCap",			 					Py_BuildValue("i", Qt::FlatCap));
	PyDict_SetItemString(d, "SquareCap",		 					Py_BuildValue("i", Qt::SquareCap));
	PyDict_SetItemString(d, "RoundCap",			 					Py_BuildValue("i", Qt::RoundCap));
	PyDict_SetItemString(d, "NoButton",			 					Py_BuildValue("i", QMessageBox::NoButton));
	PyDict_SetItemString(d, "Ok",			 								Py_BuildValue("i", QMessageBox::Ok));
	PyDict_SetItemString(d, "Cancel",			 						Py_BuildValue("i", QMessageBox::Cancel));
	PyDict_SetItemString(d, "Yes",			 							Py_BuildValue("i", QMessageBox::Yes));
	PyDict_SetItemString(d, "No",			 								Py_BuildValue("i", QMessageBox::No));
	PyDict_SetItemString(d, "Abort",			 						Py_BuildValue("i", QMessageBox::Abort));
	PyDict_SetItemString(d, "Retry",			 						Py_BuildValue("i", QMessageBox::Retry));
	PyDict_SetItemString(d, "Ignore",			 						Py_BuildValue("i", QMessageBox::Ignore));
	PyDict_SetItemString(d, "NoIcon",			 						Py_BuildValue("i", QMessageBox::NoIcon));
	PyDict_SetItemString(d, "Information",	 					Py_BuildValue("i", QMessageBox::Information));
	PyDict_SetItemString(d, "Warning",			 					Py_BuildValue("i", QMessageBox::Warning));
	PyDict_SetItemString(d, "Critical",			 					Py_BuildValue("i", QMessageBox::Critical));
	PyDict_SetItemString(d, "Paper_A0", 							Py_BuildValue("(ff)", 2380.0, 3368.0));
	PyDict_SetItemString(d, "Paper_A1", 							Py_BuildValue("(ff)", 1684.0, 2380.0));
	PyDict_SetItemString(d, "Paper_A2", 							Py_BuildValue("(ff)", 1190.0, 1684.0));
	PyDict_SetItemString(d, "Paper_A3", 							Py_BuildValue("(ff)", 842.0, 1190.0));
	PyDict_SetItemString(d, "Paper_A4", 							Py_BuildValue("(ff)", 595.0, 842.0));
	PyDict_SetItemString(d, "Paper_A5", 							Py_BuildValue("(ff)", 421.0, 595.0));
	PyDict_SetItemString(d, "Paper_A6", 							Py_BuildValue("(ff)", 297.0, 421.0));
	PyDict_SetItemString(d, "Paper_A7", 							Py_BuildValue("(ff)", 210.0, 297.0));
	PyDict_SetItemString(d, "Paper_A8", 							Py_BuildValue("(ff)", 148.0, 210.0));
	PyDict_SetItemString(d, "Paper_A9", 							Py_BuildValue("(ff)", 105.0, 148.0));
	PyDict_SetItemString(d, "Paper_B0", 							Py_BuildValue("(ff)", 2836.0, 4008.0));
	PyDict_SetItemString(d, "Paper_B1", 							Py_BuildValue("(ff)", 2004.0, 2836.0));
	PyDict_SetItemString(d, "Paper_B2", 							Py_BuildValue("(ff)", 1418.0, 2004.0));
	PyDict_SetItemString(d, "Paper_B3", 							Py_BuildValue("(ff)", 1002.0, 1418.0));
	PyDict_SetItemString(d, "Paper_B4", 							Py_BuildValue("(ff)", 709.0, 1002.0));
	PyDict_SetItemString(d, "Paper_B5", 							Py_BuildValue("(ff)", 501.0, 709.0));
	PyDict_SetItemString(d, "Paper_B6", 							Py_BuildValue("(ff)", 355.0, 501.0));
	PyDict_SetItemString(d, "Paper_B7", 							Py_BuildValue("(ff)", 250.0, 355.0));
	PyDict_SetItemString(d, "Paper_B8", 							Py_BuildValue("(ff)", 178.0, 250.0));
	PyDict_SetItemString(d, "Paper_B9", 							Py_BuildValue("(ff)", 125.0, 178.0));
	PyDict_SetItemString(d, "Paper_B10", 							Py_BuildValue("(ff)", 89.0, 125.0));
	PyDict_SetItemString(d, "Paper_C5E", 							Py_BuildValue("(ff)", 462.0, 649.0));
	PyDict_SetItemString(d, "Paper_Comm10E", 					Py_BuildValue("(ff)", 298.0, 683.0));
	PyDict_SetItemString(d, "Paper_DLE", 							Py_BuildValue("(ff)", 312.0, 624.0));
	PyDict_SetItemString(d, "Paper_Executive",				Py_BuildValue("(ff)", 542.0, 720.0));
	PyDict_SetItemString(d, "Paper_Folio", 						Py_BuildValue("(ff)", 595.0, 935.0));
	PyDict_SetItemString(d, "Paper_Ledger", 					Py_BuildValue("(ff)", 1224.0, 792.0));
	PyDict_SetItemString(d, "Paper_Legal", 						Py_BuildValue("(ff)", 612.0, 1008.0));
	PyDict_SetItemString(d, "Paper_Letter", 					Py_BuildValue("(ff)", 612.0, 792.0));
	PyDict_SetItemString(d, "Paper_Tabloid", 					Py_BuildValue("(ff)", 792.0, 1224.0));
	Carrier = pl;
}

