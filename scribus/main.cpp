/***************************************************************************
						main.cpp  -  description
							-------------------
	begin                : Fre Apr  6 21:47:55 CEST 2001
	copyright            : (C) 2001 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
	copyright            : (C) 2004 by Alessandro Rimoldi
	email                : http://ideale.ch/contact
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtranslator.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <cstdlib>

#include <iostream>

#define BASE_QM "scribus"

#include "scribus.h"
#include "langmgr.h"
#include "prefsfile.h"



#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#define ARG_VERSION "--version"
#define ARG_HELP "--help"
#define ARG_LANG "--lang"
#define ARG_AVAILLANG "--langs-available"
#define ARG_NOSPLASH "--no-splash"
#define ARG_NOGUI "--no-gui"
#define ARG_DISPLAY "--display"

#define ARG_VERSION_SHORT "-v"
#define ARG_HELP_SHORT "-h"
#define ARG_LANG_SHORT "-l"
#define ARG_AVAILLANG_SHORT "-la"
#define ARG_NOSPLASH_SHORT "-ns"
#define ARG_NOGUI_SHORT "-g"
#define ARG_DISPLAY_SHORT "-d"


QString lang = "";
bool showSplash = true;
bool useGui = true;
QString file;

void showUsage();
void showAvailLangs();
int mainGui(int argc, char **argv);
QStringList getLang(QString lang);
void installTranslators(QApplication *app, QStringList langs);


int main(int argc, char *argv[])
{
	QString arg = "";

	arg = argv[1];
	if (arg == ARG_VERSION || arg == ARG_VERSION_SHORT) {
		std::cout << "Scribus Version " << VERSION << std::endl;
		return 0;
	} else if (arg == ARG_HELP || arg == ARG_HELP_SHORT) {
		std::cout << std::endl;
		std::cout << "Scribus, Open Source Desktop Publishing" << std::endl;
		std::cout << "---------------------------------------" << std::endl;
		std::cout << "Homepage:       http://www.scribus.net " << std::endl;
		std::cout << "Documentation:  http://docs.scribus.net" << std::endl;
		std::cout << "Issues:         http://bugs.scribus.net" << std::endl;
		showUsage();
		return 0;
	} else if (arg == ARG_AVAILLANG || arg == ARG_AVAILLANG_SHORT) {
		showAvailLangs();
		return 0;
	}

	for(int i = 1; i < argc; i++) {
		arg = argv[i];
		if ((arg == ARG_LANG || arg == ARG_LANG_SHORT) && (++i < argc)) {
			lang = argv[i];
		} else if (arg == ARG_NOSPLASH || arg == ARG_NOSPLASH_SHORT) {
			showSplash = false;
		} else if (arg == ARG_NOGUI || arg == ARG_NOGUI_SHORT) {
			useGui = false;
		} else if ((arg == ARG_DISPLAY || arg==ARG_DISPLAY_SHORT) && ++i < argc) {
		// allow setting of display, QT expect the
		// option -display <display_name>
		} else {
			file = QFile::decodeName(argv[i]);
			if (!QFileInfo(file).exists()) {
				std::cout << std::endl;
				if (file.left(1) == "-" || file.left(2) == "--") {
					std::cout << "Invalid argument: " << file << std::endl;
				} else {
					std::cout << "File " << file << "does not exist, aborting." << std::endl;
				}
				showUsage();
				return 0;
			}
		}
	}

	if (useGui)
		return mainGui(argc, argv);
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

#define ARG_VERSION "--version"
#define ARG_HELP "--help"
#define ARG_LANG "--lang"
#define ARG_AVAILLANG "--langs-available"
#define ARG_NOSPLASH "--no-splash"
#define ARG_NOGUI "--no-gui"
#define ARG_DISPLAY "--display"

#define ARG_VERSION_SHORT "-v"
#define ARG_HELP_SHORT "-h"
#define ARG_LANG_SHORT "-l"
#define ARG_AVAILLANG_SHORT "-la"
#define ARG_NOSPLASH_SHORT "-ns"
#define ARG_NOGUI_SHORT "-g"
#define ARG_DISPLAY_SHORT "-d"


void showUsage()
{
	std::cout << std::endl;
	std::cout << "Usage: scribus [option ... ] [file]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  " << ARG_HELP_SHORT      << ",  " << ARG_HELP      << "             Print help (this message) and exit"     << std::endl;
	std::cout << "  " << ARG_LANG_SHORT      << ",  " << ARG_LANG      << "             Uses xx as shortcut for a language"     << std::endl;
	std::cout << "  " << ARG_AVAILLANG_SHORT << ", "  << ARG_AVAILLANG << "  Lists the currently installed interface languages" << std::endl;
	std::cout << "  " << ARG_NOSPLASH_SHORT  << ", "  << ARG_NOSPLASH  << "        Do not show the splashscreen on startup"     << std::endl;
	std::cout << "  " << ARG_VERSION_SHORT   << ",  " << ARG_VERSION   << "          Output version information and exit"       << std::endl;
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

void showAvailLangs()
{
	std::cout << "Installed Interface Languages for Scribus are as follows:" << std::endl;
	std::cout << std::endl;

	LanguageManager langMgr;
	langMgr.init();
	langMgr.printInstalledList();

	std::cout << std::endl;
	std::cout << "To override the default language choice:" << std::endl;
	std::cout << "scribus -l xx or scribus --lang xx, where xx is the language of choice." << std::endl;
}

/*!
\fn int mainGui(int argc, char **argv)
\author Franz Schmid
\author Alessandro Rimoldi
\date Mon Feb  9 14:07:46 CET 2004
\brief Launches the Gui
\param int Number of arguments passed to Scribus
\param char *argv list of the arguments passed to Scribus
\retval int Error code from the execution of Scribus
*/
int mainGui(int argc, char **argv)
{
	QApplication app(argc, argv);
	QStringList langs = getLang(QString(lang));

	ScribusApp *scribus = new ScribusApp();
	if (!scribus)
		exit(EXIT_FAILURE);
	if (!langs.isEmpty())
		installTranslators(&app, langs);

	app.processEvents();
	
	int scribusRetVal = scribus->initScribus(showSplash, lang);
	if (scribusRetVal == 1)
		exit(EXIT_FAILURE);
	app.setMainWidget(scribus);
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

	scribus->show();
	scribus->ShowSubs();
	if (file != "")
		scribus->LadeDoc(file);

	return app.exec();
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
QStringList getLang(QString lang)
{
	QStringList langs;

	// read the locales
	if (lang != "")
		langs.push_back(lang);
	
	//add in user preferences lang, only overridden by lang command line option
	QString Pff = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus");
	QString PrefsPfad;
	QFileInfo Pffi = QFileInfo(Pff);
	if (Pffi.exists())
	{
		if (Pffi.isDir())
			PrefsPfad = Pff;
		else
			PrefsPfad = QDir::homeDirPath();
			
		PrefsFile* prefsFile = new PrefsFile(QDir::convertSeparators(PrefsPfad + "/prefs.xml"));
		if (prefsFile) {
			PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
			if (userprefsContext) {
				QString prefslang = userprefsContext->get("gui_language","");
				if (prefslang!="")
					langs.push_back(prefslang);
			}
		}
	}
	#ifdef linux
	if ((lang = ::getenv("LC_ALL")) != "")
		langs.push_back(lang);
	if ((lang = ::getenv("LC_MESSAGES")) != "")
		langs.push_back(lang);
	if ((lang = ::getenv("LANG")) != "")
		langs.push_back(lang);
	#endif
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
void installTranslators(QApplication *app, QStringList langs)
{
	QTranslator *trans= new QTranslator(0);
	QString path = LIBDIR;
	path += BASE_QM;

	bool loaded = false;
	for (QStringList::Iterator it = langs.begin(); it != langs.end() && !loaded; ++it) {
		if ((*it).left(5) == "en_GB")
			lang = "en_GB";
		else
			lang = (*it).left(2);

		if (lang == "en")
			break;
		else if (loaded = trans->load(QString(path + '.' + lang), "."))
			loaded = true;
	}

	if (loaded)
		app->installTranslator(trans);

	path = PLUGINDIR;
	QDir dir(path , "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if (dir.exists() && (dir.count() != 0)) {
		for (uint i = 0; i < dir.count(); ++i) {
			QFileInfo file(path + dir[i]);
			if ((file.extension(false).lower() == "qm")
			&& (file.extension(true).lower().left(2) == lang)) {
				trans = new QTranslator(0);
				trans->load(QString(path + dir[i]), ".");
				app->installTranslator(trans);
			}
		}
	}
}
