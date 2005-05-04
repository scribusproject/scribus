// Scripter headers
#include "cmdvar.h"
#include "scriptercore.h"
#include "pconsole.h"
#include "conswin.h"
#include "macromanager.h"
#include "scripterprefs.h"

// Scribus headers
#include "helpbrowser.h"
#include "customfdialog.h"
#include "mpalette.h"

// Qt headers
#include "qwidget.h"
#include "qpopupmenu.h"
#include "qdir.h"
#include "qfileinfo.h"
#include "qmessagebox.h"
#include "qtextcodec.h"
#include "qdom.h"
#include "qtextstream.h"

#include <cstdlib>

#include "scribus.h"

ScripterCore::ScripterCore(QWidget* parent)
{
	this->setName("scripterCore");
	men = new QPopupMenu();
	this->rmen = new QPopupMenu();
	this->smen = new QPopupMenu();
	if (!men | !this->rmen | !this->smen)
	{
		qDebug("ScripterCore::ScripterCore(): Couldn't build menus");
		return;
	}
	this->SavedRecentScripts.clear();
	// init prefs variables
	this->enableExtPython = false;
	this->importAllNames = true;
	this->legacyAliases = true;
	this->startupScriptEnable = false;
	this->useDummyStdin = true;
	this->startupScript = QString();
	// now load the prefs
	this->ReadPlugPrefs();
	QString pfad = SCRIPTSDIR;
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad);
	QDir ds(pfad2, "*.py", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((ds.exists()) && (ds.count() != 0))
	{
		for (uint dc = 0; dc < ds.count(); ++dc)
		{
			QFileInfo fs(ds[dc]);
			this->smen->insertItem(fs.baseName(true));
		}
	}
	this->RecentScripts.clear();
	if (this->SavedRecentScripts.count() != 0)
	{
		uint max = QMIN(Carrier->Prefs.RecentDCount, this->SavedRecentScripts.count());
		for (uint m = 0; m < max; ++m)
		{
			QFileInfo fd(this->SavedRecentScripts[m]);
			if (fd.exists())
			{
				this->RecentScripts.append(this->SavedRecentScripts[m]);
				this->rmen->insertItem(this->SavedRecentScripts[m]);
			}
		}
	}
	int id = -1;
	this->pcon = new PConsole(parent);
	if (!this->pcon)
	{
		qDebug("ScripterCore::ScripterCore(): Unable to construct script console");
		return;
	}
	this->smenid = men->insertItem(tr("&Scribus Scripts"), this->smen);
	id = men->insertItem(tr("&Execute Script..."), this, SLOT(slotTest()));
	men->setWhatsThis(id, tr("Run a Python script from a file.","scripter"));
	id = men->insertItem(tr("&Load Extension Script..."), this, SLOT(loadScript()));
	men->setWhatsThis(id, tr("Load a Python script as an extension. "
	                  "Used for loading macros and for advanced Python scripts that "
	                  "extend the Scribus user interface.","scripter"));
	this->rmenid = men->insertItem(tr("&Recent Scripts"), this->rmen);
	men->insertSeparator();
	id = this->cons = men->insertItem(tr("Show &Console"), this, SLOT(slotInteractiveScript()));
	men->setWhatsThis(id, tr("Display an interactive Python console where you can write and "
	                                  "run Python programs that use the Scripter tools.","scripter"));
	this->about = men->insertItem(tr("&About Script..."), this, SLOT(aboutScript()));
	Carrier->menuBar()->insertItem(tr("S&cript"), men, -1, Carrier->menuBar()->count() - 2);
	connect(this->pcon->OutWin, SIGNAL(returnPressed()), this, SLOT(slotExecute()));
	connect(this->pcon, SIGNAL(Schliessen()), this, SLOT(slotInteractiveScript()));
	connect(this->rmen, SIGNAL(activated(int)), this, SLOT(RecentScript(int)));
	connect(this->smen, SIGNAL(activated(int)), this, SLOT(StdScript(int)));
	men->insertSeparator();
	id = men->insertItem(tr("Scripter &Settings","script menu"), this, SLOT(preferencesDialog()));
}

