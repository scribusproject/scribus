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

#include <qfont.h>
#include <qstring.h>
#include <qtranslator.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <cstdlib>

#include "scribusapp.h"
#include "scribus.h"
#include "scpaths.h"
#include "prefsfile.h"

ScribusQApp::ScribusQApp ( int & argc, char ** argv ) : QApplication(argc, argv)
{
}

int ScribusQApp::init(bool /*useGUI*/, bool showSplash, QString lang, QString file)
{
	QStringList langs = getLang(QString(lang));

	scribus = new ScribusApp();
	if (!scribus)
		exit(EXIT_FAILURE);
	if (!langs.isEmpty())
		installTranslators(langs);

	processEvents();
	
	int scribusRetVal = scribus->initScribus(showSplash, lang);
	if (scribusRetVal == 1)
		return(EXIT_FAILURE);
	scribus->initCrashHandler();
	setMainWidget(scribus);
	connect(this, SIGNAL(lastWindowClosed()), this, SLOT(quit()));

	scribus->show();
	scribus->ShowSubs();
	if (file != "")
		scribus->loadDoc(file);
	return 0;
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
	if (lang != "")
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
					if (prefslang!="")
						langs.push_back(prefslang);
				}
			}
		}
	}

	if ((lang = ::getenv("LC_ALL")) != "")
		langs.push_back(lang);
	if ((lang = ::getenv("LC_MESSAGES")) != "")
		langs.push_back(lang);
	if ((lang = ::getenv("LANG")) != "")
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
	if (newGUILang=="")
		newLangs.append("en");
	else
		newLangs.append(newGUILang);
	installTranslators(newLangs);
}
