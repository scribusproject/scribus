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


#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QLibraryInfo>
#include <QLocale>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QTranslator>

#include "scribusapp.h"

#include "commonstrings.h"
#include "downloadmanager/scdlmgr.h"
#include "iconmanager.h"
#include "langmgr.h"
#include "localemgr.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "upgradechecker.h"
#include "util.h"

#ifdef WITH_TESTS
#include "tests/runtests.h"
#endif

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
#define ARG_PREFS "--prefs"
#define ARG_UPGRADECHECK "--upgradecheck"
#define ARG_TESTS "--tests"
#define ARG_PYTHONSCRIPT "--python-script"
#define CMD_OPTIONS_END "--"

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
#define ARG_PREFS_SHORT "-pr"
#define ARG_UPGRADECHECK_SHORT "-u"
#define ARG_TESTS_SHORT "-T"
#define ARG_PYTHONSCRIPT_SHORT "-py"

// Qt wants -display not --display or -d
#define ARG_DISPLAY_QT "-display"

// Windows specific options, allows to display a console windows
extern const char ARG_CONSOLE[] =  "--console";
extern const char ARG_CONSOLE_SHORT[] = "-cl";

extern ScribusQApp* ScQApp;
extern ScribusCore* ScCore;

bool ScribusQApp::useGUI=false;

ScribusQApp::ScribusQApp( int & argc, char ** argv ) : QApplication(argc, argv),
	m_lang(""),
	m_GUILang("en_GB")
{
	ScQApp = this;
	ScCore = 0;
	m_scDLMgr = 0;
	m_ScCore = NULL;
	initDLMgr();
	setAttribute(Qt::AA_UseHighDpiPixmaps, true);
}

ScribusQApp::~ScribusQApp()
{
	if (m_ScCore)
		delete m_ScCore;
	if (m_scDLMgr)
		delete m_scDLMgr;
	PrefsManager::deleteInstance();
	LocaleManager::deleteInstance();
	LanguageManager::deleteInstance();
	IconManager::deleteInstance();
}

void ScribusQApp::initLang()
{
	QStringList langs = getLang(QString(m_lang));

	if (!langs.isEmpty())
		installTranslators(langs);
}

void ScribusQApp::initDLMgr()
{
	m_scDLMgr = new ScDLManager(this);
	//connect(m_scDLMgr, SIGNAL(fileReceived(const QString&)), SLOT(downloadComplete(const QString&)));
}

