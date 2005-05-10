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

#include "scribusapp.h"
#include "scribus.h"
#include "langmgr.h"
#include "prefsfile.h"
#include "scpaths.h"



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

// Qt wants -display not --display or -d
#define ARG_DISPLAY_QT "-display"


QString lang = "";
bool showSplash = true;
bool useGui = true;
QString file;

void showUsage();
void showAvailLangs();
int mainApp(int argc, char **argv);

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
		} else if ((arg == ARG_DISPLAY || arg==ARG_DISPLAY_SHORT || arg==ARG_DISPLAY_QT) && ++i < argc) {
			// allow setting of display, QT expect the option -display <display_name> so we discard the
			// last argument. FIXME: Qt only understands -display not --display and -d , we need to work
			// around this.
		} else if (strncmp(arg,"-psn_",4) == 0)
		{
			// Andreas Vox: Qt/Mac has -psn_blah flags that must be accepted.
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

	return mainApp(argc, argv);
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
int mainApp(int argc, char **argv)
{
	ScribusQApp app(argc, argv);
	int appRetVal=app.init(useGui, showSplash, lang, file);
	if (appRetVal==EXIT_FAILURE)
		return(EXIT_FAILURE);
	return app.exec();
}
