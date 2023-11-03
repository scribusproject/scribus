/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scpaths.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>

#include "prefsmanager.h"
#include "scconfig.h"
#include "scribusapp.h"

// On Qt/Mac we need CoreFoundation to discover the location
// of the app bundle.
#ifdef Q_OS_MACOS
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <versionhelpers.h>
#endif

#if defined(Q_OS_WIN32) || defined (Q_OS_OS2)
const char ScPaths::envPathSeparator = ';';
#else
const char ScPaths::envPathSeparator = ':';
#endif

// Init the singleton's "self" address to nullptr
ScPaths* ScPaths::m_instance = nullptr;

// Singleton's public constructor
const ScPaths& ScPaths::instance()
{
	if (!ScPaths::m_instance)
		ScPaths::m_instance = new ScPaths();
	return *ScPaths::m_instance;
}

// Singleton's public destructor
void ScPaths::destroy()
{
	delete ScPaths::m_instance;
}

// Protected "real" constructor
ScPaths::ScPaths()
{
// On *nix, all paths are initialized to compile-time defaults passed in
// as preprocessor macros and set by autoconf.
#if !defined(Q_OS_MACOS) && !defined(_WIN32) && defined(WANT_RELOCATABLE)
	QString appPath = qApp->applicationDirPath();
	m_docDir = appPath + "/../" + QString(DOCDIR);
	m_iconDir = appPath + "/../" + QString(ICONDIR);
	m_libDir = appPath + "/../" + QString(LIBDIR);
	m_pluginDir = appPath + "/../" + QString(PLUGINDIR);
	m_qmlDir = appPath + "/../" + QString(QMLDIR);
	m_sampleScriptDir = appPath + "/../" + QString(SAMPLESDIR);
	m_scriptDir = appPath + "/../" + QString(SCRIPTSDIR);
	m_shareDir = appPath + "/../" + QString(SHAREDIR);
	m_templateDir = appPath + "/../" + QString(TEMPLATEDIR);
#elif !defined(Q_OS_MACOS) && !defined(_WIN32)
	m_docDir = QString(DOCDIR);
	m_iconDir = QString(ICONDIR);
	m_libDir = QString(LIBDIR);
	m_pluginDir = QString(PLUGINDIR);
	m_qmlDir = QString(QMLDIR);
	m_sampleScriptDir = QString(SAMPLESDIR);
	m_scriptDir = QString(SCRIPTSDIR);
	m_shareDir = QString(SHAREDIR);
	m_templateDir = QString(TEMPLATEDIR);
#endif

// On MacOS/X, override the compile-time settings with a location
// obtained from the system.
#ifdef Q_OS_MACOS
	QString pathPtr(bundleDir());
	qDebug() << QString("scpaths: bundle at %1").arg(pathPtr);
	m_shareDir = QString("%1/Contents/share/scribus/").arg(pathPtr);
	m_docDir = QString("%1/Contents/share/doc/scribus/").arg(pathPtr);
	m_fontDir = QString("%1/Contents/share/scribus/fonts/").arg(pathPtr);
	m_iconDir = QString("%1/Contents/share/scribus/icons/").arg(pathPtr);
	m_sampleScriptDir = QString("%1/Contents/share/scribus/samples/").arg(pathPtr);
	m_scriptDir = QString("%1/Contents/share/scribus/scripts/").arg(pathPtr);
	m_templateDir = QString("%1/Contents/share/scribus/templates/").arg(pathPtr);
	m_libDir = QString("%1/Contents/lib/").arg(pathPtr);
	m_pluginDir = QString("%1/Contents/lib/").arg(pathPtr);
	m_qmlDir = QString("%1/Contents/share/scribus/qml/").arg(pathPtr);
	//QApplication::setLibraryPaths(QStringList(QString("%1/Contents/lib/qtplugins/").arg(pathPtr)));
	QApplication::addLibraryPath(QString("%1/Contents/PlugIns/").arg(pathPtr));
	// on OSX this goes to the sys console, so user only sees it when they care -- AV
	qDebug() << QString("scpaths: doc dir=%1").arg(m_docDir);
	qDebug() << QString("scpaths: icon dir=%1").arg(m_iconDir);
	qDebug() << QString("scpaths: font dir=%1").arg(m_fontDir);
	qDebug() << QString("scpaths: sample dir=%1").arg(m_sampleScriptDir);
	qDebug() << QString("scpaths: script dir=%1").arg(m_scriptDir);
	qDebug() << QString("scpaths: template dir=%1").arg(m_templateDir);
	qDebug() << QString("scpaths: lib dir=%1").arg(m_libDir);
	qDebug() << QString("scpaths: plugin dir=%1").arg(m_pluginDir);
	qDebug() << QString("scpaths: QML dir=%1").arg(m_qmlDir);
	qDebug() << QString("scpaths: qtplugins=%1").arg(QApplication::libraryPaths().join(":"));

#elif defined(_WIN32)
	QFileInfo appInfo(QCoreApplication::applicationDirPath());
	QString appPath = QCoreApplication::applicationDirPath();
	QString cleanAppPath = appInfo.canonicalFilePath();
	if (!cleanAppPath.isEmpty())
		appPath = cleanAppPath;

	m_shareDir = QString("%1/share/").arg(appPath);
	m_docDir = QString("%1/share/doc/").arg(appPath);
	m_fontDir = QString("%1/share/fonts/").arg(appPath);
	m_iconDir = QString("%1/share/icons/").arg(appPath);
	m_sampleScriptDir = QString("%1/share/samples/").arg(appPath);
	m_scriptDir = QString("%1/share/scripts/").arg(appPath);
	m_templateDir = QString("%1/share/templates/").arg(appPath);
	m_libDir = QString("%1/libs/").arg(appPath);
	m_pluginDir = QString("%1/plugins/").arg(appPath);
	m_qmlDir = QString("%1/share/qml/").arg(appPath);

	QString qtpluginDir = QString("%1/qtplugins/").arg(appPath);
	if (QDir(qtpluginDir).exists())
		QApplication::setLibraryPaths( QStringList(qtpluginDir) );
#endif
	
// 	if (!m_shareDir.endsWith("/"))        m_shareDir += "/";
// 	if (!m_docDir.endsWith("/"))          m_docDir += "/";
// 	if (!m_fontDir.endsWith("/"))         m_fontDir += "/";
	if (!m_iconDir.endsWith("/"))         m_iconDir += "/";
// 	if (!m_sampleScriptDir.endsWith("/")) m_sampleScriptDir += "/";
// 	if (!m_scriptDir.endsWith("/"))       m_scriptDir += "/";
// 	if (!m_templateDir.endsWith("/"))     m_templateDir += "/";
// 	if (!m_libDir.endsWith("/"))          m_libDir += "/";
// 	if (!m_pluginDir.endsWith("/"))       m_pluginDir += "/";
}

