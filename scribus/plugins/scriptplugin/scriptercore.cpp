#include "scriptercore.h"

#include <qglobal.h>
#include <qwidget.h>
#include <qstring.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qdom.h>
#include <qtextstream.h>
#include <cstdlib>

#include "customfdialog.h"
#include "helpbrowser.h"
#include "mpalette.h"
#include "seiten.h"
#include "layers.h"
#include "tree.h"
#include "menumanager.h"
#include "scraction.h"
#include "scpaths.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"

extern PrefsFile* prefsFile;

#include "scriptercore.moc"

ScripterCore::ScripterCore(QWidget* parent) :
	pcon(parent)
{
	menuMgr = Carrier->scrMenuMgr;
	scrScripterActions.clear();
	scrRecentScriptActions.clear();

	scrScripterActions.insert("scripterExecuteScript", new ScrAction(QObject::tr("&Execute Script..."), QKeySequence(), this, "scripterExecuteScript"));
	scrScripterActions.insert("scripterShowConsole", new ScrAction(QObject::tr("Show &Console"), QKeySequence(), this, "scripterShowConsole"));
	scrScripterActions.insert("scripterAboutScript", new ScrAction(QObject::tr("&About Script..."), QKeySequence(), this, "scripterAboutScript"));

	scrScripterActions["scripterShowConsole"]->setToggleAction(true);

	QObject::connect( scrScripterActions["scripterExecuteScript"], SIGNAL(activated()) , this, SLOT(slotTest()) );
	QObject::connect( scrScripterActions["scripterShowConsole"], SIGNAL(toggled(bool)) , this, SLOT(slotInteractiveScript(bool)) );
	QObject::connect( scrScripterActions["scripterAboutScript"], SIGNAL(activated()) , this, SLOT(aboutScript()) );

	menuMgr->createMenu("Scripter", QObject::tr("&Script"));
	menuMgr->addMenuToMenuBarAfter("Scripter","Windows");
	menuMgr->createMenu("ScribusScripts", QObject::tr("&Scribus Scripts"), "Scripter");
	menuMgr->addMenuItem(scrScripterActions["scripterExecuteScript"], "Scripter");
	menuMgr->createMenu("RecentScripts", QObject::tr("&Recent Scripts"), "Scripter");
	menuMgr->addMenuSeparator("Scripter");
	menuMgr->addMenuItem(scrScripterActions["scripterShowConsole"], "Scripter");
	menuMgr->addMenuItem(scrScripterActions["scripterAboutScript"], "Scripter");

	SavedRecentScripts.clear();
	ReadPlugPrefs();
	buildScribusScriptsMenu();
	buildRecentScriptsMenu();

	QObject::connect(pcon.OutWin, SIGNAL(returnPressed()), this, SLOT(slotExecute()));
	QObject::connect(&pcon, SIGNAL(paletteShown(bool)), this, SLOT(slotInteractiveScript(bool)));
}

ScripterCore::~ScripterCore()
{
	SavePlugPrefs();
}


void ScripterCore::buildScribusScriptsMenu()
{
	QString pfad = ScPaths::instance().scriptDir();
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad);
	QDir ds(pfad2, "*.py", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((ds.exists()) && (ds.count() != 0))
	{
		for (uint dc = 0; dc < ds.count(); ++dc)
		{
			QFileInfo fs(ds[dc]);
			QString strippedName=fs.baseName(false);
			scrScripterActions.insert(strippedName, new ScrAction( ScrAction::RecentScript, QIconSet(), strippedName, QKeySequence(), this, strippedName));
			connect( scrScripterActions[strippedName], SIGNAL(activatedData(QString)), this, SLOT(StdScript(QString)) );
			menuMgr->addMenuItem(scrScripterActions[strippedName], "ScribusScripts");
		}
	}


}

