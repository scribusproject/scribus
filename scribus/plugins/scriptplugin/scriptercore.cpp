/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scriptercore.h"

#include <QGlobalStatic>
#include <QWidget>
#include <QString>
#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>
#include <QByteArray>
#include <QPixmap>
#include <cstdlib>

#include "runscriptdialog.h"
#include "helpbrowser.h"
#include "propertiespalette.h" //TODO Move the calls to this to a signal
#include "pagepalette.h" //TODO Move the calls to this to a signal
#include "layers.h" //TODO Move the calls to this to a signal
#include "outlinepalette.h" //TODO Move the calls to this to a signal
#include "menumanager.h"
#include "pconsole.h"
#include "scraction.h"
#include "scribuscore.h"
#include "scpaths.h"
#include "selection.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "prefsmanager.h"

ScripterCore::ScripterCore(QWidget* parent)
{
	pcon = new PythonConsole(parent);
	scrScripterActions.clear();
	scrRecentScriptActions.clear();
	returnString = "init";

	scrScripterActions.insert("scripterExecuteScript", new ScrAction(QObject::tr("&Execute Script..."), QKeySequence(), this));
	scrScripterActions.insert("scripterShowConsole", new ScrAction(QObject::tr("Show &Console"), QKeySequence(), this));
	scrScripterActions.insert("scripterAboutScript", new ScrAction(QObject::tr("&About Script..."), QKeySequence(), this));

	scrScripterActions["scripterShowConsole"]->setToggleAction(true);
	scrScripterActions["scripterShowConsole"]->setChecked(false);

	QObject::connect( scrScripterActions["scripterExecuteScript"], SIGNAL(triggered()) , this, SLOT(runScriptDialog()) );
	QObject::connect( scrScripterActions["scripterShowConsole"], SIGNAL(toggled(bool)) , this, SLOT(slotInteractiveScript(bool)) );
	QObject::connect( scrScripterActions["scripterAboutScript"], SIGNAL(triggered()) , this, SLOT(aboutScript()) );

	SavedRecentScripts.clear();
	ReadPlugPrefs();

	QObject::connect(pcon, SIGNAL(runCommand()), this, SLOT(slotExecute()));
	QObject::connect(pcon, SIGNAL(paletteShown(bool)), this, SLOT(slotInteractiveScript(bool)));
}

ScripterCore::~ScripterCore()
{
	SavePlugPrefs();
	delete pcon;
}

void ScripterCore::addToMainWindowMenu(ScribusMainWindow *mw)
{
	menuMgr = mw->scrMenuMgr;
	menuMgr->createMenu("Scripter", QObject::tr("&Script"));
	menuMgr->addMenuToMenuBarBefore("Scripter","Windows");
	menuMgr->createMenu("ScribusScripts", QObject::tr("&Scribus Scripts"), "Scripter");
	menuMgr->addMenuItem(scrScripterActions["scripterExecuteScript"], "Scripter");
	menuMgr->createMenu("RecentScripts", QObject::tr("&Recent Scripts"), "Scripter");
	menuMgr->addMenuSeparator("Scripter");
	menuMgr->addMenuItem(scrScripterActions["scripterShowConsole"], "Scripter");
	menuMgr->addMenuItem(scrScripterActions["scripterAboutScript"], "Scripter");
	buildScribusScriptsMenu();
	buildRecentScriptsMenu();
}


void ScripterCore::buildScribusScriptsMenu()
{
	QString pfad = ScPaths::instance().scriptDir();
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad);
	QDir ds(pfad2, "*.py", QDir::Name | QDir::IgnoreCase, QDir::Files | QDir::NoSymLinks);
	if ((ds.exists()) && (ds.count() != 0))
	{
		for (uint dc = 0; dc < ds.count(); ++dc)
		{
			QFileInfo fs(ds[dc]);
			QString strippedName=fs.baseName();
			scrScripterActions.insert(strippedName, new ScrAction( ScrAction::RecentScript, QPixmap(), QPixmap(), strippedName, QKeySequence(), this));
			connect( scrScripterActions[strippedName], SIGNAL(triggeredData(QString)), this, SLOT(StdScript(QString)) );
			menuMgr->addMenuItem(scrScripterActions[strippedName], "ScribusScripts");
		}
	}


}

