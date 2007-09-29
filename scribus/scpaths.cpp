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

#include "scconfig.h"

// On Qt/Mac we need CoreFoundation to discover the location
// of the app bundle.
#ifdef Q_WS_MAC
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#endif

#ifdef _WIN32
const char ScPaths::envPathSeparator = ';';
#else
const char ScPaths::envPathSeparator = ':';
#endif

// Init the singleton's "self" address to NULL
ScPaths* ScPaths::m_instance = NULL;

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
	if (ScPaths::m_instance)
		delete ScPaths::m_instance;
}

// Protected "real" constructor
// All paths are initialized to compile-time defaults passed in
// as preprocessor macros and set by autoconf.
ScPaths::ScPaths() :
	m_docDir(DOCDIR),
	m_iconDir(ICONDIR),
	m_libDir(LIBDIR),
	m_pluginDir(PLUGINDIR),
	m_sampleScriptDir(SAMPLESDIR),
	m_scriptDir(SCRIPTSDIR),
	m_templateDir(TEMPLATEDIR),
	m_shareDir(SHAREDIR)
{
// On MacOS/X, override the compile-time settings with a location
// obtained from the system.
#ifdef Q_WS_MAC
	// Set up the various app paths to look inside the app bundle
	CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
	CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef,
	                                       kCFURLPOSIXPathStyle);
	const char *pathPtr = CFStringGetCStringPtr(macPath,
	                                       CFStringGetSystemEncoding());

	// make sure we get the Scribus.app directory, not some subdir

	// strip trailing '/':
	char *p = const_cast<char*>(pathPtr + strlen(pathPtr) - 1);
	while (*p == '/')
		--p;
	++p;
	*p = '\0';
	if (strcmp("/bin", p-4) == 0) {
		p -= 4;
		*p = '\0';
	}
	if (strcmp("/MacOS", p-6) == 0) {
		p -= 6;
		*p = '\0';
	}
	if (strcmp("/Contents", p-9) == 0) {
		p -= 9;
		*p = '\0';
	}

	qDebug() << QString("scpaths: bundle at %1:").arg(pathPtr);
	m_shareDir = QString("%1/Contents/share/scribus/").arg(pathPtr);
	m_docDir = QString("%1/Contents/share/scribus/doc/").arg(pathPtr);
	m_iconDir = QString("%1/Contents/share/scribus/icons/").arg(pathPtr);
	m_sampleScriptDir = QString("%1/Contents/share/scribus/samples/").arg(pathPtr);
	m_scriptDir = QString("%1/Contents/share/scribus/scripts/").arg(pathPtr);
	m_templateDir = QString("%1/Contents/share/scribus/templates/").arg(pathPtr);
	m_libDir = QString("%1/Contents/lib/scribus/").arg(pathPtr);
	m_pluginDir = QString("%1/Contents/lib/scribus/plugins/").arg(pathPtr);
	QApplication::setLibraryPaths(QStringList(QString("%1/Contents/lib/qtplugins/").arg(pathPtr)));
	CFRelease(pluginRef);
	CFRelease(macPath);

	// on OSX this goes to the sys console, so user only sees it when they care -- AV
	qDebug() << QString("scpaths: doc dir=%1").arg(m_docDir);
	qDebug() << QString("scpaths: icon dir=%1").arg(m_iconDir);
	qDebug() << QString("scpaths: font dir=%1").arg(m_fontDir);
	qDebug() << QString("scpaths: sample dir=%1").arg(m_sampleScriptDir);
	qDebug() << QString("scpaths: script dir=%1").arg(m_scriptDir);
	qDebug() << QString("scpaths: template dir=%1").arg(m_templateDir);
	qDebug() << QString("scpaths: lib dir=%1").arg(m_libDir);
	qDebug() << QString("scpaths: plugin dir=%1").arg(m_pluginDir);
	qDebug() << QString("scpaths: qtplugins=%1").arg(QApplication::libraryPaths().join(":"));

#elif defined(_WIN32)
	QString appPath = qApp->applicationDirPath();
	m_shareDir = QString("%1/share/").arg(appPath);
	m_docDir = QString("%1/share/doc/").arg(appPath);
	m_fontDir = QString("%1/share/fonts/").arg(appPath);
	m_iconDir = QString("%1/share/icons/").arg(appPath);
	m_sampleScriptDir = QString("%1/share/samples/").arg(appPath);
	m_scriptDir = QString("%1/share/scripts/").arg(appPath);
	m_templateDir = QString("%1/share/templates/").arg(appPath);
	m_libDir = QString("%1/libs/").arg(appPath);
	m_pluginDir = QString("%1/plugins/").arg(appPath);
	QApplication::setLibraryPaths( QStringList(QString("%1/qtplugins/").arg(appPath)) );
#endif
	
// 	if(!m_shareDir.endsWith("/"))        m_shareDir += "/";
// 	if(!m_docDir.endsWith("/"))          m_docDir += "/";
// 	if(!m_fontDir.endsWith("/"))         m_fontDir += "/";
	if(!m_iconDir.endsWith("/"))         m_iconDir += "/";
// 	if(!m_sampleScriptDir.endsWith("/")) m_sampleScriptDir += "/";
// 	if(!m_scriptDir.endsWith("/"))       m_scriptDir += "/";
// 	if(!m_templateDir.endsWith("/"))     m_templateDir += "/";
// 	if(!m_libDir.endsWith("/"))          m_libDir += "/";
// 	if(!m_pluginDir.endsWith("/"))       m_pluginDir += "/";
}

ScPaths::~ScPaths() {};