void ScripterCore::rebuildRecentScriptsMenu()
{
	for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrRecentScriptActions.begin(); it!=scrRecentScriptActions.end(); ++it )
		menuMgr->removeMenuItem((*it), "RecentScripts");

	scrRecentScriptActions.clear();
	uint max = QMIN(Carrier->Prefs.RecentDCount, RecentScripts.count());
	for (uint m = 0; m < max; ++m)
	{
		QString strippedName=RecentScripts[m];
		strippedName.remove(QDir::separator());
		scrRecentScriptActions.insert(strippedName, new ScrAction( ScrAction::RecentScript, QIconSet(), RecentScripts[m], QKeySequence(), this, strippedName));
		connect( scrRecentScriptActions[strippedName], SIGNAL(activatedData(QString)), this, SLOT(RecentScript(QString)) );
		menuMgr->addMenuItem(scrRecentScriptActions[strippedName], "RecentScripts");
	}
}

void ScripterCore::buildRecentScriptsMenu()
{
	RecentScripts.clear();
	scrRecentScriptActions.clear();
	if (SavedRecentScripts.count() != 0)
	{
		uint max = QMIN(Carrier->Prefs.RecentDCount, SavedRecentScripts.count());
		for (uint m = 0; m < max; ++m)
		{
			QFileInfo fd(SavedRecentScripts[m]);
			if (fd.exists())
			{
				QString strippedName=SavedRecentScripts[m];
				strippedName.remove(QDir::separator());
				scrRecentScriptActions.insert(strippedName, new ScrAction( ScrAction::RecentScript, QIconSet(), SavedRecentScripts[m], QKeySequence(), this, strippedName));
				connect( scrRecentScriptActions[strippedName], SIGNAL(activatedData(QString)), this, SLOT(RecentScript(QString)) );
				menuMgr->addMenuItem(scrRecentScriptActions[strippedName], "RecentScripts");
			}
		}
	}
}

void ScripterCore::FinishScriptRun()
{
	if (Carrier->HaveDoc)
	{
		Carrier->propertiesPalette->SetDoc(Carrier->doc);
		Carrier->propertiesPalette->updateCList();
		Carrier->propertiesPalette->Spal->setFormats(Carrier->doc);
		Carrier->propertiesPalette->SetLineFormats(Carrier->doc);
		Carrier->propertiesPalette->Cpal->SetColors(Carrier->doc->PageColors);
		Carrier->layerPalette->setLayers(&Carrier->doc->Layers, &Carrier->doc->ActiveLayer);
		Carrier->outlinePalette->BuildTree(Carrier->doc);
		Carrier->pagePalette->SetView(Carrier->view);
		Carrier->pagePalette->Rebuild();
		if (Carrier->view->SelItem.count() != 0)
			Carrier->HaveNewSel(Carrier->view->SelItem.at(0)->itemType());
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
	CustomFDialog diaf((QWidget*)parent(), scriptDir, QObject::tr("Open"), QObject::tr("Python Scripts (*.py);; All Files (*)"));
	if (diaf.exec())
	{
		fileName = diaf.selectedFile();
		slotRunScriptFile(fileName);

		if (RecentScripts.findIndex(fileName) == -1)
			RecentScripts.prepend(fileName);
		else
		{
			RecentScripts.remove(fileName);
			RecentScripts.prepend(fileName);
		}
		rebuildRecentScriptsMenu();
	}
	QDir::setCurrent(CurDirP);
	FinishScriptRun();
}

void ScripterCore::StdScript(QString basefilename)
{
	QString pfad = ScPaths::instance().scriptDir();
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad);
	QString fn = pfad2+basefilename+".py";
	//QString fn = pfad2+smen->text(id)+".py";
	QFileInfo fd(fn);
	if (!fd.exists())
		return;
	slotRunScriptFile(fn);
	FinishScriptRun();
}

void ScripterCore::RecentScript(QString fn)
{
	//QString fn = rmen->text(id);
	QFileInfo fd(fn);
	if (!fd.exists())
	{
		RecentScripts.remove(fn);
		rebuildRecentScriptsMenu();
		return;
	}
	slotRunScriptFile(fn);
	FinishScriptRun();
}