void ScripterCore::rebuildRecentScriptsMenu()
{
	for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrRecentScriptActions.begin(); it!=scrRecentScriptActions.end(); ++it )
		menuMgr->removeMenuItem((*it), "RecentScripts");

	scrRecentScriptActions.clear();
	uint max = qMin(PrefsManager::instance()->appPrefs.RecentDCount, RecentScripts.count());
	for (uint m = 0; m < max; ++m)
	{
		QString strippedName=RecentScripts[m];
		strippedName.remove(QDir::separator());
		scrRecentScriptActions.insert(strippedName, new ScrAction( ScrAction::RecentScript, QPixmap(), QPixmap(), RecentScripts[m], QKeySequence(), this));
		connect( scrRecentScriptActions[strippedName], SIGNAL(triggeredData(QString)), this, SLOT(RecentScript(QString)) );
		menuMgr->addMenuItem(scrRecentScriptActions[strippedName], "RecentScripts");
	}
}

void ScripterCore::buildRecentScriptsMenu()
{
	RecentScripts = SavedRecentScripts;
	scrRecentScriptActions.clear();
	if (SavedRecentScripts.count() != 0)
	{
		uint max = qMin(PrefsManager::instance()->appPrefs.RecentDCount, SavedRecentScripts.count());
		for (uint m = 0; m < max; ++m)
		{
			QFileInfo fd(SavedRecentScripts[m]);
			if (fd.exists())
			{
				QString strippedName=SavedRecentScripts[m];
				strippedName.remove(QDir::separator());
				scrRecentScriptActions.insert(strippedName, new ScrAction( ScrAction::RecentScript, QPixmap(), QPixmap(), SavedRecentScripts[m], QKeySequence(), this));
				connect( scrRecentScriptActions[strippedName], SIGNAL(triggeredData(QString)), this, SLOT(RecentScript(QString)) );
				menuMgr->addMenuItem(scrRecentScriptActions[strippedName], "RecentScripts");
			}
		}
	}
}

