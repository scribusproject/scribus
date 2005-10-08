/***************************************************************************
	begin                : May 2005
	copyright            : (C) 2005 by Craig Bradney	
	email                : cbradney@zip.com.au
	copyright            : (C) 2001 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScApp program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <iostream>
#include <cstdlib>

#include <qfont.h>
#include <qstring.h>
#include <qtranslator.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextcodec.h>

#include "scribusapp.h"
#include "scribusapp.moc"
#include "scribus.h"
#include "scpaths.h"
#include "prefsfile.h"
#include "langmgr.h"
#include "prefsmanager.h"
#include "commonstrings.h"

#define ARG_VERSION "--version"
#define ARG_HELP "--help"
#define ARG_LANG "--lang"
#define ARG_AVAILLANG "--langs-available"
#define ARG_NOSPLASH "--no-splash"
#define ARG_NOGUI "--no-gui"
#define ARG_DISPLAY "--display"
#define ARG_FONTINFO "--font-info"
#define ARG_SWAPDIABUTTONS "--swap-buttons"

#define ARG_VERSION_SHORT "-v"
#define ARG_HELP_SHORT "-h"
#define ARG_LANG_SHORT "-l"
#define ARG_AVAILLANG_SHORT "-la"
#define ARG_NOSPLASH_SHORT "-ns"
#define ARG_NOGUI_SHORT "-g"
#define ARG_DISPLAY_SHORT "-d"
#define ARG_FONTINFO_SHORT "-fi"
#define ARG_SWAPDIABUTTONS_SHORT "-sb"

// Qt wants -display not --display or -d
#define ARG_DISPLAY_QT "-display"


extern ScribusQApp* ScQApp;

bool ScribusQApp::useGUI=false;

ScribusQApp::ScribusQApp( int & argc, char ** argv ) : QApplication(argc, argv)
{
	ScQApp=this;
	ScApp=NULL;
	lang="";
}

ScribusQApp::~ScribusQApp()
{
	PrefsManager::deleteInstance();
}

void ScribusQApp::initLang()
{
	QStringList langs = getLang(QString(lang));

	if (!langs.isEmpty())
		installTranslators(langs);
}

void ScribusQApp::parseCommandLine()
{
	showSplash=true;
	QString arg = "";
	bool usage=false;
	bool header=false;
	bool availlangs=false;
	bool version=false;
	showFontInfo=false;
	swapDialogButtonOrder=false;

	//Parse for command line information options, and lang
	for(int i = 1; i < argc(); i++) 
	{
		arg = argv()[i];
		
		if ((arg == ARG_LANG || arg == ARG_LANG_SHORT) && (++i < argc())) {
			lang = argv()[i];
		}
		else if (arg == ARG_VERSION || arg == ARG_VERSION_SHORT) {
			header=true;
			version=true;
		} else if (arg == ARG_HELP || arg == ARG_HELP_SHORT) {
			header=true;
			usage=true;
		} else if (arg == ARG_AVAILLANG || arg == ARG_AVAILLANG_SHORT) {
			header=true;
			availlangs=true;
		}
	}
	//Init translations
	initLang();	
	//Show command line help
	if (header)
		showHeader();
	if (version)
		showVersion();
	if (availlangs)
		showAvailLangs();
	if (usage)
		showUsage();
	//Dont run the GUI init process called from main.cpp, and return
	if (!header)
		useGUI=true;
	else
		return;
	//We are going to run something other than command line help
	for(int i = 1; i < argc(); i++) {
		arg = argv()[i];
		
		if ((arg == ARG_LANG || arg == ARG_LANG_SHORT) && (++i < argc())) {
		}
		else if (arg == ARG_NOSPLASH || arg == ARG_NOSPLASH_SHORT) {
			showSplash = false;
		} else if (arg == ARG_NOGUI || arg == ARG_NOGUI_SHORT) {
			useGUI=false;
		} else if (arg == ARG_FONTINFO || arg == ARG_FONTINFO_SHORT) {
			showFontInfo=true;
		} else if (arg == ARG_SWAPDIABUTTONS || arg == ARG_SWAPDIABUTTONS_SHORT) {
			swapDialogButtonOrder=true;	
		} else if ((arg == ARG_DISPLAY || arg==ARG_DISPLAY_SHORT || arg==ARG_DISPLAY_QT) && ++i < argc()) {
			// allow setting of display, QT expect the option -display <display_name> so we discard the
			// last argument. FIXME: Qt only understands -display not --display and -d , we need to work
			// around this.
		} else if (strncmp(arg,"-psn_",4) == 0)
		{
			// Andreas Vox: Qt/Mac has -psn_blah flags that must be accepted.
		} else {
			file = QFile::decodeName(argv()[i]);
			if (!QFileInfo(file).exists()) {
				showHeader();
				if (file.left(1) == "-" || file.left(2) == "--") {
					std::cout << QObject::tr("Invalid argument: ") << file << std::endl;
				} else {
					std::cout << QObject::tr("File %1 does not exist, aborting.").arg(file) << std::endl;
				}
				showUsage();
				useGUI=false;
				return;
			}
		}
	}
}

int ScribusQApp::init()
{
	processEvents();
	if (useGUI)
	{
		scribus = new ScribusApp();
		ScApp=scribus;
		if (!scribus)
			exit(EXIT_FAILURE);
		int scribusRetVal = scribus->initScribus(showSplash, showFontInfo, lang);
		if (scribusRetVal == 1)
			return(EXIT_FAILURE);

		setMainWidget(scribus);
		connect(this, SIGNAL(lastWindowClosed()), this, SLOT(quit()));

		scribus->show();
		scribus->ShowSubs();

		if (!file.isEmpty())
			scribus->loadDoc(file);
		else
		{
			if (PrefsManager::instance()->appPrefs.showStartupDialog)
				scribus->startUpDialog();
			else
				scribus->setFocus();
		}

		// A hook for plug-ins and scripts to trigger on. Some plugins and scripts
		// require the app to be fully set up (in particular, the main window to be
		// built and shown) before running their setup.
		emit appStarted();
	}
	return EXIT_SUCCESS;
}

/*!
\fn void getLang(QString lang)
\author Franz Schmid
\author Alessandro Rimoldi
\date Mon Feb  9 14:07:46 CET 2004
\brief If the lang argument is empty, returns the value in the locales

The lang is always a two character code, except for "en_GB" where
the whole string is returned. For all the other locales starting
with "en", no locale is returned.

(Inspired from Klocale.cpp)

\param lang QString a two letter string describing the lang environement
\retval QString A string describing the language environement
*/

