#include "scpaths.h"
#include <qapplication.h>

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif


// On Qt/Mac we need CoreFoundation to discover the location
// of the app bundle.
#ifdef BUILD_MAC_BUNDLE
#include <CoreFoundation/CoreFoundation.h>
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
	m_templateDir(TEMPLATEDIR)
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
#endif // defined(BUILD_MAC_BUNDLE)
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