void ScribusQApp::parseCommandLine()
{
	m_showSplash=!neverSplashExists();
	QString arg("");
	bool usage=false;
	bool header=false;
	bool availlangs=false;
	bool version=false;
	bool runUpgradeCheck=false;
#ifdef WITH_TESTS
	bool runtests = false;
	char** testargsv;
	int testargsc;
#endif
	m_showFontInfo=false;
	m_showProfileInfo=false;	
	bool neversplash = false;

	//Parse for command line options
	// Qt5 port: do this in a Qt compatible manner
	QStringList args = arguments();
	int argsc = args.count();

	useGUI = true;
	int argi = 1;
	for( ; argi < argsc; argi++)
	{ //handle options (not positional parameters)
		arg = args[argi];
		if (arg == ARG_PYTHONSCRIPT || arg == ARG_PYTHONSCRIPT_SHORT)
		{
			if (argi+1 == argsc)
			{
				std::cout << tr("Option %1 requires an argument.").arg(arg).toLocal8Bit().data() << std::endl;
				std::exit(EXIT_FAILURE);
			}
			pythonScript = QFile::decodeName(args[argi + 1].toLocal8Bit());
			if (!QFileInfo::exists(pythonScript))
			{
				std::cout << tr("Python script %1 does not exist, aborting.").arg(pythonScript).toLocal8Bit().data() << std::endl;
				std::exit(EXIT_FAILURE);
			}
			++argi;

			while (++argi < argsc && (args[argi] != CMD_OPTIONS_END))
			{
				pythonScriptArgs.append(args[argi]); // script argument
			}
			// We reached end of all arguments or CMD_OPTIONS_END marker. Stop parsing options
			if (argi < argsc)
			{
				argi++; // skip CMD_OPTIONS_END
			}
			break;
		}
		else if ((arg == ARG_LANG || arg == ARG_LANG_SHORT))
		{
			if  (++argi < argsc)
				m_lang = args[argi];
			else
			{
				std::cout << tr("Option %1 requires an argument.").arg(arg).toLocal8Bit().data() << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}
		else if (arg == ARG_VERSION || arg == ARG_VERSION_SHORT)
		{
			header=true;
			version=true;
		} else if (arg == ARG_HELP || arg == ARG_HELP_SHORT)
		{
			header=true;
			usage=true;
		}
#ifdef WITH_TESTS
		else if (arg == ARG_TESTS || arg == ARG_TESTS_SHORT)
		{
			header=true;
			runtests=true;
			testargsc = argc() - argi;
			testargsv = argv() + argi;
			break;
		}
#endif
		else if (arg == ARG_AVAILLANG || arg == ARG_AVAILLANG_SHORT)
		{
			header=true;
			availlangs=true;
		} else if (arg == ARG_UPGRADECHECK || arg == ARG_UPGRADECHECK_SHORT)
		{
			header=true;
			runUpgradeCheck=true;
		}
		else if ( arg == ARG_CONSOLE || arg == ARG_CONSOLE_SHORT )
		{
			continue;
		} else if (arg == ARG_NOSPLASH || arg == ARG_NOSPLASH_SHORT)
		{
			m_showSplash = false;
		}
		else if (arg == ARG_NEVERSPLASH || arg == ARG_NEVERSPLASH_SHORT)
		{
			m_showSplash = false;
			neversplash = true;
		}
		else if (arg == ARG_NOGUI || arg == ARG_NOGUI_SHORT)
		{
			useGUI=false;
		}
		else if (arg == ARG_FONTINFO || arg == ARG_FONTINFO_SHORT)
		{
			m_showFontInfo=true;
		}
		else if (arg == ARG_PROFILEINFO || arg == ARG_PROFILEINFO_SHORT)
		{
			m_showProfileInfo=true;
		}
		else if ((arg == ARG_DISPLAY || arg==ARG_DISPLAY_SHORT || arg==ARG_DISPLAY_QT) && ++argi < argsc)
		{
			// allow setting of display, QT expect the option -display <display_name> so we discard the
			// last argument. FIXME: Qt only understands -display not --display and -d , we need to work
			// around this.
		}
		else if (arg == ARG_PREFS || arg == ARG_PREFS_SHORT)
		{
			if (argi+1 == argsc)
			{
				std::cout << tr("Option %1 requires an argument.").arg(arg).toLocal8Bit().data() << std::endl;
				std::exit(EXIT_FAILURE);
			}
			m_prefsUserDir = QFile::decodeName(args[argi + 1].toLocal8Bit());
			if (!m_prefsUserDir.endsWith("/"))
				m_prefsUserDir.append('/');
			if (!QDir(m_prefsUserDir).exists())
			{
				std::cout << tr("Preferences directory %1 does not exist, aborting.").arg(m_prefsUserDir).toLocal8Bit().data() << std::endl;
				std::exit(EXIT_FAILURE);
			} else {
				++argi;
			}
		}
		else if (strncmp(arg.toLocal8Bit().data(),"-psn_",4) == 0)
		{
			// Andreas Vox: Qt/Mac has -psn_blah flags that must be accepted.
		}
		else if (arg == CMD_OPTIONS_END)
		{ //double dash, indicates end of command line options, see http://unix.stackexchange.com/questions/11376/what-does-double-dash-mean-also-known-as-bare-double-dash
			argi++;
			break;
		}
		else
		{ //argument is not a known option, but either positional parameter or invalid.
			if (arg.left(1) == "-")
			{
				std::cout << tr("Invalid argument: %1").arg(arg).toLocal8Bit().data() << std::endl;
				std::exit(EXIT_FAILURE);
			}
			m_fileName = QFile::decodeName(args[argi].toLocal8Bit());
			if (!QFileInfo::exists(m_fileName))
			{
				std::cout << tr("File %1 does not exist, aborting.").arg(m_fileName).toLocal8Bit().data() << std::endl;
				std::exit(EXIT_FAILURE);
			}
			else
			{
				m_filesToLoad.append(m_fileName);
			}
		}
	}
	// parse for remaining (positional) arguments, if any
	for ( ; argi<argsc; argi++)
	{
		m_fileName = QFile::decodeName(args[argi].toLocal8Bit());
		if (!QFileInfo::exists(m_fileName))
		{
			std::cout << tr("File %1 does not exist, aborting.").arg(m_fileName).toLocal8Bit().data() << std::endl;
			std::exit(EXIT_FAILURE);
		}
		else
		{
			m_filesToLoad.append(m_fileName);
		}
	}
	//Init translations
	initLang();
	
	//Show command line info
	if (header)
	{
		useGUI = false;
		showHeader();
	}
	if (version)
		showVersion();
	if (availlangs)
		showAvailLangs();
	if (usage)
		showUsage();
#ifdef WITH_TESTS
	if (runtests)
		RunTests::runTests(testargsc, testargsv);
#endif
	if (runUpgradeCheck)
	{
		UpgradeChecker uc;
		uc.fetch();
	}
	//Don't run the GUI init process called from main.cpp, and return
	if (header)
		std::exit(EXIT_SUCCESS);
	//proceed
	if(neversplash)
		neverSplash(true);
	
}

int ScribusQApp::init()
{
	m_ScCore=new ScribusCore();
	Q_CHECK_PTR(m_ScCore);
	if (!m_ScCore)
		return EXIT_FAILURE;
	ScCore=m_ScCore;
	processEvents();
	ScCore->init(useGUI, m_filesToLoad);
	int retVal=EXIT_SUCCESS;
	/* TODO:
	 * When Scribus is truly able to run without GUI
	 * we should uncomment if (useGUI)
	 * and delete if (true)
	 */
	// if (useGUI)
	if (true)
		retVal=ScCore->startGUI(m_showSplash, m_showFontInfo, m_showProfileInfo, m_lang);

	// A hook for plugins and scripts to trigger on. Some plugins and scripts
	// require the app to be fully set up (in particular, the main window to be
	// built and shown) before running their setup.
	emit appStarted();

	return retVal;
}

QStringList ScribusQApp::getLang(QString lang)
{
	QStringList langs;

	// read the locales
	if (!lang.isEmpty())
		langs.append(lang);

	//add in user preferences lang, only overridden by lang command line option
	QString Pff = QDir::toNativeSeparators(ScPaths::preferencesDir());
	QFileInfo Pffi(Pff);
	if (Pffi.exists())
	{
		QString PrefsPfad;
		if (Pffi.isDir())
			PrefsPfad = Pff;
		else
			PrefsPfad = ScPaths::preferencesDir();
		QString prefsXMLFile=QDir::toNativeSeparators(PrefsPfad + "prefs150.xml");
		QFileInfo infoPrefsFile(prefsXMLFile);
		if (infoPrefsFile.exists())
		{
			PrefsFile* prefsFile = new PrefsFile(prefsXMLFile);
			if (prefsFile)
			{
				PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
				if (userprefsContext)
				{
					QString prefslang = userprefsContext->get("gui_language","");
					prefslang = cleanupLang(prefslang);
					if (!prefslang.isEmpty())
						langs.append(prefslang);
				}
			}
			delete prefsFile;
		}
	}

	if (!(lang = ::getenv("LANG")).isEmpty())
	{
		lang = cleanupLang(lang);
		if (lang=="C")
			lang="en";
		if (!lang.isEmpty())
			langs.append(lang);
	}
	if (!(lang = ::getenv("LC_MESSAGES")).isEmpty())
	{
		lang = cleanupLang(lang);
		if (lang=="C")
			lang="en";
		if (!lang.isEmpty())
			langs.append(lang);
	}
	if (!(lang = ::getenv("LC_ALL")).isEmpty())
	{
		lang = cleanupLang(lang);
		if (lang=="C")
			lang="en";
		if (!lang.isEmpty())
			langs.append(lang);
	}

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
			langs.append(lang);
		}
	}