void ScripterCore::FinishScriptRun()
{
	ScribusMainWindow* ScMW=ScCore->primaryMainWindow();
	if (ScMW->HaveDoc)
	{
		ScMW->propertiesPalette->setDoc(ScMW->doc);
		ScMW->layerPalette->setDoc(ScMW->doc);
		ScMW->outlinePalette->setDoc(ScMW->doc);
		ScMW->outlinePalette->BuildTree();
		ScMW->pagePalette->setView(ScMW->view);
		ScMW->pagePalette->Rebuild();
		ScMW->doc->RePos = true;
/*		QImage pgPix(10, 10, QImage::Format_ARGB32);
		QRect rd = QRect(0,0,9,9);
		ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
		for (int azz=0; azz<ScMW->doc->Items->count(); ++azz)
		{
			PageItem *ite = ScMW->doc->Items->at(azz);
			if (ite->Groups.count() != 0)
				ScMW->doc->GroupOnPage(ite);
			else
				ite->OwnPage = ScMW->doc->OnPage(ite);
			ite->setRedrawBounding();
			if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText)) // && (!ite->Redrawn))
			{
				if (ite->itemType() == PageItem::PathText)
				{
					ite->Frame = false;
					ite->updatePolyClip();
					ite->DrawObj(painter, rd);
				}
				else
				{
					if ((ite->prevInChain() != 0) || (ite->nextInChain() != 0))
					{
						PageItem *nextItem = ite;
						while (nextItem->prevInChain() != 0)
							nextItem = nextItem->prevInChain();
						ite = nextItem;
						ite->DrawObj(painter, rd);
					}
					else
						ite->DrawObj(painter, rd);
				}
			}
		}
		delete painter; */
		ScMW->doc->RePos = false;
		if (ScMW->doc->m_Selection->count() != 0)
		{
			ScMW->doc->m_Selection->itemAt(0)->emitAllToGUI();
			ScMW->HaveNewSel(ScMW->doc->m_Selection->itemAt(0)->itemType());
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
	QString curDirPath = QDir::currentPath();
	RunScriptDialog dia( ScCore->primaryMainWindow(), m_enableExtPython );
	if (dia.exec())
	{
		fileName = dia.selectedFile();
		slotRunScriptFile(fileName, dia.extensionRequested());

		if (RecentScripts.indexOf(fileName) == -1)
			RecentScripts.prepend(fileName);
		else
		{
			RecentScripts.removeAll(fileName);
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
	QFileInfo fd(fn);
	if (!fd.exists())
		return;
	slotRunScriptFile(fn);
	FinishScriptRun();
}

void ScripterCore::RecentScript(QString fn)
{
	QFileInfo fd(fn);
	if (!fd.exists())
	{
		RecentScripts.removeAll(fn);
		rebuildRecentScriptsMenu();
		return;
	}
	slotRunScriptFile(fn);
	FinishScriptRun();
}

void ScripterCore::slotRunScriptFile(QString fileName, bool inMainInterpreter)
{
	PyThreadState *stateo = NULL;
	PyThreadState *state = NULL;
	QFileInfo fi(fileName);
	QByteArray na = fi.fileName().toLocal8Bit();
	// Set up a sub-interpreter if needed:
	if (!inMainInterpreter)
	{
		ScCore->primaryMainWindow()->propertiesPalette->unsetDoc();
		ScCore->primaryMainWindow()->pagePalette->setView(NULL);
		ScCore->primaryMainWindow()->ScriptRunning = true;
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
		// Create the sub-interpreter
		// FIXME: This calls abort() in a Python debug build. We're doing something wrong.
		stateo = PyEval_SaveThread();
		state = Py_NewInterpreter();
		// Chdir to the dir the script is in
		QDir::setCurrent(fi.absolutePath());
		// Init the scripter module in the sub-interpreter
		initscribus(ScCore->primaryMainWindow());
	}
	// Make sure sys.argv[0] is the path to the script
	char* comm[2];
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
		//QString cm = QString("from __future__ import division\n"); removed due #5252 PV
		QString cm = QString("import sys\n");
		cm        += QString("import cStringIO\n");
		/* Implementation of the help() in pydoc.py reads some OS variables
		 * for output settings. I use ugly hack to stop freezing calling help()
		 * in script. pv. */
		cm        += QString("import os\nos.environ['PAGER'] = '/bin/false'\n"); // HACK
		cm        += QString("sys.path[0] = \"%1\"\n").arg(fi.absolutePath());
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
		QByteArray cmd = cm.toLatin1();
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
				ScCore->closeSplash();
				QMessageBox::warning(ScCore->primaryMainWindow(),
									tr("Script error"),
									"<qt><p>"
									+ tr("If you are running an official script report it at <a href=\"http://bugs.scribus.net\">bugs.scribus.net</a> please.")
									+ "</p><pre>" +errorMsg + "</pre><p>"
									+ tr("This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.")
									+ "</p></qt>");
			}
		} // end if result == NULL
		// Because 'result' may be NULL, not a PyObject*, we must call PyXDECREF not Py_DECREF
		Py_XDECREF(result);
	} // end if m == NULL
	if (!inMainInterpreter)
	{
		Py_EndInterpreter(state);
		PyEval_RestoreThread(stateo);
//		qApp->restoreOverrideCursor();
	}
	ScCore->primaryMainWindow()->ScriptRunning = false;
}

void ScripterCore::slotRunScript(const QString Script)
{
	ScCore->primaryMainWindow()->propertiesPalette->unsetDoc();
	ScCore->primaryMainWindow()->pagePalette->setView(NULL);
	ScCore->primaryMainWindow()->ScriptRunning = true;
	inValue = Script;
	QString cm;
	cm = "# -*- coding: utf8 -*- \n";
	if (PyThreadState_Get() != NULL)
	{
		initscribus(ScCore->primaryMainWindow());
		/* HACK: following loop handles all input line by line.
		It *should* use I.C. because of docstrings etc. I.I. cannot
		handle docstrings right.
		Calling all code in one command:
		ia = code.InteractiveInterpreter() ia.runsource(getval())
		works fine in plain Python. Not here. WTF? */
		cm += (
				"try:\n"
				"    import cStringIO\n"
				"    scribus._bu = cStringIO.StringIO()\n"
				"    sys.stdout = scribus._bu\n"
				"    sys.stderr = scribus._bu\n"
				"    sys.argv = ['scribus', 'ext']\n" // this is the PySys_SetArgv replacement
				"    for i in scribus.getval().splitlines():\n"
				"        scribus._ia.push(i)\n"
				"    scribus.retval(scribus._bu.getvalue())\n"
				"    sys.stdout = sys.__stdout__\n"
				"    sys.stderr = sys.__stderr__\n"
				"except SystemExit:\n"
				"    print 'Catched SystemExit - it is not good for Scribus'\n"
				"except KeyboardInterrupt:\n"
				"    print 'Catched KeyboardInterrupt - it is not good for Scribus'\n"
			  );
	}
	// Set up sys.argv
	/* PV - WARNING: THIS IS EVIL! This code summons a crash - see
	bug #3510. I don't know why as the Python C API is a little
	bit magic for me. It looks like it replaces the cm QString or what...
	"In file tools/qgarray.cpp, line 147: Out of memory"
	Anyway - sys.argv is set above
	char* comm[1];
	comm[0] = const_cast<char*>("scribus");
	// the scripter console runs everything in the main interpreter
	// tell the code it's running there.
	comm[1] = const_cast<char*>("ext");
	PySys_SetArgv(2, comm); */
	// then run the code
	PyObject* m = PyImport_AddModule((char*)"__main__");
	if (m == NULL)
		qDebug("Failed to get __main__ - aborting script");
	else
	{
		PyObject* globals = PyModule_GetDict(m);
		PyObject* result = PyRun_String(cm.toUtf8().data(), Py_file_input, globals, globals);
		if (result == NULL)
		{
			PyErr_Print();
			QMessageBox::warning(ScCore->primaryMainWindow(), tr("Script error"),
					"<qt>" + tr("There was an internal error while trying the "
					   "command you entered. Details were printed to "
					   "stderr. ") + "</qt>");
		}
		else
		// Because 'result' may be NULL, not a PyObject*, we must call PyXDECREF not Py_DECREF
			Py_XDECREF(result);
	}
	ScCore->primaryMainWindow()->ScriptRunning = false;
}

void ScripterCore::slotInteractiveScript(bool visible)
{
	QObject::disconnect( scrScripterActions["scripterShowConsole"], SIGNAL(toggled(bool)) , this, SLOT(slotInteractiveScript(bool)) );

	scrScripterActions["scripterShowConsole"]->setChecked(visible);
	pcon->setFonts();
	pcon->setShown(visible);

	QObject::connect( scrScripterActions["scripterShowConsole"], SIGNAL(toggled(bool)) , this, SLOT(slotInteractiveScript(bool)) );
}

void ScripterCore::slotExecute()
{
	slotRunScript(pcon->command());
	pcon->outputEdit->append(returnString);
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
	// and have the console window set up its position
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
	for (int i = 0; i < RecentScripts.count(); i++)
		prefRecentScripts->set(i, 0, RecentScripts[i]);
	// then save more general preferences
	prefs->set("extensionscripts", m_enableExtPython);
	prefs->set("importall", m_importAllNames);
	prefs->set("startupscript", m_startupScript);
}

void ScripterCore::aboutScript()
{
	QString fname = ScCore->primaryMainWindow()->CFileDialog(".", tr("Examine Script"), tr("Python Scripts (*.py *.PY);;All Files (*)"), "", fdNone);
	if (fname == QString::null)
		return;
	QString html("<html><body>");
	QFileInfo fi = QFileInfo(fname);
	QFile input(fname);
	if(!input.open(QIODevice::ReadOnly))
		return;
	QTextStream intputstream(&input);
	QString content = intputstream.readAll();
	QString docstring = content.section("\"\"\"", 1, 1);
	if (!docstring.isEmpty())
	{
		html += QString("<h1>%1 %2</h1>").arg( tr("Documentation for:")).arg(fi.fileName());
		html += QString("<p>%1</p>").arg(docstring.replace("\n\n", "<br><br>"));
	}
	else
	{
		html += QString("<p><b>%1 %2 %3</b></p>").arg( tr("Script")).arg(fi.fileName()).arg( tr(" doesn't contain any docstring!"));
		html += QString("<pre>%4</pre>").arg(content);
	}
	html += "</body></html>";
	input.close();
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("About Script") + " " + fi.fileName(), "en");
	dia->setText(html);
	dia->show();
}