QStringList ScribusQApp::getLang(QString lang)
{
	QStringList langs;

	// read the locales
	if (!lang.isEmpty())
		langs.push_back(lang);
	
	//add in user preferences lang, only overridden by lang command line option
	QString Pff = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus");
	QFileInfo Pffi = QFileInfo(Pff);
	if (Pffi.exists())
	{
		QString PrefsPfad;
		if (Pffi.isDir())
			PrefsPfad = Pff;
		else
			PrefsPfad = QDir::homeDirPath();
		QString prefsXMLFile=QDir::convertSeparators(PrefsPfad + "/prefs13.xml");
		QFileInfo infoPrefsFile(prefsXMLFile);
		if (infoPrefsFile.exists())
		{
			PrefsFile* prefsFile = new PrefsFile(prefsXMLFile);
			if (prefsFile) {
				PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
				if (userprefsContext) {
					QString prefslang = userprefsContext->get("gui_language","");
					if (!prefslang.isEmpty())
						langs.push_back(prefslang);
				}
			}
		}
	}

	if (!(lang = ::getenv("LC_ALL")).isEmpty())
		langs.push_back(lang);
	if (!(lang = ::getenv("LC_MESSAGES")).isEmpty())
		langs.push_back(lang);
	if (!(lang = ::getenv("LANG")).isEmpty())
		langs.push_back(lang);

	langs.push_back(QString(QTextCodec::locale()));

	// remove duplicate entries...
	for (QStringList::Iterator it = langs.fromLast(); it != langs.begin(); --it)
		if (langs.contains(*it) > 1)
			it = langs.remove(it);

	return langs;
} 


/*!
\fn void installTranslators(QApplication *app, QStringList langs)
\author Franz Schmid
\author Alessandro Rimoldi
\date Mon Feb  9 14:07:46 CET 2004
\brief Loads the translations for Scribus and for the Plugins
\param app QApplication pointer to the application object
\param lang QString a two letter string describing the lang environement
\retval void
*/
void ScribusQApp::installTranslators(QStringList langs)
{
	QString lang = "";
	static QTranslator *trans = 0;
	
	if ( trans )
	{
		removeTranslator( trans );
		delete trans;
	}
	trans = new QTranslator(0);
	QString path = ScPaths::instance().libDir();
	path += "scribus";

	bool loaded = false;
	for (QStringList::Iterator it = langs.begin(); it != langs.end() && !loaded; ++it) {
		lang=(*it).left(5);
		if (lang == "en")
			break;
		else if (loaded = trans->load(QString(path + '.' + lang), "."))
			loaded = true;
	}

	if (loaded)
		installTranslator(trans);
	/* CB TODO, currently disabled, because its broken broken broken
	path = ScPaths::instance().pluginDir();
	QDir dir(path , "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if (dir.exists() && (dir.count() != 0)) {
		for (uint i = 0; i < dir.count(); ++i) {
			QFileInfo file(path + dir[i]);
			if ((file.extension(false).lower() == "qm")
			&& (file.extension(true).lower().left(5) == lang)) {
				trans = new QTranslator(0);
				trans->load(QString(path + dir[i]), ".");
				installTranslator(trans);
			}
		}
	}*/
}