QString ScPaths::separatorsToSlashes(const QString& path)
{
	QString qtPath(path);
	qtPath.replace( QDir::separator(), '/' );
	return qtPath;
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

const QString&  ScPaths::scriptDir() const
{
	return m_scriptDir;
}

const QString&  ScPaths::templateDir() const
{
	return m_templateDir;
}

const QString&  ScPaths::shareDir() const
{
	return m_shareDir;
}

QStringList ScPaths::getSystemFontDirs(void)
{
	QStringList fontDirs;
#ifdef Q_OS_MAC
	fontDirs.append(QDir::homePath() + "/Library/Fonts/");
	fontDirs.append("/Library/Fonts/");
	fontDirs.append("/Network/Library/Fonts/");
	fontDirs.append("/System/Library/Fonts/");
#elif defined(_WIN32)
	fontDirs.append( getSpecialDir(CSIDL_FONTS) );
#endif
	return fontDirs;
}

QStringList ScPaths::getSystemProfilesDirs(void)
{
	QStringList iccProfDirs;
#ifdef Q_OS_MAC
	iccProfDirs.append(QDir::homePath()+"/Library/ColorSync/Profiles/");
	iccProfDirs.append("/System/Library/ColorSync/Profiles/");
	iccProfDirs.append("/Library/ColorSync/Profiles/");
#elif defined(Q_WS_X11)
	iccProfDirs.append(QDir::homePath()+"/color/icc/");
	iccProfDirs.append(QDir::homePath()+"/.color/icc/");
	iccProfDirs.append("/usr/share/color/icc/");
	iccProfDirs.append("/usr/local/share/color/icc/");
#elif defined(_WIN32)
	// On Windows it's more complicated, profiles location depends on OS version
	WCHAR sysDir[MAX_PATH + 1];
	OSVERSIONINFO osVersion;
	ZeroMemory( &osVersion, sizeof(OSVERSIONINFO));
	osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); // Necessary for GetVersionEx to succeed
	GetVersionEx(&osVersion);  // Get Windows version infos
	GetSystemDirectoryW( sysDir, MAX_PATH ); // getSpecialDir(CSIDL_SYSTEM) fails on Win9x
	QString winSysDir = QString::fromUtf16((const ushort*) sysDir);
	winSysDir = winSysDir.replace('\\','/');
	if( osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT	) // Windows NT/2k/XP
	{
		if( osVersion.dwMajorVersion >= 5 ) // for 2k and XP dwMajorVersion == 5 
			iccProfDirs.append( winSysDir + "/Spool/Drivers/Color/");
	}
	else if( osVersion.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ) // Windows 9x/Me 
	{
		if( osVersion.dwMajorVersion >= 4 && osVersion.dwMinorVersion >= 10) // Win98 or WinMe
			iccProfDirs.append( winSysDir + "/Color/");
	}
#endif
	return iccProfDirs;
}

QStringList ScPaths::getSystemCreateSwatchesDirs(void)
{
	QStringList createDirs;
#ifdef Q_OS_MAC
	createDirs.append(QDir::homePath()+"/create/swatches/");
	createDirs.append(QDir::homePath()+"/.create/swatches/");
#elif defined(Q_WS_X11)
	createDirs.append(QDir::homePath()+"/create/swatches/");
	createDirs.append(QDir::homePath()+"/.create/swatches/");
	createDirs.append("/usr/share/create/swatches/");
	createDirs.append("/usr/local/share/create/swatches/");
#elif defined(_WIN32)
	QString localAppData = getSpecialDir(CSIDL_LOCAL_APPDATA);
	QString commonAppData = getSpecialDir(CSIDL_COMMON_APPDATA);
	QString programFilesCommon = getSpecialDir(CSIDL_PROGRAM_FILES_COMMON);
	createDirs.append(getSpecialDir(CSIDL_APPDATA) + "create/swatches/");
	if ( !localAppData.isEmpty() )
		createDirs.append(localAppData + "create/swatches/");
	if ( !commonAppData.isEmpty() )
		createDirs.append(commonAppData + "create/swatches/");
	if ( !programFilesCommon.isEmpty() )
		createDirs.append(programFilesCommon + "create/swatches/");
#endif
	return createDirs;
}

QString ScPaths::getApplicationDataDir(void)
{
#if defined(_WIN32)
	QString appData = getSpecialDir(CSIDL_APPDATA);
	if (QDir(appData).exists())
		return (appData + "/Scribus/");
#endif
	return (QDir::homePath() + "/.scribus/");
}

QString ScPaths::getUserDocumentDir(void)
{
#if defined(_WIN32)
	QString userDocs = getSpecialDir(CSIDL_PERSONAL);
	if	(QDir(userDocs).exists())
		return userDocs;
#endif
	return (QDir::homePath() + "/");
}

QString ScPaths::getTempFileDir(void)
{
#if defined(_WIN32)
	QString tempPath;
	WCHAR wTempPath[1024];
	DWORD result = GetTempPathW(1024, wTempPath);
	if ( result )
	{
		tempPath = QString::fromUtf16((const unsigned short*) wTempPath);
		tempPath.replace( '\\', '/' );
		tempPath += "/";
		// GetTempPath may return Windows directory, better not use this one
		// for temporary files
		if (QDir(tempPath).exists() && tempPath != getSpecialDir(CSIDL_WINDOWS))
			return tempPath;
	}
	return getApplicationDataDir();
#else
	return (QDir::homePath() + "/.scribus/");
#endif
}

QString ScPaths::getSpecialDir(int folder)
{
	QString qstr;
#if defined(_WIN32)
	WCHAR dir[256];
	if ( SHGetSpecialFolderPathW(NULL, dir, folder , false) )
	{
		qstr = QString::fromUtf16((const unsigned short*) dir);
		if( !qstr.endsWith("\\") )
			qstr += "\\";
		qstr.replace( '\\', '/' );
	}
#else
	Q_ASSERT(false);
#endif
	return qstr;
}