void ScripterCore::initExtensionScripts()
{
	// Nothing to do currently
}

void ScripterCore::runStartupScript()
{
	if ((m_enableExtPython) && (!m_startupScript.isNull()))
	{
		if (QFile::exists(this->m_startupScript))
		{
			// run the script in the main interpreter. The user will be informed
			// with a dialog if something has gone wrong.
			this->slotRunScriptFile(this->m_startupScript, true);
		}
		else
			qDebug("Startup script enabled, but couln't find script %s.", m_startupScript.toAscii().constData());
	}
}

void ScripterCore::languageChange()
{
	scrScripterActions["scripterExecuteScript"]->setText(QObject::tr("&Execute Script..."));
	scrScripterActions["scripterShowConsole"]->setText(QObject::tr("Show &Console"));
	scrScripterActions["scripterAboutScript"]->setText(QObject::tr("&About Script..."));

	menuMgr->setText("Scripter", QObject::tr("&Script"));
	menuMgr->setText("ScribusScripts", QObject::tr("&Scribus Scripts"));
	menuMgr->setText("RecentScripts", QObject::tr("&Recent Scripts"));
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
	QByteArray cmd = cm.toUtf8();
	if (PyRun_SimpleString(cmd.data()))
	{
		PyErr_Print();
		QMessageBox::warning(ScCore->primaryMainWindow(), tr("Script error"),
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