void ScribusQApp::changeGUILanguage(QString newGUILang)
{
	QStringList newLangs;
	if (newGUILang.isEmpty())
		newLangs.append("en");
	else
		newLangs.append(newGUILang);
	installTranslators(newLangs);
}


/*!
\fn void showUsage()
\author Franz Schmid
\author Alessandro Rimoldi
\date Mon Feb  9 14:07:46 CET 2004
\brief If no argument specified the lang, returns the one in the locales
\param lang QString a two letter string describing the lang environement
\retval QString A string describing the language environement
*/

void ScribusQApp::showUsage()
{
	std::cout << QObject::tr("Usage: scribus [option ... ] [file]") << std::endl;
	std::cout << QObject::tr("Options:") << std::endl;
	std::cout << "  " << ARG_HELP_SHORT           << ",  " << ARG_HELP             << "             " << QObject::tr("Print help (this message) and exit")     << std::endl;
	std::cout << "  " << ARG_LANG_SHORT           << ",  " << ARG_LANG             << "             " << QObject::tr("Uses xx as shortcut for a language")     << std::endl;
	std::cout << "  " << ARG_AVAILLANG_SHORT      << ", "  << ARG_AVAILLANG        << "  "            << QObject::tr("List the currently installed interface languages") << std::endl;
	std::cout << "  " << ARG_FONTINFO_SHORT       << ", "  << ARG_FONTINFO         << "        "      << QObject::tr("Show information on the console when fonts are being loaded") << std::endl;
	std::cout << "  " << ARG_NOSPLASH_SHORT       << ", "  << ARG_NOSPLASH         << "        "      << QObject::tr("Do not show the splashscreen on startup")     << std::endl;
	std::cout << "  " << ARG_VERSION_SHORT        << ",  " << ARG_VERSION          << "          "    << QObject::tr("Output version information and exit")       << std::endl;
	std::cout << "  " << ARG_SWAPDIABUTTONS_SHORT << ",  " << ARG_SWAPDIABUTTONS   << "    "    << QObject::tr("Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)")       << std::endl;
/*
	std::cout << "-file|-- name Open file 'name'" << std::endl;
	std::cout << "name          Open file 'name', the file name must not begin with '-'" << std::endl;
	std::cout << "QT specific options as -display ..." << std::endl;
*/
	std::cout << std::endl;
}

/*!
\fn void showAvailLangs()
\author Craig Bradney
\date Wed Nov 18 2004
\brief Instantiates the Language Manager and prints installed languages with brief instructions around
\param None
\retval None
*/

void ScribusQApp::showAvailLangs()
{
	std::cout << QObject::tr("Installed interface languages for Scribus are as follows:") << std::endl;
	std::cout << std::endl;

	LanguageManager langMgr;
	langMgr.init();
	langMgr.printInstalledList();

	std::cout << std::endl;
	std::cout << QObject::tr("To override the default language choice:") << std::endl;
	std::cout << QObject::tr("scribus -l xx or scribus --lang xx, where xx is the language of choice.") << std::endl;
}

void ScribusQApp::showVersion()
{
	std::cout << QObject::tr("Scribus Version ") << VERSION << std::endl;
}

void ScribusQApp::showHeader()
{
	std::cout << std::endl;
	std::cout << QObject::tr("Scribus, Open Source Desktop Publishing") << std::endl;
	std::cout << QObject::tr("---------------------------------------") << std::endl;
	std::cout << QObject::tr("Homepage:       http://www.scribus.net ") << std::endl;
	std::cout << QObject::tr("Documentation:  http://docs.scribus.net") << std::endl;
	std::cout << QObject::tr("Wiki:           http://wiki.scribus.net") << std::endl;
	std::cout << QObject::tr("Issues:         http://bugs.scribus.net") << std::endl;
	std::cout << std::endl;	
}

const bool ScribusQApp::usingGUI()
{
	return useGUI;
}

const bool ScribusQApp::reverseDialogButtons()
{
	if (swapDialogButtonOrder)
		return true;
	//Win32 - dont switch
	#if defined(_WIN32)
		return false;
	//Mac Aqua - switch
	#elif defined(Q_WS_MAC)
		return true;
	#else
	//Gnome - switch
	QString gnomesession= ::getenv("GNOME_DESKTOP_SESSION_ID");
	if (!gnomesession.isEmpty())
		return true;
	
	//KDE/KDE Aqua - dont switch
	//Best guess for now if we are running under KDE
	QString kdesession= ::getenv("KDE_FULL_SESSION");
	if (!kdesession.isEmpty())
		return false;
	#endif
	return false;
}

