#ifndef SCPATHS_H
#define SCPATHS_H

#include <qstring.h>
#include <qstringlist.h>
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

	/** @brief Return path to documentation directory */
	const QString& docDir() const;
	/** @brief Return path to icons directory*/
	const QString& iconDir() const;
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

	/** @brief Return paths to system font directories*/
	static QStringList getSystemFontDirs(void);
	/** @brief Return paths to system icc profiles directories*/
	static QStringList getSystemProfilesDirs(void);

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
	QString m_libDir;
	QString m_pluginDir;
	QString m_sampleScriptDir;
	QString m_scriptDir;
	QString m_templateDir;
	QString m_shareDir;
};
#endif