ScPaths::~ScPaths() = default;

QString ScPaths::bundleDir() const
{
	// On MacOS/X, override the compile-time settings with a location
// obtained from the system.
#ifdef Q_OS_MACOS
	// Set up the various app paths to look inside the app bundle
	CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
	CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef, kCFURLPOSIXPathStyle);
	const char *pathPtr = CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding());
	if (pathPtr != nullptr && strlen(pathPtr) > 0)
	{
		// make sure we get the Scribus.app directory, not some subdir
		// strip trailing '/':
		qDebug("Path = %s", pathPtr);
		char *p = const_cast<char*>(pathPtr + strlen(pathPtr) - 1);
		while (*p == '/')
			--p;
		++p;
		*p = '\0';
		if (strcmp("/bin", p-4) == 0)
		{
			p -= 4;
			*p = '\0';
		}
		if (strcmp("/MacOS", p-6) == 0)
		{
			p -= 6;
			*p = '\0';
		}
		if (strcmp("/Contents", p-9) == 0)
		{
			p -= 9;
			*p = '\0';
		}
		QString q_pathPtr(pathPtr);
		CFRelease(pluginRef);
		CFRelease(macPath);
		return q_pathPtr;
	}
	char buf[2048];
	CFStringGetCString (macPath, buf, 2048, kCFStringEncodingUTF8);
	QString q_pathPtr = QString::fromUtf8(buf);
	if (q_pathPtr.endsWith("/bin"))
		q_pathPtr.chop(4);
	if (q_pathPtr.endsWith("/MacOS"))
		q_pathPtr.chop(6);
	if (q_pathPtr.endsWith("/Contents"))
		q_pathPtr.chop(9);
	CFRelease(pluginRef);
	CFRelease(macPath);
	return q_pathPtr;