void ScripterCore::slotRunScriptFile(QString fileName)
{
	Carrier->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), false);
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
	PyObject* m = PyImport_AddModule((char*)"__main__");
	if (m == NULL)
		qDebug("Failed to get __main__ - aborting script");
	else
	{
		// FIXME: If filename contains chars outside 7bit ascii, might be problems
		PyObject* globals = PyModule_GetDict(m);
		// Build the Python code to run the script
		QString cm = QString("import sys,StringIO,traceback\n");
		/* Implementation of the help() in pydoc.py reads some OS variables
		 * for output settings. I use ugly hack to stop freezing calling help()
		 * in script. pv. */
		cm += QString("import os\nos.environ['PAGER'] = '/bin/false'\n"); // HACK
		cm += QString("sys.path[0] = \"%1\"\n").arg(fi.dirPath(true));
		cm += QString("try:\n");
		cm += QString("    execfile(\"%1\")\n").arg(fileName);
		cm += QString("except SystemExit:\n");
		cm += QString("    pass\n");
		// Capture the text of any other exception that's raised by the interpreter
		// into a StringIO buffer for later extraction.
		cm += QString("except Exception, err:\n");
		cm += QString("    f=StringIO.StringIO()\n");
		cm += QString("    traceback.print_exc(file=f)\n");
		cm += QString("    errorMsg = f.getvalue()\n");
		cm += QString("    del(f)\n");
		// We re-raise the exception so the return value of PyRun_String reflects
		// the fact that an exception has ocurred.
		cm += QString("    raise\n");
		// FIXME: if cmd contains chars outside 7bit ascii, might be problems
		QCString cmd = cm.latin1();
		// Now run the script in the interpreter's global scope
		PyObject* result = PyRun_String(cmd.data(), Py_file_input, globals, globals);
		// NULL is returned if an exception is set. We don't care about any
		// other return value (most likely None anyway) and can ignore it.
		if (result == NULL)
		{
			PyObject* errorMsgPyStr = PyMapping_GetItemString(globals, (char*)"errorMsg");
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
	qApp->restoreOverrideCursor();
}

QString ScripterCore::slotRunScript(QString Script)
{
	Carrier->ScriptRunning = true;
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
			cm = "import sys\nsys.path[0] = \""+CurDir+"\"\n";
			cm += "import cStringIO\n";
			// TODO: fix it during macro/properties merging
			//if (useDummyStdin)
			cm += QString("sys.stdin = cStringIO.StringIO()\n");
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
	comm[0] = (char*)"scribus";
	PySys_SetArgv(1, comm);
	PyRun_SimpleString(cmd.data());
	if (RetVal == 0)
	{
		RetString += ">>>";
		pcon.OutWin->Prompt = ">>>";
	}
	else
		pcon.OutWin->Prompt = "...";
	Carrier->ScriptRunning = false;
	qApp->restoreOverrideCursor();
	return RetString;
}

void ScripterCore::slotInteractiveScript(bool visible)
{
	scrScripterActions["scripterShowConsole"]->setOn(visible);
	pcon.setShown(visible);
}

void ScripterCore::slotExecute()
{
	pcon.OutWin->append(slotRunScript(pcon.OutWin->LastComm));
	pcon.OutWin->moveCursor(QTextEdit::MoveEnd, false);
	pcon.OutWin->scrollToBottom();
	pcon.OutWin->ensureCursorVisible();
	FinishScriptRun();
}

void ScripterCore::ReadPlugPrefs()
{
	PrefsContext* prefs = prefsFile->getPluginContext("scriptplugin");
	if (!prefs)
	{
		qDebug("scriptplugin: Unable to load prefs");
		return;
	}
	PrefsTable* prefRecentScripts = prefs->getTable("recentscripts");
	if (!prefRecentScripts)
	{
		qDebug("scriptplugin: Unable to get recent scripts");
		return;
	}
	// Load recent scripts from the prefs
	for (int i = 0; i < prefRecentScripts->getRowCount(); i++)
		SavedRecentScripts.append(prefRecentScripts->get(i,0));
}

void ScripterCore::SavePlugPrefs()
{
	PrefsContext* prefs = prefsFile->getPluginContext("scriptplugin");
	if (!prefs)
	{
		qDebug("scriptplugin: Unable to load prefs");
		return;
	}
	PrefsTable* prefRecentScripts = prefs->getTable("recentscripts");
	if (!prefRecentScripts)
	{
		qDebug("scriptplugin: Unable to get recent scripts");
		return;
	}
	for (uint i = 0; i < RecentScripts.count(); i++)
		prefRecentScripts->set(i, 0, RecentScripts[i]);
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
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("About Script") + " " + fi.fileName(), "en", "", html);
	dia->show();
}
