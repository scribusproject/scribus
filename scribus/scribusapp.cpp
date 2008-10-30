/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : May 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
	copyright            : (C) 2001 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScMW program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#include <iostream>
#include <cstdlib>

#include <QString>
#include <QFont>
#include <QTranslator>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include <QLocale>
#include <QTextStream>

#include "scribusapp.h"
#include "scribuscore.h"
#include "scpaths.h"
#include "prefsfile.h"
#include "langmgr.h"
#include "prefsmanager.h"
#include "commonstrings.h"
#include "upgradechecker.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#define ARG_VERSION "--version"
#define ARG_HELP "--help"
#define ARG_LANG "--lang"
#define ARG_AVAILLANG "--langs-available"
#define ARG_NOSPLASH "--no-splash"
#define ARG_NEVERSPLASH "--never-splash"
#define ARG_NOGUI "--no-gui"
#define ARG_DISPLAY "--display"
#define ARG_FONTINFO "--font-info"
#define ARG_PROFILEINFO "--profile-info"
#define ARG_SWAPDIABUTTONS "--swap-buttons"
#define ARG_PREFS "--prefs"
#define ARG_UPGRADECHECK "--upgradecheck"

#define ARG_VERSION_SHORT "-v"
#define ARG_HELP_SHORT "-h"
#define ARG_LANG_SHORT "-l"
#define ARG_AVAILLANG_SHORT "-la"
#define ARG_NOSPLASH_SHORT "-ns"
#define ARG_NEVERSPLASH_SHORT "-nns"
#define ARG_NOGUI_SHORT "-g"
#define ARG_DISPLAY_SHORT "-d"
#define ARG_FONTINFO_SHORT "-fi"
#define ARG_PROFILEINFO_SHORT "-pi"
#define ARG_SWAPDIABUTTONS_SHORT "-sb"
#define ARG_PREFS_SHORT "-pr"
#define ARG_UPGRADECHECK_SHORT "-u"

// Qt wants -display not --display or -d
#define ARG_DISPLAY_QT "-display"

// Windows specific options, allows to display a console windows
extern const char ARG_CONSOLE[] =  "--console";
extern const char ARG_CONSOLE_SHORT[] = "-cl";

extern ScribusQApp* ScQApp;
extern ScribusCore* ScCore;

bool ScribusQApp::useGUI=false;