#endif
	return QString();
}

QString ScPaths::defaultImageEditorApp()
{
#ifdef Q_OS_MACOS
	QString appName("/Applications/GIMP.app");
	QFile app(appName);
	if (app.exists())
		return appName;
#endif
	return "gimp";
}

const QString&  ScPaths::docDir() const
{
	return m_docDir;
}

const QString&  ScPaths::iconDir() const
{
	return m_iconDir;
}

const QString&  ScPaths::fontDir() const
{
	return m_fontDir;
}

const QString&  ScPaths::libDir() const
{
	return m_libDir;
}

const QString&  ScPaths::pluginDir() const
{
	return m_pluginDir;
}

const QString&  ScPaths::sampleScriptDir() const
{
	return m_sampleScriptDir;
}

const QString& ScPaths::scriptDir() const
{
	return m_scriptDir;
}

const QString& ScPaths::templateDir() const
{
	return m_templateDir;
}

const QString& ScPaths::shareDir() const
{
	return m_shareDir;
}

const QString& ScPaths::qmlDir() const
{
	return m_qmlDir;
}

QString ScPaths::translationDir() const
{
	return (m_shareDir + "translations/");
}

QString ScPaths::dictDir() const
{
	return(m_shareDir + "dicts/");
}

QStringList ScPaths::spellDirs() const
{
	QDir d;
	QStringList spellDirs;
	spellDirs.append(userDictDir(ScPaths::Spell, false));
	spellDirs.append(m_shareDir + "dicts/spelling/");
#ifdef Q_OS_MACOS
	QString macPortsPath("/opt/local/share/hunspell/");
	QString finkPath("/sw/share/hunspell/");
	QString osxLibreOfficePath("/Applications/LibreOffice.app/Contents/Resources/extensions");
	QString osxUserLibreOfficePath(QDir::homePath()+"/Applications/LibreOffice.app/Contents/Resources/extensions");
	d.setPath(macPortsPath);
	if (d.exists())
		spellDirs.append(macPortsPath);
	d.setPath(finkPath);
	if (d.exists())
		spellDirs.append(finkPath);
	d.setPath(osxLibreOfficePath);
	if (d.exists())
	{
		QStringList dictDirFilters("dict-*");
		const QStringList dictDirList(d.entryList(dictDirFilters, QDir::Dirs, QDir::Name));
		for (const QString& dir : dictDirList)
			spellDirs.append(osxLibreOfficePath + "/" + dir + "/");
	}
	d.setPath(osxUserLibreOfficePath);
	if (d.exists())
	{
		QStringList dictDirFilters("dict-*");
		const QStringList dictDirList(d.entryList(dictDirFilters, QDir::Dirs, QDir::Name));
		for (const QString& dir : dictDirList)
			spellDirs.append(osxUserLibreOfficePath + "/" + dir + "/");
	}

#elif defined(_WIN32)
	QString windowsLOPath("LibreOffice 3.5/share/extensions");
	QString progFiles = windowsSpecialDir(CSIDL_PROGRAM_FILES);
	d.setPath(progFiles+windowsLOPath);
	if (d.exists())
	{
		QStringList dictDirFilters("dict-*");
		const QStringList dictDirList(d.entryList(dictDirFilters, QDir::Dirs, QDir::Name));
		for (const QString& dir : dictDirList)
			spellDirs.append(progFiles+windowsLOPath + "/" + dir + "/");
	}
#elif defined(Q_OS_LINUX)
	QString linuxLocalPath("/usr/local/share/hunspell/");
	QString linuxHunspellPath("/usr/share/hunspell/");
	QString linuxMyspellPath("/usr/share/myspell/");
	d.setPath(linuxHunspellPath);
	if (d.exists())
		spellDirs.append(linuxHunspellPath);
	d.setPath(linuxMyspellPath);
	if (d.exists())
		spellDirs.append(linuxMyspellPath);
	d.setPath(linuxLocalPath);
	if (d.exists())
		spellDirs.append(linuxLocalPath);
#endif
	return spellDirs;
}