void ScripterCore::runStartupScript()
{
	// and run the start-up script, if any
	if (this->enableExtPython && this->startupScriptEnable)
	{
		if (QFile::exists(this->startupScript))
		{
			// run the script in the main interpreter. The user will be informed
			// with a dialog if something has gone wrong.
			this->slotRunScriptFile(this->startupScript, true);
		}
		else
			qDebug("Startup script enabled, but couln't find script %s.", (const char*)(this->startupScript.utf8()) );
	}
}

ScripterCore::~ScripterCore()
{
	SavePlugPrefs();
}

void ScripterCore::FinishScriptRun()
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
		{
			Carrier->doc->ActPage->EmitValues(Carrier->doc->ActPage->SelItem.at(0));
			Carrier->HaveNewSel(Carrier->doc->ActPage->SelItem.at(0)->PType);
		}
		else
			Carrier->HaveNewSel(-1);
		Carrier->view->DrawNew();
	}
}

void ScripterCore::slotTest()
{
	QString fileName;
	QString CurDirP = QDir::currentDirPath();
	QString scriptDir = Carrier->Prefs.ScriptDir;
	if (scriptDir == "")
		scriptDir = CurDirP;
	CustomFDialog diaf((QWidget*)parent(), scriptDir, tr("Open"), tr("Python Scripts (*.py);; All Files (*)"));
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

void ScripterCore::StdScript(int id)
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

void ScripterCore::RecentScript(int id)
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

void ScripterCore::slotRunScriptFile(QString fileName, bool inMainInterpreter)
{
	char* comm[1];
	PyThreadState *stateo, *state;
	QFileInfo fi(fileName);
	QCString na = fi.fileName().latin1();
	// Set up a sub-interpreter if needed:
	if (!inMainInterpreter)
	{
		Carrier->ScriptRunning = true;
		qApp->setOverrideCursor(QCursor(waitCursor), false);
		// Actually make the sub interpreter
		// FIXME: This calls abort() in a Python debug build. We're doing something wrong.
		stateo = PyEval_SaveThread();
		state = Py_NewInterpreter();
		// chdir to the dir the script is in
		QDir::setCurrent(fi.dirPath(true));
		// init the 'scribus' module in the sub-interpreter
		initscribus(Carrier);
	}
	// make sure sys.argv[0] is the path to the script
	comm[0] = na.data();
	PySys_SetArgv(1, comm);
	// call python script
	PyObject* m = PyImport_AddModule((char*)"__main__");
	if (m == NULL)
		qDebug("Failed to get __main__ - aborting script");
	else
	{
		// FIXME: If filename contains chars outside 7bit ascii, might be problems
		PyObject* globals = PyModule_GetDict(m);
		// Build the Python code to run the script
		QString cm = QString("from __future__ import division\n");
		cm        += QString("import sys\n");
		cm        += QString("import cStringIO\n");
		/* Implementation of the help() in pydoc.py reads some OS variables
		 * for output settings. I use ugly hack to stop freezing calling help()
		 * in script. pv. */
		cm += QString("import os\nos.environ['PAGER'] = '/bin/false'\n"); // HACK
		cm        += QString("sys.path[0] = \"%1\"\n").arg(fi.dirPath(true));
		// Replace sys.stdin with a dummy StringIO that always returns
		// "" for read
		if (useDummyStdin)
			cm    += QString("sys.stdin = cStringIO.StringIO()\n");
		cm        += QString("try:\n");
		cm        += QString("    execfile(\"%1\")\n").arg(fileName);
		cm        += QString("except SystemExit:\n");
		cm        += QString("    pass\n");
		// Capture the text of any other exception that's raised by the interpreter
		// into a StringIO buffer for later extraction.
		cm        += QString("except:\n");
		cm        += QString("    import traceback\n");
		cm        += QString("    import scribus\n");                  // we stash our working vars here
		cm        += QString("    scribus._f=cStringIO.StringIO()\n");
		cm        += QString("    traceback.print_exc(file=scribus._f)\n");
		cm        += QString("    _errorMsg = scribus._f.getvalue()\n");
		cm        += QString("    del(scribus._f)\n");
		// We re-raise the exception so the return value of PyRun_StringFlags reflects
		// the fact that an exception has ocurred.
		cm        += QString("    raise\n");
		// FIXME: if cmd contains chars outside 7bit ascii, might be problems
		QCString cmd = cm.latin1();
		// Now run the script in the interpreter's global scope. It'll run in a
		// sub-interpreter if we created and switched to one earlier, otherwise
		// it'll run in the main interpreter.
		PyObject* result = PyRun_String(cmd.data(), Py_file_input, globals, globals);
		// NULL is returned if an exception is set. We don't care about any
		// other return value (most likely None anyway) and can ignore it.
		if (result == NULL)
		{
			// We've already saved the exception text, so clear the exception
			PyErr_Clear();
			PyObject* errorMsgPyStr = PyMapping_GetItemString(globals, (char*)"_errorMsg");
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
	if (!inMainInterpreter)
	{
		Py_EndInterpreter(state);
		PyEval_RestoreThread(stateo);
		Carrier->ScriptRunning = false;
		qApp->restoreOverrideCursor();
	}
}

QString ScripterCore::slotRunScript(QString Script)
{
	qApp->setOverrideCursor(QCursor(waitCursor), false);
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
			cm =  "import sys\n";
			cm += "sys.path[0] = \""+CurDir+"\"\n";
			cm += "import cStringIO\n";
			cm += "import scribus\n";
			// Only import all names from 'scribus' to the global namespace if the user wants us to.
			// We still need to pull in a few special names used by Scribus though.
			if (importAllNames)
				cm += "from scribus import *\n";
			if (useDummyStdin)
				cm += "sys.stdin = cStringIO.StringIO()\n";
			cm += "scribus._bu = cStringIO.StringIO()\n";
			cm += "sys.stdout = scribus._bu\n";
			cm += "sys.stderr = scribus._bu\n";
			cm += "import code\n";
			cm += "scribus._ia = code.InteractiveConsole(globals())\n";
		}
		cm += "scribus._sc = scribus._getval()\n";
		cm += "scribus._rv = scribus._ia.push(scribus._sc)\n";
		cm += "if scribus._rv == 1:\n";
		cm += "    scribus._re = \"...\"\n";
		cm += "else:\n";
		cm += "    scribus._re = scribus._bu.getvalue()\n";
		cm += "scribus._retval(scribus._re, scribus._rv)\n";
	}
	// FIXME: if cmd contains chars outside 7bit ascii, might be problems
	QCString cmd = cm.latin1();
	comm[0] = (char*)"scribus";
	PySys_SetArgv(1, comm);
	PyRun_SimpleString(cmd.data());
	if (RetVal == 0)
	{
		RetString += ">>>";
		pcon->OutWin->Prompt = ">>>";
	}
	else
		pcon->OutWin->Prompt = "...";
	qApp->restoreOverrideCursor();
	return RetString;
}

void ScripterCore::loadScript()
{
	if (!this->enableExtPython)
	{
		QMessageBox::information(Carrier, tr("Scribus - Script Plugin"),
			tr("The 'Load Script' function of the script plugin is currently disabled.\n"
			   "If you just want to run a normal script, you probably want to use\n"
			   "'Execute Script...' instead.\n\n"
			   "If you do actually want to load a Python extension script or macro, you\n"
			   "need to go into the Scripter Settings in the Script menu and enable\n"
			   "scripter extensions there.\n\n"
			   "Please read the documentation on extension scripts first.\n"));
		return;
	}
	QString fileName;
	QString scriptDir = Carrier->Prefs.ScriptDir;
	if (scriptDir == "")
		scriptDir = QDir::currentDirPath();
	CustomFDialog diaf((QWidget*)parent(), scriptDir, tr("Open"), tr("Python Scripts (*.py);; All Files (*)"));
	if (diaf.exec())
	{
		fileName = diaf.selectedFile();
		// Run the script in the main interpreter, not a sub-interpreter
		slotRunScriptFile(fileName, true);
	}
}

void ScripterCore::slotInteractiveScript()
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

void ScripterCore::slotExecute()
{
	pcon->OutWin->append(slotRunScript(pcon->OutWin->LastComm));
	pcon->OutWin->moveCursor(QTextEdit::MoveEnd, false);
	pcon->OutWin->scrollToBottom();
	pcon->OutWin->ensureCursorVisible();
	FinishScriptRun();
}

void ScripterCore::ReadPlugPrefs()
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
		// Check to see if the 'load script' menu item should be enabled
		else if (dc.tagName() == "EXTPYTHON")
			enableExtPython = dc.attribute("ENABLE") == "TRUE";
		// and check whether we should do a 'from scribus import *' in the main interpreter;
		else if (dc.tagName() == "IMPORTNAMES")
			importAllNames = dc.attribute("ENABLE") == "TRUE";
		// Should we import the old-style names?
		else if (dc.tagName() == "LEGACYALIASES")
			legacyAliases = dc.attribute("ENABLE") == "TRUE";
		// Should we run a startup script? If so, where to load it from?
		else if (dc.tagName() == "STARTUPSCRIPT")
		{
			startupScriptEnable = dc.attribute("ENABLE") == "TRUE";
			startupScript = dc.attribute("FILE");
		}
		// replace stdin with a dummy object that always returns "" ?
		else if (dc.tagName() == "USEDUMMYSTDIN")
			useDummyStdin = dc.attribute("ENABLE") == "TRUE";
		DOC=DOC.nextSibling();
	}
}

