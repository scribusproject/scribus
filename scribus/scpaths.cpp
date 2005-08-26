#include "scpaths.h"
#include <qapplication.h>
#include <qdir.h>

#include "scconfig.h"

// On Qt/Mac we need CoreFoundation to discover the location
// of the app bundle.
#ifdef BUILD_MAC_BUNDLE
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
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
#ifdef BUILD_MAC_BUNDLE
	// Set up the various app paths to look inside the app bundle
	CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
	CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef,
	                                       kCFURLPOSIXPathStyle);
	const char *pathPtr = CFStringGetCStringPtr(macPath,
	                                       CFStringGetSystemEncoding());
	char *p = pathPtr + strlen(pathPtr) -1;
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

//	qDebug(QString("scpaths: bundle at %1:").arg(pathPtr));
	m_shareDir = strdup(QString("%1/Contents/share/scribus/").arg(pathPtr));
	m_docDir = strdup(QString("%1/Contents/share/scribus/doc/").arg(pathPtr));
	m_iconDir = strdup(QString("%1/Contents/share/scribus/icons/").arg(pathPtr));
	m_sampleScriptDir = strdup(QString("%1/Contents/share/scribus/samples/").arg(pathPtr));
	m_scriptDir = strdup(QString("%1/Contents/share/scribus/scripts/").arg(pathPtr));
	m_templateDir = strdup(QString("%1/Contents/share/scribus/templates/").arg(pathPtr));
	m_libDir = strdup(QString("%1/Contents/lib/scribus/").arg(pathPtr));
	m_pluginDir = strdup(QString("%1/Contents/lib/scribus/plugins/").arg(pathPtr));
	QApplication::setLibraryPaths(QString("%1/Contents/lib/qtplugins/").arg(pathPtr));
	CFRelease(pluginRef);
	CFRelease(macPath);
/*
	qDebug(QString("scpaths: doc dir=%1").arg(m_docDir));
	qDebug(QString("scpaths: icon dir=%1").arg(m_iconDir));
	qDebug(QString("scpaths: sample dir=%1").arg(m_sampleScriptDir));
	qDebug(QString("scpaths: script dir=%1").arg(m_scriptDir));
	qDebug(QString("scpaths: template dir=%1").arg(m_templateDir));
	qDebug(QString("scpaths: lib dir=%1").arg(m_libDir));
	qDebug(QString("scpaths: plugin dir=%1").arg(m_pluginDir));
	qDebug(QString("scpaths: qtplugins=%1").arg(QApplication::libraryPaths().join(":")));
*/
#elif defined(_WIN32)
	QString appPath = qApp->applicationDirPath();
	m_shareDir = strdup(QString("%1/share/").arg(appPath));
	m_docDir = strdup(QString("%1/share/doc/").arg(appPath));
	m_iconDir = strdup(QString("%1/share/icons/").arg(appPath));
	m_sampleScriptDir = strdup(QString("%1/share/samples/").arg(appPath));
	m_scriptDir = strdup(QString("%1/share/scripts/").arg(appPath));
	m_templateDir = strdup(QString("%1/share/templates/").arg(appPath));
	m_libDir = strdup(QString("%1/libs/").arg(appPath));
	m_pluginDir = strdup(QString("%1/plugins/").arg(appPath));
#endif
}

ScPaths::~ScPaths() {};

const QString&  ScPaths::docDir() const
{
	return m_docDir;
}

const QString&  ScPaths::iconDir() const
{
	return m_iconDir;
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
#ifdef BUILD_MAC_BUNDLE
	fontDirs.append(QDir::homeDirPath() + "/Library/Fonts/");
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
#ifdef BUILD_MAC_BUNDLE
	iccProfDirs.append(QDir::homeDirPath()+"/Library/ColorSync/Profiles/");
	iccProfDirs.append("/System/Library/ColorSync/Profiles/");
	iccProfDirs.append("/Library/ColorSync/Profiles/");
#elif defined(Q_WS_X11)
	iccProfDirs.append(QDir::homeDirPath()+"/color/icc/");
	iccProfDirs.append(QDir::homeDirPath()+"/.color/icc/");
	iccProfDirs.append("/usr/share/color/icc/");
#elif defined(_WIN32)
	// On Windows it's more complicated, profiles location depends on OS version
	char sysDir[MAX_PATH + 1];
	OSVERSIONINFO osVersion;
	ZeroMemory( &osVersion, sizeof(OSVERSIONINFO));
	osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); // Necessary for GetVersionEx to succeed
	GetVersionEx(&osVersion);  // Get Windows version infos
	GetSystemDirectory( sysDir, MAX_PATH ); // getSpecialDir(CSIDL_SYSTEM) fails on Win9x
	QString winSysDir = QString(sysDir).replace('\\','/');
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

QString ScPaths::getSpecialDir(int folder)
{
	QString qstr;
#if defined(_WIN32)
	char dir[256];
	SHGetSpecialFolderPath(NULL, dir, folder , false);
	qstr = dir;
	if( !qstr.endsWith("\\") )
		qstr += "\\";
	qstr.replace( '\\', '/' );
#else
	Q_ASSERT(false);
#endif
	return qstr;
}