QStringList ScPaths::hyphDirs() const
{
	QDir d;
	QStringList hyphDirs;
	hyphDirs.append(userDictDir(ScPaths::Hyph, false));
	hyphDirs.append(m_shareDir + "dicts/hyph/");
#ifdef Q_OS_MACOS
	QString macPortsPath("/opt/local/share/hunspell/");
	QString finkPath("/sw/share/hunspell/");
	QString osxLibreOfficePath("/Applications/LibreOffice.app/Contents/Resources/extensions");
	QString osxUserLibreOfficePath(QDir::homePath()+"/Applications/LibreOffice.app/Contents/Resources/extensions");
	d.setPath(macPortsPath);
	if (d.exists())
		hyphDirs.append(macPortsPath);
	d.setPath(finkPath);
	if (d.exists())
		hyphDirs.append(finkPath);
	d.setPath(osxLibreOfficePath);
	if (d.exists())
	{
		QStringList dictDirFilters("dict-*");
		const QStringList dictDirList(d.entryList(dictDirFilters, QDir::Dirs, QDir::Name));
		for (const QString& dir : dictDirList)
			hyphDirs.append(osxLibreOfficePath + "/" + dir + "/");
	}
	d.setPath(osxUserLibreOfficePath);
	if (d.exists())
	{
		QStringList dictDirFilters("dict-*");
		const QStringList dictDirList(d.entryList(dictDirFilters, QDir::Dirs, QDir::Name));
		for (const QString& dir : dictDirList)
			hyphDirs.append(osxUserLibreOfficePath + "/" + dir + "/");
	}

#elif defined(_WIN32)
	QString windowsLOPath("LibreOffice 3.5/share/extensions");
	QString progFiles = windowsSpecialDir(CSIDL_PROGRAM_FILES);
	d.setPath(progFiles+windowsLOPath);
	if (d.exists())
	{
		QStringList dictDirFilters("dict-*");
		const QStringList dictDirList(d.entryList(dictDirFilters, QDir::Dirs, QDir::Name));
		for (const QString& dir : dictDirList)
			hyphDirs.append(progFiles+windowsLOPath + "/" + dir + "/");
	}
#elif defined(Q_OS_LINUX)
	QString linuxLocalPath("/usr/local/share/hunspell/");
	QString linuxHunspellPath("/usr/share/hunspell/");
	QString linuxMyspellPath("/usr/share/myspell/");
	QString linuxHyphen1Path("/usr/share/hyphen/");
	d.setPath(linuxHyphen1Path);
	if (d.exists())
		hyphDirs.append(linuxHyphen1Path);
	d.setPath(linuxHunspellPath);
	if (d.exists())
		hyphDirs.append(linuxHunspellPath);
	d.setPath(linuxMyspellPath);
	if (d.exists())
		hyphDirs.append(linuxMyspellPath);
	d.setPath(linuxLocalPath);
	if (d.exists())
		hyphDirs.append(linuxLocalPath);
#endif
	return hyphDirs;
}

