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
#include <qtextedit.h>

#include "runscriptdialog.h"
#include "helpbrowser.h"
#include "mpalette.h" //TODO Move the calls to this to a signal
#include "seiten.h" //TODO Move the calls to this to a signal
#include "layers.h" //TODO Move the calls to this to a signal
#include "tree.h" //TODO Move the calls to this to a signal
#include "menumanager.h"
#include "pconsole.h"
#include "scraction.h"
#include "scpaths.h"
#include "selection.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "prefsmanager.h"

#include "scriptercore.moc"

ScripterCore::ScripterCore(QWidget* parent)
{
	pcon = new PythonConsole(parent);
	menuMgr = ScMW->scrMenuMgr;
	scrScripterActions.clear();
	scrRecentScriptActions.clear();

	scrScripterActions.insert("scripterExecuteScript", new ScrAction(QObject::tr("&Execute Script..."), QKeySequence(), this, "scripterExecuteScript"));
	scrScripterActions.insert("scripterShowConsole", new ScrAction(QObject::tr("Show &Console"), QKeySequence(), this, "scripterShowConsole"));
	scrScripterActions.insert("scripterAboutScript", new ScrAction(QObject::tr("&About Script..."), QKeySequence(), this, "scripterAboutScript"));

	scrScripterActions["scripterShowConsole"]->setToggleAction(true);

	QObject::connect( scrScripterActions["scripterExecuteScript"], SIGNAL(activated()) , this, SLOT(runScriptDialog()) );
	QObject::connect( scrScripterActions["scripterShowConsole"], SIGNAL(toggled(bool)) , this, SLOT(slotInteractiveScript(bool)) );
	QObject::connect( scrScripterActions["scripterAboutScript"], SIGNAL(activated()) , this, SLOT(aboutScript()) );

	menuMgr->createMenu("Scripter", QObject::tr("&Script"));
	menuMgr->addMenuToMenuBarAfter("Scripter","Extras");
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

	QObject::connect(pcon, SIGNAL(runCommand()), this, SLOT(slotExecute()));
	QObject::connect(pcon, SIGNAL(paletteShown(bool)), this, SLOT(slotInteractiveScript(bool)));
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
	uint max = QMIN(PrefsManager::instance()->appPrefs.RecentDCount, RecentScripts.count());
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
		uint max = QMIN(PrefsManager::instance()->appPrefs.RecentDCount, SavedRecentScripts.count());
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
	if (ScMW->HaveDoc)
	{
		ScMW->propertiesPalette->setDoc(ScMW->doc);
		ScMW->propertiesPalette->updateCList();
		ScMW->propertiesPalette->Spal->setFormats(ScMW->doc);
		ScMW->propertiesPalette->SetLineFormats(ScMW->doc);
		ScMW->propertiesPalette->updateColorList();
		ScMW->layerPalette->setLayers(&ScMW->doc->Layers, ScMW->doc->activeLayer());
		ScMW->outlinePalette->setDoc(ScMW->doc);
		ScMW->outlinePalette->BuildTree();
		ScMW->pagePalette->setView(ScMW->view);
		ScMW->pagePalette->Rebuild();
		ScMW->doc->RePos = true;
		QPixmap pgPix(10, 10);
		QRect rd = QRect(0,0,9,9);
		ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
		for (uint azz=0; azz<ScMW->doc->Items->count(); ++azz)
		{
			PageItem *ite = ScMW->doc->Items->at(azz);
			if (ite->Groups.count() != 0)
				ScMW->doc->GroupOnPage(ite);
			else
				ite->OwnPage = ScMW->doc->OnPage(ite);
			//ScMW->view->setRedrawBounding(ite);
			ite->setRedrawBounding();
			if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText) && (!ite->Redrawn))
			{
				if (ite->itemType() == PageItem::PathText)
				{
					ite->Frame = false;
					ite->UpdatePolyClip();
					ite->DrawObj(painter, rd);
				}
				else
				{
					if ((ite->BackBox != 0) || (ite->NextBox != 0))
					{
						PageItem *nextItem = ite;
						while (nextItem != 0)
						{
							if (nextItem->BackBox != 0)
								nextItem = nextItem->BackBox;
							else
								break;
						}
						ite = nextItem;
						ite->DrawObj(painter, rd);
					}
					else
						ite->DrawObj(painter, rd);
				}
			}
		}
		delete painter;
		ScMW->doc->RePos = false;
		//if (ScMW->view->SelItem.count() != 0)
		if (ScMW->doc->selection->count() != 0)
		{
			//ScMW->view->EmitValues(ScMW->view->SelItem.at(0));
			//ScMW->view->EmitValues(ScMW->doc->selection->itemAt(0));
			ScMW->doc->selection->itemAt(0)->emitAllToGUI();
			//ScMW->HaveNewSel(ScMW->view->SelItem.at(0)->itemType());
			ScMW->HaveNewSel(ScMW->doc->selection->itemAt(0)->itemType());
		}
		else
			ScMW->HaveNewSel(-1);
		ScMW->view->DrawNew();
		//CB Really only need (want?) this for new docs, but we need it after a call to ScMW doFileNew.
		//We don't want it in cmddoc calls as itll interact with the GUI before a script may be finished.
		ScMW->HaveNewDoc();
	}
}

