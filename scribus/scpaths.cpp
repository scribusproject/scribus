#include "scpaths.h"
#include "qstring.h"

// On Qt/Mac we need CoreFoundation to discover the location
// of the app bundle.
#ifdef QT_MAC
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
#if defined(QT_MAC)
	// Set up the various app paths to look inside the app bundle
	CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
	CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef,
	                                       kCFURLPOSIXPathStyle);
	const char *pathPtr = CFStringGetCStringPtr(macPath,
	                                       CFStringGetSystemEncoding());
	m_docDir = strdup(QString("%1/Resources/docs/").arg(pathPtr));
	m_iconDir = strdup(QString("%1/Resources/icons/").arg(pathPtr));
	m_sampleDir = strdup(QString("%1/Resources/samples/").arg(pathPtr));
	m_scriptDir = strdup(QString("%1/Resources/scripts/").arg(pathPtr));
	m_templateDir = strdup(QString("%1/Resources/templates/").arg(pathPtr));
	m_libDir = strdup(QString("%1/Resources/lib/").arg(pathPtr));
	m_pluginDir = strdup(QString("%1/Resources/plugins/").arg(pathPtr));
	CFRelease(pluginRef);
	CFRelease(macPath);
#endif // defined(QT_MAC)
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