QStringList ScPaths::systemFontDirs()
{
	QStringList fontDirs;
#ifdef Q_OS_MACOS
	fontDirs.append(QDir::homePath() + "/Library/Fonts/");
	fontDirs.append("/Library/Fonts/");
	fontDirs.append("/Network/Library/Fonts/");
	fontDirs.append("/System/Library/Fonts/");
#elif defined(_WIN32)
	QDir d;
	QString localFontDir = windowsSpecialDir(CSIDL_LOCAL_APPDATA)+"Microsoft/Windows/Fonts"; // Added by Windows 10 1809
	QString roamingFontDir = windowsSpecialDir(CSIDL_APPDATA)+"Microsoft/Windows/Fonts"; // Added by Windows 10 1809
	d.setPath(localFontDir);
	if (d.exists())
		fontDirs.append(localFontDir);
	d.setPath(roamingFontDir);
	if (d.exists())
		fontDirs.append(roamingFontDir);
	fontDirs.append(windowsSpecialDir(CSIDL_FONTS));
#endif
	return fontDirs;
}

QStringList ScPaths::systemProfilesDirs()
{
	QStringList iccProfDirs;
#ifdef Q_OS_MACOS
	iccProfDirs.append(QDir::homePath()+"/Library/ColorSync/Profiles/");
	iccProfDirs.append("/System/Library/ColorSync/Profiles/");
	iccProfDirs.append("/Library/ColorSync/Profiles/");
#elif defined(Q_OS_LINUX)
	iccProfDirs.append(QDir::homePath()+"/color/icc/");
	iccProfDirs.append(QDir::homePath()+"/.color/icc/");
	iccProfDirs.append(QDir::homePath()+"/.local/share/icc/");
	iccProfDirs.append(QDir::homePath()+"/.local/share/color/icc/");
	iccProfDirs.append("/usr/share/color/icc/");
	iccProfDirs.append("/usr/local/share/color/icc/");
	iccProfDirs.append("/var/lib/color/icc/");
#elif defined(_WIN32)
	WCHAR sysDir[MAX_PATH + 1] = { 0 };
	GetSystemDirectoryW(sysDir, MAX_PATH); // windowsSpecialDir(CSIDL_SYSTEM) fails on Win9x
	QString winSysDir(QString::fromUtf16((const ushort*) sysDir));
	winSysDir.replace('\\','/');
	if (IsWindowsVersionOrGreater(5, 0, 0)) // for 2k and XP dwMajorVersion == 5 
		iccProfDirs.append(winSysDir + "/Spool/Drivers/Color/");
#endif
	return iccProfDirs;
}

QStringList ScPaths::dirsFromEnvVar(const QString& envVar, const QString& dirToFind)
{
	QChar sep(ScPaths::envPathSeparator);
	QStringList dirs;
#if defined(Q_OS_MACOS) || defined(Q_OS_UNIX)
	const QStringList env(QProcess::systemEnvironment());
	QString path_data;
	for (const QString& line : env)
	{
		if (line.indexOf(envVar) == 0)
			path_data = line.mid(envVar.length()+1); //eg, Strip "XDG_DATA_DIRS="
	}
	const QStringList splitpath_data(path_data.split(sep, Qt::SkipEmptyParts));
	for (const QString &dir : splitpath_data)
	{
		QFileInfo info(dir+dirToFind);
		if (info.exists())
			dirs.append(dir+dirToFind);
	}
#endif
	return dirs;
}



QStringList ScPaths::systemCreatePalettesDirs()
{
	QStringList createDirs;
#ifdef Q_OS_MACOS
	createDirs.append(QDir::homePath()+"/create/swatches/");
	createDirs.append(QDir::homePath()+"/.create/swatches/");
#elif defined(Q_OS_LINUX)
	createDirs.append(QDir::homePath()+"/create/swatches/");
	createDirs.append(QDir::homePath()+"/.create/swatches/");
	createDirs.append("/usr/share/create/swatches/");
	createDirs.append("/usr/local/share/create/swatches/");
#elif defined(_WIN32)
	QString localAppData = windowsSpecialDir(CSIDL_LOCAL_APPDATA);
	QString commonAppData = windowsSpecialDir(CSIDL_COMMON_APPDATA);
	QString programFilesCommon = windowsSpecialDir(CSIDL_PROGRAM_FILES_COMMON);
	createDirs.append(windowsSpecialDir(CSIDL_APPDATA) + "create/swatches/");
	if (!localAppData.isEmpty())
		createDirs.append(localAppData + "create/swatches/");
	if (!commonAppData.isEmpty())
		createDirs.append(commonAppData + "create/swatches/");
	if (!programFilesCommon.isEmpty())
		createDirs.append(programFilesCommon + "create/swatches/");
#endif
	return createDirs;
}

