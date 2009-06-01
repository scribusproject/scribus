/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCPATHS_H
#define SCPATHS_H

#include <QString>
#include <QStringList>
#include "scribusapi.h"

class SCRIBUS_API ScPaths
{
public:
	/**
	 * @brief Get a reference to the ScPaths instance
	 *        creating it if necessary.
	 * @sa ScPaths::destroy()
	 */
	static const ScPaths& instance();
	/**
	 * @brief Destroy the ScPaths instance, if any.
	 * @sa ScPaths::instnace()
	 */
	static void destroy();

	/*** The separator used between path list entries in environment variables */
	static const char envPathSeparator;

	/** @brief Return path to documentation directory */
	const QString& docDir() const;
	/** @brief Return path to icons directory*/
	const QString& iconDir() const;
	/** @brief Return path to the Scribus font directory */
	const QString& fontDir() const;
	/** @brief Return path to lib directory containing translations,
	 *         keysets, etc. */
	const QString& libDir() const;
	/** @brief Return path to dir containing plugins. */
	const QString& pluginDir() const;
	/** @brief Return path to dir containing sample Python scripts */
	const QString& sampleScriptDir() const;
	/** @brief Return path to dir containing "official" Python scripts */
	const QString& scriptDir() const;
	/** @brief Return path to containing stock templates */
	const QString& templateDir() const;
	/** @brief Return path to the Scribus share directory */
	const QString& shareDir() const;

	/** @brief Return path to the translations directory */
	QString translationDir() const;
	/** @brief Return path to the dicts directory */
	QString dictDir() const;

	/** @brief Return paths to system font directories*/
	static QStringList getSystemFontDirs(void);
	/** @brief Return paths to system icc profiles directories*/
	static QStringList getSystemProfilesDirs(void);
	/** @brief Return paths to system create project directories*/
	static QStringList getSystemCreateSwatchesDirs(void);

	/** @brief Return path to application data dir*/
	static QString getApplicationDataDir(void);
	/** @brief Return path to plugin data dir*/
	static QString getPluginDataDir(void);
	/** @brief Return path to user documents*/
	static QString getUserDocumentDir(void);
	/** @brief Return path to directory used for temporary files*/
	static QString getTempFileDir(void);
	/** @brief Return path to Contents OSX subdirectory*/
	QString bundleDir(void) const;

protected:
	/** @brief Constructor. Use ScPaths::instance() instead. */
	ScPaths();
	/** @brief Destructor. Use ScPaths::destroy() instead. */
	~ScPaths();

	/** @brief Return path to a special directory, should be used only on Windows*/
	static QString getSpecialDir(int folder);

	/** @brief Pointer to existing instance of ScPaths, if any. */
	static ScPaths* m_instance;

	// Members to hold system paths
	QString m_docDir;
	QString m_iconDir;
	QString m_fontDir;
	QString m_libDir;
	QString m_pluginDir;
	QString m_sampleScriptDir;
	QString m_scriptDir;
	QString m_templateDir;
	QString m_shareDir;
};
#endif
