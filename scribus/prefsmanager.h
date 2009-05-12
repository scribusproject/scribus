/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#ifndef PREFSMANAGER_H
#define PREFSMANAGER_H

#include <QKeySequence>
#include <QMap>
#include <QObject>
#include <QStringList>

#include "prefsstructs.h"
#include "scribusapi.h"

class PrefsFile;
class ScribusMainWindow;


/**
  * @author Craig Bradney
  * @brief Manage Scribus preferences here, and here alone
  * Start to move the preferences out of scribus.cpp and provide some
  * more features and hide some of the data within this class
  * TODO Move prefsFile in here from scribus.cpp and stop passing it in for convertOldPreferences
  * TODO Lots more :)
  */
class SCRIBUS_API PrefsManager : public QObject
{
Q_OBJECT
public:
	PrefsManager(QObject *parent = 0);
	~PrefsManager();

	/**
	* @brief Returns a pointer to the PrefsManager instance
	* @return A pointer to the PrefsManager instance
	*/
	static PrefsManager* instance();
	/**
	* @brief Deletes the PrefsManager Instance
	* Must be called when PrefsManager is no longer needed.
	*/
	static void deleteInstance();

	void setup();
	/*!
	\brief Initialise the defaults for prefs in this class
	Only set the GUI font stuff up if we have a GUI!!!
	Must be run after: PrefsManager::GetAllFonts()
	*/
	void initDefaults();
	void initDefaultGUIFont(const QFont&);
	void initDefaultCheckerPrefs(CheckerPrefsList* cp);
	void initDefaultActionKeys();
	void applyLoadedShortCuts();
	void initArrowStyles();
	/*!
	\author Craig Bradney
	\date Thu 18 Nov 2004
	\brief Set the user's preference file location. Rename any existing old preferences files
	\retval QString Location of the user's preferences
	*/
	QString setupPreferencesLocation();
	/*!
	\author Craig Bradney
	\date Sun 09 Jan 2005
	\brief Copy 1.2 prefs XML before loading, and copy rc files we don't yet convert
	\retval bool true if prefs were imported
	*/
	bool copyOldPreferences();
	/*!
	\author Craig Ringer
	\date Sun 26 June 2005
	\brief Move and convert 1.2 style preferences to new XML format
	\retval None
	*/
	void convert12Preferences();
	/*!
	\brief Get the prefs location property
	\retval QString Location of the user's preferences
	*/
	const QString preferencesLocation();

	/*! \brief Read the preferences.
	\param fname optional filename with preferences. If is not given,
	the defualt is used. */
	void ReadPrefs(const QString & fname=QString::null);
	void ReadPrefsXML();

	/*! \brief Save the preferences.
	\param fname optional filename with preferences. If is not given,
	the defualt is used. */
	void SavePrefs(const QString & fname=QString::null);
	void SavePrefsXML();

	/*! \brief Writes the preferences physically to the file.
	Here is the XML file created itself.
	\param ho a file name to write
	\retval bool true on success, false on write error */
	bool WritePref(QString ho);
	/*! \brief Reads the preferences physically from the file.
	Here is the XML file parsed itself. Returns false on error. 
	It's the caller's job to make sure the prefs file actually exists.
	\param ho a file name to write
	\retval bool true on success, false on write error */
	bool ReadPref(QString ho);
	//! \brief Set up the main window with prefs values, recent files list, main window size etc
	void setupMainWindow(ScribusMainWindow* mw);
	void setGhostscriptExecutable(const QString&);
	void setImageEditorExecutable(const QString&);
	void setExtBrowserExecutable(const QString&);
	void setLatexConfigs(const QStringList&);
	void setLatexCommands(const QMap<QString, QString>& commands) { appPrefs.latexCommands=commands; }
	void setLatexEditorExecutable(const QString&);
	QString ghostscriptExecutable() const {return appPrefs.gs_exe;};
	QString imageEditorExecutable() const {return appPrefs.imageEditorExecutable;};
	QString extBrowserExecutable() const {return appPrefs.extBrowserExecutable;};
	QStringList latexConfigs() const {return appPrefs.latexConfigs;}
	QMap<QString, QString> latexCommands() const {return appPrefs.latexCommands;}
	QString latexEditorExecutable() const {return appPrefs.latexEditorExecutable;}
	bool latexStartWithEmptyFrames() const {return appPrefs.latexStartWithEmptyFrames;}
	//! \brief Get the users preferred preview resolution
	int gsResolution();
	int latexResolution() const {return appPrefs.latexResolution;}
	bool latexForceDpi() const {return appPrefs.latexForceDpi;}
	//! \brief Get the users preferred document directory
	const QString documentDir();
	void setDocumentDir(const QString& dirname);
	int mouseWheelValue() const;
	//! \brief Get the user set display scale
	double displayScale() const;
	//! \brief Get the GUI language from preferences
	const QString& guiLanguage() const;
	//! \brief Get the GUI style from preferences
	const QString& guiStyle() const;
	const QString& guiSystemStyle() const;
	//! \brief Get the GUI style from preferences
	const int& guiFontSize() const;
	const int& paletteFontSize() const;
	void setKeyEntry(const QString&, const QString&, const QKeySequence&, const int&);
	void setShowStartupDialog(const bool);
	bool showPageShadow() const;
	//! \brief Sets the preferences' color set
	void setColorSet(const ColorList&);
	//! \brief Sets the preferences' color set name
	void setColorSetName(const QString&);
	//! \brief Returns the preferences' color set
	const ColorList& colorSet();
	//! \brief Returns a pointer to the preferences' color set. Needed for now until colors are better defined
	ColorList* colorSetPtr();
	//! \brief Returns the preferences' color set name
	const QString& colorSetName();
	/*! \brief Return if a color belongs to tools prefs colors */
	bool isToolColor(const QString& name);
	static bool isToolColor(const struct toolPrefs& settings, const QString& name);
	/*! \brief Return the list of used colors in tool prefs */
	QStringList toolColorNames();
	static QStringList toolColorNames(const struct toolPrefs& settings);
	/*! \brief Replace used colors in tool prefs */
	void replaceToolColors(const QMap<QString, QString> replaceMap);
	static void replaceToolColors(struct toolPrefs& settings, const QMap<QString, QString> replaceMap);
	/*! \brief Finds the fonts on the system
	Must be run after: PrefsManager::setup()
	Must be run before: PrefsManager::initDefaults()
	\param showFontInfo flag to get extended fotn info
	\retval bool true on finding 1 or more fonts */
	bool GetAllFonts(bool showFontInfo);

	ApplicationPrefs* applicationPrefs();
	PrefsFile* applicationPrefsFile();
	bool importingFrom12x();

	//! \brief Return the last error reported by a PrefsManager method
	const QString & lastError() const;

	/*! \brief Display a GUI alert informing the user that saving prefs failed.
	This might want to move to another class later, perhaps being triggered
	by a signal emitted here. */
	void alertSavePrefsFailed() const;
	void alertLoadPrefsFailed() const;

	//! \brief Temporarily public while this class takes shape so progress can happen and cvs can build
	struct ApplicationPrefs appPrefs;
	PrefsFile* prefsFile;
private:
	/**
	* @brief The only instance of PrefsManager available.
	*
	* PrefsManager is singleton and the instance can be queried with the method
	* instance().
	*/
	static PrefsManager* _instance;

	QString prefsLocation;
	bool importingFrom12;
	bool firstTimeIgnoreOldPrefs;

	/*! \brief The last error message generated by a method of this class.
	Do not write "success" messages to this. */
	QString m_lastError;

signals:
	void prefsChanged();
};

#endif