QString ScPaths::oldApplicationDataDir()
{
#ifdef Q_OS_WIN32
	QString appData = windowsSpecialDir(CSIDL_APPDATA);
	if (QDir(appData).exists())
#ifdef APPLICATION_DATA_DIR
		return (appData + "/" + APPLICATION_DATA_DIR + "/");
#else
		return (appData + "/Scribus/");
#endif
#endif

#ifdef APPLICATION_DATA_DIR
	return QDir::homePath() + "/" + APPLICATION_DATA_DIR + "/";
#else
	#ifdef Q_OS_MACOS
		return (QDir::homePath() + "/Library/Preferences/Scribus/");
	#else
		return (QDir::homePath() + "/.scribus/");
	#endif
#endif
}

QString ScPaths::applicationDataDir(bool createIfNotExists)
{
	QString dataDir;
#ifdef APPLICATION_DATA_DIR
	dataDir =  QDir::homePath() + "/" + APPLICATION_DATA_DIR + "/";
#else
	dataDir =  QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
#endif
	QDir prefsDirectory(dataDir);
	if (createIfNotExists && !prefsDirectory.exists())
		prefsDirectory.mkpath(prefsDirectory.absolutePath());
	return dataDir;
}

QString ScPaths::preferencesDir(bool createIfNotExists)
{
	//If we have been passed a dir on cmd line, use this. Start up errors if it does not exist.
	if (!ScQApp->userPrefsDir().isEmpty())
		return ScQApp->userPrefsDir();
	QString prefsDir;
#ifdef APPLICATION_CONFIG_DIR
	prefsDir =  QDir::homePath() + "/" + APPLICATION_CONFIG_DIR + "/";
#else
	//Jean wants to make all prefs for Scribus be in the roaming directory on Windows so return the same as applicationDataDir
	#ifdef Q_OS_WIN32
		prefsDir =  QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
	#else
		prefsDir =  QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/";
	#endif
#endif
	QDir prefsDirectory(prefsDir);
	if (createIfNotExists && !prefsDirectory.exists())
		prefsDirectory.mkpath(prefsDirectory.absolutePath());
	return prefsDir;
}

QString ScPaths::imageCacheDir()
{
	return applicationDataDir() + "cache/img/";
}

QString ScPaths::pluginDataDir(bool createIfNotExists)
{
	QDir useFilesDirectory(applicationDataDir() + "plugins/");
	if (createIfNotExists && !useFilesDirectory.exists())
		useFilesDirectory.mkpath(useFilesDirectory.absolutePath());
	return useFilesDirectory.absolutePath() + "/";
}

QString ScPaths::userDictDir(ScPaths::DictType dictType, bool createIfNotExists)
{
	QString dictSuffix;
	if (dictType == ScPaths::Hyph)
		dictSuffix = "hyph/";
	if (dictType == ScPaths::Spell)
		dictSuffix = "spell/";
	QDir useFilesDirectory(applicationDataDir() + "dicts/" + dictSuffix);
	if (createIfNotExists && !useFilesDirectory.exists())
		useFilesDirectory.mkpath(useFilesDirectory.absolutePath());
	return useFilesDirectory.absolutePath() + "/";
}

QString ScPaths::userFontDir(bool createIfNotExists)
{
	QDir useFilesDirectory(applicationDataDir() + "fonts/");
	if (createIfNotExists && !useFilesDirectory.exists())
		useFilesDirectory.mkpath(useFilesDirectory.absolutePath());
	return useFilesDirectory.absolutePath() + "/";
}

