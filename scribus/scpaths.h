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
	typedef enum {Hyph, Spell} DictType;

	/**
	 * @brief Get a reference to the ScPaths instance
	 *        creating it if necessary.
	 * @sa ScPaths::destroy()
	 */
	static const ScPaths& instance();
	/**
	 * @brief Destroy the ScPaths instance, if any.
	 * @sa ScPaths::instance()
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
	/** @brief Return path to lib directory containing translations, keysets, etc. */
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
	/** @brief Return path to the Scribus QML directory */
	const QString& qmlDir() const;

	/** @brief Return path to the translations directory */
	QString translationDir() const;
	/** @brief Return path to the dicts directory */
	QString dictDir() const;
	/** @brief Return path to the spelling dictionary directory */
	QStringList spellDirs() const;
	/** @brief Return path to the hyphenation dictionary directory */
	QStringList hyphDirs() const;

	/** @brief Return paths to system font directories*/
	static QStringList systemFontDirs(void);
	/** @brief Return paths to system icc profiles directories*/
	static QStringList systemProfilesDirs(void);
	/** @brief Return paths to system create project directories*/
	static QStringList systemCreatePalettesDirs(void);
	/** @brief Return paths to directories held in an environment variable*/
	static QStringList dirsFromEnvVar(const QString envVar, const QString dirToFind);
	/** @brief Return path to application prefs dir*/
	static QString preferencesDir(bool createIfNotExists = false);
	/** @brief Return path to application data dir*/
	static QString applicationDataDir(bool createIfNotExists = false);
	/** @brief Return path to pre XDG/QStandardPaths application data dir*/
	static QString oldApplicationDataDir(void);
	/** @brief Return path to application data dir for downloaded hyph dictionaries*/
	static QString userDictDir(ScPaths::DictType dictType, bool createIfNotExists);
	/** @brief Return path to application data dir for downloaded fonts*/
	static QString userFontDir(bool createIfNotExists);
	/** @brief Return path to application data dir for downloaded docs*/
	static QString userHelpFilesDir(bool createIfNotExists);
	/** @brief Return path to application data dir for downloaded palettes */
	static QString userPaletteFilesDir(bool createIfNotExists);
	/** @brief Return path to user template dir */
	static QString userTemplateDir(bool createIfNotExists);
	/** @brief Return path to image cache dir*/
	static QString imageCacheDir(void);
	/** @brief Return path to plugin data dir*/
	static QString pluginDataDir(bool createIfNotExists);
	/** @brief Return path to user documents*/
	static QString userDocumentDir(void);
	/** @brief Return path to scrapbook dir*/
	static QString scrapbookDir(bool createIfNotExists);
	/** @brief Return path to directory used for temporary files*/
	static QString tempFileDir(void);
	/** @brief Return path to directory used for downloaded (permanent) files*/
	static QString downloadDir(void);
	/** @brief Return path to Contents OSX subdirectory*/
	QString bundleDir(void) const;


	/** @brief Until we get enough of these, lets have this here for now, gs stuff is in util_ghostscript */
	static QString defaultImageEditorApp();

protected:
	/** @brief Constructor. Use ScPaths::instance() instead. */
	ScPaths();
	/** @brief Destructor. Use ScPaths::destroy() instead. */
	~ScPaths();

	/** @brief Return path to a special directory, should be used only on Windows*/
	static QString windowsSpecialDir(int folder);

	/** @brief Pointer to existing instance of ScPaths, if any. */
	static ScPaths* m_instance;

	// Members to hold system paths
	QString m_docDir;
	QString m_fontDir;
	QString m_iconDir;
	QString m_libDir;
	QString m_pluginDir;
	QString m_qmlDir;
	QString m_sampleScriptDir;
	QString m_scriptDir;
	QString m_shareDir;
	QString m_templateDir;
};
#endif