#endif

	langs.append(QLocale::system().name());

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
	static QTranslator *transQt = 0;
	static QTranslator *trans = 0;

	if (transQt)
	{
		removeTranslator( transQt );
		delete transQt;
		transQt=0;
	}
	if (trans)
	{
		removeTranslator( trans );
		delete trans;
		trans=0;
	}

	transQt = new QTranslator(0);
	trans = new QTranslator(0);
	QString path(ScPaths::instance().translationDir());

	bool loadedQt = false;
	bool loadedScribus = false;
	QString lang;
	for (QStringList::const_iterator it = langs.constBegin(); it != langs.constEnd() && !loadedScribus; ++it)
	{
		lang=(*it);
		if (lang == "en")
		{
			m_GUILang=lang;
			break;
		}
		else
		{
			//CB: This might need adjusting for qm files distribution locations
			if (transQt->load("qt_" + lang,	QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
				loadedQt = true;
			if (trans->load(QString("scribus." + lang), path))
				loadedScribus = true;
			if (!loadedScribus)
			{
				QString altLang(LanguageManager::instance()->getAlternativeAbbrevfromAbbrev(lang));
				if (!altLang.isEmpty())
					if (trans->load(QString("scribus." + altLang), path))
						loadedScribus = true;
			}
		}
	}
	if (loadedQt)
		installTranslator(transQt);
	if (loadedScribus)
	{
		installTranslator(trans);
		m_GUILang=lang;
	}
	else if (lang == "en")
		m_GUILang=lang;
	/* CB TODO, currently disabled, because its broken broken broken
	path = ScPaths::instance().pluginDir();
	QDir dir(path , "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if (dir.exists() && (dir.count() != 0))
	{
		for (uint i = 0; i < dir.count(); ++i)
		{
			QFileInfo file(path + dir[i]);
			if ((file.extension(false).toLower() == "qm")
			&& (file.extension(true).toLower().left(5) == lang))
			{
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
	if (newLangs[0] != m_GUILang)
		installTranslators(newLangs);
}

/*! \brief Format an arguments line for printing
Helper procedure */
static void printArgLine(QTextStream & ts, const char * smallArg, const char* fullArg, const QString desc)
{
	ts << QString("     %1 %2 %3").arg(QString("%1,").arg(smallArg), -5).arg(fullArg, -32).arg(desc);
	endl(ts);
}

void ScribusQApp::showUsage()
{
	QFile f;
	f.open(stderr, QIODevice::WriteOnly);
	QTextStream ts(&f);
	ts << tr("Usage: scribus [options] [files]") ; endl(ts); endl(ts);
	ts << tr("Options:") ; endl(ts);
	printArgLine(ts, ARG_FONTINFO_SHORT, ARG_FONTINFO, tr("Show information on the console when fonts are being loaded") );
	printArgLine(ts, ARG_HELP_SHORT, ARG_HELP, tr("Print help (this message) and exit") );
	printArgLine(ts, ARG_LANG_SHORT, ARG_LANG, tr("Uses xx as shortcut for a language, eg `en' or `de'") );
	printArgLine(ts, ARG_AVAILLANG_SHORT, ARG_AVAILLANG, tr("List the currently installed interface languages") );
	printArgLine(ts, ARG_NOSPLASH_SHORT, ARG_NOSPLASH, tr("Do not show the splashscreen on startup") );
	printArgLine(ts, ARG_NEVERSPLASH_SHORT, ARG_NEVERSPLASH, tr("Stop showing the splashscreen on startup. Writes an empty file called .neversplash in ~/.config/scribus") );
	printArgLine(ts, ARG_PREFS_SHORT, qPrintable(QString("%1 <%2>").arg(ARG_PREFS).arg(tr("path"))), tr("Use path for user given preferences location") );
	printArgLine(ts, ARG_PROFILEINFO_SHORT, ARG_PROFILEINFO, tr("Show location of ICC profile information on console while starting") );
	printArgLine(ts, ARG_UPGRADECHECK_SHORT, ARG_UPGRADECHECK, tr("Download a file from the Scribus website and show the latest available version") );
	printArgLine(ts, ARG_VERSION_SHORT, ARG_VERSION, tr("Output version information and exit") );
	printArgLine(ts, ARG_PYTHONSCRIPT_SHORT, qPrintable(QString("%1 <%2> [%3] ").arg(ARG_PYTHONSCRIPT).arg(tr("script")).arg(tr("arguments ..."))), tr("Run script in Python [with optional arguments]. This option must be last option used") );
	printArgLine(ts, ARG_NOGUI_SHORT, ARG_NOGUI, tr("Do not start GUI") );
	ts << (QString("     %1").arg(CMD_OPTIONS_END,-39)) << tr("Explicit end of command line options"); endl(ts);
 	
	
#if defined(_WIN32) && !defined(_CONSOLE)
	printArgLine(ts, ARG_CONSOLE_SHORT, ARG_CONSOLE, tr("Display a console window") );
#endif

#if WITH_TESTS
	printArgLine(ts, ARG_TESTS_SHORT, ARG_TESTS, tr("Run unit tests and exit") );
#endif

/* Delete me?
	std::cout << "-file|-- name Open file 'name'" ; endl(ts);
	std::cout << "name          Open file 'name', the file name must not begin with '-'" ; endl(ts);
	std::cout << "QT specific options as -display ..." ; endl(ts);
*/
	endl(ts);
	f.close();
}

void ScribusQApp::showAvailLangs()
{
	QFile f;
	f.open(stderr, QIODevice::WriteOnly);
	QTextStream ts(&f);
	ts << tr("Installed interface languages for Scribus are as follows:"); endl(ts);
	endl(ts);

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
	QString prefsDir = ScPaths::preferencesDir(true);
	QFile nsFile(prefsDir + ".neversplash");
	if (splashOff)
	{
		if (!nsFile.exists() && nsFile.open(QIODevice::WriteOnly))
			nsFile.close();
	}
	else
	{
		if (neverSplashExists())
			nsFile.remove();
	}
}

bool ScribusQApp::neverSplashExists()
{
	return QFileInfo::exists(ScPaths::preferencesDir() + ".neversplash");
}

void ScribusQApp::downloadComplete(const QString &t)
{
	//qDebug()<<"ScribusQApp: download finished:"<<t;
}

bool ScribusQApp::event(QEvent *event)
{
	switch (event->type()) 
	{
	case QEvent::FileOpen:
		{
			QString filename = static_cast<QFileOpenEvent*>(event)->file();
			if(m_ScCore && m_ScCore->initialized())
			{
				ScribusMainWindow* mw = m_ScCore->primaryMainWindow();
				mw->loadDoc(filename);
			}
			else
			{
				m_filesToLoad.append(filename);
			}
			return true;
		}
	default:
		return QApplication::event(event);
	}
}