void ScripterCore::runScriptDialog()
{
	QString fileName;
	QString curDirPath = QDir::currentDirPath();
	RunScriptDialog dia( ScMW, m_enableExtPython );
	if (dia.exec())
	{
		fileName = dia.selectedFile();
		slotRunScriptFile(fileName, dia.extensionRequested());

		if (RecentScripts.findIndex(fileName) == -1)
			RecentScripts.prepend(fileName);
		else
		{
			RecentScripts.remove(fileName);
			RecentScripts.prepend(fileName);
		}
		rebuildRecentScriptsMenu();
	}
	QDir::setCurrent(curDirPath);
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

void ScripterCore::slotRunScriptFile(QString fileName, bool inMainInterpreter)
{
	PyThreadState *stateo, *state;
	QFileInfo fi(fileName);
	QCString na = fi.fileName().latin1();
	// Set up a sub-interpreter if needed:
	if (!inMainInterpreter)
	{
		ScMW->ScriptRunning = true;
		qApp->setOverrideCursor(QCursor(waitCursor), false);
		// Create the sub-interpreter
		// FIXME: This calls abort() in a Python debug build. We're doing something wrong.
		stateo = PyEval_SaveThread();
		state = Py_NewInterpreter();
		// Chdir to the dir the script is in
		QDir::setCurrent(fi.dirPath(true));
		// Init the scripter module in the sub-interpreter
		initscribus(ScMW);
	}
	// Make sure sys.argv[0] is the path to the script
	char* comm[1];
	comm[0] = na.data();
	// and tell the script if it's running in the main intepreter or
	// a subinterpreter using the second argument, ie sys.argv[1]
	if (inMainInterpreter)
		comm[1] = const_cast<char*>("ext");
	else
		comm[1] = const_cast<char*>("sub");
	PySys_SetArgv(2, comm);
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
		cm        += QString("import os\nos.environ['PAGER'] = '/bin/false'\n"); // HACK
		cm        += QString("sys.path[0] = \"%1\"\n").arg(fi.dirPath(true));
		// Replace sys.stdin with a dummy StringIO that always returns
		// "" for read
		cm        += QString("sys.stdin = cStringIO.StringIO()\n");
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
				ScMW->closeSplash();
				QMessageBox::warning(ScMW,
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
		qApp->restoreOverrideCursor();
	}
	ScMW->ScriptRunning = false;
}

QString ScripterCore::slotRunScript(QString Script)
{
	ScMW->ScriptRunning = true;
	qApp->setOverrideCursor(QCursor(waitCursor), false);
	InValue = Script;
	QString CurDir = QDir::currentDirPath();
	QString cm(
			"# -*- coding: utf8 -*- \n"
//			"try:\n"
			);
	if(PyThreadState_Get() != NULL)
	{
		initscribus(ScMW);
		if (RetVal == 0)
			cm += (
				"scribus._bu = cStringIO.StringIO()\n"
				"sys.stdout = scribus._bu\n"
				"sys.stderr = scribus._bu\n"
				);
		/* HACK: following loop handles all input line by line.
		It *should* use I.C. because of docstrings etc. I.I. cannot
		handle docstrings right.
		Calling all code in one command:
		ia = code.InteractiveInterpreter() ia.runsource(getval())
		works fine in plain Python. Not here. WTF?
		*/
		cm += (
				"for i in scribus.getval().splitlines():\n"
				"    scribus._ia.push(i)\n"
				"scribus.retval(scribus._bu.getvalue(), 0)\n"
//				"finally:\n"
				"sys.stdout = sys.__stdout__\n"
				"sys.stderr = sys.__stderr__\n"
			  );
	}
	QCString cmd = cm.utf8();
	// Set up sys.argv
	char* comm[1];
	comm[0] = const_cast<char*>("scribus");
	// the scripter console runs everything in the main interpreter
	// tell the code it's running there.
	comm[1] = const_cast<char*>("ext");
	PySys_SetArgv(2, comm);
	// then run the code
	PyObject* m = PyImport_AddModule((char*)"__main__");
	if (m == NULL)
		qDebug("Failed to get __main__ - aborting script");
	else
	{
		PyObject* globals = PyModule_GetDict(m);
		PyObject* result = PyRun_String(cmd.data(), Py_file_input, globals, globals);
		if (result == NULL)
		{
			PyErr_Print();
			QMessageBox::warning(ScMW, tr("Script error"),
					"<qt>" + tr("There was an internal error while trying the "
					   "command you entered. Details were printed to "
					   "stderr. ") + "</qt>");
		}
	}
	ScMW->ScriptRunning = false;
	qApp->restoreOverrideCursor();
	return RetString;
}

void ScripterCore::slotInteractiveScript(bool visible)
{
	QObject::disconnect( scrScripterActions["scripterShowConsole"], SIGNAL(toggled(bool)) , this, SLOT(slotInteractiveScript(bool)) );

	scrScripterActions["scripterShowConsole"]->setOn(visible);
	pcon->setShown(visible);

	QObject::connect( scrScripterActions["scripterShowConsole"], SIGNAL(toggled(bool)) , this, SLOT(slotInteractiveScript(bool)) );
}

void ScripterCore::slotExecute()
{
	pcon->outputEdit->append(slotRunScript(pcon->command));
	pcon->outputEdit->setCursorPosition(0, 0);
	pcon->commandEdit->ensureCursorVisible();
	FinishScriptRun();
}

void ScripterCore::ReadPlugPrefs()
{
	PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("scriptplugin");
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
	// then get more general preferences
	m_enableExtPython = prefs->getBool("extensionscripts",false);
	m_importAllNames = prefs->getBool("importall",true);
	m_startupScript = prefs->get("startupscript", QString::null);
}

void ScripterCore::SavePlugPrefs()
{
	PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("scriptplugin");
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
	// then save more general preferences
	prefs->set("extensionscripts", m_enableExtPython);
	prefs->set("importall", m_importAllNames);
	prefs->set("startupscript", m_startupScript);
}

/* 11/1/2004 pv - Show docstring of the script to the user.
 * I don't know how to get docstring via e.g. pydoc because of
 * it needs to run script => error cannot find scribus module
 */
void ScripterCore::aboutScript()
{
	QString fname = ScMW->CFileDialog(".", tr("Examine Script"), tr("Python Scripts (*.py)"), "", 0, 0, 0, 0);
	if (fname == QString::null)
		return;
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
	if (!docstring.isEmpty())
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

void ScripterCore::initExtensionScripts()
{
	// Nothing to do currently
}

void ScripterCore::runStartupScript()
{
	if ((m_enableExtPython) && (m_startupScript))
	{
		if (QFile::exists(this->m_startupScript))
		{
			// run the script in the main interpreter. The user will be informed
			// with a dialog if something has gone wrong.
			this->slotRunScriptFile(this->m_startupScript, true);
		}
		else
			qDebug("Startup script enabled, but couln't find script %s.", m_startupScript.ascii());
	}
}

void ScripterCore::languageChange()
{
	scrScripterActions["scripterExecuteScript"]->setMenuText(QObject::tr("&Execute Script..."));
	scrScripterActions["scripterShowConsole"]->setMenuText(QObject::tr("Show &Console"));
	scrScripterActions["scripterAboutScript"]->setMenuText(QObject::tr("&About Script..."));

	menuMgr->setMenuText("Scripter", QObject::tr("&Script"));
	menuMgr->setMenuText("ScribusScripts", QObject::tr("&Scribus Scripts"));
	menuMgr->setMenuText("RecentScripts", QObject::tr("&Recent Scripts"));
}

bool ScripterCore::setupMainInterpreter()
{
	QString cm = QString(
		"# -*- coding: utf-8 -*-\n"
		"import scribus\n"
		"import sys\n"
		"sys.path[0] = \"%1\"\n"
		"import cStringIO\n"
		"sys.stdin = cStringIO.StringIO()\n"
		"import code\n"
		"scribus._ia = code.InteractiveConsole(globals())\n"
		).arg(ScPaths::instance().scriptDir());
	if (m_importAllNames)
		cm += "from scribus import *\n";
	QCString cmd = cm.utf8();
	if (PyRun_SimpleString(cmd.data()))
	{
		PyErr_Print();
		QMessageBox::warning(ScMW, tr("Script error"),
				tr("Setting up the Python plugin failed. "
				   "Error details were printed to stderr. "));
		return false;
	}
	else
		return true;
}

void ScripterCore::setStartupScript(const QString& newScript)
{
	m_startupScript = newScript;
}

void ScripterCore::setExtensionsEnabled(bool enable)
{
	m_enableExtPython = enable;
}

const QString & ScripterCore::startupScript() const
{
	return m_startupScript;
}

bool ScripterCore::extensionsEnabled() const
{
	return m_enableExtPython;
}