void ScripterCore::SavePlugPrefs()
{
	QDomDocument docu("scriptrc");
	QString st="<SCRIPTRC></SCRIPTRC>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	for (uint rd=0; rd < this->RecentScripts.count(); ++rd)
	{
		QDomElement rde=docu.createElement("RECENT");
		rde.setAttribute("NAME",this->RecentScripts[rd]);
		elem.appendChild(rde);
	}

	// save the "load script" flag, from import flag, and aliases flag
	QDomElement extPythonItem = docu.createElement("EXTPYTHON");
	extPythonItem.setAttribute("ENABLE", enableExtPython ? "TRUE" : "FALSE");
	elem.appendChild(extPythonItem);

	QDomElement allNamesItem = docu.createElement("IMPORTNAMES");
	allNamesItem.setAttribute("ENABLE", importAllNames ? "TRUE" : "FALSE");
	elem.appendChild(allNamesItem);

	QDomElement legacyAliasesItem = docu.createElement("LEGACYALIASES");
	legacyAliasesItem.setAttribute("ENABLE", legacyAliases ? "TRUE" : "FALSE");
	elem.appendChild(legacyAliasesItem);

	QDomElement dummyStdinItem = docu.createElement("USEDUMMYSTDIN");
	dummyStdinItem.setAttribute("ENABLE", useDummyStdin ? "TRUE" : "FALSE");
	elem.appendChild(dummyStdinItem);

	// save the startup script path and flag
	QDomElement startupScriptItem = docu.createElement("STARTUPSCRIPT");
	startupScriptItem.setAttribute("ENABLE", startupScriptEnable ? "TRUE" : "FALSE");
	startupScriptItem.setAttribute("FILE", startupScript);
	elem.appendChild(startupScriptItem);

	// then write out the prefs file
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
void ScripterCore::aboutScript()
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
	HelpBrowser *dia = new HelpBrowser(0, tr("About Script") + " " + fi.fileName(), "en", "", html);
	dia->show();
}