QString ScPaths::userHelpFilesDir(bool createIfNotExists)
{
	QDir useFilesDirectory(applicationDataDir() + "helpfiles/");
	if (createIfNotExists && !useFilesDirectory.exists())
		useFilesDirectory.mkpath(useFilesDirectory.absolutePath());
	return useFilesDirectory.absolutePath() + "/";
}

QString ScPaths::userPaletteFilesDir(bool createIfNotExists)
{
	QDir useFilesDirectory(applicationDataDir() + "palettes/");
	if (createIfNotExists && !useFilesDirectory.exists())
	{
		useFilesDirectory.mkpath(useFilesDirectory.absolutePath());
		useFilesDirectory.mkpath(useFilesDirectory.absolutePath() + "/locked");
	}
	return useFilesDirectory.absolutePath() + "/";
}

QString ScPaths::userTemplateDir(bool createIfNotExists)
{
	if (PrefsManager::instance().appPrefs.pathPrefs.documentTemplates.isEmpty())
		return QString();
	QDir useFilesDirectory(PrefsManager::instance().appPrefs.pathPrefs.documentTemplates);
	if (createIfNotExists && !useFilesDirectory.exists())
		useFilesDirectory.mkpath(useFilesDirectory.absolutePath());
	return useFilesDirectory.absolutePath() + "/";
}

QString ScPaths::userDocumentDir()
{
	QString userDocs = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	if (QDir(userDocs).exists())
		return userDocs + "/";
	return QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/";
}

QString ScPaths::scrapbookDir(bool createIfNotExists)
{
	QDir useFilesDirectory(applicationDataDir() + "scrapbook/");
	if (createIfNotExists)
	{
		if (!useFilesDirectory.exists())
			useFilesDirectory.mkpath(useFilesDirectory.absolutePath());
		if (!useFilesDirectory.exists("main"))
			useFilesDirectory.mkpath(useFilesDirectory.absolutePath() + "/main");
		if (!useFilesDirectory.exists("tmp"))
			useFilesDirectory.mkpath(useFilesDirectory.absolutePath() + "/tmp");
	}
	return useFilesDirectory.absolutePath() + "/";
}

QString ScPaths::tempFileDir()
{
#ifdef Q_OS_WIN32
	QString tempPath;
	WCHAR wTempPath[1024];
	DWORD result = GetTempPathW(1024, wTempPath);
	if (result)
	{
		tempPath = QString::fromUtf16((const unsigned short*) wTempPath);
		tempPath.replace('\\', '/');
		tempPath += "/";
		// GetTempPath may return Windows directory, better not use this one
		// for temporary files
		if (QDir(tempPath).exists() && tempPath != windowsSpecialDir(CSIDL_WINDOWS))
			return tempPath;
	}
#endif

	QDir tempAppDirectory(applicationDataDir() + "temp/");
	if (!tempAppDirectory.exists())
		tempAppDirectory.mkpath(tempAppDirectory.absolutePath());
	return tempAppDirectory.absolutePath() + "/";
}

QString ScPaths::downloadDir()
{
	QDir downloadDirectory(applicationDataDir() + "downloads/");
	if (!downloadDirectory.exists())
		downloadDirectory.mkpath(downloadDirectory.absolutePath());
	return downloadDirectory.absolutePath() + "/";
}

QString ScPaths::windowsSpecialDir(int folder)
{
	QString qstr;
#ifdef Q_OS_WIN32
	WCHAR dir[MAX_PATH];
	if (SHGetSpecialFolderPathW(nullptr, dir, folder , false))
	{
		qstr = QString::fromUtf16((const unsigned short*) dir);
		if (!qstr.endsWith("\\"))
			qstr += "\\";
		qstr.replace('\\', '/');
	}
#else
	Q_ASSERT(false);
#endif
	return qstr;
}