ScribusQApp::ScribusQApp( int & argc, char ** argv ) : QApplication(argc, argv),
	lang(""),
	GUILang("")
{
	ScQApp=this;
	ScCore=NULL;
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
	showSplash=!neverSplashExists();
	QString arg("");
	bool usage=false;
	bool header=false;
	bool availlangs=false;
	bool version=false;
	bool runUpgradeCheck=false;
	showFontInfo=false;
	showProfileInfo=false;
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
		} else if (arg == ARG_UPGRADECHECK || arg == ARG_UPGRADECHECK_SHORT) {
			header=true;
			runUpgradeCheck=true;
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
	if (runUpgradeCheck)
	{
		UpgradeChecker uc;
		uc.fetch();
	}
	//Dont run the GUI init process called from main.cpp, and return
	if (!header)
		useGUI=true;
	else
		return;
	//We are going to run something other than command line help
	for(int i = 1; i < argc(); i++) {
		arg = argv()[i];

		if ((arg == ARG_LANG || arg == ARG_LANG_SHORT) && (++i < argc())) {
			continue;
		} else if ( arg == ARG_CONSOLE || arg == ARG_CONSOLE_SHORT ) {
			continue;
		} else if (arg == ARG_NOSPLASH || arg == ARG_NOSPLASH_SHORT) {
			showSplash = false;
		}
		else if (arg == ARG_NEVERSPLASH || arg == ARG_NEVERSPLASH_SHORT) {
			showSplash = false;
			neverSplash(true);
		} else if (arg == ARG_NOGUI || arg == ARG_NOGUI_SHORT) {
			useGUI=false;
		} else if (arg == ARG_FONTINFO || arg == ARG_FONTINFO_SHORT) {
			showFontInfo=true;
		} else if (arg == ARG_PROFILEINFO || arg == ARG_PROFILEINFO_SHORT) {
			showProfileInfo=true;
		} else if (arg == ARG_SWAPDIABUTTONS || arg == ARG_SWAPDIABUTTONS_SHORT) {
			swapDialogButtonOrder=true;
		} else if ((arg == ARG_DISPLAY || arg==ARG_DISPLAY_SHORT || arg==ARG_DISPLAY_QT) && ++i < argc()) {
			// allow setting of display, QT expect the option -display <display_name> so we discard the
			// last argument. FIXME: Qt only understands -display not --display and -d , we need to work
			// around this.
		} else if (arg == ARG_PREFS || arg == ARG_PREFS_SHORT) {
			prefsUserFile = QFile::decodeName(argv()[i + 1]);
			if (!QFileInfo(prefsUserFile).exists()) {
				showHeader();
				if (file.left(1) == "-" || file.left(2) == "--") {
					std::cout << tr("Invalid argument: ").toLocal8Bit().data() << file.toLocal8Bit().data() << std::endl;
				} else {
					std::cout << tr("File %1 does not exist, aborting.").arg(file).toLocal8Bit().data() << std::endl;
				}
				showUsage();
				useGUI=false;
				return;
			} else {
				++i;
			}
		} else if (strncmp(arg.toLocal8Bit().data(),"-psn_",4) == 0)
		{
			// Andreas Vox: Qt/Mac has -psn_blah flags that must be accepted.
		} else {
			file = QFile::decodeName(argv()[i]);
			if (!QFileInfo(file).exists()) {
				showHeader();
				if (file.left(1) == "-" || file.left(2) == "--") {
					std::cout << tr("Invalid argument: %1").arg(file).toLocal8Bit().data() << std::endl;
				} else {
					std::cout << tr("File %1 does not exist, aborting.").arg(file).toLocal8Bit().data() << std::endl;
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
	m_ScCore=new ScribusCore();
	Q_CHECK_PTR(m_ScCore);
	if (!m_ScCore)
		return EXIT_FAILURE;
	ScCore=m_ScCore;
	processEvents();
	ScCore->init(useGUI, swapDialogButtonOrder, file);
	int retVal=EXIT_SUCCESS;
	if (useGUI)
		retVal=ScCore->startGUI(showSplash, showFontInfo, showProfileInfo, lang, prefsUserFile);
	return retVal;
}

QStringList ScribusQApp::getLang(QString lang)
{
	QStringList langs;

	// read the locales
	if (!lang.isEmpty())
		langs.push_back(lang);

	//add in user preferences lang, only overridden by lang command line option
	QString Pff = QDir::convertSeparators(ScPaths::getApplicationDataDir());
	QFileInfo Pffi = QFileInfo(Pff);
	if (Pffi.exists())
	{
		QString PrefsPfad;
		if (Pffi.isDir())
			PrefsPfad = Pff;
		else
			PrefsPfad = QDir::homePath();
		QString prefsXMLFile=QDir::convertSeparators(PrefsPfad + "/prefs135.xml");
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
			delete prefsFile;
		}
	}

	if (!(lang = ::getenv("LC_ALL")).isEmpty())
		langs.push_back(lang);
	if (!(lang = ::getenv("LC_MESSAGES")).isEmpty())
		langs.push_back(lang);
	if (!(lang = ::getenv("LANG")).isEmpty())
		langs.push_back(lang);

#if defined(_WIN32)
	wchar_t out[256];
	QString language, sublanguage;
	LCID lcIdo = GetUserDefaultLCID();
	WORD sortId = SORTIDFROMLCID(lcIdo);
	LANGID langId = GetUserDefaultUILanguage();
	LCID lcIdn = MAKELCID(langId, sortId);
	if ( GetLocaleInfoW(lcIdn, LOCALE_SISO639LANGNAME , out, 255) )
	{
		language = QString::fromUtf16( (ushort*)out );
		if ( GetLocaleInfoW(lcIdn, LOCALE_SISO3166CTRYNAME, out, 255) )
		{
			sublanguage = QString::fromUtf16( (ushort*)out ).toLower();
			lang = language;
			if ( sublanguage != language && !sublanguage.isEmpty() )
				lang += "_" + sublanguage.toUpper();
			langs.push_back(lang);
		}
	}
#endif

	langs.push_back(QString(QLocale::system().name()));

	// remove duplicate entries...
	QStringList::Iterator it = langs.end();
	while (it != langs.begin())
	{
		--it;
		if (langs.count(*it) > 1)
			it = langs.erase(it);
	}

	return langs;
}

void ScribusQApp::installTranslators(const QStringList & langs)
{
	static QTranslator *trans = 0;

	if ( trans )
	{
		removeTranslator( trans );
		delete trans;
	}
	trans = new QTranslator(0);
	QString path(ScPaths::instance().translationDir());
	path += "scribus";

	bool loaded = false;
	QString lang;
	for (QStringList::const_iterator it = langs.constBegin(); it != langs.constEnd() && !loaded; ++it) 
	{
		lang=(*it).left(5);
		if (lang == "en")
		{
			GUILang=lang;
			break;
		}
		else if (trans->load(QString(path + '.' + lang), "."))
			loaded = true;
	}
	if (loaded)
	{
		installTranslator(trans);
		GUILang=lang;
	}
	else if (lang == "en")
		GUILang=lang;
	/* CB TODO, currently disabled, because its broken broken broken
	path = ScPaths::instance().pluginDir();
	QDir dir(path , "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if (dir.exists() && (dir.count() != 0)) {
		for (uint i = 0; i < dir.count(); ++i) {
			QFileInfo file(path + dir[i]);
			if ((file.extension(false).toLower() == "qm")
			&& (file.extension(true).toLower().left(5) == lang)) {
				trans = new QTranslator(0);
				trans->load(QString(path + dir[i]), ".");
				installTranslator(trans);
			}
		}
	}*/
}

void ScribusQApp::changeGUILanguage(const QString & newGUILang)
{
	QStringList newLangs;
	if (newGUILang.isEmpty())
	{
		newLangs = getLang(QString());
		newLangs.append("en");
	}
	else
		newLangs.append(newGUILang);
	if (newLangs[0] != GUILang)
		installTranslators(newLangs);
}

/*! \brief Format an arguments line for printing
Helper procedure */
static void printArgLine(QTextStream & ts, const char * smallArg,
						  const char* fullArg, const QString desc)
{
	const char* lineformat = "  %1, %2 %3";
	const int saw = 4;   // Short argument width
	const int aw = -18;   // Argument width (negative is left aligned)
	QString line = QString(lineformat)
		.arg(smallArg, saw)
		.arg(fullArg, aw)
		.arg(desc);
	ts << line;
	endl(ts);
}

void ScribusQApp::showUsage()
{
	QFile f;
	f.open(stderr, QIODevice::WriteOnly);
	QTextStream ts(&f);
	ts << tr("Usage: scribus [option ... ] [file]") ; endl(ts);
	ts << tr("Options:") ; endl(ts);
	printArgLine(ts, ARG_FONTINFO_SHORT, ARG_FONTINFO, tr("Show information on the console when fonts are being loaded") );
	printArgLine(ts, ARG_HELP_SHORT, ARG_HELP, tr("Print help (this message) and exit") );
	printArgLine(ts, ARG_LANG_SHORT, ARG_LANG, tr("Uses xx as shortcut for a language, eg `en' or `de'") );
	printArgLine(ts, ARG_AVAILLANG_SHORT, ARG_AVAILLANG, tr("List the currently installed interface languages") );
	printArgLine(ts, ARG_NOSPLASH_SHORT, ARG_NOSPLASH, tr("Do not show the splashscreen on startup") );
	printArgLine(ts, ARG_NEVERSPLASH_SHORT, ARG_NEVERSPLASH, tr("Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.") );
	printArgLine(ts, ARG_PREFS_SHORT, QString(QString(ARG_PREFS) + QString(" ") + tr("filename")).toLocal8Bit().constData(), tr("Use filename as path for user given preferences") );
	printArgLine(ts, ARG_PROFILEINFO_SHORT, ARG_PROFILEINFO, tr("Show location ICC profile information on console while starting") );
	printArgLine(ts, ARG_SWAPDIABUTTONS_SHORT, ARG_SWAPDIABUTTONS, tr("Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)") );
	printArgLine(ts, ARG_UPGRADECHECK_SHORT, ARG_UPGRADECHECK, tr("Download a file from the Scribus website and show the latest available version.") );
	printArgLine(ts, ARG_VERSION_SHORT, ARG_VERSION, tr("Output version information and exit") );
	
	
#if defined(_WIN32) && !defined(_CONSOLE)
	printArgLine(ts, ARG_CONSOLE_SHORT, ARG_CONSOLE, tr("Display a console window") );
#endif
/* Delete me?
	std::cout << "-file|-- name Open file 'name'" ; endl(ts);
	std::cout << "name          Open file 'name', the file name must not begin with '-'" ; endl(ts);
	std::cout << "QT specific options as -display ..." ; endl(ts);
*/
	endl(ts);
}

void ScribusQApp::showAvailLangs()
{
	QFile f;
	f.open(stderr, QIODevice::WriteOnly);
	QTextStream ts(&f);
	ts << tr("Installed interface languages for Scribus are as follows:"); endl(ts);
	endl(ts);

// 	LanguageManager langMgr;
// 	langMgr.init();
	LanguageManager::instance()->printInstalledList();

	endl(ts);
	ts << tr("To override the default language choice:"); endl(ts);
	ts << tr("scribus -l xx or scribus --lang xx, where xx is the language of choice."); endl(ts);
}

void ScribusQApp::showVersion()
{
	std::cout << tr("Scribus Version").toLocal8Bit().data() << " " << VERSION << std::endl;
}

void ScribusQApp::showHeader()
{
	QFile f;
	f.open(stderr, QIODevice::WriteOnly);
	QTextStream ts(&f);
	endl(ts);
	QString heading( tr("Scribus, Open Source Desktop Publishing") );
	// Build a separator of ----s the same width as the heading
	QString separator = QString("").rightJustified(heading.length(),'-');
	// Then output the heading, separator, and docs/www/etc info in an aligned table
	const int urlwidth = 23;
	const int descwidth = -(heading.length() - urlwidth - 1);
	ts << heading; endl(ts);
	ts << separator; endl(ts);
	ts << QString("%1 %2").arg( tr("Homepage")+":",      descwidth).arg("http://www.scribus.net" ); endl(ts);
	ts << QString("%1 %2").arg( tr("Documentation")+":", descwidth).arg("http://docs.scribus.net"); endl(ts);
	ts << QString("%1 %2").arg( tr("Wiki")+":",          descwidth).arg("http://wiki.scribus.net"); endl(ts);
	ts << QString("%1 %2").arg( tr("Issues")+":",        descwidth).arg("http://bugs.scribus.net"); endl(ts);
	endl(ts);
}

void ScribusQApp::neverSplash(bool splashOff)
{
	QString prefsDir = ScPaths::getApplicationDataDir();
	QFile ns(prefsDir + ".neversplash");
	if (splashOff)
	{
		if (QFileInfo(QDir::homePath()).exists())
		{
			QDir prefsDirectory(prefsDir);
			if (!QFileInfo(prefsDir).exists())
				prefsDirectory.mkdir(prefsDir);
			if (!ns.exists() && ns.open(QIODevice::WriteOnly))
				ns.close();
		}
	}
	else
	{
		if (neverSplashExists())
			ns.remove();
	}
}

bool ScribusQApp::neverSplashExists()
{
	return QFileInfo(ScPaths::getApplicationDataDir() + ".neversplash").exists();
}