/* 2005-01-02 CR
 * Display a preferences dialog to let the user configure
 * scripter settings such as enabling Python extension scripts
 * and setting a start-up script. The prefs dialog is defined in
 * scripterprefs.ui .
 */
void ScripterCore::preferencesDialog()
{
	ScripterPreferences* prefDia = new ScripterPreferences(Carrier, "scripterPreferences");
	prefDia->extPythonChk->setChecked(this->enableExtPython);
	prefDia->startupScriptGroup->setChecked(this->startupScriptEnable);
	prefDia->startupScriptPath->setText(this->startupScript);
	prefDia->importNamesChk->setChecked(this->importAllNames);
	prefDia->legacyAliasesChk->setChecked(this->legacyAliases);
	prefDia->useFakeStdinChk->setChecked(this->useDummyStdin);
	if (prefDia->exec())
	{
		this->enableExtPython = prefDia->extPythonChk->isChecked();
		this->startupScriptEnable = prefDia->startupScriptGroup->isChecked();
		this->startupScript = prefDia->startupScriptPath->text();
		this->importAllNames = prefDia->importNamesChk->isChecked();
		this->legacyAliases = prefDia->legacyAliasesChk->isChecked();
		this->useDummyStdin = prefDia->useFakeStdinChk->isChecked();
	}
	delete prefDia;
}
